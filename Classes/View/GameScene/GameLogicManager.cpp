/*
 * GameLogicManager.cpp
 *
 *  游戏逻辑管理类
 *  理论上，游戏中数据导向、UI相关都由该对象控制。
 *  Created on: 2015-5-18
 *      Author: diyal.yin
 */

#include "GameLogicManager.h"
#include "cocos2d.h"
#include "Control/WWRequest/MsgGameCommonRequest.h"
#include "Control/WWRequest/MsgLordGameRequest.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "BaseCore/WWBGUI/WWToast.h"
#include "View/Common/SetLayer/SetLayer.h"
#include "View/Common/PokerLogic/PokerLogic.h"
#include "Control/WWCSound/GameSoundManager.h"
#include "View/Common/CommLayer/CommDialog.h"
#include "View/GameScene/layer/GamePlayerInfo.h"
#include "View/LobbyScene/LobbyScene.h"
#include "BaseCore/Common/UICoordConfig.h"
#include "Control/WWDataManager/GameGlobalDataManager.h"
#include "../EffectView/EffectComponent.h"

USING_NS_CC;

static GameLogicManager* g_pGameLogicManager = NULL;

extern void setLaiziNumber(int num);
extern int getLaiziNumber();

GameLogicManager::GameLogicManager() :
	m_pUIgameScene(nullptr),
	m_pUIGameTopMenuLayer(nullptr),
	m_pUIGamePlayerLayer(nullptr),
	m_pUIMyCardLayer(nullptr),
	m_pUIGameOverLayer(nullptr),
	m_pUIToast(nullptr),
	m_pIsGameStart(false),
	m_pIsXuju(false),
	m_pCurOutCardUser(kSiteSelf),
	m_pLastOutCardUser(-1),
	m_pLandlordId(0),
	m_pLeftUserId(0),
	m_pRightUserId(0),
	m_pStepTime(0),
	m_pJiaBeiTime(0),
	m_pMultiple(1),
	m_pGlobelTag(0),
	m_pNoCallCount(0),
	m_gameState(kGameState::State_GameOver)
{

}

GameLogicManager::~GameLogicManager()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	removeToast();
	m_pGlobelTag = 0;
	m_gameState=kGameState::State_GameOver;
	//释放对象指针
	if (g_pGameLogicManager)
	{
		delete g_pGameLogicManager;
		g_pGameLogicManager=nullptr;
	}
}

GameLogicManager* GameLogicManager::getInstance()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	if(!g_pGameLogicManager)
	{
		g_pGameLogicManager = new GameLogicManager();
	}
	return g_pGameLogicManager;
}

/************************************************************************/
/* 请求类成员函数                                                       */
/************************************************************************/
void GameLogicManager::requestEnterGame()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	GameRoom* room = WWDataManager::getInstance()->getGameRoom();
	std::string noFlg = "no";
	MsgGameCommonRequest::enterGame(room, noFlg); //请求开局
}
void GameLogicManager::requestBackGame()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	GameRoom* room = WWDataManager::getInstance()->getGameRoom();
	std::string backFlg = "back";
	MsgGameCommonRequest::enterGame(room, backFlg); //请求恢复对局
}
void GameLogicManager::requestPass()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	m_pUIGamePlayerLayer->setShowPlayCardFrame(false);
	char szResult[2] = {'0',0};
	MsgLordGameRequest::sendTurnOutPoker(szResult); //请求不出
}

void GameLogicManager::requestContinueGame()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	MsgGameCommonRequest::sendContinueGame(); //续局
}

void GameLogicManager::sendQuickStartGame()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	log("GameLogicManager::sendQuickStartGame-------------------current state = %d",getGameState());
	if(m_gameState != kGameState::State_GameOver )//已经开局，不在发送相关消息
	{
		return;
	}
	MsgGameCommonRequest::sendQuickStartGame();//续局，15秒未开局，则发送该消息
}

void GameLogicManager::requestMinPai(const cocos2d::Vector<PokerObject*> &vecPokers)
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	//将自己的牌传给服务器
	kPokerUint_8 szPokerData = PokerUtil::getPokersFromArr(vecPokers);
	PokerUtil::changeDataToSever(szPokerData);

	MsgLordGameRequest::sendSelfMingPai(true, (char*)szPokerData.data); //明牌

	delePokerData(szPokerData);
}

void GameLogicManager::requestNoMinPai()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	MsgLordGameRequest::sendSelfNoMingPai();
	//不明牌时没有协议返回，本地显示出牌工具栏
	showMyPlayBtnFrame();
	removeToast();
	m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_BUMINGPAI_TAG, PlayerSite::kSiteSelf);
	// 重新显示倒计时
	showClockByUserId(UserData::getInstance()->getIntValue(INT_USER_ID));

	setGameState(kGameState::State_PlayCard);
}

/************************************************************************/
/* UI展示类函数                                                         */
/************************************************************************/
void GameLogicManager::dealMyPoker(const std::string myCards, bool isResume)  //玩家发牌
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	if(myCards.size() <= 0){
		return;
	}
	
	//隐藏顶部工具栏任务
	m_pUIGameTopMenuLayer->unShowKeyWin();

	Vector<PokerObject*> addCardByteArr = PokerUtil::getSeverData((uint8_t*) myCards.c_str(), myCards.size());
	m_pUIMyCardLayer->addCard(addCardByteArr);
	if(isResume){
		m_pUIMyCardLayer->showLandLordCard();
	}else{
		m_pUIMyCardLayer->showLandLordCardAnim();
	}
	m_pUIMyCardLayer->setTouchEnabled(true);
}
void GameLogicManager::showMyJDZFrame(int noCall)  //显示玩家叫地主界面
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	m_pUIGamePlayerLayer->setShowLandlordFrame(true, noCall);
}
void GameLogicManager::showMyQDZFrame()  //显示玩家抢地主界面
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	m_pUIGamePlayerLayer->setShowGraplordFrame(true);
}
void GameLogicManager::showMyMinPaiFrame()  //显示玩家明牌界面
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	m_pUIGamePlayerLayer->setShowMingPaiFrame(true);
}
void GameLogicManager::showMyPlayBtnFrame()  //显示玩家局中出牌按钮界面
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	setGameState(kGameState::State_PlayCard);
	m_pUIGamePlayerLayer->setShowPlayCardFrame(true);
	m_pUIMyCardLayer->checkPlayCardBtnState(); //检查出牌工具栏按纽状态
}
void GameLogicManager::showMyJiaBeiFrame() //显示加倍按纽界面
{
	m_pUIGamePlayerLayer->setShowJiaBeiFrame(true);
}
void GameLogicManager::showClockByUserId(int userId) //显示倒计时
{
	int countTime = m_pStepTime;
	if(m_gameState == kGameState::State_JiaBei || m_gameState == kGameState::State_MinPai)
	{
		countTime = m_pJiaBeiTime;
	}
	if (userId == m_pLeftUserId){
		m_pUIGamePlayerLayer->startToCountdown(countTime, PlayerSite::kSiteLeft);
	}else if(userId == m_pRightUserId){
		m_pUIGamePlayerLayer->startToCountdown(countTime, PlayerSite::kSiteRight);
	}else if(userId == -1){
		m_pUIGamePlayerLayer->startToCountdown(countTime, PlayerSite::kSiteCenter);
	}else{
		m_pUIGamePlayerLayer->startToCountdown(countTime, PlayerSite::kSiteSelf);
	}
}

void GameLogicManager::showToast(const std::string &str, float time)
{
	removeToast();
	if(time > 0){
		m_pUIToast = WWToast::createToast(str, time);
	}else{
		m_pUIToast = WWToast::createToast(str, m_pStepTime);
	}
	m_pUIgameScene->openGlobalUI(m_pUIToast);
}

void GameLogicManager::showWaitForStartToast()
{
	std::string waitForStartToastStr = GetWWString(g_Game_WaitFor_Start,g_GameSence_Toast);
	showToast(waitForStartToastStr.c_str(), 30.0f);

	//洗牌动画
	m_pUIgameScene->showWaitAnimation();

	//显示顶部工具栏任务
	m_pUIGameTopMenuLayer->showKeyWin();
}

//显示房间底注
void GameLogicManager::showRoomBaseTx()
{
	m_pUIGameTopMenuLayer->updateBase(m_curRoomBase);
}

void GameLogicManager::clearLastPlayPokers()
{
	m_arrLastPlayPokers.clear();
}

/************************************************************************/
/* 逻辑类函数                                                          */
/************************************************************************/
PlayerSite GameLogicManager::getPlaySite(int userId)
{
	PlayerSite retSite;
	
	if (userId == m_pLeftUserId){
		retSite = PlayerSite::kSiteLeft;
	}else if(userId == m_pRightUserId){
		retSite = PlayerSite::kSiteRight;
	}else if( userId == UserData::getInstance()->getIntValue(INT_USER_ID) ){
		retSite = PlayerSite::kSiteSelf;
	}else{
		retSite = PlayerSite::kSiteSelf;
	}
	return retSite;
}

void GameLogicManager::removeToast()
{
	if(m_pUIToast){
		m_pUIgameScene->closeGlobalUI(m_pUIToast);
		m_pUIToast = nullptr;
	}
}

void GameLogicManager::resetGameInfo()
{
	m_pLandlordId = 0;
	clearLastPlayPokers();
	setLaiziNumber(0);
	setLastOutCardUser(-1);

	if(m_pUIMyCardLayer){
		m_pUIMyCardLayer->reset();
	}

	if(m_pUIGameTopMenuLayer){
		m_pUIGameTopMenuLayer->updateMultiple(0);
	}
	if(m_pUIgameScene)
	{
		m_pUIgameScene->setSubstituteEnable(false);	
		//隐藏癞子选择框
		m_pUIgameScene->removeChooseLaiziTable();
		//隐藏聊天按纽
		m_pUIgameScene->hideChatMenuBtn();
	}
	if(m_pUIGamePlayerLayer){
		m_pUIGamePlayerLayer->reset();
		m_pUIGamePlayerLayer->resetAllData();
		m_pUIGamePlayerLayer->getnpcLeft()->clearOutCard();
		m_pUIGamePlayerLayer->getnpcLeft()->resetAllData();
		m_pUIGamePlayerLayer->getnpcRight()->clearOutCard();
		m_pUIGamePlayerLayer->getnpcRight()->resetAllData();
	}
}

void GameLogicManager::showPlayerInfo(int site)
{
	auto info = GamePlayerInfo::create(site);
	if (site == 0)//自家位置
	{
		info->setPosition(POSITION_SELF[0]);
	}
	else  if (site == 1)//右边  即下家
	{
		info->setPosition(Vec2(POSITION_NEXT[0].x, CUR_SCREEN_SIZE.height - POSITION_NEXT[0].y));
	}
	else//左边 即上家
	{
		info->setPosition(Vec2( - POSITION_LAST[0].x, CUR_SCREEN_SIZE.height - POSITION_LAST[0].y));
	}
	
	m_pUIgameScene->openGlobalUI(info);
}

void GameLogicManager::onClockTimeOut()
{
	//隐藏操作工具条
	m_pUIGamePlayerLayer->hideAllBtnFrame();
	//隐藏癞子选择框
	m_pUIgameScene->removeChooseLaiziTable();
	
	if(!m_pUIMyCardLayer->getIsAutoPlay()){
		//明牌阶段超时，则默认不明牌
		if( getGameState() == kGameState::State_MinPai && getPlaySite(m_pLandlordId) == PlayerSite::kSiteSelf ){
			//requestNoMinPai();
			MsgGameCommonRequest::sendSubstituteRequest(0);
		}else if( getGameState() == kGameState::State_CallLord && getCurOutCardUser() == PlayerSite::kSiteSelf ){
			//MsgLordGameRequest::sendSelfCallLord(false); //自己超时不叫地主
			MsgGameCommonRequest::sendSubstituteRequest(0);
		}else if( getGameState() == kGameState::State_GrapLord && getCurOutCardUser() == PlayerSite::kSiteSelf ){
			//MsgLordGameRequest::sendSelfGrabLord(false); //自己超时不抢地主
			MsgGameCommonRequest::sendSubstituteRequest(0);
		}else if( getGameState() == kGameState::State_JiaBei && getCurOutCardUser() == PlayerSite::kSiteSelf && !m_pUIGamePlayerLayer->getIsRecvJiaBei()){
			log("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxaaaaaaaaaaaaaxxxxxxxxxxxxxxxxxxxxxxxx=");
			//MsgLordGameRequest::sendJiaBei(0); //自己超时不加倍
			//MsgGameCommonRequest::sendSubstituteRequest(0);
		}else if( getGameState() == kGameState::State_PlayCard && getCurOutCardUser() == PlayerSite::kSiteSelf)
		{
			MsgGameCommonRequest::sendSubstituteRequest(0);
		}
		//else if(getCurOutCardUser() == PlayerSite::kSiteSelf){
		//	// just for test. 后面可能变，先参照之前版本
		//	//超时请求托管 
		//	log("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxaaaaaaaaaaaaaxxxxxxxxxxxxxxxxxxxxxxxx=111111111111111111111");
		//	MsgGameCommonRequest::sendSubstituteRequest(0);
		//}

		//MsgGameCommonRequest::sendSubstituteRequest(0);
	}
}

//更新自己手中的癞子牌
void GameLogicManager::updateMyLaiZiCards()
{
	//其他玩家打出的牌是实时创建，不需要更新
	m_pUIMyCardLayer->updateMyLaiZiCards(getLaiziNumber());
}

void GameLogicManager::updateLaiziCards(const Vector<PokerObject*> &vecPlayPokers, const Vector<PokerObject*> &vecReplacePokers)
{
	//拷贝替换后的数组，这里用浅拷贝，不要用深拷贝。
	Vector<PokerObject*> vecTmpReplacePokers = vecReplacePokers;

	//找出替换后的牌中，癞子牌变成的牌
	for (auto poker : vecPlayPokers)
	{
		if (poker)
		{
			for (auto pTmpPoker : vecTmpReplacePokers)
			{
				if (pTmpPoker)
				{
					if (poker->getPokerValue() == pTmpPoker->getPokerValue())
					{
						vecTmpReplacePokers.eraseObject(pTmpPoker);
						break;
					}
				}
			}
		}
	}

	//改变替换后的牌的花色为癞子花色，因为是浅拷贝，所以会对arrReplaceCards中的牌同步修改
	for (auto poker : vecTmpReplacePokers)
	{
		if (poker)
		{
			poker->changColorToLaizi();
		}
	}
}

/************************************************************************/
/* 动画显示类函数                                                       */
/************************************************************************/
void GameLogicManager::showX2Animation()
{
	auto X2Anim = X2Layer::create();
	if(X2Anim){
		X2Anim->setPosition(Vec2(0,0));
		m_pUIgameScene->openGlobalUI(X2Anim);
	}else{
		onX2AnimationEnd();
	}
}

void GameLogicManager::onX2AnimationEnd()
{
	m_pMultiple *= 2;
	m_pUIGameTopMenuLayer->updateMultiple(m_pMultiple);
}

int GameLogicManager::showSpecialCardAnimation(PlayerSite outCardSite, int lastOutCardSite, const cocos2d::Vector<PokerObject*> &vecPlayPokers)
{
	 CCAssert(vecPlayPokers.size() > 0, "");

	 PlayerSite lordSite = getPlaySite(getLandLordId());

	 kWeightValue weightValue = PokerTypeCheck::getPokerType(vecPlayPokers);
	 int type = weightValue.type;
	 int count = weightValue.pokerCount;
	 CCLOG("weightValue.type=%d", type);
	 switch (type)
	 {
	 case T_SEQ_1:   //顺子
		 {
			 //WWAnimationManager::showShunZi((PlayerSite)mySite);
		 }
		 break;
	 case T_SEQ_2:   //连对
		 {
			 //WWAnimationManager::showLianDui(PlayerSite(mySite));
		 }
		 break;
	 case T_SEQ_3:   //三顺
		 {
			 if( outCardSite >= 0 && outCardSite< 3 ){
				 auto planeAnim = FlyChickenLayer::createWithSite(outCardSite);
				 if(planeAnim){
					m_pUIgameScene->openGlobalUI(planeAnim);
				 }
			 }
		 }
		 break;
	 case T_THREE_1: 
		 {
			 if (count > 4)   //飞机
			 {
				 if( outCardSite >= 0 && outCardSite< 3 ){
					 auto planeAnim = FlyChickenLayer::createWithSite(outCardSite);
					 if(planeAnim){
						m_pUIgameScene->openGlobalUI(planeAnim);
					 }
				 }
			 }
			 else             //三带一
			 {
				 //WWAnimationManager::showLianDai(LianDaiType_3_1, (PlayerSite)mySite);
			 }
		 }
		 break;
	 case T_THREE_2:
		 {
			 if (count > 5)  //飞机
			 {
				 if( outCardSite >= 0 && outCardSite< 3 ){
					 auto planeAnim = FlyChickenLayer::createWithSite(outCardSite);
					 if(planeAnim){
						m_pUIgameScene->openGlobalUI(planeAnim);
					 }
				 }
			 }
			 else            //三带二
			 {
				 //WWAnimationManager::showLianDai(LianDaiType_3_2, (PlayerSite)mySite);
			 }
		 }
		 break;
	 case T_FOUR_2:           //四带单,使用同一个动画
		 /*{
		 WWAnimationManager::showLianDai(LianDaiType_4_1, (PlayerSite)mySite);
		 }
		 break;*/
	 case T_FOUR_4:           //四带双
		 {
			 //WWAnimationManager::showLianDai(LianDaiType_4_2, (PlayerSite)mySite);
		 }
		 break;
	 case T_BOMB:            //炸弹
	 case T_BOMB_S:
	 case T_BOMB_L:
		 {
			 if( outCardSite >= 0 && outCardSite< 3 ){
				 if ( outCardSite == lordSite )
				 {
					 PlayerSite site1 = PlayerSite::kSiteLeft;
					 PlayerSite site2 = PlayerSite::kSiteRight;
					 if( outCardSite == PlayerSite::kSiteLeft ){
						 site1 = PlayerSite::kSiteSelf;
						 site2 = PlayerSite::kSiteRight;
					 }else if( outCardSite == PlayerSite::kSiteRight ){
						 site1 = PlayerSite::kSiteSelf;
						 site2 = PlayerSite::kSiteLeft;
					 }
					 auto bombAnim = BombsLayer::createWithSite(outCardSite, site1, site2);
					 if(bombAnim){
						m_pUIgameScene->openGlobalUI(bombAnim);
					 }else{
						showX2Animation();
					 }
				 }
				 else
				 {
					 if( lastOutCardSite >= 0 && lastOutCardSite< 3 ){
						 int finalSite = lastOutCardSite;
						 if( lastOutCardSite == outCardSite){ //玩家当前轮首出炸弹
							 finalSite = lordSite;
						 }
						 auto bombAnim = BombsLayer::createWithSite(outCardSite, finalSite, -1);
						 if(bombAnim){
							m_pUIgameScene->openGlobalUI(bombAnim);
						 }else{
							showX2Animation();
						 }
					 }else{
						showX2Animation();
					 }
				 }
			 }else{
				 showX2Animation();
			 }

		 }
		 break;
	 case T_Rocket_2:        //王炸
		 {
			 if (outCardSite == lordSite)
			 {
				 PlayerSite site1 = PlayerSite::kSiteLeft;
				 PlayerSite site2 = PlayerSite::kSiteRight;
				 if( outCardSite == PlayerSite::kSiteLeft ){
					 site1 = PlayerSite::kSiteSelf;
					 site2 = PlayerSite::kSiteRight;
				 }else if( outCardSite == PlayerSite::kSiteRight ){
					 site1 = PlayerSite::kSiteSelf;
					 site2 = PlayerSite::kSiteLeft;
				 }
				 auto rocketAnim = RocketLayer::createWith(site1, site2);
				 if(rocketAnim){
					 m_pUIgameScene->openGlobalUI(rocketAnim);
				 }else{
					 showX2Animation();
				 }
			 }
			 else if( lastOutCardSite >= 0 && lastOutCardSite< 3 )
			 {
				int finalSite = lastOutCardSite;
				if( lastOutCardSite == outCardSite){ //玩家当前轮首出王炸
					finalSite = lordSite;
				}
				auto rocketAnim = RocketLayer::createWith(finalSite, -1);
				if(rocketAnim){
					 m_pUIgameScene->openGlobalUI(rocketAnim);
				 }else{
					 showX2Animation();
				 }
			 }else{
				 showX2Animation();
			 }
		 }
		 break;
	 default:
		 break;
	 }

	 return type;
}

bool GameLogicManager::playSpecialCardEffect(PlayCardData *data)
{
	bool bHandle = false;
	std::string strFirstAudio = "";
	std::string strSecAudio = "";
	int num = 0;

	if (data->getPlayPokers().size() == 1)
	{
		num = data->getPlayPokers().at(0)->getPokerNum();
		if (num == 2)          //出牌为4 时特殊处理
		{
			strFirstAudio = "one_poker_1_t.mp3";
			strSecAudio = "one_poker_1.mp3";
			bHandle = true;
		}
		else if (num == 13)    //出牌为2 时 如果是本家出第一手牌则提示一张2，如果是管上家的牌，则随机提示，返回给后面的音频方法去播放。
		{
			strSecAudio = "one_poker_12.mp3";
			bHandle = true;
		}
	}

	if (!bHandle)
	{
		return bHandle;
	}
	
	PlayerSite outCardSite = getPlaySite(data->getUserID());
	switch (outCardSite)
	{
	case kSiteSelf:
		{
			if ( m_pUIGamePlayerLayer->getnpcLeft()->getpIsOutCard() || m_pUIGamePlayerLayer->getnpcLeft()->getpIsOutCard() )
			{
				if (num == 13)
				{
					return false;
				}
				PLAY_EFFECT_NAME(strFirstAudio);
			}
			else
			{
				PLAY_EFFECT_NAME(strSecAudio);
			}
		}
		break;
	case kSiteRight:
		{
			if (m_pUIGamePlayerLayer->getnpcLeft()->getpIsOutCard() || m_pUIGamePlayerLayer->getnpcLeft()->getpIsOutCard())
			{
				if (num == 13)
				{
					return false;
				}
				PLAY_EFFECT_NAME(strFirstAudio);
			}
			else
			{
				PLAY_EFFECT_NAME(strSecAudio);
			}
		}
		break;
	case kSiteLeft:
		{
			if ( m_pUIGamePlayerLayer->getnpcLeft()->getpIsOutCard() || m_pUIGamePlayerLayer->getnpcLeft()->getpIsOutCard() )
			{
				if (num == 13)
				{
					return false;
				}
				PLAY_EFFECT_NAME(strFirstAudio);
			}
			else
			{
				PLAY_EFFECT_NAME(strSecAudio);
			}
		}
		break;
	default:
		break;
	}

	return bHandle;
}

/************************************************************************/
/* 按纽回调函数                                                         */
/************************************************************************/
void GameLogicManager::TopMenuTrusteeCallback()
{
	MsgGameCommonRequest::sendSubstituteRequest(0);
}

void GameLogicManager::TopMenuSetCallback()
{
	auto set = SetLayer::create(false);
	set->setAnchorPoint(Vec2(0.5,0.5));
	set->setPosition(CUR_SCREEN_SIZE.width * 0.5f - set->getContentSize().width * 0.5f,CUR_SCREEN_SIZE.height * 0.5f - set->getContentSize().height * 0.5f);
	m_pUIgameScene->addChild(set, BASIC_ZORDER_TOP);
	m_pGlobelTag = SCENR_GLOBEL_SET_TAG;
	m_pUIgameScene->openGlobalUI(set,SCENR_GLOBEL_SET_TAG);
}

void GameLogicManager::TopMenuReturnCallback()
{
	m_pUIgameScene->closeAllUI();

	if(m_gameState != kGameState::State_GameOver){
		auto tishi = CommDialog::creatTwoBtnOneMessage(GetWWString(g_Game_exit_sure,g_GameSence_Toast).c_str(),GetWWString(g_Game_exit_cansle,g_GameSence_Toast).c_str()
			,GetWWString(g_Game_exit_tishi,g_GameSence_Toast).c_str(),callfuncO_selector(GameLogicManager::exitGame),NULL);
		tishi->setPosition(CUR_SCREEN_SIZE.width * 0.5f - tishi->getContentSize().width * 0.5f,CUR_SCREEN_SIZE.height * 0.5f - tishi->getContentSize().height * 0.5f);
		//m_pUIgameScene->addChild(tishi,BASIC_ZORDER_TOP);
		m_pGlobelTag = SCENR_GLOBEL_EXIT_TAG;
		m_pUIgameScene->openGlobalUI(tishi,SCENR_GLOBEL_EXIT_TAG);
	}
	else
	{
		if(m_gameState != kGameState::State_GameOver){
			MsgGameCommonRequest::sendLeaveGame();
		}
		
		auto *pScene = LobbyScene::create();
		Director::getInstance()->replaceScene(pScene);

	}
}

void GameLogicManager::exitGame(Ref* pSender)
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);
	if(m_gameState != kGameState::State_GameOver){
		MsgGameCommonRequest::sendLeaveGame();
	}

	if(m_pUIgameScene){
		m_pUIgameScene->closeAllUI();
	}

	auto *pScene = LobbyScene::create();
	Director::getInstance()->replaceScene(pScene);

}

void GameLogicManager::GameOverContinueCallBack()
{
	m_pUIgameScene->closeAllUI();
	m_pUIgameScene->endGameOverTime();
	GameOverCloseCallBack();

	resetGameInfo();
	setIsXuJu(true);
	requestContinueGame();
	m_pUIgameScene->startContinueTimer(); //开始 15 秒倒计时

	showWaitForStartToast();
}

void GameLogicManager::GameOverCloseCallBack()
{
	//关闭结算页面
	if(m_pUIGameOverLayer){
		m_pUIgameScene->closeGlobalUI(m_pUIGameOverLayer);
		m_pUIGameOverLayer = nullptr;
	}
}

/************************************************************************/
/* scoket 消息处理函数                                                  */
/************************************************************************/
void GameLogicManager::ProcessMatchStart(GameStartData* startGameData)
{
	removeToast();

	m_pIsGameStart = true;
	m_pNoCallCount = 0;
	if (m_pIsXuju)
	{
		SOUND_MANGER->playBackGroundMusic(backGoundMusic_key);
	}
	resetGameInfo();

	setGameState(kGameState::State_CallLord);

	m_pUIGamePlayerLayer->hideAllBtnFrame();
	m_pUIGamePlayerLayer->getRecordIcon()->setEnabled(true);//开局后才能使用记牌器
	//关闭洗牌动画
	m_pUIgameScene->closeWaitAnimation();

	//发自己的牌
	int selfIndex = 0;
	for (int i = 0; i < startGameData->getPlayerIds().size(); ++i)
	{
		if(UserData::getInstance()->getIntValue(INT_USER_ID) == startGameData->getPlayerIds()[i])
		{
			selfIndex = i;
			break;
		}
	}
	dealMyPoker(startGameData->getPlayerPokers().at(selfIndex));

	//开局展示其它玩家手牌，需要播放动画
	m_pUIGamePlayerLayer->dealNpcPoker(PlayerSite::kSiteLeft, 17);
	m_pUIGamePlayerLayer->dealNpcPoker(PlayerSite::kSiteRight, 17);

	//比赛只有欢乐玩法
	setLaiziNumber(0);

	// 保存玩家信息
	m_pUIGamePlayerLayer->initPlayerData(startGameData);
	m_pUIGamePlayerLayer->updateUserInfo();

	m_pRightUserId= m_pUIGamePlayerLayer->getnpcRight()->getUserid();
	m_pLeftUserId = m_pUIGamePlayerLayer->getnpcLeft()->getUserid();

	m_pStepTime = startGameData->getPlayTimeOut();
	m_pJiaBeiTime = startGameData->getDoubleTime();
	auto firstId = startGameData->getFirstPlayer();

	setCurOutCardUser(getPlaySite(firstId));

	if(firstId == UserData::getInstance()->getIntValue(INT_USER_ID))
	{
		if(!m_pUIMyCardLayer->getIsAutoPlay()){
			showMyJDZFrame(m_pNoCallCount);
		}
	}

	// 显示倒计时
	showClockByUserId(firstId);

	//设置底注和倍数
	m_curRoomBase = startGameData->getFortuneBase();
	showRoomBaseTx();
	m_pMultiple = 1;
	m_pUIGameTopMenuLayer->updateMultiple(m_pMultiple);
	//设置托管按纽可点击
	m_pUIgameScene->setSubstituteEnable(true);
	autoTestStartGame(1.f);
}

void GameLogicManager::ProcessMatchResume(GameStartData* startGameData)
{
}
void GameLogicManager::ProcessResumeGame(GameStartData* startGameData)
{
	if(!startGameData)
		return;
	
	m_pIsGameStart = true;

	//处理恢复对局消息
	resetGameInfo();

	m_pUIGamePlayerLayer->hideAllBtnFrame();

	m_pUIgameScene->closeAllUI();

	//关闭洗牌动画
	m_pUIgameScene->closeWaitAnimation();
	//显示聊天按纽
	m_pUIgameScene->showChatMenuBtn();

	int selfIndex = 0;
	for (int i = 0; i < startGameData->getPlayerIds().size(); ++i)
	{
		if(UserData::getInstance()->getIntValue(INT_USER_ID) == startGameData->getPlayerIds()[i])
		{
			selfIndex = i;
			break;
		}
	}
	log(" GameLogicManager::ProcessResumeGame---selfIndex=%d, size = %d",selfIndex,startGameData->getPlayerPokers().at(selfIndex).size());
	//展示重连玩家手牌
	dealMyPoker(startGameData->getPlayerPokers().at(selfIndex), true);
	m_pUIGamePlayerLayer->dealNpcPoker(PlayerSite::kSiteLeft, startGameData->getCardNum().at(2), true); //重连需要服务器传左边玩家剩余手牌数量 这里写 17 仅测试
	m_pUIGamePlayerLayer->dealNpcPoker(PlayerSite::kSiteRight, startGameData->getCardNum().at(1), true); //重连需要服务器传右边玩家剩余手牌数量 这里写 17 仅测试

	//如果是癞子玩法确定痞子牌
	if(m_curRoomType == RoomType::RASCAL_ROOM){
		setLaiziNumber(startGameData->getLaizi());
	}else{
		setLaiziNumber(0);
	}

	//记录地主id
	m_pLandlordId = startGameData->getLordId();

	// 保存玩家信息
	m_pUIGamePlayerLayer->initPlayerData(startGameData);
	m_pUIGamePlayerLayer->updateUserInfo();
	m_pRightUserId= m_pUIGamePlayerLayer->getnpcRight()->getUserid();
	m_pLeftUserId = m_pUIGamePlayerLayer->getnpcLeft()->getUserid();

	auto lastPlayerId = startGameData->getLastPlayerID();
	auto nextId = startGameData->getNextPlayUserID();
	setCurOutCardUser(getPlaySite(nextId));

	switch (startGameData->getGameStatus())//(int1)1叫地主，2抢地主，3加倍，4明牌  5打牌 6结算
	{
	case 1://1叫地主
		{
			setGameState(kGameState::State_CallLord);
			if(nextId == UserData::getInstance()->getIntValue(INT_USER_ID))
			{
				showMyJDZFrame(m_pNoCallCount);
			}
		}
		break;
	case 2://抢地主
		{
			setGameState(kGameState::State_GrapLord);
			if(nextId == UserData::getInstance()->getIntValue(INT_USER_ID))
			{
				showMyQDZFrame();
			}
			/*else
			{
				if(startGameData->getQiangDizhus()[1])
				{
					m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_GRAPLORD_TAG, PlayerSite::kSiteRight);
				}
				else
				{
					m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_GRAPLORD_TAG, PlayerSite::kSiteRight);
				}
				if(startGameData->getQiangDizhus()[2])
				{
					m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_GRAPLORD_TAG, PlayerSite::kSiteLeft);
				}
				else
				{
					m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_GRAPLORD_TAG, PlayerSite::kSiteLeft);
				}
			}*/
			//startGameData->getQiangDizhus()[1]
			//BTN_GAMEPLAYERLAYER_GRAPLORD_TAG, //抢地主
			//BTN_GAMEPLAYERLAYER_NOTGRAPLORD_TAG,

			
			//m_pUIGamePlayerLayer->setPlayerStatus(1, PlayerSite::kSiteRight);
			//m_pUIGamePlayerLayer->setPlayerStatus(1, PlayerSite::kSiteLeft);
		}
		break;
	
	case 3://加倍
		{
			setGameState(kGameState::State_JiaBei);

			if(nextId == UserData::getInstance()->getIntValue(INT_USER_ID))
			{
				showMyJiaBeiFrame();
			}

			/*if(startGameData->getDoubleScoreFlag()[1])
			{
				m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_GRAPLORD_TAG, PlayerSite::kSiteRight);
			}
			else
			{
				m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_GRAPLORD_TAG, PlayerSite::kSiteRight);
			}*/

		}
		break;
	case 4://明牌
		{
			setGameState(kGameState::State_MinPai);

			if(nextId == UserData::getInstance()->getIntValue(INT_USER_ID))
			{
				showMyMinPaiFrame();
			}
		}
		break;
	case 5://打牌
		{
			setGameState(kGameState::State_PlayCard);
			
			std::string playCards = startGameData->getLastPokers();
			Vector<PokerObject*> outCardsArr =PokerUtil::getSeverData((uint8_t*)playCards.c_str(),playCards.size());
			//得到牌型
			kWeightValue weightValue = PokerTypeCheck::getPokerType(outCardsArr);
			int outCardType = weightValue.type;

			PlayerSite outCardSite = getPlaySite(lastPlayerId);
			setLastOutCardUser(outCardSite);

			//从大到小排序
			outCardsArr = PokerUtil::sortArrayBtoS(outCardsArr);
			
			//隐藏上一轮状态
			m_pUIGamePlayerLayer->hidePlayerStatus(outCardSite);

			bool isLandLord = m_pLandlordId == lastPlayerId;
			if( outCardSite == PlayerSite::kSiteLeft )
			{
				m_pUIgameScene->showOutCard(PlayerSite::kSiteLeft, outCardsArr, isLandLord, NULL);
				setLastPlayPokers(outCardsArr);
				m_pUIGamePlayerLayer->getnpcLeft()->setpIsOutCard(true);
			}
			else if( outCardSite == PlayerSite::kSiteRight )
			{
				m_pUIgameScene->showOutCard(PlayerSite::kSiteRight, outCardsArr, isLandLord, NULL);
				setLastPlayPokers(outCardsArr);
				m_pUIGamePlayerLayer->getnpcRight()->setpIsOutCard(true);
			}
			else if( outCardSite == PlayerSite::kSiteSelf )
			{
				m_pUIMyCardLayer->showPlayCardsResult(outCardsArr, outCardType);
				clearLastPlayPokers();
				m_pUIMyCardLayer->setIsOutCards(true);
			}

			if(nextId == UserData::getInstance()->getIntValue(INT_USER_ID))
			{
				showMyPlayBtnFrame();
				m_pUIMyCardLayer->setNoBiggerTipsShow(true);
			}
		}
		break;
	case 6://结算
		{
			setGameState(kGameState::State_GameOver);
		}
		 break;
	default:
		break;
	}

	if( startGameData->getGameStatus() > 2){
		
		//在顶部工具栏显示地主牌
		std::string baseCards = startGameData->getBasePokers();
		Vector<PokerObject*> baseCardByteArr = PokerUtil::getSeverData((uint8_t*) baseCards.c_str(), baseCards.size());
		m_pUIMyCardLayer->handleLastThreeCards(baseCardByteArr,false); //重连时地主牌已添加到自己手牌中

		//设置玩家角色
		m_pUIGamePlayerLayer->setPlayerRole(getPlaySite(m_pLandlordId));
		m_pUIMyCardLayer->setIsLord(m_pLandlordId == UserData::getInstance()->getIntValue(INT_USER_ID));
		
		if( m_curRoomType == RoomType::RASCAL_ROOM ){
			m_pUIMyCardLayer->addLaiziCardToTop();	//重连显示癞子牌
			updateMyLaiZiCards();
		}
	}

	// 显示倒计时
	m_pStepTime = startGameData->getPlayTimeOut();
	m_pJiaBeiTime = startGameData->getDoubleTime();
	// just for test.
	if( m_pJiaBeiTime < 0 ){
		m_pJiaBeiTime = m_pStepTime;
	}
	if(startGameData->getGameStatus() == 3)
	{
		showClockByUserId(-1);
	}else
	{
		showClockByUserId(nextId);
	}

	//设置底注和倍数
	m_curRoomBase = startGameData->getFortuneBase();
	showRoomBaseTx();
	m_pUIGameTopMenuLayer->updateMultiple(m_pMultiple);

	//设置托管按纽可点击
	m_pUIgameScene->setSubstituteEnable(true);
}

void GameLogicManager::ProcessStartGame(GameStartData* startGameData)
{
	removeToast();

	m_pIsGameStart = true;
	m_pNoCallCount = 0;
	if (m_pIsXuju)
	{
		SOUND_MANGER->playBackGroundMusic(backGoundMusic_key);
	}
	resetGameInfo();

	setGameState(kGameState::State_CallLord);

	m_pUIGamePlayerLayer->hideAllBtnFrame();
	m_pUIGamePlayerLayer->getRecordIcon()->setEnabled(true);//开局后才能使用记牌器
	//关闭洗牌动画
	m_pUIgameScene->closeWaitAnimation();
	//显示聊天按纽
	m_pUIgameScene->showChatMenuBtn();

	//发自己的牌
	int selfIndex = 0;
	for (int i = 0; i < startGameData->getPlayerIds().size(); ++i)
	{
		if(UserData::getInstance()->getIntValue(INT_USER_ID) == startGameData->getPlayerIds()[i])
		{
			selfIndex = i;
			break;
		}
	}
	dealMyPoker(startGameData->getPlayerPokers().at(selfIndex));

	//开局展示其它玩家手牌，需要播放动画
	m_pUIGamePlayerLayer->dealNpcPoker(PlayerSite::kSiteLeft, 17);
	m_pUIGamePlayerLayer->dealNpcPoker(PlayerSite::kSiteRight, 17);

	//如果是癞子玩法确定痞子牌
	if(m_curRoomType == RoomType::RASCAL_ROOM){ 
		setLaiziNumber(startGameData->getLaizi());
	}else{
		setLaiziNumber(0);
	}

	// 保存玩家信息
	m_pUIGamePlayerLayer->initPlayerData(startGameData);
	m_pUIGamePlayerLayer->updateUserInfo();

	m_pRightUserId= m_pUIGamePlayerLayer->getnpcRight()->getUserid();
	m_pLeftUserId = m_pUIGamePlayerLayer->getnpcLeft()->getUserid();

	m_pStepTime = startGameData->getPlayTimeOut();
	m_pJiaBeiTime = startGameData->getDoubleTime();
	auto firstId = startGameData->getFirstPlayer();

	setCurOutCardUser(getPlaySite(firstId));

	char toastStr[50] = {0};
	std::string formatStr = GetWWString(g_Game_WaitFor_CallLord,g_GameSence_Toast);
	if( firstId == m_pLeftUserId ){
		
		sprintf(toastStr, formatStr.c_str(), "left");

	}else if( firstId == m_pLeftUserId ){
		
		sprintf(toastStr, formatStr.c_str(), "right");

	}else if(firstId == UserData::getInstance()->getIntValue(INT_USER_ID)){
		
		sprintf(toastStr, formatStr.c_str(), "self");

		if(!m_pUIMyCardLayer->getIsAutoPlay()){
			showMyJDZFrame(m_pNoCallCount);
		}
	}
	
	////显示Toast
	//showToast(toastStr);

	// 显示倒计时
	showClockByUserId(firstId);

	//设置底注和倍数
	m_curRoomBase = startGameData->getFortuneBase();
	showRoomBaseTx();
	m_pMultiple = 1;
	m_pUIGameTopMenuLayer->updateMultiple(m_pMultiple);
	//设置托管按纽可点击
	m_pUIgameScene->setSubstituteEnable(true);
	autoTestStartGame(1.f);
}

void GameLogicManager::ProcessCallLoard(CallLordData* callLordData)
{
	m_pUIGamePlayerLayer->hideAllBtnFrame();

	if( callLordData->getJiaoFlag() ){
		m_pLandlordId = callLordData->getUserIdJiaoFen();
		//记录玩家是否叫地主
		PlayerSite callLordSite = getPlaySite(m_pLandlordId);
		if( callLordSite == PlayerSite::kSiteLeft ){
			m_pUIGamePlayerLayer->getnpcLeft()->setpIsCallLord(true);
		}else if( callLordSite == PlayerSite::kSiteRight ){
			m_pUIGamePlayerLayer->getnpcRight()->setpIsCallLord(true);
		}else if( callLordSite == PlayerSite::kSiteSelf ){
			m_pUIGamePlayerLayer->setpIsCallLord(true);
		}
		PLAY_EFFECT(jiaodizhu_key);
	}
	else
	{
		PLAY_EFFECT(bujiao_key);
		m_pNoCallCount++;
	}
	
	auto nextPlayerId = callLordData->getNextPlayer();
	PlayerSite nextSite = getPlaySite(nextPlayerId);
	setCurOutCardUser(nextSite);
	m_pUIGamePlayerLayer->hidePlayerStatus(nextSite);
	
	char toastStr[50] = {0};
	std::string formatStr = GetWWString(g_Game_WaitFor_CallLord,g_GameSence_Toast);
	if( callLordData->getJiaoFlag() ){	
		setGameState(kGameState::State_GrapLord);
		formatStr = GetWWString(g_Game_WaitFor_GrabLord,g_GameSence_Toast);
	}

	if( nextSite == PlayerSite::kSiteLeft )
	{
		sprintf(toastStr, formatStr.c_str(), "left");
	}
	else if( nextSite == PlayerSite::kSiteRight )
	{
		sprintf(toastStr, formatStr.c_str(), "right");
	}
	else if( nextSite == PlayerSite::kSiteSelf )
	{
		sprintf(toastStr, formatStr.c_str(), "self");
		
		/*if(!m_pUIMyCardLayer->getIsAutoPlay()){
			if( callLordData->getJiaoFlag() ){	
				setGameState(kGameState::State_GrapLord);
				showMyQDZFrame();
			}else{
				showMyJDZFrame();
			}
		}

		bool test = true;
		if(test)return;*/
		// --------------------------------new logic-----------------
		if(!m_pUIMyCardLayer->getIsAutoPlay())
		{
			switch (callLordData->getNextAction())
			{
			case 1://1:叫地主 
				if(callLordData->getJiaoFlag())
				{
					setGameState(kGameState::State_GrapLord);
					showMyQDZFrame();
				}
				else
				{
					showMyJDZFrame(m_pNoCallCount);
				}
				//showMyJDZFrame();
				break;
			case 2://2：抢地主
				setGameState(kGameState::State_GrapLord);
				showMyQDZFrame();
				break;;
			case 3://3加倍
				/*setGameState(kGameState::State_JiaBei);
				showMyJiaBeiFrame();*/
				break;
			case 4://4地主明牌
				setGameState(kGameState::State_MinPai);
				showMyMinPaiFrame();
				break;
			case 5://5出牌
				setGameState(kGameState::State_PlayCard);
				showMyPlayBtnFrame();
				break;
			default:
				break;
			}
		}
		
	}
	////显示Toast
	//showToast(toastStr);

	// 叫地主状态展示
	auto userId = callLordData->getUserIdJiaoFen();
	auto grabTag = callLordData->getJiaoFlag() ? BTN_GAMEPLAYERLAYER_JIAODIZHU_TAG : BTN_GAMEPLAYERLAYER_BUJIAODIZHU_TAG;
	m_pUIGamePlayerLayer->setPlayerStatus(grabTag, getPlaySite(userId));


	if(callLordData->getNextAction() == 3){
		setGameState(kGameState::State_JiaBei);
		if(!m_pUIMyCardLayer->getIsAutoPlay()){
			showMyJiaBeiFrame();
		}
		showClockByUserId(-1);
		setCurOutCardUser(PlayerSite::kSiteLeft);
		if(m_pUIMyCardLayer->getIsAutoPlay())
		{
			MsgLordGameRequest::sendJiaBei(0); //自己超时不加倍
		}
	}else{
		// 显示倒计时
		showClockByUserId(nextPlayerId);
	}
}

void GameLogicManager::ProcessGrabLoard(GrabLordData* grabLordData)
{
	m_pUIGamePlayerLayer->hideAllBtnFrame();

	if( grabLordData->getIsGrab()){
		m_pLandlordId = grabLordData->getUserIdQiangDiZhu();
		
		showX2Animation();
		PLAY_EFFECT(qiangdizhu_key);
	}
	else
	{
		PLAY_EFFECT(buqiang_key);
	}

	auto nextPlayerId = grabLordData->getNextPlayer();
	auto nextSite = getPlaySite(nextPlayerId);

	setCurOutCardUser(nextSite);
	m_pUIGamePlayerLayer->hidePlayerStatus(nextSite);
	if( grabLordData->getIsQiangDiZhuOver() == 2){//加倍
		setGameState(kGameState::State_GrapLord);
	}else if( grabLordData->getIsQiangDiZhuOver() == 3){//加倍
		setGameState(kGameState::State_JiaBei);
	}else if(grabLordData->getIsQiangDiZhuOver() == 4){ //明牌
		setGameState(kGameState::State_MinPai);
	}else if(grabLordData->getIsQiangDiZhuOver() == 5){ //出牌
		setGameState(kGameState::State_PlayCard);
	}

	if( grabLordData->getIsQiangDiZhuOver() > 2){//-------------------抢地主结束
		m_pUIMyCardLayer->handleLastThreeCards(grabLordData->getBottomPokers(),m_pLandlordId==UserData::getInstance()->getIntValue(INT_USER_ID));
		//玩家手牌张数增加地主牌张数
		m_pUIGamePlayerLayer->updateLeftCardCountById(m_pLandlordId, grabLordData->getBottomPokers().size());
		//设置玩家角色
		m_pUIGamePlayerLayer->setPlayerRole(getPlaySite(m_pLandlordId));
		//隐藏所有状态展示
		m_pUIGamePlayerLayer->hideAllPlayerStatus();
		GameSoundManager::getInstance()->playEffectWithName(zaiqiang_file);
		
		//播放选癞子动画
		if( m_curRoomType == RoomType::RASCAL_ROOM ) //叫抢地主结束
		{
			m_pUIGamePlayerLayer->endCountdown();
			m_pUIMyCardLayer->startChooseLaiZi();
		}else{
			onLaiZiAnimEnd();
		}
	}else{
		// 抢地主状态展示
		auto userId = grabLordData->getUserIdQiangDiZhu();
		auto grabTag = grabLordData->getIsGrab() ? BTN_GAMEPLAYERLAYER_GRAPLORD_TAG : BTN_GAMEPLAYERLAYER_NOTGRAPLORD_TAG;
		m_pUIGamePlayerLayer->setPlayerStatus(grabTag, getPlaySite(userId));
		
		onLaiZiAnimEnd();
	}
	
}

void GameLogicManager::onLaiZiAnimEnd()
{
	auto nextSite = getCurOutCardUser();
	if( nextSite == PlayerSite::kSiteSelf ){
		if(!m_pUIMyCardLayer->getIsAutoPlay())
		{
			switch( getGameState() )
			{
			case kGameState::State_GrapLord://2:继续抢地主
				showMyQDZFrame();
				break;
			case kGameState::State_JiaBei://3加倍
				//showMyJiaBeiFrame();
				break;
			case kGameState::State_MinPai://4地主明牌
				showMyMinPaiFrame();
				m_pUIMyCardLayer->removeMyOutCards();
				break;
			case kGameState::State_PlayCard://5出牌
				showMyPlayBtnFrame();
				break;
			default:
				break;
			}
		}
	}

	if( getGameState() == kGameState::State_JiaBei)
	{
		if(!m_pUIMyCardLayer->getIsAutoPlay()){
			showMyJiaBeiFrame();
		}
		showClockByUserId(-1);
		setCurOutCardUser(PlayerSite::kSiteSelf);
		if(m_pUIMyCardLayer->getIsAutoPlay())
		{
			MsgLordGameRequest::sendJiaBei(0); //自己超时不加倍
		}
	}else{
		// 显示倒计时
		int countTime = m_pStepTime;
		if( getGameState() == kGameState::State_MinPai)
		{
			countTime = m_pJiaBeiTime;
		}
		m_pUIGamePlayerLayer->startToCountdown(countTime, nextSite);
	}
}

void GameLogicManager::ProcessOutCard(PlayCardData* outCardData)
{
	removeToast();    
	
	setGameState(kGameState::State_PlayCard);

	m_pUIGamePlayerLayer->hideAllBtnFrame();
	
	//隐藏癞子选择框
	m_pUIgameScene->removeChooseLaiziTable();

	Vector<PokerObject*> outCardsArr = outCardData->getPlayPokers();
	//癞子时，牌型判断使用替换后的牌判断
	if (m_curRoomType == RoomType::RASCAL_ROOM)
	{
		//找出替换后的牌中，癞子变成的牌，并设置其花色为癞子花色
		updateLaiziCards(outCardData->getPlayPokers(), outCardData->getReplacePokers());
		outCardsArr = outCardData->getReplacePokers();
	}

	m_pUIGamePlayerLayer->updataRecord(outCardsArr);
	int userId = outCardData->getUserID();
	PlayerSite outCardSite = getPlaySite(userId);

	if( outCardsArr.size() > 0 ){

		//更新玩家手牌张数
		m_pUIGamePlayerLayer->updateLeftCardCountById(userId, -outCardsArr.size());
		//从大到小排序
		outCardsArr = PokerUtil::sortArrayBtoS(outCardsArr);
		//播放特殊牌型动画及判断翻倍
		int outCardType = showSpecialCardAnimation( outCardSite, getLastOutCardUser(), outCardsArr );
		//隐藏上一轮状态
		m_pUIGamePlayerLayer->hidePlayerStatus(outCardSite);

		bool isLandLord = m_pLandlordId == userId;
		if( outCardSite == PlayerSite::kSiteLeft )
		{
			m_pUIgameScene->showOutCard(PlayerSite::kSiteLeft, outCardsArr, isLandLord, NULL);
			setLastPlayPokers(outCardsArr);
			m_pUIGamePlayerLayer->getnpcLeft()->setpIsOutCard(true);
		}
		else if( outCardSite == PlayerSite::kSiteRight )
		{
			m_pUIgameScene->showOutCard(PlayerSite::kSiteRight, outCardsArr, isLandLord, NULL);
			setLastPlayPokers(outCardsArr);
			m_pUIGamePlayerLayer->getnpcRight()->setpIsOutCard(true);
		}
		else if( outCardSite == PlayerSite::kSiteSelf )
		{
			m_pUIMyCardLayer->showPlayCardsResult(outCardsArr, outCardData->getReplacePokers(), outCardType);
			clearLastPlayPokers();
			m_pUIMyCardLayer->setIsOutCards(true);
			m_pUIMyCardLayer->setNoBiggerTipsShow(false);
		}
		setLastOutCardUser(outCardSite);
		
	}else{
		// 不出状态展示
		m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_BUCHU_TAG, outCardSite);
		
		if( outCardSite == PlayerSite::kSiteLeft )
		{
			m_pUIGamePlayerLayer->getnpcLeft()->setpIsOutCard(false);
		}
		else if( outCardSite == PlayerSite::kSiteRight )
		{
			m_pUIGamePlayerLayer->getnpcRight()->setpIsOutCard(false);
		}
		else if( outCardSite == PlayerSite::kSiteSelf )
		{
			m_pUIMyCardLayer->setIsOutCards(false);
			m_pUIMyCardLayer->setNoBiggerTipsShow(false);
		}
	}

	//播放出牌音效
	if (!playSpecialCardEffect(outCardData))
	{
		//播放出牌音效
		kWeightValue weight = PokerTypeCheck::getPokerType(outCardsArr);
		SOUND_MANGER->playPokerAudio(weight);
	}

	auto nextPlayerId = outCardData->getNextPlayer();
	setCurOutCardUser(getPlaySite(nextPlayerId));
	if(nextPlayerId == m_pLeftUserId)
	{
		m_pUIGamePlayerLayer->hidePlayerStatus(PlayerSite::kSiteLeft);
		m_pUIGamePlayerLayer->getnpcLeft()->clearOutCard();
	}
	else if(nextPlayerId == m_pRightUserId)
	{
		m_pUIGamePlayerLayer->hidePlayerStatus(PlayerSite::kSiteRight);
		m_pUIGamePlayerLayer->getnpcRight()->clearOutCard();
	}
	else if( nextPlayerId == UserData::getInstance()->getIntValue(INT_USER_ID) )
	{
		m_pUIGamePlayerLayer->hidePlayerStatus(PlayerSite::kSiteSelf);
		if(!m_pUIMyCardLayer->getIsAutoPlay()){
			m_pUIMyCardLayer->setNoBiggerTipsShow(true);
			showMyPlayBtnFrame();
		}
		m_pUIMyCardLayer->removeMyOutCards();
	}
	
	// 显示倒计时
	showClockByUserId(nextPlayerId);
}

void GameLogicManager::ProcessGameOver(LordGameOverData* gameOverData)
{
	/*gameOverData->getMultiple();
	gameOverData->getIsChunTian();
	gameOverData->getWinnerUserID();*/
	setGameState(kGameState::State_GameOver);

	//展示玩家剩余手牌
	int selfLeftCardCount = 0;
	int leftLeftCardCount = 0;
	int rightLeftCardCount = 0;
	auto userIds = gameOverData->getUserIDs();
	for(int index=0; index < userIds.size(); index++){

		std::string remainCard = gameOverData->getCard().at(index);
		Vector<PokerObject*> pokerArr =PokerUtil::getSeverData((uint8_t*)remainCard.c_str(),remainCard.size());
		if( !(pokerArr.size() > 0) ){
			continue;
		}
		//从大到小排序
		pokerArr = PokerUtil::sortArrayBtoS(pokerArr);

		if(userIds.at(index) == m_pUIGamePlayerLayer->getUserid())
		{
			selfLeftCardCount = pokerArr.size();
			////自己剩余的牌留在手中不打出
			//m_pUIMyCardLayer->showPlayCardsResult(pokerArr, T_SEQ_1);
		}
		else if(userIds.at(index) == m_pUIGamePlayerLayer->getnpcLeft()->getUserid())
		{
			leftLeftCardCount = pokerArr.size();
			m_pUIGamePlayerLayer->getnpcLeft()->clearOutCard();
			m_pUIgameScene->showLeftCard(PlayerSite::kSiteLeft, pokerArr, getPlaySite(m_pLandlordId)==PlayerSite::kSiteLeft, NULL);
		}
		else if(userIds.at(index) == m_pUIGamePlayerLayer->getnpcRight()->getUserid())
		{
			rightLeftCardCount = pokerArr.size();
			m_pUIGamePlayerLayer->getnpcRight()->clearOutCard();
			m_pUIgameScene->showLeftCard(PlayerSite::kSiteRight, pokerArr, getPlaySite(m_pLandlordId)==PlayerSite::kSiteRight, NULL);
		}
	}
	log("gameOverData->getIsChunTian()--------------------------------------%d------",gameOverData->getIsChunTian());
	if(gameOverData->getIsChunTian()){  //(int1)是否春天 0：否 1：是 
	//服务器传的是否是春天字段有误，自己重新判断
	/*if((selfLeftCardCount == 0 && leftLeftCardCount == 17 && rightLeftCardCount == 17) 
		||(selfLeftCardCount == 17 && leftLeftCardCount == 0 && rightLeftCardCount == 17)
		||(selfLeftCardCount == 17 && leftLeftCardCount == 17 && rightLeftCardCount == 0))
	{ */
		auto springAnim = LeafFallingLayer::create();
		m_pUIgameScene->openGlobalUI(springAnim);
	}

}

void GameLogicManager::ProcessCommonGameOver(GameOverCommonData* gameOverData)
{
	////清除 gameId, playId
	//GameGlobalDataManager::getInstance()->clearCurGameData();

	//--------------------------test code -----------
	if (DebugMode == 2 || DebugMode == 3)
	{
		m_pUIGamePlayerLayer->scheduleOnce(CC_SCHEDULE_SELECTOR(GameLogicManager::autoTestEndGame), 5.0f);
	}

	m_pUIGamePlayerLayer->gameOverReset();
	m_pUIMyCardLayer->removeMyOutCards();
	m_pUIGamePlayerLayer->resetRecord();//重置记牌器
	m_pUIGamePlayerLayer->getRecordIcon()->setEnabled(false);
	m_pUIMyCardLayer->setSubtitude(false);
	m_pUIMyCardLayer->setTouchEnabled(false);
	m_pUIMyCardLayer->setNoBiggerTipsShow(false);
	clearLastPlayPokers();
	
	//隐藏癞子选择框
	m_pUIgameScene->removeChooseLaiziTable();
	//隐藏聊天按纽
	m_pUIgameScene->hideChatMenuBtn();
	//设置托管按纽不可点击
	m_pUIgameScene->setSubstituteEnable(false);

	setGameState(kGameState::State_GameOver);
	SOUND_MANGER->stopBackGroundMusic(true);
	
	// 目前只用通用结算
	int selfIndex = 0;
	for (int i = 0; i < gameOverData->getUserIds().size(); ++i)
	{
		if(UserData::getInstance()->getIntValue(INT_USER_ID) == gameOverData->getUserIds()[i])
		{
			selfIndex = i;
			break;
		}
	}
	m_pUIGameOverLayer = GameOverLayer::create(gameOverData);
	m_pUIgameScene->openGlobalUI(m_pUIGameOverLayer);
}

void GameLogicManager::ProcessSubstitute(GameSubstituteData* substitutedata)
{
	if(! substitutedata) return;

	auto userId = substitutedata->getUserID();
	
	if( userId == UserData::getInstance()->getIntValue(INT_USER_ID) ){
		//关闭叫抢地主阶段 toast 提示
		removeToast();

		auto isAi = substitutedata->getType() ? false : true;
		m_pUIMyCardLayer->setSubtitude( isAi );
		m_pUIgameScene->setSubstituteEnable( !isAi );
		if(! isAi){
			if(getCurOutCardUser() == PlayerSite::kSiteSelf){
				// 重新显示倒计时
				showClockByUserId(UserData::getInstance()->getIntValue(INT_USER_ID));

				m_pUIMyCardLayer->removeMyOutCards();

				switch(getGameState())
				{
				case State_CallLord:
					showMyJDZFrame(m_pNoCallCount);
					break;
				case State_GrapLord:
					showMyQDZFrame();
					break;
				case State_MinPai:
					showMyMinPaiFrame();
					break;
				case State_PlayCard:
					showMyPlayBtnFrame();
					m_pUIMyCardLayer->setNoBiggerTipsShow(true);
					break;
				}
			}
		}else{
			m_pUIGamePlayerLayer->hideAllBtnFrame();
		}
	}
}

void GameLogicManager::ProcessMinPai(MingPaiData* mingPaiData)
{
	if(! mingPaiData) return;
	PLAY_EFFECT(mingpai_key);
	
	showX2Animation();

	auto userId = mingPaiData->getUserID();
	m_pUIGamePlayerLayer->showMinPaiResult(mingPaiData->getPlayerPokers(), getPlaySite(userId));
	m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_MINTPAI_TAG, getPlaySite(userId));

	// 显示倒计时
	m_pUIGamePlayerLayer->startToCountdown(m_pStepTime, getPlaySite(userId));


	if(getCurOutCardUser() == PlayerSite::kSiteSelf)
	{
		if(!m_pUIMyCardLayer->getIsAutoPlay()){
			showMyPlayBtnFrame();
		}
	}
}

void  GameLogicManager::ProcessJiaBei(JiaBeiData* JiaBeiData)
{
	// 显示倒计时
	//showClockByUserId(nextPlayerId);
	if(! JiaBeiData) return;
	//TODO play music
	auto currSite = getPlaySite(JiaBeiData->getUserID());
	auto nextSite = getPlaySite(JiaBeiData->getNextPlay());
	
	auto jiaBeiTag = BTN_GAMEPLAYERLAYER_BUJIABEI_TAG;
	log("-----------jiabei-----abd-----flag = %d- ",JiaBeiData->getFlag());
	switch (JiaBeiData->getFlag())
	{
	case 0://不加倍
		jiaBeiTag = BTN_GAMEPLAYERLAYER_BUJIABEI_TAG;
		break;
	case 1://加倍
		{
			if(currSite == PlayerSite::kSiteSelf)
			{
				showX2Animation();
			}
			jiaBeiTag = BTN_GAMEPLAYERLAYER_JIABEI_TAG;
		}
		break;
	case 2://加强加倍
		{
			if(currSite == PlayerSite::kSiteSelf)
			{
				//TODO 播放 X4 动画
				m_pMultiple *= 4;
				m_pUIGameTopMenuLayer->updateMultiple(m_pMultiple);
			}
			jiaBeiTag = BTN_GAMEPLAYERLAYER_SUPER_JIABEI_TAG;
		}
		break;
	case 3://加倍结束
		break;
	default:
		break;
	}
	if(JiaBeiData->getFlag() != 3)
	{
		m_pUIGamePlayerLayer->setPlayerStatus(jiaBeiTag, currSite);
		
		//保存是否收到对应玩家的加倍消息
		if(currSite == PlayerSite::kSiteRight)
		{
			//TODO 处理界面
			m_pUIGamePlayerLayer->getnpcRight()->setIsRecvJiaBei(true);
		}
		else if(currSite == PlayerSite::kSiteLeft)
		{
			//TODO 处理界面
			m_pUIGamePlayerLayer->getnpcLeft()->setIsRecvJiaBei(true);
		}
		else if(currSite == PlayerSite::kSiteSelf)
		{
			//TODO 处理界面
			m_pUIGamePlayerLayer->setIsRecvJiaBei(true);
		}
	}
	else
	{
		setCurOutCardUser(nextSite);
		m_pUIGamePlayerLayer->hidePlayerStatus(nextSite);
		m_pUIGamePlayerLayer->setShowJiaBeiFrame(false);


		//加倍结束，若未收到玩家加倍消息， 默认为用户未加倍，显示相应界面
		if(!m_pUIGamePlayerLayer->getnpcRight()->getIsRecvJiaBei())
		{
			m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_BUJIABEI_TAG, PlayerSite::kSiteRight);
		}else
		{
			m_pUIGamePlayerLayer->getnpcRight()->setIsRecvJiaBei(false);
		}
		if(!m_pUIGamePlayerLayer->getnpcLeft()->getIsRecvJiaBei())
		{
			m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_BUJIABEI_TAG, PlayerSite::kSiteLeft);
		}else
		{
			m_pUIGamePlayerLayer->getnpcLeft()->setIsRecvJiaBei(false);
		}
		if(!m_pUIGamePlayerLayer->getIsRecvJiaBei())
		{
			m_pUIGamePlayerLayer->setPlayerStatus(BTN_GAMEPLAYERLAYER_BUJIABEI_TAG, PlayerSite::kSiteSelf);
		}else
		{
			m_pUIGamePlayerLayer->setIsRecvJiaBei(false);
		}
	}

	log("-----------jiabei-----111------- ");
	if(JiaBeiData->getFlag() == 3 && nextSite == PlayerSite::kSiteSelf && !m_pUIMyCardLayer->getIsAutoPlay())
	{
		log("-----------jiabei-----111----222--- ");
		switch (JiaBeiData->getNextAction())
		{
		case 3://3加倍
			//showMyJDZFrame();
			break;
		case 4://4地主明牌
			log("-----------jiabei-----111---333---- ");
			setGameState(kGameState::State_MinPai);
			showMyMinPaiFrame();
			m_pUIMyCardLayer->removeMyOutCards();
			showClockByUserId(JiaBeiData->getNextPlay());
			break;
		case 5://5出牌
			setGameState(kGameState::State_PlayCard);
			showMyPlayBtnFrame();
			break;
		default:
			break;
		}
	}

	//if(JiaBeiData->getFlag() != 3)
	//{
	//	BTN_GAMEPLAYERLAYER_JIABEI_TAG, //加倍
	//	BTN_GAMEPLAYERLAYER_SUPER_JIABEI_TAG,//超级加倍
	//	BTN_GAMEPLAYERLAYER_BUJIABEI_TAG, //不加倍
	//	auto userId = JiaBeiData->getUserID();
	//	auto grabTag = JiaBeiData->getJiaoFlag() ? BTN_GAMEPLAYERLAYER_JIABEI_TAG : BTN_GAMEPLAYERLAYER_BUJIAODIZHU_TAG;
	//	m_pUIGamePlayerLayer->setPlayerStatus(grabTag, getPlaySite(userId));
	//}
	
	
}

/************************************************************************/
/* 其它逻辑类函数                                                       */
/************************************************************************/
//数据派发。不同地方调用了数据请求后，统一交给这里处理
void GameLogicManager::dataDistributedLogic(int msgType, cocos2d::Ref* objectData)
{
	log("dataDistributedLogic had been called -> msgType : %x", msgType);
	switch (msgType)
	{
	case MSG_GAME_START:
		{
			log("GameLogicManager::onDataReceiver---- MSG_GAME_START");
			ProcessStartGame((GameStartData*)objectData);
			//MsgGameCommonRequest::sendSubstituteRequest(0);
		}
		break;
	//叫地主数据
	case MSG_GAME_CALL_LORD:
		{
			log("GameLogicManager::onDataReceiver----  MSG_GAME_CALL_LORD");
			ProcessCallLoard((CallLordData*)objectData);
		}
		break;
	//抢地主数据
	case MSG_GAME_GRAP_LORD:
		{
			log("GameLogicManager::onDataReceiver---- MSG_GAME_GRAP_LORD");
			ProcessGrabLoard((GrabLordData*)(objectData));
		}
		break;
	//其他玩家明牌数据
	case MSG_GAME_MING_PAI:
		{
			log("GameLogicManager::onDataReceiver---- MSG_GAME_MING_PAI");
			//在牌面层显示其他玩家的明牌信息，服务器不会返回自己的明牌信息
			ProcessMinPai((MingPaiData*)(objectData));
		}
		break;
	case MSG_GAME_DOUBLE_SCORE:
		{
			log("GameLogicManager::onDataReceiver---- MSG_GAME_DOUBLE_SCORE");
			ProcessJiaBei((JiaBeiData*)(objectData));
		}
		break;
	//出牌数据
	case MSG_GAME_PLAY_CARD:
		{
			log("GameLogicManager::onDataReceiver---- MSG_GAME_PLAY_CARD");
			ProcessOutCard((PlayCardData*)(objectData));
		}
		break;
	//结算数据
	case MSG_GAME_GAMEOVER:
		{
			log("GameLogicManager::onDataReceiver---- MSG_GAME_GAMEOVER");
			ProcessGameOver((LordGameOverData*)(objectData));
		}
		break;
	//通用结算
	case MSG_GAME_COMMON_GAMEOVER:
		{
			log("GameLogicManager::onDataReceiver---- MSG_GAME_COMMON_GAMEOVER");
			ProcessCommonGameOver((GameOverCommonData*)(objectData));
		}
		break;
	//托管
    case MSG_GAME_SUBSTITUTE:
		{
			log("GameLogicManager::onDataReceiver---- MSG_GAME_SUBSTITUTE");
			ProcessSubstitute((GameSubstituteData*)(objectData));
		}
		break;
	//聊天
	case MSG_GAME_CHAT:
		{
		}
		break;
	//道具使用
	case MSG_PROP_USE_INFOR:
		{
		}
		break;
	//请求游戏离开返回
	case MSG_LEAVEGAME_BACK:
		{
		}
		break;
	//拒绝续局请求返回
	case MSG_REFUSE_CONTINUE_GAME:
		{
		}
		break;

	default:
		break;
	}
}

void GameLogicManager::autoTestStartGame(float dt)
{
	if(DebugMode >= 2)
	{
		MsgGameCommonRequest::sendSubstituteRequest(0);
	}
}
void GameLogicManager::autoTestEndGame(float dt)
{
	if(DebugMode == 2)
	{//离开游戏再次开局
		GameLogicManager::getInstance()->getpUIgameScene()->endGameOverTime();
		GameLogicManager::getInstance()->TopMenuReturnCallback();
	}
	else if(DebugMode == 3)
	{//一直在游戏中续局 ，  有自动续局功能，此处可不做处理

	}
}

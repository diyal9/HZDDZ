/*
 * GameScene.h
 *
 *
 *  Created on: 2015-5-11
 *      Author: diyal.yin
 */

#include "GameBasicScene.h"
#include "WWMacros.h"
#include "BaseCore/WWBGUI/WWToast.h"
#include "View/GameScene/Layer/GameBgLayer.h"
#include "View/GameScene/Layer/GamePlayerLayer.h"
#include "View/GameScene/Layer/GameTopMenuLayer.h"
#include "View/GameScene/Layer/MyCardsLayer.h"
#include "View/GameScene/Layer/LaiziCardsChoose.h"

#include "View/GameScene/GameLogicManager.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "View/GameScene/Animation/OutCardLayer.h"
#include "Control/WWCSound/WWSoundManager.h"
#include "Control/WWCSound/GameSoundManager.h"
#include "BaseCore/WWEffectBase/WWAnimation.h"
#include "View/Common/ChatLayer/ChatLayer.h"
#include "Control/WWDataManager/GameGlobalDataManager.h"

USING_NS_CC;
using namespace std;

const int g_nMaxCardNum = 20;               //最大牌数

GameBasicScene::GameBasicScene(void) :
	rooms(NULL),
	m_GameOverTime(15)/*,
	isOpenMenu(false)*/
{
	isWaitAnimRun = false;
	m_chooseRescalTx = nullptr;
}

GameBasicScene::~GameBasicScene(void)
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);
}

void GameBasicScene::onEnter()
{
   log("%s %d\r\n", __FUNCTION__, __LINE__);

   Scene::onEnter();
   
   //当前界面在游戏中
   GameGlobalDataManager::getInstance()->setCurSceneType(GameSceneType::ROOM_SCENE);

   //注册捕捉监听
   auto listenerkeyPad = EventListenerKeyboard::create();
   listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameBasicScene::onKeyReleased, this);
   _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

   //GameRoom* room = WWDataManager::getInstance()->getGameRoom();
   //std::string noFlg = "no";
   //MsgGameCommonRequest::enterGame(room, noFlg); //请求开局
   WWSoundManager::getInstance()->stopBackgroundMusic();
   SOUND_MANGER->playBackGroundMusic(backGoundMusic_key);
   
   log("全局UI");
   //全局UI层
   GameLogicManager::getInstance()->showWaitForStartToast();
   GameLogicManager::getInstance()->showRoomBaseTx();
   GameLogicManager::getInstance()->resetGameInfo();

   if(GameGlobalDataManager::getInstance()->getStartGameType() == StartGameType::SEND_NOMAL)
   {
	   GameLogicManager::getInstance()->requestEnterGame();
   }
   else if(GameGlobalDataManager::getInstance()->getStartGameType() == StartGameType::SEND_RESUME)
   {
	    GameLogicManager::getInstance()->requestBackGame();
   }
   else if(GameGlobalDataManager::getInstance()->getStartGameType() == StartGameType::NO_SEND)
   {
	   //已经收到恢复对局消息
	   GameLogicManager::getInstance()->ProcessResumeGame(WWDataManager::getInstance()->getGameStartData());
   }
   else if(GameGlobalDataManager::getInstance()->getStartGameType() == StartGameType::MATCH_START_FROM_LOBBY)
   {
	   //处理比赛开局
	   GameLogicManager::getInstance()->ProcessMatchStart(WWDataManager::getInstance()->getGameStartData());
   }
   
   //WWNetWorkManager::getInstance()->registerMsgFilter(this);

	scheduleUpdate();
}

void GameBasicScene::onExit()
{
   log("%s %d\r\n", __FUNCTION__, __LINE__);
   
   this->unschedule(schedule_selector(GameBasicScene::updateContinueTimer));
   this->unschedule(schedule_selector(GameBasicScene::updataGameOverTime));

   _eventDispatcher->removeEventListenersForTarget(this);

   SOUND_MANGER->stopBackGroundMusic(true);
   //WWNetWorkManager::getInstance()->unregisterMsgFilter(this);

	// 释放资源
	this->resume();
	this->unscheduleAllCallbacks();
	this->stopAllActions();

	this->removeAllChildrenWithCleanup(true);

	WWAnimation::getInstance()->removeAnimationCache("shuffle");
	WWAnimation::getInstance()->removeAnimationCache("smogAnim");

	unscheduleUpdate();

	Scene::onExit();

	// 释放材质
	Director::getInstance()->getTextureCache()->removeUnusedTextures(); //TODO 这里先使用全部释放掉，在UI编辑的时候，需要用plist文件去释放掉

	log("%s \r\n", (Director::getInstance()->getTextureCache()->getCachedTextureInfo()).c_str());
}

bool GameBasicScene::init()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	if (!Scene::init())
	{
		return false;
	}


	//背景层
	auto bgLayer = GameBgLayer::create();
	bgLayer->setPosition(Vec2(0, 0));
	this->addChild(bgLayer, BASIC_ZORDER_ROOT);

	GameLogicManager::getInstance()->setpUIgameScene(this);

	//顶部栏
	auto topMenuLayer = GameTopMenuLayer::create();
	topMenuLayer->setPosition(Vec2(0, 0));
	this->addChild(topMenuLayer, BASIC_ZORDER_ROOT);
	if (topMenuLayer)
	{
		GameLogicManager::getInstance()->setpUIGameTopMenuLayer(topMenuLayer);
	}

	//顶部设置
	// 设置
	m_pSetMenu = GameSetMenu::create();
	m_pSetMenu->setPosition(Vec2::ZERO);
	this->addChild(m_pSetMenu,BASIC_ZORDER_TOP);

		log("创建玩家UI");
	
	//玩家和NPC层
	auto gamePlayerLayer = GamePlayerLayer::create();
	gamePlayerLayer->setPosition(Vec2(0, 0));
	this->addChild(gamePlayerLayer, BASIC_ZORDER_ROOT + 1);
	if (gamePlayerLayer)
	{
		GameLogicManager::getInstance()->setpUIGamePlayerLayer(gamePlayerLayer);
	}

	log("自己手牌层UI");
	//自己手牌层
	MyCardsLayer  *myCardsLayer= MyCardsLayer::create();
	if (myCardsLayer)
	{
		this->addChild(myCardsLayer, BASIC_ZORDER_ROOT);
		GameLogicManager::getInstance()->setpUIMyCardLayer(myCardsLayer);
	}
	
	//第二渲染树节点
	m_pSence = Scene::create();
    if (m_pSence)
    {
		this->addChild(m_pSence, BASIC_ZORDER_TOP);
    }

	return true;
}

//聊天
void GameBasicScene::showChatMenuBtn()
{
	if( this->getChildByTag(TAGCHATLAYER) ){
		this->getChildByTag(TAGCHATLAYER)->setVisible(true);
	}else{
		auto chatLayer = ChatLayer::create();
		chatLayer->setPosition(Vec2::ZERO);
		this->addChild(chatLayer, BASIC_ZORDER_FRAME, TAGCHATLAYER);
	}
}
void GameBasicScene::hideChatMenuBtn()
{
	if( this->getChildByTag(TAGCHATLAYER) ){
		this->getChildByTag(TAGCHATLAYER)->setVisible(false);
	}
}

void GameBasicScene::startGameOverTime()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	if (UserDefault::getInstance()->getBoolForKey(SET_CONTINU_KEY,true)){//自动续局开关
		
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setIsGameOverTimeUp(false);

		m_GameOverTime = 15;
		//倒计时
		this->schedule(schedule_selector(GameBasicScene::updataGameOverTime),1,15,0);
	}
}

void GameBasicScene::endGameOverTime()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	this->unschedule(schedule_selector(GameBasicScene::updataGameOverTime));
}

void GameBasicScene::updataGameOverTime(float dt)
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	if (m_GameOverTime == 0)
	{
	    this->unschedule(schedule_selector(GameBasicScene::updataGameOverTime));
		GameLogicManager::getInstance()->GameOverContinueCallBack();
		if(GameLogicManager::getInstance()->getpUIGameOverLayer()){
			GameLogicManager::getInstance()->GameOverCloseCallBack();
		}
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setShowGameOverFrame(false);
	}
	else
	{
		m_GameOverTime--;
		if(GameLogicManager::getInstance()->getpUIGameOverLayer()){
			GameLogicManager::getInstance()->getpUIGameOverLayer()->updataTime(m_GameOverTime);
		}
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->updateGameOverTime(m_GameOverTime);
	}

}
 
void GameBasicScene::startContinueTimer() //开始结局 15 秒倒计时
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);
	scheduleOnce(schedule_selector(GameBasicScene::updateContinueTimer), 15.0f);
}

void GameBasicScene::endContinueTimer()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);
	this->unschedule(schedule_selector(GameBasicScene::updateContinueTimer));
}

void GameBasicScene::updateContinueTimer(float dt)
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);
	endContinueTimer();
	GameLogicManager::getInstance()->sendQuickStartGame();
}

#define ANIMATIONTAG 200
void GameBasicScene::showWaitAnimation()
{
	if(isWaitAnimRun){ //以免重复创建
		return;
	}
	auto ani = WWAnimation::getInstance()->animationUnCache("shuffle",15,0.1f,1);
	auto node = WWAnimation::getInstance()->createAnimationSprite("shuffle");
	node->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5, CUR_SCREEN_SIZE.height * 0.5));
	node->setScale(1.0f / 0.8f);
	this->addChild(node, BASIC_ZORDER_ROOT, ANIMATIONTAG);

	node->runAction(RepeatForever::create(Animate::create(ani)));

	isWaitAnimRun = true;
}

void GameBasicScene::closeWaitAnimation()
{
	if (this->getChildByTag(ANIMATIONTAG)){
		this->removeChildByTag(ANIMATIONTAG);
	}
	isWaitAnimRun = false;
}



void GameBasicScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
    {
		if(GameLogicManager::getInstance()->getpGlobelTag() > 0){
			this->closeGlobalUI(GameLogicManager::getInstance()->getpGlobelTag());
			GameLogicManager::getInstance()->setpGlobelTag(0);
		}else{
			GameLogicManager::getInstance()->TopMenuReturnCallback();
		}
    }
}


void GameBasicScene::ChooseLaiziCardsToPlay(const std::vector<cocos2d::Vector<PokerObject*>> &vecAllPokersVec)
{	
	LaiziCardsTable *table = LaiziCardsTable::create(vecAllPokersVec);
	if (table)
	{
		table->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5f - table->getContentSize().width*0.5f +30, CUR_SCREEN_SIZE.height * 0.5f + 30));
		addChild(table,BASIC_ZORDER_TOP,BIN_GAMEPLAYERLAYER_CHOOSE_RESCAL_TAG);
	}

	if(!m_chooseRescalTx){
		m_chooseRescalTx = Sprite::create("res/Images/GameScene/Label/choose_card_type.png");
		m_chooseRescalTx->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5f +60, 418));
		addChild(m_chooseRescalTx, BASIC_ZORDER_TOP);
	}
	if(table){
		m_chooseRescalTx->setVisible(true);
	}else{
		m_chooseRescalTx->setVisible(false);
	}
}

void GameBasicScene::removeChooseLaiziTable()
{
	if (this->getChildByTag(BIN_GAMEPLAYERLAYER_CHOOSE_RESCAL_TAG) != nullptr)
	{
		removeChildByTag(BIN_GAMEPLAYERLAYER_CHOOSE_RESCAL_TAG);
	}

	if(m_chooseRescalTx){
		m_chooseRescalTx->setVisible(false);
	}
}


void GameBasicScene::setSubstituteEnable(bool isEnable)
{
	m_pSetMenu->setSubstituteEnable(isEnable);
}

void GameBasicScene::openGlobalUI(Node* pNode, int tag)
{
	CCAssert(pNode != nullptr, "openGlobalUI args pNode is NULL");
    CC_ASSERT(m_pSence && m_pSence->getParent());
	do
	{
		//判断第二渲染树根节点是否为空，为空则跳出
		CC_BREAK_IF(!m_pSence);
		log("openGlobalUI !m_pSence");
		//判断加入的节点是否为空，为空则跳出
		CC_BREAK_IF(!pNode);
		log("openGlobalUI !pNode");
		//判断加入的的节点的父节点是否为空，不为空则跳出
		CC_BREAK_IF(pNode->getParent());
		log("openGlobalUI pNode->getParent()");
		//判断加入的的节点的父节点是否已经存在，有则跳出
		CC_BREAK_IF(m_pSence->getChildren().contains(pNode));
		log("openGlobalUI m_pSence->getChildren().contains(pNode)");
		pNode->setTag(tag);

		//DEBUG Trace
		if (m_pSence->getChildByTag(tag))
		{
			log("Child Is Exist");
		}

		m_pSence->addChild(pNode);
	} while (0);
}

void GameBasicScene::closeGlobalUI(Node* pNode)
{
	CCAssert(pNode != nullptr, "closeGlobalUI args pNode is NULL");
	do
	{
		CC_BREAK_IF(!m_pSence);
		CC_BREAK_IF(!pNode);
		m_pSence->removeChild(pNode);
	} while (0);

}

void GameBasicScene::closeGlobalUI(int tag)
{
	do
	{
		CC_BREAK_IF(!m_pSence);
		m_pSence->removeChildByTag(tag);
	} while (0);

}


void GameBasicScene::closeAllUI()
{
	do
	{
		CC_BREAK_IF(!m_pSence);
		//清空第二渲染树根节点
		m_pSence->removeAllChildren();
        
        //Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(g_closeGlUIEventName);
	} while (0);
}

//自己出牌
void GameBasicScene::showOutCard(const cocos2d::Vector<CardSprite*> &outCards, int outCardType, bool isAutoPlay, bool isLanlord, cocos2d::CallFunc *func)
{
	openGlobalUI(OutCardLayer::showOutCardSelf(outCards, outCardType, isAutoPlay, isLanlord, func));
}

//其它玩家出牌
void GameBasicScene::showOutCard(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func)
{
	openGlobalUI(OutCardLayer::showOutCardOther(seat, outCards, isLandlord, func));
}

//牌局结束显示其它玩家剩余手牌
void GameBasicScene::showLeftCard(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func)
{
	openGlobalUI(OutCardLayer::showLeftCardOther(seat, outCards, isLandlord, func));
}

void GameBasicScene::update(float dt)
{
	dealNetRecv();
}

void GameBasicScene::onMsgRecv(int msgType)
{
	Ref* objData=nullptr;
	switch (msgType)
	{
        case MSG_GAME_START://游戏开局
            objData = parseStartGame();
            break;
        case MSG_GAME_CALL_LORD://叫地主
            objData = parseCallLord();
            break;
        case MSG_GAME_GRAP_LORD://抢地主
            objData = parseGrabLord();
            break;
        case MSG_GAME_MING_PAI://明牌
            objData = parseMingPai();
            break;
        case MSG_GAME_PLAY_CARD://出牌
            objData = parsePlayCard();
            break;
        case MSG_GAME_GAMEOVER://对局结算
            objData = parseGameOver();
            break;
        case MSG_GAME_COMMON_GAMEOVER: //通用結算
            objData = parseCommonGameOver();
            break;
        case MSG_GAME_SUBSTITUTE://托管
            objData = parseGameSubstitute();
            break;
        case MSG_GAME_CHAT://聊天
            objData = parseGameChat();
            break;
        case MSG_PROP_USE_INFOR://道具使用
            objData = parsePropUseToInfo();
            break;
		case MSG_GAME_DOUBLE_SCORE://加倍
			objData = parseJiaBei();
			break;
        default:
            return;
            
	}
	GameLogicManager::getInstance()->dataDistributedLogic(msgType, objData);
}

void GameBasicScene::onRootMsgRecv(int msgType, int userId, int result, std::string reason, int reasonType)
{
}

Ref* GameBasicScene::parseStartGame()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	GameStartData* data = GameStartData::create();
	data->setGameType(1);
    data->setResume(false);

	int gameId = pClientSocket->ReadInt();//(int4)游戏ＩＤ
	int playId = pClientSocket->ReadInt();//(int4)对局ID
	data->setGameId(gameId);
	data->setGamePlayId(playId);
	GameGlobalDataManager::getInstance()->setCurGameId(gameId);
	GameGlobalDataManager::getInstance()->setCurPlayId(playId);
	GameGlobalDataManager::getInstance()->setPlayRoomId(WWDataManager::getInstance()->getGameRoom()->getGZID());//保存正在对局的房间ID

	data->setPlayTimeOut(pClientSocket->ReadUnsignedShort());//(int2)单手牌超时时间（单位：秒）
	int DoubleTime = pClientSocket->ReadShort();//(int2)加倍时间（单位：秒）
	data->setDoubleTime(DoubleTime);
	// 
	data->setFirstPlayer(pClientSocket->ReadInt());//(int4)第一个叫分用户 
	std::string datas = pClientSocket->ReadString();//(String)附加数据
	int Num4Award = pClientSocket->ReadUnsignedShort();//(int2)奖宝箱局数
	int Playnum = pClientSocket->ReadUnsignedShort();//(int2)当前财富局数
	int FortuneBase = pClientSocket->ReadInt();//(int4)财富底分
	int Laizi = pClientSocket->ReadChar();//(int1)赖子: 有效值1-13,依次表示345678910JQKA2

	CCLOG("MsgLordGameFilter::parseStartGame--------datas=%s, Num4Award=%d, Playnum=%d, FortuneBase=%d, Laizi=%d",datas.c_str(),Num4Award,Playnum,FortuneBase,Laizi);
	data->setData(datas);
	data->setTaskNum4Award(Num4Award);
	data->setPlayedNum4Award(Playnum);
	data->setFortuneBase(FortuneBase);
	data->setLaizi(Laizi);
	//
	data->setPlayerCount(pClientSocket->ReadChar());//(int1)游戏人数
	int playerCount = data->getPlayerCount();
    
	data->getPlayerIds().resize(playerCount);
	data->getPlayerNickNames().resize(playerCount);
	data->getPlayerIcons().resize(playerCount);
	//data->getGameLevels().resize(playerCount);
	data->getPlayerVips().resize(playerCount);
	//data->getTeamLevels().resize(playerCount);
	data->getPlayerPokers().resize(playerCount);
	data->getPlayerCashs().resize(playerCount);
	//data->getTeamLeader().resize(playerCount);
	//data->getTongLeader().resize(playerCount);
	//data->getPropDoublePower().resize(playerCount);
	//data->getRegion().resize(playerCount);
	data->getJiFenLevel().resize(playerCount);
	data->getGenders().resize(playerCount);
	data->getOnlineTypes().resize(playerCount);
    
	CCLOG("MsgLordGameFilter::parseStartGame--------gameid=%d,gamePlayId=%d,timeOut=%d,DoubleTime=%d,playerCount=%d,firstPlayer=%d", data->getGameId(), data->getGamePlayId(), data->getPlayTimeOut(), DoubleTime, playerCount, data->getFirstPlayer());
	for (int j = 0; j < playerCount; j++)
	{
		data->getPlayerIds()[j]=(pClientSocket->ReadInt());//(int4)用户ＩＤ
		data->getPlayerNickNames()[j]=(pClientSocket->ReadString());//(String)用户名称
		data->getPlayerIcons()[j]=(pClientSocket->ReadInt());//(int4)用户头像
		data->getPlayerVips()[j]=(pClientSocket->ReadChar());//(int2)用户VIP等级
		data->getPlayerPokers()[j]=(pClientSocket->ReadString());//(byte[])用户的牌
		data->getPlayerCashs()[j]=(pClientSocket->ReadInt());//(int4)用户财富值
		data->getJiFenLevel()[j]=(pClientSocket->ReadUnsignedShort());//(int2)游戏积分等级
		data->getGenders()[j] = pClientSocket->ReadChar();//(int1)性别
		data->getOnlineTypes()[j] = pClientSocket->ReadString();//(String)网络接入

		log("MsgLordGameFilter::parseStartGame-----index = %d---, UserID=%d, UserName=%s, Icon=%d, VIPGrade= %d, Card=%s, Fortune=%d, GameLevel=%d, Gender=%d, APN=%s "
			,j,data->getPlayerIds()[j], data->getPlayerNickNames()[j].c_str(), data->getPlayerIcons()[j], data->getPlayerVips()[j], data->getPlayerPokers()[j].c_str(), data->getPlayerCashs()[j], data->getJiFenLevel()[j], data->getGenders()[j], data->getOnlineTypes()[j].c_str());
	}
    
	log("MsgLordGameFilter::parseStartGame -------recv start game Msg");

	return data;
}

Ref* GameBasicScene::parseCallLord()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	CallLordData* data = CallLordData::create();
	data->setGamePlayId(pClientSocket->ReadInt()) ;
	data->setUserIdJiaoFen( pClientSocket->ReadInt());
	data->setJiaoFlag(pClientSocket->ReadChar());
	data->setNextPlayer(pClientSocket->ReadInt());
	std::string pokers = pClientSocket->ReadString();

	int nextAction = pClientSocket->ReadChar();//（int1）1:叫地主   2：抢地主  3加倍  4明牌  5出牌
	data->setNextAction(nextAction);

	CCLOG("MsgLordGameFilter::parseCallLord-----nextAction = %d, userID = %d",nextAction,data->getUserIdJiaoFen());
	Vector<PokerObject*> arrays =PokerUtil::getSeverData((uint8_t*)pokers.c_str(),pokers.size());
    
	for (auto obj : arrays)
	{
		if(obj)
		{
			data->getBottomPokers().pushBack(obj);
		}
	}

	return data;
}

Ref* GameBasicScene::parseGrabLord()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	GrabLordData* data = GrabLordData::create();
    
	data->setGamePlayId(pClientSocket->ReadInt()) ;
	data->setUserIdQiangDiZhu( pClientSocket->ReadInt());
	data->setIsGrab(pClientSocket->ReadChar());
	data->setIsQiangDiZhuOver(pClientSocket->ReadChar());//(int1) 2:抢地主  3加倍  4地主明牌  5出牌
	data->setNextPlayer(pClientSocket->ReadInt());
	std::string bottomPokers = pClientSocket->ReadString();

	CCLOG("MsgLordGameFilter::parseGrabLord-----IsQiangDiZhuOver = %d, userID = %d",data->getIsQiangDiZhuOver(),data->getUserIdQiangDiZhu());
	Vector<PokerObject*> arrays =PokerUtil::getSeverData((uint8_t*)bottomPokers.c_str(),bottomPokers.size());
	for (auto obj : arrays)
	{
		if(obj)
		{
			data->getBottomPokers().pushBack(obj);
		}
	}

	return data;
}

Ref* GameBasicScene::parseMingPai()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	
	MingPaiData* data = MingPaiData::create();
	data->setGamePlayId(pClientSocket->ReadInt());
	data->setUserID(pClientSocket->ReadInt());
	std::string playerCards = pClientSocket->ReadString();
	Vector<PokerObject*> arrays =PokerUtil::getSeverData((uint8_t*)playerCards.c_str(),playerCards.size());
    
	for (auto obj : arrays)
	{
		if(obj)
		{
			data->getPlayerPokers().pushBack(obj);
		}
	}
    
	return data;
}

Ref* GameBasicScene::parseJiaBei()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	JiaBeiData* data = JiaBeiData::create();
	int GamePlayID = pClientSocket->ReadInt();//(int4)对局标示 
	int userID = pClientSocket->ReadInt();//(int4)用户ＩＤ 
	int flag = pClientSocket->ReadChar();//(int1)用户是否抢地主  0:不加倍 1:加倍  2：加强加倍  3：加倍结束
	int NextAction = pClientSocket->ReadChar();//(int1)2叫牌 当flag=3时  3地主明牌  4出牌
	int NextPlay = pClientSocket->ReadInt();//(int4)flag=3加倍结束，为下一个明牌或出牌用户ＩＤ 

	CCLOG("MsgLordGameFilter::parseJiaBei----GamePlayID=%d, userID=%d, flag=%d, NextAction=%d, NextPlay=%d",
		GamePlayID,userID,flag,NextAction,NextPlay);

	data->setGamePlayId(GamePlayID);
	data->setUserID(userID);
	data->setFlag(flag);
	data->setNextAction(NextAction);
	data->setNextPlay(NextPlay);
	
	return data;
}

Ref* GameBasicScene::parsePlayCard()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	
	PlayCardData* data = PlayCardData::create();
	data->setGamePlayId(pClientSocket->ReadInt());
	data->setUserID(pClientSocket->ReadInt());
	std::string playCards = pClientSocket->ReadString();
	Vector<PokerObject*> arrays =PokerUtil::getSeverData((uint8_t*)playCards.c_str(),playCards.size());
	for (auto obj : arrays)
	{
		if(obj)
		{
			data->getPlayPokers().pushBack(obj);
		}
	}
	data->setNextPlayer(pClientSocket->ReadInt());
	std::string replaceCards = pClientSocket->ReadString();
	Vector<PokerObject*> arrays2 =PokerUtil::getSeverData((uint8_t*)replaceCards.c_str(),replaceCards.size());
	for (auto obj : arrays2)
	{
		if(obj)
		{
			data->getReplacePokers().pushBack(obj);
		}
	}
	data->setType(pClientSocket->ReadChar());
	data->setValue(pClientSocket->ReadChar());
	
	return data;
}

Ref*  GameBasicScene::parseGameOver()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	LordGameOverData* data = LordGameOverData::create();
	data->setGamePlayId(pClientSocket->ReadInt());//(int4)对局标示 
	data->setWinnerUserID(pClientSocket->ReadInt());//(int4)赢家用户ＩＤ 

	//
	data->setUnit(pClientSocket->ReadString());//(String)单位
	log("parseGameOver:Unit=%s",data->getUnit().c_str());
	data->setData(pClientSocket->ReadString());//(String)附加数据
	log("parseGameOver:Data=%s",data->getData().c_str());
	data->setZoneWin(pClientSocket->ReadChar());//(int1)输赢类型:1:战斗力 2:财富 5.金豆
	log("parseGameOver:ZoneWin=%d",data->getZoneWin());
	data->setMultiple(pClientSocket->ReadUnsignedShort());//(int2)输赢倍数,由游戏区定义
	log("parseGameOver:Multiple=%d",data->getMultiple());
	data->setTime(pClientSocket->ReadString());//(String)游戏时长
	log("parseGameOver:Time=%s",data->getTime().c_str());
	data->setBankUserID(pClientSocket->ReadInt());//(int4)地主UserID
	log("parseGameOver:BankUserID=%d",data->getBankUserID());
	data->setBankScore(pClientSocket->ReadChar());//(int1)地主叫分
	log("parseGameOver:BankScore=%d",data->getBankScore());
	data->setBoomCount(pClientSocket->ReadChar());//(int1)炸弹数量
	log("parseGameOver:BoomCount=%d",data->getBoomCount());
	data->setFan(pClientSocket->ReadUnsignedShort());//(int2)游戏番数
	log("parseGameOver:Fan=%d",data->getFan());
	data->setFortuneBase(pClientSocket->ReadInt());//(int4)财富底分
	log("parseGameOver:FortuneBase=%d",data->getFortuneBase());
	data->setBigBoomCount(pClientSocket->ReadChar());//(int1)炸弹数量2
	log("parseGameOver:BigBoomCount=%d",data->getBigBoomCount());
	data->setIsChunTian(pClientSocket->ReadChar());//(int1)是否春天 0：否 1：是
	CCLOG("parseGameOver:IsChunTian=%d",data->getIsChunTian());
	//add end
	//-----------------
	
	int MagicCount =pClientSocket->ReadShort();//(int2)礼包数量
	CCLOG("parseGameOver:MagicCount=%d",MagicCount);

	data->setMagicCount(MagicCount);
	data->getMagicIDs().resize(MagicCount);
	data->getMagicTypes().resize(MagicCount);
	for(int i = 0; i < MagicCount; i++)
	{
		int MagicID = pClientSocket->ReadInt();//(int4)礼包ID
		data->getMagicIDs()[i] = MagicID;
		int MagicType = pClientSocket->ReadChar();//(int1)礼包类型  1-连胜宝箱；  2-对局宝箱；
		data->getMagicTypes()[i] = MagicType;
		log("parseGameOver:----MagicID=%d, MagicType = %d",MagicID,MagicType);
	}
	//--------------------
	int playerCount = pClientSocket->ReadChar();
	data->setCount(playerCount);

	data->getUserIDs().resize(playerCount);
	data->getScores().resize(playerCount);
	data->getTScores().resize(playerCount);
	data->getLevels().resize(playerCount);
	data->getCard().resize(playerCount);
    
	data->getDLWLs().resize(playerCount);
	data->getFans().resize(playerCount);
	data->getGamePoints().resize(playerCount);
	data->getTGamePoints().resize(playerCount);
	data->getGameLevelNames().resize(playerCount);

	data->getContinueWinNums().resize(playerCount);
	data->getTopWinNums().resize(playerCount);
    
	data->getAllPlayCounts().resize(playerCount);
	data->getNeededPlayCounts().resize(playerCount);
	data->getLuckyPoints().resize(playerCount);
	data->getGreatAwards().resize(playerCount);

	std::vector<std::string> remainPokerStr;
	remainPokerStr.resize(playerCount);

	for (int i= 0; i < playerCount; ++i)
	{
		int goverCurUserID = pClientSocket->ReadInt();//(int4)用户ＩＤ 
		log("parseGameOver:goverCurUserID=%d",goverCurUserID);
		//int pos = GameContext::getInstance()->getPlayerIndex(goverCurUserID);//getPlayerIndex待修改
		int pos = i;//测试代码   -------该处， 需要对比UserID来确定是哪家的数据
		log("parseGameOver:pos=%d",pos);
		data->getUserIDs()[pos]=goverCurUserID;
		log("parseGameOver:UserID=%d",data->getUserIDs()[pos]);

		data->getScores()[pos]=(pClientSocket->ReadInt());//(int4)用户本局财富变化/战力/筹码 
		log("parseGameOver:Score=%d",data->getScores()[pos]);
		data->getTScores()[pos]=(pClientSocket->ReadInt());//(int4)用户本局后总财富 /战力/筹码
        
		log("parseGameOver:TScore=%d",data->getTScores()[pos]);
		data->getLevels()[pos]=(pClientSocket->ReadShort());//(int2)用户本局后级别
		log("parseGameOver:Level=%d",data->getLevels()[pos]);
		remainPokerStr[pos]=(pClientSocket->ReadString());//(byte[])用户剩余在手牌
		log("parseGameOver:remainPokerStr=%s",remainPokerStr[pos].c_str());
		data->getCard()[pos] = remainPokerStr[pos];
        
		//data->getMagics()[pos]=(pClientSocket->ReadChar());
		//log("parseGameOver:Magic=%d",data->getMagics()[pos]);
		data->getDLWLs()[pos]=(pClientSocket->ReadChar());//(int1)倒拉窝垒标志,低4位表示
		log("parseGameOver:DLWL=%d",data->getDLWLs()[pos]);
		data->getFans()[pos]=(pClientSocket->ReadUnsignedShort());//(int2)倍数
		log("parseGameOver:Fan=%d",data->getFans()[pos]);
		data->getGamePoints()[pos]=(pClientSocket->ReadInt());//(int4)用户1本局游戏积分变化
		log("parseGameOver:GamePoint=%d",data->getGamePoints()[pos]);
		data->getTGamePoints()[pos]=(pClientSocket->ReadInt());//(int4)用户1本局后游戏总积分
		log("parseGameOver:TGamePoint=%d",data->getTGamePoints()[pos]);
		data->getGameLevelNames()[pos]=(pClientSocket->ReadString());//(String)用户1本局后游戏积分等级称号
		log("parseGameOver:GameLevelName=%s",data->getGameLevelNames()[pos].c_str());
        
		//add
		data->getAllPlayCounts()[i]=(pClientSocket->ReadUnsignedShort());//(int2)当天本游戏区总局数
		data->getNeededPlayCounts()[i]=(pClientSocket->ReadUnsignedShort());//(int2)获取金宝箱需要对局数
		data->getLuckyPoints()[i]=(pClientSocket->ReadInt());//(int4)幸运值
		data->getGreatAwards()[i]=(pClientSocket->ReadInt());//(int4)最高奖蛙币

		data->getContinueWinNums()[i] = (pClientSocket->ReadUnsignedShort());//(int2)连胜数
		data->getTopWinNums()[i] = (pClientSocket->ReadUnsignedShort());//(int2)历史最高连胜数
	}
    
	return data;
}

Ref* GameBasicScene::parseCommonGameOver()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	
	GameOverCommonData* data = GameOverCommonData::create();
    
	data->setGameId(pClientSocket->ReadInt());//(int4)游戏ID
	data->setGamePlayId(pClientSocket->ReadInt());//(int4)对局ID
	data->setBeanTax(pClientSocket->ReadInt());//(int4)蛙豆房系统收税
	int playerCount = pClientSocket->ReadUnsignedShort();//(int2)玩家数量
	data->setPlayerCount(playerCount);
	CCLOG("MsgLordGameFilter::parseCommonGameOverNew---------gameid=%d,gamePlayID=%d,playerCount=%d",data->getGameId(),data->getGamePlayId(),playerCount);
	data->getUserIds().resize(playerCount);
	data->getFortunes().resize(playerCount);
	data->getTFortunes().resize(playerCount);
	//data->getScores().resize(playerCount);
	//data->getTScores().resize(playerCount);
	data->getChips().resize(playerCount);
	data->getTChips().resize(playerCount);
	data->getGamePoints().resize(playerCount);
	data->getTGamePoints().resize(playerCount);
	data->getGamePointLevels().resize(playerCount);
	//data->getMagics().resize(playerCount);
	data->getPlayResults().resize(playerCount);

	data->getCurrLevel().resize(playerCount);
	data->getCurrExp().resize(playerCount);
	data->getAddExp().resize(playerCount);
	data->getLevelExp().resize(playerCount);
	//data->getOriFortunes().resize(playerCount);
	//std::vector<int> curUserIDs;
	//curUserIDs.resize(playerCount);
	for (int i = 0; i < playerCount; i++)
	{
		int goverCurUserID = pClientSocket->ReadInt();//(int4)用户1ＩＤ 
		CCLOG("MsgLordGameFilter::parseCommonGameOver----goverCurUserID=%d",goverCurUserID);
		
		//curUserIDs[i]=goverCurUserID;
		//int pos =GameContext::getInstance()->getPlayerIndex(goverCurUserID);
		int pos = i; //-----------------------测试代码  ,需根据开局消息中userId，来确定数据的具体index
		data->getUserIds()[pos]=goverCurUserID;

		std::string Fortune = pClientSocket->ReadString();//(String)用户本局财富变化 
		data->getFortunes()[pos] = Fortune;
		CCLOG("MsgLordGameFilter::parseCommonGameOver----Fortune=%s",Fortune.c_str());

		std::string TFortune = pClientSocket->ReadString();//(String)用户本局后总财富 
		data->getTFortunes()[pos] = TFortune;
		CCLOG("MsgLordGameFilter::parseCommonGameOver----TFortune=%s",TFortune.c_str());

		data->getChips()[pos]=pClientSocket->ReadInt();//(int4)用户本局临时积分变化 
		CCLOG("MsgLordGameFilter::parseCommonGameOver----Chips=%d",data->getChips()[pos]);

		data->getTChips()[pos]=pClientSocket->ReadInt();//TChip(n)	(int4)用户本局后临时积分
		CCLOG("MsgLordGameFilter::parseCommonGameOver----TChips=%d",data->getTChips()[pos]);

		data->getGamePoints()[pos]=pClientSocket->ReadInt();//(int4)用户本局游戏积分变化
		CCLOG("MsgLordGameFilter::parseCommonGameOver----GamePoints=%d",data->getGamePoints()[pos]);

		data->getTGamePoints()[pos]=pClientSocket->ReadInt();//(int4)用户本局后游戏总积分
		CCLOG("MsgLordGameFilter::parseCommonGameOver----TGamePoints=%d",data->getTGamePoints()[pos]);

		data->getGamePointLevels()[pos]=pClientSocket->ReadUnsignedShort();//(int2)积分级别
		CCLOG("MsgLordGameFilter::parseCommonGameOver----GamePointLevels=%d",data->getGamePointLevels()[pos]);

		data->getPlayResults()[pos]=pClientSocket->ReadUnsignedShort();//(int2)1赢、2输、3和
		CCLOG("MsgLordGameFilter::parseCommonGameOver----PlayResults=%d",data->getPlayResults()[pos]);

		int CurrLevel = pClientSocket->ReadInt();//(int4)用户当前等级
		log("MsgLordGameFilter::parseCommonGameOver----CurrLevel=%d",CurrLevel);
		data->getCurrLevel()[pos] = CurrLevel;

		int CurrExp = pClientSocket->ReadInt();//(int4)用户当前经验
		log("MsgLordGameFilter::parseCommonGameOver----CurrExp=%d",CurrExp);
		data->getCurrExp()[pos] = CurrExp;

		int AddExp = pClientSocket->ReadInt();//(int4)用户本局新增经验
		log("MsgLordGameFilter::parseCommonGameOver----AddExp=%d",AddExp);
		data->getAddExp()[pos] = AddExp;

		int LevelExp = pClientSocket->ReadInt();//(int4)当前等级升到下一等级所需的总经验
		log("MsgLordGameFilter::parseCommonGameOver----LevelExp=%d",LevelExp);
		data->getLevelExp()[pos] = LevelExp;
	}
    
	//data->getNickNames().resize(playerCount);
	/*cocos2d::Vector<Player *> players = GameContext::getInstance()->getPlayers();
	for (int i = 0; i < players.size(); ++i)
	{
		data->getNickNames()[i] = players.at(i)->getNickName();
	}
	GameContext::getInstance()->setGameOverCommonData(data);*/
	
	return data;
}

Ref* GameBasicScene:: parseGameSubstitute()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	GameSubstituteData* data = GameSubstituteData::create();
	data->setGameId(pClientSocket->ReadInt());
	data->setGameZoneID(pClientSocket->ReadInt());
	data->setGameplayID(pClientSocket->ReadInt());
	data->setUserID(pClientSocket->ReadInt());
	data->setType(pClientSocket->ReadChar());
	
	return data;
}

Ref* GameBasicScene::parseGameChat()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	GameChatData* data = GameChatData::create();
	data->setGamePlayID(pClientSocket->ReadInt());
	data->setFromUserID(pClientSocket->ReadInt());
	data->setType(pClientSocket->ReadChar());
	data->setChatContent(pClientSocket->ReadString());
	data->setUserNick(pClientSocket->ReadString());
	data->setToUserID(pClientSocket->ReadInt());
	
	return data;
}

Ref* GameBasicScene::parsePropUseToInfo()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	PropUseInfoData* data = PropUseInfoData::create();
    data->setGamePlayID(pClientSocket->ReadInt());
    data->setPropId(pClientSocket->ReadInt());
    data->setFromUserID(pClientSocket->ReadInt());
    data->setUseToUserID(pClientSocket->ReadInt());
	
    return data;
}

/*
 * GamePlayerLayer.cpp
 *
 *  Created on: 2015-5-13
 *      Author: diyal.yin
 */

#include "GamePlayerLayer.h"
#include "../GameSceneDefine.h"
#include "WWMacros.h"
#include "View/GameScene/Component/NpcPlayer.h"
#include "BaseCore/Common/VisibleRect.h"
#include "Control/WWRequest/MsgLordGameRequest.h"
#include "Control/WWRequest/MsgGameCommonRequest.h"
#include "View/GameScene/GameLogicManager.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "BaseCore/WWEffectBase/WWAnimation.h"
#include "View/Common/PokerLogic/PokerConstants.h"
#include "BaseCore/WWBTools/StringTool.h"
#include "View/GameScene/GameOver/GameOverLayerRes.h"

USING_NS_CC;

const int indexs[] = {R_WB,R_WA,R2,R1,RK,RQ,RJ,R10,R9,R8,R7,R6,R5,R4,R3};

const int leave_game_txt_color = 0x0077a1;  //离开游戏
const int continue_txt_color = 0x8a511b;    //再来一局

GamePlayerLayer::GamePlayerLayer(void) : 
	b_lUnOut(nullptr),
	b_lTips(nullptr),
	b_lDown(nullptr),
	b_lchupai(nullptr),
	b_lJiaoDiZhu(nullptr),
	b_lBuJiaoDiZhu(nullptr),
	b_lBuMingpai(nullptr),
	b_lMingpai(nullptr),
	b_lJiaBei(nullptr),
	b_lBuJiaBei(nullptr),
	b_lSurperJiaBei(nullptr),
	b_lLeaveGame(nullptr),
	b_lContinueGame(nullptr),
	b_lContinueTx(nullptr),
	b_lTimeTx(nullptr),
	b_isGameOverTimeUp(false),
	m_nTime(0),
	m_pIsUsedRecord(false)
{
}


GamePlayerLayer::~GamePlayerLayer(void)
{
}

bool GamePlayerLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0,0,0,0))) return false;


	bool bRet = false;
	do 
	{
		//数据处理
		//UI创建
		createView();

		createInteractionView();

		CC_BREAK_IF(!createPlayStatus());

		bRet = true;
	}
	while (0);

	return bRet;
}

void GamePlayerLayer::createView()
{
	//创建NPC
	l_npcLeft = NpcPlayer::create(false);
	l_npcLeft->setPosition(Vec2(0, 0));
	this->addChild(l_npcLeft);

	//创建NPC
	l_npcRight = NpcPlayer::create(true);
	l_npcRight->setPosition(Vec2(0, 0));
	this->addChild(l_npcRight);
}

void GamePlayerLayer::createInteractionView()
{	
	//头像框
	m_lRoleFrame = WWTouchSprite::createWithFile("res/Images/GameScene/roleBg.png", this, callfuncO_selector(GamePlayerLayer::onHeadDepress));
	m_lRoleFrame->setPosition(Vec2(127, 462));
	this->addChild(m_lRoleFrame, BASIC_ZORDER_ROOT);
	
	//玩家头像
	m_lRealHead = Sprite::create("res/Images/Common/head-temp.png");
	m_lRealHead->setPosition(Vec2(0, 0));
	m_lRoleFrame->addChild(m_lRealHead, BASIC_ZORDER_ROOT);

	//地主角色头像
	m_lLandLordRole = Sprite::create("res/Images/GameScene/dzRole.png");
	m_lLandLordRole->setPosition(Vec2(0, 0));
	m_lLandLordRole->setVisible(false);
	m_lRoleFrame->addChild(m_lLandLordRole, BASIC_ZORDER_ROOT);
	
	//农民角色头像
	m_lFarmerRole = Sprite::create("res/Images/GameScene/farmerHead.png");
	m_lFarmerRole->setPosition(Vec2(0, 0));
	m_lFarmerRole->setVisible(false);
	m_lRoleFrame->addChild(m_lFarmerRole, BASIC_ZORDER_ROOT);

	//房间自己信息展示
	auto l_myCoinBg = Sprite::create("res/Images/GameScene/ownData.png");
	l_myCoinBg->setPosition(Vec2(l_myCoinBg->getContentSize().width * 0.5f + 20, 312));
	l_myCoinBg->setTag(BTN_GAMEPLAYERLAYER_MYCOIN_AREA_TAG);
	this->addChild(l_myCoinBg, BASIC_ZORDER_ROOT);

	auto l_myCoinImg = Sprite::create("res/Images/GameScene/coin.png");
	l_myCoinImg->setPosition(Vec2(33, l_myCoinImg->getContentSize().height * 0.5f + 10));
	l_myCoinBg->addChild(l_myCoinImg, BASIC_ZORDER_ROOT);
	
	std::string userCash = WWDataManager::getInstance()->getUserBasicInfoData()->getGameCash();
	auto l_myCoinTx = Label::createWithTTF(userCash.c_str(),"res/Fonts/FounderSemiBold.ttf",30,Size(100,0),TextHAlignment::LEFT,TextVAlignment::CENTER);
	l_myCoinTx->setColor(GET_RGBCOLOR(16643283));
	l_myCoinTx->setTag(BTN_GAMEPLAYERLAYER_MYCOIN_TAG);
	l_myCoinTx->setPosition(Vec2(116, l_myCoinTx->getContentSize().height * 0.5f + 13));
	l_myCoinBg->addChild(l_myCoinTx,BASIC_ZORDER_ROOT);
	
	auto l_myCoinAddBtn = WWTouchSprite::createWithFile("res/Images/GameScene/gameSceneAddBtn.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	l_myCoinAddBtn->setTag(BTN_GAMEPLAYERLAYER_ADDCOIN_TAG);
	l_myCoinAddBtn->setPosition(Vec2(223, l_myCoinAddBtn->getContentSize().height * 0.5f + 8));
	l_myCoinBg->addChild(l_myCoinAddBtn, BASIC_ZORDER_ROOT);

	//不出
	b_lUnOut = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_buchu.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lUnOut->setPosition(Vec2(503, 398));
	this->addChild(b_lUnOut, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_BUCHU_TAG);

	//提示
	b_lTips = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_tishi.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lTips->setPosition(Vec2(926, 398));
	this->addChild(b_lTips, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_TISHI_TAG);

	//放下
	b_lDown = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_fangxia.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lDown->setPosition(Vec2(1219, 398));
	this->addChild(b_lDown, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_FANGXIA_TAG);

	//出牌
	b_lchupai = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_chupai.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lchupai->setPosition(Vec2(1506, 398));
	this->addChild(b_lchupai, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_CHUPAI_TAG);

	//叫地主
	b_lJiaoDiZhu = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_jiaodz.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lJiaoDiZhu->setPosition(Vec2(1219, 398));
	this->addChild(b_lJiaoDiZhu, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_JIAODIZHU_TAG);

	//不叫地主
	b_lBuJiaoDiZhu = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_bujiao.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lBuJiaoDiZhu->setPosition(Vec2(926, 398));
	this->addChild(b_lBuJiaoDiZhu, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_BUJIAODIZHU_TAG);
	
	//抢地主
	b_lGrapLord = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_qiangdz.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lGrapLord->setPosition(Vec2(1219, 398));
	this->addChild(b_lGrapLord, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_GRAPLORD_TAG);

	//不抢地主
	b_lNotGrapLord = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_buqiang.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lNotGrapLord->setPosition(Vec2(926, 398));
	this->addChild(b_lNotGrapLord, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_NOTGRAPLORD_TAG);

	//明牌
	b_lMingpai = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_mingpai.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lMingpai->setPosition(Vec2(1219, 398));
	this->addChild(b_lMingpai, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_MINTPAI_TAG);

	//不明牌
	b_lBuMingpai = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_bump.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lBuMingpai->setPosition(Vec2(926, 398));
	this->addChild(b_lBuMingpai, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_BUMINGPAI_TAG);

	//超级加倍
	b_lSurperJiaBei = WWTouchSprite::createCrossButton("res/Images/Common/btnLong.png", "res/Images/GameScene/Label/label_superjiabei.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lSurperJiaBei->setPosition(Vec2(610, 398));
	this->addChild(b_lSurperJiaBei, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_SUPER_JIABEI_TAG);

	//加倍
	b_lJiaBei = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_jiabei.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lJiaBei->setPosition(Vec2(980, 398));
	this->addChild(b_lJiaBei, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_JIABEI_TAG);

	//不加倍
	b_lBuJiaBei = WWTouchSprite::createCrossButton("res/Images/Common/gameingBtn.png", "res/Images/GameScene/Label/label_bujiabei.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lBuJiaBei->setPosition(Vec2(1270, 398));
	this->addChild(b_lBuJiaBei, BASIC_ZORDER_ROOT, BTN_GAMEPLAYERLAYER_BUJIABEI_TAG);

	//离开游戏
	b_lLeaveGame= WWTouchSprite::createWithFile("res/Images/Common/btn_blue_small.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lLeaveGame->setPosition(Vec2(726, 398));
	this->addChild(b_lLeaveGame,BASIC_ZORDER_FRAME,BIN_GAMEPLAYERLAYER_LEAVEGAME_TAG);
	//文字
	auto leaveGameLabel = Label::createWithTTF(GetWWString(g_over_leave,g_over_section),"res/Fonts/FounderSemiBold.ttf",45,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	leaveGameLabel->setColor(GET_RGBCOLOR(leave_game_txt_color));
	leaveGameLabel->setPosition(Vec2(0,0));
	b_lLeaveGame->addChild(leaveGameLabel);
	
	//再来一局
	b_lContinueGame = WWTouchSprite::createWithFile("res/Images/Common/btnLong.png",this,callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_lContinueGame->setPosition(Vec2(1219, 398));
	this->addChild(b_lContinueGame,BASIC_ZORDER_FRAME, BIN_GAMEPLAYERLAYER_CONTINUE_TAG);
	//文字
	b_lContinueTx = Label::createWithTTF(GetWWString(g_over_agan,g_over_section),"res/Fonts/FounderSemiBold.ttf",45,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	b_lContinueTx->setColor(GET_RGBCOLOR(continue_txt_color));
	b_lContinueTx->setPosition(Vec2(0,0));
	b_lContinueGame->addChild(b_lContinueTx);
	//倒计时
	b_lTimeTx = Label::createWithTTF(StringUtils::format(GetWWString(g_over_times,g_over_section).c_str(), 15),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	b_lTimeTx->setColor(GET_RGBCOLOR(continue_txt_color));
	b_lTimeTx->setPosition(Vec2(b_lContinueGame->getContentSize().width*0.25f,0));
	b_lTimeTx->setVisible(false);
	b_lContinueGame->addChild(b_lTimeTx);

	//闹钟
	m_pClock = Sprite::create("res/Images/GameScene/clock.png");
	m_pClock->setPosition(Vec2(715, 426));
	m_pClock->setVisible(false);
	this->addChild(m_pClock, BASIC_ZORDER_ROOT+1);

	//时间
	m_pTimeLabel = Label::create("30","res/Fonts/FounderSemiBold.ttf",60,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	m_pTimeLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_pTimeLabel->setPosition(Vec2(m_pClock->getContentSize().width / 2, m_pClock->getContentSize().height / 2));
	m_pTimeLabel->setColor(GET_RGBCOLOR(14044232));
	m_pClock->addChild(m_pTimeLabel);

	//记牌器
	b_recordIcon = WWTouchSprite::createCrossButton("res/Images/GameScene/pockRecord.png", "res/Images/GameScene/pockRecord.png", this, callfuncO_selector(GamePlayerLayer::onTouchSpriteDepress));
	b_recordIcon->setPosition(Vec2(70,64));
	b_recordIcon->setEnabled(false);
	this->addChild(b_recordIcon,BASIC_ZORDER_FRAME,BIN_GAMEPLAYERLAYER_RECORD_TAG);

	int count =  (sizeof(indexs) / sizeof(indexs[0]));
	for (int i = 2; i < count; i++)
	{
		pMMap[indexs[i]] = 4;
	}
	pMMap[R_WA]=1;
	pMMap[R_WB]=1;
	b_recordBg = Sprite::create("res/Images/GameScene/jipaiqiBg.png");
	b_recordBg->setPosition(Vec2(960,75));
	for (int i = 0; i < count; i++)
	{
		auto counLabel = Label::createWithTTF(StringTool::intToString(pMMap[indexs[i]]),"res/Fonts/FounderSemiBold.ttf",45,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		counLabel->setPosition(Vec2(109+60*i, 38));
		b_recordBg->addChild(counLabel);
	}
	this->addChild(b_recordBg,BASIC_ZORDER_TOP);
	b_recordBg->setVisible(false);
	hideAllBtnFrame();
}

bool GamePlayerLayer::createPlayStatus()
{
	bool bRet = false;
	do{
		auto statusPos = Vec2(318,494);

		m_statusCallLord = Sprite::create("res/Images/GameScene/Label/status_calllord.png");
		CC_BREAK_IF(!m_statusCallLord);
		m_statusCallLord->setPosition(statusPos);
		this->addChild(m_statusCallLord, BASIC_ZORDER_ROOT);
	
		m_statusNotCallLord = Sprite::create("res/Images/GameScene/Label/status_notcall.png");
		CC_BREAK_IF(!m_statusNotCallLord);
		m_statusNotCallLord->setPosition(statusPos);
		this->addChild(m_statusNotCallLord, BASIC_ZORDER_ROOT);
	
		m_statusGrabLord = Sprite::create("res/Images/GameScene/Label/status_grablord.png");
		CC_BREAK_IF(!m_statusGrabLord);
		m_statusGrabLord->setPosition(statusPos);
		this->addChild(m_statusGrabLord, BASIC_ZORDER_ROOT);
	
		m_statusNotGrabLord = Sprite::create("res/Images/GameScene/Label/status_notgrab.png");
		CC_BREAK_IF(!m_statusNotGrabLord);
		m_statusNotGrabLord->setPosition(statusPos);
		this->addChild(m_statusNotGrabLord, BASIC_ZORDER_ROOT);
	
		m_statusMingCard = Sprite::create("res/Images/GameScene/Label/status_mingcard.png");
		CC_BREAK_IF(!m_statusMingCard);
		m_statusMingCard->setPosition(statusPos);
		this->addChild(m_statusMingCard, BASIC_ZORDER_ROOT);

		m_statusNotMingCard = Sprite::create("res/Images/GameScene/Label/status_notming.png");
		CC_BREAK_IF(!m_statusNotMingCard);
		m_statusNotMingCard->setPosition(statusPos);
		this->addChild(m_statusNotMingCard, BASIC_ZORDER_ROOT);
		
		m_statusSuperJiaBei = Sprite::create("res/Images/GameScene/Label/status_superjiabei.png");
		CC_BREAK_IF(!m_statusSuperJiaBei);
		m_statusSuperJiaBei->setPosition(Vec2(statusPos.x + 40, statusPos.y));
		this->addChild(m_statusSuperJiaBei, BASIC_ZORDER_ROOT);

		m_statusJiaBei = Sprite::create("res/Images/GameScene/Label/status_jiabei.png");
		CC_BREAK_IF(!m_statusJiaBei);
		m_statusJiaBei->setPosition(statusPos);
		this->addChild(m_statusJiaBei, BASIC_ZORDER_ROOT);

		m_statusNotJiaBei = Sprite::create("res/Images/GameScene/Label/status_bujiabei.png");
		CC_BREAK_IF(!m_statusNotJiaBei);
		m_statusNotJiaBei->setPosition(statusPos);
		this->addChild(m_statusNotJiaBei, BASIC_ZORDER_ROOT);

		m_statusPass = Sprite::create("res/Images/GameScene/Label/status_notout.png");
		CC_BREAK_IF(!m_statusPass);
		m_statusPass->setPosition(statusPos);
		this->addChild(m_statusPass, BASIC_ZORDER_ROOT);

		hidePlayStatus();

		bRet = true;

	}while(0);

	return bRet;
}

//设置显示续局工具栏
void GamePlayerLayer::setShowGameOverFrame(bool isShow)
{
	if(isShow && b_isGameOverTimeUp){
		b_isGameOverTimeUp = false;
		return;
	}

	b_lLeaveGame->setVisible(isShow); //离开游戏
	b_lContinueGame->setVisible(isShow); //再来一局

	if( !isShow )
	{
		std::string timeStr = StringUtils::format(GetWWString(g_over_times,g_over_section).c_str(),15);
		b_lTimeTx->setString(timeStr);
		return;
	}

	if (UserDefault::getInstance()->getBoolForKey(SET_CONTINU_KEY,true))//自动续局开关
	{
		//文字
		b_lContinueTx->setPosition(Vec2(-b_lContinueTx->getContentSize().width*0.3f,0));
		//倒计时
		b_lTimeTx->setPosition(Vec2(b_lContinueGame->getContentSize().width*0.25f,0));
		b_lTimeTx->setVisible(true);
	}
	else
	{
		//文字
		b_lContinueTx->setPosition(Vec2(0,0));
		//倒计时
		b_lTimeTx->setVisible(false);
	}
}

void GamePlayerLayer::updateGameOverTime(int time) //更新结算倒计时
{
	if(time >= 0 && time <=15){
		std::string timeStr = StringUtils::format(GetWWString(g_over_times,g_over_section).c_str(),time);
		b_lTimeTx->setString(timeStr);
	}
	else
	{
		b_isGameOverTimeUp = true;
		b_lTimeTx->setVisible(false);
	}
}

void GamePlayerLayer::updateLeftCardCount(int changeCount) //更新自己剩余牌数
{
	this->setLeftPokersCount( getLeftPokersCount() + changeCount );
}

void GamePlayerLayer::updateLeftCardCountById(int userId, int changeCount) //更新玩家剩余牌
{
	if(userId == getUserid()){
		updateLeftCardCount(changeCount);
	}else if( userId == l_npcLeft->getUserid() ){
		l_npcLeft->updateLeftCardCount(changeCount);
	}else if( userId == l_npcRight->getUserid() ){
		l_npcRight->updateLeftCardCount(changeCount);
	}
}

void GamePlayerLayer::initPlayerData(GameStartData* startData)
{
	if(startData != nullptr)
	{
		int selfIndex = 0;
		for (int i = 0; i < startData->getPlayerIds().size(); ++i)
		{
			if(UserData::getInstance()->getIntValue(INT_USER_ID) == startData->getPlayerIds()[i])
			{
				selfIndex = i;
				break;
			}
		}

		//保存自己信息
		this->setUserid(startData->getPlayerIds()[selfIndex]);
		this->setNickName(startData->getPlayerNickNames()[selfIndex]);
		this->setVip(startData->getPlayerVips()[selfIndex]);
		this->setHeadImgId(startData->getPlayerIcons()[selfIndex]);
		this->setCash(startData->getPlayerCashs()[selfIndex]);
		this->setJiFenLevel(startData->getJiFenLevel()[selfIndex]);
		this->setGender(startData->getGenders()[selfIndex]);
		this->setOnlineType(startData->getOnlineTypes()[selfIndex]);
		this->setBean(startData->getPlayerCashs()[selfIndex]);
		if(startData->getQiangDizhus().size() > 0)
		{
			this->setIsQiangdizhu(startData->getQiangDizhus()[selfIndex]);
		}
		this->setLeftPokersCount(17);

		//保存左边玩家信息
		int leftIndex = selfIndex - 1 < 0 ? 2 : selfIndex - 1;
		l_npcLeft->setUserid(startData->getPlayerIds()[leftIndex]);
		l_npcLeft->setNickName(startData->getPlayerNickNames()[leftIndex]);
		l_npcLeft->setVip(startData->getPlayerVips()[leftIndex]);
		l_npcLeft->setHeadImgId(startData->getPlayerIcons()[leftIndex]);
		l_npcLeft->setCash(startData->getPlayerCashs()[leftIndex]);
		l_npcLeft->setJiFenLevel(startData->getJiFenLevel()[leftIndex]);
		l_npcLeft->setGender(startData->getGenders()[leftIndex]);
		l_npcLeft->setOnlineType(startData->getOnlineTypes()[leftIndex]);
		l_npcLeft->setBean(startData->getPlayerCashs()[leftIndex]);
		if(startData->getQiangDizhus().size() > 0)
		{
			l_npcLeft->setIsQiangdizhu(startData->getQiangDizhus()[leftIndex]);
		}
		l_npcLeft->setLeftPokersCount(17);

		//保存右边玩家信息
		int rightIndex = selfIndex + 1 > 2 ? 0 : selfIndex + 1;
		l_npcRight->setUserid(startData->getPlayerIds()[rightIndex]);
		l_npcRight->setNickName(startData->getPlayerNickNames()[rightIndex]);
		l_npcRight->setVip(startData->getPlayerVips()[rightIndex]);
		l_npcRight->setHeadImgId(startData->getPlayerIcons()[rightIndex]);
		l_npcRight->setCash(startData->getPlayerCashs()[rightIndex]);
		l_npcRight->setJiFenLevel(startData->getJiFenLevel()[rightIndex]);
		l_npcRight->setGender(startData->getGenders()[rightIndex]);
		l_npcRight->setOnlineType(startData->getOnlineTypes()[rightIndex]);
		l_npcRight->setBean(startData->getPlayerCashs()[rightIndex]);
		if(startData->getQiangDizhus().size() > 0)
		{
			l_npcRight->setIsQiangdizhu(startData->getQiangDizhus()[rightIndex]);
		}
		l_npcRight->setLeftPokersCount(17);
	} 
}

//牌局结束重置数据
void GamePlayerLayer::gameOverReset()
{
	b_isGameOverTimeUp = false;
	hideAllPlayerStatus();
	hideAllBtnFrame();
	endCountdown();

	if(l_npcLeft){
		l_npcLeft->cleanPoker();
		/*l_npcLeft->hideUserInfo();*/
	}
	if(l_npcRight){
		l_npcRight->cleanPoker();
		/*l_npcRight->hideUserInfo();*/
	}
}

// 开局重置 gameScene 数据，目前没清除牌
void GamePlayerLayer::reset()
{
	b_isGameOverTimeUp = false;
	hideAllPlayerStatus();
	hideAllBtnFrame();
	endCountdown();
	resetHead();
	
	m_pIsCallLord = false;
	if(l_npcLeft){
		l_npcLeft->setpIsCallLord(false);
		l_npcLeft->setpIsOutCard(false);
		l_npcLeft->cleanPoker();
		l_npcLeft->resetHead();
		l_npcLeft->hideUserInfo();
	}
	if(l_npcRight){
		l_npcRight->setpIsCallLord(false);
		l_npcRight->setpIsOutCard(false);
		l_npcRight->cleanPoker();
		l_npcRight->resetHead();
		l_npcRight->hideUserInfo();
	}
}

void GamePlayerLayer::setPlayerStatus(int statusTag, PlayerSite seat)
{
	if(seat == PlayerSite::kSiteLeft){
		l_npcLeft->setPlayStatus(statusTag);
	}else if (seat == PlayerSite::kSiteRight){
		l_npcRight->setPlayStatus(statusTag);
	}else{
		setPlayStatus(statusTag);
		if( statusTag == BTN_GAMEPLAYERLAYER_BUCHU_TAG){ // 不出
			GameLogicManager::getInstance()->getpUIMyCardLayer()->putCardsDown();
		}
	}
}

void GamePlayerLayer::updateUserInfo()
{
	l_npcLeft->updateUserInfo();
	l_npcRight->updateUserInfo();

	auto l_myInfoBg = this->getChildByTag(BTN_GAMEPLAYERLAYER_MYCOIN_AREA_TAG);
	auto l_myCoinTx = (Label*)l_myInfoBg->getChildByTag(BTN_GAMEPLAYERLAYER_MYCOIN_TAG);
	char myCoinStr[30];
	sprintf(myCoinStr,"%d",getBean());
	l_myCoinTx->setString(myCoinStr);
}

void GamePlayerLayer::showMinPaiResult(const cocos2d::Vector<PokerObject*> &vecPokers, PlayerSite seat)
{
	if(seat == PlayerSite::kSiteLeft){
		//l_npcLeft->addMingPai(vecPokers);
		l_npcRight->setPlayStatus(BTN_GAMEPLAYERLAYER_MINTPAI_TAG);
	}else if (seat == PlayerSite::kSiteRight){
		//l_npcLeft->addMingPai(vecPokers);
		l_npcRight->setPlayStatus(BTN_GAMEPLAYERLAYER_MINTPAI_TAG);
	}else{
		setPlayStatus(BTN_GAMEPLAYERLAYER_MINTPAI_TAG);
	}
}

void GamePlayerLayer::setPlayerRole(PlayerSite landLordSite)
{
	if(landLordSite == PlayerSite::kSiteLeft){
		l_npcLeft->updateRole(LANDLORDHEAD);
		updateRole(FARMERHEAD);
		l_npcRight->updateRole(FARMERHEAD);
	}else if (landLordSite == PlayerSite::kSiteRight){
		l_npcRight->updateRole(LANDLORDHEAD);
		updateRole(FARMERHEAD);
		l_npcLeft->updateRole(FARMERHEAD);
	}else{
		updateRole(LANDLORDHEAD);
		l_npcLeft->updateRole(FARMERHEAD);
		l_npcRight->updateRole(FARMERHEAD);
	}
}

void GamePlayerLayer::hidePlayerStatus(PlayerSite seat)
{
	if(seat == PlayerSite::kSiteLeft){
		l_npcLeft->hidePlayStatus();
	}else if (seat == PlayerSite::kSiteRight){
		l_npcRight->hidePlayStatus();
	}else{
		hidePlayStatus();
	}
}

void GamePlayerLayer::hideAllPlayerStatus()
{
	hidePlayStatus();
	l_npcLeft->hidePlayStatus();
	l_npcRight->hidePlayStatus();
}

void GamePlayerLayer::startToCountdown(int seconds, PlayerSite seat)
{
	m_nTime = seconds;
	if (m_nTime > 0)  
	{
		char szNum[4] = {0};
		sprintf(szNum, "%d", m_nTime);
		m_pTimeLabel->setString(szNum);

		schedule(schedule_selector(GamePlayerLayer::tick), 1.0f, kRepeatForever, 0.0f);
		m_pClock->setVisible(true);
		m_pClock->stopAllActions(); //先停止闹钟移动动画，避免闹钟位置错乱
		if(seat == PlayerSite::kSiteCenter){
			auto forword = MoveTo::create(0.4f,Vec2(1920/2, 1080/2 + 100));//屏幕中间
			m_pClock->runAction(forword);
		}else if(seat == PlayerSite::kSiteLeft){
			auto forword = MoveTo::create(0.4f,Vec2(465,827));
			m_pClock->runAction(forword);
			//m_pClock->setPosition(Vec2(465,827));
		}else if (seat == PlayerSite::kSiteRight){
			auto forword = MoveTo::create(0.4f,Vec2(1456,827));
			m_pClock->runAction(forword);
			//m_pClock->setPosition(Vec2(1456,827));
		}else{
			auto forword = MoveTo::create(0.4f,Vec2(715, 426));
			m_pClock->runAction(forword);
			//m_pClock->setPosition(Vec2(715, 426));
		}
	}
}

void GamePlayerLayer::tick(float ft)
{
	if (m_nTime >= 1)
	{
		m_nTime--;
		char szNum[4] = {0};
		sprintf(szNum, "%d", m_nTime);
		m_pTimeLabel->setString(szNum);
		
		// TODO 剩余 5 秒时，音效、震动提示
	}
	else
	{
		endCountdown();
		//超时后，UI逻辑调整
		GameLogicManager::getInstance()->onClockTimeOut();
	}
}

void GamePlayerLayer::endCountdown()
{
	m_pClock->setVisible(false);

	unschedule(schedule_selector(GamePlayerLayer::tick));
}
void GamePlayerLayer::setOutBtnEnable(bool isEnable)
{
	b_lchupai->setEnabled(isEnable);
}

void GamePlayerLayer::setDownBtnEnable(bool isEnable)
{
	b_lDown->setEnabled(isEnable);
}

void GamePlayerLayer::setPassBtnEnable(bool isEnable)
{
	b_lUnOut->setEnabled(isEnable);
}

void GamePlayerLayer::setSuperJiaBeiBtnEnable(bool isEnable)
{
	b_lSurperJiaBei->setEnabled(isEnable);
}

void GamePlayerLayer::setShowPlayCardFrame(bool isShow)
{
	b_lUnOut->setVisible(isShow);
	b_lTips->setVisible(isShow);
	b_lDown->setVisible(isShow);
	b_lchupai->setVisible(isShow);
}

void GamePlayerLayer::setShowLandlordFrame(bool isShow, int noCall)
{
	b_lJiaoDiZhu->setVisible(isShow);
	b_lBuJiaoDiZhu->setVisible(isShow);
	if (noCall == 3) //叫抢地主逻辑改动
		b_lBuJiaoDiZhu->setEnabled(false);
	else
		b_lBuJiaoDiZhu->setEnabled(true);
}

void GamePlayerLayer::setShowGraplordFrame(bool isShow)
{
	b_lGrapLord->setVisible(isShow);
	b_lNotGrapLord->setVisible(isShow);
}

void GamePlayerLayer::setShowMingPaiFrame(bool isShow)
{
	b_lMingpai->setVisible(isShow);
	b_lBuMingpai->setVisible(isShow);
}

void GamePlayerLayer::setShowJiaBeiFrame(bool isShow)
{
	b_lSurperJiaBei->setVisible(isShow);
	b_lJiaBei->setVisible(isShow);
	b_lBuJiaBei->setVisible(isShow);
}

void GamePlayerLayer::hideAllBtnFrame()
{
	setShowPlayCardFrame(false);
	setShowLandlordFrame(false,0);
	setShowGraplordFrame(false);
	setShowMingPaiFrame(false);
	setShowJiaBeiFrame(false);
	setShowGameOverFrame(false);
}

void GamePlayerLayer::dealNpcPoker(PlayerSite seat, int count, bool isResume )
{
	if(seat == PlayerSite::kSiteLeft){
		l_npcLeft->dealAction(count, !isResume);
	}else if(seat == PlayerSite::kSiteRight){
		l_npcRight->dealAction(count, !isResume);
	}
}

void GamePlayerLayer::setAutoPlay(bool isAutoPlay, PlayerSite seat)
{
	//TODO 展示玩家托管状态……
	if( seat == PlayerSite::kSiteLeft ){
	}else if( seat == PlayerSite::kSiteRight ){
	}
}

/* 页面按纽响应 */
void GamePlayerLayer::onTouchSpriteDepress(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	switch (tagId)
	{
	case BTN_GAMEPLAYERLAYER_BUCHU_TAG: //不出
		{
			log("BTN_GAMEPLAYERLAYER_BUCHU_TAG press!");
			GameLogicManager::getInstance()->requestPass();
		}
		break;
	case BTN_GAMEPLAYERLAYER_TISHI_TAG: //提示
		{
			log("BTN_GAMEPLAYERLAYER_TISHI_TAG press!");
			GameLogicManager::getInstance()->getpUIMyCardLayer()->onTipBtnClick();
		}
		break;
	case BTN_GAMEPLAYERLAYER_FANGXIA_TAG: //放下
		{
			log("BTN_GAMEPLAYERLAYER_FANGXIA_TAG press!");
			GameLogicManager::getInstance()->getpUIMyCardLayer()->putCardsDown();
		}
		break;
	case BTN_GAMEPLAYERLAYER_CHUPAI_TAG: //出牌
		{
			log("BTN_GAMEPLAYERLAYER_CHUPAI_TAG press!");
			setShowPlayCardFrame(false);
			GameLogicManager::getInstance()->getpUIMyCardLayer()->onOutCardBtnClick();
		}
		break;
	case BTN_GAMEPLAYERLAYER_JIAODIZHU_TAG: //叫地主
		{
			log("BTN_GAMEPLAYERLAYER_JIAODIZHU_TAG press!");
			setShowLandlordFrame(false,0);
			MsgLordGameRequest::sendSelfCallLord(true);
		}
		break;
	case BTN_GAMEPLAYERLAYER_BUJIAODIZHU_TAG: //不叫地主
		{
			log("BTN_GAMEPLAYERLAYER_BUJIAODIZHU_TAG press!");
			setShowLandlordFrame(false,0);

			MsgLordGameRequest::sendSelfCallLord(false);
		}
		break;
	case BTN_GAMEPLAYERLAYER_GRAPLORD_TAG: //抢地主
		{
			log("BTN_GAMEPLAYERLAYER_GRAPLORD_TAG press!");
			setShowGraplordFrame(false);
			MsgLordGameRequest::sendSelfGrabLord(true);
		}
		break;
	case BTN_GAMEPLAYERLAYER_NOTGRAPLORD_TAG: //不抢地主
		{
			log("BTN_GAMEPLAYERLAYER_NOTGRAPLORD_TAG press!");
			setShowGraplordFrame(false);
			MsgLordGameRequest::sendSelfGrabLord(false);
		}
		break;
	case BTN_GAMEPLAYERLAYER_MINTPAI_TAG: //明牌
		{
			log("BTN_GAMEPLAYERLAYER_MINTPAI_TAG press!");
			setShowMingPaiFrame(false);
			GameLogicManager::getInstance()->getpUIMyCardLayer()->onMinPaiBtnClick();
		}
		break;
	case BTN_GAMEPLAYERLAYER_BUMINGPAI_TAG: //不明牌
		{
			log("BTN_GAMEPLAYERLAYER_BUMINGPAI_TAG press!");
			setShowMingPaiFrame(false);
			GameLogicManager::getInstance()->requestNoMinPai();
		}
		break;
		
	case BTN_GAMEPLAYERLAYER_SUPER_JIABEI_TAG: //超级加倍
		{
			log("BTN_GAMEPLAYERLAYER_SUPER_JIABEI_TAG press!");
			setShowJiaBeiFrame(false);
			MsgLordGameRequest::sendJiaBei(2); //(int1)  0:不加倍  1:加倍  2：加强加倍  3：加倍结束
		}
		break;

	case BTN_GAMEPLAYERLAYER_JIABEI_TAG: //加倍
		{
			log("BTN_GAMEPLAYERLAYER_JIABEI_TAG press!");
			setShowJiaBeiFrame(false);
			MsgLordGameRequest::sendJiaBei(1); //(int1)  0:不加倍  1:加倍  2：加强加倍  3：加倍结束
		}
		break;
		
	case BTN_GAMEPLAYERLAYER_BUJIABEI_TAG: //不加倍
		{
			log("BTN_GAMEPLAYERLAYER_BUJIABEI_TAG press!");
			setShowJiaBeiFrame(false);
			// TODO send socket msg
			MsgLordGameRequest::sendJiaBei(0);
		}
		break;
	case BIN_GAMEPLAYERLAYER_LEAVEGAME_TAG: //离开游戏
		{
			log("BIN_GAMEPLAYERLAYER_LEAVEGAME_TAG press!");
			setShowGameOverFrame(false);
			GameLogicManager::getInstance()->getpUIgameScene()->endGameOverTime();
			GameLogicManager::getInstance()->TopMenuReturnCallback();
		}
		break;
		
	case BIN_GAMEPLAYERLAYER_CONTINUE_TAG: //续局
		{
			log("BIN_GAMEPLAYERLAYER_CONTINUE_TAG press!");
			setShowGameOverFrame(false);
			GameLogicManager::getInstance()->GameOverContinueCallBack();
		}
		break;

	case BTN_GAMEPLAYERLAYER_ADDCOIN_TAG: //加金币
		{
			log("BTN_GAMEPLAYERLAYER_ADDCOIN_TAG press!");
		}
		break;
	case BIN_GAMEPLAYERLAYER_RECORD_TAG://记牌器
		{
			log("BIN_GAMEPLAYERLAYER_RECORD_TAG press!");
			addRecord();
		}
		break;
	default:
		break;
	}
}

void GamePlayerLayer::addRecord()
{
	
	if (!m_pIsUsedRecord)
		b_recordBg->setVisible(true);
	else
		b_recordBg->setVisible(false);
	m_pIsUsedRecord = !m_pIsUsedRecord;
}

void GamePlayerLayer::updataRecord(Vector<PokerObject*> outCardsArr)
{
	//log("update record....");
	b_recordBg->removeAllChildren();
	int count =  (sizeof(indexs) / sizeof(indexs[0]));
	for (PokerObject* poker : outCardsArr)
	{
		int key = (int)(poker->getPokerNum());
		//log("update record....%d----,%d",key,pMMap[key]);
		pMMap[key] = pMMap[key] - 1;
	}
	
	for (int i = 0; i < count; i++)
	{
		auto counLabel = Label::createWithTTF(StringTool::intToString(pMMap[indexs[i]]),"res/Fonts/FounderSemiBold.ttf",45,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		counLabel->setPosition(Vec2(109+60*i, 38));
		b_recordBg->addChild(counLabel);
	}
}

void GamePlayerLayer::resetRecord()
{
	//log("reset record....");
	b_recordBg->removeAllChildren();
	b_recordBg->setVisible(false);
	m_pIsUsedRecord = !m_pIsUsedRecord;
	int count =  (sizeof(indexs) / sizeof(indexs[0]));	
	for (int i = 2; i < count; i++)
	{
		pMMap[indexs[i]] = 4;
	}
	pMMap[R_WA]=1;
	pMMap[R_WB]=1;
	for (int i = 0; i < count; i++)
	{
		auto counLabel = Label::createWithTTF(StringTool::intToString(pMMap[indexs[i]]),"res/Fonts/FounderSemiBold.ttf",45,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		counLabel->setPosition(Vec2(109+60*i, 38));
		b_recordBg->addChild(counLabel);
	}
}

void GamePlayerLayer::resetAllData()
{
}
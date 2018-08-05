/*
 * GameBgLayer.h
 *
 *
 *  Created on: 2015-5-11
 *      Author: diyal.yin
 */

#include "GameTopMenuLayer.h"
#include "../GameSceneDefine.h"
#include "WWMacros.h"
#include "BaseCore/Common/FactoryTools.h"
#include "View/GameScene/GameLogicManager.h"
#include "ThirdPlatform/PlatformUtils.h"
#include "Control/WWDataManager/WWDataManager.h"


USING_NS_CC;
const char* game_top_bar = "res/Images/GameScene/GameTopBar/gs_topbar.plist";

GameTopMenuLayer::GameTopMenuLayer(void)
{
	/*auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(GameTopMenuLayer::onTouchBegan, this);
	m_pListner->onTouchEnded = CC_CALLBACK_2(GameTopMenuLayer::onTouchEnded, this);

	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);*/
}


GameTopMenuLayer::~GameTopMenuLayer(void)
{

}

bool GameTopMenuLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0,0,0,0))) return false;
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(game_top_bar);

	bool bRet = false;
	do 
	{
		createView();

		bRet = true;
	}
	while (0);

	return bRet;
}

void GameTopMenuLayer::onEnter()
{
	LayerColor::onEnter();
	this->schedule(schedule_selector(GameTopMenuLayer::updateAllStatu),60.0f,kRepeatForever,0.0f);
	//不加载资源图片重连时报错
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(game_top_bar);
}

void GameTopMenuLayer::onExit()
{
	LayerColor::onExit();
	this->unschedule(schedule_selector(GameTopMenuLayer::updateAllStatu));
	//释放资源
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(game_top_bar);
}

void GameTopMenuLayer::createView()
{
	m_pBgSprite = Sprite::createWithSpriteFrameName("gs_top_menu_bg.png");
	m_pBgSprite->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5, CUR_SCREEN_SIZE.height - m_pBgSprite->getContentSize().height * 0.5));
	this->addChild(m_pBgSprite, BASIC_ZORDER_ROOT);
	
	//电量背景
	auto s_lBattery = Sprite::createWithSpriteFrameName("gs_battry.png");
	s_lBattery->setPosition(CENTER_A_CONVERT(m_pBgSprite, -273, -10.0));
	m_pBgSprite->addChild(s_lBattery, BASIC_ZORDER_ROOT);
	//电池状态
	m_pBatteryStatu = LoadingBar::create("res/Images/GameScene/battryPrent.png", 100);
	m_pBatteryStatu->setPosition(Vec2(s_lBattery->getContentSize().width / 2, s_lBattery->getContentSize().height / 2));
	s_lBattery->addChild(m_pBatteryStatu);
	//时间
	m_pTimeStatu = Label::create();
	m_pTimeStatu->setSystemFontSize(31.0f);
	m_pTimeStatu->setColor(GET_RGBCOLOR(0x5c3223));
	m_pTimeStatu->setPosition(CENTER_A_CONVERT(m_pBgSprite, -245, 29));
	m_pBgSprite->addChild(m_pTimeStatu,BASIC_ZORDER_ROOT);

	//信号
	m_pNetStatus = Sprite::createWithSpriteFrameName("gs_wifi_4.png");
	m_pNetStatus->setPosition(CENTER_A_CONVERT(m_pBgSprite, -213, -9.0));
	m_pBgSprite->addChild(m_pNetStatus, BASIC_ZORDER_ROOT);
	//任务
	//赢足xx送
	m_pWinMax = Label::create();
	m_pWinMax->setSystemFontSize(28.0f);
	m_pWinMax->setColor(GET_RGBCOLOR(0xffffff));
	m_pWinMax->setPosition(CENTER_A_CONVERT(m_pBgSprite,-65, 31));
	m_pBgSprite->addChild(m_pWinMax,BASIC_ZORDER_ROOT);
	//奖品icon
	m_pKey = Sprite::createWithSpriteFrameName("gs_top_key.png");
	m_pKey->setPosition(CENTER_A_CONVERT(m_pBgSprite,-105,-10));
	m_pBgSprite->addChild(m_pKey,BASIC_ZORDER_ROOT);
	//奖品数量
	m_pKeyNum = Label::create();
	m_pKeyNum->setSystemFontSize(36.0f);
	m_pKeyNum->setColor(GET_RGBCOLOR(0xffffff));
	m_pKeyNum->setPosition(Vec2(m_pKey->getPositionX()+m_pKey->getContentSize().width,m_pKey->getPositionY()));
	m_pBgSprite->addChild(m_pKeyNum,BASIC_ZORDER_ROOT);
	

	//倍数
	std::string multipleStr = GetWWString(g_Game_TopMenu_Multiple,g_GameSence_Toast);
	auto l_lBeishu = Label::createWithTTF(multipleStr,"res/Fonts/FounderSemiBold.ttf",30,Size(200,0),TextHAlignment::LEFT,TextVAlignment::CENTER);
	l_lBeishu->setColor(GET_RGBCOLOR(16643283));
	l_lBeishu->setTag(BTN_GAMETOPMENU_MULTIPLE_TAG);
	l_lBeishu->setPosition(Vec2(CENTER_A_CONVERT(m_pBgSprite, 150, 35.0)));
	m_pBgSprite->addChild(l_lBeishu,BASIC_ZORDER_ROOT);

	//底注
	std::string baseStr = GetWWString(g_Game_TopMenu_Base,g_GameSence_Toast);
	auto l_lDizhu = Label::createWithTTF(baseStr,"res/Fonts/FounderSemiBold.ttf",30,Size(200,0),TextHAlignment::LEFT,TextVAlignment::CENTER);
	l_lDizhu->setColor(GET_RGBCOLOR(16643283));
	l_lDizhu->setTag(BTN_GAMETOPMENU_BASE_TAG);
	l_lDizhu->setPosition(Vec2(CENTER_A_CONVERT(m_pBgSprite, 150, -8.0)));
	m_pBgSprite->addChild(l_lDizhu,BASIC_ZORDER_ROOT);


	updateAllStatu();
	showKeyWin();
}

void GameTopMenuLayer::updateBase(int base)
{
	Label* l_lDizhu = (Label*)m_pBgSprite->getChildByTag(BTN_GAMETOPMENU_BASE_TAG);
	std::string baseStr = GetWWString(g_Game_TopMenu_Base,g_GameSence_Toast);
	if( base <= 0 ){
		l_lDizhu->setString(baseStr.c_str());
	}else{
		std::string baseText = StringUtils::format("%s%d", baseStr.c_str(), base);
		l_lDizhu->setString(baseText.c_str());
	}
}


void GameTopMenuLayer::updateMultiple(int multi)
{
	Label* l_lBeishu = (Label*)m_pBgSprite->getChildByTag(BTN_GAMETOPMENU_MULTIPLE_TAG);
	std::string multipleStr = GetWWString(g_Game_TopMenu_Multiple,g_GameSence_Toast);
	if( multi <= 0 ){
		std::string multiTx = StringUtils::format("%s%d", multipleStr.c_str(), 1);
		l_lBeishu->setString(multiTx.c_str());
	}else{
		std::string multiTx = StringUtils::format("%s%d", multipleStr.c_str(), multi);
		l_lBeishu->setString(multiTx.c_str());
	}
}

void GameTopMenuLayer::setSubstituteEnable(bool isEnable)
{
	
}

void GameTopMenuLayer::updateAllStatu(float dt)
{
	updateBatteryStatu();
	updateTimeStatu();
	updateNetStatu();
}

void GameTopMenuLayer::updateTimeStatu()
{
	do {
		CC_BREAK_IF(!m_pTimeStatu);
		
		m_pTimeStatu->setString(PlatformUtils::getFormatCurTime());
	} while (0);
}

void GameTopMenuLayer::updateNetStatu()
{
	do{
		CC_BREAK_IF(!m_pNetStatus);
		int netStatus = 0;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		netStatus = PlatformUtils::getNetStatus();
#endif
		std::string apnType = WWDataManager::getInstance()->deviceInfo.apnType.c_str();
		if (apnType == "wifi")
		{
			if (netStatus == 0)
			{
				m_pNetStatus->setSpriteFrame("gs_wifi_1.png");
			}
			else
			{
				std::string path = StringUtils::format("gs_wifi_%d.png",netStatus);
				m_pNetStatus->setSpriteFrame(path);
			}
		}
		else
		{
			if (netStatus == 0)
			{
				m_pNetStatus->setSpriteFrame("gs_signal_1.png");
			}
			else
			{
				std::string path = StringUtils::format("gs_signal_%d.png",netStatus);
				m_pNetStatus->setSpriteFrame(path);
			}
		}
	}while(0);
}

void GameTopMenuLayer::updateBatteryStatu()
{
	do
	{
		CC_BREAK_IF(!m_pBatteryStatu);
		int batteryInt = 0;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		batteryInt = PlatformUtils::getBatteryPersent();
#endif
		m_pBatteryStatu->setPercent((float) batteryInt);
		log(" StatusBarSprite::updateBatteryStatu");
	} while (0);

}

void GameTopMenuLayer::showKeyWin()
{
	std::string winTxt = StringUtils::format(GetWWString(g_Game_top_key,g_GameSence_Toast).c_str(),"10W");
	m_pWinMax->setString(winTxt);
	std::string keyTxt = StringUtils::format("X%s","3");
	m_pKeyNum->setString(keyTxt);
	m_pWinMax->setVisible(true);
	m_pKey->setVisible(true);
	m_pKeyNum->setVisible(true);
}

void GameTopMenuLayer::unShowKeyWin()
{
	m_pWinMax->setVisible(false);
	m_pKey->setVisible(false);
	m_pKeyNum->setVisible(false);
}


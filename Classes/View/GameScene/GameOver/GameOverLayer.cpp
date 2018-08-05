#include "GameOverLayer.h"

#include "WWMacros.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"
#include "GameOverLayerRes.h"
#include "../../Common/CommLayer/CommDialog.h"
#include "../../../Model/WWDataModel/WWGameData/GameCommonData.h"
#include "../GameLogicManager.h"
#include "BaseCore/WWBTools/StringTool.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "Control/WWCSound/GameSoundManager.h"

USING_NS_CC;

GameOverLayer::GameOverLayer(void) :
	m_times(15)
{
}

GameOverLayer::~GameOverLayer()
{
	
}


void GameOverLayer::onEnter()
{
	LayerColor::onEnter();
	//禁止触摸穿透
	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(GameOverLayer::onTouchBegan, this);
	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(game_over_plist);
}

void GameOverLayer::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	LayerColor::onExit();
	//资源释放
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(game_over_plist);
}

GameOverLayer* GameOverLayer::create(cocos2d::Ref* objectData)
{
	GameOverLayer* pItem = new GameOverLayer();
	if(pItem && pItem->init(objectData))
	{
		pItem->autorelease();
		return pItem;
	}
	else
	{
		CC_SAFE_DELETE(pItem);
		return NULL;
	}
}


bool GameOverLayer::init(cocos2d::Ref* objectData)
{
	if (!WWFrameBase::init()) return false;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(game_over_plist);
	createView(objectData);

	return true;
}

void GameOverLayer::createView(cocos2d::Ref* objectData)
{
	do{
		GameOverCommonData* gameOverData = (GameOverCommonData*) objectData;
		if(gameOverData == NULL)return;
		
		//得到自己的 index
		int selfIndex = 0;
		for (int i = 0; i < gameOverData->getUserIds().size(); ++i)
		{
			if(UserData::getInstance()->getIntValue(INT_USER_ID) == gameOverData->getUserIds()[i])
			{
				selfIndex = i;
				break;
			}
		} 
		int rightIndex = selfIndex + 1 > 2 ? 0 : selfIndex + 1;
		int leftIndex = selfIndex - 1 < 0 ? 2 : selfIndex - 1;

		//背景
		auto bg = this->getpBg();
		bg->setPosition(POSTIONS_IMG[0]);
		if (gameOverData->getPlayResults()[selfIndex] == 1)
			bg->setSpriteFrame("go_win_bg.png");//bg->setTexture(winBg);
		else
			bg->setSpriteFrame("go_lose_bg.png");//bg->setTexture(loseBg);
		//关闭按钮
		auto closeBtn = this->getpClose();
		closeBtn->setPosition(POSTIONS_IMG[2]);
		//形象
		auto man = Sprite::createWithSpriteFrameName("go_lord_win_bg.png");
		CC_BREAK_IF(!man);
		man->setPosition(POSTIONS_IMG[1]);
		bg->addChild(man, BASIC_ZORDER_ROOT);
		if (gameOverData->getUserIds()[selfIndex] == GameLogicManager::getInstance()->getLandLordId())//判断自己是否是地主 
		{
			if (gameOverData->getPlayResults()[selfIndex] == 1)//判断是赢是输
			{
				man->setSpriteFrame("go_lord_win_bg.png");//man->setTexture(lordWinBigImg);
			}
			else
			{
				man->setSpriteFrame("go_lord_lose_bg.png");//man->setTexture(lordLoseBigImg);
			}
		}
		else
		{
			if (gameOverData->getPlayResults()[selfIndex] == 1)
			{
				man->setSpriteFrame("go_farmer_win_bg.png");//man->setTexture(farmerWinBigImg);
			}
			else
			{
				man->setSpriteFrame("go_farmer_lose_bg.png");//man->setTexture(farmerLoseBigImg);
			}
		}
		//金币icon
		auto beanIcon = Sprite::createWithSpriteFrameName("go_gold_icon.png");
		CC_BREAK_IF(!beanIcon);
		beanIcon->setPosition(POSTIONS_IMG[3]);
		bg->addChild(beanIcon, BASIC_ZORDER_ROOT);
		//升级背景
		auto lvUpBg = Sprite::createWithSpriteFrameName("go_lv_up.png");
		CC_BREAK_IF(!lvUpBg);
		lvUpBg->setPosition(POSTIONS_IMG[6]);
		bg->addChild(lvUpBg,BASIC_ZORDER_ROOT);
		//倍数
		int mult = GameLogicManager::getInstance()->getMultiple();
		mult = mult > 0 ? mult:1;
		log("mult = %d", mult);
		auto beishuLabel = LabelAtlas::create(StringTool::intToString(mult),beishu,68,78,'0');
		beishuLabel->setAnchorPoint(Vec2(0,0.5));
		beishuLabel->setPosition(POSTIONS_IMG[4]);
		bg->addChild(beishuLabel,BASIC_ZORDER_ROOT);
		//倍字
		auto beiSprite = Sprite::createWithSpriteFrameName("go_bei_txt.png");
		beiSprite->setPosition(Vec2(beishuLabel->getPositionX()+beishuLabel->getContentSize().width+beiSprite->getContentSize().width*0.5f,beishuLabel->getPositionY()));
		bg->addChild(beiSprite,BASIC_ZORDER_ROOT);

		//加号减号 //需要判断是加还是减号
		auto fuhao = Sprite::createWithSpriteFrameName("go_jia.png");
		fuhao->setAnchorPoint(Vec2(0,0.5));
		fuhao->setPosition(POSTIONS_IMG[5]);
		bg->addChild(fuhao,BASIC_ZORDER_ROOT);
		if (gameOverData->getPlayResults()[selfIndex] != 1)
		{
			fuhao->setSpriteFrame("go_jian.png");//fuhao->setTexture(jian);
		}		
		else
		{
			fuhao->setSpriteFrame("go_jia.png");//fuhao->setTexture(jia);
		}
		//自己金币变化 //加减
		std::string jinbTxt = gameOverData->getFortunes()[selfIndex];
		log("jinb change = %s", jinbTxt.c_str());
		if (gameOverData->getPlayResults()[selfIndex] == 1)//自己赢
		{
			auto jinbiAdd = LabelAtlas::create(jinbTxt,addNum,74,76,'0');
			jinbiAdd->setAnchorPoint(Vec2(0,0.5));
			jinbiAdd->setPosition(Vec2(fuhao->getPositionX()+fuhao->getContentSize().width,fuhao->getPositionY()));
			bg->addChild(jinbiAdd,BASIC_ZORDER_ROOT);
		}
		else
		{
			auto jinbiAdd = LabelAtlas::create(jinbTxt.substr(1,jinbTxt.length()),jianNum,74,76,'0');
			jinbiAdd->setAnchorPoint(Vec2(0,0.5));
			jinbiAdd->setPosition(Vec2(fuhao->getPositionX()+fuhao->getContentSize().width,fuhao->getPositionY()));
			bg->addChild(jinbiAdd,BASIC_ZORDER_ROOT);
		}
		
		//上家区域
	
		auto lastHeadBg = Sprite::createWithSpriteFrameName("go_farmer_lose_icon.png");
		CC_BREAK_IF(!lastHeadBg);
		lastHeadBg->setPosition(POSTIONS_IMG[7]);
		bg->addChild(lastHeadBg,BASIC_ZORDER_ROOT);
		if (gameOverData->getUserIds()[leftIndex] == GameLogicManager::getInstance()->getLandLordId())//判断上家是否地主  先暂时为地主
		{
			if (gameOverData->getPlayResults()[leftIndex] == 1)
			{
				lastHeadBg->setSpriteFrame("go_lord_win_icon.png");//lastHeadBg->setTexture(lordWinSmallImg);
			}
			else
			{
				lastHeadBg->setSpriteFrame("go_lord_lose_icon.png");//lastHeadBg->setTexture(lordLoseSmallImg);
			}
		}
		else
		{
			if (gameOverData->getPlayResults()[leftIndex] == 1)
			{
				lastHeadBg->setSpriteFrame("go_farmer_win_icon.png");//lastHeadBg->setTexture(farmerWinSmallImg);
			}
			else
			{
				lastHeadBg->setSpriteFrame("go_farmer_lose_icon.png");//lastHeadBg->setTexture(farmerLoseSmallImg);
			}
		}
		//上家昵称
		std::string lastTxt = StringTool::intToString(gameOverData->getUserIds()[leftIndex]);
		auto lastName = Label::createWithTTF(lastTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!lastName);
		lastName->setColor(GET_RGBCOLOR(other_txt_color));
		lastName->setAnchorPoint(Vec2(0,0.5));
		lastName->setPosition(POSTIONS_TXT[4]);
		bg->addChild(lastName,BASIC_ZORDER_ROOT);
		//上家结算
		auto lastWin = Label::createWithTTF("-25000","res/Fonts/FounderSemiBold.ttf",60,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!lastWin);
		lastWin->setColor(GET_RGBCOLOR(txt_color));
		if (gameOverData!=NULL)
		{
			std::string lastWinTxt = StringUtils::format("%s",gameOverData->getFortunes()[leftIndex].c_str());
			lastWin->setString(lastWinTxt);
		}
		lastWin->setAnchorPoint(Vec2(0,0.5));
		lastWin->setPosition(POSTIONS_TXT[5]);
		bg->addChild(lastWin,BASIC_ZORDER_ROOT);
		auto lastVip = Sprite::createWithSpriteFrameName("go_vip_icon.png");
		lastVip->setPosition(POSTIONS_IMG[10]);
		bg->addChild(lastVip,BASIC_ZORDER_ROOT);


		//下家区域	
		//下家昵称
		std::string nextTxt = StringTool::intToString(gameOverData->getUserIds()[rightIndex]);
		auto nextName = Label::createWithTTF(nextTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!nextName);
		nextName->setColor(GET_RGBCOLOR(other_txt_color));
		nextName->setAnchorPoint(Vec2(0,0.5));
		nextName->setPosition(POSTIONS_TXT[6]);
		bg->addChild(nextName,BASIC_ZORDER_ROOT);
		//下家结算
		auto nextHeadBg = Sprite::createWithSpriteFrameName("go_farmer_lose_icon.png");
		CC_BREAK_IF(!nextHeadBg);
		nextHeadBg->setPosition(POSTIONS_IMG[8]);
		bg->addChild(nextHeadBg,BASIC_ZORDER_ROOT);
		if (gameOverData->getUserIds()[rightIndex] == GameLogicManager::getInstance()->getLandLordId())//判断是否地主  先暂时为地主
		{
			if (gameOverData->getPlayResults()[rightIndex] == 1)
			{
				nextHeadBg->setSpriteFrame("go_lord_win_icon.png");//nextHeadBg->setTexture(lordWinSmallImg);
			}
			else
			{
				nextHeadBg->setSpriteFrame("go_lord_lose_icon.png");//nextHeadBg->setTexture(lordLoseSmallImg);
			}
		}
		else
		{
			if (gameOverData->getPlayResults()[rightIndex] == 1)
			{
				nextHeadBg->setSpriteFrame("go_farmer_win_icon.png");//nextHeadBg->setTexture(farmerWinSmallImg);
			}
			else
			{
				nextHeadBg->setSpriteFrame("go_farmer_lose_icon.png");//nextHeadBg->setTexture(farmerLoseSmallImg);
			}
		}
		auto nextWin = Label::createWithTTF("-25000","res/Fonts/FounderSemiBold.ttf",60,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!nextWin);
		nextWin->setColor(GET_RGBCOLOR(txt_color));
		if (gameOverData!=NULL)
		{
			std::string nextWinTxt = StringUtils::format("%s",gameOverData->getFortunes()[rightIndex].c_str());
			nextWin->setString(nextWinTxt);
		}
		nextWin->setAnchorPoint(Vec2(0,0.5));
		nextWin->setPosition(POSTIONS_TXT[7]);
		bg->addChild(nextWin,BASIC_ZORDER_ROOT);
		auto nextVip = Sprite::createWithSpriteFrameName("go_vip_icon.png");
		nextVip->setPosition(POSTIONS_IMG[12]);
		bg->addChild(nextVip,BASIC_ZORDER_ROOT);

		//宝箱区域
		//宝箱icon
		auto boxIcon = Sprite::createWithSpriteFrameName("go_box.png");
		CC_BREAK_IF(!boxIcon);
		boxIcon->setPosition(POSTIONS_IMG[9]);
		this->addChild(boxIcon, BASIC_ZORDER_ROOT+1);
	
		//说明文字
		std::string boxDescTxt = StringUtils::format(GetWWString(g_over_box_desc,g_over_section).c_str(),4,1,4);
		auto boxDescLabel = Label::createWithTTF(boxDescTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!boxDescLabel);
		boxDescLabel->setColor(GET_RGBCOLOR(box_txt_color));
		boxDescLabel->setAnchorPoint(Vec2(0,0.5));
		boxDescLabel->setPosition(POSTIONS_TXT[8]);
		this->addChild(boxDescLabel,BASIC_ZORDER_ROOT+3);
		//当前等级
		int lvInt = gameOverData->getGamePointLevels()[selfIndex];
		std::string noeLvTxt = StringUtils::format("Lv.%d",lvInt);
		auto nowLvLabel = Label::createWithTTF(noeLvTxt,"res/Fonts/FounderSemiBold.ttf",60,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!nowLvLabel);
		nowLvLabel->setColor(GET_RGBCOLOR(txt_color));
		nowLvLabel->setAnchorPoint(Vec2(0,0.5));
		nowLvLabel->setPosition(POSTIONS_TXT[0]);
		bg->addChild(nowLvLabel,BASIC_ZORDER_ROOT+2);
		//当前经验
		std::string jingyanTxt = StringUtils::format(GetWWString(g_over_jingyan,g_over_section).c_str(),gameOverData->getGamePoints()[selfIndex]);
		auto jingyanLabel = Label::createWithTTF(jingyanTxt,"res/Fonts/FounderSemiBold.ttf",30,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!jingyanLabel);
		jingyanLabel->setColor(GET_RGBCOLOR(txt_color));
		jingyanLabel->setAnchorPoint(Vec2(0,0.5));
		jingyanLabel->setPosition(POSTIONS_TXT[1]);
		bg->addChild(jingyanLabel,BASIC_ZORDER_ROOT+2);
		//获取经验
		auto addJingYanLabel = Label::createWithTTF(StringUtils::format("%d",gameOverData->getGamePoints()[selfIndex]),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!addJingYanLabel);
		addJingYanLabel->setAnchorPoint(Vec2(0,0.5));
		addJingYanLabel->setPosition(POSTIONS_TXT[2]);
		bg->addChild(addJingYanLabel,BASIC_ZORDER_ROOT+2);
		//下一等级
		std::string nextLvTxt = StringUtils::format("Lv.%d",lvInt+1);
		auto nonextLvLabel = Label::createWithTTF(nextLvTxt,"res/Fonts/FounderSemiBold.ttf",60,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!nonextLvLabel);
		nonextLvLabel->setColor(GET_RGBCOLOR(txt_color));
		nonextLvLabel->setAnchorPoint(Vec2(0,0.5));
		nonextLvLabel->setPosition(POSTIONS_TXT[3]);
		bg->addChild(nonextLvLabel,BASIC_ZORDER_ROOT+2);
		if (UserDefault::getInstance()->getBoolForKey(SET_CONTINU_KEY,true))//自动续局开关
		{
			//再来一局
			auto aganBtn = WWTouchSprite::createWithFile(btnLong,this,callfuncO_selector(GameOverLayer::onTouchSpritePree));
			CC_BREAK_IF(!aganBtn);
			aganBtn->setPosition(POSTIONS_IMG[11]);
			aganBtn->setTag(GAME_OVER_ANGIN);
			this->addChild(aganBtn,BASIC_ZORDER_FRAME);
			//文字
			auto aganLabel = Label::createWithTTF(GetWWString(g_over_agan,g_over_section),"res/Fonts/FounderSemiBold.ttf",45,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			CC_BREAK_IF(!aganLabel);
			aganLabel->setColor(GET_RGBCOLOR(contniu_txt_color));
			aganLabel->setPosition(Vec2(-aganLabel->getContentSize().width*0.3f,0));
			aganBtn->addChild(aganLabel,BASIC_ZORDER_ROOT);
			//倒计时
			m_pTime = Label::createWithTTF(StringUtils::format(GetWWString(g_over_times,g_over_section).c_str(),m_times),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			m_pTime->setColor(GET_RGBCOLOR(contniu_txt_color));
			m_pTime->setPosition(Vec2(aganBtn->getContentSize().width*0.25f,0));
			aganBtn->addChild(m_pTime,BASIC_ZORDER_ROOT);
		}
		else
		{
			//再来一局
			auto aganBtn = WWTouchSprite::createWithFile(btnYellow,this,callfuncO_selector(GameOverLayer::onTouchSpritePree));
			CC_BREAK_IF(!aganBtn);
			aganBtn->setPosition(POSTIONS_IMG[11]);
			aganBtn->setTag(GAME_OVER_ANGIN);
			this->addChild(aganBtn,BASIC_ZORDER_FRAME);
			//文字
			auto aganLabel = Label::createWithTTF(GetWWString(g_over_agan,g_over_section),"res/Fonts/FounderSemiBold.ttf",45,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			CC_BREAK_IF(!aganLabel);
			aganLabel->setColor(GET_RGBCOLOR(contniu_txt_color));
			aganLabel->setPosition(Vec2(0,0));
			aganBtn->addChild(aganLabel,BASIC_ZORDER_ROOT);
		}
		

		//本局消耗
		std::string xiaohaoTxt = StringUtils::format(GetWWString(g_over_xiaohao,g_over_section).c_str(),gameOverData->getBeanTax());
		auto xiaohaoLabel = Label::createWithTTF(xiaohaoTxt,"res/Fonts/FounderSemiBold.ttf",24,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!xiaohaoLabel);
		xiaohaoLabel->setColor(GET_RGBCOLOR(xiao_txt_color));
		xiaohaoLabel->setAnchorPoint(Vec2(0,0.5));
		xiaohaoLabel->setPosition(POSTIONS_TXT[9]);
		this->addChild(xiaohaoLabel,BASIC_ZORDER_FRAME);
		istringstream is(gameOverData->getFortunes()[selfIndex]);
		is >> m_beanChange;
	//int bean = gameOverData->getFortunes()[0];
	}while(0);
	this->setVisible(false);
	this->runAction(Sequence::create(DelayTime::create(3.5f), CallFunc::create(CC_CALLBACK_0(GameOverLayer::onDelayActionEnd, this)), nullptr));
}

void GameOverLayer::onDelayActionEnd()
{
	
	if (m_beanChange > 0)
	{
		PLAY_EFFECT(win_key);
	}
	else
	{
		PLAY_EFFECT(failed_key);
	}
	this->setVisible(true);
	
	//启动结算倒计时
	GameLogicManager::getInstance()->getpUIgameScene()->startGameOverTime();
}

bool GameOverLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void GameOverLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void GameOverLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{

}

void GameOverLayer::close(Ref * node)
{
	GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setShowGameOverFrame(true);
	GameLogicManager::getInstance()->GameOverCloseCallBack();
}

void GameOverLayer::updataTime(int dt)
{
	if (dt == 0)
	{
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setIsGameOverTimeUp(true);
		GameLogicManager::getInstance()->GameOverCloseCallBack();
	}
	else
	{
		std::string timeStr = StringUtils::format(GetWWString(g_over_times,g_over_section).c_str(),dt);
		m_pTime->setString(timeStr);
	}

}

void GameOverLayer::onTouchSpritePree(cocos2d::Ref* theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int pTag = btn->getTag();
	switch(pTag)
	{
	case GAME_OVER_ANGIN:
		GameLogicManager::getInstance()->GameOverContinueCallBack();
		break;
	default:
		break;
	}
}

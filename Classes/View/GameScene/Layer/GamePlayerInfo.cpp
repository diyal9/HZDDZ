#include "GamePlayerInfo.h"

#include "WWMacros.h"
#include "../GameLogicManager.h"
#include "BaseCore/WWBTools/StringTool.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "BaseCore/Common/UICoordConfig.h"

USING_NS_CC;

const char* game_user_info = "res/Images/GameScene/PlayerInfo/gs_userinfo.plist";

GamePlayerInfo::GamePlayerInfo(void)
{
}

GamePlayerInfo::~GamePlayerInfo()
{
}


void GamePlayerInfo::onEnter()
{
	LayerColor::onEnter();
	//禁止触摸穿透
	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(GamePlayerInfo::onTouchBegan, this);
	m_pListner->onTouchEnded = CC_CALLBACK_2(GamePlayerInfo::onTouchEnded, this);

	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);
}

void GamePlayerInfo::onExit()
{
	LayerColor::onExit();
	//释放资源
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(game_user_info);

	_eventDispatcher->removeEventListenersForTarget(this);
}

GamePlayerInfo* GamePlayerInfo::create(int site)
{
	GamePlayerInfo* pItem = new GamePlayerInfo();
	if(pItem && pItem->init(site))
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


bool GamePlayerInfo::init(int site)
{
	if (!LayerColor::init()) return false;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(game_user_info);

	createView(site);

	return true;
}

void GamePlayerInfo::createView(int site)
{
	do{
		NpcPlayer* player;
		if (site == 1)//下家
			player = GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getnpcRight();
		if (site == 2)//上家
			player = GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getnpcLeft();
		//背景
		m_pBg = Sprite::createWithSpriteFrameName("gs_info_bg.png");
		this->setContentSize(m_pBg->getContentSize());
		CC_BREAK_IF(!m_pBg);
		if (site == 1)//下家锚点设置
		{
			m_pBg->setAnchorPoint(Vec2(1.0f,1.0f));
			m_pBg->setPosition(CUR_SCREEN_SIZE.width * 0.5f - m_pBg->getContentSize().width * 0.5f,CUR_SCREEN_SIZE.height * 0.5f - m_pBg->getContentSize().height);
		}
		else
		{
			m_pBg->setAnchorPoint(Vec2(0,1.0f));
			m_pBg->setPosition(CUR_SCREEN_SIZE.width * 0.5f - m_pBg->getContentSize().width * 0.5f,CUR_SCREEN_SIZE.height * 0.5f - m_pBg->getContentSize().height);
		}		
		this->addChild(m_pBg,BASIC_ZORDER_ROOT);
		//头像区域
		auto left = Sprite::createWithSpriteFrameName("gs_info_head_party.png");
		CC_BREAK_IF(!left);
		if (site == 1)//下家头像区域和基本资料区域需要翻转
		{
			left->setFlippedX(true);
			left->setPosition(Vec2(m_pBg->getContentSize().width - left->getContentSize().width*0.5, left->getContentSize().height*0.5));
		}
		else
		{
			left->setFlippedX(false);
			left->setPosition(Vec2(left->getContentSize().width*0.5, left->getContentSize().height*0.5));
		}		
		m_pBg->addChild(left);
		//头像背景
		auto headBg = Sprite::create(infoHeadImageBg);
		CC_BREAK_IF(!headBg);
		headBg->setPosition(POSITION_LAST[1]);
		left->addChild(headBg,BASIC_ZORDER_ROOT);
		//背景
		auto headImag = Sprite::create(infoHeadTempImage);
		CC_BREAK_IF(!headImag);
		headImag->setPosition(POSITION_LAST[1]);
		headImag->setScale(1.3f,1.3f);
		left->addChild(headImag,BASIC_ZORDER_ROOT+1);
		//VIP
		auto vipLog = Sprite::create(infoVip);
		CC_BREAK_IF(!vipLog);
		vipLog->setPosition(POSITION_LAST[2]);
		left->addChild(vipLog,BASIC_ZORDER_ROOT-1);
		//金币icon
		auto jinbiIcon = Sprite::createWithSpriteFrameName("gs_info_bean_icon.png");
		CC_BREAK_IF(!jinbiIcon);
		jinbiIcon->setPosition(POSITION_LAST[4]);
		left->addChild(jinbiIcon,BASIC_ZORDER_ROOT);
		//金币数量
		std::string jinbiValue = "1000";
		if (site != 0)
			jinbiValue = StringTool::intToString(player->getCash());
		else
			jinbiValue = StringTool::intToString(GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getCash());
		auto jinbiNum = Label::createWithTTF(jinbiValue.c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!jinbiNum);
		jinbiNum->setAnchorPoint(Vec2(0,0.5f));
		jinbiNum->setColor(GET_RGBCOLOR(0xffffff));
		jinbiNum->setPosition(Vec2(jinbiIcon->getPositionX()+jinbiIcon->getContentSize().width,jinbiIcon->getPositionY()));
		left->addChild(jinbiNum,BASIC_ZORDER_ROOT);
		//基本资料区域
		auto right = Sprite::createWithSpriteFrameName("gs_info_com.png");
		CC_BREAK_IF(!right);
		if (site == 1)
		{
			right->setFlippedX(true);
			right->setPosition(Vec2(right->getContentSize().width*0.5f, right->getContentSize().height*0.5));
			left->setPosition(Vec2(right->getContentSize().width + left->getContentSize().width*0.5f,left->getContentSize().height*0.5f));
		}
		else
		{
			right->setFlippedX(false);
			right->setPosition(Vec2(right->getContentSize().width*0.5 + left->getContentSize().width, right->getContentSize().height*0.5));
		}
		
		m_pBg->addChild(right);
		int offer = 52;
		if (site == 0)
			offer = 70;
		//基本信息
		//昵称
		auto nickTalel = Label::createWithTTF(GetWWString(g_Game_info_nick,g_GameSence_Toast).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!nickTalel);
		nickTalel->setColor(GET_RGBCOLOR(0xbf8c63));
		nickTalel->setAnchorPoint(Vec2(0,0));
		nickTalel->setPosition(POSITION_LAST[3]);
		if (site == 1)//下家位置
			nickTalel->setPosition(POSITION_NEXT[3]);
		if (site == 0)//自己家
			nickTalel->setPosition(POSITION_SELF[3]);
		right->addChild(nickTalel,BASIC_ZORDER_ROOT);
		std::string nickName = "667847";
		if(site != 0)
			nickName = player->getNickName();
		else
			nickName = GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getNickName();
		auto nickLabel = Label::createWithTTF(nickName.c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!nickLabel);
		nickLabel->setColor(GET_RGBCOLOR(0x87402a));
		nickLabel->setAnchorPoint(Vec2(0,0));
		nickLabel->setPosition(Vec2(nickTalel->getContentSize().width + nickTalel->getPositionX(),nickTalel->getPositionY()));
		right->addChild(nickLabel,BASIC_ZORDER_ROOT);
		auto line1 = Sprite::createWithSpriteFrameName("gs_info_line.png");
		CC_BREAK_IF(!line1);
		line1->setAnchorPoint(Vec2(0,0));
		line1->setPosition(POSITION_LAST[5]);
		if (site == 1) //下家
			line1->setPosition(POSITION_NEXT[5]);
		if (site == 0) //自己家
			line1->setPosition(POSITION_SELF[5]);
		right->addChild(line1,BASIC_ZORDER_ROOT);
		//id
		auto idTable = Label::createWithTTF(GetWWString(g_Game_info_id,g_GameSence_Toast).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!idTable);
		idTable->setColor(GET_RGBCOLOR(0xbf8c63));
		idTable->setAnchorPoint(Vec2(0,0));
		idTable->setPosition(Vec2(nickTalel->getPositionX(),nickTalel->getPositionY()-offer));
		right->addChild(idTable,BASIC_ZORDER_ROOT);
		std::string idTxt = "8987899";
		if(site !=0 )
			idTxt = StringTool::intToString(player->getUserid());
		else
			idTxt = StringTool::intToString(GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getUserid());
		auto idLabel = Label::createWithTTF(idTxt.c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!idLabel);
		idLabel->setColor(GET_RGBCOLOR(0x87402a));
		idLabel->setAnchorPoint(Vec2(0,0));
		idLabel->setPosition(Vec2(idTable->getPositionX() + idTable->getContentSize().width,nickLabel->getPositionY()-offer));
		right->addChild(idLabel,BASIC_ZORDER_ROOT);
		auto line2 = Sprite::createWithSpriteFrameName("gs_info_line.png");
		CC_BREAK_IF(!line2);
		line2->setAnchorPoint(Vec2(0,0));
		line2->setPosition(Vec2(line1->getPositionX(),line1->getPositionY()-offer));
		right->addChild(line2,BASIC_ZORDER_ROOT);
		//称号
		auto chenghTable = Label::createWithTTF(GetWWString(g_Game_info_chenghao,g_GameSence_Toast).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!chenghTable);
		chenghTable->setColor(GET_RGBCOLOR(0xbf8c63));
		chenghTable->setAnchorPoint(Vec2(0,0));
		chenghTable->setPosition(Vec2(idTable->getPositionX(),idTable->getPositionY()-offer));
		right->addChild(chenghTable,BASIC_ZORDER_ROOT);
		/*std::string chenghTxt = "ALL WIN";
		if (site == 0)
			chenghTxt = player->getPlayerLevelName();
		else
			chenghTxt = GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getPlayerLevelName();*/
		auto chenghLabel = Label::createWithTTF("ALL WIN","res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!chenghLabel);
		chenghLabel->setColor(GET_RGBCOLOR(0x87402a));
		chenghLabel->setAnchorPoint(Vec2(0,0));
		chenghLabel->setPosition(Vec2(chenghTable->getContentSize().width + chenghTable->getPositionX(),chenghTable->getPositionY()));
		right->addChild(chenghLabel,BASIC_ZORDER_ROOT);
		/*chenghLabel->setString(chenghTxt.c_str());*/
		auto line3 = Sprite::createWithSpriteFrameName("gs_info_line.png");
		CC_BREAK_IF(!line3);
		line3->setAnchorPoint(Vec2(0,0));
		line3->setPosition(Vec2(line2->getPositionX(),line2->getPositionY()-offer));
		right->addChild(line3,BASIC_ZORDER_ROOT);

		//地区
		auto addressTable = Label::createWithTTF(GetWWString(g_Game_info_address,g_GameSence_Toast).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!addressTable);
		addressTable->setColor(GET_RGBCOLOR(0xbf8c63));
		addressTable->setAnchorPoint(Vec2(0,0));
		addressTable->setPosition(Vec2(chenghTable->getPositionX(),chenghTable->getPositionY()-offer));
		right->addChild(addressTable,BASIC_ZORDER_ROOT);
		auto addressLabel = Label::createWithTTF("shenzhen","res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!addressLabel);
		addressLabel->setColor(GET_RGBCOLOR(0x87402a));
		addressLabel->setAnchorPoint(Vec2(0,0));
		addressLabel->setPosition(Vec2(addressTable->getContentSize().width + addressTable->getPositionX(),addressTable->getPositionY()));
		right->addChild(addressLabel,BASIC_ZORDER_ROOT);
		auto line4 = Sprite::createWithSpriteFrameName("gs_info_line.png");
		CC_BREAK_IF(!line4);
		line4->setAnchorPoint(Vec2(0,0));
		line4->setPosition(Vec2(line3->getPositionX(),line3->getPositionY()-offer));
		right->addChild(line4,BASIC_ZORDER_ROOT);

		//胜率背景
		auto shengBg = Sprite::createWithSpriteFrameName("gs_info_win_bg.png");
		CC_BREAK_IF(!shengBg);
		shengBg->setPosition(POSITION_LAST[6]);
		if(site == 1)//下家
			shengBg->setPosition(POSITION_NEXT[6]);
		if(site == 0) //自己家
			shengBg->setPosition(POSITION_SELF[6]);
		right->addChild(shengBg,BASIC_ZORDER_ROOT);
		//胜率文字
		auto shengTxt = Label::createWithTTF(StringUtils::format(GetWWString(g_Game_info_shenglv,g_GameSence_Toast).c_str(), 53),"res/Fonts/FounderSemiBold.ttf",30,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!shengTxt);
		shengTxt->setColor(GET_RGBCOLOR(0xfff6dc));
		shengTxt->setAnchorPoint(Vec2(0,0.5f));
		shengTxt->setPosition(POSITION_LAST[7]);
		if(site == 1)//下家
			shengTxt->setPosition(POSITION_NEXT[7]);
		if(site == 0)//自己家
			shengTxt->setPosition(POSITION_SELF[7]);
		right->addChild(shengTxt,BASIC_ZORDER_ROOT+1);
		//总局数文字
		auto totleNum = Label::createWithTTF(StringUtils::format(GetWWString(g_Game_info_totle,g_GameSence_Toast).c_str(), 583),"res/Fonts/FounderSemiBold.ttf",30,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		CC_BREAK_IF(!totleNum);
		totleNum->setColor(GET_RGBCOLOR(0x87402a));
		totleNum->setAnchorPoint(Vec2(0,0.5f));
		totleNum->setPosition(POSITION_LAST[8]);
		if(site == 1)//下家
			totleNum->setPosition(POSITION_NEXT[8]);
		if(site == 0)//自己家
			totleNum->setPosition(POSITION_SELF[8]);
		right->addChild(totleNum,BASIC_ZORDER_ROOT+1);
		if (site != 0)//自己家信息中没有道具
		{
			//道具
			//花
			auto flower = WWTouchSprite::createWithSpriteFrameName("gs_info_hua.png",this,callfuncO_selector(GamePlayerInfo::onTouchSpriteDepress));
			CC_BREAK_IF(!flower);
			flower->setPosition(POSITION_LAST[9]);
			if(site == 1)//下家
				flower->setPosition(POSITION_NEXT[9]);
			flower->setTag(PROPS_FLOWER_TAG);
			right->addChild(flower,BASIC_ZORDER_ROOT);
			auto flowerBg = Sprite::createWithSpriteFrameName("gs_info_djbg.png");
			flowerBg->setPosition(flower->getPosition());
			right->addChild(flowerBg,BASIC_ZORDER_ROOT - 1);
			//道具数量  后面需要做判断 处理为0的时候 不显示
			auto flowerCountBg = Sprite::createWithSpriteFrameName("gs_info_djcount.png");
			flowerCountBg->setPosition(flowerBg->getPosition() + Vec2(flowerBg->getContentSize().width * 0.4f,-flower->getContentSize().height*0.3f));
			right->addChild(flowerCountBg,BASIC_ZORDER_FRAME);
			auto flowerCountLabel = Label::createWithTTF("12","res/Fonts/FounderSemiBold.ttf",18,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			flowerCountLabel->setPosition(Vec2(flowerCountBg->getContentSize().width*0.5f,flowerCountBg->getContentSize().height*0.5f));
			flowerCountBg->addChild(flowerCountLabel,BASIC_ZORDER_ROOT);
			//啤酒
			auto pijiu = WWTouchSprite::createWithSpriteFrameName("gs_info_bear.png",this,callfuncO_selector(GamePlayerInfo::onTouchSpriteDepress));
			CC_BREAK_IF(!pijiu);
			pijiu->setPosition(Vec2(flower->getPositionX()+108,flower->getPositionY()));
			pijiu->setTag(PROPS_PIJIU_TAG);
			right->addChild(pijiu,BASIC_ZORDER_ROOT);
			auto pijiuBg = Sprite::createWithSpriteFrameName("gs_info_djbg.png");
			pijiuBg->setPosition(pijiu->getPosition());
			right->addChild(pijiuBg,BASIC_ZORDER_ROOT - 1);
			//道具数量  后面需要做判断 处理为0的时候 不显示
			auto pijiuCountBg = Sprite::createWithSpriteFrameName("gs_info_djcount.png");
			pijiuCountBg->setPosition(pijiuBg->getPosition() + Vec2(pijiuBg->getContentSize().width * 0.4f,-pijiu->getContentSize().height*0.3f));
			right->addChild(pijiuCountBg,BASIC_ZORDER_FRAME);
			auto pijiuCountLabel = Label::createWithTTF("34","res/Fonts/FounderSemiBold.ttf",18,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			pijiuCountLabel->setPosition(Vec2(pijiuCountBg->getContentSize().width*0.5f,pijiuCountBg->getContentSize().height*0.5f));
			pijiuCountBg->addChild(pijiuCountLabel,BASIC_ZORDER_ROOT);
			//鸡蛋
			auto jidan = WWTouchSprite::createWithSpriteFrameName("gs_info_egg.png",this,callfuncO_selector(GamePlayerInfo::onTouchSpriteDepress));
			CC_BREAK_IF(!jidan);
			jidan->setPosition(Vec2(pijiu->getPositionX()+108,pijiu->getPositionY()));
			jidan->setTag(PROPS_JIDAN_TAG);
			right->addChild(jidan,BASIC_ZORDER_ROOT);
			auto jidanBg = Sprite::createWithSpriteFrameName("gs_info_djbg.png");
			jidanBg->setPosition(jidan->getPosition());
			right->addChild(jidanBg,BASIC_ZORDER_ROOT - 1);
			//道具数量  后面需要做判断 处理为0的时候 不显示
			auto jidanCountBg = Sprite::createWithSpriteFrameName("gs_info_djcount.png");
			jidanCountBg->setPosition(jidanBg->getPosition() + Vec2(jidanBg->getContentSize().width * 0.4f,-jidan->getContentSize().height*0.3f));
			right->addChild(jidanCountBg,BASIC_ZORDER_FRAME);
			auto jidanCountLabel = Label::createWithTTF("567","res/Fonts/FounderSemiBold.ttf",18,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			jidanCountLabel->setPosition(Vec2(jidanCountBg->getContentSize().width*0.5f,jidanCountBg->getContentSize().height*0.5f));
			jidanCountBg->addChild(jidanCountLabel,BASIC_ZORDER_ROOT);
			//板砖
			auto banzuan = WWTouchSprite::createWithSpriteFrameName("gs_info_bz.png",this,callfuncO_selector(GamePlayerInfo::onTouchSpriteDepress));
			CC_BREAK_IF(!banzuan);
			banzuan->setPosition(Vec2(jidan->getPositionX()+108,jidan->getPositionY()));
			banzuan->setTag(PROPS_BANZUAN_TAG);
			right->addChild(banzuan,BASIC_ZORDER_ROOT);
			auto banzuanBg = Sprite::createWithSpriteFrameName("gs_info_djbg.png");
			banzuanBg->setPosition(banzuan->getPosition());
			right->addChild(banzuanBg,BASIC_ZORDER_ROOT - 1);
			//道具数量  后面需要做判断 处理为0的时候 不显示
			auto banzuanCountBg = Sprite::createWithSpriteFrameName("gs_info_djcount.png");
			banzuanCountBg->setPosition(banzuanBg->getPosition() + Vec2(banzuanBg->getContentSize().width * 0.4f,-banzuan->getContentSize().height*0.3f));
			right->addChild(banzuanCountBg,BASIC_ZORDER_FRAME);
			auto banzuanCountLabel = Label::createWithTTF("6","res/Fonts/FounderSemiBold.ttf",18,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			banzuanCountLabel->setPosition(Vec2(banzuanCountBg->getContentSize().width*0.5f,banzuanCountBg->getContentSize().height*0.5f));
			banzuanCountBg->addChild(banzuanCountLabel,BASIC_ZORDER_ROOT);
			auto xiaohao = Label::createWithTTF(StringUtils::format(GetWWString(g_Gmae_info_daoju,g_GameSence_Toast).c_str(), 100),"res/Fonts/FounderSemiBold.ttf",24,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			xiaohao->setAnchorPoint(Vec2(0,0.5f));
			xiaohao->setPosition(Vec2(flower->getPositionX() - 15,flower->getPositionY() - flower->getContentSize().height*0.5f - xiaohao->getContentSize().height*0.5f - 10));
			xiaohao->setColor(GET_RGBCOLOR(0xbf8c63));
			right->addChild(xiaohao, BASIC_ZORDER_ROOT);
		}
		
		//特效
		m_pBg->setScale(0.2f);
		EaseBackOut* _easeIn = EaseBackOut::create(ScaleTo::create(0.2f,1.0f));
		Action* _action = Spawn::create(_easeIn,nullptr);
		m_pBg->runAction(_action);
	}while(0);
}


bool GamePlayerInfo::onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
	return true;
}


void GamePlayerInfo::onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
	Point startPoint =  m_pBg->convertToNodeSpace(pTouch->getStartLocation());
	Point locationInNode = m_pBg->convertToNodeSpace(pTouch->getLocation());
	Size s = m_pBg->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	// 点击范围判断检测
	if ((!rect.containsPoint(locationInNode)) && (!rect.containsPoint(startPoint)))
	{
		log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
		this->removeFromParent();
	}
}

void GamePlayerInfo::onTouchSpriteDepress(Ref* pBtn)
{

}
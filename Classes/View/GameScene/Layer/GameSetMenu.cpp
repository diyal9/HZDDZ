#include "GameSetMenu.h"
#include "View/GameScene/GameLogicManager.h"
USING_NS_CC;

const char* game_top_menu = "res/Images/GameScene/GameTopMenu/gs_topmenu.plist";
GameSetMenu::GameSetMenu(void) : 
	isOpenMenu(false)
{
}

GameSetMenu::~GameSetMenu()
{
	//释放资源
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(game_top_menu);
}

bool GameSetMenu::init()
{
	bool ret = false;

	do{
		CC_BREAK_IF(!LayerColor::initWithColor(Color4B(0,0,0,0)));
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(game_top_menu);
		//禁止触摸穿透
		auto m_pListner  = EventListenerTouchOneByOne::create();
		m_pListner->onTouchBegan = CC_CALLBACK_2(GameSetMenu::onTouchBegan, this);
		m_pListner->onTouchEnded = CC_CALLBACK_2(GameSetMenu::onTouchEnded, this);
		m_pListner->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);

		//顶部设置
		// 设置
		m_pSetBtn = WWTouchSprite::createWithSpriteFrameName("gs_menu_btn_down.png", this, callfuncO_selector(GameSetMenu::subBtnCallBack));
		m_pSetBtn->setPosition(Vec2(1250.0f, 1020.0f));
		this->addChild(m_pSetBtn, BASIC_ZORDER_TOP, SUB_BTN_TAG_MAIN);

		m_pBg = Sprite::createWithSpriteFrameName("gs_open_menu_bg.png");
		m_pBg->setAnchorPoint(Vec2(0.5, 1));
		m_pBg->setPosition(Vec2(m_pSetBtn->getPositionX(), m_pSetBtn->getPositionY() - m_pSetBtn->getContentSize().height * 0.5));
		this->addChild(m_pBg, BASIC_ZORDER_TOP, 1);
		

		//托管
		auto m_pTrusteeBtn = WWTouchSprite::createWithSpriteFrameName("gs_menu_trustee.png", this, callfuncO_selector(GameSetMenu::subBtnCallBack));
		m_pTrusteeBtn->setPosition(Vec2(m_pBg->getContentSize().width * 0.5, m_pBg->getContentSize().height * 0.8));
		m_pTrusteeBtn->setEnabled(false);
		m_pTrusteeBtn->setVisible(false);
		m_pBg->addChild(m_pTrusteeBtn, BASIC_ZORDER_ROOT, SUB_BTN_TAG_TRUSTEE);

		//设置
		auto m_pSetBtn = WWTouchSprite::createWithSpriteFrameName("gs_menu_set.png", this, callfuncO_selector(GameSetMenu::subBtnCallBack));
		m_pSetBtn->setPosition(Vec2(m_pBg->getContentSize().width * 0.5, m_pBg->getContentSize().height * 0.5));
		m_pSetBtn->setVisible(false);
		m_pBg->addChild(m_pSetBtn, BASIC_ZORDER_ROOT, SUB_BTN_TAG_SET);

		//退出
		auto m_pBackBtn = WWTouchSprite::createWithSpriteFrameName("gs_menu_back.png", this, callfuncO_selector(GameSetMenu::subBtnCallBack));
		m_pBackBtn->setPosition(Vec2(m_pBg->getContentSize().width * 0.5, m_pBg->getContentSize().height * 0.2));
		m_pBackBtn->setVisible(false);
		m_pBg->addChild(m_pBackBtn, BASIC_ZORDER_ROOT, SUB_BTN_TAG_BACK);
		
		isOpenMenu = false; //初始设置隐藏
		btnStateCall();
		ret = true;

	}while(0);

	return ret;
}

bool GameSetMenu::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	//如果是收缩状态，则直接穿透,否则拦截
	log("GameSetMenu::onTouchBegan");
	if(isOpenMenu){
		//btnStateCall();
		return false;
	}
	return true;
}


void GameSetMenu::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	log("GameSetMenu::onTouchEnded");
	/*if(isOpenMenu){
		btnStateCall();
	}*/
	Point startPoint =  m_pBg->convertToNodeSpace(pTouch->getStartLocation());
	Point locationInNode = m_pBg->convertToNodeSpace(pTouch->getLocation());
	Size s = m_pBg->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
	// 点击范围判断检测
	if ((!rect.containsPoint(locationInNode)) && (!rect.containsPoint(startPoint)) && !isOpenMenu)
	{
		btnStateCall();
	}
}

void GameSetMenu::btnStateCall()
{	
	Action* _action;
	if (!isOpenMenu)
	{
		m_pSetBtn->setRotation(0);
		_action = Sequence::create(FadeOut::create(0.1f),nullptr);
	}
	else
	{
		m_pSetBtn->setRotation(180);
		_action = Sequence::create(FadeIn::create(0.1f),nullptr);
	}
	
	m_pBg->runAction(_action);

	m_pBg->getChildByTag(SUB_BTN_TAG_TRUSTEE)->setVisible(isOpenMenu);
	m_pBg->getChildByTag(SUB_BTN_TAG_SET)->setVisible(isOpenMenu);
	m_pBg->getChildByTag(SUB_BTN_TAG_BACK)->setVisible(isOpenMenu);	
	isOpenMenu = !isOpenMenu; //状态反置
}



void GameSetMenu::subBtnCallBack(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	switch (tagId)
	{
	case SUB_BTN_TAG_MAIN : 
		{
			btnStateCall();
		}
		break;
	case SUB_BTN_TAG_TRUSTEE :    //托管
		{
			GameLogicManager::getInstance()->TopMenuTrusteeCallback();
			btnStateCall();
		}
		break;
	case SUB_BTN_TAG_SET :    //设置
		{
			GameLogicManager::getInstance()->TopMenuSetCallback();
			btnStateCall();
		}
		break;
	case SUB_BTN_TAG_BACK :    //返回
		{
			GameLogicManager::getInstance()->TopMenuReturnCallback();
			btnStateCall();
		}
		break;
	}
}

void GameSetMenu::setSubstituteEnable(bool isEnable)
{
	isEnable = isEnable ? true : false;
	auto substituteBtn = (WWTouchSprite*)m_pBg->getChildByTag(SUB_BTN_TAG_TRUSTEE);
	substituteBtn->setEnabled(isEnable);
}

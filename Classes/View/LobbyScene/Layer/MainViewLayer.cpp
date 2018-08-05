#include "MainViewLayer.h"
#include "../LobbyScene.h"
#include "BaseCore/Common/VisibleRect.h"
#include "BaseCore/WWBTools/StringTool.h"
#include "Control/WWCSound/WWSoundManager.h"
#include "Control/WWRequest/MsgChargeRequest.h"

USING_NS_CC;

using namespace std;

/*一般情况下、按照1024屏幕标准。偏移量平面积为25一下，就响应屏幕点击事件，否则为滑动）*/
const float SCROLLFACTOR = 30.0 / 1024.0; //判断滑动还是点击按钮，的比例系数 

const double SCROLL_FACTOR_CENTER = 0.8; //判断滑动还是点击按钮，的比例系数 
const double SCROLL_FACTOR_BOTTOM = 0.65; //判断滑动还是点击按钮，的比例系数 
const char* lobby_scene_plist = "res/Images/Lobby/lb_scene.plist";

MainViewLayer::MainViewLayer():
	scrollPos(0),
	m_pScroller(nullptr),
	m_pTopBar(nullptr),
	m_pBottomBar(nullptr),
	m_pBottomLayer(nullptr),
	m_pCenterLayer(nullptr),
	m_pScrollLayer(nullptr),
	cloudVector(NULL),
	scrollWidth(0)
{
}

MainViewLayer::~MainViewLayer(void)
{
	CC_SAFE_RELEASE_NULL(m_pScroller);
	//CC_SAFE_RELEASE_NULL(m_pScrollLayer);
	//CC_SAFE_RELEASE_NULL(m_pCenterLayer);
	//CC_SAFE_RELEASE_NULL(m_pBottomLayer);
	//CC_SAFE_RELEASE_NULL(m_pTouchLayer);
	//
	//CC_SAFE_RELEASE_NULL(m_pTopBar);
	//CC_SAFE_RELEASE_NULL(m_pBottomBar);

}

void MainViewLayer::onEnter()
{

	Layer::onEnter();

	auto touchListener = EventListenerTouchOneByOne::create();  
	touchListener->setSwallowTouches(true); 
	touchListener->onTouchBegan = CC_CALLBACK_2(MainViewLayer::onTouchBegan, this);  
	touchListener->onTouchMoved = CC_CALLBACK_2(MainViewLayer::onTouchMoved, this);  
	touchListener->onTouchEnded = CC_CALLBACK_2(MainViewLayer::onTouchEnded, this);  
	touchListener->onTouchCancelled = CC_CALLBACK_2(MainViewLayer::onTouchCancelled, this);  

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, m_pTouchLayer);  

	log("%s %d\r\n", __FUNCTION__, __LINE__);

	log("%s \r\n", (Director::getInstance()->getTextureCache()->getCachedTextureInfo()).c_str());
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(lobby_scene_plist);
}

void MainViewLayer::onExit()
{
	
	_eventDispatcher->removeEventListenersForTarget(this);
	Layer::onExit();

	unscheduleUpdate();

	//材质释放
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(lobby_scene_plist);

	log("%s %d\r\n", __FUNCTION__, __LINE__);
}

bool MainViewLayer::init()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	bool bRet = false;
	do
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(lobby_scene_plist);
		//视差层 前景
		m_pScrollLayer = LayerColor::create(Color4B(255,255,255,0));
		m_pScrollLayer->setAnchorPoint(VisibleRect::anchLeftBottom());
		m_pScrollLayer->setPosition(VisibleRect::leftBottom());
		this->addChild(m_pScrollLayer, BASIC_ZORDER_ROOT + 2);

		//视差层 中景
		m_pCenterLayer = LayerColor::create(Color4B(255,255,255,0));
		m_pCenterLayer->setAnchorPoint(VisibleRect::anchLeftBottom());
		m_pCenterLayer->setPosition(VisibleRect::leftBottom());
		this->addChild(m_pCenterLayer, BASIC_ZORDER_ROOT + 1);

		//视差层 后景
		m_pBottomLayer = LayerColor::create(Color4B(255,255,255,0));
		m_pBottomLayer->setAnchorPoint(VisibleRect::anchLeftBottom());
		m_pBottomLayer->setPosition(VisibleRect::leftBottom());
		this->addChild(m_pBottomLayer, BASIC_ZORDER_ROOT);

		//触摸层
		m_pTouchLayer = Layer::create();
		m_pTouchLayer->setAnchorPoint(VisibleRect::anchLeftBottom());
		m_pTouchLayer->setPosition(Size(0, 0));
		this->addChild(m_pTouchLayer, BASIC_ZORDER_FRAME + 1);

		createForwardLayer(); //前景
		createCentreLayer(); //中景
		createBehindLayer(); //后景
		setScrollInit(); //滑动器设置
		createFuncMenuFrame(); //创建功能按钮

		scheduleUpdate();

		bRet = true;

	}while(0);

	return bRet;
}

void MainViewLayer::setScrollInit()
{
	if (!m_pScroller)
	{
		m_pScroller = new Scroller();

		/*
		* 滑动范围算法
		* //前提条件: 背景图锚点是中心点，贴在Layer中心点上（Layer锚点为原点），目的尽量将背景的中间内容展示。实际滑动的是背景图的父节点 this
		* //所以该Layer实际的滑动Range min =  -(背景图大小 - 当前窗口大小) / 2 ~ 0   max =   (背景图大小 - 当前窗口大小) / 2
		*/
		float rangeWidth = scrollWidth - CUR_SCREEN_SIZE.width;
		m_pScroller->setRange(-rangeWidth, 0);
		m_pScroller->scrollTo(0, true);
	}
	else
	{
		m_pScrollLayer->setPositionX(m_pScroller->getpPos());
		m_pCenterLayer->setPositionX(m_pScroller->getpPos() * SCROLL_FACTOR_CENTER);
		m_pBottomLayer->setPositionX(m_pScroller->getpPos() * SCROLL_FACTOR_BOTTOM);
	}
}

void MainViewLayer::createFuncMenuFrame()
{
	// 创建公告按纽 相对于屏幕左边对齐
	auto noticeBtn = WWTouchSprite::createWithSpriteFrameName("lb_left_notice_icon.png", this, callfuncO_selector(MainViewLayer::TouchSpriteBtnEvent));
	auto tempPos = VisibleRect::getNodeAlignPos(NodeAlignMent::LEFT,Size(51,168),LOBBY_UI_POS[3]);
	noticeBtn->setPosition(tempPos);
	noticeBtn->setTag(BTN_LOBBYLAYER_NOTICE_TAG);
	m_pTouchLayer->addChild(noticeBtn, BASIC_ZORDER_ROOT + 2);

	// topBar
	m_pTopBar = TopBarLayer::create();
	// 设置工具条相对于屏幕顶部对齐
	tempPos = VisibleRect::getNodeAlignPos(NodeAlignMent::TOP,m_pTopBar->getContentSize(),Vec2::ZERO);
	m_pTopBar->setPosition(tempPos);
	m_pTouchLayer->addChild(m_pTopBar,  BASIC_ZORDER_ROOT + 1);

	// bottomBar
	m_pBottomBar = BottomBarLayer::create();
	// 设置工具条相对于屏幕底部对齐
	tempPos = VisibleRect::getNodeAlignPos(NodeAlignMent::BOTTOM,m_pBottomBar->getContentSize(),Vec2::ZERO);
	m_pBottomBar->setPosition(tempPos);
	m_pTouchLayer->addChild(m_pBottomBar,  BASIC_ZORDER_ROOT + 1);

	//添加折叠按钮层
	auto w_funcBtnLayer = FuncFoldMenuLayer::create();
	w_funcBtnLayer->setAnchorPoint(Vec2(0.5, 1));
	w_funcBtnLayer->setPosition(Vec2(m_pTouchLayer->getContentSize().width - w_funcBtnLayer->getContentSize().width, m_pTouchLayer->getContentSize().height - w_funcBtnLayer->getContentSize().height));
	//w_funcBtnLayer->setSwallowsTouches(true);
	m_pTouchLayer->addChild(w_funcBtnLayer, BASIC_ZORDER_ROOT + 2);

}

void MainViewLayer::createRoomLabel()
{
	std::string nameLabel[] = {"h_Game_happy","h_Game_rascal","h_Game_match","h_Game_crazy"};
	for (int i = 0; i < 4; i++)
	{
		auto happyLabel =  Sprite::createWithSpriteFrameName("lb_room_name_bg.png");
		happyLabel->setPosition(LOBBY_UI_POS[28 + i]);
		m_pScrollLayer->addChild(happyLabel, BASIC_ZORDER_FRAME);

		Label* label= Label::createWithTTF(GetWWString(nameLabel[i], "HallScene"),"res/Fonts/FounderSemiBold.ttf",35,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		label->setColor(Color3B::WHITE);
		label->setAnchorPoint(VisibleRect::anchCenter());
		label->setPosition(Vec2(happyLabel->getContentSize().width * 0.5, happyLabel->getContentSize().height * 0.5));
		happyLabel->addChild(label, BASIC_ZORDER_TOP+1);
	}
}

//前景
void MainViewLayer::createForwardLayer()
{
	// lobby bg
	auto lobbyBgImg1 = Sprite::create("res/Images/Lobby/lobbyBg1.png");
	lobbyBgImg1->setAnchorPoint(VisibleRect::anchLeftBottom());
	lobbyBgImg1->setPosition(Vec2(VisibleRect::leftBottom().x, VisibleRect::leftBottom().y));
	lobbyBgImg1->setTag(BTN_LOBBYLAYER_TOPTITBG_TAG);
	m_pScrollLayer->addChild(lobbyBgImg1,BASIC_ZORDER_ROOT + 1);

	auto lobbyBgImg2 = Sprite::create("res/Images/Lobby/lobbyBg2.png");
	lobbyBgImg2->setAnchorPoint(VisibleRect::anchLeftBottom());
	lobbyBgImg2->setPosition(Vec2(lobbyBgImg1->getPositionX() + lobbyBgImg1->getContentSize().width, VisibleRect::leftBottom().y));
	lobbyBgImg2->setTag(BTN_LOBBYLAYER_TOPTITBG_TAG + 1);
	m_pScrollLayer->addChild(lobbyBgImg2,BASIC_ZORDER_ROOT + 1);

	//TODO 优化 懒加载
	auto lobbyBgImg3 = Sprite::create("res/Images/Lobby/lobbyBg3.png");
	lobbyBgImg3->setAnchorPoint(VisibleRect::anchLeftBottom());
	lobbyBgImg3->setPosition(Vec2(lobbyBgImg2->getPositionX() + lobbyBgImg2->getContentSize().width , VisibleRect::leftBottom().y));
	lobbyBgImg3->setTag(BTN_LOBBYLAYER_TOPTITBG_TAG + 2);
	m_pScrollLayer->addChild(lobbyBgImg3,BASIC_ZORDER_ROOT + 1);

	scrollWidth = lobbyBgImg1->getContentSize().width + lobbyBgImg2->getContentSize().width + lobbyBgImg3->getContentSize().width;

	//欢乐斗地主
	auto happyRoomBtn =  WWTouchSprite::createWithSpriteFrameName("lb_happy_room_icon.png", this, callfuncO_selector(MainViewLayer::TouchSpriteBtnEvent));
	happyRoomBtn->setPosition(LOBBY_UI_POS[0]);
	happyRoomBtn->setTag(BTN_LOBBYLAYER_HAPPY_TAG);
	m_pScrollLayer->addChild(happyRoomBtn, BASIC_ZORDER_FRAME);
	happyRoomBtn->getpBg()->getTexture()->setAntiAliasTexParameters();

	//比赛斗地主
	auto matchRoomBtn = WWTouchSprite::createWithSpriteFrameName("lb_match_room_icon.png", this, callfuncO_selector(MainViewLayer::TouchSpriteBtnEvent));
	matchRoomBtn->setPosition(LOBBY_UI_POS[1]);
	matchRoomBtn->setTag(BTN_LOBBYLAYER_MATCH_TAG);
	m_pScrollLayer->addChild(matchRoomBtn, BASIC_ZORDER_FRAME);
	matchRoomBtn->getpBg()->getTexture()->setAntiAliasTexParameters();

	//癞子斗地主
	auto rascalRoomBtn = WWTouchSprite::createWithSpriteFrameName("lb_rascal_room_icon.png", this, callfuncO_selector(MainViewLayer::TouchSpriteBtnEvent));
	rascalRoomBtn->setPosition(LOBBY_UI_POS[2]);
	rascalRoomBtn->setTag(BTN_LOBBYLAYER_RASCAL_TAG);
	m_pScrollLayer->addChild(rascalRoomBtn, BASIC_ZORDER_FRAME);
	rascalRoomBtn->getpBg()->getTexture()->setAntiAliasTexParameters();

	//疯狂斗地主
	auto crazyRoomBtn = WWTouchSprite::createWithSpriteFrameName("lb_crazy_room_icon.png", this, callfuncO_selector(MainViewLayer::TouchSpriteBtnEvent));
	crazyRoomBtn->setPosition(LOBBY_UI_POS[27]);
	crazyRoomBtn->setTag(BTN_LOBBYLAYER_CRAZY_TAG);
	m_pScrollLayer->addChild(crazyRoomBtn, BASIC_ZORDER_FRAME);
	crazyRoomBtn->getpBg()->getTexture()->setAntiAliasTexParameters();

	// 创建首次礼包按纽 相对于屏幕左边对齐
	auto firstBuyBtn = WWTouchSprite::createWithSpriteFrameName("lb_first_box_icon.png", this, callfuncO_selector(MainViewLayer::TouchSpriteBtnEvent));
	firstBuyBtn->setPosition(LOBBY_UI_POS[4]);
	firstBuyBtn->setTag(BTN_LOBBYLAYER_FIRST_BUY_TAG);
	m_pScrollLayer->addChild(firstBuyBtn,  BASIC_ZORDER_ROOT + 2);

	createRoomLabel();
}

//中景
void MainViewLayer::createCentreLayer()
{
	auto lobbyMontain = Sprite::create("res/Images/Lobby/lb_montain_bg.png");
	//lobbyMontain->setAnchorPoint(VisibleRect::anchLeftBottom());
	lobbyMontain->setPosition(Vec2(lobbyMontain->getContentSize().width * 0.5, DISTANCE_CONV(930 - 180)));
	m_pCenterLayer->addChild(lobbyMontain,BASIC_ZORDER_ROOT);

	auto lobbyMontain2 = Sprite::create("res/Images/Lobby/lb_montain_bg.png");
	//lobbyMontain2->setAnchorPoint(VisibleRect::anchLeftBottom());
	lobbyMontain2->setPosition(Vec2(lobbyMontain->getPositionX() + lobbyMontain2->getContentSize().width, DISTANCE_CONV(930 - 180)));
	m_pCenterLayer->addChild(lobbyMontain2,BASIC_ZORDER_ROOT);
}

//后景
void MainViewLayer::createBehindLayer()
{
	//蓝天
	auto scaleBlueSky = ui::Scale9Sprite::create("res/Images/Lobby/blueSky.png");
	scaleBlueSky->setPosition(Vec2(scrollWidth * 0.5, CUR_SCREEN_SIZE.height * 0.5));
	scaleBlueSky->setContentSize(Size(scrollWidth, CUR_SCREEN_SIZE.height));
	m_pBottomLayer->addChild(scaleBlueSky, BASIC_ZORDER_ROOT);

	//白云
	float cloudSize = 800;
	for (unsigned int i = 0; i < 5; i++)
	{
		Sprite* whriteCloud;
		Vec2 vec;
		if (i % 2 == 0)
		{
			whriteCloud	= Sprite::create("res/Images/Lobby/lb_cloud1.png");
			vec =  Vec2(scrollWidth - cloudSize * i, VisibleRect::leftTop().y * 0.9);
		}
		else
		{
			whriteCloud	= Sprite::create("res/Images/Lobby/lb_cloud2.png");
			vec =  Vec2(scrollWidth - cloudSize * i, VisibleRect::leftTop().y);
		}

		whriteCloud->setAnchorPoint(VisibleRect::anchRightTop());
		whriteCloud->setPosition(vec);
		m_pBottomLayer->addChild(whriteCloud, BASIC_ZORDER_TOP, i + 1);
		cloudVector.push_back(whriteCloud);
	}

}

void MainViewLayer::update(float delta)
{
	//在帧函数里调用需要
	CloudMovement(delta);

	//每帧都刷新主界面滑动层的位置
	if (m_pScroller)
	{
		float movex = delta * 0.7f;

		m_pScroller->update(movex);

		//log("move to new Pos : %f * %f * %f", 
			//m_pScroller->getpPos(), m_pScroller->getpPos() * SCROLL_FACTOR_CENTER, m_pScroller->getpPos() * SCROLL_FACTOR_BOTTOM);

		m_pScrollLayer->setPositionX(m_pScroller->getpPos());
		m_pCenterLayer->setPositionX(m_pScroller->getpPos() * SCROLL_FACTOR_CENTER);
		m_pBottomLayer->setPositionX(m_pScroller->getpPos() * SCROLL_FACTOR_BOTTOM);
	}
	else
	{

	}
}

void MainViewLayer::CloudMovement(float delta)
{
	float movex = delta / (1.0 / 40.0f);
	//云层运动
	for (unsigned int i = 0; i < cloudVector.size(); i++)
	{
		//如果图片超出Layer右边，则移动到右边
		auto cloudSprite = static_cast<Sprite*>(cloudVector[i]);

		float localPoint = cloudSprite->getPositionX() - cloudSprite->getContentSize().width;

		if (localPoint >= scrollWidth)
		{
			//如果超过屏幕，则移动到最左边
			cloudSprite->setPositionX(cloudSprite->getPositionX() - (cloudSprite->getContentSize().width) * cloudVector.size());

			//其它图片对齐矫正

		}
		else
		{
			cloudSprite->setPositionX(cloudSprite->getPositionX() + movex);
		}
	}
}

bool MainViewLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("MainViewLayer::onkTouchBegan");

	if (m_pScroller)
	{
		m_pScroller->TouchBegan(pTouch, pEvent);
		return true;
	}

	return true;
}

void MainViewLayer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("MainViewLayer::onTouchMoved");

	if(m_pScroller)
		return	m_pScroller->TouchMoved(pTouch, pEvent);
}

void MainViewLayer::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("MainViewLakkyer::onTouchEnded");

	float move_x = pTouch->getLocation().x - pTouch->getStartLocation().x;
	float move_y = pTouch->getLocation().y - pTouch->getStartLocation().y;

	if (move_x * move_x + move_y * move_y < CUR_SCREEN_SIZE.width * SCROLLFACTOR) //移动范围大则，说明是滑动
	{
		Vec2 touchPoint = pTouch->getLocation();	

		IsClickRoom(touchPoint); //滑动层按钮点击事件判断
	}

	return m_pScroller->TouchEnded(pTouch, pEvent);
}

void MainViewLayer::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("MainViewLayer::onTouchCancelled");
}

//房间按钮响应
void MainViewLayer::ClickRoomMusic()
{
	if (FileUtils::getInstance()->isFileExist("res/Audio/mp3/Audio_Button_Click.mp3"))
	{
		WWSoundManager::getInstance()->playEffect("res/Audio/mp3/Audio_Button_Click.mp3");
	}
}

//房间按钮响应
void MainViewLayer::IsClickRoom(Vec2 touchPoint)
{
	//选中欢乐斗地主
	//auto rootNode = m_pScrollLayer->getChildByTag(BTN_LOBBYLAYER_TOPTITBG_TAG);
	//auto tempSprite = rootNode->getChildByTag(BTN_LOBBYLAYER_HAPPY_TAG);
	
	auto tempSprite = m_pScrollLayer->getChildByTag(BTN_LOBBYLAYER_HAPPY_TAG);
	Vec2 bodVect = tempSprite->getParent()->convertToWorldSpace(tempSprite->getPosition());
	Rect rect = Rect(bodVect.x - tempSprite->getContentSize().width * 0.5, 
		bodVect.y - tempSprite->getContentSize().height * 0.5, 
		tempSprite->getContentSize().width, 
		tempSprite->getContentSize().height);

	if (rect.containsPoint(touchPoint))
	{
		ClickRoomMusic();
		TouchSpriteBtnEvent(tempSprite);
		log("had click m_pHappyRoomBtn");
	}

	//选中癞子斗地主
	tempSprite = m_pScrollLayer->getChildByTag(BTN_LOBBYLAYER_RASCAL_TAG);
	Vec2 bodVect2 = tempSprite->getParent()->convertToWorldSpace(tempSprite->getPosition());
	Rect rect2 = Rect(bodVect2.x - tempSprite->getContentSize().width * 0.5, 
		bodVect2.y - tempSprite->getContentSize().height * 0.5, 
		tempSprite->getContentSize().width, 
		tempSprite->getContentSize().height);

	if (rect2.containsPoint(touchPoint))
	{
		ClickRoomMusic();
		TouchSpriteBtnEvent(tempSprite);
		log("had click m_pRascalRoomBtn");
	}

	//选中比赛场
	tempSprite = m_pScrollLayer->getChildByTag(BTN_LOBBYLAYER_MATCH_TAG);
	Vec2 bodVect3 = tempSprite->getParent()->convertToWorldSpace(tempSprite->getPosition());
	Rect rect3 = Rect(bodVect3.x - tempSprite->getContentSize().width * 0.5,
		bodVect3.y - tempSprite->getContentSize().height * 0.5,
		tempSprite->getContentSize().width,
		tempSprite->getContentSize().height);

	if (rect3.containsPoint(touchPoint))
	{
		ClickRoomMusic();
		TouchSpriteBtnEvent(tempSprite);
		log("had click m_pMatchRoomBtn");
	}

	//选中疯狂场
	tempSprite = m_pScrollLayer->getChildByTag(BTN_LOBBYLAYER_CRAZY_TAG);
	Vec2 bodVect4 = tempSprite->getParent()->convertToWorldSpace(tempSprite->getPosition());
	Rect rect4 = Rect(bodVect4.x - tempSprite->getContentSize().width * 0.5,
		bodVect3.y - tempSprite->getContentSize().height * 0.5,
		tempSprite->getContentSize().width,
		tempSprite->getContentSize().height);

	if (rect4.containsPoint(touchPoint))
	{
		ClickRoomMusic();
		TouchSpriteBtnEvent(tempSprite);
		log("had click m_pCrazyRoomBtn");
	}

}

void MainViewLayer::TouchSpriteBtnEvent(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	switch (tagId)
	{
	case BTN_LOBBYLAYER_NOTICE_TAG:{ //公告
			log("notice button press!");
			//WWToast::makeText(GetWWString("room_zhunru","Other"));
		}
		break;
	case BTN_LOBBYLAYER_HAPPY_TAG: //欢乐斗地主
		{
			log("happy room button press!");
			if (this->getChildByTag(BTN_LOBBYLAYER_HAPPY_TAG + 1000))
			{
				this->removeChildByTag(BTN_LOBBYLAYER_HAPPY_TAG + 1000);
			}
			auto l_lRoomLayer = RoomListLayer::create(RoomType::HAPPY_ROOM);
			l_lRoomLayer->setPosition(Vec2(0, 0));
			this->addChild(l_lRoomLayer, BASIC_ZORDER_TOP, BTN_LOBBYLAYER_HAPPY_TAG + 1000);
			log("had click m_pHappyRoomBtn");
		}
		break;
		
	case BTN_LOBBYLAYER_RASCAL_TAG: // 赖子斗地主
		{
			log("rascal room button press!");
			auto l_lRoomLayer = RoomListLayer::create(RoomType::RASCAL_ROOM);
			l_lRoomLayer->setPosition(Vec2(0, 0));
			this->addChild(l_lRoomLayer,BASIC_ZORDER_TOP);

			//l_lRoomLayer->setpPlayerTypeID(4);
		}
		break;
	case  BTN_LOBBYLAYER_MATCH_TAG: // 比赛场
		{
			log("match room button press!");
			auto l_lRoomLayer = RoomListLayer::create(RoomType::MATCH_ROOM);
			l_lRoomLayer->setPosition(Vec2(0, 0));
			this->addChild(l_lRoomLayer,BASIC_ZORDER_TOP);
		}
	case  BTN_LOBBYLAYER_CRAZY_TAG: // 疯狂场
		{
			log("crazy room button press!");
			//WWToast::makeText(GetWWString("room_zhunru","Other"));

		}
		break;
	case BTN_LOBBYLAYER_FIRST_BUY_TAG:{ //首充
			log("first buy button press!");
			//MsgLobbyRequest::requestRoomDatas( 1001, 2);
			MsgChargeRequest::requestMoneyInfo();
		}
		break;
	default:
		break;
	}
}
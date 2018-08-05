#include "BottomBarLayer.h"
#include "../LobbyScene.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"
#include "View/Common/ChargeLayer/ChargeLayer.h"
#include "../../Common/FriendLayer/FriendLayer.h"
#include "View/Common/PropShop/PropShopLayer.h"

USING_NS_CC;

using namespace std;

const char* lobby_bottom_plist = "res/Images/Lobby/bottom/lb_bottom.plist";

BottomBarLayer::BottomBarLayer()
{
	log("BottomBarLayer begin to begin");
	/*setKeypadEnabled(true);
	setTouchEnabled(true);*/



	log("BottomBarLayer begin to end");

}

BottomBarLayer::~BottomBarLayer(void)
{
}

bool BottomBarLayer::init()
{
	log("TopBarLayer::init");

	bool bRet = false;
	do
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(lobby_bottom_plist);
		auto bottomBarBg = Sprite::createWithSpriteFrameName("lb_bottom_bg.png");
		bottomBarBg->setPosition(bottomBarBg->getContentSize().width*0.5, bottomBarBg->getContentSize().height);

		//根据背景设置层大小
		this->setContentSize(bottomBarBg->getContentSize());

		this->addChild(bottomBarBg,1);

		//邮件按钮
		auto mailBtn = WWTouchSprite::createWithSpriteFrameName("lb_bot_mail_icon.png",this,callfuncO_selector(BottomBarLayer::onTouchSpriteDepress));
		mailBtn->setTag(BTN_LOBBYLAYER_EMAIL_TAG);
		mailBtn->setPosition(LOBBY_UI_POS[5]);
		bottomBarBg->addChild(mailBtn,1);
		//每日签到
		auto teskBtn = WWTouchSprite::createWithSpriteFrameName("lb_bot_sig_icon.png",this,callfuncO_selector(BottomBarLayer::onTouchSpriteDepress));
		teskBtn->setTag(BTN_LOBBYLAYER_TESK_TAG);
		teskBtn->setPosition(LOBBY_UI_POS[6]);
		bottomBarBg->addChild(teskBtn,2);
		//购买金币
		auto buyBtn = WWTouchSprite::createWithSpriteFrameName("lb_bot_charge_icon.png",this,callfuncO_selector(BottomBarLayer::onTouchSpriteDepress));
		buyBtn->setTag(BTN_LOBBYLAYER_BUY_TAG);
		buyBtn->setPosition(LOBBY_UI_POS[7]);
		bottomBarBg->addChild(buyBtn,2);
		//商城
		auto shopBtn = WWTouchSprite::createWithSpriteFrameName("lb_bot_shop_icon.png",this,callfuncO_selector(BottomBarLayer::onTouchSpriteDepress));
		shopBtn->setTag(BTN_LOBBYLAYER_SHOP_TAG);
		shopBtn->setPosition(LOBBY_UI_POS[8]);
		bottomBarBg->addChild(shopBtn,2);
		//活动
		auto activityBtn = WWTouchSprite::createWithSpriteFrameName("lb_bot_alive_icon.png",this,callfuncO_selector(BottomBarLayer::onTouchSpriteDepress));
		activityBtn->setTag(BTN_LOBBYLAYER_ACTIVITY_TAG);
		activityBtn->setPosition(LOBBY_UI_POS[9]);
		bottomBarBg->addChild(activityBtn,3);
		//快速开始
		auto quickStartBtn =  WWTouchSprite::createWithSpriteFrameName("lb_quick_start_btn.png",this,callfuncO_selector(BottomBarLayer::onTouchSpriteDepress));
		quickStartBtn->setTag(BTN_LOBBYLAYER_QUICK_START_TAG);
		quickStartBtn->setPosition(LOBBY_UI_POS[10]);
		bottomBarBg->addChild(quickStartBtn,3);
	
		auto birdImg = Sprite::createWithSpriteFrameName("lb_bottom_bird.png");
		birdImg->setPosition(LOBBY_UI_POS[11]);
		bottomBarBg->addChild(birdImg,4);

		bRet = true;

	}while(0);

	return bRet;
}

void BottomBarLayer::onEnter()
{
	Layer::onEnter();

	auto touchListener = EventListenerTouchOneByOne::create();  
	touchListener->setSwallowTouches(true);  

	touchListener->onTouchBegan = CC_CALLBACK_2(BottomBarLayer::onTouchBegan, this);  
	touchListener->onTouchMoved = CC_CALLBACK_2(BottomBarLayer::onTouchMoved, this);  
	touchListener->onTouchEnded = CC_CALLBACK_2(BottomBarLayer::onTouchEnded, this);  
	touchListener->onTouchCancelled = CC_CALLBACK_2(BottomBarLayer::onTouchCancelled, this);  

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);  
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(lobby_bottom_plist);
}

void BottomBarLayer::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	Layer::onExit();
	//释放材质
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(lobby_bottom_plist);
}

bool BottomBarLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("BottomBarLayer::onTouchBegan");
	auto pWorldPoint = pTouch->getLocation();
	auto rect = this->getBoundingBox();

	return rect.containsPoint(pWorldPoint);
}

void BottomBarLayer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("BottomBarLayer::onTouchMoved");
}

void BottomBarLayer::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("BottomBarLayer::onTouchEnded");
}

void BottomBarLayer::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("BottomBarLayer::onTouchCancelled");
}

////////////////////////////////////////////////////////////////////
/////////////  页面按纽响应  ////////////////////
void BottomBarLayer::onTouchSpriteDepress(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	switch (tagId)
	{
	case BTN_LOBBYLAYER_EMAIL_TAG:{ // 邮件系统
			log("bottom bar email button press!");
			//WWToast::makeText(GetWWString("room_zhunru","Other"));
			auto charge = FriendLayer::create();
			charge->setAnchorPoint(Vec2(0.5,0.5));
			charge->setPosition(CUR_SCREEN_SIZE.width/2 - charge->getContentSize().width/2,CUR_SCREEN_SIZE.height/2-charge->getContentSize().height/2);
			this->getParent()->addChild(charge, 100);
		}
		break;
	case BTN_LOBBYLAYER_TESK_TAG:{ // 每日任务
			log("bottom bar tesk button press!");
			//WWToast::makeText(GetWWString("room_zhunru","Other"));
		}
		break;
	case BTN_LOBBYLAYER_BUY_TAG:{ // 充值
			log("bottom bar buy button press!");
			auto charge = ChargeLayer::create();
			charge->setAnchorPoint(Vec2(0.5,0.5));
			charge->setPosition(CUR_SCREEN_SIZE.width/2 - charge->getContentSize().width/2,CUR_SCREEN_SIZE.height/2-charge->getContentSize().height/2);
			this->getParent()->addChild(charge, 100);
		}
		break;
	case BTN_LOBBYLAYER_SHOP_TAG:{ // 商城
			log("bottom bar shop button press!");
			auto propShop = PropShopLayer::create();
			propShop->setAnchorPoint(Vec2(0.5,0.5));
			propShop->setPosition(CUR_SCREEN_SIZE.width * 0.5 - propShop->getContentSize().width/2,CUR_SCREEN_SIZE.height/2 - propShop->getContentSize().height * 0.5);
			Director::getInstance()->getRunningScene()->addChild(propShop, BASIC_ZORDER_FRAME, FRAMEPOP_TAG_PROPSHOP);
		}
		break;
	case BTN_LOBBYLAYER_ACTIVITY_TAG:{ // 活动中心
			log("bottom bar activity button press!");
			//WWToast::makeText(GetWWString("room_zhunru","Other"));
		}
		break;
	case BTN_LOBBYLAYER_QUICK_START_TAG:{ //快速开始
			log("bottom bar quick start button press!");
			//WWToast::makeText(GetWWString("room_zhunru","Other"));
		}
		break;
	default:
		break;
	}
}
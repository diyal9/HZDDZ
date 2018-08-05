#include "TopBarLayer.h"
#include "../LobbyScene.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "Control/WWDataManager/WWDataManager.h"
#include "BaseCore/WWBGUI/WWToast.h"
#include "../../Common/UserInfoLayer/UserInfoLayer.h"

USING_NS_CC;

using namespace std;
const char* top_bar_plist = "res/Images/Lobby/TopBar/lb_top.plist";

TopBarLayer::TopBarLayer()
{


}

TopBarLayer::~TopBarLayer(void)
{
}

void TopBarLayer::onEnter()
{
	Layer::onEnter();

	/*setKeypadEnabled(true);
	setTouchEnabled(true);*/	

	auto touchListener = EventListenerTouchOneByOne::create();  
	touchListener->setSwallowTouches(true); 

	touchListener->onTouchBegan = CC_CALLBACK_2(TopBarLayer::onTouchBegan, this);  
	touchListener->onTouchMoved = CC_CALLBACK_2(TopBarLayer::onTouchMoved, this);  
	touchListener->onTouchEnded = CC_CALLBACK_2(TopBarLayer::onTouchEnded, this);  
	touchListener->onTouchCancelled = CC_CALLBACK_2(TopBarLayer::onTouchCancelled, this);  

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);  
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(top_bar_plist);
}

void TopBarLayer::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	Layer::onExit();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(top_bar_plist);
}


bool TopBarLayer::init()
{
	log("TopBarLayer::init");

	bool bRet = false;
	do
	{
		//加载材质
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(top_bar_plist);
		std::string userName = WWDataManager::getInstance()->getUserBasicInfoData()->getNickName();
		std::string userCash = WWDataManager::getInstance()->getUserBasicInfoData()->getGameCash();
		std::string userCash2 = WWDataManager::getInstance()->getUserBasicInfoData()->getGameCash2();

		auto topBarBg = Sprite::createWithSpriteFrameName("lb_top_bg.png");
		topBarBg->setPosition(topBarBg->getContentSize().width*0.5f,0);

		//根据背景设置层大小
		this->setContentSize(topBarBg->getContentSize());

		this->addChild(topBarBg,BASIC_ZORDER_ROOT);
	
		//Vip 标识
		auto vipTag = Sprite::createWithSpriteFrameName("lb_top_head_vip.png");
		vipTag->setPosition(LOBBY_UI_POS[12]);
		topBarBg->addChild(vipTag,BASIC_ZORDER_ROOT + 1);
	
		//头像背景
		auto headFrameBg = Sprite::createWithSpriteFrameName("lb_top_head_bg.png");
		headFrameBg->setPosition(LOBBY_UI_POS[13]);
		topBarBg->addChild(headFrameBg,BASIC_ZORDER_ROOT + 2);
		//头像
		auto headImg = Sprite::create("res/Images/Common/head-temp.png");
		headImg->setPosition(LOBBY_UI_POS[14]);
		topBarBg->addChild(headImg,BASIC_ZORDER_ROOT + 3);
		//头像框
		auto headFrame = WWTouchSprite::createWithSpriteFrameName("lb_top_head_pand.png",this,callfuncO_selector(TopBarLayer::onTouchSpritePrs));
		headFrame->setTag(BTN_LOBBYLAYER_HEAD_TAG);
		headFrame->setPosition(LOBBY_UI_POS[13]);
		topBarBg->addChild(headFrame,BASIC_ZORDER_ROOT + 3);
		//昵称背景
		auto nameBg = Sprite::createWithSpriteFrameName("lb_top_name_bg.png");
		nameBg->setPosition(LOBBY_UI_POS[15]);
		topBarBg->addChild(nameBg,BASIC_ZORDER_ROOT + 2);
		//金币背景
		auto coinBg = Sprite::createWithSpriteFrameName("lb_top_coin_bg.png");
		coinBg->setPosition(LOBBY_UI_POS[16]);
		topBarBg->addChild(coinBg,BASIC_ZORDER_ROOT + 2);
		//金币 + 按钮
		auto m_pCoinAddBtn = WWTouchSprite::createWithSpriteFrameName("lb_top_add_btn.png",this,callfuncO_selector(TopBarLayer::onTouchSpritePrs));
		m_pCoinAddBtn->setTag(BTN_LOBBYLAYER_COINADD_TAG);
		m_pCoinAddBtn->setPosition(LOBBY_UI_POS[17]);
		topBarBg->addChild(m_pCoinAddBtn,BASIC_ZORDER_ROOT + 4);
		//钻石背景
		auto diamBg = Sprite::createWithSpriteFrameName("lb_top_diam_bg.png");
		diamBg->setPosition(LOBBY_UI_POS[18]);
		topBarBg->addChild(diamBg,BASIC_ZORDER_ROOT + 2);
		//钻石 + 按钮
		auto m_pDiamAddBtn = WWTouchSprite::createWithSpriteFrameName("lb_top_add_btn.png",this,callfuncO_selector(TopBarLayer::onTouchSpritePrs));
		m_pDiamAddBtn->setTag(BTN_LOBBYLAYER_DIAMADD_TAG);
		m_pDiamAddBtn->setPosition(LOBBY_UI_POS[19]);
		topBarBg->addChild(m_pDiamAddBtn,BASIC_ZORDER_ROOT + 4);
		//昵称内容
		auto nameTx = FactoryTools::setLabelShowRect(LOBBY_USER_NAME, Size(LOBBY_UI_POS[22].x,LOBBY_UI_POS[22].y), TextHAlignment::LEFT, GAME_FONT_NAME, 36, Rect(0,0,LOBBY_UI_POS[22].x,LOBBY_UI_POS[22].y), LOBBY_UI_POS[21]);
		topBarBg->addChild(nameTx,BASIC_ZORDER_ROOT + 3);
		nameTx->setString(userName);
		//金币数
		auto moneyTx = FactoryTools::setLabelShowRect(LOBBY_COIN_NUM, Size(LOBBY_UI_POS[24].x,LOBBY_UI_POS[24].y), TextHAlignment::RIGHT, GAME_FONT_NAME, 36, Rect(0,0,LOBBY_UI_POS[24].x,LOBBY_UI_POS[24].y), LOBBY_UI_POS[23]);
		topBarBg->addChild(moneyTx,BASIC_ZORDER_ROOT + 3);
		moneyTx->setString(userCash);//设置金币 后面要处理过长 换算单位
		//钻石数
		auto diamTx = FactoryTools::setLabelShowRect(LOBBY_DIAM_NUM, Size(LOBBY_UI_POS[26].x,LOBBY_UI_POS[26].y), TextHAlignment::RIGHT, GAME_FONT_NAME, 36, Rect(0,0,LOBBY_UI_POS[26].x,LOBBY_UI_POS[26].y), LOBBY_UI_POS[25]);
		topBarBg->addChild(diamTx,BASIC_ZORDER_ROOT + 3);
		if (userCash2.empty())
			userCash2 = "0";
		diamTx->setString(userCash2);//设置钻石 后面要处理过长 换算单位
		
		bRet = true;

	}while(0);

	return bRet;
}

bool TopBarLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("TopBarLayer::onTouchBegan");
	auto pWorldPoint = pTouch->getLocation();
	auto rect = this->getBoundingBox();

	return rect.containsPoint(pWorldPoint);
}

void TopBarLayer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("TopBarLayer::onTouchMoved");
}

void TopBarLayer::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("TopBarLayer::onTouchEnded");
}

void TopBarLayer::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("TopBarLayer::onTouchCancelled");
}

////////////////////////////////////////////////////////////////////
/////////////  页面按纽响应  ////////////////////
void TopBarLayer::onTouchSpritePrs(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	switch(tagId)
	{
	case BTN_LOBBYLAYER_HEAD_TAG:{ // 大厅玩家头像
			log("top bar head frame click!");
			auto userInfo = UserInfoLayer::create();
			userInfo->setPosition(Vec2(0,0));
			this->getParent()->addChild(userInfo,BASIC_ZORDER_FRAME);
		}
		break;
	case BTN_LOBBYLAYER_COINADD_TAG:{ // 顶部工具条金币“+”
			log("topBar coin add button press!");
			WWToast::makeText(GetWWString("room_zhunru","Other"));
		}
		break;
	case BTN_LOBBYLAYER_DIAMADD_TAG:{ // 顶部工具条钻石“+”
			log("topBar diam add button press!");
			WWToast::makeText(GetWWString("room_zhunru","Other"));

		}
		break;
	case BTN_LOBBYLAYER_TOPBAR_MENU_TAG:{ // 顶部下拉菜单
			log("topBar menu button press!");
		}
		break;
	
	default:
		break;
	}

}
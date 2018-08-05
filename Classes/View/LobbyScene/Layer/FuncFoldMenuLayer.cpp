#include "FuncFoldMenuLayer.h"

#include "Control//WWRequest/MsgLogonRequest.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "../LobbyScene.h"
#include "View/Common/SetLayer/SetLayer.h"

USING_NS_CC;

using namespace std;

//功能按钮图片路径
#define MENUAYYAY_SIZE 5 //图片个数
const std::string menuArray[] = { 
	"lb_func_menu_backbag.png",
	"lb_func_menu_achi.png",
	"lb_func_menu_daily.png",
	"lb_func_menu_rank.png",
	"lb_func_menu_set.png",
};
const char* menu_plist = "res/Images/Lobby/TopMenu/lb_topmenu.plist";

const double CLIPLAYER_FACTOR =  0.2; //状态按钮跟字按钮背景的重叠比

FuncFoldMenuLayer::FuncFoldMenuLayer(void) : 
	isOpenMenu(true),
	m_ptopMenuBtn(nullptr),
	s_menuBg(nullptr)
{
}

FuncFoldMenuLayer::~FuncFoldMenuLayer()
{
}


void FuncFoldMenuLayer::onEnter()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	LayerColor::onEnter();
	//禁止触摸穿透
	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(FuncFoldMenuLayer::onTouchBegan, this);
	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(menu_plist);
}

void FuncFoldMenuLayer::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	LayerColor::onExit();
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(menu_plist);
}

bool FuncFoldMenuLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0,0,0,0))) return false;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(menu_plist);

	//创建背景精灵
	s_menuBg = Sprite::createWithSpriteFrameName("lb_func_menu_bg.png");
	/* 创建状态按钮 */
	m_ptopMenuBtn = WWTouchSprite::createWithSpriteFrameName("lb_top_menu_open.png", this, callfuncO_selector(FuncFoldMenuLayer::TouchSpriteBtnEvent));

	//根据背景设置层大小
	this->setContentSize(Size(s_menuBg->getContentSize().width,
		s_menuBg->getContentSize().height + m_ptopMenuBtn->getContentSize().height - m_ptopMenuBtn->getContentSize().height * CLIPLAYER_FACTOR));

	//设置状态按钮属性
	m_ptopMenuBtn->setPosition(Vec2(this->getContentSize().width * 0.5, this->getContentSize().height - m_ptopMenuBtn->getContentSize().height * 0.5));
	m_ptopMenuBtn->setTag(BTN_LOBBYLAYER_TOPBAR_MENU_TAG);
	this->addChild(m_ptopMenuBtn,5);

	//遮罩精灵（向上箭头）
	auto s_topMenuBtn = Sprite::createWithSpriteFrameName("lb_top_menu_close.png");
	s_topMenuBtn->setPosition(Vec2(0, 0));
	s_topMenuBtn->setVisible(true); //初始设置向上箭头（在按钮上覆盖精灵）
	s_topMenuBtn->setName("BtnBarModeLayer");
	m_ptopMenuBtn->addChild(s_topMenuBtn,10);

	//设置子按钮背景
	s_menuBg->setAnchorPoint(Vec2(0.5, 1));
	s_menuBg->setPosition(Vec2(m_ptopMenuBtn->getPositionX(), m_ptopMenuBtn->getPositionY() - m_ptopMenuBtn->getContentSize().height * CLIPLAYER_FACTOR));
	this->addChild(s_menuBg, 2);

	//添加子按钮
	Size funcMenuSize = s_menuBg->getContentSize();
	int firstTag = BTN_LOBBYLAYER_FUNCTAG_BAG; //展开的第一个按钮Tag
	for (int i = 0; i < MENUAYYAY_SIZE; i++)
	{
		std::string res_path = menuArray[i];
		float positionPer = 1 - i * 0.2;
		float delSize = funcMenuSize.height * 0.01 * i; //调整间距
		auto s_menu = WWTouchSprite::createWithSpriteFrameName(res_path.c_str(), this, callfuncO_selector(FuncFoldMenuLayer::TouchSpriteBtnEvent));
		s_menu->setAnchorPoint(Size(0, 0.5));
		s_menu->setPosition(funcMenuSize.width * 0.5, funcMenuSize.height * 0.96 * positionPer + delSize);
		s_menuBg->addChild(s_menu, 2, firstTag++);
	}

	this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&](){
		touchFuncMenuBar();
	})));

	return true;
}

bool FuncFoldMenuLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	//如果是收缩状态，则直接穿透
	if (!isOpenMenu) return false;

	auto pWorldPoint = pTouch->getLocation();
	auto rect = this->getBoundingBox();

	return rect.containsPoint(pWorldPoint);
}
	
void FuncFoldMenuLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void FuncFoldMenuLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{

}

void FuncFoldMenuLayer::touchFuncMenuBar()
{
	isOpenMenu = !isOpenMenu; //状态反置
	int firstTag = BTN_LOBBYLAYER_FUNCTAG_BAG; //展开的第一个按钮Tag
	if (isOpenMenu)
	{
		//放开按钮
		s_menuBg->setVisible(true);

		Size funcMenuSize = s_menuBg->getContentSize();
		for (int i = 0; i < MENUAYYAY_SIZE; i++)
		{
			float positionPer = 1 - i * 0.2;
			float delSize = funcMenuSize.height * 0.01 * i; 
			Vec2 pos =Vec2(funcMenuSize.width * 0.5, funcMenuSize.height * 0.96 * positionPer + delSize);
			auto tmpChild = s_menuBg->getChildByTag(firstTag + i);
			controlFadeIn(tmpChild, 0.1f, (i + 1), pos);
		}
	}
	else
	{
		s_menuBg->setVisible(false);

		//收按钮
		for (int i = 0; i < MENUAYYAY_SIZE; i++)
		{
			auto tmpChild = s_menuBg->getChildByTag(i + firstTag);
			controlFadeOut(tmpChild, 0.1f);
		}
	}
	m_ptopMenuBtn->getChildByName("BtnBarModeLayer")->setVisible(isOpenMenu); //设置箭头按钮隐藏状态
}

void FuncFoldMenuLayer::controlFadeIn(Node* node, float time, int seq, Vec2 pos)
{
	auto moveTo = MoveTo::create(0.1f, pos);
	auto fadeIn = FadeIn::create(0.1f);
	node->setVisible(true);
	node->runAction(moveTo);
	node->runAction(fadeIn);
}

void FuncFoldMenuLayer::controlFadeOut(Node* node, float time)
{
	auto moveTo = MoveTo::create(time, m_ptopMenuBtn->getPosition());
	auto fadeOut = FadeIn::create(time);
	node->runAction(moveTo);
	node->runAction( Sequence::create(
		fadeOut,
		CallFunc::create(
		// lambda
		[&](){

	}  ),
		nullptr));
}

void FuncFoldMenuLayer::TouchSpriteBtnEvent(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	if (tagId == BTN_LOBBYLAYER_TOPBAR_MENU_TAG)
	{
		touchFuncMenuBar();
	}else if(tagId == BTN_LOBBYLAYER_FUNCTAG_BAG){
		log("BTN_LOBBYLAYER_FUNCTAG_BAG");
		//WWToast::makeText(GetWWString("room_zhunru","Other"));
	}else if(tagId == BTN_LOBBYLAYER_FUNCTAG_SET){
		log("BTN_LOBBYLAYER_FUNCTAG_SET");
		auto set = SetLayer::create(true);
		set->setAnchorPoint(Vec2(0.5,0.5));
		set->setPosition(CUR_SCREEN_SIZE.width/2 - set->getContentSize().width/2,CUR_SCREEN_SIZE.height/2-set->getContentSize().height/2);
		this->getParent()->addChild(set, 100);
	}
}


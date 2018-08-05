#include "ChargeLayer.h"
#include "ChargeLayerRes.h"
#include "../../../BaseCore/Common/UICoordConfig.h"
#include "../../../BaseCore/WWBGUI/WWTouchSprite.h"
#include "../../../WWMacros.h"
#include "../CommLayer/CommDialog.h"
#include "cocos/ui/UIRichText.h"

enum chargeTag
{
	Tag_JinBi = 1,
	Tag_ZuanShi
};

ChargeLayer::ChargeLayer():
	vipLv100(0.0f),
	isJinbi(false),
	m_pListView(nullptr)
{

}

ChargeLayer::~ChargeLayer()
{
	
}

bool ChargeLayer::init()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(chargePlist);
	if ( !WWFrameBase::init() )
	{
		return false;
	}

	createView();
	
	return true;
}

void ChargeLayer::createView()
{
	//添加背景
	auto bg = this->getpBg();

	//标题背景
	auto titleBg = Sprite::createWithSpriteFrameName(charge_title_bg);
	titleBg->setPosition(CHARGE_POSTIONS[0]);
	bg->addChild(titleBg,BASIC_ZORDER_ROOT);
	//标题字
	auto titleTxt = Sprite::createWithSpriteFrameName(charge_title_zuanshi);
	titleTxt->setPosition(CHARGE_POSTIONS[1]);
	bg->addChild(titleTxt, BASIC_ZORDER_ROOT+1);
	//特权区域
	auto descBg = Sprite::createWithSpriteFrameName(charge_des_bg);
	descBg->setPosition(CHARGE_POSTIONS[2]);
	bg->addChild(descBg, BASIC_ZORDER_ROOT);
	//判断金币充值和钻石充值
	if (isJinbi)
	{
		//金币充值 只提示使用钻石充值更优惠的字
		Label* jinbiDesc = Label::createWithTTF(GetWWString(c_charge_desc,c_Charge_section),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		jinbiDesc->setColor(GET_RGBCOLOR(SET_TXT_COLOR));
		jinbiDesc->setAnchorPoint(Vec2(0,0.5));
		jinbiDesc->setPosition(CHARGE_POSTIONS[15]);
		descBg->addChild(jinbiDesc,BASIC_ZORDER_ROOT);

	}
	else
	{
		///以下为钻石充值才有的内容  start
		//特权icon
		auto vipIcon = Sprite::createWithSpriteFrameName(charge_vviipp_icon);
		vipIcon->setPosition(CHARGE_POSTIONS[3]);
		descBg->addChild(vipIcon,BASIC_ZORDER_ROOT+1);
		//vipbg
		auto vipBg = Sprite::createWithSpriteFrameName(charge_vviipp_bg);
		vipBg->setPosition(CHARGE_POSTIONS[5]);
		descBg->addChild(vipBg,BASIC_ZORDER_ROOT);
		//vip lv
		auto vipLv = Sprite::createWithSpriteFrameName(charge_vip_log);
		vipLv->setPosition(CHARGE_POSTIONS[4]);
		descBg->addChild(vipLv,BASIC_ZORDER_ROOT+1);

		auto vipLvTxt = Label::createWithTTF("4","res/Fonts/FounderSemiBold.ttf",46,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		vipLvTxt->setColor(GET_RGBCOLOR(VIP_LV_COCLOR));
		vipLvTxt->setAnchorPoint(Vec2(0,0.5));
		vipLvTxt->setPosition(CHARGE_POSTIONS[13]);
		descBg->addChild(vipLvTxt,BASIC_ZORDER_ROOT+1);

		//Vip等级 Slider
		auto slider = Slider::create();
		slider->loadBarTexture(charge_vip_lv_nrom);
		slider->loadProgressBarTexture(charge_vip_lv_pro);
		slider->setPosition(CHARGE_POSTIONS[6]);
		slider->setPercent(90);
		slider->setScale9Enabled(true);
		descBg->addChild(slider,BASIC_ZORDER_ROOT+1);

	
		//再充值XX元即可成为VIP 4556777
		std::string vipUp = StringUtils::format(GetWWString(c_Charge_viplv,c_Charge_section).c_str(),20,5);
		Label* vipUpLabel = Label::createWithTTF(vipUp,"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		vipUpLabel->setColor(GET_RGBCOLOR(SET_TXT_COLOR));
		vipUpLabel->setAnchorPoint(Vec2(0,0.5));
		vipUpLabel->setPosition(CHARGE_POSTIONS[14]);
		descBg->addChild(vipUpLabel,BASIC_ZORDER_ROOT+1);
		//以上为钻石充值才有的内容  end
	}
	//ListView
	if (!m_pListView)
	{
		m_pListView = ListView::create();
		m_pListView->setDirection(ui::ScrollView::Direction::VERTICAL);
		m_pListView->setTouchEnabled(true);
		m_pListView->setBounceEnabled(true);//是否回弹
		m_pListView->setBackGroundImageScale9Enabled(true);
		m_pListView->setContentSize(Size(1428,644));
		m_pListView->setAnchorPoint(Vec2(0.5,0.5));
		m_pListView->setItemsMargin(28.0f);
		m_pListView->setPosition(CHARGE_POSTIONS[7]);
		m_pListView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(ChargeLayer::listItemClick,this));
		bg->addChild(m_pListView,BASIC_ZORDER_FRAME);
	}
	reflashListView();
}

void ChargeLayer::onEnter()
{
	LayerColor::onEnter();
	//禁止触摸穿透
	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(ChargeLayer::onTouchBegan, this);
	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);
	log("%s \r\n", (Director::getInstance()->getTextureCache()->getCachedTextureInfo()).c_str());

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(chargePlist);
}

void ChargeLayer::onExit()
{
	LayerColor::onExit();
	//释放plist资源
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(chargePlist);
}


Layout* ChargeLayer::createItem()
{
	Layout* itemLayout = Layout::create();//Item布局
	itemLayout->setTouchEnabled(true);
	itemLayout->setContentSize(Size(1428,204));
	//横向第一项S
	auto item1 = Sprite::createWithSpriteFrameName(charge_item_bg);
	item1->setPosition(Vec2(item1->getContentSize().width/2,itemLayout->getContentSize().height/2));
	//icon
	if (isJinbi)
	{
		auto icon1 = Sprite::createWithSpriteFrameName(charge_jinbi_1);
		icon1->setPosition(CHARGE_POSTIONS[9]);
		item1->addChild(icon1);
	}
	else
	{
		auto icon1 = Sprite::createWithSpriteFrameName(charge_zuanshi_1);
		icon1->setPosition(CHARGE_POSTIONS[9]);
		item1->addChild(icon1);
	}
	//首充图标
	auto fristC1 = Sprite::createWithSpriteFrameName(charge_double_icon);
	fristC1->setPosition(CHARGE_POSTIONS[8]);
	item1->addChild(fristC1);
	
	//充值按钮
	auto chargeBtn1 = WWTouchSprite::createWithSpriteFrameName(charge_item_btn,this,callfuncO_selector(ChargeLayer::itemCallBack));
	chargeBtn1->setPosition(CHARGE_POSTIONS[10]);
	item1->addChild(chargeBtn1);
	//菜单金额
	RichText* moneyRich = RichText::create();
	moneyRich->ignoreContentAdaptWithSize(false);
	auto mon = Label::createWithTTF("6","res/Fonts/FounderSemiBold.ttf",42,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	auto yu = Label::createWithTTF(GetWWString(c_charge_yuan,c_Charge_section),"res/Fonts/FounderSemiBold.ttf",37,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	moneyRich->setContentSize(Size(mon->getContentSize().width+yu->getContentSize().width,mon->getContentSize().height));

	RichElementText* money = RichElementText::create(1, GET_RGBCOLOR(SET_TXT_COLOR),255, "6","res/Fonts/FounderSemiBold.ttf",42);
	RichElementText* yuan = RichElementText::create(2, GET_RGBCOLOR(SET_TXT_COLOR),255, GetWWString(c_charge_yuan,c_Charge_section),"res/Fonts/FounderSemiBold.ttf",37);
	moneyRich->pushBackElement(money);
	moneyRich->pushBackElement(yuan);
	moneyRich->setPosition(Vec2(0,0));
	chargeBtn1->addChild(moneyRich,BASIC_ZORDER_ROOT);
	//游戏币和赠送的游戏币
	auto youxiBi = Label::createWithTTF("2000","res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	auto zuanshiBi = Label::createWithTTF("5000","res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	auto jinbi = Label::createWithTTF(GetWWString(c_Charge_jinbi,c_Charge_section),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	auto zuanshi = Label::createWithTTF(GetWWString(c_Charge_zuanshi,c_Charge_section),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	
	youxiBi->setAnchorPoint(Vec2(0,0.5));
	jinbi->setAnchorPoint(Vec2(0,0.5));
	zuanshiBi->setAnchorPoint(Vec2(0,0.5));
	zuanshi->setAnchorPoint(Vec2(0,0.5));
	youxiBi->setColor(GET_RGBCOLOR(JINBI_TXT_COLOR));
	zuanshiBi->setColor(GET_RGBCOLOR(ZUANSHI_TXT_COLOR));
	jinbi->setColor(GET_RGBCOLOR(JINBI_COLOR));
	zuanshi->setColor(GET_RGBCOLOR(JINBI_COLOR));

	if (isJinbi)
	{
		youxiBi->setPosition(CHARGE_POSTIONS[11]);
		jinbi->setPosition(CHARGE_POSTIONS[11] + Vec2(youxiBi->getContentSize().width,0));
		item1->addChild(youxiBi);
		item1->addChild(jinbi);
	}
	else
	{
		zuanshiBi->setPosition(CHARGE_POSTIONS[11]);
		zuanshi->setPosition(CHARGE_POSTIONS[11] + Vec2(zuanshiBi->getContentSize().width,0));
		item1->addChild(zuanshiBi);
		item1->addChild(zuanshi);
	}
	//赠送的游戏币
	std::string zengsTxt = StringUtils::format(GetWWString(c_Charge_zengs_jinbi,c_Charge_section).c_str(),20000);
	Label* zengong = Label::createWithTTF(zengsTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	zengong->setColor(GET_RGBCOLOR(ZENGSONG_TXT_COLOR));
	zengong->setAnchorPoint(Vec2(0,0.5));
	zengong->setPosition(CHARGE_POSTIONS[12]);
	item1->addChild(zengong);
	itemLayout->addChild(item1);


	//横向第二项
	auto item2 = Sprite::createWithSpriteFrameName(charge_item_bg);
	item2->setPosition(Vec2(item1->getPositionX() + item1->getContentSize().width + 10,itemLayout->getContentSize().height/2));
	//icon
	if (isJinbi)
	{
		auto icon2 = Sprite::createWithSpriteFrameName(charge_jinbi_4);
		icon2->setAnchorPoint(Vec2(0.5,0.5));
		icon2->setPosition(CHARGE_POSTIONS[9]);
		item2->addChild(icon2);
	}
	else
	{
		auto icon2 = Sprite::createWithSpriteFrameName(charge_zuanshi_4);
		icon2->setAnchorPoint(Vec2(0.5,0.5));
		icon2->setPosition(CHARGE_POSTIONS[9]);
		item2->addChild(icon2);
	}
	//首充图标
	auto fristC2 = Sprite::createWithSpriteFrameName(charge_double_icon);
	fristC2->setPosition(CHARGE_POSTIONS[8]);
	item2->addChild(fristC2);
	itemLayout->addChild(item2);
	//充值按钮
	auto chargeBtn2 = WWTouchSprite::createWithSpriteFrameName(charge_item_btn,this,callfuncO_selector(ChargeLayer::itemCallBack));
	chargeBtn2->setPosition(CHARGE_POSTIONS[10]);
	//菜单金额
	RichText* moneyRich2 = RichText::create();
	moneyRich2->ignoreContentAdaptWithSize(false);
	auto mon2 = Label::createWithTTF("20","res/Fonts/FounderSemiBold.ttf",42,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	auto yu2 = Label::createWithTTF(GetWWString(c_charge_yuan,c_Charge_section),"res/Fonts/FounderSemiBold.ttf",37,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	moneyRich2->setContentSize(Size(mon2->getContentSize().width+yu2->getContentSize().width,mon2->getContentSize().height));

	RichElementText* money2 = RichElementText::create(1, GET_RGBCOLOR(SET_TXT_COLOR),255, "20","res/Fonts/FounderSemiBold.ttf",42);
	RichElementText* yuan2 = RichElementText::create(2, GET_RGBCOLOR(SET_TXT_COLOR),255, GetWWString(c_charge_yuan,c_Charge_section),"res/Fonts/FounderSemiBold.ttf",37);
	moneyRich2->pushBackElement(money2);
	moneyRich2->pushBackElement(yuan2);
	moneyRich2->setPosition(Vec2(0,0));
	chargeBtn2->addChild(moneyRich2,BASIC_ZORDER_ROOT);

	//游戏币和赠送的游戏币
	RichText* youxiBiRich2 = RichText::create();
	youxiBiRich2->ignoreContentAdaptWithSize(false);
	auto youxiBi2 = Label::createWithTTF("2000","res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	auto zuanshiBi2 = Label::createWithTTF("50000","res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	auto jinbi2 = Label::createWithTTF(GetWWString(c_Charge_jinbi,c_Charge_section),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	auto zuanshi2 = Label::createWithTTF(GetWWString(c_Charge_zuanshi,c_Charge_section),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	
	youxiBi2->setAnchorPoint(Vec2(0,0.5));
	jinbi2->setAnchorPoint(Vec2(0,0.5));
	zuanshiBi2->setAnchorPoint(Vec2(0,0.5));
	zuanshi2->setAnchorPoint(Vec2(0,0.5));
	youxiBi2->setColor(GET_RGBCOLOR(JINBI_TXT_COLOR));
	zuanshiBi2->setColor(GET_RGBCOLOR(ZUANSHI_TXT_COLOR));
	jinbi2->setColor(GET_RGBCOLOR(JINBI_COLOR));
	zuanshi2->setColor(GET_RGBCOLOR(JINBI_COLOR));

	if (isJinbi)
	{
		youxiBi2->setPosition(CHARGE_POSTIONS[11]);
		jinbi2->setPosition(CHARGE_POSTIONS[11] + Vec2(youxiBi2->getContentSize().width,0));
		item2->addChild(youxiBi2);
		item2->addChild(jinbi2);
	}
	else
	{		
		zuanshiBi2->setPosition(CHARGE_POSTIONS[11]);
		zuanshi2->setPosition(CHARGE_POSTIONS[11] + Vec2(zuanshiBi2->getContentSize().width,0));
		item2->addChild(zuanshiBi2);
		item2->addChild(zuanshi2);
	}
	//赠送的游戏币
	std::string zengsTxt2 = StringUtils::format(GetWWString(c_Charge_zengs_jinbi,c_Charge_section).c_str(),20000);
	Label* zengong2 = Label::createWithTTF(zengsTxt2,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	zengong2->setColor(GET_RGBCOLOR(ZENGSONG_TXT_COLOR));
	zengong2->setAnchorPoint(Vec2(0,0.5));
	zengong2->setPosition(CHARGE_POSTIONS[12]);
	item2->addChild(zengong2);
	item2->addChild(chargeBtn2);
	return itemLayout;
}

void ChargeLayer::reflashListView()
{
	m_pListView->removeAllItems();

	for (int i = 0; i < 3; i++)
	{
		m_pListView->pushBackCustomItem(createItem());
	}
}

void ChargeLayer::listItemClick(Ref* pSender, ListView::EventType type)
{

}


bool ChargeLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	return true;
}

void ChargeLayer::onTouchMoved(Touch* pTouch, Event* pEvent)
{
}

void ChargeLayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
}

void ChargeLayer::onTouchCancelled(Touch* pTouch, Event* pEvent)
{
}



void ChargeLayer::itemCallBack(Ref * node)
{

}



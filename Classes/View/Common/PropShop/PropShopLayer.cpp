#include "PropShopLayer.h"
#include "PropShopLayerRes.h"
#include "WWMacros.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"
#include "Control/WWRequest/MsgCommonRequest.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "BaseCore/WWBGUI/WWToast.h"
#include "BaseCore/WWBGUI/WWRichText.h"
#include "BaseCore/Common/VisibleRect.h"
#include "View/Common/CommLayer/CommDialog.h"

PropShopLayer::PropShopLayer(void) : 
	m_pListView(nullptr),
	nItemIndex(0),
	itemListSize(0),
	isAllSaleOut(true)
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

}

PropShopLayer::~PropShopLayer()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);
}


void PropShopLayer::onEnter()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	LayerColor::onEnter();
	//禁止触摸穿透
	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(PropShopLayer::onTouchBegan, this);
	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(propShop_list_plist);
}

void PropShopLayer::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	LayerColor::onExit();
	log("%s %d\r\n", __FUNCTION__, __LINE__);


	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(propShop_list_plist);
}

bool PropShopLayer::init()
{
	if ( !WWFrameBase::init() )
	{
		return false;
	}

	this->setName("PropShopLayer");

	MsgCommonRequest::sendMagicStoreReq(1006);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(propShop_list_plist);

	bool resurlt = false;
	do 
	{
		createView();

		resurlt = true;
	} 
	while (0);

	return resurlt;
}

void PropShopLayer::createView()
{
	//获取背景
	auto bg = this->getpBg();

	//标题
	auto titleBg = Sprite::createWithSpriteFrameName(prop_title);
	titleBg->setPosition(Vec2(bg->getContentSize().width * 0.5, bg->getContentSize().height - titleBg->getContentSize().height * 0.6));
	bg->addChild(titleBg,BASIC_ZORDER_ROOT);

	//书页背景
	auto s_lFrameSubBg = Sprite::create(frameBg2);
	s_lFrameSubBg->setPosition(Vec2(bg->getContentSize().width * 0.5, bg->getContentSize().height * 0.48));
	bg->addChild(s_lFrameSubBg);

	// 创建ListView
	if (!m_pListView)
	{
		m_pListView = ListView::create();
		// 设置方向
		m_pListView->setDirection(ui::ScrollView::Direction::VERTICAL);
		m_pListView->setTouchEnabled(true);
		m_pListView->setBounceEnabled(true); //是否开启回弹
		m_pListView->setBackGroundImage(frameBg3);
		m_pListView->setBackGroundImageScale9Enabled(true); //是否按.9方式拉升
		m_pListView->setContentSize(listViewBgSize); 
		m_pListView->setAnchorPoint(Vec2(0.5, 0.5));
		m_pListView->setPosition(Vec2(s_lFrameSubBg->getContentSize().width * 0.5, s_lFrameSubBg->getContentSize().height * 0.55));
		m_pListView->setItemsMargin(17.0f);
		//m_pListView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(PropShopLayer::listItemClick, this));
		s_lFrameSubBg->addChild(m_pListView);
	}

	//reflashListView();
}

Layout* PropShopLayer::createCell()
{
	Layout* default_item = Layout::create();
	default_item->setTouchEnabled(true);
	default_item->setContentSize(Size(cellItemSize.width * 2 + 17 * 2, cellItemSize.height));

	/* 每次创建的是一个Cell，一个Cell包含两个元素，总个数为基数的时候，右边不创建，每个Item的Tag是位移识别的标识,设置为物品ID */
	for (unsigned int i = 0; i < 2; i++)
	{
		if (nItemIndex >= itemListSize)
		{
			break;
		}

		if (nItemIndex >= itemListSize)
		{
			break;
		}

		auto itemNode = createCellItem(nItemIndex);
		if (i == 0) //左边
		{
			itemNode->setPosition(Vec2(default_item->getContentSize().width * 0.5 - cellItemSize.width * 0.5, default_item->getContentSize().height * 0.4f));
		}
		else
		{
			itemNode->setPosition(Vec2(default_item->getContentSize().width * 0.5 + cellItemSize.width * 0.5, default_item->getContentSize().height * 0.4f));
		}
		default_item->addChild(itemNode, BASIC_ZORDER_TOP, nItemIndex); //设置Tag位该条数据所在的索引，它的相关信息可以根据这个ID去容器中获取

		++nItemIndex;
	}

	return default_item;
}

ImageView* PropShopLayer::createCellItem(int index)
{
	//背景
	auto cellBg = ImageView::create(prop_cellbg);
	cellBg->setContentSize(cellItemSize);
	cellBg->setScale9Enabled(true); //打开ImageView的九宫模式
	cellBg->setTouchEnabled(true);
	cellBg->setPosition(Vec2(cellBg->getContentSize().width * 0.5, cellBg->getContentSize().height * 0.5));

	//物品图片背景
	auto itemBg = Sprite::createWithSpriteFrameName(prop_itembg);
	itemBg->setPosition(Vec2(94, cellBg->getContentSize().height * 0.53));
	cellBg->addChild(itemBg, BASIC_ZORDER_FRAME);

	//图片 TODO 这里应该封装一个对象，根据URL去获得图片资源
	auto item = Sprite::create(prop_itemtmp);
	item->setPosition(Vec2(itemBg->getContentSize().width * 0.5, itemBg->getContentSize().height * 0.5));
	item->setScale(1.8);
	itemBg->addChild(item);

	//名字背景 (跟单个Cell的背景宽度一致)
	auto nameBg = Scale9Sprite::createWithSpriteFrameName(prop_namebg);
	nameBg->setContentSize(Size(650, 70));
	nameBg->setPosition(Vec2(cellBg->getContentSize().width * 0.5, 125));
	cellBg->addChild(nameBg, BASIC_ZORDER_ROOT);

	//房间名称
	std::string sName = shopListVector.at(index).Name;
	auto labelName = Label::createWithTTF(sName, "res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	labelName->setColor(GET_RGBCOLOR(propshop_title_txt_color));
	labelName->setAnchorPoint(Vec2(0,0.5f));
	labelName->setPosition(Vec2(nameBg->getContentSize().width * 0.3, nameBg->getContentSize().height * 0.5));
	nameBg->addChild(labelName,BASIC_ZORDER_TOP + 1);

	//按钮
	auto itemBtn = WWTouchSprite::createWithFile(prop_itembtn, this, callfuncO_selector(PropShopLayer::TouchSpriteBtnEvent));
	itemBtn->setPosition(Vec2(558, 56));
	cellBg->addChild(itemBtn);

	std::string sCost = StringUtils::format("%d元", shopListVector.at(index).Money);
	auto l_cost = Text::create();
	l_cost->setContentSize(Size(333, 34));
	l_cost->setFontSize(30);
	l_cost->setFontName("res/Fonts/FounderSemiBold.ttf");
	l_cost->setColor(GET_RGBCOLOR(propshop_title_txt_color));
	l_cost->setString(sCost);
	itemBtn->addChild(l_cost);
	l_cost->setPosition(Vec2(0, 0));

	//灰态
	int nSaleOut = shopListVector.at(index).SoldOut;
	if (nSaleOut == 0)
	{
		itemBtn->setEnabled(false, false);

		auto graySprite = Scale9Sprite::create(comm_graybgp9);
		graySprite->setContentSize(cellItemSize);
		graySprite->setPosition(Vec2(cellItemSize.width * 0.5, cellItemSize.height * 0.5));
		cellBg->addChild(graySprite, BASIC_ZORDER_TOP);

		auto saleOutSprite = Sprite::createWithSpriteFrameName("prop_saleout.png");
		saleOutSprite->setPosition(Vec2(cellBg->getContentSize().width * 0.5, cellBg->getContentSize().height * 0.5));
		cellBg->addChild(saleOutSprite, BASIC_ZORDER_TOP);
	}

	//添加监听
	cellBg->addTouchEventListener(CC_CALLBACK_2(PropShopLayer::isItemClick, this));

	return cellBg;
}

int PropShopLayer::calcCellCount()
{
	itemListSize = shopListVector.size();

	int cellNum;
	if (itemListSize % 2 == 0)
	{
		cellNum = itemListSize / 2;
	}
	else
	{
		cellNum = (itemListSize + 1) / 2;
	}
	return cellNum;
}

void PropShopLayer::reflashListView()
{
	m_pListView->removeAllItems(); //清空Item

	nItemIndex = 0; //每次刷新的时候，这个Index一定要reset

	//itemListSize = 7; //TODO TEST
	int cellCount = calcCellCount();

	for (int i = 0; i < cellCount; ++i)
	{
		m_pListView->pushBackCustomItem(createCell());
	}

	//下次刷新时间
	RichTextCellObject pRichObj;
	pRichObj.Content = GetWWString("PropShopLayer_nextReflashTime","PropShopLayer");
	pRichObj.Font = "res/Fonts/FounderSemiBold.ttf";
	pRichObj.rgb = propshop_nextflash_txt1_color;
	pRichObj.Size = 38;

	RichTextCellObject pRichObj2;
	//char todaytime[100] = {0};
	//sprintf(todaytime, GetWWString("PropShopLayer_todaytime","PropShopLayer").c_str(), reflashTime.c_str());
	pRichObj2.Content = reflashTime;
	pRichObj2.Font = "res/Fonts/FounderSemiBold.ttf";
	pRichObj2.rgb = propshop_nextflash_txt2_color;
	pRichObj2.Size = 38;

	std::vector<RichTextCellObject> richTextArr;
	richTextArr.push_back(pRichObj);
	richTextArr.push_back(pRichObj2);

	auto richText = WWRichText::create(Size(800, 38), richTextArr);
	richText->setAnchorPoint(VisibleRect::anchLeft());
	richText->setPosition(Vec2(m_pListView->getPositionX() - m_pListView->getContentSize().width * 0.5, 60));
	//richText->setOpacity(255);
	//richText->setColor(ccColor3B(255, 255,255));
	m_pListView->getParent()->addChild(richText);

	//刷新的时候，判断如果全部售空，则弹出Tip提示
	if (isAllSaleOut)
	{
		saleOutTip();
	}
}

void PropShopLayer::saleOutTip()
{
	auto tipBg = Sprite::create("res/Images/GameScene/Chat/voiceBg.png");
	tipBg->setPosition(Vec2(this->getpBg()->getContentSize().width * 0.6, this->getpBg()->getContentSize().height * 0.15));
	this->getpBg()->addChild(tipBg, BASIC_ZORDER_TOP);

	//根据种类去随机获得对话类容
	auto m_pLabel = Label::createWithTTF("","res/Fonts/FounderSemiBold.ttf",40,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	m_pLabel->setDimensions(500, 80);
	m_pLabel->setPosition(Vec2(tipBg->getContentSize().width * 0.5, tipBg->getContentSize().height * 0.5));
	m_pLabel->setColor(Color3B::BLACK);
	m_pLabel->setString(GetWWString("PropShopLayer_reflashNewTip","PropShopLayer"));
	tipBg->addChild(m_pLabel);

	tipBg->runAction(Sequence::create(FadeIn::create(0.5f),
		DelayTime::create(2.0f),
		RemoveSelf::create(),nullptr));
}

bool PropShopLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	m_tPoint = pTouch->getLocation();

	WWFrameBase::onTouchBegan(pTouch, pEvent);

	return true;
}

void PropShopLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	WWFrameBase::onTouchEnded(pTouch, pEvent);
}

//Item被选中的时候
void PropShopLayer::isItemClick(Ref *pSender, ui::Widget::TouchEventType type)
{
	auto node = dynamic_cast<ImageView*>(pSender);

	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		{
			node->setScale(0.98f);
		}
		break;
	case ui::Widget::TouchEventType::CANCELED:
		{
			node->setScale(1.0f);
		}
		break;
	case ui::Widget::TouchEventType::ENDED:
		{
			node->setScale(1.0f);

			int nSaleOut = shopListVector.at(node->getTag()).SoldOut;
			if (nSaleOut == 0)
			{
				WWToast::makeText(GetWWString("PropShopLayer_SaleOutTip","PropShopLayer"));
			}
			else
			{
				//购买逻辑
				buyDialogFrame(node->getTag());
			}

		}
		break;
	default:
		break;
	}

}

void PropShopLayer::TouchSpriteBtnEvent(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	//TODO 购买
}

//购买框
void PropShopLayer::buyDialogFrame(int id)
{
	PropDataStruct itemInfo = shopListVector.at(id);

	if (this->getChildByTag(SHOPTAG_BUYLAYER_TAG))
	{
		this->removeChildByTag(SHOPTAG_BUYLAYER_TAG);
	}
	auto buyLayer = BuyDialog::create(itemInfo, StoreID);
	buyLayer->setPosition(VisibleRect::leftBottom());
	addChild(buyLayer, BASIC_ZORDER_TOP, SHOPTAG_BUYLAYER_TAG);
}

void PropShopLayer::buyEndHandler(int id)
{
	shopListVector.at(id).SoldOut = 0;

	reflashListView();
}

void PropShopLayer::onDataReceiver()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	int gameID = pClientSocket->ReadInt();//(int4)游戏ID
	StoreID = pClientSocket->ReadInt();
	reflashTime = pClientSocket->ReadString();
	int count = pClientSocket->ReadShort();//(int2)商品数量

	shopListVector.clear();

	for(int i = 0; i < count; i++) //商店道具ID
	{
		PropDataStruct PropData;
		PropData.index = i;
		PropData.StoreMagicID = pClientSocket->ReadInt(); //(int4)商店道具ID
		PropData.MagicID = pClientSocket->ReadInt(); //(int4)道具ID
		PropData.Money = pClientSocket->ReadInt(); //(int4)价格,单位分
		PropData.MoneyType = pClientSocket->ReadChar(); //货币类型
		PropData.SoldOut = pClientSocket->ReadChar(); //是否售空
		PropData.Name = pClientSocket->ReadString(); //(String)商品名称

		PropData.Description = pClientSocket->ReadString(); //(String)描述文字
		PropData.Introduce = pClientSocket->ReadString(); //(String)简介
		PropData.magicCount = pClientSocket->ReadInt(); //(int4) 物品道具、数量
		PropData.fid = pClientSocket->ReadInt(); //(int4) 功能ID
		PropData.Expire = pClientSocket->ReadString(); //(String) 有效时间

		if (PropData.SoldOut == 1) //未售空
		{
			isAllSaleOut = false;
		}

		shopListVector.push_back(PropData);
	}

	reflashListView();
}

void PropShopLayer::onCanSaleListReceiver()
{
	//SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	//int userID = pClientSocket->ReadInt(); //用户ID
	//int gameID = pClientSocket->ReadInt();//(int4)游戏ID

	//int count = pClientSocket->ReadShort();//(int2)记录条数
	//for(int i = 0; i < count; i++) //商店道具ID
	//{
	//	PropCanSaleListStruct PropData;
	//	PropData.magicID = pClientSocket->ReadInt();
	//	PropData.magicCount = pClientSocket->ReadInt();
	//	PropData.name = pClientSocket->ReadString();
	//	PropData.expire = pClientSocket->ReadString();
	//	PropData.fid = pClientSocket->ReadInt();
	//	PropData.desc = pClientSocket->ReadString();
	//	PropData.sellFlag = pClientSocket->ReadChar();
	//	PropData.sellPrice = pClientSocket->ReadInt();
	//	PropData.sellCashType = pClientSocket->ReadChar();
	//}


	auto sale = SaleDialog::create();
	this->addChild(sale, BASIC_ZORDER_TOP);
}





/************************************************************************/
/*   购买                                                               */
/************************************************************************/
BuyDialog::BuyDialog(void)
{

}

BuyDialog::~BuyDialog()
{
}

void BuyDialog::onEnter()
{
	LayerColor::onEnter();
	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(BuyDialog::onTouchBegan, this);
	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);
}

void BuyDialog::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	LayerColor::onExit();
}

BuyDialog* BuyDialog::create(PropDataStruct &prodata, int StoreID)
{
	BuyDialog* pRet = new BuyDialog();
	if (pRet && pRet->init(prodata, StoreID))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool BuyDialog::init(PropDataStruct &prodata, int StoreID)
{
	if (!LayerColor::initWithColor(Color4B(0, 0 ,0, 128)))
		return false;

	if (!prodata.MagicID)
	{
		return false;
	}

	this->setName("BuyDialog");
	this->prodata = prodata;
	nStoreID = StoreID;

	auto buy_lBg = Sprite::createWithSpriteFrameName(prop_detailbg);
	buy_lBg->setPosition(Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5));
	this->addChild(buy_lBg, BASIC_ZORDER_TOP);

	//物品图片背景
	auto itemBg = Sprite::createWithSpriteFrameName(prop_itembg);
	itemBg->setPosition(Vec2(124, 211));
	buy_lBg->addChild(itemBg, BASIC_ZORDER_FRAME);

	//图片 TODO 这里应该封装一个对象，根据URL去获得图片资源
	auto item = Sprite::create(prop_itemtmp);
	item->setPosition(Vec2(itemBg->getContentSize().width * 0.5, itemBg->getContentSize().height * 0.5));
	item->setScale(1.8);
	itemBg->addChild(item);

	//名称
	std::string sName = prodata.Name;
	auto labelName = Label::createWithTTF(sName, "res/Fonts/FounderSemiBold.ttf",40,Size::ZERO,TextHAlignment::LEFT, TextVAlignment::CENTER);
	labelName->setColor(GET_RGBCOLOR(propshop_sale_title_text));
	labelName->setAnchorPoint(VisibleRect::anchCenter());
	labelName->setPosition(Vec2(itemBg->getPositionX(), 105));
	buy_lBg->addChild(labelName,BASIC_ZORDER_ROOT);

	//详细
	std::string sDetail = prodata.Description;
	auto labelDetail = Label::createWithTTF(sDetail, "res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	labelDetail->setColor(GET_RGBCOLOR(propshop_sale_title_text));
	labelDetail->setAnchorPoint(VisibleRect::anchLeft());
	labelDetail->setPosition(Vec2(267, 280));
	labelDetail->setDimensions(395, 131);
	buy_lBg->addChild(labelDetail,BASIC_ZORDER_ROOT);

	
	auto spliteLine = Sprite::createWithSpriteFrameName(prop_spiltline);
	spliteLine->setPosition(Vec2(469, 145));
	buy_lBg->addChild(spliteLine);

	//购买按钮
	auto buyBtn = WWTouchSprite::createWithFile(prop_itembtn, this, callfuncO_selector(BuyDialog::TouchSpriteBtnEvent));
	buyBtn->setPosition(Vec2(585, 77));
	buy_lBg->addChild(buyBtn, BASIC_ZORDER_ROOT, SHOPTAG_BUY);

	auto labelBuy = Label::createWithTTF(GetWWString("PropShopLayer_BuyBtnLabel","PropShopLayer"), "res/Fonts/FounderSemiBold.ttf",40,Size::ZERO,TextHAlignment::CENTER, TextVAlignment::CENTER);
	labelBuy->setColor(GET_RGBCOLOR(propshop_buy_title_text));
	labelBuy->setAnchorPoint(VisibleRect::anchCenter());
	labelBuy->setPosition(Vec2(0, 0));
	buyBtn->addChild(labelBuy,BASIC_ZORDER_ROOT);

	//添加关闭按钮
	auto m_pClose = WWTouchSprite::createWithSpriteFrameName(buy_closeBtn,this, callfuncO_selector(BuyDialog::TouchSpriteBtnEvent));
	m_pClose->setPosition(Vec2(700,350));
	buy_lBg->addChild(m_pClose,BASIC_ZORDER_ROOT, SHOPTAG_CLOSE);

	return true;
}

bool BuyDialog::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void BuyDialog::TouchSpriteBtnEvent(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();

	switch (tagId)
	{
	case SHOPTAG_BUY:
		{
			//发送购买协议
			int magicID, storeID, price;
			magicID = prodata.MagicID;
			storeID = nStoreID;
			price = prodata.Money;
			MsgCommonRequest::sendBuyMagicReq(magicID, storeID, price);

		}
		break;
	case SHOPTAG_CLOSE:
		{
			this->removeFromParent();
		}
		break;
	default:
		break;
	}

}

void BuyDialog::onDataReceiver()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int MagicID  = pClientSocket->ReadInt();//道具ID
	int StoreID  = pClientSocket->ReadInt();//(int4) 商店ID
	int DestUserID  = pClientSocket->ReadInt();//道具使用目标用户ID，对即买即用类游戏道具有效
	int BuyPrice  = pClientSocket->ReadInt();//道具的支付总价

	/*
	0=成功；
	1＝用户帐户余额不足；
	11=物品不存在或者已经售馨;
	2, 用户帐户余额不足,不足房间准入
	-1=其它异常
	*/
	int result  = pClientSocket->ReadChar();
	int SpareTime  = pClientSocket->ReadInt();//剩余有效时间(毫秒)
	std::string UseCash  = pClientSocket->ReadString();//(String)用户当前剩余财富
	std::string Desc  = pClientSocket->ReadString();//描述信息

	int CashType  = pClientSocket->ReadChar();//(int1) 1钻石 2 金币 
	int Count  = pClientSocket->ReadShort();//购买数量

	switch (result)
	{
	case 0: //成功
		{
			WWToast::makeText(GetWWString("PropShopLayer_Success","PropShopLayer"));

			//购买成功后刷新商城列表
			auto propShoper = dynamic_cast<PropShopLayer*>(this->getParent());
			if (propShoper) //存在购买框子节点
			{
				propShoper->buyEndHandler(prodata.index);
			}
		}
		break;
	case 1: //用户帐户余额不足
		{
			WWToast::makeText(GetWWString("PropShopLayer_nomoney","PropShopLayer"));
		}
		break;
	case 11: //物品不存在或者已经售馨
		{
			WWToast::makeText(GetWWString("PropShopLayer_saleout","PropShopLayer"));
		}
		break;
	case 2: //用户帐户余额不足,不足房间准入
		{

		}
		break;
	case 3: //指定时段已经购买过该道具
		{
			WWToast::makeText(GetWWString("PropShopLayer_hadbuy","PropShopLayer"));
		}
		break;

	default:
		//其它
		break;
	}

	this->removeFromParent(); //不管什么情况，只要消息一来就首先关闭掉窗口
}


/************************************************************************/
/*   出售                                                               */
/************************************************************************/
SaleDialog::SaleDialog(void) : 
	m_pListView(nullptr),
	nItemIndex(0)
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

}

SaleDialog::~SaleDialog()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);
}

bool SaleDialog::init()
{
	if(!WWFrameBase::initSmallFrame())
	{
		return false;
	}

	this->setName("SaleDialog");

	bool resurlt = false;
	do 
	{
		createView();
		PaserData();
		resurlt = true;
	} 
	while (0);

	return resurlt;
}

void SaleDialog::createView()
{


	 //创建ListView
	if (!m_pListView)
	{
		m_pListView = ListView::create();
		// 设置方向
		m_pListView->setDirection(ui::ScrollView::Direction::VERTICAL);
		m_pListView->setTouchEnabled(true);
		m_pListView->setBounceEnabled(true); //是否开启回弹
		m_pListView->setBackGroundImage(bgFrameIn);
		m_pListView->setBackGroundImageScale9Enabled(true); //是否按.9方式拉升
		m_pListView->setContentSize(listSaleBgSize); 
		m_pListView->setAnchorPoint(Vec2(0.5, 0.5));
		m_pListView->setPosition(Vec2(this->getpBg()->getContentSize().width * 0.5, this->getpBg()->getContentSize().height * 0.55));
		m_pListView->setItemsMargin(17.0f);
		//m_pListView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(PropShopLayer::listItemClick, this));
		this->getpBg()->addChild(m_pListView, BASIC_ZORDER_TOP);
	}
	//出售提示Label
	std::string str_labelTitle = GetWWString("SaleDialog_saletotle","PropShopLayer");
	auto labelName = Label::createWithTTF(str_labelTitle, "res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	labelName->setColor(GET_RGBCOLOR(propshop_title_txt_color));
	labelName->setAnchorPoint(Vec2(0,0.5f));
	labelName->setPosition(Vec2(m_pListView->getPositionX() - m_pListView->getContentSize().width * 0.5, m_pListView->getPositionY() + m_pListView->getContentSize().height * 0.5 + labelName->getContentSize().height * 0.6));
	this->getpBg()->addChild(labelName,BASIC_ZORDER_TOP + 1);

	auto itemBtn = WWTouchSprite::createWithFile(btn_gold, this, callfuncO_selector(SaleDialog::TouchSpriteBtnEvent));
	itemBtn->setPosition(Vec2(this->getpBg()->getContentSize().width * 0.5, 112));
	this->getpBg()->addChild(itemBtn);

	std::string str_label = GetWWString("SaleDialog_sale","PropShopLayer");
	auto btn_label = Text::create();
	btn_label->setContentSize(Size(333, 34));
	btn_label->setFontSize(30);
	btn_label->setFontName("res/Fonts/FounderSemiBold.ttf");
	btn_label->setColor(GET_RGBCOLOR(propshop_title_txt_color));
	btn_label->setString(str_label);
	itemBtn->addChild(btn_label);
	btn_label->setPosition(Vec2(0, 0));
}

Layout* SaleDialog::createCell()
{
	Layout* default_item = Layout::create();
	default_item->setTouchEnabled(true);
	default_item->setContentSize(Size(listSaleBgSize.width, cellItemSize.height));

	for (unsigned int i = 0; i < 5; i++)
	{
		if (nItemIndex >= saleListArr.size())
		{
			break;
		}

		PropCanSaleListStruct cellData = saleListArr.at(nItemIndex); //取得列表中当前数据

		//物品地址

		//物品图片背景
		auto itemBg = Sprite::createWithSpriteFrameName(prop_itembg);

		float boldSize = (default_item->getContentSize().width - (5 * itemBg->getContentSize().width + 4 * 70)) / 2;
		float x = boldSize + itemBg->getContentSize().width * 0.5 + i * (70 + itemBg->getContentSize().width);
		float y = default_item->getContentSize().height * 0.5f;
		itemBg->setPosition(Vec2(x, y));

		//图片 TODO 这里应该封装一个对象，根据URL去获得图片资源
		auto item = Sprite::create(prop_itemtmp);
		item->setPosition(Vec2(itemBg->getContentSize().width * 0.5, itemBg->getContentSize().height * 0.5));
		item->setScale(1.8);
		itemBg->addChild(item);

		default_item->addChild(itemBg, BASIC_ZORDER_FRAME, nItemIndex);
		nItemIndex++;
	}

	return default_item;
}

int SaleDialog::calcCellCount()
{
	int itemListSize = saleListArr.size();

	int cellNum;
	if (itemListSize % 5 == 0)
	{
		cellNum = itemListSize / 5;
	}
	else
	{
		cellNum = (itemListSize) / 5  + 1;
	}
	return cellNum;
}


bool SaleDialog::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void SaleDialog::TouchSpriteBtnEvent(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	
	switch (tagId)
	{
	case SALEDIALOG_CLOSE:
		{
		   this->removeFromParent();
		}
		break;
	default:
		break;
	}
}

void SaleDialog::PaserData()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	int userID = pClientSocket->ReadInt(); //用户ID
	int gameID = pClientSocket->ReadInt();//(int4)游戏ID

	int cash1 = pClientSocket->ReadInt(); //钻石
	int cash2 = pClientSocket->ReadInt(); //金币

	int count = pClientSocket->ReadShort();//(int2)记录条数
	for(int i = 0; i < count; i++) //商店道具ID
	{
		PropCanSaleListStruct PropData;
		PropData.magicID = pClientSocket->ReadInt(); //道具id
		PropData.magicCount = pClientSocket->ReadInt(); //道具数量
		PropData.name = pClientSocket->ReadString(); //道具名称
		PropData.expire = pClientSocket->ReadString(); //道具过期时间
		PropData.fid = pClientSocket->ReadInt(); //道具fid
		PropData.desc = pClientSocket->ReadString(); //道具描述信息
		PropData.sellFlag = pClientSocket->ReadChar(); //道具是否可出售0-不可出售、1-可出售
		PropData.sellPrice = pClientSocket->ReadInt(); //道具出售价格
		PropData.sellCashType = pClientSocket->ReadChar(); //道具出售货币类型 0-钻石、1-金币
		saleListArr.push_back(PropData);
	}

	reflashListView();
}

void SaleDialog::reflashListView()
{
	for (int i = 0; i < calcCellCount(); ++i)
	{
		m_pListView->pushBackCustomItem(createCell());
	}
}
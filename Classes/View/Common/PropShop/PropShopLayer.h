/*! @file
********************************************************************************
<PRE>
模块名       :  道具商城
文件名       :  PropShowLayer.h
文件实现功能 :  道具商城
作者         :  Diyal.yin
版本         :  1.0
Copyright (c)  WaWaGame Technologies
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/07/07   1.0      diyal.yin           新建商城界面
2015/07/08   1.0      diyal.yin           新建购买框
2015/07/09   1.0      diyal.yin           新建出售框

</PRE>
*******************************************************************************/
#ifndef _VIEW_COMMON_PROPSHOPLAYER_H_
#define _VIEW_COMMON_PROPSHOPLAYER_H_

#include "cocos2d.h"
#include <iostream>
#include "BaseCore/WWBGUI/WWFrameBase.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;

/* TAG */
enum SHOPTAG
{
	SHOPTAG_BUY = 200, //购买框  购买按钮
	SHOPTAG_CLOSE, //关闭按钮
	SHOPTAG_BUYLAYER_TAG, //购买框Tag
};

//道具商品
struct PropDataStruct 
{
	int index;        //索引
	int StoreMagicID; //道具商店ID
	int MagicID;      //道具ID
	int Money;        //Price
	int MoneyType;    //MoneyType 货币类型
	int SoldOut;      //是否售空
	std::string Name; //商品名称
	std::string Description;  //描述文字
	std::string Introduce;    //简介
	int magicCount;           //物品道具，数量
	int fid;                  //功能ID
	std::string Expire;        //有效时间
};

class PropShopLayer : public WWFrameBase
{
public:
	PropShopLayer();
	virtual ~PropShopLayer();

	CREATE_FUNC(PropShopLayer);
	virtual bool init();	
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);

	virtual void onEnter();
	virtual void onExit();

	void createView();

	Layout* createCell(); //index为Item的Tag

	ImageView* createCellItem(int index); // 创建一个Cell物品

	void reflashListView(); //刷新listview内容

	int calcCellCount(); //计算创建Cell的行数

	void TouchSpriteBtnEvent(Ref * node); //图标按钮事件

	void listItemClick(Ref* pSender, ListView::EventType type);

	void saleOutTip(); //售空提示框

	void isItemClick(Ref *pSender, ui::Widget::TouchEventType type); //判断是否被选中

	//实现网络回调的虚函数
	virtual void onDataReceiver(); 

	//可出售道具列表
	virtual void onCanSaleListReceiver(); 

	void buyDialogFrame(int id);

	void buyEndHandler(int id); //购买完处理逻辑

private:
	std::vector<PropDataStruct> shopListVector;
	std::string reflashTime; //下次刷新时
	int StoreID; //商店ID

private:
	CC_SYNTHESIZE(ListView* ,m_pListView, pListView);
	int nItemIndex; //当前创建的Item个数，创建Item的时候计数用
	bool isAllSaleOut; //售空 （数据解析的时候，该值初始设置为true，一旦有一个没有售空，就设置为false）
	int itemListSize; //商品个数 方便调试

	Point m_tPoint; //触摸点的位置
};

/* 购买层 */
class BuyDialog : public LayerColor
{
public:

	BuyDialog();
	virtual ~BuyDialog();
	static BuyDialog* create(PropDataStruct &prodata, int StoreID);

	bool init(PropDataStruct &prodata, int StoreID);

	virtual void onEnter();

	virtual void onExit();

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void TouchSpriteBtnEvent(Ref * node); //图标按钮事件

	virtual void onDataReceiver(); //实现网络回调的虚函数

private:
	PropDataStruct prodata;
	int nStoreID;
};

//可售道具商品
struct PropCanSaleListStruct 
{
	int magicID;      //道具ID
	int magicCount;   //道具数量
	std::string name;         //道具名称
	std::string expire ;      //道具过期时间
	int fid;                  //道具fid
	std::string desc ;      //道具描述信息
	int sellFlag;         //道具是否可出售
	int sellPrice;        //道具出售价格
	int sellCashType;     //道具出售货币类型 0-钻石、1-金币
};

/* 购买层 */
/* TAG */
enum SALEDIALOGTAG
{
	SALEDIALOG_CLOSE, //关闭按钮
};

class SaleDialog : public WWFrameBase
{
public:

	SaleDialog();
	virtual ~SaleDialog();
	//static SaleDialog* create();

	CREATE_FUNC(SaleDialog);

	bool init();

	//virtual void onEnter();

	//virtual void onExit();

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void createView();

	Layout* createCell(); //index为Item的Tag

	int calcCellCount(); //计算创建Cell的行数

	void TouchSpriteBtnEvent(Ref * node); //图标按钮事件

	void reflashListView();

	virtual void PaserData(); //解析队列中的消息，并且调用刷新页面函数

private:
	CC_SYNTHESIZE(ListView* ,m_pListView, pListView);
	std::vector<PropCanSaleListStruct> saleListArr;
	int nItemIndex; //当前创建的Item个数，创建Item的时候计数用
};


#endif /*_VIEW_COMMON_PROPSHOPLAYER_H_*/
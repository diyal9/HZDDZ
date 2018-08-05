#include "MsgCommonRequest.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"

USING_NS_CC;

MsgCommonRequest::MsgCommonRequest()
{

}

MsgCommonRequest::~MsgCommonRequest()
{

}

void MsgCommonRequest::sendRequestMedia(int type,int userID, int iconID)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	/*
	(int1)请求类型：
	0 用户形象图片
	4 Vip图像
	5 系统道具物品图像
	9 游戏积分头衔图片
	15 游戏道具图片
	16 及时比赛图片
	*/
	pClientSocket->WriteChar(type);

	pClientSocket->WriteInt(userID);
	pClientSocket->WriteInt(iconID);

	pClientSocket->SendCommand();
}

void MsgCommonRequest::sendOpenMoneyBoxReq(int boxID, int type, int boxType)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(60);

	pClientSocket->WriteInt(boxID);//(int4)宝箱的ID
	pClientSocket->WriteChar(type);//(int1)请求类型  1 请求宝箱列表  2 请求打开宝箱
	pClientSocket->WriteInt(1001);//(int4) 游戏ID
	pClientSocket->WriteInt(boxType);//(int4)道具宝箱类型   1105 大厅宝箱   1102 连胜宝箱

	pClientSocket->SendCommand();
}

void MsgCommonRequest::sendMagicStoreReq(int storeID)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(17);
	pClientSocket->WriteChar(11);
	pClientSocket->WriteChar(1);

	pClientSocket->WriteChar(0);//(int1)0=商店商品列表
	pClientSocket->WriteInt(1001);//(int4)游戏ID
	/*
		(int4) 商店ID
		1001=比赛会员商店,
		1002=比赛门票商店
		1003：游戏会员商店
		1004：游戏对局中道具价格表
		1005：单机游戏道具价格表
		1006：IOS+COCOS道具商店1
		1007：Android道具商店1
		1008: 头像商店
		1009 : 头像框商店
	*/
	pClientSocket->WriteInt(storeID);// 商店ID
	pClientSocket->WriteChar(2);//（int1）1 – 钻石 2金币

	pClientSocket->SendCommand();
}

void MsgCommonRequest::sendBuyMagicReq(int magicID, int storeID, int price)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(17);
	pClientSocket->WriteChar(3);
	pClientSocket->WriteChar(1);

	pClientSocket->WriteInt(magicID);//道具ID;见说明

	/*
	(int4) 商店ID
	1001=比赛会员商店,
	1002=比赛门票商店
	1003：游戏会员商店
	1004：游戏对局中道具价格表
	1006：IOS+COCOS道具商店1
	1007：Android道具商店1
	1008: 头像商店
	1009 : 头像框商店
	*/
	pClientSocket->WriteInt(storeID);// 商店ID

	pClientSocket->WriteInt(price);//道具价格

	pClientSocket->WriteShort(1);//购买数量

	pClientSocket->WriteInt(0);//道具使用目标用户ID，对即买即用类游戏道具有效

	pClientSocket->WriteInt(1001);//游戏ID，游戏道具有效

	pClientSocket->WriteInt(0);//用户当前游戏对局ID,对游戏道具有效

	pClientSocket->WriteChar(2);//(int1) 1钻石 2 金币 

	pClientSocket->WriteChar(0);//(int1) 购买使用道具的游戏玩法,如果道具与玩法无关，填写0
	pClientSocket->WriteShort(0);//(int4)游戏区ＩＤ游戏中购买时填写，否则填0

	pClientSocket->SendCommand();
}

void MsgCommonRequest::sendSaleAllPropReq()
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(17);
	pClientSocket->WriteChar(11);
	pClientSocket->WriteChar(5);

	pClientSocket->WriteInt(1001);
	pClientSocket->SendCommand();
}


void MsgCommonRequest::sendBuddyInfoReq(int type,int objectID,int param1,int param2)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(64);

	/**
		0=获取推荐好友表
		1=获取我的好友列表
		2=获取好友请求表
		3=添加好友请求
		4=同意/拒绝好友请求
		5=删除好友请求
		6=赠送
		7=获取赠送
	*/
	pClientSocket->WriteChar(type);
	pClientSocket->WriteInt(objectID);
	pClientSocket->WriteInt(param1);
	pClientSocket->WriteInt(param2);

	pClientSocket->SendCommand();
	log("send buddy------->");
}
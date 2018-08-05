#include "MsgChargeRequest.h"

#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "Control/WWDataManager/WWDataManager.h"

USING_NS_CC;

MsgChargeRequest::MsgChargeRequest()
{
}

MsgChargeRequest::~MsgChargeRequest()
{
}

void MsgChargeRequest::requestChargeCenterMenu()
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(100);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(1);

	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//

	/*
	(int1)请求类型：
	0 获取充值接入信息
	1 获取充值描述信息
	2 获取充值明细
	3直充菜单
	5 购买蓝晶|红晶|紫晶会员
	6 购买物品充值菜单
	7移动会员菜单
	*/
	pClientSocket->WriteChar(0);

	pClientSocket->WriteInt(3983);//(int4) 菜单ID   13：蛙豆充值

	pClientSocket->WriteChar(1);
	pClientSocket->WriteShort(11);//[int2]运营商标识，0－忽略改条件   sp
	pClientSocket->WriteInt(5148);//[int4]合作方标识，0－忽略改条件       op

	pClientSocket->WriteInt(1);

	pClientSocket->WriteInt(0);//游戏区ID
	pClientSocket->WriteInt(0);
	pClientSocket->WriteInt(0);

	pClientSocket->SendCommand();
}

void MsgChargeRequest::requestMoneyInfo()
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(100);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(1);

	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//
	/*
	(int1)请求类型：
	0 获取充值接入信息
	1 获取充值描述信息
	2 获取充值明细
	3直充菜单
	5 购买蓝晶|红晶|紫晶会员
	6 购买物品充值菜单
	7移动会员菜单
	*/
	pClientSocket->WriteChar(3);
	pClientSocket->WriteInt(13);//(int4) 菜单ID   13：蛙豆充值
	/*
	(int1)请求位置
	1 大厅请求
	2 进房间请求
	3 游戏中请求
	*/
	pClientSocket->WriteChar(1);
	pClientSocket->WriteShort(11);//[int2]运营商标识，0－忽略改条件   sp
	pClientSocket->WriteInt(5148);//[int4]合作方标识，0－忽略改条件       op

	pClientSocket->WriteInt(1);

	//test code
	cocos2d::__Array* rooms = WWDataManager::getInstance()->getGameZone()->getRooms(2);
	GameRoom* room = (GameRoom*)(rooms->getObjectAtIndex(0));

	CCLOG("MsgChargeRequest::sendRequestMoneyInfo---room.id=%d",room->getGZID());

	pClientSocket->WriteInt(room->getGZID());//游戏区ID
	pClientSocket->WriteInt(0);
	pClientSocket->WriteInt(0);


	pClientSocket->SendCommand();
}
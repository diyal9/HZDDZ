
#include "MsgChargeFilter.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Model/WWDataModel/CommonMessageData/CommonMessageData.h"

USING_NS_CC;

const int filters[] ={ DIRECT_DEPOSITE_INFO, CHARGE_MENU_INFO};
const int rootFilters[] ={ 0 };

const std::vector<int> vfilters(filters, filters + sizeof(filters) / sizeof(int));

const std::vector<int> vrootfilters(rootFilters,rootFilters + sizeof(rootFilters) / sizeof(int));

MsgChargeFilter::MsgChargeFilter()
{
}

MsgChargeFilter::~MsgChargeFilter()
{
}

bool MsgChargeFilter::init()
{
	return true;
}


std::vector<int> MsgChargeFilter::onFilters()
{
	return vfilters;
}


bool MsgChargeFilter::onMessageReceived(int msgType)
{
	Ref* dataObj = nullptr;
	switch(msgType){
	case DIRECT_DEPOSITE_INFO: 
		dataObj = parseDirectDepositeInfo();
		break;
	case CHARGE_MENU_INFO:
		dataObj = parseChargeMenuInfo();
	default:
		return false;
	}
	onDataReceiver(msgType, dataObj);
	return true;
}


std::vector<int> MsgChargeFilter::onRootFilters()
{
	return vrootfilters;
}

bool MsgChargeFilter::onRootMessageReceived(int msgType, int userId, int result, std::string reason, int reasonType)
{
	CCLOG("MsgLogonDataFilter::onRootMessageReceived:msgType= %x,userId=%d,result=%d,reason=%s,reasonType=%x", msgType,userId,result,reason.c_str(),reasonType);
	CommonMessageData* data = CommonMessageData::create();
	data->setUserId(userId);
	data->setResult(result);
	data->setReasonType(reasonType);
	data->setReason(reason);
	//switch (msgType) {
	//case MSG_REGISTER_INFO:
	//	break;
	//case MSG_LOGON_INFO:
	//	break;
	//default:
	//	return false;
	//}

	onDataReceiver(msgType,data);
	return true;

}

//充值中心
cocos2d::Ref* MsgChargeFilter::parseChargeMenuInfo()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int MenuID = pClientSocket->ReadInt();//(int4)父菜单项ID
	int ReqType = pClientSocket->ReadChar();//(int1)请求类型 对应消息RequestMoneyInfo的Type值
	int position = pClientSocket->ReadChar();//(int1)请求位置 对应消息RequestMoneyInfo的position值
	int count = pClientSocket->ReadShort();//(int2)菜单数量

	CCLOG("MsgChargeFilter::parseChargeMenuInfo-----MenuID=%d,ReqType=%d,position=%d,count=%d",MenuID,ReqType,position,count);

	for(int i = 0; i < count; i++)
	{
		int menuIDs = pClientSocket->ReadInt();//(int4) 充值菜单项ID
		std::string Name = pClientSocket->ReadString();//(String )显示名称
		std::string icon = pClientSocket->ReadString();//(byte[])充值菜单图标(暂不需后台提供)
		int Hot = pClientSocket->ReadChar();//(int1) 0：普通菜单  1：热门菜单  2：推荐菜单
		int CashType = pClientSocket->ReadChar();//(int1)   1蛙币充值  2蛙豆充值
		CCLOG("MsgChargeFilter::parseChargeMenuInfo-----menuIDs = %d,Name = %s,icon = %s, Hot = %d, CashType = %d",menuIDs,Name.c_str(),icon.c_str(),Hot,CashType);
		/*
		(int1) 充值类型（仅二级菜单适用）
		3：充值卡
		4：香港运营商接口
		9：移动点数充值
		11：短信
		12：第三方（联想）接口充值
		13:支付宝充值
		14：MM充值
		15:电信SDK短信充值
		16;联通充值
		23移动SDK充值
		*/
		int ChargeType = pClientSocket->ReadChar();
		int ToUser = pClientSocket->ReadChar();//(int1)充值到帐对象  1：充值者自己  2：其它玩家（蛙号标识）
		std::string SendTo = pClientSocket->ReadString();//(String)接收地址(4)|端口（11）
		std::string Common = pClientSocket->ReadString();//(String) 指令（11）
		std::string MenuData = pClientSocket->ReadString();//(String)附加信息
		int Money = pClientSocket->ReadInt();//充值支付金额（RMB，单位：分）
		int SP = pClientSocket->ReadInt();//(int4)充值渠道商代码
		int SPServiceID = pClientSocket->ReadInt();//(int4)充值渠道商业务代码
		int Cash = pClientSocket->ReadInt();//(int4)充值正常应得货币（蛙币/蛙豆）
		int DonateCash = pClientSocket->ReadInt();//(int4)充值赠送货币（蛙币）

		CCLOG("MsgChargeFilter::parseChargeMenuInfo-----ChargeType = %d, ToUser = %d, SendTo = %s, Common = %s, MenuData = %s, Money = %d, SP = %d, SPServiceID = %d, Cash = %d, DonateCash = %d",
			ChargeType,ToUser,SendTo.c_str(),Common.c_str(),MenuData.c_str(),Money,SP,SPServiceID,Cash,DonateCash);

		std::string MenuKey = pClientSocket->ReadString();//(String)在发起充值时，客户端应回传给服务器
		std::string Description1 = pClientSocket->ReadString();//(String) 充值前提示
		std::string Description2 = pClientSocket->ReadString();//(String) 充值提交后的提示信息
		std::string Description3 = pClientSocket->ReadString();//(String) 重复提交的提示信息
		int Confirm = pClientSocket->ReadChar();//(int1) 短信二次确认 1：无二次确认  2：客户端安装后首次充值做二次确认n  3：每次充值做二次确认
		int SmsType = pClientSocket->ReadShort();//(int2)短信类型   1：UIDC类型  2：ODID类型  3: 第三方短信
		std::string SmsOrder = pClientSocket->ReadString();//(String)短信订单号
		int MenuType = pClientSocket->ReadChar();//(int1)菜单类型 0 普通 1 首充
		int TMagicID = pClientSocket->ReadInt();//(int4) 道具或道具包ID
		int MCount = pClientSocket->ReadChar();//(int1)道具物品数量

		CCLOG("MsgChargeFilter::parseChargeMenuInfo-----MenuKey = %s, Description1 = %s, Description2 = %s, Description3 = %s, Confirm = %d, SmsType = %d, SmsOrder = %s, MenuType = %d, TMagicID = %d,MCount = %d",
			MenuKey.c_str(), Description1.c_str(),Description2.c_str(),Description3.c_str(),Confirm,SmsType,SmsOrder.c_str(),MenuType,TMagicID,MCount);

		for(int j = 0; j < MCount; j++)
		{
			int MagicID = pClientSocket->ReadInt();//(int4)道具ID
			std::string MagicName = pClientSocket->ReadString();//(String)道具名称
			int MagicCount = pClientSocket->ReadInt();//(int4)道具数量
			int MagicFID = pClientSocket->ReadInt();//(int4)物品FID

			CCLOG("MsgChargeFilter::parseChargeMenuInfo-----MagicID = %d, MagicName = %s, MagicCount = %d, MagicFID = %d", MagicID,MagicName.c_str(),MagicCount,MagicFID);
		}
		CCLOG("------------------------------------------------");
	}

	return NULL;
}

//快充
cocos2d::Ref* MsgChargeFilter::parseDirectDepositeInfo()
{

	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	/*
	2-没有直充，转到充值菜单
	4-香港充值
	11－短信III类充值
	9-(新)网游点数充值
	12-酷宇接口
	14-MM充值
	15-电信
	16-联通
	*/
	int flag = pClientSocket->ReadChar();//(int1)

	int MenuType = pClientSocket->ReadChar();//(int1) 菜单类型 0：快充菜单 1：首充菜单
	int ReqType = pClientSocket->ReadChar();//(int1)请求类型  对应消息RequestMoneyInfo的Type值
	int Position = pClientSocket->ReadChar();//(int1)请求位置  对应消息RequestMoneyInfo的position值
	std::string SendTo = pClientSocket->ReadString();//(String)接收地址(4)|端口（11）
	std::string Commond = pClientSocket->ReadString();//(String) 指令（11）
	std::string Description = pClientSocket->ReadString();//(String)操作说明
	std::string MidURL = pClientSocket->ReadString();//(String)短信或者网游取号URL

	std::string SPServiceID = pClientSocket->ReadString();//(String)sp.spserviceids
	std::string Tips = pClientSocket->ReadString();//(String)提交的提示

	int Money = pClientSocket->ReadInt();//(int4)单次充值金额（分）
	int loopTimes = pClientSocket->ReadShort();//(int2)循环次数

	CCLOG("MsgChargeFilter::parseDirectDepositeInfo-----flag=%d,MenuType=%d,ReqType=%d,Position=%d,SendTo=%s,Commond=%s,Description=%s,MidURL=%s,SPServiceID=%s,Tips=%s,Money=%d,loopTimes=%d",
		flag,MenuType,ReqType,Position,SendTo.c_str(),Commond.c_str(),Description.c_str(),MidURL.c_str(),SPServiceID.c_str(),Tips.c_str(),Money,loopTimes);

	//for(int i = 0; i < loopTimes; i++)
	//{
		int Fortune = pClientSocket->ReadInt();//(int4) 充值获得蛙币数(含赠送数量)
		int menuID = pClientSocket->ReadInt();//(int4)充值菜单ID
		int MoneyType = pClientSocket->ReadChar();//(int1)0蛙币，1蛙豆,
		std::string MenuName = pClientSocket->ReadString();//(String)菜单名称
		int DepFromAct = pClientSocket->ReadInt();//(int4)快充菜单来源操作  1:续局  2:清空  其它=客户端自带参数
		int Confirm = pClientSocket->ReadChar();//(int1) 短信二次确认1：无二次确认  2：客户端安装后首次充值做二次确认  3：每次充值做二次确认
		std::string Description3 = pClientSocket->ReadString();//(String) 二次确认提示
		int SmsType = pClientSocket->ReadShort();//(int2)短信类型 1：UIDC类型 不需请求订单号    2：ODID类型 需请求短信订单号    3: 第三方短信 不需请求订单号,实际订单号为SmsOrder
		std::string SmsOrder = pClientSocket->ReadString();//(String)短信订单号
		int TMagicID = pClientSocket->ReadInt();//(int4) 道具或道具包ID
		int count = pClientSocket->ReadChar();//(int1)道具物品数量

		CCLOG("MsgChargeFilter::parseDirectDepositeInfo-----Fortune=%d,menuID=%d,MoneyType=%d,MenuName=%s,DepFromAct=%d,Confirm=%d,Description3=%s,SmsType=%d,SmsOrder=%s,TMagicID=%d,count=%d",
			Fortune,menuID,MoneyType,MenuName.c_str(),DepFromAct,Confirm,Description3.c_str(),SmsType,SmsOrder.c_str(),TMagicID,count );

		for(int j = 0; j < count; j++)
		{
			int MagicID = pClientSocket->ReadInt();//(int4)道具ID

			std::string MagicName = pClientSocket->ReadString();//(String)道具名称
			
			int MagicCount = pClientSocket->ReadInt();//(int4)道具数量

			int MagicFID = pClientSocket->ReadInt();//(int4)物品FID

			CCLOG("MsgChargeFilter::parseDirectDepositeInfo-----MagicID=%d,MagicName=%s,MagicCount=%d,MagicFID=%d",MagicID,MagicName.c_str(),MagicCount,MagicFID);
			CCLOG("--------------------------------------------");
		}
	//}

	return NULL;
}

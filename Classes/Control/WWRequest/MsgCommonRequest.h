#ifndef _MSG_COMMON_REQUEST_H_
#define _MSG_COMMON_REQUEST_H_

#include "cocos2d.h"

class MsgCommonRequest
{
public :
	MsgCommonRequest();
	virtual ~MsgCommonRequest();

	//用户形象请求消息
	static void sendRequestMedia(int type, int userID, int iconID);

	//开游戏奖励宝箱 
	static void sendOpenMoneyBoxReq(int boxID, int type, int boxType);

	//请求游戏商店数据
	static void sendMagicStoreReq(int storeID);

	//购买道具请求
	static void sendBuyMagicReq(int magicID, int storeID, int price);

	//2.9.5.	玩家批量出售道具请求 MagicSellAllReq
	static void sendSaleAllPropReq();

	//请求好友列表
	static void sendBuddyInfoReq(int type,int objectID,int param1,int param2);
};


#endif // !_MSG_COMMON_REQUEST_H_

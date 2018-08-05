/*
 * MsgChargeRequest.h
 *	功能描述：
 * 
 *      Author: zx
 */

#ifndef _MSG_CHARGE_REQUEST_H_
#define _MSG_CHARGE_REQUEST_H_

#include "cocos2d.h"

class MsgChargeRequest
{
public:
	MsgChargeRequest();

	virtual ~MsgChargeRequest();

	/**请求充值信息，包括普通充值菜单，快充菜单等*/
	//static void sendRequestMoneyInfo(ChargeRequestMoneyInfo* data);
	static void requestMoneyInfo();

	static void requestChargeCenterMenu();
};

#endif // !_MSG_CHARGE_REQUEST_H_

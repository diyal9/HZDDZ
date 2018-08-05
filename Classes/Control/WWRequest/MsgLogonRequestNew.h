/*
 * MsgLogonRequest.h
 *	¹¦ÄÜÃèÊö£º
 *  Created on: 2015Äê4ÔÂ24ÈÕ ÉÏÎç9:50:47
 *      Author: hp
 */

#ifndef _CONTROL_WWREQUEST_MSGLOGONREQUESTNEW_H_
#define _CONTROL_WWREQUEST_MSGLOGONREQUESTNEW_H_
#include "cocos2d.h"

/*
 *µÇÂ½×¢²áÏà¹ØÏûÏ¢ÇëÇó
 */
class MsgLogonRequestNew
{
public:
	MsgLogonRequestNew();
	virtual ~MsgLogonRequestNew();
	static void sendLogon(const char* userId, const char* userPwd, int logonType);//·¢ËÍµÇÂ½ÏûÏ¢
	static void sendLogonOut();//·¢ËÍÍË³öµÇÂ½ÏûÏ¢
};

#endif /* _CONTROL_WWREQUEST_MSGLOGONREQUESTNEW_H_ */

/*
 * MsgLogonRequest.h
 *	功能描述：
 *  Created on: 2015年4月24日 上午9:50:47
 *      Author: hp
 */

#ifndef MSGLOGONREQUEST_H_
#define MSGLOGONREQUEST_H_
#include "cocos2d.h"

/*
 *登陆注册相关消息请求
 */
class MsgLogonRequest
{
public:
	MsgLogonRequest();
	virtual ~MsgLogonRequest();
	static void sendLogon(const char* userId, const char* userPwd, int logonType);//发送登陆消息
	static void sendLogonOut();//发送退出登陆消息
};

#endif /* MSGLOGONREQUEST_H_ */

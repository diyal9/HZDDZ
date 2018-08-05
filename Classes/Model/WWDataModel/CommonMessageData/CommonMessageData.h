/*
 * CommonMessageData.h
 *	功能描述：通用消息数据
 *  Created on: 2014年7月7日 上午10:21:57
 *      Author: hp
 */

#ifndef _Model_WWSataModel_COMMONMESSAGEDATA_H_
#define _Model_WWSataModel_COMMONMESSAGEDATA_H_

#include "cocos2d.h"

class CommonMessageData:public cocos2d::Ref
{
public:
	CommonMessageData();
	virtual ~CommonMessageData();
	virtual bool init();
	CREATE_FUNC(CommonMessageData);
	CC_SYNTHESIZE(int, userId, UserId);//请求的用户ID[int4]
	CC_SYNTHESIZE(int, result, Result);//操作结果:[int1]0:成功1:失败
	CC_SYNTHESIZE(std::string, reason, Reason);//String 操作成功或者失败的原因描述
	CC_SYNTHESIZE(int, reasonType, ReasonType);//Int2 操作成功或者失败的原因类型
};

#endif /* _Model_WWSataModel_COMMONMESSAGEDATA_H_ */

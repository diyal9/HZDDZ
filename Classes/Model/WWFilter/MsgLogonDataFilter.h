/*
 * MsgLogonDataFilter.h
 *	功能描述： 登录相关数据解析
 *  Created on: 2015年4月27日 下午2:09:51
 *      Author: diyal.yin
 */

#ifndef _Model_WWFilter_MsgLogonDataFilter_H_
#define _Model_WWFilter_MsgLogonDataFilter_H_
#include "cocos2d.h"
#include "BaseCore/WWNetDataBasic/WWIMsgFilter.h"

/*
 *登陆相关消息解析
 */
class MsgLogonDataFilter: public WWIMsgFilter
{
public:
	MsgLogonDataFilter();
	virtual ~MsgLogonDataFilter();

	virtual void onDataReceiver(int msgType,cocos2d::Ref* objectData ){};
	virtual std::vector<int> onFilters();
	virtual bool onMessageReceived(int msgType);
	virtual std::vector<int> onRootFilters();
	virtual bool onRootMessageReceived(int msgType, int userId, int result,std::string reason, int reasonType);
	cocos2d::Ref* parseProfile();//登陆返回

};

#endif /* _Model_WWFilter_MsgLogonDataFilter_H_ */

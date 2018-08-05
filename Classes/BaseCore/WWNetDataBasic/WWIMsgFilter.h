/*
 * WWIMsgFilter.h
 *
 *  Created on: 2014年5月21日
 *      Author: 尹艳龙
 */

#ifndef _BaseCore_WWNetDataBasic_WWIMsgFilter_H_
#define _BaseCore_WWNetDataBasic_WWIMsgFilter_H_
#include "cocos2d.h"

/**
 *消息过滤器的基类
 *所有消息接受的Filter都应该继承这个对象，并且实现基本的虚函数，并且根据相应的解析规则（MsgId），将二进制解析成对象
 *用于MsgManager中注册获取分发消息
 */
class WWIMsgFilter{
public :
	WWIMsgFilter();
	virtual ~WWIMsgFilter();
	//响应消息过滤器
	virtual std::vector<int>  onFilters()=0;
	//响应消息回调
	virtual bool onMessageReceived( int msgType)=0;
	//通用消息过滤器
	virtual std::vector<int>  onRootFilters()=0;
	//通用消息回调
	virtual bool onRootMessageReceived( int msgType,int userId,int result,std::string reason,int reasonType)=0;
	virtual void onDataReceiver(int msgType,cocos2d::Ref* objectData)=0;//过滤器中所有的消息通过此方法分发到场景中，根据type转型为相应的数据结构
};

#endif /* _BaseCore_WWNetDataBasic_WWIMsgFilter_H_ */

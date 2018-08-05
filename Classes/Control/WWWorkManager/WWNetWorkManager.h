//
//  WWNetWorkManager.h
//
//  Created by 尹艳龙 on 2015-04-22.
//
//

#ifndef __HZ_DDZ_WWWorkManager_WWNetWorkManage__
#define __HZ_DDZ_WWWorkManager_WWNetWorkManage__

#include "cocos2d.h"
#include "BaseCore//WWBSocketCore/WWBSocketClient.h"
#include "BaseCore/WWNetDataBasic/WWIMsgFilter.h"

USING_NS_CC;

/*
 183.62.101.253:10001--->192.168.10.131:5310
 183.62.101.253:10002--->192.168.10.132:5310
*/
//#define IP_TEMP "ctnet.wawagame.cn" //正式运营
//#define IP_TEMP "192.168.10.51"
#define IP_TEMP "192.168.10.132"
//#define IP_TEMP "192.168.10.131"
#define PORT_TEMP 5310

//#define IP_TEMP "183.62.101.253"
//#define PORT_TEMP 10001
//#define PORT_TEMP 10002



class WWNetWorkManager:public cocos2d::Ref
{
public:
	WWNetWorkManager();
	virtual ~WWNetWorkManager();

	static WWNetWorkManager* getInstance();

	//调用请求一个真正的网络请求
	bool Connect(const char* szServerIP, int wPort);

	//开启网络接受schedule
	void openRecvJob();

	//关闭网络接受schedule
	void closeRecvJob();

	//发送前准备（清空发送Buffer、写入封包头）
	void SendPrepare();

	//获得当前有效的Socket对象句柄
	SocketClient* getCurSocket();

	//断网处理

	//发送心跳包
	void JumpSendMessage();

	//向网络管理器注册消息回调

	//取得消息后反射到对象的实体模型
	//注册消息过滤器
	void registerMsgFilter(WWIMsgFilter *filter);

	//反注册消息过滤器
	void unregisterMsgFilter(WWIMsgFilter *filter);

	bool CheckHasAccountRecord(std::string& sAccount, std::string& sPassword);// 判定机器中是否有帐号纪录

	/************************************新加数据、状态参数******************************************************/
	void setSendMsgId(int sendMsgId);//发送消息ID
	int getSendMsgId();

	void setRecvMsgId(int recvMsgId);//接收消息ID
	int getRecvMsgId();

	void setIsConnectSuccess(bool success);
	bool getIsConnectSuccess();
	/********************************************************************************************/

private:
	//握手
	void netShark();

	void netRecSchedule(float dt); //开启网络数据接收

	//接收Socket网络数据
	void RecvNetMessage();

	/*
	* 网络数据分发处理
	* 消息分发策略，是在新请求的时候，在网络管理单例里面存储根据消息ID存储
	*/
	//void handlerMsg(int msgType);
	void onMessageReceived(const int msgType);
	void onRootMessageReceived(const int msgType);

protected:
	bool m_isReconnect; //是否重连
	bool m_isFirstJoin;//是否是第一次进游戏

	int m_recvMsg_Id; //接受的通道ID
	int m_sendMsg_Id; //发送消息ID
	bool m_isSucces; //是否握手成功

	//心跳计时处理
	int m_heartCount;
private:
	SocketClient* m_pClientSocket;

	//用一个Map存放消息接受队列，<消息ID， 消息体>
	std::vector<WWIMsgFilter* > m_filters; //消息接收过滤器存储容器
};

#endif /* defined(__HZ_DDZ_WWWorkManager_WWNetWorkManage__) */

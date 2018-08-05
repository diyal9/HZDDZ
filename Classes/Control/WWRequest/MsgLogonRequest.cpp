/*
* MsgLogonRequest.cpp
*	功能描述：
*  Created on: 2014年6月24日 上午9:50:47
*      Author: hp
*/

#include "MsgLogonRequest.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "Control/WWDataManager/WWDataManager.h"

USING_NS_CC;
MsgLogonRequest::MsgLogonRequest()
{

}

MsgLogonRequest::~MsgLogonRequest()
{

}


void MsgLogonRequest::sendLogon(const char* userId, const char* userPwd, int logonType)
{
	const char *myID = "";
	if(logonType == 3)////手机登陆需填写手机号码
	{
		myID = userId;
	}

	log("send logon msg");
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	//ÄÚÈÝ
	pClientSocket->WriteChar(2);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(1);

	pClientSocket->WriteString(userId);// (String)UserID/MID/MDN/token/UID
	pClientSocket->WriteString(userPwd);// (String)UserPWD/SysPWD/sid/sessionid
	pClientSocket->WriteChar(logonType);//(int1)  1 快速登录;  2 账号登录;  3 手机号登录
	pClientSocket->WriteInt(5148);	// (int4)合作方SP(详细值另见约定文档)
	pClientSocket->WriteShort(11);// (int2)运营商(详细值另见约定文档)
	pClientSocket->WriteInt(1001);	// (int4)登录应用的模块ID
	pClientSocket->WriteChar(1);// (int1)语言: 1 简体中文  2 繁体中文  3 英文
	pClientSocket->WriteInt(1001313);	//(int4)大厅ID
	pClientSocket->WriteString("3.4.37");//(String)版本号
	
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		pClientSocket->WriteString("ADVGA");// (String)客户端机型
		pClientSocket->WriteShort(800);// (int2)屏幕宽度
		pClientSocket->WriteShort(480);//(int2)屏幕高度
		pClientSocket->WriteString("800x480");//(String)手机型号大小

		pClientSocket->WriteInt(9); // (int4)运营商id 

		pClientSocket->WriteString("SM-G900F"); //(String)手机型号
		pClientSocket->WriteString("samsung"); //(String)手机厂商
		pClientSocket->WriteString("WIFI"); // (String)网络接入

		pClientSocket->WriteString("460029894584244"); // (String)IMSI
		pClientSocket->WriteString("352558068271908"); // (String) IMEI
		pClientSocket->WriteString(myID); // (String)手机号码
		pClientSocket->WriteString("ANDROID4.4.2"); // (String) OS+版本号，字母全部大写，如ANDROID2.3


		pClientSocket->WriteInt(0); // (int4)登录即返回游戏区列表游戏ID， ＝0不返回游戏区列表

		pClientSocket->WriteString(""); //(String)客户端Mac字符串
	#endif
		
	#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		
		pClientSocket->WriteString("ADVGA");// (String)客户端机型
		pClientSocket->WriteShort(WWDataManager::getInstance()->deviceInfo.width);// (int2)屏幕宽度
		pClientSocket->WriteShort(WWDataManager::getInstance()->deviceInfo.height);//(int2)屏幕高度
		pClientSocket->WriteString("800x480");//(String)手机型号大小

		pClientSocket->WriteInt(WWDataManager::getInstance()->deviceInfo.networkOperatorCode); // (int4)运营商id 

		pClientSocket->WriteString(WWDataManager::getInstance()->deviceInfo.PhoneMANUFACTURER.c_str()); //(String)手机型号
		pClientSocket->WriteString(WWDataManager::getInstance()->deviceInfo.phoneModel.c_str()); //(String)手机厂商
		pClientSocket->WriteString(WWDataManager::getInstance()->deviceInfo.apnType.c_str()); // (String)网络接入
		pClientSocket->WriteString(WWDataManager::getInstance()->deviceInfo.IMSI.c_str()); // (String)IMSI
		pClientSocket->WriteString(WWDataManager::getInstance()->deviceInfo.IMEI.c_str()); // (String) IMEI

		pClientSocket->WriteString(myID); // (String)手机号码
		pClientSocket->WriteString(WWDataManager::getInstance()->deviceInfo.SDkVersion.c_str()); // (String) OS+版本号，字母全部大写，如ANDROID2.3


		pClientSocket->WriteInt(0); // (int4)登录即返回游戏区列表游戏ID， ＝0不返回游戏区列表

		pClientSocket->WriteString(WWDataManager::getInstance()->deviceInfo.MacAddress.c_str()); //(String)客户端Mac字符串
	#endif

	pClientSocket->SendCommand();
}

void MsgLogonRequest::sendLogonOut()
{

	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//(int4)用户ID

	/*
	(int1)退出方式
	1 正常退出
	2 超时退出(掉线)
	3重复登录强制退出
	4系统超时(挂线、空闲超时) 
	*/
	pClientSocket->WriteChar(1);

	pClientSocket->SendCommand();

}


/*
* MsgLogonRequest.cpp
*	¹¦ÄÜÃèÊö£º
*  Created on: 2014Äê6ÔÂ24ÈÕ ÉÏÎç9:50:47
*      Author: hp
*/

#include "MsgLogonRequestNew.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"

USING_NS_CC;
MsgLogonRequestNew::MsgLogonRequestNew()
{

}

MsgLogonRequestNew::~MsgLogonRequestNew()
{

}

void MsgLogonRequestNew::sendLogon(const char* userId, const char* userPwd, int logonType)
{
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
	pClientSocket->WriteString("13510441867"); // (String)手机号码
	pClientSocket->WriteString("ANDROID4.4.2"); // (String) OS+版本号，字母全部大写，如ANDROID2.3


	pClientSocket->WriteInt(1001); // (int4)登录即返回游戏区列表游戏ID， ＝0不返回游戏区列表

	pClientSocket->WriteString(""); //(String)客户端Mac字符串

	pClientSocket->SendCommand();

}

void MsgLogonRequestNew::sendLogonOut()
{

}


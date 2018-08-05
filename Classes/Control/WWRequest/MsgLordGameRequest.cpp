/*
 * MsgGameRequest.cpp
 *	功能描述：
 *  Created on: 2014年6月27日 上午10:01:04
 *      Author: hp
 */

#include "MsgLordGameRequest.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "Control/WWDataManager/WWDataManager.h"
#include "Control/WWDataManager/GameGlobalDataManager.h"

USING_NS_CC;

MsgLordGameRequest::MsgLordGameRequest()
{
}

MsgLordGameRequest::~MsgLordGameRequest()
{
}



void MsgLordGameRequest::sendSelfCallLord(bool isJiao)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int flag = (isJiao ? 1 : 0);

	pClientSocket->WriteChar(7);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(13);

	//test code 后面需要根据具体的消息数据填写
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurPlayId());//gamePlayID
	CCLOG("MsgLordGameRequest::sendSelfCallLord ------------------gamePlayId-- %d",GameGlobalDataManager::getInstance()->getCurPlayId());
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//userID --  test code 
	//test code end

	pClientSocket->WriteChar(flag);
	pClientSocket->WriteChar(0);

	pClientSocket->SendCommand();
}

void MsgLordGameRequest::sendSelfGrabLord(bool isGrab)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int flag = (isGrab ? 1 : 0);

	pClientSocket->WriteChar(7);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(14);

	//test code  后面需要根据具体的消息数据填写
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurPlayId());//gamePlayId
	CCLOG("MsgLordGameRequest::sendSelfGrabLord ------------------gamePlayId-- %d",GameGlobalDataManager::getInstance()->getCurPlayId());
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//UserID
	//test code end

	pClientSocket->WriteChar(flag);
	pClientSocket->WriteChar(0);
	pClientSocket->WriteChar(0);
	pClientSocket->WriteString("");

	pClientSocket->SendCommand();

}

void MsgLordGameRequest::sendSelfMingPai(bool isMingPai, char* cards)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(7);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(15);

	//test code  后面需要根据具体的消息数据填写
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurPlayId());//gamePlayId
	CCLOG("MsgLordGameRequest::sendSelfMingPai ------------------gamePlayId-- %d",GameGlobalDataManager::getInstance()->getCurPlayId());
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//UserID
	//test code end

	pClientSocket->WriteString(isMingPai ? (cards) : (""));

	pClientSocket->SendCommand();

}

void MsgLordGameRequest::sendSelfNoMingPai()
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(7);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(17);

	//test code  后面需要根据具体的消息数据填写
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurPlayId());//gamePlayId
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//UserId
	pClientSocket->WriteChar(GameGlobalDataManager::getInstance()->getCurGameId());//GameID
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//UserID
	//test code end

	pClientSocket->SendCommand();

}

void MsgLordGameRequest::sendTurnOutPoker(char* pokers,char* replacePokers,int type,int max)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(7);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(3);

	//test code  后面需要根据具体的消息数据填写
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurPlayId());//gamePlayId
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//UserID
	//test code end

	pClientSocket->WriteString(pokers);
	pClientSocket->WriteInt(0);//这个是NextPlay，因为不是在最后，所以必须加上
	pClientSocket->WriteString(replacePokers);
	pClientSocket->WriteChar(type);
	pClientSocket->WriteChar(max);

	pClientSocket->SendCommand();

}

 void MsgLordGameRequest::sendTurnOutPoker(char* pokers)
 {
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(7);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(3);

	//test code  后面需要根据具体的消息数据填写
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurPlayId());//gamePlayId
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//UserID
	//test code end

	pClientSocket->WriteString(pokers);
	pClientSocket->WriteInt(-1);

	pClientSocket->SendCommand();
 }

 void MsgLordGameRequest::sendJiaBei(int type)
 {
	 WWNetWorkManager::getInstance()->SendPrepare();
	 SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();


	 
	 pClientSocket->WriteChar(7);
	 pClientSocket->WriteChar(1);
	 pClientSocket->WriteChar(18);

	 //test code 后面需要根据具体的消息数据填写
	 pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurPlayId());//gamePlayID
	 CCLOG("MsgLordGameRequest::sendSelfCallLord ------------------gamePlayId-- %d",GameGlobalDataManager::getInstance()->getCurPlayId());
	 pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//userID 
	 //test code end

	 pClientSocket->WriteChar(type);//(int1)  0:不加倍  1:加倍  2：加强加倍  3：加倍结束
	 pClientSocket->WriteChar(0);
	 pClientSocket->WriteInt(0);


	 pClientSocket->SendCommand();
 }



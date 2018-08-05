/*
* MsgLobbyRequest.cpp
*	功能描述：
*  Created on: 2015年5月8日 上午9:50:47
*      Author: diyal.yin
*/

#include "MsgLobbyRequest.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"

USING_NS_CC;
MsgLobbyRequest::MsgLobbyRequest()
{

}

MsgLobbyRequest::~MsgLobbyRequest()
{

}

void MsgLobbyRequest::requestRoomDatas(int gameId, int playType)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(6);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(35);

	pClientSocket->WriteShort(1); // Count(int2)游戏个数
	//pClientSocket->WriteInt(gameId); // GameIDs(n) (int4)游戏ID

	pClientSocket->WriteInt(1001); // GameIDs(n) (int4)游戏ID

	pClientSocket->WriteShort(playType); // PlayType(n) (int2)游戏玩法
	pClientSocket->WriteShort(0); // InfoLanID(n)(int2)语言，1015-大陆美女版适配信息
	pClientSocket->WriteChar(0); // Zonewin(n)(int1)游戏输赢类型5－金豆，2－金币

	pClientSocket->SendCommand();
}


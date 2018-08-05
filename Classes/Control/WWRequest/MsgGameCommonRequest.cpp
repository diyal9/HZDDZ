/*
 * MsgGameCommonRequest.cpp
 *	功能描述：
 *  Created on: 2014年7月3日 上午10:21:54
 *      Author: hp
 */

#include "MsgGameCommonRequest.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "Control/WWDataManager/WWDataManager.h"
#include "Control/WWDataManager/GameGlobalDataManager.h"

USING_NS_CC;

MsgGameCommonRequest::MsgGameCommonRequest()
{

}

MsgGameCommonRequest::~MsgGameCommonRequest()
{
}

void MsgGameCommonRequest::enterGame(GameRoom* room,std::string &data)
{
	if(room)
	{
		//DataCenter::getInstance()->setGameRoom(room);
		WWDataManager::getInstance()->setGameRoom(room);
		WWNetWorkManager::getInstance()->SendPrepare();
		SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

		pClientSocket->WriteChar(6);
		pClientSocket->WriteChar(1);
		pClientSocket->WriteChar(4);

		pClientSocket->WriteInt(room->getGameId());
		log("enterGame GameID=%d",room->getGameId());

		pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));

		pClientSocket->WriteInt(room->getGZID()); 
		log("enterGame GameZoneID=%d",room->getGZID());
		pClientSocket->WriteChar(0);
		//pClientSocket->WriteString(data.c_str());

		//pClientSocket->WriteString("");
		////pClientSocket->WriteShort(0);

		pClientSocket->WriteString(data.c_str());

		pClientSocket->SendCommand();
	}
}

void MsgGameCommonRequest::sendLeaveGame()
{
	log("MsgGameCommonRequest::sendLeaveGame---------------");
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	
	pClientSocket->WriteChar(6);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(2);
	//test code 

	int gameId = GameGlobalDataManager::getInstance()->getCurGameId();

	pClientSocket->WriteInt(gameId);//(int4)游戏标示gameID
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//userID    -----------test code ------------
	//test code  end

	std::string strGameId = StringUtils::format("%d", gameId);
	pClientSocket->WriteString(strGameId.c_str());
	pClientSocket->WriteChar(0);

	pClientSocket->SendCommand();

}

void MsgGameCommonRequest::sendSubstituteRequest(int type)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(6);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(15);

	int gameID = GameGlobalDataManager::getInstance()->getCurGameId();
	int gameZoneID = GameGlobalDataManager::getInstance()->getPlayRoomId();
	int gamePlayID = GameGlobalDataManager::getInstance()->getCurPlayId();

	//log("xxxxxxxxxx--- gameID= %d, gameZoneID = %d, gamePlayID = %d",gameID,gameZoneID,gamePlayID);
	//test code
	pClientSocket->WriteInt(gameID);//(int4)游戏标示 gameID
	pClientSocket->WriteInt(gameZoneID);//(int4)游戏区ID gameZoneID
	pClientSocket->WriteInt(gamePlayID);//(int4)对局ID gamePlayID
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//(int4)用户ID
	//test code  end

	pClientSocket->WriteChar(type);//(int1)0 托管、1 取消托管

	pClientSocket->SendCommand();

}

void MsgGameCommonRequest::sendChatRequest(std::string &nickName,int speakStyle,int receiverID,int waFont,std::string &content)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(6);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(14);

	//test code
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurGameId());//(int4)游戏标示 gameID
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//(int4)用户ID
	//test code  end

	pClientSocket->WriteChar(speakStyle);//(int1)聊天内容类型:  0:文字聊天  1:语音聊天  2:视频聊天
	pClientSocket->WriteString(content.c_str());//(String)聊天内容
	pClientSocket->WriteString(nickName.c_str());//(String)昵称
	pClientSocket->WriteInt(receiverID);//(int4)私语对象ID    0-整个桌子广播
	pClientSocket->WriteChar(waFont);//1-简体，2-繁体，11-火星体

	pClientSocket->SendCommand();

}

void MsgGameCommonRequest::sendPropRequest(int propId,int price,int count,int userId)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(17);
	pClientSocket->WriteChar(3);
	pClientSocket->WriteChar(1);

	pClientSocket->WriteInt(propId);
	pClientSocket->WriteInt(price);
	pClientSocket->WriteShort(count);
	pClientSocket->WriteInt(userId);

	//test code start
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurGameId());//GameID
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurPlayId());//gamePlayId
	//test code  end

	pClientSocket->WriteString("");
	pClientSocket->WriteChar(1);

	pClientSocket->SendCommand();
}

 void MsgGameCommonRequest::sendContinueGame()
 {
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(6);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(11);

	//test code 
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurGameId());//gameID
	CCLOG("MsgGameCommonRequest::sendContinueGame------------gameID= %d -",GameGlobalDataManager::getInstance()->getCurGameId());
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//UserID
	//test code  end

	pClientSocket->WriteString("");
	//不需要赠豆
	pClientSocket->WriteString("no");

	pClientSocket->SendCommand();

 }

 void MsgGameCommonRequest::sendQuickStartGame()
 {
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	
	pClientSocket->WriteChar(6);
	pClientSocket->WriteChar(1);
	pClientSocket->WriteChar(6);

	//
	pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//test code  UserID
	pClientSocket->WriteInt(0);
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getCurGameId());//test code gameID
	pClientSocket->WriteInt(GameGlobalDataManager::getInstance()->getPlayRoomId());// test code --- gameZoneID
	//

	pClientSocket->WriteString("");
	 //不需要赠豆
	pClientSocket->WriteString("no");

	 pClientSocket->SendCommand();

 }

 //void MsgGameCommonRequest::sendEnterGameZone()
 //{
	// WWNetWorkManager::getInstance()->SendPrepare();
	//SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	//pClientSocket->WriteChar(6);
	//pClientSocket->WriteChar(1);
	//pClientSocket->WriteChar(1);

	////
	//pClientSocket->WriteInt(1001);//test code  UserID
	//pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//test code  UserID
	////pClientSocket->WriteInt(UserData::getInstance()->getIntValue(INT_USER_ID));//test code  UserID
	//pClientSocket->WriteInt(5);
	//pClientSocket->WriteString("");
	//pClientSocket->WriteShort(2);// (int2)游戏玩法  0-所有玩法    ----------需根据具体玩法填写具体玩法
	//pClientSocket->WriteShort(0);//(int2)1015-大陆美女版适配信息
	//pClientSocket->WriteChar(5);//(int1)游戏输赢类型 5－蛙豆，2－蛙币,6-银子
	////

	//pClientSocket->SendCommand();
 //}



/*
 * MsgLobbyRequestFilter.cpp
 *	功能描述：返回房间列表
 *  Created on: 2015年4月27日 下午2:09:51
 *      Author: diyal.yin
 */

#include "MsgLobbyRequestFilter.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "cocos-ext.h"
#include "Model/WWDataModel/WWGD/GD.h"
#include "Control/WWDataManager/WWDataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

const int filters[] ={ 0x060124 };
const int rootFilters[] ={ MSG_ENTER_HALL_GEN };


MsgLobbyRequestFilter::MsgLobbyRequestFilter()
{

}

MsgLobbyRequestFilter::~MsgLobbyRequestFilter()
{

}

const std::vector<int> vfilters(filters, filters + sizeof(filters) / sizeof(int));

std::vector<int> MsgLobbyRequestFilter::onFilters()
{
	return vfilters;
}

const std::vector<int> rfilters(rootFilters, rootFilters + sizeof(rootFilters) / sizeof(int));

std::vector<int> MsgLobbyRequestFilter::onRootFilters()
{
	return rfilters;
}


bool MsgLobbyRequestFilter::onMessageReceived(int msgType)
{
	log("/* MsgModule-> had rec Lobby data */");
	Ref* dataObj = nullptr;
	switch(msgType){
	case 0x060124: //大厅
		dataObj = parseRoomListData();
		break;
	default:
		return false;
	}
	onDataReceiver(msgType, dataObj);
	return true;
}

bool MsgLobbyRequestFilter::onRootMessageReceived(int msgType, int userId, int result, std::string reason, int reasonType)
{

	//CommonMessageData* data = CommonMessageData::create();
	//data->setUserId(userId);
	//data->setResult(result);
	//data->setReasonType(reasonType);
	//data->setReason(reason);

	//switch (msgType)
	//{
	//case MSG_ENTER_HALL_GEN:
	//	break;

	//default:
	//	return false;
	//}

	//onDataReceiver(msgType, data);
	return true;
}

cocos2d::Ref* MsgLobbyRequestFilter::parseRoomListData()
{
	CCLOG("MsgLobbyRequestFilter::parseRoomListData");
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	//int count = pClientSocket->ReadUnsignedShort(); //游戏数量
	//int playType = 0;
	//for (int i = 0; i < count; i++)
	//{
	//	int gameId = pClientSocket->ReadInt(); // (int4)游戏编码
	//	int gameAcount = pClientSocket->ReadInt(); // (int4)游戏人数
	//	int gzCount = pClientSocket->ReadUnsignedShort(); // (int2)游戏区数量
	//	if (gzCount == 0)
	//	{
	//		return nullptr;
	//	}
	//	cocos2d::Vector<GameRoom *> arrays;
	//	for (int m = 0; m < gzCount; m++)
	//	{
	//		int gzId = pClientSocket->ReadInt(); // (int4)游戏区1ＩＤ
	//		std::string gameName = pClientSocket->ReadString(); // (String)游戏区1名称
	//		int zoneWin = pClientSocket->ReadChar(); // (int1)标识游戏区是否是财富游戏区，1：普通游戏区 2：钻石区,5 金币区
	//		int status = pClientSocket->ReadChar();					// (byte)游戏区1状态
	//		std::string des = pClientSocket->ReadString();// (String)游戏区1描述
	//		int acount = pClientSocket->ReadUnsignedShort();//(int2)游戏区人数
	//		int playerCount = pClientSocket->ReadUnsignedShort();//(int2)游戏区每局人数
	//		playType = pClientSocket->ReadUnsignedShort();	//(int2)游戏玩法

	//		//----------

	//		int multiple = pClientSocket->ReadUnsignedShort();	//(int2)倍分--------------------
	//		int enterFortune = pClientSocket->ReadInt();//(int4)带入值----------------



	//		int zoneType = pClientSocket->ReadUnsignedShort();				// (int2)分区类型 1：普通分区 2：比赛/活动区 3：VIP专区
	//		int invite = pClientSocket->ReadChar();					// (int1)是否可以约战

	//		int fortuneBase = pClientSocket->ReadInt();				// (int4)财富计算的底数
	//		int pointBase = pClientSocket->ReadInt();				// (int4)积分计算的底数

	//		int LevelMinLimit = pClientSocket->ReadInt();				//(int4)等级下限-------------------
	//		int LevelMaxLimit = pClientSocket->ReadInt();				//(int4)等级上限-------------------

	//		int PresentBean = pClientSocket->ReadChar();			//(int1)是否赠豆，0不赠，1赠送
	//		int PlayedCount = pClientSocket->ReadUnsignedShort();	//(int2)当天当前局数
	//		int TableFee = pClientSocket->ReadInt();					//(int4)桌费
	//		//int BeanVipTax = pClientSocket->ReadInt();				//(int4)蛙豆房VIP系统收税
	//		int RecordCardFalg = pClientSocket->ReadChar();			//(int1)记牌器标志------------------
	//		int AntiCheatFlag = pClientSocket->ReadChar();			//(int1)防作弊标志---------------------
	//		log("parseRoomData::gameId=%d,gzId= %d,zoneWin= %d,des= %s,PeopleNumber= %d,playType= %d,multiple=%d,enterFortune= %d,""invite= %d,gameName= %s,zoneType= %d,fortuneBase= %d,pointBase= %d,LevelMinLimit=%d,LevelMaxLimit=%d,PresentBean=%d,PlayedCount=%d,TableFee=%d,RecordCardFalg=%d,AntiCheatFlag=%d"
	//			, gameId, gzId, zoneWin, des.c_str(), acount, playType, multiple, enterFortune, invite, gameName.c_str(), zoneType, fortuneBase, pointBase,LevelMinLimit,LevelMaxLimit,PresentBean,PlayedCount,TableFee,RecordCardFalg,AntiCheatFlag);

	//		GameRoom* room = new GameRoom();
	//		room->setGameId(gameId);
	//		room->setGZID(gzId);
	//		room->setName(gameName);
	//		room->setZoneWin(zoneWin);
	//		room->setStatus(status);
	//		room->setDescription(des);
	//		room->setAcount(acount);
	//		room->setPlayerCount(playerCount);
	//		room->setPlayType(playType);
	//		room->setMultiple(multiple);
	//		room->setEnterFortune(enterFortune);
	//		room->setZoneType(zoneType);
	//		room->setInvite(invite);
	//		room->setFortuneBase(fortuneBase);
	//		room->setLevelMinLimit(LevelMinLimit);
	//		room->setLevelMaxLimit(LevelMaxLimit);
	//		room->setPresentBean(PresentBean);
	//		room->setPlayedCount(PlayedCount);
	//		room->setTableFee(TableFee);
	//		room->setRecordCardFalg(RecordCardFalg);
	//		room->setAntiCheatFlag(AntiCheatFlag);

	//		arrays.pushBack(room);
	//	}
	//	WWDataManager::getGameData()->getZone()->setRooms(playType, arrays);
	//}

	return nullptr;
}


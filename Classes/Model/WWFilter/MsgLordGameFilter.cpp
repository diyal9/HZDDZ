/*
 * MsgGameFilter.cpp
 *	功能描述：
 *  Created on: 2014年6月27日 上午10:01:18
 *      Author: hp
 */

#include "MsgLordGameFilter.h"

#include "Model/WWDataModel/CommonMessageData/CommonMessageData.h"
#include "Model/WWDataModel/WWGameData/LordGameData.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Model/WWDataModel/WWGameData/GameStartData.h"
#include "Model/WWDataModel/WWGameData/GameCommonData.h"
#include "Control/WWDataManager/WWDataManager.h"
#include "BaseCore/WWBLog/LogUtil.h"

USING_NS_CC;

const int filters[] = { MSG_GAME_START, MSG_GAME_PLAY_CARD, MSG_GAME_CALL_LORD, MSG_GAME_GRAP_LORD, MSG_GAME_MING_PAI, MSG_GAME_GAMEOVER, MSG_GAME_COMMON_GAMEOVER,
    MSG_GAME_SUBSTITUTE, MSG_GAME_CHAT, MSG_PROP_USE_INFOR, MSG_GAME_DOUBLE_SCORE};

const int rootFilters[] ={ MSG_LEAVEGAME_BACK, MSG_REFUSE_CONTINUE_GAME};

//MsgLordGameFilter::MsgLordGameFilter():isFapaiFished(false), isTimeToPlayCard(false)
//{
//}

MsgLordGameFilter::MsgLordGameFilter()
{
}

MsgLordGameFilter::~MsgLordGameFilter()
{
}

const std::vector<int> vfilters(filters, filters + sizeof(filters) / sizeof(int));

std::vector<int> MsgLordGameFilter::onFilters()
{
	return vfilters;
}

const std::vector<int> rfilters(rootFilters, rootFilters + sizeof(rootFilters) / sizeof(int));

std::vector<int> MsgLordGameFilter::onRootFilters()
{
	return rfilters;
}

bool MsgLordGameFilter::onMessageReceived(int msgType)
{
	LogUtil::getInstance()->writeLog(LEVEL_TYPE::LOG_NET,"MsgLordGameFilter::onMessageReceived","msgType =  %x", msgType);
	Ref* objData=nullptr;
	switch (msgType)
	{
        case MSG_GAME_START://游戏开局
            objData = parseStartGame();
            break;
        case MSG_GAME_CALL_LORD://叫地主
            objData = parseCallLord();
            break;
        case MSG_GAME_GRAP_LORD://抢地主
            objData = parseGrabLord();
            break;
        case MSG_GAME_MING_PAI://明牌
            objData = parseMingPai();
            break;
        case MSG_GAME_PLAY_CARD://出牌
            objData = parsePlayCard();
            break;
        case MSG_GAME_GAMEOVER://对局结算
            objData = parseGameOver();
            break;
        case MSG_GAME_COMMON_GAMEOVER: //通用結算
            objData = parseCommonGameOver();
            break;
        case MSG_GAME_SUBSTITUTE://托管
            objData = parseGameSubstitute();
            break;
        case MSG_GAME_CHAT://聊天
            objData = parseGameChat();
            break;
        case MSG_PROP_USE_INFOR://道具使用
            objData = parsePropUseToInfo();
            break;
		case MSG_GAME_DOUBLE_SCORE://加倍
			objData = parseJiaBei();
			break;
        default:
            return false;
            
	}
	//if(!isFapaiFished && (msgType == MSG_GAME_CALL_LORD || msgType == MSG_GAME_GRAP_LORD))
	//{
 //       if (msgType == MSG_GAME_CALL_LORD) {
 //           callMessageCache.pushBack(objData);
 //       }
 //       else {
 //           grapMessageCache.pushBack(objData);
 //       }
	//}
	//else if ((wwRoomType)DataCenter::getInstance()->getGameRoom()->getPlayType() == kLaizi_room && !isTimeToPlayCard && msgType == MSG_GAME_PLAY_CARD)
	//{
 //       playMessageCache.pushBack(objData);
	//}
	//else {
	//	onDataReceiver(msgType, objData);
	//}

	onDataReceiver(msgType, objData);

	return true;
}

bool MsgLordGameFilter::onRootMessageReceived(int msgType, int userId, int result, std::string reason, int reasonType)
{
	LogUtil::getInstance()->writeLog(LEVEL_TYPE::LOG_NET, "MsgGameFilter::onRootMessageReceived", "MsgGameFilter::onRootMessageReceived:msgType= %x,userId=%d,result=%d,reason=%s,reasonType=%x", msgType, userId, result, reason.c_str(), reasonType);

	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
    
	CommonMessageData* data = CommonMessageData::create();
	data->setUserId(userId);
	data->setResult(result);
	data->setReasonType(reasonType);
	data->setReason(reason);
    
	switch (msgType)
	{
        case MSG_LEAVEGAME_BACK:
            break;
        case MSG_REFUSE_CONTINUE_GAME:
            break;
        default:
            return false;
	}
    
	onDataReceiver(msgType,data);
	return true;
    
}

Ref* MsgLordGameFilter::parseStartGame()
{
	LogUtil::getInstance()->writeLog("MsgLordGameFilter::parseStartGameNew");
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	GameStartData* data = GameStartData::create();
    data->setResume(false);
	data->setGameId(pClientSocket->ReadInt());//(int4)游戏ＩＤ
	data->setGamePlayId(pClientSocket->ReadInt());//(int4)对局ID
	data->setPlayTimeOut(pClientSocket->ReadUnsignedShort());//(int2)单手牌超时时间（单位：秒）
	int DoubleTime = pClientSocket->ReadShort();//(int2)加倍时间（单位：秒）
	data->setDoubleTime(DoubleTime);
	// 
	data->setFirstPlayer(pClientSocket->ReadInt());//(int4)第一个叫分用户 
	std::string datas = pClientSocket->ReadString();//(String)附加数据
	int Num4Award = pClientSocket->ReadUnsignedShort();//(int2)奖宝箱局数
	int Playnum = pClientSocket->ReadUnsignedShort();//(int2)当前财富局数
	int FortuneBase = pClientSocket->ReadInt();//(int4)财富底分
	int Laizi = pClientSocket->ReadChar();//(int1)赖子: 有效值1-13,依次表示345678910JQKA2

	CCLOG("MsgLordGameFilter::parseStartGame--------datas=%s, Num4Award=%d, Playnum=%d, FortuneBase=%d, Laizi=%d",datas.c_str(),Num4Award,Playnum,FortuneBase,Laizi);
	data->setTaskNum4Award(Num4Award);
	data->setPlayedNum4Award(Playnum);
	data->setFortuneBase(FortuneBase);
	data->setLaizi(Laizi);
	//
	data->setPlayerCount(pClientSocket->ReadChar());//(int1)游戏人数
	int playerCount = data->getPlayerCount();
    
	data->getPlayerIds().resize(playerCount);
	data->getPlayerNickNames().resize(playerCount);
	data->getPlayerIcons().resize(playerCount);
	//data->getGameLevels().resize(playerCount);
	data->getPlayerVips().resize(playerCount);
	//data->getTeamLevels().resize(playerCount);
	data->getPlayerPokers().resize(playerCount);
	data->getPlayerCashs().resize(playerCount);
	//data->getTeamLeader().resize(playerCount);
	//data->getTongLeader().resize(playerCount);
	//data->getPropDoublePower().resize(playerCount);
	//data->getRegion().resize(playerCount);
	data->getJiFenLevel().resize(playerCount);
	data->getGenders().resize(playerCount);
	data->getOnlineTypes().resize(playerCount);
    
	CCLOG("MsgLordGameFilter::parseStartGame--------gameid=%d,gamePlayId=%d,timeOut=%d,DoubleTime=%d,playerCount=%d,firstPlayer=%d", data->getGameId(), data->getGamePlayId(), data->getPlayTimeOut(), DoubleTime, playerCount, data->getFirstPlayer());
	for (int j = 0; j < playerCount; j++)
	{
		data->getPlayerIds()[j]=(pClientSocket->ReadInt());//(int4)用户ＩＤ
		data->getPlayerNickNames()[j]=(pClientSocket->ReadString());//(String)用户名称
		data->getPlayerIcons()[j]=(pClientSocket->ReadInt());//(int4)用户头像
		data->getPlayerVips()[j]=(pClientSocket->ReadChar());//(int2)用户VIP等级
		data->getPlayerPokers()[j]=(pClientSocket->ReadString());//(byte[])用户的牌
		data->getPlayerCashs()[j]=(pClientSocket->ReadInt());//(int4)用户财富值
		data->getJiFenLevel()[j]=(pClientSocket->ReadUnsignedShort());//(int2)游戏积分等级
		data->getGenders()[j] = pClientSocket->ReadChar();//(int1)性别
		data->getOnlineTypes()[j] = pClientSocket->ReadString();//(String)网络接入

		log("MsgLordGameFilter::parseStartGame-----index = %d---, UserID=%d, UserName=%s, Icon=%d, VIPGrade= %d, Card=%s, Fortune=%d, GameLevel=%d, Gender=%d, APN=%s "
			,j,data->getPlayerIds()[j], data->getPlayerNickNames()[j].c_str(), data->getPlayerIcons()[j], data->getPlayerVips()[j], data->getPlayerPokers()[j].c_str(), data->getPlayerCashs()[j], data->getJiFenLevel()[j], data->getGenders()[j], data->getOnlineTypes()[j].c_str());
	}
    
	log("MsgLordGameFilter::parseStartGame -------recv start game Msg");
	WWDataManager::getInstance()->setGameStartData(data);

	// just for test.
	data->retain();
	return data;
}

Ref* MsgLordGameFilter::parseCallLord()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	CallLordData* data = CallLordData::create();
	data->setGamePlayId(pClientSocket->ReadInt()) ;
	data->setUserIdJiaoFen( pClientSocket->ReadInt());
	data->setJiaoFlag(pClientSocket->ReadChar());
	data->setNextPlayer(pClientSocket->ReadInt());
	std::string pokers = pClientSocket->ReadString();

	int nextAction = pClientSocket->ReadChar();//（int1）1:叫地主   2：抢地主  3加倍  4明牌  5出牌
	data->setNextAction(nextAction);

	CCLOG("MsgLordGameFilter::parseCallLord-----nextAction = %d, userID = %d",nextAction,data->getUserIdJiaoFen());
	Vector<PokerObject*> arrays =PokerUtil::getSeverData((uint8_t*)pokers.c_str(),pokers.size());
    
	for (auto obj : arrays)
	{
		if(obj)
		{
			data->getBottomPokers().pushBack(obj);
		}
	}
	return data;
}

Ref* MsgLordGameFilter::parseGrabLord()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	GrabLordData* data = GrabLordData::create();
    
	data->setGamePlayId(pClientSocket->ReadInt()) ;
	data->setUserIdQiangDiZhu( pClientSocket->ReadInt());
	data->setIsGrab(pClientSocket->ReadChar());
	data->setIsQiangDiZhuOver(pClientSocket->ReadChar());//(int1) 2:抢地主  3加倍  4地主明牌  5出牌
	data->setNextPlayer(pClientSocket->ReadInt());
	std::string bottomPokers = pClientSocket->ReadString();

	CCLOG("MsgLordGameFilter::parseGrabLord-----IsQiangDiZhuOver = %d, userID = %d",data->getIsQiangDiZhuOver(),data->getUserIdQiangDiZhu());
	Vector<PokerObject*> arrays =PokerUtil::getSeverData((uint8_t*)bottomPokers.c_str(),bottomPokers.size());
	for (auto obj : arrays)
	{
		if(obj)
		{
			data->getBottomPokers().pushBack(obj);
		}
	}
	return data;
}

Ref* MsgLordGameFilter::parseMingPai()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	
	MingPaiData* data = MingPaiData::create();
	data->setGamePlayId(pClientSocket->ReadInt());
	data->setUserID(pClientSocket->ReadInt());
	std::string playerCards = pClientSocket->ReadString();
	Vector<PokerObject*> arrays =PokerUtil::getSeverData((uint8_t*)playerCards.c_str(),playerCards.size());
    
	for (auto obj : arrays)
	{
		if(obj)
		{
			data->getPlayerPokers().pushBack(obj);
		}
	}
    
	return data;
}

Ref* MsgLordGameFilter::parseJiaBei()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	JiaBeiData* data = JiaBeiData::create();
	int GamePlayID = pClientSocket->ReadInt();//(int4)对局标示 
	int userID = pClientSocket->ReadInt();//(int4)用户ＩＤ 
	int flag = pClientSocket->ReadChar();//(int1)用户是否抢地主  0:不加倍 1:加倍  2：加强加倍  3：加倍结束
	int NextAction = pClientSocket->ReadChar();//(int1)2叫牌 当flag=3时  3地主明牌  4出牌
	int NextPlay = pClientSocket->ReadInt();//(int4)flag=3加倍结束，为下一个明牌或出牌用户ＩＤ 

	CCLOG("MsgLordGameFilter::parseJiaBei----GamePlayID=%d, userID=%d, flag=%d, NextAction=%d, NextPlay=%d",
		GamePlayID,userID,flag,NextAction,NextPlay);

	data->setGamePlayId(GamePlayID);
	data->setUserID(userID);
	data->setFlag(flag);
	data->setNextAction(NextAction);
	data->setNextPlay(NextPlay);

	return data;
}

Ref* MsgLordGameFilter::parsePlayCard()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	
	PlayCardData* data = PlayCardData::create();
	data->setGamePlayId(pClientSocket->ReadInt());
	data->setUserID(pClientSocket->ReadInt());
	std::string playCards = pClientSocket->ReadString();
	Vector<PokerObject*> arrays =PokerUtil::getSeverData((uint8_t*)playCards.c_str(),playCards.size());
	for (auto obj : arrays)
	{
		if(obj)
		{
			data->getPlayPokers().pushBack(obj);
		}
	}
	data->setNextPlayer(pClientSocket->ReadInt());
	std::string replaceCards = pClientSocket->ReadString();
	Vector<PokerObject*> arrays2 =PokerUtil::getSeverData((uint8_t*)replaceCards.c_str(),replaceCards.size());
	for (auto obj : arrays2)
	{
		if(obj)
		{
			data->getReplacePokers().pushBack(obj);
		}
	}
	data->setType(pClientSocket->ReadChar());
	data->setValue(pClientSocket->ReadChar());
	return data;
    
}

Ref*  MsgLordGameFilter::parseGameOver()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	LordGameOverData* data = LordGameOverData::create();
	data->setGamePlayId(pClientSocket->ReadInt());//(int4)对局标示 
	data->setWinnerUserID(pClientSocket->ReadInt());//(int4)赢家用户ＩＤ 

	//
	data->setUnit(pClientSocket->ReadString());//(String)单位
	log("parseGameOver:Unit=%s",data->getUnit().c_str());
	data->setData(pClientSocket->ReadString());//(String)附加数据
	log("parseGameOver:Data=%s",data->getData().c_str());
	data->setZoneWin(pClientSocket->ReadChar());//(int1)输赢类型:1:战斗力 2:财富 5.金豆
	log("parseGameOver:ZoneWin=%d",data->getZoneWin());
	data->setMultiple(pClientSocket->ReadUnsignedShort());//(int2)输赢倍数,由游戏区定义
	log("parseGameOver:Multiple=%d",data->getMultiple());
	data->setTime(pClientSocket->ReadString());//(String)游戏时长
	log("parseGameOver:Time=%s",data->getTime().c_str());
	data->setBankUserID(pClientSocket->ReadInt());//(int4)地主UserID
	log("parseGameOver:BankUserID=%d",data->getBankUserID());
	data->setBankScore(pClientSocket->ReadChar());//(int1)地主叫分
	log("parseGameOver:BankScore=%d",data->getBankScore());
	data->setBoomCount(pClientSocket->ReadChar());//(int1)炸弹数量
	log("parseGameOver:BoomCount=%d",data->getBoomCount());
	data->setFan(pClientSocket->ReadUnsignedShort());//(int2)游戏番数
	log("parseGameOver:Fan=%d",data->getFan());
	data->setFortuneBase(pClientSocket->ReadInt());//(int4)财富底分
	log("parseGameOver:FortuneBase=%d",data->getFortuneBase());
	data->setBigBoomCount(pClientSocket->ReadChar());//(int1)炸弹数量2
	log("parseGameOver:BigBoomCount=%d",data->getBigBoomCount());
	data->setIsChunTian(pClientSocket->ReadChar());//(int1)是否春天 0：否 1：是
	CCLOG("parseGameOver:IsChunTian=%d",data->getIsChunTian());

	//add end
	int playerCount = pClientSocket->ReadChar();
	data->setCount(playerCount);

	data->getUserIDs().resize(playerCount);
	data->getScores().resize(playerCount);
	data->getTScores().resize(playerCount);
	data->getLevels().resize(playerCount);
	data->getCard().resize(playerCount);
    
	data->getDLWLs().resize(playerCount);
	data->getFans().resize(playerCount);
	data->getGamePoints().resize(playerCount);
	data->getTGamePoints().resize(playerCount);
	data->getGameLevelNames().resize(playerCount);

	data->getContinueWinNums().resize(playerCount);
	data->getTopWinNums().resize(playerCount);
    
	data->getAllPlayCounts().resize(playerCount);
	data->getNeededPlayCounts().resize(playerCount);
	data->getLuckyPoints().resize(playerCount);
	data->getGreatAwards().resize(playerCount);

	std::vector<std::string> remainPokerStr;
	remainPokerStr.resize(playerCount);

	for (int i= 0; i < playerCount; ++i)
	{
		int goverCurUserID = pClientSocket->ReadInt();//(int4)用户ＩＤ 
		log("parseGameOver:goverCurUserID=%d",goverCurUserID);
		//int pos = GameContext::getInstance()->getPlayerIndex(goverCurUserID);//getPlayerIndex待修改
		int pos = i;//测试代码   -------该处， 需要对比UserID来确定是哪家的数据
		log("parseGameOver:pos=%d",pos);
		data->getUserIDs()[pos]=goverCurUserID;
		log("parseGameOver:UserID=%d",data->getUserIDs()[pos]);

		data->getScores()[pos]=(pClientSocket->ReadInt());//(int4)用户本局财富变化/战力/筹码 
		log("parseGameOver:Score=%d",data->getScores()[pos]);
		data->getTScores()[pos]=(pClientSocket->ReadInt());//(int4)用户本局后总财富 /战力/筹码
        
		log("parseGameOver:TScore=%d",data->getTScores()[pos]);
		data->getLevels()[pos]=(pClientSocket->ReadShort());//(int2)用户本局后级别
		log("parseGameOver:Level=%d",data->getLevels()[pos]);
		remainPokerStr[pos]=(pClientSocket->ReadString());//(byte[])用户剩余在手牌
		log("parseGameOver:remainPokerStr=%s",remainPokerStr[pos].c_str());
		data->getCard()[pos] = remainPokerStr[pos];
        
		//data->getMagics()[pos]=(pClientSocket->ReadChar());
		//log("parseGameOver:Magic=%d",data->getMagics()[pos]);
		data->getDLWLs()[pos]=(pClientSocket->ReadChar());//(int1)倒拉窝垒标志,低4位表示
		log("parseGameOver:DLWL=%d",data->getDLWLs()[pos]);
		data->getFans()[pos]=(pClientSocket->ReadUnsignedShort());//(int2)倍数
		log("parseGameOver:Fan=%d",data->getFans()[pos]);
		data->getGamePoints()[pos]=(pClientSocket->ReadInt());//(int4)用户1本局游戏积分变化
		log("parseGameOver:GamePoint=%d",data->getGamePoints()[pos]);
		data->getTGamePoints()[pos]=(pClientSocket->ReadInt());//(int4)用户1本局后游戏总积分
		log("parseGameOver:TGamePoint=%d",data->getTGamePoints()[pos]);
		data->getGameLevelNames()[pos]=(pClientSocket->ReadString());//(String)用户1本局后游戏积分等级称号
		log("parseGameOver:GameLevelName=%s",data->getGameLevelNames()[pos].c_str());
        
		//add
		data->getAllPlayCounts()[i]=(pClientSocket->ReadUnsignedShort());//(int2)当天本游戏区总局数
		data->getNeededPlayCounts()[i]=(pClientSocket->ReadUnsignedShort());//(int2)获取金宝箱需要对局数
		data->getLuckyPoints()[i]=(pClientSocket->ReadInt());//(int4)幸运值
		data->getGreatAwards()[i]=(pClientSocket->ReadInt());//(int4)最高奖蛙币

		data->getContinueWinNums()[i] = (pClientSocket->ReadUnsignedShort());//(int2)连胜数
		data->getTopWinNums()[i] = (pClientSocket->ReadUnsignedShort());//(int2)历史最高连胜数
	}
    
	return data;
}

Ref* MsgLordGameFilter::parseCommonGameOver()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	
	GameOverCommonData* data = GameOverCommonData::create();
    
	data->setGameId(pClientSocket->ReadInt());//(int4)游戏ID
	data->setGamePlayId(pClientSocket->ReadInt());//(int4)对局ID
	data->setBeanTax(pClientSocket->ReadInt());//(int4)蛙豆房系统收税
	int playerCount = pClientSocket->ReadUnsignedShort();//(int2)玩家数量
	data->setPlayerCount(playerCount);
	CCLOG("MsgLordGameFilter::parseCommonGameOverNew---------gameid=%d,gamePlayID=%d,playerCount=%d",data->getGameId(),data->getGamePlayId(),playerCount);
	data->getUserIds().resize(playerCount);
	data->getFortunes().resize(playerCount);
	data->getTFortunes().resize(playerCount);
	//data->getScores().resize(playerCount);
	//data->getTScores().resize(playerCount);
	data->getChips().resize(playerCount);
	data->getTChips().resize(playerCount);
	data->getGamePoints().resize(playerCount);
	data->getTGamePoints().resize(playerCount);
	data->getGamePointLevels().resize(playerCount);
	//data->getMagics().resize(playerCount);
	data->getPlayResults().resize(playerCount);
	//data->getOriFortunes().resize(playerCount);
	//std::vector<int> curUserIDs;
	//curUserIDs.resize(playerCount);
	for (int i = 0; i < playerCount; i++)
	{
		int goverCurUserID = pClientSocket->ReadInt();//(int4)用户1ＩＤ 
		CCLOG("MsgLordGameFilter::parseCommonGameOver----goverCurUserID=%d",goverCurUserID);
		
		//curUserIDs[i]=goverCurUserID;
		//int pos =GameContext::getInstance()->getPlayerIndex(goverCurUserID);
		int pos = i; //-----------------------测试代码  ,需根据开局消息中userId，来确定数据的具体index
		data->getUserIds()[pos]=goverCurUserID;

		std::string Fortune = pClientSocket->ReadString();//(String)用户本局财富变化 
		data->getFortunes()[pos] = Fortune;
		CCLOG("MsgLordGameFilter::parseCommonGameOver----Fortune=%s",Fortune.c_str());

		std::string TFortune = pClientSocket->ReadString();//(String)用户本局后总财富 
		data->getTFortunes()[pos] = TFortune;
		CCLOG("MsgLordGameFilter::parseCommonGameOver----TFortune=%s",TFortune.c_str());

		data->getChips()[pos]=pClientSocket->ReadInt();//(int4)用户本局临时积分变化 
		CCLOG("MsgLordGameFilter::parseCommonGameOver----Chips=%d",data->getChips()[pos]);

		data->getTChips()[pos]=pClientSocket->ReadInt();//TChip(n)	(int4)用户本局后临时积分
		CCLOG("MsgLordGameFilter::parseCommonGameOver----TChips=%d",data->getTChips()[pos]);

		data->getGamePoints()[pos]=pClientSocket->ReadInt();//(int4)用户本局游戏积分变化
		CCLOG("MsgLordGameFilter::parseCommonGameOver----GamePoints=%d",data->getGamePoints()[pos]);

		data->getTGamePoints()[pos]=pClientSocket->ReadInt();//(int4)用户本局后游戏总积分
		CCLOG("MsgLordGameFilter::parseCommonGameOver----TGamePoints=%d",data->getTGamePoints()[pos]);

		data->getGamePointLevels()[pos]=pClientSocket->ReadUnsignedShort();//(int2)积分级别
		CCLOG("MsgLordGameFilter::parseCommonGameOver----GamePointLevels=%d",data->getGamePointLevels()[pos]);

		data->getPlayResults()[pos]=pClientSocket->ReadUnsignedShort();//(int2)1赢、2输、3和
		CCLOG("MsgLordGameFilter::parseCommonGameOver----PlayResults=%d",data->getPlayResults()[pos]);
	}
    
	//data->getNickNames().resize(playerCount);
	/*cocos2d::Vector<Player *> players = GameContext::getInstance()->getPlayers();
	for (int i = 0; i < players.size(); ++i)
	{
		data->getNickNames()[i] = players.at(i)->getNickName();
	}
	GameContext::getInstance()->setGameOverCommonData(data);*/
	return data;
}

Ref* MsgLordGameFilter:: parseGameSubstitute()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	GameSubstituteData* data = GameSubstituteData::create();
	data->setGameId(pClientSocket->ReadInt());
	data->setGameZoneID(pClientSocket->ReadInt());
	data->setGameplayID(pClientSocket->ReadInt());
	data->setUserID(pClientSocket->ReadInt());
	data->setType(pClientSocket->ReadChar());
	return data;
}

Ref* MsgLordGameFilter::parseGameChat()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	GameChatData* data = GameChatData::create();
	data->setGamePlayID(pClientSocket->ReadInt());
	data->setFromUserID(pClientSocket->ReadInt());
	data->setType(pClientSocket->ReadChar());
	data->setChatContent(pClientSocket->ReadString());
	data->setUserNick(pClientSocket->ReadString());
	data->setToUserID(pClientSocket->ReadInt());
	return data;
}

//void MsgLordGameFilter::fapaiFished()
//{
//	isFapaiFished = true;
//    
//    for (auto obj : callMessageCache)
//    {
//        onDataReceiver(MSG_GAME_CALL_LORD, obj);
//    }
//    for (auto obj : grapMessageCache)
//    {
//        onDataReceiver(MSG_GAME_GRAP_LORD, obj);
//    }
//    callMessageCache.clear();
//	grapMessageCache.clear();
//}
//
//void MsgLordGameFilter::beganFilterPlayCardMessage()
//{
//	isTimeToPlayCard = true;
//    
//	for (auto obj : playMessageCache)
//    {
//        onDataReceiver(MSG_GAME_PLAY_CARD, obj);
//    }
//	playMessageCache.clear();
//}

Ref* MsgLordGameFilter::parsePropUseToInfo()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	PropUseInfoData* data = PropUseInfoData::create();
    data->setGamePlayID(pClientSocket->ReadInt());
    data->setPropId(pClientSocket->ReadInt());
    data->setFromUserID(pClientSocket->ReadInt());
    data->setUseToUserID(pClientSocket->ReadInt());
    return data;
}


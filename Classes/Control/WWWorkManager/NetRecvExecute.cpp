/*
 *	author zengxin
 *	添加日期：
 *	2015年6月18日
 */


#include "NetRecvExecute.h"

#include "BaseCore/WWBLog/LogUtil.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "BaseCore/Common/MsgConstDefine.h"
#include "Control/WWWorkManager/WWHeartCheckManager.h"
#include "Model/WWDataModel/WWGameData/GameStartData.h"
#include "Control/WWDataManager/WWDataManager.h"
#include "Control/WWDataManager/GameGlobalDataManager.h"
#include "View/GameScene/GameLogicManager.h"
#include "View/Common/CommLayer/CommDialog.h"

USING_NS_CC;

NetRecvExecute::NetRecvExecute()
{
}

NetRecvExecute::~NetRecvExecute()
{
}

void NetRecvExecute::dealNetRecv()
{
	SocketClient* m_pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	long recSize = m_pClientSocket->Recv();

	if(m_pClientSocket->IsConnect())
	{
		if(recSize > 0)
		{
			WWHeartCheckManager::getInstance()->reset();
		}else
		{
			if(m_pClientSocket->IsConnect())
				WWHeartCheckManager::getInstance()->sendHeartMsg();
		}
	}

	if (recSize > 0)//有接收到消息
	{
		int readLen = 0;//已读消息长度

		bool m_isSucces = false;
		do
		{
			int n_msg_len = m_pClientSocket->ReadUnsignedShort(); //读取消息长度
			readLen += 2;

			if (n_msg_len >= 0xFFFF) //如果消息长度大于0xFFFF，则说明是超大包
			{
				n_msg_len = m_pClientSocket->ReadInt();
			}

			readLen += n_msg_len;
			//log("recv msg  a lot --------------------------- current length = %d, recSize = %d",readLen, recSize);

			int m_recvMsg_Id = (int) m_pClientSocket->ReadChar();
			WWNetWorkManager::getInstance()->setRecvMsgId(m_recvMsg_Id);
			readLen++;
			//LogUtil::getInstance()->writeLog("server recv：m_recvMsg_Id=%d", m_recvMsg_Id);
			int send_id = (int) m_pClientSocket->ReadChar();
			readLen++;
			//LogUtil::getInstance()->writeLog("server recv：send_id=%d", send_id);

			int channelID; //服务器返回连接频道标示

			int msgType = m_pClientSocket->Read3CharToInt();
			if(msgType != 0x7f7f7f)
				LogUtil::getInstance()->writeLog("-----------------------recv msg---------------------------- msgType=%x", msgType);
			switch (msgType)
			{
			case 0x7f7f7f: //心跳
				break;
			case 0x120000: //握手
				channelID = m_pClientSocket->ReadInt();
				CCLOG("channelID=%d", channelID);
				m_isSucces = m_pClientSocket->ReadBoolean();

				WWNetWorkManager::getInstance()->setIsConnectSuccess(m_isSucces);

				CCLOG("isSucess=%d", m_isSucces);

				WWNetWorkManager::getInstance()->setSendMsgId(send_id);

				break;

			default:

				do
				{
					CC_BREAK_IF(m_pClientSocket->GetReadLength() <= 0);

					switch (msgType)
					{
						//通用消息
					case 0:
						executeRootMsg();
						break;
					default:
						executeGlobleMsg(msgType);
						break;
					}

				} while (0);
				break;
			}

			if(readLen >= recSize)//读取长度大于当前消息总长度
			{
				//log("recv msg ----------------------------- already deal all msg");
				break;
			}
			else
			{
				m_pClientSocket->SkipToReadIndex(readLen);
				//log("recv msg ---------skip to index--------------index = %d ",readLen);
			}
			
		}while(true);


	}
}

void NetRecvExecute::executeGlobleMsg(int msgType)
{
	bool deal = true;
	switch (msgType)
	{
	case MSG_GAME_RESUME:
		parseResumeGame();
		break;
	case MSG_RECOVERY_GAME_NOTIY:
		parseRecoveryGameNofity();
		break;
	case MSG_OPEN_MONEY_BOX_RESP:
		parseOpenMoneyBoxResp();
		break;
	case MSG_PLAY_BOX_LIST:
		parsePlayBoxList();
		break;
		//-----------------------------------比赛消息 解析------------------------------------
	case MSG_IMatchSignupNotify:
		parseIMatchSignupNotify();
		break;
	case MSG_ImatchPlayerStatus:
		parseImatchPlayerStatus();
		break;
	case MSG_MatchGameOver:
		parseIMatchGameOver();
		break;
	case MSG_IMatchBye:
		parseIMatchBye();
		break;
	case MSG_IMatchAntilordStart:
		parseIMatchAntilordStart();
		break;
	case MSG_ImatchAntilordResume:
		parseImatchAntilordResume();
		break;
	default:
		deal = false;
		break;
	}
	if(deal)
	{
		return;
	}
	onMsgRecv(msgType);
}

void NetRecvExecute::executeRootMsg()
{
	SocketClient* m_pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	int msgType = m_pClientSocket->Read3CharToInt();//请求的消息头
	int userId = m_pClientSocket->ReadInt();
	int result = m_pClientSocket->ReadChar();
	std::string reason = m_pClientSocket->ReadString();
	int reasonType = m_pClientSocket->ReadShort();
	LogUtil::getInstance()->writeLog("root msg recv::onRootMessageReceived:msgType= %x,userId=%d,result=%d,reason=%s,reasonType=%x", msgType, userId, result, reason.c_str(), reasonType);
	if(executeGlobleRootMsg(msgType, userId, result, reason, reasonType))
	{
		return;
	}
	onRootMsgRecv(msgType, userId, result, reason, reasonType);
}

bool NetRecvExecute::executeGlobleRootMsg(int msgType, int userId, int result, std::string reason, int reasonType)
{
	bool deal = false;
	CommDialog* dialog;
	switch (msgType)
	{
	case MSG_ENTER_GAME:
		deal = true;
		if(result == 4)//// 上局未结束，提示续局
		{
			dialog = CommDialog::creatTwoBtnOneMessage("yes", "no", reason.c_str(), NULL, NULL);
			Director::getInstance()->getRunningScene()->addChild(dialog,BASIC_ZORDER_TOP);

			GameGlobalDataManager::getInstance()->setStartGameType(StartGameType::SEND_RESUME);
			GameLogicManager::getInstance()->requestBackGame();

			log("NetRecvExecute::executeGlobleRootMsg---------------------------------continue  game");
		}
		if(result == 1)//开局失败
		{
			if(reasonType == 9)//加入房间失败：房间未解锁。
			{
				dialog = CommDialog::creatOneBtnOneMessage("yes", reason.c_str());
				Director::getInstance()->getRunningScene()->addChild(dialog,BASIC_ZORDER_TOP);
			}
		}
		break;;
	case MSG_GENNAL_OTHER_LOGIN:
		deal = true;
		dialog = CommDialog::creatTwoBtnOneMessage("sure", "leave", reason.c_str(),NULL,NULL);
		Director::getInstance()->getRunningScene()->addChild(dialog);
		break;
	default:
		break;
	}
	return deal;
}

//恢复对局详细数据
void NetRecvExecute::parseResumeGame()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

    //恢复对局倍数
    int ResumeGameMuti = 1;
    
	GameStartData* data = GameStartData::create();
	data->setGameType(1);

	data->setResume(true);
	data->setGameId(WWDataManager::getInstance()->getGameRoom()->getGameId());
	data->setFortuneBase(WWDataManager::getInstance()->getGameRoom()->getFortuneBase());
	data->setGamePlayId(pClientSocket->ReadInt()); //对局标示

	GameGlobalDataManager::getInstance()->setCurGameId(data->getGameId());
	GameGlobalDataManager::getInstance()->setCurPlayId(data->getGamePlayId());
	GameGlobalDataManager::getInstance()->setPlayRoomId(WWDataManager::getInstance()->getGameRoom()->getGZID());//保存正在对局的房间ID

	CCLOG("MsgGlobalFilter::parseResumeGame----gamePlayId = %d",data->getGamePlayId());
	data->setPlayTimeOut(pClientSocket->ReadShort()); //单手牌超时时间
	CCLOG("MsgGlobalFilter::parseResumeGame----PlayTimeOut = %d",data->getPlayTimeOut());
	data->setNextPlayUserID(pClientSocket->ReadInt()); //下一出牌玩家
	CCLOG("MsgGlobalFilter::parseResumeGame----NextPlayUserID = %d",data->getNextPlayUserID());
	data->setLordId(pClientSocket->ReadInt()); //地主ID
	CCLOG("MsgGlobalFilter::parseResumeGame----LordId = %d",data->getLordId());
	std::string baseCards = pClientSocket->ReadString();//(byte[])底牌
	CCLOG("MsgGlobalFilter::parseResumeGame----baseCards = %s",baseCards.c_str());
	//Vector<PokerObject*> arrays = PokerUtil::getSeverData((uint8_t*) str.c_str(), str.size());
	data->setBasePokers(baseCards); //底牌
	data->setBankScore(pClientSocket->ReadChar()); //游戏底分
	CCLOG("MsgGlobalFilter::parseResumeGame----BankScore = %d",data->getBankScore());
	data->setGameStatus(pClientSocket->ReadChar()); //(int1)1叫地主，2抢地主，3加倍，4明牌  5打牌 6结算
	CCLOG("MsgGlobalFilter::parseResumeGame----GameStatus = %d",data->getGameStatus());
	data->setLastPlayerID(pClientSocket->ReadInt()); //最后出牌玩家
	CCLOG("MsgGlobalFilter::parseResumeGame----LastPlayerID = %d",data->getLastPlayerID());
    
	std::string lastCards = pClientSocket->ReadString();
	CCLOG("MsgGlobalFilter::parseResumeGame----lastCards = %s",lastCards.c_str());
	//arrays = PokerUtil::getSeverData((uint8_t*) str.c_str(), str.size());
	data->setLastPokers(lastCards); //最后出牌
    
	data->setBoomCount(pClientSocket->ReadChar()); //炸弹数量
	CCLOG("MsgGlobalFilter::parseResumeGame----BoomCount = %d",data->getBoomCount());
	data->setLaizi(pClientSocket->ReadChar()); //赖子: 有效值1-13,依次表示345678910JQKA2
	CCLOG("MsgGlobalFilter::parseResumeGame----Laizi = %d",data->getLaizi());

	data->setTaskNum4Award(pClientSocket->ReadShort()); //奖宝箱局数
	CCLOG("MsgGlobalFilter::parseResumeGame----TaskNum4Award = %d",data->getTaskNum4Award());
	data->setPlayedNum4Award(pClientSocket->ReadShort()); //当前财富局数
	CCLOG("MsgGlobalFilter::parseResumeGame----PlayedNum4Award = %d",data->getPlayedNum4Award());
	//data->setTaskEachNumAward(data->getTaskNum4Award());
	data->setMingPai(pClientSocket->ReadChar() == 1); //明牌标志.1-明牌,0不明牌,2未确定
	CCLOG("MsgGlobalFilter::parseResumeGame----MingPai = %d",data->getMingPai());

	//-------------------------新加数据------------记牌器-----------------
	int RecordCardFlag = pClientSocket->ReadChar();//(int1) 记牌器状态    0：未使用记牌器  1：已使用记牌器
	log("MsgGlobalFilter::parseResumeGame---RecordCardFlag=%d,",RecordCardFlag);
	std::string RecordCardData = pClientSocket->ReadString();//(byte[])记牌器数据  数据从3到大王
	log("MsgGlobalFilter::parseResumeGame---RecordCardData=%s,",RecordCardData.c_str());

	int DoubleTime = pClientSocket->ReadShort();//(int2)加倍时间（单位：秒）
	log("MsgGlobalFilter::parseResumeGame----DoubleTime=%d",DoubleTime);
	data->setDoubleTime(DoubleTime);

	data->setPlayerCount(pClientSocket->ReadChar()); //游戏人数
    CCLOG("MsgGlobalFilter::parseResumeGame----PlayerCount = %d",data->getPlayerCount());
    //明牌翻倍
    if (data->getMingPai() == 1)
    {
        ResumeGameMuti = ResumeGameMuti * 2;
    }
    //炸弹翻倍
    if (data->getBoomCount() != 0)
    {
		ResumeGameMuti = ResumeGameMuti * data->getBoomCount() * 2;
    }
    
	for (int i = 0; i < data->getPlayerCount(); ++i)
	{
		data->getPlayerIds().push_back(pClientSocket->ReadInt()); //用户ＩＤ
		CCLOG("MsgGlobalFilter::parseResumeGame----PlayerId = %d",data->getPlayerIds().at(i));
		data->getPlayerNickNames().push_back(pClientSocket->ReadString()); //用户名称
		log("MsgGlobalFilter::parseResumeGame----NickName = %s",data->getPlayerNickNames().at(i).c_str());
		data->getPlayerPokers().push_back(pClientSocket->ReadString()); //用户的牌
		log("MsgGlobalFilter::parseResumeGame----getPlayerPokers = %s",data->getPlayerPokers().at(i).c_str());
		data->getPlayerIcons().push_back(pClientSocket->ReadInt()); //用户头像
		log("MsgGlobalFilter::parseResumeGame----getPlayerIcons = %d",data->getPlayerIcons().at(i));
		data->getPlayerVips().push_back(pClientSocket->ReadChar()); //ＶＩＰ
		log("MsgGlobalFilter::parseResumeGame----getPlayerVips = %d",data->getPlayerVips().at(i));
		data->getJiFenLevel().push_back(pClientSocket->ReadShort()); //游戏积分等级
		log("MsgGlobalFilter::parseResumeGame----getGameLevels = %d",data->getJiFenLevel().at(i));
		data->getGenders().push_back(pClientSocket->ReadChar()); //性别
		log("MsgGlobalFilter::parseResumeGame----getGenders = %d",data->getGenders().at(i));
		data->getOnlineTypes().push_back(pClientSocket->ReadString()); //网络接入
		log("MsgGlobalFilter::parseResumeGame----getOnlineTypes = %s",data->getOnlineTypes().at(i).c_str());

		int getQiangDizhus = pClientSocket->ReadChar();
		data->getQiangDizhus().push_back(getQiangDizhus == 1); //抢地主标志：1－抢，0－不抢
		log("MsgGlobalFilter::parseResumeGame----getQiangDizhus = %d",getQiangDizhus);

		int doubleScore = pClientSocket->ReadChar();//(int1)加倍标志 0 未 1加倍 2 强加被
		log("MsgGlobalFilter::parseResumeGame----doubleScore = %d",doubleScore);
		data->getDoubleScoreFlag().push_back(doubleScore);

		data->getForbidTalk().push_back(pClientSocket->ReadChar() == 1); //禁言标志:1-禁言,0-不禁
		log("MsgGlobalFilter::parseResumeGame----getForbidTalk = %d",data->getForbidTalk().at(i));

		int keepWin = pClientSocket->ReadShort();//连胜数
		log("MsgGlobalFilter::parseResumeGame----keepWin = %d",keepWin);
		data->getKeepWin().push_back(keepWin);

		int CardNum = pClientSocket->ReadShort();//(int2)剩余牌数
		log("MsgGlobalFilter::parseResumeGame----CardNum = %d",CardNum);
		data->getCardNum().push_back(CardNum);
        
        //抢地主翻倍
        if (data->getQiangDizhus()[i])
        {
			CCLOG("wly::QiangDizhus (%d) muti", data->getQiangDizhus()[i]);
            ResumeGameMuti = ResumeGameMuti * 2;
        }
		if(doubleScore == 1)
		{
			ResumeGameMuti = ResumeGameMuti * 2;
		}
		else if(doubleScore == 2)
		{
			ResumeGameMuti = ResumeGameMuti * 4;
		}
        
		//重连缺少的数据 -1表示无数据
		data->getPlayerCashs().push_back(-1);
	}

	//just for test.
	//data->retain();

	WWDataManager::getInstance()->setGameStartData(data);
	if(GameGlobalDataManager::getInstance()->getCurSceneType() != GameSceneType::ROOM_SCENE)//不在游戏界面收到恢复对局消息
	{
		GameGlobalDataManager::getInstance()->setStartGameType(StartGameType::NO_SEND);

		//记录当前玩法
		int roomType = WWDataManager::getInstance()->getGameRoom()->getPlayType();
		GameLogicManager::getInstance()->setCurRoomType(roomType);
		
		//记录当前底注
		int roomBase = WWDataManager::getInstance()->getGameRoom()->getFortuneBase();
		GameLogicManager::getInstance()->setCurRoomBase(roomBase);
		
		auto *pScene = GameBasicScene::create();
		Director::getInstance()->replaceScene(pScene);

	}
	else
	{
		GameLogicManager::getInstance()->setMultiple(ResumeGameMuti);
		GameLogicManager::getInstance()->ProcessResumeGame(data);
	}    
}

//恢复房间基本信息
void NetRecvExecute::parseRecoveryGameNofity()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	RecoveryGameNotifyData *data = RecoveryGameNotifyData::create();
	data->setGameID(pClientSocket->ReadInt());
	data->setGameZoneID(pClientSocket->ReadInt());
	data->setGamePlayID(pClientSocket->ReadInt());
	data->setPlayType(pClientSocket->ReadShort());
	data->setZoneWin(pClientSocket->ReadChar());
	data->setFortuneBase(pClientSocket->ReadInt());
	data->setPointBase(pClientSocket->ReadInt());
	data->setMultiple(pClientSocket->ReadShort());
	data->setMoneyBox(pClientSocket->ReadChar());
	data->setFortuneMin(pClientSocket->ReadInt());
	data->setFortuneMax(pClientSocket->ReadInt());
	data->setRoomName(pClientSocket->ReadString());

	CCLOG("恢复对局倍数 %d, %d, %d, name =%s", data->getMultiple(), data->getFortuneBase(), data->getPointBase(), data->getRoomName().c_str());

	//玩法
	GameRoom *room = GameRoom::create();
	room->setPlayType(data->getPlayType());
	room->setFortuneBase(data->getFortuneBase());
	room->setGameId(data->getGameID());
	room->setGZID(data->getGameZoneID());
	room->setMultiple(data->getMultiple());
	//room->setFortuneMin(data->getFortuneMin());
	room->setName(data->getRoomName());
	//int nLevel = UserData::getInstance()->getIntValue(key_star_level);
	//if (nLevel == 0)
	//{
	//	nLevel = 1;
	//}
	//room->setStars(nLevel);
    
	//GameContext::getInstance()->setGameZoneId(data->getGameZoneID());
	//DataCenter::getInstance()->setGameRoom(room);
	
	// just for test.
	//room->retain();
	WWDataManager::getInstance()->setGameRoom(room);

	//TODO 进去游戏界面
	//WWSceneManager::getInstance()->setEnterGameType(StartGameType::SEND_RESUME);
	//WWSceneManager::getInstance()->changeToRoom();
    
}

void NetRecvExecute::parsePlayBoxList()//道具宝箱列表
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int GameID = pClientSocket->ReadInt();//(int4)游戏ID
	int BoxType = pClientSocket->ReadInt();//(int4)宝箱类型
	int Count = pClientSocket->ReadShort();//(int2)宝箱数量
	log("NetRecvExecute::parsePlayBoxList-----------GameID=%d,BoxType=%d, Count=%d",GameID,BoxType,Count);
	for(int i= 0; i < Count; i++)
	{
		int BoxID = pClientSocket->ReadInt();//(int4)宝箱ID(MagicID)
		int SubBoxType = pClientSocket->ReadChar();//(int1)子宝箱类型
		std::string Description = pClientSocket->ReadString();//(String)描述文字
		int BoxKey = pClientSocket->ReadInt();//(int4) 钥匙数量
		log("NetRecvExecute::parsePlayBoxList-----------BoxID=%d, SubBoxType=%d,Description=%s, BoxKey=%d",BoxID,SubBoxType,Description.c_str(),BoxKey);
	}
	//BoxType=1  SubBoxType=1金宝箱2银宝箱3铜宝箱
}
void NetRecvExecute::parseOpenMoneyBoxResp()//开奖励宝箱结果
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	int BoxID = pClientSocket->ReadInt();//(int4)宝箱的ID
	int Result = pClientSocket->ReadShort();//(int2) 0=成功；1=无效的服务代码/参数； 10=不支持, 21=物品不存在， -1=异常
	std::string desc = pClientSocket->ReadString();//(String)描述内容
	int Count = pClientSocket->ReadShort();//(int2)道具物品数量
	log("NetRecvExecute::parseOpenMoneyBoxResp-------BoxID=%d,Result=%d,desc=%s,Count=%d",BoxID,Result,desc.c_str(),Count);
	for (int i = 0; i < Count; i++)
	{
		std::string MagicName = pClientSocket->ReadString();//(String)道具名称
		int MagicID = pClientSocket->ReadInt();//(int4)道具ID
		int MagicCount = pClientSocket->ReadInt();//(int4)道具数量
		int MagicFID = pClientSocket->ReadInt();//(int4)物品FID
		int MagicType = pClientSocket->ReadChar();//(int1)1：普通道具 2：比赛门票 3：会员 4：虚拟货币 5：实物 9：道具包
		log("NetRecvExecute::parseOpenMoneyBoxResp-------MagicName=%s,MagicID=%d,MagicCount=%d,MagicFID=%d,MagicType=%d",MagicName.c_str(),MagicID,MagicCount,MagicFID,MagicType);
	}
}

void NetRecvExecute::parseIMatchSignupNotify()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	
	int MatchID = pClientSocket->ReadInt();//(int4)比赛ID

	int playerCount = pClientSocket->ReadInt();//(int4)当前报名人数

	log("NetRecvExecute::parseIMatchSignupNotify----MatchID=%d, playerCount=%d",MatchID, playerCount);

	if(WWDataManager::getInstance()->getGameZone())
	{
		MatchRoom* room = WWDataManager::getInstance()->getGameZone()->getMatchRoomByID(MatchRoom::MATCH_PEOPLE,MatchID);
		if(room)
		{
			room->setSignupPlayer(playerCount);
		}
		room = WWDataManager::getInstance()->getGameZone()->getMatchRoomByID(MatchRoom::MATCH_TIME,MatchID);
		if(room)
		{
			room->setSignupPlayer(playerCount);
		}
	}

	//TODO　刷新房间界面的报名人数
}

void NetRecvExecute::parseImatchPlayerStatus()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int MatchID = pClientSocket->ReadInt();//(int4)比赛ID

	int InstID = pClientSocket->ReadInt();//(int4)比赛实例ID

	int status = pClientSocket->ReadChar();//(int1)1开赛，2晋级下一轮,3淘汰，4比赛结束,5等待其它桌完成对局,6财富不足，等待充值,7现场恢复

	int Point = pClientSocket->ReadInt();//(int4)比赛积分

	int Ranking = pClientSocket->ReadInt();//(int4)比赛排名

	std::string Desc = pClientSocket->ReadString();//(String)描述信息

	log("NetRecvExecute::parseImatchPlayerStatus-----MatchID = %d, InstID=%d, status=%d, Point=%d,Ranking=%d,Desc=%s",MatchID,InstID,status,Point,Ranking,Desc.c_str());

	int curPart = pClientSocket->ReadChar();//(int1)当前赛段

	int curSet = pClientSocket->ReadChar();//(int1)当前第几论

	int curPlay = pClientSocket->ReadShort();//(int2)当前第几局

	std::string curPartName = pClientSocket->ReadString();//(String)当前赛段名称

	int curPlayerCount = pClientSocket->ReadInt();//(int4)当前未比赛人数

	int Matchtype = pClientSocket->ReadChar();//(int1)比赛类型 1水晶赛，2大奖赛

	int MatchPartCount = pClientSocket->ReadChar();//(int1)比赛赛段数

	int remainTable = pClientSocket->ReadShort();//(int2)当前赛段总轮数

	int SelCount = pClientSocket->ReadShort();//(int2)正在游戏的桌数

	int AllSelCount = pClientSocket->ReadShort();//(int2)当前比赛所有赛段总轮数

	log("NetRecvExecute::parseImatchPlayerStatus-----curPart=%d,curSet=%d,curPlay=%d, curPartName = %s, curPlayerCount=%d,Matchtype=%d,MatchPartCount=%d,remainTable=%d,SelCount=%d,AllSelCount",
		curPart,curSet,curPlay,curPartName.c_str(),curPlayerCount,Matchtype,MatchPartCount,remainTable,SelCount,AllSelCount);

	if(status == 1 || status == 7)
	{
		GameGlobalDataManager::getInstance()->setPlayRoomId(MatchID);
	}
}

void NetRecvExecute::parseIMatchGameOver()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int GamePlayID = pClientSocket->ReadInt();//(int4)对局标示

	int MatchID = pClientSocket->ReadInt();//(int4)比赛ID

	int GameID = pClientSocket->ReadInt();//(int4)游戏ID

	int Cuntian = pClientSocket->ReadChar();//(int1)1=春天

	int BoomCount = pClientSocket->ReadChar();//(int1)炸弹数量

	int SnatchLord = pClientSocket->ReadChar();//(int1)抢地主次数

	int DiscoverCards = pClientSocket->ReadChar();//(int1)1=明牌

	int Multiple = pClientSocket->ReadShort();//(int2)总倍数

	int Count = pClientSocket->ReadChar();//(int1)游戏人数

	log("NetRecvExecute::parseIMatchGameOver--------GamePlayID=%d,MatchID=%d,GameID=%d,Cuntian=%d,BoomCount=%d,SnatchLord=%d,DiscoverCards=%d,Multiple=%d,Count=%d,",
		GamePlayID,MatchID,GameID,Cuntian,BoomCount,SnatchLord,DiscoverCards,Multiple,Count);

	for (int i = 0; i < Count; i++)
	{
		int UserID = pClientSocket->ReadInt();//(int4)用户1ＩＤ 

		int UserType = pClientSocket->ReadChar();//(int1)1真人，4断线，6回主菜单，7托管

		std::string Card = pClientSocket->ReadString();//(byte[])剩余牌

		int Ranking = pClientSocket->ReadInt();//(int4)排名

		int Score = pClientSocket->ReadInt();//(int4)积分变化 

		int TScore = pClientSocket->ReadInt();//(int4)总积分

		int RankingDif = pClientSocket->ReadInt();//(int4)排名变化

		int OutFlag = pClientSocket->ReadChar();//(int1)1=本局被淘汰

		int Result = pClientSocket->ReadChar();//(int1)1=赢,2=输

		log("NetRecvExecute::parseIMatchGameOver--------UserID=%d,UserType=%d,Card=%s,Ranking=%d,Score=%d,TScore=%d,RankingDif=%d,OutFlag=%d,Result=%d",
			UserID,UserType,Card.c_str(),Ranking,Score,TScore,RankingDif,OutFlag,Result);
	}
}

void NetRecvExecute::parseIMatchBye()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	int userID = pClientSocket->ReadInt();//(int4)用户ID

	int MatchID = pClientSocket->ReadInt();//(int4)比赛ID

	int InstID = pClientSocket->ReadInt();//(int4)比赛实例ID

	int Action = pClientSocket->ReadChar();//(int1)1本轮轮空、等待下一轮速配 

	int curPart = pClientSocket->ReadChar();//(int1)当前赛段

	int curSet = pClientSocket->ReadChar();//(int1)当前第几论

	int curPlay = pClientSocket->ReadShort();//(int2)当前第几局

	int Score = pClientSocket->ReadInt();//(int4)比赛积分

	log("NetRecvExecute::parseIMatchBye---------userID=%d,MatchID=%d,Action=%d,curPart=%d,curSet=%d,curPlay=%d,Score=%d ", 
		userID,MatchID,Action,curPart ,curSet, curPlay, Score);
}

void NetRecvExecute::parseIMatchAntilordStart()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	GameStartData* data = GameStartData::create();

	data->setGameType(2);
	data->setGameId(1001);

	int GamePlayID = pClientSocket->ReadInt();//(int4)对局标示

	int playtimeout = pClientSocket->ReadShort();//(int2)单手牌超时时间（单位：秒）

	int MatchScoreBase = pClientSocket->ReadInt();//(int4)比赛积分基数

	int Firstplayer = pClientSocket->ReadInt();//(int4)第一个叫分用户 

	int playerCount = pClientSocket->ReadChar();//(int1)游戏人数

	GameGlobalDataManager::getInstance()->setCurPlayId(GamePlayID);
	GameGlobalDataManager::getInstance()->setCurGameId(data->getGameId());

	data->setGamePlayId(GamePlayID);
	data->setPlayTimeOut(playtimeout);
	data->setFortuneBase(MatchScoreBase);
	data->setFirstPlayer(Firstplayer);
	data->setPlayerCount(playerCount);

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

	log("NetRecvExecute::parseIMatchAntilordStart---------GamePlayID=%d,playtimeout=%d,MatchScoreBase=%d, Firstplayer=%d, playerCount=%d, ",
		GamePlayID, playtimeout, MatchScoreBase, Firstplayer, playerCount);

	for (int i = 0; i < playerCount; i++)
	{
		int UserID = pClientSocket->ReadInt();//(int4)用户1ＩＤ

		std::string UserName = pClientSocket->ReadString();//(String)用户1名称

		int Score = pClientSocket->ReadInt();//(int4)当前比赛分数

		std::string Card = pClientSocket->ReadString();//(byte[])用户1的牌

		log("NetRecvExecute::parseIMatchAntilordStart---------UserID=%d, UserName = %s, Score = %d, Card = %s",
			UserID, UserName.c_str(), Score, Card.c_str());

		data->getPlayerIds()[i] = UserID;
		data->getPlayerNickNames()[i]= UserName;
		data->getPlayerCashs()[i] = Score;
		data->getPlayerPokers()[i] = Card;
	}

	WWDataManager::getInstance()->setGameStartData(data);

	//TODO 根据界面进行操作， 如在大厅则启动游戏界面，如在游戏中，回调开局方法

	if(GameGlobalDataManager::getInstance()->getCurSceneType() != GameSceneType::ROOM_SCENE)//不在游戏界面收到恢复对局消息
	{
		GameGlobalDataManager::getInstance()->setStartGameType(StartGameType::MATCH_START_FROM_LOBBY);

		//记录当前玩法
		GameLogicManager::getInstance()->setCurRoomType(GameRoom::PLAYTYPE_HAPPY);//比赛为欢乐玩法
		
		//记录当前底注
		GameLogicManager::getInstance()->setCurRoomBase(MatchScoreBase);
		
		auto *pScene = GameBasicScene::create();
		Director::getInstance()->replaceScene(pScene);
	}
	else
	{
		GameGlobalDataManager::getInstance()->setStartGameType(StartGameType::MATCH_START_IN_GAME);
		GameLogicManager::getInstance()->setMultiple(1);
		GameLogicManager::getInstance()->ProcessMatchStart(data);
	}
}

void NetRecvExecute::parseImatchAntilordResume()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int GamePlayID = pClientSocket->ReadInt();//(int4)对局标示

	int playtimeout = pClientSocket->ReadShort();//(int2)单手牌超时时间（单位：秒）

	int MatchScoreBase = pClientSocket->ReadInt();//(int4)比赛积分基数

	int NextPlayer = pClientSocket->ReadInt();//(int4)下一出牌玩家 

	int LordID = pClientSocket->ReadInt();//(int4)地主ID

	std::string BaseCard = pClientSocket->ReadString();//(byte[])底牌

	int BankScore = pClientSocket->ReadChar();//(int1)游戏底分

	int Multiple = pClientSocket->ReadShort();//(int2)游戏倍数

	int Status = pClientSocket->ReadChar();//(int1)0叫牌，抢地主，2明牌，3打牌

	int LastPlayerID = pClientSocket->ReadInt();//(int4)最后出牌玩家

	std::string LastCards = pClientSocket->ReadString();//(byte[])最后出牌

	int Discover = pClientSocket->ReadChar();//(int1)明牌标志.1-明牌,0不明牌,2未确定

	int Count = pClientSocket->ReadChar();//(int1)游戏人数

	log("NetRecvExecute::parseImatchAntilordResume----GamePlayID=%d,playtimeout=%d, MatchScoreBase=%d, NextPlayer=%d, LordID=%d, BaseCard = %s, BankScore = %d, Multiple=%d, Status=%d, LastPlayerID=%d,LastCards=%s ,Discover=%d,Count=%d",
		GamePlayID, playtimeout,MatchScoreBase,NextPlayer,LordID, BaseCard.c_str(), BankScore,Multiple, Status,LastPlayerID,LastCards.c_str(),Discover, Count);

	for (int i = 0; i < Count; i++)
	{
		int UserID = pClientSocket->ReadInt();//(int4)用户1ＩＤ

		std::string UserName = pClientSocket->ReadString();//(String)用户1名称

		int Point = pClientSocket->ReadInt();//(int4)当前积分

		std::string Card = pClientSocket->ReadString();//(byte[])用户的牌

		log("NetRecvExecute::parseImatchAntilordResume----UserID = %d,UserName=%s,  Point = %d, Card = %s", UserID, UserName.c_str(), Point, Card.c_str());
	}
}
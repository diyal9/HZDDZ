
#include "MsgMatchRequest.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Model/WWDataModel/WWGD/GD.h"

MsgMatchRequest::MsgMatchRequest()
{
}

MsgMatchRequest::~MsgMatchRequest()
{
}

void MsgMatchRequest::sendIMatchListReq(int matchType, int type)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(6);
	pClientSocket->WriteChar(2);
	pClientSocket->WriteChar(40);

	pClientSocket->WriteInt(1001);//(int4)游戏ID
	pClientSocket->WriteShort(2);//(int2) 游戏玩法
	pClientSocket->WriteChar(matchType);//(int1)比赛类型  0不限,1水晶赛，2大奖赛
	pClientSocket->WriteChar(type);// (int1)  0当前开赛  1 推荐赛事  2 结束比赛(比赛详情)


	pClientSocket->SendCommand();
}

void MsgMatchRequest::sendIMatchInfoReq(int matchID, int type, int objectID, int userID)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(6);
	pClientSocket->WriteChar(2);
	pClientSocket->WriteChar(42);

	pClientSocket->WriteInt(matchID);//(int4)比赛ID
	/*
	Type=0报名前显示比赛详情的界面,
	Type=1需开赛后方可获取排名
	*/
	pClientSocket->WriteChar(type);//(int1)0详情，1排名,2 比赛规则,3比赛免责声明,4 结束比赛的获奖玩家列表 5 小窗口排名,6比赛奖状,7常见问题 
	pClientSocket->WriteInt(objectID);//(int4)比赛实例ID
	pClientSocket->WriteInt(userID);//(int4) type=6有效，查询用户userid


	pClientSocket->SendCommand();
}

void MsgMatchRequest::sendIMatchSignup(int matchID, int action, const char* nickName, int vipGrade, int param)
{
	WWNetWorkManager::getInstance()->SendPrepare();
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	pClientSocket->WriteChar(6);
	pClientSocket->WriteChar(2);
	pClientSocket->WriteChar(44);

	pClientSocket->WriteInt(matchID);//(int4)比赛ID
	pClientSocket->WriteChar(action);//(int1)1报名，2退赛  3请求报名人数
	pClientSocket->WriteString(nickName);//(String)昵称
	pClientSocket->WriteChar(vipGrade);//(byte)vip等级
	pClientSocket->WriteInt(param);//(int4)1=使用门票碎片报名

	pClientSocket->SendCommand();
}

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------

void MsgMatchRequest::parseIMatchSignupNotify()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	
	int MatchID = pClientSocket->ReadInt();//(int4)比赛ID

	int playerCount = pClientSocket->ReadInt();//(int4)当前报名人数
}

void MsgMatchRequest::parseImatchPlayerStatus()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int MatchID = pClientSocket->ReadInt();//(int4)比赛ID

	int InstID = pClientSocket->ReadInt();//(int4)比赛实例ID

	int status = pClientSocket->ReadChar();//(int1)1开赛，2晋级下一轮,3淘汰，4比赛结束,5等待其它桌完成对局,6财富不足，等待充值,7现场恢复

	int Point = pClientSocket->ReadInt();//(int4)比赛积分

	int Ranking = pClientSocket->ReadInt();//(int4)比赛排名

	std::string Desc = pClientSocket->ReadString();//(String)描述信息

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
}

void MsgMatchRequest::parseIMatchGameOver()
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
	}
}

void MsgMatchRequest::parseIMatchBye()
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
}

void MsgMatchRequest::parseIMatchAntilordStart()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int GamePlayID = pClientSocket->ReadInt();//(int4)对局标示

	int playtimeout = pClientSocket->ReadShort();//(int2)单手牌超时时间（单位：秒）

	int MatchScoreBase = pClientSocket->ReadInt();//(int4)比赛积分基数

	int Firstplayer = pClientSocket->ReadInt();//(int4)第一个叫分用户 

	int Count = pClientSocket->ReadChar();//(int1)游戏人数

	for (int i = 0; i < Count; i++)
	{
		int UserID = pClientSocket->ReadInt();//(int4)用户1ＩＤ

		std::string UserName = pClientSocket->ReadString();//(String)用户1名称

		int Score = pClientSocket->ReadInt();//(int4)当前比赛分数

		std::string Card = pClientSocket->ReadString();//(byte[])用户1的牌
	}
}

void MsgMatchRequest::parseImatchAntilordResume()
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

	for (int i = 0; i < Count; i++)
	{
		int UserID = pClientSocket->ReadInt();//(int4)用户1ＩＤ

		std::string UserName = pClientSocket->ReadString();//(String)用户1名称

		int Point = pClientSocket->ReadInt();//(int4)当前积分

		std::string Card = pClientSocket->ReadString();//(byte[])用户的牌
	}
}

void MsgMatchRequest::parseMatchPerformance()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int instID = pClientSocket->ReadInt();//(int4)当前比赛实例ID

	int MatchID = pClientSocket->ReadInt();//(int4)比赛ID

	int Ranking = pClientSocket->ReadInt();//(int4)比赛排名

	std::string Desc = pClientSocket->ReadString();//(String)成绩描述信息

	int BestBanking = pClientSocket->ReadInt();//(int4)历史最佳排名

	int RewardCount = pClientSocket->ReadShort();//((int2)奖励数量

	for (int i = 0; i < RewardCount; i++)
	{
		int RewardType = pClientSocket->ReadShort();//(int2)0=水晶  1=物品  2=会员  3=实物  4=话费卡

		int RewardData = pClientSocket->ReadInt();//(int4)水晶数量或者物品ID

		int UserExchID = pClientSocket->ReadInt();//(int4)实物兑换ID

		std::string RewardStr = pClientSocket->ReadString();//(String)奖励的描述

		log("MsgMatchRequest::parseMatchPerformance--------RewardType=%d,RewardData=%d,UserExchID=%d,RewardStr=%s,", 
			RewardType, RewardData, UserExchID, RewardStr.c_str());
	}

	std::string MatchName = pClientSocket->ReadString();//(String)比赛名称

	log("MsgMatchRequest::parseMatchPerformance--------instID=%d,MatchID=%d,Ranking=%d,Desc=%s,BestBanking=%d,RewardCount=%d,MatchName=%s",
		instID,MatchID,Ranking,Desc.c_str(),BestBanking,RewardCount,MatchName.c_str());
}
//斗地主开局数据

#ifndef GAMESTARTDATA_H_
#define GAMESTARTDATA_H_

#include "cocos2d.h"
#include "WWMacros.h"


class GameStartData: public cocos2d::Ref
{
public:
	GameStartData();
	virtual ~GameStartData();
	virtual bool init();
	CREATE_FUNC(GameStartData);

	CC_SYNTHESIZE(int, gameType, GameType);	//游戏类型，  1=欢乐或癞子， 2= 比赛， 3=约战
	CC_SYNTHESIZE(int, gameId, GameId);	//游戏ＩＤ
	CC_SYNTHESIZE(int, gamePlayId, GamePlayId);	//对局标示
	CC_SYNTHESIZE(int, playTimeOut, PlayTimeOut);	//单手牌超时时间（单位：秒）
	CC_SYNTHESIZE(int, doubleTime, DoubleTime);//(int2)加倍时间（单位：秒）
	CC_SYNTHESIZE(int, firstPlayer, FirstPlayer);	//第一个叫分用户
	CC_SYNTHESIZE(std::string, data, Data);	//(String)附加数据-----------------------------
	CC_SYNTHESIZE(int, taskNum4Award, TaskNum4Award);//(int2)奖宝箱局数

	CC_SYNTHESIZE(int, taskEachNumAward, TaskEachNumAward);	//奖宝的单位局数
	CC_SYNTHESIZE(int, playedNum4Award, PlayedNum4Award);	//(int2)当前财富局数

	CC_SYNTHESIZE(int, fortuneBase, FortuneBase);	//财富底注
	//CC_SYNTHESIZE(int, trump, Trump);	//灌蛋主牌
	//CC_SYNTHESIZE(int, jingGong, JingGong);	//灌蛋进贡：1-要进贡  0-不用进贡
	CC_SYNTHESIZE(int, laizi, Laizi);	//癞子

	CC_SYNTHESIZE(int, playerCount, PlayerCount);	//游戏人数

	//CC_SYNTHESIZE(bool, hasBox, HasBox);	//是否已有宝箱 0：没有 1：已有
	//CC_SYNTHESIZE(bool, boxSwitch, BoxSwitch);	//在线宝箱开关 0-关 1：开
	//CC_SYNTHESIZE(int, boxId, BoxId);	//在线宝箱id
	//CC_SYNTHESIZE(int, boxOpenTime, BoxOpenTime);	//开宝箱剩余时间(秒)

	CC_SYNTHESIZE(bool, isResume, Resume);	//恢復對局

	//CC_SYNTHESIZE_PASS_BY_REF(std::string, attachData, AttachData);

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, playerIds, PlayerIds);	//用户ＩＤ

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, playerNickNames, PlayerNickNames); //用户名称
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, playerIcons, PlayerIcons);	//用户头像

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, playerVips, PlayerVips);

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, playerPokers, PlayerPokers);	//用户的牌

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, playerCashs, PlayerCashs);	//用户财富值

	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, gameLevels, GameLevels);	//灌蛋升级达到几级
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, teamLevels, TeamLevels);	//用户所属战队星级
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, isTeamLeader, TeamLeader);	//是否战队队长； 0-不是，1－是
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, isTongLeader, TongLeader);	//是否帮会帮主； 0-不是，1－是
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, isPropDoublePower, PropDoublePower);	//0－没有道具，1－使用双倍分道具
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, isRegion, Region);	//省份
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, jiFenLevel, JiFenLevel);	//游戏积分等级
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, magicStartShow, MagicStartShow);	//1－闪亮登场道具，0－没有该道具
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, genders, Genders);	//性別
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, onlineTypes, OnlineTypes);	//(String)网络接入

	
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, shuangKouMagicShow, ShuangKouMagicShow);
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, kongs, Kongs);



	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<bool>, isMembers, Members);




	//恢复对局增加参数
	CC_SYNTHESIZE(int, nextPlayUserID, NextPlayUserID);	//下个出牌玩家
	CC_SYNTHESIZE(int, lordId, LordId);	//地主ＩＤ
	
	CC_SYNTHESIZE(bool, mingPai, MingPai);	//明牌
	//CC_SYNTHESIZE(cocos2d::Vector<PokerObject*>, basePokers, BasePokers);	//(byte[])底牌
	//CC_SYNTHESIZE(cocos2d::Vector<PokerObject*>, lastPokers, LastPokers);	//(byte[])最后出牌

	CC_SYNTHESIZE(std::string, basePokers, BasePokers);	//(byte[])底牌
	CC_SYNTHESIZE(int, bankScore, BankScore);	//(int1)游戏底分
	CC_SYNTHESIZE(int, gameStatus, GameStatus);	//(int1)1叫地主，2抢地主，3加倍，4明牌  5打牌 6结算

	CC_SYNTHESIZE(int, lastPlayerID, LastPlayerID);	//(int4)最后出牌玩家
	CC_SYNTHESIZE(std::string, lastPokers, LastPokers);	//(byte[])最后出牌

	CC_SYNTHESIZE(int, boomCount, BoomCount);	//(int1)炸弹数量

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<bool>, qiangDizhus, QiangDizhus);	//(int1)抢地主标志：1－抢，0－不抢
	
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, doubleScoreFlag, DoubleScoreFlag);	//(int1)加倍标志 0 未 1加倍 2 强加被

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<bool>, forbidTalk, ForbidTalk);//禁言标识
	
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, keepWin, KeepWin);	//(int2)连胜数

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, cardNum, CardNum);	//(int2)连胜数
};

///*
// *比赛开局数据
// *
// */
//class MatchGameStartData: public cocos2d::Ref
//{
//public:
//	MatchGameStartData();
//	virtual ~MatchGameStartData();
//	virtual bool init();
//	CREATE_FUNC(MatchGameStartData);
//};

#endif /* GAMESTARTDATA_H_ */

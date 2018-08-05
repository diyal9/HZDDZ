/*
 * LordGameData.h

 *	Author: zx
 */

#ifndef LORDGAMEDATA_H_
#define LORDGAMEDATA_H_

#include "cocos2d.h"
//#include "WWPlatform/WWMacros.h"
//#include "../../../Tools/PokerLogic/PokerLogic.h"

#include "WWMacros.h"

/**
 * 叫地主数据
 */
class CallLordData:public cocos2d::Ref
{
public:
	CallLordData();
	virtual ~CallLordData();
	virtual bool init();
	CREATE_FUNC(CallLordData);
	CC_SYNTHESIZE(int, gamePlayId, GamePlayId);//对局标示
	CC_SYNTHESIZE(int, userIdJiaoFen, UserIdJiaoFen);//(int4)用户ＩＤ
	CC_SYNTHESIZE(int, jiaoFlag, JiaoFlag);//(int1)0:不叫 1：叫
	CC_SYNTHESIZE(int, nextPlayer, NextPlayer); //(int4)Flag＝0，下一个叫牌用户ＩＤ； Flag＝1，则为下一个抢地主或者出牌用户ID。
	CC_SYNTHESIZE(int, playerCount, PlayerCount);//游戏人数
	CC_SYNTHESIZE(int, nextAction, NextAction);//（int1）1:叫地主   2：抢地主  3加倍  4明牌  5出牌

	cocos2d::Vector<PokerObject *>& getBottomPokers();	//PokerObject底牌

private:
	cocos2d::Vector<PokerObject *> bottomPokers;	//底牌
};

/**
 * 抢地主数据
 */
class GrabLordData:public cocos2d::Ref
{
public:
	GrabLordData();
	virtual ~GrabLordData();
	virtual bool init();
	CREATE_FUNC(GrabLordData);
	CC_SYNTHESIZE(int, gamePlayId, GamePlayId);//对局标示
	CC_SYNTHESIZE(int, userIdQiangDiZhu, UserIdQiangDiZhu);//(int4)用户ＩＤ
	CC_SYNTHESIZE(int, isGrab, IsGrab);//(int1)0:不叫 1：叫
	CC_SYNTHESIZE(int, isQiangDiZhuOver, IsQiangDiZhuOver); ////(int1) 2:抢地主  3加倍  4地主明牌  5出牌
	CC_SYNTHESIZE(int, nextPlayer, NextPlayer);//游戏人数

	cocos2d::Vector<PokerObject *>& getBottomPokers();	//PokerObject底牌

private:
	cocos2d::Vector<PokerObject *> bottomPokers;	//底牌
};

/**
 * 打牌数据
 */
class PlayCardData:public cocos2d::Ref
{
public:
	PlayCardData();
	virtual ~PlayCardData();
	virtual bool init();
	CREATE_FUNC(PlayCardData);
	CC_SYNTHESIZE(int, gamePlayId, GamePlayId);//对局标示
	CC_SYNTHESIZE(int, userID, UserID);//(int4)用户ＩＤ
	CC_SYNTHESIZE(int, type, Type);//(int1)牌型值,=0由接收端判断
	CC_SYNTHESIZE(int, value, Value); //(int1)牌的大小，=0由接收端判断
	CC_SYNTHESIZE(int, nextPlayer, NextPlayer);//(int4)下一个出牌用户

	cocos2d::Vector<PokerObject *>& getPlayPokers();//PokerObject(byte[])原始牌
	cocos2d::Vector<PokerObject *>& getReplacePokers();//PokerObject(byte[])替换后牌

private:
	cocos2d::Vector<PokerObject *> playPokers;	//底牌
	cocos2d::Vector<PokerObject *> replacePokers;	//底牌

};

/**
 * 明牌数据
 */
class MingPaiData:public cocos2d::Ref
{
public:
	MingPaiData();
	virtual ~MingPaiData();
	virtual bool init();
	CREATE_FUNC(MingPaiData);
	CC_SYNTHESIZE(int, gamePlayId, GamePlayId);//对局标示
	CC_SYNTHESIZE(int, userID, UserID);//(int4)用户ＩＤ

	cocos2d::Vector<PokerObject *>& getPlayerPokers();//PokerObject(byte[])原始牌

private:
	cocos2d::Vector<PokerObject *> playerPokers;//用户牌

};


/**
 * 加倍数据
 */
class JiaBeiData:public cocos2d::Ref
{
public:
	JiaBeiData();
	virtual ~JiaBeiData();
	virtual bool init();
	CREATE_FUNC(JiaBeiData);
	CC_SYNTHESIZE(int, gamePlayId, GamePlayId);//对局标示
	CC_SYNTHESIZE(int, userID, UserID);//(int4)用户ＩＤ
	CC_SYNTHESIZE(int, flag, Flag);//(int1)用户是否抢地主  0:不加倍  1:加倍  2：加强加倍  3：加倍结束
	CC_SYNTHESIZE(int, nextAction, NextAction);//(int1)2叫牌  当flag=3时  3地主明牌  4出牌
	CC_SYNTHESIZE(int, nextPlay, NextPlay);//(int4)flag=3加倍结束，为下一个明牌或出牌用户ＩＤ 
};


/**
 * 牌局结束
 */
class LordGameOverData:public cocos2d::Ref
{
public:
	LordGameOverData();
	virtual ~LordGameOverData();
	virtual bool init();
	CREATE_FUNC(LordGameOverData);
	CC_SYNTHESIZE(int, gamePlayId, GamePlayId);//对局标示
	CC_SYNTHESIZE(int, winnerUserID, WinnerUserID);//(int4)赢家用户ＩＤ
	CC_SYNTHESIZE(std::string, unit, Unit);//(String)单位
	CC_SYNTHESIZE(std::string, data, Data);//(String)附加数据
	CC_SYNTHESIZE(int, zoneWin, ZoneWin);//int1)输赢类型:1:战斗力2:财富5.金豆
	CC_SYNTHESIZE(int, multiple, Multiple);//(int2)输赢倍数,由游戏区定义
	CC_SYNTHESIZE(std::string, time, Time);//(String)游戏时长
	CC_SYNTHESIZE(int, bankUserID, BankUserID);//(int4)地主UserID
	CC_SYNTHESIZE(int, bankScore, BankScore);//(int1)地主叫分
	CC_SYNTHESIZE(int, boomCount, BoomCount);//(int1)炸弹数量
	CC_SYNTHESIZE(int, fan, Fan);//(int2)游戏番数
	CC_SYNTHESIZE(int, fortuneBase, FortuneBase);//(int4)财富底分
	CC_SYNTHESIZE(int, bigBoomCount, BigBoomCount);//(int1)炸弹数量2
	CC_SYNTHESIZE(int, isChunTian, IsChunTian);//是否是春天

	CC_SYNTHESIZE(int, magicCount, MagicCount);//(int2)礼包数量
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, magicIDs, MagicIDs);//(int4)礼包ID
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, magicTypes, MagicTypes);//(int1)礼包类型

	CC_SYNTHESIZE(int, count, Count);//玩家数量

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, userID, UserIDs);//(int4)用户1ＩＤ
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, scores, Scores);//(int4)用户1本局财富变化/战力/筹码
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, tScores, TScores);//(int4)用户1本局后总财富 /战力/筹码
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, levels, Levels);//int2)用户1本局后级别
	
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, card, Card);//(byte[])用户剩余在手牌

	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, magics, Magics);//(int1)第一位:双倍战力 第二位:双倍战力第三位:双倍战力

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, dLWLs, DLWLs);//(int1)倒拉窝垒标志,低4位表示
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, fans, Fans);//(int2)倍数
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, gamePoints, GamePoints);//(int4)用户1本局游戏积分变化
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, tGamePoints, TGamePoints);//(int4)用户1本局后游戏总积分
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, gameLevelNames, GameLevelNames);//(String)用户1本局后游戏积分等级称号
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, allPlayCounts, AllPlayCounts);//(int2)当天本游戏区总局数
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, neededPlayCounts, NeededPlayCounts);//(int2)获取金宝箱需要对局数
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, luckyPoint, LuckyPoints);//(int4)幸运值
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, greatAward, GreatAwards);//(int4)最高奖蛙币
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, continueWinNum, ContinueWinNums);//(int2)连胜数//33
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, topWinNum, TopWinNums);//(int2)历史最高连胜数

	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, giftCount, GiftCounts);//(int1)奖励数量
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::vector<std::string>>, itemName, ItemNames);//（String）奖励物品名称
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::vector<int>>, itemID, ItemIDs);//（int4）奖励物品ID
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::vector<int>>, itemCount, ItemCounts);//（int4）奖励物品数量
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::vector<int>>, itemFID, ItemFIDs);//(int4)奖励物品FID
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::vector<int>>, itemFunType, ItemFunTypes);//(int2)物品功能类别 1:使用  2：兑换

	cocos2d::Vector<PokerObject *>& getSelfRemainPokers();//自己的剩余牌
	cocos2d::Vector<PokerObject *>& getRightRemainPokers();//下家的剩余牌
	cocos2d::Vector<PokerObject *>& getLeftRemainPokers();//上家剩余牌

private:
	cocos2d::Vector<PokerObject *> selfRemainPokers;//自己的剩余牌
	cocos2d::Vector<PokerObject *> rightRemainPokers;//下家的剩余牌
	cocos2d::Vector<PokerObject *> leftRemainPokers;//上家剩余牌

};
#endif /* LORDGAMEDATA_H_ */

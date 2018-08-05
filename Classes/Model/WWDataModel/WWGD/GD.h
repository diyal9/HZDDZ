/*
 * GD.h
 *	功能描述：
 *  Created on: 2014年6月24日 下午5:09:36
 *      Author: zx
 */

#ifndef GD_H_
#define GD_H_
#include "cocos2d.h"
#include "WWMacros.h"

/**
 * 游戏房间数据
 */
class GameRoom:public cocos2d::Ref
{
public:
	static const int PLAYTYPE_HAPPY = 2;
	static const int PLAYTYPE_LAIZI = 4;
	// 为反射调用设置
	// 添加变量的时候请设置get和set方法，方便序列化
public:
	//const static int GAMENAME = 1;
	//const static int ZONENAME = 2;
	//const static int WADOUNAME = 3;
	GameRoom();

	virtual ~GameRoom();

	virtual bool init();

	CREATE_FUNC(GameRoom);
	//
	CC_SYNTHESIZE(int, gameId, GameId);//（int4）游戏编码
	CC_SYNTHESIZE(int, gZID, GZID);//(int4)游戏区ＩＤ
	CC_SYNTHESIZE(std::string, name, Name);//(String)游戏区名称
	CC_SYNTHESIZE(int, zoneWin, ZoneWin);// (int1)标识游戏区是否是财富游戏区，1：普通游戏区 2：钻石区,5 金币区
	CC_SYNTHESIZE(int, status, Status);//(byte)游戏区状态
	CC_SYNTHESIZE(std::string, description,Description);//(String)游戏区描述
	CC_SYNTHESIZE(int, acount, Acount);//(int2)游戏区人数
	CC_SYNTHESIZE(int, playerCount, PlayerCount);//(int2)游戏区每局人数
	CC_SYNTHESIZE(int, playType, PlayType);//(int2)游戏玩法
	CC_SYNTHESIZE(int, multiple, Multiple);//(int2)倍分
	CC_SYNTHESIZE(int, enterFortune, EnterFortune);//(int4)带入值
	CC_SYNTHESIZE(int, zoneType, ZoneType);//(int2)分区类型1：普通分区2：比赛/活动区  3：VIP专区
	CC_SYNTHESIZE(int, invite, Invite);//(int1)是否可以约战：0：否  1：是
	CC_SYNTHESIZE(int, fortuneBase, FortuneBase);//(int4)财富计算的底数
	CC_SYNTHESIZE(int, pointBase, PointBase);//(int4)积分计算的底数
	CC_SYNTHESIZE(int, levelMinLimit, LevelMinLimit);//(int4)等级下限
	CC_SYNTHESIZE(int, levelMaxLimit, LevelMaxLimit);//(int4)等级上限
	CC_SYNTHESIZE(int, presentBean, PresentBean);//(int1)是否赠豆，0不赠，1赠送
	CC_SYNTHESIZE(int, playedCount, PlayedCount);//(int2)当天当前局数
	CC_SYNTHESIZE(int, tableFee, TableFee);//(int4)桌费
	CC_SYNTHESIZE(int, recordCardFalg, RecordCardFalg);//(int1)记牌器标志
	CC_SYNTHESIZE(int, antiCheatFlag, AntiCheatFlag);//(int1)防作弊标志

	CC_SYNTHESIZE(int, lockFlag, LockFlag);//(int1)锁标志 0 无锁 1 有锁
};

//比赛房间信息
class MatchRoom : public cocos2d::Ref
{
public:
	static const int MATCH_PEOPLE = 1;
	static const int MATCH_TIME = 2;
public:
	MatchRoom();

	virtual ~MatchRoom();

	virtual bool init();
	
	CREATE_FUNC(MatchRoom);

	CC_SYNTHESIZE(int, gameId, GameId);//(int4)游戏ID
	CC_SYNTHESIZE(std::string, curTime, CurTime);//(String)服务器当前时间戳,from 1970-1-1 0:0:0

	//CC_SYNTHESIZE(int, count, Count);//(int2)比赛数量

	CC_SYNTHESIZE(int, matchID, MatchID);//(int4)比赛ID
	CC_SYNTHESIZE(int, matchtype, Matchtype);//(int1)比赛类型 1水晶赛，2大奖赛
	CC_SYNTHESIZE(int, beginType, BeginType);//开赛方式：1：指定人数开赛  2：定时开赛
	CC_SYNTHESIZE(int, matchPartCount, MatchPartCount);//(int1)比赛赛段数

	CC_SYNTHESIZE(std::string, matchName, MatchName);//(int1)(String)比赛名称
	CC_SYNTHESIZE(int, hot, Hot);//(int1)1=热门比赛
	CC_SYNTHESIZE(std::string, matchTime, MatchTime);//(String)开赛时间
	CC_SYNTHESIZE(std::string, signupTerm, SignupTerm);//(String)报名条件

	CC_SYNTHESIZE(int, signupPlayer , SignupPlayer );//(int4)报名人数
	CC_SYNTHESIZE(int, signupFlag, SignupFlag);//(int1)报名状态  0=未报名  1=报名
	CC_SYNTHESIZE(int, waitTime, WaitTime);//(int4)等待开赛时间,单位秒
	CC_SYNTHESIZE(int, intervalTime, IntervalTime);//(int4)开赛间隔时间

	CC_SYNTHESIZE(int, ticketFID, TicketFID);//(int4)报名门票FID
	CC_SYNTHESIZE(int, curPlayingPlayer , CurPlayingPlayer );//(int4)比赛中正在游戏中人数

	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, matchID, MatchID);//(int4)比赛ID
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, matchtype, Matchtype);//(int1)比赛类型 1水晶赛，2大奖赛
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, beginType, BeginType);//开赛方式：1：指定人数开赛  2：定时开赛
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, matchPartCount, MatchPartCount);//(int1)比赛赛段数

	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, matchName, MatchName);//(int1)(String)比赛名称
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, hot, Hot);//(int1)1=热门比赛
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, matchTime, MatchTime);//(String)开赛时间
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, signupTerm, SignupTerm);//(String)报名条件

	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, signupPlayer , SignupPlayer );//(int4)报名人数
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, signupFlag, SignupFlag);//(int1)报名状态  0=未报名  1=报名
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, waitTime, WaitTime);//(int4)等待开赛时间,单位秒
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, intervalTime, IntervalTime);//(int4)开赛间隔时间

	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, ticketFID, TicketFID);//(int4)报名门票FID
	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, curPlayingPlayer , CurPlayingPlayer );//(int4)比赛中正在游戏中人数
};

/**
 * 游戏区
 */
class GameZone:public cocos2d::Ref
{

public:
	GameZone();

	virtual ~GameZone();

	virtual bool init();
	
	CREATE_FUNC(GameZone);

	void setRooms(int playType, cocos2d::__Array* rooms);

	cocos2d::__Array* getRooms(int playType);

	GameRoom* getRoom(int playType, int zoneMuti);
    
    //add on 20141215 wildcat
    GameRoom* getRoomByGameZoonID(int playType, int gameZoonID);

	//-------------保存、获取比赛房间-----------
	void setMatchRooms(int type, cocos2d::__Array* rooms);

	cocos2d::__Array* getMatchRooms(int type);

	MatchRoom* getMatchRoomByID(int type, int matchID);

private:
	//癞子、欢乐房间参数 type = 2 为欢乐， =4 为癞子
	cocos2d::Map<int, cocos2d::__Array*> allRooms;

	//比赛房间数据  type = 1 为满人赛， = 2 为定时赛
	cocos2d::Map<int, cocos2d::__Array*> allMatchRooms;
};


///**
// * 大厅数据
// */
//class HallData: public cocos2d::Ref
//{
//
//public:
//	HallData();
//
//	virtual ~HallData();
//
//	virtual bool init();
//
//	CREATE_FUNC(HallData);
//
//	GameZone* getZone();
//
//public:
//	GameZone *pZone;
//};

/**
 * 道具发放数据（新手礼包、签到）
 */
class PropIssueData: public cocos2d::Ref
{

public:
	PropIssueData();

	virtual ~PropIssueData();

	virtual bool init();

	CREATE_FUNC(PropIssueData);

	CC_SYNTHESIZE(int, userID, UserID);//(int4)用户ID
	CC_SYNTHESIZE(std::string, gameCash, GameCash);//(String)虚拟币数量
	CC_SYNTHESIZE(int, issueType, IssueType);//(int4) 道具发放类型1001：注册1002：签到1003：更新
	CC_SYNTHESIZE(std::string, notify, Notify);//(String)通知信息
	CC_SYNTHESIZE(int, count, Count);//(int2)道具物品数量

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, magicName, MagicName);//(String)道具名称
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, magicID, MagicID);//(int4)道具ID
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, magicCount, MagicCount);//(int4)道具数量
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, magicUnit, MagicUnit);//(String)道具单位
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, magicFID, MagicFID);//(int4)物品FID
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, magicFunType, MagicFunType);//int2)物品功能类别 1:使用  2：兑换
};

/**
 * 玩家基本资料
 */
class UserBasicInfoData:public cocos2d::Ref
{

public:
	UserBasicInfoData();

	virtual ~UserBasicInfoData();

	void setUserId(int userId);//(int4)用户ID
	int getUserId();

	void setNewRegister(int newRegister);//(int1) 0原有蛙号，1新注册蛙号
	int getNewRegister();

	void setUserPwd(std::string userPwd);//(String)用户密码|token
	std::string getUserPwd();

	void setNickName(std::string nickName);//(String)昵称  
	std::string getNickName();

	void setIconID(int iconID);//(int4)头像ID
	int getIconID();

	void setGender(int gender);//(int1)性别1-男，2－女
	int getGender();

	void setVipGrade(int vipGrade);//(int1)VIP等级.0=非会员 
	int getVipGrade();

	void setLevel(int level);//(int2)等级
	int getLevel();

	void setGameCash(std::string gameCash);//(String)金币数
	std::string getGameCash();

	void setGameCash2(std::string gameCash2);//(String)钻石数 
	std::string getGameCash2();

	void setRegisterTime(std::string registerTime);//(String)注册时间 
	std::string getRegisterTime();

	void setLastTime(std::string LastTime);//(String)上次登录时间
	std::string getLastTime();

	void setAPN(std::string apn);//(String)网络接入
	std::string getAPN();

	void setRegion(std::string region);//(String)地区
	std::string getRegion();

private:
	int userId;
	int newRegister;
	std::string userPwd;
	std::string nickName;
	int iconID;
	int gender;
	int vipGrade;
	int level;
	std::string gameCash;
	std::string gameCash2;
	std::string registerTime;
	std::string lastTime;
	std::string apn;
	std::string region;
};

/**
 * 玩家游戏资料
 */
class GameInfoData: public cocos2d::Ref
{

public:
	GameInfoData();

	virtual ~GameInfoData();

	virtual bool init();

	CREATE_FUNC(GameInfoData);

	CC_SYNTHESIZE(int, userID, UserID);//(int4)用户ID
	CC_SYNTHESIZE(std::string, nickName, NickName);//
	CC_SYNTHESIZE(int, iconID, IconID);//
	CC_SYNTHESIZE(std::string, imageFile, ImageFile);
	CC_SYNTHESIZE(int, vip, Vip);//
	CC_SYNTHESIZE(int, grade, Grade);//
	CC_SYNTHESIZE(int, cash, Cash);//
	CC_SYNTHESIZE(int, gender, Gender);
	CC_SYNTHESIZE(int, charm, Charm);//
	CC_SYNTHESIZE(int, loverID, LoverID);//
	CC_SYNTHESIZE(std::string, loverNickName, LoverNickName);//
	CC_SYNTHESIZE(std::string, tongName, TongName);
	CC_SYNTHESIZE(std::string, team, Team);
	CC_SYNTHESIZE(std::string, birthDay, BirthDay);//
	CC_SYNTHESIZE(std::string, constellation ,Constellation);
	CC_SYNTHESIZE(int, province, Province);//
	CC_SYNTHESIZE(std::string, registerDate, RegisterDate);
	CC_SYNTHESIZE(std::string, sign, Sign);//

	CC_SYNTHESIZE(int, petID, PetID);//(int4)用户ID
	CC_SYNTHESIZE(int, gameID, GameID);//
	CC_SYNTHESIZE(int, allGame, AllGame);//
	CC_SYNTHESIZE(int, winPercent, WinPercent);
	CC_SYNTHESIZE(int, terminate, Terminate);//
	CC_SYNTHESIZE(int, escape, Escape);//
	CC_SYNTHESIZE(int, gamePoint, GamePoint);//
	CC_SYNTHESIZE(int, gameLevel, GameLevel);
	CC_SYNTHESIZE(std::string, gameLevelName, GameLevelName);//
	CC_SYNTHESIZE(int, winGame, WinGame);//
	CC_SYNTHESIZE(std::string, matchHonor, MatchHonor);//
	CC_SYNTHESIZE(int, bean, Bean);//
	CC_SYNTHESIZE(int, ssCount, SSCount);//
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, sCodes, SCodes);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, sNames, SNames);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, endtimes, Endtimes);

	CC_SYNTHESIZE(std::string, gameCash, GameCash);//银子
	CC_SYNTHESIZE(int, mJFan, MJFan); //用户最大总番数
	CC_SYNTHESIZE(std::string, nickName2, NickName2);//合作方昵称
	CC_SYNTHESIZE(int, maxKeepWin, MaxKeepWin);//
	CC_SYNTHESIZE(int, keepWin, KeepWin);//
	CC_SYNTHESIZE(int, rockNum, RockNum);//
	CC_SYNTHESIZE(int, playType, PlayType);//
	CC_SYNTHESIZE(std::string, regionStr, RegionStr); //地理位置

};

/**
 * 连续登录天数和抽奖次数信息
 */
class LogonAwardInfoData: public cocos2d::Ref
{

public:
	LogonAwardInfoData();

	virtual ~LogonAwardInfoData();

	virtual bool init();

	CREATE_FUNC(LogonAwardInfoData);

	CC_SYNTHESIZE(int, dayCount, DayCount);//(int4)连续登录天数
	CC_SYNTHESIZE(int, awardCount, AwardCount);//int2)剩余抽奖次数
	CC_SYNTHESIZE(std::string, lastTime, LastTime);//(String)最后签到时间:yyyy-mm-dd
	CC_SYNTHESIZE(int, count, Count);//(int2)签到天数奖励表项目数量
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, day, Day);//(int1)天数
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, gameCash, GameCash);//(int4)奖励虚拟游戏币
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, crytalStone, CrytalStone);//int2)奖励的晶石

};

/**
 *用户形象
 */
class MediaContentInfoData: public cocos2d::Ref
{
public:
	MediaContentInfoData();

	virtual ~MediaContentInfoData();

	virtual bool init();

	CREATE_FUNC(MediaContentInfoData);

	CC_SYNTHESIZE(int, type, Type);
	CC_SYNTHESIZE(int, count, Count);
	CC_SYNTHESIZE(int, parameter, Parameter);
	CC_SYNTHESIZE(int, imageSize, ImageSize);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, imageIds, ImageIds);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, mediaType, MediaType);
    
    //cocos2d::Vector<WWBuffer *> mediaContents;

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, prices, Prices);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, parameter2s, Parameter2s);

};

//恢复对局通知数据
class RecoveryGameNotifyData : public cocos2d::Ref
{
public:
	RecoveryGameNotifyData();
	virtual ~RecoveryGameNotifyData();

	virtual bool init();
	CREATE_FUNC(RecoveryGameNotifyData);

	CC_SYNTHESIZE(int, m_gameID, GameID);	//游戏ID
	CC_SYNTHESIZE(int, m_gameZoneID, GameZoneID);	//游戏区ID
	CC_SYNTHESIZE(int, m_gamePlayID, GamePlayID);	//对局ID
	CC_SYNTHESIZE(int, m_playType, PlayType);	//玩法
	CC_SYNTHESIZE(int, m_zoneWin, ZoneWin);	//对局输赢
	CC_SYNTHESIZE(int, m_fortuneBase, FortuneBase);	//财富基数
	CC_SYNTHESIZE(int, m_pointBase, PointBase);	//积分计算的底数
	CC_SYNTHESIZE(int, m_multiple, Multiple);	//倍数
	CC_SYNTHESIZE(int, m_moneyBox, MoneyBox);	//0：无宝箱 1：有20局宝箱
	CC_SYNTHESIZE(int, m_fortuneMin, FortuneMin);	//准入限制
	CC_SYNTHESIZE(int, m_fortuneMax, FortuneMax);   //准入限制，最大值
	CC_SYNTHESIZE(std::string, m_roomName, RoomName); //房间名称
};

class ResultInfoData : public cocos2d::Ref
{
public:
    ResultInfoData();
    virtual ~ResultInfoData();
    
    virtual bool init();
	CREATE_FUNC(ResultInfoData);
    
    CC_SYNTHESIZE(int, m_UserID, UserID);
    CC_SYNTHESIZE(int, m_Type, Type);
    CC_SYNTHESIZE(int, m_Result, Result);
    CC_SYNTHESIZE(std::string, m_Description, Description);
    CC_SYNTHESIZE(int, m_Parameter, Parameter);
	CC_SYNTHESIZE(std::string, m_Parameter2, Parameter2);
    CC_SYNTHESIZE(std::string, m_Parameter3, Parameter3);
};

class NoticeList: public cocos2d::Ref
{
public:
	NoticeList();
    
	virtual ~ NoticeList();
    
	virtual bool init();
    
	CREATE_FUNC(NoticeList);
    
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, m_msgID, MsgID);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, m_owner, Owner);
    WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, m_ownerId, OwnerID);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, m_modifyTime, ModifyTime);
    WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, m_subject, Subject);
    
};

class NoticeContent: public cocos2d::Ref
{
public:
	NoticeContent();
    
	virtual ~ NoticeContent();
    
	virtual bool init();
    
	CREATE_FUNC(NoticeContent);
    
	CC_SYNTHESIZE(std::string, m_content, Content);
    CC_SYNTHESIZE(int, m_response, Response);
    CC_SYNTHESIZE(int, m_haveNew, HaveNew);
    CC_SYNTHESIZE(int, m_msgid, MsgId);
    CC_SYNTHESIZE(int, m_boxId, BoxId);
};

#endif /* GD_H_ */

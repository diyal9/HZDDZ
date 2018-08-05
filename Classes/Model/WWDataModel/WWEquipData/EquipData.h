//
//  EquipData.h
//  WW_DDZ_V2
//
//  Created by 王体强 on 1/30/15.
//
//

#ifndef __WW_DDZ_V2__EquipData__
#define __WW_DDZ_V2__EquipData__

#include <stdio.h>
#include "cocos2d.h"
//#include "WWPlatform/ww_cc_platform.h"

/*物品箱菜单*/

//物品简要消息
/*UserEquipID(n)	(int4)用户物品ID
 EquipID(n)	(int4)物品ID
 EquipCount(n)	(int4)物品数量
 Name(n)	(String)物品名称
 ExpireTime(n) 	(String) 有效期
 Fid(n)	(int4)功能ID
 MagicType   (int1) 1普通道具 9 道具包
*/
class EquipInfo:public cocos2d::Ref
{
public:
    EquipInfo();
    virtual ~EquipInfo();
    virtual bool init(){return true;};
    
    CREATE_FUNC(EquipInfo);
    //    Name(n)	(String)物品名称
    //    ExpireTime(n) 	(String) 有效期
    //    Fid(n)	(int4)功能ID
    CC_SYNTHESIZE(std::string, Name, Name);
    CC_SYNTHESIZE(std::string, ExpireTime, ExpireTime);
    CC_SYNTHESIZE(int, Fid, Fid);
    CC_SYNTHESIZE(int, EquipCount, EquipCount);
    CC_SYNTHESIZE(int, EquipID, EquipID);
    CC_SYNTHESIZE(int, UserEquipID, UserEquipID);
    CC_SYNTHESIZE(uint8_t, MagicType, MagicType);
    
};

//物品详细说明信息
class GameEquipInfo:public cocos2d::Ref
{
public:
    GameEquipInfo();
    virtual ~GameEquipInfo();
    virtual bool init(){return true;};
    
    CREATE_FUNC(GameEquipInfo);
    //    EquipID	(int4)物品ID
    //    Desc	(String)物品的详细说明描述
    //    introduce	(String)物品介绍
    //    EquipName	(String)物品名称
    //    EquipCount	(int4）物品总数量
    //                 count	(int2)有效期记录数
    //                 subEquipCount(n)	(int4)同一有效期的物品数量
    //                 Expire(n)	(String)有效期
    CC_SYNTHESIZE(int, Fid, Fid);
    CC_SYNTHESIZE(int, EquipID, EquipID);
    CC_SYNTHESIZE(std::string, Desc, Desc);
    CC_SYNTHESIZE(std::string, introduce, introduce);
    CC_SYNTHESIZE(std::string, EquipName, EquipName);
    CC_SYNTHESIZE(int, EquipCount, EquipCount);
    CC_SYNTHESIZE(short, count, count);
    CC_SYNTHESIZE(int, subEquipCount, subEquipCount);
    CC_SYNTHESIZE(std::string, Expire, Expire);
    CC_SYNTHESIZE(uint8_t, MagicType, MagicType);
};
////物品箱列表
class UserMatchEquipList:public cocos2d::Ref
{
public:
    UserMatchEquipList();
    virtual ~UserMatchEquipList();
    virtual bool init(){return true;};
    
    CREATE_FUNC(UserMatchEquipList);
    //    UserID	(int4)玩家蛙号
    //    GameID	(int4)游戏ID
    //    Count	(int2)各类门票数量
    //    UserEquipID(n)	(int4)用户物品ID
    //    EquipID(n)	(int4)物品ID
    //    EquipCount(n)	(int4)物品数量
    
    CC_SYNTHESIZE(int, m_UserID,UserID);
    CC_SYNTHESIZE(int, m_GameID, GameID);
    CC_SYNTHESIZE(short, m_Count, Count);
    
    //物品详细信息列表
    CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vector<EquipInfo*>, m_equipVec, EquipVec);
    
    
    
    
};
////物品箱说明信息
//class MatchEquipInfo:public cocos2d::Ref
//{
//public:
//    
//    virtual bool init(){return true;};
//    
//    CREATE_FUNC(MatchEquipInfo);
//    
//    //    EquipID	(int4)物品ID
//    //    Desc	(String)物品的详细说明描述
//    //    introduce	(String)物品介绍
//    CC_SYNTHESIZE(int, EquipID, EquipID);
//    
//    CC_SYNTHESIZE(std::string, Desc, Desc);
//    
//    CC_SYNTHESIZE(std::string, introduce, introduce);
//};




class UserMatchEquipReq:public cocos2d::Ref
{
public:
    
    virtual bool init(){return true;};
    
    CREATE_FUNC(UserMatchEquipReq);
    
    CC_SYNTHESIZE(uint8_t, m_type, Type);
    
    CC_SYNTHESIZE(int, m_gameID, GameID);
    
    CC_SYNTHESIZE(int, m_objectID, ObjectID);
    
    
};

class PerMagicInfo:public cocos2d::Ref
{
public:
    virtual bool init(){return true;};
    
    CREATE_FUNC(PerMagicInfo);
    //    Name(n)	(String)道具名称
    //    ExpireTime(n)	(String)有效期
    //    Fid(n)	(int4)功能ID
    //    Price(n)	(int4)价格
    //    Desc(n)	(String)描述
    CC_SYNTHESIZE(std::string, Name, Name);
    
    CC_SYNTHESIZE(std::string, ExpireTime, ExpireTime);
    
    CC_SYNTHESIZE(int, Fid, Fid);
    
    CC_SYNTHESIZE(int, Price, Price);
    
    CC_SYNTHESIZE(std::string, Desc, Desc);
};




/*消息类别	(byte)17其他功能
 应用类别	(Byte)11游戏道具商店
 消息标识	(byte)1
 消息方向	Client->Server
 用途	请求道具商店的商品数据
 属性	Type	(int1)0=商店商品列表
	GameID	(int4)游戏ID
	ObjectID	(int4) 商店ID
 1001=比赛会员商店,
 1002=比赛门票商店
 1003：游戏会员商店
 1004：游戏对局中道具价格表
 1005：单机游戏道具价格表
	CashType	（int1）1 – 游戏币购买
 说明	Type=0时ObjectID=商店ID,( 1001=比赛会员商店,1002=比赛门票商店,1003=游戏会员商店)
*/
//商店物品列表请求,以及道具使用请求
class MagicStoreReq:public cocos2d::Ref
{
public:
    MagicStoreReq();
    virtual ~MagicStoreReq();
    virtual bool init(){return true;};
    CREATE_FUNC(MagicStoreReq);
    
    CC_SYNTHESIZE(char, Type, Type);
    CC_SYNTHESIZE(int, GameID, GameID);
    CC_SYNTHESIZE(int, ObjectID, ObjectID);
    CC_SYNTHESIZE(char, CashType, CashType);
    
};

/*GameID	(int4)游戏ID
 StoreID	(int4) 商店ID
 1001=比赛会员商店,1002=比赛门票商店
 Count	(int2)商品数量
 StoreMagicID(n)	(int4)商店道具ID
 MagicID(n)	(int4)道具ID
 Money(n)	(int4)价格,单位分
 Name(n)	(String)商品名称
 Description(n)	(String)描述文字
 Introduce(n)	(String)简介
	
 magicCount(n)	(int4) 物品道具、数量
 fid(n)	(int4) 功能ID
*/
//商店物品列表
class StoreMagicList:public cocos2d::Ref
{
public:
    StoreMagicList();
    virtual ~StoreMagicList();
    
    CREATE_FUNC(StoreMagicList);
    
    virtual bool init(){return true;};
    
    CC_SYNTHESIZE(int, GameID, GameID);
    CC_SYNTHESIZE(int, StoreID, StoreID);
    CC_SYNTHESIZE(short, Count, Count);
    
    CC_SYNTHESIZE(std::vector<int>, StoreMagicID, StoreMagicID);
    CC_SYNTHESIZE(std::vector<int>, MagicID, MagicID);
    CC_SYNTHESIZE(std::vector<int>, Money, Money);
    CC_SYNTHESIZE(std::vector<std::string>, Name, Name);
    CC_SYNTHESIZE(std::vector<std::string>, Description, Description);
    CC_SYNTHESIZE(std::vector<std::string>, Introduce, Introduce);
    CC_SYNTHESIZE(std::vector<int>, magicCount, magicCount);
    CC_SYNTHESIZE(std::vector<int>, fid, fid);
    CC_SYNTHESIZE(std::vector<std::string>, expire, expire);
    
    
};

//互动道具物品箱以及道具使用
/*请求用户互动道具箱数据,及操作
 Type	(int1)0=物品箱物品列表
 1=使用互动道具
 GameID	(int4)游戏ID
 TargetID	(int4)目前标ID
 ObjectID	(int4)物品ID
 StageID	(int4)使用场景ID
 Type=0, TargetID =0,ObjectID=0,StageID=0
 Type=1时TargetID =目标人物ID,ObjectID=物品ID(-1时为会员特有道具）, StageID=牌桌ID
*/
class UserInteractiveEquipReq:public cocos2d::Ref
{
public:
    UserInteractiveEquipReq();
    virtual ~UserInteractiveEquipReq();
    
    CREATE_FUNC(UserInteractiveEquipReq);
    
    virtual bool init(){return true;};
    CC_SYNTHESIZE(char, Type, Type);
    CC_SYNTHESIZE(int, GameID, GameID);
    CC_SYNTHESIZE(int, TargetID, TargetID);
    CC_SYNTHESIZE(int, ObjectID, ObjectID);
    CC_SYNTHESIZE(int, StageID, StageID);
};

//互动道具list
/*
 UserID	(int4)玩家蛙号
 GameID	(int4)游戏ID
 Count	(int2) 互动道具数量
 EquipID(n)	(int4)物品ID
 Price(n)	(int4)价格
 picFile(n)	(String)图片文件名
 Fid(n)	(int4)功能ID
 EquipCount(n)	(int4)物品数量
*/
class UserInteractiveEquipList:public cocos2d::Ref
{
public:
    UserInteractiveEquipList();
    virtual ~UserInteractiveEquipList();
    
    CREATE_FUNC(UserInteractiveEquipList);
    
    virtual bool init(){return true;};
    
    CC_SYNTHESIZE(int, UserID, UserID);
    CC_SYNTHESIZE(int, GameID, GameID);
    CC_SYNTHESIZE(short, Count, Count);
    CC_SYNTHESIZE(std::vector<int>, EquipID, EquipID);
    CC_SYNTHESIZE(std::vector<int>, Price, Price);
    CC_SYNTHESIZE(std::vector<std::string>, picFile, picFile);
    CC_SYNTHESIZE(std::vector<int>, Fid, Fid);
    CC_SYNTHESIZE(std::vector<int>, EquipCount, EquipCount);
};

//道具使用返回
/*
 用户互动道具使用返回
 UserID	(int4)使用玩家蛙号
 TargetID	(int4) 目标玩家蛙号
 Cash	(String) 玩家当前帐号金额
 MagicID	(int4) 互动道具ID（-1会员道具）
 Fid	(int4)互动道具功能ID（magicID=-1时，Fid=会员ServiceCoder)
 Count	(int4)剩余道具数量
 actionType	(int1)使用类型（1=使用自己身上道具，2购买使用,3=会员道具）
 */
class UserInteractiveAcResp:public cocos2d::Ref
{
public:
    UserInteractiveAcResp();
    virtual ~UserInteractiveAcResp();
    
    CREATE_FUNC(UserInteractiveAcResp);
    
    virtual bool init(){return true;};
    
    CC_SYNTHESIZE(int, UserID, UserID);
    CC_SYNTHESIZE(int, TargetID, TargetID);
    CC_SYNTHESIZE(std::string, Cash, Cash);
    CC_SYNTHESIZE(int, MagicID, MagicID);
    CC_SYNTHESIZE(int, Fid, Fid);
    CC_SYNTHESIZE(int, Count, Count);
    CC_SYNTHESIZE(char, actionType, actionType);
};
/*
 我的道具信息查询  最新
 UserID[int4]	用户ID
 GameID[int4]	游戏ID
 CashType[int1]	0 蛙币 1晶石 2蛙豆
 StoreID	(int4) 商店ID
 1001=比赛会员商店,
 1002=比赛门票商店
 1003：游戏会员商店
 1004：游戏对局中道具价格表
 1006：IOS+COCOS道具商店1
 1007：Android道具商店1
 
*/
class ReqestMagicInfo:public cocos2d::Ref
{
public:
    ReqestMagicInfo();
    virtual ~ReqestMagicInfo();
    CREATE_FUNC(ReqestMagicInfo);
    virtual bool init(){return true;};
    
    CC_SYNTHESIZE(int, UserID, UserID);
    CC_SYNTHESIZE(int, GameID, GameID);
    CC_SYNTHESIZE(char, CashType, CashType);
    CC_SYNTHESIZE(int, StoreID, StoreID);
};

/*
 我的道具信息结果 RespMagicInfo
 消息类别	00010001其他功能
 应用类别	00000011 道具系统
 消息标识	00000111
 消息方向	Server->Client
 用途	我的道具信息查询结果
 属性	UserID	(int4)玩家蛙号
	GameID	(int4)游戏ID
	CashType	(int1)道具的货币类型 0 蛙币 1晶石 2蛙豆
	Count	(int2)各类道具数量
	MagicID(n)	(int4)道具ID
	MagicCount(n)	(int4)道具数量
	Name(n)	(String)道具名称
	ExpireTime(n)	(String)有效期
	Fid(n)	(int4)功能ID
	Price(n)	(int4)价格
	Desc(n)	(String)描述
 */

class RespMagicInfo:public cocos2d::Ref
{
public:
    RespMagicInfo();
    virtual ~RespMagicInfo();
    CREATE_FUNC(RespMagicInfo);
    virtual bool init(){return true;};
    
    CC_SYNTHESIZE(int, UserID, UserID);
    CC_SYNTHESIZE(int, GameID, GameID);
    CC_SYNTHESIZE(uint8_t, CashType, CashType);
    CC_SYNTHESIZE(short, Count, Count);
    CC_SYNTHESIZE(std::vector<int>, MagicID, MagicID);
    CC_SYNTHESIZE(std::vector<int>, MagicCount, MagicCount);
    CC_SYNTHESIZE(std::vector<std::string>, Name, Name);
    CC_SYNTHESIZE(std::vector<std::string>, ExpireTime, ExpireTime);
    CC_SYNTHESIZE(std::vector<int>, Fid, Fid);
    CC_SYNTHESIZE(std::vector<int>, Price, Price);
    CC_SYNTHESIZE(std::vector<std::string>, Desc, Desc);
};


/*购买道具请求
 消息名称	BuyMagicReq
 消息类别	00010001其他功能
 应用类别	00000011 道具系统
 消息标识	000000001
 消息方向	Client->Server
 用途	用户购买道具
 属性	MagicID [int4]	道具ID;见说明
	Price[int4]	道具价格
	Count[int2]	购买数量
	DestUserID[int4]	道具使用目标用户ID，对即买即用类游戏道具有效
	GameID[int4]	游戏ID，游戏道具有效
	PlayID[int4]	用户当前游戏对局ID,对游戏道具有效
	Mid	(String)
	CashType	(int1) 0 蛙币 1晶石 2 蛙豆 3水晶
	playType	(int1) 购买使用道具的游戏玩法,如果道具与玩法无关，填写0
 StoreID	(int4) 商店ID
 1001=比赛会员商店,
 1002=比赛门票商店
 1003：游戏会员商店
 1004：游戏对局中道具价格表
 1006：IOS+COCOS道具商店1
 1007：Android道具商店1
 
 605-回看道具,606-禁言道具,607-猪头道具,608-双倍战斗力道具,
 701-双倍经验值道具,603-连连看提示道具,604-连连看重列道具
 617－麻将回看道具, 101好友控，102聊聊控
 晶石购买道具
 10011101-回看道具 10011102-禁言道具 10011201 负分清零之标准玩法
 

 */

class BuyMagicReq:public cocos2d::Ref
{
public:
    BuyMagicReq();
    virtual ~BuyMagicReq();
    CREATE_FUNC(BuyMagicReq);
    virtual bool init(){return true;};
    
    CC_SYNTHESIZE(int, MagicID, MagicID);
    CC_SYNTHESIZE(int, Price, Price);
    CC_SYNTHESIZE(short, Count, Count);
    CC_SYNTHESIZE(int, DestUserID, DestUserID);
    CC_SYNTHESIZE(int, GameID, GameID);
    CC_SYNTHESIZE(int, PlayID, PlayID);
    CC_SYNTHESIZE(std::string, Mid, Mid);
    CC_SYNTHESIZE(uint8_t, CashType, CashType);
    CC_SYNTHESIZE(uint8_t, playType, playType);
    CC_SYNTHESIZE(int, StoreID, StoreID);
    
};


/*
 消息名称	BuyMagicResp
 消息类别	00010001其他功能
 应用类别	00000011 道具系统
 消息标识	000000010
 消息方向	Server->Client
 用途	系统回复用户购买道具结果
 属性	MagicID [int4]	道具ID
	DestUserID[int4]	道具使用目标用户ID，对即买即用类游戏道具有效
	BuyPrice[int4]	道具的支付总价
	result[int1]	0=成功；1＝用户帐户余额不足；11=物品不存在或者已经售馨; -1=其它异常
	SpareTime[int4]	剩余有效时间(毫秒)
	UseCash	(int4)用户当前剩余财富
	Desc	(String) 	描述信息
	CashType[int1]	0 蛙币 1 晶石 2蛙豆
 说明	仅当系统道具且result＝0时sparetime有效
 */
class BuyMagicResp:public cocos2d::Ref
{
public:
    BuyMagicResp();
    virtual ~BuyMagicResp();
    CREATE_FUNC(BuyMagicResp);
    virtual bool init(){return true;};
    
    CC_SYNTHESIZE(int, MagicID, MagicID);
    CC_SYNTHESIZE(int, DestUserID, DestUserID);
    CC_SYNTHESIZE(int, BuyPrice, BuyPrice);
    CC_SYNTHESIZE(char, result, result);
    CC_SYNTHESIZE(int, SpareTime, SpareTime);
    CC_SYNTHESIZE(int, UseCash, UseCash);
    CC_SYNTHESIZE(std::string, Desc, Desc);
    CC_SYNTHESIZE(char, CashType, CashType);
    CC_SYNTHESIZE(int, BuyCount, BuyCount);//新增购买数量
};
/*
 请求打开道具宝箱
 MoneyBoxID	(int4)宝箱的ID
 Type	(int1)请求类型
 Mid	(String) 用户网游伪码
 GameID	(int4) 游戏ID
 BoxType	(int1)道具宝箱类型
 0=原对局宝箱
 1=首胜宝箱
 2=对局宝箱
 3=道具宝箱
 
	
 Type=1 广东宝盒  Type=2 在线宝箱//33
 Type=3 开道具宝箱，MoneyBoxID=用户对局宝箱ID
 Type=4 系统道具宝箱列表, 返回6:4:1
 Type=5 玩家当前道具宝箱数量, 返回通用消息
 
 */
class OpenBoxData:public cocos2d::Ref
{
public:
    OpenBoxData():MoneyBoxID(0),
    Type(0),GameID(0),BoxType(0)
    {};
    virtual ~OpenBoxData(){};
    
    CREATE_FUNC(OpenBoxData);
    virtual bool init(){return true;};
    
    CC_SYNTHESIZE(int, MoneyBoxID, MoneyBoxID);
    CC_SYNTHESIZE(char, Type, Type);
    CC_SYNTHESIZE(std::string, Mid, Mid);
    CC_SYNTHESIZE(int, GameID, GameID);
    CC_SYNTHESIZE(char, BoxType, BoxType);
};
/*
 开财富游戏满20局系统奖励给用户的宝箱结果
 MoneyBoxID	(int4)宝箱的ID
 Result	(int2) 0=成功；1=无效的服务代码/参数； 10=不支持, 21=物品不存在， -1=异常
 100 广东宝盒未中奖
 101广东宝盒中小奖(宝盒一等奖)
 102 广东宝盒中大奖(宝盒二等奖)
 103 (宝盒三等奖)
 Money	(int4)宝箱中的蛙币数量
 correspondence	(int4) 中奖的话费数量(分)
 desc	(String)描述内容
 Count	(int2)道具物品数量
 MagicName(n)	(String)道具名称
 MagicID(n)	(int4)道具ID
 MagicCount(n)	(int4)道具数量
 MagicFID(n)	(int4)物品FID
 */
class BoxDataResp:public cocos2d::Ref
{
public:
    BoxDataResp(){};
    virtual ~BoxDataResp(){};
    
    CREATE_FUNC(BoxDataResp);
    virtual bool init(){return true;};
    
    CC_SYNTHESIZE(int, MoneyBoxID, MoneyBoxID);
    CC_SYNTHESIZE(short, Result, Result);
    CC_SYNTHESIZE(int, Money, Money);
    CC_SYNTHESIZE(int, correspondence, correspondence);
    CC_SYNTHESIZE(std::string, desc, desc);
    CC_SYNTHESIZE(short, Count, Count);
    CC_SYNTHESIZE(std::vector<std::string>, MagicName, MagicName);
    CC_SYNTHESIZE(std::vector<int>, MagicID, MagicID);
    CC_SYNTHESIZE(std::vector<int>, MagicCount, MagicCount);
    CC_SYNTHESIZE(std::vector<int>, MagicFID, MagicFID);
    
    
};

#endif /* defined(__WW_DDZ_V2__EquipData__) */

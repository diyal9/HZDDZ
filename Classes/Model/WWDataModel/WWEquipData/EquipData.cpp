//
//  EquipData.cpp
//  WW_DDZ_V2
//
//  Created by 王体强 on 1/30/15.
//
//

#include "EquipData.h"
MagicStoreReq::MagicStoreReq():
Type(0),
GameID(0),
ObjectID(0),
CashType(0)
{
    
}

MagicStoreReq::~MagicStoreReq()
{
    
}



StoreMagicList::StoreMagicList():
GameID(0),
StoreID(0),
Count(0),
StoreMagicID(0),
MagicID(0),
Money(0),
magicCount(0),
fid(0)
{
    
}

StoreMagicList::~StoreMagicList()
{
    
}



/**物品箱**********************/
//物品箱列表
//UserMatchEquipList::UserMatchEquipList():
//m_UserID(0),
//m_GameID(0),
//m_Count(0),
//UserEquipID(0),
//EquipID(0),
//EquipCount(0)
//
//{
//    
//}
//UserMatchEquipList::~UserMatchEquipList()
//{
//    
//}
//物品基本信息
EquipInfo::EquipInfo():
Fid(0)

{
    
}
EquipInfo::~EquipInfo()
{
    
}
//物品详细信息
GameEquipInfo::GameEquipInfo():
EquipID(0),
EquipCount(0),
count(0),
subEquipCount(0)
{
    
}
GameEquipInfo::~GameEquipInfo()
{
    
}

//互动道具列表
UserInteractiveEquipList::UserInteractiveEquipList():
UserID(0),
GameID(0),
Count(0)
{
    
}

UserInteractiveEquipList::~UserInteractiveEquipList()
{
    
}

//互动道具使用
UserInteractiveEquipReq::UserInteractiveEquipReq():
Type(0),
GameID(0),
TargetID(0),
ObjectID(0),
StageID(0)
{
    
}

UserInteractiveEquipReq::~UserInteractiveEquipReq()
{
    
}


//我的道具查询 游戏中使用
ReqestMagicInfo::ReqestMagicInfo():
UserID(0),
GameID(0),
CashType(0),
StoreID(0)
{
    
}

ReqestMagicInfo::~ReqestMagicInfo()
{
    
}

//我的道具返回结果 游戏中使用
RespMagicInfo::RespMagicInfo():
UserID(0),
GameID(0),
CashType(0),
Count(0)
{
    
}

RespMagicInfo::~RespMagicInfo()
{
    
}

BuyMagicReq::BuyMagicReq():
MagicID(0),
Price(0),
Count(0),
DestUserID(0),
GameID(0),
PlayID(0),
Mid(""),
CashType(0),
playType(0),
StoreID(0)
{
    
}

BuyMagicReq::~BuyMagicReq()
{
    
}

UserMatchEquipList::UserMatchEquipList()
{
    
}

UserMatchEquipList::~UserMatchEquipList()
{
    
}

BuyMagicResp::BuyMagicResp():
MagicID(0),
DestUserID(0),
BuyPrice(0),
result(0),
SpareTime(0),
UseCash(0),
CashType(0),
BuyCount(0)
{
    
}

BuyMagicResp::~BuyMagicResp()
{
    
}


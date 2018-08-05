//
//  LeaderboardData.h
//  WW_DDZ_V2
//
//  Created by wildcat on 23/1/15.
//
//  排行榜

#ifndef __WW_DDZ_V2__LEADERBOARDDATA__
#define __WW_DDZ_V2__LEADERBOARDDATA__

#include "cocos2d.h"

//#include "WWPlatform/ww_cc_platform.h"

class LeaderboardData : public cocos2d::Ref
{
public:
    enum eLeaderboardType
    {
        eLeaderboardType_earn = 38, //每日赚豆榜
        eLeaderboardType_charge = 39, //每日充值榜
        eLeaderboardType_richer = 40 //上周土豪榜
    };
    
    LeaderboardData();
    
    virtual ~LeaderboardData();
    
    virtual bool init();
    
    CREATE_FUNC(LeaderboardData);
    
    CC_SYNTHESIZE(int, m_topType, TopType); //排行榜类型
    
    CC_SYNTHESIZE(std::string, m_timeStr, TimeStr); //(String)截止时间
    
    CC_SYNTHESIZE(int, m_myNo, MyNo); //(int4)我的排名
    
    CC_SYNTHESIZE(std::string, m_myScore, MyScore); //(String)我的数据
    
    CC_SYNTHESIZE(int16_t, m_count, Count); //(int2)分组数量
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, m_vNo, vNo); //(int4)排名
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, m_vUserID, vUserID); //(int4)蛙号
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, m_vNickname, vNickname); //(String)昵称
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, m_vProvince, vProvince); //(String)地区
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, m_vScore, vScore); //(String)成绩
    
    CC_SYNTHESIZE(int16_t, m_iconCount, IconCount); //(int2)分组数量

    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, m_vIconID, vIconID); //(int4)头像ID
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, m_vIconTS, vIconTS); //(String)头像时间标签
    
};

#endif /* defined(__WW_DDZ_V2__LEADERBOARDDATA__) */

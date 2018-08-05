//
//  LeaderboardData.cpp
//  WW_DDZ_V2
//
//  Created by wildcat on 23/1/15.
//
//

#include "LeaderboardData.h"

LeaderboardData::LeaderboardData() :
        m_topType(0),
        m_timeStr(""),
        m_myNo(0),
        m_myScore(""),
        m_count(0),
        m_iconCount(0)
{
    
}

LeaderboardData::~LeaderboardData()
{
    
}

bool LeaderboardData::init()
{
    return true;
}
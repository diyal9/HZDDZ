/*
 * GameCommData.cpp
 *	功能描述：
 *  Created on: 2014年6月30日 下午7:33:40
 *      Author: hp
 */

#include "GameCommonData.h"

GameSubstituteData::GameSubstituteData() : userID(0), gameId(0), gameZoneID(0), gameplayID(0), type(0)
{

}

GameSubstituteData::~GameSubstituteData()
{
}

bool GameSubstituteData::init()
{
	return true;
}

GameChatData::GameChatData() : gamePlayID(0), fromUserID(0), type(0), chatContent(""), userNick(""),toUserID(0)
{

}

GameChatData::~GameChatData()
{
}

bool GameChatData::init()
{
	return true;
}

GameOverCommonData::GameOverCommonData() : gameId(0), gamePlayId(0), playerCount(0), beanTax(0)
{

}

GameOverCommonData::~GameOverCommonData()
{
}

bool GameOverCommonData::init()
{
	return true;
}


PropUseInfoData::PropUseInfoData()
{

}

PropUseInfoData::~PropUseInfoData()
{
}

bool PropUseInfoData::init()
{
	return true;
}

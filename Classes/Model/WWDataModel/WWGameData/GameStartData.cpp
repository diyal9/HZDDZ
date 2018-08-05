/*
 * GameStartData.cpp
 *	功能描述：
 *  Created on: 2015年5月12日 下午2:09:49
 *      Author: zx
 */

#include "GameStartData.h"

GameStartData::GameStartData() :
	gameType(1),gameId(0), gamePlayId(0), playTimeOut(0), doubleTime(10), firstPlayer(0), taskNum4Award(0), taskEachNumAward(0), playedNum4Award(0), 
	fortuneBase(0), laizi(0), playerCount(0), isResume(false), nextPlayUserID(0),lordId(0),mingPai(false),
	bankScore(0), gameStatus(0),lastPlayerID(0), boomCount(0)
{

}

GameStartData::~GameStartData()
{

}

bool GameStartData::init()
{
	return true;
}




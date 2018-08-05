/*
 * DataCenter.cpp
 *	功能描述：
 *  Created on: 2014年6月26日 下午2:09:49
 *      Author: hp
 */
#include "WWDataManager.h"

USING_NS_CC;

WWDataManager::WWDataManager() : 
	gameZone(nullptr),
	gameRoom(nullptr),
	userBasicInfoData(nullptr),
	m_GameStartData(nullptr)
{
}

WWDataManager::~WWDataManager()
{
	log("WWDataManager::~WWDataManager-----------------------------------------------------");
	CC_SAFE_RELEASE_NULL(gameRoom);
	CC_SAFE_RELEASE_NULL(userBasicInfoData);
	CC_SAFE_RELEASE_NULL(gameZone);
	CC_SAFE_RELEASE_NULL(m_GameStartData);
}

//void WWDataManager::setGameRoom(GameRoom* gameRoom)
//{
//	gameRoom->retain();
//	this->gameRoom->release();
//	this->gameRoom = gameRoom;
//}
//
//GameRoom* WWDataManager::getGameRoom()
//{
//	return this->gameRoom;
//}


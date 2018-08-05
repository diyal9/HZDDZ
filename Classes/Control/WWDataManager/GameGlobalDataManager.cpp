/*
 * GameGlobalDataManager.cpp
 *	功能描述：单例，存储游戏全局公共数据中心
 *  Created on: 2015年6月17日 下午2:09:49
 *      Author: Jacky
 */
#include "GameGlobalDataManager.h"

USING_NS_CC;

GameGlobalDataManager::GameGlobalDataManager() : 
	m_StartGameType(StartGameType::SEND_NOMAL),
	m_LoginFromType(LoginType::LOGIN_FROM_ICON),
	m_CurSceneType(GameSceneType::LOGIN_SCENE),
	m_CurGameId(0),
	m_CurPlayId(0),
	m_PlayRoomId(0)
{
}

GameGlobalDataManager::~GameGlobalDataManager()
{
	m_StartGameType = StartGameType::SEND_NOMAL;
	m_LoginFromType = LoginType::LOGIN_FROM_ICON;
	m_CurSceneType = GameSceneType::LOGIN_SCENE;
	m_CurGameId = 0;
	m_CurPlayId = 0;
}

void GameGlobalDataManager::setStartGameType(int type)
{
	m_StartGameType = type;
}

int GameGlobalDataManager::getStartGameType()
{
	return m_StartGameType;
}

void GameGlobalDataManager::setLoginFromType(int type)
{
	m_LoginFromType = type;
}

int GameGlobalDataManager::getLoginFromType()
{
	return m_LoginFromType;
}

void GameGlobalDataManager::setCurSceneType(int type)
{
	m_CurSceneType = type;
}

int GameGlobalDataManager::getCurSceneType()
{
	return m_CurSceneType;
}

void GameGlobalDataManager::clearCurGameData()
{
	m_CurGameId = 0;
	m_CurPlayId = 0;
}

void GameGlobalDataManager::setCurGameId(int gameId)
{
	m_CurGameId = gameId;
}

int GameGlobalDataManager::getCurGameId()
{
	return m_CurGameId;
}
	
void GameGlobalDataManager::setCurPlayId(int playId)
{
	m_CurPlayId = playId;
}
int GameGlobalDataManager::getCurPlayId()
{
	return m_CurPlayId;
}

void  GameGlobalDataManager::setPlayRoomId(int roomId)
{
	this->m_PlayRoomId = roomId;
}
int  GameGlobalDataManager::getPlayRoomId()
{
	return this->m_PlayRoomId;
}

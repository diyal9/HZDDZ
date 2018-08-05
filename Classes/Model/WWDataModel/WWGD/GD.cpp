/*
 * GD.cpp
 *	功能描述：
 *  Created on: 2014年6月24日 下午5:09:36
 *      Author: hp
 */

#include "GD.h"
USING_NS_CC;

GameZone::GameZone():
	allRooms(NULL),
	allMatchRooms(NULL)
{
	//allRooms = Map<int,cocos2d::__Array*>();
}

GameZone::~GameZone()
{
	if(allRooms.size() > 0)
	{
		allRooms.clear();
	}
	if(allMatchRooms.size() > 0)
	{
		allMatchRooms.clear();
	}
}

bool GameZone::init()
{

	return true;
}

void GameZone::setRooms(int playType, cocos2d::__Array* rooms)
{
	allRooms.erase(playType);
	allRooms.insert(playType,rooms);
}

cocos2d::__Array* GameZone::getRooms(int playType)
{
	return allRooms.at(playType);
}

GameRoom * GameZone::getRoom(int playType, int zoneMuti)
{
	cocos2d::__Array* array = getRooms(playType);
	Ref* obj = nullptr;
	CCARRAY_FOREACH(array, obj){
		GameRoom* data = (GameRoom*) obj;
		if (data->getMultiple() == zoneMuti)
		{
			return data;
		}
	};
	return nullptr;
}

//add on 20141215 wildcat
GameRoom * GameZone::getRoomByGameZoonID(int playType, int gameZoonID)
{
    cocos2d::__Array* array = getRooms(playType);
	Ref* obj = nullptr;
	CCARRAY_FOREACH(array, obj){
		GameRoom* data = (GameRoom*) obj;
		if (data->getGZID() == gameZoonID)
		{
			return data;
		}
	};
    return nullptr;
}

void GameZone::setMatchRooms(int type, cocos2d::__Array* rooms)
{
	allMatchRooms.erase(type);
	allMatchRooms.insert(type,rooms);
}

cocos2d::__Array* GameZone::getMatchRooms(int type){

	return allMatchRooms.at(type);
}

//获取
MatchRoom* GameZone::getMatchRoomByID(int type, int matchID)
{
	cocos2d::__Array* array = getMatchRooms(type);
	Ref* obj = nullptr;
	CCARRAY_FOREACH(array, obj){
		MatchRoom* data = (MatchRoom*) obj;
		if (data->getMatchID() == matchID)
		{
			return data;
		}
	};
	return NULL;
}

/**********************************************************房间数据******start*********************************************************/
GameRoom::GameRoom() : 
	gameId(0),
	gZID(0), 
	zoneWin(0), 
	status(0), 
	acount(0),
	playerCount(0), 
	playType(0),
	multiple(0), 
	enterFortune(0), 
	zoneType(0), 
	invite(0), 
	fortuneBase(0),
	pointBase(0),
	levelMinLimit(0), 
	levelMaxLimit(0), 
	presentBean(0), 
	playedCount(0), 
	tableFee(0), 
	recordCardFalg(0),
	antiCheatFlag(0),
	lockFlag(0)
{

}

GameRoom::~GameRoom()
{

}

bool GameRoom::init()
{
	return true;
}
//--------------------------------比赛数据
MatchRoom::MatchRoom():
	gameId(0),
	matchID(0),
	matchtype(0),
	beginType(0),
	matchPartCount(0),
	hot(0),
	signupPlayer(0),
	signupFlag(0),
	waitTime(0),
	intervalTime(0),
	ticketFID(0),
	curPlayingPlayer(0)
{
}

MatchRoom::~MatchRoom()
{

}

bool MatchRoom::init()
{
	return true;
}

/**********************************************************房间数据******end*****************************************************************************************8/

HallData::HallData()
{
	pZone = nullptr;
}

HallData::~HallData()
{
	CC_SAFE_RELEASE_NULL(pZone);
}

bool HallData::init()
{
	pZone = GameZone::create();
	CC_SAFE_RETAIN(pZone);
	return true;
}

GameZone* HallData::getZone()
{
	return pZone;
}

PropIssueData::PropIssueData() : userID(0), gameCash(""), issueType(0), count(0)
{

}

PropIssueData::~PropIssueData()
{

}

bool PropIssueData::init()
{
	return true;
}
/**********************************UserBasicInfoData*******start*********************************************************************/
UserBasicInfoData::UserBasicInfoData():
	userId(0),
	newRegister(0),
	iconID(0),
	gender(0),
	vipGrade(0),
	level(0)
{

} 

UserBasicInfoData::~UserBasicInfoData()
{

}

void UserBasicInfoData::setUserId(int userId)
{
	this->userId = userId;
}

int UserBasicInfoData::getUserId()
{
	return this->userId;
}

void UserBasicInfoData::setNewRegister(int newRegister)
{
	this->newRegister = newRegister;
}

int UserBasicInfoData::getNewRegister()
{
	return this->newRegister;
}

void UserBasicInfoData::setUserPwd(std::string userPwd)
{
	this->userPwd = userPwd;
}

std::string UserBasicInfoData::getUserPwd()
{
	return this->userPwd;
}

void UserBasicInfoData::setNickName(std::string nickName)
{
	this->nickName = nickName;
}

std::string UserBasicInfoData::getNickName()
{
	return this->nickName;
}

void UserBasicInfoData::setIconID(int iconID)
{
	this->iconID = iconID;
}

int UserBasicInfoData::getIconID()
{
	return this->iconID;
}

void UserBasicInfoData::setGender(int gender)
{
	this->gender = gender;
}

int UserBasicInfoData::getGender()
{
	return this->gender;
}

void UserBasicInfoData::setVipGrade(int vipGrade)
{
	this->vipGrade = vipGrade;
}

int UserBasicInfoData::getVipGrade()
{
	return this->vipGrade;
}

void UserBasicInfoData::setLevel(int level)
{
	this->level = level;
}

int UserBasicInfoData::getLevel()
{
	return this->level;
}

void UserBasicInfoData::setGameCash(std::string gameCash)
{
	this->gameCash = gameCash;
}

std::string UserBasicInfoData::getGameCash()
{
	return this->gameCash;
}

void UserBasicInfoData::setGameCash2(std::string gameCash2)
{
	this->gameCash2 = gameCash2;
}

std::string UserBasicInfoData::getGameCash2()
{
	return this->gameCash2;
}

void UserBasicInfoData::setRegisterTime(std::string registerTime)
{
	this->registerTime = registerTime;
}

std::string UserBasicInfoData::getRegisterTime()
{
	return this->registerTime;
}

void UserBasicInfoData::setLastTime(std::string lastTime)
{
	this->lastTime = lastTime;
}

std::string UserBasicInfoData::getLastTime()
{
	return this->lastTime;
}

void UserBasicInfoData::setAPN(std::string apn)
{
	this->apn = apn;
}

std::string UserBasicInfoData::getAPN()
{
	return this->apn;
}

void UserBasicInfoData::setRegion(std::string region)
{
	this->region = region;
}

std::string UserBasicInfoData::getRegion()
{
	return this->region;
}

/********************************************UserBasicInfoData******end****************************************************************/
GameInfoData::GameInfoData()
{

}

GameInfoData::~GameInfoData()
{

}

bool GameInfoData::init()
{
	return true;
}

LogonAwardInfoData::LogonAwardInfoData():dayCount(0),awardCount(0),count(0)
{
}

LogonAwardInfoData::~LogonAwardInfoData()
{

}

bool LogonAwardInfoData::init()
{
	return true;
}

MediaContentInfoData::MediaContentInfoData()
{

}
MediaContentInfoData::~MediaContentInfoData()
{
}


bool MediaContentInfoData::init()
{
	return true;
}

RecoveryGameNotifyData::RecoveryGameNotifyData()
{}

RecoveryGameNotifyData::~RecoveryGameNotifyData()
{}

bool RecoveryGameNotifyData::init()
{
	return true;
}

ResultInfoData::ResultInfoData()
{}

ResultInfoData::~ ResultInfoData()
{}

bool ResultInfoData::init()
{
    return true;
}


NoticeList::NoticeList()
{}

NoticeList::~ NoticeList()
{}

bool NoticeList::init()
{
    return true;
}

NoticeContent::NoticeContent()
{}

NoticeContent::~ NoticeContent()
{}

bool NoticeContent::init()
{
    return true;
}


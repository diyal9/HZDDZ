/*
 * LordGameData.cpp
 *	Author: zx
 */

#include "LordGameData.h"
USING_NS_CC;
CallLordData::CallLordData() : gamePlayId(0), userIdJiaoFen(0), jiaoFlag(0), nextPlayer(0), playerCount(0)
{
	
}

CallLordData::~CallLordData()
{

}

bool CallLordData::init()
{
	return true;
}

cocos2d::Vector<PokerObject *>& CallLordData::getBottomPokers()
{
	return bottomPokers;
}



GrabLordData::GrabLordData() : gamePlayId(0), userIdQiangDiZhu(0), isGrab(0), isQiangDiZhuOver(0), nextPlayer(0)
{

}

GrabLordData::~GrabLordData()
{
}

bool GrabLordData::init()
{
	return true;
}

cocos2d::Vector<PokerObject *>& GrabLordData::getBottomPokers()
{
	return bottomPokers;
}

PlayCardData::PlayCardData() : gamePlayId(0), userID(0), type(0), value(0), nextPlayer(0)
{
}

PlayCardData::~PlayCardData()
{
}

bool PlayCardData::init()
{
	return true;
}


cocos2d::Vector<PokerObject *>& PlayCardData::getReplacePokers()
{
	return replacePokers;
}

cocos2d::Vector<PokerObject *>& PlayCardData::getPlayPokers()
{
	return playPokers;
}

JiaBeiData::JiaBeiData() : gamePlayId(0), userID(0)
{
}

JiaBeiData::~JiaBeiData()
{

}

bool JiaBeiData::init()
{
	return true;
}

MingPaiData::MingPaiData() : gamePlayId(0), userID(0)
{
}

MingPaiData::~MingPaiData()
{

}

bool MingPaiData::init()
{
	return true;
}

cocos2d::Vector<PokerObject *>& MingPaiData::getPlayerPokers()
{
	return playerPokers;
}

bool LordGameOverData::init()
{
	return true;
}

LordGameOverData::LordGameOverData() : gamePlayId(0), winnerUserID(0), zoneWin(0), multiple(0), time(""), bankUserID(0), bankScore(0), boomCount(0), fan(0), fortuneBase(0), bigBoomCount(0)
	,magicCount(0),isChunTian(0)
{

}

LordGameOverData::~LordGameOverData()
{
}

cocos2d::Vector<PokerObject *>& LordGameOverData::getSelfRemainPokers()
{
	return selfRemainPokers;
}

cocos2d::Vector<PokerObject *>& LordGameOverData::getRightRemainPokers()
{
	return rightRemainPokers;
}

cocos2d::Vector<PokerObject *>& LordGameOverData::getLeftRemainPokers()
{
	return leftRemainPokers;
}


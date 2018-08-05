/*
 * MsgLogonDataFilter.cpp
 *	功能描述：
 *  Created on: 2015年4月27日 下午2:09:51
 *      Author: diyal.yin
 */

#include "MsgLogonDataFilter.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Model/WWDataModel/CommonMessageData/CommonMessageData.h"
#include "BaseCore/Common/MsgConstDefine.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "Control/WWDataManager/WWDataManager.h"


const int filters[] ={ MSG_LOGON_PROFILE, MSG_LOGOUT_INFO, MSG_LOGON_REFUSE, MSG_RES_VERSION_STATUS, MSG_SERVER_IP, MSG_LOGON_FLASH_MESSAGE, MSG_FLASH_MESSAGE };
const int rootFilters[] ={MSG_LOGON_INFO };

MsgLogonDataFilter::MsgLogonDataFilter()
{

}

MsgLogonDataFilter::~MsgLogonDataFilter()
{

}

const std::vector<int> vfilters(filters, filters + sizeof(filters) / sizeof(int));

std::vector<int> MsgLogonDataFilter::onFilters()
{
	return vfilters;
}


bool MsgLogonDataFilter::onMessageReceived(int msgType)
{
	Ref* dataObj = nullptr;
	switch(msgType){
	case MSG_LOGON_PROFILE: 
		dataObj = parseProfile();
		//parseProfile();
		break;
	default:
		return false;
	}
	onDataReceiver(msgType, dataObj);
	return true;
}

Ref* MsgLogonDataFilter::parseProfile()
{

	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();


	int userID = pClientSocket->ReadInt();//用户ID
	int newRegister = pClientSocket->ReadChar();/*(int1) 0原有蛙号，1新注册蛙号*/
	std::string userPwd = pClientSocket->ReadString();//密码
	log("MsgLogonDataFilter::parseProfileNew-------userID = %d, newRegister = %d, userPwd = %s,",userID, newRegister, userPwd.c_str());

	//UserData::getInstance()->setIntValue(INT_USER_ID,userID);
	//UserData::getInstance()->setStringValue(STR_USER_PASSWORD,userPwd);

	////-------------------

	//UserBasicInfoData* userData = UserBasicInfoData::create();
	//
	//userData->setUserID(userID);//(int4)用户ID
	////userData->setGameId(pClientSocket->ReadInt());//(int4)游戏ID
	//userData->setNickname(pClientSocket->ReadString());//(String)昵称
	//userData->setIconID(pClientSocket->ReadInt());//(int4)用户头像ID
	//userData->setGender(pClientSocket->ReadChar());//(int1)性别1-男，2－女-------------

	//userData->setVipGrade(pClientSocket->ReadChar());//(int1)会员级别.0=非会员

	//userData->setLevel(pClientSocket->ReadShort());//(int2)等级----------

	//userData->setGameCash(pClientSocket->ReadString());//(String)金币数  
	//userData->setGameCash2(pClientSocket->ReadString());//(String)钻石数 ------------
	//userData->setRegisterTime(pClientSocket->ReadString());//(String)注册时间
	//userData->setLastlogonTime(pClientSocket->ReadString());//(String)上次登录时间
	//userData->setAPN(pClientSocket->ReadString());//(String)网络接入
	//std::string Region =  pClientSocket->ReadString();//(String)地区

	//log("MsgLogonDataFilter::parseProfile-----------UserID = %d, GameID = %d, Nickname = %s,Gander = %d, VipGrade = %d,Level=%d, GameCash = %s,GameCash2=%s, RegisterTime = %s, LastTime = %s, APN = %s, IconID = %d, Region=%s"
	//	,userData->getUserID(),userData->getGameId(),userData->getNickname().c_str(),userData->getGender(),userData->getVipGrade(),userData->getLevel(),userData->getGameCash().c_str(),userData->getGameCash2().c_str(),
	//	userData->getRegisterTime().c_str(),userData->getLastlogonTime().c_str(), userData->getAPN().c_str() ,userData->getIconID(),Region.c_str());
	//
	//
	//WWDataManager::getInstance()->setUserBasicInfoData(userData);

	return NULL;
}

const std::vector<int> vrootfilters(rootFilters,rootFilters + sizeof(rootFilters) / sizeof(int));

std::vector<int> MsgLogonDataFilter::onRootFilters()
{
	return vrootfilters;
}

bool MsgLogonDataFilter::onRootMessageReceived(int msgType, int userId, int result, std::string reason, int reasonType)
{
	CCLOG("MsgLogonDataFilter::onRootMessageReceived:msgType= %x,userId=%d,result=%d,reason=%s,reasonType=%x", msgType,userId,result,reason.c_str(),reasonType);
	CommonMessageData* data = CommonMessageData::create();
	data->setUserId(userId);
	data->setResult(result);
	data->setReasonType(reasonType);
	data->setReason(reason);
	switch (msgType) {
	case MSG_LOGON_INFO:
		break;
	default:
		return false;
	}

	onDataReceiver(msgType,data);
	return true;

}
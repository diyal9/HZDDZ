/*
 * DataCenter.h
 *	功能描述：
 *  Created on: 2014年6月26日 下午2:09:49
 *      Author: hp
 */

#ifndef _CONTROL_WWDATAMANAGER_WWDATAMANAGER_H_
#define _CONTROL_WWDATAMANAGER_WWDATAMANAGER_H_

#include "cocos2d.h"
#include "Model/WWDataModel/WWGD/GD.h"
#include "WWMacros.h"
#include "Model/WWDataModel/WWGameData/GameStartData.h"
#include "Model/WWDataModel/WWGameData/GameCommonData.h"
#include "Model/WWDataModel/WWGameData/LordGameData.h"

/*
 *数据中心
 */
class WWDataManager:public cocos2d::Ref
{
public:
	WWDataManager();

	virtual ~WWDataManager();

	SINGLE_INSTANCE_FUNC(WWDataManager);
	//CC_SYNTHESIZE_RETAIN(GameRoom*, gameRoom, GameRoom);//正在游戏的房间数据

	//void setGameRoom(GameRoom* gameRoom);
	//GameRoom* getGameRoom();
	CC_SYNTHESIZE_RETAIN(GameRoom*, gameRoom, GameRoom);//正在游戏的房间数据
	/**
	 * 玩家个人信息
	**/
	CC_SYNTHESIZE_RETAIN(UserBasicInfoData*, userBasicInfoData, UserBasicInfoData);//个人信息数据

	CC_SYNTHESIZE_RETAIN(GameZone*, gameZone, GameZone);//游戏区信息

	CC_SYNTHESIZE_RETAIN(GameStartData*, m_GameStartData, GameStartData);//开局消息

	//CC_SYNTHESIZE_RETAIN(cocos2d::Vector<MatchRoom*>, matchByPeople, MatchByPeople);//开局消息

private:
	//GameRoom* gameRoom;
	/* 跨平台相关数据结构 */
public:
	typedef struct DeviceInfo
	{
		std::string randomAsciiStr; //随机生成指定长度大小写字母、数字组合字符串
		int networkOperatorCode; //获取运营商代码
		int width;//手机宽
		int height;//手机高
		std::string apnType; //获取网络接入方式
		std::string idCode; //获取用户识别码，前5位是运营商id
		std::string SDkVersion;  //获取手机android版本号
		std::string phoneModel;  //获取手机厂商
		std::string PhoneHost;  //获取手机Host
		std::string PhoneMANUFACTURER;  //手机型号
		std::string PhoneDeviceId;  //获取设备唯一标示
		std::string MacAddress;  //获取mac地址
		std::string SimCardState;  //检查SIM卡是否可用
		std::string IMEI;  //获取手机IMEI
		std::string IMSI;  //获取手机IMSI
		std::string ipAddress;  //ip地址
		std::string phoneNum; //手机号码
		bool isMobileConnected; //是否是连接的移动网络
		bool isLTOperator; //判断是不是联通运营商
		bool isDXOperator; //判断是不是电信运营商
		bool isYDOperator; //判断是不是移动运营商

	} DEVICEINFO;

	DEVICEINFO deviceInfo;
};

#endif /* _CONTROL_WWDATAMANAGER_WWDATAMANAGER_H_ */

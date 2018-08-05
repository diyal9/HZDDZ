/*
* PlatformUtils.h
* 跨平台处理
* 
*  Created on: 2015-6-1
*      Author: diyal.yin
*/

#include "PlatformUtils.h"
#include "Control/WWDataManager/WWDataManager.h"

USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void PlatformUtils::getDeviceBaseInfo()
{
	JniMethodInfo t;
	bool hasMethod = JniHelper::getStaticMethodInfo(t, "com/wawa/hzddz/utils/PlatformUtils", "getDeviceBaseInfo", "()V");

	if (hasMethod)
	{
		log("had found StaticMethod getDeviceBaseInfo");
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		log("JNI CALL getDeviceBaseInfo but not found this method ");
	}
}

extern "C"
{
	void Java_com_wawa_hzddz_utils_PlatformUtils_getDeviceBaseInfoCallBack(JNIEnv* env, jobject thiz,
		jboolean isConnNet, 
		jstring strMac,
		jstring strIMEI,
		jstring strIMSI,
		jint width,            //屏幕宽
		jint height,           //屏幕高
		jint IdCode,           //运营商id
		jstring mobileType,    //手机型号
		jstring phoneModel,    //手机机型
		jstring apnType,       //联网类型
		jstring SDkVersion,    //os
		jstring ipAddress,
		jstring phoneNum
		)
	{
		bool lIsConnNet = isConnNet;//是否联网

		std::string testStr = JniHelper::jstring2string(strMac);
		WWDataManager::getInstance()->deviceInfo.MacAddress = testStr; //Mac地址
		log("Platform struct value MacAddress %s", WWDataManager::getInstance()->deviceInfo.MacAddress.c_str());

		std::string strIMEIc = JniHelper::jstring2string(strIMEI);
		WWDataManager::getInstance()->deviceInfo.IMEI = strIMEIc;//IMEI
		log("Platform struct value IMEI %s", WWDataManager::getInstance()->deviceInfo.IMEI.c_str());

		std::string strIMSIc = JniHelper::jstring2string(strIMSI);
		WWDataManager::getInstance()->deviceInfo.IMSI = strIMSIc;//IMSI
		log("Platform struct value IMSI %s", WWDataManager::getInstance()->deviceInfo.IMSI.c_str());

		WWDataManager::getInstance()->deviceInfo.width = width;//手机屏幕宽
		log("Platform struct value width %d", WWDataManager::getInstance()->deviceInfo.width);

		WWDataManager::getInstance()->deviceInfo.height = height;//手机屏幕高
		log("Platform struct value height %d", WWDataManager::getInstance()->deviceInfo.height);

		WWDataManager::getInstance()->deviceInfo.networkOperatorCode = IdCode;//手机运营商id
		log("Platform struct value networkOperatorCode %d", WWDataManager::getInstance()->deviceInfo.networkOperatorCode);

		std::string mobileTypec = JniHelper::jstring2string(mobileType);//手机型号
	    WWDataManager::getInstance()->deviceInfo.PhoneMANUFACTURER = mobileTypec;
		log("Platform struct value PhoneMANUFACTURER %s",WWDataManager::getInstance()->deviceInfo.PhoneMANUFACTURER.c_str());

		std::string phoneModelc = JniHelper::jstring2string(phoneModel);//手机厂商
	    WWDataManager::getInstance()->deviceInfo.phoneModel = phoneModelc;
		log("Platform struct value phoneModel %s",WWDataManager::getInstance()->deviceInfo.phoneModel.c_str());

		std::string apnTypec = JniHelper::jstring2string(apnType);//手机联网方式
	    WWDataManager::getInstance()->deviceInfo.apnType = apnTypec;
		log("Platform struct value apnType %s",WWDataManager::getInstance()->deviceInfo.apnType.c_str());

		std::string SDkVersionc = JniHelper::jstring2string(SDkVersion);//sdk版本
	    WWDataManager::getInstance()->deviceInfo.SDkVersion = SDkVersionc;
		log("Platform struct value SDkVersion %s",WWDataManager::getInstance()->deviceInfo.SDkVersion.c_str());

		std::string ipAddressc = JniHelper::jstring2string(ipAddress);//IP地址
		WWDataManager::getInstance()->deviceInfo.ipAddress = ipAddressc;
		log("Platform struct value ipAddress %s",WWDataManager::getInstance()->deviceInfo.ipAddress.c_str());

		std::string phoneNumc = JniHelper::jstring2string(phoneNum);//手机号码
		WWDataManager::getInstance()->deviceInfo.phoneNum = phoneNumc;
		log("Platform struct value phoneNum %s",WWDataManager::getInstance()->deviceInfo.phoneNum.c_str());
	}
};

int PlatformUtils::getBatteryPersent()
{
	jint ret = -1;
	JniMethodInfo t;
	bool hasMethod = JniHelper::getStaticMethodInfo(t, "com/wawa/hzddz/utils/PlatformUtils", "getBatteryPersent", "()I");
	if (hasMethod)
	{
		log("had found StaticMethod getBatteryPersent");
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
		log("Platform batteryPersent %d", ret);
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		log("JNI CALL getBatteryPersent but not found this method ");
	}
	return ret;
}


int PlatformUtils::getNetStatus()
{
	jint ret = -1;
	JniMethodInfo t;
	bool hasMethod = JniHelper::getStaticMethodInfo(t, "com/wawa/hzddz/utils/PlatformUtils", "getNetStatus", "()I");
	if (hasMethod)
	{
		log("had found StaticMethod getNetStatus");
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
		log("Platform netStatus %d", ret);
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		log("JNI CALL getNetStatus but not found this method ");
	}
	return ret;
}

std::string PlatformUtils::getSdcardPath()
{
	std::string ret = "";
	JniMethodInfo t;
	bool hasMethod = JniHelper::getStaticMethodInfo(t, "com/wawa/hzddz/utils/PlatformUtils", "getSdcardPath", "()Ljava/lang/String;");
	if (hasMethod)
	{
		log("had found StaticMethod getSdcardPath");
		jstring ts = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
		ret = JniHelper::jstring2string(ts);
		log("Platform getSdcardPath %s", ret.c_str());
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		log("JNI CALL getSdcardPath but not found this method ");
	}
	return ret;
}

void PlatformUtils::phoneVibrate()
{
	JniMethodInfo t;
	bool hasMethod = JniHelper::getStaticMethodInfo(t, "com/wawa/hzddz/utils/PlatformUtils", "phoneVibrate", "()V");

	if (hasMethod)
	{
		log("had found StaticMethod phoneVibrate");
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		log("JNI CALL phoneVibrate but not found this method ");
	}
}


#endif




std::string PlatformUtils::getFormatCurTime(bool bIsHaveSec/* = false*/)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	struct tm *tm;
	time_t timep = now.tv_sec;
	tm = localtime(&timep);
	int hour = tm->tm_hour;
	int min = tm->tm_min;
	std::string time;

	if (hour < 10)
	{
		time += StringUtils::format("0%d", hour);
	}
	else
	{
		time += StringUtils::format("%d", hour);
	}

	if (min < 10)
	{
		time += StringUtils::format(":0%d", min);
	}
	else
	{
		time += StringUtils::format(":%d", min);
	}

	if (bIsHaveSec)
	{
		int sec = tm->tm_sec;
		if (sec < 10)
		{
			time += StringUtils::format(":0%d", sec);
		}
		else
		{
			time += StringUtils::format(":%d", sec);
		}
	}
	
	return time;
}
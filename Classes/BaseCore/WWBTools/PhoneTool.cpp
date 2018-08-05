/*
 * PhoneTool.cpp
 *
 *  Created on: 2014年6月24日
 *      Author: D.K.
 */
#include "PhoneTool.h"
#include "MyDate.h"
#include "StringTool.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS /*&& COCOS2DX_DDZ*/)
#include "WWIphoneTool.h"
#endif

USING_NS_CC;

extern const char* KEY_SETTING_WI;
/***************************************************************
 android
 ***************************************************************/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void PhoneTool::updateLatestApkPackage(std::string& apkPath)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "updateVersion", "(Ljava/lang/String;)V"))
	{
		jstring jkey = t.env->NewStringUTF(apkPath.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, jkey);
		t.env->DeleteLocalRef(t.classID);
	}
}

/**
 * 获取Activity 对象
 */

/**
 * 获取运营商代码，
 *
 * @return
 */

int PhoneTool::getNetworkOperatorCode()
{
	int ret = -1;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath,
					"getNetworkOperatorCode", "()I"))
	{
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}

	return ret;
}
;

/**
 * 获取网络接入方式
 *
 * @return
 */
std::string PhoneTool::getApnType()
{

	std::string ret;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getApnType",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret = str;

		t.env->ReleaseStringUTFChars(retFromJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
;

/**
 * 获取用户识别码，前5位是运营商id
 */
std::string PhoneTool::getIdCode()
{
	std::string ret;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getIdCode",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret = str;

		t.env->ReleaseStringUTFChars(retFromJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
;

/**
 * 获取手机android版本号
 */
std::string PhoneTool::getSDkVersion()
{
	std::string ret;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getSDkVersion",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret = str;

		t.env->ReleaseStringUTFChars(retFromJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

/**
 * 获取手机厂商
 */
std::string PhoneTool::getPhoneModel()
{
	std::string ret;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getPhoneModel",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret = str;

		t.env->ReleaseStringUTFChars(retFromJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

std::string PhoneTool::getPhoneHost()
{
	std::string ret;
		JniMethodInfo t;

		if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getPhoneHost",
						"()Ljava/lang/String;"))
		{

			jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
					t.methodID);
			const char* str = t.env->GetStringUTFChars(retFromJava, 0);
			ret = str;

			t.env->ReleaseStringUTFChars(retFromJava, str);
			t.env->DeleteLocalRef(t.classID);
		}
		return ret;
}
/**
 * 手机型号
 */
std::string PhoneTool::getPhoneMANUFACTURER()
{
	std::string ret;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getPhoneMANUFACTURER",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret = str;

		t.env->ReleaseStringUTFChars(retFromJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
/**
 * 获取设备唯一标示
 * @return
 */
std::string PhoneTool::getPhoneDeviceId()
{
	std::string ret;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getPhoneDeviceId",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret = str;

		t.env->ReleaseStringUTFChars(retFromJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
/**
 * 获取手机上sdcard的状态
 * @return true-sdcard存在  false-sdcard不存在
 */
bool PhoneTool::isSdcardExist()
{
	bool ret = false;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "isSdcardExist",
					"()Z"))
	{

		jboolean retFromJava = t.env->CallStaticBooleanMethod(t.classID,
				t.methodID);
		ret = retFromJava;
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
;
std::string PhoneTool::getSdcardPath()
{

	std::string ret;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getSdcardPath",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret = str;

		t.env->ReleaseStringUTFChars(retFromJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
/**
 * 获取mac地址
 *
 * @return
 */
std::string PhoneTool::getMacAddress()
{

	std::string ret;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getMacAddress",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret = str;

		t.env->ReleaseStringUTFChars(retFromJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
;
/**
 * 检查SIM卡是否可用
 *
 * @return true 可用
 */
bool PhoneTool::getSimCardState()
{

	bool ret = false;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getSimCardState",
					"()Z"))
	{

		jboolean retFromJava = t.env->CallStaticBooleanMethod(t.classID,
				t.methodID);
		ret = retFromJava;
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
;
/**
 * 获取手机IMEI
 * @return
 */
std::string PhoneTool::getIMEI()
{

	std::string ret;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getIMEI",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret = str;

		t.env->ReleaseStringUTFChars(retFromJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

/**
 * 是否是连接的移动网络
 * @return
 */
bool PhoneTool::isMobileConnected()
{

	bool ret = false;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "isMobileConnected",
					"()Z"))
	{

		jboolean retFromJava = t.env->CallStaticBooleanMethod(t.classID,
				t.methodID);
		ret = retFromJava;
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

/**
 * 判断是不是联通运营商
 * @return
 */
bool PhoneTool::isLTOperator()
{

	bool ret = false;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "isLTOperator", "()Z"))
	{

		jboolean retFromJava = t.env->CallStaticBooleanMethod(t.classID,
				t.methodID);
		ret = retFromJava;
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

/**
 * 判断是不是电信运营商
 * @return
 */
bool PhoneTool::isDXOperator()
{

	bool ret = false;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "isDXOperator", "()Z"))
	{

		jboolean retFromJava = t.env->CallStaticBooleanMethod(t.classID,
				t.methodID);
		ret = retFromJava;
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

/**
 * 判断是不是移动运营商
 * @return
 */
bool PhoneTool::isYDOperator()
{

	bool ret = false;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "isYDOperator", "()Z"))
	{

		jboolean retFromJava = t.env->CallStaticBooleanMethod(t.classID,
				t.methodID);
		ret = retFromJava;
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

/**
 * 检查网络状态
 * @return
 */
std::string PhoneTool::checkNetState()
{

	std::string ret;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "checkNetState",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret = str;

		t.env->ReleaseStringUTFChars(retFromJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
/**
 * 获取百分比电量
 */
int PhoneTool::getBatteryStatus()
{
	int ret = -1;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath,
					"getBatteryStatus", "()I"))
	{
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
/**
 * 获取ip地址
 *
 */
std::string PhoneTool::getIpAddress()
{

	std::string ret;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getIpAddress",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret=str;
		t.env->ReleaseStringUTFChars(retFromJava, str);

		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

std::string PhoneTool::getSimOperator()
{

	std::string ret;
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getSimOperator",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret = str;

		t.env->ReleaseStringUTFChars(retFromJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

/**
 * 获取版本号
 */
int PhoneTool::getVersionCode()
{
	int ret = -1;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath,
					"getVersionCode", "()I"))
	{
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

/**
 * 获取版本名称
 */
std::string PhoneTool::getVersionName()
{
	std::string ret;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getVersionName",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret=str;
		t.env->ReleaseStringUTFChars(retFromJava, str);

		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

void PhoneTool::openPhotoAndSavePic(int userid)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath,
					"pickHeadFromAlbum", "(I)V"))
	{
		jint juserid=userid;
		t.env->CallStaticIntMethod(t.classID, t.methodID,userid);
		t.env->DeleteLocalRef(t.classID);
	}
}

void PhoneTool::openCameraAndSavePic(int userid)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath,
					"pickHeadFromCamera", "(I)V"))
	{
		jint juserid=userid;
		t.env->CallStaticIntMethod(t.classID, t.methodID,juserid);
		t.env->DeleteLocalRef(t.classID);
	}
}

void PhoneTool::makePhoneCall(const std::string phonenumber)
{
	int ret = 0;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath,
					"makePhoneCall", "(Ljava/lang/String;)V"))
	{
		jstring jurl = t.env->NewStringUTF(phonenumber.c_str());
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID,jurl);
		t.env->DeleteLocalRef(t.classID);
	}

	if(ret != 0)
	{
		//		WW_LOG_ERROR("PhoneToll=======================loadWapActivity error!");
	}
	else
	{
		//		WW_LOG_ERROR("PhoneToll=======================loadWapActivity success!");
	}
}

void PhoneTool::appUpdate(const std::string &contentText)
{
	int ret = 0;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath,
					"appUpdate", "(Ljava/lang/String;)V"))
	{
		jstring jurl = t.env->NewStringUTF(contentText.c_str());
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID,jurl);
		t.env->DeleteLocalRef(t.classID);
	}

	if(ret != 0)
	{
		//		WW_LOG_ERROR("PhoneToll=======================appUpdate error!");
	}
	else
	{
		//		WW_LOG_ERROR("PhoneToll=======================appUpdate success!");
	}
}

void PhoneTool::openLive800(const std::string &url)
{
	int ret = 0;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath,
					"loadWapActivity", "(Ljava/lang/String;)V"))
	{
		jstring jurl = t.env->NewStringUTF(url.c_str());
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID,jurl);
		t.env->DeleteLocalRef(t.classID);
	}

	if(ret != 0)
	{
		//		WW_LOG_ERROR("PhoneToll=======================openLive800 error!");
	}
	else
	{
		//		WW_LOG_ERROR("PhoneToll=======================openLive800 success!");
	}
}
void PhoneTool::loadActWapActivity(const std::string &url , const std::string &postData)
{
	int ret = 0;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath,
					"loadActWapActivity", "(Ljava/lang/String;Ljava/lang/String;)V"))
	{
		jstring jurl = t.env->NewStringUTF(url.c_str());
		jstring jpostData = t.env->NewStringUTF(postData.c_str());
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID,jurl,jpostData);
		t.env->DeleteLocalRef(t.classID);
	}

	if(ret != 0)
	{
		//		WW_LOG_ERROR("PhoneToll=======================loadActWapActivity error!");
	}
	else
	{
		//		WW_LOG_ERROR("PhoneToll=======================loadActWapActivity success!");
	}
}

void PhoneTool::openNetSetting()
{
	JniMethodInfo t;
	int ret=0;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "openNetSetting",
					"()V"))
	{

		ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		CCLOG("PhoneToll=======================openNetSetting can not find!");
	}
	if(ret == 0)
	{
		CCLOG("PhoneToll=======================openNetSetting error!");
	}
	else
	{
		CCLOG("PhoneToll=======================openNetSetting success!");
	}
}

std::string PhoneTool::getPhoneUA()
{
	std::string ret;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "getPhoneUA",
					"()Ljava/lang/String;"))
	{

		jstring retFromJava = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);
		const char* str = t.env->GetStringUTFChars(retFromJava, 0);
		ret=str;
		t.env->ReleaseStringUTFChars(retFromJava, str);

		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
//震动
void PhoneTool::phoneVibrate(int time)
{
	//震动是否为关闭状态
	//bool state = UserDefault::getInstance()->getBoolForKey(KEY_SETTING_WI, true);
	bool state = false;
	if (state)
	{
		JniMethodInfo t;
   		if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "phoneVibrate", "(I)V"))
   		{
       		t.env->CallStaticVoidMethod(t.classID, t.methodID, time);
       		t.env->DeleteLocalRef(t.classID);
   		}
	}
	else
	{
		CCLOG("PhoneTool::phoneVibrate 震动关闭了");
	}

}
//取消震动
void PhoneTool::cancelVibrate()
{
   JniMethodInfo t;
   if (JniHelper::getStaticMethodInfo(t, PhoneToolPath, "cancelVibrate", "()V"))
   {
       t.env->CallStaticVoidMethod(t.classID, t.methodID);
       t.env->DeleteLocalRef(t.classID);
   }
}

/**
 * 获取wifi信号强度
 */
int PhoneTool::getWifiState()
{
	int ret = -1;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath,
					"getWifistatus", "()I"))
	{
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}

int PhoneTool::getPhoneNetState()
{
	int ret = -1;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, PhoneToolPath,
		"getPhoneNetState", "()I"))
	{
		ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
}
/***************************************************************
 /ios
 ***************************************************************/

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && COCOS2DX_DDZ)

//网络码
int PhoneTool::getNetworkOperatorCode()
{
	return WWIphoneTool::getInstance()->getNetWorkID();
}

//获取网络接入方式
std::string PhoneTool::getApnType()
{
	return "";
}

//获取用户识别码，前5位是运营商id
std::string PhoneTool::getIdCode()
{
	return "";
}

//获取手机android版本号
std::string PhoneTool::getSDkVersion()
{
	return WWIphoneTool::getInstance()->getSystemNameVersion();
}

//获取手机厂商
std::string PhoneTool::getPhoneModel()
{
	return WWIphoneTool::getInstance()->getPhoneModel();
}
std::string PhoneTool::getPhoneHost()
{
	return "";
}
//手机型号
std::string PhoneTool::getPhoneMANUFACTURER()
{
	return WWIphoneTool::getInstance()->getPhoneModel();
}

//获取设备唯一标示
std::string PhoneTool::getPhoneDeviceId()
{
	return "";
}

//获取手机上sdcard的状态
bool PhoneTool::isSdcardExist()
{
	return false;
}

std::string PhoneTool::getSdcardPath()
{
	return "";
}
//获取mac地址
std::string PhoneTool::getMacAddress()
{
	return "";
}

//检查SIM卡是否可用
bool PhoneTool::getSimCardState()
{
	return false;
}

//获取手机IMEI
std::string PhoneTool::getIMEI()
{
	return "";
}

//是否是连接的移动网络
bool PhoneTool::isMobileConnected()
{
	return false;
}

//判断是不是联通运营商
bool PhoneTool::isLTOperator()
{
	return false;
}

//判断是不是电信运营商
bool PhoneTool::isDXOperator()
{
	return false;
}

//判断是不是移动运营商
bool PhoneTool::isYDOperator()
{
	return false;
}

//检查网络状态
std::string PhoneTool::checkNetState()
{
	int net = WWIphoneTool::getInstance()->getNetWorkID();
	if (net == 9)
	{
		return "wifi";
	}
	return "运营商";
}

//获取ip地址
std::string PhoneTool::getIpAddress()
{
	return WWIphoneTool::getInstance()->getIpAddress();;
}

std::string PhoneTool::getSimOperator()
{
	return "";
}

//float PhoneTool::getScreenWidth()
//{
//	return Director::getInstance()->getOpenGLView()->getFrameSize().width;
//}
//
//float PhoneTool::getScreenHeight()
//{
//	return Director::getInstance()->getOpenGLView()->getFrameSize().height;
//}
//
//std::string PhoneTool::getMobileModel()
//{
//	return CCString::createWithFormat("%dX%d", (int)getScreenWidth(), (int)getScreenHeight())->getCString();
//}

int PhoneTool::getVersionCode()
{
	return 0;
}

//版本号
std::string PhoneTool::getVersionName()
{

	std::string version = WWIphoneTool::getInstance()->getAppVersion();
	if (version.c_str())
	{
		log("ios PhoneTool::getVersionName : %s", version.c_str());
		return version;
	}
	else
	{
		return "";
	}

}

//获取百分比电量

int PhoneTool::getBatteryStatus()
{
	return WWIphoneTool::getInstance()->getIphoneBatteryLevel();
}

void PhoneTool::openPhotoAndSavePic(int userid)
{

	WWIphoneTool::getInstance()->openPhotoAndSavePic(userid);

}

void PhoneTool::openCameraAndSavePic(int userid)
{

	WWIphoneTool::getInstance()->openCameraAndSavePic(userid);

}

void PhoneTool::makePhoneCall(const std::string phonenumber)
{
    
}

void PhoneTool::appUpdate(const std::string &contentText)
{

}

void PhoneTool::openLive800(const std::string &url)
{

}

void PhoneTool::loadActWapActivity(const std::string &url, const std::string &postData)
{

}

void PhoneTool::openNetSetting()
{

}

//震动
void PhoneTool::phoneVibrate(int time)
{
    
}
//取消震动
void PhoneTool::cancelVibrate()
{
    
}

std::string PhoneTool::getPhoneUA()
{
	return WWIphoneTool::getInstance()->getPhoneUA();
}

int PhoneTool::getWifiState()
{
    return 0;
}
/***************************************************************
 /other platform
 ***************************************************************/
#else

/**
 * 获取运营商代码，
 *
 * @return
 */

int PhoneTool::getNetworkOperatorCode()
{

	return 0;
}
;

/**
 * 获取网络接入方式
 *
 * @return
 */std::string PhoneTool::getApnType()
{
	return "";
}
;

/**
 * 获取用户识别码，前5位是运营商id
 */std::string PhoneTool::getIdCode()
{

	return "";
}
;

/**
 * 获取手机android版本号
 */std::string PhoneTool::getSDkVersion()
{

	return "";
}

/**
 * 获取手机厂商
 */std::string PhoneTool::getPhoneModel()
{

	return "";
}
/**
 * 获取手机Host
 * @return
 */
std::string PhoneTool::getPhoneHost()
{
	return "";
}
/**
 * 手机型号
 */std::string PhoneTool::getPhoneMANUFACTURER()
{

	return "";
}
/**
 * 获取设备唯一标示
 * @return
 */std::string PhoneTool::getPhoneDeviceId()
{

	return "";
}
/**
 * 获取手机上sdcard的状态
 * @return true-sdcard存在  false-sdcard不存在
 */
bool PhoneTool::isSdcardExist()
{

	return false;
}
;

std::string PhoneTool::getSdcardPath()
{
	return "";
}
/**
 * 获取mac地址
 *
 * @return
 */std::string PhoneTool::getMacAddress()
{

	return "";
}
/**
 * 检查SIM卡是否可用
 *
 * @return true 可用
 */
bool PhoneTool::getSimCardState()
{

	return false;
}
;
/**
 * 获取手机IMEI
 * @return
 */std::string PhoneTool::getIMEI()
{

	return "";
}

/**
 * 是否是连接的移动网络
 * @return
 */
bool PhoneTool::isMobileConnected()
{

	return false;
}

/**
 * 判断是不是联通运营商
 * @return
 */
bool PhoneTool::isLTOperator()
{

	return false;
}

/**
 * 判断是不是电信运营商
 * @return
 */
bool PhoneTool::isDXOperator()
{

	return false;
}

/**
 * 判断是不是移动运营商
 * @return
 */
bool PhoneTool::isYDOperator()
{

	return false;
}

/**
 * 检查网络状态
 * @return
 */std::string PhoneTool::checkNetState()
{

	return "";
}
/**
 * 获取百分比电量
 */
int PhoneTool::getBatteryStatus()
{
	return 0;
}
/**
 * 获取ip地址
 *
 */std::string PhoneTool::getIpAddress()
{
	return "";
}

std::string PhoneTool::getSimOperator()
{

	return "";
}

int PhoneTool::getVersionCode()
{
	return 0;
}

std::string PhoneTool::getVersionName()
{
	return "";
}

void PhoneTool::openPhotoAndSavePic(int userid)
{

}

void PhoneTool::openCameraAndSavePic(int userid)
{

}

void PhoneTool::makePhoneCall(const std::string phonenumber)
{

}

void PhoneTool::appUpdate(const std::string &contentText)
{

}

void PhoneTool::openLive800(const std::string &url)
{

}
void PhoneTool::loadActWapActivity(const std::string &url, const std::string &postData)
{

}
void PhoneTool::openNetSetting()
{

}

std::string PhoneTool::getPhoneUA()
{
	return "";
}

int PhoneTool::getWifiState()
{
	return 0;
}

//震动
void PhoneTool::phoneVibrate(int time)
{

}
//取消震动
void PhoneTool::cancelVibrate()
{

}
#endif

float PhoneTool::getScreenWidth()
{
	return Director::getInstance()->getOpenGLView()->getFrameSize().width;
}

float PhoneTool::getScreenHeight()
{
	return Director::getInstance()->getOpenGLView()->getFrameSize().height;
}

std::string PhoneTool::getMobileModel()
{

	return CCString::createWithFormat("%dX%d", (int) getScreenWidth(), (int) getScreenHeight())->getCString();
}

char PhoneTool::getRandomChar()
{
	int randomInt;

	randomInt = (int) (CCRANDOM_0_1() * 62);

	//		WW_LOG_ERROR("randomInt=%d",randomInt);

	if (randomInt < 10)
	{
		randomInt += 48;
	}
	else if (randomInt < 36)
	{
		randomInt += 55;
	}
	else
	{
		randomInt += 61;
	}
	return (char) randomInt;
}

std::string PhoneTool::randomAsciiString(int len)
{
	__String *ran = __String::create("");

	struct timeval psv;
	gettimeofday(&psv, NULL); // 计算时间种子
	unsigned int tsrans = psv.tv_sec * 1000 + psv.tv_usec / 1000; // 初始化随机数
	srand(tsrans);
	for (int i = 0; i < len; ++i)
	{
		ran = CCString::createWithFormat("%s%c", ran->getCString(), getRandomChar());

	}
	return ran->getCString();

}
std::string PhoneTool::getFormatDayTime()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	struct tm *tm;
	time_t timep = now.tv_sec;
	tm = localtime(&timep);
	int year = tm->tm_year + 1900;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;

	std::string time = StringUtils::format("%d", year);
	if (month < 10)
	{
		time += StringUtils::format("-0%d", month);
	}
	else
	{
		time += StringUtils::format("-%d", month);
	}

	if (day < 10)
	{
		time += StringUtils::format("-0%d", day);
	}
	else
	{
		time += StringUtils::format("-%d", day);
	}
	return time;
}

std::string PhoneTool::getFormatCurTime(bool bIsHaveSec/* = false*/)
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

int PhoneTool::getDifferDays(std::string starttime)
{
	if (starttime.length() == 0)
	{
		return 0;
	}
	if (starttime == getFormatDayTime())
		return 0;
	struct timeval now;
	gettimeofday(&now, NULL);
	struct tm *tm;
	time_t timep = now.tv_sec;
	tm = localtime(&timep);
	int year = tm->tm_year + 1900;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;
	std::vector<std::string> valueVec = StringTool::split(starttime, "-");
	if (valueVec.size() != 3)
	{
		return 0;
	}
	int startY = Value(valueVec.at(0)).asInt();
	int startM = Value(valueVec.at(1)).asInt();
	int startD = Value(valueVec.at(2)).asInt();
	MyDate nowDate(year, month, day);
	MyDate startDate(startY, startM, startD);
	return nowDate - startDate;
}

int PhoneTool::getDifferDays(std::string starttime, std::string totime)
{
    if (starttime.length() == 0)
    {
        return 0;
    }
    
    if (starttime == totime)
    {
        return 0;
    }
    
//    struct timeval now;
//    gettimeofday(&now, NULL);
//    struct tm *tm;
//    time_t timep = now.tv_sec;
//    tm = localtime(&timep);
//    int year = tm->tm_year + 1900;
//    int month = tm->tm_mon + 1;
//    int day = tm->tm_mday;
    std::vector<std::string> valueVec = StringTool::split(starttime, "-");
    if (valueVec.size() != 3)
    {
        return 0;
    }
    std::vector<std::string> totimeVec = StringTool::split(totime, "-");
    if (totimeVec.size() != 3)
    {
        return 0;
    }
    
    int startY = Value(valueVec.at(0)).asInt();
    int startM = Value(valueVec.at(1)).asInt();
    int startD = Value(valueVec.at(2)).asInt();
    
    int toY = Value(totimeVec.at(0)).asInt();
    int toM = Value(totimeVec.at(1)).asInt();
    int toD = Value(totimeVec.at(2)).asInt();
    
    MyDate toDate(toY, toM, toD);
    MyDate startDate(startY, startM, startD);
    return toDate - startDate;
}

void PhoneTool::decodeChar(char *src, char *dest)
{
	int code;

	for (; *src != '\0'; src++, dest++)
	{
		if (*src == '%')
		{
			if (sscanf(src + 1, "%2x", &code) != 1)
				code = '?';
			*dest = code;
			src += 2;
		}
		else if (*src == '+')
			*dest = ' ';
		else
			*dest = *src;
	}
	*dest = '\0';
}

void PhoneTool::setScreenBrightness(float brightness)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	WWIphoneTool::getInstance()->setScreenBrightness(brightness);
#endif
}


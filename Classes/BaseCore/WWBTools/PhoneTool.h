/*
 * PhoneTool.h
 *
 *  Created on: 2014年6月24日
 *      Author: D.K.
 */

#ifndef PHONETOOL_H_
#define PHONETOOL_H_
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
	static const char * ActivityPath="com/ww/platform/WWPlatform";
	static const char * getActivityMethod="getActivity";
	static const char * PhoneToolPath="com/ww/platform/utils/PhoneTool";
#endif
class PhoneTool
{

private:
		static  char getRandomChar();

public:


	/**
	* 随机生成指定长度大小写字母、数字组合字符串
	*
	* @param len
	* @return
	*/
	static  std::string randomAsciiString(int len);

	/**
	 * 获取运营商代码，
	 *
	 * @return
	 */
	static int getNetworkOperatorCode();

	/**
	 * 获取网络接入方式
	 *
	 * @return
	 */
	static std::string getApnType();

	/**
	 * 获取用户识别码，前5位是运营商id
	 */
	static std::string getIdCode();

	/**
	 * 获取手机android版本号
	 */
	static std::string getSDkVersion();


	/**
	 * 获取手机厂商
	 */
	static std::string getPhoneModel();

	/**
		 * 获取手机Host
		 * @return
		 */
	static std::string getPhoneHost();
	/**
	 * 手机型号
	 */
	static std::string getPhoneMANUFACTURER();
	/**
	 * 获取设备唯一标示
	 * @return
	 */
	static std::string getPhoneDeviceId();

	/**
	 * 获取手机上sdcard的状态
	 * @return true-sdcard存在  false-sdcard不存在
	 */
	static bool isSdcardExist();
	/**
	 * 获取sd卡路径
	 */
	static std::string getSdcardPath();

	/**
	 * 获取mac地址
	 *
	 * @return
	 */
	static std::string getMacAddress();

	/**
	 * 检查SIM卡是否可用
	 *
	 * @return true 可用
	 */
	static bool getSimCardState();
	/**
	 * 获取手机IMEI
	 * @return
	 */
	static std::string getIMEI();

	/**
	 * 是否是连接的移动网络
	 * @return
	 */
	static bool isMobileConnected();

	/**
	 * 判断是不是联通运营商
	 * @return
	 */
	static bool isLTOperator();

	/**
	 * 判断是不是电信运营商
	 * @return
	 */
	static bool isDXOperator();

	/**
	 * 判断是不是移动运营商
	 * @return
	 */
	static bool isYDOperator();

	/**
	 * 检查网络状态
	 * @return
	 */
	static std::string checkNetState();
	/**
	 * 获取电量百分比
	 */
	static int getBatteryStatus();
	/**
	 * 获取ip地址
	 *
	 */
	static std::string getIpAddress();

	static std::string getSimOperator();

	static float getScreenWidth();

	static float getScreenHeight();

	static std::string getMobileModel();

	/**
	 * 获取版本号
	 */
	static int getVersionCode();

	/**
	 * 获取版本名称
	 */
	static std::string getVersionName();

    //获取系统日期yyyy-mm-dd
	static std::string getFormatDayTime();
    
    //获取当前24小时制时间hh:mm
	static std::string getFormatCurTime(bool bIsHaveSec = false);

	static int getDifferDays(std::string starttime) ;//yyyy-mm-dd与系统时间的间隔天数

    static int getDifferDays(std::string starttime, std::string totime);//yyyy-mm-dd与系统时间的间隔天数
    
	static void decodeChar(char *src, char *dest);//百分号解码
    
    //从相册获取图片
    static void openPhotoAndSavePic(int userid);
    
    //拍照
    static void openCameraAndSavePic(int userid);
    
    //拨打电话
    static void makePhoneCall(const std::string phonenumber);
    
    //版本更新
    static void appUpdate(const std::string &contentText);
    
    //打开Live800
    static void openLive800(const std::string &url);
    //打开活动页面
    static void loadActWapActivity(const std::string &url, const std::string &postData="");

    //打开网络设置
    static void openNetSetting();
    
    //获取UA信息
    static std::string getPhoneUA();
    
    //设置屏幕亮度
    static void setScreenBrightness(float brightness);
    
    //震动
    static void phoneVibrate(int time);
    //取消震动
    static void cancelVibrate();

	//获取wifi信号强度
	static int getWifiState();

	//获取移动网络信号强度
	static int getPhoneNetState();

	//更新版本
	static void updateLatestApkPackage(std::string& apkPath);
};

#endif /* PHONETOOL_H_ */

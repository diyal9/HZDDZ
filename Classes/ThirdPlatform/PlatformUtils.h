/*
 * PlatformUtils.h
 * 跨平台处理
 * 
 *  Created on: 2015-6-1
 *      Author: diyal.yin
 */

#ifndef _THIRDPLATFORM_PLATFORMUTILS_
#define _THIRDPLATFORM_PLATFORMUTILS_

#include "cocos2d.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
static const char * PhoneToolPath="com/wawa/hzddz/utils/PlatformUtils";
#endif

class PlatformUtils {
public:
	/************************************************************************/
	/* 获取设备信息                                                         */
	/************************************************************************/
	static void getDeviceBaseInfo();

	/***********************************************************************/
	/**获取电量百分比                                                     **/
	/***********************************************************************/
	static int getBatteryPersent();

	/***********************************************************************/
	/**获取信号强度                                                       **/
	/***********************************************************************/
	static int getNetStatus();

	//获取当前24小时制时间hh:mm
	static std::string getFormatCurTime(bool bIsHaveSec = false);

	//获取sdcard路径
	static std::string getSdcardPath();

	//调用手机震动
	static void phoneVibrate();

	//取消震动
	static void cancelVibrate();

};

#endif /* _THIRDPLATFORM_PLATFORMUTILS_ */

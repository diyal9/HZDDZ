/*
 * PlantformLoginUtils.h
 * 跨平台处理 登录模块相关
 * 
 *  Created on: 2015-6-1
 *      Author: diyal.yin
 */

#ifndef _THIRDPLATFORM_PLANTFORMLOGINUTILS_
#define _THIRDPLATFORM_PLANTFORMLOGINUTILS_

#include "cocos2d.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

static const char* ChargeUtilsPath = "com/ww/platform/utils/ThirdPartLoginUtils";

class PlantformLoginUtils {
public:
	/************************************************************************/
	/* 获取设备信息                                                         */
	/************************************************************************/
};

#endif /* _THIRDPLATFORM_PLANTFORMLOGINUTILS_ */

/*
 * WW_GBK_TO_UTF8.h
 *
 *  Created on: 2014年6月13日
 *      Author: wly
 */

#ifndef _GBKTOUTF8_H_
#define _GBKTOUTF8_H_

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//win32下的字符编码转换库
#include "win32-specific\icon\include\iconv.h"

//动态的链接iconv库，也可注释后在工程中设置
#pragma comment(lib, "libiconv.lib")

//字符编码转换函数
int GBKToUTF8(std::string & gbkStr, const char* toCode, const char* fromCode);

#endif

#endif // WIN32

/*
 * WW_GBK_TO_UTF8.cpp
 *
 *  Created on: 2014年6月13日
 *      Author: wly
 */
#include "WW_GBK_TO_UTF8.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

//一般我们使用GBK或者GB2312转换为UTF-8
int GBKToUTF8(std::string & gbkStr, const char* fromCode, const char* toCode)
{
	iconv_t iconvH;
	
	//具体转换函数
	iconvH = iconv_open(toCode,fromCode);

	if (iconvH == 0)
	{
		return -1;
	}

	const char* strChar = gbkStr.c_str();

	const char** pin = &strChar;
	size_t strLength = gbkStr.length();
	char* outbuf = (char*) malloc(strLength*4);
	char* pBuff = outbuf;

	memset(outbuf, 0, strLength*4);
	size_t outLength = strLength*4;

	if (-1 == iconv(iconvH, pin, &strLength, &outbuf, &outLength))
	{
		free(pBuff);
		iconv_close(iconvH);
		return -1;
	}

	gbkStr = pBuff;
	iconv_close(iconvH);

	return 0;
}
#endif

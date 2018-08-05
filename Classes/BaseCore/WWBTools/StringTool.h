/*
 * StringTool.h
 *
 *  Created on: 2014-7-31
 *      Author: chenzc
 */

#ifndef __HD_DDZ_BASECORE_WWBTOOLS_STRINGTOOL_H_
#define __HD_DDZ_BASECORE_WWBTOOLS_STRINGTOOL_H_

#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni.h"
#endif
//#include "../LogSystem/WWLogMaros.h"

class StringTool {
public:
	StringTool();
	virtual ~StringTool();
	static std::vector<std::string> split(std::string src, std::string separate_character);
	static std::vector<std::string> splitByArr(std::string src, std::string separate_character);
	static std::string removeRichString(std::string src);
	static std::string replaceAll(std::string src, std::string org_str, std::string rep_str);
	// 查找子串sub在str中出现的次数--用C++方式--子串可重叠
	static int getTimesWithSame(const std::string str, const std::string sub);
	// 查找子串sub在str中出现的次数--用C方式--子串可重叠
	static int getTimesWithSame(const char* str, const char* sub);
	// 查找子串sub在str中出现的次数--用C++方式--子串不可重叠
	static int getTimesWithNoSame(const std::string str, const std::string sub);
	// 查找子串sub在str中出现的次数--用C方式--子串不可重叠
	static int getTimesWithNoSame(const char* str, const char* sub);
    //截取字符串定长,其余替换为"..."
    static std::string getStringLenth(std::string str,int length);
    
    //float转换为string
    static std::string floatToString(float floatNum);
    
    //int转换为string
    static std::string intToString(int intNum);
    
    //string转换为int
    static int stringToint(const std::string& string);
    
    //去掉字符串中所有的includeStr 最常用的是去掉回车
    static std::string removeIncludeStr(std::string pointStr,std::string includeStr);
    
    static bool startWith(const std::string& str, const std::string& strStart);
    static bool endWith(const std::string& str, const std::string& strEnd);

	static std::string charArrayToString(char str[],int length);

    #if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	static std::string jstringToString(JNIEnv* env, jstring jstr);
    #endif
};

#endif /* __HD_DDZ_BASECORE_WWBTOOLS_StringTool_H_ */

/*
 * StringTool.cpp
 *
 *  Created on: 2014-7-31
 *      Author: chenzc
 */

#include "StringTool.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

StringTool::StringTool() {
	// TODO Auto-generated constructor stub

}

StringTool::~StringTool() {
	// TODO Auto-generated destructor stub
}

std::vector<std::string> StringTool::split(std::string src, std::string separate_character)
{
	std::vector<std::string> strs;
	int separate_characterLen = separate_character.size();//分割字符串长度，这样就可以支持多字 符串的分隔符
	int last_position = 0, index = -1;
	while (-1 != (index = src.find(separate_character, last_position)))
	{
//		WW_LOG_INFO("Charge::======================:%s", src.substr(last_position, index - last_position).c_str());
		strs.push_back(src.substr(last_position, index - last_position));
		last_position = index + separate_characterLen;
	}
	std::string lastString = src.substr(last_position);
	if (!lastString.empty())
	{
//		WW_LOG_INFO("Charge::======================:%s", lastString.c_str());
		strs.push_back(lastString);
	}
	return strs;
}
std::vector<std::string> StringTool::splitByArr(std::string src, std::string separate_character)
{
	std::vector<std::string> strs;
	int separate_characterLen = separate_character.size();//分割字符串长度，这样就可以支持多字 符串的分隔符
	int last_position = 0, index = -1;
//	WW_LOG_INFO("splitByArr::src======================:%s", src.c_str());
	while (-1 != (index = src.find(separate_character, last_position)))
	{
//		WW_LOG_INFO("splitByArr:: split======================:%s", src.substr(last_position, index - last_position).c_str());
		strs.push_back(src.substr(last_position, index - last_position).c_str());
		last_position = index + separate_characterLen;
	}
	std::string lastString = src.substr(last_position);
	if (!lastString.empty())
	{
//		WW_LOG_INFO("splitByArr:: last ======================:%s", lastString.c_str());
		strs.push_back(lastString.c_str());
//		WW_LOG_INFO("splitByArr:: last  add======================:%s", lastString.c_str());
	}
//	WW_LOG_INFO("splitByArr::strs count======================:%d", strs->count());
	return strs;
}

std::string StringTool::removeRichString(std::string src)
{
	std::vector<std::string> strs;
//	int separate_characterLen = separate_character.size();//分割字符串长度，这样就可以支持多字 符串的分隔符
	std::string separate_character_left = "[meta";
	std::string separate_character_right = "/]";
	int last_position = 0, index = -1;
	while (-1 != (index = src.find(separate_character_left, last_position)))
	{
//		WW_LOG_INFO("Charge::======================:%s", src.substr(last_position, index - last_position).c_str());
		strs.push_back(src.substr(last_position, index - last_position));
		int separate_characterLen = src.find(separate_character_right, last_position)+separate_character_right.length() - index;
		last_position = index + separate_characterLen;
	}
	std::string lastString = src.substr(last_position);
	if (!lastString.empty())
	{
//		WW_LOG_INFO("Charge::======================:%s", lastString.c_str());
		strs.push_back(lastString);
	}

	std::string str = "";
	for(int i=0; i<strs.size(); i++){
		str.append(strs[i]);
	}

	std::string result = str;
    
    //去掉@
    while (result.find("@") != std::string::npos)
    {
        result = result.replace(result.find("@"), 1, "");
    }
    
	return result;
}

std::string StringTool::replaceAll(std::string src, std::string org_str, std::string rep_str)
{
	std::vector<std::string> delimVec = split(src, org_str);
    if (delimVec.size() <= 0)
    {
        return src;
    }
    std::string target("");
    for (int i=0; i<delimVec.size(); i++)
    {
    	if(i == delimVec.size() - 1)
    	{
    		target = target + delimVec[i];
    	}
    	else
    	{
    		target = target + delimVec[i] + rep_str;
    	}
//       WW_LOG_INFO("Charge::======================:%s", target.c_str());
    }
    return target;
}


// 查找子串sub在str中出现的次数--用C++方式--子串可重叠
int StringTool::getTimesWithSame(const std::string str, const std::string sub)
{
	int num = 0;
	for (size_t i=0; (i=str.find(sub,i)) != std::string::npos; num++, i++);
	return num;
}

//函数2: 查找子串sub在str中出现的次数--用C方式--子串可重叠
int StringTool::getTimesWithSame(const char* str, const char* sub)
{
	int num = 0;
	for (const char* pstr=str; *pstr && (pstr=strstr(pstr,sub)); pstr++,num++);
	return num;
}
//函数3: 查找子串sub在str中出现的次数--用C++方式--子串不重叠
int StringTool::getTimesWithNoSame(const std::string str, const std::string sub)
{
	int num = 0;
	size_t len = sub.length();
	if (len == 0)len=1;//应付空子串调用
	for (size_t i=0; (i=str.find(sub,i)) != std::string::npos; num++, i+=len);
	return num;
}

//函数4: 查找子串sub在str中出现的次数--用C方式--子串不重叠
int StringTool::getTimesWithNoSame(const char* str, const char* sub)
{
	int num = 0;
	size_t len = strlen(sub);
	if (len == 0)len=1;//应付空子串调用
	for (const char* pstr=str; *pstr && (pstr=strstr(pstr,sub)); pstr+=len,num++);
	return num;
}

std::string StringTool::floatToString(float floatNum)
{
    std::ostringstream oss;
    oss<<floatNum;
    std::string str(oss.str());
	return str;
}

std::string StringTool::intToString(int intNum)
{
    std::ostringstream oss;
    oss<<intNum;
    std::string str(oss.str());
	return str;
}

std::string StringTool::getStringLenth(std::string str,int length)
{
    auto holelength = str.length();
    auto afterReplace = str.replace(length, holelength-length, "...");
    return afterReplace;
}

int StringTool::stringToint(const std::string& string)
{
    char* end;
    int i = static_cast<int>(strtol(string.c_str(),&end,10));
    return i;
}

std::string StringTool::removeIncludeStr(std::string pointStr,std::string includeStr)
{
    CC_ASSERT(!pointStr.empty());
    if (pointStr.empty())
    {
        return pointStr;
    }
    auto pos = pointStr.find(includeStr);
    if (pos>pointStr.size())
    {
        return pointStr;
    }
    pointStr.replace(pos, includeStr.size(), "");
    return  removeIncludeStr(pointStr, includeStr);
}

bool StringTool::startWith(const std::string& str, const std::string& strStart)
{
    if(str.empty() || strStart.empty())
    {
        return false;
    }

    return str.compare(0, strStart.size(), strStart) == 0 ? true : false;
}

bool StringTool::endWith(const std::string& str, const std::string& strEnd)
{
    if(str.empty() || strEnd.empty())
    {
        return false;
    }
    return str.compare(str.size() - strEnd.size(), strEnd.size(), strEnd) == 0 ? true : false;
}

std::string StringTool::charArrayToString(char str[],int length)
{
	ostringstream oss;
	oss << setiosflags(ios::uppercase) << hex; //大写16进制
	oss.fill('0'); //不满位数填充0
	for (int i = 0; i < length; i++)
	{
		unsigned char c = str[i];
		oss << setw(2) << (int)c; 
		if(i < length - 1)
			oss << setw(2) << "::";
	}
	string result = oss.str();
	return result;
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
std::string StringTool::jstringToString(JNIEnv* env, jstring jstr)
{
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*)malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	std::string cStr = rtn;
	free(rtn);
	rtn = NULL;
	std::string result = cStr;
	return result;
}
#endif

/*
 * WWJsonManager.h
 *
 *  Created on: 2014-8-4
 *      Author: lenovo
 */

#ifndef __HD_DDZ_CONTROL_WWCDBMANAGER_WWJSONMANAGER_H__
#define __HD_DDZ_CONTROL_WWCDBMANAGER_WWJSONMANAGER_H__

//#include "cocos-ext.h"

#include "json/document.h"
#include "json/filestream.h"
#include "json/prettywriter.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "cocos2d.h"

//#include "../LogSystem/WWLogMaros.h"

//USING_NS_CC_EXT;

using namespace rapidjson;

typedef enum JsonStatus{
	kJsonSuccess,                   //json解析成功
	kJsonParseErr,                  //json的dom树解析失败，非标准json格式
	kJsonDecodeErr,                 //json的属性解析错误，例如：getInt获取int值，但实际中json存储的是string值，即目标值和实际值的类型不一致的错误
	kJsonFileOpenSuccess,           //json文件打开成功
	kJsonFileOpenErr,               //json文件打开失败
}wwJsonStatus;

class WWJsonManager {
public:
	WWJsonManager();
	virtual ~WWJsonManager();

	std::string getContent();
	JsonStatus setContent(const char* content, std::string& error);

	JsonStatus readInt(const char* name, int& value);
	JsonStatus readBool(const char* name, bool& value);
	JsonStatus readString(const char* name, std::string& value);
	JsonStatus readStringArray(const char* name, std::vector<std::string>& value);
	JsonStatus readIntArray(const char* name, std::vector<int>& value);
	JsonStatus readObjectArray(const char* name, std::vector<std::map<std::string , std::string>>& vectors);

	void writeInt(const char* name, int value);
	void writeBool(const char* name, bool value);
	void writeString(const char* name, const char* value);
	void writeStringArray(const char* name, std::vector<std::string> &values);
	void writeIntArray(const char* name, std::vector<int> &values);

	/**
	 * 遍历json
	 * 暂时没有递归处理多层嵌套
	 * 支持int、bool、string、int数组、string数组
	 */
	void traverseJson();

	static JsonStatus getJsonFileStr(std::string filename, std::string& json);
private:
    Document _doc;
    rapidjson::Value _root;
};

#endif /* __HD_DDZ_CONTROL_WWCDBMANAGER_WWJSONMANAGER_H__ */

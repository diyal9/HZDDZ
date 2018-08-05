/*
 * WWXMLManager.h
 *
 *  Created on: 2014年5月20日
 *      Author: wly
 */

#ifndef _WWXMLMANAGER_H_
#define _WWXMLMANAGER_H_

#include "cocos2d.h"
//#include "tinyxml2.h"
#include "tinyxml2/tinyxml2.h"

/*
 *   属性类，这里属性是只读的，修改请使用WWXMLNode类
 *   <express name="test" value="nothing">This is a test!</express>
 *   name 和 value 就是结点的属性
 */
class WWXMLAttribute
{
public:

	WWXMLAttribute(const tinyxml2::XMLAttribute *pXMLAttribute);

	virtual ~WWXMLAttribute();

	//下一个属性值
	WWXMLAttribute next();

	//获取属性名称
	const char* getName();

	//获取string类型属性值
	const char* value();

	//获取int类型属性值
	int intValue();

	//获取bool类型属性值
	bool boolValue();

	//获取float类型属性值
	float floatValue();

	//获取double类型属性值
	double doubleValue();

	//返回是否是空
	bool isNULL();

private:
	//文档属性对象
	const tinyxml2::XMLAttribute *m_pXMLAttribute;
};


/*
 *  节点类，封装对节点的各种操作.
 *  如下类型节点
 *	<example name="ok", flag="1">text</example>
 *   
 *  example是 nodeName
 *  name和flag是attribute
 *  text是 nodeValue
 */
class WWXMLNode
{
public:

	WWXMLNode(tinyxml2::XMLElement *pXMLElem);

	WWXMLNode(tinyxml2::XMLElement *pXMLElem, tinyxml2::XMLDocument *pXMLDocument);

	virtual ~WWXMLNode();

	//增加子节点
	WWXMLNode addChildNode(const char* name);

	//查找子节点，默认返回第一个子节点
	WWXMLNode findChildNode(const char* name = NULL);

	//查找下一个兄弟节点，默认返回下面第一个兄弟节点
	WWXMLNode findSlibingNode(const char* name =NULL);

	//查找上一个兄弟节点,默认返回上面第一个兄弟节点
	WWXMLNode preSlibingNode(const char* name = NULL);

	//设置节点属性值
	void setAttributeValue(const char* name, const char* value);

	//获取指定的属性值
	const char* getAttributeValue(const char* name);

	//删除一个指定名称的属性值
	void deleteAttribute(const char* name);

	//设置节点名称
	void setNodeName(const char* name);

	//获取节点名称
	const char* getNodeName();

	//设置节点值
	void setNodeValue(const char* value);

	//获取节点值
	const char* getNodeValue();

	//获取属性，默认返回第一个属性
	WWXMLAttribute firstAttribute(const char* name = NULL);

	//删除本节点
	void removeSelf();

	//删除所有子节点
	void removeAllChildNode();

	//是否是空节点
	bool isNULL();

private:
	tinyxml2::XMLDocument *m_pXMLDocument;
	tinyxml2::XMLElement *m_pXMLElem;
};

/*
 * XML管理类，使用tinyxml2封装了操作xml的细节，需要配合WWXMLNode类使用。
 * 封装了CCUserDefault提供的功能
 *
 * example:
 * WWXMLManager myXML;
 * myXML.createXMLFile("myXML.xml", "TestXMLManager");
 * WWXMLNode itemNode = myXML.getXMLRootNode().addChildNode("item");
 * itemNode.setAttributeValue("flag", "true");
 * myXML.saveXMLFile();
 */
class WWXMLManager
{
public:
	WWXMLManager();

	WWXMLManager(const char* strXMLPath);

	static WWXMLManager* getInstance();

	virtual ~WWXMLManager(void);

	//加载xml文件，utf-8格式文件
	bool loadXMLFile(const char* strXmlPath);

	//获取xml文件根目录值
	const char* getXMLRootKeyValue();

	//获取根结点
	WWXMLNode getXMLRootNode();

	//创建xml文件,默认为utf-8编码
	bool createXMLFile(const char* strFileName, const char* rootNode="root");

	//保存文件，修改后需要调用此方法
	bool saveXMLFile();


	/*
	 *  以下方法适用于引擎提供的默认XML文件，可使用getXMLFilePath获取文件路径
	 *  对应于CCUserDefault类
	 */
public:
	static WWXMLManager myXML;
	//根据key获取bool的值，当key值不存在时，返回defaultValue
	static bool getDefaultXMLBoolForKey(const char* pKey);
	static bool getDefaultXMLBoolForKey(const char* pKey, bool defaultValue);

	static int getDefaultXMLIntegerForKey(const char* pKey);
	static int getDefaultXMLIntegerForKey(const char* pKey, int defaultValue); 

	static float getDefaultXMLFloatForKey(const char* pKey);
	static float getDefaultXMLFloatForKey(const char* pKey, float defaultValue);

	static double getDefaultXMLDoubleForKey(const char* pKey);
	static double getDefaultXMLDoubleForKey(const char* pKey, double defaultValue);

	static std::string getDefaultXMLStringForKey(const char* pKey);
	static std::string getDefaultXMLStringForKey(const char* pKey, const std::string & defaultValue);

	//根据key值以bool设置值
	static void setDefaultXMLBoolForKey(const char* pKey, bool value);

    static void setDefaultXMLIntegerForKey(const char* pKey, int value);

    static void setDefaultXMLFloatForKey(const char* pKey, float value);

    static void setDefaultXMLDoubleForKey(const char* pKey, double value);

    static void setDefaultXMLStringForKey(const char* pKey, const std::string & value);

    static void purgeSharedUserDefault();

	//获取默认的XML文件路径
    const static std::string& getDefaultXMLFilePath();
	//xml文件是否存在
	static bool isXmlFileExist(const char* strFilePath);

	/*
	 *   说明：获取项目的字符串资源，类似如下的文件格式
	 *   参数：键值，段名值。
	 *   例子：
	 *   <?xml version="1.0" encoding="utf-8"?>
	 *   <resources>
	 *       <HallScene>
	 *           <string name="Str_app_name">蛙蛙斗地主</string> 
	 *           <string name="Str_chujiRoom">初级房</string> 
	 *       </HallScene>
	 *       <GameScene>
	 *           <string name="Str_app_name">蛙蛙斗地主</string> 
	 *       </GameScene>
	 *    </resources>
	 *
	 */
	std::string getWWStringFromXML(const std::string &strKey, const std::string &strSection);

private:
	tinyxml2::XMLDocument *m_pXMLDocument;

	//文件路径，每次load时改变
	std::string m_strXMLFilePath;
};


#endif



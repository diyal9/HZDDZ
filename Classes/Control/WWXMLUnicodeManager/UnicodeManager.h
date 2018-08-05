//
//  UnicodeManager.h
//  WW_DDZ_V2
//
//  Created by wildcat on 5/2/15.
//
//

#ifndef __WW_DDZ_V2__UnicodeManager__
#define __WW_DDZ_V2__UnicodeManager__

#include "cocos2d.h"
#include "Control/WWXMLUnicodeManager/WWXMLManager.h"

class UnicodeManager
{
public:
    UnicodeManager();
    
    UnicodeManager(const char* strXMLPath);
    
    virtual ~UnicodeManager(void);
    
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
    static std::string getWWStringFromXML(const std::string &strKey, const std::string &strSection);
    
    static std::string getUnicodeStringValue(std::string text);
    
private:
    tinyxml2::XMLDocument *m_pXMLDocument;
    
    //文件路径，每次load时改变
    std::string m_strXMLFilePath;
};

#endif /* defined(__WW_DDZ_V2__UnicodeManager__) */

//
//  UnicodeManager.cpp
//  WW_DDZ_V2
//
//  Created by wildcat on 5/2/15.
//
//

#include "UnicodeManager.h"
#include "BaseCore/WWBTools/WW_GBK_TO_UTF8.h"

USING_NS_CC;

static const std::string g_str_UnicodeStringPath = "XML/WWUnicode.xml";

//static const std::string g_str_UnicodeStringPath = "XML/WWUnicodeKey.xml";

UnicodeManager::UnicodeManager():
m_pXMLDocument(NULL),
m_strXMLFilePath("")
{
    
}

UnicodeManager::UnicodeManager(const char* strXMLPath):
m_pXMLDocument(NULL),
m_strXMLFilePath(strXMLPath)
{
    loadXMLFile(strXMLPath);
}

UnicodeManager::~UnicodeManager(void)
{
    if (m_pXMLDocument)
    {
        delete m_pXMLDocument;
    }
}

bool UnicodeManager::loadXMLFile(const char* strXmlPath)
{
    //加载同一文件时，直接返回
    if (strcmp(m_strXMLFilePath.c_str(), strXmlPath) == 0)
    {
        return true;
    }
    
    m_strXMLFilePath = strXmlPath;
    //首先置空，防止重复加载
    if (m_pXMLDocument)
    {
        delete m_pXMLDocument;
        m_pXMLDocument = NULL;
    }
    
    m_pXMLDocument = new tinyxml2::XMLDocument();
    if (NULL == m_pXMLDocument)
    {
        return false;
    }
    
    //使用文件内容加载，直接加载可能会有兼容性问题，字符流内存需要手动释放
    std::string strXmlBuffer = CCFileUtils::getInstance()->getStringFromFile(strXmlPath);
    if (strXmlBuffer.empty())
    {
        CCLOG("UnicodeManager::%s file data is empty!", strXmlPath);
        return false;
    }
    
    //解析XML字符流
    if (tinyxml2::XML_SUCCESS != m_pXMLDocument->Parse(strXmlBuffer.c_str(), strXmlBuffer.length()))
    {
        CCLOG("UnicodeManager::%s Parse data error!", strXmlPath);
        return false;
    }
    
    return true;
}

const char* UnicodeManager::getXMLRootKeyValue()
{
    if (m_pXMLDocument)
    {
        return getXMLRootNode().getNodeName();
    }
    return "";
}

WWXMLNode UnicodeManager::getXMLRootNode()
{
    CCLOG("1) UnicodeManager::getXMLRootNode()");
    if (m_pXMLDocument)
    {
        CCLOG("2) UnicodeManager::getXMLRootNode()");
        WWXMLNode node(m_pXMLDocument->RootElement(), m_pXMLDocument);
        return node;
    }
    return NULL;
}

bool UnicodeManager::createXMLFile(const char* strFileName, const char* rootNode/* ="root" */)
{
    //利用tinyxml2方法创建xml文件
    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    if (NULL == pDoc)
    {
        return false;
    }
    
    bool bRet = false;
    do
    {
        tinyxml2::XMLDeclaration *pDeclaration = pDoc->NewDeclaration(NULL);
        if (NULL == pDeclaration)
        {
            bRet = false;
            break;
        }
        pDoc->LinkEndChild(pDeclaration);
        tinyxml2::XMLElement *pRootEle = pDoc->NewElement(rootNode);
        if (NULL == pRootEle)
        {
            bRet = false;
            break;
        }
        pDoc->LinkEndChild(pRootEle);
        
        if (tinyxml2::XML_SUCCESS != pDoc->SaveFile(strFileName))
        {
            bRet = false;
            break;
        }
        bRet = true;
    } while (0);
    
    if(pDoc)
    {
        delete pDoc;
    }
    
    //创建成功后加载
    loadXMLFile(strFileName);
    
    return bRet;
}

bool UnicodeManager::saveXMLFile()
{
    if (m_pXMLDocument)
    {
        if (tinyxml2::XML_SUCCESS != m_pXMLDocument->SaveFile(m_strXMLFilePath.c_str()))
        {
            return false;
        }
    }
    return true;
}

bool UnicodeManager::getDefaultXMLBoolForKey(const char* pKey)
{
    return CCUserDefault::getInstance()->getBoolForKey(pKey);
}

bool UnicodeManager::getDefaultXMLBoolForKey(const char* pKey, bool defaultValue)
{
    return CCUserDefault::getInstance()->getBoolForKey(pKey, defaultValue);
}

int UnicodeManager::getDefaultXMLIntegerForKey(const char* pKey)
{
    return CCUserDefault::getInstance()->getIntegerForKey(pKey);
}

int UnicodeManager::getDefaultXMLIntegerForKey(const char* pKey, int defaultValue)
{
    return CCUserDefault::getInstance()->getIntegerForKey(pKey, defaultValue);
}

float UnicodeManager::getDefaultXMLFloatForKey(const char* pKey)
{
    return CCUserDefault::getInstance()->getFloatForKey(pKey);
}

float UnicodeManager::getDefaultXMLFloatForKey(const char* pKey, float defaultValue)
{
    return CCUserDefault::getInstance()->getFloatForKey(pKey, defaultValue);
}

double UnicodeManager::getDefaultXMLDoubleForKey(const char* pKey)
{
    return CCUserDefault::getInstance()->getDoubleForKey(pKey);
}

double UnicodeManager::getDefaultXMLDoubleForKey(const char* pKey, double defaultValue)
{
    return CCUserDefault::getInstance()->getDoubleForKey(pKey, defaultValue);
}

std::string UnicodeManager::getDefaultXMLStringForKey(const char* pKey)
{
    return CCUserDefault::getInstance()->getStringForKey(pKey);
}

std::string UnicodeManager::getDefaultXMLStringForKey(const char* pKey, const std::string & defaultValue)
{
    return CCUserDefault::getInstance()->getStringForKey(pKey, defaultValue);
}

void UnicodeManager::setDefaultXMLBoolForKey(const char* pKey, bool value)
{
    CCUserDefault::getInstance()->setBoolForKey(pKey, value);
}

void UnicodeManager::setDefaultXMLIntegerForKey(const char* pKey, int value)
{
    CCUserDefault::getInstance()->setIntegerForKey(pKey, value);
}

void UnicodeManager::setDefaultXMLFloatForKey(const char* pKey, float value)
{
    CCUserDefault::getInstance()->setFloatForKey(pKey, value);
}

void UnicodeManager::setDefaultXMLDoubleForKey(const char* pKey, double value)
{
    CCUserDefault::getInstance()->setDoubleForKey(pKey, value);
}

void UnicodeManager::setDefaultXMLStringForKey(const char* pKey, const std::string & value)
{
    CCUserDefault::getInstance()->setStringForKey(pKey, value);
}

void UnicodeManager::purgeSharedUserDefault()
{
    CCUserDefault::getInstance()->destroyInstance();
}

const std::string& UnicodeManager::getDefaultXMLFilePath()
{
    return CCUserDefault::getInstance()->getXMLFilePath();
}

bool UnicodeManager::isXmlFileExist(const char* strFilePath)
{
    FILE *fp = fopen(strFilePath, "r");
    bool bRet = false;
    
    if (fp)
    {
        bRet = true;
        fclose(fp);
    }
    
    return bRet;
}

std::string UnicodeManager::getWWStringFromXML(const std::string &strKey, const std::string &strSection)
{
    if (strKey.empty() || strSection.empty())
    {
        CCLOG("key or section is empty!");
        return "";
    }
    
    //加载XML文件
    UnicodeManager myXML;
    
    if (!myXML.loadXMLFile(g_str_UnicodeStringPath.c_str()))
    {
        CCLOG("load string resource error!");
        return "";
    }
    
    CCLOG("1) UnicodeManager::getWWStringFromXML");
    //循环获取节点名为stirng,属性名为name，属性值为strKey的节点
    WWXMLNode n1 = myXML.getXMLRootNode();
    if (!n1.isNULL())
    {
        CCLOG("WWXMLNode n1");
    }
    
    WWXMLNode node = myXML.getXMLRootNode();
    if (!node.isNULL())
    {
        CCLOG("2) UnicodeManager::getWWStringFromXML");
        WWXMLNode childNode = node.findChildNode("string");
        while (!childNode.isNULL())
        {
            CCLOG("3) UnicodeManager::getWWStringFromXML");
            std::string value = childNode.firstAttribute("name").value();
            CCLOG("value:%s, key:%s", value.c_str(), strKey.c_str());
            if (strcmp(value.c_str(), strKey.c_str()) == 0)
            {
                
                std::string strResult = childNode.getNodeValue();
                //win32，模拟器上显示需要转换成gbk编码
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                GBKToUTF8(strResult, "GBK", "UTF-8");
#endif
                return strResult;
            }
            childNode = childNode.findSlibingNode("string");
        }
    }
    
//    if (strKey.empty() || strSection.empty())
//    {
//        CCLOG("key or section is empty!");
//        return "";
//    }
//    
//    //加载XML文件
//    UnicodeManager myXML;
//    
//    if (!myXML.loadXMLFile(g_str_UnicodeStringPath.c_str()))
//    {
//        CCLOG("load string resource error!");
//        return "";
//    }
//    
//    CCLOG("1) UnicodeManager::getWWStringFromXML");
//    //循环获取节点名为stirng,属性名为name，属性值为strKey的节点
//    WWXMLNode n1 = myXML.getXMLRootNode();
//    if (!n1.isNULL())
//    {
//        CCLOG("WWXMLNode n1");
//    }
//    
//    WWXMLNode node = myXML.getXMLRootNode();
//    if (!node.isNULL())
//    {
//        CCLOG("2) UnicodeManager::getWWStringFromXML");
//        WWXMLNode childNode = node.findChildNode(strKey.c_str());
//        if (!childNode.isNULL())
//        {
//            std::string strResult = childNode.getNodeValue();
//            //win32，模拟器上显示需要转换成gbk编码
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//            GBKToUTF8(strResult, "GBK", "UTF-8");
//#endif
//            return strResult;
//        }
//    }
    
    return "";
}

std::string UnicodeManager::getUnicodeStringValue(std::string text)
{
    if (text == "")
    {
        return "";
    }
    
    std::string test = "参";
    const unsigned long len = strlen(test.c_str());
    
    std::string strValue = "";
    
    unsigned long posU = text.find_first_of("u");
    while (posU != std::string::npos)
    {
        std::string temp = text.substr(posU + 1, 4);
        std::string lastLetter = temp.substr(3, 1);
        std::string unicodeTemp = temp.replace(3, 1, "0");
        std::string unicodeValue = getWWStringFromXML(unicodeTemp, "unicode");
//        CCLOG("unicodeTemp:%s, unicodeValue:%s", unicodeTemp.c_str(), unicodeValue.c_str());
        
        //
        if (unicodeTemp.compare("4e00") < 0 || unicodeTemp.compare("9fa0") > 0)
        {
            CCLOG("unicodeTemp.compare(4e00) < 0 || unicodeTemp.compare(9fa0) > 0");
            break;
        }
        //
        
        int pos = 0;
        if (lastLetter == "0")
        {
            pos = 0;
        }
        else if (lastLetter == "1")
        {
            pos = 1;
        }
        else if (lastLetter == "2")
        {
            pos = 2;
        }
        else if (lastLetter == "3")
        {
            pos = 3;
        }
        else if (lastLetter == "4")
        {
            pos = 4;
        }
        else if (lastLetter == "5")
        {
            pos = 5;
        }
        else if (lastLetter == "6")
        {
            pos = 6;
        }
        else if (lastLetter == "7")
        {
            pos = 7;
        }
        else if (lastLetter == "8")
        {
            pos = 8;
        }
        else if (lastLetter == "9")
        {
            pos = 9;
        }
        else if (lastLetter == "a")
        {
            pos = 10;
        }
        else if (lastLetter == "b")
        {
            pos = 11;
        }
        else if (lastLetter == "c")
        {
            pos = 12;
        }
        else if (lastLetter == "d")
        {
            pos = 13;
        }
        else if (lastLetter == "e")
        {
            pos = 14;
        }
        else if (lastLetter == "f")
        {
            pos = 15;
        }
        
        std::string value = unicodeValue.substr(pos * len, len);

        strValue = strValue + value;
        
        if (posU + 1 < strlen(text.c_str()))
        {
            posU = text.find_first_of("u", posU + 1);
        }
        else
        {
            break;
        }
    }
    
    
    return strValue;
}

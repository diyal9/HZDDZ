/*
 * WWXMLManager.h
 *
 *  Created on: 2014年5月20日
 *      Author: wly
 */

#include "WWXMLManager.h"
#include "WWMacros.h"
#include "BaseCore/WWBTools/WW_GBK_TO_UTF8.h"
USING_NS_CC;

WWXMLAttribute::WWXMLAttribute(const tinyxml2::XMLAttribute *pXMLAttribute):
m_pXMLAttribute(pXMLAttribute)
{
	
}

WWXMLAttribute::~WWXMLAttribute()
{

}

WWXMLAttribute WWXMLAttribute::next()
{
	if (m_pXMLAttribute)
	{
		return m_pXMLAttribute->Next();
	}
	return NULL;
}

const char* WWXMLAttribute::getName()
{
	if (m_pXMLAttribute)
	{
		const char* pName = m_pXMLAttribute->Name();
		if (pName)
		{
			return pName;
		}
	}
	return "";
}

const char* WWXMLAttribute::value()
{
	if (m_pXMLAttribute)
	{
		const char* pValue = m_pXMLAttribute->Value();
		if (pValue)
		{
			return pValue;
		}
	}
	return "";
}

int WWXMLAttribute::intValue()
{
	if (m_pXMLAttribute)
	{
		return m_pXMLAttribute->IntValue();
	}
	return -1;
}

bool WWXMLAttribute::boolValue()
{
	if (m_pXMLAttribute)
	{
		return m_pXMLAttribute->BoolValue();
	}
	return false;
}

float WWXMLAttribute::floatValue()
{
	if (m_pXMLAttribute)
	{
		return m_pXMLAttribute->FloatValue();
	}
	return -1;
}

double WWXMLAttribute::doubleValue()
{
	if (m_pXMLAttribute)
	{
		return m_pXMLAttribute->DoubleValue();
	}
	return -1;
}

bool WWXMLAttribute::isNULL()
{
	return NULL == m_pXMLAttribute;
}

WWXMLNode::WWXMLNode(tinyxml2::XMLElement *pElem):
m_pXMLElem(pElem),
m_pXMLDocument(NULL)
{

}

WWXMLNode::WWXMLNode(tinyxml2::XMLElement *pElem, tinyxml2::XMLDocument *pDocument):
m_pXMLElem(pElem),
m_pXMLDocument(pDocument)
{
	
}

WWXMLNode::~WWXMLNode()
{

}

WWXMLNode WWXMLNode::addChildNode(const char* name)
{
	if (m_pXMLElem && m_pXMLDocument)
	{
		tinyxml2::XMLElement *pElem = m_pXMLDocument->NewElement(name);
		if (pElem)
		{
			m_pXMLElem->LinkEndChild(pElem);
			WWXMLNode node(pElem, m_pXMLDocument);
			return node;
		}
	}
	return NULL;
}

WWXMLNode WWXMLNode::findChildNode(const char* name /* = NULL */)
{
	if (m_pXMLElem && m_pXMLDocument)
	{
		if (NULL == name)
		{
			WWXMLNode node(m_pXMLElem->FirstChildElement(), m_pXMLDocument);
			return node;
		}
		else
		{
			WWXMLNode node(m_pXMLElem->FirstChildElement(name), m_pXMLDocument);
			return node;	
		}
	}
	return NULL;
}

WWXMLNode WWXMLNode::findSlibingNode(const char* name /* =NULL */)
{
	if (m_pXMLElem && m_pXMLDocument)
	{
		if (NULL == name)
		{
			WWXMLNode node(m_pXMLElem->NextSiblingElement(), m_pXMLDocument);
			return node;
		}
		else
		{
			WWXMLNode node(m_pXMLElem->NextSiblingElement(name), m_pXMLDocument);
			return node;	
		}
	}
	return NULL;
}

WWXMLNode WWXMLNode::preSlibingNode(const char* name /* =NULL */)
{
	if (m_pXMLElem && m_pXMLDocument)
	{
		if (NULL == name)
		{
			WWXMLNode node(m_pXMLElem->PreviousSiblingElement(), m_pXMLDocument);
			return node;
		}
		else
		{
			WWXMLNode node(m_pXMLElem->PreviousSiblingElement(name), m_pXMLDocument);
			return node;	
		}
	}
	return NULL;
}

void WWXMLNode::setAttributeValue(const char* name, const char* value)
{
	if (m_pXMLElem && NULL != name)
	{
		m_pXMLElem->SetAttribute(name, value);
	}
}

const char* WWXMLNode::getAttributeValue(const char* name)
{
	if (m_pXMLElem && NULL != name)
	{
		const char* pName = m_pXMLElem->Attribute(name);
		if (NULL == pName)
		{
			return "";
		}
		return pName;
	}
	return "";
}

void WWXMLNode::deleteAttribute(const char* name)
{
	if (m_pXMLElem)
	{
		m_pXMLElem->DeleteAttribute(name);
	}
}

void WWXMLNode::setNodeName(const char* name)
{
	if (m_pXMLElem && NULL != name)
	{
		m_pXMLElem->SetName(name);
	}
}

const char* WWXMLNode::getNodeName()
{
	if (m_pXMLElem)
	{
		const char* pName = m_pXMLElem->Name();
		if (NULL == pName)
		{
			return "";
		}
		return pName;
	}
	return "";
}

void WWXMLNode::setNodeValue(const char* value)
{
	if (m_pXMLElem && m_pXMLDocument)
	{
		tinyxml2::XMLText *pText = m_pXMLDocument->NewText(value);
		m_pXMLElem->LinkEndChild(pText);
	}
}

const char* WWXMLNode::getNodeValue()
{
	if (m_pXMLElem)
	{
		const char* pValue = m_pXMLElem->GetText();
		if (NULL == pValue)
		{
			return "";
		}
		return pValue;
	}
	return "";
}

WWXMLAttribute WWXMLNode::firstAttribute(const char* name /*= NULL*/)
{
	if (m_pXMLElem)
	{
		if (NULL == name)
		{
			return m_pXMLElem->FirstAttribute();
		}
		else
		{
			WWXMLAttribute attribute = m_pXMLElem->FirstAttribute();
			while (!attribute.isNULL())
			{
				if (strcmp(name, attribute.getName()) == 0)
				{
					return attribute;
				}
				attribute = attribute.next();
			}
		}
		
	}
	return NULL;
}

void WWXMLNode::removeSelf()
{
	if (m_pXMLElem)
	{
		if (m_pXMLElem->Parent())
		{
			m_pXMLElem->Parent()->DeleteChild((tinyxml2::XMLNode*)m_pXMLElem);
		}
	}
}

void WWXMLNode::removeAllChildNode()
{
	if (m_pXMLElem)
	{
		m_pXMLElem->DeleteChildren();
	}
}

bool WWXMLNode::isNULL()
{
	return m_pXMLElem == NULL;
}

WWXMLManager* Instance = NULL;

WWXMLManager::WWXMLManager():
m_pXMLDocument(NULL),
m_strXMLFilePath("")
{
	loadXMLFile(WWSTRING_XML_PATH.c_str());
}

WWXMLManager::WWXMLManager(const char* strXMLPath):
m_pXMLDocument(NULL),
m_strXMLFilePath(strXMLPath)
{
	loadXMLFile(strXMLPath);
}

WWXMLManager::~WWXMLManager(void)
{
	if (m_pXMLDocument)
	{
		delete m_pXMLDocument;
	}
	
	if(Instance){
		delete(Instance);
	}
}

WWXMLManager* WWXMLManager::getInstance()
{
	if (!Instance)
	{
		Instance = new WWXMLManager();
	}

	return Instance;
}

bool WWXMLManager::loadXMLFile(const char* strXmlPath)
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
		CCLOG("WWXMLManager::%s file data is empty!", strXmlPath);
		return false;
	}

	//解析XML字符流
	if (tinyxml2::XML_SUCCESS != m_pXMLDocument->Parse(strXmlBuffer.c_str(), strXmlBuffer.length()))
	{
		CCLOG("WWXMLManager::%s Parse data error!", strXmlPath);
		return false;
	}

	return true;
}

const char* WWXMLManager::getXMLRootKeyValue()
{
	if (m_pXMLDocument)
	{
		return getXMLRootNode().getNodeName();
	}
	return "";
}

WWXMLNode WWXMLManager::getXMLRootNode()
{
	if (m_pXMLDocument)
	{
		WWXMLNode node(m_pXMLDocument->RootElement(), m_pXMLDocument);
		return node;
	}
	return NULL;
}

bool WWXMLManager::createXMLFile(const char* strFileName, const char* rootNode/* ="root" */)
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

bool WWXMLManager::saveXMLFile()
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

bool WWXMLManager::getDefaultXMLBoolForKey(const char* pKey)
{
	return CCUserDefault::getInstance()->getBoolForKey(pKey);
}

bool WWXMLManager::getDefaultXMLBoolForKey(const char* pKey, bool defaultValue)
{
	return CCUserDefault::getInstance()->getBoolForKey(pKey, defaultValue);
}

int WWXMLManager::getDefaultXMLIntegerForKey(const char* pKey)
{
	return CCUserDefault::getInstance()->getIntegerForKey(pKey);
}

int WWXMLManager::getDefaultXMLIntegerForKey(const char* pKey, int defaultValue)
{
	return CCUserDefault::getInstance()->getIntegerForKey(pKey, defaultValue);
}

float WWXMLManager::getDefaultXMLFloatForKey(const char* pKey)
{
	return CCUserDefault::getInstance()->getFloatForKey(pKey);
}

float WWXMLManager::getDefaultXMLFloatForKey(const char* pKey, float defaultValue)
{
	return CCUserDefault::getInstance()->getFloatForKey(pKey, defaultValue);
}

double WWXMLManager::getDefaultXMLDoubleForKey(const char* pKey)
{
	return CCUserDefault::getInstance()->getDoubleForKey(pKey);
}

double WWXMLManager::getDefaultXMLDoubleForKey(const char* pKey, double defaultValue)
{
	return CCUserDefault::getInstance()->getDoubleForKey(pKey, defaultValue);
}

std::string WWXMLManager::getDefaultXMLStringForKey(const char* pKey)
{
	return CCUserDefault::getInstance()->getStringForKey(pKey);
}

std::string WWXMLManager::getDefaultXMLStringForKey(const char* pKey, const std::string & defaultValue)
{
	return CCUserDefault::getInstance()->getStringForKey(pKey, defaultValue);
}

void WWXMLManager::setDefaultXMLBoolForKey(const char* pKey, bool value)
{
	CCUserDefault::getInstance()->setBoolForKey(pKey, value);
}

void WWXMLManager::setDefaultXMLIntegerForKey(const char* pKey, int value)
{
	CCUserDefault::getInstance()->setIntegerForKey(pKey, value);
}

void WWXMLManager::setDefaultXMLFloatForKey(const char* pKey, float value)
{
	CCUserDefault::getInstance()->setFloatForKey(pKey, value);
}

void WWXMLManager::setDefaultXMLDoubleForKey(const char* pKey, double value)
{
	CCUserDefault::getInstance()->setDoubleForKey(pKey, value);
}

void WWXMLManager::setDefaultXMLStringForKey(const char* pKey, const std::string & value)
{
	CCUserDefault::getInstance()->setStringForKey(pKey, value);
}

void WWXMLManager::purgeSharedUserDefault()
{
	CCUserDefault::getInstance()->destroyInstance();
}

const std::string& WWXMLManager::getDefaultXMLFilePath()
{
	return CCUserDefault::getInstance()->getXMLFilePath();
}

bool WWXMLManager::isXmlFileExist(const char* strFilePath)
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


std::string WWXMLManager::getWWStringFromXML(const std::string &strKey, const std::string &strSection)
{
	if (strKey.empty() || strSection.empty())
	{
		CCLOG("key or section is empty!");
		return "";
	}

	//加载XML文件
	/*WWXMLManager myXML;
	if (!myXML.loadXMLFile(WWSTRING_XML_PATH.c_str()))
	{
		CCLOG("load string resource error!");
		return "";
	}*/

	//循环获取节点名为stirng,属性名为name，属性值为strKey的节点
	WWXMLNode node = Instance->getXMLRootNode().findChildNode(strSection.c_str());
	if (!node.isNULL())
	{
		WWXMLNode childNode = node.findChildNode("string");
		while (!childNode.isNULL())
		{
			if (strcmp(childNode.firstAttribute("name").value(), strKey.c_str()) == 0)
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

	return "";
}

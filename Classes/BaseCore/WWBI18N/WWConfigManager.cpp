/*
 * ConfigManager.cpp
 *
 *  Created on: 2014年5月13日
 *      Author: hp
 */

/* 更改:CCString -> __String, CCDictionary -> __Dictionary
 * 目的：适配cocos2dx-3.2
 * wildcat 20140905
 */

#include "WWConfigManager.h"
USING_NS_CC;
WWConfigManager::WWConfigManager() :
		m_Dictionary(NULL)
{

}

WWConfigManager::~WWConfigManager() {
	CC_SAFE_RELEASE_NULL(m_Dictionary);
}

void WWConfigManager::loadXML(const char *pFileName) {
	CC_SAFE_RELEASE(m_Dictionary);
	m_Dictionary = __Dictionary::createWithContentsOfFile(pFileName);
	CC_SAFE_RETAIN(m_Dictionary);
}

const __String* WWConfigManager::getValue(const char *pKey) {
	if(m_Dictionary == NULL||!pKey ){
		return __String::create("");
	}
	__String *c_Value = (__String*)m_Dictionary->objectForKey(pKey);
	if(c_Value) return c_Value;
	return __String::create("");
}



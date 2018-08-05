/*
 * ConfigManager.h
 *
 *  Created on: 2014年5月13日
 *      Author: hp
 */

/* 更改:CCString -> __String, CCDictionary -> __Dictionary
 * 目的：适配cocos2dx-3.2
 * wildcat 20140905  
 */

#ifndef __HD_DDZ_BASECORE_CONFIGMANAGER_H__
#define __HD_DDZ_BASECORE_CONFIGMANAGER_H__
#include "cocos2d.h"
#include "../../WWMacros.h"

/**
 * 配置文件管理（socket地址、sp等配置的读取）
 */
class WWConfigManager: public Ref {
public:
	WWConfigManager(void);
	virtual ~WWConfigManager(void);
	SINGLE_INSTANCE_FUNC(WWConfigManager);
	/*
	 * 载入xml文件
	 */
	void loadXML(const char *pFileName);
	/**
	 * 获取相应的参数值（键在WWConfig.h）
	 */
	const cocos2d::__String* getValue(const char *pKey);
private:
	 cocos2d::__Dictionary* m_Dictionary;
};

#endif /* __HD_DDZ_BASECORE_CONFIGMANAGER_H__ */

#ifndef _WW_HEART_CHECK_MANAGER_
#define _WW_HEART_CHECK_MANAGER_

#include "cocos2d.h"
#include "WWMacros.h"


const int HEART_INTERVAL = 1800;//心跳间隔

class WWHeartCheckManager
{
public:
	WWHeartCheckManager();
	virtual ~WWHeartCheckManager();
	SINGLE_INSTANCE_FUNC(WWHeartCheckManager);

	void heartCount();//心跳计数
	void reset();//重置心跳计数

	void sendHeartMsg();

protected:
	int m_heartCount;//心跳计数
};

#endif // !_WW_HEART_CHECK_MANAGER_

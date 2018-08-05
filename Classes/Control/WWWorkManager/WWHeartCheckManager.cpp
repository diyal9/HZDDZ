
#include "WWHeartCheckManager.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"

USING_NS_CC;

WWHeartCheckManager::WWHeartCheckManager():m_heartCount(0)
{
}
WWHeartCheckManager::~WWHeartCheckManager()
{
}

void WWHeartCheckManager::heartCount()
{
	m_heartCount++;
}

void WWHeartCheckManager::reset()
{
	m_heartCount = 0;
}

void WWHeartCheckManager::sendHeartMsg()
{
	heartCount();
	if(m_heartCount > HEART_INTERVAL)
	{
		WWNetWorkManager::getInstance()->SendPrepare();
		SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

		pClientSocket->WriteChar(18);
		pClientSocket->WriteChar(2);
		pClientSocket->WriteChar(1);

		pClientSocket->SendCommand();

		reset();
	}
	
}
//
//  WWNetWorkManager.cpp
//  
//  网络管理单例
//
//  Created by 尹艳龙 on 2015-04-22.
//
//

#include "WWNetWorkManager.h"
#include "Control/WWWorkManager/WWHeartCheckManager.h"
#include "BaseCore/WWBLog/LogUtil.h"

static WWNetWorkManager* g_pNetWorkManager = NULL;

WWNetWorkManager::WWNetWorkManager() : 
	m_pClientSocket(nullptr),
	m_isReconnect(false),
	m_isFirstJoin(true),
	m_recvMsg_Id(0),
	m_sendMsg_Id(0),
	m_isSucces(false)
{

}

WWNetWorkManager::~WWNetWorkManager()
{
	if(m_pClientSocket)
	{
		CC_SAFE_RELEASE_NULL(m_pClientSocket);
	}
}

WWNetWorkManager* WWNetWorkManager::getInstance()
{
	if(!g_pNetWorkManager)
	{
		g_pNetWorkManager = new WWNetWorkManager();
	}
	return g_pNetWorkManager;
}

//连接网络
bool WWNetWorkManager::Connect(const char* szServerIP, int wPort)
{

	if (m_pClientSocket)
		CC_SAFE_RELEASE_NULL(m_pClientSocket);

	if(!m_pClientSocket)
	{
		m_pClientSocket = SocketClient::create();
		m_pClientSocket->retain();
	}
	if(!m_pClientSocket->ConnectServer(szServerIP, wPort ))
	{

		return false;
	}

	//Socket连接成功，发送握手请求
	if (m_pClientSocket->IsConnect())
	{
		netShark();
	}

	return true;
}

//接受数据
void WWNetWorkManager::openRecvJob()
{
	//暂时按照刷新频率来执行接受函数
	//Director::getInstance()->getScheduler()->schedule(schedule_selector(WWNetWorkManager::netRecSchedule), this, 1.0/60, false);
	if (!m_pClientSocket || !(m_pClientSocket->IsConnect()))
	{
		Connect(IP_TEMP, PORT_TEMP);
	}

}

//关闭接受数据
void WWNetWorkManager::closeRecvJob()
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(WWNetWorkManager::netRecSchedule), this);
}

void WWNetWorkManager::netRecSchedule(float dt)
{
	this->RecvNetMessage();
}

//握手->在Connect后需要先做一次握手
void WWNetWorkManager::netShark()
{
	//握手
	m_pClientSocket->WriteChar(18); 
	m_pClientSocket->WriteChar(0); 
	m_pClientSocket->WriteChar(0); 
	m_pClientSocket->WriteChar(0); 
	m_pClientSocket->WriteChar(0); 
	m_pClientSocket->WriteChar(0); 
	m_pClientSocket->WriteChar(3); 
	m_pClientSocket->WriteBoolean(false);

	m_pClientSocket->SendConnect();
}

void  WWNetWorkManager::JumpSendMessage()
{

}

void WWNetWorkManager::SendPrepare()
{
	//清空发送数据
	m_pClientSocket->ClearSendBuffer();
	//封装包头
	m_sendMsg_Id++;
	//m_pClientSocket->WriteChar((char)m_recvMsg_Id); //写入一个字节m_recvMsg_Id
	//m_pClientSocket->WriteChar((char)m_sendMsg_Id);//写入一个字节m_sendMsg_Id

	m_pClientSocket->setM_recvMsg_Id(m_recvMsg_Id);
	m_pClientSocket->setM_sendMsg_Id(m_sendMsg_Id);
}

SocketClient* WWNetWorkManager::getCurSocket()
{
	if (!m_pClientSocket)
	{
		return nullptr;
	}
	return m_pClientSocket;
}

//接收网络数据
void WWNetWorkManager::RecvNetMessage()
{
	long recSize = m_pClientSocket->Recv();
	//心跳处理
	if(recSize > 0)
	{
		WWHeartCheckManager::getInstance()->reset();
	}else
	{
		if(m_pClientSocket->IsConnect())
			WWHeartCheckManager::getInstance()->sendHeartMsg();
	}
	if (recSize > 0)
	{
		int readLen = 0;//已读消息长度
		do
		{
			int n_msg_len = m_pClientSocket->ReadUnsignedShort(); //读取消息长度
			readLen += 2;

			if (n_msg_len >= 0xFFFF) //如果消息长度大于0xFFFF，则说明是超大包
			{
				n_msg_len = m_pClientSocket->ReadInt();
			}

			readLen += n_msg_len;
			LogUtil::getInstance()->writeLog("recv msg  a lot --------------------------- current length = %d, recSize = %d",readLen, recSize);

			m_recvMsg_Id = (int) m_pClientSocket->ReadChar();
			readLen++;
			LogUtil::getInstance()->writeLog("server recv：m_recvMsg_Id=%d", m_recvMsg_Id);
			int send_id = (int) m_pClientSocket->ReadChar();
			readLen++;
			LogUtil::getInstance()->writeLog("server recv：send_id=%d", send_id);

			int channelID; //服务器返回连接频道标示

			int msgType = m_pClientSocket->Read3CharToInt();
			LogUtil::getInstance()->writeLog("msgType=%x", msgType);

			switch (msgType)
			{
			case 0x7f7f7f: //心跳
				break;
			case 0x120000: //握手
				channelID = m_pClientSocket->ReadInt();
				CCLOG("channelID=%d", channelID);
				m_isSucces = m_pClientSocket->ReadBoolean();
				CCLOG("isSucess=%d", m_isSucces);

				m_sendMsg_Id = send_id;

				break;

			default:

				do
				{
					CC_BREAK_IF(m_pClientSocket->GetReadLength() <= 0);

					switch (msgType)
					{
						//通用消息
					case 0:
						onRootMessageReceived(msgType);
						break;
					default:
						onMessageReceived(msgType);
						break;
					}

				} while (0);
				break;
			}

			if(readLen >= recSize)//读取长度大于当前消息总长度
			{
				LogUtil::getInstance()->writeLog("recv msg ----------------------------- already deal all msg");
				break;
			}
			else
			{
				m_pClientSocket->SkipToReadIndex(readLen);
				LogUtil::getInstance()->writeLog("recv msg ---------skip to index--------------index = %d ",readLen);
			}
			
		}while(true);


	}
}

////网络数据分发处理
//void WWNetWorkManager::handlerMsg(int msgType)
//{
//	//循环检测发送
//	bool deal = false;
//
//	std::vector<WWIMsgFilter*>::iterator itfilter;
//	//遍历filters 通过onFilters()获取 msgTypes 判读是否存在，存在则通过onMessageReceived发送
//	for (itfilter = m_filters.begin(); itfilter != m_filters.end();
//		++itfilter)
//	{
//		std::vector<int> ifilters = (*itfilter)->onFilters();
//		std::vector<int>::iterator it_msgType;
//		it_msgType = std::find(ifilters.begin(), ifilters.end(), msgType);
//
//		if (it_msgType != ifilters.end())
//		{
//			deal = (*itfilter)->onMessageReceived(msgType);
//			if (deal)
//				return;
//		}
//	}
//
//	//TODO 异常消息处理
//}

void WWNetWorkManager::onMessageReceived(const int msgType)
{
	//循环检测发送
	bool deal = false;

	std::vector<WWIMsgFilter*>::iterator itfilter;
	//遍历filters 通过onFilters()获取 msgTypes 判读是否存在，存在则通过onMessageReceived发送
	for (itfilter = m_filters.begin(); itfilter != m_filters.end();
		++itfilter)
	{
		std::vector<int> ifilters = (*itfilter)->onFilters();
		std::vector<int>::iterator it_msgType;
		it_msgType = std::find(ifilters.begin(), ifilters.end(), msgType);

		if (it_msgType != ifilters.end())
		{
			deal = (*itfilter)->onMessageReceived(msgType);
			if (deal)
				return;
		}
	}
	//WWIMsgUnCatchFilter::getInstance()->onMessageReceived(msgType, buffer->copy());

}

void WWNetWorkManager::onRootMessageReceived(const int msgType)
{
	int requestType = m_pClientSocket->Read3CharToInt();//请求的消息头
	int userId = m_pClientSocket->ReadInt();
	int result = m_pClientSocket->ReadChar();
	std::string reason = m_pClientSocket->ReadString();
	int reasonType = m_pClientSocket->ReadShort();
	CCLOG("root msg recv::onRootMessageReceived:msgType= %x,userId=%d,result=%d,reason=%s,reasonType=%x", requestType, userId, result, reason.c_str(), reasonType);

	bool deal = false;
	// 循环检测发送
	//遍历filters 通过onRootFilters()获取 msgTypes 判读是否存在，存在则通过onRootMessageReceived发送

	std::vector<WWIMsgFilter*>::iterator itfilter;
	for (itfilter = m_filters.begin(); itfilter != m_filters.end();
		++itfilter)
	{
		std::vector<int> rootfilters = (*itfilter)->onRootFilters();
		std::vector<int>::iterator it_msgType;


		it_msgType = std::find(rootfilters.begin(), rootfilters.end(), requestType);

		if (it_msgType != rootfilters.end())
		{
			deal = (*itfilter)->onRootMessageReceived(requestType, userId,
				result, reason, reasonType);
			if (deal){
				return;
			}
		}
	}
	//WWIMsgUnCatchFilter::getInstance()->onRootMessageReceived(type, userId,
	//	result, reason, reasonType);


}

void WWNetWorkManager::registerMsgFilter(WWIMsgFilter *filter)
{

	CCAssert(filter && dynamic_cast<cocos2d::Ref*>(filter), "should not null");
	do
	{
		//判断是否为空，是否继承WWObject
		CC_BREAK_IF(!filter && !dynamic_cast<cocos2d::Ref*>(filter));
		//遍历判断是否存在，存在则不加入
		std::vector<WWIMsgFilter*>::iterator itr;

		itr = std::find(m_filters.begin(), m_filters.end(), filter);

		if (itr != m_filters.end())
		{
			return;
		}

		cocos2d::Ref* pObj = dynamic_cast<cocos2d::Ref*>(filter);
		CC_SAFE_RETAIN(pObj);

		m_filters.insert(m_filters.begin(), filter);

	} while (0);

}

void WWNetWorkManager::unregisterMsgFilter(WWIMsgFilter *filter)
{
	CCAssert(filter && dynamic_cast<cocos2d::Ref*>(filter), "should not null");
	do
	{
		//判断是否为空，是否继承WWObject
		CC_BREAK_IF(!filter && !dynamic_cast<cocos2d::Ref*>(filter));

		std::vector<WWIMsgFilter*>::iterator itr;
		itr = std::find(m_filters.begin(), m_filters.end(), filter);

		if (itr != m_filters.end())
		{
			m_filters.erase(itr);
		}

		cocos2d::Ref* pObj = dynamic_cast<cocos2d::Ref*>(filter);
		CC_SAFE_RELEASE(pObj);

	} while (0);

}

void WWNetWorkManager::setSendMsgId(int sendMsgId)
{
	this->m_sendMsg_Id = sendMsgId;
}

int WWNetWorkManager::getSendMsgId()
{
	return this->m_sendMsg_Id;
}

void WWNetWorkManager::setRecvMsgId(int recvMsgId)
{
	this->m_recvMsg_Id = recvMsgId;
}

int WWNetWorkManager::getRecvMsgId()
{
	return this->m_recvMsg_Id;
}

void WWNetWorkManager::setIsConnectSuccess(bool success)
{
	this->m_isSucces = success;
}

bool WWNetWorkManager::getIsConnectSuccess()
{
	return this->m_isSucces;
}


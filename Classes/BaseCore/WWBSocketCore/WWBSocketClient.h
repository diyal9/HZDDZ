#ifndef _H_LUA_SOCKET_CLIENT_H_
#define _H_LUA_SOCKET_CLIENT_H_

#include "cocos2d.h"

USING_NS_CC;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	//#include "stdafx.h"
	#include <WinSock2.h>
	#pragma comment(lib, "wsock32.lib")

#else

	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <unistd.h>
	#include <string.h>
	#include <signal.h>
	#include <time.h>
	#include <limits.h>
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/time.h>
	#include <sys/times.h>
	#include <sys/stat.h> 
	#include <sys/wait.h>
	#include <sys/sysinfo.h>
	#include <sys/epoll.h>

	#include <sys/socket.h>	

	#include <netdb.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h> 
	#include <arpa/inet.h>	

	#include <fcntl.h>

	#include <math.h> 

	#include <pthread.h>

	#include <errno.h>

	#include <assert.h>

	#ifndef INVALID_SOCKET
		#define INVALID_SOCKET (-1)
	#endif

	#ifndef SOCKET
		#define SOCKET long
	#endif

#endif

// ¶¨Òå ×î´óÊ¹ÓÃ Socket Counter
#define _Define_Max_Socket_Counter_ (16)

#define _Define_Socket_Buffer_Size_ (1024 * 4)

class SocketClient  : public Ref
{
public:

	SocketClient();
	virtual ~SocketClient();

	static SocketClient* create();

	bool ConnectServer(const char * pServerIP, unsigned short ServerPort);
	void CloseSocket(void);

	// ´«»Ø ÊÇ·ñ Á¬ÏßÖÐ
	bool IsConnect(void);

	// Çå³ý Recv Buffer
	void ClearRecvBuffer(void);

	//ÊÇ·ñ¶ÏÏßÖØÁ¬

	// Recv
	// ·µ»Ø½ÓÊÕ³¤¶È
	// > 0 ¶ÁÈ¡×ÊÁÏ
	// 0 Ã»×ÊÁÏ or Buffer ¿Õ¼ä²»×ã
	// < 0 ¶ÏÏß
	long Recv(void);

	// È¡µÃÎ´´¦Àí°üµÄ´óÐ¡
	long GetReadLength(void);

	char ReadChar(void);
	unsigned char ReadUnsignedChar(void);

	short ReadShort(void);
	unsigned short ReadUnsignedShort(void);

	unsigned long ReadLength(void); //ÊÕÈ¡°üµÄ³¤¶È

	int ReadInt(void);
	unsigned int ReadUnsignedInt(void);

	int Read3CharToInt(void);

	long long ReadLongLong(void);
	unsigned long long ReadUnsignedLongLong(void);
	
	bool ReadBoolean(void);

	void * ReadData(unsigned long &CanReadLength);
	char * ReadString(void);

	// Çå³ý Send Buffer
	void ClearSendBuffer();

	// Send
	bool SendConnect(void);
	bool SendCommand(void);

	// È¡µÃÎ´´¦Àí°üµÄ´óÐ¡
	long GetSendLength(void);

	void WriteChar(char WriteData);
	void WriteUnsignedChar(unsigned char WriteData);

	void WriteShort(short WriteData);
	void WriteUnsignedShort(unsigned short WriteData);

	void WriteInt(int WriteData);
	void WriteUnsignedInt(unsigned int WriteData);

	void WriteLongLong(long long WriteData);
	void WriteUnsignedLongLong(unsigned long long WriteData);

	void WriteBoolean(bool WriteData);

	void WriteData(const void *pData, long DataLength);
	void WriteString(const char * pWriteData);

	//	跳转到读取数据流的固定位置
	void SkipToReadIndex(int index);

	void setM_recvMsg_Id(int id);
	void setM_sendMsg_Id(int id);

protected:

	#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

		SOCKET  m_Socket;

	#else

		int  m_Socket;

	#endif

	// Ö¸Ïò IP
	struct in_addr m_RemoteIP;

	// Recv Buffer
	// ¶þ±¶ »º´æ´óÐ¡
	char m_RecvBuffer[_Define_Socket_Buffer_Size_];
	long m_RecvIndex;
	long m_RecvReadIndex;

	// »º´æ Recv String
	char m_tempRecvString[_Define_Socket_Buffer_Size_];

	// Send Buffer
	char m_SendBuffer[_Define_Socket_Buffer_Size_];
	long m_SendIndex;

	bool SendPack(char * pPackBuffer, long PackLength);

	// ·µ»Ø½ÓÊÕ³¤¶È
	// > 0 ¶ÁÈ¡×ÊÁÏ
	// 0 Ã»×ÊÁÏ
	// < 0 ¶ÏÏß
	long RecvPack(char * pPackBuffer, long PackBufferLength);

private:

	// ·ÀÖ¹Ö±½ÓÒýÓÃ 
	SocketClient(const SocketClient&);
	SocketClient& operator = (const SocketClient&);

	int m_recvMsg_Id;
	int m_sendMsg_Id;

};


#endif
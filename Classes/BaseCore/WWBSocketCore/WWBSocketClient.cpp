#include "WWBSocketClient.h"
#include "BaseCore/WWBLog/LogUtil.h"
#include "BaseCore/WWBTools/StringTool.h"

using namespace std;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#else

#endif

static SocketClient* g_pSocketManager = NULL;


// ÔO¶¨ Socket Îª Ç¿ÆÈ¹Ø±Õ
void SetSocketLingerOFF(SOCKET SocketFD, bool IsNonBlock)
{
	// Ç¿ÖÆ¹Ø±Õ
	linger m_sLinger;

	m_sLinger.l_onoff = 1;
	m_sLinger.l_linger = 0;

	setsockopt(SocketFD, SOL_SOCKET, SO_LINGER, (const char *) &m_sLinger, sizeof(m_sLinger));

	// ·þÎñÆ÷ bind Éè¶¨
	int option = 1;
	setsockopt(SocketFD, SOL_SOCKET, SO_REUSEADDR, (char *) &option, sizeof(option));

	// No Delay
	setsockopt(SocketFD, IPPROTO_TCP, TCP_NODELAY, (char *) &option, sizeof(option));

	if(IsNonBlock)
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

		//ÉèÖÃÎª·Ç×èÈûÄ£Ê½
		unsigned long tempSocketMode = 1; 
		ioctlsocket(SocketFD, FIONBIO, &tempSocketMode); 

#else

		//ÉèÖÃÎª·Ç×èÈûÄ£Ê½
		int tempMode;  
		tempMode = fcntl(SocketFD, F_GETFL, 0);  
		fcntl(SocketFD, F_SETFL, tempMode | O_NONBLOCK);

#endif
	}
}

SocketClient::SocketClient()
{	
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	// Socket Init
	WSADATA wsaData;
	int ReturnValue = WSAStartup(MAKEWORD(2,2), &wsaData);

	this->m_Socket = INVALID_SOCKET;	

#else	

	this->m_Socket = -1;

#endif

	memset(&this->m_RemoteIP, 0, sizeof(this->m_RemoteIP));

	this->ClearRecvBuffer();
	this->ClearSendBuffer();
}

SocketClient::~SocketClient()
{	
	this->CloseSocket();	
}

SocketClient* SocketClient::create()
{
	if(!g_pSocketManager)
	{
		g_pSocketManager = new SocketClient();
	}
	return g_pSocketManager;
}

bool SocketClient::ConnectServer(const char * pServerIP, unsigned short ServerPort)
{
	this->CloseSocket();

	this->ClearRecvBuffer();
	this->ClearSendBuffer();

	struct hostent * ptempHost = gethostbyname(pServerIP);

	// È¡µÃÖ÷»úIPµØÖ·
	if(ptempHost == NULL)
	{
		return false;
	}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	SOCKET tempSocket = INVALID_SOCKET;

#else

	int tempSocket = -1;

#endif

	sockaddr_in tempSocketAddress;
	memset(&tempSocketAddress, 0, sizeof(tempSocketAddress));

	tempSocketAddress.sin_family = AF_INET;
	tempSocketAddress.sin_port = htons(ServerPort);
	tempSocketAddress.sin_addr = * ((struct in_addr *)ptempHost->h_addr);

	tempSocket = socket(AF_INET, SOCK_STREAM, 0);

	if((tempSocket == INVALID_SOCKET) || (tempSocket < 0))
	{
		return false;
	}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	if(tempSocket == INVALID_SOCKET)
	{
		return false;
	}

#else

	if(tempSocket < 0)
	{
		return false;
	}

#endif

	if(connect(tempSocket, (struct sockaddr *)&tempSocketAddress, sizeof(tempSocketAddress)) < 0)
	{
		return false;
	}

	// Éè¶¨ Socket ÊôÐÔ
	SetSocketLingerOFF(tempSocket, true);   

	this->m_Socket = tempSocket;

	// Copy IP
	memcpy(&this->m_RemoteIP, &tempSocketAddress.sin_addr, sizeof(this->m_RemoteIP));	

	return true;
}

void SocketClient::CloseSocket(void)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	if(this->m_Socket == INVALID_SOCKET)
	{
		return;
	}

	shutdown(this->m_Socket, SD_BOTH);
	closesocket(this->m_Socket);	
	this->m_Socket = INVALID_SOCKET;

#else

	if(this->m_Socket < 0)
	{
		return;
	}
	shutdown(this->m_Socket, 2);
	close(this->m_Socket);	
	this->m_Socket = -1;

#endif
}

bool SocketClient::IsConnect(void)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	if(this->m_Socket != INVALID_SOCKET)
	{
		return true;
	}
	else
	{
		return false;
	}

#else

	if(this->m_Socket >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}

#endif
}

bool SocketClient::SendPack(char * pPackBuffer, long PackLength)
{
	if(PackLength <= 0)
	{
		return false;
	}

	if(pPackBuffer == NULL)
	{
		return false;
	}

	if(!this->IsConnect())
	{
		return false;
	}

	long tempIndex = 0;
	while(PackLength > 0)
	{
		long SendBytes = send(this->m_Socket, (const char *)(pPackBuffer + tempIndex), PackLength, 0);

		if(SendBytes == 0)
		{
			// êPé] socket
			this->CloseSocket();

			return false;
		}

		if(SendBytes < 0)
		{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			int tempError = WSAGetLastError();

			if(tempError == WSAEWOULDBLOCK)
			{
				// ÊÍ·Å ½ø³Ì
				Sleep(0);
				continue;
			}

#else
			// ÐèÒª ÖØÊÔ×´¿ö
			if((errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == EINTR))
			{
				// ÊÍ·Å ½ø³Ì
				sched_yield();
				continue;
			}

#endif

			// êPé] socket
			this->CloseSocket();

			return false;
		}

		// Ö¸ÏòÎ´·¢ËÍ×ÊÁÏ
		tempIndex += SendBytes;
		PackLength -= SendBytes;
	}
	return true;
}

// ·µ»Ø½ÓÊÕ³¤¶È
// > 0 ¶ÁÈ¡×ÊÁÏ
// 0 Ã»×ÊÁÏ
// < 0 ¶ÏÏß
long SocketClient::RecvPack(char * pPackBuffer, long PackBufferLength)
{
	if(PackBufferLength <= 0)
	{
		return -1;
	}

	if(pPackBuffer == NULL)
	{
		return -1;
	}	

	if(!this->IsConnect())
	{
		return -1;
	}

	long tempRecvLength = recv(this->m_Socket, pPackBuffer, PackBufferLength, 0);

	if(tempRecvLength == 0)
	{
		// êPé] socket
		this->CloseSocket();
		return -1;
	}

	// åeÕ` ÌŽÀí
	if(tempRecvLength < 0) 
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

		int tempError = WSAGetLastError();
		if(tempError == WSAEWOULDBLOCK)
		{
			return 0;
		}

#else

		// ÓÉÓÚÊÇ·Ç×èÈûµÄÄ£Ê½,ËùÒÔµ±errnoÎªEAGAINÊ±,±íÊ¾µ±Ç°»º³åÇøÒÑÎÞÊý¾Ý¿É¶Á
		// ÔÚÕâÀï¾Íµ±×÷ÊÇ¸Ã´ÎÊÂ¼þÒÑ´¦Àí´¦.
		// ¼´µ±buflen < 0 ÇÒ errno == EAGAINÊ±£¬±íÊ¾Ã»ÓÐÊý¾ÝÁË¡£(¶Á/Ð´¶¼ÊÇÕâÑù)
		if(errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
		{
			return 0;
		}

#endif

		// êPé] socket
		this->CloseSocket();

		return -1;
	}

	return tempRecvLength;
}

// Çå³ý Recv Buffer
void SocketClient::ClearRecvBuffer(void)
{
	this->m_RecvIndex = 0;
	this->m_RecvReadIndex = 0;
}

// Recv
// ·µ»Ø½ÓÊÕ³¤¶È
// > 0 ¶ÁÈ¡×ÊÁÏ
// 0 Ã»×ÊÁÏ or Buffer ¿Õ¼ä²»×ã
// < 0 ¶ÏÏß
long SocketClient::Recv(void)
{
	this->ClearRecvBuffer();

	// ´ÓÎ´´¦Àí Buffer Íùºó½ÓÊÕ
	this->m_RecvIndex = this->RecvPack(this->m_RecvBuffer , _Define_Socket_Buffer_Size_);

	// //解析接收数据
	if(m_RecvIndex > 0 && m_RecvIndex > 8)
	{
		std::string str = StringTool::charArrayToString(m_RecvBuffer,m_RecvIndex);
		LogUtil::getInstance()->writeLog(LEVEL_TYPE::LOG_NET,"----recv msg----","%s",str.c_str());
	}

	return this->m_RecvIndex;
}

// È¡µÃÎ´´¦Àí°üµÄ´óÐ¡
long SocketClient::GetReadLength(void)
{
	// Ã»×ÊÁÏÅÐ¶Ï
	if (this->m_RecvIndex <= 0)
	{
		return 0;
	}

	// ·À´ô
	if (this->m_RecvIndex <= this->m_RecvReadIndex)
	{
		return 0;
	}

	return this->m_RecvIndex - this->m_RecvReadIndex;

}

char SocketClient::ReadChar(void)
{
	char tempReturn = *((char *)(this->m_RecvBuffer + this->m_RecvReadIndex));
	this->m_RecvReadIndex += sizeof(char);

	return tempReturn;
}
unsigned char SocketClient::ReadUnsignedChar(void)
{
	unsigned char tempReturn = *((unsigned char *)(this->m_RecvBuffer + this->m_RecvReadIndex));
	this->m_RecvReadIndex += sizeof(unsigned char);

	return tempReturn;
}

short SocketClient::ReadShort(void)
{
	short * ptempInt = (short *)(this->m_RecvBuffer + this->m_RecvReadIndex);;

	this->m_RecvReadIndex += sizeof(short);

	return * ptempInt;
}
unsigned short SocketClient::ReadUnsignedShort(void)
{
	unsigned short tempReturn = *((unsigned short *)(this->m_RecvBuffer + this->m_RecvReadIndex));
	this->m_RecvReadIndex += sizeof(unsigned short);

	return tempReturn;
}

unsigned long SocketClient::ReadLength(void)
{
	unsigned long tempLength = this->ReadUnsignedShort();

	if(tempLength == 0xFFFF)
	{
		tempLength = this->ReadUnsignedInt();
	}

	return tempLength;
}

int SocketClient::ReadInt(void)
{
	int * ptempInt = (int *)(this->m_RecvBuffer + this->m_RecvReadIndex);

	this->m_RecvReadIndex += sizeof(int);

	return * ptempInt;
}
unsigned int SocketClient::ReadUnsignedInt(void)
{
	unsigned int * ptempInt = (unsigned int *)(this->m_RecvBuffer + this->m_RecvReadIndex);

	this->m_RecvReadIndex += sizeof(unsigned int);

	return * ptempInt;
}

int SocketClient::Read3CharToInt(void)
{
	char tempReturn1 = *((char *)(this->m_RecvBuffer + this->m_RecvReadIndex));
	this->m_RecvReadIndex += sizeof(char);

	char tempReturn2 = *((char *)(this->m_RecvBuffer + this->m_RecvReadIndex));
	this->m_RecvReadIndex += sizeof(char);

	char tempReturn3 = *((char *)(this->m_RecvBuffer + this->m_RecvReadIndex));
	this->m_RecvReadIndex += sizeof(char);

	int tempReturn = (tempReturn1 << 16) + (tempReturn2 << 8) + tempReturn3;

	return tempReturn;
}

long long SocketClient::ReadLongLong(void)
{
	long long * ptempInt = (long long *)(this->m_RecvBuffer + this->m_RecvReadIndex);

	this->m_RecvReadIndex += sizeof(long long);

	return * ptempInt;
}
unsigned long long SocketClient::ReadUnsignedLongLong(void)
{
	unsigned long long * ptempInt = (unsigned long long *)(this->m_RecvBuffer + this->m_RecvReadIndex);

	this->m_RecvReadIndex += sizeof(unsigned long long);

	return * ptempInt;
}

bool SocketClient::ReadBoolean(void)
{
	unsigned char tempBoolean = this->ReadUnsignedChar();
	if (tempBoolean == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void * SocketClient::ReadData(unsigned long &CanReadLength)
{
	unsigned short tempLength = this->ReadUnsignedShort();

	if (tempLength == 0)
	{
		CanReadLength = 0;

		return NULL;
	}
	else if (tempLength >= 0xFFFF)
	{
		CanReadLength = this->ReadUnsignedInt();
		void * ptempData = this->m_RecvBuffer + this->m_RecvReadIndex;

		this->m_RecvReadIndex += CanReadLength;

		return ptempData;
	}
	else
	{
		CanReadLength = tempLength;
		void * ptempData = this->m_RecvBuffer + this->m_RecvReadIndex;

		this->m_RecvReadIndex += CanReadLength;

		return ptempData;
	}
}

char * SocketClient::ReadString(void)
{
	unsigned long tempStringLength = 0;
	void * pString = this->ReadData(tempStringLength);

	if ((pString == NULL) || (tempStringLength == 0))
	{
		//return NULL;
		return ""; //TODO
	}

	memcpy(this->m_tempRecvString, pString, tempStringLength);
	this->m_tempRecvString[tempStringLength] = 0;

	return this->m_tempRecvString;
}

// Çå³ý Send Buffer
void SocketClient::ClearSendBuffer()
{
	this->m_SendIndex = 4;
}

// Send
bool SocketClient::SendConnect(void)
{
	if(this->m_SendIndex <= 4)
	{
		return false;
	}

	unsigned short tempPackLength = (unsigned short)(this->m_SendIndex - 4);

	memcpy(this->m_SendBuffer + 2, &tempPackLength, 2);

	LogUtil::getInstance()->writeLog("send length=%d", this->m_SendIndex - 2);

	std::string result = StringTool::charArrayToString(m_SendBuffer, m_SendIndex);
	LogUtil::getInstance()->writeLog(LEVEL_TYPE::LOG_NET,"----send msg----","%s",result.c_str());

	return this->SendPack(this->m_SendBuffer + 2, this->m_SendIndex - 2);
}

// Send
bool SocketClient::SendCommand(void)
{
	if(this->m_SendIndex <= 4)
	{
		return false;
	}

	unsigned short tempPackLength = (unsigned short)(this->m_SendIndex - 4);
	memcpy(this->m_SendBuffer, &tempPackLength, 2);

	unsigned short channelID = (this-> m_recvMsg_Id << 8 + m_sendMsg_Id);

	memcpy(this->m_SendBuffer + 2, &channelID, 2);

	LogUtil::getInstance()->writeLog("send length=%d", this->GetSendLength());

	//输出通信日志
	string result = StringTool::charArrayToString(m_SendBuffer, m_SendIndex);
	if(this->GetSendLength() > 7)
	{
		LogUtil::getInstance()->writeLog(LEVEL_TYPE::LOG_NET,"----send msg----","%s",result.c_str());
	}

	return this->SendPack(this->m_SendBuffer, this->m_SendIndex);

}

// È¡µÃÎ´´¦Àí°üµÄ´óÐ¡
long SocketClient::GetSendLength(void)
{
	if(this->m_SendIndex <= 4)
	{
		return 0;
	}
	return this->m_SendIndex - 4;
}

void SocketClient::WriteChar(char WriteData)
{
	*((char *)(this->m_SendBuffer + this->m_SendIndex)) = WriteData;
	this->m_SendIndex += sizeof(char);
}
void SocketClient::WriteUnsignedChar(unsigned char WriteData)
{
	*((unsigned char *)(this->m_SendBuffer + this->m_SendIndex)) = WriteData;
	this->m_SendIndex += sizeof(unsigned char);
}

void SocketClient::WriteShort(short WriteData)
{
	char tempWriteBuffer[2];
	memcpy(this->m_SendBuffer + this->m_SendIndex, &WriteData, 2);

	this->m_SendIndex += sizeof(short);
}
void SocketClient::WriteUnsignedShort(unsigned short WriteData)
{
	char tempWriteBuffer[2];

	memcpy(this->m_SendBuffer + this->m_SendIndex, &WriteData, 2);

	this->m_SendIndex += sizeof(unsigned short);
}

void SocketClient::WriteInt(int WriteData)
{
	char tempWriteBuffer[4];
	memcpy(this->m_SendBuffer + this->m_SendIndex, &WriteData, 4);

	this->m_SendIndex += sizeof(int);
}
void SocketClient::WriteUnsignedInt(unsigned int WriteData)
{
	char tempWriteBuffer[4];
	memcpy(this->m_SendBuffer + this->m_SendIndex, &WriteData, 4);

	this->m_SendIndex += sizeof(unsigned int);
}

void SocketClient::WriteLongLong(long long WriteData)
{
	char tempWriteBuffer[8];
	memcpy(this->m_SendBuffer + this->m_SendIndex, &WriteData, 8);

	this->m_SendIndex += sizeof(long long);
}
void SocketClient::WriteUnsignedLongLong(unsigned long long WriteData)
{
	char tempWriteBuffer[8];
	memcpy(this->m_SendBuffer + this->m_SendIndex, &WriteData, 8);

	this->m_SendIndex += sizeof(unsigned long long);
}

void SocketClient::WriteBoolean(bool WriteData)
{
	if (WriteData)
	{
		this->WriteUnsignedChar(1);
	}
	else
	{
		this->WriteUnsignedChar(0);
	}
}

void SocketClient::WriteData(const void *pData, long DataLength)
{
	if ((pData == NULL) || (DataLength == 0))
	{
		this->WriteUnsignedShort(0);
	}
	else
	{
		if (DataLength >= 0xFFFF)
		{
			this->WriteUnsignedShort(0xFFFF);
			this->WriteUnsignedInt((int)DataLength);
		}
		else
		{
			this->WriteUnsignedShort((unsigned short)DataLength);
		}

		memcpy(this->m_SendBuffer + this->m_SendIndex, pData, DataLength);
		this->m_SendIndex += DataLength;
	}
}

void SocketClient::WriteString(const char * pWriteData)
{
	if (pWriteData == NULL)
	{
		this->WriteUnsignedShort(0);
	}
	else
	{
		size_t tempStringLength = strlen(pWriteData);

		this->WriteData(pWriteData, tempStringLength);
	}
}

void SocketClient::SkipToReadIndex(int index)
{
	this->m_RecvReadIndex = index;
}

void SocketClient::setM_recvMsg_Id(int id)
{
	this->m_recvMsg_Id = id;
}

void SocketClient::setM_sendMsg_Id(int id)
{
	this->m_sendMsg_Id = id;
}

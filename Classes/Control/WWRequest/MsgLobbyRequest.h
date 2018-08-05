/*
 * MsgLogonRequest.h
 *	功能描述：
 *  Created on: 2014年6月24日 上午9:50:47
 *      Author: hp
 */

#ifndef _MODEL_WWFILTER_MSGLOBBYREQUEST_H_
#define _MODEL_WWFILTER_MSGLOBBYREQUEST_H_
#include "cocos2d.h"

/*
 *大厅相关消息请求
 */
class MsgLobbyRequest
{
public:
	MsgLobbyRequest();
	virtual ~MsgLobbyRequest();
	static void requestRoomDatas(int gameId, int playType);//请求获取单个玩法的房间数据

};

#endif /* _MODEL_WWFILTER_MSGLOBBYREQUEST_H_ */

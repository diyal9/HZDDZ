/*
 * MsgGameCommonRequest.h
 *	功能描述：
 *  Created on: 2014年7月3日 上午10:21:54
 *      Author: hp
 */

#ifndef _CONTROL_WWREQUEST_MSGGAMECOMMONREQUEST_H_
#define _CONTROL_WWREQUEST_MSGGAMECOMMONREQUEST_H_

#include "cocos2d.h"
#include "Model/WWDataModel/WWGD/GD.h"

/*
 *游戏中消息请求（适用于所有游戏）
 */
class MsgGameCommonRequest
{
public:
	MsgGameCommonRequest();

	virtual ~MsgGameCommonRequest();

	static void enterGame(GameRoom* room,std::string &data);//进入游戏

	static void sendLeaveGame();//离开游戏

	static void sendSubstituteRequest(int type);//请求托管//(int1)0 托管、1 取消托管

	static void sendChatRequest(std::string &nickName, int speakStyle, int receiverID, int waFont, std::string &content);//聊天消息请求

	static void sendPropRequest(int propId, int price, int count, int userId);//道具使用（表情）

	static void sendContinueGame();//续局

	static void sendQuickStartGame();//快速开始：点了"续"15秒后，还没开局，发送允许速配，请求开局

	//static void sendEnterGameZone();//30分钟未进行过游戏对局， 就需要先发送该消息
};

#endif /* _CONTROL_WWREQUEST_MSGGAMECOMMONREQUEST_H_ */

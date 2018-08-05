/*
 * MsgLogonDataFilter.h
 *	功能描述： 大厅数据解析
 *  Created on: 2015年5月8日 下午11:09:51
 *      Author: diyal.yin
 */

#ifndef _MODEL_WWFILTER_MSGLOBBYREQUESTFILTER_H_
#define _MODEL_WWFILTER_MSGLOBBYREQUESTFILTER_H_
#include "cocos2d.h"
#include "BaseCore/WWNetDataBasic/WWIMsgFilter.h"

/**房间列表信息*/
//const int MSG_GAME_ZOON_LIST = 0x060124;

const int MSG_ENTER_HALL_GEN = 0x010101;

/********************************通用消息返回************************************************/

/*
 *大厅消息返回解析
 */
class MsgLobbyRequestFilter: public WWIMsgFilter
{
public:
	MsgLobbyRequestFilter();
	virtual ~MsgLobbyRequestFilter();

	virtual void onDataReceiver(int msgType,cocos2d::Ref* objectData ){};
	virtual std::vector<int> onFilters();
	virtual bool onMessageReceived(int msgType);
	virtual std::vector<int> onRootFilters();
	virtual bool onRootMessageReceived(int msgType, int userId, int result,std::string reason, int reasonType);
	cocos2d::Ref* parseRoomListData();//登陆返回
};

#endif /* _MODEL_WWFILTER_MSGLOBBYREQUESTFILTER_H_ */

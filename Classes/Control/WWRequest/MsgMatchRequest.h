/*
 * MsgGameRequest.h
 *	功能描述：
 *  Created on: 2015年7月2日 上午17:35:04
 *      Author: zx
 */

#ifndef _MSG_MATCH_REQUEST_H_
#define _MSG_MATCH_REQUEST_H_

#include "cocos2d.h"

class MsgMatchRequest
{
public:
	MsgMatchRequest();
	virtual ~MsgMatchRequest();

	//请求即时比赛列表
	static void sendIMatchListReq(int matchType, int type);

	//请求即时比赛详情
	static void sendIMatchInfoReq(int matchID, int type, int objectID, int userID);

	//报名|退赛
	static void sendIMatchSignup(int matchID, int action, const char* nickName, int vipGrade, int param);

public:
	//报名人数变更通知
	static void parseIMatchSignupNotify();

	//玩家当前比赛状态信息
	static void parseImatchPlayerStatus();

	//比赛游戏对局结束
	static void parseIMatchGameOver();

	//比赛轮空通知
	static void parseIMatchBye();

	//比赛游戏开局
	static void parseIMatchAntilordStart();

	//比赛游戏恢复现场
	static void parseImatchAntilordResume();

	//2.7.28.玩家比赛成绩(MatchPerformance)

	static void parseMatchPerformance();

};

#endif // !_MSG_MATCH_REQUEST_H_


/*
 * MsgGameRequest.h
 *	功能描述：
 *  Created on: 2014年6月27日 上午10:01:04
 *      Author: hp
 */

#ifndef _CONTROL_WWREQUEST_MSGLORDGAMEREQUEST_H_
#define _CONTROL_WWREQUEST_MSGLORDGAMEREQUEST_H_
#include "cocos2d.h"
/*
 *游戏中消息请求（斗地主）
 */
class MsgLordGameRequest
{
public:
	MsgLordGameRequest();
	virtual ~MsgLordGameRequest();
	static void sendSelfCallLord(bool isJiao);//叫地主
	static void sendSelfGrabLord(bool isGrab);//抢地主
	static void sendSelfMingPai(bool isMingPai, char* cards);//明牌 isMingPai 是否明牌  cards isMingPai为true时cards为自己的牌
	static void sendSelfNoMingPai();//不明牌
	static void sendTurnOutPoker(char* pokers, char* replacePokers, int type,int max);//癞子出牌
	static void sendTurnOutPoker(char* pokers);//欢乐出牌   不出  pokers直接发0

	static void sendJiaBei(int type);//(int1)  0:不加倍  1:加倍  2：加强加倍  3：加倍结束

};

#endif /* _CONTROL_WWREQUEST_MSGLORDGAMEREQUEST_H_ */

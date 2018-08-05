//斗地主开局数据

#ifndef _NETRECVEXECUTE_H_
#define _NETRECVEXECUTE_H_

#include "cocos2d.h"

class NetRecvExecute
{
public:
	
protected:
	NetRecvExecute();
	virtual ~NetRecvExecute();
	void dealNetRecv();//网路接收处理

	virtual void onMsgRecv(int type){};//普通消息处理

	void executeGlobleMsg(int type);
	
	virtual void onRootMsgRecv(int msgType, int userId, int result, std::string reason, int reasonType){};//通用消息处理回调

	void executeRootMsg();//通用消息处理

	bool executeGlobleRootMsg(int msgType, int userId, int result, std::string reason, int reasonType);//处理全局通用消息

private:
	void parseResumeGame();//恢复对局
	void parseRecoveryGameNofity();//恢复对局时的房间信息

	void parsePlayBoxList();//道具宝箱列表
	void parseOpenMoneyBoxResp();//开奖励宝箱结果

	/************************************************************************************/
	/*比赛 消息解析函数																	*/
	/************************************************************************************/

	//报名人数变更通知
	void parseIMatchSignupNotify();

	//玩家当前比赛状态信息
	void parseImatchPlayerStatus();

	//比赛游戏对局结束
	void parseIMatchGameOver();

	//比赛轮空通知
	void parseIMatchBye();

	//比赛游戏开局
	void parseIMatchAntilordStart();

	//比赛游戏恢复现场
	void parseImatchAntilordResume();
};

#endif /* _NETRECVEXECUTE_H_ */

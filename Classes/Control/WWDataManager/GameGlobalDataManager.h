/*
 * GameGlobalDataManager.h
 *	功能描述：单例，存储游戏全局公共数据中心
 *  Created on: 2015年6月17日 下午2:09:49
 *      Author: Jacky
 */

#ifndef _CONTROL_WWDATAMANAGER_GAMEGLOBALDATA_MANAGER_H_
#define _CONTROL_WWDATAMANAGER_GAMEGLOBALDATA_MANAGER_H_

#include "cocos2d.h"
#include "WWMacros.h"


/*
 *开局方式
 */
enum StartGameType{
	/*
	 *欢乐，癞子玩法
	 */
	SEND_NOMAL = 1,//发送开局消息
	SEND_RESUME,//发送恢复对局消息
	NO_SEND,//服务端主动推送恢复对局的消息

	/*
	 *比赛玩法
	 */
	MATCH_START_FROM_LOBBY,//比赛开局消息，在大厅接收
	MATCH_START_IN_GAME//比赛开局消息，在游戏中接收
};

/*
 *进入登录界面方式
 */
enum LoginType{
	LOGIN_FROM_ICON=0,//从icon启动
	LOGIN_FROM_LOBBY//从大厅返回登陆界面
};

/*
 * 当前所处游戏页面类型
 */
enum GameSceneType{
	LOGIN_SCENE = 1,//登录页面
	LOBBY_SCENE,	//大厅页面
	ROOM_SCENE,		//房间页面(游戏中)
	LOADING_SCENE	//加载动画页面
};


/*
 *游戏全局公共数据中心
 */
class GameGlobalDataManager
{
public:
	GameGlobalDataManager();

	virtual ~GameGlobalDataManager();

	SINGLE_INSTANCE_FUNC(GameGlobalDataManager);

public:

	void setStartGameType(int type);
	int  getStartGameType();

	void setLoginFromType(int type);
	int  getLoginFromType();

	void setCurSceneType(int type);
	int  getCurSceneType();
	
	void setCurGameId(int gameId);
	int  getCurGameId();
	
	void setCurPlayId(int playId);
	int  getCurPlayId();

	void setPlayRoomId(int roomId);
	int getPlayRoomId();

	//清除当前局游戏数据
	void clearCurGameData();

private:

	//开局方式    正常开局  or  恢复对局
	int m_StartGameType;

	//进入登录界面的方式
	int m_LoginFromType;

	//当前场景
	int m_CurSceneType;

	//当前(int4)游戏ＩＤ
	int m_CurGameId;

	//当前(int4)对局ID
	int m_CurPlayId;

	//当前打牌房间ID， 可能是欢乐，癞子，比赛，约战房等
	int m_PlayRoomId;

};

#endif /* _CONTROL_WWDATAMANAGER_GAMEGLOBALDATA_MANAGER_H_ */

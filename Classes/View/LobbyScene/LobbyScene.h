#ifndef _LobbyScene_LobbyScene_H_
#define _LobbyScene_LobbyScene_H_

/*
 * WWSceneManager.h
 *
 *	大厅场景

 *  Created on: 2015-5-5
 *      Author: Jacky
 */

#include <iostream>
#include "cocos2d.h"
#include "BaseCore/Common/GameConfig.h"
#include "BaseCore/Common/FactoryTools.h"
#include "Control/WWRequest/MsgLobbyRequest.h"
#include "View/LobbyScene/Layer/MainViewLayer.h"
#include "View/LobbyScene/Layer/RoomListLayer.h"
#include "Control/WWWorkManager/NetRecvExecute.h"

USING_NS_CC;

using namespace std;

enum 
{
	BTN_LOBBYLAYER_HAPPY_TAG = 1,
	BTN_LOBBYLAYER_RASCAL_TAG,
	BTN_LOBBYLAYER_MATCH_TAG,
	BTN_LOBBYLAYER_CRAZY_TAG,
	BTN_LOBBYLAYER_NOTICE_TAG,
	BTN_LOBBYLAYER_FIRST_BUY_TAG,
	BTN_LOBBYLAYER_TOPBAR_MENU_TAG, //展开功能按钮状态按钮
	BTN_LOBBYLAYER_TOPTITBG_TAG, //大背景图精灵Tag
	BTN_LOBBYLAYER_EMAIL_TAG,
	BTN_LOBBYLAYER_TESK_TAG,
	BTN_LOBBYLAYER_BUY_TAG,
	BTN_LOBBYLAYER_SHOP_TAG,
	BTN_LOBBYLAYER_ACTIVITY_TAG,
	BTN_LOBBYLAYER_QUICK_START_TAG,
	BTN_LOBBYLAYER_HEAD_TAG,
	BTN_LOBBYLAYER_COINADD_TAG,
	BTN_LOBBYLAYER_DIAMADD_TAG,
	BTN_LOBBYLAYER_FUNCTAG_BAG, //背包
	BTN_LOBBYLAYER_FUNCTAG_ACHI, //成就
	BTN_LOBBYLAYER_FUNCTAG_DIALY, //签到
	BTN_LOBBYLAYER_FUNCTAG_RANK, //排行榜
	BTN_LOBBYLAYER_FUNCTAG_SET, //设置
	BTN_EXIT_PRO,//退出
}; 

enum  //主界面上弹出按钮Tag 
{
	FRAMEPOP_TAG_PROPSHOP = 1001, //道具商城
}; 

enum
{
	UI_EXIT_TAG = 100,//退出弹出窗
};

class MainViewLayer;

class LobbyScene : public Scene , NetRecvExecute
{
public:
	LobbyScene();
	virtual ~LobbyScene();
    
	virtual bool init();

	CREATE_FUNC(LobbyScene);

	virtual void onEnter();
	
	virtual void onExit();
	
	//void ButtonDepress(Ref * theBtn);  // menu按钮事件管理

	//void onTouchSpriteDepress(Ref * theBtn);  // touchSprite按纽响应

	void exitGame(Ref* pSendser);//退出

	void onLayBtnPressed(); //子图层按纽响应

	//打开模态弹出框
	void openModelFrame();
    void closeModelFrame(); 
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

	void unitTest(float dt); //单元测试
	void autoTestEnterGame(float dt);//自动测试进入牌局
	//测试代码
	void setTest(float dt);
	void chargeTest(float dt);
	void infoTest(float dt);

	virtual void update(float dt);

private:
	CC_SYNTHESIZE(int, m_pUITag, UITag);//弹窗tag
	CC_SYNTHESIZE(MainViewLayer*, m_pMainView, pMainView);

	void onMsgRecv(int msgType);//普通消息处理

	void onRootMsgRecv(int msgType, int userId, int result, std::string reason, int reasonType);//通用消息处理

	void parseRoomListData();//解析房间数据

	//2.9.2.游戏商店商品列表 StoreMagicList  / 可出售道具列表
	void parseStoreMagicList(int msgId);

	//2.10.2.购买道具回复
	void parseBuyMagicResp();

	//返回即时比赛列表
	void parseIMatchListResp();

	//返回即时比赛详情
	void parseIMatchInfoResp();

	//2.7.33.报名|退赛回执 (IMatchSignupResp)
	void parseIMatchSignupResp();

	//返回好友列表
	void parseBuddyList();
private:
	int recvMsgCount;//接收数据的条数， 用来控制界面显示； 情况：在收到相应的所有的房间数据后，才能够让用户进行界面操作
};

#endif
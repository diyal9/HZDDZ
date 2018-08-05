#ifndef _LOGINSCENE_LOGINSCENE_H_
#define _LOGINSCENE_LOGINSCENE_H_

/*
 * WWSceneManager.h
 *
 *	登录场景

 *  Created on: 2015-5-5
 *      Author: Jacky
 */

#include "cocos2d.h"
#include <iostream>
#include "../../BaseCore/WWBTools/AccountUtils.h"
#include "../../Control/WWWorkManager/NetRecvExecute.h"

USING_NS_CC;

using namespace std;

enum 
{
	LOGIN_BTN_NODE_TAG = 1,
	BTN_LOGINLAYER_LOGIN_TAG,
	BTN_LOGINLAYER_GOBACK_TAG,
	BTN_LOGINLAYER_QUICKSTART_TAG,
	BTN_LOGINLAYER_CHANGEAC_TAG,
	BTN_LOGINLAYER_GAMEINFO_TAG,
	LOGIN_INPUT_NODE_TAG,
	LOGINLAYER_NAME_TEXT_TAG,
	LOGINLAYER_PASSWORD_TEXT_TAG, 
	LOGINLAYER_INPUT_CURSOR_TAG, 
	TAG_LOGIN_ANIM//登录动画
}; 

enum
{
	TYPE_LOGIN_FAST = 1,//快速开始
	TYPE_LOGIN_ACCOUNT,//账号登录
	TYPE_LOGIN_TELEPHONE//手机登录
};

class LoginScene : public Scene,NetRecvExecute
{
public:
	LoginScene();
	virtual ~LoginScene();
    
	virtual bool init();

	CREATE_FUNC(LoginScene);

	virtual void onEnter();
	
	virtual void onExit();
	//创建登入输入
	void CreateLoginInput();
	//创建登入等按钮
	void CreateLoginBtn();
	//按钮事件管理
	void onTouchSpriteDepress(Ref * theBtn);
	//登入中动画
	void startAction();

	void sendLogin();

	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent); 

	void unitTest(float dt); //单元测试

	virtual void update(float dt);
	
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

private:

	void initTelephoneDialog();

	void sendLogonByTelephone(const char* telephone, const char* checkCode);//手机登陆

	void sendlogonByFastEnter();//快速登陆

	void sendLogonByAcount(const char* userId, const char* userPwd);//账号登陆

	void logonFail(cocos2d::Ref* data);//登陆失败

	void onMsgRecv(int msgType);//普通消息处理

	virtual void onRootMsgRecv(int msgType, int userId, int result, std::string reason, int reasonType);//通用消息处理

	/**************************************************消息处理方法**start*********************************************************************/
	void parseProfile();//解析登陆成功
	/**************************************************消息处理方法***end**********************************************************************/
	//cocos2d::Map<int, cocos2d::Vector<GameRoom*>> allRooms;
};

#endif
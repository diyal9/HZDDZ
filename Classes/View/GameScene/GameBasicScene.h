/*
 * GameScene.h
 *
 * 设计思路:
 * GameScene是一个Scene，他包含资源管理器、逻辑管理、网络管理
 * UI这块的设计思路是、分为如下层叠
 * 1、UI背景层 （包含基本的UI展示、）
 * 2、顶部菜单栏
 * 3、玩家交互层（玩法操作按钮、牌布局、）
 * 4、全局UI层、也就是呼出层
 *
 *  Created on: 2015-5-11
 *      Author: diyal.yin
 */

#ifndef _VIEW_GAMESCENE_GAMESCENE_H_
#define _VIEW_GAMESCENE_GAMESCENE_H_

#include "GameSceneDefine.h"
#include "cocos2d.h"
#include "Model/WWDataModel/WWGD/GD.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"
#include "View/GameScene/Layer/GameSetMenu.h"
#include "View/Common/CardSprite/CardSprite.h"
#include "BaseCore/Common/FactoryTools.h"
#include "Control/WWWorkManager/NetRecvExecute.h"

USING_NS_CC;

//各层Tag
enum gameSceneTag
{
	TAGGAMEBACKGROUND = 1,
	TAGTABLELAYER,
	TAGCARDSLAYER,
	TAGGAMEUILAYER,
	TAGGAMEENDLAYER,
	TAGCHATLAYER,
	BIN_GAMEPLAYERLAYER_CHOOSE_RESCAL_TAG //多个癞子选择框
};


class GameBasicScene : public Scene, NetRecvExecute
{
public:


	GameBasicScene(void);
	virtual ~GameBasicScene(void);

	CREATE_FUNC(GameBasicScene);
    
	bool virtual init();

	void virtual onEnter();

	void virtual onExit();

	void showWaitAnimation(); //开启洗牌动画
	void closeWaitAnimation(); //关闭洗牌动画

	//房间聊天
	void showChatMenuBtn();
	void hideChatMenuBtn();

	//virtual void keyBackClicked();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	//实现网络回调的虚函数
	//virtual void onDataReceiver(int msgType, cocos2d::Ref* objectData );

	void startGameOverTime(); //开始自动续局倒计时
	void endGameOverTime();	  //结束自动续局倒计时
	void updataGameOverTime(float dt);

	void startContinueTimer(); //开始结局 15 秒倒计时
	void endContinueTimer();
	void updateContinueTimer(float dt);

	CC_SYNTHESIZE(int, m_GameOverTime, GameOverTime); //结算自动续局倒计时
	
	void setSubstituteEnable(bool isEnable);
	
	//弹出选择癞子牌型的窗口，当手中的癞子牌的组合方式有多种可以打出时
	void ChooseLaiziCardsToPlay(const std::vector<cocos2d::Vector<PokerObject*>> &vecAllPokersVec);
	//移除癞子牌型选择的窗口
	void removeChooseLaiziTable();

	//自己出牌
	void showOutCard(const cocos2d::Vector<CardSprite*> &outCards, int outCardType, bool isAutoPlay, bool isLanlord, cocos2d::CallFunc *func);
	
	//其它玩家出牌
	void showOutCard(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func);
	
	//牌局结束显示其它玩家剩余手牌
	void showLeftCard(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func);
	
	//第二渲染树根场景
	Scene*    m_pSence;
	
	//打开全局UI
	void openGlobalUI(Node* pNode, int tag = 0);

	//关闭全局UI
	void closeGlobalUI(Node* pNode);

	void closeGlobalUI(int tag);
	
	//关闭所有弹窗
	void closeAllUI();

	virtual void update(float dt);

private:
	cocos2d::Vector<GameRoom *> rooms;
	
	bool isWaitAnimRun;
	CC_SYNTHESIZE(GameSetMenu*, m_pSetMenu, pSetMenu);
	
	Sprite* m_chooseRescalTx;//癞子选择提示文字

	void onMsgRecv(int msgType);//普通消息处理

	void onRootMsgRecv(int msgType, int userId, int result, std::string reason, int reasonType);//通用消息处理

	/************************************************************************/
	/* 消息解析函数                                                         */
	/************************************************************************/
	cocos2d::Ref* parseStartGame();//开局消息

	cocos2d::Ref* parseCallLord();//叫地主

	cocos2d::Ref* parseGrabLord();//抢地主

	cocos2d::Ref* parsePlayCard();//打牌

	cocos2d::Ref* parseMingPai();//明牌

	cocos2d::Ref* parseJiaBei();//加倍

	cocos2d::Ref* parseGameOver();//结算

	cocos2d::Ref* parseCommonGameOver();//通用結算消息解析

	cocos2d::Ref* parseGameSubstitute();//游戏托管

	cocos2d::Ref* parseGameChat();//游戏中聊天

	cocos2d::Ref* parseGameUserInfo();//游戏玩家资料

	cocos2d::Ref* parsePropUseToInfo();//道具使用

};

#endif /*_VIEW_GAMESCENE_GAMESCENE_H_*/

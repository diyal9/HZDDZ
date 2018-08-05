/*
 * GameLogicManager.h
 *
 *  游戏逻辑管理类
 *  理论上，游戏中数据导向、UI相关都由该对象控制。
 *  Created on: 2015-5-18
 *      Author: diyal.yin
 *  
 *  1、逻辑控制。理论上页面所有UI、数据都由改对象管理
 *  2、网络请求。游戏中所有的数据由该对象处理
 *  3、该对象配合GameBasicScene使用。
 *  建议：该对象里面只提供各个节点的调用，具体实现放到对象当中
 */


#ifndef _VIEW_GAMESCENE_GAMELOGICMANAGER_H_
#define _VIEW_GAMESCENE_GAMELOGICMANAGER_H_

#include "cocos2d.h"
#include "WWMacros.h"
#include "View/GameScene/GameBasicScene.h"
#include "view/GameScene/Layer/GameTopMenuLayer.h"
#include "view/GameScene/Layer/GamePlayerLayer.h"
#include "view/GameScene/Layer/MyCardsLayer.h"
#include "BaseCore/Common/FactoryTools.h"
#include "Control/WWDataManager/WWDataManager.h"
#include "View/GameScene/GameOver/GameOverLayer.h"

USING_NS_CC;

enum 
{
	SCENR_GLOBEL_SET_TAG = 150, 
	SCENR_GLOBEL_EXIT_TAG

};

class GameLogicManager:public Ref
{
public:
	GameLogicManager();
	virtual ~GameLogicManager();
	static GameLogicManager* getInstance();

	/************************************************************************/
	/* 请求类成员函数                                                       */
	/************************************************************************/
	void requestEnterGame(); //请求开局
	void requestBackGame();  //请求恢复对局
	void requestPass();	//请求不出
	void requestContinueGame(); //请求续局

	void sendQuickStartGame();////快速开始：点了"续"15秒后，还没开局，发送允许速配，请求开局

	void requestMinPai(const cocos2d::Vector<PokerObject*> &vecPokers);//请求明牌
	void requestNoMinPai();//请求不要明牌

	/************************************************************************/
	/* UI展示类函数                                                         */
	/************************************************************************/
	void dealMyPoker(const std::string myCards, bool isResume = false); //玩家发牌
	void showMyJDZFrame(int noCall);//显示玩家叫地主界面
	void showMyQDZFrame();//显示玩家抢地主界面
	void showMyPlayBtnFrame(); //显示玩家局中出牌按钮界面
	void showMyMinPaiFrame(); //显示明牌按纽界面
	void showMyJiaBeiFrame(); //显示加倍按纽界面
	void showClockByUserId(int userId); //显示倒计时
	void showWaitForStartToast(); //显示等待开局 toast
	void showToast(const std::string &text, float time = 0); //显示 toast
	void showRoomBaseTx(); //显示房间底注

	/* 顶部菜单回调 */
	void TopMenuTrusteeCallback(); //托管
	void TopMenuSetCallback(); //设置
	void TopMenuReturnCallback(); //返回
	void exitGame(Ref* psender);//中途退出游戏

	/*结算页面回调*/
	void GameOverContinueCallBack(); //续局
	void GameOverCloseCallBack(); //结算页面关闭回调

	/************************************************************************/
	/* 消息处理函数                                                         */
	/************************************************************************/
	void ProcessStartGame(GameStartData* startGameData);
	void ProcessCallLoard(CallLordData* callLordData);
	void ProcessGrabLoard(GrabLordData* grabLordData);
	void ProcessOutCard(PlayCardData* outCardData);
	void ProcessGameOver(LordGameOverData* gameOverData);
	void ProcessCommonGameOver(GameOverCommonData* gameOverData);
	void ProcessSubstitute(GameSubstituteData* substitutedata);
	void ProcessMinPai(MingPaiData* mingPaiData);
	void ProcessJiaBei(JiaBeiData* JiaBeiData);
	void ProcessResumeGame(GameStartData* startGameData);//处理恢复对局
	//比赛开局
	void ProcessMatchStart(GameStartData* startGameData);
	//比赛恢复对局
	void ProcessMatchResume(GameStartData* startGameData);

	/************************************************************************/
	/* ShareData函数                                                        */
	/************************************************************************/


	/************************************************************************/
	/* 动画显示类函数                                                       */
	/************************************************************************/
	//播放特殊牌型动画、统计倍数、返回出的牌型
	int showSpecialCardAnimation(PlayerSite outCardSite, int lastOutCardSite, const cocos2d::Vector<PokerObject*> &vecPlayPokers);
	void showX2Animation();
	void onX2AnimationEnd();

	/************************************************************************/
	/* 其它逻辑类函数                                                       */
	/************************************************************************/
	void dataDistributedLogic(int msgType, cocos2d::Ref* objectData); //GameBasicScene接收到数据后，统一交给该函数处理，由它管理控制整个数据下发后的逻辑 
	//判断是否播放特殊牌型音效
	bool playSpecialCardEffect(PlayCardData *data);
	PlayerSite getPlaySite(int userId);//根据玩家 id 得到玩家座位号
	void clearLastPlayPokers(); //清除上家出牌记录
	void removeToast();		//移除 toast
	void resetGameInfo();   //开局清除上局UI
	void onClockTimeOut();  //闹钟超时
	void showPlayerInfo(int site);//显示玩家信息
	void updateMyLaiZiCards(); //癞子玩法确定癞子后更新自己手牌
	//癞子玩法中，处理服务器传过来的替换前后的牌，改变出来的牌的花色
	void updateLaiziCards(const cocos2d::Vector<PokerObject*> &vecPlayPokers, const cocos2d::Vector<PokerObject*> &vecReplacePokers);
	//摇癞子动画结束回调
	void onLaiZiAnimEnd();

	//------------------------自动测试 方法-----------
	void autoTestStartGame(float dt);
	void autoTestEndGame(float dt);
private:
	/************************************************************************/
	/* 牌局中节点相关指针对象                                               */
	/************************************************************************/
	CC_SYNTHESIZE(GameBasicScene*, m_pUIgameScene, pUIgameScene); //游戏主场景
	CC_SYNTHESIZE(GameTopMenuLayer*, m_pUIGameTopMenuLayer, pUIGameTopMenuLayer); //顶部栏
	CC_SYNTHESIZE(GamePlayerLayer*, m_pUIGamePlayerLayer, pUIGamePlayerLayer); //玩家和NPC层
	CC_SYNTHESIZE(MyCardsLayer*, m_pUIMyCardLayer, pUIMyCardLayer); //自己手牌
	CC_SYNTHESIZE(Node*, m_pUIToast, pUIToast); // Toast 提示
	CC_SYNTHESIZE(GameOverLayer*, m_pUIGameOverLayer, pUIGameOverLayer);//结算界面

private:
	/************************************************************************/
	/* 牌局数据类成员属性                                                   */
	/************************************************************************/
	//存放上个用户打出的牌，用来判断自己打牌时，是否其他玩家都不要牌
	CC_SYNTHESIZE(cocos2d::Vector<PokerObject*>, m_arrLastPlayPokers, LastPlayPokers);
	
	/************************************************************************/
	/* 牌局标识属性															*/
	/************************************************************************/
	CC_SYNTHESIZE(bool, m_pIsGameStart, IsGameStart);  //牌局是否开始
	CC_SYNTHESIZE(PlayerSite, m_pCurOutCardUser, CurOutCardUser);	//当前出牌玩家
	CC_SYNTHESIZE(int, m_pLastOutCardUser, LastOutCardUser);	//上一轮出牌玩家 等同 PlayerSite 但无效值为 -1
	CC_SYNTHESIZE(int, m_pLandlordId, LandLordId);		//地主 id
	CC_SYNTHESIZE(int, m_pStepTime, StepTime);		//单步时长
	CC_SYNTHESIZE(int, m_pJiaBeiTime, JiaBeiTime);	//加倍时长
	CC_SYNTHESIZE(int, m_pMultiple, Multiple);		//倍数

	CC_SYNTHESIZE(bool, m_pIsXuju, IsXuJu);//是否是续局
	
	CC_SYNTHESIZE(int, m_pGlobelTag, pGlobelTag);//弹窗tag
	CC_SYNTHESIZE(int, m_pNoCallCount, pNoCallCount);//不叫地主次数

	//游戏状态
	CC_SYNTHESIZE(kGameState, m_gameState, GameState);
	//当前房间类型
	CC_SYNTHESIZE(int, m_curRoomType, CurRoomType);
	//当前房间底注
	CC_SYNTHESIZE(int, m_curRoomBase, CurRoomBase);

	//just for test.
	CC_SYNTHESIZE(int, m_pLeftUserId, LeftUserId);		//左边玩家 id
	CC_SYNTHESIZE(int, m_pRightUserId, RightUserId);	//右边玩家 id
};

#endif /* _VIEW_GAMESCENE_GAMELOGICMANAGER_H_ */

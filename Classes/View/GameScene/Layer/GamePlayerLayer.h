/*
 * GameBgLayer.h
 *
 *
 *  Created on: 2015-5-11
 *      Author: diyal.yin
 */

#ifndef _VIEW_GAMESCENE_LAYER_GAMEPLAYERLAYER_H_
#define _VIEW_GAMESCENE_LAYER_GAMEPLAYERLAYER_H_

#include "cocos2d.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"
#include "View/GameScene/Component/NpcPlayer.h"
#include "BaseCore/Common/FactoryTools.h"
#include "Model/WWDataModel/WWUserData/Player.h"
#include "Model/WWDataModel/WWGameData/GameStartData.h"

USING_NS_CC;
using namespace std;

enum MENU_GAMEPLAYERLAYER_BTN
{
	BTN_GAMEPLAYERLAYER_CHAT_TAG =15, //聊天
	BTN_GAMEPLAYERLAYER_MYCOIN_AREA_TAG, //自己金币展示信息
	BTN_GAMEPLAYERLAYER_MYCOIN_TAG, //自己金币Tx
	BTN_GAMEPLAYERLAYER_ADDCOIN_TAG, //加金币Btn
	BIN_GAMEPLAYERLAYER_RECORD_TAG,  //记牌器ICON
	BIN_GAMEPLAYERLAYER_LEAVEGAME_TAG,//离开游戏
	BIN_GAMEPLAYERLAYER_CONTINUE_TAG  //续局
}; 


class GamePlayerLayer : public Player
{
public:
	GamePlayerLayer();
	virtual ~GamePlayerLayer();
	CREATE_FUNC(GamePlayerLayer);
	virtual bool init();

	virtual void resetAllData();//重置相关数据

	virtual	bool createPlayStatus();
	virtual void updateUserInfo(); //更新玩家展示信息
	virtual void updateLeftCardCount(int changeCount); //更新玩家剩余牌数

	void createView();
	void createInteractionView(); //创建玩家交互区域
	void onTouchSpriteDepress(Ref * theBtn);  // touchSprite按纽响应

	void setShowPlayCardFrame(bool isShow); //设置显示玩牌按钮区域
	void setShowLandlordFrame(bool isShow,int noCall); //设置显示叫地主按钮区域
	void setShowGraplordFrame(bool isShow); //设置显示抢地主按纽区域
	void setShowMingPaiFrame(bool isShow); //设置显示明牌按钮区域
	void setShowJiaBeiFrame(bool isShow);  //设置显示加倍按钮区域
	void setShowGameOverFrame(bool isShow); //设置显示续局工具栏

	void setOutBtnEnable(bool isEnable);	//设置出牌按纽是否可点
	void setDownBtnEnable(bool isEnable);	//设置放下按纽是否可点
	void setPassBtnEnable(bool isEnable);	//设置不出按纽是否可点
	void setSuperJiaBeiBtnEnable(bool isEnable); //设置超级加倍按纽是否可点

	void hideAllBtnFrame(); //隐藏所有工具条
	void hideAllPlayerStatus();

	void initPlayerData(GameStartData* startData); //开局初始化玩家信息
	void gameOverReset(); //牌局结束重置
	void reset(); //重置玩家状态和工具栏
	
	void addRecord();//添加记牌器
	void updataRecord(Vector<PokerObject*> outCardsArr);//更新记牌器
	void resetRecord();//重置记牌器

	//更新结算倒计时
	void updateGameOverTime(int time = 15);

	//玩家ui相关协议分发
	void setPlayerStatus(int statusTag,PlayerSite seat);
	void updateLeftCardCountById(int userId, int changeCount); //根据玩家id更新玩家剩余牌数
	void hidePlayerStatus(PlayerSite seat);
	void showMinPaiResult(const cocos2d::Vector<PokerObject*> &vecPokers, PlayerSite seat);
	void setPlayerRole(PlayerSite landLordSite);
	void dealNpcPoker(PlayerSite seat, int cont, bool isResume = false);
	void setAutoPlay(bool isAutoPlay, PlayerSite seat);
	/* 玩牌倒计时相关 */
	void startToCountdown(int seconds, PlayerSite seat); 
	void endCountdown();
 
private:

	void tick(float ft);

	CC_SYNTHESIZE_READONLY(Sprite*, m_pClock, Clock); //时钟
	int m_nTime; //倒计时时间
	//闹钟的计时文字
	CC_SYNTHESIZE_READONLY(cocos2d::Label*, m_pTimeLabel, pTimeLabel); //时钟

	CC_SYNTHESIZE(NpcPlayer*, l_npcLeft, npcLeft); //左NPC玩家
	CC_SYNTHESIZE(NpcPlayer*, l_npcRight, npcRight); //右NPC玩家

	CC_SYNTHESIZE(WWTouchSprite*, b_lUnOut, lUnOut); //不出
	CC_SYNTHESIZE(WWTouchSprite*, b_lTips, lTips); //提示
	CC_SYNTHESIZE(WWTouchSprite*, b_lDown, lDown); //放下
	CC_SYNTHESIZE(WWTouchSprite*, b_lchupai, lchupai); //出牌
	CC_SYNTHESIZE(WWTouchSprite*, b_lJiaoDiZhu, lJiaoDiZhu); //叫地主
	CC_SYNTHESIZE(WWTouchSprite*, b_lBuJiaoDiZhu, lBuJiaoDiZhu); //不叫地主
	CC_SYNTHESIZE(WWTouchSprite*, b_lGrapLord, lGrapLord); //抢地主
	CC_SYNTHESIZE(WWTouchSprite*, b_lNotGrapLord, lNotGrapLord); //不抢地主
	CC_SYNTHESIZE(WWTouchSprite*, b_lBuMingpai, lBuMingpai); //不明牌
	CC_SYNTHESIZE(WWTouchSprite*, b_lMingpai, lMingpai); //明牌
	CC_SYNTHESIZE(WWTouchSprite*, b_lBuJiaBei, lBuJiaBei); //不加倍
	CC_SYNTHESIZE(WWTouchSprite*, b_lJiaBei, lJiaBei); //加倍
	CC_SYNTHESIZE(WWTouchSprite*, b_lSurperJiaBei, lSurperJiaBei); //超级加倍
	CC_SYNTHESIZE(WWTouchSprite*, b_recordIcon, RecordIcon);//记牌器icon
	CC_SYNTHESIZE(WWTouchSprite*, b_lLeaveGame, lLeaveGame); //离开游戏
	CC_SYNTHESIZE(WWTouchSprite*, b_lContinueGame, lContinueGame); //再来一局
	CC_SYNTHESIZE(Label*, b_lContinueTx, lContinueTx); //再来一局tx
	CC_SYNTHESIZE(Label*, b_lTimeTx, lTimeTx); //倒计时Tx
	CC_SYNTHESIZE(bool, b_isGameOverTimeUp, IsGameOverTimeUp); //倒计时是否已到
	CC_SYNTHESIZE(Sprite*, b_recordBg, RecordBg);//記牌器背景
	
	CC_SYNTHESIZE(bool, m_pIsCallLord, pIsCallLord); //是否叫过地主
	CC_SYNTHESIZE(bool, m_pIsUsedRecord, IsUsedRecord);//是否使用记牌器
	//CC_SYNTHESIZE(cocos2d::Map<int,int>, m_cardRecord, CardRecord);//记牌器使用时的牌数量

	std::map<int,int> pMMap;
};

#endif /*_VIEW_GAMESCENE_LAYER_GAMEPLAYERLAYER_H_*/
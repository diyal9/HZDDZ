/*
 * NpcPlayer.h
 * NPC UI创建
 *
 *  Created on: 2015-5-13
 *      Author: diyal.yin
 */

#ifndef _VIEW_GAMESCENE_COMPONENT_NPCPLAYER_H_
#define _VIEW_GAMESCENE_COMPONENT_NPCPLAYER_H_

#include "cocos2d.h"
#include "View/Common/CardSprite/CardsGroup.h"
#include "Model/WWDataModel/WWUserData/Player.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

enum NPCPLAYERLAYER_BTN
{
	BTN_NPCPLAYERLAYER_NICK_TAG = 1,	 //昵称
	BTN_NPCPLAYERLAYER_COIN_TAG,		 //金币
	BTN_NPCPLAYERLAYER_ICON_TAG          //金币Icon  
};

class NpcPlayer : public Player
{
public:
	NpcPlayer();
	virtual ~NpcPlayer();
	static NpcPlayer* create(bool isRightDirect);

	virtual void resetAllData();//重置相关数据

	virtual bool init(bool isRightDirect);
	virtual bool createPlayStatus();
	virtual void updateUserInfo(); //更新个人信息
	virtual void updateLeftCardCount(int changeCount); //更新玩家剩余牌数
	virtual void onSmogAnimEnd();
	virtual void onHeadDepress(Ref * theBtn);  //点击头像响应
	virtual void resetHead();	//重置头像

	void createView();
	//void dealAction(); //发牌动作 默认 17张
	void dealAction(int cardCount, bool hadAction = false); //发牌动作 指定数量
	void reflashPoker(bool hadAction, int cardCount); //出牌后，牌面位置重新调整
	Point getCirclePosition(int nIndex, int curCardNum); //获得当前牌的坐标(第几张，一共几张)

	void addDiZhuPoker(int cardCount); //添加地主牌
	void playPoker(int cardCount); //出牌

	/*出牌相关*/
	void saveOutCard(const cocos2d::Vector<CardSprite*> &cardArr); 
	void saveLeftCard(CardsGroup* cardArr);
	void clearOutCard();

	void cleanPoker(); //清空牌
	void hideUserInfo(); //隐藏NPC玩家桌面信息展示

	void setBrokeHouseVisible(bool isShow); //炸弹动画显示被炸房子

	void setNpcPanelPosition(bool hadSetImage); //设置Npc坐标位置
	void changeNpcInfo(bool hadSetImage); //NPC信息状态修改

private:

	CC_SYNTHESIZE_READONLY(cocos2d::Vector<CardSprite*>, m_arrOutCards, OutCards);
	CC_SYNTHESIZE_READONLY(CardsGroup*, m_arrLeftCards, LeftCards);
	CC_SYNTHESIZE(Sprite*,m_pBgSprite,pBgSprite)
	CC_SYNTHESIZE(bool, m_pIsRightDirect, pIsRightDirect); //设置是否右方向展示、初始是右方向显示
	CC_SYNTHESIZE(Point, m_nCircleCenter, nCircleCenter); //圆形位置
	//CC_SYNTHESIZE(float, m_fRadius, fRadius); //半径
	CC_SYNTHESIZE(bool, m_pIsCallLord, pIsCallLord); //是否叫过地主
	CC_SYNTHESIZE(bool, m_pIsOutCard, pIsOutCard);	 //是否出过牌

	CC_SYNTHESIZE(int, m_pCurCardCount, pCurCardCount); //NPC的牌张数

	//账户信息
	//Sprite* b_lRoleAccount;
    ui::Scale9Sprite* b_lRoleAccount;

	//废虚房子
	Sprite* m_brokeHouse;

	//std::vector<int> m_pNpcPokerArr;//NPC手牌数据Tag
	cocos2d::Vector<Sprite*> m_pNpcPokerArr;

	Label* m_pLabelNum;

	bool isSetPlayIcon; //设置是否有头像Icon
};

#endif /*_VIEW_GAMESCENE_COMPONENT_NPCPLAYER_H_*/

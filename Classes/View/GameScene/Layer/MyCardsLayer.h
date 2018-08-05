/*
 * MyCardsLayer.h
 *
 *  Created on: 2015-5-16
 *      Author: Jacky
 */
#ifndef _View_GameScene_Layer_MyCardsLayer_H_
#define _View_GameScene_Layer_MyCardsLayer_H_

#include "cocos2d.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"
#include "View/Common/CardSprite/CardsGroup.h"
#include "View/GameScene/GameBasicScene.h"

USING_NS_CC;

class MyCardsLayer : public Layer
{
public:
	MyCardsLayer(void);
	virtual ~MyCardsLayer(void);

	CREATE_FUNC(MyCardsLayer);

	virtual bool init();
	
	virtual void onEnter();

	virtual void onExit();

	void clear();

	//触摸响应函数
	virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	//开局重置数据
	void reset();

	//初始化玩家手中的牌
	void initPlayersCard(const cocos2d::Vector<PokerObject*> &vecMyPokers);

	//初始化断线重连时玩家手中的牌
	void initRecoveryPlayersCard(const cocos2d::Vector<PokerObject*> &vecMyPokers);

	//处理三张底牌信息
	void handleLastThreeCards(const cocos2d::Vector<PokerObject*> &vecBottomPokers, bool isMyLord);

	void setTouchEnabled(bool bEnable);
	
	 /**
     *  按照从大到小排序
     *
     *  @param 要排序的数组，存放的是CardSprite
     *
     *  @return 返回排序好的数组
     */
	cocos2d::Vector<CardSprite*> sortArrBtoS(cocos2d::Vector<CardSprite*> &vecCards);

	//设置zorder
	void setCardsZOrder(const cocos2d::Vector<CardSprite*> &vecCards);

	 /**
     *  从屏幕中移除已经打出的牌
     */
	void removeMyOutCards();
	
	 /**
     *  重新开局清除自己的手牌
     */
	void removeMyHandCards();
	
	 /**
     *  重连显示顶部工具条地主牌动画
     */
	void showLandLordCard();

	 /**
     *  显示顶部工具条地主牌动画
     */
	void showLandLordCardAnim();

	 /**
     *  删除顶部工具条地主牌
     */
	void removeLandLordCards();

	/**
     *  增加手中的牌
     *
     *  @param 
     *
     *  @return 
     */
	void addCard(const cocos2d::Vector<PokerObject*> &vecPokers);
	
	//运行加牌动作
	void runAddCardAction(Ref *pNode);
	
	/**
     *  执行打牌动作
     *
     *  @param arrCards需要打牌的数据，存放的是PokerObject
     *  @param outCardType需要打的牌类型
     *
     *  @return 
     */
    void playCards(const cocos2d::Vector<PokerObject*> &vecPlayPokers, int outCardType);

	//显示打牌结果
	void showPlayCardsResult(const cocos2d::Vector<PokerObject*> &vecPlayPokers, int outCardType);

	void showPlayCardsResult(const cocos2d::Vector<PokerObject*> &vecPlayPokers, const cocos2d::Vector<PokerObject*> &vecReplacePokers, int outCardType);

	//出牌
	void requestPlayCards(const cocos2d::Vector<PokerObject*> &vecPlayPokers, const cocos2d::Vector<PokerObject*> &vecReplacePokers);

	//响应出牌按纽
	void onOutCardBtnClick();

	//响应放下按纽
	void putCardsDown();

	//响应提示按纽
	void onTipBtnClick();
	
	//响应明牌按纽
	void onMinPaiBtnClick();

	//设置出牌工具栏状态
	void checkPlayCardBtnState(); 
	
	//删除摇癞子动画资源
	void removeLaiZiAnim();

	//摇癞子时底牌左移
	void onLaiziComeAndMoveDiPai();
	
	//开始摇癞子
	void startChooseLaiZi();

	//癞子动画盒子移动和抖动完成
	void onBoxMoveAndShakeEnd(Node *pNode);
	
	//癞子动画光切完成
	void onLightMoveEnd(Node *pNode);

	//癞子选择完成
	void onChooseLaiZiEnd(Node *pNode = nullptr);

	//癞子飞到顶部后
	void onLaiZiFlyToTop(Node *pNode);
	
	//增加癞子牌到顶部栏（仅适用于断线重连时）
	void addLaiziCardToTop();

	//更新手中的癞子牌
	void updateMyLaiZiCards(int laiziNum);
	
	//癞子时，把自己手中的牌替换成服务器传过来的要替换成的牌
	void changMyCardsToReplaceCards(const cocos2d::Vector<PokerObject*> &vecReplacePokers);
    
	//检测是否有牌大过上家
	void setNoBiggerTipsShow(bool isMyTurn);

private:

	//从给定数组中把牌置为选中状态,数组中是pokerObject，提示时使用
	void setCardsSelectedState(const cocos2d::Vector<PokerObject*> &vecPokers);
	
	//最后点击的牌是否是弹起状态，提示时用
	bool isLastSelectCardUp();

	//初始化牌堆 
	bool initBackCards();

	//开始发牌
	void startDistributeCards(float dt);

	//发最后三张底牌
	void distributeLastThreeCards(const cocos2d::Vector<Node*> &vecAllBackCard);

	//底牌飞向顶部之后
	void afterFlyCards();

	//选牌移动过程中，设置牌的前景色
	void setCardsStateOnMoved();

	//比对本地记录出牌和服务返回的出牌
	void checkOutCard(const cocos2d::Vector<PokerObject*> &vecPlayPokers);
	
	/** 获取自己手牌poker数组 **/
	cocos2d::Vector<PokerObject*> getPokerArr();

private:

	//牌堆 
	SpriteBatchNode *m_pBGBatchNode;

	//触摸的第一张牌的tag值
	int m_nFirstTouchTag;

	//上次触摸的牌，用于判断连续触摸时是否会造成一张牌被多次选择
	int m_nLastTouchTag;

	//控制发牌
	int m_nCardIndex;

	//是否是滑动选牌
	bool m_nIsMoveTouch;

	cocos2d::EventListenerTouchOneByOne *m_pListener;

	/**
     *  自己手中的牌，游戏开局时初始化
     */
	CC_SYNTHESIZE_READONLY(cocos2d::Vector<CardSprite*>, m_arrMyCards, MyCards);

	/**
     *  将要打出的牌，用来移除当前桌面上自己打出的牌
     */
	CC_SYNTHESIZE_READONLY(cocos2d::Vector<CardSprite*>, m_arrPlayedCards, PlayedCards);
	
	//存放上一次提示的牌
	CC_SYNTHESIZE(cocos2d::Vector<PokerObject*>, m_arrLastTipPokers, LastTipPokers);

	//获取选中的牌
	cocos2d::Vector<PokerObject*> getSelectedPokers();

    //获取打出去的牌数组
    cocos2d::Vector<PokerObject*> getOutPokerArr();

	//牌打到桌子上的回调函数
	void onPlayCardsActionEnd(Node *pCard);

	/**
     *  是否托管
     */
	CC_SYNTHESIZE(bool, m_bIsAutoPlay, IsAutoPlay);
	CC_SYNTHESIZE_READONLY(Sprite*, b_lSubstitute, lSubstitute); //托管
	CC_SYNTHESIZE_READONLY(Sprite*, b_lNoBiggerCard, lNoBiggerCard); //没有牌大过上家
	bool m_bIsCardClick;
	void setSubtitude(bool isAuto);

	/**
     *  是否是地主
     */
	CC_SYNTHESIZE(bool, m_isLord, IsLord);
	
	//是否出过牌，在当前的一圈中
	CC_SYNTHESIZE(bool, m_bIsOutCards, IsOutCards);
	
	//更新自己手中牌的位置
	void updateMyCardsPos();
};

#endif



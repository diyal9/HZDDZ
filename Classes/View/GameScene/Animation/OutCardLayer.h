/*
 * OutCardLayer.cpp
 * 
 * desc 出牌动画统一处理，后面可能要加特效
 *
 *  Created on: 2015年5月19日
 *      Author: Jacky
 */

#ifndef OutCardLayer_H_
#define OutCardLayer_H_

#include "cocos2d.h"
#include "BaseCore/Common/FactoryTools.h"
#include "View/Common/CardSprite/CardSprite.h"

USING_NS_CC;

class OutCardLayer :public Layer
{
public:
	/**
	 * 创建自己出牌动画层
	 * @param
	 *		  outCards 出牌 UI
	 * 		  func 动画结束回调方法
	 */
	static OutCardLayer* showOutCardSelf( const cocos2d::Vector<CardSprite*> &outCards, int outCardType, bool isAutoPlay, bool isLandLord, cocos2d::CallFunc *func);
	
	/**
	 * 创建其它玩家出牌动画层
	 * @param seat 位置
	 *		  outCards 出牌 PokerObject  需要创建牌
	 *		  isLandLord 是否地主
	 * 		  func 动画结束回调方法
	 */
	static OutCardLayer* showOutCardOther(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func);

	/**
	 * 创建其它玩家剩余手牌展示层
	 * @param seat 位置
	 *		  outCards 出牌 PokerObject  需要创建牌
	 *		  isLandLord 是否地主
	 * 		  func 动画结束回调方法
	 */
	static OutCardLayer* showLeftCardOther(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func);

private:
	
	virtual bool initViewAsSelfCard(const cocos2d::Vector<CardSprite*> &outCards, int outCardType, bool isAutoPlay, bool isLandLord, cocos2d::CallFunc *func);

	virtual bool initViewAsOtherOutCard(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func);
	
	virtual bool initViewAsOtherLeftCard(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func);

	//左边玩家出牌
	void intLeftOutCardsAndRunAnim(const cocos2d::Vector<CardSprite*> &outCards, int outCardType, bool isLandlord, cocos2d::CallFunc *func);

	//右边玩家出牌
	void intRightOutCardsAndRunAnim(const cocos2d::Vector<CardSprite*> &outCards, int outCardType, bool isLandlord, cocos2d::CallFunc *func);
	
	//左边玩家剩余手牌展示
	void showLeftLeftCards(const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func);
	
	//右边玩家剩余手牌展示
	void showRightLeftCards(const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func);

	//动画结束回调
	void onActionEnd();

};

#endif /* OutCardLayer_H_ */

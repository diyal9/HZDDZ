/*
 * CardSprite.h
 *
 *  Created on: 2015-5-13
 *      Author: Jacky
 */
#ifndef _VIEW_COMMON_CARDSPRITE_CARDSPRITE_H_
#define _VIEW_COMMON_CARDSPRITE_CARDSPRITE_H_


#include "cocos2d.h" 
#include "Poker.h"

//根据pokerNum返回pokerValue
#define POKERVALUE(i) ((TYPE_L << 4) | i)

USING_NS_CC;

enum sprite_tag
{
	Tag_FrontCard = 100,
	Tag_BackCard,
	Tag_KingCard,
	Tag_UpNum,
	Tag_UpColor,
	Tag_DownNum,
	Tag_DownColor,
    Tag_LordSign
};

enum card_state
{
	card_normal = 1,                     //正常状态，在玩家手中
	card_selected,                       //选中状态，即将出牌
	card_distribute,                     //牌已出
	card_selecting,                      //正在选状态，还未选中
	card_nomal_selecting,				 //正常牌在选状态
	card_popup_selecting				 //选中牌在选状态
};

enum card_size
{
    big_Card = 10,
    normal_Card,
    small_Card,
};

class CardSprite : public cocos2d::__NodeRGBA
{
public:
	CardSprite(void);
	virtual ~CardSprite(void);

	//点数和花色创建扑克对象
	static CardSprite* create(PokerObject* poker);

    //功能同上，加了一个牌的大小
    static CardSprite* create(PokerObject* poker, card_size size);

	bool init(PokerObject* poker, card_size size);

	void setCardSize(card_size size);

	card_size getSize(){return m_size;}

	//触摸处理函数
	bool isContainsTouch(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

    //开始反转牌面
	void startTurnCard(float duration);

    //获取单张牌的尺寸
	Size getCardSize();

    //加地主角标
    void addLordSign();

    //移除地主角标
    void removeLordSign();

	//设置牌的颜色
	void setCardColor(const cocos2d::Color3B& color3);

    //重写设置锚点，连带把牌面精灵的也设置了
    void setAnchorPoint(const Vec2 &point);

	//取消牌的状态，用于滑动中取消选择
	void cancelSelecting();
    
	
    /**
     *  将三带一的牌型排序
     *
     *  @param pokerArr 需要排序的 CardSprite 数组
     *
     *  @return 主牌在前副牌在后的 CardSprite 数组
     */
	static cocos2d::Vector<CardSprite*> sortThreeTake(cocos2d::Vector<CardSprite*> pokerArr);
	
    /**
     *  将四带二的牌型排序
     *
     *  @param pokerArr 需要排序的 CardSprite 数组
     *
     *  @return 主牌在前副牌在后的 CardSprite 数组
     */
	static cocos2d::Vector<CardSprite*> sortFourTake(cocos2d::Vector<CardSprite*> pokerArr);


	//设置出牌状态
	CC_PROPERTY(card_state, m_nCardState, CardState);

	//是否是手中牌的最后一张
	CC_SYNTHESIZE(bool, m_bIsLastCard, IsLastCard);

	//牌型信息
	CC_SYNTHESIZE_RETAIN(PokerObject*, m_pPoker, Poker);
    
private:
	bool initWithSize(card_size size);

    Size m_cardSize;

    card_size   m_size;//牌大小

	//存储上次的牌的状态，用于滑动中取消选中时
	CC_SYNTHESIZE(card_state ,m_nLastCardState, LastCardState);

};

#endif

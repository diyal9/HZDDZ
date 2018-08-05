//
//  CardsGroup.h
//
//  Created by Jacky on 5/19/15.
//
//

#ifndef __View_Common_CardSprite_CardsGroup__
#define __View_Common_CardSprite_CardsGroup__

#include <iostream>
#include "cocos2d.h"
#include "CardSprite.h"
#include "Poker.h"

USING_NS_CC;

class CardsGroup : public cocos2d::__NodeRGBA
{
public:
    CardsGroup();

    virtual ~CardsGroup();

	/*创建一组牌
	* pragram:
	*	Vector<PokerObject*> &cardsArr; 牌值数组
	* return:
	*	Vector<CardSprite*> cardSprite; 牌数组
	*/
	static cocos2d::Vector<CardSprite*> createPokers(const cocos2d::Vector<PokerObject*> &cardsArr, card_size size, Vec2 pos, bool isLandLord);

    //创建一组牌,heightRatio为牌换行后，上下牌之间的间距：牌高度*heightRatio
    static CardsGroup* create(const cocos2d::Vector<PokerObject*> &cardsArr, card_size size, float heightRatio = 1.0f);

    //加上了地主标示
    static CardsGroup* create(const cocos2d::Vector<PokerObject*> &cardsArr, card_size size, bool isLandLord);

    //初始化一组牌
    bool initWithCardsArr(const cocos2d::Vector<PokerObject*> &cardsArr, card_size size, bool isLandLord);

    //减掉一组牌中的某些cardsArr为减掉的牌
    bool cutDownCards(const cocos2d::Vector<PokerObject*>& cardsArr);
    
    //更新手中的牌
    void setAllMyCards(const cocos2d::Vector<PokerObject*> &cardsArr);

    //获取到所有的牌精灵
    cocos2d::Vector<CardSprite*> getAllMyCardsSprites();

    //获取到单张牌的尺寸
    Size getCardContentSize();
    
    //获取整个手牌尺寸
    Size getGroupContentSize();

	//重新刷新牌面（明牌时候用到）
	void resetCards();

	//设置明牌时，对于牌数小于10的情况下，设置一次位置。否则位置会乱。
	void setMPLessTenCardsPos();
    
	//是否地主
    CC_SYNTHESIZE(bool, m_isLandLord, LandLord);
    
	//设置牌如果换行的时候，牌的上下间距，这里用一个值：牌高度*ratio
	CC_SYNTHESIZE(float, m_fHeightMarginRatio, HeightMarginRatio);  

private:
    cocos2d::Vector<PokerObject*>    m_allMyCards;//手牌数据
    
    cocos2d::Vector<CardSprite*>     m_allMyCardSprites;//手牌精灵
    
    card_size            m_size;//牌的大小
    
    Size         m_cardContentSize;//每张牌的尺寸
    
    //将牌加上来
    void addCardSprite();

    
};
#endif /* defined(__WW_Platform__CardsGroup__) */

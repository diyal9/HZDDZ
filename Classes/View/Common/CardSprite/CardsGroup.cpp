//
//  CardsGroup.cpp
//
//  Created by Jacky on 5/19/15.
//
//

#include "CardsGroup.h"
#include "View/GameScene/GameSceneDefine.h"

const int g_nMaxCardNum = 10;               //最大牌数

USING_NS_CC;

CardsGroup::CardsGroup() :
	m_size(big_Card),
	m_cardContentSize(0, 0),
	m_isLandLord(false),
	m_fHeightMarginRatio(1.0f),
	m_allMyCards(0)
{
    
}
CardsGroup::~CardsGroup()
{

}

cocos2d::Vector<CardSprite*> CardsGroup::createPokers(const cocos2d::Vector<PokerObject*> &cardsArr, card_size size, Vec2 pos, bool isLandLord)
{
	cocos2d::Vector<CardSprite*> cardSprites;

    int cardsCount = cardsArr.size();
    for (int i = 0; i < cardsCount; i++)
    {
        PokerObject* object = (PokerObject*)cardsArr.at(i);
        CardSprite* card = CardSprite::create(object, size);
        card->setAnchorPoint(Vec2(0.5, 0.5));
		card->setPosition(pos);
        card->setTag(object->getPokerValue());
        card->startTurnCard(0.0f);
        cardSprites.pushBack(card);
    }
    
	return cardSprites;
}

CardsGroup* CardsGroup::create(const cocos2d::Vector<PokerObject*> &cardsArr, card_size size, bool isLandLord)
{
    CardsGroup *pRet = new CardsGroup();
	if (pRet && pRet->initWithCardsArr(cardsArr,size,isLandLord))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

CardsGroup* CardsGroup::create(const cocos2d::Vector<PokerObject*> &cardsArr, card_size size, float heightRatio/* = 1.0f*/)
{
    CardsGroup *pRet = new CardsGroup();
	if (pRet)
	{
		pRet->setHeightMarginRatio(heightRatio);
		if (pRet->initWithCardsArr(cardsArr,size,false))
		{
			pRet->autorelease();
			return pRet;
		}
	}

	delete pRet;
	pRet = nullptr;
	return nullptr;
}

void CardsGroup::addCardSprite()
{
    if (m_allMyCardSprites.size() > 0) 
	{
        m_allMyCardSprites.clear();
    }
    
    removeAllChildrenWithCleanup(true);
    
    int cardsCount = m_allMyCards.size();
    for (int i = 0; i < cardsCount; i++)
    {
        PokerObject* object = (PokerObject*)m_allMyCards.at(i);
        CardSprite* card = CardSprite::create(object, m_size);
        card->setAnchorPoint(Vec2(0.5, 0.5));
        card->setTag(object->getPokerValue());
        m_cardContentSize = card->getCardSize();
        if (cardsCount > g_nMaxCardNum)//牌数数大于10的时候最初的在上，后来的在下
        {
            if (i < g_nMaxCardNum)
            {
                card->setPosition(Vec2((i % g_nMaxCardNum) * DISTRIBUTE_OUT_CARDS_MAGIN, 0));
				if( m_isLandLord && i+1 >= g_nMaxCardNum ){ //第一排最后一张加地主角标
					card->addLordSign();
				}
            }
            else
            {
                card->setPosition(Vec2((i % g_nMaxCardNum) * DISTRIBUTE_OUT_CARDS_MAGIN, -DISTRIBUTE_OTHER_OUT_CARDS_ROW_MAGIN));
				if( m_isLandLord && i+1 >= cardsCount ){ //第二排最后一张加地主角标
					card->addLordSign();
				}
            }
        }
        else//牌数小于等于10全都在上
        {
            card->setPosition(Vec2((i % g_nMaxCardNum) * DISTRIBUTE_OUT_CARDS_MAGIN, 0));
			if( m_isLandLord && i+1 >= cardsCount ){ //最后一张加地主角标
				card->addLordSign();
			}
        }
        
        card->startTurnCard(0.0f);
        log("card position x=%f,y=%f", card->getPosition().x, card->getPosition().y);
        addChild(card);
        m_allMyCardSprites.pushBack(card);
    }
}

bool CardsGroup::initWithCardsArr(const cocos2d::Vector<PokerObject*> &cardsArr, card_size size, bool isLandLord)
{
    if (!(__NodeRGBA::init()))
	{
        return false;
    }

    if (cardsArr.size() == 0)
    {
        log("cardsArr must be non nil");
        return false;
    }

    m_size = size;
    m_isLandLord = isLandLord;
    setCascadeOpacityEnabled(true);
   // setAnchorPoint(ccp(0, 1));
    
    if (m_allMyCards.size() > 0) 
	{
        m_allMyCards.clear();
    }
    m_allMyCards = cardsArr;
    
    addCardSprite();
    return true;
}

bool CardsGroup::cutDownCards(const cocos2d::Vector<PokerObject*>& cardsArr)
{
    if (cardsArr.size() > 0)
    {
		for (auto poker : cardsArr)
		{
			if (poker) 
			{
				log("cutDownCards poker value=%d ", poker->getPokerValue());
				removeChildByTag(poker->getPokerValue(), true);
				
				Vector<CardSprite*> vecTmpPokers;
				for (auto card : m_allMyCardSprites)
				{
					if (card && card->getTag() == poker->getPokerValue())
					{
						vecTmpPokers.pushBack(card);
					}
				}

				for (auto card : vecTmpPokers)
				{
					if (card)
					{
						m_allMyCardSprites.eraseObject(card);
					}
				}
			}
		}
    }
    else
    {
        log("the cardsArr is null");
    }

    resetCards();
	if (m_allMyCardSprites.size() == 0)
	{
		this->getParent()->removeFromParentAndCleanup(true);
	}
	
    return true;
}

void CardsGroup::resetCards()
{
    int cardCount = m_allMyCardSprites.size();
    for (int i = 0; i < cardCount; i++)
    {
        CardSprite* card = m_allMyCardSprites.at(i);
        Size cardSize = card->getCardSize();
		Vec2 toPoint = Vec2::ZERO;
        card->removeLordSign();//移除角标
        if (i < 10)
        {
            toPoint = Vec2((i % 10) * cardSize.width * 0.7f, cardSize.height);
        }
        else
        {
            toPoint = Vec2((i % 10) * cardSize.width * 0.7f, 0);
        }

        MoveTo* move = MoveTo::create(0.2f, toPoint);
        card->runAction(move);
    }
    if (m_isLandLord && m_allMyCardSprites.size() > 0) 
	{
		//添加角标
        CardSprite* lastSprite = m_allMyCardSprites.back();
        if (lastSprite) 
		{
            lastSprite->addLordSign();
        }
    }
}

void CardsGroup::setMPLessTenCardsPos()
{
	int cardCount = m_allMyCardSprites.size();
	for (int i = 0; i < cardCount; i++)
	{
		CardSprite* card = m_allMyCardSprites.at(i);
		Size cardSize = card->getCardSize();
		Vec2 toPoint = Vec2::ZERO;
		card->removeLordSign();//移除角标
		if (i < 10)
		{
			toPoint = Vec2((i % 10) * cardSize.width * 0.7, cardSize.height);
		}
		else
		{
			toPoint = Vec2((i % 10) * cardSize.width * 0.7, 0);
		}

		card->setPosition(toPoint);
	}
	if (m_isLandLord && m_allMyCardSprites.size() > 0) 
	{
		//添加角标
		CardSprite* lastSprite = m_allMyCardSprites.back();
		if (lastSprite) 
		{
			lastSprite->addLordSign();
		}
	}
}

void CardsGroup::setAllMyCards(const cocos2d::Vector<PokerObject*> &cardsArr)
{
	m_allMyCards.clear();
    m_allMyCards = cardsArr;
    
    resetCards();
}

 cocos2d::Vector<CardSprite*> CardsGroup::getAllMyCardsSprites()
{
    return m_allMyCardSprites;
}

Size CardsGroup::getCardContentSize()
{
    return m_cardContentSize;
}

Size CardsGroup::getGroupContentSize()
{
    CardSprite* firstCard = m_allMyCardSprites.at(0);
    CardSprite* lastCard = m_allMyCardSprites.back();

	CCAssert(firstCard != nullptr && lastCard != nullptr, "");
	
    float groupWidth = lastCard->getPosition().x - firstCard->getPosition().x + m_cardContentSize.width;
    float groupHeight = firstCard->getPosition().y - lastCard->getPosition().y + m_cardContentSize.height;

	//如果牌换行了，则取一行的末尾的牌来计算宽度
	if (m_allMyCardSprites.size() > g_nMaxCardNum)
	{
		CardSprite* tempCard = m_allMyCardSprites.at(g_nMaxCardNum - 1);
		if (tempCard)
		{
			groupWidth = tempCard->getPosition().x - firstCard->getPosition().x + m_cardContentSize.width;
		}
	}
	
    return Size(groupWidth, groupHeight);
}

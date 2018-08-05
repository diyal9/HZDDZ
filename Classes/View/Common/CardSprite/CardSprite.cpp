/*
 * CardSprite.cpp
 *
 *  Created on: 2015-5-13
 *      Author: Jacky
 */
#include "CardSprite.h"
#include "View/GameScene/GameSceneDefine.h"

USING_NS_CC;

CardSprite::CardSprite(void):
	m_nCardState(card_normal),
	m_bIsLastCard(false),
	m_pPoker(nullptr),
	m_cardSize(Vec2::ZERO),
    m_size(big_Card),
	m_nLastCardState(card_normal)
{
}

CardSprite::~CardSprite(void)
{
	CC_SAFE_RELEASE(m_pPoker);
}

CardSprite* CardSprite::create(PokerObject* poker)
{
	CardSprite *pRet = new CardSprite(); 
	if (pRet && pRet->init(poker,big_Card))
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

CardSprite* CardSprite::create(PokerObject* poker,card_size size)
{
    CardSprite *pRet = new CardSprite();
	if (pRet && pRet->init(poker,size))
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

bool CardSprite::init(PokerObject* poker,card_size size)
{
	if (!__NodeRGBA::init())
	{
		return false;
	}

	if (!poker)
	{
		log("CardSPrite::init fail! poker is nullptr");
		return false;
	}
    
    setCascadeOpacityEnabled(true);
    setAnchorPoint(Vec2(0, 0));
    
    removeAllChildrenWithCleanup(true);
    
    if (m_pPoker) 
	{
        CC_SAFE_RELEASE(m_pPoker);
        m_pPoker = nullptr;
    }

	m_pPoker = poker;
	m_pPoker->retain();

	if (!initWithSize(size))
	{
		return false;
	}

	return true;
}

bool CardSprite::initWithSize(card_size size)
{
	m_size = size;

	Sprite *pFrontSprite = nullptr;							//牌前景
	Sprite *pBackSprite = nullptr;							//牌背景
	std::string strKingTxImg = "";							//王牌左上角图片
	std::string strKingImg = "";							//王牌图片
	uint8_t color = m_pPoker->getPokerColor();              //花色
	uint8_t point = m_pPoker->getPokerNum();                //点数
    //CCLOG("the cardsize ==%d",size);

	pFrontSprite = Sprite::createWithSpriteFrameName("face.png");
	pBackSprite =  Sprite::createWithSpriteFrameName("back.png");

	pFrontSprite->setPosition(Vec2::ZERO);
	pFrontSprite->setVisible(false);
	addChild(pFrontSprite, 0, Tag_FrontCard);

	pBackSprite->setPosition(Vec2::ZERO);
	pBackSprite->setVisible(false);
	addChild(pBackSprite, 0, Tag_BackCard);

	if (point >= R_WA)	// 王牌
	{
		strKingTxImg =  StringUtils::format("joeker%d.png",point);
		Sprite *pKingTextSprite = Sprite::createWithSpriteFrameName(strKingTxImg);

		strKingImg =  StringUtils::format("king%d.png",point);
		Sprite *pKingSprite = Sprite::createWithSpriteFrameName(strKingImg);

		if (pKingSprite && pKingTextSprite)
		{
			pKingTextSprite->setPosition(Vec2(pFrontSprite->getPositionX() + pKingTextSprite->getContentSize().width * 0.5f + DISTIRBUTE_BETWEEN_NUM_CARD_MARGIN,
				pFrontSprite->getContentSize().height - pKingTextSprite->getContentSize().height * 0.5f - DISTIRBUTE_BETWEEN_NUM_CARD_MARGIN));
			pFrontSprite->addChild(pKingTextSprite, 0, Tag_KingCard);
			
			pKingSprite->setPosition(Vec2(pFrontSprite->getContentSize().width * 0.62f,pFrontSprite->getContentSize().height * 0.45f));
			pFrontSprite->addChild(pKingSprite, 0, Tag_KingCard);

			if (size == small_Card) {
				pKingTextSprite->setScale(0.92f);   // 为原始大小 22% 但需要先放大背景放大比例 24%
				pKingSprite->setScale(0.84f);	   // 为原始大小 20% 但需要先放大背景放大比例 24%
				pFrontSprite->setScale(0.24f);
				pBackSprite->setScale(0.24f);
			}
		}

	}
	else
	{
		Sprite *pUpNumSprite = nullptr;
		Sprite *pUpColorSprite = nullptr;
		Sprite *pDownColorSprite = nullptr;
		std::string numSpriteName = "";
		if (color == TYPE_B || color == TYPE_M)//黑桃和梅花
		{
			numSpriteName = StringUtils::format("B%d", point);
		}
		else//方块和红心
		{
			numSpriteName = StringUtils::format("R%d", point);
		}
		std::string numSpriteName2 = "";//加上大小的数字名字

		//花色
		const char* colorSpriteName = nullptr;
		switch (color) {
		case TYPE_B:
			{
				colorSpriteName = "spade"; //黑桃
			}
			break;
		case TYPE_H:
			{
				colorSpriteName = "heart"; //红桃
			}
			break;
		case TYPE_M:
			{
				colorSpriteName = "club"; //梅花
			}
			break;
		case TYPE_F:
			{
				colorSpriteName = "diamond"; //方块
			}
			break;
        default:
            {
                if (color >> 3 == 1)
                {
                    colorSpriteName = "rascal";
                }
            }
            break;
		}

		numSpriteName2 = StringUtils::format("%s.png", numSpriteName.c_str());
		pUpNumSprite = Sprite::createWithSpriteFrameName(numSpriteName2);

		std::string spriteColor = StringUtils::format("%s.png", colorSpriteName);
		pUpColorSprite = Sprite::createWithSpriteFrameName(spriteColor);
		pDownColorSprite = Sprite::createWithSpriteFrameName(spriteColor);

		switch (size) {
		case big_Card:
			{
				pUpColorSprite->setScale(0.6f);
			}
			break;
		case normal_Card:
			{
				pUpColorSprite->setScale(0.6f);
			}
			break;
		case small_Card:
			{
				pUpNumSprite->setScale(1.3125f);  // 为原始大小 31.5% 但需要先放大背景放大比例 24%
				pUpColorSprite->setScale(1.25f);	 // 为原始大小 30% 但需要先放大背景放大比例 24%
				pFrontSprite->setScale(0.24f);
				pBackSprite->setScale(0.24f);
			}
			break;
		default:
			break;
		}

		if (pUpNumSprite&&pUpColorSprite)
		{
			pUpNumSprite->setPosition(Vec2(pFrontSprite->getPositionX() + pUpNumSprite->getContentSize().width * 0.5f + DISTIRBUTE_BETWEEN_NUM_CARD_MARGIN,
				pFrontSprite->getPositionY() + pFrontSprite->getContentSize().height - pUpNumSprite->getContentSize().height * 0.5f - DISTIRBUTE_BETWEEN_NUM_CARD_MARGIN));
			pFrontSprite->addChild(pUpNumSprite, 0, Tag_UpNum);

			if (size == small_Card) {//小牌面花色在中间偏下
				Vec2 colorPosition = Vec2(pFrontSprite->getContentSize().width * 0.5f, pFrontSprite->getContentSize().height * 0.35f);
				pUpColorSprite->setPosition(colorPosition);
				pUpNumSprite->setPosition(Vec2(pFrontSprite->getContentSize().width * 0.25f, pFrontSprite->getContentSize().height * 0.8f));
			}
			else//大牌面花色在其下
			{
				pUpColorSprite->setPosition(pUpNumSprite->getPosition() + Vec2(0, -66));
			}
			pFrontSprite->addChild(pUpColorSprite, 0, Tag_UpColor);
		}

		if (size!=small_Card) {//小牌已将左上角花色移到中间
			if (pDownColorSprite)
			{
				pDownColorSprite->setPosition(Vec2(pFrontSprite->getContentSize().width * 0.64f, pFrontSprite->getContentSize().height * 0.25f));
				pFrontSprite->addChild(pDownColorSprite, 0, Tag_DownColor);
			}
		}

	}
	
	m_cardSize = pFrontSprite->getContentSize();            //牌大小
	pFrontSprite->getTexture()->setAntiAliasTexParameters();
	pBackSprite->getTexture()->setAntiAliasTexParameters();
	setContentSize(m_cardSize);
	setCascadeOpacityEnabled(true);

	if (size == normal_Card) {//中等大小牌为正常牌的 47%
		this->setScale(0.47f);
	}
	return true;
}

void CardSprite::setCardSize(card_size size)
{
	removeAllChildren();

	initWithSize(size);
}

void CardSprite::startTurnCard(float duration)
{

	Sprite *pFrontSprite = (Sprite*)getChildByTag(Tag_FrontCard);
	Sprite *pBackSprite = (Sprite*)getChildByTag(Tag_BackCard);
	if (duration == 0)
	{
		pFrontSprite->setVisible(true);
		pFrontSprite->setRotation(0);
		pBackSprite->setVisible(false);
	}
	else
	{
		ActionInterval *pFrontAction = (ActionInterval*)Sequence::create(
			DelayTime::create(duration / 2),
			Show::create(),  
			OrbitCamera::create(duration / 2, 1, 0, 270, 90, 0, 0),
			nullptr); 

		ActionInterval *pBackAction = (ActionInterval*)Sequence::create(
			OrbitCamera::create(duration / 2, 1, 0, 0, 90, 0, 0),
			Hide::create(),  
			DelayTime::create(duration / 2),
			nullptr); 

		pBackSprite->runAction(pBackAction);
		pFrontSprite->runAction(pFrontAction);
	}
}

bool CardSprite::isContainsTouch(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	if (pTouch)
	{
		//打出去的牌不响应触摸事件
		if (card_distribute == m_nCardState)
		{
			return false;
		}

		Vec2 point = pTouch->getLocation();
		//转化成节点坐标系
		Vec2 nodePoint = convertToNodeSpace(point);
		Rect rect = getBoundingBox();
		//由于锚点设置在0.5,0.5位置，获取到的原点位于贴图中心，需要延升到其左下角
		//如果锚点设置在0,0位置时，CCOrbitCamera动作无法正常旋转
		Sprite *pFrontSprite = (Sprite*)getChildByTag(Tag_FrontCard);
		if (pFrontSprite)
		{
			rect.origin = Vec2(-pFrontSprite->getContentSize().width * 0.5f, -pFrontSprite->getContentSize().height * 0.5f);
		}
		else
		{
			rect.origin = Vec2::ZERO;
		}
		//除了末尾的牌以外，其他牌的触摸区域只有左侧显示出来的部分
		if (!m_bIsLastCard)
		{
			rect.size.width = DISTRIBUTE_CARD_MARGIN; 
		}
		
		if (rect.containsPoint(nodePoint))
		{
			return true;
		}
	}
	
	return false;
}

card_state CardSprite::getCardState()
{
	return m_nCardState;
}

void CardSprite::setCardState(card_state state)
{
	ActionInterval *moveBy = MoveBy::create(0.02f, Vec2(0, DISTRIBUTE_CARD_SELECTED_DISTANCE));
	switch (state)
	{
	//选中牌
	case card_selected:
		{
			if ( card_normal == m_nCardState || card_nomal_selecting == m_nCardState )
			{
				/*if (this->getPositionY() == DISTRIBUTE_CARD_BOTTOM_MARGIN + this->getContentSize().height * 0.5f)
				{*/
					//手中的牌往上移动一段距离
					runAction(moveBy);
				//}
				m_nCardState = card_selected;
				log("pcardsprite tag(%d), state(%d)", getTag(), m_nCardState);
			}
		}
		break;
	//取消选中，正常状态
	case card_normal:
		{
			if ( card_selected == m_nCardState || card_popup_selecting == m_nCardState )
			{
				/*if (this->getPositionY() == DISTRIBUTE_CARD_BOTTOM_MARGIN + this->getContentSize().height * 0.5f + DISTRIBUTE_CARD_SELECTED_DISTANCE)
				{*/
					//手中的牌往下移动一段距离
					runAction(moveBy->reverse());
				//}
				m_nCardState = card_normal;
				log("pcardsprite tag(%d), state(%d)", getTag(), m_nCardState);
			}	
		}
		break;
	//出牌
	case  card_distribute:
		{
			if (card_selected == m_nCardState || card_normal == m_nCardState)
			{
				m_nCardState = card_distribute;
				log("pcardsprite tag(%d), state(%d)", getTag(), m_nCardState);
			}
		}
		break;
	case card_selecting:
		{
			if (m_nCardState == card_selected )
			{
				setCardColor(Color3B::GRAY);
				m_nCardState = card_popup_selecting;
			}
			else if (m_nCardState == card_normal)
			{
				setCardColor(Color3B::GRAY);
				m_nCardState = card_nomal_selecting;
			}
		}
		break;
	}
	
}

Size CardSprite::getCardSize()
{
    return m_cardSize;
}

void CardSprite::setCardColor(const Color3B& color3)
{
	Sprite *pFrontSprite = (Sprite*)getChildByTag(Tag_FrontCard);
    pFrontSprite->setCascadeColorEnabled(true);
	pFrontSprite->setColor(color3);
}

void CardSprite::addLordSign()
{
    removeLordSign();
    const char* spriteFrameName = nullptr;
    /*switch (m_size) 
	{
	case big_Card:
		{
			spriteFrameName = "dizhu_L.png";
		}
		break;
	case normal_Card:
		{
			spriteFrameName = "dizhu_M.png";
		}
		break;
	case small_Card:
		{
			spriteFrameName = "dizhu_S.png";
		}
		break;
	default:
		break;
    }*/
	spriteFrameName = "landlord.png";
    Sprite* lordSign = Sprite::createWithSpriteFrameName(spriteFrameName);
    Sprite *pFrontSprite = (Sprite*)getChildByTag(Tag_FrontCard);
    Size frotSpriteSize = pFrontSprite->getContentSize();
    Size lordSize = lordSign->getContentSize();
    lordSign->setPosition(Vec2(frotSpriteSize.width - lordSize.width * 0.5f, frotSpriteSize.height - lordSize.height * 0.5f));
    lordSign->setTag(Tag_LordSign);
    pFrontSprite->addChild(lordSign);
}

void CardSprite::removeLordSign()
{
    Sprite *pFrontSprite = (Sprite*)getChildByTag(Tag_FrontCard);
    if (pFrontSprite) 
	{
        Node* lordSign = pFrontSprite->getChildByTag(Tag_LordSign);
        if (lordSign) 
		{
            lordSign->removeFromParentAndCleanup(true);
        }
    }
}

void CardSprite::setAnchorPoint(const Vec2 &point)
{
    __NodeRGBA::setAnchorPoint(point);
    Node* frontCard = getChildByTag(Tag_FrontCard);
    if (frontCard) 
	{
        frontCard->setAnchorPoint(point);
    }
}

void CardSprite::cancelSelecting()
{
	if (m_nCardState == card_nomal_selecting)
	{
		m_nCardState = card_normal;
		setCardColor(Color3B::WHITE);
	}else if(m_nCardState == card_popup_selecting)
	{
		m_nCardState = card_selected;
		setCardColor(Color3B::WHITE);
	}
}


cocos2d::Vector<CardSprite*> CardSprite::sortThreeTake(cocos2d::Vector<CardSprite*> pokerArr)
{
	if( pokerArr.size() < 6 ){  // 1113 或 11133
		if( pokerArr.at(0)->getPoker()->getPokerNum() != pokerArr.at(2)->getPoker()->getPokerNum() )
		{
			if( pokerArr.size() == 4 ){
				CardSprite* poker = pokerArr.at(0);
				pokerArr.replace(0, pokerArr.at(1));
				pokerArr.replace(1, pokerArr.at(2));
				pokerArr.replace(2, pokerArr.at(3));
				pokerArr.replace(3, poker);
			}else{
				CardSprite* poker = pokerArr.at(0);
				CardSprite* poker1 = pokerArr.at(1);
				pokerArr.replace(0, pokerArr.at(2));
				pokerArr.replace(1, pokerArr.at(3));
				pokerArr.replace(2, pokerArr.at(4));
				pokerArr.replace(3, poker);
				pokerArr.replace(4, poker1);
			}
		}
	}else if( pokerArr.size() < 11 ){ // 33344412 或者 3334441122
		if( pokerArr.size() == 8 ){ 
			if( pokerArr.at(0)->getPoker()->getPokerNum() != pokerArr.at(2)->getPoker()->getPokerNum() )
			{
				if( pokerArr.at(1)->getPoker()->getPokerNum() != pokerArr.at(2)->getPoker()->getPokerNum() ) //65444333
				{
					CardSprite* poker = pokerArr.at(0);
					CardSprite* poker1 = pokerArr.at(1);
					pokerArr.replace(0, pokerArr.at(2));
					pokerArr.replace(1, pokerArr.at(3));
					pokerArr.replace(2, pokerArr.at(4));
					pokerArr.replace(3, pokerArr.at(5));
					pokerArr.replace(4, pokerArr.at(6));
					pokerArr.replace(5, pokerArr.at(7));
					pokerArr.replace(6, poker);
					pokerArr.replace(7, poker1);
				}else{ //65554443
					CardSprite* poker = pokerArr.at(0);
					pokerArr.replace(0, pokerArr.at(1));
					pokerArr.replace(1, pokerArr.at(2));
					pokerArr.replace(2, pokerArr.at(3));
					pokerArr.replace(3, pokerArr.at(4));
					pokerArr.replace(4, pokerArr.at(5));
					pokerArr.replace(5, pokerArr.at(6));
					pokerArr.replace(6, poker);
				}
			}
		}else{ //3334441122
			if( pokerArr.at(0)->getPoker()->getPokerNum() != pokerArr.at(2)->getPoker()->getPokerNum() )
			{
				if( pokerArr.at(2)->getPoker()->getPokerNum() != pokerArr.at(4)->getPoker()->getPokerNum() ) //6655444333
				{
					CardSprite* poker = pokerArr.at(0);
					CardSprite* poker1 = pokerArr.at(1);
					CardSprite* poker2 = pokerArr.at(2);
					CardSprite* poker3 = pokerArr.at(3);
					pokerArr.replace(0, pokerArr.at(4));
					pokerArr.replace(1, pokerArr.at(5));
					pokerArr.replace(2, pokerArr.at(6));
					pokerArr.replace(3, pokerArr.at(7));
					pokerArr.replace(4, pokerArr.at(8));
					pokerArr.replace(5, pokerArr.at(9));
					pokerArr.replace(6, poker);
					pokerArr.replace(7, poker1);
					pokerArr.replace(8, poker2);
					pokerArr.replace(9, poker3);
				}else{ //6655544433
					CardSprite* poker = pokerArr.at(0);
					CardSprite* poker1 = pokerArr.at(1);
					pokerArr.replace(0, pokerArr.at(2));
					pokerArr.replace(1, pokerArr.at(3));
					pokerArr.replace(2, pokerArr.at(4));
					pokerArr.replace(3, pokerArr.at(5));
					pokerArr.replace(4, pokerArr.at(6));
					pokerArr.replace(5, pokerArr.at(7));
					pokerArr.replace(6, poker);
					pokerArr.replace(7, poker1);
				}
			}
		}
	}else{ // 333444555123 或者 333444555112233
		if( pokerArr.size() == 12 ){ 
			if( pokerArr.at(0)->getPoker()->getPokerNum() != pokerArr.at(2)->getPoker()->getPokerNum() )
			{
				if( pokerArr.at(1)->getPoker()->getPokerNum() == pokerArr.at(2)->getPoker()->getPokerNum() ) // 6 555444333 21
				{
					CardSprite* poker = pokerArr.at(0);
					pokerArr.replace(0, pokerArr.at(1));
					pokerArr.replace(1, pokerArr.at(2));
					pokerArr.replace(2, pokerArr.at(3));
					pokerArr.replace(3, pokerArr.at(4));
					pokerArr.replace(4, pokerArr.at(5));
					pokerArr.replace(5, pokerArr.at(6));
					pokerArr.replace(6, pokerArr.at(7));
					pokerArr.replace(7, pokerArr.at(8));
					pokerArr.replace(8, pokerArr.at(9));
					pokerArr.replace(9, poker);
				}else if( pokerArr.at(2)->getPoker()->getPokerNum() == pokerArr.at(3)->getPoker()->getPokerNum() ){ // 76 555444333 2
					CardSprite* poker = pokerArr.at(0);
					CardSprite* poker1 = pokerArr.at(1);
					pokerArr.replace(0, pokerArr.at(2));
					pokerArr.replace(1, pokerArr.at(3));
					pokerArr.replace(2, pokerArr.at(4));
					pokerArr.replace(3, pokerArr.at(5));
					pokerArr.replace(4, pokerArr.at(6));
					pokerArr.replace(5, pokerArr.at(7));
					pokerArr.replace(6, pokerArr.at(8));
					pokerArr.replace(7, pokerArr.at(9));
					pokerArr.replace(8, pokerArr.at(10));
					pokerArr.replace(9, poker);
					pokerArr.replace(10, poker1);
				}else{ // 876 555444333
					CardSprite* poker = pokerArr.at(0);
					CardSprite* poker1 = pokerArr.at(1);
					CardSprite* poker2 = pokerArr.at(2);
					pokerArr.replace(0, pokerArr.at(3));
					pokerArr.replace(1, pokerArr.at(4));
					pokerArr.replace(2, pokerArr.at(5));
					pokerArr.replace(3, pokerArr.at(6));
					pokerArr.replace(4, pokerArr.at(7));
					pokerArr.replace(5, pokerArr.at(8));
					pokerArr.replace(6, pokerArr.at(9));
					pokerArr.replace(7, pokerArr.at(10));
					pokerArr.replace(8, pokerArr.at(11));
					pokerArr.replace(9, poker);
					pokerArr.replace(10, poker1);
					pokerArr.replace(11, poker1);
				}
			}
		}else{ //333444555112233
			if( pokerArr.at(0)->getPoker()->getPokerNum() != pokerArr.at(2)->getPoker()->getPokerNum() )
			{
				if( pokerArr.at(2)->getPoker()->getPokerNum() == pokerArr.at(4)->getPoker()->getPokerNum() ) // 66 555444333 2211
				{
					CardSprite* poker = pokerArr.at(0);
					CardSprite* poker1 = pokerArr.at(1);
					pokerArr.replace(0, pokerArr.at(2));
					pokerArr.replace(1, pokerArr.at(3));
					pokerArr.replace(2, pokerArr.at(4));
					pokerArr.replace(3, pokerArr.at(5));
					pokerArr.replace(4, pokerArr.at(6));
					pokerArr.replace(5, pokerArr.at(7));
					pokerArr.replace(6, pokerArr.at(8));
					pokerArr.replace(7, pokerArr.at(9));
					pokerArr.replace(8, pokerArr.at(10));
					pokerArr.replace(9, poker);
					pokerArr.replace(10, poker1);
				}else if( pokerArr.at(4)->getPoker()->getPokerNum() == pokerArr.at(6)->getPoker()->getPokerNum() ){ // 7766 555444333 22
					CardSprite* poker = pokerArr.at(0);
					CardSprite* poker1 = pokerArr.at(1);
					CardSprite* poker2 = pokerArr.at(2);
					CardSprite* poker3 = pokerArr.at(3);
					pokerArr.replace(0, pokerArr.at(4));
					pokerArr.replace(1, pokerArr.at(5));
					pokerArr.replace(2, pokerArr.at(6));
					pokerArr.replace(3, pokerArr.at(7));
					pokerArr.replace(4, pokerArr.at(8));
					pokerArr.replace(5, pokerArr.at(9));
					pokerArr.replace(6, pokerArr.at(10));
					pokerArr.replace(7, pokerArr.at(11));
					pokerArr.replace(8, pokerArr.at(12));
					pokerArr.replace(9, poker);
					pokerArr.replace(10, poker1);
					pokerArr.replace(11, poker2);
					pokerArr.replace(12, poker3);
				}else{ // 887766 555444333
					CardSprite* poker = pokerArr.at(0);
					CardSprite* poker1 = pokerArr.at(1);
					CardSprite* poker2 = pokerArr.at(2);
					CardSprite* poker3 = pokerArr.at(3);
					CardSprite* poker4 = pokerArr.at(4);
					CardSprite* poker5 = pokerArr.at(5);
					pokerArr.replace(0, pokerArr.at(6));
					pokerArr.replace(1, pokerArr.at(7));
					pokerArr.replace(2, pokerArr.at(8));
					pokerArr.replace(3, pokerArr.at(9));
					pokerArr.replace(4, pokerArr.at(10));
					pokerArr.replace(5, pokerArr.at(11));
					pokerArr.replace(6, pokerArr.at(12));
					pokerArr.replace(7, pokerArr.at(13));
					pokerArr.replace(8, pokerArr.at(14));
					pokerArr.replace(9, poker);
					pokerArr.replace(10, poker1);
					pokerArr.replace(11, poker2);
					pokerArr.replace(12, poker3);
					pokerArr.replace(13, poker4);
					pokerArr.replace(14, poker5);
				}
			}
		}
	}
	
    return pokerArr;
}
	

cocos2d::Vector<CardSprite*> CardSprite::sortFourTake(cocos2d::Vector<CardSprite*> pokerArr)
{
	
	if( pokerArr.at(0)->getPoker()->getPokerNum() != pokerArr.at(3)->getPoker()->getPokerNum() )
	{
		if( pokerArr.size() == 6 ){  //四带两张单牌
			if( pokerArr.at(1)->getPoker()->getPokerNum() != pokerArr.at(3)->getPoker()->getPokerNum() ){ //543333
				CardSprite* poker = pokerArr.at(0);
				CardSprite* poker1 = pokerArr.at(1);
				pokerArr.replace(0, pokerArr.at(2));
				pokerArr.replace(1, pokerArr.at(3));
				pokerArr.replace(2, pokerArr.at(4));
				pokerArr.replace(3, pokerArr.at(5));
				pokerArr.replace(4, poker);
				pokerArr.replace(5, poker1);
			}else{ //544443
				CardSprite* poker = pokerArr.at(0);
				pokerArr.replace(0, pokerArr.at(1));
				pokerArr.replace(1, pokerArr.at(2));
				pokerArr.replace(2, pokerArr.at(3));
				pokerArr.replace(3, pokerArr.at(4));
				pokerArr.replace(4, poker);
			}
		}else{   //四带两对
           if( pokerArr.at(3)->getPoker()->getPokerNum() != pokerArr.at(4)->getPoker()->getPokerNum() ){ //55443333
				CardSprite* poker = pokerArr.at(0);
				CardSprite* poker1 = pokerArr.at(1);
				CardSprite* poker2 = pokerArr.at(2);
				CardSprite* poker3 = pokerArr.at(3);
				pokerArr.replace(0, pokerArr.at(4));
				pokerArr.replace(1, pokerArr.at(5));
				pokerArr.replace(2, pokerArr.at(6));
				pokerArr.replace(3, pokerArr.at(7));
				pokerArr.replace(4, poker);
				pokerArr.replace(5, poker1);
				pokerArr.replace(6, poker2);
				pokerArr.replace(7, poker3);
			}else{ //55444433
				CardSprite* poker = pokerArr.at(0);
				CardSprite* poker1 = pokerArr.at(1);
				pokerArr.replace(0, pokerArr.at(2));
				pokerArr.replace(1, pokerArr.at(3));
				pokerArr.replace(2, pokerArr.at(4));
				pokerArr.replace(3, pokerArr.at(5));
				pokerArr.replace(4, poker);
				pokerArr.replace(5, poker1);
			}
		}
	}
	
    return pokerArr;
}

/*
 * MyCardsLayer.cpp
 *
 *  Created on: 2015-5-16
 *      Author: Jacky
 */
#include "MyCardsLayer.h"
#include "cocos2d.h"
#include "WWMacros.h"
#include "View/GameScene/GameLogicManager.h"
#include "Control/WWRequest/MsgLordGameRequest.h"
#include "Control/WWRequest/MsgGameCommonRequest.h"
#include "View/Common/PokerLogic/PokerTips.h"

USING_NS_CC;

const int Card_Org_Tag = 0;

#define LANDLORD_CARD_TAG  300  //顶部工具条地主牌
#define TOP_LAIZI_CARD_TAG 350  //顶部工具条癞子牌

enum //摇癞子动画 UI
{
	rascal_box_all = 400,	//盒子完整
	rascal_box,				//盒子底
	rascal_left,			//盒子盖子左
	rascal_right,			//盒子盖子右
	rascal_light			//光
};

extern int getLaiziNumber();

MyCardsLayer::MyCardsLayer(void) :
	m_pBGBatchNode(nullptr),
	m_nFirstTouchTag(-1),
	m_nLastTouchTag(-1),
	m_nCardIndex(0),
	m_pListener(nullptr),
	m_bIsAutoPlay(false),
	m_isLord(false),
	m_bIsOutCards(false),
	b_lSubstitute(nullptr),
	b_lNoBiggerCard(nullptr),
	m_nIsMoveTouch(false)
{
	m_bIsCardClick = true;
}


MyCardsLayer::~MyCardsLayer(void)
{
	m_bIsCardClick = true;
	m_arrMyCards.clear();
	m_arrLastTipPokers.clear();
    //PokerDesk::getInstance()->release();
}

void MyCardsLayer::onEnter()
{
	Layer::onEnter();
	
	//加载 扑克资源
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(g_PokersTextureFormatPath.c_str());
		
	m_pListener = EventListenerTouchOneByOne::create();
	m_pListener->onTouchBegan = CC_CALLBACK_2(MyCardsLayer::onTouchBegan, this);
	m_pListener->onTouchMoved = CC_CALLBACK_2(MyCardsLayer::onTouchMoved, this);
	m_pListener->onTouchEnded = CC_CALLBACK_2(MyCardsLayer::onTouchEnded, this);
	m_pListener->onTouchCancelled = CC_CALLBACK_2(MyCardsLayer::onTouchCancelled, this);
	m_pListener->setEnabled(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);

	auto toForegroundListener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND, [this](EventCustom* event)
	{
		CCLOG("wly::MyCardsLayer toForegroundListener");
		onTouchEnded(nullptr, nullptr);
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(toForegroundListener, this);
}

void MyCardsLayer::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	
	Layer::onExit();

	if (m_pBGBatchNode)
	{
		m_pBGBatchNode->removeAllChildren();
	}
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(g_PokersTextureFormatPath.c_str());
}

void MyCardsLayer::reset()
{
	m_isLord = false;
	m_bIsOutCards = false;
	removeMyOutCards();
	removeMyHandCards();
	removeLandLordCards();
	
	setNoBiggerTipsShow(false);
}

void MyCardsLayer::setTouchEnabled(bool bEnable)
{
	m_pListener->setEnabled(bEnable);
}

void MyCardsLayer::setSubtitude(bool bEnable)
{
	m_bIsAutoPlay = bEnable;
	b_lSubstitute->setVisible(bEnable);

	if(bEnable){
		b_lNoBiggerCard->setVisible(false);
	}

	for (auto card : m_arrMyCards)
	{
		if (card)
		{
			if(m_bIsAutoPlay) 
			{
				card->setCardState(card_normal);
				card->setCardColor(ccColor3B::GRAY);
			}else{
				card->setCardColor(Color3B::WHITE);
			}
		}
	}
	updateMyCardsPos();
}

void MyCardsLayer::setNoBiggerTipsShow(bool isMyTurn)
{
	if(m_bIsAutoPlay){
		return;
	}

	bool isShow = false;

	if(isMyTurn) 
	{
		Vector<PokerObject*> lastOutPokers = GameLogicManager::getInstance()->getLastPlayPokers();
		if(lastOutPokers.size() > 0)
		{
			Vector<PokerObject*> vecTipPokers = PokerTips::getTipsPokers(lastOutPokers, getPokerArr(), lastOutPokers);
			if (vecTipPokers.size() <= 0)
			{
				isShow = true;
			}
		}
	}
	
	b_lNoBiggerCard->setVisible(isShow);
	
	// 设置牌是否变暗
	for (auto card : m_arrMyCards)
	{
		if (card)
		{
			if(isShow) 
			{
				card->setCardState(card_normal);
				card->setCardColor(ccColor3B::GRAY);
			}else{
				card->setCardColor(Color3B::WHITE);
			}
		}
	}
	updateMyCardsPos();

	// 设置牌是否可点击
	m_bIsCardClick = !isShow;
}

bool MyCardsLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());
		
		// 托管文字
		b_lSubstitute = Sprite::create("res/Images/GameScene/Label/cancellAiTip.png");
		CC_BREAK_IF(!b_lSubstitute);
		b_lSubstitute->setPosition(Vec2(CUR_SCREEN_SIZE.width*0.5f, 118));
		b_lSubstitute->setVisible(false);
		this->addChild(b_lSubstitute, BASIC_ZORDER_ROOT + 100);

		// 没有牌大过上家
		b_lNoBiggerCard = Sprite::create("res/Images/GameScene/Label/no_bigger_card.png");
		CC_BREAK_IF(!b_lNoBiggerCard);
		b_lNoBiggerCard->setPosition(Vec2(CUR_SCREEN_SIZE.width*0.5f, 118));
		b_lNoBiggerCard->setVisible(false);
		this->addChild(b_lNoBiggerCard, BASIC_ZORDER_ROOT + 100);

		bRet = true;

	} while (0);

	return bRet;
}

void MyCardsLayer::clear()
{
	removeChild(m_pBGBatchNode);
	m_pBGBatchNode = nullptr;
	m_nCardIndex = 0;
	m_nLastTouchTag = -1;
}

void MyCardsLayer::initPlayersCard(const Vector<PokerObject*> &vecMyPokers)
{
	if (!Node::init())
	{
		return;
	}

	if (vecMyPokers.size() > 20)
	{
		log("WWBasePlayer::init fail! arrCards is inlegal!");
		return;
	}

	//服务器传来的牌型数据初始化每张牌，poker主要为牌的内容
	//cardsprite为牌的UI
	int tag = Card_Org_Tag;
	for (auto poker : vecMyPokers)
	{
		if (poker)
		{
			CardSprite *pCardSprite = CardSprite::create(poker);
			if (pCardSprite)
			{
				//未开始发牌的时候，牌大小减半
				pCardSprite->setScale(0.5f);
				pCardSprite->setCardState(card_normal);
				pCardSprite->setPositionY( DISTRIBUTE_CARD_BOTTOM_MARGIN + pCardSprite->getContentSize().height * 0.5f);
				addChild(pCardSprite, 0, tag);
				m_arrMyCards.pushBack(pCardSprite);
			}
		}
		tag ++;
	}
	
	//从大到小排序
	sortArrBtoS(m_arrMyCards);
	setCardsZOrder(m_arrMyCards);

	updateMyCardsPos();

	////初始化牌堆
	//initBackCards();

	////开启定时器发牌,每次执行发给一个玩家
	//schedule(schedule_selector(MyCardsLayer::startDistributeCards), DISTRIBUTE_CARD_RATE, DISTRIBUTE_CARD_COUNT, 0.5f);
}

void MyCardsLayer::initRecoveryPlayersCard(const cocos2d::Vector<PokerObject*> &vecMyPokers)
{
	if (!Node::init())
	{
		return;
	}

	if (vecMyPokers.size() > 20)
	{
		log("WWBasePlayer::init fail! arrCards is inlegal!");
		return;
	}

	//服务器传来的牌型数据初始化每张牌，poker主要为牌的内容
	//cardsprite为牌的UI
	int tag = Card_Org_Tag;
	for (auto poker : vecMyPokers)
	{
		if (poker)
		{
			CardSprite *pCardSprite = CardSprite::create(poker);
			if (pCardSprite)
			{
				//未开始发牌的时候，牌大小减半
				pCardSprite->setScale(0.5f);
				pCardSprite->setPositionY( DISTRIBUTE_CARD_BOTTOM_MARGIN + pCardSprite->getContentSize().height * 0.5f);
				addChild(pCardSprite, 0, tag);
				m_arrMyCards.pushBack(pCardSprite);
			}
		}
		tag ++;
	}

	//如果是断线重连进来，则设置一下手中牌的位置以及大小
	for (auto pCard : m_arrMyCards)
	{
		//初始是0.5f，这里还原
		pCard->setScale(1.0f);
		pCard->startTurnCard(0.0f);
		pCard->setPositionY( DISTRIBUTE_CARD_BOTTOM_MARGIN + pCard->getContentSize().height * 0.5f);
	}
	
	//从大到小排序
	sortArrBtoS(m_arrMyCards);
	setCardsZOrder(m_arrMyCards);

	updateMyCardsPos();
}


void MyCardsLayer::runAddCardAction(Ref *pObj)
{
	//创建一张牌并加到自己手中显示
	PokerObject *poker = (PokerObject*)pObj;
	if (poker)
	{
		CardSprite *pCard = CardSprite::create(poker);
		if (pCard)
		{
			pCard->startTurnCard(0.2f);
			pCard->setPositionY(DISTRIBUTE_CARD_BOTTOM_MARGIN + pCard->getContentSize().height * 0.5f);
			if(m_bIsAutoPlay) 
			{
				pCard->setCardColor(ccColor3B::GRAY);
			}else{
				pCard->setCardColor(Color3B::WHITE);
			}
			//加到层中显示
			addChild(pCard, 0, m_arrMyCards.size() - 1);
			m_arrMyCards.pushBack(pCard);
		}
	}
}

void MyCardsLayer::removeMyOutCards()
{
	for (auto card : m_arrPlayedCards)
	{
		if (card)
		{
			m_arrMyCards.eraseObject(card);
			this->removeChild(card);
		}
	}

	m_arrPlayedCards.clear();
  
	//清除上次提示
	m_arrLastTipPokers.clear();
}

void MyCardsLayer::removeMyHandCards()
{
	for(auto card: m_arrMyCards)
	{
		if(card)
		{
			this->removeChild(card);
		}
	}
	m_arrMyCards.clear();
}

void MyCardsLayer::removeLandLordCards()
{
	//地主牌
	for(int index = 0; index < 3; index++)
	{
		if(this->getChildByTag( LANDLORD_CARD_TAG + index )){
			this->removeChildByTag( LANDLORD_CARD_TAG + index );
		}
	}

	//癞子牌
	if(this->getChildByTag(TOP_LAIZI_CARD_TAG)){
		this->removeChildByTag(TOP_LAIZI_CARD_TAG);
	}
}

void MyCardsLayer::showLandLordCard()
{
	if(m_pBGBatchNode){
		m_pBGBatchNode->removeAllChildren();
		this->removeChild( m_pBGBatchNode );
		m_pBGBatchNode = nullptr;
	}
	initBackCards();
	
	Vector<Node*> vecAllBackCard = m_pBGBatchNode->getChildren();

	if (vecAllBackCard.size() != 3)
	{
		return;
	}

	for(unsigned int i = 0; i < vecAllBackCard.size(); i++)
	{
		Node* pNode = m_pBGBatchNode->getChildren().at(i);
		pNode->setPosition(Vec2(CUR_SCREEN_SIZE.width* 0.5f - pNode->getContentSize().width * DISTRIBUTE_LAST_THREE_CARDS_SCALE - 70 + i * 55, CUR_SCREEN_SIZE.height - pNode->getContentSize().height * DISTRIBUTE_LAST_THREE_CARDS_SCALE* 0.5f - 18));
		pNode->setScale(DISTRIBUTE_LAST_THREE_CARDS_SCALE);
	}
}

void MyCardsLayer::showLandLordCardAnim()
{
	if(m_pBGBatchNode){
		m_pBGBatchNode->removeAllChildren();
		this->removeChild( m_pBGBatchNode );
		m_pBGBatchNode = nullptr;
	}
	initBackCards();
	startDistributeCards(0);
}

void MyCardsLayer::addCard(const cocos2d::Vector<PokerObject*> &vecPokers)
{
	CCAssert(vecPokers.size() != 0, "addCard::vecPokers size is 0");
	for (auto poker : vecPokers)
	{
		if (poker)
		{
			runAddCardAction(poker);
		}
	}

	//重新对手中的牌排序
	sortArrBtoS(m_arrMyCards);
	//排完序后设置一遍zorder
	setCardsZOrder(m_arrMyCards);

	updateMyCardsPos();
}

void MyCardsLayer::removeLaiZiAnim()
{
	for(int tagIndex = 0; tagIndex<5; tagIndex++){
		Node *node = this->getChildByTag(rascal_box_all + tagIndex);
		if (node)
		{
			node->removeFromParent();
		}
	}
}

//摇癞子动画
void MyCardsLayer::startChooseLaiZi()
{
	if( GameLogicManager::getInstance()->getCurRoomType() != RoomType::RASCAL_ROOM || getLaiziNumber() <= 0 ){
		return;
	}
	
	removeLaiZiAnim();
	if( this->getChildByTag(TOP_LAIZI_CARD_TAG) ){
		this->removeChildByTag(TOP_LAIZI_CARD_TAG);
	}

	bool ret = false;
	do{
		auto box_all = Sprite::create("res/Images/Animation/rascal_box_all.png");
		CC_BREAK_IF(!box_all);
		box_all->setPosition(Vec2(CUR_SCREEN_SIZE.width* 0.5f, CUR_SCREEN_SIZE.height + box_all->getContentSize().height *0.5f));
		box_all->setVisible(false);
		addChild(box_all,BASIC_ZORDER_ROOT,rascal_box_all);
		
		auto box_under = Sprite::create("res/Images/Animation/rascal_box.png");
		CC_BREAK_IF(!box_under);
		box_under->setVisible(false);
		box_under->setPosition(Vec2(CUR_SCREEN_SIZE.width* 0.5f, CUR_SCREEN_SIZE.height*0.5f + box_under->getContentSize().height *0.5f));
		addChild(box_under,BASIC_ZORDER_ROOT,rascal_box);
		
		//初始化一个癞子牌
		PokerObject *poker = PokerObject::create(POKERVALUE(getLaiziNumber()));
		if (poker)
		{
			CardSprite *card = CardSprite::create(poker);
			if (card)
			{
				card->startTurnCard(0);
				card->setPosition(Vec2(box_under->getPosition().x, box_under->getPosition().y + 15));
				card->setScale(0.47f);
				card->setVisible(false);
				addChild(card, BASIC_ZORDER_ROOT, TOP_LAIZI_CARD_TAG);
			}
		}

		auto box_left = Sprite::create("res/Images/Animation/rascal_left.png");
		CC_BREAK_IF(!box_left);
		box_left->setPosition(Vec2(box_under->getPosition().x - box_left->getContentSize().width * 0.5f, box_under->getPosition().y +15));
		box_left->setVisible(false);
		addChild(box_left,BASIC_ZORDER_ROOT,rascal_left);
		
		auto box_right = Sprite::create("res/Images/Animation/rascal_right.png");
		CC_BREAK_IF(!box_right);
		box_right->setPosition(Vec2(box_under->getPosition().x +  box_right->getContentSize().width * 0.5f, box_under->getPosition().y +15));
		box_right->setVisible(false);
		addChild(box_right,BASIC_ZORDER_ROOT,rascal_right);
		
		auto box_light = Sprite::create("res/Images/Animation/rascal_light.png");
		box_light->setPosition(Vec2(CUR_SCREEN_SIZE.width* 0.5f, CUR_SCREEN_SIZE.height));
		box_light->setVisible(false);
		CC_BREAK_IF(!box_light);
		addChild(box_light,BASIC_ZORDER_ROOT,rascal_light);

		ret = true;
	}while(0);

	if( !ret ){
		if( !this->getChildByTag(TOP_LAIZI_CARD_TAG) && getLaiziNumber() > 0 )
		{
			//初始化一个癞子牌
			PokerObject *poker = PokerObject::create(POKERVALUE(getLaiziNumber()));
			if (poker)
			{
				CardSprite *card = CardSprite::create(poker);
				if (card)
				{
					card->startTurnCard(0);
					card->setPosition(Vec2(CUR_SCREEN_SIZE.width* 0.5f, CUR_SCREEN_SIZE.height*0.5f + 142));
					card->setScale(0.47f);
					card->setVisible(false);
					addChild(card, BASIC_ZORDER_ROOT, TOP_LAIZI_CARD_TAG);
				}
			}
		}
		onChooseLaiZiEnd();
	}else{
		Node* box_all = this->getChildByTag(rascal_box_all);
		if(box_all)
		{
			box_all->setVisible(true);
			MoveTo *pMoveTo = MoveTo::create(0.3f, Vec2(CUR_SCREEN_SIZE.width* 0.5f, CUR_SCREEN_SIZE.height*0.5f + 146));
			RotateTo *pRotateTo1 = RotateTo::create(0.1f, -20);
			RotateTo *pRotateTo2 = RotateTo::create(0.1f, 40);
			RotateTo *pRotateTo3 = RotateTo::create(0.1f, -40);
			RotateTo *pRotateTo4 = RotateTo::create(0.1f, 0);

			box_all->runAction(Sequence::create(pMoveTo, Spawn::create(DelayTime::create(0.1f), pRotateTo1, nullptr), Spawn::create(DelayTime::create(0.1f), pRotateTo2, nullptr), 
				Spawn::create(DelayTime::create(0.1f), pRotateTo3, nullptr), Spawn::create(DelayTime::create(0.1f), pRotateTo4, nullptr), CallFuncN::create(CC_CALLBACK_1(MyCardsLayer::onBoxMoveAndShakeEnd, this)), nullptr));
		}
	}
}

void MyCardsLayer::onBoxMoveAndShakeEnd(Node *pNode)
{
	Node* box_all = this->getChildByTag(rascal_box_all);
	if(box_all)
	{
		box_all->setVisible(false);
	}
	
	Node* box_under = this->getChildByTag(rascal_box);
	Node* card = this->getChildByTag(TOP_LAIZI_CARD_TAG);
	Node* box_left = this->getChildByTag(rascal_left);
	Node* box_right = this->getChildByTag(rascal_right);
	Node* box_light = this->getChildByTag(rascal_light);

	if( !box_under || !card || !box_left || !box_right || !box_light ){
		onChooseLaiZiEnd();
	}else{
		box_under->setVisible(true);
		card->setVisible(true);
		box_left->setVisible(true);
		box_right->setVisible(true);
		box_light->setVisible(true);
		
		MoveTo *pMoveTo = MoveTo::create(0.3f, Vec2(CUR_SCREEN_SIZE.width* 0.5f, 100));

		box_light->runAction(Sequence::create(EaseSineOut::create(pMoveTo), CallFuncN::create(CC_CALLBACK_1(MyCardsLayer::onLightMoveEnd, this)), nullptr));
	}
}

void MyCardsLayer::onLightMoveEnd(Node *pNode)
{
	Node* box_light = this->getChildByTag(rascal_light);
	if(box_light)
	{
		box_light->runAction(FadeOut::create(0.2f));
	}
	
	Node* box_left = this->getChildByTag(rascal_left);
	Node* box_right = this->getChildByTag(rascal_right);
	
	if( !box_left || !box_right ){
		onChooseLaiZiEnd();
	}else{
		RotateTo *pLeftRotateTo = RotateTo::create(0.2f, -30);
		MoveTo *pLeftMoveTo = MoveTo::create(1.0f, Vec2(box_left->getPosition().x - 250, box_left->getPosition().y - 250));
		box_left->runAction(Spawn::createWithTwoActions(pLeftMoveTo, Sequence::create(pLeftRotateTo, FadeOut::create(0.2f), nullptr)));

		RotateTo *pRightRotateTo = RotateTo::create(0.2f, 30);
		MoveTo *pRightMoveTo = MoveTo::create(1.0f, Vec2(box_right->getPosition().x + 250, box_right->getPosition().y - 250));
		box_right->runAction(Spawn::createWithTwoActions(pRightMoveTo, Sequence::create(pRightRotateTo, FadeOut::create(0.2f), CallFuncN::create(CC_CALLBACK_1(MyCardsLayer::onChooseLaiZiEnd, this)), nullptr)));
	}
}

void MyCardsLayer::onChooseLaiZiEnd(Node *pNode)
{
	removeLaiZiAnim();

	//把癞子牌移到顶部菜单栏
	Node *card = this->getChildByTag(TOP_LAIZI_CARD_TAG);
	if (card)
	{
		card->setVisible(true);
		//牌飞到顶部栏
		MoveTo *pMoveTo = MoveTo::create(1.0f, Vec2(CUR_SCREEN_SIZE.width * 0.5f, CUR_SCREEN_SIZE.height - card->getContentSize().height * DISTRIBUTE_LAST_THREE_CARDS_SCALE * 0.5f - 18));
		ScaleTo *pScale1 = ScaleTo::create(0.1f, 1.2f);
		ScaleTo *pScale2 = ScaleTo::create(0.9f, DISTRIBUTE_LAST_THREE_CARDS_SCALE);
		card->runAction(Spawn::createWithTwoActions(EaseOut::create(pMoveTo, 1.0f), 
			Sequence::create(pScale1, pScale2, CallFuncN::create(CC_CALLBACK_1(MyCardsLayer::onLaiZiFlyToTop, this)), nullptr)));
	}

	//变癞子牌
	updateMyLaiZiCards(getLaiziNumber());

	//继续牌局 UI 
	GameLogicManager::getInstance()->onLaiZiAnimEnd();
}

void MyCardsLayer::onLaiZiFlyToTop(Node *pNode)
{
	CardSprite *card = dynamic_cast<CardSprite*>(pNode);
	if (card)
	{
		card->setCardSize(small_Card);
		card->setScale(1.0f);
		card->startTurnCard(0);
	}
}

void MyCardsLayer::addLaiziCardToTop()
{
	PokerObject *poker = PokerObject::create(POKERVALUE(getLaiziNumber()));
	if (poker)
	{
		CardSprite *card = CardSprite::create(poker, small_Card);
		if (card)
		{
			card->startTurnCard(0);
			//card->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5f + 50, CUR_SCREEN_SIZE.height - card->getContentSize().height *0.5f - 16.5));
			card->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5f, CUR_SCREEN_SIZE.height - card->getContentSize().height * DISTRIBUTE_LAST_THREE_CARDS_SCALE * 0.5f - 18));
			addChild(card, 0, TOP_LAIZI_CARD_TAG);
		}
	}
}

void MyCardsLayer::onLaiziComeAndMoveDiPai()
{
	for (int index = 0; index < 3 ; index++)
	{
		Node *node = this->getChildByTag(LANDLORD_CARD_TAG + index);
		if (node)
		{
			node->runAction(MoveTo::create(0.5f, Vec2(node->getPositionX() - 20 * (index+1), node->getPositionY())));
		}
	}
}


void MyCardsLayer::updateMyLaiZiCards(int laiziNum)
{
	if( laiziNum == 0 ){
		return;
	}

	//查找自己手中的癞子牌
	for(int i = 0; i < m_arrMyCards.size(); i ++)
	{
		CardSprite *pCard = m_arrMyCards.at(i);
		if (pCard)
		{
			if (pCard->getPoker()->getPokerNum() == laiziNum)
			{
				int cardColor = pCard->getPoker()->getPokerColor();
				int tag = pCard->getTag();
				removeChild(pCard);
				m_arrMyCards.eraseObject(pCard);
				//找到后替换
				PokerObject *laiziPoker = PokerObject::create(cardColor<<4|laiziNum);
				if (laiziPoker)
				{
					CardSprite *laiziCard = CardSprite::create(laiziPoker);
					if (laiziCard)
					{
						laiziCard->startTurnCard(0);
						laiziCard->setVisible(true);
						laiziCard->setPositionY(DISTRIBUTE_CARD_BOTTOM_MARGIN + laiziCard->getContentSize().height/2);
						if (m_bIsAutoPlay)
						{
							laiziCard->setCardColor(Color3B::GRAY);
						}
						addChild(laiziCard, 0, tag);
						m_arrMyCards.insert(0, laiziCard);
					}
				}
			}
		}
	}

	//更新顶部栏的癞子牌
	for (int i = 0; i < 3; i ++)
	{
		CardSprite *card = dynamic_cast<CardSprite*>(getChildByTag(LANDLORD_CARD_TAG + i));
		if (card)
		{
			if (card->getPoker()->getPokerNum() == getLaiziNumber())
			{
				PokerObject *laiziPoker = PokerObject::create(card->getPoker()->getPokerColor()<<4|laiziNum);
				if (laiziPoker)
				{
					CardSprite *laiziCard = CardSprite::create(laiziPoker, small_Card);
					if (laiziCard)
					{
						laiziCard->startTurnCard(0);
						laiziCard->setVisible(true);
						laiziCard->setPosition(card->getPosition());
						int zorder = card->getLocalZOrder();
						removeChild(card);
						addChild(laiziCard, zorder, LANDLORD_CARD_TAG + i);
					}
				}
			}
		}
	}

	//移动顶部栏的牌
	onLaiziComeAndMoveDiPai();
	//重新设置一遍zorder并更新位置
	setCardsZOrder(m_arrMyCards);
	updateMyCardsPos();
}

void MyCardsLayer::changMyCardsToReplaceCards(const cocos2d::Vector<PokerObject*> &vecReplacePokers)
{
	Ref *pObj = nullptr;
	int nIndex = 0;
	int nSelectPokerSize = getSelectedPokers().size();
	for (auto poker : vecReplacePokers)
	{
		if (poker)
		{
			//找出服务器中癞子替换成的牌
			if ((poker->getPokerColor() >> 3) == 1)
			{
				for ( int i = nIndex; i < m_arrMyCards.size(); i ++)
				{
					CardSprite *myCard = m_arrMyCards.at(i);
					//找出自己手中的一张癞子牌
					if (myCard && myCard->getPoker()->getPokerNum() == getLaiziNumber())
					{
						if (nSelectPokerSize > 0 && myCard->getCardState() != card_selected)
						{
							//如果不是自己选中的那张癞子牌，则继续查找
							continue;
						}
						//使用服务器传过来的牌替换这张癞子牌
						myCard->setPoker(poker);
						myCard->setCardSize(big_Card);
						myCard->startTurnCard(0);
						myCard->setScale(1.0f);
						nIndex = i + 1;
						break;
					}
				}
			}
		}
	}
}


Vector<CardSprite*> MyCardsLayer::sortArrBtoS(Vector<CardSprite*> &vecCards)
{
	int count = vecCards.size();
	for (int i =0; i< count; i++) 
	{
		for (int j = i+1; j < count; j++) 
		{
			CardSprite* card = (CardSprite*)vecCards.at(i);
			CardSprite* card1 = (CardSprite*)vecCards.at(j);
			if (card->getPoker()->getPokerNum() < card1->getPoker()->getPokerNum()) 
			{
				vecCards.replace(i, card1);
				vecCards.replace(j, card);
			}
		}
	}
	return vecCards;
}


void MyCardsLayer::setCardsZOrder(const cocos2d::Vector<CardSprite*> &vecCards)
{
	int zorder = 0;
	for (auto card : vecCards)
	{
		if (card)
		{ 
			//先去掉角标
			card->removeLordSign();

			card->setLocalZOrder(zorder);
			card->setTag(zorder);
		}
		zorder ++;
	}
}


void MyCardsLayer::updateMyCardsPos()
{
	int nTag = 0;

	//牌堆离屏幕左右的间距
	float posX = (CUR_SCREEN_SIZE.width - (m_arrMyCards.size() - 1) * DISTRIBUTE_CARD_MARGIN) * 0.5f;
	for (auto pCardSprite : m_arrMyCards)
	{
		if (pCardSprite)
		{
			//根据每张牌之间固定的间距计算出牌的位置
			pCardSprite->setPositionX(posX + nTag * DISTRIBUTE_CARD_MARGIN);
			//防止丢帧
			pCardSprite->setOpacity(255);
			pCardSprite->setVisible(true);
			pCardSprite->setScale(1.0f);
			pCardSprite->startTurnCard(0.0f);
			if (nTag + 1 == m_arrMyCards.size())
			{
				pCardSprite->setIsLastCard(true);
			}
			else
			{
				pCardSprite->setIsLastCard(false);
			}
		}
		nTag ++;
	}

    //手中剩下的牌加角标
    if (getIsLord() && m_arrMyCards.size() > 0) 
	{
        CardSprite* lastCard = m_arrMyCards.back();
        if (lastCard) 
		{
            lastCard->addLordSign();
        }
    }
}

bool MyCardsLayer::initBackCards()
{
	m_pBGBatchNode = SpriteBatchNode::create(g_strBackBGImg);

	for (int i = 0; i < 3; i ++)
	{
		Sprite *pCardBGSprite = Sprite::create(g_strBackBGImg);
		if (pCardBGSprite)
		{
			pCardBGSprite->setScale(0.47f);
			pCardBGSprite->setPosition(Vec2(CUR_SCREEN_SIZE.width* 0.5f, 432 + i/10*DISTRIBUTE_CARD_HEAP_MARGIN));
			m_pBGBatchNode->addChild(pCardBGSprite, 0, i);
		}
	}

	////把牌背景叠加放到屏幕中间
	//for (int i = 0; i < DISTRIBUTE_CARD_MAX_NUM; i ++)
	//{
	//	Sprite *pCardBGSprite = Sprite::create(g_strBackBGImg);
	//	if (pCardBGSprite)
	//	{
	//		pCardBGSprite->setScale(0.5f);
	//		pCardBGSprite->setPosition(Vec2(CUR_SCREEN_SIZE.width* 0.5f, 432 + i/10*DISTRIBUTE_CARD_HEAP_MARGIN));
	//		m_pBGBatchNode->addChild(pCardBGSprite, 0, i);
	//	}
	//}

	addChild(m_pBGBatchNode,BASIC_ZORDER_TOP);
	return true;
}

void MyCardsLayer::startDistributeCards(float dt)
{
	log("MyCardsLayer::startDistributeCards run count = %d", m_nCardIndex);

	Vector<Node*> vecAllBackCard = m_pBGBatchNode->getChildren();

	//最后三张底牌处理方式
	if (vecAllBackCard.size() == 3)
	{
		distributeLastThreeCards(vecAllBackCard);
		return;
	}

	//Sprite *pCardBGSprite = (Sprite*)vecAllBackCard.back();
	//if (pCardBGSprite)
	//{
	//	if (m_nCardIndex % 3 == 0)
	//	{
	//		//发给自己
	//		m_pMyPlayer->runDistributeCardAction(pCardBGSprite->getPosition(), m_nCardIndex / 3);
	//		CCLOG("MyCardsLayer::startDistributeCards, m_pMyPlayer, m_nCardIndex = %d", m_nCardIndex);
	//	}
	//	else if ((m_nCardIndex - 1) % 3  == 0)
	//	{
	//		//发给左玩家
	//		m_pLeftPlayer->runDistributeCardAction(pCardBGSprite->getPosition(), m_nCardIndex / 3);
	//	}
	//	else
	//	{
	//		//发给右玩家
	//		m_pRightPlayer->runDistributeCardAction(pCardBGSprite->getPosition(), m_nCardIndex / 3);
	//	}
	//}
	//m_pBGBatchNode->removeChild(pCardBGSprite, true);
	//m_nCardIndex ++;
}

void MyCardsLayer::distributeLastThreeCards(const Vector<Node*> &vecAllBackCard)
{
	for(unsigned int i = 0; i < vecAllBackCard.size(); i ++)
	{
        //拉伸顶部菜单背景
        //GameManager::getInstance()->getTopMenuLayer()->topMenuStretch();
        
		Node* pNode = m_pBGBatchNode->getChildren().at(i);
        
		MoveTo* _moveTo = MoveTo::create(0.5f, Vec2(CUR_SCREEN_SIZE.width* 0.5f - pNode->getContentSize().width * DISTRIBUTE_LAST_THREE_CARDS_SCALE - 70 + i * 55, CUR_SCREEN_SIZE.height - pNode->getContentSize().height * DISTRIBUTE_LAST_THREE_CARDS_SCALE* 0.5f - 18));
        CallFunc* finishMove = CallFunc::create(CC_CALLBACK_0(MyCardsLayer::afterFlyCards, this));
        Sequence* actions = nullptr;
        if (i == vecAllBackCard.size() - 1)
		{
            actions = Sequence::create(Spawn::createWithTwoActions(MoveTo::create(0.3f, Vec2(CUR_SCREEN_SIZE.width * 0.5f, CUR_SCREEN_SIZE.height - pNode->getContentSize().height * DISTRIBUTE_LAST_THREE_CARDS_SCALE* 0.5f)), ScaleTo::create(DISTRIBUTE_LAST_THREE_CARDS_SCALE, DISTRIBUTE_LAST_THREE_CARDS_SCALE, DISTRIBUTE_LAST_THREE_CARDS_SCALE)), _moveTo, finishMove, nullptr);
        }
        else
        {
            actions = Sequence::create(Spawn::createWithTwoActions(MoveTo::create(0.3f, Vec2(CUR_SCREEN_SIZE.width * 0.5f, CUR_SCREEN_SIZE.height - pNode->getContentSize().height * DISTRIBUTE_LAST_THREE_CARDS_SCALE * 0.5f - 15)), ScaleTo::create(DISTRIBUTE_LAST_THREE_CARDS_SCALE, DISTRIBUTE_LAST_THREE_CARDS_SCALE, DISTRIBUTE_LAST_THREE_CARDS_SCALE)), _moveTo, nullptr);
        }
        pNode->runAction(actions);
	}
}

void MyCardsLayer::afterFlyCards()
{
   /* GameManager::getInstance()->onDistributeCardsEnd();
    if (AUTO_CONTINUE) 
	{
        GameManager::getInstance()->requestSubstitute(true);
    }*/
}

void MyCardsLayer::handleLastThreeCards(const Vector<PokerObject*> &vecBottomPokers, bool isMyLord)
{
	if (vecBottomPokers.size() == 3)
	{
		//确保上局牌顶部工具条地主牌已移除
		removeLandLordCards();

		int index = LANDLORD_CARD_TAG;
		//底牌的显示
		for (auto poker : vecBottomPokers)
		{
			if (poker)
			{
				CardSprite *card = CardSprite::create(poker, small_Card);
				if (card)
				{
					//把生成的牌对象跟之前的底牌重合，然后移除底牌背景
					//card->setScale(DISTRIBUTE_LAST_THREE_CARDS_SCALE);
					Node *pNode = (Node*)m_pBGBatchNode->getChildren().at(0);
					card->setPosition(pNode->getPosition());
					card->startTurnCard(0.5f);
					//为了移除方便，加到自己层上
					addChild(card, index, index);
					m_pBGBatchNode->removeChildAtIndex(0, true);
					index++;
				}
			}
		}
        
		m_pBGBatchNode->removeAllChildren();

		if (isMyLord)
		{
			m_isLord = true;
			//底牌增加到地主手中
			addCard(vecBottomPokers);
		}
	}
}

bool MyCardsLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
 	if (pTouch)
 	{
		m_nIsMoveTouch = false;

 		//遍历每张牌，判断是否被选中
		for (auto pCardSprite : m_arrMyCards)
		{
			//单点触摸情况
			if (pCardSprite && pCardSprite->isContainsTouch(pTouch, pEvent))
			{
				if (getIsAutoPlay())
				{
					MsgGameCommonRequest::sendSubstituteRequest(1);
					return true;
				}

				if(!m_bIsCardClick){
					return true;
				}

				m_nFirstTouchTag = pCardSprite->getTag();
				m_nLastTouchTag = m_nFirstTouchTag;

				pCardSprite->setCardState(card_selecting);
				/*if (pCardSprite->getCardState() == card_normal)
				{
				pCardSprite->setCardState(card_selected);
				}
				else
				{
				pCardSprite->setCardState(card_normal);
				}*/
				return true;
			}
		}
 	}
	return false;
}

void MyCardsLayer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
 	if (pTouch)
 	{
		m_nIsMoveTouch = true;

		//遍历每张牌，判断是否被选中
		for (auto pCardSprite : m_arrMyCards)
		{
			//单点触摸情况
			if (pCardSprite && pCardSprite->isContainsTouch(pTouch, pEvent))
			{
				if (getIsAutoPlay())
				{
					MsgGameCommonRequest::sendSubstituteRequest(1);
					return;
				}
				
				if(!m_bIsCardClick){
					return;
				}

				if (m_nLastTouchTag == pCardSprite->getTag())
				{
					return;
				}
				m_nLastTouchTag = pCardSprite->getTag();
				setCardsStateOnMoved();
				/*if (pCardSprite->getCardState() == card_normal)
				{
				pCardSprite->setCardState(card_selected);
				}
				else
				{
				pCardSprite->setCardState(card_normal);
				}*/
			}
		}
	}
}

void MyCardsLayer::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//起牌或者落牌
	for (auto pCard : m_arrMyCards)
	{
		if (pCard->getCardState() == card_nomal_selecting)
		{
			pCard->setCardColor(Color3B::WHITE);
			pCard->setCardState(card_selected);
		}
		else if (pCard->getCardState() == card_popup_selecting)
		{
			pCard->setCardColor(Color3B::WHITE);
			pCard->setCardState(card_normal);
		}
	}

	Vector<PokerObject*> lastOutPokers = GameLogicManager::getInstance()->getLastPlayPokers();
	Vector<PokerObject*> vecTipPokers;

	if( m_nIsMoveTouch && !isLastSelectCardUp() ){ //滑牌提示,没有轮到自己出牌或者没有出牌记录则滑选有单顺则提示，否则从所选牌中找可以提示的牌
		if(kSiteSelf != GameLogicManager::getInstance()->getCurOutCardUser() || lastOutPokers.size() <= 0 ){
			vecTipPokers = PokerTips::getMoveTipsPokers(lastOutPokers, getSelectedPokers());
		}else{
			vecTipPokers = PokerTips::getTipsPokers(lastOutPokers, getSelectedPokers(), lastOutPokers);
		}
	}
	// 有可以弹起的牌
	if (vecTipPokers.size() > 0)
	{
		setCardsSelectedState(vecTipPokers);
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setOutBtnEnable(true);
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setDownBtnEnable(true);
	}

	if (kSiteSelf != GameLogicManager::getInstance()->getCurOutCardUser())
	{
		//清除上次提示
		vecTipPokers.clear();
		return;
	}

	if (vecTipPokers.size() == 0)
	{
		//目前只做单张牌点击提示,上家出单张则不走点牌提示
		Vector<PokerObject*> selectedPokers = getSelectedPokers();
		lastOutPokers = GameLogicManager::getInstance()->getLastPlayPokers();
		if( lastOutPokers.size() > 1 && selectedPokers.size() == 1 && !isLastSelectCardUp()) // 点牌提示
		{
			vecTipPokers = PokerTips::getClickTipsPokers(lastOutPokers, getPokerArr(), selectedPokers);
		}

		// 有可以弹起的牌
		if (vecTipPokers.size() > 0)
		{
			setCardsSelectedState(vecTipPokers);
			GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setOutBtnEnable(true);
			GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setDownBtnEnable(true);
		}	
		else
		{
			if (!getIsAutoPlay())
			{
				// 检查出牌工具条按纽状态
				checkPlayCardBtnState();
			}
		}
	}
	//清除上次提示
	vecTipPokers.clear();
	m_arrLastTipPokers.clear();
}

void MyCardsLayer::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{

}

bool MyCardsLayer::isLastSelectCardUp()
{
	bool ret = false;

	for (auto pCard : m_arrMyCards)
	{
		if (pCard)
		{
			if (pCard->getTag() == m_nLastTouchTag )
			{
				if (pCard->getCardState() == card_normal )
				{
					ret = true;
				}
				break;
			}
		}
	}

	return ret;
}

void MyCardsLayer::setCardsStateOnMoved()
{
	int nMaxTag;
	int nMinTag;
	if (m_nFirstTouchTag > m_nLastTouchTag)
	{
		nMaxTag = m_nFirstTouchTag;
		nMinTag = m_nLastTouchTag;
	}
	else
	{
		nMaxTag = m_nLastTouchTag;
		nMinTag = m_nFirstTouchTag;
	}
	for (auto pCard : m_arrMyCards)
	{
		if (pCard)
		{
			if (pCard->getTag() <= nMaxTag && pCard->getTag() >= nMinTag)
			{
				pCard->setCardState(card_selecting);
			}else{
				pCard->cancelSelecting();
			}
		}
	}
}

void MyCardsLayer::checkOutCard(const cocos2d::Vector<PokerObject*> &vecPlayPokers)
{
	if(vecPlayPokers.size() != m_arrPlayedCards.size()){
		log("MyCardsLayer::checkOutCard out card err!");
		
		for (auto card : m_arrPlayedCards)
		{
			if (card)
			{
				card->setCardState(card_selected);
				m_arrMyCards.pushBack(card);
			}
		}
		
		sortArrBtoS(m_arrMyCards);
		setCardsZOrder(m_arrMyCards);

		//更新手中牌的位置
		updateMyCardsPos();
	}
}

//响应服务器出牌
void MyCardsLayer::playCards(const cocos2d::Vector<PokerObject*> &vecPlayPokers, int outCardType)
{
	m_arrPlayedCards.clear();

	Vector<PokerObject*> vecTmpPokers = vecPlayPokers;
	
	//从服务器传过来的牌数据中查找自己手中牌
	//vecPlayPoker中的牌有可能重复的情况（癞子换牌时）
	for(auto card : m_arrMyCards)
	{
		if (card)
		{
			for (auto poker : vecTmpPokers)
			{
				if (poker)
				{
					if (card->getPoker()->getPokerValue() == poker->getPokerValue())
					{
						card->removeLordSign();
						m_arrPlayedCards.pushBack(card);
						vecTmpPokers.eraseObject(poker);
						CCLOG("find card  in myCards, card value(%d)", poker->getPokerValue());
						break;
					}
				}
			}
		}
	}
	
	//重新对手中的牌排序 从大到小
	sortArrBtoS(m_arrPlayedCards);
	
	//三带一和四带一的牌型需要排序
	cocos2d::Vector<CardSprite*> outCardArr = m_arrPlayedCards;
	if( outCardType == T_THREE_1 || outCardType == T_THREE_2 ){
		outCardArr = CardSprite::sortThreeTake(m_arrPlayedCards);
	}else if( outCardType == T_FOUR_2 || outCardType == T_FOUR_4 ){
		outCardArr = CardSprite::sortFourTake(m_arrPlayedCards);
	}

	//重新设置出牌层级
	setCardsZOrder(outCardArr);
	
	//调用出牌动画
	GameLogicManager::getInstance()->getpUIgameScene()->showOutCard(outCardArr, outCardType, m_bIsAutoPlay, getIsLord(), CallFuncN::create(CC_CALLBACK_1(MyCardsLayer::onPlayCardsActionEnd, this)));

	for (auto card : m_arrPlayedCards)
	{
		if (card)
		{
			card->setCardState(card_distribute);
			m_arrMyCards.eraseObject(card);
		}
	}

	////记录出牌以便从屏幕移出，不能清除,目前不效验
	//m_arrPlayedCards.clear();

	//更新手中牌的位置
	updateMyCardsPos();
}

void MyCardsLayer::requestPlayCards(const cocos2d::Vector<PokerObject*> &vecPlayPokers, const cocos2d::Vector<PokerObject*> &vecReplacePokers)
{
	if (RoomType::RASCAL_ROOM == GameLogicManager::getInstance()->getCurRoomType())     //癞子玩法
	{
		//原牌字符串
		kPokerUint_8 szPokerData = PokerUtil::getPokersFromArr(vecPlayPokers);
		kPokerUint_8 szReplacePokerData = PokerUtil::getPokersFromArr(vecPlayPokers);
		if(vecReplacePokers.size()>0){
			//替换后的牌字符串
			szReplacePokerData = PokerUtil::getPokersFromArr(vecReplacePokers);
		}
		PokerUtil::changeDataToSever(szPokerData);
		PokerUtil::changeDataToSever(szReplacePokerData);
		kWeightValue replaceWeight = PokerTypeCheck::getPokerType(PokerUtil::getPokerArrFromPokers(szReplacePokerData));
		MsgLordGameRequest::sendTurnOutPoker((char*)szPokerData.data, (char*)szReplacePokerData.data, replaceWeight.type, PokerTypeCheck::pokersNumberToService(replaceWeight));
		delePokerData(szPokerData);
		delePokerData(szReplacePokerData);
	}
	else
	{
		//把poker转换成字符流返回给服务器
		kPokerUint_8 szPokerData = PokerUtil::getPokersFromArr(vecPlayPokers);
		PokerUtil::changeDataToSever(szPokerData);
		log("MyCardsLayer::onOutCardBtnClick pokerData(%s)", szPokerData.data);
		MsgLordGameRequest::sendTurnOutPoker((char*)szPokerData.data);
		delePokerData(szPokerData);
	}
}

//响应出牌按纽
void MyCardsLayer::onOutCardBtnClick()
{
	m_arrPlayedCards.clear();

	Vector<PokerObject*> vecPlayPokers = getSelectedPokers();
	CCAssert(vecPlayPokers.size() > 0, "select pokers is nullptr!");
	
	if(vecPlayPokers.size()<=0){
		return;
	}
	
	if (RoomType::RASCAL_ROOM == GameLogicManager::getInstance()->getCurRoomType())     //癞子玩法
	{
		std::vector<Vector<PokerObject*>> vecAllPokersVec = PokerTips::getAllPokersCanPlay(GameLogicManager::getInstance()->getLastPlayPokers(), vecPlayPokers);

		//返回空的时候，直接使用原牌
		if (vecAllPokersVec.size() > 0)
		{
			//只有一个癞子牌型的时候，直接打出。多个癞子牌型的时候，弹出牌型选择框
			if (vecAllPokersVec.size() == 1)
			{
				requestPlayCards(vecPlayPokers, vecAllPokersVec.at(0));
			}
			else
			{
				GameLogicManager::getInstance()->getpUIgameScene()->ChooseLaiziCardsToPlay(vecAllPokersVec);
			}
		}
		else
		{
			requestPlayCards(vecPlayPokers, vecPlayPokers);
		}
	}
	else
	{
		Vector<PokerObject*> vecTmpPokers;
		requestPlayCards(vecPlayPokers, vecTmpPokers);
	}
}

void MyCardsLayer::showPlayCardsResult(const cocos2d::Vector<PokerObject*> &vecPlayPokers, int outCardType)
{
	Vector<PokerObject*> vecTmpPokers;
	showPlayCardsResult(vecPlayPokers, vecTmpPokers, outCardType);
}

void MyCardsLayer::showPlayCardsResult(const cocos2d::Vector<PokerObject*> &vecPlayPokers, const cocos2d::Vector<PokerObject*> &vecReplacePokers, int outCardType)
{
	//比对出牌
	//checkOutCard(vecPlayPokers);

	if (vecPlayPokers.size() > 0)
	{
		Vector<PokerObject*> vecTmpPokers = vecPlayPokers;

		if (GameLogicManager::getInstance()->getCurRoomType() == RoomType::RASCAL_ROOM)
		{
			if (vecReplacePokers.size() > 0)
			{
				//把自己手中的癞子牌转换成服务器返回来的替换后的牌
				changMyCardsToReplaceCards(vecReplacePokers);
				vecTmpPokers = vecReplacePokers;
			}
		}

		//打牌
		playCards(vecTmpPokers, outCardType);
	}
}

// "放下"按纽响应
void MyCardsLayer::putCardsDown()
{
	for (auto card : m_arrMyCards)
	{
		if (card)
		{
			//找出手中被选中的牌
			if (card_selected == card->getCardState() || card_popup_selecting == card->getCardState() )
			{
				card->setCardState(card_normal);
			}
		}
	}

	updateMyCardsPos();

	GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setOutBtnEnable(false);
	GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setDownBtnEnable(false);
	
	//清除上次提示
	m_arrLastTipPokers.clear();
}

//"提示" 按钮响应事件
void MyCardsLayer::onTipBtnClick()
{
	//如果上家有人出牌，则需要判断当前选择的牌是否大过上家牌
	//如果上家没有人出牌，则出一张最小的牌
	Vector<PokerObject*> paramPokers = m_arrLastTipPokers.size()>0 ? m_arrLastTipPokers : GameLogicManager::getInstance()->getLastPlayPokers();

	Vector<PokerObject*> vecTipPokers = PokerTips::getTipsPokers(paramPokers, getPokerArr(), GameLogicManager::getInstance()->getLastPlayPokers());

	if (vecTipPokers.size() > 0)
	{
		if(m_arrLastTipPokers.size()>0 && PokerTypeCheck::chekPokersEqual(vecTipPokers, m_arrLastTipPokers)){
			//重新开始提示
			m_arrLastTipPokers.clear();
			onTipBtnClick();
		}else{
			//使当前手中的牌置为选中状态
			setCardsSelectedState(vecTipPokers);
			m_arrLastTipPokers = vecTipPokers;
			GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setOutBtnEnable(true);
			GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setDownBtnEnable(true);
		}
	}
	else
	{
		//判断是否重新开始提示
		if( m_arrLastTipPokers.size() > 0 ){
			//重新开始提示
			m_arrLastTipPokers.clear();
			onTipBtnClick();
		}else{
			//不出
			m_arrLastTipPokers.clear();
			GameLogicManager::getInstance()->requestPass();
		}
	}
}

void MyCardsLayer::onMinPaiBtnClick()
{
	GameLogicManager::getInstance()->requestMinPai(getPokerArr());
}

void MyCardsLayer::checkPlayCardBtnState()
{
	//如果上家有人出牌，则需要判断当前选择的牌是否大过上家牌
	auto lastOutCard = GameLogicManager::getInstance()->getLastPlayPokers();
	//判断不出按钮可用
	if ( lastOutCard.size() > 0 )
	{
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setPassBtnEnable(true);
	}
	else
	{
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setPassBtnEnable(false);
	}

	//如果上家都不要，则只需判断当前选择牌是否符合出牌规则
	//判断出牌按钮可用
	if (PokerTips::checkPokerAvaild(lastOutCard, getSelectedPokers()))
	{
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setOutBtnEnable(true);
	}
	else
	{
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setOutBtnEnable(false);
	}

	//判断放下按钮是否可用
	if (getSelectedPokers().size() > 0)
	{
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setDownBtnEnable(true);
	}
	else
	{
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->setDownBtnEnable(false);
	}
}

void MyCardsLayer::setCardsSelectedState(const cocos2d::Vector<PokerObject*> &vecPokers)
{
	if(vecPokers.size() <= 0){
		return;
	}

	//去掉已经弹起的牌
	Vector<PokerObject*> vecTmpPokers = vecPokers;
	bool isPopUp = false;
	for(auto card : m_arrMyCards)
	{
		if(card && (card->getCardState() == card_selected || card->getCardState() == card_popup_selecting) ){
			isPopUp = false;
			for (auto poker : vecTmpPokers)
			{
				if (poker)
				{
					if (poker->getPokerValue() == card->getPoker()->getPokerValue())
					{
						vecTmpPokers.eraseObject(poker);
						isPopUp = true;
						break;
					}
				}
			}
			if(! isPopUp){
				card->setCardState(card_normal);
			}
		}
	}

	//弹起其它剩余提示的牌
	for(auto card : m_arrMyCards)
	{
		//已经提示的牌都处理完
		if(vecTmpPokers.size() <= 0){
			break;
		}

		if (card)
		{
			for (auto poker : vecTmpPokers)
			{
				if (poker)
				{
					if (RoomType::RASCAL_ROOM == GameLogicManager::getInstance()->getCurRoomType())
					{
						if (poker->getPokerNum() == getLaiziNumber())
						{
							if (card->getPoker()->getPokerNum() == getLaiziNumber())
							{
								card->setCardState(card_selected);
								vecTmpPokers.eraseObject(poker);
								break;
							}
						}
					}
					//设置手中指定的牌的状态为选中状态
					if (poker->getPokerValue() == card->getPoker()->getPokerValue())
					{
						card->setCardState(card_selected);
						vecTmpPokers.eraseObject(poker);
						break;
					}
				}
			}
		}
	}

	updateMyCardsPos();
	
	if (!getIsAutoPlay())
	{
		// 检查出牌工具条按纽状态
		checkPlayCardBtnState();
	}
}

void MyCardsLayer::onPlayCardsActionEnd(Node *pCard)
{
	log("MyCardsLayer::onPlayCardsActionEnd");
	CardSprite *pCardSprite = dynamic_cast<CardSprite*>(pCard);
	if (pCardSprite)
	{
		if (m_isLord && pCardSprite->getIsLastCard())
		{
			pCardSprite->addLordSign();
			//PLAY_EFFECT("yao");
		}
	}
}

Vector<PokerObject*> MyCardsLayer::getPokerArr()
{
	Vector<PokerObject*> vecPokers;
	for (auto card : m_arrMyCards)
	{
		if (card)
		{
			vecPokers.pushBack(card->getPoker());
		}
	}
    
	return vecPokers;
}

Vector<PokerObject*> MyCardsLayer::getSelectedPokers()
{
	Vector<PokerObject*> vecPokers;
	for (auto card : m_arrMyCards)
	{
		if (card)
		{
			//找出手中被选中的牌
			if (card_selected == card->getCardState())
			{
				vecPokers.pushBack(card->getPoker());
			}
		}
	}

	return vecPokers;
}

Vector<PokerObject*> MyCardsLayer::getOutPokerArr()
{
	Vector<PokerObject*> vecPokers;
	for (auto card : m_arrPlayedCards)
	{
		if (card)
		{
			vecPokers.pushBack(card->getPoker());
		}
	}
    
	return vecPokers;
}



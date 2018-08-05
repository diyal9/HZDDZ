/*
 * OutCardLayer.cpp
 *
 *  Created on: 2015年5月19日
 *      Author: Jacky
 */

#include "OutCardLayer.h"
#include "AnimationRes.h"
#include "View/Common/CardSprite/CardsGroup.h"
#include "View/GameScene/GameLogicManager.h"
#include "View/Common/PokerLogic/PokerLogic.h"

USING_NS_CC;

static const int TAG_BG = 1;
OutCardLayer *OutCardLayer::showOutCardSelf(const cocos2d::Vector<CardSprite*> &outCards, int outCardType, bool isAutoPlay, bool isLandLord, cocos2d::CallFunc *func)
{
	OutCardLayer *pRet = new OutCardLayer();
	if (pRet && pRet->initViewAsSelfCard(outCards, outCardType, isAutoPlay, isLandLord, func))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = 0;
		return 0;
	}
}

OutCardLayer *OutCardLayer::showOutCardOther(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func)
{
	OutCardLayer *pRet = new OutCardLayer();
	if (pRet && pRet->initViewAsOtherOutCard(seat, outCards, isLandlord, func))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = 0;
		return 0;
	}
}

OutCardLayer* OutCardLayer::showLeftCardOther(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func)
{
	OutCardLayer *pRet = new OutCardLayer();
	if (pRet && pRet->initViewAsOtherLeftCard(seat, outCards, isLandlord, func))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = 0;
		return 0;
	}
}

bool OutCardLayer::initViewAsSelfCard(const cocos2d::Vector<CardSprite*> &outCards, int outCardType, bool isAutoPlay, bool isLandLord, cocos2d::CallFunc *func)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		int totalNum = outCards.size();
		CC_BREAK_IF(totalNum<=0);

		log("weightValue.type=%d", outCardType);
		//动画结束回调
		//CallFunc *animFinishCall = CallFunc::create(CC_CALLBACK_0(OutCardLayer::onActionEnd, this));

		auto tmpCatdType = outCardType;
		if( (outCardType == T_THREE_1 && outCards.size()>= 8)  ||(outCardType == T_THREE_2 && outCards.size()>= 10) ){
			tmpCatdType = T_SEQ_1;
		}
		 switch (tmpCatdType)
		 {
		 case T_ONE:
		 case T_TWO:
		 case T_Rocket_2:
			 {
				 for(int count=0; count < totalNum; count ++){
					 auto pCardSprite = outCards.at(count);
					 if( pCardSprite == nullptr ){
						 continue;
					 }
					 if (isAutoPlay)
					 {
						 pCardSprite->setCardColor(Color3B::WHITE);
					 }

					 float duration = DISTIRBUTE_CARD_PLAY_RATE;

					 //动作序列：缩放+(移动，缩放，旋转)
					 ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
					 ActionInterval *pMoveTo = MoveTo::create(duration, Vec2((CUR_SCREEN_SIZE.width - (totalNum - 1) * DISTRIBUTE_CARD_PLAY_MARGIN) / 2 + count * DISTRIBUTE_CARD_PLAY_MARGIN, DISTRIBUTE_CARD_PLAY_CARD_POS_Y));
					 EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
					 ActionInterval *pScaleTo = ScaleTo::create(duration, 0.47f); //出牌为正常牌的 47%
					 //pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, CallFunc::create(CC_CALLBACK_0(OutCardLayer::onActionEnd, this)), nullptr));
					 pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, nullptr));
					 //为了给打出去的牌最后一张加角标
					 if (count + 1 == totalNum && isLandLord)
					 {
						 pCardSprite->setIsLastCard(true);
					 }
				 }
				bRet = true;
			 }
			 break;
		 case T_THREE:   //三条
		 case T_THREE_1: //三带一
		 case T_THREE_2: //三带二
			 {
				//动作序列：主牌 缩放+(移动，缩放，旋转)  副牌 缩放+（移动，缩放）
				 for(int index=0; index < totalNum; index ++){
					 auto pCardSprite = outCards.at(index);
					 if( pCardSprite == nullptr ){
						 continue;
					 }
					 
					if (isAutoPlay)
					{
						pCardSprite->setCardColor(Color3B::WHITE);
					}

					pCardSprite->setAnchorPoint(Vec2(0,0));
					float duration = DISTIRBUTE_CARD_PLAY_RATE;
					ActionInterval *pScaleTo = ScaleTo::create(duration, 0.47f); //出牌为正常牌的 47%
					if(index<3){ //主牌

						ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
						ActionInterval *pMoveTo = MoveTo::create(duration, Vec2((CUR_SCREEN_SIZE.width - (totalNum - 1) * DISTRIBUTE_CARD_PLAY_MARGIN) / 2, DISTRIBUTE_CARD_PLAY_CARD_POS_Y));
						EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
						if( index == 0 ){
							RotateTo *RotateTo = RotateTo::create(0.2f, -30);
							ActionInterval *pFinalMove = MoveTo::create(duration, Vec2((CUR_SCREEN_SIZE.width - (totalNum - 1) * DISTRIBUTE_CARD_PLAY_MARGIN) / 2 +11, DISTRIBUTE_CARD_PLAY_CARD_POS_Y -14));
							EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
							pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, nullptr)); //主牌第一张牌向左旋转
						}else if( index == 2 ){
							RotateTo *RotateTo = RotateTo::create(0.2f, 24);
							ActionInterval *pFinalMove = MoveTo::create(duration, Vec2((CUR_SCREEN_SIZE.width - (totalNum - 1) * DISTRIBUTE_CARD_PLAY_MARGIN) / 2 +4, DISTRIBUTE_CARD_PLAY_CARD_POS_Y +14));
							EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
							pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, nullptr)); //主牌第三张牌向右旋转
							if(isLandLord){
								pCardSprite->addLordSign(); //添加地主标识
							}
						}else{
							// 第二张牌不旋转
							pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, nullptr));
						}
					}
					else
					{ //副牌
						ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
						ActionInterval *pMoveTo = MoveTo::create(duration, Vec2((CUR_SCREEN_SIZE.width - (totalNum - 1) * DISTRIBUTE_CARD_PLAY_MARGIN) / 2 + (index+1) * DISTRIBUTE_CARD_PLAY_MARGIN, DISTRIBUTE_CARD_PLAY_CARD_POS_Y - 10));
						EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, nullptr));
						//为了给打出去的牌最后一张加角标
						if (index + 1 == totalNum && isLandLord)
						{
							pCardSprite->setIsLastCard(true);
						}
					}
				 }
				bRet = true;
			 }
			 break;
			
		 case T_BOMB:	//炸弹
		 case T_BOMB_L:
		 case T_BOMB_S:
		 case T_FOUR_2: //四带两单
		 case T_FOUR_4: //四带双双
			 {
				//动作序列：主牌 缩放+(移动，缩放，旋转)  副牌 缩放+（移动，缩放）
				for(int index=0; index < totalNum; index ++){
					auto pCardSprite = outCards.at(index);
					if( pCardSprite == nullptr ){
						continue;
					}
				
					if (isAutoPlay)
					{
						pCardSprite->setCardColor(Color3B::WHITE);
					}

					pCardSprite->setAnchorPoint(Vec2(0,0));
					float duration = DISTIRBUTE_CARD_PLAY_RATE;
					ActionInterval *pScaleTo = ScaleTo::create(duration, 0.47f); //出牌为正常牌的 47%
					if(index<4){ //主牌

						ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
						ActionInterval *pMoveTo = MoveTo::create(duration, Vec2((CUR_SCREEN_SIZE.width - (totalNum - 1) * DISTRIBUTE_CARD_PLAY_MARGIN) / 2 , DISTRIBUTE_CARD_PLAY_CARD_POS_Y));
						EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
						if( index == 0 ){
							RotateTo *RotateTo = RotateTo::create(0.2f, -25);
							ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2((CUR_SCREEN_SIZE.width - (totalNum - 1) * DISTRIBUTE_CARD_PLAY_MARGIN) / 2 -30, DISTRIBUTE_CARD_PLAY_CARD_POS_Y -22));
							EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
							pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, nullptr)); //主牌第一张牌向左旋转
						}else if( index == 1 ){
							RotateTo *RotateTo = RotateTo::create(0.2f, -12);
							ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2((CUR_SCREEN_SIZE.width - (totalNum - 1) * DISTRIBUTE_CARD_PLAY_MARGIN) / 2 -17, DISTRIBUTE_CARD_PLAY_CARD_POS_Y -10));
							EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
							pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, nullptr)); //主牌第二张牌向右旋转
						}else if( index == 3 ){
							RotateTo *RotateTo = RotateTo::create(0.2f, 16);
							ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2((CUR_SCREEN_SIZE.width - (totalNum - 1) * DISTRIBUTE_CARD_PLAY_MARGIN) / 2 +15, DISTRIBUTE_CARD_PLAY_CARD_POS_Y +5));
							EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
							pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, nullptr)); //主牌第四张牌向右旋转
							if(isLandLord){
								pCardSprite->addLordSign(); //添加地主标识
							}
						}else{
							// 第三张牌不旋转
							pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, nullptr));
						}
					}
					else
					{ //副牌
						ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
						ActionInterval *pMoveTo = MoveTo::create(duration, Vec2((CUR_SCREEN_SIZE.width - (totalNum - 1) * DISTRIBUTE_CARD_PLAY_MARGIN) / 2 + index * DISTRIBUTE_CARD_PLAY_MARGIN, DISTRIBUTE_CARD_PLAY_CARD_POS_Y - 10));
						EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, nullptr));
						//为了给打出去的牌最后一张加角标
						if (index + 1 == totalNum && isLandLord)
						{
							pCardSprite->setIsLastCard(true);
						}
					}
				}
				bRet = true;
			 }
			 break;

		 default:	//默认顺子出牌
			 {
				int count = 0;
				for (auto pCardSprite : outCards)
				{
					if (pCardSprite)
					{ 
						if (isAutoPlay)
						{
							pCardSprite->setCardColor(Color3B::WHITE);
						}

						float duration = DISTIRBUTE_CARD_PLAY_RATE;

						//动作序列：缩放+(移动，缩放，旋转)
						ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
						ActionInterval *pMoveTo = MoveTo::create(duration, Vec2((CUR_SCREEN_SIZE.width - (totalNum - 1) * DISTRIBUTE_CARD_PLAY_MARGIN) / 2 + count * DISTRIBUTE_CARD_PLAY_MARGIN, DISTRIBUTE_CARD_PLAY_CARD_POS_Y));
						EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
						ActionInterval *pScaleTo = ScaleTo::create(duration, 0.47f); //出牌为正常牌的 47%
						pCardSprite->runAction(Sequence::create(DelayTime::create(0.06*(totalNum - count -1)), pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, nullptr));
						//为了给打出去的牌最后一张加角标
						if (count + 1 == totalNum && isLandLord)
						{
							pCardSprite->setIsLastCard(true);
						}

						count ++;
					}
				}
				bRet = true;
			 }
			break;
		 }


	} while (0);
	return bRet;
}

bool OutCardLayer::initViewAsOtherOutCard(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		
		kWeightValue weight = PokerTypeCheck::getPokerType(outCards);
		int outCardType = weight.type;

		//从大到小排序
		cocos2d::Vector<PokerObject*> outCardArr = PokerUtil::sortArrayBtoS(outCards);

		if( outCardType == T_THREE_1 || outCardType == T_THREE_2 ){
			outCardArr = PokerUtil::sortThreeTake(outCardArr);
		}else if( outCardType == T_FOUR_2 || outCardType == T_FOUR_4 ){
			outCardArr = PokerUtil::sortFourTake(outCardArr);
		}

		//创建一组牌
		Vec2 startPos = Vec2(CREATE_OUT_CARD_RIGHT_POSITON_X,CREATE_OUT_CARD_RIGHT_POSITON_Y);
		if(seat == kSiteLeft){
			startPos = Vec2(CREATE_OUT_CARD_LEFT_POSITON_X,CREATE_OUT_CARD_LEFT_POSITON_Y);
		}
		auto outCardSprites = CardsGroup::createPokers(outCardArr,normal_Card, startPos, isLandlord);
		
		CC_BREAK_IF(outCardSprites.size()<=0);

		if(seat == kSiteLeft)
		{
			intLeftOutCardsAndRunAnim(outCardSprites, outCardType, isLandlord, func);
		}else{
			intRightOutCardsAndRunAnim(outCardSprites, outCardType, isLandlord, func);
		}
		bRet = true;
	} while (0);
	return bRet;
}

bool OutCardLayer::initViewAsOtherLeftCard(PlayerSite seat, const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		if(seat == kSiteLeft)
		{
			showLeftLeftCards(outCards, isLandlord, func);
		}else{
			showRightLeftCards(outCards, isLandlord, func);
		}
		bRet = true;
	} while (0);
	return bRet;
}

//左边玩家出牌
void OutCardLayer::intLeftOutCardsAndRunAnim(const cocos2d::Vector<CardSprite*> &outCards, int outCardType, bool isLandlord, cocos2d::CallFunc *func)
{
	GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getnpcLeft()->saveOutCard(outCards);

	log("weightValue.type=%d", outCardType);

	int totalNum = outCards.size();
	//动画结束回调
	CallFunc *animFinishCall = CallFunc::create(CC_CALLBACK_0(OutCardLayer::onActionEnd, this));
	 
	auto tmpCatdType = outCardType;
	if( (outCardType == T_THREE_1 && outCards.size()>= 8)  ||(outCardType == T_THREE_2 && outCards.size()>= 10) ){
		tmpCatdType = T_SEQ_1;
	}
	switch (tmpCatdType)
	 {
	 case T_THREE:   //三条
	 case T_THREE_1: //三带一
	 case T_THREE_2: //三带二
		 {
			 //动作序列：主牌 缩放+(移动，缩放，旋转)  副牌 缩放+（移动，缩放）
			for(int index=0; index < totalNum; index ++){
				auto pCardSprite = outCards.at(index);
				if( pCardSprite == nullptr ){
					continue;
				}
				
				pCardSprite->setAnchorPoint(Vec2(0,0));
				float duration = DISTIRBUTE_CARD_PLAY_RATE;
				ActionInterval *pScaleTo = ScaleTo::create(duration, 0.47f); //出牌为正常牌的 47%
				if(index<3){ //主牌

					ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
					ActionInterval *pMoveTo = MoveTo::create(duration, Vec2(OUT_CARD_LEFT_POSITON_START_X + DISTRIBUTE_CARD_PLAY_MARGIN/3, OUT_CARD_LEFT_POSITON_START_Y));
					EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
					if( index == 0 ){
						RotateTo *RotateTo = RotateTo::create(0.2f, -30);
						ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2(OUT_CARD_LEFT_POSITON_START_X + DISTRIBUTE_CARD_PLAY_MARGIN/3 +11, OUT_CARD_LEFT_POSITON_START_Y -14));
						EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, animFinishCall, nullptr)); //主牌第一张牌向左旋转
					}else if( index == 2 ){
						RotateTo *RotateTo = RotateTo::create(0.2f, 24);
						ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2(OUT_CARD_LEFT_POSITON_START_X + DISTRIBUTE_CARD_PLAY_MARGIN/3 +4, OUT_CARD_LEFT_POSITON_START_Y +14));
						EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, animFinishCall, nullptr)); //主牌第三张牌向右旋转
						if(isLandlord){
							pCardSprite->addLordSign(); //添加地主标识
						}
					}else{
						// 第二张牌不旋转
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, animFinishCall, nullptr));
					}
				}
				else
				{ //副牌
					ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
					ActionInterval *pMoveTo = MoveTo::create(duration, Vec2(OUT_CARD_LEFT_POSITON_START_X + DISTRIBUTE_CARD_PLAY_MARGIN/3 + (index+1) * DISTRIBUTE_CARD_PLAY_MARGIN, OUT_CARD_LEFT_POSITON_START_Y - 10));
					EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
					pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, animFinishCall, nullptr));
					//为了给打出去的牌最后一张加角标
					if (index + 1 == totalNum && isLandlord)
					{
						pCardSprite->addLordSign();
					}
				}
			}
		}
		 break;
		 
	 case T_BOMB:	//炸弹
	 case T_BOMB_L:
	 case T_BOMB_S:
	 case T_FOUR_2: //四带两单
	 case T_FOUR_4: //四带双双
		 {
			 //动作序列：主牌 缩放+(移动，缩放，旋转)  副牌 缩放+（移动，缩放）
			for(int index=0; index < totalNum; index ++){
				auto pCardSprite = outCards.at(index);
				if( pCardSprite == nullptr ){
					continue;
				}
				
				pCardSprite->setAnchorPoint(Vec2(0,0));
				float duration = DISTIRBUTE_CARD_PLAY_RATE;
				ActionInterval *pScaleTo = ScaleTo::create(duration, 0.47f); //出牌为正常牌的 47%
				if(index<4){ //主牌

					ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
					ActionInterval *pMoveTo = MoveTo::create(duration, Vec2(OUT_CARD_LEFT_POSITON_START_X + DISTRIBUTE_CARD_PLAY_MARGIN, OUT_CARD_LEFT_POSITON_START_Y));
					EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
					if( index == 0 ){
						RotateTo *RotateTo = RotateTo::create(0.2f, -25);
						ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2(OUT_CARD_LEFT_POSITON_START_X + DISTRIBUTE_CARD_PLAY_MARGIN -30, OUT_CARD_LEFT_POSITON_START_Y -22));
						EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, animFinishCall, nullptr)); //主牌第一张牌向左旋转
					}else if( index == 1 ){
						RotateTo *RotateTo = RotateTo::create(0.2f, -12);
						ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2(OUT_CARD_LEFT_POSITON_START_X + DISTRIBUTE_CARD_PLAY_MARGIN -17, OUT_CARD_LEFT_POSITON_START_Y -10));
						EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, animFinishCall, nullptr)); //主牌第二张牌向右旋转
					}else if( index == 3 ){
						RotateTo *RotateTo = RotateTo::create(0.2f, 16);
						ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2(OUT_CARD_LEFT_POSITON_START_X + DISTRIBUTE_CARD_PLAY_MARGIN +15, OUT_CARD_LEFT_POSITON_START_Y +5));
						EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, animFinishCall, nullptr)); //主牌第四张牌向右旋转
						if(isLandlord){
							pCardSprite->addLordSign(); //添加地主标识
						}
					}else{
						// 第三张牌不旋转
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, animFinishCall, nullptr));
					}
				}
				else
				{ //副牌
					ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
					ActionInterval *pMoveTo = MoveTo::create(duration, Vec2(OUT_CARD_LEFT_POSITON_START_X + (index + 1) * DISTRIBUTE_CARD_PLAY_MARGIN, OUT_CARD_LEFT_POSITON_START_Y - 10));
					EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
					pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, animFinishCall, nullptr));
					//为了给打出去的牌最后一张加角标
					if (index + 1 == totalNum && isLandlord)
					{
						pCardSprite->addLordSign();
					}
				}
			}
		 }
		 break;

	 default:
		 { //默认当顺子出
			 int TotalNum = outCards.size();
			 for(int index = 0; index< TotalNum; index++)
			{
				CardSprite* pCardSprite = outCards.at(index);
				if (pCardSprite)
				{ 
					pCardSprite->setAnchorPoint(Vec2(0,0));
					//动作序列：缩放+(移动，缩放)
					float duration = DISTIRBUTE_CARD_PLAY_RATE;
					ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
					ActionInterval *pMoveTo = MoveTo::create(duration, Vec2(OUT_CARD_LEFT_POSITON_START_X - DISTRIBUTE_CARD_PLAY_MARGIN/2 + index * DISTRIBUTE_CARD_PLAY_MARGIN, OUT_CARD_LEFT_POSITON_START_Y));
					EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
					ActionInterval *pScaleTo = ScaleTo::create(duration, 0.47f); //出牌为正常牌的 47%
					pCardSprite->runAction(Sequence::create(DelayTime::create(0.06*index), pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, animFinishCall, nullptr));
					//为了给打出去的牌最后一张加角标
					if (index + 1 == TotalNum && isLandlord)
					{
						pCardSprite->addLordSign();
					}
				}
			}
		 }
		 break;
	 }
}

//右边玩家出牌
void OutCardLayer::intRightOutCardsAndRunAnim(const cocos2d::Vector<CardSprite*> &outCards, int outCardType, bool isLandlord, cocos2d::CallFunc *func)
{
	//计算出牌起始座标
	float diffX = 0.0f;
	diffX = - DISTRIBUTE_OUT_CARDS_MAGIN * outCards.size();
	float startX = OUT_CARD_RIGHT_POSITON_START_X + diffX;
	
	GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getnpcRight()->saveOutCard(outCards);

	log("weightValue.type=%d", outCardType);
	
	int totalNum = outCards.size();
	//动画结束回调
	CallFunc *animFinishCall = CallFunc::create(CC_CALLBACK_0(OutCardLayer::onActionEnd, this));
	 
	auto tmpCatdType = outCardType;
	if( (outCardType == T_THREE_1 && outCards.size()>= 8)  ||(outCardType == T_THREE_2 && outCards.size()>= 10) ){
		tmpCatdType = T_SEQ_1;
	}
	switch (tmpCatdType)
	 {
	 case T_THREE:   //三条
	 case T_THREE_1: //三带一
	 case T_THREE_2: //三带二
		 {
			 if(outCardType == T_THREE){
				 startX += DISTRIBUTE_CARD_PLAY_MARGIN;
			 }else{
				 startX -= DISTRIBUTE_CARD_PLAY_MARGIN;
			 }

			 //动作序列：主牌 缩放+(移动，缩放，旋转)  副牌 缩放+（移动，缩放）
			for(int index=0; index < totalNum; index ++){
				auto pCardSprite = outCards.at(index);
				if( pCardSprite == nullptr ){
					continue;
				}
				
				pCardSprite->setAnchorPoint(Vec2(0,0));
				float duration = DISTIRBUTE_CARD_PLAY_RATE;
				ActionInterval *pScaleTo = ScaleTo::create(duration, 0.47f); //出牌为正常牌的 47%
				if(index<3){ //主牌

					ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
					ActionInterval *pMoveTo = MoveTo::create(duration, Vec2(startX, OUT_CARD_RIGHT_POSITON_START_Y));
					EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
					if( index == 0 ){
						RotateTo *RotateTo = RotateTo::create(0.2f, -30);
						ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2(startX +11, OUT_CARD_RIGHT_POSITON_START_Y -14));
						EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, animFinishCall, nullptr)); //主牌第一张牌向左旋转
					}else if( index == 2 ){
						RotateTo *RotateTo = RotateTo::create(0.2f, 24);
						ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2(startX +4, OUT_CARD_RIGHT_POSITON_START_Y +14));
						EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, animFinishCall, nullptr)); //主牌第三张牌向右旋转
						if(isLandlord){
							pCardSprite->addLordSign(); //添加地主标识
						}
					}else{
						// 第二张牌不旋转
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, animFinishCall, nullptr));
					}
				}
				else
				{ //副牌
					ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
					ActionInterval *pMoveTo = MoveTo::create(duration, Vec2(startX + (index+1) * DISTRIBUTE_CARD_PLAY_MARGIN, OUT_CARD_RIGHT_POSITON_START_Y - 10));
					EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
					pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, animFinishCall, nullptr));
					//为了给打出去的牌最后一张加角标
					if (index + 1 == totalNum && isLandlord)
					{
						pCardSprite->addLordSign();
					}
				}
			}
		}
		 break;
		
	 case T_BOMB:	//炸弹
	 case T_BOMB_L:
	 case T_BOMB_S:
	 case T_FOUR_2: //四带两单
	 case T_FOUR_4: //四带双双
		 {
			 if( outCardType == T_FOUR_2 ){
				 startX -= DISTRIBUTE_CARD_PLAY_MARGIN;
			 }else if( outCardType == T_FOUR_2 ){
				 startX -= 2*DISTRIBUTE_CARD_PLAY_MARGIN;
			 }else{
				 startX += DISTRIBUTE_CARD_PLAY_MARGIN;
			 }

			 //动作序列：主牌 缩放+(移动，缩放，旋转)  副牌 缩放+（移动，缩放）
			for(int index=0; index < totalNum; index ++){
				auto pCardSprite = outCards.at(index);
				if( pCardSprite == nullptr ){
					continue;
				}
				
				pCardSprite->setAnchorPoint(Vec2(0,0));
				float duration = DISTIRBUTE_CARD_PLAY_RATE;
				ActionInterval *pScaleTo = ScaleTo::create(duration, 0.47f); //出牌为正常牌的 47%
				if(index<4){ //主牌

					ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
					ActionInterval *pMoveTo = MoveTo::create(duration, Vec2(startX + DISTRIBUTE_CARD_PLAY_MARGIN, OUT_CARD_RIGHT_POSITON_START_Y));
					EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
					if( index == 0 ){
						RotateTo *RotateTo = RotateTo::create(0.2f, -25);
						ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2(startX + DISTRIBUTE_CARD_PLAY_MARGIN -30, OUT_CARD_RIGHT_POSITON_START_Y -22));
						EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, animFinishCall, nullptr)); //主牌第一张牌向左旋转
					}else if( index == 1 ){
						RotateTo *RotateTo = RotateTo::create(0.2f, -12);
						ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2(startX + DISTRIBUTE_CARD_PLAY_MARGIN -17, OUT_CARD_RIGHT_POSITON_START_Y -10));
						EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, animFinishCall, nullptr)); //主牌第二张牌向右旋转
					}else if( index == 3 ){
						RotateTo *RotateTo = RotateTo::create(0.2f, 16);
						ActionInterval *pFinalMove = MoveTo::create(0.2f, Vec2(startX + DISTRIBUTE_CARD_PLAY_MARGIN +15, OUT_CARD_RIGHT_POSITON_START_Y +5));
						EaseSineOut *pFinalSineOut = EaseSineOut::create(pFinalMove);
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), Spawn::create(DelayTime::create(0.4f), RotateTo, pFinalSineOut, nullptr), func, animFinishCall, nullptr)); //主牌第四张牌向右旋转
						if(isLandlord){
							pCardSprite->addLordSign(); //添加地主标识
						}
					}else{
						// 第三张牌不旋转
						pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, animFinishCall, nullptr));
					}
				}
				else
				{ //副牌
					ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
					ActionInterval *pMoveTo = MoveTo::create(duration, Vec2(startX + (index + 1) * DISTRIBUTE_CARD_PLAY_MARGIN, OUT_CARD_RIGHT_POSITON_START_Y - 10));
					EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
					pCardSprite->runAction(Sequence::create(pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, animFinishCall, nullptr));
					//为了给打出去的牌最后一张加角标
					if (index + 1 == totalNum && isLandlord)
					{
						pCardSprite->addLordSign();
					}
				}
			}
		 }
		 break;

	 default:
		 { //默认当顺子出
			 int TotalNum = outCards.size();
			 for(int index = 0; index< TotalNum; index++)
			{
				CardSprite* pCardSprite = outCards.at(index);
				if (pCardSprite)
				{ 
					pCardSprite->setAnchorPoint(Vec2(0,0));
					//动作序列：缩放+(移动，缩放)
					float duration = DISTIRBUTE_CARD_PLAY_RATE;
					ActionInterval *pScaleBy = ScaleBy::create(0.1f, 1.1f);
					ActionInterval *pMoveTo = MoveTo::create(duration, Vec2(startX - DISTRIBUTE_CARD_PLAY_MARGIN/2 + index * DISTRIBUTE_CARD_PLAY_MARGIN, OUT_CARD_RIGHT_POSITON_START_Y));
					EaseSineOut *pSineOut = EaseSineOut::create(pMoveTo);
					ActionInterval *pScaleTo = ScaleTo::create(duration, 0.47f); //出牌为正常牌的 47%
					pCardSprite->runAction(Sequence::create(DelayTime::create(0.06*index), pScaleBy, Spawn::create(pSineOut, pScaleTo, nullptr), func, animFinishCall, nullptr));
					//为了给打出去的牌最后一张加角标
					if (index + 1 == TotalNum && isLandlord)
					{
						pCardSprite->addLordSign();
					}
				}
			}
		 }
		 break;
	 }
}


//左边玩家剩余手牌展示
void OutCardLayer::showLeftLeftCards(const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func)
{
	//创建一组牌
	auto cardLayerRoot = CardsGroup::create(outCards,normal_Card,isLandlord);
	cardLayerRoot->setAnchorPoint(Vec2(0, 0));
	cardLayerRoot->setPosition(Vec2(OUT_CARD_LEFT_POSITON_REMAIN_X, OUT_CARD_LEFT_POSITON_REMAIN_Y));

	GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getnpcLeft()->saveLeftCard(cardLayerRoot);
	cardLayerRoot->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(OutCardLayer::onActionEnd, this)), nullptr));
}
	
//右边玩家剩余手牌展示
void OutCardLayer::showRightLeftCards(const cocos2d::Vector<PokerObject*> &outCards, bool isLandlord, cocos2d::CallFunc *func)
{
	//创建一组牌
	auto cardLayerRoot = CardsGroup::create(outCards,normal_Card,isLandlord);

	float diffX = 0.0f;
	if(outCards.size() >= 10)
	{
		diffX = - DISTRIBUTE_OUT_CARDS_MAGIN * 10;
	}else{
		diffX = - DISTRIBUTE_OUT_CARDS_MAGIN * outCards.size();
	}
	cardLayerRoot->setPosition(Vec2(OUT_CARD_RIGHT_POSITON_REMAIN_X + diffX,OUT_CARD_RIGHT_POSITON_REMAIN_Y));
	
	GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getnpcRight()->saveLeftCard(cardLayerRoot);
	cardLayerRoot->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(OutCardLayer::onActionEnd, this)), nullptr));
}

void OutCardLayer::onActionEnd()
{
	log("onActionEnd");
	if(getParent()){
		removeFromParentAndCleanup(true);
	}
}

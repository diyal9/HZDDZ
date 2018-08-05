/*
* GameBgLayer.h
*
*
*  Created on: 2015-5-11
*      Author: diyal.yin
*/

#include "GameBgLayer.h"
#include "../GameSceneDefine.h"
#include "WWMacros.h"

USING_NS_CC;

GameBgLayer::GameBgLayer(void)
{
	
}


GameBgLayer::~GameBgLayer(void)
{
}

bool GameBgLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0,0,0,0))) return false;

	
	bool bRet = false;
	do 
	{
		createView();

		bRet = true;
	}
	while (0);

	return bRet;
}

void GameBgLayer::createView()
{
	auto pBgSprite = Sprite::create("res/Images/GameScene/game_bg.jpg");
	pBgSprite->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5, CUR_SCREEN_SIZE.height * 0.5));
	this->addChild(pBgSprite, BASIC_ZORDER_ROOT);

	//栅栏
	auto s_lBarier = Sprite::create("res/Images/GameScene/barrier.png");
	s_lBarier->setPosition(Vec2(s_lBarier->getContentSize().width * 0.25, CUR_SCREEN_SIZE.height * 0.3));
	this->addChild(s_lBarier, BASIC_ZORDER_ROOT);

	auto s_lBarierRight = Sprite::create("res/Images/GameScene/barrier.png");
	s_lBarierRight->setPosition(Vec2(pBgSprite->getContentSize().width - s_lBarier->getContentSize().width * 0.25, CUR_SCREEN_SIZE.height * 0.3));
	s_lBarierRight->setFlippedX(true);
	this->addChild(s_lBarierRight, BASIC_ZORDER_ROOT);

	//屋檐
	auto s_lHouseBottom = Sprite::create("res/Images/GameScene/roomFrame1.png");
	s_lHouseBottom->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5, s_lHouseBottom->getContentSize().height * 0.5));
	this->addChild(s_lHouseBottom, BASIC_ZORDER_ROOT + 1);

}

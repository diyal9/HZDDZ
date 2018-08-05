/**
 * LoadingLayer.h
 *
 * created in 2015-6-3
 * by Diyal.yin
 *
**/
#include "LoadingLayer.h"

LoadingLayer::LoadingLayer(void)
{
	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(LoadingLayer::onTouchBegan, this);
	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);
}

LoadingLayer::~LoadingLayer()
{

}

bool LoadingLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0,0,0,128))) return false;

	do 
	{

	} while (0);

	return true;
}

void LoadingLayer::onEnter()
{

	LayerColor::onEnter();
	log("CommDialog::onEnter");
}

void LoadingLayer::onExit()
{
	LayerColor::onExit();

	log("CommDialog::onExit");
}

bool LoadingLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	CCLOG("WWDialog::ccTouchBegan");
	return true;
}
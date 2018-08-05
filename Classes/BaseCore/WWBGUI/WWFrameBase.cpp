/*
* WWFrameBase.h
*
*	Popup Frame基类
*
*  Created on: 2015-5-22
*      Author: diyal.yin
*/
#include "WWFrameBase.h"
#include "cocos2d.h"


USING_NS_CC;
WWFrameBase::WWFrameBase() : 
	m_pFrameMode(1)
{

}

WWFrameBase::~WWFrameBase()
{
}

bool WWFrameBase::init()
{
	if(!LayerColor::initWithColor(Color4B(0,0,0,200)))
	{
		return false;
	}

	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(WWFrameBase::onTouchBegan, this);
	m_pListner->onTouchEnded = CC_CALLBACK_2(WWFrameBase::onTouchEnded, this);

	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);

	initView();

	return true;
}

bool WWFrameBase::initSmallFrame()
{
	if(!LayerColor::initWithColor(Color4B(0,0,0,200)))
	{
		return false;
	}

	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(WWFrameBase::onTouchBegan, this);
	m_pListner->onTouchEnded = CC_CALLBACK_2(WWFrameBase::onTouchEnded, this);

	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);

	m_pFrameMode = 2;
	initView();

	return true;
}

void WWFrameBase::onEnter()
{

	LayerColor::onEnter();
	log("WWFrameBase::onEnter");
}

void WWFrameBase::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	LayerColor::onExit();

	log("WWFrameBase::onExit");
}

void WWFrameBase::initView()
{
	//添加背景
	if (m_pFrameMode == 1)
	{
		m_pBg = Sprite::create("res/Images/Common/frameBg1.png");
	}
	else if (m_pFrameMode == 2)
	{
		m_pBg = Sprite::create("res/Images/Common/dialogBg.png");
	}
	m_pBg->setPosition(CUR_SCREEN_SIZE.width*0.5,CUR_SCREEN_SIZE.height*0.5);
	this->addChild(m_pBg, BASIC_ZORDER_ROOT);

	//添加关闭按钮
	m_pClose = WWTouchSprite::createWithFile("res/Images/Common/closeBtn.png",this, callfuncO_selector(WWFrameBase::close));
	if (m_pFrameMode == 1)
	{
	    m_pClose->setPosition(Vec2(1493,875));
	}
	else if (m_pFrameMode == 2)
	{
        m_pClose->setPosition(Vec2(m_pBg->getContentSize().width * 0.98,m_pBg->getContentSize().height * 0.95));
	}
	m_pBg->addChild(m_pClose,BASIC_ZORDER_ROOT+3);

	//特效
	m_pBg->setScale(0.9f);
	EaseBackOut* _easeIn = EaseBackOut::create(ScaleTo::create(0.2f,1.0f));
	Action* _action = Sequence::create(_easeIn,nullptr);
	m_pBg->runAction(_action);
}

bool WWFrameBase::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	CCLOG("WWDialog::ccTouchBegan");
	return true;
}

void WWFrameBase::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point startPoint =  m_pBg->convertToNodeSpace(touch->getStartLocation());
	Point locationInNode = m_pBg->convertToNodeSpace(touch->getLocation());
	Size s = m_pBg->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	// 点击范围判断检测
	if ((!rect.containsPoint(locationInNode)) && (!rect.containsPoint(startPoint)))
	{
		log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
		this->removeFromParent();
	}
}

void WWFrameBase::close(Ref * node)
{
	this->removeFromParent();
}
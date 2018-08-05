#include "WWTouchSprite.h"
#include "../../WWMacros.h"
#include "Control/WWCSound/WWSoundManager.h"

USING_NS_CC;

const static float g_moveMax = 30;

const static std::string btn_click_sound = "res/Audio/mp3/Audio_Button_Click.mp3";//按钮点击音效

WWTouchSprite::WWTouchSprite() :
	m_bEnable(true)
	,m_swallow(true)
	,m_touchScale(1.0f)
	,m_pListener(nullptr)
	,m_touchFuncO(nullptr)
	,m_hadCancel(false)
	,m_bIsHold(false)
	,fHoldTime(0)
{
}

WWTouchSprite::~WWTouchSprite()
{
	//log("%s %d\r\n", __FUNCTION__, __LINE__);
	m_pListener = nullptr;

}

void WWTouchSprite::onEnter()
{
	//log("%s %d\r\n", __FUNCTION__, __LINE__);
    Node::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(m_swallow);

	listener->onTouchBegan = CC_CALLBACK_2(WWTouchSprite::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(WWTouchSprite::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(WWTouchSprite::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(WWTouchSprite::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void WWTouchSprite::onExit()
{
	//log("%s %d\r\n", __FUNCTION__, __LINE__);
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    Node::onExit();
}

WWTouchSprite* WWTouchSprite::createWithFile(const char *pszFileName, cocos2d::Ref *rec, cocos2d::SEL_CallFuncO selector)
{
	WWTouchSprite *sp = new WWTouchSprite();
	if (sp&&sp->init(pszFileName, rec, selector))
	{
		sp->autorelease();
		return sp;
	}
	CC_SAFE_DELETE(sp);
	return sp;
}

bool WWTouchSprite::init(const char *pszFileName, Ref *rec, cocos2d::SEL_CallFuncO selector)
{
	bool bRet = false;
	do 
	{
		m_pBg = cocos2d::Sprite::create(pszFileName);
		CC_BREAK_IF(!m_pBg);
		this->setContentSize(m_pBg->getContentSize());
		this->addChild(m_pBg);
		bRet = true;

		m_pListener = rec;
		m_pfnSelector = selector;
	} while (0);

	return bRet;
}

WWTouchSprite* WWTouchSprite::createWithSpriteFrameName(const char *pszSpriteFrameName, cocos2d::Ref *rec, cocos2d::SEL_CallFuncO selector)
{
	WWTouchSprite *sp = new WWTouchSprite();
	if (pszSpriteFrameName && rec && selector && sp->initWithSpriteFrameName(pszSpriteFrameName, rec, selector))
	{
		sp->autorelease();
		return sp;
	}
	CC_SAFE_DELETE(sp);
	return sp;
}


bool WWTouchSprite::initWithSpriteFrameName(const char *pszSpriteFrameName, Ref *rec, cocos2d::SEL_CallFuncO selector)
{
	bool bRet = false;
	do 
	{
		m_pBg = cocos2d::Sprite::createWithSpriteFrameName(pszSpriteFrameName);
		CC_BREAK_IF(!m_pBg);
		this->setContentSize(m_pBg->getContentSize());
		this->addChild(m_pBg);

		m_pListener = rec;
		m_pfnSelector = selector;

		bRet = true;
	} while (0);

	return bRet;
}

WWTouchSprite* WWTouchSprite::createCrossButton(const char *pszFileName, const char *labelFileName, cocos2d::Ref *rec, cocos2d::SEL_CallFuncO selector)
{
	WWTouchSprite *sp = new WWTouchSprite();
	if (sp&&sp->initWithFile(pszFileName, labelFileName, rec, selector))
	{
		sp->autorelease();
		return sp;
	}
	CC_SAFE_DELETE(sp);

	return nullptr;
}

bool WWTouchSprite::initWithFile(const std::string& filePath, const std::string& labelFileName, Ref *rec, cocos2d::SEL_CallFuncO selector)
{
	bool bRet = false;
	do 
	{
		m_pBg = cocos2d::Sprite::create(filePath);
		CC_BREAK_IF(!m_pBg);
		this->setContentSize(m_pBg->getContentSize());
		this->addChild(m_pBg);

		auto labelSprite = Sprite::create(labelFileName);
		labelSprite->setPosition(Vec2(m_pBg->getContentSize().width * 0.5, m_pBg->getContentSize().height * 0.5));
		m_pBg->addChild(labelSprite,1);

		m_pListener = rec;
		m_pfnSelector = selector;

		bRet = true;
	} while (0);

	return bRet;
}

void WWTouchSprite::setEnabled(bool en, bool isGray)
{
	m_bEnable = en;
	if (m_bEnable)
	{
		m_pBg->setColor(cocos2d::Color3B::WHITE);
	}
	else
	{
		if (isGray)
		{
			m_pBg->setColor(cocos2d::Color3B::GRAY);
		}
		else
		{
			m_pBg->setColor(cocos2d::Color3B::WHITE);
		}
	}
}

void WWTouchSprite::setClickHoldTime(float nTime)
{
	fHoldTime = nTime;
}

void WWTouchSprite::setClickCommon(float delta)
{
	m_bIsHold = false;
}

cocos2d::Sprite* WWTouchSprite::getSprite()
{
	return m_pBg;
}

bool WWTouchSprite::containTouch(cocos2d::Touch* touch)
{
	cocos2d::Point touchLocation = touch->getLocation();
	cocos2d::Rect r;
	cocos2d::Point local;
	if (m_pBg)
	{
		local = m_pBg->convertToNodeSpace(touchLocation);
		r = m_pBg->getBoundingBox();
		r.origin = cocos2d::Vec2::ZERO;

		//对点击区域缩放进行调整
		r = getTouchRect(r, m_touchScale);
	}
	return r.containsPoint(local);
}

cocos2d::Rect WWTouchSprite::getTouchRect(cocos2d::Rect rect,float scale)
{
	//对点击区域缩放进行调整
	rect.origin.x = -(m_touchScale-1)*rect.size.width/2.f;
	rect.origin.y = -(m_touchScale-1)*rect.size.height/2.f;
	rect.size = Size(m_touchScale*rect.size.width, m_touchScale*rect.size.height);

	return rect;
}


bool WWTouchSprite::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	//log("%s %d\r\n", __FUNCTION__, __LINE__);

	if (this->getParent())
	{
		if (!this->isVisible())
		{
			return false;
		}
	}
	
	if (!m_bEnable)
	{
		return false;
	}

	if (!containTouch(touch))
	{
		return false;
	}

	if (m_bIsHold)
	{
		log("now is a hold state");
		return false;
	}

	m_pBg->setColor(cocos2d::Color3B::GRAY);

	m_beginPoint = touch->getLocation();

	if (FileUtils::getInstance()->isFileExist(btn_click_sound))
	{
		WWSoundManager::getInstance()->playEffect(btn_click_sound.c_str());
	}

	if (m_pBg)
	{
		if (m_pBg->getScale() == 1.0f)
		{
			m_pBg->setScale(0.98f);
		}
	}

    return true;
}

void WWTouchSprite::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	//log("%s %d\r\n", __FUNCTION__, __LINE__);

	if (!m_swallow)
	{
		auto movePoint = touch->getLocation();

		if (abs(movePoint.x-m_beginPoint.x) > g_moveMax ||abs(movePoint.y-m_beginPoint.y) > g_moveMax)
		{
			onTouchCancelled(touch, unused_event);
		}
	}
}

void WWTouchSprite::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);
	/*
	    不同于2.x,3.0新的渲染机制是以队列的方式调用OPGL渲染接口。所以回调函数需要Hold到下一帧。
		否则，在下一帧处理回调函数的时候，会当做野指针。
	*/
	this->retain(); 
	m_pBg->setColor(cocos2d::Color3B::WHITE);
	if (!m_hadCancel)
	{
		touchActive(touch);

		if (!m_bIsHold)
		{
			m_bIsHold = true;
			scheduleOnce(CC_SCHEDULE_SELECTOR(WWTouchSprite::setClickCommon), fHoldTime);
		}

	}
	m_hadCancel = false;
	this->release();
}

void WWTouchSprite::touchActive(cocos2d::Touch* touch)
{
	if (m_pBg)
	{
		if (m_pBg->getScale() == 0.98f)
		{
			m_pBg->setScale(1.0f);
		}
	}

	if (containTouch(touch))
	{
		activate();
	}
}

void WWTouchSprite::activate()
{
	if (m_pListener && m_pfnSelector)
	{
		(m_pListener->*m_pfnSelector)(this);
	}
	else if(m_touchFuncO)
	{
		m_touchFuncO(this);
	}
}

void WWTouchSprite::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (m_pBg)
    {
        if (m_pBg->getScale() == 0.98f)
        {
            m_pBg->setScale(1.0f);
        }
    }
    
	m_pBg->setColor(cocos2d::Color3B::WHITE);
    m_hadCancel = true;
}



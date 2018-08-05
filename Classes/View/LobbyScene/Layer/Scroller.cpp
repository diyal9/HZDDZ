#include "Scroller.h"

USING_NS_CC;

Scroller::Scroller() : 
	m_pDir(NULL),
	m_pAllowBounce(false),
	m_pIsTouching(false),
	m_pMode(0),
	m_pNewPos(0),
	m_pRangeMin(0),
	m_pRangeMax(100), //初始设置为100.测试用
	m_pSpeed(0),
	m_pTargetPos(0),
	m_pPos(0)
{
}

Scroller::~Scroller(void)
{
	savePos.clear();
}

bool Scroller::init()
{

	return true;
}

bool Scroller::TouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//Vec2 m_beginPos = pTouch->getLocationInView();	
	//m_beginPos = Director::getInstance()->convertToGL(m_beginPos);

	if (m_pIsTouching)
	{
		//不处理多点触摸
		return false;
	}

	m_pIsTouching = true;
	m_pMode = SCROLLTYPE_TOUCH;

	return true;
}

void Scroller::TouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	float delta = pTouch->getDelta().x; //帧增量
	float newpost = m_pPos + delta;  //新的位移点
	if (newpost < m_pRangeMin || newpost > m_pRangeMax) //如果在屏外
	{
		if (m_pAllowBounce)
		{
			m_pPos = m_pPos + delta * 0.5;
		}
		else 
		{
			if (newpost < m_pRangeMin)
			{
				m_pPos = m_pRangeMin;
			}
			else
			{
				m_pPos = m_pRangeMax;
			}
		}
	}
	else
	{
		m_pPos = m_pPos + delta;
	}
}

void Scroller::TouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	m_pIsTouching = false;
	m_pMode = SCROLLTYPE_NONE;
	if (savePos.size() > 0)
	{
		int avgPos = 0;

		//std::vector<int>::iterator iter = savePos.begin(); 
		//avgPos += *iter;

		for (unsigned int i = 0; i < savePos.size(); i++)
		{
			float tmpPos = savePos[i];
			avgPos = avgPos + abs(tmpPos); 
		}

		avgPos = avgPos / savePos.size();
		m_pSpeed = (m_pPos + avgPos) * 8;
		m_pMode = SCROLLTYPE_INERTIA;
	} 
	else
	{
		checkBounce();
	}
}

void Scroller::TouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
}

void Scroller::update(float dt)
{
	//速度处理
	if (m_pMode == SCROLLTYPE_INERTIA)
	{
		log("update inertia");
		int oldSpeed = m_pSpeed;
		int speedChangeFactor = 0; //变速因子
		if ((m_pPos < m_pRangeMin) || (m_pPos > m_pRangeMax))
		{
			speedChangeFactor = 1000;
		}
		else
		{
			speedChangeFactor = 200;
		}

		if (m_pSpeed >= 0) //根据变速度因子做降速运动
		{
			m_pSpeed = m_pSpeed  - speedChangeFactor * dt;
		}
		else
		{
			m_pSpeed = m_pSpeed + speedChangeFactor * dt;
		}

		if (oldSpeed * m_pSpeed <= 0)
		{
			m_pSpeed = 0;
			m_pMode = SCROLLTYPE_NONE;
			checkBounce();
		}
		else
		{
			m_pPos = m_pPos + m_pSpeed * dt;
		}
	}
	else if ( (m_pMode == SCROLLTYPE_BOUNCE) || (m_pMode == SCROLLTYPE_CODE))
	{
		//log("update, %s", m_pMode);
		m_pPos = m_pPos + (m_pTargetPos - m_pPos) * 5 * dt;
		if (abs(m_pTargetPos - m_pPos) < 5)
		{
			m_pPos = m_pTargetPos;
			//m_pMode = "";
			m_pMode = SCROLLTYPE_NONE;
		}
	}

	//触摸点处理
	if (m_pIsTouching)
	{
		log("update, Is Touching");
		if (savePos.size() >= 5)
		{
			std::vector<float>::iterator iter = savePos.begin();
			savePos.erase(iter); //删除第一个pos
		}
		savePos.push_back(m_pPos);
	}
	else
	{
		if (savePos.size() > 0)
		{
			savePos.clear();
		}
	}

	if (!m_pAllowBounce)
	{
		if (m_pPos < m_pRangeMin)
		{
			m_pPos = m_pRangeMin;
			m_pSpeed = 0;
			m_pMode = SCROLLTYPE_NONE;
		}
		else if (m_pPos > m_pRangeMax)
		{
			m_pPos = m_pRangeMax;
			m_pSpeed = 0;
			m_pMode = SCROLLTYPE_NONE;
		}
	}
}

void Scroller::checkBounce()
{
	if (m_pPos < m_pRangeMin)
	{
		m_pMode = SCROLLTYPE_BOUNCE;
		m_pTargetPos = m_pRangeMin;
	}
	else if (m_pPos > m_pRangeMax)
	{
		m_pMode = SCROLLTYPE_BOUNCE;
		m_pTargetPos = m_pRangeMax;
	}
}

void Scroller::setRange(float rangeMin, float rangeMax)
{
	m_pRangeMin = rangeMin;
	m_pRangeMax = rangeMax;

	if ((m_pMode != SCROLLTYPE_TOUCH) && (m_pMode != SCROLLTYPE_INERTIA))
	{
		checkBounce();
	}
}

//滚动到 animated为是否有滚动效果
void Scroller::scrollTo(float pos, bool animated)
{
	if (m_pIsTouching) 
	{
		return;
	}

	if (animated)
	{
		m_pMode = SCROLLTYPE_CODE;
		m_pTargetPos = pos;
	}
	else
	{
		m_pPos = pos;
	}
}





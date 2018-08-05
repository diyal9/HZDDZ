#include "WWCircleAction.h"

USING_NS_CC;

#include "WWMacros.h"

CircleBy* CircleBy::create(float t, const Vec2& position, Point circleCenter, float radius)
{
	CircleBy *pCircle = new (std::nothrow) CircleBy();
	if (pCircle && pCircle->init(t,position,circleCenter,radius))
	{
		pCircle->autorelease();
		return pCircle;
	}
	CC_SAFE_DELETE(pCircle);
	return nullptr;
}

bool CircleBy::init(float t, const Vec2& position, Point circleCenter, float radius)
{
	bool ret = false;
	if (ActionInterval::initWithDuration(t))
	{
		_endPosition = Vec3(position.x, position.y, 0);

		m_circleCenter = circleCenter;
		m_radius = radius;
		/* 每次转动的时转过的单位弧度 */
		m_radian = 2.0 * PI / (t / Director::getInstance()->getAnimationInterval());
		m_moveTimes = 1;

		ret = true;
	}
	return ret;
}

void CircleBy::update(float time)
{
	float radian = m_radian * m_moveTimes; //每次偏移的弧度 = 单位弧度 * 运动次数

	/* 根据半径和弧度求出圆上一点坐标 */
	float x = m_radius * sin(radian);
	float y = m_radius * cos(radian);
	Point newPos = Vec2(x + m_circleCenter.x, y + m_circleCenter.y);
	getTarget()->setPosition(newPos);
	m_moveTimes += 1;
}




RadianBy* RadianBy::create(float duration, const Point& ptCenter, float deltaAngle)
{
	RadianBy* pRet= new RadianBy();
	pRet->initWithDuration(duration, ptCenter, deltaAngle);
	pRet->autorelease();

	return pRet;
}

bool RadianBy::initWithDuration(float duration, const Point& ptCenter, float deltaAngle)
{
	if(ActionInterval::initWithDuration(duration))
	{
		m_ptCenter= ptCenter;
		m_fAngleDelta= deltaAngle;
		return true;
	}

	return false;
}

//CCObject* CCArcBy::copyWithZone(CCZone* pZone)
//{
//	CCZone* pNewZone = NULL;
//	CCArcBy* pCopy = NULL;
//	if(pZone && pZone->m_pCopyObject) 
//	{
//		//in case of being called at sub class
//		pCopy = (CCArcBy*)(pZone->m_pCopyObject);
//	}
//	else
//	{
//		pCopy = new CCArcBy();
//		pZone = pNewZone = new CCZone(pCopy);
//	}
//
//	CCActionInterval::copyWithZone(pZone);
//
//	pCopy->initWithDuration(m_fDuration, m_ptCenter, m_fAngleDelta);
//
//	CC_SAFE_DELETE(pNewZone);
//	return pCopy;
//}

void RadianBy::startWithTarget(CCNode *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
	m_previousPosition = m_startPosition = pTarget->getPosition();
}

ActionInterval* RadianBy::reverse()
{
	return RadianBy::create(_duration, m_ptCenter, -m_fAngleDelta);
}

void RadianBy::update(float time)
{
	log("%f", time);
	if(getTarget())
	{
#if CC_ENABLE_STACKABLE_ACTIONS
		Point currentPos = getTarget()->getPosition();
		Point diff = ccpSub(currentPos, m_previousPosition);
		m_startPosition = ccpAdd( m_startPosition, diff);
		Point newPos =  m_ptCenter + ccpRotateByAngle(m_startPosition-m_ptCenter, Vect::ZERO, m_fAngleDelta*time);

		getTarget()->setPosition(newPos);
		getTarget()->setRotation(-CC_RADIANS_TO_DEGREES(m_fAngleDelta*time));
		m_previousPosition = newPos;
#else
		getTarget()->setPosition(m_ptCenter + ccpRotateByAngle(m_startPosition-m_ptCenter, Vect::ZERO, m_fAngleDelta*time));
#endif  // CC_ENABLE_STACKABLE_ACTIONS
	}
}


// 抛物线运动
EaseInOut* ParabolaTo::create(float t, Point startPoint, Point endPoint, float ctrl1, float ctrl2, float height /* = 0 */, float angle /* = 60 */){

	// 把角度转换为弧度
	float radian = angle * PI / 180.0f;
	// 第一个控制点为抛物线左半弧的中点
	float q1x = startPoint.x+(endPoint.x - startPoint.x) * ctrl1;
	Point q1 = Vec2(q1x, height + startPoint.y + cos(radian)*q1x);		
	// 第二个控制点为整个抛物线的中点
	float q2x = startPoint.x + (endPoint.x - startPoint.x) * ctrl2;
	Point q2 = Vec2(q2x, height + startPoint.y + cos(radian)*q2x);
	
	//曲线配置
	ccBezierConfig cfg;
	cfg.controlPoint_1 = q1;
	cfg.controlPoint_2 = q2;
	cfg.endPosition = endPoint;
	//使用CCEaseInOut让曲线运动有一个由慢到快的变化，显得更自然
	return CCEaseInOut::create(CCBezierTo::create(t,cfg), t); 
}

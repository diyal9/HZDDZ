/*
 * WWCircleAction.h
 * 弧形动画
 * 设计思路:
 *
 *  Created on: 2015-6-4
 *      Author: diyal.yin
 */
#ifndef __BASECORE_WWEFFECTBASE_WWCIRCLEACTION__
#define __BASECORE_WWEFFECTBASE_WWCIRCLEACTION__

#include "cocos2d.h"

USING_NS_CC;

/*
圆周运动类
*/
class CircleBy : public MoveTo
{
public:
	/* 创建动作 参数:圆心坐标、半径 */
	static CircleBy* create(float t, const Vec2& position, Point circleCenter, float radius); 
	bool init(float t, const Vec2& position, Point circleCenter, float radius);
	virtual void update(float time);
protected:
	Point m_circleCenter; //圆心坐标
	float m_radius; //半径
	float m_radian; //弧度
	float m_moveTimes; //刷新次数
};

/*
圆弧动作类
*/
class RadianBy : public ActionInterval
{
public:
	//初始化圆弧动作类
	bool initWithDuration(float duration, const Point& ptCenter, float deltaAngle);

	//virtual CCObject* copyWithZone(cocos2d::CCZone* pZone);
	virtual void startWithTarget(Node *pTarget);
	virtual ActionInterval* reverse(void);
	virtual void update(float time);

public:
	static RadianBy* create(float duration, const Point& ptCenter, float deltaAngle);

protected:
	Point m_startPosition;
	Point m_previousPosition;

	Point m_ptCenter;
	float m_fAngleDelta;
};

// 抛物线运动
class ParabolaTo
{
public:
	/*创建一个抛物线动作
	参数：
		t					时间
		startPoint		开始点
		endPoint		结束点
		ctrl1           贝塞尔曲线控制点1,距离比
		ctrl2           贝塞尔曲线控制点2
		height			高度（影响抛物线的高度）
		angle			角度（贝塞尔曲线两个控制点与y轴的夹角，直接影响精灵的抛出角度）
	*/
	static EaseInOut* create(float t, Point startPoint, Point endPoint, float ctrl1, float ctrl2, float height = 0, float angle = 60);
};
#endif /* __BASECORE_WWEFFECTBASE_WWCIRCLEACTION__ */

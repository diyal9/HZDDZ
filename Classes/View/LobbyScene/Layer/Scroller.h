/*
* Scroller.h
*	功能描述： 滚动器
*  Created on: 2015年4月30日 下午2:09:51
*      Author: diyal.yin
*  //视差节点ParallaxNode，这里没有用到这个，而是自己封装的。
*
*  备注：
*	滚动有四种情况
*	1、mode = "touch"	玩家通过触摸方式拖动
*	2、mode = "inertia"	玩家拖动完毕后，惯性滑动（加速度恒定）
*	3、mode = "bounce"	玩家拖动完毕或惯性滑动后，如果超出边界，快速弹回（速度跟越界程度成正比）
*	4、mode = "code"	程序控制自动移动（速度与目标距离成正比）
*	其中3、4在实现上可以归为一类
*	优先顺序按照1、4、2、3来判断
*	eg:程序控制自动移动（情形4时），玩家手动触摸（情形1），则会停止自动移动，改为手动。
*
*	   
*  滑动范围算法:
*	前提条件: 背景图锚点是中心点，贴在Layer中心点上（Layer锚点为原点），目的尽量将背景的中间内容展示。实际滑动的是背景图的父节点
*	所以该Layer实际的滑动Range min =  -(背景图大小 - 当前窗口大小) / 2  ~  max =   (背景图大小 - 当前窗口大小) / 2
*  公开属性:
*   scrollTo(float rangeMin, float rangeMax) 滚动到（pos为位移目标位置， animated为是否有移动效果）
*   setRange(float rangeMin, float rangeMax) 设置滑动范围
*/

#ifndef _VIEW_LOBBYSCENE_LAYER_SCROLLER_H
#define _VIEW_LOBBYSCENE_LAYER_SCROLLER_H


#include "cocos2d.h"
#include <iostream>

USING_NS_CC; 

//滚动类型
enum
{
	SCROLLTYPE_NONE,      //无
	SCROLLTYPE_TOUCH,     //触摸拖动
	SCROLLTYPE_INERTIA,   //惯性滑动
	SCROLLTYPE_BOUNCE,    //边界触碰反弹
	SCROLLTYPE_CODE       //自动控制（程序里自动移动）
};

class Scroller : public cocos2d::Ref
{
public:
	Scroller();
	virtual ~Scroller();
	virtual bool init();
	CREATE_FUNC(Scroller);

	bool TouchBegan(Touch* pTouch, Event* pEvent);
	void TouchMoved(Touch *touch, Event *pEvent);
	void TouchCancelled(Touch *touch, Event *pEvent);
	void TouchEnded(Touch* pTouch, Event* pEvent);

private:
	CC_SYNTHESIZE(char, m_pDir, pDir); //判断是横向滑动
	CC_SYNTHESIZE(bool, m_pAllowBounce, pAllowBounce); //是否反弹
	CC_SYNTHESIZE(bool, m_pIsTouching, pIsTouching);
	CC_SYNTHESIZE(int, m_pMode, pMode);

	CC_SYNTHESIZE(float, m_pNewPos, pNewPos); //新位置
	CC_SYNTHESIZE(float, m_pRangeMin, pRangeMin); //最小范围
	CC_SYNTHESIZE(float, m_pRangeMax, pRangeMax); //最大范围
	CC_SYNTHESIZE(float, m_pSpeed, pSpeed); //滑动速度
	CC_SYNTHESIZE(float, m_pTargetPos, pTargetPos);
	CC_SYNTHESIZE(float, m_pPos, pPos);

	std::vector<float> savePos; //点阵列

	void checkBounce();
	void update(float dt);
	void setRange(float rangeMin, float rangeMax);
	void scrollTo(float pos, bool animated);
};

#endif /* _VIEW_LOBBYLAYER_SCROLLER_H */

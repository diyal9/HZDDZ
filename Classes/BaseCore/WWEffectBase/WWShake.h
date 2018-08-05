/*! @file
********************************************************************************
<PRE>
模块名       :  基本控件
文件名       :  WWShake.h
文件实现功能 :  节点震动动画
作者         :  Diyal.yin
版本         :  1.0
Copyright (c)  WaWaGame Technologies
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/07/03   1.0      diyal.yin           新建
</PRE>
*******************************************************************************/
#ifndef __BASECORE_WWEFFECTBASE_WWSHAKE__
#define __BASECORE_WWEFFECTBASE_WWSHAKE__

#include "cocos2d.h"

USING_NS_CC;

class WWShake : public ActionInterval
{
public:
	WWShake();
	virtual ~WWShake();

	 /* 
      * 产生震动效果的初始化函数参数,两个方向相同
      * @param d 震动持续的时间
      * @param strength 震动的幅度
     */
	static WWShake* create(float d, float strength);

	// 产生震动效果的初始化函数,两个方向值不一样
	static WWShake* create(float d, float strength_x, float strength_y);

	bool initWithDuration(float d, float strength_x, float strength_y);

	virtual WWShake* clone() const override;
	virtual WWShake* reverse(void) const override;
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;
	virtual void stop(void); //停止抖动，位置恢复

	float fgRangeRand(float min, float max);

protected:
	float _initial_x, _initial_y; //位置
	float _strength_x, _strength_y; //振幅
};

#endif /* __BASECORE_WWEFFECTBASE_WWSHAKE__ */

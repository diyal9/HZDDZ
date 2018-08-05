/*
 * WWFrameBase.h
 *
 *	Popup Frame基类
 *
 *  Created on: 2015-5-22
 *      Author: diyal.yin
 */

#ifndef __HD_DDZ_BASECORE_WWBGUI_WWFrameBase_H__
#define __HD_DDZ_BASECORE_WWBGUI_WWFrameBase_H__
#include "../../WWMacros.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"

USING_NS_CC;

class WWFrameBase : public LayerColor
{
public:
	WWFrameBase();

	virtual ~WWFrameBase();

	virtual bool init();

	virtual bool initSmallFrame(); //中号Frame初始化

	virtual void onEnter();

	virtual void onExit();

	virtual void initView();

	void setBackgroudColor(cocos2d::Color4B bgColor){initWithColor(bgColor);};

	virtual void close(Ref * node);

	/*** back按钮监听 ****/
	virtual void setBackKeyEnabled(bool enable){};

    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	//监听触摸事件，子类如果需要触摸范围内关闭 则需要调用父类此方法
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	//弹窗特效

private:
	CC_SYNTHESIZE(Sprite*, m_pBg, pBg);
	CC_SYNTHESIZE(WWTouchSprite*, m_pClose, pClose);

    CC_SYNTHESIZE(int, m_pFrameMode, pFrameMode); //设置窗体大小 （默认为1，大Frame框， 2 为中号）
};

#endif /* __HD_DDZ_BASECORE_WWBGUI_WWFrameBase_H__ */

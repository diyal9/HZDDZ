/*! @file
********************************************************************************
<PRE>
模块名       :  基本控件
文件名       :  WWTouchSprite.h
文件实现功能 :  包装的基本控件
作者         :  Diyal.yin
版本         :  1.0
Copyright (c)  WaWaGame Technologies
--------------------------------------------------------------------------------
普通图片按钮创建
Cross图片按钮创建
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/06/19   1.0      diyal.yin           新建
</PRE>
*******************************************************************************/

#ifndef __HD_DDZ_BASECORE_WWBGUI_TouchSprite_H__
#define __HD_DDZ_BASECORE_WWBGUI_TouchSprite_H__

#include "cocos2d.h"

USING_NS_CC;

class WWTouchSprite : public cocos2d::Node
{

public:
    
    WWTouchSprite();
    virtual ~WWTouchSprite();
	
    virtual void onEnter();
    virtual void onExit();

	//根据文件创建
    static WWTouchSprite* createWithFile(const char *pszFileName, Ref *rec, cocos2d::SEL_CallFuncO selector);
	bool init(const char *pszFileName, Ref *rec, cocos2d::SEL_CallFuncO selector);

	//根据FrameName创建
	static WWTouchSprite* createWithSpriteFrameName(const char *pszSpriteFrameName, Ref *rec, cocos2d::SEL_CallFuncO selector);
	bool initWithSpriteFrameName(const char *pszSpriteFrameName, Ref *rec, cocos2d::SEL_CallFuncO selector);

	//创建重叠交叉按钮（带标签）
    static WWTouchSprite* createCrossButton(const char *pszFileName, const char *labelFileName, cocos2d::Ref *rec, cocos2d::SEL_CallFuncO selector);
	bool initWithFile(const std::string& filePath, const std::string& labelFileName, Ref *rec, cocos2d::SEL_CallFuncO selector);

	//使能
	void setEnabled(bool en, bool isGray = true); 

	//设置锁模式（防连点)
	void setClickHoldTime(float nTime = 0.5f);

	//获得背景图
	cocos2d::Sprite* getSprite();
private:
	void setClickCommon(float delta);

protected:
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

	virtual void touchActive(cocos2d::Touch* touch);
    
private:
	CC_SYNTHESIZE(cocos2d::Sprite*, m_pBg, pBg); //背景图

	cocos2d::SEL_CallFuncO m_pfnSelector; //回调函数
	cocos2d::Ref *m_pListener;
	std::function<void(Ref*)>  m_touchFuncO;

	bool m_bEnable; 
	bool m_swallow;
	float m_touchScale;
	CC_SYNTHESIZE(bool, m_hadCancel, hadCancel);

	cocos2d::Vec2 m_beginPoint;
	bool containTouch(cocos2d::Touch* touch);
	cocos2d::Rect getTouchRect(cocos2d::Rect rect,float scale);
	void activate(); //按钮响应

	CC_SYNTHESIZE(bool, m_bIsHold, bIsHold); //按钮点击过一次，现处于Hold模态
	float fHoldTime; //Hold时间
};


#endif /* defined(__HD_DDZ_BASECORE_WWBGUI_TouchSprite_H__) */

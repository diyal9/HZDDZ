#ifndef _VIEW_LOBBYSCENE_LAYER_BOTTOMBARLAYER_h
#define _VIEW_LOBBYSCENE_LAYER_BOTTOMBARLAYER_h


#include "cocos2d.h"
#include <iostream>

#include "BaseCore/Common/FactoryTools.h"
#include "BaseCore/Common/UICoordConfig.h"

USING_NS_CC; 

class BottomBarLayer : public Layer
{
public:
	BottomBarLayer(void);
	virtual ~BottomBarLayer(void);

	virtual bool init();
	
	CREATE_FUNC(BottomBarLayer);

	void ButtonDepress(Ref * theBtn);  //按钮事件管理

	void onTouchSpriteDepress(Ref * theBtn);  // touchSprite按纽响应
	
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch *touch, Event *pEvent);
	virtual void onTouchCancelled(Touch *touch, Event *pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
};

#endif

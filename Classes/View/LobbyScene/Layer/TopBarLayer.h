#ifndef _VIEW_LOBBYSCENE_LAYER_TOPBARLAYER_h
#define _VIEW_LOBBYSCENE_LAYER_TOPBARLAYER_h

#include "cocos2d.h"
#include <iostream>

#include "BaseCore/Common/FactoryTools.h"
#include "BaseCore/Common/UICoordConfig.h"

USING_NS_CC; 

class TopBarLayer : public Layer
{
public:
	TopBarLayer(void);
	virtual ~TopBarLayer(void);

	virtual bool init();

	CREATE_FUNC(TopBarLayer);

	//按钮事件管理
	void onTouchSpritePrs(Ref* theBtn);
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch *touch, Event *pEvent);
	virtual void onTouchCancelled(Touch *touch, Event *pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
};

#endif

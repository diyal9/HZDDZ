/*
* FuncFoldMenuLayer.h
*	功能描述： 折叠菜单层
*  Created on: 2015年4月30日 下午2:09:51
*      Author: diyal.yin
*/
#ifndef _VIEW_LOBBYLAYER_FUNCFOLDMENULAYER_H_
#define _VIEW_LOBBYLAYER_FUNCFOLDMENULAYER_H_

#include "cocos2d.h"
#include <iostream>
#include "WWMacros.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"

USING_NS_CC; 

class WWTouchSprite;


class FuncFoldMenuLayer : public LayerColor
{
public:
	FuncFoldMenuLayer();
	virtual ~FuncFoldMenuLayer();

	CREATE_FUNC(FuncFoldMenuLayer);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch *touch, Event *pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);

	void touchFuncMenuBar();
	void controlFadeIn(Node* node, float time, int seq, Vec2 pos);
	void controlFadeOut(Node* node, float time);
	void TouchSpriteBtnEvent(Ref * node); //图标按钮事件

	void setVisableStatus();

private:
	CC_SYNTHESIZE(WWTouchSprite*, m_ptopMenuBtn, ptopMenuBtn);
	CC_SYNTHESIZE(Sprite*, s_menuBg, menuBg);
	bool isOpenMenu;

};
#endif /*_VIEW_LOBBYLAYER_FUNCFOLDMENULAYER_H_*/





































 
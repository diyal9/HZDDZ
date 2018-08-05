/**
 * ChatLayer.h
 * 聊天弹出菜单
 * created in 2015-6-4
 * by Jacky
 *
**/
#ifndef __HZ_DDZ_VIEW_CHATLAYER_CHATLAYER_H__
#define __HZ_DDZ_VIEW_CHATLAYER_CHATLAYER_H__

#include "cocos2d.h"
#include <iostream>
#include "WWMacros.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"

USING_NS_CC; 

enum MENU_CHATLAYERLAYER_BTN
{
	BTN_CHATLAYER_MENU_BTN_TAG = 1, //房间聊天按纽
	BTN_CHATLAYER_FUNCTAG_FIRST_BAG	//聊天展开菜单第一个按纽 TAG
};

class WWTouchSprite;

class ChatLayer : public LayerColor
{
public:
	ChatLayer();
	virtual ~ChatLayer();

	CREATE_FUNC(ChatLayer);
	virtual bool init();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch *touch, Event *pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);

	void touchFuncMenuBar();
	void onTouchSpriteBtnEvent(Ref * node); //图标按钮事件

	void talk(int type);
private:

	void controlFadeIn(Node* node, float time, int seq, Vec2 pos);
	void controlFadeOut(Node* node, float time);
	void onMenuFadeInAnimEnd(Node *theBtn); //子菜单弹出后操作
	void onMenuFadeOutAnimEnd(Node *theBtn);//子菜单收起后操作

	CC_SYNTHESIZE(WWTouchSprite*, m_pChatBtn, ChatBtn);
	bool isOpenMenu;

};

#endif // !__HZ_DDZ_VIEW_SETLAYER_SETLAYER_H__

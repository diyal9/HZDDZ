/**
 * GameSetMenu.h
 * 游戏中设置弹出菜单
 * created in 2015-6-15
 * by Toby
 *
**/
#ifndef __HZ_DDZ_VIEW_GAMESCENE_GAMESETMENU_H__
#define __HZ_DDZ_VIEW_GAMESCENE_GAMESETMENU_H__

#include "cocos2d.h"
#include <iostream>
#include "WWMacros.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"

USING_NS_CC; 

enum SET_MENU_TAG
{
	SUB_BTN_TAG_MAIN = 50, //主设置图标按钮
    SUB_BTN_TAG_TRUSTEE, //托管
    SUB_BTN_TAG_SET, //设置
    SUB_BTN_TAG_BACK //退出
};

class WWTouchSprite;

class GameSetMenu : public LayerColor
{
public:
	GameSetMenu();
	virtual ~GameSetMenu();

	CREATE_FUNC(GameSetMenu);
	virtual bool init();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);

	void btnStateCall();
	void subBtnCallBack(Ref * theBtn); //子按钮回调
	void setSubstituteEnable(bool isEnable);

private:

	CC_SYNTHESIZE(WWTouchSprite*, m_pSetBtn, pSetBtn); //设置按钮
	
	CC_SYNTHESIZE(Sprite*, m_pBg, pBg); //字按钮背景
	bool isOpenMenu;

};

#endif // !__HZ_DDZ_VIEW_GAMESCENE_GAMESETMENU_H__

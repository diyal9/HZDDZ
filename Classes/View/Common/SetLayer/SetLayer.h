/*! @file
********************************************************************************
<PRE>
模块名       :  基本Layer
文件名       :  SetLayer.h
文件实现功能 :  设置Layer
作者         :  Toby
版本         :  1.0
--------------------------------------------------------------------------------
备注         : 设置Layer
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/05/9   1.0      Toby                 新建
</PRE>
*******************************************************************************/
#ifndef __HZ_DDZ_VIEW_SETLAYER_SETLAYER_H__
#define __HZ_DDZ_VIEW_SETLAYER_SETLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "BaseCore/WWBGUI/WWFrameBase.h"

using namespace cocos2d::ui;


/**
 * 开关TAG
**/
enum SwitchTAG
{
	GAME_SOUNG_SWITCH = 1,
	BG_SOUNG_SWITCH,
	NO_SOUNG_SWITCH,
	SHOCK_SWITCH,
	ANMAITE_SWITCH,
	SCROLL_SWITCH,
	CONTIUN_SWITCH
};

enum TOUCH_TAG
{
	UPDATE_TAG = 100,
	KEFU_TAG
};

class SetLayer : public WWFrameBase
{
public:
	SetLayer();
	virtual ~SetLayer();
	virtual bool init(bool isFromLobby);
	virtual void onEnter();
	virtual void onExit();
	static SetLayer* create(bool isFromLobby = false);
	/*CREATE_FUNC(SetLayer);*/
	
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	//virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	//virtual void onTouchCancelled(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	void createView(bool isFromLobby);
	void gameSoundSetCallBack(Ref* sender, CheckBox::EventType controlEvent);
	void backgroundSetCallBack(Ref* sender, CheckBox::EventType controlEvent);
	void noSoundSetCallBack(Ref* sender, CheckBox::EventType controlEvent);
	void shockSetCallBack(Ref* sender, CheckBox::EventType controlEvent);
	void animateSetCallBack(Ref* sender, CheckBox::EventType controlEvent);
	void scrollSetCallBack(Ref* sender, CheckBox::EventType controlEvent);
	void continuSetCallBack(Ref* sender, CheckBox::EventType controlEvent);
	void onTouchSpritePree(cocos2d::Ref* theBtn);

private:
	CC_SYNTHESIZE(bool, m_pIsFormLobby,pIsFromLobby);
};

#endif // !__HZ_DDZ_VIEW_SETLAYER_SETLAYER_H__

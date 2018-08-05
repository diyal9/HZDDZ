/*! @file
********************************************************************************
<PRE>
模块名       :  基本Layer
文件名       :  GameOverLayer.h
文件实现功能 :  游戏结算Layer
作者         :  Toby
版本         :  1.0
--------------------------------------------------------------------------------
备注         : 游戏结算Layer
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/05/11   1.0      Toby                新建
</PRE>
*******************************************************************************/

#ifndef _VIEW_GAMESCENE_GAMEOVER_GAMEOVERLAYER_H_
#define _VIEW_GAMESCENE_GAMEOVER_GAMEOVERLAYER_H_

#include "cocos2d.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "Model/WWDataModel/WWGD/GD.h"
#include "Model/WWFilter/MsgLordGameFilter.h"
#include "BaseCore/WWBGUI/WWFrameBase.h"

USING_NS_CC;

using namespace ui;
enum 
{
	GAME_OVER_ANGIN = 1,  //再来一局
	GAME_OVER_OTHER
};


class GameOverLayer : public WWFrameBase
{
public:
	GameOverLayer();
	virtual ~GameOverLayer();

	//CREATE_FUNC(GameOverLayer);
	static GameOverLayer* create(cocos2d::Ref* objectData);
	virtual bool init(cocos2d::Ref* objectData);	
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch *touch, Event *pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);

	virtual void onEnter();
	virtual void onExit();

	void onDelayActionEnd();
	virtual void close(Ref * node);
	void createView(cocos2d::Ref* objectData);

	void onTouchSpritePree(cocos2d::Ref* theBtn);
	void updataTime(int dt = 15);
private:
	CC_SYNTHESIZE(int, m_times, Times);
	CC_SYNTHESIZE(Label*, m_pTime, mPTime);
	CC_SYNTHESIZE(int, m_beanChange, BeanChange);

};

#endif /*_VIEW_GAMESCENE_GAMEOVER_GAMEOVERLAYER_H_*/





































 
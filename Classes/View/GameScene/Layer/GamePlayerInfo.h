/*! @file
********************************************************************************
<PRE>
模块名       :  基本Layer
文件名       :  GamePlayerInfo.h
文件实现功能 :  游戏中玩家基本信息Layer
作者         :  Toby
版本         :  1.0
--------------------------------------------------------------------------------
备注         : 游戏中玩家基本信息Layer
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/06/10   1.0      Toby                新建
</PRE>
*******************************************************************************/

#ifndef _VIEW_GAMESCENE_LAYER_GamePlayerInfo_H_
#define _VIEW_GAMESCENE_LAYER_GamePlayerInfo_H_

#include "cocos2d.h"

USING_NS_CC;

enum
{
	PROPS_FLOWER_TAG = 1000, //花
	PROPS_PIJIU_TAG,         //啤酒
	PROPS_JIDAN_TAG,         //鸡蛋
	PROPS_BANZUAN_TAG        //板砖
};

class GamePlayerInfo : public LayerColor
{
public:
	GamePlayerInfo();
	virtual ~GamePlayerInfo();

	//CREATE_FUNC(GameOverLayer);
	/**
	 *传入位置  0为自己  1为下家 2为上家
	**/
	static GamePlayerInfo* create(int site);
	virtual bool init(int site);

	virtual void onEnter();
	virtual void onExit();

	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	
	void createView(int site);

	void onTouchSpriteDepress(Ref * theBtn);

private:
	CC_SYNTHESIZE(Sprite*, m_pBg, pBg);

};

#endif /*_VIEW_GAMESCENE_LAYER_GamePlayerInfo_H_*/





































 
/*
 * GameBgLayer.h
 *
 *
 *  Created on: 2015-5-11
 *      Author: diyal.yin
 */

#ifndef _VIEW_GAMELAYER_GAMETOPMENULAYER_H_
#define _VIEW_GAMELAYER_GAMETOPMENULAYER_H_

#include "cocos2d.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

enum SUB_BTN_TAG
{
	BTN_GAMETOPMENU_MULTIPLE_TAG = 10,
	BTN_GAMETOPMENU_BASE_TAG,
};

class GameTopMenuLayer : public LayerColor
{
public:
	GameTopMenuLayer();
	virtual ~GameTopMenuLayer();
	CREATE_FUNC(GameTopMenuLayer);

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void createView();
	void updateBase(int base);
	void updateMultiple(int multi);
	void setSubstituteEnable(bool isEnable);


	void updateAllStatu(float dt=0.0f);
	//更新时间状态
	void updateTimeStatu();
	//更新网络状态上
	void updateNetStatu();
	//更新电池状态
	void updateBatteryStatu();
	//显示任务区域
	void showKeyWin();
	//隐藏任务区域
	void unShowKeyWin();
private:
	CC_SYNTHESIZE(Sprite*, m_pBgSprite, pBgSprite); //背景
	cocos2d::ui::LoadingBar* m_pBatteryStatu;//电量百分比
	Label* m_pTimeStatu;//时间
	Sprite* m_pNetStatus;//网络强度
	Label* m_pWinMax;//赢足
	Sprite* m_pKey;//钥匙icon
	Label* m_pKeyNum;//钥匙数量
};

#endif /*_VIEW_GAMELAYER_GAMETOPMENULAYER_H_*/

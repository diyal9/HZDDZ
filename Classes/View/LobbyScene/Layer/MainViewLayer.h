#ifndef _VIEW_LOBBYSCENE_LAYER_MAINVIEWLAYER_h
#define _VIEW_LOBBYSCENE_LAYER_MAINVIEWLAYER_h

#include "cocos2d.h"
#include <iostream>

#include "BaseCore/Common/FactoryTools.h"
#include "BaseCore/Common/UICoordConfig.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"
#include "View/LobbyScene/Layer/Scroller.h"
#include "View/LobbyScene/Layer/TopBarLayer.h"
#include "View/LobbyScene/Layer/BottomBarLayer.h"
#include "View/LobbyScene/Layer/MainViewLayer.h"
#include "View/LobbyScene/Layer/FuncFoldMenuLayer.h"

USING_NS_CC; 

class TopBarLayer;
class BottomBarLayer;
class FuncFoldMenuLayer;

class MainViewLayer : public Layer
{
public:
	MainViewLayer(void);
	virtual ~MainViewLayer(void);

	CREATE_FUNC(MainViewLayer);
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch *touch, Event *pEvent);
	virtual void onTouchCancelled(Touch *touch, Event *pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
	
	void createForwardLayer(); //前景
	void createCentreLayer();  //中景
	void createBehindLayer();  //后景
    void setScrollInit();  //初始化设置Scroller
	void createFuncMenuFrame(); //功能按钮内容创建

	void createRoomLabel(); //创建房间标签

	void TouchSpriteBtnEvent(Ref * node); //图标按钮事件
	void ButtonDepress(Ref * theBtn);     //按钮事件管理
	void IsClickRoom(Vec2 touchPoint);    //选中房间

	void CloudMovement(float delta); //云层运动

	void ClickRoomMusic();

private:
	CC_SYNTHESIZE(Scroller*, m_pScroller, pScroller);	//Scroller
	CC_SYNTHESIZE(Layer*, m_pBottomLayer, pBottomLayer); //视差层 后景
	CC_SYNTHESIZE(Layer*, m_pCenterLayer, pCenterLayer); //视差层 中景
	CC_SYNTHESIZE(Layer*, m_pScrollLayer, pScrollLayer); //视差层 前景
	CC_SYNTHESIZE(Layer*, m_pTouchLayer, pTouchLayer);	//顶部功能按钮层

	virtual void update(float delta); //update为scheduleUpdate定时器的刷新函数体.

	std::vector<Sprite*> cloudVector; //存放云图片

	int scrollPos;
	int scrollWidth; //滑动屏幕宽度
	
	TopBarLayer *m_pTopBar;
	BottomBarLayer *m_pBottomBar;
};

#endif

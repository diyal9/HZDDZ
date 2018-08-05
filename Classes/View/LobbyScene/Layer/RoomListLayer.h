#ifndef _VIEW_LOBBYLAYER_ROOMLISTLAYER_H_
#define _VIEW_LOBBYLAYER_ROOMLISTLAYER_H_

#include "cocos2d.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "Model/WWDataModel/WWGD/GD.h"

USING_NS_CC;

using namespace ui;

enum BTN_TAG
{
	ROOMLIST_BTN_CLOSE = 1, //房间列表关闭按钮
	ROOMLIST_BTN_QUICKSTART, //快速开始
	ROOMLIST_BTN_BOX,		//宝箱
	ROOMLIST_BTN_PEOPLE,	//定人赛
	ROOMLIST_BTN_TIME		//定时赛
}; 

class RoomListLayer : public LayerColor
{
public:
	RoomListLayer();
	virtual ~RoomListLayer();

	//CREATE_FUNC(RoomListLayer);
	static RoomListLayer* create(int playGameId);
	virtual bool init(int playGameId);	
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch *touch, Event *pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);

	virtual void onEnter();
	virtual void onExit();

	void createView(int playGameId);
	Layout* createItem(GameRoom* roomData, int roodId);
	
	Layout* createMatchItem(MatchRoom* roomData1, int roomId1, MatchRoom* roomData2, int roomId2);

	void reflashListView(); //刷新listview内容
	
	void ButtonDepress(Ref * theBtn);  // menu按钮事件管理

	void TouchSpriteBtnEvent(Ref * node); //图标按钮事件

	void listItemClick(Ref* pSender, ListView::EventType type);

	//实现网络回调的虚函数
	//virtual void onDataReceiver(int msgType, cocos2d::Ref* objectData );

private:
	CC_SYNTHESIZE(ListView* ,m_pListView, pListView);
	CC_SYNTHESIZE(int ,m_pPlayerTypeID, pPlayerTypeID); //玩法ID
	cocos2d::__Array* rooms;
	int curMatchListTag;
};

#endif /*_VIEW_LOBBYLAYER_ROOMLISTLAYER_H_*/





































 
/*! @file
********************************************************************************
<PRE>
模块名       :  基本Layer
文件名       :  ChargeLayer.h
文件实现功能 :  购买金币钻石Layer
作者         :  Toby
版本         :  1.0
--------------------------------------------------------------------------------
备注         : 购买金币钻石Layer
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/05/11   1.0      Toby                新建
</PRE>
*******************************************************************************/
#ifndef __HZ_DDZ_VIEW_Common_ChargeLayer_ChargeLayer_H__
#define __HZ_DDZ_VIEW_Common_ChargeLayer_ChargeLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "BaseCore/WWBGUI/WWFrameBase.h"

using namespace cocos2d::ui;


class ChargeLayer : public WWFrameBase
{
public:
	ChargeLayer();
	virtual ~ChargeLayer();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	CREATE_FUNC(ChargeLayer);
	
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchCancelled(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	void createView();
	Layout* createItem();
	void reflashListView();
	void listItemClick(Ref* pSender, ListView::EventType type);

	void itemCallBack(Ref * node);
private:
	float vipLv100; //距离下一等级VIP所达到的百分百
	bool isJinbi;//是否是金币充值
	CC_SYNTHESIZE(ListView*, m_pListView, pListView);
};

#endif // !__HZ_DDZ_VIEW_Common_ChargeLayer_ChargeLayer_H__

/**
 * LoadingLayer.h
 *
 * created in 2015-6-3
 * by Diyal.yin
 *
**/
#ifndef __HZ_DDZ_VIEW_COMMDIALOG_LOADINGLAYER_H__
#define __HZ_DDZ_VIEW_COMMDIALOG_LOADINGLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

class LoadingLayer : public LayerColor
{
public:
	LoadingLayer();
	virtual ~LoadingLayer();

	CREATE_FUNC(LoadingLayer);
	virtual bool init();	

	virtual void onEnter();
	virtual void onExit();

	/*** back按钮监听 ****/
	virtual void setBackKeyEnabled(bool enable){};

    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

private:

};

#endif // !__HZ_DDZ_VIEW_COMMDIALOG_LOADINGLAYER_H__

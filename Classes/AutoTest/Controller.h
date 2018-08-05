/*
 * TestScene.h
 *
 *	UnitTest 自动化测试
 *  Created on: 2015-6-16
 *      Author: diyal.yin
 *  自动化测试设计方案：构建一个模拟运行测试环境。程序会按照一定的流程跑下去。
 *  鉴于游戏比较小，就按照代码流程走。如后期需要可以根据配置文件或者脚本去跑。
 */
#ifndef _TESTSCENE_CONTROLLER_H_
#define _TESTSCENE_CONTROLLER_H_

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class Controller : public Layer
{
public:
	Controller();
	virtual ~Controller();
    
	virtual bool init();

	CREATE_FUNC(Controller);

	virtual void onEnter();
	
	virtual void onExit();

	void autoOpenLayer(int times, Layer* targetLayer); //连续打开一个Layer多少次

	void TouchSpriteBtnEvent(Ref * node); //图标按钮事件

	void onMouseScroll(Event *event);
	void addConsoleAutoTest();
	void autorun();
	void startAutoRun();
	void runAllTests(int fd);
	ssize_t readline(int fd, char* ptr, size_t maxlen);

private:
	Layer* nextTestLayer();

private:
	Vec2 _beginPos;
	Menu* _itemMenu;
	bool _exitThread;
};

#endif //_TESTSCENE_CONTROLLER_H_
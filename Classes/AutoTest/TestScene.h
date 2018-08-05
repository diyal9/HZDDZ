/*
 * TestScene.h
 *
 *	UnitTest 自动化测试
 *  Created on: 2015-6-16
 *      Author: diyal.yin
 *  自动化测试设计方案：构建一个模拟运行测试环境。程序会按照一定的流程跑下去。
 *  鉴于游戏比较小，就按照代码流程走。如后期需要可以根据配置文件或者脚本去跑。
 */
#ifndef _TESTSCENE_TESTSCENE_H_
#define _TESTSCENE_TESTSCENE_H_

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class TestScene : public Scene 
{
public:
	TestScene();
	virtual ~TestScene();
    
	virtual bool init();

	CREATE_FUNC(TestScene);

	virtual void onEnter();
	
	virtual void onExit();

	void autoOpenScene(float dt);

	void TouchSpriteBtnEvent(Ref * node); //图标按钮事件

private:
    float accum;
	int nCountIndex; //当前执行次数

};

#endif //_TESTSCENE_TESTSCENE_H_
/*
 * TestScene.h
 *
 *	UnitTest 自动化测试
 *  Created on: 2015-6-16
 *      Author: diyal.yin
 */
#include "Controller.h"
#include "AutoTest/TestScene.h"
#include "View/LoginScene/LoginScene.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"

#include "View/LobbyScene/Layer/TopBarLayer.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#else
#include <io.h>
#include <WS2tcpip.h>
#endif

typedef struct _Controller{
	const char *test_name;
	std::function<Layer()> callback;
} ControllerType;

//测试的Layer集合
//ControllerType g_aTestNames[] = 
//{ 
//	{ "TopBarLayer", {return TopBarLayer::create(); } }
//};


Controller::Controller()
{
}

Controller::~Controller()
{ 
}


bool Controller::init()
{
	bool bRet = false;
	do
	{
		
		//auto noticeBtn = WWTouchSprite::createWithFile("res/Images/Lobby/sysNoticeBtn.png", this, callfuncO_selector(Controller::TouchSpriteBtnEvent));
		//noticeBtn->setPosition(Vec2(100, 100));
		//addChild(noticeBtn);

		bRet = true;

	}while (0);
	log("end of loading res");

	return bRet;
}

void Controller::autoOpenLayer(int times, Layer* targetLayer)
{

}

void Controller::TouchSpriteBtnEvent(Ref * theBtn)
{

}

void Controller::onEnter()
{
    Layer::onEnter();
}

void Controller::onExit()
{
	Layer::onExit();
}

static int layerIdx = -1; 
Layer* nextTestLayer()
{
	layerIdx++;

	auto layer = Layer::create();
	return layer;
}

/*
 * TestScene.h
 *
 *	UnitTest 自动化测试
 *  Created on: 2015-6-16
 *      Author: diyal.yin
 */
#include "TestScene.h"
#include "AutoTest/Controller.h"
#include "View/LoginScene/LoginScene.h"
#include "VIew/LobbyScene/LobbyScene.h"

#include "Control/WWCSound/WWSoundManager.h"
#include "BaseCore/WWBGUI/WWToast.h"
#include "BaseCore/WWBLog/LogUtil.h"

#include "View/EffectView/EffectComponent.h"
#include "BaseCore/WWEffectBase/WWAnimation.h"

#include "View/GameScene/Component/NpcPlayer.h"

USING_NS_CC;

TestScene::TestScene() : nCountIndex(0),
	accum(0)
{
}

TestScene::~TestScene()
{ 
}


bool TestScene::init()
{
	bool bRet = false;
	do
	{
		//auto layer = Controller::create();
		//layer->setPosition(Vec2(0, 0));
		//addChild(layer);

		//创建一个TouchSprite
		//auto noticeBtn = WWTouchSprite::createWithFile("res/Images/Lobby/sysNoticeBtn.png", this, callfuncO_selector(TestScene::TouchSpriteBtnEvent));
		//noticeBtn->setPosition(Vec2(100, 100));
		//addChild(noticeBtn);

		//初始化的时候，开启回调
		//scheduleOnce(CC_SCHEDULE_SELECTOR(TestScene::autoOpenScene), 2.0f);

		//schedule(CC_SCHEDULE_SELECTOR(TestScene::autoOpenScene), 5.0f);


		//落叶效果
		//auto lay = LeafFallingLayer::create();
		//lay->setPosition(Vec2(0, 0));
		//addChild(lay);

		////飞机
		//auto lay = FlyChickenLayer::create();
		//lay->setPosition(Vec2(0, 0));
		//addChild(lay);

		//火箭
		//auto lay = RocketLayer::createWith(1, 2);
		//lay->setPosition(Vec2(0, 0));
		//addChild(lay);

		//炸弹效果
		//auto lay = BombsLayer::createWithSite(1, 2,-1);
		//lay->setPosition(Vec2(0, 0));
		//addChild(lay);

		//NPC
		auto npc = NpcPlayer::create(true);
		npc->setPosition(Vec2(0, 0));
		npc->changeNpcInfo(true);
		addChild(npc);

		auto npc1 = NpcPlayer::create(false);
		npc1->setPosition(Vec2(0, 0));
		npc1->changeNpcInfo(true);
		addChild(npc1);

		bRet = true;

	}while (0);

	return bRet;
}

void TestScene::onEnter()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

    Scene::onEnter();
}

void TestScene::onExit()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	Scene::onExit();

}

void TestScene::autoOpenScene(float dt)
{
	nCountIndex++;
	accum += dt;
	log("Times on: %d,  %f", nCountIndex, accum);

	//auto m_LoginScene = LoginScene::create();
	//Director::getInstance()->replaceScene(m_LoginScene);

	//auto s_Lobby = LobbyScene::create();
	//Director::getInstance()->replaceScene(s_Lobby);

	auto lay = RocketLayer::createWith(1, 2);
	lay->setPosition(Vec2(0, 0));
	addChild(lay);

}

void TestScene::TouchSpriteBtnEvent(Ref * theBtn)
{
	log("Had Touch Sprite");

	//WWToast::makeText("Had Touch Sprite", 5.0f);

	//TestLog
	wwlog(LOG_ERROR, __FILE__, "This is a test of Log");
}
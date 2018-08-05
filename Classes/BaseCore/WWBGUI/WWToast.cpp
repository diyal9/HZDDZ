/*! @file
********************************************************************************
<PRE>
模块名       :  基本控件
文件名       :  WWToast.cpp
文件实现功能 :  Toast
作者         :  Diyal.yin
版本         :  1.0
Copyright (c)  WaWaGame Technologies
--------------------------------------------------------------------------------
Toast 仿照Android的Toast实现方式。分为默认和传递时间的两个函数
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/06/22   1.0      diyal.yin           新建
</PRE>
*******************************************************************************/

#include "WWToast.h"
#include "WWMacros.h"

USING_NS_CC;

const int g_tostTag = 123456;
WWToast::WWToast()
{
}

WWToast::~WWToast()
{
    
}

void WWToast::makeText(const std::string &text, float time)
{
	do {

		Size size = CUR_WIN_SIZE;

		auto label = Label::create();
		label->setString(text);
		label->setSystemFontSize(36.0f);
		label->setColor(Color3B::WHITE);

		if (label->getContentSize().width > size.width * 3 / 5)
		{
			label->setDimensions(size.width * 3 / 5, 0);
		}

		auto root = LayerColor::create(Color4B(0,0,0,204));
		root->setTag(g_tostTag);
		root->setContentSize(Size(label->getContentSize().width + 160,label->getContentSize().height+20));
		root->setColor(Color3B::BLACK);
		root->setPosition(Vec2(size.width/2-root->getContentSize().width * 0.5, size.height * 0.5));

		label->setPosition(Vec2(root->getContentSize().width/2,root->getContentSize().height/2));
		root->addChild(label);

		Director::getInstance()->getRunningScene()->addChild(root, BASIC_ZORDER_TOP * 2);

		//动画
		auto moveBy = MoveBy::create(0.08f, Vec2(0, root->getPositionY() * 0.1f));
		Action* _action = Sequence::create(moveBy, DelayTime::create(time),
			RemoveSelf::create(true),
			nullptr);
		root->runAction(_action);

	} while (0);

}

cocos2d::Node* WWToast::createToast(const std::string &text, float time)
{
	
	do {

		Size size = CUR_WIN_SIZE;

		auto label = Label::create();
		label->setString(text);
		label->setSystemFontSize(36.0f);
		label->setColor(Color3B::WHITE);

		if (label->getContentSize().width > size.width * 3 / 5)
		{
			label->setDimensions(size.width * 3 / 5, 0);
		}

		auto root = LayerColor::create(Color4B(0,0,0,204));
		root->setTag(g_tostTag);
		root->setContentSize(Size(label->getContentSize().width + 160,label->getContentSize().height+20));
		root->setColor(Color3B::BLACK);
		root->setPosition(Vec2(size.width/2-root->getContentSize().width * 0.5, size.height * 0.5 - 150));

		label->setPosition(Vec2(root->getContentSize().width/2,root->getContentSize().height/2));
		root->addChild(label);

		return root;

	} while (0);

    return nullptr;
}


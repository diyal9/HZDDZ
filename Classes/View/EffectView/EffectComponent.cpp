#include "EffectComponent.h"
#include "WWMacros.h"
#include "BaseCore/WWEffectBase/WWAnimation.h"
#include "View/GameScene/GameLogicManager.h"
#include "BaseCore/WWEffectBase/WWCircleAction.h"
#include "BaseCore/WWEffectBase/WWShake.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "ThirdPlatform/PlatformUtils.h"
#endif

static const char* spring_plist   = "res/Images/Animation/springAnimation.plist";
static const char* flyChicken_plist   = "res/Images/Animation/flyChicken.plist";
static const char* rocketFrame_plist   = "res/Images/Animation/rocketFrame.plist";

LeafFallingLayer::LeafFallingLayer()
{
}

LeafFallingLayer::~LeafFallingLayer()
{ 
}


bool LeafFallingLayer::init()
{
	bool bRet = false;
	do
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(spring_plist);

		srand((unsigned)time(nullptr)); //设置随机数种子按照时间分布

		int Tag = 100;
		char str[100] = {0};
		for (unsigned int i = 0; i < 5; i++)
		{
			int iRandPosX = rand() % 400  * i + CUR_SCREEN_SIZE.width * 0.4;

			sprintf(str, "img_yezi_%d.png",i + 1);
			auto spriteLeaf1 = Sprite::createWithSpriteFrameName(str);
			spriteLeaf1->setRotation(30);//旋转角度
			spriteLeaf1->setAnchorPoint(Vec2(0.5, 3));//设置精灵锚点
			spriteLeaf1->setPosition(Vec2(iRandPosX, CUR_SCREEN_SIZE.height * 0.7));//叶子1第一次初始位置
			spriteLeaf1->setScale(1.0/0.8);//设置叶片大小

			Tag = Tag + i;
			this->addChild(spriteLeaf1,Tag,Tag);
			this->playLeafAnim(spriteLeaf1);//调用play函数播实现叶动作
		}

		auto bg = Sprite::createWithSpriteFrameName("springBg.png");
		bg->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5f, CUR_SCREEN_SIZE.height * 0.6f));
		bg->setScale(1.0f/0.8f);
		this->addChild(bg);

		bg->setOpacity(0);

		auto _easeIn = FadeIn::create(2.0f);
		auto _action = Sequence::create(/*DelayTime::create(1.0),*/_easeIn,nullptr);
		bg->runAction(_action);

		auto ani = WWAnimation::getInstance()->animationUnCache("X2", 5, 0.15f, 1);
		auto boNode = WWAnimation::getInstance()->createAnimationSprite("X2");
		boNode->setPosition(Vec2(CUR_SCREEN_SIZE.width*0.5f,CUR_SCREEN_SIZE.height*0.5f));
		bg->addChild(boNode, BASIC_ZORDER_ROOT);
		boNode->runAction(Animate::create(ani));

		bRet = true;

	}while (0);

	return bRet;
}

//叶子飘落动作
void LeafFallingLayer::playLeafAnim(Sprite *spriteLeaf)
{
	int iTag = spriteLeaf->getTag();

	int downTime = 3.0; //落叶持续时间（整个动画也是在这个时间结束）

	float fromtime, roTime, fAngle1, fAngle2;  //叶子下落的时间, 叶子单向摆动一次时间，叶子逆时针摆动角度，顺时针摆动角度 (fromtime最小的那个执行删除)

	if (iTag == 100)
	{
		fromtime= downTime + 0.1f;//叶子下落的时间
		roTime = 1.5f;//叶子单向摆动一次时间
		fAngle1 = -80;//叶子逆时针摆动角度
		fAngle2 = 80;//顺时针摆动角度
	}
	else if(iTag == 101)
	{
		fromtime= downTime + 0.2f;
		roTime = 1.3f;
		fAngle1 = -100;
		fAngle2 = 100;
	}
	else if(iTag == 102)
	{
		fromtime= downTime + 0.3f;
		roTime = 2.1f;
		fAngle1 = -100;
		fAngle2 = 100;
	}
	else if(iTag == 103)
	{
		fromtime= downTime + 0.4f;
		roTime = 1.1f;
		fAngle1 = -100;
		fAngle2 = 90;
	}
	else
	{
		fromtime= downTime + 0.5f;
		roTime = 2.2f;
		fAngle1 = -90;
		fAngle2 = 100;
	}


	//随机生成叶子横向偏移值
	int iRandPos = rand() % 200 + 100;
	int iRandPosY = rand() % 100;
	//叶子所运动到的位置
	auto moveTo = MoveTo::create(fromtime, Vec2(CUR_SCREEN_SIZE.width * 0.5f - iRandPos, CUR_SCREEN_SIZE.height * 0.6f - iRandPosY));
	auto actDone = CallFuncN::create(CC_CALLBACK_1(LeafFallingLayer::resetLeafPos,this));
	auto putdown = Sequence::create(moveTo, actDone, nullptr);
	//叶子旋转动作
	auto rotaBy1 = RotateBy::create(roTime, fAngle1);
	auto rotaBy2 = RotateBy::create(roTime, fAngle2);

	//叶子翻转动作
	spriteLeaf->setPositionZ(60);//设置深度抬高60，避免出现使用CCOrbitCamera实现空间翻转时产生错位和遮挡等问题(原来没有设置，出现翻转乱掉的情况)
	//CCDirector::sharedDirector()->setDepthTest(false);
	//关闭深度测试同样可以避免上述问题，不过，推荐使用深度设置setVertexZ来正确解决，因为有时你可能需要遮挡的效果，关闭深度测试后将造成遮挡效果的缺失
	auto orbit = OrbitCamera::create(8, 1, 0, 0, 360, 45, 0);
	//让树叶精灵始终执行三维翻转的动作
	auto fz3d = Repeat::create(orbit, -1);//无限循环执行叶片翻转的动作
	//CCRepeatForever *fz3d = CCRepeatForever::actionWithAction(orbit);
	//由于下面使用CCSpawn同时执行动作，所以不可以使用无限次数类型的动作，而因使用有线次数循环CCRepeat将循环次数设置为-1

	//用CCEaseInOut包装落叶摆动的动作，让树叶的进入、出现更自然（淡入淡出效果）
	auto ease1 = EaseInOut::create(rotaBy1, 3);
	auto ease2 = EaseInOut::create(rotaBy2, 3);
	//摆动动作合成
	auto seq2 = Sequence::create(ease1, ease2, nullptr);//依次执行顺时针、逆时针摆动
	auto baidong = Repeat::create(seq2, -1);//摆动合成

	//动作执行->同时执行所有动作
	spriteLeaf->runAction(Spawn::create(putdown, baidong, fz3d, nullptr));

}

//重置叶子的位置
void LeafFallingLayer::resetLeafPos(Node* sender)
{
	this->removeFromParent();
}

void LeafFallingLayer::onEnter()
{
	Layer::onEnter();
}

void LeafFallingLayer::onExit()
{
	Layer::onExit();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(spring_plist);
}


/* 飞鸡动画 */
FlyChickenLayer::FlyChickenLayer():
	p_fromSite(0)
{
}

FlyChickenLayer::~FlyChickenLayer()
{ 
}

FlyChickenLayer* FlyChickenLayer::createWithSite(int fromSite)
{
	FlyChickenLayer* pItem = new FlyChickenLayer();
	pItem->setpFromSite(fromSite);
	if(pItem && pItem->init())
	{
		pItem->autorelease();
		return pItem;
	}
	else
	{
		CC_SAFE_DELETE(pItem);
		return NULL;
	}
}


bool FlyChickenLayer::init()
{
	if(!LayerColor::initWithColor(Color4B(255,255,255,0)))
	{
		return false;
	}

	bool bRet = false;
	do
	{
		auto ani = WWAnimation::getInstance()->animationUnCache("flyChicken",2,0.1f,1);
		auto node = WWAnimation::getInstance()->createAnimationSprite("flyChicken");
		node->setPosition(Vec2(515, 800));
		node->setFlippedX(true);
		if (p_fromSite == 1)//右边出飞机
		{
			node->setFlippedX(false);
			node->setPosition(Vec2(CUR_SCREEN_SIZE.width - 117 - node->getContentSize().width,934));
		}
		else if (p_fromSite == 2)//左边出飞机
		{
			node->setPosition(Vec2(117 + node->getContentSize().width,934));
		}
		this->addChild(node, BASIC_ZORDER_ROOT, 100);
		
		node->runAction(RepeatForever::create(Animate::create(ani)));
		if (p_fromSite ==0)
		{

			auto shander = Sprite::create("res/Images/Animation/chickenShander.png");
			shander->setPosition(Vec2(node->getContentSize().width * 0.5,  - shander->getContentSize().height ));
			node->addChild(shander);

			auto action = JumpTo::create(0.4f, Vec2(CUR_SCREEN_SIZE.width * 0.5, 800), 70, 1);
			auto delay = DelayTime::create(0.6f);
			auto actDone = CallFuncN::create(CC_CALLBACK_0(FlyChickenLayer::callBackEndAction,this));

			auto seq = Sequence::create(EaseInOut::create(action, 1), delay, actDone, nullptr);
			node->runAction(seq);
		}
		else if (p_fromSite ==1)//右边出飞机
		{
			auto moveBy = MoveBy::create(0.5f,Point(-400,0));
			auto actDone = CallFuncN::create(CC_CALLBACK_0(FlyChickenLayer::removeSelf,this));
			node->runAction(Sequence::create(moveBy,actDone,nullptr));
		}
		else
		{
			auto moveBy = MoveBy::create(0.5f,Point(400,0));			
			auto actDone = CallFuncN::create(CC_CALLBACK_0(FlyChickenLayer::removeSelf,this));
			node->runAction(Sequence::create(moveBy,actDone,nullptr));
		}
		
		

		bRet = true;

	}while (0);

	return bRet;
}

void FlyChickenLayer::callBackEndAction()
{
	auto actionJump2 = JumpTo::create(0.5f, Vec2(CUR_SCREEN_SIZE.width * 0.8, 800), 100, 1);
	auto scaleTo = ScaleTo::create(0.5f, 2.0f);
	auto actDone = CallFuncN::create(CC_CALLBACK_0(FlyChickenLayer::removeSelf,this));

	auto seq1 = Sequence::create(actionJump2, nullptr);
	auto seq2 = Sequence::create(scaleTo, actDone, nullptr);
	this->getChildByTag(100)->runAction(Spawn::create(seq1, seq2, nullptr));
}

void FlyChickenLayer::removeSelf()
{
	this->removeFromParent();
}

void FlyChickenLayer::onEnter()
{
	Layer::onEnter();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(flyChicken_plist);
}

void FlyChickenLayer::onExit()
{
	Layer::onExit();
	WWAnimation::getInstance()->removeAnimationCache("flyChicken");
}

/* 火箭动画 */
RocketLayer::RocketLayer() : 
	iCurSignIndex(0),
	iCurSignIndex1(0),
	p_site(0),
	p_site2(0)
{
}

RocketLayer::~RocketLayer()
{ 
}

RocketLayer* RocketLayer::createWith(int toSite, int toSite2)
{
	RocketLayer* pItem = new RocketLayer();
	pItem->setpSite(toSite);
	pItem->setpSite2(toSite2);
	if(pItem && pItem->init())
	{
		pItem->autorelease();
		return pItem;
	}
	else
	{
		CC_SAFE_DELETE(pItem);
		return NULL;
	}
}

bool RocketLayer::init()
{
	if(!LayerColor::initWithColor(Color4B(255,255,255,0)))
	{
		return false;
	}

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(rocketFrame_plist);

	srand((unsigned)time(nullptr)); //设置随机数种子按照时间分布

	bool bRet = false;
	do
	{
		auto sR1 = Sprite::createWithSpriteFrameName("rocket.png");
		sR1->setPosition(Vec2(CUR_SCREEN_SIZE.width, CUR_SCREEN_SIZE.height * 0.5));
		addChild(sR1, BASIC_ZORDER_TOP + 1);

		auto sR2 = Sprite::createWithSpriteFrameName("rocket.png");
		sR2->setPosition(Vec2(CUR_SCREEN_SIZE.width + sR2->getContentSize().width, CUR_SCREEN_SIZE.height * 0.7));
		addChild(sR2, BASIC_ZORDER_TOP);

		auto sR3 = Sprite::createWithSpriteFrameName("rocket.png");
		sR3->setPosition(Vec2(CUR_SCREEN_SIZE.width + sR3->getContentSize().width, CUR_SCREEN_SIZE.height * 0.3));
		addChild(sR3, BASIC_ZORDER_TOP);


		//阶段一 (这里代码不大整洁，可以修改成循环创建)
		auto sr1_MoveTo = MoveTo::create(0.5f, Vec2(CUR_SCREEN_SIZE.width * 0.5, sR1->getPositionY()));
		auto move_ease_out1 = EaseExponentialOut::create(sr1_MoveTo); //缓慢结束
		auto delay = DelayTime::create(0.2f);
		auto sr1_MoveTo2 = MoveTo::create(0.1f, Vec2(0 - sR1->getContentSize().width, sR1->getPositionY()));
		auto move_ease_out2 = EaseExponentialOut::create(sr1_MoveTo2);
		sR1->runAction(Sequence::create(move_ease_out1,delay,move_ease_out2, nullptr));

		auto sr2_MoveTo = MoveTo::create(0.5f, Vec2(CUR_SCREEN_SIZE.width * 0.55, sR2->getPositionY()));
		auto move_ease_out3 = EaseExponentialOut::create(sr2_MoveTo); //缓慢结束
		auto delay2 = DelayTime::create(0.3f);
		auto sr1_MoveTo4 = MoveTo::create(0.6f, Vec2(0 - sR2->getContentSize().width, sR2->getPositionY()));
		auto move_ease_out4 = EaseExponentialOut::create(sr1_MoveTo4);
		auto actDone = CallFuncN::create(CC_CALLBACK_0(RocketLayer::stepTwo,this));
		sR2->runAction(Sequence::create(move_ease_out3,delay2,move_ease_out4, actDone, nullptr));

		auto sr3_MoveTo = MoveTo::create(0.5f, Vec2(CUR_SCREEN_SIZE.width * 0.53, sR3->getPositionY()));
		auto move_ease_out5 = EaseExponentialOut::create(sr3_MoveTo); //缓慢结束
		auto delay3 = DelayTime::create(0.15f);
		auto sr1_MoveTo6 = MoveTo::create(0.6f, Vec2(0 - sR3->getContentSize().width, sR3->getPositionY()));
		auto move_ease_out6 = EaseExponentialOut::create(sr1_MoveTo6);
		sR3->runAction(Sequence::create(move_ease_out5,delay3,move_ease_out6, nullptr));

		bRet = true;

	} while (0);

	return bRet;
}

void RocketLayer::stepTwo()
{
	for (unsigned int i = 1; i <= 3; i++)
	{
		auto delay = DelayTime::create(0.1f * i);
		auto callCreateFunc = CallFuncN::create(CC_CALLBACK_1(RocketLayer::frontsight,this));
		this->runAction(Sequence::create(delay, callCreateFunc, nullptr));
	}
	
	if (p_site2 != -1)
	{
		for (unsigned int i = 1; i <= 3; i++)
		{
			auto delay = DelayTime::create(0.1f * i);
			auto callCreateFunc = CallFuncN::create(CC_CALLBACK_1(RocketLayer::frontsight2,this));
			this->runAction(Sequence::create(delay, callCreateFunc, nullptr));
		}
	}
	

}

void RocketLayer::frontsight(Node* node)
{	
	iCurSignIndex++;
	Point basePoint = Vec2(117, 954); //基本坐标，参照人物的坐标来定，让弹坑尽量不贴合在一起
	if (p_site == 1)//炸右边玩家
	{
		basePoint = Vec2(CUR_SCREEN_SIZE.width - 117, 954);
	}
	else if (p_site == 0)//自己被炸
	{
		basePoint = Vec2(127, 462);
	}

	auto frontsightTmp = Sprite::createWithSpriteFrameName("frontsight.png");
	if (iCurSignIndex == 3) //第三颗固定放在头像上
	{
		frontsightTmp->setPosition(basePoint);
	}
	else
	{
		int iRandPosX = rand() % 80;
		int iRandPosY = rand() % 80;
		Point randomPoint;
		if (iCurSignIndex == 2) //第二颗炸弹位置相对固定设置在随机的右上角
		{
			if (p_site == 0)
			{
				randomPoint = Vec2(basePoint.x + frontsightTmp->getContentSize().width +  iRandPosX  , basePoint.y + iRandPosY );
			}
			else if (p_site == 1)//右边玩家 第二颗和第三课 是往左边偏移的
			{
				randomPoint = Vec2(basePoint.x - frontsightTmp->getContentSize().width -  iRandPosX  , basePoint.y - iRandPosY );
			}
			else
			{
				randomPoint = Vec2(basePoint.x + frontsightTmp->getContentSize().width +  iRandPosX  , basePoint.y - iRandPosY );
			}
		}
		else
		{
			if (p_site == 0)
			{
				randomPoint = Vec2(basePoint.x + iRandPosX, basePoint.y + frontsightTmp->getContentSize().height + iRandPosY);
			}
			else if (p_site == 1)//右边玩家 第二颗和第三课 是往左边偏移的
			{
				randomPoint = Vec2(basePoint.x - iRandPosX, basePoint.y - frontsightTmp->getContentSize().height - iRandPosY);
			}
			else
			{
				randomPoint = Vec2(basePoint.x + iRandPosX, basePoint.y - frontsightTmp->getContentSize().height - iRandPosY);
			}
		}
		frontsightTmp->setPosition(randomPoint);
		log("Point %f, %f",randomPoint.x, randomPoint.y);
	}
	this->addChild(frontsightTmp);

	auto delay = DelayTime::create(0.3f);
	auto callNextStep1 = CallFuncN::create(CC_CALLBACK_1(RocketLayer::rocketMoveInBack,this));
	frontsightTmp->runAction(Sequence::create(delay, callNextStep1,nullptr));

	
}

void RocketLayer::frontsight2(Node* node)
{	
	iCurSignIndex1++;
	Point basePoint = Vec2(117, 954); //基本坐标，参照人物的坐标来定，让弹坑尽量不贴合在一起
	if (p_site2 == 1)//炸右边玩家
	{
		basePoint = Vec2(CUR_SCREEN_SIZE.width - 117, 954);
	}
	else if (p_site2 == 0)//自己被炸
	{
		basePoint = Vec2(127, 462);
	}

	auto frontsightTmp = Sprite::createWithSpriteFrameName("frontsight.png");
	if (iCurSignIndex1 == 3) //第三颗固定放在头像上
	{
		frontsightTmp->setPosition(basePoint);
	}
	else
	{
		int iRandPosX = rand() % 80;
		int iRandPosY = rand() % 80;
		Point randomPoint;
		if (iCurSignIndex1 == 2) //第二颗炸弹位置相对固定设置在随机的右上角
		{
			if (p_site2 == 0)
			{
				randomPoint = Vec2(basePoint.x + frontsightTmp->getContentSize().width +  iRandPosX  , basePoint.y + iRandPosY );
			}
			else if (p_site2 == 1)//右边玩家 第二颗和第三课 是往左边偏移的
			{
				randomPoint = Vec2(basePoint.x - frontsightTmp->getContentSize().width -  iRandPosX  , basePoint.y - iRandPosY );
			}
			else
			{
				randomPoint = Vec2(basePoint.x + frontsightTmp->getContentSize().width +  iRandPosX  , basePoint.y - iRandPosY );
			}
		}
		else
		{
			if (p_site2 == 0)
			{
				randomPoint = Vec2(basePoint.x + iRandPosX, basePoint.y + frontsightTmp->getContentSize().height + iRandPosY);
			}
			else if (p_site2 == 1)//右边玩家 第二颗和第三课 是往左边偏移的
			{
				randomPoint = Vec2(basePoint.x - iRandPosX, basePoint.y - frontsightTmp->getContentSize().height - iRandPosY);
			}
			else
			{
				randomPoint = Vec2(basePoint.x + iRandPosX, basePoint.y - frontsightTmp->getContentSize().height - iRandPosY);
			}
		}
		frontsightTmp->setPosition(randomPoint);
		log("Point %f, %f",randomPoint.x, randomPoint.y);
	}
	this->addChild(frontsightTmp);

	auto delay = DelayTime::create(0.3f);
	auto callNextStep1 = CallFuncN::create(CC_CALLBACK_1(RocketLayer::rocketMoveInBack,this));
	frontsightTmp->runAction(Sequence::create(delay, callNextStep1,nullptr));

}

void RocketLayer::rocketMoveInBack(Node* node)
{
	auto rocketIn = Sprite::createWithSpriteFrameName("rocketsmall.png");
	rocketIn->setPosition(Vec2(node->getPosition().x - node->getContentSize().width, node->getPosition().y + node->getContentSize().height));
	rocketIn->setRotation(180 + 58);
	this->addChild(rocketIn, BASIC_ZORDER_TOP);

	auto moveIn = MoveTo::create(0.1f, node->getPosition());
	auto removeSelf = RemoveSelf::create();
	auto callNextStep2 = CallFuncN::create(CC_CALLBACK_1(RocketLayer::bombStep,this));
	rocketIn->runAction(Sequence::create(moveIn, removeSelf, callNextStep2, nullptr));

}

void RocketLayer::bombStep(Node* node)
{

	node->removeFromParent();

	//轰炸
	auto ani = WWAnimation::getInstance()->animationUnCache("rocketbomb",4,0.15f,1);
	auto nodeBomb = WWAnimation::getInstance()->createAnimationSprite("rocketbomb");
	nodeBomb->setPosition(node->getPosition());
	this->addChild(nodeBomb, BASIC_ZORDER_ROOT, 99 + iCurSignIndex);
	if (UserDefault::getInstance()->getBoolForKey(SET_SHOCT_KEY,true))
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	PlatformUtils::phoneVibrate();
#endif	
	}
	auto delay = DelayTime::create(0.3f);
	auto setX2CallStep = CallFuncN::create(CC_CALLBACK_0(RocketLayer::setX2,this));
	auto anis = Sequence::create(delay,setX2CallStep,nullptr);
	nodeBomb->runAction(Animate::create(ani));
	nodeBomb->runAction(anis);

}

void RocketLayer::setX2()
{
	log("setX2....");
	if (this->getChildByTag(1000))
	{
		return;
	}
	auto boNode = X2Layer::create();
	boNode->setPosition(Vec2(0,0));
	this->addChild(boNode);
	boNode->setTag(1000);
	auto ACNode = Node::create();
	ACNode->setPosition(Vec2(0,0));
	this->addChild(ACNode);
	auto delay = DelayTime::create(3.3f);
	auto removeCallStep = CallFuncN::create(CC_CALLBACK_0(RocketLayer::removeSelf,this));
	auto anis = Sequence::create(delay,removeCallStep,nullptr);
	ACNode->runAction(anis);

}

void RocketLayer::removeSelf()
{
	this->removeFromParent();
}

void RocketLayer::onEnter()
{
	Layer::onEnter();
}

void RocketLayer::onExit()
{
	Layer::onExit();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(rocketFrame_plist);

}


/* 炸弹动画 */
BombsLayer::BombsLayer() :
	npcBombFlyTimes(0),
	BombsRotateby(0),
	p_fromSite(2),
	p_toSite1(0),
	p_toSite2(-1),
	isStorePoint(false)
{
}

BombsLayer::~BombsLayer()
{ 
}

BombsLayer* BombsLayer::createWithSite(int fromSite,int toSite1,int toSite2)
{
	BombsLayer* pItem = new BombsLayer();
	pItem->setpFromSite(fromSite);
	pItem->setpToSite1(toSite1);
	pItem->setpToSite2(toSite2);
	if(pItem && pItem->init())
	{
		pItem->autorelease();
		return pItem;
	}
	else
	{
		CC_SAFE_DELETE(pItem);
		return NULL;
	}
}


bool BombsLayer::init()
{
	if(!LayerColor::initWithColor(Color4B(255,255,255,0)))
	{
		return false;
	}

	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile(rocketFrame_plist);

	bool bRet = false;
	do
	{
		//GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getnpcLeft()->getPosition(); //左边玩家
		if (p_fromSite == 0)
		{
			if (p_toSite2 != -1)
			{
				setToNpc(1);
				setToNpc(2);
			}
			else
			{
				if (p_toSite1 == 1)
				{
					setToNpc(1);
				}
				else
				{
					setToNpc(2);
				}
			}
		}
		else
		{
			if (p_fromSite == 2)
			{
				fromRolePoint = Vec2(117, 954);
			}
			else if (p_fromSite == 1)
			{
				fromRolePoint = Vec2(CUR_SCREEN_SIZE.width - 117, 954);
			}

			auto ani = WWAnimation::getInstance()->animationUnCache("roleDroppingBomb2",7,0.15f,1);
			auto node = WWAnimation::getInstance()->createAnimationSprite("roleDroppingBomb2");
			if (p_toSite2 != -1)
			{
				ani = WWAnimation::getInstance()->animationUnCache("roleDroppingBomb1",7,0.15f,1);
				node = WWAnimation::getInstance()->createAnimationSprite("roleDroppingBomb1");
			}
			node->setPosition(fromRolePoint);
			if (p_fromSite == 2)
			{
				node->setRotation(-28);
			}
			else if (p_fromSite == 1)
			{
				node->setRotation(28);
			}
			this->addChild(node, BASIC_ZORDER_ROOT, 100);

			node->runAction(Animate::create(ani));

			auto delay = DelayTime::create(0.15f * 4); //在扔炸弹动画的地4帧，开始触发炸弹轨迹Action
			auto callFlyAction = CallFuncN::create(CC_CALLBACK_0(BombsLayer::setNpcBombsFlyAction,this));
			if (p_toSite1 == 0)//目标是自己家
			{
				callFlyAction = CallFuncN::create(CC_CALLBACK_0(BombsLayer::setNpc2Me,this));
			}
			node->runAction(Sequence::create(delay, callFlyAction, nullptr));
			if (p_toSite2 != -1)//表示有第二个目标
			{
				node->runAction(Sequence::create(delay,Animate::create(ani),nullptr));
				auto callFlyAction2 = CallFuncN::create(CC_CALLBACK_0(BombsLayer::setNpcBombsFlyAction,this));
				if (p_toSite2 == 0)//目标是自己
				{
					callFlyAction2 = CallFuncN::create(CC_CALLBACK_0(BombsLayer::setNpc2Me,this));
				}
				auto delay2 = DelayTime::create(0.15f * 8);
				node->runAction(Sequence::create(delay2, callFlyAction2, nullptr));
			}
		
		}


		bRet = true;

	} while (0);

	return bRet;
}

void BombsLayer::afterBomb()
{
	if (p_toSite1 ==1 || p_toSite2 ==1)//目标位置为右边
	{
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getnpcRight()->setBrokeHouseVisible(true);
	}

	if (p_toSite1 == 2 ||p_toSite2 ==2)//目标位置为左边
	{
		GameLogicManager::getInstance()->getpUIGamePlayerLayer()->getnpcLeft()->setBrokeHouseVisible(true);
	}
}

void BombsLayer::removeSelf()
{
	this->removeFromParent();
}

void BombsLayer::onEnter()
{
	Layer::onEnter();
}

void BombsLayer::onExit()
{
	Layer::onExit();
	WWAnimation::getInstance()->removeAnimationCache("roleDroppingBomb2");
	WWAnimation::getInstance()->removeAnimationCache("bomb1");
}

void BombsLayer::toSecond()
{

}

/*
* 构思：人物扔炸弹是可以控制其循环播放两次的
* 在炸弹扔出来的时候，在帧动画节点上添加炸弹精灵，
* （炸弹在NPC之间算法）将锚点设置在中间偏上的位置，这样在旋转动画的时候有弧形角度出现。
* 在飞行的前半段，需要执行一个放大的效果，后半执行缩小的效果。这样可以出现视觉差，可以考虑加入摄像机角度，让炸弹有Z坐标纵深飞行的感觉
*/
void BombsLayer::setNpcBombsFlyAction()
{
	npcBombFlyTimes = 1.1f; //炸弹飞行时间
	BombsRotateby = 720;

	auto bombSprite = Sprite::create("res/Images/Animation/bomb.png");
	bombSprite->setAnchorPoint(Vec2(0.5f, 0.6f));
	this->addChild(bombSprite, BASIC_ZORDER_TOP);
	//位置设置，稍微偏向扔的方向
	
	if (p_toSite1 ==1 || p_toSite2 ==1)//目标位置为右边
	{
		bombSprite->setPosition(Vec2(117 + bombSprite->getContentSize().width * 0.1, 954 + bombSprite->getContentSize().height * 0.3));
		toRolePoint = Vec2(CUR_SCREEN_SIZE.width - 117, 954);
	}
	else if (p_toSite1 == 2 ||p_toSite2 ==2)//目标位置为左边
	{
		bombSprite->setPosition(Vec2(CUR_SCREEN_SIZE.width - 117 - bombSprite->getContentSize().width * 0.1, 954 + bombSprite->getContentSize().height * 0.3));
		toRolePoint = Vec2(117, 954);
	}
	Point endPoint = toRolePoint;

	auto action = Spawn::create(
		ParabolaTo::create(npcBombFlyTimes, bombSprite->getPosition(), endPoint, 0.5f, 0.5f,3,-80),
		RotateBy::create(npcBombFlyTimes,  BombsRotateby),
		nullptr);

	//调用爆炸效果
	auto callBombAction = CallFuncN::create(CC_CALLBACK_1(BombsLayer::bombsAction,this));
	bombSprite->runAction(Sequence::create(action, callBombAction, RemoveSelf::create(),nullptr));

	//影子
	auto shanderSprite = Sprite::create("res/Images/Animation/bombShander.png");
	this->addChild(shanderSprite, BASIC_ZORDER_TOP);
	//位置设置，稍微偏向扔的方向
	if (p_toSite1 ==1 || p_toSite2 ==1)
	{
		shanderSprite->setPosition(Vec2(117, 874));
	}
	else if (p_toSite1 == 2 ||p_toSite2 ==2)
	{
		shanderSprite->setPosition(Vec2(CUR_SCREEN_SIZE.width -117, 874));
	}
	Point endPoint2 = Vec2(CUR_SCREEN_SIZE.width - 117 * 0.5f, 754);
	if (p_toSite1 == 2 || p_toSite2 == 2)
	{
		endPoint2 = Vec2(117 * 0.5f, 754);
	}

	auto action2 = Spawn::create(
		ParabolaTo::create(npcBombFlyTimes + 0.1f, fromRolePoint, endPoint2, 1 / 20.0f, 0.1f,2,-80),
		RotateBy::create(npcBombFlyTimes,  BombsRotateby),
		nullptr);
	
	shanderSprite->runAction(Sequence::create(action2, RemoveSelf::create(),nullptr));
}

void BombsLayer::bombsAction(Node* node)
{
	auto ani = WWAnimation::getInstance()->animationUnCache("bomb1", 5, 0.15f, 1);
	auto boNode = WWAnimation::getInstance()->createAnimationSprite("bomb1");
	boNode->setPosition(node->getPosition());
	boNode->setScale(1.0f / 0.8f);
	this->addChild(boNode, BASIC_ZORDER_ROOT, 100);
	if (UserDefault::getInstance()->getBoolForKey(SET_SHOCT_KEY,true))
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	PlatformUtils::phoneVibrate();
#endif	
	}
	
	boNode->runAction(Sequence::create(Animate::create(ani), RemoveSelf::create(), nullptr));

	if (!isStorePoint)
	{
	    shakePointReset = this->getParent()->getPosition(); //记录父节点位置
		isStorePoint = true;
	}

	if (this->getParent())
	{
		auto callResetFunc = CallFuncN::create(CC_CALLBACK_0(BombsLayer::resetPosition,this));
		this->getParent()->runAction(Sequence::create(WWShake::create(0.75f,10,10), callResetFunc, nullptr));
	}
	log("to setX2...");
	auto delay = DelayTime::create(0.3f);
	auto setX2CallStep = CallFuncN::create(CC_CALLBACK_0(BombsLayer::setX2,this));
	auto afterBombCall = CallFuncN::create(CC_CALLBACK_0(BombsLayer::afterBomb,this));
	auto anis = Sequence::create(afterBombCall,delay,setX2CallStep,nullptr);
	boNode->runAction(anis);
}

void BombsLayer::resetPosition()
{
	this->getParent()->setPosition(shakePointReset);
}

void BombsLayer::setNpc2Me()
{
	npcBombFlyTimes = 0.5f; //炸弹飞行时间
	BombsRotateby = 720;

	auto bombSprite = Sprite::create("res/Images/Animation/bomb.png");
	bombSprite->setAnchorPoint(Vec2(0.5f, 0.55f));
	this->addChild(bombSprite, BASIC_ZORDER_TOP);
	//位置设置，稍微偏向扔的方向
	bombSprite->setPosition(Vec2(fromRolePoint.x - bombSprite->getContentSize().width * 0.1, fromRolePoint.y + bombSprite->getContentSize().height * 0.3));

	toRolePoint = Vec2(127, 462); //我的坐标

	Point endPoint = toRolePoint;

	auto action = Spawn::create(
		//ParabolaTo::create(npcBombFlyTimes, fromRolePoint, endPoint, 1 / 6.0f, 0.5,10),
		MoveTo::create(npcBombFlyTimes, endPoint),
		ScaleTo::create(npcBombFlyTimes * 0.7f, 1.8f),
		//RotateBy::create(npcBombFlyTimes,  BombsRotateby),
		nullptr);

	auto moveToBack = ScaleTo::create(npcBombFlyTimes * 0.3f, 1.0f);

	//调用爆炸效果
	auto callBombAction = CallFuncN::create(CC_CALLBACK_1(BombsLayer::bombsAction,this));
	bombSprite->runAction(Sequence::create(action, moveToBack, callBombAction, RemoveSelf::create(),nullptr));
}

void BombsLayer::setToNpc(int toSite)
{
	npcBombFlyTimes = 1.5f; //炸弹飞行时间
	BombsRotateby = 720;

	auto bombSprite = Sprite::create("res/Images/Animation/bomb.png");
	bombSprite->setAnchorPoint(Vec2(0.5f, 0.55f));
	this->addChild(bombSprite, BASIC_ZORDER_TOP);
	bombSprite->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5, 0));
	bombSprite->setScale(2.0f);

	Point endRolePoint = Vec2(117,954);
	if (toSite == 1)
	{
		endRolePoint = Vec2(CUR_SCREEN_SIZE.width - 117, 954);
	}

	auto action2 = Spawn::create(
		ParabolaTo::create(npcBombFlyTimes + 0.1f, bombSprite->getPosition() , endRolePoint, 1 / 10.0f, 0.5,10),
		RotateBy::create(npcBombFlyTimes,  BombsRotateby),
		ScaleTo::create(npcBombFlyTimes, 1.0f),
		nullptr);
	//调用爆炸效果
	auto callBombAction = CallFuncN::create(CC_CALLBACK_1(BombsLayer::bombsAction,this));
	bombSprite->runAction(Sequence::create(action2,callBombAction, RemoveSelf::create(),nullptr));
}

void BombsLayer::setX2()
{
	log("setX2....");
	if (this->getChildByTag(1000))
	{
		return;
	}
	auto boNode = X2Layer::create();
	boNode->setPosition(Vec2(0,0));
	this->addChild(boNode);
	boNode->setTag(1000);
	auto ACNode = Node::create();
	ACNode->setPosition(Vec2(0,0));
	this->addChild(ACNode);
	auto delay = DelayTime::create(3.3f);
	auto removeCallStep = CallFuncN::create(CC_CALLBACK_0(BombsLayer::removeSelf,this));
	auto anis = Sequence::create(delay,removeCallStep,nullptr);
	ACNode->runAction(anis);

}

X2Layer::X2Layer()
{

}

X2Layer::~X2Layer()
{

}


bool X2Layer::init()
{
	log("x2 init");
	if(!LayerColor::initWithColor(Color4B(255,255,255,0)))
	{
		return false;
	}

	bool bRet = false;
	do{
		auto ani = WWAnimation::getInstance()->animationUnCache("X2", 5, 0.15f, 1);
		auto boNode = WWAnimation::getInstance()->createAnimationSprite("X2");
		boNode->setPosition(Vec2(CUR_SCREEN_SIZE.width*0.5f,CUR_SCREEN_SIZE.height*0.5f));
		this->addChild(boNode, BASIC_ZORDER_ROOT);
		boNode->runAction(Animate::create(ani));

		auto scTo = ScaleTo::create(1,0.1f,0.1f,0);
		auto moTo = MoveTo::create(1,Point(1150,1050));
		auto mo = Spawn::createWithTwoActions(scTo,moTo);
		auto removeCallStep = CallFuncN::create(CC_CALLBACK_0(X2Layer::removeSelf,this));
		boNode->runAction(Sequence::create(Animate::create(ani),mo,removeCallStep,nullptr));
	
		bRet = true;

	} while (0);

	return bRet;
}

void X2Layer::removeSelf()
{
	GameLogicManager::getInstance()->onX2AnimationEnd();
	this->removeFromParent();
}

void X2Layer::onEnter()
{
	LayerColor::onEnter();
}

void X2Layer::onExit()
{
	LayerColor::onExit();
	WWAnimation::getInstance()->removeAnimationCache("X2");
}

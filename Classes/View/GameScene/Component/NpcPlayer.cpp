/*
 * NpcPlayer.cpp
 * NPC 
 *
 *  Created on: 2015-5-13
 *      Author: diyal.yin
 */


#include "NpcPlayer.h"
#include "../GameSceneDefine.h"
#include "WWMacros.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"
#include "BaseCore/Common/VisibleRect.h"
#include "View/GameScene/Layer/GamePlayerLayer.h"
#include "BaseCore/WWEffectBase/WWAnimation.h"
#include "BaseCore/WWBLog/LogUtil.h"
#include "View/GameScene/GameLogicManager.h"

//USING_NS_CC;

const float M_FRADIUS = 228; //半径
//const float PERRANDIUS = atan(90) / 20 * PI * M_FRADIUS / 720; //单位弧度 = 角度 * PI / 180 / 2 (总共90度弧)

NpcPlayer::NpcPlayer(void) :
	m_pIsRightDirect(false),
	m_pIsCallLord(false),
	m_pIsOutCard(false),
	m_arrLeftCards(nullptr),
	b_lRoleAccount(nullptr),
	m_pNpcPokerArr(NULL),
	isSetPlayIcon(false)
{
}

NpcPlayer::~NpcPlayer(void)
{
	clearOutCard();
}

NpcPlayer* NpcPlayer::create(bool isRightDirect)
{
	NpcPlayer* p = new NpcPlayer();
	if(p && p->init(isRightDirect))
	{
		p->autorelease();
		return p;
	}
	else
	{
		CC_SAFE_DELETE(p);
		return NULL;
	}
}

bool NpcPlayer::init(bool isRightDirect)
{
	if (!LayerColor::initWithColor(Color4B(0,0,0,0))) return false;

	m_pIsRightDirect = isRightDirect;

	bool bRet = false;
	do 
	{
		//数据处理
		//UI创建
		createView();
		
		CC_BREAK_IF(!createPlayStatus());

		bRet = true;
	}
	while (0);

	return bRet;
}

void NpcPlayer::createView()
{
	//背景
	m_pBgSprite = Sprite::create("res/Images/GameScene/Brick.png");
	if (!m_pIsRightDirect)
	{
		m_pBgSprite->setPosition(Vec2(m_pBgSprite->getContentSize().width * 0.5, CUR_SCREEN_SIZE.height - m_pBgSprite->getContentSize().height * 0.5));
	}
	else
	{
		m_pBgSprite->setFlippedX(true);
		m_pBgSprite->setPosition(Vec2(CUR_SCREEN_SIZE.width - m_pBgSprite->getContentSize().width * 0.5, CUR_SCREEN_SIZE.height - m_pBgSprite->getContentSize().height * 0.5));
	}
	this->addChild(m_pBgSprite, BASIC_ZORDER_ROOT);

	//TODO  测试牌剩余张数
	m_pLabelNum = Label::createWithSystemFont(" ","res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	m_pLabelNum->setColor(GET_RGBCOLOR(16643283));
	//m_pLabelNum->setTag(BTN_NPCPLAYERLAYER_NICK_TAG);
	if (!m_pIsRightDirect)
	{
	    m_pLabelNum->setPosition(Vec2(30, 115));
	}
	else
	{
		m_pLabelNum->setPosition(Vec2(m_pBgSprite->getContentSize().width - 30, 115));
	}
	m_pLabelNum->setVisible(false);
	m_pBgSprite->addChild(m_pLabelNum,BASIC_ZORDER_TOP);

	//房子
	auto b_lHouse = Sprite::create("res/Images/GameScene/farmerRoom.png");//农民房子
	m_pBgSprite->addChild(b_lHouse, BASIC_ZORDER_ROOT);
	m_brokeHouse = Sprite::create("res/Images/GameScene/broke_room.png"); //被炸废虚房子
	m_brokeHouse->setVisible(false);
	m_pBgSprite->addChild(m_brokeHouse, BASIC_ZORDER_ROOT);
	if (!m_pIsRightDirect)
	{
		b_lHouse->setPosition(Vec2(87, 392));
		m_brokeHouse->setPosition(Vec2(87, 392));
	}
	else  //如果是右边NPC则旋转90度
	{
		b_lHouse->setPosition(Vec2(m_pBgSprite->getContentSize().width - 87, 392));
		b_lHouse->setFlippedX(true);
		m_brokeHouse->setPosition(Vec2(m_pBgSprite->getContentSize().width - 87, 392));
		m_brokeHouse->setFlippedX(true);
	}

	//头像框
	m_lRoleFrame = WWTouchSprite::createWithFile("res/Images/GameScene/roleBg.png",this, callfuncO_selector(NpcPlayer::onHeadDepress));
	this->addChild(m_lRoleFrame, BASIC_ZORDER_TOP);
	
	//玩家头像
	m_lRealHead = Sprite::create("res/Images/Common/head-temp.png");
	m_lRealHead->setVisible(false);
	m_lRealHead->setPosition(Vec2(0, 0));
	m_lRoleFrame->addChild(m_lRealHead, BASIC_ZORDER_ROOT);

	//地主角色头像
	m_lLandLordRole = WWTouchSprite::createWithFile("res/Images/GameScene/dz28r.png",this, callfuncO_selector(NpcPlayer::onHeadDepress));
	m_lLandLordRole->setVisible(false);
	m_pBgSprite->addChild(m_lLandLordRole, BASIC_ZORDER_ROOT);
	
	//农民角色头像
	m_lFarmerRole = WWTouchSprite::createWithFile("res/Images/GameScene/famer28r.png",this, callfuncO_selector(NpcPlayer::onHeadDepress));
	m_lFarmerRole->setVisible(false);
	m_pBgSprite->addChild(m_lFarmerRole, BASIC_ZORDER_ROOT);

	if (!m_pIsRightDirect)
	{ 
		//旋转角度设置
		m_lLandLordRole->setRotation(-28);
		m_lFarmerRole->setRotation(-28);
		//位置设置
		m_lRoleFrame->setPosition(Vec2(127, m_pBgSprite->getPosition().y + m_lRoleFrame->getContentSize().height/2));
		m_lLandLordRole->setPosition(Vec2(131, 312));
		m_lFarmerRole->setPosition(Vec2(131, 312));
	}
	else //如果是右边NPC则旋转90度
	{
		//旋转角度设置
		m_lLandLordRole->setRotation(28);
		m_lFarmerRole->setRotation(28);
		//位置设置
		m_lRoleFrame->setPosition(Vec2(CUR_SCREEN_SIZE.width - 127, m_pBgSprite->getPosition().y + m_lRoleFrame->getContentSize().height/2));
        m_lLandLordRole->setPosition(Vec2(m_pBgSprite->getContentSize().width - 131, 312));
        m_lFarmerRole->setPosition(Vec2(m_pBgSprite->getContentSize().width - 131, 312));
		//设置水平反转
		m_lRoleFrame->getSprite()->setFlippedX(true);
		((WWTouchSprite*)m_lLandLordRole)->getSprite()->setFlippedX(true);
		((WWTouchSprite*)m_lFarmerRole)->getSprite()->setFlippedX(true);
	}

	//账户信息
	b_lRoleAccount = ui::Scale9Sprite::create("res/Images/GameScene/accountBg.png");
	this->addChild(b_lRoleAccount, BASIC_ZORDER_ROOT + 2);

	auto l_npcName = Label::createWithSystemFont(" ","res/Fonts/FounderSemiBold.ttf",30,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	l_npcName->setColor(GET_RGBCOLOR(16643283));
	l_npcName->setString("ABCD");
	b_lRoleAccount->addChild(l_npcName,1, BTN_NPCPLAYERLAYER_NICK_TAG);

	auto s_lRoleCoin = Sprite::create("res/Images/GameScene/coin.png");
	b_lRoleAccount->addChild(s_lRoleCoin, BASIC_ZORDER_ROOT, BTN_NPCPLAYERLAYER_ICON_TAG);

	auto l_npcMoney = Label::createWithSystemFont(" ","res/Fonts/FounderSemiBold.ttf",30,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	l_npcMoney->setColor(GET_RGBCOLOR(16643283));
	l_npcMoney->setString("27800W");
	b_lRoleAccount->addChild(l_npcMoney,1, BTN_NPCPLAYERLAYER_COIN_TAG);

	setNpcPanelPosition(false); //设置个人信息区域相对位置

	if (!m_pIsRightDirect)
	{
		m_nCircleCenter = Vec2(117, 954); //圆心坐标
	}
	else
	{
		m_nCircleCenter = Vec2(CUR_SCREEN_SIZE.width - 117, 954); //圆心坐标
	}

	//dealAction(17);
}

bool NpcPlayer::createPlayStatus()
{
	bool bRet = false;
	do{
		auto statusPos = Vec2::ZERO;
		if(m_pIsRightDirect){ //右边玩家
			statusPos = Vec2(1436,827);
		}else{ //左边玩家
			statusPos = Vec2(465,827);
		}
		
		m_statusCallLord = Sprite::create("res/Images/GameScene/Label/status_calllord.png");
		CC_BREAK_IF(!m_statusCallLord);
		m_statusCallLord->setPosition(statusPos);
		this->addChild(m_statusCallLord, BASIC_ZORDER_ROOT);
	
		m_statusNotCallLord = Sprite::create("res/Images/GameScene/Label/status_notcall.png");
		CC_BREAK_IF(!m_statusNotCallLord);
		m_statusNotCallLord->setPosition(statusPos);
		this->addChild(m_statusNotCallLord, BASIC_ZORDER_ROOT);
	
		m_statusGrabLord = Sprite::create("res/Images/GameScene/Label/status_grablord.png");
		CC_BREAK_IF(!m_statusGrabLord);
		m_statusGrabLord->setPosition(statusPos);
		this->addChild(m_statusGrabLord, BASIC_ZORDER_ROOT);
	
		m_statusNotGrabLord = Sprite::create("res/Images/GameScene/Label/status_notgrab.png");
		CC_BREAK_IF(!m_statusNotGrabLord);
		m_statusNotGrabLord->setPosition(statusPos);
		this->addChild(m_statusNotGrabLord, BASIC_ZORDER_ROOT);
	
		m_statusMingCard = Sprite::create("res/Images/GameScene/Label/status_mingcard.png");
		CC_BREAK_IF(!m_statusMingCard);
		m_statusMingCard->setPosition(statusPos);
		this->addChild(m_statusMingCard, BASIC_ZORDER_ROOT);

		m_statusNotMingCard = Sprite::create("res/Images/GameScene/Label/status_notming.png");
		CC_BREAK_IF(!m_statusNotMingCard);
		m_statusNotMingCard->setPosition(statusPos);
		this->addChild(m_statusNotMingCard, BASIC_ZORDER_ROOT);
		
		m_statusSuperJiaBei = Sprite::create("res/Images/GameScene/Label/status_superjiabei.png");
		CC_BREAK_IF(!m_statusSuperJiaBei);
		if(m_pIsRightDirect){ //右边玩家
			m_statusSuperJiaBei->setPosition(Vec2(statusPos.x - 30, statusPos.y));
		}else{ //左边玩家
			m_statusSuperJiaBei->setPosition(Vec2(statusPos.x + 40, statusPos.y));
		}
		this->addChild(m_statusSuperJiaBei, BASIC_ZORDER_ROOT);

		m_statusJiaBei = Sprite::create("res/Images/GameScene/Label/status_jiabei.png");
		CC_BREAK_IF(!m_statusJiaBei);
		m_statusJiaBei->setPosition(statusPos);
		this->addChild(m_statusJiaBei, BASIC_ZORDER_ROOT);

		m_statusNotJiaBei = Sprite::create("res/Images/GameScene/Label/status_bujiabei.png");
		CC_BREAK_IF(!m_statusNotJiaBei);
		m_statusNotJiaBei->setPosition(statusPos);
		this->addChild(m_statusNotJiaBei, BASIC_ZORDER_ROOT);

		m_statusPass = Sprite::create("res/Images/GameScene/Label/status_notout.png");
		CC_BREAK_IF(!m_statusPass);
		m_statusPass->setPosition(statusPos);
		this->addChild(m_statusPass, BASIC_ZORDER_ROOT);

		hidePlayStatus();

		bRet = true;

	}while(0);

	return bRet;
}

void NpcPlayer::setBrokeHouseVisible(bool isShow)
{
	m_brokeHouse->setVisible(isShow);
}

//重置头像 清空玩家头像
void NpcPlayer::resetHead()
{
	m_lRoleFrame->setVisible(true);
	m_lRoleFrame->setEnabled(false);
	m_lRealHead->setVisible(false);
	m_lLandLordRole->setVisible(false);
	m_lFarmerRole->setVisible(false);
	hideUserInfo();
	m_pLabelNum->setVisible(false);
	m_brokeHouse->setVisible(false);

	//牌局换人后，修改头像信息框
	changeNpcInfo(false);
	if (isSetPlayIcon)
	{
		b_lRoleAccount->setContentSize(Size(b_lRoleAccount->getContentSize().width - 69, b_lRoleAccount->getContentSize().height));
		m_lRoleFrame->setScale(1.0f);
		isSetPlayIcon = false;
	}
}

//点击头像响应
void NpcPlayer::onHeadDepress(Ref * theBtn)
{
	log("NpcPlayer::onHeadDepress!");
	if(m_pIsRightDirect){
		log("right");
		GameLogicManager::getInstance()->showPlayerInfo(1);
	}else{
		log("left");
		GameLogicManager::getInstance()->showPlayerInfo(2);
	}
}

void NpcPlayer::onSmogAnimEnd()
{
	setHeadAfterSmogAnim();

	if( headId == FARMERHEAD || headId == LANDLORDHEAD  )
	{
		//m_lRoleFrame->setVisible(false); //NPC 玩家隐藏头像框
		m_lRealHead->setVisible(true);
		changeNpcInfo(true);

	}

}

void NpcPlayer::setNpcPanelPosition(bool hadSetImage)
{
	b_lRoleAccount->setAnchorPoint(VisibleRect::anchRight()); //都按照右边锚点的方式设置坐标
	auto l_GoldIcon = b_lRoleAccount->getChildByTag(BTN_NPCPLAYERLAYER_ICON_TAG);
	auto l_NameLabel = b_lRoleAccount->getChildByTag(BTN_NPCPLAYERLAYER_NICK_TAG);
	auto l_CoinLabel = b_lRoleAccount->getChildByTag(BTN_NPCPLAYERLAYER_COIN_TAG);

	if (hadSetImage) //牌局开始后，添加头像区域
	{

		if (!m_pIsRightDirect)
		{
			b_lRoleAccount->setPositionX(337 + b_lRoleAccount->getContentSize().width * 0.5 + 69);
		}
		else
		{
			b_lRoleAccount->setPositionX(CUR_SCREEN_SIZE.width - b_lRoleAccount->getContentSize().width + 69);
		}

		//头像移动到Frame
		Point moveToPoint;
		if (!m_pIsRightDirect)
		{
			moveToPoint = Vec2(b_lRoleAccount->getPositionX() - b_lRoleAccount->getContentSize().width + m_lRoleFrame->getContentSize().width * 0.25, b_lRoleAccount->getPositionY());
		}
		else
		{
			moveToPoint = Vec2(b_lRoleAccount->getPositionX() - b_lRoleAccount->getContentSize().width + m_lRoleFrame->getContentSize().width * 0.25, b_lRoleAccount->getPositionY());
		}

		auto moveTo = MoveTo::create(0.5f, moveToPoint);
		auto scaleTo = ScaleTo::create(0.5f, 0.5f);

		m_lRoleFrame->runAction(Spawn::create(moveTo, scaleTo, nullptr));

		isSetPlayIcon = true;

		l_GoldIcon->setPosition(Vec2(b_lRoleAccount->getContentSize().width * 0.4, l_GoldIcon->getContentSize().height * 0.7));
		l_NameLabel->setPosition(Vec2(b_lRoleAccount->getContentSize().width * 0.7, b_lRoleAccount->getContentSize().height * 0.7));
		l_CoinLabel->setPosition(Vec2(b_lRoleAccount->getContentSize().width * 0.7, b_lRoleAccount->getContentSize().height * 0.3));
	}
	else
	{

		if (!m_pIsRightDirect)
		{
			b_lRoleAccount->setPosition(Vec2(337 + b_lRoleAccount->getContentSize().width * 0.5, VisibleRect::leftTop().y - b_lRoleAccount->getContentSize().height * 0.5));
			m_lRoleFrame->setPosition(Vec2(m_pBgSprite->getPosition().x - m_lRoleFrame->getContentSize().width/2 - 20, m_pBgSprite->getPosition().y + m_lRoleFrame->getContentSize().height/2));
		}
		else
		{
			b_lRoleAccount->setPosition(Vec2(CUR_SCREEN_SIZE.width - b_lRoleAccount->getContentSize().width, VisibleRect::leftTop().y - b_lRoleAccount->getContentSize().height * 0.5));
			m_lRoleFrame->setPosition(Vec2(m_pBgSprite->getPosition().x - m_lRoleFrame->getContentSize().width/2 + 181, m_pBgSprite->getPosition().y + m_lRoleFrame->getContentSize().height/2));
		}

		l_GoldIcon->setPosition(Vec2(l_GoldIcon->getContentSize().width * 0.7, l_GoldIcon->getContentSize().height * 0.7));
		l_NameLabel->setPosition(Vec2(b_lRoleAccount->getContentSize().width * 0.6, b_lRoleAccount->getContentSize().height * 0.7));
		l_CoinLabel->setPosition(Vec2(b_lRoleAccount->getContentSize().width * 0.6, b_lRoleAccount->getContentSize().height * 0.3));

	}
}

void NpcPlayer::changeNpcInfo(bool hadSetImage)
{
	if (hadSetImage)
	{
		b_lRoleAccount->setContentSize(Size(b_lRoleAccount->getContentSize().width + 69, b_lRoleAccount->getContentSize().height));
	}

	setNpcPanelPosition(hadSetImage);

}

void NpcPlayer::updateUserInfo()
{
	updateRole(REALHEAD);
	
	b_lRoleAccount->setVisible(true);
	m_lRoleFrame->setEnabled(true);
	auto nickTx = (Label*)b_lRoleAccount->getChildByTag(BTN_NPCPLAYERLAYER_NICK_TAG);
	nickTx->setString(getNickName());

	auto coinTx = (Label*)b_lRoleAccount->getChildByTag(BTN_NPCPLAYERLAYER_COIN_TAG);
	char coinStr[30];
	sprintf(coinStr,"%d",getBean());
	coinTx->setString(coinStr);
}

void NpcPlayer::updateLeftCardCount(int changeCount) //更新玩家剩余牌
{

	if (!m_pIsRightDirect)
	{
		log("Left----------------> %d", changeCount);
	}
	else
	{
		log("Right---------------->%d", changeCount);
	}

	this->setLeftPokersCount( getLeftPokersCount() + changeCount );
	if( changeCount > 0 ){
		//TODO 弧形摆牌增加 changeCount 张数，并重新摆位置
		addDiZhuPoker(changeCount);
	}else{
		//TODO 弧形摆牌减少 changeCount 张数，并重新摆位置
		playPoker(changeCount);
	}
}

#define POKER_TAG 1000
#define POKER_R_TAG 2000
const int StepIndexArr[]  = {10, 11, 9, 12, 8, 13, 7, 14, 6, 15, 5, 16, 4, 17, 3, 18, 2, 19, 1, 20}; //牌顺序Index

void NpcPlayer::dealAction(int cardCount, bool hadAction)
{
	m_pCurCardCount = cardCount;

	char coinStr[30];
	sprintf(coinStr,"%d",m_pCurCardCount);
	m_pLabelNum->setString(coinStr);
	
	if( m_pCurCardCount > 0 ){
		m_pLabelNum->setVisible(true);
	}else{
		m_pLabelNum->setVisible(false);
	}

	cleanPoker();

	if (!m_pIsRightDirect)
	{
		m_nCircleCenter = Vec2(117, 954); //圆心坐标
	}
	else
	{
		m_nCircleCenter = Vec2(CUR_SCREEN_SIZE.width - 117, 954); //圆心坐标
	}

	SpriteBatchNode *batchNode = SpriteBatchNode::create("res/Images/GameScene/pokerBg.png", cardCount); //批处理渲染精灵
	Point defalPoint = getCirclePosition(StepIndexArr[0], cardCount);
	for (int i = 0; i < cardCount; i++)
	{
		auto smallPoker = Sprite::createWithTexture(batchNode->getTexture());

		int tag;
		smallPoker->setRotation(90 + 90/ 14 * StepIndexArr[0]);

		if (!m_pIsRightDirect)
		{
			tag = POKER_TAG + StepIndexArr[i];
		}
		else
		{
			tag = POKER_R_TAG + StepIndexArr[i];
		}
		
		this->addChild(smallPoker, BASIC_ZORDER_FRAME - StepIndexArr[i], tag);

		//m_pNpcPokerArr.push_back(tag); //将牌Tag存起来
		m_pNpcPokerArr.pushBack(smallPoker); //将牌Tag存起来

		smallPoker->setPosition(defalPoint); //一开始设置在屏幕中间
	}

	reflashPoker(hadAction, cardCount); //刷新位置

}

void NpcPlayer::reflashPoker(bool hadAction, int cardCount)
{
	//wwlog(LOG_INFO,"NpcPlayer","Size %d", m_pNpcPokerArr.size());
	for (unsigned int i=0; i<m_pNpcPokerArr.size(); i++)
	{
		//int tag = m_pNpcPokerArr[i];

		//auto smallPoker = this->getChildByTag(tag);

		auto smallPoker = m_pNpcPokerArr.at(i);
		if (smallPoker)
		{

			Point targetPoint = getCirclePosition(StepIndexArr[i], cardCount); //目标位置
			if (hadAction)
			{
				float perSecFrame = 0.015f;
				float perSecound;
				if (i > cardCount / 2)
				{
					perSecound = perSecFrame * (i + 1);
				}
				else
				{
					perSecound = perSecFrame * (cardCount - i + 1);
				}
				auto actionTo = MoveTo::create(perSecound, targetPoint);
				smallPoker->runAction(actionTo);
			}
			else
			{
				smallPoker->setPosition(targetPoint);
			}

			smallPoker->setZOrder(BASIC_ZORDER_FRAME - StepIndexArr[i]);

			//设置旋转角度
			if (!m_pIsRightDirect)
			{
				smallPoker->setRotation(90 + 90/ 14 * StepIndexArr[i]);
			}
			else
			{
				smallPoker->setRotation(270 - 90/ 14 * StepIndexArr[i]);
			}

			//wwlog(LOG_INFO,"NpcPlayer","位移第%d张, Tag为  %d,  ZOrder为%d, (%f,%f), 角度%d", i, smallPoker->getTag(),smallPoker->getZOrder(),targetPoint.x,targetPoint.y,smallPoker->getRotation());
		}

	}

	//wwlog(LOG_INFO,"NpcPlayer","剩余%d", m_pNpcPokerArr.size());

}

Point NpcPlayer::getCirclePosition(int nIndex, int curCardNum)
{
	float x; //计算X增量 
	float y; //计算Y增量 

	float PERRANDIUS = atan(90) / 20 * PI * M_FRADIUS / 720; //单位弧度 = 角度 * PI / 180 / 2 (总共90度弧)

	float radian = PERRANDIUS * nIndex; //弧度 = 单位弧度 * index

	if (!m_pIsRightDirect)
	{
		x = M_FRADIUS * std::cos(radian); //计算X增量 
		y = M_FRADIUS * std::sin(-radian); //计算Y增量 
	}
	else
	{
		x =  - M_FRADIUS * std::cos(radian); //计算X增量 
		y = M_FRADIUS * std::sin(-radian); //计算Y增量 
	}

	return  Vec2(x + m_nCircleCenter.x, y + m_nCircleCenter.y);
}

void NpcPlayer::addDiZhuPoker(int cardCount)
{
	SpriteBatchNode *batchNode = SpriteBatchNode::create("res/Images/GameScene/pokerBg.png", cardCount); //批处理渲染精灵
	for (int i = 17; i < 20; i++)
	{
		auto smallPoker = Sprite::createWithTexture(batchNode->getTexture());

		int tag;
		smallPoker->setRotation(90 + 90/ 14 * StepIndexArr[0]);

		if (!m_pIsRightDirect)
		{
			tag = POKER_TAG + StepIndexArr[i];
		}
		else
		{
			tag = POKER_R_TAG + StepIndexArr[i];
		}

		this->addChild(smallPoker, BASIC_ZORDER_FRAME - StepIndexArr[i], tag);

		//m_pNpcPokerArr.push_back(tag); //将牌Tag存起来
		m_pNpcPokerArr.pushBack(smallPoker); //将牌Tag存起来

	}

	m_pCurCardCount = m_pCurCardCount + cardCount;
	
	char coinStr[30];
	sprintf(coinStr,"%d",m_pCurCardCount);
	m_pLabelNum->setString(coinStr);

	if( m_pCurCardCount > 0 ){
		m_pLabelNum->setVisible(true);
	}else{
		m_pLabelNum->setVisible(false);
	}

	reflashPoker(false, cardCount); //刷新位置
}

void NpcPlayer::playPoker(int cardCount)
{
	int l_cardCount = abs(cardCount);
	if (l_cardCount > m_pNpcPokerArr.size()){
		return;
	}

	//打出的牌每次从队列最开始删除
	for (int i = 0; i < l_cardCount; i++)
	{
		if ( m_pNpcPokerArr.size() == 0 )
		{
			CCAssert(m_pNpcPokerArr.size() != 0, "m_pNpcPokerArr is empty.");
			break;
		}
		m_pNpcPokerArr.at(0)->removeFromParent();
		m_pNpcPokerArr.erase(0);
	}

	m_pCurCardCount = m_pCurCardCount - l_cardCount;

	if (!m_pIsRightDirect)
	{
		if (m_pCurCardCount != m_pNpcPokerArr.size()) //TODO test 当排面跟个数不一致的时候
		{
			CCAssert(m_pNpcPokerArr.size() == m_pCurCardCount, "m_pNpcPokerArr card count is not same");
			wwlog(LOG_INFO, __FUNCTION__,"牌张数不一致 （%d,%d） 出%d张牌的时候", m_pCurCardCount, m_pNpcPokerArr.size(), l_cardCount);
		}
	}

	char coinStr[30];
	sprintf(coinStr,"%d",m_pCurCardCount);
	m_pLabelNum->setString(coinStr);

	if( m_pCurCardCount > 0 ){
		m_pLabelNum->setVisible(true);
	}else{
		m_pLabelNum->setVisible(false);
	}

	reflashPoker(false, cardCount); //刷新位置

}


void NpcPlayer::saveOutCard(const cocos2d::Vector<CardSprite*> &cardArr)
{
	if( cardArr.size() > 0 ){
		for( CardSprite* cardSprite : cardArr){
			m_arrOutCards.pushBack(cardSprite);
			this->addChild(cardSprite, BASIC_ZORDER_FRAME + 1);
		}
	}
}

void NpcPlayer::saveLeftCard(CardsGroup* cardArr)
{
	m_arrLeftCards = cardArr;
	this->addChild(cardArr, BASIC_ZORDER_ROOT + 1);
}

void NpcPlayer::clearOutCard()
{
	//弧形手牌
	if(m_arrOutCards.size())
	{
		for( CardSprite* cardSprite : m_arrOutCards){
			this->removeChild(cardSprite);
		}
		m_arrOutCards.clear();
	}

	//上一局牌玩家手中未出完的牌
	if(m_arrLeftCards)
	{
		this->removeChild(m_arrLeftCards);
		m_arrLeftCards = nullptr;
	}
}

void NpcPlayer::cleanPoker()
{
	for (int i = 0; i < sizeof(StepIndexArr); i++)
	{
		int tag;
		if (!m_pIsRightDirect)
		{
			tag = POKER_TAG + StepIndexArr[i];
		}
		else
		{
			tag = POKER_R_TAG + StepIndexArr[i];
		}
		if( this->getChildByTag(tag) ){
			this->getChildByTag(tag)->removeFromParent();
		}
	}
	m_pNpcPokerArr.clear();
}

void NpcPlayer::hideUserInfo() //隐藏NPC玩家桌面信息展示
{
	b_lRoleAccount->setVisible(false);
}

void NpcPlayer::resetAllData()
{
}
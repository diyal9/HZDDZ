/*
 * Player.cpp
 *	功能描述：
 *  Created on: 2014年7月8日 下午2:57:55
 *      Author: hp
 */

#include "Player.h"
#include "BaseCore/WWEffectBase/WWAnimation.h"
#include "View/GameScene/GameLogicManager.h"

USING_NS_CC;

Player::Player() :
		userId(0), isVip(0), vip(0), grade(0), headImgId(0), cash(0), jiFenLevel(
				0), isSubstituted(false), gender(0), winPercent(0), escape(0), playCount(
				0), offRate(false), gamePoints(0), winCount(0), isQiangdizhu(
				false), isMingpai(false), leftPokerCount(0),
	m_statusCallLord(nullptr),
	m_statusNotCallLord(nullptr),
	m_statusGrabLord(nullptr),
	m_statusNotGrabLord(nullptr),
	m_statusMingCard(nullptr),
	m_statusNotMingCard(nullptr),
	m_statusSuperJiaBei(nullptr),
	m_statusJiaBei(nullptr),
	m_statusNotJiaBei(nullptr),
	m_statusPass(nullptr),
	m_lRoleFrame(nullptr),
	m_lRealHead(nullptr),
	m_lLandLordRole(nullptr),
	m_lFarmerRole(nullptr),
	headId(REALHEAD),
	isRecvJiaBei(false)
{

}

Player::~Player()
{
}


void Player::hidePlayStatus()
{
	m_statusCallLord->setVisible(false);
	m_statusNotCallLord->setVisible(false);
	m_statusGrabLord->setVisible(false);
	m_statusNotGrabLord->setVisible(false);
	m_statusMingCard->setVisible(false);
	m_statusNotMingCard->setVisible(false);
	m_statusSuperJiaBei->setVisible(false);
	m_statusJiaBei->setVisible(false);
	m_statusNotJiaBei->setVisible(false);
	m_statusPass->setVisible(false);
}

void Player::setPlayStatus(int statusTag)
{
	hidePlayStatus();

	switch(statusTag)
	{
	case BTN_GAMEPLAYERLAYER_JIAODIZHU_TAG : //叫地主
		{
			m_statusCallLord->setVisible(true);
		}
		break;
	case BTN_GAMEPLAYERLAYER_BUJIAODIZHU_TAG : //不叫地主
		{
			m_statusNotCallLord->setVisible(true);
		}
		break;
	case BTN_GAMEPLAYERLAYER_GRAPLORD_TAG : //抢地主
		{
			m_statusGrabLord->setVisible(true);
		}
		break;
	case BTN_GAMEPLAYERLAYER_NOTGRAPLORD_TAG ://不抢地主
		{
			m_statusNotGrabLord->setVisible(true);
		}
		break;
	case BTN_GAMEPLAYERLAYER_MINTPAI_TAG : //明牌
		{
			m_statusMingCard->setVisible(true);
		}
		break;
	case BTN_GAMEPLAYERLAYER_BUMINGPAI_TAG : //不明牌
		{
			m_statusNotMingCard->setVisible(true);
		}
		break;
	case BTN_GAMEPLAYERLAYER_SUPER_JIABEI_TAG : //超级加倍
		{
			m_statusSuperJiaBei->setVisible(true);
		}
		break;
	case BTN_GAMEPLAYERLAYER_JIABEI_TAG : //加倍
		{
			m_statusJiaBei ->setVisible(true);
		}
		break;
	case BTN_GAMEPLAYERLAYER_BUJIABEI_TAG : //不加倍
		{
			m_statusNotJiaBei->setVisible(true);
		}
		break;
	case BTN_GAMEPLAYERLAYER_BUCHU_TAG : //不出
		{
			m_statusPass->setVisible(true);
		}
		break;
	default:
		hidePlayStatus();
	}
}

void Player::setHeadAfterSmogAnim()
{
	if (this->getChildByTag(SMOGANIMATIONTAG)){
		this->removeChildByTag(SMOGANIMATIONTAG);
	}
	
	if( headId == FARMERHEAD )
	{
		m_lFarmerRole->setVisible(true);
		m_lRealHead->setVisible(false);
		m_lLandLordRole->setVisible(false);
	}
	else if( headId == LANDLORDHEAD )
	{
		m_lLandLordRole->setVisible(true);
		m_lRealHead->setVisible(false);
		m_lFarmerRole->setVisible(false);
	}
}

//点击头像响应
void Player::onHeadDepress(Ref * theBtn)
{
	log("Player::onHeadDepress!");
	GameLogicManager::getInstance()->showPlayerInfo(0);
}

void Player::showSmogAnimation()
{
	auto ani = WWAnimation::getInstance()->animationUnCache("smogAnim", 5, 0.1f, 1);
	auto node = WWAnimation::getInstance()->createAnimationSprite("smogAnim");
	log("smogAnim pos: %d,%d", m_lRoleFrame->getPosition().x, m_lRoleFrame->getPosition().y);
	node->setPosition(m_lRoleFrame->getPosition());
	node->setScale(1.0f / 0.8f);
	this->addChild(node, BASIC_ZORDER_ROOT+2, SMOGANIMATIONTAG);

	node->runAction(Sequence::create(Animate::create(ani), CallFuncN::create(CC_CALLBACK_0(Player::onSmogAnimEnd, this)),nullptr));
}

void Player::onSmogAnimEnd()
{
	setHeadAfterSmogAnim();
}

void Player::updateRole(int headType)	//设置玩家角色头像
{
	switch(headType)
	{
	case FARMERHEAD:
		{
			headId = FARMERHEAD;
			m_lRealHead->setVisible(false);
			showSmogAnimation();
		}
		break;
	case LANDLORDHEAD:
		{
			headId = LANDLORDHEAD;
			m_lRealHead->setVisible(false);
			showSmogAnimation();
		}
		break;
	default:
		headId = REALHEAD;
		m_lRoleFrame->setVisible(true);
		m_lRealHead->setVisible(true);
		m_lLandLordRole->setVisible(false);
		m_lFarmerRole->setVisible(false);
	}
}

void Player::resetHead()	//重置头像
{
	this->updateRole(REALHEAD);
}

void Player::resetAllData()
{
	isRecvJiaBei = false;
}


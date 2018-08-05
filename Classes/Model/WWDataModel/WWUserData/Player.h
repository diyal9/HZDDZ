/*
 * Player.h
 *	功能描述：
 *  Created on: 2014年7月8日 下午2:57:55
 *      Author: hp
 *
 *	changed by Jacky
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "cocos2d.h"
#include "WWMacros.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"

USING_NS_CC;

enum HeadType
{
	FARMERHEAD = 0,//0农民
	LANDLORDHEAD,  //1地主
	REALHEAD	   //2自己
};

const int SMOGANIMATIONTAG = 200; //烟雾动画 TAG.

enum BASE_GAMEPLAYERLAYER_BTN
{
	BTN_GAMEPLAYERLAYER_BUCHU_TAG = 1, //不出
	BTN_GAMEPLAYERLAYER_TISHI_TAG, //提示
	BTN_GAMEPLAYERLAYER_FANGXIA_TAG, //放下
	BTN_GAMEPLAYERLAYER_CHUPAI_TAG, //出牌
	BTN_GAMEPLAYERLAYER_JIAODIZHU_TAG, //叫地主
	BTN_GAMEPLAYERLAYER_BUJIAODIZHU_TAG, //不叫地主
	BTN_GAMEPLAYERLAYER_GRAPLORD_TAG, //抢地主
	BTN_GAMEPLAYERLAYER_NOTGRAPLORD_TAG, //不抢地主
	BTN_GAMEPLAYERLAYER_MINTPAI_TAG, //明牌
	BTN_GAMEPLAYERLAYER_BUMINGPAI_TAG, //不明牌
	BTN_GAMEPLAYERLAYER_JIABEI_TAG, //加倍
	BTN_GAMEPLAYERLAYER_SUPER_JIABEI_TAG,//超级加倍
	BTN_GAMEPLAYERLAYER_BUJIABEI_TAG, //不加倍
}; 

/*
 *用户数据
 */
class Player: public cocos2d::LayerColor
{
public:
	Player();
	virtual ~Player();

	virtual void resetAllData();//重置相关数据

	virtual bool createPlayStatus() = 0; //创建玩家状态
	virtual	void updateUserInfo() = 0;   //更新个人信息显示
	virtual void updateLeftCardCount(int changeCount) = 0; //更新玩家剩余牌数
	virtual void updateRole(int headType);		//设置玩家角色头像
	virtual void resetHead();	//重置头像
	virtual void onHeadDepress(Ref * theBtn);  //点击头像响应
	
	//玩家变身烟雾动画
	virtual void showSmogAnimation();
	virtual void onSmogAnimEnd();
	void setHeadAfterSmogAnim();
	
	//玩家玩牌状态展示
	virtual void hidePlayStatus();
	virtual void setPlayStatus(int tag);

	CC_SYNTHESIZE(int, userId, Userid);//用户ＩＤ
	CC_SYNTHESIZE(std::string, nickName, NickName);//用户名称
	CC_SYNTHESIZE(int, isVip, IsVip);//用户是否ＶＩＰ
	CC_SYNTHESIZE(int, vip, Vip);//vip等级 ：金 、银、铜
	CC_SYNTHESIZE(int, grade, Grade);//用户等级
	CC_SYNTHESIZE(int, headImgId, HeadImgId);//用户头像id
	CC_SYNTHESIZE(int, cash, Cash);//用户财富值
	CC_SYNTHESIZE(int, bean, Bean);//蛙豆
	CC_SYNTHESIZE(int, crystal, Crystal);//水晶
	CC_SYNTHESIZE(int, jiFenLevel, JiFenLevel);//游戏积分等级
	CC_SYNTHESIZE(bool, isSubstituted, IsSubstituted);//是否被托管
	CC_SYNTHESIZE(int, gender, Gender);//用户性别：0 不确定	1 男	2 女
	CC_SYNTHESIZE(int, winPercent, WinPercent); //胜率
	CC_SYNTHESIZE(int, escape, Escape);//逃跑率
	CC_SYNTHESIZE(int, playCount, PlayCount);//总局数
	CC_SYNTHESIZE(int, offRate, OffRate);//断线率
	CC_SYNTHESIZE(int, gamePoints, GamePoints);//积分
	CC_SYNTHESIZE(int, winCount, WinCount);//胜利局数
	CC_SYNTHESIZE(std::string, onlineType, OnlineType);//网络连接方式
	CC_SYNTHESIZE(bool, isQiangdizhu, IsQiangdizhu);//是否抢地主
	CC_SYNTHESIZE(bool, isMingpai, IsMingpai);//是否明牌

	CC_SYNTHESIZE(std::string, levelName, PlayerLevelName);//用户称号
	CC_SYNTHESIZE(int, waDou, PlayerWaDou);//胜利局数

	CC_SYNTHESIZE(int, leftPokerCount, LeftPokersCount); //玩家剩余牌数

	CC_SYNTHESIZE(bool, isRecvJiaBei, IsRecvJiaBei); ////是否收到加倍结果消息   ps::当用户不发送加倍消息，或者用户为机器人时， 不会收到加倍结果消息
	
	//玩牌状态
	Sprite* m_statusCallLord;
	Sprite* m_statusNotCallLord;
	Sprite* m_statusGrabLord;
	Sprite* m_statusNotGrabLord;
	Sprite* m_statusMingCard;
	Sprite* m_statusNotMingCard;
	Sprite* m_statusSuperJiaBei;
	Sprite* m_statusJiaBei;
	Sprite* m_statusNotJiaBei;
	Sprite* m_statusPass;

	//玩家头像、角色
	WWTouchSprite* m_lRoleFrame;  //头像框
	Sprite* m_lRealHead;	//玩家头像
	Node* m_lLandLordRole;//地主角色头像
	Node* m_lFarmerRole;  //地主角色头像
	int headId;	 //玩家头像id
};


#endif /* PLAYER_H_ */

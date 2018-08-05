/*
 * GameCommData.h
 *	功能描述：
 *  Created on: 2014年6月30日 下午7:33:40
 *      Author: hp
 */

#ifndef GAMECOMMDATA_H_
#define GAMECOMMDATA_H_

#include "cocos2d.h"
#include "WWMacros.h"

/**
 * 托管数据
 */
class GameSubstituteData:public cocos2d::Ref
{
public:
	GameSubstituteData();
	virtual ~GameSubstituteData();
	virtual bool init();
	CREATE_FUNC(GameSubstituteData);
	CC_SYNTHESIZE(int, gameId, GameId);//(int4)游戏标示
	CC_SYNTHESIZE(int, gameZoneID, GameZoneID);//(int4)游戏区ID
	CC_SYNTHESIZE(int, gameplayID, GameplayID);//(int4)对局ID
	CC_SYNTHESIZE(int, userID, UserID);//(int4)用户ID
	CC_SYNTHESIZE(int, type, Type);//(int1)0 托管、1 取消托管

};

/**
 * 聊天数据
 */
class GameChatData:public cocos2d::Ref
{
public:
	GameChatData();
	virtual ~GameChatData();
	virtual bool init();
	CREATE_FUNC(GameChatData);
	CC_SYNTHESIZE(int, gamePlayID, GamePlayID);//(int4)游戏标示
	CC_SYNTHESIZE(int, fromUserID, FromUserID);//用户ID
	CC_SYNTHESIZE(int, type, Type);//int1)聊天内容类型:0:文字聊天1:语音聊天2:视频聊天
	CC_SYNTHESIZE(std::string, chatContent, ChatContent);//(String)聊天内容
	CC_SYNTHESIZE(std::string, userNick, UserNick);//(String)昵称
	CC_SYNTHESIZE(int, toUserID, ToUserID);//(int4)私语对象ID0-整个桌子广播
};

/*
 *通用结算数据
 */
class GameOverCommonData:public cocos2d::Ref
{
public:
	GameOverCommonData();
	virtual ~GameOverCommonData();
	CREATE_FUNC(GameOverCommonData);
	virtual bool init();

	CC_SYNTHESIZE(int, gameId, GameId);//游戏ＩＤ
	CC_SYNTHESIZE(int, gamePlayId, GamePlayId);//对局标示
	CC_SYNTHESIZE(int, beanTax, BeanTax); //(int4)蛙豆房系统收税
	CC_SYNTHESIZE(int, playerCount, PlayerCount);//(int2)玩家数量

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, userIDs, UserIds);//用户ＩＤ

	//WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, nickNames, NickNames);//用户昵称

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, fortunes, Fortunes);//(int4)用户本局财富变化
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<std::string>, tFortunes, TFortunes);//(int4)用户本局后总财富

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, chips, Chips);//(int4)用户本局临时积分变化 
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, tChips, TChips);//(int4)用户本局后临时积分

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, gamePoints, GamePoints);//(int4)用户本局游戏积分变化
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, tGamePoints, TGamePoints);//(int4)用户本局后游戏总积分

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, gamePointLevels, GamePointLevels);//(int2)积分级别

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, playResults, PlayResults);//(int2)1赢、2输、3和

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, currLevel, CurrLevel);//(int4)用户当前等级
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, currExp, CurrExp);//(int4)用户当前经验
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, addExp, AddExp);//(int4)用户本局新增经验
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<int>, levelExp, LevelExp);//(int4)当前等级升到下一等级所需的总经验
};

/**
 * 道具使用数据
 */
class PropUseInfoData:public cocos2d::Ref
{
	public:
		PropUseInfoData();

		virtual ~PropUseInfoData();

		CREATE_FUNC(PropUseInfoData);

		virtual bool init();

		CC_SYNTHESIZE(int, gamePlayID, GamePlayID);//对局标示
		CC_SYNTHESIZE(int, propID, PropId);//道具id
		CC_SYNTHESIZE(int, fromUserID, FromUserID);
		CC_SYNTHESIZE(int, useToUserID, UseToUserID);

};
#endif /* GAMECOMMDATA_H_ */

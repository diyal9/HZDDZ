/*
 * MsgGameFilter.h
 *	功能描述：
 *  Created on: 2014年6月27日 上午10:01:18
 *      Author: hp
 */

#ifndef _Model_WWFilter_MSGLORDGAMEFILTER_H_
#define _Model_WWFilter_MSGLORDGAMEFILTER_H_
#include "cocos2d.h"
#include "BaseCore/WWNetDataBasic/WWIMsgFilter.h"

const int MSG_GAME_START = 0x070101; //游戏开局
const int MSG_GAME_PLAY_CARD = 0x070103; //出牌
const int MSG_GAME_CALL_LORD = 0x07010D; //叫地主
const int MSG_GAME_GRAP_LORD = 0x07010E; //抢地主
const int MSG_GAME_MING_PAI = 0x07010F; //明牌
const int MSG_GAME_DOUBLE_SCORE = 0x070112;//加倍
static const int MSG_GAME_GAMEOVER = 0x070104; //结算


const int MSG_GAME_COMMON_GAMEOVER = 0x060117; //通用结算
const int MSG_GAME_SUBSTITUTE = 0x06010f; //托管
const int MSG_GAME_CHAT = 0x06010E; //聊天
const int MSG_PROP_USE_INFOR=0x110305;//道具使用（表情）
//通用消息返回
const int MSG_LEAVEGAME_BACK = 0x060102; //请求离开游戏的返回
static const int MSG_REFUSE_CONTINUE_GAME = 0x06010b; //拒绝游戏续局请求返回


/*
 *游戏中数据解析
 */
class MsgLordGameFilter:public WWIMsgFilter
{
public:
	MsgLordGameFilter();

	virtual ~MsgLordGameFilter();

//	virtual bool init();
//	CREATE_FUNC(MsgLordGameFilter);
	virtual void onDataReceiver(int msgType,cocos2d::Ref* objectData ){};//过滤器中所有的消息通过此方法分发到场景中，根据type转型为相应的数据结构

	virtual std::vector<int> onRootFilters();

	virtual std::vector<int> onFilters();

	virtual bool onMessageReceived(int msgType);

	virtual bool onRootMessageReceived(int msgType, int userId, int result,std::string reason, int reasonType);

	cocos2d::Ref* parseStartGame();//开局消息

	cocos2d::Ref* parseCallLord();//叫地主

	cocos2d::Ref* parseGrabLord();//抢地主

	cocos2d::Ref* parsePlayCard();//打牌

	cocos2d::Ref* parseMingPai();//明牌

	cocos2d::Ref* parseJiaBei();//加倍

	cocos2d::Ref* parseGameOver();//结算

	cocos2d::Ref* parseCommonGameOver();//通用結算消息解析

	cocos2d::Ref* parseGameSubstitute();//游戏托管

	cocos2d::Ref* parseGameChat();//游戏中聊天

	cocos2d::Ref* parseGameUserInfo();//游戏玩家资料

	cocos2d::Ref* parsePropUseToInfo();//道具使用

	//-----------------------------
//
//	void fapaiFished();//发牌结束调用
//
//	void beganFilterPlayCardMessage();	//摇癞子结束调用
//
//private:
//	bool isFapaiFished;
//	bool isTimeToPlayCard;
//
//    cocos2d::Vector<cocos2d::Ref*> callMessageCache;	//叫地主消息缓存
//    cocos2d::Vector<cocos2d::Ref*> grapMessageCache;	//抢地主消息缓存
//    cocos2d::Vector<cocos2d::Ref*> playMessageCache;	//打牌消息缓存
};

#endif /* _Model_WWFilter_MSGLORDGAMEFILTER_H_ */

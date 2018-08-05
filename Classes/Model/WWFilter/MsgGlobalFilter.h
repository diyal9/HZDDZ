/*
 * MsgGlobalFilter.h
 *	功能描述：
 *  Created on: 2014年7月15日 上午11:14:31
 *      Author: hp
 */

#ifndef MSGGLOBALFILTER_H_
#define MSGGLOBALFILTER_H_

#include "cocos2d.h"
#include "BaseCore/WWNetDataBasic/WWIMsgFilter.h"

const int MSG_PORP_ISSUE_NOTIFY = 0x010137;	//物品发放：新手礼包  签到
const int MSG_LOGON_AWARD_INFO =0x020118;//连续登录天数和抽奖次数信息
const int MSG_MEDIA_CONTENT = 0x010107;	//用户形象
//const int MSG_MAIN_USER_INFO = 0x01010b;	//用户主界面消息，登陆成功后收到
//const int MSG_RECOVERY_GAME_NOTIY = 0x06013a;	//恢复对局通知
//const int MSG_GAME_RESUME = 0x070117; //恢复对局响应
const int MSG_GAME_RESUME_RECORD_CARD = 0x070118; //恢复对局记牌器消息
static const int MSG_GAME_PLAYERS_INFO = 0x01010c;  //玩家资料数据
static const int MSG_ACTIVITY_NOTIY = 0x000001; //蛙园活动速递

const int MSG_GLOBAL_LOGOUT_INFO = 0x020104; //用户退出确认消息

//排行榜
static const int MSG_TOP_SCORE = 0x03011b;

//物品箱列表(文档中8.x)
const static int EQUIP_LIST_ID = 0x110802;

//物品箱详细信息
const static int EQUIP_DETAIL_INF_ID = 0x110804;
const static std::string g_equipBoxList = "equip_box_list_recive";

//开宝箱回调
const static int OPEN_BOX_RESP = 0x060116;

//积分清零回调
const static int CLEAN_SCORE_RESP = 0x110305;

//解析操作反馈信息
const int MSG_GLOBAL_RESULT_INFO = 0x010108;

/**************************************新加消息头********START***********************************************/
//----------------------普通消息返回头--------------------------
//新游戏玩家基本资料
const int MSG_GAME_USER_BASIC_INFO = 0x010134;

//-------------------通用消息返回头------------------------------
//const static int MSG_ENTER_GAME = 0x060104;
//const static int MSG_GENNAL_OTHER_LOGIN = 0x000000;//账号在别处登陆
/**************************************新加消息头*********END**********************************************/

class MsgGlobalDelegate
{
public:
    virtual void openBoxCallBack(void* data) = 0;
};


/*
 *全局消息
 */
class MsgGlobalFilter: public WWIMsgFilter, public cocos2d::Ref
{
public:
	MsgGlobalFilter();

	virtual ~MsgGlobalFilter();

	CREATE_FUNC(MsgGlobalFilter);

	bool init();

	virtual void onDataReceiver(int msgType,cocos2d::Ref* objectData ){};//过滤器中所有的消息通过此方法分发到场景中，根据type转型为相应的数据结构

	virtual std::vector<int> onFilters();

	virtual bool onMessageReceived(int msgType);

	virtual std::vector<int> onRootFilters();

	virtual bool onRootMessageReceived(int msgType, int userId, int result,std::string reason, int reasonType);

	//cocos2d::Ref* parsePropIssue(WWBuffer *oBuffer);//解析道具物品发放

	//cocos2d::Ref* parseLogonAwardInfo(WWBuffer *oBuffer);//连续登录天数和抽奖次数信息

	//cocos2d::Ref* parseMediaContentInfo(WWBuffer *oBuffer);//用户形象信息

	cocos2d::Ref* parseMainUserInfo();//用户详细信息

	cocos2d::Ref* parseRecoveryGameNofity();	//恢复对局通知

	cocos2d::Ref* parseResumeGame();	//恢复对局响应
 //     
 //   cocos2d::Ref* parseGameUserInfo(WWBuffer *oBuffer);  //玩家数据
 //   
 //   cocos2d::Ref* parseActivityNotify(WWBuffer *oBuffer);   //活动信息
 //   
 //   //物品箱所有物品列表 系统其他文档8.x
 //   cocos2d::Ref* parseBoxInfoParam(WWBuffer* oBuffer);
 //   
 //   //物品箱中单个物品详细列表 通常与8.x配对使用
 //   cocos2d::Ref* parseEquipDetailInfoParam(WWBuffer* oBuffer);
 //   
 //   //排行榜
 //   cocos2d::Ref* parseTopScore(WWBuffer *oBuffer);
 //   
 //   //开宝箱结果
 //   cocos2d::Ref* parseOpenBoxResult(WWBuffer* oBuffer);
 //   
 //   //道具使用结果
 //   Ref* parsePropUseToInfo(WWBuffer *oBuffer);
 //   
 //   Ref* parseLogoutInfo(WWBuffer *oBuffer);
 //   
 //   //解析操作反馈信息
 //   cocos2d::Ref* parseResultInfo(WWBuffer *oBuffer);

	/***************************新加消息解析****************START*******************/
	//新游戏玩家基本资料
	 cocos2d::Ref* parseGameUserBasicInfo();
	/***************************新加消息解析****************END*******************/
private:
	void parseEnterGameRootMsg(cocos2d::Ref* data);
};

#endif /* MSGGLOBALFILTER_H_ */

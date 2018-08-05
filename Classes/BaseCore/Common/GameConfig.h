#ifndef _BASECORE_COMMON_GAME_CONFIG_H_
#define _BASECORE_COMMON_GAME_CONFIG_H_

#include "cocos2d.h"

using namespace std;
USING_NS_CC;

enum class NodeAlignMent 
{
    LEFT,
    RIGHT,
    CENTER,
	TOP,
	BOTTOM
};

//玩家位置
typedef enum PlayerSite
{
	kSiteSelf = 0,
	kSiteRight = 1,
	kSiteLeft = 2,
	kSiteCenter = 3 //中心位子，用于加倍状态
    
}wwPlayerSite;

//游戏状态
enum kGameState
{
	State_GameReady = 1,
	State_GameStart,
	State_CallLord,
	State_GrapLord,
	State_MinPai,
	State_JiaBei,
	State_ChooseLaizi,
	State_PlayCard,
	State_GameOver
};

//房间类型
enum RoomType
{
	HAPPY_ROOM  =2, //欢乐场
	RASCAL_ROOM =4, //癞子场
	MATCH_ROOM	=1,	//比赛场
	INVITE_ROOM =3	//约战场
};

#define VERSIONNUM  "Version: BateV1.0.0" //"版本号：BateV1.0.0"
#define USER_NAME_PLACEHOLDER  "name" //"输入帐号"
#define USER_PASSWORD_PLACEHOLDER "password" //"输入密码"
#define LOBBY_USER_NAME "name 12345"	// 默认名称
#define LOBBY_COIN_NUM "100,000"		// 金币数量
#define LOBBY_DIAM_NUM "100,000"		//"输入密码"

#define LOBBY_LOGIN_LAYER  1  //登录
#define LOBBY_LOBBY_LAYER  2  //大厅
#define LOBBY_ROOM_LAYER   3  //房间
#define LOBBY_TEST_LAYER   100 //测试界面

#define GAME_FONT_NAME     "res/Fonts/arial.ttf"
#define GAME_FONT_NAME_1   "AppleGothic"

#define GAME_FONT_SIZE_20   20
#define GAME_FONT_SIZE_23   23
#define GAME_FONT_SIZE_24   24
#define GAME_FONT_SIZE_26   26
#define GAME_FONT_SIZE_28   28
#define GAME_FONT_SIZE_30   30
#define GAME_FONT_SIZE_32   32


//************ toast 提示文本 ***************//
static const char* g_GameSence_Toast = "GameScene";
//等待开局
static const char* g_Game_WaitFor_Start = "g_Game_WaitFor_Start";
//等待叫地主
static const char* g_Game_WaitFor_CallLord = "g_Game_WaitFor_CallLord";
//等待抢地主
static const char* g_Game_WaitFor_GrabLord = "g_Game_WaitFor_GrabLord";
//等待明牌
static const char* g_Game_WaitFor_MinPai = "g_Game_WaitFor_MinPai";
//等待加倍
static const char* g_Game_WaitFor_JiaBei = "g_Game_WaitFor_JiaBei";
//倍数
static const char* g_Game_TopMenu_Multiple = "g_Game_TopMenu_Multiple";
//底注
static const char* g_Game_TopMenu_Base = "g_Game_TopMenu_Base";
//任务赢足送钥匙
static const char* g_Game_top_key = "g_Game_top_key";
//中途退出游戏提示
static const char* g_Game_exit_tishi = "g_Game_exit_tishi";
//确认退出
static const char* g_Game_exit_sure = "g_Game_exit_sure";
//取消退出
static const char* g_Game_exit_cansle = "g_Game_exit_cansle";

//玩家资料
//昵称
static const char* g_Game_info_nick = "g_Game_info_nick";
//id
static const char* g_Game_info_id = "g_Game_info_id";
//称号
static const char* g_Game_info_chenghao = "g_Game_info_chenghao";
//地区
static const char* g_Game_info_address = "g_Game_info_address";
//胜率
static const char* g_Game_info_shenglv = "g_Game_info_shenglv";
//总局数
static const char* g_Game_info_totle = "g_Game_info_totle";
//互动道具使用
static const char* g_Gmae_info_daoju = "g_Gmae_info_daoju";
#endif

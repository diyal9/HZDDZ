#ifndef _BASECORE_COMMON_UiCoordConfig_h
#define _BASECORE_COMMON_UiCoordConfig_h

#include "cocos2d.h"
#include "WWMacros.h"

USING_NS_CC;

//Login Layer
const Vec2 LOGIN_UI_POS[]={
	Vec2(-100, 100),//nameInputBg
	Vec2(0, 100),	//nameInputTx
	Vec2(-100, 0),	//pswInputBg
	Vec2(0, 0),		//pswInputtx
	Vec2(-20, -100),//loginBtn
	Vec2(260, 139), //goBackBtn 
	Vec2(960, 139), //quickStartBtn
	Vec2(1640, 139),//changeAcBtn
	Vec2(40, 100),	//gameInfoBtn
	Vec2(-65, -40),	//versionTx
	Vec2(350,40),	//versionTx size(width,height)
};

//Lobby Layer
const Vec2 LOBBY_UI_POS[]={
	Vec2(345, 668 - 180),	//happyBtn
	Vec2(1031, 914 - 180), //rascalBtn
	Vec2(1557, 612 - 180),//matchBtn
	Vec2(0, 0),		//sysNoticeBtn
	Vec2(988, 519 - 180),	//firstBuyBtn
	/* bottomBar */
	Vec2(115,74),	//emailBtn
	Vec2(315,74),	//teskBtn
	Vec2(528,74),	//buyBtn
	Vec2(745,74),	//shopBtn
	Vec2(950,74),	//activityBtn
	Vec2(1193,43),	//quickStartBtn
	Vec2(1250,123),	//birdImg
	/* topBar */
	Vec2(95,-77),	//vipTag
	Vec2(95,0),		//headFrame
	Vec2(95,5),		//headImage
	Vec2(430, 36),  //nameBg 
	Vec2(945, 36),  //moneyBg
	Vec2(1135, 43),	//moneyAdd
	Vec2(1453, 36),	//diamBg
	Vec2(1652, 43),	//diamAdd
	Vec2(1820,20),	//menuBtn

	Vec2(440, 40),	//nameTx
	Vec2(400, 40),	//nameTxSize (width,height)
	Vec2(915, 40),	//moneyTx
	Vec2(400, 40),	//moneyTxSize (width,height)
	Vec2(1443, 40),	//diamTx
	Vec2(400, 40),	//diamTxSize (width,height)
	Vec2(2127, 834 - 180),	//crazyBtn 28
	Vec2(364, 432 - 180),	//happy label
	Vec2(1587, 394 - 180),	//rascal label
	Vec2(1069, 696 - 180),	//match label
	Vec2(2127, 530 - 180),	//crazy label

};

//set Layer Ui
const Vec2 SET_LAYER_UI_POS[] =
{
	Vec2(-5, 407),       //0 top title
	Vec2(731, 431),      //1 top closeBtn
	Vec2(0, 51),         //2 content bg
	Vec2(-324, 245),     //3 game sound set content
	Vec2(-222, 243),     //4 game sound set btn
	Vec2(-324, 115),     //5 bg sound content
	Vec2(-222, 113),     //6 bg sound btn
	Vec2(-324, -15),     //7 no sound content
	Vec2(-222, -18),     //8 no sound btn
	Vec2(-324, -145),    //9 shock content
	Vec2(-222, -147),    //10 shock btn
	Vec2(418, 243),      //11 animate btn
	Vec2(316, 245),      //12 animate content 
	Vec2(418, 113),      //13 scroll btn
	Vec2(316, 115),      //14 scroll content
	Vec2(418, -18),      //15 contiun btn
	Vec2(316, -15),      //16 contiun content
	Vec2(-316, -317),    //17 verson update btn
	Vec2(119, 37),       //18 verson update red
	Vec2(318, -317)      //19 kefu btn
};

//****************************游戏中玩家信息位置 start******************************************************//
//游戏中上家位置
const Vec2 POSITION_LAST[] = 
{
	Vec2(405, 200),              //0 整体位置
	Vec2(132, 286),              //1 头像位置
	Vec2(132, 176),              //2 VIP位置
	Vec2(10, 358),               //3 昵称位置
	Vec2(51, 52),                //4 金币位置
	Vec2(10, 345),               //5 横线
	Vec2(208, 155),              //6 胜率背景
	Vec2(65, 153),               //7 胜率字
	Vec2(225, 153),              //8 总局数字
	Vec2(60, 85)                 //9 第一个道具位置 间隔 105
};

//游戏中下家位置
const Vec2 POSITION_NEXT[] = 
{
	Vec2(1090, 200),             //0 整体位置
	Vec2(132, 286),              //1 头像位置
	Vec2(132, 176),              //2 VIP位置
	Vec2(20, 358),               //3 昵称位置
	Vec2(61, 52),                //4 金币位置
	Vec2(20, 345),               //5 横线
	Vec2(218, 155),              //6 胜率背景
	Vec2(75, 153),               //7 胜率字
	Vec2(235, 153),              //8 总局数字
	Vec2(70, 85)                 //9 第一个道具位置 间隔 105
};

//游戏中自己家位置
const Vec2 POSITION_SELF[] = 
{
	Vec2(-405, 400),             //0 整体位置
	Vec2(132, 286),              //1 头像位置
	Vec2(132, 176),              //2 VIP位置
	Vec2(10, 345),               //3 昵称位置
	Vec2(51, 52),                //4 金币位置
	Vec2(10, 330),               //5 横线
	Vec2(208, 66),              //6 胜率背景
	Vec2(65, 65),               //7 胜率字
	Vec2(225, 65),              //8 总局数字
};
//****************************游戏中玩家信息位置 end******************************************************//

//***********Charge layer 位置坐标**********//
const Vec2 CHARGE_POSTIONS[] = {
	Vec2(760, 861),         //0  titleBg
	Vec2(760, 860),         //1  titleTxt
	Vec2(760, 766),         //2  descBg
	Vec2(106, 67),          //3  特权Icon
	Vec2(252, 52),          //4  Vip Icon
	Vec2(533, 55),          //5  Vip bg
	Vec2(613, 54),          //6  Vip pro
	Vec2(762, 377),         //7  ItemContent
	Vec2(83, 122),          //8  ItemFrist
	Vec2(112, 98),          //9  ItemIcon
	Vec2(584, 94),          //10 ItemBtn
	Vec2(210, 123),         //11 jinbi
	Vec2(210, 74),          //12 赠送
	Vec2(298, 53),          //13 当前VIP等级数字
	Vec2(949, 53),          //14 再充多少就升级Vip字
	Vec2(106, 53)           //15 充值钻石更加好
};

//***********位置坐标**********//
const Vec2 USER_INFO_POSTIONS[] = 
{
	Vec2(757, 861),                     //0  title
	Vec2(251, 701),                     //1  头像背景
	Vec2(326, 623),                     //2  头像编辑按钮
	Vec2(251, 597),                     //3  头像VIP标识
	Vec2(424, 478),                     //4  昵称编辑按钮
	Vec2(765, 428),                     //5  内容背景图
	Vec2(657, 152),                     //6  注销按钮
	Vec2(980, 152),                     //7  绑定账号
	Vec2(1302, 152),                    //8  修改密码
	Vec2(418, 431),                     //9  基本信息背景阴影
	Vec2(978, 490),                     //10 右边信息背景
	Vec2(980, 723),                     //11 切换按钮背景
	Vec2(717, 723),                     //12 切换按钮 欢乐斗地主
	Vec2(980, 723),                     //13 切换按钮 癞子斗地主
	Vec2(1244, 723),                    //14 切换按钮 比赛
	Vec2(90, 420),                      //15 账号文字位置 间距68
	Vec2(273, 390),                     //16 横线位置  间距68
	Vec2(1438, 255),                    //17 帮助按钮
	Vec2(978, 608),                     //18 胜率背景
	Vec2(320, 353),                     //19 等级seekbar
	Vec2(978, 323),                     //20 积分seekbar
	Vec2(272, 478),                     //21 昵称背景
	Vec2(110, 478),                     //22 昵称文字
	Vec2(670, 444),                     //23 小土豪
	Vec2(950, 444),                     //24 大土豪
	Vec2(1244, 444),                    //25 超级大土豪
	Vec2(655, 608),                     //26 胜率字
	Vec2(1000, 608),                    //27 胜率局数
};

/****************************好友坐标***********************************************/
const Vec2 FRIEND_POSTIONS[] = 
{
	Vec2(757, 861),                     //0  title
};
#endif
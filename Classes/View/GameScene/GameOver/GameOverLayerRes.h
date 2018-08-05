/*! @file
********************************************************************************
<PRE>
模块名       :  基本Layer
文件名       :  GameOverLayerRes.h
文件实现功能 :  游戏结算资源文件
作者         :  Toby
版本         :  1.0
--------------------------------------------------------------------------------
备注         : 游戏结算资源
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/05/11   1.0      Toby                新建
</PRE>
*******************************************************************************/

#ifndef __HZ_DDZ_VIEW_GAMESCENE_GameOver_GameOverLayerRes_H__
#define __HZ_DDZ_VIEW_GAMESCENE_GameOver_GameOverLayerRes_H__
#include "cocos2d.h"

USING_NS_CC;

//**********资源文件**********************//
static const char* game_over_plist                   = "res/Images/GameScene/GameOver/gameover.plist";             //结算界面资源plist
static const char* beiTxt                            = "res/Images/GameScene/GameOver/beiTxt.png";                 //倍数
static const char* box                               = "res/Images/GameScene/GameOver/box.png";                    //宝箱
static const char* btnLong                           = "res/Images/Common/btnLong.png";                            //续局按钮
static const char* farmerLoseBigImg                  = "res/Images/GameScene/GameOver/farmerLoseBigImg.png";       //自己农民输形象
static const char* farmerLoseSmallImg                = "res/Images/GameScene/GameOver/farmerLoseSmallImg.png";     //别人农民输icon
static const char* farmerWinBigImg                   = "res/Images/GameScene/GameOver/farmerWinBigImg.png";        //自己农民赢形象
static const char* farmerWinSmallImg                 = "res/Images/GameScene/GameOver/farmerWinSmallImg.png";      //别人农民赢icon
static const char* goldBigImg                        = "res/Images/GameScene/GameOver/goldBigImg.png";             //金币icon
static const char* lordLoseBigImg                    = "res/Images/GameScene/GameOver/lordLoseBigImg.png";         //自己地主输形象
static const char* lordLoseSmallImg                  = "res/Images/GameScene/GameOver/lordLoseSmallImg.png";       //别人地主输icon
static const char* lordWinBigImg                     = "res/Images/GameScene/GameOver/lordWinBigImg.png";          //自己地主赢形象
static const char* lordWinSmallImg                   = "res/Images/GameScene/GameOver/lordWinSmallImg.png";        //别人地主赢icon
static const char* loseBg                            = "res/Images/GameScene/GameOver/loseBg.png";                 //输背景
static const char* lvUp                              = "res/Images/GameScene/GameOver/lvUp.png";                   //升级背景
static const char* winBg                             = "res/Images/GameScene/GameOver/winBg.png";                  //赢背景
static const char* addNum                            = "res/Images/GameScene/GameOver/addNum.png";                 //赢金币数字
static const char* beishu                            = "res/Images/GameScene/GameOver/beishu.png";                 //倍数数字
static const char* jia                               = "res/Images/GameScene/GameOver/jia.png";                    //加号
static const char* jian                              = "res/Images/GameScene/GameOver/jian.png";                   //减号
static const char* jianNum                           = "res/Images/GameScene/GameOver/jianNum.png";                //输金币数字
static const char* vip                               = "res/Images/GameScene/GameOver/vip.png";                    //其他玩家VIPicon
static const char* btnYellow                         = "res/Images/Common/btn_yellow.png";                         //再来一局


//************文本***************//
static const char* g_over_section       = "GameOverLayer";            //结算section
static const char* g_over_difen         = "g_over_difen";             //低分说明
static const char* g_over_box_desc      = "g_over_box_desc";          //宝箱说明  
static const char* g_over_jingyan       = "g_over_jingyan";           //获取经验
static const char* g_over_agan          = "g_over_agan";              //再来一局
static const char* g_over_leave         = "g_over_leave";             //离开游戏
static const char* g_over_xiaohao       = "g_over_xiaohao";           //本局消耗
static const char* g_over_times         = "g_over_times";             //倒计时

//***********字体颜色***************//
const int txt_color = 0x952200;            //普通字颜色
const int other_txt_color = 0x92671d;      //其他玩家昵称
const int box_txt_color = 0xf8d85d;        //宝箱文字
const int xiao_txt_color = 0xaa9a6f;       //本局消耗
const int contniu_txt_color = 0x8a511b;    //再来一局

//***********位置坐标**********//
const Vec2 POSTIONS[] = 
{
	Vec2(757, 861),                     //0  title
	Vec2(765, 428),                     //1  内容背景图
	Vec2(760, 771),                     //2  底分背景
	Vec2(560, 772),                     //3  底分倍数文字
	Vec2(430, 475),                     //4  自己背景
	Vec2(1125, 590),                    //5  上家背景
	Vec2(1125, 370),                    //6  下家背景
	Vec2(757, 117),                     //7  再来一局
	Vec2(310, 170),                     //8  宝箱背景
	Vec2(130, 170),                     //9  宝箱文字
	Vec2(1095,150),                     //10 获取经验文字
	Vec2(120, 280),                     //11 自己头像
	Vec2(250, 280),                     //12 自己结算
	Vec2(115, 95),                      //13 别家头像
	Vec2(255, 90),                      //14 别家结算
	Vec2(30, 190),                      //15 宝箱
};

const Vec2 POSTIONS_IMG[] = 
{
	Vec2(992, 660),                     //0 背景
	Vec2(397, 649),                     //1 自己形象位置
	Vec2(1454, 726),                    //2 关闭按钮
	Vec2(594, 461),                     //3 金币图标
	Vec2(728, 545),                     //4 倍数
	Vec2(650, 451),                     //5 金币加减数
	Vec2(780, 295),                     //6 升级图片
	Vec2(232, 100),                     //7 上家头像
	Vec2(908, 100),                     //8 下家头像
	Vec2(418, 100),                     //9 宝箱
	Vec2(735, 160),                     //10 上家VIPicon
	Vec2(1406, 105),                    //11 续局按钮
	Vec2(1412, 160),                    //12 下家VIPicon
};

const Vec2 POSTIONS_TXT[] = 
{
	Vec2(221, 285),                     //0 当前等级文字
	Vec2(405, 283),                     //1 经验值
	Vec2(760, 283),                     //2 获取经验
	Vec2(1195, 285),                    //3 下一等级
	Vec2(339, 144),                     //4 上家昵称
	Vec2(331, 68),                      //5 上家金币输赢
	Vec2(1021, 144),                    //6 下家昵称
	Vec2(1009, 68),                     //7 下家金币输赢
	Vec2(518, 86),                      //8 宝箱说明
	Vec2(1287, 30),                     //9 本局消耗
};

#endif // !__HZ_DDZ_VIEW_GAMESCENE_GameOver_GameOverLayerRes_H__

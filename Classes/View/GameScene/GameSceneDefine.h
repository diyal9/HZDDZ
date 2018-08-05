/*
 * GameSceneDefine.h
 *
 *	资源路径
 *
 *  Created on: 2015-5-11
 *      Author: wly
 */
#ifndef _DISTRIBUTESCENEDEFINE_H_
#define _DISTRIBUTESCENEDEFINE_H_
#include "cocos2d.h"

/************************************************************************/
/* 牌桌层相关                                                         */
/************************************************************************/

#define DISTIRBUTE_BETWEEN_NUM_CARD_MARGIN 15                                           //牌的数字部分与牌边缘的距离
#define DISTRIBUTE_LAST_THREE_CARDS_BG_SCALE 0.24f										//房间顶部工具条地主牌底缩小倍数
#define DISTRIBUTE_LAST_THREE_CARDS_COLOR_SCALE 0.385f									//房间顶部工具条地主牌花色缩小倍数
#define DISTRIBUTE_LAST_THREE_CARDS_NUM_SCALE 0.315f									//房间顶部工具条地主牌数字缩小倍数
#define DISTRIBUTE_OUT_CARDS_NUM_SCALE 0.315f											//出牌缩小倍数
#define DISTRIBUTE_OUT_CARDS_MAGIN 48													//出牌间距
#define DISTRIBUTE_OTHER_OUT_CARDS_ROW_MAGIN 71											//其它玩家出牌上下行间距
#define DISTRIBUTE_CARD_MARGIN 86				                                        //发完牌后，牌之间的间距
#define DISTRIBUTE_CARD_BOTTOM_MARGIN -15                                               //发牌后，手中的牌离屏幕下方的距离
#define DISTRIBUTE_CARD_SELECTED_DISTANCE  40                                           //牌选中时，往上移动的距离
#define DISTRIBUTE_CARD_PLAY_MARGIN 48                                                  //牌打到桌上时每张牌之间的间距
#define DISTRIBUTE_CARD_PLAY_CARD_POS_Y 450                                             //牌打到桌面的Y坐标
#define DISTRIBUTE_LAST_THREE_CARDS_SCALE 0.24f                                         //最后三张底牌移到顶部时的缩小倍数


#define DISTRIBUTE_CARD_MAX_NUM 54				                                        //牌堆数量
#define DISTRIBUTE_CARD_HEAP_MARGIN 5                                                   //牌堆中每张牌之间的间距
#define DISTRIBUTE_CARD_RATE 0.03f				                                        //发牌速率
#define DISTRIBUTE_CARD_MOVE_RATE 0.3f			                                        //牌移动到玩家手中速率
#define DISTRIBUTE_CARD_OTHER_PALYER_HAND_SCALE 0.25f                                   //牌在其他两个玩家手中缩放率
#define DISTRIBUTE_CARD_TURN_RATE 0.2f                                                  //发牌过程中，牌翻转的速率
#define DISTRIBUTE_CARD_COUNT 51				                                        //发牌次数，每人手中17张牌
#define DISTRIBUTE_CARD_OTHERUSER_NUM  5	                                            //其他玩家手中的牌数量
#define DISTRIBUTE_CARD_PLAY_ANGLEX -30                                                 //牌打到桌面上时与X轴的角度
#define DISTIRBUTE_CARD_PLAY_RATE 0.15f                                                 //出牌速率

#define PLAYTYPE_CHOOSE_LAIZI_DURATION   3.0f                                           //摇癞子时间
#define PLAYTYPE_CHOOSE_LAIZI_POS_X   640                                               //摇癞子控件的X坐标
#define PLAYTYPE_CHOOSE_LAIZI_POS_Y   503												//摇癞子控件的Y坐标

const static std::string g_strBackBGImg  = "res/Images/GameScene/poker_back.png";			//卡牌背景图片地址
const static std::string g_strBackBgSmallImg = "res/Images/GameScene/poker_back_s.png";		//小号的牌背景
const static std::string g_PokersTextureFormatPath = "res/Images/GameScene/pokers.plist";	//扑克牌资源

/************************************************************************/
/* 癞子相关                                                           */
/************************************************************************/
const float LAIZI_CARDS_TABLE_WIDTH = 540.0f;                                               //控件的宽度
const float LAIZI_CARDS_TABLE_HEIGHT = 364.0f;                                              //控件的高度
const float LAIZI_CARDS_TABLE_CARDS_MARGIN = 48.3f;                                         //控件中牌与牌之间的间距
const float LAIZI_CARDS_TABLE_CARDS_GROUP_MARGIN = 15.0f;                                   //控件中每组牌上下之间的间距

const static std::string g_rescalChooseGg = "res/Images/GameScene/choose_rescal_bg_9.png";	//癞子选择框背景

/************************************************************************/
/* 菜单层相关                                                         */
/************************************************************************/


/************************************************************************/
/* 顶部菜单                                                         */
/************************************************************************/


/************************************************************************/
/* 头像层                                                             */
/************************************************************************/


/************************************************************************/
/* 通用                                                 */
/************************************************************************/

/************************************************************************/
/* 癞子相关                                                           */
/************************************************************************/


/************************************************************************/
/* 表情层                                                           */
/************************************************************************/


/************************************************************************/
/* 手机状态栏相关                                                           */
/************************************************************************/
 
/************************************************************************/
/* 游戏中个人资料                                                        */
/************************************************************************/
const static char* infoBeanIcon                   = "res/Images/GameScene/PlayerInfo/infoBeanIcon.png";
const static char* infoBg                         = "res/Images/GameScene/PlayerInfo/infoBg.png";
const static char* infoLine                       = "res/Images/GameScene/PlayerInfo/infoLine.png";
const static char* infoWinPer                     = "res/Images/GameScene/PlayerInfo/infoWinPer.png";
const static char* infoHeadParty                  = "res/Images/GameScene/PlayerInfo/infoHeadParty.png";
const static char* infoHeadRight                  = "res/Images/GameScene/PlayerInfo/infoHeadRight.png";
const static char* infoBanzhuan                   = "res/Images/GameScene/PlayerInfo/infoBanzhuan.png";
const static char* infoHua                        = "res/Images/GameScene/PlayerInfo/infoHua.png";
const static char* infoJidan                      = "res/Images/GameScene/PlayerInfo/infoJidan.png";
const static char* infoPijiu                      = "res/Images/GameScene/PlayerInfo/infoPijiu.png";
const static char* infoHeadImageBg                = "res/Images/Common/userInfoHeadImageBg.png";
const static char* infoVip                        = "res/Images/Common/vip_comm.png";
const static char* infoHeadTempImage              = "res/Images/Common/head-temp.png";




#endif /* RESDEFINE_H_ */

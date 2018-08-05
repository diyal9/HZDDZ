#ifndef __HZ_DDZ_VIEW_COMMON_PROPSHOPLAYERRES_H__
#define __HZ_DDZ_VIEW_COMMON_PROPSHOPLAYERRES_H__
#include "cocos2d.h"

USING_NS_CC;

//**********资源文件**********************//
const char* propShop_list_plist = "res/Images/Lobby/Prop/proshoplayer.plist";

static const char* frameBg2                      = "res/Images/Common/frameBg2.png";
static const char* frameBg3                      = "res/Images/Common/comm_redbgp9.png";
static const char* prop_title                    = "prop_title.png";     //标题
static const char* prop_cellbg                   = "res/Images/Common/cellbgp9.png";    //Cell背景
static const char* prop_itembg                   = "prop_itembg.png";    //物品背景框
static const char* prop_saleout                  = "prop_saleout.png";   //售空
static const char* prop_spiltline                = "prop_spiltline.png"; //分隔线
static const char* prop_detailbg                 = "prop_detailbg.png";  //属性框
static const char* prop_namebg                   = "prop_namebg.png";    //名称背景
static const char* buy_closeBtn                 = "buy_closeBtn.png";       //关闭
static const char* prop_itembtn                  = "res/Images/Common/refuseBtn.png"; 
static const char* comm_graybgp9                 = "res/Images/Common/comm_graybgp9.png";
static const char* prop_itemtmp                  = "res/Images/GameScene/Chat/IconTmp.png";  //临时占位图


//出售
static const char* dialogBg                      = "res/Images/Common/dialogBg.png";
static const char* bgFrameIn                     = "res/Images/Common/bgFrameIn.png";
static const char* btn_gold                      = "res/Images/Common/btn_gold.png";




//************文本***************//

//************坐标***************//
static const Size listViewBgSize = Size(1388, 624); //ListView的大小
static const Size cellItemSize = Size(676, 184); //Item的大小

static const Size listSaleBgSize = Size(1000, 408); //出售ListVIew大小


static const int propshop_title_txt_color = 0xc96e32;
static const std::string propshop_nextflash_txt1_color = "135, 64, 42";
static const std::string propshop_nextflash_txt2_color = "234, 90, 3";
static const int propshop_sale_title_text = 0x844029;
static const int propshop_buy_title_text = 0x854c15;


#endif // __HZ_DDZ_VIEW_COMMON_PROPSHOPLAYERRES_H__

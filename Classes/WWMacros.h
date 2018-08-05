
#ifndef WWMACROS_H_
#define WWMACROS_H_
#include "cocos2d.h"

#include "Control/WWXMLUnicodeManager/WWXMLManager.h"
#include "View/Common/CardSprite/Poker.h"

/*
 * DebugMode :
 *	2  大厅到游戏界面， 打一局再退出到大厅， 如此重复
 *  3  一直在游戏中打牌，让自动续局
 */
#define DebugMode Configuration::getInstance()->getValue("cocos2d.kechu.debugmode", Value(0)).asInt()

/******************************************************************************
 * 适配方案
 *******************************************************************************/
typedef struct tagResource 
{
	cocos2d::Size size;
	std::string directory[100];
} WWResource;

//可用的资源尺寸 (考虑到包大小，每个包都应该只有自己的一套资源)
static WWResource commonResource = { cocos2d::Size(960, 640), {"res"} }; //普通
static WWResource retinaResource = { cocos2d::Size(1920, 1080), {"res_retina"} }; //高清

//设计方案
static cocos2d::Size designResolutionSize = cocos2d::Size(1920.0f, 1080.0f);

//项目字符串资源的存放路径 Resource/XML/WWString.xml//一些配置文件之类的 路劲确定后修改
const std::string WWSTRING_XML_PATH = "res/XML/WWString.xml";
//项目config配置文件地址
const std::string CONFIG_XML_PATH = "res/XML/config.xml";
//字符串获取宏
#define GetWWString(__KEY__,__SECTION__) WWXMLManager::getInstance()->getWWStringFromXML(__KEY__, __SECTION__)

/******************************************************************************
 * 设计宏
 *******************************************************************************/
#define	STANDARD_SCREEN_SIZE	 Size(960, 640)
#define	STANDARD_SCREEN_CENTER   Vec2(480,320)

#define	CUR_SCREEN_SIZE  Director::getInstance()->getVisibleSize()
#define	CUR_WIN_SIZE  Director::getInstance()->getWinSize()
#define	CUR_ORIGIN_SIZE  Director::getInstance()->getVisibleOrigin()

#define SCALE_X   CUR_SCREEN_SIZE.width/1920
#define SCALE_Y   CUR_SCREEN_SIZE.height/1080

//居中对齐的得到的坐标（@param node 贴图节点）
#define CENTER_A_CONVERT(node, x, y) Vec2(x + node->getContentSize().width * 0.5, y + node->getContentSize().height * 0.5)

//根据缩放因子换算的距离 （根据高匹配）
#define DISTANCE_CONV(x) x * SCALE_X  

//坐标转换 MoveTo的时候使用
#define POINT_CONV(x, y) Vec2(x * SCALE_X, y * SCALE_Y)   

/*
* Zorder UI创建的基本构建层架构
* 理论上OPGL渲染的时候也是按照这个来。Z越大越先渲染，越居于屏幕前。
*/
#define BASIC_ZORDER_ROOT 1   //根节点（基础节点在order）
#define BASIC_ZORDER_FRAME  100 //基础层之外功能性逻辑层Z起点
#define BASIC_ZORDER_TOP  1000 //最上层

/*
* PI 圆周率
*/
#define PI 3.141592

/******************************************************************************
 * 对象宏
 *******************************************************************************/

//第二渲染树清空节点通知
const std::string g_closeGlUIEventName = "close_all_glui";

//异地登录被挤掉线消息
const std::string g_closeDoubleLogin = "close_double_login";

//静态单例方法 获取代理实例
#ifndef SINGLE_INSTANCE_FUNC
#define SINGLE_INSTANCE_FUNC(_CLASS_) \
static _CLASS_* getInstance() \
{ \
	static _CLASS_* pRet = NULL; \
	if( !pRet ) pRet = new _CLASS_(); \
	return pRet; \
}
#endif //SINGLE_DELEGATE_INSTANCE_FUNC

//层中创建场景方法
#ifndef SCENE_FUNC
#define  SCENE_FUNC(_CLASS_) \
static Scene* scene() \
{ \
 	Scene* scene = Scene::create(); \
	_CLASS_* layer = _CLASS_::create(); \
	scene->addChild(layer); \
	return scene;\
}//SCENE_FUNC
#endif

#ifndef WW_SYNTHESIZE_READONLY_PASS_BY_REF
#define WW_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual  varType& get##funName(void)  { return varName; }
#endif

/******************************************************************************
 * 其它
 *******************************************************************************/
//设置 key
#define SET_GAME_MUSIC_KEY "game_music_key"  //游戏影响
#define SET_BG_MUSIC_KEY "bg_music_key"      //背景音乐      
#define SET_NO_MUSIC_KEY "no_music"          //静音
#define SET_SHOCT_KEY "shoct_key"            //震动
#define SET_ANIMATE_KEY "animate_key"        //动画
#define SET_SCROLL_KEY "scroll_key"          //滚报
#define SET_CONTINU_KEY "continu_key"        //自动续局

//通用字体颜色
const int SET_TXT_COLOR = 0x87402a;
const int TXT_LOGIN_COLOR = 0x5c3223;

//无色
const cocos2d::Color4B  g_clear = cocos2d::Color4B(0,0,0,0);

//根据16进制颜色值得到RGB值
inline cocos2d::Color3B GET_RGBCOLOR(int number)
{
	uint8_t red = number>>16;
	uint8_t green = number>>8;
	uint8_t blue = number;

	return cocos2d::Color3B(red,green,blue);
}

#endif /* WWMACROS_H_ */



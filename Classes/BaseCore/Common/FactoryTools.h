#ifndef _BASECORE_COMMON_FACTORY_TOOLS_h
#define _BASECORE_COMMON_FACTORY_TOOLS_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameConfig.h"
#include "WWMacros.h"

USING_NS_CC;
USING_NS_CC_EXT;

class FactoryTools {
public:
	/** 开关 
		只创建 UI ，响应需要自己在创建完开关后添加，例：
		tempSwitch->addTargetWithActionForControlEvents(this, cccontrol_selector(TestScene::valueChanged), Control::EventType::VALUE_CHANGED);
	**/
	static ControlSwitch* createSwitch(Vec2 position, int iTag = -1);

    /*常用普通按钮;*/
    static Menu* Button(char *normalFile, char *selectFile, Vec2 position, const ccMenuCallback& callback, int iTag = -1 );

    static MenuItemSprite* Button(char *normalFile, char *selectFile,char * disableFile, Vec2 position, const ccMenuCallback& callback, int iTag = -1 );
    
    static Menu* toggle(char *normalFile, char *selectFile, Vec2 position, const ccMenuCallback& callback,  bool isSelected, int iTag = -1 );
    
    static MenuItemToggle* itemToggle(char *normalFile, char *selectFile, char* pressFile1, char* pressFile2, const ccMenuCallback& callback);
    
    static Menu* toggleMenu(MenuItemToggle *itemToggle, Vec2 position, bool isSelected, int iTag);
    
    static Menu* toggleMenu(char* normarFileOne, char* normalFileTwo, char* selectFileOne, char* selectFileTwo, Vec2 position, const ccMenuCallback& callback, int iTag);
    

    /*输入框;*/
    static TextFieldTTF *AddTextField(Vec2 pos, const char *placeholder, const Size& dimensions, TextHAlignment alignment, const char *fontName, float fontSize);
    
    /*设置label显示区域;*/
    static Label* setLabelShowRect(const char *label, const Size &dimensions, TextHAlignment alignment, const char *fontName, float fontSize, Rect rect, Vec2 position);
    
    /*得到节点的Rect大小;*/
    static Rect GetRect(Node* pNode);
    
    /*得到光标的位置;*/
    static Vec2 GetCursorPos(TextFieldTTF* pTextField, const char* fontName, float  fontSize, bool oneLine = true);
    
    /**提示框;
     info: 文字自动换行;
     confirmSEL: 确认按钮的回调函数;
     cancelSEL: 取消按钮的回调函数;
     若不需要显示某一个按钮，直接给NULL;
     */
    static Menu *AlertMenu(char *info, Ref *node, SEL_MenuHandler confirmSEL, SEL_MenuHandler cancelSEL);
    
    static int CalcCharCount(const char * pszText);
    
    static bool SaveFile(const char* path, void * buffer, int len);
    static bool LoadFile(const char* path, void * buffer, int len);
    
    static unsigned int GetTime();
    
    static int CheckNetStatus();
    
    static int ReplaceStr(char *sSrc, char *sMatchStr, char *sReplaceStr);
    
    static int GetStringLine(char* sSrc, const char* fontName, float fontSize);
    static char* DealString(char* pszString);
    static float GetStringLen(char* sSrc,const char* fontName, int fontSize);

	/**得到对齐coord
		返回相对于屏幕尺寸 pAlign 对齐时的座标
		默认为 pAligh 方式居中
		nodeSize 为设置位置节点大小
		pos 为修正座标
		此方式仅为模块设计，如大厅工具条相对于屏幕
	**/
	static Vec2 getNodeAlignPos(NodeAlignMent pAlign, Size nodeSize, Vec2 pos);
};
#endif

#include "FactoryTools.h"

using namespace std;

/* 开关按钮图片路径 */
const std::string switchImageArray[] = { 
	"res/Images/set/set_switch.png",
	"res/Images/set/set_on.png",
	"res/Images/set/set_off.png",
	"res/Images/set/set_thumb.png",
};

ControlSwitch* FactoryTools::createSwitch(Vec2 position, int iTag)
{
	auto controlSwitch = ControlSwitch::create(
            Sprite::create(switchImageArray[0]),
            Sprite::create(switchImageArray[1]),
            Sprite::create(switchImageArray[2]),
            Sprite::create(switchImageArray[3]),
            Label::createWithSystemFont(" ", "Arial-BoldMT", 16),
            Label::createWithSystemFont(" ", "Arial-BoldMT", 16)
		);
	controlSwitch->setPosition(position);
    if(iTag != -1)
    {
        controlSwitch->setTag(iTag);
    }

	return controlSwitch;
}

Menu* FactoryTools::Button(char *normalFile, char *selectFile, Vec2 position, const ccMenuCallback& callback,int iTag)
{
	/*Sprite *buttonNormal = Sprite::createWithSpriteFrameName(normalFile);
    Sprite *buttonSelect = Sprite::createWithSpriteFrameName(selectFile);*/

	Sprite *buttonNormal = Sprite::create(normalFile);
    Sprite *buttonSelect = Sprite::create(selectFile);

	auto *button = MenuItemSprite::create(buttonNormal, buttonSelect, callback);
    Menu *menu = Menu::create(button, NULL);
    menu->setPosition(position);
    if(iTag != -1)
    {
        menu->setTag(iTag);
    }
    button->setUserData(menu);
    return menu;
}

MenuItemSprite* FactoryTools::Button(char *normalFile, char *selectFile,char * disableFile, Vec2 position, const ccMenuCallback& callback,int iTag)
{
    /*Sprite *btnNormal = Sprite::createWithSpriteFrameName(normalFile);
    Sprite *btnSelect = Sprite::createWithSpriteFrameName(selectFile);
    Sprite *btnDisable = Sprite::createWithSpriteFrameName(disableFile);*/
    
    Sprite *btnNormal = Sprite::create(normalFile);
    Sprite *btnSelect = Sprite::create(selectFile);
    Sprite *btnDisable = Sprite::create(disableFile);

    MenuItemSprite *pButton = MenuItemSprite::create(btnNormal, btnSelect,btnDisable, callback);
    
    if(iTag != -1)
    {
        pButton->setTag(iTag);
    }
    
    pButton->setUserData(pButton);
    
    return pButton;
}

Menu* FactoryTools::toggle(char *normalFile, char *selectFile, Vec2 position, const ccMenuCallback& callback,bool isSelected, int iTag)
{
    Sprite *buttonNormal = Sprite::createWithSpriteFrameName(normalFile);
    Sprite *buttonNormal2 = Sprite::createWithSpriteFrameName(normalFile);
    
    Sprite *buttonSelect = Sprite::createWithSpriteFrameName(selectFile);
    Sprite *buttonSelect2 = Sprite::createWithSpriteFrameName(selectFile);
      
    MenuItemSprite *menuItem = MenuItemSprite::create(buttonNormal, buttonNormal2);
    MenuItemSprite *menuItem2 = MenuItemSprite::create(buttonSelect, buttonSelect2);
    
    MenuItemToggle *tog = MenuItemToggle::createWithCallback(callback, menuItem, menuItem2, NULL);
    if(isSelected)
	{
        tog->setSelectedIndex(1);
    }
	else
	{
        tog->setSelectedIndex(0);
    }

    Menu *menu = Menu::create(tog);
    menu->setPosition(position);
    if(iTag != -1)
    {
        menu->setTag(iTag);
    }
    tog->setUserData(menu);
    return menu;
}

MenuItemToggle* FactoryTools::itemToggle(char *normalFile, char *selectFile, char* pressFile1, char* pressFile2, const ccMenuCallback& callback)
{
    Sprite *buttonNormal = Sprite::createWithSpriteFrameName(normalFile);
    Sprite *buttonNormal2;
    if(pressFile1)
    {
        buttonNormal2 = Sprite::createWithSpriteFrameName(pressFile1);
    }else
    {
        buttonNormal2 = Sprite::createWithSpriteFrameName(normalFile);
    }
    
    Sprite *buttonSelect = Sprite::createWithSpriteFrameName(selectFile);
    Sprite *buttonSelect2 ;
    if(pressFile2)
    {
        buttonSelect2 = Sprite::createWithSpriteFrameName(pressFile2);
    }
	else
    {
        buttonSelect2 = Sprite::createWithSpriteFrameName(selectFile);
    }
    
    
    MenuItemSprite *menuItem = MenuItemSprite::create(buttonNormal, buttonNormal2);
    MenuItemSprite *menuItem2 = MenuItemSprite::create(buttonSelect, buttonSelect2);
    
    MenuItemToggle *itemToggle = MenuItemToggle::createWithCallback(callback, menuItem, menuItem2, NULL);
    return itemToggle;
}

Menu* FactoryTools::toggleMenu(cocos2d::MenuItemToggle *itemToggle, cocos2d::Vec2 position, bool isSelected, int iTag)
{
    if(isSelected)
	{
        itemToggle->setSelectedIndex(1);
    }
	else
	{
        itemToggle->setSelectedIndex(0);
    }
	Menu *menu = Menu::createWithItem(itemToggle);
    menu->setPosition(position);
    if (iTag != -1) 
    {
        menu->setTag(iTag);
    }
    itemToggle->setUserData(menu);
    return menu;
}

TextFieldTTF * FactoryTools::AddTextField(Vec2 pos, const char *placeholder, const cocos2d::Size &dimensions, TextHAlignment alignment, const char *fontName, float fontSize)
{
	Label *pTTf = Label::createWithTTF(placeholder ,fontName, fontSize);

	Size dimSize = Size(dimensions.width, pTTf->getDimensions().height);//the zones which will display the text.
    
    TextFieldTTF *pTextField = TextFieldTTF::textFieldWithPlaceHolder(placeholder, dimSize, alignment, fontName, fontSize);

	//pTextField->setAnchorPoint(p(0.0f,0.5f));

    pTextField->setPosition(pos);
	pTextField->setColor(Color3B::BLACK);
    
    return pTextField;
}

Label* FactoryTools::setLabelShowRect(const char *label, const Size &dimensions, TextHAlignment alignment, const char *fontName, float fontSize, Rect rect, Vec2 position)
{
	//Label* pLabel = Label::createWithSystemFont(label,fontName,fontSize,dimensions,alignment,TextVAlignment::TOP);
	Label* pLabel = Label::createWithTTF(label,"res/Fonts/FounderSemiBold.ttf", fontSize, Size::ZERO,alignment,TextVAlignment::CENTER);
	//pLabel->setContentSize(Size::ZERO)
    //pLabel->setTextureRect(rect);
    pLabel->setPosition(position);
    return pLabel;
}

Rect FactoryTools::GetRect(Node* pNode)
{
    Rect rc;
    
    rc.origin = pNode->getPosition(); 
    rc.size = pNode->getContentSize();
    rc.origin.x -= rc.size.width/2;
    rc.origin.y -= rc.size.height/2;
    rc.size.height +=40;
    rc.origin.y -=20;

    return rc;
}

Vec2 FactoryTools::GetCursorPos(cocos2d::TextFieldTTF *pTextField, const char *fontName, float fontSize , bool oneLine)
{
    Vec2 pos;
    
	float stand_width = pTextField->getContentSize().width;
    float stand_height = pTextField->getContentSize().height;
    pos = pTextField->getPosition();

    float distance = pos.x - stand_width/2;
    pos.x -= stand_width/2;
    
    string name = pTextField->getString();
    Label* pTTf;
    if(oneLine)
    {
        pTTf = Label::createWithTTF(name.c_str() ,fontName, fontSize);
        pos.x += pTTf->getContentSize().width;
    }
    
    if(!oneLine)
    {
        pos.y += stand_height/2;
        if(strlen(name.c_str()) == 0)
        {
            name = "i";
            pTTf = Label::createWithTTF(name.c_str(), fontName, fontSize);
            pos.y-=(pTTf->getContentSize().height/2);
        }
		else
        {
            name.append("\n");
            char * cName = (char*)name.c_str();
            char ch = 0;
            int cNum = 0;
            int cNumPre = 0;
            int lineNum = 0;
            float fWidth=0;
            vector<string> vecStr;
            
            while ((ch = *cName))
            {
                CC_BREAK_IF(!ch);
                if(0x80 != (0xC0 & ch))
                {
                    if(cNum > 0)
                    {
                        
                        std::string tempName(name.c_str(), cNum);
                        pTTf = Label::createWithTTF(tempName.c_str() ,fontName, fontSize);
                        fWidth = pTTf->getContentSize().width;
                        
                        if(fWidth > stand_width)
                        {
                            ++lineNum;
                            std::string tempStr2(name, cNumPre);
                            vecStr.push_back(tempStr2);
                            name = tempStr2;
                            cName = (char*)name.c_str();
                            ch = 0;
                            cNum = 0;
                            cNumPre = 0;
                            continue;
                        }
                        
                        cNumPre = cNum;
                    }
                }
                ++cName;
                ++cNum;
            }
            
            if(vecStr.size() == 0)
            {
                vecStr.push_back((char*)name.c_str());
            }
            
            pTTf = Label::createWithTTF((vecStr.at(vecStr.size()-1)).c_str(), fontName, fontSize);
            pos.y-=((pTTf->getContentSize().height/2)+lineNum*(pTTf->getContentSize().height));
            pos.x =distance+ pTTf->getContentSize().width;
        }
    }
    
    return pos;
}



Menu* FactoryTools::toggleMenu(char *normarFileOne, char *normalFileTwo, char *selectFileOne, char *selectFileTwo, cocos2d::Vec2 position, const ccMenuCallback& callback, int iTag)
{
    Sprite* normal1 = Sprite::createWithSpriteFrameName(normarFileOne);
    Sprite* normal2 = Sprite::createWithSpriteFrameName(normalFileTwo);
    
    Sprite* select1 = Sprite::createWithSpriteFrameName(selectFileOne);
    Sprite* select2 = Sprite::createWithSpriteFrameName(selectFileTwo);
    
    MenuItemSprite* menuItem1 = MenuItemSprite::create(normal1, normal2);
    MenuItemSprite* menuItem2 = MenuItemSprite::create(select1, select2);
    
    MenuItemToggle *toggle = MenuItemToggle::createWithCallback(callback, menuItem1, menuItem2, NULL);
    Menu* menu = Menu::create(toggle);
    toggle->setUserData(menu);
    menu->setPosition(position);
    menu->setTag(iTag);
    return menu;
}

Menu* FactoryTools::AlertMenu(char *info, Ref *node, SEL_MenuHandler confirmSEL, SEL_MenuHandler cancelSEL)
{
    MenuItemSprite *pConfirmButton = NULL;
    MenuItemSprite *pCancelButton = NULL;
    Menu *pAlertMenu = NULL;
    
    Sprite *back = Sprite::createWithSpriteFrameName("alert-back.png");
    Sprite *back1 =Sprite::createWithSpriteFrameName("alert-back.png"); 
    MenuItemSprite *pBackItem = MenuItemSprite::create(back, back1);
    pBackItem->setPosition(STANDARD_SCREEN_CENTER);
	pBackItem->setEnabled(false);
    
	Label* pMsgLabel = Label::createWithSystemFont(info,GAME_FONT_NAME, GAME_FONT_SIZE_24, Size(280 ,70 ), TextHAlignment::LEFT,TextVAlignment::TOP);
    MenuItemLabel *pMsgItem = MenuItemLabel::create(pMsgLabel);
	pMsgItem->setDisabledColor(Color3B(255, 255, 255));
    pMsgItem->setEnabled(false);
    pMsgItem->setPosition(Vec2(STANDARD_SCREEN_SIZE.width/2, STANDARD_SCREEN_CENTER.y+20));
    
    Sprite *confirmNormal = Sprite::createWithSpriteFrameName("confirm-1.png");
    Sprite *confirmSelect = Sprite::createWithSpriteFrameName("confirm-2.png");
    Sprite *cancelNormal = Sprite::createWithSpriteFrameName("cancel-1.png");
    Sprite *cancelSelect = Sprite::createWithSpriteFrameName("cancel-2.png");
    
    if (confirmSEL && cancelSEL) 
	{  /*确定 & 取消按钮同时存在;*/
        
        //pConfirmButton = MenuItemSprite::create(confirmNormal, confirmSelect, node, confirmSEL);
        pConfirmButton = MenuItemSprite::create(confirmNormal, confirmSelect);
        pConfirmButton->setPosition(Vec2(STANDARD_SCREEN_SIZE.width/2-70, STANDARD_SCREEN_CENTER.y-60));
        
        //pCancelButton = MenuItemSprite::create(cancelNormal, cancelSelect, node, cancelSEL);
        pCancelButton = MenuItemSprite::create(cancelNormal, cancelSelect);
        pCancelButton->setPosition(Vec2(STANDARD_SCREEN_SIZE.width/2+70, STANDARD_SCREEN_CENTER.y-60));
        
        pAlertMenu = Menu::create(pBackItem, pMsgItem, pConfirmButton, pCancelButton, NULL);
    } else if (confirmSEL) 
	{    /*只有确定按钮;*/
        
        //pConfirmButton = MenuItemSprite::create(confirmNormal, confirmSelect, node, confirmSEL);
        pConfirmButton = MenuItemSprite::create(confirmNormal, confirmSelect);
        pConfirmButton->setPosition(Vec2(STANDARD_SCREEN_SIZE.width/2, STANDARD_SCREEN_CENTER.y-60));
        
        pAlertMenu = Menu::create(pBackItem, pMsgItem, pConfirmButton, NULL);
    } 
	else if (cancelSEL) 
	{    /*只有取消按钮;*/
        //pCancelButton = MenuItemSprite::create(cancelNormal, cancelSelect, node, cancelSEL);
        pCancelButton = MenuItemSprite::create(cancelNormal, cancelSelect);
        pCancelButton->setPosition(Vec2(STANDARD_SCREEN_SIZE.width/2, STANDARD_SCREEN_CENTER.y-60));
        
        pAlertMenu = Menu::create(pBackItem, pMsgItem, pCancelButton, NULL);
    } 
	else 
	{    /*无按钮;*/
        pAlertMenu = Menu::create(pBackItem, pMsgItem, NULL);
    }
    
    pAlertMenu = Menu::create(pBackItem, pMsgItem, NULL);

    pAlertMenu->setContentSize(STANDARD_SCREEN_SIZE);
    pAlertMenu->setAnchorPoint(Vec2(0, 0));
    
	//pAlertMenu->setPosition(CCPointZero);
    //pAlertMenu->setPosition(Vec2(0,0));
    
    return pAlertMenu;
}


int FactoryTools::CalcCharCount(const char * pszText)
{
    int n = 0;
    char ch = 0;
    while ((ch = *pszText))
    {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch))
        {
            ++n;
        }
        ++pszText;
    }
    return n;
}

bool FactoryTools::SaveFile(const char* path, void * buffer, int len) //path 文件路径 T要存储的数据对象
{
    remove(path);
    FILE* file = fopen(path, "wb+");
    if(file != NULL)
    {
        fwrite(buffer, 1, len, file);
        fclose(file);
        return true;
    }
    return false;
}

bool FactoryTools::LoadFile(const char* path, void * buffer, int len)//path 文件路径 T要从文件读取的数据对象 
{
    FILE* file = fopen(path, "rb+");
    if(file != NULL)
    {
        fread(buffer, 1, len, file);
        fclose(file);
        return true;
    }
    return false;
}

unsigned int FactoryTools::GetTime()//Andy-14
{
	struct timeval nowTimeval;
    gettimeofday(&nowTimeval, NULL);
    struct tm * tm;
    time_t time_sec ;
    time_sec = nowTimeval.tv_sec;
    tm = localtime(&time_sec);
  /*  nMinute = tm->tm_min;
    nSecond = tm->tm_sec;
    nHour = tm->tm_hour; */

	/*_timeval now;
	Time::gettimeofdayCocos2d(&now,NULL);*/
	unsigned int iRt = tm->tm_sec*1000 + tm->tm_sec/1000;
	return iRt;
}

int FactoryTools::ReplaceStr(char *sSrc, char *sMatchStr, char *sReplaceStr)
{
    int iStringLen;
    char cNewString[1024*2];
    char* FindPos;
    FindPos = (char*)strstr(sSrc, sMatchStr);
    if ((!FindPos) || (!sMatchStr)) 
    {
        return -1;
    }
    
    while (FindPos) 
    {
        memset(cNewString, 0, sizeof(cNewString));
        iStringLen = FindPos-sSrc;
        strncpy(cNewString, sSrc, iStringLen);
        strcat(cNewString, sReplaceStr);
        strcat(cNewString, FindPos+strlen(sMatchStr));
        strcpy(sSrc, cNewString);
        
        FindPos = (char*)strstr(sSrc, sMatchStr);
    }
    return 0;
}

int FactoryTools::GetStringLine(char* sSrc, const char* fontName, float fontSize)
{
    ReplaceStr(sSrc, (char*)"<br />", (char*)"");
    ReplaceStr(sSrc, (char*)"&nbsp;", (char*)"");
    ReplaceStr(sSrc, (char*)"<p>", (char*)"");
    ReplaceStr(sSrc, (char*)"</p>", (char*)"");
    
    ReplaceStr(sSrc, (char*)"\r\n", (char*)"nil\n");
    ReplaceStr(sSrc, (char*)"\r", (char*)"nil\n");  //nil 是为了避免当2个换行符连在一起的时候空行被忽略，没有加入对邮件行数的计算
    
    int iLineNum = 0;
    int iTempLineNum = 0;
    char* token = NULL;
    
    Label* pTtf = Label::createWithTTF("nil", fontName, fontSize);
    float fTempLenght = pTtf->getContentSize().width;
    
    token = strtok(sSrc, (char*)"\n");
    token = DealString(token);
    while (token) 
    {
        float fLenght = GetStringLen(token, fontName, fontSize);
        
        iTempLineNum++;
        int iTempLine = (fLenght-fTempLenght)/500;
        iLineNum += iTempLine;
        
        token = strtok(NULL, (char*)"\n");
    }
    
    return (iLineNum+iTempLineNum);
}

float FactoryTools::GetStringLen(char *sSrc,const char *fontName, int fontSize)
{
    float fTotalLen = 0;
    int cNum = 0;
    char ch = 0;
    while ((ch = *sSrc)) // ch指向sSrc的第一个字节
    {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch))
        {
            if (cNum > 0) 
            {
                string sTempStr(sSrc, cNum);
                Label* pTtf = Label::createWithTTF(sTempStr.c_str(), fontName, fontSize);
                float fLenght = pTtf->getContentSize().width;
                fTotalLen += fLenght;
                cNum = 0;
                ch = 0;
            }
        }
        ++cNum;
        ++sSrc;
    }    
    
    return fTotalLen;
}

char* FactoryTools::DealString(char* pszString)  //去除字符串为首的空格
{
    for(size_t i = 0; i < strlen(pszString);i++)
    {
        if(pszString[0] == 32)
        {
            pszString = pszString+1;
            
        }
		else 
		{
            break;
        }
    }
    printf("%s", pszString);
    return pszString;
}

Vec2 FactoryTools::getNodeAlignPos(NodeAlignMent pAlign, Size nodeSize, Vec2 pos)
{
	Vec2 tempPos;
	switch (pAlign)
	{
	case NodeAlignMent::LEFT:
		tempPos = Vec2(nodeSize.width*0.5, CUR_SCREEN_SIZE.height*0.5);
		break;
	case NodeAlignMent::RIGHT:
		tempPos = Vec2(-nodeSize.width*0.5, CUR_SCREEN_SIZE.height*0.5);
		break;
	case NodeAlignMent::CENTER:
		tempPos = Vec2(CUR_SCREEN_SIZE.width*0.5, CUR_SCREEN_SIZE.height*0.5);
		break;
	case NodeAlignMent::TOP:
		tempPos = Vec2(CUR_SCREEN_SIZE.width*0.5 - nodeSize.width*0.5, CUR_SCREEN_SIZE.height - nodeSize.height*0.5);
		break;
	case NodeAlignMent::BOTTOM:
		tempPos = Vec2(CUR_SCREEN_SIZE.width*0.5 - nodeSize.width*0.5, -nodeSize.height*0.5);
		break;
	default:
		tempPos = Vec2::ZERO;
		break;
	}

	tempPos.x += pos.x;
	tempPos.y += pos.y;

	return tempPos;
}

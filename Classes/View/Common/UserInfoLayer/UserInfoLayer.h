/*! @file
********************************************************************************
<PRE>
模块名       :  基本Layer
文件名       :  UserInfoLayer.h
文件实现功能 :  个人资料Layer
作者         :  Toby
版本         :  1.0
--------------------------------------------------------------------------------
备注         : 登入用户个人档案Layer
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/05/19   1.0      Toby                新建
</PRE>
*******************************************************************************/
#ifndef __HZ_DDZ_VIEW_Common_UserInfoLayer_UserInfoLayer_H__
#define __HZ_DDZ_VIEW_Common_UserInfoLayer_UserInfoLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "BaseCore/WWBGUI/WWFrameBase.h"


using namespace cocos2d::ui;

enum 
{
	USER_INFO_HEAD_IMAGE_MOD,            //头像编辑
	USER_INFO_HEAD_IMAGE_MOD2,
	USER_INFO_NICK_NAME_MOD,             //昵称编辑
	USER_INFO_LOGIN_OUT,                 //注销账号
	USER_INFO_BIND,                      //绑定账号
	USER_INFO_CHANGE_PWD,                //修改密码
	USER_INFO_HAPPY_BTN,                 //欢乐斗地主
	USER_INFO_LAIZI_BTN,                 //癞子斗地主
	USER_INFO_BISAI_BTN,                 //比赛
	USER_INFO_HELP_BTN                   //帮助
};

enum
{
	TOGGLE_HAPPY= 500,
	TOGGLE_LAIZI,
	TOGGLE_BISAI,
	BG_TAG
};

class UserInfoLayer : public WWFrameBase
{
public:
	UserInfoLayer();
	virtual ~UserInfoLayer();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	CREATE_FUNC(UserInfoLayer);
	
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchCancelled(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	void createView();
	void ButtonDepress(Ref * theBtn);
	void onTouchSpriteDepress(Ref * theBtn);
	void onLoginOut(); //注销操作
	void reflashHappyNodes();
	void reflashLaiziNodes();
	void reflashBisaiNodes();
	void removeNodes();
	
	void maleCallback(Ref* sender, CheckBox::EventType controlEvent);
	void femaleCallback(Ref* sender, CheckBox::EventType controlEvent);
	
private:
	CC_SYNTHESIZE(Sprite*,m_pHuanleBtn,mPHuanleBtn);
	CC_SYNTHESIZE(Sprite*,m_pLaiziBtn,mPLaiziBtn);
	CC_SYNTHESIZE(Sprite*,m_pBisaiBtn,mPBisaiBtn);
	CC_SYNTHESIZE(Sprite*,bg,mBg);
	CC_SYNTHESIZE(Label*, m_pHuanleTxt,mPHuanleTxt);
	CC_SYNTHESIZE(Label*,m_pLaiziTxt,mPLaiziTxt);
	CC_SYNTHESIZE(Label*,m_pBisaiTxt,mPBisaiTxt);
	CC_SYNTHESIZE(CheckBox*, m_pMaleCheckBox, mMaleCheckBox);
	CC_SYNTHESIZE(CheckBox*, m_pFemaleCheckBox, mFemaleCheckBox);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<Node*>,m_happyNodes,mHappyNodes);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<Node*>,m_laiziNodes,mLaiziNodes);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<Node*>,m_bisaiNodes,mBisaiNodes);
};

#endif // !__HZ_DDZ_VIEW_Common_UserInfoLayer_UserInfoLayer_H__

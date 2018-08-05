/*! @file
********************************************************************************
<PRE>
模块名       :  基本Layer
文件名       :  FriendLayer.h
文件实现功能 :  好友Layer
作者         :  Toby
版本         :  1.0
--------------------------------------------------------------------------------
备注         :  好友Layer
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/07/06   1.0      Toby                新建
</PRE>
*******************************************************************************/
#ifndef __HZ_DDZ_VIEW_Common_FriendLayer_FriendLayer_H__
#define __HZ_DDZ_VIEW_Common_FriendLayer_FriendLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "BaseCore/WWBGUI/WWFrameBase.h"
#include "../../../Model/WWDataModel/WWGD/GD.h"


using namespace cocos2d::ui;


enum FRIEND_BTN_TAG
{
	FRIEND_ADD_BTN = 200,//添加好友
	FRIEND_SEACH_BTN     //搜索好友
};

enum 
{
	FRIEND_LAST_APPLY = 300, //申请列表上一页
	FRIEND_NEXT_APPLY,       //申请列表下一页
	FRIEND_LAST_LIST,        //好友列表上一页
	FRIEND_NEXT_LIST         //好友列表下一页
};

enum
{
	FRIEND_ALL_REFUSE = 400,  //全部拒绝
	FRIEND_ALL_AGREE,         //全部同意
	FRIEND_ALL_GET,           //一键送出
	FRIEND_ALL_RESAVE         //一键收
};

class FriendLayer : public WWFrameBase
{
public:
	FriendLayer();
	virtual ~FriendLayer();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	CREATE_FUNC(FriendLayer);
	
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchCancelled(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	void createView();
	
	Layout* createItem(UserBasicInfoData* userInfo, int index);
	void reflashListView();

	void reflashFriendApply();//好友申请页面
	void reflashFriendList();// 好友列表
	void reflashFriendSeach();//好友查找
	void removeNodes();
	
	void onTouchSpriteDepress(Ref * theBtn);  // touchSprite按纽响应
	void listItemClick(Ref* pSender, ListView::EventType type);

	//拒绝和同意好友响应
	void onTouchSpriteBuddyAgree(Ref * theBtn);  
	void onTouchSpriteBuddyUagree(Ref * theBtn);  

	//领取和赠送相应
	void onTouchSpriteBuddyGet(Ref * theBtn);  
	void onTouchSpriteBuddyGive(Ref * theBtn);  

	//推荐好友列表点击添加相应
	void onTouchSpriteBuddyAdd(Ref * theBtn); 
	
private:
	CC_SYNTHESIZE(Sprite*,bg,mBg);
	CC_SYNTHESIZE(ListView*, m_pListView, pListView);
	
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<Node*>,m_friendApply,mFriendApply);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<Node*>,m_friendList,mFriendList);
	WW_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<Node*>,m_friendSeach,mFriendSeach);

	WW_SYNTHESIZE_READONLY_PASS_BY_REF(Vector<UserBasicInfoData*>,m_userList,mUserList);
};

#endif // !__HZ_DDZ_VIEW_Common_FriendLayer_FriendLayer_H__

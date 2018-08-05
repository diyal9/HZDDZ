#include "FriendLayer.h"
#include "FriendLayerRes.h"
#include "ui/CocosGUI.h"
#include "../../../BaseCore/Common/UICoordConfig.h"
#include "../UserInfoLayer/UserInfoLayer.h"
#include "Control/WWRequest/MsgCommonRequest.h"


#define BUDDY_AGREE_TAG 10000 //好友同意tag
#define BUDDY_UAGREE_TAG 2000 //好友拒绝tag

FriendLayer::FriendLayer():	
	m_pListView(nullptr)
{

}

FriendLayer::~FriendLayer()
{
	
}

bool FriendLayer::init()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(friend_plist);
	if ( !WWFrameBase::init() )
	{
		return false;
	}

	createView();
	
	return true;
}

void FriendLayer::createView()
{

	MsgCommonRequest::sendBuddyInfoReq(0,1,10,0);
	//添加背景
	bg = this->getpBg();
	//标题 //添加好友和好友
	auto titleSp = Sprite::createWithSpriteFrameName("friend_list_title.png");
	if (true)
	{
		titleSp->setSpriteFrame("friend_add_title.png");
	}
	titleSp->setPosition(FRIEND_POSTIONS[0]);
	bg->addChild(titleSp,BASIC_ZORDER_ROOT);
	//内容背景
	auto contentBg = Sprite::create(friend_bg1);
	contentBg->setPosition(Vec2(770,420));
	bg->addChild(contentBg,BASIC_ZORDER_ROOT);
	//灰色背景
	auto gayBg = Scale9Sprite::createWithSpriteFrameName("friend_content_bg.png");
	gayBg->setContentSize(Size(1400,666));
	gayBg->setScale9Enabled(true);
	gayBg->setPosition(Vec2(772,400));
	bg->addChild(gayBg,BASIC_ZORDER_ROOT +1);
	//红色背景
	auto redBg = Scale9Sprite::createWithSpriteFrameName("friend_red_bg.png");
	redBg->setContentSize(Size(1386,576));
	redBg->setScale9Enabled(true);
	redBg->setPosition(Vec2(772,438));
	bg->addChild(redBg,BASIC_ZORDER_ROOT+2);
	
	//当没有好友的时候或者有好友申请  显示推荐好友字和好友申请文字以及背景
	if (false)
	{
		auto tuijBg = Scale9Sprite::createWithSpriteFrameName("friend_bg_title.png");
		tuijBg->setContentSize(Size(1386,41));
		tuijBg->setScale9Enabled(true);
		tuijBg->setPosition(Vec2(redBg->getPositionX(),redBg->getPositionY() + redBg->getContentSize().height*0.5f - tuijBg->getContentSize().height*0.5));
		bg->addChild(tuijBg,BASIC_ZORDER_ROOT+3);
	}

	//好友列表ListView
	//ListView
	if (!m_pListView)
	{
		m_pListView = ListView::create();
		m_pListView->setDirection(ui::ScrollView::Direction::VERTICAL);
		m_pListView->setTouchEnabled(true);
		m_pListView->setBounceEnabled(true);//是否回弹
		m_pListView->setBackGroundImageScale9Enabled(true);
		m_pListView->setAnchorPoint(Vec2(0.5,0.5));
		m_pListView->setItemsMargin(1.0f);
		m_pListView->setContentSize(Size(1372,520));
		m_pListView->setPosition(Vec2(775,420));
		m_pListView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(FriendLayer::listItemClick,this));
		bg->addChild(m_pListView,BASIC_ZORDER_FRAME);
	}

	m_userList.clear();
	for (int i = 0; i < 10; i++)
	{
		UserBasicInfoData* info = new UserBasicInfoData();
		info->setNickName(StringUtils::format("nick name %d",i));
		info->setLastTime(StringUtils::format("%d mins ago",i));
		info->setLevel(i+ 1);
		m_userList.pushBack(info);
		info->autorelease();
	}

    //底部

	reflashListView();
	
	//reflashFriendApply();
	//reflashFriendList();
	reflashFriendSeach();
}

Layout* FriendLayer::createItem(UserBasicInfoData* userInfo, int index)
{
	if(!userInfo) return NULL;
	Layout* itemLayout = Layout::create();//Item布局
	itemLayout->setTouchEnabled(true);
	itemLayout->setContentSize(Size(1360,124));
	//item背景
	auto itemBg = Scale9Sprite::createWithSpriteFrameName("friend_item_bg.png");
	itemBg->setContentSize(Size(1360,124));
	itemBg->setScale9Enabled(true);
	itemBg->setPosition(Vec2(itemLayout->getContentSize().width/2,itemLayout->getContentSize().height/2));
	//头像
	auto headIcon = Sprite::create(head_temp);
	headIcon->setScale(0.55f);
	headIcon->setPosition(Vec2(92,62));
	itemBg->addChild(headIcon);
	//昵称背景图
	auto nickNameBg = Sprite::createWithSpriteFrameName("friend_name_bg.png");
	nickNameBg->setPosition(Vec2(352,57));
	itemBg->addChild(nickNameBg,BASIC_ZORDER_ROOT);
	//昵称
	auto nickNameLabel = Label::createWithTTF("nick name ","res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	nickNameLabel->setColor(GET_RGBCOLOR(txt_num));
	nickNameLabel->setAnchorPoint(Vec2(0,0.5f));
	nickNameLabel->setPosition(Vec2(148,62));
	itemBg->addChild(nickNameLabel,BASIC_ZORDER_ROOT+1);
	nickNameLabel->setString(userInfo->getNickName());
	//等级
	auto lvLabel = Label::createWithTTF(StringUtils::format(GetWWString(friend_lv,friend_section).c_str(),userInfo->getLevel()).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	lvLabel->setColor(GET_RGBCOLOR(txt_num));
	lvLabel->setAnchorPoint(Vec2(0,0.5f));
	lvLabel->setPosition(Vec2(nickNameLabel->getPositionX() + nickNameLabel->getContentSize().width*1.3f,nickNameLabel->getPositionY()));
	itemBg->addChild(lvLabel,BASIC_ZORDER_ROOT+1);
	//上次登入时间
	auto lastLoginTime = Label::createWithTTF(StringUtils::format(GetWWString(friend_last_logintime,friend_section).c_str(),userInfo->getLastTime().c_str()).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	lastLoginTime->setColor(GET_RGBCOLOR(txt_num));
	lastLoginTime->setAnchorPoint(Vec2(0,0.5f));
	lastLoginTime->setPosition(Vec2(572,62));
	itemBg->addChild(lastLoginTime,BASIC_ZORDER_ROOT+1);
	//按钮  需要看情况  好友申请的 是 同意和拒绝       
	//好友列表的 是 赠送和领取  已赠送和已领取
	// 推荐好友的 是  添加
	if (false)//是好友申请
	{
		//同意按钮
		auto agreeBtn = WWTouchSprite::createWithFile(comm_siglebule_btn,this,callfuncO_selector(FriendLayer::onTouchSpriteBuddyAgree));
		agreeBtn->setPosition(Vec2(1035,63));
		itemBg->addChild(agreeBtn,BASIC_ZORDER_ROOT,BUDDY_AGREE_TAG + index);
		//同意字
		auto agreeLabel = Label::createWithTTF(GetWWString(friend_agree,friend_section).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		agreeLabel->setColor(GET_RGBCOLOR(txt_green));
		agreeLabel->setPosition(Vec2(0,0));
		agreeBtn->addChild(agreeLabel,BASIC_ZORDER_ROOT +1);
		//拒绝按钮
		auto uagreeBtn = WWTouchSprite::createWithFile(comm_sigleyellow_btn,this,callfuncO_selector(FriendLayer::onTouchSpriteBuddyUagree));
		uagreeBtn->setPosition(Vec2(1231,63));
		//拒绝字
		auto uagreeLabel = Label::createWithTTF(GetWWString(friend_uagree,friend_section).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		uagreeLabel->setColor(GET_RGBCOLOR(txt_agree));
		uagreeLabel->setPosition(Vec2(0,0));
		uagreeBtn->addChild(uagreeLabel,BASIC_ZORDER_ROOT +1);
		itemBg->addChild(uagreeBtn,BASIC_ZORDER_ROOT,BUDDY_UAGREE_TAG + index);
	}
	if (false)//如果是好友列表 则显示 领取和赠送  已领取和已赠送
	{
		if (true)//没有领取显示按钮 否则显示 已领取
		{
			//领取按钮
			auto agreeBtn = WWTouchSprite::createWithFile(comm_siglebule_btn,this,callfuncO_selector(FriendLayer::onTouchSpriteBuddyGet));
			agreeBtn->setPosition(Vec2(1035,63));
			itemBg->addChild(agreeBtn,BASIC_ZORDER_ROOT,BUDDY_AGREE_TAG + index);
			//领取字
			auto agreeLabel = Label::createWithTTF(GetWWString(friend_get,friend_section).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			agreeLabel->setColor(GET_RGBCOLOR(txt_green));
			agreeLabel->setPosition(Vec2(0,0));
			agreeBtn->addChild(agreeLabel,BASIC_ZORDER_ROOT +1);			
		}
		else
		{
			auto getedLabel = Label::createWithTTF(GetWWString(friend_geted,friend_section).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			getedLabel->setColor(GET_RGBCOLOR(txt_ed));
			getedLabel->setPosition(Vec2(1035,63));
			itemBg->addChild(getedLabel,BASIC_ZORDER_ROOT);
		}

		if (true)
		{
			//赠送按钮
			auto uagreeBtn = WWTouchSprite::createWithFile(comm_sigleyellow_btn,this,callfuncO_selector(FriendLayer::onTouchSpriteBuddyGive));
			uagreeBtn->setPosition(Vec2(1231,63));
			//赠送字
			auto uagreeLabel = Label::createWithTTF(GetWWString(friend_give,friend_section).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			uagreeLabel->setColor(GET_RGBCOLOR(txt_agree));
			uagreeLabel->setPosition(Vec2(0,0));
			uagreeBtn->addChild(uagreeLabel,BASIC_ZORDER_ROOT +1);
			itemBg->addChild(uagreeBtn,BASIC_ZORDER_ROOT,BUDDY_UAGREE_TAG + index);
		}
		else
		{
			auto givedLabel = Label::createWithTTF(GetWWString(friend_gived,friend_section).c_str(),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
			givedLabel->setColor(GET_RGBCOLOR(txt_ed));
			givedLabel->setPosition(Vec2(1231,63));
			itemBg->addChild(givedLabel,BASIC_ZORDER_ROOT);
		}
		
	}
	if (true)
	{
		//添加好友
		auto addBtn = WWTouchSprite::createWithFile(matchBtn,this,callfuncO_selector(FriendLayer::onTouchSpriteBuddyAdd));
		addBtn->setTag(BUDDY_AGREE_TAG + index);
		addBtn->setPosition(Vec2(1231,63));
		itemBg->addChild(addBtn,BASIC_ZORDER_ROOT+1);
		auto allLabel = Label::createWithTTF(GetWWString(friend_add_txt,friend_section).c_str(),"res/Fonts/FounderSemiBold.ttf",32,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		allLabel->setColor(GET_RGBCOLOR(txt_agree));
		allLabel->setPosition(Vec2(0,0));
		addBtn->addChild(allLabel,BASIC_ZORDER_ROOT+1);
	}

	itemLayout->addChild(itemBg);
	return itemLayout;
}

void FriendLayer::reflashListView()
{
	m_pListView->removeAllItems();
	
	int count = m_userList.size();
	for (int i = 0; i < count; i++)
	{
		UserBasicInfoData* userInfo = m_userList.at(i);
		m_pListView->pushBackCustomItem(createItem(userInfo,i + 1));
	}
}

void FriendLayer::listItemClick(Ref* pSender, ListView::EventType type)
{
	if (type == ListView::EventType::ON_SELECTED_ITEM_END)
	{
		auto userInfo = UserInfoLayer::create();
		userInfo->setPosition(Vec2(0,0));
		this->getParent()->addChild(userInfo,BASIC_ZORDER_FRAME);
	}
	
}

void FriendLayer::onTouchSpriteBuddyAgree(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	int index = tagId - BUDDY_AGREE_TAG - 1;//获取到下标
	//TO DO 发送同意加好友消息 同时把该相从列表中移除
	m_userList.eraseObject(m_userList.at(index));
	reflashListView();
}

void FriendLayer::onTouchSpriteBuddyUagree(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	
	int index = tagId - BUDDY_UAGREE_TAG - 1;//获取到下标
	//TO DO 发送拒绝加好友消息 同时把该相从列表中移除
	m_userList.eraseObject(m_userList.at(index));
	reflashListView();
}

void FriendLayer::onTouchSpriteBuddyGet(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	int index = tagId - BUDDY_AGREE_TAG - 1;//获取到下标
	//To Do 修改该项为已领取并且发送已领取消息 然后刷新列表
}

void FriendLayer::onTouchSpriteBuddyGive(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();	
	int index = tagId - BUDDY_UAGREE_TAG - 1;//获取到下标
	//To Do 修改该项为已赠送并且发送已赠送消息 然后刷新列表
}

void FriendLayer::onTouchSpriteBuddyAdd(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();	
	int index = tagId - BUDDY_AGREE_TAG - 1;//获取到下标
	//To Do 发送添加好友请求 然后移除该项
	m_userList.eraseObject(m_userList.at(index));
	reflashListView();
}

void FriendLayer::reflashFriendApply()
{
	reflashListView();
	removeNodes();
	m_pListView->setContentSize(Size(1372,520));
	m_pListView->setPosition(Vec2(775,420));
	auto youIdLabel = Label::createWithTTF(StringUtils::format(GetWWString(friend_tuijian_txt,friend_section).c_str(),"1156888").c_str(),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	youIdLabel->setColor(GET_RGBCOLOR(txt_gay));
	youIdLabel->setAnchorPoint(Vec2(0,0.5));
	youIdLabel->setPosition(Vec2(88,773));
	m_friendApply.push_back(youIdLabel);
	bg->addChild(youIdLabel,BASIC_ZORDER_ROOT+2);
	//输入框
	auto seachEditBox = EditBox::create(Size(390,70),Scale9Sprite::createWithSpriteFrameName("friend_serch_bg.png"));
	seachEditBox->setPosition(Vec2(1250,770));
	seachEditBox->setFontName("res/Fonts/FounderSemiBold.ttf");
	seachEditBox->setFontSize(34);
	seachEditBox->setFontColor(GET_RGBCOLOR(0x85412a));
	seachEditBox->setPlaceHolder(GetWWString(friend_sach_holder,friend_section).c_str());
	seachEditBox->setPlaceholderFontColor(GET_RGBCOLOR(0x85412a));
	seachEditBox->setMaxLength(8);
	seachEditBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	m_friendApply.push_back(seachEditBox);
	bg->addChild(seachEditBox,BASIC_ZORDER_ROOT+2);
	//搜索按钮
	auto seachBtn = WWTouchSprite::createWithSpriteFrameName("friend_serch_icon.png",this,callfuncO_selector(FriendLayer::onTouchSpriteDepress));
	seachBtn->setTag(FRIEND_SEACH_BTN);
	seachBtn->setPosition(Vec2(1400,773));
	m_friendApply.push_back(seachBtn);
	bg->addChild(seachBtn,BASIC_ZORDER_ROOT+3);
	//当前申请人数
	auto applyNum = Label::createWithTTF(StringUtils::format(GetWWString(friend_now_apply,friend_section).c_str(),6).c_str(),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	applyNum->setColor(GET_RGBCOLOR(txt_gay));
	applyNum->setAnchorPoint(Vec2(0,0.5f));
	applyNum->setPosition(Vec2(88,101));
	m_friendApply.push_back(applyNum);
	bg->addChild(applyNum,BASIC_ZORDER_ROOT +1);

	auto tuijLabel = Label::createWithTTF(GetWWString(friend_shenqing_txt,friend_section),"res/Fonts/FounderSemiBold.ttf",30,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	tuijLabel->setAnchorPoint(Vec2(0,0.5));
	tuijLabel->setPosition(Vec2(92,704));
	tuijLabel->setColor(GET_RGBCOLOR(txt_white));
	bg->addChild(tuijLabel,BASIC_ZORDER_ROOT+4);

	//上一页按钮
	auto lastBtn = WWTouchSprite::createWithSpriteFrameName("friend_jiantou_gay.png",this,callfuncO_selector(FriendLayer::onTouchSpriteDepress));
	lastBtn->setTag(FRIEND_LAST_APPLY);
	lastBtn->setPosition(Vec2(623,113));
	m_friendApply.push_back(lastBtn);
	bg->addChild(lastBtn,BASIC_ZORDER_ROOT + 1);
	//页码背景
	auto pageBg = Sprite::createWithSpriteFrameName("friend_page_bg.png");
	pageBg->setPosition(Vec2(763,112));
	m_friendApply.push_back(pageBg);
	bg->addChild(pageBg,BASIC_ZORDER_ROOT + 2);
	//页码数字
	auto pageNum = Label::createWithTTF(StringUtils::format(GetWWString(friend_page,friend_section).c_str(),1,3).c_str(),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	pageNum->setColor(GET_RGBCOLOR(txt_with_bg));
	pageNum->setPosition(Vec2(pageBg->getContentSize().width*0.5f,pageBg->getContentSize().height*0.5f));
	pageBg->addChild(pageNum,BASIC_ZORDER_ROOT);
	//下一页按钮
	auto nextBtn = WWTouchSprite::createWithSpriteFrameName("friend_jiantou_gay.png",this,callfuncO_selector(FriendLayer::onTouchSpriteDepress));
	nextBtn->setTag(FRIEND_NEXT_APPLY);
	nextBtn->getpBg()->setFlippedX(true);
	nextBtn->setPosition(Vec2(902,113));
	m_friendApply.push_back(nextBtn);
	bg->addChild(nextBtn,BASIC_ZORDER_ROOT + 1);

	//全部同意
	auto allAgreeBtn = WWTouchSprite::createWithFile(comm_allbule_btn,this,callfuncO_selector(FriendLayer::onTouchSpriteDepress));
	allAgreeBtn->setTag(FRIEND_ALL_AGREE);
	allAgreeBtn->setPosition(Vec2(1119,115));
	m_friendApply.push_back(allAgreeBtn);
	bg->addChild(allAgreeBtn,BASIC_ZORDER_ROOT+1);
	auto allAgreeLabel = Label::createWithTTF(GetWWString(friend_all_agree,friend_section).c_str(),"res/Fonts/FounderSemiBold.ttf",32,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	allAgreeLabel->setColor(GET_RGBCOLOR(txt_green));
	allAgreeLabel->setPosition(Vec2(0,0));
	allAgreeBtn->addChild(allAgreeLabel,BASIC_ZORDER_ROOT+1);

	//全部拒绝
	auto allRefuseBtn = WWTouchSprite::createWithFile(comm_allyellow_btn,this,callfuncO_selector(FriendLayer::onTouchSpriteDepress));
	allRefuseBtn->setTag(FRIEND_ALL_REFUSE);
	allRefuseBtn->setPosition(Vec2(1314,115));
	m_friendApply.push_back(allRefuseBtn);
	bg->addChild(allRefuseBtn,BASIC_ZORDER_ROOT+1);
	auto allRefuseLabel = Label::createWithTTF(GetWWString(friend_all_uagree,friend_section).c_str(),"res/Fonts/FounderSemiBold.ttf",32,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	allRefuseLabel->setColor(GET_RGBCOLOR(txt_agree));
	allRefuseLabel->setPosition(Vec2(0,0));
	allRefuseBtn->addChild(allRefuseLabel,BASIC_ZORDER_ROOT+1);

}

void FriendLayer::reflashFriendList()
{
	reflashListView();
	removeNodes();
	m_pListView->setContentSize(Size(1372,550));
	m_pListView->setPosition(Vec2(775,440));
	//当前好友数
	auto youFriendLabel = Label::createWithTTF(StringUtils::format(GetWWString(friend_youcount_txt,friend_section).c_str(),11).c_str(),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	youFriendLabel->setColor(GET_RGBCOLOR(txt_num));
	youFriendLabel->setAnchorPoint(Vec2(0,0.5));
	youFriendLabel->setPosition(Vec2(88,773));
	m_friendList.push_back(youFriendLabel);
	bg->addChild(youFriendLabel,BASIC_ZORDER_ROOT+2);
	//好友上限
	auto friendMost = Label::createWithTTF(StringUtils::format(GetWWString(friend_most_txt,friend_section).c_str(),60).c_str(),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	friendMost->setColor(GET_RGBCOLOR(txt_gay));
	friendMost->setAnchorPoint(Vec2(0,0.5));
	friendMost->setPosition(Vec2(1177,772));
	m_friendList.push_back(friendMost);
	bg->addChild(friendMost,BASIC_ZORDER_ROOT+2);

	//添加按钮
	auto addFriendBtn = WWTouchSprite::createWithFile(comm_unselect_c,this,callfuncO_selector(FriendLayer::onTouchSpriteDepress));
	addFriendBtn->setPosition(Vec2(221,101));
	addFriendBtn->setTag(FRIEND_ADD_BTN);
	m_friendList.push_back(addFriendBtn);
	bg->addChild(addFriendBtn,BASIC_ZORDER_FRAME);

	auto add_label = Label::createWithTTF(GetWWString(friend_add_txt,friend_section),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	add_label->setPosition(Vec2(0,0));
	add_label->setColor(GET_RGBCOLOR(txt_color));
	addFriendBtn->addChild(add_label,BASIC_ZORDER_ROOT);

	//上一页按钮
	auto lastBtn = WWTouchSprite::createWithSpriteFrameName("friend_jiantou_gay.png",this,callfuncO_selector(FriendLayer::onTouchSpriteDepress));
	lastBtn->setTag(FRIEND_LAST_APPLY);
	lastBtn->setPosition(Vec2(623,111));
	m_friendList.push_back(lastBtn);
	bg->addChild(lastBtn,BASIC_ZORDER_ROOT + 1);
	//页码背景
	auto pageBg = Sprite::createWithSpriteFrameName("friend_page_bg.png");
	pageBg->setPosition(Vec2(763,110));
	m_friendList.push_back(pageBg);
	bg->addChild(pageBg,BASIC_ZORDER_ROOT + 2);
	//页码数字
	auto pageNum = Label::createWithTTF(StringUtils::format(GetWWString(friend_page,friend_section).c_str(),1,3).c_str(),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	pageNum->setColor(GET_RGBCOLOR(txt_with_bg));
	pageNum->setPosition(Vec2(pageBg->getContentSize().width*0.5f,pageBg->getContentSize().height*0.5f));
	pageBg->addChild(pageNum,BASIC_ZORDER_ROOT);
	//下一页按钮
	auto nextBtn = WWTouchSprite::createWithSpriteFrameName("friend_jiantou_gay.png",this,callfuncO_selector(FriendLayer::onTouchSpriteDepress));
	nextBtn->setTag(FRIEND_NEXT_APPLY);
	nextBtn->getpBg()->setFlippedX(true);
	nextBtn->setPosition(Vec2(902,111));
	m_friendList.push_back(nextBtn);
	bg->addChild(nextBtn,BASIC_ZORDER_ROOT + 1);

	//全部领取
	auto allAgreeBtn = WWTouchSprite::createWithFile(comm_allbule_btn,this,callfuncO_selector(FriendLayer::onTouchSpriteDepress));
	allAgreeBtn->setTag(FRIEND_ALL_GET);
	allAgreeBtn->setPosition(Vec2(1119,112));
	m_friendList.push_back(allAgreeBtn);
	bg->addChild(allAgreeBtn,BASIC_ZORDER_ROOT+1);
	auto allAgreeLabel = Label::createWithTTF(GetWWString(friend_all_get,friend_section).c_str(),"res/Fonts/FounderSemiBold.ttf",32,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	allAgreeLabel->setColor(GET_RGBCOLOR(txt_green));
	allAgreeLabel->setPosition(Vec2(0,0));
	allAgreeBtn->addChild(allAgreeLabel,BASIC_ZORDER_ROOT+1);

	//全部赠送
	auto allRefuseBtn = WWTouchSprite::createWithFile(comm_allyellow_btn,this,callfuncO_selector(FriendLayer::onTouchSpriteDepress));
	allRefuseBtn->setTag(FRIEND_ALL_RESAVE);
	allRefuseBtn->setPosition(Vec2(1314,112));
	m_friendList.push_back(allRefuseBtn);
	bg->addChild(allRefuseBtn,BASIC_ZORDER_ROOT+1);
	auto allRefuseLabel = Label::createWithTTF(GetWWString(friend_all_give,friend_section).c_str(),"res/Fonts/FounderSemiBold.ttf",32,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	allRefuseLabel->setColor(GET_RGBCOLOR(txt_agree));
	allRefuseLabel->setPosition(Vec2(0,0));
	allRefuseBtn->addChild(allRefuseLabel,BASIC_ZORDER_ROOT+1);
}

void FriendLayer::reflashFriendSeach()
{
	reflashListView();
	removeNodes();
	m_pListView->setContentSize(Size(1372,520));
	m_pListView->setPosition(Vec2(775,420));
	//当前好友数
	auto youFriendLabel = Label::createWithTTF(StringUtils::format(GetWWString(friend_youcount_txt,friend_section).c_str(),11).c_str(),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	youFriendLabel->setColor(GET_RGBCOLOR(txt_num));
	youFriendLabel->setAnchorPoint(Vec2(0,0.5));
	youFriendLabel->setPosition(Vec2(88,773));
	m_friendSeach.push_back(youFriendLabel);
	bg->addChild(youFriendLabel,BASIC_ZORDER_ROOT+2);
	//好友上限
	auto friendMost = Label::createWithTTF(StringUtils::format(GetWWString(friend_most_txt,friend_section).c_str(),60).c_str(),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	friendMost->setColor(GET_RGBCOLOR(txt_gay));
	friendMost->setAnchorPoint(Vec2(0,0.5));
	friendMost->setPosition(Vec2(1177,772));
	m_friendSeach.push_back(friendMost);
	bg->addChild(friendMost,BASIC_ZORDER_ROOT+2);

	auto tuijLabel = Label::createWithTTF(GetWWString(friend_tuijian_txt,friend_section),"res/Fonts/FounderSemiBold.ttf",30,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	tuijLabel->setAnchorPoint(Vec2(0,0.5));
	tuijLabel->setPosition(Vec2(92,704));
	tuijLabel->setColor(GET_RGBCOLOR(txt_white));
	m_friendSeach.push_back(tuijLabel);
	bg->addChild(tuijLabel,BASIC_ZORDER_ROOT+4);

	//添加按钮
	auto addFriendBtn = WWTouchSprite::createWithFile(comm_unselect_c,this,callfuncO_selector(FriendLayer::onTouchSpriteDepress));
	addFriendBtn->setPosition(Vec2(221,101));
	addFriendBtn->setTag(FRIEND_ADD_BTN);
	m_friendSeach.push_back(addFriendBtn);
	bg->addChild(addFriendBtn,BASIC_ZORDER_FRAME);

	auto add_label = Label::createWithTTF(GetWWString(friend_add_txt,friend_section),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	add_label->setPosition(Vec2(0,0));
	add_label->setColor(GET_RGBCOLOR(txt_color));
	addFriendBtn->addChild(add_label,BASIC_ZORDER_ROOT);

	auto addBtnDesc = Label::createWithTTF(GetWWString(friend_add_desc,friend_section),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	addBtnDesc->setAnchorPoint(Vec2(0,0.5));
	addBtnDesc->setPosition(Vec2(416,106));
	addBtnDesc->setColor(GET_RGBCOLOR(txt_gay));
	m_friendSeach.push_back(addBtnDesc);
	bg->addChild(addBtnDesc,BASIC_ZORDER_ROOT+4);
}

void FriendLayer::onEnter()
{
	LayerColor::onEnter();
	//禁止触摸穿透
	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(FriendLayer::onTouchBegan, this);
	m_pListner->onTouchEnded = CC_CALLBACK_2(FriendLayer::onTouchEnded,this);
	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);
	
}

void FriendLayer::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	LayerColor::onExit();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(friend_plist);
}

//按钮相应事件
void FriendLayer::onTouchSpriteDepress(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	switch (tagId)
	{
	case FRIEND_ADD_BTN:
		reflashFriendList();
		break;
	case FRIEND_ALL_REFUSE://全部拒绝
		log("FRIEND_ALL_REFUSE");
		break;
	case FRIEND_ALL_AGREE://全部同意
		log("FRIEND_ALL_AGREE");
		break;
	case FRIEND_ALL_GET://一键领取
		log("FRIEND_ALL_GET");
		break;
	case FRIEND_ALL_RESAVE://一键赠送
		log("FRIEND_ALL_RESAVE");
		break;
	default:
		break;
	}
}

void FriendLayer::removeNodes()
{
	for (const auto& node : this->m_friendApply)
	{
		bg->removeChild(node,true);
	}
	m_friendApply.clear();
	for (const auto& node : this->m_friendList)
	{
		bg->removeChild(node,true);
	}
	m_friendList.clear();
	for (const auto& node : this->m_friendSeach)
	{
		bg->removeChild(node,true);
	}
	m_friendSeach.clear();
}


bool FriendLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	WWFrameBase::onTouchBegan(pTouch, pEvent);
	return true;
}

void FriendLayer::onTouchMoved(Touch* pTouch, Event* pEvent)
{

}

void FriendLayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	WWFrameBase::onTouchEnded(pTouch, pEvent);
}

void FriendLayer::onTouchCancelled(Touch* pTouch, Event* pEvent)
{

}


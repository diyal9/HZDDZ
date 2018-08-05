#include "RoomListLayer.h"

#include "Control//WWRequest/MsgLogonRequest.h"
#include "WWMacros.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"

#include "View/GameScene/GameBasicScene.h"
#include "View/GameScene/GameLogicManager.h"
#include "Control/WWDataManager/WWDataManager.h"
#include "BaseCore/WWBTools/StringTool.h"

#include "Control/WWDataManager/GameGlobalDataManager.h"
#include "Control/WWRequest/MsgGameCommonRequest.h"
#include "Control/WWRequest/MsgLobbyRequest.h"
#include "Model/WWDataModel/WWGD/GD.h"
#include "RoomListLayerRes.h"

#include "Control/WWCSound/WWSoundManager.h"
#include "BaseCore/Common/MsgConstDefine.h"

#include "View/Common/CommLayer/CommDialog.h"

#include "Control/WWRequest/MsgMatchRequest.h"

USING_NS_CC;
const char* room_list_plist = "res/Images/Lobby/roomList/lb_roomlost.plist";

#define	ROOMLIST_BTN_MATCH	300 	//比赛场房间TAG

RoomListLayer::RoomListLayer(void) : 
	m_pListView(nullptr),
	m_pPlayerTypeID(0),
	rooms(NULL)
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

}

RoomListLayer::~RoomListLayer()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);
	//CC_SAFE_RELEASE_NULL(m_pListView);
	//rooms->removeAllObjects();
}


void RoomListLayer::onEnter()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	LayerColor::onEnter();
	//禁止触摸穿透
	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(RoomListLayer::onTouchBegan, this);
	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(room_list_plist);
}

void RoomListLayer::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	LayerColor::onExit();
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(room_list_plist);
}

RoomListLayer* RoomListLayer::create(int playGameId)
{
	RoomListLayer* pItem = new RoomListLayer();
	if(pItem && pItem->init(playGameId))
	{
		pItem->autorelease();
		return pItem;
	}
	else
	{
		CC_SAFE_DELETE(pItem);
		return NULL;
	}
}


bool RoomListLayer::init(int playGameId)
{
	if (!LayerColor::initWithColor(Color4B(0,0,0,128))) return false;

	log("%s %d\r\n", __FUNCTION__, __LINE__);

	//MsgLobbyRequest::requestRoomDatas( 1001, playGameId);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(room_list_plist);

	log("%s %d\r\n %s", __FUNCTION__, __LINE__, "MsgLobbyRequest::requestRoomDatas( 1001, playGameId);");

	m_pPlayerTypeID = playGameId;
	createView(playGameId);

	log("%s %d\r\n %s", __FUNCTION__, __LINE__, "createView");

	return true;
}

void RoomListLayer::createView(int playGameId)
{

	auto s_lFrameBg = Sprite::create(frameBg1);
	s_lFrameBg->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5, CUR_SCREEN_SIZE.height * 0.5));
	this->addChild(s_lFrameBg);

	Sprite* s_lFrameSubBg;
	if ( playGameId != RoomType::MATCH_ROOM )
	{
		s_lFrameSubBg= Sprite::create(frameBg2);
		s_lFrameSubBg->setPosition(Vec2(s_lFrameBg->getContentSize().width * 0.5, s_lFrameBg->getContentSize().height * 0.48f));
		s_lFrameBg->addChild(s_lFrameSubBg);
	}

	//房顶
	auto s_lRoomTop = Sprite::createWithSpriteFrameName("roomlist_other1.png");
	s_lRoomTop->setPosition(Vec2(s_lFrameBg->getContentSize().width * 0.5, s_lFrameBg->getContentSize().height - s_lRoomTop->getContentSize().height * 0.3));
	s_lFrameBg->addChild(s_lRoomTop,1000);

	if (playGameId == RoomType::HAPPY_ROOM)//欢乐斗地主
	{
		auto s_lRoomTitle = Sprite::createWithSpriteFrameName("roomlist_title.png");
		s_lRoomTitle->setPosition(Vec2(s_lRoomTop->getContentSize().width * 0.5, s_lRoomTop->getContentSize().height - s_lRoomTitle->getContentSize().height * 0.3));
		s_lRoomTop->addChild(s_lRoomTitle,1000);
	}
	else if (playGameId == RoomType::RASCAL_ROOM)//癞子斗地主
	{
		auto s_lRoomTitle = Sprite::createWithSpriteFrameName("roomlist_title1.png");
		s_lRoomTitle->setPosition(Vec2(s_lRoomTop->getContentSize().width * 0.5, s_lRoomTop->getContentSize().height - s_lRoomTitle->getContentSize().height * 0.3));
		s_lRoomTop->addChild(s_lRoomTitle,1000);
	}
	else if (playGameId == RoomType::MATCH_ROOM)//比赛场
	{
		auto s_lRoomTitle = Sprite::create("res/Images/Lobby/roomList/roomList_matchTitlle.png");
		s_lRoomTitle->setPosition(Vec2(s_lRoomTop->getContentSize().width * 0.5, s_lRoomTop->getContentSize().height - s_lRoomTitle->getContentSize().height * 0.3));
		s_lRoomTop->addChild(s_lRoomTitle,1000);
	}

	//关闭按钮
	auto b_lClose = WWTouchSprite::createWithFile(closeBtn, this, callfuncO_selector(RoomListLayer::TouchSpriteBtnEvent));
	b_lClose->setPosition(Vec2(s_lRoomTop->getContentSize().width - b_lClose->getContentSize().width * 0.6, s_lRoomTop->getContentSize().height * 0.62));
	s_lRoomTop->addChild(b_lClose, 100,ROOMLIST_BTN_CLOSE);
	
	if( playGameId == RoomType::MATCH_ROOM ){//比赛场

		//定人赛
		auto b_lPeopleMatchBtn = WWTouchSprite::createWithFile("res/Images/Common/btn_gray.png", this, callfuncO_selector(RoomListLayer::TouchSpriteBtnEvent));
		b_lPeopleMatchBtn->setPosition(Vec2(s_lFrameBg->getContentSize().width - b_lPeopleMatchBtn->getContentSize().width * 0.7f, b_lPeopleMatchBtn->getContentSize().height*0.7f));
		s_lFrameBg->addChild(b_lPeopleMatchBtn,1, ROOMLIST_BTN_PEOPLE);
		//文字
		auto peopleLabel = Label::createWithTTF(GetWWString(room_peopleMatch,roomList_Section),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		peopleLabel->setColor(GET_RGBCOLOR(room_match_txt_color));
		peopleLabel->setPosition(Vec2(0,0));
		b_lPeopleMatchBtn->addChild(peopleLabel,BASIC_ZORDER_ROOT);

		//定时赛
		auto b_lTimeMatchBtn = WWTouchSprite::createWithFile("res/Images/Common/btn_gold.png", this, callfuncO_selector(RoomListLayer::TouchSpriteBtnEvent));
		b_lTimeMatchBtn->setPosition(Vec2(s_lFrameBg->getContentSize().width - b_lPeopleMatchBtn->getContentSize().width - b_lTimeMatchBtn->getContentSize().width*0.8f, b_lTimeMatchBtn->getContentSize().height*0.7f));
		s_lFrameBg->addChild(b_lTimeMatchBtn,1, ROOMLIST_BTN_TIME);
		//文字
		auto timeLabel = Label::createWithTTF(GetWWString(room_timeMatch,roomList_Section),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		timeLabel->setColor(GET_RGBCOLOR(room_match_txt_color));
		timeLabel->setPosition(Vec2(0,0));
		b_lTimeMatchBtn->addChild(timeLabel,BASIC_ZORDER_ROOT);

	}else{
		//宝箱
		auto s_lBox = WWTouchSprite::createWithSpriteFrameName("roomlist_box.png", this, callfuncO_selector(RoomListLayer::TouchSpriteBtnEvent));
		s_lBox->setPosition(Vec2(30, s_lBox->getContentSize().height * 0.5));
		s_lFrameBg->addChild(s_lBox, 1 ,ROOMLIST_BTN_BOX);

		//快速开始
		auto b_lQuickStart = WWTouchSprite::createWithSpriteFrameName("roomlist_quickstart.png", this, callfuncO_selector(RoomListLayer::TouchSpriteBtnEvent));
		b_lQuickStart->setPosition(Vec2(s_lFrameSubBg->getContentSize().width - b_lQuickStart->getContentSize().width * 0.6, b_lQuickStart->getContentSize().height));
		s_lFrameSubBg->addChild(b_lQuickStart,1, ROOMLIST_BTN_QUICKSTART);
	}

	//创建ListView背景
	Sprite *s_lListViewBg; 
	if( playGameId == RoomType::MATCH_ROOM ){//比赛场
		s_lListViewBg = Sprite::create("res/Images/Lobby/roomList/roomList_matchListBg.png");
		s_lFrameBg->addChild(s_lListViewBg);
		s_lListViewBg->setPosition(Vec2(s_lFrameBg->getContentSize().width * 0.5, s_lFrameBg->getContentSize().height * 0.48f));
	}else{
		s_lListViewBg = Sprite::createWithSpriteFrameName("roomlist_bottom.png");
		s_lFrameSubBg->addChild(s_lListViewBg);
		s_lListViewBg->setPosition(Vec2(s_lFrameSubBg->getContentSize().width * 0.5, s_lFrameSubBg->getContentSize().height * 0.55f));
	}

	// 创建ListView
	if (!m_pListView)
	{
		Size lvBgSize = s_lListViewBg->getContentSize(); //ListView背景图大小

		m_pListView = ListView::create();
		m_pListView->setTouchEnabled(true);
		m_pListView->setBounceEnabled(true); //是否开启回弹
		m_pListView->setBackGroundImageScale9Enabled(true); //是否按.9方式拉升
		//m_pListView->setItemsMargin(25.0f);
		if( playGameId == RoomType::MATCH_ROOM ){
			// 设置方向
			m_pListView->setDirection(ui::ScrollView::Direction::VERTICAL);
			m_pListView->setContentSize(Size(lvBgSize.width, lvBgSize.height - 20)); 
			m_pListView->setPosition(Vec2(0, 20));
		}else{
			// 设置方向
			m_pListView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
			m_pListView->setContentSize(Size(lvBgSize.width, lvBgSize.height)); 
			m_pListView->setPosition(Vec2(0, 0));
			m_pListView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(RoomListLayer::listItemClick, this));
		}
		s_lListViewBg->addChild(m_pListView);
	}

	//默认定时赛
	curMatchListTag = ROOMLIST_BTN_TIME;

	reflashListView();
}

//创建ListView item
Layout* RoomListLayer::createItem(GameRoom* roomData, int roodId)
{
	if (!roomData) return nullptr;

	//背景图
	auto default_button = Sprite::createWithSpriteFrameName("roomlist_roombg.png");

	//头像
	int mutil = roomData->getMultiple();
	if (mutil == 1)
	{
		auto s_role1 = Sprite::createWithSpriteFrameName("roomlist_role1.png");
		s_role1->setPosition(Vec2(default_button->getContentSize().width * 0.5, default_button->getContentSize().height - s_role1->getContentSize().height * 0.28));	
		default_button->addChild(s_role1);
	}
	else if (mutil == 2)
	{
		auto s_role2 = Sprite::createWithSpriteFrameName("roomlist_role2.png");
		s_role2->setPosition(Vec2(default_button->getContentSize().width * 0.5, default_button->getContentSize().height - s_role2->getContentSize().height * 0.28));	
		default_button->addChild(s_role2);
	}
	else if (mutil == 4)
	{
		auto s_role3 = Sprite::createWithSpriteFrameName("roomlist_role3.png");
		s_role3->setPosition(Vec2(default_button->getContentSize().width * 0.5, default_button->getContentSize().height - s_role3->getContentSize().height * 0.28));	
		default_button->addChild(s_role3);
	}
	else
	{
		auto s_role4 = Sprite::createWithSpriteFrameName("roomlist_role4.png");
		s_role4->setPosition(Vec2(default_button->getContentSize().width * 0.5, default_button->getContentSize().height - s_role4->getContentSize().height * 0.28));	
		default_button->addChild(s_role4);
	}
	

	//房间名称
	auto label = Text::create();
	label->setContentSize(Size(333, 34));
	label->setFontSize(44);
	label->setFontName("res/Fonts/FounderSemiBold.ttf");
	label->setColor(GET_RGBCOLOR(room_title_txt_color));
	label->setString(roomData->getName());
	default_button->addChild(label);
	label->setPosition(Vec2(default_button->getContentSize().width * 0.5, default_button->getContentSize().height * 0.48));

	//底分
	std::string needScoreTxt = StringUtils::format(GetWWString(room_difen,roomList_Section).c_str(),roomData->getFortuneBase());
	auto labelNeedScore = Text::create();
	labelNeedScore->setContentSize(Size(333, 34));
	labelNeedScore->setColor(GET_RGBCOLOR(room_difen_txt_color));
	labelNeedScore->setFontSize(33);
	labelNeedScore->setFontName("res/Fonts/FounderSemiBold.ttf");
	labelNeedScore->setString(needScoreTxt);
	default_button->addChild(labelNeedScore);
	labelNeedScore->setPosition(Vec2(default_button->getContentSize().width * 0.5, default_button->getContentSize().height * 0.33));

	//准入
	std::string conditiScoreTxt = StringUtils::format(GetWWString(room_zhunru,roomList_Section).c_str(),roomData->getLevelMinLimit());
	auto labelCondition = Text::create();
	labelCondition->setContentSize(Size(333, 34));
	labelCondition->setColor(GET_RGBCOLOR(room_zhunru_txt_color));
	labelCondition->setFontSize(33);
	labelCondition->setFontName("res/Fonts/FounderSemiBold.ttf");
	labelCondition->setString(conditiScoreTxt);
	default_button->addChild(labelCondition);
	labelCondition->setPosition(Vec2(default_button->getContentSize().width * 0.5, default_button->getContentSize().height * 0.23));

	//奖励
	auto labelCanGet= Text::create();
	labelCanGet->setContentSize(Size(333, 34));
	labelCanGet->setColor(GET_RGBCOLOR(room_jingl_txt_color));
	labelCanGet->setFontSize(33);
	labelCanGet->setFontName("res/Fonts/FounderSemiBold.ttf");
	labelCanGet->setString(GetWWString(room_jiangli,roomList_Section));
	default_button->addChild(labelCanGet);
	labelCanGet->setPosition(Vec2(default_button->getContentSize().width * 0.5, default_button->getContentSize().height * 0.13));

	//布局
	Layout* default_item = Layout::create();
	default_item->setTouchEnabled(true);
	default_item->setContentSize(Size(default_button->getContentSize().width * 1.08, default_button->getContentSize().height));
	default_button->setPosition(Vec2(default_item->getContentSize().width * 0.5, default_item->getContentSize().height * 0.45));
	default_item->addChild(default_button, BASIC_ZORDER_FRAME, roodId); //设置房间ID为Tag

	return default_item;
}

Layout* RoomListLayer::createMatchItem(MatchRoom* roomData1, int roomId1, MatchRoom* roomData2, int roomId2)
{
	//背景图
	auto* default_button = Sprite::create("res/Images/Lobby/roomList/roomList_matchRoomBg.png");
	
	auto s_image = Sprite::create("res/Images/Lobby/roomList/roomList_matchRoomImg.png");
	s_image->setPosition(Vec2(s_image->getContentSize().width*0.5 +10, default_button->getContentSize().height - s_image->getContentSize().height*0.5 -10));	
	default_button->addChild(s_image);

	//if(roomId == 0){
		auto s_tag = Sprite::create("res/Images/Lobby/roomList/roomList_openTag.png");
		s_tag->setPosition(Vec2(s_tag->getContentSize().width*0.5 + 5, default_button->getContentSize().height - s_tag->getContentSize().height*0.5 - 5));	
		default_button->addChild(s_tag);
	/*}else if(roomId == 1){
		auto s_tag = Sprite::create("res/Images/Lobby/roomList/roomList_newTag.png");
		s_tag->setPosition(Vec2(s_tag->getContentSize().width*0.5 + 5, default_button->getContentSize().height - s_tag->getContentSize().height*0.5 - 5));	
		default_button->addChild(s_tag);
	}else if(roomId == 3){
		auto s_tag = Sprite::create("res/Images/Lobby/roomList/roomList_closeTag.png");
		s_tag->setPosition(Vec2(s_tag->getContentSize().width*0.5 + 5, default_button->getContentSize().height - s_tag->getContentSize().height*0.5 - 5));	
		default_button->addChild(s_tag);
	}*/
	
	//房间名称
	auto label = Text::create();
	label->setContentSize(Size(333, 34));
	label->setFontSize(44);
	label->setFontName("res/Fonts/FounderSemiBold.ttf");
	label->setColor(GET_RGBCOLOR(room_match_name_color));
	label->setString(roomData1->getMatchName());
	default_button->addChild(label);
	label->setPosition(Vec2(default_button->getContentSize().width * 0.5f, default_button->getContentSize().height * 0.5f - label->getContentSize().height * 0.7f));
	
	//房间tips
	std::string tipsTxt = StringUtils::format(GetWWString(room_peopleTipFormet,roomList_Section).c_str(), 123456, 24);
	auto tipsLabel = Text::create();
	tipsLabel->setContentSize(Size(333, 34));
	tipsLabel->setFontSize(30);
	tipsLabel->setFontName("res/Fonts/FounderSemiBold.ttf");
	tipsLabel->setColor(GET_RGBCOLOR(room_match_tips_color));
	tipsLabel->setString(tipsTxt);
	default_button->addChild(tipsLabel);
	tipsLabel->setPosition(Vec2(tipsLabel->getContentSize().width * 0.5 + 30, tipsLabel->getContentSize().height));

	//报名按纽
	auto b_lRequestBtn = WWTouchSprite::createWithFile("res/Images/Common/matchBtn.png", this, callfuncO_selector(RoomListLayer::TouchSpriteBtnEvent));
	b_lRequestBtn->setPosition(Vec2(default_button->getContentSize().width - b_lRequestBtn->getContentSize().width * 0.6f, b_lRequestBtn->getContentSize().height * 0.7f));
	default_button->addChild(b_lRequestBtn, BASIC_ZORDER_ROOT, ROOMLIST_BTN_MATCH + roomId1);
	//文字
	std::string requestBtnTx = GetWWString(room_matchRequest,roomList_Section);
	if(roomData1->getSignupFlag() == 1){
		requestBtnTx = GetWWString(room_matchRequested,roomList_Section);
	}
	auto requestLabel = Label::createWithTTF(requestBtnTx,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	requestLabel->setColor(GET_RGBCOLOR(room_match_txt_color));
	requestLabel->setPosition(Vec2(0,0));
	b_lRequestBtn->addChild(requestLabel,BASIC_ZORDER_ROOT);
	if(roomData1->getSignupFlag() == 1){
		b_lRequestBtn->setEnabled(false);
	}

	//布局
	Layout* default_item = Layout::create();
	default_item->setContentSize(Size(default_button->getContentSize().width*2, default_button->getContentSize().height * 1.07f));
	default_button->setPosition(Vec2(default_button->getContentSize().width*0.56f, default_button->getContentSize().height * 0.41f));
	default_item->addChild(default_button, BASIC_ZORDER_FRAME, roomId1); //设置房间ID为Tag

	if(roomData2){
		//背景图
		auto* default_button2 = Sprite::create("res/Images/Lobby/roomList/roomList_matchRoomBg.png");
	
		auto s_image = Sprite::create("res/Images/Lobby/roomList/roomList_matchRoomImg.png");
		s_image->setPosition(Vec2(s_image->getContentSize().width*0.5 +10, default_button2->getContentSize().height - s_image->getContentSize().height*0.5 -10));	
		default_button2->addChild(s_image);

		//if(roomId == 0){
			auto s_tag = Sprite::create("res/Images/Lobby/roomList/roomList_openTag.png");
			s_tag->setPosition(Vec2(s_tag->getContentSize().width*0.5 + 5, default_button2->getContentSize().height - s_tag->getContentSize().height*0.5 - 5));	
			default_button2->addChild(s_tag);
		/*}else if(roomId == 1){
			auto s_tag = Sprite::create("res/Images/Lobby/roomList/roomList_newTag.png");
			s_tag->setPosition(Vec2(s_tag->getContentSize().width*0.5 + 5, default_button2->getContentSize().height - s_tag->getContentSize().height*0.5 - 5));	
			default_button2->addChild(s_tag);
		}else if(roomId == 3){
			auto s_tag = Sprite::create("res/Images/Lobby/roomList/roomList_closeTag.png");
			s_tag->setPosition(Vec2(s_tag->getContentSize().width*0.5 + 5, default_button2->getContentSize().height - s_tag->getContentSize().height*0.5 - 5));	
			default_button2->addChild(s_tag);
		}*/
	
		//房间名称
		auto label = Text::create();
		label->setContentSize(Size(333, 34));
		label->setFontSize(44);
		label->setFontName("res/Fonts/FounderSemiBold.ttf");
		label->setColor(GET_RGBCOLOR(room_match_name_color));
		label->setString(roomData2->getMatchName());
		default_button2->addChild(label);
		label->setPosition(Vec2(default_button2->getContentSize().width * 0.5f, default_button2->getContentSize().height * 0.5f - label->getContentSize().height * 0.7f));
	
		//房间tips
		std::string tipsTxt = StringUtils::format(GetWWString(room_peopleTipFormet,roomList_Section).c_str(), 123456, 24);
		auto tipsLabel = Text::create();
		tipsLabel->setContentSize(Size(333, 34));
		tipsLabel->setFontSize(30);
		tipsLabel->setFontName("res/Fonts/FounderSemiBold.ttf");
		tipsLabel->setColor(GET_RGBCOLOR(room_match_tips_color));
		tipsLabel->setString(tipsTxt);
		default_button2->addChild(tipsLabel);
		tipsLabel->setPosition(Vec2(tipsLabel->getContentSize().width * 0.5 + 30, tipsLabel->getContentSize().height));

		//报名按纽
		auto b_lRequestBtn = WWTouchSprite::createWithFile("res/Images/Common/matchBtn.png", this, callfuncO_selector(RoomListLayer::TouchSpriteBtnEvent));
		b_lRequestBtn->setPosition(Vec2(default_button2->getContentSize().width - b_lRequestBtn->getContentSize().width * 0.6f, b_lRequestBtn->getContentSize().height * 0.7f));
		default_button2->addChild(b_lRequestBtn, BASIC_ZORDER_ROOT, ROOMLIST_BTN_MATCH + roomId1);
		//文字
		std::string requestBtnTx = GetWWString(room_matchRequest,roomList_Section);
		if(roomData2->getSignupFlag() == 1){
			requestBtnTx = GetWWString(room_matchRequested,roomList_Section);
		}
		auto requestLabel = Label::createWithTTF(requestBtnTx,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		requestLabel->setColor(GET_RGBCOLOR(room_match_txt_color));
		requestLabel->setPosition(Vec2(0,0));
		b_lRequestBtn->addChild(requestLabel,BASIC_ZORDER_ROOT);
		if(roomData2->getSignupFlag() == 1){
			b_lRequestBtn->setEnabled(false);
		}

		default_button2->setPosition(Vec2(default_button2->getContentSize().width + default_button2->getContentSize().width*0.56f + 20, default_button2->getContentSize().height * 0.41f));
		default_item->addChild(default_button2, BASIC_ZORDER_FRAME, roomId2); //设置房间ID为Tag
	}

	return default_item;
}

void RoomListLayer::reflashListView()
{
	if( m_pPlayerTypeID == RoomType::MATCH_ROOM){
		if(WWDataManager::getInstance()->getGameZone() == nullptr || WWDataManager::getInstance()->getGameZone()->getMatchRooms( curMatchListTag - ROOMLIST_BTN_PEOPLE + 1 ) == nullptr){
			return ;
		}

		m_pListView->removeAllItems(); //清空Item

		rooms = WWDataManager::getInstance()->getGameZone()->getMatchRooms( curMatchListTag - ROOMLIST_BTN_PEOPLE + 1 );  //根据选择的玩法ID获得房间数据

		// 根据数据列表创建ListVIew的Cell
		for (int i = 0; i < (int)(rooms->count()/2); i++)
		{
			MatchRoom* room = (MatchRoom*)(rooms->getObjectAtIndex(i*2));
			MatchRoom* room2 = (MatchRoom*)(rooms->getObjectAtIndex(i*2+1));
			if (room){
				m_pListView->pushBackCustomItem(createMatchItem(room, i*2, room2, i*2+1));
			}
		}

		if( rooms->count()%2 )
		{
			MatchRoom* room = (MatchRoom*)(rooms->getObjectAtIndex( rooms->count()-1 ));
			if(room){
				m_pListView->pushBackCustomItem(createMatchItem(room, rooms->count()-1, NULL, -1));
			}
		}
	}
	else
	{
		if(WWDataManager::getInstance()->getGameZone() == nullptr 
			|| WWDataManager::getInstance()->getGameZone()->getRooms(m_pPlayerTypeID) == nullptr){
			return ;
		}
		
		m_pListView->removeAllItems(); //清空Item
		
		rooms = WWDataManager::getInstance()->getGameZone()->getRooms(m_pPlayerTypeID); //根据选择的玩法ID获得房间数据
		// 根据数据列表创建ListVIew的Cell
		for (int i = 0; i < rooms->count(); ++i)
		{
			GameRoom* room = (GameRoom*)(rooms->getObjectAtIndex(i));
			if (room){
				m_pListView->pushBackCustomItem(createItem(room, i));
			}
		}
	}

}

bool RoomListLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{

	return true;
}

void RoomListLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void RoomListLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{

}

void RoomListLayer::listItemClick(Ref* pSender, ListView::EventType type)
{

	switch (type)
	{
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
		break;
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
		{

			if (FileUtils::getInstance()->isFileExist("res/Audio/mp3/Audio_Button_Click.mp3"))
			{
				WWSoundManager::getInstance()->playEffect("res/Audio/mp3/Audio_Button_Click.mp3");
			}

			//设置选中房间数据到临时数据单例 (这里不用setUserData()防止野指针)
			ListView* listView = static_cast<ListView*>(pSender);
			log("select item index = %d", listView->getCurSelectedIndex());
			int index = (int)listView->getCurSelectedIndex();

			cocos2d::__Array* rooms = WWDataManager::getInstance()->getGameZone()->getRooms(m_pPlayerTypeID);
			GameRoom* room = (GameRoom*)(rooms->getObjectAtIndex(index));
			if(room->getLockFlag())
			{
				//CommDialog* dialog = CommDialog::createOneBtn("lock room",NULL);
				CommDialog* dialog = CommDialog::creatOneBtnOneMessage("yes", "lock room");
				this->addChild(dialog);
				return;
			}
			log("设置房间数据");
			WWDataManager::getInstance()->setGameRoom(room);
			log("切换到牌局场景");
			GameGlobalDataManager::getInstance()->setStartGameType(StartGameType::SEND_NOMAL);
			//记录当前玩法
			GameLogicManager::getInstance()->setCurRoomType(m_pPlayerTypeID);
			//记录当前底注
			GameLogicManager::getInstance()->setCurRoomBase(room->getFortuneBase());

			GameBasicScene* pScene = GameBasicScene::create();
			//Director::getInstance()->replaceScene(TransitionFade::create(1.5, pScene));
			Director::getInstance()->replaceScene(pScene);

		}

		break;
	default:
		break;
	}
}

void RoomListLayer::ButtonDepress(Ref * theBtn)
{
	MenuItem *pItem = (MenuItem*)theBtn; 
	Menu* pMenu = (Menu*)pItem->getUserData(); 
	int tagId = pMenu->getTag();
	if(tagId == ROOMLIST_BTN_CLOSE)
	{
		if (this->getParent()){
			removeFromParent();
		}
	}
}

void RoomListLayer::TouchSpriteBtnEvent(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	switch (tagId)
	{
	case  ROOMLIST_BTN_CLOSE: //关闭
		{
			if (this->getParent()){
				removeFromParent();
			}
		}
		break;
	case  ROOMLIST_BTN_QUICKSTART: //快速开始
		{
			log("Quick start click");
			//just for test. 取列表第一个房间
			int index = 0;
			cocos2d::__Array* rooms = WWDataManager::getInstance()->getGameZone()->getRooms(m_pPlayerTypeID);
			GameRoom* room = (GameRoom*)(rooms->getObjectAtIndex(index));
			log("设置房间数据");
			WWDataManager::getInstance()->setGameRoom(room);
			log("切换到牌局场景");
			GameGlobalDataManager::getInstance()->setStartGameType(StartGameType::SEND_NOMAL);
			//记录当前玩法
			GameLogicManager::getInstance()->setCurRoomType(m_pPlayerTypeID);
			//记录当前底注
			GameLogicManager::getInstance()->setCurRoomBase(room->getFortuneBase());
			
			GameBasicScene* pScene = GameBasicScene::create();
			//Director::getInstance()->replaceScene(TransitionFade::create(1.5, pScene));
			Director::getInstance()->replaceScene(pScene);

		}

		break;
	case  ROOMLIST_BTN_BOX: //宝箱
		{
			log("Box Btn click");
		}
		break;
		
	case  ROOMLIST_BTN_TIME:		//定时赛
		{
			log("Time Btn click");
			curMatchListTag = ROOMLIST_BTN_TIME;
			reflashListView();
		}
		break;

	case  ROOMLIST_BTN_PEOPLE:		//定时赛
		{
			log("People Btn click");
			curMatchListTag = ROOMLIST_BTN_PEOPLE;
			reflashListView();
		}
		break;

	default:
		break;
	}

	//比赛场房间
	if( tagId >= ROOMLIST_BTN_MATCH )
	{
		int index = tagId - ROOMLIST_BTN_MATCH;
		cocos2d::__Array* rooms = rooms = WWDataManager::getInstance()->getGameZone()->getMatchRooms( curMatchListTag - ROOMLIST_BTN_PEOPLE + 1 );
		MatchRoom* room = (MatchRoom*)(rooms->getObjectAtIndex(index));
		//TODO 报名
		MsgMatchRequest::sendIMatchSignup(room->getMatchID(),
				1,//(int1)1报名，2退赛  3请求报名人数
				WWDataManager::getInstance()->getUserBasicInfoData()->getNickName().c_str(),
				WWDataManager::getInstance()->getUserBasicInfoData()->getVipGrade(),
				0);
		//if(room->getSignupFlag())
		//{
		//	MsgMatchRequest::sendIMatchSignup(room->getMatchID(),
		//		2,//(int1)1报名，2退赛  3请求报名人数
		//		WWDataManager::getInstance()->getUserBasicInfoData()->getNickName().c_str(),
		//		WWDataManager::getInstance()->getUserBasicInfoData()->getVipGrade(),
		//		0);
		//}
		//else
		//{
		//	MsgMatchRequest::sendIMatchSignup(room->getMatchID(),
		//		1,//(int1)1报名，2退赛  3请求报名人数
		//		WWDataManager::getInstance()->getUserBasicInfoData()->getNickName().c_str(),
		//		WWDataManager::getInstance()->getUserBasicInfoData()->getVipGrade(),
		//		0);
		//}
			
	}
}

//消息处理
//void RoomListLayer::onDataReceiver(int msgType, cocos2d::Ref* objectData)
//{
//	log("LobbyScene rec msgType : %d", msgType);
//	switch (msgType)
//	{
//	case MSG_GAME_ZOON_LIST:
//		log("LobbyScene::onDataReceiver---- MSG_GAME_ZOON_LIST");
//		reflashListView();
//		break;
//	default:
//		break;
//	}
//}

#include "LobbyScene.h"
#include "View/LoginScene/LoginScene.h"

#include "BaseCore/Common/MsgConstDefine.h"
#include "BaseCore/Common/GameConfig.h"
#include "BaseCore/Common/FactoryTools.h"
#include "BaseCore/Common/UICoordConfig.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Control/WWDataManager/WWDataManager.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "View/Common/SetLayer/SetLayer.h"
#include "../Common/ChargeLayer/ChargeLayer.h"
#include "../Common/UserInfoLayer/UserInfoLayer.h"
#include "../../Control/WWCSound/WWSoundManager.h"
#include "View/GameScene/GameOver/GameOverLayer.h"
#include "View/Common/CommLayer/CommDialog.h"
#include "Control/WWRequest/MsgChargeRequest.h"
#include "BaseCore/WWBGUI/WWToast.h"
#include "BaseCore/WWBGUI/WWTouchSprite.h"
#include "AutoTest/TestScene.h"

#include "Control/WWDataManager/GameGlobalDataManager.h"
#include "Control/WWRequest/MsgLogonRequest.h"
#include "View/GameScene/GameLogicManager.h"
#include "View/Common/UserInfoLayer/UserInfoLayer.h"
#include "View/Common/ChargeLayer/ChargeLayer.h"
#include "View/Common/SetLayer/SetLayer.h"

#include "Control/WWRequest/MsgCommonRequest.h"
#include "Control/WWRequest/MsgMatchRequest.h"

//View节点头文件
#include "View/Common/PropShop/PropShopLayer.h"

USING_NS_CC;

using namespace std;

LobbyScene::LobbyScene():
	m_pMainView(nullptr),
	m_pUITag(0),
	recvMsgCount(0)
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);
}

bool LobbyScene::init()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

	bool bRet = false;
	do
	{

		//MsgLobbyRequest::requestRoomDatas( 1001, 2);

		// Scene根Layer节点创建
		m_pMainView = MainViewLayer::create();
		CC_BREAK_IF(!m_pMainView);
		m_pMainView->setAnchorPoint(Vec2(0, 0));
		m_pMainView->setPosition(Vec2(0, 0));
		this->addChild(m_pMainView, 1);

		//背景音乐
		if (FileUtils::getInstance()->isFileExist("res/Audio/mp3/rivulet.mp3") 
					&& UserDefault::getInstance()->getBoolForKey(SET_BG_MUSIC_KEY,true))
		{
			//WWSoundManager::getInstance()->preloadBackgroundMusic("res/Audio/mp3/rivulet.mp3");
			//WWSoundManager::getInstance()->playBackgroundMusic("res/Audio/mp3/rivulet.mp3", true);
		}

		//UnitTest
		if (DebugMode == 1)
		{
			scheduleOnce(CC_SCHEDULE_SELECTOR(LobbyScene::setTest), 2.0f);
		}
		if (DebugMode == 3)
		{
			scheduleOnce(CC_SCHEDULE_SELECTOR(LobbyScene::autoTestEnterGame), 2.0f);
		}

		bRet = true;

	}while (0);

	return bRet;
}

LobbyScene::~LobbyScene()
{ 
	log("%s %d\r\n", __FUNCTION__, __LINE__);
}

void LobbyScene::onEnter()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);

    Scene::onEnter();
	
	//当前界面在大厅
	GameGlobalDataManager::getInstance()->setCurSceneType(GameSceneType::LOBBY_SCENE);

	//注册捕捉监听
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(LobbyScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	log("%s \r\n", (Director::getInstance()->getTextureCache()->getCachedTextureInfo()).c_str());

	scheduleUpdate();

	
	MsgLobbyRequest::requestRoomDatas(1001,2);
	MsgLobbyRequest::requestRoomDatas(1001,4);

	//test code  宝箱
	//MsgCommonRequest::sendOpenMoneyBoxReq(0,1,1102);

	//MsgCommonRequest::sendOpenMoneyBoxReq(20785,2,1105);

	//MsgCommonRequest::sendOpenMoneyBoxReq(20692,2,1102);

	//测试比赛
	//MsgMatchRequest::sendIMatchListReq(1,0);

	MsgMatchRequest::sendIMatchListReq(2,0);
}

void LobbyScene::onExit()
{
	log("%s %d\r\n", __FUNCTION__, __LINE__);
	
	_eventDispatcher->removeEventListenersForTarget(this);

	unscheduleUpdate();

	// 释放资源
	this->resume();
	this->unscheduleAllCallbacks();
	this->stopAllActions();

	this->removeAllChildrenWithCleanup(true);

	Scene::onExit();

	// TODO 
	// 释放材质
	Director::getInstance()->getTextureCache()->removeUnusedTextures(); //TODO 这里先使用全部释放掉，在UI编辑的时候，需要用plist文件去释放掉

	log("%s \r\n", (Director::getInstance()->getTextureCache()->getCachedTextureInfo()).c_str());
	
}

//void LobbyScene::ButtonDepress(Ref * theBtn)
//{
//	MenuItem *pItem = (MenuItem*)theBtn; 
//	Menu* pMenu = (Menu*)pItem->getUserData(); 
//	int tagId = pMenu->getTag();
//	switch(tagId)
//	{
//	case BTN_LOBBYLAYER_NOTICE_TAG:{ //公告
//			log("notice button press!");
//			//WWSceneManager::getInstance()->changeToTest();
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//		}
//		break;
//	case BTN_LOBBYLAYER_FIRST_BUY_TAG:{ //首充
//			log("first buy button press!");
//			//MsgLobbyRequest::requestRoomDatas( 1001, 2);
//			MsgChargeRequest::requestMoneyInfo();
//		}
//		break;
//	case BTN_LOBBYLAYER_HEAD_TAG:{ // 大厅玩家头像
//			log("top bar head frame click!");
//			auto userInfo = UserInfoLayer::create();
//			userInfo->setPosition(Vec2(0,0));
//			this->addChild(userInfo,BASIC_ZORDER_FRAME);
//		}
//		break;
//	case BTN_LOBBYLAYER_COINADD_TAG:{ // 顶部工具条金币“+”
//			log("topBar coin add button press!");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//		}
//		break;
//	case BTN_LOBBYLAYER_DIAMADD_TAG:{ // 顶部工具条钻石“+”
//			log("topBar diam add button press!");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//
//		}
//		break;
//	case BTN_LOBBYLAYER_TOPBAR_MENU_TAG:{ // 顶部下拉菜单
//			log("topBar menu button press!");
//		}
//		break;
//	
//	default:
//		break;
//	}
//}
//
//// touchSprite 按纽响应
//void LobbyScene::onTouchSpriteDepress(Ref * theBtn)
//{
//	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
//	int tagId = btn->getTag();
//	switch (tagId)
//	{
//	case BTN_LOBBYLAYER_HAPPY_TAG: //欢乐斗地主
//		{
//			if(recvMsgCount <2)
//			{
//				return;
//			}
//			log("happy room button press!");
//			if (this->getChildByTag(BTN_LOBBYLAYER_HAPPY_TAG + 1000))
//			{
//				this->removeChildByTag(BTN_LOBBYLAYER_HAPPY_TAG + 1000);
//			}
//			auto l_lRoomLayer = RoomListLayer::create(RoomType::HAPPY_ROOM);
//			l_lRoomLayer->setPosition(Vec2(0, 0));
//			this->addChild(l_lRoomLayer,2, BTN_LOBBYLAYER_HAPPY_TAG + 1000);
//			log("had click m_pHappyRoomBtn");
//		}
//		break;
//		
//	case BTN_LOBBYLAYER_RASCAL_TAG: // 赖子斗地主
//		{
//			if(recvMsgCount <2)
//			{
//				return;
//			}
//			log("rascal room button press!");
//			auto l_lRoomLayer = RoomListLayer::create(RoomType::RASCAL_ROOM);
//			l_lRoomLayer->setPosition(Vec2(0, 0));
//			this->addChild(l_lRoomLayer,4);
//
//			//l_lRoomLayer->setpPlayerTypeID(4);
//		}
//		break;
//	case  BTN_LOBBYLAYER_MATCH_TAG: // 比赛场
//		{
//			log("match room button press!");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//
//		}
//	case  BTN_LOBBYLAYER_CRAZY_TAG: // 疯狂场
//		{
//			log("crazy room button press!");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//
//		}
//	case  BTN_LOBBYLAYER_FUNCTAG_BAG: //背包
//		{
//			log("BTN_LOBBYLAYER_FUNCTAG_BAG");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//
//		}
//		break;
//	case  BTN_LOBBYLAYER_FUNCTAG_ACHI: //成就
//		{
//			log("BTN_LOBBYLAYER_FUNCTAG_ACHI");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//
//		}
//		break;
//	case  BTN_LOBBYLAYER_FUNCTAG_DIALY: //签到
//		{
//			log("BTN_LOBBYLAYER_FUNCTAG_DIALY");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//
//		}
//		break;
//	case  BTN_LOBBYLAYER_FUNCTAG_RANK: //排行榜
//		{
//			log("BTN_LOBBYLAYER_FUNCTAG_RANK");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//
//		}
//		break;
//	case  BTN_LOBBYLAYER_FUNCTAG_SET: //设置
//		{
//			log("BTN_LOBBYLAYER_FUNCTAG_SET");
//			auto set = SetLayer::create(true);
//			set->setAnchorPoint(Vec2(0.5,0.5));
//			set->setPosition(CUR_SCREEN_SIZE.width/2 - set->getContentSize().width/2,CUR_SCREEN_SIZE.height/2-set->getContentSize().height/2);
//			this->addChild(set, 100);
//		}
//		break;
//		case BTN_LOBBYLAYER_EMAIL_TAG:{ // 邮件系统
//			log("bottom bar email button press!");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//
//		}
//		break;
//	case BTN_LOBBYLAYER_TESK_TAG:{ // 每日任务
//			log("bottom bar tesk button press!");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//
//		}
//		break;
//	case BTN_LOBBYLAYER_BUY_TAG:{ // 充值
//			log("bottom bar buy button press!");
//			auto charge = ChargeLayer::create();
//			charge->setAnchorPoint(Vec2(0.5,0.5));
//			charge->setPosition(CUR_SCREEN_SIZE.width/2 - charge->getContentSize().width/2,CUR_SCREEN_SIZE.height/2-charge->getContentSize().height/2);
//			this->addChild(charge, 100);
//		}
//		break;
//	case BTN_LOBBYLAYER_SHOP_TAG:{ // 商城
//			log("bottom bar shop button press!");
//            WWToast::makeText(GetWWString("room_zhunru","Other"));
//			//WWSceneManager::getInstance()->openGlobalUI(GameOverLayer::create(NULL));
//		}
//		break;
//	case BTN_LOBBYLAYER_ACTIVITY_TAG:{ // 活动中心
//			log("bottom bar activity button press!");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//
//		}
//		break;
//	case BTN_LOBBYLAYER_QUICK_START_TAG:{ //快速开始
//			log("bottom bar quick start button press!");
//			WWToast::makeText(GetWWString("room_zhunru","Other"));
//
//		}
//		break;
//	default:
//		break;
//	}
//}

void LobbyScene::exitGame(Ref* pSendser)
{
	log("exit game ....");//退出游戏 发送登出消息
	WWToast::makeText(GetWWString("room_zhunru","Other"));
	//WWSceneManager::getInstance()->changeToLogin();
}

void LobbyScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
    {
		//just for test. 不弹提示框，直接注销
		MsgLogonRequest::sendLogonOut();
		WWNetWorkManager::getInstance()->getCurSocket()->CloseSocket();
		GameGlobalDataManager::getInstance()->setLoginFromType(LoginType::LOGIN_FROM_LOBBY);

	   	auto pScene = LoginScene::create();
		Director::getInstance()->replaceScene(pScene);

		/*if(getUITag() > 0)
		{
			WWSceneManager::getInstance()->closeGlobalUI(getUITag());
			m_pUITag = 0;
		}
		else
		{
			auto tishi = CommDialog::creatTwoBtnOneMessage(GetWWString("h_Exit_sure","HallScene").c_str(),GetWWString("h_Exit_cansle","HallScene").c_str()
				,GetWWString("h_Exit_tishi","HallScene").c_str(),callfuncO_selector(LobbyScene::exitGame),NULL);
			tishi->setPosition(CUR_SCREEN_SIZE.width * 0.5f - tishi->getContentSize().width * 0.5f,CUR_SCREEN_SIZE.height * 0.5f - tishi->getContentSize().height * 0.5f);
			m_pUITag = UI_EXIT_TAG;
			WWSceneManager::getInstance()->openGlobalUI(tishi,UI_EXIT_TAG);
		}*/
		
    }
}

void LobbyScene::unitTest(float dt)
{
	this->removeChildByTag(96);
	MsgLogonRequest::sendLogonOut();
	WWNetWorkManager::getInstance()->getCurSocket()->CloseSocket();

	//切换到测试场景
	auto s_TestScene = TestScene::create();
	Director::getInstance()->replaceScene(s_TestScene);

	//auto s_TestScene = LoginScene::create();
	//Director::getInstance()->replaceScene(s_TestScene);
}

void LobbyScene::setTest(float dt)
{
	auto set = SetLayer::create(true);
	set->setTag(94);
	set->setAnchorPoint(Vec2(0.5,0.5));
	set->setPosition(CUR_SCREEN_SIZE.width/2 - set->getContentSize().width/2,CUR_SCREEN_SIZE.height/2-set->getContentSize().height/2);
	this->addChild(set, 100);
	scheduleOnce(CC_SCHEDULE_SELECTOR(LobbyScene::chargeTest), 2.0f);
}

void LobbyScene::chargeTest(float dt)
{
	this->removeChildByTag(94);
	auto charge = ChargeLayer::create();
	charge->setTag(85);
	charge->setAnchorPoint(Vec2(0.5,0.5));
	charge->setPosition(CUR_SCREEN_SIZE.width/2 - charge->getContentSize().width/2,CUR_SCREEN_SIZE.height/2-charge->getContentSize().height/2);
	this->addChild(charge, 100);
	scheduleOnce(CC_SCHEDULE_SELECTOR(LobbyScene::infoTest), 2.0f);
}

void LobbyScene::infoTest(float dt)
{
	this->removeChildByTag(95);
	auto userInfo = UserInfoLayer::create();
	userInfo->setTag(96);
	userInfo->setPosition(Vec2(0,0));
	this->addChild(userInfo,BASIC_ZORDER_FRAME);
	scheduleOnce(CC_SCHEDULE_SELECTOR(LobbyScene::unitTest), 2.0f);
}

void LobbyScene::autoTestEnterGame(float dt)
{//启动游戏，默认 进入第一个 房间
	if(DebugMode == 2)
	{
		int m_pPlayerTypeID = 2;
		cocos2d::__Array* rooms = WWDataManager::getInstance()->getGameZone()->getRooms(m_pPlayerTypeID);
		GameRoom* room = (GameRoom*)(rooms->getObjectAtIndex(0));
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
}

void LobbyScene::update(float dt)
{
	dealNetRecv();
}

void LobbyScene::onMsgRecv(int msgType)
{
	switch (msgType)
	{
	case MSG_GAME_ZOON_LIST:
		log("-------------------登陆成功------------------------");
		parseRoomListData();
		recvMsgCount++;
		break;
	case MSG_STORE_MAGIC_LIST:
		{
			parseStoreMagicList(MSG_STORE_MAGIC_LIST);
		}
		break;
	case MSG_STORE_CANSALE_LIST:
		{
			parseStoreMagicList(MSG_STORE_CANSALE_LIST);
		}
		break;
	case MSG_BUY_MAGIC_RESP:
		parseBuyMagicResp();
		break;
	case MSG_IMatchListResp://返回即时比赛列表
		parseIMatchListResp();
		break;
	case MSG_IMatchInfoReq://返回即时比赛详情
		parseIMatchInfoResp();
		break;
	case MSG_IMatchSignupResp://报名|退赛回执
		parseIMatchSignupResp();
		break;
	case MSG_BUDDY_INFO_RESP:
		parseBuddyList();//好友列表
		break;
	default:
		break;
	}
}
void LobbyScene::onRootMsgRecv(int msgType, int userId, int result, std::string reason, int reasonType)
{
	CommDialog* dialog;
	const char* signUp;
	if(result)
	{
		signUp = "sign up false";
	}
	else
	{
		signUp = "sign up success";
	}
	switch (msgType)
	{
	case MSG_IMatchSignup:
		//报名通用返回
		dialog = CommDialog::creatOneBtnOneMessage("sure",signUp);
		this->addChild(dialog, BASIC_ZORDER_TOP);
		break;
	}
}

void LobbyScene::parseRoomListData()
{
	CCLOG("LobbyScene::parseRoomListData");
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int count = pClientSocket->ReadUnsignedShort(); //游戏数量
	int playType = 0;
	for (int i = 0; i < count; i++)
	{
		int gameId = pClientSocket->ReadInt(); // (int4)游戏编码
		int gameAcount = pClientSocket->ReadInt(); // (int4)游戏人数
		int gzCount = pClientSocket->ReadUnsignedShort(); // (int2)游戏区数量
		if (gzCount == 0)
		{
			return;
		}
		cocos2d::__Array* arrays = cocos2d::__Array::create();
		for (int m = 0; m < gzCount; m++)
		{
			int gzId = pClientSocket->ReadInt(); // (int4)游戏区1ＩＤ
			std::string gameName = pClientSocket->ReadString(); // (String)游戏区1名称
			int zoneWin = pClientSocket->ReadChar(); // (int1)标识游戏区是否是财富游戏区，1：普通游戏区 2：钻石区,5 金币区
			int status = pClientSocket->ReadChar();					// (byte)游戏区1状态
			std::string des = pClientSocket->ReadString();// (String)游戏区1描述
			int acount = pClientSocket->ReadUnsignedShort();//(int2)游戏区人数
			int playerCount = pClientSocket->ReadUnsignedShort();//(int2)游戏区每局人数
			playType = pClientSocket->ReadUnsignedShort();	//(int2)游戏玩法

			//----------

			int multiple = pClientSocket->ReadUnsignedShort();	//(int2)倍分--------------------
			int enterFortune = pClientSocket->ReadInt();//(int4)带入值----------------



			int zoneType = pClientSocket->ReadUnsignedShort();				// (int2)分区类型 1：普通分区 2：比赛/活动区 3：VIP专区
			int invite = pClientSocket->ReadChar();					// (int1)是否可以约战

			int fortuneBase = pClientSocket->ReadInt();				// (int4)财富计算的底数
			int pointBase = pClientSocket->ReadInt();				// (int4)积分计算的底数

			int LevelMinLimit = pClientSocket->ReadInt();				//(int4)等级下限-------------------
			int LevelMaxLimit = pClientSocket->ReadInt();				//(int4)等级上限-------------------

			int PresentBean = pClientSocket->ReadChar();			//(int1)是否赠豆，0不赠，1赠送
			int PlayedCount = pClientSocket->ReadUnsignedShort();	//(int2)当天当前局数
			int TableFee = pClientSocket->ReadInt();					//(int4)桌费
			//int BeanVipTax = pClientSocket->ReadInt();				//(int4)蛙豆房VIP系统收税
			int RecordCardFalg = pClientSocket->ReadChar();			//(int1)记牌器标志------------------
			int AntiCheatFlag = pClientSocket->ReadChar();			//(int1)防作弊标志---------------------
			int LockFlag = pClientSocket->ReadChar();				//(int1)锁标志 0 无锁 1 有锁
			log("parseRoomData::gameId=%d,gzId= %d,zoneWin= %d,des= %s,PeopleNumber= %d,playType= %d,multiple=%d,enterFortune= %d,""invite= %d,gameName= %s,zoneType= %d,fortuneBase= %d,pointBase= %d,LevelMinLimit=%d,LevelMaxLimit=%d,PresentBean=%d,PlayedCount=%d,TableFee=%d,RecordCardFalg=%d,AntiCheatFlag=%d,LockFlag=%d"
				, gameId, gzId, zoneWin, des.c_str(), acount, playType, multiple, enterFortune, invite, gameName.c_str(), zoneType, fortuneBase, pointBase,LevelMinLimit,LevelMaxLimit,PresentBean,PlayedCount,TableFee,RecordCardFalg,AntiCheatFlag,LockFlag);
			GameRoom* room = GameRoom::create();
			room->setGameId(gameId);
			room->setGZID(gzId);
			room->setName(gameName);
			room->setZoneWin(zoneWin);
			room->setStatus(status);
			room->setDescription(des);
			room->setAcount(acount);
			room->setPlayerCount(playerCount);
			room->setPlayType(playType);
			room->setMultiple(multiple);
			room->setEnterFortune(enterFortune);
			room->setZoneType(zoneType);
			room->setInvite(invite);
			room->setFortuneBase(fortuneBase);
			room->setLevelMinLimit(LevelMinLimit);
			room->setLevelMaxLimit(LevelMaxLimit);
			room->setPresentBean(PresentBean);
			room->setPlayedCount(PlayedCount);
			room->setTableFee(TableFee);
			room->setRecordCardFalg(RecordCardFalg);
			room->setAntiCheatFlag(AntiCheatFlag);
			room->setLockFlag(LockFlag);
			//arrays.pushBack(room);
			arrays->addObject(room);
		}
		if(!WWDataManager::getInstance()->getGameZone())
		{
			GameZone* zone = GameZone::create();
			zone->setRooms(playType, arrays);
			WWDataManager::getInstance()->setGameZone(zone);
		}
		else
		{
			WWDataManager::getInstance()->getGameZone()->setRooms(playType, arrays);
		}

		//WWDataManager::getInstance()->getGameZone()->getRooms(2)->count();
		//log("WWDataManager::getInstance()->getGameZone()---------%d",WWDataManager::getInstance()->getGameZone()->getRooms(2)->count());

		//GameRoom* r = (GameRoom*)(WWDataManager::getInstance()->getGameZone()->getRooms(2)->getObjectAtIndex(0));
		//log("WWDataManager::getInstance()->getGameZone()---------%s",r->getName().c_str());
	}
	if(count>0)
	{
		this->scheduleOnce(CC_SCHEDULE_SELECTOR(LobbyScene::autoTestEnterGame), 2.0f);
	}
}

void LobbyScene::parseStoreMagicList(int msgId)
{
	//接受到商城的数据，去调用商城的数据解析函数
	auto curPopFrame = static_cast<PropShopLayer*>(Director::getInstance()->getRunningScene()->getChildByTag(FRAMEPOP_TAG_PROPSHOP));
	if (curPopFrame && (curPopFrame->getName() == "PropShopLayer") && (msgId == MSG_STORE_MAGIC_LIST))
	{
		curPopFrame->onDataReceiver();
	}
	else if(curPopFrame && (curPopFrame->getName() == "PropShopLayer") && (msgId == MSG_STORE_CANSALE_LIST))
	{
		curPopFrame->onCanSaleListReceiver();
	}
}

void LobbyScene::parseBuyMagicResp()
{
	auto curPopFrame = dynamic_cast<PropShopLayer*>(Director::getInstance()->getRunningScene()->getChildByTag(FRAMEPOP_TAG_PROPSHOP));
	if (curPopFrame && (curPopFrame->getName() == "PropShopLayer"))
	{
		auto buyLayer = dynamic_cast<BuyDialog*>(curPopFrame->getChildByTag(SHOPTAG_BUYLAYER_TAG));
		if (buyLayer) //存在购买框子节点
		{
			buyLayer->onDataReceiver();
		}
	}
}

void LobbyScene::parseIMatchListResp()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int GameID = pClientSocket->ReadInt();//(int4)游戏ID
	std::string CurTime = pClientSocket->ReadString();//(String)服务器当前时间戳,from 1970-1-1 0:0:0

	int Count = pClientSocket->ReadShort();//(int2)比赛数量

	log("LobbyScene:: parseIMatchListResp---------GameID=%d, CurTim=%s, Count=%d",GameID,CurTime.c_str(),Count);
	
	cocos2d::__Array* rooms_1 = cocos2d::__Array::create();
	cocos2d::__Array* rooms_2 = cocos2d::__Array::create();

	for (int i = 0; i < Count; i++)
	{
		MatchRoom* room = MatchRoom::create();
		room->setGameId(GameID);
		room->setCurTime(CurTime);

		int MatchID = pClientSocket->ReadInt();//(int4)比赛ID
		room->setMatchID(MatchID);

		int Matchtype = pClientSocket->ReadChar();//(int1)比赛类型 1水晶赛，2大奖赛
		room->setMatchtype(Matchtype);

		int BeginType = pClientSocket->ReadChar();//开赛方式：1：指定人数开赛 2：定时开赛
		room->setBeginType(BeginType);

		int MatchPartCount = pClientSocket->ReadChar();//(int1)比赛赛段数
		room->setMatchPartCount(MatchPartCount);

		std::string MatchName = pClientSocket->ReadString();//(String)比赛名称
		room->setMatchName(MatchName);

		int Hot = pClientSocket->ReadChar();//(int1)1=热门比赛
		room->setHot(Hot);

		std::string MatchTime = pClientSocket->ReadString();//(String)开赛时间
		room->setMatchTime(MatchTime);

		std::string SignupTerm = pClientSocket->ReadString();//(String)报名条件
		room->setSignupTerm(SignupTerm);

		int SignupPlayer  = pClientSocket->ReadInt();//(int4)报名人数
		room->setSignupPlayer(SignupPlayer);

		int SignupFlag = pClientSocket->ReadChar();//(int1)报名状态 0=未报名 1=报名
		room->setSignupFlag(SignupFlag);

		int WaitTime = pClientSocket->ReadInt();//(int4)等待开赛时间,单位秒
		room->setWaitTime(WaitTime);

		int IntervalTime = pClientSocket->ReadInt();//(int4)开赛间隔时间
		room->setIntervalTime(IntervalTime);

		int TicketFID = pClientSocket->ReadInt();//(int4)报名门票FID
		room->setTicketFID(TicketFID);

		int curPlayingPlayer = pClientSocket->ReadInt();//(int4)比赛中正在游戏中人数
		room->setCurPlayingPlayer(curPlayingPlayer);

		log("LobbyScene:: parseIMatchListResp---------MatchID=%d,Matchtype=%d,BeginType=%d,MatchPartCount=%d,MatchName=%s,Hot=%d,MatchTime=%s,SignupTerm=%s,SignupPlayer=%d,SignupFlag=%d,WaitTime=%d,IntervalTime=%d,TicketFID=%d,curPlayingPlayer=%d",
			MatchID,Matchtype,BeginType,MatchPartCount,MatchName.c_str(),Hot,
			MatchTime.c_str(),SignupTerm.c_str(),SignupPlayer,SignupFlag,
			WaitTime,IntervalTime,TicketFID,curPlayingPlayer);

		if(BeginType ==1)
		{
			rooms_1->addObject(room);
		}
		else if(BeginType ==2)
		{
			rooms_2->addObject(room);
		}
	}
	if(!WWDataManager::getInstance()->getGameZone())
	{
		GameZone* zone = GameZone::create();
		zone->setMatchRooms(1, rooms_1);
		zone->setMatchRooms(2, rooms_2);
		WWDataManager::getInstance()->setGameZone(zone);
	}
	else
	{
		WWDataManager::getInstance()->getGameZone()->setMatchRooms(1, rooms_1);
		WWDataManager::getInstance()->getGameZone()->setMatchRooms(2, rooms_2);
	}
	WWDataManager::getInstance()->getUserBasicInfoData()->getNickName();
	//MsgMatchRequest::sendIMatchSignup(188,1,WWDataManager::getInstance()->getUserBasicInfoData()->getNickName().c_str(),0,1);
}

void LobbyScene::parseIMatchInfoResp()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int MatchID = pClientSocket->ReadInt();//(int4)比赛ID
	std::string Name = pClientSocket->ReadString();//(String)比赛名称
	std::string MatchTime = pClientSocket->ReadString();//(String)开赛时间
	std::string SignupTerm = pClientSocket->ReadString();//(String)报名条件

	int BeginPlayer = pClientSocket->ReadInt();//(int4)开赛人数

	int SignupPlayer  = pClientSocket->ReadInt();//(int4)已经报名人数

	int Matchtype = pClientSocket->ReadChar();//(int1)比赛类型 1水晶赛，2大奖赛

	int BeginType = pClientSocket->ReadChar();//开赛方式：1：指定人数开赛 2：定时开赛

	int MatchPartCount = pClientSocket->ReadChar();//(int1)比赛赛段数

	int SignupFlag = pClientSocket->ReadChar();//(int1)报名状态 0=未报名 1=报名

	int WaitTime = pClientSocket->ReadInt();//(int4)等待开赛时间,单位秒

	int AwardCount = pClientSocket->ReadShort();//(int2)奖励条目数量

	for (int i = 0; i < AwardCount; i++)
	{
		int BeginRankNo = pClientSocket->ReadShort();//(int2) 奖励名次范围开始值
		int EndRankNo = pClientSocket->ReadShort();//(int2) 奖励名次范围结束值

		std::string Award = pClientSocket->ReadString();//(String)比赛奖励描述
		int initScore = pClientSocket->ReadInt();//[int4]初始积分

		std::string knockoutDesc = pClientSocket->ReadString();//(String)淘汰赛描述
		std::string finalsDesc = pClientSocket->ReadString();//(String)决赛描述
	}
}

//报名|退赛回执
void LobbyScene::parseIMatchSignupResp()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	int MatchID = pClientSocket->ReadInt();//(int4)比赛ID

	int Action = pClientSocket->ReadChar();//(int1)1报名，2退赛

	int result = pClientSocket->ReadChar();//(byte)结果  0 成功  1失败
	log("LobbyScene::parseIMatchSignupResp------MatchID = %d, Action = %d, result = %d",MatchID, Action, result);

	MatchRoom* room;
	if(result == 0)
	{
		//TODO 报名或退赛 成功
		room = WWDataManager::getInstance()->getGameZone()->getMatchRoomByID(MatchRoom::MATCH_PEOPLE, MatchID);
		if(room)
		{
			if(Action == 1)
				room->setSignupFlag(1);
			else
				room->setSignupFlag(0);
			return;
		}
		else
		{
			room = WWDataManager::getInstance()->getGameZone()->getMatchRoomByID(MatchRoom::MATCH_TIME, MatchID);
			if(Action == 1)
				room->setSignupFlag(1);
			else
				room->setSignupFlag(0);
		}
		//TODO 刷新比赛房间界面
	}
	else
	{
		//TODO 报名或退赛 失败
	}
}

void LobbyScene::parseBuddyList()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	
	/**
		0=我的好友
		1=系统推荐好友 
		2=新版我的好友
	*/
	int type = pClientSocket->ReadChar();
	//返回好友数量
	int count = pClientSocket->ReadShort();
	log("type=%d,count=%d",type,count);
	for (int i = 0; i < count; i++)
	{
		int userId = pClientSocket->ReadInt();//用户id

		int iconId = pClientSocket->ReadInt();//用户头像id

		std::string nickName = pClientSocket->ReadString();//用户昵称

		int grade = pClientSocket->ReadInt();//等级

		int online = pClientSocket->ReadChar();//在线状态 0=离线 1=在线

		int donateFlag = pClientSocket->ReadChar();//赠送标识 0=未赠送 1=已赠送

		int recvieFlag = pClientSocket->ReadChar();//领取标识 0=未领取 1=已领取

		std::string lastTime = pClientSocket->ReadString();//上次登录时间

		log("LobbyScene: userid=%d,iconid=%d,nickName=%s,grade=%d,online=%d,donateFlag=%d,recvierFlag=%d,lastTime=%s",
			userId,iconId,nickName.c_str(),grade,online,donateFlag,recvieFlag,lastTime.c_str());
	}
}
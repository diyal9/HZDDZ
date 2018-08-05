#include "LoginScene.h"

#include "View/LobbyScene/LobbyScene.h"

#include "BaseCore/Common/UICoordConfig.h"
#include "Control/WWRequest/MsgLogonRequest.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "BaseCore/Common/MsgConstDefine.h"
#include "WWMacros.h"

#include "Model/WWDataModel/WWUserData/UserData.h"
#include "BaseCore\WWBGUI\WWToast.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Model/WWFilter/MsgGlobalFilter.h"
#include "Model/WWDataModel/CommonMessageData/CommonMessageData.h"
#include "View/Common/CommLayer/CommDialog.h"
#include "BaseCore/WWBLog/LogUtil.h"
#include "Control/WWDataManager/GameGlobalDataManager.h"
#include "Control/WWDataManager/WWDataManager.h"
#include "BaseCore/Common/VisibleRect.h"
#include "BaseCore/WWBTools/StringTool.h"

//#if (COCOS2D_DEBUG >= 1) 
#include "AutoTest/TestScene.h"
//#endif

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#include <vld.h>
//#endif

#include "BaseCore/WWEffectBase/WWAnimation.h"

USING_NS_CC;

using namespace std;
//登入页面 plist文件地址
const char* login_plist_path = "res/Images/Login/login_scene.plist";
LoginScene::LoginScene()
{
	CCLOG("%s %d\r\n", __FUNCTION__, __LINE__);
}

LoginScene::~LoginScene()
{ 
	CCLOG("%s %d\r\n", __FUNCTION__, __LINE__);
}


bool LoginScene::init()
{
	CCLOG("%s %d\r\n", __FUNCTION__, __LINE__);

	bool bRet = false;
	do
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(login_plist_path);	
		//bg
		auto loginBgImg = Sprite::create("res/Images/Login/login_bg.jpg");
		CC_BREAK_IF(!loginBgImg);
		loginBgImg->setPosition(CUR_SCREEN_SIZE.width/2,CUR_SCREEN_SIZE.height/2);
		this->addChild(loginBgImg);

		//创建Logo
		auto logoRole = Sprite::createWithSpriteFrameName("login_logo_role.png");
		CC_BREAK_IF(!logoRole);
		logoRole->setPosition(Vec2(945, 663));
		loginBgImg->addChild(logoRole);

		auto logoTitle = Sprite::createWithSpriteFrameName("login_logo.png");
		CC_BREAK_IF(!logoTitle);
		logoTitle->setPosition(Vec2(957, 321));
		loginBgImg->addChild(logoTitle);


		//创建账号输入框
		//CreateLoginInput();

		//创建登录页面按纽
		//CreateLoginBtn();  //TODO 启动应用的时候，不会有BTN
	
		// 帮助按钮 相对于屏幕左边对齐
		auto gameInfoBtn = WWTouchSprite::createWithSpriteFrameName("login_info_btn.png",this,callfuncO_selector(LoginScene::onTouchSpriteDepress));
		gameInfoBtn->setTag(BTN_LOGINLAYER_GAMEINFO_TAG);
		auto diffPos = Vec2(LOGIN_UI_POS[8].x, CUR_SCREEN_SIZE.height/2 - LOGIN_UI_POS[8].y);
		Vec2 tempPos = VisibleRect::getNodeAlignPos(NodeAlignMent::LEFT,Size(137,137),diffPos);
		gameInfoBtn->setPosition(tempPos);
		this->addChild(gameInfoBtn, BASIC_ZORDER_ROOT);
	
		// version text. 相对于屏幕顶部对齐
		auto versionTx = Label::createWithTTF(VERSIONNUM,"res/Fonts/FounderSemiBold.ttf", GAME_FONT_SIZE_28, Size::ZERO,TextHAlignment::RIGHT,TextVAlignment::CENTER);
		diffPos = Vec2(CUR_SCREEN_SIZE.width/2 + LOGIN_UI_POS[9].x, LOGIN_UI_POS[9].y);
		tempPos = VisibleRect::getNodeAlignPos(NodeAlignMent::TOP,Size(LOGIN_UI_POS[10].x,LOGIN_UI_POS[10].y),diffPos);
		versionTx->setPosition(tempPos);
		this->addChild(versionTx, 1);

		auto sprCursor = Sprite::createWithSpriteFrameName("login_input_cursor.png");
		sprCursor->setTag(LOGINLAYER_INPUT_CURSOR_TAG);
		this->addChild(sprCursor);

		//input Cursor
		//auto sprCursor = Sprite::create("res/Images/Login/inputCursor.png");
		//sprCursor->setTag(LOGINLAYER_INPUT_CURSOR_TAG);
		//this->addChild(sprCursor);

		//test code
		//UserData::getInstance()->setIntValue(INT_USER_ID, 1157327);
		//UserData::getInstance()->setStringValue(STR_USER_PASSWORD,"DJQgLf");

		/*UserData::getInstance()->setIntValue(INT_USER_ID, 1161125);
		UserData::getInstance()->setStringValue(STR_USER_PASSWORD,"123456");*/

		UserData::getInstance()->setIntValue(INT_USER_ID, 1158245);
		UserData::getInstance()->setStringValue(STR_USER_PASSWORD,"23186336");
		//UserData::getInstance()->setIntValue(INT_USER_ID, 1161584);
		//UserData::getInstance()->setStringValue(STR_USER_PASSWORD,"123456");
		/*UserData::getInstance()->setIntValue(INT_USER_ID, 1161584);
		UserData::getInstance()->setStringValue(STR_USER_PASSWORD,"123456");*/

		/*UserData::getInstance()->setIntValue(INT_USER_ID, 150491857);
		UserData::getInstance()->setStringValue(STR_USER_PASSWORD,"123456");*/

		//UserData::getInstance()->setIntValue(INT_USER_ID, 1158249);
		//UserData::getInstance()->setStringValue(STR_USER_PASSWORD,"123456");

		//徐波
		/*UserData::getInstance()->setIntValue(INT_USER_ID, 1161621);
		UserData::getInstance()->setStringValue(STR_USER_PASSWORD,"123456");*/

		////肖老板
		/*UserData::getInstance()->setIntValue(INT_USER_ID, 1005344);
		UserData::getInstance()->setStringValue(STR_USER_PASSWORD,"3202910");*/

		/*测试账号
		pwd :: 123456

		uid :: 1161125  

		*/

		//test code

		//cocos2d::Map<int, cocos2d::__Array*> allRooms = Map<int,cocos2d::__Array*>();
		cocos2d::Map<int, cocos2d::__Array*> allRooms;
		cocos2d::__Array* arrays = cocos2d::__Array::create();
		GameRoom* room = GameRoom::create();
		room->setName("test name");
		arrays->addObject(room);

		allRooms.insert(2, arrays);
		GameRoom* r = (GameRoom*)(allRooms.at(2)->getObjectAtIndex(0));
		std::string s = r->getName();
		log("------------1111111111111----------------------%s", s.c_str());
		//

		if(GameGlobalDataManager::getInstance()->getLoginFromType() == LoginType::LOGIN_FROM_ICON)
		{
			startAction();

		}
		else if(GameGlobalDataManager::getInstance()->getLoginFromType() == LoginType::LOGIN_FROM_LOBBY)
		{
			CreateLoginBtn();
		}

		bRet = true;

	   if (DebugMode == 1)
	   {
		   //scheduleOnce(CC_SCHEDULE_SELECTOR(LoginScene::unitTest), 2.0f);
	   }
				


	}while (0);
	log("end of loading res");

	return bRet;
}

void LoginScene::onEnter()
{
	CCLOG("%s %d\r\n", __FUNCTION__, __LINE__);

    Scene::onEnter();
	
	//当前界面在登录
	GameGlobalDataManager::getInstance()->setCurSceneType(GameSceneType::LOGIN_SCENE);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(login_plist_path);

	//注册捕捉监听
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(LoginScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	CCLOG("%s \r\n", (Director::getInstance()->getTextureCache()->getCachedTextureInfo()).c_str());

	this->scheduleUpdate();

	//启动接收

	WWNetWorkManager::getInstance()->openRecvJob();
	if(GameGlobalDataManager::getInstance()->getLoginFromType() == LoginType::LOGIN_FROM_ICON)
	{
		sendLogin();
	}
}

void LoginScene::onExit()
{
	CCLOG("%s %d\r\n", __FUNCTION__, __LINE__);
	
	_eventDispatcher->removeEventListenersForTarget(this);

	unscheduleUpdate();

	// TODO save player data.
	//WWNetWorkManager::getInstance()->unregisterMsgFilter(this);

	// 释放资源
	
	this->resume();
	this->unscheduleAllCallbacks();
	this->stopAllActions();

	this->removeAllChildrenWithCleanup(true);

	Scene::onExit();
	

	// TODO 
	// 释放材质
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(login_plist_path);
	WWAnimation::getInstance()->removeAnimationCache("loginLoading");
	Director::getInstance()->getTextureCache()->removeUnusedTextures(); //TODO 这里先使用全部释放掉，在UI编辑的时候，需要用plist文件去释放掉
}


void LoginScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
    {
		//just for test. 不弹提示框，直接退出
		this->onExit();

		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
			MessageBox("WP操作系统，请自行退出","退出提醒 !!!");
			return;
		#endif
			Director::getInstance()->end();

		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			::exit(0);
		#endif

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

void LoginScene::startAction()
{
	Label* label = Label::createWithTTF(GetWWString("logining","LoginScene"),"res/Fonts/FounderSemiBold.ttf",30,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	label->setColor(GET_RGBCOLOR(TXT_LOGIN_COLOR));
	label->setAnchorPoint(Vec2(0.5f,0.5f));
	label->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5, label->getContentSize().height));
	this->addChild(label,BASIC_ZORDER_TOP+1);

	//调用创登录动画
	auto ani = WWAnimation::getInstance()->animationUnCache("loginLoading",13,0.04f,1);
	auto node = WWAnimation::getInstance()->createAnimationSprite("loginLoading");
	node->setPosition(Vec2(CUR_SCREEN_SIZE.width * 0.5, label->getPositionY() + label->getContentSize().height * 0.5 + node->getContentSize().height * 1.1));
	node->setScale(1.0f / 0.6f);
	node->setTag(TAG_LOGIN_ANIM);
	this->addChild(node, 100);

	node->runAction(RepeatForever::create(Animate::create(ani)));
}

void LoginScene::CreateLoginBtn()
{
	Node* loginBtnRoot = Node::create();
	loginBtnRoot->setTag(LOGIN_BTN_NODE_TAG);
	loginBtnRoot->setContentSize(Size(1920,139));

	//手机登入按钮
	auto mobileLoginBtn = WWTouchSprite::createWithSpriteFrameName("login_mobile_btn.png",this,callfuncO_selector(LoginScene::onTouchSpriteDepress));
	mobileLoginBtn->setPosition(LOGIN_UI_POS[5]);
	mobileLoginBtn->setTag(BTN_LOGINLAYER_GOBACK_TAG);
	loginBtnRoot->addChild(mobileLoginBtn, BASIC_ZORDER_ROOT);
	//快速开始
	auto quickStartBtn = WWTouchSprite::createWithSpriteFrameName("login_start_qucky.png",this, callfuncO_selector(LoginScene::onTouchSpriteDepress));
	quickStartBtn->setPosition(LOGIN_UI_POS[6]);
	quickStartBtn->setTag(BTN_LOGINLAYER_QUICKSTART_TAG);
	loginBtnRoot->addChild(quickStartBtn, BASIC_ZORDER_ROOT);
	//合作登入
	auto changeAccountBtn = WWTouchSprite::createWithSpriteFrameName("login_other_btn.png",this,callfuncO_selector(LoginScene::onTouchSpriteDepress));
	changeAccountBtn->setPosition(LOGIN_UI_POS[7]);
	changeAccountBtn->setTag(BTN_LOGINLAYER_CHANGEAC_TAG);
	loginBtnRoot->addChild(changeAccountBtn, BASIC_ZORDER_ROOT);
	
	// 设置工具条相对于屏幕底部对齐
	Vec2 tempPos = VisibleRect::getNodeAlignPos(NodeAlignMent::BOTTOM,loginBtnRoot->getContentSize(),Vec2(0,20));
	loginBtnRoot->setPosition(tempPos);

	this->addChild(loginBtnRoot, BASIC_ZORDER_ROOT+1);
}

bool LoginScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	Vec2 m_beginPos = pTouch->getLocationInView();	
	m_beginPos = Director::getInstance()->convertToGL(m_beginPos);
	return true;
}

void LoginScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{

}

void LoginScene::sendLogin()
{
	AccountUtils::getInstance()->init();
	auto accounts = AccountUtils::getInstance()->getAllAccountAry();
	if(!accounts.empty() && accounts.size() > 0)
	{
		log("krLog fast enter logon --------------account--");
		std::string userID = accounts.at(0);
		std::string pwd = AccountUtils::getInstance()->getPwdByUserId(userID.c_str());
		log("krLog userId=%s   pwd=%s",userID.c_str(),pwd.c_str());

		sendLogonByAcount(userID.c_str(), pwd.c_str());
	}
	else
	{
		log("krLog fast enter logon ------------fast enter----");
		sendlogonByFastEnter();
	}

}

/************************************************************************/
/*         页面按纽响应                                                 */
/************************************************************************/
/************************************************************************/
/*         页面按纽响应                                                 */
/************************************************************************/
void LoginScene::onTouchSpriteDepress(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int pTag = btn->getTag();
	switch (pTag)
	{
	case BTN_LOGINLAYER_LOGIN_TAG:
		log("login button press!");
		break;

	case BTN_LOGINLAYER_GOBACK_TAG:
		log("exit button press!");
		this->onExit();

		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
			MessageBox("WP操作系统，请自行退出","退出提醒 !!!");
			return;
		#endif
			Director::getInstance()->end();

		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			::exit(0);
		#endif
		break;

	case BTN_LOGINLAYER_QUICKSTART_TAG:
		log("quickStart button press!");
		if(this->getChildByTag(LOGIN_BTN_NODE_TAG))
		{
			this->removeChild(this->getChildByTag(LOGIN_BTN_NODE_TAG));
		}
		startAction();
		sendLogin();
		break;

	case BTN_LOGINLAYER_CHANGEAC_TAG:
		log("change account button press!");
		break;

	case BTN_LOGINLAYER_GAMEINFO_TAG:
		log("gameInfo button press!");
		break;
	default:
		break;
	}
}

void LoginScene::initTelephoneDialog()
{
}

void LoginScene::sendLogonByTelephone(const char* telephone, const char* checkCode)
{
	MsgLogonRequest::sendLogon(telephone, checkCode, TYPE_LOGIN_TELEPHONE);
}

void LoginScene::sendlogonByFastEnter()
{
	MsgLogonRequest::sendLogon("", "", TYPE_LOGIN_FAST);
}

void LoginScene::sendLogonByAcount(const char* userId, const char* userPwd)
{
	MsgLogonRequest::sendLogon(userId, userPwd, TYPE_LOGIN_ACCOUNT);
}

void LoginScene::logonFail(cocos2d::Ref* data)
{
	CCLOG("LoginScene::logonFail-------------");
	//CommonMessageData* obj = (CommonMessageData*)data;
	//std::string str = obj->getReason();
	//CommDialog* dialog = CommDialog::creatOneBtnOneMessage("sure", str.c_str());

	//this->addChild(dialog);
}

void LoginScene::update(float dt)
{
	dealNetRecv();
}

void LoginScene::onMsgRecv(int msgType)
{
	switch (msgType)
	{
	case MSG_LOGON_PROFILE:
		log("-------------------登陆成功------------------------");
		parseProfile();
	default:
		break;
	}
}
void LoginScene::onRootMsgRecv(int msgType, int userId, int result, std::string reason, int reasonType)
{
	CommDialog* dialog;
	switch (msgType)
	{
	case MSG_LOGON_INFO:
		log("-------------------登陆失败------------------------");
		dialog = CommDialog::creatOneBtnOneMessage("sure", reason.c_str());
		this->addChild(dialog);
		
		if(this->getChildByTag(TAG_LOGIN_ANIM))
		{
			this->removeChild(this->getChildByTag(TAG_LOGIN_ANIM));
		}
		
		if(!(this->getChildByTag(LOGIN_BTN_NODE_TAG)))
		{
			CreateLoginBtn();
		}
		break;
	default:
		break;
	}
}

void LoginScene::unitTest(float dt)
{
	//切换到测试场景
	auto s_TestScene = TestScene::create();
	Director::getInstance()->replaceScene(s_TestScene);



	//auto s_TestScene = LobbyScene::create();
	//Director::getInstance()->replaceScene(s_TestScene);
}

void LoginScene::parseProfile()
{

	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();


	int userID = pClientSocket->ReadInt();//用户ID
	int newRegister = pClientSocket->ReadChar();/*(int1) 0原有蛙号，1新注册蛙号*/
	std::string userPwd = pClientSocket->ReadString();//密码
	log("MsgLogonDataFilter::parseProfileNew-------userID = %d, newRegister = %d, userPwd = %s,",userID, newRegister, userPwd.c_str());

	UserData::getInstance()->setIntValue(INT_USER_ID,userID);
	UserData::getInstance()->setStringValue(STR_USER_PASSWORD,userPwd);
	AccountUtils::getInstance()->saveAccount(StringTool::intToString(userID).c_str(),userPwd.c_str());

	UserBasicInfoData* userData = new UserBasicInfoData();
	
	userData->setUserId(userID);//(int4)用户ID
	userData->setNewRegister(newRegister);
	userData->setUserPwd(userPwd);
	//userData->setGameId(pClientSocket->ReadInt());//(int4)游戏ID
	userData->setNickName(pClientSocket->ReadString());//(String)昵称
	userData->setIconID(pClientSocket->ReadInt());//(int4)用户头像ID
	userData->setGender(pClientSocket->ReadChar());//(int1)性别1-男，2－女-------------

	userData->setVipGrade(pClientSocket->ReadChar());//(int1)会员级别.0=非会员

	userData->setLevel(pClientSocket->ReadShort());//(int2)等级----------

	userData->setGameCash(pClientSocket->ReadString());//(String)金币数  
	userData->setGameCash2(pClientSocket->ReadString());//(String)钻石数 ------------
	userData->setRegisterTime(pClientSocket->ReadString());//(String)注册时间
	userData->setLastTime(pClientSocket->ReadString());//(String)上次登录时间
	userData->setAPN(pClientSocket->ReadString());//(String)网络接入
	userData->setRegion(pClientSocket->ReadString());//(String)地区

	log("MsgLogonDataFilter::parseProfile-----------UserID = %d, Nickname = %s,Gander = %d, VipGrade = %d,Level=%d, GameCash = %s,GameCash2=%s, RegisterTime = %s, LastTime = %s, APN = %s, IconID = %d, Region=%s"
		,userData->getUserId(),userData->getNickName().c_str(),userData->getGender(),userData->getVipGrade(),userData->getLevel(),userData->getGameCash().c_str(),userData->getGameCash2().c_str(),
		userData->getRegisterTime().c_str(),userData->getLastTime().c_str(), userData->getAPN().c_str() ,userData->getIconID(),userData->getRegion().c_str());
	
	
	WWDataManager::getInstance()->setUserBasicInfoData(userData);

	auto lobbyScene = LobbyScene::create();
	Director::getInstance()->replaceScene(lobbyScene);

}
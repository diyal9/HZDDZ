#include "UserInfoLayer.h"
#include "UserInfoLayerRes.h"
#include "../../../BaseCore/Common/UICoordConfig.h"
#include "../../../BaseCore/WWBGUI/WWTouchSprite.h"
#include "../../../WWMacros.h"
#include "../CommLayer/CommDialog.h"
#include "../../../BaseCore/Common/FactoryTools.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "BaseCore/WWBTools/StringTool.h"
#include "Control/WWRequest/MsgLogonRequest.h"
#include "Control/WWDataManager/WWDataManager.h"
#include "Control/WWDataManager/GameGlobalDataManager.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "View/LoginScene/LoginScene.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"


UserInfoLayer::UserInfoLayer()
{

}

UserInfoLayer::~UserInfoLayer()
{
	/*m_happyNodes.clear();
	m_laiziNodes.clear();
	m_bisaiNodes.clear();*/
	
}

bool UserInfoLayer::init()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(userinfoPlist);
	if ( !WWFrameBase::init() )
	{
		return false;
	}

	createView();
	
	return true;
}

void UserInfoLayer::createView()
{
	//添加背景
	bg = this->getpBg();
	//title
	auto title = Sprite::createWithSpriteFrameName(userInfoTitleTxt);
	title->setPosition(USER_INFO_POSTIONS[0]);
	bg->addChild(title, BASIC_ZORDER_ROOT);
	//内容背景
	auto contentBg = Sprite::create(contentBgImag);
	contentBg->setPosition(USER_INFO_POSTIONS[5]);
	bg->addChild(contentBg, BASIC_ZORDER_ROOT+1);
	//基本资料背景阴影
	auto basicBg = Sprite::createWithSpriteFrameName(userInfoBasicBg);
	basicBg->setPosition(USER_INFO_POSTIONS[9]);
	bg->addChild(basicBg, BASIC_ZORDER_ROOT+2);
	//头像框背景
	auto headBg = WWTouchSprite::createWithFile(userInfoHeadImageBg,this,callfuncO_selector(UserInfoLayer::onTouchSpriteDepress));
	headBg->setTag(USER_INFO_HEAD_IMAGE_MOD2);
	headBg->setPosition(USER_INFO_POSTIONS[1]);
	bg->addChild(headBg, BASIC_ZORDER_ROOT+3);
	//vip头像标识
	auto vipHead = Sprite::create(vip_comm);
	vipHead->setPosition(USER_INFO_POSTIONS[3]);
	bg->addChild(vipHead, BASIC_ZORDER_ROOT+2);
   
	//玩家头像
	auto headImage = Sprite::create(headTempImage);
	headImage->setScale(1.4f);
	headImage->setPosition(Vec2(0,0));
	headBg->addChild(headImage,BASIC_ZORDER_ROOT+3);
	//编辑头像按钮
	auto changeHeadBtn = WWTouchSprite::createWithSpriteFrameName(userInfoHeadImagModfly,this,callfuncO_selector(UserInfoLayer::onTouchSpriteDepress));
	changeHeadBtn->setTag(USER_INFO_HEAD_IMAGE_MOD);
	changeHeadBtn->setPosition(USER_INFO_POSTIONS[2]);
	bg->addChild(changeHeadBtn, BASIC_ZORDER_ROOT+4);

	//帮助标签 问好
	auto helpBtn = WWTouchSprite::createWithSpriteFrameName(userInfoHelp,this,callfuncO_selector(UserInfoLayer::onTouchSpriteDepress));
	helpBtn->setPosition(USER_INFO_POSTIONS[17]);
	helpBtn->setTag(USER_INFO_HELP_BTN);
	bg->addChild(helpBtn,BASIC_ZORDER_FRAME);
	//************************************基本资料**********************************************************//
	//昵称
	auto nameBg = ImageView::create(userinfoNameBg);
	nameBg->setContentSize(Size(370, 55));
	nameBg->setScale9Enabled(true);
	nameBg->setPosition(USER_INFO_POSTIONS[21]);
	bg->addChild(nameBg,BASIC_ZORDER_FRAME);

	std::string nickName = WWDataManager::getInstance()->getUserBasicInfoData()->getNickName();
	//log("nick name = %s",nickName.c_str());
	auto nameLabel = Label::createWithTTF("nick name 8th","res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	nameLabel->setAnchorPoint(Vec2(0,0.5));
	nameLabel->setPosition(USER_INFO_POSTIONS[22]);
	bg->addChild(nameLabel,BASIC_ZORDER_FRAME+1);
	nameLabel->setString(nickName);

	auto nameChangeBt = WWTouchSprite::createWithSpriteFrameName(userInfoNameModefly,this,callfuncO_selector(UserInfoLayer::onTouchSpriteDepress));
	nameChangeBt->setPosition(USER_INFO_POSTIONS[4]);
	nameChangeBt->setTag(USER_INFO_NICK_NAME_MOD);
	bg->addChild(nameChangeBt,BASIC_ZORDER_FRAME+1);
	
	//账号
	std::string id = StringTool::intToString(WWDataManager::getInstance()->getUserBasicInfoData()->getUserId());
	std::string idTxt = StringUtils::format(GetWWString(u_info_id, userInfoSection).c_str(), id.c_str());
	auto idLabel = Label::createWithTTF(idTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	idLabel->setColor(GET_RGBCOLOR(txt_color));
	idLabel->setAnchorPoint(Vec2(0,0.5));
	idLabel->setPosition(USER_INFO_POSTIONS[15]);
	bg->addChild(idLabel, BASIC_ZORDER_FRAME);
	//性别
	int gender = WWDataManager::getInstance()->getUserBasicInfoData()->getGender();
	/*std::string genderTxt;
	if (gender == 1)
		genderTxt = GetWWString(u_male,userInfoSection);
	else
		genderTxt = GetWWString(u_female,userInfoSection);
	auto genderLabel = Label::createWithTTF(genderTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	genderLabel->setColor(GET_RGBCOLOR(txt_color));
	genderLabel->setAnchorPoint(Vec2(0,0.5));
	genderLabel->setPosition(Vec2(nameChangeBt->getPositionX() - genderLabel->getContentSize().width,idLabel->getPositionY()));
	bg->addChild(genderLabel, BASIC_ZORDER_FRAME);*/
	auto maleIcon = Sprite::createWithSpriteFrameName("user_male_icon.png");
	maleIcon->setPosition(Vec2(idLabel->getPositionX() + 30,idLabel->getPositionY() - 68));
	bg->addChild(maleIcon,BASIC_ZORDER_FRAME);
	m_pMaleCheckBox = CheckBox::create(userGenderUnselect,userGenderUnselect,userGenderSelect,"","");
	m_pMaleCheckBox->setPosition(Vec2(maleIcon->getPositionX() + maleIcon->getContentSize().width + 10,maleIcon->getPositionY()));
	m_pMaleCheckBox->addEventListener(CC_CALLBACK_2(UserInfoLayer::maleCallback,this));
	bg->addChild(m_pMaleCheckBox,BASIC_ZORDER_FRAME);
	auto femaleIcon = Sprite::createWithSpriteFrameName("user_female_icon.png");
	femaleIcon->setPosition(Vec2(m_pMaleCheckBox->getPositionX() + 140,m_pMaleCheckBox->getPositionY()));
	bg->addChild(femaleIcon,BASIC_ZORDER_FRAME);
	m_pFemaleCheckBox = CheckBox::create(userGenderUnselect,userGenderUnselect,userGenderSelect,"","");
	m_pFemaleCheckBox->addEventListener(CC_CALLBACK_2(UserInfoLayer::femaleCallback,this));
	m_pFemaleCheckBox->setPosition(Vec2(femaleIcon->getPositionX()+m_pFemaleCheckBox->getContentSize().width + 10,femaleIcon->getPositionY()));
	bg->addChild(m_pFemaleCheckBox,BASIC_ZORDER_FRAME);
	if (gender == 1)
	{
		m_pMaleCheckBox->setSelected(true);
		m_pFemaleCheckBox->setSelected(false);
	}
	else
	{
		m_pMaleCheckBox->setSelected(false);
		m_pFemaleCheckBox->setSelected(true);
	}
	//横线1
	auto lineId = Sprite::createWithSpriteFrameName(userInfoLine);
	lineId->setPosition(USER_INFO_POSTIONS[16]);
	bg->addChild(lineId, BASIC_ZORDER_FRAME);
	//等级
	auto lvLabel = Label::createWithTTF(GetWWString(u_info_lv,userInfoSection),"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	lvLabel->setColor(GET_RGBCOLOR(txt_color));
	lvLabel->setAnchorPoint(Vec2(0,0.5));
	lvLabel->setPosition(Vec2(idLabel->getPositionX(),idLabel->getPositionY()-136));
	bg->addChild(lvLabel,BASIC_ZORDER_FRAME);

	auto lvSeek = Slider::create();
	lvSeek->loadBarTexture(userinfoNameBg);
	lvSeek->loadProgressBarTexture(userInfoLvSeekPro);
	lvSeek->setPosition(USER_INFO_POSTIONS[19] + Vec2(0,-68));
	lvSeek->setPercent(29);
	lvSeek->setScale9Enabled(true);
    lvSeek->setCapInsets(Rect(0, 0, 0, 0));
    lvSeek->setSize(Size(250.0, 54));
	bg->addChild(lvSeek,BASIC_ZORDER_FRAME);

	std::string lvTxt = StringUtils::format("LV%d", WWDataManager::getInstance()->getUserBasicInfoData()->getLevel());
	auto lvLabelUser = Label::createWithTTF(lvTxt,"res/Fonts/FounderSemiBold.ttf",30,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	lvLabelUser->setPosition(USER_INFO_POSTIONS[19] + Vec2(0,-68));
	bg->addChild(lvLabelUser,BASIC_ZORDER_FRAME+1);
	//横线2
	auto lineLv = Sprite::createWithSpriteFrameName(userInfoLine);
	lineLv->setPosition(Vec2(lineId->getPositionX(),lineId->getPositionY()-68));
	bg->addChild(lineLv,BASIC_ZORDER_FRAME);
	//蛙龄
	std::string waYearTxt = StringUtils::format(GetWWString(u_info_waling,userInfoSection).c_str(),WWDataManager::getInstance()->getUserBasicInfoData()->getRegisterTime().c_str());
	auto waLingLabel = Label::createWithTTF(waYearTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	waLingLabel->setColor(GET_RGBCOLOR(txt_color));
	waLingLabel->setAnchorPoint(Vec2(0,0.5));
	waLingLabel->setPosition(Vec2(lvLabel->getPositionX(),lvLabel->getPositionY()-68));
	bg->addChild(waLingLabel,BASIC_ZORDER_FRAME);
	//横线3
	auto linewl = Sprite::createWithSpriteFrameName(userInfoLine);
	linewl->setPosition(Vec2(lineLv->getPositionX(),lineLv->getPositionY()-68));
	bg->addChild(linewl,BASIC_ZORDER_FRAME);
	//距离上次登入
	std::string lastTimeTxt = StringUtils::format(GetWWString(u_info_last_online,userInfoSection).c_str(),WWDataManager::getInstance()->getUserBasicInfoData()->getLastTime().c_str());
	auto lastTimeLabel = Label::createWithTTF(lastTimeTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	lastTimeLabel->setColor(GET_RGBCOLOR(txt_color));
	lastTimeLabel->setAnchorPoint(Vec2(0,0.5));
	lastTimeLabel->setPosition(Vec2(waLingLabel->getPositionX(),waLingLabel->getPositionY()-68));
	bg->addChild(lastTimeLabel,BASIC_ZORDER_FRAME);
	//横线4
	auto lineLin = Sprite::createWithSpriteFrameName(userInfoLine);
	lineLin->setPosition(Vec2(linewl->getPositionX(),linewl->getPositionY()-68));
	bg->addChild(lineLin,BASIC_ZORDER_FRAME);
	//横线5
	auto line5 = Sprite::createWithSpriteFrameName(userInfoLine);
	line5->setPosition(Vec2(lineLin->getPositionX(),lineLin->getPositionY()-68));
	bg->addChild(line5,BASIC_ZORDER_FRAME);
	//所在地
	std::string adressTxt = StringUtils::format(GetWWString(u_info_address,userInfoSection).c_str(),WWDataManager::getInstance()->getUserBasicInfoData()->getRegion().c_str());
	auto adressLabel = Label::createWithTTF(adressTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	adressLabel->setColor(GET_RGBCOLOR(txt_color));
	adressLabel->setAnchorPoint(Vec2(0,0.5));
	adressLabel->setPosition(Vec2(lastTimeLabel->getPositionX(),lastTimeLabel->getPositionY()-68));
	bg->addChild(adressLabel,BASIC_ZORDER_FRAME);

	//************************************右边内容*************************************************//
	//内容背景
	auto contBgImage = Sprite::createWithSpriteFrameName(userInfoInfoBg);
	contBgImage->setPosition(USER_INFO_POSTIONS[10]);
	bg->addChild(contBgImage,BASIC_ZORDER_ROOT+4);
	//切换按钮背景
	auto toggleBg = Sprite::createWithSpriteFrameName(userInfoToggBg);
	toggleBg->setPosition(USER_INFO_POSTIONS[11]);
	bg->addChild(toggleBg, BASIC_ZORDER_ROOT+5);
	//欢乐斗地主按钮
	auto happyBtn = WWTouchSprite::createWithSpriteFrameName(userInfoToggBtnSelect,this,callfuncO_selector(UserInfoLayer::onTouchSpriteDepress));
	happyBtn->setPosition(USER_INFO_POSTIONS[12]);
	happyBtn->setTag(USER_INFO_HAPPY_BTN);
	bg->addChild(happyBtn, BASIC_ZORDER_FRAME);
	m_pHuanleBtn = Sprite::createWithSpriteFrameName(userInfoToggBtnUnselect);
	m_pHuanleBtn->setPosition(Vec2(0,0));
	happyBtn->addChild(m_pHuanleBtn,BASIC_ZORDER_ROOT);
	m_pHuanleBtn->setVisible(false);
	m_pHuanleTxt = Label::createWithTTF(GetWWString(u_info_happy_txt,userInfoSection),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	m_pHuanleTxt->setPosition(Vec2(0,0));
	m_pHuanleTxt->setColor(GET_RGBCOLOR(togg_txt_color));
	happyBtn->addChild(m_pHuanleTxt,BASIC_ZORDER_ROOT+1);
	//癞子斗地主按钮
	auto laiziBtn = WWTouchSprite::createWithSpriteFrameName(userInfoToggBtnSelect,this,callfuncO_selector(UserInfoLayer::onTouchSpriteDepress));
	laiziBtn->setPosition(USER_INFO_POSTIONS[13]);
	laiziBtn->setTag(USER_INFO_LAIZI_BTN);
	bg->addChild(laiziBtn,BASIC_ZORDER_FRAME);
	m_pLaiziBtn = Sprite::createWithSpriteFrameName(userInfoToggBtnUnselect);
	m_pLaiziBtn->setPosition(Vec2(0,0));
	laiziBtn->addChild(m_pLaiziBtn,BASIC_ZORDER_ROOT);
	m_pLaiziTxt = Label::createWithTTF(GetWWString(u_info_laizi_txt,userInfoSection),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	m_pLaiziTxt->setPosition(Vec2(0,0));
	m_pLaiziTxt->setColor(GET_RGBCOLOR(txt_color));
	laiziBtn->addChild(m_pLaiziTxt,BASIC_ZORDER_ROOT+1);
	//比赛
	auto bisaiBtn = WWTouchSprite::createWithSpriteFrameName(userInfoToggBtnSelect,this,callfuncO_selector(UserInfoLayer::onTouchSpriteDepress));
	bisaiBtn->setPosition(USER_INFO_POSTIONS[14]);
	bisaiBtn->setTag(USER_INFO_BISAI_BTN);
	bg->addChild(bisaiBtn,BASIC_ZORDER_FRAME);
	m_pBisaiBtn = Sprite::createWithSpriteFrameName(userInfoToggBtnUnselect);
	m_pBisaiBtn->setPosition(Vec2(0,0));
	bisaiBtn->addChild(m_pBisaiBtn,BASIC_ZORDER_ROOT);
	m_pBisaiTxt = Label::createWithTTF(GetWWString(u_info_bisai,userInfoSection),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	m_pBisaiTxt->setPosition(Vec2(0,0));
	m_pBisaiTxt->setColor(GET_RGBCOLOR(txt_color));
	bisaiBtn->addChild(m_pBisaiTxt,BASIC_ZORDER_ROOT+1);

	//************************************底部按钮********************************************//
	//注销账号
	auto loginOut = WWTouchSprite::createWithFile(btnYellow,this,callfuncO_selector(UserInfoLayer::onTouchSpriteDepress));
	loginOut->setPosition(USER_INFO_POSTIONS[6]);
	loginOut->setTag(USER_INFO_LOGIN_OUT);
	bg->addChild(loginOut,BASIC_ZORDER_FRAME);
	auto loginOutTxt = Sprite::createWithSpriteFrameName(userInfoLoginOutTxt);
	loginOutTxt->setPosition(Vec2(0,0));
	loginOut->addChild(loginOutTxt,BASIC_ZORDER_ROOT);
	//绑定账号
	auto bindUser = WWTouchSprite::createWithFile(btnBlue,this,callfuncO_selector(UserInfoLayer::onTouchSpriteDepress));
	bindUser->setPosition(USER_INFO_POSTIONS[7]);
	bindUser->setTag(USER_INFO_BIND);
	bg->addChild(bindUser,BASIC_ZORDER_FRAME);
	auto bindTxt = Sprite::createWithSpriteFrameName("user_bind_txt.png");
	bindTxt->setPosition(Vec2(0,0));
	bindUser->addChild(bindTxt,BASIC_ZORDER_ROOT);

	//修改密码
	auto changePwd = WWTouchSprite::createWithFile(btnBlue,this,callfuncO_selector(UserInfoLayer::onTouchSpriteDepress));
	changePwd->setPosition(USER_INFO_POSTIONS[8]);
	changePwd->setTag(USER_INFO_CHANGE_PWD);
	bg->addChild(changePwd,BASIC_ZORDER_FRAME);
	auto changePwdTxt = Sprite::createWithSpriteFrameName(userInfoPwdModfly);
	changePwdTxt->setPosition(Vec2(0,0));
	changePwd->addChild(changePwdTxt,BASIC_ZORDER_ROOT);


	//切换不同玩法的信息
	reflashHappyNodes();
}

void UserInfoLayer::onEnter()
{
	LayerColor::onEnter();
	//禁止触摸穿透
	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(UserInfoLayer::onTouchBegan, this);
	m_pListner->onTouchEnded = CC_CALLBACK_2(UserInfoLayer::onTouchEnded,this);
	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(userinfoPlist);
}

void UserInfoLayer::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	LayerColor::onExit();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(userinfoPlist);
}



bool UserInfoLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	WWFrameBase::onTouchBegan(pTouch, pEvent);
	return true;
}

void UserInfoLayer::onTouchMoved(Touch* pTouch, Event* pEvent)
{
}

void UserInfoLayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	WWFrameBase::onTouchEnded(pTouch, pEvent);
}

void UserInfoLayer::onTouchCancelled(Touch* pTouch, Event* pEvent)
{
}

//Menu Button 点击事件
void UserInfoLayer::ButtonDepress(Ref * theBtn)
{
	MenuItem* pItem = (MenuItem*)theBtn;
	Menu* pMenu = (Menu*)pItem->getUserData();
	int pTag = pMenu->getTag();
	switch (pTag)
	{
		case USER_INFO_HEAD_IMAGE_MOD:
			log("change head image btn");
			break;
	default:
		break;
	}
}

void UserInfoLayer::onLoginOut()
{
	MsgLogonRequest::sendLogonOut();
	WWNetWorkManager::getInstance()->getCurSocket()->CloseSocket();
	GameGlobalDataManager::getInstance()->setLoginFromType(LoginType::LOGIN_FROM_LOBBY);

	auto pScene = LoginScene::create();
	Director::getInstance()->replaceScene(pScene);
}

void UserInfoLayer::onTouchSpriteDepress(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int pTag = btn->getTag();
	switch (pTag)
	{
	case USER_INFO_LOGIN_OUT:
		log("login out btn");
		onLoginOut();
		break;

	case USER_INFO_BIND:
		log("bind user btn");
		break;

	case USER_INFO_CHANGE_PWD:
		log("change pwd btn");
		break;

	case USER_INFO_HAPPY_BTN:
		log("happy btn");
		m_pHuanleBtn->setVisible(false);
		m_pLaiziBtn->setVisible(true);
		m_pBisaiBtn->setVisible(true);
		m_pHuanleTxt->setColor(GET_RGBCOLOR(togg_txt_color));
		m_pLaiziTxt->setColor(GET_RGBCOLOR(txt_color));
		m_pBisaiTxt->setColor(GET_RGBCOLOR(txt_color));
		reflashHappyNodes();
		break;
	case USER_INFO_LAIZI_BTN:
		log("laizi btn");
		m_pHuanleBtn->setVisible(true);
		m_pLaiziBtn->setVisible(false);
		m_pBisaiBtn->setVisible(true);
		m_pHuanleTxt->setColor(GET_RGBCOLOR(txt_color));
		m_pLaiziTxt->setColor(GET_RGBCOLOR(togg_txt_color));
		m_pBisaiTxt->setColor(GET_RGBCOLOR(txt_color));
		reflashLaiziNodes();
		break;
	case USER_INFO_BISAI_BTN:
		log("bisai btn");
		m_pHuanleBtn->setVisible(true);
		m_pLaiziBtn->setVisible(true);
		m_pBisaiBtn->setVisible(false);
		m_pHuanleTxt->setColor(GET_RGBCOLOR(txt_color));
		m_pLaiziTxt->setColor(GET_RGBCOLOR(txt_color));
		m_pBisaiTxt->setColor(GET_RGBCOLOR(togg_txt_color));
		break;
	case USER_INFO_HELP_BTN:
		log("help btn");
		break;
	case USER_INFO_NICK_NAME_MOD:
		log("nick name md btn");
		break;
	case USER_INFO_HEAD_IMAGE_MOD2:
		log("head image md btn");
		break;
	default:
		break;
	}
}

void UserInfoLayer::reflashHappyNodes()
{
	removeNodes();
	//胜率
	auto winPerBg = Sprite::createWithSpriteFrameName(userInfoWinPerBg);
	winPerBg->setPosition(USER_INFO_POSTIONS[18]);
	bg->addChild(winPerBg,BASIC_ZORDER_TOP);
	m_happyNodes.push_back(winPerBg);
	std::string winPerTxt = StringUtils::format(GetWWString(u_info_win_precent,userInfoSection).c_str(),52);
	auto winPerLabel = Label::createWithTTF(winPerTxt,"res/Fonts/FounderSemiBold.ttf",32,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	winPerLabel->setColor(GET_RGBCOLOR(txt_color));
	winPerLabel->setPosition(USER_INFO_POSTIONS[26]);
	bg->addChild(winPerLabel,BASIC_ZORDER_TOP+1);
	m_happyNodes.push_back(winPerLabel);
	std::string winCountTxt = StringUtils::format(GetWWString(u_info_jus,userInfoSection).c_str(),520,1000);
	auto winCountLabel = Label::create(winCountTxt,"res/Fonts/FounderSemiBold.ttf",32,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	winCountLabel->setColor(GET_RGBCOLOR(txt_color));
	winCountLabel->setPosition(USER_INFO_POSTIONS[27]);
	bg->addChild(winCountLabel,BASIC_ZORDER_TOP+1);
	m_happyNodes.push_back(winCountLabel);

	//当前积分条
	auto jifenSeek = Slider::create();
	jifenSeek->loadBarTexture(userInfoLvSeekBg);
	jifenSeek->loadProgressBarTexture(userInfoJifenPro);
	jifenSeek->setPosition(USER_INFO_POSTIONS[20]);
	jifenSeek->setPercent(59);
	jifenSeek->setScale9Enabled(true);
    jifenSeek->setCapInsets(Rect(0, 0, 0, 0));
    jifenSeek->setSize(Size(900.0, 54));
	bg->addChild(jifenSeek,BASIC_ZORDER_TOP);
	m_happyNodes.push_back(jifenSeek);

	auto jifenRich = RichText::create();
	jifenRich->ignoreContentAdaptWithSize(true);
	auto nowTxt = RichElementText::create(1, GET_RGBCOLOR(jifen_now_txt_color),255, GetWWString(u_info_jifen,userInfoSection),"res/Fonts/FounderSemiBold.ttf",28);
	auto nowNum = RichElementText::create(1, GET_RGBCOLOR(jifen_now_num_color),255, "1000","res/Fonts/FounderSemiBold.ttf",30);
	jifenRich->setContentSize(Size(180,35));
	jifenRich->pushBackElement(nowTxt);
	jifenRich->pushBackElement(nowNum);
	jifenRich->setPosition(USER_INFO_POSTIONS[20]);
	bg->addChild(jifenRich,BASIC_ZORDER_TOP+1);
	m_happyNodes.push_back(jifenRich);

	//小土豪牌
	auto smallTu = Sprite::createWithSpriteFrameName(point_norm);
	smallTu->setPosition(USER_INFO_POSTIONS[23]);
	bg->addChild(smallTu,BASIC_ZORDER_TOP);
	m_happyNodes.push_back(smallTu);
	auto smallLabel = Label::createWithTTF(GetWWString(u_small_tuhao,userInfoSection).c_str(),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	smallLabel->setColor(GET_RGBCOLOR(lv_nick_color));
	smallLabel->setPosition(Vec2(smallTu->getContentSize().width*0.5f,smallTu->getContentSize().height*0.58f));
	smallTu->addChild(smallLabel);
	m_happyNodes.push_back(smallLabel);
	auto smallJifen = Label::createWithTTF(StringUtils::format("%d",100),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	smallJifen->setColor(GET_RGBCOLOR(lv_jifen_color));
	smallJifen->setPosition(Vec2(smallTu->getContentSize().width*0.5f,smallTu->getContentSize().height*0.4f));
	smallTu->addChild(smallJifen);
	m_happyNodes.push_back(smallJifen);

	//大土豪牌
	auto bigTu = Sprite::createWithSpriteFrameName(point_norm);
	bigTu->setPosition(USER_INFO_POSTIONS[24]);
	bg->addChild(bigTu,BASIC_ZORDER_TOP);
	m_happyNodes.push_back(bigTu);
	auto bigTuLabel = Label::createWithTTF(GetWWString(u_big_tuhao,userInfoSection).c_str(),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	bigTuLabel->setColor(GET_RGBCOLOR(lv_nick_color));
	bigTuLabel->setPosition(Vec2(bigTu->getContentSize().width*0.5f,bigTu->getContentSize().height*0.58f));
	bigTu->addChild(bigTuLabel);
	m_happyNodes.push_back(bigTuLabel);
	auto bigJifen = Label::createWithTTF(StringUtils::format("%d",999),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	bigJifen->setColor(GET_RGBCOLOR(lv_jifen_color));
	bigJifen->setPosition(Vec2(bigTu->getContentSize().width*0.5f,bigTu->getContentSize().height*0.4f));
	bigTu->addChild(bigJifen);
	m_happyNodes.push_back(bigJifen);

	//超级大土豪牌
	auto superTu = Sprite::createWithSpriteFrameName(point_gay);
	superTu->setPosition(USER_INFO_POSTIONS[25]);
	bg->addChild(superTu,BASIC_ZORDER_TOP);
	m_happyNodes.push_back(superTu);
	auto superTuLabel = Label::createWithTTF(GetWWString(u_super_tuhao,userInfoSection).c_str(),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	superTuLabel->setColor(GET_RGBCOLOR(lv_nick_gay_color));
	superTuLabel->setDimensions(120.0f,120.0f);
	superTuLabel->setPosition(Vec2(superTu->getContentSize().width*0.5f,superTu->getContentSize().height*0.55f));
	superTu->addChild(superTuLabel);
	m_happyNodes.push_back(superTuLabel);
	auto superJifen = Label::createWithTTF(StringUtils::format("%d",1000),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	superJifen->setColor(GET_RGBCOLOR(lv_jifen_gay_color));
	superJifen->setPosition(Vec2(superTu->getContentSize().width*0.5f,superTu->getContentSize().height*0.32f));
	superTu->addChild(superJifen);
	m_happyNodes.push_back(superJifen);
}

void UserInfoLayer::reflashLaiziNodes()
{
	removeNodes();
	//胜率
	auto winPerBg = Sprite::createWithSpriteFrameName(userInfoWinPerBg);
	winPerBg->setPosition(USER_INFO_POSTIONS[18]);
	bg->addChild(winPerBg,BASIC_ZORDER_TOP);
	m_laiziNodes.push_back(winPerBg);
	std::string winPerTxt = StringUtils::format(GetWWString(u_info_win_precent,userInfoSection).c_str(),46);
	auto winPerLabel = Label::createWithTTF(winPerTxt,"res/Fonts/FounderSemiBold.ttf",32,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	winPerLabel->setColor(GET_RGBCOLOR(txt_color));
	winPerLabel->setPosition(USER_INFO_POSTIONS[26]);
	bg->addChild(winPerLabel,BASIC_ZORDER_TOP+1);
	m_laiziNodes.push_back(winPerLabel);
	std::string winCountTxt = StringUtils::format(GetWWString(u_info_jus,userInfoSection).c_str(),432,970);
	auto winCountLabel = Label::create(winCountTxt,"res/Fonts/FounderSemiBold.ttf",32,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	winCountLabel->setColor(GET_RGBCOLOR(txt_color));
	winCountLabel->setPosition(USER_INFO_POSTIONS[27]);
	bg->addChild(winCountLabel,BASIC_ZORDER_TOP+1);
	m_laiziNodes.push_back(winCountLabel);

	auto jifenSeek = Slider::create();
	jifenSeek->loadBarTexture(userInfoLvSeekBg);
	jifenSeek->loadProgressBarTexture(userInfoJifenPro);
	jifenSeek->setPosition(USER_INFO_POSTIONS[20]);
	jifenSeek->setPercent(39);
	jifenSeek->setScale9Enabled(true);
    jifenSeek->setCapInsets(Rect(0, 0, 0, 0));
    jifenSeek->setSize(Size(900.0, 54));
	bg->addChild(jifenSeek,BASIC_ZORDER_TOP);
	m_laiziNodes.push_back(jifenSeek);

	auto jifenRich = RichText::create();
	jifenRich->ignoreContentAdaptWithSize(true);
	auto nowTxt = RichElementText::create(1, GET_RGBCOLOR(jifen_now_txt_color),255, GetWWString(u_info_jifen,userInfoSection),"res/Fonts/FounderSemiBold.ttf",28);
	auto nowNum = RichElementText::create(1, GET_RGBCOLOR(jifen_now_num_color),255, "300","res/Fonts/FounderSemiBold.ttf",30);
	jifenRich->setContentSize(Size(180,35));
	jifenRich->pushBackElement(nowTxt);
	jifenRich->pushBackElement(nowNum);
	jifenRich->setPosition(USER_INFO_POSTIONS[20]);
	bg->addChild(jifenRich,BASIC_ZORDER_TOP+1);
	m_laiziNodes.push_back(jifenRich);
	//小土豪牌
	auto smallTu = Sprite::createWithSpriteFrameName(point_norm);
	smallTu->setPosition(USER_INFO_POSTIONS[23]);
	bg->addChild(smallTu,BASIC_ZORDER_TOP);
	m_laiziNodes.push_back(smallTu);
	auto smallLabel = Label::createWithTTF(GetWWString(u_small_tuhao,userInfoSection).c_str(),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	smallLabel->setColor(GET_RGBCOLOR(lv_nick_color));
	smallLabel->setPosition(Vec2(smallTu->getContentSize().width*0.5f,smallTu->getContentSize().height*0.58f));
	smallTu->addChild(smallLabel);
	m_laiziNodes.push_back(smallLabel);
	auto smallJifen = Label::createWithTTF(StringUtils::format("%d",100),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	smallJifen->setColor(GET_RGBCOLOR(lv_jifen_color));
	smallJifen->setPosition(Vec2(smallTu->getContentSize().width*0.5f,smallTu->getContentSize().height*0.4f));
	smallTu->addChild(smallJifen);
	m_laiziNodes.push_back(smallJifen);

	//大土豪牌
	auto bigTu = Sprite::createWithSpriteFrameName(point_gay);
	bigTu->setPosition(USER_INFO_POSTIONS[24]);
	bg->addChild(bigTu,BASIC_ZORDER_TOP);
	m_laiziNodes.push_back(bigTu);
	auto bigTuLabel = Label::createWithTTF(GetWWString(u_big_tuhao,userInfoSection).c_str(),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	bigTuLabel->setColor(GET_RGBCOLOR(lv_nick_gay_color));
	bigTuLabel->setPosition(Vec2(bigTu->getContentSize().width*0.5f,bigTu->getContentSize().height*0.58f));
	bigTu->addChild(bigTuLabel);
	m_laiziNodes.push_back(bigTuLabel);
	auto bigJifen = Label::createWithTTF(StringUtils::format("%d",999),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	bigJifen->setColor(GET_RGBCOLOR(lv_jifen_gay_color));
	bigJifen->setPosition(Vec2(bigTu->getContentSize().width*0.5f,bigTu->getContentSize().height*0.4f));
	bigTu->addChild(bigJifen);
	m_laiziNodes.push_back(bigJifen);

	//超级大土豪牌
	auto superTu = Sprite::createWithSpriteFrameName(point_gay);
	superTu->setPosition(USER_INFO_POSTIONS[25]);
	bg->addChild(superTu,BASIC_ZORDER_TOP);
	m_laiziNodes.push_back(superTu);
	auto superTuLabel = Label::createWithTTF(GetWWString(u_super_tuhao,userInfoSection).c_str(),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	superTuLabel->setColor(GET_RGBCOLOR(lv_nick_gay_color));
	superTuLabel->setDimensions(120.0f,120.0f);
	superTuLabel->setPosition(Vec2(superTu->getContentSize().width*0.5f,superTu->getContentSize().height*0.55f));
	superTu->addChild(superTuLabel);
	m_laiziNodes.push_back(superTuLabel);
	auto superJifen = Label::createWithTTF(StringUtils::format("%d",1000),"res/Fonts/FounderSemiBold.ttf",34,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	superJifen->setColor(GET_RGBCOLOR(lv_jifen_gay_color));
	superJifen->setPosition(Vec2(superTu->getContentSize().width*0.5f,superTu->getContentSize().height*0.32f));
	superTu->addChild(superJifen);
	m_laiziNodes.push_back(superJifen);

}


void UserInfoLayer::reflashBisaiNodes()
{

}

void UserInfoLayer::removeNodes()
{
	for (const auto& node : this->m_happyNodes)
	{
		bg->removeChild(node,true);
	}
	m_happyNodes.clear();
	for (const auto& node : this->m_laiziNodes)
	{
		bg->removeChild(node,true);
	}
	m_laiziNodes.clear();
}

void UserInfoLayer::maleCallback(Ref* sender, CheckBox::EventType controlEvent)
{
	switch (controlEvent)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("male open");
		m_pMaleCheckBox->setSelected(true);
		m_pFemaleCheckBox->setSelected(false);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("female close");		
		m_pMaleCheckBox->setSelected(false);
		m_pFemaleCheckBox->setSelected(true);
		break;
	default:
		break;
	}
}

void UserInfoLayer::femaleCallback(Ref* sender, CheckBox::EventType controlEvent)
{
	switch (controlEvent)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("female open");		
		m_pMaleCheckBox->setSelected(false);
		m_pFemaleCheckBox->setSelected(true);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("male close");
		m_pMaleCheckBox->setSelected(true);
		m_pFemaleCheckBox->setSelected(false);
		break;
	default:
		break;
	}
}
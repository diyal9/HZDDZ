#include "CommDialog.h"
#include "CommDialogRes.h"
#include "../../../BaseCore/Common/UICoordConfig.h"
#include "../../../BaseCore/WWBGUI/WWTouchSprite.h"
#include "../../../WWMacros.h"


CommDialog::CommDialog(void):
	m_pBg(nullptr)
{
	auto m_pListner  = EventListenerTouchOneByOne::create();
	m_pListner->onTouchBegan = CC_CALLBACK_2(CommDialog::onTouchBegan, this);
	m_pListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);
}

CommDialog::~CommDialog()
{

}

CommDialog* CommDialog::creatOneBtnOneMessage(const char* btnTxt,const char* messageTxt)
{
	CommDialog* pItem = new CommDialog();
	if (pItem && pItem->initOneBtnOneMessage(btnTxt,messageTxt))
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

CommDialog* CommDialog::createOneBtn(const char* btnTxt, SEL_CallFuncO callback)
{
	CommDialog* pItem = new CommDialog();
	if(pItem && pItem->initOneBtn(btnTxt,callback))
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

CommDialog* CommDialog::createTwoBtn(const char* leftTxt,const char* rightTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback)
{
	CommDialog* pItem = new CommDialog();
	if(pItem && pItem->initTwoBtn(leftTxt,rightTxt,leftCallback,rightCallback))
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

CommDialog* CommDialog::creatTwoBtnOneMessage(const char* leftTxt,const char* rightTxt,const char* messageTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback)
{
	CommDialog* pItem = new CommDialog();
	if(pItem && pItem->initTwoBtnOneMessage(leftTxt,rightTxt,messageTxt,leftCallback,rightCallback))
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

bool CommDialog::initOneBtnOneMessage(const char* btnTxt,const char* messageTxt)
{
	if (!LayerColor::init())
	{
		return false;
	}
	initViewOneBtnOneMessage(btnTxt,messageTxt);
	return true;
}


bool CommDialog::initOneBtn(const char* btnTxt, SEL_CallFuncO callback)
{
	if(!LayerColor::init())
	{
		return false;
	}
	initViewOneBtn(btnTxt,callback);

	return true;
}

bool CommDialog::initTwoBtn(const char* leftTxt,const char* rightTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback)
{
	if(!LayerColor::init())
	{
		return false;
	}
	initViewTwoBtn(leftTxt,rightTxt,leftCallback,rightCallback);

	return true;
}

bool CommDialog::initTwoBtnOneMessage(const char* leftTxt,const char* rightTxt,const char* messageTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback)
{
	if (!LayerColor::init())
	{
		return false;
	}
	initViewTwoBtnOneMessage(leftTxt,rightTxt,messageTxt,leftCallback,rightCallback);
	return true;
}

void CommDialog::onEnter()
{

	LayerColor::onEnter();
	log("CommDialog::onEnter");
}

void CommDialog::onExit()
{
	LayerColor::onExit();

	log("CommDialog::onExit");
}

void CommDialog::initViewOneBtnOneMessage(const char* btnTxt,const char* messageTxt)
{
	//添加背景
	m_pBg = Sprite::create(contentBgImage);
	m_pBg->setPosition(CUR_SCREEN_SIZE.width*0.5,CUR_SCREEN_SIZE.height*0.5);
	this->addChild(m_pBg, BASIC_ZORDER_ROOT);

	//添加关闭按钮
	/*auto closeBtn = WWTouchSprite::createWithFile(closeBtnImage,this, callfuncO_selector(CommDialog::close));
	closeBtn->setPosition(Vec2(1116,731));
	m_pBg->addChild(closeBtn,BASIC_ZORDER_ROOT+3);*/
	//提示信息文字
	auto messageLabel = Label::createWithTTF(messageTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	messageLabel->setDimensions(900.0f,500.0f);
	messageLabel->setPosition(Vec2(m_pBg->getContentSize().width*0.5f,m_pBg->getContentSize().height*0.7f));
	messageLabel->setColor(GET_RGBCOLOR(txt_color));
	m_pBg->addChild(messageLabel,BASIC_ZORDER_FRAME);

	//中间按钮
	auto middBtn = WWTouchSprite::createWithFile(btn_yellow,this, callfuncO_selector(CommDialog::close));
	middBtn->setPosition(Vec2(558, 120));
	m_pBg->addChild(middBtn,BASIC_ZORDER_ROOT+3);
	//中间按钮字
	auto midLabel = Label::createWithTTF(btnTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	midLabel->setColor(GET_RGBCOLOR(txt_color));
	midLabel->setPosition(Vec2(0,0));
	middBtn->addChild(midLabel);
	//特效
	m_pBg->setScale(0.9f);
	EaseBackOut* _easeIn = EaseBackOut::create(ScaleTo::create(0.2f,1.0f));
	Action* _action = Sequence::create(_easeIn,nullptr);
	m_pBg->runAction(_action);
}

void CommDialog::initViewOneBtn(const char* btnTxt, SEL_CallFuncO callback)
{
	//添加背景
	m_pBg = Sprite::create(contentBgImage);
	m_pBg->setPosition(CUR_SCREEN_SIZE.width*0.5,CUR_SCREEN_SIZE.height*0.5);
	this->addChild(m_pBg, BASIC_ZORDER_ROOT);

	//添加关闭按钮
	auto closeBtn = WWTouchSprite::createWithFile(closeBtnImage,this, callfuncO_selector(CommDialog::close));
	closeBtn->setPosition(Vec2(1116,731));
	m_pBg->addChild(closeBtn,BASIC_ZORDER_ROOT+3);

	//中间按钮
	SEL_CallFuncO callBack = (callback == NULL)?callfuncO_selector(CommDialog::close):callback;
	auto middBtn = WWTouchSprite::createWithFile(btn_yellow,this, callBack);
	middBtn->setPosition(Vec2(558, 120));
	m_pBg->addChild(middBtn,BASIC_ZORDER_ROOT+3);
	//中间按钮字
	auto midLabel = Label::createWithTTF(btnTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	midLabel->setColor(GET_RGBCOLOR(txt_color));
	midLabel->setPosition(Vec2(0,0));
	middBtn->addChild(midLabel);
	//特效
	m_pBg->setScale(0.9f);
	EaseBackOut* _easeIn = EaseBackOut::create(ScaleTo::create(0.2f,1.0f));
	Action* _action = Sequence::create(_easeIn,nullptr);
	m_pBg->runAction(_action);
}

void CommDialog::initViewTwoBtn(const char* leftTxt,const char* rightTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback)
{
	//添加背景
	m_pBg = Sprite::create(contentBgImage);
	m_pBg->setPosition(CUR_SCREEN_SIZE.width*0.5,CUR_SCREEN_SIZE.height*0.5);
	this->addChild(m_pBg, BASIC_ZORDER_ROOT);

	//添加关闭按钮
	auto closeBtn = WWTouchSprite::createWithFile(closeBtnImage,this, callfuncO_selector(CommDialog::close));
	closeBtn->setPosition(Vec2(1116,731));
	m_pBg->addChild(closeBtn,BASIC_ZORDER_ROOT+3);

	//左边按钮
	SEL_CallFuncO leftCallBack = (leftCallback == NULL)?callfuncO_selector(CommDialog::close):leftCallback;
	auto leftBtn = WWTouchSprite::createWithFile(btn_bule,this, leftCallBack);
	leftBtn->setPosition(Vec2(331, 120));
	m_pBg->addChild(leftBtn,BASIC_ZORDER_ROOT+3);
	auto leftLabel = Label::createWithTTF(leftTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	leftLabel->setColor(GET_RGBCOLOR(txt_color));
	leftLabel->setPosition(Vec2(0,0));
	leftBtn->addChild(leftLabel);

	//右边按钮
	SEL_CallFuncO rightCallBack = (rightCallback == NULL)?callfuncO_selector(CommDialog::close):rightCallback;
	auto rightBtn = WWTouchSprite::createWithFile(btn_yellow,this, rightCallBack);
	rightBtn->setPosition(Vec2(785,120));
	m_pBg->addChild(rightBtn,BASIC_ZORDER_ROOT+3);
	auto rightLabel = Label::createWithTTF(rightTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	rightLabel->setColor(GET_RGBCOLOR(txt_color));
	rightLabel->setPosition(Vec2(0,0));
	rightBtn->addChild(rightLabel);

	//特效f
	m_pBg->setScale(0.9f);
	EaseBackOut* _easeIn = EaseBackOut::create(ScaleTo::create(0.2f,1.0f));
	Action* _action = Sequence::create(_easeIn,nullptr);
	m_pBg->runAction(_action);
}

void CommDialog::initViewTwoBtnOneMessage(const char* leftTxt,const char* rightTxt,const char* messageTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback)
{
	//添加背景
	m_pBg = Sprite::create(contentBgImage);
	m_pBg->setPosition(CUR_SCREEN_SIZE.width*0.5,CUR_SCREEN_SIZE.height*0.5);
	this->addChild(m_pBg, BASIC_ZORDER_ROOT);

	//添加关闭按钮
	/*auto closeBtn = WWTouchSprite::createWithFile(closeBtnImage,this, callfuncO_selector(CommDialog::close));
	closeBtn->setPosition(Vec2(1116,731));
	m_pBg->addChild(closeBtn,BASIC_ZORDER_ROOT+3);*/
	//提示信息文字
	auto messageLabel = Label::createWithTTF(messageTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	messageLabel->setDimensions(900.0f,500.0f);
	messageLabel->setPosition(Vec2(m_pBg->getContentSize().width*0.5f,m_pBg->getContentSize().height*0.7f));
	messageLabel->setColor(GET_RGBCOLOR(txt_color));
	m_pBg->addChild(messageLabel,BASIC_ZORDER_FRAME);

	//左边按钮
	SEL_CallFuncO leftCallBack = (leftCallback == NULL)?callfuncO_selector(CommDialog::close):leftCallback;
	auto leftBtn = WWTouchSprite::createWithFile(btn_bule,this, leftCallBack);
	leftBtn->setPosition(Vec2(331, 120));
	m_pBg->addChild(leftBtn,BASIC_ZORDER_ROOT+3);
	auto leftLabel = Label::createWithTTF(leftTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	leftLabel->setColor(GET_RGBCOLOR(txt_color));
	leftLabel->setPosition(Vec2(0,0));
	leftBtn->addChild(leftLabel);

	//右边按钮
	SEL_CallFuncO rightCallBack = (rightCallback == NULL)?callfuncO_selector(CommDialog::close):rightCallback;
	auto rightBtn = WWTouchSprite::createWithFile(btn_yellow,this, rightCallBack);
	rightBtn->setPosition(Vec2(785,120));
	m_pBg->addChild(rightBtn,BASIC_ZORDER_ROOT+3);
	auto rightLabel = Label::createWithTTF(rightTxt,"res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	rightLabel->setColor(GET_RGBCOLOR(txt_color));
	rightLabel->setPosition(Vec2(0,0));
	rightBtn->addChild(rightLabel);

	//特效
	m_pBg->setScale(0.9f);
	EaseBackOut* _easeIn = EaseBackOut::create(ScaleTo::create(0.2f,1.0f));
	Action* _action = Sequence::create(_easeIn,nullptr);
	m_pBg->runAction(_action);
}

bool CommDialog::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	CCLOG("WWDialog::ccTouchBegan");
	return true;
}

void CommDialog::close(Ref * node)
{
	this->removeFromParent();
}


void CommDialog::cancle(Ref* node)
{

}

void CommDialog::sure(Ref* node)
{

}


/************************************************************************/
/*   游戏中对话                                                         */
/************************************************************************/
TalkDialog::TalkDialog(void) :
	m_pLabel(nullptr)
{

}

TalkDialog::~TalkDialog()
{
}

void TalkDialog::onEnter()
{
	Sprite::onEnter();
	//auto m_pListner  = EventListenerTouchOneByOne::create();
	//m_pListner->onTouchBegan = CC_CALLBACK_2(TalkDialog::onTouchBegan, this);
	//m_pListner->setSwallowTouches(true);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);
}

void TalkDialog::onExit()
{
	//_eventDispatcher->removeEventListenersForTarget(this);
	Sprite::onExit();
}

TalkDialog* TalkDialog::create(int chatType)
{
	TalkDialog* pRet = new TalkDialog();
	if (pRet && pRet->init(chatType))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool TalkDialog::init(int chatType)
{
	if (!initWithFile("res/Images/GameScene/Chat/voiceBg.png"))
		return false;

	auto s_lChatIcon = Sprite::create("res/Images/GameScene/Chat/IconTmp.png");
    s_lChatIcon->setPosition(Vec2(s_lChatIcon->getContentSize().width, this->getContentSize().height * 0.5));
	this->addChild(s_lChatIcon);

	//根据种类去随机获得对话类容
	m_pLabel = Label::createWithTTF("","res/Fonts/FounderSemiBold.ttf",36,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
	m_pLabel->setDimensions(500, 80);
	m_pLabel->setColor(GET_RGBCOLOR(txt_color));
	m_pLabel->setPosition(Vec2(this->getContentSize().width * 0.6, this->getContentSize().height * 0.5));
	addChild(m_pLabel);
	
	getRandomString(chatType);

	this->runAction(Sequence::create(FadeIn::create(0.5f),
		                             DelayTime::create(2.0f),
									 RemoveSelf::create(),nullptr));

	return true;
}

bool TalkDialog::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void TalkDialog::getRandomString(int chatType)
{
	auto chatMap = FileUtils::getInstance()->getValueMapFromFile("res/XML/chatString.plist");
	ValueVector valueV;
	switch (chatType) {
	case CHATTYPE_URGE:
		valueV = chatMap.at("urge").asValueVector();
		break;
	case CHATTYPE_HAPPY:
		valueV = chatMap.at("happy").asValueVector();
		break;
	case CHATTYPE_RAGE:
		valueV = chatMap.at("rage").asValueVector();
		break;
	case CHATTYPE_DALLIANCE:
		valueV = chatMap.at("Dalliance").asValueVector();
		break;
	default:
		break;
	}

	auto vecCount = valueV.size();
	auto sayString = valueV.at(rand()%vecCount).asString();
    m_pLabel->setString(sayString);

	//发送说话请求
}
#include "SetLayer.h"
#include "SetLayerRes.h"
#include "../../../BaseCore/Common/UICoordConfig.h"
#include "../../../BaseCore/Common/FactoryTools.h"
#include "../../../WWMacros.h"
#include "../../../BaseCore/WWBGUI/WWTouchSprite.h"
#include "../../../Control/WWCSound/WWSoundManager.h"
#include "../CommLayer/CommDialog.h"
#include "Control/WWCSound/GameSoundManager.h"


SetLayer::SetLayer():
	m_pIsFormLobby(false)
{

}

SetLayer::~SetLayer()
{
	
}

SetLayer* SetLayer::create(bool isFromLobby)
{
	SetLayer* pItem = new SetLayer();
	if(pItem && pItem->init(isFromLobby))
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

bool SetLayer::init(bool isFromLobby)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(setLayerPlist);

	if ( !WWFrameBase::init())
	{
		return false;
	}
	m_pIsFormLobby = isFromLobby;
	createView(isFromLobby);
	
	return true;
}

void SetLayer::createView(bool isFromLobby)
{
	do 
	{
		

		auto bg = this->getpBg();

		//添加内容背景
		Sprite* contentBg = Sprite::create(contentBgImage);
		contentBg->setAnchorPoint(Vec2(0.5f,0.53f));
		contentBg->setPosition(bg->getContentSize().width/2, bg->getContentSize().height/2);
		bg->addChild(contentBg,BASIC_ZORDER_ROOT+1);

		//添加设置背景
		Sprite* contentImg = Sprite::createWithSpriteFrameName(contentImage);
		contentImg->setAnchorPoint(Vec2(0.5f,0.5f));
		contentImg->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[2]);
		bg->addChild(contentImg,BASIC_ZORDER_ROOT+2);
		//添加标题图片
		Sprite* title = Sprite::createWithSpriteFrameName(setTitleImage);
		title->setAnchorPoint(Vec2(0.5f,0.5f));
		title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[0]);
		bg->addChild(title, BASIC_ZORDER_ROOT+3);

	
		if (isFromLobby)
		{
			//添加版本更新按钮
			auto update = WWTouchSprite::createWithFile(updateBtnImage,this,callfuncO_selector(SetLayer::onTouchSpritePree));
			update->setTag(UPDATE_TAG);
			update->setAnchorPoint(Vec2(0,0));
			update->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[17]);
			bg->addChild(update,BASIC_ZORDER_FRAME);
			Sprite* updateTxt = Sprite::createWithSpriteFrameName(setUpdateImage);
			updateTxt->setAnchorPoint(Vec2(0.5f,0.5f));
			updateTxt->setPosition(Vec2(0,0));
			update->addChild(updateTxt,BASIC_ZORDER_FRAME+1);
			//有版本更新时的红点
			Sprite* red = Sprite::createWithSpriteFrameName(setRedImage);
			red->setAnchorPoint(Vec2(0.5f,0.5f));
			red->setPosition(Vec2(SET_LAYER_UI_POS[18]));
			update->addChild(red,BASIC_ZORDER_FRAME+2);

			//添加客服按钮
			auto kefu = WWTouchSprite::createWithFile(updateBtnImage,this,callfuncO_selector(SetLayer::onTouchSpritePree));
			kefu->setTag(KEFU_TAG);
			kefu->setAnchorPoint(Vec2(0,0));
			kefu->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[19]);
			bg->addChild(kefu, BASIC_ZORDER_FRAME);
			Sprite* kefuTxt = Sprite::createWithSpriteFrameName(setKefuImage);
			kefuTxt->setAnchorPoint(Vec2(0.5f,0.5f));
			kefuTxt->setPosition(Vec2(0,0));
			kefu->addChild(kefuTxt,BASIC_ZORDER_FRAME+1);
		}
		//UserDefault* default = UserDefault::getInstance();

		//游戏音效设置
		Sprite* gameSoundBg = Sprite::createWithSpriteFrameName(setItemBgImage);
		gameSoundBg->setAnchorPoint(Vec2(0.5f,0.5f));
		gameSoundBg->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[3]);
		bg->addChild(gameSoundBg, BASIC_ZORDER_TOP);

		Label* gameSwitch = Label::createWithTTF(GetWWString(g_Setting_effect_Str,g_Setting_Section),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		gameSwitch->setColor(GET_RGBCOLOR(SET_TXT_COLOR));
		gameSwitch->setAnchorPoint(Vec2(0.1f,0.5f));
		gameSwitch->setPosition(Vec2(gameSoundBg->getContentSize().width/10,gameSoundBg->getContentSize().height/2));
		gameSoundBg->addChild(gameSwitch,BASIC_ZORDER_TOP+1);

		CheckBox* gameSoundSwitch = CheckBox::create(setOff,setOff,setOn,"","");
		gameSoundSwitch->setAnchorPoint(Vec2(0.5f,0.5f));
		gameSoundSwitch->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[4]);
		bg->addChild(gameSoundSwitch,BASIC_ZORDER_TOP+1);
		gameSoundSwitch->setTag(SwitchTAG::GAME_SOUNG_SWITCH);
		gameSoundSwitch->addEventListener(CC_CALLBACK_2(SetLayer::gameSoundSetCallBack,this));

		if (UserDefault::getInstance()->getBoolForKey(SET_GAME_MUSIC_KEY,true))
		{
			gameSoundSwitch->setSelected(true);
		}
		else
		{
			gameSoundSwitch->setSelected(false);
		}

		//游戏背景音效开关
		Sprite* bgSound = Sprite::createWithSpriteFrameName(setItemBgImage);
		bgSound->setAnchorPoint(Vec2(0.5f,0.5f));
		bgSound->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[5]);
		bg->addChild(bgSound, BASIC_ZORDER_TOP);

		Label* bgSwitch = Label::createWithTTF(GetWWString(g_Setting_bg_music_Str,g_Setting_Section),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		//LabelBMFont* bgSwitch = LabelBMFont::create(GetWWString(g_Setting_bg_music_Str,g_Setting_Section),"res/Fonts/FounderSemiBold.ttf");
		bgSwitch->setColor(GET_RGBCOLOR(SET_TXT_COLOR));
		bgSwitch->setAnchorPoint(Vec2(0.1f,0.5f));
		bgSwitch->setPosition(Vec2(bgSound->getContentSize().width/10,bgSound->getContentSize().height/2));
		bgSound->addChild(bgSwitch,BASIC_ZORDER_TOP+1);

		CheckBox* bgSoundSwitch = CheckBox::create(setOff,setOff,setOn,"","");
		bgSoundSwitch->setAnchorPoint(Vec2(0.5f,0.5f));
		bgSoundSwitch->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[6]);
		bg->addChild(bgSoundSwitch,BASIC_ZORDER_TOP+1);
		bgSoundSwitch->setTag(SwitchTAG::BG_SOUNG_SWITCH);
		bgSoundSwitch->addEventListener(CC_CALLBACK_2(SetLayer::backgroundSetCallBack,this));

		if (UserDefault::getInstance()->getBoolForKey(SET_BG_MUSIC_KEY,true))
		{
			bgSoundSwitch->setSelected(true);
		}
		else
		{
			bgSoundSwitch->setSelected(false);
		}

		//游戏静音开关
		Sprite* noSound = Sprite::createWithSpriteFrameName(setItemBgImage);
		noSound->setAnchorPoint(Vec2(0.5f,0.5f));
		noSound->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[7]);
		bg->addChild(noSound, BASIC_ZORDER_TOP);

		Label* noSoundSwitch = Label::createWithTTF(GetWWString(g_Setting_Silent_onoff_Str,g_Setting_Section),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		noSoundSwitch->setColor(GET_RGBCOLOR(SET_TXT_COLOR));
		noSoundSwitch->setAnchorPoint(Vec2(0.1f,0.5f));
		noSoundSwitch->setPosition(Vec2(noSound->getContentSize().width/10,noSound->getContentSize().height/2));
		noSound->addChild(noSoundSwitch,BASIC_ZORDER_TOP+1);

		CheckBox* noSoundSwitchB = CheckBox::create(setOff,setOff,setOn,"","");
		noSoundSwitchB->setAnchorPoint(Vec2(0.5f,0.5f));
		noSoundSwitchB->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[8]);
		bg->addChild(noSoundSwitchB,BASIC_ZORDER_TOP+1);
		noSoundSwitchB->setTag(SwitchTAG::NO_SOUNG_SWITCH);
		noSoundSwitchB->addEventListener(CC_CALLBACK_2(SetLayer::noSoundSetCallBack,this));
		if (UserDefault::getInstance()->getBoolForKey(SET_NO_MUSIC_KEY,false))
		{
			noSoundSwitchB->setSelected(true);
		}
		else
		{
			noSoundSwitchB->setSelected(false);
		}

		//震动开关
		Sprite* shockSet = Sprite::createWithSpriteFrameName(setItemBgImage);
		shockSet->setAnchorPoint(Vec2(0.5f,0.5f));
		shockSet->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[9]);
		bg->addChild(shockSet, BASIC_ZORDER_TOP);

		Label* shockSwitch = Label::createWithTTF(GetWWString(g_Setting_vibrate_onoff_Str,g_Setting_Section),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		shockSwitch->setColor(GET_RGBCOLOR(SET_TXT_COLOR));
		shockSwitch->setAnchorPoint(Vec2(0.1f,0.5f));
		shockSwitch->setPosition(Vec2(shockSet->getContentSize().width/10,shockSet->getContentSize().height/2));
		shockSet->addChild(shockSwitch,BASIC_ZORDER_TOP+1);

		CheckBox* shockBox = CheckBox::create(setOff,setOff,setOn,"","");
		shockBox->setAnchorPoint(Vec2(0.5f,0.5f));
		shockBox->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[10]);
		bg->addChild(shockBox,BASIC_ZORDER_TOP+1);
		shockBox->setTag(SwitchTAG::SHOCK_SWITCH);
		shockBox->addEventListener(CC_CALLBACK_2(SetLayer::shockSetCallBack,this));
		if (UserDefault::getInstance()->getBoolForKey(SET_SHOCT_KEY,true))
		{
			shockBox->setSelected(true);
		}
		else
		{
			shockBox->setSelected(false);
		}
		//动画设置
		Sprite* animateSet = Sprite::createWithSpriteFrameName(setItemBgImage);
		animateSet->setAnchorPoint(Vec2(0.5f,0.5f));
		animateSet->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[12]);
		bg->addChild(animateSet, BASIC_ZORDER_TOP);

		Label* naimateSwitch = Label::createWithTTF(GetWWString(g_Setting_animation_onoff_Str,g_Setting_Section),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		naimateSwitch->setColor(GET_RGBCOLOR(SET_TXT_COLOR));
		naimateSwitch->setAnchorPoint(Vec2(0.1f,0.5f));
		naimateSwitch->setPosition(Vec2(animateSet->getContentSize().width/10,animateSet->getContentSize().height/2));
		animateSet->addChild(naimateSwitch,BASIC_ZORDER_TOP+1);

		CheckBox* animateBox = CheckBox::create(setOff,setOff,setOn,"","");
		animateBox->setAnchorPoint(Vec2(0.5f,0.5f));
		animateBox->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[11]);
		bg->addChild(animateBox,BASIC_ZORDER_TOP+1);
		animateBox->setTag(SwitchTAG::ANMAITE_SWITCH);
		animateBox->addEventListener(CC_CALLBACK_2(SetLayer::animateSetCallBack,this));
		if (UserDefault::getInstance()->getBoolForKey(SET_ANIMATE_KEY,true))
		{
			animateBox->setSelected(true);
		}
		else
		{
			animateBox->setSelected(false);
		}
		//滚动设置
		Sprite* scrollSet = Sprite::createWithSpriteFrameName(setItemBgImage);
		scrollSet->setAnchorPoint(Vec2(0.5f,0.5f));
		scrollSet->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[14]);
		bg->addChild(scrollSet, BASIC_ZORDER_TOP);

		Label* scrollSwitch = Label::createWithTTF(GetWWString(g_Setting_scroll_onoff_str,g_Setting_Section),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		scrollSwitch->setColor(GET_RGBCOLOR(SET_TXT_COLOR));
		scrollSwitch->setAnchorPoint(Vec2(0.1f,0.5f));
		scrollSwitch->setPosition(Vec2(scrollSet->getContentSize().width/10,scrollSet->getContentSize().height/2));
		scrollSet->addChild(scrollSwitch,BASIC_ZORDER_TOP+1);

		CheckBox* scrollBox = CheckBox::create(setOff,setOff,setOn,"","");
		scrollBox->setAnchorPoint(Vec2(0.5f,0.5f));
		scrollBox->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[13]);
		bg->addChild(scrollBox,BASIC_ZORDER_TOP+1);
		scrollBox->setTag(SwitchTAG::SCROLL_SWITCH);
		scrollBox->addEventListener(CC_CALLBACK_2(SetLayer::scrollSetCallBack,this));
		if (UserDefault::getInstance()->getBoolForKey(SET_SCROLL_KEY,true))
		{
			scrollBox->setSelected(true);
		}
		else
		{
			scrollBox->setSelected(false);
		}
		//自动续局设置
		Sprite* continuSet = Sprite::createWithSpriteFrameName(setItemBgImage);
		continuSet->setAnchorPoint(Vec2(0.5f,0.5f));
		continuSet->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[16]);
		bg->addChild(continuSet, BASIC_ZORDER_TOP);

		Label* continuSwitch = Label::createWithTTF(GetWWString(g_Setting_continu_onoff_str,g_Setting_Section),"res/Fonts/FounderSemiBold.ttf",38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
		continuSwitch->setColor(GET_RGBCOLOR(SET_TXT_COLOR));
		continuSwitch->setAnchorPoint(Vec2(0.1f,0.5f));
		continuSwitch->setPosition(Vec2(continuSet->getContentSize().width/10,continuSet->getContentSize().height/2));
		continuSet->addChild(continuSwitch,BASIC_ZORDER_TOP+1);

		CheckBox* continuBox = CheckBox::create(setOff,setOff,setOn,"","");
		continuBox->setAnchorPoint(Vec2(0.5f,0.5f));
		continuBox->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2) + SET_LAYER_UI_POS[15]);
		bg->addChild(continuBox,BASIC_ZORDER_TOP+1);
		continuBox->setTag(SwitchTAG::CONTIUN_SWITCH);
		continuBox->addEventListener(CC_CALLBACK_2(SetLayer::continuSetCallBack,this));
		if (UserDefault::getInstance()->getBoolForKey(SET_CONTINU_KEY,true))
		{
			continuBox->setSelected(true);
		}
		else
		{
			continuBox->setSelected(false);
		}
	}while (0);
}

void SetLayer::onEnter()
{
	LayerColor::onEnter();
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(setLayerPlist);
}

void SetLayer::onExit()
{
	LayerColor::onExit();
	//释放plist图片资源
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(setLayerPlist);
}


bool SetLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	WWFrameBase::onTouchBegan(pTouch, pEvent);
	return true;
}

//void SetLayer::onTouchMoved(Touch* pTouch, Event* pEvent)
//{
//}

void SetLayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	WWFrameBase::onTouchEnded(pTouch, pEvent);
}

//void SetLayer::onTouchCancelled(Touch* pTouch, Event* pEvent)
//{
//}


void SetLayer::gameSoundSetCallBack(Ref* sender, CheckBox::EventType controlEvent)
{
	switch (controlEvent)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("gamesound open");
		UserDefault::getInstance()->setBoolForKey(SET_GAME_MUSIC_KEY,true);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("gamesound close");
		UserDefault::getInstance()->setBoolForKey(SET_GAME_MUSIC_KEY,false);
		break;
	default:
		break;
	}
}


void SetLayer::backgroundSetCallBack(Ref* sender, CheckBox::EventType controlEvent)
{
	switch (controlEvent)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("bg sound open");
		
		UserDefault::getInstance()->setBoolForKey(SET_BG_MUSIC_KEY,true);
		if (m_pIsFormLobby)//判断是从哪里进的设置页面
		{
			if (FileUtils::getInstance()->isFileExist("res/Audio/mp3/rivulet.mp3"))
			{
				WWSoundManager::getInstance()->preloadBackgroundMusic("res/Audio/mp3/rivulet.mp3");
				WWSoundManager::getInstance()->playBackgroundMusic("res/Audio/mp3/rivulet.mp3", true);
			}
		}
		else
		{
			SOUND_MANGER->playBackGroundMusic(backGoundMusic_key);
		}
		
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("bg sound close");
		UserDefault::getInstance()->setBoolForKey(SET_BG_MUSIC_KEY,false);
		if (WWSoundManager::getInstance()->isBackgroundMusicPlaying())
		{
			WWSoundManager::getInstance()->stopBackgroundMusic();
		}
		break;
	default:
		break;
	}
}

void SetLayer::noSoundSetCallBack(Ref* sender, CheckBox::EventType controlEvent)
{
	switch (controlEvent)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("no sound");
		UserDefault::getInstance()->setBoolForKey(SET_NO_MUSIC_KEY,true);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("have sound");
		UserDefault::getInstance()->setBoolForKey(SET_NO_MUSIC_KEY,false);
		break;
	default:
		break;
	}
}

void SetLayer::shockSetCallBack(Ref* sender, CheckBox::EventType controlEvent)
{
	switch (controlEvent)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("shock open");
		UserDefault::getInstance()->setBoolForKey(SET_SHOCT_KEY,true);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("shock close");
		UserDefault::getInstance()->setBoolForKey(SET_SHOCT_KEY,false);
		break;
	default:
		break;
	}
}

void SetLayer::animateSetCallBack(Ref* sender, CheckBox::EventType controlEvent)
{
	switch (controlEvent)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("animate open");
		UserDefault::getInstance()->setBoolForKey(SET_ANIMATE_KEY,true);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("animate close");
		UserDefault::getInstance()->setBoolForKey(SET_ANIMATE_KEY,false);
		break;
	default:
		break;
	}
}

void SetLayer::scrollSetCallBack(Ref* sender, CheckBox::EventType controlEvent)
{
	switch (controlEvent)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("scroll open");
		UserDefault::getInstance()->setBoolForKey(SET_SCROLL_KEY,true);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("scroll close");
		UserDefault::getInstance()->setBoolForKey(SET_SCROLL_KEY,false);
		break;
	default:
		break;
	}
}

void SetLayer::continuSetCallBack(Ref* sender, CheckBox::EventType controlEvent)
{
	switch (controlEvent)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("continu open");
		UserDefault::getInstance()->setBoolForKey(SET_CONTINU_KEY,true);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("continu close");
		UserDefault::getInstance()->setBoolForKey(SET_CONTINU_KEY,false);
		break;
	default:
		break;
	}
}

void SetLayer::onTouchSpritePree(cocos2d::Ref* theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int pTag = btn->getTag();
	switch(pTag)
	{
	case UPDATE_TAG:
		log("updata.....");
		break;

	case KEFU_TAG:
		log("kefu......");
		break;
	}
}
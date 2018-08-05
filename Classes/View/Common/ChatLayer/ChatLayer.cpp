#include "ChatLayer.h"

#include "Control//WWRequest/MsgLogonRequest.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "View/Common/CommLayer/CommDialog.h"

USING_NS_CC;

using namespace std;

//功能按钮图片路径
#define CHATMENUAYYAY_SIZE 4 //图片个数
const char* menuArray[] = { 
	"chat_hurryup.png", //催促
	"chat_happy.png",	 //高兴
	"chat_angry.png",	 //愤怒
	"chat_flirtwith.png"//调戏
};

//功能按纽座标
const Vec2 kMenuAnimPos[]={
	Vec2(0, 134),  //催促
	Vec2(-116, 68),//高兴
	Vec2(-116, -68),//愤怒
	Vec2(0, -134),//调戏
};

const double CLIPLAYER_FACTOR =  0.2; //状态按钮跟字按钮背景的重叠比

const char* chat_plist_path = "res/Images/GameScene/Chat/chat.plist";

ChatLayer::ChatLayer(void) : 
	isOpenMenu(false),
	m_pChatBtn(nullptr)
{
}

ChatLayer::~ChatLayer()
{
	//释放资源
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(chat_plist_path);
}

bool ChatLayer::init()
{
	bool ret = false;

	do{
		CC_BREAK_IF(!LayerColor::initWithColor(Color4B(0,0,0,0)));
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(chat_plist_path);

		//禁止触摸穿透
		auto m_pListner  = EventListenerTouchOneByOne::create();
		m_pListner->onTouchBegan = CC_CALLBACK_2(ChatLayer::onTouchBegan, this);
		m_pListner->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner,this);

		/* 创建聊天按钮 */
		m_pChatBtn = WWTouchSprite::createWithSpriteFrameName("chat_icon.png", this, callfuncO_selector(ChatLayer::onTouchSpriteBtnEvent));
		CC_BREAK_IF(!m_pChatBtn);
		m_pChatBtn->setTag(BTN_CHATLAYER_MENU_BTN_TAG);
		m_pChatBtn->setPosition(Vec2(1807, 398));
		this->addChild(m_pChatBtn, BASIC_ZORDER_ROOT + 1 );

		//添加子按钮
		int firstTag = BTN_CHATLAYER_FUNCTAG_FIRST_BAG; //展开的第一个按钮Tag
		for (int i = 0; i < CHATMENUAYYAY_SIZE; i++)
		{
			auto s_menu = WWTouchSprite::createWithSpriteFrameName(menuArray[i], this, callfuncO_selector(ChatLayer::onTouchSpriteBtnEvent));
			CC_BREAK_IF(!s_menu);
			s_menu->setVisible(false);
			s_menu->setPosition(m_pChatBtn->getPosition());
			this->addChild(s_menu, BASIC_ZORDER_ROOT, firstTag++);
		}

		ret = true;

	}while(0);

	return ret;
}

bool ChatLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	//如果是收缩状态，则直接穿透,否则拦截
	log("ChatLayer::onTouchBegan");
	if(isOpenMenu){
		touchFuncMenuBar();
		return true;
	}
	return false;
}

void ChatLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void ChatLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	log("ChatLayer::onTouchEnded");
	if(isOpenMenu){
		touchFuncMenuBar();
	}
}

void ChatLayer::touchFuncMenuBar()
{
	isOpenMenu = !isOpenMenu; //状态反置
	int firstTag = BTN_CHATLAYER_FUNCTAG_FIRST_BAG; //展开的第一个按钮Tag
	if (isOpenMenu)
	{
		for (int i = 0; i < CHATMENUAYYAY_SIZE; i++)
		{
			auto tmpChild = (WWTouchSprite*)this->getChildByTag(firstTag + i);
			tmpChild->setVisible(true);
			tmpChild->setEnabled(false); //动画期间不响应
			auto temPos = Vec2(m_pChatBtn->getPosition().x + kMenuAnimPos[i].x, m_pChatBtn->getPosition().y + kMenuAnimPos[i].y);
			controlFadeIn(tmpChild, 0.1f, (i + 1), temPos);
		}
	}
	else
	{
		//收按钮
		for (int i = 0; i < CHATMENUAYYAY_SIZE; i++)
		{
			auto tmpChild = (WWTouchSprite*)this->getChildByTag(i + firstTag);
			tmpChild->setEnabled(false); //动画期间不响应
			controlFadeOut(tmpChild, 0.1f);
		}
	}

}

void ChatLayer::controlFadeIn(Node* node, float time, int seq, Vec2 pos)
{
	auto pScaleBy = ScaleBy::create(0.0f, 0.3f);
	auto moveTo = MoveTo::create(time, pos);
	auto fadeIn = FadeIn::create(time);
	auto pScaleTo = ScaleTo::create(time, 1.0f);
	node->setVisible(true);
	node->runAction(Sequence::create(pScaleBy, Spawn::create(moveTo,fadeIn,pScaleTo, nullptr), CallFuncN::create(CC_CALLBACK_1(ChatLayer::onMenuFadeInAnimEnd, this)),nullptr));
}

void ChatLayer::onMenuFadeInAnimEnd(Node *theBtn)
{
	auto tmpBtn = (WWTouchSprite*)theBtn;
	if(tmpBtn){
		tmpBtn->setEnabled(true);
	}
}

void ChatLayer::controlFadeOut(Node* node, float time)
{
	auto moveTo = MoveTo::create(time, m_pChatBtn->getPosition());
	auto fadeOut = FadeOut::create(time);
	auto pScaleTo = ScaleTo::create(time, 0.3f);
	node->runAction(Sequence::create( Spawn::create(moveTo,fadeOut,pScaleTo, nullptr), CallFuncN::create(CC_CALLBACK_1(ChatLayer::onMenuFadeOutAnimEnd, this)),nullptr));
}

void ChatLayer::onMenuFadeOutAnimEnd(Node *theBtn)
{
	auto tmpBtn = (WWTouchSprite*)theBtn;
	if(tmpBtn){
		tmpBtn->setEnabled(false);
		tmpBtn->setVisible(false);
	}
}

void ChatLayer::onTouchSpriteBtnEvent(Ref * theBtn)
{
	auto btn = dynamic_cast<WWTouchSprite*>(theBtn);
	int tagId = btn->getTag();
	if (tagId == BTN_CHATLAYER_MENU_BTN_TAG)
	{
		touchFuncMenuBar();
	}else if(tagId == BTN_CHATLAYER_FUNCTAG_FIRST_BAG){	//催促
		log(" hurry up! ");
		touchFuncMenuBar();

		talk(CHATTYPE_URGE);
	}else if(tagId == BTN_CHATLAYER_FUNCTAG_FIRST_BAG + 1 ){	//高兴
		log(" happy!");
		touchFuncMenuBar();

		talk(CHATTYPE_HAPPY);
	}else if(tagId == BTN_CHATLAYER_FUNCTAG_FIRST_BAG + 2 ){	//愤怒
		log(" angry!");
		touchFuncMenuBar();

		talk(CHATTYPE_RAGE);
	}else if(tagId == BTN_CHATLAYER_FUNCTAG_FIRST_BAG + 3 ){	//调戏
		log(" flirtwith!");
		touchFuncMenuBar();

		talk(CHATTYPE_DALLIANCE);
	}
}

void ChatLayer::talk(int type)
{
	auto talk = TalkDialog::create(type);
	talk->setPosition(Vec2(500, 500));
	this->addChild(talk);
}



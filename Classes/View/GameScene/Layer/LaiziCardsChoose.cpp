/*
 * LaiziCardsChoose.cpp
 *
 *  Created on: 2015-6-26
 *      Author: Jacky
 */

#include "LaiziCardsChoose.h"
#include "../../Common/CardSprite/CardsGroup.h"
#include "../GameSceneDefine.h"
#include "Control/WWCSound/WWSoundManager.h"
#include "View/GameScene/GameLogicManager.h"

USING_NS_CC;

const float g_fCardsGroupHeightRatio = 0.7f;                    //牌换行时，上下高度的距离，牌高度*ratio

LaiziCardsTable::LaiziCardsTable() :
	m_pListView(nullptr),
	m_cellSize(Size(0, 0))
{
    
}
LaiziCardsTable::~LaiziCardsTable()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}

LaiziCardsTable* LaiziCardsTable::create(const std::vector<Vector<PokerObject*>> &pokerArr)
{
    LaiziCardsTable *pRet = new LaiziCardsTable();
	if (pRet && pRet->initWithPokerArr(pokerArr))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool LaiziCardsTable::initWithPokerArr(const std::vector<Vector<PokerObject*>> &pokerArr)
{
    if (!CCLayer::init()|| pokerArr.size() == 0) 
	{
        CCLOG("LaiziCardsTable init failed");
        return false;
    }

	//为了屏幕除了当前框口之外的触摸，使用此控件时，zorder要设置为最高的
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(LaiziCardsTable::onTouchBegan, this);
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//设置当前控件大小
	this->setContentSize(Size(LAIZI_CARDS_TABLE_WIDTH, LAIZI_CARDS_TABLE_HEIGHT));
	// 创建ListView
	if (!m_pListView)
	{
		m_pListView = ListView::create();
		// 设置方向
		m_pListView->setDirection(ui::ScrollView::Direction::VERTICAL);
		m_pListView->setTouchEnabled(true);
		m_pListView->setBounceEnabled(true); //是否开启回弹
		m_pListView->setBackGroundImage(g_rescalChooseGg.c_str());
		m_pListView->setBackGroundImageScale9Enabled(true); //是否按.9方式拉升
		m_pListView->setContentSize(Size(LAIZI_CARDS_TABLE_WIDTH - 20.0f, LAIZI_CARDS_TABLE_HEIGHT - 20.0f)); //控件背景，固定大小
		m_pListView->setPosition(Vec2::ZERO);
		m_pListView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(LaiziCardsTable::listItemClick, this));
	}
    addChild(m_pListView);

    setPokerGroupArr(pokerArr);
    
    //此处是为了获取到一手牌的长度，由于其他视图加载在每个表格里，在那里获取会再for循环中多次获取，
	//所以在这里创建一个获取到长度,并设置格子的宽度，此处并不需要加载到视图上
    Vector<PokerObject*> oneArr = pokerArr.at(0);
    CardsGroup* group = CardsGroup::create(oneArr, normal_Card, g_fCardsGroupHeightRatio);
    Size groupSize = group->getGroupContentSize();
    setCellSize(Size(groupSize.width, groupSize.height * 0.47f + LAIZI_CARDS_TABLE_CARDS_GROUP_MARGIN)); //牌大小为正常牌大小的 47%, just for test.
  
	createListView();

    return true;
}

void LaiziCardsTable::createListView()
{
	m_pListView->removeAllItems(); //清空Item

	for(int index = 0; index < m_pokerGroupArr.size(); index++)
	{
		CardsGroup* cardGroup = CardsGroup::create(m_pokerGroupArr.at(index), normal_Card, g_fCardsGroupHeightRatio);
		if(cardGroup){
			auto pokerGroup = m_pokerGroupArr.at(index);
			int diffX = 0;
			if(pokerGroup.size()>4){
				if( pokerGroup.size()>10 ){ //一行最多 10 张
					diffX = (10 - 4 + 1) * 20;
				}else{
					diffX = (pokerGroup.size() - 4 + 1) * 20;
				}
			}
			auto custom_item = Layout::create();
			custom_item->setContentSize(Size((LAIZI_CARDS_TABLE_WIDTH - 20.0f)*0.98f, m_cellSize.height));
			cardGroup->setPosition(Vec2(custom_item->getContentSize().width*0.5f - diffX, m_cellSize.height - 25.0f));
			custom_item->addChild(cardGroup);
			custom_item->setTouchEnabled(true);

			m_pListView->pushBackCustomItem(custom_item);
		}
	}

}

void LaiziCardsTable::listItemClick(Ref *pSender, ListView::EventType type)
{
	log("LaiziCardsTable::listItemClick");
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

			ListView* listView = static_cast<ListView*>(pSender);
			log("select item index = %d", listView->getCurSelectedIndex());
			int index = (int)listView->getCurSelectedIndex();
			Vector<PokerObject*> pokerArr = m_pokerGroupArr.at( index );
			if (pokerArr.size() > 0)
			{
				GameLogicManager::getInstance()->getpUIMyCardLayer()->requestPlayCards(GameLogicManager::getInstance()->getpUIMyCardLayer()->getSelectedPokers(), pokerArr);
				/*GameLogicManager::getInstance()->getpUIgameScene()->removeChooseLaiziTable();*/ //马上释放会报错，等超时或收到出牌协议再释放
			}
		}

		break;
	default:
		break;
	}
}

//重写触摸函数，返回true，屏蔽其它层，达到“模态”效果
bool LaiziCardsTable::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	//log("LaiziCardsTable::onTouchBegan.");
	return true;
}

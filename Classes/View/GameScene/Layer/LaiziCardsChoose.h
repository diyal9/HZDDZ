/*
 * LaiziCardsChoose.h
 *
 *  Created on: 2015-6-26
 *      Author: Jacky
 */

#ifndef __VIEW_GAMESCENE_LAYER_LaiziCardsChoose__
#define __VIEW_GAMESCENE_LAYER_LaiziCardsChoose__

#include "cocos2d.h"
#include <iostream>
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class PokerObject;

class LaiziCardsTable : public Layer
{
public:
    LaiziCardsTable();
    ~LaiziCardsTable();

    //创建一个选牌的tableView
    static LaiziCardsTable* create(const std::vector<cocos2d::Vector<PokerObject*>> &pokerArr);
    
    bool initWithPokerArr(const std::vector<cocos2d::Vector<PokerObject*>> &pokerArr);

	//重写触摸函数，返回true，屏蔽其它层，达到“模态”效果
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	void createListView(); //创建listview内容
	
	void listItemClick(Ref *pSender, ListView::EventType type);

public:
    //所有能打出的牌
    CC_SYNTHESIZE(std::vector<cocos2d::Vector<PokerObject*>>, m_pokerGroupArr, PokerGroupArr);
	
	CC_SYNTHESIZE(ListView* ,m_pListView, pListView);

    //单元格大小
    CC_SYNTHESIZE(Size, m_cellSize, CellSize);
    
private:
    
};
#endif /* defined(__WW_Platform__LaiziCardsChoose__) */

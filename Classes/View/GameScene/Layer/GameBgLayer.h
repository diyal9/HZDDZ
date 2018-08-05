/*
 * GameBgLayer.h
 *
 *
 *  Created on: 2015-5-11
 *      Author: diyal.yin
 */

#ifndef _VIEW_GAMELAYER_GAMEBGLAYER_H_
#define _VIEW_GAMELAYER_GAMEBGLAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class GameBgLayer : public LayerColor
{
public:
	GameBgLayer();
	virtual ~GameBgLayer();
	CREATE_FUNC(GameBgLayer);
	virtual bool init();
	void createView();
	
};

#endif /*_VIEW_GAMELAYER_GAMEBGLAYER_H_*/

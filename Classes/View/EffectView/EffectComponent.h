/*! @file
********************************************************************************
<PRE>
模块名       :  动画特效
文件名       :  EffectComponent.h
文件实现功能 :  游戏中各动画效果层
作者         :  Diyal.yin
版本         :  1.0
Copyright (c)  WaWaGame Technologies
--------------------------------------------------------------------------------
LeafFallingLayer 2.5D落叶效果。
FlyChickenLayer  飞机动画
RocketLayer      火箭动画
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/06/29   1.0      diyal.yin           新建 春天动画
2015/07/01   1.0      diyal.yin           飞机动画
2015/07/01   1.0      diyal.yin           火箭动画
2015/07/02   1.0      diyal.yin           炸弹动画
</PRE>
*******************************************************************************/
#ifndef _EFFECTVIEW_LEAFFALLINGLAYER_H_
#define _EFFECTVIEW_LEAFFALLINGLAYER_H_

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

//春天动画
class LeafFallingLayer : public Layer 
{
public:
	LeafFallingLayer();
	virtual ~LeafFallingLayer();
    
	virtual bool init();

	CREATE_FUNC(LeafFallingLayer);

	virtual void onEnter();
	
	virtual void onExit();

	void resetLeafPos(Node* sender);//叶片位置重置函数

	void playLeafAnim(Sprite *spriteLeaf);//下落过程实现函数

};

//飞鸡动画
class FlyChickenLayer : public LayerColor 
{
public:
	FlyChickenLayer();
	virtual ~FlyChickenLayer();

	virtual bool init();

	/**
	* 创建飞机动画
	* @program fromSite 出飞机位置  0为自己出飞机 1为右边（即下家）出飞机 2为左边（即上家）出飞机
	*/
	static FlyChickenLayer* createWithSite(int fromSite);
	//CREATE_FUNC(FlyChickenLayer);

	void callBackEndAction();

	void removeSelf();

	virtual void onEnter();

	virtual void onExit();

private:
	CC_SYNTHESIZE(int,p_fromSite,pFromSite);

};

//火箭动画
class RocketLayer : public LayerColor
{
public:
	RocketLayer();
	virtual ~RocketLayer();

	virtual bool init();

	/**
	* 创建火箭动画
	* @program toSite 火箭爆炸的第一个目标 0为自己家爆炸 1为右边玩家（即下家）爆炸 2为左边玩家（即上家）爆炸
	* @program toSite2 火箭爆炸的第二个目标 如果火箭爆炸目标只有一个 则此参数传 -1
	*/
	static RocketLayer* createWith(int toSite, int toSite2);
	//CREATE_FUNC(RocketLayer);

	void stepTwo(); //阶段二

	void frontsight(Node* node);
	void frontsight2(Node* node);

	void rocketMoveInBack(Node* node);
	void bombStep(Node* node);

	void removeSelf();
	void setX2();

	virtual void onEnter();

	virtual void onExit();

private:
	int iCurSignIndex; //当前第几个瞄准器
	int iCurSignIndex1;
	CC_SYNTHESIZE(int,p_site, pSite);//第一个火箭目标
	CC_SYNTHESIZE(int,p_site2, pSite2);//第二个火箭目标
};

//炸弹动画
class BombsLayer : public LayerColor
{
public:
	BombsLayer();
	virtual ~BombsLayer();

	virtual bool init();

	/**
	* 创建炸弹动画
	* @program fromSite  炸弹抛出位置 0为 自己家  1为右边（即下家）  2为左边（即上家）
	* @program toSite1   炸弹目标1位置 0为 自己家  1为右边（即下家）  2为左边（即上家）
	* @program toSite2   炸弹目标2位置 0为 自己家  1为右边（即下家）  2为左边（即上家） 若只有一个目标则此参数传 -1
	*/
	static BombsLayer* createWithSite(int fromSite,int toSite1,int toSite2);
	//CREATE_FUNC(BombsLayer);

	void afterBomb(); //爆炸效果结束后，显示被炸的房子
	void removeSelf();

	virtual void onEnter();

	virtual void onExit();

private:
	float npcBombFlyTimes; //Npc之间炸弹飞行持续时间
	int BombsRotateby; //炸弹旋转角度
	Point fromRolePoint;
	Point toRolePoint;
	CC_SYNTHESIZE(int,p_fromSite,pFromSite);//炸弹抛出位置 0为自己家 1为右边（即下家） 2为左边（即上家）
	CC_SYNTHESIZE(int,p_toSite1,pToSite1);//炸弹目标位置1 0为自己家 1为右边（即下家） 2为左边（即上家）
	CC_SYNTHESIZE(int,p_toSite2,pToSite2);//炸弹目标位置2 0为自己家 1为右边（即下家） 2为左边（即上家）

	bool isStorePoint;
	Point shakePointReset; 
private:
	void setNpcBombsFlyAction(); //NPC之间炸弹飞行轨迹动画执行

	void bombsAction(Node* node); //爆炸效果

	void setNpc2Me(); //NPC扔我
	void toSecond();//第二个目标
	void setToNpc(int toSite);
	void setX2();

	void resetPosition(); //抖动后的位置矫正
};

/**
*   X2动画
*/
class X2Layer : public LayerColor
{
public:
	X2Layer();
	virtual ~X2Layer();

	virtual bool init();

	CREATE_FUNC(X2Layer);
   
	void removeSelf();

	virtual void onEnter();

	virtual void onExit();
};

#endif //_EFFECTVIEW_LEAFFALLINGLAYER_H_
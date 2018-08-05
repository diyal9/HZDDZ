/*! @file
********************************************************************************
<PRE>
模块名       :  基本控件
文件名       :  WWAnimation.h
文件实现功能 :  帧动画播放器
作者         :  Diyal.yin
版本         :  1.0
Copyright (c)  WaWaGame Technologies
--------------------------------------------------------------------------------
普通图片按钮创建
Cross图片按钮创建
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/06/04   1.0      diyal.yin           新建
</PRE>
*******************************************************************************/
#ifndef __BASECORE_WWEFFECTBASE_WWANIMATION__
#define __BASECORE_WWEFFECTBASE_WWANIMATION__

#include "cocos2d.h"

USING_NS_CC;

class WWAnimation
{
public:
	WWAnimation();
	virtual ~WWAnimation();
	static WWAnimation* getInstance();
	void init();
	Animation* createAnimationByName(const std::string& animName, float delay = 0.1f); //创建帧动画
	Sprite* createAnimationSprite(const std::string& animName); //创建帧动画精灵
	Sprite* createSpriteByAnimation(Animation* anim);

	Animation* animationUnCache(const std::string& animName, int frameCnt, float delay = 0.1f, int times = 1); //创建帧动画 不缓存 不用的时候需要手动删除缓存
	void removeAnimationCache(const std::string& animName); //删除指定的缓存

private:
	std::map<std::string, Animation*> m_vAnimations;
	std::map<std::string, Value> m_vAnimConfig;
};

#endif /* __BASECORE_WWEFFECTBASE_WWANIMATION__ */

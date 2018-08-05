/*
 * VisibleRect.h
 *
 * 设计思路:
 * VisibleRect是相对屏幕可视区域的位置贴图的设计
 *
 *  Created on: 2015-5-11
 *      Author: diyal.yin
 */
#ifndef __BASECORE_COMMON_VISIBLERECT_H__
#define __BASECORE_COMMON_VISIBLERECT_H__

#include "cocos2d.h"
#include "GameConfig.h"
#include "WWMacros.h"

class VisibleRect
{
public:
    static cocos2d::Rect getVisibleRect(); //获得可视区域

    static cocos2d::Vec2 left(); //左中
    static cocos2d::Vec2 right(); //右中
    static cocos2d::Vec2 top(); //顶中
    static cocos2d::Vec2 bottom(); //底中
    static cocos2d::Vec2 center(); //正中
    static cocos2d::Vec2 leftTop(); //左顶
    static cocos2d::Vec2 rightTop(); //右顶
    static cocos2d::Vec2 leftBottom(); //左底
    static cocos2d::Vec2 rightBottom(); //右底

	//锚点
	static cocos2d::Vec2 anchLeft(); //左中 (0, 0.5)
	static cocos2d::Vec2 anchRight(); //右中 (1, 0.5)
	static cocos2d::Vec2 anchTop(); //顶中 (0.5, 1)
	static cocos2d::Vec2 anchBottom(); //底中 (0.5, 0)
	static cocos2d::Vec2 anchCenter(); //正中(0.5, 0.5)
	static cocos2d::Vec2 anchLeftTop(); //左顶(0, 1)
	static cocos2d::Vec2 anchRightTop(); //右顶(1, 1)
	static cocos2d::Vec2 anchLeftBottom(); //左底(0, 0)
	static cocos2d::Vec2 anchRightBottom(); //右底(1, 0)

	/**得到对齐coord
		返回相对于屏幕尺寸 pAlign 对齐时的座标
		默认为 pAligh 方式居中
		nodeSize 为设置位置节点大小
		pos 为修正座标
		此方式仅为模块设计，如大厅工具条相对于屏幕
	**/
	static Vec2 getNodeAlignPos(NodeAlignMent pAlign, Size nodeSize, Vec2 pos);

private:
    static void lazyInit();
    static cocos2d::Rect s_visibleRect;
};

#endif /* __BASECORE_COMMON_VISIBLERECT_H__ */

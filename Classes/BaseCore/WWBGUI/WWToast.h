/*! @file
********************************************************************************
<PRE>
模块名       :  基本控件
文件名       :  WWToast.h
文件实现功能 :  Toast
作者         :  Diyal.yin
版本         :  1.0
Copyright (c)  WaWaGame Technologies
--------------------------------------------------------------------------------
Toast 仿照Android的Toast实现方式。分为默认和传递时间的两个函数
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2015/06/22   1.0      diyal.yin           新建
</PRE>
*******************************************************************************/

#ifndef __HD_DDZ_BASECORE_WWBGUI_WWTOAST_H__
#define __HD_DDZ_BASECORE_WWBGUI_WWTOAST_H__

#include "cocos2d.h"

class WWToast
{
public:
	WWToast();
	virtual ~WWToast();

	static void makeText(const std::string &text, float time = 0.8f); //Toast
	static cocos2d::Node* createToast(const std::string &text, float time); //为了对应品质向上之前的代码，添加一个这样的函数

};

#endif /* __HD_DDZ_BASECORE_WWBGUI_WWTOAST_H__ */

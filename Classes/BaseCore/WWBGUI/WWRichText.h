/*
* WWRichText.h
*
*	富文本标签
*
*  Created on: 2015-3-24
*      Author: Diyal
*/

/*
//根据xml配置文件创建，直接根据xml的key创建
auto richText = WWRichText::create("TestDemo",Size(500, 300));
richText->setZOrder(10000);
richText->setPosition(Size(winSize.width * 0.5, winSize.height*0.3));
richText->setVerticalSpace(10.0f);
this->addChild(richText);

//自定义数据格式创建
RichTextCellObject pRichObj;
pRichObj.Content = "自定义非固定配置方式创建RichText";
pRichObj.Font = "Marker Felt";
pRichObj.rgb = "255,202,76";
pRichObj.Size = 30;

RichTextCellObject pRichObj2;
pRichObj2.Content = "create RichText by data array ";
pRichObj2.Font = "Helvetica";
pRichObj2.rgb = "0,0,50";
pRichObj2.Size = 15;

RichTextCellObject pRichObj3;
pRichObj3.Content = "helloworld";
pRichObj3.Font = "Helvetica";
pRichObj3.rgb = "255,255,0";
pRichObj3.Size = 35;

std::vector<RichTextCellObject> richTextArr;
richTextArr.push_back(pRichObj);
richTextArr.push_back(pRichObj2);
richTextArr.push_back(pRichObj3);

auto richText2 = WWRichText::create(Size(winSize.width * 0.5, winSize.height*0.3), richTextArr);
richText2->setZOrder(10000);
richText2->setPosition(Size(winSize.width * 0.5, winSize.height*0.7));
richText2->setVerticalSpace(10.0f);
this->addChild(richText2);

create函数里面的第一个参数为，XML文件夹下的WWRichText.xml里面注册好，直接就可以使用了。

*/

#ifndef _WWRichText_H_
#define _WWRichText_H_

#include "cocos2d.h"
#include "cocos/ui/UIRichText.h"

USING_NS_CC;

using namespace ui;

struct RichTextCellObject
{
	std::string Content;  //类容
	std::string rgb;      //颜色
	std::string Font;     //字体
	int Size;             //字体大小
};

//项目富文本资源的存放路径 Resource/XML/WWString.xml
const std::string RICHTEXT_XML_PATH = "XML/WWRichText.xml";

class WWRichText : public cocos2d::LayerColor
{
public:
	WWRichText();

	virtual ~WWRichText();

	//根据文件读取
	static WWRichText * create(const char* keyName, cocos2d::Size textSize);

	//传入参数创建富文本
	static WWRichText * create(cocos2d::Size textSize, std::vector<RichTextCellObject> pVecRichTextArr);

	virtual bool init(const char* keyName, cocos2d::Size textSize);

	virtual bool init(cocos2d::Size textSize, std::vector<RichTextCellObject> pVecRichTextArr);

	//解析xml文件并且
	bool getAllCellByKey(const char* keyName);

	//根据解析数据绘制标签
	void createTextByDataArr();

	//设置竖直方向间距
	void setVerticalSpace(float verticalValue);

private:
	std::vector<RichTextCellObject> vecRichTextArr;
protected:
	ui::RichText* _richText;
};


#endif //_WWRichText_H_
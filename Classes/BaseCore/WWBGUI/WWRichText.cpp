/*
* WWRichText.cpp
*
*	富文本标签
*
*  Created on: 2015-3-24
*      Author: Diyal
*/
#include "WWRichText.h"
#include "Control/WWXMLUnicodeManager/WWXMLManager.h"
#include "BaseCore/WWBTools/WW_GBK_TO_UTF8.h"
#include "BaseCore/WWBTools/StringTool.h"

WWRichText::WWRichText():
	_richText(nullptr),
	vecRichTextArr(NULL)
{

}

WWRichText::~WWRichText()
{

}

WWRichText * WWRichText::create(const char* keyName, Size textSize)
{
	WWRichText * text = new WWRichText();
	if (text && text->init(keyName, textSize))
	{
		text->autorelease();
		return text;
	}

	CC_SAFE_DELETE(text);

	return NULL;
}

WWRichText * WWRichText::create(cocos2d::Size textSize, std::vector<RichTextCellObject> pVecRichTextArr)
{
	WWRichText * text = new WWRichText();
	if (text && text->init(textSize, pVecRichTextArr))
	{
		text->autorelease();
		return text;
	}

	CC_SAFE_DELETE(text);

	return NULL;
}

bool WWRichText::init(const char* keyName, Size textSize)
{
	if ( !LayerColor::initWithColor(ccColor4B(255,255,255,0)))
	{
		return false;
	}

	this->setContentSize(textSize);
	this->setAnchorPoint(Vec2(0.5, 0.5));

	_richText = RichText::create();
	_richText->ignoreContentAdaptWithSize(false);
	_richText->setContentSize(textSize);
	_richText->setPosition(Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5));
	this->addChild(_richText);

	//数据解析
	getAllCellByKey(keyName);

	//根据数据创建UI
	createTextByDataArr();

	return true;
}

bool WWRichText::init(cocos2d::Size textSize, std::vector<RichTextCellObject> pVecRichTextArr)
{
	if ( !LayerColor::initWithColor(ccColor4B(255,255,255,0)))
	{
		return false;
	}

	this->setContentSize(textSize);
	this->setAnchorPoint(Vec2(0.5, 0.5));

	if (pVecRichTextArr.size() == 0)
	{
		return false;
	}


	_richText = RichText::create();
	_richText->ignoreContentAdaptWithSize(false);
	_richText->setPosition(Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5));
	_richText->setContentSize(textSize);
	this->addChild(_richText);

	vecRichTextArr = pVecRichTextArr;

	//根据数据创建UI
	createTextByDataArr();

	return true;
}

bool WWRichText::getAllCellByKey(const char* keyName)
{

	//加载XML文件
	WWXMLManager myXML;
	if (!myXML.loadXMLFile(RICHTEXT_XML_PATH.c_str()))
	{
		CCLOG("load xml resource error!");
		return false;
	}

	//遍历xml指定node，并且将其子节点存到vector里面
	WWXMLNode node = myXML.getXMLRootNode().findChildNode(keyName);
	if (!node.isNULL())
	{

		WWXMLNode childNode = NULL;
		do 
		{

			if (childNode.isNULL())
			{
				childNode = node.findChildNode("Cell");
			}
			else
			{
				childNode = childNode.findSlibingNode("Cell");
			}

			RichTextCellObject cell;

			//字体
			const char *fontValue = childNode.getAttributeValue("Font");
			if (fontValue)
			{
				cell.Font = fontValue;
			}

			//颜色
			const char *colorValue = childNode.getAttributeValue("Color");
			if (colorValue)
			{
				cell.rgb = colorValue;
			}

			//字体大小
			const char *sizeValue = childNode.getAttributeValue("Size");
			if (sizeValue)
			{
				cell.Size = std::atoi(sizeValue);
			}

			WWXMLNode valueNode = childNode.findChildNode("String");

			std::string strResult = valueNode.getNodeValue();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			GBKToUTF8(strResult, "UTF-8", "GBK");
#endif
			cell.Content = strResult;

			vecRichTextArr.push_back(cell);

		} while (!childNode.findSlibingNode("Cell").isNULL());

		CCLOG("tests" );
	}
}

void WWRichText::createTextByDataArr()
{
	if (vecRichTextArr.size() == 0)
	{
		return;
	}

	for (int i = 0; i < vecRichTextArr.size(); i++)
	{
		RichTextCellObject cell = vecRichTextArr[i];

		std::vector<std::string> problemStrVec = StringTool::splitByArr(cell.rgb, ",");

		RichElementText* re = RichElementText::create(i + 1,
			Color3B(std::atoi(problemStrVec[0].c_str()),std::atoi(problemStrVec[1].c_str()),std::atoi(problemStrVec[2].c_str())), 
			255, 
			cell.Content,
			cell.Font,
			cell.Size);

		_richText->pushBackElement(re);
	}
}

void WWRichText::setVerticalSpace(float verticalValue)
{
	_richText->setVerticalSpace(verticalValue);
}
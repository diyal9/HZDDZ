/**
 * SetLayer.h
 * created in 2015-5-7
 * by Toby
 *
**/
#ifndef __HZ_DDZ_VIEW_CommDialog_SETLAYER_H__
#define __HZ_DDZ_VIEW_CommDialog_SETLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

USING_NS_CC;

/**
 * 开关TAG
**/


class CommDialog : public LayerColor
{
public:
	CommDialog();
	virtual ~CommDialog();
	/**
	 *创建一个带有一个按钮和提示信息的弹框
	**/
	static CommDialog* creatOneBtnOneMessage(const char* btnTxt,const char* messageTxt);
	/*CREATE_FUNC(CommDialog);*/
	/**
	 * 创建一个按钮的弹框
	 * 如果callback传NULL点击时为关闭弹框
	**/
	static CommDialog* createOneBtn(const char* btnTxt, SEL_CallFuncO callback);
	/**
	 * 创建两个个按钮的弹框
	 * 如果callback传NULL点击时为关闭弹框
	**/
	static CommDialog* createTwoBtn(const char* leftTxt,const char* rightTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback);
	/**
	 *创建2个按钮的信息提示框
	 *取消按钮callback传NULL即可，另一个callback为自定义
	**/
	static CommDialog* creatTwoBtnOneMessage(const char* leftTxt,const char* rightTxt,const char* messageTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback);
	virtual bool initOneBtnOneMessage(const char* btnTxt,const char* messageTxt);
	virtual bool initOneBtn(const char* btnTxt, SEL_CallFuncO callback);
	virtual bool initTwoBtn(const char* leftTxt,const char* rightTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback);
	virtual bool initTwoBtnOneMessage(const char* leftTxt,const char* rightTxt,const char* messageTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback);
	virtual void onEnter();

	virtual void onExit();

	virtual void initViewOneBtnOneMessage(const char* btnTxt,const char* messageTxt);
	
	virtual void initViewOneBtn(const char* btnTxt, SEL_CallFuncO callback);
	
	virtual void initViewTwoBtn(const char* leftTxt,const char* rightTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback);

	virtual void initViewTwoBtnOneMessage(const char* leftTxt,const char* rightTxt,const char* messageTxt, SEL_CallFuncO leftCallback,SEL_CallFuncO rightCallback);
	void setBackgroudColor(cocos2d::Color4B bgColor){initWithColor(bgColor);};

	void close(Ref * node);
	void cancle(Ref* node);
	void sure(Ref* node);

	/*** back按钮监听 ****/
	virtual void setBackKeyEnabled(bool enable){};

    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	//弹窗特效

private:
	CC_SYNTHESIZE(Sprite*, m_pBg, pBg);
};

/************************************************************************/
/*   游戏中对话                                                         */
/************************************************************************/
enum ChatType
{
	CHATTYPE_URGE,      //催促
	CHATTYPE_HAPPY = 1, //愤怒
	CHATTYPE_RAGE,     //高兴
	CHATTYPE_DALLIANCE //调戏
};

class TalkDialog : public Sprite
{
public:

	TalkDialog();
	virtual ~TalkDialog();
    static TalkDialog* create(int chatType);

	bool init(int chatType);

	virtual void onEnter();

	virtual void onExit();

    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void getRandomString(int chatType);

private:
	 CC_SYNTHESIZE(Label*, m_pLabel, m_pLabel); 
};

#endif // !__HZ_DDZ_VIEW_CommDialog_SETLAYER_H__

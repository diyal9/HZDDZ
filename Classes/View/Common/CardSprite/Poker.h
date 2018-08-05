//
//  Poker.h
//  PokerTest
//
//  Created by 王体强 on 6/12/14.
//
//  Changed by Jacky 
/*
 此文件包含三个类，CCNumber类暂时没有用到，可以直接作为一个整型对象放入Vector<PokerObject*>
 PokerObject 是poker的数据类，用于存放花色，点数以及是否为癞子的状态。
 PokerUtil 用途：
 1、服务器与本地数据之间的格式转换
 2、char*数组与Vector<PokerObject*>之间的转换
 3、char*数组排序与Vector<PokerObject*>数组排序
 4、通过一个uint8_t的值获取响应的点数和花色
 5、Vector<PokerObject*>的一些处理（有与Vector<PokerObject*>功能重合的地方,由于其他地方有用到，还没来得及改动）
 */
#ifndef __VIEW_COMMON_CARDSPRITE_POKER__
#define __VIEW_COMMON_CARDSPRITE_POKER__

#include <iostream>
#include "cocos2d.h"

#include "../PokerLogic/PokerConstants.h"

using namespace std;
using namespace cocos2d;

class PokerUtil;
class PokerObject : public cocos2d::Ref
{

    PokerObject();
    
    virtual ~PokerObject();
public:
    static PokerObject* create(int pokerValue);
    
    bool init(int pokerValue);
    
    uint8_t getPokerColor();	//获取花色
    
    uint8_t getPokerNum();	//获取点数	
    
    uint8_t getPokerValue();	//获取牌值
    
    bool getLaiziStatue();	//是否是癞子牌

	void changColorToLaizi(); //改变当前牌的花色为癞子花色，但不改变牌值
protected:
    uint8_t m_color;//花色
    
    uint8_t m_pokerNum;//点数
    
    uint8_t m_pokerValue;//牌值
    
};

class PokerUtil
{
public:
    /**
     *  检查牌是否合法
     *
     *  @param pokerData 需要检测的数据
     *
     *  @return 返回是否合法
     */
    static bool checkDataLegal(kPokerUint_8 &pokerData);
    
    /**
     * 服务器传过来的数据调用这个方法
     */
    static cocos2d::Vector<PokerObject*> getSeverData(unsigned char* data, int lenth);
    
    /**
     *  将本地的规则转换为服务器的规则(特指大小王点数)
     *  传给服务器的时候调用
     */
    static kPokerUint_8 changeDataToSever(kPokerUint_8 &pokerData);

    /**
     *  从服务器获取到的牌数据，转换为本地处理的规则
     *
     *  @param 服务器获取到的牌
     *
     *  @return 返回转换过后的牌
     */
	static kPokerUint_8 getDataFromSever(kPokerUint_8& pokerdata);

	/**
     *  通过poker的值获取牌的点数
     *
     *  @param pokerValue poker的值
     *
     *  @return 返回牌的点数
     */
	static uint8_t getPokerNum(uint8_t pokerValue);

	/**
     *  通过poker值获取到花色
     *
     *  @param pokerValue poker值
     *
     *  @return 花色
     */
	static uint8_t getPokerColor(uint8_t pokerValue);

    /**
     *  排序从小到大
     *
     *  @param array 需要排序的uint8_t数组
     *
     *  @return 牌需要的数组
     */
	static kPokerUint_8 sortArrayStoB(kPokerUint_8 &array);
	
    /**
     *  获取一手牌的点数
     *
     *  @param pokers 一手牌的数据
     *
     *  @return 手牌点数
     */
    static kPokerUint_8 getPokerNumbers(const kPokerUint_8 &pokers);

    /**
     *  从手牌数组对象中获取到牌
     */
    static kPokerUint_8 getPokersFromArr(cocos2d::Vector<PokerObject*> pokerArr);

    /**
     *  通过数组对象获取一手牌的点数
     *
     *  @param pokerArr 一手牌的对象数组
     *
     *  @return 手牌点数数组
     */
    static kPokerUint_8 getPokerNumbersFromArr(cocos2d::Vector<PokerObject*> pokerArr);

    /**
     *  通过手牌数组得到数据
     *
     *  @param pokersArr 手牌对象数组
     *
     *  @return返回手牌对象
     */
    static cocos2d::Vector<PokerObject*> getPokerArrFromPokers(const kPokerUint_8 &pokers);

    /**
     *  冒泡排序数组对象，根据点数来
     *
     *  @param pokerArr <#pokerArr description#>
     *
     *  @return <#return value description#>
     */
    static cocos2d::Vector<PokerObject*> sortArrayStoB(cocos2d::Vector<PokerObject*> pokerArr);

    /**
     *  由大到小
     *
     *  @param pokerArr <#pokerArr description#>
     *
     *  @return <#return value description#>
     */
    static cocos2d::Vector<PokerObject*> sortArrayBtoS(cocos2d::Vector<PokerObject*> pokerArr);
	
    /**
     *  将三带一的牌型排序
     *
     *  @param pokerArr 需要排序的 PokerObject 数组
     *
     *  @return 主牌在前副牌在后的 PokerObject 数组
     */
	static cocos2d::Vector<PokerObject*> sortThreeTake(cocos2d::Vector<PokerObject*> pokerArr);
	
    /**
     *  将四带二的牌型排序
     *
     *  @param pokerArr 需要排序的 PokerObject 数组
     *
     *  @return 主牌在前副牌在后的 PokerObject 数组
     */
	static cocos2d::Vector<PokerObject*> sortFourTake(cocos2d::Vector<PokerObject*> pokerArr);

    /**
     *  随机生成一副牌
     *
     *  @return 返回生成的牌的数据
     */
    static kPokerUint_8 createAllPokers();

    /**
     *  洗牌
     *
     *  @param pokers 要洗的牌
     *
     *  @return 洗后的牌
     */
    static kPokerUint_8 randPokers( kPokerUint_8 pokers);

    /**
     *  从一个副牌中拿出想要的张数
     *
     *  @param pokers 一副牌
     *  @param count  要拿的牌数
     *
     *  @return 需要的牌
     */
    static kPokerUint_8 getPokers(const kPokerUint_8 pokers, int count);
    
    static cocos2d::Vector<PokerObject*> copyWithPokerArr(cocos2d::Vector<PokerObject*> pokerArr);
    
    /**
     *  移除数组尾部的对象
     *
     *  @param arr   要移除的数组
     *  @param count 需要移除的尾部对象个数
     *
     *  @return 处理后的数组
     */
    static cocos2d::Vector<PokerObject*> removeLastObjects(cocos2d::Vector<PokerObject*> arr, int count);
    
    static kPokerUint_8 removeLastObjects(kPokerUint_8, int count);
    
    static cocos2d::Vector<PokerObject*> getLastObjects(cocos2d::Vector<PokerObject*> arr, int count);
    
    static kPokerUint_8 getLastObjects(kPokerUint_8 pokerData, int count);
    
	/////////////////////////////////////////////////////////////////
	/************************ constants 相关 ***********************/
	/////////////////////////////////////////////////////////////////

	/**
	 *  判断是否为炸弹和火箭
	 *
	 *  @param weightValue 传入的牌的权值
	 *
	 *  @return 返回结果true为炸弹，false为不是炸弹
	 */
	static bool isBomb(kWeightValue weightValue);
	
	/**
	 *  判断一手牌是否打过另一手
	 *
	 *  @param weightValue       要比较的牌（自己的牌）
	 *
	 *  @param lastWeightValue   需要比较是否打过的牌，也就是上家的出牌
	 *
	 *  @return true为大过，fasle为不大过
	 */
	static bool compareWeightValueIsBig(kWeightValue lastWeightValue, kWeightValue selfWeightValue);

	/**
	 * 对牌值进行从小到大排序
	 * @param datas      牌值
	 * @param dataSize   牌的长度
	 *
	 * @return uint8_t* 排序后牌值
	 */
	static uint8_t* sortUint8StoB(uint8_t* datas, int dataSize);
};


#endif /* defined(__PokerTest__Poker__) */

//
//  PokerCheck.h
//  PokerTest
//
//  Created by 王体强 on 6/13/14.
//
//
/*
 此文件包含一个类PokerTypeCheck
 功能主要有：
 1、牌型检测
 2、将牌按照一定的规则进行排列，比如要同时满足数量多且点数小的排在前（飞机牌型需要如此），利用了Map中int作为key的自增特性，将数量与点数进行位运算直接作为key使用，变包含了两者的信息
具体可以见std::map<int, kPokerUint_8> putPokersToPool(kPokerUint_8 pokerData);里边有详细的注释
 */

#ifndef __PokerTest__PokerCheck__
#define __PokerTest__PokerCheck__

#include <iostream>
#include <map>
#include "../CardSprite/Poker.h"

/*
 此类大多数的运算直接在c数组中进行，效率会高些，之前那个类没有考虑到效率，用Vector<PokerObject*>开销有些大
 */
class PokerTypeCheck
{
public:
    PokerTypeCheck();
    virtual ~PokerTypeCheck();

     /**
     *  检测牌是否相同
     *
     *  @param pokerArrA 牌对象数据A
     *
     *  @param pokerArrB 牌对象数据B
	 *
     *  @return bool 两组牌是否相同
     */
	static bool chekPokersEqual(cocos2d::Vector<PokerObject*> pokerArrA, cocos2d::Vector<PokerObject*> pokerArrB);

    /**
     *  检测牌型
     *
     *  @param pokerArr 牌对象数据
     *
     *  @return 返回牌类型（包含数量、类型以及大小）
     */
    static kWeightValue getPokerType(cocos2d::Vector<PokerObject*> pokerArr);
    
    /**
     *  牌型检测
     *
     *  @param pokerData poker数据
     *
     *  @return 返回牌型
     */
    kWeightValue getPokerType(kPokerUint_8 &pokerData);
    
    /**
     *  将poker类型转换为服务器的
     *
     *  @param weight 本地poker权值
     *
     *  @return 服务器的类型
     */
    static int pokersNumberToService(kWeightValue &weight);
    
    void resetChoose();
    
    void signLaiziPoker(kPokerUint_8 &pokerData);
    
    /**
     *  获取癞子牌个数
     *
     *  @param pokers 手牌数据结构
     *
     *  @return 癞子个数
     */
    int getLaiziCount(kPokerUint_8 &pokers);
    /**
	 *  获取癞子牌个数
	 *  新增这个接口的目的：避免由服务器返回cocos2d::Vector<PokerObject*>数据转换成kPokerUint_8时，
	 *  PokerObject对象的部分属性变化发生损耗，例如癞子花色的变化，由于value值没有变化，
	 *  只有花色变化了，转换成kPokerUint_8时，再去获取花色就不准确了
	 *
	 *  @param pokerArr 手牌对象数组
	 *
	 *  @return 癞子个数
	 */
    int getLaiziCount(cocos2d::Vector<PokerObject*> pokerArr);

    /**
     *  获取一手牌中的癞子牌
     *
     *  @param pokerArr <#pokerArr description#>
     *
     *  @return<#return value description#>
     */
    kPokerUint_8 getLaiziPokers(kPokerUint_8 pokerData, int laiziCount);

    /**
     *  将一副手牌中的癞子移除
     *
     *  @param pokerArr 需要移除的手牌
     *
     *  @return 返回移除癞子的牌数据
     */
    kPokerUint_8 removeLaizi(kPokerUint_8 &pokerData, int laiziCount);

    /**
     *  将牌放入一个容器,容器中的排列规则为：优先按照数量排列,数量多的排前，其次按照点数大小排列，点数小的排在前面
     *
     *  @param pokerData 需要放入的数据
     *
     *  @return 返回这个容器
     */
    
    std::map<int, kPokerUint_8> putPokersToPool(kPokerUint_8 &pokerData);
    
    /**
     *  将牌放入一个容器,容器中的排列规则为：优先按照数量排列,数量少的排前，其次按照点数大小排列，点数小的排在前面
     *
     *  @param pokerData 需要放入的数据
     *
     *  @return 返回这个容器
     */
    
    std::map<int, kPokerUint_8> putPokersToTipsPool(kPokerUint_8 &pokerData);
    
    /**
     *  得出可以出牌的可能性，适用于癞子
     *
     *  @param pokerData 手牌对象数组
     *
     *  @return 返回所有能够打出的手牌数组，里边包含各种可出的手牌
     */
    std::map<int, kPokerUint_8> getAllCanPlay(kPokerUint_8 &pokerData);
    
private:

    /**
     *  判断一手牌中是否点数相同
     *
     *  @param pokerData 传入手牌
     *
     *  @return 返回是否相同，true代表相同，fasle不同
     */
    bool isAllnumSame(kPokerUint_8 &pokerData);

    /**
     *  找出一手牌中相同点数的个数
     *
     *  @param pokerData 传入手牌
     *
     *  @return 返回相同牌张数
     */
    int checkSameCardNum(kPokerUint_8 &pokerData);

    /**
     *  检测三带一和四带二的牌型
     *
     *  @param pokerData 传入手牌
     *
     *  @return 返回牌型
     */
    kWeightValue checkSiDaiPokers(kPokerUint_8 &pokerData);

    /**
     *  顺子检测
     *
     *  @param pokerData 传入手牌
     *
     *  @return 返回牌型
     */
    kWeightValue checkShunzi(kPokerUint_8 &pokerData);

    /**
     *  双顺检测
     *
     *  @param pokerData 传入手牌
     *
     *  @return 返回牌型
     */
    kWeightValue checkShuangShun(kPokerUint_8 &pokerData);

    /**
     *  三顺检测
     *
     *  @param pokerData 传入手牌
     *
     *  @return 返回牌型
     */
    kWeightValue checkSanShun(kPokerUint_8 &pokerData);

    /**
     *  飞机检测
     *
     *  @param pokerData 传入手牌
     *
     *  @return 返回牌型
     */
    kWeightValue checkFeiji(kPokerUint_8 &pokerData);

    /**
     *  检查三带二牌的合法性
     *
     *	@param remainPokers 主牌
     *  @param pokerData 副牌
     *
     *  @return 返回是否合法
     */
    bool checkDaiIsaVaild(kPokerUint_8 remainPokers, kPokerUint_8 &pokerData);

    /**
     *  一手牌中是否包含2
     *
     *  @param pokerData 传入手牌
     *
     *  @return true包含2，false不包含2
     */
    bool isIncludeR2(kPokerUint_8 &pokerData);

    /**
     *  将相同点数的牌放在一起，数量由小到大排列
     *
     *  @param pokerData 传入手牌
     *
     *  @return
     */
    kPokerUint_8 sortArrCountsBtoS(kPokerUint_8 &pokerData);
    
    PokerObject* getPokerObjectAtIndex(kPokerUint_8 pokerData, int index);
    
    
    
    uint8_t** getAllLaiziCombination(int laiziCount);
    
    bool isContainLaizi(kPokerUint_8 &pokerArr);
    
    
    
    cocos2d::Vector<PokerObject*> m_pokerArray;
private:
    int m_laiziCount;
    int m_combinationCount;
    //测试变量‘
    long testTime;
};


#endif /* defined(__PokerTest__PokerCheck__) */

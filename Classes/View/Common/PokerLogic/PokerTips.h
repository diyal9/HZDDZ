//
//  PokerTips.h
//  PokerTest
//
//  Created by diyal on 6/27/14.
//
//  Changed by Jacky
/*
 此文件包含一个类PokerTips
 功能：
 1、提示出牌
 2、检测牌是否可以打出
 3、获取所有可以打出的牌
 */
#ifndef __PokerTest__PokerTips__
#define __PokerTest__PokerTips__

#include <iostream>
#include "PokerCheck.h"

class PokerTips
{
public:
    PokerTips();
    virtual ~PokerTips();
    /**
     *  提示出牌
     *
     *  @param lastTipsPokers   上次提示的牌
     *  @param myPokers         玩家还持有的牌
     *  @param lastPlayerPokers 上家的牌
     *
     *  @return 返回可以出的牌
     */
    static cocos2d::Vector<PokerObject*> getTipsPokers(cocos2d::Vector<PokerObject*> lastTipsPokers, cocos2d::Vector<PokerObject*> myPokers, cocos2d::Vector<PokerObject*> lastPlayerPokers);
    
    /**
     *  点牌提示
     *
     *  @param lastPlayerPokers 上家的牌
     *  @param myPokers         玩家还持有的牌
     *  @param seletPokers      玩家点击选中的牌
     *
     *  @return 返回可以弹起的牌
     */
    static cocos2d::Vector<PokerObject*> getClickTipsPokers(cocos2d::Vector<PokerObject*> lastPlayerPokers, cocos2d::Vector<PokerObject*> myPokers, cocos2d::Vector<PokerObject*> seletPokers);
	
    /**
     *  滑牌提示
     *
     *  @param lastPlayerPokers 上家的牌
     *  @param myPokers         玩家还持有的牌
     *  @param seletPokers       玩家滑动选中的牌
     *
     *  @return 返回可以弹起的单顺
     */
    static cocos2d::Vector<PokerObject*> getMoveTipsPokers( cocos2d::Vector<PokerObject*> myPokers, cocos2d::Vector<PokerObject*> seletPokers);
	
    /**
     *  提示出牌
     *
     *  @param lastPlayerPokers  上家的牌对象
     *  @param seletPokers       玩家选中的牌
     *
     *  @return 返回是否可以出牌
     */
    static bool checkPokerAvaild(cocos2d::Vector<PokerObject*> lastPlayerPokers, cocos2d::Vector<PokerObject*> seletPokers);

    /**
     *  得出可以出牌的可能性，适用于癞子
     *
     *  @param lastPlayerPokers 上家的牌对象
     *  @param selectPokers     玩家选中的牌
     *
     *  @return 返回所有能够打出的手牌数组，里边包含各种可出的手牌
     */
    static std::vector<cocos2d::Vector<PokerObject*>> getAllPokersCanPlay(cocos2d::Vector<PokerObject*> lastPlayerPokers, cocos2d::Vector<PokerObject*> selectPokers);

    /**
     *  将一个手牌中的一定张数的牌加到另一个手牌中
     *
     *  @param currentPokers 手中持有的牌
     *  @param targetPokers  添加的手牌
     *  @param addCount      添加手牌的张数
     *
     *  @return 返回添加后的手牌
     */
    kPokerUint_8 addPokersWith(kPokerUint_8 &currentPokers, kPokerUint_8 &targetPokers, int addCount);
    

    /**
     * 提示
     *
     *  @param lastPlayerPokers 上家牌数据
     *  @param seletPokers      选中牌数据
     *
     *  @return 返回是否可以出牌
     */
    bool checkPokerAvaild(kPokerUint_8 lastPlayerPokers, kPokerUint_8 seletPokers);

    
    /**
     *  将一个手牌中的计划一定张数的牌加到另一个手牌中，若未达到计划值，则有几张加几张。
     *
     *  @param currentPokers 手中持有的牌
     *  @param targetPokers  添加的手牌
     *  @param addCount      添加手牌的张数
     *
     *  @return 返回添加后的手牌
     */
    kPokerUint_8 addPokersWithPlanCount(kPokerUint_8 &currentPokers, kPokerUint_8 &targetPokers, int addCount);

    /**
     *  获取一定数量的牌
     *
     *  @param pokerData 原始牌
     *  @param count     取牌的张数
     *
     *  @return 返回取出来的牌
     */
    kPokerUint_8 getPokersWithCount(kPokerUint_8 &pokerData, int count);
    
    /**
     *  得出可以出牌的可能性，适用于癞子
     *
     *  @param lastPlayerPokers 上家牌数据
     *  @param selectPokers     本家牌数据
     *
     *  @return 返回所有能够打出的手牌数组，里边包含各种可出的手牌
     */
    std::map<int, kPokerUint_8> getAllPokersCanPlay(kPokerUint_8 &lastPlayerPokers, kPokerUint_8 &selectPokers);
    

private:

    /**
     * 从手牌中选出大于上家的 count 张数的牌
     * @param myPokers 我的手牌
     */
    kPokerUint_8 getCountSamePoker(int pokerPoint, int count, int lastPoint);

    /**
     * 重置手牌的癞子数据
     * 在kPokerUint_8 getXXX()方法之前使用，每次都要重置，否则会影响牌型检测结果
     * @param myPokers 我的手牌
     */
    void resetLaiziData(kPokerUint_8 &myPokers);

    /**
     *  将牌按照点数大小，相同的牌放入一个容器中
     *
     *  @param myPokers 手中持有的牌
     */
    void putSameCardToPool(kPokerUint_8 &myPokers);

    /**
     *  根据容器中的牌拿到点数
     *
     *  @param myPokers 手中持有的牌
     *
     *  @return 返回牌的点数
     */
    uint8_t getPokerPoint(kPokerUint_8 &myPokers);

    /**
     *  标记一些牌的状态为已选
     *
     *  @param somePokers 需要标记的牌
     */
    void signPokerStatue(kPokerUint_8 &somePokers);

    /**
     *  标记一些点数已经被选中
     *
     *  @param somePokers 需要标记的牌
     */
    void signPokerPointSatue(kPokerUint_8 &somePokers);

    /**
     *  根据somePokers中的癞子来减去总癞子数，用于带牌的时候不多加癞子
     *
     *  @param somePokers 当前选中的牌
     */
    void lessLaiziCount(kPokerUint_8 &somePokers);
    
    kPokerUint_8 getPokersWithUnKnow();
    
    kPokerUint_8 getOnePoker(int pokerPoint, int lastPoint = 0);
    
    kPokerUint_8 getTwoPokers(int pokerPoint, int lastPoint = 0);
    
    kPokerUint_8 getThreePokers(int pokerPoint, int lastPoint = 0);
    
    kPokerUint_8 getSanDai(int pokerPoint,int pokerCount);
    
    /**
     * 根据点数返回大于上家牌的最小匹配的炸弹牌型
     * @param pokerPoint 上家牌的大小
     * @param type 炸弹类型  ( T_Rocket_2 > T_BOMB_L > T_BOMB > T_BOMB_S )(王炸 > 癞子炸 > 硬炸 > 软炸)
     * 				T_Unknow：       返回最小匹配的炸弹（最小的炸弹，按炸弹的权值查找）
     * 				T_BOMB_S：       返回大于牌值为pokerPoint的软炸的最小匹配的炸弹(可能返回：zero_pokerData，软炸，硬炸，癞子炸，王炸)
     * 				T_BOMB：            返回大于牌值为pokerPoint的硬炸的最小匹配的炸弹(可能返回：zero_pokerData，硬炸，癞子炸，王炸)
     * 				T_BOMB_L：       返回大于牌值为pokerPoint的癞子炸的最小匹配的炸弹（可能返回：zero_pokerData，王炸     注：一副牌中只能存在一副癞子炸，别人出了，我们就检测不出来了）
     * 				T_Rocket_2： 返回大于牌值为pokerPoint的王炸的最小匹配的炸弹（只能返回：zero_pokerData   注：一副牌中只能存在一副王炸，别人出了，我们就检测不出来了）
	 * @return 牌型
     */
    kPokerUint_8 getBombPokers(int pokerPoint, int type);
    
    kPokerUint_8 getRocketPokers();
    
    kPokerUint_8 getShunzi(int pokerPoint, int pokerCount);
    
    kPokerUint_8 getShuangShun(int pokerPoint, int pokerCount);
    
    kPokerUint_8 getSanShun(int pokerPoint, int pokerCount);
    
    kPokerUint_8 getFeijiDan(int pokerPoint, int pokerCount);
    
    kPokerUint_8 getFeijiShuang(int pokerPoint, int pokerCount);
    

	// 点牌提示相关
    kPokerUint_8 getTwoPokersByPart(int pokerPoint);
    
    kPokerUint_8 getThreePokersByPart(int pokerPoint);
    
    kPokerUint_8 getSanDaiByPart(int pokerPoint,int pokerCount);
    
    /**
     * 根据点数返回当前点数的最小匹配的炸弹牌型
     * @param pokerPoint 当前牌的大小
     * @param type 炸弹类型  ( T_Rocket_2 > T_BOMB_L > T_BOMB > T_BOMB_S )(王炸 > 癞子炸 > 硬炸 > 软炸)
     * 				T_Unknow：       返回最小匹配的炸弹（最小的炸弹，按炸弹的权值查找）
     * 				T_BOMB_S：       返回大于牌值为pokerPoint的软炸的最小匹配的炸弹(可能返回：zero_pokerData，软炸，硬炸，癞子炸，王炸)
     * 				T_BOMB：            返回大于牌值为pokerPoint的硬炸的最小匹配的炸弹(可能返回：zero_pokerData，硬炸，癞子炸，王炸)
     * 				T_BOMB_L：       返回大于牌值为pokerPoint的癞子炸的最小匹配的炸弹（可能返回：zero_pokerData，王炸     注：一副牌中只能存在一副癞子炸，别人出了，我们就检测不出来了）
     * 				T_Rocket_2： 返回大于牌值为pokerPoint的王炸的最小匹配的炸弹（只能返回：zero_pokerData   注：一副牌中只能存在一副王炸，别人出了，我们就检测不出来了）
	 * @return 牌型
     */
    kPokerUint_8 getBombPokersByPart(int pokerPoint, int type);
    
    kPokerUint_8 getShunziByPart(int pokerPoint, int pokerCount);
    
    kPokerUint_8 getShuangShunByPart(int pokerPoint, int pokerCount);
    
    kPokerUint_8 getSanShunByPart(int pokerPoint, int pokerCount);
    
    kPokerUint_8 getFeijiDanByPart(int pokerPoint, int pokerCount);
    
    kPokerUint_8 getFeijiShuangByPart(int pokerPoint, int pokerCount);
    
private:
    
    std::map<uint8_t, kPokerUint_8> __pokerPool;//点数由小到大的池
    
    std::map<int, kPokerUint_8> __sbCountPool;//数量由小到大的池
    
    std::map<uint8_t, bool> __pokerStatue;//牌被选中的状态
    
    std::map<uint8_t, bool> __pokerPointStatue;//牌的点数被选中的状态
    
    kPokerUint_8  __laiziPokerData;//当前一组牌中所有的癞子牌
    
    int __laiziCount;//癞子个数
    
    
    PokerTypeCheck* __check;//牌型检测器
    
    
    
};


#endif /* defined(__PokerTest__PokerTips__) */

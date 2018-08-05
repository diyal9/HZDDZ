#ifndef BaseCore_WWBTools_PokerConstants_h
#define BaseCore_WWBTools_PokerConstants_h

//
//  PokerConstain.h
//  PokerTest
//
//  Created by 王体强 on 6/12/14.
//	
// 	Changed by Jacky
//

using namespace std;

/* 牌型定义 */
static uint8_t const TYPE_F = 1;//方块
static uint8_t const TYPE_M = 2;//梅花
static uint8_t const TYPE_H = 3;//红桃
static uint8_t const TYPE_B = 4;//黑桃
static uint8_t const TYPE_W = 5;//王牌
static uint8_t const TYPE_L = 1<<3;//癞子

/* 点数定义 */
static uint8_t const R3 = 1;//3点
static uint8_t const R4 = 2;//4点
static uint8_t const R5 = 3;
static uint8_t const R6 = 4;
static uint8_t const R7 = 5;
static uint8_t const R8 = 6;
static uint8_t const R9 = 7;
static uint8_t const R10 = 8;
static uint8_t const RJ = 9;//J
static uint8_t const RQ = 10;//Q
static uint8_t const RK = 11;//K
static uint8_t const R1 = 12;//A
static uint8_t const R2 = 13;//2
static uint8_t const R_WA = 14;//WA 小

static uint8_t const R_WB = 15;//WB 大


/*牌类型*/
typedef enum pokerType
{
    T_Unknow = 0,
    T_ONE = 1,//单张
    T_TWO = 2,//一对     11
    T_THREE = 3,//三张    111
    T_BOMB = 4,//炸弹 (硬炸)  1111
    T_BOMB_S = 5,//炸弹 (软炸)  1111(至少带有一张癞子但又不全是癞子的炸弹)
    T_BOMB_L = 6,//炸弹 (癞子炸)  1111(全都是癞子的炸弹)
    
    T_SEQ_1 = 7,//单顺34567       顺子
    T_SEQ_2 = 8,//双顺 334455     连对
    T_SEQ_3 = 9,//三顺333444      三连对
    
    T_THREE_1 = 10,//三带一 1113  或 三连对带一  33344412
    T_THREE_2 = 11,//三带对 11133 或 三连对带对  3334441122
    
    T_FOUR_2 = 12,//四条+两张单牌     四带两单
    T_FOUR_4 = 13,//四带二对(带一对)  四带一对或两对
    
    T_Rocket_2 = 14,//双王火箭
}kPokerType;


/**
 *  牌的权值，包含牌的类型，牌的大小，牌的张数，用于比牌判断
 */
typedef struct
{
    int type;           //牌的类型

    int number;         //牌的点数值，取牌型的最小点数，在类型和张数一样的情况下，可用于比较牌的大小

    int pokerCount;     //牌的张数

}kWeightValue;


/**
 *  uint8_t基本信息
 */
typedef struct
{
    uint8_t* data;      //牌的数值

    bool isAlloc;       //牌是否动态创建，用于避免内存溢出

    int lenth;          //牌的张数

}kPokerUint_8;


/**
 *  生成一个kPokerUint_8结构体
 *
 *  @param data    牌型的牌值
 *  @param lenth   牌型长度
 *
 *  @return kPokerUint_8结构体
 */
inline kPokerUint_8 PokerUint8_tMake(uint8_t* data, int lenth)
{
    kPokerUint_8 pokerData;
    pokerData.data = data;
    pokerData.lenth = lenth;
    pokerData.isAlloc = false;
    return pokerData;
}

/**
 *  生成一个kPokerUint_8结构体
 *
 *  @param data    牌型的牌值
 *  @param lenth   牌型长度
 * 	@param isAlloc 是否有malloc，为了释放的时候方便
 *
 * 	@return kPokerUint_8结构体
 *
 */
inline kPokerUint_8 PokerUint8_tMake(uint8_t* data, int lenth, bool isAlloc)
{
    kPokerUint_8 pokerData;
    pokerData.data = data;
    if (data == nullptr)
    {
        lenth = 0;
    }
    pokerData.lenth = lenth;
    pokerData.isAlloc = isAlloc;
    return pokerData;
}

/**
 *  生成一个牌的权值
 *
 *  @param type   牌的类型
 *  @param number 牌的点数
 *  @param count  牌的张数
 *
 *  @return 返回一个牌的权值
 */
inline kWeightValue WWWeightValueMake(int type, int number, int count)
{
    kWeightValue cardType;
    cardType.type = type;
    cardType.number = number;
    cardType.pokerCount = count;
    return cardType;
}

const static kWeightValue weight_Unknown = WWWeightValueMake(0, 0, 0);         //无效牌的权值

const static kPokerUint_8 zero_pokerData = PokerUint8_tMake(nullptr, 0, false);    //无效牌的kPokerUint_8结构体

/**
 *  判断是否为无效的牌
 *
 *  @param weightValue 传入的牌的权值
 *
 *  @return 返回结果true为有效牌，false为无效牌
 */
inline bool isAvaildWeightValue(kWeightValue weightValue)
{
    if (weightValue.type == weight_Unknown.type || weightValue.number == weight_Unknown.number || weightValue.pokerCount == weight_Unknown.pokerCount)
    {
        return false;
    }
    return true;
}

/**
 *  释放poker数据
 */
inline void delePokerData(kPokerUint_8 &pokerData)
{
    if (pokerData.isAlloc&&pokerData.data!=nullptr)
    {
        free(pokerData.data);
        pokerData.data = nullptr;
    }
    pokerData.lenth = 0;
    pokerData.isAlloc = false;
}


/**
 *  打印一个uint8_t数组
 *
 *  @param strInfo 增加的打印消息说明
 *  @param data    被打印的数组
 */
inline void printUint8_t(const char* strInfo,const kPokerUint_8 pokers)
{
    CCLOG("%s--{\n",strInfo);
    for (int i=0; i<pokers.lenth; i++)
    {
        CCLOG("%d,\n",pokers.data[i]);
    }
    CCLOG("}\n");
    
}

inline void printPokerNum(const char* strInfo,const kPokerUint_8 pokers)
{
    CCLOG("%s--pokerNum--{\n",strInfo);
    for (int i=0; i<pokers.lenth; i++)
    {
        int j = pokers.data[i]%16;
        CCLOG("%d,\n",j);
    }
    CCLOG("}\n");
}



#endif

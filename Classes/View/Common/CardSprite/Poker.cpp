//
//  Poker.cpp
//  PokerTest
//
//  Created by 王体强 on 6/12/14.
//
//

#include "Poker.h"

USING_NS_CC;

extern int getLaiziNumber();

/*Poker*/
PokerObject::PokerObject():
	m_color(0),
	m_pokerNum(0),
	m_pokerValue(0)
{
}

PokerObject::~PokerObject()
{
    m_color = 0;
    m_pokerNum = 0;
    m_pokerValue = 0;
}

uint8_t PokerObject::getPokerColor()
{
    return m_color;
}

uint8_t PokerObject::getPokerNum()
{
    return m_pokerNum;
}

uint8_t PokerObject::getPokerValue()
{
    return m_pokerValue;
}

void PokerObject::changColorToLaizi()
{
	m_color = m_color | TYPE_L;
}

bool PokerObject::getLaiziStatue()
{
    bool isLaizi = false;
    if (m_pokerNum == getLaiziNumber())
    {
        isLaizi = true;
    }
    else
    {
        isLaizi = false;
    }
    return isLaizi;
}

bool PokerObject::init(int pokerValue)
{
    m_pokerValue = pokerValue;
    m_pokerNum = PokerUtil::getPokerNum(pokerValue);
    m_color = PokerUtil::getPokerColor(pokerValue);

    if (m_pokerNum == getLaiziNumber())
    {
        m_color = m_color|TYPE_L;
    }
    
    return true;
}

PokerObject* PokerObject::create(int pokerValue)
{
    PokerObject *poker = new PokerObject();
    if (poker&&poker->init(pokerValue))
    {
        poker->autorelease();
        return poker;
    }
    CC_SAFE_DELETE(poker);
    return nullptr;
    
}

/*PokerUtil*/
uint8_t PokerUtil::getPokerNum(uint8_t pokerValue)
{
    CCAssert(pokerValue > 16, "pokerValue must be bigger than 16");
    uint8_t num = pokerValue%16;
    return num;
}

uint8_t PokerUtil::getPokerColor(uint8_t pokerValue)
{
    CCAssert(pokerValue > 16, "pokerValue must be bigger than 16");
    uint8_t color = pokerValue / 16;
    return color;
}

kPokerUint_8 PokerUtil::sortArrayStoB(kPokerUint_8 &array)
{
    uint8_t* arrayData = array.data;
    int lenth = array.lenth;
    for (int i = 0; i < lenth; i++)
    {
        for (int j = i + 1; j < lenth; j++)
        {
            uint8_t num1 = arrayData[i];
            uint8_t num2 = arrayData[j];
            if (num1%16 > num2%16)
            {
                uint8_t temp = num2;
                arrayData[j] = num1;
                arrayData[i] = temp;
            }
        }
    }
    
    return PokerUint8_tMake(arrayData,lenth);
}

kPokerUint_8 PokerUtil::getPokerNumbers(const kPokerUint_8 &pokers)
{
    int lenth = pokers.lenth;
    uint8_t* numbers = (uint8_t*)malloc(lenth);
    for (int i = 0; i < lenth; i++)
    {
        numbers[i] = getPokerNum(pokers.data[i]);
    }
    return PokerUint8_tMake(numbers,lenth,true);
    
}

kPokerUint_8 PokerUtil::getPokerNumbersFromArr(Vector<PokerObject*> pokerArr)
{
    uint8_t* pokers = (uint8_t*)malloc(pokerArr.size());
    int count = pokerArr.size();
    for (int i = 0; i < count; i++)
    {
        PokerObject* poker = pokerArr.at(i);
        pokers[i] = poker->getPokerNum();
    }
    CCAssert(pokers != nullptr, "pokers must be non null");
    
    kPokerUint_8 pokerNums = PokerUint8_tMake(pokers, count, true);
    return pokerNums;
}



cocos2d::Vector<PokerObject*> PokerUtil::getPokerArrFromPokers(const kPokerUint_8 &pokers)
{
   /* printUint8_t("getPokerArrFromPokers", pokers);
    printPokerNum("getPokerArrFromPokers", pokers);*/
    Vector<PokerObject*> pokerArr;
    int count = pokers.lenth;
    for (int i = 0; i < count; i++)
    {
        PokerObject* poker = PokerObject::create(pokers.data[i]);
        pokerArr.pushBack(poker);
    }
    return pokerArr;
}

cocos2d::Vector<PokerObject*> PokerUtil::sortArrayStoB(Vector<PokerObject*> pokerArr)
{
    int count = pokerArr.size();
    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            PokerObject* poker = pokerArr.at(i);
            PokerObject* poker1 = pokerArr.at(j);
            if (poker->getPokerNum() > poker1->getPokerNum())
            {
//                pokerArr->exchangeObject(poker, poker1);
            	pokerArr.replace(i, poker1);
            	pokerArr.replace(j, poker);
            }
            
        }
    }
    return pokerArr;
}

cocos2d::Vector<PokerObject*> PokerUtil::sortArrayBtoS(cocos2d::Vector<PokerObject*> pokerArr)
{
    int count = pokerArr.size();
    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            PokerObject* poker = pokerArr.at(i);
            PokerObject* poker1 = pokerArr.at(j);
            if (poker->getPokerNum() < poker1->getPokerNum())
            {
//                pokerArr->exchangeObject(poker, poker1);
            	pokerArr.replace(i, poker1);
            	pokerArr.replace(j, poker);
            }
            
        }
    }
    return pokerArr;
}


cocos2d::Vector<PokerObject*> PokerUtil::sortThreeTake(cocos2d::Vector<PokerObject*> pokerArr)
{
	if( pokerArr.size() < 6 ){  // 1113 或 11133
		if( pokerArr.at(0)->getPokerNum() != pokerArr.at(2)->getPokerNum() )
		{
			if( pokerArr.size() == 4 ){
				PokerObject* poker = pokerArr.at(0);
				pokerArr.replace(0, pokerArr.at(1));
				pokerArr.replace(1, pokerArr.at(2));
				pokerArr.replace(2, pokerArr.at(3));
				pokerArr.replace(3, poker);
			}else{
				PokerObject* poker = pokerArr.at(0);
				PokerObject* poker1 = pokerArr.at(1);
				pokerArr.replace(0, pokerArr.at(2));
				pokerArr.replace(1, pokerArr.at(3));
				pokerArr.replace(2, pokerArr.at(4));
				pokerArr.replace(3, poker);
				pokerArr.replace(4, poker1);
			}
		}
	}else if( pokerArr.size() < 11 ){ // 33344412 或者 3334441122
		if( pokerArr.size() == 8 ){ 
			if( pokerArr.at(0)->getPokerNum() != pokerArr.at(2)->getPokerNum() )
			{
				if( pokerArr.at(1)->getPokerNum() != pokerArr.at(2)->getPokerNum() ) //65444333
				{
					PokerObject* poker = pokerArr.at(0);
					PokerObject* poker1 = pokerArr.at(1);
					pokerArr.replace(0, pokerArr.at(2));
					pokerArr.replace(1, pokerArr.at(3));
					pokerArr.replace(2, pokerArr.at(4));
					pokerArr.replace(3, pokerArr.at(5));
					pokerArr.replace(4, pokerArr.at(6));
					pokerArr.replace(5, pokerArr.at(7));
					pokerArr.replace(6, poker);
					pokerArr.replace(7, poker1);
				}else{ //65554443
					PokerObject* poker = pokerArr.at(0);
					pokerArr.replace(0, pokerArr.at(1));
					pokerArr.replace(1, pokerArr.at(2));
					pokerArr.replace(2, pokerArr.at(3));
					pokerArr.replace(3, pokerArr.at(4));
					pokerArr.replace(4, pokerArr.at(5));
					pokerArr.replace(5, pokerArr.at(6));
					pokerArr.replace(6, poker);
				}
			}
		}else{ //3334441122
			if( pokerArr.at(0)->getPokerNum() != pokerArr.at(2)->getPokerNum() )
			{
				if( pokerArr.at(2)->getPokerNum() != pokerArr.at(4)->getPokerNum() ) //6655444333
				{
					PokerObject* poker = pokerArr.at(0);
					PokerObject* poker1 = pokerArr.at(1);
					PokerObject* poker2 = pokerArr.at(2);
					PokerObject* poker3 = pokerArr.at(3);
					pokerArr.replace(0, pokerArr.at(4));
					pokerArr.replace(1, pokerArr.at(5));
					pokerArr.replace(2, pokerArr.at(6));
					pokerArr.replace(3, pokerArr.at(7));
					pokerArr.replace(4, pokerArr.at(8));
					pokerArr.replace(5, pokerArr.at(9));
					pokerArr.replace(6, poker);
					pokerArr.replace(7, poker1);
					pokerArr.replace(8, poker2);
					pokerArr.replace(9, poker3);
				}else{ //6655544433
					PokerObject* poker = pokerArr.at(0);
					PokerObject* poker1 = pokerArr.at(1);
					pokerArr.replace(0, pokerArr.at(2));
					pokerArr.replace(1, pokerArr.at(3));
					pokerArr.replace(2, pokerArr.at(4));
					pokerArr.replace(3, pokerArr.at(5));
					pokerArr.replace(4, pokerArr.at(6));
					pokerArr.replace(5, pokerArr.at(7));
					pokerArr.replace(6, poker);
					pokerArr.replace(7, poker1);
				}
			}
		}
	}else{ // 333444555123 或者 333444555112233
		if( pokerArr.size() == 12 ){ 
			if( pokerArr.at(0)->getPokerNum() != pokerArr.at(2)->getPokerNum() )
			{
				if( pokerArr.at(1)->getPokerNum() == pokerArr.at(2)->getPokerNum() ) // 6 555444333 21
				{
					PokerObject* poker = pokerArr.at(0);
					pokerArr.replace(0, pokerArr.at(1));
					pokerArr.replace(1, pokerArr.at(2));
					pokerArr.replace(2, pokerArr.at(3));
					pokerArr.replace(3, pokerArr.at(4));
					pokerArr.replace(4, pokerArr.at(5));
					pokerArr.replace(5, pokerArr.at(6));
					pokerArr.replace(6, pokerArr.at(7));
					pokerArr.replace(7, pokerArr.at(8));
					pokerArr.replace(8, pokerArr.at(9));
					pokerArr.replace(9, poker);
				}else if( pokerArr.at(2)->getPokerNum() == pokerArr.at(3)->getPokerNum() ){ // 76 555444333 2
					PokerObject* poker = pokerArr.at(0);
					PokerObject* poker1 = pokerArr.at(1);
					pokerArr.replace(0, pokerArr.at(2));
					pokerArr.replace(1, pokerArr.at(3));
					pokerArr.replace(2, pokerArr.at(4));
					pokerArr.replace(3, pokerArr.at(5));
					pokerArr.replace(4, pokerArr.at(6));
					pokerArr.replace(5, pokerArr.at(7));
					pokerArr.replace(6, pokerArr.at(8));
					pokerArr.replace(7, pokerArr.at(9));
					pokerArr.replace(8, pokerArr.at(10));
					pokerArr.replace(9, poker);
					pokerArr.replace(10, poker1);
				}else{ // 876 555444333
					PokerObject* poker = pokerArr.at(0);
					PokerObject* poker1 = pokerArr.at(1);
					PokerObject* poker2 = pokerArr.at(2);
					pokerArr.replace(0, pokerArr.at(3));
					pokerArr.replace(1, pokerArr.at(4));
					pokerArr.replace(2, pokerArr.at(5));
					pokerArr.replace(3, pokerArr.at(6));
					pokerArr.replace(4, pokerArr.at(7));
					pokerArr.replace(5, pokerArr.at(8));
					pokerArr.replace(6, pokerArr.at(9));
					pokerArr.replace(7, pokerArr.at(10));
					pokerArr.replace(8, pokerArr.at(11));
					pokerArr.replace(9, poker);
					pokerArr.replace(10, poker1);
					pokerArr.replace(11, poker1);
				}
			}
		}else{ //333444555112233
			if( pokerArr.at(0)->getPokerNum() != pokerArr.at(2)->getPokerNum() )
			{
				if( pokerArr.at(2)->getPokerNum() == pokerArr.at(4)->getPokerNum() ) // 66 555444333 2211
				{
					PokerObject* poker = pokerArr.at(0);
					PokerObject* poker1 = pokerArr.at(1);
					pokerArr.replace(0, pokerArr.at(2));
					pokerArr.replace(1, pokerArr.at(3));
					pokerArr.replace(2, pokerArr.at(4));
					pokerArr.replace(3, pokerArr.at(5));
					pokerArr.replace(4, pokerArr.at(6));
					pokerArr.replace(5, pokerArr.at(7));
					pokerArr.replace(6, pokerArr.at(8));
					pokerArr.replace(7, pokerArr.at(9));
					pokerArr.replace(8, pokerArr.at(10));
					pokerArr.replace(9, poker);
					pokerArr.replace(10, poker1);
				}else if( pokerArr.at(4)->getPokerNum() == pokerArr.at(6)->getPokerNum() ){ // 7766 555444333 22
					PokerObject* poker = pokerArr.at(0);
					PokerObject* poker1 = pokerArr.at(1);
					PokerObject* poker2 = pokerArr.at(2);
					PokerObject* poker3 = pokerArr.at(3);
					pokerArr.replace(0, pokerArr.at(4));
					pokerArr.replace(1, pokerArr.at(5));
					pokerArr.replace(2, pokerArr.at(6));
					pokerArr.replace(3, pokerArr.at(7));
					pokerArr.replace(4, pokerArr.at(8));
					pokerArr.replace(5, pokerArr.at(9));
					pokerArr.replace(6, pokerArr.at(10));
					pokerArr.replace(7, pokerArr.at(11));
					pokerArr.replace(8, pokerArr.at(12));
					pokerArr.replace(9, poker);
					pokerArr.replace(10, poker1);
					pokerArr.replace(11, poker2);
					pokerArr.replace(12, poker3);
				}else{ // 887766 555444333
					PokerObject* poker = pokerArr.at(0);
					PokerObject* poker1 = pokerArr.at(1);
					PokerObject* poker2 = pokerArr.at(2);
					PokerObject* poker3 = pokerArr.at(3);
					PokerObject* poker4 = pokerArr.at(4);
					PokerObject* poker5 = pokerArr.at(5);
					pokerArr.replace(0, pokerArr.at(6));
					pokerArr.replace(1, pokerArr.at(7));
					pokerArr.replace(2, pokerArr.at(8));
					pokerArr.replace(3, pokerArr.at(9));
					pokerArr.replace(4, pokerArr.at(10));
					pokerArr.replace(5, pokerArr.at(11));
					pokerArr.replace(6, pokerArr.at(12));
					pokerArr.replace(7, pokerArr.at(13));
					pokerArr.replace(8, pokerArr.at(14));
					pokerArr.replace(9, poker);
					pokerArr.replace(10, poker1);
					pokerArr.replace(11, poker2);
					pokerArr.replace(12, poker3);
					pokerArr.replace(13, poker4);
					pokerArr.replace(14, poker5);
				}
			}
		}
	}
	
    return pokerArr;
}
	

cocos2d::Vector<PokerObject*> PokerUtil::sortFourTake(cocos2d::Vector<PokerObject*> pokerArr)
{
	
	if( pokerArr.at(0)->getPokerNum() != pokerArr.at(3)->getPokerNum() )
	{
		if( pokerArr.size() == 6 ){  //四带两张单牌
			if( pokerArr.at(1)->getPokerNum() != pokerArr.at(3)->getPokerNum() ){ //543333
				PokerObject* poker = pokerArr.at(0);
				PokerObject* poker1 = pokerArr.at(1);
				pokerArr.replace(0, pokerArr.at(2));
				pokerArr.replace(1, pokerArr.at(3));
				pokerArr.replace(2, pokerArr.at(4));
				pokerArr.replace(3, pokerArr.at(5));
				pokerArr.replace(4, poker);
				pokerArr.replace(5, poker1);
			}else{ //544443
				PokerObject* poker = pokerArr.at(0);
				pokerArr.replace(0, pokerArr.at(1));
				pokerArr.replace(1, pokerArr.at(2));
				pokerArr.replace(2, pokerArr.at(3));
				pokerArr.replace(3, pokerArr.at(4));
				pokerArr.replace(4, poker);
			}
		}else{   //四带两对
           if( pokerArr.at(3)->getPokerNum() != pokerArr.at(4)->getPokerNum() ){ //55443333
				PokerObject* poker = pokerArr.at(0);
				PokerObject* poker1 = pokerArr.at(1);
				PokerObject* poker2 = pokerArr.at(2);
				PokerObject* poker3 = pokerArr.at(3);
				pokerArr.replace(0, pokerArr.at(4));
				pokerArr.replace(1, pokerArr.at(5));
				pokerArr.replace(2, pokerArr.at(6));
				pokerArr.replace(3, pokerArr.at(7));
				pokerArr.replace(4, poker);
				pokerArr.replace(5, poker1);
				pokerArr.replace(6, poker2);
				pokerArr.replace(7, poker3);
			}else{ //55444433
				PokerObject* poker = pokerArr.at(0);
				PokerObject* poker1 = pokerArr.at(1);
				pokerArr.replace(0, pokerArr.at(2));
				pokerArr.replace(1, pokerArr.at(3));
				pokerArr.replace(2, pokerArr.at(4));
				pokerArr.replace(3, pokerArr.at(5));
				pokerArr.replace(4, poker);
				pokerArr.replace(5, poker1);
			}
		}
	}
	
    return pokerArr;
}


kPokerUint_8 PokerUtil::createAllPokers()
{
    uint8_t* pokers = (uint8_t*)malloc(54);
    int index = 0;
    for (int i = 1; i < R_WA; i++)
    {
        for (int j = 1; j < TYPE_W; j++)
        {
            uint8_t num = i;
            uint8_t color = j<<4;
            uint8_t value = num | color;
            pokers[index] = value;
            index++;
        }
    }
    
    uint8_t sw = (TYPE_W << 4) | R_WA;
    uint8_t bw = (TYPE_W << 4) | R_WB;
    
    pokers[52] = sw;
    pokers[53] = bw;
    CCLOG("poker sizeof =%lu\n",sizeof(*pokers));
    return PokerUint8_tMake(pokers, 54, true);
    
}

kPokerUint_8 PokerUtil::randPokers(kPokerUint_8 pokers)
{
    int count = pokers.lenth;
    CCLOG("the count ==%d", count);
    uint8_t* randPokers = (uint8_t*)malloc(count);
    int randCount = 0;
    while (randCount < 54)
    {
        int index = rand() % 54;
        uint8_t poker = pokers.data[index];
        if (poker != 0)
        {
        	//数字不为0才添加进入
            randPokers[randCount] = poker;
            pokers.data[index] = 0x0;
            randCount++;
        }
    }
    return PokerUint8_tMake(randPokers,count,true);
}

kPokerUint_8 PokerUtil::getPokers(const kPokerUint_8 pokers, int count)
{
    CC_ASSERT(pokers.lenth >= count);
    uint8_t* data = (uint8_t*)malloc(count);
    for (int i = 0; i < count; i++)
    {
        data[i] = pokers.data[i];
    }
    return PokerUint8_tMake(data, count,true);
}

cocos2d::Vector<PokerObject*> PokerUtil::copyWithPokerArr(Vector<PokerObject*> pokerArr)
{
    Vector<PokerObject*> newArr;
    for (int i = 0; i < pokerArr.size(); i++)
    {
        int pokerValue = pokerArr.at(i)->getPokerValue();
        PokerObject* poker = PokerObject::create(pokerValue);
        newArr.pushBack(poker);
    }
    CCAssert(newArr.size() > 0, "newArr must be non nil");
    return newArr;
}

cocos2d::Vector<PokerObject*> PokerUtil::removeLastObjects(Vector<PokerObject*> arr, int count)
{
    CC_ASSERT(arr.size() >= count);
    for (int i = 0; i < count; i++)
    {
//        arr->removeLastObject();
    	arr.popBack();
    }
    return arr;
}

kPokerUint_8 PokerUtil::removeLastObjects(kPokerUint_8 pokerData, int count)
{
    CC_ASSERT(pokerData.lenth >= count);
    uint8_t* pokers = (uint8_t*)malloc(pokerData.lenth - count);
    for (int i = 0; i < pokerData.lenth - count; i++)
    {
        pokers[i] = pokerData.data[i];
    }
    return PokerUint8_tMake(pokers, pokerData.lenth-count, true);
}

cocos2d::Vector<PokerObject*> PokerUtil::getLastObjects(Vector<PokerObject*> arr, int count)
{
    CC_ASSERT(arr.size() >= count);
    Vector<PokerObject*> lastObjects;
    for (int i = 0; i < count; i++)
    {
//        lastObjects->addObject(arr->lastObject());
//        arr->removeLastObject();//先添加后移除
    	lastObjects.pushBack(arr.back());
    	arr.popBack();//先添加后移除
    }
    return lastObjects;
}

kPokerUint_8 PokerUtil::getLastObjects(kPokerUint_8 pokerData, int count)
{
    CC_ASSERT(pokerData.lenth >= count);
    uint8_t* pokers = (uint8_t*)malloc(count);
    for (int i = 0; i < count; i++)
    {
        pokers[i] = pokerData.data[pokerData.lenth - 1 - i];
    }
    return PokerUint8_tMake(pokers, count, true);
}

kPokerUint_8 PokerUtil::getPokersFromArr(Vector<PokerObject*> pokerArr)
{
    if (pokerArr.size() == 0)
    {
        return PokerUint8_tMake(nullptr, 0, false);    //无效牌的kPokerUint_8结构体;
    }
    CCAssert(pokerArr.size() != 0, "pokerArr must have object");
    uint8_t* pokers = (uint8_t*)malloc(pokerArr.size() + 1);
	memset(pokers, 0, pokerArr.size() + 1);
    if (pokers == nullptr)
    {
        return PokerUint8_tMake(nullptr, 0);
    }
    for (int i = 0; i < pokerArr.size(); i++)
    {
        pokers[i] = pokerArr.at(i)->getPokerValue();
        CCLOG("the pokers[i]=%d, num=%d", pokers[i], pokerArr.at(i)->getPokerNum());
    }
    return PokerUint8_tMake(pokers, pokerArr.size(), true);
}

kPokerUint_8 PokerUtil::getDataFromSever(kPokerUint_8& pokerdata)
{
    for (int i = 0; i < pokerdata.lenth; i++)
    {
        uint8_t color = getPokerColor(pokerdata.data[i]);
        uint8_t num = getPokerNum(pokerdata.data[i]);
        if (color==TYPE_W)
        {
            if (num==1)
            {
                num = R_WA;
            }
            if (num==2)
            {
                num  = R_WB;
            }
            pokerdata.data[i] = (color << 4) | num;
        }
    }
    return pokerdata;
}

kPokerUint_8 PokerUtil::changeDataToSever(kPokerUint_8 &pokerData)
{
    for (int i = 0; i < pokerData.lenth; i++) {
        uint8_t color = getPokerColor(pokerData.data[i]);
        uint8_t num = getPokerNum(pokerData.data[i]);
        if (color == TYPE_W)
        {
            if (num == R_WA)
            {
                num = 1;
            }
            if (num == R_WB)
            {
                num  = 2;
            }
            pokerData.data[i] = (color << 4) | num;
        }
        CCLOG("the poker number=%d\n", num);
        CCLOG("the poker color==%d\n", color);
        if (color > TYPE_L)//去掉癞子
        {
            color -= TYPE_L;
            pokerData.data[i] = (color << 4) | num;
            
        }
    }
    return pokerData;
}

//将服务器给的手牌数据转换成PokerObject存入数组，并返回
cocos2d::Vector<PokerObject*> PokerUtil::getSeverData(unsigned char* data, int lenth)
{
    kPokerUint_8 pokerData = PokerUint8_tMake((uint8_t*)data, lenth);
    getDataFromSever(pokerData);
    return getPokerArrFromPokers(pokerData);
}

bool PokerUtil::checkDataLegal(kPokerUint_8 &pokerData)
{
    for (int i = 0; i < pokerData.lenth; i++)
    {
        uint8_t poker = pokerData.data[i];
        if (poker == 0)
        {
            return false;
        }
    }
    return true;
}

bool PokerUtil::isBomb(kWeightValue weightValue)
{
    if (weightValue.type == T_BOMB_S || weightValue.type == T_BOMB_L || weightValue.type == T_BOMB || weightValue.type == T_Rocket_2)
    {
        return true;
    }
    return false;
}

bool PokerUtil::compareWeightValueIsBig(kWeightValue lastWeightValue, kWeightValue selfWeightValue)
{
	//若本家为火箭，则肯定大过
    if (selfWeightValue.type == T_Rocket_2)
    {
        return true;
    }

    //若上家为火箭，则肯定不大过
    if (lastWeightValue.type == T_Rocket_2)
    {
        return false;
    }

    //若本家为无效牌，则肯定不大过
    if (selfWeightValue.type == T_Unknow)
    {
        return false;
    }

    //若上家为无效牌，则肯定大过
    if (lastWeightValue.type == T_Unknow)
    {
        return true;
    }

    //如果类型和数量都一样，则比大小
    if (selfWeightValue.pokerCount  == lastWeightValue.pokerCount
    		&& selfWeightValue.type == lastWeightValue.type)
    {
    	if (selfWeightValue.number > lastWeightValue.number)
    	{
    		return true;
    	}
    	else
    	{
    		return false;
    	}
    }

    //若上家不是炸弹，而我是炸弹，则肯定大过
    if (isBomb(selfWeightValue) && !isBomb(lastWeightValue))
    {
        return true;
    }

    //若本家不是炸弹，则肯定不大过
    if (!isBomb(selfWeightValue))
    {
        return false;
    }

    //如果两家都是炸弹，且为不同类型炸弹时（同类型炸弹的比较在上面已经处理，就在“如果类型和数量都一样，就比大小”中处理的），且都不是王炸（注：王炸的都在前面处理掉了）
    if (isBomb(selfWeightValue) && isBomb(lastWeightValue))
    {
    	//若本家是软炸（注：上家肯定不是软炸和王炸），则肯定不大过
        if (selfWeightValue.type == T_BOMB_S)
        {
            return false;
        }

        //若本家是硬炸（注：上家肯定不是硬炸和王炸）
        if (selfWeightValue.type == T_BOMB)
        {
        	// 若上家是软炸，肯定大过
            if (lastWeightValue.type == T_BOMB_S)
            {
                return true;
            }
            else
            {
            	// 若上家不是软炸，上家肯定不是硬炸和王炸，那上家只能是癞子炸，则肯定不大过
                return false;
            }
        }

        //若本家是癞子炸，则肯定大过
        if (selfWeightValue.type == T_BOMB_L)
        {
            return true;
        }
    }
    
    //默认大不过
    char* lastWeightValueStr = new char[256];
	sprintf(lastWeightValueStr, "kWeightValue::type:%d,number:%d,pokerCount:%d", lastWeightValue.type, lastWeightValue.number, lastWeightValue.pokerCount);

	char* selfWeightValueStr = new char[256];
	sprintf(selfWeightValueStr, "kWeightValue::type:%d,number:%d,pokerCount:%d", selfWeightValue.type, selfWeightValue.number, selfWeightValue.pokerCount);

    log("default deal with the poker weight value=====lastWeightValue:%s", lastWeightValueStr);
    log("default deal with the poker weight value=====selfWeightValue:%s", selfWeightValueStr);
    return false;
}

/**
 * 对牌值进行从小到大排序
 * @param datas      牌值
 * @param dataSize   牌的长度
 *
 * @return uint8_t* 排序后牌值
 */
uint8_t* PokerUtil::sortUint8StoB(uint8_t* datas, int dataSize)
{
    for (int i=0; i<dataSize; i++)
    {
        for (int j=i+1; j<dataSize; j++)
        {
            uint8_t num1 = datas[i];
            uint8_t num2 = datas[j];
            if (num1%16>num2%16)
            {
                uint8_t temp = num2;
                datas[j] = num1;
                datas[i] = temp;
            }
        }
    }
    
    return datas;
}

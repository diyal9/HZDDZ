//
//  PokerCheck.cpp
//  PokerTest
//
//  Created by 王体强 on 6/13/14.
//
//
/*
 */
#include "PokerCheck.h"
//癞子牌数字
int sg_laiziNum = 0;

int getLaiziNumber()
{
    return sg_laiziNum;
}

void setLaiziNumber(int num)
{
    sg_laiziNum = num;
}


USING_NS_CC;

/*牌型检测纯C版*/
PokerTypeCheck::PokerTypeCheck() :testTime(0)
								  ,m_laiziCount(0)
								  ,m_combinationCount(0)
{
    
}

PokerTypeCheck::~PokerTypeCheck()
{
    m_pokerArray.clear();
    m_combinationCount = 0;
    m_laiziCount = 0;
}

bool PokerTypeCheck::chekPokersEqual(cocos2d::Vector<PokerObject*> pokerArrA, cocos2d::Vector<PokerObject*> pokerArrB)
{
	if(pokerArrA.size() != pokerArrB.size()){
		return false;
	}

	pokerArrA = PokerUtil::sortArrayStoB(pokerArrA);
	pokerArrB = PokerUtil::sortArrayStoB(pokerArrB);

	for(int index = 0; index < pokerArrA.size(); index++){
		if(pokerArrA.at(index)->getPokerColor() != pokerArrB.at(index)->getPokerColor() ||
			pokerArrA.at(index)->getPokerNum() != pokerArrB.at(index)->getPokerNum() ||
			pokerArrA.at(index)->getPokerValue() != pokerArrB.at(index)->getPokerValue()
			){
			return false;
		}
	}

	return true;
}

kWeightValue PokerTypeCheck::getPokerType(cocos2d::Vector<PokerObject*> pokerArr)
{
    PokerTypeCheck* check = new PokerTypeCheck();
    kPokerUint_8 pokerData = PokerUtil::getPokersFromArr(pokerArr);
    //避免cocos2d::Vector<PokerObject*>在转换成kPokerUint_8时，出现数据损耗现象
//    kWeightValue weight = check->getPokerType(pokerData);
//    delete check;
//    return weight;

    //add by chenzc
    //以下代码的逻辑全都来自PokerTypeCheck::getPokerType(kPokerUint_8 &pokerData)方法，
    //只是一些癞子的检测换成了使用pokerArr里面的PokerObject数据进行检测，避免出现癞子变色出现损耗，导致牌型检测不准确的问题
	if (pokerArr.size() == 0)
	{
		return weight_Unknown;
	}
	int arrCout = pokerArr.size();
	int value = 0;
	int type = 0;

	int laiziCount = check->getLaiziCount(pokerArr);
	check->m_laiziCount = laiziCount;
	uint8_t* pokers = pokerData.data;
	kWeightValue weightValue = weight_Unknown;

	if (arrCout == 1)
	{
		type = T_ONE;
		value = PokerUtil::getPokerNum(pokers[0]);
	}
	else if (arrCout == 2)
	{
		value =PokerUtil::getPokerNum(pokers[0]);
		int value2 = PokerUtil::getPokerNum(pokers[1]);

		if (value < R_WA && value == value2)
		{
			//如果两个牌点数相同
			type = T_TWO;
			value = value2;
		}
		else if (value >= R_WA && value2 >= R_WA)//如果两个牌都不小于小王，那么肯定是火箭了
		{
			type = T_Rocket_2;
			value = R_WA;
		}
	}
	else if (arrCout == 3)
	{
		if (check->isAllnumSame(pokerData))
		{
			value = PokerUtil::getPokerNum(pokers[0]);
			type = T_THREE;
		}
	}
	else if (arrCout == 4)//四张牌的时候先判断是不是炸弹，根据炸弹类型判断软炸还是硬炸。如果不是炸弹再判断是不是三带一
	{
		if (laiziCount == 4)
		{//如果手牌和癞子牌都是四张，则直接癞子炸
			pokerData.data[0] = ((TYPE_L+1) << 4) | getLaiziNumber();
			pokerData.data[1] = ((TYPE_L+1) << 4) | getLaiziNumber();
			pokerData.data[2] = ((TYPE_L+1) << 4) | getLaiziNumber();
			pokerData.data[3] = ((TYPE_L+1) << 4) | getLaiziNumber();

			return WWWeightValueMake(T_BOMB_L, getLaiziNumber(), 4);
		}
		if (check->isAllnumSame(pokerData))
		{
			int laiziCountBomb = 0;

			value = PokerUtil::getPokerNum(pokers[0]);

			for(int i = 0; i < 4; i++)
			{
				PokerObject* obj = pokerArr.at(i);
				if(obj->getPokerColor() > TYPE_W)
				{
					laiziCountBomb++;
				}
			}

			if (laiziCountBomb == 0)
			{
				//没有癞子，硬炸
				type = T_BOMB;
			}
			else if (laiziCountBomb > 0 && laiziCountBomb < 4)//有癞子,癞子小于四张，软炸
			{
				type = T_BOMB_S;
			}

		}
		else
		{
			weightValue = check->checkFeiji(pokerData);
			return weightValue;
		}
	}
	else if (arrCout == 5)//五张牌的时候两种情况：顺子和三代二，先判断是不是三带二，然后判断是不是顺子。
	{

		weightValue = check->checkFeiji(pokerData);

		if (!isAvaildWeightValue(weightValue))
		{
			weightValue = check->checkShunzi(pokerData);
		}
		if (!isAvaildWeightValue(weightValue))
		{
			weightValue = weight_Unknown;
		}
		return weightValue;

	}
	else if (arrCout >= 6)
	{
		/*
		 先如果牌数大于6检测顺序为
		 1、飞机双
		 2、飞机单
		 3、三顺
		 4、双顺
		 5、顺子
		 */
		if (arrCout % 5 == 0 || arrCout % 4 == 0)
		{
			weightValue = check->checkFeiji(pokerData);
		}
		if(!isAvaildWeightValue(weightValue) && arrCout % 3 == 0)
		{
			weightValue = check->checkSanShun(pokerData);
		}
		if(!isAvaildWeightValue(weightValue) && arrCout % 2 == 0)
		{
			weightValue = check->checkShuangShun(pokerData);
		}
		if(!isAvaildWeightValue(weightValue))
		{
			weightValue = check->checkShunzi(pokerData);
		}
		if (!isAvaildWeightValue(weightValue))
		{
			weightValue = check->checkSiDaiPokers(pokerData);
		}
		if (isAvaildWeightValue(weightValue))
		{
			return weightValue;
		}

	}
	CCLOG("return weightValue type=%d\n,value=%d\n,arrCount=%d\n",type,value,arrCout);
	weightValue = WWWeightValueMake(type, value, arrCout);
	return weightValue;
}

void PokerTypeCheck::signLaiziPoker(kPokerUint_8 &pokerData)
{
    for (int i = 0; i < pokerData.lenth; i++)
    {
        uint8_t pokerNum = PokerUtil::getPokerNum(pokerData.data[i]);
        if (pokerNum == getLaiziNumber())
        {
            pokerData.data[i] = ((TYPE_L+1) << 4) | pokerNum;
        }
    }
}


kWeightValue PokerTypeCheck::getPokerType(kPokerUint_8 &pokerData)
{
    if (pokerData.lenth == 0)
    {
        return weight_Unknown;
    }
    int arrCout = pokerData.lenth;
    int value = 0;
    int type = 0;
    
    int laiziCount = getLaiziCount(pokerData);
    m_laiziCount = laiziCount;
    uint8_t* pokers = pokerData.data;
   // printPokerNum("getPokerType pokerdata=", pokerData);
    kWeightValue weightValue = weight_Unknown;

    
    if (arrCout == 1)
    {
        type = T_ONE;
        value = PokerUtil::getPokerNum(pokers[0]);
        
    }
    else if (arrCout == 2)
    {
        value =PokerUtil::getPokerNum(pokers[0]);
        int value2 = PokerUtil::getPokerNum(pokers[1]);
        
        if (value < R_WA && value == value2)
        {
        	//如果两个牌点数相同
            type = T_TWO;
            value = value2;
        }
        else if (value >= R_WA && value2 >= R_WA)//如果两个牌都不小于小王，那么肯定是火箭了
        {
            type = T_Rocket_2;
            value = R_WA;
        }
        
        
    }
    else if (arrCout == 3)
    {
        if (this->isAllnumSame(pokerData))
        {
            value = PokerUtil::getPokerNum(pokers[0]);
            type = T_THREE;
        }
    }
    else if (arrCout == 4)//四张牌的时候先判断是不是炸弹，根据炸弹类型判断软炸还是硬炸。如果不是炸弹再判断是不是三带一
    {
        if (laiziCount == 4)
        {//如果手牌和癞子牌都是四张，则直接癞子炸
            pokerData.data[0] = ((TYPE_L+1) << 4) | getLaiziNumber();
            pokerData.data[1] = ((TYPE_L+1) << 4) | getLaiziNumber();
            pokerData.data[2] = ((TYPE_L+1) << 4) | getLaiziNumber();
            pokerData.data[3] = ((TYPE_L+1) << 4) | getLaiziNumber();
            
            return WWWeightValueMake(T_BOMB_L, getLaiziNumber(), 4);
        }
        if (this->isAllnumSame(pokerData))
        {
            value = PokerUtil::getPokerNum(pokers[0]);

            for(int i = 0; i < 4; i++)
            {
            	PokerObject* obj = PokerObject::create(pokerData.data[i]);
            	if(obj->getPokerColor() > TYPE_W)
            	{
            		laiziCount++;
            	}
            }

            if (laiziCount==0)
            {
            	//没有癞子，硬炸
                type = T_BOMB;
            }
            else if (laiziCount > 0 && laiziCount < 4)//有癞子,癞子小于四张，软炸
            {
                type = T_BOMB_S;
            }
            
        }
        else
        {
            weightValue = checkFeiji(pokerData);
            return weightValue;
        }
    }
    else if (arrCout == 5)//五张牌的时候两种情况：顺子和三代二，先判断是不是三带二，然后判断是不是顺子。
    {
        
        weightValue = this->checkFeiji(pokerData);
        
        if (!isAvaildWeightValue(weightValue))
        {
            weightValue = this->checkShunzi(pokerData);
        }
        if (!isAvaildWeightValue(weightValue))
        {
            weightValue = weight_Unknown;
        }
        return weightValue;
        
    }
    else if (arrCout >= 6)
    {
        /*
         先如果牌数大于6检测顺序为
         1、飞机双
         2、飞机单
         3、三顺
         4、双顺
         5、顺子
         */
        if (arrCout % 5 == 0 || arrCout % 4 == 0)
        {
            weightValue = this->checkFeiji(pokerData);
        }
        if(!isAvaildWeightValue(weightValue) && arrCout % 3 == 0)
        {
            weightValue = this->checkSanShun(pokerData);
        }
        if(!isAvaildWeightValue(weightValue) && arrCout % 2 == 0)
        {
            weightValue = this->checkShuangShun(pokerData);
        }
        if(!isAvaildWeightValue(weightValue))
        {
            weightValue = this->checkShunzi(pokerData);
        }
        if (!isAvaildWeightValue(weightValue))
        {
            weightValue = this->checkSiDaiPokers(pokerData);
        }
        if (isAvaildWeightValue(weightValue))
        {
            return weightValue;
        }
        
    }
    //CCLOG("return weightValue type=%d\n,value=%d\n,arrCount=%d\n",type,value,arrCout);
    weightValue = WWWeightValueMake(type, value, arrCout);
    return weightValue;
    

}

bool checkPokersValid(const kPokerUint_8 pokerData)
{
    return false;
}

void resetChoose()
{
    
}

/**
 *  获取癞子牌个数
 *
 *  @param pokerArr 手牌对象数组
 *
 *  @return 癞子个数
 */
int PokerTypeCheck::getLaiziCount(kPokerUint_8 &pokers)
{
    int laiziCount = 0;
    for (int i = 0; i < pokers.lenth; i++)
    {
        if (PokerUtil::getPokerNum(pokers.data[i]) == getLaiziNumber())
        {
            laiziCount++;
        }
    }
    return laiziCount;
}

int PokerTypeCheck::getLaiziCount(cocos2d::Vector<PokerObject*> pokerArr)
{
    int laiziCount = 0;
    for (int i = 0; i < pokerArr.size(); i++)
    {
    	PokerObject* pokerObject = pokerArr.at(i);
        if (pokerObject->getPokerNum() == getLaiziNumber())
        {
            laiziCount++;
        }
    }
    return laiziCount;
}

/**
 *  获取一手牌中的癞子牌
 *
 */
kPokerUint_8 PokerTypeCheck::getLaiziPokers(kPokerUint_8 pokerData, int laiziCount)
{
    uint8_t* laiziPokers = (uint8_t*)malloc(laiziCount);
    int pokerIndex = 0;
    for (int i = 0; i < pokerData.lenth; i++)
    {
        if (PokerUtil::getPokerNum(pokerData.data[i]) == getLaiziNumber())
        {
            laiziPokers[pokerIndex] = pokerData.data[i];
            pokerIndex++;
        }
    }
    return PokerUint8_tMake(laiziPokers, laiziCount, true);
}

/**
 *  将一副手牌中的癞子移除
 *
 *  @param pokerArr 需要移除的手牌
 */
kPokerUint_8 PokerTypeCheck::removeLaizi(kPokerUint_8 &pokerData, int laiziCount)
{
    int index = 0;
    uint8_t* afterRemovePokers = (uint8_t*)malloc(pokerData.lenth - laiziCount);
    for (int i = 0; i < pokerData.lenth; i++)
    {
        if (getLaiziNumber() != PokerUtil::getPokerNum(pokerData.data[i]))
        {
            afterRemovePokers[index] = pokerData.data[i];
            index++;
        }
    }
    return PokerUint8_tMake(afterRemovePokers, pokerData.lenth - laiziCount, true);

}

/**
 *  判断一手牌中是否点数相同
 *
 *  @param pokerArr 传入手牌
 *
 *  @return 返回是否相同，true代表相同，fasle不同
 */
bool PokerTypeCheck::isAllnumSame(kPokerUint_8 &pokerData)
{
    uint8_t* pokers = pokerData.data;
    uint8_t num = PokerUtil::getPokerNum(pokers[0]);
    for (int i = 0; i < pokerData.lenth; i++)
    {
        uint8_t numi = PokerUtil::getPokerNum(pokers[i]);
        if (num != numi)
        {
            return false;
        }
    }
    return true;
}
/**
 *  找出一手牌中相同点数的个数
 *
 *  @param pokerArr
 *
 *  @return 返回相同牌张数
 */
int PokerTypeCheck::checkSameCardNum(kPokerUint_8 &pokerData)//暂时未用
{
    return 0;
}
/**
 *  检测三带一和四带二的牌型
 *
 *  @param pokerArr
 *
 *  @return 返回牌型
 */
kWeightValue PokerTypeCheck::checkSiDaiPokers(kPokerUint_8 &pokerData)
{
    //CCLOG("checkDaiPokers");
    int value = 0;
    int type = 0;
    int count = pokerData.lenth;
    if (count != 6 && count != 8)
    {
        //CCLOG("the poker count is unable");
        return weight_Unknown;
    }
    kWeightValue pokerType;
    kPokerUint_8 BtoSpokerData = sortArrCountsBtoS(pokerData);
    if (count == 6)
    {
        kPokerUint_8 noLastTwo = PokerUtil::removeLastObjects(BtoSpokerData, 2);
        if (isAllnumSame(noLastTwo))
        {
            type = T_FOUR_2;
            value = getPokerObjectAtIndex(noLastTwo, 0)->getPokerNum();
        }
        delePokerData(noLastTwo);
    }
    else if(count == 8)
    {
        kPokerUint_8 noLasts = PokerUtil::removeLastObjects(BtoSpokerData, 4);
        kPokerUint_8 lasts = PokerUtil::getLastObjects(BtoSpokerData, 4);
        if (isAllnumSame(noLasts) && checkDaiIsaVaild(noLasts, lasts))
        {
            type = T_FOUR_4;
            value = PokerUtil::getPokerNum(noLasts.data[0]);
        }
        delePokerData(noLasts);
        delePokerData(lasts);
    }
    pokerType = WWWeightValueMake(type, value, count);
    delePokerData(BtoSpokerData);
    return pokerType;

}
/**
 *  顺子检测
 *
 *  @param pokerArr <#pokerArr description#>
 *
 *  @return <#return value description#>
 */
kWeightValue PokerTypeCheck::checkShunzi(kPokerUint_8 &pokerData)
{
    //CCLOG("check shunzi pokerArr");
    if (isIncludeR2(pokerData))
    {
        return weight_Unknown;
    }
    int value = 0;
    int type = 0;
    int count = pokerData.lenth;
    if (count < 5)
    {
        //CCLOG("poker count is unable");
        return weight_Unknown;
    }
    kWeightValue pokerType;
    PokerUtil::sortArrayStoB(pokerData);
    for (int i = 0; i < count - 1; i++)
    {
        uint8_t num = PokerUtil::getPokerNum(pokerData.data[i]);
        uint8_t num1 = PokerUtil::getPokerNum(pokerData.data[i + 1]);
        if (num != num1 - 1)
        {
            return weight_Unknown;
        }
    }

    value = PokerUtil::getPokerNum(pokerData.data[0]);
    type = T_SEQ_1;
    pokerType = WWWeightValueMake(type, value, count);
    
    return pokerType;

}
/**
 *  双顺检测
 *
 *  @param pokerArr pokerArr description
 *
 *  @return return value description
 */
kWeightValue PokerTypeCheck::checkShuangShun(kPokerUint_8 &pokerData)
{
    //CCLOG("check shuangshun pokerArr");
    if (isIncludeR2(pokerData))
    {
        return weight_Unknown;
    }
    
    int count = pokerData.lenth;
    int type = 0;
    int value = 0;
    

    kWeightValue pokerType;
    if (count < 6 || count % 2 != 0)
    {
        //CCLOG("poker count is unable");
        return weight_Unknown;
    }
    PokerUtil::sortArrayStoB(pokerData);
    uint8_t* pokers = pokerData.data;
    for (int i = 0; i < count; i += 2)
    {
        uint8_t num = PokerUtil::getPokerNum(pokers[i]);
        uint8_t num1 = PokerUtil::getPokerNum(pokers[i + 1]);
        if ((i + 2) >= count)
        {
            if (num == num1)
            {//如果为最后一组，只需要判断一组数字相等即可
                continue;
            }
            else
            {//若最后两张牌条件不成立，返回无效牌型
                return weight_Unknown;
            }
        }
        int num2 = PokerUtil::getPokerNum(pokers[i + 2]);
        
        
        if ((num != num2 - 1 || num != num1))
        {//条件：一组中的两个数相等，相邻两组值相差为1
            return weight_Unknown;
        }
    }
    type = T_SEQ_2;
    value = PokerUtil::getPokerNum(pokers[0]);
    pokerType = WWWeightValueMake(type, value, count);
    return pokerType;

}

/**
 *  三顺检测
 *
 *  @param pokerArr <#pokerArr description#>
 *
 *  @return <#return value description#>
 */
kWeightValue PokerTypeCheck::checkSanShun(kPokerUint_8 &pokerData)
{
    //CCLOG("check shuangshun pokerArr");
    if (pokerData.lenth >= 6)
    {
        if (isIncludeR2(pokerData))
        {//如果大于等于六张才去检测是否包含2
            return weight_Unknown;
        }
    }

    int count = pokerData.lenth;
    int type = 0;
    int value = 0;
    
    uint8_t* pokers = pokerData.data;
    kWeightValue pokerType;
    if (count < 3 || count % 3 != 0)
    {
        //CCLOG("poker count is unable");
        return weight_Unknown;
    }
    PokerUtil::sortArrayStoB(pokerData);
    
    for (int i = 0; i < count; i += 3)
    {
        uint8_t num = PokerUtil::getPokerNum(pokers[i]);
        uint8_t num1 = PokerUtil::getPokerNum(pokers[i + 1]);
        uint8_t num2 = PokerUtil::getPokerNum(pokers[i + 2]);
        CCLOG("the num ==%d", num);
        if ((i + 3) >= count)
        {
            if (num == num1 && num == num2)
            {
            	//如果为最后一组，只需要判断一组数字相等即可
                continue;
            }
            else
            {
            	//若最后两张牌条件不成立，返回无效牌型
                return weight_Unknown;
            }
        }
        uint8_t num3 =  PokerUtil::getPokerNum(pokers[i + 3]);
        CCLOG("the num3==%d", num3);
        if (num != num3 - 1 || (num != num1 || num != num2))
        {
        	//条件：一组中的两个数相等，相邻两组值相差为1
            return weight_Unknown;
        }

    }
    type = T_SEQ_3;
    value = PokerUtil::getPokerNum(pokers[0]);
    pokerType = WWWeightValueMake(type, value, count);
    return pokerType;

}

/**
 *  飞机检测
 *
 */
kWeightValue PokerTypeCheck::checkFeiji(kPokerUint_8 &pokerData)
{
    //CCLOG("checkFeiji");
    int count = pokerData.lenth;
    int type = 0;
    int value = 0;
    kWeightValue pokerType;
    if ((count < 4 || count % 4 != 0) && (count < 5 || count % 5 != 0))
    {//飞机带单和带双
        //CCLOG("checkFeiji poker count is unable");
        return weight_Unknown;
    }
    //printUint8_t("feiji poker uint=", pokerUint);
    kPokerUint_8 feijiPokers = sortArrCountsBtoS(pokerData);
    if (count % 4 == 0)
    {//飞机带单
        int shunCount = count/4;//算出有多少个三带一
        kPokerUint_8 remainPokers = PokerUtil::removeLastObjects(feijiPokers, shunCount);
        pokerType = checkSanShun(remainPokers);
        printPokerNum("pokernumber", remainPokers);
        if (isAvaildWeightValue(pokerType))
        {

            if (remainPokers.lenth == 3)
            {
            	int laiziCount = 0;
				for (int i = 0; i < remainPokers.lenth; i++)
				{
					if (PokerUtil::getPokerNum(remainPokers.data[i]) == getLaiziNumber()
							|| PokerUtil::getPokerColor(remainPokers.data[i]) > TYPE_W)
					{
						laiziCount++;
					}
				}
                if (laiziCount == 3)
                {
					/*
					 *  在只有三张牌的时候，癞子牌也有三张的时候，将主题牌直接变成癞子牌
					 */
//                    pokerData.data[0] = ((TYPE_L+1)<<4)|getLaiziNumber();
//                    pokerData.data[1] = ((TYPE_L+1)<<4)|getLaiziNumber();
//                    pokerData.data[2] = ((TYPE_L+1)<<4)|getLaiziNumber();
                    return WWWeightValueMake(T_THREE_1, getLaiziNumber(), count);
                }
            }

            for(int i = 0; i < pokerData.lenth; i++)
            {
            	uint8_t pokerI = pokerData.data[i];
            	bool isContain = false;
            	for(int j = 0; j < remainPokers.lenth; j++)
            	{
            		uint8_t pokerJ = remainPokers.data[j];
            		if(pokerI == pokerJ)
            		{
            			isContain = true;
						break;
            		}
            	}

				//如果单牌是癞子，则癞子牌值不变
				if(!isContain)
				{
					PokerObject* obj = PokerObject::create(pokerData.data[i]);
					if(obj->getPokerColor() > TYPE_W && obj->getPokerNum() != getLaiziNumber())
					{
						//转换成癞子
						pokerData.data[i] = ((TYPE_L + 1) << 4) | getLaiziNumber();
					}
				}
            }

            value = pokerType.number;
            type = T_THREE_1;
        }
        delePokerData(remainPokers);

        //特殊处理双炸、单炸类型的飞机（如：55554444、55544443、55554443）
        if (feijiPokers.lenth == 8)
        {
        	int threePointArr[2] = {0, 0};
        	for (int i = 0; i < 2; i++)
        	{
        		bool isNotOver = true;
        		int threePoint = 0;
        		for (int j = 0; j < 4 && isNotOver; j++)
        		{
        			uint8_t numJ = PokerUtil::getPokerNum(feijiPokers.data[i * 4 + j]);
        			int equalCount = 0;
        			for (int k = 0; k < 4; k++)
        			{
        				uint8_t numK = PokerUtil::getPokerNum(feijiPokers.data[i * 4 + k]);
        				if(numK == numJ)
        				{
        					threePoint = numJ;
        					equalCount++;
        				}
        				if(equalCount >= 3)
						{
        					isNotOver = false;
        					threePointArr[i] = threePoint;
							break;
						}
        			}
        		}
        	}

        	if (threePointArr[0] != 0 && threePointArr[1] != 0
        			&& (threePointArr[0] - threePointArr[1] == 1 || threePointArr[1] - threePointArr[0] == 1))
			{
				value = threePointArr[0] > threePointArr[1] ? threePointArr[1] : threePointArr[0];
				type = T_THREE_1;
			}
        }
    }
    else if(count % 5 == 0)//飞机带双
    {
        int shunCount = count / 5;
        kPokerUint_8 remainPokers = PokerUtil::removeLastObjects(feijiPokers, shunCount * 2);
        
        kPokerUint_8 lastPokers = PokerUtil::getLastObjects(feijiPokers, shunCount * 2);
        
        if (!this->checkDaiIsaVaild(remainPokers, lastPokers))
        {
        	//带牌不符合全是对牌的规则
            return weight_Unknown;
        }
        pokerType = checkSanShun(remainPokers);
        if (isAvaildWeightValue(pokerType))
        {
            if (remainPokers.lenth == 3)
            {
            	int laiziCount = 0;
				for (int i = 0; i < remainPokers.lenth; i++)
				{
					if (PokerUtil::getPokerNum(remainPokers.data[i]) == getLaiziNumber()
							|| PokerUtil::getPokerColor(remainPokers.data[i]) > TYPE_W)
					{
						laiziCount++;
					}
				}
                if (laiziCount == 3)
                {
                    /*
                     * 在只有三张牌的时候，癞子牌也有三张的时候，将主题牌直接变成癞子牌
                     */
                    pokerData.data[0] = ((TYPE_L+1) << 4) | getLaiziNumber();
                    pokerData.data[1] = ((TYPE_L+1) << 4) | getLaiziNumber();
                    pokerData.data[2] = ((TYPE_L+1) << 4) | getLaiziNumber();
                    return WWWeightValueMake(T_THREE_2, getLaiziNumber(), count);
                }
            }

            for(int i = 0; i < pokerData.lenth; i++)
			{
				uint8_t pokerI = pokerData.data[i];
				bool isContain = false;
				for(int j = 0; j < remainPokers.lenth; j++)
				{
					uint8_t pokerJ = remainPokers.data[j];
					if(pokerI == pokerJ)
					{
						isContain = true;
						break;
					}
				}

				//如果双牌是癞子，则癞子牌值不变
				if(!isContain)
				{
					PokerObject* obj = PokerObject::create(pokerData.data[i]);
					if(obj->getPokerColor() > TYPE_W && obj->getPokerNum() != getLaiziNumber())
					{
						//转换成癞子
						pokerData.data[i] = ((TYPE_L + 1) << 4) | getLaiziNumber();
					}
				}
			}

            value = pokerType.number;
            type = T_THREE_2;
        }
        delePokerData(remainPokers);
        delePokerData(lastPokers);
    }
    delePokerData(feijiPokers);
    pokerType = WWWeightValueMake(type, value, count);
    return pokerType;

}
/**
 *  得出可以出牌的可能性，只适用于癞子
 *
 *  注意：当返回的手牌map的size为0或者为1的时候，直接将本手牌出掉
 *
 *  @param 本手牌
 *
 *  @return 返回所有能够打出的手牌数组，里边包含各种可出的手牌
 */
std::map<int, kPokerUint_8> PokerTypeCheck::getAllCanPlay(kPokerUint_8 &pokerData)
{
    int pokerCount = pokerData.lenth;
    int laiziCount = getLaiziCount(pokerData);

    int value = 0;
    int type = 0;
    
    int key = 0;
    
    std::map<int, kPokerUint_8> allCanPlay;

	//修正传入空手牌导致崩溃的问题
    if(pokerCount == 0)
	{
		return allCanPlay;
	}

    if (pokerCount == 4)
    {
        if (laiziCount == pokerCount)
        {
        	//如果只有纯癞子牌
            value = PokerUtil::getPokerNum(pokerData.data[0]);
            type = T_BOMB_L;
            key = (type << 4) + value;
            allCanPlay.insert(std::pair<int,kPokerUint_8>(key, pokerData));
            return allCanPlay;
        }
        if (laiziCount == 1)
        {
            kPokerUint_8 withNolaizi = removeLaizi(pokerData, 1);
            if (isAllnumSame(withNolaizi))
            {
                
            }
        }
    }

    // 处理纯癞子（不到4张）
	if (pokerCount != 4 && laiziCount == pokerCount)
	{
		value = PokerUtil::getPokerNum(pokerData.data[0]);
		allCanPlay.insert(std::pair<int,kPokerUint_8>(value, pokerData));
		return allCanPlay;
	}

    //CCLOG("the pokerArr count--%d",pokerArr.size());
    CCLOG("the laiziCount===%d,and combinationCount==%d", laiziCount, m_combinationCount);
    uint8_t** allCom = this->getAllLaiziCombination(laiziCount);
    
    kPokerUint_8 withNolaizi = removeLaizi(pokerData, laiziCount);//剔除癞子牌
    long beginAdd = clock();
    for (int i = 0; i < m_combinationCount; i++)
    {
        uint8_t* pokers = (uint8_t*)malloc(pokerCount);//一手牌的长度
        assert(pokers != nullptr);

        for (int j = 0; j < pokerCount; j++)
        {
            if (j < laiziCount)
            {//先将癞子变的牌加入
                pokers[j] = allCom[i][j];
            }
            else
            {
                pokers[j] = withNolaizi.data[j - laiziCount];//大于癞子之后将非癞子牌加入
            }
            
        }
        pokers = PokerUtil::sortUint8StoB(pokers, pokerCount);//然后排列之
        kPokerUint_8 pokersUint = PokerUint8_tMake(pokers, pokerCount, true);
        
        kWeightValue weightValue = this->getPokerType(pokersUint);
//        CCLOG("chenzc============value:%d", weightValue.number);
        if (isAvaildWeightValue(weightValue))
        {
            value = weightValue.number;
            type = weightValue.type;
            key = (type << 4) + value;
//            if(type == 10)
//            {
//            	CCLOG("chenzc============value:%d,type:%d,key:%d", value, type, key);
//            }

            allCanPlay.insert(std::pair<int, kPokerUint_8>(key, pokersUint));
        }
        free(allCom[i]);
        allCom[i] = nullptr;
    }
//    for (int i=0; i<m_combinationCount; i++) {
//        uint8_t* com = allCom[i];
//        free(com);
//        com = nullptr;
//    }
    CCLOG("the size of all can play =%lu", allCanPlay.size());
    delePokerData(withNolaizi);
    CCLOG("add allCanplay duration=%f second\n", (clock() - beginAdd) /1000000.0);
    return allCanPlay;

}

/**
*  检查三带二牌的合法性
*
*	@param remainPokers 主牌
*  @param pokerData 副牌
*
*  @return 返回是否合法
*/
bool PokerTypeCheck::checkDaiIsaVaild(kPokerUint_8 remainPokers, kPokerUint_8 &pokerData)
{
    //printUint8_t("checkDaiIsaVaild", pokerData);
	std::map<int, kPokerUint_8> poolRemainMap = putPokersToPool(remainPokers);
    std::map<int, kPokerUint_8> poolMap = putPokersToPool(pokerData);
    for (std::map<int, kPokerUint_8>::iterator i = poolMap.begin(); i != poolMap.end(); i++)
    {
        kPokerUint_8 sonPokers = i->second;
        if (sonPokers.lenth != 2)
        {
            return false;
        }

		for (std::map<int, kPokerUint_8>::iterator j = poolRemainMap.begin(); j != poolRemainMap.end(); j++)
		{
			int sonValue = i->first;
			int remainValue = j->first;
			if(sonValue == remainValue)
			{
				return false;
			}
			
			int firstValue = PokerUtil::getPokerNum(i->second.data[0]);
			int secValue = PokerUtil::getPokerNum(i->second.data[1]);
			int mainValue = PokerUtil::getPokerNum(j->second.data[0]);
			if( (firstValue == mainValue && firstValue != getLaiziNumber()) || (secValue == mainValue && secValue != getLaiziNumber()) )
			{
				return false;
			}
		}
    }
    return true;
}
/**
 *  一手牌中是否包含2
 *
 *  @param pokerArr 一手牌
 *
 *  @return true包含2，false不包含2
 */
bool PokerTypeCheck::isIncludeR2(kPokerUint_8 &pokerData)
{
    for (int i = 0; i < pokerData.lenth; i++)
    {
        uint8_t num = PokerUtil::getPokerNum(pokerData.data[i]);
        if (num == R2)
        {
            return true;
        }
    }
    return false;
}
/**
 *  将相同点数的牌放在一起，数量由小到大排列
 *
 *  @param pokerAr
 *
 *  @return
 */
kPokerUint_8 PokerTypeCheck::sortArrCountsBtoS(kPokerUint_8 &pokerData)
{
    uint8_t* feijiPokers = (uint8_t*)malloc(pokerData.lenth);//先创建飞机容器
    std::map<int, kPokerUint_8> poolMap = putPokersToPool(pokerData);//得到相同点数的map
    int index = 0;
    
    for (std::map<int, kPokerUint_8>::iterator i = poolMap.begin(); i != poolMap.end(); i++)
    {//遍历map，将map中的值放进数组
        kPokerUint_8 pokerUint = i->second;
        if (pokerUint.lenth <= 0)
        {//数组不能为空的话直接下一轮
            continue;
        }
        //printUint8_t("map pokeruint", pokerUint);
        for (int j = 0; j < pokerUint.lenth; j++)
        {
            feijiPokers[index] = pokerUint.data[j];
            index++;
        }
    }
    
    return PokerUint8_tMake(feijiPokers, pokerData.lenth, true);
}

PokerObject* PokerTypeCheck::getPokerObjectAtIndex(kPokerUint_8 pokerData, int index)

{
    PokerObject* poker = PokerObject::create(pokerData.data[index]);
    return poker;
}

std::map<int, kPokerUint_8> PokerTypeCheck::putPokersToPool(kPokerUint_8 &pokerData)
{
   // long begin = clock();
    std::map<int,kPokerUint_8> pokerPool;
    for (int i = 0; i < pokerData.lenth; i++ )
    {
        uint8_t num = PokerUtil::getPokerNum(pokerData.data[i]);
        //CCLOG("the num is----%d\n",num);
        uint8_t* samePokers = (uint8_t*)malloc(4);//相同的牌最多只能有四个
        samePokers[0] = pokerData.data[i];
        int sameCount = 1;
        for (int j = i + 1; j < pokerData.lenth; j++)
        {
            uint8_t numj = PokerUtil::getPokerNum(pokerData.data[j]);
            //CCLOG("the numj---%d\n",numj);
            if (num == numj)
            {
                i++;
               // CCLOG("the sameCount--%d\n",sameCount);
                samePokers[sameCount] = pokerData.data[j];
                sameCount++;
                //CCLOG("the j=%d",j);
            }
            else
            {
                break;
            }
        }
        /*将数量作为最优先的值，比大小的时候使用,由于map中的键位大小默认是升序排列，所以要想得到自己想要的键位排列顺序，则必须做如下如理
         我们这个数列排列的宗旨是数量优先点数顺序次之，也就是数量越大，则考前排，数量一样，点数越小则越靠前排
         所以，先将点数进行一次位运算，向左移16位，然后向右移动个数的位数x4，即可得达到这样的效果
         */
        int keyNum = (num << 16) >> (sameCount * 4);
        //CCLOG("the keyNum is==%d\n",keyNum);
        //printUint8_t("samePokers", PokerUint8_tMake(samePokers, sameCount));
        pokerPool.insert(std::pair<int,kPokerUint_8>(keyNum, PokerUint8_tMake(samePokers, sameCount, true)));
    }
//    testTime+=(clock()-begin);
//    CCLOG("put pokers to pool cost =%lu\n",testTime);
    return pokerPool;
}

std::map<int, kPokerUint_8> PokerTypeCheck::putPokersToTipsPool(kPokerUint_8 &pokerData)
{
   // long begin = clock();
    std::map<int,kPokerUint_8> pokerPool;
    for (int i = 0; i < pokerData.lenth; i++ )
    {
        uint8_t num = PokerUtil::getPokerNum(pokerData.data[i]);
        //CCLOG("the num is----%d\n",num);
        uint8_t* samePokers = (uint8_t*)malloc(4);//相同的牌最多只能有四个
        samePokers[0] = pokerData.data[i];
        int sameCount = 1;
        for (int j = i + 1; j < pokerData.lenth; j++)
        {
            uint8_t numj = PokerUtil::getPokerNum(pokerData.data[j]);
            //CCLOG("the numj---%d\n",numj);
            if (num == numj)
            {
                i++;
               // CCLOG("the sameCount--%d\n",sameCount);
                samePokers[sameCount] = pokerData.data[j];
                sameCount++;
                //CCLOG("the j=%d",j);
            }
            else
            {
                break;
            }
        }
        int keyNum = (sameCount << 8) + num;
        pokerPool.insert(std::pair<int,kPokerUint_8>(keyNum, PokerUint8_tMake(samePokers, sameCount, true)));
    }
    return pokerPool;
}

uint8_t** PokerTypeCheck::getAllLaiziCombination(int laiziCount)
{
    //int  comCount = pow(13, laiziCount);//组合的个数
    
    //CCLOG("the laiziCount===%d,and combinationCount==%d",laiziCount,m_combinationCount);
    uint8_t** allCom = nullptr;
    long long index = 0;
    
    long begin = clock();
    switch (laiziCount)
    {
        case 1:
            allCom = (uint8_t**)malloc(4 * 13);//所有的组合形式
            for (int i = R3; i < R_WA; i++)
            {
                uint8_t* laiziCom = (uint8_t*)malloc(laiziCount);//癞子牌
                laiziCom[0] = ((TYPE_L + 1) << 4) | i;//加上癞子花色和方块
                allCom[index] = laiziCom;
                index++;
                m_combinationCount++;
            }
            break;
        case 2:
            allCom = (uint8_t**)malloc(4*91);
            for (int i = R3; i < R_WA; i++)
            {
                for (int j = i; j < R_WA; j++)
                {
                    uint8_t* laiziCom = (uint8_t*)malloc(laiziCount);//癞子牌
                    laiziCom[0] = ((TYPE_L + 1) << 4) | i;
                    laiziCom[1] = ((TYPE_L + 1) << 4) | j;
                    
                    allCom[index] = laiziCom;
                    index++;
                    m_combinationCount++;
                }
            }
            break;
        case 3:
            allCom = (uint8_t**)malloc(4 * 455);
            for (int i = R3; i < R_WA; i++)
            {
                for (int j = i; j < R_WA; j++)
                {
                    for (int m = j; m < R_WA; m++)
                    {
                        uint8_t* laiziCom = (uint8_t*)malloc(laiziCount);//癞子牌
                        laiziCom[0] = ((TYPE_L + 1) << 4) | i;
                        laiziCom[1] = ((TYPE_L + 1) << 4) | j;
                        laiziCom[2] = ((TYPE_L + 1) << 4) | m;
                        
                        allCom[index] = laiziCom;
                        index++;
                        m_combinationCount++;
                    }
                }
            }
            break;
        case 4:
            allCom = (uint8_t**)malloc(4 * 1820);
            for (int i = R3; i < R_WA; i++)
            {
                for (int j = i; j < R_WA; j++)
                {
                    for (int m = j; m < R_WA; m++)
                    {
                        for (int n = m; n < R_WA; n++)
                        {
                            uint8_t* laiziCom = (uint8_t*)malloc(laiziCount);//癞子牌
                            laiziCom[0] = ((TYPE_L + 1) << 4) | i;
                            laiziCom[1] = ((TYPE_L + 1) << 4) | j;
                            laiziCom[2] = ((TYPE_L + 1) << 4) | m;
                            laiziCom[3] = ((TYPE_L + 1) << 4) | n;
                            
                            if (i == j && i == m && i == n)
                            {//当四个癞子都变成一样的牌的时候只能当做癞子本身的癞子炸
                                laiziCom[0] = ((TYPE_L + 1) << 4) | getLaiziNumber();
                                laiziCom[1] = ((TYPE_L + 1) << 4) | getLaiziNumber();
                                laiziCom[2] = ((TYPE_L + 1) << 4) | getLaiziNumber();
                                laiziCom[3] = ((TYPE_L + 1) << 4) | getLaiziNumber();
                            }
                            //CCLOG("---------- i=%d\n,j=%d\n,m=%d\n,n=%d\n",i,j,m,n);
                            allCom[index] = laiziCom;
                            index++;
                            m_combinationCount++;
                        }
                    }
                    
                }
            }
            break;
            
        default:
            break;
    }
    
//        //两组完全相同点数的移除
//    long before = clock();
//        for (int i=0; i<comCount; i++) {
//            for (int j=0; j<comCount; j++) {
//                uint8_t* inums = sortUint8StoB(allCom[i], laiziCount);
//                uint8_t* jnums = sortUint8StoB(allCom[j], laiziCount);
//                int sameCount = 0;
//                for (int m=0; m<laiziCount; m++) {
//                    if (jnums[m]==inums[m]) {
//                        sameCount++;
//                    }
//                    if (sameCount==laiziCount) {
//                        inums[0]=0;
//                        //free(inums);
//                    }
//                }
//            }
//        }
//    CCLOG("after remove same point==%lu\n",clock()-before);
    CCLOG("getAllLaiziCombination spend time==%lu\n", clock() - begin);
    
    return allCom;

}

bool PokerTypeCheck::isContainLaizi(kPokerUint_8 &pokerData)
{
    for (int i = 0; i < pokerData.lenth; i++)
    {
        if (PokerUtil::getPokerNum(pokerData.data[i]) == getLaiziNumber())
        {
            return true;
        }
    }
    return false;
}
/*
 由于类似于顺子这种牌，服务器的点数计算是从最小值开始，而本地这边是从最大值开始，所以用这个方法进行转换
*/
int PokerTypeCheck::pokersNumberToService(kWeightValue &weight)
{
    int number = weight.number;
    int count = weight.pokerCount;
    int serviceNumber = 0;
    switch (number)
    {
        case T_SEQ_1:
            serviceNumber = number + count - 1;
            break;
        case T_SEQ_2:
            serviceNumber = number + (count / 2) - 1;
            break;
        case T_SEQ_3:
            serviceNumber = number + (count / 3) - 1;
            break;
        case T_THREE_1:
            serviceNumber = number + (count / 4) - 1;
            break;
        case T_THREE_2:
            serviceNumber = number + (count / 5) - 1;
            break;
        default:
            serviceNumber = number;
            break;
    }
    return serviceNumber;
}


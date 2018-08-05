//
//  PokerTips.cpp
//  PokerTest
//
//  Created by 王体强 on 6/27/14.
//
//	Changed by Jacky
//

#include "PokerTips.h"

USING_NS_CC;

extern int getLaiziNumber();

PokerTips::PokerTips() : __check(nullptr)
						,__laiziCount(0)
						,__laiziPokerData(zero_pokerData)
{
    __check = new PokerTypeCheck();
}

PokerTips::~PokerTips()
{
    for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
    {
        if (i->second.lenth > 0)
        {
            delePokerData(i->second);
        }
    }
    for (std::map<int, kPokerUint_8>::iterator i = __sbCountPool.begin(); i != __sbCountPool.end(); i++)
    {
        delePokerData(i->second);
    }
    delePokerData(__laiziPokerData);
    if (__check)
    {
        delete __check;
    }
    CCLOG("PokerTips delete");
}

kPokerUint_8 PokerTips::addPokersWith(kPokerUint_8 &currentPokers, kPokerUint_8 &targetPokers, int addCount)
{
	if (addCount>targetPokers.lenth)
    {
		return zero_pokerData;
	}
	int sumCount = currentPokers.lenth+addCount;
	uint8_t* sumPokers = (uint8_t*)malloc(sumCount);
	for (int i = 0; i < sumCount; i++)
    {
		if (i < currentPokers.lenth)
        {
			sumPokers[i] = currentPokers.data[i];
		}
		else
		{
			sumPokers[i] = targetPokers.data[i - currentPokers.lenth];
		}
	}
	delePokerData(currentPokers);
	return PokerUint8_tMake(sumPokers, sumCount, true);
}

kPokerUint_8 PokerTips::addPokersWithPlanCount(kPokerUint_8 &currentPokers, kPokerUint_8 &targetPokers, int addCount)
{
    kPokerUint_8 pokers = zero_pokerData;
    if (addCount <= targetPokers.lenth)
    {
        pokers = addPokersWith(currentPokers, targetPokers, addCount);
    }
    else
    {
        int sumCount = currentPokers.lenth + targetPokers.lenth;//如果长度超过了目标的数组，则目标有多长就加多少
        uint8_t* sumPokers = (uint8_t*)malloc(sumCount);
        for (int i = 0; i < sumCount; i++)
        {
            if (i < currentPokers.lenth)
            {
                sumPokers[i] = currentPokers.data[i];
            }
            else
            {
                sumPokers[i] = targetPokers.data[i - currentPokers.lenth];
            }
        }
        pokers = PokerUint8_tMake(sumPokers, sumCount, true);
        targetPokers.lenth -= sumCount - currentPokers.lenth;//将添加的数据长度减少
        delePokerData(currentPokers);
    }

    return pokers;
}

kPokerUint_8 PokerTips::getPokersWithCount(kPokerUint_8 &pokerData, int count)
{
    CCAssert(pokerData.lenth > count, "target lenth must bigger than count");
    uint8_t* pokers = (uint8_t*)malloc(count);
    for (int i = 0; i < count; i++)
    {
        pokers[i] = pokerData.data[i];
    }
    return PokerUint8_tMake(pokers, count, true);
}

void PokerTips::putSameCardToPool(kPokerUint_8 &myPokers)
{
    //assert(myPokers.lenth>0);
    for (int i = 0; i < myPokers.lenth; i++)
    {
        uint8_t numi = PokerUtil::getPokerNum(myPokers.data[i]);
        uint8_t* samePokers = (uint8_t*)malloc(4);
        samePokers[0] = myPokers.data[i];
        int sameCount = 1;
        for (int j = i + 1; j < myPokers.lenth; j++)
        {
            uint8_t numj = PokerUtil::getPokerNum(myPokers.data[j]);
            if (numi == numj)
            {
                samePokers[sameCount] = myPokers.data[j];
                sameCount++;
                i++;
            }
            else
            {
                break;
            }
        }
        //牌池中的点数作为key
        __pokerPool.insert(std::pair<uint8_t, kPokerUint_8>(numi,PokerUint8_tMake(samePokers, sameCount, true)));
        CCLOG("the pair key==%d", numi);
        printUint8_t("the pair data", PokerUint8_tMake(samePokers, sameCount, true));
    }
}

void PokerTips::signPokerStatue(kPokerUint_8 &somePokers)
{
    printUint8_t("signPokerStatue", somePokers);
    for (int i = 0; i < somePokers.lenth; i++)
    {

        __pokerStatue.insert(std::pair<uint8_t, int> (somePokers.data[i], true));
    }

}

void PokerTips::signPokerPointSatue(kPokerUint_8 &somePokers)
{
    printPokerNum("signPokerPointSatue", somePokers);
    for (int i = 0; i < somePokers.lenth; i++)
    {
        uint8_t point = PokerUtil::getPokerNum(somePokers.data[i]);
        __pokerStatue.insert(std::pair<uint8_t, int> (point, true));
    }
}

void PokerTips::lessLaiziCount(kPokerUint_8 &somePokers)
{
    int lessCount = 0;
    for (int i = 0; i < somePokers.lenth; i++)
    {
        uint8_t pokerColor = PokerUtil::getPokerColor(somePokers.data[i]);
        if (pokerColor == TYPE_L)
        {
            lessCount++;
        }
    }
    __laiziPokerData.lenth = __laiziPokerData.lenth - lessCount;
}

Vector<PokerObject*> PokerTips::getTipsPokers(cocos2d::Vector<PokerObject*> lastTipsPokers, cocos2d::Vector<PokerObject*> myPokers, cocos2d::Vector<PokerObject*> lastPlayerPokers)
{
    PokerTips *tips = new PokerTips();
    kPokerUint_8 myPokerData = PokerUtil::getPokersFromArr(myPokers);

    //add by chenzc
	//以下代码的逻辑全都来自PokerTips::getTips(kPokerUint_8 &lastPlayerPokers, kPokerUint_8 &myPokers)方法，
	//只是一些癞子的检测换成了使用lastPlayerPokers里面的PokerObject数据进行检测，避免出现癞子变色出现损耗，导致牌型检测不准确的问题
    tips->resetLaiziData(myPokerData);
	//printPokerNum("__laiziData==", tips->__laiziPokerData);
	kPokerUint_8 withNolaizi = tips->__check->removeLaizi(myPokerData, tips->__laiziCount);//剔除掉癞子牌
	//signPokerStatue(withNolaizi, 0);//先把所有的牌状态标记为未选择
	tips->__sbCountPool = tips->__check->putPokersToTipsPool(withNolaizi);//得到一个数量由小到大的牌池(用于飞机或需要按照牌数来的牌型)

	//得到上一次提示的牌型和权值
	kWeightValue lastTipsWeight = tips->__check->getPokerType(lastTipsPokers);//得到上次提示的权重
	if( tips->__check->getLaiziCount(lastTipsPokers) > 0 )
	{
		std::vector<Vector<PokerObject*>> vecAllPokersVec = PokerTips::getAllPokersCanPlay(lastPlayerPokers, lastTipsPokers);
		//替换后的牌字符串
		kPokerUint_8 szReplacePokerData;
		//返回空的时候，直接使用原牌
		if (vecAllPokersVec.size() > 0)
		{
			szReplacePokerData = PokerUtil::getPokersFromArr(vecAllPokersVec.at(0));
		}else{
			szReplacePokerData = PokerUtil::getPokersFromArr(lastTipsPokers);
		}
		PokerUtil::changeDataToSever(szReplacePokerData);
		lastTipsWeight = PokerTypeCheck::getPokerType(PokerUtil::getPokerArrFromPokers(szReplacePokerData));
		
		delePokerData(szReplacePokerData);
	}
	int lastTipsType = lastTipsWeight.type;
	int lastPoint = lastTipsWeight.number;
	int lastCount = lastTipsWeight.pokerCount;

	PokerUtil::sortArrayStoB(withNolaizi);//将牌按照点数由小到大排序
	tips->putSameCardToPool(withNolaizi);
	kPokerUint_8 tipPokers = zero_pokerData;
	//memset(&tipPokers, 0, sizeof(kPokerUint_8));
	tipPokers.lenth = 0;

	kWeightValue lastPlayWeight = tips->__check->getPokerType(lastPlayerPokers);//得到上家出牌的权重
	//上家没有出牌时玩家首出点提示
	if(lastPlayWeight.type == T_Unknow){
		tipPokers = tips->getPokersWithUnKnow();
	}
	else
	{
		switch (lastTipsType)
		{
			case T_Unknow:
				tipPokers = tips->getPokersWithUnKnow();
				break;
			case T_ONE:
				tipPokers = tips->getOnePoker(lastPoint, lastPlayWeight.number);
				break;
			case T_TWO:
				tipPokers = tips->getTwoPokers(lastPoint, lastPlayWeight.number);
				break;
			case T_THREE:
				tipPokers = tips->getThreePokers(lastPoint, lastPlayWeight.number);
				break;
			case T_THREE_1:
			{
				if (lastCount < 6)
				{
					tipPokers = tips->getSanDai(lastPoint, lastCount);
				}
				else
				{
					tipPokers = tips->getFeijiDan(lastPoint, lastCount);
				}
				break;
			}
			case T_THREE_2:
			{
				if (lastCount<6) {
					tipPokers = tips->getSanDai(lastPoint, lastCount);
				}
				else
				{
					tipPokers = tips->getFeijiShuang(lastPoint, lastCount);
				}
				break;
			}
			case T_FOUR_2:
				tipPokers = tips->getBombPokers(0, 0);
				break;
			case T_FOUR_4:
				tipPokers = tips->getBombPokers(0, 0);
				break;
			case T_SEQ_1:
				tipPokers = tips->getShunzi(lastPoint, lastCount);
				break;
			case T_SEQ_2:
				tipPokers = tips->getShuangShun(lastPoint, lastCount);
				break;
			case T_SEQ_3:
				tipPokers = tips->getSanShun(lastPoint, lastCount);
				break;
			case T_BOMB_S:
				tipPokers = tips->getBombPokers(lastPoint, T_BOMB_S);
				break;
			case T_BOMB:
				tipPokers = tips->getBombPokers(lastPoint, T_BOMB);
				break;
			case T_BOMB_L:
				tipPokers = tips->getBombPokers(lastPoint, T_BOMB_L);
				break;
			case T_Rocket_2:
				tipPokers = tips->getBombPokers(lastPoint, T_Rocket_2);
				break;
			default:
				break;
		}

		if (tipPokers.lenth == 0)//常规提示未得到牌，只能开始找炸
		{
			if(lastTipsType != T_BOMB_S && lastTipsType != T_BOMB && lastTipsType != T_BOMB_L && lastTipsType != T_Rocket_2)
			{
				tipPokers = tips->getBombPokers(0, T_Unknow);
			}
		}
	}

	//printPokerNum("lastPlayerPokers", PokerUtil::getPokersFromArr(lastPlayerPokers));
	if (tipPokers.lenth > 0)
	{
		printPokerNum("the tips pokers", tipPokers);
		CCAssert(PokerUtil::checkDataLegal(tipPokers), "tipPokers must be legal");//最后一道工序,检测是否合法
	}
	delePokerData(withNolaizi);

    Vector<PokerObject*> pokerDataArr = PokerUtil::getPokerArrFromPokers(tipPokers);
    delete tips;
    CCLOG("the getTipsPokers count==%zd", pokerDataArr.size());
    for (int i = 0; i < pokerDataArr.size(); i++)
    {
        CCLOG("Trace::PokerTips::getTipsPokers result arr(%d)=%d", pokerDataArr.at(i)->getPokerNum());
    }
    //pokerDataArr->retain();
    return pokerDataArr;
}

Vector<PokerObject*> PokerTips::getClickTipsPokers(cocos2d::Vector<PokerObject*> lastPlayerPokers, cocos2d::Vector<PokerObject*> myPokers, cocos2d::Vector<PokerObject*> seletPokers)
{
    PokerTips *tips = new PokerTips();
    kPokerUint_8 myPokerData = PokerUtil::getPokersFromArr(myPokers);
	
    //changed by Jacky
    tips->resetLaiziData(myPokerData);
	printPokerNum("__laiziData==", tips->__laiziPokerData);
	kPokerUint_8 withNolaizi = tips->__check->removeLaizi(myPokerData, tips->__laiziCount);//剔除掉癞子牌
	//signPokerStatue(withNolaizi, 0);//先把所有的牌状态标记为未选择
	tips->__sbCountPool = tips->__check->putPokersToTipsPool(withNolaizi);//得到一个数量由小到大的牌池(用于飞机或需要按照牌数来的牌型)
	kWeightValue lastWeight = tips->__check->getPokerType(lastPlayerPokers);//得到上家出牌的权重
	CCLOG("the lastWeight type=%d,number=%d,count=%d", lastWeight.type, lastWeight.number, lastWeight.pokerCount);
	int lastType = lastWeight.type;
	int lastPoint = lastWeight.number;
	int lastCount = lastWeight.pokerCount;
	PokerUtil::sortArrayStoB(withNolaizi);//将牌按照点数由小到大排序

	tips->putSameCardToPool(withNolaizi);
	kPokerUint_8 tipPokers = zero_pokerData;
	//memset(&tipPokers, 0, sizeof(kPokerUint_8));
	tipPokers.lenth = 0;

	kWeightValue selectWeight = tips->__check->getPokerType(seletPokers);//得到选择中牌的权重
	int selectPoint = selectWeight.number;
	if(selectWeight.type == T_ONE && selectPoint > lastPoint)
	{
		switch (lastType)
		{
			case T_Unknow:
				break;
			case T_ONE:
				break;
			case T_TWO:
				tipPokers = tips->getTwoPokersByPart(selectPoint);
				break;
			case T_THREE:
				tipPokers = tips->getThreePokersByPart(selectPoint);
				break;
			case T_THREE_1:
			{
				if (lastCount < 6)
				{
					tipPokers = tips->getSanDaiByPart(selectPoint, lastCount);
				}
				else
				{
					tipPokers = tips->getFeijiDanByPart(selectPoint, lastCount);
				}
				break;
			}
			case T_THREE_2:
			{
				if (lastCount<6) {
					tipPokers = tips->getSanDaiByPart(selectPoint, lastCount);
				}
				else
				{
					tipPokers = tips->getFeijiShuangByPart(selectPoint, lastCount);
				}
				break;
			}
			case T_FOUR_2:
				tipPokers = tips->getBombPokersByPart(0, 0);
				break;
			case T_FOUR_4:
				tipPokers = tips->getBombPokersByPart(0, 0);
				break;
			case T_SEQ_1:
				tipPokers = tips->getShunziByPart(selectPoint, lastCount);
				break;
			case T_SEQ_2:
				tipPokers = tips->getShuangShunByPart(selectPoint, lastCount);
				break;
			case T_SEQ_3:
				tipPokers = tips->getSanShunByPart(selectPoint, lastCount);
				break;
			case T_BOMB_S:
				tipPokers = tips->getBombPokersByPart(selectPoint, T_BOMB_S);
				break;
			case T_BOMB:
				tipPokers = tips->getBombPokersByPart(selectPoint, T_BOMB);
				break;
			case T_BOMB_L:
				tipPokers = tips->getBombPokersByPart(selectPoint, T_BOMB_L);
				break;
			case T_Rocket_2:
				break;
			default:
				break;
		}
	}
	
	if (tipPokers.lenth == 0)//常规提示未得到牌，只能开始找炸
	{
		if(lastType != T_BOMB_S && lastType != T_BOMB && lastType != T_BOMB_L && lastType != T_Rocket_2)
		{
			tipPokers = tips->getBombPokersByPart(selectPoint, T_Unknow);
		}
	}

	printPokerNum("lastPlayerPokers", PokerUtil::getPokersFromArr(lastPlayerPokers));
	if (tipPokers.lenth > 0)
	{
		printPokerNum("the tips pokers", tipPokers);
		CCAssert(PokerUtil::checkDataLegal(tipPokers), "tipPokers must be legal");//最后一道工序,检测是否合法
	}
	delePokerData(withNolaizi);


    Vector<PokerObject*> pokerDataArr = PokerUtil::getPokerArrFromPokers(tipPokers);
    delete tips;
    CCLOG("the getClickTipsPokers count==%d", pokerDataArr.size());
    for (int i = 0; i < pokerDataArr.size(); i++)
    {
        CCLOG("Trace::PokerTips::getClickTipsPokers result arr(%d)=%d", pokerDataArr.at(i)->getPokerNum());
    }
    //pokerDataArr->retain();
    return pokerDataArr;
}

Vector<PokerObject*> PokerTips::getMoveTipsPokers(cocos2d::Vector<PokerObject*> myPokers, cocos2d::Vector<PokerObject*> seletPokers)
{
    PokerTips *tips = new PokerTips();
    kPokerUint_8 myPokerData = PokerUtil::getPokersFromArr(seletPokers);

    //changed by Jacky
    tips->resetLaiziData(myPokerData);
	printPokerNum("__laiziData==", tips->__laiziPokerData);
	kPokerUint_8 withNolaizi = tips->__check->removeLaizi(myPokerData, tips->__laiziCount);//剔除掉癞子牌
	//signPokerStatue(withNolaizi, 0);//先把所有的牌状态标记为未选择
	tips->__sbCountPool = tips->__check->putPokersToTipsPool(withNolaizi);//得到一个数量由小到大的牌池(用于飞机或需要按照牌数来的牌型)
	PokerUtil::sortArrayStoB(withNolaizi);//将牌按照点数由小到大排序

	tips->putSameCardToPool(withNolaizi);
	kPokerUint_8 tipPokers = zero_pokerData;
	//memset(&tipPokers, 0, sizeof(kPokerUint_8));
	tipPokers.lenth = 0;
	
	for(int len = seletPokers.size(); len >= 5; len--){
		// 先找三顺
		if(len >= 15){
			tipPokers = tips->getSanShun(0,len);
			if(tipPokers.lenth > 0){
				break;
			}
		}
		// 找双顺
		if(len >= 10){
			tipPokers = tips->getShuangShun(0,len);
			if(tipPokers.lenth > 0){
				break;
			}
		}
		// 找单顺
		tipPokers = tips->getShunzi(0, len);
		if( tipPokers.lenth > 0 ){
			break;
		}
	}
	delePokerData(withNolaizi);

    Vector<PokerObject*> pokerDataArr = PokerUtil::getPokerArrFromPokers(tipPokers);
    delete tips;
    CCLOG("the getMoveTipsPokers count==%d", pokerDataArr.size());
    for (int i = 0; i < pokerDataArr.size(); i++)
    {
        CCLOG("Trace::PokerTips::getMoveTipsPokers result arr(%d)=%d", pokerDataArr.at(i)->getPokerNum());
    }
    //pokerDataArr->retain();
    return pokerDataArr;
}

bool PokerTips::checkPokerAvaild(cocos2d::Vector<PokerObject*> lastPlayerPokers, cocos2d::Vector<PokerObject*> seletPokers)
{
	for ( int i = 0; i < lastPlayerPokers.size(); i ++)
	{
		CCLOG("Trace::PokerTips::checkPokerAvaild lastPlayerPokers(%d), num = %d", i, lastPlayerPokers.at(i)->getPokerNum());
	}

	for ( int i = 0; i < seletPokers.size(); i ++)
	{
		CCLOG("Trace::PokerTips::checkPokerAvaild seletPokers(%d), num = %d", i, seletPokers.at(i)->getPokerNum());
	}
    bool isCanOut = false;
    PokerTips* tip = new PokerTips();
//    kPokerUint_8 lastData = PokerUtil::getPokersFromArr(lastPlayerPokers);
//    kPokerUint_8 seletData = PokerUtil::getPokersFromArr(seletPokers);
//    isCanOut = tip->checkPokerAvaild(lastData, seletData);

    //add by chenzc
	//以下代码的逻辑全都来自PokerTips::checkPokerAvaild(kPokerUint_8 lastPlayerPokers, kPokerUint_8 seletPokers)方法，
	//只是一些癞子的检测换成了使用lastPlayerPokers里面的PokerObject数据进行检测，避免出现癞子变色出现损耗，导致牌型检测不准确的问题
    PokerTypeCheck *check = new PokerTypeCheck();
	kWeightValue lastWeight = weight_Unknown;
	kWeightValue seletWeight = weight_Unknown;

	if (check->getLaiziCount(seletPokers) > 0)
	{
		if (getAllPokersCanPlay(lastPlayerPokers, seletPokers).size() > 0)
		{
			isCanOut = true;
		}
	}
	else
	{
		lastWeight = check->getPokerType(lastPlayerPokers);
		CCLOG("checkPokerAvaild  lastWeight type=%d,number=%d,pokerCount=%d", lastWeight.type, lastWeight.number, lastWeight.pokerCount);
		seletWeight = check->getPokerType(seletPokers);
		CCLOG("checkPokerAvaild  seletWeight type=%d,number=%d,pokerCount=%d", seletWeight.type, seletWeight.number, seletWeight.pokerCount);
		isCanOut = PokerUtil::compareWeightValueIsBig(lastWeight, seletWeight);
	}


	delete check;
	CCLOG("Trace::PokerTips::checkPokerAvaild result = %d", isCanOut);
    return isCanOut;
}

bool PokerTips::checkPokerAvaild(kPokerUint_8 lastPlayerPokers, kPokerUint_8 seletPokers)
{
    printUint8_t("checkPokerAvaild lastPlayerPokers", lastPlayerPokers);
    printUint8_t("checkPokerAvaild seletPokers", seletPokers);
    bool isCanOut = false;
    PokerTypeCheck *check = new PokerTypeCheck();
    kWeightValue lastWeight = weight_Unknown;
    kWeightValue seletWeight = weight_Unknown;
    
    if (__check->getLaiziCount(seletPokers) > 0)
    {
        if (getAllPokersCanPlay(lastPlayerPokers, seletPokers).size() > 0)
        {
            isCanOut = true;
        }
    }
    else
    {
        lastWeight = check->getPokerType(lastPlayerPokers);
        CCLOG("checkPokerAvaild  lastWeight type=%d,number=%d,pokerCount=%d", lastWeight.type, lastWeight.number, lastWeight.pokerCount);
        seletWeight = check->getPokerType(seletPokers);
        CCLOG("checkPokerAvaild  seletWeight type=%d,number=%d,pokerCount=%d", seletWeight.type, seletWeight.number, seletWeight.pokerCount);
        isCanOut = PokerUtil::compareWeightValueIsBig(lastWeight, seletWeight);
    }

    
    delete check;
    return isCanOut;  
}

uint8_t PokerTips::getPokerPoint(kPokerUint_8 &myPokers)
{
    uint8_t num = PokerUtil::getPokerNum(myPokers.data[0]);
    assert(num > 0);
    return num;
}

void PokerTips::resetLaiziData(kPokerUint_8 &myPokers)
{
	__check->signLaiziPoker(myPokers);//将癞子牌标记
	__laiziCount = __check->getLaiziCount(myPokers);//得到癞子的个数
	__laiziPokerData = __check->getLaiziPokers(myPokers, __laiziCount);//将癞子拿出来
}

/*
 
 提示顺序 单张 -> 对子 -> 三张 -> 四张
 __sbCountPool 已经按张数从小到大，点数从小到大排列
 */
kPokerUint_8 PokerTips::getPokersWithUnKnow()
{
    kPokerUint_8 tipPokers = zero_pokerData;
	
	bool isNotFind = true;
	for( std::map<int, kPokerUint_8>::iterator i = __sbCountPool.begin(); i != __sbCountPool.end() && isNotFind; i++){
		uint8_t value = i->second.data[0];
		uint8_t point = PokerUtil::getPokerNum(value);
		kPokerUint_8 pokers = i->second;
		
        if (__pokerStatue.find(value) != __pokerStatue.end())
        {
            continue;
        }
		
		if( !(pokers.lenth > 0) ){
			continue;
		}

		tipPokers = pokers;
		isNotFind = false;
		break;
	}
	
    signPokerStatue(tipPokers);

    return  tipPokers;
}


 kPokerUint_8 PokerTips::getCountSamePoker(int pokerPoint, int count, int lastPoint)
{
    kPokerUint_8 tipPokers = zero_pokerData;

	int startCount = 0;
	//得到上一次提示的遍历牌张数
	bool isNotFind = true;
	if( pokerPoint > 0 && pokerPoint != lastPoint )
	{
		for( std::map<int, kPokerUint_8>::iterator i = __sbCountPool.begin(); i != __sbCountPool.end() && isNotFind; i++){
			/*kPokerUint_8 pokerUint = i->second;
			printUint8_t("map pokeruint", pokerUint);*/
			uint8_t value = i->second.data[0];
			uint8_t point = PokerUtil::getPokerNum(value);
			if( point == pokerPoint ){
				startCount = i->second.lenth;
				isNotFind = false;
				break;
			}
		}
	}
	int endCount = 4;
	if( startCount>0 ){
		endCount = startCount;
	}else{
		startCount = count;
	}

	isNotFind = true;
	for(int turnIndex = 1; turnIndex < 3 && isNotFind; turnIndex++){

		int checkPoint = turnIndex == 1 ? pokerPoint : lastPoint;

		if(turnIndex == 2 && lastPoint > 0){
			startCount += 1;
			endCount = 4;
		}

		isNotFind = true;
		for( std::map<int, kPokerUint_8>::iterator i = __sbCountPool.begin(); i != __sbCountPool.end() && isNotFind; i++)
		{
			uint8_t value = i->second.data[0];
			uint8_t point = PokerUtil::getPokerNum(value);
			
            if (__pokerStatue.find(value) != __pokerStatue.end())
            {
                continue;
            }

			kPokerUint_8 pokers = i->second;
			if( !(pokers.lenth > 0) ){
				continue;
			}else if( pokers.lenth < startCount || pokers.lenth > endCount ){
				continue;
			}

			if ( point > checkPoint )
			{
				if (pokers.lenth == count )
				{//刚好 count 个
					tipPokers = pokers;
					isNotFind = false;
					break;
				}
				else if (pokers.lenth > count)
				{
					tipPokers = getPokersWithCount(pokers, count);
					isNotFind = false;
					break;
				}
			}
		}
	}
	
    signPokerStatue(tipPokers);

	return tipPokers;
}

kPokerUint_8 PokerTips::getOnePoker(int pokerPoint, int lastPoint)
{
	kPokerUint_8 tipPokers = getCountSamePoker(pokerPoint, 1, lastPoint);

    if (__laiziCount > 0)
    {
        if (tipPokers.lenth == 0 && PokerUtil::getPokerNum(__laiziPokerData.data[0]) > pokerPoint)
        {
            //如果以上还未选中，则从癞子中选一张
            tipPokers = addPokersWithPlanCount(tipPokers, __laiziPokerData, 1);
        }
    }

    signPokerStatue(tipPokers);

    printPokerNum("getOnePoker", tipPokers);
    return tipPokers;
}

/*
 1、先寻找有无对牌
 2、如果没有对牌，则拆大于两站的牌
 3、如果还没有则将癞子加上
 */
kPokerUint_8 PokerTips::getTwoPokers(int pokerPoint, int lastPoint)
{
    if (pokerPoint == R2)//如果上家直接出对2，那就不需要再去遍历了
    {
        return zero_pokerData;
    }
	
	kPokerUint_8 tipPokers = getCountSamePoker(pokerPoint, 2, lastPoint);

    if (tipPokers.lenth == 0)
    {
        for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
        {
            uint8_t value = i->second.data[0];
            uint8_t point = i->first;
            //CCLOG("the point sign statue--%d",__pokerStatue.find(point)->second);
            if (__pokerStatue.find(value) != __pokerStatue.end())
            {
                continue;
            }
            kPokerUint_8 pokers = i->second;
            //此处用于复制一个__pokerPool中的字符串数组，防止多处释放，保证__pokerPool中的对象只在本类的析构函数中释放
            uint8_t* pokersCp = (uint8_t*)malloc(pokers.lenth);
            memcpy(pokersCp, pokers.data, pokers.lenth);
            kPokerUint_8 pokersCopy = PokerUint8_tMake(pokersCp, pokers.lenth, true);
            
            if (pokersCopy.lenth == 1 && point > pokerPoint && point < R_WA)//满足条件一个单牌点数大于上家，且小于小王
            {
                if (__laiziPokerData.lenth > 0)
                {
                   tipPokers = addPokersWith(pokersCopy, __laiziPokerData, 1);
                }
                break;
            }
        }
    }
    
    signPokerStatue(tipPokers);
    printPokerNum("getTwoPokers", tipPokers);
    return PokerUint8_tMake(tipPokers.data, tipPokers.lenth);
}

kPokerUint_8 PokerTips::getThreePokers(int pokerPoint,int lastPoint)
{
    if (pokerPoint == R2)//如果上家直接出3个2，那就不需要再去遍历了
    {
        return zero_pokerData;
    }

	kPokerUint_8 tipPokers = getCountSamePoker(pokerPoint, 3, lastPoint);

    if (tipPokers.lenth == 0)
    {
        for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
        {
            uint8_t point = i->first;
            kPokerUint_8 pokers = i->second;
            //此处用于复制一个__pokerPool中的字符串数组，防止多处释放，保证__pokerPool中的对象只在本类的析构函数中释放
            uint8_t* pokersCp = (uint8_t*)malloc(pokers.lenth);
            memcpy(pokersCp, pokers.data, pokers.lenth);
            kPokerUint_8 pokersCopy = PokerUint8_tMake(pokersCp, pokers.lenth, true);
            
            int needCount = 3 - pokersCopy.lenth;
            if (point > pokerPoint
            		&& (point >= R3 && point <= R2))
            {
                tipPokers = addPokersWith(pokersCopy, __laiziPokerData, needCount);
                //修正找3张，有癞子的情况下，出现不匹配的时候，没有继续循环查找的问题，导致牌型提示不正确
                if(tipPokers.lenth != 0){
                	break;
                }
            }
        }
    }
    
    signPokerStatue(tipPokers);
    lessLaiziCount(tipPokers);
    printPokerNum("getThreePokers", tipPokers);
    return tipPokers;
}

kPokerUint_8 PokerTips::getBombPokers(int pokerPoint, int type)
{
    kPokerUint_8 tipPokers = zero_pokerData;

    //先找软炸
    if (type == T_BOMB_S || type == T_Unknow)
    {
        if (tipPokers.lenth == 0)
        {
            for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
            {
                uint8_t point = i->first;
                if (point >= R_WA)
                {
                    break;
                }
                if (point <= pokerPoint)
                {
                	//点数要大于上一个炸
                    continue;
                }
                kPokerUint_8 pokers = i->second;
                //此处用于复制一个__pokerPool中的字符串数组，防止多处释放，保证__pokerPool中的对象只在本类的析构函数中释放
                uint8_t* pokersCp = (uint8_t*)malloc(pokers.lenth);
                memcpy(pokersCp, pokers.data, pokers.lenth);
                kPokerUint_8 pokersCopy = PokerUint8_tMake(pokersCp, pokers.lenth, true);
                
				int needCount = 4 - pokersCopy.lenth;//癞子炸需要的癞子个数
				if(needCount <= __laiziCount){
					kPokerUint_8 sumPokers = addPokersWithPlanCount(pokersCopy, __laiziPokerData, needCount); //加上癞子牌之后的牌
					if (point > pokerPoint && sumPokers.lenth == 4)
					{
						return sumPokers;
					}
					else
					{
						__laiziPokerData.lenth = __laiziCount;//没有合法的牌型，则把减去的癞子加上去
					}
				}
            }
        }
    }
    //重置癞子
    lessLaiziCount(tipPokers);

    //再找硬炸
    if (type == T_BOMB || type == T_BOMB_S || type == T_Unknow)
	{
		for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
		{
			uint8_t point = i->first;
			kPokerUint_8 pokers = i->second;
			if (pokers.lenth == 4)
			{
				//如果获得比软炸大的炸，则任何硬炸都可以
				if (type == T_BOMB_S)
				{
					return pokers;
				}

				if (point > pokerPoint)
				{
					return pokers;
				}
			}
		}
	}

    //接着找癞子炸
    if (type == T_BOMB_L || type == T_BOMB || type == T_BOMB_S || type == T_Unknow)
    {
    	kPokerUint_8 pokersCopy = zero_pokerData;

		kPokerUint_8 sumPokers = addPokersWith(pokersCopy, __laiziPokerData, 4);//加上癞子牌之后的牌
		if (sumPokers.lenth == 4)
		{
			return sumPokers;
		}
    }
    //重置癞子
    lessLaiziCount(tipPokers);

    //最后找王炸
    if(type == T_Rocket_2 || type == T_BOMB_L || type == T_BOMB || type == T_BOMB_S || type == T_Unknow)
    {
    	tipPokers = getRocketPokers();
    }
    printPokerNum("getBombPokers", tipPokers);
    return tipPokers;
    
}

kPokerUint_8 PokerTips::getRocketPokers()
{
    kPokerUint_8 tipPokers = zero_pokerData;
    uint8_t* rocket = (uint8_t*)malloc(2);
    memset(rocket, 0, 2*sizeof(uint8_t));
    for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
    {
        uint8_t point = i->first;
        CCLOG("the point---%d", point);
        kPokerUint_8 pokers = i->second;
        if (point == R_WA)
        {
            rocket[0] = pokers.data[0];
        }

        if (point == R_WB)
        {
            rocket[1] = pokers.data[0];
        }

    }
    
    tipPokers = PokerUint8_tMake(rocket, 2, true);
    printPokerNum("getRocketPokers", tipPokers);
    if (!PokerUtil::checkDataLegal(tipPokers)) {
        tipPokers = zero_pokerData;
    }
    return tipPokers;
}
/*
 顺子
 1、先检测非癞子
 2、非癞子没有则将癞子加入继续检测
 */
kPokerUint_8 PokerTips::getShunzi(int pokerPoint, int pokerCount)
{
    kPokerUint_8 tipPokers = zero_pokerData;
    if (pokerPoint >= R10)
    {
    	//如果顺子为10,J,Q,K,A，则不再获取顺子提示了
        return tipPokers;
    }
   
    for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
    {
        
        uint8_t pointi = i->first;
        uint8_t lastnum = pointi;
        kPokerUint_8 pokersi = i->second;
        
        int index = 1;
        if (pointi <= pokerPoint || pointi >= R1)
        {
        	//如果迭代器i元素不存在，则直接继续下一轮遍历
            continue;
        }
        CCLOG("getShunzi no laizi pointi==%d",pointi);
        uint8_t* shunzi = (uint8_t*)malloc(pokerCount);
        shunzi[0] = pokersi.data[0];
        for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.find(pointi + 1); j != __pokerPool.end(); j++)
        {
            kPokerUint_8 pokersj = j->second;
            uint8_t pointj = j->first;
            if (pointj > R1)
            {
                break;
            }
            CCLOG("get Shunzi no laizi pointj==%d",pointj);
            if (pointj - lastnum == 1)
            {
                shunzi[index] = pokersj.data[0];//符合等差数列条件，则将j依次放入
                lastnum = pointj;
                index++;
            }

            if (index == pokerCount)
            {
                tipPokers = PokerUint8_tMake(shunzi, index, true);
                return tipPokers;
            }
            
        }

    }
    //无癞子不行就加入癞子
    if (tipPokers.lenth == 0 && __laiziCount > 0)
    {
        for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
        {
            uint8_t pointi = i->first;
            if(pointi <= pokerPoint || pointi >= R1)
            {
                continue;
            }
            kPokerUint_8 pokersi = i->second;
            uint8_t lastPoint = pointi;
            uint8_t* shunzi = (uint8_t*)malloc(pokerCount);
            kPokerUint_8 shunPokers = PokerUint8_tMake(shunzi, 0, true);
            int index = 1;
            shunPokers = addPokersWithPlanCount(shunPokers, pokersi, 1);
            
            index = shunPokers.lenth;
            //printPokerNum("shuangshun sshunpokers", sshunPokers);
            
            int addLaiziCount = 0;
            for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.begin(); j != __pokerPool.end(); j++)
            {
//            	addLaiziCount = 0;
                uint8_t pointj = j->first;
                if (pointi <= pokerPoint || pointi >= R1)
                {
                    delePokerData(shunPokers);
                    break;
                }
                if (pointj <= pointi)
                {
                    continue;
                }

                if (pointj - pointi <= pokerCount && pointi + pokerCount - 1 <= R1)
                {
                    if (pointj - lastPoint == 1)
                    {
                        kPokerUint_8 pokersj = j->second;
                        shunPokers = addPokersWithPlanCount(shunPokers, pokersj, 1);
                        index = shunPokers.lenth;
                        lastPoint = pointj;
                    }
                    else
                    {
                    	int addCount = (pokerCount - index) < (pointj - lastPoint - 1) ? (pokerCount - index) : (pointj - lastPoint - 1);
                    	addLaiziCount += addCount;
                    	if(addLaiziCount <= __laiziCount)
                    	{
                    		lastPoint += addCount;
                    		shunPokers = addPokersWithPlanCount(shunPokers, __laiziPokerData, addCount);
                    		index = shunPokers.lenth;
                    		if (index < pokerCount)
                    		{
                    			kPokerUint_8 pokersj = j->second;
								shunPokers = addPokersWithPlanCount(shunPokers, pokersj, 1);
								index = shunPokers.lenth;
								lastPoint = pointj;
                    		}
                    	}
                    	else
                    	{
							index=1;
							delePokerData(shunPokers);
							break;
                    	}
                    }

                }

                else
                {
                    index=1;
                    delePokerData(shunPokers);
                    break;
                }
                if (index == pokerCount)
                {
                    tipPokers = shunPokers;
                    return tipPokers;
                }
            }
            __laiziPokerData.lenth = __laiziCount;//没有合法的牌型，则把减去的癞子加上去
            delePokerData(shunPokers);
        }
    }
    return tipPokers;
}

kPokerUint_8 PokerTips::getShuangShun(int pokerPoint, int pokerCount)
{
    kPokerUint_8 tipPokers = zero_pokerData;
    if (pokerPoint >= RQ)
    {
    	//如果双顺QQ,KK,AA，则不再获取顺子提示了
        return tipPokers;
    }
    
    for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
    {
        uint8_t* sshun = (uint8_t*)malloc(pokerCount);
        uint8_t pointi = i->first;
        uint8_t lastPoint = pointi;
        kPokerUint_8 pokersi = i->second;
        if (pokersi.lenth < 2 || pointi <= pokerPoint || pointi >= R1)
        {
            free(sshun);
            continue;
        }
       
        int index = 2;
        CCLOG("cacacacaca and pointi==%d", pointi);
        sshun[0] = pokersi.data[0];//先将i中放入一对
        sshun[1] = pokersi.data[1];
        for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.find(pointi + 1); j != __pokerPool.end(); j++)
        {
            uint8_t pointj = j->first;
            if (pointj > R1)
            {
                break;
            }
            kPokerUint_8 pokersj = j->second;

            if (lastPoint == pointj - 1 && pokersj.lenth >= 2)
            {
                CCLOG("the index==%d",index);
                sshun[index] = pokersj.data[0];//符合等差数列条件，则将j依次放入
                sshun[index+1] = pokersj.data[1];
                lastPoint = pointj;//将当前的数字缓存，便于下一次判断
                index+=2;
                CCLOG("the shuangshun pointj==%d index ==%d",pointj,index);
            }


            if (index==pokerCount)
            {
                tipPokers = PokerUint8_tMake(sshun,index,true);
                return tipPokers;
            }

            
        }
        
    }
    
    if (tipPokers.lenth == 0 && __laiziCount > 0)
    {
        for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
        {
            uint8_t pointi = i->first;
            if (pointi >= R2 || pointi <= pokerPoint)
            {
                continue;
            }
            uint8_t lastPoint = pointi;
            kPokerUint_8 pokersi = i->second;
            uint8_t* sshuni = (uint8_t*)malloc(pokerCount);
            kPokerUint_8 sshunPokers = PokerUint8_tMake(sshuni, 0, true);
            
            int index = 0;
            sshunPokers = addPokersWithPlanCount(sshunPokers, pokersi, 2);
            index = sshunPokers.lenth;
            //printPokerNum("shuangshun sshunpokers", sshunPokers);
            
            int addLaiziCount = 0;
            for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.begin(); j != __pokerPool.end(); j++)
            {
                uint8_t pointj = j->first;
                if (pointj >= R2 || pointj <= pokerPoint)
                {
                    delePokerData(sshunPokers);
                    break;
                }
                if (pointj <= pointi)
                {
                    continue;
                }
                
                //1、两个点数的间隔的两倍应该不大于癞子的个数,而且两个数的差值应该小于牌数的二分之一,第一次循环的时候不能进入这个条件判断
                // && (pointj - lastPoint) * 2 <= pokerCount
                if ((pointj - lastPoint - 1)*2 <= __laiziCount)
                {
                    if ((pointj-PokerUtil::getPokerNum(sshunPokers.data[0]))*2<pokerCount)
                    {
                        kPokerUint_8 pokersj = j->second;
                        sshunPokers = addPokersWithPlanCount(sshunPokers, pokersj, 2);
                        index = sshunPokers.lenth;
                        
                        lastPoint = pointj;
                        sshunPokers = addPokersWithPlanCount(sshunPokers, __laiziPokerData, pokerCount - index);
                        addLaiziCount = sshunPokers.lenth - index;//将加上的癞子数保存，用于牌型不合法的时候恢复癞子数
                        index = sshunPokers.lenth;
                    }
                    else
                    {
                        delePokerData(sshunPokers);
                        break;
                    }
                    //printPokerNum("add laizi sshunpokers", sshunPokers);
                }
                
                else
                {
                    delePokerData(sshunPokers);
                    break;
                }
                if (index == pokerCount)
                {
                    //printPokerNum("right sshunPokers", sshunPokers);
                    tipPokers = sshunPokers;
                    return tipPokers;
                }
            }
            __laiziPokerData.lenth = __laiziCount;//没有合法的牌型，则把减去的癞子加上去
            delePokerData(sshunPokers);
            
        }
    }
    return tipPokers;
}

kPokerUint_8 PokerTips::getSanShun(int pokerPoint, int pokerCount)
{
    kPokerUint_8 tipPokers = zero_pokerData;
    if (pokerPoint >= RK)
    {
    	//如果三顺为KK,AA则不再获取提示
        return tipPokers;
    }
    uint8_t* sanshuni = (uint8_t*)malloc(pokerCount);
    for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
    {
        uint8_t pointi = i->first;
        uint8_t lastPoint = pointi;
        kPokerUint_8 pokersi = i->second;
        if (pokersi.lenth < 3 || pointi <= pokerPoint || pointi >= R1)
        {
            continue;
        }
        
        int index = 3;
        CCLOG("cacacacaca and pointi==%d",pointi);
        sanshuni[0] = pokersi.data[0];//先将i中放入三张
        sanshuni[1] = pokersi.data[1];
        sanshuni[2] = pokersi.data[2];
        for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.begin(); j != __pokerPool.end(); j++)
        {
            uint8_t pointj = j->first;
            if (pointj > R1)
            {
                break;
            }
            kPokerUint_8 pokersj = j->second;
            if (pokersj.lenth < 3)
            {
                continue;
            }
            if (lastPoint == pointj - 1)
            {
                CCLOG("the index==%d",index);
                sanshuni[index] = pokersj.data[0];//符合等差数列条件，则将j依次放入
                sanshuni[index+1] = pokersj.data[1];
                sanshuni[index+2] = pokersj.data[2];
                lastPoint = pointj;//将当前的数字缓存，便于下一次判断
                index += 3;
                CCLOG("the shuangshun pointj==%d index ==%d",pointj,index);
            }

            if (index == pokerCount)
            {
                tipPokers = PokerUint8_tMake(sanshuni, index, true);
                return tipPokers;
            }
            
        }
        
    }
    free(sanshuni);

    if (tipPokers.lenth == 0 && __laiziCount > 0)
    {
        for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
        {
            uint8_t pointi = i->first;
            if (pointi <= pokerPoint || pointi >= R2)
            {
                continue;
            }
            uint8_t lastPoint = pointi;
            kPokerUint_8 pokersi = i->second;
            uint8_t* sanshuni = (uint8_t*)malloc(pokerCount);
            kPokerUint_8 sanshunPokers = PokerUint8_tMake(sanshuni, 0, true);
            
            int index = 0;
            sanshunPokers = addPokersWithPlanCount(sanshunPokers, pokersi, 3);
            index = sanshunPokers.lenth;
            printPokerNum("sanshunPokers", sanshunPokers);
            
            int addLaiziCount = 0;
            for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.begin(); j != __pokerPool.end(); j++)
            {
                uint8_t pointj = j->first;
                if (pointj <= pokerPoint || pointj >= R2)
                {
                    delePokerData(sanshunPokers);
                    break;
                }
                if (pointj <= pointi)
                {
                    continue;
                }
                
                if ((pointj - lastPoint - 1) * 3 <= __laiziCount && (pointj - lastPoint) * 3 <= pokerCount)
                {
                    if ((pointj-PokerUtil::getPokerNum(sanshunPokers.data[0]))*3<pokerCount)
                    {
                        kPokerUint_8 pokersj = j->second;
                        sanshunPokers = addPokersWithPlanCount(sanshunPokers, pokersj, 3);
                        index = sanshunPokers.lenth;
                        
                        lastPoint = pointj;
                        sanshunPokers = addPokersWithPlanCount(sanshunPokers, __laiziPokerData, pokerCount - index);
                        addLaiziCount = sanshunPokers.lenth - index;//将加上的癞子数保存，用于牌型不合法的时候恢复癞子数
                        index = sanshunPokers.lenth;
                        printPokerNum("add laizi sanshunPokers", sanshunPokers);
                    }
                    else
                    {
                        delePokerData(sanshunPokers);
                        break;
                    }
                    
                }
                
                else
                {
                    delePokerData(sanshunPokers);
                    break;
                }
                if (index == pokerCount)
                {
                    printPokerNum("right sanshunPokers", sanshunPokers);
                    tipPokers = sanshunPokers;
                    return tipPokers;
                }
            }
            __laiziPokerData.lenth = __laiziCount;//没有合法的牌型，则把减去的癞子加上去
            delePokerData(sanshunPokers);
        }
    }

    return tipPokers;
}

kPokerUint_8 PokerTips::getFeijiDan(int pokerPoint, int pokerCount)
{
    uint8_t* fejiDan = (uint8_t*)malloc(pokerCount);
    
    kPokerUint_8 tipPokers = zero_pokerData;
    
    int groupCount = pokerCount / 4;
    int sanShunCount = groupCount * 3;
    kPokerUint_8 feijiDanPokers = PokerUint8_tMake(fejiDan, 0, true);
    kPokerUint_8 sanshunPokers = getSanShun(pokerPoint, sanShunCount);
    printPokerNum("getFeijiDan sanshunPokers", sanshunPokers);
    signPokerStatue(sanshunPokers);
    if (sanshunPokers.lenth == sanShunCount)
    {
        feijiDanPokers = addPokersWithPlanCount(feijiDanPokers, sanshunPokers, sanShunCount);
    }
    
    for (int i = 0; i < groupCount; i++)
    {
        kPokerUint_8 danTip = getOnePoker(0);
        feijiDanPokers = addPokersWithPlanCount(feijiDanPokers, danTip, 1);
    }
    
    if (feijiDanPokers.lenth == pokerCount)
    {
        tipPokers = feijiDanPokers;
    }
    return tipPokers;
}

kPokerUint_8 PokerTips::getFeijiShuang(int pokerPoint, int pokerCount)
{
    uint8_t* fejiDan = (uint8_t*)malloc(pokerCount);
    
    kPokerUint_8 tipPokers = zero_pokerData;
    
    int groupCount = pokerCount / 4;
    int sanShunCount = groupCount * 3;
    kPokerUint_8 feijiDanPokers = PokerUint8_tMake(fejiDan, 0, true);
    kPokerUint_8 sanshunPokers = getSanShun(pokerPoint, sanShunCount);
   // printPokerNum("sanshunPokers", sanshunPokers);
    signPokerStatue(sanshunPokers);
    if (sanshunPokers.lenth == sanShunCount)
    {
        feijiDanPokers = addPokersWithPlanCount(feijiDanPokers, sanshunPokers, sanShunCount);
    }
    
    for (int i = 0; i < groupCount; i++)
    {
        kPokerUint_8 twoTip = getTwoPokers(0);
        feijiDanPokers = addPokersWithPlanCount(feijiDanPokers, twoTip, 2);
    }
    
    if (feijiDanPokers.lenth == pokerCount)
    {
        tipPokers = feijiDanPokers;
    }
    return tipPokers;
}

kPokerUint_8 PokerTips::getSanDai(int pokerPoint, int pokerCount)
{
	
    kPokerUint_8 tipPokers = zero_pokerData;
    if (pokerPoint >= R2)
    {
    	//如果三带一为 222 + 则不再获取提示
        return tipPokers;
    }
    uint8_t* sandai = (uint8_t*)malloc(pokerCount);
    
    kPokerUint_8 sandaiPokers = PokerUint8_tMake(sandai, 0, true);
    kPokerUint_8 threePokers = getThreePokers(pokerPoint,pokerPoint);
    sandaiPokers = addPokersWithPlanCount(sandaiPokers, threePokers, 3);
    signPokerStatue(sandaiPokers);
    printPokerNum("sandaiPokers", sandaiPokers);
    
    if (pokerCount == 4)
    {
    	//带单张
        if (sandaiPokers.lenth == 3)
        {
            kPokerUint_8 daiTip = getOnePoker(0);
            sandaiPokers = addPokersWithPlanCount(sandaiPokers, daiTip, 1);
        }
    }
    else if (pokerCount == 5)//带一对
    {
        if (sandaiPokers.lenth == 3)
        {
            kPokerUint_8 daiTip = getTwoPokers(0);
            printPokerNum("daiTip", daiTip);
            sandaiPokers = addPokersWithPlanCount(sandaiPokers, daiTip, 2);

        }
    }
    
    if (sandaiPokers.lenth == pokerCount)
    {
        tipPokers = sandaiPokers;
        printPokerNum("sandaiTippokers", tipPokers);
    }
    return tipPokers;
}


/****************************************************/
/******************   点牌提示相关   ****************/
/****************************************************/

/*
 1、找相应点数取两张牌
 2、如果还没有则将癞子加上
 */
kPokerUint_8 PokerTips::getTwoPokersByPart(int pokerPoint)
{
    kPokerUint_8 tipPokers = zero_pokerData;
    for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
    {

        uint8_t value = i->second.data[0];
        uint8_t point = i->first;
        //CCLOG("the point sign statue--%d",__pokerStatue.find(point)->second);
        if (__pokerStatue.find(value) != __pokerStatue.end())
        {//被标记过的就下一轮
            continue;
        }
        kPokerUint_8 pokers = i->second;
        printPokerNum("getTwoPokersByPart pokers", pokers);
		if( point == pokerPoint ){
			//判断是否当前牌张数是两个以上
			if (pokers.lenth == 2)
			{
				tipPokers = pokers;
				printPokerNum("getTwoPokersByPart", tipPokers);
			}else if(pokers.lenth > 2)
			{
				tipPokers = getPokersWithCount(pokers, 2);
				printPokerNum("getTwoPokersByPart", tipPokers);
			}
			else
			{
				//加上癞子牌
				kPokerUint_8 pokers = i->second;
				//此处用于复制一个__pokerPool中的字符串数组，防止多处释放，保证__pokerPool中的对象只在本类的析构函数中释放
				uint8_t* pokersCp = (uint8_t*)malloc(pokers.lenth);
				memcpy(pokersCp, pokers.data, pokers.lenth);
				kPokerUint_8 pokersCopy = PokerUint8_tMake(pokersCp, pokers.lenth, true);
            
				if (pokersCopy.lenth == 1 && point < R_WA)//满足条件一个单牌点数大于上家，且小于小王
				{
					if (__laiziPokerData.lenth > 0)
					{
					   tipPokers = addPokersWith(pokersCopy, __laiziPokerData, 1);
					}
					break;
				}
			}
			break;
		}
    }

    signPokerStatue(tipPokers);
    printPokerNum("getTwoPokersByPart", tipPokers);
    return PokerUint8_tMake(tipPokers.data, tipPokers.lenth);
}

kPokerUint_8 PokerTips::getThreePokersByPart(int pokerPoint)
{
    kPokerUint_8 tipPokers = zero_pokerData;
    for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
    {
        uint8_t point = i->first;
        kPokerUint_8 pokers = i->second;
		
		if( point == pokerPoint ){
			if (pokers.lenth > 3 && (point >= R3 && point <= R2))
			{
				tipPokers = getPokersWithCount(pokers, 3);
				printPokerNum("getThreePokersByPart", tipPokers);
			}
			else if(pokers.lenth == 3 && (point >= R3 && point <= R2))
			{
				tipPokers = pokers;
				printPokerNum("getThreePokersByPart", tipPokers);
			}
			else
			{
				 //此处用于复制一个__pokerPool中的字符串数组，防止多处释放，保证__pokerPool中的对象只在本类的析构函数中释放
				uint8_t* pokersCp = (uint8_t*)malloc(pokers.lenth);
				memcpy(pokersCp, pokers.data, pokers.lenth);
				kPokerUint_8 pokersCopy = PokerUint8_tMake(pokersCp, pokers.lenth, true);
            
				int needCount = 3 - pokersCopy.lenth;
				if (point >= R3 && point <= R2)
				{
					tipPokers = addPokersWith(pokersCopy, __laiziPokerData, needCount);
					//修正找3张，有癞子的情况下，出现不匹配的时候，没有继续循环查找的问题，导致牌型提示不正确
					if(tipPokers.lenth != 0){
                		break;
					}
				}
			}
			break;
		}
    }

    lessLaiziCount(tipPokers);
    printPokerNum("getThreePokersByPart", tipPokers);
    return tipPokers;
}

kPokerUint_8 PokerTips::getBombPokersByPart(int pokerPoint, int type)
{
    kPokerUint_8 tipPokers = zero_pokerData;

    //先找软炸
    if (type == T_BOMB_S || type == T_Unknow)
    {
        if (tipPokers.lenth == 0)
        {
            for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
            {
                uint8_t point = i->first;
                if (point >= R_WA)
                {
                    break;
                }
                if (point != pokerPoint)
                {
                	//不是对应点数
                    continue;
                }
                kPokerUint_8 pokers = i->second;
                //此处用于复制一个__pokerPool中的字符串数组，防止多处释放，保证__pokerPool中的对象只在本类的析构函数中释放
                uint8_t* pokersCp = (uint8_t*)malloc(pokers.lenth);
                memcpy(pokersCp, pokers.data, pokers.lenth);
                kPokerUint_8 pokersCopy = PokerUint8_tMake(pokersCp, pokers.lenth, true);
                
				int needCount = 4 - pokersCopy.lenth;//癞子炸需要的癞子个数
				if(needCount <= __laiziCount){
					kPokerUint_8 sumPokers = addPokersWithPlanCount(pokersCopy, __laiziPokerData, needCount); //加上癞子牌之后的牌
					if (sumPokers.lenth == 4)
					{
						return sumPokers;
					}
					else
					{
						__laiziPokerData.lenth = __laiziCount;//没有合法的牌型，则把减去的癞子加上去
					}
				}
            }
        }
    }

    //再找硬炸
    if (type == T_BOMB || type == T_BOMB_S || type == T_Unknow)
	{
		for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
		{
			uint8_t point = i->first;
			kPokerUint_8 pokers = i->second;
			if ( point == pokerPoint ){
				if(pokers.lenth == 4)
				{
					////如果获得比软炸大的炸，则任何硬炸都可以
					//if (type == T_BOMB_S)
					//{
						return pokers;
					//}
				}
				break;
			}
		}
	}

    //重置癞子
    lessLaiziCount(tipPokers);

    //最后找王炸
    if(type == T_Rocket_2 || type == T_BOMB_L || type == T_BOMB || type == T_BOMB_S || type == T_Unknow)
    {
		if( pokerPoint == R_WA ){
    		tipPokers = getRocketPokers();
		}
    }
    printPokerNum("getBombPokersByPart", tipPokers);

    return tipPokers;
}

/*
 顺子
 1、先检测非癞子
 2、非癞子没有则将癞子加入继续检测
 */
kPokerUint_8 PokerTips::getShunziByPart(int pokerPoint, int pokerCount)
{
    kPokerUint_8 tipPokers = zero_pokerData;
    if (pokerPoint >= R10)
    {
    	//如果顺子为10,J,Q,K,A，则不再获取顺子提示了
        return tipPokers;
    }
   
    for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
    {
        
        uint8_t pointi = i->first;
        uint8_t lastnum = pointi;
        kPokerUint_8 pokersi = i->second;
        
        int index = 1;
        if (pointi != pokerPoint || pointi >= R1)
        {
        	//如果迭代器i元素不存在，则直接继续下一轮遍历
            continue;
        }
        CCLOG("getShunziByPart no laizi pointi==%d",pointi);
        uint8_t* shunzi = (uint8_t*)malloc(pokerCount);
        shunzi[0] = pokersi.data[0];
        for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.find(pointi + 1); j != __pokerPool.end(); j++)
        {
            kPokerUint_8 pokersj = j->second;
            uint8_t pointj = j->first;
            if (pointj > R1)
            {
                break;
            }
            CCLOG("getShunziByPart no laizi pointj==%d",pointj);
            if (pointj - lastnum == 1)
            {
                shunzi[index] = pokersj.data[0];//符合等差数列条件，则将j依次放入
                lastnum = pointj;
                index++;
            }

            if (index == pokerCount)
            {
                tipPokers = PokerUint8_tMake(shunzi, index, true);
                return tipPokers;
            }
            
        }
		break; //只找对应顺子
    }

    //无癞子不行就加入癞子
    if (tipPokers.lenth == 0 && __laiziCount > 0)
    {
        for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
        {
            uint8_t pointi = i->first;
            if(pointi != pokerPoint || pointi >= R1)
            {
                continue;
            }
            kPokerUint_8 pokersi = i->second;
            uint8_t lastPoint = pointi;
            uint8_t* shunzi = (uint8_t*)malloc(pokerCount);
            kPokerUint_8 shunPokers = PokerUint8_tMake(shunzi, 0, true);
            int index = 1;
            shunPokers = addPokersWithPlanCount(shunPokers, pokersi, 1);
            
            index = shunPokers.lenth;
            //printPokerNum("shuangshun sshunpokers", sshunPokers);
            
            int addLaiziCount = 0;
            for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.begin(); j != __pokerPool.end(); j++)
            {
//            	addLaiziCount = 0;
                uint8_t pointj = j->first;
                if (pointi != pokerPoint || pointi >= R1)
                {
                    delePokerData(shunPokers);
                    break;
                }
                if (pointj <= pointi)
                {
                    continue;
                }

                if (pointj - pointi <= pokerCount && pointi + pokerCount - 1 <= R1)
                {
                    if (pointj - lastPoint == 1)
                    {
                        kPokerUint_8 pokersj = j->second;
                        shunPokers = addPokersWithPlanCount(shunPokers, pokersj, 1);
                        index = shunPokers.lenth;
                        lastPoint = pointj;
                    }
                    else
                    {
                    	int addCount = (pokerCount - index) < (pointj - lastPoint - 1) ? (pokerCount - index) : (pointj - lastPoint - 1);
                    	addLaiziCount += addCount;
                    	if(addLaiziCount <= __laiziCount)
                    	{
                    		lastPoint += addCount;
                    		shunPokers = addPokersWithPlanCount(shunPokers, __laiziPokerData, addCount);
                    		index = shunPokers.lenth;
                    		if (index < pokerCount)
                    		{
                    			kPokerUint_8 pokersj = j->second;
								shunPokers = addPokersWithPlanCount(shunPokers, pokersj, 1);
								index = shunPokers.lenth;
								lastPoint = pointj;
                    		}
                    	}
                    	else
                    	{
							index=1;
							delePokerData(shunPokers);
							break;
                    	}
                    }

                }

                else
                {
                    index=1;
                    delePokerData(shunPokers);
                    break;
                }
                if (index == pokerCount)
                {
                    tipPokers = shunPokers;
                    return tipPokers;
                }
            }
            __laiziPokerData.lenth = __laiziCount;//没有合法的牌型，则把减去的癞子加上去
            delePokerData(shunPokers);
			break; //只找对应顺子
        }
    }
    return tipPokers;
}

kPokerUint_8 PokerTips::getShuangShunByPart(int pokerPoint, int pokerCount)
{
    kPokerUint_8 tipPokers = zero_pokerData;
    if (pokerPoint >= RQ)
    {
    	//如果双顺QQ,KK,AA，则不再获取顺子提示了
        return tipPokers;
    }
    
    for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
    {
        uint8_t* sshun = (uint8_t*)malloc(pokerCount);
        uint8_t pointi = i->first;
        uint8_t lastPoint = pointi;
        kPokerUint_8 pokersi = i->second;
        if (pokersi.lenth < 2 || pointi != pokerPoint || pointi >= R1)
        {
            free(sshun);
            continue;
        }
       
        int index = 2;
        CCLOG("cacacacaca and pointi==%d", pointi);
        sshun[0] = pokersi.data[0];//先将i中放入一对
        sshun[1] = pokersi.data[1];
        for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.find(pointi + 1); j != __pokerPool.end(); j++)
        {
            uint8_t pointj = j->first;
            if (pointj > R1)
            {
                break;
            }
            kPokerUint_8 pokersj = j->second;

            if (lastPoint == pointj - 1 && pokersj.lenth >= 2)
            {
                CCLOG("the index==%d",index);
                sshun[index] = pokersj.data[0];//符合等差数列条件，则将j依次放入
                sshun[index+1] = pokersj.data[1];
                lastPoint = pointj;//将当前的数字缓存，便于下一次判断
                index+=2;
                CCLOG("the getShuangShunByPart pointj==%d index ==%d",pointj,index);
            }

            if (index==pokerCount)
            {
                tipPokers = PokerUint8_tMake(sshun,index,true);
                return tipPokers;
            }
        }
		break; //只找对应顺子
    }
    
    if (tipPokers.lenth == 0 && __laiziCount > 0)
    {
        for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
        {
            uint8_t pointi = i->first;
            if (pointi >= R2 || pointi != pokerPoint)
            {
                continue;
            }
            uint8_t lastPoint = pointi;
            kPokerUint_8 pokersi = i->second;
            uint8_t* sshuni = (uint8_t*)malloc(pokerCount);
            kPokerUint_8 sshunPokers = PokerUint8_tMake(sshuni, 0, true);
            
            int index = 0;
            sshunPokers = addPokersWithPlanCount(sshunPokers, pokersi, 2);
            index = sshunPokers.lenth;
            //printPokerNum("shuangshun sshunpokers", sshunPokers);
            
            int addLaiziCount = 0;
            for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.begin(); j != __pokerPool.end(); j++)
            {
                uint8_t pointj = j->first;
                if (pointj >= R2 || pointj <= pokerPoint)
                {
                    delePokerData(sshunPokers);
                    break;
                }
                if (pointj <= pointi)
                {
                    continue;
                }
                
                //1、两个点数的间隔的两倍应该不大于癞子的个数,而且两个数的差值应该小于牌数的二分之一,第一次循环的时候不能进入这个条件判断
                // && (pointj - lastPoint) * 2 <= pokerCount
                if ((pointj - lastPoint - 1)*2 <= __laiziCount)
                {
                    if ((pointj-PokerUtil::getPokerNum(sshunPokers.data[0]))*2<pokerCount)
                    {
                        kPokerUint_8 pokersj = j->second;
                        sshunPokers = addPokersWithPlanCount(sshunPokers, pokersj, 2);
                        index = sshunPokers.lenth;
                        
                        lastPoint = pointj;
                        sshunPokers = addPokersWithPlanCount(sshunPokers, __laiziPokerData, pokerCount - index);
                        addLaiziCount = sshunPokers.lenth - index;//将加上的癞子数保存，用于牌型不合法的时候恢复癞子数
                        index = sshunPokers.lenth;
                    }
                    else
                    {
                        delePokerData(sshunPokers);
                        break;
                    }
                    //printPokerNum("add laizi sshunpokers", sshunPokers);
                }
                
                else
                {
                    delePokerData(sshunPokers);
                    break;
                }
                if (index == pokerCount)
                {
                    //printPokerNum("right sshunPokers", sshunPokers);
                    tipPokers = sshunPokers;
                    return tipPokers;
                }
            }
            __laiziPokerData.lenth = __laiziCount;//没有合法的牌型，则把减去的癞子加上去
            delePokerData(sshunPokers);

			break; //只找对应顺子
        }
    }
    return tipPokers;
}

kPokerUint_8 PokerTips::getSanShunByPart(int pokerPoint, int pokerCount)
{
    kPokerUint_8 tipPokers = zero_pokerData;
    if (pokerPoint >= RK)
    {
    	//如果三顺为KK,AA则不再获取提示
        return tipPokers;
    }
    uint8_t* sanshuni = (uint8_t*)malloc(pokerCount);
    for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
    {
        uint8_t pointi = i->first;
        uint8_t lastPoint = pointi;
        kPokerUint_8 pokersi = i->second;
        if (pokersi.lenth < 3 || pointi != pokerPoint || pointi >= R1)
        {
            continue;
        }
        
        int index = 3;
        CCLOG("cacacacaca and pointi==%d",pointi);
        sanshuni[0] = pokersi.data[0];//先将i中放入三张
        sanshuni[1] = pokersi.data[1];
        sanshuni[2] = pokersi.data[2];
        for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.begin(); j != __pokerPool.end(); j++)
        {
            uint8_t pointj = j->first;
            if (pointj > R1)
            {
                break;
            }
            kPokerUint_8 pokersj = j->second;
            if (pokersj.lenth < 3)
            {
                continue;
            }
            if (lastPoint == pointj - 1)
            {
                CCLOG("the index==%d",index);
                sanshuni[index] = pokersj.data[0];//符合等差数列条件，则将j依次放入
                sanshuni[index+1] = pokersj.data[1];
                sanshuni[index+2] = pokersj.data[2];
                lastPoint = pointj;//将当前的数字缓存，便于下一次判断
                index += 3;
                CCLOG("the getSanShunByPart pointj==%d index ==%d",pointj,index);
            }

            if (index == pokerCount)
            {
                tipPokers = PokerUint8_tMake(sanshuni, index, true);
                return tipPokers;
            }
            
        }
        
		break; //只找对应顺子
    }
    free(sanshuni);

    if (tipPokers.lenth == 0 && __laiziCount > 0)
    {
        for (std::map<uint8_t, kPokerUint_8>::iterator i = __pokerPool.begin(); i != __pokerPool.end(); i++)
        {
            uint8_t pointi = i->first;
            if (pointi != pokerPoint || pointi >= R2)
            {
                continue;
            }
            uint8_t lastPoint = pointi;
            kPokerUint_8 pokersi = i->second;
            uint8_t* sanshuni = (uint8_t*)malloc(pokerCount);
            kPokerUint_8 sanshunPokers = PokerUint8_tMake(sanshuni, 0, true);
            
            int index = 0;
            sanshunPokers = addPokersWithPlanCount(sanshunPokers, pokersi, 3);
            index = sanshunPokers.lenth;
            printPokerNum("sanshunPokers", sanshunPokers);
            
            int addLaiziCount = 0;
            for (std::map<uint8_t, kPokerUint_8>::iterator j = __pokerPool.begin(); j != __pokerPool.end(); j++)
            {
                uint8_t pointj = j->first;
                if (pointj <= pokerPoint || pointj >= R2)
                {
                    delePokerData(sanshunPokers);
                    break;
                }
                if (pointj <= pointi)
                {
                    continue;
                }
                
                if ((pointj - lastPoint - 1) * 3 <= __laiziCount && (pointj - lastPoint) * 3 <= pokerCount)
                {
                    if ((pointj-PokerUtil::getPokerNum(sanshunPokers.data[0]))*3<pokerCount)
                    {
                        kPokerUint_8 pokersj = j->second;
                        sanshunPokers = addPokersWithPlanCount(sanshunPokers, pokersj, 3);
                        index = sanshunPokers.lenth;
                        
                        lastPoint = pointj;
                        sanshunPokers = addPokersWithPlanCount(sanshunPokers, __laiziPokerData, pokerCount - index);
                        addLaiziCount = sanshunPokers.lenth - index;//将加上的癞子数保存，用于牌型不合法的时候恢复癞子数
                        index = sanshunPokers.lenth;
                        printPokerNum("add laizi sanshunPokers", sanshunPokers);
                    }
                    else
                    {
                        delePokerData(sanshunPokers);
                        break;
                    }
                    
                }
                
                else
                {
                    delePokerData(sanshunPokers);
                    break;
                }
                if (index == pokerCount)
                {
                    printPokerNum("right sanshunPokers", sanshunPokers);
                    tipPokers = sanshunPokers;
                    return tipPokers;
                }
            }
            __laiziPokerData.lenth = __laiziCount;//没有合法的牌型，则把减去的癞子加上去
            delePokerData(sanshunPokers);

			break; //只找对应顺子
        }
    }

    return tipPokers;
}

kPokerUint_8 PokerTips::getFeijiDanByPart(int pokerPoint, int pokerCount)
{
    uint8_t* fejiDan = (uint8_t*)malloc(pokerCount);
    
    kPokerUint_8 tipPokers = zero_pokerData;
    
    int groupCount = pokerCount / 4;
    int sanShunCount = groupCount * 3;
    kPokerUint_8 feijiDanPokers = PokerUint8_tMake(fejiDan, 0, true);
    kPokerUint_8 sanshunPokers = getSanShunByPart(pokerPoint, sanShunCount);
    printPokerNum("getFeijiDan sanshunPokers", sanshunPokers);
    signPokerStatue(sanshunPokers);
    if (sanshunPokers.lenth == sanShunCount)
    {
        feijiDanPokers = addPokersWithPlanCount(feijiDanPokers, sanshunPokers, sanShunCount);
    }
    
    for (int i = 0; i < groupCount; i++)
    {
        kPokerUint_8 danTip = getOnePoker(0);
        feijiDanPokers = addPokersWithPlanCount(feijiDanPokers, danTip, 1);
    }
    
    if (feijiDanPokers.lenth == pokerCount)
    {
        tipPokers = feijiDanPokers;
    }
    return tipPokers;
}

kPokerUint_8 PokerTips::getFeijiShuangByPart(int pokerPoint, int pokerCount)
{
    uint8_t* fejiDan = (uint8_t*)malloc(pokerCount);
    
    kPokerUint_8 tipPokers = zero_pokerData;
    
    int groupCount = pokerCount / 4;
    int sanShunCount = groupCount * 3;
    kPokerUint_8 feijiDanPokers = PokerUint8_tMake(fejiDan, 0, true);
    kPokerUint_8 sanshunPokers = getSanShunByPart(pokerPoint, sanShunCount);
   // printPokerNum("sanshunPokers", sanshunPokers);
    signPokerStatue(sanshunPokers);
    if (sanshunPokers.lenth == sanShunCount)
    {
        feijiDanPokers = addPokersWithPlanCount(feijiDanPokers, sanshunPokers, sanShunCount);
    }
    
    for (int i = 0; i < groupCount; i++)
    {
        kPokerUint_8 twoTip = getTwoPokers(0);
        feijiDanPokers = addPokersWithPlanCount(feijiDanPokers, twoTip, 2);
    }
    
    if (feijiDanPokers.lenth == pokerCount)
    {
        tipPokers = feijiDanPokers;
    }
    return tipPokers;
}

kPokerUint_8 PokerTips::getSanDaiByPart(int pokerPoint, int pokerCount)
{
    uint8_t* sandai = (uint8_t*)malloc(pokerCount);
    
    kPokerUint_8 tipPokers = zero_pokerData;
    
    kPokerUint_8 sandaiPokers = PokerUint8_tMake(sandai, 0, true);
    kPokerUint_8 threePokers = getThreePokersByPart(pokerPoint);
    sandaiPokers = addPokersWithPlanCount(sandaiPokers, threePokers, 3);
    signPokerStatue(sandaiPokers);
    printPokerNum("getSanDaiByPart", sandaiPokers);
    
    if (pokerCount == 4)
    {
    	//带单张
        if (sandaiPokers.lenth == 3)
        {
            kPokerUint_8 daiTip = getOnePoker(0);
            sandaiPokers = addPokersWithPlanCount(sandaiPokers, daiTip, 1);
        }
    }
    else if (pokerCount == 5)//带一对
    {
        if (sandaiPokers.lenth == 3)
        {

            kPokerUint_8 daiTip = getTwoPokers(0);
            printPokerNum("daiTip", daiTip);
            sandaiPokers = addPokersWithPlanCount(sandaiPokers, daiTip, 2);

        }
    }
    
    if (sandaiPokers.lenth == pokerCount)
    {
        tipPokers = sandaiPokers;
        printPokerNum("sandaiTippokers", tipPokers);
    }
    return tipPokers;
}



std::vector<cocos2d::Vector<PokerObject*>> PokerTips::getAllPokersCanPlay(cocos2d::Vector<PokerObject*> lastPlayerPokers, cocos2d::Vector<PokerObject*> selectPokers)
{
    PokerTips* tip = new PokerTips();
//    kPokerUint_8 lastPokers = PokerUtil::getPokersFromArr(lastPlayerPokers);
    kPokerUint_8 seletPokers = PokerUtil::getPokersFromArr(selectPokers);
//    std::map<int, kPokerUint_8> allCanPlay = tip->getAllPokersCanPlay(lastPokers, seletPokers);

    //add by chenzc
	//以下代码的逻辑全都来自PokerTips::getAllPokersCanPlay(kPokerUint_8 &lastPlayerPokers, kPokerUint_8 &selectPokers)方法，
	//只是一些癞子的检测换成了使用lastPlayerPokers里面的PokerObject数据进行检测，避免出现癞子变色出现损耗，导致牌型检测不准确的问题
    kWeightValue lastWeight = tip->__check->getPokerType(lastPlayerPokers);
	std::map<int, kPokerUint_8> biggerThanLast;
	std::map<int, kPokerUint_8> allCanPlay = tip->__check->PokerTypeCheck::getAllCanPlay(seletPokers);
	for (std::map<int, kPokerUint_8>::iterator i = allCanPlay.begin(); i != allCanPlay.end(); i++)
	{
		int key = i->first;
		kPokerUint_8 pokerData = i->second;
		kWeightValue selectWeight = tip->__check->getPokerType(pokerData);
		if (PokerUtil::compareWeightValueIsBig(lastWeight, selectWeight))
		{
			biggerThanLast.insert(std::pair<int, kPokerUint_8>(key, pokerData));
		}
		else
		{
			delePokerData(pokerData);
		}
	}

    std::vector<Vector<PokerObject*>> allCanPlayArr;
    if (allCanPlay.size() > 0)
    {
        for (std::map<int, kPokerUint_8>::iterator i = biggerThanLast.begin(); i != biggerThanLast.end(); i++)
        {
            kPokerUint_8 pokerData = i->second;
            Vector<PokerObject*> pokerArr = PokerUtil::getPokerArrFromPokers(pokerData);
            allCanPlayArr.push_back(pokerArr);
            delePokerData(pokerData);
        }
    }
    delete tip;
    return allCanPlayArr;
}

std::map<int, kPokerUint_8> PokerTips::getAllPokersCanPlay(kPokerUint_8 &lastPlayerPokers, kPokerUint_8 &selectPokers)
{
    
    kWeightValue lastWeight = __check->getPokerType(lastPlayerPokers);
    std::map<int, kPokerUint_8> biggerThanLast;
    std::map<int, kPokerUint_8> allCanPlay = __check->PokerTypeCheck::getAllCanPlay(selectPokers);
    for (std::map<int, kPokerUint_8>::iterator i = allCanPlay.begin(); i != allCanPlay.end(); i++)
    {
        int key = i->first;
        kPokerUint_8 pokerData = i->second;
        kWeightValue selectWeight = __check->getPokerType(pokerData);

        if (PokerUtil::compareWeightValueIsBig(lastWeight, selectWeight))
        {
            biggerThanLast.insert(std::pair<int, kPokerUint_8>(key, pokerData));
        }
        else
        {
            delePokerData(pokerData);
        }
    }
    
    return biggerThanLast;
}


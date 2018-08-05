/*
 * MyDate.h
 *	功能描述：
 *  Created on: 2014年7月23日 上午10:13:57
 *      Author: hp
 */

#ifndef MYDATE_H_
#define MYDATE_H_

//#include "../../WWPlatform/ww_cc_platform.h"

#include <vector>

/*
 *时间类
 */
class MyDate
{
public:
	MyDate();
	virtual ~MyDate();
	int operator-(const MyDate&);
	MyDate(int y, int m, int d);

private:
	int year;         //年份
	int month;          //月份
	int day;              //几号
	int days;               //该月共有几天
	int lDays;                 //当前日期后面的天数，lDays=days-day;
	std::vector<int> myYear;          //存放万年历日期排列的容器
	void Set();          //输入了日期后初始化days lDays myYear
	void clear();
	void Reset(int y, int m, int d = 0);
	bool isR(int n);
};

#endif /* MYDATE_H_ */

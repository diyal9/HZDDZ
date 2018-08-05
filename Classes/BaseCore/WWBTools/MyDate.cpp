/*
 * MyDate.cpp
 *	功能描述：
 *  Created on: 2014年7月23日 上午10:13:57
 *      Author: hp
 */

#include "MyDate.h"

MyDate::MyDate() :
		year(0), month(0), day(0), days(0), lDays(0)
{

}
MyDate::MyDate(int y, int m, int d) :
		year(y), month(m), day(d)
{
	myYear.resize(40);
	Set();
}

MyDate::~MyDate()
{
}

bool MyDate::isR(int n)
{
	if (n % 100 != 0 && n % 4 == 0)
		return 1;
	else if (n % 100 == 0 && n % 400 == 0)
		return 1;
	else
		return 0;
}

void MyDate::Set()
{
	short beginDay = 1;
	for (int i = 1901; i <= year; i++)     //年份产生的初始位置的改变
	{
		if (!isR(i - 1))
			beginDay++;
		else
			beginDay += 2;
		if (beginDay > 6)
			beginDay %= 7;
	}
	for (int i = 2; i <= month; i++)        //月份产生的初始位置的改变
	{
		switch (i - 1)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
		{
			beginDay += 3;
			if (beginDay > 6)
				beginDay %= 7;
			break;
		}
		case 2:
		{
			if (isR(year))
				beginDay++;
			if (beginDay > 6)
				beginDay %= 7;
			break;
		}
		default:
		{
			beginDay += 2;
			if (beginDay > 6)
				beginDay %= 7;
			break;
		}
		}
	}
	for (int i = 0; i < 31; i++)       //将容器全部设置为31天
		myYear[beginDay + i] = i + 1;
	switch (month)
	//重设尾部几天
	{
	case 4:
	case 6:
	case 9:
	case 11:
	{
		myYear[beginDay + 30] = 0;
		days = 30;
		break;
	}
	case 2:
	{
		myYear[beginDay + 30] = 0;
		myYear[beginDay + 29] = 0;
		days = 29;
		if (!isR(year))
		{
			myYear[beginDay + 28] = 0;
			days = 28;
		}
		break;
	}
	default:
	{
		days = 31;
		break;
	}
	}
	lDays = days - day;
}

void MyDate::Reset(int y, int m, int d)
{
	this->clear();
	year = y;
	month = m;
	day = d;
	Set();
}
void MyDate::clear()
{
	myYear.clear();
	myYear.resize(40);
}
int MyDate::operator-(const MyDate& da)
{
	int sum = 0;
	MyDate temp;
	if (this->year < da.year)    //几个无法计算的提示
	{
//		cout << "必须以较早的日期减去较迟的日期！\n";
		return -1;
	}
	else if (this->year == da.year && this->month < da.month)
	{
//		cout << "必须以较早的日期减去较迟的日期！\n";
		return -2;
	}
	else if (this->year == da.year && this->month == da.month
			&& this->day < da.day)
	{
//		cout << "必须以较早的日期减去较迟的日期！\n";
		return -3;
	}

	else
	{
		if (year == da.year && month == da.month && day != da.day)    //同年同月不同日
		{
			return day - da.day;
		}
		if (year == da.year && month > da.month)    //同年不同月
		{
			for (int i = da.month + 1; i < month; i++)
			{
				temp.Reset(year, i);
				sum += temp.lDays;
			}
			return sum + da.lDays + day;
		}
		else
		{
			for (int i = da.year + 1; i < year; i++)
			{
				if (isR(i))
					sum += 366;
				else
					sum += 365;
			}
			for (int i = da.month + 1; i < 13; i++)
			{
				temp.Reset(da.year, i);
				sum += temp.lDays;
			}
			sum += da.lDays;
			for (int i = 1; i < month; i++)
			{
				temp.Reset(year, i);
				sum += temp.lDays;
			}
			sum += day;
			return sum;
		}
	}
}

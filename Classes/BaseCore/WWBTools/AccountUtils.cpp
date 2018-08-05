/*
 * AccountUtils.cpp
 *
 *  Created on: 2015-6-17
 *      Author: D.K.
 */

#include "AccountUtils.h"
#include "BaseCore/WWBTools/StringTool.h"

USING_NS_CC;

const char * KR_ACCOUNT_KEY = "kr_account_key";

static AccountUtils* g_pAccountUtils = NULL;

AccountUtils::AccountUtils(void)
{
}

AccountUtils::~AccountUtils(void)
{
    log("CALL AccountUtils::~AccountUtils()");
}

AccountUtils* AccountUtils::getInstance()
{
	if(!g_pAccountUtils)
	{
		g_pAccountUtils = new AccountUtils();
	}
	return g_pAccountUtils;
}

bool AccountUtils::init()
{
	bool bRet = false;
	do
	{
		std::string usernamelist = UserDefault::getInstance()->getStringForKey(KR_ACCOUNT_KEY, "");
		if (usernamelist != "")
		{
			for (auto tmp: StringTool::splitByArr(usernamelist, ","))
			{
				m_pAccounts.push_back(tmp);
			}
		}
		bRet = true;
	} while (0);
	return bRet;
}

void AccountUtils::saveAllAccountAry()
{
	std::string  saveStr ;

	int index = 0;
	for (auto str : m_pAccounts)
	{
		if (index == 0)
		{
			saveStr += str ;
		}
		else
		{
			saveStr += ("," + str);
		}
		++index;
	}

	UserDefault::getInstance()->setStringForKey(KR_ACCOUNT_KEY, saveStr.c_str());
}

/**获取所有的用户ID*/
std::vector<std::string> AccountUtils::getAllAccountAry()
{
	return m_pAccounts;
}

/**保存用户名及密码*/
void AccountUtils::saveAccount(const char* userId, const char* pwd)
{
	//保存帐号密码到文件
	UserDefault::getInstance()->setStringForKey(userId, pwd);

	std::vector<int> Accounts;
    
	//如果该账号已有，则不用保存
	for (auto str : m_pAccounts)
	{
		if (strcmp(str.c_str(), userId) == 0)
		{
			return;
		}
	}
	m_pAccounts.insert(m_pAccounts.begin(), userId);
	log("AccountUtils::saveAccount---------%d",m_pAccounts.size());
    
    //最多保留5个蛙号
    if (m_pAccounts.size() == 6)
    {
        m_pAccounts.erase(m_pAccounts.begin() + 5);
    }
	saveAllAccountAry();
}

/**根据用户ID取密码*/
std::string AccountUtils::getPwdByUserId(const char* userId)
{
	if (userId == nullptr)
	{
		return "";
	}
	return UserDefault::getInstance()->getStringForKey(userId, "");
}

void AccountUtils::moveIndexToFirst(int index)
{
	std::string tmp = m_pAccounts.at(index);
	m_pAccounts.erase(m_pAccounts.begin() + index);
	m_pAccounts.insert(m_pAccounts.begin(), tmp);
}

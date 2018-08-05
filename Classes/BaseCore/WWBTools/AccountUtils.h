/*
 * AccountUtils.cpp
 *
 *  Created on: 2015-6-17
 *      Author: D.K.
 */

#ifndef ACCOUNTUTILS_H_
#define ACCOUNTUTILS_H_


#include "cocos2d.h"

class AccountUtils : public cocos2d::Ref
{
public:
	AccountUtils(void);
	virtual ~AccountUtils(void);

	static AccountUtils* getInstance();

	virtual bool init();

	/**保存用户名及密码*/
	void saveAccount(const char* userId, const char* pwd);

	/**获取所有的用户ID*/
	std::vector<std::string> getAllAccountAry();

	/**根据用户ID取密码*/
	std::string getPwdByUserId(const char* userId);

	/**修改密码*/
	void saveAllAccountAry();

	//将选中项移到最前
	void moveIndexToFirst(int index);

private:

	std::vector<std::string> m_pAccounts;
};

#endif /* ACCOUNTUTILS_H_ */

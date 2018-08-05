#include "UserData.h"
#include "CustomEventConstant.h"
#include "Model/WWDataModel/WWGD/GD.h"
//#include "WWPlatform/tools/ww_tool.h"
#include "Model/WWDataModel/WWLeaderboardData/LeaderboardData.h"
#include "Control/WWXMLUnicodeManager/UnicodeManager.h"
#include "BaseCore/WWBTools/PhoneTool.h"
#include "BaseCore/WWBTools/StringTool.h"
//#include "TaskData.h"

USING_NS_CC;

static UserData* g_pUserData = NULL;
UserData* UserData::getInstance()
{
	if(!g_pUserData)
	{
		g_pUserData = new UserData();
	}
	return g_pUserData;
}

void UserData::setBoolValue( const char* key,bool value )
{
	UserDefault::getInstance()->setBoolForKey(key, value);
}

void UserData::setIntValue( const char* key, int value )
{
    if (strcmp(key, INT_USER_BEAN) == 0)
    {
        //add on 20150120 xuhuanqi赠送蛙豆：
        //printf("UserData::setIntValue:INT_USER_BEAN\n");
        UserData::getInstance()->setBean(value);
        
        if (!m_isNewPlayerGift && !this->getIsFirstHere())
        {
            //printf("setIntValue:!m_isNewPlayerGift:%d\n", m_isNewPlayerGift);
            EventCustom event(EVENT_REFRESH_MONEY);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }
    else if (strcmp(key, INT_USER_GENDER) == 0)
    {
        UserData::getInstance()->setGender(value);
    }
    else if (strcmp(key, INT_USER_VIP) == 0)
    {
        UserData::getInstance()->setVip(value);
    }
    
    if (strcmp(key, INT_USER_ID) == 0)
    {
        CCLOG("setIntValue, INT_USER_ID:%s=%d", key, value);
    }
    
	UserDefault::getInstance()->setIntegerForKey(key, value);
    UserDefault::getInstance()->flush();
}

void UserData::setDoubleValue( const char* key,double value )
{
	UserDefault::getInstance()->setDoubleForKey(key, value);
}

void UserData::setFloatValue( const char* key,float value )
{
	UserDefault::getInstance()->setFloatForKey(key, value);
}

//void UserData::setCStringValue( const char* key, cocos2d::CCString value )
//{
//	CCUserDefault::getInstance()->setStringForKey(key,value.getCString());
//	CCUserDefault::getInstance()->flush();
//}

void UserData::setStringValue( const char* key, std::string value )
{
	UserDefault::getInstance()->setStringForKey(key, value);
}

bool UserData::getBoolValue( const char* key )
{
	return UserDefault::getInstance()->getBoolForKey(key, false);
}

int UserData::getIntValue( const char* key )
{
	return UserDefault::getInstance()->getIntegerForKey(key, 0);
}

double UserData::getDoubleValue( const char* key )
{
	return UserDefault::getInstance()->getDoubleForKey(key, 0);
}

float UserData::getFloatValue( const char* key )
{
	return UserDefault::getInstance()->getFloatForKey(key, 0.0f);
}

//cocos2d::CCString UserData::getCString( const char* key )
//{
//	return CCUserDefault::getInstance()->getStringForKey(key,"");
//}

std::string UserData::getString( const char* key )
{
	return UserDefault::getInstance()->getStringForKey(key, "");
}

/****************add on 2015********************/
UserData::UserData() :
        m_version(0),
        //m_basicInfo(nullptr),
        m_isInitialized(false),
        m_isFirstHere(true),
        m_isNewPlayerGift(false),
        m_isShowSignLayer(false),
        m_isRedCardAward(false),
        m_exitType(0),
        m_firstCharge(0),
        m_gameID(0),
        m_vip(0),
        m_gender(0),
        m_userID(0),
        m_bean(0),
        m_crystal(0),
        m_signDay(0),
        m_signDateOfSecond(0),
        m_brokenDelay(0),
        m_brokenSecond(0),
        m_milsecondOfServerTime(0),
        m_orderingOfEarnBean(0),
        m_orderingOfCharge(0),
        m_orderingOfRicher(0),
        m_numOfLeaderboard(0),
        m_numOfTaskCompleted(0),
        m_countOfGameAwarded(0),
        m_selectPlayType(0),
        m_strNickname(""),
        m_strPassword(""),
        m_strRegionUnicode(""),
        m_strRegionValue(""),
        m_strDefautlHeadIcon(""),
        m_strSignDate(""),
        m_happyGameInfo(nullptr),
        m_laiziGameInfo(nullptr),
        m_colorCardList(nullptr),
        m_strColorCardStatus(""),
        m_strColorCardExp(""),
        m_strBlueCardParam2(""),
        m_strBlueCardParam3("")

{
    for (int i = 0; i < 7; ++i)
    {
        m_taskStatus[i] = 0;
    }
    
    for (int i = 0; i < eNum_message; ++i)
    {
        m_aMessage[i] = -1;
    }
}

UserData::~UserData()
{
    //CC_SAFE_RELEASE_NULL(m_basicInfo);
    CC_SAFE_RELEASE_NULL(m_happyGameInfo);
    CC_SAFE_RELEASE_NULL(m_laiziGameInfo);
    CC_SAFE_RELEASE_NULL(m_colorCardList);
}

//
void UserData::setGender(int gender)
{
    m_gender = gender;
}

int UserData::getGender()
{
    return m_gender;
}

void UserData::setUserID(int userID)
{
    m_userID = userID;
}

int UserData::getUserID()
{
    return m_userID;
}

void UserData::setBean(int bean)
{
    m_bean = bean;
}

int UserData::getBean()
{
    return m_bean;
}

void UserData::setCrystal(int crystal)
{
    m_crystal = crystal;
}

int UserData::getCrystal()
{
    return m_crystal;
}

void UserData::setNickname(const std::string &nickname)
{
    m_strNickname = nickname;
}

std::string UserData::getNickname()
{
    return m_strNickname;
}

void UserData::setPassword(const std::string &password)
{
    m_strPassword = password;
}

std::string UserData::getPassword()
{
    return m_strPassword;
}

//查询签到天数
int UserData::getCurrentSignDay()
{
    if (m_userID != 0)
    {
//        std::string keySecond = StringUtils::format("%s%d", key_sign_data_time, m_userID);
//        std::string keyDay = StringUtils::format("%s%d", key_sign_data_days, m_userID);
//        m_signDateOfSecond = UserDefault::getInstance()->getIntegerForKey(keySecond.c_str(), 0);
//        m_signDay = UserDefault::getInstance()->getIntegerForKey(keyDay.c_str(), 0);
//        if (m_signDay < 0)
//        {
//            m_signDay = 0;
//        }
//        
//        struct timeval now;
//        gettimeofday(&now, NULL);
//        
//        time_t nowSecond = now.tv_sec;
//        struct tm * nowDate;
//        nowDate = localtime(&nowSecond);
//        int nowDay = nowDate->tm_mday;
//        
//        if (m_strSignDate != "")
//        {
//            if (m_strSignDate != PhoneTool::getFormatDayTime())
//            {
//                return m_signDay + 1;
//            }
//        }
//        else  if (m_signDateOfSecond == 0)
//        {
//            m_signDateOfSecond = nowSecond;
//            return m_signDay + 1;
//        }
//        else
//        {
//            time_t signSecond = m_signDateOfSecond;
//            struct tm * signDate = localtime(&signSecond);
//            int signDay = signDate->tm_mday;
//            
//            if (signDay != nowDay)
//            {
////                if (nowSecond - signSecond < 24 * 60 * 60)
////                {
////                    m_signDateOfSecond = nowSecond;
////                    return m_signDay + 1;
////                } 
//            }
//        }
//
//    }

        if (m_signDay <= 0)
        {
            return 1;
        }
        else
        {
            std::string signDate = UserData::getInstance()->getSignDate();
            std::string today = this->getServerTimeFormatDaty();
            if (strlen(signDate.c_str()) <= 0)
            {
                return m_signDay + 1;
            }
            else if (strcmp(signDate.c_str(), today.c_str()) != 0)
            {
                return m_signDay + 1;
            }
        }
        
    }
    
    return 0;
}

/*
 *  查询签到奖励值
 */
int UserData::getSignBean(int dayIndex)
{
    
    return 0;
}

//连续签到
void UserData::setSignDay(int signDay)
{
    if (signDay < 0)
    {
        signDay = 0;
    }
    
    m_signDay = signDay;

//    std::string keyDay = StringUtils::format("%s%d", key_sign_data_days, m_userID);
//    UserDefault::getInstance()->setIntegerForKey(keyDay.c_str(), m_signDay);
}

int UserData::getSignDay()
{
    return m_signDay;
}

void UserData::setSignDate(const std::string &strSignDate)
{
    CCLOG("UserData::setSignDate:%s", strSignDate.c_str());
    
    m_strSignDate = strSignDate;
    
    std::string keyDate = StringUtils::format("%s%d", key_sign_data_date, m_userID);
    UserDefault::getInstance()->setStringForKey(keyDate.c_str(), m_strSignDate);
}

std::string UserData::getSignDate()
{
    if (m_strSignDate == "")
    {
        std::string keyDate = StringUtils::format("%s%d", key_sign_data_date, m_userID);
        m_strSignDate = UserDefault::getInstance()->getStringForKey(keyDate.c_str(), "");
    }
    
    CCLOG("UserData::getSignDate:%s", m_strSignDate.c_str());
    
    return m_strSignDate;
}

/*
 *
 */
void UserData::setVip(int vip)
{
    //注释掉VIP功能
    return;
    
    m_vip = vip;
    
    EventCustom event(EVENT_FRESH_VIP);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

/*
 * 0=非VIP会员， 1=VIP会员
 */
int UserData::getVip()
{
    return m_vip;
}

//游戏ID
void UserData::setGameID(int gameID)
{
    m_gameID = gameID;
}

/*
 *  获取游戏ID
 */
int UserData::getGameID()
{
    return m_gameID;
}

/*
 *  任务状态
 */
void UserData::setTaskStatus(int taskType, int status)
{
    m_taskStatus[taskType - 1] = status;
}

int UserData::getTaskStatus(int taskType)
{
    return m_taskStatus[taskType - 1];
}

void UserData::setIsNewplayerGift(bool value)
{
    m_isNewPlayerGift = value;
}

bool UserData::getIsNewplayerGift()
{
    return m_isNewPlayerGift;
}

void UserData::setOnlyTaskGot(const std::string strTaskID)
{
    UserDefault::getInstance()->setBoolForKey(strTaskID.c_str(), true);
}

bool UserData::getOnlyTaskStatus(const std::string strTaskID)
{
    return UserDefault::getInstance()->getBoolForKey(strTaskID.c_str(), false);
}

/*
 *  保存签到数据
 */
void UserData::saveSignData()
{
//    int second = m_signDateOfSecond;
//    std::string keySecond = StringUtils::format("%s%d", key_sign_data_time, m_userID);
//    std::string keyDay = StringUtils::format("%s%d", key_sign_data_days, m_userID);
//    
//    UserDefault::getInstance()->setIntegerForKey(keySecond.c_str(), second);
//    UserDefault::getInstance()->setIntegerForKey(keyDay.c_str(), m_signDay);
//    UserDefault::getInstance()->flush();
}

void UserData::setFirstCharge(int arge)
{
    std::string key = StringUtils::format("key_firstCharge_%d", m_userID);
    if (m_firstCharge == 0 && arge > 0)
    {
        EventCustom event(EVENT_FRESH_FIRST_CHARGE);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
    
    m_firstCharge = arge;
    UserDefault::getInstance()->setIntegerForKey(key.c_str(), m_firstCharge);
}

int UserData::getFirstCharge()
{
    if (m_firstCharge == 0)
    {
        std::string key = StringUtils::format("key_firstCharge_%d", m_userID);
        m_firstCharge = UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
    }
    return m_firstCharge;
}

int UserData::isExistNewNotice(cocos2d::Ref *objectData)
{
    int num = 0;
    
    if (auto data = dynamic_cast<NoticeList *>(objectData))
    {
        
        
        std::string strNoticeArrID = UserData::getInstance()->getString(STR_NOTICE_ID.c_str());
        
        std::vector<int> vMsgID = data->getMsgID();
        long count = vMsgID.size();
        
        if (count > 0)
        {
            for (int i = 0; i < count; ++i)
            {
                int msgID = vMsgID.at(i);
                std::string strMsgID = StringUtils::format("%d", msgID);
                
                if (strNoticeArrID.find(strMsgID) == std::string::npos)
                {
                    ++num;
                }
            }
        }
    }
    
    return num;
}


void UserData::setChangePassword(int userID)
{
    std::string key = StringUtils::format("%d_chpw", userID);
    UserDefault::getInstance()->setBoolForKey(key.c_str(), true);
}

bool UserData::getChangePassword(int userID)
{
    std::string key = StringUtils::format("%d_chpw", userID);
    return UserDefault::getInstance()->getBoolForKey(key.c_str(), false);
}

void UserData::setRegionUnicode(const std::string &strRegion)
{
    m_strRegionUnicode = strRegion;
}

void UserData::saveRegionUnicode()
{
    std::string key = StringUtils::format("%s_%d", key_region_unicode.c_str(), m_userID);
    UserDefault::getInstance()->setStringForKey(key.c_str(), m_strRegionUnicode);
    UserDefault::getInstance()->flush();
}

std::string UserData::getRegionUnicode()
{
    if (m_strRegionUnicode.length() <= 0)
    {
        std::string key = StringUtils::format("%s_%d", key_region_unicode.c_str(), m_userID);
        m_strRegionUnicode = UserDefault::getInstance()->getStringForKey(key.c_str(), "");
    }
    
    return m_strRegionUnicode;
}

void UserData::setRegionValue(const std::string &strValue)
{
//    printf("UserData::setRegionValue:strValue:%s\n", strValue.c_str());
    
    m_strRegionValue = strValue;
    
//    UserDefault::getInstance()->setStringForKey(key_region_value.c_str(), m_strRegionValue);
}

std::string UserData::getRegionValue()
{
    if (m_strRegionValue.length() <= 0)
    {
        std::string unicode = UserData::getInstance()->getRegionUnicode();
        std::string region = UnicodeManager::getUnicodeStringValue(unicode);
        m_strRegionValue = region;
    }
    
    return m_strRegionValue;
}

void UserData::setDefaultIcon(std::string strHeadIcon)
{
    if (strlen(strHeadIcon.c_str()) <= 0)
    {
        return;
    }
    
    m_strDefautlHeadIcon = strHeadIcon;
    std::string key = StringUtils::format("%s_%d", key_defaultIcon, m_userID);
    UserDefault::getInstance()->setStringForKey(key.c_str(), strHeadIcon);
    UserDefault::getInstance()->flush();
}

std::string UserData::getDefaultIcon()
{
    if (m_strDefautlHeadIcon == "")
    {
        std::string key = StringUtils::format("%s_%d", key_defaultIcon, m_userID);
        std::string strHeadIcon = UserDefault::getInstance()->getStringForKey(key.c_str());
        if (strlen(strHeadIcon.c_str()) <= 0)
        {
            strHeadIcon = "public/classic_1.png";
            if (m_gender == 2)
            {
                strHeadIcon = "public/classic_5.png";
            }
            
            UserDefault::getInstance()->setStringForKey(key_defaultIcon, strHeadIcon);
            UserDefault::getInstance()->flush();
        }
        
        m_strDefautlHeadIcon = strHeadIcon;
    }
    
    return m_strDefautlHeadIcon;
}

void UserData::setIsFirstHere(bool value)
{
    m_isFirstHere = value;
    std::string key = StringUtils::format("key_isFirstHere_%d", m_userID);
    UserDefault::getInstance()->setBoolForKey(key.c_str(), value);
    UserDefault::getInstance()->flush();
}

bool UserData::getIsFirstHere()
{
    if (m_isFirstHere)
    {
        std::string key = StringUtils::format("key_isFirstHere_%d", m_userID);
        m_isFirstHere = UserDefault::getInstance()->getBoolForKey(key.c_str(), true);
    }
    
    return m_isFirstHere;
}

void UserData::setBrokenDelay(int brokenDelay)
{
    m_brokenDelay = brokenDelay;
    
    struct timeval now;
    gettimeofday(&now, NULL);
    
    time_t nowSecond = now.tv_sec;
    m_brokenSecond = nowSecond;
    
}

int UserData::getBrokenDelay()
{
    long delay = 0;
    if (m_brokenDelay > 0 && m_brokenSecond > 0)
    {
        struct timeval now;
        gettimeofday(&now, NULL);
        
        time_t nowSecond = now.tv_sec;
        delay = m_brokenDelay - (nowSecond - m_brokenSecond);
        if (delay <= 0)
        {
            delay = 0;
        }
        
    }
    
    return delay;
}


bool UserData::getIsFirstLoginOfToday(int userID)
{
    std::string key = StringUtils::format("%s%d", key_firstLogin, userID);
    std::string strLastLogin = UserDefault::getInstance()->getStringForKey(key.c_str(), "");
    std::string strToday = PhoneTool::getFormatDayTime();
    if (strlen(strLastLogin.c_str()) <= 0)
    {
        UserDefault::getInstance()->setStringForKey(key.c_str(), strToday);
        UserDefault::getInstance()->flush();
        
        return true;
    }
    else
    {
        if (strToday.compare(strLastLogin) == 0)
        {
            return false;
        }
        else
        {
            UserDefault::getInstance()->setStringForKey(key.c_str(), strToday);
            UserDefault::getInstance()->flush();
            return true;
        }
    }
    
    return false;
}

void UserData::setOrderingOfLeaderboard(int type, int order, int userID)
{
    std::string key = "";
    if (type == LeaderboardData::eLeaderboardType_earn)
    {
        key = StringUtils::format("key_earnBeanLB_%d", userID);
        if ((order >= 1 && order <= 100) && (m_orderingOfEarnBean == 0 || m_orderingOfEarnBean > 100))
        {
            int myOrder = UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
            if (order != myOrder)
            {
                m_numOfLeaderboard = m_numOfLeaderboard + 1;
            }
            
        }
        
        m_orderingOfEarnBean = order;
        
    }
    else if (type == LeaderboardData::eLeaderboardType_charge)
    {
        key = StringUtils::format("key_chargeLB_%d", userID);
        if ((order >= 1 && order <= 100) && (m_orderingOfCharge == 0 || m_orderingOfCharge > 100))
        {
            int myOrder = UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
            if (order != myOrder)
            {
                m_numOfLeaderboard = m_numOfLeaderboard + 1;
            }
            
        }
        
        m_orderingOfCharge = order;
        
    }
    else if (type == LeaderboardData::eLeaderboardType_richer)
    {
        key = StringUtils::format("key_richerLB_%d", userID);
        if ((order >= 1 && order <= 100) && (m_orderingOfRicher == 0 || m_orderingOfRicher > 100))
        {
            int myOrder = UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
            if (order != myOrder)
            {
                m_numOfLeaderboard = m_numOfLeaderboard + 1;
            }
            
        }
        
        m_orderingOfRicher = order;
        
    }
    
    if (strlen(key.c_str()) > 0)
    {
        UserDefault::getInstance()->setIntegerForKey(key.c_str(), order);
    }
    
    this->setNumOfLeaderboard(m_numOfLeaderboard);
}

int UserData::getOrderingOfLeaderboard(int type, int userID)
{
    int order = 0;
    std::string key = "";
    if (type == LeaderboardData::eLeaderboardType_earn)
    {
        key = StringUtils::format("key_earnBeanLB_%d", userID);
    }
    else if (type == LeaderboardData::eLeaderboardType_charge)
    {
        key = StringUtils::format("key_chargeLB_%d", userID);
    }
    else if (type == LeaderboardData::eLeaderboardType_richer)
    {
        key = StringUtils::format("key_richerLB_%d", userID);
    }
    
    if (strlen(key.c_str()) > 0)
    {
        order = UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
    }
    
    return order;
}

void UserData::setNumOfLeaderboard(int num)
{
    m_numOfLeaderboard = num;
    
    EventCustom event(EVENT_FRESH_NUM_OF_LEADERBOARD);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

int UserData::getNumOfLeaderboard()
{
    return m_numOfLeaderboard;
}

void UserData::setNumOfTaskCompleted(int num)
{
    m_numOfTaskCompleted = num;
    
    EventCustom event(EVENT_FRESH_NUM_OF_TASK);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

int UserData::getNumOfTaskCompleted()
{
    return m_numOfTaskCompleted;
}

void UserData::setCountOfGameAwarded(int num)
{
    if (num > 0 && m_countOfGameAwarded != num)
    {
        m_countOfGameAwarded = num;
        std::string key = StringUtils::format("%s_%d", key_count_of_gameAwarded, m_userID);
        CCLOG("UserData::setCountOfGameAwarded:num=%d", num);
        UserDefault::getInstance()->setIntegerForKey(key.c_str(), m_countOfGameAwarded);
    }
}

int UserData::getCountOfGameAwarded()
{
    if (m_countOfGameAwarded == 0)
    {
        std::string key = StringUtils::format("%s_%d", key_count_of_gameAwarded, m_userID);
        m_countOfGameAwarded = UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
    }
    
    if (m_countOfGameAwarded == 0)
    {
        m_countOfGameAwarded = 20;
        if (m_vip > 0)
        {
            m_countOfGameAwarded = 10;
        }
    }
    
    return m_countOfGameAwarded;
}


void UserData::setExitType(char type)
{
    m_exitType = type;
}

char UserData::getExitType()
{
    return m_exitType;
}


/*
 *  功能：设置特权卡信息
 */
void UserData::setColorCardInfo(std::string strStatus, std::string strExp)
{
    m_strColorCardStatus = strStatus;
    m_strColorCardExp = strExp;
}

/*
 *  功能：查询特权卡状态
 */
std::string UserData::getColorCardStatus(eColorCardIndex index)
{
    std::string value = "";
    if (index >= 0 && strlen(m_strColorCardStatus.c_str()) > 0)
    {
        std::string text = m_strColorCardStatus;
        std::vector<std::string> vStatus = StringTool::splitByArr(text, ",");
        if (vStatus.size() > index)
        {
            value = vStatus.at(index);
            
            return value;
        }
    }
    
    return value;
}

/*
 *  功能：查询特权卡有效期
 */
std::string UserData::getColorCardExp(eColorCardIndex index)
{
    std::string value = "0";
    if (index >= 0 && strlen(m_strColorCardStatus.c_str()) > 0)
    {
        std::string text = m_strColorCardExp;
        std::vector<std::string> vStatus = StringTool::splitByArr(text, ",");
        if (vStatus.size() > index)
        {
            value = vStatus.at(index);
        }
    }
    
    return value;
}

std::string UserData::getColorCardIntroduce(int fid)
{
    if (m_colorCardList)
    {
        int index = -1;
        int count = m_colorCardList->getCount();
        std::vector<int> vFid = m_colorCardList->getfid();
        for (int i = 0; i < count; ++i)
        {
            int fidTemp = vFid.at(i);
            if (fid == fidTemp)
            {
                index = i;
                break;
            }
        }
        
        if (index >= 0)
        {
            std::vector<std::string> vIntr = m_colorCardList->getIntroduce();
            return vIntr.at(index);
        }
    }
    
    return "";
}

bool UserData::getIsRedCardAward()
{
    return m_isRedCardAward;
}

//根据本地数据计算红卡领取状态
bool UserData::caculateRedCardAward()
{
    std::string key = StringUtils::format("key_redCardAward_%d", m_userID);
    std::string day = UserDefault::getInstance()->getStringForKey(key.c_str(), "");
    std::string strToday = "";
    if (m_milsecondOfServerTime > 0)
    {
        strToday = getServerTimeFormatDaty();
    }
    else
    {
        strToday = PhoneTool::getFormatDayTime();
    }
    
    if (strcmp(day.c_str(), strToday.c_str()) == 0)
    {
        m_isRedCardAward = false;
        return m_isRedCardAward;
    }
    
    UserDefault::getInstance()->setStringForKey(key.c_str(), strToday);
    UserDefault::getInstance()->flush();
    
    m_isRedCardAward = true;
    return m_isRedCardAward;
}

int UserData::getFidWithMagicID(int magicID)
{
    if (m_colorCardList)
    {
        std::vector<int> vMagicID = m_colorCardList->getMagicID();
        std::vector<int> vFid = m_colorCardList->getfid();
        int count = m_colorCardList->getCount();
        for (int i = 0; i < count; ++i)
        {
            int temp = vMagicID.at(i);
            if (temp == magicID)
            {
                return vFid.at(i);
            }
        }
    }
    
    return 0;
}
/*
 *  蓝卡破产时间和蛙豆奖励
 */
void UserData::setBlueCardLeftTimeAndAwardBean(const std::string &text)
{
    m_strBlueCardParam2 = text;
}

/*
 * 蓝卡剩余次数和蛙豆
 */
void UserData::setBlueCardNumAndAwardBean(const std::string &text)
{
    m_strBlueCardParam3 = text;
}



/*
 * 蓝卡破产蛙豆
 * @param index=1, 返回接下来破产蛙豆； index=2, 返回接下来第二此破产蛙豆；主要用于区分首次蛙豆数值
 */
std::string UserData::getBlueCardNextAwardBean(int index)
{
    if (m_strBlueCardParam3 != "")
    {
        std::string text = m_strBlueCardParam3;
        std::vector<std::string> vNum = StringTool::splitByArr(text, ",");
        int size = vNum.size();
        if (index > 0 && size > index)
        {
            return vNum.at(index);
        }
    }
    return "";
}


/*
 * 蓝卡剩余次数
 */
int UserData::getBlueCardLeftNum()
{
    std::string text = m_strBlueCardParam3;
    std::vector<std::string> vNum = StringTool::splitByArr(text, ",");
    int size = vNum.size();
    
    return size - 1;
}

/*
 * 蓝卡最大次数
 */
int UserData::getBlueCardMaxNum()
{
    std::string text = m_strBlueCardParam3;
    std::vector<std::string> vNum = StringTool::splitByArr(text, ",");
    int size = vNum.size();
    if (size > 0)
    {
        std::string num = vNum.at(0);
        int totalNum = atoi(num.c_str());
        return totalNum;
    }
    
    return 0;
}

void UserData::setIsShowSignLayer(bool isShow)
{
    m_isShowSignLayer = isShow;
}

bool UserData::getIsShowSignLayer()
{
    return m_isShowSignLayer;
}

void UserData::setMilsecondOfServerTime(long milsecond)
{
    m_milsecondOfServerTime = milsecond;
}

std::string UserData::getServerTimeFormatDaty()
{
    if (m_milsecondOfServerTime > 0)
    {
        time_t timep = m_milsecondOfServerTime;
        
        struct tm *tm;
        tm = localtime(&timep);
        int year = tm->tm_year + 1900;
        int month = tm->tm_mon + 1;
        int day = tm->tm_mday;
        
        std::string time = StringUtils::format("%d", year);
        if (month < 10)
        {
            time += StringUtils::format("-0%d", month);
        }
        else
        {
            time += StringUtils::format("-%d", month);
        }
        
        if (day < 10)
        {
            time += StringUtils::format("-0%d", day);
        }
        else
        {
            time += StringUtils::format("-%d", day);
        }
        return time;
    }

    return PhoneTool::getFormatDayTime();
}

/*
 *  为排行榜没有加载到头像的用户获取一个随机经典头像
 */
char UserData::getClassicIconIndex(int userID)
{
    int index = -1;
    for (int temp : m_vUserID)
    {
        ++index;
        if (temp == userID)
        {
            if (index < m_vClassicIconIndex.size())
            {
                return m_vClassicIconIndex.at(index);
            }
        }
    }
    
    int r = rand() % 8 + 1;
    m_vUserID.push_back(userID);
    m_vClassicIconIndex.push_back(r);
    
    return r;
}

/*
 *  收到的消息队列，用于界面显示
 */
void UserData::setMessageReceive(int index, int value)
{
    if (m_aMessage[index] == -1)
    {
        m_aMessage[index] = value;
        
        for (int i = 0; i < eNum_message; ++i)
        {
            if (m_aMessage[i] == -1)
            {
                return;
            }
        }
        
        EventCustom event(EVETN_SHOW_POP_LAYER);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

bool UserData::getIsAllMessageReceived()
{
    for (int i = 0; i < eNum_message; ++i)
    {
        if (m_aMessage[i] == -1)
        {
            return false;
        }
    }
    return true;
}

//缓存玩法选择
void UserData::setSeletedPlayType(int type)
{
    m_selectPlayType = type;
}

int UserData::getSeletedPlayType()
{
    return m_selectPlayType;
}

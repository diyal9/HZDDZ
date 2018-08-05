#ifndef _UserData_H__
#define _UserData_H__

#include "cocos2d.h"
#include "WWMacros.h"
#include "Model/WWDataModel/WWGD/GD.h"
#include "Model/WWDataModel/WWEquipData/EquipData.h"

//---------------------char*--------------------------
/**账号*/
static const char* INT_USER_ID = "0x000001";
static const char* INT_USER_TYPE = "0x000003";
/**年龄*/
static const char* INT_USER_AGE = "0x000004";
/**蛙币*/
static const char* INT_USER_CASH= "0x000006";
/**vip*/
static const char* INT_USER_VIP = "0x000007";
/**性别*/
static const char* INT_USER_GENDER = "0x000008";
/**会员身份";（0不是，1是）*/
static const char* INT_MEMBER_IDENTITY = "0x000009";
/**会员剩余天数"; (会员身份剩余天数|会员过期天数)*/
static const char* INT_MEMBER_EXP = "0x000010";
/**省*/
static const char* INT_PROVINCE = "0x000011";
/**市*/
static const char* INT_CITY = "0x000012";
/**用户在线状态*/
static const char* INT_ONLINE_TYPE = "0x000013";
/**游戏积分等级*/
static const char* INT_USER_GRADE = "0x000014";
/**战斗力*/
static const char* INT_USER_POWER = "0x000015";
/**经验值*/
static const char* INT_USER_LONGEVITY = "0x000016";
/**幸运值*/
static const char* INT_USER_LUCKY = "0x000017";
/**配偶账号*/
static const char* INT_LOVER_ID = "0x000018";
/**帮会账号*/
static const char* INT_TONG_ID = "0x000019";
/**战队账号*/
static const char* INT_TEAM_ID = "0x00001A";
/**用户积分*/
static const char* INT_USER_POINT = "0x000020";
/**玩过的游戏数量*/
static const char* INT_USER_PLAYCOUNTGAME = "0x000021";
/**胜局数*/
static const char* INT_USER_WIN = "0x000022";
/**对局数*/
static const char* INT_USER_PLAY = "0x000023";
/**掉线局数*/
static const char* INT_USER_BROKEN = "0x000024";
/**用户所在模块ID*/
static const char* INT_USER_MODULE_ID = "0x000025";
/**聊聊控：(0不是，1是)*/
static const char* INT_TALK_IDENTITY = "0x000026";
/**交友控 ：(0不是，1是)*/
static const char* INT_BUDDY_IDENTITY = "0x000027";
/**配偶头像ID*/
static const char* INT_LOVER_HEAD_ID = "0x000028";
/**当前游戏ID*/
static const char* INT_GAME_ID = "0x000029";
/**当前游戏玩法*/
static const char* INT_PLAY_TYPE = "0x000030";
/**玩家头像ID*/
static const char* INT_USER_HEAD_ID = "0x000031";
/**蛙豆*/
static const char* INT_USER_BEAN = "0x000032";
/**用户成就数*/
static const char* INT_USER_ACHIEVE_NUM = "0x000033";
/**全部成就数*/
static const char* INT_ALL_ACHIEVE_COUNT = "0x000034";
/**1-国家，2-省份，3-地区，4-区/县*/
static const char* INT_LOCATION_GRADE = "0x000035";
/**省市以下的填写上级地区的Region，否则填写 0*/
static const char* INT_LOCATION_PARENT_ID = "0x000036";
/**返回记录数量*/
static const char* INT_LOCATION_COUNT = "0x000037";
/**刷新GuaGua列表时间:0 不刷新";>0 刷新频率(秒)*/
static const char* INT_REFRESH_GUAGUA = "0x000040";
/**版本状态 0 没有新版本可更新；1 有新版本，非必需更新；2 有新版本，必需更新*/
static const char* INT_VERSION_STATUS = "0x000041";
/**已经绑定手机(0不是，1是)*/
static const char* INT_BANDED_PHONE = "0x000042";
/**领豆标识>0: 可以领取并表示可以领取的数量, -101=因蛙豆多于限额不能领取；-102＝今天已经领取过不能重复领取*/
static const char* INT_BEAN_AWARD_COUNT = "0x000043";
/**连续签到天数 */
static const char* INT_LOGON_DAY_COUNT = "0x000044";
/**金子*/
static const char* INT_USER_GAME_CASH= "0x000045";
/**新手任务修改密码奖励的金子*/
static const char* INT_GAME_TASK_MODIFY_PWD = "0x000046";
/**新手任务修改昵称奖励的金子*/
static const char* INT_GAME_TASK_MODIFY_NICKNAME = "0x000047";
/**新手任务修改头像奖励的金子*/
static const char* INT_GAME_TASK_MODIFY_AVATAR = "0x000048";
/**新手任务绑定手机奖励的金子*/
static const char* INT_GAME_TASK_BIND_PHONE = "0x000049";
/**新手任务新手教程奖励的金子*/
static const char* INT_GAME_TASK_NEW_PLAYER = "0x000050";
/**罗盘开关 0-关 1-开*/
static const char* INT_COMPASS_SWITCH = "0x000051";
/**兑换中心开关 0-关 1-开*/
static const char* INT_EXCHANGE_SWITCH = "0x000052";
/**登录返回信息，是否第一次登录，1为第一次登录，0为第二次登录*/
static const char* INT_FIRST_LOGON = "0x000053";
/**今天是否为第一次签到（用于手动签到判断）     0为登录后已知当天签过到    1为未签到，自动弹框     2为未签到时，登录后，签到成功*/
static const char* INT_TODAY_FIRST_SIGN = "0x000054";
/**用户宝盒是否处于打开状态*/
static const char* INT_WHETHER_BOX_OPEN = "0x000055";
/**用户是否需要输入旧密码    1为需要输入旧密码     0为不需要输入旧密码*/
static const char* INT_INPUT_OLD_PWD = "0x000056";

//---------------------string--------------------------
/**昵称*/
static const char* STR_NICKNAME = "a";
/**密码*/
static const char* STR_USER_PASSWORD = "b";
/**绑定手机号*/
static const char* STR_USER_PHONE = "c";
/**银子*/
static const char* STR_USER_SILVER ="d";
/**用户注册时间*/
static const char* STR_REGISTER_TIME = "e";
/**第三方账号*/
static const char* STR_THIRD_PARTY_ID = "f";
/**生日*/
static const char* STR_USER_BIRTHDAY = "g";
/**生肖*/
static const char* STR_USER_BIRTHDAY_ANIMAL = "h";
/**血型*/
static const char* STR_USER_BLOOD_TYPE = "i";
/**上次登录时间*/
static const char* STR_USER_LAST_TIME = "j";
/**星座*/
static const char* STR_USER_CONSTELLATION = "k";
/**爱好*/
static const char* STR_USER_HOBBY = "l";
/**心情*/
static const char* STR_USER_MOOD = "m";	
/**自我介绍*/
static const char* STR_USER_INTRODUCE = "n";
/**网络运营商名称*/
static const char* STR_NET_NAME = "o";
/**真实姓名*/
static const char* STR_USER_TRUENAME = "p";
/**邮箱*/
static const char* STR_USER_MAIL = "q";
/**配偶姓名*/
static const char* STR_LOVER_NAME = "r";
/**帮会名称*/
static const char* STR_TONG_NAME = "s";
/**个性签名*/
static const char* STR_USER_SIGNATURE = "t";
/**荣誉值*/
static const char* STR_USER_HONOUR = "u";
/**积分等级称号*/
static const char* STR_USER_TITTLE = "v";
/**玩家头衔*/
static const char* STR_USER_HONOR = "w";
/**版本更新的下载地址*/
static const char* STR_DOWNLOAD_URL = "x";
/**版本更新的描述*/
static const char* STR_DOWNLOAD_DESC = "y";
/**金子**/
static const char* STR_USER_GOLD = "z";
/**胜率（string）*/
static const char* STR_WIN_PERCENT = "1";
/**逃跑率（string）*/
static const char* STR_ESCAPE_PERCENT = "2";
/**单把赢最多游戏币*/
static const char* STR_BEST_STAKE = "3";
/**新手礼包奖励的金子*/
static const char* STR_NEW_PLAYER_GIFT_CASH = "4";
/**昵称缓存，在保存成功后更新昵称STR_NICKNAME*/
static const char* STR_NICKNAME_CACHE = "5";
/**百宝箱奖励*/
static const char* STR_BOXGIFT = "6";
/** 更新的版本号 */
static const char* STR_DOWNLOAD_VERSION_CODE= "7";

//----------------------object-------------------------
/**用户头像*/
static const std::string OBJ_BMP_HEAD = "bmp_head";
/**int[] 游戏ID*/
static const std::string OBJ_GAME_ID = "game_id";
/**int[] 游戏玩法*/
static const std::string OBJ_PLAY_TYPE = "play_type";
/**int[] 游戏积分*/
static const std::string OBJ_POINT = "point";
/**int[] 游戏积分等级*/
static const std::string OBJ_GAME_GRADE = "game_grade";
/**String[] 游戏积分等级称号*/
static const std::string OBJ_TITLE = "title";
/**int[] 胜局数*/
static const std::string OBJ_WIN = "win";
/**int[] 对局数*/
static const std::string OBJ_PLAY = "play";
/**int[] 掉线局数*/
static const std::string OBJ_BROKEN = "broken";
/**int[] 地区ID*/
static const std::string OBJ_LOCATION_REGION_ID = "region_id";
/**String[] 地区名*/
static const std::string OBJ_LOCATION_REGION_NAME = "region_name";
/**int[] 成就ID*/
static const std::string OBJ_ACHIEVE_ID = "achieve_id";
/**int[] 1-获得成就，2-未获得成就*/
static const std::string OBJ_ACHIEVE_FLAG = "achieve_flag";
/**int[] 成就名称*/
static const std::string OBJ_ACHIEVE_NAME = "achieve_name";
/**int[] 成就描述*/
static const std::string OBJ_ACHIEVE_DESC = "achieve_desc";
/**int[] 所有各个成就的的完成值*/
static const std::string OBJ_ACHIEVE_FINISH_NUM = "achieve_finish_num";
/**香港为好友充值好友的蛙号**/
static const std::string OBJ_OTHER_USER_ID = "charge_other_user_id";
/**显示dialog接口参数**/
static const std::string OBJ_SHOW_DIALOG_INTERFACE = "show_dialog_interface";
/**提示消息滚报是否显示(0显示，1不显示) */
static const std::string OBJ_LOUDER_SPEAKER_MASK = "louder_speaker_mask";
/**提示消息内容 1*/
static const std::string OBJ_LOUDER_SPEAKER_TIP1 = "louder_speaker_tip1";
/**提示消息内容 2*/
static const std::string OBJ_LOUDER_SPEAKER_TIP2 = "louder_speaker_tip2";
/**提示消息内容3 */
static const std::string OBJ_LOUDER_SPEAKER_TIP3 = "louder_speaker_tip3";
/**每日签到信息 */
static const std::string OBJ_AWARD_FOR_COME_HERE = "award_for_come_here";
/**是否显示会员标识，是否可以购买会员**/
static const std::string OBJ_CAN_BUY_MEMBER = "can_buy_member";
/**最大手牌（byte[]）*/
static const std::string OBJ_BEST_POKER = "best_poker";
/** 记录用户登录状态 **/
static const std::string OBJ_IS_LOGIN_STATUS = "login_status";

//以下用于存放旧版本任务相关数据
static const std::string PROGRESS_NUMBER = "bouts";
static const std::string ZONE_MULTIPLE = "zoneMutiple";
static const std::string ZONE_WIN_TYPE = "zoneWinType";
static const std::string TXT_TITLE = "txtMultiple";
static const std::string PROCEING = "proceing";
static const std::string GAMEZONEID = "gameZoneID";
static const std::string PROGRESS_NO = "PROGRESS_NO";
//帮助界面文字内容存储
static const std::string HELP_SUBJECT_INTRODUCE = "HELP_SUBJECT_INTRODUCE";//游戏介绍
static const std::string HELP_SUBJECT_NEWER = "SUBJECT_NEWER";//新手必看
static const std::string HELP_SUBJECT_ABOUT_US = "SUBJECT_ABOUT_US";//关于我们
static const std::string HELP_SUBJECT_DECLARATION = "SUBJECT_DECLARATION";//免责声明
static const std::string HELP_PRIVICY              = "HELP_PRIVICY";   //隐私政策
static const std::string HAPPY_RULE                 = "HAPPY_RULE";//欢乐玩法规则
static const std::string LAIZI_RULE                 = "LAIZI_RULE";//癞子玩法规则
static const std::string SERVER_DEAL                =  "SERVER_DEAL";          //服务协议
static const std::string MONEY_DECLARE               = "MONEY_DECLARE";         //货币声明
static const std::string HELP_LAST_UPDATE_TIME = "HELP_LAST_UPDATE_TIME";//上次更新时间

static const char * key_sign_data_days = "key_sign_data_days";

static const char * key_sign_data_date = "key_sign_data_date";

static const char * key_sign_data_time = "key_sign_data_time";

//公告
static const std::string STR_NOTICE_ID = "Notice_Msg_Id";
static const std::string IS_FIRST_BUY_RECORD_CARD = "is_first_buy_record_card";

static const std::string key_region_unicode = "key_region_unicode";
static const std::string key_region_value = "key_region_value";

static const char * key_firstLogin = "key_firstLogin_";

static const char * key_defaultIcon = "key_defaultIcon";
//星星等级
static const char * key_star_level = "key_star_level";
//宝箱局数
static const char * key_count_of_gameAwarded = "key_count_of_gameAwarded";

class MarketLayer;
class UserData : public cocos2d::Ref
{
public:
	
	void setBoolValue(const char* key,bool value);

	void setIntValue(const char* key, int value);

	void setDoubleValue(const char* key,double value);

	void setFloatValue(const char* key,float value);

	//void setCStringValue(const char* key, cocos2d::CCString value);

	void setStringValue(const char* key,std::string value);

	bool getBoolValue(const char* key);

	int getIntValue(const char* key);

	double getDoubleValue(const char* key);

	float getFloatValue(const char* key);

	//cocos2d::CCString getCString(const char* key);

	std::string getString(const char* key);

	//SINGLE_INSTANCE_FUNC(UserData);

	static UserData* getInstance();


    //add on 20150114 wildcat
public:
    UserData();
    virtual ~UserData();
    
    enum eGenderType
    {
        eGenderType_unknow = 0,
        eGenderType_male = 1,
        eGenderType_female = 2
    };
    
    enum eColorCardIndex
    {
        eColorCardIndex_blue = 0,
        eColorCardIndex_red
    };
    
    
    enum eFidType
    {
        eFidType_blue = 10010106,
        eFidType_red = 10010107
    };
    
    enum eNum
    {
        eNum_message = 2
    };
    
    enum eMessageIndex
    {
        eMessageIndex_sign = 0,
        eMessageIndex_redCard
    };
    
    //性别
    void setGender(int gender);

    int getGender();
    
    //用户ID
    void setUserID(int userID);
    
    int getUserID();
    
    //蛙豆
    void setBean(int bean);
    
    int getBean();
    
    //水晶
    void setCrystal(int crystal);
    
    int getCrystal();
    
    //昵称
    void setNickname(const std::string &nickname);
    
    std::string getNickname();
    
    //密码
    void setPassword(const std::string &password);

    std::string getPassword();
    
    //查询签到天数
    int getCurrentSignDay();
    
    /*
     *  查询签到奖励值
     */
    int getSignBean(int dayIndex);
    
    //连续签到
    void setSignDay(int signDay);
    
    int getSignDay();
    
    void setSignDate(const std::string &strSignDate);
    
    std::string getSignDate();
    

    
    /*
     * 0=非VIP会员， 1=VIP会员
     */
    int getVip();
    
    /*
     *  保存签到数据
     */
    void saveSignData();
    
    //游戏ID
    void setGameID(int gameID);
    
    /*
     *  获取游戏ID
     */
    int getGameID();
    
    /*
     *  任务状态
     */
    void setTaskStatus(int taskType, int status);
    
    int getTaskStatus(int taskType);
    
    int getNumOfLeftTasks();
    
    void setIsNewplayerGift(bool value);
    
    bool getIsNewplayerGift();
    
    void setFirstCharge(int arge);
    
    int getFirstCharge();
    
    void setOnlyTaskGot(const std::string strTaskID);
    
    bool getOnlyTaskStatus(const std::string strTaskID);
    
    int isExistNewNotice(cocos2d::Ref *objectData);
    
    void setChangePassword(int userID);
    
    bool getChangePassword(int userID);
    
    void setRegionUnicode(const std::string &strRegion);
    
    void saveRegionUnicode();

    std::string getRegionUnicode();
    
    void setRegionValue(const std::string &strValue);
    
    std::string getRegionValue();
    
    //是否需要强制更新
    CC_SYNTHESIZE(int, m_version, VersionStatue);
    
    //用户基本信息
    //CC_SYNTHESIZE_RETAIN(UserBasicInfoData*, m_basicInfo, BasicInfo);
    
    CC_SYNTHESIZE_RETAIN(GameInfoData *, m_happyGameInfo, HappyGameInfo);
    
    CC_SYNTHESIZE_RETAIN(GameInfoData *, m_laiziGameInfo, LaiziGameInfo);
    
    CC_SYNTHESIZE_RETAIN(StoreMagicList *, m_colorCardList, ColorCardList);//特权卡列表
    
    //默认头像
    void setDefaultIcon(std::string strHeadIcon);
    
    std::string getDefaultIcon();
    
    //
   
    //
    
    void setIsFirstHere(bool value);
    
    bool getIsFirstHere();
    
    /* 
     * 破产倒计时
     * @param brokenDelay 破产倒计时
     */
    void setBrokenDelay(int brokenDelay);
    
    int getBrokenDelay();
    
    //
    bool getIsFirstLoginOfToday(int userID);
    
    void setOrderingOfLeaderboard(int type, int order, int userID);
    
    int getOrderingOfLeaderboard(int type, int userID);
    
    void setNumOfLeaderboard(int num);
    
    int getNumOfLeaderboard();
    
    void setNumOfTaskCompleted(int num);
    
    int getNumOfTaskCompleted();
    
    void setCountOfGameAwarded(int num);
    
    int getCountOfGameAwarded();
    
    friend MarketLayer;
    
    void setExitType(char type);
    
    char getExitType();
    
    /*
     *  功能：设置特权卡信息
     */
    void setColorCardInfo(std::string strStatus, std::string strExp);
    
    /*
     *  功能：查询特权卡状态
     *  return "1"=有卡  "0"=没卡
     */
    std::string getColorCardStatus(eColorCardIndex index);
    
    /*
     *  功能：查询特权卡有效期
     */
    std::string getColorCardExp(eColorCardIndex index);
    
    std::string getColorCardIntroduce(int fid);
    
    //返回红卡领取状态
    bool getIsRedCardAward();
    
    //根据本地数据计算红卡领取状态
    bool caculateRedCardAward();
    
    int getFidWithMagicID(int magicID);
    
    /*
     *  蓝卡破产时间和蛙豆奖励
     */
    void setBlueCardLeftTimeAndAwardBean(const std::string &text);
    
    /*
     * 蓝卡剩余次数和蛙豆
     */
    void setBlueCardNumAndAwardBean(const std::string &text);
    
    /*
     * 蓝卡破产蛙豆
     */
    std::string getBlueCardNextAwardBean(int index);
    
    /*
     * 蓝卡剩余次数
     */
    int getBlueCardLeftNum();
    
    /*
     * 蓝卡最大次数
     */
    int getBlueCardMaxNum();
    
    void setIsShowSignLayer(bool isShow);
    
    bool getIsShowSignLayer();
    
    void setMilsecondOfServerTime(long milsecond);
    
    std::string getServerTimeFormatDaty();
    
    /*
     *  为排行榜没有加载到头像的用户获取一个随机经典头像
     */
    char getClassicIconIndex(int userID);
    
    /*
     *  收到的消息队列，用于界面显示
     */
    void setMessageReceive(int index, int value);
    
    bool getIsAllMessageReceived();
    
    //缓存玩法选择
    void setSeletedPlayType(int type);
    
    int getSeletedPlayType();
private:
    /*
     *
     */
    void setVip(int vip);
    
private:
    bool m_isInitialized;
    
    bool m_isFirstHere;
    
    bool m_isNewPlayerGift;
    
    bool m_isShowSignLayer;
    
    bool m_isRedCardAward;
    
    char m_exitType;
    
    int m_firstCharge;
    
    int m_gameID;
    
    int m_vip;
    
    int m_gender; //性别
    
    int m_userID; //账号
    
    int m_bean; //蛙豆
    
    int m_crystal; //水晶
    
    int m_signDay; //签到天数
    
    int m_taskStatus[7];
    
    long m_signDateOfSecond; //最后一次签到时间
    
    int m_brokenDelay; //
    
    unsigned long m_brokenSecond;
    
    long m_milsecondOfServerTime;
    
    int m_orderingOfEarnBean;
    
    int m_orderingOfCharge;
    
    int m_orderingOfRicher;
    
    int m_numOfLeaderboard;
    
    int m_numOfTaskCompleted;
    
    int m_countOfGameAwarded;
    
    int m_selectPlayType;
    
    int m_aMessage[eNum_message];
    
    std::string m_strNickname; //昵称
    
    std::string m_strPassword; //密码
    
    std::string m_strRegionUnicode;
    
    std::string m_strRegionValue;
    
    std::string m_strDefautlHeadIcon;
    
    std::string m_strSignDate;

    std::string m_strColorCardStatus;
    
    std::string m_strColorCardExp;
    
    std::string m_strBlueCardParam2;
    
    std::string m_strBlueCardParam3;
    
    std::vector<int> m_vUserID;
    
    std::vector<char> m_vClassicIconIndex;

	CC_SYNTHESIZE(int, m_pUserId, pUserId);
	CC_SYNTHESIZE(std::string, m_pNickname, pNickname);
};

#endif

/****************************************************************************
 Copyright (c) Diyal Technologies
 Author : diyal.yin
 Email  : 670924505@qq.com
 Time   : 2015-6-7
 
 Title  : 跨平台Log解决方案
 Detail : 本对象为跨平台日志解决方案，提供如下功能
        1、常规日志筛选，自动加上自定义Tag，方便筛选
        2、写日志文件，提供带Timestamp的自定义日志，并且带上日志级别
        3、专门提供一个网络消息日志接口，文件自定义。（TODO可扩展每条协议发送的响应时间）
        4、可扩展功能（重做日志及删除当天前N天的日志）
        日志文件查看方式：
 Ios ： /Users/yinyanlong/Library/Developer/CoreSimulator/Devices/5F76A861-8108-4659-A0F6-F10CADE9CE69/data/Containers/Data/Application/34EB2742-246A-4138-9B2E-5894E3E6EBA4/Documents/log/2015-06-06.log
 Android : /data/data/包名/file/log/2015-06-06.log
 Win32 : C:\Users\hp\AppData\Local\HZ_DDZ
 
 Sample : wwlog(LOG_ERROR, __FILE__, "My Log %s - %d",str.c_str(),i); //
 
 ****************************************************************************/
#ifndef __DIYAL_BASECORE_LOGUTIL__
#define __DIYAL_BASECORE_LOGUTIL__

typedef enum
{
    LOG_NONE = 0,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_NET
} LEVEL_TYPE; //日志类型

const char LOG_TYPE[7][10] = 
{
    "",
    "Debug",
    "Info",
    "Warning",
    "Error",
    "Net"
};

/* 写log */
#define wwlog(_level ,_method , _format, ...) \
LogUtil::getInstance()->writeLog(_level ,_method , _format, ##__VA_ARGS__)

/* 写log */
#define wwonlylog(_format, ...) \
LogUtil::getInstance()->writeLog(_format, ##__VA_ARGS__)

/* 设置文件名称 */
#define SETLOGFILENAME(_fileName) \
LogUtil::getInstance()->setFileName(_fileName);

/* 设置是否输出日志 */
#define SETLOGENABLE(_enable) \
LogUtil::getInstance()->setEnable(_enable);

/* 设置输出的日志级别 */
#define SETLOGLEVEL(_level) \
LogUtil::getInstance()->setLevelCtrl(_level);

#define SETLOGREMOVE \
LogUtil::getInstance()->remove();

#define MAXLENGTH 1024

#define EVERYDAYMODE 0 //(0:单独文件 1:每日策略、带日期)

#include "cocos2d.h"

class LogUtil : public cocos2d::Ref
{
public:
    
    virtual ~LogUtil();
    static LogUtil* getInstance();
    
    void writeLog(LEVEL_TYPE level , const char* method , const char* format , ...);
    void writeLog(const char* format , ...);
    std::string getDirPath();
    std::string getFilePath();
    void getAllFile(const char *dir, int depth);
    bool remove();
    std::string getCurrentDay(); //获取当前日期
    std::string getCurrentTime(); //获取当前时间
    std::string combine(LEVEL_TYPE level, const char *method , const char* clog); //日志内容格式化

	void setFileName(std::string pFileName); //设置文件名称
	void setLevelCtrl(int pLevelCtrl); //设置日志级别
	void setLogRemove(); //删除日志
    void setEnable(bool isEnable);
private:
    LogUtil();
    std::vector<std::string> files;

	bool enable; //是否写日志（在Release的时候可以关掉）
	std::string fileName;
	LEVEL_TYPE levelCtrl;
    
};
#endif /* __DIYAL_BASECORE_LOGUTIL__ */
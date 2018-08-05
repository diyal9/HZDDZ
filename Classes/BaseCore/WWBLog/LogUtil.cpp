#include "LogUtil.h"
#include <stdarg.h>
#include <sys/stat.h>
#include <stdio.h>
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#else
#include <unistd.h>
#include <dirent.h>
#endif

USING_NS_CC;
using namespace std;

/* 日志配置 */
const char LOG_CONFIG_SECTION[] = "LOG";
const char LOG_CONFIG_LEVELTYPE[10] = "LEVELTYPE"; // 打印日志的等级
const char LOG_DIRNAME[32] = "log"; //日志文件夹

static LogUtil* pLog = NULL;

LogUtil::LogUtil() :
	enable(true),
	levelCtrl(LOG_NONE)
{
	files.clear();
	fileName =  "diyal"; // 默认日志文件名
	this->getAllFile(this->getDirPath().c_str(), 0);
}

LogUtil::~LogUtil(){
	//delete pLog;
	//pLog = NULL;
	CC_SAFE_DELETE(pLog);
}

LogUtil* LogUtil::getInstance()
{
	if (pLog == NULL) {
		pLog = new LogUtil();
	}
	return pLog;
}

void LogUtil::writeLog(LEVEL_TYPE level, const char *method, const char *format, ...){

	if (!enable) {
		return;
	}

	/* 输入检查 */
	if (NULL == format || strcmp(format, "") == 0) {
		return;
	}

	/* 等级控制 */
	if ( level < levelCtrl ) {
		return;
	}

	FILE * fp = fopen(this->getFilePath().c_str(), "at+"); //读写打开一个文件，允许读或在文本末添加数据
	if (fp) {
		char szBuf[MAXLENGTH+1] = {0};
		va_list ap;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_PLATFORM_IOS)
		va_start(ap, format);
		vsnprintf(szBuf, MAXLENGTH, format, ap);
		va_end(ap);
#else
		try
		{
			va_start(ap, format);
			vsprintf_s(szBuf, MAXLENGTH, format, ap);
			va_end(ap);
		}
		catch (...)
		{
		}
#endif 
		string tmp = this->combine(level, method, szBuf);
		fwrite(tmp.c_str(), 1, tmp.size(), fp);
		fclose(fp);
	}
}

void LogUtil::writeLog(const char *format, ...){


	if (!enable) {
		return;
	}

	/* 输入检查 */
	if (NULL == format || strcmp(format, "") == 0) {
		return;
	}

	FILE * fp = fopen(this->getFilePath().c_str(), "at+");
	if (fp) {
		char szBuf[MAXLENGTH+1] = {0};
		va_list ap;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_PLATFORM_IOS)
		va_start(ap, format);
		vsnprintf(szBuf, MAXLENGTH, format, ap);
		va_end(ap);
#else
		try
		{
			va_start(ap, format);
			vsprintf_s(szBuf, MAXLENGTH, format, ap);
			va_end(ap);
		}
		catch (...)
		{
		}
#endif 

		string tmp = this->combine(LOG_INFO, "non", szBuf);
		fwrite(tmp.c_str(), 1, tmp.size(), fp);
		fclose(fp);
	}

}

string LogUtil::getDirPath(){
	std::string writablePath = FileUtils::getInstance()->getWritablePath();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	writablePath = "/mnt/sdcard/";
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	writablePath.append(LOG_DIRNAME);
	if(access(writablePath.c_str(),0) !=0)//判断目录是否存在
	{
		mkdir((writablePath.c_str()),0755);
	}
#endif
	return writablePath.append("/");
}

string LogUtil::getFilePath()
{
	string filePath;
	if (EVERYDAYMODE == 0)
	{
		return this->getDirPath().append(fileName).append(".log");
	}
	else
	{
		return this->getDirPath().append(fileName).append(this->getCurrentDay()).append(".log");
	}
}

void LogUtil::getAllFile(const char *dir, int depth)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#else
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(dir)) == NULL) {
		fprintf(stderr,"cannot open directory: %s\n", dir);
		return;
	}
	chdir(dir);
	while((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) {
			if(strcmp(".",entry->d_name) == 0 ||
				strcmp("..",entry->d_name) == 0)
				continue;
			printf("in %*s%s/\n",depth,"",entry->d_name);
		}
		files.push_back(entry->d_name);
	}
	chdir("..");
	closedir(dp);
#endif
}

bool LogUtil::remove(){
	if (!files.empty()) {
		for (int i = 0 ; i < files.size() ; i++) {
			::remove(files[i].c_str());
			//unlink(files[i].c_str());
			printf("remove the file - %s\n" , files[i].c_str());
		}
		return true;
	}
	return false;
}

string LogUtil::getCurrentDay(){
	char timeNow[20];
	time_t lt;
	struct tm * tp;
	lt = time(NULL);
	tp = localtime(&lt);
	strftime(timeNow, 20, "%Y-%m-%d", tp);
	return timeNow;
}

string LogUtil::getCurrentTime(){

	time_t t = time(0);
	char timeNow[64] = {0};
	strftime( timeNow, sizeof(timeNow), "[%Y/%m/%d %X] ", localtime(&t) ); 

	return string(timeNow);
}

string LogUtil::combine(LEVEL_TYPE level, const char *method , const char* clog){
	std::string content("");
	content.append(this->getCurrentTime());
	content.append("-[");
	content.append(LOG_TYPE[level]);
	content.append("]-[");
	content.append(method);
	content.append("]-[");
	if (level == LOG_NET) {
		content.append("Msg:");
	}

	content.append(clog);
	content.append("]");
	content.append("\n");

	log("%s", content.c_str());

	return content;
}

void LogUtil::setFileName(std::string pFileName)
{
	fileName = pFileName;
}

void LogUtil::setLevelCtrl(int pLevelCtrl)
{
	switch (pLevelCtrl)
	{
	case LOG_NONE:
		levelCtrl = LOG_NONE;
	case LOG_DEBUG:
		levelCtrl = LOG_DEBUG;
	case LOG_INFO:
		levelCtrl = LOG_INFO;
	case LOG_WARNING:
		levelCtrl = LOG_WARNING;
	case LOG_ERROR:
		levelCtrl = LOG_ERROR;
	default:
		break;
	}
}

void LogUtil::setLogRemove()
{
	remove();
}

void LogUtil::setEnable(bool isEnable)
{
	enable = isEnable;
}
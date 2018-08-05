

/*
 * WWSoundManager.cpp
 *声音管理
 *  Created on: 2014年5月16日
 *      Author: YYH
 *  Editor: Toby on 2015-4-28
 */
#ifndef __HD_DDZ_Control_WWCSound_WWSoundManager_H__
#define __HD_DDZ_Control_WWCSound_WWSoundManager_H__


#include "cocos2d.h"
#include "../../WWMacros.h"
#include "SimpleAudioEngine.h"
/**
 * 声音管理
 */
class WWSoundManager: public Ref {
public:
	WWSoundManager();

	virtual ~WWSoundManager();
	SINGLE_INSTANCE_FUNC(WWSoundManager);
	/**
	 * 释放  shared Engine object
	 */
	static void end();
	/**
	 * 预加载背景音乐
	 */
	void preloadBackgroundMusic(const char* pszFilePath);
	/**
	 * 播放背景音乐
	 * bLoop 是否循环播放
	 */
	void playBackgroundMusic(const char* pszFilePath, bool bLoop);

	/**
	 * 播放背景音乐（不循环）
	 */
	void playBackgroundMusic(const char* pszFilePath);
	/**
	 * 停止背景音乐
	 */
	void stopBackgroundMusic(bool bReleaseData);
	/**
	 * 停止背景音乐
	 */
	void stopBackgroundMusic();
	/**
	 * 暂停背景音乐
	 */
	void pauseBackgroundMusic();
	/**
	 * 重新开始背景音乐
	 */
	void resumeBackgroundMusic();
	/**
	 * 回放背景音乐
	 */
	void rewindBackgroundMusic();
	/**
	 * 是否会播放背景音乐
	 */
	bool willPlayBackgroundMusic();
	/**
	 * 是否整播放背景音乐
	 */
	bool isBackgroundMusicPlaying();

	/**
	 * 获得背景音乐音量
	 */
	float getBackgroundMusicVolume();
	/**
	 * 设置背景音乐音量
	 */
	void setBackgroundMusicVolume(float volume);
	/**
	 * 获得音效音量
	 */
	float getEffectsVolume();
	/**
	 * 设置音效音量
	 */
	void setEffectsVolume(float volume);

	/**
	 * 播放音效
	 * bLoop 是否循环
	 */
	unsigned int playEffect(const char* pszFilePath, bool bLoop);
	unsigned int playEffect(const char* pszFilePath);
	/**
	 * 暂停音效
	 * nSoundId 播放时获得的id号
	 */
	void pauseEffect(unsigned int nSoundId);
	/**
	 * 暂停所有音效
	 */
	void pauseAllEffects();
	/**
	 * 开始所有音效
	 * nSoundId 播放时获得的id号
	 */
	void resumeEffect(unsigned int nSoundId);
	/**
	 * 开始所有音效
	 */
	void resumeAllEffects();
	/**
	 * 停止音效
	 * nSoundId 播放时获得的id号
	 */
	void stopEffect(unsigned int nSoundId);
	/**
	 * 停止所有音效
	 */
	void stopAllEffects();
	/**
	 * 预加载音效
	 */
	void preloadEffect(const char* pszFilePath);
	/**
	 * 将预加载的音效从缓存中删除
	 */
	void unloadEffect(const char* pszFilePath);
    

};

#endif  /*__HD_DDZ_Control_WWCSound_WWSoundManager_H__*/

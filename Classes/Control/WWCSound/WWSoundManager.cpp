/*
 * WWSoundManager.cpp
 *声音管理
 *  Created on: 2014年5月16日
 *      Author: YYH
 *  Editor: Toby on 2015-4-28
 */
#include "WWSoundManager.h"
USING_NS_CC;

WWSoundManager::WWSoundManager() {

}

WWSoundManager::~WWSoundManager() {

}

 void WWSoundManager::end() {
	CocosDenshion::SimpleAudioEngine::getInstance()->end();
}

void WWSoundManager::preloadBackgroundMusic(const char* pszFilePath) {
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(pszFilePath);

}

void WWSoundManager::playBackgroundMusic(const char* pszFilePath, bool bLoop) {
	bool isMusicClose = UserDefault::getInstance()->getBoolForKey(SET_BG_MUSIC_KEY,true);
	if (!isMusicClose)
	{
		return;
	}
	bool isNosound = UserDefault::getInstance()->getBoolForKey(SET_NO_MUSIC_KEY,false);
	if (isNosound)
	{
		return;
	}
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath, bLoop);

}
void WWSoundManager::playBackgroundMusic(const char* pszFilePath) {
	this->playBackgroundMusic(pszFilePath, false);
}

void WWSoundManager::stopBackgroundMusic(bool bReleaseData) {
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(bReleaseData);

}
void WWSoundManager::stopBackgroundMusic() {
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);

}

void WWSoundManager::pauseBackgroundMusic() {
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void WWSoundManager::resumeBackgroundMusic() {
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void WWSoundManager::rewindBackgroundMusic() {
	CocosDenshion::SimpleAudioEngine::getInstance()->rewindBackgroundMusic();
}
bool WWSoundManager::willPlayBackgroundMusic() {
	return CocosDenshion::SimpleAudioEngine::getInstance()->willPlayBackgroundMusic();
}

bool WWSoundManager::isBackgroundMusicPlaying() {
	return CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}

float WWSoundManager::getBackgroundMusicVolume() {
	return CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
}

void WWSoundManager::setBackgroundMusicVolume(float volume) {
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
}

float WWSoundManager::getEffectsVolume() {
	return CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume();

}

void WWSoundManager::setEffectsVolume(float volume) {
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(volume);

}

unsigned int WWSoundManager::playEffect(const char* pszFilePath, bool bLoop) {
	return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pszFilePath, bLoop);

}
unsigned int WWSoundManager::playEffect(const char* pszFilePath) {
	return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pszFilePath);

}

void WWSoundManager::pauseEffect(unsigned int nSoundId) {
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(nSoundId);

}

void WWSoundManager::pauseAllEffects() {
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();

}

void WWSoundManager::resumeEffect(unsigned int nSoundId) {
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeEffect(nSoundId);

}

void WWSoundManager::resumeAllEffects() {
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();

}

void WWSoundManager::stopEffect(unsigned int nSoundId) {
	CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(nSoundId);

}

void WWSoundManager::stopAllEffects() {
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();

}

void WWSoundManager::preloadEffect(const char* pszFilePath) {
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(pszFilePath);

}

void WWSoundManager::unloadEffect(const char* pszFilePath) {
	CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect(pszFilePath);

}

/*
 * WWCacheManager.cpp
 *
 *  Created on: 2014年5月19日
 *      Author: wly
 */

#include "WWCacheManager.h"
USING_NS_CC;

WWCacheManager::WWCacheManager(void)
{
}


WWCacheManager::~WWCacheManager(void)
{
	AnimationCache::destroyInstance();
	SpriteFrameCache::destroyInstance();
}

void WWCacheManager::purgeAllCache()
{
	//保证如下调用顺序，按照引用层级由高到低
	AnimationCache::destroyInstance();
	SpriteFrameCache::destroyInstance();
}

void WWCacheManager::removeAllUnusedCache()
{
	//保证如下调用顺序，按照引用层级由高到低
	AnimationCache::destroyInstance();
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void WWCacheManager::addSpriteFramesWithFile(const std::string& plist)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist);
}

void WWCacheManager::addSpriteFramesWithFile(const std::string& plist, const std::string& textureFileName)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist, textureFileName);
}

void WWCacheManager::addSpriteFramesWithFile(const std::string& plist, Texture2D *texture)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist, texture);
}

void WWCacheManager::addSpriteFrame(SpriteFrame *frame, const std::string& frameName)
{
	SpriteFrameCache::getInstance()->addSpriteFrame(frame, frameName);
}

void WWCacheManager::removeSpriteFrames()
{
	SpriteFrameCache::getInstance()->removeSpriteFrames();
}

void WWCacheManager::removeUnusedSpriteFrames()
{
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

void WWCacheManager::removeSpriteFrameByName(const std::string& name)
{
	SpriteFrameCache::getInstance()->removeSpriteFrameByName(name);
}

void WWCacheManager::removeSpriteFramesFromFile(const std::string& plist)
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plist);
}

void WWCacheManager::removeSpriteFramesFromTexture(Texture2D* texture)
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromTexture(texture);
}

SpriteFrame* WWCacheManager::getSpriteFrameByName (const std::string& name)
{
	return SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
}

std::string WWCacheManager::getDescription() const
{
	return Director::getInstance()->getTextureCache()->getDescription();
}

Texture2D* WWCacheManager::addImage(const std::string &filepath)
{
	return Director::getInstance()->getTextureCache()->addImage(filepath);
}

void WWCacheManager::addImageAsync(const std::string &filepath, const std::function<void(Texture2D*)>& callback)
{
	Director::getInstance()->getTextureCache()->addImageAsync(filepath, callback);
}

void WWCacheManager::unbindImageAsync(const std::string &filename)
{
	Director::getInstance()->getTextureCache()->unbindImageAsync(filename);
}

void WWCacheManager::unbindAllImageAsync()
{
	Director::getInstance()->getTextureCache()->unbindAllImageAsync();
}

Texture2D* WWCacheManager::addImage(Image *image, const std::string &key)
{
	return Director::getInstance()->getTextureCache()->addImage(image, key);
}

Texture2D* WWCacheManager::getTextureForKey(const std::string& key) const
{
	return Director::getInstance()->getTextureCache()->getTextureForKey(key);
}

bool WWCacheManager::reloadTexture(const std::string& fileName)
{
	return Director::getInstance()->getTextureCache()->reloadTexture(fileName);
}

void WWCacheManager::removeAllTextures()
{
	Director::getInstance()->getTextureCache()->removeAllTextures();
}

void WWCacheManager::removeUnusedTextures()
{
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void WWCacheManager::removeTexture(Texture2D* texture)
{
	Director::getInstance()->getTextureCache()->removeTexture(texture);
}

void WWCacheManager::removeTextureForKey(const std::string &key)
{
	Director::getInstance()->getTextureCache()->removeTextureForKey(key);
}

std::string WWCacheManager::getCachedTextureInfo() const
{
	return Director::getInstance()->getTextureCache()->getCachedTextureInfo();
}

void WWCacheManager::waitForQuit()
{
	Director::getInstance()->getTextureCache()->waitForQuit();
}

void WWCacheManager::purgeSharedAnimationCache()
{
	AnimationCache::getInstance()->destroyInstance();
}

void WWCacheManager::addAnimation(Animation *animation, const std::string& name)
{
	AnimationCache::getInstance()->addAnimation(animation, name);
}

void WWCacheManager::removeAnimation(const std::string& name)
{
	AnimationCache::getInstance()->removeAnimation(name);
}

Animation* WWCacheManager::getAnimation(const std::string& name)
{
	return AnimationCache::getInstance()->getAnimation(name);
}

void WWCacheManager::addAnimationsWithDictionary(const ValueMap& dictionary,const std::string& plist)
{
	AnimationCache::getInstance()->addAnimationsWithDictionary(dictionary, plist);
}

void WWCacheManager::addAnimationsWithFile(const std::string& plist)
{
	AnimationCache::getInstance()->addAnimationsWithFile(plist);
}


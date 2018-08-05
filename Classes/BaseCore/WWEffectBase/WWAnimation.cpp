#include "WWAnimation.h"

static WWAnimation* g_pAnimation = NULL;

WWAnimation::WWAnimation()
{

}

WWAnimation::~WWAnimation()
{

}

WWAnimation* WWAnimation::getInstance()
{
	if (!g_pAnimation) {
		g_pAnimation = new WWAnimation();
	}
	return g_pAnimation;
}

void WWAnimation::init()
{
	//加载动画资源配置文件
	//Json::Value pv;
	//Json::Reader jsReader;
	//std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/JsonData/Animation.json");

	//CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
	//if(pcstr == NULL)
	//{
	//	CCLOG("This file %s is not exist!!!", "Data/JsonData/Animation.json");
	//	return;
	//}
	//jsReader.parse(pcstr->getCString(), pv, false);
	//Json::Value configData = pv["data"];
	//int animSize = configData.size();

	//for(int i=0; i < animSize; ++i)
	//{
	//	m_vAnimConfig.insert(std::pair<std::string, Json::Value>(configData[i]["name"].asString(), configData[i]));
	//}
}

Animation* WWAnimation::createAnimationByName(const std::string& animName, float delay)
{
	Animation *pAnimation = NULL;
	//查找已经加载过的动画
	std::map<std::string, Animation*>::iterator animIter = m_vAnimations.find(animName);
	if (animIter != m_vAnimations.end())
	{
		pAnimation = animIter->second;
	}
	else
	{
		//获取动画配置
		std::map<std::string, Value>::iterator configIter = m_vAnimConfig.find(animName);
		if (configIter != m_vAnimConfig.end())
		{
			//加载动画资源
			SpriteFrameCache* cache = SpriteFrameCache::getInstance();
			cache->addSpriteFramesWithFile(("res/Images/Animation/" + animName + ".plist").c_str(),
				("res/Images/Animation/" + animName + ".png").c_str());

			Value animConfig = configIter->second;
			int frameCnt = 10; //TODO帧数

			Vector<SpriteFrame*> *animFrames = nullptr;
			for (int i = 0; i < frameCnt; ++i)
			{
				const char* realAnimName = String::createWithFormat("%s_%d.png", animName.c_str(), i+1)->getCString();
				auto frame = (SpriteFrame*)cache->getSpriteFrameByName(realAnimName);
				animFrames->pushBack(frame);
			}
			pAnimation = Animation::createWithSpriteFrames(*animFrames, 0.2f);
			m_vAnimations.insert(std::pair<std::string, Animation*>(animName, pAnimation));

		}
		else
		{
			log("%s animation config be not set", animName.c_str());
		}
	}
	return NULL;
}

Animation* WWAnimation::animationUnCache(const std::string& animName, int frameCnt, float delay, int times)
{
	//加载动画资源
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile(("res/Images/Animation/" + animName + ".plist").c_str(),
		("res/Images/Animation/" + animName + ".png").c_str());

	Vector<SpriteFrame*> animFrames(frameCnt);
	char str[100] = {0};
	for (int i = 0; i < frameCnt; ++i)
	{
		//const char* realAnimName = String::createWithFormat("%s_%d.png", animName.c_str(), i+1)->getCString();
		sprintf(str, "%s_%d.png", animName.c_str(), i + 1);
		auto frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	auto pAnimation = Animation::createWithSpriteFrames(animFrames, delay);

	if (pAnimation)
	{
		return pAnimation;
	}

	return nullptr;
}

void WWAnimation::removeAnimationCache(const std::string& animName)
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("res/Images/Animation/" + animName + ".plist");
	log("remove animation SpriteFrameCache res/Images/Animation/%s.plist", animName.c_str());
}

Sprite* WWAnimation::createAnimationSprite(const std::string &animName)
{
	return Sprite::createWithSpriteFrameName((animName + "_1.png").c_str());
}
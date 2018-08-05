//
//  GameSoundManager.cpp
//  WW_Platform
//
//  Created by 王体强 on 8/15/14.
//
//  Edited by Toby on 6/1/2015
//

#include "GameSoundManager.h"
#include "WWSoundManager.h"
USING_NS_CC;

GameSoundManager::GameSoundManager()
{
    
}

GameSoundManager::~GameSoundManager()
{
    m_soundInfo.clear();
}
static GameSoundManager* s_soundManager = NULL;


GameSoundManager* GameSoundManager::getInstance()
{
    if (!s_soundManager) 
	{
        s_soundManager = new GameSoundManager();

		ValueMap soundInfo;
		if (FileUtils::getInstance()->isFileExist(soundInfoPath)) 
		{
			soundInfo = FileUtils::getInstance()->getValueMapFromFile(soundInfoPath);

			CCLOG("the soundinfo key count==%d",soundInfo.size());
		}
		else
		{
			CCLOG("soundinfo is not exist");
		}
		if (!s_soundManager->initWithSoundInfo(soundInfo))
		{
			delete s_soundManager;
			s_soundManager = NULL;
		}
    }
    return s_soundManager;
}

bool GameSoundManager::initWithSoundInfo(const ValueMap &soundInfo)
{
    if (soundInfo.size() == 0)
	{
        return false;
    }
    m_soundInfo = soundInfo;
    return true;
}

void GameSoundManager::playBackGroundMusic(const std::string &musicKey)
{
	bool isSilence = UserDefault::getInstance()->getBoolForKey(SET_GAME_MUSIC_KEY,true);
	if (!isSilence)
	{
		return;
	}
	bool isNosound = UserDefault::getInstance()->getBoolForKey(SET_NO_MUSIC_KEY,false);
	if (isNosound)
	{
		return;
	}
    std::string filePath = getFilePathWithKey(musicKey);
    WWSoundManager::getInstance()->playBackgroundMusic(filePath.c_str(), true);
}

std::string GameSoundManager::getFilePathWithKey(const std::string& key)
{
    ValueMap::iterator iter = m_soundInfo.find(key);
    std::string fileName;
    //动态类型检测
	if (iter != m_soundInfo.end())
	{
		if ((*iter).second.getType() == cocos2d::Value::Type::VECTOR)
		{
			ValueVector vecSound = (*iter).second.asValueVector();

			fileName = vecSound.at(getRandomNumInScope(vecSound.size())).asString();
		}
		else
		{
			fileName = (*iter).second.asString();
		}
	}

    std::string filePath = soundPath+fileName;
    CC_ASSERT(CCFileUtils::sharedFileUtils()->isFileExist(filePath));
    return filePath;
}

void GameSoundManager::stopBackGroundMusic(bool bReleaseData)
{
    WWSoundManager::getInstance()->stopBackgroundMusic(bReleaseData);
}

void GameSoundManager::playRandomEffectWith(const cocos2d::ValueVector &vecSound)
{
	bool isSilence = UserDefault::getInstance()->getBoolForKey(SET_GAME_MUSIC_KEY,true);
	if (!isSilence)
	{
		return;
	}
	bool isNosound = UserDefault::getInstance()->getBoolForKey(SET_NO_MUSIC_KEY,false);
	if (isNosound)
	{
		return;
	}
    CC_ASSERT(vecSound.size() > 0);

    std::string effectStr = vecSound.at(getRandomNumInScope(vecSound.size())).asString();
    playEffectWithKey(effectStr);
}

void GameSoundManager::playEffectWithKey(const std::string &key)
{
    bool isSilence = UserDefault::getInstance()->getBoolForKey(SET_GAME_MUSIC_KEY,true);
    if (!isSilence)
    {
        return;
    }
    bool isNosound = UserDefault::getInstance()->getBoolForKey(SET_NO_MUSIC_KEY,false);
	if (isNosound)
	{
		return;
	}
    CCLOG("the play key===%s",key.c_str());
    CC_ASSERT(&key&&key.length()>0);
	WWSoundManager::getInstance()->playEffect(getFilePathWithKey(key).c_str());
	
    
}


void GameSoundManager::playEffectWithNameArr(const cocos2d::ValueVector &vecName)
{
	bool isSilence = UserDefault::getInstance()->getBoolForKey(SET_GAME_MUSIC_KEY,true);
	if (!isSilence)
	{
		return;
	}
	bool isNosound = UserDefault::getInstance()->getBoolForKey(SET_NO_MUSIC_KEY,false);
	if (isNosound)
	{
		return;
	}
	//取数组中一个随机声音播放
    std::string soundName = vecName.at(getRandomNumInScope(vecName.size())).asString();
    std::string filePath = soundPath+soundName;
    CC_ASSERT(FileUtils::sharedFileUtils()->isFileExist(filePath));
    WWSoundManager::getInstance()->playEffect(filePath.c_str());
}

void GameSoundManager::playEffectWithPath(const std::string &path)
{
    bool isSilence = UserDefault::getInstance()->getBoolForKey(SET_GAME_MUSIC_KEY,true);
    if (!isSilence)
    {
        return;
    }
	bool isNosound = UserDefault::getInstance()->getBoolForKey(SET_NO_MUSIC_KEY,false);
	if (isNosound)
	{
		return;
	}
    WWSoundManager::getInstance()->playEffect(path.c_str());
}

void GameSoundManager::playEffectWithName(const std::string &fileName)
{
    bool isSilence = UserDefault::getInstance()->getBoolForKey(SET_GAME_MUSIC_KEY,true);
    if (!isSilence)
    {
        return;
    }
	bool isNosound = UserDefault::getInstance()->getBoolForKey(SET_NO_MUSIC_KEY,false);
	if (isNosound)
	{
		return;
	}
    std::string path = soundPath+fileName;
    playEffectWithPath(path);
}

void GameSoundManager::playPokerAudio(kWeightValue &weight)
{
    std::string audioKey;
    switch (weight.type) 
	{
        case T_Unknow:
            audioKey = buyao_key;
            break;
        case T_ONE:
			{
				ValueVector vecSingleCard = (*m_soundInfo.find(outPokerOne_key)).second.asValueVector();
				cocos2d::Value valueObj = vecSingleCard.at(weight.number-1);

				if (valueObj.getType() == cocos2d::Value::Type::VECTOR)
				{
					playEffectWithNameArr(valueObj.asValueVector());
				}
				else
				{
					if (valueObj.getType() == cocos2d::Value::Type::STRING)
					{
						playEffectWithName(valueObj.asString());
					}
				}
				return;
			 }
             break;
        case T_TWO:
			{
				ValueVector vecSingleCard = (*m_soundInfo.find(outPokerTwo_key)).second.asValueVector();
				cocos2d::Value valueObj = vecSingleCard.at(weight.number-1);
				if (valueObj.getType() == cocos2d::Value::Type::VECTOR)
				{
					playEffectWithNameArr(valueObj.asValueVector());
				}
				else
				{
					if (valueObj.getType() == cocos2d::Value::Type::STRING)
					{
						playEffectWithName(valueObj.asString());
					}
				}
				return;
			}
            break;
        case T_THREE:
            audioKey = sanzhang_key;
            break;
        case T_THREE_1:
            if (weight.pokerCount>4)
            {
                audioKey = feiji_key;
            }
            else
            {
                audioKey = sandaiyi_key;
            }
            
            break;
        case T_THREE_2:
            if (weight.pokerCount>5)
            {
                audioKey = feiji_key;
            }
            else
            {
                audioKey = sandaier_key;
            }
            break;
        case T_SEQ_1:
            audioKey = shunzi_key;
            break;
        case T_SEQ_2:
            audioKey = liandui_key;
            break;
        case T_SEQ_3:
            audioKey = feiji_key;
            break;
        case T_BOMB:
            audioKey = bomb_key;
            break;
        case T_BOMB_L:
            audioKey = bomb_key;
            break;
        case T_BOMB_S:
            audioKey = bomb_key;
            break;
        case T_Rocket_2:
            audioKey = rocket_key;
            break;
        case T_FOUR_2:
            audioKey = sidaier_key;
            break;
        case T_FOUR_4:
            audioKey = sidaier_key;
            break;
        default:
            break;
    }
    CCLOG("playeffect weight number=%d,type=%d,count=%d",weight.number,weight.type,weight.pokerCount);
    playEffectWithKey(audioKey);
}

int GameSoundManager::getRandomNumInScope(int nScope)
{
	timeval psv;
	gettimeofday( &psv, NULL );										 // 计算时间种子
	unsigned int tsrans = psv.tv_sec * 1000 + psv.tv_usec / 1000;    // 初始化随机数
	srand( tsrans );

	return (int)(CCRANDOM_0_1()*1000)%nScope;
}

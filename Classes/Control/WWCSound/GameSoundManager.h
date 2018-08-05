//
//  GameSoundManager.h
//  
//
//  Created by Toby on 6/1/15.
//
//
/*
 本类用于播放游戏中的音乐和出牌音效，按钮音效等等
 采用了plist文件key value方式来存储音效
 有些出牌的音效由于不止一种，所以暂时的处理方式是随机从中选择一种出牌，不过这种方式可能会随着产品需求来进行优化
 可以适当增加一些出牌音效的借口。
 其中数组中音效读取采用了动态类型检测的方式（墙砖后判断知否为空，虽然是伪动态类型），来判断要播放的声音是唯一的还是有多个
 */

#ifndef __HD_DDZ_Control_WWCSoundGameSoundManager_H__
#define __HD_DDZ_Control_WWCSoundGameSoundManager_H__



#include <iostream>
#include "cocos2d.h"
#include "../../WWMacros.h"

const static std::string soundPath = "res/Audio/mp3/";                     //声音根目录
const static std::string soundInfoPath = "res/Audio/mp3/soundsInfo.plist"; //声音配置文件
const static std::string backGoundMusic_key = "backGroundMusic";    //背景音乐
const static std::string outPokerOne_key = "outPoker_one";          //单张
const static std::string buyao_key = "buyao";                       //不出
const static std::string outPokerTwo_key = "outPoker_two";          //对子
const static std::string dazhu_key = "dazhu";                       //大住对方的牌
const static std::string rocket_key = "rocket";                     //火箭
const static std::string bomb_key = "zhadan";                       //炸弹
const static std::string liandui_key = "liandui";                   //连对
const static std::string sandui_key = "sandui";                     //三对
const static std::string sandaiyi_key = "sandaiyi";                 //三带一
const static std::string shunzi_key = "shunzi";                     //顺子
const static std::string feiji_key = "feiji";                       //飞机
const static std::string lordWin_key = "dizhuwin";                  //地主赢
const static std::string sandaier_key = "sandaier";                 //三带二
const static std::string sanzhang_key = "sanzhang";                 //三张
const static std::string sidaier_key = "sidaier";                   //四带二
const static std::string getBean_key = "getBean_success";           //获取蛙豆成功
const static std::string failed_key = "fail";                       //失败
const static std::string win_key = "win";                           //胜利
const static std::string click_key = "click";                       //点击
const static std::string yaoPai_key = "yao";                        //要牌
const static std::string pageChange_key = "changed";                //翻页
const static std::string zaiqiang_key = "zaiqiang";                 //再抢
const static std::string qiangdizhu_key = "qiangdizhu";             //抢地主
const static std::string mingpai_key = "mingpai";                   //明牌
const static std::string bujiao_key = "bujiao";                     //不叫
const static std::string buqiang_key = "buqiang";                   //不抢
const static std::string jiaodizhu_key = "jiaodizhu";               //叫地主
const static std::string expend_key = "expend";                     //破产
const static std::string zaiqiang_file = "call_zaiqiang_2.mp3";     //再抢2

#define SOUND_MANGER GameSoundManager::getInstance()

#define PLAY_EFFECT(KEY) SOUND_MANGER->playEffectWithKey(KEY)  //通过key播放音效,键值对应关系在sounds/soundInfo.plist中

#define PLAY_EFFECT_PATH(PATH) SOUND_MANGER->playEffectWithPath(PATH) //通过路径播放音效

#define PLAY_EFFECT_NAME(FILE_NAME) SOUND_MANGER->playEffectWithName(FILE_NAME) //通过文件名播放（在sounds文件夹中的文件）

USING_NS_CC;

class GameSoundManager:public Ref
{
public:
    GameSoundManager();
    virtual ~GameSoundManager();
    
    static GameSoundManager* getInstance();
    //根据出牌类型播放语音
    void playPokerAudio(kWeightValue &weight);
    
    //播放背景音乐
    void playBackGroundMusic(const std::string &musicKey);
    
    //背景音乐停止
    void stopBackGroundMusic(bool bReleaseData);
    
    //通过key播放音频
    void playEffectWithKey(const std::string &key);
    
    //通过路径播放音频
    void playEffectWithPath(const std::string &path);
    
    //通过文件名播放
    void playEffectWithName(const std::string &fileName);
    
private:
    //音频配置
    cocos2d::ValueMap m_soundInfo;
    
private:
    //通过名称数组播放音频
    void playEffectWithNameArr(const cocos2d::ValueVector &vecName);

    //获取通过key获取资源路径
    std::string getFilePathWithKey(const std::string &key);
    
    //从数组中选一个随机数播放
    void playRandomEffectWith(const cocos2d::ValueVector &vecSound);
    
    //初始化音频内容
    bool initWithSoundInfo(const cocos2d::ValueMap &soundInfo);

	//获取一个到指定范围内的随机数
	int getRandomNumInScope(int nScope);
    
};
#endif /* defined(__HD_DDZ_Control_WWCSoundGameSoundManager_H__) */

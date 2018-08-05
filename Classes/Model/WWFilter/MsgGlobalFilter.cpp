/*
 * MsgGlobalFilter.cpp
 *	功能描述：
 *  Created on: 2014年7月15日 上午11:14:31
 *      Author: hp
 */

#include "MsgGlobalFilter.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Model/WWDataModel/WWUserData/UserData.h"
#include "Model/WWDataModel/WWGD/GD.h"
#include "Control/WWDataManager/WWDataManager.h"
#include "Control/WWDataManager/GameGlobalDataManager.h"
#include "Model/WWDataModel/WWGameData/GameStartData.h"
#include "Model/WWDataModel/CommonMessageData/CommonMessageData.h"
#include "View/Common/CommLayer/CommDialog.h"
#include "View/GameScene/GameBasicScene.h"
#include "View/GameScene/GameLogicManager.h"
#include "BaseCore/WWBLog/LogUtil.h"

const int filters[] =
{ MSG_PORP_ISSUE_NOTIFY, MSG_LOGON_AWARD_INFO, MSG_MEDIA_CONTENT, MSG_GAME_PLAYERS_INFO,
MSG_ACTIVITY_NOTIY, MSG_TOP_SCORE,EQUIP_LIST_ID,EQUIP_DETAIL_INF_ID, MSG_GAME_RESUME_RECORD_CARD,OPEN_BOX_RESP,CLEAN_SCORE_RESP, MSG_GLOBAL_RESULT_INFO,MSG_GAME_USER_BASIC_INFO};

const int rootFilters[] =
{ 0};

//记录收到玩家数据信息的次数，3次表示收完
static int nReceivePlayersInfoCount = 0;

USING_NS_CC;

MsgGlobalFilter::MsgGlobalFilter()
{
}

MsgGlobalFilter::~MsgGlobalFilter()
{
}

bool MsgGlobalFilter::init()
{
	return true;
}

const std::vector<int> vfilters(filters, filters + sizeof(filters) / sizeof(int));

std::vector<int> MsgGlobalFilter::onFilters()
{
	return vfilters;
}

bool MsgGlobalFilter::onMessageReceived(int msgType)
{
	//CCLOG("MsgGlobalFilter::onMessageReceived %x", msgType);
	LogUtil::getInstance()->writeLog(LEVEL_TYPE::LOG_NET,"MsgGlobalFilter::onMessageReceived %x", "msgType = %x" ,msgType);

    void* receivedData = nullptr;
	switch (msgType)
	{
        case MSG_PORP_ISSUE_NOTIFY:
           // parsePropIssue(oBuffer);
			break;
        case MSG_LOGON_AWARD_INFO:
            //parseLogonAwardInfo(oBuffer);
            return true;
        case MSG_MEDIA_CONTENT:
            //parseMediaContentInfo(oBuffer);
            return true;
     /*   case MSG_MAIN_USER_INFO:
            parseMainUserInfo();
            return true;*/
       /* case MSG_RECOVERY_GAME_NOTIY:
            parseRecoveryGameNofity();
            return true;
        case MSG_GAME_RESUME:
            parseResumeGame();
            return true;*/
		case MSG_GAME_USER_BASIC_INFO:
			//parseGameUserBasicInfo();
			return true;
        default:
            return false;
	}

	return false;
}

const std::vector<int> vrootfilters(rootFilters, rootFilters + sizeof(rootFilters) / sizeof(int));

std::vector<int> MsgGlobalFilter::onRootFilters()
{
	return vrootfilters;
}

bool MsgGlobalFilter::onRootMessageReceived(int msgType, int userId, int result, std::string reason, int reasonType)
{
	CCLOG("MsgGlobalFilter::onRootMessageReceived:msgType= %x,userId=%d,result=%d,reason=%s,reasonType=%x", msgType, userId, result, reason.c_str(), reasonType);
	LogUtil::getInstance()->writeLog(LEVEL_TYPE::LOG_NET,"MsgGlobalFilter::onRootMessageReceived","msgType= %x,userId=%d,result=%d,reason=%s,reasonType=%x", msgType, userId, result, reason.c_str(), reasonType);


	CommonMessageData* data = CommonMessageData::create();
	data->setUserId(userId);
	data->setResult(result);
	data->setReasonType(reasonType);
	data->setReason(reason);
	CommDialog* dialog;
	/*switch (msgType)
	{
	case MSG_ENTER_GAME:
		if(result == 4)//// 上局未结束，提示续局
		{
			dialog = CommDialog::creatTwoBtnOneMessage("yes", "no", reason.c_str(),callfuncO_selector(MsgGlobalFilter::parseEnterGameRootMsg),NULL);
			WWSceneManager::getInstance()->openGlobalUI(dialog);
		}
		break;
	case MSG_GENNAL_OTHER_LOGIN:
		dialog = CommDialog::creatTwoBtnOneMessage("login", "leave", reason.c_str(),NULL,NULL);
		WWSceneManager::getInstance()->openGlobalUI(dialog);
		break;
	default:
		break;
	}*/


    //Color4B color(0, 0, 0, 200);
    //auto layer = DoubleLoginLayer::create(color);
    //layer->setTag(WWSceneManager::eSecondLayerTag_doubleLogin);
    //WWSceneManager::getInstance()->openGlobalUI(layer);
    //
    ////20150323 wildcat
    //MsgLogonRequest::sendLogonOut();
    //LordNetManger::getInstance()->onDisconnect(nullptr);
    
	return true;
}
void MsgGlobalFilter::parseEnterGameRootMsg(cocos2d::Ref* data)
{
	log("MsgGlobalFilter::parseEnterGameRootMsg-----------");
	//发送恢复对局消息
	GameGlobalDataManager::getInstance()->setStartGameType(StartGameType::SEND_RESUME);
	//WWSceneManager::getInstance()->changeToRoom();
	GameLogicManager::getInstance()->requestBackGame();
}

Ref* MsgGlobalFilter::parseGameUserBasicInfo()
{
	//SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();
	//UserBasicInfoData* userData = UserBasicInfoData::create();
	//
	//userData->setUserID(pClientSocket->ReadInt());//(int4)用户ID
	//userData->setGameId(pClientSocket->ReadInt());//(int4)游戏ID
	//userData->setNickname(pClientSocket->ReadString());//(String)昵称  
	//userData->setGender(pClientSocket->ReadChar());//(int1)性别1-男，2－女-------------

	//userData->setVipGrade(pClientSocket->ReadChar());//(int1)会员级别.0=非会员

	//userData->setLevel(pClientSocket->ReadShort());//(int2)等级----------

	//userData->setGameCash(pClientSocket->ReadString());//(String)金币数  
	//userData->setGameCash2(pClientSocket->ReadString());//(String)钻石数 ------------
	//userData->setRegisterTime(pClientSocket->ReadString());//(String)注册时间
	//userData->setLastlogonTime(pClientSocket->ReadString());//(String)上次登录时间
	//userData->setAPN(pClientSocket->ReadString());//(String)网络接入
	//userData->setIconID(pClientSocket->ReadInt());//(int4)用户头像ID
	//WWDataManager::getInstance()->setUserBasicInfoData(userData);
	//log("MsgGlobalFilter::parseGameUserBasicInfo-----------UserID = %d, GameID = %d, Nickname = %s,Gander = %d, VipGrade = %d,Level=%d, GameCash = %s,GameCash2=%s, RegisterTime = %s, LastTime = %s, APN = %s, IconID = %d"
	//	,userData->getUserID(),userData->getGameId(),userData->getNickname().c_str(),userData->getGender(),userData->getVipGrade(),userData->getLevel(),userData->getGameCash().c_str(),userData->getGameCash2().c_str(),userData->getRegisterTime().c_str(),userData->getLastlogonTime().c_str(), userData->getAPN().c_str() ,userData->getIconID());
	return NULL;
}

//Ref* MsgGlobalFilter::parseActivityNotify(WWBuffer *oBuffer)
//{
//    oBuffer->readChar();
//    std::string title = oBuffer->readLengthAndString();
//    std::string content = oBuffer->readLengthAndString();
//    int type = oBuffer->readChar();
//    
//    log("活动速递 : %s, %s", title.c_str(), content.c_str());
//    
//    if (type == 0)
//    {
//        WaWaActMessLayer *tip = WaWaActMessLayer::create(title, StringTool::removeRichString(content));
//        WWSceneManager::getInstance()->openGlobalUI(tip);
//    }
//    
//    return nullptr;
//}
//Ref* MsgGlobalFilter::parsePropUseToInfo(WWBuffer *oBuffer)
//{
//    PropUseInfoData* data = PropUseInfoData::create();
//    data->setGamePlayID(oBuffer->readInt());
//    data->setPropId(oBuffer->readInt());
//    CCLOG("MsgGlobalFilter::parsePropUseToInfo prop id=%d",data->getPropId());
//    data->setFromUserID(oBuffer->readInt());
//    data->setUseToUserID(oBuffer->readInt());
//    data->setParam(oBuffer->readLengthAndString());
//    return data;
//}
//
//Ref* MsgGlobalFilter::parseGameUserInfo(WWBuffer *oBuffer)
//{
//    log("CALL MsgLordGameFilter::parseGameUserInfo()");
//	GameInfoData* data = GameInfoData::create();
//	data->setUserID(oBuffer->readInt()) ;// ID
//	data->setNickName(oBuffer->readLengthAndString());// 昵称
//	data->setIconID(oBuffer->readInt());//头像ID
//	data->setImageFile(oBuffer->readLengthAndString());//用户形象
//	data->setVip(oBuffer->readChar());//VIP
//	data->setGrade(oBuffer->readUShort());//等级
//	data->setCash(oBuffer->readInt());// 用户金币值
//	CCLOG("parseStartGame getPlayerCashs use cash = %d", data->getCash());
//	data->setGender(oBuffer->readChar());//男女
//	data->setCharm(oBuffer->readInt());//魅力
//	data->setLoverID(oBuffer->readInt());//爱人ID
//	data->setLoverNickName(oBuffer->readLengthAndString());//爱人昵称
//	data->setTongName(oBuffer->readLengthAndString());//帮会
//	data->setTeam(oBuffer->readLengthAndString());//战队
//	data->setBirthDay(oBuffer->readLengthAndString());//生日
//	data->setConstellation(oBuffer->readLengthAndString());//星座
//	data->setProvince(oBuffer->readInt());//地区
//	data->setRegisterDate(oBuffer->readLengthAndString());//注册日期
//	data->setSign(oBuffer->readLengthAndString());//个性签名
//	data->setPetID(oBuffer->readInt());//宠物id
//	data->setGameID(oBuffer->readInt());//游戏id
//	data->setAllGame( oBuffer->readInt());//总局数
//	data->setWinPercent(oBuffer->readChar());//胜率
//	data->setTerminate(oBuffer->readChar());//断线率
//	data->setEscape(oBuffer->readChar());//逃跑率
//	data->setGamePoint(oBuffer->readInt());//游戏积分
//	data->setGameLevel(oBuffer->readUShort());//积分等级
//	data->setGameLevelName(oBuffer->readLengthAndString());	//称号
//	data->setWinGame(oBuffer->readInt());//胜利总局数
//	data->setMatchHonor(oBuffer->readLengthAndString());//比赛荣誉
//	data->setBean(oBuffer->readInt());	//蛙豆
//	CCLOG("parseStartGame getPlayerCashs use bean = %d", data->getBean());
//	data->setSSCount(oBuffer->readUShort());//服务订阅
//    
//    CCLOG("gamePoint:%d", data->getGamePoint());
//    CCLOG("gamePoint:%d", data->getGameLevel());
//    CCLOG("gamePoint:%s", data->getGameLevelName().c_str());
//    
//	for(int i=0;i<data->getSSCount();i++)
//	{
//		data->getSCodes().push_back(oBuffer->readInt());   //服务代码
//		data->getSNames().push_back(oBuffer->readLengthAndString());  //服务名称
//		data->getEndtimes().push_back(oBuffer->readLengthAndString());  //服务到期时间
//	}
//	data->setGameCash(oBuffer->readLengthAndString());//银子
//	data->setMJFan(oBuffer->readInt());  //用户最大总番数
//	data->setNickName2(oBuffer->readLengthAndString()); //合作方昵称
//	data->setMaxKeepWin(oBuffer->readUShort());//最大连胜数
//	data->setKeepWin(oBuffer->readUShort()) ;  //当前连胜数
//	CCLOG("parseGameInfo: keepwin=%d",data->getKeepWin());
//	data->setRockNum(oBuffer->readInt());//晶石数
//	CCLOG("parseGameInfo: rocknum=%d",data->getRockNum());
//	data->setPlayType(oBuffer->readUShort());//游戏玩法
//	CCLOG("parseGameInfo:%d",data->getPlayType());
//    oBuffer->readUShort(); //最大番类型
//    oBuffer->readLengthAndString();  //最大番名称
//    oBuffer->readLengthAndString();
//    oBuffer->readLengthAndString();
//    oBuffer->readLengthAndString();
//	data->setRegionStr(oBuffer->readLengthAndString());//地理位置
//	CCLOG("parseGameInfo location:%s",data->getRegionStr().c_str());
//
//	//保存用户称号到 GameContext 中 Players
//    if (GameContext::getInstance()->getPlayerById(data->getUserID()) != nullptr)
//    {
//        GameContext::getInstance()->getPlayerById(data->getUserID())->setPlayerLevelName(data->getGameLevelName());
//        GameContext::getInstance()->getPlayerById(data->getUserID())->setPlayerWaDou(data->getBean());
//        GameContext::getInstance()->getPlayerById(data->getUserID())->setWinPercent(data->getWinPercent());
//        GameContext::getInstance()->getPlayerById(data->getUserID())->setWinCount(data->getWinGame());
//		GameContext::getInstance()->getPlayerById(data->getUserID())->setPlayCount(data->getAllGame());
//        GameContext::getInstance()->getPlayerById(data->getUserID())->setGamePoints(data->getGamePoint());
//		GameContext::getInstance()->getPlayerById(data->getUserID())->setLocation(data->getRegionStr());
//		GameManager::getInstance()->updatePlayersInfo(data->getUserID());
//    }
//    
//    //恢复对局时、玩家数据接收完毕后才能进入房间。这里也可能会导致一个问题，当恢复对局后，快速点击用户资料框，可能会有获取不到数据的情况，标记一下，以后解决
//    //if (GameContext::getInstance()->getGameStartData() != nullptr && GameContext::getInstance()->getGameStartData()->getResume() == true)
//    //{
//    //    nReceivePlayersInfoCount ++;
//    //    if (nReceivePlayersInfoCount == 3)
//    //    {
//    //        //启动游戏场景
//    //        GameScene* _pGameScene = GameScene::create();
//    //        WWSceneManager::getInstance()->openScene(_pGameScene);
//    //        nReceivePlayersInfoCount = 0;
//    //    }
//    //}
//    
//	return data;
//}


//Ref* MsgGlobalFilter::parsePropIssue(WWBuffer *oBuffer)
//{
//	PropIssueData* data = PropIssueData::create();
//	data->setUserID(oBuffer->readInt());
//	data->setGameCash(oBuffer->readLengthAndString());
//	data->setIssueType(oBuffer->readInt());
//	data->setNotify(oBuffer->readLengthAndString());
//	data->setCount(oBuffer->readShort());
//	CCLOG("parsePropIssue:count=%d", data->getCount());
//    
//	for (int i = 0; i < data->getCount(); ++i)
//	{
//		data->getMagicName().push_back(oBuffer->readLengthAndString());
//		CCLOG("parsePropIssue:MagicName=%s", data->getMagicName().at(i).c_str());
//		data->getMagicID().push_back(oBuffer->readInt());
//		CCLOG("parsePropIssue:MagicID=%d", data->getMagicID().at(i));
//		data->getMagicCount().push_back(oBuffer->readInt());
//		CCLOG("parsePropIssue:MagicCount=%d", data->getMagicCount().at(i));
//		data->getMagicUnit().push_back(oBuffer->readLengthAndString());
//		CCLOG("parsePropIssue:MagicUnit=%s", data->getMagicUnit().at(i).c_str());
//		data->getMagicFID().push_back(oBuffer->readInt());
//		CCLOG("parsePropIssue:MagicFID=%d", data->getMagicFID().at(i));
//		data->getMagicFunType().push_back(oBuffer->readShort());
//		CCLOG("parsePropIssue:MagicFunType=%d", data->getMagicFunType().at(i));
//
//	}
//	CCLOG("parsePropIssue:IssueType=%d,gameCash=%s", data->getIssueType(), data->getGameCash().c_str());
//	int userBean = UserData::getInstance()->getIntValue(INT_USER_BEAN);
//    
//	switch (data->getIssueType())
//	{
//		//首充礼包
//	case 1000:
//		{
//			CCLOG("首充礼包 物品");
//			DataCenter::getInstance()->setChargeData(data);
//		}
//		break;
//        //新手礼包
//    case 1001:
//        {
//            CCLOG("新手礼包：data->getGameCash=%s", data->getGameCash().c_str());
//            if (data->getGameCash() != "0")
//            {
//                CCLOG("新手礼包");
//                UserData::getInstance()->setStringValue(STR_NEW_PLAYER_GIFT_CASH, data->getGameCash());
//                
//                std::string beanStr = data->getGameCash();
//                //            UserData::getInstance()->setIntValue(INT_USER_BEAN, atoi(beanStr.c_str()) + userBean);
//                UserData::getInstance()->setIsNewplayerGift(true);
//                //printf("UserData::getInstance()->setIsNewplayerGift(true)");
//                //add on 20150106 wildcat
//                int bean = atoi(beanStr.c_str());
//                //            if (HallManager::getInstance()->getHallScene())
//                //            {
//                //                HallManager::getInstance()->showNewplayerGift(bean);
//                //            }
//                //            else
//                //            {
//                //                HallManager::getInstance()->setNewPlayerGiftDelay(true, bean);
//                //            }
//                HallManager::getInstance()->setNewPlayerGiftDelay(true, bean);
//            }
//        }
//
//        break;
//        //签到
//    case 1002:
//        {
//            CCLOG("签到状态");
//            //            CC_ASSERT(data->getGameCash()!="0");
//            if (data->getGameCash() == "0" && data->getGameCash() == "")
//            {
//                break;
//            }
//            else
//            {
//                UserData::getInstance()->setIntValue(INT_LOGON_DAY_COUNT, UserData::getInstance()->getIntValue(INT_LOGON_DAY_COUNT) + 1);
//                
//                //保存蛙豆
//                std::string beanStr = StringUtils::format("%s", data->getGameCash().c_str());
//                UserData::getInstance()->setIntValue(INT_USER_BEAN, atoi(beanStr.c_str()) + userBean);
//                
//                
//                //            //条件
//                //            std::string giftBean = UserData::getInstance()->getString(STR_NEW_PLAYER_GIFT_CASH);
//                //            if (giftBean == "" && DataCenter::getInstance()->nowWhichScene == e_scene_hall && UserData::getInstance()->getIntValue(INT_LOGON_DAY_COUNT) > 0)
//                //            {
//                ////                    WWSceneManager::getInstance()->openGlobalUI(SignDialog::create());
//                ////                    Color4B color(0, 0, 0, 100);
//                ////                    WWSceneManager::getInstance()->openGlobalUI(SignLayer::create(color));
//                //
//                //                //签到状态
//                //
//                //                HallManager::getInstance()->signLayerStatus(true);
//                //                UserData::getInstance()->saveSignData();
//                //            }
//                
//                //条件
//                if (HallManager::getInstance()->getSignLayer())
//                {
//                    //                    WWSceneManager::getInstance()->openGlobalUI(SignDialog::create());
//                    //                    Color4B color(0, 0, 0, 100);
//                    //                    WWSceneManager::getInstance()->openGlobalUI(SignLayer::create(color));
//                    
//                    //签到状态
//                    
//                    HallManager::getInstance()->signLayerStatus(true);
//                    UserData::getInstance()->saveSignData();
//                }
//                
//            }
//        }
//
//        break;
//        //更新
//    case 1003:
//    {
//        log("CALL MsgGlobalFilter::parsePropIssue()->更新");
//    }
//        break;
//        //在线宝箱
//    case 1101:
//        break;
//    default:
//        break;
//	}
//	//分发充值刷新蛙豆事件
//	EventCustom event(EVENT_REFRESH_MONEY);
//	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
//	return data;
//}
//
//Ref* MsgGlobalFilter::parseLogonAwardInfo(WWBuffer *oBuffer)
//{
//	LogonAwardInfoData* data = LogonAwardInfoData::create();
//	data->setDayCount(oBuffer->readInt());
//	data->setAwardCount(oBuffer->readShort());
//	data->setLastTime(oBuffer->readLengthAndString());
//	data->setCount(oBuffer->readShort());
//	data->getDay().resize(data->getCount());
//	data->getGameCash().resize(data->getCount());
//	data->getCrytalStone().resize(data->getCount());
//    
//    CCLOG("awardCount:%d", data->getAwardCount());
//    
//	//本地保存服务器返回的连续签到天数
//    //
//    UserData::getInstance()->setSignDay(data->getDayCount());
//    UserData::getInstance()->setSignDate(data->getLastTime());
//    //
////	UserData::getInstance()->setIntValue(INT_LOGON_DAY_COUNT, data->getDayCount());
//	log("MsgGlobalFilter::parseLogonAwardInfo count = %d", data->getDayCount());
//    CCLOG("date->getLastTime:%s", data->getLastTime().c_str());
//    
//
//	for (int i = 0; i < data->getCount(); ++i)
//	{
//		data->getDay()[i] = oBuffer->readChar();
//		data->getGameCash()[i] = oBuffer->readInt();
//        
//		CCLOG("parseLogonAwardInfo:day=%d, gameCash=%d",data->getDay()[i], data->getGameCash()[i]);
//	}
//    
//	for (int i = 0; i < data->getCount(); ++i)
//	{
//		data->getCrytalStone()[i] = oBuffer->readShort();
//	}
//    
////	int id = UserData::getInstance()->getIntValue(INT_USER_ID);
//    
//    bool isShow = false;
//
//	//如果签过，则比较日期，如今天签过，则不再次请求，如今天没签，则请求
//	if (data->getLastTime() != "")
//	{
//		if (data->getLastTime() != PhoneTool::getFormatDayTime())
//        {
//            //20150119  xuhuanqi
////            MsgCommonRequest::sendRequestInfo(id, 59, 1001, 10010003);
//
//            isShow = true;
//        }
//        
//	}
//	//如果没签过，直接请求签到
//	else
//	{
//        //20150119  xuhuanqi
////		MsgCommonRequest::sendRequestInfo(id, 59, 1001, 10010003); // 登录签到
//
//        isShow = true;
//	}
//    
//    
//	DataCenter::getInstance()->setlogonAwardInfoData(data);
//    
//    if (UserData::getInstance()->getIsFirstHere())
//    {
//        UserData::getInstance()->setIsFirstHere(false);
//        isShow = false;
//    }
//
////    if (isShow)
////    {
////        //弹框 20150119  xuhuanqi
////        HallManager::getInstance()->showSignLayer(0);
////    }
//    UserData::getInstance()->setIsShowSignLayer(isShow);
//    UserData::getInstance()->setMessageReceive(UserData::eMessageIndex_sign, isShow);
//    
//	return data;
//    
//}
//
//Ref* MsgGlobalFilter::parseMediaContentInfo(WWBuffer *oBuffer)
//{
//    CCLOG("MsgGlobalFilter::parseMediaContentInfo:oBuffer->length=%d", oBuffer->length());
//    //imageID
////    std::string text = StringUtils::format("media:oBuffer->length=%d", oBuffer->length());
////    auto toast = WWToast::createToast(text, 1.5);
////    Director::getInstance()->getRunningScene()->addChild(toast);
//    
//	bool isOtherHeadDataNull = false;
//	MediaContentInfoData* data = MediaContentInfoData::create();
//	data->setType(oBuffer->readChar());
//	data->setCount(oBuffer->readUShort());
//    
//	int count = data->getCount();
//	data->getImageIds().resize(count);
//	data->getMediaType().resize(count);
//	data->getPrices().resize(count);
//	data->getParameter2s().resize(count);
//    
//    CCLOG("MsgGlobalFilter::parseMediaContentInfo:type=%d, count=%d", data->getType(), data->getCount());
//    
//	if (data->mediaContents.size() != 0)
//	{
//		data->mediaContents.clear();
//	}
//    
//	for (int i = 0; i < count; i++)
//	{
//		data->getImageIds()[i] = oBuffer->readInt();
//		data->getMediaType()[i] = oBuffer->readChar();
//        
//		WWBuffer *buf = oBuffer->readLengthAndData();
//		if (buf == nullptr)
//		{
//			CCLOG("CALL MsgGlobalFilter::parseMediaContentInfo() buf is null");
//			isOtherHeadDataNull = true;
//		}
//		else
//		{
//			data->mediaContents.pushBack(buf);
//		}
//        
//		data->getPrices()[i] = oBuffer->readInt();
//	}
//    
//	data->setParameter(oBuffer->readInt());
//	data->setImageSize(oBuffer->readChar());
//	for (int i = 0; i < count; i++)
//	{
//		data->getParameter2s()[i] = oBuffer->readLengthAndString();
//	}
//	//解析完毕
//    
//	//如果不是在房间中发起头像请求
//    if (DataCenter::getInstance()->getMediaRequestType() == DataCenter::eMediaRequestType_hallScence)
//	{
//        
//        //on 20150326 wildcat
////        if (data->mediaContents.size() >  0)
////        {
////            MediaData * media = MediaData::create();
////            
////            int userID = UserData::getInstance()->getUserID();
////            
////            CCLOG("1) MsgGlobalFilter:useID=%d", userID);
////            
////            WWBuffer * buffer = data->mediaContents.at(0);
////            CCLOG("buffer.length = %d", buffer->length());
////            if (buffer->length() > 0)
////            {
////                media->setUserID(userID);
////                auto mediaData = new Data();
////                mediaData->copy((unsigned char *)buffer->data(), buffer->length());
////                media->setMediaBuffer(mediaData);
////                
////                DataCenter::getInstance()->setMediaContent(media);
////            }
////        }
//        
//		//保存用户头像数据到 DataCenter
//		if (DataCenter::getInstance()->mediaContents.size() != 0)
//		{
//			DataCenter::getInstance()->mediaContents.clear();
//		}
//        
//		for (int i = 0; i < count; i++)
//		{
//			if (data->mediaContents.size() > 0) //如果没有头像，DataCenter::getInstance()->MediaContents 则为空
//			{
//				DataCenter::getInstance()->mediaContents.pushBack(data->mediaContents.at(0));
//                
////				if (HallManager::getInstance()->getUserInfoLayer() != nullptr)
////				{
////					log("头像数据返回，个人信息层不会空");
////					//刷新头像
////					HallManager::getInstance()->getUserInfoLayer()->flashHead();
////				}
//                WW_LOG_DEBUG("MsgGlobalFilter::parseMediaContentInfo:data->mediaContents.size() = %d", data->mediaContents.size());
//                HallManager::getInstance()->updateUserInfo(HallManager::eUpdateType_headIcon);
//			}
//		}
//
//	}
//    else if (DataCenter::getInstance()->getMediaRequestType() == DataCenter::eMediaRequestType_leaderboard)
//    {
//        CCLOG("getMediaRequestType() == DataCenter::eMediaRequestType_leaderboard");
//        
//        if (data->mediaContents.size() >  0)
//        {
//            MediaData * media = MediaData::create();
//            
//            int userID = data->getImageIds()[0];
//            
//            CCLOG("MsgGlobalFilter:useID=%d", userID);
//            
//            WWBuffer * buffer = data->mediaContents.at(0);
//            if (buffer->length() > 0)
//            {
//                media->setUserID(userID);
//                auto mediaData = new Data();
//                mediaData->copy((unsigned char *)buffer->data(), buffer->length());
//                media->setMediaBuffer(mediaData);
//                
//                DataCenter::getInstance()->setMediaContent(media);
//            }
//        }
//
//    }
//    else if (DataCenter::getInstance()->getMediaRequestType() == DataCenter::eMediaRequestType_gameScence)//在房间中发起头像请求
//	{
//		if (!isOtherHeadDataNull)
//		{
//			//本家头像ID
//			int iconId0 = GameContext::getInstance()->getPlayers().at(0)->getHeadImgId();
//			//下家头像ID
//			int iconId1 = GameContext::getInstance()->getPlayers().at(1)->getHeadImgId();
//			//上家头像ID
//			int iconId2 = GameContext::getInstance()->getPlayers().at(2)->getHeadImgId();
//			CCLOG("CALL MsgGlobalFilter::parseMediaContentInfo() iconid1 :%d, iconid2 :%d, iconId0 :%d, data.imageID=%d", iconId1, iconId2, iconId0, data->getImageIds()[0]);
//            
//			if (data->getImageIds()[0] == iconId0 || (iconId0 == 101 && data->getImageIds()[0] == GameContext::getInstance()->getPlayers().at(kSiteSelf)->getUserid()))
//			{
//				//保存用户头像数据到 DataCenter
//				if (DataCenter::getInstance()->mediaContents.size() != 0)
//				{
//					DataCenter::getInstance()->mediaContents.clear();
//				}
//                
//				for (int i = 0; i < count; i++)
//				{
//					if (data->mediaContents.size() > 0) //如果没有头像，DataCenter::getInstance()->MediaContents 则为空
//					{
//						DataCenter::getInstance()->mediaContents.pushBack(data->mediaContents.at(0));
//						GameManager::getInstance()->handleOtherPlayersMediaContent(kSiteSelf);
//					}
//				}
//			}
//			if (data->getImageIds()[0] == iconId1 || (iconId1 == 101 && data->getImageIds()[0] == GameContext::getInstance()->getPlayers().at(kSiteRight)->getUserid()))
//			{
//				DataCenter::getInstance()->setRightPlayerMedia(data->mediaContents.at(0));
//				GameManager::getInstance()->handleOtherPlayersMediaContent(kSiteRight);
//			}
//			if (data->getImageIds()[0] == iconId2 || (iconId2 == 101 && data->getImageIds()[0] == GameContext::getInstance()->getPlayers().at(kSiteLeft)->getUserid()))
//			{
//				DataCenter::getInstance()->setLeftPlayerMedia(data->mediaContents.at(0));
//				GameManager::getInstance()->handleOtherPlayersMediaContent(kSiteLeft);
//			}
//		}
//	}
//    
//	return data;
//}
//
//恢复房间基本信息
Ref* MsgGlobalFilter::parseRecoveryGameNofity()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

	RecoveryGameNotifyData *data = RecoveryGameNotifyData::create();
	data->setGameID(pClientSocket->ReadInt());
	data->setGameZoneID(pClientSocket->ReadInt());
	data->setGamePlayID(pClientSocket->ReadInt());
	data->setPlayType(pClientSocket->ReadShort());
	data->setZoneWin(pClientSocket->ReadChar());
	data->setFortuneBase(pClientSocket->ReadInt());
	data->setPointBase(pClientSocket->ReadInt());
	data->setMultiple(pClientSocket->ReadShort());
	data->setMoneyBox(pClientSocket->ReadChar());
	data->setFortuneMin(pClientSocket->ReadInt());
	data->setFortuneMax(pClientSocket->ReadInt());
	data->setRoomName(pClientSocket->ReadString());

	CCLOG("恢复对局倍数 %d, %d, %d, name =%s", data->getMultiple(), data->getFortuneBase(), data->getPointBase(), data->getRoomName().c_str());

	//玩法
	GameRoom *room = GameRoom::create();
	room->setPlayType(data->getPlayType());
	room->setFortuneBase(data->getFortuneBase());
	room->setGameId(data->getGameID());
	room->setGZID(data->getGameZoneID());
	room->setMultiple(data->getMultiple());
	//room->setFortuneMin(data->getFortuneMin());
	room->setName(data->getRoomName());
	int nLevel = UserData::getInstance()->getIntValue(key_star_level);
	if (nLevel == 0)
	{
		nLevel = 1;
	}
	//room->setStars(nLevel);
    
	//GameContext::getInstance()->setGameZoneId(data->getGameZoneID());
	//DataCenter::getInstance()->setGameRoom(room);
	
	// just for test.
	//room->retain();
	WWDataManager::getInstance()->setGameRoom(room);

	//TODO 进去游戏界面
	//WWSceneManager::getInstance()->setEnterGameType(StartGameType::SEND_RESUME);
	//WWSceneManager::getInstance()->changeToRoom();

    
	return data;
}

//恢复对局详细数据
Ref* MsgGlobalFilter::parseResumeGame()
{
	SocketClient* pClientSocket = WWNetWorkManager::getInstance()->getCurSocket();

    //恢复对局倍数
    int ResumeGameMuti = 1;
    
	GameStartData* data = GameStartData::create();
	data->setResume(true);
	data->setGameId(WWDataManager::getInstance()->getGameRoom()->getGameId());
	data->setFortuneBase(WWDataManager::getInstance()->getGameRoom()->getFortuneBase());
	data->setGamePlayId(pClientSocket->ReadInt()); //对局标示
	CCLOG("MsgGlobalFilter::parseResumeGame----gamePlayId = %d",data->getGamePlayId());
	data->setPlayTimeOut(pClientSocket->ReadShort()); //单手牌超时时间
	CCLOG("MsgGlobalFilter::parseResumeGame----PlayTimeOut = %d",data->getPlayTimeOut());
	data->setNextPlayUserID(pClientSocket->ReadInt()); //下一出牌玩家
	CCLOG("MsgGlobalFilter::parseResumeGame----NextPlayUserID = %d",data->getNextPlayUserID());
	data->setLordId(pClientSocket->ReadInt()); //地主ID
	CCLOG("MsgGlobalFilter::parseResumeGame----LordId = %d",data->getLordId());
	std::string baseCards = pClientSocket->ReadString();//(byte[])底牌
	CCLOG("MsgGlobalFilter::parseResumeGame----baseCards = %s",baseCards.c_str());
	//Vector<PokerObject*> arrays = PokerUtil::getSeverData((uint8_t*) str.c_str(), str.size());
	data->setBasePokers(baseCards); //底牌
	data->setBankScore(pClientSocket->ReadChar()); //游戏底分
	CCLOG("MsgGlobalFilter::parseResumeGame----BankScore = %d",data->getBankScore());
	data->setGameStatus(pClientSocket->ReadChar()); //(int1)1叫地主，2抢地主，3加倍，4明牌  5打牌 6结算
	CCLOG("MsgGlobalFilter::parseResumeGame----GameStatus = %d",data->getGameStatus());
	data->setLastPlayerID(pClientSocket->ReadInt()); //最后出牌玩家
	CCLOG("MsgGlobalFilter::parseResumeGame----LastPlayerID = %d",data->getLastPlayerID());
    
	std::string lastCards = pClientSocket->ReadString();
	CCLOG("MsgGlobalFilter::parseResumeGame----lastCards = %s",lastCards.c_str());
	//arrays = PokerUtil::getSeverData((uint8_t*) str.c_str(), str.size());
	data->setLastPokers(lastCards); //最后出牌
    
	data->setBoomCount(pClientSocket->ReadChar()); //炸弹数量
	CCLOG("MsgGlobalFilter::parseResumeGame----BoomCount = %d",data->getBoomCount());
	data->setLaizi(pClientSocket->ReadChar()); //赖子: 有效值1-13,依次表示345678910JQKA2
	CCLOG("MsgGlobalFilter::parseResumeGame----Laizi = %d",data->getLaizi());

	data->setTaskNum4Award(pClientSocket->ReadShort()); //奖宝箱局数
	CCLOG("MsgGlobalFilter::parseResumeGame----TaskNum4Award = %d",data->getTaskNum4Award());
	data->setPlayedNum4Award(pClientSocket->ReadShort()); //当前财富局数
	CCLOG("MsgGlobalFilter::parseResumeGame----PlayedNum4Award = %d",data->getPlayedNum4Award());
	//data->setTaskEachNumAward(data->getTaskNum4Award());
	data->setMingPai(pClientSocket->ReadChar() == 1); //明牌标志.1-明牌,0不明牌,2未确定
	CCLOG("MsgGlobalFilter::parseResumeGame----MingPai = %d",data->getMingPai());
	data->setPlayerCount(pClientSocket->ReadChar()); //游戏人数
    CCLOG("MsgGlobalFilter::parseResumeGame----PlayerCount = %d",data->getPlayerCount());
    //明牌翻倍
    if (data->getMingPai() == 1)
    {
        ResumeGameMuti = ResumeGameMuti * 2;
    }
    //炸弹翻倍
    if (data->getBoomCount() != 0)
    {
		ResumeGameMuti = ResumeGameMuti * data->getBoomCount() * 2;
    }
    
	for (int i = 0; i < data->getPlayerCount(); ++i)
	{
		data->getPlayerIds().push_back(pClientSocket->ReadInt()); //用户ＩＤ
		CCLOG("MsgGlobalFilter::parseResumeGame----PlayerId = %d",data->getPlayerIds().at(i));
		data->getPlayerNickNames().push_back(pClientSocket->ReadString()); //用户名称
		data->getPlayerPokers().push_back(pClientSocket->ReadString()); //用户的牌
		data->getPlayerIcons().push_back(pClientSocket->ReadInt()); //用户头像
		data->getPlayerVips().push_back(pClientSocket->ReadChar()); //ＶＩＰ
		data->getJiFenLevel().push_back(pClientSocket->ReadShort()); //游戏积分等级
		data->getGenders().push_back(pClientSocket->ReadChar()); //性别
		data->getOnlineTypes().push_back(pClientSocket->ReadString()); //网络接入
		data->getQiangDizhus().push_back(pClientSocket->ReadChar() == 1); //抢地主标志：1－抢，0－不抢

		int doubleScore = pClientSocket->ReadChar();//(int1)加倍标志 0 未 1加倍 2 强加被

		data->getForbidTalk().push_back(pClientSocket->ReadChar() == 1); //禁言标志:1-禁言,0-不禁

		int keepWin = pClientSocket->ReadShort();//连胜数

		int recordCardFlag = pClientSocket->ReadChar();//(int1) 记牌器状态 0：未使用记牌器  1：已使用记牌器
        
        //抢地主翻倍
        if (data->getQiangDizhus()[i] == 1)
        {
			CCLOG("wly::QiangDizhus (%d) muti", data->getQiangDizhus()[i]);
            ResumeGameMuti = ResumeGameMuti * 2;
        }
        
		//重连缺少的数据 -1表示无数据
		data->getPlayerCashs().push_back(-1);
		data->getJiFenLevel().push_back(-1);
	}

	//just for test.
	data->retain();

	WWDataManager::getInstance()->setGameStartData(data);

	if(GameGlobalDataManager::getInstance()->getCurSceneType() != GameSceneType::ROOM_SCENE)//不在游戏界面收到恢复对局消息
	{
		GameGlobalDataManager::getInstance()->setStartGameType(StartGameType::NO_SEND);

		//记录当前玩法
		int roomType = WWDataManager::getInstance()->getGameRoom()->getPlayType();
		GameLogicManager::getInstance()->setCurRoomType(roomType);
		
		//记录当前底注
		int roomBase = WWDataManager::getInstance()->getGameRoom()->getFortuneBase();
		GameLogicManager::getInstance()->setCurRoomBase(roomBase);
		
		auto *pScene = GameBasicScene::create();
		Director::getInstance()->replaceScene(pScene);

	}
	else
	{
		GameLogicManager::getInstance()->setMultiple(ResumeGameMuti);
		GameLogicManager::getInstance()->ProcessResumeGame(data);
	}

	//WWSceneManager::getInstance()->setEnterGameType(StartGameType::SEND_RESUME);
	//WWSceneManager::getInstance()->changeToRoom();

	//------------------------------------------------------------end-------------------------------------------------------------------------------------------------
    
	//for (int i = 0; i < data->getPlayerCount(); ++i)
	//{
	//	data->getForbidTalk().push_back(pClientSocket->ReadChar() == 1); //禁言标志:1-禁言,0-不禁
	//}
	//int playCount2 = pClientSocket->ReadChar(); //游戏人数
	//for (int i = 0; i < playCount2; i++)
	//{
	//	pClientSocket->ReadString(); //多酷账号
	//}
	//int playCount3 = pClientSocket->ReadChar(); //游戏人数
	//for (int i = 0; i < playCount3; i++)
	//{
	//	data->getMembers().push_back(pClientSocket->ReadChar() == 1); //蓝晶会员信息
 //       
	//}
 //   
	//data->setHasBox(pClientSocket->ReadChar() == 1); //是否已有宝箱 0：没有 1：已有
	//data->setBoxSwitch(pClientSocket->ReadChar() == 1); //在线宝箱开关 0-关 1：开//33
	//data->setBoxId(pClientSocket->ReadInt()); //在线宝箱ID
	//data->setBoxOpenTime(pClientSocket->ReadInt()); //开宝箱剩余时间(秒)
	//for (int i = 0; i < data->getPlayerCount(); i++)
	//{
	//	CCLOG("连胜数=%d", pClientSocket->ReadShort());
	//}

	//----------------------------------------------------------------------...................----------------------
	//for (int i = 0; i < data->getPlayerCount(); i++)
	//{
	//	data->getrecordCardFlag().push_back(pClientSocket->ReadChar() == 1);
	//	CCLOG("使用记牌器(%d)=%d",i, data->getrecordCardFlag().at(i));
	//}

	////红蓝卡身份标识
	//for (int i =0; i < data->getPlayerCount(); i ++)
	//{
	//	data->getColorVIP().push_back(oBuffer->readLengthAndString());
	//}
 //   
	////Sam 登陆断线重连 直接进入房间
	//GameContext::getInstance()->initPlayerData(data);
    
    //for (int i = 0; i < data->getPlayerCount(); ++i)
    //{
    //    //恢复对局玩家资料数据
    //    MsgCommonRequest::sendRequestInfo(data->getPlayerIds()[i], 11, DataCenter::getInstance()->getGameRoom()->getGameID(), DataCenter::getInstance()->getGameRoom()->getPlayType());
    //}
    
 //   DataCenter::getInstance()->m_resumeGameMulti = ResumeGameMuti;
 //   
 //   //	//启动游戏场景
	//if (DataCenter::getInstance()->nowWhichScene == e_scene_game)
	//{
	//	GameManager::getInstance()->handleGameRecoveryMsg();
	//}
	//else
	//{
 //       LoadingManager::getInstance()->endLoading();
	//	GameScene* _pGameScene = GameScene::create();
	//	WWSceneManager::getInstance()->openScene(_pGameScene);
	//}
    
	return data;
}
//
//
////排行榜
//cocos2d::Ref* MsgGlobalFilter::parseTopScore(WWBuffer *oBuffer)
//{
//    CCLOG("MsgHallDataFilter::parseTopScore");
//    LeaderboardData * data = LeaderboardData::create();
//    
//    data->setTopType(oBuffer->readInt());
//    data->setTimeStr(oBuffer->readLengthAndString());
//    data->setMyNo(oBuffer->readInt());
//    data->setMyScore(oBuffer->readLengthAndString());
//    data->setCount(oBuffer->readShort());
//    
//    int count = data->getCount();
//    CCLOG("TopType:%d, timeStr:%s, myNo:%d, myScore:%s, count:%d", data->getTopType(), data->getTimeStr().c_str(), data->getMyNo(), data->getMyScore().c_str(), data->getCount());
//        data->getvNo().resize(count);
//        data->getvUserID().resize(count);
//        data->getvNickname().resize(count);
//        data->getvProvince().resize(count);
//        data->getvScore().resize(count);
//    for (int i = 0; i < count; ++i)
//    {
//        data->getvNo()[i] = oBuffer->readInt();
//        data->getvUserID()[i] = oBuffer->readInt();
//        data->getvNickname()[i] = oBuffer->readLengthAndString();
//        data->getvProvince()[i] = oBuffer->readLengthAndString();
//        data->getvScore()[i] = oBuffer->readLengthAndString();
//        
//        CCLOG("i=%d, No=%d, nickname:%s, score:%s", i, data->getvNo()[i], data->getvNickname()[i].c_str(), data->getvScore()[i].c_str());
////        data->getvNo().push_back(oBuffer->readInt());
////        data->getvUserID().push_back(oBuffer->readInt());
////        data->getvNickname().push_back(oBuffer->readLengthAndString());
////        data->getvProvince().push_back(oBuffer->readLengthAndString());
////        data->getvScore().push_back(oBuffer->readLengthAndString());
//    }
//    
//    data->setIconCount(oBuffer->readShort());
//    int iconCount = data->getIconCount();
//    data->getvIconID().resize(iconCount);
//    data->getvIconTS().resize(iconCount);
//    CCLOG("iconCount:%d", iconCount);
//    for (int i = 0; i < iconCount; ++i)
//    {
//        data->getvIconID().push_back(oBuffer->readInt());
//        data->getvIconTS().push_back(oBuffer->readLengthAndString());
//    }
//    
//    return data;
//}
////物品箱列表解析
//cocos2d::Ref* MsgGlobalFilter::parseBoxInfoParam(WWBuffer* oBuffer)
//{
//    UserMatchEquipList* data = UserMatchEquipList::create();
//    
//    data->setUserID(oBuffer->readInt());
//    data->setGameID(oBuffer->readInt());
//    data->setCount(oBuffer->readShort());
//    
//    
//    Vector<EquipInfo*>   vec;
//    for (int i=0; i<data->getCount(); i++)
//    {
//        auto equip = EquipInfo::create();
//        equip->setUserEquipID(oBuffer->readInt());
//        equip->setEquipID(oBuffer->readInt());
//        equip->setEquipCount(oBuffer->readInt());
//        equip->setName(oBuffer->readLengthAndString());
//        equip->setExpireTime(oBuffer->readLengthAndString());
//        equip->setFid(oBuffer->readInt());
//        vec.pushBack(equip);
//    }
//    //新增道具类型
//    for (auto equip:vec)
//    {
//        equip->setMagicType(oBuffer->readChar());
//    }
//    
//    data->setEquipVec(vec);
//    
//    DataCenter::getInstance()->setAllEquipList(data);
//    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(g_equipBoxList);
//    return data;
//}
////物品箱详细解析
//cocos2d::Ref* MsgGlobalFilter::parseEquipDetailInfoParam(WWBuffer* oBuffer)
//{
//    GameEquipInfo* data = GameEquipInfo::create();
//    data->setEquipID(oBuffer->readInt());
//    data->setDesc(oBuffer->readLengthAndString());
//    data->setintroduce(oBuffer->readLengthAndString());
//    data->setEquipName(oBuffer->readLengthAndString());
//    data->setEquipCount(oBuffer->readInt());
//    data->setcount(oBuffer->readShort());
//    data->setsubEquipCount(oBuffer->readInt());
//    data->setExpire(oBuffer->readLengthAndString());
//    auto equipVec = HallContext::getInstance()->getEquipInfoVec();
//    equipVec.pushBack(data);
//    HallContext::getInstance()->setEquipInfoVec(equipVec);
//    
//    
//    
//    return data;
//}
//
////开宝箱结果
//cocos2d::Ref* MsgGlobalFilter::parseOpenBoxResult(WWBuffer* oBuffer)
//{
//    auto data = BoxDataResp::create();
//    data->setMoneyBoxID(oBuffer->readInt());
//    data->setResult(oBuffer->readShort());
//    data->setMoney(oBuffer->readInt());
//    data->setcorrespondence(oBuffer->readInt());
//    data->setdesc(oBuffer->readLengthAndString());
//    data->setCount(oBuffer->readShort());
//    CCLOG("MsgGlobalFilter::parseOpenBoxResult magicCount=%d",data->getCount());
//    std::vector<std::string> nameVec;
//    std::vector<int> magicIDVec;
//    std::vector<int> magicCountVec;
//    std::vector<int> magicFidVec;
//
//    for (int i=0; i<data->getCount(); i++)
//    {
//        nameVec.push_back(oBuffer->readLengthAndString());
//        magicIDVec.push_back(oBuffer->readInt());
//        magicCountVec.push_back(oBuffer->readInt());
//        magicFidVec.push_back(oBuffer->readInt());
//        
//        CCLOG("MsgGlobalFilter::parseOpenBoxResult name==%s\n id=%d\n count=%d\n fid=%d",nameVec.at(i).c_str(),magicIDVec.at(i),magicCountVec.at(i),magicFidVec.at(i));
//    }
//    data->setMagicName(nameVec);
//    data->setMagicID(magicIDVec);
//    data->setMagicCount(magicCountVec);
//    data->setMagicFID(magicFidVec);
//    
////    if (data->getCount() == 0)
////    {
////        auto dialog = GeneralDialog::create("新手宝箱空空如也，一定是配置的有问题");
////        Director::getInstance()->getRunningScene()->addChild(dialog);
////    }
//    
//    return data;
//}
//
//Ref* MsgGlobalFilter::parseLogoutInfo(WWBuffer *oBuffer)
//{
//    LogonOutData* data = LogonOutData::create();
//    data->setUserId(oBuffer->readInt());//用户ID
//    data->setExitType(oBuffer->readChar());//退出方式:1 正常退出;2超时退出;3重复登录强制退出;4系统超时(挂线、空闲超时)
//    data->setPower(oBuffer->readInt());////改变的战斗力值
//    data->setLongevity(oBuffer->readInt());//改变的经验值
//    data->setCharm(oBuffer->readInt()); //改变的魅力值
//    data->setCash(oBuffer->readInt());//改变的财富值
//    data->setLogonId(oBuffer->readInt());//用户的登录ID
//    data->setOnlineTime(oBuffer->readUShort()) ;////在线时长(分钟)
//    data->setIsDoubleExp(oBuffer->readChar());//双倍经验值；0-没使用，1－使用
//    data->setBean(oBuffer->readInt()); //改变的蛙豆值
//    data->setGameCash(oBuffer->readLengthAndString());//改变的银子值
//    return data;
//}
//
//Ref* MsgGlobalFilter::parseResultInfo(WWBuffer *oBuffer)
//{
//    ResultInfoData *data = ResultInfoData::create();
//    data->setUserID(oBuffer->readInt());
//    data->setType(oBuffer->readChar());
//    data->setResult(oBuffer->readChar());
//    data->setDescription(oBuffer->readLengthAndString());
//    data->setParameter(oBuffer->readInt());
//    data->setParameter2(oBuffer->readLengthAndString());
//    data->setParameter3(oBuffer->readLengthAndString());
//    
//    return data;
//}


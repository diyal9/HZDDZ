#ifndef _MSG_CHARGE_FILTER_
#define _MSG_CHARGE_FILTER_

#include "cocos2d.h"
#include "BaseCore/WWNetDataBasic/WWIMsgFilter.h"

const int DIRECT_DEPOSITE_INFO = 0x640110;//快充消息返回
const int CHARGE_MENU_INFO = 0x640115;//充值中心消息返回

class MsgChargeFilter: public WWIMsgFilter, public cocos2d::Ref
{
public:
	MsgChargeFilter();
	virtual ~MsgChargeFilter();

	CREATE_FUNC(MsgChargeFilter);

	bool init();

	virtual void onDataReceiver(int msgType,cocos2d::Ref* objectData ){};
	virtual std::vector<int> onFilters();
	virtual bool onMessageReceived(int msgType);
	virtual std::vector<int> onRootFilters();
	virtual bool onRootMessageReceived(int msgType, int userId, int result,std::string reason, int reasonType);

	cocos2d::Ref* parseDirectDepositeInfo();//解析快充消息

	cocos2d::Ref* parseChargeMenuInfo();//充值中心菜单信息

};


#endif // !_MSG_CHARGE_FILTER_


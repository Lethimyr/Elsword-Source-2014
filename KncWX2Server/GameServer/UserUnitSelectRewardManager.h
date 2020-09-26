#pragma once
#include "Event.h"
#include "CommonPacket.h"

//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY

class KUserUnitSelectRewardManager
{
public:

	KUserUnitSelectRewardManager(void);
	~KUserUnitSelectRewardManager(void);

	void Clear();
	void AddRewardItemInfo( IN const int iAddRewardID );
	void GetRewardAndClear( OUT std::vector< int >& vecRewardInfo );


	//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	void SetFirstSelectUnitRewardInfo( IN std::vector<KFirstSelectUnitReward>& vecInfo );
	bool GetFirstSelectUnitRewardInfo( IN int iServerGroupID, OUT std::vector<KFirstSelectUnitReward>& vecInfo );
	void UpdateFirstSelectUnitRewardInfo( IN int iDBIndexID, UidType iUnitUID );
	void DeleteAndGetFirstSelectUnitRewardInfo( IN int iDBIndexID, KFirstSelectUnitReward& kInfo );
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}

private:
	std::vector< int >			m_vecAccessItem;			// 접속시 아이템 지급 - 보안 패드 사용 보상용

	//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	std::map<int, KFirstSelectUnitReward>			m_mapFirstSelectUnitRewardInfo;
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}
};


#endif SERV_SECOND_SECURITY
//}}
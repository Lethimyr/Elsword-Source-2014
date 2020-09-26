#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <set>
#include <boost/timer.hpp>
#include "KncSend.h"


//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT


class KRandomItemEventManager
{
	DeclareSingleton( KRandomItemEventManager );
	DeclDump;
	DeclToStringW;

	struct SRandomItemEvent
	{
		int								m_iID;
		int								m_iItemID;
		CTime							m_tDropTime;
		//{{ 2011. 07. 04    김민성    네이버 프로모션
#ifdef SERV_NAVER_PROMOTIONS
		CTime							m_tDropEndTime;
#endif SERV_NAVER_PROMOTIONS
		//}} 
		//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
		int								m_iOpenItemID;
#endif SERV_RANDOM_CUBE_GOLD_REWARD
		//}}

		SRandomItemEvent()
		{
			m_iID			= 0;
			m_iItemID		= 0;
		}
	};

public:
	KRandomItemEventManager(void);
	~KRandomItemEventManager(void);

	void Init();
	void UpdateEventInfo( IN const std::vector< KTimeOpenRandomItemEventInfo >& vecEventInfo, IN const std::set< UidType >& setGetRewardUserUIDList );
	//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
	void CheckTimeOpenRandomItemEvent( IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iRandomItemID, OUT int& iEventItemID );
#else
	void CheckTimeOpenRandomItemEvent( IN const UidType iUserUID, IN const UidType iUnitUID, OUT int& iEventItemID );
#endif SERV_RANDOM_CUBE_GOLD_REWARD
	//}}

protected:
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:
	//{{ 2011. 07. 04    김민성    네이버 프로모션
#ifdef SERV_NAVER_PROMOTIONS
	std::map< __int64, std::vector< SRandomItemEvent > >			m_mapEventInfo;
#else
	std::map< __int64, SRandomItemEvent >			m_mapEventInfo;
#endif SERV_NAVER_PROMOTIONS
	//}} 
	std::vector< KTimeOpenRandomItemEventDBUPdate >	m_vecReservedDBUpdate;
	std::set< UidType >								m_setGetRewardUserUIDList;
};

DefSingletonInline( KRandomItemEventManager );

template < class T >
void KRandomItemEventManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_LOG_DB, 0, NULL, usEventID, data );
}


#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}



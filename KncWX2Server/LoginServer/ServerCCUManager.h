#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <map>
#include <boost/timer.hpp>
#include "KncSend.h"


//////////////////////////////////////////////////////////////////////////
// Server CCU Manager : 동접 정보 관리
#ifdef SERV_CCU_NEW


class KServerCCUManager
{
	DeclareSingleton( KServerCCUManager );
	DeclDump;
	DeclToStringW;

	enum ENUM_TIMER
	{		
		TM_CCU_UPDATE = 0,
		//{{ 2012. 07. 10	최육사		대기열 시스템
//#ifdef SERV_WAIT_QUEUE_SYSTEM
		TM_FOR_WAIT_QUEUE,
//#endif SERV_WAIT_QUEUE_SYSTEM
		//}}

		TM_TOTAL_NUM,
	};

public:
	KServerCCUManager(void);
	~KServerCCUManager(void);

	void Tick();

	void DBUpdateCCUInfo();
	void UpdateCCUInfo( IN const KUpdateCCUInfo& kInfo );
#ifdef SERV_CHANNELING_USER_MANAGER
	void UpdateChannelingUserInfo( IN std::map<int,int> m_mapChannelingUserList , OUT KUpdateCCUInfo& kTotalCCUInfo );
#endif SERV_CHANNELING_USER_MANAGER

#ifdef SERV_COUNTRY_CN
	const std::map< UidType, KUpdateCCUInfo >&	GetLastCCUInfo() const		{ return m_mapLastCCUInfo; }
#endif SERV_COUNTRY_CN
protected:
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:
	std::map< UidType, KUpdateCCUInfo >		m_mapCCUInfo;
#ifdef SERV_COUNTRY_CN
	std::map< UidType, KUpdateCCUInfo >		m_mapLastCCUInfo;
	int                                     m_iUpdatedMinute;
#endif SERV_COUNTRY_CN

	boost::timer							m_kTimer[TM_TOTAL_NUM];
	//CTime									m_tNextUpdateTime;

	//{{ 2011.10.12 조효진 동접 정보 버그 수정 및 전체 동접 서버에서 남기도록 수정
#ifdef SERV_FIX_CCU_REPORT
	CTime									m_tLastUpdateTime;
#endif SERV_FIX_CCU_REPORT
	//}}
};

DefSingletonInline( KServerCCUManager );

template < class T >
void KServerCCUManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_LOG_DB, 0, NULL, usEventID, data );
}


#endif SERV_CCU_NEW
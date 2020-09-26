#pragma once


#include <KNCSingleton.h>

//{{ 2009. 6. 10  최육사	타이머 매니저
#include "TimerManager.h"
//}}

//{{ 2012. 09. 03	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER

class KGSGlobalMissionManager
{
	DeclareSingleton( KGSGlobalMissionManager );

public:


	KGSGlobalMissionManager(void);
	~KGSGlobalMissionManager(void);

	void Init();

	void UpdateGlobalMissionInfo( IN const KEGB_GET_GLOBAL_MISSION_INFO_ACK& kPacketAck );
	void GetGlobalMissionInfo( OUT KEGS_GLOBAL_MISSION_UPDATE_NOT &kOut );
	void UpdateGlobalMissionStartTime( IN std::wstring wstrGlobalMissionDay, IN std::wstring& wstrDate );
	
	int GetRemainTime();
	int GetGlobalMissionStartRemainTime();

	int GetTickCount()	{ return m_iTickCount; }
protected:
	template < class T > void SendToGlobalServer( unsigned short usEventID, const T& data );

private:
	int		m_iTickCount;
	KTimerManager	m_kTimer;
	CTime	m_tWorldMissionEndTime;
	CTime	m_tWorldMissionNextStartTime;
	
	std::map< std::wstring, KGlobalMissionInfo > m_mapGlobalMissionInfo;

};

DefSingletonInline( KGSGlobalMissionManager );

template < class T >
void KGSGlobalMissionManager::SendToGlobalServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GLOBAL_USER, 0, NULL, usEventID, data );
}

#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03  임홍락  글로벌 미션 매니저

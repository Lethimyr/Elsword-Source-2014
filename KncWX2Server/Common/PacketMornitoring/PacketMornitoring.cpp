#include "PacketMornitoring.h"


#include <dbg/dbg.hpp>
#include "BaseServer.h"
#include "NetError.h"
#include "CompareLimitList.h"

#include <iomanip>


//{{ 2011. 12. 14	최육사	패킷 처리 지연 시간 체크
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK // 빌드 오류로 해외팀 추가


ImplementRefreshSingleton( KPacketMornitoring );



KPacketMornitoring::KPacketMornitoring(void)
{
}

KPacketMornitoring::~KPacketMornitoring(void)
{
}

ImplToStringW( KPacketMornitoring )
{
	return stm_;
}

ImplementLuaScriptParser( KPacketMornitoring )
{
}

void KPacketMornitoring::Tick()
{
	// 5분간격으로 DB에 백업하자!
	if( m_tDBUpdateTimer.elapsed() < 300 )
		return;

	m_tDBUpdateTimer.restart();
    
	// DB에 로그 업데이트
	BackUpLogToDB();
}

void KPacketMornitoring::Clear()
{
	{	
		for( int iIndex = 0; iIndex < EVENT_EVENTID_SENTINEL; ++iIndex )
		{
			m_arrPacketLatency[iIndex].Clear();
			m_arrLoginProxyPacketCount[iIndex].Clear();
			m_arrCnProxyPacketCount[iIndex].Clear();
			m_arrGlobalProxyPacketCount[iIndex].Clear();
		}
	}
}

void KPacketMornitoring::CheckPacketProcessingLatency( IN const u_short usEventID, IN const UidType iUnitUID, IN const DWORD dwTickCount )
{
	if( usEventID >= EVENT_EVENTID_SENTINEL )
	{
		START_LOG( cerr, L"비 정상적인 eventid값입니다!" )
			<< BUILD_LOG( usEventID )
			<< END_LOG;
		return;
	}

	if( dwTickCount > m_arrPacketLatency[usEventID].m_dwMaxTickCount )
	{
		m_arrPacketLatency[usEventID].m_dwMaxTickCount = dwTickCount;
		m_arrPacketLatency[usEventID].m_iUnitUID = iUnitUID;
		m_arrPacketLatency[usEventID].m_tMaxCurTime = CTime::GetCurrentTime();
	}
}

void KPacketMornitoring::CheckLoginProxyPacketProcessingCount( IN const u_short usEventID )
{
	if( usEventID >= EVENT_EVENTID_SENTINEL )
	{
		START_LOG( cerr, L"비 정상적인 eventid값입니다!" )
			<< BUILD_LOG( usEventID )
			<< END_LOG;
		return;
	}

	++m_arrLoginProxyPacketCount[usEventID].m_dwProcessingCount;
}

void KPacketMornitoring::CheckCnProxyPacketProcessingCount( IN const u_short usEventID )
{
	if( usEventID >= EVENT_EVENTID_SENTINEL )
	{
		START_LOG( cerr, L"비 정상적인 eventid값입니다!" )
			<< BUILD_LOG( usEventID )
			<< END_LOG;
		return;
	}

	++m_arrCnProxyPacketCount[usEventID].m_dwProcessingCount;
}

void KPacketMornitoring::CheckGlobalProxyPacketProcessingCount( IN const u_short usEventID )
{
	if( usEventID >= EVENT_EVENTID_SENTINEL )
	{
		START_LOG( cerr, L"비 정상적인 eventid값입니다!" )
			<< BUILD_LOG( usEventID )
			<< END_LOG;
		return;
	}

	++m_arrGlobalProxyPacketCount[usEventID].m_dwProcessingCount;
}

void KPacketMornitoring::BackUpLogToDB()
{
    KDBE_UPDATE_PAKCET_MORNITORING_LOG_NOT kPacketNot;

	{
		KCompareLimitList< KPacketProcessingLatencyInfo, 10 > listLatencyInfo;
		KCompareLimitList< KPacketProcessingCountInfo, 10 > listLoginProxyCountInfo;
		KCompareLimitList< KPacketProcessingCountInfo, 10 > listCnProxyCountInfo;
		KCompareLimitList< KPacketProcessingCountInfo, 10 > listGlobalCountInfo;

		for( int iIndex = 0; iIndex < EVENT_EVENTID_SENTINEL; ++iIndex )
		{
			if( m_arrPacketLatency[iIndex].m_dwMaxTickCount > 0 )
			{
				KPacketProcessingLatencyInfo kInfo;
				kInfo.m_dwMaxLatency	   = m_arrPacketLatency[iIndex].m_dwMaxTickCount;
				kInfo.m_usEventID		   = static_cast<u_short>(iIndex);
				kInfo.m_iUnitUID		   = m_arrPacketLatency[iIndex].m_iUnitUID;
				kInfo.m_wstrMaxLatencyTime = (std::wstring)m_arrPacketLatency[iIndex].m_tMaxCurTime.Format( _T("%Y-%m-%d %H:%M:%S") );
				listLatencyInfo.IfLowerValuePushBack( kInfo.m_dwMaxLatency, kInfo );
			}
			
			if( m_arrLoginProxyPacketCount[iIndex].m_dwProcessingCount > 0 )
			{
				KPacketProcessingCountInfo kInfo;
				kInfo.m_dwCount = m_arrLoginProxyPacketCount[iIndex].m_dwProcessingCount;
				kInfo.m_usEventID = static_cast<u_short>(iIndex);
				listLoginProxyCountInfo.IfLowerValuePushBack( kInfo.m_dwCount, kInfo );
			}

			if( m_arrCnProxyPacketCount[iIndex].m_dwProcessingCount > 0 )
			{
				KPacketProcessingCountInfo kInfo;
				kInfo.m_dwCount = m_arrCnProxyPacketCount[iIndex].m_dwProcessingCount;
				kInfo.m_usEventID = static_cast<u_short>(iIndex);
				listCnProxyCountInfo.IfLowerValuePushBack( kInfo.m_dwCount, kInfo );
			}

			if( m_arrGlobalProxyPacketCount[iIndex].m_dwProcessingCount > 0 )
			{
				KPacketProcessingCountInfo kInfo;
				kInfo.m_dwCount = m_arrGlobalProxyPacketCount[iIndex].m_dwProcessingCount;
				kInfo.m_usEventID = static_cast<u_short>(iIndex);
				listGlobalCountInfo.IfLowerValuePushBack( kInfo.m_dwCount, kInfo );
			}
		}

		{
			std::list< std::pair< int, KPacketProcessingLatencyInfo > >::const_iterator lt;
			for( lt = listLatencyInfo.GetList().begin(); lt != listLatencyInfo.GetList().end(); ++lt )
			{
				kPacketNot.m_vecGSUserPacketLatencyInfo.push_back( lt->second );
			}
		}
		{
			std::list< std::pair< int, KPacketProcessingCountInfo > >::const_iterator lt;
			for( lt = listLoginProxyCountInfo.GetList().begin(); lt != listLoginProxyCountInfo.GetList().end(); ++lt )
			{
				kPacketNot.m_vecLoginProxyPacketCountInfo.push_back( lt->second );
			}
		}
		{
			std::list< std::pair< int, KPacketProcessingCountInfo > >::const_iterator lt;
			for( lt = listCnProxyCountInfo.GetList().begin(); lt != listCnProxyCountInfo.GetList().end(); ++lt )
			{
				kPacketNot.m_vecCnProxyPacketCountInfo.push_back( lt->second );
			}
		}
		{
			std::list< std::pair< int, KPacketProcessingCountInfo > >::const_iterator lt;
			for( lt = listGlobalCountInfo.GetList().begin(); lt != listGlobalCountInfo.GetList().end(); ++lt )
			{
				kPacketNot.m_vecGlobalProxyPacketCountInfo.push_back( lt->second );
			}
		}
	}

	// 백업할 데이터를 모두 모았으니 현재까지 쌓인 데이터는 초기화 하자!
	Clear();

	// DB로 가서 업데이트!
	SendToLogDB( DBE_UPDATE_PAKCET_MORNITORING_LOG_NOT, kPacketNot );
}

void KPacketMornitoring::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}


#endif SERV_PACKET_PROCESSING_LATENCY_CHECK	// 빌드 오류로 해외팀 추가
//}}



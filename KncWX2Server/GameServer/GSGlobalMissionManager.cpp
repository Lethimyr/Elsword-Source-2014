

#include "GameServer.h"
#include ".\GSGlobalMissionManager.h"
#include "Enum/Enum.h"
#include "socket/NetCommon.h"

//{{ 2012. 09. 03	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER

ImplementSingleton( KGSGlobalMissionManager );

KGSGlobalMissionManager::KGSGlobalMissionManager(void)
{	
	m_iTickCount = 0;
}

KGSGlobalMissionManager::~KGSGlobalMissionManager(void)
{
}

void KGSGlobalMissionManager::Init()
{
	// 글로벌 서버로 부터 이벤트 던전 데이터 얻기
	KEGB_GET_MISSION_INFO_REQ kPacketReq;
	kPacketReq.m_iServerUID = KBaseServer::GetKObj()->GetUID();
	SendToGlobalServer( EGB_GET_GLOBAL_MISSION_INFO_REQ, kPacketReq );
}

void KGSGlobalMissionManager::UpdateGlobalMissionInfo( IN const KEGB_GET_GLOBAL_MISSION_INFO_ACK& kPacketAck )
{
	switch( kPacketAck.m_iFlag )
	{
	case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_NONE:
		{
			START_LOG( cerr, L"받아서는 안되는 타입" )
				<< BUILD_LOG( kPacketAck.m_iFlag )
				<< END_LOG;

			return;
		} break;
		case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_INIT:
		case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_UPDATE:
		case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_BEGIN:
		{
			m_mapGlobalMissionInfo = kPacketAck.m_mapGlobalMissionInfo;

			if( kPacketAck.m_iFlag == KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_BEGIN )
			{
				m_kTimer.restart();
			}
		}break;
	case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_SUCCESS:
	case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_FAIL:
		{
			m_mapGlobalMissionInfo = kPacketAck.m_mapGlobalMissionInfo;
		} break;
	default:
		{
			START_LOG( cerr, L"받아서는 안되는 타입" )
				<< BUILD_LOG( kPacketAck.m_iFlag )
				<< END_LOG;

			return;
		}break;
	}

	m_iTickCount++;

	START_LOG( clog, L"글로벌 미션 정보 변경" )
		<< BUILD_LOG( m_iTickCount )
		<< END_LOG;
}

void KGSGlobalMissionManager::GetGlobalMissionInfo( OUT KEGS_GLOBAL_MISSION_UPDATE_NOT &kOut )
{
	kOut.m_mapGlobalMissionInfo = m_mapGlobalMissionInfo;
}

void KGSGlobalMissionManager::UpdateGlobalMissionStartTime( IN std::wstring wstrGlobalMissionDay, IN std::wstring& wstrDate )
{
/*
	std::map<int, KGlobalMissionOpenInfo>::iterator mit = m_mapGlobalMissionOpenInfo.find(iGlobalMissionID);
	if(mit != m_mapGlobalMissionOpenInfo.end())
	{
		mit->second.m_wstrScriptStartTime = wstrDate;
	}
*/
}

int KGSGlobalMissionManager::GetGlobalMissionStartRemainTime()
{
/*
	CTime tCurrent = CTime::GetCurrentTime();

	if( m_tWorldMissionNextStartTime > tCurrent )	// 월드 미션이 이후에 존재한다
	{
		CTimeSpan gap = m_tWorldMissionNextStartTime - tCurrent;

		return (int)gap.GetTotalMinutes();
	}
	else if( m_iMissionTime == 0 )					// 월드 미션 이후에 없다
	{
		return -1;
	}
	else											// 월드 미션이 진행 중이다
	{
		return 0;
	}
*/
	return 0;
}
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03  임홍락	글로벌 미션 매니저
#pragma once

#ifdef SERV_GLOBAL_MISSION_MANAGER

#include "RefreshSingleton.h"
#include "DBLayer.h"
#include <KncSend.h>
#include <atltime.h>
#include <boost/timer.hpp>
#include "CommonPacket.h"
#include "ServerPacket.h"

class KGlobalMissionManager
{
	DeclareRefreshSingleton( KGlobalMissionManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:

	KGlobalMissionManager(void);
	~KGlobalMissionManager(void);

	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );

	void	Tick();

	void	Init();


	void	SetGlobalMissionSuccess_LUA( bool bSuccess, std::wstring wstrGlobalMissionDay );	//	치트용
	void	SetGlobalMissionOn_LUA( bool bOn )		{ m_bGlobalMissionOn = bOn; }
	
	void	GlobalMissionDump_LUA();

	bool	SetTodayMissionTime();
	void	CheckGlobalMissionProcess();
	void	Reset_LUA();	//	치트용
	
	bool	GetGlobalMissionState()					{ return m_bGlobalMissionOn; }
	void	InitMissionData();
	void	SetMissionData();
	void	StartMission( std::wstring wstrGlobalMissionDay, bool IsReStart = false );
	void	EndMission( std::wstring wstrGlobalMissionDay );
	void	SetInitMissionInfo( KDBE_GET_GLOBAL_MISSION_INFO_ACK kPacket );
	int		GetCorrectMissionTime( std::wstring wstrGlobalMissionDay );

	void	BroadCastGlobalMission( KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FLAG_TYPE eType, std::wstring wstrGlobalMissionDay );
	void	BroadCastGlobalMissionStartTime();

	int		IncreaseGlobalMissionClearCount( std::wstring& wstrGlobalMissionDay, int iClearCount );
	void	GetGlobalMissionInfo( IN const int& iType, OUT KEGB_GET_GLOBAL_MISSION_INFO_ACK &kPacketAck );

	void	SetIsFirstTick( bool IsFirst )			{ m_bIsFirstTick = IsFirst; }
	bool	GetIsFirstTick()						{ return m_bIsFirstTick; }

	void	WriteGlobalMissionInfo();
	//void	WriteGlobalMissionRewardInfo( KGlobalMissionInfo* pkGlobalMissionInfo, bool bSuccess );

private:
	bool			m_bIsFirstTick;
	bool			m_bReceiveGlobalMissionInfo;

	boost::timer	m_kTimerMissionStart;				//	다음 글로벌 미션 시작 시간 체크를 위한 타이머
	boost::timer	m_kTimerEnableCheck;				//	일정 주기 체크를 위한 타이머
	boost::timer	m_kTimerUpdate;						//	게임 서버 정보 업데이트용 타이머
	
	std::map< std::wstring, KGlobalMissionInfo >	m_mapGlobalMissionInfo;					// 글로벌 미션별 오픈 정보
	bool											m_bGlobalMissionOn;						// 글로벌 미션을 하는가?
	int												m_iNextGlobalMissionStartRemainTime;	// 다음 글로벌 미션이 앞으로 몇초후에 시작될 것인가?
};

DefRefreshSingletonInline( KGlobalMissionManager );

template < class T >
void KGlobalMissionManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, usEventID, data );
}

#endif SERV_GLOBAL_MISSION_MANAGER

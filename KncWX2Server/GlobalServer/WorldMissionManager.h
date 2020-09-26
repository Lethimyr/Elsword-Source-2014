//{{2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER

#pragma once

#include "RefreshSingleton.h"
#include "DBLayer.h"
#include <KncSend.h>
#include <atltime.h>
#include <boost/timer.hpp>
#include "CommonPacket.h"
#include "ServerPacket.h"

//{{ 공존의 축제 - 지진 효과 - 김민성
#ifdef SERV_EARTH_QUAKE_EFFECT
#include "Lottery.h"
#endif SERV_EARTH_QUAKE_EFFECT
//}}

//{{ 2013. 04. 13	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
class KWorldMissionManager
{
	DeclareRefreshSingleton( KWorldMissionManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum DEFENSE_REWARD_BUFF_LEVEL_NEEDS
	{
		DEFENSE_REWARD_BUFF_LEVEL1_NEEDS = 0,
		DEFENSE_REWARD_BUFF_LEVEL2_NEEDS = 3,
		DEFENSE_REWARD_BUFF_LEVEL3_NEEDS = 9,
		DEFENSE_REWARD_BUFF_LEVEL4_NEEDS = 15,
		DEFENSE_REWARD_BUFF_MAX_NEEDS = 16,
	};

public:
	KWorldMissionManager(void);
	~KWorldMissionManager(void);

	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );

	void	Tick( void );
	void	Init( void );

	void	SetMissionSuccess_LUA(bool bSuccess);	//	치트용
	void	SetDefenseOpenTime_LUA( int iDayOfWeek, int iStartHour, int iStartMin, int iDurationTime );
	void	SetWorldBuffDurationTime_LUA( int iBuffDurationTime )		{ m_usWorldBuffDurationTime = static_cast<u_short>(iBuffDurationTime); }

	void	WorldMissionDump_LUA();
	
	//{{ 공존의 축제 - 지진 효과 - 김민성
#ifdef SERV_EARTH_QUAKE_EFFECT
	void SetRandomEarthQuakeTime_LUA( int iSec, float fRate );
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

	void	SetReduceTerm_LUA( float fReduceTerm ){	m_fReduceTerm = fReduceTerm;	}
	float	GetReduceTerm( void ) const{	return m_fReduceTerm;	}

	int		GetDayOfTheWeek( void ) const{	return m_tToDayMissionTime.GetDayOfWeek();	}
	bool	SetTodayMissionTime( void );
	void	CheckWorldMissionProcess( void );

	void	SetActive( IN const bool bActive ){ m_bActive = bActive; }
	bool	IsActive( void ) const{ return m_bActive; }

	void	InitMissionData( void );
	void	SetMissionData( void );
	void	StartMission( IN const bool IsReStart = false );
	void	EndMission( void );

	void	BroadCastMission( IN const KEGB_UPDATE_MISSION_INFO_NOT::FLAG_TYPE eType );
	void	BroadCastWorldBuff( IN const int _skill );
	void	BroadCastMissionStartTime( void );

	void	IncreaseProtectedCrystalCount( IN const int iProtectedCrystalCount );
	int		GetProtectedCrystalCount( void ) const{	return m_iProtectedCrystalCount;	}
	void	ResetProtectedCrystalCount( void );
	
	void	GetWorldMissionInfo( IN const int& iType, OUT KEGB_GET_MISSION_INFO_ACK &kPacketAck ) const;

	void	SetIsFirstTick( IN const bool IsFirst ){	m_bIsFirstTick = IsFirst;	}
	bool	GetIsFirstTick( void ) const{	return m_bIsFirstTick;	}
	int		GetCorrectMissionTime( void ) const;
	CTime	GetToDayMissionTime( void ) const{	return m_tToDayMissionTime;	}

	void	WriteWorldMissionCrystalCount( IN const bool bforce = false );
	void	SetInitMissionInfo( IN const KDBE_GET_DEFENCE_CRYSTAL_ACK& KPacket );

	void	SetMissionTime( IN const int iTime ){	m_iMissionTime = iTime;	}
	bool	GetWorldMissionState( void ) const{	return m_bWorldMissionOn;	}

	void	BroadCastIndividualBuffLevel( IN const int& iIndividualBuffLevel ) const;

	int		CheckWorldBuff( void ) const;
	int		CheckIndividualBuff( void ) const;

	void	BuffProcess( void );

	//{{ 공존의 축제 - 지진 효과 - 김민성
#ifdef SERV_EARTH_QUAKE_EFFECT
	int  GetRandomEarthQuakeTime();
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

private:
	bool			m_bIsFirstTick;

	boost::timer	m_kTimerDOT;			//	던전오픈 시간 체크를 위한 타이머
	boost::timer	m_kTimerEnableCheck;	//	일정 주기 체크를 위한 타이머
	boost::timer	m_kTimerUpdate;			//	1분에 한번씩 게임서버로 현재상태 전달
	boost::timer	m_kTimerWorldBuff;		//	월드 버프 종료 시간 용
	boost::timer	m_kTimerForReduceTheProtectedCrystalCount;		//	분당 소모되는 카운트를 위한 타이머

	bool			m_bActive;				//	현재 던전 개방되어 있지 여부
	bool			m_bWorldBuffTime;		//  월드 버프 시간 용
	int				m_iMissionTime;		//	월드미션 진행시간(단위 : 초)
	int				m_iNextDOT;			//	다음에 개방될 던전 대기시간(단위 : 초)
	int				m_iTotalCCU;

	//	현재 진행 미션
	int				m_iStartedCrystalCount;
	int				m_iProtectedCrystalCount;	//  보호된 전체 수정 수
	int				m_iReduceCount;				//	분당 소모되는 카운트	: m_iTotalCCU / 60

	CTime													m_tToDayMissionTime;	// 스크립드상의 던전 오픈 시간
	CTime													m_tToDayStartTime;		// 실제 던전 오픈된 시간
	std::map< int, std::vector< KDefenseOpenTime > >		m_mapDefenseOpenTime;	// 요일별 오픈 시간
	bool													m_bWorldMissionOn;		// 월드 미션을 하는가?

	u_short			m_usWorldBuffDurationTime;	//  월드 버프 지속 시간

	int				m_iWorldBuffID;
	int				m_iIndividualBuffID;

	//{{ 공존의 축제 - 지진 효과 - 김민성
#ifdef SERV_EARTH_QUAKE_EFFECT
	KLottery					m_kEarthQuakeLottery;
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

	float			m_fReduceTerm;
};
#else
class KWorldMissionManager
{
	DeclareRefreshSingleton( KWorldMissionManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:

	enum WORLD_BUFF_START_CONDITION
	{
		WBSC_STEP_NONE = 0,
		WBSC_STEP_ONE = 3,
		WBSCL_STEP_TWO = 9,
		WBSC_STEP_THREE = 15,
	};

	//{{ 2012. 04. 2	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	enum DEFENSE_REWARD_BUFF_LEVEL_NEEDS
	{
		DEFENSE_REWARD_BUFF_LEVEL1_NEEDS = 0,
		DEFENSE_REWARD_BUFF_LEVEL2_NEEDS = 3,
		DEFENSE_REWARD_BUFF_LEVEL3_NEEDS = 9,
		DEFENSE_REWARD_BUFF_LEVEL4_NEEDS = 15,
		DEFENSE_REWARD_BUFF_MAX_NEEDS = 16,
	};

	enum GATE_OF_THE_DARKNESS_DIFF
	{
		GATE_OF_THE_DARKNESS_HARD	= 7,
		GATE_OF_THE_DARKNESS_EXPERT = 13,
	};
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	KWorldMissionManager(void);
	~KWorldMissionManager(void);

	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );

	void	Tick();

	void	Init();

	void	SetDungeonPlayTime_LUA(int iTime)		{ m_iDungeonPlayTime = iTime; }
	void	SetMissionSuccess_LUA(bool bSuccess);	//	치트용

	void	SetDefenseOpenTime_LUA( int iDayOfWeek, int iStartHour, int iStartMin, int iDurationTime );
	void	SetWorldMissionOn_LUA( bool bOn )			{ m_bWorldMissionOn = bOn; 	}
	
	void	SetNormalPlayRate_LUA(float fRate)			{ m_fNormalPlayRate = fRate; }
	void	SetHardPlayRate_LUA(float fRate)			{ m_fHardPlayRate = fRate; }
	void	SetExpertPlayRate_LUA(float fRate)			{ m_fExpertPlayRate = fRate; }

	void	SetNormalSuccessRate_LUA(float fRate)		{ m_fNormalSuccessRate = fRate; }
	void	SetHardSuccessRate_LUA(float fRate)			{ m_fHardSuccessRate = fRate; }
	void	SetExpertSuccessRate_LUA(float fRate)		{ m_fExpertSuccessRate = fRate; }

	void	SetWorldBuffDurationTime_LUA( int iBuffDurationTime )		{ m_usWorldBuffDurationTime = static_cast<u_short>(iBuffDurationTime); }

	void	SetNormalBasicCrystalCount_LUA( char cCrystal )				{ m_cNormalBasicCrystalCount = cCrystal; }
	void	SetHardBasicCrystalCount_LUA( char cCrystal )				{ m_cHardBasicCrystalCount = cCrystal; }
	void	SetExpertBasicCrystalCount_LUA( char cCrystal )				{ m_cExpertBasicCrystalCount = cCrystal; }

	void	BuffCheat_LUA( int iBuffType, bool bForce );
	void	WorldMissionDump_LUA();

	int		GetDayOfTheWeek()		{ return m_tToDayMissionTime.GetDayOfWeek(); }
	bool	SetTodayMissionTime();
	void	CheckWorldMissionProcess();

	void	Reset_LUA();	//	치트용

	void	SetActive( bool bActive )				{ m_bActive = bActive; }
	bool	IsActive()								{ return m_bActive; }

	void	InitMissionData();
	void	SetMissionData();
	void	StartMission(bool IsReStart = false );
	void	EndMission();

	void	BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FLAG_TYPE eType );
	void	BroadCastWorldBuff( int _skill, bool bIsEnd = false );
	void	BroadCastMissionStartTime();

	int		IncreaseProtectedCrystalCount( int ProtectedCrystalCount );
	int		IncreaseWorldCrystalCount()													{ return ++m_iWorldCrystalCount; }

	int		GetProtectedCrystalCount()													{ return m_iProtectedCrystalCount; }
	int		GetWorldCrystalCount();

	//{{ 2012. 04. 24	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	void	ResetProtectedCrystalCount( void );
	void	GetWorldMissionInfo( IN const int& iType, OUT KEGB_GET_MISSION_INFO_ACK &kPacketAck );
#else
	void	GetWorldMissionInfo( OUT KEGB_GET_MISSION_INFO_ACK &kPacketAck );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	
	void	SetIsFirstTick( bool IsFirst )			{ m_bIsFirstTick = IsFirst; }
	bool	GetIsFirstTick()						{ return m_bIsFirstTick; }
	void	CheckWorldCrystalCount( bool flag = false );
	int		GetCorrectMissionTime();
	CTime	GetToDayMissionTime()					{ return m_tToDayMissionTime; }

	void	WriteWorldMissionCrystalCount( bool bforce = false );
	void	SetInitMissionInfo( KDBE_GET_DEFENCE_CRYSTAL_ACK KPacket );

	void	SetMissionTime( int iTime );
	bool	GetWorldMissionState()					{ return m_bWorldMissionOn; }

	//{{ 2012. 03. 23	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	void BroadCastIndividualBuffLevel( void ) const;
	void BroadCastIndividualBuffLevel( IN const int& iIndividualBuffLevel ) const;
	void IndividualBuffEndProcess( void );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 공존의 축제 - 지진 효과 - 김민성
#ifdef SERV_EARTH_QUAKE_EFFECT
	void SetRandomEarthQuakeTime_LUA( int iSec, float fRate );
	int  GetRandomEarthQuakeTime();
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

private:
	bool			m_bIsFirstTick;

	boost::timer	m_kTimerDOT;			//	던전오픈 시간 체크를 위한 타이머
	boost::timer	m_kTimerEnableCheck;	//	일정 주기 체크를 위한 타이머
	boost::timer	m_kTimerUpdate;			//	1분에 한번씩 게임서버로 현재상태 전달
	boost::timer	m_kTimerWorldBuff;		//	월드 버프 종료 시간 용

	bool			m_bActive;				//	현재 던전 개방되어 있지 여부
	bool			m_bWorldBuffTime;		//  월드 버프 시간 용

	int				m_iMissionTime;		//	월드미션 진행시간(단위 : 초)

	int				m_iNextDOT;			//	다음에 개방될 던전 대기시간(단위 : 초)
	
	int				m_iDungeonPlayTime;	//	던전 1회 플레이 타임(단위 : 초)

	int				m_iTotalCCU;

	//	현재 진행 미션
	int				m_iCrystalTargetCount;		//	보호 수정 목표 수
	int				m_iProtectedCrystalCount;	//  보호된 전체 수정 수
	float			m_fCountPerWorldCrystal;	//  월드 크리스탈 하나당 수정의 수   m_iCrystalTargetCount = m_fCountPerWorldCrystal * m_iWorldCrystalCount
	int				m_iWorldCrystalCount;		//	보호된 월드 크리스탈 수   x / 15

	CTime													m_tToDayMissionTime;	// 스크립드상의 던전 오픈 시간
	CTime													m_tToDayStartTime;		// 실제 던전 오픈된 시간
	std::map< int, std::vector< KDefenseOpenTime > >		m_mapDefenseOpenTime;	// 요일별 오픈 시간
	bool													m_bWorldMissionOn;		// 월드 미션을 하는가?

	float			m_fNormalPlayRate;			//	보통 참여율
	float			m_fHardPlayRate;			//	어려움 참여율
	float			m_fExpertPlayRate;			//	매우어려움 참여율

	float			m_fNormalSuccessRate;		//	보통 성공률
	float			m_fHardSuccessRate;			//	어려움 성공률
	float			m_fExpertSuccessRate;		//	매우어려움 성공률

	char			m_cNormalBasicCrystalCount;	//  보통 기본 수정(계산식 용)
	char			m_cHardBasicCrystalCount;	//  어려움 기본 수정(계산식 용)
	char			m_cExpertBasicCrystalCount;	//  매우 어려움 기본 수정(계산식 용)

	u_short			m_usSuccessWorldBuff;		//  적용된 월드 버프
	u_short			m_usWorldBuffDurationTime;	//  월드 버프 지속 시간

	//{{ 2012. 05. 03	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	bool			m_bRewardBuff;				// 어둠의 문 참가자 버프 상태
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 공존의 축제 - 지진 효과 - 김민성
#ifdef SERV_EARTH_QUAKE_EFFECT
	KLottery					m_kEarthQuakeLottery;
#endif SERV_EARTH_QUAKE_EFFECT
	//}}
};
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

DefRefreshSingletonInline( KWorldMissionManager );

template < class T >
void KWorldMissionManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, usEventID, data );
}

#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

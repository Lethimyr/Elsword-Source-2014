#include "BattleFieldGameManager.h"


//{{ 2011. 10. 18	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


KBattleFieldGameManager::KBattleFieldGameManager() : 
m_iDangerousValue( 0 )
//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
, m_iOldDangerousValue( 0 )
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}
{
}

KBattleFieldGameManager::~KBattleFieldGameManager()
{
}

void KBattleFieldGameManager::StartGame()
{
	ResetDangerousValue();
	m_kDangerousEvent.Clear();
	m_kLotEliteMonsterDrop.Clear();
}

void KBattleFieldGameManager::EndGame()
{
	ResetDangerousValue();
	m_kDangerousEvent.Clear();
	m_kLotEliteMonsterDrop.Clear();
}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-28	// 박세훈
#else // SERV_BATTLE_FIELD_BOSS
void KBattleFieldGameManager::IncreaseDangerousValue( IN const int iIncreaseValue )
{
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_iOldDangerousValue = m_iDangerousValue;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	
	//{{ 2013. 03. 07	배틀 필드 위험도 이벤트 - 김민성
#ifdef SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	m_iDangerousValue += iIncreaseValue * SiCXSLBattleFieldManager()->GetDangerousValueEventRate();
#else
	m_iDangerousValue += iIncreaseValue;
#endif SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	//}

	if( ( m_iDangerousValue < 0 ) || ( SiCXSLBattleFieldManager()->GetDangerousValueMax() <= m_iDangerousValue ) )
	{
		ResetDangerousValue();
	}
}

void KBattleFieldGameManager::OnNpcUnitDie( IN const int iPlayerCount, 
										    IN const bool bIsAttribNpc, 
											IN const char cDifficultyLevel, 
											IN const char cMonsterGrade
											//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
											, IN const bool bIncreaseDanger 
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
											//}
											)
{
	// 변경전의 위험도 저장
	const int iBeforeDangeroursValue = GetDangerousValue();

	// 죽은 몬스터의 몬스터 타입 상수값을 구한다.
	const int iMonsterTypeFactor = CXSLUnitManager::GetMonsterTypeFactor( bIsAttribNpc, cDifficultyLevel, cMonsterGrade );

	// 위험도를 증가 시킨다.
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	if( bIncreaseDanger == true )
	{
		IncreaseDangerousValue( iMonsterTypeFactor );
	}
	else
	{
		return;
	}
#else
	IncreaseDangerousValue( iMonsterTypeFactor );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	//IncreaseDangerousValue( 500 ); //-- 테스트로 일단 500씩 증가시키자!

	// 경고 메시지 이벤트 체크
	CheckReserveWarningEvent( iBeforeDangeroursValue );	

	// 엘리트 몬스터 출현 이벤트 체크
	CheckReserveEliteMonsterDropEvent( iBeforeDangeroursValue );

	// 중간 보스 출현 이벤트 체크
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	CheckReserveMiddleBossDropEvent( iPlayerCount );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	CheckReserveEventBossDropEvent( iPlayerCount );
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
	
	// 보스 출현 이벤트 체크
	CheckReserveBossDropEvent( iPlayerCount );
}
#endif // SERV_BATTLE_FIELD_BOSS

void KBattleFieldGameManager::CheckReserveWarningEvent( IN const int iBeforeDangerousValue )
{
	// 위험도 수치가 경고범위 안으로 들어오면 경고 이벤트를 발생 시킨다!
	if( iBeforeDangerousValue < SiCXSLBattleFieldManager()->GetDangerousValueWarning()  &&  GetDangerousValue() >= SiCXSLBattleFieldManager()->GetDangerousValueWarning() )
	{
		m_kDangerousEvent.ReserveEvent( KDangerousEventInfo::DE_WARNING_MESSAGE );
	}
}

void KBattleFieldGameManager::CheckReserveEliteMonsterDropEvent( IN const int iBeforeDangerousValue )
{
	// 위험도가 최소한 5는 넘어야 한다.
	if( GetDangerousValue() < SiCXSLBattleFieldManager()->GetEliteMonsterDropValue() )
		return;

	if( SiCXSLBattleFieldManager()->GetEliteMonsterDropValue() == 0 )
	{
		START_LOG( cerr, L"엘리트 몬스터 출현 상수값이 0이다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( SiCXSLBattleFieldManager()->GetEliteMonsterDropValue() )
			<< END_LOG;
		return;
	}

	// 위험도가 상수값의 배수인가?
	const bool bIsMultiplesOf = ( ( GetDangerousValue() % SiCXSLBattleFieldManager()->GetEliteMonsterDropValue() ) == 0 );

	// 위험도 상수값의 배수를 지나쳤는가? 그럼 이전보다 상수값 이상 증가하면 그게 지나친거 아닌가?
	const bool bIsGap = ( ( GetDangerousValue() - iBeforeDangerousValue ) > SiCXSLBattleFieldManager()->GetEliteMonsterDropValue() );

	// 위 사항에 모두 해당이 안되면 나가자!
	if( bIsMultiplesOf == false  &&  bIsGap == false )
		return;

	// 누적된 확률에 20%를 더해서 굴림하자!
	static int ELITE_MONSTER_DROP_CASE = 1;
	LIF( m_kLotEliteMonsterDrop.AddCase( ELITE_MONSTER_DROP_CASE, 20.f ) );

	// 굴림 결과가 ok면 확률 초기화, 아니면 패스!
	const int iResult = m_kLotEliteMonsterDrop.Decision();
	if( iResult != ELITE_MONSTER_DROP_CASE )
		return;

	// 드롭 성공이면 확률을 초기화 하자!
	m_kLotEliteMonsterDrop.Clear();

	// 엘리트 몬스터 드롭 예약!
	m_kDangerousEvent.ReserveEvent( KDangerousEventInfo::DE_ELITE_MONSTER_DROP );
}

//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
void KBattleFieldGameManager::CheckReserveMiddleBossDropEvent( IN const int iPlayerCount )
{
	// 현재 인원이 n명 이하면 나가자!
	if( iPlayerCount < SiCXSLBattleFieldManager()->GetBossCheckUserCount() )
		return;

	// 위험도 수치가 최소한 위험 상태 이상이어야 한다.
	if( GetDangerousValue() < SiCXSLBattleFieldManager()->GetDangerousValueWarning() )
		return;

	// 이미 국지 이벤트에 몬스터 드롭이 예약되어있다면 나가자!
	if( m_kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_MIDDLE_BOSS_MONSTER_DROP ) == true )
		return;

	//////////////////////////////////////////////////////////////////////////
	// 국지 이벤트 랜덤굴림 준비!
	// 4300 이후로는 50증가할때마다 이벤트 발생 확률이 10%씩 증가한다.
	static int BOSS_DROP_EVENT = 1;
	const float fBossDropRate = SiCXSLBattleFieldManager()->GetMiddleBossMonsterDropRate( GetDangerousValue(), GetOldDangerousValue() );
	//////////////////////////////////////////////////////////////////////////

	KLottery kBossDropLot;
	LIF( kBossDropLot.AddCase( BOSS_DROP_EVENT, fBossDropRate ) );

	// 국지 이벤트 굴림!
	const int iResult = kBossDropLot.Decision();
	if( iResult == BOSS_DROP_EVENT )
	{
		m_kDangerousEvent.ReserveEvent( KDangerousEventInfo::DE_MIDDLE_BOSS_MONSTER_DROP );
	}
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
void KBattleFieldGameManager::CheckReserveEventBossDropEvent( IN const int iPlayerCount )
{
	// 현재 인원이 n명 이하면 나가자!
	if( iPlayerCount < SiCXSLBattleFieldManager()->GetBossCheckUserCount() )
		return;

	// 위험도 수치가 최소한 위험 상태 이상이어야 한다.
	if( GetDangerousValue() < SiCXSLBattleFieldManager()->GetDangerousValueWarning() )
		return;

	// 이미 국지 이벤트에 몬스터 드롭이 예약되어있다면 나가자!
	if( m_kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_EVENT_BOSS_MONSTER_DROP ) == true )
		return;

	//////////////////////////////////////////////////////////////////////////
	// 국지 이벤트 랜덤굴림 준비!
	// 4300 이후로는 50증가할때마다 이벤트 발생 확률이 10%씩 증가한다.
	static int BOSS_DROP_EVENT = 1;
	const float fBossDropRate = SiCXSLBattleFieldManager()->GetEventBossMonsterDropRate( GetDangerousValue(), GetOldDangerousValue() );
	//////////////////////////////////////////////////////////////////////////

	KLottery kBossDropLot;
	LIF( kBossDropLot.AddCase( BOSS_DROP_EVENT, fBossDropRate ) );

	// 국지 이벤트 굴림!
	const int iResult = kBossDropLot.Decision();
	if( iResult == BOSS_DROP_EVENT )
	{
		m_kDangerousEvent.ReserveEvent( KDangerousEventInfo::DE_EVENT_BOSS_MONSTER_DROP );
	}
}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

void KBattleFieldGameManager::CheckReserveBossDropEvent( IN const int iPlayerCount )
{
	// 현재 인원이 n명 이하면 나가자!
	if( iPlayerCount < SiCXSLBattleFieldManager()->GetBossCheckUserCount() )
		return;

	// 위험도 수치가 최소한 위험 상태 이상이어야 한다.
	if( GetDangerousValue() < SiCXSLBattleFieldManager()->GetDangerousValueWarning() )
		return;

	// 이미 국지 이벤트에 몬스터 드롭이 예약되어있다면 나가자!
	if( m_kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_BOSS_MONSTER_DROP ) == true )
		return;

	//////////////////////////////////////////////////////////////////////////
	// 국지 이벤트 랜덤굴림 준비!
	// 4300 이후로는 50증가할때마다 이벤트 발생 확률이 10%씩 증가한다.
	static int BOSS_DROP_EVENT = 1;
	const float fBossDropRate = SiCXSLBattleFieldManager()->GetBossMonsterDropRate( GetDangerousValue() );
	//////////////////////////////////////////////////////////////////////////

	KLottery kBossDropLot;
	LIF( kBossDropLot.AddCase( BOSS_DROP_EVENT, fBossDropRate ) );

	// 국지 이벤트 굴림!
	const int iResult = kBossDropLot.Decision();
	if( iResult == BOSS_DROP_EVENT )
	{
		m_kDangerousEvent.ReserveEvent( KDangerousEventInfo::DE_BOSS_MONSTER_DROP );
	}
}

bool KBattleFieldGameManager::CheckAndDeleteReservedDangerousEvent( IN const KDangerousEventInfo::DANGEROUS_EVENT eEventEnum )
{
    const bool bRet = m_kDangerousEvent.IsEventReserved( eEventEnum );
	if( bRet )
	{
		m_kDangerousEvent.DeleteEvent( eEventEnum );
	}
	return bRet;
}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-28	// 박세훈
void KBattleFieldGameManager::UpdateDangerousValue( IN const int iDangerousValue )
{
	m_iOldDangerousValue	= m_iDangerousValue;
	m_iDangerousValue		= iDangerousValue;
}
#endif // SERV_BATTLE_FIELD_BOSS

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

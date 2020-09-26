#pragma once
#include "X2Data/XSLBattleFieldManager.h"


//{{ 2011. 10. 18	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KBattleFieldGameManager
{
public:
    KBattleFieldGameManager();
    virtual ~KBattleFieldGameManager();

	// 게임 시작, 종료 처리
	void	StartGame();
	void	EndGame();

	// 정보 얻기
	int		GetDangerousValue() const				{ return m_iDangerousValue; }
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	int		GetOldDangerousValue() const				{ return m_iOldDangerousValue; }
	void	ResetDangerousValue()						{ m_iDangerousValue = 0; m_iOldDangerousValue = 0; }
#else
	void	ResetDangerousValue()					{ m_iDangerousValue = 0; }
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	KDangerousEventInfo& GetDangerousEventInfo()	{ return m_kDangerousEvent; }

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-28	// 박세훈
	void	UpdateDangerousValue( IN const int iDangerousValue );
#else // SERV_BATTLE_FIELD_BOSS
	void	IncreaseDangerousValue( IN const int iIncreaseValue );
	
	// 처리 함수
	void	OnNpcUnitDie( IN const int iPlayerCount,
						  IN const bool bIsAttribNpc, 
						  IN const char cDifficultyLevel, 
						  IN const char cMonsterGrade 
						  //{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
						  , IN const bool bIncreaseDanger 
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
						  //}
						  );
#endif // SERV_BATTLE_FIELD_BOSS

	bool	CheckAndDeleteReservedDangerousEvent( IN const KDangerousEventInfo::DANGEROUS_EVENT eEventEnum );

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-28	// 박세훈
public:
	void	CheckReserveWarningEvent( IN const int iBeforeDangerousValue );
	void	CheckReserveEliteMonsterDropEvent( IN const int iBeforeDangerousValue );
	void	CheckReserveBossDropEvent( IN const int iPlayerCount );
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	void	CheckReserveMiddleBossDropEvent( IN const int iPlayerCount );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#else // SERV_BATTLE_FIELD_BOSS
protected:
	void	CheckReserveWarningEvent( IN const int iBeforeDangerousValue );
	void	CheckReserveEliteMonsterDropEvent( IN const int iBeforeDangerousValue );
	void	CheckReserveBossDropEvent( IN const int iPlayerCount );
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	void	CheckReserveMiddleBossDropEvent( IN const int iPlayerCount );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#endif // SERV_BATTLE_FIELD_BOSS
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	void	CheckReserveEventBossDropEvent( IN const int iPlayerCount );
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT			
protected:
	int								m_iDangerousValue;			// 위험도
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	int								m_iOldDangerousValue;			// 이전 위험도
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	KDangerousEventInfo				m_kDangerousEvent;			// 이벤트 예약
	KLottery						m_kLotEliteMonsterDrop;		// 엘리트 몬스터 드롭 확률
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}



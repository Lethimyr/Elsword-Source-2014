#include ".\userspiritmanager.h"
#include "X2Data/XSLDungeonManager.h"
#include "X2Data/XSLBattleFieldManager.h"
#include "GameEvent/GameEventManager.h"
#include "SpiritTable.h"


#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
#include "UserQuestManager.h"
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST

//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


KUserSpiritManager::KUserSpiritManager(void)
{
	Clear();
}

KUserSpiritManager::~KUserSpiritManager(void)
{
}

void KUserSpiritManager::Clear()
{
	SetSpiritMax( 0 );
	SetSpirit( 0 );
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	SetAccumulationSpirit( 0 );
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
	SetIsSpiritUpdated( false );
}

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
void KUserSpiritManager::Init( IN const int iSpirit, IN const int iSpiritMax, IN const int iAccumulationSpirit )
#else
void KUserSpiritManager::Init( IN const int iSpirit, IN const int iSpiritMax )
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
{
	SetSpirit( iSpirit );
	SetSpiritMax( iSpiritMax );
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	SetAccumulationSpirit( iAccumulationSpirit );
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
}

void KUserSpiritManager::GetDBUpdateInfo( OUT int& iSpirit, OUT bool& bIsSpiritUpdated )
{
    iSpirit = GetSpirit();
	bIsSpiritUpdated = IsSpiritUpdated();
}

int	KUserSpiritManager::RestoreSpirit( IN const int iRestoreSpirit )
{
	// 근성도 최대치 검사
	if( GetSpiritMax() < ( GetSpirit() + iRestoreSpirit ) )
	{
		m_iSpirit = m_iSpiritMax;
	}
	else
	{
		m_iSpirit += iRestoreSpirit;
	}
	
	return GetSpirit();
}

bool KUserSpiritManager::CheckingSpirit( IN const int iDungeonID, IN const bool bIsPcBang ) const
{
	// 0. 던전ID값이 유효한지 체크하자!
	if( iDungeonID == 0 )
	{
		START_LOG( cerr, L"던전ID값이 유효하지 않다!" )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;
		return false;
	}

	// 근성도와 관련없는 던전이면 바로 입장처리
	if( CXSLDungeon::IsTutorialDungeon( iDungeonID )  ||
		CXSLDungeon::IsEventDungeon( iDungeonID ) )
	{
		return true;
	}

	//{{ 2010. 12. 30	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON
	if( CXSLDungeon::IsShipDungeon( iDungeonID ) )
	{
		return true;
	}
#endif SERV_SHIP_DUNGEON
	//}}

	//{{ 2009. 7. 3  최육사		헤니르 시공
	if( SiCXSLDungeonManager()->GetDungeonType( iDungeonID ) == CXSLDungeon::DT_HENIR )
	{
		return true;
	}
	//}}

	//{{ 2009. 10. 26  최육사	PC방 혜택
	if( bIsPcBang == true )
	{
		return true;
	}
	//}}

	// [이벤트] 080611.hoons.근성도 이벤트 중일경우..
	if( SiKGameEventManager()->CheckSpiritEvent() == true )
		return true;
	//}}

	// 근성도가 남아있는지 검사
	if( GetSpirit() > 0 )
		return true;

	return false;
}

int	KUserSpiritManager::CalculateDecreaseSpirit( IN const char cUserCount, IN const short sStageNpcCount, IN const bool bIsTutorial, IN const bool bHalfDecreaseEvent, IN const bool bIsBattleField )
{
	//////////////////////////////////////////////////////////////////////////
	// 근성도 공식
	//
	// Y = (42-(2*X+A))/30*B
	// ( 몬스터가 한마리 죽을 때 모든 파티원의 근성도 감소량 = Y )
	// Y ≤ 1 의 경우, Y = 1로 처리 한다. 

	if( sStageNpcCount <= 0 )
		return 0;

	float fDecreaseSpirit = 0.0f;

	// 배틀 필드에서는 sStageNpcCount 상수 30으로 고정	2013-09-11	박세훈
	const float fCopyStageNpcCount = ( bIsBattleField == true ) ? 30.0f : static_cast<float>( sStageNpcCount );

	if( bHalfDecreaseEvent )
	{
		fDecreaseSpirit = (float)( 22 - ( 2 * static_cast<int>(cUserCount) + ( bIsTutorial ? 26 : 0 ) ) ) / fCopyStageNpcCount;
	}
	else
	{
		fDecreaseSpirit = (float)( 42 - ( 2 * static_cast<int>(cUserCount) + ( bIsTutorial ? 46 : 0 ) ) ) / fCopyStageNpcCount;
	}

	// 배틀필드라면 배틀필드 상수값을 곱하자!
	if( bIsBattleField )
	{
		//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
		fDecreaseSpirit = fDecreaseSpirit * SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor();
#else
		fDecreaseSpirit = fDecreaseSpirit * SiCXSLBattleFieldManager()->GetBattleFieldFactor();
#endif SERV_FIELD_ED_EXP_FACTOR
		//}
	}

	if( 0.f < fDecreaseSpirit  &&  fDecreaseSpirit < 1.f )
	{
		fDecreaseSpirit = 1.f;
	}
	else if( 0.f > fDecreaseSpirit  &&  fDecreaseSpirit > -1.f  )
	{
		fDecreaseSpirit = -1.f;
	}

	return static_cast<int>(fDecreaseSpirit);
}

#if defined( SERV_LOCAL_RANKING_SYSTEM ) || defined( SERV_CHINA_SPIRIT_EVENT )
bool KUserSpiritManager::DecreaseSpirit( IN const int iDungeonID
									   , IN const char cUserCount
									   , IN const short sStageNpcCount
									   , IN const bool bIsTutorial
									   , IN const bool bIsPcBang
									   , IN const bool bIsBattleField
									   , OUT int& iDecreaseSpirit
									   , OUT bool& bIsSpiritUpdated
									   , OUT int& iAbsoluteDecreaseSpirit
									   )
{
	iDecreaseSpirit = 0;
	bIsSpiritUpdated = false;

	// 소모시킬 근성도 계산
	//{{ 2011. 09. 23	최육사	공존의 축제 근성도 소모 이벤트
#ifdef SERV_COEXISTENCE_FESTIVAL
	iAbsoluteDecreaseSpirit = CalculateDecreaseSpirit( cUserCount, sStageNpcCount, bIsTutorial, true, bIsBattleField ); // [주의] 사제관계일경우 근성도가 보충된다.
#else
	iAbsoluteDecreaseSpirit = CalculateDecreaseSpirit( cUserCount, sStageNpcCount, bIsTutorial, false, bIsBattleField ); // [주의] 사제관계일경우 근성도가 보충된다.
#endif SERV_COEXISTENCE_FESTIVAL
	//}}
	const int iRemain = GetSpirit() - iAbsoluteDecreaseSpirit;
	if( iRemain < 0 )
	{
		iAbsoluteDecreaseSpirit += iRemain;
	}

	// 일반 던전인 경우
	if( bIsBattleField == false )
	{
		// 던전ID값이 유효한지 체크하자!
		if( iDungeonID == 0 )
		{
			START_LOG( cerr, L"던전ID값이 유효하지 않다!" )
				<< BUILD_LOG( iDungeonID )
				<< END_LOG;
			return false;
		}

		// 1. 근성도 소모와는 전혀 관계없는 던전인 경우를 체크하여 return함.
		if( CXSLDungeon::IsTutorialDungeon( iDungeonID )  ||
			CXSLDungeon::IsEventDungeon( iDungeonID ) )
		{
			return false;
		}

		//{{ 2010. 12. 30	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON
		if( CXSLDungeon::IsShipDungeon( iDungeonID ) )
		{
			return false;
		}
#endif SERV_SHIP_DUNGEON
		//}}

		// 2. 헤니르 시공은 근성도와는 관계 없다.	
		if( SiCXSLDungeonManager()->GetDungeonType( iDungeonID ) == CXSLDungeon::DT_HENIR )
		{
			return false;
		}
	}		

	// 3. 스테이지에 npc가 한마리도 없다면 근성도 소모 없음
	if( sStageNpcCount <= 0 )
	{
		return false;
	}

	// 5. 사제관계가 아닌경우 근성도 이벤트와 PC방혜택 검사
	if( bIsTutorial == false )
	{
		// [이벤트] 080611.hoons.근성도 이벤트 중일경우..
		if( SiKGameEventManager()->CheckSpiritEvent() == true )
		{
			return false;
		}

		//{{ 2009. 10. 26  최육사	PC방 혜택
		if( bIsPcBang == true )
		{
			return false;
		}
		//}}
	}

	// 실제 감소 수치에 반영
	iDecreaseSpirit = iAbsoluteDecreaseSpirit;

	// 근성도 소모 ( 사제관계일경우 증가처리 )
	m_iSpirit -= iDecreaseSpirit;

	// 근성도 크기 보정
	if( GetSpirit() <= 0 )
	{
		SetSpirit( 0 );

		// 근성도 하루치 모두 소모시 카운트 [한번 카운트 올라가면 다시 갱신안함]
		if( IsSpiritUpdated() == false )
		{
			SetIsSpiritUpdated( true );
		}
	}
	else if( GetSpirit() > SiKSpiritTable()->GetMaxSpirit() )
	{
		SetSpirit( SiKSpiritTable()->GetMaxSpirit() );
	}

	return true;
}
#else	// SERV_LOCAL_RANKING_SYSTEM
bool KUserSpiritManager::DecreaseSpirit( IN const int iDungeonID, 
										 IN const char cUserCount, 
					   					 IN const short sStageNpcCount, 
					   					 IN const bool bIsTutorial, 
										 IN const bool bIsPcBang,
					   					 IN const bool bIsBattleField,
										 OUT int& iDecreaseSpirit,
										 OUT bool& bIsSpiritUpdated
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
										 , OUT bool& bReward
#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
										 ,IN const bool bCharQuest
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
										 )
{
	iDecreaseSpirit = 0;
	bIsSpiritUpdated = false;

	// 일반 던전인 경우
	if( bIsBattleField == false )
	{
		// 던전ID값이 유효한지 체크하자!
		if( iDungeonID == 0 )
		{
			START_LOG( cerr, L"던전ID값이 유효하지 않다!" )
				<< BUILD_LOG( iDungeonID )
				<< END_LOG;
			return false;
		}

		// 1. 근성도 소모와는 전혀 관계없는 던전인 경우를 체크하여 return함.
		if( CXSLDungeon::IsTutorialDungeon( iDungeonID )  ||
			CXSLDungeon::IsEventDungeon( iDungeonID ) )
		{
			return false;
		}

		//{{ 2010. 12. 30	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON
		if( CXSLDungeon::IsShipDungeon( iDungeonID ) )
		{
			return false;
		}
#endif SERV_SHIP_DUNGEON
		//}}

		// 2. 헤니르 시공은 근성도와는 관계 없다.	
		if( SiCXSLDungeonManager()->GetDungeonType( iDungeonID ) == CXSLDungeon::DT_HENIR )
		{
			return false;
		}
	}		

	// 3. 스테이지에 npc가 한마리도 없다면 근성도 소모 없음
	if( sStageNpcCount <= 0 )
	{
		return false;
	}

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
	// 특정 서브 퀘스트 보유했을 때 근성도 깎는다.
	// 현재 퀘스트 관련 정보와 캐릭터 성별 정보 비교에서 같을 경우에만 아이템 지급
	if(true == bCharQuest)
	{
		CalculateAccumulationSpirit( cUserCount, sStageNpcCount, bIsTutorial, false, bIsBattleField, bReward );

		START_LOG( cout2, L"김석근_캐릭터 버닝이벤트_엘리오스의 짐승남" )
			<< BUILD_LOG( _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestSpiritQuest )
			<< END_LOG;
	}
#else //SERV_BURNING_CHAR_EVENT_SUB_QUEST
	IF_EVENT_ENABLED( CEI_ACCUMULATION_SPIRIT_EVENT )
	{
		CalculateAccumulationSpirit( cUserCount, sStageNpcCount, bIsTutorial, false, bIsBattleField, bReward );
	}	
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

	// 5. 사제관계가 아닌경우 근성도 이벤트와 PC방혜택 검사
	if( bIsTutorial == false )
	{
		// [이벤트] 080611.hoons.근성도 이벤트 중일경우..
		if( SiKGameEventManager()->CheckSpiritEvent() == true )
		{
			return false;
		}

		//{{ 2009. 10. 26  최육사	PC방 혜택
		if( bIsPcBang == true )
		{
			return false;
		}
		//}}
	}

	// 6. 소모시킬 근성도 계산
	//{{ 2011. 09. 23	최육사	공존의 축제 근성도 소모 이벤트
#ifdef SERV_COEXISTENCE_FESTIVAL
	iDecreaseSpirit = CalculateDecreaseSpirit( cUserCount, sStageNpcCount, bIsTutorial, true, bIsBattleField ); // [주의] 사제관계일경우 근성도가 보충된다.
#else
	iDecreaseSpirit = CalculateDecreaseSpirit( cUserCount, sStageNpcCount, bIsTutorial, false, bIsBattleField ); // [주의] 사제관계일경우 근성도가 보충된다.
#endif SERV_COEXISTENCE_FESTIVAL
	//}}
	const int iRemain = GetSpirit() - iDecreaseSpirit;
	if( iRemain < 0 )
	{
		iDecreaseSpirit += iRemain;
	}

	// 근성도 소모 ( 사제관계일경우 증가처리 )
	m_iSpirit -= iDecreaseSpirit;

	// 근성도 크기 보정
	if( GetSpirit() <= 0 )
	{
		SetSpirit( 0 );

		// 근성도 하루치 모두 소모시 카운트 [한번 카운트 올라가면 다시 갱신안함]
		if( IsSpiritUpdated() == false )
		{
			SetIsSpiritUpdated( true );
		}
	}
	else if( GetSpirit() > SiKSpiritTable()->GetMaxSpirit() )
	{
		SetSpirit( SiKSpiritTable()->GetMaxSpirit() );
	}

	return true;
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
void KUserSpiritManager::GetDBUpdateInfo2( OUT int& iAccumulationSpirit )
{
	iAccumulationSpirit = m_iAccumulationSpirit;
}

void KUserSpiritManager::CalculateAccumulationSpirit( IN char cUserCount, IN short sStageNpcCount, IN bool bIsTutorial, IN bool bHalfDecreaseEvent, IN bool bIsBattleField, OUT bool& bReward )
{
	int iDecreaseSpirit = CalculateDecreaseSpirit( cUserCount, sStageNpcCount, bIsTutorial, bHalfDecreaseEvent, bIsBattleField ); // [주의] 사제관계일경우 근성도가 보충된다.

	const int iRemain = GetSpirit() - iDecreaseSpirit;
	if( iRemain < 0 )
	{
		iDecreaseSpirit += iRemain;
	}

	if( iDecreaseSpirit > 0 )
	{
		m_iAccumulationSpirit += iDecreaseSpirit;

#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
		START_LOG( cout2, L"김석근_캐릭터 버닝이벤트_엘리오스의 짐승남 확인" )
			<< BUILD_LOG( m_iAccumulationSpirit )
			<< BUILD_LOG( iDecreaseSpirit )
			<< END_LOG;

		if( m_iAccumulationSpirit >= 3360 )					// 960은 최대 근성도의 70% 수치
		{			
			bReward = true;
			m_iAccumulationSpirit -= 3360;
		}
#else //SERV_BURNING_CHAR_EVENT_SUB_QUEST
		if( m_iAccumulationSpirit >= 960 )					// 960은 최대 근성도의 20% 수치
		{			
			bReward = true;
			m_iAccumulationSpirit -= 960;
		}
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST
	}
}
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
#endif SERV_BATTLE_FIELD_SYSTEM
//}}



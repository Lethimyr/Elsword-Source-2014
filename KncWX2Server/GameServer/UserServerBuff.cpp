#include "BaseServer.h"
#include ".\UserServerBuff.h"
#include "X2Data/XSLBuffManager.h"
#include "GSWorldMissionManager.h"


//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM


KUserServerBuff::KUserServerBuff( IN const KBuffInfo& kBuffInfo ) : 
m_iBuffID( kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID ),
m_iFactorID( kBuffInfo.m_kFactorInfo.m_iFactorID ),
m_fBuffPeriodTime( 0.0 )
{
}

KUserServerBuff::~KUserServerBuff(void)
{
}

void KUserServerBuff::InitPeriod( IN const CTime& tStartTime )
{
	const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( GetBuffID() );
	if( pBuffTemplet == NULL )
	{
		START_LOG( cerr, L"존재 하지 않는 버프입니다!" )
			<< BUILD_LOG( GetBuffID() )
			<< END_LOG;
		return;
	}

	// 기간제 버프인지 검사
	if( pBuffTemplet->m_fBuffPeriodSecond == 0.0f )
		return;

	const CTime tCurrentTime = CTime::GetCurrentTime();

	// 이 버프가 기간 버프인것을 구분하기 위해 기간정보를 저장한다.
	m_fBuffPeriodTime = pBuffTemplet->m_fBuffPeriodSecond;

	// 버프가 시작한 시각을 설정한다!
	if( tStartTime != 0 )
	{
		// 시작한 시각 정보가 따로 있으면 바로 저장!
		m_tStartTime = tStartTime;
	}
	else
	{
		// 없으면 현재 시각을 시작한 시각으로 저장!
		m_tStartTime = tCurrentTime;
	}
	
	// 외부에서 입력한 종료 시각이 있다면 그것을 사용하고, 그렇지 않다면 설정 해준다.
	switch( GetBuffID() )
	{
	case CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_1:
	case CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_2:
	case CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_3:
	case CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_4:
		{
			CTime tDefenseDungeonBuffStartTime;

			// 현재 어문의 종료 시간을 구한 다음에, 그걸 useTime과 비교해보자.
			if( SiKGSWorldMissionManager()->GetDefenseDungeonEndTime( tDefenseDungeonBuffStartTime, m_tEndTime ) == false )
			{
				START_LOG( cerr, L"디펜스 던전의 알맞은 종료 시점이 없습니다.")
					<< END_LOG;
				return;
			}

			if( m_tStartTime < tDefenseDungeonBuffStartTime )
			{
				return;
			}
			else if( ( m_tEndTime + CTimeSpan( static_cast<int>( pBuffTemplet->m_fBuffPeriodSecond ) ) ) <= m_tStartTime )
			{
				return;
			}

			m_tEndTime += CTimeSpan( static_cast<int>( pBuffTemplet->m_fBuffPeriodSecond ) );
		}
		break;

	default:
		{
			// 기본적으로는 시작 시간으로부터 지속시간을 더한 것이 종료 시간이 된다.
			m_tEndTime = m_tStartTime + CTimeSpan( ( __int64 )pBuffTemplet->m_fBuffPeriodSecond );
		}
		break;
	}

	// 시간 정보를 문자열로도 저장!
	m_wstrStartTime = m_tStartTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	m_wstrEndTime = m_tEndTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
}

void KUserServerBuff::GetBuffInfo( OUT KBuffInfo& kBuffInfo ) const
{
	kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID	= GetBuffID();
	kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = m_iFactorID;
	kBuffInfo.m_iBuffStartTime	= m_tStartTime.GetTime();
	kBuffInfo.m_iBuffEndTime	= m_tEndTime.GetTime();
}

bool KUserServerBuff::IsExpired() const
{
	CTime tCurTime = CTime::GetCurrentTime();
	return( tCurTime > m_tEndTime );
}


#endif SERV_SERVER_BUFF_SYSTEM
//}}



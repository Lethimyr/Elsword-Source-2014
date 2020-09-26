#include "GSUser.h"
#include ".\UserEDManager.h"


//{{ 2012. 09. 28	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM


KUserEDManager::KUserEDManager(void)
{
	Clear();
	m_iED.SetMaxValue( SEnum::UI_MAX_ED_CHAR_HAVE );
}

KUserEDManager::~KUserEDManager(void)
{
}

void KUserEDManager::Clear()
{
	m_iED.SetValue( 0 );
	ClearStatistics();
}

void KUserEDManager::ClearStatistics()
{
	m_mapEdStatistics.clear();
}

void KUserEDManager::Init( IN const int iED, IN const ED_REASON eReason )
{
	const int iBeforeED = GetED();

    m_iED.SetValue( iED );

	AddEdStatistics( iED - iBeforeED, eReason );
}

void KUserEDManager::GetDBUpdateInfo( OUT int& iED )
{
    iED = m_iED.GetChangeValue();
	m_iED.SetValue( m_iED );
}

void KUserEDManager::RollBackDBUpdateInfo( IN const int iED )
{
	m_iED.AdjustInitValue( m_iED.GetInitValue() - iED );
}

bool KUserEDManager::IncreaseMyED( IN const int iIncreaseED, IN const ED_REASON eReason )
{
	m_iED += iIncreaseED;

	AddEdStatistics( iIncreaseED, eReason );
	return true;
}

bool KUserEDManager::DecreaseMyED( IN const int iDecreaseED, IN const ED_REASON eReason )
{
    m_iED -= iDecreaseED;

	AddEdStatistics( iDecreaseED, eReason );
	return true;
}

void KUserEDManager::AddEdStatistics( IN const int iED, IN const ED_REASON eReason )
{
	if( eReason < 0  ||  eReason >= ER_MAX )
	{
        START_LOG( cerr, L"reason값이 이상합니다." )
			<< BUILD_LOG( iED )
			<< BUILD_LOG( eReason )
			<< END_LOG;
		return;
	}

	std::map< int, __int64 >::iterator mit;
	mit = m_mapEdStatistics.find( eReason );
	if( mit == m_mapEdStatistics.end() )
	{
		m_mapEdStatistics.insert( std::make_pair( eReason, iED ) );
	}
	else
	{
		mit->second += iED;
	}
}

#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}



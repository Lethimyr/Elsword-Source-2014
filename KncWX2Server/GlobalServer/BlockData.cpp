#include "BlockData.h"


//{{ 2013. 05. 30	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST


KBlockData::KBlockData() :
m_iBlockUID( 0 ),
m_cBlockType( 0 ),
m_iBlockTarget( 0 )
{
}

KBlockData::KBlockData( const KBlockInfo& kInfo ) : 
m_iBlockUID( 0 ),
m_cBlockType( 0 ),
m_iBlockTarget( 0 )
{
	SetBlockInfo( kInfo );
}

KBlockData::~KBlockData()
{
}

void KBlockData::GetBlockInfo( KBlockInfo& kInfo )
{
	kInfo.m_iBlockUID = m_iBlockUID;
	kInfo.m_cBlockType = m_cBlockType;
	kInfo.m_iBlockTarget = m_iBlockTarget;
	kInfo.m_wstrBlockTarget = m_wstrBlockTarget;
	kInfo.m_wstrStartDate = ( std::wstring )( m_tStartTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	kInfo.m_wstrEndDate = ( std::wstring )( m_tEndTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
}

void KBlockData::SetBlockInfo( const KBlockInfo& kInfo )
{
	m_iBlockUID = kInfo.m_iBlockUID;
	m_cBlockType = kInfo.m_cBlockType;
	m_iBlockTarget = kInfo.m_iBlockTarget;
	m_wstrBlockTarget = kInfo.m_wstrBlockTarget;

	if( kInfo.m_wstrStartDate.empty() == false )
	{
		if( KncUtil::ConvertStringToCTime( kInfo.m_wstrStartDate, m_tStartTime ) == false )
		{
			START_LOG( cerr, L"DB에서 받아온 날짜 정보값이 이상합니다." )
				<< BUILD_LOG( kInfo.m_wstrStartDate )
				<< END_LOG;
		}
	}

	if( kInfo.m_wstrEndDate.empty() == false )
	{
		if( KncUtil::ConvertStringToCTime( kInfo.m_wstrEndDate, m_tEndTime ) == false )
		{
			START_LOG( cerr, L"DB에서 받아온 날짜 정보값이 이상합니다." )
				<< BUILD_LOG( kInfo.m_wstrEndDate )
				<< END_LOG;
		}
	}
}

bool KBlockData::UpdateAndChanged( const KBlockInfo& kInfo )
{
	bool bChanged = false;
	CTime tStartTime;
	CTime tEndTime;

	if( m_iBlockUID != kInfo.m_iBlockUID )
	{
		bChanged = true;
		goto end_proc;
	}

	if( m_cBlockType != kInfo.m_cBlockType )
	{
		bChanged = true;
		goto end_proc;
	}

	if( m_wstrBlockTarget != kInfo.m_wstrBlockTarget )
	{
		bChanged = true;
		goto end_proc;
	}

	if( m_iBlockTarget != kInfo.m_iBlockTarget )
	{
		bChanged = true;
		goto end_proc;
	}
	
	LIF( KncUtil::ConvertStringToCTime( kInfo.m_wstrStartDate, tStartTime ) );
	if( m_tStartTime != tStartTime )
	{
		bChanged = true;
		goto end_proc;
	}
	
	LIF( KncUtil::ConvertStringToCTime( kInfo.m_wstrEndDate, tEndTime ) );
	if( m_tStartTime != tEndTime )
	{
		bChanged = true;
		goto end_proc;
	}

	SetBlockInfo( kInfo );
	return false;

end_proc:
	SetBlockInfo( kInfo );
    return true;
}

bool KBlockData::IsActiveBlock() const
{
	const CTime tCurTime = CTime::GetCurrentTime();

	return ( m_tStartTime < tCurTime  &&  tCurTime < m_tEndTime );
}


#endif SERV_BLOCK_LIST
//}}
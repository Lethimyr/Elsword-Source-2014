#include ".\userrecommendmanager.h"

KUserRecommendManager::KUserRecommendManager(void) :
m_bIsRecommend( false )
{
	Clear();
}

KUserRecommendManager::~KUserRecommendManager(void)
{
}

void KUserRecommendManager::Clear()
{
	// 유닛 단위 데이터
	m_iRecommendedUnitUID		= 0;
	m_bInitRecommendUserList	= false;
	m_vecRecommendUserList.clear();

#ifdef SERV_RECRUIT_EVENT_BASE
	m_bInitRecruitRecruiterList = false;
	m_vecRecruitUnitList.clear();
	m_vecRecruiterUnitList.clear();
#endif SERV_RECRUIT_EVENT_BASE
}

void KUserRecommendManager::SetRecommendUserList( IN std::vector< KRecommendUserInfo >& vecUserList )
{
	m_bInitRecommendUserList = true;
    m_vecRecommendUserList = vecUserList;
}

void KUserRecommendManager::GetRecommendUserList( IN OUT u_int& uiViewPage, OUT u_int& uiTotalPage, OUT std::vector< KRecommendUserInfo >& vecUserList )
{
	vecUserList.clear();
    
	_JIF( uiViewPage > 0, return; );

	if( m_vecRecommendUserList.empty() )
	{
		uiViewPage = 1;
		uiTotalPage = 1;
		return;
	}

	uiTotalPage = m_vecRecommendUserList.size() / PAGE_PER_COUNT;
	if( ( m_vecRecommendUserList.size() % PAGE_PER_COUNT ) > 0 )
	{
		++uiTotalPage;
	}

	u_int uiBeginIndex = ( uiViewPage * PAGE_PER_COUNT ) - PAGE_PER_COUNT;
	if( m_vecRecommendUserList.size() <= uiBeginIndex )
	{
		uiViewPage = uiTotalPage; // 마지막 페이지
		uiBeginIndex = ( uiViewPage * PAGE_PER_COUNT ) - PAGE_PER_COUNT;

		if( m_vecRecommendUserList.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"일어날 수 없는 에러!" )
				<< BUILD_LOG( uiViewPage )
				<< BUILD_LOG( uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecRecommendUserList.size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KRecommendUserInfo >::const_iterator vit = m_vecRecommendUserList.begin();
	vit += uiBeginIndex;

	while( vit != m_vecRecommendUserList.end() )
	{
		vecUserList.push_back( *vit );

		if( vecUserList.size() >= PAGE_PER_COUNT )
			break;

		++vit;
	}
}

void KUserRecommendManager::AddRecommendUser( IN const KRecommendUserInfo& kInfo )
{
	if( IsInitRecommendUserList() == false )
	{
		START_LOG( clog, L"추천인 리스트가 초기화 되지 않았는데 리스트에 추가하려한다." );
		return;
	}

    m_vecRecommendUserList.push_back( kInfo );
}

void KUserRecommendManager::ModifyRecommendUser( IN const KELG_RECOMMEND_USER_INFO_NOT& kInfo )
{
	if( IsInitRecommendUserList() == false )
	{
		START_LOG( clog, L"추천인 리스트가 초기화 되지 않았는데 리스트에 추가하려한다." );
		return;
	}

	std::vector< KRecommendUserInfo >::iterator vit = m_vecRecommendUserList.begin();
	for( ; vit != m_vecRecommendUserList.end(); ++vit )
	{
		if( vit->m_iUnitUID != kInfo.m_iRecommendUnitUID )
			continue;

        switch( kInfo.m_cType )
		{
		case KRecommendUserInfo::RUIT_LOGIN:
			{
                vit->m_bIsOnline = true;
			}
			break;

		case KRecommendUserInfo::RUIT_LOGOUT:
			{
				vit->m_bIsOnline = false;
			}
			break;

		case KRecommendUserInfo::RUIT_LEVEL_UP:
			{
                vit->m_ucLevel = static_cast<u_char>(kInfo.m_iData);
			}
			break;

		default:
			START_LOG( cerr, L"이상한 추천인 정보 타입이 왔습니다." )
				<< BUILD_LOG( kInfo.m_iRecommendedUnitUID )
				<< BUILD_LOG( kInfo.m_iRecommendUnitUID )
				<< BUILD_LOGc( kInfo.m_cType )
				<< BUILD_LOG( kInfo.m_iData )
				<< END_LOG;
		}

		break;
	}
}

#ifdef SERV_RECRUIT_EVENT_BASE
void KUserRecommendManager::ModifyRecruitRecruiterState( IN const KELG_RECRUIT_RECRUITER_INFO_NOT& kInfo )
{
	if( IsInitRecruitRecruiterList() == false )
	{
		START_LOG( clog, L"추천인 리스트가 초기화 되지 않았는데 리스트에 추가하려한다." );
		return;
	}

	std::vector< KRecommendUserInfo >::iterator vit;
	for( vit = m_vecRecruitUnitList.begin(); vit != m_vecRecruitUnitList.end(); ++vit )
	{
		if( vit->m_iUnitUID != kInfo.m_iReceiverUnitUID )
			continue;

		switch( kInfo.m_cType )
		{
		case KRecommendUserInfo::RUIT_LOGIN:
			{
				vit->m_bIsOnline = true;
			}
			break;

		case KRecommendUserInfo::RUIT_LOGOUT:
			{
				vit->m_bIsOnline = false;
			}
			break;

		case KRecommendUserInfo::RUIT_LEVEL_UP:
			{
				vit->m_ucLevel = static_cast<u_char>(kInfo.m_iData);
			}
			break;

		default:
			START_LOG( cerr, L"이상한 추천인 정보 타입이 왔습니다." )
				<< BUILD_LOG( kInfo.m_iSenderUnitUID )
				<< BUILD_LOG( kInfo.m_iReceiverUnitUID )
				<< BUILD_LOGc( kInfo.m_cType )
				<< BUILD_LOG( kInfo.m_iData )
				<< END_LOG;
			return;
		}

		break;
	}
	for( vit = m_vecRecruiterUnitList.begin(); vit != m_vecRecruiterUnitList.end(); ++vit )
	{
		if( vit->m_iUnitUID != kInfo.m_iReceiverUnitUID )
			continue;

		switch( kInfo.m_cType )
		{
		case KRecommendUserInfo::RUIT_LOGIN:
			{
				vit->m_bIsOnline = true;
			}
			break;

		case KRecommendUserInfo::RUIT_LOGOUT:
			{
				vit->m_bIsOnline = false;
			}
			break;

		case KRecommendUserInfo::RUIT_LEVEL_UP:
			{
				vit->m_ucLevel = static_cast<u_char>(kInfo.m_iData);
			}
			break;

		default:
			START_LOG( cerr, L"이상한 추천인 정보 타입이 왔습니다." )
				<< BUILD_LOG( kInfo.m_iSenderUnitUID )
				<< BUILD_LOG( kInfo.m_iReceiverUnitUID )
				<< BUILD_LOGc( kInfo.m_cType )
				<< BUILD_LOG( kInfo.m_iData )
				<< END_LOG;
			return;
		}
		break;
	}
}
#endif SERV_RECRUIT_EVENT_BASE


#include ".\guildad.h"
#include "NetError.h"
#include "Enum/Enum.h"


#ifdef SERV_GUILD_AD


KGuildAd::KGuildAd( const KGuildAdInfo& kGuildAdInfo ) :
m_iInitGuildExp( 0 )
{
	SetGuildAdInfo( kGuildAdInfo );
}

KGuildAd::~KGuildAd(void)
{
}

void KGuildAd::SetGuildAdInfo( IN const KGuildAdInfo& kGuildAdInfo )
{
	// 길드 광고 정보 업데이트
	m_kGuildAdInfo = kGuildAdInfo;

	// 길드 광고가 등록되는순간의 길드 경험치를 저장 ( 일주일간 가장 경험치가 많이 상승한 Hot Guild 리스트를 위해 )
	m_iInitGuildExp = kGuildAdInfo.m_iGuildEXP;

	//{{ 2010. 02. 04  최육사	길드 광고 오류 수정
	// 길드 창단일 날짜 세팅
	LIF( KncUtil::ConvertStringToCTime( kGuildAdInfo.m_wstrFoundingDay, m_tFoundingDay ) );
	//}}

	// 길드 광고 등록 날짜 세팅
	LIF( KncUtil::ConvertStringToCTime( kGuildAdInfo.m_wstrAdRegDate, m_tAdRegDate ) );

	// 길드 광고 종료 날짜 세팅
	LIF( KncUtil::ConvertStringToCTime( kGuildAdInfo.m_wstrAdEndDate, m_tAdEndDate ) );
}

void KGuildAd::InsertApplyJoin( IN const KApplyJoinGuildInfo& kInfo )
{
	// 길드 가입 신청 객체 생성
	KApplyJoinGuildInfoPtr spApplyInfo( new KApplyJoinGuildInfo( kInfo ) );

	// RegDate저장
	CTime tRegDate;
	KncUtil::ConvertStringToCTime( kInfo.m_wstrRegDate, tRegDate );
	spApplyInfo->m_tRegDate = static_cast<__int64>(tRegDate.GetTime());

	// map컨테이너에 넣기
	m_mapApplyJoinGuild.insert( std::make_pair( kInfo.m_iUnitUID, spApplyInfo ) );

	// vector컨테이너에 넣기
	std::vector< KApplyJoinGuildInfoPtr >::iterator vit;
	for( vit = m_vecApplyJoinGuild.begin(); vit != m_vecApplyJoinGuild.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->m_iUnitUID == kInfo.m_iUnitUID )
		{
			START_LOG( cerr, L"apply vector에 중복된 유저가 있다!" )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< END_LOG;

			// 스마트포인터를 덮어씌우자!
			(*vit) = spApplyInfo;
			break;
		}
	}

	if( vit == m_vecApplyJoinGuild.end() )
	{
		m_vecApplyJoinGuild.push_back( spApplyInfo );
	}

	// 정렬 한번 해주자!
	SortApplyJoinList();
}

void KGuildAd::DeleteApplyJoin( IN UidType iUnitUID )
{
	// map컨테이너에서 지우기
	m_mapApplyJoinGuild.erase( iUnitUID );

	// vector컨테이너에서 지우기
	std::vector< KApplyJoinGuildInfoPtr >::iterator vit;
	for( vit = m_vecApplyJoinGuild.begin(); vit != m_vecApplyJoinGuild.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->m_iUnitUID == iUnitUID )
			break;
	}

	if( vit != m_vecApplyJoinGuild.end() )
	{
		m_vecApplyJoinGuild.erase( vit );
	}

	// 정렬 한번 해주자!
	SortApplyJoinList();
}

void KGuildAd::UpdateApplyJoinInfo( IN const KApplyJoinGuildInfo& kInfo )
{
	KApplyJoinGuildInfoPtr spApplyJoinGuildInfo = GetApplyJoinGuildInfo( kInfo.m_iUnitUID );
	if( spApplyJoinGuildInfo == NULL )
	{
		START_LOG( cerr, L"길드 가입 신청자 정보가 없다!" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		return;
	}
    
	// 가입 신청자 정보 업데이트	
	spApplyJoinGuildInfo->m_cUnitClass		= kInfo.m_cUnitClass;
	spApplyJoinGuildInfo->m_ucLevel			= kInfo.m_ucLevel;
	spApplyJoinGuildInfo->m_wstrRegDate		= kInfo.m_wstrRegDate;	

	// RegDate저장
	CTime tRegDate;
	KncUtil::ConvertStringToCTime( kInfo.m_wstrRegDate, tRegDate );
	spApplyJoinGuildInfo->m_tRegDate = static_cast<__int64>(tRegDate.GetTime());
}

void KGuildAd::SortApplyJoinList()
{
	// [참고] 객체를 정렬하기 위해서는 std::stable_sort()함수를 써야함.

	std::stable_sort( m_vecApplyJoinGuild.begin(), m_vecApplyJoinGuild.end(), &KGuildAd::QuickOrder );
}

bool KGuildAd::QuickOrder( const KApplyJoinGuildInfoPtr spFirst, const KApplyJoinGuildInfoPtr spSecond )
{
	// NULL포인터 검사
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 등록순서가 빠른순
	if( spFirst->m_tRegDate > spSecond->m_tRegDate )
	{
		return true;
	}	

	return false;
}

KApplyJoinGuildInfoPtr KGuildAd::GetApplyJoinGuildInfo( IN UidType iUnitUID )
{
	std::map< UidType, KApplyJoinGuildInfoPtr >::iterator mit = m_mapApplyJoinGuild.find( iUnitUID );
	if( mit == m_mapApplyJoinGuild.end() )
	{
		START_LOG( cerr, L"길드 가입 신청 정보 찾기 실패!" )
			<< BUILD_LOG( iUnitUID );

		return KApplyJoinGuildInfoPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"길드 가입 신청 포인터 이상.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KApplyJoinGuildInfoPtr();
	}

	return mit->second;
}

bool KGuildAd::GetApplyJoinGuildInfo( IN UidType iUnitUID, OUT KApplyJoinGuildInfo& kInfo )
{
	KApplyJoinGuildInfoPtr spApplyJoinGuildInfo = GetApplyJoinGuildInfo( iUnitUID );
	if( spApplyJoinGuildInfo == NULL )
	{
		START_LOG( cerr, L"길드 가입 신청자 정보가 없다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

    kInfo = *spApplyJoinGuildInfo;
	return true;
}

bool KGuildAd::IsExpired()
{
	CTime tCurTime = CTime::GetCurrentTime();

	if( GetCTimeEndDate() < tCurTime )
		return true;

	return false;
}

bool KGuildAd::IsDeleteTime()
{
	CTime tCurTime = CTime::GetCurrentTime();

	// 기간 만료된 이후로 일주일이 지났는지 체크
	CTimeSpan tPassedTime = tCurTime - GetCTimeEndDate();
	if( tPassedTime.GetDays() > 7 )
		return true;

	return false;
}

void KGuildAd::NewApplyJoin( IN const KApplyJoinGuildInfo& kInfo )
{
	// 이미 가입된 유저인지 체크
	if( IsExistMyApplyJoinGuild( kInfo.m_iUnitUID ) == true )
	{
		START_LOG( cerr, L"이미 가입된 유저 입니다!" )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		// 기존 데이터 삭제 처리
		DeleteApplyJoin( kInfo.m_iUnitUID );
	}

	// 가입 신청
    InsertApplyJoin( kInfo );	
}

void KGuildAd::GetApplyJoinGuildList( IN const KELG_GET_APPLY_JOIN_GUILD_LIST_REQ& kReq, OUT KEGS_GET_APPLY_JOIN_GUILD_LIST_ACK& kAck )
{
	// 돌려줄 데이터 정리	
	kAck.m_vecApplyJoinGuildList.clear();	
	kAck.m_wstrGuildName = m_kGuildAdInfo.m_wstrGuildName;
	kAck.m_uiViewPage = kReq.m_uiViewPage;
	kAck.m_uiTotalPage = 1;	

	_JIF( kAck.m_uiViewPage > 0, return; );

	if( m_vecApplyJoinGuild.empty() )
	{
		kAck.m_uiViewPage = 1;
		kAck.m_uiTotalPage = 1;
		return;
	}

	kAck.m_uiTotalPage = m_vecApplyJoinGuild.size() / APPLY_JOIN_PAGE_PER_COUNT;
	if( ( m_vecApplyJoinGuild.size() % APPLY_JOIN_PAGE_PER_COUNT ) > 0 )
	{
		++kAck.m_uiTotalPage;
	}

	u_int uiBeginIndex = ( kAck.m_uiViewPage * APPLY_JOIN_PAGE_PER_COUNT ) - APPLY_JOIN_PAGE_PER_COUNT;
	if( m_vecApplyJoinGuild.size() <= uiBeginIndex )
	{
		kAck.m_uiViewPage = kAck.m_uiTotalPage; // 마지막 페이지
		uiBeginIndex = ( kAck.m_uiViewPage * APPLY_JOIN_PAGE_PER_COUNT ) - APPLY_JOIN_PAGE_PER_COUNT;

		if( m_vecApplyJoinGuild.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"일어날 수 없는 에러!" )
				<< BUILD_LOG( kAck.m_uiViewPage )
				<< BUILD_LOG( kAck.m_uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecApplyJoinGuild.size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KApplyJoinGuildInfoPtr >::const_iterator vit = m_vecApplyJoinGuild.begin();
	vit += uiBeginIndex;

	while( vit != m_vecApplyJoinGuild.end() )
	{
		if( (*vit) == NULL )
		{
			START_LOG( cerr, L"길드 가입 신청 정보 포인터가 이상합니다." )
				<< END_LOG;
			break;
		}

		kAck.m_vecApplyJoinGuildList.push_back( *(*vit) );

		if( kAck.m_vecApplyJoinGuildList.size() >= APPLY_JOIN_PAGE_PER_COUNT )
			break;

		++vit;
	}
}

void KGuildAd::GetApplyJoinGuildList( OUT std::vector< KApplyDeleteInfo >& vecApplyDelete )
{
	std::vector< KApplyJoinGuildInfoPtr >::const_iterator vit;
	for( vit = m_vecApplyJoinGuild.begin(); vit != m_vecApplyJoinGuild.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

        KApplyDeleteInfo kInfo;
		kInfo.m_iGuildUID = GetGuildUID();
		kInfo.m_iUnitUID = (*vit)->m_iUnitUID;
		vecApplyDelete.push_back( kInfo );
	}
}

//////////////////////////////////////////////////////////////////////////
// guild skill manager class
KGuildAd::GuildSkillInfoManager::GuildSkillInfoManager() :
m_bInitGuildSkillInfo( false )
{
}

KGuildAd::GuildSkillInfoManager::~GuildSkillInfoManager()
{
}

bool KGuildAd::GuildSkillInfoManager::GetGuildSkillInfo( OUT KGuildSkillInfo& kInfo )
{
    if( !m_bInitGuildSkillInfo )
		return false;

	kInfo = m_kGuildSkillInfo;
	return true;
}

void KGuildAd::GuildSkillInfoManager::SetGuildSkillInfo( IN const KGuildSkillInfo& kInfo )
{
	m_bInitGuildSkillInfo = true;
    m_kGuildSkillInfo = kInfo;	
}

//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
void KGuildAd::UpdateGuildName( IN const std::wstring& wstrGuildName )
{
	m_kGuildAdInfo.m_wstrGuildName = wstrGuildName;
}
#endif SERV_GUILD_CHANGE_NAME
//}}

#endif SERV_GUILD_AD
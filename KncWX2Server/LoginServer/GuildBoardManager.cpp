#include ".\guildBoardmanager.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "BaseServer.h"
#include "NetError.h"
#include "Enum/Enum.h"


#ifdef SERV_GUILD_AD


ImplementSingleton( KGuildBoardManager );

KGuildBoardManager::KGuildBoardManager(void)
{
	// lua
	lua_tinker::class_add<KGuildBoardManager>( g_pLua, "KGuildBoardManager" );
	lua_tinker::class_def<KGuildBoardManager>( g_pLua, "dump",					&KGuildBoardManager::Dump );

	lua_tinker::decl( g_pLua, "GuildBoardManager", this );
}

KGuildBoardManager::~KGuildBoardManager(void)
{
}

ImplToStringW( KGuildBoardManager )
{
	stm_ << L"------------------------------------------" << std::endl;

	stm_ << TOSTRINGW( m_mapGuildAd.size() );

	for( int i = 0; i < AST_MAX; ++i )
	{
		stm_ << TOSTRINGW( m_vecGuildAd[i].size() );
	}
	
	stm_ << TOSTRINGW( m_vecExpiredGuildAd.size() );
	stm_ << TOSTRINGW( m_mapApplyJoinGuildList.size() );

	return stm_;
}

void KGuildBoardManager::Init()
{
	// 게임DB로 가서 길드 광고 리스트를 얻자!
    SendToGameDB( DBE_GET_GUILD_BOARD_INFO_REQ );
}

void KGuildBoardManager::UpdateGuildBoardInfo( IN const KDBE_GET_GUILD_BOARD_INFO_ACK& kInfo )
{
	KDBE_DELETE_GUILD_AD_LIST_NOT kPacketNot;

	// 1. 길드 광고 정보 업데이트
	{
		std::vector< KGuildAdInfo >::const_iterator vit;
		for( vit = kInfo.m_vecGuildAdList.begin(); vit != kInfo.m_vecGuildAdList.end(); ++vit )
		{
			InsertGuildAd( *vit );
		}
	}

	// 2. 길드 가입 신청 정보 업데이트
	{
		std::vector< KApplyJoinGuildInfo >::const_iterator vitAJ = kInfo.m_vecApplyJoinGuildList.begin();
		for( ; vitAJ != kInfo.m_vecApplyJoinGuildList.end(); ++vitAJ )
		{
			if( InsertApplyJoinGuild( *vitAJ ) == false )
			{
				START_LOG( cerr, L"길드 가입 신청자 정보 업데이트 실패!" )
					<< BUILD_LOG( vitAJ->m_iApplyJoinGuildUID )
					<< BUILD_LOG( vitAJ->m_iUnitUID )
					<< END_LOG;

				//{{ 2010. 02. 10  최육사	강제 삭제 하지 말고 로그만 찍자
				//kPacketNot.m_vecDeleteApply.push_back( KApplyDeleteInfo( vitAJ->m_iApplyJoinGuildUID, vitAJ->m_iUnitUID ) );
				//}}
				continue;
			}
		}
	}

	if( !kPacketNot.m_vecDeleteApply.empty() )
	{
		START_LOG( cerr, L"길드 광고가 없어서 강제 삭제 처리된 가입 신청 리스트!" )
			<< BUILD_LOG( kPacketNot.m_vecDeleteApply.size() );

        SendToGameDB( DBE_DELETE_GUILD_AD_LIST_NOT, kPacketNot );
	}
	
	// 3. 길드 광고 삭제 체크
	CheckGuildAdDeleteTime( true );

	START_LOG( cout, L"길드 게시판 정보 업데이트 완료!" )
		<< BUILD_LOG( kInfo.m_vecGuildAdList.size() )
		<< BUILD_LOG( kInfo.m_vecApplyJoinGuildList.size() );
}

void KGuildBoardManager::Tick()
{
	// 길드 광고 기간 만료 체크
	CheckGuildAdPeriod();

	// 길드 광고 삭제 체크
	CheckGuildAdDeleteTime();

	// 길드 광고 정렬 체크
	CheckGuildAdHotGuildSort();
}

KGuildAdPtr KGuildBoardManager::GetGuildAd( IN int iGuildUID )
{
	std::map< int, KGuildAdPtr >::iterator mit = m_mapGuildAd.find( iGuildUID );
	if( mit == m_mapGuildAd.end() )
	{
		START_LOG( clog, L"길드 광고 정보 찾기 실패!" )
			<< BUILD_LOG( iGuildUID );

		return KGuildAdPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"길드 포인터 이상.!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return KGuildAdPtr();
	}

	return mit->second;
}

bool KGuildBoardManager::IsExpiredGuildAd( IN int iGuildUID )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"길드 광고 정보가 존재하지 않습니다." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

        return true;
	}

	return spGuildAd->IsExpired();
}

void KGuildBoardManager::InsertGuildAd( IN const KGuildAdInfo& kInfo )
{
	// 1. 기존에 남아있던 길드 광고 정보가 있는지 찾아보자!
	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_iGuildUID );
	if( spGuildAd == NULL )
	{
		// 없으면 새로 생성!
		spGuildAd = KGuildAdPtr( new KGuildAd( kInfo ) );

		m_mapGuildAd.insert( std::make_pair( kInfo.m_iGuildUID, spGuildAd ) );
	}
	else
	{
		// 있으면 덮어띄우자!
		spGuildAd->SetGuildAdInfo( kInfo );
	}

	// 2. 기간 만료된 광고인지 체크해서 만료되었다면 리스트에는 넣지 말자!
	if( spGuildAd->GetCTimeEndDate() < CTime::GetCurrentTime() )
	{
		START_LOG( clog, L"기간 만료된 광고라서 기간만료 리스트에 넣습니다." )
			<< BUILD_LOG( spGuildAd->GetGuildUID() );

		ExpiredGuildAd( kInfo.m_iGuildUID );
	}
	else
	{
		// 광고 리스트에 추가
		for( int iType = 0; iType < AST_MAX; ++iType )
		{
			std::vector< KGuildAdPtr >::iterator vit;
			for( vit = m_vecGuildAd[iType].begin(); vit != m_vecGuildAd[iType].end(); ++vit )
			{
				if( (*vit) == NULL )
					continue;

				if( (*vit)->GetGuildUID() == kInfo.m_iGuildUID )
				{
					START_LOG( cerr, L"guild vector에 중복된 guild가 있다!" )
						<< BUILD_LOG( kInfo.m_iGuildUID )
						<< END_LOG;

					// 스마트포인터를 덮어씌우자!
					(*vit) = spGuildAd;
					break;
				}
			}

			if( vit == m_vecGuildAd[iType].end() )
			{
				m_vecGuildAd[iType].push_back( spGuildAd );
			}
		}
	}

	// 3. 광고 리스트 정렬
	SortGuildAd();
}

void KGuildBoardManager::DeleteGuildAd( IN int iGuildUID )
{
	// 1. map컨테이너에서 지우기
	m_mapGuildAd.erase( iGuildUID );

	// 2. vector컨테이너에서 지우기
	for( int iType = 0; iType < AST_MAX; ++iType )
	{
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecGuildAd[iType].begin(); vit != m_vecGuildAd[iType].end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == iGuildUID )
				break;
		}

		if( vit != m_vecGuildAd[iType].end() )
		{
			m_vecGuildAd[iType].erase( vit );
		}
	}

	{
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecExpiredGuildAd.begin(); vit != m_vecExpiredGuildAd.end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == iGuildUID )
				break;
		}

		if( vit != m_vecExpiredGuildAd.end() )
		{
			m_vecExpiredGuildAd.erase( vit );
		}
	}

	// 3. 광고 리스트 정렬
	SortGuildAd();
}

void KGuildBoardManager::ExpiredGuildAd( IN int iGuildUID )
{
	// 1. 광고리스트에서 삭제
	for( int iType = 0; iType < AST_MAX; ++iType )
	{
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecGuildAd[iType].begin(); vit != m_vecGuildAd[iType].end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == iGuildUID )
				break;
		}

		if( vit != m_vecGuildAd[iType].end() )
		{
			m_vecGuildAd[iType].erase( vit );
		}
	}

	// 2. 기간만료 리스트에 추가
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"기간 만료된 광고 정보가 없습니다. 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;
		return;
	}

	{
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecExpiredGuildAd.begin(); vit != m_vecExpiredGuildAd.end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == iGuildUID )
			{
				START_LOG( cerr, L"기간 만료된 길드 광고 정보가 중복되면 안되는데 중복된게 있네?" )
					<< BUILD_LOG( iGuildUID )
					<< END_LOG;
				
				// 덮어씌운다.
				(*vit) = spGuildAd;
				return;
			}
		}

		if( vit == m_vecExpiredGuildAd.end() )
		{
			m_vecExpiredGuildAd.push_back( spGuildAd );
		}
	}

	// 3. 광고 리스트 정렬
	SortGuildAd();
}

void KGuildBoardManager::UnexpiredGuildAd( IN const KGuildAdInfo& kInfo )
{
	// 1. 길드 광고 정보 업데이트
	UpdateGuildAdInfo( kInfo );

	{ // 2. 기간 만료된 광고 리스트에서 뺀다.
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecExpiredGuildAd.begin(); vit != m_vecExpiredGuildAd.end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == kInfo.m_iGuildUID )
				break;
		}

		if( vit != m_vecExpiredGuildAd.end() )
		{
			m_vecExpiredGuildAd.erase( vit );
		}
	}

	// 3. 광고 리스트에 추가
	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"기간 만료된 광고 정보가 없습니다. 일어나서는 안되는 에러!" )
			<< BUILD_LOG( kInfo.m_iGuildUID )
			<< END_LOG;
		return;
	}
	
	for( int iType = 0; iType < AST_MAX; ++iType )
	{
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecGuildAd[iType].begin(); vit != m_vecGuildAd[iType].end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == kInfo.m_iGuildUID )
			{
				START_LOG( cerr, L"guild vector에 중복된 guild가 있다!" )
					<< BUILD_LOG( kInfo.m_iGuildUID )
					<< END_LOG;

				// 스마트포인터를 덮어씌우자!
				(*vit) = spGuildAd;
				break;
			}
		}

		if( vit == m_vecGuildAd[iType].end() )
		{
			m_vecGuildAd[iType].push_back( spGuildAd );
		}
	}

	// 4. 광고 리스트 정렬
	SortGuildAd();
}

void KGuildBoardManager::CheckGuildAdPeriod( bool bForce /*= false*/ )
{
	// 1분마다 길드 광고 기간 만료 체크
	if( m_kTimer[TM_CHECK_GUILD_AD_PERIOD].elapsed() < 60.0  &&  bForce == false )
		return;

	m_kTimer[TM_CHECK_GUILD_AD_PERIOD].restart();

	// 길드 광고 기간 체크	
	std::vector< int > vecEndGuildAd;

	std::vector< KGuildAdPtr >::const_iterator vit;
	for( vit = m_vecGuildAd[AST_MEMBER_COUNT].begin(); vit != m_vecGuildAd[AST_MEMBER_COUNT].end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->IsExpired() == true )
		{
			vecEndGuildAd.push_back( (*vit)->GetGuildUID() );
		}
	}

	// 기간이 만료되면 게시판에서만 삭제처리한다. ( map에는 그대로 보관 )
	std::vector< int >::const_iterator vitEG;
	for( vitEG = vecEndGuildAd.begin(); vitEG != vecEndGuildAd.end(); ++vitEG )
	{
		ExpiredGuildAd( *vitEG );
	}
}

void KGuildBoardManager::CheckGuildAdDeleteTime( bool bForce /*= false*/ )
{
	// 5분마다 길드 광고 삭제 체크
	if( m_kTimer[TM_CHECK_GUILD_AD_DELETE_TIME].elapsed() < 60.0 * 5.0  &&  bForce == false )
		return;

	m_kTimer[TM_CHECK_GUILD_AD_DELETE_TIME].restart();
	
	KDBE_DELETE_GUILD_AD_LIST_NOT kPacketNot;

	std::vector< KGuildAdPtr >::const_iterator vit;
	for( vit = m_vecExpiredGuildAd.begin(); vit != m_vecExpiredGuildAd.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->IsDeleteTime() == true )
		{
			// 삭제 대상에 해당 길드를 넣는다.
			kPacketNot.m_vecDeleteGuildAd.push_back( (*vit)->GetGuildUID() );
		}
	}
	
	std::vector< int >::const_iterator vitDG = kPacketNot.m_vecDeleteGuildAd.begin();
	for( ; vitDG != kPacketNot.m_vecDeleteGuildAd.end(); ++vitDG )
	{
		// 길드 가입 신청 리스트 삭제
		DeleteApplyJoinGuildList( *vitDG, kPacketNot.m_vecDeleteApply );

		// 길드 광고 삭제
		DeleteGuildAd( *vitDG );
	}

	if( !kPacketNot.m_vecDeleteGuildAd.empty() )
	{
		SendToGameDB( DBE_DELETE_GUILD_AD_LIST_NOT, kPacketNot );
	}
}

void KGuildBoardManager::CheckGuildAdHotGuildSort()
{
	// 1분마다 길드 HOT리스트 정렬
	if( m_kTimer[TM_SORT_GUILD_AD].elapsed() < 60.0 )
		return;

	m_kTimer[TM_SORT_GUILD_AD].restart();

	// 길드 광고 멤버 카운트 정렬
	std::stable_sort( m_vecGuildAd[AST_HOT_GUILD].begin(),		m_vecGuildAd[AST_HOT_GUILD].end(),		&KGuildBoardManager::HotGuildSort );
}

void KGuildBoardManager::SortGuildAd()
{
	// 리스트 다섯개를 소트해야 하므로 퍼포먼스를 생각하자.

	std::stable_sort( m_vecGuildAd[AST_REG_DATE].begin(),		m_vecGuildAd[AST_REG_DATE].end(),		&KGuildBoardManager::RegDateSort );
	std::stable_sort( m_vecGuildAd[AST_NEW_GUILD].begin(),		m_vecGuildAd[AST_NEW_GUILD].end(),		&KGuildBoardManager::NewGuildSort );
	std::stable_sort( m_vecGuildAd[AST_HOT_GUILD].begin(),		m_vecGuildAd[AST_HOT_GUILD].end(),		&KGuildBoardManager::HotGuildSort );
	std::stable_sort( m_vecGuildAd[AST_MEMBER_COUNT].begin(),	m_vecGuildAd[AST_MEMBER_COUNT].end(),	&KGuildBoardManager::MemberCountSort );
	std::stable_sort( m_vecGuildAd[AST_GUILD_LEVEL].begin(),	m_vecGuildAd[AST_GUILD_LEVEL].end(),	&KGuildBoardManager::GuildLevelSort );
}

bool KGuildBoardManager::RegDateSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond )
{
	// NULL포인터 검사
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 1. 등록 순서
	if( spFirst->GetCTimeRegDate() > spSecond->GetCTimeRegDate() )
	{
		return true;
	}

	return false;    
}

bool KGuildBoardManager::MemberCountSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond )
{
	// NULL포인터 검사
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 1. 인원이 많은 순
	if( spFirst->GetGuildNumMember() > spSecond->GetGuildNumMember() )
	{
		return true;
	}
	else if( spFirst->GetGuildNumMember() == spSecond->GetGuildNumMember() )
	{
		// 2. 인원이 같다면 등록 순서
		if( spFirst->GetCTimeRegDate() > spSecond->GetCTimeRegDate() )
		{
			return true;
		}
	}

	return false;
}

bool KGuildBoardManager::GuildLevelSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond )
{
	// NULL포인터 검사
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 1. 길드 레벨이 높은순
	if( spFirst->GetGuildLevel() > spSecond->GetGuildLevel() )
	{
		return true;
	}
	else if( spFirst->GetGuildLevel() == spSecond->GetGuildLevel() )
	{
		// 2. 길드 레벨이 같다면 등록 순서
		if( spFirst->GetCTimeRegDate() > spSecond->GetCTimeRegDate() )
		{
			return true;
		}
	}

	return false;
}

bool KGuildBoardManager::NewGuildSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond )
{
	// NULL포인터 검사
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 1. 길드 생성 순서가 가장 빠른 순
	if( spFirst->GetGuildFoundingDay() > spSecond->GetGuildFoundingDay() )
	{
		return true;
	}
	else if( spFirst->GetGuildFoundingDay() == spSecond->GetGuildFoundingDay() )
	{
		// 2. 같다면 등록 순서
		if( spFirst->GetCTimeRegDate() > spSecond->GetCTimeRegDate() )
		{
			return true;
		}
	}

	return false;
}

bool KGuildBoardManager::HotGuildSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond )
{
	// NULL포인터 검사
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 1. 길드 레벨업을 많이한 순서
	if( spFirst->GetGuildEXPUpCount() > spSecond->GetGuildEXPUpCount() )
	{
		return true;
	}
	else if( spFirst->GetGuildEXPUpCount() == spSecond->GetGuildEXPUpCount() )
	{
		// 2. 같다면 등록 순서
		if( spFirst->GetCTimeRegDate() > spSecond->GetCTimeRegDate() )
		{
			return true;
		}
	}

	return false;
}

bool KGuildBoardManager::IsExistApplyJoinGuild( IN int iGuildUID, IN UidType iUnitUID )
{
	std::map< UidType, int >::const_iterator mit = m_mapApplyJoinGuildList.find( iUnitUID );
	if( mit == m_mapApplyJoinGuildList.end() )
	{
		// 발생 가능한 예외 입니다.
		START_LOG( clog, L"존재하지않는 가입 신청입니다." )
			<< BUILD_LOG( iGuildUID )
			<< BUILD_LOG( iUnitUID );

		return false;
	}

	if( iGuildUID != mit->second )
	{
		START_LOG( cerr, L"가입 신청한 길드가 다릅니다. 있을수 없는 에러!" )
			<< BUILD_LOG( iGuildUID )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고에 가입 신청을 삭제하려고 하네?" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	if( spGuildAd->IsExistMyApplyJoinGuild( iUnitUID ) == false )
	{
		START_LOG( cerr, L"길드 광고에 가입 신청이 등록되어있지 않다!" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		// 유령 데이터 이므로 삭제 처리합니다.
		DeleteApplyJoinGuild( iUnitUID );
		return false;
	}

	return true;
}

bool KGuildBoardManager::GetMyApplyJoinGuildUID( IN UidType iUnitUID, OUT int& iGuildUID )
{
	iGuildUID = 0;

	std::map< UidType, int >::const_iterator mit = m_mapApplyJoinGuildList.find( iUnitUID );
	if( mit == m_mapApplyJoinGuildList.end() )
	{
		// 에러가 아니다.
		START_LOG( clog, L"존재하지않는 가입 신청입니다." )
			<< BUILD_LOG( iGuildUID )
			<< BUILD_LOG( iUnitUID );
		
		return false;
	}

	iGuildUID = mit->second;
	return true;
}

bool KGuildBoardManager::InsertApplyJoinGuild( IN const KApplyJoinGuildInfo& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_iApplyJoinGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고에 가입 신청을 하려고 하네?" )
			<< BUILD_LOG( kInfo.m_iApplyJoinGuildUID )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;
		
		return false;
	}

	// 길드에 가입 신청자 등록
	spGuildAd->NewApplyJoin( kInfo );

	// 가입 신청자 리스트에도 등록
	m_mapApplyJoinGuildList.insert( std::make_pair( kInfo.m_iUnitUID, kInfo.m_iApplyJoinGuildUID ) );
	return true;
}

bool KGuildBoardManager::DeleteApplyJoinGuild( IN UidType iUnitUID )
{
	std::map< UidType, int >::iterator mit = m_mapApplyJoinGuildList.find( iUnitUID );
	if( mit == m_mapApplyJoinGuildList.end() )
	{
		START_LOG( cerr, L"가입 신청한적이 없는데 가입신청을 지우려 합니다." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}
	
	KGuildAdPtr spGuildAd = GetGuildAd( mit->second );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고에 가입 신청을 삭제하려고 하네?" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( mit->second )
			<< END_LOG;

		return false;
	}

	// 가입 신청 삭제 처리
	spGuildAd->DeleteApplyJoin( iUnitUID );
	m_mapApplyJoinGuildList.erase( mit );
	return true;
}

bool KGuildBoardManager::DeleteApplyJoinGuildList( IN int iGuildUID, OUT std::vector< KApplyDeleteInfo >& vecApplyDeleteList )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고입니다." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	// 길드 가입 신청 리스트 얻기
	spGuildAd->GetApplyJoinGuildList( vecApplyDeleteList );

	// 길드 가입 신청 리스트 삭제 처리
	std::vector< KApplyDeleteInfo >::const_iterator vit;
	for( vit = vecApplyDeleteList.begin(); vit != vecApplyDeleteList.end(); ++vit )
	{
		DeleteApplyJoinGuild( vit->m_iUnitUID );
	}

	return true;
}

void KGuildBoardManager::NewGuildAd( IN const KGuildAdInfo& kInfo )
{
	if( IsExistGuildAd( kInfo.m_iGuildUID ) == true )
	{
		START_LOG( cerr, L"이미 등록되어있는 길드 광고를 등록하려 했습니다." )
			<< BUILD_LOG( kInfo.m_iGuildUID )
			<< END_LOG;

		// 기존 데이터 삭제
		DeleteGuildAd( kInfo.m_iGuildUID );
	}
    
	// 길드 광고 추가
    InsertGuildAd( kInfo );
}

bool KGuildBoardManager::ApplyJoinGuild( IN const KELG_APPLY_JOIN_GUILD_REQ& kInfo, OUT std::vector< KApplyDeleteInfo >& vecDeleteApply )
{
	vecDeleteApply.clear();

	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고에 가입 신청을 하려고 하네?" )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_BOARD_01 );
		return false;
	}

	// 기간이 다된 광고인지 확인
	if( spGuildAd->IsExpired() == true )
	{
		SET_ERROR( ERR_GUILD_BOARD_13 );
		return false;
	}

	// 가입 신청자 리스트가 100명인지 확인
	if( spGuildAd->GetApplyJoinNumber() >= KGuildAd::MAX_APPLY_LIST_COUNT )
	{
		SET_ERROR( ERR_GUILD_BOARD_16 );
		return false;
	}

	// 기존에 가입 신청 해놓은게 있는지 확인
	int iApplyJoinGuildUID = 0;
	if( GetMyApplyJoinGuildUID( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID, iApplyJoinGuildUID ) == true )
	{
		// 기존 가입 신청을 삭제 할지 여부
		if( kInfo.m_bOldApplyDelete )
		{
            if( DeleteApplyJoinGuild( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID ) == false )
			{
				START_LOG( cerr, L"기존 가입 신청 삭제 실패!" )
					<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID )
					<< END_LOG;

				SET_ERROR( ERR_GUILD_BOARD_08 );
				return false;
			}

			// DB에서 기존 기록 지우기 위해 등록
			KApplyDeleteInfo kDeleteInfo;
			kDeleteInfo.m_iGuildUID = iApplyJoinGuildUID;
			kDeleteInfo.m_iUnitUID = kInfo.m_kApplyJoinGuildInfo.m_iUnitUID;
			vecDeleteApply.push_back( kDeleteInfo );
		}
		else
		{
			SET_ERROR( ERR_GUILD_BOARD_09 );
			return false;
		}
	}

	// 가입 신청
    if( InsertApplyJoinGuild( kInfo.m_kApplyJoinGuildInfo ) == false )
	{
		START_LOG( cerr, L"가입 신청 실패!" )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID )
			<< BUILD_LOG( kInfo.m_bOldApplyDelete )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_BOARD_08 );
		return false;
	}
	
	return true;
}

void KGuildBoardManager::RollbackApplyJoinGuild( IN const KDBE_APPLY_JOIN_GUILD_ACK& kInfo )
{
    if( DeleteApplyJoinGuild( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"길드 가입 신청 롤백 실패!" )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID )
			<< END_LOG;
	}
}

void KGuildBoardManager::UpdateApplyJoinGuildInfo( IN const KDBE_APPLY_JOIN_GUILD_ACK& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"길드광고가 존재하지 않습니다. 있을수 없는에러!" )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID )
			<< END_LOG;		
		return;
	}

	// 가입 정보 업데이트
    spGuildAd->UpdateApplyJoinInfo( kInfo.m_kApplyJoinGuildInfo );
}

void KGuildBoardManager::GetGuildAdList( IN const KELG_GET_GUILD_AD_LIST_REQ& kReq, OUT KEGS_GET_GUILD_AD_LIST_ACK& kAck )
{
	// 돌려줄 데이터 정리
	const u_int SORT_TYPE = kReq.m_cSortType;
	kAck.m_vecGuildAdList.clear();
	kAck.m_cSortType = kReq.m_cSortType;
	kAck.m_uiViewPage = kReq.m_uiViewPage;
	kAck.m_uiTotalPage = 1;

	if( SORT_TYPE < 0  ||  SORT_TYPE >= AST_MAX )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고 타입입니다." )
			<< BUILD_LOGc( SORT_TYPE )
			<< END_LOG;
		return;
	}

	_JIF( kAck.m_uiViewPage > 0, return; );

	if( m_vecGuildAd[SORT_TYPE].empty() )
	{
		kAck.m_uiViewPage = 1;
		kAck.m_uiTotalPage = 1;
		return;
	}

	kAck.m_uiTotalPage = m_vecGuildAd[SORT_TYPE].size() / AE_PAGE_PER_COUNT;
	if( ( m_vecGuildAd[SORT_TYPE].size() % AE_PAGE_PER_COUNT ) > 0 )
	{
		++kAck.m_uiTotalPage;
	}

	u_int uiBeginIndex = ( kAck.m_uiViewPage * AE_PAGE_PER_COUNT ) - AE_PAGE_PER_COUNT;
	if( m_vecGuildAd[SORT_TYPE].size() <= uiBeginIndex )
	{
		kAck.m_uiViewPage = kAck.m_uiTotalPage; // 마지막 페이지
		uiBeginIndex = ( kAck.m_uiViewPage * AE_PAGE_PER_COUNT ) - AE_PAGE_PER_COUNT;

		if( m_vecGuildAd[SORT_TYPE].size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"일어날 수 없는 에러!" )
				<< BUILD_LOG( kAck.m_uiViewPage )
				<< BUILD_LOG( kAck.m_uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecGuildAd[SORT_TYPE].size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KGuildAdPtr >::const_iterator vit = m_vecGuildAd[SORT_TYPE].begin();
	vit += uiBeginIndex;

	while( vit != m_vecGuildAd[SORT_TYPE].end() )
	{
		if( (*vit) == NULL )
		{
			START_LOG( cerr, L"길드 광고 포인터가 이상합니다." )
				<< END_LOG;
			break;
		}

		kAck.m_vecGuildAdList.push_back( (*vit)->GetGuildAdInfo() );

		if( kAck.m_vecGuildAdList.size() >= AE_PAGE_PER_COUNT )
			break;

		++vit;
	}
}

bool KGuildBoardManager::GetGuildAdEndDate( IN int iGuildUID, OUT std::wstring& wstrEndDate )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )	
	{
		return false;
	}
    
	wstrEndDate = spGuildAd->GetGuildAdEndDate();
	return true;
}

bool KGuildBoardManager::GetApplyJoinGuildList( IN const KELG_GET_APPLY_JOIN_GUILD_LIST_REQ& kReq, OUT KEGS_GET_APPLY_JOIN_GUILD_LIST_ACK& kAck )
{
	KGuildAdPtr spGuildAd = GetGuildAd( kReq.m_iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고입니다." )
			<< BUILD_LOG( kReq.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_BOARD_01 );
		return false;
	}

	spGuildAd->GetApplyJoinGuildList( kReq, kAck );
	return true;
}

bool KGuildBoardManager::MakeGuildMemberInfoForAcceptJoinGuild( IN int iGuildUID, IN UidType iUnitUID, OUT KGuildMemberInfo& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고입니다." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;
		
		return false;
	}

	KApplyJoinGuildInfo kApplyInfo;
	if( spGuildAd->GetApplyJoinGuildInfo( iUnitUID, kApplyInfo ) == false )
	{
		START_LOG( cerr, L"길드 가입 신청 정보 얻기 실패!" )
			<< BUILD_LOG( iGuildUID )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	// 길드 멤버 정보 만들기
	kInfo.m_iUnitUID		  = kApplyInfo.m_iUnitUID;
	kInfo.m_wstrNickName	  = kApplyInfo.m_wstrNickName;
	kInfo.m_cUnitClass		  = kApplyInfo.m_cUnitClass;
	kInfo.m_ucLevel			  = kApplyInfo.m_ucLevel;
	kInfo.m_ucMemberShipGrade = SEnum::GUG_NEW_USER;	
	return true;
}

bool KGuildBoardManager::GetGuildSkillInfo( IN int iGuildUID, OUT KGuildSkillInfo& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고입니다." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}
	
	return spGuildAd->GetGuildSkillInfo( kInfo );
}

bool KGuildBoardManager::SetGuildSkillInfo( IN int iGuildUID, IN const KGuildSkillInfo& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고입니다." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuildAd->SetGuildSkillInfo( kInfo );
	return true;
}

bool KGuildBoardManager::DeleteGuildSkillInfo( IN int iGuildUID )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고입니다." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuildAd->DeleteGuildSkillInfo();
	return true;
}

bool KGuildBoardManager::UpdateGuildAdInfo( IN const KGuildAdInfo& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 길드 광고입니다." )
			<< BUILD_LOG( kInfo.m_iGuildUID )
			<< END_LOG;

		return false;
	}

	// 길드 광고 정보 갱신
    spGuildAd->SetGuildAdInfo( kInfo );
	return true;
}

void KGuildBoardManager::UpdateGuildEXP( IN int iGuildUID, IN int iGuildExp )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		return;
	}

	spGuildAd->UpdateGuildEXP( iGuildExp );	
}

void KGuildBoardManager::UpdateGuildLevelResult( IN int iGuildUID, IN u_char ucGuildLevel )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		return;
	}

	spGuildAd->UpdateGuildLevelResult( ucGuildLevel );

	// 길드 레벨 순으로 정렬
	std::stable_sort( m_vecGuildAd[AST_GUILD_LEVEL].begin(),	m_vecGuildAd[AST_GUILD_LEVEL].end(),	&KGuildBoardManager::GuildLevelSort );
}

void KGuildBoardManager::UpdateGuildMasterNickName( IN int iGuildUID, IN const std::wstring& wstrNickName )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		return;
	}

	spGuildAd->UpdateGuildMasterNickName( wstrNickName );
}

void KGuildBoardManager::UpdateGuildCurMemberCount( IN int iGuildUID, IN u_short usMemberCount )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		return;
	}

	spGuildAd->UpdateGuildCurMemberCount( usMemberCount );

	// 길드 광고 멤버 카운트 정렬
	std::stable_sort( m_vecGuildAd[AST_MEMBER_COUNT].begin(),	m_vecGuildAd[AST_MEMBER_COUNT].end(),	&KGuildBoardManager::MemberCountSort );
}

//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
void KGuildBoardManager::UpdateGuildName( IN const int& iGuildUID, IN const std::wstring& wstrGuildName )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		return;
	}

	spGuildAd->UpdateGuildName( wstrGuildName );
}
#endif SERV_GUILD_CHANGE_NAME
//}}

void KGuildBoardManager::SendToGameDB( unsigned short usEventID )
{
	SendToGameDB( usEventID, char() );
}


#endif SERV_GUILD_AD
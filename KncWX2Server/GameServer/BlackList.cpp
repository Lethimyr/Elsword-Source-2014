#include ".\blacklist.h"
#include "UserManager.h"
#include "GSUser.h"
#include "NetError.h"
#include "CommonPacket.h"


KBlackList::KBlackList(void)
: m_cInsertFriendDeny(BL_OFF), m_cManToManChatDeny(BL_OFF), m_cPartyDeny(BL_OFF), m_cPrivateDealDeny(BL_OFF)
{
	m_mapBlackListUid.clear();
    m_mapBlackListNick.clear();
}

KBlackList::~KBlackList(void)
{
}

bool KBlackList::SetInsertFriendDeny( char stateVal )
{
	// InsertFriendDeny는 BL_ON, BL_OFF만 해당됨
	if( stateVal >= BL_OFF )
		return false;

	m_cInsertFriendDeny = stateVal;
	return true;
}

bool KBlackList::IsAddPossible()
{
	SET_ERROR( NET_OK );

	if( m_kAddBlackListTimer.elapsed() < ADD_LIMIT_TIME )
	{
		SET_ERROR( ERR_BLACKLIST_00 );
		return false;
	}

	if( m_mapBlackListUid.size() >= MAX_BLACKLIST_USER )
	{
		SET_ERROR( ERR_BLACKLIST_01 );
		return false;
	}

	m_kAddBlackListTimer.restart();

	return true;
}

bool KBlackList::IsDelPossible()
{
	SET_ERROR( NET_OK );

	if( m_kDelBlackListTimer.elapsed() < DEL_LIMIT_TIME )
	{
		SET_ERROR( ERR_BLACKLIST_00 );
		return false;
	}

	m_kDelBlackListTimer.restart();

	return true;
}

bool KBlackList::IsUpdateOptPossible()
{
	SET_ERROR( NET_OK );

	if( m_kDenyOptionTimer.elapsed() < DEL_LIMIT_TIME )
	{
		SET_ERROR( ERR_BLACKLIST_02 );
		return false;
	}

	m_kDenyOptionTimer.restart();

	return true;	
}

void KBlackList::AddNewBlackListUser( UidType nUnitUID, std::wstring &wstrNickName )
{
	m_mapBlackListUid.insert( std::make_pair( nUnitUID, wstrNickName ) );
	m_mapBlackListNick.insert( std::make_pair( wstrNickName, nUnitUID ) );
}

bool KBlackList::DeleteBlackListUser( std::wstring &wstrNickName, UidType &unitUid )
{
	std::map< std::wstring, UidType >::iterator mit;
	mit = m_mapBlackListNick.find( wstrNickName );
	if( mit == m_mapBlackListNick.end() )
	{
		START_LOG( cerr, L"블랙 리스트 삭제 실패. 이미 삭제?" )
			<< BUILD_LOG( wstrNickName )
			<< END_LOG;

		return false;
	}

	unitUid = mit->second; // 삭제할 UID
	m_mapBlackListNick.erase( mit );

	std::map< UidType, std::wstring >::iterator mit2;
	mit2 = m_mapBlackListUid.find( unitUid );
	if( mit2 == m_mapBlackListUid.end() )
	{
		START_LOG( cerr, L"블랙 리스트 삭제 실패. 이미 삭제?" )
			<< BUILD_LOG( unitUid )
			<< END_LOG;

		return false;
	}
	
	m_mapBlackListUid.erase( mit2 );
	
	return true;
}

void KBlackList::SetBlackList( std::map< UidType, std::wstring > &mapBlackList )
{
	m_mapBlackListUid = mapBlackList;
	std::map< UidType, std::wstring >::iterator mit;
		
	for(mit = m_mapBlackListUid.begin(); mit != m_mapBlackListUid.end(); mit++ )
	{
		m_mapBlackListNick.insert( std::make_pair( mit->second, mit->first ) );
	}
}

bool KBlackList::VerifyBlackListUser( UidType nUnitUID )
{
	std::map< UidType, std::wstring >::iterator mit;
	mit = m_mapBlackListUid.find( nUnitUID );
	if( mit == m_mapBlackListUid.end() )
		return false;

	return true;
}

bool KBlackList::VerifyBlackListUser( std::wstring wstrNickName )
{
	std::map< std::wstring, UidType >::iterator mit;
	mit = m_mapBlackListNick.find( wstrNickName );
	if( mit == m_mapBlackListNick.end() )
		return false;

	return true;
}
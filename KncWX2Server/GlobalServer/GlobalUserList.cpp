#include "NetError.h"
#include "ActorManager.h"
#include "GlobalUser.h"
#include "SimLayer.h"
#include "BaseServer.h"
#include "GlobalUserList.h"

//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	#include "..\Common\PvpMatch\PvpMatchManager.h"
	#include "odbc\Odbc.h"
#endif	// SERV_HERO_PVP_MANAGE_LIST

KGlobalUserList::KGlobalUserList()
{
}

KGlobalUserList::~KGlobalUserList()
{
}

bool KGlobalUserList::GetGSUserInfoByUnitUID( IN const UidType iUnitUID, OUT KGSUserInfo& kInfo )
{
    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::const_iterator mit;
    mit = m_mapUnitUID.find( iUnitUID );
    if( mit == m_mapUnitUID.end() )
    {
        return false;
    }

    if( IS_NULL( mit->second ) )
    {
        START_LOG( cerr, L"포인터 이상." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;
        return false;
    }

    kInfo = *( mit->second );
    return true;
}

#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // 적용날짜: 2013-04-04
bool KGlobalUserList::GetGSUserInfoByUnitNickName( IN const UidType iServerGroupID, IN const std::wstring& wstrUnitNickName, OUT KGSUserInfo& kInfo ) const
{
#ifdef EXTEND_SERVER_GROUP_MASK
	if( m_mapUnitNickName.find( static_cast< int >( iServerGroupID ) ) == m_mapUnitNickName.end() )
#else EXTEND_SERVER_GROUP_MASK
	if( ( iServerGroupID <= SEnum::SGI_INVALID ) || ( SEnum::SGI_END <= iServerGroupID ) )
#endif EXTEND_SERVER_GROUP_MASK
	{
		START_LOG( cerr, L"잘못된 ServerGroupID 입니다." )
			<< BUILD_LOG( iServerGroupID )
			<< BUILD_LOG( wstrUnitNickName )
			<< END_LOG;
		return false;
	}

#ifdef EXTEND_SERVER_GROUP_MASK
	const std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >& mapUnitNickName = m_mapUnitNickName.find( static_cast< int >( iServerGroupID ) )->second;
#else EXTEND_SERVER_GROUP_MASK
	const std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >& mapUnitNickName = m_mapUnitNickName[iServerGroupID];
#endif EXTEND_SERVER_GROUP_MASK
	std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::const_iterator mit;
	mit = mapUnitNickName.find( wstrUnitNickName );
	if( mit == mapUnitNickName.end() )
	{
		return false;
	}

	if( IS_NULL( mit->second ) )
	{
		START_LOG( cerr, L"포인터 이상." )
			<< BUILD_LOG( wstrUnitNickName )
			<< END_LOG;

		return false;
	}

	kInfo = *( mit->second );
	return true;
}
#else	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
bool KGlobalUserList::GetGSUserInfoByUnitNickName( IN const std::wstring& wstrUnitNickName, OUT KGSUserInfo& kInfo )
{
    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::const_iterator mit;
    mit = m_mapUnitNickName.find( wstrUnitNickName );
    if( mit == m_mapUnitNickName.end() )
    {
        return false;
    }

    if( IS_NULL( mit->second ) )
    {
        START_LOG( cerr, L"포인터 이상." )
            << BUILD_LOG( wstrUnitNickName )
            << END_LOG;

        return false;
    }

    kInfo = *( mit->second );
    return true;
}
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE

//{{ 2013. 05. 29	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
bool KGlobalUserList::GetGSUserInfoByUserUID( IN const UidType iUserUID, OUT KGSUserInfo& kInfo )
{
	std::map< UidType, boost::shared_ptr< KGSUserInfo > >::const_iterator mit;
	mit = m_mapUserUID.find( iUserUID );
	if( mit == m_mapUserUID.end() )
	{
		return false;
	}

	if( IS_NULL( mit->second ) )
	{
		START_LOG( cerr, L"포인터 이상." )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;
		return false;
	}

	kInfo = *( mit->second );
	return true;
}
#endif SERV_BLOCK_LIST
//}}

void KGlobalUserList::UnRegAllUserOfThisGS( IN const UidType iGSUID )
{
    START_LOG( cout, L"게임 서버 죽은 후 처리." )
        << BUILD_LOG( iGSUID );

    if( iGSUID <= 0 )
    {
        START_LOG( cerr, L"서버 UID 이상." )
            << BUILD_LOG( iGSUID )
            << END_LOG;

        return;
    }

    std::vector< UidType > vecUnitUID;
    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitUID;
    for( mitUnitUID = m_mapUnitUID.begin(); mitUnitUID != m_mapUnitUID.end(); ++mitUnitUID )
    {
        if( IS_NULL( mitUnitUID->second ) )
        {
            START_LOG( cerr, L"유저 정보 포인터 이상." )
                << BUILD_LOG( mitUnitUID->first )
                << END_LOG;

            continue;
        }

        if( mitUnitUID->second->m_nGSUID == iGSUID )
        {
            vecUnitUID.push_back( mitUnitUID->first );
        }
    }

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUnitUID )
    {
        UnRegUnit( iUnitUID );
    }
}

int KGlobalUserList::RegUnit( IN const KRegUnitInfo& kUnitInfo )
{
    if( kUnitInfo.m_iUserUID <= 0 )
    {
        START_LOG( cerr, L"유저 UID 이상." )
            << BUILD_LOG( kUnitInfo.m_iUserUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_01;
    }

    if( kUnitInfo.m_iUnitUID <= 0 )
    {
        START_LOG( cerr, L"유닛 UID 이상." )
            << BUILD_LOG( kUnitInfo.m_iUserUID )
            << BUILD_LOG( kUnitInfo.m_iUnitUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_03;
    }

    if( kUnitInfo.m_wstrUnitNickName.empty() )
    {
        START_LOG( cerr, L"유닛 닉네임 이상." )
			<< BUILD_LOG( kUnitInfo.m_iUserUID )
			<< BUILD_LOG( kUnitInfo.m_iUnitUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_04;
    }

	//UID로 검색
    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitUID;
    mitUnitUID = m_mapUnitUID.find( kUnitInfo.m_iUnitUID );
    if( mitUnitUID != m_mapUnitUID.end() )
    {
        START_LOG( cerr, L"이미 등록되어 있는 UnitUID입니다! 설마 중복 선택 캐릭터 인가!! 일어나서는 안되는 에러!" )
            << BUILD_LOG( kUnitInfo.m_iUnitUID )
            << END_LOG;

        if( IS_NULL( mitUnitUID->second ) )
        {
            START_LOG( cerr, L"포인터 이상." )
                << BUILD_LOG( kUnitInfo.m_iUserUID )
                << BUILD_LOG( kUnitInfo.m_iUnitUID )
                << BUILD_LOG( kUnitInfo.m_wstrUnitNickName )
                << END_LOG;
        }

		// 기존에 저장되어있던 정보를 지웁니다!
		UnRegUnit( kUnitInfo.m_iUnitUID );
    }

	//닉네임 검색
#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // 적용날짜: 2013-04-04

#ifdef EXTEND_SERVER_GROUP_MASK
	const int iServerGroupID = static_cast< int >( KncUid::ExtractServerGroupID( kUnitInfo.m_iUnitUID ) );
#else EXTEND_SERVER_GROUP_MASK
	const UidType iServerGroupID = KncUid::ExtractServerGroupID( kUnitInfo.m_iUnitUID );
	if( ( iServerGroupID <= SEnum::SGI_INVALID ) || ( SEnum::SGI_END <= iServerGroupID ) )
	{
		START_LOG( cerr, L"잘못된 ServerGroupID 입니다." )
			<< BUILD_LOG( kUnitInfo.m_iUnitUID )
			<< BUILD_LOG( iServerGroupID )
			<< BUILD_LOG( kUnitInfo.m_wstrUnitNickName )
			<< END_LOG;
		return NetError::ERR_USER_REGISTRATION_03;
	}
#endif EXTEND_SERVER_GROUP_MASK

	std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >& mapUnitNickName = m_mapUnitNickName[iServerGroupID];
#else	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
	std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >& mapUnitNickName = m_mapUnitNickName;
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE

    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitNickName;
    mitUnitNickName = mapUnitNickName.find( kUnitInfo.m_wstrUnitNickName );
    if( mitUnitNickName != mapUnitNickName.end() )
    {
        START_LOG( cerr, L"UnReg 되지 않은 유닛 닉네임." )
            << BUILD_LOG( kUnitInfo.m_wstrUnitNickName )
            << END_LOG;

        if( IS_NULL( mitUnitNickName->second ) )
        {
            START_LOG( cerr, L"포인터 이상." )
                << BUILD_LOG( kUnitInfo.m_iUserUID )
                << BUILD_LOG( kUnitInfo.m_iUnitUID )
                << BUILD_LOG( kUnitInfo.m_wstrUnitNickName )
                << END_LOG;
        }
		else
		{
			if( mitUnitNickName->second->m_nUnitUID != kUnitInfo.m_iUnitUID )
			{
				START_LOG( cerr, L"캐릭터 정보가 이상합니다! 닉네임은 같은데 UnitUID가 다른 유저가 존재함! 일어나서는 안되는 에러!" )
					<< BUILD_LOG( mitUnitNickName->second->m_nUnitUID )
					<< BUILD_LOG( kUnitInfo.m_iUnitUID )
					<< BUILD_LOG( kUnitInfo.m_wstrUnitNickName )
					<< END_LOG;
			}
		}

		// 닉네임 정보를 지웁니다. ( UnitUID정보는 위에서 지웠을테니 참고 합니다.
        mapUnitNickName.erase( mitUnitNickName );
    }

	// 캐릭터 정보 추가
	boost::shared_ptr< KGSUserInfo > spGSUserInfo( new KGSUserInfo() );
	spGSUserInfo->m_nUserUID			= kUnitInfo.m_iUserUID;
	spGSUserInfo->m_nUnitUID			= kUnitInfo.m_iUnitUID;
	spGSUserInfo->m_wstrUnitNickName	= kUnitInfo.m_wstrUnitNickName;
	spGSUserInfo->m_nGSUID				= kUnitInfo.m_iGSUID;
	spGSUserInfo->m_iChannelID			= kUnitInfo.m_iChannelID;
	spGSUserInfo->m_wstrIP				= kUnitInfo.m_wstrIP;
	spGSUserInfo->m_cAuthLevel			= kUnitInfo.m_cAuthLevel;
	m_mapUnitUID.insert( std::make_pair( kUnitInfo.m_iUnitUID, spGSUserInfo ) );

	// 닉네임 정보 추가
    mapUnitNickName.insert( std::make_pair( kUnitInfo.m_wstrUnitNickName, spGSUserInfo ) );

	//{{ 2013. 05. 29	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
	m_mapUserUID.insert( std::make_pair( kUnitInfo.m_iUserUID, spGSUserInfo ) );
#endif SERV_BLOCK_LIST
	//}}
#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	if( kUnitInfo.m_cAuthLevel <= SEnum::UAL_DEVELOPER )
	{
		AddHeroPvpObserver( kUnitInfo.m_iGSUID, kUnitInfo.m_iUnitUID );
	}

	HeroPvpUserOn( KncUid::ExtractServerGroupID( kUnitInfo.m_iUnitUID ), kUnitInfo.m_wstrUnitNickName );
#endif	// SERV_HERO_PVP_MANAGE_LIST

    return NetError::NET_OK;
}

bool KGlobalUserList::UnRegUnit( IN const UidType iUnitUID )
{
    if( iUnitUID <= 0 )
    {
        START_LOG( cerr, L"UnitUID 이상." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;
        return false;
    }

    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitUID;
    mitUnitUID = m_mapUnitUID.find( iUnitUID );
    if( mitUnitUID == m_mapUnitUID.end() )
    {
        START_LOG( cerr, L"삭제 하려는 유닛이 없습니다!" )
            << BUILD_LOG( iUnitUID )
            << END_LOG;
        return false;
    }

    if( IS_NULL( mitUnitUID->second ) )
    {
        START_LOG( cerr, L"포인터 이상." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return false;
    }

    if( mitUnitUID->second->m_wstrUnitNickName.empty() )
    {
        START_LOG( cerr, L"닉네임은 등록되지 않음." )
            << BUILD_LOG( iUnitUID )
            << BUILD_LOG( mitUnitUID->second->m_nUnitUID )
            << END_LOG;
    }

	//{{ 2013. 05. 29	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
	m_mapUserUID.erase( mitUnitUID->second->m_nUserUID );
#endif SERV_BLOCK_LIST
	//}}

#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // 적용날짜: 2013-04-04
#ifdef EXTEND_SERVER_GROUP_MASK
	const int iServerGroupID = KncUid::ExtractServerGroupID( iUnitUID );
#else EXTEND_SERVER_GROUP_MASK
	const UidType iServerGroupID = KncUid::ExtractServerGroupID( iUnitUID );
	if( ( iServerGroupID <= SEnum::SGI_INVALID ) || ( SEnum::SGI_END <= iServerGroupID ) )
	{
		START_LOG( cerr, L"잘못된 ServerGroupID 입니다." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iServerGroupID )
			<< END_LOG;
		return false;
	}
#endif EXTEND_SERVER_GROUP_MASK

	std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >& mapUnitNickName = m_mapUnitNickName[iServerGroupID];
#else	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
	std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >& mapUnitNickName = m_mapUnitNickName;
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitNickName;
    if( !mitUnitUID->second->m_wstrUnitNickName.empty() )
    {
        mitUnitNickName = mapUnitNickName.find( mitUnitUID->second->m_wstrUnitNickName );
        if( mitUnitNickName == mapUnitNickName.end() )
        {
            START_LOG( cerr, L"유닛 닉네임으로 검색 실패." )
                << BUILD_LOG( mitUnitUID->second->m_wstrUnitNickName )
                << END_LOG;
        }
        else
        {
            if( IS_NULL( mitUnitNickName->second ) )
            {
                START_LOG( cerr, L"포인터 이상." )
                    << BUILD_LOG( mitUnitUID->second->m_wstrUnitNickName )
                    << END_LOG;
            }
            else
            {
                LIF( mitUnitNickName->second->m_nUnitUID == iUnitUID );
            }

			// 닉네임 정보를 지우자!
            mapUnitNickName.erase( mitUnitNickName );
        }
    }

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	if( mitUnitUID->second->m_cAuthLevel <= SEnum::UAL_DEVELOPER )
	{
		RemoveHeroPvpObserver( mitUnitUID->second->m_nUnitUID );
	}

	HeroPvpUserOff( KncUid::ExtractServerGroupID( mitUnitUID->second->m_nUnitUID ), mitUnitUID->second->m_wstrUnitNickName );
#endif	// SERV_HERO_PVP_MANAGE_LIST

	// 캐릭터 정보를 지우자!
	m_mapUnitUID.erase( mitUnitUID );

    return true;
}

//{{ 2012. 11. 21	박세훈	영웅 대전 참가자 관리용 리스트
#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
#define DEF_ArgChk_HeroPvpUserServerGroupID( iServerGroupID, iRet ) \
{ \
	iRet = ArgChk_HeroPvpUserServerGroupID( iServerGroupID ); \
	if( iRet == NetError::ERR_HERO_PVP_USER_LIST_01 ) \
	{ \
		START_LOG( cerr, L"서버 그룹 아이디가 잘못되었습니다.") \
			<< BUILD_LOG( iServerGroupID ) \
			<< END_LOG; \
	} \
}

#define DEF_ArgChk_HeroPvpUserNickName( wstrNickName, iRet ) \
{ \
	iRet = ArgChk_HeroPvpUserNickName( wstrNickName ); \
	if( iRet == NetError::ERR_HERO_PVP_USER_LIST_03 ) \
	{ \
		START_LOG( cerr, L"닉네임 길이가 잘못되었습니다.") \
		<< BUILD_LOG( wstrNickName ) \
		<< END_LOG; \
	} \
	else if( iRet == NetError::ERR_HERO_PVP_USER_LIST_04 ) \
	{ \
		START_LOG( cerr, L"--/;/따옴표/쉼표/공백 등은 사용할 수 없습니다.") \
		<< BUILD_LOG( wstrNickName ) \
		<< END_LOG; \
	} \
}

int KGlobalUserList::FindHeroPvpUnit( IN const UidType iServerGroupID, IN const std::wstring wstrNickName ) const
{
	// 인자 검사
	int iOK;
	DEF_ArgChk_HeroPvpUserServerGroupID( iServerGroupID, iOK );
	if( iOK != NetError::NET_OK )
	{
		return iOK;
	}

	DEF_ArgChk_HeroPvpUserNickName( wstrNickName, iOK );
	if( iOK != NetError::NET_OK )
	{
		return iOK;
	}

	HERO_PVP_USER_TYPE::const_iterator it;
	for( it = m_vecHeroPvpUserList.begin(); it != m_vecHeroPvpUserList.end(); ++it )
	{
		if( it->m_iServerGroupID != iServerGroupID )
		{
			continue;
		}

		if( it->m_wstrNickName != wstrNickName )
		{
			continue;
		}

		return NetError::NET_OK;
	}

	return NetError::ERR_HERO_PVP_USER_LIST_05;	// 영웅 대전 후보 리스트에 존재하지 않는 유닛입니다.
}

int KGlobalUserList::AddHeroPvpUser( IN const UidType iServerGroupID, IN const std::wstring wstrNickName )
{
	// 인자 검사
	int iOK;
	DEF_ArgChk_HeroPvpUserServerGroupID( iServerGroupID, iOK );
	if( iOK != NetError::NET_OK )
	{
		return iOK;
	}

	DEF_ArgChk_HeroPvpUserNickName( wstrNickName, iOK );
	if( iOK != NetError::NET_OK )
	{
		return iOK;
	}

	iOK = FindHeroPvpUnit( iServerGroupID, wstrNickName );
	if( iOK == NetError::NET_OK )
	{
		START_LOG( cerr, L"영웅 대전 후보 리스트에 이미 존재하는 유닛입니다." )
			<< BUILD_LOG( iServerGroupID )
			<< BUILD_LOG( wstrNickName )
			<< END_LOG;
		return NetError::ERR_HERO_PVP_USER_LIST_06;	// 영웅 대전 후보 리스트에 이미 존재하는 유닛입니다.
	}
	else if( iOK != NetError::ERR_HERO_PVP_USER_LIST_05 )
	{
		return iOK;
	}

	KGSUserInfo kGSUserInfo;
	KHeroPvpUserInfo kInfo;
	kInfo.m_iServerGroupID	= iServerGroupID;
	kInfo.m_wstrNickName	= wstrNickName;
	kInfo.m_bOn				= GetGSUserInfoByUnitNickName( iServerGroupID, wstrNickName, kGSUserInfo );
	
	m_vecHeroPvpUserList.push_back( kInfo );

	// 변경 정보 저장
	const KHeroPvpUserInfo& kHeroPvpUserInfo = kInfo;
	std::vector< std::pair< bool, KHeroPvpUserInfo > >::iterator itChange;

	for( itChange = m_vecHeroPvpUserChangeInfo.begin(); itChange != m_vecHeroPvpUserChangeInfo.end(); ++itChange )
	{
		if( itChange->second.m_iServerGroupID != iServerGroupID )
		{
			continue;
		}

		if( itChange->second.m_wstrNickName != wstrNickName )
		{
			continue;
		}

		// 저장 정보가 있다면, 추가 체크
        itChange->first = true;
		break;
	}

	if( itChange == m_vecHeroPvpUserChangeInfo.end() )
	{
		// 저장 정보가 없다면, 추가 체크 및 현재 접속 여부 기록
		m_vecHeroPvpUserChangeInfo.push_back( std::make_pair( true, kHeroPvpUserInfo ) );
	}

	return NetError::NET_OK;
}

int KGlobalUserList::RemoveHeroPvpUser( IN const UidType iServerGroupID, IN const std::wstring wstrNickName )
{
	// 인자 검사
	int iOK;
	DEF_ArgChk_HeroPvpUserServerGroupID( iServerGroupID, iOK );
	if( iOK != NetError::NET_OK )
	{
		return iOK;
	}

	DEF_ArgChk_HeroPvpUserNickName( wstrNickName, iOK );
	if( iOK != NetError::NET_OK )
	{
		return iOK;
	}

	HERO_PVP_USER_TYPE::iterator it;
	for( it = m_vecHeroPvpUserList.begin(); it != m_vecHeroPvpUserList.end(); ++it )
	{
		if( it->m_iServerGroupID != iServerGroupID )
		{
			continue;
		}

		if( it->m_wstrNickName != wstrNickName )
		{
			continue;
		}

		m_vecHeroPvpUserList.erase( it );
		break;
	}

	// 변경 정보 저장
	std::vector< std::pair< bool, KHeroPvpUserInfo > >::iterator itChange;
	for( itChange = m_vecHeroPvpUserChangeInfo.begin(); itChange != m_vecHeroPvpUserChangeInfo.end(); ++itChange )
	{
		if( itChange->second.m_iServerGroupID != iServerGroupID )
		{
			continue;
		}

		if( itChange->second.m_wstrNickName != wstrNickName )
		{
			continue;
		}

		// 저장 정보가 있다면, 삭제 체크
		itChange->first = false;
		break;
	}

	if( itChange == m_vecHeroPvpUserChangeInfo.end() )
	{
		// 저장 정보가 없다면, 삭제 체크
		KHeroPvpUserInfo kHeroPvpUserInfo;
		kHeroPvpUserInfo.m_iServerGroupID	= iServerGroupID;
		kHeroPvpUserInfo.m_wstrNickName		= wstrNickName;
		m_vecHeroPvpUserChangeInfo.push_back( std::make_pair( false, kHeroPvpUserInfo ) );
	}
	
	return NetError::NET_OK;
}

void KGlobalUserList::AddHeroPvpUserList( IN const std::vector< std::pair< UidType, std::wstring > >& vecAddUserList
										, OUT std::vector< std::pair< UidType, std::wstring > >& vecAddResultUserList
										)
{
	std::vector< std::pair< UidType, std::wstring > >::const_iterator it;
	for( it = vecAddUserList.begin(); it != vecAddUserList.end(); ++it )
	{
		if( FindHeroPvpUnit( it->first, it->second ) == NetError::NET_OK )
		{
			continue;
		}

		int iOK = AddHeroPvpUser( it->first, it->second );
		if( ( iOK == NetError::NET_OK ) || ( iOK == NetError::ERR_HERO_PVP_USER_LIST_06 ) )
		{
			vecAddResultUserList.push_back( std::make_pair( it->first, it->second ) );
			continue;
		}
	}
}

void KGlobalUserList::RemoveHeroPvpUserList( IN const std::vector< std::pair< UidType, std::wstring > >& vecRemoveUserList
										   , OUT std::vector< std::pair< UidType, std::wstring > >& vecRemoveResultUserList
										   )
{
	std::vector< std::pair< UidType, std::wstring > >::const_iterator it;
	for( it = vecRemoveUserList.begin(); it != vecRemoveUserList.end(); ++it )
	{
		if( RemoveHeroPvpUser( it->first, it->second ) == NetError::NET_OK )
		{
			vecRemoveResultUserList.push_back( std::make_pair( it->first, it->second ) );
		}
	}
}

void KGlobalUserList::HeroPvpUserOn( IN const UidType iServerGroupID, IN const std::wstring wstrNickName )
{
	// 인자 검사
	int iOK;
	DEF_ArgChk_HeroPvpUserServerGroupID( iServerGroupID, iOK );
	if( iOK != NetError::NET_OK )
	{
		return;
	}

	DEF_ArgChk_HeroPvpUserNickName( wstrNickName, iOK );
	if( iOK != NetError::NET_OK )
	{
		return;
	}

	HERO_PVP_USER_TYPE::iterator it;
	for( it = m_vecHeroPvpUserList.begin(); it != m_vecHeroPvpUserList.end(); ++it )
	{
		if( it->m_iServerGroupID != iServerGroupID )
		{
			continue;
		}

		if( it->m_wstrNickName != wstrNickName )
		{
			continue;
		}

		it->m_bOn = true;
		break;
	}

	if( it == m_vecHeroPvpUserList.end() )
	{
		return;
	}

	// 변경 정보 저장
	const KHeroPvpUserInfo& kHeroPvpUserInfo = *it;
	std::vector< std::pair< bool, KHeroPvpUserInfo > >::iterator itChange;

	for( itChange = m_vecHeroPvpUserChangeInfo.begin(); itChange != m_vecHeroPvpUserChangeInfo.end(); ++itChange )
	{
		if( itChange->second.m_iServerGroupID != iServerGroupID )
		{
			continue;
		}

		if( itChange->second.m_wstrNickName != wstrNickName )
		{
			continue;
		}

		// 저장 정보가 있다면, On 체크
		itChange->second.m_bOn = true;
		break;
	}

	if( itChange == m_vecHeroPvpUserChangeInfo.end() )
	{
		// 저장 정보가 없다면, 추가 체크 후 On 기록
		m_vecHeroPvpUserChangeInfo.push_back( std::make_pair( true, kHeroPvpUserInfo ) );
	}
}

void KGlobalUserList::HeroPvpUserOff( IN const UidType iServerGroupID, IN const std::wstring wstrNickName )
{
	// 인자 검사
	int iOK;
	DEF_ArgChk_HeroPvpUserServerGroupID( iServerGroupID, iOK );
	if( iOK != NetError::NET_OK )
	{
		return;
	}

	DEF_ArgChk_HeroPvpUserNickName( wstrNickName, iOK );
	if( iOK != NetError::NET_OK )
	{
		return;
	}

	HERO_PVP_USER_TYPE::iterator it;
	for( it = m_vecHeroPvpUserList.begin(); it != m_vecHeroPvpUserList.end(); ++it )
	{
		if( it->m_iServerGroupID != iServerGroupID )
		{
			continue;
		}

		if( it->m_wstrNickName != wstrNickName )
		{
			continue;
		}

		it->m_bOn = false;
		break;
	}

	if( it == m_vecHeroPvpUserList.end() )
	{
		return;
	}

	// 변경 정보 저장
	const KHeroPvpUserInfo& kHeroPvpUserInfo = *it;
	std::vector< std::pair< bool, KHeroPvpUserInfo > >::iterator itChange;

	for( itChange = m_vecHeroPvpUserChangeInfo.begin(); itChange != m_vecHeroPvpUserChangeInfo.end(); ++itChange )
	{
		if( itChange->second.m_iServerGroupID != iServerGroupID )
		{
			continue;
		}

		if( itChange->second.m_wstrNickName != wstrNickName )
		{
			continue;
		}

		// 저장 정보가 있다면, Off 체크
		itChange->second.m_bOn = false;
		break;
	}

	if( itChange == m_vecHeroPvpUserChangeInfo.end() )
	{
		// 저장 정보가 없다면, 추가 체크 후 Off 기록
		m_vecHeroPvpUserChangeInfo.push_back( std::make_pair( true, kHeroPvpUserInfo ) );
	}
}

int KGlobalUserList::ArgChk_HeroPvpUserServerGroupID( IN const UidType iServerGroupID ) const
{
#ifdef EXTEND_SERVER_GROUP_MASK
	if( iServerGroupID <= -1 )
#else EXTEND_SERVER_GROUP_MASK
	if( ( iServerGroupID <= SEnum::SGI_INVALID ) || ( SEnum::SGI_END <= iServerGroupID ) )
#endif EXTEND_SERVER_GROUP_MASK
	{
		return NetError::ERR_HERO_PVP_USER_LIST_01;
	}

	return NetError::NET_OK;
}

int KGlobalUserList::ArgChk_HeroPvpUserNickName( IN const std::wstring wstrNickName ) const
{
	if( ( wstrNickName.size() < 2 ) || ( 13 < wstrNickName.size() ) )   // 캐릭터 닉네임 길이 검사.
	{
		return NetError::ERR_CREATE_UNIT_04;
	}

	// SQL인젝션방지
	if( KODBC::IsInvalidMarkIn( wstrNickName ) == true )
	{
		return NetError::ERR_ODBC_00;
	}

	return NetError::NET_OK;
}

void KGlobalUserList::AddHeroPvpObserver( IN const UidType iAddGSUID, IN const UidType iAddUnitUID )
{
	std::vector< std::pair< UidType, UidType > >::iterator it;
	for( it = m_vecHeroPvpObserverList.begin(); it != m_vecHeroPvpObserverList.end(); ++it )
	{
		if( it->second == iAddUnitUID )
		{
			it->first = iAddGSUID;
			return;
		}
	}

	m_vecHeroPvpObserverList.push_back( std::make_pair( iAddGSUID, iAddUnitUID ) );
}

void KGlobalUserList::RemoveHeroPvpObserver( IN const UidType iRemoveUnitUID )
{
	std::vector< std::pair< UidType, UidType > >::iterator it;
	for( it = m_vecHeroPvpObserverList.begin(); it != m_vecHeroPvpObserverList.end(); ++it )
	{
		if( it->second == iRemoveUnitUID )
		{
			m_vecHeroPvpObserverList.erase( it );
			return;
		}
	}
}

void KGlobalUserList::SetHeroPvpUserTournamentConnection( IN const UidType iServerGroupID, IN const std::wstring wstrNickName, IN const bool bConnect )
{
	HERO_PVP_USER_TYPE::iterator it;
	for( it = m_vecHeroPvpUserList.begin(); it != m_vecHeroPvpUserList.end(); ++it )
	{
		if( it->m_iServerGroupID != iServerGroupID )
		{
			continue;
		}

		if( it->m_wstrNickName != wstrNickName )
		{
			continue;
		}

		it->m_bConnect = bConnect;
		break;
	}

	if( it == m_vecHeroPvpUserList.end() )
	{
		return;
	}

	// 변경 정보 저장
	const KHeroPvpUserInfo& kHeroPvpUserInfo = *it;
	std::vector< std::pair< bool, KHeroPvpUserInfo > >::iterator itChange;

	for( itChange = m_vecHeroPvpUserChangeInfo.begin(); itChange != m_vecHeroPvpUserChangeInfo.end(); ++itChange )
	{
		if( itChange->second.m_iServerGroupID != iServerGroupID )
		{
			continue;
		}

		if( itChange->second.m_wstrNickName != wstrNickName )
		{
			continue;
		}

		// 저장 정보가 있다면, On 체크
		itChange->second.m_bConnect = bConnect;
		break;
	}

	if( itChange == m_vecHeroPvpUserChangeInfo.end() )
	{
		// 저장 정보가 없다면, 추가 체크 후 On 기록
		m_vecHeroPvpUserChangeInfo.push_back( std::make_pair( true, kHeroPvpUserInfo ) );
	}
}
#endif SERV_HERO_PVP_MANAGE_LIST
//}}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

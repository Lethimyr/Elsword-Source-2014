#include "NetError.h"
#include "ActorManager.h"
#include "LoginUser.h"
#include "SimLayer.h"
//{{ 2010. 06. 04  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
	#include "NexonAuthManager.h"
#else
	#include "NexonBillingAuthManager.h"
#endif SERV_PCBANG_AUTH_NEW
//}}
//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}
//#include "GASHAuthManager.h"

#include "UserList.h"
#include "BaseServer.h"


KUserList::KUserList() :
m_uiConcurrentUser( 0 ),
//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
m_bEnableAuthWait( true ),
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
//}}
m_iGuestConcurrentUser( 0 )
{
}

KUserList::~KUserList()
{
}

bool KUserList::GetGSUserInfoByUserUID( UidType iUserUID, KGSUserInfo& kInfo )
{
    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mit;

    mit = m_mapUserUID.find( iUserUID );
    if( mit == m_mapUserUID.end() )
    {
        return false;
    }

    if( !mit->second )
    {
        START_LOG( cerr, L"포인터 이상." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    kInfo = *( mit->second );
    return true;
}

bool KUserList::GetGSUserInfoByUserID( const std::wstring& wstrUserID, KUserList::KGSUserInfo& kInfo )
{
    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mit;

    mit = m_mapUserID.find( wstrUserID );
    if( mit == m_mapUserID.end() )
    {
        return false;
    }

    if( !mit->second )
    {
        START_LOG( cerr, L"포인터 이상." )
#ifndef SERV_PRIVACY_AGREEMENT
            << BUILD_LOG( wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
            << END_LOG;

        return false;
    }

    kInfo = *( mit->second );
    return true;
}

bool KUserList::GetGSUserInfoByUnitUID( UidType iUnitUID, KGSUserInfo& kInfo )
{
    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mit;

    mit = m_mapUnitUID.find( iUnitUID );
    if( mit == m_mapUnitUID.end() )
    {
        return false;
    }

    if( !mit->second )
    {
        START_LOG( cerr, L"포인터 이상." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return false;
    }

    kInfo = *( mit->second );
    return true;
}

bool KUserList::GetGSUserInfoByUnitNickName( std::wstring& wstrUnitNickName, KGSUserInfo& kInfo )
{
	//To do..
	//추후 찾는 유저의 uid(first_sender)를 받아와 서버군코드가 같은 유저의 닉네임을 찾게 수정
	//그럼 키값이 멀티로 서버군코드, 닉네임이 되어야 겠다.

    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mit;

    mit = m_mapUnitNickName.find( wstrUnitNickName );
    if( mit == m_mapUnitNickName.end() )
    {
        return false;
    }

    if( !mit->second )
    {
        START_LOG( cerr, L"포인터 이상." )
#ifndef SERV_PRIVACY_AGREEMENT
            << BUILD_LOG( wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT			
            << END_LOG;

        return false;
    }

    kInfo = *( mit->second );
    return true;
}

#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
bool KUserList::GetGSUserInfoByPublisherUID( unsigned int uiPublisherUID, KUserList::KGSUserInfo& kInfo )
{
	std::map< unsigned int, boost::shared_ptr< KGSUserInfo > >::iterator mit;

	mit = m_mapPublisherUID.find( uiPublisherUID );
	if( mit == m_mapPublisherUID.end() )
	{
		return false;
	}

	if( !mit->second )
	{
		START_LOG( cerr, L"포인터 이상." )
			<< BUILD_LOG( uiPublisherUID )
			<< END_LOG;

		return false;
	}

	kInfo = *( mit->second );
	return true;
}
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID

//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
bool KUserList::GetGSUserInfoByMachineID( IN const std::string& strMachineID, KUserList::KGSUserInfo& kInfo )
{
	std::map< std::string, boost::shared_ptr< KGSUserInfo > >::iterator mit;

	mit = m_mapMachineID.find( strMachineID );
	if( mit == m_mapMachineID.end() )
	{
		return false;
	}

	if( !mit->second )
	{
		START_LOG( cerr, L"포인터 이상." )
			<< BUILD_LOG( strMachineID )
			<< END_LOG;

		return false;
	}

	kInfo = *( mit->second );
	return true;    
}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}

//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW

void KUserList::CheckAuthSynchronize( const KENX_AUTH_SYNCHRONIZE_REQ& kInfo )
{
	//////////////////////////////////////////////////////////////////////////	
	// m_mapSynchronizeReq가 비어있지 않다면 인증서버에서 동기화 요청이 들어온것이다!
	// UserID로 검색하여 접속중인지 확인해보고 동기화 정보를 인증서버로 보내주자!

	if( kInfo.m_mapSynchronizeReq.empty() == false )
	{
		KENX_AUTH_SYNCHRONIZE_ACK kPacketAck;
		kPacketAck.m_bytePacketType = 46;
		kPacketAck.m_byteIsMonitoring = kInfo.m_byteIsMonitoring;
		__int64 iSessionNo = 0;

		std::map< __int64, std::wstring >::const_iterator mit;
		for( mit = kInfo.m_mapSynchronizeReq.begin(); mit != kInfo.m_mapSynchronizeReq.end(); ++mit )
		{
			KGSUserInfo kUserInfo;

			//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
			BYTE byteSyncRet = ( GetGSUserInfoBySessionNo( mit->first, kUserInfo ) ? 1 : 0 );
#else
			BYTE byteSyncRet = ( GetGSUserInfoByUserID( mit->second, kUserInfo ) ? 1 : 0 );
#endif SERV_PCBANG_INCORRECT_BILLING
			//}}

			// 접속중인데 SessionNo가 0이면 여기서는 넘어가고 동기화 완료 후 재로그인 처리 한다.
			if( kUserInfo.m_iSessionNo == 0 )
			{
				START_LOG( clog, L"유저의 SessionNo 가 0 입니깡?" )
					<< BUILD_LOG( kInfo.m_bytePacketType )
					<< BUILD_LOG( kInfo.m_byteIsMonitoring )
					<< BUILD_LOG( mit->second )
					<< BUILD_LOG( byteSyncRet )
					<< END_LOG;

				// PC 방 오과금 방지 수정
//				continue;
				byteSyncRet = 0;
			}

			// SessionNo가 0이 아닌데 원래 가지고 있던 SessionNo와 다르다면?
			if( kUserInfo.m_iSessionNo != mit->first )
			{
				byteSyncRet = 0;
			}

 			START_LOG( clog, L"세션으로 넘어가는 값을 봅시다" )
 				<< BUILD_LOG( mit->first )
 				<< BUILD_LOG( byteSyncRet );

			kPacketAck.m_mapSynchronizeAck.insert( std::make_pair( mit->first, byteSyncRet ) );
		}

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ENX_AUTH_SYNCHRONIZE_ACK, kPacketAck );
		SiKNexonAuthManager()->QueueingEvent( spEvent );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// m_mapSynchronizeReq가 비어있다면 인증서버에서 동기화가 완료된것이다!
	// 이제 유저 리스트를 확인해서 SessionNo가 0인것은 재 로그인 처리하자!
	CheckUnauthorizedSession();
}

void KUserList::CheckUnauthorizedSession()
{
	// SessionNo가 0인 유저를 찾아서 재 로그인 처리하자!

	std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >::const_iterator mitID;
	for( mitID = m_mapUserID.begin(); mitID != m_mapUserID.end(); ++mitID )
	{
		const boost::shared_ptr< KGSUserInfo > spGSUserInfo = mitID->second;
		if( spGSUserInfo == NULL )
		{
			START_LOG( cerr, L"유저 포인터가 이상합니다." )
				<< BUILD_LOG( mitID->first )
				<< END_LOG;
			continue;
		}

		// SessionNo가 0이라면 로그인 재시도!
		if( spGSUserInfo->m_iSessionNo == 0 )
		{
			//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
			//{{ 2012. 04. 02	김민성		넥슨 auth soap
//#ifdef SERV_NEXON_AUTH_SOAP

            

            int nRet1st = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, spGSUserInfo->m_ucChannelCode );
            int nRet2nd = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, spGSUserInfo->m_ucChannelCode );
            int nRet3rd = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, spGSUserInfo->m_ucChannelCode );

            if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
            {
                spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저 , 기본값
            }

			//int iRet = static_cast<int>( spGSUserInfo->m_wstrUserID.find( L"@tooni", 0 ) );
			//if( iRet != -1 ) // @가 없다. 검색 결과가 있다
			//{
			//	if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
			//	{
			//		START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
			//			<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
			//			<< END_LOG;
			//		spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // 투니랜드유저
			//	}
			//}
			//else		// @가 있다
			//{
   //             //CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, spGSUserInfo->m_ucChannelCode );
			//	iRet = static_cast<int>( spGSUserInfo->m_wstrUserID.find( L"@nx", 0 ) );
			//	if( iRet != -1 )
			//	{          
			//		if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
			//		{
			//			START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
			//				<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
			//				<< END_LOG;
			//			spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
			//		}
			//	}
			//	else
			//	{
			//		spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
			//	}
			//}

			std::wstring wstrResult;
			std::wstring wstrID;
			wstrResult = boost::str( boost::wformat( L"%d" ) % spGSUserInfo->m_ucChannelCode );
			wstrResult += ';';
			wstrID = boost::str( boost::wformat( L"%s" ) % spGSUserInfo->m_wstrUserID );
			wstrResult += wstrID;

			KGSUserInfo temp = *spGSUserInfo;
			temp.m_wstrUserID = wstrResult;

			// 인증 서버로 로그인 요청
			NexonAuthLogin( temp, true );
#else
			// 인증 서버로 로그인 요청
			NexonAuthLogin( *spGSUserInfo, true );
#endif SERV_TOONILAND_CHANNELING
			//}}

			START_LOG( clog2, L"넥슨 PC방 인증 서버 : 인증서버 연결실패로 인한 로그인 재시도!" )
				<< BUILD_LOG( spGSUserInfo->m_wstrUserID )
				<< BUILD_LOG( spGSUserInfo->m_uiIP )
				<< BUILD_LOG( spGSUserInfo->m_byteCanTry )
				<< BUILD_LOG( spGSUserInfo->m_bIsGuestUser );
		}
	}
}

int KUserList::CheckUserIDAndSetChannelCode( IN const std::wstring& wstrUserID_, IN const std::wstring& wstrPostfix_ , IN const int& nAcocuntType_, OUT UCHAR& ucChannelCode_ )
{
    int nRet = static_cast<int>( wstrUserID_.find( wstrPostfix_, 0 ) );

    if ( nRet != -1 )
    {
        if( ucChannelCode_ != nAcocuntType_ )
        {
            START_LOG( cwarn, L"이상한 채널코드로 등록 되어 있다." )
                << BUILD_LOG( ucChannelCode_ )
                << BUILD_LOG( nAcocuntType_ )
                << END_LOG;
            ucChannelCode_ = nAcocuntType_;
        }

#ifdef SERV_NAVER_CHANNELING
        if ( KNexonAccountInfo::CE_NAVER_ACCOUNT == nAcocuntType_ ) 
        { // 넥슨-네이버 채널링은 세션 채널코드와  SSO 채널코드가 다르다. 여기서 세션 채널코드로 변환
            ucChannelCode_ = KNexonAccountInfo::CE_NAVER_SESSION;
        }
#endif SERV_NAVER_CHANNELING
    }

    return nRet;
}

void KUserList::NexonAuthLogin( const KGSUserInfo& kUserInfo, bool bOnlyAuthentication /*= false*/ )
{
	KENX_AUTH_LOGIN_REQ kPacketReq;
	kPacketReq.m_bytePacketType	= 42;
	kPacketReq.m_wstrUserID		= kUserInfo.m_wstrUserID;
	kPacketReq.m_uiIP			= kUserInfo.m_uiIP;

	// 아이디의 체험판 여부
	{
		KPropertyValue kProperty;
		kProperty.m_bytePropertyValue = kUserInfo.m_byteCanTry;
		kPacketReq.m_mapProperty.insert( std::make_pair( 3, kProperty ) );
	}

	// 체험ID 여부
	{
		KPropertyValue kProperty;
		kProperty.m_bytePropertyValue = ( kUserInfo.m_bIsGuestUser ? 1 : 0 );
		kPacketReq.m_mapProperty.insert( std::make_pair( 4, kProperty ) );
	}

	// 엘소드는 게임 실행 중 프리미엄 적용이 불가능하므로 OnlyAuthentication을 보냄
	if( bOnlyAuthentication )
	{
		KPropertyValue kProperty;
		kProperty.m_bytePropertyValue = 1; // 일반 개인
		kPacketReq.m_mapProperty.insert( std::make_pair( 12, kProperty ) );
	}
	
	//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM

	KPropertyValue kProperty;
	KRolicyResults kResult;

	//{{ 2012. 06. 13	김민성       선택적 셧다운제
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
	kResult.m_byteRolicyListNo = KENX_AUTH_LOGIN_ACK::NPN_SELECTIVE_SHUTDOWN;
	kProperty.m_mapRolicyResult.insert( std::make_pair(kResult.m_byteRolicyListNo, kResult) ); // 게임 이용 차단 정책
#else
	//  이탈 방지 모델 기능 삭제
	// 	kResult.m_byteRolicyListNo = KENX_AUTH_LOGIN_ACK::NPN_OUT_USER_RETAINING;
	// 	kProperty.m_mapRolicyResult.insert( std::make_pair(kResult.m_byteRolicyListNo, kResult) ); // 이탈 방지

	kResult.m_byteRolicyListNo = KENX_AUTH_LOGIN_ACK::NPN_SHUTDOWN;
	kProperty.m_mapRolicyResult.insert( std::make_pair(kResult.m_byteRolicyListNo, kResult) ); // 셧다운제

	//{{ 2012. 02. 14    김민성   넥슨 안심 로그인 - 목표 위치기반 로그인
#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
	// 채널링 유저는 제외한다.
	if( kUserInfo.m_ucChannelCode == KNexonAccountInfo::CE_NEXON_ACCOUNT )
	{
		kResult.m_byteRolicyListNo = KENX_AUTH_LOGIN_ACK::NPN_PEACE_OF_MIND_LOGIN;
		kProperty.m_mapRolicyResult.insert( std::make_pair(kResult.m_byteRolicyListNo, kResult) ); // 안심 로그인
	}
#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
	//}}
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
	//}}


	kPacketReq.m_mapProperty.insert( std::make_pair( 17, kProperty ) );
#else
	/*
	//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
	// 이탈 예상 여부
	{
		KPropertyValue kProperty;
		kProperty.m_bytePropertyValue = 4;
		kPacketReq.m_mapProperty.insert( std::make_pair( 17, kProperty ) );
	}
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 
	*/
#endif SERV_SHUTDOWN_SYSTEM
	//}}

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ENX_AUTH_LOGIN_REQ, kPacketReq );
	SiKNexonAuthManager()->QueueingEvent( spEvent );

	//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
	RegAuthWaitUser( kUserInfo );
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
	//}}
}

void KUserList::NexonAuthLogout( const std::wstring& wstrUserID, __int64 iSessionNo )
{
	KENX_AUTH_LOGOUT_NOT kPacketNot;
	kPacketNot.m_bytePacketType = 43;
	kPacketNot.m_byteLogoutType = 8;
	kPacketNot.m_wstrUserID		= wstrUserID;
	kPacketNot.m_iSessionNo		= iSessionNo;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ENX_AUTH_LOGOUT_NOT, kPacketNot );
	SiKNexonAuthManager()->QueueingEvent( spEvent );    
}

bool KUserList::UpdateSessionNoByUserID( const std::wstring& wstrUserID, __int64 iSessionNo )
{
	std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mit;

	mit = m_mapUserID.find( wstrUserID );
	if( mit == m_mapUserID.end() )
	{
		return false;
	}

	if( !mit->second )
	{
		START_LOG( cerr, L"포인터 이상." )
			<< BUILD_LOG( wstrUserID )
			<< END_LOG;

		return false;
	}

	//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
	if( iSessionNo == 0 )
	{
		m_mapUserSessionNo.erase( mit->second->m_iSessionNo );
		return false;
	}
	else
	{
		std::map< UidType,	boost::shared_ptr< KGSUserInfo > >::iterator mitSessionNo = m_mapUserSessionNo.find( iSessionNo );
		if( mitSessionNo != m_mapUserSessionNo.end() )
		{
			mit->second->m_iSessionNo = iSessionNo;
			mitSessionNo->second = mit->second;
			START_LOG( clog, L"이미 존재하는 번호이다? 덮어 쓰자" );
		}
		else
		{
			m_mapUserSessionNo.insert( std::make_pair( iSessionNo, mit->second ) );
		}
	}
#endif SERV_PCBANG_INCORRECT_BILLING
	//}}

	mit->second->m_iSessionNo = iSessionNo;

	return true;
}

void KUserList::LoginTimeOut( const KELG_NEXON_USER_LOGIN_TIME_OUT_NOT& kInfo )
{
	KGSUserInfo kUserInfo;
	if( GetGSUserInfoByUserID( kInfo.m_wstrUserID, kUserInfo ) == false )
	{
		START_LOG( cerr, L"유저 정보가 존재하지 않습니다!" )
			<< BUILD_LOG( kInfo.m_wstrUserID )
			<< END_LOG;
		return;
	}

	//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
	//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	//int iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L"@tooni", 0 ) );
	//if( iRet != -1 ) // @tooni 가 있다
	//{
	//	if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
	//	{
	//		START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
	//			<< BUILD_LOG( kUserInfo.m_ucChannelCode )
	//			<< END_LOG;
	//		kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // 투니랜드유저
	//	}
	//}
	//else		// @가 있다
	//{
	//	iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L"@nx", 0 ) );
	//	if( iRet != -1 ) // @nx 가 있다
	//	{
	//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
	//		{
	//			START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
	//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
	//				<< END_LOG;
	//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
	//		}
	//	}
	//	else
	//	{
	//		kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
	//	}
	//}

    int nRet1st = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, kUserInfo.m_ucChannelCode );
    int nRet2nd = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, kUserInfo.m_ucChannelCode );
    int nRet3rd = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, kUserInfo.m_ucChannelCode );

    if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
    {
        kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저 , 기본값
    }

//#else
//	int iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L'@', 0 ) );
//	if( iRet == -1 ) // @가 없다
//	{
//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
//		{
//			START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
//				<< END_LOG;
//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
//		}
//	}
//	else		// @가 있다
//	{
//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
//		{
//			START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
//				<< END_LOG;
//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // 투니랜드유저
//		}
//	}
#endif SERV_NEXON_AUTH_SOAP
	//}}

	std::wstring wstrResult;
	std::wstring wstrID;
	wstrResult = boost::str( boost::wformat( L"%d" ) % kUserInfo.m_ucChannelCode );
	wstrResult += ';';
	wstrID = boost::str( boost::wformat( L"%s" ) % kUserInfo.m_wstrUserID );
	wstrResult += wstrID; // wstrResult 는 채널링코드(세션용);userid@naver 로 결합된 형태 -SSO채널링코드 아님-에 주의

	// 인증서버로 로그아웃 요청
	NexonAuthLogout( wstrResult, kUserInfo.m_iSessionNo ); 
#else
	// 인증서버로 로그아웃 요청
	NexonAuthLogout( kInfo.m_wstrUserID, kUserInfo.m_iSessionNo );
#endif SERV_TOONILAND_CHANNELING
	//}}

	START_LOG( clog2, L"넥슨 PC방 인증 서버 개편 : 타임 아웃으로 인한 계정 로그아웃!" )
		<< BUILD_LOG( kInfo.m_wstrUserID )
		<< BUILD_LOG( kUserInfo.m_iSessionNo );
	
	// SessionNo는 0으로 갱신!
	LIF( UpdateSessionNoByUserID( kInfo.m_wstrUserID, 0 ) );
	
	//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
	//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	//iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L"@tooni", 0 ) );
	//if( iRet != -1 ) // @tooni가 있다
	//{
	//	if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
	//	{
	//		START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
	//			<< BUILD_LOG( kUserInfo.m_ucChannelCode )
	//			<< END_LOG;
	//		kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // 투니랜드유저
	//	}
	//}
	//else		// @가 있다
	//{
	//	iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L"@nx", 0 ) );
	//	if( iRet != -1 ) // @nx가 있다
	//	{
	//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
	//		{
	//			START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
	//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
	//				<< END_LOG;
	//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
	//		}
	//	}
	//	else
	//	{
	//		kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
	//	}
	//}

    nRet1st = nRet2nd = nRet3rd = -1;
    nRet1st = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, kUserInfo.m_ucChannelCode );
    nRet2nd = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, kUserInfo.m_ucChannelCode );
    nRet3rd = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, kUserInfo.m_ucChannelCode );  // TODO : 네이버 채널링의 경우에는 세션 채널링코드로 변환해야 한다. 

    if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
    {
        kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저 , 기본값
    }
//#else
//	iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L'@', 0 ) );
//	if( iRet == -1 ) // @가 없다
//	{
//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
//		{
//			START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
//				<< END_LOG;
//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
//		}
//	}
//	else		// @가 있다
//	{
//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
//		{
//			START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
//				<< END_LOG;
//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // 투니랜드유저
//		}
//	}
#endif SERV_NEXON_AUTH_SOAP
	//}}

	wstrResult.clear();
	wstrID.clear();
	wstrResult = boost::str( boost::wformat( L"%d" ) % kUserInfo.m_ucChannelCode );
	wstrResult += ';';
	wstrID = boost::str( boost::wformat( L"%s" ) % kUserInfo.m_wstrUserID );
	wstrResult += wstrID;

	KGSUserInfo temp = kUserInfo;
	temp.m_wstrUserID = wstrResult;

	// 인증 서버로 로그인 요청
	NexonAuthLogin( temp, true );
#else
	// 인증 서버로 로그인 요청
	NexonAuthLogin( kUserInfo, true );
#endif SERV_TOONILAND_CHANNELING
	//}}

	START_LOG( clog2, L"넥슨 PC방 인증 서버 개편 : 타임 아웃으로 인한 계정 로그인!" )
		<< BUILD_LOG( kUserInfo.m_wstrUserID )
		<< BUILD_LOG( kUserInfo.m_uiIP )
		<< BUILD_LOG( kUserInfo.m_byteCanTry )
		<< BUILD_LOG( kUserInfo.m_bIsGuestUser );
}

#endif SERV_PCBANG_AUTH_NEW
//}}

int KUserList::RegUser( IN const UidType iUserUID, IN const UidType iGSUID, IN const KELG_REGISTER_USER_REQ& kInfo_, IN const bool bChannelChanged )
{
	// 1. 전달인자 값 검사
    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"유저 UID 이상." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_01;
    }

    if( kInfo_.m_wstrUserID.empty() )
    {
        START_LOG( cerr, L"유저 ID 이상." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_02;
    }

    if( iGSUID <= 0 )
    {
        START_LOG( cerr, L"게임 서버 UID 이상." )
            << BUILD_LOG( iGSUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_05;
    }

	// 2. 유저 정보 생성
    boost::shared_ptr< KGSUserInfo > spGSUserInfo( new KGSUserInfo() );
    spGSUserInfo->m_nUserUID	 = iUserUID;
    spGSUserInfo->m_wstrUserID	 = kInfo_.m_wstrUserID;
    spGSUserInfo->m_nGSUID		 = iGSUID;
	spGSUserInfo->m_iChannelID	 = kInfo_.m_iChannelID; // 채널ID
    spGSUserInfo->m_uiIP		 = kInfo_.m_uiIP;
    spGSUserInfo->m_byteCanTry	 = kInfo_.m_byteCanTry;	
	spGSUserInfo->m_bIsGuestUser = IsGuestUser( kInfo_.m_byteGuestUser ); // 체험ID 제한
	spGSUserInfo->m_cAuthLevel	 = kInfo_.m_cAuthLevel;
	//{{ 2010. 07. 01  최육사	PC방 인증 서버 개편
	spGSUserInfo->m_bChannelChanged = bChannelChanged;
	//}}
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	spGSUserInfo->m_uiPublisherUID = kInfo_.m_uiPublisherUID;
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
	//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
	//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	//int iRet = static_cast<int>( kInfo_.m_wstrUserID.find( L"@tooni", 0 ) );
	//if( iRet != -1 ) // @tooni 있다
	//{
	//	if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
	//	{
	//		START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
	//			<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
	//			<< END_LOG;
	//		spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 투니랜드유저
	//	}
	//}
	//else		
	//{
	//	iRet = static_cast<int>( kInfo_.m_wstrUserID.find( L"@nx", 0 ) );
	//	if( iRet != -1 ) // @nx 있다
	//	{
	//		if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
	//		{
	//			START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
	//				<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
	//				<< END_LOG;
	//			spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
	//		}
	//	}
	//	else
	//	{
	//		spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT;	// 넥슨 유저
	//	}
	//}

    int nRet1st = CheckUserIDAndSetChannelCode( kInfo_.m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, spGSUserInfo->m_ucChannelCode );
    int nRet2nd = CheckUserIDAndSetChannelCode( kInfo_.m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, spGSUserInfo->m_ucChannelCode );
    int nRet3rd = CheckUserIDAndSetChannelCode( kInfo_.m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, spGSUserInfo->m_ucChannelCode );

    if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
    {
        spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저 , 기본값
    }


//#else
//	int iRet = static_cast<int>( kInfo_.m_wstrUserID.find( L'@', 0 ) );
//	if( iRet == -1 ) // @가 없다
//	{
//		if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
//		{
//			START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
//				<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
//				<< END_LOG;
//			spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
//		}
//	}
//	else		// @가 있다
//	{
//		if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
//		{
//			START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
//				<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
//				<< END_LOG;
//			spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // 투니랜드유저
//		}
//	}
#endif SERV_NEXON_AUTH_SOAP
	//}}

#endif SERV_TOONILAND_CHANNELING
	//}}
#ifdef SERV_JAPAN_CHANNELING
	spGSUserInfo->m_ucChannelCode = kInfo_.m_ucChannelCode;
#endif // SERV_JAPAN_CHANNELING

	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	spGSUserInfo->m_strMachineID = kInfo_.m_strMachineID;
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUserUID;
    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mitUserID;
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	// 중복되는 머신ID가 있는지 확인!
	if( KSimLayer::GetKObj()->GetVersionFlag() != KSimLayer::VF_OPEN_TEST )		// 오픈 테섭에서는 검사 제외
	{
		if( SiKGameSysVal()->IsMachineIDDuplicateCheck() == true )
		{
			if( kInfo_.m_strMachineID.empty() )
			{
				START_LOG( cout, L"[알림] 머신ID가 비어있는 유저 발견!" )
					<< BUILD_LOG( kInfo_.m_wstrUserID )
					<< BUILD_LOGc( kInfo_.m_cAuthLevel )
					<< BUILD_LOG( kInfo_.m_iChannelID )
					<< BUILD_LOG( kInfo_.m_strMachineID );
			}
			else
			{
				if( m_mapMachineID.find( kInfo_.m_strMachineID ) != m_mapMachineID.end() )
				{
					// 운영자급 이상이 아니면 머신ID로 중복 접속한것인지 체크한다!
					if( kInfo_.m_cAuthLevel < SEnum::UAL_GM )
					{
						// 사내서버가 아닐때만 검사하기!
						if( KSimLayer::GetKObj()->GetAuthFlag() != KSimLayer::AF_INTERNAL )
						{
							//{{ 2012. 10. 12	최육사		2중 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX_COMPLETE
							KGSUserInfo kUserInfo;
							if( GetGSUserInfoByMachineID( kInfo_.m_strMachineID, kUserInfo ) == true )
							{
								KLoginUserPtr spServer;
								KActorManager::GetKObj()->Get( kUserInfo.m_nGSUID, spServer );
								if( IS_NOT_NULL( spServer ) )
								{
									START_LOG( cout, L"[알림] 머신ID 중복 접속!! 기존에 접속해 있던 유저를 접속 종료 시킵니다!" )
										<< BUILD_LOG( kUserInfo.m_nUserUID )
										<< BUILD_LOG( kUserInfo.m_wstrUserID )
										<< BUILD_LOG( kInfo_.m_wstrUserID )
										<< BUILD_LOG( kUserInfo.m_strMachineID );

									// 이전에 접속했던 유저를 튕겨내도록 한다.
									int kPacket = NetError::ERR_USER_REGISTRATION_10;
									KEvent kEvent;
									kEvent.SetData( PI_GS_USER, NULL, E_KICK_USER_NOT, kPacket );
									LIF( kEvent.m_kDestPerformer.AddUID( iUserUID ) );
									spServer->KSession::SendPacket( kEvent );
								}
							}
#endif SERV_DUPLICATE_CONNECT_BUG_FIX_COMPLETE
							//}}

							START_LOG( cout, L"[알림] 머신ID가 중복되는 유저가 있습니다! 해킹유저인가!?" )
								<< BUILD_LOG( kInfo_.m_wstrUserID )
								<< BUILD_LOG( kInfo_.m_strMachineID );

							// 현재 접속중인 유저를 튕겨낸다.
							return NetError::ERR_USER_REGISTRATION_10;
						}
						else
						{
							START_LOG( cout, L"[알림] 일반 계정으로 동일머신 중복 접속하였으나 서버 버전이 사내 버전이므로 정상 접속 처리 합니다." )
								<< BUILD_LOG( kInfo_.m_wstrUserID )
								<< BUILD_LOGc( kInfo_.m_cAuthLevel )
								<< BUILD_LOG( kInfo_.m_iChannelID )
								<< BUILD_LOG( kInfo_.m_strMachineID );
						}
					}
					else
					{
						START_LOG( cout, L"[알림] 운영자 또는 개발자가 동일 머신으로 중복 접속 시도를 하였습니다! 확인 바랍니다." )
							<< BUILD_LOG( kInfo_.m_wstrUserID )
							<< BUILD_LOGc( kInfo_.m_cAuthLevel )
							<< BUILD_LOG( kInfo_.m_iChannelID )
							<< BUILD_LOG( kInfo_.m_strMachineID );
					}
				}
			}
		}
	}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

	// 3. 이미 존재하는 유저 정보인지 확인
    mitUserUID = m_mapUserUID.find( iUserUID );
    if( mitUserUID != m_mapUserUID.end() )
    {
        START_LOG( cerr, L"유저 정보 존재." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        if( !mitUserUID->second )
        {
            START_LOG( cerr, L"포인터 이상." )
                << BUILD_LOG( iUserUID )
                << END_LOG;

            // 일어나서는 안되는 상황. 그냥 새 정보로 덮어 쓰자.
            m_mapUserUID.erase( mitUserUID );
            m_mapUserUID.insert( std::make_pair( iUserUID, spGSUserInfo ) );

            mitUserID = m_mapUserID.find( kInfo_.m_wstrUserID );
            if( mitUserID != m_mapUserID.end() )
            {
                if( mitUserID->second )
                {
                    START_LOG( cerr, L"포인터 이상이 아님." )
                        << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                        << BUILD_LOG( mitUserID->first )
#endif SERV_PRIVACY_AGREEMENT
                        << BUILD_LOG( mitUserID->second->m_nUserUID )
                        << END_LOG;
                }
                m_mapUserID.erase( mitUserID );
            }
            m_mapUserID.insert( std::make_pair( kInfo_.m_wstrUserID, spGSUserInfo ) );
			//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
			if( kInfo_.m_strMachineID.empty() == false )
			{
				m_mapMachineID.insert( std::make_pair( kInfo_.m_strMachineID, spGSUserInfo ) );
			}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
			//}}
            goto end_proc;
        }

        if( mitUserUID->second->m_nGSUID == iGSUID )
        {
            START_LOG( cerr, L"같은 게임 서버 이중접속." )  // 이미 검사를 한 경우이므로 일어날 수 없다.
                << BUILD_LOG( iUserUID )                    // 아니면 이전 문제로 정보가 안지워진 경우이다.
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( iGSUID )
                << END_LOG;

            mitUserUID->second.reset();
            mitUserUID->second = spGSUserInfo;
            mitUserID = m_mapUserID.find( kInfo_.m_wstrUserID );
            if( mitUserID != m_mapUserID.end() )
            {
                START_LOG( cerr, L"UserID 맵에 있음." )
                    << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( iGSUID )
                    << END_LOG;

                m_mapUserID.erase( mitUserID );
            }
            else
            {
                START_LOG( cerr, L"UserID 맵에 없음." )
                    << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( iGSUID )
                    << END_LOG;
            }
            m_mapUserID.insert( std::make_pair( kInfo_.m_wstrUserID, spGSUserInfo ) );
			//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
			if( kInfo_.m_strMachineID.empty() == false )
			{
				m_mapMachineID.insert( std::make_pair( kInfo_.m_strMachineID, spGSUserInfo ) );
			}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
			//}}
            goto end_proc;
        }

        KLoginUserPtr spServer;
        KActorManager::GetKObj()->Get( mitUserUID->second->m_nGSUID, spServer );
        if( spServer )   // 타 서버간 이중접속
        {
            START_LOG( cerr2, L"타 서버간 이중접속 감지." )
                << BUILD_LOG( spServer->GetName() )
                << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                << END_LOG;

            // 이전에 접속했던 유저를 튕겨내도록 한다.
            int kPacket = NetError::ERR_VERIFY_01;
            KEvent kEvent;
            kEvent.SetData( PI_GS_USER, NULL, E_KICK_USER_NOT, kPacket );
            LIF( kEvent.m_kDestPerformer.AddUID( iUserUID ) );
            spServer->KSession::SendPacket( kEvent );

            // 현재 접속하려는 유저도 튕겨낸다.
            return NetError::ERR_USER_REGISTRATION_00;
        }
        else    // 이전에 접속해있었다는 서버가 연결되어있지 않음. 서버 다운 등으로 잘못된 데이터가 남아있던 경우.
        {
            KLoginUserPtr spServerCurrent;
            KActorManager::GetKObj()->Get( iGSUID, spServerCurrent );
            if( !spServerCurrent )
            {
                // 서버가 죽기 직전 인증 요청을 한 것일까? 어쨌든 이 유저를 리스트에 추가하면 안된다.
                START_LOG( cerr, L"인증 요청을 한 유저의 게임 서버가 연결되어 있지 않음." )
                    << BUILD_LOG( iGSUID )
                    << BUILD_LOG( iUserUID )
                    << END_LOG;

                return NetError::ERR_USER_REGISTRATION_07;
            }

            mitUserUID->second.reset();
            mitUserUID->second = spGSUserInfo;

            mitUserID = m_mapUserID.find( kInfo_.m_wstrUserID );
            if( mitUserID != m_mapUserID.end() )
            {
                START_LOG( cerr, L"UserID 맵에 있음." )
                    << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( iGSUID )
                    << END_LOG;

                m_mapUserID.erase( mitUserID );
            }
            else
            {
                START_LOG( cerr, L"UserID 맵에 없음." )
                    << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( iGSUID )
                    << END_LOG;
            }

            m_mapUserID.insert( std::make_pair( kInfo_.m_wstrUserID, spGSUserInfo ) );
			//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
			if( kInfo_.m_strMachineID.empty() == false )
			{
				m_mapMachineID.insert( std::make_pair( kInfo_.m_strMachineID, spGSUserInfo ) );
			}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
			//}}
            goto end_proc;
        }
    }

	// 4. 유저 정보 저장
    m_mapUserUID.insert( std::make_pair( iUserUID, spGSUserInfo ) );
    mitUserID = m_mapUserID.find( kInfo_.m_wstrUserID );
    if( mitUserID != m_mapUserID.end() )
    {
        START_LOG( cerr, L"UserUID 맵에서 없다고 확인되었는데 UserID 맵에는 있음." )
            << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
            << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
            << END_LOG;

        m_mapUserID.erase( mitUserID );
    }
    m_mapUserID.insert( std::make_pair( kInfo_.m_wstrUserID, spGSUserInfo ) );
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	if( kInfo_.m_strMachineID.empty() == false )
	{
		m_mapMachineID.insert( std::make_pair( kInfo_.m_strMachineID, spGSUserInfo ) );
	}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	// 4.1. 퍼블리셔 정보 저장
	if( kInfo_.m_uiPublisherUID != 0)
	{
		std::map< unsigned int, boost::shared_ptr< KGSUserInfo > >::iterator mitPublisherUID;

		mitPublisherUID = m_mapPublisherUID.find( kInfo_.m_uiPublisherUID );
		if( mitPublisherUID != m_mapPublisherUID.end() )
		{
			START_LOG( cerr, L"PublisherUID 맵에 있음." )
				<< BUILD_LOG( iUserUID )
				<< BUILD_LOG( kInfo_.m_uiPublisherUID )
				<< END_LOG;

			m_mapPublisherUID.erase( mitPublisherUID );
		}
		m_mapPublisherUID.insert( std::make_pair( kInfo_.m_uiPublisherUID, spGSUserInfo ) );
	}
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID

	// 5. 현재 동접 저장
	SetConcurrentUserCount( m_mapUserUID.size() );
    goto end_proc;

end_proc:
    // 6. 인증 서버 로그인 - 정상적인 유저 등록이면 여기로 온다.
    if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
    {
		//{{ 2010. 06. 04  최육사	넥슨PC방 인증 서버 개편
//#ifdef SERV_PCBANG_AUTH_NEW
		//{{ 2011. 07. 27    김민성    투니랜드 채널링
//#ifdef SERV_TOONILAND_CHANNELING
		//{{ 2012. 04. 02	김민성		넥슨 auth soap
//#ifdef SERV_NEXON_AUTH_SOAP
		/*int iRet = static_cast<int>( spGSUserInfo->m_wstrUserID.find( L"@tooni", 0 ) );
		if( iRet != -1 ) // @tooni 있다
		{
			if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
			{
				START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
					<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
					<< END_LOG;
				spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // 투니랜드유저
			}
		}
		else		
		{
			iRet = static_cast<int>( spGSUserInfo->m_wstrUserID.find( L"@nx", 0 ) );
			if( iRet != -1 ) // @nx 있다
			{
				if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
				{
					START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
						<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
						<< END_LOG;
					spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
				}
			}
			else
			{
				spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
			}
		}*/

        int nRet1st = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, spGSUserInfo->m_ucChannelCode );
        int nRet2nd = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, spGSUserInfo->m_ucChannelCode );
        int nRet3rd = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, spGSUserInfo->m_ucChannelCode );

        if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
        {
            spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저 , 기본값
        }

//#else
//		int iRet = static_cast<int>( spGSUserInfo->m_wstrUserID.find( L'@', 0 ) );
//		if( iRet == -1 ) // @가 없다
//		{
//			if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
//			{
//				START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
//					<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
//					<< END_LOG;
//				spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
//			}
//		}
//		else		// @가 있다
//		{
//			if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
//			{
//				START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
//					<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
//					<< END_LOG;
//				spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // 투니랜드유저
//			}
//		}
//#endif SERV_NEXON_AUTH_SOAP
		//}}
		
		std::wstring wstrResult;
		std::wstring wstrID;
		wstrResult = boost::str( boost::wformat( L"%d" ) % spGSUserInfo->m_ucChannelCode );
		wstrResult += ';';
		wstrID = boost::str( boost::wformat( L"%s" ) % spGSUserInfo->m_wstrUserID );
		wstrResult += wstrID;

		KGSUserInfo temp = *spGSUserInfo;
		temp.m_wstrUserID = wstrResult;

		// 인증 서버로 로그인 요청
		NexonAuthLogin( temp );
//#else
//		// 인증서버로 로그인 요청
//		NexonAuthLogin( *spGSUserInfo );
//#endif SERV_TOONILAND_CHANNELING
		//}}
		
//#else
//		//////////////////////////////////////////////////////////////////////////
//		KENX_USER_LOGIN_REQ kPacketReq;
//
//		//if( StrRChrW( wstrUserID.c_str(), wstrUserID.c_str()+wstrUserID.size(), L'/' ) != NULL )
//		//{
//		//	kPacketReq.m_wstrUserID = wstrUserID.substr( 0, wstrUserID.size() - 1 );
//		//}
//
//		kPacketReq.m_bytePacketType = 39;
//		kPacketReq.m_byteLogin		= 4;
//		kPacketReq.m_wstrUserID		= kInfo_.m_wstrUserID;
//		kPacketReq.m_wstrUnitNickName.clear();
//		kPacketReq.m_uiIP			= kInfo_.m_uiIP;
//		kPacketReq.m_byteCanTry		= kInfo_.m_byteCanTry;
//		kPacketReq.m_byteGuestUser	= kInfo_.m_byteGuestUser;
//
//		KEventPtr spEvent( new KEvent );
//		spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ENX_USER_LOGIN_REQ, kPacketReq );
//		SiKNexonBillingAuthManager()->QueueingEvent( spEvent );
//#endif SERV_PCBANG_AUTH_NEW
		//}}

		//{{ 2008. 5. 27  최육사  체험ID 동접
		if( IsGuestUser( kInfo_.m_byteGuestUser ) )
		{
			IncreaseGuestConcurrentUser();
		}
		//}}
    }
	
	else
	{
		//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
		RegAuthWaitUser( *spGSUserInfo );
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
		//}}
#ifdef SERV_GLOBAL_AUTH
#if defined(SERV_COUNTRY_TWHK) || defined(SERV_COUNTRY_JP)
		RequestRegUser( iUserUID, *spGSUserInfo );	// 해외용 인증 요청
#elif defined(SERV_COUNTRY_TH)
		RequestRegUser( iUserUID, *spGSUserInfo, kInfo_.m_wstrSocketID );	// 해외용 인증 요청
#endif // SERV_COUNTRY_XX
#endif // SERV_GLOBAL_AUTH
	}

    return NetError::NET_OK;
}

bool KUserList::UnRegUser( IN const UidType iUserUID )
{
	// 1. 전달인자 검사
    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"유저 UID 이상." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUserUID;
    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mitUserID;

	// 2. 삭제하려는 유저가 있는지 검사
    mitUserUID = m_mapUserUID.find( iUserUID );
    if( mitUserUID == m_mapUserUID.end() )
    {
        START_LOG( cerr, L"삭제하려는 유저가 없음." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    if( !mitUserUID->second )
    {
        START_LOG( cerr, L"유저 정보 포인터 이상." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        m_mapUserUID.erase( mitUserUID );
        return false;
    }

    // 유닛이 등록되어 있으면 삭제한다. 
    // -> 유저 객체 삭제 시에 유닉 삭제를 거치지 않고 유저 삭제를 해왔으나 꼭 유닛 삭제를 먼저 하도록 변경함.
    // LIF( !UnRegUnit( iUserUID ) );

	//{{ 2009. 7. 9  최육사		유닛정보 먼저 지우고 유저정보 지워야 한다!
	// 3. 유닛 정보가 아직 남아있는지 검사
	if( mitUserUID->second->m_nUnitUID > 0 )
	{
		START_LOG( cerr, L"유닛정보가 지워지지 않았는데 유저정보를 지우려 한다!" )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( mitUserUID->second->m_nUnitUID )
			<< END_LOG;
	}
	//}}
	
    KGSUserInfo kInfo = *( mitUserUID->second );
	//{{ 2010. 06. 28  최육사	게임 PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
	const __int64 iSessionNo = kInfo.m_iSessionNo;
#endif SERV_PCBANG_AUTH_NEW
	//}}

	// 4. 유저 정보 삭제
    if( kInfo.m_wstrUserID.empty() )
    {
        START_LOG( cerr, L"유저 아이디 이상." )
            << BUILD_LOG( iUserUID )
            << END_LOG;
    }
    else
    {
        mitUserID = m_mapUserID.find( kInfo.m_wstrUserID );
        if( mitUserID == m_mapUserID.end() )
        {
            START_LOG( cerr, L"유저 ID 로 검색 실패." )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( kInfo.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( iUserUID )
                << END_LOG;
        }
        else
        {
            m_mapUserID.erase( mitUserID );
        }
    }

	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	// 머신ID정보 삭제
	if( kInfo.m_strMachineID.empty() )
	{
		START_LOG( cout, L"[알림] 머신ID가 비어있는 유저가 로그 아웃 하였습니다!" )
			<< BUILD_LOG( kInfo.m_wstrUserID )
			<< BUILD_LOGc( kInfo.m_cAuthLevel )
			<< BUILD_LOG( kInfo.m_iChannelID );
	}
	else
	{
		std::map< std::string, boost::shared_ptr< KGSUserInfo > >::iterator mitMachineID;
		mitMachineID = m_mapMachineID.find( kInfo.m_strMachineID );
		if( mitMachineID == m_mapMachineID.end() )
		{
			START_LOG( cerr, L"머신ID 로 검색 실패." )
				<< BUILD_LOG( kInfo.m_strMachineID )
				<< BUILD_LOG( iUserUID )
				<< END_LOG;
		}
		else
		{
			m_mapMachineID.erase( mitMachineID );
		}
	}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

	//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
	std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitSessionNo = m_mapUserSessionNo.find( iSessionNo );
	if( mitSessionNo != m_mapUserSessionNo.end() )
	{
		m_mapUserSessionNo.erase( mitSessionNo );
	}
#endif SERV_PCBANG_INCORRECT_BILLING
	//}}

    m_mapUserUID.erase( mitUserUID );

#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	// 4.1. 퍼블리셔 정보 삭제
	std::map< unsigned int, boost::shared_ptr< KGSUserInfo > >::iterator mitPublisherUID;

	if( kInfo.m_uiPublisherUID == 0)
	{
		START_LOG( cerr, L"퍼블리셔 UID 이상." )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( kInfo.m_wstrUserID )
			<< BUILD_LOG( kInfo.m_uiPublisherUID )
			<< END_LOG;
	}
	else
	{
		mitPublisherUID = m_mapPublisherUID.find( kInfo.m_uiPublisherUID );
		if( mitPublisherUID == m_mapPublisherUID.end() )
		{
			START_LOG( cerr, L"퍼블리셔 UID 로 검색 실패." )
				<< BUILD_LOG( iUserUID )
				<< BUILD_LOG( kInfo.m_wstrUserID )
				<< BUILD_LOG( kInfo.m_uiPublisherUID )
				<< END_LOG;
		}
		else
		{
			m_mapPublisherUID.erase( mitPublisherUID );
		}
	}
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID

	// 5. 현재 동접 저장
	SetConcurrentUserCount( m_mapUserUID.size() );

	// 6. 인증 서버 로그 아웃
    switch( KSimLayer::GetKObj()->GetAuthFlag() )
    {
    case KSimLayer::AF_NEXON_KOREA:
        {
			//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
			// 인증서버로 로그아웃 요청
			//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
			//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP
			/*int iRet = static_cast<int>( kInfo.m_wstrUserID.find( L"@tooni", 0 ) );
			if( iRet != -1 ) // @tooni 있다
			{
				if( kInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
				{
					START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
						<< BUILD_LOG( kInfo.m_ucChannelCode )
						<< END_LOG;
					kInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // 투니랜드유저
				}
			}
			else		
			{
				iRet = static_cast<int>( kInfo.m_wstrUserID.find( L"@nx", 0 ) );
				if( iRet != -1 ) // @nx 있다
				{
					if( kInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
					{
						START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
							<< BUILD_LOG( kInfo.m_ucChannelCode )
							<< END_LOG;
						kInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
					}
				}
				else
				{
					kInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
				}
			}*/

            int nRet1st = CheckUserIDAndSetChannelCode( kInfo.m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, kInfo.m_ucChannelCode );
            int nRet2nd = CheckUserIDAndSetChannelCode( kInfo.m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, kInfo.m_ucChannelCode );
            int nRet3rd = CheckUserIDAndSetChannelCode( kInfo.m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, kInfo.m_ucChannelCode );

            if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
            {
                kInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저 , 기본값
            }
//#else
//			int iRet = static_cast<int>( kInfo.m_wstrUserID.find( L'@', 0 ) );
//			if( iRet == -1 ) // @가 없다
//			{
//				if( kInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
//				{
//					START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
//						<< BUILD_LOG( kInfo.m_ucChannelCode )
//						<< END_LOG;
//					kInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // 넥슨 유저
//				}
//			}
//			else		// @가 있다
//			{
//				if( kInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
//				{
//					START_LOG( clog, L"이상한 채널코드로 동록 되어 있다." )
//						<< BUILD_LOG( kInfo.m_ucChannelCode )
//						<< END_LOG;
//					kInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // 투니랜드유저
//				}
//			}
#endif SERV_NEXON_AUTH_SOAP
			//}}
		
			std::wstring wstrResult;
			std::wstring wstrID;
		
			wstrResult = boost::str( boost::wformat( L"%d" ) % kInfo.m_ucChannelCode );
			wstrResult += ';';
			wstrID = boost::str( boost::wformat( L"%s" ) % kInfo.m_wstrUserID );
			wstrResult += wstrID;
			
			NexonAuthLogout( wstrResult, iSessionNo );
#else
			NexonAuthLogout( kInfo.m_wstrUserID, iSessionNo );
#endif SERV_TOONILAND_CHANNELING
			//}}
#else
			//////////////////////////////////////////////////////////////////////////
			KENX_USER_LOGOUT_REQ kPacketReq;

			//if( StrRChrW( kInfo.m_wstrUserID.c_str(), kInfo.m_wstrUserID.c_str()+kInfo.m_wstrUserID.size(), L'/' ) != NULL )
			//{
			//	kPacketReq.m_wstrUserID = kInfo.m_wstrUserID.substr( 0, kInfo.m_wstrUserID.size() - 1 );
			//}

			kPacketReq.m_bytePacketType = 39;
			kPacketReq.m_byteLogin		= 1;
			kPacketReq.m_wstrUserID		= kInfo.m_wstrUserID;
			kPacketReq.m_wstrUnitNickName.clear();
			kPacketReq.m_uiIP			= kInfo.m_uiIP;
			kPacketReq.m_byteCanTry		= kInfo.m_byteCanTry;
			kPacketReq.m_byteGuestUser	= kInfo.m_bIsGuestUser;

			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ENX_USER_LOGOUT_REQ, kPacketReq );
			SiKNexonBillingAuthManager()->QueueingEvent( spEvent );
			//////////////////////////////////////////////////////////////////////////
#endif SERV_PCBANG_AUTH_NEW
			//}}            
			//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
			m_mapUserSessionNo.erase( iSessionNo );
#endif SERV_PCBANG_INCORRECT_BILLING
			//}}

			//{{ 2008. 5. 27  최육사  체험ID 동접
			if( kInfo.m_bIsGuestUser )
			{
				DecreaseGuestConcurrentUser();
			}
			//}}
		}
        break;
    default:
        break;
    }

#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_TWHK
	RequestUnRegUser(iUserUID, kInfo);
#endif // SERV_COUNTRY_TWHK
#endif // SERV_GLOBAL_AUTH

    return true;
}

void KUserList::UnRegAllUserOfThisGS( IN const UidType iGSUID )
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

    std::vector< UidType > vecUserUID;
    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUserUID;
    for( mitUserUID = m_mapUserUID.begin(); mitUserUID != m_mapUserUID.end(); mitUserUID++ )
    {
        if( !mitUserUID->second )
        {
            START_LOG( cerr, L"유저 정보 포인터 이상." )
                << BUILD_LOG( mitUserUID->first )
                << END_LOG;

            continue;
        }

        if( mitUserUID->second->m_nGSUID == iGSUID )
        {
            vecUserUID.push_back( mitUserUID->first );
        }
    }

    std::vector< UidType >::iterator vitUserUID;
    for( vitUserUID = vecUserUID.begin(); vitUserUID != vecUserUID.end(); vitUserUID++ )
    {
        UnRegUser( *vitUserUID );
    }

    return;
}

int KUserList::RegUnit( IN const UidType iUserUID, IN const UidType iUnitUID, IN const std::wstring& wstrUnitNickName, IN const unsigned int uiKNMSerialNum, IN const bool bDenyFriendShip )
{
    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"유저 UID 이상." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_01;
    }

    if( iUnitUID <= 0 )
    {
        START_LOG( cerr, L"유닛 UID 이상." )
            << BUILD_LOG( iUserUID )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_03;
    }

    if( wstrUnitNickName.empty() )
    {
        START_LOG( cerr, L"유닛 닉네임 이상." )
            << BUILD_LOG( iUserUID )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_04;
    }

	if( uiKNMSerialNum <= 0  )
	{
		START_LOG( cerr, L"유닛 메신져 SN 이상." )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( uiKNMSerialNum )
			<< END_LOG;

		return NetError::ERR_USER_REGISTRATION_09;
	}

    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUserUID;
    mitUserUID = m_mapUserUID.find( iUserUID );
    if( mitUserUID == m_mapUserUID.end() )
    {
        START_LOG( cerr, L"유닛을 등록하려는데 계정이 등록되어 있지 않음." )
            << BUILD_LOG( iUserUID )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_08;
    }

    if( !mitUserUID->second )
    {
        START_LOG( cerr, L"포인터 이상." )
            << BUILD_LOG( iUserUID )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_06;
    }

	//UID로 검색
    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitUID;
    mitUnitUID = m_mapUnitUID.find( mitUserUID->second->m_nUnitUID );
    if( mitUnitUID != m_mapUnitUID.end() )
    {
        START_LOG( cerr, L"UnReg 되지 않은 유닛." )
            << BUILD_LOG( mitUnitUID->first )
            << END_LOG;

        if( !mitUnitUID->second )
        {
            START_LOG( cerr, L"포인터 이상." )
                << BUILD_LOG( iUserUID )
                << BUILD_LOG( iUnitUID )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT
                << END_LOG;
        }
        else
        {
            LIF( mitUnitUID->second->m_nUserUID == iUserUID );
        }
        m_mapUnitUID.erase( mitUnitUID );
    }
    m_mapUnitUID.insert( std::make_pair( iUnitUID, mitUserUID->second ) );

	//닉네임 검색
    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitNickName;
    mitUnitNickName = m_mapUnitNickName.find( mitUserUID->second->m_wstrUnitNickName );
    if( mitUnitNickName != m_mapUnitNickName.end() )
    {
        START_LOG( cerr, L"UnReg 되지 않은 유닛 닉네임." )
#ifndef SERV_PRIVACY_AGREEMENT
            << BUILD_LOG( mitUnitNickName->first )
#endif SERV_PRIVACY_AGREEMENT
            << END_LOG;

        if( !mitUnitNickName->second )
        {
            START_LOG( cerr, L"포인터 이상." )
                << BUILD_LOG( iUserUID )
                << BUILD_LOG( iUnitUID )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT
                << END_LOG;
        }
        else
        {
            LIF( mitUnitNickName->second->m_nUserUID == iUserUID );
        }
        m_mapUnitNickName.erase( mitUnitNickName );
    }
    m_mapUnitNickName.insert( std::make_pair( wstrUnitNickName, mitUserUID->second ) );

	//기존 데이터 검사를 마친후 오류처리를 한다음 데이터 셋팅
	//포인터 이기때문에 한곳에만 하면된다.
	mitUserUID->second->m_nUnitUID			= iUnitUID;
	mitUserUID->second->m_wstrUnitNickName	= wstrUnitNickName;
	mitUserUID->second->m_uiKNMSerialNum	= uiKNMSerialNum;
	//{{ 2009. 4. 1  최육사		친구차단
	mitUserUID->second->m_bDenyFriendShip	= bDenyFriendShip;
	//}}

    return NetError::NET_OK;
}

bool KUserList::UnRegUnit( IN const UidType iUserUID )
{
    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"유저 UID 이상." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUserUID;
    mitUserUID = m_mapUserUID.find( iUserUID );
    if( mitUserUID == m_mapUserUID.end() )
    {
        START_LOG( cerr, L"유닛을 삭제하려는데 계정이 등록되어 있지 않음." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    if( !mitUserUID->second )
    {
        START_LOG( cerr, L"포인터 이상." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitUID;
    if( mitUserUID->second->m_nUnitUID > 0 )
    {
        mitUnitUID = m_mapUnitUID.find( mitUserUID->second->m_nUnitUID );
        if( mitUnitUID == m_mapUnitUID.end() )
        {
            START_LOG( cerr, L"유닛 UID 로 검색 실패." )
                << BUILD_LOG( mitUserUID->second->m_nUnitUID )
                << END_LOG;
        }
        else
        {
            if( !mitUnitUID->second )
            {
                START_LOG( cerr, L"포인터 이상." )
                    << BUILD_LOG( iUserUID )
                    << BUILD_LOG( mitUserUID->second->m_nUnitUID )
                    << END_LOG;
            }
            else
            {
                LIF( mitUnitUID->second->m_nUserUID == iUserUID );
            }
            m_mapUnitUID.erase( mitUnitUID );
        }

        if( mitUserUID->second->m_wstrUnitNickName.empty() )
        {
            START_LOG( cerr, L"유닛 UID는 등록이 되어있는데 닉네임은 등록되지 않음." )
                << BUILD_LOG( iUserUID )
                << BUILD_LOG( mitUserUID->second->m_nUnitUID )
                << END_LOG;
        }
    }
    else
    {
        if( !mitUserUID->second->m_wstrUnitNickName.empty() )
        {
            START_LOG( cerr, L"유닛 UID는 등록이 안되어있는데 닉네임은 있음." )
                << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( mitUserUID->second->m_wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT
                << END_LOG;
        }
    }

    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitNickName;
    if( !mitUserUID->second->m_wstrUnitNickName.empty() )
    {
        mitUnitNickName = m_mapUnitNickName.find( mitUserUID->second->m_wstrUnitNickName );
        if( mitUnitNickName == m_mapUnitNickName.end() )
        {
            START_LOG( cerr, L"유닛 닉네임으로 검색 실패." )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( mitUserUID->second->m_wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT
                << END_LOG;
        }
        else
        {
            if( !mitUnitNickName->second )
            {
                START_LOG( cerr, L"포인터 이상." )
                    << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( mitUserUID->second->m_wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT
                    << END_LOG;
            }
            else
            {
                LIF( mitUnitNickName->second->m_nUserUID == iUserUID );
            }
            m_mapUnitNickName.erase( mitUnitNickName );
        }
    }

	mitUserUID->second->m_nUnitUID = 0;
	mitUserUID->second->m_wstrUnitNickName.clear();
    mitUserUID->second->m_uiKNMSerialNum = 0;
	//{{ 2009. 4. 1  최육사		친구차단
	mitUserUID->second->m_bDenyFriendShip = false;
	//}}

    return true;
}

//{{ 2009. 4. 1  최육사		친구 차단
bool KUserList::SetDenyFriendShip( UidType iUnitUID, bool bDenyFriendShip )
{
	std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mit;

	mit = m_mapUnitUID.find( iUnitUID );
	if( mit == m_mapUnitUID.end() )
	{
		return false;
	}

	if( !mit->second )
	{
		// 시간차로 발생가능.
		START_LOG( cwarn, L"포인터 이상." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	( mit->second )->m_bDenyFriendShip = bDenyFriendShip;
	return true;
}
//}}

//{{ 2009. 11. 16  최육사	 체험ID동접
void KUserList::DecreaseGuestConcurrentUser()
{
	--m_iGuestConcurrentUser;

	if( m_iGuestConcurrentUser < 0 )
	{
		m_iGuestConcurrentUser = 0;
	}
}
//}}

//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
bool KUserList::GetGSUserInfoBySessionNo( const __int64& iSessionNo, KUserList::KGSUserInfo& kInfo )
{
	std::map< UidType,		boost::shared_ptr< KGSUserInfo > >::iterator mit;

	mit = m_mapUserSessionNo.find( iSessionNo );
	if( mit == m_mapUserSessionNo.end() )
	{
		START_LOG( clog, L"없는 SessionNO 입니다." )
			<< BUILD_LOG( iSessionNo )
			<< END_LOG;
		return false;
	}

	if( !mit->second )
	{
		START_LOG( cerr, L"포인터 이상." )
			<< BUILD_LOG( iSessionNo )
			<< END_LOG;

		return false;
	}

	kInfo = *( mit->second );
	return true;
}
#endif SERV_PCBANG_INCORRECT_BILLING
//}}

//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
bool KUserList::RegAuthWaitUser( IN const KUserList::KGSUserInfo& kUserInfo )
{
	if( IsEnableAuthWait() == false )
		return true;

	std::map< UidType, KAuthWaitUser >::iterator mit;
	mit = m_mapAuthWaitUser.find( kUserInfo.m_nUserUID );
	if( mit != m_mapAuthWaitUser.end() )
	{
		START_LOG( cerr, L"이미 인증 대기자로 등록된 유저입니다!" )
			<< BUILD_LOG( kUserInfo.m_nUserUID )
			<< BUILD_LOG( kUserInfo.m_wstrUserID )
			<< END_LOG;
		return false;
	}

	KAuthWaitUser kAuthWaitUser;
	kAuthWaitUser.m_iUserUID = kUserInfo.m_nUserUID;
	m_mapAuthWaitUser.insert( std::make_pair( kUserInfo.m_nUserUID, kAuthWaitUser ) );
	return true;
}

void KUserList::UnRegAuthWaitUser( IN const UidType iUserUID )
{
	m_mapAuthWaitUser.erase( iUserUID );
}

void KUserList::Tick()
{
	if( m_tCheckAuthWaitTimer.elapsed() < 1.0 )
		return;

	m_tCheckAuthWaitTimer.restart();

	std::vector< UidType > vecDeleteUserList;

	std::map< UidType, KAuthWaitUser >::const_iterator mit;
	for( mit = m_mapAuthWaitUser.begin(); mit != m_mapAuthWaitUser.end(); ++mit )
	{
#ifdef SERV_GLOBAL_AUTH
		if( mit->second.m_tTimer.elapsed() > 30.0 )	// 해외는 통신 느려서 30초까지 대기 시간 늘림
#else //SERV_GLOBAL_AUTH
		if( mit->second.m_tTimer.elapsed() > 10.0 )
#endif //SERV_GLOBAL_AUTH
		{
			vecDeleteUserList.push_back( mit->first );
		}
	}

	BOOST_TEST_FOREACH( const UidType, iUserUID, vecDeleteUserList )
	{
		// 일단 먼저 지우고
		UnRegAuthWaitUser( iUserUID );

		KUserList::KGSUserInfo kInfo;
		if( GetGSUserInfoByUserUID( iUserUID, kInfo ) == false )
		{
			START_LOG( cerr, L"유저 게임 서버 검색 실패." )
				<< BUILD_LOG( iUserUID )
				<< END_LOG;
			continue;
		}

		START_LOG( cout, L"넥슨 PC방 인증 서버 : 인증 시간 지연으로 강제로 게임 시작 처리!" )
			<< BUILD_LOG( kInfo.m_wstrUserID );

		KENX_AUTH_LOGIN_ACK kPacketAuth;
		kPacketAuth.m_bytePacketType;
		kPacketAuth.m_byteLoginType;
		kPacketAuth.m_wstrUserID = kInfo.m_wstrUserID;
		kPacketAuth.m_byteAuthorizeResult = 2; // Trial - 프리미엄 없이 게임을 실행한다.
		
		//{{ 2010. 07. 01  최육사	PC방 인증 서버 개편
		if( kInfo.m_bChannelChanged )
		{
			UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
			KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ELG_REGISTER_NEXON_CHANNEL_CHANGE_USER_ACK, kPacketAuth );
		}
		else
		{
			UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
			KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ELG_REGISTER_NEXON_USER_ACK, kPacketAuth );
		}
		//}}
	}
}
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-07
void KUserList::ReserveLocalRankingUserUnitInfo( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo )
{
	if( 100 < m_mapLocalRankingUserUnitInfo.size() )
	{
		m_mapLocalRankingUserUnitInfo.clear();
	}

	m_mapLocalRankingUserUnitInfo.insert( std::map< UidType, std::pair< KLocalRankingUserInfo, KLocalRankingUnitInfo > >::value_type( kUnitInfo.m_iUnitUID, std::make_pair( kUserInfo, kUnitInfo ) ) );
}

bool KUserList::GetLocalRankingUserUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUserInfo& kUserInfo, OUT KLocalRankingUnitInfo& kUnitInfo ) const
{
	std::map< UidType, std::pair< KLocalRankingUserInfo, KLocalRankingUnitInfo > >::const_iterator it = m_mapLocalRankingUserUnitInfo.find( iUnitUID );
	if( it == m_mapLocalRankingUserUnitInfo.end() )
	{
		return false;
	}

	kUserInfo = it->second.first;
	kUnitInfo = it->second.second;
	return true;
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

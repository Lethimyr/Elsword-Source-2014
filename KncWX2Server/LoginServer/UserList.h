#pragma once

#include <map>
#include "KncUidType.h"
#include "KncSmartPtr.h"

#include <boost/timer.hpp>

#include "Enum/Enum.h"

#pragma pack( push, 1 )

class KUserList
{
public:
    struct KGSUserInfo 
    {
    public:
		//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
		__int64			m_iSessionNo;
		//}}
        UidType         m_nUserUID;
        std::wstring	m_wstrUserID;
        UidType			m_nUnitUID;
        std::wstring	m_wstrUnitNickName;
		unsigned int	m_uiKNMSerialNum;		
        UidType         m_nGSUID;		
		//{{ 2009. 4. 17  최육사	채널id
		int				m_iChannelID;
		//}}
        unsigned int    m_uiIP;
        unsigned char   m_byteCanTry;
		bool			m_bIsGuestUser;
		//{{ 2009. 4. 1  최육사		차단기능
		char			m_cAuthLevel;
		bool			m_bDenyFriendShip;
		//}}
		//{{ 2010. 07. 01  최육사	PC방 인증 서버 개편
		bool			m_bChannelChanged;
		//}}
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
		unsigned int	m_uiPublisherUID;
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
		//{{ 2011. 07. 27    김민성    투니랜드 채널링
//#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		unsigned char	m_ucChannelCode;
//#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		//}}
		//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
//#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
		std::string		m_strMachineID;
//#endif SERV_MACHINE_ID_DUPLICATE_CHECK
		//}}

        KGSUserInfo()
        {
			//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
			m_iSessionNo		= 0;
			//}}
            m_nUserUID			= 0;
            m_wstrUserID		= std::wstring();
            m_nUnitUID			= 0;
            m_wstrUnitNickName	= std::wstring();
			m_uiKNMSerialNum	= 0;
            m_nGSUID			= 0;
			//{{ 2009. 4. 17  최육사	채널id
			m_iChannelID		= 0;
			//}}
            m_uiIP              = 0;
            m_byteCanTry        = 0;			
			m_bIsGuestUser		= false;
			//{{ 2009. 4. 1  최육사		차단기능
			m_cAuthLevel		= SEnum::UAL_NORMAL;
			m_bDenyFriendShip	= false;
			//}}
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
			m_uiPublisherUID	= 0;
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
			//{{ 2010. 07. 01  최육사	PC방 인증 서버 개편
			m_bChannelChanged	= false;
			//}}
			//{{ 2011. 07. 27    김민성    투니랜드 채널링
//#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
			m_ucChannelCode		= 0;
//#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
			//}}

        }
//{{ 2011. 07. 27    김민성    투니랜드 채널링
//#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
        KGSUserInfo( UidType iUserUID, std::wstring& wstrUserID, UidType iGSUID, unsigned char ucChannelCode )
//#else
//        KGSUserInfo( UidType iUserUID, std::wstring& wstrUserID, UidType iGSUID )
//#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
			//}}
        {
			//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
			m_iSessionNo		= 0;
			//}}
            m_nUserUID			= iUserUID;
            m_wstrUserID		= wstrUserID;
            m_nUnitUID			= 0;
            m_wstrUnitNickName	= std::wstring();
            m_uiKNMSerialNum    = 0;			
            m_nGSUID			= iGSUID;
			//{{ 2009. 4. 17  최육사	채널id
			m_iChannelID		= 0;
			//}}
            m_uiIP              = 0;
            m_byteCanTry        = 0;			
			m_bIsGuestUser		= false;
			//{{ 2009. 4. 1  최육사		차단기능
			m_cAuthLevel		= SEnum::UAL_NORMAL;
			m_bDenyFriendShip	= false;
			//}}
			//{{ 2010. 07. 01  최육사	PC방 인증 서버 개편
			m_bChannelChanged	= false;
			//}}
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
			m_uiPublisherUID	= 0;
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
			//{{ 2011. 07. 27    김민성    투니랜드 채널링
//#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
			m_ucChannelCode		= ucChannelCode;
//#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
			//}}

            return;
        }

        KGSUserInfo( const KGSUserInfo& t )
        {
            *this = t;
        }

        KGSUserInfo& operator=( const KGSUserInfo& t )
        {
			//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
			m_iSessionNo		= t.m_iSessionNo;
			//}}
            m_nUserUID			= t.m_nUserUID;
            m_wstrUserID		= t.m_wstrUserID;
            m_nUnitUID			= t.m_nUnitUID;
            m_wstrUnitNickName	= t.m_wstrUnitNickName;
            m_uiKNMSerialNum    = t.m_uiKNMSerialNum;
            m_nGSUID			= t.m_nGSUID;
			//{{ 2009. 4. 17  최육사	채널id
			m_iChannelID		= t.m_iChannelID;
			//}}
            m_uiIP              = t.m_uiIP;
            m_byteCanTry        = t.m_byteCanTry;			
			m_bIsGuestUser		= t.m_bIsGuestUser;
			//{{ 2009. 4. 1  최육사		차단기능
			m_cAuthLevel		= t.m_cAuthLevel;
			m_bDenyFriendShip	= t.m_bDenyFriendShip;
			//}}
			//{{ 2010. 07. 01  최육사	PC방 인증 서버 개편
			m_bChannelChanged	= t.m_bChannelChanged;
			//}}
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
			m_uiPublisherUID	= t.m_uiPublisherUID;
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
			//{{ 2011. 07. 27    김민성    투니랜드 채널링
//#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
			m_ucChannelCode		= t.m_ucChannelCode;
//#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
			//}}
			//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
			//#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
			m_strMachineID		= t.m_strMachineID;
			//#endif SERV_MACHINE_ID_DUPLICATE_CHECK
			//}}

            return *this;
        }
    };

	//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
	struct KAuthWaitUser
	{
		UidType         m_iUserUID;
		boost::timer	m_tTimer;

		KAuthWaitUser()
		{
			m_iUserUID = 0;
		}
	};
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
	//}}

public:
	KUserList();
	virtual ~KUserList();

#ifdef SERV_GLOBAL_AUTH
#if defined (SERV_COUNTRY_TWHK) || defined (SERV_COUNTRY_TH) || defined (SERV_COUNTRY_JP)
	void RequestRegUser( IN const UidType iUserUID, const KGSUserInfo& kInfo, std::wstring wstrSockID = L"" );
	void RequestUnRegUser( IN const UidType iUserUID, const KGSUserInfo& kInfo );
#endif // SERV_COUNTRY_TWHK
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
	void	LogoutPCBang( const KELG_PCBANG_LOGOUT_NOT& kInfo );
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

    bool	GetGSUserInfoByUserUID( UidType iUserUID, KGSUserInfo& kInfo );
    bool	GetGSUserInfoByUserID( const std::wstring& wstrUserID, KUserList::KGSUserInfo& kInfo );
    bool	GetGSUserInfoByUnitUID( UidType iUnitUID, KGSUserInfo& kInfo );
    bool	GetGSUserInfoByUnitNickName( std::wstring& wstrUnitNickName, KGSUserInfo& kInfo );
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	bool	GetGSUserInfoByPublisherUID( unsigned int uiPublisherUID, KUserList::KGSUserInfo& kInfo );
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	bool	GetGSUserInfoByMachineID( IN const std::string& strMachineID, KUserList::KGSUserInfo& kInfo );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
	//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
	bool	GetGSUserInfoBySessionNo( const __int64& iSessionNo, KUserList::KGSUserInfo& kInfo );
#endif SERV_PCBANG_INCORRECT_BILLING
	//}}

	//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
	void	CheckAuthSynchronize( const KENX_AUTH_SYNCHRONIZE_REQ& kInfo );
	bool	UpdateSessionNoByUserID( const std::wstring& wstrUserID, __int64 iSessionNo );
	void	LoginTimeOut( const KELG_NEXON_USER_LOGIN_TIME_OUT_NOT& kInfo );
	u_int	GetConcurrentUserCount()					{ KLocker lock( m_csConcurrentUser ); return m_uiConcurrentUser; }
	void	SetConcurrentUserCount( u_int uiCCU )		{ KLocker lock( m_csConcurrentUser ); m_uiConcurrentUser = uiCCU; }
#endif SERV_PCBANG_AUTH_NEW
	//}}

	//{{ 2010. 01. 11  최육사	추천인리스트
	bool	IsLoginedUnitUID( UidType iUnitUID )		{ return ( m_mapUnitUID.find( iUnitUID ) != m_mapUnitUID.end() ); }
	//}}

    int		RegUser( IN const UidType iUserUID, IN const UidType iGSUID, IN const KELG_REGISTER_USER_REQ& kInfo, IN const bool bChannelChanged );
    bool	UnRegUser( IN const UidType iUserUID );
    void    UnRegAllUserOfThisGS( IN const UidType iGSUID );
    int		RegUnit( IN const UidType iUserUID, IN const UidType iUnitUID, IN const std::wstring& wstrUnitNickName, IN const unsigned int uiKNMSerialNum, IN const bool bDenyFriendShip );
    bool	UnRegUnit( IN const UidType iUserUID );

	//{{ 2008. 5. 27  최육사  체험ID 
	bool	IsGuestUser( unsigned char byteGuestUser )	{ return ( byteGuestUser == 1 ); }
	int		GetGuestConcurrentUserCount()				{ return m_iGuestConcurrentUser; }
	//}}

	//{{ 2009. 4. 1  최육사		친구 차단
	bool	SetDenyFriendShip( UidType iUnitUID, bool bDenyFriendShip );
	//}}

	//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
	void	SetEnableAuthWait( IN const bool bEnable )	{ m_bEnableAuthWait = bEnable; }
	bool	RegAuthWaitUser( IN const KGSUserInfo& kUserInfo );
	void	UnRegAuthWaitUser( IN const UidType iUserUID );
	void	Tick();
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
	//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-07
	void	ReserveLocalRankingUserUnitInfo( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo );
	bool	GetLocalRankingUserUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUserInfo& kUserInfo, OUT KLocalRankingUnitInfo& kUnitInfo ) const;
	void	RemoveLocalRankingUserUnitInfo( IN const UidType iUnitUID ){	m_mapLocalRankingUserUnitInfo.erase( iUnitUID );	}
#endif	// SERV_LOCAL_RANKING_SYSTEM

protected:
	//{{ 2009. 11. 16  최육사	 체험ID동접
	void	IncreaseGuestConcurrentUser()				{ ++m_iGuestConcurrentUser; }
	void	DecreaseGuestConcurrentUser();
	//}}

	//{{ 2010. 06. 28  최육사	게임 PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW	
	bool	IsEnableAuthWait() const					{ return m_bEnableAuthWait; }
	void	CheckUnauthorizedSession();
	void	NexonAuthLogin( const KGSUserInfo& kUserInfo, bool bOnlyAuthentication = false );
	void	NexonAuthLogout( const std::wstring& wstrUserID, __int64 iSessionNo );
#endif SERV_PCBANG_AUTH_NEW
	//}}	

    int CheckUserIDAndSetChannelCode( IN const std::wstring& wstrUserID_, IN const std::wstring& wstrPostfix_ , IN const int& nAcocuntType_, OUT UCHAR& ucChannelCode_ );

protected:
	std::map< UidType,		boost::shared_ptr< KGSUserInfo > >		m_mapUserUID;   // main key, data
	std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >		m_mapUserID;

	std::map< UidType,		boost::shared_ptr< KGSUserInfo > >		m_mapUnitUID;
	std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >		m_mapUnitNickName;
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	std::map< unsigned int,	boost::shared_ptr< KGSUserInfo > >		m_mapPublisherUID;
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	std::map< std::string,  boost::shared_ptr< KGSUserInfo > >		m_mapMachineID;
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

	//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
	std::map< UidType,		boost::shared_ptr< KGSUserInfo > >		m_mapUserSessionNo;   // main key(SessionNo), data
#endif SERV_PCBANG_INCORRECT_BILLING
	//}}
	//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
	std::map< UidType,		 KAuthWaitUser >						m_mapAuthWaitUser;	// key(UserUID)
	boost::timer													m_tCheckAuthWaitTimer;
	bool															m_bEnableAuthWait;
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
	//}}

	u_int															m_uiConcurrentUser;		// 현재 동접 (동기화)
	mutable KncCriticalSection										m_csConcurrentUser;

	int																m_iGuestConcurrentUser; // 체험ID 동접

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-07
	std::map< UidType, std::pair< KLocalRankingUserInfo, KLocalRankingUnitInfo > >	m_mapLocalRankingUserUnitInfo;
#endif	// SERV_LOCAL_RANKING_SYSTEM
};

#pragma pack( pop )




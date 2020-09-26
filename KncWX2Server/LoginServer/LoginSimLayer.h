#pragma once
#include "SimLayer.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "UserList.h"

class KLoginSimLayer : public KSimLayer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclDump;

public:
    KLoginSimLayer(void);
    virtual ~KLoginSimLayer(void);

    // derived from KSimLayer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();
    virtual void RegToLua();

    bool	GetGSUserInfoByUserUID( UidType iUserUID, KUserList::KGSUserInfo& kInfo );
    bool	GetGSUserInfoByUserID( const std::wstring& wstrUserID, KUserList::KGSUserInfo& kInfo );
    bool	GetGSUserInfoByUnitUID( UidType iUnitUID, KUserList::KGSUserInfo& kInfo );
    bool	GetGSUserInfoByUnitNickName( std::wstring& wstrUnitNickName, KUserList::KGSUserInfo& kInfo );
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	bool	GetGSUserInfoByPublisherUID( unsigned int uiPublisherUID, KUserList::KGSUserInfo& kInfo );
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	bool	GetGSUserInfoByMachineID( IN const std::string& strMachineID, KUserList::KGSUserInfo& kInfo );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

	//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
	void	CheckAuthSynchronize( const KENX_AUTH_SYNCHRONIZE_REQ& kInfo );
	bool	UpdateSessionNoByUserID( const std::wstring& wstrUserID, __int64 iSessionNo );
	void	LoginTimeOut( const KELG_NEXON_USER_LOGIN_TIME_OUT_NOT& kInfo );
	u_int	GetConcurrentUserCount()					{ return m_kUserList.GetConcurrentUserCount(); }
#endif SERV_PCBANG_AUTH_NEW
	//}}
#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
	void	LogoutPcBang( const KELG_PCBANG_LOGOUT_NOT& kInfo );
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

	//{{ 2010. 01. 11  최육사	추천인리스트
	bool	IsLoginedUnitUID( UidType iUnitUID )		{ return m_kUserList.IsLoginedUnitUID( iUnitUID ); }
	//}}

    int		RegUser( UidType iUserUID, UidType iGSUID, const KELG_REGISTER_USER_REQ& kInfo, bool bChannelChanged );
    void	UnRegUser( UidType iUserUID );
    void    UnRegAllUserOfThisGS( UidType iGSUID );
    int		RegUnit( UidType iUserUID, UidType iUnitUID, std::wstring& wstrUnitNickName, u_int& uiKNMSerialNum, bool bDenyFriendShip );
    void	UnRegUnit( UidType iUserUID );

	//{{ 2008. 5. 27  최육사  체험ID 동접
	int		GetGuestConcurrentUserCount()				{ return m_kUserList.GetGuestConcurrentUserCount(); }
	//}}

	//{{ 2009. 4. 1  최육사		친구 차단
	bool	SetDenyFriendShip( UidType iUnitUID, bool bDenyFriendShip );
	//}}

	//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
	void	SetEnableAuthWait_LUA( bool bEnable );
	void	UnRegAuthWaitUser( IN const UidType iUserUID )	{ m_kUserList.UnRegAuthWaitUser( iUserUID ); }
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
	//}}

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	void	SetEnableCNDirectCharge_LUA(bool bValue);
	const bool	IsEnableCNDirectCharge() const { return m_bEnableCNDirectCharge; }
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

	//{{ 2013.05.20 조효진 게임서버 접속 시 퍼블리셔 인증 갔다오는지 여부 (PC방 인증 포함) UnRegAuthWaitUser 위치 달라짐
#ifdef SERV_GLOBAL_AUTH
	void	SetPublisherCheckGameServerLogin_LUA( bool bValue ){ m_bPublisherCheckGameServerLogin = bValue; };
	bool	IsPublisherCheckGameServerLogin(){ return m_bPublisherCheckGameServerLogin; };
protected:
	bool					m_bPublisherCheckGameServerLogin;
#endif //SERV_GLOBAL_AUTH
	//}}
#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-07
	void	ReserveLocalRankingUserUnitInfo( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo ){	m_kUserList.ReserveLocalRankingUserUnitInfo( kUserInfo, kUnitInfo );	}
	bool	GetLocalRankingUserUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUserInfo& kUserInfo, OUT KLocalRankingUnitInfo& kUnitInfo ) const{	return m_kUserList.GetLocalRankingUserUnitInfo( iUnitUID, kUserInfo, kUnitInfo );	}
	void	RemoveLocalRankingUserUnitInfo( IN const UidType iUnitUID ){	m_kUserList.RemoveLocalRankingUserUnitInfo( iUnitUID );	}
#endif	// SERV_LOCAL_RANKING_SYSTEM

protected:
	KUserList               m_kUserList;
	//{{ 2010. 02. 24  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
	boost::timer			m_tWebPointEventTimer;
#endif SERV_WEB_POINT_EVENT
	//}}
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	bool					m_bEnableCNDirectCharge;
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
};

DefKObjectInline( KLoginSimLayer, KSimLayer );



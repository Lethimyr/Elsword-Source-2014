#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "KncSend.h"
#include "BaseServer.h"
//{{ 2012. 03. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
#include "X2Data/XSLRoom.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_CHANNEL_PARTY
//////////////////////////////////////////////////////////////////////////


#define KCnPartyUserPtr KCnPartyUser*
#undef KCnPartyUserPtr
SmartPointer( KCnPartyUser );


class KCnPartyUser
{
public:
    KCnPartyUser( const KPartyUserInfo& kInfo );
    virtual ~KCnPartyUser();

    void	GetPartyUserInfo( OUT KPartyUserInfo& kInfo )			{ kInfo = m_kPartyUserInfo; }
	const KPartyUserInfo& GetPartyUserInfo()						{ return m_kPartyUserInfo; }
    void	SetPartyUserInfo( IN const KPartyUserInfo& kInfo )		{ m_kPartyUserInfo = kInfo; }

	void	SetPartyRoomUserInfo( IN const KPartyRoomUserInfo& kInfo );	
	bool	GetPartyRoomUserInfo( OUT KPartyRoomUserInfo& kInfo );

	//{{ 2012. 03. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool	GetOldPartyInfo( OUT KOldPartyInfo& kInfo ) const;
	void	SetOldPartyInfo( IN const KOldPartyInfo& kInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	void	ClearPartyRoomUserInfo()								{ m_vecPartyRoomUserInfo.clear(); }
	bool	IsExistPartyRoomUserInfo()								{ return !m_vecPartyRoomUserInfo.empty(); }

	UidType	GetGSUID() const										{ return m_kPartyUserInfo.m_iGSUID; }
	UidType GetFieldUID() const										{ return m_kPartyUserInfo.m_iFieldUID; }

	UidType GetCharUID() const										{ return m_kPartyUserInfo.m_iUnitUID; }
	const std::wstring& GetCharName() const							{ return m_kPartyUserInfo.m_wstrNickName; }
	char	GetUnitClass() const									{ return m_kPartyUserInfo.m_cUnitClass; }
	void	SetUnitClass( IN char cUnitClass )						{ m_kPartyUserInfo.m_cUnitClass = cUnitClass; }
	u_char	GetLevel() const 										{ return m_kPartyUserInfo.m_ucLevel; }
	void	SetLevel( IN u_char ucLevel )							{ m_kPartyUserInfo.m_ucLevel = ucLevel; }

	void	SetState( IN char cState )								{ m_kPartyUserInfo.m_cState = cState; }
	void	SetStateCode( IN int iStateCode )						{ m_kPartyUserInfo.m_iStateCode = iStateCode; }

	void	SetHost( bool bVal )									{ m_bHost = bVal; m_kPartyUserInfo.m_bHost = bVal; }
	bool	IsHost()												{ return m_bHost; }
	void	SetReady( bool bVal )									{ m_bReady = bVal; m_kPartyUserInfo.m_bReady = bVal; }
	bool	IsReady()												{ return m_bReady; }

	//{{ 2012. 05. 30	최육사		공식 대전 버그 수정
#ifdef SERV_OFFCIAL_PVP_BUG_FIX
	//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	void	SetRank( IN const char cRank )							{ m_kPartyUserInfo.m_cRank = cRank; }
	void	SetOfficialMatchCnt( IN const int iOfficialMatchCnt )	{ m_kPartyUserInfo.m_iOfficialMatchCnt = iOfficialMatchCnt; }
#endif SERV_2012_PVP_SEASON2
	//}}
	void	SetRating( IN const int iRating )						{ m_kPartyUserInfo.m_iRating = iRating; }
	void	SetRPoint( IN const int iRPoint )						{ m_kPartyUserInfo.m_iRPoint = iRPoint; }
#endif SERV_OFFCIAL_PVP_BUG_FIX
	//}}

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	void	SetComeBackUser( bool bVal )							{ m_kPartyUserInfo.m_bComeBack = bVal; }
	bool	GetComeBackUser()										{ return m_kPartyUserInfo.m_bComeBack; }
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	void	GetMatchUserInfo( IN const UidType iPartyUID, OUT KMatchUserInfo& kInfo ) const;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	GetAutoPartyUserInfo( OUT KAutoPartyUserInfo& kInfo ) const;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 03. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	const KPartyMemberStatus& GetPartyMemberStatus() const					{ return m_kPartyUserInfo.m_kGamePlayStatus; }
	void	SetPartyMemberStatus( IN const KPartyMemberStatus& kInfo )		{ m_kPartyUserInfo.m_kGamePlayStatus = kInfo; }

	void	SetOnGoToPartyButton( IN const bool bIsOnGoToPartyButton )		{ m_bIsOnGoToPartyButton = bIsOnGoToPartyButton; }
	bool	IsOnGoToPartyButton() const										{ return m_bIsOnGoToPartyButton; }

	void	SetCheckedForAutoParty( IN const bool bCheckForAutoParty )		{ m_bCheckedForAutoParty = bCheckForAutoParty; }
	bool	IsCheckedForAutoParty() const									{ return m_bCheckedForAutoParty; }

	void	SetCheckedPlayCondition( IN const bool bCheckedPlayCondition )	{ m_bCheckedPlayCondition = bCheckedPlayCondition; }
	bool	IsCheckedPlayCondition() const									{ return m_bCheckedPlayCondition; }

	void	UpdateJoinRoomInfo( IN const KPartyUserJoinRoomInfo& kJoinRoomInfo )	{ m_kJoinRoomInfo = kJoinRoomInfo; }
	const KPartyUserJoinRoomInfo& GetJoinRoomInfo() const					{ return m_kJoinRoomInfo; }
	UidType GetJoinRoomUID() const											{ return m_kJoinRoomInfo.m_iRoomUID; }
	int		GetJoinBattleFieldID() const									{ return m_kJoinRoomInfo.m_iBattleFieldID; }
	CXSLRoom::ROOM_TYPE GetJoinRoomType() const								{ return CXSLRoom::GetRoomType( m_kJoinRoomInfo.m_iRoomUID ); }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

public:
	template < class T > void SendToGSCharacter( unsigned short usEventID, const T& data );
	void SendToGSCharacter( unsigned short usEventID );	

private:
    KPartyUserInfo						m_kPartyUserInfo;
	bool								m_bReady;
	bool								m_bHost;
	//{{ 2012. 03. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool								m_bIsOnGoToPartyButton;
	bool								m_bCheckedForAutoParty;
	bool								m_bCheckedPlayCondition;
	KPartyUserJoinRoomInfo				m_kJoinRoomInfo;
	std::vector< KOldPartyInfo >		m_vecOldPartyInfo;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	
	std::vector< KPartyRoomUserInfo >	m_vecPartyRoomUserInfo;
};

template < class T >
void KCnPartyUser::SendToGSCharacter( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { m_kPartyUserInfo.m_iGSUID, -1 };
	KncSend( PI_CN_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, m_kPartyUserInfo.m_iUnitUID, anTrace, usEventID, data );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_CHANNEL_PARTY
//////////////////////////////////////////////////////////////////////////



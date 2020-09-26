#pragma once

#include <KncSmartPtr.h>
#include "AutoPartyUser.h"
#include "Performer.h"
#include "FSM/support_FSM.h"
#include "AutoPartyUserManager.h"
#include "AutoPartyUserWaitList.h"
#include "X2Data/XSLRoom.h"
#include "Enum/Enum.h"


//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM



//////////////////////////////////////////////////////////////////////////
// autoparty state not
#define VERIFY_AUTO_PARTY_STATE( varg ) \
if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
		<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
		<< L"    허용 상태 : "L ## #varg << END_LOG; \
	return; \
} \
START_LOG_WITH_NAME( clog ); \

//////////////////////////////////////////////////////////////////////////
// autoparty state err
#define _VERIFY_AUTO_PARTY_STATE_ACK( varg, eventid, packet, neterror ) \
packet kPacket; \
if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
		<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
		<< L"    허용 상태 : "L ## #varg << END_LOG; \
	kPacket.m_iOK = neterror; \
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
START_LOG_WITH_NAME( clog ); \

#define VERIFY_AUTO_PARTY_STATE_ACK( varg, eventid, packet ) _VERIFY_AUTO_PARTY_STATE_ACK( varg, eventid, packet, NetError::ERR_UNKNOWN );

//////////////////////////////////////////////////////////////////////////
// autoparty state warn
#define _VERIFY_AUTO_PARTY_STATE_ACK_WARN( varg, eventid, packet, neterror ) \
packet kPacket; \
if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cwarn, L"상태 오류. Name : " << m_strName ) \
		<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
		<< L"    허용 상태 : "L ## #varg << END_LOG; \
	kPacket.m_iOK = neterror; \
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
START_LOG_WITH_NAME( clog ); \

#define VERIFY_AUTO_PARTY_STATE_ACK_WARN( varg, eventid, packet ) _VERIFY_AUTO_PARTY_STATE_ACK_WARN( varg, eventid, packet, NetError::ERR_UNKNOWN );


#define KAutoPartyPtr KAutoParty*
#undef KAutoPartyPtr
SmartPointer( KAutoParty );


class KAutoParty : public KPerformer
{
	DeclareFSM_MT;
	DeclPfID;

public:
	enum AUTO_PARTY_TIMER_ENUM
	{
		APTE_AUTO_PARTY_MAKING = 0,
		APTE_CHECK_ACCEPT,
		APTE_REGROUP_PARTY,
		APTE_PLAY,

		APTE_MAX,
	};

	KAutoParty( IN const UidType iAutoPartyUID, IN const KAutoPartyConditionInfo& kInfo );
    virtual ~KAutoParty();

	void		Tick();
	void		OnDestroy();

	void		CloseAutoParty( IN const int iReason, IN const UidType iCancelUnitUID = 0 );
	void		CloseAutoPartyByCancel( IN const UidType iCancelUnitUID );

	//{{ 2012. 11. 28 던전,대전 자동 매칭 취소 되지 않는 오류 수정(예외 상황) - 김민성
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
	void		CloseAutoPartyByCancel( IN const std::vector<UidType>& vecCancelUnitUID );
#endif SERV_AUTO_MATCH_CANCEL_ERROR
	//}

	// AutoParty info
	UidType		GetAutoPartyUID() const				{ return GetUID(); }
	int			GetLimitPlayerCount() const			{ return static_cast<int>(m_kAutoPartyInfo.m_cLimitPlayerCount); }
	bool		GetCreatePartyInfo( OUT KCreatePartyInfo& kInfo ) const;
	int			GetNumMember() const				{ return m_kAutoPartyUserManager.GetNumMember(); }
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	char		GetDifficultyLevel() const			{ return m_kAutoPartyInfo.m_cDifficultyLevel; }
	void		SetDifficultyLevel( IN const char cDifficultyLevel ) { m_kAutoPartyInfo.m_cDifficultyLevel = cDifficultyLevel; }
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

	// AutoParty function
	bool		Enter( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList );
	bool		CheckAutoPartyMakingSuccess();
	
	//////////////////////////////////////////////////////////////////////////
	// process by gsuser event..
protected:
	virtual inline void ProcessEvent( const KEventPtr& spEvent );
	
	DECL_ON_FUNC( EGB_CANCEL_AUTO_PARTY_MAKING_REQ );
   _DECL_ON_FUNC( EGB_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT, KEGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT );
	DECL_ON_FUNC( EGB_GET_REGROUP_PARTY_INFO_ACK );
   _DECL_ON_FUNC( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK );

public:
	template< class T >	void SendToGSCharacter( IN const UidType iGSUID, IN const UidType iUnitUID, IN const u_short usEventID, IN const T& data );
	template< class T > void BroadCast( IN const u_short usEventID, IN const T& data, IN const UidType iUnitUID_ = 0 );

protected:
	KAutoPartyConditionInfo	m_kAutoPartyInfo;
	KAutoPartyUserManager	m_kAutoPartyUserManager;
	boost::timer			m_tTimer[APTE_MAX];
};

template < class T >
void KAutoParty::SendToGSCharacter( IN const UidType iGSUID, IN const UidType iUnitUID, IN const u_short usEventID, IN const T& data )
{
	UidType anTrace[2] = { iGSUID, -1 };
	KncSend( PI_GLOBAL_AUTO_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, iUnitUID, anTrace, usEventID, data );
}

template< class T >
void KAutoParty::BroadCast( IN const u_short usEventID, IN const T& data, IN const UidType iUnitUID_ /*= 0*/ )
{
	std::vector< UidType > vecAutoPartyUserList;
    m_kAutoPartyUserManager.GetAutoPartyUserList( vecAutoPartyUserList );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecAutoPartyUserList )
	{
		KAutoPartyUserPtr spAutoPartyUser = m_kAutoPartyUserManager.GetUser( iUnitUID );
		if( spAutoPartyUser == NULL )
			continue;

		if( iUnitUID_ != 0  &&  spAutoPartyUser->GetCharUID() == iUnitUID_ )
			continue;

		spAutoPartyUser->SendToGSCharacter( usEventID, data );
	}
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}



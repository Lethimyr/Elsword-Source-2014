#pragma once

#include <KncSmartPtr.h>
#include "MatchUser.h"
#include "Performer.h"
#include "FSM/support_FSM.h"
#include "MatchUserManager.h"
#include "MatchUserWaitList.h"
#include "X2Data/XSLRoom.h"
#include "Enum/Enum.h"


//{{ 2011. 06. 14	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM



//////////////////////////////////////////////////////////////////////////
// match state not
#define VERIFY_MATCH_STATE( varg ) \
if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
		<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
		<< L"    허용 상태 : "L ## #varg << END_LOG; \
	return; \
} \
START_LOG_WITH_NAME( clog ); \

//////////////////////////////////////////////////////////////////////////
// match state err
#define _VERIFY_MATCH_STATE_ACK( varg, eventid, packet, neterror ) \
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

#define VERIFY_MATCH_STATE_ACK( varg, eventid, packet ) _VERIFY_MATCH_STATE_ACK( varg, eventid, packet, NetError::ERR_UNKNOWN );

//////////////////////////////////////////////////////////////////////////
// match state warn
#define _VERIFY_MATCH_STATE_ACK_WARN( varg, eventid, packet, neterror ) \
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

#define VERIFY_MATCH_STATE_ACK_WARN( varg, eventid, packet ) _VERIFY_MATCH_STATE_ACK_WARN( varg, eventid, packet, NetError::ERR_UNKNOWN );


#define KMatchPtr KMatch*
#undef KMatchPtr
SmartPointer( KMatch );


class KMatch : public KPerformer
{
	DeclareFSM_MT;
	DeclPfID;

public:
	enum MATCH_TIMER_ENUM
	{
		MTE_MATCH_MAKING = 0,
		MTE_CHECK_ACCEPT,
		MTE_PREPARE_FOR_PLAY,
		MTE_PLAY,

		MTE_MAX,
	};

	KMatch( IN const UidType iMatchUID, IN const SEnum::MATCH_TYPE eMatchType );
    virtual ~KMatch();

	void		Tick();
	void		OnDestroy();

	void		CloseMatch( IN const int iReason, IN const UidType iCancelUnitUID = 0 );
	void		CloseMatchByCancel( IN const UidType iCancelUnitUID );

	//{{ 2012. 11. 28 던전,대전 자동 매칭 취소 되지 않는 오류 수정(예외 상황) - 김민성
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
	void		CloseMatchByCancel( IN const std::vector<UidType>& vecCancelUnitUID );
#endif SERV_AUTO_MATCH_CANCEL_ERROR
	//}

	// match info
	UidType		GetMatchUID() const			{ return GetUID(); }
	SEnum::MATCH_TYPE GetMatchType() const	{ return m_eMatchType; }
	bool		GetRoomInfoForPvpGame( IN const bool bIsNpcPvpMode, OUT KRoomInfo& kInfo ) const;
	int			GetNumMember() const		{ return m_kMatchUserManager.GetNumMember(); }

	// rank
	bool		IsAllPlayersSameRank( IN const CXSLUnit::PVP_RANK eRank )  { return m_kMatchUserManager.IsAllPlayersSameRank( eRank ); }

	// match function
	bool		Enter( IN const std::vector< KMatchUserInfo >& vecMatchUserList );
	bool		CheckOptimalMatch( IN KMatchUserWaitListPtr spWaitList, OUT int& iOptimalMatchPoint );
	bool		CheckMatchMakingSuccess();

	// static function
	static CXSLRoom::PVP_GAME_TYPE GetMatchPvpGameType( IN const SEnum::MATCH_TYPE eMatchType );
	static int	GetMatchPlayerCount( IN const SEnum::MATCH_TYPE eMatchType );
	static int	GetMatchTotalPlayerCount( IN const SEnum::MATCH_TYPE eMatchType );
	static bool GetMatchTypeByMatchInfo( IN const char cPvpGameType, IN const char cPvpPlayerCount, OUT SEnum::MATCH_TYPE& eMatchType );
	
	//////////////////////////////////////////////////////////////////////////
	// process by gsuser event..
protected:
	virtual inline void ProcessEvent( const KEventPtr& spEvent );
	
	DECL_ON_FUNC( EGB_CANCEL_MATCH_MAKING_REQ );
   _DECL_ON_FUNC( EGB_MATCH_MAKING_SUCCESS_REPLY_NOT, KEGS_MATCH_MAKING_SUCCESS_REPLY_NOT );
	DECL_ON_FUNC( EGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK );
	DECL_ON_FUNC( EGB_OPEN_PVP_ROOM_FOR_MATCH_ACK );

public:
	template< class T >	void SendToGSCharacter( IN const UidType iGSUID, IN const UidType iUnitUID, IN const u_short usEventID, IN const T& data );
	template< class T > void BroadCast( IN const u_short usEventID, IN const T& data, IN const UidType iUnitUID_ = 0 );

protected:
	SEnum::MATCH_TYPE		m_eMatchType;
	KMatchUserManager		m_kMatchUserManager;
	boost::timer			m_tTimer[MTE_MAX];
};

template < class T >
void KMatch::SendToGSCharacter( IN const UidType iGSUID, IN const UidType iUnitUID, IN const u_short usEventID, IN const T& data )
{
	UidType anTrace[2] = { iGSUID, -1 };
	KncSend( PI_GLOBAL_MATCH_MAKING, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, iUnitUID, anTrace, usEventID, data );
}

template< class T >
void KMatch::BroadCast( IN const u_short usEventID, IN const T& data, IN const UidType iUnitUID_ /*= 0*/ )
{
	std::vector< UidType > vecMatchUserList;
    m_kMatchUserManager.GetMatchUserList( vecMatchUserList );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecMatchUserList )
	{
		KMatchUserPtr spMatchUser = m_kMatchUserManager.GetUser( iUnitUID );
		if( spMatchUser == NULL )
			continue;

		if( iUnitUID_ != 0  &&  spMatchUser->GetCharUID() == iUnitUID_ )
			continue;

		spMatchUser->SendToGSCharacter( usEventID, data );
	}
}

#endif SERV_PVP_NEW_SYSTEM
//}}



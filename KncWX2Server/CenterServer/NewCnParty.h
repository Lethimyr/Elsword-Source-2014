// 2008. 12. 16. jseop. 파티 시스템.
//
// GSUser 객체에 접근 하므로 메인 쓰레드에서 이 클래스의 Tick을 수행하도록 한다.
//
// 파티의 멤버는 정회원과 후보 회원으로 이루어져 있다.
// 후보 회원은 정식 파티원이 아니고 편의적인 설정이므로 파티 관련 프로세스에서
// 배제되어야 한다.
// 정회원은 다음과 같다.
//  1. 파티를 만들어서 파티장이 된 회원.
//  2. 파티장의 초대를 수락하여 파티에 가입한 회원.
//  3. 파티장에게 가입 요청을 하여 파티장으로부터 승인된 회원.
// 파티장이 아닌 정회원이 누군가를 파티에 초대한 경우에는 내부적으로
// 초대받은 유저가 파티장에게 가입 요청을 하도록 처리한다.
// 즉 그 경우에 초대받은 유저가 정회원이 되려면 위의 3번의 과정을 거친다.
// 후보 회원은 다음과 같다.
//  1. 파티장이 초대했지만 파티 가입을 아직 수락하지 않은 회원.
//  2. 파티장에게 가입 요청을 했지만 아직 파티장이 승인하지 않은 회원.
// 위의 1번의 경우 파티장이 초대를 철회하거나 초대받은 회원이 거절하거나
// 10초가 지나면 해당 후보 회원은 파티를 탈퇴한다.
// 위의 2번의 경우 후보 회원이 가입 요청을 철회하거나 파티장이 거절하거나
// 10초가 지나면 해당 후보 회원은 파티를 탈퇴한다.
//
// 정회원과 후보 회원은 합해서 파티의 정원을 넘지 않아야 한다.
//
// 파티의 상태는 플레이 중인지 아닌지 구분한다.

#pragma once

#include <KncSmartPtr.h>
#include "CnPartyUser.h"
#include "../Common/X2Data/XSLDungeon.h"
#include "Performer.h"
#include "FSM/support_FSM.h"
#include "CnPartyUserManager.h"
#include "CnPartyGameManager.h"
//{{ 2012. 08. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "CnPartyFeverManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_CHANNEL_PARTY
//////////////////////////////////////////////////////////////////////////


#define KCnPartyPtr KCnParty*
#undef KCnPartyPtr
SmartPointer( KCnParty );

//////////////////////////////////////////////////////////////////////////
// party state ack err
#define _VERIFY_PARTY_STATE_ACK( varg, eventid, packet, neterror ) \
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

#define VERIFY_PARTY_STATE_ACK( varg, eventid, packet ) _VERIFY_PARTY_STATE_ACK( varg, eventid, packet, NetError::ERR_WRONG_STATE_00 );

//////////////////////////////////////////////////////////////////////////
// party state ack warn
#define _VERIFY_PARTY_STATE_ACK_WARN( varg, eventid, packet, neterror ) \
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

#define VERIFY_PARTY_STATE_ACK_WARN( varg, eventid, packet ) _VERIFY_PARTY_STATE_ACK_WARN( varg, eventid, packet, NetError::ERR_WRONG_STATE_00 );

//////////////////////////////////////////////////////////////////////////
// party state err
#define VERIFY_PARTY_STATE( varg ) \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #varg << END_LOG; \
	return; \
} \

//////////////////////////////////////////////////////////////////////////
// party state warn
#define VERIFY_PARTY_STATE_WARN( varg ) \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cwarn, L"상태 오류. Name : " << m_strName ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #varg << END_LOG; \
	return; \
} \


class KCnParty : public KPerformer
{
	DeclareFSM_MT;
	DeclPfID;

private:
	//{{ 2011. 07. 08	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	enum PARTY_ENUM
	{
		PE_LIMIT_PARTY_MEBMER = 4,
		PE_CLOSE_PARTY_NUM_MEMBER = 1,
	};
#endif SERV_PVP_NEW_SYSTEM
	//}}	

	//{{ 2012. 05. 17	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	enum TIMER_ENUM
	{
		TE_FIRST_INVITE = 0,
		TE_CHECK_PLAY_CONDITION,	// 플레이 조건 체크에 대한 시간제한 타이머
		TE_CHECK_AUTO_PARTY,		// 자동파티 조건 체크에 대한 시간제한 타이머
		TE_ACCEPT_FOR_PLAY,			// 플레이 수락 체크에 대한 시간제한 타이머
		TE_PREPARE_FOR_PLAY,		// 플레이 정보 수집에 대한 시간제한 타이머

		TE_MAX,
	};
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}


public:
	KCnParty( UidType iUID );
    virtual ~KCnParty();

	void	Tick();
		
	void	SetInfo( IN const KPartyInfo& kInfo );
	//{{ 2012. 01. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	SetInfo( IN const KCreatePartyInfo& kInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	void	GetInfo( OUT KPartyInfo& kInfo );
	void	GetInfo( OUT std::vector< KPartyInfo >& vecInfo );
	void	Reset();

	UidType GetUID() const												{ return m_iUID; }
	void	SetName( IN const std::wstring& wstrName )					{ m_wstrName = wstrName; }
	const std::wstring& GetName() const									{ return m_wstrName; }

	//{{ 2011. 06. 21	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	char	GetPartyType() const										{ return m_cPartyType; }
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	GetDungeonGameInfo( OUT KDungeonGameInfo& kInfo ) const;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	int		GetDungeonID() const										{ return m_iDungeonID; }
	char	GetDungeonDif() const										{ return m_cDifficultyLevel; }
	int		GetDungeonIDAndDif() const									{ return m_iDungeonID + static_cast<int>( m_cDifficultyLevel ); }
	char	GetItemType() const											{ return m_cGetItemType; }
	void	SetDungeonID( int iDungeonID )								{ m_iDungeonID = iDungeonID; }
	void	SetDungeonDif( char cDif )									{ m_cDifficultyLevel = cDif; }
	void	SetPublic( bool bPublic )									{ m_bPublic = bPublic; }	
	void	SetItemType( char cGetItemType )							{ m_cGetItemType = cGetItemType; }
	void	SetPlaying( bool bPlay )									{ m_bIsPlaying = bPlay; }

	CXSLDungeon::DUNGEON_MODE GetDungeonMode() const					{ return m_eDungeonMode; }
	void	SetDungeonMode( const CXSLDungeon::DUNGEON_MODE eDungeonMode );
    
	bool	IsPlaying() const											{ return m_bIsPlaying; } // 현재 던전방에 입장해 있다는 의미임. [추가] 함수 이름을 바꾸는건 어떨까?
	bool	IsPublic() const											{ return m_bPublic; }	

	// 쿨타임 초기화
	void	AllPartyMembersQuickSlotCoolTimeReset();

	// fever
	void	UpdateAndBroadCastFeverPoint( IN const int iFeverPoint );
	void	IncreaseFeverPoint( IN const char cRoomType, IN const short sSubStageNpcCount );
	void	DecreaseFeverPoint( IN const int iDecreaseFeverPoint );
	short	GetFeverPoint() const										{ return m_kFeverManager.GetFeverPoint( m_kPartyUserManager.GetNumMember() ); } 
		
	void	AddPartyInfoInPartyList();													// 게임서버의 파티 리스트에 파티 정보 추가
	//{{ 2011. 07. 11	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
	void	DelPartyInfoInPartyList( IN const bool bForce = false );					// 게임서버의 파티 리스트에 파티 정보 삭제
	void	UpdatePartyInfoInPartyList( IN const bool bForce = false );					// 게임서버의 파티 리스트에 현재 파티 정보 업데이트	
//#else
//	void	DelPartyInfoInPartyList();													// 게임서버의 파티 리스트에 파티 정보 삭제
//	void	UpdatePartyInfoInPartyList();												// 게임서버의 파티 리스트에 현재 파티 정보 업데이트
//#endif SERV_PVP_NEW_SYSTEM
	//}}		

	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	UpdatePartyFeverInPartyList( IN const bool bForce = false );					// 게임서버의 파티 리스트에 현재 파티 피버 업데이트	
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	
	void	AddPartyMemberInPartyList( IN const KPartyUserInfo& kInfo );				// 게임서버의 파티 리스트에 파티 멤버 추가
	//{{ 2011. 07. 11	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
	void	DelPartyMemberInPartyList( IN const UidType iUnitUID, IN const bool bForce = false );	// 게임서버의 파티 리스트에 파티 멤버 삭제
//#else
//	void	DelPartyMemberInPartyList( IN UidType iUnitUID );							// 게임서버의 파티 리스트에 파티 멤버 삭제
//#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2012. 03. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	UpdateJoinRoomInfoAndGetOnOffResult( IN const UidType iUnitUID, IN const KPartyUserJoinRoomInfo& kPartyUserJoinRoomInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	void	SetCheckLowLevel( IN bool bCheck )			{ m_bCheckLowLevel = bCheck; }
	bool	GetCheckLowLevel()							{ return m_bCheckLowLevel; }
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	//{{ 2012. 05. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KOldPartyInfo MakeOldPartyInfo() const;

	// 플레이 조건 체크
	void	CheckAllMembersPlayCondition();
	void	FailProcessPlayCondition( IN const int iNetError, IN const UidType iFailUnitUID = 0, IN const std::wstring wstrFailUserNickName = L"" );

	// 자동 파티 조건 체크
	void	CheckAllMembersAutoParty();
	void	FailProcessAutoParty( IN const int iNetError, IN const std::wstring wstrFailUserNickName = L"" );

	// 플레이 수락 체크
	void	CheckAllMembersAcceptForPlay();
	void	FailProcessAcceptForPlay( IN const int iNetError, IN const UidType iFailUnitUID = 0 );

	// 게임 시작 정보 수집 체크
	void	FailProcessPrepareForPlay( IN const int iNetError );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

protected:
	//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool	LeaveCandidateMember( IN const UidType iUnitUID );
	bool	LeaveMember( IN const UidType iUnitUID 
					#ifdef LOG_PARTY_BREAK
						, IN const int iReason	
					#endif // LOG_PARTY_BREAK
						);
	void	CloseParty( IN const int iReason );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//////////////////////////////////////////////////////////////////////////
	// process by gsuser event..
protected:
	virtual inline void ProcessEvent( const KEventPtr& spEvent );
	
	DECL_ON_FUNC( EPM_CREATE_PARTY_BY_INVITE_REQ );
	DECL_ON_FUNC( EPM_INVITE_PARTY_REQ );
	DECL_ON_FUNC( EPM_INVITE_PARTY_REPLY_NOT );
	DECL_ON_FUNC( EPM_UPDATE_PARTY_USER_INFO_NOT );
	DECL_ON_FUNC( EPM_LEAVE_PARTY_REQ );
	DECL_ON_FUNC( EPM_PARTY_CHANGE_READY_REQ );
	DECL_ON_FUNC( EPM_PARTY_CHANGE_DUNGEON_REQ );
	DECL_ON_FUNC( EPM_PARTY_GAME_START_REQ );
	//{{ 2012. 01. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK );
   _DECL_ON_FUNC( EPM_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT, KEGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	DECL_ON_FUNC( EPM_CHECK_FOR_PARTY_GAME_START_ACK );
	DECL_ON_FUNC( EPM_END_GAME_NOT );	
	DECL_ON_FUNC( EPM_PARTY_CHANGE_HOST_REQ );
	DECL_ON_FUNC( EPM_CHECK_PARTY_CHANGE_HOST_ACK );
	DECL_ON_FUNC( EPM_PARTY_CHANGE_NUM_OF_PER_REQ );
	DECL_ON_FUNC( EPM_PARTY_CHANGE_GET_ITEM_TYPE_REQ );
	DECL_ON_FUNC( EPM_PARTY_BAN_USER_REQ );
	DECL_ON_FUNC( EPM_CHAT_REQ );
	DECL_ON_FUNC( EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ );
   _DECL_ON_FUNC( EPM_INCREASE_PARTY_FEVER_NOT, KERM_INCREASE_PARTY_FEVER_NOT );
	//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _DECL_ON_FUNC( EPM_MAX_PARTY_FEVER_NOT, KERM_MAX_PARTY_FEVER_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	DECL_ON_FUNC( EPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT );
   _DECL_ON_FUNC( EPM_CHECK_INVALID_USER_NOT, UidType );

	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	DECL_ON_FUNC( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
   _DECL_ON_FUNC( EPM_CHANGE_PARTY_TYPE_REQ, KEGS_CHANGE_PARTY_TYPE_REQ );
	DECL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ );
   _DECL_ON_FUNC( EPM_PVP_PARTY_CHANGE_MATCH_INFO_REQ, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ );
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_REQ );
	DECL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ );
	DECL_ON_FUNC( EPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK );
	DECL_ON_FUNC( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ );
	DECL_ON_FUNC( EPM_REQUEST_REGROUP_PARTY_NOT );
	DECL_ON_FUNC_NOPARAM( ERM_END_DUNGEON_GAME_PARTY_FEVER_MODIFY_NOT );
	DECL_ON_FUNC( EPM_UPDATE_PARTY_USER_JOIN_ROOM_INFO_NOT );
	DECL_ON_FUNC_NOPARAM( EPM_END_GAME_REGROUP_PARTY_NOT );

   _DECL_ON_FUNC( EPM_UPDATE_GAME_PLAY_STATUS_NOT, KPartyMemberStatus );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
   //{{ 2012. 05. 30	최육사		공식 대전 버그 수정
#ifdef SERV_OFFCIAL_PVP_BUG_FIX
   DECL_ON_FUNC( EPM_UPDATE_PVP_INFO_NOT );
#endif SERV_OFFCIAL_PVP_BUG_FIX
   //}}

public:
	// SendTo
	template< class T >	void SendToGSCharacter( IN UidType iGSUID, IN UidType iUnitUID, IN unsigned short usEventID, IN const T& data );

	// BroadCast
	template< class T >	void BroadCastGameServer( IN unsigned short usEventID, IN const T& data );
	template< class T >	void BroadCast( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ = 0 );
	//{{ 2012. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	template< class T >	void BroadCastCadidateUser( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ = 0 );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	void BroadCastID( IN u_short usEventID, IN const UidType iUnitUID_ = 0 );

protected:
	const UidType						m_iUID;							//UID는 한번 받으면 건들지 않아야 한다.
	std::wstring						m_wstrName;
	//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	boost::timer						m_tTimer[TE_MAX];
	bool								m_bRemakingAutoParty;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	int									m_iDungeonID;
	//{{ 2011. 06. 28	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
    char								m_cPartyType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	char								m_cDifficultyLevel;	
	bool								m_bPublic;
	char								m_cGetItemType;
    bool								m_bIsPlaying;					// 던전 게임 플레이 중인지의 여부

	CXSLDungeon::DUNGEON_MODE			m_eDungeonMode;	

	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	bool								m_bCheckLowLevel;
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	char								m_cPvpGameType; // CXSLRoom::PVP_GAME_TYPE
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KCnPartyUserManager					m_kPartyUserManager;	// 파티 유저 매니저
	KCnPartyGameManager					m_kGameManager;			// 게임 시작 관리자
	//{{ 2012. 08. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KCnPartyFeverManager				m_kFeverManager;		// 파티 fever 매니저
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
};

template < class T >
void KCnParty::SendToGSCharacter( IN UidType iGSUID, IN UidType iUnitUID, IN unsigned short usEventID, IN const T& data )
{
	UidType anTrace[2] = { iGSUID, -1 };
	KncSend( PI_CN_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, iUnitUID, anTrace, usEventID, data );
}

template < class T >
void KCnParty::BroadCastGameServer( IN unsigned short usEventID, IN const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, usEventID, data );
}

template< class T >
void KCnParty::BroadCast( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ /*= 0*/ )
{
    m_kPartyUserManager.BroadCastToGSCharacter( usEventID, data, iUnitUID_ );
}

//{{ 2012. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
template< class T >
void KCnParty::BroadCastCadidateUser( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ /*= 0*/ )
{
	m_kPartyUserManager.BroadCastToGSCharacter_CadidateUser( usEventID, data, iUnitUID_ );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//////////////////////////////////////////////////////////////////////////
#endif SERV_CHANNEL_PARTY
//////////////////////////////////////////////////////////////////////////


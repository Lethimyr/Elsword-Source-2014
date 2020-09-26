#pragma once
#include "Actor.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"
//#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ActorManager.h"
#include "BaseServer.h"

#include <KncSmartPtr.h>

//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	#include "GuildSkillTree.h"
#endif GUILD_SKILL_TEST
//}}

//{{ 2009. 6. 10  최육사	타이머 매니저
#include <boost/timer.hpp>
//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
	#include "Enum/Enum.h"
#endif	// SERV_LOCAL_RANKING_SYSTEM

#define KGuildMemberInfoPtr KGuildMemberInfo*
#undef KGuildMemberInfoPtr
SmartPointers( KGuildMemberInfo );

#define KGuildPtr KGuild*
#undef KGuildPtr
SmartPointer( KGuild );

#ifdef GUILD_TEST


class KGuild
{
	enum GUILD_INFO
	{
		MEMBER_PAGE_PER_COUNT = 8,
	};

	enum ENUM_TIMER
	{
		TM_DB_UPDATE = 0,						// DB 업데이트 주기
		TM_CHECK_GUILD_CASH_SKILL_POINT,

		TM_TOTAL_NUM,
	};

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
	enum LOCAL_RANKING_MANAGER
	{
		LRM_NONE				= 0,
		LRM_SPIRIT_CHANGED		= 1,
		LRM_AP_CHANGED			= 2,
		LRM_INSERT_DATA			= 3,
		LRM_DATA_CHANGED		= 4,
	};

	typedef	std::multimap< int, UidType, std::greater<int> >	RankMMAP;
#endif	// SERV_LOCAL_RANKING_SYSTEM

public:
	KGuild( const KGuildInfo& kGuildInfo );
	virtual ~KGuild(void);

	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	void Tick();
#endif GUILD_SKILL_TEST
	//}}
	
	//////////////////////////////////////////////////////////////////////////
	// 길드 정보 & 길드원 정보
	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	void GetGuildInfo( OUT KGuildInfo& kGuildInfo, OUT std::wstring& wstrGuildMasterName, OUT std::vector< UidType >& vecMemberUnitUID, OUT KGuildSkillInfo& kGuildSkillInfo ) const;
#else
	void GetGuildInfo( OUT KGuildInfo& kGuildInfo, OUT std::wstring& wstrGuildMasterName, OUT std::vector< UidType >& vecMemberUnitUID ) const;
#endif GUILD_SKILL_TEST
	//}}
	void GetGuildInfo( OUT KGuildInfo& kGuildInfo ) const	{ kGuildInfo = m_kGuildInfo; }
	int  GetUID() const										{ return m_kGuildInfo.m_iGuildUID; }
	u_char GetGuildLevel()									{ return m_kGuildInfo.m_ucGuildLevel; }
	std::wstring GetGuildName()								{ return m_kGuildInfo.m_wstrGuildName; }
	void GetGuildSkillInfo( OUT KGuildSkillInfo& kInfo )	{ m_kSkillTree.GetGuildSkillInfo( kInfo ); }
	
	KGuildMemberInfoPtr GetGuildMember( IN UidType iUnitUID ) const;	
	bool IsGuildMember( IN UidType iUnitUID ) const			{ return ( m_mapGuildMember.find( iUnitUID ) != m_mapGuildMember.end() ); }
	bool IsLoginedMember( IN UidType iUnitUID );
	bool IsGuildMaster( IN UidType iUnitUID ) const			{ return ( iUnitUID == m_iGuildMasterUnitUID ); }
	int  GetNumMember() const								{ return static_cast<int>(m_mapGuildMember.size()); }
	int  GetMaxMember()	const								{ return m_kGuildInfo.m_usMaxNumMember; }	
	int  GetWaitingJoinNumMember() const					{ return static_cast<int>(m_setWaitingForJoinUserList.size()); }	
	int	 GetLoginedMemberCount() const						{ return m_iLoginedMemberCount; }

	void UpdateGuildMemberList( IN const std::vector< KGuildMemberInfo >& vecMemberInfo );
	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	void UpdateGuildSkill( IN const KGuildSkillInfo& kInfo );
#endif GUILD_SKILL_TEST
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 길드 관리 기능
	bool CheckJoinGuild( IN UidType iUnitUID, OUT UidType& iGuildMasterUnitUID );
	void RemoveReserve( IN UidType iUnitUID );

	bool JoinGuildMember( IN const KGuildMemberInfo& kMemberInfo, IN bool bIsLoginUser );						// 길드 가입
	bool KickGuildMemeber( IN UidType iUnitUID, IN UidType iTargetUnitUID );									// 길드 탈퇴 & 강퇴
	
	bool LoginGuildMember( IN const KGuildMemberInfo& kMemberInfo );											// 길드원 로그인
	bool LogoutGuildMember( IN UidType iUnitUID );																// 길드원 로그아웃
	
	bool CheckInviteCondition( IN UidType iReceiverUnitUID, IN UidType iSenderUnitUID, OUT KELG_INVITE_GUILD_FIND_USER_REQ& kPacket ) const;				// 길드원 초대 가능
	void GetGuildUserList( IN const KELG_GET_GUILD_USER_LIST_REQ& kReq, OUT KEGS_GET_GUILD_USER_LIST_ACK& kAck ) const;										// 길드 유저 리스트	

	void GuildChat( IN const KELG_CHAT_REQ& kInfo ) const;														// 길드 채팅

	bool CheckChangeGradeGuild( IN const KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ& kReq, OUT KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_ACK& kAck ) const;		// 길드 관리자 임명 체크	
	bool UpdateChangeGuildMemberGrade( IN const KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK& kInfo );					// 길드 멤버 등급 변경
	void UpdateChangeGuildMessage( IN const KDBE_CHANGE_GUILD_MESSAGE_ACK& kInfo );								// 길드 메시지 변경
	bool UpdateChangeGuildMemberMessage( IN const KDBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK& kInfo );				// 길드원 인사말 변경
	bool UpdateGuildMemberInfo( IN const KGuildMemberInfo& kInfo );												// 길드 멤버 정보 갱신

	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
	void SetGuildName( IN const std::wstring wstrGuildName ){ m_kGuildInfo.m_wstrGuildName = wstrGuildName; }
	template < class T > void BroadCast( unsigned short usEventID, const T& data ) const;
#endif SERV_GUILD_CHANGE_NAME
	//}}

	bool CheckGuildMaxMember( IN const KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ& kInfo );							// 길드 최대 인원 확장 가능 여부 검사
	void UpdateGuildMaxMember( IN u_short usMaxNumMember );														// 길드 최대 인원 확장

	//{{ 2009. 10. 27  최육사	길드레벨
	void UpdateGuildEXP( IN const KDBE_UPDATE_GUILD_EXP_ACK& kInfo );											// 길드 경험치
	void UpdateGuildLevelResult( IN const KDBE_GUILD_LEVEL_UP_ACK& kInfo );										// 길드 레벨 업데이트 결과 처리
	//}}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

	bool CheckLearnGuildSkill( IN const KELG_GET_GUILD_SKILL_REQ& kReq, OUT KDBE_GET_GUILD_SKILL_REQ& kPacketToDB );
	bool InsertGuildSkill( IN const KDBE_GET_GUILD_SKILL_ACK& kAck );

	bool CheckResetGuildSkill( IN const KELG_RESET_GUILD_SKILL_REQ& kReq, OUT KDBE_RESET_GUILD_SKILL_REQ& kPacketToDB );
	void ResetGuildSkill( IN const KDBE_RESET_GUILD_SKILL_ACK& kAck );

	bool CheckInitGuildSkill( IN const KELG_INIT_GUILD_SKILL_REQ& kReq, OUT KDBE_INIT_GUILD_SKILL_REQ& kPacketToDB );
	void InitGuildSkill( IN const KDBE_INIT_GUILD_SKILL_ACK& kAck );

	bool InsertGuildCashSkillPoint( IN const KELG_INSERT_GUILD_CASH_SKILL_POINT_NOT& kNot );
	void InsertGuildCashSkillPointResult( IN const KDBE_INSERT_GUILD_CASH_SKILL_POINT_ACK& kAck );

	void ExpireGuildCashSkillPointResult( IN const KDBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK& kAck );

	bool AdminInsertGuildSkillPoint( IN const KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ& kReq );
	void AdminInsertGuildSkillPointResult( IN const KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK& kAck );

#endif GUILD_SKILL_TEST
	//}}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 01. 13  최육사	길드게시판
#ifdef SERV_GUILD_AD	
	bool CheckRegistrationGuildAd( IN UidType iUnitUID );
	bool MakeGuildAdInfo( IN const KDBE_REGISTRATION_GUILD_AD_ACK& kAck, OUT KGuildAdInfo& kInfo );
	void ResultRegGuildAd( IN const std::wstring& wstrEndDate );
	
	bool CheckGetApplyJoinGuildList( IN const KELG_GET_APPLY_JOIN_GUILD_LIST_REQ& kReq );
	bool CheckDeleteApplyJoin( IN const KELG_DELETE_APPLY_JOIN_GUILD_REQ& kReq );
#endif SERV_GUILD_AD
	//}}
	//////////////////////////////////////////////////////////////////////////	

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
	void RankingClear( void );

	void UpdateGuildMemberRanking( IN int iGuildUID
		, IN const std::map<UidType, KLocalRankingUnitInfo>& mapRankerUnitInfo
		, IN const std::map<UidType, UidType>& mapRankerUIDInfo
		, IN const std::map<UidType, KLocalRankingUserInfo>& mapRankerUserInfo
		);

	void	GetRankerList( IN OUT UINT& nViewPage
		, IN const char cMainTabIndex
		, IN const byte byteFilter
		, IN const std::set<UidType>& setFriendList
		, OUT UINT& nTotalPage
		, OUT std::vector<KLocalRankingRankerInfo>& vecRankerList
		, OUT KLocalRankingRankerInfo& kPrevRankerInfo
		, IN OUT KLocalRankingRankerInfo& kMyRankInfo
		);

	void	_GetRankerList( IN const RankMMAP& mmapRanker
		, IN OUT UINT& nViewPage
		, OUT UINT& nTotalPage
		, OUT std::vector<KLocalRankingRankerInfo>& vecRankerList
		, OUT KLocalRankingRankerInfo& kPrevRankerInfo
		, IN OUT KLocalRankingRankerInfo& kMyRankInfo
		) const;

	void	GetUserUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUserInfo& kUserInfo , OUT KLocalRankingUnitInfo& kUnitInfo, IN const bool bUserFilter = false ) const;

	bool	SomethingChanged( IN const KLocalRankingUserInfo& kUserInfo
		, IN const KLocalRankingUnitInfo& kUnitInfo
		, OUT std::vector<KELG_LOCAL_RANKING_RANK_UP_NOT>& vecRankUpNot
		);

	void	ResetLocalRankingInfo( void );
#endif	// SERV_LOCAL_RANKING_SYSTEM

protected:
	//{{ 2009. 11. 19  최육사	길드구조개선
	template < class T > void SendToGameDB( unsigned short usEventID, const T& data );
	//}}
	template < class T > void SendToGameServer( unsigned short usEventID, const T& data ) const;
	template < class T > void SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data ) const;
	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
#else
	template < class T > void BroadCast( unsigned short usEventID, const T& data ) const;
#endif SERV_GUILD_CHANGE_NAME
	//}}

	// 길드 유저 정보 생성 & 삭제
	bool InsertGuildMember( IN const KGuildMemberInfo& kMemberInfo );
	bool DeleteGuildMember( IN UidType iUnitUID );

	// 길드 유저 리스트 얻기
	void GetGuildMemberPositionGrade( IN OUT UINT& uiViewPage, OUT UINT& uiTotalPage, OUT std::vector< KGuildMemberInfo >& vecMemberList ) const;
	void GetGuildMemberMessageInfo( IN OUT UINT& uiViewPage, OUT UINT& uiTotalPage, OUT std::vector< KGuildMemberMessageInfo >& vecMemberList ) const;

	int  GetGuildGradeMemberCount( IN u_char ucMemberShipGrade ) const;

	// 길드 유저 리스트 정렬조건함수
	void GuildMemberSort();
	static bool GuildMemberGradeSort( const KGuildMemberInfoPtr spFirst, const KGuildMemberInfoPtr spSecond );

	// 길드 레벨
	void CheckGuildLevelUp();

	// 로그인한 길드 멤버
	int	 LoginedMemberCounting() const;

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
	int		CheckChangedInfo( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo ) const;

	void	RankingProcess( IN const int iChangeFlag
		, IN const KLocalRankingUserInfo& kUserInfo
		, IN const KLocalRankingUnitInfo& kUnitInfo
		, OUT std::vector<KELG_LOCAL_RANKING_RANK_UP_NOT>& vecRankUpNot
		);

	void	DataInsert( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo );
	void	DataRemove( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo );
	void	DataUpdate( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo );

	bool	Remove( IN const int iMainTabIndex
		, IN int iCategory
		, IN const KLocalRankingUserInfo& kUserInfo
		, IN const KLocalRankingUnitInfo& kUnitInfo
		);

	bool	Add( IN const int iMainTabIndex
		, IN int iCategory
		, IN const int iAccumulatedPoint
		, IN const KLocalRankingUserInfo& kUserInfo
		, IN const KLocalRankingUnitInfo& kUnitInfo
		);

	bool	Remove( IN OUT RankMMAP& mmapRanker
		, IN const KLocalRankingUserInfo& kUserInfo
		, IN const KLocalRankingUnitInfo& kUnitInfo
		);

	bool	Add( IN OUT RankMMAP& mmapRanker
		, IN const int iAccumulatedPoint
		, IN const KLocalRankingUserInfo& kUserInfo
		, IN const KLocalRankingUnitInfo& kUnitInfo
		);

	int		GetRankAndNextUnitUID( IN const int iMainTabIndex, IN const int iCategory, IN const UidType iMyUnitUID, OUT UidType& iNextUnitUID ) const;
	void	GetUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUnitInfo& kInfo ) const;
#endif	// SERV_LOCAL_RANKING_SYSTEM

private:
	KGuildInfo									m_kGuildInfo;
	UidType										m_iGuildMasterUnitUID;
	int											m_iLoginedMemberCount;

	std::map< UidType, KGuildMemberInfoPtr >	m_mapGuildMember;
	std::vector< KGuildMemberInfoPtr >			m_vecGuildMember;

	//{{ 2009. 11. 4  최육사	길드 가입
	std::set< UidType >							m_setWaitingForJoinUserList;
	//}}

	//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	KGuildSkillTree								m_kSkillTree;

	boost::timer								m_kTimer[TM_TOTAL_NUM];
#endif GUILD_SKILL_TEST
	//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
	std::map< std::pair< int, int >, RankMMAP >		m_mapRankerTab;			// 랭크 비교를 위한 컨테이너
	std::map<UidType, KLocalRankingUserInfoCounter>	m_mapRankerUserInfo;	// 랭커들의 실제 유저 정보를 보관하는 컨테이너
	std::map<UidType, KLocalRankingUnitInfoCounter>	m_mapRankerUnitInfo;	// 랭커들의 실제 유닛 정보를 보관하는 컨테이너

	std::map< UidType, std::set< UidType > >		m_mapUserUnitList;		// 랭커의 UserUID에 해당하는 Unit 리스트
#endif	// SERV_LOCAL_RANKING_SYSTEM
};

//{{ 2009. 11. 19  최육사	길드구조개선
template < class T >
void KGuild::SendToGameDB( unsigned short usEventID_, const T& data_ )
{
	UidType anTrace[2] = { -1, -1 };

	if( anTrace[0] == -1 )
		anTrace[0] = GetUID();
	else
		anTrace[1] = GetUID();

	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_GAME_DB, 0, anTrace, usEventID_, data_ );
}
//}}

template < class T >
void KGuild::SendToGameServer( unsigned short usEventID, const T& data ) const
{
	// 연결된 게임서버중 한군데만 전송
	UidType iGameServerUID = KBaseServer::GetKObj()->GetFirstActorKey();
	if( iGameServerUID <= 0 )
	{
		START_LOG( cwarn, L"접속된 게임서버가 없습니다." )
			<< BUILD_LOG( iGameServerUID )
			<< END_LOG;
		return;
	}
	
	UidType anTrace[2] = { iGameServerUID, -1 };
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, iGameServerUID, anTrace, usEventID, data );
}

template < class T >
void KGuild::SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data ) const
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_CHARACTER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToUnitUID ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"타 게임서버로 패킷 보내기 실패.? 왜왜" )
			<< BUILD_LOG( nToGSUID )
			<< BUILD_LOG( nToUnitUID )
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}

template < class T >
void KGuild::BroadCast( unsigned short usEventID, const T& data ) const
{
	std::vector< KGuildMemberInfoPtr >::const_iterator vit;
	for( vit = m_vecGuildMember.begin(); vit != m_vecGuildMember.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->m_iGSUID == 0 )
			continue;
		
		SendToGSCharacter( (*vit)->m_iGSUID, (*vit)->m_iUnitUID, usEventID, data );
	}
}

#endif GUILD_TEST



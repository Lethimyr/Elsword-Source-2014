#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <map>
#include "Guild.h"
#include <boost/timer.hpp>

#ifdef GUILD_TEST


//////////////////////////////////////////////////////////////////////////
// Guild Manager : 길드 정보를 관리

class KGuildManager
{
	DeclareSingleton( KGuildManager );
	DeclDump;
	DeclToStringW;

public:
	KGuildManager(void);
	~KGuildManager(void);

	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	void Tick();
#endif GUILD_SKILL_TEST
	//}}

	bool IsExistGuild( IN int iGuildUID ) { return ( m_mapGuild.find( iGuildUID ) != m_mapGuild.end() ); }
	KGuildPtr GetGuild( IN int iGuildUID );

	//////////////////////////////////////////////////////////////////////////
	// 길드 정보 얻기 & 업데이트
	bool GetGuildInfo( IN int iGuildUID, OUT KEGS_GUILD_INFO_NOT& kInfo );
	//}}
	bool GetGuildSkillInfo( IN int iGuildUID, OUT KGuildSkillInfo& kInfo );
	bool UpdateGuildInfo( IN const KGuildInfo& kGuildInfo );
	bool UpdateGuildMemberList( IN int iGuildUID, IN const std::vector< KGuildMemberInfo >& vecMemberInfo );	// 길드원 리스트 업데이트

	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	bool UpdateGuildSkill( IN int iGuildUID, IN const KGuildSkillInfo& kInfo );
#endif GUILD_SKILL_TEST
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 길드 관리 기능
	bool CheckJoinGuild( IN int iGuildUID, IN UidType iUnitUID, OUT UidType& iGuildMasterUnitUID );
	bool RemoveReserve( IN int iGuildUID, IN UidType iUnitUID );

	bool JoinGuildMember( IN int iGuildUID, IN const KGuildMemberInfo& kMemberInfo, IN bool bIsLoginUser );		// 길드 가입
	bool LoginGuildMember( IN int iGuildUID, IN const KGuildMemberInfo& kMemberInfo );							// 길드원 로그인
	bool LogoutGuildMember( IN int iGuildUID, IN UidType iUnitUID );											// 길드원 로그아웃
	
	bool CheckInviteCondition( IN int iGuildUID, IN UidType iReceiverUnitUID, IN UidType iSenderUnitUID, OUT KELG_INVITE_GUILD_FIND_USER_REQ& kPacket );	// 가입 초대
	bool GetGuildUserList( IN const KELG_GET_GUILD_USER_LIST_REQ& kReq, OUT KEGS_GET_GUILD_USER_LIST_ACK& kAck );									// 길드 유저 리스트
	
	bool GuildChat( IN const KELG_CHAT_REQ& kInfo );															// 길드 채팅

	bool CheckChangeGradeGuild( IN const KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ& kReq, OUT KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_ACK& kAck );		// 길드 멤버 등급 변경 체크
	bool UpdateChangeGuildMemberGrade( IN const KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK& kInfo );					// 길드 멤버 등급 변경
	bool UpdateChangeGuildMessage( IN const KDBE_CHANGE_GUILD_MESSAGE_ACK& kInfo );								// 길드 메시지 변경
	bool UpdateKickGuildMember( IN const KDBE_KICK_GUILD_MEMBER_ACK& kInfo, OUT std::wstring& wstrGuildName );	// 길드원 탈퇴
	bool UpdateChangeGuildMemberMessage( IN const KDBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK& kInfo );				// 길드원 인사말 변경
	bool DisbandGuild( IN int iGuildUID );																		// 길드 해산
	bool UpdateGuildMemberInfo( IN int iGuildUID, IN const KGuildMemberInfo& kInfo );							// 길드 멤버 정보 변경

	bool CheckGuildMaxMember( IN const KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ& kInfo );							// 길드 최대 인원 확장 가능 여부 검사
	bool UpdateGuildMaxMember( IN int iGuildUID, IN u_short usMaxNumMember );									// 길드 최대 인원 확장

	//{{ 2009. 10. 27  최육사	길드레벨
	bool UpdateGuildEXP( IN int iGuildUID, IN const KDBE_UPDATE_GUILD_EXP_ACK& kInfo );
	bool UpdateGuildLevelResult( IN int iGuildUID, IN const KDBE_GUILD_LEVEL_UP_ACK& kInfo );
	//}}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

	bool CheckLearnGuildSkill( IN const KELG_GET_GUILD_SKILL_REQ& kReq, OUT KDBE_GET_GUILD_SKILL_REQ& kPacketToDB );
	bool InsertGuildSkill( IN const KDBE_GET_GUILD_SKILL_ACK& kAck );

	bool CheckResetGuildSkill( IN const KELG_RESET_GUILD_SKILL_REQ& kReq, OUT KDBE_RESET_GUILD_SKILL_REQ& kPacketToDB );
	bool ResetGuildSkill( IN const KDBE_RESET_GUILD_SKILL_ACK& kAck );

	bool CheckInitGuildSkill( IN const KELG_INIT_GUILD_SKILL_REQ& kReq, OUT KDBE_INIT_GUILD_SKILL_REQ& kPacketToDB );
	bool InitGuildSkill( IN const KDBE_INIT_GUILD_SKILL_ACK& kAck );

	bool InsertGuildCashSkillPoint( IN const KELG_INSERT_GUILD_CASH_SKILL_POINT_NOT& kNot );
	bool InsertGuildCashSkillPointResult( IN const KDBE_INSERT_GUILD_CASH_SKILL_POINT_ACK& kAck );

	bool ExpireGuildCashSkillPointResult( IN const KDBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK& kAck );

	bool AdminInsertGuildSkillPoint( IN const KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ& kReq );
	bool AdminInsertGuildSkillPointResult( IN const KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK& kAck );

#endif GUILD_SKILL_TEST
	//}}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 01. 13  최육사	길드게시판
#ifdef SERV_GUILD_AD	
	
	bool CheckRegistrationGuildAd( IN int iGuildUID, IN UidType iUnitUID );
	bool MakeGuildAdInfo( IN const KDBE_REGISTRATION_GUILD_AD_ACK& kAck, OUT KGuildAdInfo& kInfo );
	bool ResultRegGuildAd( IN int iGuildUID, IN const std::wstring& wstrEndDate );
	
	bool CheckGetApplyJoinGuildList( IN const KELG_GET_APPLY_JOIN_GUILD_LIST_REQ& kReq );
	bool CheckDeleteApplyJoin( IN const KELG_DELETE_APPLY_JOIN_GUILD_REQ& kReq );

#endif SERV_GUILD_AD
	//}}
	//////////////////////////////////////////////////////////////////////////	

	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
	bool ReserveGuildName( IN const UidType& iUnitUID, IN const std::wstring& wstrGuildName );
	bool CancelGuildName( IN const UidType& iUnitUID );
	bool GetReservedGuildName( IN const UidType& iUnitUID, OUT std::wstring& wstrReservedGuildName );
	bool CheckReservedGuildName( IN const UidType& iUnitUID );
	void TimeRefresh( IN const UidType& iUnitUID );
#endif SERV_GUILD_CHANGE_NAME
	//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
	void ResetLocalRankingInfo( void );
#endif	// SERV_LOCAL_RANKING_SYSTEM

private:
	void InsertGuild( IN const KGuildInfo& kInfo );
	void DeleteGuild( IN int iGuildUID );
	void DeleteReserve( IN int iGuildUID );

private:
	std::map< int, KGuildPtr >		m_mapGuild;
	std::vector< KGuildPtr >		m_vecGuild;

	//{{ 2009. 12. 2  최육사	길드구조개선
	std::vector< int >				m_vecDelReserved;
	//}}

	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
	struct ReservedGuildName
	{
		CTime			m_dRecordTime;
		UidType			m_iUnitUID;
		std::wstring	m_wstrGuildName;

		ReservedGuildName( IN const CTime& dRecordTime, IN const UidType& iUnitUID, IN const std::wstring& wstrGuildName )
		{
			m_dRecordTime = dRecordTime;
			m_iUnitUID = iUnitUID;
			m_wstrGuildName = wstrGuildName;
		}
	};

	std::list< ReservedGuildName >		m_listReservedGuildName;
	boost::timer						m_kTimer;
#endif SERV_GUILD_CHANGE_NAME
	//}}
};

DefSingletonInline( KGuildManager );


#endif GUILD_TEST



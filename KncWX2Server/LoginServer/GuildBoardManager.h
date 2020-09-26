#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <map>
#include "GuildAd.h"
#include <boost/timer.hpp>
#include "KncSend.h"


//////////////////////////////////////////////////////////////////////////
// Guild Board Manager : 길드 게시판
#ifdef SERV_GUILD_AD


class KGuildBoardManager
{
	DeclareSingleton( KGuildBoardManager );
	DeclDump;
	DeclToStringW;

	enum AD_ENUM
	{
		AE_PAGE_PER_COUNT = 6,
	};

	enum AD_SORT_TYPE
	{
		AST_REG_DATE = 0,
		AST_NEW_GUILD,
		AST_HOT_GUILD,
		AST_MEMBER_COUNT,
		AST_GUILD_LEVEL,

		AST_MAX,
	};

	enum ENUM_TIMER
	{		
		TM_CHECK_GUILD_AD_PERIOD = 0,
		TM_CHECK_GUILD_AD_DELETE_TIME,
		TM_SORT_GUILD_AD,

		TM_TOTAL_NUM,
	};

public:
	KGuildBoardManager(void);
	~KGuildBoardManager(void);
	
	void Init();
	void UpdateGuildBoardInfo( IN const KDBE_GET_GUILD_BOARD_INFO_ACK& kInfo );

	void Tick();

	bool IsExistGuildAd( IN int iGuildUID )		{ return ( m_mapGuildAd.find( iGuildUID ) != m_mapGuildAd.end() ); }
	KGuildAdPtr GetGuildAd( IN int iGuildUID );

	bool IsExpiredGuildAd( IN int iGuildUID );

	bool IsExistApplyJoinGuild( IN int iGuildUID, IN UidType iUnitUID );
	bool GetMyApplyJoinGuildUID( IN UidType iUnitUID, OUT int& iGuildUID );

	//////////////////////////////////////////////////////////////////////////	
	// 길드 광고
	void NewGuildAd( IN const KGuildAdInfo& kInfo );
	void UnexpiredGuildAd( IN const KGuildAdInfo& kInfo );
	void GetGuildAdList( IN const KELG_GET_GUILD_AD_LIST_REQ& kReq, OUT KEGS_GET_GUILD_AD_LIST_ACK& kAck );
	bool GetGuildAdEndDate( IN int iGuildUID, OUT std::wstring& wstrEndDate );
	void DeleteGuildAd( IN int iGuildUID );

	//////////////////////////////////////////////////////////////////////////
	// 가입 신청
	bool ApplyJoinGuild( IN const KELG_APPLY_JOIN_GUILD_REQ& kInfo, OUT std::vector< KApplyDeleteInfo >& vecDeleteApply ); // 길드 가입 신청	
	void RollbackApplyJoinGuild( IN const KDBE_APPLY_JOIN_GUILD_ACK& kInfo );
	void UpdateApplyJoinGuildInfo( IN const KDBE_APPLY_JOIN_GUILD_ACK& kInfo );

	bool GetApplyJoinGuildList( IN const KELG_GET_APPLY_JOIN_GUILD_LIST_REQ& kReq, OUT KEGS_GET_APPLY_JOIN_GUILD_LIST_ACK& kAck );	
	bool MakeGuildMemberInfoForAcceptJoinGuild( IN int iGuildUID, IN UidType iUnitUID, OUT KGuildMemberInfo& kInfo );
	bool DeleteApplyJoinGuild( IN UidType iUnitUID );
	bool DeleteApplyJoinGuildList( IN int iGuildUID, OUT std::vector< KApplyDeleteInfo >& vecApplyDeleteList );

	//////////////////////////////////////////////////////////////////////////
	// 길드 스킬
	bool GetGuildSkillInfo( IN int iGuildUID, OUT KGuildSkillInfo& kInfo );
	bool SetGuildSkillInfo( IN int iGuildUID, IN const KGuildSkillInfo& kInfo );
	bool DeleteGuildSkillInfo( IN int iGuildUID );

	//////////////////////////////////////////////////////////////////////////
	// 길드 정보 업데이트
	bool UpdateGuildAdInfo( IN const KGuildAdInfo& kInfo );
	void UpdateGuildEXP( IN int iGuildUID, IN int iGuildExp );
	void UpdateGuildLevelResult( IN int iGuildUID, IN u_char ucGuildLevel );
	void UpdateGuildMasterNickName( IN int iGuildUID, IN const std::wstring& wstrNickName );
	void UpdateGuildCurMemberCount( IN int iGuildUID, IN u_short usMemberCount );

	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
	void UpdateGuildName( IN const int& iGuildUID, IN const std::wstring& wstrGuildName );
#endif SERV_GUILD_CHANGE_NAME
	//}}

protected:
	template < class T > void SendToGameDB( unsigned short usEventID, const T& data );
	void SendToGameDB( unsigned short usEventID );

private:
	// 길드 광고 추가 삭제
	void InsertGuildAd( IN const KGuildAdInfo& kInfo );
	void ExpiredGuildAd( IN int iGuildUID );

	// 길드 광고 정렬
	void SortGuildAd();
	static bool RegDateSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond );
	static bool MemberCountSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond );
	static bool GuildLevelSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond );
	static bool NewGuildSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond );
	static bool HotGuildSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond );

	// 길드 가입 신청
	bool InsertApplyJoinGuild( IN const KApplyJoinGuildInfo& kInfo );
	
	void CheckGuildAdPeriod( bool bForce = false );			// 길드 광고 기간 체크	
	void CheckGuildAdDeleteTime( bool bForce = false );		// 길드 광고 삭제 체크
	void CheckGuildAdHotGuildSort();						// 길드 광고 정렬 체크

private:
	std::map< int, KGuildAdPtr >		m_mapGuildAd;
	std::vector< KGuildAdPtr >			m_vecGuildAd[AST_MAX];
	std::vector< KGuildAdPtr >			m_vecExpiredGuildAd;

	std::map< UidType, int >			m_mapApplyJoinGuildList;

	boost::timer						m_kTimer[TM_TOTAL_NUM];
};

DefSingletonInline( KGuildBoardManager );

template < class T >
void KGuildBoardManager::SendToGameDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_USER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_GAME_DB, 0, NULL, usEventID, data );
}


#endif SERV_GUILD_AD
#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <list>
#include "Enum/Enum.h"
#include "Match.h"
#include "MatchUserWaitList.h"

//{{ 2011. 06. 14	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM

//{{ 2012. 02. 21	김민성	공식 대전 대기 시간 표시 변경
#ifdef SERV_PVP_MATCH_WAIT_TIME
class KMatchCompleteTimeManager
{
public:
	enum COMPLETE_TIME_ENUM
	{
		CTE_MAX_COUNT = 3,
	};

	struct KMatchTime 
	{
		CTime		m_tMatchTime;
		int			m_iWatiTime;		// 매칭이 성공할때까지 대기한 시간(초단위)

		KMatchTime()
		{
			m_iWatiTime = 0;
		}
	};



	KMatchCompleteTimeManager();
	~KMatchCompleteTimeManager();

	unsigned int GetListSize() const { return m_listCompleteTime.size(); }
	void AddCompleteTime( IN const int iCompleteTime );
	int	GetMatchMakingEstimatedTime() const;

private:
	std::list< KMatchTime >		m_listCompleteTime;
};
#else
class KMatchCompleteTimeManager
{
public:
	enum COMPLETE_TIME_ENUM
	{
		CTE_MAX_COUNT = 5,
	};

	KMatchCompleteTimeManager();
	~KMatchCompleteTimeManager();

	unsigned int GetListSize() const { return m_listCompleteTime.size(); }
	void AddCompleteTime( IN const int iCompleteTime );
	int	GetMatchMakingEstimatedTime() const;

private:
	std::list< int >		m_listCompleteTime;
};
#endif SERV_PVP_MATCH_WAIT_TIME
//}}



class KMatchMakingManager : public KPerformer, public KThread
{
	DeclareSingleton( KMatchMakingManager );
	DeclDump;
	DeclToStringW;
	DeclPfID;

public:
    KMatchMakingManager();
    ~KMatchMakingManager();
	
	virtual void Run();     // derived from KThread
	virtual void ShutDown();

	bool	Init();
	void	ClearAllMatchInfo();
    void	Tick();

	// for lua
	void	Command_LUA( char cType );

	// function
	bool	IsExistInWaitListByUnitUID( IN const UidType iUnitUID )		{ return ( m_mapMatchWaitUser.find( iUnitUID ) != m_mapMatchWaitUser.end() ); }
	bool	IsExistInWaitListByPartyUID( IN const UidType iPartyUID )	{ return ( m_mapMatchWaitParty.find( iPartyUID ) != m_mapMatchWaitParty.end() ); } 

	bool	RegMatchWaitList( IN const SEnum::MATCH_TYPE eType, IN const std::vector< KMatchUserInfo >& vecMatchUserList, IN const bool bRegFrontOfList );
	bool	UnRegMatchWaitListByUnitUID( IN const UidType iUnitUID, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName = L"" );
	bool	UnRegMatchWaitListByWaitNumber( IN const int iWaitNumber, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName = L"" );
	bool	UnRegMatchWaitListByPartyUID( IN const UidType iPartyUID, IN const int iUnRegReason, IN const UidType iRequestCancelUnitUID );
	void	MatchMakingForCreatedMatch( IN const SEnum::MATCH_TYPE eType );
	void	MatchMakingForWaitUser( IN const SEnum::MATCH_TYPE eType );

protected:
	// WaitUserList
	KMatchUserPtr GetMatchWaitUserByUnitUID( IN const UidType iUnitUID );
	KMatchUserWaitListPtr GetMatchWaitListByWaitNumber( IN const int iWaitNumber );
	KMatchUserWaitListPtr GetMatchWaitListByPartyUID( IN const UidType iPartyUID );

	// Match
	KMatchPtr	GetMatch( IN const UidType iMatchUID );
	KMatchPtr	CreateMatch( IN const SEnum::MATCH_TYPE eMatchType, IN const std::vector< KMatchUserInfo >& vecMatchUserInfo );
	void		DestroyMatch( IN const UidType iMatchUID );

	// make object
	KMatchUserPtr	MakeUser( IN const SEnum::MATCH_TYPE eMatchType, IN const KMatchUserInfo& kInfo );
	KMatchPtr		MakeMatch( IN const SEnum::MATCH_TYPE eMatchType );

	//{{ 2012. 09. 15	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	bool		FindTheOpponent( IN const SEnum::MATCH_TYPE& eType,
								 IN const KMatchUserWaitListPtr& spFrontMatchWaitUser,
								 IN const std::list<KMatchUserWaitListPtr>& listMatchMakingWaitList,
								 OUT std::vector<int>& vecSelectedWaitUserList );

	bool		_CheckTheMemberNum( IN const SEnum::MATCH_TYPE& eType,
									IN const bool bIsAllMemberBeginner,
									IN std::list<KMatchUserWaitListPtr>& listMatchMakingWaitList,
									IN OUT int& iSumMatchPlayerCount,
									IN OUT int& iMinRating,
									IN OUT int& iMaxRating,
									IN OUT int& i2NumPartyCount,
									OUT std::vector<int>& vecSelectedWaitUserList );
#endif SERV_2012_PVP_SEASON2
	//}}

public:
	void	QueueingEventToMatch( IN const KEventPtr& spEvent );
	void	QueueingToAll( IN const KEventPtr& spEvent );

	//////////////////////////////////////////////////////////////////////////
	// process by gsuser event..
protected:
	virtual inline void ProcessEvent( const KEventPtr& spEvent );

	// 여기는 파티의 생성과 소멸에 관련된 이벤트만 있어야함..
	// 그외에 이벤트는 해당 파티에서 직접 처리한다.
	//{{ 2011. 06. 23	최육사	대전 개편
	DECL_ON_FUNC( EGB_REQUEST_MATCH_MAKING_REQ );
	DECL_ON_FUNC( EGB_CANCEL_MATCH_MAKING_REQ );	
   _DECL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT );
	DECL_ON_FUNC_NOPARAM( EGB_CHECK_INVALID_MATCH_NOT );
	DECL_ON_FUNC( EGB_MATCH_MAKING_ADMIN_COMMAND_NOT );

protected:
	template < class T > void SendToGSCharacter( IN UidType iGSUID, IN UidType iUnitUID, IN unsigned short usEventID, IN const T& data );
	template < class T > void BroadCastGameServer( IN unsigned short usEventID, IN const T& data );

protected:
	// 매치 리스트
	UidType										m_iLastMatchUID;
	std::map< UidType, KMatchPtr >				m_mapMatch;
	std::vector< KMatchPtr >					m_vecMatch[SEnum::MT_MAX];

	// 매치 대기자
	int											m_iLastWaitNumber;
	std::map< UidType, KMatchUserPtr >			m_mapMatchWaitUser;		// key : UnitUID
	std::map< int, KMatchUserWaitListPtr >		m_mapMatchWaitList;		// key : WaitNumber
	std::map< UidType, KMatchUserWaitListPtr >	m_mapMatchWaitParty;	// key : PartyUID
	std::list< KMatchUserWaitListPtr >			m_listMatchWaitList[SEnum::MT_MAX];
	static FSMclassPtr							ms_spUserFSM;

	// 매치 통계 정보
	KMatchCompleteTimeManager					m_kCompleteTimeManager[SEnum::MT_MAX];
    int											m_iRematchCount;
};

DefSingletonInline( KMatchMakingManager );


template < class T >
void KMatchMakingManager::SendToGSCharacter( IN UidType iGSUID, IN UidType iUnitUID, IN unsigned short usEventID, IN const T& data )
{
	UidType anTrace[2] = { iGSUID, -1 };
	KncSend( PI_GLOBAL_MATCH_MAKING, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, iUnitUID, anTrace, usEventID, data );
}

template < class T >
void KMatchMakingManager::BroadCastGameServer( IN unsigned short usEventID, IN const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_MATCH_MAKING, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, usEventID, data );
}


#endif SERV_PVP_NEW_SYSTEM
//}}



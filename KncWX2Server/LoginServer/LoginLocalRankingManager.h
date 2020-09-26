#pragma once
#include "ServerDefine.h"

//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
#include "ActorManager.h"
#include "BaseServer.h"
#include "Enum/Enum.h"

class KLoginLocalRankingManager : public KThread, public KPerformer
{
	DeclareSingleton( KLoginLocalRankingManager );
	DeclPfID;

	typedef	std::multimap< int, UidType, std::greater<int> >	RankMMAP;

public:
	KLoginLocalRankingManager();
	~KLoginLocalRankingManager(){}

	virtual	bool	Begin( void );
	void	ShutDown( void );

protected:
	// override from KPerformer	
	virtual	void	Tick( void );
	virtual	void	ProcessEvent( const KEventPtr& spEvent_ );

	// derived from KThread
	virtual	void	Run( void );

	DECL_ON_FUNC( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT );
	DECL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK );
	DECL_ON_FUNC( DBE_LOCAL_RANKING_RESET_ACK );
	_DECL_ON_FUNC( ELG_LOCAL_RANKING_SYSTEM_INFO_CHECK_NOT, int );
	_DECL_ON_FUNC( DBE_LOCAL_RANKING_RESET_CHECK_ACK, int );
	DECL_ON_FUNC_NOPARAM( ELG_LOCAL_RANKING_RESET_NOT );
	DECL_ON_FUNC( ELG_LOCAL_RANKING_SYSTEM_CHECK_NOT );

private:
	enum LOCAL_RANKING_MANAGER
	{
		LRM_NONE				= 0,
		LRM_CATEGORY_CHANGED	= 1,
		LRM_SPIRIT_CHANGED		= 2,
		LRM_AP_CHANGED			= 3,
		LRM_INSERT_DATA			= 4,
		LRM_DATA_CHANGED		= 5,
	};

	template < class T > void SendToGSUser( UidType nToGSUID, UidType nToGSUser, unsigned short usEventID, const T& data ) const;
	void	SendToGSUser( UidType nToGSUID, UidType nToGSUser, unsigned short usEventID ) const;

	template < class T > void SendToGameServer( UidType iGameServerUID, unsigned short usEventID, const T& data ) const;
	void	SendToGameServer( UidType iGameServerUID, unsigned short usEventID ) const;

	template < class T > void SendToAccountDB( unsigned short usEventID, const T& data ) const;
	void	SendToAccountDB( unsigned short usEventID ) const;

	template < class T > void SendToGameDB( unsigned short usEventID, const T& data ) const;
	void	SendToGameDB( unsigned short usEventID ) const;

	template < class T > void BroadCastAllGS( unsigned short usEventID, const T& data ) const;
	void	BroadCastAllGS( unsigned short usEventID ) const;

	bool	CheckInit( void ) const{	return m_bInit;	}
	bool	CheckUID( IN const int iUID ) const{	return m_iUID == iUID;	}

	void	clear( void );
	void	Init( IN const int iUID
		, IN const std::wstring& wstrResetTime
		, IN const std::map<UidType, KLocalRankingUnitInfo>& mapRankerUnitInfo
		, IN const std::map<UidType, UidType>& mapRankerUIDInfo
		, IN const std::map<UidType, KLocalRankingUserInfo>& mapRankerUserInfo
		);

	int		CheckChangedInfo( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo ) const;

	bool	SomethingChanged( IN const int iUID
		, IN const KLocalRankingUserInfo& kUserInfo
		, IN const KLocalRankingUnitInfo& kUnitInfo
		, OUT std::map<UidType, bool>& mapChangeInfo
		, OUT std::vector<KELG_LOCAL_RANKING_RANK_UP_NOT>& vecRankUpNot
		);

	void	RankingProcess( IN const int iChangeFlag
		, IN const KLocalRankingUserInfo& kPrevUserInfo
		, IN const KLocalRankingUnitInfo& kPrevUnitInfo
		, IN const KLocalRankingUserInfo& kUserInfo
		, IN const KLocalRankingUnitInfo& kUnitInfo
		, OUT std::map<UidType, bool>& mapChangeInfo
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

	bool	Remove( IN RankMMAP& mmapRanker
		, IN const KLocalRankingUserInfo& kUserInfo
		, IN const KLocalRankingUnitInfo& kUnitInfo
		);

	bool	Add( IN RankMMAP& mmapRanker
		, IN const int iAccumulatedPoint
		, IN const KLocalRankingUserInfo& kUserInfo
		, IN const KLocalRankingUnitInfo& kUnitInfo
		);

	void	SizeProcess( void );
	void	_SizeProcess( IN OUT RankMMAP& mmapRanker, IN OUT std::map<UidType, bool>& mapChangeInfo );

	int		GetRankAndNextUnitUID( IN const int iMainTabIndex, IN const int iCategory, IN const UidType iMyUnitUID, OUT UidType& iNextUnitUID ) const;
	
	bool	GetUserInfo( IN const UidType iUserUID, OUT KLocalRankingUserInfo& kInfo ) const;
	bool	GetUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUnitInfo& kInfo ) const;

	int		GetInitInfo( IN const int iNumOnce
		, IN OUT std::map<UidType, KLocalRankingUnitInfo>::const_iterator& itUnitEnd
		, IN const std::map<UidType, KLocalRankingUnitInfo>& inMapUnitInfo
		, OUT std::map<UidType, KLocalRankingUnitInfo>& outMapUnitInfo
		, IN OUT std::map<UidType, UidType>::const_iterator& itUIDEnd
		, IN const std::map<UidType, UidType>& inMapUIDInfo
		, OUT std::map<UidType, UidType>& outMapUIDInfo
		, IN OUT std::map<UidType, KLocalRankingUserInfo>::const_iterator& itUserEnd
		, IN const std::map<UidType, KLocalRankingUserInfo>& inMapUserInfo
		, OUT std::map<UidType, KLocalRankingUserInfo>& outMapUserInfo
		);

private:
	bool											m_bInit;
	int												m_iUID;
	boost::timer									m_kResetTimer;			// 갱신 시각 체크 텀
	CTime											m_tResetTime;			// 다음 갱신 예정 시각

	std::map< std::pair< int, int >, RankMMAP >		m_mapRankerTab;			// 랭크 비교를 위한 컨테이너
	std::map<UidType, KLocalRankingUserInfoCounter>	m_mapRankerUserInfo;	// 랭커들의 실제 유저 정보를 보관하는 컨테이너
	std::map<UidType, KLocalRankingUnitInfoCounter>	m_mapRankerUnitInfo;	// 랭커들의 실제 유닛 정보를 보관하는 컨테이너

	std::map< UidType, std::set< UidType > >		m_mapUserUnitList;		// 랭커의 UserUID에 해당하는 Unit 리스트

	// 백업 데이터
	std::map<UidType, KLocalRankingUnitInfo>		m_mapBackUpUnitInfo;
	std::map<UidType, UidType>						m_mapBackUpUIDInfo;
	std::map<UidType, KLocalRankingUserInfo>		m_mapBackUpUserInfo;
};
DefSingletonInline( KLoginLocalRankingManager );

template < class T >
void KLoginLocalRankingManager::SendToGSUser( UidType nToGSUID, UidType nToGSUser, unsigned short usEventID, const T& data ) const
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_USER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToGSUser ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
}

template < class T >
void KLoginLocalRankingManager::SendToGameServer( UidType iGameServerUID, unsigned short usEventID, const T& data ) const
{
	UidType anTrace[2] = { iGameServerUID, -1 };
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, iGameServerUID, anTrace, usEventID, data );
}

template < class T >
void KLoginLocalRankingManager::SendToAccountDB( unsigned short usEventID, const T& data ) const
{
	KncSend( KBaseServer::GetKObj()->GetPfID(), KBaseServer::GetKObj()->GetUID(), PI_ACCOUNT_DB, 0, NULL, usEventID, data );
}

template < class T >
void KLoginLocalRankingManager::SendToGameDB( unsigned short usEventID, const T& data ) const
{
	KncSend( KBaseServer::GetKObj()->GetPfID(), KBaseServer::GetKObj()->GetUID(), PI_LOGIN_GAME_DB, 0, NULL, usEventID, data );
}

template < class T >
void KLoginLocalRankingManager::BroadCastAllGS( unsigned short usEventID, const T& data ) const
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, usEventID, data );
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

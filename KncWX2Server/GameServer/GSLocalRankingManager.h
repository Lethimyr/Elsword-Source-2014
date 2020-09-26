#pragma once
#include "ServerDefine.h"

//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
#include "ActorManager.h"
#include "BaseServer.h"
#include "Enum/Enum.h"

class KGSLocalRankingManager : public KThread, public KPerformer
{
	DeclareSingleton( KGSLocalRankingManager );
	DeclPfID;

	typedef	std::multimap< int, UidType, std::greater<int> >	RankMMAP;

public:
	KGSLocalRankingManager();
	~KGSLocalRankingManager(){}

	virtual	bool	Begin( void );
	void	ShutDown( void );

protected:
	// override from KPerformer	
	virtual	void	Tick( void );
	virtual	void	ProcessEvent( const KEventPtr& spEvent_ );

	// derived from KThread
	virtual	void	Run( void );

	_DECL_ON_FUNC( EGS_LOCAL_RANKING_INQUIRY_REQ, KLocalRankingInquiryData );
	DECL_ON_FUNC( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_CHECK_NOT );
	DECL_ON_FUNC( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT );
	DECL_ON_FUNC( ELG_LOCAL_RANKING_RESET_NOT );
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

	template < class T > void SendToLoginServer( unsigned short usEventID, const T& data ) const;
	void	SendToLoginServer( unsigned short usEventID ) const;

	template < class T > void BroadCastAllGSUser( unsigned short usEventID, const T& data ) const;
	void	BroadCastAllGSUser( unsigned short usEventID ) const;

	template < class T > void SendToGSUser( UidType nTo_, unsigned short usEventID, const T& data ) const;
	void	SendToGSUser( UidType nTo_, unsigned short usEventID ) const;

	bool	CheckInit( void ) const{	return m_bInit;	}
	bool	CheckUID( IN const int iUID ) const{	return m_iUID == iUID;	}

	void	clear( void );
	void	Init( IN const int iOption
		, IN const int iUID
		, IN const std::map<UidType, KLocalRankingUnitInfo>& mapRankerUnitInfo
		, IN const std::map<UidType, UidType>& mapRankerUIDInfo
		, IN const std::map<UidType, KLocalRankingUserInfo>& mapRankerUserInfo
		);

	int		CheckChangedInfo( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo ) const;

	bool	SomethingChanged( IN const int iUID
		, IN const KLocalRankingUserInfo& kUserInfo
		, IN const KLocalRankingUnitInfo& kUnitInfo
		);

	void	RankingProcess( IN const int iChangeFlag
						  , IN const KLocalRankingUserInfo& kPrevUserInfo
						  , IN const KLocalRankingUnitInfo& kPrevUnitInfo
						  , IN const KLocalRankingUserInfo& kUserInfo
						  , IN const KLocalRankingUnitInfo& kUnitInfo
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

	void	SizeProcess( void );
	void	_SizeProcess( IN OUT RankMMAP& mmapRanker );

	bool	IsRankingUser( IN const UidType iUserUID ) const{	return m_mapRankerUserInfo.find( iUserUID ) != m_mapRankerUserInfo.end();	}
	bool	RankingCheck( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo ) const;
	bool	_RankingCheck( IN const int iMainTabIndex
		, IN int iCategory
		, IN const int iAccumulatedPoint
		) const;

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
	bool	GetUserInfo( IN const UidType iUserUID, OUT KLocalRankingUserInfo& kInfo ) const;
	bool	GetUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUnitInfo& kInfo ) const;

private:
	bool											m_bInit;
	int												m_iUID;
	boost::timer									m_kResetTimer;			// 갱신 시각 체크 텀

	std::map< std::pair< int, int >, RankMMAP >		m_mapRankerTab;			// 랭크 비교를 위한 컨테이너
	std::map<UidType, KLocalRankingUserInfoCounter>	m_mapRankerUserInfo;	// 랭커들의 실제 유저 정보를 보관하는 컨테이너
	std::map<UidType, KLocalRankingUnitInfoCounter>	m_mapRankerUnitInfo;	// 랭커들의 실제 유닛 정보를 보관하는 컨테이너

	std::map< UidType, std::set< UidType > >		m_mapUserUnitList;		// 랭커의 UserUID에 해당하는 Unit 리스트

	// 백업 데이터
	std::map<UidType, KLocalRankingUnitInfo>		m_mapBackUpUnitInfo;
	std::map<UidType, UidType>						m_mapBackUpUIDInfo;
	std::map<UidType, KLocalRankingUserInfo>		m_mapBackUpUserInfo;
};

DefSingletonInline( KGSLocalRankingManager );

template < class T >
void KGSLocalRankingManager::SendToLoginServer( unsigned short usEventID, const T& data ) const
{
	UidType anTrace[2] = { KBaseServer::GetKObj()->GetUID(), -1 };
	KncSend( KBaseServer::GetKObj()->GetPfID(), KBaseServer::GetKObj()->GetUID(), PI_LOGIN_SERVER, 0, anTrace, usEventID, data );
}

template < class T >
void KGSLocalRankingManager::BroadCastAllGSUser( unsigned short usEventID, const T& data ) const
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, usEventID, data );
}

template < class T >
void KGSLocalRankingManager::SendToGSUser( UidType nTo_, unsigned short usEventID, const T& data ) const
{
	UidType anTrace[2] = { KBaseServer::GetKObj()->GetUID(), -1 };
	KncSend( KBaseServer::GetKObj()->GetPfID(), KBaseServer::GetKObj()->GetUID(), PI_GS_USER, nTo_, anTrace, usEventID, data );
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}
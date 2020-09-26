#pragma once
#include "Event.h"
#include "CommonPacket.h"
#include "CacheData.h"
#include "X2Data\XSLUnit.h"


//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM


class KUserPvpManager
{
public:

	//{{ 2012. 09. 28	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
	enum KUserPvpManagerValue
	{
		KPMV_AP_MIN			= 0,
		KPMV_KFACTOR_MIN	= 0,
		KPMV_KFACTOR_MAX	= 500,
		KPMV_RATING_MIN		= 0,
		KPMV_RATING_MAX		= 3300,
	};
#endif SERV_2012_PVP_SEASON2_1
	//}}

	KUserPvpManager(void);
	~KUserPvpManager(void);

	void	Clear();
	void	Init( IN const int iOfficialMatchCnt, 
				  IN const int iRating, 
				  IN const int iMaxRating, 
				  IN const int iRPoint, 
				  IN const int iAPoint,
				  IN const bool bIsWinBeforeMatch,
				  IN const int iWin, 
				  IN const int iLose
				  //{{ 2012. 06. 20	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
				  , IN const char cRank
				  , IN const float fKFactor
				  , IN const bool bIsRedistributionUser
				  , IN const int iPastSeasonWin
				  , IN const UidType iUnitUID
#endif SERV_2012_PVP_SEASON2
				  //}}
				  );

	UidType	GetMatchUID() const							{ return m_iMatchUID; }
	void	SetMatchUID( IN const UidType iMatchUID )	{ m_iMatchUID = iMatchUID; }

	int		GetOfficialMatchCount() const				{ return m_iOfficialMatchCnt; }
	void	IncreaseOfficialMatchCount()				{ m_iOfficialMatchCnt += 1; }

	int		GetRating() const							{ return m_iRating; }
	//{{ 2012. 06. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	void	AdjustInitValue( IN const UidType iUnitUID );

	bool	AddRatingAndRankUpDownProcess( IN const int iRating );
	float	GetKFactor( void ) const					{ return m_fKFactor; }
	void	AddKFactor( IN const float& fKFactor );
	bool	IsRedistributionUser( void ) const			{ return m_bIsRedistributionUser; }
	int		GetPastSeasonWin( void ) const				{ return m_iPastSeasonWin; }
#else
	void	AddRating( IN const int iRating );
#endif SERV_2012_PVP_SEASON2
	//}}

	int		GetMaxRating() const						{ return m_iMaxRating; }

	int		GetRPoint() const							{ return m_iRPoint; }
	void	AddRPoint( IN const int iRPoint );

	int		GetAPoint() const							{ return m_iAPoint; }
	void	AddAPoint( IN const int iAPoint );

	bool	IsWinBeforeMatch() const					{ return m_bIsWinBeforeMatch; }

	CXSLUnit::PVP_RANK		GetPvpRank() const			{ return m_ePvpRank; }

	int		GetWin() const								{ return m_iWin; }
	int		GetLose() const								{ return m_iLose; }
	bool	UpdateWinLose( IN const int iWin, IN const int iLose, IN const bool bIsOfficialMatch );

	// 대전 결과 업데이트
	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	void	UpdatePvpResult( IN const KERM_UPDATE_PVP_UNIT_INFO_NOT kData );
#else
	void	UpdatePvpResult( IN const int iRating, IN const int iRPoint, IN const int iAPoint, IN const int iWin, IN const int iLose, IN const bool bIsOfficialMatch, IN const bool bOutRoom );
#endif SERV_2012_PVP_SEASON2
	//}}

	// DB업데이트 준비
	void	GetDBUpdateInfo( OUT int& iOfficialMatchCnt,
							 OUT int& iRating, 
							 OUT int& iMaxRating, 
							 OUT int& iRPoint, 
							 OUT int& iAPoint, 
							 OUT bool& bIsWinBeforeMatch, 
							 OUT int& iWin, 
							 OUT int& iLose,
							 OUT char& cEmblemEnum

							 //{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
							 , OUT float& fKFactor
#endif SERV_2012_PVP_SEASON2
							 //}}
							 );

	// DB업데이트 롤백
	void	CheckRollbackInfo( IN const int iOfficialMatchCnt,
								IN const int iRating,
								IN const int iMaxRating,
								IN const int iRPoint,
								IN const int iAPoint,
								IN const int iWin, 
								IN const int iLose

								//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
								, IN const float& fKFactor
#endif SERV_2012_PVP_SEASON2
								//}}
								);

	void	SetFlagRequestMatchMaking( IN const bool bVal )	{ m_bRequestMatchMaking = bVal; }
	bool	IsRequestMatchMaking() const				{ return m_bRequestMatchMaking; }

private:
	UidType					m_iMatchUID;				// MatchUID
	KCacheData< int >		m_iOfficialMatchCnt;		// Official Match Count
	KCacheData< int >		m_iRating;					// Rating
	KCacheData< int >		m_iMaxRating;				// MaxRating
	KCacheData< int >		m_iRPoint;					// Ranking Point
	KCacheData< int >		m_iAPoint;					// Arena Point

	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	KCacheData< float >		m_fKFactor;
	bool					m_bIsRedistributionUser;
	int						m_iPastSeasonWin;
#endif SERV_2012_PVP_SEASON2
	//}}

	CXSLUnit::PVP_RANK		m_ePvpRank;
	bool					m_bIsWinBeforeMatch;
	KCacheData< int >		m_iWin;
	KCacheData< int >		m_iLose;
	bool					m_bRequestMatchMaking;
};


#endif SERV_PVP_NEW_SYSTEM
//}}
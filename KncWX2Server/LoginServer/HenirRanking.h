#pragma once

#include "Event.h"
#include "CommonPacket.h"


class KHenirRanking
{
public:
	KHenirRanking(void);
	virtual ~KHenirRanking(void);

	u_int	GetLastRank()												{ return m_uiLastRank; }
	void	SetLastRank( u_int uiLastRank )								{ m_uiLastRank = uiLastRank; }
	void	SetRefreshTime( const int iRankingType, const int iWeekOfMonth, const int iDayOfWeek, const int iHour );
	CTime	GetRefreshTime() const										{ return m_tRefreshTime; }
	void	NextRefreshTimeSet();

	void	SetRewardBeginTime( const std::wstring& wstrBeginTime );
	CTime	GetRewardBeginTime()										{ return m_tRewardBeginTime; }
	
	bool	IsRankingChanged() const									{ return m_bIsRankingChange; }		// 랭킹 변동 확인
	void	SetRankingChanged( bool bVal )								{ m_bIsRankingChange = bVal; }
	
	bool	CheckNewRecord( IN const KHenirRankingInfo& kNewRankingInfo );									// 랭킹 등록 시도
	bool	DeleteRecord( IN const std::wstring& wstrNickName );
	bool	DeleteRecord( IN const UidType iUnitUID );

	const std::vector< KHenirRankingInfo >& GetRankingInfo() const		{ return m_vecRankingInfo; }
	void	SetRankingInfo( const std::vector< KHenirRankingInfo >& vecRankingInfo );
	void	ClearRanking();

private:	
#ifdef SERV_HENIR_RANKING_GROUP_FIX
	bool	RankingSort( const UidType& iUnitUID );
#endif SERV_HENIR_RANKING_GROUP_FIX
	void	RankingSort();
	static CTime GetDayOfWeekInMonth( int iYear, int iMonth, int iWeekOfMonth, int iDayOfWeek, int iHour );

private:
	std::vector< KHenirRankingInfo >	m_vecRankingInfo;		// 랭킹 리스트
	u_int								m_uiLastRank;			// 랭킹 리스트의 마지막 랭크
	bool								m_bIsRankingChange;		// 랭킹 변동 여부
	
	int									m_iRankingType;			// 랭킹 타입
	CTime								m_tRefreshTime;			// 랭킹 정보 초기화 시각
	int									m_iRefreshWeekOfMonth;
	int									m_iRefreshDayOfWeek;
	int									m_iRefreshHour;

	//{{ 2009. 8. 3  최육사		랭킹 보상 시작시각
	CTime								m_tRewardBeginTime;		// 보상 시작 시간
	//}}
};
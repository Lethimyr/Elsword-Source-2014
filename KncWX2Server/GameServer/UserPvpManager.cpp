#include ".\userpvpmanager.h"
//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch\PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}


//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM


KUserPvpManager::KUserPvpManager(void)
{
	Clear();
}

KUserPvpManager::~KUserPvpManager(void)
{
}

void KUserPvpManager::Clear()
{
	m_iMatchUID = 0;
	m_iOfficialMatchCnt.SetValue( 0 );
	m_iRating.SetValue( 0 );
	m_iMaxRating.SetValue( 0 );
	m_iRPoint.SetValue( 0 );
	m_iAPoint.SetValue( 0 );

	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	m_fKFactor.SetValue( 0.0 );
	m_bIsRedistributionUser	= false;
	m_iPastSeasonWin		= 0;
#endif SERV_2012_PVP_SEASON2
	//}}

	m_bIsWinBeforeMatch = false;
	m_iWin.SetValue( 0 );
	m_iLose.SetValue( 0 );
	m_ePvpRank = CXSLUnit::PVPRANK_NONE;
	m_bRequestMatchMaking = false;
}

void KUserPvpManager::Init( IN const int iOfficialMatchCnt, 
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
							//{{ 2012. 06. 25	박세훈	2012 대전 시즌2
							, IN const float fKFactor
							, IN const bool bIsRedistributionUser
							, IN const int iPastSeasonWin
							, IN const UidType iUnitUID
#endif SERV_2012_PVP_SEASON2
							//}}
							)
{
	m_iOfficialMatchCnt.SetValue( iOfficialMatchCnt );
	m_iRating.SetValue( iRating );
	m_iMaxRating.SetValue( iMaxRating );
	m_iRPoint.SetValue( iRPoint );
	m_iAPoint.SetValue( iAPoint );
	m_bIsWinBeforeMatch = bIsWinBeforeMatch;
	m_iWin.SetValue( iWin );
	m_iLose.SetValue( iLose );

	// 랭크 구하기
	//{{ 2012. 06. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	m_ePvpRank				= static_cast<CXSLUnit::PVP_RANK>( cRank );
	m_fKFactor.SetValue( fKFactor );
	m_bIsRedistributionUser	= bIsRedistributionUser;
	m_iPastSeasonWin		= iPastSeasonWin;

	AdjustInitValue( iUnitUID );
#else
	const CXSLUnit::PVP_EMBLEM eEmblem = CXSLUnit::GetPVPEmblem( GetRating() );
	m_ePvpRank = CXSLUnit::PvpEmblemToPvpRank( eEmblem );
#endif SERV_2012_PVP_SEASON2
	//}}
}

void KUserPvpManager::GetDBUpdateInfo( OUT int& iOfficialMatchCnt,
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
										)
{
	iOfficialMatchCnt = m_iOfficialMatchCnt.GetChangeValue();
	m_iOfficialMatchCnt.SetValue( m_iOfficialMatchCnt );

	iRating		= m_iRating.GetChangeValue();
	m_iRating.SetValue( m_iRating );

	iMaxRating	= m_iMaxRating.GetChangeValue();
	m_iMaxRating.SetValue( m_iMaxRating );

	iRPoint		= m_iRPoint.GetChangeValue();
	m_iRPoint.SetValue( m_iRPoint );

	iAPoint		= m_iAPoint.GetChangeValue();
	m_iAPoint.SetValue( m_iAPoint );

    bIsWinBeforeMatch = m_bIsWinBeforeMatch;

	iWin		= m_iWin.GetChangeValue();
	m_iWin.SetValue( m_iWin );

	iLose		= m_iLose.GetChangeValue();
	m_iLose.SetValue( m_iLose );

	cEmblemEnum = static_cast<CXSLUnit::PVP_RANK>(GetPvpRank());

	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	fKFactor	= m_fKFactor.GetChangeValue();
	m_fKFactor.SetValue( m_fKFactor );
#endif SERV_2012_PVP_SEASON2
	//}}
}

void KUserPvpManager::CheckRollbackInfo( IN const int iOfficialMatchCnt,
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
										)
{
	m_iOfficialMatchCnt.AdjustInitValue( m_iOfficialMatchCnt.GetInitValue() - iOfficialMatchCnt );
	m_iRating.AdjustInitValue( m_iRating.GetInitValue() - iRating );
	m_iMaxRating.AdjustInitValue( m_iMaxRating.GetInitValue() - iMaxRating );
	m_iRPoint.AdjustInitValue( m_iRPoint.GetInitValue() - iRPoint );
	m_iAPoint.AdjustInitValue( m_iAPoint.GetInitValue() - iAPoint );
	m_iWin.AdjustInitValue( m_iWin.GetInitValue() - iWin );
	m_iLose.AdjustInitValue( m_iLose.GetInitValue() - iLose );

	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	m_fKFactor.AdjustInitValue( m_fKFactor.GetInitValue() - fKFactor );
#endif SERV_2012_PVP_SEASON2
	//}}
}

void KUserPvpManager::AdjustInitValue( IN const UidType iUnitUID )
{
	if( ( m_ePvpRank < CXSLUnit::PVPRANK_RANK_E ) || ( CXSLUnit::PVPRANK_RANK_SSS < m_ePvpRank ) )
	{
		START_LOG( cerr, L"있을 수 없는 랭크 정보가 들어왔습니다!! DB 데이터 점검 및 코드 오류 체크가 필요합니다!!")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_ePvpRank )
			<< END_LOG;
	}

	if( m_iAPoint < KPMV_AP_MIN )
	{
		START_LOG( cerr, L"있을 수 없는 AP 정보가 들어왔습니다!! DB 데이터 점검 및 코드 오류 체크가 필요합니다!!")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_iAPoint )
			<< END_LOG;
	}

	if( m_fKFactor < KPMV_KFACTOR_MIN )
	{
		START_LOG( cerr, L"KFactor가 최소 수치보다 작습니다. 최소 수치로 보정합니다.")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_fKFactor )
			<< BUILD_LOG( KPMV_KFACTOR_MIN )
			<< END_LOG;

		m_fKFactor.SetValue( KPMV_KFACTOR_MIN );
	}
	else if( KPMV_KFACTOR_MAX < m_fKFactor )
	{
		START_LOG( cerr, L"KFactor가 최대 수치보다 큽니다. 최대 수치로 보정합니다.")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_fKFactor )
			<< BUILD_LOG( KPMV_KFACTOR_MAX )
			<< END_LOG;

		m_fKFactor.SetValue( KPMV_KFACTOR_MAX );
	}

	if( m_iRating < KPMV_RATING_MIN )
	{
		START_LOG( cerr, L"Rating이 최소 수치보다 작습니다. 최소 수치로 보정합니다.")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_iRating )
			<< BUILD_LOG( KPMV_RATING_MIN )
			<< END_LOG;

		m_iRating.SetValue( KPMV_RATING_MIN );
	}
	else if( KPMV_RATING_MAX < m_iRating )
	{
		START_LOG( cerr, L"Rating이 최대 수치보다 큽니다. 최대 수치로 보정합니다.")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_iRating )
			<< BUILD_LOG( KPMV_RATING_MAX )
			<< END_LOG;

		m_iRating.SetValue( KPMV_RATING_MAX );
	}
}

//{{ 2012. 06. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2	// Rating 포인트 제한선 3300
bool KUserPvpManager::AddRatingAndRankUpDownProcess( IN const int iRating )
#else
void KUserPvpManager::AddRating( IN const int iRating )
#endif SERV_2012_PVP_SEASON2
//}}
{
	int iAddRating = iRating;

	// 최대 Rating값을 넘지 않는지 검사
	if( ( GetRating() + iAddRating ) > CXSLUnit::PE_END )
	{
        iAddRating = ( CXSLUnit::PE_END - GetRating() );
	}

	// 최소 Rating값을 넘지 않는지 검사
	if( ( GetRating() + iAddRating ) < 0 )
	{
		iAddRating = -( GetRating() );
	}

	// Rating값 증가!
	m_iRating += iAddRating;

	// 최대 Rating값 체크!
	if( GetMaxRating() < GetRating() )
	{
		m_iMaxRating += ( GetRating() - GetMaxRating() );
	}

	// 랭크 갱신
	if( iAddRating != 0 )
	{
		//{{ 2012. 06. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2	// 랭크 업/다운 시 랭킹 포인트 수정
		const CXSLUnit::PVP_RANK eNowPvpRank = CXSLUnit::ComputeNewPVPRank( GetPvpRank(), GetRating() );
		if( eNowPvpRank == CXSLUnit::PE_NONE )
		{
			return false;
		}

		if( m_ePvpRank < eNowPvpRank )
		{
			// 랭크 업
			//m_iRPoint = 0;
			m_iRPoint -= m_iRPoint;
		}
		else if( eNowPvpRank < m_ePvpRank )
		{
			// 랭크 다운
			switch( eNowPvpRank )
			{
			case CXSLUnit::PVPRANK_RANK_E:	m_iRPoint -= m_iRPoint;	m_iRPoint += 67;	break;
			case CXSLUnit::PVPRANK_RANK_D:	m_iRPoint -= m_iRPoint;	m_iRPoint += 111;	break;
			case CXSLUnit::PVPRANK_RANK_C:	m_iRPoint -= m_iRPoint;	m_iRPoint += 156;	break;
			case CXSLUnit::PVPRANK_RANK_B:	m_iRPoint -= m_iRPoint;	m_iRPoint += 156;	break;
			case CXSLUnit::PVPRANK_RANK_A:	m_iRPoint -= m_iRPoint;	m_iRPoint += 200;	break;
			case CXSLUnit::PVPRANK_RANK_S:	m_iRPoint -= m_iRPoint;	m_iRPoint += 200;	break;
			case CXSLUnit::PVPRANK_RANK_SS:	m_iRPoint -= m_iRPoint;	m_iRPoint += 289;	break;
			}
		}

		if( m_ePvpRank != eNowPvpRank )
		{
			m_ePvpRank = eNowPvpRank;
			return true;
		}
	}
	return false;
#else
		const CXSLUnit::PVP_EMBLEM eEmblem = CXSLUnit::GetPVPEmblem( GetRating() );
		m_ePvpRank = CXSLUnit::PvpEmblemToPvpRank( eEmblem );
	}
#endif SERV_2012_PVP_SEASON2
		//}}
}

void KUserPvpManager::AddRPoint( IN const int iRPoint )
{
	int iAddRPoint = iRPoint;

	// 최대 RankingPoint값을 넘지 않는지 검사
	if( ( GetRPoint() + iAddRPoint ) > SiKPvpMatchManager()->GetMaxRankingPoint() )
	{
		iAddRPoint = ( SiKPvpMatchManager()->GetMaxRankingPoint() - GetRPoint() );
	}

	// 최소 RankingPoint값을 넘지 않는지 검사
	if( ( GetRPoint() + iAddRPoint ) < 0 )
	{
		iAddRPoint = -( GetRPoint() );
	}

	// Ranking Point값 증가!
	m_iRPoint += iAddRPoint;
}

void KUserPvpManager::AddAPoint( IN const int iAPoint )
{
	int iAddAPoint = iAPoint;

	// 최대 ArenaPoint값을 넘지 않는지 검사
	if( ( GetAPoint() + iAddAPoint ) > SiKPvpMatchManager()->GetMaxArenaPoint() )
	{
		iAddAPoint = ( SiKPvpMatchManager()->GetMaxArenaPoint() - GetAPoint() );
	}

	// 최소 ArenaPoint값을 넘지 않는지 검사
	if( ( GetAPoint() + iAddAPoint ) < 0 )
	{
		iAddAPoint = -( GetAPoint() );
	}

	// Rating값 증가!
	m_iAPoint += iAddAPoint;
}

bool KUserPvpManager::UpdateWinLose( IN const int iWin, IN const int iLose, IN const bool bIsOfficialMatch )
{
	if( iWin > 1  &&  iLose > 1 )
	{
		START_LOG( cerr, L"승리결과 값이 동시에 2개가 셋팅되어 왔다.!" )
			<< BUILD_LOG( iWin )
			<< BUILD_LOG( iLose )
			<< END_LOG;
		return false;
	}
	if( iWin > 1 )
	{
		START_LOG( cerr, L"WIN 포인트 이상.!" )
			<< BUILD_LOG( iWin )
			<< END_LOG;
		//iWin = 1;
		return false;
	}
	if( iLose > 1 )
	{
		START_LOG( cerr, L"LOSE 포인트 이상.!" )
			<< BUILD_LOG( iLose )
			<< END_LOG;

		//iLose = 1;
		return false;
	}

	m_iWin	+= iWin;
	m_iLose	+= iLose;

	// 공식 대전인 경우
	if( bIsOfficialMatch )
	{
        // 승리했다면!
		if( iWin == 1 )
		{
			m_bIsWinBeforeMatch = true;
		}
		else if( iLose == 1 )
		{
			m_bIsWinBeforeMatch = false;
		}
	}
	return true;
}

//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
void KUserPvpManager::AddKFactor( IN const float& fKFactor )
{
	float fAddKFactor = fKFactor;

	// 최소 K-Factor값을 넘지 않는지 검사
	if( ( GetKFactor() + fAddKFactor ) < 0.f )
	{
		fAddKFactor = -( GetKFactor() );

		START_LOG( cerr, L"현재 KFactor와 증분값을 합했을때 0보다 작다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetKFactor() )
			<< BUILD_LOG( fAddKFactor )
			<< END_LOG;
	}

	if( ( GetKFactor() + fAddKFactor ) > 500.f )
	{
		fAddKFactor = ( 500.f - GetKFactor() );

		START_LOG( cerr, L"현재 KFactor와 증분값을 합했을때 500보다 크다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetKFactor() )
			<< BUILD_LOG( fAddKFactor )
			<< END_LOG;
	}

	// K-Factor값 증가!
	m_fKFactor += fAddKFactor;
}

void KUserPvpManager::UpdatePvpResult( IN const KERM_UPDATE_PVP_UNIT_INFO_NOT kData )
{
	/* Rating업데이트
		- AddRating의 결과 랭크 업/다운이 발생할 경우,
		강제로 RankPoint값을 지정해준다. ( AddRatingAndRankUpDownProcess 내부에서. )
		그러므로 이후 AddRPoint는 적용되어서는 안된다.
	*/
	if( AddRatingAndRankUpDownProcess( kData.m_iRating ) == false )
	{
		// RPoint업데이트
		AddRPoint( kData.m_iRPoint );
	}

	// APoint업데이트
	AddAPoint( kData.m_iAPoint );

	// K-Factor 업데이트
	AddKFactor( kData.m_fKFactorIncrement );

	// 승패 업데이트
	if( UpdateWinLose( kData.m_iWin, kData.m_iLose, kData.m_bIsOfficialMatch ) == false )
	{
		START_LOG( cerr, L"승패 정보 업데이트 실패! 승패 값이 이상합니다!" )
			<< BUILD_LOG( kData.m_iWin )
			<< BUILD_LOG( kData.m_iLose )
			<< END_LOG;
	}

	// 공식 대전 플레이 카운트
	if( kData.m_bIsOfficialMatch == true  &&  kData.m_bOutRoom == false )
	{
		// 공식 대전 플레이 카운트 증가!
		IncreaseOfficialMatchCount();
	}

	//////////////////////////////////////////////////////////////////////////
	if( kData.m_iWin > 0  &&  kData.m_iAPoint < 0 )
	{
        START_LOG( cout, L"[대전결과확인] 이겼는데 Arena Point값이 마이너스로 들어온다! 문제가 있다!" )
			<< BUILD_LOG( kData.m_iWin )
			<< BUILD_LOG( kData.m_iAPoint );
	}
	//////////////////////////////////////////////////////////////////////////	
}
#else
void KUserPvpManager::UpdatePvpResult( IN const int iRating, IN const int iRPoint, IN const int iAPoint, IN const int iWin, IN const int iLose, IN const bool bIsOfficialMatch, IN const bool bOutRoom )
{
	// Rating업데이트
	AddRating( iRating );

	// RPoint업데이트
	AddRPoint( iRPoint );

	// APoint업데이트
	AddAPoint( iAPoint );

	// 승패 업데이트
	if( UpdateWinLose( iWin, iLose, bIsOfficialMatch ) == false )
	{
		START_LOG( cerr, L"승패 정보 업데이트 실패! 승패 값이 이상합니다!" )
			<< BUILD_LOG( iWin )
			<< BUILD_LOG( iLose )
			<< END_LOG;
	}

	// 공식 대전 플레이 카운트
	if( bIsOfficialMatch == true  &&  bOutRoom == false )
	{
		// 공식 대전 플레이 카운트 증가!
		IncreaseOfficialMatchCount();
	}

	//////////////////////////////////////////////////////////////////////////
	if( iWin > 0  &&  iAPoint < 0 )
	{
        START_LOG( cout, L"[대전결과확인] 이겼는데 Arena Point값이 마이너스로 들어온다! 문제가 있다!" )
			<< BUILD_LOG( iWin )
			<< BUILD_LOG( iAPoint );
	}
	//////////////////////////////////////////////////////////////////////////	
}
#endif SERV_2012_PVP_SEASON2
//}}

#endif SERV_PVP_NEW_SYSTEM
//}}



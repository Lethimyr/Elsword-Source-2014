#include "dxstdafx.h"
#include "Lottery.h"
#include "dbg/dbg.hpp"

KLottery::KLottery() : m_fTotalProb( 0.0f )
{
}

KLottery::~KLottery() 
{
}

KLottery& KLottery::operator=( const KLottery& t )
{
    m_mapCase       = t.m_mapCase;
    m_fTotalProb    = t.m_fTotalProb;
    return *this;
}

bool KLottery::AddCase(  int nCaseID, float fProbability, int nParam /*= PARAM_BLANK*/ )
{
    if( m_fTotalProb + fProbability > 100.F )
    {
        START_LOG( cerr, L"100퍼센트를 넘는 확률 설정을 시도." )
            << BUILD_LOG( nCaseID )
            << BUILD_LOG( fProbability )
            << BUILD_LOG( m_fTotalProb )
            << BUILD_LOG( m_mapCase.size() );

        return false;
    }

    std::map<int,KCaseUnit>::iterator mit;
    mit = m_mapCase.find( nCaseID );

    if( mit != m_mapCase.end() )    // 이전에 동일한 case의 확률이 존재하고 있던 경우
    {
        mit->second.m_fProb     += fProbability;
        mit->second.m_nParam    = nParam;

        m_fTotalProb += fProbability;
    }
    else
    {
        KCaseUnit kUnit = { fProbability, nParam };
        m_mapCase.insert( std::make_pair( nCaseID, kUnit ) );
        m_fTotalProb += fProbability;
    }

    return true;
}

#include <boost/random.hpp>

int KLottery::Decision() const
{
    static boost::minstd_rand generator(42u);
    static boost::uniform_real<> uni_dist(0,100);
    static boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > uni(generator, uni_dist);

    float fRoulette = (float)uni();

    float fAccumulate = 0.0f;
    std::map<int,KCaseUnit>::const_iterator mit;

    for( mit = m_mapCase.begin(); mit != m_mapCase.end(); mit++ )
    {
        fAccumulate += mit->second.m_fProb;
        if( fRoulette <= fAccumulate ) return mit->first;
    }

    return CASE_BLANK;
}

#include <boost/format.hpp>

ImplToStringW( KLottery )
{
    float fAccumulate = 0;
    std::map<int,KCaseUnit>::const_iterator mit;
    for( mit = m_mapCase.begin(); mit != m_mapCase.end(); mit++ )
    {
        stm_ << boost::wformat( L"%2.2f ~ %2.2f (%2.2f%%) : %d (param:%d) \n" )
            % fAccumulate
            % (float)(fAccumulate + mit->second.m_fProb)
            % (float)(mit->second.m_fProb)
            % mit->first
            % mit->second.m_nParam;

        fAccumulate += mit->second.m_fProb;
    }

    stm_ << boost::wformat( L"%2.2f ~ 100 (%2.2f%%) : -1 \n" )
        % fAccumulate
        % (100.F - fAccumulate);

    return stm_ << std::endl;
}

int KLottery::GetParam( int nCaseID ) const
{
    std::map<int,KCaseUnit>::const_iterator mit;
    mit = m_mapCase.find( nCaseID );

    if( mit == m_mapCase.end() )
        return PARAM_BLANK;

    return mit->second.m_nParam;
}

bool KLottery::AddProbability( float fProbAdd )
{
    _JIF( fProbAdd > 0.0f, return false );

    if( fProbAdd * m_mapCase.size() + m_fTotalProb > 1.0f )
    {
        START_LOG( cerr, L"Probability overflow." )
            << BUILD_LOG( fProbAdd )
            << BUILD_LOG( m_mapCase.size() )
            << BUILD_LOG( m_fTotalProb )
            << BUILD_LOG( fProbAdd * m_mapCase.size() + m_fTotalProb );

        return false;
    }

    m_fTotalProb = 0.0f;    // 전체 확률합도 이참에 새로 계산하자.
    std::map<int,KCaseUnit>::iterator mit;
    for( mit = m_mapCase.begin(); mit != m_mapCase.end(); mit++ )
    {
        mit->second.m_fProb += fProbAdd;

        m_fTotalProb += mit->second.m_fProb;
    }

    LIF( m_fTotalProb <= 1.0f );

    return true;
}

//{{ 2007. 11. 8  최육사  설정된 모든 case의 확률에 똑같이 덧셈. 
void KLottery::AddProbabilityHundredMaxProb( float fProbAdd )
{
	_JIF( fProbAdd > 0.0f, return );

	if( fProbAdd * m_mapCase.size() + m_fTotalProb > 100.0f )
	{
		START_LOG( clog2, L"Probability overflow." )
			<< BUILD_LOG( fProbAdd )
			<< BUILD_LOG( m_mapCase.size() )
			<< BUILD_LOG( m_fTotalProb )
			<< BUILD_LOG( fProbAdd * m_mapCase.size() + m_fTotalProb );
	
		// over-flow나면 ( 100% - fTotalProb ) / mapCaseCount 만큼 각 case에 더해서 총합 100%로 만들어준다.
		float fIncreaseProb = ( 100.0f - m_fTotalProb ) / m_mapCase.size();

		m_fTotalProb = 0.0f;    // 전체 확률합도 이참에 새로 계산하자.
		std::map<int,KCaseUnit>::iterator mit;
		for( mit = m_mapCase.begin(); mit != m_mapCase.end(); mit++ )
		{
			mit->second.m_fProb += fIncreaseProb;

			m_fTotalProb += mit->second.m_fProb;
		}
	}
	else
	{
		m_fTotalProb = 0.0f;    // 전체 확률합도 이참에 새로 계산하자.
		std::map<int,KCaseUnit>::iterator mit;
		for( mit = m_mapCase.begin(); mit != m_mapCase.end(); mit++ )
		{
			mit->second.m_fProb += fProbAdd;

			m_fTotalProb += mit->second.m_fProb;
		}
	}

	LIF( m_fTotalProb <= 1.0f );

	return;
}
//}}
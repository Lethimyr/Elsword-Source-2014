#include "ActionDecider.h"

void KActionUnit::AddActionProbability(  int nActionID, float fProbability )
{
    m_vecSet.push_back( std::make_pair( nActionID, fProbability ) );
    m_fTotalProb = GetTotalProb() + fProbability;

    if( m_fTotalProb > 1.0f )
    {
        float fFactor = 1.0f / m_fTotalProb;

        std::vector< std::pair<int,float> >::iterator vit;
        for( vit = m_vecSet.begin(); vit != m_vecSet.end(); vit++ )
        {
            vit->second *= fFactor;
        }

        m_fTotalProb = 1.0f;
    }
}

float KActionUnit::GetTotalProb( bool bCalc /*= false*/ )
{
    if( bCalc )
    {
        m_fTotalProb = 0.0f;
        std::vector< std::pair<int,float> >::iterator vit;
        for( vit = m_vecSet.begin(); vit != m_vecSet.end(); vit++ )
        {
            m_fTotalProb += vit->second;
        }
    }

    return m_fTotalProb;
}

bool KActionUnit::Decision( IN float fRoulette, OUT int& nActionID )
{
    if( fRoulette < 0.0f )
    {
        fRoulette = 0.0f;
    }
    else if( fRoulette > 1.0f )
    {
        fRoulette = 1.0f;
    }

    if( fRoulette > GetTotalProb() )    return false;
    if( m_vecSet.empty() )                return false;

    std::vector<float> vecAccProb( m_vecSet.size() );

    vecAccProb[0] = m_vecSet[0].second;

    for( size_t i = 1; i < m_vecSet.size(); i++ )
    {
        vecAccProb[i] = m_vecSet[i].second + vecAccProb[i - 1];
    }

    int iSTART, iMIDDLE, iEND;
    iSTART = 0;
    iEND = m_vecSet.size() - 1;

    while( iSTART != iEND )
    {
        iMIDDLE = ( iSTART + iEND ) / 2;
        if( fRoulette > vecAccProb[iMIDDLE] )
        {
            iSTART = iMIDDLE + 1;
        }
        else
        {
            iEND = iMIDDLE;
        }
    }

    nActionID = m_vecSet[iSTART].first;
    return true;
}

//////////////////////////////////////////////////////////////////////////

KActionDecider::KActionDecider( int iInvalidAction /* = DEFAULT_INVALID_ACTION  */) : m_iInvalidAction( iInvalidAction )
{
}

KActionDecider::~KActionDecider()
{
}

void KActionDecider::AddAction( int iState, int iAction, float fProb )
{
    std::map<int, KActionUnit>::iterator atTableIt;
    atTableIt = m_mapActionTransitionTable.find( iState );

    if( atTableIt != m_mapActionTransitionTable.end() )
    {
        atTableIt->second.AddActionProbability( iAction, fProb );
    }
    else
    {
        KActionUnit kSet;
        kSet.AddActionProbability( iAction, fProb );
        m_mapActionTransitionTable[iState] = kSet;
    }
}

void KActionDecider::MakeDecision( std::map<int, int>& mapResult )
{
    mapResult.clear();

    std::map<int, KActionUnit>::iterator mit;

    for( mit = m_mapActionTransitionTable.begin(); mit != m_mapActionTransitionTable.end(); mit++ )
    {
        int iAction = MakeDecision( mit->first );

        if( iAction != m_iInvalidAction )
        {
            mapResult[mit->first] = iAction;
        }

    }
}

int KActionDecider::MakeDecision( int iState )
{
    float fRoulette = ( float )rand() / ( float )RAND_MAX;

    return MakeDecision( iState, fRoulette );
}

int KActionDecider::MakeDecision( int iState, float fRoulette )
{
    // START_LOG( clog, "·ê·¿°ª : " << fRoulette );

    int nReturn = m_iInvalidAction;

    std::map<int, KActionUnit>::iterator mit;
    mit = m_mapActionTransitionTable.find( iState );
    if( mit != m_mapActionTransitionTable.end() )
    {
        mit->second.Decision( fRoulette, nReturn );
    }

    return nReturn;
}

ImplToStringW( KActionUnit )
{
    std::vector< std::pair<int,float> >::const_iterator vit;
    for( vit = m_vecSet.begin(); vit != m_vecSet.end(); vit++ )
    {
        stm_ << vit->first << L"( " << vit->second << L" ) ";
    }

    return stm_ << std::endl;
}

ImplToStringW( KActionDecider )
{
    stm_ << L"Number of States : " << m_mapActionTransitionTable.size() << std::endl;

    std::map<int, KActionUnit>::const_iterator mit;
    for( mit = m_mapActionTransitionTable.begin(); mit != m_mapActionTransitionTable.end(); mit++ )
    {
        stm_ << L"State " << mit->first << L" : ";

        mit->second.ToString( stm_ );
    }
    
    return stm_;
}

ImplOstmOperatorW( KActionDecider );




#pragma once

#include <vector>
#include <map>
#include <ToString.h>

#include "dbg/dbg.hpp"

#define DEFAULT_INVALID_ACTION -1

class KActionUnit
{
    DeclToStringW;
public:
    KActionUnit() : m_fTotalProb( 0.0f ) {}
    KActionUnit( const KActionUnit& t ) { *this = t; }
    KActionUnit& operator=( const KActionUnit& t )
    {
        m_vecSet      = t.m_vecSet;
        m_fTotalProb  = t.m_fTotalProb;
        return *this;
    }

    // 동일한 ActionID를 두번 add하지 않음을 전제해야 한다. (별도로 체크하지 않음.)
    void AddActionProbability( int nActionID, float fProbability );
    float GetTotalProb( bool bCalc = false );
    bool Decision( IN float fRoulette, OUT int& nActionID );

protected:
    std::vector< std::pair<int,float> > m_vecSet; // pair( int : action id, float : probality )
    float m_fTotalProb;
};

class KActionDecider
{
    DeclToStringW;

public:
    KActionDecider( int iInvalidAction = DEFAULT_INVALID_ACTION );
    virtual ~KActionDecider();
    KActionDecider( const KActionDecider& t ) { *this = t; }
    KActionDecider& operator=( const KActionDecider& t )
    {
        m_mapActionTransitionTable  = t.m_mapActionTransitionTable;
        m_iInvalidAction            = t.m_iInvalidAction;

        return *this;
    }

    void AddAction( int iState, int iAction, float fProb );
    void MakeDecision( std::map<int, int>& mapResult );
    int MakeDecision( int iState );
    int MakeDecision( int iState, float fRoulette );    // for debugging & unit testing.

protected:
    std::map<int, KActionUnit> m_mapActionTransitionTable;
    int m_iInvalidAction;
};

DeclOstmOperatorW( KActionDecider );




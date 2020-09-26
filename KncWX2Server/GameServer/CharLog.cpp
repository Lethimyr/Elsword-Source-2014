#include "CharLog.h"
#include "x2data/XSLUnit.h"

KModeLogInfo::KModeLogInfo()
{
    m_nModeID       = 0;
    m_nModeCount    = 0;
    m_nPlayTime     = 0;
    m_nWin          = 0;
    m_nLose         = 0;
    m_nDraw         = 0;
}

KCharLogInfo::KCharLogInfo()
{
    m_nCharUID      = 0;
    m_ucCharType    = CXSLUnit::UT_ELSWORD;
    m_ucLevel       = 1;
    m_nEDGain       = 0;
    m_nEDSell       = 0;
    m_nEDSpend      = 0;
    m_nVP           = 0;
    m_nExp          = 0;
    m_nVP2Exp       = 0;
}

void KCharLogInfo::AddModeLog( int nModeID, int nPlayTime, eModeResult eResult )
{
    // 기존에 해당 모드에 대한 데이터가 있었는지를 검사할 필요 없다.
    // operator[]의 의도적인 사용으로 최초 데이터 추가를 자동으로 일어나게 한다.
    m_mapModeLog[ nModeID ].m_nModeID = nModeID;
    m_mapModeLog[ nModeID ].m_nModeCount++;
    m_mapModeLog[ nModeID ].m_nPlayTime += nPlayTime;
    switch( eResult )
    {
    case MR_WIN:
        break;
    case MR_DRAW:
        break;
    case MR_LOSE:
        break;
    }

    std::map<int,KModeLogInfo>::iterator mit;
    mit = m_mapModeLog.find( nModeID );

    if( mit == m_mapModeLog.end() ) // 기존에 해당 모드에 대한 데이터 없음 - 추가작업 진행
    {
        m_mapModeLog[ nModeID ].m_nModeCount++;
    }
}
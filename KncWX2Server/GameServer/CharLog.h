#pragma once

#include "KncUidType.h"
#include <string>
#include <map>

enum eModeResult {  // KCharLogInfo::AddModeLog(...) 함수 호출을 위해서만 사용.
    MR_WIN = 0,
    MR_DRAW,
    MR_LOSE,
};

struct KModeLogInfo
{
    KModeLogInfo();     // 멤버변수 기본값 설정을 위한 생성자. 

    int     m_nModeID;
    int     m_nModeCount;
    int     m_nPlayTime;
    int     m_nWin;         // 대전-승, 던전-클리어
    int     m_nLose;        // 대전-패, 던전-실패
    int     m_nDraw;        // 대전-비김, 던전-포기
};

// 캐릭터 1명의 게임 내용을
struct KCharLogInfo 
{
    UidType         m_nCharUID;

    unsigned char   m_ucCharType;
    unsigned char   m_ucLevel;
    std::wstring    m_strNickName;
    int             m_nEDGain;      // 획득한 게임머니
    int             m_nEDSell;      // 아이템 판매로 얻은 게임머니
    int             m_nEDSpend;     // 사용한 게임머니 양
    int             m_nVP;
    int             m_nExp;
    int             m_nVP2Exp;
    
    std::map<int,KModeLogInfo>  m_mapModeLog;

    KCharLogInfo();
    KCharLogInfo& operator=( const KCharLogInfo& right )
    {
        m_nCharUID      = right.m_nCharUID;
        m_ucCharType    = right.m_ucCharType;
        m_ucLevel       = right.m_ucLevel;
        m_strNickName   = right.m_strNickName;
        m_nEDGain       = right.m_nEDGain;
        m_nEDSell       = right.m_nEDSell;
        m_nEDSpend      = right.m_nEDSpend;
        m_nVP           = right.m_nVP;
        m_nExp          = right.m_nExp;
        m_nVP2Exp       = right.m_nVP2Exp;
        m_mapModeLog    = right.m_mapModeLog;

        return *this;
    }

    void AddModeLog( int nModeID, int nPlayTime, eModeResult eResult );
};
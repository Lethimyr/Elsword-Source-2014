#pragma once

#include <map>
#include <ToString.h>
#include <KncUtil.h>    // IN OUT

class KLottery
{
    DeclToStringW;
public:
    enum {
        CASE_BLANK = -1,
        PARAM_BLANK = -2,
    };

    struct KCaseUnit    // Decision 1회로 결정한 결과. 확률과 여분의 정보기록을 위한 param을 가짐.
    {
        float   m_fProb;
        int     m_nParam;
    };

    KLottery();
    ~KLottery();
    KLottery( const KLottery& t ) { *this = t; }    // copy constructor
    KLottery& operator=( const KLottery& t );       // assign operator

    float GetTotalProb() const  { return m_fTotalProb; }
    size_t GetCaseNum() const   { return m_mapCase.size(); }

    bool AddCase( int nCaseID, float fProbability, int nParam = PARAM_BLANK );
    int Decision() const;
    int GetParam( int nCaseID ) const;

    // various implement... (for Elsword project. 07.06.12.)
    bool AddProbability( float fProbAdd );  // 설정된 모든 case의 확률에 똑같이 덧셈. over-flow 나면 return false.
	//{{ 2007. 11. 8  최육사  설정된 모든 case의 확률에 똑같이 덧셈.
	// over-flow나면 ( 100% - TotalProb ) / mapCaseCount 의 값을 각 case의 Prob에 더함 [TotalProb이 100%가 넘지 않고 확률증가율을 동등하게 분배]
	void AddProbabilityHundredMaxProb( float fProbAdd );
	//}}

public:
//protected:
    std::map<int,KCaseUnit> m_mapCase;          // key : case id, element : KCaseUnit struct.
    float                   m_fTotalProb;       // 단위는 퍼센트(%)로 한다. 100이면 항상 일어남.
};
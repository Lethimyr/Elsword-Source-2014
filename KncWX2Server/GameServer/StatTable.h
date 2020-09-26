#pragma once

#include <vector>
#include <ToString.h>
#include <KNCSingleton.h>
#include <KncUtil.h>    // IN, OUT, windows.h...
#include "X2Data/XSLUnit.h"

class KStatTable
{
	DeclareSingleton( KStatTable );

public:
	struct KUnitStatInfo
	{
        KUnitStatInfo();    // default constructor

		//공격
		USHORT	m_usAtkPhysic;
		USHORT	m_usAtkMagic;

		//방어
		USHORT	m_usDefPhysic;
		USHORT	m_usDefMagic;

		//{{ 2008. 4. 11  최육사  
		UINT	m_uiHP;
		//}}
	};


public:
	virtual ~KStatTable();
	DeclToStringW;
	DeclDump;

	bool OpenScriptFile( const char* pFileName );
	bool GetUnitStat( IN char cUnitClass, IN u_char unitLevel, OUT KUnitStatInfo& sUnitStat );
    void SetUnitStat( IN int nUnitClass, IN u_char unitLevel );

    // 070605. florist. 굳이 이렇게 하지 않아도 되지만 오류 검사를 확실히 하기 위해 미리 갯수를 입력받는다.
    void ReserveMemory( IN int nUnitClass, IN u_char ucMaxLevel );

protected:
	KStatTable();

	void RegisterLuaBind();

	//{{ 2008. 2. 18  최육사  1차 전직으로 인한 자료형 수정
	std::map< int, std::vector< KUnitStatInfo > > m_mapStat;
	//}}
};

DefSingletonInline( KStatTable );

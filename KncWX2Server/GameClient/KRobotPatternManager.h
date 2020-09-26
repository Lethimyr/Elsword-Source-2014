#pragma once

#include <KNCSingleton.h>
#include "lottery.h"

struct KRobotPattern
{
	int								m_MapID;
	VECTOR3							m_vPos;
	std::vector< KFieldFrameInfo >	m_vecFieldFrame;
	bool							m_bIsRight;
};

class KRobotPatternManager
{
	DeclareSingleton( KRobotPatternManager );

public:
	typedef std::vector< int >		KPatternList;

public:
	KRobotPatternManager(void);
	~KRobotPatternManager(void);

	void SetMapLottery( const KLottery& kLot ) { m_kMapLottery = kLot; }
	bool OpenScriptFile( const char* pFileName );

	bool AddRobotPattern_LUA();

	bool GetRandomFieldRobotPattern( IN int iMapID, OUT std::list< KRobotPattern >& listPattern );	
	int	 GetRandomMapID();

private:
	std::map< int, KPatternList >					m_mapFieldPatternList;
	std::map< int, std::vector< KRobotPattern > >	m_mapPattern;

	KLottery										m_kMapLottery;
};

DefSingletonInline( KRobotPatternManager );
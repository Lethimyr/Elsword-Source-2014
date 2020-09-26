#pragma once

#include "GSUser.h"
#include "ServerPacket.h"

#include <Windows.h>
#include <map>

#include <RTTI.h>
#include <ToString.h>
#include <KNCSingleton.h>

//훈련소 템플릿 관리 클래스 ㅡㅡ;; 제발 클래스 이름좀 고정화 시키자..

class KTrainingCenterTable
{
	DeclareSingleton( KTrainingCenterTable );
	DeclToStringW;
	DeclDump;

	struct TC_TABLE_INFO
	{
		int		m_iID;
		int		m_iDungeonID;

		int		m_iBeforeID;
		int		m_iRewardED;
		int		m_iRewardEXP;
		int		m_iMinLevel;
		float	m_fPlayTime;
		
		int		m_iDifficulty;

		//std::wstring		m_wstrTitle;
		//std::wstring		m_wstrDesc;

		std::vector<char>	m_vecUnitClass;
	};

public:
	KTrainingCenterTable(void);
	virtual ~KTrainingCenterTable(void);

	bool	OpenScriptFile( const char* pFileName );

	bool	AddTCTemplet_LUA();
	bool	SetTrainingListByUnitClass_LUA() { return true; };

	bool	CheckIfEnter( IN std::map< int, KTCClearInfo >& mapTCClear, IN int iTCID );
	int		GetDungeonID( IN int iTCID );	//return -1이면 에러
	float	GetPlayTime( IN int iTCID );
	bool	GetReward( IN int iTCID, OUT int& iRewardED, OUT int& iRewardEXP );

#ifdef DO_NOT_COUNT_DEAD_NPC_RELATED_TO_TITLE_MISSION_IN_TRAINING_ROOM
	bool	IsTrainingCenter( IN int iDungeonID );
#endif // DO_NOT_COUNT_DEAD_NPC_RELATED_TO_TITLE_MISSION_IN_TRAINING_ROOM


private:
	std::map<int, TC_TABLE_INFO>		m_mapTCInfo;
};

DefSingletonInline( KTrainingCenterTable );
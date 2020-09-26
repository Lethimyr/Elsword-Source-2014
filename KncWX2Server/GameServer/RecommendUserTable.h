#pragma once

#include "serverdefine.h"
#include "RefreshSingleton.h"
#include <map>


class KRecommendUserTable
{
private:
	DeclareRefreshSingleton( KRecommendUserTable );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclToStringW;
	DeclDump;

public:
	KRecommendUserTable(void);
	~KRecommendUserTable(void);

	// for lua
	bool AddRecommendUserData_LUA( u_char ucLevel, int iRewardID );

	// result
	int GetRecommendUserReward( IN u_char ucLevel ) const;

	//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
	bool AddRecommendEventReward_LUA( int iRewardID );
	int GetRecommendEventReward()				{ return m_iRecommendEventReward; }

	bool AddRecommendUserEventReward_LUA( u_char ucLevel, int iRewardID );
	int GetRecommendUserEventReward( u_char ucLevel );
	
	bool AddRecommendedEventReward1_LUA( int iRewardID );
	int GetRecommendedEventReward1()				{ return m_iRecommendedEventReward1; }

	bool AddRecommendedEventReward2_LUA( int iRewardID );
	int GetRecommendedEventReward2()				{ return m_iRecommendedEventReward2; }
#endif SERV_RECOMMEND_LIST_EVENT
	//}

private:
	std::map< u_char, int > m_mapRecommendData;

	//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
	int m_iRecommendEventReward;
	int m_iRecommendedEventReward1;
	int m_iRecommendedEventReward2;
	std::map< u_char, int > m_mapRecommendEventData;
#endif SERV_RECOMMEND_LIST_EVENT
	//}
};

DefRefreshSingletonInline( KRecommendUserTable );
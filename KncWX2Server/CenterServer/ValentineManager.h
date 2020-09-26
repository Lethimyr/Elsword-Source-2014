#pragma once


#include "Event.h"
#include "CommonPacket.h"
#include <boost/timer.hpp>

//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON

class KValentineManager
{

public:
	KValentineManager(void);
	~KValentineManager(void);

	enum ENUM_ETC_TIMER
	{
		TEM_AFTER_DUNGEON_START,			// 던전 시작 시 'START' UI 노출과 동시에 흘러가는 시간
		TEM_AFTER_DUNGEON_START_5_SEC,		// 던전 시작 시 'START' UI 노출과 동시에 흘러가는 시간 이후 5초
		TEM_TOTAL_NUM,
	};

	enum ENUM_LIMITE_DUNGEON_TIME
	{
		LDT_VALENTINE = 99,			// 던전 종료 시간
	};
	
	enum ENUM_TIME_TYPE
	{
		TT_NONE,
		TT_SYNC_TIME,
		TT_END_GAME,
	};

	void Init();
	bool SetStartTimer( IN int iEnum, IN bool bStart );
	bool IsOnTimer( IN int iEnum );
	void CheckTimer( IN int iEnum, OUT int& iType );
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	void CheckTimerForInt( IN int iEnum, OUT int& iType );		// 해외팀 발렌타인 이벤트 던전용 함수 추가
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
	int GetRemainTime( IN int iEnum );

private:
	boost::timer		m_kTimer[TEM_TOTAL_NUM];
	bool				m_bOnTimer[TEM_TOTAL_NUM];
};

#endif SERV_EVENT_VALENTINE_DUNGEON
//}
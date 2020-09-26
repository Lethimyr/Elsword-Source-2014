#pragma once 

#include "TRThread.h"
#include <windows.h>
#include <KNCSingleton.h>
//#include "ThreadManager.h"
#include <Thread/Thread.h>
#include "KncLua.h"
#include <lua_tinker.h>
//#include <ToString.h>

class KTRUserManager : public KThread//public KThreadManager
{
	DeclareSingleton( KTRUserManager );
	DeclToStringW;
	DeclDump;

	struct USER_AGE
	{
		int m_iMin;
		int m_iMax;

		USER_AGE()
		{
			m_iMin = 0;
			m_iMax = 0;
		}
	};

protected:
	KTRUserManager();
public:
	virtual ~KTRUserManager();

	bool	Init();
	bool	ShutDown();

	virtual	void	Run();
			void	Tick();

			void	DeleteRobot( IN UidType iUnitUID );

	//virtual void EndThread();	// derived from KThreadManager

	void lua_inter(){lua_interpreter( m_pLua );}

	// lua
	void UserAge_LUA( int iMin, int iMax );
	void SetIP_LUA( const char* strIP ){m_strIP = strIP;}
	void GetLiveRobot_LUA();

private:
	//virtual KThread* CreateThread();	// derived from KThreadManager
	void	RegToLua();
	int		TokenizebyIP();	//임시 테스트를 위한 함수.

protected:
	lua_State*	m_pLua;

	//std::vector<USER_AGE>	m_vecUserAge;	// 쓰레드별 유저 인덱스 범위
	std::string					m_strIP;
	USER_AGE					m_sUserAge;

	mutable CRITICAL_SECTION	m_csRobot;
	std::vector<KTRUserPtr>		m_vecRobot;
};

DefSingletonInline( KTRUserManager );
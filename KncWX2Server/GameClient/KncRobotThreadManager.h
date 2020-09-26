#pragma once

#include <KNCSingleton.h>
#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

#include "KncRobotThread.h"

class KncRobotThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclareSingleton( KncRobotThreadManager );

	struct UNIT_RANGE
	{
		int		m_iMin;
		int		m_iMax;
	};

public:
	KncRobotThreadManager();
	virtual ~KncRobotThreadManager();

	void		RegToLua();
	bool		Init();
	void		Run( bool bRunInterpreter );
	bool		ShutDown();

	void		SetNum_Lua( int iMin, int iMax );
	void		SetGSAddress_Lua( const char* szIP, u_short usPort );
	void		SetVersion_Lua( const char* szVersion );
	std::wstring& GetVersion(){ return m_wstrVersion; }

protected:
	FSMclassPtr                     m_spFSM;
	boost::shared_ptr<lua_State>    m_sptlua;
	mutable CRITICAL_SECTION        m_csRobot;

	std::vector<KncRobotThread*>	m_vecRobotThread;

	std::wstring                    m_wstrVersion;

	std::vector<UNIT_RANGE>			m_vecUnitRange;
};

DefSingletonInline( KncRobotThreadManager );
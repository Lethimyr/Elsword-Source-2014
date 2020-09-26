#pragma once

#include "KncRobot.h"

#include <vector>
#include <Thread/Thread.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <KncLua.h>
#include <lua_tinker.h>
#include "lua/KLuaManager.h"
using namespace lua_tinker;

SmartPointer( KRBFSM );

class KncRobotManager : public KThread
{
    DeclDump;
    DeclToStringW;
    DeclareSingleton( KncRobotManager );

public:
    KncRobotManager();
    virtual ~KncRobotManager();

	boost::shared_ptr<lua_State> GetLuaState() { return m_sptlua; }
    void	RegToLua();
    bool	Init();
    void	Run( bool bRunInterpreter );
    bool	ShutDown();

    virtual void	Run();
			void	Tick();

    int		GetNum() const;
    int		GetLiveNum() const;
    void	SetNum( size_t nMin, size_t nMax );
    void	AddGSAddress( const char* szIP, u_short usPort );
	void	SetVersion( const char* szVersion );
	int		GetMaxRobotCount() const;

	//{{ 2008. 12. 11  최육사
	void	AddFieldRandomInfo( int iMapID, float fRate );
	void	SetLimitOption( int iValue ) { m_iLimitOption = iValue; }
	int		GetLimitOption() { return m_iLimitOption; }
	//}}

	std::wstring& GetVersion(){ return m_wstrVersion; }

	//{{ 2009. 9. 28  최육사	메가폰테스트
	void	SetEnumFilePath_LUA( const char* pPath );
	void	SetVillageMapFilePath_LUA( const char* pPath );
	void	SetRobotPatternFilePath_LUA( const char* pPath );
	//}}

	void	DeleteByRobot( int iIndex );

protected:
	void	Tick_RobotProcess();

protected:
    FSMclassPtr                     m_spFSM;

    boost::shared_ptr<lua_State>    m_sptlua;
    mutable CRITICAL_SECTION        m_csRobot;

    std::vector< KncRobotPtr >      m_vecRobot;
	std::vector< int >				m_vecCreateRobot;
	std::vector< int >				m_vecDeleteRobot;

	std::wstring                    m_wstrVersion;

	int								m_maxBotCnt;
	int								m_iLimitOption;

	KLottery						m_kRandomMapTemp;

	//{{ 2009. 9. 28  최육사	메가폰테스트
	std::string						m_strEnumFilePath;
	std::string						m_strVillageMapFilePath;
	std::string						m_strRobotPatternFilePath;
	//}}
};

DefSingletonInline( KncRobotManager );



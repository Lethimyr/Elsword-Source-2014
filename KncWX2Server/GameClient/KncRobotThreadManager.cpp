#include "KncRobotThreadManager.h"

#include <dbg/dbg.hpp>
#include "Socket/IOCP.h"
#include "GSFSM.h"

ImplToStringW( KncRobotThreadManager )
{
	stm_ << "*** KncRobotThreadManager ***" << std::endl;

	KLocker lock( m_csRobot );

	for( int i = 0; i < (int)m_vecRobotThread.size(); ++i )
	{
		m_vecRobotThread[i]->ToString( stm_ );
	}

	return stm_;
}

ImplementSingleton( KncRobotThreadManager );

KncRobotThreadManager::KncRobotThreadManager()
{
	InitializeCriticalSection( &m_csRobot );
}

KncRobotThreadManager::~KncRobotThreadManager()
{
	m_vecUnitRange.clear();

	DeleteCriticalSection( &m_csRobot );
}

void KncRobotThreadManager::RegToLua()
{
	def( "log",			dbg::logfile::SetLogLevel );
	def( "changelog",	dbg::logfile::CloseFile );
	def( "GetLocalIP",	NetCommon::GetLocalIP );

	class_< KActionDecider >( "KActionDecider" )
		.def( "AddAction", &KActionDecider::AddAction );
	decl( "ActionDecider", &KncRobot::ms_kActionDecider );

#   undef _ENUM
#   define _ENUM( id ) decl( #id, KGSFSM::##id );
#   include "GSFSMState_def.h"

#   undef _ENUM
#   define _ENUM( id ) decl( #id, KncRobot::##id );
#   include "RobotAction_def.h"

	def( "SetConnectCapacity", &KncRobot::SetConnectCapacity );

	class_< KncRobotThreadManager >( "KncRobotThreadManager" )
		.def( "dump",		&KncRobotThreadManager::Dump )
		.def( "set",		&KncRobotThreadManager::SetNum_Lua )
		.def( "AddServer",	&KncRobotThreadManager::SetGSAddress_Lua )
		.def( "SetVersion",	&KncRobotThreadManager::SetVersion_Lua );

	decl( "RobotManager", this );
}

bool KncRobotThreadManager::Init()
{
	dbg::logfile::SetFileNameAtTime( true );

	NetCommon::InitWinSock();
	KncSecurity::InitSecurity();
	SiKIocp()->Init( 4 );

	m_spFSM = KGSFSMPtr( new KGSFSM );

	m_sptlua = boost::shared_ptr<lua_State>( lua_open(), lua_close );

	_JIF( m_sptlua != NULL, return false );

    //luaopen_base( m_sptlua.get() );  // 루아함수 tonumber를 사용해야 한다.
    //luaopen_table( m_sptlua.get() );
    //luaopen_io( m_sptlua.get() );
    //luaopen_string( m_sptlua.get() );
    //luaopen_math( m_sptlua.get() );

    luaL_openlibs( m_sptlua.get() );

    lua_settop( m_sptlua.get(), 0 );

	RegToLua();
	_JIF( 0 == luaL_dofile( m_sptlua.get(), "config_rb.lua" ), return false );

	SiKIocp()->BeginThread();

	bool bRet =  true;
	{
		KLocker lock( m_csRobot );

		for( int i = 0; i < (int)m_vecUnitRange.size(); ++i )
		{
			KncRobotThread*	pThread = new KncRobotThread( i, m_spFSM );

			UNIT_RANGE sUnitRange = m_vecUnitRange[i];
			pThread->SetNum( sUnitRange.m_iMin, sUnitRange.m_iMax );

			if( pThread->Init() == false )
			{
				START_LOG( cerr, L"쓰레드 생성실패.!" )
					<< BUILD_LOG( i )
					<< END_LOG;

				delete pThread;

				continue;
			}

			m_vecRobotThread.push_back( pThread );

			START_LOG( cout2, L":: 쓰레드 생성 ::" )
				<< BUILD_LOG( i )
				<< BUILD_LOG( sUnitRange.m_iMin )
				<< BUILD_LOG( sUnitRange.m_iMax )
				;
		}
	}
	return bRet;
}

void KncRobotThreadManager::Run( bool bRunInterpreter )
{
	if( bRunInterpreter )
	{
		lua_interpreter( m_sptlua.get() );
	}
}

bool KncRobotThreadManager::ShutDown()
{
	for( int i = 0; i < (int)m_vecRobotThread.size(); ++i )
	{
		if( m_vecRobotThread[i] != NULL )
		{
			m_vecRobotThread[i]->ShutDown();

			delete m_vecRobotThread[i];
		}
		else
		{
			START_LOG( cerr, L"RobotThread 삭제실패.~!" )
				<< END_LOG;
		}
	}
	m_vecRobotThread.clear();

	SiKIocp()->EndThread();
	KncSecurity::ReleaseSecurity();
	::WSACleanup();

	return true;
}

void KncRobotThreadManager::SetNum_Lua( int iMin, int iMax )
{
	UNIT_RANGE sUnitRange;
	sUnitRange.m_iMin	= iMin;
	sUnitRange.m_iMax	= iMax;

	m_vecUnitRange.push_back( sUnitRange );

	START_LOG( cout2, L"로봇 생성정뵤 : " )
		<< BUILD_LOG( iMin )
		<< BUILD_LOG( iMax )
		<< BUILD_LOG( m_vecUnitRange.size() )
		;
}

void KncRobotThreadManager::SetGSAddress_Lua( const char* szIP, u_short usPort )
{
	std::string strIP = szIP;
	KNetAddress kAddress;
	kAddress.m_wstrIP = KncUtil::toWideString( strIP );
	kAddress.m_usPort = usPort;
	KncRobot::AddGSAddress( kAddress );
}

void KncRobotThreadManager::SetVersion_Lua( const char* szVersion )
{
	std::string str	= szVersion;
	m_wstrVersion	= KncUtil::toWideString( str );

	dbg::cout << "---------------------------------------" << dbg::endl;
	dbg::cout << "ROBOT CONNECTING VERSION : " << str.c_str() << dbg::endl;
	dbg::cout << "---------------------------------------" << dbg::endl;
}
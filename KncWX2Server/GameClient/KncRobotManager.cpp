#include "KncRobotManager.h"

#include <boost/format.hpp>
#include <dbg/dbg.hpp>
#include "Socket/IOCP.h"
#include "RBFSM.h"

#include "Lib/X2LocationManager.h"
#include "Lib/KTDGLineMap.h"
#include "KRobotPatternManager.h"

ImplToStringW( KncRobotManager )
{
    return stm_ << "*** KncRobotManager ***" << std::endl
				<< TOSTRINGW( GetMaxRobotCount() )
                << TOSTRINGW( GetNum() )
                << TOSTRINGW( GetLiveNum() );
}

ImplementSingleton( KncRobotManager );

KncRobotManager::KncRobotManager() :
m_iLimitOption( 0 )
{
    InitializeCriticalSection( &m_csRobot );
}

KncRobotManager::~KncRobotManager()
{
    DeleteCriticalSection( &m_csRobot );
}

void KncRobotManager::RegToLua()
{
	lua_tinker::def( m_sptlua.get(), "log",			&dbg::logfile::SetLogLevel );
	lua_tinker::def( m_sptlua.get(), "changelog",	&dbg::logfile::CloseFile );
	lua_tinker::def( m_sptlua.get(), "GetLocalIP",	&NetCommon::GetLocalIP );

    //def( "log", dbg::logfile::SetLogLevel );
    //def( "changelog", dbg::logfile::CloseFile );
    //def( "GetLocalIP", NetCommon::GetLocalIP );

	lua_tinker::class_add<KActionDecider>( m_sptlua.get(), "KActionDecider" );
	lua_tinker::class_def<KActionDecider>( m_sptlua.get(), "AddAction", &KActionDecider::AddAction );
	lua_tinker::decl( m_sptlua.get(), "ActionDecider", &KncRobot::ms_kActionDecider );

    //class_< KActionDecider >( "KActionDecider" )
    //    .def( "AddAction", &KActionDecider::AddAction );
    //decl( "ActionDecider", &KncRobot::ms_kActionDecider );

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( m_sptlua.get(), #id, KRBFSM::##id );
#   include "RBFSMState_def.h"

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( m_sptlua.get(), #id, KncRobot::##id );
#   include "RobotAction_def.h"

	lua_tinker::def( m_sptlua.get(), "SetConnectCapacity", &KncRobot::SetConnectCapacity );

	lua_tinker::class_add<KncRobotManager>( m_sptlua.get(), "KncRobotManager" );
	lua_tinker::class_def<KncRobotManager>( m_sptlua.get(), "dump",						&KncRobotManager::Dump );
	lua_tinker::class_def<KncRobotManager>( m_sptlua.get(), "set",						&KncRobotManager::SetNum );
	lua_tinker::class_def<KncRobotManager>( m_sptlua.get(), "AddServer",				&KncRobotManager::AddGSAddress );
	lua_tinker::class_def<KncRobotManager>( m_sptlua.get(), "SetVersion",				&KncRobotManager::SetVersion );
	lua_tinker::class_def<KncRobotManager>( m_sptlua.get(), "AddFieldRandomInfo",		&KncRobotManager::AddFieldRandomInfo );
	lua_tinker::class_def<KncRobotManager>( m_sptlua.get(), "SetLimitOption",			&KncRobotManager::SetLimitOption );
	//{{ 2009. 9. 28  최육사	메가폰테스트
	lua_tinker::class_def<KncRobotManager>( m_sptlua.get(), "SetEnumFilePath",			&KncRobotManager::SetEnumFilePath_LUA );
	lua_tinker::class_def<KncRobotManager>( m_sptlua.get(), "SetVillageMapFilePath",	&KncRobotManager::SetVillageMapFilePath_LUA );
	lua_tinker::class_def<KncRobotManager>( m_sptlua.get(), "SetRobotPatternFilePath",	&KncRobotManager::SetRobotPatternFilePath_LUA );
	//}}
	
	lua_tinker::decl( m_sptlua.get(), "RobotManager", this );

    //def( "SetConnectCapacity", &KncRobot::SetConnectCapacity );

  //  class_< KncRobotManager >( "KncRobotManager" )
  //      .def( "dump",		&KncRobotManager::Dump )
  //      .def( "set",		&KncRobotManager::SetNum )
  //      .def( "AddServer",	&KncRobotManager::AddGSAddress )
		//.def( "SetVersion",	&KncRobotManager::SetVersion );

    //decl( "RobotManager", this );
}

bool KncRobotManager::Init()
{
    dbg::logfile::SetFileNameAtTime( true );

    NetCommon::InitWinSock();
    KncSecurity::InitSecurity();
    SiKIocp()->Init( 4 );

    m_spFSM = KRBFSMPtr( new KRBFSM );

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

	//////////////////////////////////////////////////////////////////////////
	// X2Lib 초기화

	// enum
	if( 0 != luaL_dofile( m_sptlua.get(), m_strEnumFilePath.c_str() ) )
	{
		START_LOG( cerr, L"enum.lua 스크립트 파싱 실패!" );
		return false;
	}

	// dlg_map_village
	if( SiCX2LocationManager()->OpenScriptFile( "MapVillage.lua" ) == false )
	{
		START_LOG( cerr, L"MapVillage.lua 스크립트 파싱 실패!" );
		return false;
	}

	// robot pattern
	int iIdx = 0;
	while( true )
	{
		std::string strFileName = m_strRobotPatternFilePath;
		strFileName += boost::str( boost::format( "%d" ) % iIdx );
		strFileName += ".lua";

		if( !SiKRobotPatternManager()->OpenScriptFile( strFileName.c_str() ) )
			break;

		++iIdx;

		START_LOG( cout, L"로봇 패턴 스크립트 파싱 성공!" )
			<< BUILD_LOG( strFileName );
	}

	// mapid random init
	SiKRobotPatternManager()->SetMapLottery( m_kRandomMapTemp );
	//////////////////////////////////////////////////////////////////////////

    return KThread::Begin();
}

void KncRobotManager::Run( bool bRunInterpreter )
{
    if( bRunInterpreter )
    {
        lua_interpreter( m_sptlua.get() );
    }
}

bool KncRobotManager::ShutDown()
{
    KThread::End();
    
    SiKIocp()->EndThread();
    KncSecurity::ReleaseSecurity();
    ::WSACleanup();

    return true;
}

void KncRobotManager::Run()
{
    srand( ( unsigned int )time( NULL ) );      // 쓰레드마다 해줘야 한다.

    DWORD ret;

    while( true )
    {
        ret = ::WaitForSingleObject( m_hKillEvent, 50 );       // 0.05s 간격

        if( ret == WAIT_OBJECT_0 ) break;

        if( ret == WAIT_TIMEOUT ) Tick();

        else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;

    }

    return;
}

void KncRobotManager::Tick()
{
    std::vector< KncRobotPtr >::iterator vit;

	Tick_RobotProcess();

    KLocker lock( m_csRobot );
    for( vit = m_vecRobot.begin(); vit != m_vecRobot.end(); vit++ )
    {
        ( *vit )->Tick();
    }
}

int KncRobotManager::GetNum() const
{
    KLocker lock( m_csRobot );

    return ( int )m_vecRobot.size();
}

int KncRobotManager::GetLiveNum() const
{
    KLocker lock( m_csRobot );

    int iCount = 0;
    std::vector< KncRobotPtr >::const_iterator vit;
    for( vit = m_vecRobot.begin(); vit != m_vecRobot.end(); vit++ )
    {
        if( ( *vit )->GetStateID() >= KRBFSM::S_LOGINED && ( *vit )->GetStateID() < KRBFSM::STATE_SENTINEL )
        {
            iCount++;
        }
    }

    return iCount;
}

int KncRobotManager::GetMaxRobotCount() const
{
	KLocker lock( m_csRobot );

	return m_maxBotCnt;
}

void KncRobotManager::AddFieldRandomInfo( int iMapID, float fRate )
{
	m_kRandomMapTemp.AddCase( iMapID, fRate );
}

void KncRobotManager::SetNum( size_t nMin, size_t nMax )
{
    START_LOG( cout, "로봇 생성 : " << m_vecRobot.size() << " -> " << nMin << " <-> " << nMax );

    KLocker lock( m_csRobot );

	for( size_t i = nMin; i <= nMax; ++i )
	{
		//KncRobotPtr spRobot( new KncRobot );

		//spRobot->SetFSM( m_spFSM );
		//spRobot->StateTransition( KGSFSM::I_TO_EXIT );
		//spRobot->Init( true );
		//spRobot->SetIndex( i );
		//m_vecRobot.push_back( spRobot );

		m_vecCreateRobot.push_back( i );
	}

	m_maxBotCnt = nMax;

    //size_t nCurNum = m_vecRobot.size();

    //if( nCurNum == nNum )
    //{
    //    dbg::cout << "현재 생성된 로봇 수와 동일";
    //}
    //else if( nCurNum > nNum )   // 객체를 소멸해야 하는 경우
    //{
    //    m_vecRobot.erase( m_vecRobot.begin() + nNum, m_vecRobot.end() );
    //}
    //else    // 객체를 생성해야 하는 경우
    //{
    //    m_vecRobot.reserve( nNum );

    //    for( size_t i = m_vecRobot.size(); i < nNum; i++ )
    //    {
    //        KncRobotPtr spRobot( new KncRobot );

    //        spRobot->SetFSM( m_spFSM );
    //        spRobot->StateTransition( KGSFSM::I_TO_EXIT );
    //        spRobot->Init( true );
    //        spRobot->SetIndex( i );
    //        m_vecRobot.push_back( spRobot );
    //    }
    //}
}

void KncRobotManager::AddGSAddress( const char* szIP, u_short usPort )
{
    std::string strIP = szIP;
    KNetAddress kAddress;
    kAddress.m_wstrIP = KncUtil::toWideString( strIP );
    kAddress.m_usPort = usPort;
    KncRobot::AddGSAddress( kAddress );
}

void KncRobotManager::SetVersion( const char* szVersion )
{
	std::string str	= szVersion;
	m_wstrVersion	= KncUtil::toWideString( str );

	dbg::cout << "---------------------------------------" << dbg::endl;
	dbg::cout << "ROBOT CONNECTING VERSION : " << str.c_str() << dbg::endl;
	dbg::cout << "---------------------------------------" << dbg::endl;
}

void KncRobotManager::DeleteByRobot( int iIndex )
{
	KLocker lock( m_csRobot );

	m_vecDeleteRobot.push_back( iIndex );
}

//{{ 2009. 9. 28  최육사	메가폰테스트
void KncRobotManager::SetEnumFilePath_LUA( const char* pPath )
{
	m_strEnumFilePath = pPath;
}

void KncRobotManager::SetVillageMapFilePath_LUA( const char* pPath )
{
	m_strVillageMapFilePath = pPath;
}

void KncRobotManager::SetRobotPatternFilePath_LUA( const char* pPath )
{
    m_strRobotPatternFilePath = pPath;
}
//}}

void KncRobotManager::Tick_RobotProcess()
{
	KLocker lock( m_csRobot );

	std::vector< int >::iterator vit;
	std::vector< KncRobotPtr >::iterator vitRobot;
	bool bIsDelete = false;

	//로봇 삭제
	for( vit = m_vecDeleteRobot.begin(); vit != m_vecDeleteRobot.end(); ++vit )
	{
		bIsDelete = false;
		int iIndex = *vit;

		for( vitRobot = m_vecRobot.begin(); vitRobot != m_vecRobot.end(); ++vitRobot )
		{
			if( (*vitRobot)->GetIndex() == iIndex )
			{
				m_vecRobot.erase( vitRobot );
				m_vecCreateRobot.push_back( iIndex );

				bIsDelete = true;

				START_LOG( clog, L"로봇봇~~ 삭제.!" )
					<< BUILD_LOG( iIndex );

				break;
			}
		}

		if( bIsDelete == false )
		{
			START_LOG( cerr, L"로봇 삭제 실패.!" )
				<< BUILD_LOG( iIndex )
				<< END_LOG;
		}
	}
	m_vecDeleteRobot.clear();

	//로봇 생성
	//int nCreateCnt = 0;
	for( int i = 0; i < (int)m_vecCreateRobot.size(); ++i )
	{
		//우선 테스트로 한틱에 10명만 생성..
		//if( nCreateCnt > 9 )
		//	break;

		KncRobotPtr spRobot( new KncRobot );

		spRobot->SetFSM( m_spFSM );
		spRobot->StateTransition( KRBFSM::I_TO_EXIT );
		spRobot->Init( true );

		spRobot->SetIndex( m_vecCreateRobot[i] );

		m_vecRobot.push_back( spRobot );

		START_LOG( clog, L"로봇봇~~ 생성.!" )
			<< BUILD_LOG( m_vecCreateRobot[i] );

		m_vecCreateRobot.erase( m_vecCreateRobot.begin() + i );
		--i;

		//++nCreateCnt;
	}
}




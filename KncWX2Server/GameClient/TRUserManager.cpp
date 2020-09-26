#include "TRUserManager.h"
#include "KncSend.h"
#include "Socket/NetCommon.h"
#include "TRIOCP.h"

ImplementSingleton( KTRUserManager );

ImplToStringW( KTRUserManager )
{
	return stm_ << L"*** KTRUserManager ***" << std::endl;
	//<< TOSTRINGW( GetMaxRobotCount() )
	//<< TOSTRINGW( GetNum() )
	//<< TOSTRINGW( GetLiveNum() );
}


KTRUserManager::KTRUserManager()
:
m_pLua(NULL)
{
	InitializeCriticalSection( &m_csRobot );
}

KTRUserManager::~KTRUserManager()
{
	DeleteCriticalSection( &m_csRobot );

	lua_close(m_pLua);
}

bool KTRUserManager::Init()
{
	dbg::logfile::SetFileNameAtTime( true );

	NetCommon::InitWinSock();
	KncSecurity::InitSecurity();
	SiKTRIocp()->Init(4);

	m_pLua = lua_open();

	_JIF( m_pLua != NULL, return false );
    //luaopen_base( m_pLua );  // 루아함수 tonumber를 사용해야 한다.
    //luaopen_table( m_pLua );
    //luaopen_io( m_pLua );
    //luaopen_string( m_pLua );
    //luaopen_math( m_pLua );

    luaL_openlibs( m_pLua );

    lua_settop( m_pLua, 0 );

	RegToLua();
	//_JIF( 0 == luaL_dofile( m_pLua, "config_rb.lua" ), return false );
	dbg::logfile::SetLogLevel(2);

	//생성할 쓰레드 개수를 넣어주고
	//오버로드한 CreaeThread() 함수에서 설정값을 넘겨준다.
	//if( m_vecUserAge.empty() == false )
	//	KThreadManager::Init(static_cast<int>(m_vecUserAge.size()));

	START_LOG( clog, L"로봇 시작.!" );

	SiKTRIocp()->BeginThread();

	m_sUserAge.m_iMin = TokenizebyIP();
	m_sUserAge.m_iMax = m_sUserAge.m_iMin + 99;
	for( int i = m_sUserAge.m_iMin; i <= m_sUserAge.m_iMax; ++i )
	{
		KTRUserPtr spTRUser = KTRUserPtr( new KTRUser() );

		spTRUser->Init();

		if( !spTRUser->Connect( "192.168.140.53"/*m_strIP.c_str()*/, 9301, static_cast<UidType>(i) ) )
		{
			START_LOG( cerr, L"접속 실패.!" )
				<< BUILD_LOG( i )
				<< END_LOG;

			continue;
		}
		else
		{
			START_LOG( clog, L"접속 성공.!" )
				<< BUILD_LOG( i )
				;

			UidType iUID = spTRUser->GetUID();
			if( spTRUser->SendPacket( ETR_REG_UID, iUID ) == false )
			{
				START_LOG( cerr, L"REG_UID send failed.!" )
					<< END_LOG;
			}
		}

		m_vecRobot.push_back( spTRUser );
	}

	dbg::logfile::SetLogLevel(0);

	return Begin();
}

//void KTRUserManager::EndThread()
//{
//	std::vector< boost::shared_ptr<KThread> >::iterator vit;
//	for( vit = m_vecThread.begin(); vit != m_vecThread.end(); ++vit )
//	{
//		(*vit)->End();
//	}
//}

//KThread* KTRUserManager::CreateThread()
//{
//	KTRThread* pThread = NULL;
//
//	std::vector<USER_AGE>::iterator vit;
//	vit = m_vecUserAge.begin();
//	if( vit != m_vecUserAge.end() )
//	{
//		pThread = new KTRThread(vit->m_iMin, vit->m_iMax, m_strIP.c_str());
//		m_vecUserAge.erase(vit);
//	}
//
//	return pThread;
//}

bool KTRUserManager::ShutDown()
{
	KThread::End();

	SiKTRIocp()->EndThread();
	KncSecurity::ReleaseSecurity();
	::WSACleanup();

	return true;
}

void KTRUserManager::UserAge_LUA(int iMin, int iMax)
{
	m_sUserAge.m_iMin	= iMin;
	m_sUserAge.m_iMax	= iMax;

	START_LOG( cout2, L"Set Robot indes.!" )
		<< BUILD_LOG( iMin )
		<< BUILD_LOG( iMax )
		;
	//USER_AGE uAge;
	//uAge.m_iMin = iMin;
	//uAge.m_iMax = iMax;

	//m_vecUserAge.push_back(uAge);
}

void KTRUserManager::Run()
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

void KTRUserManager::Tick()
{
	std::vector<KTRUserPtr>::iterator vit;

	KLocker lock(m_csRobot);
	for( vit = m_vecRobot.begin(); vit != m_vecRobot.end(); vit++ )
	{
		( *vit )->Tick();
	}
}

void KTRUserManager::DeleteRobot(IN UidType iUnitUID)
{
	KLocker lock(m_csRobot);

	std::vector<KTRUserPtr>::iterator vit;
	for( vit = m_vecRobot.begin(); vit != m_vecRobot.end(); vit++ )
	{
		if( (*vit)->GetUID() == iUnitUID )
		{
			m_vecRobot.erase(vit);

			START_LOG( cout2, L"나 지워진다.!!" )
				<< BUILD_LOG( iUnitUID )
				<< dbg::endl;

			break;
		}
	}
}

void KTRUserManager::GetLiveRobot_LUA()
{
	KLocker lock(m_csRobot);
	START_LOG( cout2, L"살아있는 로봇의 개수는.?" )
		<< BUILD_LOG( static_cast<int>(m_vecRobot.size()) )
		<< dbg::endl;
}

void KTRUserManager::RegToLua()
{
	lua_tinker::def( m_pLua,	"log",			&dbg::logfile::SetLogLevel );
	//lua_tinker::def( m_pLua,	"changelog",	&dbg::logfile::CloseFile );
	//lua_tinker::def( m_pLua,	"GetLocalIP",	&NetCommon::GetLocalIP );

	lua_tinker::class_add<KTRUserManager>( m_pLua, "KTRUserManager" );
	lua_tinker::class_def<KTRUserManager>( m_pLua, "UserAge",		&KTRUserManager::UserAge_LUA );
	lua_tinker::class_def<KTRUserManager>( m_pLua, "SetIP",			&KTRUserManager::SetIP_LUA );
	lua_tinker::class_def<KTRUserManager>( m_pLua, "GetLiveRobot",	&KTRUserManager::GetLiveRobot_LUA );

	lua_tinker::decl( m_pLua, "robot", this );
}

int KTRUserManager::TokenizebyIP()
{
	char szIP[MAX_PATH] = "";

	strcpy( szIP, NetCommon::GetLocalIP() );

	if( strlen(szIP) <= 0 )
		return 0;

	int iPoint = 0;

	std::string strTemp;

	for( int i = 0; i < static_cast<int>(strlen(szIP)); ++i )
	{
		if( iPoint == 3 )
		{
			strTemp.push_back(*(szIP+i));
		}

		if( *(szIP+i) == '.' )
		{
			++iPoint;
		}
	}

	if( strTemp.size() >= 1 )
	{
		int iTemp = atoi(strTemp.c_str());

		return iTemp * 100;
	}
	
	return 0;
}







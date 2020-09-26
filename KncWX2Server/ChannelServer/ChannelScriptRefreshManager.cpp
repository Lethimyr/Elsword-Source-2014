#include "ProxyManager.h"
#include "ChannelScriptRefreshThread.h"
#include "ChannelScriptRefreshManager.h"
#include "KncSend.h"
#include "KAutoPath.h"


//{{ 2012. 10. 15	박세훈	채널 서버 실시간 스크립트 갱신 기능
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT

//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT

ImplPfID( KChannelScriptRefreshManager, PI_NULL );

ImplementSingleton( KChannelScriptRefreshManager );

KChannelScriptRefreshManager::KChannelScriptRefreshManager() :
m_pRefreshLuaState( NULL )
{
}

KChannelScriptRefreshManager::~KChannelScriptRefreshManager()
{
	if( m_pRefreshLuaState != NULL )
	{
		lua_close( m_pRefreshLuaState );
		//{{ 2010. 8. 30	최육사	서버 정상 종료 처리		
		m_pRefreshLuaState = NULL;
		//}}
	}
}

void KChannelScriptRefreshManager::RegToLua( lua_State* pLuaState )
{
	lua_tinker::class_add<KChannelScriptRefreshManager>( pLuaState, "KChannelScriptRefreshManager" );

	//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	lua_tinker::class_def<KChannelScriptRefreshManager>( pLuaState, "SmsManager",	&KChannelScriptRefreshManager::SmsManager_Lua );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	lua_tinker::class_def<KChannelScriptRefreshManager>( pLuaState, "LogManager",	&KChannelScriptRefreshManager::LogManager_Lua );
#endif SERV_LOG_SYSTEM_NEW
	//}}

	lua_tinker::decl( pLuaState, "ScriptRefresh", this );
}

void KChannelScriptRefreshManager::Init()
{
	// 스크립트 갱신 전용 lua_state 생성!
	m_pRefreshLuaState = lua_open();

	_JIF( m_pRefreshLuaState != NULL, return );

	luaL_openlibs( m_pRefreshLuaState );
	lua_settop( m_pRefreshLuaState, 0 );

	{
		// enum.lua 파싱
		std::string strFile = "Enum.lua";
		KAutoPath kAutoPath;
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( m_pRefreshLuaState, strFile.c_str() ) )
		{
			START_LOG( cerr, L"GSScriptRefreshManager : Enum 정보 로드 실패.!" );
			return;
		}
	}

	{
		// enum.lua 파싱
		std::string strFile = "DungeonEnum.lua";
		KAutoPath kAutoPath;
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( m_pRefreshLuaState, strFile.c_str() ) )
		{
			START_LOG( cerr, L"GSScriptRefreshManager : DungeonEnum 정보 로드 실패.!" );
			return;
		}
	}

	// 스레드는 무조건 1개만 생성!
	KThreadManager::Init( 1 );

	// 스레드 시작!
	KThreadManager::BeginThread();

	START_LOG( cout, L"스크립트 리프레쉬 스레드 시작!" );
}

KThread* KChannelScriptRefreshManager::CreateThread()
{
	return new KChannelScriptRefreshThread;
}

//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
void KChannelScriptRefreshManager::SmsManager_Lua( void )
{
	START_LOG( cout, L"KSMSPhoneNumberManager 클래스 스크립트 실시간 패치!");

	// 채널 서버의 SMSPhoneNumberManager 갱신 요청!
	KEventPtr spEvent( new KEvent );
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CH_SMS_MANAGER;

	spEvent->SetData( PI_CHANNEL_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kNot );
	SiKChannelScriptRefreshManager()->QueueingEvent( spEvent );
}
#endif SERV_SMS_TOTAL_MANAGER
//}}

//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
void KChannelScriptRefreshManager::LogManager_Lua()
{
	START_LOG( cout, L"KLogManager 클래스 스크립트 실시간 패치!");

	// 채널 서버의 SMSPhoneNumberManager 갱신 요청!
	KEventPtr spEvent( new KEvent );
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CH_LOG_MANAGER;

	spEvent->SetData( PI_CHANNEL_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kNot );
	SiKChannelScriptRefreshManager()->QueueingEvent( spEvent );
}
#endif SERV_LOG_SYSTEM_NEW
//}}

#endif SERV_REALTIME_SCRIPT
//}}

#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
//}}

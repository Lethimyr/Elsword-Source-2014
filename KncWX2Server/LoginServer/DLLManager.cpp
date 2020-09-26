#include "LoginSimLayer.h"
#include "BaseServer.h"

#include ".\DLLManager.h"


//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
	#include "AccountBlockManager.h"
#endif SERV_HACKING_DLL_CHECK
//}}


#ifdef SERV_DLL_MANAGER


ImplementSingleton( KDLLManager );

KDLLManager::KDLLManager(void)
{
	//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
	m_iReleaseTick				= 0;
#endif SERV_HACKING_DLL_CHECK
	//}}
}

KDLLManager::~KDLLManager(void)
{
}

ImplToStringW( KDLLManager )
{
	return stm_;
}

void KDLLManager::Tick()
{
	//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
	if( m_kReleaseTickTimer.elapsed() > 120.0 )
	{
		m_kReleaseTickTimer.restart();

		KDBE_GET_HACKING_MODULE_LIST_REQ kReq;
		kReq.m_iReleaseTick = m_iReleaseTick;
		SendToAccountDB( DBE_GET_HACKING_MODULE_LIST_REQ, kReq );
	}
#endif SERV_HACKING_DLL_CHECK
	//}}

	if( m_kTimer.elapsed() < 300.0 )
		return;

	m_kTimer.restart();

	// DLL 관리 리스트
	KDBE_GET_MODULE_INFO_LIST_ACK kPacket;
	GetDBUPdateInfo( kPacket.m_vecModuleList );
	if( kPacket.m_vecModuleList.empty() == false )
	{
		SendToLogDB( DBE_UPDATE_MODULE_INFO_LIST_REQ, kPacket );
	}
}

void KDLLManager::OnServerReadyComplete()
{
	START_LOG( cout, L"DLL 관리 리스트 요청!" );

	SendToLogDB( DBE_GET_MODULE_INFO_LIST_REQ );

	START_LOG( cout, L"해킹 DLL 리스트 요청!" );

#ifdef SERV_HACKING_DLL_CHECK // 빌드 오류로 해외팀 추가
	KDBE_GET_HACKING_MODULE_LIST_REQ kReq;
	kReq.m_iReleaseTick = m_iReleaseTick;
	SendToAccountDB( DBE_GET_HACKING_MODULE_LIST_REQ, kReq );
#endif SERV_HACKING_DLL_CHECK
}

//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
void KDLLManager::Init( IN const KDBE_GET_MODULE_INFO_LIST_ACK& kPacket )
{
	BOOST_TEST_FOREACH( const KModuleInfo&, kInfo, kPacket.m_vecModuleList )
	{
		AddModuleInfo( kInfo );

		if( kInfo.m_iAllow != KModuleInfo::MIA_NONE )
		{
			START_LOG( cerr, L"Allow 값이 있는 module info" )
				<< BUILD_LOG( kInfo.m_wstrModuleName )
				<< BUILD_LOG( kInfo.m_iAllow )
				<< BUILD_LOG( kInfo.m_iCount )
				<< BUILD_LOG( kInfo.m_iModuleUID )
				<< END_LOG;
		}
	}

	std::map< int, int >::const_iterator mit = kPacket.m_mapReleaseTick.find( KAccountBlockManager::BT_MODULE_LIST );
	if( mit != kPacket.m_mapReleaseTick.end() )
	{
		m_iReleaseTick = mit->second;
	}
	else
	{
		m_iReleaseTick = 0;
	}
}

bool KDLLManager::UpdateModuleInfo( IN const std::wstring& wstrModuleName, OUT KModuleInfo& kModule )
{
	std::map< std::wstring, KModuleInfo >::iterator mit;
	mit = m_mapModuleList.find( wstrModuleName );
	if( mit == m_mapModuleList.end() )
	{
		KModuleInfo kInfo;
		kInfo.m_iModuleUID = 0; // 0으로 초기화! UID발급 안된 상태임
		kInfo.m_iCount = 1;
		kInfo.m_bReserveDBUpdate = true;
		kInfo.m_wstrModuleName = wstrModuleName;
		AddModuleInfo( kInfo );
	}
	else
	{
		++mit->second.m_iCount;
		mit->second.m_bReserveDBUpdate = true;

		if( mit->second.m_iAllow != KModuleInfo::MIA_NONE )
		{
			kModule = mit->second;
			return false;
		}
	}

	return true;
}

void KDLLManager::InitHackingModuleList( IN const std::vector< KModuleInfo >& vecModuleList, IN int& iReleaseTick )
{
	m_mapHackingModuleList.clear();

	m_iReleaseTick = iReleaseTick;

	BOOST_TEST_FOREACH( const KModuleInfo&, kInfo, vecModuleList )
	{
		m_mapHackingModuleList.insert( std::make_pair( kInfo.m_wstrModuleName, kInfo ) );

		if( kInfo.m_iAllow != KModuleInfo::MIA_NONE )
		{
			START_LOG( clog, L"Allow 값이 Black List 입니다." )
				<< BUILD_LOG( kInfo.m_wstrModuleName )
				<< BUILD_LOG( kInfo.m_iAllow )
				<< BUILD_LOG( kInfo.m_iCount )
				<< BUILD_LOG( kInfo.m_iModuleUID )
				<< END_LOG;
		}
	}
}

bool KDLLManager::CheckHackingModuleList( IN std::vector< std::wstring >& vecModuleList, OUT KELG_CHECK_MODULE_INFO_UPDATE_NOT& kPacket )
{
	bool bCheckResult = true;

	std::map< std::wstring, KModuleInfo >::iterator mit;
	
	BOOST_TEST_FOREACH( const std::wstring&, wstrName, vecModuleList )
	{
		mit = m_mapHackingModuleList.find( wstrName );	
		if( mit != m_mapHackingModuleList.end() )
		{

			START_LOG( clog, L"Allow 값이 Black List 입니다." )
				<< BUILD_LOG( mit->second.m_wstrModuleName )
				<< BUILD_LOG( mit->second.m_iAllow )
				<< BUILD_LOG( mit->second.m_iCount )
				<< BUILD_LOG( mit->second.m_iModuleUID )
				<< END_LOG;

			bCheckResult = false;
			kPacket.m_mapModuleName.insert( std::make_pair( mit->second.m_wstrModuleName, mit->second ) );
		}
	}

	return bCheckResult;
}
#else
void KDLLManager::Init( IN const std::vector< KModuleInfo >& vecModuleList )
{
	BOOST_TEST_FOREACH( const KModuleInfo&, kInfo, vecModuleList )
	{
		AddModuleInfo( kInfo );
	}
}
#endif SERV_HACKING_DLL_CHECK
//}}

void KDLLManager::UpdateModuleInfo( IN const std::wstring& wstrModuleName )
{
	std::map< std::wstring, KModuleInfo >::iterator mit;
	mit = m_mapModuleList.find( wstrModuleName );
	if( mit == m_mapModuleList.end() )
	{
		KModuleInfo kInfo;
		kInfo.m_iModuleUID = 0; // 0으로 초기화! UID발급 안된 상태임
		kInfo.m_iCount = 1;
		kInfo.m_bReserveDBUpdate = true;
		kInfo.m_wstrModuleName = wstrModuleName;
		AddModuleInfo( kInfo );
	}
	else
	{
		++mit->second.m_iCount;
		mit->second.m_bReserveDBUpdate = true;
	}
}

void KDLLManager::Shutdown()
{
	KDBE_GET_MODULE_INFO_LIST_ACK kPacket;
	GetDBUPdateInfo( kPacket.m_vecModuleList );
	if( kPacket.m_vecModuleList.empty() == false )
	{
		SendToLogDB( DBE_UPDATE_MODULE_INFO_LIST_REQ, kPacket );
	}
}

void KDLLManager::UpdateModuleList( IN const std::vector< std::wstring >& vecModuleList )
{
	BOOST_TEST_FOREACH( const std::wstring&, wstrModuleName, vecModuleList )
	{
		UpdateModuleInfo( wstrModuleName );
	}
}

void KDLLManager::UpdateModuleList( IN const std::vector< KModuleInfo >& vecModuleList )
{
	BOOST_TEST_FOREACH( const KModuleInfo&, kInfo, vecModuleList )
	{
		UpdateModuleUID( kInfo.m_wstrModuleName, kInfo.m_iModuleUID );
	}
}

bool KDLLManager::AddModuleInfo( IN const KModuleInfo& kInfo )
{
	if( m_mapModuleList.find( kInfo.m_wstrModuleName ) != m_mapModuleList.end() )
	{
		START_LOG( cwarn, L"이미 등록되어있는 모듈UID입니다." )
			<< BUILD_LOG( kInfo.m_wstrModuleName )
			<< END_LOG;
		return false;
	}

	m_mapModuleList.insert( std::make_pair( kInfo.m_wstrModuleName, kInfo ) );
	return true;
}

bool KDLLManager::UpdateModuleUID( IN const std::wstring& wstrModuleName, IN int iModuleUID )
{
	std::map< std::wstring, KModuleInfo >::iterator mit;
	mit = m_mapModuleList.find( wstrModuleName );
	if( mit == m_mapModuleList.end() )
	{
		START_LOG( cerr, L"모듈 정보를 찾지 못했습니다!" )
			<< BUILD_LOG( wstrModuleName )
			<< END_LOG;
		return false;
	}

    mit->second.m_iModuleUID = iModuleUID;
	return true;
}

void KDLLManager::GetDBUPdateInfo( OUT std::vector< KModuleInfo >& vecUpdateInfo )
{
	vecUpdateInfo.clear();

	std::map< std::wstring, KModuleInfo >::iterator mit;
	for( mit = m_mapModuleList.begin(); mit != m_mapModuleList.end(); ++mit )
	{
		if( mit->second.m_bReserveDBUpdate == false )
			continue;

        vecUpdateInfo.push_back( mit->second );

		mit->second.m_bReserveDBUpdate = false;
	}
}

void KDLLManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}

//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
void KDLLManager::SendToAccountDB( unsigned short usEventID )
{
	SendToAccountDB( usEventID, char() );
}
#endif SERV_HACKING_DLL_CHECK
//}}



#endif SERV_DLL_MANAGER



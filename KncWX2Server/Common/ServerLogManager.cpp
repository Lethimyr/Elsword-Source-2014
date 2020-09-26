#include "ServerLogManager.h"



//////////////////////////////////////////////////////////////////////////
const wchar_t* serverlog::GetLogLevelStr( serverlog::LOG_LEVEL eLogLevel )
{
	switch( eLogLevel )
	{
	case serverlog::CLOG:	return L"clog";
	case serverlog::CLOG2:	return L"clog2";
	case serverlog::CWARN:	return L"cwarn";
	case serverlog::CERR:	return L"cerr";
	case serverlog::COUT:	return L"cout";
	case serverlog::COUT2:	return L"cout2";
	}

	return L"unknown";
}


//////////////////////////////////////////////////////////////////////////
// log stream
KLogStream::KLogStream()
{
}

KLogStream::~KLogStream()
{
	// log 데이터 생성
	KLogInfoPtr spLogInfo = KLogInfoPtr( new KLogInfo( m_kLogInfo ) );
	if( spLogInfo == NULL )
	{
		START_LOG( cerr, L"log 데이터 생성 실패!" )
			<< END_LOG;
		return;
	}

	KServerLogManager::GetInstance().QueueingToLog( spLogInfo );
}

void KLogStream::EndLog( const wchar_t* pStrFileName, int iStrLine )
{
	std::wstringstream wstrStream;
	wstrStream << L"    " << L"(" << pStrFileName << L", " << iStrLine << L")" << std::endl;

	m_kLogInfo.m_vecContent.push_back( wstrStream.str() );
	m_kLogInfo.m_iFileLine = iStrLine;
}


//////////////////////////////////////////////////////////////////////////
// server log manager
KServerLogManager KServerLogManager::ms_selfInstance;


ImplToStringW( KServerLogManager )
{
	return stm_;
}

KServerLogManager::KServerLogManager() : 
m_bIsShutDown( false )
{
}

KServerLogManager::~KServerLogManager()
{
}

void KServerLogManager::QueueingToLog( const KLogInfoPtr& spLogInfo )
{
	KLocker lock( m_csLogDataQueue );

	if( m_bIsShutDown )
		return;
	
	m_LogDataQueue.push( spLogInfo );
}

bool KServerLogManager::GetLogInfo( KLogInfoPtr& spLogInfo )
{
	KLocker lock( m_csLogDataQueue );

	if( m_LogDataQueue.empty() )
		return false;

	spLogInfo = m_LogDataQueue.front();
	m_LogDataQueue.pop();
	return true;
}



void KServerLogManager::Run()
{
	START_LOG( cout, L"Server Log Manager Run()" );

	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 1 );     // 0.05s 간격

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Loop();

		else std::cout <<  "*** WaitForSingleObject() - return : "<< ret << std::endl;
	}
}

void KServerLogManager::Loop()
{
	KLogInfoPtr spLogInfo;

	if( GetLogInfo( spLogInfo ) == false )	
		return;

	switch( spLogInfo->m_cLogLevel )
	{
	case serverlog::CLOG:
		{
			for( u_int ui = 0; ui < spLogInfo->m_vecContent.size(); ++ui )
			{
				dbg::clog << spLogInfo->m_vecContent[ui];
			}
		}
		break;

	case serverlog::CLOG2:
		{
			for( u_int ui = 0; ui < spLogInfo->m_vecContent.size(); ++ui )
			{
				dbg::clog2 << spLogInfo->m_vecContent[ui];
			}
		}
		break;

	case serverlog::CWARN:
		{
			for( u_int ui = 0; ui < spLogInfo->m_vecContent.size(); ++ui )
			{
				dbg::cwarn << spLogInfo->m_vecContent[ui];
			}
		}
		break;

	case serverlog::CERR:
		{
			for( u_int ui = 0; ui < spLogInfo->m_vecContent.size(); ++ui )
			{
				dbg::cerr << spLogInfo->m_vecContent[ui];
			}
		}
		break;

	case serverlog::COUT:
		{
			for( u_int ui = 0; ui < spLogInfo->m_vecContent.size(); ++ui )
			{
				dbg::cout << spLogInfo->m_vecContent[ui];
			}
		}
		break;

	case serverlog::COUT2:
		{
			for( u_int ui = 0; ui < spLogInfo->m_vecContent.size(); ++ui )
			{
				dbg::cout2 << spLogInfo->m_vecContent[ui];
			}
		}
		break;
	}
}

void KServerLogManager::Shutdown()
{
	KLocker lock( m_csLogDataQueue );

	// 종료 예약
	m_bIsShutDown = true;

	// 스레드 종료
	End();
}



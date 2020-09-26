#include "Socket/Session.h"
#include "GarenaBillingIOThread.h"
#include "dbg/dbg.hpp"
#include "GarenaBillingIOCP.h"

_ImplementException( KGarenaBillingIOThread )

KGarenaBillingIOThread::KGarenaBillingIOThread(void)
{
}

KGarenaBillingIOThread::~KGarenaBillingIOThread(void)
{
	SiKGarenaBillingIocp()->PostStatus( KGarenaBillingIocp::SIGNAL_SHUTDOWN_SERVER );
	End();
}

void KGarenaBillingIOThread::Run()
{
	START_LOG( clog, L"Enter Run()" );

	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 0 );     // 0.05s 간격

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Loop();

		else std::cout <<  "*** WaitForSingleObject() - return : "<< ret << std::endl;

	}// while
	//START_LOG( clog, L"Leave Run()" );    // 041229. 가끔 exception 발생
}

void KGarenaBillingIOThread::Loop() 
{
	_BEGIN_TRY_BLOCK;

	LPOVERLAPPED povl       = NULL;
	DWORD dwBytesTransfer   = 0; 
	DWORD dwKey             = 0;

	bool bOK = SiKGarenaBillingIocp()->GetStatus(&dwKey, &dwBytesTransfer, &povl );

	if( dwKey == KGarenaBillingIocp::SIGNAL_SHUTDOWN_SERVER && povl == NULL )
	{
		// 종료용 I/O Completion packet인지 확인 
		std::cout << "*** SIGNAL_SHUTDOWN_SERVER entered.\n" << std::endl;
		SetEvent( m_hKillEvent );
		return;
	} // if

	if( bOK )
	{
		SiKGarenaBillingIocp()->OnIOCompleted( dwKey, dwBytesTransfer, povl );
	}

	else // ( !bOK ) // local machine이 연결을 종료한 경우
	{
		START_LOG( clog2, L"closed by local machine. dwKey : " << dwKey );
		//std::wstring wstrErr = KncUtil::GetLastErrMsg();

		//LOG_SUCCESS( wstrErr.compare(L"에러값 얻기 실패.!") == 0 ) 
		//	<< L"closed by local machine. dwKey : " << dwKey << L", Err Msg : " << wstrErr << dbg::endl;

		SiKGarenaBillingIocp()->OnClosedByLocal( dwKey );
	}

	_END_TRY_BLOCK( L"" );
}
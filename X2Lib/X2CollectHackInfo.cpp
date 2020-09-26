
#include "StdAfx.h"

#ifdef ADD_COLLECT_CLIENT_INFO

#include "X2CollectHackInfo.h"

CX2CollectHackInfo::CX2CollectHackInfo(void)
{
	m_pCollectServer = NULL;
	m_bSendPacket = false;
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
    m_vecHackImgBuf_Thread.resize(0);
    m_iImageOffset_Thread = 0;
    m_lCancelSendCSImg_Interlocked = 0;
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	m_pHackImgBuf = NULL;
	m_lTotalSize = 0;
	m_iImageOffset = 0;
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX

	m_bRunThread = false;
	::InitializeCriticalSection( &m_csEventQueue );
}


CX2CollectHackInfo::~CX2CollectHackInfo(void)
{
	m_bRunThread = false;
	ClearQueue();

	EndThread(5000);

	::DeleteCriticalSection( &m_csEventQueue );
#ifndef X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	SAFE_DELETE_ARRAY( m_pHackImgBuf );
	m_lTotalSize = 0;
	m_iImageOffset = 0;
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX

}

bool CX2CollectHackInfo::BeginThread()
{
	m_bRunThread = true;
	return KJohnThread::BeginThread();
}//BeginThread()

void CX2CollectHackInfo::EndThread(DWORD dwTimeout_)
{				
	KJohnThread::EndThread(dwTimeout_);
}//EndThread()

DWORD CX2CollectHackInfo::RunThread()
{
#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_START
#endif

#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
    g_CKTDXLog.RegisterCurrentThread();
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

	while(1)
	{	
		THEMIDA_ENCODE_START

		if( m_bRunThread == false )
			break;

		DoProcessEvent_Thread();
		Sleep(100);

		THEMIDA_ENCODE_END
	}

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif
	
	return 0;
}//RunThread()

void CX2CollectHackInfo::ClearQueue()
{
	::EnterCriticalSection( &m_csEventQueue );  // 050630. 이벤트를 처리하는 동안 unlock 되도록 수정.

	// traverse all event
	COLLECT_SERVER_PACKET spEvent;

	while( !m_queEvent.empty() )	
	{
		// 050912. florist. 050911에 넷마블 테섭이 이곳에서 crash를 일으킴. 별도 예외처리.
		try 
		{ 
			spEvent = m_queEvent.front(); 
		} 
		catch( ... ) 
		{ 
			m_queEvent.pop(); 			
			continue;
		}

		m_queEvent.pop();

		::LeaveCriticalSection( &m_csEventQueue );

		SAFE_DELETE( spEvent.m_pSerBuff )
			::EnterCriticalSection( &m_csEventQueue );
	}

	::LeaveCriticalSection( &m_csEventQueue );
}
void CX2CollectHackInfo::EnQueue( unsigned short iEventID, KSerBuffer *pBuff, bool bSend, unsigned short usType, std::string strHackInfo, bool bExit )
{
	CSLock lock( m_csEventQueue );

	COLLECT_SERVER_PACKET csPacket;
	csPacket.m_iEventID = iEventID;
	csPacket.m_pSerBuff = pBuff;
	csPacket.m_bSend = bSend;
	csPacket.m_usType = usType;
	csPacket.m_strHackInfo = strHackInfo;
	csPacket.m_bExit = bExit;
	m_queEvent.push( csPacket );

}

bool CX2CollectHackInfo::ConnectCollectServer_Thread()
{
	bool bIsSERVICE = false;

#ifdef _SERVICE_
	bIsSERVICE = true;
#endif
	if( m_pCollectServer == NULL )
		m_pCollectServer = new CX2ServerProtocol( KTDXSendGameMessage, g_pKTDXApp->GetHWND(), XGM_RECEIVE_FROM_COLLECTSERVER, CX2ServerEvent::COLLECT_SERVER_EVENT_ID_STR, COLLECT_EVENTID_SENTINEL, bIsSERVICE );


	if( m_pCollectServer != NULL )
	{
		if( m_pCollectServer->IsCSConnected() == true )
			return true;

		bool bCSConnected = m_pCollectServer->ConnectedToCollectServer( L"14.45.79.56", 13050 );
		//bool bCSConnected = m_pCollectServer->ConnectedToCollectServer( L"192.168.66.28", 13050 );

		if( bCSConnected == false )
			return false;

		return true;
	}	

	return false;
}

void CX2CollectHackInfo::LoadHackImg_Thread( const std::string &strFileName )
{
	FILE* hFile = NULL;

	g_pMain->MakeHackScreenShot( strFileName.c_str() );

	hFile = fopen(strFileName.c_str(), "rb");
	if(hFile == NULL)
	{
		m_bSendPacket = false;
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
        m_vecHackImgBuf_Thread.resize( 0 );
        m_iImageOffset_Thread = 0;
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
		SAFE_DELETE_ARRAY( m_pHackImgBuf );
		m_lTotalSize = 0;
		m_iImageOffset = 0;
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX

		return;
	}

	fseek(hFile, 0, SEEK_END);
	long lTotalSize = ftell(hFile);	

	if ( 0 < lTotalSize )
	{
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
        m_vecHackImgBuf_Thread.resize( lTotalSize + 1 );
        memset( &m_vecHackImgBuf_Thread.front(), 0, sizeof(char) * lTotalSize+1 );
        m_iImageOffset_Thread = 0;
        fseek (hFile,0,SEEK_SET);
        fread(&m_vecHackImgBuf_Thread.front(), sizeof(char), lTotalSize, hFile);
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
        m_lTotalSize = lTotalSize;
		m_pHackImgBuf = new char[m_lTotalSize+1];
		memset( m_pHackImgBuf, 0, sizeof(char) * m_lTotalSize+1 );

        		m_iImageOffset = 0;

		fseek (hFile,0,SEEK_SET);
		fread(m_pHackImgBuf, sizeof(char), m_lTotalSize, hFile);
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	}
    else
    {
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
        m_vecHackImgBuf_Thread.resize( 0 );
        m_iImageOffset_Thread = 0;
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
        m_lTotalSize = 0;
        m_iImageOffset = 0;
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
    }

	fclose(hFile);
	DeleteFileA( strFileName.c_str() );
}

void CX2CollectHackInfo::SendCSImg_Thread( const std::string &strFileName, bool bExit )
{
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
    if ( m_vecHackImgBuf_Thread.empty() == true 
        || m_iImageOffset_Thread * CRASH_IMAGE_BUFF_MAX >= (int) m_vecHackImgBuf_Thread.size() )
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	if( m_pHackImgBuf == NULL || m_lTotalSize <= 0 )
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	{
#if defined( _SERVICE_ )
		ELSWORD_VIRTUALIZER_START
#endif
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
        m_iImageOffset_Thread = 0;
        m_vecHackImgBuf_Thread.resize( 0 );
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
		m_iImageOffset = 0;
		m_lTotalSize = 0;
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
		m_bSendPacket = false;
		if( g_pKTDXApp != NULL && bExit == true )
		{
			g_pKTDXApp->SetFindHacking(true);
		}
#if defined( _SERVICE_ )
		ELSWORD_VIRTUALIZER_END
#endif
		return;
	}

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_START
#endif
	long lStartOffset = 0l;
	long lEndOffset = 0l;
	bool bLast = false;

#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	lStartOffset = m_iImageOffset_Thread * CRASH_IMAGE_BUFF_MAX;
	lEndOffset = lStartOffset + CRASH_IMAGE_BUFF_MAX - 1;
	if( lEndOffset >= (long) m_vecHackImgBuf_Thread.size() - 1 )
	{
		bLast = true;
		lEndOffset = m_vecHackImgBuf_Thread.size() - 1;
	}	
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	lStartOffset = m_iImageOffset * CRASH_IMAGE_BUFF_MAX;
	lEndOffset = lStartOffset + CRASH_IMAGE_BUFF_MAX - 1;
	if( lEndOffset >= m_lTotalSize - 1 )
	{
		bLast = true;
		lEndOffset = m_lTotalSize - 1;
	}		
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX

	KECL_CRASH_IMAGE_INFO_REQ kEvent;
	kEvent.m_strFileName = strFileName;
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
    kEvent.m_sNo = m_iImageOffset_Thread;
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	kEvent.m_sNo = m_iImageOffset;
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	kEvent.m_bLast = bLast;
	kEvent.m_bExit = bExit;
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
    kEvent.m_vecImageBuff.assign( m_vecHackImgBuf_Thread.begin() + lStartOffset, m_vecHackImgBuf_Thread.begin() + lEndOffset+1 );
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	kEvent.m_vecImageBuff = vector<char>( &m_pHackImgBuf[lStartOffset], &m_pHackImgBuf[lEndOffset+1] );
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX

	kEvent.m_dwBuffSize	= lEndOffset - lStartOffset + 1;

	bool bConnectedCollectServer = true;
	if( m_pCollectServer == NULL || m_pCollectServer->IsCSConnected() == false )
	{
		bConnectedCollectServer = ConnectCollectServer_Thread();

		if( g_pKTDXApp != NULL && bExit == true )
		{
			g_pKTDXApp->SetFindHacking(true);
		}
	}
	if( bConnectedCollectServer == false )
	{
		ErrorLogMsg( XEM_ERROR1, "Connect Fail (CollectServer)" );

		g_pMain->SendHackMail_HackUserNot("Not Connect Server");
				
		m_bSendPacket = false;

#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
		m_iImageOffset_Thread = 0;
        m_vecHackImgBuf_Thread.resize( 0 );
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
		m_iImageOffset = 0;
		m_lTotalSize = 0;
		SAFE_DELETE_ARRAY( m_pHackImgBuf );
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX

		if( g_pKTDXApp != NULL && bExit == true )
		{
			g_pKTDXApp->SetFindHacking(true);
		}

		return;
	}

	m_pCollectServer->SendCSPacket( ECL_CRASH_IMAGE_INFO_REQ, kEvent );
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
    ++m_iImageOffset_Thread;
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	++m_iImageOffset;
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
	//CRASH_IMAGE_BUFF_MAX

// 이미지 파일들을 모두 보낸경우
	if( bLast == true )
	{
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
		m_iImageOffset_Thread = 0;
        m_vecHackImgBuf_Thread.resize( 0 );
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
		m_iImageOffset = 0;
		m_lTotalSize = 0;
		SAFE_DELETE_ARRAY( m_pHackImgBuf );
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
						
		m_bSendPacket = false;
		
		if( g_pKTDXApp != NULL && bExit == true )
		{
			g_pKTDXApp->SetFindHacking(true);
		}
	}	

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_END
#endif
}

void CX2CollectHackInfo::SendCSEvent_Thread( unsigned short iEventId, unsigned short usType, const std::string &strHackInfo, bool bExit )
{

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_START
#endif

	bool bConnectedCollectServer = true;
	if( m_pCollectServer == NULL || m_pCollectServer->IsCSConnected() == false )
	{
		bConnectedCollectServer = ConnectCollectServer_Thread();
		//g_pMain->EnQueue( iEventId, usType );
		//return;
	}

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_END
#endif

	if( bConnectedCollectServer == false )
	{
#if defined( _SERVICE_ )
		ELSWORD_VIRTUALIZER_START
#endif
		ErrorLogMsg( XEM_ERROR1, "Connect Fail (CollectServer)" );

		g_pMain->SendHackMail_HackUserNot("Not Connect Server");

		m_bSendPacket = false;

#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
		m_iImageOffset_Thread = 0;
        m_vecHackImgBuf_Thread.resize( 0 );
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
		m_iImageOffset = 0;
		m_lTotalSize = 0;
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX

#if defined( _SERVICE_ )
		ELSWORD_VIRTUALIZER_END
#endif

		// kimhc // 김현철 // 2013-11-20// 연결이 안된 경우에는 그냥 해킹 종료 시키자
		if( g_pKTDXApp != NULL && bExit == true )
		{
			g_pKTDXApp->SetFindHacking(true);
		}

		return;
	}

	if( iEventId == ECL_CRASH_INFO_REQ && m_bSendPacket == true )
	{
#if defined( _SERVICE_ )
		ELSWORD_VIRTUALIZER_START
#endif
		EnQueue( iEventId, NULL, true, usType, strHackInfo, bExit );
#if defined( _SERVICE_ )
		ELSWORD_VIRTUALIZER_END
#endif
		return;
	}

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_START
#endif
#ifdef MORE_INFO_ABOUT_HACK
	stringstream strstmInfo;
	// 시스템 정보
	g_pMain->AddSystemInfo(strstmInfo);

	// DLL 정보
	g_pMain->AddDLLInfo(strstmInfo);

	// 프로세스 & 스레드 정보
	g_pMain->AddProcessInfo(strstmInfo);
#endif // MORE_INFO_ABOUT_HACK

#ifdef CHECK_ALL_WINDOW
	HWND hWnd = ::GetTopWindow(NULL);
	while(hWnd)
	{
		string strWndText = " ";
		string strWndClass = " ";
		WCHAR wTextWindow[_MAX_PATH] = L"";
		WCHAR wTextClass[_MAX_PATH] = L"";
		int hr1 = GetWindowText(hWnd, wTextWindow, _MAX_PATH);
		int hr2 = GetClassName(hWnd,wTextClass,_MAX_PATH);

		THREAD_WND_INFO threadWndInfo;

		if( hr1 > 0 )
			ConvertWCHARToChar(strWndText, wTextWindow);

		if( hr2 > 0 )
			ConvertWCHARToChar(strWndClass, wTextClass);

		if( hr1 > 0 )
		{
			if( strWndText.compare( "MSCTFIME UI") != 0 && strWndText.compare( "Default IME") != 0 )
			{
				strstmInfo << strWndText.c_str() << "\t" << strWndClass.c_str() << "\n";
			}
		}

		hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);
	}
#endif


#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_END
#endif

	switch( iEventId )
	{
	case ECL_CRASH_INFO_REQ:
		{
#if defined( _SERVICE_ )
			ELSWORD_VIRTUALIZER_START
#endif
			string strId = "";
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
            if ( g_pMain != NULL )
                g_pMain->GetUserIdToFindHackIfNotEmpty_ThreadSafe( strId );
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
			if( g_pMain != NULL &&
				g_pMain->GetUserIdToFindHack().size() > 0 )
			{
				ConvertWCHARToChar( strId, g_pMain->GetUserIdToFindHack() );				
			}
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX

			KECL_CRASH_INFO_REQ kEvent;
			kEvent.m_kCrashInfo.m_sType = usType;
			kEvent.m_kCrashInfo.m_strHackingInfo = strHackInfo;
			kEvent.m_kCrashInfo.m_strUserID = strId;
#ifdef MACHINE_ID
			kEvent.m_kCrashInfo.m_strMachineID = g_pMain->GetMachineId();
#endif
			kEvent.m_kCrashInfo.m_strText = strstmInfo.str();
			kEvent.m_kCrashInfo.m_iServerSN = 0;

#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK	// 빌드 오류로 해외팀 추가
			memcpy(kEvent.m_kCrashInfo.m_charServerSN, g_pInstanceData->GetSN(), sizeof(unsigned char) * SERVER_SN);	
#endif // SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
			kEvent.m_bExit = bExit;

			m_pCollectServer->SendCSPacket( ECL_CRASH_INFO_REQ, kEvent );	

			m_bSendPacket = true;	
#if defined( _SERVICE_ )
			ELSWORD_VIRTUALIZER_END
#endif
		}
		break;
	default:
		break;
	}
}

void CX2CollectHackInfo::ReceiveCSEvent_Thread( COLLECT_SERVER_PACKET &spEvent )
{
	//MessageBox(NULL, CX2ServerEvent::COLLECT_SERVER_EVENT_ID_STR[spEvent.m_iEventID], L"CSEvent!", MB_OK);

	switch( spEvent.m_iEventID )
	{
	
	case ECL_CRASH_INFO_ACK:
		{
			KSerBuffer* pBuff = (KSerBuffer*)spEvent.m_pSerBuff;
			KECL_CRASH_INFO_ACK kEvent;
			DeSerialize( pBuff, &kEvent );

			if( g_pMain != NULL && g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
			{
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
				m_iImageOffset_Thread = 0;
                m_vecHackImgBuf_Thread.resize( 0 );
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
                m_iImageOffset = 0;
				m_lTotalSize = 0;
				SAFE_DELETE_ARRAY(m_pHackImgBuf);
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX

				//if( m_iImageOffset == 0 )
				LoadHackImg_Thread( kEvent.m_strFileName );
                SendCSImg_Thread( kEvent.m_strFileName, kEvent.m_bExit );
				//EnQueue( ECL_CRASH_IMAGE_INFO_REQ, NULL, true, 0,  kEvent.m_strFileName, kEvent.m_bExit );				
			}
		}
		break;
	case ECL_CRASH_IMAGE_INFO_ACK:
		{
			KSerBuffer* pBuff = (KSerBuffer*)spEvent.m_pSerBuff;
			KECL_CRASH_IMAGE_INFO_ACK kEvent;
			DeSerialize( pBuff, &kEvent );

			if( g_pMain != NULL && g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
			{
				//EnQueue( ECL_CRASH_IMAGE_INFO_REQ, NULL, true, 0,  kEvent.m_strFileName, kEvent.m_bExit );
				if( kEvent.m_dwRecvedTotalBuffSize < 
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
                    (DWORD) m_vecHackImgBuf_Thread.size()
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
                    (DWORD)m_lTotalSize 
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
                    )
                {
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
                    SendCSImg_Thread( kEvent.m_strFileName, kEvent.m_bExit );
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
					SendCSImg_Thread( kEvent.m_strFileName, kEvent.m_bExit );
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
                }
			}			
		}
		break;
	default:
		break;
	}

	SAFE_DELETE( spEvent.m_pSerBuff );
}

void CX2CollectHackInfo::DoProcessEvent_Thread()
{	
	if( m_bRunThread == false )
		return;

#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
    if ( ::InterlockedCompareExchange( &m_lCancelSendCSImg_Interlocked, 0L, 1L ) == 1L )
    {
        m_vecHackImgBuf_Thread.resize( 0 );
        m_iImageOffset_Thread = 0;
    }
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX

	::EnterCriticalSection( &m_csEventQueue );  // 050630. 이벤트를 처리하는 동안 unlock 되도록 수정.

	// traverse all event
	COLLECT_SERVER_PACKET spEvent;

	//while( !g_pMain->m_queEvent.empty() )
	if( m_queEvent.empty() == false )	
	{
		// 050912. florist. 050911에 넷마블 테섭이 이곳에서 crash를 일으킴. 별도 예외처리.
		try 
		{ 
			spEvent = m_queEvent.front(); 
		} 
		catch( ... ) 
		{ 
			m_queEvent.pop(); 
			::LeaveCriticalSection( &m_csEventQueue );
			return;
		}

		m_queEvent.pop();

		::LeaveCriticalSection( &m_csEventQueue );


		// 2. call event handler
		if( spEvent.m_bSend == true )
		{
			SendCSEvent_Thread( spEvent.m_iEventID, spEvent.m_usType, spEvent.m_strHackInfo, spEvent.m_bExit );
		}
		else
		{
			ReceiveCSEvent_Thread( spEvent );		
		}
	}
    else
    {
    	::LeaveCriticalSection( &m_csEventQueue );
    }
}

bool CX2CollectHackInfo::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;

	switch( wParam )
	{
	case ECL_CONNECTION_LOST_NOT:
	case ECL_RESERVE_DESTROY:
		{
			SAFE_DELETE( pBuff );
			ClearQueue();
			m_bSendPacket = false;
#ifdef  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
            ::InterlockedExchange( &m_lCancelSendCSImg_Interlocked, 1L );
#else   X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
			m_iImageOffset = 0;
			m_lTotalSize = 0;
			SAFE_DELETE_ARRAY( m_pHackImgBuf );
#endif  X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX

			if( m_pCollectServer != NULL )
				m_pCollectServer->DisconnectFromCollectServer();
			//SAFE_DELETE( m_pCollectServer );		
		}
		break;	
	case COLLECT_EVENTID_SENTINEL:
		{
			SAFE_DELETE( pBuff );
			//m_bSendPacket = false;
		}
		break;
	default:
		{
			EnQueue( (unsigned short)wParam, pBuff, false, 0, "", false );
		}
		break;
	}

	return true;
}

#endif //ADD_COLLECT_CLIENT_INFO
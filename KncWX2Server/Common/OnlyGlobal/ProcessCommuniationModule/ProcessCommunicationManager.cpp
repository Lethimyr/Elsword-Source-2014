#include "ProcessCommunicationManager.h"

// 2013.07.09 lygan_조성욱 // 해외팀 서버 관리툴(KogServerManagementStudio)과 각 서버간에 통신을 위한 작업
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "GameSysVal/GameSysVal.h"

#define BUFSIZE 1024

ImplementSingleton( KProcessCommunicationManager );

KProcessCommunicationManager::KProcessCommunicationManager()
:
 m_hPipeRead (INVALID_HANDLE_VALUE)
{
	
}

void KProcessCommunicationManager::InitRead( LPTSTR lpszPipeName )
{
	m_lpszPipeNameRead = lpszPipeName;

	m_hPipeRead = CreateNamedPipe( 
		m_lpszPipeNameRead,             // pipe name 
		PIPE_ACCESS_DUPLEX,       // read/write access 
		PIPE_TYPE_MESSAGE |       // message type pipe 
		PIPE_READMODE_MESSAGE |   // message-read mode 
		PIPE_WAIT,                // blocking mode 
		PIPE_UNLIMITED_INSTANCES, // max. instances  
		BUFSIZE,                  // output buffer size 
		BUFSIZE,                  // input buffer size 
		NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
		NULL);  

	if (m_spThreadRead)
	{
		m_spThreadRead->Begin();
	}

}

void KProcessCommunicationManager::InitWrite( LPTSTR lpszPipeName )
{
	m_lpszPipeNameWrite = lpszPipeName;

	m_hPipeWrite = CreateNamedPipe( 
		m_lpszPipeNameWrite,             // pipe name 
		PIPE_ACCESS_DUPLEX,       // read/write access 
		PIPE_TYPE_MESSAGE |       // message type pipe 
		PIPE_READMODE_MESSAGE |   // message-read mode 
		PIPE_WAIT,                // blocking mode 
		PIPE_UNLIMITED_INSTANCES, // max. instances  
		BUFSIZE,                  // output buffer size 
		BUFSIZE,                  // input buffer size 
		NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
		NULL);  


	// 2013.07.10 lygan_조성욱 // Agent가 연결 대기 상태면 위에 CreateNamedPipe 함수 실행과 동시에 연결된다. 그래서 Write 쓰래드 바로 생성 부터 우선 해주자
	m_spThreadWrite = boost::shared_ptr< KTThread< KProcessCommunicationManager > >( new KTThread< KProcessCommunicationManager >( *this, &KProcessCommunicationManager::WriteData, 100 ) );

	if (m_spThreadWrite)
	{
		m_spThreadWrite->Begin();
	}



}


bool KProcessCommunicationManager::Begin( void )
{
	return KThread::Begin();
}

void KProcessCommunicationManager::ShutDown( void )
{

	{
		KLocker lock( m_csPipeNameRead );
		if (m_spThreadRead != NULL)
		{
			m_spThreadRead->End(3000);
		}
	}
	
	{
		KLocker lock( m_csPipeNameWrite );
		if (m_spThreadWrite != NULL)
		{
			m_spThreadWrite->End(3000);	
		}
	}
	

	

	KThread::End();
}

void KProcessCommunicationManager::Run( void )
{
	START_LOG( clog, L"Enter Run()" );

	DWORD ret;

	while( SiKGameSysVal()->GetProcessCommunication() )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else    START_LOG( cout, L"*** WaitForSingleObject() - return : " << ret );
	}

	ShutDown();
}

void KProcessCommunicationManager::Tick( void )
{

	if (m_hPipeRead != INVALID_HANDLE_VALUE)
	{
	
		//KLocker lock( m_csPipeNameRead );
		bool bConnectedRead = false;

		bConnectedRead = ConnectNamedPipe(m_hPipeRead, NULL);
		
		KLocker lock( m_csPipeNameRead );
		if ( bConnectedRead == true )
		{
			if ( m_spThreadRead == NULL )
			{
				m_spThreadRead = boost::shared_ptr< KTThread< KProcessCommunicationManager > >( new KTThread< KProcessCommunicationManager >( *this, &KProcessCommunicationManager::ReadTickData, 100 ) );

				if (m_spThreadRead)
				{
					m_spThreadRead->Begin();
				}
			}
		}
	}
	else
	{
		KLocker lock( m_csPipeNameRead );

		m_hPipeRead = CreateNamedPipe( 
			m_lpszPipeNameRead,             // pipe name 
			PIPE_ACCESS_DUPLEX,       // read/write access 
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			PIPE_UNLIMITED_INSTANCES, // max. instances  
			BUFSIZE,                  // output buffer size 
			BUFSIZE,                  // input buffer size 
			NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
			NULL);    
	}

}

void KProcessCommunicationManager::ReadTickData(void)
{
	TCHAR chRequest[BUFSIZE]; 
	DWORD cbBytesRead, cbReplyBytes, cbWritten; 
	BOOL bSuccess; 

	//START_LOG( cout, L"ReadTickData 시작" );

	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		bSuccess = ReadFile( 
			m_hPipeRead,        // handle to pipe 
			chRequest,    // buffer to receive data 
			BUFSIZE*sizeof(TCHAR), // size of buffer 
			&cbBytesRead, // number of bytes read 
			NULL);     
		if ( bSuccess == true )
		{
			KLocker lock( m_csPipeNameRead );

			std::string strReadData((char *)chRequest);

			luaL_dostring( g_pLua, strReadData.c_str() );

		}
		else
		{
			KLocker lock( m_csPipeNameRead );

			FlushFileBuffers(m_hPipeRead); 
			DisconnectNamedPipe(m_hPipeRead); 
			CloseHandle(m_hPipeRead); 
			m_hPipeRead = INVALID_HANDLE_VALUE;

			m_hPipeRead = CreateNamedPipe( 
				m_lpszPipeNameRead,             // pipe name 
				PIPE_ACCESS_DUPLEX,       // read/write access 
				PIPE_TYPE_MESSAGE |       // message type pipe 
				PIPE_READMODE_MESSAGE |   // message-read mode 
				PIPE_WAIT,                // blocking mode 
				PIPE_UNLIMITED_INSTANCES, // max. instances  
				BUFSIZE,                  // output buffer size 
				BUFSIZE,                  // input buffer size 
				NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
				NULL);  
		}
	}
	else
	{
		KLocker lock( m_csPipeNameRead );
		FlushFileBuffers(m_hPipeRead); 
		DisconnectNamedPipe(m_hPipeRead); 
		CloseHandle(m_hPipeRead); 
		m_hPipeRead = INVALID_HANDLE_VALUE;
	}

	

}

void KProcessCommunicationManager::WriteData(void)
{

	TCHAR chRequest[BUFSIZE]; 
	WCHAR wchReply[BUFSIZE]; 
	DWORD cbBytesRead, cbReplyBytes, cbWritten; 
	BOOL bSuccess; 
	std::wstring wstrWriteData =L"";

	//START_LOG( cout, L"WriteData 시작" );

	if ( m_hPipeWrite != INVALID_HANDLE_VALUE && SiKGameSysVal()->GetProcessCommunication() == true)
	{

		if (GetQueuingWriteData( wchReply, cbReplyBytes) == true)
		{

			bSuccess = WriteFile( 
				m_hPipeWrite,        // handle to pipe 
				wchReply,      // buffer to write from 
				cbReplyBytes, // number of bytes to write 
				&cbWritten,   // number of bytes written 
				NULL);        // not overlapped I/O 

			if ( bSuccess == true )
			{
				START_LOG( cout, L"Agent WriteData 완료" );
			}
			else
			{
				{
					KLocker lock( m_csPipeNameWrite );

					FlushFileBuffers(m_hPipeWrite); 
					DisconnectNamedPipe(m_hPipeWrite); 
					CloseHandle(m_hPipeWrite); 
					m_hPipeWrite = INVALID_HANDLE_VALUE;

					m_hPipeWrite = CreateNamedPipe( 
						m_lpszPipeNameWrite,             // pipe name 
						PIPE_ACCESS_DUPLEX,       // read/write access 
						PIPE_TYPE_MESSAGE |       // message type pipe 
						PIPE_READMODE_MESSAGE |   // message-read mode 
						PIPE_WAIT,                // blocking mode 
						PIPE_UNLIMITED_INSTANCES, // max. instances  
						BUFSIZE,                  // output buffer size 
						BUFSIZE,                  // input buffer size 
						NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
						NULL);  

				}
				


				bool bConnectedWrite = false;

				bConnectedWrite = ConnectNamedPipe(m_hPipeWrite, NULL);

				{
					KLocker lock( m_csPipeNameWrite );

					if ( bConnectedWrite == true)
					{
						bSuccess = WriteFile( 
							m_hPipeWrite,        // handle to pipe 
							wchReply,      // buffer to write from 
							cbReplyBytes, // number of bytes to write 
							&cbWritten,   // number of bytes written 
							NULL);        // not overlapped I/O 
						if ( bSuccess == true )
						{
							START_LOG( cout, L"WriteData 완료" );
						}
					}
				}
				
			}
		}
	}
	else
	{
		KLocker lock( m_csPipeNameWrite );
		FlushFileBuffers(m_hPipeWrite); 
		DisconnectNamedPipe(m_hPipeWrite); 
		CloseHandle(m_hPipeWrite); 
		m_hPipeWrite = INVALID_HANDLE_VALUE;
	}
}

void KProcessCommunicationManager::QueueingProcessWrite(std::wstring _wstrQueueing)
{
//////////////////////////////////////////////////// 2013.07.17 lygan_조성욱 // 서버에서 Agent로 문자열 데이터 넘길때 규칙
//서버관리툴의 Agent로 갈 문자열 앞에 어떤 타입인지 구분 지어 주자
// 0 번은 실시간 서버 패치 관련 메시지 알림
// 1 번은 이벤트 등록에 관련된 메시지 알림
// 2 번은 이벤트 시작과 종료에 관련된 메시지 알림
////////////////////////////////////////////////////

////////////////////////////////////////////////////
//1번과 2번은 이벤트 동작의 경우 
//타입, 이벤트UID, 이벤트이름, 이벤트타입, 이벤트스크립트, 이벤트 시작, 이벤트 종료, 이벤트 동작 유무 이렇게 구성해준다.
////////////////////////////////////////////////////

	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		KLocker lock( m_csPipeNameWrite );

		if (m_queWriteMessage.size() > 1000 ) // 2013.07.17 lygan_조성욱 // QueSize 가 여기까지 올라갔다는건 Agent와 서버간 연결이 끊어졌다는거다. QueSize 오버로 서버 죽기 전에 Que 에 다 쌓인거 뺀다.
		{
			while (!m_queWriteMessage.empty())
			{
				m_queWriteMessage.pop();
			}

		}

		START_LOG( cout, L"WriteData Queueing 추가" )
			<< BUILD_LOG( _wstrQueueing );

		m_queWriteMessage.push(_wstrQueueing);

	}
}

bool KProcessCommunicationManager::GetQueuingWriteData(WCHAR *wchReply, DWORD  &cbReplyBytes)
{
	KLocker lock( m_csPipeNameWrite );
	
	if (SiKGameSysVal()->GetProcessCommunication() == false)
	{
		return false;
	}


	if ( m_queWriteMessage.empty() == true )
	{
		return false;
	}

	std::wstring tempwString = m_queWriteMessage.front();
	if (tempwString.length() == 0)
	{
		return false;
	}

	wchReply[0] = '\0';
	std::wstring tempwStringwchEnd = L"\n";

	StrCatW(wchReply, tempwString.c_str());
	StrCatW(wchReply, tempwStringwchEnd.c_str());

	//cbReplyBytes = ( strlen(chReply)*sizeof(WCHAR) );
	cbReplyBytes = ( wcslen(wchReply)*sizeof(WCHAR) );


	m_queWriteMessage.pop();

	return true;

}

#endif SERV_PROCESS_COMMUNICATION_KSMS

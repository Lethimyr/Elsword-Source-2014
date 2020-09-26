#include "StdAfx.h"
#include ".\ktdxthread.h"
//
//CKTDXThread::CKTDXThread(void)
//{
//	m_Handle	= NULL;
//	m_ThreadID	= 0;
//	m_bRun		= false;
//}
//
//CKTDXThread::~CKTDXThread(void)
//{
//	End();
//}
//
//void CKTDXThread::Begin( int priolity )
//{
//	m_Handle = (HANDLE)_beginthreadex( (void*)NULL,
//										(unsigned)0,
//										HandleRunner,
//										this,
//										(unsigned)0,
//										(unsigned*)m_ThreadID
//										);
//
//	SetThreadPriority( m_Handle, priolity );
//}
//
//void CKTDXThread::End()
//{
//	m_bRun = false;
//
//	DWORD exitCode;
//	if( GetExitCodeThread( m_Handle, &exitCode ) == TRUE )
//	{
//		while( exitCode == STILL_ACTIVE )
//		{
//			Sleep( 100 );
//			GetExitCodeThread( m_Handle, &exitCode );
//		}
//
//		CloseHandle( m_Handle );
//		m_Handle = NULL;
//	}
//}
//
//
//unsigned int WINAPI CKTDXThread::HandleRunner( LPVOID parameter )
//{
//	CKTDXThread* thread = reinterpret_cast<CKTDXThread*>(parameter);
//
//	thread->Run();
//
//	return 0;
//}
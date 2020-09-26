#pragma once
#include <process.h>

#ifndef _BEGINTHREADEX 
typedef unsigned (__stdcall* PTHREAD_START)(void*);
#   define _BEGINTHREADEX( psa, \
	cbStack, \
	pfnStartAddr, \
	pvParam, \
	fdwCreate, \
	pdwThreadId) \
	((HANDLE)_beginthreadex( (void*) (psa), \
	(unsigned) (cbStack), \
	(PTHREAD_START) (pfnStartAddr), \
	(void*) (pvParam), \
	(unsigned) (fdwCreate), \
	(unsigned*) (pdwThreadId)))
#endif

class KSimpleThreadManager;
class KSimpleThread
{
public:
	KSimpleThread(void);
	virtual ~KSimpleThread(void);

	virtual bool Begin()                        // _BEGINTHREADEX 호출. Run()이 호출되는 시점.
	{
		if( m_hThread != NULL )
			return false;

		m_hKillEvent = CreateEvent(NULL, false, false, NULL);
		if( NULL == m_hKillEvent )
			return false;

		DWORD dwThreadId;
		m_hThread = _BEGINTHREADEX( NULL,
			0,
			ThreadProc,
			this,
			0,
			&dwThreadId );
		if( NULL == m_hThread )
		{
			::CloseHandle( m_hKillEvent);
			m_hKillEvent = NULL;
			return false;
		} // if

		return true;
	}
	virtual void End(DWORD dwTimeout = 3000)    // 스레드의 종료를 요청하고 이를 기다렸다가 릴리즈
	{
		if( NULL != m_hThread )
		{
			SetEvent( m_hKillEvent );
			::WaitForSingleObject( m_hThread, dwTimeout );
			::CloseHandle( m_hThread );
			m_hThread = NULL;
		} // if
	}

	virtual void Run() = 0;

	void SetThreadManager( KSimpleThreadManager* pSimpleThreadManager ) { m_pSimpleThreadManager = pSimpleThreadManager; }

protected:
	static unsigned int WINAPI ThreadProc(LPVOID pvParam)
	{
		KSimpleThread* thread = reinterpret_cast<KSimpleThread*>(pvParam);
		thread->Run();
		return true;
	}

protected:
	HANDLE          m_hKillEvent;
	HANDLE          m_hThread;
	KSimpleThreadManager* m_pSimpleThreadManager;
};



class KIOThread : public KSimpleThread
{
public:
	KIOThread(void);
	virtual ~KIOThread(void);

protected:
	virtual void Run();
	void Loop();
};

#pragma once
#include <thread/Thread.h>
#include <atltime.h>
#include "ThreadManager.h"

#include "Event.h"

// 가장 일반적인 경우의 loop, dispatch가 구현되어 있다. 
// 이벤트 아이디에 대한 핸들러 함수만 구현하고자 한다면 
// KSubThread에서 상속받고 핸들러만 추가한다.
class KTRSubThread : public KThread
{
protected:  // 직접 객체를 생성하지는 않는다.
	KTRSubThread() {}
public:
	virtual ~KTRSubThread(void) {}

protected:
	inline void Loop()
	{
		if( !m_pkThreadMgr->GetKEventAll( m_EventQueue ) )
		{
			std::cout << "Sub-Thread Terminated." << std::endl;
			SetEvent( m_hKillEvent );
			return;
		}

		//DWORD dwElapTime = ::GetTickCount();  // 040428. 처리 소요시간을 구한다.

		KEventPtr spEvent;

		while( !m_EventQueue.empty() )
		{
			spEvent = m_EventQueue.front();
			m_EventQueue.pop();

			ProcessEvent( spEvent );
		}

		//dwElapTime = ::GetTickCount() - dwElapTime;

		//m_pkThreadMgr->CheckMaxProcessingInfo( dwElapTime, spEvent );
	}

	virtual void Run()     // derived from KThread
	{
		std::cout << "KSubThread::Run(), thread start." << std::endl;

		DWORD ret;

		while( true )
		{
			ret = ::WaitForSingleObject( m_hKillEvent, 1 );

			if( ret == WAIT_OBJECT_0 ) break;

			if( ret == WAIT_TIMEOUT ) Loop();

			else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

		}
	}

	virtual void ProcessEvent( const KEventPtr& spEvent )   = 0;

private:
	std::queue< KEventPtr >                     m_EventQueue;
};




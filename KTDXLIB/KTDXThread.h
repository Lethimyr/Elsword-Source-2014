#pragma once


// dmlee 2008.05.14
// obsolete: 지금은 사용하고 있지 않는 코드 입니다. KJohnThread 를 사용해주세요.
//


#pragma once

//typedef unsigned (__stdcall* PTHREAD_START)(void*);
//
//class CKTDXThread
//{
//	public:
//		class CLocker
//		{
//			public:
//				CLocker(CRITICAL_SECTION& cs ) : m_pcs( &cs )
//				{
//					Lock();
//				}
//				CLocker(CRITICAL_SECTION* pCs ) : m_pcs( pCs )
//				{
//					Lock();
//				}
//
//				~CLocker() 
//				{ 
//					if( m_pcs )
//						Unlock(); 
//				}
//				void Lock()
//				{
//					EnterCriticalSection( m_pcs );
//				}
//				void Unlock()
//				{
//					LeaveCriticalSection( m_pcs );
//					m_pcs = NULL;
//				}
//			protected:
//				CRITICAL_SECTION*   m_pcs;
//		};
//
//	public:
//		CKTDXThread(void);
//		virtual ~CKTDXThread(void);
//		
//		void							Begin( int priolity = THREAD_PRIORITY_NORMAL );
//		void							End();
//
//		virtual	void					Run() = NULL;
//
//		HANDLE							GetThreadHandle(){ return m_Handle; }
//
//	protected:
//		static unsigned int WINAPI		HandleRunner( LPVOID parameter );
//
//		HANDLE							m_Handle;
//		DWORD							m_ThreadID;
//		bool							m_bRun;
//};

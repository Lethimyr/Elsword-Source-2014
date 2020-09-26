#include "StdAfx.h"
#include <boost/bind.hpp>
#include "SimpleLocker.h"
#include "KSimpleThread.h"
#include ".\ksimplethreadmanager.h"

KSimpleThreadManager::KSimpleThreadManager(void)
{
	InitializeCriticalSection( &m_csVecThread );
}

KSimpleThreadManager::~KSimpleThreadManager(void)
{
	DeleteCriticalSection( &m_csVecThread );

	std::vector< KSimpleThread* >::iterator vit = m_vecThread.begin();
	for( ; vit != m_vecThread.end(); vit++ )
	{		
		KSimpleThread* pST = (*vit);
		pST->End();
		delete pST;
	}
}

void KSimpleThreadManager::Init( int nThreadNum_ )
{
	// create I/O thread
	KSimpleLocker lock( &m_csVecThread );

	for( int i = 0; i < nThreadNum_; i++ )
	{
		KSimpleThread* pThread = CreateThread();
		pThread->SetThreadManager( this );
		m_vecThread.push_back( pThread );
	}
}

void KSimpleThreadManager::BeginThread()
{
	KSimpleLocker lock( &m_csVecThread );

	std::for_each( m_vecThread.begin(), m_vecThread.end(), boost::bind( KSimpleThread::Begin, _1 ) );
}

void KSimpleThreadManager::EndThread()
{
	KSimpleLocker lock( &m_csVecThread );

	if( m_vecThread.empty() )
		return;

	m_bTerminateReserved = true;

	//START_LOG( cout, L"모든 스레드가 종료되도록 대기합니다." );

	DWORD dwWaitTick = ::GetTickCount();
	while( m_nTerminateCount < m_vecThread.size() && ::GetTickCount() - dwWaitTick < 10000 )
	{
		::Sleep( 20 );
	}

	//START_LOG( cout, L"대기 후 남은 스레드 수 : " << m_vecThread.size() - m_nTerminateCount )
	//	<< BUILD_LOG( m_vecThread.size() )
	//	<< BUILD_LOG( m_nTerminateCount );
	
	m_vecThread.clear();
}
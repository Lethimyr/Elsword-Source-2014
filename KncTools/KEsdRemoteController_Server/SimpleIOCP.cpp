#include "StdAfx.h"
#include "KTCPSocket.h"
#include "SimpleLocker.h"
#include "Overlapped.h"
#include "KSimpleThread.h"
#include ".\simpleiocp.h"

ImplementSingleton( KSimpleIOCP );

KSimpleIOCP::KSimpleIOCP(void)
: m_hIocp( NULL )
{
	m_hIocp = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	InitializeCriticalSection( &m_csSockObj );
}

KSimpleIOCP::~KSimpleIOCP(void)
{
	if( m_hIocp != NULL ) 
		CloseHandle( m_hIocp );

	DeleteCriticalSection( &m_csSockObj );

	std::map< DWORD, KTCPSocket* >::iterator mit = m_mapSockObj.begin();
	for( ; mit != m_mapSockObj.end(); mit++ )
	{
		delete (mit->second);
	}
	m_mapSockObj.clear();
}

bool KSimpleIOCP::AssociateSocket( KTCPSocket* pkSockObj )
{
	if( pkSockObj == NULL ) return false;

	KSimpleLocker lock( &m_csSockObj );

	DWORD dwCompKey = GenNewCompKey();

	if( AssociateSocket( pkSockObj->GetSocket(), dwCompKey ) )
	{
		m_mapSockObj.insert( std::make_pair( dwCompKey, pkSockObj ) );
		pkSockObj->SetKey( dwCompKey );
		return true;
	}

	return false;
}

bool KSimpleIOCP::AssociateSocket( SOCKET hSocket, ULONG_PTR CompKey )
{
	return ( CreateIoCompletionPort( (HANDLE) hSocket, m_hIocp, CompKey, 0 ) != NULL );
}

bool KSimpleIOCP::PostStatus( ULONG_PTR CompKey, 
					   DWORD dwNumBytes /* = 0 */, 
					   OVERLAPPED* po /* = NULL */ )
{
	return (PostQueuedCompletionStatus(m_hIocp, dwNumBytes, CompKey, po) == TRUE);
}

bool KSimpleIOCP::GetStatus( ULONG_PTR* pCompKey,
					  PDWORD pdwNumBytes,
					  OVERLAPPED** ppo,
					  DWORD dwMilliseconds /* = INFINITE */)
{
	return (GetQueuedCompletionStatus(m_hIocp, 
		pdwNumBytes, 
		pCompKey, 
		ppo, 
		dwMilliseconds) == TRUE);
}

#undef max
#include <boost/random.hpp>

DWORD KSimpleIOCP::GenNewCompKey()
{
	static boost::mt19937 rng;
	static boost::uniform_int<DWORD> uint32( 1, UINT_MAX );
	static boost::variate_generator<boost::mt19937&, boost::uniform_int<DWORD> > die(rng, uint32);

	KSimpleLocker lock( &m_csSockObj );

	//DWORD dwStart = GetTickCount();
	for(;;)
	{
		DWORD dwCompKey = die();

		if( m_mapSockObj.find( dwCompKey ) == m_mapSockObj.end() )
			return dwCompKey;
	}

	//DWORD dwTerm  = GetTickCount() - dwStart;
	//if( dwTerm > m_dwTerm )
	//{
	//	KLocker lock( KSimpleIOCP::m_cs );
	//	m_dwTerm         = dwTerm;
	//	m_cTime          = CTime::GetCurrentTime();
	//	m_dwSockObjNum   = m_mapSockObj.size();
	//}
}

void KSimpleIOCP::DeleteCompKey( KTCPSocket* pkSockObj_ )
{
	KSimpleLocker lock( &m_csSockObj );

	m_mapSockObj.erase( pkSockObj_->GetKey() );

	pkSockObj_->SetKey( 0 );
}

KSimpleThread* KSimpleIOCP::CreateThread()
{
	return new KIOThread;
}

void KSimpleIOCP::EndThread()
{
	std::vector< KSimpleThread* >::iterator vit;
	for( vit = m_vecThread.begin(); vit != m_vecThread.end(); vit++ )
	{
		PostStatus( SIGNAL_SHUTDOWN_SERVER );
		//(*vit)->End();
	}
}

void KSimpleIOCP::OnClosedByLocal( IN DWORD dwKey_ )
{
	KTCPSocket* pkObj = GetSockObject( dwKey_ );

	if( pkObj )
	{
		pkObj->CloseSocket();
		delete pkObj;
		m_mapSockObj.erase( dwKey_ );
	}
}

KTCPSocket* KSimpleIOCP::GetSockObject( DWORD dwKey_ )
{
	KSimpleLocker lock( &m_csSockObj );

	std::map<DWORD,KTCPSocket*>::iterator mit;

	mit = m_mapSockObj.find( dwKey_ );

	if( mit == m_mapSockObj.end() )
	{
		//START_LOG( clog2, "요청한 소켓의 포인터를 찾을 수 없음. Key : " << dwKey_ )
		//	<< BUILD_LOG( m_mapSockObj.size() )
		//	<< END_LOG;
		return NULL;
	}

	return mit->second;
}

void KSimpleIOCP::OnIOCompleted( IN DWORD dwKey_, IN DWORD dwBytesTransfer_, IN OVERLAPPED* povl_ )
{
	if( povl_ == NULL )
	{
		//START_LOG( cerr, L"IOCP Error" );
		return;
	}

	KOVERLAPPED* pkOvl = static_cast<KOVERLAPPED*>( povl_ );

	KTCPSocket* pkObj = GetSockObject( dwKey_ );

	if( pkObj == NULL )
	{
		//START_LOG( cwarn, "IO가 종료된 소켓의 포인터를 찾을 수 없음. Key : " << dwKey_ )
		//	<< BUILD_LOG( dwBytesTransfer_ )
		//	<< END_LOG;
		return;
	}

	// 종료처리를 여기서 하자!
	if( dwBytesTransfer_ == 0 )
	{
		pkObj->CloseSocket();
		delete pkObj;
		m_mapSockObj.erase( dwKey_ );
		return;
	}

	switch( pkOvl->m_eIOMode ) {
		case KOVERLAPPED::IO_RECV:
			if( &pkObj->m_ovlRecv != pkOvl )
			{
				//START_LOG( cerr, "diffrent CompKey. (recv) key : " << dwKey_ );
			}
			pkObj->OnRecvCompleted( dwBytesTransfer_ );
			break;
		case KOVERLAPPED::IO_SEND:
			if( &pkObj->m_ovlSend != pkOvl )
			{
				//START_LOG( cerr, "diffrent CompKey. (send) key : " << dwKey_ );
			}
			pkObj->OnSendCompleted( dwBytesTransfer_ );
			break;
	}
}

void KSimpleIOCP::BroadCastPacket( KPacket& kPacket )
{
	KSimpleLocker lock( &m_csSockObj );

	std::map< DWORD, KTCPSocket* >::iterator mit = m_mapSockObj.begin();
	for( ; mit != m_mapSockObj.end(); mit++ )
	{
		KTCPSocket* pSockObj = mit->second;
		if( pSockObj == NULL )
			continue;

		pSockObj->Send( kPacket.GetBuffer(), kPacket.GetSize() );
	}
}
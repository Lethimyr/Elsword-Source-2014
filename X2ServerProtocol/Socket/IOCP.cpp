#include "StdAfx.h"

//#include "SocketObject.h"
#include "IOCP.h"
//#include "dbg/dbg.hpp"
//#include "IOThread.h"
//#include <boost/bind.hpp>

ImplementSingleton( KIocp );
NiImplementRTTI( KIocp, KThreadManager );

KIocp::KIocp() 
: 
    m_hIOCP( NULL ),
    m_dwTerm(0L),
m_dwSockObjNum(0L)
{ 
    m_cTime = CTime::GetCurrentTime();   
    m_hIOCP = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

    LIF( m_hIOCP != NULL );
}

KIocp::~KIocp()
{ 
    if( m_hIOCP != NULL ) 
        ::CloseHandle( m_hIOCP );
}

void KIocp::EndThread()
{
    std::vector< boost::shared_ptr<KThread> >::iterator vit;
    for( vit = m_vecThread.begin(); vit != m_vecThread.end(); vit++ )
    {
        PostStatus( SIGNAL_SHUTDOWN_SERVER );
        //(*vit)->End();
    }
}

bool KIocp::AssociateSocket( KSocketObjectPtr spSockObj )
{
    _JIF( spSockObj, return false );

    DWORD dwCompKey = GenNewCompKey();

    SOCKET& sock = spSockObj->GetSocket();

    KLocker lock( m_csSockObj );

    if( AssociateSocket( sock, dwCompKey ) )
    {
        m_mapSockObj.insert( std::make_pair( dwCompKey, spSockObj ) );
        spSockObj->SetKey( dwCompKey );
        return true;
    }

    return false;
}

bool KIocp::AssociateSocket( SOCKET hSocket, ULONG_PTR CompKey )
{
    return ( ::CreateIoCompletionPort( ( HANDLE ) hSocket, m_hIOCP, CompKey, 0 ) != NULL );
}

bool KIocp::PostStatus( ULONG_PTR CompKey, 
                       DWORD dwNumBytes /* = 0 */, 
                       OVERLAPPED* po /* = NULL */ )
{
    return ( ::PostQueuedCompletionStatus( m_hIOCP, dwNumBytes, CompKey, po ) != 0 );
}

bool KIocp::GetStatus( ULONG_PTR* pCompKey,
                      PDWORD pdwNumBytes,
                      OVERLAPPED** ppo,
                      DWORD dwMilliseconds /* = INFINITE */)
{
    return ( ::GetQueuedCompletionStatus( m_hIOCP, 
        pdwNumBytes, 
        pCompKey, 
        ppo, 
        dwMilliseconds ) != 0 );
}

bool KIocp::DeleteCompKey( IN DWORD dwKey_ )
{
    KLocker lock( m_csSockObj );
    std::map< DWORD, KSocketObjectPtr >::iterator mit = m_mapSockObj.find( dwKey_ );
    if( mit == m_mapSockObj.end() )
    {
        START_LOG( cerr, L"지우려는 completion key에 대한 세션이 없음." )
            << BUILD_LOG( dwKey_ )
            << END_LOG;

        return false;
    }

    if( !mit->second )
    {
        START_LOG( cerr, L"IOCP에 등록된 소켓 오브젝트 포인터가 이상함." )
            << BUILD_LOG( mit->first )
            << END_LOG;
    }
    else
    {
        mit->second->SetKey( 0 );
    }

    m_mapSockObj.erase( mit );
    return true;
}

void KIocp::OnIOCompleted( IN DWORD dwKey_, IN DWORD dwBytesTransfer_, IN OVERLAPPED* povl_ )
{
    if( povl_ == NULL )
    {
        START_LOG( cerr, L"IOCP Error" );
        return;
    }

    KOVERLAPPED* pkOvl = static_cast<KOVERLAPPED*>( povl_ );

    KSocketObjectPtr spSockObj = GetSockObj( dwKey_ );

    if( !spSockObj )
    {
        START_LOG( cwarn, "IO가 종료된 소켓의 세션 포인터를 찾을 수 없음. Key : " << dwKey_ )
            << BUILD_LOG( dwBytesTransfer_ )
            << END_LOG;
        return;
    }

    switch( pkOvl->m_eIOMode ) {
        case KOVERLAPPED::IO_RECV: 
            if( &spSockObj->m_ovlRecv != pkOvl )
            {
                START_LOG( cerr, "diffrent CompKey. (recv) key : " << dwKey_ );
            }
            spSockObj->OnRecvCompleted( dwBytesTransfer_ ); 
            break;
        case KOVERLAPPED::IO_SEND:
            if( &spSockObj->m_ovlSend != pkOvl )
            {
                START_LOG( cerr, "diffrent CompKey. (send) key : " << dwKey_ );
            }
            spSockObj->OnSendCompleted( dwBytesTransfer_ );
            break;
    }
}

void KIocp::OnClosedByLocal( IN DWORD dwKey_ )
{
    KSocketObjectPtr spSockObj = GetSockObj( dwKey_ );

    if( spSockObj )
    {
        //spSockObj->SetDisconnectReason( KStatistics::eSIColDR_IOCP_GetStatus );
        spSockObj->OnSocketError();
    }
    else
    {
        START_LOG( clog, L"소켓 오브젝트 없음. 세션 객체 소멸되었음." )
            << BUILD_LOG( dwKey_ )
            << END_LOG;
    }
}

size_t KIocp::GetSockObjNum() const
{ 
    KLocker lock( m_csSockObj ); 
    return m_mapSockObj.size(); 
}

KSocketObjectPtr KIocp::GetSockObj( DWORD dwKey_ )
{
    KLocker lock( m_csSockObj );

    std::map< DWORD, KSocketObjectPtr >::iterator mit;

    mit = m_mapSockObj.find( dwKey_ );

    if( mit == m_mapSockObj.end() )
    {
        START_LOG( clog2, "요청한 소켓의 포인터를 찾을 수 없음. Key : " << dwKey_ )
            << BUILD_LOG( m_mapSockObj.size() )
            << END_LOG;
        return KSocketObjectPtr();
    }

    return mit->second;
}

KThread* KIocp::CreateThread()
{
    return new KIOThread;
}

#undef max
//#include <boost/random.hpp>

DWORD KIocp::GenNewCompKey() const
{
    //static boost::mt19937 rng;
    //static boost::uniform_int<DWORD> uint32( 1, UINT_MAX );
    //static boost::variate_generator<boost::mt19937&, boost::uniform_int<DWORD> > die(rng, uint32);

    KLocker lock( m_csSockObj );

    DWORD dwStart = GetTickCount();
    for(;;)
    {
        DWORD dwCompKey = (rand() % RAND_MAX);//die();

        if( m_mapSockObj.find( dwCompKey ) == m_mapSockObj.end() )
            return dwCompKey;
    }
    DWORD dwTerm  = GetTickCount() - dwStart;

    
    if( dwTerm > m_dwTerm )
    {
        KLocker lock( KIocp::m_cs );
        m_dwTerm         = dwTerm;
        m_cTime          = CTime::GetCurrentTime();
        m_dwSockObjNum   = m_mapSockObj.size();
    }
}

ImplToStringW( KIocp )
{
    START_TOSTRING_PARENTW( KThreadManager );

    size_t sockobjNum = GetSockObjNum();

    KLocker lock( m_cs );
    return stm_ << L" -- Max IOCP INFO --" << std::endl
        << L"  Current SockObj NUM : " << sockobjNum << std::endl
        << L"  Key Gen Term        : " << KIocp::m_dwTerm << std::endl
        << L"  Occured Time        : " << ( const wchar_t* )KIocp::m_cTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) << std::endl
        << L"  socket object NUM   : " << KIocp::m_dwSockObjNum << std::endl;
}
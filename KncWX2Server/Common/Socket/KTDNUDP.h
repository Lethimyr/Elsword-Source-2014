#pragma once



#include "ServerDefine.h"
#include <Winsock2.h>
#include <windows.h>
#include <boost/timer.hpp>
#include <Thread/Locker.h>

#include <string>
using std::wstring;
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <deque>
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#include <list>
using std::list;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#define ZLIB_WINAPI
#include <Serializer/zlib/zlib.h>

#pragma warning( push )
#pragma warning( disable : 4200 )

#define KTDXPROFILE()
#define KTDXPROFILE2(name)
#define KTDXPROFILE_BEGIN(name)
#define KTDXPROFILE_END()

#define RECV_BUFFER_SIZE 2048
#define CKTDNUDP_MTU_MAX        508

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#endif 

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifndef SAFE_FREE
#define SAFE_FREE(p)        { if(p) { ::free(p); (p)=NULL; } }
#endif
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

class CKTDNUDP
{
public:

	// 서버와 클라의 것이 일치해야 함.
	enum SYSTEM_PACKET
	{
		SP_CONNECT_TEST_REQ				= 1,
		SP_CONNECT_TEST_ACK				= 2,
		SP_DEFENCE_PORT					= 3,
//}}
//{{ 2013. 1. 16	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
		SP_RETRY_INTERNAL_CONNECT_REQ	= 4,
		SP_RETRY_INTERNAL_CONNECT_ACK	= 5,
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//}}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		SP_CONNECT_RELAY_REQ			= 6,
		SP_CONNECT_RELAY_ACK			= 7,
        SP_RELAY_SET_UID_LIST_REQ       = 8,
        SP_RELAY_SET_UID_LIST_ACK       = 9,
        SP_RELAY_PRECONFIG              = 10,
		SP_RELAY						= 11,
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef UDP_DOWNLOAD_BLOCK_CHECK
		SP_CONNECT_CHECK_REQ			= 12,
		SP_CONNECT_CHECK_ACK			= 13,
//#endif //UDP_DOWNLOAD_BLOCK_CHECK
//{{ 2012. 10. 31	박세훈	Merge 랜선랙 방지 작업-릴레이를 기본적으로 사용한다.
//#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
		SP_HEART_BEAT_REQ				= 14,
		SP_HEART_BEAT_ACK				= 15,
//#endif SERV_FIX_SYNC_PACKET_USING_RELAY

		//{{ 2013. 2. 5	박세훈	랜선렉 방지 코드2
//#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
		//SP_MORNITORING_NOT				= 16,
//#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

		SP_END,
	};
	//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	// recv info
	enum
	{
		IUS_PORT		= 9151,
	};

	enum LB_EVENT_ID
	{
		LEI_NONE				= 0,
		LEI_CHECK_NOT			= 1,
		LEI_CHECK_CANCLE_NOT	= 2,

		LEI_CONFIRM_NOT			= 3,

		LEI_VERIFY_NOT			= 4,
		LEI_VERIFY_CANCLE_NOT	= 5,
	};

	struct LB_INFO_STRUCT
	{
		LB_EVENT_ID	m_eEventID;
		__int64	m_iRoomUID;
		__int64	m_iUnitUID;

		LB_INFO_STRUCT()
			: m_eEventID( LEI_NONE )
			, m_iRoomUID( 0 )
			, m_iUnitUID( 0 )
		{
		}

		LB_INFO_STRUCT( LB_EVENT_ID eEventID, __int64 iRoomUID, __int64 iUnitUID )
			: m_eEventID( eEventID )
			, m_iRoomUID( iRoomUID )
			, m_iUnitUID( iUnitUID )
		{
		}
	};

	struct LB_INFO_STRUCT_CHECKER
	{
		__int64			m_iRoomUID;
		//boost::timer	m_kTimer;
        //boost::timer	m_kRepeatTimer;
        DWORD           m_dwTimerStamp;
		DWORD           m_dwRepeatTimerStamp;
		int				m_iHeartBeatUID;
		float			m_fCheckTerm;
		bool			m_bState;
		byte			m_byteMaxCount;
		byte			m_byteCount;

		LB_INFO_STRUCT_CHECKER( __int64 iRoomUID )
			: m_iRoomUID( iRoomUID )
			, m_iHeartBeatUID( 0 )
			, m_fCheckTerm( 0.0f )
			, m_bState( true )
			, m_byteMaxCount( 0 )
			, m_byteCount( 0 )
		{
			//m_kTimer.restart();
            m_dwRepeatTimerStamp = m_dwTimerStamp = ::GetTickCount();
		}
	};

	struct LB_INFO_STRUCT_VERIFY
	{
		__int64			m_iRoomUID;
		//boost::timer	m_kTimer;
        //boost::timer	m_kRepeatTimer;
        DWORD           m_dwTimerStamp;      
        DWORD           m_dwRepeatTimerStamp;
		int				m_iHeartBeatUID;
		float			m_fVerifyTerm;
		bool			m_bState;
		byte			m_byteMaxCount;
		byte			m_byteCount;

		LB_INFO_STRUCT_VERIFY( __int64 iRoomUID )
			: m_iRoomUID( iRoomUID )
			, m_iHeartBeatUID( 0 )
			, m_fVerifyTerm( 0.0f )
			, m_bState( true )
			, m_byteMaxCount( 0 )
			, m_byteCount( 0 )
		{
		//	m_kTimer.restart();
            m_dwRepeatTimerStamp = m_dwTimerStamp = ::GetTickCount();
		}
	};

#endif  SERV_FIX_SYNC_PACKET_USING_RELAY


//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#pragma pack( push, 1 )

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY

	struct SSP_HEART_BEAT_REQ
	{
		int			m_iHeartBeatUID;

		SSP_HEART_BEAT_REQ()
			: m_iHeartBeatUID( 0 )
		{
		}
	};

	struct SSP_HEART_BEAT_ACK
	{
		__int64		m_iUnitUID;
		int			m_iHeartBeatUID;

		SSP_HEART_BEAT_ACK()
			: m_iUnitUID( 0 )
			, m_iHeartBeatUID( 0 )
		{
		}
	};

	struct SSP_CONNECT_CHECK_REQ
	{
		int			m_iHeartBeatUID;

		SSP_CONNECT_CHECK_REQ()
			: m_iHeartBeatUID( 0 )
		{
		}
	};

	struct SSP_CONNECT_CHECK_ACK
	{
		__int64		m_iUnitUID;
		int			m_iHeartBeatUID;

		SSP_CONNECT_CHECK_ACK()
			: m_iUnitUID( 0 )
			, m_iHeartBeatUID( 0 )
		{
		}
	};
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	struct SSP_CONNECT_TEST_REQ
//	{
//		__int64		m_UID;
//		DWORD		m_SendTime;
//	};
//
//	struct SSP_CONNECT_TEST_ACK
//	{
//		__int64		m_UID;
//		DWORD		m_SendTime;
//	};
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


	struct SSP_CONNECT_RELAY_REQ
	{
		__int64		m_UID;
        DWORD       m_dwStamp;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
        DWORD       m_dwTimestamp;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
	};

	struct SSP_CONNECT_RELAY_ACK
	{
		__int64		m_UID;
        DWORD       m_dwStamp;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
        DWORD       m_dwTimestamp;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
	};

    struct  SSP_RELAY_SET_UID_LIST_REQ
    {
        __int64     m_UID;
        DWORD       m_dwStamp;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
        DWORD       m_dwTimestamp;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
        BYTE        m_byNumUIDs;
        __int64     m_aUIDs[];
    };

    struct  SSP_RELAY_SET_UID_LIST_ACK
    {
        __int64     m_UID;
        DWORD       m_dwStamp;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
        DWORD       m_dwTimestamp;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
    };

    struct  SSP_RELAY_PRECONFIG
    {
        __int64     m_UID;
    };

    struct  SSP_RELAY
    {
        __int64     m_UID;
        BYTE        m_byNumUIDs;
        __int64     m_aUIDs[];
    };

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	struct SSP_DEFENCE_PORT
	{
		bool		bDummy;
	};

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#pragma pack( pop )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	struct Peer
	{
		__int64		m_UID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        u_long      m_IPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		wstring		m_IP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        u_long      m_InternalIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		wstring		m_InternalIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		//bool		m_bUseInternalIP;
		//int			m_InternalPort;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
		//}}
		int			m_Port;
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		bool		m_bUseRelay;
//
//		int			m_ConnectTestCount;
//		bool		m_bConnected;
//		float		m_fPingTime;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		float		m_SleepTime;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        std::vector<__int64>    m_vecRelayUIDs;
        DWORD                   m_dwRelayUIDsStamp;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		Peer()
		{
			m_UID				= 0;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            m_IPAddress = INADDR_ANY;
            //m_InternalIPAddress = INADDR_ANY;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_IP				= L"";
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			m_Port				= 0;
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_bUseRelay			= false;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
			//m_bUseInternalIP	= false;
			//m_InternalPort		= 0;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
			//}}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_ConnectTestCount	= 0;
//			m_bConnected		= false;
//			m_fPingTime			= 0.0f;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

			m_SleepTime			= 0.0f;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            m_vecRelayUIDs.resize(0);
            m_dwRelayUIDsStamp = 0;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		}
		Peer( const Peer& t ) { *this = t; }    // copy constructor
		Peer& operator=( const Peer& t )       // assign operator
		{
			m_UID = t.m_UID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            m_IPAddress = t.m_IPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_IP = t.m_IP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			m_Port = t.m_Port;
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_bUseRelay = t.m_bUseRelay;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

			//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//            m_InternalIPAddress = t.m_InternalIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_InternalIP = t.m_InternalIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			//m_InternalPort = t.m_InternalPort;
			//m_bUseInternalIP = t.m_bUseInternalIP;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
			//}}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_ConnectTestCount = t.m_ConnectTestCount;
//			m_bConnected = t.m_bConnected;
//			m_fPingTime = t.m_fPingTime;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

			m_SleepTime = t.m_SleepTime;


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            m_vecRelayUIDs = t.m_vecRelayUIDs;
            m_dwRelayUIDsStamp = t.m_dwRelayUIDsStamp;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

			return *this;
		}
	};

	struct RecvData
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        u_long      m_SenderIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		wstring		m_SenderIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		int			m_SenderPort;
		//char		m_ID;
		//int		m_Size;
        int         m_iRecvBufferSize;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		char*       m_pRecvBuffer;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        char*		m_pRecvBuffer;
//		float		m_fLocalWaitTime;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
        DWORD       m_dwRecvTimestamp;
#endif  SERV_FIX_SYNC_PACKET_USING_RELAY
        
               
		RecvData()
		{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            m_SenderIPAddress = INADDR_ANY;
//#else	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_SenderIP = wstring();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			m_SenderPort	= 0;
			//m_ID			= 0;
			//m_Size			= 0;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            m_iRecvBufferSize = 0;
            m_pRecvBuffer = new char[RECV_BUFFER_SIZE];
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_pRecvBuffer	= NULL;
//            m_fLocalWaitTime = 0.f;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
            m_dwRecvTimestamp = 0;
#endif  SERV_FIX_SYNC_PACKET_USING_RELAY        
		}
        RecvData( const RecvData& rhs_ )
        {
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            m_SenderIPAddress = rhs_.m_SenderIPAddress;
//#else	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_SenderIP = rhs_.m_SenderIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			m_SenderPort	= rhs_.m_SenderPort;
			//m_ID			= rhs_.m_ID;
			//m_Size			= rhs_.m_Size;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            m_pRecvBuffer = new char[RECV_BUFFER_SIZE];
            m_iRecvBufferSize = rhs_.m_iRecvBufferSize;
            if ( m_iRecvBufferSize > 0 && m_iRecvBufferSize <= RECV_BUFFER_SIZE )
            {
                memcpy( m_pRecvBuffer, rhs_.m_pRecvBuffer, m_iRecvBufferSize );
            }
            else
            {
                m_iRecvBufferSize = 0;
            }
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_pRecvBuffer	= NULL;
//            m_fLocalWaitTime = rhs_.m_fLocalWaitTime;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
            m_dwRecvTimestamp = rhs_.m_dwRecvTimestamp;
#endif  SERV_FIX_SYNC_PACKET_USING_RELA
        }
		~RecvData()
		{
            SAFE_DELETE_ARRAY( m_pRecvBuffer );
		}
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		void Init()
		{
            m_SenderIPAddress = INADDR_ANY;
			m_SenderPort	= 0;
			//m_ID			= 0;
			//m_Size			= 0;
            m_iRecvBufferSize = 0;
#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
            m_dwRecvTimestamp = 0;
#endif  SERV_FIX_SYNC_PACKET_USING_RELA        
		}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//		void SetReceiveBuffer( char*pData, int size )
//		{
////#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
////			SAFE_DELETE_ARRAY( m_pRecvBuffer );
////			m_pRecvBuffer = new char[size];
////#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//            m_iRecvBufferSize = size;
//			memcpy( m_pRecvBuffer, pData, size );
//		}

        char        GetID() const { return ( m_iRecvBufferSize <= 0 ) ? 0 : m_pRecvBuffer[0]; }
        int         GetSize() const { return ( m_iRecvBufferSize <= 0 ) ? 0 : m_iRecvBufferSize - 1; }
        const char* GetData() const { return &m_pRecvBuffer[1]; }
private:
        RecvData& operator = ( const RecvData& rhs_ );
	};

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	struct  OverlappedDataInOp
	{
		OVERLAPPED m_overlapped;

		WSABUF  m_wsaBuf;
		std::list<OverlappedDataInOp>::iterator  m_iterList;
		CKTDNUDP* m_pKTDNUDP;

		int m_iSendCounter;//send 전용
		struct sockaddr_in m_sin;//recv 전용
		int m_iSinSize;//recv 전용
		char*    m_pRecvBuffer;

		OverlappedDataInOp()
			: m_overlapped()
			, m_wsaBuf()
			, m_iterList()
			, m_pKTDNUDP( NULL )
			, m_iSendCounter( 0 )
			, m_sin()
			, m_iSinSize( sizeof( m_sin ) )
		{
			ZeroMemory( &m_overlapped, sizeof(m_overlapped) );
            m_pRecvBuffer = new char[RECV_BUFFER_SIZE];
            m_wsaBuf.buf = &m_pRecvBuffer[0];
            m_wsaBuf.len = 0;
		}
        OverlappedDataInOp( const OverlappedDataInOp& rhs_ )
            : m_overlapped()
            , m_iterList( rhs_.m_iterList )
            , m_pKTDNUDP( rhs_.m_pKTDNUDP )
            , m_iSendCounter( rhs_.m_iSendCounter )
            , m_sin( rhs_.m_sin )
            , m_iSinSize( sizeof( m_sin ) )
        {
			ZeroMemory( &m_overlapped, sizeof(m_overlapped) );
            m_pRecvBuffer = new char[RECV_BUFFER_SIZE];
            m_wsaBuf.buf = &m_pRecvBuffer[0];
            m_wsaBuf.len = rhs_.m_wsaBuf.len;
            if ( m_wsaBuf.len > 0 && m_wsaBuf.len <= RECV_BUFFER_SIZE )
            {
                memcpy( m_pRecvBuffer, rhs_.m_pRecvBuffer, rhs_.m_wsaBuf.len );
            }
            else
            {
                m_wsaBuf.len = 0;
            }
        }
		~OverlappedDataInOp()
		{
            SAFE_DELETE_ARRAY( m_pRecvBuffer );
		}
		void Init()
		{
			ZeroMemory( &m_overlapped, sizeof(m_overlapped) );
            m_wsaBuf.buf = &m_pRecvBuffer[0];
            m_wsaBuf.len = 0;
			m_iterList = std::list<OverlappedDataInOp>::iterator();
			m_pKTDNUDP = NULL;
			m_iSendCounter = 0;
			m_sin = sockaddr_in();
			m_iSinSize = sizeof( m_sin );
		}

        void    SwapRecvBuffer( RecvData& recvData )
        {
            //ASSERT( recvData.m_pRecvBuffer != NULL );
            char* pOrgBuffer = m_pRecvBuffer;
            int iOrgLen = m_wsaBuf.len;
            m_pRecvBuffer = recvData.m_pRecvBuffer;
            m_wsaBuf.buf = &m_pRecvBuffer[0];
            m_wsaBuf.len = recvData.m_iRecvBufferSize;
            recvData.m_pRecvBuffer = pOrgBuffer;
            recvData.m_iRecvBufferSize = iOrgLen;
        }
private:
        OverlappedDataInOp& operator = ( const OverlappedDataInOp& rhs_ );
	};//struct  OverlappedDataInOp
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

public:
	CKTDNUDP( int port );
	~CKTDNUDP(void);

	//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	void						OnFrameMove( void );
#else
	void						OnFrameMove( double fTime, float fElapsedTime );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	__forceinline bool		GetFrontRecvData( list<RecvData>::iterator& itRecvData )
	{
		KTDXPROFILE();

		if( m_RecvDataList.empty() )
			return false;

		itRecvData = m_RecvDataList.begin();
		return true;
	}

    static u_long               ConvertIPToAddress( const WCHAR* pIP );
    static std::wstring         ConvertAddressToIP( u_long ulIP );

//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	__forceinline RecvData*		PopRecvData()
//	{
//		KTDXPROFILE();
//
//		if( m_RecvDataList.size() == 0 )
//			return NULL;
//
//		RecvData* pRecvData = m_RecvDataList[0];
//		m_RecvDataList.erase( m_RecvDataList.begin() );
//		return pRecvData;
//	}
//
//	__forceinline void			ClearRecvBuffer()
//	{
//		KTDXPROFILE();
//
//		WSANETWORKEVENTS	netEvent;
//
//		//패킷 리시브
//		while( true )
//		{
//			//Event select
//			::ZeroMemory( &netEvent, sizeof(netEvent) );
//			::WSAEnumNetworkEvents( m_Socket, m_RecvEvent, &netEvent );
//
//			if( (netEvent.lNetworkEvents & FD_READ) == FD_READ )
//				Recv();
//			else
//				break;
//		}
//
//		for( int i = 0; i < (int)m_RecvDataList.size(); i++ )
//		{
//			RecvData* pRecvData = m_RecvDataList[i];
//			SAFE_DELETE( pRecvData );
//		}
//		m_RecvDataList.clear();
//
//		//for( int i = 0; i < (int)m_RecvDataWaitList.size(); i++ )
//		//{
//		//	RecvData* pRecvData = m_RecvDataWaitList[i];
//		//	SAFE_DELETE( pRecvData );
//		//}
//		//m_RecvDataWaitList.clear();
//	}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK



//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	__forceinline bool			SendToPeer( __int64 UID, const char ID, const void* pBuffer, int size );
	__forceinline bool			SendToIP( u_long  ulIP, int port, const char ID, const void* pBuffer, int size );
//#else  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	__forceinline bool			Send( __int64 UID, const char ID, const char* pBuffer, int size );
//	__forceinline bool			Send( const WCHAR* pIP, int port, const char ID, const char* pBuffer, int size );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	bool						SendRelay( const char ID, const char* pBuffer, int size, vector<__int64>& UIDList );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK			
	__forceinline bool			BroadCast( const char ID, const void* pBuffer, int size )
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	__forceinline bool			BroadCast( const char ID, const char* pBuffer, int size )
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
	{
		KTDXPROFILE();

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
        if( size < 0 || size > 0 && pBuffer == NULL )
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        if( !pBuffer )
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
        {
            return false;
        }

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		unsigned long	compressSize			= 2000;
//		char			pCompressBuffer[2000]	= {0,};
//        if( !Compress( ID, pCompressBuffer, compressSize, pBuffer, size ) )
//        {
//            return false;
//        }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
		bool retval = true;
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		vector<__int64> UIDList;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		map<__int64,Peer>::iterator iter;

		for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
		{
			Peer* pPeer = &iter->second;
			pPeer->m_SleepTime = 0.0f;

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			if( pPeer->m_bUseRelay == true )
//			{			
//				UIDList.push_back( pPeer->m_UID );
//			}
//			else
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                WSABUF  alpBuffers[2];
                alpBuffers[0].buf = (CHAR*)&ID;
                alpBuffers[0].len = 1;
                alpBuffers[1].buf = (CHAR*)pBuffer;
                alpBuffers[1].len = size;
                retval = SendPure_Scattered( pPeer->m_IPAddress, pPeer->m_Port, alpBuffers, (size>0) ? 2 : 1 );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				retval = SendPure( pPeer->m_IP.c_str(), pPeer->m_Port, ID, pCompressBuffer, compressSize );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			}
		}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		if( UIDList.size() > 0 )
//		{
//			char	pFinalBuffer[3000]	= {0,};
//			int		finalBufSize = 0;
//
//			AddRelayHeader( ID, pFinalBuffer, finalBufSize, pCompressBuffer + 1, compressSize - 1, UIDList );
//			retval = SendPure( m_RelayIP.c_str(), m_RelayPort, ID, pFinalBuffer, finalBufSize );
//		}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		return retval;
	}

	int							GetPeerSize() const { return static_cast<int>(m_PeerMap.size()); }
	bool						RemovePeer( __int64 UID );
	void						ClearPeer(){ m_PeerMap.clear(); }
	__forceinline const Peer*	GetPeer( __int64 UID );
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	__forceinline Peer*			GetMyPeer();
//	__forceinline void			SetMyUID( __int64 myUID ){ m_MyUID = myUID; }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    __forceinline u_long        GetMyIPAddress(){ return m_MyIPAddress; }
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	__forceinline const WCHAR*	GetMyIP(){ return m_MyIP.c_str(); }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	__forceinline const int		GetMyPort(){ return m_MyPort; }
	__forceinline const int		GetMyExtPort(){ return m_MyExtPort; }
	__forceinline void			SetMyExtPort( int extPort ){ m_MyExtPort = extPort; }

//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	void						ConnectTestToPeer();
//	bool						ConnectTestResult();
//
//	void						SetRelayIP( const WCHAR* pIP ){ m_RelayIP = pIP; }
//	void						SetRelayPort( int port ){ m_RelayPort = port; }
//
//	void						ConnectTestToRelay();
//	bool						ConnectRelayTestResult(){ return m_bConnectRelay; }
//	void						DisconnectToRelay(){ m_bConnectRelay = false; }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	//{{ 2009. 12. 14  최육사	RELAY서버예외처리
	//int							GetPacketLossCount() { return m_iPacketLoss; }
	//}}
	//{{ 2010. 07. 05  최육사	UDP 트래픽 로그
	__int64						GetUDPSendTraffic()	{ return m_iUDPSendTraffic; }
	__int64						GetUDPRecvTraffic() { return m_iUDPRecvTraffic; }
	//}}
	//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	__forceinline	bool		IsLocal( IN const u_long ulCheckIP )
//	{
//		if( ( ulCheckIP != m_ulLocalIP1 ) &&
//			( ulCheckIP != m_ulLocalIP2 )
//			)
//		{
//			return false;
//		}
//		return true;
//	}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	void						LanBugCheckProcess( void );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	static	__forceinline bool	Compress( const char& ID, char* pCompressBuffer, unsigned long& compressSize, const char* pBuffer, const int& size )
//	{
//		if( !pCompressBuffer )
//		{
//			return false;
//		}
//
//		if( !pBuffer )
//		{
//			return false;
//		}
//
//		pCompressBuffer[0] = ID;
//		if(Z_OK	!= compress( (BYTE*)(&pCompressBuffer[1]), &compressSize, (BYTE*)pBuffer, size ) )
//		{
//			return false;
//		}
//		else
//		{
//			compressSize++;
//		}
//
//		return true;
//	}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    void    ThreadSafeSwapLBInfo( std::vector<CKTDNUDP::LB_INFO_STRUCT>& vecInOut );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#endif  SERV_FIX_SYNC_PACKET_USING_RELAY

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    // KTDNUDP 개체를 생성한 thread에서만 사용가능!
    void    ThreadUnsafe_ObtainOverlappedData( std::list<OverlappedDataInOp>& list1, std::list<OverlappedDataInOp>& list2 );
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

protected:
	//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
//#ifdef SERV_UDP_RELAY_CHECKER	
	SOCKET						GetSocketHandle()	{ return m_Socket; }
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    void						AddPeerForRelayTest( __int64 UID, u_long ulIP, int port );
	bool						RecvForRelayTest();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	WSAEVENT&					GetRecvEvent()		{ return m_RecvEvent; }
//	void						AddPeerForRelayTest( __int64 UID, const WCHAR* pIP, int port );
//	bool						RecvForRelayTest( RecvData& kRecvData );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


//#endif SERV_UDP_RELAY_CHECKER
	//}}

private:

    __forceinline Peer*	        _GetPeer( __int64 UID );
    Peer*                       _AddPeer( __int64 UID, u_long ulIP, int port );    

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    __forceinline bool			SendPure_Scattered( u_long ulIP, int port, WSABUF alpBuffers[], DWORD dwBuffCount );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	__forceinline bool			SendPure( const WCHAR* pIP, int port, const char ID, const char* pBuffer, int size );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	__forceinline bool			Recv();
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK	
		void			Relay( unsigned uNumUIDS, const __int64* pUIDs, const char* Data, int iSize );
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	__forceinline void			Relay( int recvSize );
//	__forceinline void			DefencePort();
//	__forceinline void			AddRelayHeader( const char& ID, char* pFinalBuffer, int& finalSize, const char* pBuffer, const int& size, vector<__int64>& UIDList )
//	{
//		unsigned long headerSize	= (3 * sizeof(char)) + (UIDList.size() * sizeof(__int64));
//		char headerBuf[500] = {0,};
//		ZeroMemory( headerBuf, sizeof(char) * headerSize );
//		headerBuf[0] = SP_RELAY;
//		headerBuf[1] = ID;
//		headerBuf[2] = (char) UIDList.size();
//		char* pHeaderIndex = &headerBuf[3];
//		for( int i = 0; i < (int)UIDList.size(); i++ )
//		{
//			__int64 UID = UIDList[i];
//			memcpy( pHeaderIndex, &UID, sizeof(__int64) );
//			pHeaderIndex += sizeof(__int64);
//		}
//
//		memcpy( pFinalBuffer, headerBuf, headerSize );
//		memcpy( (pFinalBuffer + headerSize), pBuffer, size );
//
//		finalSize = size+headerSize;
//	}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    u_long                      m_MyIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	wstring						m_MyIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	int							m_MyPort;
	int							m_MyExtPort;

//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	bool						m_bConnectRelay;
//	float						m_fConnectRelayTime;
//	wstring						m_RelayIP;
//	int							m_RelayPort;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	SOCKET						m_Socket;
	SOCKADDR_IN					m_LocalAddr;

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	list<OverlappedDataInOp> m_listOverlappedFree;
	list<OverlappedDataInOp> m_listSendOverlappedInOp;
	list<OverlappedDataInOp> m_listRecvOverlappedInOp;


	list<RecvData>		m_RecvDataList;
	list<RecvData>		m_RecvFreeDataList;
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	WSAEVENT					m_RecvEvent;
//
//	char						m_pRecvBuffer[RECV_BUFFER_SIZE];
//	vector<RecvData*>			m_RecvDataList;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	//vector<RecvData*>			m_RecvDataWaitList;
	map<__int64,Peer>			m_PeerMap;
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	__int64						m_MyUID;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	//float						m_fLocalWaitTime;
//	float						m_fConnectTestTime;
//
//
//	float						m_fDefencePortTime;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	//{{ 2009. 12. 14  최육사	RELAY서버예외처리
	int							m_iPacketLoss;
	//}}
	//{{ 2010. 07. 05  최육사	UDP 트래픽 로그
	__int64						m_iUDPSendTraffic;
	__int64						m_iUDPRecvTraffic;
	//}}
	//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

    void    _ProcessMonitoringNot( const LB_INFO_STRUCT& kInfo );

    std::vector<LB_INFO_STRUCT>                         m_vecLocalLBInfo;
    KncCriticalSection                                  m_csLBInfo;
    std::vector<LB_INFO_STRUCT>                         m_vecLBInfo;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	u_long												m_ulLocalIP1;
//	u_long												m_ulLocalIP2;
//	SOCKADDR_IN											m_DesAddr;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	boost::timer										m_kTimer;
	boost::timer										m_kCheckTimer;
	std::map< __int64, LB_INFO_STRUCT_CHECKER >			m_mapCheckInfo;
	std::map< __int64, LB_INFO_STRUCT_VERIFY >			m_mapVerifyInfo;
#endif SERV_FIX_SYNC_PACKET_USING_RELAY

public:

	void ProcessRecvData();

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
public:
	RecvData& GetEmptyRecvData();
	void ReturnRecvData( list<RecvData>::iterator it );

private:
	static void CALLBACK _CompletionRoutine_Send(
		DWORD dwError
		, DWORD cbTransferred
		, LPWSAOVERLAPPED lpOverlapped
		, DWORD dwFlags );

	static void CALLBACK _CompletionRoutine_Recv(
		DWORD dwError
		, DWORD cbTransferred
		, LPWSAOVERLAPPED lpOverlapped
		, DWORD dwFlags );

	static void CALLBACK _CompletionRoutine_RecvOverlappedForRelayTest(
		DWORD dwError
		, DWORD cbTransferred
		, LPWSAOVERLAPPED lpOverlapped
		, DWORD dwFlags );

	//
	list<OverlappedDataInOp>::iterator _GetEmptySendOverlappedData();
	void _ReturnSendOverlappedData( OverlappedDataInOp& sendInOp );
	bool _SendOverlapped( OverlappedDataInOp& sendInOp, DWORD dwSendLength );

	list<OverlappedDataInOp>::iterator _GetEmptyRecvOverlappedData();
	void _ReturnRecvOverlappedData( OverlappedDataInOp& recvInOp );
	bool _RecvOverlapped( OverlappedDataInOp& recvInOp, DWORD dwRecvLength );
	bool _RecvOverlappedForRelayTest( OverlappedDataInOp& recvInOp, DWORD dwRecvLength );

	//

	//
	int m_iMaxNumOverlappedRecvData;
	float m_fElapsedTime_FrameMove;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//}}
};

#pragma warning(pop)
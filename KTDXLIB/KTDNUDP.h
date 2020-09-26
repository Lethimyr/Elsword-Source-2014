#pragma once

#pragma warning( push )
#pragma warning( disable : 4200 )

//#ifdef X2OPTIMIZE_TCP_RELAY_TEST
//#include "../X2ServerProtocol/X2ServerProtocolLib.h"
//#endif//X2OPTIMIZE_TCP_RELAY_TEST

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
#define CKTDNUDP_MTU_MAX        508
#define RECV_BUFFER_SIZE 2048
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#define RECV_BUFFER_SIZE 500
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#define KTDX_MAX_P2P_CONNECT_TEST_COUNT 40
#define KTDX_MAX_P2P_INTERNAL_CONNECT_TEST_COUNT 5

#ifdef CLIENT_PORT_CHANGE_REQUEST
#define UDP_STATIC_PORT_START 29890
#define UDP_PORT_START 8000
#endif //CLIENT_PORT_CHANGE_REQUEST

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

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        enum    EP2PConnectType
        {
            P2PCONNECT_NONE,
            P2PCONNECT_EXTERNAL,
//#ifdef  SERV_KTDX_RETRY_USING_INTERNAL_IP
            P2PCONNECT_INTERNAL,
//#endif  SERV_KTDX_RETRY_USING_INTERNAL_IP
        };
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
#pragma pack( push, 1 )
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK				

		//{{ 2013. 2. 15	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
		struct SSP_HEART_BEAT_REQ
		{
			int			m_iHeartBeatUID;
		};

		struct SSP_HEART_BEAT_ACK
		{
			__int64		m_iUnitUID;
			int			m_iHeartBeatUID;
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



		struct SSP_CONNECT_TEST_REQ
		{
			__int64		m_UID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef  SERV_KTDX_RETRY_USING_INTERNAL_IP
            bool        m_bInternalIP;
//#endif  SERV_KTDX_RETRY_USING_INTERNAL_IP
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//            DWORD		m_SendTime;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		};

		struct SSP_CONNECT_TEST_ACK
		{
			__int64		m_UID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef  SERV_KTDX_RETRY_USING_INTERNAL_IP
            bool        m_bInternalIP;
//#endif  SERV_KTDX_RETRY_USING_INTERNAL_IP
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		    DWORD		m_SendTime;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		};

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
////#ifdef  SERV_KTDX_RETRY_USING_INTERNAL_IP
//        struct SSP_RETRY_INTERNAL_CONNECT_REQ
//        {
//			__int64		m_UID;
//        };
//
//        struct SSP_RETRY_INTERNAL_CONNECT_ACK
//        {
//			__int64		m_UID;
//        };
////#endif  SERV_KTDX_RETRY_USING_INTERNAL_IP
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

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
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK				

		struct Peer
		{
			__int64		m_UID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            u_long      m_IPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			wstring		m_IP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            u_long      m_InternalIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			wstring		m_InternalIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			//bool		m_bUseInternalIP;
			int			m_InternalPort;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
			//}}
			int			m_Port;
			bool		m_bUseRelay;

			int			m_ConnectTestCount;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            EP2PConnectType m_eP2PConnectType;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			bool		m_bP2PConnected;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

			float		m_SleepTime;
//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			float		m_fPingTime;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

			Peer()
			{
				m_UID				= 0;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                m_IPAddress         = INADDR_ANY;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				m_IP				= L"";
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				m_Port				= 0;
				m_bUseRelay			= false;
				//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                m_InternalIPAddress = INADDR_ANY;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				//m_bUseInternalIP	= false;
				m_InternalPort		= 0;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
				//}}

				m_ConnectTestCount	= 0;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                m_eP2PConnectType    = P2PCONNECT_NONE;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				m_bP2PConnected		= false;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

				m_SleepTime			= 0.0f;
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				m_fPingTime			= 0.0f;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			}
			Peer( const Peer& t ) { *this = t; }    // copy constructor
			Peer& operator=( const Peer& t )       // assign operator
			{
				m_UID = t.m_UID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                m_IPAddress = t.m_IPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				m_IP = t.m_IP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				m_Port = t.m_Port;
				m_bUseRelay = t.m_bUseRelay;

				//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                m_InternalIPAddress = t.m_InternalIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				m_InternalIP = t.m_InternalIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				m_InternalPort = t.m_InternalPort;
				//m_bUseInternalIP = t.m_bUseInternalIP;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
				//}}

				m_ConnectTestCount = t.m_ConnectTestCount;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                m_eP2PConnectType = t.m_eP2PConnectType;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				m_bP2PConnected = t.m_bP2PConnected;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

				m_SleepTime = t.m_SleepTime;
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				m_fPingTime = t.m_fPingTime;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				return *this;
			}
            bool    GetP2PConnected() const
            {
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                return  m_eP2PConnectType != P2PCONNECT_NONE;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//                return  m_bP2PConnected;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            }
            bool    GetUseRelay() const
            {
                return  m_bUseRelay;
            }
		};

		struct RecvData
		{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            u_long      m_SenderIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			wstring		m_SenderIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			int			m_SenderPort;
			char		m_ID;
			int			m_Size;
			char*		m_pRecvBuffer;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
            DWORD       m_dwReceiveTimestamp;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY

			RecvData()
			{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                m_SenderIPAddress = INADDR_ANY;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				m_SenderPort	= 0;
				m_ID			= 0;
				m_Size			= 0;
				m_pRecvBuffer	= NULL;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
                m_dwReceiveTimestamp = 0;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
			}
			~RecvData()
			{
				SAFE_DELETE_ARRAY( m_pRecvBuffer );
			}
			void SetData( char*pData, int size )
			{
				SAFE_DELETE_ARRAY( m_pRecvBuffer );
				m_pRecvBuffer = new char[size];
				memcpy( m_pRecvBuffer, pData, size );
			}
		};

	public:
		CKTDNUDP( int port );
		~CKTDNUDP(void);

		void						OnFrameMove( double fTime, float fElapsedTime );
		__forceinline RecvData*		PopRecvData()
		{
			KTDXPROFILE();

			if( m_RecvDataList.size() == 0 )
				return NULL;

			RecvData* pRecvData = m_RecvDataList[0];
			m_RecvDataList.erase( m_RecvDataList.begin() );
			return pRecvData;
		}
		__forceinline void			ClearRecvBuffer()
		{
			KTDXPROFILE();

			WSANETWORKEVENTS	netEvent;

			//패킷 리시브
			while( true )
			{
				//Event select
				::ZeroMemory( &netEvent, sizeof(netEvent) );
				::WSAEnumNetworkEvents( m_Socket, m_RecvEvent, &netEvent );

				if( (netEvent.lNetworkEvents & FD_READ) == FD_READ )
					Recv();
				else
					break;
			}

			for( int i = 0; i < (int)m_RecvDataList.size(); i++ )
			{
				RecvData* pRecvData = m_RecvDataList[i];
				SAFE_DELETE( pRecvData );
			}
			m_RecvDataList.clear();

			//for( int i = 0; i < (int)m_RecvDataWaitList.size(); i++ )
			//{
			//	RecvData* pRecvData = m_RecvDataWaitList[i];
			//	SAFE_DELETE( pRecvData );
			//}
			//m_RecvDataWaitList.clear();
		}
	
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        static u_long               ConvertIPToAddress( const WCHAR* pIP );
        static std::wstring         ConvertAddressToIP( u_long ulIP );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
		bool						SendToPeer( __int64 UID, const char ID, const void* pBuffer, int size );
		bool						SendToIP( u_long  ulIP, int port, const char ID, const void* pBuffer, int size );
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		bool						Send( __int64 UID, const char ID, const char* pBuffer, int size );
//		bool						Send( const WCHAR* pIP, int port, const char ID, const char* pBuffer, int size );
//		bool						SendRelay( const char ID, const char* pBuffer, int size, vector<__int64>& UIDList );
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK				

//#ifdef    SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
		bool			            BroadCast( const char ID, const void* pBuffer, int size );
		bool                        BroadCast( std::vector<__int64>& toUIDList, const char ID, const void* pBuffer, int size );			
//#endif    SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		const map<__int64,Peer>& GetPeerMap() { return m_PeerMap; }
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        const std::vector<__int64>& GetNonRelayUIDs() { return m_vecNonRelayUIDs; }
        const std::vector<__int64>& GetRelayUIDs()    { return m_vecRelayUIDs; }
		
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
		void						AddPeer( __int64 UID, u_long  ulIP, int port, u_long  ulInternalIP = INADDR_ANY, int iInternalPort = 0 );
//#else SERV_KTDX_RETRY_USING_INTERNAL_IP
//		void						AddPeer( __int64 UID, u_long  ulIP, int port );
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//#else  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
////#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//		void						AddPeer( __int64 UID, const WCHAR* pIP, int port, const WCHAR* pInternalIP = NULL, int iInternalPort = 0 );
////#else
////		void						AddPeer( __int64 UID, const WCHAR* pIP, int port );
////#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//		//}}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		bool						RemovePeer( __int64 UID );
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
        void                        ResetConnectTestToPeersAll();
        void                        ResetConnectTestToPeer( __int64 UID );
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

		
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
		void						ClearPeer();
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		void						ClearPeer(){ m_PeerMap.clear(); }
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		
		__forceinline const Peer*	GetMyPeer();
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
		__forceinline const Peer*	GetPeer( __int64 UID );
		void			            SetMyUID( __int64 myUID );
        __int64                     GetMyUID() { return m_MyUID; }
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		__forceinline Peer*	        GetPeer( __int64 UID );
//		__forceinline void			SetMyUID( __int64 myUID ){ m_MyUID = myUID; }
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        __forceinline u_long        GetMyIPAddress(){ return m_MyIPAddress; }
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		__forceinline const WCHAR*	GetMyIP(){ return m_MyIP.c_str(); }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		__forceinline const int		GetMyPort(){ return m_MyPort; }
		__forceinline const int		GetMyExtPort(){ return m_MyExtPort; }
		__forceinline void			SetMyExtPort( int extPort ){ m_MyExtPort = extPort; }

#ifdef CLIENT_PORT_CHANGE_REQUEST
		__forceinline const int		GetMyPortIndex(){ return m_MyPortIndex; }
		__forceinline const int		GetMyOldPort(){ return m_MyOldPort; }
#endif //CLIENT_PORT_CHANGE_REQUEST

#ifndef SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
		void						SendConnectReq( const Peer* pOtherPeer_ );
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
		void						ConnectTestToPeer();
		bool						ConnectTestResult();


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        void						SetRelayIPAddress( u_long ulIP ){ m_RelayIPAddress = ulIP; }
		void						SetMyUIDAndRelayIPAddressAndPort( const __int64 UID_, u_long ulIP, const int iPort_ )
		{
			SetMyUID( UID_ );
			SetRelayIPAddress( ulIP );
			SetRelayPort( iPort_ );
		}
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		void						SetRelayIP( const WCHAR* pIP ){ m_RelayIP = pIP; }
//		void						SetMyUIDAndRelayIpAndPort( const __int64 UID_, const WCHAR* pIP_, const int iPort_ )
//		{
//			SetMyUID( UID_ );
//			SetRelayIP( pIP_ );
//			SetRelayPort( iPort_ );
//		}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		void						SetRelayPort( int port ){ m_RelayPort = port; }


		void						ConnectTestToRelay();
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        void                        ConnectCheckToRelay();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		bool						ConnectRelayTestResult()
                                    { 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                                        return  m_eRelayState == RELAY_STATE_CONNECTED;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//                                        return m_bConnectRelay; 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                                    }
		void						DisconnectToRelay()
                                    { 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                                        m_eRelayState = RELAY_STATE_DISCONNECTED;
                                        m_fRelayTimer = 0.f;
                                        m_iRelayTimerCounter = 0;
                                        m_dwRelayUIDStampFromServer = 0;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//                                        m_bConnectRelay = false; 
//                                        m_fConnectRelayTime = 0.0f; 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                                    }
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        bool                        GetRelayUIDsUpdated() { return m_dwRelayUIDStamp == m_dwRelayUIDStampFromServer; }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef UDP_DOWNLOAD_BLOCK_CHECK
//		void						ConnectTestToCheck();
//		bool						ConnectCheckResult() const { return m_bConnectCheck; }
//		bool						ReceiveCheckResult() const { return m_bReceiveCheckResult; }
//		void						InitConnectTestToCheck(){m_bConnectCheck = true; m_fConnectCheckTime = 0.f; }
//#endif //UDP_DOWNLOAD_BLOCK_CHECK
		int							GetMyIPListNum(){ return m_MyIPList.size(); }
		bool						IncreaseMyIPIndex()
		{
			if( m_MyIPIndex >= (int)m_MyIPList.size()-1 )
			{
				m_MyIPIndex = (int)m_MyIPList.size()-1;
#ifdef CLIENT_PORT_CHANGE_REQUEST
#else //CLIENT_PORT_CHANGE_REQUEST
				return false;
#endif //CLIENT_PORT_CHANGE_REQUEST
			}
#ifdef CLIENT_PORT_CHANGE_REQUEST
			else
#endif //CLIENT_PORT_CHANGE_REQUEST
			{
				m_MyIPIndex++;
			}
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            m_MyIPAddress = m_MyIPList[m_MyIPIndex];
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_MyIP = m_MyIPList[m_MyIPIndex].c_str();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef CLIENT_PORT_CHANGE_REQUEST
			m_MyOldPort = m_MyPort;
			if ( m_MyPortIndex == 0 )
			{
				m_MyPort = UDP_STATIC_PORT_START;
			}
			else
			{
				m_MyPort = UDP_PORT_START + m_MyPortIndex - 1;
			}

			m_MyPortIndex++;
#endif //CLIENT_PORT_CHANGE_REQUEST

			DeleteSocket();
			CreateSocket();
			return true;
		}

		void						RemoveOtherPeer( std::vector<__int64> UIDList );

        WORD                        EstimateEffectiveMTUSize();

#ifdef NO_P2P_NO_GAME
		UINT GetElapsedFrameAfterPacketReceive() const { return m_uiElapsedFrameAfterPacketReceive; }
#endif NO_P2P_NO_GAME

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        bool                        ForceUseRelay( __int64 UID );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
        void                        RemoveAllPendingPingSends();
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY

	private:

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
        bool                        _RobustConnectTestResult();
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION


        __forceinline Peer*	        _GetPeer( __int64 UID );
	    __forceinline bool			SendPure_Scattered( u_long ulIP, int port, WSABUF alpBuffers[], DWORD dwBuffCount );

        int                         Estimate_RelayPacketSize( const char cRelayID, const std::vector<__int64>& vecUIDs, const char ID = 0, const void* pBuffer = NULL, int size = 0 );
        bool                        SendPure_Relay( const char cRelayID, const std::vector<__int64>& vecUIDs, const char ID = 0, const void* pBuffer = NULL, int size = 0 );

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef UDP_PACKET_ANALYSIS_LOG
//		__forceinline bool			SendPure( const WCHAR* pIP, int port, const char ID, const char* pBuffer, int size, bool bCompress = true );
//#else//UDP_PACKET_ANALYSIS_LOG
//		__forceinline bool			SendPure( const WCHAR* pIP, int port, const char ID, const char* pBuffer, int size );
//#endif//UDP_PACKET_ANALYSIS_LOG
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		__forceinline bool			Recv();
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		__forceinline void			Relay( int recvSize );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		__forceinline void			DefencePort();

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		__forceinline bool			Compress( const char& ID, char* pCompressBuffer, unsigned long& compressSize, const char* pBuffer, const int& size )
//		{
//            if( !pCompressBuffer )
//            {
//                return false;
//            }
//
//            if( !pBuffer )
//            {
//                return false;
//            }
//
//			pCompressBuffer[0] = ID;
//			if(Z_OK	!= compress( (BYTE*)(&pCompressBuffer[1]), &compressSize, (BYTE*)pBuffer, size ) )
//            {
//                return false;
//            }
//			else
//			{
//				compressSize++;
//			}
//
//            return true;
//		}
//		__forceinline void			AddRelayHeader( const char& ID, char* pFinalBuffer, int& finalSize, const char* pBuffer, const int& size, vector<__int64>& UIDList )
//		{
//			unsigned long headerSize	= (3 * sizeof(char)) + (UIDList.size() * sizeof(__int64));
//			char headerBuf[500] = {0,};
//			ZeroMemory( headerBuf, sizeof(char) * headerSize );
//			headerBuf[0] = SP_RELAY;
//			headerBuf[1] = ID;
//			headerBuf[2] = (char) UIDList.size();
//			char* pHeaderIndex = &headerBuf[3];
//			for( int i = 0; i < (int)UIDList.size(); i++ )
//			{
//				__int64 UID = UIDList[i];
//				memcpy( pHeaderIndex, &UID, sizeof(__int64) );
//				pHeaderIndex += sizeof(__int64);
//			}
//
//			memcpy( pFinalBuffer, headerBuf, headerSize );
//			memcpy( (pFinalBuffer + headerSize), pBuffer, size );
//
//			finalSize = size+headerSize;
//		}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		void CreateSocket();
		void DeleteSocket();

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
		void						SendToMe( const char ID, const void* pBuffer, int size );
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		void						SendToMe( const char ID, const char* pBuffer, int size );
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK			
		//{{ 2013. 1. 16	박세훈	//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )

#ifndef SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
		void						ReseveInternalConnect( const __int64 iUID );
		void						CheckAndProcessInternalConnect( IN const float fElapsedTime );
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
		//}}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        void                        _SetUseRelay( Peer* pPeer, bool bOnOff );
        void                        _SetNonRelayUID( __int64 iUID );
        void                        _RemoveRelayUID( __int64 iUID );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		u_long                      m_MyIPAddress;
		vector<u_long>				m_MyIPList;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		wstring						m_MyIP;
//		vector<wstring>				m_MyIPList;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		int							m_MyIPIndex;
		int							m_MyPort;
		int							m_MyExtPort;

#ifdef CLIENT_PORT_CHANGE_REQUEST
		int							m_MyPortIndex; // 2013.05.09 lygan_조성욱 // 포트 번호를 계속 증가 시켜 주기 위해
		int							m_MyOldPort;	// 2013.05.09 lygan_조성욱 // 포트가 막혀있어 변경 되기 전의 포트 임시 저장 ( 유저의 막혀 있는 포트리스트 통계 내기 위해 )
#endif //CLIENT_PORT_CHANGE_REQUEST

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        enum ERelayState
        {
            RELAY_STATE_DISCONNECTED,
            RELAY_STATE_TRYING_TO_CONNECT,
            RELAY_STATE_CONNECTED,
        };
        ERelayState                 m_eRelayState;
        float                       m_fRelayTimer;
        int                         m_iRelayTimerCounter;
        DWORD                       m_dwRelayUIDStamp;
        DWORD                       m_dwRelayUIDStampFromServer;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		bool						m_bConnectRelay;
//		float						m_fConnectRelayTime;
//		float						m_fRelayContinueTime;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        u_long                      m_RelayIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		wstring						m_RelayIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		int							m_RelayPort;

		SOCKET						m_Socket;
		SOCKADDR_IN					m_LocalAddr;
		WSAEVENT					m_RecvEvent;

		char						m_pRecvBuffer[RECV_BUFFER_SIZE];
		vector<RecvData*>			m_RecvDataList;
		//vector<RecvData*>			m_RecvDataWaitList;
		map<__int64,Peer>			m_PeerMap;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        std::vector<__int64>        m_vecNonRelayUIDs;
        std::vector<__int64>        m_vecRelayUIDs;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		__int64						m_MyUID;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
        CKTDNUDPStatisticData       m_kRelayStatistic;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY

		//float						m_fLocalWaitTime;
		float						m_fConnectTestTime;

		float						m_fDefencePortTime;

        float                       m_fElapsedTime;
        DWORD                       m_dwSendByte;
        DWORD                       m_dwSendCount;

#ifdef NO_P2P_NO_GAME
		UINT						m_uiElapsedFrameAfterPacketReceive;	// 마지막으로 실제 네트웍을 통해 패킷을 받은 후 지나간 frame count
#endif NO_P2P_NO_GAME
//#ifdef UDP_DOWNLOAD_BLOCK_CHECK
//		//랜선랙 유저 신고 전 UDP 릴레이 체크
//		float						m_fConnectCheckTime;
//		bool						m_bConnectCheck;
//		bool						m_bReceiveCheckResult;
//#endif //UDP_DOWNLOAD_BLOCK_CHECK
		//{{ 2013. 1. 16	박세훈	//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
#ifndef SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
		std::list< std::pair<__int64, float> >	m_listResevedInternalConnect;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
		//}}

//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	public:
		//치트키용

        enum    EForceConnectMode
        {
            FORCE_CONNECT_MODE_DEFAULT = 0,
            FORCE_CONNECT_MODE_P2P = 1,
            FORCE_CONNECT_MODE_RELAY = 2,
            FORCE_CONNECT_MODE_NUMS
        };

		void SetForceConnectMode( EForceConnectMode eFCMode );

	private:
		//치트키용
		EForceConnectMode   m_eForceConnectMode;
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef UDP_PACKET_ANALYSIS_LOG
	public:
		struct KAnalysisResult
		{
			DWORD m_dwTransCount;
			DWORD m_dwTransSize;
			//DWORD m_dwUnCompressTransSize;

			KAnalysisResult()
			{
				m_dwTransCount = 0;
				m_dwTransSize = 0;
				//m_dwUnCompressTransSize = 0;
			}
		};

		void SetEnablePacketAnalysisLog( bool bEnable ) { m_bUdpPacketAnalysisLog = bEnable; }

	private:
		//패킷이 몇 번 send하는지 저장
		std::map< int, KAnalysisResult > m_mapSendAnalysisResult;
		//패킷이 몇 번 receive하는지 저장
		std::map< int, KAnalysisResult > m_mapReceiveAnalysisResult;

		//
		float m_fSendAnalysisTime;
		float m_fReceiveAnalysisTime;

		//
		bool m_bUdpPacketAnalysisLog;
#endif//UDP_PACKET_ANALYSIS_LOG

//#ifdef X2OPTIMIZE_TCP_RELAY_TEST
//public:
//		void SetServerProtocol( CX2ServerProtocol* pkServerProtocol ) { m_pkServerProtocol = pkServerProtocol; }
//
//		void ReceiveTcpRelay( const char ID, const void* pBuffer, int size );
//
//private:
//	CX2ServerProtocol* m_pkServerProtocol;
//#endif//X2OPTIMIZE_TCP_RELAY_TEST

};

#pragma warning( pop )
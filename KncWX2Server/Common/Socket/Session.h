#pragma once
#include "ServerDefine.h"
#include <WinSock2.h>
#include "Performer.h"
#include "Thread/Thread.h"
#include "SocketObject.h"
#include "KncSecurity/KncSecurity.h"
#include "KncSend.h"
//#include "ServerDefine.h"

SmartPointer( KSession );
SmartPointer( KSkSession );

class KSkSession : public KSocketObject
{
public:
    void OnRecvCompleted( DWORD dwTransfered );
    void OnSocketError();
    void OnAcceptConnection();
    KSessionPtr GetSessionPtr();

    boost::weak_ptr< KSession > m_pkSession;
};

class KSession : public KPerformer, public KThread
{
    NiDeclareRTTI;
    DeclToStringW;

    KSession(); // 구현부는 없음. 사용되면 안된다. (사용하면 링크에러남.)
    KSession(const KSession& right);
    KSession& operator=(const KSession& right);

public:
    enum
    {
        SECURE_KEY_WAITING_TIME     = 5000,		// 접속시 보안키를 받기까지 대기하는 최대시간
		PACKET_AUTH_FAIL_DEFAULT	= 100,		// 패킷 캡쳐 유저 판단 기준 default값
    };

	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	enum SESSION_TYPE
	{
		SST_NONE = 0,
		SST_SERVER,
		SST_CLIENT,
	};
#endif SERV_KEVENT_FROM
	//}}

    KSession( bool bIsProxy );
	virtual ~KSession( void );

    virtual bool Init( bool bUseIocp );

    // derived from KThread
    virtual bool Begin();

    // derived from KPerformer
    virtual void Tick();    // reserve destroy, check heart beat

    void ReserveDestroy();  // OnDestory 예약

    // 060321. florist. RefCount가 남아있어 삭제되지 않을 가능성이 있는 경우를 검사한다.
    virtual bool CheckExceedRefCount( int nCount );

    template < typename T > 
    bool SendPacket( DWORD dwDestPI, UidType nTo, UidType anTrace[], unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = true );
    bool SendPacket( IN const KEvent& kEvent );
    bool SendID( DWORD dwDestPI, UidType nTo, UidType anTrace[], unsigned short usEventID );

    bool IsConnected() const			{ _JIF( m_spSockObj, return false ); return m_spSockObj->IsConnected(); }
    unsigned int GetIP() const			{ _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetIP(); }
    const char* GetIPStr() const		{ _JIF( m_spSockObj, return NULL ); return m_spSockObj->GetIPStr(); }
    unsigned short GetPort() const		{ _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetPort(); }
    KSkSessionPtr GetSockObj()          { return m_spSockObj; }
    bool GetKey( DWORD& dwKey ) const   { _JIF( m_spSockObj, return false ); dwKey = m_spSockObj->GetKey(); return true; }
    bool SetKey( DWORD dwKey )          { _JIF( m_spSockObj, return false ); m_spSockObj->SetKey( dwKey ); return true; }

    static void DumpMaxSendData( std::wostream& stm );
    static void ResetMaxSendData();
	//{{ 2009. 10. 18  최육사	SequenceNum
	static void DumpPacketAuthInfo( std::wostream& stm );
	//}}

    // use proxy-child only.
    virtual bool Connect( const char* szIP, unsigned short usPort );

    // user unproxy-child only.
    virtual void OnAcceptConnection(); // accept 되자마자 호출. 이곳에서 인증키를 생성해 클라이언트로 전송한다.

	void			SetP2PIP( const std::wstring& wstrIP );//{ m_wstrIP = wstrIP; }
	std::wstring	GetP2PIP(){ return m_wstrIP; }
	void	SetP2PPort( int nPort ){ m_nPort = nPort; }
	int		GetP2PPort(){ return m_nPort; }
    void SetSendQueueSize( int nSize ) { JIF( m_spSockObj ); m_spSockObj->SetSendQueueSize( nSize ); }

	//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
	void			SetP2PInternalIP( const std::wstring& wstrIP ); //{ m_wstrInternalIP = wstrIP; }
	std::wstring	GetP2PInternalIP(){ return m_wstrInternalIP; }
	void			SetP2PInternalPort( USHORT usPort ){ m_usInternalPort = usPort; }
	USHORT			GetP2PInternalPort(){ return m_usInternalPort; }
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}

	// Disconnect Reason Function
	int GetDisconnectReason() { _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetDisconnectReason(); }
	void SetDisconnectReason( int iDisconnectReason ) { _JIF( m_spSockObj, return ); m_spSockObj->SetDisconnectReason( iDisconnectReason ); }

	//{{ 2009. 9. 1  최육사		IOCP종료사유
	DWORD GetLastIocpFailedError() { _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetLastIocpFailedError(); }
	//}}
	//{{ 2009. 10. 18  최육사	Sequence Number
	void InitCheckSequenceNumInfo( bool bIsCheckSequenceNum, u_int uiLimitCount ) { m_bCheckSequenceNum = bIsCheckSequenceNum; m_uiPacketAuthFailLimit = uiLimitCount; }	
	u_int GetPacketAuthFailLimit() const			{ return m_uiPacketAuthFailLimit; }
	bool IsCheckSequenceNum()						{ return m_bCheckSequenceNum; }
	void IncreasePacketAuthFailCnt()				{ KLocker lock( m_csPacketAuthFailCnt ); ++m_uiPacketAuthFailCnt; }
	unsigned int GetPacketAuthFailCnt()				{ KLocker lock( m_csPacketAuthFailCnt ); return m_uiPacketAuthFailCnt; }
	//}}

	//{{ 2009. 8. 10  최육사	Send Buffer Full
	unsigned short GetLastSendEventID() { return m_usLastSendEventID; }
	//}}
	//{{ 2009. 9. 29  최육사	하트비트
	DWORD GetHBTick() { return m_dwHBTick; }
	//}}
	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	SESSION_TYPE GetSessionType() const { return m_eSessionType; }
	void SetSessionType( const SESSION_TYPE eType ) { m_eSessionType = eType; }
#endif SERV_KEVENT_FROM
	//}}

protected:
    // util function
	//void OnSendCompleted( DWORD dwTransfered );
    //void OnRecvCompleted( DWORD dwTransfered );
    void OnRecvCompleted( IN const KncSecurity::KByteStream& payload );
    // derived from KThread
    virtual void Run();
    // derive to child.
    virtual void OnSocketError();   ///< 050129. WSASend, IOCP등에서 소켓 오류를 발견한 경우 호출.
    virtual void OnDestroy();       ///< 050129. multi thread safe하도록 수정함

protected:
    KSkSessionPtr       m_spSockObj;
    bool                m_bUseIocp;     ///< if use Iocp
    bool				m_bIsProxy;		///< Proxy인 경우, H.B를 보내고, Connect를 할 수 있다.
    DWORD               m_dwHBTick;     // heart bit tick    
    bool                m_bDestroyReserved;     ///< 050129. 생성한 스레드에서 소멸 처리.

    static struct KMaxPacketInfo {
        KncCriticalSection  m_cs;           // lua script에서의 작업에 대해 보호한다
        std::wstring        m_strName;
        std::wstring        m_strDataDesc;
        size_t              m_nDataSize;    // 순수 게임 데이터만 고려된 사이즈
        size_t              m_nPacketSize;  // 암호화 처리를 거친 후, 실제 전송되는 데이터 사이즈
        CTime               m_cTime;
    } ms_kMaxPacketInfo;

	//{{ 2009. 10. 18  최육사	SequenceNum	
	static struct KPacketAuthInfo {
		KncCriticalSection  m_cs;			// lua script에서의 작업에 대해 보호한다
		std::wstring        m_strName;
		unsigned int		m_uiMaxFailCnt;
		unsigned int		m_uiTotalFailCnt;		
		CTime               m_cRegTime;
	} ms_kPacketAuthInfo;
	//}}

    // proxy-child only.
    enum ENUM_KEventType {    
        EVENT_DISCONNECTED,
        EVENT_RECV_COMPLETED,        
        EVENT_SEND_COMPLETED,        
        EVENT_MAX_VALUE
    };
    HANDLE              m_hEvents[EVENT_MAX_VALUE];

    friend class KIOThread;         // access On(Recv/Send)oCompleted()
    friend class KSkSession;        // m_nSPIndex, OnRecvCompleted, OnSocketError

    bool                m_bAuthKeyRecved;   // 보안 키 받았는지 확인 하는 변수.
    SpiType             m_nSPIndex;
    bool                m_bCheckHBTick;     // florist. debug의 편의를 위해 hb 체크 on/off를 가능하게 함.
	//{{ 2009. 10. 18  최육사	Sequence Number
	bool				m_bCheckSequenceNum;
	unsigned int		m_uiPacketAuthFailLimit;
	unsigned int		m_uiPacketAuthFailCnt;
	KncCriticalSection  m_csPacketAuthFailCnt;
	//}}

	//p2p통신을 위한 ip/port
	std::wstring		m_wstrIP;
	int					m_nPort;

	//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
	std::wstring		m_wstrInternalIP;
	USHORT				m_usInternalPort;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}

	//{{ 2009. 8. 10  최육사	send buffer full사유찾기
	unsigned short		m_usLastSendEventID;
	//}}

	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	SESSION_TYPE		m_eSessionType;
#endif SERV_KEVENT_FROM
	//}}
	//{{ 2011. 10. 31	최육사	하트비트 오류 원인 분석
#ifdef SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	DWORD				m_dwHBTickDump;
	DWORD				m_dwCurrentTickDump;
#endif SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	//}}

#ifdef SERV_DISCONNECT_LOG_FOR_SENDBUFFERFULL// 작업날짜: 2013-09-23// 박세훈
	typedef std::pair< unsigned short, CTime >		TYPE_PAIR_USHORT_CTIME;
	typedef	std::queue< TYPE_PAIR_USHORT_CTIME >	TYPE_SEND_BUFFER_FULL_LOG;
	TYPE_SEND_BUFFER_FULL_LOG	m_queEventID;
#endif // SERV_DISCONNECT_LOG_FOR_SENDBUFFERFULL
};

template < typename T > 
bool KSession::SendPacket( DWORD dwDestPI_, UidType nTo_, UidType anTrace_[], unsigned short usEventID_, T& data_, bool bLogging_, bool bCompress_ )
{
	KEvent kEvent;
    kEvent.SetData( dwDestPI_, anTrace_, usEventID_, data_ );
    LIF( kEvent.m_kDestPerformer.AddUID( nTo_ ) );

    if( bCompress_ ) kEvent.m_kbuff.Compress();   // 압축할 경우, KEvent의 buff만을 압축한다.

    // send packet
    if( ! KSession::SendPacket( kEvent ) )
        return false;

    if( bLogging_ )
        dbg::clog << L"ⓢ " << kEvent.GetIDStr() << L" (name:" << m_strName << L")" << END_LOG;

    return true;
}
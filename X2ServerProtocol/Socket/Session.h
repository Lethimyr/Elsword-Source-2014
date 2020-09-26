#pragma once
#include "SocketObject.h"

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
		SECURE_KEY_WAITING_TIME     = 5000, // 접속시 보안키를 받기까지 대기하는 최대시간
	};

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
//#ifdef X2OPTIMIZE_TCP_RELAY_TEST
//	bool SendPacket( DWORD dwDestPI, UidType nTo, UidType anTrace[], unsigned short usEventID, char* pBuffer, int iBufferSize );
//#endif//X2OPTIMIZE_TCP_RELAY_TEST
    bool SendPacket( IN const KEvent& kEvent );
    bool SendID( DWORD dwDestPI, UidType nTo, UidType anTrace[], unsigned short usEventID );
	//bool InitRecv();

    bool IsConnected() const			{ _JIF( m_spSockObj, return false ); return m_spSockObj->IsConnected(); }
    unsigned int GetIP() const			{ _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetIP(); }
    const char* GetIPStr() const		{ _JIF( m_spSockObj, return NULL ); return m_spSockObj->GetIPStr(); }
    unsigned short GetPort() const		{ _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetPort(); }
    KSkSessionPtr GetSockObj()          { return m_spSockObj; }
    bool GetKey( DWORD& dwKey ) const   { _JIF( m_spSockObj, return false ); dwKey = m_spSockObj->GetKey(); return true; }
    bool SetKey( DWORD dwKey )          { _JIF( m_spSockObj, return false ); m_spSockObj->SetKey( dwKey ); return true; }

	static void DumpMaxSendData( std::wostream& stm );
	static void ResetMaxSendData();

	// use proxy-child only.
	virtual bool Connect( const char* szIP, unsigned short usPort );

	// user unproxy-child only.
	virtual void OnAcceptConnection(); // accept 되자마자 호출. 이곳에서 인증키를 생성해 클라이언트로 전송한다.

	void			SetP2PIP( std::wstring wstrIP ){ m_wstrIP = wstrIP; }
	std::wstring	GetP2PIP(){ return m_wstrIP; }
	void	SetP2PPort( int nPort ){ m_nPort = nPort; }
	int		GetP2PPort(){ return m_nPort; }
    void SetSendQueueSize( int nSize ) { JIF( m_spSockObj ); m_spSockObj->SetSendQueueSize( nSize ); }	

	// Disconnect Reason Function
	int GetDisconnectReason() { _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetDisconnectReason(); }
	void SetDisconnectReason( int iDisconnectReason ) { JIF( m_spSockObj ); m_spSockObj->SetDisconnectReason( iDisconnectReason ); }
	
	//{{ 2012. 03. 19	김민성		KSADatabase 를 Session 의 멤버로 이동(클라이언트만)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	KncSecurity::KSADatabase* GetSADatabase();
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
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
	virtual void OnAuthenticFailed(); // 패킷 인증이 실패 했을때 호출되는 가상함수, 세부 동작은 재정의 하여 사용할것!

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

	//p2p통신을 위한 ip/port
	std::wstring					m_wstrIP;
	int								m_nPort;	

	//{{ 2012. 03. 19	김민성		KSADatabase 를 Session 의 멤버로 이동(클라이언트만)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	KncSecurity::KSADatabase*					m_pSADatabase;
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
	//}}

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

#ifdef _IN_HOUSE_
	if( g_bIsSERVICE == false )
	{
		if( bLogging_ )
			dbg::clog << L"ⓢ " << kEvent.GetIDStr() << L" (name:" << m_strName << L")" << END_LOG;
	}
#endif

    return true;
}
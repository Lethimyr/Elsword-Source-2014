#pragma once

#include "ServerDefine.h"
#include "ActorFactory.h"

#include <ToString.h>
#include <RTTI.h>
#include <Socket/Accepter.h>
#include <Socket/UdpEcho.h>
#include <Socket/LBSUdpEcho.h>
#include <KncSingleton.h>
//#include "ServerDefine.h"

class KNetLayer
{
    DeclToStringW;
    DeclInstanceKeeping( KNetLayer );
    NiDeclareRootRTTI( KNetLayer );

protected:
    KNetLayer( void );  // KNetLayer의 객체가 직접적으로 생성되지 않도록 생성자의 접근 권한을 protected로 한다.
public:
    virtual ~KNetLayer( void );

    void ResetMaxData();

    // derive to child...	
	virtual bool Init();
	virtual bool InitNCUDP( int port = -1 );
    virtual void CloseAccepter(); // 서버 종료시 더이상의 접속을 받지 않도록 Accepter 먼저 닫아야 한다.
    virtual void ShutDown();
    virtual void RegToLua();

    // 070423. florist. Accepter thread만 접근한다. (다른 스레드는 접근 시키지 마세요)
    static void OnAccept( SOCKET sock, const SOCKADDR_IN& sinRemote );

    void SetActorFactory( IActorFactoryPtr spActorFactory )     { m_spActorFactory = spActorFactory; }
    void DumpMaxTimeInfo( OUT std::wostream& stm ) const;
    void ResetMaxTimeInfo();

    void SetPort( unsigned short usPort )       { m_usPortMaster = usPort; }
    void SetUdpEcho( unsigned short usPort )    { m_usUdpPort = usPort; }

    u_short GetNCUDPPort()                      { return m_kLBSUdpEcho.GetPort(); }

	//{{ 2009. 10. 18  최육사	SequenceNum
	void SetCheckSequenceNum_LUA( bool bVal );
	bool IsCheckSequenceNum()					{ KLocker lock( m_csCheckSequenceNum ); return m_bCheckSequenceNum; }
	void SetPacketAuthFailLimit_LUA( u_int uiCount );
	u_int GetPacketAuthFailLimit()				{ KLocker lock( m_csPacketAuthFailLimit ); return m_uiPacketAuthFailLimit; }
	//}}

	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	unsigned int GetSessionType() const					{ return m_uiSessionType; }
	void SetSessionType( unsigned int uiSessionType )	{ m_uiSessionType = uiSessionType; }
#endif SERV_KEVENT_FROM
	//}}

	//{{ 2009. 12. 15  최육사	서버관리
	virtual void DumpToLogFile();
	//}}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    CKTDNUDPPtr GetKTUDP()                      { return m_kLBSUdpEcho.GetKTUDP(); }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

public: // lua access
    unsigned short m_usPortMaster;
    USHORT         m_usUdpPort;

protected:
    void OnAccept_( SOCKET sock, const SOCKADDR_IN& sinRemote ); // static 함수로부터 단순히 delegate받음.

protected:
    KUdpEcho                m_kUdpEcho;
    KLBSUdpEcho             m_kLBSUdpEcho;
    KAccepter               m_kAccepter;
	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	unsigned int					m_uiSessionType;
#endif SERV_KEVENT_FROM
	//}}

    struct KMaxTimeInfo {
        mutable KncCriticalSection  m_cs;
        std::wstring                m_strIP;
        DWORD                       m_dwTerm;
        CTime                       m_cTime;
    } m_kMaxTimeInfo;

    IActorFactoryPtr    m_spActorFactory;

	//{{ 2009. 10. 18  최육사	SequenceNum
	bool							m_bCheckSequenceNum;
	mutable KncCriticalSection      m_csCheckSequenceNum;
	u_int							m_uiPacketAuthFailLimit;
	mutable KncCriticalSection      m_csPacketAuthFailLimit;
	//}}
};

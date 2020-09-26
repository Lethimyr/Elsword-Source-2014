#pragma once

#include "Socket/SocketObject.h"
#include "RTTI.h"
#include "KncSmartPtr.h"
#include "KncUidType.h"
#include "Serializer/SerBuffer.h"
#include <boost/enable_shared_from_this.hpp>
#include <dbg/dbg.hpp>
#include <KncSecurity/KncSecurity.h>

#define KTRUserPtr KTRUser*
#undef KTRUserPtr
#define KSkTRUserPtr KSkTRUser*
#undef KSkTRUserPtr

SmartPointer( KTRUser );
SmartPointer( KSkTRUser );

class KSkTRUser : public KSocketObject 
{
public:
    enum
    {
        MINIMUM_PACKET_SIZE = 3,
        MAXIMUM_PACKET_SIZE = 512
    };
    void OnRecvCompleted( DWORD dwTransfered );
    void OnSocketError();
    //void OnAcceptConnection() { m_pkTRUser->OnAcceptConnection(); }
    KTRUserPtr GetTRUserPtr();
    boost::weak_ptr< KTRUser > m_pkTRUser;
};

class KTRUser : public boost::enable_shared_from_this< KTRUser >
{
public:
	enum ENUM_TICKS // Tick 관련
	{
		CONNECT_TICK = 0,
		HB_TICK,            // heart_bit
		TICKS_NUM,
	};

	KTRUser(void);
	virtual ~KTRUser(void);

	void Tick();

	virtual bool Init();
	virtual bool Connect( const char* szIP, unsigned short usPort, const UidType iUnitUID );

	UidType			GetUID()const       { return m_iUnitUID; }

	template< typename T >
	bool			SendPacket( unsigned short usEventID, T& data, bool bCompress = true );

	void			ReserveDestroy() { m_bDestroyReserved = true; }

	bool			IsConnected() const { _JIF( m_spSockObj, return false ); return m_spSockObj->IsConnected(); }
	const char*		GetIPStr() const	{ _JIF( m_spSockObj, return NULL ); return m_spSockObj->GetIPStr(); }
	unsigned short	GetPort() const		{ _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetPort(); }

	void			SetClassID( int iClassID )	{ m_iClassID = iClassID; }  // 윈도우 메세지를 구성할때 사용할 id 할당.
	int				GetClassID()				{ return m_iClassID; }
	void			SetServerMsgID( UINT serverMsgID ){ m_ServerMsgID = serverMsgID; }

    KSkTRUserPtr	GetSkTRUserPtr()        { return m_spSockObj; }

    void	OnSocketError();

protected:
	//virtual	void	Run();  

	void	SetTick( IN ENUM_TICKS eIndex )	{ m_adwTickCount[ eIndex ] = ::GetTickCount(); }
	DWORD	GetTick( IN ENUM_TICKS eIndex)	{ return m_adwTickCount[ eIndex ]; }

	//void	OnRecvCompleted( IN const char cRecvCnt, IN KSerBuffer& buff );
	void	OnRecvCompleted( IN KSerBuffer& buffer );
	void	OnDestroy();

protected:
    KSkTRUserPtr        m_spSockObj;
	bool                m_bUseIocp;
	bool				m_bDestroyReserved;
	UidType             m_iUnitUID;
	DWORD               m_adwTickCount[ENUM_TICKS::TICKS_NUM];
	UINT				m_ServerMsgID;
	int					m_iClassID;         // 이벤트를 윈도우 메세지로 던질 때, 구분자 역할을 한다.

	DWORD	ret;
	bool	bLoop;

	// proxy-child only.
	enum ENUM_EVENT_TYPE 
	{    
		EVENT_DISCONNECTED,
		EVENT_RECV_COMPLETED,        
		EVENT_SEND_COMPLETED,        
		EVENT_MAX_VALUE
	};
	HANDLE              m_hEvents[EVENT_MAX_VALUE];
};

template<typename T>
bool KTRUser::SendPacket( unsigned short usEventID, T& data_, bool bCompress_ /* = true  */)
{
	//if( m_bDestroyReserved ) return true;///< 종료가 예약되었을때 send 시도. 이런 경우가 다분하므로, true 인정.
	_JIF( m_spSockObj, return false );
    _JIF( m_spSockObj->IsConnected(), return false );    ///< 소켓이 유효하지 않음.

	KEvent kEvent;
	kEvent.SetData( 0, NULL, usEventID, data_ );

	if( bCompress_ ) kEvent.m_kbuff.Compress();

	// serialize - full event object
	KSerializer     ks;
	KSerBuffer      kbuff;
	ks.BeginWriting( &kbuff );
	ks.Put( kEvent );
	ks.EndWriting();

	KncSecurity::KByteStream    bsbuff;
	bsbuff.reserve( kbuff.GetLength() + sizeof(USHORT) );
	bsbuff.Assign( kbuff.GetLength() + sizeof(USHORT), sizeof(USHORT) );
	bsbuff.append( (byte*)kbuff.GetData(), kbuff.GetLength() );   // 실제 전송할 데이터 넣기.

	if( kEvent.m_usEventID != E_HEART_BEAT )
		dbg::clog << L"ⓢⓢ " << kEvent.GetIDStr() << L" - TR SEND" << END_LOG;

	return m_spSockObj->SendData( (const char*)bsbuff.data(), bsbuff.length() );
}
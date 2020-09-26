//////////////////////////////////////////////////////////////////////////
//des : IOCP 를 이용한 통신 수신을 하여 릴레이 하며
//.1. : PROXY 를 사용하지 않음
//.2. : 패킷을 암호화 하지 않기 위해 SOCKET OBJECT를 활용.
//////////////////////////////////////////////////////////////////////////
#pragma once 

#include "Socket/SocketObject.h"
#include "RTTI.h"
#include "KncSmartPtr.h"
#include "KncUidType.h"
#include "Serializer/SerBuffer.h"
#include <boost/enable_shared_from_this.hpp>
#include "FSM/support_FSM.h"
#include <dbg/dbg.hpp>
#include <KncSecurity/KncSecurity.h>

#include "Profiler/Profiler.h"
#include "GarenaBillingPacket.h"



#ifdef SERV_COUNTRY_PH


#define KGarenaBillingUserPtr KGarenaBillingUser*
#undef KGarenaBillingUserPtr
#define KSkGarenaBillingUserPtr KSkGarenaBillingUser*
#undef KSkGarenaBillingUserPtr

SmartPointer(KGarenaBillingUser);
SmartPointer(KSkGarenaBillingUser);


class KSkGarenaBillingUser : public KSocketObject
{
public:
    enum
    {
        MINIMUM_PACKET_SIZE = 3,
        MAXIMUM_PACKET_SIZE = 512,
    };

public:
    void OnRecvCompleted( DWORD dwTransfered );
    void OnSocketError();
    void OnAcceptConnection();
    KGarenaBillingUserPtr GetGarenaBillingUserPtr();

	//{{ 2009. 3. 12  최육사	프로파일러 체크용 함수
	bool SendDataWithProfile( const char* szData, int iSize );
	//}}

    boost::weak_ptr< KGarenaBillingUser > m_pkTRUser;
};

class KGarenaBillingUser : public boost::enable_shared_from_this< KGarenaBillingUser >
{
	DeclareFSM_MT;
	NiDeclareRootRTTI(KGarenaBillingUser);

public:
	enum ENUM_TICKS
	{
		CONNECT_TICK = 0,
		HB_TICK,
		TICKS_NUM,
	};

	KGarenaBillingUser(void);
	virtual ~KGarenaBillingUser(void);

	UidType	GetUID(){ return m_spSockObj->GetUID(); }

	bool	Init();
	void	Tick();
	
	bool	SendPacket( const KncSecurity::KByteStream& kByteStream );
	template< typename T >
	static bool MakeByteStream( KncSecurity::KByteStream& kByteStream, unsigned short usEventID, T& data, bool bCompress = true );

	void	ReserveDestroy(){ m_bDestroyReserved = true; }
	void	SetSendQueueSize( int iQueueSize );

	//derive to KSkTRUser
	void	OnSocketError();
	void	OnAcceptConnection();
	void	OnRecvCompleted( KGarenaBillingPacket& kPacket );

	KSkGarenaBillingUserPtr	GetSkGarenaBillingUserPtr(){ return m_spSockObj; }

protected:

	void	SetTick( IN ENUM_TICKS eIndex )	{ m_adwTickCount[ eIndex ] = ::GetTickCount(); }
	DWORD	GetTick( IN ENUM_TICKS eIndex)	{ return m_adwTickCount[ eIndex ]; }

	void	OnDestroy();

protected:
	KSkGarenaBillingUserPtr	m_spSockObj;
	bool					m_bDestroyReserved;
	DWORD					m_adwTickCount[ENUM_TICKS::TICKS_NUM];
};

template< typename T >
bool KGarenaBillingUser::MakeByteStream( KncSecurity::KByteStream& kByteStream, unsigned short usEventID, T& data_, bool bCompress_ /*= true*/ )
{
	KEvent kEvent;
	kEvent.SetData( 0, NULL, usEventID, data_ );

	// serialize - full event object
	KSerializer     ks;
	KSerBuffer      kbuff;
	ks.BeginWriting( &kbuff );
	ks.Put( kEvent );
	ks.EndWriting();

	kByteStream.reserve( kbuff.GetLength() + sizeof(USHORT) );
	kByteStream.Assign( kbuff.GetLength() + sizeof(USHORT), sizeof(USHORT) );
	kByteStream.append( (byte*)kbuff.GetData(), kbuff.GetLength() );   // 실제 전송할 데이터 넣기.

	return true;
}


#endif SERV_COUNTRY_PH
//}}
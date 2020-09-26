#pragma once

#include "Room/RoomManager.h"
#include "LoginBaseRoom.h"
#include "X2Data/XSLRoom.h"
//{{ 2010. 03. 08  최육사	TickDumper
#ifdef SERV_ROOM_TICK
	#include "TickDumper.h"
#endif SERV_ROOM_TICK
//}}

#include "LoginRMEventProcess.h"

//{{ 2008. 12. 3  최육사	클래스 상속으로 인한 메모리 참조 오류 해결
#pragma pack( push, 1 )
//}}

class KLoginRoomManager : public KRoomManager
{
	NiDeclareRTTI;
	DeclDump;
	DeclToStringW;

public:
	KLoginRoomManager();
	virtual ~KLoginRoomManager();

	virtual void Run();     // derived from KThread

	bool Init();
	void Tick();
	void RegToLua();	

	virtual void ShutDown();

	DWORD GetNumEmptyRoom() const;

	UidType OpenRoom();
	bool CloseRoom( UidType iRoomUID );

	// derived from KRoomManager
	virtual void QueueingEventToRoom( const KEventPtr& spEvent );
	virtual void QueueingToAll( int iRoomType, const KEventPtr& spEvent );
	virtual void QueueingToAll( const KEventPtr& spEvent );

	//{{ 2010. 06. 17  최육사	오토핵 방지
	void QueueingEventToRoomManager( const KEventPtr& spEvent );	
	//}}
	void QueueingEventToRoomByRoomUID( IN const UidType iRoomUID, IN const KEventPtr& spEvent );

	//Zombie user 관련.
	void	SetZUTickTime( double dfTickTime );
	double	GetZUTickTime();

	bool AddPShopOwner( IN const UidType iUnitUID, IN const UidType iRoomUID );
	void DelPShopOwner( IN const UidType iUnitUID );
	bool GetPShopAgencyUIDByUnitUID( IN const UidType iUnitUID, OUT UidType& iRoomUID );

protected:
	// packet send function
	template < class T >
	void SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
	void SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID );

	FSMclassPtr CreateRoomFSM();
	KLoginBaseRoomPtr CreateRoom();
	KLoginBaseRoomPtr Get( UidType iRoomUID );

	// 빈방 만들기
    bool CreateEmptyRoom();

protected:
	std::map< UidType, KLoginBaseRoomPtr >	m_mapRoomList;
	std::vector< KLoginBaseRoomPtr >		m_vecRoomList;
	std::map< UidType, UidType >			m_mapEmptyRoomList;
	DWORD									m_dwNextRoomNumber;
	FSMclassPtr                             m_spRoomFSM;
	mutable KncCriticalSection				m_csEmptyRoomList;	// 빈 방 리스트 보호.

	std::map< UidType, UidType >			m_mapPShopOwnerList; // 대리상점 개설자 리스트

	double									m_dfZUTickTime;

	//{{ 2010. 06. 17  최육사	스크립트 실시간 패치
#ifdef SERV_REALTIME_SCRIPT
	KLoginRMEventProcess					m_kRMEventProcess;
#endif SERV_REALTIME_SCRIPT
	//}}

	//{{ 2010. 03. 08  최육사	TickDumper
#ifdef SERV_ROOM_TICK
	KTickDumper								m_kRoomManagerTickDump;
#endif SERV_ROOM_TICK
	//}}
	//{{ 2011. 03. 24	최육사	서버 상태 체크
#ifdef SERV_ROOM_TICK_DUMP
	KMultiTickDumper						m_kRoomTickDump;
#endif SERV_ROOM_TICK_DUMP
	//}}
};

DefKObjectInline( KLoginRoomManager, KRoomManager );

template < class T >
void KLoginRoomManager::SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( PI_LOGIN_ROOM, 0, PI_GS_CHARACTER, nTo, anTrace, usEventID, data );
}

//{{ 2008. 12. 3  최육사	클래스 상속으로 인한 메모리 참조 오류 해결
#pragma pack( pop )
//}}

/*    64 bit ( 8 byte )

| ff ff ff ff | ff ff ff ff |

| 00 00 00 ff | ff ff ff ff | 순수 uid 영역. ( 5byte = 40bit. 2^40 = 1 0995 1162 7776 )
| 80 00 00 00 | 00 00 00 00 | 부호비트로 남겨둠
| 40 00 00 00 | 00 00 00 00 | 로컬에서 임시로 발행되는 uid.
| 3f 00 00 00 | 00 00 00 00 | 서버군 uid.
| 00 ff 00 00 | 00 00 00 00 | 룸 서버 uid.
| 00 00 f0 00 | 00 00 00 00 | reserved.
| 00 00 0f 00 | 00 00 00 00 | room type.

*/
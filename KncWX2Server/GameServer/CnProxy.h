#pragma once
#include "ActorProxy.h"
#include <windows.h>
#include "Thread/Thread.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

SmartPointer( KCnProxy );

class KGSSimLayer;
class KCnProxy : public KActorProxy
{
    typedef KGSSimLayer SLayerType;

    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KCnProxy(void);
    virtual ~KCnProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& spEvent );

    // derived from KSession
    virtual void OnDestroy();

protected:

	//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING	
	DECL_ON_FUNC( ECN_VERIFY_SERVER_CONNECT_ACK );
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}

	//{{ 2010. 01. 27  최육사	방 정보
#ifdef SERV_ROOM_COUNT
	DECL_ON_FUNC( ERM_ROOM_COUNT_INFO_NOT );
#endif SERV_ROOM_COUNT
	//}}

    DECL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY );

    bool RoutePacket( const KEventPtr& spEvent );
	void ProcbyNotExistUser( const KEventPtr& spEvent, std::vector<UidType>& vecUID );

protected:
	//{{ 2010. 04. 16  최육사	서버군 전체 통합
#ifdef SERV_INTEGRATION
public:
	virtual int	 GetActorServerGroup()		{ return m_iCnServerGroupID; }
protected:
	virtual bool IsSameServerGroup()		{ return ( m_iCnServerGroupID == m_iGsServerGroupID ); }
protected:
	int			m_iCnServerGroupID;
	int			m_iGsServerGroupID;
#endif SERV_INTEGRATION
	//}}
};
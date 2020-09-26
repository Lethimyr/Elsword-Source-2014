#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 17  최육사	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "Enum/Enum.h"

//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
	#include "Room/RoomManager.h"
#endif SERV_TRADE_DDOS_DEFENCE
//}}

class KCnGameDBThread : public KSubThread
{
    DeclareDBThread( KCnGameDBThread );
	DeclareSPProfiler;
    DeclPfID;
public:

    KCnGameDBThread();   // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KCnGameDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );
	
	bool Query_InsertItemList( IN const SEnum::GET_ITEM_REASON eGetItemReason, 
							   IN const UidType iUnitUID, 
							   IN const std::vector< KItemInfo >& vecItemInfo, 
							   OUT std::map< UidType, KItemInfo >& mapInsertedItemInfo, 
							   IN const bool bUnSeal = true );

	bool Query_UpdateItemQuantity( IN const UidType iUnitUID, 
								   IN const std::map< UidType, int >& mapUpdated, 
								   OUT std::map< UidType, int >& mapFailed, 
								   OUT bool& bUpdateFailed );

	void Query_DeleteItem( IN const std::vector< KDeletedItemInfo >& vecDeleted, OUT std::vector< KDeletedItemInfo >& vecFailed );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToRoom( UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );

	//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
	DECL_ON_FUNC( DBE_DB_UPDATE_TRADE_RESULT_REQ );
#else
//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
	DECL_ON_FUNC( DBE_DB_UPDATE_TRADE_RESULT_REQ );
#endif SERV_TRADE_DDOS_DEFENCE
//}}
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
	//}}
	//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	DECL_ON_FUNC( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ );
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	//}}

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	DECL_ON_FUNC( DBE_WEDDING_COMPLETE_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
	//}
};

template < class T >
void KCnGameDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KCnGameDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_CN_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KCnGameDBThread::SendToRoom( UidType nTo, unsigned short usEventID, const T& data )
{
	// KncSend로는 가지 않는다. 직접 KEvent만들어서 넣자!
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_ROOM, NULL, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( nTo ) );
	KRoomManager::GetKObj()->QueueingEventToRoom( spEvent );
}




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


//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	#include "Room/RoomManager.h"
#endif SERV_PSHOP_AGENCY
//}}


class KLoginGameDBThread : public KSubThread
{
    DeclareDBThread( KLoginGameDBThread );
	//{{ 2009. 8. 17  최육사	sp profiler
	DeclareSPProfiler;
	//}}
    DeclPfID;
public:

    KLoginGameDBThread();   // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KLoginGameDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	template < class T > void SendToRoom( UidType nTo, unsigned short usEventID, const T& data );
#endif SERV_PSHOP_AGENCY
	//}}	
    void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );

	// query function
#ifdef SERV_GUILD_AD
	bool DeleteApplyJoinGuildList( IN int iGuildUID, IN UidType iUnitUID );
	bool DeleteApplyJoinGuildList( IN const std::vector< KApplyDeleteInfo >& vecInfo );
#endif SERV_GUILD_AD
    
#ifdef SERV_UPGRADE_TRADE_SYSTEM
    void GetPShopItem( IN const UidType& iUnitUID_, OUT std::vector< KSellPersonalShopItemInfo >& vecSellItemInfo_ );
#endif SERV_UPGRADE_TRADE_SYSTEM

	//{{ 2009. 7. 6  최육사		랭킹 개편
	DECL_ON_FUNC_NOPARAM( DBE_GET_WEB_RANKING_INFO_REQ );
	//}}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 11. 19  최육사	길드구조개선
	DECL_ON_FUNC( DBE_GET_GUILD_INFO_REQ );
	DECL_ON_FUNC( DBE_GUILD_LEVEL_UP_REQ );
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2009. 11. 24  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

	DECL_ON_FUNC( DBE_GET_GUILD_SKILL_REQ );
	DECL_ON_FUNC( DBE_INSERT_GUILD_CASH_SKILL_POINT_REQ );
	DECL_ON_FUNC( DBE_EXPIRE_GUILD_CASH_SKILL_POINT_REQ );
   _DECL_ON_FUNC( DBE_ADMIN_GET_GUILD_SKILL_POINT_REQ, KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ );

#endif GUILD_SKILL_TEST
	//}}

	//////////////////////////////////////////////////////////////////////////   
	//{{ 2010. 01. 13  최육사	길드게시판
#ifdef SERV_GUILD_AD

	DECL_ON_FUNC_NOPARAM( DBE_GET_GUILD_BOARD_INFO_REQ );
   _DECL_ON_FUNC( DBE_GET_GUILD_SKILL_IN_BOARD_REQ, KELG_GET_GUILD_SKILL_IN_BOARD_REQ );
	DECL_ON_FUNC( DBE_REGISTRATION_GUILD_AD_REQ );
   _DECL_ON_FUNC( DBE_MODIFY_REG_GUILD_AD_REQ, KELG_MODIFY_REG_GUILD_AD_REQ );
	DECL_ON_FUNC( DBE_APPLY_JOIN_GUILD_REQ );
   _DECL_ON_FUNC( DBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_REQ, KELG_ACCEPT_JOIN_GUILD_REQ );
	DECL_ON_FUNC( DBE_ACCEPT_JOIN_GUILD_REQ );
   _DECL_ON_FUNC( DBE_DELETE_APPLY_JOIN_GUILD_REQ, KELG_DELETE_APPLY_JOIN_GUILD_REQ );
	DECL_ON_FUNC( DBE_DELETE_GUILD_AD_LIST_NOT );

#endif SERV_GUILD_AD   
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT	
   _DECL_ON_FUNC( DBE_GET_WEB_POINT_REWARD_CHECK_NICKNAME_REQ, KDBE_GET_WEB_POINT_REWARD_ACK );
#endif SERV_WEB_POINT_EVENT
	//}}

	//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	DECL_ON_FUNC( DBE_INSERT_REWARD_TO_POST_REQ );

	DECL_ON_FUNC_NOPARAM( DBE_LOAD_PSHOP_AGENCY_REQ );
   _DECL_ON_FUNC( DBE_OPEN_PSHOP_AGENCY_REQ, KERM_OPEN_PERSONAL_SHOP_ACK );
   _DECL_ON_FUNC( DBE_BREAK_PSHOP_AGENCY_REQ, KERM_BREAK_PSHOP_AGENCY_NOT );
   _DECL_ON_FUNC( DBE_REG_PSHOP_AGENCY_ITEM_REQ, KERM_REG_PERSONAL_SHOP_ITEM_ACK );
	DECL_ON_FUNC( DBE_BUY_PSHOP_AGENCY_ITEM_REQ );
	DECL_ON_FUNC( DBE_STOP_SALE_PSHOP_AGENCY_REQ );
	DECL_ON_FUNC( DBE_PICK_UP_FROM_PSHOP_AGENCY_REQ );
#endif SERV_PSHOP_AGENCY
	//}}
	//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	DECL_ON_FUNC( DBE_INSERT_TITLE_REQ );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}

	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
	DECL_ON_FUNC( DBE_CREATE_GUILD_REQ );
	DECL_ON_FUNC( DBE_CHANGE_GUILD_NAME_CHECK_REQ );
	DECL_ON_FUNC( DBE_CHANGE_GUILD_NAME_REQ );
#endif SERV_GUILD_CHANGE_NAME
	//}}
	//{{ 2012. 06. 05	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _DECL_ON_FUNC( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
   //{{ 2013. 3. 18	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
   DECL_ON_FUNC_NOPARAM( DBE_GAME_LOCAL_RANKING_INIT_INFO_REQ );
   _DECL_ON_FUNC( DBE_LOCAL_RANKING_RESET_REQ, int );
   DECL_ON_FUNC( DBE_LOCAL_RANKING_RANKER_CHANGE_INFO_NOT );
   DECL_ON_FUNC_NOPARAM( DBE_LOCAL_RANKING_RESET_CHECK_REQ );
   _DECL_ON_FUNC( DBE_GAME_LOCAL_RANKING_WATCH_UNIT_REQ, KELG_LOCAL_RANKING_WATCH_UNIT_REQ );
#endif SERV_LOCAL_RANKING_SYSTEM
   //}}

   //{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
   DECL_ON_FUNC_NOPARAM( DBE_LOAD_WEDDING_HALL_INFO_REQ );
   DECL_ON_FUNC( DBE_WEDDING_HALL_INFO_UPDATE_NOT );
   _DECL_ON_FUNC( DBE_RELATIONSHIP_INFO_GAME_DB_REQ, KELG_RELATIONSHIP_INFO_REQ );
   _DECL_ON_FUNC( DBE_WEDDING_ITEM_FIND_INFO_REQ, KELG_WEDDING_ITEM_FIND_INFO_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
   //}

};

template < class T >
void KLoginGameDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KLoginGameDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_LOGIN_USER, nTo, NULL, usEventID, data );
}

//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
template < class T >
void KLoginGameDBThread::SendToRoom( UidType nTo, unsigned short usEventID, const T& data )
{
	// KncSend로는 가지 않는다. 직접 KEvent만들어서 넣자!
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_ROOM, NULL, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( nTo ) );
	KRoomManager::GetKObj()->QueueingEventToRoom( spEvent );
}
#endif SERV_PSHOP_AGENCY
//}}


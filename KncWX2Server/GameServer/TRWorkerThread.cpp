#include "TRServer.h"
#include "TRWorkerThread.h"



#define CLASS_TYPE      KTRWorkerThread

KTRWorkerThread::KTRWorkerThread()
{
}

KTRWorkerThread::~KTRWorkerThread()
{
	End( 3000 );
}

void KTRWorkerThread::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
		CASE( ETR_FIELD_UNIT_SYNC_DATA_BROAD_NOT );
		CASE( ETR_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT );
		CASE( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT );	
		//{{ 2009. 3. 31  최육사	파티UID갱신
		CASE( ETR_UPDATE_PARTY_NOT );
		//}}
		//{{ 2009. 3. 31  최육사	필드채팅
		CASE( ETR_CHAT_NOT );
		//}}
		//{{ 2009. 9. 23  최육사	1차전직캐쉬템
		CASE( ETR_CHANGE_FIELD_USER_INFO_NOT );
		//}}
		//{{ 2010. 7. 26  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		CASE( ETR_SUMMON_PET_NOT );
		CASE( ETR_PET_ACTION_NOT );
		CASE( ETR_PET_EVOLUTION_NOT );
#endif SERV_PET_SYSTEM
		//}}
#ifdef SERV_INVISIBLE_GM
		CASE( ETR_TOGGLE_INVISIBLE_NOT );
#endif SERV_INVISIBLE_GM

		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		CASE( ETR_COUPLE_MAKING_SUCCESS_NOT );
		CASE( ETR_RELATIONSHIP_EFFECT_NOT );
#endif SERV_RELATIONSHIP_SYSTEM
		//}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		CASE( ETR_SUMMON_RIDING_PET_NOT );
		CASE( ETR_UNSUMMON_RIDING_PET_NOT );
#endif	// SERV_RIDING_PET_SYSTM

	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음 - " << spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID );
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 1000 )
	{
		START_LOG( cwarn, L"TCP RELAY 패킷 처리 시간지연.!" )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}
}

IMPL_ON_FUNC( ETR_FIELD_UNIT_SYNC_DATA_BROAD_NOT )
{	
	KEGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT kPacketNot;
	kPacketNot.m_UnitUID	   = kPacket_.m_UnitUID;
	kPacketNot.m_vPos		   = kPacket_.m_vPos;
	kPacketNot.m_vecFieldFrame = kPacket_.m_vecFieldFrame;	
	kPacketNot.m_bIsRight	   = kPacket_.m_bIsRight;	

	BroadCast( kPacket_.m_vecUID, EGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT, kPacketNot );
}

IMPL_ON_FUNC( ETR_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT )
{	
	KEGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT kPacketNot;
	kPacketNot.m_iUnitUID			  = kPacket_.m_iUnitUID;
	kPacketNot.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
	kPacketNot.m_iEquippedTitleID	  = kPacket_.m_iEquippedTitleID;
#else
	kPacketNot.m_sEquippedTitleID	  = kPacket_.m_sEquippedTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}

	BroadCast( kPacket_.m_vecUID, EGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT, kPacketNot );
}

IMPL_ON_FUNC( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT )
{	
	KEGS_CHANGE_USER_LIST_IN_SECTOR_NOT kPacketNot;
	kPacketNot.m_vecNewUserList = kPacket_.m_vecNewUserList;
	kPacketNot.m_vecDelUserList = kPacket_.m_vecDelUserList;

	BroadCast( kPacket_.m_vecUID, EGS_CHANGE_USER_LIST_IN_SECTOR_NOT, kPacketNot );
}

//{{ 2009. 3. 31  최육사	파티UID갱신
IMPL_ON_FUNC( ETR_UPDATE_PARTY_NOT )
{	
	KEGS_UPDATE_PARTY_UID_NOT kPacketNot;
	kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacketNot.m_iPartyUID = kPacket_.m_iPartyUID;

	BroadCast( kPacket_.m_vecUID, EGS_UPDATE_PARTY_UID_NOT, kPacketNot );
}
//}}

//{{ 2009. 3. 31  최육사	필드채팅
IMPL_ON_FUNC( ETR_CHAT_NOT )
{	
	KEGS_CHAT_NOT kPacketNot;
	kPacketNot.m_SenderUnitUID	 = kPacket_.m_SenderUnitUID;
	kPacketNot.m_RecieverUnitUID = kPacket_.m_RecieverUnitUID;
	kPacketNot.m_cChatPacketType = kPacket_.m_cChatPacketType;
	kPacketNot.m_cRoomChatType	 = kPacket_.m_cRoomChatType;
	kPacketNot.m_wstrMsg		 = kPacket_.m_wstrMsg;

	BroadCast( kPacket_.m_vecUID, EGS_CHAT_NOT, kPacketNot );
}
//}}

//{{ 2009. 9. 23  최육사	1차전직캐쉬템
IMPL_ON_FUNC( ETR_CHANGE_FIELD_USER_INFO_NOT )
{
	KEGS_CHANGE_USER_INFO_NOT kPacketNot;
	kPacketNot.m_iUnitUID		= kPacket_.m_iUnitUID;
	kPacketNot.m_cUnitClass		= kPacket_.m_cUnitClass;
	//{{ 2009. 10. 9  최육사	길드
#ifdef GUILD_TEST
	kPacketNot.m_wstrGuildName	= kPacket_.m_wstrGuildName;
#endif GUILD_TEST
	//}}

	BroadCast( kPacket_.m_vecUID, EGS_CHANGE_USER_INFO_NOT, kPacketNot );
}
//}}

//{{ 2010. 7. 26  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

IMPL_ON_FUNC( ETR_SUMMON_PET_NOT )
{
	KEGS_SUMMON_PET_NOT kPacketNot;
	kPacketNot.m_iUnitUID		= kPacket_.m_iUnitUID;
	kPacketNot.m_vecPetInfo		= kPacket_.m_vecPetInfo;

	BroadCast( kPacket_.m_vecUID, EGS_SUMMON_PET_NOT, kPacketNot );
}

IMPL_ON_FUNC( ETR_PET_ACTION_NOT )
{
	KEGS_PET_ACTION_NOT kPacketNot;
	kPacketNot.m_iUnitUID		= kPacket_.m_iUnitUID;	
	kPacketNot.m_cActionType	= kPacket_.m_cActionType;

	BroadCast( kPacket_.m_vecUID, EGS_PET_ACTION_NOT, kPacketNot );
}

IMPL_ON_FUNC( ETR_PET_EVOLUTION_NOT )
{
	KEGS_PET_EVOLUTION_NOT kPacketNot;
	kPacketNot.m_iUnitUID		= kPacket_.m_iUnitUID;
	kPacketNot.m_kPetInfo		= kPacket_.m_kEvolutionPetInfo;	

	BroadCast( kPacket_.m_vecUID, EGS_PET_EVOLUTION_NOT, kPacketNot );
}

#endif SERV_PET_SYSTEM
//}}

#ifdef SERV_INVISIBLE_GM
IMPL_ON_FUNC( ETR_TOGGLE_INVISIBLE_NOT )
{
	KEGS_TOGGLE_INVISIBLE_NOT kPacketNot;
	kPacketNot.m_iUnitUID			= kPacket_.m_iUnitUID;
	kPacketNot.m_kFieldUserInfo		= kPacket_.m_kFieldUserInfo;
	BroadCast( kPacket_.m_vecUID, EGS_TOGGLE_INVISIBLE_NOT, kPacketNot );
}
#endif SERV_INVISIBLE_GM

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
IMPL_ON_FUNC( ETR_SUMMON_RIDING_PET_NOT )
{
	KEGS_SUMMON_RIDING_PET_NOT kPacketNot;
	kPacketNot.m_iUnitUID		= kPacket_.m_iUnitUID;
	kPacketNot.m_iRidingPetUID	= kPacket_.m_iRidingPetUID;
	kPacketNot.m_usRidingPetID	= kPacket_.m_usRidingPetID;

	BroadCast( kPacket_.m_vecUID, EGS_SUMMON_RIDING_PET_NOT, kPacketNot );
}

IMPL_ON_FUNC( ETR_UNSUMMON_RIDING_PET_NOT )
{
	KEGS_UNSUMMON_RIDING_PET_NOT kPacketNot;
	kPacketNot.m_iUnitUID		= kPacket_.m_iUnitUID;

	BroadCast( kPacket_.m_vecUID, EGS_UNSUMMON_RIDING_PET_NOT, kPacketNot );
}
#endif	// SERV_RIDING_PET_SYSTM
//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( ETR_COUPLE_MAKING_SUCCESS_NOT )
{	
	KEGS_COUPLE_MAKING_SUCCESS_NOT kPacketNot;
	kPacketNot.m_wstrRequestUnitName	 = kPacket_.m_wstrRequestUnitName;
	kPacketNot.m_wstrAcceptUnitName		 = kPacket_.m_wstrAcceptUnitName;
	
	BroadCast( kPacket_.m_vecUID, EGS_COUPLE_MAKING_SUCCESS_NOT, kPacketNot );
}

IMPL_ON_FUNC( ETR_RELATIONSHIP_EFFECT_NOT )
{	
	KEGS_RELATIONSHIP_EFFECT_NOT kPacketNot;
	kPacketNot.m_iManUID		= kPacket_.m_iManUID;
	kPacketNot.m_iGirlUID		= kPacket_.m_iGirlUID;
	kPacketNot.m_wstrLoveWord	= kPacket_.m_wstrLoveWord;
	kPacketNot.m_iEffectIndex	= kPacket_.m_iEffectIndex;

	BroadCast( kPacket_.m_vecUID, EGS_RELATIONSHIP_EFFECT_NOT, kPacketNot );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

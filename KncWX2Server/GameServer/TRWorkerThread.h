#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "TRSubThread.h"

class KTRWorkerThread : public KTRSubThread
{
public:
	KTRWorkerThread();
	virtual ~KTRWorkerThread();

protected:
	virtual void ProcessEvent( const KEventPtr& spEvent );

	template < class T >
	void BroadCast( const std::vector< UidType >& vecUID, u_short usEventID, const T& data );

	// process tcp packet
	DECL_ON_FUNC( ETR_FIELD_UNIT_SYNC_DATA_BROAD_NOT );
	DECL_ON_FUNC( ETR_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT );
	DECL_ON_FUNC( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT );
	//{{ 2009. 3. 31  최육사	파티UID갱신
	DECL_ON_FUNC( ETR_UPDATE_PARTY_NOT );
	//}}
	//{{ 2009. 3. 31  최육사	필드채팅
	DECL_ON_FUNC( ETR_CHAT_NOT );
	//}}
	//{{ 2009. 9. 23  최육사	1차전직캐쉬템
	DECL_ON_FUNC( ETR_CHANGE_FIELD_USER_INFO_NOT );
	//}}
	//{{ 2010. 7. 26  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	DECL_ON_FUNC( ETR_SUMMON_PET_NOT );
	DECL_ON_FUNC( ETR_PET_ACTION_NOT );
	DECL_ON_FUNC( ETR_PET_EVOLUTION_NOT );
#endif SERV_PET_SYSTEM
	//}}
#ifdef SERV_INVISIBLE_GM
	DECL_ON_FUNC( ETR_TOGGLE_INVISIBLE_NOT );
#endif SERV_INVISIBLE_GM

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	DECL_ON_FUNC( ETR_SUMMON_RIDING_PET_NOT );
	DECL_ON_FUNC( ETR_UNSUMMON_RIDING_PET_NOT );
#endif	// SERV_RIDING_PET_SYSTM
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	DECL_ON_FUNC( ETR_COUPLE_MAKING_SUCCESS_NOT );
	DECL_ON_FUNC( ETR_RELATIONSHIP_EFFECT_NOT );
#endif SERV_RELATIONSHIP_SYSTEM
	//}


};

template < class T >
void KTRWorkerThread::BroadCast( const std::vector< UidType >& vecUID, u_short usEventID, const T& data )
{
	KncSecurity::KByteStream kStreamBuf;
	KTRUser::MakeByteStream( kStreamBuf, usEventID, data );

	KTRUserPtr spTRUser;
	for( unsigned int uiIdx = 0; uiIdx < vecUID.size(); ++uiIdx )
	{
		spTRUser = SiKTRServer()->GetTRUserManager().GetTRUser( vecUID[uiIdx] );
		if( spTRUser )
		{			
			spTRUser->SendPacket( kStreamBuf );
		}
		else
		{
			START_LOG( cwarn, L" Can't Find TRUser. UID : " << vecUID[uiIdx] );
			//<< BUILD_LOG( spData->m_vecUID.size() )
			//<< BUILD_LOG( spData->m_buff.size() );
		}
	}

	//SiKTRServer()->UpdateStatistics( kPacket_.m_vecUID.size() );
}



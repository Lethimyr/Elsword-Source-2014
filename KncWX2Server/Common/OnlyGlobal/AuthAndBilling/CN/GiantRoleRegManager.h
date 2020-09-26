#pragma once

#include "GiantBaseManager.h"
#include "GiantRoleRegPacket.h"

#ifdef SERV_COUNTRY_CN

#include <boost/timer.hpp>


class KGiantRoleRegManager : public KGiantBaseManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KGiantRoleRegManager );

protected:
	KGiantRoleRegManager();

public:
	virtual ~KGiantRoleRegManager();

	virtual void RegToLua();
	// derived from KThreadManager
	
	virtual KThread* CreateThread();
	
	void RegisterUserIDUnit( unsigned int uiGiantUID, KEGIANT_ROLEREG_CREATE_UNIT_REQ kPacket );
	void UnregisterUserIDUnit( unsigned int uiGiantUID, KEGIANT_ROLEREG_CREATE_UNIT_REQ& kPacket );
	void RegisterUserIDGuild( unsigned int uiGiantUID, KEGIANT_ROLEREG_CREATE_GUILD_REQ kPacket );
	void UnregisterUserIDGuild( unsigned int uiGiantUID, KEGIANT_ROLEREG_CREATE_GUILD_REQ& kPacket );

   	//void TickDeleteRole();
	//void SetDeleteRoleTime_LUA( const int iHour );
	//void SetNextDeleteRoleTime();

	template < class T > void SendToGameDB( unsigned short usEventID, const T& data );
	void SendToGameDB( unsigned short usEventID );
	template < class T > void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data ) const;

protected:

	virtual void MakeEventFromReceivedPacket();
	virtual void Handle_OnCommonPacket(KGiantCommonPacket& kReceivedPacket);
	void Handle_OnRoleRegPacket(KGiantRoleRegPacket& kReceivedPacket);

protected:
	mutable KncCriticalSection									m_csUserIDUnit;
	std::map< unsigned int, KEGIANT_ROLEREG_CREATE_UNIT_REQ >	m_mapUserIDUnit;
	mutable KncCriticalSection									m_csUserIDGuild;
	std::map< unsigned int, KEGIANT_ROLEREG_CREATE_GUILD_REQ >	m_mapUserIDGuild;

	CTime										m_tDeleteRoleTime;			// 삭제 패킷 전송 시간
	boost::timer								m_kRefreshTimer;
};

DefSingletonInline( KGiantRoleRegManager );

template < class T >
void KGiantRoleRegManager::SendToGameDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_USER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_GAME_DB, 0, NULL, usEventID, data );
}

template < class T >
void KGiantRoleRegManager::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( GetPfID(), 0, PI_GS_USER, nTo, anTrace, usEventID, data );
}

template < class T >
void KGiantRoleRegManager::SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data ) const
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_CHARACTER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToUnitUID ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"타 게임서버로 패킷 보내기 실패.? 왜왜" )
			<< BUILD_LOG( nToGSUID )
			<< BUILD_LOG( nToUnitUID )
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}

#endif // SERV_COUNTRY_CN
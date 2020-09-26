#pragma once

#include "GiantBaseManager.h"
#include "GiantInfoPacket.h"

#ifdef SERV_COUNTRY_CN

class KGiantInfoManager : public KGiantBaseManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KGiantInfoManager );

protected:
	KGiantInfoManager();

public:
	virtual ~KGiantInfoManager();

	virtual void RegToLua();

	// derived from KThreadManager
	virtual KThread* CreateThread();

protected:
	void MakeEventFromReceivedPacket();

	virtual void Handle_OnCommonPacket(KGiantCommonPacket& kReceivedPacket);
	void Handle_InfoServerPacket(KGiantInfoPacket& kReceivedPacket);
	void Handle_GMToolPacket(KGiantInfoPacket& kReceivedPacket);

protected:
	template < class T > void SendToGameDB( unsigned short usEventID, const T& data );
	void SendToGameDB( unsigned short usEventID );

protected:

};

DefSingletonInline( KGiantInfoManager );

template < class T >
void KGiantInfoManager::SendToGameDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_USER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_GAME_DB, 0, NULL, usEventID, data );
}

#endif // SERV_COUNTRY_CN
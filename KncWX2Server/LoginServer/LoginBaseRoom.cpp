#include "LoginBaseRoom.h"
#include "LoginRoomManager.h"

NiImplementRTTI( KLoginBaseRoom, KPerformer );
ImplPfID( KLoginBaseRoom, PI_LOGIN_ROOM );

ImplToStringW( KLoginBaseRoom )
{
	return START_TOSTRING_PARENTW( KPerformer );
}

KLoginBaseRoom::KLoginBaseRoom()
{
}

KLoginBaseRoom::~KLoginBaseRoom()
{
}

void KLoginBaseRoom::Tick()
{
	KPerformer::Tick();
}

void KLoginBaseRoom::SetUDPRelayInfo( const KNetAddress& kAddress )
{
	m_wstrUDPRelayIP = kAddress.m_wstrIP;
	m_usUDPRelayPort = kAddress.m_usPort;
}

void KLoginBaseRoom::CloseRoom()
{
	LIF( GetKLoginRoomManager()->CloseRoom( GetUID() ) );
	return;
}

void KLoginBaseRoom::SendToGSServer( UidType nTo, unsigned short usEventID )
{
	SendToGSServer( nTo, usEventID, char() );
}

void KLoginBaseRoom::SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID )
{
	SendToGSCharacter( nGS, nTo, usEventID, char() );
}

void KLoginBaseRoom::SendToLogDB( unsigned short usEventID )
{
    SendToLogDB( usEventID, char() );
}
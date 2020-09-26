#include "RoomManager.h"

ImplInstanceKeeping( KRoomManager );
NiImplementRootRTTI( KRoomManager );
#ifdef _CONVERT_VS_2010
#else
ImplementException( KRoomManager );
#endif _CONVERT_VS_2010

KRoomManager::KRoomManager()
{
}

KRoomManager::~KRoomManager()
{
}

ImplToStringW( KRoomManager )
{
	return stm_;
}

void KRoomManager::ShutDown()
{
	End();

	ReleaseKObj();
}
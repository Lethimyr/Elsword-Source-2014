#include ".\roomlist.h"
#include "NetError.h"
#include "GSFSM.h"

#include <boost/bind.hpp>

NiImplementRootRTTI( KRoomList );

KRoomList::KRoomList( UINT uiRoomListID, ROOM_LIST_TYPE eRoomListType, UINT uiPageNum ) :
m_uiRoomListID( uiRoomListID ),
m_eRoomListType( eRoomListType ),
m_uiPageNum( uiPageNum )
{
}

KRoomList::~KRoomList(void)
{
}

ImplOstmOperatorW( KRoomList );
ImplToStringW( KRoomList )
{
	return stm_ << GetRTTI()->GetName()
		<< L", RoomList ID-" << m_uiRoomListID
		<< std::endl;
}
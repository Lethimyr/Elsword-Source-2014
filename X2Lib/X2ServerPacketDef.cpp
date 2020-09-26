



/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CONNECT_REQ, obj, ks )
{
	return	PUT( gameMainVersion )
		&&	PUT( gameSubVersion )
		&&	PUT( gamePatchVersion );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CONNECT_REQ, obj, ks )
{
	return	GET( gameMainVersion )
		&&	GET( gameSubVersion )
		&&	GET( gamePatchVersion );
}


SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CONNECT_ACK, obj, ks )
{
	return	PUT( bResult ) 
		&&	PUT( errCode );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CONNECT_ACK, obj, ks )
{
	return	GET( bResult ) 
		&&	GET( errCode );
}


/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_NEW_USER_JOIN_REQ, obj, ks )
{
	return	PUT( ID ) 
		&&	PUT( password )
		&&	PUT( nickName )
		&&	PUT( name );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_NEW_USER_JOIN_REQ, obj, ks )
{
	return	GET( ID ) 
		&&	GET( password )
		&&	GET( nickName )
		&&	GET( name );
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_NEW_USER_JOIN_ACK, obj, ks )
{
	return	PUT( bResult ) 
		&&	PUT( errCode )
		&&	PUT( userUID );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_NEW_USER_JOIN_ACK, obj, ks )
{
	return	GET( bResult ) 
		&&	GET( errCode )
		&&	GET( userUID );
}

/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_LOGIN_REQ, obj, ks )
{
	return	PUT( ID ) 
		&&	PUT( password );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_LOGIN_REQ, obj, ks )
{
	return	GET( ID ) 
		&&	GET( password );
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_LOGIN_ACK, obj, ks )
{
	return	PUT( bResult ) 
		&&	PUT( errCode )
		&&	PUT( userUID );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_LOGIN_ACK, obj, ks )
{
	return	GET( bResult ) 
		&&	GET( errCode )
		&&	GET( userUID );

}

/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_USER_INFO_REQ, obj, ks )
{
	return	PUT( userUID );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_USER_INFO_REQ, obj, ks )
{
	return	GET( userUID );
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_USER_INFO_ACK, obj, ks )
{
	return	PUT( bResult ) 
		&&	PUT( errCode )
		&&	PUT( userUID )
		&&	PUT( userID )
		&&	PUT( userNickName )
		&&	PUT( maxUnitCount )
		&&	PUT( cash )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_USER_INFO_ACK, obj, ks )
{
	return	GET( bResult ) 
		&&	GET( errCode )
		&&	GET( userUID )
		&&	GET( userID )
		&&	GET( userNickName )
		&&	GET( maxUnitCount )
		&&	GET( cash )
		;
}

/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_UNIT_INFO_REQ, obj, ks )
{
	return	PUT( unitUID );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_UNIT_INFO_REQ, obj, ks )
{
	return	GET( unitUID );
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_UNIT_INFO_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( userUID )
		&&	PUT( unitUID )
		&&	PUT( unitClass )
		&&	PUT( IP )
		&&	PUT( nickName )
		&&	PUT( GP )
		&&	PUT( level )
		&&	PUT( EXP )
		&&	PUT( inventorySize )
		&&	PUT( quickSlotSize )
		&&	PUT( Str )
		&&	PUT( Dex )
		&&	PUT( Int )
		&&	PUT( Vit )
		&&	PUT( Luck )
		&&	PUT( remainStatPoint )
		&&	PUT( nowEqipItemUIDList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_UNIT_INFO_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( userUID )
		&&	GET( unitUID )
		&&	GET( unitClass )
		&&	GET( IP )
		&&	GET( nickName )
		&&	GET( GP )
		&&	GET( level )
		&&	GET( EXP )
		&&	GET( inventorySize )
		&&	GET( quickSlotSize )
		&&	GET( Str )
		&&	GET( Dex )
		&&	GET( Int )
		&&	GET( Vit )
		&&	GET( Luck )
		&&	GET( remainStatPoint )
		&&	GET( nowEqipItemUIDList )
		;
}


/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_UNIT_INFO_LIST_REQ, obj, ks )
{
	return	PUT( userUID );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_UNIT_INFO_LIST_REQ, obj, ks )
{
	return	GET( userUID );
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_UNIT_INFO_LIST_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( unitInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_UNIT_INFO_LIST_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( unitInfoList )
		;
}


/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_ITEM_INFO_REQ, obj, ks )
{
	return	PUT( itemUID );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_ITEM_INFO_REQ, obj, ks )
{
	return	GET( itemUID );
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_ITEM_INFO_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( itemUID )
		&&	PUT( itemID )
		&&	PUT( ownerUserUID )
		&&	PUT( ownerUnitUID )
		&&	PUT( hyperGage )
		&&	PUT( endurance )
		&&	PUT( quantity )
		&&	PUT( enchantAttribute )
		&&	PUT( enchantLevel )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_ITEM_INFO_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( itemUID )
		&&	GET( itemID )
		&&	GET( ownerUserUID )
		&&	GET( ownerUnitUID )
		&&	GET( hyperGage )
		&&	GET( endurance )
		&&	GET( quantity )
		&&	GET( enchantAttribute )
		&&	GET( enchantLevel )
		;
}


/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_UNIT_INVENTORY_ITEM_LIST_REQ, obj, ks )
{
	return	PUT( unitUID );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_UNIT_INVENTORY_ITEM_LIST_REQ, obj, ks )
{
	return	GET( unitUID );
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_UNIT_INVENTORY_ITEM_LIST_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( inventoryItemList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_UNIT_INVENTORY_ITEM_LIST_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( inventoryItemList )
		;
}


/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_STATE_CHANGE_UNIT_SELECT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_STATE_CHANGE_UNIT_SELECT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}


/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_STATE_CHANGE_CREATE_UNIT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_STATE_CHANGE_CREATE_UNIT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}


/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_UNIT_SELECT_REQ, obj, ks )
{
	return	PUT( unitUID );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_UNIT_SELECT_REQ, obj, ks )
{
	return	GET( unitUID );
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_UNIT_SELECT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( unitUID )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_UNIT_SELECT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( unitUID )
		;
}

/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CREATE_UNIT_REQ, obj, ks )
{
	return	PUT( nickName )
		&&	PUT( unitClass )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CREATE_UNIT_REQ, obj, ks )
{
	return	GET( nickName )
		&&	GET( unitClass )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CREATE_UNIT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( unitUID )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CREATE_UNIT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( unitUID )
		;
}

/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DELETE_UNIT_REQ, obj, ks )
{
	return	PUT( unitUID );
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DELETE_UNIT_REQ, obj, ks )
{
	return	GET( unitUID );
}

SERIALIZE_DEFINE_PUT( KDB_EVENT_X2_SERVER_DELETE_UNIT_REQ, obj, ks )
{
	return	PUT( userUID )
		&&	PUT( unitUID )
		;
}

SERIALIZE_DEFINE_GET( KDB_EVENT_X2_SERVER_DELETE_UNIT_REQ, obj, ks )
{
	return	GET( userUID )
		&&	GET( unitUID )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DELETE_UNIT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( unitUID )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DELETE_UNIT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( unitUID )
		;
}


/*
/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_STATE_CHANGE_MAIN_LOBBY_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_STATE_CHANGE_MAIN_LOBBY_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}
*/

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_STATE_CHANGE_VILLAGE_MAP_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_STATE_CHANGE_VILLAGE_MAP_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}
////////////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_STATE_CHANGE_LOCAL_MAP_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_STATE_CHANGE_LOCAL_MAP_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}
////////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_STATE_CHANGE_WORLD_MAP_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_STATE_CHANGE_WORLD_MAP_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}




/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_CHANNEL_INFO, obj, ks )
{
	return	PUT( ID )
		&&	PUT( name )
		&&	PUT( minLevel )
		&&	PUT( maxLevel )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_CHANNEL_INFO, obj, ks )
{
	return	GET( ID )
		&&	GET( name )
		&&	GET( minLevel )
		&&	GET( maxLevel )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_GET_PVP_CHANNEL_INFO_LIST_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( channelInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_GET_PVP_CHANNEL_INFO_LIST_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( channelInfoList )
		;
}



/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_JOIN_PVP_CHANNEL_REQ, obj, ks )
{
	return	PUT( bInitChannel )
		&&	PUT( channelID )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_JOIN_PVP_CHANNEL_REQ, obj, ks )
{
	return	GET( bInitChannel )
		&&	GET( channelID )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_JOIN_PVP_CHANNEL_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( channelID )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_JOIN_PVP_CHANNEL_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( channelID )
		;
}




/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_STATE_CHANGE_PVP_LOBBY_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_STATE_CHANGE_PVP_LOBBY_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}




/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_GET_PVP_ROOM_SIMPLE_INFO_LIST_REQ, obj, ks )
{
	return	PUT( pageNum )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_GET_PVP_ROOM_SIMPLE_INFO_LIST_REQ, obj, ks )
{
	return	GET( pageNum )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_ROOM_SIMPLE_INFO, obj, ks )
{
	return	PUT( roomNum )
		&&	PUT( roomName )
		&&	PUT( roomState )
		&&	PUT( gameType )
		&&	PUT( worldID )
		&&	PUT( maxSlot )
		&&	PUT( joinSlot )
		&&	PUT( winGameNum )
		&&	PUT( winKillNum )
		&&	PUT( bPossibleIntrude )
		&&	PUT( bIsOpen )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_ROOM_SIMPLE_INFO, obj, ks )
{
	return	GET( roomNum )
		&&	GET( roomName )
		&&	GET( roomState )
		&&	GET( gameType )
		&&	GET( worldID )
		&&	GET( maxSlot )
		&&	GET( joinSlot )
		&&	GET( winGameNum )
		&&	GET( winKillNum )
		&&	GET( bPossibleIntrude )
		&&	GET( bIsOpen )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_GET_PVP_ROOM_SIMPLE_INFO_LIST_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( maxPageNum )
		&&	PUT( roomSimpleInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_GET_PVP_ROOM_SIMPLE_INFO_LIST_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( maxPageNum )
		&&	GET( roomSimpleInfoList )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_GET_PVP_ROOM_SIMPLE_INFO_LIST_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( maxPageNum )
		&&	PUT( roomSimpleInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_GET_PVP_ROOM_SIMPLE_INFO_LIST_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( maxPageNum )
		&&	GET( roomSimpleInfoList )
		;
}









/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_PVP_ROOM_INFO, obj, ks )
{
	return	PUT( roomNum )
		&&	PUT( roomName )
		&&	PUT( hostSlot )
		&&	PUT( roomState )
		&&	PUT( gameType )
		&&	PUT( winGameNum )
		&&	PUT( winKillNum )
		&&	PUT( worldID )
		&&	PUT( bPossibleIntrude )
		&&	PUT( bIsOpen )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_PVP_ROOM_INFO, obj, ks )
{
	return	GET( roomNum )
		&&	GET( roomName )
		&&	GET( hostSlot )
		&&	GET( roomState )
		&&	GET( gameType )
		&&	GET( winGameNum )
		&&	GET( winKillNum )
		&&	GET( worldID )
		&&	GET( bPossibleIntrude )
		&&	GET( bIsOpen )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_SLOT_INFO, obj, ks )
{
	return	PUT( slotState )
		&&	PUT( slotEvent )
		&&	PUT( teamNum )
		&&	PUT( slotUnitSpecialMotion )
		&&	PUT( unitInfo )
		&&	PUT( eqipItemList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_SLOT_INFO, obj, ks )
{
	return	GET( slotState )
		&&	GET( slotEvent )
		&&	GET( teamNum )
		&&	GET( slotUnitSpecialMotion )
		&&	GET( unitInfo )
		&&	GET( eqipItemList )
		;
}







/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CREATE_PVP_ROOM_REQ, obj, ks )
{
	return	PUT( roomName )
		&&	PUT( gameMode )
		&&	PUT( winGameNum )
		&&	PUT( winKillNum )
		&&	PUT( unitNum )
		&&	PUT( bIsOpen )
		&&	PUT( RoomPasswd )
		&&  PUT( bPossibleIntrude )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CREATE_PVP_ROOM_REQ, obj, ks )
{
	return	GET( roomName )
		&&	GET( gameMode )
		&&	GET( winGameNum )
		&&	GET( winKillNum )
		&&	GET( unitNum )
		&&	GET( bIsOpen )
		&&	GET( RoomPasswd )
		&&  GET( bPossibleIntrude )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CREATE_PVP_ROOM_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( roomInfo )
		&&	PUT( slotInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CREATE_PVP_ROOM_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( roomInfo )
		&&	GET( slotInfoList )
		;
}






/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_JOIN_PVP_ROOM_REQ, obj, ks )
{
	return	PUT( roomNum )
		&&	PUT( RoomPasswd )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_JOIN_PVP_ROOM_REQ, obj, ks )
{
	return	GET( roomNum )
		&&	GET( RoomPasswd )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_JOIN_PVP_ROOM_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( roomInfo )
		&&	PUT( slotInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_JOIN_PVP_ROOM_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( roomInfo )
		&&	GET( slotInfoList )
		;
}




/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_EXIT_PVP_ROOM_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_EXIT_PVP_ROOM_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}








/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_INFO_REQ, obj, ks )
{
	return	PUT( changeFlag )
		&&  PUT( nChangeData )
		&&  PUT( bPossibleIntrude )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_INFO_REQ, obj, ks )
{
	return	GET( changeFlag )
		&&  GET( nChangeData )
		&&  GET( bPossibleIntrude )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_INFO_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_INFO_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}





/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_SLOT_INFO_REQ, obj, ks )
{
	return	PUT( slotInfo )
		&&  PUT( slotChangeFlag )
		&&  PUT( slotIndex )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_SLOT_INFO_REQ, obj, ks )
{
	return	GET( slotInfo )
		&&  GET( slotChangeFlag )
		&&  GET( slotIndex )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_SLOT_INFO_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_SLOT_INFO_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}






/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_INFO_NOT, obj, ks )
{
	return	PUT( roomInfo )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_INFO_NOT, obj, ks )
{
	return	GET( roomInfo )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_SLOT_INFO_LIST_NOT, obj, ks )
{
	return	PUT( slotIndex )
		&&	PUT( slotInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_SLOT_INFO_LIST_NOT, obj, ks )
{
	return	GET( slotIndex )
		&&	GET( slotInfoList )
		;
}









/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_PVP_ROOM_GAME_START_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_PVP_ROOM_GAME_START_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_START_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_START_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}






/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_PVP_ROOM_GAME_HOST_READY_REQ, obj, ks )
{
	return	PUT( port )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_PVP_ROOM_GAME_HOST_READY_REQ, obj, ks )
{
	return	GET( port )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_PVP_ROOM_GAME_HOST_READY_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_PVP_ROOM_GAME_HOST_READY_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_HOST_READY_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( IP )
		&&	PUT( port )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_HOST_READY_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( IP )
		&&	GET( port )
		;
}






/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_PVP_ROOM_GAME_LOADING_COMPLETE_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_PVP_ROOM_GAME_LOADING_COMPLETE_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_ALL_UNIT_LOADING_COMPLETE_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_ALL_UNIT_LOADING_COMPLETE_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

//////////////////////////////////////////////////////////////////////////
//PVP GAME 로딩현황
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_ONE_UNIT_LOADING_COMPLETE_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( slotIndex )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_ONE_UNIT_LOADING_COMPLETE_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( slotIndex )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_PVP_GAME_RESULT_REQ, obj, ks )
{
	return	PUT( winTeam )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_PVP_GAME_RESULT_REQ, obj, ks )
{
	return	GET( winTeam )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_PVP_GAME_RESULT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_PVP_GAME_RESULT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_PVP_GAME_RESULT_NOT, obj, ks )
{
	return	PUT( winTeam )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_PVP_GAME_RESULT_NOT, obj, ks )
{
	return	GET( winTeam )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_STATE_CHANGE_PVP_GAME_RESULT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_STATE_CHANGE_PVP_GAME_RESULT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_STATE_CHANGE_PVP_GAME_RESULT_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( slotList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_STATE_CHANGE_PVP_GAME_RESULT_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( slotList )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_EXIT_PVP_GAME_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_EXIT_PVP_GAME_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_PVP_ROOM_CHAT_REQ, obj, ks )
{
	return	PUT( MsgType )
		&&	PUT( mySlotIndex )
		&&	PUT( strMsg )
		&&	PUT( toSlotIndex )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_PVP_ROOM_CHAT_REQ, obj, ks )
{
	return	GET( MsgType )
		&&	GET( mySlotIndex )
		&&	GET( strMsg )
		&&	GET( toSlotIndex )
		;
}
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_PVP_ROOM_CHAT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_PVP_ROOM_CHAT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_CHAT_NOT, obj, ks )
{
	return	PUT( slotIndex )
		&&	PUT( strMsg )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_CHAT_NOT, obj, ks )
{
	return	GET( slotIndex )
		&&	GET( strMsg )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_PVP_GAME_CHAT_REQ, obj, ks )
{
	return	PUT( MsgType )
		&&	PUT( mySlotIndex )
		&&	PUT( strMsg )
		&&	PUT( toSlotIndex )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_PVP_GAME_CHAT_REQ, obj, ks )
{
	return	GET( MsgType )
		&&	GET( mySlotIndex )
		&&	GET( strMsg )
		&&	GET( toSlotIndex )
		;
}
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_PVP_GAME_CHAT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_PVP_GAME_CHAT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_PVP_GAME_CHAT_NOT, obj, ks )
{
	return	PUT( slotIndex )
		&&	PUT( strMsg )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_PVP_GAME_CHAT_NOT, obj, ks )
{
	return	GET( slotIndex )
		&&	GET( strMsg )
		;
}


////////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_RELAY, obj, ks )
{
	return	PUT( size )
		&&	ks.PutRaw( &obj.buffer, sizeof(char) * 300 )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_RELAY, obj, ks )
{
	return	GET( size )
		&&	ks.GetRaw( &obj.buffer, sizeof(char) * 300 )
		;
}

//////////////////////////////////////////////////////////////////////////
//SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_END_PVP_GAME_REQ, obj, ks )
//{
//	return	PUT( winTeam )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_END_PVP_GAME_REQ, obj, ks )
//{
//	return	GET( winTeam )
//		;
//}
//
//SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_END_PVP_GAME_ACK, obj, ks )
//{
//	return	PUT( bResult )
//		&&	PUT( errCode )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_END_PVP_GAME_ACK, obj, ks )
//{
//	return	GET( bResult )
//		&&	GET( errCode )
//		;
//}
//
//SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_END_PVP_GAME_NOT, obj, ks )
//{
//	return	PUT( bResult )
//		&&	PUT( errCode )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_END_PVP_GAME_NOT, obj, ks )
//{
//	return	GET( bResult )
//		&&	GET( errCode )
//		;
//}

//////////////////////////////////////////////////////////////////////////
//SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_STATE_CHANGE_PVP_ROOM_ACK, obj, ks )
//{
//	return	GET( bResult )
//		&&	GET( errCode )
//		;
//}



// DUNGEON_TEST!!
/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_GET_DUNGEON_CHANNEL_INFO_LIST_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( channelInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_GET_DUNGEON_CHANNEL_INFO_LIST_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( channelInfoList )
		;
}



/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_JOIN_DUNGEON_CHANNEL_REQ, obj, ks )
{
	return	PUT( bInitChannel )
		&&	PUT( channelID )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_JOIN_DUNGEON_CHANNEL_REQ, obj, ks )
{
	return	GET( bInitChannel )
		&&	GET( channelID )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_JOIN_DUNGEON_CHANNEL_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( channelID )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_JOIN_DUNGEON_CHANNEL_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( channelID )
		;
}




/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_STATE_CHANGE_DUNGEON_LOBBY_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_STATE_CHANGE_DUNGEON_LOBBY_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}




/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_REQ, obj, ks )
{
	return	PUT( pageNum )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_REQ, obj, ks )
{
	return	GET( pageNum )
		;
}


SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_ROOM_SIMPLE_INFO, obj, ks )
{
	return PUT( roomNum )
		&& PUT( roomName )
		&& PUT( roomState )
		&& PUT( difficultyLevel )
		&& PUT( dungeonID )
		&& PUT( maxSlot )
		&& PUT( joinSlot )
		&& PUT( bIsOpen )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_ROOM_SIMPLE_INFO, obj, ks )
{
	return GET( roomNum )
		&& GET( roomName )
		&& GET( roomState )
		&& GET( difficultyLevel )
		&& GET( dungeonID )
		&& GET( maxSlot )
		&& GET( joinSlot )
		&& GET( bIsOpen )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( maxPageNum )
		&&	PUT( roomSimpleInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( maxPageNum )
		&&	GET( roomSimpleInfoList )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( maxPageNum )
		&&	PUT( roomSimpleInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( maxPageNum )
		&&	GET( roomSimpleInfoList )
		;
}





/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_ROOM_INFO, obj, ks )
{
	return PUT( roomNum )
		&& PUT( roomName )
		&& PUT( roomState )
		&& PUT( difficultyLevel )
		&& PUT( dungeonID )
		&& PUT( bIsOpen )
		&& PUT( bViewComics )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_ROOM_INFO, obj, ks )
{
	return GET( roomNum )
		&& GET( roomName )
		&& GET( roomState )
		&& GET( difficultyLevel )
		&& GET( dungeonID )
		&& GET( bIsOpen )
		&& GET( bViewComics )
		;
}

/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CREATE_DUNGEON_ROOM_REQ, obj, ks )
{
	return	PUT( roomName )
		//&&	PUT( gameMode )
		&&	PUT( difficultyLevel )
		&&	PUT( dungeonID )
		&&	PUT( unitNum )
		&&	PUT( bIsOpen )
		&&	PUT( RoomPasswd )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CREATE_DUNGEON_ROOM_REQ, obj, ks )
{
	return	GET( roomName )
		//&&	GET( gameMode )
		&&	GET( difficultyLevel )
		&&	GET( dungeonID )
		&&	GET( unitNum )
		&&	GET( bIsOpen )
		&&	GET( RoomPasswd )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CREATE_DUNGEON_ROOM_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( roomInfo )
		&&	PUT( slotInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CREATE_DUNGEON_ROOM_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( roomInfo )
		&&	GET( slotInfoList )
		;
}






/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_JOIN_DUNGEON_ROOM_REQ, obj, ks )
{
	return	PUT( roomNum )
		&&	PUT( RoomPasswd )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_JOIN_DUNGEON_ROOM_REQ, obj, ks )
{
	return	GET( roomNum )
		&&	GET( RoomPasswd )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_JOIN_DUNGEON_ROOM_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( roomInfo )
		&&	PUT( slotInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_JOIN_DUNGEON_ROOM_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( roomInfo )
		&&	GET( slotInfoList )
		;
}




/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_EXIT_DUNGEON_ROOM_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_EXIT_DUNGEON_ROOM_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}








/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_INFO_REQ, obj, ks )
{
	return	PUT( changeFlag )
		&&  PUT( nChangeData )
		//&&  PUT( bPossibleIntrude )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_INFO_REQ, obj, ks )
{
	return	GET( changeFlag )
		&&  GET( nChangeData )
		//&&  GET( bPossibleIntrude )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_INFO_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_INFO_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}





/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_SLOT_INFO_REQ, obj, ks )
{
	return	PUT( slotInfo )
		&&  PUT( slotChangeFlag )
		&&  PUT( slotIndex )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_SLOT_INFO_REQ, obj, ks )
{
	return	GET( slotInfo )
		&&  GET( slotChangeFlag )
		&&  GET( slotIndex )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_SLOT_INFO_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_SLOT_INFO_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}






/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_INFO_NOT, obj, ks )
{
	return	PUT( roomInfo )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_INFO_NOT, obj, ks )
{
	return	GET( roomInfo )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_SLOT_INFO_LIST_NOT, obj, ks )
{
	return	PUT( slotIndex )
		&&	PUT( slotInfoList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_SLOT_INFO_LIST_NOT, obj, ks )
{
	return	GET( slotIndex )
		&&	GET( slotInfoList )
		;
}









/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_START_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_START_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_START_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_START_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}






/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_HOST_READY_REQ, obj, ks )
{
	return	PUT( port )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_HOST_READY_REQ, obj, ks )
{
	return	GET( port )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_HOST_READY_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_HOST_READY_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_HOST_READY_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( IP )
		&&	PUT( port )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_HOST_READY_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( IP )
		&&	GET( port )
		;
}






/////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_LOADING_COMPLETE_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_LOADING_COMPLETE_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_ALL_UNIT_LOADING_COMPLETE_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_ALL_UNIT_LOADING_COMPLETE_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

//////////////////////////////////////////////////////////////////////////
//DUNGEON GAME 로딩현황
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_ONE_UNIT_LOADING_COMPLETE_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( slotIndex )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_ONE_UNIT_LOADING_COMPLETE_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( slotIndex )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_GAME_RESULT_REQ, obj, ks )
{
	return	PUT( winTeam )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_GAME_RESULT_REQ, obj, ks )
{
	return	GET( winTeam )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_GAME_RESULT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_GAME_RESULT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_DUNGEON_GAME_RESULT_NOT, obj, ks )
{
	return	PUT( winTeam )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_DUNGEON_GAME_RESULT_NOT, obj, ks )
{
	return	GET( winTeam )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_STATE_CHANGE_DUNGEON_GAME_RESULT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_STATE_CHANGE_DUNGEON_GAME_RESULT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_STATE_CHANGE_DUNGEON_GAME_RESULT_NOT, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		&&	PUT( slotList )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_STATE_CHANGE_DUNGEON_GAME_RESULT_NOT, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		&&	GET( slotList )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_EXIT_DUNGEON_GAME_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_EXIT_DUNGEON_GAME_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_ROOM_CHAT_REQ, obj, ks )
{
	return	PUT( MsgType )
		&&	PUT( mySlotIndex )
		&&	PUT( strMsg )
		&&	PUT( toSlotIndex )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_ROOM_CHAT_REQ, obj, ks )
{
	return	GET( MsgType )
		&&	GET( mySlotIndex )
		&&	GET( strMsg )
		&&	GET( toSlotIndex )
		;
}
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_ROOM_CHAT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_ROOM_CHAT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_CHAT_NOT, obj, ks )
{
	return	PUT( slotIndex )
		&&	PUT( strMsg )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_CHAT_NOT, obj, ks )
{
	return	GET( slotIndex )
		&&	GET( strMsg )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_GAME_CHAT_REQ, obj, ks )
{
	return	PUT( MsgType )
		&&	PUT( mySlotIndex )
		&&	PUT( strMsg )
		&&	PUT( toSlotIndex )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_GAME_CHAT_REQ, obj, ks )
{
	return	GET( MsgType )
		&&	GET( mySlotIndex )
		&&	GET( strMsg )
		&&	GET( toSlotIndex )
		;
}
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_DUNGEON_GAME_CHAT_ACK, obj, ks )
{
	return	PUT( bResult )
		&&	PUT( errCode )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_DUNGEON_GAME_CHAT_ACK, obj, ks )
{
	return	GET( bResult )
		&&	GET( errCode )
		;
}
SERIALIZE_DEFINE_PUT( KEVENT_X2_SERVER_BROADCAST_DUNGEON_GAME_CHAT_NOT, obj, ks )
{
	return	PUT( slotIndex )
		&&	PUT( strMsg )
		;
}

SERIALIZE_DEFINE_GET( KEVENT_X2_SERVER_BROADCAST_DUNGEON_GAME_CHAT_NOT, obj, ks )
{
	return	GET( slotIndex )
		&&	GET( strMsg )
		;
}







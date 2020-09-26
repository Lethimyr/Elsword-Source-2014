#pragma once

class CX2PacketLog
{
public:
	CX2PacketLog(void);
	virtual ~CX2PacketLog(void);

	static void SetEnable( bool bEnable ) { m_bEnable = bEnable; }	

	// CommonPacket
	static void PrintLog( KNetAddress* pPacket );
	static void PrintLog( KItemInfo* pPacket );
	static void PrintLog( KUnitInfo* pPacket );
	static void PrintLog( KRoomInfo* pPacket );
	static void PrintLog( KRoomUserInfo* pPacket );
	static void PrintLog( KRoomSlotInfo* pPacket );
	//static void PrintLog( KECN_VERIFY_SERVER_ACK* pPacket );
	static void PrintLog( KPacketOK* pPacket );
	//static void PrintLog( KEGDB_CREATE_UNIT_REQ* pPacket );
	static void PrintLog( KERM_OPEN_ROOM_REQ* pPacket );
// 	static void PrintLog( KERM_JOIN_ROOM_REQ* pPacket );
// 	static void PrintLog( KERM_LEAVE_ROOM_REQ* pPacket );
//	static void PrintLog( KERM_ROOM_LIST_INFO_NOT* pPacket );
	static void PrintLog( KDBE_UPDATE_UNIT_INFO_REQ* pPacket );
	static void PrintLog( KDBE_UPDATE_UNIT_INFO_ACK* pPacket );

	// ClientPacket
	static void PrintLog( KEGS_CONNECT_REQ* pPacket );
	static void PrintLog( KEGS_NEW_USER_JOIN_REQ* pPacket );
	static void PrintLog( KEGS_NEW_USER_JOIN_ACK* pPacket );
	static void PrintLog( KEGS_VERIFY_ACCOUNT_REQ* pPacket );
	static void PrintLog( KEGS_VERIFY_ACCOUNT_ACK* pPacket );
	static void PrintLog( KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK* pPacket );
	static void PrintLog( KEGS_CREATE_UNIT_REQ* pPacket );
	static void PrintLog( KEGS_CREATE_UNIT_ACK* pPacket );
	static void PrintLog( KEGS_DELETE_UNIT_REQ* pPacket );
	static void PrintLog( KEGS_DELETE_UNIT_ACK* pPacket );
	static void PrintLog( KEGS_SELECT_UNIT_REQ* pPacket );
	static void PrintLog( KEGS_SELECT_UNIT_ACK* pPacket );
	static void PrintLog( KEGS_STATE_CHANGE_SERVER_SELECT_ACK* pPacket );
	static void PrintLog( KServerSetData *pPacket );
	static void PrintLog( KEGS_GET_SERVER_SET_DATA_ACK* pPacket );
	static void PrintLog( KEGS_SELECT_SERVER_SET_REQ* pPacket );
	static void PrintLog( KEGS_SELECT_SERVER_SET_ACK* pPacket );
	static void PrintLog( KStateChangeReq* pPacket );
	static void PrintLog( KStateChangeAck* pPacket );
	//static void PrintLog( KEGS_ENTER_DUNGEON_CHANNEL_REQ* pPacket );
	static void PrintLog( KEGS_ROOM_LIST_REQ* pPacket );
	static void PrintLog( KEGS_ROOM_LIST_ACK* pPacket );
	static void PrintLog( KEGS_CREATE_ROOM_REQ* pPacket );
	static void PrintLog( KEGS_CREATE_ROOM_ACK* pPacket );
	static void PrintLog( KEGS_JOIN_ROOM_REQ* pPacket );
	static void PrintLog( KEGS_JOIN_ROOM_ACK* pPacket );
	static void PrintLog( KEGS_JOIN_ROOM_NOT* pPacket );
	static void PrintLog( KEGS_LEAVE_ROOM_NOT* pPacket );
	static void PrintLog( KEGS_LEAVE_GAME_NOT* pPacket );
	static void PrintLog( KEGS_BAN_USER_REQ* pPacket );
	static void PrintLog( KEGS_CHANGE_TEAM_REQ* pPacket );
	static void PrintLog( KEGS_CHANGE_TEAM_ACK* pPacket );
	static void PrintLog( KEGS_CHANGE_TEAM_NOT* pPacket );
	static void PrintLog( KEGS_CHANGE_READY_REQ* pPacket );
	static void PrintLog( KEGS_CHANGE_READY_ACK* pPacket );
	static void PrintLog( KEGS_CHANGE_READY_NOT* pPacket );
	static void PrintLog( KEGS_CHANGE_MAP_REQ* pPacket );
	static void PrintLog( KEGS_CHANGE_MAP_ACK* pPacket );
	static void PrintLog( KEGS_CHANGE_MAP_NOT* pPacket );
	static void PrintLog( KEGS_CHANGE_SLOT_OPEN_REQ* pPacket );
	static void PrintLog( KEGS_CHANGE_SLOT_OPEN_ACK* pPacket );
	static void PrintLog( KEGS_CHANGE_SLOT_OPEN_NOT* pPacket );
	static void PrintLog( KEGS_CHANGE_PLAY_TIME_LIMIT_REQ* pPacket );
	static void PrintLog( KEGS_CHANGE_PLAY_TIME_LIMIT_NOT* pPacket );
	static void PrintLog( KEGS_CHANGE_WINNING_NUM_KILL_REQ* pPacket );
	static void PrintLog( KEGS_CHANGE_WINNING_NUM_KILL_NOT* pPacket );
	static void PrintLog( KEGS_CHAT_REQ* pPacket );
	//static void PrintLog( KEGS_CHAT_ACK* pPacket );
	static void PrintLog( KEGS_CHAT_NOT* pPacket );
	static void PrintLog( KEGS_STATE_CHANGE_GAME_START_NOT* pPacket );
	static void PrintLog( KEGS_STATE_CHANGE_GAME_INTRUDE_NOT* pPacket );
	static void PrintLog( KEGS_GAME_LOADING_REQ* pPacket );
	static void PrintLog( KEGS_GAME_LOADING_NOT* pPacket );
	static void PrintLog( KEGS_PLAY_START_NOT* pPacket );
	static void PrintLog( KEGS_INTRUDE_START_REQ* pPacket );
	static void PrintLog( KEGS_INTRUDE_START_ACK* pPacket );
	static void PrintLog( KEGS_INTRUDE_START_NOT* pPacket );
	static void PrintLog( KEGS_CURRENT_KILL_SCORE_NOT* pPacket );
	static void PrintLog( KEGS_MY_USER_UNIT_INFO_TO_SERVER_REQ* pPacket );
	static void PrintLog( KEGS_USER_UNIT_RE_BIRTH_POS_REQ* pPacket );
	static void PrintLog( KEGS_USER_UNIT_RE_BIRTH_NOT* pPacket );
	//static void PrintLog( KChannelReq* pPacket );
	//static void PrintLog( KChannelAck* pPacket );
	static void PrintLog( KEGS_USER_UNIT_DIE_REQ* pPacket );
	static void PrintLog( KEGS_USER_UNIT_DIE_ACK* pPacket );
	static void PrintLog( KEGS_USER_UNIT_DIE_NOT* pPacket );
	static void PrintLog( VECTOR3* pPacket );
	static void PrintLog( KNPCUnitReq* pPacket );
	static void PrintLog( KEGS_NPC_UNIT_CREATE_REQ* pPacket );
	static void PrintLog( KNPCUnitNot* pPacket );
	static void PrintLog( KEGS_NPC_UNIT_CREATE_NOT* pPacket );
	static void PrintLog( KEGS_NPC_UNIT_DIE_REQ* pPacket );
	static void PrintLog( KEGS_NPC_UNIT_DIE_NOT* pPacket );
	static void PrintLog( KEGS_DUNGEON_STAGE_LOAD_REQ* pPacket );
	static void PrintLog( KEGS_DUNGEON_STAGE_LOAD_NOT* pPacket );
	static void PrintLog( KEGS_DUNGEON_STAGE_LOAD_COMPLETE_NOT* pPacket );
	static void PrintLog( KEGS_DUNGEON_STAGE_START_NOT* pPacket );

	static void PrintLog( KEGS_DUNGEON_SUB_STAGE_OPEN_REQ* pPacket );
//	static void PrintLog( KEGS_DUNGEON_SUB_STAGE_OPEN_ACK* pPacket );
	static void PrintLog( KEGS_DUNGEON_SUB_STAGE_OPEN_NOT* pPacket );
	static void PrintLog( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ* pPacket );
//	static void PrintLog( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK* pPacket );
	static void PrintLog( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT* pPacket );

	static void PrintLog( KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ* pPacket );
//	static void PrintLog( KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK* pPacket );
	static void PrintLog( KEGS_DUNGEON_SUB_STAGE_START_NOT* pPacket );

	static void PrintLog( KDungeonNextStageData* pPacket );
	static void PrintLog( KEGS_END_GAME_REQ* pPacket );
	static void PrintLog( KEGS_END_GAME_NOT* pPacket );
	static void PrintLog( KEGS_STATE_CHANGE_RESULT_REQ* pPacket );
	static void PrintLog( KEGS_STATE_CHANGE_RESULT_ACK* pPacket );
	static void PrintLog( KEGS_STATE_CHANGE_RESULT_NOT* pPacket );
	
	static void PrintLog( int* pPacket );
	static void PrintLog( float* pPacket );

private:
	static bool m_bEnable;

};


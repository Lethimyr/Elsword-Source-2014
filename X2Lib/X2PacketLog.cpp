#include "StdAfx.h"
#include ".\x2packetlog.h"


bool CX2PacketLog::m_bEnable = true;



CX2PacketLog::CX2PacketLog(void)
{
}

CX2PacketLog::~CX2PacketLog(void)
{
}


//////////////////////////////////////////////////////////////////////////
// CommonPacket
void CX2PacketLog::PrintLog( KNetAddress* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KNetAddress : " << dbg::endl 
		<< L" m_wstrIP: " << pPacket->m_wstrIP								<< dbg::endl
		<< L" m_usPort: " << pPacket->m_usPort								<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KItemInfo* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KItemInfo : " << dbg::endl 
		<< L" m_iItemID			: " << pPacket->m_iItemID							<< dbg::endl
//		<< L" m_iItemUID		: " << pPacket->m_iItemUID							<< dbg::endl
        << L" m_iUsageType      : " << pPacket->m_cUsageType
		<< L" m_iQuantity		: " << pPacket->m_iQuantity							<< dbg::endl;
//		<< L" m_iEndurance		: " << pPacket->m_iEndurance						<< dbg::endl
//		<< L" m_iEType			: " << pPacket->m_iEType							<< dbg::endl
//		<< L" m_iELevel			: " << pPacket->m_iELevel							<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KUnitInfo* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KUnitInfo : " << dbg::endl 
		<< L" m_iOwnerUserUID	: " << pPacket->m_iOwnerUserUID						<< dbg::endl
		<< L" m_nUnitUID			: " << pPacket->m_nUnitUID								<< dbg::endl
		//<< L" m_iUnitClass		: " << pPacket->m_iUnitClass						<< dbg::endl
		<< L" m_wstrNickName		: " << pPacket->m_wstrNickName					<< dbg::endl
		<< L" m_wstrIP		: " << pPacket->m_wstrIP								<< dbg::endl
		<< L" m_nPort		: " << pPacket->m_usPort								<< dbg::endl
		<< L" m_iED			: " << pPacket->m_iED									<< dbg::endl
		<< L" m_ucLevel		: " << (int)pPacket->m_ucLevel							<< dbg::endl
		<< L" m_iEXP			: " << pPacket->m_iEXP								<< dbg::endl
		<< L" m_nStraightVictories	: " << pPacket->m_nStraightVictories			<< dbg::endl

		<< L" m_iSPoint	: " << (int)pPacket->m_iSPoint					<< dbg::endl;

	
// 	dbg::clog 
// 		<< L" m_iMapID : "  << pPacket->m_iMapID							<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KRoomInfo* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KRoomInfo : " << dbg::endl 
		<< L" m_RoomType	: " << g_pKTDXApp->GetEnumToString( L"ROOM_TYPE", (int)pPacket->m_RoomType ) << dbg::endl		
		<< L" m_RoomUID	: " << pPacket->m_RoomUID						<< dbg::endl
		//<< L" m_uiChannelID	: " << pPacket->m_uiChannelID				<< dbg::endl
		<< L" m_RoomName	: " << pPacket->m_RoomName					<< dbg::endl
		<< L" m_RoomState	: " << g_pKTDXApp->GetEnumToString( L"ROOM_STATE", (int)pPacket->m_RoomState )			<< dbg::endl;

	if( true == pPacket->m_bPublic )
		dbg::clog << L" m_bPublic	: TRUE"								<< dbg::endl;
	else	
		dbg::clog << L" m_bPublic	: FALSE"							<< dbg::endl;

	dbg::clog 
		<< L" m_wstrPassword	: " << pPacket->m_wstrPassword			<< dbg::endl
		<< L" m_MaxSlot	: " << (int)pPacket->m_MaxSlot					<< dbg::endl
		<< L" m_JoinSlot	: " << (int)pPacket->m_JoinSlot				<< dbg::endl
		<< L" m_PVPGameType	: " << (int)pPacket->m_PVPGameType			<< dbg::endl
		<< L" m_WinKillNum	: " << (int)pPacket->m_WinKillNum			<< dbg::endl;

	if( true == pPacket->m_bCanIntrude )
		dbg::clog << L" m_bCanIntrude	: TRUE"							<< dbg::endl;
	else 
		dbg::clog << L" m_bCanIntrude	: FALSE"						<< dbg::endl;

	dbg::clog 
		<< L" m_fPlayTime	: " << pPacket->m_fPlayTime					<< dbg::endl
		<< L" m_WorldID	: " << g_pKTDXApp->GetEnumToString( L"WORLD_ID", (int)pPacket->m_WorldID ) << dbg::endl
		<< L" m_DifficultyLevel : " << g_pKTDXApp->GetEnumToString( L"DIFFICULTY_LEVEL", (int)pPacket->m_DifficultyLevel )		<< dbg::endl
		<< L" m_iDungeonID	: " << g_pKTDXApp->GetEnumToString( L"DUNGEON_ID", pPacket->m_iDungeonID ) << dbg::endl;
#endif
}


void CX2PacketLog::PrintLog( KRoomUserInfo* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KRoomUserInfo : " << dbg::endl 
		<< L" m_iGSUID : " << pPacket->m_iGSUID						<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KRoomSlotInfo* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KRoomSlotInfo : " << dbg::endl 
		<< L" m_Index : " << (int)pPacket->m_Index					<< dbg::endl
		<< L" m_SlotState : " << g_pKTDXApp->GetEnumToString( L"SLOT_STATE", (int)pPacket->m_SlotState ) << dbg::endl;

	if( true == pPacket->m_bHost )
		dbg::clog << L" m_bHost : TRUE"							<< dbg::endl;
	else 
		dbg::clog << L" m_bHost : FALSE"						<< dbg::endl;

	if( true == pPacket->m_bReady )
		dbg::clog << L" m_bReady : TRUE"						<< dbg::endl;
	else 
		dbg::clog << L" m_bReady : FALSE"						<< dbg::endl;

	dbg::clog 
		<< L" m_TeamNum : " << g_pKTDXApp->GetEnumToString( L"TEAM_NUM", (int)pPacket->m_TeamNum )		<< dbg::endl;

	CX2PacketLog::PrintLog( &pPacket->m_kRoomUserInfo );
#endif
}

void CX2PacketLog::PrintLog( KPacketOK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KPacketOK : " << dbg::endl
		<< L" m_iOK : " << pPacket->m_iOK						<< dbg::endl;
#endif
}







//////////////////////////////////////////////////////////////////////////
// ClientPacket
void CX2PacketLog::PrintLog( KEGS_CONNECT_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;
#endif
}

void CX2PacketLog::PrintLog( KEGS_NEW_USER_JOIN_REQ* pPacket )
{
	if( false == m_bEnable )
		return;
}

void CX2PacketLog::PrintLog( KEGS_NEW_USER_JOIN_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_NEW_USER_JOIN_ACK : " << dbg::endl 
		<< L" m_iOK		: "		<< pPacket->m_iOK							<< dbg::endl
		<< L" m_iUserUID	: " << pPacket->m_iUserUID						<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_VERIFY_ACCOUNT_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_VERIFY_ACCOUNT_REQ : " << dbg::endl 
		<< L" m_wstrUserID : " << pPacket->m_wstrUserID                     << dbg::endl
		<< L" m_wstrPasswd : " << pPacket->m_wstrPasswd                     << dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_VERIFY_ACCOUNT_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_VERIFY_ACCOUNT_ACK : " << dbg::endl 
		<< L" m_nOK				: " << pPacket->m_iOK                       << dbg::endl
		<< L" m_nUserUID		: " << pPacket->m_kAccountInfo.m_nUserUID   << dbg::endl
		<< L" m_wstrID			: " << pPacket->m_kAccountInfo.m_wstrID     << dbg::endl
		<< L" m_wstrName		: " << pPacket->m_kAccountInfo.m_wstrName   << dbg::endl
        << L" m_iAuthLevel      : " << pPacket->m_kAccountInfo.m_iAuthLevel << dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK : " << dbg::endl 
		<< L" m_iOK : "			<< pPacket->m_iOK							<< dbg::endl
		<< L" m_nUnitSlot : "	<< pPacket->m_nUnitSlot						<< dbg::endl;

	for( UINT i=0; i<pPacket->m_vecUnitInfo.size(); i++ )
	{
		KUnitInfo* pUnitInfo = &pPacket->m_vecUnitInfo[i];
		PrintLog( pUnitInfo );
	}
#endif
}

void CX2PacketLog::PrintLog( KEGS_CREATE_UNIT_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CREATE_UNIT_REQ : " << dbg::endl 
		<< L" m_wstrNickName : " << pPacket->m_wstrNickName					<< dbg::endl
		<< L" m_iClass : " << g_pKTDXApp->GetEnumToString( L"UNIT_CLASS", pPacket->m_iClass ) << dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CREATE_UNIT_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CREATE_UNIT_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK									<< dbg::endl;

	PrintLog( &pPacket->m_kUnitInfo );
#endif
}

void CX2PacketLog::PrintLog( KEGS_DELETE_UNIT_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DELETE_UNIT_REQ : " << dbg::endl 
		<< L" m_iUnitUID : " << pPacket->m_iUnitUID							<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_DELETE_UNIT_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DELETE_UNIT_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK									<< dbg::endl
		<< L" m_iUnitUID : " << pPacket->m_iUnitUID							<< dbg::endl;
#endif
}
void CX2PacketLog::PrintLog( KEGS_SELECT_UNIT_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_SELECT_UNIT_REQ : " << dbg::endl 
		<< L" m_iUnitUID : " << pPacket->m_iUnitUID							<< dbg::endl;
#endif
}
void CX2PacketLog::PrintLog( KEGS_SELECT_UNIT_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_SELECT_UNIT_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK							<< dbg::endl;
#endif
}
void CX2PacketLog::PrintLog( KEGS_STATE_CHANGE_SERVER_SELECT_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_STATE_CHANGE_SERVER_SELECT_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK							<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KServerSetData* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KServerSetData : " << dbg::endl 
		<< L" serverSetID : " << pPacket->serverSetID						<< dbg::endl
		<< L" serverSetName : " << pPacket->serverSetName					<< dbg::endl
		<< L" userCountLevel : " << (int)pPacket->userCountLevel			<< dbg::endl;
#endif
}
void CX2PacketLog::PrintLog( KEGS_GET_SERVER_SET_DATA_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_GET_SERVER_SET_DATA_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK						<< dbg::endl;

	for( UINT i=0; i<pPacket->m_ServerSetList.size(); i++ )
	{
		KServerSetData* pServerSetData = &pPacket->m_ServerSetList[i];
		PrintLog( pServerSetData );
	}
#endif
}

void CX2PacketLog::PrintLog( KEGS_SELECT_SERVER_SET_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_SELECT_SERVER_SET_REQ : " << dbg::endl 
		<< L" m_ServerSetID : " << pPacket->m_ServerSetID				<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_SELECT_SERVER_SET_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_SELECT_SERVER_SET_ACK : " << dbg::endl 
		<< L" m_iOK		: " << pPacket->m_iOK							<< dbg::endl
		<< L" m_wstrIP		: " << pPacket->m_wstrIP						<< dbg::endl
		<< L" m_usPort	: " << pPacket->m_usPort						<< dbg::endl;
#endif
}
void CX2PacketLog::PrintLog( KStateChangeReq* pPacket )
{
	if( false == m_bEnable )
		return;

	dbg::clog << L" KStateChangeReq : " << dbg::endl 
		<< L" m_nMapID : " << pPacket->m_nMapID				<< dbg::endl;
}
void CX2PacketLog::PrintLog( KStateChangeAck* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KStateChangeAck : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK				<< dbg::endl
		<< L" m_nMapID : " << pPacket->m_nMapID				<< dbg::endl;
#endif
}

//void CX2PacketLog::PrintLog( KEGS_ENTER_DUNGEON_CHANNEL_REQ* pPacket )
//{
//	if( false == m_bEnable )
//		return;
//
//	dbg::clog << L" KEGS_ENTER_DUNGEON_CHANNEL_REQ : " << dbg::endl 
//		<< L" m_nMapID : " << pPacket->m_nMapID				<< dbg::endl;
//}

void CX2PacketLog::PrintLog( KEGS_ROOM_LIST_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_ROOM_LIST_REQ : " << dbg::endl 
		<< L" m_nViewPage : " << pPacket->m_nViewPage					<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_ROOM_LIST_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_ROOM_LIST_ACK : " << dbg::endl 
		<< L" m_iOK		: " << pPacket->m_iOK							<< dbg::endl
		<< L" m_nTotalPage	: " << pPacket->m_nTotalPage				<< dbg::endl
		<< L" m_nViewPage	: " << pPacket->m_nViewPage					<< dbg::endl;

	for( UINT i=0; i<pPacket->m_vRoomInfo.size(); i++ )
	{
		KRoomInfo* pRoomInfo = &pPacket->m_vRoomInfo[i];
		PrintLog( pRoomInfo );
	}
#endif
}

void CX2PacketLog::PrintLog( KEGS_CREATE_ROOM_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CREATE_ROOM_REQ : " << dbg::endl;

	PrintLog( &pPacket->m_RoomInfo );
#endif
}

void CX2PacketLog::PrintLog( KEGS_CREATE_ROOM_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CREATE_ROOM_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK							<< dbg::endl;

	PrintLog( &pPacket->m_RoomInfo );

	for( UINT i=0; i<pPacket->m_vecSlot.size(); i++ )
	{
		KRoomSlotInfo* pRoomSlotInfo = &pPacket->m_vecSlot[i];
		PrintLog( pRoomSlotInfo );
	}
#endif
}

void CX2PacketLog::PrintLog( KEGS_JOIN_ROOM_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_JOIN_ROOM_REQ : " << dbg::endl 
		<< L" m_iRoomUID : " << pPacket->m_iRoomUID						<< dbg::endl
		<< L" m_wstrPassword : " << pPacket->m_wstrPassword				<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_JOIN_ROOM_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_JOIN_ROOM_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK								<< dbg::endl;

	PrintLog( &pPacket->m_RoomInfo );

	for( UINT i=0; i<pPacket->m_vecSlot.size(); i++ )
	{
		KRoomSlotInfo* pRoomSlotInfo = &pPacket->m_vecSlot[i];
		PrintLog( pRoomSlotInfo );
	}
#endif
}

void CX2PacketLog::PrintLog( KEGS_JOIN_ROOM_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_JOIN_ROOM_NOT : " << dbg::endl;

	PrintLog( &pPacket->m_JoinSlot );
#endif
}

void CX2PacketLog::PrintLog( KEGS_LEAVE_ROOM_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_LEAVE_ROOM_NOT : " << dbg::endl 
		<< L" m_cRoomState : " << g_pKTDXApp->GetEnumToString( L"ROOM_STATE", (int)pPacket->m_cRoomState )			<< dbg::endl
		<< L" m_iUnitUID : " << pPacket->m_iUnitUID							<< dbg::endl;

	for( UINT i=0; i<pPacket->m_vecSlot.size(); i++ )
	{
		KRoomSlotInfo* pRoomSlotInfo = &pPacket->m_vecSlot[i];
		PrintLog( pRoomSlotInfo );
	}
#endif
}

void CX2PacketLog::PrintLog( KEGS_LEAVE_GAME_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_LEAVE_GAME_NOT : " << dbg::endl 
		<< L" m_cRoomState : " << g_pKTDXApp->GetEnumToString( L"ROOM_STATE", (int)pPacket->m_cRoomState )			<< dbg::endl
		<< L" m_iUnitUID : " << pPacket->m_iUnitUID							<< dbg::endl;

	for( UINT i=0; i<pPacket->m_vecSlot.size(); i++ )
	{
		KRoomSlotInfo* pRoomSlotInfo = &pPacket->m_vecSlot[i];
		PrintLog( pRoomSlotInfo );
	}
#endif
}

void CX2PacketLog::PrintLog( KEGS_BAN_USER_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_BAN_USER_REQ : " << dbg::endl 
		<< L" m_iUnitUID : " << pPacket->m_iUnitUID					<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_TEAM_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_TEAM_REQ : " << dbg::endl 
		<< L" m_TeamNum : " << g_pKTDXApp->GetEnumToString( L"TEAM_NUM", (int)pPacket->m_TeamNum )		<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_TEAM_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_TEAM_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK							<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_TEAM_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_TEAM_NOT : " << dbg::endl 
		<< L" m_UnitUID		: " << pPacket->m_UnitUID							<< dbg::endl
		<< L" m_TeamNum		: " << g_pKTDXApp->GetEnumToString( L"TEAM_NUM", (int)pPacket->m_TeamNum )		<< dbg::endl
		<< L" m_SwapSlotIndex	: " << (int)pPacket->m_SwapSlotIndex			<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_READY_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_READY_REQ : " << dbg::endl;

	if( true == pPacket->m_bReady )
		dbg::clog << L" m_bReady : TRUE"						<< dbg::endl;
	else
		dbg::clog << L" m_bReady : FALSE"						<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_READY_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_READY_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK						<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_READY_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_READY_NOT : " << dbg::endl
		<< L" m_UnitUID : " << pPacket->m_UnitUID				<< dbg::endl;

	if( true == pPacket->m_bReady )
		dbg::clog << L" m_bReady : TRUE"						<< dbg::endl;
	else
		dbg::clog << L" m_bReady : FALSE"						<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_MAP_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_MAP_REQ : " << dbg::endl 
		<< L" m_WorldID : " << g_pKTDXApp->GetEnumToString( L"WORLD_ID", (int)pPacket->m_WorldID ) << dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_MAP_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_MAP_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK								<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_MAP_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_MAP_NOT : " << dbg::endl 
		<< L" m_WorldID : " << g_pKTDXApp->GetEnumToString( L"WORLD_ID", (int)pPacket->m_WorldID ) << dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_SLOT_OPEN_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_SLOT_OPEN_REQ : " << dbg::endl 
		<< L" m_SlotIndex : " << (int)pPacket->m_SlotIndex				<< dbg::endl
		<< L" m_SlotState : " << g_pKTDXApp->GetEnumToString( L"SLOT_STATE", (int)pPacket->m_SlotState ) << dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_SLOT_OPEN_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_SLOT_OPEN_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK								<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_SLOT_OPEN_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_SLOT_OPEN_NOT : " << dbg::endl 
		<< L" m_SlotIndex : " << (int)pPacket->m_SlotIndex				<< dbg::endl
		<< L" m_SlotState : " << g_pKTDXApp->GetEnumToString( L"SLOT_STATE", (int)pPacket->m_SlotState ) << dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_PLAY_TIME_LIMIT_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_PLAY_TIME_LIMIT_REQ : " << dbg::endl 
		<< L" m_fPlayTime : " << (int)pPacket->m_fPlayTime				<< dbg::endl;
#endif
}


void CX2PacketLog::PrintLog( KEGS_CHANGE_PLAY_TIME_LIMIT_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_PLAY_TIME_LIMIT_NOT : " << dbg::endl 
		<< L" m_fPlayTime : " << (int)pPacket->m_fPlayTime				<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_CHANGE_WINNING_NUM_KILL_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_WINNING_NUM_KILL_REQ : " << dbg::endl 
		<< L" m_fPlayTime : " << (int)pPacket->m_iWinKillNum				<< dbg::endl;
#endif
}



void CX2PacketLog::PrintLog( KEGS_CHANGE_WINNING_NUM_KILL_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHANGE_WINNING_NUM_KILL_REQ : " << dbg::endl 
		<< L" m_fPlayTime : " << (int)pPacket->m_iWinKillNum				<< dbg::endl;
#endif
}


void CX2PacketLog::PrintLog( KEGS_CHAT_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHAT_REQ : " << dbg::endl 
		<< L" m_wstrMsg : " << pPacket->m_wstrMsg						<< dbg::endl;
#endif
}

//void CX2PacketLog::PrintLog( KEGS_CHAT_ACK* pPacket )
//{
//	if( false == m_bEnable )
//		return;
//
//	dbg::clog << L" KEGS_CHAT_ACK : " << dbg::endl 
//		<< L" m_iOK : " << pPacket->m_iOK								<< dbg::endl;
//}

void CX2PacketLog::PrintLog( KEGS_CHAT_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CHAT_NOT : " << dbg::endl 
		<< L" m_SenderUnitUID : " << pPacket->m_SenderUnitUID					<< dbg::endl
		<< L" m_wstrMsg : " << pPacket->m_wstrMsg						<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_STATE_CHANGE_GAME_START_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_STATE_CHANGE_GAME_START_NOT : " << dbg::endl 
		<< L" m_cRoomState : " << g_pKTDXApp->GetEnumToString( L"ROOM_STATE", (int)pPacket->m_cRoomState )			<< dbg::endl;

	for( UINT i=0; i<pPacket->m_vecSlot.size(); i++ )
	{
		KRoomSlotInfo* pRoomSlotInfo = &pPacket->m_vecSlot[i];
		PrintLog( pRoomSlotInfo );
	}
#endif
}

void CX2PacketLog::PrintLog( KEGS_STATE_CHANGE_GAME_INTRUDE_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_STATE_CHANGE_GAME_INTRUDE_NOT : " << dbg::endl 
		<< L" m_RoomState : " << g_pKTDXApp->GetEnumToString( L"ROOM_STATE", (int)pPacket->m_RoomState )			<< dbg::endl;

	PrintLog( &pPacket->m_IntruderSlot );
#endif
}

void CX2PacketLog::PrintLog( KEGS_GAME_LOADING_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_GAME_LOADING_REQ : " << dbg::endl 
		<< L"  : " << pPacket->m_iLoadingProgress		<<  dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_GAME_LOADING_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_GAME_LOADING_NOT : " << dbg::endl 
		<< L" m_iUnitUID : "		<< pPacket->m_iUnitUID						<< dbg::endl
		<< L" m_iLoadingProgress : " << pPacket->m_iLoadingProgress				<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_PLAY_START_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_PLAY_START_NOT : " << dbg::endl 
		<< L" m_RoomState : " << g_pKTDXApp->GetEnumToString( L"ROOM_STATE", (int)pPacket->m_RoomState )			<< dbg::endl;

	for( UINT i=0; i<pPacket->m_vecSlot.size(); i++ )
	{
		KRoomSlotInfo* pRoomSlotInfo = &pPacket->m_vecSlot[i];
		PrintLog( pRoomSlotInfo );
	}
#endif
}

void CX2PacketLog::PrintLog( KEGS_INTRUDE_START_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_INTRUDE_START_REQ : " << dbg::endl 
		<< L" m_StartPosIndex : " << pPacket->m_StartPosIndex						<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_INTRUDE_START_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_INTRUDE_START_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK								<< dbg::endl;
#endif
}
void CX2PacketLog::PrintLog( KEGS_INTRUDE_START_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_INTRUDE_START_NOT : " << dbg::endl 
		<< L" m_RoomState : " << g_pKTDXApp->GetEnumToString( L"ROOM_STATE", (int)pPacket->m_RoomState )			<< dbg::endl
		<< L" m_StartPosIndex : " << pPacket->m_StartPosIndex						<< dbg::endl;

	PrintLog( &pPacket->m_IntruderSlot );
#endif
}

void CX2PacketLog::PrintLog( KEGS_CURRENT_KILL_SCORE_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_CURRENT_KILL_SCORE_NOT : " << dbg::endl; 

	std::map< UidType, std::pair< int, int > >::iterator it;
	for( it=pPacket->m_mapKillScore.begin(); it!=pPacket->m_mapKillScore.end(); it++ )
	{
		dbg::clog 
			<< L" m_mapKillScore : "							<< dbg::endl
			<< L" UidType : " << it->first						<< dbg::endl
			<< L" int : " << it->second.first						<< dbg::endl
			<< L" int : " << it->second.second						<< dbg::endl;
	}
#endif
}

void CX2PacketLog::PrintLog( KEGS_MY_USER_UNIT_INFO_TO_SERVER_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	//{{ 2011. 01. 21	ÃÖÀ°»ç	´øÀü ·©Å© °³Æí
#ifdef SERV_DUNGEON_RANK_NEW
	dbg::clog << L" KEGS_MY_USER_UNIT_INFO_TO_SERVER_REQ : " << dbg::endl 
		<< L" m_fHP : " << pPacket->m_kMyPlayResult.m_fHP									<< dbg::endl;
#else
	dbg::clog << L" KEGS_MY_USER_UNIT_INFO_TO_SERVER_REQ : " << dbg::endl 
		<< L" m_fHP : " << pPacket->m_fHP									<< dbg::endl;
#endif SERV_DUNGEON_RANK_NEW
	//}}	
#endif
}

void CX2PacketLog::PrintLog( KEGS_USER_UNIT_RE_BIRTH_POS_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_USER_UNIT_RE_BIRTH_POS_REQ : " << dbg::endl 
		<< L" m_StartPosIndex : " << pPacket->m_StartPosIndex							<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_USER_UNIT_RE_BIRTH_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_USER_UNIT_RE_BIRTH_NOT : " << dbg::endl 
		<< L" m_UserUnitUID : " << pPacket->m_UserUnitUID							<< dbg::endl
		<< L" m_StartPosIndex : " << pPacket->m_StartPosIndex							<< dbg::endl;
#endif
}

//void CX2PacketLog::PrintLog( KChannelReq* pPacket )
//{
//	if( false == m_bEnable )
//		return;
//
//	dbg::clog << L" KChannelReq : " << dbg::endl 
//		<< L" m_iType : " << pPacket->m_iType									<< dbg::endl
//		<< L" m_nMapID : " << pPacket->m_nMapID								<< dbg::endl;
//}
//
//void CX2PacketLog::PrintLog( KChannelAck* pPacket )
//{
//	if( false == m_bEnable )
//		return;
//
//	dbg::clog << L" KChannelAck : " << dbg::endl 
//		<< L" m_iOK : " << pPacket->m_iOK									<< dbg::endl
//		<< L" m_nChannelID : " << pPacket->m_nChannelID							<< dbg::endl;
//}


void CX2PacketLog::PrintLog( KEGS_USER_UNIT_DIE_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_USER_UNIT_DIE_REQ : " << dbg::endl 
		<< L" m_KillerNPCUID : " << pPacket->m_KillerNPCUID						<< dbg::endl
		<< L" m_KillerUserUnitUID : " << pPacket->m_KillerUserUnitUID					<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_USER_UNIT_DIE_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_USER_UNIT_DIE_ACK : " << dbg::endl 
		<< L" m_fReBirthTime : " << pPacket->m_fReBirthTime						<< dbg::endl
		<< L" m_iOK : " << pPacket->m_iOK								<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_USER_UNIT_DIE_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_USER_UNIT_DIE_NOT : " << dbg::endl 
		<< L" m_KillerNPCUID		: " << pPacket->m_KillerNPCUID						<< dbg::endl
		<< L" m_KillerUserUnitUID	: " << pPacket->m_KillerUserUnitUID					<< dbg::endl
		<< L" m_KilledUserUnitUID	: " << pPacket->m_KilledUserUnitUID					<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( VECTOR3* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" VECTOR3 : " << dbg::endl 
		<< L" x : " << pPacket->x				<< dbg::endl
		<< L" y : " << pPacket->y				<< dbg::endl
		<< L" z : " << pPacket->z				<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KNPCUnitReq* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KNPCUnitReq : " << dbg::endl 
		<< L" m_NPCID : " << pPacket->m_NPCID					<< dbg::endl;

	PrintLog( &pPacket->m_vPos );

	if( true == pPacket->m_bIsRight )
		dbg::clog << L" m_bIsRight : TRUE"						<< dbg::endl;
	else
		dbg::clog << L" m_bIsRight : FALSE"						<< dbg::endl;

	dbg::clog 
		<< L" m_fDelayTime : " << pPacket->m_fDelayTime				<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_NPC_UNIT_CREATE_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_NPC_UNIT_CREATE_REQ : " << dbg::endl;

	for( UINT i=0; i<pPacket->m_vecNPCUnitReq.size(); i++ )
	{
		KNPCUnitReq* pKNPCUnitReq = &pPacket->m_vecNPCUnitReq[i];
		PrintLog( pKNPCUnitReq );
	}
#endif
}

void CX2PacketLog::PrintLog( KNPCUnitNot* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	PrintLog( &pPacket->m_kNPCUnitReq );
#endif
}

void CX2PacketLog::PrintLog( KEGS_NPC_UNIT_CREATE_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_NPC_UNIT_CREATE_NOT : " << dbg::endl;

	for( UINT i=0; i<pPacket->m_vecNPCUnitAck.size(); i++ )
	{
		KNPCUnitNot* pNPCUnitNot = &pPacket->m_vecNPCUnitAck[i];
		PrintLog( pNPCUnitNot );
	}
#endif
}

void CX2PacketLog::PrintLog( KEGS_NPC_UNIT_DIE_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_NPC_UNIT_DIE_REQ : " << dbg::endl 
		<< L" m_nDieNPCUID		: " << pPacket->m_nDieNPCUID							<< dbg::endl
		<< L" m_cAttUnitType	: " << g_pKTDXApp->GetEnumToString( L"GAME_UNIT_TYPE", (int)pPacket->m_cAttUnitType	) << dbg::endl
		<< L" m_uiAttUnit		: " << pPacket->m_uiAttUnit								<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_NPC_UNIT_DIE_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_NPC_UNIT_DIE_NOT : " << dbg::endl 
		<< L" m_nDieNPCUID	: " << pPacket->m_nDieNPCUID							<< dbg::endl
		<< L" m_cAttUnitType	: " << g_pKTDXApp->GetEnumToString( L"GAME_UNIT_TYPE", (int)pPacket->m_cAttUnitType	) << dbg::endl
		<< L" m_uiAttUnit	: " << pPacket->m_uiAttUnit								<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_DUNGEON_STAGE_LOAD_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DUNGEON_STAGE_LOAD_REQ : " << dbg::endl 
		<< L" m_iStageID : " << pPacket->m_iStageID							<< dbg::endl;
#endif
}


void CX2PacketLog::PrintLog( KEGS_DUNGEON_STAGE_LOAD_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DUNGEON_STAGE_LOAD_NOT : " << dbg::endl
		<< L" m_iStageID : " << pPacket->m_iStageID								<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_DUNGEON_STAGE_LOAD_COMPLETE_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DUNGEON_STAGE_LOAD_COMPLETE_NOT : " << dbg::endl 
		<< L" m_iSlotID : " << pPacket->m_iSlotID							<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_DUNGEON_STAGE_START_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DUNGEON_STAGE_START_NOT : " << dbg::endl 
		<< L" m_iStageID : " << pPacket->m_iStageID							<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_DUNGEON_SUB_STAGE_OPEN_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DUNGEON_SUB_STAGE_OPEN_REQ : " << dbg::endl 
		<< L" m_iSubStageID : " << (int)pPacket->m_iSubStageID							<< dbg::endl;
#endif
}

//void CX2PacketLog::PrintLog( KEGS_DUNGEON_SUB_STAGE_OPEN_ACK* pPacket )
//{
//	dbg::clog << L" KEGS_DUNGEON_SUB_STAGE_OPEN_ACK : " << dbg::endl 
//		<< L" m_iOK : " << pPacket->m_iOK							<< dbg::endl;
//}

void CX2PacketLog::PrintLog( KEGS_DUNGEON_SUB_STAGE_OPEN_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DUNGEON_SUB_STAGE_OPEN_NOT : " << dbg::endl 
		<< L" m_iSubStageID : " << (int)pPacket->m_iSubStageID							<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ : " << dbg::endl 
		<< L" m_iBeforeStageID : " << (int)pPacket->m_iBeforeStageID						<< dbg::endl
		<< L" m_iNextStageID : " << (int)pPacket->m_iNextStageID							<< dbg::endl;
#endif
}

//void CX2PacketLog::PrintLog( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK* pPacket )
//{
//	dbg::clog << L" KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK : " << dbg::endl 
//		<< L" m_iOK : " << pPacket->m_iOK							<< dbg::endl;
//}

void CX2PacketLog::PrintLog( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT : " << dbg::endl 
		<< L" m_iBeforeStageID	: " << (int)pPacket->m_iBeforeStageID						<< dbg::endl
		<< L" m_iNextStageID	: " << (int)pPacket->m_iNextStageID							<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ : " << dbg::endl 
		<< L" m_iSubStageID	: " << (int)pPacket->m_iSubStageID						<< dbg::endl;
#endif
}

//void CX2PacketLog::PrintLog( KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK* pPacket )
//{
//	dbg::clog << L" KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK : " << dbg::endl 
//		<< L" m_iOK	: " << pPacket->m_iOK						<< dbg::endl;
//}

void CX2PacketLog::PrintLog( KEGS_DUNGEON_SUB_STAGE_START_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_DUNGEON_SUB_STAGE_START_NOT : " << dbg::endl 
		<< L" m_iSubStageID	: " << (int)pPacket->m_iSubStageID						<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KDungeonNextStageData* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KDungeonNextStageData : " << dbg::endl 
		<< L" m_wstrStageID : " << pPacket->m_wstrStageID							<< dbg::endl
		<< L" m_nSubStageIndex : " << pPacket->m_nSubStageIndex						<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_END_GAME_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_END_GAME_REQ : " << dbg::endl 
		<< L" m_iWinTeam : " << g_pKTDXApp->GetEnumToString( L"TEAM_NUM", pPacket->m_iWinTeam )		<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_END_GAME_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_END_GAME_NOT : " << dbg::endl 
		//<< L" m_cRoomState : " << g_pKTDXApp->GetEnumToString( L"ROOM_STATE", (int)pPacket->m_cRoomState )			<< dbg::endl
		<< L" m_iWinTeam : " << g_pKTDXApp->GetEnumToString( L"TEAM_NUM", pPacket->m_iWinTeam )		<< dbg::endl;

	//for( UINT i=0; i<pPacket->m_vecSlot.size(); i++ )
	//{
	//	KRoomSlotInfo* pRoomSlotInfo = &pPacket->m_vecSlot[i];
	//	PrintLog( pRoomSlotInfo );
	//}
#endif
}

void CX2PacketLog::PrintLog( KEGS_STATE_CHANGE_RESULT_REQ* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_STATE_CHANGE_RESULT_REQ : " << dbg::endl 
		<< L" m_Dummy : " << pPacket->m_Dummy								<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_STATE_CHANGE_RESULT_ACK* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_STATE_CHANGE_RESULT_ACK : " << dbg::endl 
		<< L" m_iOK : " << pPacket->m_iOK								<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( KEGS_STATE_CHANGE_RESULT_NOT* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" KEGS_STATE_CHANGE_RESULT_NOT : " << dbg::endl 
		<< L" m_cRoomState : " << g_pKTDXApp->GetEnumToString( L"ROOM_STATE", (int)pPacket->m_cRoomState )			<< dbg::endl;

	for( UINT i=0; i<pPacket->m_vecSlot.size(); i++ )
	{
		KRoomSlotInfo* pRoomSlotInfo = &pPacket->m_vecSlot[i];
		PrintLog( pRoomSlotInfo );
	}
#endif
}

void CX2PacketLog::PrintLog( int* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" PACKET_LOG(int) : " << dbg::endl 
		<< L" int : " << *pPacket						<< dbg::endl;
#endif
}

void CX2PacketLog::PrintLog( float* pPacket )
{
#ifdef _IN_HOUSE_
	if( false == m_bEnable )
		return;

	dbg::clog << L" PACKET_LOG(float) : " << dbg::endl 
		<< L" float : " << *pPacket						<< dbg::endl;
#endif
}

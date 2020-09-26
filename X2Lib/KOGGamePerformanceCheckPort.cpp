#include "StdAfx.h"

#include "KOGGamePerformanceCheckPort.h"

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
KOGGamePerformanceCheckPort_Function g_KOGGamePerformanceCheckPort_Function;

KOGGamePerformanceCheckPort_Function::KOGGamePerformanceCheckPort_Function()
{
#ifndef _SERVICE_
	//클라 시작할때는 로그파일을 항상 지워준다.
	FILE* pPingLog_Field = fopen( "PingLog_Field.txt", "w" );
	fclose( pPingLog_Field );
	FILE* pPingLog_Dungeon = fopen( "PingLog_Dungeon.txt", "w" );
	fclose( pPingLog_Dungeon );
	FILE* pPingLog_PvP = fopen( "PingLog_PvP.txt", "w" );
	fclose( pPingLog_PvP );
#endif//_SERVICE_
}

KOGGamePerformanceCheckPort_Function::~KOGGamePerformanceCheckPort_Function()
{
}

int KOGGamePerformanceCheckPort_Function::GetGameSpace()
{
	return g_pMain->GetNowStateID();
}

bool KOGGamePerformanceCheckPort_Function::IsGameSpaceVillage( int iGameSpace )
{
	if( iGameSpace != CX2Main::XS_VILLAGE_MAP )
		return false;
	return true;
}

bool KOGGamePerformanceCheckPort_Function::IsGameSpaceField( int iGameSpace )
{
	if( iGameSpace != CX2Main::XS_BATTLE_FIELD )
		return false;
	return true;
}

bool KOGGamePerformanceCheckPort_Function::IsGameSpaceDungeon( int iGameSpace )
{
	if( iGameSpace != CX2Main::XS_DUNGEON_GAME )
		return false;
	return true;
}

bool KOGGamePerformanceCheckPort_Function::IsGameSpacePvP( int iGameSpace )
{
	if( iGameSpace != CX2Main::XS_PVP_GAME )
		return false;
	return true;
}


KOGGamePerformanceCheck::EPartyType KOGGamePerformanceCheckPort_Function::GetVillagePartyType()
{
	if( g_pData->GetPartyManager() &&
		g_pData->GetPartyManager()->DoIHaveParty() )
	{
		if( g_pData->GetPartyManager()->AmIPartyLeader() )
			return KOGGamePerformanceCheck::ePartyType_Host;
		else
			return KOGGamePerformanceCheck::ePartyType_Member;
	}

	return KOGGamePerformanceCheck::ePartyType_No;
}

int KOGGamePerformanceCheckPort_Function::GetVillagePartyUserNum()
{
	if( g_pData->GetPartyManager() )
		return g_pData->GetPartyManager()->GetMyPartyData()->m_iPartyMemberCount -1;

	return -1;
}

int KOGGamePerformanceCheckPort_Function::GetVillageMapID()
{
	if( !g_pTFieldGame || !g_pTFieldGame->GetWorld() || !g_pTFieldGame->GetWorld()->GetWorldData() )
		return -1;

	return g_pTFieldGame->GetWorld()->GetWorldData()->worldID;
}

int KOGGamePerformanceCheckPort_Function::GetVillageUserNum()
{
	if( !g_pTFieldGame )
		return -1;
	return g_pTFieldGame->GetSquareUnitNum();
}

KOGGamePerformanceCheck::EPartyType KOGGamePerformanceCheckPort_Function::GetFieldPartyType()
{
	if( g_pData->GetPartyManager() &&
		g_pData->GetPartyManager()->DoIHaveParty() )
	{
		if( g_pData->GetPartyManager()->AmIPartyLeader() )
			return KOGGamePerformanceCheck::ePartyType_Host;
		else
			return KOGGamePerformanceCheck::ePartyType_Member;
	}

	return KOGGamePerformanceCheck::ePartyType_No;
}

int KOGGamePerformanceCheckPort_Function::GetFieldPartyUserNum()
{
	if( g_pData->GetPartyManager() )
		return g_pData->GetPartyManager()->GetMyPartyData()->m_iPartyMemberCount - 1;

	return -1;
}

int KOGGamePerformanceCheckPort_Function::GetFieldMapID()
{
	if( !g_pX2Game ||!g_pX2Game->GetWorld() || !g_pX2Game->GetWorld()->GetWorldData() )
		return -1;

	return g_pX2Game->GetWorld()->GetWorldData()->worldID;
}

int KOGGamePerformanceCheckPort_Function::GetFieldUserNum()
{
	if( !g_pX2Game )
		return -1;

	return g_pX2Game->GetUserUnitNum();
}

KOGGamePerformanceCheck::EPartyType KOGGamePerformanceCheckPort_Function::GetDungeonPartyType()
{
	if( g_pData->GetPartyManager() &&
		g_pData->GetPartyManager()->DoIHaveParty() )
	{
		if( g_pData->GetPartyManager()->AmIPartyLeader() )
			return KOGGamePerformanceCheck::ePartyType_Host;
		else
			return KOGGamePerformanceCheck::ePartyType_Member;
	}

	return KOGGamePerformanceCheck::ePartyType_No;
}

int KOGGamePerformanceCheckPort_Function::GetDungeonPartyUserNum()
{
	if( g_pData->GetPartyManager() )
		return g_pData->GetPartyManager()->GetMyPartyData()->m_iPartyMemberCount - 1;

	return -1;
}

int KOGGamePerformanceCheckPort_Function::GetDungeonMapID()
{
	if( !g_pX2Game || !g_pX2Game->GetWorld() || !g_pX2Game->GetWorld()->GetWorldData() )
		return -1;

	return g_pX2Game->GetWorld()->GetWorldData()->worldID;
}

int KOGGamePerformanceCheckPort_Function::GetDungeonDifficulty()
{
	if( !g_pData->GetDungeonRoom() )
		return -1;
	return g_pData->GetDungeonRoom()->GetDifficulty();
}

int KOGGamePerformanceCheckPort_Function::GetDungeonStage()
{
	if( !g_pX2Game )
		return -1;

	//stage 1부터 시작임으로 +1 해준다.
	return g_pX2Game->GetNowStageIndex() + 1;
}

bool KOGGamePerformanceCheckPort_Function::IsDungeonEscape()
{
	if( !g_pX2Game )
		return false;
	
	if( CX2Game::GT_DUNGEON != g_pX2Game->GetGameType() )
		return false;

	CX2DungeonGame* pkDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	if( !pkDungeonGame )
		return false;

	return pkDungeonGame->IsDungeonEscape();
}

int KOGGamePerformanceCheckPort_Function::GetDungeonBlock()
{
	if( !g_pX2Game )
		return -1;

	if( CX2Game::GT_DUNGEON != g_pX2Game->GetGameType() )
		return -1;

	CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	if( !pDungeonGame )
		return -1;

	if( !pDungeonGame->GetDungeon() || 
		!pDungeonGame->GetDungeon()->GetNowStage() )
		return -1;

	//block 1부터 시작임으로 +1 해준다.
	return pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStageIndex() + 1;
}

int KOGGamePerformanceCheckPort_Function::GetPvPMapID()
{
	if( !g_pX2Game ||!g_pX2Game->GetWorld() || !g_pX2Game->GetWorld()->GetWorldData() )
		return -1;

	return g_pX2Game->GetWorld()->GetWorldData()->worldID;
}

int KOGGamePerformanceCheckPort_Function::GetPvPUserNum()
{
	if( !g_pX2Game )
		return -1;

	return g_pX2Game->GetUserUnitNum();
}

int KOGGamePerformanceCheckPort_Function::GetPvPRuleType()
{
	if( !g_pData->GetPVPRoom() )
		return -1;

	return ( g_pData->GetPVPRoom()->GetPVPGameType() << 16 ) | (int)g_pData->GetPVPRoom()->GetItemMode();
}

bool KOGGamePerformanceCheckPort_Function::IsPvPEscape()
{
	if( NULL == g_pX2Game || g_pX2Game->GetGameType() != CX2Game::GT_PVP )
		return false;

	if( CX2Game::GT_PVP != g_pX2Game->GetGameType() )
		return false;

	CX2PVPGame* pkPvPGame = static_cast<CX2PVPGame*>(g_pX2Game);
	if( !pkPvPGame )
		return false;

	return pkPvPGame->IsPvPEscape();
}

int KOGGamePerformanceCheckPort_Function::GetPvPRound()
{
	if( !g_pX2Game )
		return -1;

	switch( g_pData->GetPVPRoom()->GetPVPGameType() )
	{
	case CX2PVPRoom::PGT_TEAM:
		//엘소드의 팀매치는 1 라운드 밖에 없다.
		return 1;
	case CX2PVPRoom::PGT_TEAM_DEATH:
		return g_pX2Game->GetRedTeamKill() + g_pX2Game->GetBlueTeamKill();
	case CX2PVPRoom::PGT_SURVIVAL:
		{
			int iTotalKillNum = 0;
			for ( int i = 0; i < g_pX2Game->GetUserUnitListSize(); i++ )
			{
				CX2GUUser* pGUUser = g_pX2Game->GetUserUnit(i);
				if ( pGUUser == NULL )
					continue;

				iTotalKillNum += pGUUser->m_KillUserUnitList.size();
			}

			return iTotalKillNum;
		}
	}

	return -1;
}

float KOGGamePerformanceCheckPort_Function::GetFramePerSecond()
{
	return DXUTGetFPS2();
}

void KOGGamePerformanceCheckPort_Function::ResetFramePerSecond()
{
	DXUTResetStatsUpdateFrame2();
}

bool KOGGamePerformanceCheckPort_Function::IsGameHost()
{
	if( !g_pX2Game )
		return false;
	return g_pX2Game->IsHost();
}

void KOGGamePerformanceCheckPort_Function::SendServer_Village( const KOGGamePerformanceCheck::KVillageInfo& kVillageInfo )
{
	bool bExistData = false;
	KEGS_KOG_GAME_PERFORMANCE_VILLAGE_LOG kPacket;

	//캐릭터 UID 얻기
	kPacket.m_iCharUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	//시스템 키 얻기
	kPacket.m_iSystemKey = g_pMain->GetSystemInfo()->GetMacAddress();

	//평균프레임, UDP Ping, TCP Ping 정보를 서버로 전송
	const KAvgFpsCheckManager::KAvgFps& kAvgFps = KOGGamePerformanceCheck::GetInstance()->GetAvgFpsCheckMgr()->GetAvgFps();
	const KMemUsageCheckManager::KMemUsage& kMemUsage = KOGGamePerformanceCheck::GetInstance()->GetMemUsageCheckMgr()->GetMemUsage();

	if( kAvgFps.GetValue() != -1.0f )
	{
		//평균프레임
		kPacket.m_fAvgFps = kAvgFps.GetValue();
		kPacket.m_fAvgFps_StdDev = kAvgFps.GetValue_StdDev();
		//메모리 사용량
		static bool bFirstMemUsageCheck = true;
		kPacket.m_bFirstMemUsageCheck = bFirstMemUsageCheck;
		kPacket.m_fMinWorkingSetSize = kMemUsage.GetMinWorkingSetSize();
		kPacket.m_fMaxWorkingSetSize = kMemUsage.GetMaxWorkingSetSize();
		kPacket.m_fMinPagefileUsage = kMemUsage.GetMinPagefileUsage();
		kPacket.m_fMaxPagefileUsage = kMemUsage.GetMaxPagefileUsage();
		bFirstMemUsageCheck = false;
		//부가정보
		kPacket.m_iPartyType = kVillageInfo.m_ePartyType;
		kPacket.m_iPartyUserNum = kVillageInfo.m_iPartyUserNum;
		kPacket.m_iMapID = kVillageInfo.m_iMapID;
		kPacket.m_iVillageUserNum = kVillageInfo.m_iVillageUserNum;

		//
		bExistData = true;
	}

	//알려지지 않은 랙 정보를 서버로 전송
	const KLagCheckManager::KVillageInfo& kUnKnownLagInfo = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetUnKnownLagInfo_Village();

	if( kUnKnownLagInfo.GetLagTime() != -1.0f )
	{
		kPacket.m_kUnKnownLagInfo.m_iLagType = kUnKnownLagInfo.GetLagType();
		kPacket.m_kUnKnownLagInfo.m_iPartyType = kUnKnownLagInfo.GetPartyType();
		kPacket.m_kUnKnownLagInfo.m_iPartyUserNum = kUnKnownLagInfo.GetPartyUserNum();
		kPacket.m_kUnKnownLagInfo.m_iMapID = kUnKnownLagInfo.GetMapID();
		kPacket.m_kUnKnownLagInfo.m_iVillageUserNum = kUnKnownLagInfo.GetVillageUserNum();
		kPacket.m_kUnKnownLagInfo.m_fLagTime = kUnKnownLagInfo.GetLagTime();
		kPacket.m_kUnKnownLagInfo.m_fPlayTime = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetPlayTime();
		kPacket.m_kUnKnownLagInfo.m_iLagTotalNum = kUnKnownLagInfo.GetLagTotalNum();
		kPacket.m_kUnKnownLagInfo.m_fLagTotalTime = kUnKnownLagInfo.GetLagTotalTime();

		//
		bExistData = true;
	}

	//알려진 랙 정보를 서버로 전송
	const KLagCheckManager::KVillageInfo& kKnownLagInfo = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetKnownLagInfo_Village();

	if( kKnownLagInfo.GetLagTime() != -1.0f )
	{
		kPacket.m_kKnownLagInfo.m_iLagType = kKnownLagInfo.GetLagType();
		kPacket.m_kKnownLagInfo.m_iPartyType = kKnownLagInfo.GetPartyType();
		kPacket.m_kKnownLagInfo.m_iPartyUserNum = kKnownLagInfo.GetPartyUserNum();
		kPacket.m_kKnownLagInfo.m_iMapID = kKnownLagInfo.GetMapID();
		kPacket.m_kKnownLagInfo.m_iVillageUserNum = kKnownLagInfo.GetVillageUserNum();
		kPacket.m_kKnownLagInfo.m_fLagTime = kKnownLagInfo.GetLagTime();
		kPacket.m_kKnownLagInfo.m_fPlayTime = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetPlayTime();
		kPacket.m_kKnownLagInfo.m_iLagTotalNum = kKnownLagInfo.GetLagTotalNum();
		kPacket.m_kKnownLagInfo.m_fLagTotalTime = kKnownLagInfo.GetLagTotalTime();

		//
		bExistData = true;
	}

	if( bExistData )
	{
		//먼가 측정한 수치가 존재한다면 서버로 보낸다.
		g_pData->GetServerProtocol()->SendPacket( EGS_KOG_GAME_PERFORMANCE_VILLAGE_LOG, kPacket );
	}
}

void KOGGamePerformanceCheckPort_Function::SendServer_Field( const KOGGamePerformanceCheck::KFieldInfo& kFieldInfo )
{
	bool bExistData = false;
	KEGS_KOG_GAME_PERFORMANCE_FIELD_LOG kPacket;

	//캐릭터 UID 얻기
	kPacket.m_iCharUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	//시스템 키 얻기
	kPacket.m_iSystemKey = g_pMain->GetSystemInfo()->GetMacAddress();

	//평균프레임, UDP Ping, TCP Ping 정보를 서버로 전송
	const KAvgFpsCheckManager::KAvgFps& kAvgFps = KOGGamePerformanceCheck::GetInstance()->GetAvgFpsCheckMgr()->GetAvgFps();
	const KPingCheckManager::KPing& kP2PPing = KOGGamePerformanceCheck::GetInstance()->GetP2PPingCheckMgr()->GetPing();
	const KPingCheckManager::KPing& kRelayPing = KOGGamePerformanceCheck::GetInstance()->GetRelayPingCheckMgr()->GetPing();
	const KPingCheckManager::KPing& kUdpPing = KOGGamePerformanceCheck::GetInstance()->GetUdpPingCheckMgr()->GetPing();
	const KPingCheckManager::KPing& kTcpPing = KOGGamePerformanceCheck::GetInstance()->GetTcpPingCheckMgr()->GetPing();
	const KUDPTransCheckManager::KTrans& kUdpSend = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetSend();
	const KUDPTransCheckManager::KTrans& kUdpRecv = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetRecv();
	const KUDPTransCheckManager::KUnitSyncPacketLossRate& kUdpUserSyncPacketLossRate = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetUserSyncPacketLossRate();
	const KUDPTransCheckManager::KUnitSyncPacketLossRate& kUdpNpcSyncPacketLossRate = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetNpcSyncPacketLossRate();
	const KNonHostNpcReactionCheckManager::KDelay& kNonHostNpcReactionDelay = KOGGamePerformanceCheck::GetInstance()->GetNonHostNpcReactionCheckMgr()->GetDelay();
	const KMemUsageCheckManager::KMemUsage& kMemUsage = KOGGamePerformanceCheck::GetInstance()->GetMemUsageCheckMgr()->GetMemUsage();

	if( kAvgFps.GetValue() != -1.0f )
	{
		//평균프레임
		kPacket.m_fAvgFps = kAvgFps.GetValue();
		kPacket.m_fAvgFps_StdDev = kAvgFps.GetValue_StdDev();
		//UDP Ping
		kPacket.m_fP2PMinPingTime = kP2PPing.GetMinPingTime();
		kPacket.m_fP2PMaxPingTime = kP2PPing.GetMaxPingTime();
		kPacket.m_fP2PAvgPingTime = kP2PPing.GetAvgPingTime();
		kPacket.m_fP2PAvgPingTime_StdDev = kP2PPing.GetAvgPingTime_StdDev();
		kPacket.m_fP2PAvgPingLossRate = kP2PPing.GetAvgPingLossRate();
		kPacket.m_fRelayMinPingTime = kRelayPing.GetMinPingTime();
		kPacket.m_fRelayMaxPingTime = kRelayPing.GetMaxPingTime();
		kPacket.m_fRelayAvgPingTime = kRelayPing.GetAvgPingTime();
		kPacket.m_fRelayAvgPingTime_StdDev = kRelayPing.GetAvgPingTime_StdDev();
		kPacket.m_fRelayAvgPingLossRate = kRelayPing.GetAvgPingLossRate();
		kPacket.m_fUdpMinPingTime = kUdpPing.GetMinPingTime();
		kPacket.m_fUdpMaxPingTime = kUdpPing.GetMaxPingTime();
		kPacket.m_fUdpAvgPingTime = kUdpPing.GetAvgPingTime();
		kPacket.m_fUdpAvgPingTime_StdDev = kUdpPing.GetAvgPingTime_StdDev();
		kPacket.m_fUdpAvgPingLossRate = kUdpPing.GetAvgPingLossRate();
		//TCP Ping
		kPacket.m_fTcpMinPingTime = kTcpPing.GetMinPingTime();
		kPacket.m_fTcpMaxPingTime = kTcpPing.GetMaxPingTime();
		kPacket.m_fTcpAvgPingTime = kTcpPing.GetAvgPingTime();
		kPacket.m_fTcpAvgPingTime_StdDev = kTcpPing.GetAvgPingTime_StdDev();
		//UDP Trans
		kUdpSend.GetValuePerSec( kPacket.m_fUdpSendCountPerSec_Host, kPacket.m_fUdpSendAmountPerSec_Host, kPacket.m_fUdpSendCountPerSec_NonHost, kPacket.m_fUdpSendAmountPerSec_NonHost );
		kUdpRecv.GetValuePerSec( kPacket.m_fUdpRecvCountPerSec_Host, kPacket.m_fUdpRecvAmountPerSec_Host, kPacket.m_fUdpRecvCountPerSec_NonHost, kPacket.m_fUdpRecvAmountPerSec_NonHost );
		//UDP 유닛싱크 로스율
		kPacket.m_fUdpUserSyncPacketLossRate = kUdpUserSyncPacketLossRate.GetValue();
		kPacket.m_fUdpNpcSyncPacketLossRate = kUdpNpcSyncPacketLossRate.GetValue();
		//비호스트 평균 NPC 리액션 delay
		kPacket.m_fNonHostNpcReaction_HostUdpAvgPingTime = kNonHostNpcReactionDelay.GetHostUdpAvgPingTime();
		kPacket.m_fNonHostNpcReaction_MinDelayTime_SimulOff = kNonHostNpcReactionDelay.GetMinDelayTime_SimulOff();
		kPacket.m_fNonHostNpcReaction_MaxDelayTime_SimulOff = kNonHostNpcReactionDelay.GetMaxDelayTime_SimulOff();
		kPacket.m_fNonHostNpcReaction_AvgDelayTime_SimulOff = kNonHostNpcReactionDelay.GetAvgDelayTime_SimulOff();
		kPacket.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOff = kNonHostNpcReactionDelay.GetAvgDelayTime_StdDev_SimulOff();
		kPacket.m_fNonHostNpcReaction_MinDelayTime_SimulOn = kNonHostNpcReactionDelay.GetMinDelayTime_SimulOn();
		kPacket.m_fNonHostNpcReaction_MaxDelayTime_SimulOn = kNonHostNpcReactionDelay.GetMaxDelayTime_SimulOn();
		kPacket.m_fNonHostNpcReaction_AvgDelayTime_SimulOn = kNonHostNpcReactionDelay.GetAvgDelayTime_SimulOn();
		kPacket.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOn = kNonHostNpcReactionDelay.GetAvgDelayTime_StdDev_SimulOn();
		//메모리 사용량
		static bool bFirstMemUsageCheck = true;
		kPacket.m_bFirstMemUsageCheck = bFirstMemUsageCheck;
		kPacket.m_fMinWorkingSetSize = kMemUsage.GetMinWorkingSetSize();
		kPacket.m_fMaxWorkingSetSize = kMemUsage.GetMaxWorkingSetSize();
		kPacket.m_fMinPagefileUsage = kMemUsage.GetMinPagefileUsage();
		kPacket.m_fMaxPagefileUsage = kMemUsage.GetMaxPagefileUsage();
		bFirstMemUsageCheck = false;
		//부가정보
		kPacket.m_iPartyType = kFieldInfo.m_ePartyType;
		kPacket.m_iPartyUserNum = kFieldInfo.m_iPartyUserNum;
		kPacket.m_iMapID = kFieldInfo.m_iMapID;
		kPacket.m_iFieldUserNum = kFieldInfo.m_iFieldUserNum;

		//
		bExistData = true;

#ifndef _SERVICE_
		{
			FILE* pPingLog_Field = fopen( "PingLog_Field.txt", "a+" );

			//udp ping 로그
			fprintf( pPingLog_Field, "---- Field Start ----\n" );
			fprintf( pPingLog_Field, "P2P MinPingTime: %f\n", kPacket.m_fP2PMinPingTime );
			fprintf( pPingLog_Field, "P2P MaxPingTime: %f\n", kPacket.m_fP2PMaxPingTime );
			fprintf( pPingLog_Field, "P2P AvgPingTime: %f\n", kPacket.m_fP2PAvgPingTime );
			fprintf( pPingLog_Field, "P2P AvgPingTime StdDev: %f\n", kPacket.m_fP2PAvgPingTime_StdDev );
			fprintf( pPingLog_Field, "P2P AvgPingLossRate: %f\n", kPacket.m_fP2PAvgPingLossRate );
			fprintf( pPingLog_Field, "Relay MinPingTime: %f\n", kPacket.m_fRelayMinPingTime );
			fprintf( pPingLog_Field, "Relay MaxPingTime: %f\n", kPacket.m_fRelayMaxPingTime );
			fprintf( pPingLog_Field, "Relay AvgPingTime: %f\n", kPacket.m_fRelayAvgPingTime );
			fprintf( pPingLog_Field, "Relay AvgPingTime StdDev: %f\n", kPacket.m_fRelayAvgPingTime_StdDev );
			fprintf( pPingLog_Field, "Relay AvgPingLossRate: %f\n", kPacket.m_fRelayAvgPingLossRate );
			fprintf( pPingLog_Field, "UDP MinPingTime: %f\n", kPacket.m_fUdpMinPingTime );
			fprintf( pPingLog_Field, "UDP MaxPingTime: %f\n", kPacket.m_fUdpMaxPingTime );
			fprintf( pPingLog_Field, "UDP AvgPingTime: %f\n", kPacket.m_fUdpAvgPingTime );
			fprintf( pPingLog_Field, "UDP AvgPingTime StdDev: %f\n", kPacket.m_fUdpAvgPingTime_StdDev );
			fprintf( pPingLog_Field, "UDP AvgPingLossRate: %f\n", kPacket.m_fUdpAvgPingLossRate );

			//tcp ping 로그
			fprintf( pPingLog_Field, "TCP MinPingTime: %f\n", kPacket.m_fTcpMinPingTime );
			fprintf( pPingLog_Field, "TCP MaxPingTime: %f\n", kPacket.m_fTcpMaxPingTime );
			fprintf( pPingLog_Field, "TCP AvgPingTime: %f\n", kPacket.m_fTcpAvgPingTime );
			fprintf( pPingLog_Field, "TCP AvgPingTime StdDev: %f\n", kPacket.m_fTcpAvgPingTime_StdDev );

			//UDP 전송측정 로그
			fprintf( pPingLog_Field, "UDP SendCountPerSec(Host): %f\n", kPacket.m_fUdpSendCountPerSec_Host );
			fprintf( pPingLog_Field, "UDP SendAmountPerSec(Host): %f\n", kPacket.m_fUdpSendAmountPerSec_Host );
			fprintf( pPingLog_Field, "UDP RecvCountPerSec(Host): %f\n", kPacket.m_fUdpRecvCountPerSec_Host );
			fprintf( pPingLog_Field, "UDP RecvAmountPerSec(Host): %f\n", kPacket.m_fUdpRecvAmountPerSec_Host );
			fprintf( pPingLog_Field, "UDP SendCountPerSec(NonHost): %f\n", kPacket.m_fUdpSendCountPerSec_NonHost );
			fprintf( pPingLog_Field, "UDP SendAmountPerSec(NonHost): %f\n", kPacket.m_fUdpSendAmountPerSec_NonHost );
			fprintf( pPingLog_Field, "UDP RecvCountPerSec(NonHost): %f\n", kPacket.m_fUdpRecvCountPerSec_NonHost );
			fprintf( pPingLog_Field, "UDP RecvAmountPerSec(NonHost): %f\n", kPacket.m_fUdpRecvAmountPerSec_NonHost );

			//UDP 유닛싱크 로스율
			fprintf( pPingLog_Field, "UDP UserSyncPacketLossRate: %f\n", kPacket.m_fUdpUserSyncPacketLossRate );
			fprintf( pPingLog_Field, "UDP NpcSyncPacketLossRate: %f\n", kPacket.m_fUdpNpcSyncPacketLossRate );

			//비호스트 평균 NPC 리액션 delay
			fprintf( pPingLog_Field, "NonHostNpcReaction HostUdpAvgPingTime: %f\n", kPacket.m_fNonHostNpcReaction_HostUdpAvgPingTime );
			fprintf( pPingLog_Field, "NonHostNpcReaction MinDealyTime_SimulOff: %f\n", kPacket.m_fNonHostNpcReaction_MinDelayTime_SimulOff );
			fprintf( pPingLog_Field, "NonHostNpcReaction MaxDealyTime_SimulOff: %f\n", kPacket.m_fNonHostNpcReaction_MaxDelayTime_SimulOff );
			fprintf( pPingLog_Field, "NonHostNpcReaction AvgDealyTime_SimulOff: %f\n", kPacket.m_fNonHostNpcReaction_AvgDelayTime_SimulOff );
			fprintf( pPingLog_Field, "NonHostNpcReaction AvgDealyTime_StdDev_SimulOff: %f\n", kPacket.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOff );
			fprintf( pPingLog_Field, "NonHostNpcReaction MinDealyTime_SimulOn: %f\n", kPacket.m_fNonHostNpcReaction_MinDelayTime_SimulOn );
			fprintf( pPingLog_Field, "NonHostNpcReaction MaxDealyTime_SimulOn: %f\n", kPacket.m_fNonHostNpcReaction_MaxDelayTime_SimulOn );
			fprintf( pPingLog_Field, "NonHostNpcReaction AvgDealyTime_SimulOn: %f\n", kPacket.m_fNonHostNpcReaction_AvgDelayTime_SimulOn );
			fprintf( pPingLog_Field, "NonHostNpcReaction AvgDealyTime_StdDev_SimulOn: %f\n", kPacket.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOn );

			fprintf( pPingLog_Field, "---- Field End ----\n\n" );

			fclose( pPingLog_Field );
		}
#endif//_SERVICE_
	}

	//알려지지 않은 랙 정보를 서버로 전송
	const KLagCheckManager::KFieldInfo& kUnKnownLagInfo = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetUnKnownLagInfo_Field();

	if( kUnKnownLagInfo.GetLagTime() != -1.0f )
	{
		kPacket.m_kUnKnownLagInfo.m_iLagType = kUnKnownLagInfo.GetLagType();
		kPacket.m_kUnKnownLagInfo.m_iPartyType = kUnKnownLagInfo.GetPartyType();
		kPacket.m_kUnKnownLagInfo.m_iPartyUserNum = kUnKnownLagInfo.GetPartyUserNum();
		kPacket.m_kUnKnownLagInfo.m_iMapID = kUnKnownLagInfo.GetMapID();
		kPacket.m_kUnKnownLagInfo.m_iFieldUserNum = kUnKnownLagInfo.GetFieldUserNum();
		kPacket.m_kUnKnownLagInfo.m_fLagTime = kUnKnownLagInfo.GetLagTime();
		kPacket.m_kUnKnownLagInfo.m_fPlayTime = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetPlayTime();
		kPacket.m_kUnKnownLagInfo.m_iLagTotalNum = kUnKnownLagInfo.GetLagTotalNum();
		kPacket.m_kUnKnownLagInfo.m_fLagTotalTime = kUnKnownLagInfo.GetLagTotalTime();

		//
		bExistData = true;
	}

	//알려진 랙 정보를 서버로 전송
	const KLagCheckManager::KFieldInfo& kKnownLagInfo = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetKnownLagInfo_Field();

	if( kKnownLagInfo.GetLagTime() != -1.0f )
	{
		kPacket.m_kKnownLagInfo.m_iLagType = kKnownLagInfo.GetLagType();
		kPacket.m_kKnownLagInfo.m_iPartyType = kKnownLagInfo.GetPartyType();
		kPacket.m_kKnownLagInfo.m_iPartyUserNum = kKnownLagInfo.GetPartyUserNum();
		kPacket.m_kKnownLagInfo.m_iMapID = kKnownLagInfo.GetMapID();
		kPacket.m_kKnownLagInfo.m_iFieldUserNum = kKnownLagInfo.GetFieldUserNum();
		kPacket.m_kKnownLagInfo.m_fLagTime = kKnownLagInfo.GetLagTime();
		kPacket.m_kKnownLagInfo.m_fPlayTime = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetPlayTime();
		kPacket.m_kKnownLagInfo.m_iLagTotalNum = kKnownLagInfo.GetLagTotalNum();
		kPacket.m_kKnownLagInfo.m_fLagTotalTime = kKnownLagInfo.GetLagTotalTime();

		//
		bExistData = true;
	}

	if( bExistData )
	{
		//먼가 측정한 수치가 존재한다면 서버로 보낸다.
		g_pData->GetServerProtocol()->SendPacket( EGS_KOG_GAME_PERFORMANCE_FIELD_LOG, kPacket );
	}
}

void KOGGamePerformanceCheckPort_Function::SendServer_Dungeon( const KOGGamePerformanceCheck::KDungeonInfo& kDungeonInfo )
{
	bool bExistData = false;
	KEGS_KOG_GAME_PERFORMANCE_DUNGEON_LOG kPacket;

	//캐릭터 UID 얻기
	kPacket.m_iCharUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	//시스템 키 얻기
	kPacket.m_iSystemKey = g_pMain->GetSystemInfo()->GetMacAddress();

	//평균프레임, UDP Ping, TCP Ping 정보를 서버로 전송
	const KAvgFpsCheckManager::KAvgFps& kAvgFps = KOGGamePerformanceCheck::GetInstance()->GetAvgFpsCheckMgr()->GetAvgFps();
	const KPingCheckManager::KPing& kP2PPing = KOGGamePerformanceCheck::GetInstance()->GetP2PPingCheckMgr()->GetPing();
	const KPingCheckManager::KPing& kRelayPing = KOGGamePerformanceCheck::GetInstance()->GetRelayPingCheckMgr()->GetPing();
	const KPingCheckManager::KPing& kUdpPing = KOGGamePerformanceCheck::GetInstance()->GetUdpPingCheckMgr()->GetPing();
	const KPingCheckManager::KPing& kTcpPing = KOGGamePerformanceCheck::GetInstance()->GetTcpPingCheckMgr()->GetPing();
	const KUDPTransCheckManager::KTrans& kUdpSend = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetSend();
	const KUDPTransCheckManager::KTrans& kUdpRecv = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetRecv();
	const KUDPTransCheckManager::KUnitSyncPacketLossRate& kUdpUserSyncPacketLossRate = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetUserSyncPacketLossRate();
	const KUDPTransCheckManager::KUnitSyncPacketLossRate& kUdpNpcSyncPacketLossRate = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetNpcSyncPacketLossRate();
	const KNonHostNpcReactionCheckManager::KDelay& kNonHostNpcReactionDelay = KOGGamePerformanceCheck::GetInstance()->GetNonHostNpcReactionCheckMgr()->GetDelay();
	const KMemUsageCheckManager::KMemUsage& kMemUsage = KOGGamePerformanceCheck::GetInstance()->GetMemUsageCheckMgr()->GetMemUsage();

	if( kAvgFps.GetValue() != -1.0f )
	{
		//평균프레임
		kPacket.m_fAvgFps = kAvgFps.GetValue();
		kPacket.m_fAvgFps_StdDev = kAvgFps.GetValue_StdDev();
		//UDP Ping
		kPacket.m_fP2PMinPingTime = kP2PPing.GetMinPingTime();
		kPacket.m_fP2PMaxPingTime = kP2PPing.GetMaxPingTime();
		kPacket.m_fP2PAvgPingTime = kP2PPing.GetAvgPingTime();
		kPacket.m_fP2PAvgPingTime_StdDev = kP2PPing.GetAvgPingTime_StdDev();
		kPacket.m_fP2PAvgPingLossRate = kP2PPing.GetAvgPingLossRate();
		kPacket.m_fRelayMinPingTime = kRelayPing.GetMinPingTime();
		kPacket.m_fRelayMaxPingTime = kRelayPing.GetMaxPingTime();
		kPacket.m_fRelayAvgPingTime = kRelayPing.GetAvgPingTime();
		kPacket.m_fRelayAvgPingTime_StdDev = kRelayPing.GetAvgPingTime_StdDev();
		kPacket.m_fRelayAvgPingLossRate = kRelayPing.GetAvgPingLossRate();
		kPacket.m_fUdpMinPingTime = kUdpPing.GetMinPingTime();
		kPacket.m_fUdpMaxPingTime = kUdpPing.GetMaxPingTime();
		kPacket.m_fUdpAvgPingTime = kUdpPing.GetAvgPingTime();
		kPacket.m_fUdpAvgPingTime_StdDev = kUdpPing.GetAvgPingTime_StdDev();
		kPacket.m_fUdpAvgPingLossRate = kUdpPing.GetAvgPingLossRate();
		//TCP Ping
		kPacket.m_fTcpMinPingTime = kTcpPing.GetMinPingTime();
		kPacket.m_fTcpMaxPingTime = kTcpPing.GetMaxPingTime();
		kPacket.m_fTcpAvgPingTime = kTcpPing.GetAvgPingTime();
		kPacket.m_fTcpAvgPingTime_StdDev = kTcpPing.GetAvgPingTime_StdDev();
		//UDP Trans
		kUdpSend.GetValuePerSec( kPacket.m_fUdpSendCountPerSec_Host, kPacket.m_fUdpSendAmountPerSec_Host, kPacket.m_fUdpSendCountPerSec_NonHost, kPacket.m_fUdpSendAmountPerSec_NonHost );
		kUdpRecv.GetValuePerSec( kPacket.m_fUdpRecvCountPerSec_Host, kPacket.m_fUdpRecvAmountPerSec_Host, kPacket.m_fUdpRecvCountPerSec_NonHost, kPacket.m_fUdpRecvAmountPerSec_NonHost );
		//UDP 유닛싱크 로스율
		kPacket.m_fUdpUserSyncPacketLossRate = kUdpUserSyncPacketLossRate.GetValue();
		kPacket.m_fUdpNpcSyncPacketLossRate = kUdpNpcSyncPacketLossRate.GetValue();
		//비호스트 평균 NPC 리액션 delay
		kPacket.m_fNonHostNpcReaction_HostUdpAvgPingTime = kNonHostNpcReactionDelay.GetHostUdpAvgPingTime();
		kPacket.m_fNonHostNpcReaction_MinDelayTime_SimulOff = kNonHostNpcReactionDelay.GetMinDelayTime_SimulOff();
		kPacket.m_fNonHostNpcReaction_MaxDelayTime_SimulOff = kNonHostNpcReactionDelay.GetMaxDelayTime_SimulOff();
		kPacket.m_fNonHostNpcReaction_AvgDelayTime_SimulOff = kNonHostNpcReactionDelay.GetAvgDelayTime_SimulOff();
		kPacket.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOff = kNonHostNpcReactionDelay.GetAvgDelayTime_StdDev_SimulOff();
		kPacket.m_fNonHostNpcReaction_MinDelayTime_SimulOn = kNonHostNpcReactionDelay.GetMinDelayTime_SimulOn();
		kPacket.m_fNonHostNpcReaction_MaxDelayTime_SimulOn = kNonHostNpcReactionDelay.GetMaxDelayTime_SimulOn();
		kPacket.m_fNonHostNpcReaction_AvgDelayTime_SimulOn = kNonHostNpcReactionDelay.GetAvgDelayTime_SimulOn();
		kPacket.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOn = kNonHostNpcReactionDelay.GetAvgDelayTime_StdDev_SimulOn();
		//메모리 사용량
		static bool bFirstMemUsageCheck = true;
		kPacket.m_bFirstMemUsageCheck = bFirstMemUsageCheck;
		kPacket.m_fMinWorkingSetSize = kMemUsage.GetMinWorkingSetSize();
		kPacket.m_fMaxWorkingSetSize = kMemUsage.GetMaxWorkingSetSize();
		kPacket.m_fMinPagefileUsage = kMemUsage.GetMinPagefileUsage();
		kPacket.m_fMaxPagefileUsage = kMemUsage.GetMaxPagefileUsage();
		bFirstMemUsageCheck = false;
		//부가정보
		kPacket.m_iPartyType = kDungeonInfo.m_ePartyType;
		kPacket.m_iPartyUserNum = kDungeonInfo.m_iPartyUserNum;
		kPacket.m_iMapID = kDungeonInfo.m_iMapID;
		kPacket.m_iDifficulty = kDungeonInfo.m_iDifficulty;
		kPacket.m_bEscape = kDungeonInfo.m_bEscape;
		kPacket.m_iStage = kDungeonInfo.m_iStage;
		kPacket.m_iBlock = kDungeonInfo.m_iBlock;

		//
		bExistData = true;

#ifndef _SERVICE_
		{
			FILE* pPingLog_Dungeon = fopen( "PingLog_Dungeon.txt", "a+" );

			//udp ping 로그
			fprintf( pPingLog_Dungeon, "---- Dungeon Start ----\n" );
			fprintf( pPingLog_Dungeon, "P2P MinPingTime: %f\n", kPacket.m_fP2PMinPingTime );
			fprintf( pPingLog_Dungeon, "P2P MaxPingTime: %f\n", kPacket.m_fP2PMaxPingTime );
			fprintf( pPingLog_Dungeon, "P2P AvgPingTime: %f\n", kPacket.m_fP2PAvgPingTime );
			fprintf( pPingLog_Dungeon, "P2P AvgPingTime StdDev: %f\n", kPacket.m_fP2PAvgPingTime_StdDev );
			fprintf( pPingLog_Dungeon, "P2P AvgPingLossRate: %f\n", kPacket.m_fP2PAvgPingLossRate );
			fprintf( pPingLog_Dungeon, "Relay MinPingTime: %f\n", kPacket.m_fRelayMinPingTime );
			fprintf( pPingLog_Dungeon, "Relay MaxPingTime: %f\n", kPacket.m_fRelayMaxPingTime );
			fprintf( pPingLog_Dungeon, "Relay AvgPingTime: %f\n", kPacket.m_fRelayAvgPingTime );
			fprintf( pPingLog_Dungeon, "Relay AvgPingTime StdDev: %f\n", kPacket.m_fRelayAvgPingTime_StdDev );
			fprintf( pPingLog_Dungeon, "Relay AvgPingLossRate: %f\n", kPacket.m_fRelayAvgPingLossRate );
			fprintf( pPingLog_Dungeon, "UDP MinPingTime: %f\n", kPacket.m_fUdpMinPingTime );
			fprintf( pPingLog_Dungeon, "UDP MaxPingTime: %f\n", kPacket.m_fUdpMaxPingTime );
			fprintf( pPingLog_Dungeon, "UDP AvgPingTime: %f\n", kPacket.m_fUdpAvgPingTime );
			fprintf( pPingLog_Dungeon, "UDP AvgPingTime StdDev: %f\n", kPacket.m_fUdpAvgPingTime_StdDev );
			fprintf( pPingLog_Dungeon, "UDP AvgPingLossRate: %f\n", kPacket.m_fUdpAvgPingLossRate );

			//tcp ping 로그
			fprintf( pPingLog_Dungeon, "TCP MinPingTime: %f\n", kPacket.m_fTcpMinPingTime );
			fprintf( pPingLog_Dungeon, "TCP MaxPingTime: %f\n", kPacket.m_fTcpMaxPingTime );
			fprintf( pPingLog_Dungeon, "TCP AvgPingTime: %f\n", kPacket.m_fTcpAvgPingTime );
			fprintf( pPingLog_Dungeon, "TCP AvgPingTime StdDev: %f\n", kPacket.m_fTcpAvgPingTime_StdDev );

			//UDP 전송측정 로그
			fprintf( pPingLog_Dungeon, "UDP SendCountPerSec(Host): %f\n", kPacket.m_fUdpSendCountPerSec_Host );
			fprintf( pPingLog_Dungeon, "UDP SendAmountPerSec(Host): %f\n", kPacket.m_fUdpSendAmountPerSec_Host );
			fprintf( pPingLog_Dungeon, "UDP RecvCountPerSec(Host): %f\n", kPacket.m_fUdpRecvCountPerSec_Host );
			fprintf( pPingLog_Dungeon, "UDP RecvAmountPerSec(Host): %f\n", kPacket.m_fUdpRecvAmountPerSec_Host );
			fprintf( pPingLog_Dungeon, "UDP SendCountPerSec(NonHost): %f\n", kPacket.m_fUdpSendCountPerSec_NonHost );
			fprintf( pPingLog_Dungeon, "UDP SendAmountPerSec(NonHost): %f\n", kPacket.m_fUdpSendAmountPerSec_NonHost );
			fprintf( pPingLog_Dungeon, "UDP RecvCountPerSec(NonHost): %f\n", kPacket.m_fUdpRecvCountPerSec_NonHost );
			fprintf( pPingLog_Dungeon, "UDP RecvAmountPerSec(NonHost): %f\n", kPacket.m_fUdpRecvAmountPerSec_NonHost );

			//비호스트 평균 NPC 리액션 delay
			fprintf( pPingLog_Dungeon, "NonHostNpcReaction HostUdpAvgPingTime: %f\n", kPacket.m_fNonHostNpcReaction_HostUdpAvgPingTime );
			fprintf( pPingLog_Dungeon, "NonHostNpcReaction MinDealyTime_SimulOff: %f\n", kPacket.m_fNonHostNpcReaction_MinDelayTime_SimulOff );
			fprintf( pPingLog_Dungeon, "NonHostNpcReaction MaxDealyTime_SimulOff: %f\n", kPacket.m_fNonHostNpcReaction_MaxDelayTime_SimulOff );
			fprintf( pPingLog_Dungeon, "NonHostNpcReaction AvgDealyTime_SimulOff: %f\n", kPacket.m_fNonHostNpcReaction_AvgDelayTime_SimulOff );
			fprintf( pPingLog_Dungeon, "NonHostNpcReaction AvgDealyTime_StdDev_SimulOff: %f\n", kPacket.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOff );
			fprintf( pPingLog_Dungeon, "NonHostNpcReaction MinDealyTime_SimulOn: %f\n", kPacket.m_fNonHostNpcReaction_MinDelayTime_SimulOn );
			fprintf( pPingLog_Dungeon, "NonHostNpcReaction MaxDealyTime_SimulOn: %f\n", kPacket.m_fNonHostNpcReaction_MaxDelayTime_SimulOn );
			fprintf( pPingLog_Dungeon, "NonHostNpcReaction AvgDealyTime_SimulOn: %f\n", kPacket.m_fNonHostNpcReaction_AvgDelayTime_SimulOn );
			fprintf( pPingLog_Dungeon, "NonHostNpcReaction AvgDealyTime_StdDev_SimulOn: %f\n", kPacket.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOn );

			//UDP 유닛싱크 로스율
			fprintf( pPingLog_Dungeon, "UDP UserSyncPacketLossRate: %f\n", kPacket.m_fUdpUserSyncPacketLossRate );
			fprintf( pPingLog_Dungeon, "UDP NpcSyncPacketLossRate: %f\n", kPacket.m_fUdpNpcSyncPacketLossRate );

			fprintf( pPingLog_Dungeon, "---- Dungeon End ----\n\n" );

			fclose( pPingLog_Dungeon );
		}
#endif//_SERVICE_
	}

	//알려지지 않은 랙 정보를 서버로 전송
	const KLagCheckManager::KDungeonInfo& kUnKnownLagInfo = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetUnKnownLagInfo_Dungeon();

	if( kUnKnownLagInfo.GetLagTime() != -1.0f )
	{
		kPacket.m_kUnKnownLagInfo.m_iLagType = kUnKnownLagInfo.GetLagType();
		kPacket.m_kUnKnownLagInfo.m_iPartyType = kUnKnownLagInfo.GetPartyType();
		kPacket.m_kUnKnownLagInfo.m_iPartyUserNum = kUnKnownLagInfo.GetPartyUserNum();
		kPacket.m_kUnKnownLagInfo.m_iMapID = kUnKnownLagInfo.GetMapID();
		kPacket.m_kUnKnownLagInfo.m_iDifficulty = kUnKnownLagInfo.GetDifficulty();
		kPacket.m_kUnKnownLagInfo.m_iStage = kUnKnownLagInfo.GetStage();
		kPacket.m_kUnKnownLagInfo.m_iBlock = kUnKnownLagInfo.GetBlock();
		kPacket.m_kUnKnownLagInfo.m_fLagTime = kUnKnownLagInfo.GetLagTime();
		kPacket.m_kUnKnownLagInfo.m_fPlayTime = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetPlayTime();
		kPacket.m_kUnKnownLagInfo.m_iLagTotalNum = kUnKnownLagInfo.GetLagTotalNum();
		kPacket.m_kUnKnownLagInfo.m_fLagTotalTime = kUnKnownLagInfo.GetLagTotalTime();

		//
		bExistData = true;
	}

	//알려진 랙 정보를 서버로 전송
	const KLagCheckManager::KDungeonInfo& kKnownLagInfo = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetKnownLagInfo_Dungeon();

	if( kKnownLagInfo.GetLagTime() != -1.0f )
	{
		kPacket.m_kKnownLagInfo.m_iLagType = kKnownLagInfo.GetLagType();
		kPacket.m_kKnownLagInfo.m_iPartyType = kKnownLagInfo.GetPartyType();
		kPacket.m_kKnownLagInfo.m_iPartyUserNum = kKnownLagInfo.GetPartyUserNum();
		kPacket.m_kKnownLagInfo.m_iMapID = kKnownLagInfo.GetMapID();
		kPacket.m_kKnownLagInfo.m_iDifficulty = kKnownLagInfo.GetDifficulty();
		kPacket.m_kKnownLagInfo.m_iStage = kKnownLagInfo.GetStage();
		kPacket.m_kKnownLagInfo.m_iBlock = kKnownLagInfo.GetBlock();
		kPacket.m_kKnownLagInfo.m_fLagTime = kKnownLagInfo.GetLagTime();
		kPacket.m_kKnownLagInfo.m_fPlayTime = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetPlayTime();
		kPacket.m_kKnownLagInfo.m_iLagTotalNum = kKnownLagInfo.GetLagTotalNum();
		kPacket.m_kKnownLagInfo.m_fLagTotalTime = kKnownLagInfo.GetLagTotalTime();

		//
		bExistData = true;
	}

	if( bExistData )
	{
		//먼가 측정한 수치가 존재한다면 서버로 보낸다.
		g_pData->GetServerProtocol()->SendPacket( EGS_KOG_GAME_PERFORMANCE_DUNGEON_LOG, kPacket );
	}
}

void KOGGamePerformanceCheckPort_Function::SendServer_PvP( const KOGGamePerformanceCheck::KPvPInfo& kPvPInfo )
{
	bool bExistData = false;
	KEGS_KOG_GAME_PERFORMANCE_PVP_LOG kPacket;

	//캐릭터 UID 얻기
	kPacket.m_iCharUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	//시스템 키 얻기
	kPacket.m_iSystemKey = g_pMain->GetSystemInfo()->GetMacAddress();

	//평균프레임, UDP Ping, TCP Ping 정보를 서버로 전송
	const KAvgFpsCheckManager::KAvgFps& kAvgFps = KOGGamePerformanceCheck::GetInstance()->GetAvgFpsCheckMgr()->GetAvgFps();
	const KPingCheckManager::KPing& kP2PPing = KOGGamePerformanceCheck::GetInstance()->GetP2PPingCheckMgr()->GetPing();
	const KPingCheckManager::KPing& kRelayPing = KOGGamePerformanceCheck::GetInstance()->GetRelayPingCheckMgr()->GetPing();
	const KPingCheckManager::KPing& kUdpPing = KOGGamePerformanceCheck::GetInstance()->GetUdpPingCheckMgr()->GetPing();
	const KPingCheckManager::KPing& kTcpPing = KOGGamePerformanceCheck::GetInstance()->GetTcpPingCheckMgr()->GetPing();
	const KUDPTransCheckManager::KTrans& kUdpSend = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetSend();
	const KUDPTransCheckManager::KTrans& kUdpRecv = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetRecv();
	const KUDPTransCheckManager::KUnitSyncPacketLossRate& kUdpUserSyncPacketLossRate = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetUserSyncPacketLossRate();
	const KUDPTransCheckManager::KUnitSyncPacketLossRate& kUdpNpcSyncPacketLossRate = KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->GetNpcSyncPacketLossRate();
	const KMemUsageCheckManager::KMemUsage& kMemUsage = KOGGamePerformanceCheck::GetInstance()->GetMemUsageCheckMgr()->GetMemUsage();

	if( kAvgFps.GetValue() != -1.0f )
	{
		//평균프레임
		kPacket.m_fAvgFps = kAvgFps.GetValue();
		kPacket.m_fAvgFps_StdDev = kAvgFps.GetValue_StdDev();
		//UDP Ping
		kPacket.m_fP2PMinPingTime = kP2PPing.GetMinPingTime();
		kPacket.m_fP2PMaxPingTime = kP2PPing.GetMaxPingTime();
		kPacket.m_fP2PAvgPingTime = kP2PPing.GetAvgPingTime();
		kPacket.m_fP2PAvgPingTime_StdDev = kP2PPing.GetAvgPingTime_StdDev();
		kPacket.m_fP2PAvgPingLossRate = kP2PPing.GetAvgPingLossRate();
		kPacket.m_fRelayMinPingTime = kRelayPing.GetMinPingTime();
		kPacket.m_fRelayMaxPingTime = kRelayPing.GetMaxPingTime();
		kPacket.m_fRelayAvgPingTime = kRelayPing.GetAvgPingTime();
		kPacket.m_fRelayAvgPingTime_StdDev = kRelayPing.GetAvgPingTime_StdDev();
		kPacket.m_fRelayAvgPingLossRate = kRelayPing.GetAvgPingLossRate();
		kPacket.m_fUdpMinPingTime = kUdpPing.GetMinPingTime();
		kPacket.m_fUdpMaxPingTime = kUdpPing.GetMaxPingTime();
		kPacket.m_fUdpAvgPingTime = kUdpPing.GetAvgPingTime();
		kPacket.m_fUdpAvgPingTime_StdDev = kUdpPing.GetAvgPingTime_StdDev();
		kPacket.m_fUdpAvgPingLossRate = kUdpPing.GetAvgPingLossRate();
		//TCP Ping
		kPacket.m_fTcpMinPingTime = kTcpPing.GetMinPingTime();
		kPacket.m_fTcpMaxPingTime = kTcpPing.GetMaxPingTime();
		kPacket.m_fTcpAvgPingTime = kTcpPing.GetAvgPingTime();
		kPacket.m_fTcpAvgPingTime_StdDev = kTcpPing.GetAvgPingTime_StdDev();
		//UDP Trans
		kUdpSend.GetValuePerSec( kPacket.m_fUdpSendCountPerSec_Host, kPacket.m_fUdpSendAmountPerSec_Host, kPacket.m_fUdpSendCountPerSec_NonHost, kPacket.m_fUdpSendAmountPerSec_NonHost );
		kUdpRecv.GetValuePerSec( kPacket.m_fUdpRecvCountPerSec_Host, kPacket.m_fUdpRecvAmountPerSec_Host, kPacket.m_fUdpRecvCountPerSec_NonHost, kPacket.m_fUdpRecvAmountPerSec_NonHost );
		//UDP 유닛싱크 로스율
		kPacket.m_fUdpUserSyncPacketLossRate = kUdpUserSyncPacketLossRate.GetValue();
		kPacket.m_fUdpNpcSyncPacketLossRate = kUdpNpcSyncPacketLossRate.GetValue();
		//메모리 사용량
		static bool bFirstMemUsageCheck = true;
		kPacket.m_bFirstMemUsageCheck = bFirstMemUsageCheck;
		kPacket.m_fMinWorkingSetSize = kMemUsage.GetMinWorkingSetSize();
		kPacket.m_fMaxWorkingSetSize = kMemUsage.GetMaxWorkingSetSize();
		kPacket.m_fMinPagefileUsage = kMemUsage.GetMinPagefileUsage();
		kPacket.m_fMaxPagefileUsage = kMemUsage.GetMaxPagefileUsage();
		bFirstMemUsageCheck = false;
		//부가정보
		kPacket.m_iMapID = kPvPInfo.m_iMapID;
		kPacket.m_iPvPUserNum = kPvPInfo.m_iPvPUserNum;
		kPacket.m_iRuleType = kPvPInfo.m_iRuleType;
		kPacket.m_bEscape = kPvPInfo.m_bEscape;
		kPacket.m_iRound = kPvPInfo.m_iRound;

		//
		bExistData = true;

#ifndef _SERVICE_
		{
			FILE* pPingLog_PvP = fopen( "PingLog_PvP.txt", "a+" );

			//udp ping 로그
			fprintf( pPingLog_PvP, "---- PvP Start ----\n" );
			fprintf( pPingLog_PvP, "P2P MinPingTime: %f\n", kPacket.m_fP2PMinPingTime );
			fprintf( pPingLog_PvP, "P2P MaxPingTime: %f\n", kPacket.m_fP2PMaxPingTime );
			fprintf( pPingLog_PvP, "P2P AvgPingTime: %f\n", kPacket.m_fP2PAvgPingTime );
			fprintf( pPingLog_PvP, "P2P AvgPingTime StdDev: %f\n", kPacket.m_fP2PAvgPingTime_StdDev );
			fprintf( pPingLog_PvP, "P2P AvgPingLossRate: %f\n", kPacket.m_fP2PAvgPingLossRate );
			fprintf( pPingLog_PvP, "Relay MinPingTime: %f\n", kPacket.m_fRelayMinPingTime );
			fprintf( pPingLog_PvP, "Relay MaxPingTime: %f\n", kPacket.m_fRelayMaxPingTime );
			fprintf( pPingLog_PvP, "Relay AvgPingTime: %f\n", kPacket.m_fRelayAvgPingTime );
			fprintf( pPingLog_PvP, "Relay AvgPingTime StdDev: %f\n", kPacket.m_fRelayAvgPingTime_StdDev );
			fprintf( pPingLog_PvP, "Relay AvgPingLossRate: %f\n", kPacket.m_fRelayAvgPingLossRate );
			fprintf( pPingLog_PvP, "UDP MinPingTime: %f\n", kPacket.m_fUdpMinPingTime );
			fprintf( pPingLog_PvP, "UDP MaxPingTime: %f\n", kPacket.m_fUdpMaxPingTime );
			fprintf( pPingLog_PvP, "UDP AvgPingTime: %f\n", kPacket.m_fUdpAvgPingTime );
			fprintf( pPingLog_PvP, "UDP AvgPingTime StdDev: %f\n", kPacket.m_fUdpAvgPingTime_StdDev );
			fprintf( pPingLog_PvP, "UDP AvgPingLossRate: %f\n", kPacket.m_fUdpAvgPingLossRate );

			//tcp ping 로그
			fprintf( pPingLog_PvP, "TCP MinPingTime: %f\n", kPacket.m_fTcpMinPingTime );
			fprintf( pPingLog_PvP, "TCP MaxPingTime: %f\n", kPacket.m_fTcpMaxPingTime );
			fprintf( pPingLog_PvP, "TCP AvgPingTime: %f\n", kPacket.m_fTcpAvgPingTime );
			fprintf( pPingLog_PvP, "TCP AvgPingTime StdDev: %f\n", kPacket.m_fTcpAvgPingTime_StdDev );

			//UDP 전송측정 로그
			fprintf( pPingLog_PvP, "UDP SendCountPerSec(Host): %f\n", kPacket.m_fUdpSendCountPerSec_Host );
			fprintf( pPingLog_PvP, "UDP SendAmountPerSec(Host): %f\n", kPacket.m_fUdpSendAmountPerSec_Host );
			fprintf( pPingLog_PvP, "UDP RecvCountPerSec(Host): %f\n", kPacket.m_fUdpRecvCountPerSec_Host );
			fprintf( pPingLog_PvP, "UDP RecvAmountPerSec(Host): %f\n", kPacket.m_fUdpRecvAmountPerSec_Host );
			fprintf( pPingLog_PvP, "UDP SendCountPerSec(NonHost): %f\n", kPacket.m_fUdpSendCountPerSec_NonHost );
			fprintf( pPingLog_PvP, "UDP SendAmountPerSec(NonHost): %f\n", kPacket.m_fUdpSendAmountPerSec_NonHost );
			fprintf( pPingLog_PvP, "UDP RecvCountPerSec(NonHost): %f\n", kPacket.m_fUdpRecvCountPerSec_NonHost );
			fprintf( pPingLog_PvP, "UDP RecvAmountPerSec(NonHost): %f\n", kPacket.m_fUdpRecvAmountPerSec_NonHost );

			//UDP 유닛싱크 로스율
			fprintf( pPingLog_PvP, "UDP UserSyncPacketLossRate: %f\n", kPacket.m_fUdpUserSyncPacketLossRate );
			fprintf( pPingLog_PvP, "UDP NpcSyncPacketLossRate: %f\n", kPacket.m_fUdpNpcSyncPacketLossRate );

			fprintf( pPingLog_PvP, "---- PvP End ----\n\n" );

			fclose( pPingLog_PvP );
		}
#endif//_SERVICE_
	}

	//알려지지 않은 랙 정보를 서버로 전송
	const KLagCheckManager::KPvPInfo& kUnKnownLagInfo = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetUnKnownLagInfo_PvP();

	if( kUnKnownLagInfo.GetLagTime() != -1.0f )
	{
		kPacket.m_kUnKnownLagInfo.m_iLagType = kUnKnownLagInfo.GetLagType();
		kPacket.m_kUnKnownLagInfo.m_iMapID = kUnKnownLagInfo.GetMapID();
		kPacket.m_kUnKnownLagInfo.m_iPvPUserNum = kUnKnownLagInfo.GetPvPUserNum();
		kPacket.m_kUnKnownLagInfo.m_iRuleType = kUnKnownLagInfo.GetRuleType();
		kPacket.m_kUnKnownLagInfo.m_iRound = kUnKnownLagInfo.GetRound();
		kPacket.m_kUnKnownLagInfo.m_fLagTime = kUnKnownLagInfo.GetLagTime();
		kPacket.m_kUnKnownLagInfo.m_fPlayTime = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetPlayTime();
		kPacket.m_kUnKnownLagInfo.m_iLagTotalNum = kUnKnownLagInfo.GetLagTotalNum();
		kPacket.m_kUnKnownLagInfo.m_fLagTotalTime = kUnKnownLagInfo.GetLagTotalTime();

		//
		bExistData = true;
	}

	//알려진 랙 정보를 서버로 전송
	const KLagCheckManager::KPvPInfo& kKnownLagInfo = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetKnownLagInfo_PvP();

	if( kKnownLagInfo.GetLagTime() != -1.0f )
	{
		kPacket.m_kKnownLagInfo.m_iLagType = kKnownLagInfo.GetLagType();
		kPacket.m_kKnownLagInfo.m_iMapID = kKnownLagInfo.GetMapID();
		kPacket.m_kKnownLagInfo.m_iPvPUserNum = kKnownLagInfo.GetPvPUserNum();
		kPacket.m_kKnownLagInfo.m_iRuleType = kUnKnownLagInfo.GetRuleType();
		kPacket.m_kKnownLagInfo.m_iRound = kKnownLagInfo.GetRound();
		kPacket.m_kKnownLagInfo.m_fLagTime = kKnownLagInfo.GetLagTime();
		kPacket.m_kKnownLagInfo.m_fPlayTime = KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->GetPlayTime();
		kPacket.m_kKnownLagInfo.m_iLagTotalNum = kKnownLagInfo.GetLagTotalNum();
		kPacket.m_kKnownLagInfo.m_fLagTotalTime = kKnownLagInfo.GetLagTotalTime();

		//
		bExistData = true;
	}

	if( bExistData )
	{
		//먼가 측정한 수치가 존재한다면 서버로 보낸다.
		g_pData->GetServerProtocol()->SendPacket( EGS_KOG_GAME_PERFORMANCE_PVP_LOG, kPacket );
	}
}

bool KOGGamePerformanceCheckPort_Function::IsValidGameSpace_AvgFps( int iGameSpace )
{
	if( iGameSpace == CX2Main::XS_VILLAGE_MAP ||
		iGameSpace == CX2Main::XS_BATTLE_FIELD ||
		iGameSpace == CX2Main::XS_DUNGEON_GAME ||
		iGameSpace == CX2Main::XS_PVP_GAME )
		return true;
	return false;
}

bool KOGGamePerformanceCheckPort_Function::IsUnKnownLagType_Lag( int iLagType )
{
	if( iLagType == eUnKnown_LagCheckType_OnIdle ||
		iLagType == eUnKnown_LagCheckType_Simulation ||
		iLagType == eUnKnown_LagCheckType_Rendering )
		return true;
	return false;
}

bool KOGGamePerformanceCheckPort_Function::IsValidGameSpace_Lag( int iGameSpace )
{
	if( iGameSpace == CX2Main::XS_VILLAGE_MAP ||
		iGameSpace == CX2Main::XS_BATTLE_FIELD ||
		iGameSpace == CX2Main::XS_DUNGEON_GAME ||
		iGameSpace == CX2Main::XS_PVP_GAME )
		return true;
	return false;
}

bool KOGGamePerformanceCheckPort_Function::IsValidGameSpace_Ping( int iGameSpace )
{
	if( iGameSpace == CX2Main::XS_BATTLE_FIELD ||
		iGameSpace == CX2Main::XS_DUNGEON_GAME ||
		iGameSpace == CX2Main::XS_PVP_GAME )
		return true;
	return false;
}

bool KOGGamePerformanceCheckPort_Function::IsValidGameSpace_UDPTrans( int iGameSpace )
{
	if( iGameSpace == CX2Main::XS_BATTLE_FIELD ||
		iGameSpace == CX2Main::XS_DUNGEON_GAME ||
		iGameSpace == CX2Main::XS_PVP_GAME )
		return true;
	return false;
}

bool KOGGamePerformanceCheckPort_Function::IsValidGameSpace_NonHostNpcReaction( int iGameSpace )
{
	if( iGameSpace == CX2Main::XS_BATTLE_FIELD ||
		iGameSpace == CX2Main::XS_DUNGEON_GAME )
		return true;
	return false;
}

bool KOGGamePerformanceCheckPort_Function::IsValidGameSpace_MemUsage( int iGameSpace )
{
	if( iGameSpace == CX2Main::XS_VILLAGE_MAP ||
		iGameSpace == CX2Main::XS_BATTLE_FIELD ||
		iGameSpace == CX2Main::XS_DUNGEON_GAME ||
		iGameSpace == CX2Main::XS_PVP_GAME )
		return true;
	return false;
}
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
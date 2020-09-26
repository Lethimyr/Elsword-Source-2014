#pragma once

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#include <Windows.h>

#include "KOGGamePerformanceCheck.h"

class KOGGamePerformanceCheck_CallbackFuncTable
{
public:
	static KOGGamePerformanceCheck_CallbackFuncTable* GetInstance();
	static void DeleteInstance();

private:
	static KOGGamePerformanceCheck_CallbackFuncTable* m_pkInstance;

public:
	//평균 프레임, 랙 측정 공통
	typedef int (*GetGameSpace)();

	typedef bool (*IsGameSpaceVillage)( int iGameSpace );
	typedef bool (*IsGameSpaceField)( int iGameSpace );
	typedef bool (*IsGameSpaceDungeon)( int iGameSpace );
	typedef bool (*IsGameSpacePvP)( int iGameSpace );

	typedef KOGGamePerformanceCheck::EPartyType (*GetVillagePartyType)();
	typedef int (*GetVillagePartyUserNum)();
	typedef int (*GetVillageMapID)();
	typedef int (*GetVillageUserNum)();

	typedef KOGGamePerformanceCheck::EPartyType (*GetFieldPartyType)();
	typedef int (*GetFieldPartyUserNum)();
	typedef int (*GetFieldMapID)();
	typedef int (*GetFieldUserNum)();

	typedef KOGGamePerformanceCheck::EPartyType (*GetDungeonPartyType)();
	typedef int (*GetDungeonPartyUserNum)();
	typedef int (*GetDungeonMapID)();
	typedef int (*GetDungeonDifficulty)();
	typedef bool (*IsDungeonEscape)();
	typedef int (*GetDungeonStage)();
	typedef int (*GetDungeonBlock)();
	typedef int (*GetDungeonUserNum)();

	typedef int (*GetPvPMapID)();
	typedef int (*GetPvPUserNum)();
	typedef int (*GetPvPRuleType)();
	typedef bool (*IsPvPEscape)();
	typedef int (*GetPvPRound)();
	typedef int (*GetPvPObserverNum)();

	typedef float (*GetFramePerSecond)();
	typedef void (*ResetFramePerSecond)();

	typedef bool (*IsGameHost)();
	
	typedef void (*SendServer_Village)( const KOGGamePerformanceCheck::KVillageInfo& kVillageInfo );
	typedef void (*SendServer_Field)( const KOGGamePerformanceCheck::KFieldInfo& kFieldInfo );
	typedef void (*SendServer_Dungeon)( const KOGGamePerformanceCheck::KDungeonInfo& kDungeonInfo );
	typedef void (*SendServer_PvP)( const KOGGamePerformanceCheck::KPvPInfo& kPvPInfo );

	//평균 프레임 용
	typedef bool (*IsValidGameSpace_AvgFps)( int iGameSpace );

	//랙 측정 용
	typedef bool (*IsUnKnownLagType_Lag)( int iLagType );
	typedef bool (*IsValidGameSpace_Lag)( int iGameSpace );

	//ping 측정 용
	typedef bool (*IsValidGameSpace_Ping)( int iGameSpace );

	//UDP 전송측정 용
	typedef bool (*IsValidGameSpace_UDPTrans)( int iGameSpace );
	
	//비호스트 평균 NPC 리액션 측정 용
	typedef bool (*IsValidGameSpace_NonHostNpcReaction)( int iGameSpace );

	//memory usage 측정 용
	typedef bool (*IsValidGameSpace_MemUsage)( int iGameSpace );
	
	//
	KOGGamePerformanceCheck_CallbackFuncTable();
	~KOGGamePerformanceCheck_CallbackFuncTable();

	//평균 프레임, 랙 측정 공통
	void RegisterCallbackFunc_GetGameSpace( GetGameSpace pfnCallback ) { m_pfnGetGameSpace = pfnCallback; }

	void RegisterCallbackFunc_IsGameSpaceVillage( IsGameSpaceVillage pfnCallback ) { m_pfnIsGameSpaceVillage = pfnCallback; }
	void RegisterCallbackFunc_IsGameSpaceField( IsGameSpaceField pfnCallback ) { m_pfnIsGameSpaceField = pfnCallback; }
	void RegisterCallbackFunc_IsGameSpaceDungeon( IsGameSpaceDungeon pfnCallback ) { m_pfnIsGameSpaceDungeon = pfnCallback; }
	void RegisterCallbackFunc_IsGameSpacePvP( IsGameSpacePvP pfnCallback ) { m_pfnIsGameSpacePvP = pfnCallback; }

	void RegisterCallbackFunc_GetVillagePartyType( GetVillagePartyType pfnCallback ) { m_pfnGetVillagePartyType = pfnCallback; }
	void RegisterCallbackFunc_GetVillagePartyUserNum( GetVillagePartyUserNum pfnCallback ) { m_pfnGetVillagePartyUserNum = pfnCallback; }
	void RegisterCallbackFunc_GetVillageMapID( GetVillageMapID pfnCallback ) { m_pfnGetVillageMapID = pfnCallback; }
	void RegisterCallbackFunc_GetVillageUserNum( GetVillageUserNum pfnCallback ) { m_pfnGetVillageUserNum = pfnCallback; }

	void RegisterCallbackFunc_GetFieldPartyType( GetFieldPartyType pfnCallback ) { m_pfnGetFieldPartyType = pfnCallback; }
	void RegisterCallbackFunc_GetFieldPartyUserNum( GetFieldPartyUserNum pfnCallback ) { m_pfnGetFieldPartyUserNum = pfnCallback; }
	void RegisterCallbackFunc_GetFieldMapID( GetFieldMapID pfnCallback ) { m_pfnGetFieldMapID = pfnCallback; }
	void RegisterCallbackFunc_GetFieldUserNum( GetFieldUserNum pfnCallback ) { m_pfnGetFieldUserNum = pfnCallback; }

	void RegisterCallbackFunc_GetDungeonPartyType( GetDungeonPartyType pfnCallback ) { m_pfnGetDungeonPartyType = pfnCallback; }
	void RegisterCallbackFunc_GetDungeonPartyUserNum( GetDungeonPartyUserNum pfnCallback ) { m_pfnGetDungeonPartyUserNum = pfnCallback; }
	void RegisterCallbackFunc_GetDungeonMapID( GetDungeonMapID pfnCallback ) { m_pfnGetDungeonMapID = pfnCallback; }
	void RegisterCallbackFunc_GetDungeonDifficulty( GetDungeonDifficulty pfnCallback ) { m_pfnGetDungeonDifficulty = pfnCallback; }
	void RegisterCallbackFunc_IsDungeonEscape( IsDungeonEscape pfnCallback ) { m_pfnIsDungeonEscape = pfnCallback; }
	void RegisterCallbackFunc_GetDungeonStage( GetDungeonStage pfnCallback ) { m_pfnGetDungeonStage = pfnCallback; }
	void RegisterCallbackFunc_GetDungeonBlock( GetDungeonBlock pfnCallback ) { m_pfnGetDungeonBlock = pfnCallback; }

	void RegisterCallbackFunc_GetPvPMapID( GetPvPMapID pfnCallback ) { m_pfnGetPvPMapID = pfnCallback; }
	void RegisterCallbackFunc_GetPvPUserNum( GetPvPUserNum pfnCallback ) { m_pfnGetPvPUserNum = pfnCallback; }
	void RegisterCallbackFunc_GetPvPRuleType( GetPvPRuleType pfnCallback ) { m_pfnGetPvPRuleType = pfnCallback; }
	void RegisterCallbackFunc_IsPvPEscape( IsPvPEscape pfnCallback ) { m_pfnIsPvPEscape = pfnCallback; }
	
	void RegisterCallbackFunc_GetPvPRound( GetPvPRound pfnCallback ) { m_pfnGetPvPRound = pfnCallback; }
	void RegisterCallbackFunc_GetPvPObserverNum( GetPvPObserverNum pfnCallback ) { m_pfnGetPvPObserverNum = pfnCallback; }

	void RegisterCallbackFunc_GetFramePerSecond( GetFramePerSecond pfnCallback ) { m_pfnGetFramePerSecond = pfnCallback; }
	void RegisterCallbackFunc_ResetFramePerSecond( ResetFramePerSecond pfnCallback ) { m_pfnResetFramePerSecond = pfnCallback; }

	void RegisterCallbackFunc_IsGameHost( IsGameHost pfnCallback ) { m_pfnIsGameHost = pfnCallback; }
	

	void RegisterCallbackFunc_SendServer_Village( SendServer_Village pfnCallback ) { m_pfnSendServer_Village = pfnCallback; }
	void RegisterCallbackFunc_SendServer_Field( SendServer_Field pfnCallback ) { m_pfnSendServer_Field = pfnCallback; }
	void RegisterCallbackFunc_SendServer_Dungeon( SendServer_Dungeon pfnCallback ) { m_pfnSendServer_Dungeon = pfnCallback; }
	void RegisterCallbackFunc_SendServer_PvP( SendServer_PvP pfnCallback ) { m_pfnSendServer_PvP = pfnCallback; }

	//평균 프레임 용
	void RegisterCallbackFunc_IsValidGameSpace_AvgFps( IsValidGameSpace_AvgFps pfnCallback ) { m_pfnIsValidGameSpace_AvgFps = pfnCallback; }

	//랙 측정 용
	void RegisterCallbackFunc_IsUnKnownLagType_Lag( IsUnKnownLagType_Lag pfnCallback ) { m_pfnIsUnKnownLagType_Lag = pfnCallback; }
	void RegisterCallbackFunc_IsValidGameSpace_Lag( IsValidGameSpace_Lag pfnCallback ) { m_pfnIsValidGameSpace_Lag = pfnCallback; }

	//ping 측정 용
	void RegisterCallbackFunc_IsValidGameSpace_Ping( IsValidGameSpace_Ping pfnCallback ) { m_pfnIsValidGameSpace_Ping = pfnCallback; }

	//UDP 전송측정 용
	void RegisterCallbackFunc_IsValidGameSpace_UDPTrans( IsValidGameSpace_UDPTrans pfnCallback ) { m_pfnIsValidGameSpace_UDPTrans = pfnCallback; }

	//비호스트 평균 NPC 리액션 측정 용
	void RegisterCallbackFunc_IsValidGameSpace_NonHostNpcReaction( IsValidGameSpace_NonHostNpcReaction pfnCallback ) { m_pfnIsValidGameSpace_NonHostNpcReaction = pfnCallback; }
	
	//memory usage 측정 용
	void RegisterCallbackFunc_IsValidGameSpace_MemUsage( IsValidGameSpace_MemUsage pfnCallback ) { m_pfnIsValidGameSpace_MemUsage = pfnCallback; }

	//평균 프레임, 랙 측정 공통
	GetGameSpace m_pfnGetGameSpace;

	IsGameSpaceVillage m_pfnIsGameSpaceVillage;
	IsGameSpaceField m_pfnIsGameSpaceField;
	IsGameSpaceDungeon m_pfnIsGameSpaceDungeon;
	IsGameSpacePvP m_pfnIsGameSpacePvP;

	GetVillagePartyType m_pfnGetVillagePartyType;
	GetVillagePartyUserNum m_pfnGetVillagePartyUserNum;
	GetVillageMapID m_pfnGetVillageMapID;
	GetVillageUserNum m_pfnGetVillageUserNum;

	GetFieldPartyType m_pfnGetFieldPartyType;
	GetFieldPartyUserNum m_pfnGetFieldPartyUserNum;
	GetFieldMapID m_pfnGetFieldMapID;
	GetFieldUserNum m_pfnGetFieldUserNum;

	GetDungeonPartyType m_pfnGetDungeonPartyType;
	GetDungeonPartyUserNum m_pfnGetDungeonPartyUserNum;
	GetDungeonMapID m_pfnGetDungeonMapID;
	GetDungeonDifficulty m_pfnGetDungeonDifficulty;
	IsDungeonEscape m_pfnIsDungeonEscape;
	GetDungeonStage m_pfnGetDungeonStage;
	GetDungeonBlock m_pfnGetDungeonBlock;

	GetPvPMapID m_pfnGetPvPMapID;
	GetPvPUserNum m_pfnGetPvPUserNum;
	GetPvPRuleType m_pfnGetPvPRuleType;
	IsPvPEscape m_pfnIsPvPEscape;
	GetPvPRound m_pfnGetPvPRound;
	GetPvPObserverNum m_pfnGetPvPObserverNum;

	GetFramePerSecond m_pfnGetFramePerSecond;
	ResetFramePerSecond m_pfnResetFramePerSecond;

	IsGameHost m_pfnIsGameHost;

	SendServer_Village m_pfnSendServer_Village;
	SendServer_Field m_pfnSendServer_Field;
	SendServer_Dungeon m_pfnSendServer_Dungeon;
	SendServer_PvP m_pfnSendServer_PvP;

	//평균 프레임 용
	IsValidGameSpace_AvgFps m_pfnIsValidGameSpace_AvgFps;

	//랙 측정 용
	IsUnKnownLagType_Lag m_pfnIsUnKnownLagType_Lag;
	IsValidGameSpace_Lag m_pfnIsValidGameSpace_Lag;

	//ping 측정 용
	IsValidGameSpace_Ping m_pfnIsValidGameSpace_Ping;

	//UDP 전송측정 용
	IsValidGameSpace_UDPTrans m_pfnIsValidGameSpace_UDPTrans;

	//비호스트 평균 NPC 리액션 측정 용
	IsValidGameSpace_NonHostNpcReaction m_pfnIsValidGameSpace_NonHostNpcReaction;

	//memory usage 측정 용
	IsValidGameSpace_MemUsage m_pfnIsValidGameSpace_MemUsage;
};
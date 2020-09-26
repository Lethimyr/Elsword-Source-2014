#pragma once

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#include <Windows.h>

class KAvgFpsCheckManager;
class KPingCheckManager;
class KUDPTransCheckManager;
class KNonHostNpcReactionCheckManager;
class KLagCheckManager;
class KMemUsageCheckManager;

class KOGGamePerformanceCheck
{
public:
	static KOGGamePerformanceCheck* GetInstance();
	static void DeleteInstance();

private:
	static KOGGamePerformanceCheck* m_pkInstance;

public:
	enum EPartyType
	{
		ePartyType_No = 0,
		ePartyType_Host = 1,
		ePartyType_Member = 2,
		ePartyType_Observer = 3,
	};

	class KVillageInfo
	{
	public:
		KVillageInfo()
		{
			m_ePartyType = KOGGamePerformanceCheck::ePartyType_No;
			m_iPartyUserNum = -1;
			m_iMapID = -1;
			m_iVillageUserNum = -1;
		}

		KOGGamePerformanceCheck::EPartyType m_ePartyType;		//파티타입
		int m_iPartyUserNum;//파티유저수
		int m_iMapID;		//던전ID 
		int m_iVillageUserNum;	//마을유저수
	};

	class KFieldInfo
	{
	public:
		KFieldInfo()
		{
			m_ePartyType = KOGGamePerformanceCheck::ePartyType_No;
			m_iPartyUserNum = -1;
			m_iMapID = -1;
			m_iFieldUserNum = -1;
		}

		KOGGamePerformanceCheck::EPartyType m_ePartyType;		//파티타입
		int m_iPartyUserNum;//파티유저수
		int m_iMapID;		//던전ID 
		int m_iFieldUserNum;	//필드유저수
	};

	class KDungeonInfo
	{
	public:
		KDungeonInfo()
		{
			m_ePartyType = KOGGamePerformanceCheck::ePartyType_No;
			m_iPartyUserNum = -1;
			m_iMapID = -1;
			m_iDifficulty = -1;
			m_bEscape = true;
			m_iStage = -1;
			m_iBlock = -1;
		}

		KOGGamePerformanceCheck::EPartyType m_ePartyType;		//파티타입
		int m_iPartyUserNum;//파티유저수
		int m_iMapID;		//던전ID 
		int m_iDifficulty;//던전난이도
		bool m_bEscape;	//던전플레이 중 빠져나갔는지 여부
		int m_iStage;		//Stage번호 
		int m_iBlock;		//Block번호
	};

	class KPvPInfo
	{
	public:
		KPvPInfo()
		{
			m_iMapID = -1;
			m_iPvPUserNum = -1;
			m_iRuleType = -1;
			m_bEscape = true;
			m_iRound = -1;
			m_iObserverNum = -1;
		}

		int m_iMapID;		//대전맵ID 
		int m_iPvPUserNum;	//대전유저수
		int m_iRuleType;	//룰타입
		bool m_bEscape;	//대전플레이 중 빠져나갔는지 여부
		int m_iRound;		//Round번호 또는 전체 Kill수
		int m_iObserverNum;		//관중수 
	};

	KOGGamePerformanceCheck();
	~KOGGamePerformanceCheck();

	void Init( float fAvgFpsUpdateTime,//평균 
				int iUdpPingLossWindowSize,//UDP
				int iUdpPingTimeOut,//UDP
				int iMinSampleNum,//최소 샘플 수
				float fMemUsageUpdateTime, //메모리
				DWORD dwLagTimeDefinition, int iLagTypeMax//랙 
				);

	void Start( bool bUdpPing_MainThread, bool bTcpPing_MainThread );
	void Pause();
	void Resume();
	void End();

	//
	KAvgFpsCheckManager* GetAvgFpsCheckMgr() { return m_pkAvgFpsCheckManager; }
	KPingCheckManager* GetP2PPingCheckMgr() { return m_pkP2PPingCheckManager; }
	KPingCheckManager* GetRelayPingCheckMgr() { return m_pkRelayPingCheckManager; }
	KPingCheckManager* GetUdpPingCheckMgr() { return m_pkUdpPingCheckManager; }
	KPingCheckManager* GetTcpPingCheckMgr() { return m_pkTcpPingCheckManager; }
	KUDPTransCheckManager* GetUDPTransCheckMgr() { return m_pkUdpTransCheckManager; }
	KNonHostNpcReactionCheckManager* GetNonHostNpcReactionCheckMgr() { return m_pkNonHostNpcReactionCheckManager; }
	KMemUsageCheckManager* GetMemUsageCheckMgr() { return m_pkMemUsageCheckManager; }
	KLagCheckManager* GetLagCheckMgr() { return m_pkLagCheckManager; }

private:
	//
	KAvgFpsCheckManager* m_pkAvgFpsCheckManager;
	KPingCheckManager* m_pkP2PPingCheckManager;
	KPingCheckManager* m_pkRelayPingCheckManager;
	KPingCheckManager* m_pkUdpPingCheckManager;
	KPingCheckManager* m_pkTcpPingCheckManager;
	KUDPTransCheckManager* m_pkUdpTransCheckManager;
	KNonHostNpcReactionCheckManager* m_pkNonHostNpcReactionCheckManager;
	KLagCheckManager* m_pkLagCheckManager;
	KMemUsageCheckManager* m_pkMemUsageCheckManager;
};
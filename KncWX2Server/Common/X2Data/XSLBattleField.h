#pragma once
#include <string>
#include <vector>
#include "Lua/KLuaManager.h"
#include "dbg/dbg.hpp"
#include "Event.h"
#include "CommonPacket.h"

#include "X2Data/XSLUnitManager.h"
#include "Lottery.h"
#include <d3dx9math.h>
#include "Enum/Enum.h"


class CXSLBattleField
{
public:
	// 필드 내에 있는 포탈의 이동 타입
	enum PORTAL_MOVE_TYPE
	{
		PMT_INVALID 			= 0,
		PMT_MOVE_TO_VILLAGE		= 1, // 필드 -> 마을로 이동하는 포탈
		PMT_MOVE_TO_FIELD		= 2, // 필드 -> 필드로 이동하는 포탈
		PMT_END					= 3,
	};

	//////////////////////////////////////////////////////////////////////////	
	// 포탈 정보
	struct SPotalMovingInfo
	{
		PORTAL_MOVE_TYPE	m_ePortalMoveType;
		int					m_iLineNumber;
		int					m_iPlaceIDToMove;
		int					m_iPositionIndexToMove;
        
		SPotalMovingInfo()
		{
			m_ePortalMoveType		= PMT_INVALID;
			m_iLineNumber			= 0;
			m_iPlaceIDToMove		= 0;
			m_iPositionIndexToMove	= 0;
		}
	};
	
	//////////////////////////////////////////////////////////////////////////	
	// 위험도 정보
	struct SRiskInfo
	{
		//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		int					m_iSpawnID;				// 생성 ID
		int					m_iSpawnRate;			// 확률
		std::vector<int>	m_vecSpawnGroupID;		// 보스 몬스터가 속한 그룹
#else
		int					m_iSpawnGroupID;		// 보스 몬스터가 속한 그룹
		int					m_iSpawnMonsterID;		// 보스 몬스터ID
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}
		int					m_iRiskValue;
		bool				m_bShowBossName;
		bool				m_bBossGaugeHpLines;

		SRiskInfo()
		{
			//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			m_iSpawnID			= 0;
			m_iSpawnRate		= 0;
#else
			m_iSpawnGroupID		= 0;
			m_iSpawnMonsterID	= 0;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}
			m_iRiskValue		= 0;
			m_bShowBossName		= false;
			m_bBossGaugeHpLines = false;
		}
	};


	//////////////////////////////////////////////////////////////////////////	
	// 몬스터 정보
	struct SSpawnNpcInfo
	{
        CXSLUnitManager::NPC_UNIT_ID	m_eNpcID;
		float							m_fRate;
		int								m_iMaxNumberOfThisMonster;
		int								m_iAddLevel;
		bool							m_bDrop;
		bool							m_bAggressive;
        float							m_fAddPosY;	
		//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		bool				m_bFocusCamera;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}
		

		KLotterySameProb< int >			m_StartPositionLot;
		std::vector< int >				m_vecPetrolLineIndex;
		std::vector< int >				m_vecPlayLineIndex;

		SSpawnNpcInfo()
		{
			m_eNpcID		= CXSLUnitManager::NUI_NONE;
			m_fRate			= 0.f;
			m_iMaxNumberOfThisMonster = 0;
			m_iAddLevel		= 0;
			m_bDrop			= false;
			m_fAddPosY		= 0.f;
			m_bAggressive	= true;
			//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			m_bFocusCamera	= false;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}
		}
	};

	//////////////////////////////////////////////////////////////////////////	
	// 몬스터 그룹 정보
	struct SSpawnNpcGroup
	{
		bool							m_bIsBossMonsterGroup;
		int								m_iGroupID;		
		int								m_iMinNumberOfMonsterInThisGroup;
		int								m_iMaxNumberOfMonsterInThisGroup;
		std::set< int >					m_setSiegeMonster;
		std::map< int, SSpawnNpcInfo >	m_mapSpawnNpcList;

		SSpawnNpcGroup()
		{
			m_bIsBossMonsterGroup = false;
			m_iGroupID = 0;
			m_iMinNumberOfMonsterInThisGroup = 0;
			m_iMaxNumberOfMonsterInThisGroup = 0;
		}

		bool IsSiegeMonster( IN const int iNpcID ) const
		{
			return ( m_setSiegeMonster.find( iNpcID ) != m_setSiegeMonster.end() );
		}
	};

	//////////////////////////////////////////////////////////////////////////	
	// 그룹 단위 몬스터 카운트 정보
	class KGroupMonsterCountInfo
	{
	private:
		int						m_iTotalMonsterCount;
		std::map< int, int >	m_mapMonsterCount;		// [key : NpcID, value : AliveCount] 

	public:
		KGroupMonsterCountInfo() : 
		m_iTotalMonsterCount( 0 )
		{
		}

		void AddMonsterCount( IN const int iNpcID )
		{
			std::map< int, int >::iterator mit;
			mit = m_mapMonsterCount.find( iNpcID );
			if( mit == m_mapMonsterCount.end() )
			{
				m_mapMonsterCount.insert( std::make_pair( iNpcID, 1 ) );
			}
			else
			{
                ++mit->second;
			}

			++m_iTotalMonsterCount;
		}

		bool GetMonsterCount( IN const int iNpcID, OUT int& iCount ) const
		{
			iCount = 0;

			std::map< int, int >::const_iterator mit;
			mit = m_mapMonsterCount.find( iNpcID );
			if( mit == m_mapMonsterCount.end() )
			{
				return false;
			}

			iCount = mit->second;
			return true;
		}

		int GetTotalMonsterCount() const	{ return m_iTotalMonsterCount; }
	};

	//////////////////////////////////////////////////////////////////////////	
	// 필드 단위 몬스터 카운트 정보
	class KBattleFieldMonsterCountInfo
	{
	private:
		std::map< int, KGroupMonsterCountInfo >	m_mapGroupMonsterCount;		// [key : GroupID, value : GroupInfo] 

	public:
		KBattleFieldMonsterCountInfo()
		{
		}

		void AddGroupMonsterCount( IN const int iGroupID, IN const int iNpcID )
		{
			std::map< int, KGroupMonsterCountInfo >::iterator mitMC;
			mitMC = m_mapGroupMonsterCount.find( iGroupID );
			if( mitMC == m_mapGroupMonsterCount.end() )
			{
				KGroupMonsterCountInfo sCountInfo;
				sCountInfo.AddMonsterCount( iNpcID );
				m_mapGroupMonsterCount.insert( std::make_pair( iGroupID, sCountInfo ) );
			}
			else
			{
				mitMC->second.AddMonsterCount( iNpcID );
			}
		}

		bool GetGroupMonsterCount( IN const int iGroupID, OUT int& iCount ) const
		{
			iCount = 0;

			std::map< int, KGroupMonsterCountInfo >::const_iterator mit;
			mit = m_mapGroupMonsterCount.find( iGroupID );
			if( mit == m_mapGroupMonsterCount.end() )
			{
				return false;
			}

			iCount = mit->second.GetTotalMonsterCount();
			return true;
		}

		bool IsExistGroupID( IN const int iGroupID ) const
		{
			return ( m_mapGroupMonsterCount.find( iGroupID ) != m_mapGroupMonsterCount.end() );
		}

		bool GetMonsterCountByGroupIDAndNpcID( IN const int iGroupID, IN const int iNpcID, OUT int& iNpcCount ) const
		{
			iNpcCount = 0;

			std::map< int, KGroupMonsterCountInfo >::const_iterator mitMC;
			mitMC = m_mapGroupMonsterCount.find( iGroupID );
			if( mitMC == m_mapGroupMonsterCount.end() )
			{
				return false;
			}

			return mitMC->second.GetMonsterCount( iNpcID, iNpcCount );
		}

		int GetTotalMonsterCount() const
		{
			int iTotalCount = 0;

			std::map< int, KGroupMonsterCountInfo >::const_iterator mit;
			for( mit = m_mapGroupMonsterCount.begin(); mit != m_mapGroupMonsterCount.end(); ++mit )
			{
				iTotalCount += mit->second.GetTotalMonsterCount();
			}

			return iTotalCount;
		}
	};

	//////////////////////////////////////////////////////////////////////////	
	// 라인맵 NPC 스타트 포지션 정보
	struct SMonsterStartPos
	{
		VECTOR3			m_vPos;
		bool			m_bRight;

		SMonsterStartPos()
		{
			m_bRight = false;
		}
	};
	

public:
	CXSLBattleField();
	~CXSLBattleField();

	// 초기화
	bool	LoadBattleFieldData( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN OUT KLuaManager& kLuaManager );
	bool	AddSpawnMonsterGroup_LUA();

	bool	OpenScriptFile();
	void	SetLandHeight_LUA( int iLandHeight );	
	void	SetTeamStartPos_LUA();
	bool	LoadLineMapData_LUA( int iStartPos, D3DXVECTOR3 vPos, bool bRight, int iLineMapIndex );
	void	AddLine_LUA();
	void	AddCameraData_LUA();
	void	AddNpcGroup( IN const SSpawnNpcGroup& sSpawnNpcGroup );

	// 몬스터 리스트 초기화
	bool	GetCreateMonsterList( IN const int iPlayerCount,
							   OUT std::vector< KNPCUnitReq >& vecCreateNPCList ) const;

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	bool	GetCreateBossMonsterList( OUT std::vector<KNPCUnitReq>& vecCreateNPCList ) const;
#endif // SERV_BATTLE_FIELD_BOSS

	// 리스폰 정보
	bool	GetRespawnMonsterList( IN const CXSLBattleField::KBattleFieldMonsterCountInfo& kAliveMonsterCountInfo,
								IN const std::map< int, int >& mapRespawnReadyNpcCount,
								OUT std::vector< KNPCUnitReq >& vecCreateNPCList ) const;

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	bool	GetBattieFieldEventBossMonsterInfo( IN const int iDangerousValue, OUT std::vector<KNPCUnitReq>& vecNpcInfo );
	bool	MakeEventBossMonsterInfo( IN const SSpawnNpcInfo& sSpawnNpcInfo, IN const bool bSiegeMode, IN const SRiskInfo& sRiskInfo, OUT KNPCUnitReq& kInfo ) const;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	bool	GetBattieFieldMiddleBossMonsterInfo( IN const int iDangerousValue, OUT std::vector<KNPCUnitReq>& vecNpcInfo );
	bool	MakeMiddleBossMonsterInfo( IN const SSpawnNpcInfo& sSpawnNpcInfo, IN const bool bSiegeMode, IN const SRiskInfo& sRiskInfo, OUT KNPCUnitReq& kInfo ) const;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	bool	GetBattieFieldBossMonsterInfo( IN const int iDangerousValue, OUT KNPCUnitReq& kNpcInfo );

	int		GetMaxMonsterCountInThisBattleField( IN const int iPlayerCount ) const;

	int		GetMaxNumberMonster() const		{ return m_iMaxNumberMonster; }
	int		GetMaxJoinUserCount() const		{ return m_iMaxJoinUserCount; }
	int		GetRequireLevel() const			{ return m_iRequireLevel; }
	int		GetMonsterRespawnFactorByUserCount() const { return m_iMonsterRespawnFactorByUserCount; }
	void	SetMonsterRespawnFactorByUserCount( IN const int MonsterRespawnFactorByUserCount ) { m_iMonsterRespawnFactorByUserCount = MonsterRespawnFactorByUserCount; }
	int		GetMonsterStartPosByRandom( IN const std::vector< int >& vecLineMapIndexList ) const;
	bool	GetMonsterStartPosInfo( IN const int iStartPos, OUT VECTOR3& vPos, OUT bool& bRight ) const;
	const SSpawnNpcGroup* GetSpawnNpcGroup( IN const int iGroupID ) const;

	// 입장 조건 체크
	bool	IsCanEnterBattleField( IN const u_char ucLevel, IN const std::map< int, KDungeonClearInfo >& mapDungeonClear, OUT int& iRequireLevel, OUT int& iRequireDungeonID ) const;
	//{{ 2012. 11. 13	최육사		입장 가능한 마을 검사 및 보정
#ifdef SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	int		GetEnterVillageMapID() const	{ return m_iEnterVillageMapID; }
#endif SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	//}}
	//{{ 2013. 1. 1	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
	bool	GetEventMonsterCreateInfo( OUT KNPCUnitReq& kInfo );
#endif SERV_FIELD_EVENT_MONSTER
	//}}

	int		GetStandardMonsterLevel() const { return m_iStandardMonsterLevel; }

private:
	// 몬스터 생성
	bool	CreateMonsterFromSpawnNpcGroup( IN const SSpawnNpcGroup& sSpawnNpcGroup, IN const CXSLBattleField::KBattleFieldMonsterCountInfo& kMonsterCountInfo, OUT KNPCUnitReq& kInfo ) const;

	// 몬스터 정보 생성
	bool	MakeMonsterInfo( IN const SSpawnNpcGroup& sSpawnNpcGroup, IN const int iNpcID, OUT KNPCUnitReq& kInfo ) const;

private:
	lua_State*							m_luaState;

private:
	SEnum::BATTLE_FIELD_ID				m_eBattleFieldID;					// 배틀필드ID
	short								m_sWorldID;							// 월드정보
	std::wstring						m_wstrLineMapFileName;				// 라인맵 파일 이름
	std::wstring						m_wstrNpcTableFileName;				// NPC출현 설정 파일 이름
	int									m_iStandardMonsterLevel;			// 필드에 나오는 몬스터들의 기준 레벨
	std::wstring						m_wstrBattleFieldName;				// 배틀필드 이름	
	int									m_iMaxNumberMonster;				// 이 필드내에 최대 몬스터 수
	int									m_iMaxJoinUserCount;				// 최대 입장 가능한 유저 수
	int									m_iMonsterRespawnFactorByUserCount; // 배틀필드에 입장한 유저 수에 따른 몬스터 출현 수 계산 상수

	int									m_iRequireLevel;					// 입장 가능 레벨
	std::set< int >						m_setRequireDungeon;				// 입장 조건 던전
	
	std::vector< SPotalMovingInfo > 	m_vecPotalMovingInfo;				// 포탈로 이동하는곳의 정보
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	std::vector< SRiskInfo >			m_vecMiddleBossInfo;				// 중간보스 그룹정보
#else
	std::vector< SRiskInfo >			m_vecRiskInfo;						// 위험도 정보
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	std::vector< SRiskInfo >			m_vecEventBossInfo;				// 이벤트보스 그룹정보
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	std::vector<SSpawnNpcGroup>			m_vecBossNpcGroupList;				// 출현 Boss NPC 그룹
#endif // SERV_BATTLE_FIELD_BOSS
	std::vector< SSpawnNpcGroup >		m_vecRespawnNpcGroupList;			// 출현 NPC 그룹
	std::map< int, SSpawnNpcGroup >		m_mapNpcGroupList;					// 출현 NPC 그룹

	std::map< int, std::vector< int > >	m_mapLineIndexRandomStartPos;		// 라인맵 인덱스에 대한 StartPos그룹
	std::map< int, SMonsterStartPos >	m_mapMonsterStartPos;				// 몬스터 Start Position

	//{{ 2012. 11. 13	최육사		입장 가능한 마을 검사 및 보정
#ifdef SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	int									m_iEnterVillageMapID;				// 해당 배틀필드가 속한 마을 MapID
#endif SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	//}}
};




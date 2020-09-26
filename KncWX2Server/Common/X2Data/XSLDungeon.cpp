#include "XSLDungeon.h"
#include "KAutoPath.h"

#ifdef SERV_DUNGEON_OPTION_IN_LUA
#include "XSLDungeonManager.h"
#endif SERV_DUNGEON_OPTION_IN_LUA

#ifdef _CONVERT_VS_2010
#define ARRAY_SIZE(a)       (sizeof(a)/sizeof((a)[0]))
#endif _CONVERT_VS_2010

CXSLDungeon::CXSLDungeon( const DungeonData& dungeonData )
{
	m_sDungeonData = dungeonData;

	//m_sDungeonData.m_DungeonID			= dungeonData.m_DungeonID;
	//m_sDungeonData.m_DungeonName		= dungeonData.m_DungeonName;
	//m_sDungeonData.m_DataFileName		= dungeonData.m_DataFileName;
	////m_sDungeonData.m_RequireLevel		= dungeonData.m_RequireLevel;
	//m_sDungeonData.m_RequireDungeonID	= dungeonData.m_RequireDungeonID;	
	//m_sDungeonData.m_RequireItemID		= dungeonData.m_RequireItemID;
	//m_sDungeonData.m_RequireItemCount	= dungeonData.m_RequireItemCount;	

	//m_sDungeonData.m_RequireItemID2		= dungeonData.m_RequireItemID2;
	//m_sDungeonData.m_RequireItemCount2	= dungeonData.m_RequireItemCount2;	

	//m_sDungeonData.m_RequireSpirit		= dungeonData.m_RequireSpirit;
	//m_sDungeonData.m_MinLevel			= dungeonData.m_MinLevel;
	//m_sDungeonData.m_MaxLevel			= dungeonData.m_MaxLevel;
	//m_sDungeonData.m_eDifficulty		= dungeonData.m_eDifficulty;

	//m_sDungeonData.m_bNormalOnly		= dungeonData.m_bNormalOnly;
	//m_sDungeonData.m_fTimeLimit			= dungeonData.m_fTimeLimit;

	////{{ 2009. 7. 21  최육사	헤니르 시공
	//m_sDungeonData.m_eDungeonType		= dungeonData.m_eDungeonType;
	//m_sDungeonData.m_bRelativeMonsterLevel = dungeonData.m_bRelativeMonsterLevel;
	//}}
}

CXSLDungeon::~CXSLDungeon()
{
	Clear();
}

void CXSLDungeon::Clear()
{
	for( int i = 0; i < (int)m_vecStageData.size(); ++i )
	{
		CXSLDungeonStage::StageData* pStageData = m_vecStageData[i];
		SAFE_DELETE( pStageData );
	}
	m_vecStageData.clear();
}

bool CXSLDungeon::OpenScriptFile()
{
	// Enum.lua 파일은 파싱되어 있어야 한다.

	char szFile[MAX_PATH]	= "";
	char strTable[MAX_PATH]	= "";
	WideCharToMultiByte( CP_ACP, 0, m_sDungeonData.m_DataFileName.c_str(), -1, szFile, MAX_PATH, NULL, NULL );

	KLuaManager luaManager;

	KAutoPath		kautoPaht;
	std::string		strFile;

	strFile = "Enum.lua";
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< BUILD_LOG( szFile )
			<< END_LOG;

		return false;
	}

	strFile = "DungeonEnum.lua";
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< BUILD_LOG( szFile )
			<< END_LOG;

		return false;
	}

	strFile = "DLG_Map_Enum.lua";
	kautoPaht.GetPullPath( strFile );

	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< BUILD_LOG( szFile )
			<< END_LOG;

		return false;
	}

	strFile = szFile;
	kautoPaht.GetPullPath( strFile );

	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, szFile )
			<< END_LOG;

		return false;
	}

	//{{ 2009. 12. 18  최육사	던전경험치개편
	const bool bScriptCheck = CXSLDungeon::IsScriptCheckDungeon( (int)m_sDungeonData.m_DungeonID );
	//}}

	if( luaManager.BeginTable( "STAGE_LIST" ) != E_FAIL )
	{
		int stageIndex = 0;
		char strTable[MAX_PATH] = "";
		while( true )
		{
#ifdef _CONVERT_VS_2010
			sprintf_s( strTable, ARRAY_SIZE(strTable), "STAGE%d", stageIndex );
#else
			sprintf( strTable, "STAGE%d", stageIndex );
#endif _CONVERT_VS_2010
			if( luaManager.BeginTable( strTable ) == E_FAIL )
				break;

			CXSLDungeonStage::StageData* pStageData = new CXSLDungeonStage::StageData();
			if( pStageData->LoadData( bScriptCheck, luaManager ) == false )
			{
				START_LOG( cerr, L"던전 스크립트 파싱 실패!" )
					<< BUILD_LOG( m_sDungeonData.m_DataFileName )
					<< END_LOG;
			}

			++stageIndex;
			m_vecStageData.push_back( pStageData );
			luaManager.EndTable(); // Stage(i)	
		}
		luaManager.EndTable(); // StageList
	}

	if( DT_NORMAL == m_sDungeonData.m_eDungeonType )
	{
		// 스크립트에서 레벨 정보가 없는 npc들은 던전 npc레벨에 맞춘다
		for( UINT i=0; i<m_vecStageData.size(); i++ )
		{
			CXSLDungeonStage::StageData* pStageData = m_vecStageData[i];
			for( UINT j=0; j<pStageData->m_SubStageDataList.size(); j++ )
			{
				CXSLDungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[j];
				//{{ 2012. 12. 21  던전 몬스터 그룹 랜던 배치 - 김민성
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
				std::map< int, std::vector<CXSLDungeonSubStage::NPCData*> >::iterator mit = pSubStageData->m_mapNPCDataList.begin();
				for( ; mit != pSubStageData->m_mapNPCDataList.end() ; ++mit )
				{
					for( UINT k=0; k<mit->second.size(); k++ )
					{
						CXSLDungeonSubStage::NPCData* pNPCData = mit->second[k];
						if( pNPCData->m_Level <= 0 )
						{
							pNPCData->m_Level = m_sDungeonData.m_NpcLevel;
						}
					}
				}
#else
				for( UINT k=0; k<pSubStageData->m_NPCDataList.size(); k++ )
				{
					CXSLDungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[k];
					if( pNPCData->m_Level <= 0 )
					{
						//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_DUNGEON_NPC_LEVEL
						pNPCData->m_Level = m_sDungeonData.m_NpcLevel;
#else
						pNPCData->m_Level = m_sDungeonData.m_MinLevel;
#endif SERV_DUNGEON_NPC_LEVEL
						//}}
					}
				}
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
				//}}
			}
		}
	}


	return true;
}

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
int CXSLDungeon::GetNPCDropTime( IN const int iStageIndex, IN const int iSubStageIndex, IN const int iNPCID ) const
{
	//{{ 2009. 7. 15  최육사	index 범위 체크
	if( iStageIndex < 0 || iStageIndex >= (int)m_vecStageData.size() )
	{
		START_LOG( cerr, L"stage index값이 이상합니다." )
			<< BUILD_LOG( iStageIndex )
			<< BUILD_LOG( m_vecStageData.size() )
			<< END_LOG;
		return 0;
	}
	//}}
	CXSLDungeonStage::StageData* pStageData = m_vecStageData[iStageIndex];

	if( iSubStageIndex < 0 || iSubStageIndex >= (int)pStageData->m_SubStageDataList.size() )
	{
		START_LOG( cerr, L"substage index값이 이상합니다." )
			<< BUILD_LOG( iSubStageIndex )
			<< BUILD_LOG( pStageData->m_SubStageDataList.size() )
			<< END_LOG;
		return 0;
	}
	CXSLDungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[iSubStageIndex];

	return pSubStageData->GetNpcDropTimes( iNPCID );
}
#endif SERV_CREATED_NPC_LIMITED_DROPS

#ifdef SERV_DUNGEON_NPC_DATA_EXP_RATE		// 적용날짜: 2013-08-13
float CXSLDungeon::GetNPCExpRate( IN const int iStageIndex, IN const int iSubStageIndex, IN const int iNPCID ) const
{
	//{{ 2009. 7. 15  최육사	index 범위 체크
	if( iStageIndex < 0 || iStageIndex >= (int)m_vecStageData.size() )
	{
		START_LOG( cerr, L"stage index값이 이상합니다." )
			<< BUILD_LOG( m_sDungeonData.m_DungeonID )
			<< BUILD_LOG( iStageIndex )
			<< BUILD_LOG( m_vecStageData.size() )
			<< END_LOG;
		return 1.f;
	}
	//}}
	CXSLDungeonStage::StageData* pStageData = m_vecStageData[iStageIndex];

	if( iSubStageIndex < 0 || iSubStageIndex >= (int)pStageData->m_SubStageDataList.size() )
	{
		START_LOG( cerr, L"substage index값이 이상합니다." )
			<< BUILD_LOG( m_sDungeonData.m_DungeonID )
			<< BUILD_LOG( iStageIndex )
			<< BUILD_LOG( iSubStageIndex )
			<< BUILD_LOG( pStageData->m_SubStageDataList.size() )
			<< END_LOG;
		return 1.f;;
	}
	CXSLDungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[iSubStageIndex];

	return pSubStageData->GetNpcExpRate( iNPCID );
}
#endif // SERV_DUNGEON_NPC_DATA_EXP_RATE

//{{ 2012. 12. 21  던전 몬스터 그룹 랜던 배치 - 김민성
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
bool CXSLDungeon::GetNPCData( IN const int iStageIndex, IN const int iRelativeMonsterLevel, OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot, OUT bool& bIsBossStage ) const
{
	bIsBossStage = false;

	//{{ 2009. 7. 15  최육사	index 범위 체크
	if( iStageIndex < 0  ||  iStageIndex >= (int)m_vecStageData.size() )
	{
		START_LOG( cerr, L"stage index값이 이상합니다." )
			<< BUILD_LOG( iStageIndex )
			<< BUILD_LOG( m_vecStageData.size() )
			<< END_LOG;
		return false;
	}
	//}}

	kNot.m_iStageID	= iStageIndex;
	CXSLDungeonStage::StageData* pStageData = m_vecStageData[iStageIndex];

	for( int i = 0; i < (int)pStageData->m_SubStageDataList.size(); i++ )
	{
		CXSLDungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[i];

		if( pSubStageData == NULL )
			return false;

		KNPCList	kNPCList;
		int			nSubNPCIndex = -1;
		int			nSubNPCRand	 = 0;
		bool		nSubNPCCheck = false;

		int iNpcGroupID = pSubStageData->GetRandomNpcGruopID();

#ifdef SERV_TEST_LOG_FOR_SUB_STAGE_NPC_GROUP_ID// 작업날짜: 2013-09-03	// 박세훈
		kNot.m_mapSubStageNpcGroupID.insert( std::map< int, int >::value_type( i, iNpcGroupID ) );
#endif // SERV_TEST_LOG_FOR_SUB_STAGE_NPC_GROUP_ID

		std::map< int, std::vector<CXSLDungeonSubStage::NPCData*> >::iterator mit = pSubStageData->m_mapNPCDataList.find( iNpcGroupID );
		if( mit == pSubStageData->m_mapNPCDataList.end() )
		{
			START_LOG( cerr, L"Random NPC Group ID 값으로 찾은 데이터가 없습니다." )
				<< BUILD_LOG( iStageIndex )
				<< BUILD_LOG( iRelativeMonsterLevel )
				<< BUILD_LOG( iNpcGroupID )
				<< END_LOG;

			return false;
		}

		for( int j = 0; j < (int)mit->second.size(); j++ )
		{
			CXSLDungeonSubStage::NPCData* pNPCData = mit->second[j];

			if( pNPCData == NULL )
				return false;

			if( pNPCData->m_bSubNPC == false )
			{
				//확률을 돌려 생성될 유닛을 골라낸다.
				if( pNPCData->m_Rate < (rand() % 100) )
					continue;
			}
			else
			{
				//Sub NPC 중에서 골라내기
				if( nSubNPCIndex != pNPCData->m_iSubNPCIndex )
				{
					nSubNPCCheck	= false;
					nSubNPCRand		= 0;
					nSubNPCIndex	= pNPCData->m_iSubNPCIndex;
				}

				if( nSubNPCCheck == false )
				{
					int nRand = (int)(rand() % 100);
					nSubNPCRand += pNPCData->m_iSubNPCRate;

					if( nRand >= nSubNPCRand || nRand >= pNPCData->m_Rate )
						continue;

					nSubNPCCheck = true;
				}
				else
					continue;
			}

			KNPCUnitReq kNPCUnitReq;
			kNPCUnitReq.Init();

			kNPCUnitReq.m_NPCID				= pNPCData->m_UnitID;
			kNPCUnitReq.m_KeyCode			= (char)pNPCData->m_KeyCode;
			kNPCUnitReq.m_bFocusCamera		= pNPCData->m_bFocusCamera;

			//몬스터를 vector에 들어있는 시작지점중 하나를 랜덤으로 골라 넣어준다.
			//kNPCUnitReq.m_nStartPos			= pNPCData->m_StartPos;
			if( pNPCData->m_vecStartPos.empty()  == true )
			{
				kNPCUnitReq.m_nStartPos = 0;

				START_LOG( cerr, L"생성될 몬스터 시작위치가 없네 누구냐 넌..?" )
					<< BUILD_LOG( pNPCData->m_UnitID )
					<< BUILD_LOG( m_sDungeonData.m_DungeonID )
					<< BUILD_LOG( m_sDungeonData.m_DungeonName )
					<< END_LOG;
			}
			else
			{
				int nSIndex = (int)(rand() % (int)pNPCData->m_vecStartPos.size());
				kNPCUnitReq.m_nStartPos			= pNPCData->m_vecStartPos[nSIndex];
			}

			kNPCUnitReq.m_bActive			= pNPCData->m_bActive;
			kNPCUnitReq.m_bShowGage			= pNPCData->m_bShowGage;
			kNPCUnitReq.m_AddPos.x			= pNPCData->m_AddPos.x;
			kNPCUnitReq.m_AddPos.y			= pNPCData->m_AddPos.y;
			kNPCUnitReq.m_AddPos.z			= pNPCData->m_AddPos.z;

			if( true == m_sDungeonData.m_bRelativeMonsterLevel )
			{
				kNPCUnitReq.m_Level				= (char) ( iRelativeMonsterLevel + pNPCData->m_Level );
			}
			else
			{
				kNPCUnitReq.m_Level				= (char)pNPCData->m_Level;
			}

			kNPCUnitReq.m_bHasBossGage		= pNPCData->m_bHasBossGage;
			kNPCUnitReq.m_bShowBossName		= pNPCData->m_bShowBossName;
			kNPCUnitReq.m_bShowSubBossName	= pNPCData->m_bShowSubBossName;
			kNPCUnitReq.m_bSiegeMode		= pNPCData->m_bSiegeMode;			
			kNPCUnitReq.m_bIsRight			= pNPCData->m_bIsRight;
			kNPCUnitReq.m_bNoDrop			= pNPCData->m_bNoDrop;
			kNPCUnitReq.m_fUnitScale		= pNPCData->m_fUnitScale;
			//{{ 2009. 12. 18  최육사	던전경험치개편
			kNPCUnitReq.m_cMonsterGrade		= (char)pNPCData->m_MonsterGrade;
			//}}
			//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
			kNPCUnitReq.m_usBossGaugeHPLines = pNPCData->m_BossGaugeHPLines;

			kNPCUnitReq.m_bAllyNpc = pNPCData->m_bAllyNpc;

			kNPCUnitReq.m_fShowBossNameDelayTime	= pNPCData->m_fShowBossNameDelayTime;
			kNPCUnitReq.m_bStopAtStartState			= pNPCData->m_bStopAtStartState;

			// 보스 몬스터가 있다면?
			if( CXSLUnitManager::MG_BOSS_NPC == kNPCUnitReq.m_cMonsterGrade )
			{
				bIsBossStage = true; // 보스 스테이지 체크!
			}

			kNPCList.m_NPCList.push_back( kNPCUnitReq );
		}

		kNot.m_mapNPCData.insert( std::make_pair( i, kNPCList ) );
	}

	return true;
}

#else	// SERV_DUNGEON_RANDOM_NPC_GROUP

//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
bool CXSLDungeon::GetNPCData( IN const int iStageIndex, IN const int iRelativeMonsterLevel, OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot, OUT bool& bIsBossStage ) const
#else
bool CXSLDungeon::GetNPCData( IN const int iStageIndex, IN const int iHighestUserLevel, OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot, OUT bool& bIsBossStage ) const
#endif SERV_DUNGEON_NPC_LEVEL
//}}
{
	bIsBossStage = false;

	//{{ 2009. 7. 15  최육사	index 범위 체크
	if( iStageIndex < 0  ||  iStageIndex >= (int)m_vecStageData.size() )
	{
		START_LOG( cerr, L"stage index값이 이상합니다." )
			<< BUILD_LOG( iStageIndex )
			<< BUILD_LOG( m_vecStageData.size() )
			<< END_LOG;
		return false;
	}
	//}}

	kNot.m_iStageID	= iStageIndex;
	CXSLDungeonStage::StageData* pStageData = m_vecStageData[iStageIndex];

	for( int i = 0; i < (int)pStageData->m_SubStageDataList.size(); i++ )
	{
		CXSLDungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[i];

		if( pSubStageData == NULL )
			return false;

		KNPCList	kNPCList;
		int			nSubNPCIndex = -1;
		int			nSubNPCRand	 = 0;
		bool		nSubNPCCheck = false;

		for( int j = 0; j < (int)pSubStageData->m_NPCDataList.size(); j++ )
		{
			CXSLDungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[j];

			if( pNPCData == NULL )
				return false;

			if( pNPCData->m_bSubNPC == false )
			{
				//확률을 돌려 생성될 유닛을 골라낸다.
				if( pNPCData->m_Rate < (rand() % 100) )
					continue;
			}
			else
			{
				//Sub NPC 중에서 골라내기
				if( nSubNPCIndex != pNPCData->m_iSubNPCIndex )
				{
					nSubNPCCheck	= false;
					nSubNPCRand		= 0;
					nSubNPCIndex	= pNPCData->m_iSubNPCIndex;
				}

				if( nSubNPCCheck == false )
				{
					int nRand = (int)(rand() % 100);
					nSubNPCRand += pNPCData->m_iSubNPCRate;

					if( nRand >= nSubNPCRand || nRand >= pNPCData->m_Rate )
						continue;

					nSubNPCCheck = true;
				}
				else
					continue;
			}

			KNPCUnitReq kNPCUnitReq;
			kNPCUnitReq.Init();

			kNPCUnitReq.m_NPCID				= pNPCData->m_UnitID;
			kNPCUnitReq.m_KeyCode			= (char)pNPCData->m_KeyCode;
			kNPCUnitReq.m_bFocusCamera		= pNPCData->m_bFocusCamera;

			//몬스터를 vector에 들어있는 시작지점중 하나를 랜덤으로 골라 넣어준다.
			//kNPCUnitReq.m_nStartPos			= pNPCData->m_StartPos;
			if( pNPCData->m_vecStartPos.empty()  == true )
			{
				kNPCUnitReq.m_nStartPos = 0;

				START_LOG( cerr, L"생성될 몬스터 시작위치가 없네 누구냐 넌..?" )
					<< BUILD_LOG( pNPCData->m_UnitID )
					<< BUILD_LOG( m_sDungeonData.m_DungeonID )
					<< BUILD_LOG( m_sDungeonData.m_DungeonName )
					<< END_LOG;
			}
			else
			{
				int nSIndex = (int)(rand() % (int)pNPCData->m_vecStartPos.size());
				kNPCUnitReq.m_nStartPos			= pNPCData->m_vecStartPos[nSIndex];
			}

			kNPCUnitReq.m_bActive			= pNPCData->m_bActive;
			kNPCUnitReq.m_bShowGage			= pNPCData->m_bShowGage;
			kNPCUnitReq.m_AddPos.x			= pNPCData->m_AddPos.x;
			kNPCUnitReq.m_AddPos.y			= pNPCData->m_AddPos.y;
			kNPCUnitReq.m_AddPos.z			= pNPCData->m_AddPos.z;

			if( true == m_sDungeonData.m_bRelativeMonsterLevel )
			{
				//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
				kNPCUnitReq.m_Level				= (char) ( iRelativeMonsterLevel + pNPCData->m_Level );
#else
				kNPCUnitReq.m_Level				= (char) ( iHighestUserLevel + pNPCData->m_Level );
#endif SERV_DUNGEON_NPC_LEVEL
				//}}				
			}
			else
			{
				kNPCUnitReq.m_Level				= (char)pNPCData->m_Level;
			}

			kNPCUnitReq.m_bHasBossGage		= pNPCData->m_bHasBossGage;
			kNPCUnitReq.m_bShowBossName		= pNPCData->m_bShowBossName;
			kNPCUnitReq.m_bShowSubBossName	= pNPCData->m_bShowSubBossName;
			kNPCUnitReq.m_bSiegeMode		= pNPCData->m_bSiegeMode;			
			kNPCUnitReq.m_bIsRight			= pNPCData->m_bIsRight;
			kNPCUnitReq.m_bNoDrop			= pNPCData->m_bNoDrop;
			kNPCUnitReq.m_fUnitScale		= pNPCData->m_fUnitScale;
			//{{ 2009. 12. 18  최육사	던전경험치개편
			kNPCUnitReq.m_cMonsterGrade		= (char)pNPCData->m_MonsterGrade;
			//}}
			//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_BOSS_GAUGE_HP_LINES
			kNPCUnitReq.m_usBossGaugeHPLines = pNPCData->m_BossGaugeHPLines;
#endif SERV_BOSS_GAUGE_HP_LINES
			//}}

#ifdef SERV_ALLY_NPC
			kNPCUnitReq.m_bAllyNpc = pNPCData->m_bAllyNpc;
#endif SERV_ALLY_NPC

#ifdef SERV_MODIFY_DUNGEON_STAGING
			kNPCUnitReq.m_fShowBossNameDelayTime	= pNPCData->m_fShowBossNameDelayTime;
			kNPCUnitReq.m_bStopAtStartState			= pNPCData->m_bStopAtStartState;
#endif //SERV_MODIFY_DUNGEON_STAGING
			// 보스 몬스터가 있다면?
			if( CXSLUnitManager::MG_BOSS_NPC == kNPCUnitReq.m_cMonsterGrade )
			{
				bIsBossStage = true; // 보스 스테이지 체크!
			}

			kNPCList.m_NPCList.push_back( kNPCUnitReq );
		}

		kNot.m_mapNPCData.insert( std::make_pair( i, kNPCList ) );
	}

	return true;
}
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
//}}

const CXSLDungeon::DungeonData& CXSLDungeon::GetDungeonData() const
{
	return m_sDungeonData;
}

int CXSLDungeon::GetStartPosByRandom( IN int iStageIndex, IN int iSubStageIndex ) const
{
	if( (int)m_vecStageData.size() <= iStageIndex || 0 > iStageIndex )
	{
		START_LOG( cerr, L"stage 인덱스 오류.!" )
			<< BUILD_LOG( (int)m_vecStageData.size() )
			<< BUILD_LOG( iStageIndex )
			<< END_LOG;

		return -1;
	}

	CXSLDungeonStage::StageData* pStageData = m_vecStageData[iStageIndex];
	if( pStageData == NULL )
	{
		START_LOG( cerr, L"stage 포인터 오류.!" )
			<< BUILD_LOG( iStageIndex )
			<< END_LOG;

		return -1;
	}

	if( (int)pStageData->m_SubStageDataList.size() <= iSubStageIndex || 0 > iSubStageIndex )
	{
		START_LOG( cerr, L"Sub stage 인덱스 오류.!" )
			<< BUILD_LOG( (int)pStageData->m_SubStageDataList.size() )
			<< BUILD_LOG( (int)m_sDungeonData.m_DungeonID )
			<< BUILD_LOG( (int)m_sDungeonData.m_eDifficulty )
			<< BUILD_LOG( iStageIndex )
			<< BUILD_LOG( iSubStageIndex )
			<< END_LOG;

		return -1;
	}

	CXSLDungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[iSubStageIndex];
	if( pSubStageData == NULL )
	{
		START_LOG( cerr, L"Sub stage 포인터 오류.!" )
			<< BUILD_LOG( iSubStageIndex )
			<< END_LOG;

		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	//des : 랜덤 루틴.

	int startPos = -1;

	//{{ 2012. 12. 21  던전 몬스터 그룹 랜던 배치 - 김민성
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
	int iNpcGroupID = pSubStageData->GetRandomNpcGruopID();
	std::map<int,std::vector<CXSLDungeonSubStage::NPCData*> >::iterator mit = pSubStageData->m_mapNPCDataList.find( iNpcGroupID );
	if( mit == pSubStageData->m_mapNPCDataList.end() )
	{
		START_LOG( cerr, L"Random Npc Group ID 값으로 찾은 데이터가 없다." )
			<< BUILD_LOG( iNpcGroupID )
			<< END_LOG;
		return -1;
	}

	for( u_int ui = 0; ui < mit->second.size(); ++ui )
	{
		//안전하게 인덱스를 뽑아내기 위해 조건에 맞는 몬스터를 체크함.
		if( mit->second[ui]->m_bActive == true &&
			mit->second[ui]->m_bNoDrop == false/* &&
			mit->second[ui]->m_bHasBossGage == false*/
			)
		{
			CXSLDungeonSubStage::NPCData* pNPCData = mit->second[ui];
			if( pNPCData == NULL )
				continue;

            if( pNPCData->m_vecStartPos.size() > 0 )
			{
				startPos = pNPCData->m_vecStartPos[0];

				if( rand() % 2 )
					break;
			}
		}
	}
#else
	for( u_int ui = 0; ui < pSubStageData->m_NPCDataList.size(); ++ui )
	{
		//안전하게 인덱스를 뽑아내기 위해 조건에 맞는 몬스터를 체크함.
		if( pSubStageData->m_NPCDataList[ui]->m_bActive == true &&
			pSubStageData->m_NPCDataList[ui]->m_bNoDrop == false/* &&
			pSubStageData->m_NPCDataList[ui]->m_bHasBossGage == false*/
			)
		{
			CXSLDungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[ui];
			if( pNPCData == NULL )
				continue;

            if( pNPCData->m_vecStartPos.size() > 0 )
			{
				startPos = pNPCData->m_vecStartPos[0];

				if( rand() % 2 )
					break;
			}
		}
	}
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
	//}}

	return startPos;

	//////////////////////////////////////////////////////////////////////////
}

//{{ 2010. 04. 26  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
void CXSLDungeon::GetExtraNpcInfo( IN int iStageIndex, IN int iSubStageIndex, OUT int& iStartPos, OUT bool& bIsRight ) const
{
	// 초기화
	iStartPos = -1;
	bIsRight = false;

	if( (int)m_vecStageData.size() <= iStageIndex || 0 > iStageIndex )
	{
		START_LOG( cerr, L"stage 인덱스 오류.!" )
			<< BUILD_LOG( (int)m_vecStageData.size() )
			<< BUILD_LOG( iStageIndex )
			<< END_LOG;

		return;
	}

	CXSLDungeonStage::StageData* pStageData = m_vecStageData[iStageIndex];
	if( pStageData == NULL )
	{
		START_LOG( cerr, L"stage 포인터 오류.!" )
			<< BUILD_LOG( iStageIndex )
			<< END_LOG;

		return;
	}

	if( (int)pStageData->m_SubStageDataList.size() <= iSubStageIndex || 0 > iSubStageIndex )
	{
		START_LOG( cerr, L"Sub stage 인덱스 오류.!" )
			<< BUILD_LOG( (int)pStageData->m_SubStageDataList.size() )
			<< BUILD_LOG( (int)m_sDungeonData.m_DungeonID )
			<< BUILD_LOG( (int)m_sDungeonData.m_eDifficulty )
			<< BUILD_LOG( iStageIndex )
			<< BUILD_LOG( iSubStageIndex )
			<< END_LOG;

		return;
	}

	CXSLDungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[iSubStageIndex];
	if( pSubStageData == NULL )
	{
		START_LOG( cerr, L"Sub stage 포인터 오류.!" )
			<< BUILD_LOG( iSubStageIndex )
			<< END_LOG;

		return;
	}

	// Extra Npc Position 획득!
	iStartPos = pSubStageData->m_ExtraNpcStartPos;
	bIsRight = !(pSubStageData->m_bExtraNpcIsLeft); // 반대로 넣어준다
}
#endif SERV_SECRET_HELL
//}}

int CXSLDungeon::GetSubStageIndexByRandom( IN int iStageIndex )
{
	if( (int)m_vecStageData.size() <= iStageIndex  ||  0 > iStageIndex )
		return 0;

	CXSLDungeonStage::StageData* pStageData = m_vecStageData[iStageIndex];

	if( pStageData == NULL )
		return 0;

	return static_cast<int>(rand() % pStageData->m_SubStageDataList.size());
}

//{{ 2010. 03. 24  최육사	비밀던전 헬모드
CXSLDungeonStage::STAGE_TYPE CXSLDungeon::GetStageType( IN u_int uiStageIndex ) const
{
	if( m_vecStageData.size() <= uiStageIndex )
		return CXSLDungeonStage::ST_NONE;

	CXSLDungeonStage::StageData* pStageData = m_vecStageData[uiStageIndex];
	if( pStageData == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 스테이지입니다." )
			<< BUILD_LOG( uiStageIndex )
			<< END_LOG;

		return CXSLDungeonStage::ST_NONE;
	}

	return pStageData->m_eStageType;
}
//}}

//{{ 2010. 11. 8	최육사	코드 정리	
const wchar_t* CXSLDungeon::GetRankString( IN RANK_TYPE eRankType )
{
	switch ( eRankType )
	{
	case RT_NONE:
		return L"None";
	case RT_F:
		return L"F";
	case RT_E:
		return L"E";
	case RT_D:
		return L"D";
	case RT_C:
		return L"C";
	case RT_B:
		return L"B";
	case RT_A:
		return L"A";
	case RT_S:
		return L"S";
		//{{ 2011. 02. 22	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW
	case RT_SS:
		return L"SS";
#endif SERV_DUNGEON_RANK_NEW
		//}}
	}

	return L"Unknown";
}
//}}

bool CXSLDungeon::IsTutorialDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_TUTORIAL_ELSWORD:
	case SEnum::DI_TUTORIAL_ARME:
	case SEnum::DI_TUTORIAL_LIRE:
	case SEnum::DI_TUTORIAL_RAVEN:
	case SEnum::DI_TUTORIAL_EVE:
	case SEnum::DI_TUTORIAL_CHUNG:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_ELSWORD:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_AISHA:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_LENA:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_RAVEN:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_EVE:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_CHUNG:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_ARA:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_EL:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_ADD:
		return true;
	}

	return false;
}

bool CXSLDungeon::IsTCDungeon( IN int iDungeonID )
{
	if( iDungeonID >= SEnum::DI_TRAINING_FREE &&
		iDungeonID <  SEnum::DI_END )
	{
		return true;
	}

	return false;
}

bool CXSLDungeon::IsRubenDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	//case DI_EL_FOREST_MAIN_NORMAL:
	case SEnum::DI_EL_FOREST_WEST_NORMAL:
	case SEnum::DI_EL_FOREST_NORTH_NORMAL:
	case SEnum::DI_EL_FOREST_GATE_NORMAL:
	case SEnum::DI_EL_FOREST_HELL_NORMAL:
		//{{ 2009. 4. 24  최육사	이벤트던전
	case SEnum::DI_EVENT_KIDDAY_RUBEN:
		//}}
	case SEnum::DI_MONSTER_TEST_NORMAL:		// dmlee 2009/11/17 몬스터 테스트 던전 사내에서 입장가능하도록
		//{{ 2010. 04. 05  최육사	비밀던전 헬모드
	case SEnum::DI_RUBEN_SECRET_COMMON:
	case SEnum::DI_RUBEN_SECRET_HELL:
		//}}

	case SEnum::DI_RUBEN_EL_TREE_NORMAL:
	case SEnum::DI_RUBEN_RUIN_OF_ELF_NORMAL:
	case SEnum::DI_RUBEN_SWAMP_NORMAL:
		return true;
	}

	return false;
}

bool CXSLDungeon::IsElderDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_ELDER_BENDERS_CAVE_NORMAL:
	case SEnum::DI_ELDER_NATURE_CAVE_NORMAL:
	case SEnum::DI_ELDER_DRUID_FOREST_NORMAL:
	case SEnum::DI_ELDER_WALLY_CASTLE_CENTER_NORMAL:
	case SEnum::DI_ELDER_WALLY_CASTLE_ROOF_NORMAL:
	case SEnum::DI_ELDER_BELOW_PASSAGE_NORMAL:
	case SEnum::DI_ELDER_WALLY_CASTLE_LAB_NORMAL:
		//{{ 2009. 4. 24  최육사	이벤트던전
	case SEnum::DI_EVENT_KIDDAY_ELDER:
		//}}
		//{{ 2009. 7. 26  최육사	헤니르 시공
	case SEnum::DI_ELDER_HENIR_SPACE:
		//}}
	case SEnum::DI_EVENT_TREE_DAY_ELDER:		// 식목일 이벤트 던전
		//{{ 2010. 04. 05  최육사	비밀던전 헬모드
	case SEnum::DI_ELDER_SECRET_COMMON:
	case SEnum::DI_ELDER_SECRET_HELL:
		//}}
	case SEnum::DI_ELDER_HALLOWEEN_NORMAL: // 할로윈 이벤트 던전
		//{{ 허상형 : [2011/2/20/] //	월드 미션
	case SEnum::DI_DEFENCE_DUNGEON_ELDER_NORMAL:
		//}} 허상형 : [2011/2/20/] //	월드 미션
		return true;
	}

	return false;
}

bool CXSLDungeon::IsBesmaDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_BESMA_DRAGON_ROAD_NORMAL:
	case SEnum::DI_BESMA_MINE_NORMAL:
	case SEnum::DI_BESMA_LAKE_NORMAL:
	case SEnum::DI_BESMA_DRAGON_NEST_NORMAL:
	case SEnum::DI_BESMA_AIRSHIP_NORMAL:
	case SEnum::DI_BESMA_LAKE_NIGHT_NORMAL:
	case SEnum::DI_BESMA_MINE2_NORMAL:
	case SEnum::DI_BESMA_CANYON_NORMAL:
	case SEnum::DI_BESMA_SECRET_NORMAL:
		//{{ 2009. 4. 24  최육사	이벤트던전
	case SEnum::DI_EVENT_KIDDAY_BESMA:
		//}}
		
	case SEnum::DI_EVENT_TREE_DAY_BESMA:		// 식목일 이벤트 던전
		//{{ 2010. 04. 05  최육사	비밀던전 헬모드
	case SEnum::DI_BESMA_SECRET_COMMON:
	case SEnum::DI_BESMA_SECRET_HELL:
		//}}
		
		//{{ 2011. 10.10	할로윈 던전 하드코딩
#ifdef SERV_HALLOWEEN_DUNGEON
	case SEnum::DI_ELDER_HALLOWEEN_NORMAL: // 할로윈 이벤트 던전
#endif SERV_HALLOWEEN_DUNGEON
			//}}
		return true;
	}

	return false;
}

bool CXSLDungeon::IsAlteraDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_ALTERA_BATTLE_AIR_SHIP_NORMAL:
	case SEnum::DI_ALTERA_PLAIN_RECYCLE_NORMAL:
	case SEnum::DI_ALTERA_BELOW_TUNNEL_NORMAL:
	case SEnum::DI_ALTERA_PLAIN_NORMAL:
	case SEnum::DI_ALTERA_NASOD_FACTORY_NORMAL:
	case SEnum::DI_ALTERA_CORE_NORMAL:
	case SEnum::DI_ALTERA_SECRET_NORMAL:
		//{{ 2009. 4. 24  최육사	이벤트던전
	case SEnum::DI_EVENT_KIDDAY_ALTERA:
		//}}
		
	case SEnum::DI_EVENT_TREE_DAY_ALTERA:		// 식목일 이벤트 던전
		//{{ 2010. 04. 05  최육사	비밀던전 헬모드
	case SEnum::DI_ALTERA_SECRET_COMMON:
	case SEnum::DI_ALTERA_SECRET_HELL:
		//}}
		
		//{{ 2011. 10.10	할로윈 던전 하드코딩
#ifdef SERV_HALLOWEEN_DUNGEON
	case SEnum::DI_ELDER_HALLOWEEN_NORMAL: // 할로윈 이벤트 던전
#endif SERV_HALLOWEEN_DUNGEON
		//}}
		return true;		
	}
	
	return false;
}

bool CXSLDungeon::IsPeitaDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_PEITA_OFFERINGS_NORMAL:
	case SEnum::DI_PEITA_SPIRAL_CORRIDOR_NORMAL:
	case SEnum::DI_PEITA_CHAPEL_NORMAL:
	case SEnum::DI_PEITA_UNDER_GARDEN_NORMAL:
	case SEnum::DI_PEITA_TOWER_HEART_NORMAL:
	case SEnum::DI_PEITA_OFFERINGS_ALTER_NORMAL:
	case SEnum::DI_EVENT_TREE_DAY_PEITA:		// 식목일 이벤트 던전		
		
		//{{ 2011. 10.10	할로윈 던전 하드코딩
#ifdef SERV_HALLOWEEN_DUNGEON
	case SEnum::DI_ELDER_HALLOWEEN_NORMAL: // 할로윈 이벤트 던전
#endif SERV_HALLOWEEN_DUNGEON
		//}}
		return true;
	}

	return false;
}

//{{ 2009. 10. 16  최육사	벨더 던전 (제 3거주지구, 희망의 다리)
bool CXSLDungeon::IsVelderDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_VELDER_THIRD_DWELLING_NORMAL:
	case SEnum::DI_VELDER_BRIDGE_HOPE_NORMAL:
	case SEnum::DI_EVENT_TREE_DAY_VELDER:		// 식목일 이벤트 던전	

	case SEnum::DI_VELDER_PALACE_ENTRANCE_NORMAL:
	case SEnum::DI_VELDER_BRIDGE_BUNNING_NORMAL:
	case SEnum::DI_VELDER_MARKET_NORMAL:
	case SEnum::DI_VELDER_GATE_NORMAL:
	case SEnum::DI_BATTLE_SHIP_VELDER:	// 벨더 배던전
		//{{ JHKang / 강정훈 / 2011.8.24
	case SEnum::DI_VELDER_SECRET_COMMON:
	case SEnum::DI_VELDER_SECRET_HELL:
		//}}
		//{{ 2011. 10.10	할로윈 던전 하드코딩
#ifdef SERV_HALLOWEEN_DUNGEON
	case SEnum::DI_ELDER_HALLOWEEN_NORMAL: // 할로윈 이벤트 던전
#endif SERV_HALLOWEEN_DUNGEON
		//}}	
		return true;
	}

	return false;
}
//}}

bool CXSLDungeon::IsHamelDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_BATTLE_SHIP_HAMEL:
	case SEnum::DI_HAMEL_OUTSKIRTS_RESIAM_NORMAL:
	case SEnum::DI_HAMEL_BREAK_RESIAM_NORMAL:
	case SEnum::DI_HAMEL_ANCIENT_WATERWAY_NORMAL:
	case SEnum::DI_HAMEL_ANCIENT_WATERWAY_CORE_NORMAL:
	case SEnum::DI_EVENT_TREE_DAY_HAMEL:
	case SEnum::DI_HAMEL_CAVE_OF_MAGMANTA_NORMAL:
	case SEnum::DI_HAMEL_FROZEN_WATER_TEMPLE_NORMAL:
	case SEnum::DI_HAMEL_FROZEN_WATER_HALL_NORMAL:

	// cocy33 / 김창한 / 2013.08.14 / 하멜 비던
	case SEnum::DI_HAMEL_SECRET_COMMON:
	case SEnum::DI_HAMEL_SECRET_HELL:
		//{{ 2011. 10.10	할로윈 던전 하드코딩
#ifdef SERV_HALLOWEEN_DUNGEON
	case SEnum::DI_ELDER_HALLOWEEN_NORMAL: // 할로윈 이벤트 던전
#endif SERV_HALLOWEEN_DUNGEON
		//}}
		return true;
	}

	return false;    
}

//{{ 2012. 10. 29	박세훈	샌더 마을 추가
#ifdef SERV_VILLAGE_SANDER
bool CXSLDungeon::IsSanderDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_EVENT_TREE_DAY_HAMEL:
		//{{ 허상형 : [2011/2/20/] //	월드 미션
	//case DI_DEFENCE_DUNGEON_HAMEL_NORMAL:
		//{{ 2011. 10.10	할로윈 던전 하드코딩
#ifdef SERV_HALLOWEEN_DUNGEON
	case SEnum::DI_ELDER_HALLOWEEN_NORMAL: // 할로윈 이벤트 던전
#endif SERV_HALLOWEEN_DUNGEON
		//}}

	case SEnum::DI_SANDER_DRY_SANDER_NORMAL:
	case SEnum::DI_SANDER_GARPAI_ROCK_NORMAL:
	case SEnum::DI_SANDER_DEN_OF_TROCK_NORMAL:
	case SEnum::DI_SANDER_KARUSO_VILLAGE_NORMAL:
	case SEnum::DI_SANDER_SANTILUS_SHIP_NORMAL :
	case SEnum::DI_SANDER_BEHIMOSS_HEART_NORMAL :
		return true;
	}

	return false;
}
#endif SERV_VILLAGE_SANDER
//}}

#ifdef SERV_DUNGEON_OPTION_IN_LUA
bool CXSLDungeon::IsFixedMembers( IN int iDungeonID )
{
	return SiCXSLDungeonManager()->IsFixedMembers( iDungeonID );
}
short CXSLDungeon::GetFixedMembers( IN int iDungeonID )
{
	return SiCXSLDungeonManager()->GetFixedMembers( iDungeonID );
}
#endif SERV_DUNGEON_OPTION_IN_LUA

//{{ 2009. 4. 24  최육사	이벤트 던전
bool CXSLDungeon::IsEventDungeon( IN int iDungeonID )
{
#ifdef SERV_DUNGEON_OPTION_IN_LUA
	if( SiCXSLDungeonManager()->IsEventDungeon( iDungeonID ) == true )
		return true;
#endif SERV_DUNGEON_OPTION_IN_LUA

	switch( iDungeonID )
	{
	case SEnum::DI_EVENT_KIDDAY_RUBEN:
	case SEnum::DI_EVENT_KIDDAY_ELDER:
	case SEnum::DI_EVENT_KIDDAY_BESMA:
	case SEnum::DI_EVENT_KIDDAY_ALTERA:

	case SEnum::DI_EVENT_TREE_DAY_ELDER:		// 식목일 이벤트 던전
	case SEnum::DI_EVENT_TREE_DAY_BESMA:
	case SEnum::DI_EVENT_TREE_DAY_ALTERA:
	case SEnum::DI_EVENT_TREE_DAY_PEITA:
	case SEnum::DI_EVENT_TREE_DAY_VELDER:
	case SEnum::DI_EVENT_TREE_DAY_HAMEL:

		//{{ 2010. 10. 19	최육사	비밀던전 이벤트 업데이트
#ifdef SERV_SECRET_DUNGEON_EVENT
	case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
	case SEnum::DI_ELDER_HALLOWEEN_HARD:
	case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
#endif SERV_SECRET_DUNGEON_EVENT
		//}}
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	CASE_DEFENCE_DUNGEON
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
		//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	case SEnum::DI_EVENT_VALENTINE_DAY:
#endif SERV_EVENT_VALENTINE_DUNGEON
		//}
#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / 강정훈
	case SEnum::DI_EVENT_HALLOWEEN_DAY:
#endif //SERV_HALLOWEEN_EVENT_2013
		return true;
	}

	return false;
}
//}}

//{{ 2009. 7. 26  최육사	헤니르 시공
bool CXSLDungeon::IsHenirDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_ELDER_HENIR_SPACE:
		return true;
	}

	return false;
}
//}}

bool CXSLDungeon::IsSinglePlayDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_EL_FOREST_GATE_NORMAL:
	case SEnum::DI_TUTORIAL_ELSWORD:
	case SEnum::DI_TUTORIAL_ARME:
	case SEnum::DI_TUTORIAL_LIRE:
	case SEnum::DI_TUTORIAL_RAVEN:
	case SEnum::DI_TUTORIAL_EVE:
	case SEnum::DI_TUTORIAL_CHUNG:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_ELSWORD:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_AISHA:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_LENA:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_RAVEN:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_EVE:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_CHUNG:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_ARA:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_EL:
	case SEnum::DI_BATTLE_FIELD_TUTORIAL_ADD:
		return true;
	}

	return false;
}

//{{ 2009. 3. 3  최육사		드롭지역 제한
bool CXSLDungeon::IsItemDropDungeon( IN int iItemID, IN int iDungeonID )
{
	switch( iItemID )
	{
	case 91620:  // 마법의 얼음가루
	case 91630:  // 연금술의 비약
		{
			if( SEnum::DI_EL_FOREST_GATE_NORMAL == iDungeonID ||
				CXSLDungeon::IsTutorialDungeon( iDungeonID ) || 
				CXSLDungeon::IsTCDungeon( iDungeonID ) )
			{
				return false;
			}
		}
		break;
	}

	return true;
}

//{{ 2010. 03. 18  최육사	비밀던전 헬모드
bool CXSLDungeon::IsValidDungeonMode( DUNGEON_MODE eDungeonMode )
{
	switch( eDungeonMode )
	{
	case DM_INVALID:
	case DM_HENIR_PRACTICE:
	case DM_HENIR_CHALLENGE:
	case DM_SECRET_NORMAL:
	case DM_SECRET_HELL:
		return true;
	}

	return false;
}
//}}

//{{ 2009. 12. 18  최육사	서버군 확장
bool CXSLDungeon::IsScriptCheckDungeon( IN int iDungeonID )
{
	if( CXSLDungeon::IsHenirDungeon( iDungeonID ) == true )
		return false;

	if( CXSLDungeon::IsEventDungeon( iDungeonID ) == true )
		return false;

	switch( iDungeonID )
	{
	case SEnum::DI_ELDER_NEWYEAR_NORMAL:
	case SEnum::DI_TEST_DUNGEON_NORMAL:
	case SEnum::DI_TEST_DUNGEON_HARD:
	case SEnum::DI_TEST_DUNGEON_EXPERT:
	case SEnum::DI_TEST1_DUNGEON_NORMAL:
	case SEnum::DI_TEST1_DUNGEON_HARD:
	case SEnum::DI_TEST1_DUNGEON_EXPERT:
	case SEnum::DI_TEST2_DUNGEON_NORMAL:
	case SEnum::DI_TEST2_DUNGEON_HARD:
	case SEnum::DI_TEST2_DUNGEON_EXPERT:
	case SEnum::DI_TEST3_DUNGEON_NORMAL:
	case SEnum::DI_TEST3_DUNGEON_HARD:
	case SEnum::DI_TEST3_DUNGEON_EXPERT:
		return false;
	}

	return true;
}
//}}

//{{ 2010. 03. 25  최육사	비밀던전 헬모드
bool CXSLDungeon::IsSecretDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_EL_FOREST_HELL_NORMAL:
	case SEnum::DI_EL_FOREST_HELL_HARD:
	case SEnum::DI_EL_FOREST_HELL_EXPERT:
	case SEnum::DI_ELDER_WALLY_CASTLE_LAB_NORMAL:
	case SEnum::DI_ELDER_WALLY_CASTLE_LAB_HARD:
	case SEnum::DI_ELDER_WALLY_CASTLE_LAB_EXPERT:
	case SEnum::DI_BESMA_SECRET_NORMAL:
	case SEnum::DI_BESMA_SECRET_HARD:
	case SEnum::DI_BESMA_SECRET_EXPERT:
	case SEnum::DI_ALTERA_SECRET_NORMAL:
	case SEnum::DI_ALTERA_SECRET_HARD:
	case SEnum::DI_ALTERA_SECRET_EXPERT:
		//{{ 2010. 04. 05  최육사	비밀던전 헬모드
	case SEnum::DI_RUBEN_SECRET_COMMON:
	case SEnum::DI_RUBEN_SECRET_HELL:
	case SEnum::DI_ELDER_SECRET_COMMON:
	case SEnum::DI_ELDER_SECRET_HELL:
	case SEnum::DI_BESMA_SECRET_COMMON:
	case SEnum::DI_BESMA_SECRET_HELL:
	case SEnum::DI_ALTERA_SECRET_COMMON:
	case SEnum::DI_ALTERA_SECRET_HELL:
		//}}
		//{{ JHKang / 강정훈 / 2011.8.24
	case SEnum::DI_VELDER_SECRET_COMMON:
	case SEnum::DI_VELDER_SECRET_HELL:
		//}} 
	case SEnum::DI_HAMEL_SECRET_COMMON:
	case SEnum::DI_HAMEL_SECRET_HELL:
		return true;
	}

	return false;
}
//}}

//{{ 2010. 12. 28	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON
bool CXSLDungeon::IsShipDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_BATTLE_SHIP_VELDER:
	case SEnum::DI_BATTLE_SHIP_HAMEL:
		return true;
	}

	return false;
}
#endif SERV_SHIP_DUNGEON
//}}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
bool CXSLDungeon::IsDefenceDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	CASE_DEFENCE_DUNGEON
		return true;

	default:
		return false;
	}

	return false;
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
bool CXSLDungeon::IsValentineDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_EVENT_VALENTINE_DAY:
		return true;
	}

	return false;
}
#endif SERV_EVENT_VALENTINE_DUNGEON
//}

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
bool CXSLDungeon::IsValentineDungeonInt( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_EVENT_VALENTINE_DUNGEON_INT:
		return true;
	}

	return false;
}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / 강정훈
bool CXSLDungeon::IsHalloweenDungeon( IN int iDungeonID )
{
	switch( iDungeonID )
	{
	case SEnum::DI_EVENT_HALLOWEEN_DAY:
		return true;
	}

	return false;
}
#endif //SERV_HALLOWEEN_EVENT_2013

//{{ 2010. 04. 30  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
float CXSLDungeon::CheckDungeonBalRateByNpcLv( IN const int iUnitLevel, IN const int iNpcLv )
{
	const int nConst = ::abs( iNpcLv - iUnitLevel );
	if( nConst <= 3 )
	{
		return 1.0f;
	}

	const float fFactor = static_cast<float>( 20 - nConst ) / 20.0f;
	if( fFactor < 0.0f )
	{
		return 0.0f;
	}

	return fFactor;
}
#endif SERV_SECRET_HELL
//}}

//{{ 2012. 06. 29	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
float CXSLDungeon::GetMonsterLevelBalanceRate( IN const int iUnitLevel, IN const int iNpcLv )
{
	const int nConst = ::abs( iNpcLv - iUnitLevel );
	if( nConst <= 3 )
	{
		return 1.0f;
	}

	const float fFactor = static_cast<float>( 10 - nConst ) / 10.0f;
	if( fFactor < 0.0f )
	{
		return 0.0f;
	}

	return fFactor;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#ifdef SERV_HENIR_EVENT_SORT_NORMAL_CHALLENGE
bool CXSLDungeon::IsHenirChallengeMode( IN int iDungeonModeID )
{
	switch( iDungeonModeID )
	{
	case DM_HENIR_CHALLENGE:
		return true;
	}

	return false;
}
bool CXSLDungeon::IsHenirPracticeMode( IN int iDungeonModeID )
{
	switch( iDungeonModeID )
	{
	case DM_HENIR_PRACTICE:
		return true;
	}

	return false;
}
#endif SERV_HENIR_EVENT_SORT_NORMAL_CHALLENGE

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
int CXSLDungeon::GetSecretStageEnteringEvent( IN const int iStageIndex, IN const int iSubStageIndex, IN int iClearConditionIndex ) const
{
	//{{ 2009. 7. 15  최육사	index 범위 체크
	if( iStageIndex < 0 || iStageIndex >= (int)m_vecStageData.size() )
	{
		START_LOG( cerr, L"stage index값이 이상합니다." )
			<< BUILD_LOG( iStageIndex )
			<< BUILD_LOG( m_vecStageData.size() )
			<< END_LOG;
		return 0;
	}
	//}}
	CXSLDungeonStage::StageData* pStageData = m_vecStageData[iStageIndex];

	if( iSubStageIndex < 0 || iSubStageIndex >= (int)pStageData->m_SubStageDataList.size() )
	{
		START_LOG( cerr, L"substage index값이 이상합니다." )
			<< BUILD_LOG( iSubStageIndex )
			<< BUILD_LOG( pStageData->m_SubStageDataList.size() )
			<< END_LOG;
		return 0;
	}
	CXSLDungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[iSubStageIndex];

	return pSubStageData->GetSecretStageEnteringEvent( iClearConditionIndex );
}

bool CXSLDungeon::GetNextStage( OUT CXSLDungeonSubStage::NextStageData& kNextStageData, IN const int iStageIndex, IN const int iSubStageIndex, IN int iClearConditionIndex, IN int iSecretPadIndex ) const
{
	//{{ 2009. 7. 15  최육사	index 범위 체크
	if( iStageIndex < 0 || iStageIndex >= (int)m_vecStageData.size() )
	{
		START_LOG( cerr, L"stage index값이 이상합니다." )
			<< BUILD_LOG( iStageIndex )
			<< BUILD_LOG( m_vecStageData.size() )
			<< END_LOG;
		return false;
	}
	//}}
	CXSLDungeonStage::StageData* pStageData = m_vecStageData[iStageIndex];

	if( iSubStageIndex < 0 || iSubStageIndex >= (int)pStageData->m_SubStageDataList.size() )
	{
		START_LOG( cerr, L"substage index값이 이상합니다." )
			<< BUILD_LOG( iSubStageIndex )
			<< BUILD_LOG( pStageData->m_SubStageDataList.size() )
			<< END_LOG;
		return false;
	}
	CXSLDungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[iSubStageIndex];

	return pSubStageData->GetNextStage( kNextStageData, iClearConditionIndex, iSecretPadIndex );
}
#endif // SERV_STAGE_CLEAR_IN_SERVER

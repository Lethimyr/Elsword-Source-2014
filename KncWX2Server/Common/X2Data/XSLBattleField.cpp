#include "XSLBattleField.h"
#include "X2Data/XSLRoom.h"
#include "Enum/Enum.h"
#include "NetError.h"


CXSLBattleField::CXSLBattleField()
	: m_luaState( NULL )
	, m_eBattleFieldID( SEnum::BFI_INVALID )
	, m_sWorldID( 0 )
	, m_iStandardMonsterLevel( 0 )
	, m_iMaxNumberMonster( 0 )
	, m_iMonsterRespawnFactorByUserCount( 0 )
	, m_iMaxJoinUserCount( SEnum::BATTLE_FIELD_MAX_NUM )
	, m_iRequireLevel( 0 )
{
}

CXSLBattleField::~CXSLBattleField()
{
}

bool CXSLBattleField::OpenScriptFile()
{
	// Enum.lua 파일은 파싱되어 있어야 한다.

	KLuaManager luaManager;

	KAutoPath		kautoPaht;
	std::string		strFile;

	strFile = "Enum.lua";
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< END_LOG;
		return false;
	}

	strFile = "DungeonEnum.lua";
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< END_LOG;
		return false;
	}

	strFile = "DLG_Map_Enum.lua";
	kautoPaht.GetPullPath( strFile );

	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< END_LOG;
		return false;
	}
	
	lua_tinker::class_add<CXSLBattleField>( luaManager.GetLuaState(), "CXSLBattleField" );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "SetTeamStartPos",		&CXSLBattleField::SetTeamStartPos_LUA );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "AddStartPos",			&CXSLBattleField::LoadLineMapData_LUA );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "SetLandHeight",			&CXSLBattleField::SetLandHeight_LUA );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "AddLine_LUA",			&CXSLBattleField::AddLine_LUA );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "AddCameraData_LUA",		&CXSLBattleField::AddCameraData_LUA );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "AddSpawnMonsterGroup",	&CXSLBattleField::AddSpawnMonsterGroup_LUA );
	lua_tinker::decl( luaManager.GetLuaState(), "g_pLineMap", this );
	lua_tinker::decl( luaManager.GetLuaState(), "g_pBattleFieldManager", this );

	lua_tinker::class_add<D3DXVECTOR3>( luaManager.GetLuaState(), "D3DXVECTOR3" );
#ifdef _CONVERT_VS_2010
	lua_tinker::class_con<D3DXVECTOR3>( luaManager.GetLuaState(), lua_tinker::constructor<float, float, float> );
#else
	lua_tinker::class_con<D3DXVECTOR3>( luaManager.GetLuaState(), lua_tinker::constructor<float, float, float>() );
#endif _CONVERT_VS_2010
	lua_tinker::class_mem<D3DXVECTOR3>( luaManager.GetLuaState(), "x", &D3DXVECTOR3::x );
	lua_tinker::class_mem<D3DXVECTOR3>( luaManager.GetLuaState(), "y", &D3DXVECTOR3::y );
	lua_tinker::class_mem<D3DXVECTOR3>( luaManager.GetLuaState(), "z", &D3DXVECTOR3::z );

	// lua state값 저장!
	m_luaState = luaManager.GetLuaState();

	if( m_wstrLineMapFileName.empty() )
	{
		START_LOG( cout, L"[알림] 해당 필드에는 라인맵 스크립트가 없네요!" )
			<< BUILD_LOG( m_eBattleFieldID )
			<< END_LOG;
		return true;
	}

	strFile = KncUtil::toNarrowString( m_wstrLineMapFileName );
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		std::wstring wstrErr = KncUtil::toWideString( lua_tostring( luaManager.GetLuaState(), -1 ) );
		START_LOG( cerr, m_wstrLineMapFileName )
			<< BUILD_LOG( wstrErr )
			<< END_LOG;
		return false;
	}

	if( m_wstrNpcTableFileName.empty() )
	{
		START_LOG( cout, L"[알림] 해당 필드에는 NPC 스크립트가 없네요!" )
			<< BUILD_LOG( m_eBattleFieldID )
			<< END_LOG;
		return true;
	}

	strFile = KncUtil::toNarrowString( m_wstrNpcTableFileName );
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		std::wstring wstrErr = KncUtil::toWideString( lua_tostring( luaManager.GetLuaState(), -1 ) );
		START_LOG( cerr, m_wstrNpcTableFileName )
			<< BUILD_LOG( wstrErr )
			<< END_LOG;
		return false;
	}

	return true;
}

bool CXSLBattleField::LoadBattleFieldData( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN OUT KLuaManager& kLuaManager )
{
	m_eBattleFieldID = eBattleFieldID;

	//{{ 2012. 11. 13	최육사		입장 가능한 마을 검사 및 보정
#ifdef SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	LUA_GET_VALUE_RETURN(		kLuaManager, L"ENTER_VILLAGE_MAP_ID",						m_iEnterVillageMapID,		0,		return false; );
#endif SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	//}}
	LUA_GET_VALUE_RETURN(		kLuaManager, L"WORLD_ID",									m_sWorldID,					0,		return false; );
	LUA_GET_VALUE_RETURN(		kLuaManager, L"LINE_MAP_FILE_NAME",							m_wstrLineMapFileName,		L"",	return false; );
	LUA_GET_VALUE(				kLuaManager, L"NPC_TABLE_FILE_NAME",						m_wstrNpcTableFileName,		L"" );
	if( m_wstrNpcTableFileName.empty() )
	{
		START_LOG( cout, L"[알림] 해당 배틀필드는 NPC테이블 정보가 없습니다! 몬스터가 출현하지 않을 수 있으니 참고 바랍니다." )
			<< BUILD_LOG( eBattleFieldID );
	}

	LUA_GET_VALUE(				kLuaManager, L"REQUIRE_LEVEL",								m_iRequireLevel,			0 );
	if( kLuaManager.BeginTable( L"REQUIRE_DUNGEON" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( kLuaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf > 0 )
			{
				m_setRequireDungeon.insert( buf );
			}

			index++;
		}

		kLuaManager.EndTable();
	}

	LUA_GET_VALUE_RETURN(		kLuaManager, L"STANDARD_MONSTER_LEVEL",						m_iStandardMonsterLevel,	0,		return false; );
	//LUA_GET_VALUE_RETURN(		kLuaManager, L"BATTLE_FIELD_NAME",							m_wstrBattleFieldName,		L"",	return false; ); -- 서버에서는 파싱 안됩니다.
	LUA_GET_VALUE_RETURN(		kLuaManager, L"MAX_NUMBER_OF_MONSTER_IN_THIS_BATTLE_FIELD",	m_iMaxNumberMonster,		0,		return false; );
	LUA_GET_VALUE(				kLuaManager, L"MAX_JOIN_USER_COUNT",						m_iMaxJoinUserCount,		SEnum::BATTLE_FIELD_MAX_NUM );

	if( kLuaManager.BeginTable( L"BATTLE_FIELD_PORTAL_MOVING_INFO" ) == S_OK )
	{
		int index = 1;
		while( kLuaManager.BeginTable( index ) == S_OK )
		{
			CXSLBattleField::SPotalMovingInfo sPotalMovingInfo;
			LUA_GET_VALUE_RETURN_ENUM(	kLuaManager, L"PORTAL_MOVE_TYPE",		sPotalMovingInfo.m_ePortalMoveType,			CXSLBattleField::PORTAL_MOVE_TYPE, CXSLBattleField::PMT_INVALID, return false );
			LUA_GET_VALUE(				kLuaManager, L"LINE_NUMBER",			sPotalMovingInfo.m_iLineNumber,				0 );
			LUA_GET_VALUE(				kLuaManager, L"PLACE_ID_TO_MOVE",		sPotalMovingInfo.m_iPlaceIDToMove,			0 );
			LUA_GET_VALUE(				kLuaManager, L"POSITION_INDEX_TO_MOVE",	sPotalMovingInfo.m_iPositionIndexToMove,	0 );

			m_vecPotalMovingInfo.push_back( sPotalMovingInfo );

			index++;
			kLuaManager.EndTable();
		}
		kLuaManager.EndTable();
	}

	
		//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	if( kLuaManager.BeginTable( L"BATTLE_FIELD_MIDDLE_BOSS_INFO" ) == S_OK )
	{
		int index = 1; 
		while( kLuaManager.BeginTable( index ) == S_OK )
		{
			SRiskInfo sRiskInfo;
			LUA_GET_VALUE_RETURN(	kLuaManager, L"SPAWN_ID",		sRiskInfo.m_iSpawnID,	0, return false );

			if( kLuaManager.BeginTable( L"SPAWN_GROUP_ID" ) == S_OK )
			{
				int iValue	= 1; 
				int buf		= -1;
				while( kLuaManager.GetValue( iValue, buf ) == S_OK )
				{
					if( buf > 0 )
					{
						// 예외처리 중복 확인
						bool bExist = false;
						BOOST_TEST_FOREACH( int, iGroupID, sRiskInfo.m_vecSpawnGroupID )
						{
							if( iGroupID == buf )
							{
								bExist = true;
								break;
							}
						}

						if( bExist == false )
						{
							sRiskInfo.m_vecSpawnGroupID.push_back( buf );
						}
						else
						{
							START_LOG( cerr, L"존재하는 중보 그룹 ID 입력입니다." )
								<< BUILD_LOG( eBattleFieldID )
								<< END_LOG;
						}
					}
					iValue++;
				}

				kLuaManager.EndTable();
			}

			LUA_GET_VALUE_RETURN(	kLuaManager, L"SPAWN_RATE",				sRiskInfo.m_iSpawnRate,			0, return false );
			LUA_GET_VALUE_RETURN(	kLuaManager, L"RISK_VALUE",				sRiskInfo.m_iRiskValue,			0, return false );
			LUA_GET_VALUE(			kLuaManager, L"SHOW_BOSS_NAME",			sRiskInfo.m_bShowBossName,		false );
			LUA_GET_VALUE(			kLuaManager, L"BOSS_GAUGE_HP_LINES",	sRiskInfo.m_bBossGaugeHpLines,	false );

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
			if( sRiskInfo.m_iSpawnID > 100 )
			{
				m_vecEventBossInfo.push_back( sRiskInfo );
			}
			else
			{
				m_vecMiddleBossInfo.push_back( sRiskInfo );
			}
#else //SERV_BATTLEFIELD_EVENT_BOSS_INT
			m_vecMiddleBossInfo.push_back( sRiskInfo );
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT

			index++;
			kLuaManager.EndTable();
		}

		kLuaManager.EndTable();

		// 예외처리 확률 확인
		int iTotalRate = 0; 
		BOOST_TEST_FOREACH( SRiskInfo&, sRiskInfo, m_vecMiddleBossInfo )
		{
			iTotalRate += sRiskInfo.m_iSpawnRate;
		}

		if( iTotalRate < 100 || iTotalRate > 100 )
		{
			START_LOG( cerr, L"중보 그룹 확률값이 이상합니다." )
				<< BUILD_LOG( eBattleFieldID )
				<< BUILD_LOG( iTotalRate )
				<< END_LOG;
		}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		// 예외처리 확률 확인
		if( m_vecEventBossInfo.size() != 0 )
		{
			int iEventTotalRate = 0; 
			BOOST_TEST_FOREACH( SRiskInfo&, sRiskInfo, m_vecEventBossInfo )
			{
				iEventTotalRate += sRiskInfo.m_iSpawnRate;
			}

			if( iEventTotalRate < 100 || iEventTotalRate > 100 )
			{
				START_LOG( cerr, L"이벤트보스 그룹 확률값이 이상합니다." )
					<< BUILD_LOG( eBattleFieldID )
					<< BUILD_LOG( iEventTotalRate )
					<< END_LOG;
			}
		}	
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
	}
#else
	if( kLuaManager.BeginTable( L"BATTLE_FIELD_RISK_INFO" ) == S_OK )
	{
		int index = 1; 
		while( kLuaManager.BeginTable( index ) == S_OK )
		{
			SRiskInfo sRiskInfo;
			LUA_GET_VALUE_RETURN(	kLuaManager, L"RISK_VALUE",				sRiskInfo.m_iRiskValue,			0, return false );
			LUA_GET_VALUE_RETURN(	kLuaManager, L"SPAWN_MONSTER_ID",		sRiskInfo.m_iSpawnMonsterID,	0, return false );
			LUA_GET_VALUE_RETURN(	kLuaManager, L"SPAWN_GROUP_ID",			sRiskInfo.m_iSpawnGroupID,		0, return false );
			LUA_GET_VALUE(			kLuaManager, L"SHOW_BOSS_NAME",			sRiskInfo.m_bShowBossName,		false );
			LUA_GET_VALUE(			kLuaManager, L"BOSS_GAUGE_HP_LINES",	sRiskInfo.m_bBossGaugeHpLines,	false );

			m_vecRiskInfo.push_back( sRiskInfo );

			index++;
			kLuaManager.EndTable();
		}
		kLuaManager.EndTable();
	}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	return true;
}

bool CXSLBattleField::AddSpawnMonsterGroup_LUA()
{
	KLuaManager luaManager( m_luaState );

	SSpawnNpcGroup sNpcGroup;
	LUA_GET_VALUE_RETURN(	luaManager, L"SPAWN_GROUP_ID",							sNpcGroup.m_iGroupID,						0,	return false; );
	LUA_GET_VALUE(			luaManager, L"IS_BOSS_MONSTER_GROUP",					sNpcGroup.m_bIsBossMonsterGroup,			false );
	LUA_GET_VALUE(			luaManager, L"MIN_NUMBER_OF_MONSTER_IN_THIS_GROUP",		sNpcGroup.m_iMinNumberOfMonsterInThisGroup,	0 );
	LUA_GET_VALUE_RETURN(	luaManager, L"MAX_NUMBER_OF_MONSTER_IN_THIS_GROUP",		sNpcGroup.m_iMaxNumberOfMonsterInThisGroup,	0,	return false; );


	if( luaManager.BeginTable( L"SIEGE_MONSTER" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf > 0 )
			{
				sNpcGroup.m_setSiegeMonster.insert( buf );
			}

			index++;
		}

		luaManager.EndTable();
	}

	int index = 1;
	while( luaManager.BeginTable( index ) == S_OK )
	{
		SSpawnNpcInfo sNpcInfo;
		LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"MONSTER_ID",					sNpcInfo.m_eNpcID,		CXSLUnitManager::NPC_UNIT_ID, CXSLUnitManager::NUI_NONE, return false );
		LUA_GET_VALUE(				luaManager, L"RATE",						sNpcInfo.m_fRate,					0.f );
		LUA_GET_VALUE(				luaManager, L"MAX_NUMBER_OF_THIS_MONSTER",	sNpcInfo.m_iMaxNumberOfThisMonster,	0 );
		LUA_GET_VALUE(				luaManager, L"ADD_LEVEL",					sNpcInfo.m_iAddLevel,				0 );
		LUA_GET_VALUE(				luaManager, L"DROP",						sNpcInfo.m_bDrop,					false );
		LUA_GET_VALUE(				luaManager, L"AGRESSIVE",					sNpcInfo.m_bAggressive,				true );
		LUA_GET_VALUE(				luaManager, L"ADD_POS_Y",					sNpcInfo.m_fAddPosY,				0.f );
		//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		LUA_GET_VALUE(				luaManager, L"FOCUS_CAMERA",				sNpcInfo.m_bFocusCamera,			false );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}

		if( luaManager.BeginTable( L"START_POSITION" ) == S_OK )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == S_OK )
			{
				if( buf > 0 )
				{
					sNpcInfo.m_StartPositionLot.AddCaseSameProb( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}

		if( luaManager.BeginTable( L"PETROL_LINE_INDEX" ) == S_OK )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == S_OK )
			{
				if( buf > 0 )
				{
					sNpcInfo.m_vecPetrolLineIndex.push_back( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}

		if( luaManager.BeginTable( L"PLAY_LINE_INDEX" ) == S_OK )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == S_OK )
			{
				if( buf > 0 )
				{
					sNpcInfo.m_vecPlayLineIndex.push_back( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}

		sNpcGroup.m_mapSpawnNpcList.insert( std::make_pair( sNpcInfo.m_eNpcID, sNpcInfo ) );

		index++;
		luaManager.EndTable();
	}

	if( m_mapNpcGroupList.find( sNpcGroup.m_iGroupID ) != m_mapNpcGroupList.end() )
	{
		START_LOG( cerr, L"이미 등록된 몬스터 그룹입니다!" )
			<< BUILD_LOG( m_eBattleFieldID )
			<< BUILD_LOG( sNpcGroup.m_iGroupID )
			<< END_LOG;
		return false;
	}

	// 보스 몬스터 그룹이 아닌경우에만 리스트에 넣는다!
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	( sNpcGroup.m_bIsBossMonsterGroup == true ) ? m_vecBossNpcGroupList.push_back( sNpcGroup ) : m_vecRespawnNpcGroupList.push_back( sNpcGroup );
#else // SERV_BATTLE_FIELD_BOSS
	if( sNpcGroup.m_bIsBossMonsterGroup == false )
	{
		m_vecRespawnNpcGroupList.push_back( sNpcGroup );
	}	
#endif // SERV_BATTLE_FIELD_BOSS
	m_mapNpcGroupList.insert( std::make_pair( sNpcGroup.m_iGroupID, sNpcGroup ) );
	return true;
}

void CXSLBattleField::SetLandHeight_LUA( int iLandHeight )
{
	// 더미
}

void CXSLBattleField::SetTeamStartPos_LUA()
{
	// 더미
}

bool CXSLBattleField::LoadLineMapData_LUA( int iStartPos, D3DXVECTOR3 vPos, bool bRight, int iLineMapIndex )
{
	std::map< int, SMonsterStartPos >::iterator mitMSP;
	mitMSP = m_mapMonsterStartPos.find( iStartPos );
	if( mitMSP != m_mapMonsterStartPos.end() )
	{
		START_LOG( cerr, L"이미 존재하는 Start Pos값입니다." )
			<< BUILD_LOG( iStartPos )
			<< END_LOG;
		return false;
	}
	else
	{
		SMonsterStartPos sMonsterStartPos;
		sMonsterStartPos.m_vPos = VECTOR3( vPos.x, vPos.y, vPos.z );
		sMonsterStartPos.m_bRight = bRight;
		m_mapMonsterStartPos.insert( std::make_pair( iStartPos, sMonsterStartPos ) );
	}

	std::map< int, std::vector< int > >::iterator mitLRS;
	mitLRS = m_mapLineIndexRandomStartPos.find( iLineMapIndex );
	if( mitLRS == m_mapLineIndexRandomStartPos.end() )
	{
		std::vector< int > vecStartPosList;
		vecStartPosList.push_back( iStartPos );
		m_mapLineIndexRandomStartPos.insert( std::make_pair( iLineMapIndex, vecStartPosList ) );
	}
	else
	{
		mitLRS->second.push_back( iStartPos );
	}

	return true;
}

void CXSLBattleField::AddLine_LUA()
{
	// 더미
}

void CXSLBattleField::AddCameraData_LUA()
{
	// 더미
}

void CXSLBattleField::AddNpcGroup( IN const SSpawnNpcGroup& sSpawnNpcGroup )
{
	m_vecRespawnNpcGroupList.push_back( sSpawnNpcGroup );
}

//bool CXSLBattleField::GetBattleFieldRespawnMonsterList( IN const int iMaxMonsterCountInBattleField, 
//													    IN const int iMaxMonsterCountByUserCount,
//													    IN const int iRespawnTimeNotOverCount, 
//													    IN const std::map< int, SMonsterCountInfo >& mapMonsterCount, 
//														OUT std::vector< KNPCUnitReq >& vecNPCList )
//{
//	vecNPCList.clear();
//	
//	// 필요한 몬스터 수 만큼 몬스터를 결정내자!
//	int iRestCount = iNeedMonsterCount;
//	while( iRestCount > 0 )
//	{
//        // 1. 각 그룹을 한번씩 돌아가면서 출현시키자!
//		BOOST_TEST_FOREACH( const SSpawnNpcGroup&, sSpawnNpcGroup, m_vecNpcGroupList )
//		{
//			// 더 남은게 있는지 검사!
//			if( iRestCount <= 0 )
//			{
//				break;
//			}
//			else
//			{
//				// 남은 횟수를 1회 줄입니다.
//				--iRestCount;
//			}
//
//			KLottery kMonsterLot;
//			KLotterySameProb kMonsterSameLot;
//
//			// 2. 각 몬스터들에 대한 출현 조건을 만족하는게 있는지 확인!
//			{
//				std::map< int, SSpawnNpcInfo >::const_iterator mitSN;
//				for( mitSN = sSpawnNpcGroup.m_mapSpawnNpcList.begin(); mitSN != sSpawnNpcGroup.m_mapSpawnNpcList.end(); ++mitSN )
//				{
//					const SSpawnNpcInfo& sSpawnNpcInfo = mitSN->second;
//
//					// 해당 그룹의 몬스터 카운트 정보를 찾는다!
//					std::map< int, SMonsterCountInfo >::const_iterator mit;
//					mit = mapMonsterCount.find( static_cast<int>(sSpawnNpcGroup.m_iGroupID) );
//					if( mit != mapMonsterCount.end() )
//					{
//						int iMonsterCount = 0;
//						if( mit->second.GetMonsterCount( sSpawnNpcInfo.m_eNpcID, iMonsterCount ) == true )
//						{
//							// 이미 최대 출현 마리수를 충족했으므로 패스!
//							if( iMonsterCount >= sSpawnNpcInfo.m_iMaxNumberOfThisMonster )
//								continue;
//						}
//					}
//
//					// 몬스터 출현 확률을 결정!
//					LIF( kMonsterLot.AddCase( static_cast<int>(sSpawnNpcInfo.m_eNpcID), sSpawnNpcInfo.m_fRate ) );
//					kMonsterSameLot.AddCaseSameProb( static_cast<int>(sSpawnNpcInfo.m_eNpcID) );
//				}
//			}
//
//			// 3. 출현할 몬스터를 결정하자!
//			int iResultNpcID = kMonsterLot.Decision();
//			if( iResultNpcID == KLottery::CASE_BLANK )
//			{
//                iResultNpcID = kMonsterSameLot.DecisionSameProb();
//				if( iResultNpcID == KLottery::CASE_BLANK )
//				{
//					START_LOG( cerr, L"몬스터 랜덤 결정이 완전히 실패했다! 절대 일어나서는 안되는 에러!" )
//						<< BUILD_LOG( iResultNpcID )
//						<< END_LOG;
//					continue;
//				}
//			}
//
//			// 4. 출현할 몬스터가 결정되었다면 출현 세팅을 하자!
//			KNPCUnitReq kInfo;
//			if( MakeMonsterInfo( sSpawnNpcGroup, iResultNpcID, kInfo ) == false )
//			{
//				START_LOG( cerr, L"몬스터 정보 생성 실패!" )
//					<< BUILD_LOG( sSpawnNpcGroup.m_iGroupID )
//					<< BUILD_LOG( iResultNpcID )
//					<< END_LOG;
//				continue;
//			}
//			
//			// 5. 결과물을 저장!
//			vecNPCList.push_back( kInfo );
//		}
//	}
//
//	return true;
//}

bool CXSLBattleField::GetCreateMonsterList( IN const int iPlayerCount, OUT std::vector< KNPCUnitReq >& vecCreateNPCList ) const
{
	vecCreateNPCList.clear();
    	
	//////////////////////////////////////////////////////////////////////////
	// 필드내에 최소한으로 생성되어 있어야 하는 몬스터 생성!

	CXSLBattleField::KBattleFieldMonsterCountInfo kCreateMonsterCountInfo;


	//////////////////////////////////////////////////////////////////////////
	// 생성할 몬스터 수 구하기!
	int iCreateMonsterCount = 0;

	{
		// 현재 필드에서 최대 생성 가능한 몬스터 수 : 사람수에 비례한 몬스터수보다 필드 최대 몬스터수가 더 크다면 필드 최대 몬스터수를 적용한다.
		//const int MAX_MONSTER_COUNT_IN_BATTLE_FIELD = GetMaxMonsterCountInThisBattleField( iPlayerCount );
		iCreateMonsterCount = GetMaxNumberMonster(); // 현재 필드에서 등장하는 최대 몬스터 수!

		// 현재 총 몬스터 수 보다 유저수에 비례한 최대값이 더 크다면 그만큼 몬스터를 더 생성하자!
		//if( MAX_MONSTER_COUNT_IN_BATTLE_FIELD > kCreateMonsterCountInfo.GetTotalMonsterCount() )
		//{
		//	iCreateMonsterCount += ( MAX_MONSTER_COUNT_IN_BATTLE_FIELD - kCreateMonsterCountInfo.GetTotalMonsterCount() );
		//}
	}


	//////////////////////////////////////////////////////////////////////////
	// 몬스터 추가 또는 삭제
	
	// 생성해야하는 몬스터 수가 0이될때까지 loop를 돌자!
	while( 0 <= iCreateMonsterCount )
	{
		if( m_vecRespawnNpcGroupList.empty() == true )
		{
			START_LOG( cerr, L"몬스터 생성을 하려고 하는데 NPC 그룹이 비어있습니다! 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( m_vecRespawnNpcGroupList.size() )
				<< BUILD_LOG( iCreateMonsterCount )
				<< END_LOG;
			break;
		}

		bool bCreatedMonsterAnyGroup = false;

		BOOST_TEST_FOREACH( const SSpawnNpcGroup&, sSpawnNpcGroup, m_vecRespawnNpcGroupList )
		{
			// 생성해야할 몬스터 수가 없다면 여기서 나가자!
			if( iCreateMonsterCount <= 0 )
				break;

			// 보스 몬스터 그룹이면 지나가자!
			if( sSpawnNpcGroup.m_bIsBossMonsterGroup )
				continue;

			KNPCUnitReq kInfo;
			if( CreateMonsterFromSpawnNpcGroup( sSpawnNpcGroup, kCreateMonsterCountInfo, kInfo ) == true )
			{
				// 몬스터 생성 성공했다면 리스트에 넣자!
				vecCreateNPCList.push_back( kInfo );

				// 전체 카운트에도 계산하자!
				kCreateMonsterCountInfo.AddGroupMonsterCount( kInfo.m_iGroupID, kInfo.m_NPCID );

				// 생성해야할 몬스터 수값을 하나 줄이자!
				--iCreateMonsterCount;

				// 어떤 그룹이든 한마리의 몬스터가 생성되었다.
				bCreatedMonsterAnyGroup = true;
			}
		}

		// 더이상 생성할 몬스터 그룹이 남지 않았다면 여기로!
		if( bCreatedMonsterAnyGroup == false )
			break;
	}

	return true;
}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
bool CXSLBattleField::GetCreateBossMonsterList( OUT std::vector<KNPCUnitReq>& vecCreateNPCList ) const
{
	vecCreateNPCList.clear();

	//////////////////////////////////////////////////////////////////////////
	// 필드내에 최소한으로 생성되어 있어야 하는 몬스터 생성!

	CXSLBattleField::KBattleFieldMonsterCountInfo kCreateMonsterCountInfo;


	//////////////////////////////////////////////////////////////////////////
	// 생성할 몬스터 수 구하기!
	int iCreateMonsterCount = 0;

	{
		// 현재 필드에서 최대 생성 가능한 몬스터 수 : 사람수에 비례한 몬스터수보다 필드 최대 몬스터수가 더 크다면 필드 최대 몬스터수를 적용한다.
		//const int MAX_MONSTER_COUNT_IN_BATTLE_FIELD = GetMaxMonsterCountInThisBattleField( iPlayerCount );
		iCreateMonsterCount = GetMaxNumberMonster(); // 현재 필드에서 등장하는 최대 몬스터 수!

		// 현재 총 몬스터 수 보다 유저수에 비례한 최대값이 더 크다면 그만큼 몬스터를 더 생성하자!
		//if( MAX_MONSTER_COUNT_IN_BATTLE_FIELD > kCreateMonsterCountInfo.GetTotalMonsterCount() )
		//{
		//	iCreateMonsterCount += ( MAX_MONSTER_COUNT_IN_BATTLE_FIELD - kCreateMonsterCountInfo.GetTotalMonsterCount() );
		//}
	}


	//////////////////////////////////////////////////////////////////////////
	// 몬스터 추가 또는 삭제

	// 생성해야하는 몬스터 수가 0이될때까지 loop를 돌자!
	while( 0 <= iCreateMonsterCount )
	{
		if( m_vecBossNpcGroupList.empty() == true )
		{
			START_LOG( cerr, L"몬스터 생성을 하려고 하는데 NPC 그룹이 비어있습니다! 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( m_vecBossNpcGroupList.size() )
				<< BUILD_LOG( iCreateMonsterCount )
				<< END_LOG;
			break;
		}

		bool bCreatedMonsterAnyGroup = false;

		BOOST_TEST_FOREACH( const SSpawnNpcGroup&, sSpawnNpcGroup, m_vecBossNpcGroupList )
		{
			// 생성해야할 몬스터 수가 없다면 여기서 나가자!
			if( iCreateMonsterCount <= 0 )
				break;

			KNPCUnitReq kInfo;
			if( CreateMonsterFromSpawnNpcGroup( sSpawnNpcGroup, kCreateMonsterCountInfo, kInfo ) == true )
			{
				kInfo.m_cMonsterGrade = CXSLUnitManager::MG_BOSS_NPC;

				// 몬스터 생성 성공했다면 리스트에 넣자!
				vecCreateNPCList.push_back( kInfo );

				// 전체 카운트에도 계산하자!
				kCreateMonsterCountInfo.AddGroupMonsterCount( kInfo.m_iGroupID, kInfo.m_NPCID );

				// 생성해야할 몬스터 수값을 하나 줄이자!
				--iCreateMonsterCount;

				// 어떤 그룹이든 한마리의 몬스터가 생성되었다.
				bCreatedMonsterAnyGroup = true;
			}
		}

		// 더이상 생성할 몬스터 그룹이 남지 않았다면 여기로!
		if( bCreatedMonsterAnyGroup == false )
			break;
	}

	return true;
}
#endif // SERV_BATTLE_FIELD_BOSS


bool CXSLBattleField::GetRespawnMonsterList( IN const CXSLBattleField::KBattleFieldMonsterCountInfo& kAliveMonsterCountInfo,
											 IN const std::map< int, int >& mapRespawnReadyNpcCount,
											 OUT std::vector< KNPCUnitReq >& vecCreateNPCList ) const
{
	vecCreateNPCList.clear();

	KBattleFieldMonsterCountInfo kTotalMonsterCountInfo = kAliveMonsterCountInfo; // 전체 몬스터 수를 계산하기 위해 전달받은 살아있는 몬스터 수 객체를 복사합니다.


	// 그룹별로 몬스터 생성!
	//BOOST_TEST_FOREACH( const SSpawnNpcGroup&, sSpawnNpcGroup, m_vecNpcGroupList )
	//{
	//	int iCreateMonsterCount = 0;
	//	int iAliveGroupMonsterCount = 0;

	//	if( kCreateMonsterCountInfo.GetGroupMonsterCount( sSpawnNpcGroup.m_iGroupID, iAliveGroupMonsterCount ) == false )
	//	{
	//		// 해당 그룹에 생성된 몬스터가 한마리도 없으니 최소값을 필요한 몬스터 생성 수로 넣자!
	//		iCreateMonsterCount = sSpawnNpcGroup.m_iMinNumberOfMonsterInThisGroup;
	//	}
	//	else
	//	{
	//		// 현재 살아있는 몬스터 수가 최소생성몬스터수보다 작으면 더 생성 하도록 카운트를 올리자!
	//		if( iAliveGroupMonsterCount < sSpawnNpcGroup.m_iMinNumberOfMonsterInThisGroup )
	//		{
	//			iCreateMonsterCount = sSpawnNpcGroup.m_iMinNumberOfMonsterInThisGroup - iAliveGroupMonsterCount;
	//		}
	//	}

	//	// 생성이 필요한 몬스터수가 0이면 다음 그룹으로 가자!
	//	if( iCreateMonsterCount <= 0 )
	//		continue;

	//	// 필요한 숫자만큼 몬스터 생성!
	//	for( int iCount = 0; iCount < iCreateMonsterCount; ++iCount )
	//	{
	//		KNPCUnitReq kInfo;
	//		if( CreateMonsterFromSpawnNpcGroup( sSpawnNpcGroup, kCreateMonsterCountInfo, kInfo ) == true )
	//		{
	//			// 몬스터 생성 성공했다면 리스트에 넣자!
	//			vecCreateNPCList.push_back( kInfo );

	//			// 전체 카운트에도 계산하자!
	//			kCreateMonsterCountInfo.AddGroupMonsterCount( kInfo.m_iGroupID, kInfo.m_NPCID );
	//		}
	//	}
	//}

	// 몬스터 리스폰!
	std::map< int, int >::const_iterator mit;
	for( mit = mapRespawnReadyNpcCount.begin(); mit != mapRespawnReadyNpcCount.end(); ++mit )
	{
		const int iGroupID = mit->first;
		const int iRespawnCount = mit->second;

		const SSpawnNpcGroup* pSpawnNpcGroup = GetSpawnNpcGroup( iGroupID );
		if( IS_NULL( pSpawnNpcGroup ) )
		{
			START_LOG( cerr, L"존재 하지 않는 몬스터 그룹인데 리스폰 하려고 하였습니다." )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( iGroupID )
				<< END_LOG;
			continue;
		}

		for( int iCount = 0; iCount < iRespawnCount; ++iCount )
		{
			KNPCUnitReq kInfo;
			if( CreateMonsterFromSpawnNpcGroup( *pSpawnNpcGroup, kTotalMonsterCountInfo, kInfo ) == true )
			{
				// 몬스터 생성 성공했다면 리스트에 넣자!
				vecCreateNPCList.push_back( kInfo );

				// 전체 카운트에도 계산하자!
				kTotalMonsterCountInfo.AddGroupMonsterCount( kInfo.m_iGroupID, kInfo.m_NPCID );
			}
			else
			{
				START_LOG( cerr, L"몬스터 리스폰 실패! 절대 일어나서는 안되는 에러!" )
					<< BUILD_LOG( m_eBattleFieldID )
					<< BUILD_LOG( iGroupID )
					<< END_LOG;
			}
		}
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////



int	CXSLBattleField::GetMaxMonsterCountInThisBattleField( IN const int iPlayerCount ) const
{
	const int MAX_MONSTER_COUNT_BY_USER_COUNT = iPlayerCount * GetMonsterRespawnFactorByUserCount();
    return ( MAX_MONSTER_COUNT_BY_USER_COUNT < GetMaxNumberMonster() ? GetMaxNumberMonster() : MAX_MONSTER_COUNT_BY_USER_COUNT );
}

//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
bool CXSLBattleField::GetBattieFieldMiddleBossMonsterInfo( IN const int iDangerousValue, OUT std::vector<KNPCUnitReq>& vecNpcInfo )
{
	KLottery kMiddleBossLot;

	BOOST_TEST_FOREACH( const SRiskInfo&, sRandomInfo, m_vecMiddleBossInfo )
	{
		if( kMiddleBossLot.AddCase( sRandomInfo.m_iSpawnID, static_cast<float>(sRandomInfo.m_iSpawnRate) ) == false )
		{
			START_LOG( cerr, L"몬스터 출현 확률 결정에 문제가 생겼다!" )
				<< BUILD_LOG( sRandomInfo.m_iSpawnID )
				<< BUILD_LOG( sRandomInfo.m_iSpawnRate )
				<< END_LOG;
		}
	}

	if( kMiddleBossLot.GetCaseNum() == 0 )
	{
		START_LOG( clog, L"위험도에 따른 보스 몬스터 정보를 찾지 못하였습니다!" )
		<< BUILD_LOG( iDangerousValue )
		<< END_LOG;

		return false;
	}

	// 3. 출현할 몬스터를 결정하자!
	int iSpawnID = kMiddleBossLot.Decision();
	if( iSpawnID == KLottery::CASE_BLANK )
	{
		START_LOG( clog, L"위험도에 따른 보스 몬스터 정보를 찾지 못하였습니다!" )
			<< BUILD_LOG( iDangerousValue )
			<< END_LOG;

		return false;
	}

	BOOST_TEST_FOREACH( const SRiskInfo&, sRiskInfo, m_vecMiddleBossInfo )
	{
		// 랜덤으로 찾은 중간보스 Spwan Monster ID 를 찾는다
		if( sRiskInfo.m_iSpawnID != iSpawnID )
			continue;

		if( sRiskInfo.m_iRiskValue > iDangerousValue )			
			continue;

		BOOST_TEST_FOREACH( const int, iSpawnGroupID, sRiskInfo.m_vecSpawnGroupID )
		{
			const CXSLBattleField::SSpawnNpcGroup* pNpcSpawnGroup = GetSpawnNpcGroup( iSpawnGroupID );
			if( IS_NULL( pNpcSpawnGroup ) )
			{
				START_LOG( cerr, L"보스 몬스터가 출현할 스폰 그룹이 없습니다!" )
					<< BUILD_LOG( iSpawnGroupID )
					<< END_LOG;
				return false;
			}

			// 출현할 중보가 결정되었다면 출현 세팅을 하자!
			std::map< int, SSpawnNpcInfo >::const_iterator mitSNG = pNpcSpawnGroup->m_mapSpawnNpcList.begin();
			for( ; mitSNG != pNpcSpawnGroup->m_mapSpawnNpcList.end() ; ++mitSNG )
			{
				KNPCUnitReq kInfo;

				// RATE 를 비교해서 한마리를 출현 시키도록 하자
				KLottery kNpcLot;
				kNpcLot.AddCase( mitSNG->first, mitSNG->second.m_fRate );
				int iNpcID = kNpcLot.Decision();
				if( iNpcID == KLottery::CASE_BLANK )
					continue;
				
				if( MakeMiddleBossMonsterInfo( mitSNG->second, pNpcSpawnGroup->IsSiegeMonster( mitSNG->first ), sRiskInfo, kInfo ) == false )
				{
					START_LOG( cerr, L"중간 보스 몬스터 정보를 생성하지 못하였습니다!" )
						<< BUILD_LOG( iSpawnGroupID )
						<< END_LOG;
					return false;
				}

				vecNpcInfo.push_back( kInfo );
			}
		}

		return true;
	}

#ifdef DELETE_INCORRECT_ERROR_LOG
	// 중간 보스 몬스터를 의도해서 설정하지 않은 것에 대한 예외처리
	if( true == m_vecMiddleBossInfo.empty() )
	{
		switch ( m_eBattleFieldID )
		{
		case SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01:
			{
				return true;
			} break;
		default:
			break;
		}
	}
#endif // DELETE_INCORRECT_ERROR_LOG

	START_LOG( clog, L"위험도에 따른 보스 몬스터 정보를 찾지 못하였습니다!" )
		<< BUILD_LOG( iDangerousValue )
		<< END_LOG;

	return false;
}

bool CXSLBattleField::MakeMiddleBossMonsterInfo( IN const SSpawnNpcInfo& sSpawnNpcInfo, IN const bool bSiegeMode, IN const SRiskInfo& sRiskInfo, OUT KNPCUnitReq& kInfo ) const
{
	kInfo.Init(); // 의도적으로 초기화 시킨다!
	kInfo.m_bAggressive			= sSpawnNpcInfo.m_bAggressive;
	kInfo.m_NPCID				= sSpawnNpcInfo.m_eNpcID;            
	kInfo.m_iGroupID			= 0;
	kInfo.m_iBossGroupID		= sRiskInfo.m_iSpawnID;		// Spawn Group ID 가 아님, 몬스터 그룹핑 구분용 ID
	kInfo.m_Level				= m_iStandardMonsterLevel;  // 몬스터 기준 레벨
	kInfo.m_bAggressive			= sSpawnNpcInfo.m_bAggressive;
	if( sSpawnNpcInfo.m_StartPositionLot.Empty() == true )
	{
		kInfo.m_nStartPos		= GetMonsterStartPosByRandom( sSpawnNpcInfo.m_vecPetrolLineIndex );
		if( kInfo.m_nStartPos == -1 )
		{
			START_LOG( cerr, L"중보 몬스터 등장위치를 얻지 못했습니다!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( kInfo.m_iGroupID )
				<< BUILD_LOG( kInfo.m_iBossGroupID )
				<< BUILD_LOG( kInfo.m_NPCID )
				<< BUILD_LOG( sSpawnNpcInfo.m_vecPetrolLineIndex.size() )
				<< BUILD_LOG( kInfo.m_nStartPos )
				<< END_LOG;
		}
	}
	else
	{
		kInfo.m_nStartPos		= sSpawnNpcInfo.m_StartPositionLot.DecisionSameProb();
	}
	LIF( GetMonsterStartPosInfo( kInfo.m_nStartPos, kInfo.m_vPos, kInfo.m_bIsRight ) );
	kInfo.m_vecPetrolLineIndex	= sSpawnNpcInfo.m_vecPetrolLineIndex;
	kInfo.m_vecPlayLineIndex	= sSpawnNpcInfo.m_vecPlayLineIndex;
	kInfo.m_bActive				= true;
	//kInfo.m_fDelayTime;
	//kInfo.m_KeyCode;
	kInfo.m_bShowGage			= true;
	kInfo.m_AddPos.y			= sSpawnNpcInfo.m_fAddPosY;
	kInfo.m_bShowBossName		= sRiskInfo.m_bShowBossName;
	kInfo.m_usBossGaugeHPLines	= sRiskInfo.m_bBossGaugeHpLines;
	kInfo.m_bFocusCamera		= sSpawnNpcInfo.m_bFocusCamera;
	//kInfo.m_bHasBossGage;
	//kInfo.m_bShowSubBossName;
	kInfo.m_bSiegeMode			= bSiegeMode;
	kInfo.m_bNoDrop				= !(sSpawnNpcInfo.m_bDrop);
	kInfo.m_fUnitScale			= 1.f;
	kInfo.m_cMonsterGrade		= CXSLUnitManager::MG_MIDDLE_BOSS_NPC;
	kInfo.m_cTeamNum			= CXSLRoom::TN_MONSTER;
	kInfo.m_iAllyUID			= -1;
	return true;
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
bool CXSLBattleField::GetBattieFieldEventBossMonsterInfo( IN const int iDangerousValue, OUT std::vector<KNPCUnitReq>& vecNpcInfo )
{
	KLottery kEventBossLot;

	BOOST_TEST_FOREACH( const SRiskInfo&, sRandomInfo, m_vecEventBossInfo )
	{
		if( kEventBossLot.AddCase( sRandomInfo.m_iSpawnID, static_cast<float>(sRandomInfo.m_iSpawnRate) ) == false )
		{
			START_LOG( cerr, L"몬스터 출현 확률 결정에 문제가 생겼다!" )
				<< BUILD_LOG( sRandomInfo.m_iSpawnID )
				<< BUILD_LOG( sRandomInfo.m_iSpawnRate )
				<< END_LOG;
		}
	}

	if( kEventBossLot.GetCaseNum() == 0 )
	{
		START_LOG( clog, L"위험도에 따른 보스 몬스터 정보를 찾지 못하였습니다!" )
			<< BUILD_LOG( iDangerousValue )
			<< END_LOG;

		return false;
	}

	// 3. 출현할 몬스터를 결정하자!
	int iSpawnID = kEventBossLot.Decision();
	if( iSpawnID == KLottery::CASE_BLANK )
	{
		START_LOG( clog, L"위험도에 따른 보스 몬스터 정보를 찾지 못하였습니다!" )
			<< BUILD_LOG( iDangerousValue )
			<< END_LOG;

		return false;
	}

	BOOST_TEST_FOREACH( const SRiskInfo&, sRiskInfo, m_vecEventBossInfo )
	{
		// 랜덤으로 찾은 중간보스 Spwan Monster ID 를 찾는다
		if( sRiskInfo.m_iSpawnID != iSpawnID )
			continue;

		if( sRiskInfo.m_iRiskValue > iDangerousValue )			
			continue;

		BOOST_TEST_FOREACH( const int, iSpawnGroupID, sRiskInfo.m_vecSpawnGroupID )
		{
			const CXSLBattleField::SSpawnNpcGroup* pNpcSpawnGroup = GetSpawnNpcGroup( iSpawnGroupID );
			if( IS_NULL( pNpcSpawnGroup ) )
			{
				START_LOG( cerr, L"보스 몬스터가 출현할 스폰 그룹이 없습니다!" )
					<< BUILD_LOG( iSpawnGroupID )
					<< END_LOG;
				return false;
			}

			// 출현할 중보가 결정되었다면 출현 세팅을 하자!
			std::map< int, SSpawnNpcInfo >::const_iterator mitSNG = pNpcSpawnGroup->m_mapSpawnNpcList.begin();
			for( ; mitSNG != pNpcSpawnGroup->m_mapSpawnNpcList.end() ; ++mitSNG )
			{
				KNPCUnitReq kInfo;

				// RATE 를 비교해서 한마리를 출현 시키도록 하자
				KLottery kNpcLot;
				kNpcLot.AddCase( mitSNG->first, mitSNG->second.m_fRate );
				int iNpcID = kNpcLot.Decision();
				if( iNpcID == KLottery::CASE_BLANK )
					continue;

				if( MakeEventBossMonsterInfo( mitSNG->second, pNpcSpawnGroup->IsSiegeMonster( mitSNG->first ), sRiskInfo, kInfo ) == false )
				{
					START_LOG( cerr, L"중간 보스 몬스터 정보를 생성하지 못하였습니다!" )
						<< BUILD_LOG( iSpawnGroupID )
						<< END_LOG;
					return false;
				}

				vecNpcInfo.push_back( kInfo );
			}
		}

		return true;
	}

	START_LOG( clog, L"위험도에 따른 보스 몬스터 정보를 찾지 못하였습니다!" )
		<< BUILD_LOG( iDangerousValue )
		<< END_LOG;

	return false;
}

bool CXSLBattleField::MakeEventBossMonsterInfo( IN const SSpawnNpcInfo& sSpawnNpcInfo, IN const bool bSiegeMode, IN const SRiskInfo& sRiskInfo, OUT KNPCUnitReq& kInfo ) const
{
	kInfo.Init(); // 의도적으로 초기화 시킨다!
	kInfo.m_bAggressive			= sSpawnNpcInfo.m_bAggressive;
	kInfo.m_NPCID				= sSpawnNpcInfo.m_eNpcID;            
	kInfo.m_iGroupID			= 0;
	kInfo.m_iBossGroupID		= sRiskInfo.m_iSpawnID;		// Spawn Group ID 가 아님, 몬스터 그룹핑 구분용 ID
	kInfo.m_Level				= m_iStandardMonsterLevel;  // 몬스터 기준 레벨
	kInfo.m_bAggressive			= sSpawnNpcInfo.m_bAggressive;
	if( sSpawnNpcInfo.m_StartPositionLot.Empty() == true )
	{
		kInfo.m_nStartPos		= GetMonsterStartPosByRandom( sSpawnNpcInfo.m_vecPetrolLineIndex );
		if( kInfo.m_nStartPos == -1 )
		{
			START_LOG( cerr, L"중보 몬스터 등장위치를 얻지 못했습니다!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( kInfo.m_iGroupID )
				<< BUILD_LOG( kInfo.m_iBossGroupID )
				<< BUILD_LOG( kInfo.m_NPCID )
				<< BUILD_LOG( sSpawnNpcInfo.m_vecPetrolLineIndex.size() )
				<< BUILD_LOG( kInfo.m_nStartPos )
				<< END_LOG;
		}
	}
	else
	{
		kInfo.m_nStartPos		= sSpawnNpcInfo.m_StartPositionLot.DecisionSameProb();
	}
	LIF( GetMonsterStartPosInfo( kInfo.m_nStartPos, kInfo.m_vPos, kInfo.m_bIsRight ) );
	kInfo.m_vecPetrolLineIndex	= sSpawnNpcInfo.m_vecPetrolLineIndex;
	kInfo.m_vecPlayLineIndex	= sSpawnNpcInfo.m_vecPlayLineIndex;
	kInfo.m_bActive				= true;
	//kInfo.m_fDelayTime;
	//kInfo.m_KeyCode;
	kInfo.m_bShowGage			= true;
	kInfo.m_AddPos.y			= sSpawnNpcInfo.m_fAddPosY;
	kInfo.m_bShowBossName		= sRiskInfo.m_bShowBossName;
	kInfo.m_usBossGaugeHPLines	= sRiskInfo.m_bBossGaugeHpLines;
	kInfo.m_bFocusCamera		= sSpawnNpcInfo.m_bFocusCamera;
	//kInfo.m_bHasBossGage;
	//kInfo.m_bShowSubBossName;
	kInfo.m_bSiegeMode			= bSiegeMode;
	kInfo.m_bNoDrop				= !(sSpawnNpcInfo.m_bDrop);
	kInfo.m_fUnitScale			= 1.f;
	kInfo.m_cMonsterGrade		= CXSLUnitManager::MG_EVENT_BOSS_NPC;
	kInfo.m_cTeamNum			= CXSLRoom::TN_MONSTER;
	kInfo.m_iAllyUID			= -1;
	return true;
}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

bool CXSLBattleField::GetBattieFieldBossMonsterInfo( IN const int iDangerousValue, OUT KNPCUnitReq& kNpcInfo )
{
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	// 중보 작업 하면서 수정 되었으니 보스 레이드 추가 시 변경 해야함
#else
	BOOST_TEST_FOREACH( const SRiskInfo&, sRiskInfo, m_vecRiskInfo )
	{
		if( sRiskInfo.m_iRiskValue > iDangerousValue )			
			continue;

		const CXSLBattleField::SSpawnNpcGroup* pNpcSpawnGroup = GetSpawnNpcGroup( sRiskInfo.m_iSpawnGroupID );
		if( IS_NULL( pNpcSpawnGroup ) )
		{
			START_LOG( cerr, L"보스 몬스터가 출현할 스폰 그룹이 없습니다!" )
				<< BUILD_LOG( sRiskInfo.m_iSpawnGroupID )
				<< END_LOG;
			return false;
		}

		// 여기서 출현시킬 그룹을 정해줘야한다!
		if( MakeMonsterInfo( *pNpcSpawnGroup, sRiskInfo.m_iSpawnMonsterID, kNpcInfo ) == false )
		{
			START_LOG( cerr, L"보스 몬스터 정보를 생성하지 못하였습니다!" )
				<< BUILD_LOG( sRiskInfo.m_iSpawnMonsterID )
				<< END_LOG;
			return false;
		}

		kNpcInfo.m_NPCID				= sRiskInfo.m_iSpawnMonsterID;
		kNpcInfo.m_bShowBossName		= sRiskInfo.m_bShowBossName;
		kNpcInfo.m_usBossGaugeHPLines	= sRiskInfo.m_bBossGaugeHpLines;
		kNpcInfo.m_bFocusCamera			= true;
		kNpcInfo.m_cMonsterGrade		= CXSLUnitManager::MG_BOSS_NPC; // 보스로 세팅
		return true;
	}

	START_LOG( clog, L"위험도에 따른 보스 몬스터 정보를 찾지 못하였습니다!" )
		<< BUILD_LOG( iDangerousValue )
		<< END_LOG;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	return false;
}

int	CXSLBattleField::GetMonsterStartPosByRandom( IN const std::vector< int >& vecLineMapIndexList ) const
{
	KLotterySameProb< int > kLotStartPosRandom;

    BOOST_TEST_FOREACH( const int, iLineMapIndex, vecLineMapIndexList )
	{
		std::map< int, std::vector< int > >::const_iterator mit;
		mit = m_mapLineIndexRandomStartPos.find( iLineMapIndex );
		if( mit == m_mapLineIndexRandomStartPos.end() )
		{
			// 오류가 아니라 정상적인 상황입니다.
			START_LOG( clog, L"해당 라인맵 인덱스에는 StartPos 정보가 없습니다." )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( iLineMapIndex )
				<< END_LOG;
			continue;
		}

		BOOST_TEST_FOREACH( const int, iStartPos, mit->second )
		{
			kLotStartPosRandom.AddCaseSameProb( iStartPos );
		}
	}

	if( kLotStartPosRandom.Empty() == true )
	{
		START_LOG( cerr, L"랜덤 결과값이 없습니다. 일어나서는 안되는 에러! 몬스터의 출현 위치가 랜덤하게 결정됩니다!!" )
			<< BUILD_LOG( m_eBattleFieldID )
			<< BUILD_LOG( vecLineMapIndexList.size() )
			<< END_LOG;
		return -1;
	}

	return kLotStartPosRandom.DecisionSameProb();
}

bool CXSLBattleField::GetMonsterStartPosInfo( IN const int iStartPos, OUT VECTOR3& vPos, OUT bool& bRight ) const
{
	std::map< int, SMonsterStartPos >::const_iterator mit;
	mit = m_mapMonsterStartPos.find( iStartPos );
	if( mit == m_mapMonsterStartPos.end() )
	{
		START_LOG( cerr, L"존재하지않는 Start Position입니다" )
			<< BUILD_LOG( m_eBattleFieldID )
			<< BUILD_LOG( iStartPos )
			<< END_LOG;
		return false;
	}

	vPos = mit->second.m_vPos;
	bRight = mit->second.m_bRight;
	return true;
}

const CXSLBattleField::SSpawnNpcGroup* CXSLBattleField::GetSpawnNpcGroup( IN const int iGroupID ) const
{
	std::map< int, SSpawnNpcGroup >::const_iterator mit;
	mit = m_mapNpcGroupList.find( iGroupID );
	if( mit == m_mapNpcGroupList.end() )
	{
		START_LOG( cerr, L"존재 하지 않는 몬스터 그룹입니다." )
			<< BUILD_LOG( iGroupID )
			<< END_LOG;
		return NULL;
	}

	return &mit->second;
}

bool CXSLBattleField::CreateMonsterFromSpawnNpcGroup( IN const SSpawnNpcGroup& sSpawnNpcGroup, IN const CXSLBattleField::KBattleFieldMonsterCountInfo& kMonsterCountInfo, OUT KNPCUnitReq& kInfo ) const
{
	KLottery kMonsterLot;

	// 1. 각 몬스터들에 대한 출현 조건을 만족하는게 있는지 확인!
	{
		std::map< int, SSpawnNpcInfo >::const_iterator mitSN;
		for( mitSN = sSpawnNpcGroup.m_mapSpawnNpcList.begin(); mitSN != sSpawnNpcGroup.m_mapSpawnNpcList.end(); ++mitSN )
		{
			const SSpawnNpcInfo& sSpawnNpcInfo = mitSN->second;

			// 살아있는 몬스터 정보에 해당 몬스터 GroupID정보가 존재 하는가?
			int iAliveMonsterCount = 0;
			if( kMonsterCountInfo.GetMonsterCountByGroupIDAndNpcID( sSpawnNpcGroup.m_iGroupID, sSpawnNpcInfo.m_eNpcID, iAliveMonsterCount ) == true )
			{
				// 해당 몬스터의 최대 출현 마리수를 충족했는지 확인하기! (몬스터 출현 MAX값 제한 1순위)
				if( iAliveMonsterCount >= sSpawnNpcInfo.m_iMaxNumberOfThisMonster )
					continue;
			}

			// 해당 그룹에 현재 몬스터가 몇마리 얻기!
			int iAliveGroupCount = 0;
			if( kMonsterCountInfo.GetGroupMonsterCount( sSpawnNpcGroup.m_iGroupID, iAliveGroupCount ) == true )
			{
				// 해당 그룹에서 최대 출현 마리수를 충족했는지 확인하기! (몬스터 출현 MAX값 제한 2순위)
				if( iAliveGroupCount >= sSpawnNpcGroup.m_iMaxNumberOfMonsterInThisGroup )
					continue;
			}

			// 몬스터 출현 확률을 결정!
			if( kMonsterLot.AddCase( static_cast<int>(sSpawnNpcInfo.m_eNpcID), sSpawnNpcInfo.m_fRate ) == false )
			{
                START_LOG( cerr, L"몬스터 출현 확률 결정에 문제가 생겼다!" )
					<< BUILD_LOG( mitSN->first )
					<< BUILD_LOG( sSpawnNpcInfo.m_eNpcID )
					<< BUILD_LOG( sSpawnNpcInfo.m_fRate )
					<< END_LOG;
			}
		}
	}

	// 2. 출현할 수 있는 몬스터 수를 넘었는지 체크!
	if( kMonsterLot.GetCaseNum() == 0 )
		return false;

	const float fExProb = ( 100.f - kMonsterLot.GetTotalProb() );
	const float fAddProb = fExProb / static_cast<float>(kMonsterLot.GetCaseNum());
	if( fAddProb > 0.f )
	{
		kMonsterLot.AddProbability( fAddProb );
	}

	// 3. 출현할 몬스터를 결정하자!
	int iResultNpcID = kMonsterLot.Decision();
	if( iResultNpcID == KLottery::CASE_BLANK )
	{
		START_LOG( cerr, L"몬스터 랜덤 결정이 실패했다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iResultNpcID )
			<< BUILD_LOG( kMonsterLot.GetTotalProb() )
			<< END_LOG;

		//////////////////////////////////////////////////////////////////////////		
		// 각 몬스터 출현 케이스 별로 랜덤 돌려서 결정하자!
		KLotterySameProb< int > kSameMonsterLot;

		std::vector< int > vecCaseList;
		kMonsterLot.GetCaseList( vecCaseList );

		BOOST_TEST_FOREACH( const int, iCaseID, vecCaseList )
		{
			kSameMonsterLot.AddCaseSameProb( iCaseID );
		}
        
		iResultNpcID = kSameMonsterLot.DecisionSameProb();
		if( iResultNpcID == KLottery::CASE_BLANK )
		{
			START_LOG( cerr, L"몬스터 랜덤 결정이 완전히 실패했다! 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( iResultNpcID )
				<< BUILD_LOG( kMonsterLot.GetCaseNum() )
				<< END_LOG;
			return false;
		}
	}

	// 4. 출현할 몬스터가 결정되었다면 출현 세팅을 하자!
	if( MakeMonsterInfo( sSpawnNpcGroup, iResultNpcID, kInfo ) == false )
	{
		START_LOG( cerr, L"몬스터 정보 생성 실패!" )
			<< BUILD_LOG( sSpawnNpcGroup.m_iGroupID )
			<< BUILD_LOG( iResultNpcID )
			<< END_LOG;
		return false;
	}

	return true;
}

bool CXSLBattleField::MakeMonsterInfo( IN const SSpawnNpcGroup& sSpawnNpcGroup, IN const int iNpcID, OUT KNPCUnitReq& kInfo ) const
{
	// 4. 출현할 몬스터가 결정되었다면 출현 세팅을 하자!
	const CXSLUnitManager::NPCUnitTemplet* pNpcTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iNpcID) );
	if( pNpcTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지않는 NpcID입니다!" )
			<< BUILD_LOG( iNpcID )
			<< END_LOG;
		return false;
	}

	std::map< int, SSpawnNpcInfo >::const_iterator mitSNG;
	mitSNG = sSpawnNpcGroup.m_mapSpawnNpcList.find( iNpcID );
	if( mitSNG == sSpawnNpcGroup.m_mapSpawnNpcList.end() )
	{
		START_LOG( cerr, L"등록되지않은 NpcID입니다!" )
			<< BUILD_LOG( sSpawnNpcGroup.m_iGroupID )
			<< BUILD_LOG( iNpcID )
			<< END_LOG;
		return false;
	}

	kInfo.Init(); // 의도적으로 초기화 시킨다!
	kInfo.m_iGroupID			= sSpawnNpcGroup.m_iGroupID;
	kInfo.m_NPCID				= iNpcID;
	kInfo.m_Level				= m_iStandardMonsterLevel + mitSNG->second.m_iAddLevel; // 몬스터 기준 레벨 + 추가 레벨
	kInfo.m_bAggressive			= mitSNG->second.m_bAggressive;
	if( mitSNG->second.m_StartPositionLot.Empty() == true )
	{
		kInfo.m_nStartPos		= GetMonsterStartPosByRandom( mitSNG->second.m_vecPetrolLineIndex );
		if( kInfo.m_nStartPos == -1 )
		{
			START_LOG( cerr, L"랜덤한 몬스터 등장위치를 얻지 못했습니다!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( sSpawnNpcGroup.m_iGroupID )
				<< BUILD_LOG( iNpcID )
				<< BUILD_LOG( mitSNG->second.m_vecPetrolLineIndex.size() )
				<< BUILD_LOG( kInfo.m_nStartPos )
				<< END_LOG;
		}
	}
	else
	{
		kInfo.m_nStartPos		= mitSNG->second.m_StartPositionLot.DecisionSameProb();
	}
	LIF( GetMonsterStartPosInfo( kInfo.m_nStartPos, kInfo.m_vPos, kInfo.m_bIsRight ) );
	kInfo.m_vecPetrolLineIndex	= mitSNG->second.m_vecPetrolLineIndex;
	kInfo.m_vecPlayLineIndex	= mitSNG->second.m_vecPlayLineIndex;
	kInfo.m_bActive				= true;
	//kInfo.m_fDelayTime;
	//kInfo.m_KeyCode;
	//kInfo.m_bFocusCamera;
	kInfo.m_bShowGage			= true;
	kInfo.m_AddPos.y			= mitSNG->second.m_fAddPosY;
	//kInfo.m_bHasBossGage;
	//kInfo.m_bShowBossName;
	//kInfo.m_bShowSubBossName;
	kInfo.m_bSiegeMode			= sSpawnNpcGroup.IsSiegeMonster( iNpcID );
	kInfo.m_bNoDrop				= !(mitSNG->second.m_bDrop);
	kInfo.m_fUnitScale			= 1.f;
	kInfo.m_cMonsterGrade		= CXSLUnitManager::MG_NORMAL_NPC; // 일단 일반NPC로 넣자
	kInfo.m_cTeamNum			= CXSLRoom::TN_MONSTER;
	kInfo.m_iAllyUID			= -1;
	return true;
}

bool CXSLBattleField::IsCanEnterBattleField( IN const u_char ucLevel, IN const std::map< int, KDungeonClearInfo >& mapDungeonClear, OUT int& iRequireLevel, OUT int& iRequireDungeonID ) const
{
	SET_ERROR( NET_OK );

	iRequireLevel = 0;
	iRequireDungeonID = 0;

    // 레벨 제한 조건 체크
	if( static_cast<int>(ucLevel) < GetRequireLevel() )
	{
		iRequireLevel = GetRequireLevel();

		SET_ERROR( ERR_BATTLEFIELD_13 );
		return false;
	}

	// 던전 클리어 조건 체크
	BOOST_TEST_FOREACH( const int, iDungeonID, m_setRequireDungeon )
	{
		if( mapDungeonClear.find( iDungeonID ) == mapDungeonClear.end() )
		{
            iRequireDungeonID = iDungeonID;

			SET_ERROR( ERR_BATTLEFIELD_14 );
			return false;
		}
	}

	return true;
}

//{{ 2013. 1. 1	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
bool CXSLBattleField::GetEventMonsterCreateInfo( OUT KNPCUnitReq& kInfo )
{
	// 4. 출현할 몬스터가 결정되었다면 출현 세팅을 하자!
	const CXSLUnitManager::NPCUnitTemplet* pNpcTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( static_cast<CXSLUnitManager::NPC_UNIT_ID>( kInfo.m_NPCID ) );
	if( pNpcTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지않는 NpcID입니다!" )
			<< BUILD_LOG( kInfo.m_NPCID )
			<< END_LOG;
		return false;
	}

	if( m_vecRespawnNpcGroupList.empty() == true )
	{
		START_LOG( cerr, L"가져올 수 있는 NPC 정보가 없습니다." )
			<< BUILD_LOG( m_eBattleFieldID )
			<< BUILD_LOG( m_sWorldID )
			<< END_LOG;
		return false;
	}

	unsigned int uiIndex = rand() % m_vecRespawnNpcGroupList.size();
	const SSpawnNpcGroup& sSpawnNpcGroup = m_vecRespawnNpcGroupList[uiIndex];
	
	uiIndex = rand() % sSpawnNpcGroup.m_mapSpawnNpcList.size();
	std::map<int, SSpawnNpcInfo>::const_iterator it = sSpawnNpcGroup.m_mapSpawnNpcList.begin();
	for( unsigned int i=0; i < uiIndex; ++i )
	{
		++it;
	}
	const SSpawnNpcInfo& sSpawnNpcInfo = it->second;
	const KLotterySameProb<int>&	StartPositionLot	= sSpawnNpcInfo.m_StartPositionLot;
	const std::vector<int>&			vecPetrolLineIndex	= sSpawnNpcInfo.m_vecPetrolLineIndex;
	const std::vector<int>&			vecPlayLineIndex	= sSpawnNpcInfo.m_vecPlayLineIndex;

	//kInfo.Init(); // 의도적으로 초기화 시킨다!
	//kInfo.m_iGroupID			= sSpawnNpcGroup.m_iGroupID;
	//kInfo.m_NPCID				= iNpcID;
	kInfo.m_Level				= m_iStandardMonsterLevel; // 몬스터 기준 레벨
	//kInfo.m_bAggressive			= mitSNG->second.m_bAggressive;
	if( StartPositionLot.Empty() == true )
	{
		kInfo.m_nStartPos		= GetMonsterStartPosByRandom( vecPetrolLineIndex );
		if( kInfo.m_nStartPos == -1 )
		{
			START_LOG( cerr, L"랜덤한 몬스터 등장위치를 얻지 못했습니다!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( kInfo.m_NPCID )
				<< BUILD_LOG( vecPetrolLineIndex.size() )
				<< BUILD_LOG( kInfo.m_nStartPos )
				<< END_LOG;
		}
	}
	else
	{
		kInfo.m_nStartPos		= StartPositionLot.DecisionSameProb();
	}
	LIF( GetMonsterStartPosInfo( kInfo.m_nStartPos, kInfo.m_vPos, kInfo.m_bIsRight ) );
	kInfo.m_vecPetrolLineIndex	= vecPetrolLineIndex;
	kInfo.m_vecPlayLineIndex	= vecPlayLineIndex;
	kInfo.m_bActive				= true;
	//kInfo.m_fDelayTime;
	//kInfo.m_KeyCode;
	//kInfo.m_bFocusCamera;
	kInfo.m_bShowGage			= true;
	//kInfo.m_AddPos.y			= mitSNG->second.m_fAddPosY;
	//kInfo.m_bHasBossGage;
	//kInfo.m_bShowBossName;
	//kInfo.m_bShowSubBossName;
	//kInfo.m_bSiegeMode			= sSpawnNpcGroup.IsSiegeMonster( iNpcID );
	//kInfo.m_bNoDrop				= !(mitSNG->second.m_bDrop);
	kInfo.m_fUnitScale			= 1.f;
	kInfo.m_cMonsterGrade		= CXSLUnitManager::MG_NORMAL_NPC; // 일단 일반NPC로 넣자
	kInfo.m_cTeamNum			= CXSLRoom::TN_MONSTER;
	kInfo.m_iAllyUID			= -1;
	return true;
}
#endif SERV_FIELD_EVENT_MONSTER
//}}

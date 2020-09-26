#include "StdAfx.h"
#include ".\X2BattleFieldManager.h"

////////////////////////////////////////////////////////////////////////////////////////
// CBattleFieldPortalMovingInfo
////////////////////////////////////////////////////////////////////////////////////////
/** @function : ParsingScriptFile
	@brief : CBattleFieldPortalMovingInfo에 필요한 정보 스크립트로 부터 읽어들임
	@param : IN KLuaManager& luaManager_
	@return : bool (필드에는 반드시 한개 이상의 포탈 정보가 있어야 하기 때문에 bool 값을 리턴)
*/
bool CBattleFieldPortalMovingInfo::ParsingScriptFile( IN KLuaManager& luaManager_ )
{
	// 포탈의 이동 타입 (CX2BattleFieldManager::PORTAL_MOVE_TYPE, 포탈->마을, 포탈->포탈)
	LUA_GET_VALUE( luaManager_, "PORTAL_MOVE_TYPE", m_usPortalMoveType, 0 );

	if ( 0 != m_usPortalMoveType )
	{
		// 필드의 라인맵에서 이 포탈이 위치한 필드 내의 라인 번호
		LUA_GET_VALUE( luaManager_, "LINE_NUMBER", m_uiLineNumber, 0 );

		// 이동할 장소의 ID (VillageMapID or BattleFieldID)
		LUA_GET_VALUE( luaManager_, "PLACE_ID_TO_MOVE", m_uiPlaceIdToMove, 0 );
		ASSERT( 0 != m_uiPlaceIdToMove );

		// 이동할 장소의 Postion Index로 보통 마을의 경우는 DLG_Map_Village.lua에 있는 StartPosId, 필드의 경우에는 StartPos(?) 가 될듯
		LUA_GET_VALUE( luaManager_, "POSITION_INDEX_TO_MOVE", m_usPositionIndexToMove, 0 );
		return true;
	}
	else
	{
		ASSERT( 0 != m_usPortalMoveType );
		StateLog( L"MOVE_TYPE is undefined!" );
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// CBattleFieldRiskInfo
////////////////////////////////////////////////////////////////////////////////////////
/** @function : ParsingScriptFile
	@brief : CBattleFieldRiskInfo에 필요한 정보 스크립트로 부터 읽어들임
	@param : IN KLuaManager& luaManager_
	@return : void (필드에는 RISK_INFO가 없을 수 도 있기 때문에 void 리턴 사용)
*/
bool CBattleFieldRiskInfo::ParsingScriptFile( IN KLuaManager& luaManager_ )
{
	bool bResult = true;
	// 위험도 수치
	LUA_GET_VALUE( luaManager_, "RISK_VALUE", m_usRiskValue, 0 );
	ASSERT( 0 != m_usRiskValue );	// 위험도 테이블이 있는데, 위험도 수치가 0이라는 것은 말이 안됨
	if ( 0 == m_usRiskValue )
	{
		StateLog( L"RISK_VALUE is undefined!" );
		bResult = false;
	}

	// 위험도 수치에 따라 출현할 몬스터
	LUA_GET_VALUE( luaManager_, "SPAWN_MONSTER_ID", m_uiMonsterIdToBeSpawned, 0 );
	ASSERT( 0 != m_uiMonsterIdToBeSpawned );	// 위험도 테이블이 있는데, 위험도 수치가 0이라는 것은 말이 안됨
	if ( 0 == m_uiMonsterIdToBeSpawned )
	{
		StateLog( L"SPAWN_MONSTER_ID is undefined!" );
		bResult = false;
	}

	// 보스 이름을 보여 줄 것인지 여부
	bool bShowBossName;
	LUA_GET_VALUE( luaManager_, "SHOW_BOSS_NAME", bShowBossName, false );
	SetBitSetFlag( FRI_SHOW_BOSS_NAME, bShowBossName );

	// 보스의 HP 게이지를 여러 줄로 나눌 것인지 여부
	bool bDevideBossHPGaugeToLines;
	LUA_GET_VALUE( luaManager_, "DEVIDE_BOSS_HP_GAUGE_TO_LINES", bDevideBossHPGaugeToLines, false );
	SetBitSetFlag( FRI_DEVIDE_BOSS_HP_GAUGE_TO_LINES, bDevideBossHPGaugeToLines );

	return bResult;
}

////////////////////////////////////////////////////////////////////////////////////////
// CBattleFieldData
////////////////////////////////////////////////////////////////////////////////////////
/** @function : ParsingScriptFile
	@brief : CBattleFieldData에 필요한 정보 스크립트로 부터 읽어들임 (m_wstrBattleFieldName, m_strFileNameAtLoading 은 외부에서 읽어들임)
	@param : IN KLuaManager& luaManager_
	@return : bool
*/
bool CBattleFieldData::ParsingScriptFile( IN KLuaManager& luaManager_ )
{
	bool bResult = true;
	// 필드 ID
	LUA_GET_VALUE( luaManager_, "BATTLE_FIELD_ID", m_uiBattleFieldId, 0 );
	if ( SEnum::VMI_INVALID == m_uiBattleFieldId )
	{
		ASSERT( SEnum::VMI_INVALID != m_uiBattleFieldId );
		StateLog( L"BATTLE_FIELD_ID is undefined!" );

		bResult = false;
	}

	// 이 필드에서 사용하는 World ID
	LUA_GET_VALUE( luaManager_, "WORLD_ID", m_uiWorldId, 0 );
	if ( CX2World::WI_NONE == m_uiWorldId )
	{
		ASSERT( CX2World::WI_NONE != m_uiWorldId );
		StateLog( L"WORLD_ID is undefined!" );

		bResult = false;
	}

	// 유저가 이 필드에서 죽고서 부활하지 않을 경우 돌아가게될 마을	
	LUA_GET_VALUE( luaManager_, "RETURN_VILLAGE_ID", m_uiReturnVillageId, 0 );

	// 현재 필드에서 나오는 몬스터들의 기준 레벨 
	LUA_GET_VALUE( luaManager_, "STANDARD_MONSTER_LEVEL", m_uiStandardMonsterLevel, 0 );
	if ( 0 == m_uiStandardMonsterLevel )
	{
		ASSERT( 0 != m_uiStandardMonsterLevel );
		StateLog( L"STANDARD_MONSTER_LEVEL is undefined!" );

		bResult = false;
	}

	// 현재 필드에서 최대로 나올 수 있는 몬스터 수
	LUA_GET_VALUE( luaManager_, "MAX_NUMBER_OF_MONSTER_IN_THIS_BATTLE_FIELD", m_usMaxNumberOfMonsterInThisBattleField, 0 );
	if ( 0 == m_usMaxNumberOfMonsterInThisBattleField )
	{
		ASSERT( 0 != m_usMaxNumberOfMonsterInThisBattleField );
		StateLog( L"MAX_NUMBER_OF_MONSTER_IN_THIS_BATTLE_FIELD is undefined!" );

		bResult = false;
	}

	// 필드 명
	int iStringIndex = 0;
	LUA_GET_VALUE( luaManager_, "BATTLE_FIELD_NAME", iStringIndex,	STR_ID_EMPTY );
	
	if ( STR_ID_EMPTY != iStringIndex )
		m_wstrBattleFieldName = GET_STRING( iStringIndex );
	else
	{
		ASSERT( STR_ID_EMPTY != iStringIndex );
		StateLog( L"BATTLE_FIELD_NAME is undefined!" );
		bResult = false;
	}

	// 필드 명을 나타내는 텍스쳐 파일 명
	LUA_GET_VALUE( luaManager_, "BATTLE_FIELD_TEXTURE_NAME", m_wstrBattleFieldTextureName, L"" );

	// 필드 명을 나타내는 텍스쳐 파일 키값
	LUA_GET_VALUE( luaManager_, "BATTLE_FIELD_TEXTURE_KEY", m_wstrBattleFieldTextureKey, L"" );

	// 필드 내에 있는 포탈의 이동 정보 파싱
	if ( false == ParsingBattleFieldMovingInfo( luaManager_ ) )
		bResult = false;

	// 필드의 위험도 정보
	if ( false == ParsingBattleFieldRiskInfo( luaManager_ ) )
		bResult = false;

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	ParsingBattleFieldMiddleBossInfo ( luaManager_ );
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
	LUA_GET_VALUE( luaManager_, "IS_BOSS_RAID_FIELD", m_bIsBossRaidField, false);
	LUA_GET_VALUE( luaManager_, "RAID_FIELD_PORTAL_POSITION_INDEX", m_usRaidFieldPortalPositionIndex, 0);
#endif // FIELD_BOSS_RAID

	return bResult;
}

/** @function : ParsingBattleFieldMovingInfo
	@brief : CBattleFieldMovingInfo 정보를 lua로 부터 읽어들임
	@param : IN KLuaManager& luaManager_
	@return : bool (성공, 실패)
*/
bool CBattleFieldData::ParsingBattleFieldMovingInfo( IN KLuaManager& luaManager_ )
{
	bool bResult = true;

	// 필드 내에 있는 포탈의 이동 정보
	if ( luaManager_.BeginTable( "BATTLE_FIELD_PORTAL_MOVING_INFO") )
	{
		int iTableIndex = 1;	// 테이블 인덱스
		while ( true == luaManager_.BeginTable( iTableIndex ) )
		{
			// BattleFieldPortalMovingInfo의 shared_ptr 생성
			CBattleFieldPortalMovingInfoPtr ptrBattleFieldPortalMovingInfo = CreateBattleFieldPortalMovingInfoPtr();

			// BattleFieldPortalMovingInfo가 생성되지 않았거나, 파싱 실패 했으면
			if ( ptrBattleFieldPortalMovingInfo == NULL ||
				false == ptrBattleFieldPortalMovingInfo->ParsingScriptFile( luaManager_ ) )
				bResult = false;
			else	// 성공적으로 수행 했으면 vector에 push 한다.
				m_vecBattleFieldPortalMovingInfoPtr.push_back( ptrBattleFieldPortalMovingInfo );

			luaManager_.EndTable();
			++iTableIndex;	// 테이블의 인덱스 증가
		}	
		luaManager_.EndTable();
	}
	else
	{
		ASSERT( !"BATTLE_FIELD_PORTAL_MOVING_INFO is Empty!" );
		StateLog( L"BATTLE_FIELD_PORTAL_MOVING_INFO is Empty!" );

		bResult = false;
	}

	return bResult;
}

/** @function : ParsingBattleFieldRiskInfo
	@brief : CBattleFieldRiskInfo 정보를 lua로 부터 읽어들임
	@param : IN KLuaManager& luaManager_
	@return : bool (성공, 실패)
*/
bool CBattleFieldData::ParsingBattleFieldRiskInfo( IN KLuaManager& luaManager_ )
{
	bool bResult = true;

	// 필드의 위험도 정보
	if ( luaManager_.BeginTable( "BATTLE_FIELD_RISK_INFO" ) )
	{
		int iTableIndex = 1;	// 테이블 인덱스
		while ( true == luaManager_.BeginTable( iTableIndex ) )
		{
			// BattleFieldRisk의 shared_ptr 생성
			CBattleFieldRiskInfoPtr ptrBattleFieldRiskInfo = CreateBattleFieldRiskInfoPtr();

			// BattleFieldRiskInfo가 생성되지 않았거나, 파싱 실패 했으면
			if ( ptrBattleFieldRiskInfo == NULL ||
				false == ptrBattleFieldRiskInfo->ParsingScriptFile( luaManager_ ) )
				bResult = false;
			else	// 성공적으로 수행 했으면 vector에 push 한다.
				m_vecBattleFieldRiskInfoPtr.push_back( ptrBattleFieldRiskInfo );

			luaManager_.EndTable();
			++iTableIndex;	// 테이블의 인덱스 증가
		}		

		luaManager_.EndTable();
	}

	return bResult;
}

CBattleFieldData::CBattleFieldPortalMovingInfoPtr CBattleFieldData::GetBattleFieldPortalMovingInfoPtrByLineMapIndex( const UINT uiLineMapIndex_ )
{
	BOOST_FOREACH( CBattleFieldPortalMovingInfoPtr val, m_vecBattleFieldPortalMovingInfoPtr )
	{
		if ( val->GetLineNumber() == uiLineMapIndex_ )
			return val;
	}

	return CBattleFieldPortalMovingInfoPtr();
}

#ifdef QUEST_GUIDE
bool CBattleFieldData::GetBattleFieldPortalMovingInfo( OUT map<UINT, UINT>& mapBattleFieldMovingInfo_)
{
	mapBattleFieldMovingInfo_.clear();
	BOOST_FOREACH( CBattleFieldPortalMovingInfoPtr val, m_vecBattleFieldPortalMovingInfoPtr )
	{
		UINT uiPlaceToMove = val->GetPlaceIdToMove();
		UINT uiLineMapIndex = val->GetLineNumber();
		mapBattleFieldMovingInfo_.insert( std::make_pair(uiPlaceToMove, uiLineMapIndex) );
	}
	return true;
}
#endif //QUEST_GUIDE



#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
void CBattleFieldData::ParsingBattleFieldMiddleBossInfo ( IN KLuaManager& luaManager_ )	// BATTLE_FIELD_MIDDLE_BOSS_INFO 파싱
{
	if ( true == luaManager_.BeginTable( "BATTLE_FIELD_MIDDLE_BOSS_INFO" ) )
	{
		int iTableIndex = 1;	// 테이블 인덱스
		while ( true == luaManager_.BeginTable( iTableIndex ) )
		{
			// BATTLE_FIELD_MIDDLE_BOSS_INFO 의 shared_ptr 생성
			CBattleFieldMiddleBossInfoPtr ptrBattleFieldMiddleBossInfo = CreateBattleFieldMiddleBossInfoPtr();

			// BATTLE_FIELD_MIDDLE_BOSS_INFO 가 생성되지 않았거나, 파싱 실패 했으면
			if ( ptrBattleFieldMiddleBossInfo != NULL &&
				true == ptrBattleFieldMiddleBossInfo->ParsingScriptFile( luaManager_ ) )
			{
				m_vecBattleFieldMiddleBossInfoPtr.push_back( ptrBattleFieldMiddleBossInfo );
			}
			luaManager_.EndTable();
			++iTableIndex;	// 테이블의 인덱스 증가
		}		
		luaManager_.EndTable();
	}
}

#endif // SERV_BATTLEFIELD_MIDDLE_BOSS


////////////////////////////////////////////////////////////////////////////////////////
// CX2BattleFieldManager
////////////////////////////////////////////////////////////////////////////////////////
/** @function : OpenScriptFile
	@brief : BattleField 스크립트 파싱
	@param : const char* szScriptFileName_
*/
void CX2BattleFieldManager::OpenScriptFile( const wchar_t* wszScriptFileName_ )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pBattleFieldManager", this );

    if ( g_pKTDXApp->LoadLuaTinker( wszScriptFileName_ ) == false )
    {
		ASSERT( !"DoMemory doesn't work!" );
		ErrorLogMsg( XEM_ERROR2, wszScriptFileName_ );
    }
}


/** @function : AddBattleFieldData_LUA
	@brief : BattleFieldData.lua의 AddBattleFieldData 수행
*/
void CX2BattleFieldManager::AddBattleFieldData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	// BattleFieldData의 shared_ptr 생성
	CBattleFieldDataPtr ptrBattleFieldData = CreateBattleFieldDataPtr();

	// 생성이 잘 되었으면
	if ( ptrBattleFieldData != NULL )
	{
		// 파싱에 성공했으면
		if ( ptrBattleFieldData->ParsingScriptFile( luaManager ) )
			m_mapBattleFieldDataPtr.insert( BattleFieldDataPtrPair( ptrBattleFieldData->GetBattleFieldId(), ptrBattleFieldData ) );
		else
		{
			// 파싱에 실패했으면
			ASSERT( !"BattleField Data is fault!" );
			StateLog( L"BattleField Data Is fault!" );
		}
	}
	else
	{
		// 생성이 안되었으면
		ASSERT( !"BattleField Data is NULL!" );
		StateLog( L"BattleField Data Is NULL!" );
	}
}

/** @function : ReOpenScriptFile
	@brief : 기존의 BattleField정보(m_mapBattleFieldDataPtr)을 clear 하고, 새로 읽어 들임
	@param : const char* szScriptFileName_
*/
void CX2BattleFieldManager::ReOpenScriptFile( const wchar_t* wszScriptFileName_ )
{
	m_mapBattleFieldDataPtr.clear();
	OpenScriptFile( wszScriptFileName_ );
}

/** @function : GetWorldIdByBattleFieldId
	@brief : 배틀필드 ID에 해당하는 WorldID를 리턴
	@param : UINT uiBattleFieldId_
	@return : UINT (WorldId)
*/
UINT CX2BattleFieldManager::GetWorldIdByBattleFieldId( UINT uiBattleFieldId_ ) const
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldId_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
			return mItrBattleFieldDataPtr->second->GetWorldId();
	}

	return 0;
}

/** @function : GetPortalMovingInfoByLineMapIndexInNowBattleField
	@brief : 인자로 받은 라인맵의 인덱스와 일치하는 PortalMovingInfo를 리턴
	@param : const int iLineMapIndex_ (라인맵 인덱스)
	@return : 
*/
CBattleFieldData::CBattleFieldPortalMovingInfoPtr CX2BattleFieldManager::GetPortalMovingInfoByLineMapIndexInNowBattleField( const int iLineMapIndex_ ) const
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		// 현재 배틀필드의 정보를 가지고 온다.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( GetBattleFieldIdWhereIam() );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			// 현재 배틀필드 정보에서
			return mItrBattleFieldDataPtr->second->GetBattleFieldPortalMovingInfoPtrByLineMapIndex( iLineMapIndex_ );
		}
	}

	return CBattleFieldData::CBattleFieldPortalMovingInfoPtr();
}
#ifdef QUEST_GUIDE
/** @function : GetBattleFieldPortalMovingInfo
	@brief : 현재 필드에서 이동 가능한 장소의 MapID와 연결된 곳의 LINE_NUMBER 얻어옴
	@param : map<UINT, UINT>& mapBattleFieldMovingInfo_ ( PLACE_ID_TO_MOVE, LINE_NUMBER )
	@return : bool(성공, 실패)
*/
bool CX2BattleFieldManager::GetBattleFieldPortalMovingInfo( OUT map<UINT, UINT>& mapBattleFieldMovingInfo_)
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		// 현재 배틀필드의 정보를 가지고 온다.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( GetBattleFieldIdWhereIam() );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			// 현재 배틀필드 정보에서
			mItrBattleFieldDataPtr->second->GetBattleFieldPortalMovingInfo( mapBattleFieldMovingInfo_ );
			return true;
		}
	}

	return false;
}
#endif //QUEST_GUIDE



void CX2BattleFieldManager::SetMonsterRespawnFactorByUserCount_LUA()
{
}

void CX2BattleFieldManager::SetMonsterRespawnTimeMinMax_LUA()
{
}

void CX2BattleFieldManager::SetBattleFieldFactor_LUA()
{

}

UINT CX2BattleFieldManager::GetReturnVillageId( UINT uiBattleFieldId_ /*= -1*/) const
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
#ifdef FIELD_BOSS_RAID // 김태환
		/// 레이드 필드일 경우, 입장 전 필드 아이디를 사용
		if( true == g_pData->GetBattleFieldManager().GetIsBossRaidFieldByFieldID( uiBattleFieldId_ ) )
		{
			uiBattleFieldId_ = CX2BossRaidManager::GetInstance()->GetBossRaidCreatorMapID();
		}
		else
#endif // FIELD_BOSS_RAID
		{
			//오현빈//2012-10-25//인자 비워서 호출하면 현재 필드의 ReturnVillageId 구하도록 변경
			if( -1 == uiBattleFieldId_ )
			{
				uiBattleFieldId_ = m_BattleFieldPositionInfo.m_uiBattleFieldIdWhereIam;
			}
		}

		// 현재 배틀필드의 정보를 가지고 온다.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldId_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			// 현재 배틀필드 정보에서

			return mItrBattleFieldDataPtr->second->GetReturnVillageId();
		}
	}

	return 0;
}

bool CX2BattleFieldManager::GetBattleFieldTextureNameAndKey( IN const UINT uiBattleFieldId_, OUT const WCHAR** pTextureName_, OUT const WCHAR** pKeyName_ )
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		// 현재 배틀필드의 정보를 가지고 온다.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldId_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			*pTextureName_ = mItrBattleFieldDataPtr->second->GetBattleFieldTextureName();
			ASSERT( NULL != pTextureName_ && NULL != pTextureName_[0] );

			*pKeyName_ = mItrBattleFieldDataPtr->second->GetBattleFieldTextureKey();
			ASSERT( NULL != pKeyName_ && NULL != pKeyName_[0] );
		}
		else
		{
			pTextureName_ = NULL;
			pKeyName_ = NULL;
			ASSERT( !L"BattleFieldData doesn't exist" );
		}
	}
	else
	{
		pTextureName_ = NULL;
		pKeyName_ = NULL;
	}

	return ( NULL != pTextureName_ && NULL != pKeyName_ );
}

#ifdef REFORM_QUEST
/** @function : GetBattleFieldNameByBattleFieldId
	@brief : 배틀필드 ID에 해당하는 필드이름을 리턴
	@param : UINT uiBattleFieldId_
	@return : wstring
*/
const WCHAR* CX2BattleFieldManager::GetBattleFieldNameByBattleFieldId( UINT uiBattleFieldId_ ) const
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldId_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
			return mItrBattleFieldDataPtr->second->GetBattleFieldName();
	}

	return 0;
}

#endif //REFORM_QUEST 


#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
wstring CX2BattleFieldManager::GetBattleFieldBossDataGroupName ( UINT uiBattleFieldId_, int iBattleFieldBossGroupId_ )
{
	if ( false == m_mapBattleFieldDataPtr.empty() )		// BattleFieldData 가 존재한다면
	{
		// FieldID 로 검색한다.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldId_ );
		
		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )	// FieldID 가 존재한다면
		{
			// Group 이름을 반환한다.
			return mItrBattleFieldDataPtr->second->GetBattleFieldMiddleBossInfoGroupName( iBattleFieldBossGroupId_ );			
		}
	}
	return L"NoData";
}

bool CBattleFieldMiddleBossInfo::ParsingScriptFile ( IN KLuaManager& luaManager_ )
{

	LUA_GET_VALUE( luaManager_, "SPAWN_ID", m_uiMonsterSpawnID, 0 );

	if ( 0 != m_uiMonsterSpawnID )
	{
		if( true == luaManager_.BeginTable( "SPAWN_GROUP_ID" ) )
		{
			m_usMonsterSpawnMany = 0;
			UINT uiMonsterSpawnID = 0;
			while ( true == luaManager_.GetValue ( m_usMonsterSpawnMany + 1, uiMonsterSpawnID  ) )
			{		
				m_vecMonsterSpawnGroupID.push_back(uiMonsterSpawnID);
				m_usMonsterSpawnMany += 1;
			}
			luaManager_.EndTable();
		}		
		LUA_GET_VALUE( luaManager_, "SET_BOSS_GROUP_NAME", m_iBossGroupStringIndex, STR_ID_EMPTY );
		if ( STR_ID_EMPTY != m_iBossGroupStringIndex )
			m_wstrMainMonsterName = GET_STRING( m_iBossGroupStringIndex );
		else
			m_wstrMainMonsterName = L"NoData";
		LUA_GET_VALUE( luaManager_, "SPAWN_RATE", m_usSpawnRate, 0 );
		LUA_GET_VALUE( luaManager_, "RISK_VALUE", m_usRiskValue, 0 );
		LUA_GET_VALUE( luaManager_, "SHOW_BOSS_NAME", m_bIsShowBossGage, 0 );	
		LUA_GET_VALUE( luaManager_, "BOSS_GAUGE_HP_LINES", m_bIsSplitBossGage, 0 );
		return true;
	}

	return false;
}



#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
bool CX2BattleFieldManager::GetIsBossRaidFieldByFieldID( const UINT uiBattleFieldID_ ) const
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		// 현재 배틀필드의 정보를 가지고 온다.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldID_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			return mItrBattleFieldDataPtr->second->GetIsBossRaidField();
		}
	}

	return false;
}
/** @function : GetIsBossRaidCurrentField
	@brief : 현재 필드가 보스 레이드 필드인지 검사
*/
bool CX2BattleFieldManager::GetIsBossRaidCurrentField() const
{
	if( CX2Main::XS_BATTLE_FIELD != g_pMain->GetNowStateID() )
		return false;

	return GetIsBossRaidFieldByFieldID( GetBattleFieldIdWhereIam() );
}
/** @function : GetBossFieldPortalLineByFieldID
	@brief : 보스 레이드 필드의 포탈이 열리는 라인맵 인덱스 얻기.
*/
const USHORT CX2BattleFieldManager::GetRaidFieldPortalLineByFieldID( const UINT uiBattleFieldID_ ) const
{	
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		// 현재 배틀필드의 정보를 가지고 온다.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldID_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			return mItrBattleFieldDataPtr->second->GetRaidFieldPortalLineIndex();
		}
	}

	return 0;
}
#endif // FIELD_BOSS_RAID
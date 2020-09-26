#include "StdAfx.h"
#include ".\ktdglinemap.h"


CKTDGLineMap::CKTDGLineMap() 
: m_RootLineAnimData( LAT_NONE )
, m_fLandHeight( 0.f )
, m_fStopTime( 0.f )
, m_fXMax( 0.f )
, m_fXMin( 0.f )
, m_fYMax( 0.f )
, m_fYMin( 0.f )
{
	m_pLineMapVB	= NULL;
	m_pKLuabinder	= NULL;

	m_LineList.reserve( 64 );
#ifdef  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
    m_vecRedTeamStartInfo.reserve( 8 );
    m_vecBlueTeamStartInfo.reserve( 8 );
#else   X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
	m_RedTeamStartPos.reserve( 8 );
	m_BlueTeamStartPos.reserve( 8 );
	m_RedTeamStartRight.reserve( 8 );
	m_BlueTeamStartRight.reserve( 8 );
	m_RedTeamStartLineIndex.reserve( 8 );
	m_BlueTeamStartLineIndex.reserve( 8 );
#endif  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
	m_vecLineGroupList.reserve( 32 );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP


	//{{ kimhc // 2010.7.22 // 몬스터가 생성한 라인맵
#ifdef	LINE_MAP_CREATED_BY_MONSTER
	SetNumofLineMapCreatedByMonster( 0 );	// 몬스터에 의해 생성된 라인맵의 개수
#endif	LINE_MAP_CREATED_BY_MONSTER
	//}} kimhc // 2010.7.22 // 몬스터가 생성한 라인맵

#ifdef WORLD_TRIGGER
	m_LineMapTrigger.m_vecNowTrigger.clear();
	m_LineMapTrigger.m_vecTriggerSeg.clear();
#endif

#ifdef SHOW_LINEMAP_FILE_NAME
	m_wstrLineFullName = L"";
#endif // SHOW_LINEMAP_FILE_NAME


	D3DXCreateLine( g_pKTDXApp->GetDevice(), &m_pLine );
	m_pLine->SetWidth( 3.f );
	m_pLine->SetAntialias( true );
}

CKTDGLineMap::~CKTDGLineMap(void)
{
	SAFE_RELEASE( m_pLine );
	SAFE_RELEASE( m_pLineMapVB );

	for( int i = 0; i < (int)m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		SAFE_DELETE( pLineData );
	}
	m_LineList.clear();

	//{{ kimhc // 2010-08-03 // WALL 타입의 라인맵 처리 변경
#ifdef	FIX_LINE_TYPE_WALL
	m_WallList.clear();
#else	FIX_LINE_TYPE_WALL
	for( int i = 0; i < (int)m_WallList.size(); i++ )
	{
		LineData* pLineData = m_WallList[i];
		SAFE_DELETE( pLineData );
	}
	m_WallList.clear();
#endif	FIX_LINE_TYPE_WALL
	//}} kimhc // 2010-08-03 // WALL 타입의 라인맵 처리 변경

	BOOST_TEST_FOREACH( LineAnimDataMap::value_type&, value, m_mapLineAnimData )
	{
		SAFE_DELETE( value.second );		
	}
	m_mapLineAnimData.clear();
	



#ifdef LINEMAP_RECT_TEST
	BOOST_TEST_FOREACH( RectData*, pRectData, m_RectList )
	{
		SAFE_DELETE( pRectData );
	}
#endif LINEMAP_RECT_TEST
#ifdef  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
    m_vecRedTeamStartInfo.clear();
    m_vecBlueTeamStartInfo.clear();
    m_mapStartInfo.clear();
#else   X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
	m_RedTeamStartPos.clear();
	m_BlueTeamStartPos.clear();
	m_mapStartPos.clear();
#endif	X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
#ifndef	X2OPTIMIZE_LINEMAP_LINEGROUP
	ReleaseLineGroup();
#endif	X2OPTIMIZE_LINEMAP_LINEGROUP

}

bool CKTDGLineMap::LoadLineMap( wstring pFileName )
{
#ifdef SHOW_LINEMAP_FILE_NAME
	WCHAR wszFullPath[MAX_PATH] = L"";
	wcscpy( wszFullPath, pFileName.c_str() );
	wstring wstrFullPath = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->GetPullPath( wszFullPath );
	CompulsorybyConvertFileAnsiToUTF8W( wstrFullPath );		
	m_wstrLineFullName = wstrFullPath;
#endif // SHOW_LINEMAP_FILE_NAME

#ifdef WORLD_TRIGGER
	m_LineMapTrigger.m_vecNowTrigger.clear();
	m_LineMapTrigger.m_vecTriggerSeg.clear();
#endif

	KLuabinder* pKRealLuaBinder = g_pKTDXApp->GetLuaBinder();

	if ( m_pKLuabinder != NULL )
		pKRealLuaBinder = m_pKLuabinder;

	lua_tinker::decl( pKRealLuaBinder->GetLuaState(), "g_pLineMap", this );


    if ( g_pKTDXApp->LoadAndDoMemory( pKRealLuaBinder, pFileName.c_str() ) == false )
    {
		ErrorLogMsg( KEM_ERROR20, pFileName.c_str() );
		return false;
    }


	ReBuildLineGroup();


#ifdef WORLD_TRIGGER		
	if( m_LineMapTrigger.m_vecTriggerSeg.size() > 2 )
		std::sort( m_LineMapTrigger.m_vecTriggerSeg.begin(), m_LineMapTrigger.m_vecTriggerSeg.end(), TriggerLess() );
#endif

	return true;
}




bool CKTDGLineMap::AddCameraData_LUA()
{

#ifdef NEW_VILLAGE_RENDERING_TEST
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	CameraData cameraData;


	bool bIsRelativePosition = true;

	LUA_GET_VALUE_RETURN( luaManager, "RELATIVE_EYE_POS_X", cameraData.m_vRelativeEye.x, 0.f, bIsRelativePosition = false; );
	LUA_GET_VALUE_RETURN( luaManager, "RELATIVE_EYE_POS_Y", cameraData.m_vRelativeEye.y, 0.f, bIsRelativePosition = false; );
	LUA_GET_VALUE_RETURN( luaManager, "RELATIVE_EYE_POS_Z", cameraData.m_vRelativeEye.z, 0.f, bIsRelativePosition = false; );

	if( true == bIsRelativePosition )
	{
		cameraData.m_eType = LCT_RELATIE_POSITION; 

		LUA_GET_VALUE( luaManager, "CAMERA_REPOSITION_SPEED", cameraData.m_fCameraRepositionSpeed, 100.f );
	}
	else
	{
		cameraData.m_eType = LCT_ABSOULTE_POSITION; 

		LUA_GET_VALUE( luaManager,		"FOCUS_UNIT",		cameraData.m_bFocusUnit,				true );

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "EYE_POS", cameraData.m_vEye, D3DXVECTOR3(0,0,0) );
		LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "LOOKAT_POS", cameraData.m_vLookAt, D3DXVECTOR3(0,0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		cameraData.m_vEye		= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "EYE_POS" );
		cameraData.m_vLookAt	= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "LOOKAT_POS" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	}

	m_vecCameraData.push_back( cameraData );

#endif NEW_VILLAGE_RENDERING_TEST

	return true;
}




bool CKTDGLineMap::AddLine_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	
	LineData*	pLineData = new LineData();

	LUA_GET_VALUE_ENUM( luaManager, "LINE_TYPE",		pLineData->lineType,			LINE_TYPE, LT_NORMAL	);
	LUA_GET_VALUE( luaManager,		"ENABLE",			pLineData->bEnable,				true					);
	LUA_GET_VALUE( luaManager,		"POTAL_GROUP",		pLineData->potalGroup,			-1						);
	LUA_GET_VALUE( luaManager,		"STOP_FACTOR",		pLineData->fStopFactor,			1.0f					);
	LUA_GET_VALUE_ENUM( luaManager, "DUST_TYPE",		pLineData->eDustType,			LINE_DUST_TYPE, LDT_DEFALT_DUST	);

	
	LUA_GET_VALUE( luaManager,		"BEFORE_LINE",		pLineData->beforeLine,			-1						);
	LUA_GET_VALUE( luaManager,		"NEXT_LINE",		pLineData->nextLine,			-1						);
	LUA_GET_VALUE( luaManager,		"CAN_DOWN",			pLineData->bCanDown,			true					);
	LUA_GET_VALUE( luaManager,		"LINE_SET",			pLineData->lineSet,				-1						);
	LUA_GET_VALUE( luaManager,		"SPEED",			pLineData->m_fSpeed,			0.0f					);

	LUA_GET_VALUE( luaManager,		"JUMP_SPEED_X",		pLineData->m_vJumpSpeed.x,		0.f						);
	LUA_GET_VALUE( luaManager,		"JUMP_SPEED_Y",		pLineData->m_vJumpSpeed.y,		0.f						);

	LUA_GET_VALUE( luaManager,		"TELEPORT_LINE",	pLineData->m_iTeleportLineIndex,			-1	);

#ifdef WALL_JUMP_TEST
	LUA_GET_VALUE( luaManager,		"ENABLE_WALL_JUMP",		pLineData->m_bEnableWallJump,				false			);
#endif WALL_JUMP_TEST

	LUA_GET_VALUE( luaManager,		"MONSTER_ALLOWED",		pLineData->m_bMonsterAllowed,				true			);
	LUA_GET_VALUE( luaManager,		"CAN_PASS",				pLineData->m_bCanPass,						true			);


	LUA_GET_VALUE( luaManager,		"CLIFF_HEIGHT",			pLineData->m_fCliffHeight,					0.f			);


	
#ifdef NEW_VILLAGE_RENDERING_TEST

	LUA_GET_VALUE( luaManager,		"CAMERA_INDEX",			pLineData->m_iCameraIndex,			-1	);

#endif NEW_VILLAGE_RENDERING_TEST

#ifdef OTHER_ROAD
	LUA_GET_VALUE( luaManager,		"OTHER_ROAD_UP",			pLineData->m_bOtherRoadUp,			false	);	
#endif


//#ifdef LINEMAP_SLOW_WIND_TEST
//	LUA_GET_VALUE( luaManager,		"WIND_SPEED_X",				pLineData->m_vWindSpeed.x,		0.f			);
//	LUA_GET_VALUE( luaManager,		"WIND_SPEED_Y",				pLineData->m_vWindSpeed.y,		0.f			);
//	LUA_GET_VALUE( luaManager,		"WIND_RANGE",				pLineData->m_fWindRange,		0.f			);
//#endif LINEMAP_SLOW_WIND_TEST


//#ifdef LINEMAP_FAST_WIND_TEST
//	LUA_GET_VALUE( luaManager,		"UPSIDE_WIND_ACCELARATION",		pLineData->m_fUpsideWindAccelaration,		0.f			);
//	LUA_GET_VALUE( luaManager,		"UPSIDE_WIND_RANGE",			pLineData->m_fUpsideWindRange,				0.f			);
//#endif LINEMAP_FAST_WIND_TEST

#ifdef WORLD_TRIGGER
	LUA_GET_VALUE( luaManager,		"TRIGGER_ID",				pLineData->m_iTriggerId,				-1			);	// Trigger Id
	LUA_GET_VALUE( luaManager,		"TRIGGER_ON_LINE",			pLineData->m_bFootOnLine,				true		);	// Trigger Id
#endif

	LUA_GET_VALUE( luaManager,		"IS_UNDERWATER",			pLineData->m_bUnderWater,				false		);	// 수중라인맵 여부
	LUA_GET_VALUE( luaManager,		"UNDERWATER_HEIGHT",		pLineData->m_fWaterHeight,				0.f			);	// 부력(y)
	LUA_GET_VALUE( luaManager,		"UNDERWATER_BUOYANCY",		pLineData->m_fBuoyancy,					0.6f		);	// 부력(y)
	LUA_GET_VALUE( luaManager,		"UNDERWATER_RESISTANCE",	pLineData->m_fResistance,				0.8f		);	// 저항(x,z)
	LUA_GET_VALUE( luaManager,		"UNDERWATER_ANIMSPEED",		pLineData->m_fAnimSpeed,				0.9f		);	// 속도
	LUA_GET_VALUE( luaManager,		"UNDERWATER_VARIANCEOXYEN",	pLineData->m_fVarianceOxyen,			-1.f		);	// 산소변동량

	float fR, fG, fB;
	LUA_GET_VALUE( luaManager,		"UNIT_COLOR_R",	fR,			0.f		);	
	LUA_GET_VALUE( luaManager,		"UNIT_COLOR_G",	fG,			0.f		);	
	LUA_GET_VALUE( luaManager,		"UNIT_COLOR_B",	fB,			0.f		);	
	pLineData->m_cLineColor	= D3DXCOLOR( fR, fG, fB, 0.f );

	LUA_GET_VALUE( luaManager,		"FORCE_CHANGE_COLOR",		pLineData->m_bForceChangeColor,				false		);	// 강제 색상변경 여부
	

	if( pLineData->m_bUnderWater == true )
	{
		pLineData->eDustType = LDT_WATER_SPLASH; 
	}

//{{ kimhc // 2010.7.6 // 몬스터가 생성한 라인맵
#ifdef	LINE_MAP_CREATED_BY_MONSTER
	LUA_GET_VALUE( luaManager,		"TEMP_LINE_MAP",				pLineData->m_bTempLineMap,		false			);

	if ( pLineData->m_bTempLineMap == true )
		SetNumofLineMapCreatedByMonster( GetNumofLineMapCreatedByMonster() + 1 );
#endif	LINE_MAP_CREATED_BY_MONSTER
//}} kimhc // 2010.7.6 // 몬스터가 생성한 라인맵

#ifdef ADD_LINEMAP_JUMPUP_DIRECTION
	LUA_GET_VALUE_ENUM( luaManager, "LINEMAP_DIRECTION_OPTION",		pLineData->m_eDirection, LINEMAP_DIRECTION_OPTION, LDO_NONE );
	LUA_GET_VALUE( luaManager,		"JUMP_REL_UP_KEYSTOP_TIME",		pLineData->m_fKeyStopTime,	0.f	);
#endif	


#ifdef DYNAMIC_PORTAL_LINE_MAP
	LUA_GET_VALUE_ENUM( luaManager,	"CHANGE_LINE_TYPE_AFTER_CLEAR_STAGE",		pLineData->m_eLineTypeAfterClearStage,	LINE_TYPE,	LT_INVALID	);
	LUA_GET_VALUE( luaManager,		"CHANGE_AFTER_CLEAR_STAGE",					pLineData->m_iChangeAfterClearStage,		-1 );	
	LUA_GET_VALUE( luaManager,		"CHANGE_AFTER_CLEAR_SUB_STAGE",				pLineData->m_iChangeAfterClearSubStage,		-1 );
#endif // DYNAMIC_PORTAL_LINE_MAP

#ifdef FIELD_BOSS_RAID
	LUA_GET_VALUE( luaManager,		"PORTAL_WHEN_ACTIVE_RAID",					pLineData->m_bIsPortalWhenActiveRaid,		false			);
#endif // FIELD_BOSS_RAID

#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
	LUA_GET_VALUE( luaManager,		"LINE_DEPTH",				pLineData->m_iLineDepth,		0			);

	if( luaManager.BeginTable( "LINKED_LINE" ) == true )
	{
		int iLinkedLineIndex = 0;
		int iValueIndex = 1;
		while( true == luaManager.GetValue( iValueIndex, iLinkedLineIndex ) )
		{
			if( iLinkedLineIndex >= 0 )
			{
				pLineData->m_vecLinkedLineIndex.push_back( iLinkedLineIndex );
			}
			iValueIndex++;
		}
		luaManager.EndTable(); // LINKED_LINE
	}
#else LINKED_LINEMAP_JUMP_MONSTER_TEST

#pragma DMLEE_NOTE( "본섭에 테스트 라인맵이 적용되지 않게 하기 위해서 line_depth 가 0이 아닌 라인은 로딩안되게 임시로 막음" )

	int iLineDepth = 0;
	LUA_GET_VALUE( luaManager,		"LINE_DEPTH",				iLineDepth,		0			);

	if( 0 != iLineDepth )
	{
		SAFE_DELETE( pLineData );
		return false; 
	}

#endif LINKED_LINEMAP_JUMP_MONSTER_TEST

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "START_POS", pLineData->startPos, D3DXVECTOR3(0,0,0) );
	LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "END_POS", pLineData->endPos, D3DXVECTOR3(0,0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	pLineData->startPos		= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(),  "START_POS" );
	pLineData->endPos		= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(),  "END_POS" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	
	ASSERT( pLineData->startPos != pLineData->endPos );
#ifdef NO_MORE_ERROR_TEST

	if( pLineData->startPos == pLineData->endPos )
	{
		InHouseLog( "ERROR: linemap에 startpos == endpos인 line이 포함되어있음" );
	}

#endif NO_MORE_ERROR_TEST

	CKTDGLineMap::MakeLine( pLineData );


	if( pLineData->lineType == LT_WALL )
	{
		m_WallList.push_back( pLineData );
	}
	//{{ kimhc // 2010-08-03 // WALL 타입의 라인맵 처리 변경
#ifdef	FIX_LINE_TYPE_WALL
	m_LineList.push_back( pLineData );
	pLineData->lineIndex = (int) m_LineList.size() - 1;

	if( LT_BUNGEE != pLineData->lineType && LT_WALL != pLineData->lineType )
	{
		if( 1 == m_LineList.size() )
		{
			m_fXMin = pLineData->startPos.x;
			m_fXMax = pLineData->endPos.x;
			m_fYMin = pLineData->startPos.y;
			m_fYMax = pLineData->endPos.y;
		}

		m_fXMin = min( pLineData->startPos.x, m_fXMin );
		m_fXMin = min( pLineData->endPos.x, m_fXMin );
		m_fYMin = min( pLineData->startPos.y, m_fYMin );
		m_fYMin = min( pLineData->endPos.y, m_fYMin );

		m_fXMax = max( pLineData->startPos.x, m_fXMax );
		m_fXMax = max( pLineData->endPos.x, m_fXMax );
		m_fYMax = max( pLineData->startPos.y, m_fYMax );
		m_fYMax = max( pLineData->endPos.y, m_fYMax );
	}


#else	FIX_LINE_TYPE_WALL
	else
	{
		m_LineList.push_back( pLineData );
		pLineData->lineIndex = (int) m_LineList.size() - 1;


		if( LT_BUNGEE != pLineData->lineType )
		{
			if( 1 == m_LineList.size() )
			{
				m_fXMin = pLineData->startPos.x;
				m_fXMax = pLineData->endPos.x;
				m_fYMin = pLineData->startPos.y;
				m_fYMax = pLineData->endPos.y;
			}

			m_fXMin = min( pLineData->startPos.x, m_fXMin );
			m_fXMin = min( pLineData->endPos.x, m_fXMin );
			m_fYMin = min( pLineData->startPos.y, m_fYMin );
			m_fYMin = min( pLineData->endPos.y, m_fYMin );

			m_fXMax = max( pLineData->startPos.x, m_fXMax );
			m_fXMax = max( pLineData->endPos.x, m_fXMax );
			m_fYMax = max( pLineData->startPos.y, m_fYMax );
			m_fYMax = max( pLineData->endPos.y, m_fYMax );
		}





	}
#endif	FIX_LINE_TYPE_WALL
	//}} kimhc // 2010-08-03 // WALL 타입의 라인맵 처리 변경
	
#if defined(WORLD_TOOL) || defined(X2TOOL)
	pLineData->m_vOriJumpSpeed = pLineData->m_vJumpSpeed;
	pLineData->m_fOriSpeed = pLineData->m_fSpeed;
	pLineData->m_iOriTeleportLineIndex = pLineData->m_iTeleportLineIndex;
	pLineData->m_iOriCameraIndex = pLineData->m_iCameraIndex;

	pLineData->m_bOriCanPass	= pLineData->m_bCanPass;
	pLineData->bOriEnable		= pLineData->bEnable;
#endif

	return true;
}

#ifdef X2TOOL
CKTDGLineMap::LineData* CKTDGLineMap::AddNewLine(LineData *pLineData)
{
	LineData*	pNewLineData = new LineData();
	*pNewLineData = *pLineData;

	m_LineList.push_back( pNewLineData );	
	pNewLineData->lineIndex = (int) m_LineList.size() - 1;
	

	return pNewLineData;
}
#endif







bool CKTDGLineMap::AddLineAnim_LUA()
{


	return false;
	
}





void CKTDGLineMap::BuildLineAnimDataTree()
{
	BOOST_TEST_FOREACH( LineAnimDataMap::value_type&, value, m_mapLineAnimData )
	{
		LineAnimData* pLineAnimData = value.second;
		if( pLineAnimData->m_iParentAnimDataIndex >= 0 )
		{
			LineAnimData* pParentLineAnimData = GetLineAnimData( pLineAnimData->m_iParentAnimDataIndex );
			if( NULL != pParentLineAnimData )
			{
				pParentLineAnimData->m_vecpChildAnimData.push_back( pLineAnimData );
				continue;
			}
		}

		m_RootLineAnimData.m_vecpChildAnimData.push_back( pLineAnimData );
	}

}
	




bool CKTDGLineMap::AssignLineAnimDataToLine( CKTDGLineMap::LineData* pLineData, int iLineAnimDataIndex )
{
	if( NULL == pLineData )
		return false; 

	if( iLineAnimDataIndex < 0 )
		return false; 


	LineAnimData* pAnimData = GetLineAnimData( iLineAnimDataIndex );
	if( NULL == pAnimData )
		return false;

	pLineData->m_pLineAnimData = pAnimData;
	pAnimData->m_pLineData = pLineData;


	switch( pAnimData->m_eAnimType )
	{
	case LAT_BACK_AND_FORTH:
	case LAT_MOVE_IF_STEPPED_ON:
		{
			if( D3DXVECTOR3(0,0,0) == pAnimData->m_vPivotPosition )
			{
				pAnimData->m_vPivotPosition = ( pLineData->startPos + pLineData->endPos ) * 0.5f;
			}
		} break;
	
	
	case LAT_VIKING:
		{
			LineAnimViking* pAnimDataViking = (LineAnimViking*) pAnimData;
			if( D3DXVECTOR3(0,0,0) == pAnimDataViking->m_vRotationAxis )
			{
				D3DXVec3Cross( &pAnimDataViking->m_vRotationAxis, &pLineData->dirVector, &Y_AXIS );
				D3DXVec3Normalize( &pAnimDataViking->m_vRotationAxis, &pAnimDataViking->m_vRotationAxis );
			}
		} break;


	case LAT_CIRCLE:
		{
			LineAnimCircle* pAnimDataCircle = (LineAnimCircle*) pAnimData;
			if( D3DXVECTOR3(0,0,0) == pAnimDataCircle->m_vRotationAxis )
			{
				D3DXVec3Cross( &pAnimDataCircle->m_vRotationAxis, &pLineData->dirVector, &Y_AXIS );
				D3DXVec3Normalize( &pAnimDataCircle->m_vRotationAxis, &pAnimDataCircle->m_vRotationAxis );
			}
		} break;

	}

	return true;
}






CKTDGLineMap::LineAnimData* CKTDGLineMap::GetLineAnimData( int iLineAnimDataIndex )
{
	LineAnimDataMap::iterator it = m_mapLineAnimData.find( iLineAnimDataIndex );	
	if( it != m_mapLineAnimData.end() )
	{
		return it->second;
	}

	return NULL;
}




#ifdef LINEMAP_RECT_TEST
	bool CKTDGLineMap::AddRect_LUA() 
	{
		KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

		RectData* pRectData = new RectData;

		LUA_GET_VALUE( luaManager,		"ENABLE",			pRectData->m_bEnable,				true					);
		
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "LEFT_TOP", pRectData->m_vLeftTop, D3DXVECTOR3(0,0,0) );
		LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "RIGHT_BOTTOM", pRectData->m_vRightBottom, D3DXVECTOR3(0,0,0) );
		LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "LEFT_BOTTOM", pRectData->m_vLeftBottom, D3DXVECTOR3(0,0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		pRectData->m_vLeftTop		= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(),  "LEFT_TOP" );
		pRectData->m_vRightBottom	= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(),  "RIGHT_BOTTOM" );
		pRectData->m_vLeftBottom	= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(),  "LEFT_BOTTOM" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		
		m_RectList.push_back( pRectData );

		return true;
	}


	bool CKTDGLineMap::IsInAnyRect( const D3DXVECTOR3& vPoint, D3DXVECTOR3* pNearestOutPoint /*= NULL*/ )
	{
		for( int i=0; i<(int) m_RectList.size(); i++ )
		{
			RectData* pRectData = m_RectList[i];
			if( false == pRectData->m_bEnable )
				continue;
			
			if( true == g_pKTDXApp->GetCollision()->IsPointInRect( vPoint, pRectData->m_vLeftTop, pRectData->m_vRightBottom, pRectData->m_vLeftBottom, 20.f, pNearestOutPoint ) )
			{
				return true;
			}
		}
		return false;
	}

#endif LINEMAP_RECT_TEST


	

/*virtual*/ HRESULT CKTDGLineMap::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();



#if defined(WORLD_TOOL) || defined(X2TOOL)
	UpdateLineVB();
#endif

	return S_OK;
}



//{{ robobeg : 2008-10-28
//virtual HRESULT CKTDGLineMap::OnFrameRender();
/*virtual*/
RENDER_HINT CKTDGLineMap::OnFrameRender_Prepare()
{
	if( m_pLineMapVB == NULL )
	{
		UpdateLineVB();
	}

    if ( m_pLineMapVB == NULL )
        return RENDER_HINT_NORENDER;

    return RENDER_HINT_DEFAULT;
}//CKTDGLineMap::OnFrameRender_Prepare()


/*virtual*/
void    CKTDGLineMap::OnFrameRender_Draw()
{
    if ( m_pLineMapVB == NULL )
        return;

    GetMatrix().UpdateWorldMatrix();

//{{ robobeg : 2008-10-24
	//g_pKTDXApp->SetNULLTexture( 0 ); 
    CKTDGStateManager::SetTexture( 0, NULL );
//}} robobeg : 2008-10-24
    KD3DPUSH( g_pKTDXApp->GetDGManager()->GetRSICKTDGLineMap() )

	HRESULT hr;

	if(FAILED(hr = g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_LINE_MAP_VERTEX )))
	{
		ErrorLog( KEM_ERROR21 );
//		return hr;
        return;
	}

	if(FAILED(hr = g_pKTDXApp->GetDevice()->SetStreamSource( 0, m_pLineMapVB, 0, sizeof(LINE_MAP_VERTEX) )))
	{
		ErrorLog( KEM_ERROR22 );
//		return hr;
        return;
	}

	//{{ kimhc // 2010.7.22 // 몬스터가 생성한 라인맵
#ifdef	LINE_MAP_CREATED_BY_MONSTER
	// 몬스터 라인맵은 m_LineMapVB 사이즈에 포함되지 않음으로 DrawPrimitive시에 빼줌
	if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_LINELIST, 0, (UINT)m_LineList.size() - GetNumofLineMapCreatedByMonster() )))
#else	LINE_MAP_CREATED_BY_MONSTER
	if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_LINELIST, 0, (UINT)m_LineList.size() )))
#endif	LINE_MAP_CREATED_BY_MONSTER
	//}} kimhc // 2010.7.22 // 몬스터가 생성한 라인맵	
	{
		ErrorLog( KEM_ERROR23 );
//		return hr;
        return;
	}
	
    KD3DEND()
}//CKTDGLineMap::OnFrameRender_Draw()
//}} robobeg : 2008-10-28





/*static*/ void CKTDGLineMap::MakeLine( LineData* pLineData )
{
	if( NULL == pLineData )
		return; 

	float temp;
	D3DXMATRIX matRotationOrth;
	D3DXMATRIX matRotation;

#ifdef	X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
    D3DXVec3Minimize( &pLineData->m_vPosMin, &pLineData->startPos, &pLineData->endPos );
    D3DXVec3Maximize( &pLineData->m_vPosMax, &pLineData->startPos, &pLineData->endPos );
#endif  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
	D3DXVec3Normalize( &pLineData->dirVector, &(pLineData->endPos - pLineData->startPos) );
	D3DXMatrixLookAtLH( &matRotation, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &pLineData->dirVector, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
	D3DXMatrixInverse( &matRotationOrth, NULL, &matRotation );

	temp = matRotationOrth._11;
	matRotationOrth._11 = matRotationOrth._13;
	matRotationOrth._13 = temp;

	temp = matRotationOrth._21;
	matRotationOrth._21 = matRotationOrth._23;
	matRotationOrth._23 = temp;

	temp = matRotationOrth._31;
	matRotationOrth._31 = matRotationOrth._33;
	matRotationOrth._33 = temp;

	if( matRotationOrth._12 == 0 && matRotationOrth._22 == 0 )
	{
		pLineData->dirRadian.z	= 0.0f;
		pLineData->dirRadian.y	= atan2f( -matRotationOrth._31, matRotationOrth._33 );
		pLineData->dirRadian.x	= atan2f( -matRotationOrth._21, matRotationOrth._11 );

		pLineData->dirDegree.z	= 0.0f;
		pLineData->dirDegree.y	= D3DXToDegree(pLineData->dirRadian.y);
		pLineData->dirDegree.x	= D3DXToDegree(pLineData->dirRadian.x);
	}
	else
	{
		pLineData->dirRadian.z	= asinf( matRotationOrth._32 );
		pLineData->dirRadian.y	= atan2f( -matRotationOrth._31, matRotationOrth._33 );
		pLineData->dirRadian.x	= 0.0f;//D3DXToDegree(atan2f( -matRotationOrth._12, matRotationOrth._22 ));

		pLineData->dirDegree.z	= D3DXToDegree(pLineData->dirRadian.z);
		pLineData->dirDegree.y	= D3DXToDegree(pLineData->dirRadian.y);
		pLineData->dirDegree.x	= 0.0f;
	}

	pLineData->fLength		= GetDistance( pLineData->startPos, pLineData->endPos );
}

bool CKTDGLineMap::MakeLineVB()
{
	HRESULT				hr;
	int					vbNum = (int)m_LineList.size() * 2 + (int)m_WallList.size() * 2;
	
#ifdef X2TOOL
	SAFE_RELEASE( m_pLineMapVB );
	m_pLineMapVB = NULL;
#endif

	if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( vbNum * sizeof(LINE_MAP_VERTEX), 
						D3DUSAGE_WRITEONLY, D3DFVF_LINE_MAP_VERTEX, 
						D3DPOOL_MANAGED, &m_pLineMapVB, NULL) ))
	{
		ErrorLog( KEM_ERROR24 );
		return false;
	}

	return true;
}


// @bOnlyAnimLine : 움직임이 있는 line만 갱신하기
bool CKTDGLineMap::UpdateLineVB( bool bOnlyAnimLine /*= false*/ )
{
	KTDXPROFILE();


	if( NULL == m_pLineMapVB )
	{
		MakeLineVB();
		bOnlyAnimLine = false;
	}

	HRESULT				hr;
	LINE_MAP_VERTEX*	pVertices = NULL;

	//{{ kimhc // 2010.7.22 // 몬스터가 생성한 라인맵
#ifdef	LINE_MAP_CREATED_BY_MONSTER
	int					vbNum = ( m_LineList.size() - GetNumofLineMapCreatedByMonster() ) * 2;
#else	LINE_MAP_CREATED_BY_MONSTER
	int					vbNum = (int)m_LineList.size() * 2;
#endif	LINE_MAP_CREATED_BY_MONSTER
	//}} kimhc // 2010.7.22 // 몬스터가 생성한 라인맵

    // D3DUSAGE_DYNAMIC으로 생성되지 않은 vertex buffer에 대해 D3DLOCK_DISCARD 사용 불가능
    // - jintaeks on 2008-10-15, 11:28
	if(FAILED(hr = m_pLineMapVB->Lock( 0, vbNum * sizeof(LINE_MAP_VERTEX), (void**) &pVertices, 0L/*D3DLOCK_DISCARD*/ )))
	{
		ErrorLog( KEM_ERROR25 );
		return false;
	}
	{
		int iSize = vbNum / 2;
		for( int i = 0; i < iSize; i++ )
		{
			const LineData* pLineData = m_LineList[i];

			if( true == bOnlyAnimLine &&
				NULL == pLineData->m_pLineAnimData )
				continue;

			pVertices[(i*2)].pos		= pLineData->startPos;
			pVertices[(i*2)+1].pos		= pLineData->endPos;

			if( pLineData->bEnable == false )
			{
				pVertices[(i*2)].color		= 0xff000000;
				pVertices[(i*2)+1].color	= 0xff000000;
			}
			else
			{
#if defined(WORLD_TOOL) || defined(X2TOOL)
				if( pLineData->m_bSelected == true )
				{
					if( pVertices[(i*2)].color == 0xffff0000 )
					{
						pVertices[(i*2)].color		= 0xff00ffff;
						pVertices[(i*2)+1].color	= 0xff00ffff;
					}
					else
					{
						pVertices[(i*2)].color		= 0xffff0000;
						pVertices[(i*2)+1].color	= 0xffff0000;
					}				

					continue;
				}
				else if( pLineData->m_bOver == true )
				{
					if( pVertices[(i*2)].color == 0xff00ffff )
					{
						pVertices[(i*2)].color		= 0xffffffff;
						pVertices[(i*2)+1].color	= 0xffffffff;
					}
					else
					{
						pVertices[(i*2)].color		= 0xff00ffff;
						pVertices[(i*2)+1].color	= 0xff00ffff;
					}				

					continue;
				}
#endif

				switch( pLineData->lineType )
				{
				default:
#ifdef OTHER_ROAD
				case LT_OTHER_ROAD:
#endif
				case LT_NORMAL:
					{                        
#if !defined(WORLD_TOOL) && !defined(X2TOOL)
                        pVertices[(i*2)].color		= 0xffff0000;
                        pVertices[(i*2)+1].color	= 0xffff0000;
#else
                        // 월드툴용
                        if(pLineData->lineSet % 2 == 0)
                        {
                            pVertices[(i*2)].color		= 0xffff0000;
                            pVertices[(i*2)+1].color	= 0xffff0000;
                        }
                        else
                        {
                            pVertices[(i*2)].color		= 0xff00ff00;
                            pVertices[(i*2)+1].color	= 0xff00ff00;
                        }
#endif						
					}
					break;

				case LT_WALL:
					{
						pVertices[(i*2)].color		= 0x000000;//0xff00ff00;
						pVertices[(i*2)+1].color	= 0x000000;//0xff00ff00;
					}
					break;

				case LT_POTAL:
					{
						pVertices[(i*2)].color		= 0xff00ffff;
						pVertices[(i*2)+1].color	= 0xff00ffff;
					}
					break;

				case LT_HOLE:
					{
						pVertices[(i*2)].color		= 0xff0000ff;
						pVertices[(i*2)+1].color	= 0xff0000ff;
					}
					break;

				case LT_JUMP_UP_REL:
					{
						pVertices[(i*2)].color		= 0xffAAAA00;
						pVertices[(i*2)+1].color	= 0xffAAAA00;
					} break;

				case LT_JUMP_UP_ABS:
					{
						pVertices[(i*2)].color		= 0xffAA00AA;
						pVertices[(i*2)+1].color	= 0xffAA00AA;
					} break;

				case LT_BUNGEE:
					{
						pVertices[(i*2)].color		= 0xffAAAAAA;
						pVertices[(i*2)+1].color	= 0xffAAAAAA;
					} break;

#ifdef MONSTER_ROAD
                case LT_MONSTER_ROAD:
                    {						
                        pVertices[(i*2)].color		= 0x00000000;
                        pVertices[(i*2)+1].color	= 0x00000000;
                    } break;
#endif

#ifdef UNIT_ROAD
                case LT_UNIT_ROAD:
                    {
                        pVertices[(i*2)].color		= 0xffff0000;
                        pVertices[(i*2)+1].color	= 0xffff0000;
                    } break;
#endif
				}				
			}			
		}


		int nLineVertexCount = vbNum;
		for( int i = 0; i < (int)m_WallList.size(); i++ )
		{
			const LineData* pLineData = m_WallList[i];

			if( true == bOnlyAnimLine &&
				NULL == pLineData->m_pLineAnimData )
				continue;

			pVertices[nLineVertexCount+(i*2)].pos		= pLineData->startPos;
			pVertices[nLineVertexCount+(i*2)+1].pos		= pLineData->endPos;

			if( pLineData->bEnable == false )
			{
				pVertices[nLineVertexCount+(i*2)].color		= 0xff000000;
				pVertices[nLineVertexCount+(i*2)+1].color	= 0xff000000;
			}
			else
			{
				switch( pLineData->lineType )
				{
				default:
				case LT_WALL:
					{
						pVertices[nLineVertexCount+(i*2)].color		= 0xff00ff00;
						pVertices[nLineVertexCount+(i*2)+1].color	= 0xff00ff00;
					}
					break;
				}				
			}			
		}

	}
	if(FAILED(hr = m_pLineMapVB->Unlock()))
	{
		ErrorLog( KEM_ERROR26 );
		return false;
	}
	return true;
}


D3DXVECTOR3 CKTDGLineMap::GetLandPosition( D3DXVECTOR3 pos, float fRadius, int* pLineIndex ) const
{
	KTDXPROFILE();
	D3DXVECTOR3 zeroPos, pointL1, pointL2, retVal;

    zeroPos		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pointL1		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pointL2		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	retVal		= D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
	int lineIndex = -1;
	const LineData* pLineData = NULL;

	for( int i = 0; i < (int)m_LineList.size(); i++ )
	{
		pLineData = m_LineList[i];
		if( false == pLineData->bEnable )
			continue;

//{{ kimhc // 2010.8.3 // WALL 타입의 라인맵 처리 변경
#ifdef	FIX_LINE_TYPE_WALL
		if ( LT_WALL == pLineData->lineType )
			continue;
#endif	FIX_LINE_TYPE_WALL
//}} kimhc // 2010.8.3 // WALL 타입의 라인맵 처리 변경
		
		if( pos.y + fRadius < pLineData->startPos.y && pos.y + fRadius < pLineData->endPos.y )
			continue;

		if( (pos.x < pLineData->startPos.x && pos.x < pLineData->endPos.x)
			|| (pos.x > pLineData->startPos.x && pos.x > pLineData->endPos.x) )
		{
			if( (pos.y < pLineData->startPos.y && pos.y < pLineData->endPos.y)
				|| (pos.y > pLineData->startPos.y && pos.y > pLineData->endPos.y) )
			{
				if( (pos.z < pLineData->startPos.z && pos.z < pLineData->endPos.z)
					|| (pos.z > pLineData->startPos.z && pos.z > pLineData->endPos.z) )
				{
					continue;
				}
			}
		}

		zeroPos		= pos;
		zeroPos.y	= 0.0f;
		if( g_pKTDXApp->GetCollision()->LineToLine( pos, zeroPos, pLineData->startPos, pLineData->endPos, fRadius, pointL1, pointL2 ) == true )
		{
			if( pointL2.y > retVal.y )
			{
				lineIndex	= i;
				retVal		= pointL2;
			}
		}
	}

	if( lineIndex == -1 )
	{
		if( pLineIndex != NULL )
			lineIndex = *pLineIndex;

		pLineData = GetLineData(lineIndex);
		if ( pLineData != NULL )
		{
			float fDistToStart = GetDistance( pos, pLineData->startPos );
			float fDistToEnd = GetDistance( pos, pLineData->endPos );

			if( fDistToStart > fDistToEnd )
			{
				retVal = pLineData->endPos;
			}
			else
			{
				retVal = pLineData->startPos;
			}
		}
	}
	if( lineIndex == -1 )
	{
		float nearPoint = 999999.0f;
		for( int i = 0; i < (int)m_LineList.size(); i++ )
		{
			pLineData = m_LineList[i];
			if( pLineData->bEnable == true )
			{
				float fDistToStart = GetDistance( pos, pLineData->startPos );
				float fDistToEnd = GetDistance( pos, pLineData->endPos );
				if( fDistToStart < nearPoint )
				{
					nearPoint = fDistToStart;
					lineIndex = i;
				}
				if( fDistToEnd < nearPoint )
				{
					nearPoint = fDistToEnd;
					lineIndex = i;
				}
			}			
		}
	}

	ASSERT( -1 != lineIndex );
    
	if( pLineIndex != NULL )
		*pLineIndex = lineIndex;

	return retVal;
}


bool CKTDGLineMap::CanDown( D3DXVECTOR3 pos, float fRadius, D3DXVECTOR3* pOutPos, int* pLineIndex, bool bDownRadius, bool bIsMonster, bool bOnLine )
{
	KTDXPROFILE();
	
	if( bDownRadius == true )
		pos.y -= fRadius + 1.0f;

	if( bOnLine == true )	// 현재 line을 밟고 있고, 그 line이 밑으로 못 내려가는 line이라면 
	{
		if( pLineIndex != NULL )
		{
			const LineData* pLine = m_LineList[*pLineIndex];
			if( pLine->bCanDown == false )
			{
				*pOutPos = pos;
				return false;
			}
		}
	}

	D3DXVECTOR3 zeroPos( pos.x, 0.0f, pos.z );

	D3DXVECTOR3 pointL1( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 pointL2( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 retVal( 0.0f, -1.0f, 0.0f );
	
	int line	= -1;
	bool touch	= false;



	for( int i = 0; i < (int)m_LineList.size(); i++ )
	{
		const LineData* pLine = m_LineList[i];
		if( false == pLine->bEnable )
			continue;

		switch ( pLine->lineType )
		{
		case LT_MONSTER_ROAD:
			{
				if( false == bIsMonster )
					continue;
			} break;

		case LT_UNIT_ROAD:
		case LT_JUMP_UP_REL:
		case LT_JUMP_UP_ABS:
			{
				if( true == bIsMonster )
					continue;
			} break;

		case LT_WALL:
			{
				continue;
			} break;

		default:
			break;
		}   

		const D3DXVECTOR3& vStartPos = pLine->startPos;
		const D3DXVECTOR3& vEndPos = pLine->endPos;

		if( pos.y < vStartPos.y && pos.y < vEndPos.y )
			continue;

		if( (pos.x < vStartPos.x && pos.x < vEndPos.x)
			|| (pos.x > vStartPos.x && pos.x > vEndPos.x) )
		{
			if( (pos.y < vStartPos.y && pos.y < vEndPos.y)
				|| (pos.y > vStartPos.y && pos.y > vEndPos.y) )
			{
				if( (pos.z < vStartPos.z && pos.z < vEndPos.z)
					|| (pos.z > vStartPos.z && pos.z > vEndPos.z) )
				{
					continue;
				}
			}
		}
		
		CKTDXCollision* pCollistion = g_pKTDXApp->GetCollision();

		if( pCollistion->LineToLine( pos, zeroPos, vStartPos, vEndPos, fRadius, pointL1, pointL2 ) == true )
		{	
			KTDXPROFILE_BEGIN( "CanDown()-GetDistance()" );

			if( pointL2.y >= retVal.y ) //충돌된 라인의 높이가 더 높으면
			{
				if( GetDistance3Sq( pointL2, vStartPos ) < 0.0001f )
				{
					if( pLine->beforeLine > -1 &&
						m_LineList[ pLine->beforeLine ]->bEnable == true ) //시작 지점에 연결라인이 있으면
					{
						//연결라인으로 보정한다
						line	= m_LineList[i]->beforeLine;
						pCollistion->LineToLine( pos, zeroPos, m_LineList[line]->startPos, m_LineList[line]->endPos, fRadius, pointL1, pointL2 );

						retVal	= pointL2;
					}
					else
					{
						//아직 터치되지 않았으면 보정한다
						if( touch == false )
						{
							retVal	= pointL2;
							line	= i;
						}
					}
				}
				else if( GetDistance3Sq( pointL2, vEndPos ) < 0.0001f )
				{
					if( pLine->nextLine > -1 &&
						m_LineList[ pLine->nextLine ]->bEnable == true ) //AI¾iAø ¼±AI AO´A°¡?
					{
						//AO´U¸e AI¾iAø ¶oAIA¸·I ¿A±a´U
						line	= m_LineList[i]->nextLine;
						pCollistion->LineToLine( pos, zeroPos, m_LineList[line]->startPos, m_LineList[line]->endPos, fRadius, pointL1, pointL2 );

						retVal	= pointL2;
					}
					else
					{
						//¾ø´U¸e A¹¹øA° AIA¡AI °æ¿i Au¿eCN´U.
						if( touch == false )
						{
							retVal	= pointL2;
							line	= i;
						}
					}
				}
				else
				{
					retVal	= pointL2;
					line	= i;
				}

			}
			else ////충돌된 라인의 높이가 더 낮으면
			{
				/// 비정상 라인맵 인덱스일 때, 구문 탈출
				if ( 0 > line || (int) m_LineList.size() <= line )
				{
					return false;
				}

				const LineData* pLineData = m_LineList[line];

				if ( NULL != pLineData )
				{
					if( GetDistance3Sq( pLineData->startPos, retVal ) < 0.0001f )
					{
						if( pLineData->beforeLine == -1 )
						{
							line	= i;
							retVal	= pointL2;						
						}
						else if( pLineData->beforeLine > -1 )
						{
							const LineData* pBeforeLine = m_LineList[ pLineData->beforeLine ];
							if( NULL != pBeforeLine && true == pBeforeLine->bEnable )
							{
								line	= pLineData->beforeLine;
								retVal	= pLineData->endPos;
							}
							else
							{
								line = i;
								retVal = pointL2;
							}
						}
					}
					else if( GetDistance3Sq( pLineData->endPos, retVal ) < 0.0001f )
					{
						if( pLineData->nextLine == -1 )
						{
							line	= i;
							retVal	= pointL2;
						}
						else if( pLineData->nextLine > -1)
						{
							const LineData* pNextLine = m_LineList[ pLineData->nextLine ];
							if( NULL != pNextLine && true == pNextLine->bEnable )
							{
								line	= pLineData->nextLine;
								retVal	= pLineData->startPos;
							}
							else
							{
								line = i;
								retVal = pointL2;
							}
						}					
					}
				}			
			}
			touch = true;

			KTDXPROFILE_END();
		}
	}

	if( touch == false && pLineIndex != NULL )
	{
		const LineData* pLineData = m_LineList[*pLineIndex];

		float dist1 = GetDistance3Sq( pLineData->startPos, pos );
		float dist2 = GetDistance3Sq( pLineData->endPos, pos );

		if( dist1 < dist2 )
		{
			if( pLineData->beforeLine > 0
				&& dist1 < fRadius )
			{
				retVal	= pLineData->startPos;
				line	= pLineData->beforeLine;
				touch	= true;
			}
		}
		else
		{
			if( pLineData->nextLine > 0
				&& dist2 < fRadius )
			{
				retVal	= pLineData->endPos;
				line	= pLineData->nextLine;
				touch	= true;
			}
		}
	}

	if( touch == true )
	{
		if( pOutPos != NULL )
			*pOutPos = retVal;
		if( pLineIndex != NULL )
			*pLineIndex = line;
	}

	if( pos.y < 0.0f )
		return false;

	return touch;
}




bool CKTDGLineMap::CanPassUp( const D3DXVECTOR3& pos )
{
	const D3DXVECTOR3 zeroPos( pos.x, pos.y + 50.0f, pos.z );
	//D3DXVECTOR3 zeroPos, pointL1, pointL2, retVal;
	//zeroPos		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 pointL1( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 pointL2( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 retVal( 0.0f, -1.0f, 0.0f );
	bool bFound = false;

	for( UINT i = 0; i < m_LineList.size(); i++ )
	{
		const LineData* pLine = m_LineList[i];
		if( false == pLine->bEnable )
			continue;

		if( true == pLine->m_bCanPass )
			continue;

		const D3DXVECTOR3& vStartPos = pLine->startPos;
		const D3DXVECTOR3& vEndPos = pLine->endPos;

		if( (pos.x < vStartPos.x && pos.x < vEndPos.x)
			|| (pos.x > vStartPos.x && pos.x > vEndPos.x) )
		{
			if( (pos.y < vStartPos.y && pos.y < vEndPos.y)
				|| (pos.y > vStartPos.y && pos.y > vEndPos.y) )
			{
				if( (pos.z < vStartPos.z && pos.z < vEndPos.z)
					|| (pos.z > vStartPos.z && pos.z > vEndPos.z) )
				{
					continue;
				}
			}
		}

		if( g_pKTDXApp->GetCollision()->LineToLine( zeroPos, pos, vStartPos, vEndPos, 50.f, pointL1, pointL2 ) == true )
		{	
			if ( pointL2.y > pos.y )
			{
				if ( false == bFound )
				{
					retVal = pointL2;
					bFound = true;
				}
				else if ( pointL2.y < retVal.y )
				{
					retVal = pointL2;
				}
			}
		}
	}

	if( true == bFound && GetDistance3Sq( pos, retVal ) < 2500.0f )	// 2500 = 50 * 50
	{
		return false;
	}

	return true;
}

#ifdef WALL_JUMP_TEST
	bool CKTDGLineMap::CrossedWall( D3DXVECTOR3& oldPos, D3DXVECTOR3& newPos, bool &bCanWallJump )
#else
	bool CKTDGLineMap::CrossedWall( D3DXVECTOR3& oldPos, D3DXVECTOR3& newPos )
#endif 
{
	bool bResult = false;

	D3DXVECTOR3 pointL1		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 pointL2		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	for( UINT i=0; i<m_WallList.size(); i++ )
	{
		const LineData* pLineData = m_WallList[i];
		if( pLineData->bEnable == true )
		{
			bResult = g_pKTDXApp->GetCollision()->LineToLine( oldPos, newPos, pLineData->startPos, pLineData->endPos, 1.f, pointL1, pointL2 );
			if( bResult == true )
			{
#ifdef WALL_JUMP_TEST
				bCanWallJump = pLineData->m_bEnableWallJump;
#endif WALL_JUMP_TEST
				break;
			}
		}
	}
	return bResult;
}

#ifdef X2TOOL
CKTDGLineMap::LineData* CKTDGLineMap::GetLineDataById( int index )
{
	KTDXPROFILE();
	for(UINT i=0; i<m_LineList.size(); ++i)
	{
		if( m_LineList[i]->lineIndex == index )
			return m_LineList[index]; 
	}
	return NULL;
}


#endif





#ifdef	X2OPTIMIZE_LINEMAP_LINEGROUP

CKTDGLineMap::LineGroupID   CKTDGLineMap::GetLineGroupIncludesLineData( int iLineIndex ) const
{
	KTDXPROFILE();

	if ( false == ( iLineIndex >= 0 && iLineIndex < (int) m_LineList.size() ) )
		return LineGroupID();

    const LineData* pLineData = m_LineList[ iLineIndex ];
    ASSERT( pLineData->bEnable == true && pLineData->m_lineGroupID.IsValid() == true
        || pLineData->bEnable == false && pLineData->m_lineGroupID.IsValid() == false );
    return  pLineData->m_lineGroupID;
}


void CKTDGLineMap::DisableLines( const std::vector<int>& vecLineIndices )
{
	KTDXPROFILE();

	if ( vecLineIndices.empty() == true )
		return;

    bool    bModified = false;
   	int	iNumLineData = (int) m_LineList.size();
    BOOST_FOREACH( int i,  vecLineIndices )
	{
		if ( i >= 0 && i < iNumLineData )
        {
            LineData* pLineData = m_LineList[i];
            if ( pLineData->bEnable == true )
            {
                pLineData->bEnable = false;
                bModified = true;
            }
        }
	}

    if ( bModified == true )
    {
	    ReBuildLineGroup();
    }
}//CKTDGLineMap::DisableLines()

void CKTDGLineMap::EnableLineData( int iIndex, bool bEnable )
{
    if ( iIndex < 0 || iIndex >= (int) m_LineList.size() )
        return;
    LineData* pLineData = m_LineList[iIndex];
    if ( pLineData->bEnable == bEnable )
        return;
    pLineData->bEnable = bEnable;
    ReBuildLineGroup();
}


#else	X2OPTIMIZE_LINEMAP_LINEGROUP


void CKTDGLineMap::ReleaseLineGroup()
{
	KTDXPROFILE();
	for( unsigned i=0; i<m_vecLineGroupList.size(); i++ )
	{
		SAFE_DELETE( m_vecLineGroupList[i] );
	}

	m_vecLineGroupList.resize( 0 );
	m_mapLineData2LineGroup.clear();
}


CKTDGLineMap::LineGroup* CKTDGLineMap::GetLineGroupIncludesLineData( const LineData* pLineData )
{
	KTDXPROFILE();
	if( NULL == pLineData )
		return NULL;

	map<const LineData*, LineGroup*>::iterator it;
	it = m_mapLineData2LineGroup.find( pLineData );
	if( it != m_mapLineData2LineGroup.end() )
		return it->second;
	else
		return NULL;
}

// fix!! 이 코드 왠지 계산시간 많이 걸릴것 같은데...
// linegroup로 vector가 아닌 map에 저장해야? 
void CKTDGLineMap::EnableLineData( CKTDGLineMap::LineData* pLineData, bool bEnable )
{
	KTDXPROFILE();


	if( pLineData == NULL || pLineData->bEnable == bEnable )
		return;

	pLineData->bEnable = bEnable;

	LineData* pBeforeLine = _AccessLineData( pLineData->beforeLine );
	LineData* pNextLine = _AccessLineData( pLineData->nextLine );

	vector<LineGroup*>::iterator it;
	if( true == bEnable ) // disable --> enable 
	{
		if( NULL != pBeforeLine && NULL != pNextLine )
		{
			LineGroup* pBeforeLineGroup = m_mapLineData2LineGroup[ pBeforeLine ];
			LineGroup* pNextLineGroup = m_mapLineData2LineGroup[ pNextLine ];

			if( pBeforeLineGroup->vecpLineData.size() < pNextLineGroup->vecpLineData.size() )
			{
				LineGroup* pTemp = pBeforeLineGroup;
				pBeforeLineGroup = pNextLineGroup;
				pNextLineGroup = pTemp;				
			}

			// 두 line group을 합한다
			pBeforeLineGroup->vecpLineData.push_back( pLineData );
			m_mapLineData2LineGroup[ pLineData ] = pBeforeLineGroup;

			pBeforeLineGroup->vecpLineData.insert( pBeforeLineGroup->vecpLineData.begin(), 
				pNextLineGroup->vecpLineData.begin(), pNextLineGroup->vecpLineData.end() );
			for( unsigned i=0; i<pNextLineGroup->vecpLineData.size(); i++ )
			{
				m_mapLineData2LineGroup[ pNextLineGroup->vecpLineData[i] ] = pBeforeLineGroup;
			}

			for( it = m_vecLineGroupList.begin(); it != m_vecLineGroupList.end() ; it++ )
			{
				if( *it == pNextLineGroup )
				{
					it = m_vecLineGroupList.erase(it);
					break;						
				}
			}
			SAFE_DELETE( pNextLineGroup );
		}
		else if( NULL != pBeforeLine )
		{
			LineGroup* pBeforeLineGroup = m_mapLineData2LineGroup[ pBeforeLine ];

			pBeforeLineGroup->vecpLineData.push_back( pLineData );
			m_mapLineData2LineGroup[ pLineData ] = pBeforeLineGroup;
		}
		else if( NULL != pNextLine )
		{
			LineGroup* pNextLineGroup = m_mapLineData2LineGroup[ pBeforeLine ];

			pNextLineGroup->vecpLineData.push_back( pLineData );
			m_mapLineData2LineGroup[ pLineData ] = pNextLineGroup;			
		}
		else
		{
			LineGroup* pNewLineGroup = new LineGroup;
			pNewLineGroup->vecpLineData.push_back( pLineData );
			m_vecLineGroupList.push_back( pNewLineGroup );
			m_mapLineData2LineGroup[ pLineData ] = pNewLineGroup;
		}
	}
	else // enable --> disable 
	{
		if( NULL != pBeforeLine && NULL != pNextLine )
		{
			LineGroup* pCurrLineGroup = m_mapLineData2LineGroup[ pLineData ];
			m_mapLineData2LineGroup[ pLineData ] = NULL;

			// 2개의 linegroup으로 분리
			LineData* pTempLine = NULL;
			
			LineGroup* pBeforeLineGroup = new LineGroup;
			LineGroup* pNextLineGroup = new LineGroup;
			while( NULL != pBeforeLine )
			{
				pBeforeLineGroup->vecpLineData.push_back( pBeforeLine );
				m_mapLineData2LineGroup[ pBeforeLine ] = pBeforeLineGroup;
				pBeforeLine = _AccessLineData( pBeforeLine->beforeLine );
			}
			
			while( NULL != pNextLine )
			{
				pNextLineGroup->vecpLineData.push_back( pNextLine );
				m_mapLineData2LineGroup[ pNextLine ] = pNextLineGroup;
				pNextLine = _AccessLineData( pNextLine->nextLine );
			}			

			for( it = m_vecLineGroupList.begin(); it != m_vecLineGroupList.end() ; it++ )
			{
				if( *it == pCurrLineGroup )
				{
					it = m_vecLineGroupList.erase(it);
					break;						
				}
			}
			SAFE_DELETE( pCurrLineGroup );

			m_vecLineGroupList.push_back( pBeforeLineGroup );
			m_vecLineGroupList.push_back( pNextLineGroup );
		}
		else if( NULL != pBeforeLine )
		{
			m_mapLineData2LineGroup[ pLineData ] = NULL;
		}
		else if( NULL != pNextLine )
		{
			m_mapLineData2LineGroup[ pLineData ] = NULL;
		}
		else 
		{
			LineGroup* pCurrLineGroup = m_mapLineData2LineGroup[ pLineData ];
			for( it = m_vecLineGroupList.begin(); it != m_vecLineGroupList.end() ; it++ )
			{
				if( *it == pCurrLineGroup )
				{
					it = m_vecLineGroupList.erase(it);
					break;						
				}
			}
			SAFE_DELETE( pCurrLineGroup );
			m_mapLineData2LineGroup[ pLineData ] = NULL;
		}
	}
}

#endif	X2OPTIMIZE_LINEMAP_LINEGROUP

void CKTDGLineMap::EnableLineSet( int iLineSetIndex, bool bEnable )	
{	
	KTDXPROFILE();

	if( iLineSetIndex < 0 )
		return;

    bool    bModified = false;

	for( unsigned int i = 0; i < m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		if( iLineSetIndex == pLineData->lineSet )
		{
            if ( pLineData->bEnable != bEnable )
            {
			    pLineData->bEnable = bEnable;
                bModified = true;
            }
		}
	}

    if ( bModified == true )
    {
	    ReBuildLineGroup();
    }

}

void CKTDGLineMap::DisableAllLineData()
{
	for( int i = 0 ; i < (int) m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		pLineData->bEnable = false;
#ifdef	X2OPTIMIZE_LINEMAP_LINEGROUP
		pLineData->m_lineGroupID.Init();
#endif	X2OPTIMIZE_LINEMAP_LINEGROUP
	}
#ifndef	X2OPTIMIZE_LINEMAP_LINEGROUP
	ReBuildLineGroup();
#endif	X2OPTIMIZE_LINEMAP_LINEGROUP	
}

#ifdef	X2OPTIMIZE_LINEMAP_LINEGROUP

void CKTDGLineMap::ReBuildLineGroup()
{
	KTDXPROFILE();

	int nNodes = m_LineList.size();
	if( nNodes <= 0 )
		return;

    int*  piLineMembers = (int*) _alloca( nNodes * sizeof(int) );
	bool* bDoneLineData = (bool*) _alloca( nNodes * sizeof(bool) );
    ASSERT( bDoneLineData != NULL );
    int i;
	for( i=0; i<nNodes; i++ )
    {
		bDoneLineData[i] = false;
        m_LineList[i]->m_lineGroupID.Init();
    }//for

    int iNumMembers;
    LineGroupID id;

    const LineData* pLineData = NULL;

	// line group 생성
	for( i = 0; i <nNodes; i++ )
	{
        const LineData* pLineData = m_LineList[i];
		if ( pLineData->bEnable == false )
		    continue;
		if( bDoneLineData[i] == true )
			continue;

        piLineMembers[ 0 ] = i;
        iNumMembers = 1;
        id.m_iStartLineIndex = i;
        id.m_iEndLineIndex = i;

		const LineData* pTempLineData = pLineData;
		int		  iTempLineIndex = i;
        int       iNextLine, iBeforeLine;
		while( true )
		{			
            iNextLine = pTempLineData->nextLine;
			if( iNextLine >= 0 
                && iNextLine < nNodes
                && false == bDoneLineData[iNextLine] )
			{
			    const LineData* pNextLineData = m_LineList[iNextLine];
				if ( pNextLineData->bEnable == true )
				{	
					pTempLineData = pNextLineData;
	                if ( pTempLineData->beforeLine != iTempLineIndex )
    	                break;
					bDoneLineData[iNextLine] = true;
	                id.m_iEndLineIndex = iNextLine;
	                piLineMembers[ iNumMembers++ ] = iNextLine;
	                iTempLineIndex = iNextLine;
				}
				else
				{
					break;
				}				
			}
			else
			{
				break;
			}
		}

		pTempLineData = pLineData;
		iTempLineIndex = i;
		while( true )
		{			
            iBeforeLine = pTempLineData->beforeLine;
			if( iBeforeLine >= 0 
                && iBeforeLine < nNodes
                && false == bDoneLineData[iBeforeLine] )
			{
			    const LineData* pBeforeLineData = m_LineList[iBeforeLine];			
				if ( pBeforeLineData->bEnable == true )
				{				
					pTempLineData = pBeforeLineData;
	                if ( pTempLineData->nextLine != iTempLineIndex )
	                    break;
					bDoneLineData[iBeforeLine] = true;
	                id.m_iStartLineIndex = iBeforeLine;
	                piLineMembers[ iNumMembers++ ] = iBeforeLine;
	                iTempLineIndex = iBeforeLine;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}				
		}

        for( int i = 0; i < iNumMembers; i++ )
        {
            m_LineList[ piLineMembers[ i ] ]->m_lineGroupID = id;
        }//for
	}
}

#else	X2OPTIMIZE_LINEMAP_LINEGROUP

void CKTDGLineMap::ReBuildLineGroup()
{
	KTDXPROFILE();

	CKTDGLineMap* pLineMap = this;

	if( NULL == pLineMap )
		return;

	int nNodes = pLineMap->GetNumLineData();
	if( nNodes <= 0 )
		return;

	bool* bDoneLineData = new bool[nNodes];
	for( int i=0; i<nNodes; i++ )
		bDoneLineData[i] = false;

	ReleaseLineGroup();

	// line group 생성
	for( int i=0; i<nNodes; i++ )
	{
		if( bDoneLineData[i] == true )
			continue;

		const LineData* pLineData = pLineMap->GetLineData(i);
        if( false == pLineData->bEnable )		
		{
			continue;
		}

		bDoneLineData[i] = true;
		LineGroup* pGroup = new LineGroup();		
		pGroup->vecpLineData.push_back( pLineData );
		pGroup->vStartPos = pLineData->startPos;
		pGroup->vEndPos = pLineData->endPos;

		const LineData* pTempLineData = pLineData;
		while( true )
		{			
			if( pTempLineData->nextLine >= 0 && false == bDoneLineData[pTempLineData->nextLine] &&
				pTempLineData->bEnable == true )
			{	
				bDoneLineData[pTempLineData->nextLine] = true;
				pTempLineData = pLineMap->GetLineData( pTempLineData->nextLine );
				pGroup->vecpLineData.push_back( pTempLineData );
				pGroup->vEndPos = pTempLineData->endPos;
			}
			else
			{
				break;
			}				
		}

		pTempLineData = pLineData;
		while( true )
		{			
			if( pTempLineData->beforeLine >= 0 && false == bDoneLineData[pTempLineData->beforeLine] &&
				pTempLineData->bEnable == true )
			{						
				bDoneLineData[pTempLineData->beforeLine] = true;
				pTempLineData = pLineMap->GetLineData( pTempLineData->beforeLine );
				pGroup->vecpLineData.push_back( pTempLineData );
				pGroup->vStartPos = pTempLineData->startPos;
			}
			else
			{
				break;
			}				
		}

		m_vecLineGroupList.push_back( pGroup );
	}

	m_mapLineData2LineGroup.clear();
	int nLineGroups = (int)m_vecLineGroupList.size(); 
	for( int i=0; i<nLineGroups; i++ )
	{
		LineGroup* pGroup = m_vecLineGroupList.at(i);
		int nLineDataInANode = (int)pGroup->vecpLineData.size();

		for(int j=0; j<nLineDataInANode; j++ )
		{					
			const LineData* pLineData = pGroup->vecpLineData.at(j);
			m_mapLineData2LineGroup.insert( std::make_pair(pLineData, pGroup) );
		}
	}

	SAFE_DELETE_ARRAY(bDoneLineData);			
}

#endif	X2OPTIMIZE_LINEMAP_LINEGROUP

//{{oasis:김상윤////2009-10-7////
D3DXVECTOR3 CKTDGLineMap::GetFixedDistancePosition( D3DXVECTOR3* pvPos, float fDist, bool bIsRight )
{
	int iLineIndex = -1;

	const float MAGIC_LINE_RADIUS = 30.f;

	D3DXVECTOR3 vPos = GetLandPosition( *pvPos, MAGIC_LINE_RADIUS, &iLineIndex );
	if( -1 == iLineIndex )
		return *pvPos;	// 아마 error

	const LineData* pLineData = GetLineData( iLineIndex );
#ifndef	X2OPTIMIZE_LINEMAP_LINEGROUP	
	LineGroup* pLineGroup = this->GetLineGroupIncludesLineData( pLineData );
#endif	X2OPTIMIZE_LINEMAP_LINEGROUP	

	if( false == bIsRight ) // 왼쪽으로
	{
		float fWalkDist = GetDistance( *pvPos, pLineData->startPos );
		const LineData* pTempLineData = pLineData;
		while( pTempLineData->beforeLine >= 0 && fWalkDist < fDist )
		{
			pTempLineData = this->GetLineData( pTempLineData->beforeLine );					
			fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
		}

		if( fWalkDist < fDist + 15.f ) // 라인 그룹 끝에 도달한 경우
		{
			D3DXVECTOR3 vResultPos = 0.8f * pTempLineData->startPos + 0.2f * pTempLineData->endPos;
			return vResultPos;
		}
		else
		{
			float fLerp = (fWalkDist - fDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
			D3DXVECTOR3 vResultPos = (1.f - fLerp) * pTempLineData->startPos + fLerp * pTempLineData->endPos;
			return vResultPos;
		}
	}
	else // 오른쪽으로
	{
		float fWalkDist = GetDistance( *pvPos, pLineData->endPos );
		const LineData* pTempLineData = pLineData;
		while( pTempLineData->nextLine >= 0 && fWalkDist < fDist )
		{
			pTempLineData = this->GetLineData( pTempLineData->nextLine );					
			fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
		}

		if( fWalkDist < fDist + 15.f )
		{
			D3DXVECTOR3 vResultPos = 0.2f * pTempLineData->startPos + 0.8f * pTempLineData->endPos;
			return vResultPos;
		}
		else
		{
			float fLerp = (fWalkDist - fDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
			D3DXVECTOR3 vResultPos = fLerp * pTempLineData->startPos + (1.f - fLerp) * pTempLineData->endPos;
			return vResultPos;
		}		
	}
	// 여기까지 오면 에러 
	return D3DXVECTOR3(0,0,0);
}
//}}oasis:김상윤////2009-10-7////


#ifdef RAVEN_WEAPON_TAKER
bool CKTDGLineMap::GetFixedDistancePositionForWeaponTaker( const D3DXVECTOR3* pvPos, float fDist, bool bIsRight, OUT D3DXVECTOR3& vResultPos )
{
	int iLineIndex = -1;

	const float MAGIC_LINE_RADIUS = 30.f;

	D3DXVECTOR3 vPos = GetLandPosition( *pvPos, MAGIC_LINE_RADIUS, &iLineIndex );
	if( -1 == iLineIndex )
		return false;	// 아마 error

	const LineData* pLineData = GetLineData( iLineIndex );
#ifndef	X2OPTIMIZE_LINEMAP_LINEGROUP		
	LineGroup* pLineGroup = this->GetLineGroupIncludesLineData( pLineData );
#endif	X2OPTIMIZE_LINEMAP_LINEGROUP		

	if( false == bIsRight ) // 왼쪽으로
	{
		float fWalkDist = GetDistance( *pvPos, pLineData->startPos );
		const LineData* pTempLineData = pLineData;
		while( pTempLineData->beforeLine >= 0 && fWalkDist < fDist )
		{
			pTempLineData = this->GetLineData( pTempLineData->beforeLine );					
			fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
		}

		if( fWalkDist < fDist + 15.f ) // 라인 그룹 끝에 도달한 경우
		{
			vResultPos = pTempLineData->startPos;						
			return false;
		}
		else
		{
			float fLerp = (fWalkDist - fDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
			vResultPos = (1.f - fLerp) * pTempLineData->startPos + fLerp * pTempLineData->endPos;
			return true;
		}
	}
	else // 오른쪽으로
	{
		float fWalkDist = GetDistance( *pvPos, pLineData->endPos );
		const LineData* pTempLineData = pLineData;
		while( pTempLineData->nextLine >= 0 && fWalkDist < fDist )
		{
			pTempLineData = this->GetLineData( pTempLineData->nextLine );					
			fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
		}

		if( fWalkDist < fDist + 15.f )
		{
			vResultPos = pTempLineData->endPos;
			return false;
		}
		else
		{
			float fLerp = (fWalkDist - fDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
			vResultPos = fLerp * pTempLineData->startPos + (1.f - fLerp) * pTempLineData->endPos;
			return true;
		}		
	}
	// 여기까지 오면 에러 
	return false;
}
#endif RAVEN_WEAPON_TAKER



// fix!! getlandposition이 실패하는 경우가 많을 수 있다
D3DXVECTOR3 CKTDGLineMap::GetRandomPosition( const D3DXVECTOR3* pvPos, float fRange, bool bOnThisLineGroup )
{
	int iLineIndex = -1;

	const float MAGIC_LINE_RADIUS = 30.f;

	if( NULL == pvPos )
	{
        int iNumLineData = GetNumLineData();
		int iRandLineIndex = rand() % iNumLineData;
#ifdef  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
        const LineData* pLineData = NULL;
        int  iTempRandIndex = 0;
		for( int i = 0; i < iNumLineData; i++ )
		{
            iTempRandIndex = iRandLineIndex + i;
            if ( iTempRandIndex >= iNumLineData )
                iTempRandIndex -= iNumLineData;
			pLineData = GetLineData( iTempRandIndex );
            if(pLineData != NULL && pLineData->bEnable == true && pLineData->lineType == LT_NORMAL)
                break;
		}        
        if ( pLineData != NULL )
        {
		    float fLerpCoeff = (float)(rand()%10001) / 10000.f; 		
		    D3DXVECTOR3 vResultPos = (1.f-fLerpCoeff) * pLineData->startPos + fLerpCoeff * pLineData->endPos;		
    		return vResultPos;	
        }//if
#else   X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
		const LineData* pLineData = GetLineData( iRandLineIndex );
		if( pLineData == NULL || pLineData->bEnable == false || pLineData->lineType != LT_NORMAL)
		{
			int iTempRandIndex = iRandLineIndex+1;
			for( ; iTempRandIndex != iRandLineIndex; iTempRandIndex++ )
			{
				if( iTempRandIndex > GetNumLineData()-1 )
				{
					iTempRandIndex = 0;
				}

				pLineData = GetLineData( iTempRandIndex );
                
                if(pLineData != NULL && pLineData->bEnable == true && pLineData->lineType == LT_NORMAL)
                    break;
			}
		}
		float fLerpCoeff = (float)(rand()%10001) / 10000.f; 		
		D3DXVECTOR3 vResultPos = (1.f-fLerpCoeff) * pLineData->startPos + fLerpCoeff * pLineData->endPos;		
    	return vResultPos;	
#endif  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
	}
	else
	{
		D3DXVECTOR3 vPos = GetLandPosition( *pvPos, MAGIC_LINE_RADIUS, &iLineIndex );
		if( -1 == iLineIndex )
			return *pvPos;	// 아마 error

		const LineData* pLineData = GetLineData( iLineIndex );
#ifndef	X2OPTIMIZE_LINEMAP_LINEGROUP
		LineGroup* pLineGroup = this->GetLineGroupIncludesLineData( pLineData );
#endif	X2OPTIMIZE_LINEMAP_LINEGROUP		

		if( false == bOnThisLineGroup )
		{
			int iResultLineIndex = -1;
			D3DXVECTOR3 vResultPos;
			bool bDone = false;
			while( false == bDone )
			{
				float fRandX = (float)(rand()%10001 - 5000) / 5000.f; 
				float fRandY = (float)(rand()%10001 - 5000) / 5000.f; 
				float fRandZ = (float)(rand()%10001 - 5000) / 5000.f;
				D3DXVECTOR3 vDir = D3DXVECTOR3( fRandX, fRandY, fRandZ );
				D3DXVec3Normalize( &vDir, &vDir );
				vResultPos = vPos + vDir * fRange;
				vResultPos = GetLandPosition( vResultPos, MAGIC_LINE_RADIUS, &iResultLineIndex );
				if( -1 != iResultLineIndex )
				{
					bDone = true;
				}
			}
			return vResultPos;
		}
		else
		{
			float fRandDist = (float)(rand()%10001) / 10000.f * fRange;
			int IsRight = rand()%2;
			if( 0 == IsRight ) // 왼쪽으로
			{
				float fWalkDist = GetDistance( *pvPos, pLineData->startPos );
				const LineData* pTempLineData = pLineData;
				while( pTempLineData->beforeLine >= 0 && fWalkDist < fRandDist )
				{
					pTempLineData = this->GetLineData( pTempLineData->beforeLine );					
					fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
				}

				if( fWalkDist < fRandDist )
				{
					fRandDist = (float)(rand()%10001) / 10000.f * fWalkDist;
					fWalkDist = GetDistance( *pvPos, pLineData->startPos );
					pTempLineData = pLineData;
					while( pTempLineData->beforeLine >= 0 && fWalkDist < fRandDist )
					{
						pTempLineData = this->GetLineData( pTempLineData->beforeLine );					
						fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					}

					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = fLerp * pTempLineData->startPos + (1.f-fLerp) * pTempLineData->endPos;
					return vResultPos;
				}
				else
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = fLerp * pTempLineData->startPos + (1.f-fLerp) * pTempLineData->endPos;
					return vResultPos;
				}
			}
			else // 오른쪽으로
			{
				float fWalkDist = GetDistance( *pvPos, pLineData->endPos );
				const LineData* pTempLineData = pLineData;
				while( pTempLineData->nextLine >= 0 && fWalkDist < fRandDist )
				{
					pTempLineData = this->GetLineData( pTempLineData->nextLine );					
					fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
				}

				if( fWalkDist < fRandDist )
				{
					fRandDist = (float)(rand()%10001) / 10000.f * fWalkDist;
					fWalkDist = GetDistance( *pvPos, pLineData->startPos );
					pTempLineData = pLineData;
					while( pTempLineData->nextLine >= 0 && fWalkDist < fRandDist )
					{
						pTempLineData = this->GetLineData( pTempLineData->nextLine );					
						fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					}

					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pTempLineData->startPos + fLerp * pTempLineData->endPos;
					return vResultPos;
				}
				else
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pTempLineData->startPos + fLerp * pTempLineData->endPos;
					return vResultPos;
				}		
			}
		}		
	}



	// 여기까지 오면 에러 
	return D3DXVECTOR3(0,0,0);
}



D3DXVECTOR3 CKTDGLineMap::GetRandomPositionDir( const D3DXVECTOR3* pvPos, float fRange, bool bOnThisLineGroup, bool bIsRight )
{
	int iLineIndex = -1;

	const float MAGIC_LINE_RADIUS = 30.f;

	if( NULL == pvPos )
	{
		// 여기는 아직 bIsRight 구현 안됨

		int iRandLineIndex = rand() % GetNumLineData();
		const LineData* pLineData = GetLineData( iRandLineIndex );


		if( pLineData == NULL || pLineData->bEnable == false || pLineData->lineType != LT_NORMAL)
		{
			int iTempRandIndex = iRandLineIndex+1;
			for( ; iTempRandIndex != iRandLineIndex; iTempRandIndex++ )
			{
				if( iTempRandIndex > GetNumLineData()-1 )
				{
					iTempRandIndex = 0;
				}

				pLineData = GetLineData( iTempRandIndex );
                if(pLineData != NULL && pLineData->bEnable == true && pLineData->lineType == LT_NORMAL) 
                    break;
			}
		}


		float fLerpCoeff = (float)(rand()%10001) / 10000.f; 		
		D3DXVECTOR3 vResultPos = (1.f-fLerpCoeff) * pLineData->startPos + fLerpCoeff * pLineData->endPos;		

		return vResultPos;	
	}
	else
	{
		D3DXVECTOR3 vPos = GetLandPosition( *pvPos, MAGIC_LINE_RADIUS, &iLineIndex );
		if( -1 == iLineIndex )
			return *pvPos;	// 아마 error

		const LineData* pLineData = GetLineData( iLineIndex );
#ifndef	X2OPTIMIZE_LINEMAP_LINEGROUP		
		LineGroup* pLineGroup = this->GetLineGroupIncludesLineData( pLineData );
#endif	X2OPTIMIZE_LINEMAP_LINEGROUP		
		

		if( false == bOnThisLineGroup )
		{
			int iResultLineIndex = -1;
			D3DXVECTOR3 vResultPos;
			bool bDone = false;
			while( false == bDone )
			{
				float fRandX = (float)(rand()%10001) / 10000.f;
				float fRandY = (float)(rand()%10001 - 5000) / 5000.f;
				float fRandZ = (float)(rand()%10001) / 10000.f;
				D3DXVECTOR3 vDir = pLineData->dirVector;
				if( false == bIsRight )
				{
					vDir.x = -vDir.x;
					vDir.z = -vDir.z;
				}

				vDir.x *= fRandX;
				vDir.z *= fRandZ;
				vDir.y = fRandY;
				D3DXVec3Normalize( &vDir, &vDir );				
				vResultPos = vPos + vDir * fRange;
				vResultPos = GetLandPosition( vResultPos, MAGIC_LINE_RADIUS, &iResultLineIndex );
				if( -1 != iResultLineIndex )
				{
					bDone = true;
				}
			}
			return vResultPos;
		}
		else
		{
			float fRandDist = (float)(rand()%10001) / 10000.f * fRange;
			if( false == bIsRight ) // 왼쪽으로
			{
				float fWalkDist = GetDistance( *pvPos, pLineData->startPos );

				if( fWalkDist > fRandDist )
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pLineData->startPos, *pvPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pLineData->startPos + fLerp * (*pvPos);
					return vResultPos;
				}

				
				const LineData* pTempLineData = pLineData;
				while( pTempLineData->beforeLine >= 0 && fWalkDist < fRandDist )
				{
					pTempLineData = this->GetLineData( pTempLineData->beforeLine );					
					fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
				}

				if( fWalkDist < fRandDist )
				{
					fRandDist = (float)(rand()%10001) / 10000.f * fWalkDist;
					fWalkDist = GetDistance( *pvPos, pLineData->startPos );
					pTempLineData = pLineData;
					while( pTempLineData->beforeLine >= 0 && fWalkDist < fRandDist )
					{
						pTempLineData = this->GetLineData( pTempLineData->beforeLine );					
						fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					}

					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = fLerp * pTempLineData->startPos + (1.f-fLerp) * pTempLineData->endPos;
					return vResultPos;
				}
				else
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = fLerp * pTempLineData->startPos + (1.f-fLerp) * pTempLineData->endPos;
					return vResultPos;
				}
			}
			else // 오른쪽으로
			{
				float fWalkDist = GetDistance( *pvPos, pLineData->endPos );

				if( fWalkDist > fRandDist )
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pLineData->endPos, *pvPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pLineData->endPos + fLerp * (*pvPos);

					return vResultPos;
				}

				const LineData* pTempLineData = pLineData;
				while( pTempLineData->nextLine >= 0 && fWalkDist < fRandDist )
				{
					pTempLineData = this->GetLineData( pTempLineData->nextLine );					
					fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
				}

				if( fWalkDist < fRandDist )
				{
					fRandDist = (float)(rand()%10001) / 10000.f * fWalkDist;
					fWalkDist = GetDistance( *pvPos, pLineData->startPos );
					pTempLineData = pLineData;
					while( pTempLineData->nextLine >= 0 && fWalkDist < fRandDist )
					{
						pTempLineData = this->GetLineData( pTempLineData->nextLine );					
						fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					}

					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pTempLineData->startPos + fLerp * pTempLineData->endPos;
					return vResultPos;
				}
				else
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pTempLineData->startPos + fLerp * pTempLineData->endPos;
					return vResultPos;
				}		
			}
		}		
	}



	// 여기까지 오면 에러 
	return D3DXVECTOR3(0,0,0);
}

D3DXVECTOR3 CKTDGLineMap::GetNearest_LUA(D3DXVECTOR3 inPos)
{
    D3DXVECTOR3 outPos(0,0,0);

#ifdef X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
	int iLineIndex = -1;
	GetNearestWithBound(inPos, FLT_MAX, &outPos, &iLineIndex);
#else//X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
	GetNearest(inPos, &outPos);
#endif//X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX

    return outPos;
}

int CKTDGLineMap::GetLineSetIndex( int lineIndex )
{
	const LineData* pLineData = GetLineData( lineIndex );
	if( pLineData == NULL )
		return -1;

	return pLineData->lineSet;
}



int CKTDGLineMap::GetRandomStartPosIndex()
{
#ifdef  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
	ASSERT( false == m_mapStartInfo.empty() );
	if( true == m_mapStartInfo.empty() )
		return -1;

	int iRand = rand() % (int)m_mapStartInfo.size();
    StartInfoMap::const_iterator it = m_mapStartInfo.begin();
    std::advance( it, iRand );
	return (int) it->first;
#else   X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
	ASSERT( false == m_mapStartPos.empty() );
	if( true == m_mapStartPos.empty() )
		return -1;

	int iRand = rand() % (int)m_mapStartPos.size();	
	map<int, D3DXVECTOR3>::iterator it = m_mapStartPos.begin();
	for( int i=0; i<iRand; i++ )
	{
		it++;
	}
	return (int) it->first;
#endif  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
}



const CKTDGLineMap::LineData* CKTDGLineMap::GetAnyEnabledNormalLine()
{
	for( int i = 0 ; i < (int) m_LineList.size(); i++ )
	{
		const LineData* pLineData = m_LineList[i];
		if( NULL == pLineData )
			continue;

		if( false == pLineData->bEnable )
			continue;

		if( LT_NORMAL == pLineData->lineType )
			return pLineData;
	}

	return NULL;
}

// SERV_PET_SYSTEM
bool CKTDGLineMap::GetAnyEnabledLine()
{
	for( int i = 0 ; i < (int) m_LineList.size(); i++ )
	{
		const LineData* pLineData = m_LineList[i];
		if( NULL == pLineData )
			continue;

		if( false == pLineData->bEnable )
			continue;

		return true;
	}

	return false;
}


#ifdef OTHER_ROAD
bool CKTDGLineMap::GetNearOtherLine( D3DXVECTOR3 pos, D3DXVECTOR3 &outPos, int lastLine, int &lineIndex, float fDistMax )
{
	float fDist = fDistMax;
	int bFindIndex = -1;
	bool bFind = false;
	const LineData* pLastLineData = GetLineData( lastLine );

	if(pLastLineData == NULL)
		return bFind;

	for( int i = 0 ; i < (int) m_LineList.size(); i++ )
	{
		if( i == lastLine )
			continue;

		const LineData* pLineData = m_LineList[i];
		if( NULL == pLineData )
			continue;

		if( false == pLineData->bEnable )
			continue;

//{{ kimhc // 2010.8.3 // WALL 타입의 라인맵 처리 변경
#ifdef	FIX_LINE_TYPE_WALL
		if ( LT_WALL == pLineData->lineType )
			continue;
#endif	FIX_LINE_TYPE_WALL
//}} kimhc // 2010.8.3 // WALL 타입의 라인맵 처리 변경
		
		if( pLineData->lineType == LT_OTHER_ROAD && pLastLineData->lineType == LT_NORMAL )
		{			
			D3DXVECTOR3 vOutPos = D3DXVECTOR3(0.f, 0.f, 0.f);
			float fNearLen = GetNearest(pos, &vOutPos, i);

			if( fNearLen < 999999.f )
			{
				// 두벡터 사이각이 90도 미만일경우
				if(fNearLen <= fDist)
				{				
					bFindIndex = i;
					fDist = fNearLen;
					bFind = true;
					outPos = vOutPos;
				}			
			}
			else
			{
				// 두벡터 사이각이 90도 이상일 경우
				float fDistStart	= 99999.f;
				float fDistEnd		= 99999.f;

				fDistStart	= GetDistance(pos, pLineData->startPos);
				fDistEnd	= GetDistance(pos, pLineData->endPos);

				if(fDistStart <= fDist)
				{
					bFindIndex = i;
					fDist = fDistStart;
					bFind = true;
					outPos = pLineData->startPos + pLineData->dirVector;
				}
				if(fDistEnd <= fDist)
				{
					bFindIndex = i;
					fDist = fDistEnd;
					bFind = true;
					outPos = pLineData->endPos - pLineData->dirVector;
				}
			}
			

			
		}	 
		else if( pLineData->lineType == LT_NORMAL && pLastLineData->lineType == LT_OTHER_ROAD )
		{			
			D3DXVECTOR3 vOutPos = D3DXVECTOR3(0.f, 0.f, 0.f);
			float fNearLen = GetNearest(pos, &vOutPos, i);
			if(fNearLen <= fDist)
			{				
				bFindIndex = i;
				fDist = fNearLen;
				bFind = true;
				outPos = vOutPos;
			}			
		}
	}

	lineIndex = bFindIndex;

	return bFind;
}
#endif

//{{ kimhc // 2010.7.6 // 몬스터가 생성한 라인맵
#ifdef	LINE_MAP_CREATED_BY_MONSTER

void CKTDGLineMap::SetStartEndPosAtThisLineMap_LUA( int iIndex, D3DXVECTOR3 vStartPos, D3DXVECTOR3 vEndPos )
{
	LineData* pLineData = _AccessLineData( iIndex );

	if ( pLineData != NULL )
	{
		pLineData->startPos = vStartPos;
		pLineData->endPos	= vEndPos;
	}

	MakeLine( pLineData );
}

#endif	LINE_MAP_CREATED_BY_MONSTER
//}} kimhc // 2010.7.6 // 몬스터가 생성한 라인맵



#ifdef WORLD_TRIGGER
bool CKTDGLineMap::AddTrigger_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	TRIGGER_SEGMENT triggerSeg;
    
	//LUA_GET_VALUE_ENUM( luaManager, "LINE_TYPE",		pLineData->lineType,			LINE_TYPE, LT_NORMAL	);
	LUA_GET_VALUE( luaManager,		"TRIGGER_ID",	triggerSeg.m_iTriggerId,	-1 );
	LUA_GET_VALUE( luaManager,		"TRIGGER_LOOP",	triggerSeg.m_bLoop,			false );
    	
	if( triggerSeg.m_iTriggerId < 0 )
		return false;

	int iTriggerDataIndex = 0;
	while( true == luaManager.BeginTable( "TRIGGER_DATA", iTriggerDataIndex ) )
	{
		TRIGGER_DATA	triggerData;

		int iTriggerMeshIndex = 0;
		while( true == luaManager.BeginTable( "TRIGGER_DATA_MESH", iTriggerMeshIndex ) )
		{
			TRIGGER_DATA_MESH triggerDataMesh;

			LUA_GET_VALUE_ENUM( luaManager, "TRIGGER_ACTION",		triggerDataMesh.m_iMeshTriggerAction,	TRIGGER_MESH_ACTION_TYPE, TAT_MESH_NONE	);
			LUA_GET_VALUE(		luaManager,	"PLAY_TYPE",	triggerDataMesh.m_iPlayType,			1 );
			LUA_GET_VALUE(		luaManager,	"DELAY_TIME",	triggerDataMesh.m_fTriggerTime,			0.f );			
			LUA_GET_VALUE(		luaManager,	"MESH_NAME",	triggerDataMesh.m_wstrMeshName,			L"" );
			LUA_GET_VALUE(		luaManager,	"ANIM_NAME",	triggerDataMesh.m_wstrName,				L"" );			
			LUA_GET_VALUE(		luaManager,	"ANIM_SPEED",	triggerDataMesh.m_fAnimSpeed,			1 );

			triggerData.m_vecTriggerMesh.push_back( triggerDataMesh );

			luaManager.EndTable();

			++iTriggerMeshIndex;
		}

		int iTriggerLineIndex = 0;
		while( true == luaManager.BeginTable( "TRIGGER_DATA_LINE", iTriggerLineIndex ) )
		{
			TRIGGER_DATA_LINE triggerDataLine;

			LUA_GET_VALUE_ENUM( luaManager, "TRIGGER_ACTION",	triggerDataLine.m_iLineTriggerAction,	TRIGGER_LINE_ACTION_TYPE, TAT_LINE_NONE	);
			LUA_GET_VALUE(		luaManager,	"DELAY_TIME",		triggerDataLine.m_fTriggerTime,			0 );
			LUA_GET_VALUE(		luaManager,	"LINE_ID",			triggerDataLine.m_iTriggerLineIndex,	-1 );
			
			switch( triggerDataLine.m_iLineTriggerAction )
			{
			case TAT_LINE_NONE:
				break;
			case TAT_LINE_CHANGE_ENABLE:
				{
					LUA_GET_VALUE(		luaManager,	"LINE_ENABLE",		triggerDataLine.m_bEnable,				true );
				}
				break;
			case TAT_LINE_CHANGE_SPEED:
				{
					LUA_GET_VALUE(		luaManager,	"LINE_SPEED",		triggerDataLine.m_fSpeed,				0 );
				}
				break;
			case TAT_LINE_CHANGE_JUMPSPEED:
				{
					LUA_GET_VALUE(		luaManager,	"LINE_JUMPSPEED_X",	triggerDataLine.m_vJumpSpeed.x,			0 );
					LUA_GET_VALUE(		luaManager,	"LINE_JUMPSPEED_Y",	triggerDataLine.m_vJumpSpeed.y,			0 );
				}
				break;
			case TAT_LINE_CHANGE_CAMERA:
				{
					LUA_GET_VALUE(		luaManager,	"LINE_CAMERA",		triggerDataLine.m_iCamera,				-1 );
				}
				break;
			case TAT_LINE_CHANGE_CANPASS:
				{
					LUA_GET_VALUE(		luaManager,	"LINE_CANPASS",		triggerDataLine.m_bEnable,				true );
				}
				break;
			case TAT_LINE_CHANGE_TELEPORT:
				{
					LUA_GET_VALUE(		luaManager,	"LINE_TELEPORT",	triggerDataLine.m_iCamera,				-1 );
				}
				break;
			default:
				break;
			}

			triggerData.m_vecTriggerLine.push_back( triggerDataLine );

			luaManager.EndTable();

			++iTriggerLineIndex;
		}

		int iTriggerEtcIndex = 0;
		while( true == luaManager.BeginTable( "TRIGGER_DATA_ETC", iTriggerEtcIndex ) )
		{
			TRIGGER_DATA_ETC triggerDataEtc;

			LUA_GET_VALUE_ENUM( luaManager, "TRIGGER_ACTION",		triggerDataEtc.m_iEtcTriggerAction,	TRIGGER_ETC_ACTION_TYPE, TAT_ETC_NONE	);
			LUA_GET_VALUE(		luaManager,	"DELAY_TIME",	triggerDataEtc.m_fTriggerTime,		0 );
			
			switch(triggerDataEtc.m_iEtcTriggerAction)
			{
			case TAT_ETC_NONE:
				break;
			case TAT_ETC_PLAY_SOUND:
				{
					LUA_GET_VALUE(		luaManager,	"LINE_SOUND",		triggerDataEtc.m_wstrActionName,	L"" );
				}
				break;
			case TAT_ETC_DURATION_TIME:
				break;
			case TAT_ETC_CAMERA:
				{
					LUA_GET_VALUE(		luaManager,	"CAMERA_INDEX",		triggerDataEtc.m_iDummyValue01,	-1 );
					LUA_GET_VALUE(		luaManager,	"CAMERA_DURATION",	triggerDataEtc.m_fDummyValue01,	0.f );
				}
				break;
			case TAT_ETC_CAMERA_SHAKE:
				{
					LUA_GET_VALUE(		luaManager,	"CAMERA_SHAKE_GAP",			triggerDataEtc.m_iDummyValue01,	0 );
					LUA_GET_VALUE(		luaManager,	"CAMERA_SHAKE_DURATION",	triggerDataEtc.m_fDummyValue01,	0.f );
				}
				break;
			case TAT_ETC_ACTIVE_LAYER:
				{
					LUA_GET_VALUE(		luaManager,	"ACTIVE_LAYER_ID",			triggerDataEtc.m_iDummyValue01,	-1 );
					LUA_GET_VALUE(		luaManager,	"FADE_TIME",				triggerDataEtc.m_fDummyValue01,	0.f );
				}
				break;
			}
			
			

			

			triggerData.m_vecTriggerEtc.push_back( triggerDataEtc );

			luaManager.EndTable();

			++iTriggerEtcIndex;
		}

		triggerSeg.m_vecTriggerData.push_back( triggerData );

		luaManager.EndTable();		

		++iTriggerDataIndex;
	}

	
	m_LineMapTrigger.m_vecTriggerSeg.push_back( triggerSeg );

	return true;
}
#endif




//////////////////////////////////////////////////////////////////////////
CKTDGLineMap::LineAnimData::LineAnimData( LINE_ANIM_TYPE eAnimType )
: m_eAnimType( eAnimType )
, m_bEnabled( true )
, m_bMoveIfSteppedOn( false )
, m_vPivotPosition( 0, 0, 0 )
, m_vChildPivotPosition( 0, 0, 0 )
, m_vChildPivotPositionDisplace( 0, 0, 0 )	
, m_pLineData( NULL )
, m_iLineAnimIndex( -1 )
, m_iParentAnimDataIndex( -1 )
{
}

void CKTDGLineMap::LineAnimData::OnFrameMove( double fTime, float fElapsedTime )
{
	//if( NULL != m_pLineData && false == m_pLineData->bEnable )
	//	return; 

	if( false == m_bEnabled )
		return;

	BOOST_TEST_FOREACH( LineAnimData*, pChlidAnimData, m_vecpChildAnimData )
	{
		if( pChlidAnimData->m_iParentAnimDataIndex >= 0 )
		{
			pChlidAnimData->m_vPivotPosition = m_vChildPivotPosition;
		}
		pChlidAnimData->OnFrameMove( fTime, fElapsedTime );
	}
}


//////////////////////////////////////////////////////////////////////////
CKTDGLineMap::LineAnimBackAndForth::LineAnimBackAndForth()
: LineAnimData( LAT_BACK_AND_FORTH )
, m_fCurrentPositionValue( 0.f )
, m_fMovingSpeed( 0.f )
, m_fRange( 0.f )
{
}

void CKTDGLineMap::LineAnimBackAndForth::OnFrameMove( double fTime, float fElapsedTime )
{
	m_fCurrentPositionValue += m_fMovingSpeed * fElapsedTime;
	if( m_fCurrentPositionValue > m_fRange )
	{
		m_fCurrentPositionValue = m_fRange;
		m_fMovingSpeed = -m_fMovingSpeed;
	}
	else if( m_fCurrentPositionValue < -m_fRange )
	{
		m_fCurrentPositionValue = -m_fRange;
		m_fMovingSpeed = -m_fMovingSpeed;
	}

	D3DXVECTOR3 vOldChildPivotPosition = m_vChildPivotPosition;
	m_vChildPivotPosition = m_vPivotPosition + m_vMovingDirection * m_fCurrentPositionValue;
	m_vChildPivotPositionDisplace = m_vChildPivotPosition - vOldChildPivotPosition;

	if( NULL != m_pLineData )
	{
		m_pLineData->startPos = m_vChildPivotPosition - m_pLineData->dirVector * m_pLineData->fLength * 0.5f;
		m_pLineData->endPos	= m_vChildPivotPosition + m_pLineData->dirVector * m_pLineData->fLength * 0.5f;
	}


	LineAnimData::OnFrameMove( fTime, fElapsedTime );
}


//////////////////////////////////////////////////////////////////////////
CKTDGLineMap::LineAnimMoveIfSteppedOn::LineAnimMoveIfSteppedOn()
: LineAnimData( LAT_MOVE_IF_STEPPED_ON )
, m_bSteppedOnForward( false )
, m_bSteppedOnBackward( false )
, m_fCurrentMovingSpeed( 0.f )
, m_fCurrentPostionValue( 0.f )
, m_fAccelerationWhenSteppedOn( 0.f )
, m_fBackwardSpeed( 0.f )
, m_fMaxMovingRange( 0.f )
, m_fMaxMovingSpeed( 0.f )
{
}

void CKTDGLineMap::LineAnimMoveIfSteppedOn::OnFrameMove( double fTime, float fElapsedTime )
{

	if( true == m_bSteppedOnForward )
	{
		m_fCurrentMovingSpeed += m_fAccelerationWhenSteppedOn * fElapsedTime;
	}
	
	if ( true == m_bSteppedOnBackward )
	{
		m_fCurrentMovingSpeed -= m_fAccelerationWhenSteppedOn * fElapsedTime;
	}
	
	if( false == m_bSteppedOnForward &&
		false == m_bSteppedOnBackward )
	{
		m_fCurrentMovingSpeed = m_fBackwardSpeed;
	}

	

	if( 0.f != m_fMaxMovingSpeed )
	{
		if( m_fCurrentMovingSpeed > m_fMaxMovingSpeed )
		{
			m_fCurrentMovingSpeed = m_fMaxMovingSpeed;
		}
		else if( m_fCurrentMovingSpeed < -m_fMaxMovingSpeed )
		{
			m_fCurrentMovingSpeed = -m_fMaxMovingSpeed;
		}
	}



	m_fCurrentPostionValue += m_fCurrentMovingSpeed * fElapsedTime;

	if( m_fCurrentPostionValue < 0.f )
	{
		m_fCurrentPostionValue = 0.f;
	}
	else if( m_fCurrentPostionValue > m_fMaxMovingRange )
	{
		m_fCurrentPostionValue = m_fMaxMovingRange;
	}


	D3DXVECTOR3 vOldChildPivotPosition = m_vChildPivotPosition;
	m_vChildPivotPosition	= m_vPivotPosition + m_vMovingDirection * m_fCurrentPostionValue;
	m_vChildPivotPositionDisplace = m_vChildPivotPosition - vOldChildPivotPosition;

	if( NULL != m_pLineData )
	{
		m_pLineData->startPos	= m_vChildPivotPosition - m_pLineData->dirVector * m_pLineData->fLength * 0.5f;
		m_pLineData->endPos		= m_vChildPivotPosition + m_pLineData->dirVector * m_pLineData->fLength * 0.5f;
	}


	LineAnimData::OnFrameMove( fTime, fElapsedTime );
}

//////////////////////////////////////////////////////////////////////////
CKTDGLineMap::LineAnimViking::LineAnimViking()
: LineAnimData( LAT_VIKING )
, m_fCurrentTime( 0.f )
, m_fMaxRotationRadian( 0.f )
, m_fPeriod( 0.f )
{
}

void CKTDGLineMap::LineAnimViking::OnFrameMove( double fTime, float fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;
	if( m_fCurrentTime > m_fPeriod )
	{
		m_fCurrentTime -= m_fPeriod;
	}

	CalcLinePosition( m_fCurrentTime );
	LineAnimData::OnFrameMove( fTime, fElapsedTime );
}

void CKTDGLineMap::LineAnimViking::CalcLinePosition( float fCurrentTime )
{
	float fCurrentRadian = m_fMaxRotationRadian * cos( 2.f * D3DX_PI * fCurrentTime / m_fPeriod );
	D3DXMATRIX matRotation;
	D3DXMatrixRotationAxis( &matRotation, &m_vRotationAxis, fCurrentRadian );

	
	D3DXVECTOR3 vDisplace;
	D3DXVec3TransformCoord( &vDisplace, &m_vEquilibriumPosition, &matRotation );

	D3DXVECTOR3 vTangentVector;
	D3DXVec3Cross( &vTangentVector, &vDisplace, &m_vRotationAxis );
	D3DXVec3Normalize( &vTangentVector, &vTangentVector );

	D3DXVECTOR3 vOldChildPivotPosition = m_vChildPivotPosition;
	m_vChildPivotPosition = m_vPivotPosition + vDisplace;
	m_vChildPivotPositionDisplace = m_vChildPivotPosition - vOldChildPivotPosition;

	if( NULL != m_pLineData )
	{
		m_pLineData->startPos = m_vChildPivotPosition + vTangentVector * m_pLineData->fLength * 0.5f;
		m_pLineData->endPos = m_vChildPivotPosition - vTangentVector * m_pLineData->fLength * 0.5f;

		CKTDGLineMap::MakeLine( m_pLineData );	// line dir vector, direction degree 등을 갱신해주어야 한다
	}
}



//////////////////////////////////////////////////////////////////////////
CKTDGLineMap::LineAnimCircle::LineAnimCircle()
: LineAnimData( LAT_CIRCLE )
, m_fCurrentRotationRadian( 0.f )
, m_fRotationSpeed( 0.f )
{
}

void CKTDGLineMap::LineAnimCircle::OnFrameMove( double fTime, float fElapsedTime )
{
	const float DOUBLE_PI = 2.f * D3DX_PI;
	m_fCurrentRotationRadian += m_fRotationSpeed * fElapsedTime;
	if( m_fCurrentRotationRadian > DOUBLE_PI )
	{
		m_fCurrentRotationRadian -= DOUBLE_PI;
	}
	else if( m_fCurrentRotationRadian < -DOUBLE_PI )
	{
		m_fCurrentRotationRadian += DOUBLE_PI;
	}

	CalcLinePosition( m_fCurrentRotationRadian );
	LineAnimData::OnFrameMove( fTime, fElapsedTime );
}



void CKTDGLineMap::LineAnimCircle::CalcLinePosition( float fCurrentRotationRadian )
{
	D3DXMATRIX matRotation;
	D3DXMatrixRotationAxis( &matRotation, &m_vRotationAxis, fCurrentRotationRadian );


	D3DXVECTOR3 vOldChildPivotPosition = m_vChildPivotPosition;
	D3DXVec3TransformCoord( &m_vChildPivotPosition, &m_vPositionVector, &matRotation );
	m_vChildPivotPosition += m_vPivotPosition;
	m_vChildPivotPositionDisplace = m_vChildPivotPosition - vOldChildPivotPosition;

	if( NULL != m_pLineData )
	{
		m_pLineData->startPos = m_vChildPivotPosition - m_pLineData->dirVector * m_pLineData->fLength * 0.5f;
		m_pLineData->endPos	= m_vChildPivotPosition + m_pLineData->dirVector * m_pLineData->fLength * 0.5f;
	}
}






//////////////////////////////////////////////////////////////////////////
//CKTDGLineMap::LineAnimRotateIfSteppedOn::LineAnimRotateIfSteppedOn()
//: LineAnimData( LAT_ROTATE_IF_STEPPED_ON )
//, m_bSteppedOnLeft( false )
//, m_bSteppedOnRight( false )
//, m_fCurrentRotationRadian( 0.f )
//{
//}
//
//void CKTDGLineMap::LineAnimRotateIfSteppedOn::OnFrameMove( double fTime, float fElapsedTime )
//{
//
//
//	
//	
//	m_vRotationAxis;
//m_vRotationCenter;
//m_fLeftWingLength;
//m_fRightWingLength;
//m_fMaxRotationRadian;
//m_fMaxRotationSpeed;
//m_fRotationAcceleration;
//m_bRotateBackToOriginalWhenNotSteppedOn;
//m_fOriginalRotationRadian;
//
//
//m_bSteppedOnLeft;
//m_bSteppedOnRight;
//m_fCurrentRotationRadian;
//m_fCurrentRotationSpeed;
//
//	if( true == m_bSteppedOnLeft )
//	{
//
//
//
//
//	}
//	else if( true == m_bSteppedOnRight )
//	{
//
//	}
//	else
//	{
//		if( true == m_bRotateBackToOriginalWhenNotSteppedOn )
//		{
//			if( m_fCurrentRotationSpeed > 0.f )
//			{
//
//
//			}
//
//
//		}
//
//
//
//	}
//
//	
//	D3DXMATRIX matRotation;
//	D3DXMatrixRotationAxis( &matRotation, &vLocalRotationAxis, m_fHeadIKAngle );
//
//	
//	
//
//
//	D3dx
//
//m_vRotationAxis;
//m_vRotationCenter;
//m_fMaxRotationRadian;
//m_fRotationSpeed;
//m_bRollBackWhenNotSteppedOn;
//m_fOriginalRotationRadian;
//
//m_bSteppedOnLeft;
//m_bSteppedOnRight;
//m_fCurrentRotationRadian;
//
//
//
//
//	m_vRotationAxis;
//	m_vRotationCenter
//
//		m_fMaxRotationAngle;
//	m_fRotationSpeed;
//	m_bRollBackWhenNotSteppedOn;
//	m_bRotateForward;
//
//	m_bSteppedOn;
//	m_fCurrentRotationAngle;
//
//
//}

#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
	int CKTDGLineMap::LineData::GetRandomLinkedLineIndex( int iRandomSeed /*= 0 */ )
	{
		if( m_vecLinkedLineIndex.empty() )
			return -1;

		//iRandomSeed = iRandomSeed % (int)m_vecLinkedLineIndex.size();
		// 이 함수 수정해야함 무조건 하나의 라인에는 하나의 linked line이 있는 것으로 하자

		//return m_vecLinkedLineIndex[ iRandomSeed ];
		return m_vecLinkedLineIndex[0];
	}
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST



// 기가 프로미넌스를 위한 위치 잡는 기능
#ifdef RAVEN_WEAPON_TAKER
bool CKTDGLineMap::GetFixedDistancePositionInAllLine( const D3DXVECTOR3& vDirVector, const D3DXVECTOR3& vPos, float fDist, bool bIsRight, OUT D3DXVECTOR3& vTargetPos, OUT int& iTargetLineIndex )
{
	D3DXVECTOR3 vTempTargetPos( 0.f, -1.f, 0.f );
	int iTempTargetLineIndex = -1;
	float fMinDistance = 999999.f;

	// 모든 선을 검색한다
	for( int i = 0; i < (int)m_LineList.size(); i++ )
	{

		const LineData* pLineData = m_LineList[i];

		// 기가 프로미넌스는 몬스터 위에서도 사용할 수 있으므로
		if( VerifyLine( i, true ) == false )
			continue;


		//점과 선사이의 최단 거리를 구한다.
		float fLineLength				= GetDistance( pLineData->startPos, pLineData->endPos );
		float fLineStartToPointLength	= GetDistance( pLineData->startPos, vPos );
		float fLineEndToPointLength		= GetDistance( pLineData->endPos, vPos );

		D3DXVECTOR3 lineDir = pLineData->endPos - pLineData->startPos;
		D3DXVec3Normalize( &lineDir, &lineDir );
		D3DXVECTOR3 pointDir = vPos - pLineData->startPos;

		D3DXVECTOR3 targetPoint;

		if( fDist < fLineStartToPointLength && fDist < fLineEndToPointLength )
		{

			D3DXVECTOR3 projectionPoint;
			float		fProjectionLength	= D3DXVec3Dot( &lineDir, &pointDir );

			if( fProjectionLength <= 0.0f )
				continue;

			projectionPoint	= lineDir * fProjectionLength + pLineData->startPos;


			float fLineStartToProjectPointLength = GetDistance( pLineData->startPos, projectionPoint );

			float fLinePointToProjectPointLength2 = fLineStartToPointLength * fLineStartToPointLength - fLineStartToProjectPointLength * fLineStartToProjectPointLength;

			if( fLinePointToProjectPointLength2 > fDist * fDist)
				continue;

			if( fLineStartToProjectPointLength > fLineLength )
				continue;



			float fSqrt = sqrt( fProjectionLength * fProjectionLength - ( fLineStartToPointLength*fLineStartToPointLength - fDist*fDist ) );

			float fTargetLength = fProjectionLength - fSqrt;

			D3DXVECTOR3 lineVector = pLineData->endPos - pLineData->startPos;

			if( bIsRight == true )
			{
				if( D3DXVec3Dot( &vDirVector, &lineVector ) < 0.f )
				{
					targetPoint		= lineDir * fTargetLength + pLineData->startPos;
					vTempTargetPos = targetPoint;
					iTempTargetLineIndex	= i;
				}
				else
				{
					
					targetPoint		= lineDir * ( 2*fLineStartToProjectPointLength - fTargetLength) + pLineData->startPos;
					vTempTargetPos = targetPoint;
					iTempTargetLineIndex	= i;
				}
			}
			else
			{
				if( D3DXVec3Dot( &vDirVector, &lineVector ) > 0.f )
				{
					targetPoint		= lineDir * fTargetLength + pLineData->startPos;
					vTempTargetPos = targetPoint;
					iTempTargetLineIndex	= i;
				}
				else
				{

					targetPoint		= lineDir * (2 * fLineStartToProjectPointLength - fTargetLength) + pLineData->startPos;
					vTempTargetPos = targetPoint;
					iTempTargetLineIndex	= i;
				}
			}
		}
		else if( fLineStartToPointLength >= fDist && fLineEndToPointLength < fDist )
		{
			
			float		fProjectionLength	= D3DXVec3Dot( &lineDir, &pointDir );

			if( fProjectionLength <= 0.0f )
				continue;


			float fSqrt = sqrt( fProjectionLength * fProjectionLength - ( fLineStartToPointLength*fLineStartToPointLength - fDist*fDist ) );

			float fTargetLength = fProjectionLength - fSqrt;




			D3DXVECTOR3 lineVector = pLineData->endPos - pLineData->startPos;

			if( bIsRight == true)
			{
				if( D3DXVec3Dot( &vDirVector, &lineVector ) < 0.f )
				{
					targetPoint		= lineDir * fTargetLength + pLineData->startPos;
					vTempTargetPos = targetPoint;
					iTempTargetLineIndex	= i;
				}
			}
			else
			{
				if( D3DXVec3Dot( &vDirVector, &lineVector ) > 0.f )
				{
					targetPoint		= lineDir * fTargetLength + pLineData->startPos;
					vTempTargetPos = targetPoint;
					iTempTargetLineIndex	= i;
				}

			}

		}
		else if( fLineEndToPointLength >= fDist && fLineStartToPointLength < fDist )
		{
			float fLineStartToPointLength	= GetDistance( pLineData->endPos, vPos );
			float fLineEndToPointLength		= GetDistance( pLineData->startPos, vPos );
			D3DXVECTOR3 pointDir = vPos - pLineData->endPos;
			D3DXVECTOR3 lineDir = pLineData->startPos - pLineData->endPos;
			D3DXVec3Normalize( &lineDir, &lineDir );

			D3DXVECTOR3 projectionPoint;
			float		fProjectionLength	= D3DXVec3Dot( &lineDir, &pointDir );

			if( fProjectionLength <= 0.0f )
				continue;


			float fSqrt = sqrt( fProjectionLength * fProjectionLength - ( fLineStartToPointLength*fLineStartToPointLength - fDist*fDist ) );

			float fTargetLength = fProjectionLength - fSqrt;



			D3DXVECTOR3 lineVector = pLineData->endPos - pLineData->startPos;

			if( bIsRight == true)
			{
				if( D3DXVec3Dot( &vDirVector, &lineVector ) > 0.f )
				{
					targetPoint		= lineDir * fTargetLength + pLineData->endPos;
					vTempTargetPos = targetPoint;
					iTempTargetLineIndex	= i;
				}
			}
			else
			{
				if( D3DXVec3Dot( &vDirVector, &lineVector ) < 0.f )
				{
					targetPoint		= lineDir * fTargetLength + pLineData->endPos;
					vTempTargetPos = targetPoint;
					iTempTargetLineIndex	= i;
				}
			}
		}
		else
		{
			continue;
		}

/*
		// 점과 선사이의 최단 거리를 구한다.
		D3DXVECTOR3 projectionPoint;
		float		fFinalLength;
		fFinalLength = GetLengthToLine( vPos, i, &projectionPoint );
		float fLengthOnLine = sqrt( fDist * fDist - fFinalLength * fFinalLength );

		if( fFinalLength > fMinDistance )
			continue;

		if( bIsRight == false )
		{
			D3DXVECTOR3 vProjectionToStart = pLineData->startPos - projectionPoint;
			float fProjectionToStart = D3DXVec3Length( &vProjectionToStart );

			D3DXVECTOR3 vProjectionToTarget;
			D3DXVec3Normalize( &vProjectionToTarget, &vProjectionToStart );
			vProjectionToTarget *= fLengthOnLine;
			float fProjectionToTarget = D3DXVec3Length( &vProjectionToTarget );

			if( fProjectionToTarget < fProjectionToStart )
			{
				fMinDistance = fFinalLength;
				vTempTargetPos = vProjectionToTarget + projectionPoint;
				iTempTargetLineIndex = i;
			}
		}
		else
		{
			D3DXVECTOR3 vProjectionToEnd = pLineData->endPos - projectionPoint;
			float fProjectionToEnd = D3DXVec3Length( &vProjectionToEnd );

			D3DXVECTOR3 vProjectionToTarget;
			D3DXVec3Normalize( &vProjectionToTarget, &vProjectionToEnd );
			vProjectionToTarget *= fLengthOnLine;
			float fProjectionToTarget = D3DXVec3Length( &vProjectionToTarget );

			if( fProjectionToTarget < fProjectionToEnd )
			{
				fMinDistance = fFinalLength;
				vTempTargetPos = vProjectionToTarget + projectionPoint;
				iTempTargetLineIndex = i;
			}
		}
*/
	}

	if( iTempTargetLineIndex == -1 )
		return false;

	vTargetPos = vTempTargetPos;
	iTargetLineIndex = iTempTargetLineIndex;

	return true;
}
#endif RAVEN_WEAPON_TAKER

/*
	LPD3DXLINE은 내부적으로 D3DPOOL_DEFAULT를 사용하는 것 같다.
	해상도 변경 또는 D3DDevice 리셋이 발생할 때 Device->Reset()이
	실패해서 게임이 무한 루프에 빠지는 경우가 발생한다.
	ResetDevice 및 LostDevice를 정의해야된다.
*/
HRESULT CKTDGLineMap::OnResetDevice()
{
	D3DXCreateLine( g_pKTDXApp->GetDevice(), &m_pLine );
	m_pLine->SetWidth( 3.f );
	m_pLine->SetAntialias( true );

	return S_OK;
}

HRESULT CKTDGLineMap::OnLostDevice()
{
	SAFE_RELEASE( m_pLine );

	return S_OK;
}



#ifdef  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
bool    CKTDGLineMap::UpdateLineData( int iIndex, const D3DXVECTOR3& vStart, const D3DXVECTOR3& vEnd )
{
    if ( iIndex < 0 || iIndex >= (int) m_LineList.size() )
        return false;
    LineData* pLineData = m_LineList[ iIndex ];
    if ( pLineData == NULL )
        return false;
    pLineData->startPos = vStart;
    pLineData->endPos = vEnd;
    MakeLine( pLineData );
    return true;
}
#endif  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
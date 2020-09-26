#include "baseserver.h"
#include "X2Data/XSLStat.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include <d3dx9math.h>
#include ".\xsllinemap.h"


#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "d3dx9d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#endif


CXSLLineMap::CXSLLineMap()
{
	m_pLineMapVB	= NULL;

	m_LineList.reserve( 64 );
	m_RedTeamStartPos.reserve( 8 );
	m_BlueTeamStartPos.reserve( 8 );
	m_RedTeamStartRight.reserve( 8 );
	m_BlueTeamStartRight.reserve( 8 );
	m_RedTeamStartLineIndex.reserve( 8 );
	m_BlueTeamStartLineIndex.reserve( 8 );

	m_vecLineGroupList.reserve( 32 );

#ifdef WORLD_TRIGGER
	m_LineMapTrigger.m_vecNowTrigger.clear();
	m_LineMapTrigger.m_vecTriggerSeg.clear();
#endif
}

CXSLLineMap::~CXSLLineMap(void)
{
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

#ifdef LINEMAP_RECT_TEST
	BOOST_TEST_FOREACH( RectData*, pRectData, m_RectList )
	{
		SAFE_DELETE( pRectData );
	}
#endif LINEMAP_RECT_TEST


	m_RedTeamStartPos.clear();
	m_BlueTeamStartPos.clear();
	m_mapStartPos.clear();

	ReleaseLineGroup();
}

bool CXSLLineMap::LoadLineMap( std::wstring pFileName )
{
#ifdef WORLD_TOOL
	WCHAR wszFullPath[MAX_PATH] = L"";
	wcscpy( wszFullPath, pFileName.c_str() );
	std::wstring wstrFullPath = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->GetPullPath( wszFullPath );
	CompulsorybyConvertFileAnsiToUTF8W( wstrFullPath );
#endif WORLD_TOOL

#ifdef WORLD_TRIGGER
	m_LineMapTrigger.m_vecNowTrigger.clear();
	m_LineMapTrigger.m_vecTriggerSeg.clear();
#endif

	//KLuabinder* pKReay6lLuaBinder = g_pKTDXApp->GetLuaBinder();

	//if ( m_pKLuabinder != NULL )
	//	pKRealLuaBinder = m_pKLuabinder;

	//lua_tinker::decl( pKRealLuaBinder->GetLuaState(), "g_pLineMap", this );

	//if( pKRealLuaBinder->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	//{
	//	string strFileName;
	//	ConvertWCHARToChar( strFileName, pFileName.c_str() );
	//	return false;
	//}

	ReBuildLineGroup();

#ifdef WORLD_TRIGGER		
	if( m_LineMapTrigger.m_vecTriggerSeg.size() > 2 )
		std::sort( m_LineMapTrigger.m_vecTriggerSeg.begin(), m_LineMapTrigger.m_vecTriggerSeg.end(), TriggerLess() );
#endif

	return true;
}




bool CXSLLineMap::AddCameraData_LUA()
{

#ifdef NEW_VILLAGE_RENDERING_TEST
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

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




bool CXSLLineMap::AddLine_LUA()
{
	KLuaManager luaManager( g_pLua );
	
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

	int iLineDepth = 0;
	LUA_GET_VALUE( luaManager,		"LINE_DEPTH",				iLineDepth,		0			);

	if( 0 != iLineDepth )
	{
		SAFE_DELETE( pLineData );
		return false; 
	}

#endif LINKED_LINEMAP_JUMP_MONSTER_TEST


#ifdef MOVING_LINE_MAP_TEST

	int iLineAnimDataIndex = -1;
	LUA_GET_VALUE( luaManager,		"LINE_ANIM_DATA_INDEX",				iLineAnimDataIndex,		-1			);

#endif MOVING_LINE_MAP_TEST

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "START_POS", pLineData->startPos, D3DXVECTOR3(0,0,0) );
	LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "END_POS", pLineData->endPos, D3DXVECTOR3(0,0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	pLineData->startPos		= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(),  "START_POS" );
	pLineData->endPos		= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(),  "END_POS" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


#ifdef ATTACH_MESH_LINE
	pLineData->m_vPivotPos = ( pLineData->endPos - pLineData->startPos ) * 0.5f;
#endif

	_JIF( pLineData->startPos != pLineData->endPos, return false );
#ifdef NO_MORE_ERROR_TEST

	if( pLineData->startPos == pLineData->endPos )
	{
		InHouseLog( "ERROR: linemap에 startpos == endpos인 line이 포함되어있음" );
	}

#endif NO_MORE_ERROR_TEST

	CXSLLineMap::MakeLine( pLineData );


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

	#ifdef MOVING_LINE_MAP_TEST
		AssignLineAnimDataToLine( pLineData, iLineAnimDataIndex );
	#endif MOVING_LINE_MAP_TEST

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



	#ifdef MOVING_LINE_MAP_TEST
		AssignLineAnimDataToLine( pLineData, iLineAnimDataIndex );
	#endif MOVING_LINE_MAP_TEST


	}
#endif	FIX_LINE_TYPE_WALL
	//}} kimhc // 2010-08-03 // WALL 타입의 라인맵 처리 변경
	
#ifdef WORLD_TOOL
	pLineData->m_vOriJumpSpeed = pLineData->m_vJumpSpeed;
	pLineData->m_fOriSpeed = pLineData->m_fSpeed;
	pLineData->m_iOriTeleportLineIndex = pLineData->m_iTeleportLineIndex;
	pLineData->m_iOriCameraIndex = pLineData->m_iCameraIndex;

	pLineData->m_bOriCanPass	= pLineData->m_bCanPass;
	pLineData->bOriEnable		= pLineData->bEnable;
#endif

	return true;
}









bool CXSLLineMap::AddLineAnim_LUA()
{

#ifdef MOVING_LINE_MAP_TEST

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );


	LINE_ANIM_TYPE eLineAniType = LAT_NONE;
	LUA_GET_VALUE_ENUM( luaManager, L"LINE_ANIM_TYPE",			eLineAniType,	LINE_ANIM_TYPE,		LAT_NONE	);
	ASSERT( LAT_NONE != eLineAniType );
	if( LAT_NONE == eLineAniType )
		return false;


	int iLineAnimIndex = 0;
	LUA_GET_VALUE( luaManager,		L"LINE_ANIM_INDEX",			iLineAnimIndex,				-1	);
	ASSERT( iLineAnimIndex >= 0 );
	if( iLineAnimIndex < 0 )
		return false;



	int iParentLineAnimIndex = 0;
	LUA_GET_VALUE( luaManager,		L"PARENT_LINE_ANIM_INDEX",			iParentLineAnimIndex,				-1	);



	bool bLineAniEnabled = false;
	LUA_GET_VALUE( luaManager,		L"LINE_ANIM_ENABLE",		bLineAniEnabled,			true	);

	bool bMoveIfSteppedOn = false;
	LUA_GET_VALUE( luaManager,		L"LINE_ANIM_IF_STEPPED_ON",	bMoveIfSteppedOn,			false	);	


	LineAnimData* pAnimDataBasic = NULL;

	switch( eLineAniType )
	{
	case LAT_BACK_AND_FORTH:
		{
			LineAnimBackAndForth* pAnimData = new LineAnimBackAndForth;
			LUA_GET_VALUE( luaManager,		L"LINE_ANIM_RANGE",		pAnimData->m_fRange,				0.f		);
			LUA_GET_VALUE( luaManager,		L"LINE_ANIM_SPEED",		pAnimData->m_fMovingSpeed,			0.f		);

			ASSERT( pAnimData->m_fRange > 0.f );

			pAnimData->m_vPivotPosition		= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "PIVOT_POSITION"			);
			pAnimData->m_vMovingDirection	= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "LINE_ANIM_DIRECTION"	);
			D3DXVec3Normalize( &pAnimData->m_vMovingDirection, &pAnimData->m_vMovingDirection );


			pAnimDataBasic = pAnimData;

		} break;

	case LAT_VIKING:
		{
			LineAnimViking* pAnimData = new LineAnimViking;

			LUA_GET_VALUE( luaManager,		L"LINE_ANIM_PERIOD",				pAnimData->m_fPeriod,								0.f );
			LUA_GET_VALUE( luaManager,		L"LINE_ANIM_MAX_ROTATION_DEGREE",	pAnimData->m_fMaxRotationRadian,					0.f );
			pAnimData->m_fMaxRotationRadian *= D3DX_PI / 180.f;


			pAnimData->m_vPivotPosition			= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "PIVOT_POSITION"	);
			pAnimData->m_vEquilibriumPosition	= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "LINE_ANIM_EQUILIBRIUM_POSITION"		);
			pAnimData->m_vRotationAxis			= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "LINE_ANIM_ROTATION_AXIS"			);
			D3DXVec3Normalize( &pAnimData->m_vRotationAxis, &pAnimData->m_vRotationAxis );

			pAnimDataBasic = pAnimData;

		} break;



	case LAT_CIRCLE:
		{
			LineAnimCircle* pAnimData = new LineAnimCircle;

			LUA_GET_VALUE( luaManager,		L"LINE_ANIM_ROTATION_SPEED",		pAnimData->m_fRotationSpeed,				0.f	);

			pAnimData->m_vPivotPosition			= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "PIVOT_POSITION"	);
			pAnimData->m_vPositionVector		= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "LINE_ANIM_POSITION_VECTOR"		);
			pAnimData->m_vRotationAxis			= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "LINE_ANIM_ROTATION_AXIS"		);
			D3DXVec3Normalize( &pAnimData->m_vRotationAxis, &pAnimData->m_vRotationAxis );


			pAnimDataBasic = pAnimData;

		} break;



	case LAT_MOVE_IF_STEPPED_ON:
		{
			LineAnimMoveIfSteppedOn* pAnimData = new LineAnimMoveIfSteppedOn;

			LUA_GET_VALUE( luaManager,		L"LINE_ANIM_ACCELERATION",		pAnimData->m_fAccelerationWhenSteppedOn,	0.f		);
			LUA_GET_VALUE( luaManager,		L"LINE_ANIM_MAX_SPEED",			pAnimData->m_fMaxMovingSpeed,				0.f		);
			LUA_GET_VALUE( luaManager,		L"LINE_ANIM_BACKWARD_SPEED",	pAnimData->m_fBackwardSpeed,				0.f		);
			LUA_GET_VALUE( luaManager,		L"LINE_ANIM_RANGE",				pAnimData->m_fMaxMovingRange,				0.f		);

			ASSERT( pAnimData->m_fMaxMovingSpeed >= 0.f );
			ASSERT( pAnimData->m_fMaxMovingRange >= 0.f );

			pAnimData->m_vPivotPosition		= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "PIVOT_POSITION"	);
			pAnimData->m_vMovingDirection	= lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "LINE_ANIM_DIRECTION"		);
			D3DXVec3Normalize( &pAnimData->m_vMovingDirection, &pAnimData->m_vMovingDirection );


			pAnimDataBasic = pAnimData;
		} break;
	}





	ASSERT( NULL != pAnimDataBasic );
	if( NULL != pAnimDataBasic )
	{
		pAnimDataBasic->m_iLineAnimIndex = iLineAnimIndex;
		pAnimDataBasic->m_bEnabled = bLineAniEnabled;
		pAnimDataBasic->m_bMoveIfSteppedOn = bMoveIfSteppedOn;
		pAnimDataBasic->m_iParentAnimDataIndex = iParentLineAnimIndex;

		if( NULL != GetLineAnimData( iLineAnimIndex ) ) // 이미 있으면 오류
		{
			ASSERT( !"existing line anim data" );
			SAFE_DELETE( pAnimDataBasic );
			return false;
		}

		m_mapLineAnimData[ iLineAnimIndex ] = pAnimDataBasic;
		return true;
	}



#endif MOVING_LINE_MAP_TEST

	return false;
	
}

#ifdef LINEMAP_RECT_TEST
	bool CXSLLineMap::AddRect_LUA() 
	{
		KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
		TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

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


	bool CXSLLineMap::IsInAnyRect( const D3DXVECTOR3& vPoint, D3DXVECTOR3* pNearestOutPoint /*= NULL*/ )
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

/*static*/ void CXSLLineMap::MakeLine( LineData* pLineData )
{
	if( NULL == pLineData )
		return; 

	float temp;
	D3DXMATRIX matRotationOrth;
	D3DXMATRIX matRotation;

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

CXSLLineMap::LineData* CXSLLineMap::GetLineData( int index )
{ 
	if ( index < 0 || index >= (int)m_LineList.size() )
	{
		return NULL;
	}

	return m_LineList[index]; 
}

#if 0
float CXSLLineMap::GetDownNearest( D3DXVECTOR3 inPos, D3DXVECTOR3* pOutPos, int* pLlineIndex )
{
	D3DXVECTOR3 target;
	float		fProjectionLength;
	float		outLength = 999999;
	float		tempLength;
	D3DXVECTOR3	tempPos;

	//*lineIndex = -1;

	for( int i = 0; i < (int)m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		if( false == pLineData->bEnable )
			continue;

		if( inPos.y < pLineData->startPos.y && inPos.y < pLineData->endPos.y )
			continue;

		if( (inPos.x < pLineData->startPos.x && inPos.x < pLineData->endPos.x)
			|| (inPos.x > pLineData->startPos.x && inPos.x > pLineData->endPos.x) )
		{
			if( (inPos.y < pLineData->startPos.y && inPos.y < pLineData->endPos.y)
				|| (inPos.y > pLineData->startPos.y && inPos.y > pLineData->endPos.y) )
			{
				if( (inPos.z < pLineData->startPos.z && inPos.z < pLineData->endPos.z)
					|| (inPos.z > pLineData->startPos.z && inPos.z > pLineData->endPos.z) )
				{
					continue;
				}
			}
		}

		target				= inPos - pLineData->startPos;
		fProjectionLength	= D3DXVec3Dot( &pLineData->dirVector, &target );
		if( fProjectionLength >= 0.0f && fProjectionLength < pLineData->fLength )
		{
			tempPos		= fProjectionLength * pLineData->dirVector + pLineData->startPos;
			tempLength	= GetDistance( tempPos, inPos );
			if( tempLength < outLength )
			{
				outLength		= tempLength;
				*pOutPos		= tempPos;
				*pLlineIndex	= i;
			}
		}
	}
	return outLength;
}
#endif



void CXSLLineMap::ReleaseLineGroup()
{
	for( unsigned i=0; i<m_vecLineGroupList.size(); i++ )
	{
		SAFE_DELETE( m_vecLineGroupList[i] );
	}

	m_vecLineGroupList.resize( 0 );
	m_mapLineData2LineGroup.clear();
}

CXSLLineMap::LineGroup* CXSLLineMap::GetLineGroupIncludesLineData( CXSLLineMap::LineData* pLineData )
{
	if( NULL == pLineData )
		return NULL;

	std::map<LineData*, LineGroup*>::iterator it;
	it = m_mapLineData2LineGroup.find( pLineData );
	if( it != m_mapLineData2LineGroup.end() )
		return it->second;
	else
		return NULL;
}

// fix!! 이 코드 왠지 계산시간 많이 걸릴것 같은데...
// linegroup로 vector가 아닌 map에 저장해야? 
void CXSLLineMap::EnableLineData( CXSLLineMap::LineData* pLineData, bool bEnable )
{
	if( pLineData->bEnable == bEnable )
		return;

	pLineData->bEnable = bEnable;

	LineData* pBeforeLine = GetLineData( pLineData->beforeLine );
	LineData* pNextLine = GetLineData( pLineData->nextLine );

	std::vector<LineGroup*>::iterator it;
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
				pBeforeLine = GetLineData( pBeforeLine->beforeLine );
			}
			
			while( NULL != pNextLine )
			{
				pNextLineGroup->vecpLineData.push_back( pNextLine );
				m_mapLineData2LineGroup[ pNextLine ] = pNextLineGroup;
				pNextLine = GetLineData( pNextLine->nextLine );
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


void CXSLLineMap::EnableLineSet( int iLineSetIndex, bool bEnable )	
{
	if( iLineSetIndex < 0 )
		return;

	for( unsigned int i = 0; i < m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		if( iLineSetIndex == pLineData->lineSet )
		{
			pLineData->bEnable = bEnable;
		}
	}


	ReBuildLineGroup();

}

void CXSLLineMap::DisableAllLineData()
{
	for( int i = 0 ; i < (int) m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		pLineData->bEnable = false;
	}

	ReBuildLineGroup();
}

void CXSLLineMap::ReBuildLineGroup()
{
	CXSLLineMap* pLineMap = this;

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

		LineData* pLineData = pLineMap->GetLineData(i);
        if( false == pLineData->bEnable )		
		{
			continue;
		}

		bDoneLineData[i] = true;
		LineGroup* pGroup = new LineGroup();		
		pGroup->vecpLineData.push_back( pLineData );
		pGroup->vStartPos = pLineData->startPos;
		pGroup->vEndPos = pLineData->endPos;

		LineData* pTempLineData = pLineData;
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
			LineData* pLineData = pGroup->vecpLineData.at(j);
			m_mapLineData2LineGroup.insert( std::make_pair(pLineData, pGroup) );
		}
	}

	SAFE_DELETE_ARRAY(bDoneLineData);			
}

D3DXVECTOR3 CXSLLineMap::GetNearest_LUA(D3DXVECTOR3 inPos)
{
    D3DXVECTOR3 outPos;

    GetNearest(inPos, &outPos);

    return outPos;
}

int CXSLLineMap::GetLineSetIndex( int lineIndex )
{
	LineData* pLineData = GetLineData( lineIndex );
	if( pLineData == NULL )
		return -1;

	return pLineData->lineSet;
}



int CXSLLineMap::GetRandomStartPosIndex()
{
	_JIF( false == m_mapStartPos.empty(), return -1 );
	if( true == m_mapStartPos.empty() )
		return -1;

	int iRand = rand() % (int)m_mapStartPos.size();	
	std::map<int, D3DXVECTOR3>::iterator it = m_mapStartPos.begin();
	for( int i=0; i<iRand; i++ )
	{
		it++;
	}

	return (int) it->first;
}



CXSLLineMap::LineData* CXSLLineMap::GetAnyEnabledNormalLine()
{
	for( int i = 0 ; i < (int) m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
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
bool CXSLLineMap::GetAnyEnabledLine()
{
	for( int i = 0 ; i < (int) m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		if( NULL == pLineData )
			continue;

		if( false == pLineData->bEnable )
			continue;

		return true;
	}

	return false;
}

//{{ kimhc // 2010.7.6 // 몬스터가 생성한 라인맵
#ifdef	LINE_MAP_CREATED_BY_MONSTER

void CXSLLineMap::SetStartEndPosAtThisLineMap_LUA( int iIndex, D3DXVECTOR3 vStartPos, D3DXVECTOR3 vEndPos )
{
	LineData* pLineData = GetLineData( iIndex );

	if ( pLineData != NULL )
	{
		pLineData->startPos = vStartPos;
		pLineData->endPos	= vEndPos;
	}

	MakeLine( pLineData );
}

#endif	LINE_MAP_CREATED_BY_MONSTER
//}} kimhc // 2010.7.6 // 몬스터가 생성한 라인맵



#ifdef ATTACH_MESH_LINE
void CXSLLineMap::UpdateMoveLine(int iLineIndex, D3DXVECTOR3 &vPivotPos)
{
	if( iLineIndex < 0 )
		return;

	LineData *pLineData = GetLineData( iLineIndex );

	if( pLineData != NULL )
	{
		pLineData->m_vOffsetPos = vPivotPos - pLineData->m_vPivotPos;
		pLineData->m_vPivotPos = vPivotPos;
		pLineData->startPos = pLineData->m_vPivotPos - pLineData->dirVector * pLineData->fLength * 0.5f;
		pLineData->endPos	= pLineData->m_vPivotPos + pLineData->dirVector * pLineData->fLength * 0.5f;
		pLineData->m_bMoveLine = true;
	}
}
#endif

#ifdef WORLD_TRIGGER
bool CXSLLineMap::AddTrigger_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	TRIGGER_SEGMENT triggerSeg;
    
	//LUA_GET_VALUE_ENUM( luaManager, "LINE_TYPE",		pLineData->lineType,			LINE_TYPE, LT_NORMAL	);
	LUA_GET_VALUE( luaManager,		"TRIGGER_ID",	triggerSeg.m_iTriggerId,	-1 );
	LUA_GET_VALUE( luaManager,		"TRIGGER_LOOP",	triggerSeg.m_bLoop,			false );
    	
	if( triggerSeg.m_iTriggerId < 0 )
		return false;

	int iTriggerDataIndex = 0;
	while( true == luaManager.BeginTable( L"TRIGGER_DATA", iTriggerDataIndex ) )
	{
		TRIGGER_DATA	triggerData;

		int iTriggerMeshIndex = 0;
		while( true == luaManager.BeginTable( L"TRIGGER_DATA_MESH", iTriggerMeshIndex ) )
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
		while( true == luaManager.BeginTable( L"TRIGGER_DATA_LINE", iTriggerLineIndex ) )
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
		while( true == luaManager.BeginTable( L"TRIGGER_DATA_ETC", iTriggerEtcIndex ) )
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
CXSLLineMap::LineAnimData::LineAnimData( LINE_ANIM_TYPE eAnimType )
: m_eAnimType( eAnimType )
, m_bEnabled( true )
, m_bMoveIfSteppedOn( false )
, m_vPivotPosition( 0, 0, 0 )
, m_vChildPivotPosition( 0, 0, 0 )
, m_vChildPivotPositionDisplace( 0, 0, 0 )	
, m_pLineData( NULL )
{
}

void CXSLLineMap::LineAnimData::OnFrameMove( double fTime, float fElapsedTime )
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
CXSLLineMap::LineAnimBackAndForth::LineAnimBackAndForth()
: LineAnimData( LAT_BACK_AND_FORTH )
, m_fCurrentPositionValue( 0.f )
{
}

void CXSLLineMap::LineAnimBackAndForth::OnFrameMove( double fTime, float fElapsedTime )
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
CXSLLineMap::LineAnimMoveIfSteppedOn::LineAnimMoveIfSteppedOn()
: LineAnimData( LAT_MOVE_IF_STEPPED_ON )
, m_bSteppedOnForward( false )
, m_bSteppedOnBackward( false )
, m_fCurrentMovingSpeed( 0.f )
, m_fCurrentPostionValue( 0.f )
{
}

void CXSLLineMap::LineAnimMoveIfSteppedOn::OnFrameMove( double fTime, float fElapsedTime )
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
CXSLLineMap::LineAnimViking::LineAnimViking()
: LineAnimData( LAT_VIKING )
, m_fCurrentTime( 0.f )
{
}

void CXSLLineMap::LineAnimViking::OnFrameMove( double fTime, float fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;
	if( m_fCurrentTime > m_fPeriod )
	{
		m_fCurrentTime -= m_fPeriod;
	}

	CalcLinePosition( m_fCurrentTime );
	LineAnimData::OnFrameMove( fTime, fElapsedTime );
}

void CXSLLineMap::LineAnimViking::CalcLinePosition( float fCurrentTime )
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

		CXSLLineMap::MakeLine( m_pLineData );	// line dir vector, direction degree 등을 갱신해주어야 한다
	}
}



//////////////////////////////////////////////////////////////////////////
CXSLLineMap::LineAnimCircle::LineAnimCircle()
: LineAnimData( LAT_CIRCLE )
, m_fCurrentRotationRadian( 0.f )
{
}

void CXSLLineMap::LineAnimCircle::OnFrameMove( double fTime, float fElapsedTime )
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



void CXSLLineMap::LineAnimCircle::CalcLinePosition( float fCurrentRotationRadian )
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
//CXSLLineMap::LineAnimRotateIfSteppedOn::LineAnimRotateIfSteppedOn()
//: LineAnimData( LAT_ROTATE_IF_STEPPED_ON )
//, m_bSteppedOnLeft( false )
//, m_bSteppedOnRight( false )
//, m_fCurrentRotationRadian( 0.f )
//{
//}
//
//void CXSLLineMap::LineAnimRotateIfSteppedOn::OnFrameMove( double fTime, float fElapsedTime )
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
	int CXSLLineMap::LineData::GetRandomLinkedLineIndex( int iRandomSeed /*= 0 */ )
	{
		if( m_vecLinkedLineIndex.empty() )
			return -1;

		//iRandomSeed = iRandomSeed % (int)m_vecLinkedLineIndex.size();
		// 이 함수 수정해야함 무조건 하나의 라인에는 하나의 linked line이 있는 것으로 하자

		//return m_vecLinkedLineIndex[ iRandomSeed ];
		return m_vecLinkedLineIndex[0];
	}
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST
#include "stdafx.h"
#include ".\x2camera.h"

CX2Camera::CX2Camera(void)
    : m_kCamera( g_pKTDXApp->GetDGManager()->GetCamera() )
#ifdef IN_GAME_MANUAL_CAMERA_POSITION_TEST
	, m_bEnabledManualCameraPosition( false )
	, m_vManualCameraPosition( 0, 0, 0 )
#endif IN_GAME_MANUAL_CAMERA_POSITION_TEST
{  
	m_CameraState		= CS_NORMAL;
	//m_pCamera			= g_pKTDXApp->GetDGManager()->GetCamera();
	m_fLand				= 0.0f;
	m_fAngleDegree		= 0.0f;
	m_fSetAngleDegree	= 0.0f;
	m_bBigAngle			= false;
	m_bRight			= true;

#ifdef BUBBLE_BOBBLE_TEST

	m_eArcadeCameraMode = ACM_UNIT_FOCUS;			

	m_vArcadeCameraLookAt	= D3DXVECTOR3(0, 0, 0);
	m_vArcadeCameraEye		= D3DXVECTOR3(0, 0, -100);

#endif BUBBLE_BOBBLE_TEST

#ifdef KEYFRAME_CAMERA
	m_bWorldCameraEdit				= false;
	m_bWorldCameraManualPlay		= false;

	m_bHoldViewWorldCamera			= false;
	m_fNowWorldCameraDuration			= 0.f;
	m_fNowWorldCameraElapsedTime		= 0.f;

	m_iEditKeyFrameIndex			= -1;

	m_bEnableWorldCamera			= false;


	m_pNowPlayingWorldCamera		= NULL;
	m_wstrNowWorldCameraFilename	= L"";
	m_vecWorldCameraData.clear();

	m_iNowKeyFrameIndex				= -1;
	m_fNowKeyFrameElapsedTime		= -1.f;
	m_fNowKeyFrameTotalTime			= 0.f;

	m_fNowKeyFrameMoveDistance		= 0.f;
	m_fNowKeyFrameTotalDistance		= 0.f;

	m_iNowLoopCount					= 0;

	m_iNowEyeInterPositionIndex		= 0;
	m_iNowEyeInterPositionMoveDistance	= 0.f;

	m_pDlgWorldCameraEdit			= NULL;


	m_pDlgWorldCameraEdit = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_World_Camera_Edit.lua" );

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgWorldCameraEdit );
	m_pDlgWorldCameraEdit->SetDisableUnderWindow(true);
	m_pDlgWorldCameraEdit->SetShowEnable(false, false);


	m_pStaticKeyFrameState		= static_cast< CKTDGUIStatic* >( m_pDlgWorldCameraEdit->GetControl( L"StaticKeyFrameState" ) );
	m_pStaticKeyFrameState->SetString(0, L"2");
	m_pStaticKeyFrameState->SetString(1, L"/45");


	CKTDGUIStatic* pStaticAngleType				= static_cast< CKTDGUIStatic* >( m_pDlgWorldCameraEdit->GetControl( L"StaticAngleType" ) );
	pStaticAngleType->SetString(0, L"Angle Interpolation");

	m_pStaticMoveVelocity		=  static_cast< CKTDGUIStatic* >( m_pDlgWorldCameraEdit->GetControl( L"StaticMoveVelo" ) );
	m_pStaticRotateTime		=  static_cast< CKTDGUIStatic* >( m_pDlgWorldCameraEdit->GetControl( L"StaticRotateVelo" ) );
	m_pStaticMoveVelocity->SetString(0, L"Mov Vel.");
	m_pStaticRotateTime->SetString(0, L"Rot Time");
	//m_pStaticRotateTime->SetShowEnable(false, false);

	m_pIMEEditMoveVelocity		=  static_cast< CKTDGUIIMEEditBox* >( m_pDlgWorldCameraEdit->GetControl( L"IMEEditMoveVelo" ) );
	m_pIMEEditRotateTime	=  static_cast< CKTDGUIIMEEditBox* >( m_pDlgWorldCameraEdit->GetControl( L"IMEEditRotateVelo" ) );


	m_pComboAngleInterpolation = static_cast< CKTDGUIComboBox* >( m_pDlgWorldCameraEdit->GetControl( L"ComboAngleInterpolation" ) );
	m_pComboAngleInterpolation->RemoveAllItems();
	m_pComboAngleInterpolation->AddItem(L"AIT_LINEAR", NULL, false);
	m_pComboAngleInterpolation->AddItem(L"AIT_LOOK", NULL, false);
	m_pComboAngleInterpolation->AddItem(L"AIT_FIXED", NULL, false);
	m_pComboAngleInterpolation->AddItem(L"AIT_TRACE", NULL, false);


	CKTDGUIStatic* pStaticKeyFrameButtonName = static_cast< CKTDGUIStatic* >( m_pDlgWorldCameraEdit->GetControl( L"StaticKeyFrameButtonName" ) );
	pStaticKeyFrameButtonName->SetString(0, L"ADD KeyFrame");
	pStaticKeyFrameButtonName->SetString(1, L"SAVE KeyFrame");
	pStaticKeyFrameButtonName->SetString(2, L"DEL KeyFrame");


	CKTDGUIStatic* pStaticFixedPosition = static_cast< CKTDGUIStatic* >( m_pDlgWorldCameraEdit->GetControl( L"StaticFixedPosition" ) );
	pStaticFixedPosition->SetString(0, L"Coord X");
	pStaticFixedPosition->SetString(1, L"Coord X");
	pStaticFixedPosition->SetString(2, L"Coord Y");

	m_pIMEEditCoordX	=  static_cast< CKTDGUIIMEEditBox* >( m_pDlgWorldCameraEdit->GetControl( L"IMEEditCoordX" ) );
	m_pIMEEditCoordY	=  static_cast< CKTDGUIIMEEditBox* >( m_pDlgWorldCameraEdit->GetControl( L"IMEEditCoordY" ) );
	m_pIMEEditCoordZ	=  static_cast< CKTDGUIIMEEditBox* >( m_pDlgWorldCameraEdit->GetControl( L"IMEEditCoordZ" ) );


#endif KEYFRAME_CAMERA

#ifdef NPC_TRACKINGCAMERA
	m_bNpcTrackingCamera = false;
	m_fTrackingTime = 0.f;
#endif

	m_iLastAngleInterpolationIndex = 0;
	m_iLastCoordX = 0;
	m_iLastCoordY = 0;
	m_iLastCoordZ = 0;
	m_iLastMoveVelocity = 0;
	m_iLastRotateTime = 0;
}

CX2Camera::~CX2Camera(void)
{
#ifdef KEYFRAME_CAMERA
	m_vecWorldCameraData.clear();
	SAFE_DELETE_DIALOG(m_pDlgWorldCameraEdit);
#endif KEYFRAME_CAMERA
}

HRESULT CX2Camera::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bBigAngle == true )
	{
		m_fAngleDegree += fElapsedTime*50.0f;
		if ( m_fAngleDegree >= m_fSetAngleDegree )
			m_fAngleDegree = m_fSetAngleDegree;
	}
	else
	{
		m_fAngleDegree -= fElapsedTime*50.0f;
		if ( m_fAngleDegree <= m_fSetAngleDegree )
			m_fAngleDegree = m_fSetAngleDegree;
	}

#ifdef NPC_TRACKINGCAMERA
	m_fTrackingTime -= fElapsedTime;
	if( m_bNpcTrackingCamera == true && m_fTrackingTime <= 0.f )
		m_bNpcTrackingCamera = false;
#endif

#ifdef KEYFRAME_CAMERA
	WorldCameraFrameMove( fTime, fElapsedTime );
#endif KEYFRAME_CAMERA
	m_LineScriptedCameraData.OnFrameMove( fTime, fElapsedTime );

	m_kCamera.UpdateCamera( fElapsedTime );

	return S_OK;
}

void CX2Camera::PlayLuaCamera( CX2GameUnit* pFocusUnit, KLuaManager& luaManger, int index )
{
	if( luaManger.BeginTable( index ) == true )
	{
		PlayLuaCamera( pFocusUnit, luaManger );
		luaManger.EndTable();
	}
}
void CX2Camera::PlayLuaCamera( CX2GameUnit* pFocusUnit, KLuaManager& luaManger, const char* pszTableNameUTF8 )
{
	if( luaManger.BeginTable( pszTableNameUTF8 ) == true )
	{
		// 캐릭터 left, right 방향에 따른 카메라 셋팅 설정
		bool bDirection = false;
		LUA_GET_VALUE(		luaManger, "Direction",		bDirection,		false );		


		if( bDirection == true )
		{
		    const char* pszDirectionCamera = "";
			if( pFocusUnit->GetIsRight() == true )
			{
				pszDirectionCamera = "RightDirCamear";
			}
			else
			{
				pszDirectionCamera = "LeftDirCamear";
			}

			if( luaManger.BeginTable( pszDirectionCamera ) == true )
			{
				PlayLuaCamera( pFocusUnit, luaManger );
				luaManger.EndTable();
			}
		}		
		else
		{
			PlayLuaCamera( pFocusUnit, luaManger );
		}

		luaManger.EndTable();
	}
}

void CX2Camera::PlayLuaCamera( CX2GameUnit* pFocusUnit, KLuaManager& luaManger )
{
#ifdef NPC_TRACKINGCAMERA
	if( m_bNpcTrackingCamera == true )
		return;
#endif
	CAMERA_TYPE		cameraType;
	LUA_GET_VALUE_ENUM( luaManger, "CAMERA_TYPE",		cameraType, CAMERA_TYPE, CT_NORMAL_TRACKING );
	
	float			distance;
	LUA_GET_VALUE(		luaManger, "DISTANCE",			distance,		g_pMain->GetGameOption().GetCameraDistance() );

	float			height;
	LUA_GET_VALUE(		luaManger, "HEIGHT",			height,			200.0f );

	float			angleDegree;
	LUA_GET_VALUE(		luaManger, "ANGLEDEGREE",		angleDegree,	0.0f );
	
	float			eyeDistance;
	LUA_GET_VALUE(		luaManger, "EYEDISTANCE",		eyeDistance,	0.0f );

	float			lookDistance;
	LUA_GET_VALUE(		luaManger, "LOOKATDISTANCE",	lookDistance,	0.0f );

	switch( cameraType )
	{
	case CT_NORMAL_TRACKING:
		{
			float			trackingTime;
			LUA_GET_VALUE( luaManger, "TRACKINGTIME", trackingTime,	0.3f );
			NomalTrackingCamera( pFocusUnit, distance, height, angleDegree, eyeDistance, lookDistance, trackingTime );
		}
		break;

	case CT_PARTS_LOOK_TRACKING:
		{
			D3DXVECTOR3 vLookPos( 0.0f, 0.0f, 0.0f );
			if ( ParsingLookTypeAndPos( luaManger, pFocusUnit, vLookPos ) )
			{
				float			fTrackingTime;
				LUA_GET_VALUE(		luaManger, "TRACKINGTIME",		fTrackingTime,	0.3f );

				EYE_POS_RELATIVE_UNIT eEysPosRelativeUnit;
				LUA_GET_VALUE_ENUM( luaManger, "EYE_POS_RELATIVE_UNIT",		eEysPosRelativeUnit, EYE_POS_RELATIVE_UNIT, EPRU_FOUCS_UNIT );

				D3DXVECTOR3 vEyePos( 0.0f, 0.0f, 0.0f );
				if( EPRU_MY_UNIT == eEysPosRelativeUnit && ParsingEyeTypeAndPos( luaManger, g_pX2Game->GetMyUnit(), vEyePos ) )
					PartsLookTrackingCameraFromPartsEye( pFocusUnit, vLookPos, vEyePos, fTrackingTime );
				else if ( ParsingEyeTypeAndPos( luaManger, pFocusUnit, vEyePos ) )
					PartsLookTrackingCameraFromPartsEye( pFocusUnit, vLookPos, vEyePos, fTrackingTime );
				else
					PartsLookTrackingCamera( pFocusUnit, vLookPos, distance, height, angleDegree, eyeDistance, lookDistance, fTrackingTime );
			}			
		}
		break;

	case CT_NORMAL_DIRECT:
		{
			NomalDirectCamera( pFocusUnit, distance, height, angleDegree, eyeDistance, lookDistance );
		}
		break;

	case CT_PARTS_LOOK_DIRECT:
		{

			D3DXVECTOR3 vLookPos( 0.0f, 0.0f, 0.0f );
			if ( ParsingLookTypeAndPos( luaManger, pFocusUnit, vLookPos ) )
			{
				EYE_POS_RELATIVE_UNIT eEysPosRelativeUnit;
				LUA_GET_VALUE_ENUM( luaManger, "EYE_POS_RELATIVE_UNIT",		eEysPosRelativeUnit, EYE_POS_RELATIVE_UNIT, EPRU_FOUCS_UNIT );

				D3DXVECTOR3 vEyePos( 0.0f, 0.0f, 0.0f );
				if( EPRU_MY_UNIT == eEysPosRelativeUnit && ParsingEyeTypeAndPos( luaManger, g_pX2Game->GetMyUnit(), vEyePos ) )
					PartsLookDirectCameraFromPartsEye( pFocusUnit, vLookPos, vEyePos );
				else if ( ParsingEyeTypeAndPos( luaManger, pFocusUnit, vEyePos ) )
					PartsLookDirectCameraFromPartsEye( pFocusUnit, vLookPos, vEyePos );
				else
					PartsLookDirectCamera( pFocusUnit, vLookPos, distance, height, angleDegree, eyeDistance, lookDistance );
			}
		}
		break;


	case CT_BONE_ATTACHED:
		{
#ifdef HEAD_ATTACHED_CAMERA_TEST

			if( CX2GameUnit::GUT_USER == pFocusUnit->GetGameUnitType() )
			{
				std::wstring boneName;
				LUA_GET_VALUE(		luaManger, "BONE_NAME",		boneName,		L"" );

				D3DXVECTOR3 vOffset(0, 0, 0);
				LUA_GET_VALUE(		luaManger, "OFFSET_X",		vOffset.x,		0.f );
				LUA_GET_VALUE(		luaManger, "OFFSET_Y",		vOffset.y,		0.f );
				LUA_GET_VALUE(		luaManger, "OFFSET_Z",		vOffset.z,		0.f );


				CX2GUUser* pUser = (CX2GUUser*) pFocusUnit;
				pUser->BoneAttachedCamera( boneName, vOffset );
			}

#endif HEAD_ATTACHED_CAMERA_TEST
		} break;
	}
}


#ifdef IN_GAME_MANUAL_CAMERA_POSITION_TEST

void CX2Camera::CalcManualCameraPosition( CX2GameUnit* pFocusUnit, const D3DXVECTOR3& vEyePosition )
{
	if( pFocusUnit == NULL )
		return;

	D3DXVECTOR3 vRelativeEyePos = vEyePosition - pFocusUnit->GetPos();

	D3DXVECTOR3 vDirVector = pFocusUnit->GetDirVector();
	D3DXVECTOR3 vZVector = pFocusUnit->GetZVector();
	D3DXVec3Normalize( &vDirVector, &vDirVector );
	D3DXVec3Normalize( &vZVector, &vZVector );


	m_vManualCameraPosition.x = D3DXVec3Dot( &vDirVector, &vRelativeEyePos );
	m_vManualCameraPosition.y = vRelativeEyePos.y;
	m_vManualCameraPosition.z = D3DXVec3Dot( &vZVector, &vRelativeEyePos );
}


void CX2Camera::ManualTrackingCamera( CX2GameUnit* pFocusUnit, float distance, const D3DXVECTOR3& vRelativeEyePos )
{
	m_CameraState	= CS_NORMAL;

	if( pFocusUnit == NULL )
		return;

	float fCameraHeightCompensation = 400.f;

	if( g_pX2Game != NULL 		
#ifdef PVP_ZOOM_CAMERA_1
		&& (g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_PVP ) )
#else
		&& ( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_BATTLE_FIELD ) )
#endif
	{
		if( distance <= 800.f )
		{
			fCameraHeightCompensation = 100.f;
		}
		if( distance <= 900.f )
		{
			fCameraHeightCompensation = 200.f;
		}
		else if( distance <= 1200.f )
		{
			fCameraHeightCompensation = 300.f;
		}
	}


	D3DXVECTOR3 lookAt = pFocusUnit->GetPos();
	lookAt.y += 100.0f;

	D3DXVECTOR3 eye = lookAt;
	eye += pFocusUnit->GetDirVector() * vRelativeEyePos.x;
	eye.y += vRelativeEyePos.y;
	eye += pFocusUnit->GetZVector() * vRelativeEyePos.z;

	if( eye.y < m_fLand + fCameraHeightCompensation )
		eye.y = m_fLand + fCameraHeightCompensation;
	if( lookAt.y < m_fLand + fCameraHeightCompensation )
		lookAt.y = m_fLand + fCameraHeightCompensation;

	m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, 0.3f );
}

#endif IN_GAME_MANUAL_CAMERA_POSITION_TEST







void CX2Camera::NomalTrackingCamera( CX2GameUnit* pFocusUnit, float distance, float height, float angleDegree, float eyeDistance, 
	float lookatDistance, float trackingTime, float trackingAtTime )
{
#ifdef NPC_TRACKINGCAMERA
	if( m_bNpcTrackingCamera == true )
		return;
#endif

#ifdef IN_GAME_MANUAL_CAMERA_POSITION_TEST
	if( true == m_bEnabledManualCameraPosition )
	{
		ManualTrackingCamera( pFocusUnit, distance, m_vManualCameraPosition );
		return;
	}
#endif IN_GAME_MANUAL_CAMERA_POSITION_TEST


	m_CameraState	= CS_NORMAL;

	if( pFocusUnit == NULL )
		return;

	float fCameraHeightCompensation = 400.f;

	if( g_pX2Game != NULL 		
#ifdef PVP_ZOOM_CAMERA_1
		&& (g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_PVP ) )
#else
		&& ( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_BATTLE_FIELD ) )
#endif PVP_ZOOM_CAMERA
	{
		if( distance <= 800.f )
		{
			fCameraHeightCompensation = 100.f;
		}
		else if( distance <= 900.f )
		{
			fCameraHeightCompensation = 200.f;
		}
		else if( distance <= 1200.f )
		{
			fCameraHeightCompensation = 300.f;
		}
	}

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;

	lookAt = pFocusUnit->GetPos();
	lookAt.y += 100.0f;
#ifdef DASH_CAMERA_ANGLE_TEST
	//{{AFX
	if ( m_fSetAngleDegree != angleDegree )
	{
		m_fSetAngleDegree = angleDegree;

		if ( m_fAngleDegree < m_fSetAngleDegree )
		{
			m_bBigAngle = true;
		}
		else
		{
			m_bBigAngle = false;
		}
	}
	//}}AFX

#endif //DASH_CAMERA_ANGLE_TEST
	if( pFocusUnit->GetIsRight() == true )
	{
#ifdef DASH_CAMERA_ANGLE_TEST
		if ( m_bRight == false )
		{
			m_fSetAngleDegree = (-m_fSetAngleDegree);
			m_fAngleDegree = (-m_fAngleDegree);

			if ( m_fAngleDegree < m_fSetAngleDegree )
			{
				m_bBigAngle = true;
			}
			else
			{
				m_bBigAngle = false;
			}
		}
#endif //DASH_CAMERA_ANGLE_TEST
		m_bRight = true;
		lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
#ifdef DASH_CAMERA_ANGLE_TEST
		//{{AFX
		eye.x = pFocusUnit->GetXPos() + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + m_fAngleDegree) ) * -distance;
		eye.y = pFocusUnit->GetYPos() + height; 
		eye.z = pFocusUnit->GetZPos() + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + m_fAngleDegree) ) * -distance;
		//}}AFX

#else //DASH_CAMERA_ANGLE_TEST
		eye.x = pFocusUnit->GetXPos() + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetYPos() + height; 
		eye.z = pFocusUnit->GetZPos() + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
#endif //DASH_CAMERA_ANGLE_TEST
		eye += (pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		if ( m_bRight == true )
		{
#ifdef DASH_CAMERA_ANGLE_TEST
			//{{AFX
			m_fSetAngleDegree = (-m_fSetAngleDegree);
			m_fAngleDegree = (-m_fAngleDegree);

			if ( m_fAngleDegree < m_fSetAngleDegree )
			{
				m_bBigAngle = true;
			}
			else
			{
				m_bBigAngle = false;
			}
			//}}AFX

#endif //DASH_CAMERA_ANGLE_TEST
		}
		m_bRight = false;
		lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);	
#ifdef DASH_CAMERA_ANGLE_TEST
		//{{AFX
		eye.x = pFocusUnit->GetXPos() + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - m_fAngleDegree) ) * -distance;
		eye.y = pFocusUnit->GetYPos() + height; 
		eye.z = pFocusUnit->GetZPos() + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - m_fAngleDegree) ) * -distance;
		//}}AFX

#else //DASH_CAMERA_ANGLE_TEST
		eye.x = pFocusUnit->GetXPos() + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetYPos() + height; 
		eye.z = pFocusUnit->GetZPos() + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
#endif //DASH_CAMERA_ANGLE_TEST
		eye += (pFocusUnit->GetDirVector() * -eyeDistance);
	}




#ifdef CAMERA_SCRIPTING_TEST
	eye += pFocusUnit->GetDirVector() * m_LineScriptedCameraData.m_vCurrRelativeEyePosition.x;
	eye.y += m_LineScriptedCameraData.m_vCurrRelativeEyePosition.y;
	eye += pFocusUnit->GetZVector() * m_LineScriptedCameraData.m_vCurrRelativeEyePosition.z;
#endif CAMERA_SCRIPTING_TEST





	if( eye.y < m_fLand + fCameraHeightCompensation )
		eye.y = m_fLand + fCameraHeightCompensation;
	if( lookAt.y < m_fLand + fCameraHeightCompensation )
		lookAt.y = m_fLand + fCameraHeightCompensation;
	m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime, trackingAtTime );
}
#ifdef MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
void CX2Camera::NomalTrackingCamera( CX2SquareUnit* pFocusUnit, float distance, float height, float angleDegree, float eyeDistance, 
	float lookatDistance, float trackingTime, float trackingAtTime )
{
	m_CameraState	= CS_NORMAL;

	if( pFocusUnit == NULL )
		return;

	float fCameraHeightCompensation = 400.f;

	if( distance <= 800.f )
		fCameraHeightCompensation = 100.f;
	else if( distance <= 900.f )
		fCameraHeightCompensation = 200.f;
	else if( distance <= 1200.f )
		fCameraHeightCompensation = 300.f;

	D3DXVECTOR3 eye(0,0,0);
	D3DXVECTOR3 lookAt(0,0,0);

	lookAt = pFocusUnit->GetPos();
	lookAt.y += 100.0f;
	
	D3DXVECTOR3 DirDegree = pFocusUnit->GetDirDegree();

	if( pFocusUnit->GetIsRight() == true )
	{
		m_bRight = true;
		lookAt += (pFocusUnit->GetDirVector() * lookatDistance);

		eye.x = pFocusUnit->GetXPos() + sinf( D3DXToRadian(DirDegree.y + angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetYPos() + height; 
		eye.z = pFocusUnit->GetZPos() + cosf( D3DXToRadian(DirDegree.y + angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		m_bRight = false;
		lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);	

		eye.x = pFocusUnit->GetXPos() + sinf( D3DXToRadian(DirDegree.y + 180.0f - angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetYPos() + height; 
		eye.z = pFocusUnit->GetZPos() + cosf( D3DXToRadian(DirDegree.y + 180.0f - angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * -eyeDistance);
	}

#ifdef CAMERA_SCRIPTING_TEST
	eye += pFocusUnit->GetDirVector() * m_LineScriptedCameraData.m_vCurrRelativeEyePosition.x;
	eye.y += m_LineScriptedCameraData.m_vCurrRelativeEyePosition.y;
	eye += pFocusUnit->GetZVector() * m_LineScriptedCameraData.m_vCurrRelativeEyePosition.z;
#endif CAMERA_SCRIPTING_TEST

	if( eye.y < m_fLand + fCameraHeightCompensation )
		eye.y = m_fLand + fCameraHeightCompensation;
	if( lookAt.y < m_fLand + fCameraHeightCompensation )
		lookAt.y = m_fLand + fCameraHeightCompensation;
	m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime, trackingAtTime );
}
#endif //MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
void CX2Camera::PartsLookTrackingCamera( CX2GameUnit* pFocusUnit, D3DXVECTOR3 look, float distance, float height, float angleDegree, float eyeDistance, float lookatDistance, float trackingTime )
{
	m_CameraState	= CS_EVENT;

	if( pFocusUnit == NULL )
		return;

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt = look;

	if( pFocusUnit->GetIsRight() == true )
	{
		m_bRight = true;
		lookAt	+=	(pFocusUnit->GetDirVector() * lookatDistance);
		eye.x	=	look.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
		eye.y	=	look.y + height; 
		eye.z	=	look.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
		eye		+=	(pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		m_bRight = false;
		lookAt	+=	(pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x	=	look.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
		eye.y	=	look.y + height; 
		eye.z	=	look.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
		eye		+=	(pFocusUnit->GetDirVector() * -eyeDistance);
	}
	m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime );
}

void CX2Camera::NomalDirectCamera( CX2GameUnit* pFocusUnit, float distance, float height, float angleDegree, float eyeDistance, float lookatDistance )
{
#ifdef NPC_TRACKINGCAMERA
	if( m_bNpcTrackingCamera == true )
		return;
#endif

	m_CameraState	= CS_EVENT;

	if( pFocusUnit == NULL )
		return;

	float fCameraHeightCompensation = 400.f;

	if( g_pX2Game != NULL 
		&& ( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_BATTLE_FIELD ) )
	{
		if( distance <= 800.f )
		{
			fCameraHeightCompensation = 100.f;
		}
		if( distance <= 900.f )
		{
			fCameraHeightCompensation = 200.f;
		}
		else if( distance <= 1200.f )
		{
			fCameraHeightCompensation = 300.f;
		}
	}

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;

	lookAt = pFocusUnit->GetPos();
	lookAt.y += 100.0f;
	if( pFocusUnit->GetIsRight() == true )
	{
		m_bRight = true;
		lookAt	+=	(pFocusUnit->GetDirVector() * lookatDistance);
		eye.x	=	pFocusUnit->GetXPos() + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
		eye.y	=	pFocusUnit->GetYPos() + height; 
		eye.z	=	pFocusUnit->GetZPos() + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
		eye		+=	(pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		m_bRight = false;
		lookAt	+=	(pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x	=	pFocusUnit->GetXPos() + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
		eye.y	=	pFocusUnit->GetYPos() + height; 
		eye.z	=	pFocusUnit->GetZPos() + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
		eye		+=	(pFocusUnit->GetDirVector() * -eyeDistance);
	}
	if( eye.y < m_fLand + fCameraHeightCompensation )
		eye.y = m_fLand + fCameraHeightCompensation;
	if( lookAt.y < m_fLand + fCameraHeightCompensation )
		lookAt.y = m_fLand + fCameraHeightCompensation;
	m_kCamera.Point( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z );
}





void CX2Camera::PartsLookDirectCamera( CX2GameUnit* pFocusUnit, D3DXVECTOR3 look, float distance, float height, float angleDegree, float eyeDistance, float lookatDistance )
{
#ifdef NPC_TRACKINGCAMERA
	if( m_bNpcTrackingCamera == true )
		return;
#endif

	m_CameraState	= CS_EVENT;

	if( pFocusUnit == NULL )
		return;

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt = look;

	if( pFocusUnit->GetIsRight() == true )
	{
		m_bRight = true;
		lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
		eye.x = look.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
		eye.y = look.y + height; 
		eye.z = look.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		m_bRight = false;
		lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x = look.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
		eye.y = look.y + height; 
		eye.z = look.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * -eyeDistance);
	}
	m_kCamera.Point( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z );
}

void CX2Camera::FieldCamera( CX2SquareUnit* pFocusUnit, float distance, float lookatheight, float eyeHeight, float angleDegree, float trackingTime )
{	
	float lookatDistance = 0.0f;
	float eyeDistance = 0.0f;


	m_CameraState	= CS_NORMAL;

	if( pFocusUnit == NULL )
		return;

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;	

	lookAt = pFocusUnit->GetPos();
	lookAt.y += lookatheight;

	if( pFocusUnit->GetIsRight() == true )
	{
		m_bRight = true;
		lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + eyeHeight; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		m_bRight = false;
		lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + eyeHeight; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * -eyeDistance);
	}
	if( eye.y < m_fLand + 200.0f )
		eye.y = m_fLand + 200.0f;
	if( lookAt.y < m_fLand + 200.0f )
		lookAt.y = m_fLand + 200.0f;
	m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime );
}



void CX2Camera::SquareCamera( CX2SquareUnit* pFocusUnit, float distance, float eyeHeight, float lookAtHeight, float angleDegree, float eyeDistance,
	float lookatDistance, float trackingTime )
{
	m_CameraState	= CS_NORMAL;

	if( pFocusUnit == NULL )
		return;

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;

	lookAt = pFocusUnit->GetPos();

#ifdef NEW_VILLAGE_RENDERING_TEST
	lookAt.y += lookAtHeight;
#else
	lookAt.y += 100.0f;
#endif NEW_VILLAGE_RENDERING_TEST


	if( pFocusUnit->GetIsRight() == true )
	{
		m_bRight = true;
		lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + eyeHeight; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		m_bRight = false;
		lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + eyeHeight; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * -eyeDistance);
	}
	if( eye.y < m_fLand + 200.0f )
		eye.y = m_fLand + 200.0f;
	if( lookAt.y < m_fLand + 200.0f )
		lookAt.y = m_fLand + 200.0f;
	m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime );
}//CX2Camera::SquareCamera()

void CX2Camera::SquareCamera( CX2SquareUnit* pFocusUnit, D3DXVECTOR3 vLookAt, float distance, float height, float angleDegree, float eyeDistance, float lookatDistance, float trackingTime )
{
	m_CameraState	= CS_NORMAL;


	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;

	lookAt = vLookAt;

#ifdef NEW_VILLAGE_RENDERING_TEST
	lookAt.y += 250.f;
#else
	lookAt.y += 100.0f;
#endif NEW_VILLAGE_RENDERING_TEST


	if( pFocusUnit->GetIsRight() == true )
	{
		m_bRight = true;
		lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + height; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		m_bRight = false;
		lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + height; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * -eyeDistance);
	}
	if( eye.y < m_fLand + 200.0f )
		eye.y = m_fLand + 200.0f;
	if( lookAt.y < m_fLand + 200.0f )
		lookAt.y = m_fLand + 200.0f;
	m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime );
}//CX2Camera::SquareCamera()

//{{ seojt // 2008-10-23, 15:44
void CX2Camera::SquareCameraUpdate( CX2SquareUnit* pFocusUnit, float distance, float height, float angleDegree, float eyeDistance, float lookatDistance )
{
	m_CameraState	= CS_NORMAL;

	if( pFocusUnit == NULL )
		return;

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;

	lookAt = pFocusUnit->GetPos();

#ifdef NEW_VILLAGE_RENDERING_TEST
	lookAt.y += 300.f;
#else
	lookAt.y += 100.0f;
#endif NEW_VILLAGE_RENDERING_TEST


	if( pFocusUnit->GetIsRight() == true )
	{
		m_bRight = true;
		lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + height; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		m_bRight = false;
		lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + height; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * -eyeDistance);
	}
	if( eye.y < m_fLand + 200.0f )
		eye.y = m_fLand + 200.0f;
	if( lookAt.y < m_fLand + 200.0f )
		lookAt.y = m_fLand + 200.0f;

	//m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime );

	//D3DXVECTOR3 vUp( 0, 1, 0 );
	m_kCamera.UpdateViewTransform( eye, lookAt );
}//CX2Camera::SquareCameraUpdate()

void CX2Camera::TFieldCameraUpdate( CX2SquareUnit* pFocusUnit, float distance, float lookatHeight, float height, float angleDegree, float eyeDistance, float lookatDistance )
{
	m_CameraState	= CS_NORMAL;

	if( pFocusUnit == NULL )
		return;

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;

	lookAt = pFocusUnit->GetPos();
	lookAt.y += lookatHeight;

	if( pFocusUnit->GetIsRight() == true )
	{
		m_bRight = true;
		lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + height; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		m_bRight = false;
		lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + height; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * -eyeDistance);
	}
	if( eye.y < m_fLand + 200.0f )
		eye.y = m_fLand + 200.0f;
	if( lookAt.y < m_fLand + 200.0f )
		lookAt.y = m_fLand + 200.0f;

	//m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime );

	//D3DXVECTOR3 vUp( 0, 1, 0 );
	m_kCamera.UpdateViewTransform( eye, lookAt );
}//CX2Camera::TFieldCameraUpdate()
//}} seojt // 2008-10-23, 15:44

#ifdef NEW_VILLAGE_RENDERING_TEST

void CX2Camera::SquareLineCamera( CX2SquareUnit* pFocusUnit, const D3DXVECTOR3& vEye, const D3DXVECTOR3& vLookAt, 
	bool bFocusOnUnit /*= true*/, float trackingTime /*= 0.5f*/  )
{
	m_CameraState	= CS_NORMAL;

	
	D3DXVECTOR3 eye = vEye;
	D3DXVECTOR3 lookAt = vLookAt;

	if( true == bFocusOnUnit )
	{
		if( NULL != pFocusUnit )
			lookAt = pFocusUnit->GetPos();
	}

	lookAt.y += 300.f;


	if( NULL != pFocusUnit && true == pFocusUnit->GetIsRight() )
	{
		m_bRight = true;
	}
	else
	{
		m_bRight = false;
	}

	if( eye.y < m_fLand + 200.0f )
		eye.y = m_fLand + 200.0f;

	if( lookAt.y < m_fLand + 200.0f )
		lookAt.y = m_fLand + 200.0f;

	m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime );
}

#endif NEW_VILLAGE_RENDERING_TEST

void CX2Camera::GameLineCamera( CX2GUUser* pFocusUnit, const D3DXVECTOR3& vEye, const D3DXVECTOR3& vLookAt, 
	bool bFocusOnUnit /*= true*/, float trackingTime /*= 0.5f*/  )
{
#ifdef NPC_TRACKINGCAMERA
	if( m_bNpcTrackingCamera == true )
		return;
#endif

	m_CameraState	= CS_NORMAL;

	if( pFocusUnit == NULL )
		return;

	D3DXVECTOR3 eye = vEye;
	D3DXVECTOR3 lookAt = vLookAt;

	if( true == bFocusOnUnit )
	{
		lookAt = pFocusUnit->GetPos();
	}

	lookAt.y += 300.f;


	if( pFocusUnit->GetIsRight() == true )
	{
		m_bRight = true;
	}
	else
	{
		m_bRight = false;
	}

	if( eye.y < m_fLand + 200.0f )
		eye.y = m_fLand + 200.0f;

	if( lookAt.y < m_fLand + 200.0f )
		lookAt.y = m_fLand + 200.0f;

	m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime );
}


#ifdef BUBBLE_BOBBLE_TEST

void CX2Camera::FixedCamera( CX2GameUnit* pFocusUnit, D3DXVECTOR3 vLookAt, D3DXVECTOR3 vEye, 
	float fXMin, float fXMax, float fYMin, float fYMax, 
	float distance, float height, float angleDegree, float eyeDistance, float lookatDistance, bool bDirect )
{
	m_CameraState	= CS_NORMAL;


	float fZoomScale = 1.f;

	// note!! 아케이드 모드에서는 카메라 zoom in/out 안되게
	//switch( g_pMain->GetGameOption().GetOptionList().m_iZoomLevel )
	//{
	//default:
	//case 0:
	//	fZoomScale = 1.f;
	//	break;

	//case 1:
	//	fZoomScale = 1.7f / 2.f;
	//	break;

	//case -1:
	//	fZoomScale = 2.5f / 2.f;
	//	break;
	//}

	const float MAGIC_HORIZONTAL_THRESHOLD1 	= 600.f * fZoomScale;
	const float MAGIC_HORIZONTAL_THRESHOLD2 	= 800.f * fZoomScale;
	const float MAGIC_VERTICAL_THRESHOLD1		= 400.f * fZoomScale;
	const float MAGIC_VERTICAL_THRESHOLD2		= 500.f * fZoomScale;

	const float MAGIC_HORIZONTAL_TRACKING_TIME1 = 2.f;
	const float MAGIC_HORIZONTAL_TRACKING_TIME2 = 0.5f;
	const float MAGIC_VERTICAL_TRACKING_TIME1	= 1.f;
	const float MAGIC_VERTICAL_TRACKING_TIME2	= 0.2f;

	const float MAGIC_CAMERA_MARGIN_HORIZONTAL	= 900.f * fZoomScale;
	const float MAGIC_CAMERA_MARGIN_VERTICAL	= 600.f * fZoomScale;

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 vUnitPos	= m_vArcadeCameraLookAt;
	if( NULL != pFocusUnit )
	{
		vUnitPos = pFocusUnit->GetPos();
		vUnitPos.y += 100.f; // MAGIC
	}

	switch( m_eArcadeCameraMode )
	{
	case ACM_FIXED:
		{
			lookAt = vLookAt;

			if( pFocusUnit->GetIsRight() == true )
			{
				lookAt	+=	(pFocusUnit->GetDirVector() * lookatDistance);
				eye.x	=	vEye.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
				eye.y	=	vEye.y + height; 
				eye.z	=	vEye.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
				eye		+=	(pFocusUnit->GetDirVector() * eyeDistance);
			}
			else
			{
				lookAt	+=	(pFocusUnit->GetDirVector() * -lookatDistance);
				eye.x	=	vEye.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
				eye.y	=	vEye.y + height; 
				eye.z	=	vEye.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
				eye		+=	(pFocusUnit->GetDirVector() * -eyeDistance);
			}

			m_vArcadeCameraLookAt	= lookAt;
			m_vArcadeCameraEye		= eye;
			m_kCamera.Point( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z );

		} break;

	case ACM_FULL_SCROLL:
		{
			lookAt = m_vArcadeCameraLookAt;
			float fTrackingTime = 2.f;
			float fDistX = fabs(m_vArcadeCameraLookAt.x - vUnitPos.x );
			float fDistY = fabs(m_vArcadeCameraLookAt.y - vUnitPos.y );

			// horizontal scroll
			if( fDistX > MAGIC_HORIZONTAL_THRESHOLD1 )
			{
				lookAt.x = vUnitPos.x;
				fTrackingTime = MAGIC_HORIZONTAL_TRACKING_TIME1;

				if( fDistX > MAGIC_HORIZONTAL_THRESHOLD2 )
				{
					fTrackingTime = MAGIC_HORIZONTAL_TRACKING_TIME2;
				}
			}

			// vertical scroll
			if( fDistY > MAGIC_VERTICAL_THRESHOLD1 )
			{
				lookAt.y = vUnitPos.y;
				fTrackingTime = MAGIC_VERTICAL_TRACKING_TIME1;

				if( fDistY > MAGIC_VERTICAL_THRESHOLD2 )
				{
					fTrackingTime = MAGIC_VERTICAL_TRACKING_TIME2;
				}
			}

			lookAt.x = max( lookAt.x, fXMin+MAGIC_CAMERA_MARGIN_HORIZONTAL );
			lookAt.x = min( lookAt.x, fXMax-MAGIC_CAMERA_MARGIN_HORIZONTAL );
			lookAt.y = min( lookAt.y, fYMax );
			lookAt.y = max( lookAt.y, fYMin+MAGIC_CAMERA_MARGIN_VERTICAL );



			if( pFocusUnit->GetIsRight() == true )
			{
				lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
				eye.x = lookAt.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
				eye.y = lookAt.y + height; 
				eye.z = lookAt.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance; 
				eye += (pFocusUnit->GetDirVector() * eyeDistance);
			}
			else
			{
				lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
				eye.x = lookAt.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
				eye.y = lookAt.y + height; 
				eye.z = lookAt.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
				eye += (pFocusUnit->GetDirVector() * -eyeDistance);
			}


			m_vArcadeCameraLookAt	= lookAt;
			m_vArcadeCameraEye		= eye;
			if( true == bDirect )
			{
				m_kCamera.Point( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z );
			}
			else
			{
				m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, fTrackingTime );
			}

		} break;

	case ACM_HORIZONTAL_SCROLL:
		{
			lookAt = m_vArcadeCameraLookAt;
			float fTrackingTime = 2.f;
			float fDistX = fabs(m_vArcadeCameraLookAt.x - vUnitPos.x );
			float fDistY = fabs(m_vArcadeCameraLookAt.y - vUnitPos.y );

			// horizontal scroll
			if( fDistX > MAGIC_HORIZONTAL_THRESHOLD1 )
			{
				lookAt.x = vUnitPos.x;
				fTrackingTime = MAGIC_HORIZONTAL_TRACKING_TIME1;

				if( fDistX > MAGIC_HORIZONTAL_THRESHOLD2 )
				{
					fTrackingTime = MAGIC_HORIZONTAL_TRACKING_TIME2;
				}
			}

			lookAt.x = max( lookAt.x, fXMin+MAGIC_CAMERA_MARGIN_HORIZONTAL );
			lookAt.x = min( lookAt.x, fXMax-MAGIC_CAMERA_MARGIN_HORIZONTAL );
			lookAt.y = min( lookAt.y, fYMax );
			lookAt.y = max( lookAt.y, fYMin+MAGIC_CAMERA_MARGIN_VERTICAL );


			if( pFocusUnit->GetIsRight() == true )
			{
				lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
				eye.x = lookAt.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
				eye.y = lookAt.y + height; 
				eye.z = lookAt.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance; 
				eye += (pFocusUnit->GetDirVector() * eyeDistance);
			}
			else
			{
				lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
				eye.x = lookAt.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
				eye.y = lookAt.y + height; 
				eye.z = lookAt.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance; 
				eye += (pFocusUnit->GetDirVector() * -eyeDistance);
			}

			m_vArcadeCameraLookAt	= lookAt;
			m_vArcadeCameraEye		= eye;
			if( true == bDirect )
			{
				m_kCamera.Point( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z );
			}
			else
			{
				m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, fTrackingTime );
			}
		} break;

	case ACM_VERTICAL_SCROLL:
		{
			lookAt = m_vArcadeCameraLookAt;
			float fTrackingTime = 2.f;
			float fDistX = fabs(m_vArcadeCameraLookAt.x - vUnitPos.x );
			float fDistY = fabs(m_vArcadeCameraLookAt.y - vUnitPos.y );

			// vertical scroll
			if( fDistY > MAGIC_VERTICAL_THRESHOLD1 )
			{
				lookAt.y = vUnitPos.y;
				fTrackingTime = MAGIC_VERTICAL_TRACKING_TIME1;

				if( fDistY > MAGIC_VERTICAL_THRESHOLD2 )
				{
					fTrackingTime = MAGIC_VERTICAL_TRACKING_TIME2;
				}
			}

			lookAt.x = max( lookAt.x, fXMin+MAGIC_CAMERA_MARGIN_HORIZONTAL );
			lookAt.x = min( lookAt.x, fXMax-MAGIC_CAMERA_MARGIN_HORIZONTAL );
			lookAt.y = min( lookAt.y, fYMax );
			lookAt.y = max( lookAt.y, fYMin+MAGIC_CAMERA_MARGIN_VERTICAL );

			if( pFocusUnit->GetIsRight() == true )
			{
				lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
				eye.x = lookAt.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
				eye.y = lookAt.y + height; 
				eye.z = lookAt.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance; 
				eye += (pFocusUnit->GetDirVector() * eyeDistance);
			}
			else
			{
				lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
				eye.x = lookAt.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
				eye.y = lookAt.y + height; 
				eye.z = lookAt.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance; 
				eye += (pFocusUnit->GetDirVector() * -eyeDistance);
			}

			m_vArcadeCameraLookAt	= lookAt;
			m_vArcadeCameraEye		= eye;
			if( true == bDirect )
			{
				m_kCamera.Point( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z );
			}
			else
			{
				m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, fTrackingTime );
			}



		} break;

	case ACM_UNIT_FOCUS:
		{
			lookAt = m_vArcadeCameraLookAt;
			lookAt.x = vUnitPos.x;
			lookAt.y = vUnitPos.y;


			lookAt.x = max( lookAt.x, fXMin+MAGIC_CAMERA_MARGIN_HORIZONTAL );
			lookAt.x = min( lookAt.x, fXMax-MAGIC_CAMERA_MARGIN_HORIZONTAL );
			lookAt.y = min( lookAt.y, fYMax );
			lookAt.y = max( lookAt.y, fYMin+MAGIC_CAMERA_MARGIN_VERTICAL );


			float fTrackingTime = MAGIC_VERTICAL_TRACKING_TIME1;

			if( pFocusUnit->GetIsRight() == true )
			{
				lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
				eye.x = lookAt.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
				eye.y = lookAt.y + height; 
				eye.z = lookAt.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance; 
				eye += (pFocusUnit->GetDirVector() * eyeDistance);
			}
			else
			{
				lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
				eye.x = lookAt.x + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
				eye.y = lookAt.y + height; 
				eye.z = lookAt.z + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance; 
				eye += (pFocusUnit->GetDirVector() * -eyeDistance);
			}

			m_vArcadeCameraLookAt	= lookAt;
			m_vArcadeCameraEye		= eye;
			if( true == bDirect )
			{
				m_kCamera.Point( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z );
			}
			else
			{
				m_kCamera.SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, fTrackingTime );
			}

		} break;
	}
}

#endif BUBBLE_BOBBLE_TEST





#ifdef HEAD_ATTACHED_CAMERA_TEST

void CX2Camera::ManualDirectCamera( const D3DXVECTOR3& vEye, const D3DXVECTOR3& vLookAt )
{
	m_CameraState = CS_EVENT;

	m_kCamera.Point( vEye.x, vEye.y, vEye.z, vLookAt.x, vLookAt.y, vLookAt.z );
	//m_kCamera.SetTracking( vEye.x, vEye.y, vEye.z, vLookAt.x, vLookAt.y, vLookAt.z, 0.3f );
}


#endif HEAD_ATTACHED_CAMERA_TEST 




















#ifdef CAMERA_SCRIPTING_TEST
const D3DXVECTOR3 CX2Camera::CalcNormalCameraPosition( CX2GameUnit* pFocusUnit, float distance, float angleDegree, float lookatDistance, float height, float eyeDistance )
{
	if( pFocusUnit == NULL )
		return D3DXVECTOR3( 0, 0, 0 );

	float fCameraHeightCompensation = 400.f;

	if( g_pX2Game != NULL 		
#ifdef PVP_ZOOM_CAMERA_1
		&& (g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_PVP ) )
#else
		&& ( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_BATTLE_FIELD ) )
#endif PVP_ZOOM_CAMERA
	{
		if( distance <= 800.f )
		{
			fCameraHeightCompensation = 100.f;
		}
		if( distance <= 900.f )
		{
			fCameraHeightCompensation = 200.f;
		}
		else if( distance <= 1200.f )
		{
			fCameraHeightCompensation = 300.f;
		}
	}

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;

	lookAt = pFocusUnit->GetPos();
	lookAt.y += 100.0f;
	if( pFocusUnit->GetIsRight() == true )
	{
		lookAt += (pFocusUnit->GetDirVector() * lookatDistance);

		eye.x = pFocusUnit->GetXPos() + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetYPos() + height; 
		eye.z = pFocusUnit->GetZPos() + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;

		eye += (pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);	

		eye.x = pFocusUnit->GetXPos() + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetYPos() + height; 
		eye.z = pFocusUnit->GetZPos() + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * -eyeDistance);
	}

	return eye;
}

#endif CAMERA_SCRIPTING_TEST

#ifdef KEYFRAME_CAMERA
void CX2Camera::SetEnableRotateVelocity(bool bEnable)
{
	m_pStaticRotateTime->SetShowEnable(bEnable, bEnable);
	m_pIMEEditRotateTime->SetShowEnable(bEnable, bEnable);
	return;
}

void CX2Camera::SetEnableFixedCoord(bool bEnable)
{
	CKTDGUIStatic* pStaticFixedPosition = static_cast< CKTDGUIStatic* >( m_pDlgWorldCameraEdit->GetControl( L"StaticFixedPosition" ) );
	pStaticFixedPosition->SetShowEnable(bEnable, bEnable);

	m_pIMEEditCoordX->SetShowEnable(bEnable, bEnable);
	m_pIMEEditCoordY->SetShowEnable(bEnable, bEnable);
	m_pIMEEditCoordZ->SetShowEnable(bEnable, bEnable);

	return;
}

CFirstPersonCamera* CX2Camera::GetFPSCamera()
{
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			//if( g_pTFieldGame != NULL )
			//return g_pTFieldGame->GetFPSCamera()

		} break;
	case CX2Main::XS_SQUARE_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			//if( g_pSquareGame != NULL )
			//return g_pSquareGame->GetFPSCamera()

		} break;
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_DUNGEON_GAME:
		{
			if( g_pX2Game != NULL )
				return g_pX2Game->GetFPSCamera();
		} break;
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
		{
			if( g_pBeginningGame != NULL )
				return g_pBeginningGame->GetFPSCamera();
		}
#endif ELSWORD_NEW_BEGINNING
	}
	return NULL;
}


void CX2Camera::UpdateUIAngleInterpolation()
{
	switch(m_iLastAngleInterpolationIndex)
	{
	case AIT_LINEAR:
		{
			SetEnableRotateVelocity(false);
			SetEnableFixedCoord(false);
		}
		break;
	case AIT_LOOK:
		{
			SetEnableRotateVelocity(true);
			SetEnableFixedCoord(false);
		}
		break;
	case AIT_FIXED:
		{
			SetEnableRotateVelocity(false);
			SetEnableFixedCoord(true);
		}
		break;
	case AIT_TRACE:
		{
			SetEnableRotateVelocity(true);
			SetEnableFixedCoord(true);
		}
		break;
	}
	return;
}

void CX2Camera::SetViewNowKeyFrame()
{
	m_kCamera.SetTrackingTime(0.f);

	// 카메라 뷰 설정
	D3DXVECTOR3 vEyePositionOut(0, 0, 0);
	vEyePositionOut = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vEyePosition;
	m_kCamera.Move( vEyePositionOut.x, vEyePositionOut.y, vEyePositionOut.z );

	D3DXVECTOR3 vLookAtOut(0, 0, 0);
	D3DXVECTOR3 vLookAtPos(0, 0, 0);
	if(m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_eAITType == AIT_FIXED)
	{
		WCHAR buff[256] = {0};

		vLookAtPos = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vLookAtPos;
		m_kCamera.LookAt( vLookAtPos.x, vLookAtPos.y, vLookAtPos.z );

		int iCoordX = (int)vLookAtPos.x;
		_itow( iCoordX, buff, 10 );
		m_pIMEEditCoordX->SetText(buff, false);
		m_iLastCoordX = iCoordX;

		int iCoordY = (int)vLookAtPos.y;
		_itow( iCoordY, buff, 10 );
		m_pIMEEditCoordY->SetText(buff, false);
		m_iLastCoordY = iCoordY;

		int iCoordZ = (int)vLookAtPos.z;
		_itow( iCoordZ, buff, 10 );
		m_pIMEEditCoordZ->SetText(buff, false);
		m_iLastCoordZ = iCoordZ;
	}
	else
	{
		vLookAtOut = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vLookAt;
		vLookAtPos = vLookAtOut * 500.f + vEyePositionOut;
		m_kCamera.LookAt( vLookAtPos.x, vLookAtPos.y, vLookAtPos.z );
	}



	D3DXVECTOR3 vUpVecOut(0, 0, 0);
	vUpVecOut = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vUpVec;
	m_kCamera.UpVec( vUpVecOut.x, vUpVecOut.y, vUpVecOut.z );

	m_kCamera.UpdateCamera( 0 );

	GetFPSCamera()->SetViewParams( &vEyePositionOut, &vLookAtPos, &vUpVecOut );

	// 다이얼로그 값 설정

	int iAngleInterpolationIndex = (int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_eAITType;
	m_pComboAngleInterpolation->SetSelectedByIndex(iAngleInterpolationIndex);
	m_iLastAngleInterpolationIndex = iAngleInterpolationIndex;

	WCHAR buff[256] = {0};
	int iMoveVelocity = (int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_fMoveVelocity;
	_itow( iMoveVelocity, buff, 10 );
	m_pIMEEditMoveVelocity->SetText(buff, false);
	m_iLastMoveVelocity = iMoveVelocity;

	int iRotateVelocity = (int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_fRotateTime;
	_itow( iRotateVelocity, buff, 10 );
	m_pIMEEditRotateTime->SetText(buff, false);
	m_iLastRotateTime = iRotateVelocity;

	UpdateUIAngleInterpolation();



	CKTDGUIStatic *pStaticKeyFrameState = static_cast< CKTDGUIStatic* >( m_pDlgWorldCameraEdit->GetControl( L"StaticKeyFrameState" ) );
	_itow( m_iEditKeyFrameIndex+1, buff, 10 );
	pStaticKeyFrameState->SetString(0, buff);
	pStaticKeyFrameState->SetString(1, L"/");
	_itow( (int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData.size(), buff, 10 );
	pStaticKeyFrameState->SetString(2, buff);




	return;
}

void CX2Camera::PrevKeyFrame()
{
	// oasis907 : 김상윤 [2010.11.21] // 키 프레임 간 이동
	m_iEditKeyFrameIndex--;

	if(m_iEditKeyFrameIndex < 0)
		m_iEditKeyFrameIndex = 0;
	if(m_iEditKeyFrameIndex >(int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData.size() - 1)
		m_iEditKeyFrameIndex = (int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData.size() - 1;

	SetViewNowKeyFrame();
	return;
}

void CX2Camera::NextKeyFrame()
{
	// oasis907 : 김상윤 [2010.11.21] // 키 프레임 간 이동
	m_iEditKeyFrameIndex++;

	if(m_iEditKeyFrameIndex < 0)
		m_iEditKeyFrameIndex = 0;
	if(m_iEditKeyFrameIndex > (int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData.size() - 1)
		m_iEditKeyFrameIndex = (int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData.size() - 1;

	SetViewNowKeyFrame();
	return;
}

void CX2Camera::CreateFirstKeyFrame()
{
	// oasis907 : 김상윤 [2010.11.26] // 월드 카메라 정보가 없을 경우 처음 생성하는 키 프레임
	WorldCameraData kWorldCameraData;
	kWorldCameraData.m_iWorldCameraID = 0;

	KeyFrameCameraData kKeyFrameCameraData;
	kKeyFrameCameraData.m_vEyePosition	= D3DXVECTOR3(0.f, 0.f, 0.f);
	kKeyFrameCameraData.m_vLookAt		= D3DXVECTOR3(0.f, 0.f, 1.f);
	kKeyFrameCameraData.m_vUpVec		= D3DXVECTOR3(0.f, 1.f, 0.f);
	kKeyFrameCameraData.m_fTime			= 5.f;
	kKeyFrameCameraData.m_eAITType		= AIT_LINEAR;

	kKeyFrameCameraData.m_fMoveVelocity		= 100.f;
	kKeyFrameCameraData.m_fRotateTime	= 100.f;
	kKeyFrameCameraData.m_fCurveLength		= -1.f; // 첫 프레임의 구간 길이 정보는 없음

	kWorldCameraData.m_vecKeyFrameCameraData.push_back(kKeyFrameCameraData);
	m_vecWorldCameraData.push_back(kWorldCameraData);	

	m_iEditKeyFrameIndex = 0;
	return;
}


void CX2Camera::AddKeyFrame()
{
	// oasis907 : 김상윤 [2010.11.21] // 키 프레임 추가

	// 다른 요소들은 모두 복사
	m_vecWorldCameraData[0].m_vecKeyFrameCameraData.insert(m_vecWorldCameraData[0].m_vecKeyFrameCameraData.begin() + m_iEditKeyFrameIndex, 
		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex]);
	m_iEditKeyFrameIndex++;

	int iNowKeyFrame = m_iEditKeyFrameIndex;
	int iPrevKeyFrame = m_iEditKeyFrameIndex - 1;

	// 키 프레임 구간의 길이 근사
	// 복사하였기 때문에 같은 Position이므로 0
	m_vecWorldCameraData[0].m_vecKeyFrameCameraData[iNowKeyFrame].m_fCurveLength = 0.f;

	SetViewNowKeyFrame();
	return;
}

void CX2Camera::SaveKeyFrame()
{
	// oasis907 : 김상윤 [2010.11.21] // 키 프레임 저장
	D3DXVECTOR3 vEyePosition(0, 0, 0);
	D3DXVECTOR3 vLookAt(0, 0, 0);
	D3DXVECTOR3 vUpVec(0, 0, 0);

	GetFPSCamera()->FrameMove(0);

	vEyePosition.x = GetFPSCamera()->GetEyePt()->x;
	vEyePosition.y = GetFPSCamera()->GetEyePt()->y;
	vEyePosition.z = GetFPSCamera()->GetEyePt()->z;

	vLookAt.x = GetFPSCamera()->GetWorldAhead()->x;
	vLookAt.y = GetFPSCamera()->GetWorldAhead()->y;
	vLookAt.z = GetFPSCamera()->GetWorldAhead()->z;

	vUpVec.x = GetFPSCamera()->GetWorldUp()->x;
	vUpVec.y = GetFPSCamera()->GetWorldUp()->y;
	vUpVec.z = GetFPSCamera()->GetWorldUp()->z;


	if(m_iLastAngleInterpolationIndex == AIT_FIXED)
	{
		D3DXVECTOR3 vLookAtPos(0, 0, 0);
		vLookAtPos.x = (float)m_iLastCoordX;
		vLookAtPos.y = (float)m_iLastCoordY;
		vLookAtPos.z = (float)m_iLastCoordZ;

		D3DXVECTOR3 vFixedUpVec(0, 1, 0);

		GetFPSCamera()->SetViewParams( &vEyePosition, &vLookAtPos, &vUpVec );

		GetFPSCamera()->FrameMove(0);

		vEyePosition.x = GetFPSCamera()->GetEyePt()->x;
		vEyePosition.y = GetFPSCamera()->GetEyePt()->y;
		vEyePosition.z = GetFPSCamera()->GetEyePt()->z;

		vLookAt.x = GetFPSCamera()->GetWorldAhead()->x;
		vLookAt.y = GetFPSCamera()->GetWorldAhead()->y;
		vLookAt.z = GetFPSCamera()->GetWorldAhead()->z;

		vUpVec.x = GetFPSCamera()->GetWorldUp()->x;
		vUpVec.y = GetFPSCamera()->GetWorldUp()->y;
		vUpVec.z = GetFPSCamera()->GetWorldUp()->z;

		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vLookAtPos	= vLookAtPos;
		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vLookAt		= vLookAt;
		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vUpVec		= vUpVec;

		/*
		if((m_iEditKeyFrameIndex + 1 <= (int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData.size() - 1) &&
		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex+1].m_eAITType != AIT_FIXED)
		{

		D3DXVECTOR3 vEyePositionTmp = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex+1].m_vEyePosition;
		D3DXVECTOR3 vUpVecTmp = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex+1].m_vUpVec;

		GetFPSCamera()->SetViewParams( &vEyePositionTmp, &vLookAtPos, &vUpVecTmp );

		GetFPSCamera()->FrameMove(0);

		vLookAt.x = GetFPSCamera()->GetWorldAhead()->x;
		vLookAt.y = GetFPSCamera()->GetWorldAhead()->y;
		vLookAt.z = GetFPSCamera()->GetWorldAhead()->z;


		vUpVec.x = GetFPSCamera()->GetWorldUp()->x;
		vUpVec.y = GetFPSCamera()->GetWorldUp()->y;
		vUpVec.z = GetFPSCamera()->GetWorldUp()->z;

		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex+1].m_vLookAtPos	= vLookAtPos;
		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex+1].m_vLookAt		= vLookAt;
		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex+1].m_vUpVec		= vUpVec;
		}
		*/
	}
	else if(m_iLastAngleInterpolationIndex == AIT_TRACE)
	{
		D3DXVECTOR3 vLookAtPos(0, 0, 0);
		vLookAtPos.x = (float)m_iLastCoordX;
		vLookAtPos.y = (float)m_iLastCoordY;
		vLookAtPos.z = (float)m_iLastCoordZ;

		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vLookAtPos	= vLookAtPos;
		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vLookAt		= vLookAt;
		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vUpVec		= vUpVec;
	}
	else
	{
		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vLookAt		= vLookAt;
		m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vUpVec		= vUpVec;
	}

	m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_vEyePosition = vEyePosition;
	m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_fTime		= 5.f;
	m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_eAITType	= (ANGULAR_INTERPOLATION_TYPE)m_iLastAngleInterpolationIndex;



	m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_fMoveVelocity = (float)m_iLastMoveVelocity;
	m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iEditKeyFrameIndex].m_fRotateTime = (float)m_iLastRotateTime;

	/*
	// 키 프레임 구간의 길이 근사

	int iNowKeyFrame	= m_iEditKeyFrameIndex;
	int iPrevKeyFrame	= m_iEditKeyFrameIndex - 1;
	int iNextKeyFrame	= m_iEditKeyFrameIndex + 1;

	// iPrevKeyFrame - iNowKeyFrame 구간
	int IdxPos[4] = {0,};
	IdxPos[0] = iNowKeyFrame - 2;
	IdxPos[1] = iNowKeyFrame - 1;
	IdxPos[2] = iNowKeyFrame;
	IdxPos[3] = iNowKeyFrame + 1;

	bool bExistFirstCurve = true;
	int LastIdx = m_vecWorldCameraData[0].m_vecKeyFrameCameraData.size() - 1;

	if(IdxPos[0] < 0)
	{
	IdxPos[0] = 0;
	}
	if(IdxPos[1] < 0)
	{
	bExistFirstCurve = false;
	}
	if(IdxPos[3] > LastIdx)
	{
	IdxPos[3] = LastIdx;
	}

	if(true == bExistFirstCurve)
	{
	D3DXVECTOR3 vEyePosition0 = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[IdxPos[0]].m_vEyePosition;
	D3DXVECTOR3 vEyePosition1 = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[IdxPos[1]].m_vEyePosition;
	D3DXVECTOR3 vEyePosition2 = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[IdxPos[2]].m_vEyePosition;
	D3DXVECTOR3 vEyePosition3 = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[IdxPos[3]].m_vEyePosition;

	D3DXVECTOR3 vEyePositionPrev = vEyePosition1;
	D3DXVECTOR3 vEyePositionOut(0, 0, 0);
	float fCurveLength = 0.f;
	float s = 0.f;
	for(int i=1; i<2; i++)
	{
	s = (float)i / 1.f;
	D3DXVec3CatmullRom( &vEyePositionOut, &vEyePosition0, &vEyePosition1, &vEyePosition2, &vEyePosition3, s );
	D3DXVECTOR3 vTemp = vEyePositionOut - vEyePositionPrev;
	fCurveLength += D3DXVec3Length(&vTemp);
	vEyePositionPrev = vEyePositionOut;
	}
	m_vecWorldCameraData[0].m_vecKeyFrameCameraData[iNowKeyFrame].m_fCurveLength = fCurveLength;
	}


	// iNowKeyFrame - iNextKeyFrame 구간
	IdxPos[0] = iNowKeyFrame - 1;
	IdxPos[1] = iNowKeyFrame;
	IdxPos[2] = iNowKeyFrame + 1;
	IdxPos[3] = iNowKeyFrame + 2;

	bool bExistSecondCurve = true;
	if(IdxPos[0] < 0)
	{
	IdxPos[0] = 0;
	}
	if(IdxPos[2] > LastIdx)
	{
	bExistSecondCurve = false;
	}
	if(IdxPos[3] > LastIdx)
	{
	IdxPos[3] = LastIdx;
	}

	if(true == bExistSecondCurve)
	{
	D3DXVECTOR3 vEyePosition0 = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[IdxPos[0]].m_vEyePosition;
	D3DXVECTOR3 vEyePosition1 = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[IdxPos[1]].m_vEyePosition;
	D3DXVECTOR3 vEyePosition2 = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[IdxPos[2]].m_vEyePosition;
	D3DXVECTOR3 vEyePosition3 = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[IdxPos[3]].m_vEyePosition;

	D3DXVECTOR3 vEyePositionPrev = vEyePosition1;
	D3DXVECTOR3 vEyePositionOut(0, 0, 0);
	float fCurveLength = 0.f;
	float s = 0.f;
	for(int i=1; i<2; i++)
	{
	s = (float)i / 1.f;
	D3DXVec3CatmullRom( &vEyePositionOut, &vEyePosition0, &vEyePosition1, &vEyePosition2, &vEyePosition3, s );
	D3DXVECTOR3 vTemp = vEyePositionOut - vEyePositionPrev;
	fCurveLength += D3DXVec3Length(&vTemp);
	vEyePositionPrev = vEyePositionOut;
	}
	m_vecWorldCameraData[0].m_vecKeyFrameCameraData[iNextKeyFrame].m_fCurveLength = fCurveLength;
	}
	*/

	return;
}

void CX2Camera::DeleteKeyFrame()
{
	// 키프레임 하나는 남겨 두어야함.
	//if(m_iEditKeyFrameIndex == 0)
	//	return;

	if((int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData.size() <= 1)
	{
		return;
	}

	// oasis907 : 김상윤 [2010.11.21] // 키 프레임 삭제
	m_vecWorldCameraData[0].m_vecKeyFrameCameraData.erase(m_vecWorldCameraData[0].m_vecKeyFrameCameraData.begin() + m_iEditKeyFrameIndex);

	if(m_iEditKeyFrameIndex != 0)
	{
		m_iEditKeyFrameIndex--;
	}

	SetViewNowKeyFrame();

	return;
}




void CX2Camera::WorldCameraKeyProcess()
{
#ifdef _IN_HOUSE_
	if(false == m_bWorldCameraEdit)
	{
		// oasis907 : 김상윤 [2010.12.28] // 스크립트의 월드 카메라 재생
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_1) == TRUE )
		{
			if(true == m_bEnableWorldCamera)
				EndWorldCamera();

			m_bWorldCameraManualPlay = true;
			StartWorldCamera(1);
			return;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_2) == TRUE )
		{
			if(true == m_bEnableWorldCamera)
				EndWorldCamera();

			m_bWorldCameraManualPlay = true;
			StartWorldCamera(2);
			return;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_3) == TRUE )
		{
			if(true == m_bEnableWorldCamera)
				EndWorldCamera();

			m_bWorldCameraManualPlay = true;
			StartWorldCamera(3);
			return;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_4) == TRUE )
		{
			if(true == m_bEnableWorldCamera)
				EndWorldCamera();

			m_bWorldCameraManualPlay = true;
			StartWorldCamera(4);
			return;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_5) == TRUE )
		{
			if(true == m_bEnableWorldCamera)
				EndWorldCamera();

			m_bWorldCameraManualPlay = true;
			StartWorldCamera(5);
			return;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_6) == TRUE )
		{
			if(true == m_bEnableWorldCamera)
				EndWorldCamera();

			m_bWorldCameraManualPlay = true;
			StartWorldCamera(6);
			return;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_7) == TRUE )
		{
			if(true == m_bEnableWorldCamera)
				EndWorldCamera();

			m_bWorldCameraManualPlay = true;
			StartWorldCamera(7);
			return;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_8) == TRUE )
		{
			if(true == m_bEnableWorldCamera)
				EndWorldCamera();

			m_bWorldCameraManualPlay = true;
			StartWorldCamera(8);
			return;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_9) == TRUE )
		{
			if(true == m_bEnableWorldCamera)
				EndWorldCamera();

			m_bWorldCameraManualPlay = true;
			StartWorldCamera(9);
			return;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_0) == TRUE )
		{
			if(true == m_bEnableWorldCamera)
				EndWorldCamera();

			m_bWorldCameraManualPlay = true;
			StartWorldCamera(0);
			return;
		}
	}
	else
	{
		// oasis907 : 김상윤 [2010.11.21] // 키 프레임 추가
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F2) == TRUE )
		{
			AddKeyFrame();
			return;
		}

		// oasis907 : 김상윤 [2010.11.21] // 키 프레임 저장
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F3) == TRUE )
		{
			SaveKeyFrame();
			return;
		}

		// oasis907 : 김상윤 [2010.11.21] // 키 프레임 삭제
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F4) == TRUE )
		{
			DeleteKeyFrame();
			return;
		}

		// oasis907 : 김상윤 [2010.11.21] // 키 프레임 간 이동
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_LBRACKET) == TRUE )
		{
			PrevKeyFrame();
			return;
		}

		// oasis907 : 김상윤 [2010.11.21] // 키 프레임 간 이동
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RBRACKET) == TRUE )
		{
			NextKeyFrame();
			return;
		}

		// oasis907 : 김상윤 [2010.11.21] // 월드 카메라 재생
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_SPACE) == TRUE )
		{
			if(false == m_bEnableWorldCamera)
				StartWorldCamera(0);
			else
				EndWorldCamera();
			return;
		}

		// oasis907 : 김상윤 [2010.11.21] // 키 프레임 파일 출력
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F12) == TRUE )
		{
			FILE *fp = fopen("CamerKeyFrame.txt", "w");

			fwprintf(fp, L"g_pCamera:AddWorldCameraData_LUA\n{\n\tWORLD_CAMERA_ID = 0,\n\n");
			for(int i=0; i<(int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData.size(); i++)
			{
				fwprintf(fp, L"\t{ ");
				fwprintf(fp, L"{ ");
				fwprintf(fp, L"%8.2f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vEyePosition.x);
				fwprintf(fp, L"%8.2f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vEyePosition.y);
				fwprintf(fp, L"%8.2f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vEyePosition.z);
				fwprintf(fp, L"}, ");
				fwprintf(fp, L"{ ");
				fwprintf(fp, L"%8.4f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vLookAt.x);
				fwprintf(fp, L"%8.4f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vLookAt.y);
				fwprintf(fp, L"%8.4f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vLookAt.z);
				fwprintf(fp, L"}, ");
				fwprintf(fp, L"{ ");
				fwprintf(fp, L"%8.4f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vUpVec.x);
				fwprintf(fp, L"%8.4f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vUpVec.y);
				fwprintf(fp, L"%8.4f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vUpVec.z);
				fwprintf(fp, L"}, ");
				fwprintf(fp, L"%8d, ", (int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_eAITType);
				fwprintf(fp, L"%8d, ", (int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_fMoveVelocity);
				fwprintf(fp, L"%8d, ", (int)m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_fRotateTime);
				fwprintf(fp, L"%8.2f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_fCurveLength);
				fwprintf(fp, L"{ ");
				fwprintf(fp, L"%8.2f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vLookAtPos.x);
				fwprintf(fp, L"%8.2f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vLookAtPos.y);
				fwprintf(fp, L"%8.2f, ", m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_vLookAtPos.z);
				fwprintf(fp, L"}, ");
				fwprintf(fp, L"},\n"); 


				float a = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[i].m_fCurveLength;
			}
			fwprintf(fp, L"}\n");
			fclose(fp);


			return;
		}

	}
#endif _IN_HOUSE_
	return;
}

void CX2Camera::SetWorldCameraEdit( bool bEnable)
{
	m_bWorldCameraEdit = bEnable;

	m_pDlgWorldCameraEdit->SetShowEnable(bEnable, bEnable);

	if(true == bEnable)
	{
		g_pChatBox->CloseChatWindowButton();
		g_pChatBox->CloseChatWindow();

		// oasis907 : 김상윤 [2010.11.21] // 월드 카메라 데이터가 없을 경우 키프레임 하나 생성
		if(m_vecWorldCameraData.size() == 0)
		{
			CreateFirstKeyFrame();

		}
		m_iEditKeyFrameIndex = 0;
		SetViewNowKeyFrame();
	}
	else
	{
		g_pChatBox->SetChatBoxLayerUp(false);
		g_pChatBox->CloseChatWindowButton();
		g_pChatBox->OpenChatWindow();
		g_pChatBox->HideChatWindowFrame();
		g_pChatBox->HideChatEditBox();
	}
	return;
}


bool CX2Camera::ReLoadWorldCamera()
{
	if(m_wstrNowWorldCameraFilename == L"")
	{
		return false;
	}
	return LoadWorldCamera(m_wstrNowWorldCameraFilename.c_str());
}

bool CX2Camera::LoadWorldCamera( const WCHAR* pFileName )
{
	m_vecWorldCameraData.clear();

	m_wstrNowWorldCameraFilename = pFileName;
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCamera", this );
	if(true == g_pKTDXApp->LoadLuaTinker( pFileName ))
	{
		PreProcessOnCurve(false);
		return true;
	}
	else
	{
		return false;
	}
}

void CX2Camera::UnLoadWorldCamera()
{
	EndWorldCamera();
	for(int i=0; i< (int)m_vecWorldCameraData.size(); i++)
	{
		for(int j=0; j< (int)m_vecWorldCameraData[i].m_vecKeyFrameCameraData.size(); j++)
		{
			m_vecWorldCameraData[i].m_vecKeyFrameCameraData[j].m_vecEyeInterPosition.clear();
		}
		m_vecWorldCameraData[i].m_vecKeyFrameCameraData.clear();
	}
	m_vecWorldCameraData.clear();
	return;
}

void CX2Camera::PreProcessOnCurve( bool bWorldCameraEdit )
{
	int NumWorldCamera = 0;
	if(true == bWorldCameraEdit)
	{
		NumWorldCamera = 1;
	}
	else
	{
		NumWorldCamera = (int)m_vecWorldCameraData.size();
	}

	for(int i=0; i< NumWorldCamera; i++)
	{
		for(int j=0; j< (int)m_vecWorldCameraData[i].m_vecKeyFrameCameraData.size()-1; j++)
		{
			m_vecWorldCameraData[i].m_vecKeyFrameCameraData[j].m_vecEyeInterPosition.clear();

			int IdxPos[4] = {0,};
			IdxPos[0] = j - 1;
			IdxPos[1] = j;
			IdxPos[2] = j + 1;
			IdxPos[3] = j + 2;

			int LastIdx = m_vecWorldCameraData[i].m_vecKeyFrameCameraData.size() - 1;
			for(int k=0; k<4; k++)
			{
				if(IdxPos[k] < 0)
				{
					IdxPos[k] = 0;
				}
				if(IdxPos[k] > LastIdx)
				{
					IdxPos[k] = LastIdx;
				}
			}

			D3DXVECTOR3 vEyePosition0 = m_vecWorldCameraData[i].m_vecKeyFrameCameraData[IdxPos[0]].m_vEyePosition;
			D3DXVECTOR3 vEyePosition1 = m_vecWorldCameraData[i].m_vecKeyFrameCameraData[IdxPos[1]].m_vEyePosition;
			D3DXVECTOR3 vEyePosition2 = m_vecWorldCameraData[i].m_vecKeyFrameCameraData[IdxPos[2]].m_vEyePosition;
			D3DXVECTOR3 vEyePosition3 = m_vecWorldCameraData[i].m_vecKeyFrameCameraData[IdxPos[3]].m_vEyePosition;

			D3DXVECTOR3 vEyePositionPrev = vEyePosition1;
			D3DXVECTOR3 vEyePositionOut(0, 0, 0);
			float fTotalCurveLength = 0.f;
			float s = 0.f;
			EyeInterPosition kEyeInterPositionStart;
			kEyeInterPositionStart.m_vEyeInterPosition = vEyePosition1;
			kEyeInterPositionStart.m_fCurveLength = 0.f;
			m_vecWorldCameraData[i].m_vecKeyFrameCameraData[j].m_vecEyeInterPosition.push_back(kEyeInterPositionStart);
			for(int k=1; k<50; k++)
			{
				s = (float)k / 50.f;
				D3DXVec3CatmullRom( &vEyePositionOut, &vEyePosition0, &vEyePosition1, &vEyePosition2, &vEyePosition3, s );

				D3DXVECTOR3 vTemp = vEyePositionOut - vEyePositionPrev;
				float fCurveLength = D3DXVec3Length(&vTemp);

				EyeInterPosition kEyeInterPosition;
				kEyeInterPosition.m_vEyeInterPosition = vEyePositionOut;
				kEyeInterPosition.m_fCurveLength = fCurveLength;

				m_vecWorldCameraData[i].m_vecKeyFrameCameraData[j].m_vecEyeInterPosition.push_back(kEyeInterPosition);

				fTotalCurveLength += fCurveLength;
				vEyePositionPrev = vEyePositionOut;
			}
			D3DXVECTOR3 vTemp = vEyePosition2 - vEyePositionPrev;
			float fLastCurveLength = D3DXVec3Length(&vTemp);

			EyeInterPosition kEyeInterPositionEnd;
			kEyeInterPositionEnd.m_vEyeInterPosition = vEyePosition2;
			kEyeInterPositionEnd.m_fCurveLength = fLastCurveLength;

			m_vecWorldCameraData[i].m_vecKeyFrameCameraData[j].m_vecEyeInterPosition.push_back(kEyeInterPositionEnd);

			fTotalCurveLength += fLastCurveLength;

			m_vecWorldCameraData[i].m_vecKeyFrameCameraData[j+1].m_fCurveLength = fTotalCurveLength;
		}
	}
	return;
}


bool CX2Camera::AddWorldCameraData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	WorldCameraData kWorldCameraData;

	LUA_GET_VALUE_RETURN(	luaManager, "WORLD_CAMERA_ID",	kWorldCameraData.m_iWorldCameraID,	-1,	goto LoadFail; );

	if( luaManager.BeginTable( "LOOP_SEQUENCE" ) == true )
	{
		LUA_GET_VALUE( luaManager, 1,	kWorldCameraData.m_iLoopSequenceStart,	0 );
		LUA_GET_VALUE( luaManager, 2,	kWorldCameraData.m_iLoopSequenceEnd,	0 );
		LUA_GET_VALUE( luaManager, 3,	kWorldCameraData.m_iLoopCount,			0 );
		luaManager.EndTable();
	}
	
	LUA_GET_VALUE( luaManager, "HOLD_VIEW_WHEN_CAMERA_END", kWorldCameraData.m_bHoldViewWhenCameraEnd,	false	);	// duration time 내에 keyframecamera 유지

	LUA_GET_VALUE( luaManager, "WORLD_CAMERA_DURATION", kWorldCameraData.m_fWorldCameraDuration,	30.f	);	// play duration time

	int index = 1;
	while( luaManager.BeginTable( index ) == true )
	{
		KeyFrameCameraData kKeyFrameCameraData;

		if( luaManager.BeginTable( 1 )  == true)
		{
			LUA_GET_VALUE( luaManager,		1,	kKeyFrameCameraData.m_vEyePosition.x,		0.f );
			LUA_GET_VALUE( luaManager,		2,	kKeyFrameCameraData.m_vEyePosition.y,		0.f );
			LUA_GET_VALUE( luaManager,		3,	kKeyFrameCameraData.m_vEyePosition.z,		0.f );
			luaManager.EndTable();
		}

		if( luaManager.BeginTable( 2 )  == true)
		{
			LUA_GET_VALUE( luaManager,		1,	kKeyFrameCameraData.m_vLookAt.x,			0.f );
			LUA_GET_VALUE( luaManager,		2,	kKeyFrameCameraData.m_vLookAt.y,			0.f );
			LUA_GET_VALUE( luaManager,		3,	kKeyFrameCameraData.m_vLookAt.z,			0.f );
			luaManager.EndTable();
		}

		if( luaManager.BeginTable( 3 )  == true)
		{
			LUA_GET_VALUE( luaManager,		1,	kKeyFrameCameraData.m_vUpVec.x,				0.f );
			LUA_GET_VALUE( luaManager,		2,	kKeyFrameCameraData.m_vUpVec.y,				0.f );
			LUA_GET_VALUE( luaManager,		3,	kKeyFrameCameraData.m_vUpVec.z,				0.f );
			luaManager.EndTable();
		}

		int iAITType = 0;
		LUA_GET_VALUE( luaManager,		4,	iAITType,			0 );
		kKeyFrameCameraData.m_eAITType = (ANGULAR_INTERPOLATION_TYPE)iAITType;


		LUA_GET_VALUE( luaManager,		5,	kKeyFrameCameraData.m_fMoveVelocity,	0.f );

		LUA_GET_VALUE( luaManager,		6,	kKeyFrameCameraData.m_fRotateTime,	0.f );

		LUA_GET_VALUE( luaManager,		7,	kKeyFrameCameraData.m_fCurveLength,		0.f );

		if( luaManager.BeginTable( 8 )  == true)
		{
			LUA_GET_VALUE( luaManager,		1,	kKeyFrameCameraData.m_vLookAtPos.x,				0.f );
			LUA_GET_VALUE( luaManager,		2,	kKeyFrameCameraData.m_vLookAtPos.y,				0.f );
			LUA_GET_VALUE( luaManager,		3,	kKeyFrameCameraData.m_vLookAtPos.z,				0.f );
			luaManager.EndTable();
		}

		kWorldCameraData.m_vecKeyFrameCameraData.push_back(kKeyFrameCameraData);
		index++;
		luaManager.EndTable();
	}
	m_vecWorldCameraData.push_back(kWorldCameraData);
	return true;

LoadFail:
	wstringstream wstrStream;
	wstrStream << L"WorldCameraLoadFail m_iWorldCameraID = " << kWorldCameraData.m_iWorldCameraID;

	ErrorLogMsg( XEM_ERROR79, wstrStream.str().c_str() );

	return false;
}

void CX2Camera::WorldCameraFrameMove( double fTime, float fElapsedTime )
{
	if(true == m_bWorldCameraEdit &&  true == m_bEnableWorldCamera)
	{
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_SPACE) == TRUE )
		{
			EndWorldCamera();
			return;
		}
	}


	if(false == m_bEnableWorldCamera)
	{
		EndWorldCamera();
		return;
	}

	if(NULL == m_pNowPlayingWorldCamera)
	{
		EndWorldCamera();
		return;
	}

	if(-1 == m_iNowKeyFrameIndex)
	{
		EndWorldCamera();
		return;
	}

	if(true == m_bHoldViewWorldCamera )
		return;

	m_fNowWorldCameraElapsedTime += fElapsedTime;
	if(m_fNowWorldCameraElapsedTime > m_fNowWorldCameraDuration)
	{
		EndWorldCamera();
		return;
	}

	// 트래킹 카메라 끄기
	m_kCamera.SetTrackingTime(0.f);







	/* 
	// 시간 기반
	m_fNowKeyFrameElapsedTime += fElapsedTime;

	if(m_fNowKeyFrameElapsedTime >= m_fNowKeyFrameTotalTime)
	{
	m_iNowKeyFrameIndex++;

	if(m_iNowKeyFrameIndex >= (int)m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData.size())
	{
	EndWorldCamera();
	return;
	}

	m_fNowKeyFrameElapsedTime = m_fNowKeyFrameElapsedTime - m_fNowKeyFrameTotalTime;
	m_fNowKeyFrameTotalTime = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_fTime;	

	m_fNowKeyFrameLeftDistance = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_fCurveLength;

	}
	*/

	// 속도 기반

	// 보간 없음
	//float fNowMoveVelocty = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_fMoveVelocity;


	// 속도 선형 보간
	float fNowMoveVelocty = 
		(m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_fMoveVelocity * (1.f - m_fNowKeyFrameMoveDistance/m_fNowKeyFrameTotalDistance) ) +
		(m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex + 1].m_fMoveVelocity * (m_fNowKeyFrameMoveDistance/m_fNowKeyFrameTotalDistance) );

	float fNowMoveDistance = fNowMoveVelocty * fElapsedTime;





	//float fNowKeyFrameAverageVelocity = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_fMoveVelocity - m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex + 1].m_fMoveVelocity;



	m_fNowKeyFrameElapsedTime += fElapsedTime;





	m_fNowKeyFrameMoveDistance += fNowMoveDistance;

	if(m_fNowKeyFrameMoveDistance >= m_fNowKeyFrameTotalDistance)
	{
		m_iNowKeyFrameIndex++;

		//if(m_iNowKeyFrameIndex >= (int)m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData.size())

		// 루프 기능
		if(m_pNowPlayingWorldCamera->m_iLoopCount != 0 && 
			m_iNowKeyFrameIndex + 1 > m_pNowPlayingWorldCamera->m_iLoopSequenceEnd &&
			m_iNowLoopCount < m_pNowPlayingWorldCamera->m_iLoopCount)
		{
			m_iNowLoopCount++;
			m_iNowKeyFrameIndex = m_pNowPlayingWorldCamera->m_iLoopSequenceStart;
		}

		// 카메라 재생 종료
		if(m_iNowKeyFrameIndex + 1 > (int)m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData.size() - 1)
		{
			if(true == m_pNowPlayingWorldCamera->m_bHoldViewWhenCameraEnd)
				m_bHoldViewWorldCamera = true;
			else
				EndWorldCamera();
			return;
		}
		m_fNowKeyFrameElapsedTime = 0.f; 

		fNowMoveDistance = m_fNowKeyFrameMoveDistance - m_fNowKeyFrameTotalDistance;

		m_fNowKeyFrameMoveDistance = m_fNowKeyFrameMoveDistance - m_fNowKeyFrameTotalDistance;

		m_iNowEyeInterPositionMoveDistance = 0.f; 
		m_iNowEyeInterPositionIndex = 0;

		// **



		//m_fNowKeyFrameMoveDistance = 0.f;
		m_fNowKeyFrameTotalDistance = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex + 1].m_fCurveLength;


		if(m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iNowKeyFrameIndex-1].m_eAITType == AIT_FIXED &&
			m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_eAITType != AIT_FIXED)
		{
			D3DXVECTOR3 vEyePosition(0, 0, 0);
			D3DXVECTOR3 vLookAt(0, 0, 0);
			D3DXVECTOR3 vUpVec(0, 0, 0);


			D3DXVECTOR3 vLookAtPos = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iNowKeyFrameIndex-1].m_vLookAtPos;
			D3DXVECTOR3 vEyePositionTmp = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vEyePosition;
			D3DXVECTOR3 vUpVecTmp = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vUpVec;

			GetFPSCamera()->SetViewParams( &vEyePositionTmp, &vLookAtPos, &vUpVecTmp );

			GetFPSCamera()->FrameMove(0);

			vLookAt.x = GetFPSCamera()->GetWorldAhead()->x;
			vLookAt.y = GetFPSCamera()->GetWorldAhead()->y;
			vLookAt.z = GetFPSCamera()->GetWorldAhead()->z;


			vUpVec.x = GetFPSCamera()->GetWorldUp()->x;
			vUpVec.y = GetFPSCamera()->GetWorldUp()->y;
			vUpVec.z = GetFPSCamera()->GetWorldUp()->z;

			m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vLookAtPos	= vLookAtPos;
			m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vLookAt		= vLookAt;
			m_vecWorldCameraData[0].m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vUpVec		= vUpVec;
		}	




	}

	/*	// 위치 보간 1. D3DXVec3CatmullRom 그대로 사용 - 균일한 속도 불가능
	int IdxPos[4] = {0,};
	IdxPos[0] = m_iNowKeyFrameIndex - 1;
	IdxPos[1] = m_iNowKeyFrameIndex;
	IdxPos[2] = m_iNowKeyFrameIndex + 1;
	IdxPos[3] = m_iNowKeyFrameIndex + 2;

	int LastIdx = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData.size() - 1;
	for(int i=0; i<4; i++)
	{
	if(IdxPos[i] < 0)
	{
	IdxPos[i] = 0;
	}
	if(IdxPos[i] > LastIdx)
	{
	IdxPos[i] = LastIdx;
	}
	}

	D3DXVECTOR3 vEyePosition0 = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[IdxPos[0]].m_vEyePosition;
	D3DXVECTOR3 vEyePosition1 = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[IdxPos[1]].m_vEyePosition;
	D3DXVECTOR3 vEyePosition2 = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[IdxPos[2]].m_vEyePosition;
	D3DXVECTOR3 vEyePosition3 = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[IdxPos[3]].m_vEyePosition;

	// 시간 기반
	//float s = m_fNowKeyFrameElapsedTime / m_fNowKeyFrameTotalTime;

	// 속도 기반
	float s = m_fNowKeyFrameMoveDistance / m_fNowKeyFrameTotalDistance;


	D3DXVECTOR3 vEyePositionOut(0, 0, 0);

	D3DXVec3CatmullRom( &vEyePositionOut, &vEyePosition0, &vEyePosition1, &vEyePosition2, &vEyePosition3, s );

	m_kCamera.Move( vEyePositionOut.x, vEyePositionOut.y, vEyePositionOut.z );
	*/	


	// 속도 기반
	float s = m_fNowKeyFrameMoveDistance / m_fNowKeyFrameTotalDistance;


	m_iNowEyeInterPositionMoveDistance += fNowMoveDistance;

	if(m_iNowEyeInterPositionMoveDistance > 
		m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vecEyeInterPosition[m_iNowEyeInterPositionIndex+1].m_fCurveLength )
	{
		m_iNowEyeInterPositionMoveDistance = m_iNowEyeInterPositionMoveDistance - m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vecEyeInterPosition[m_iNowEyeInterPositionIndex+1].m_fCurveLength;
		m_iNowEyeInterPositionIndex++;
	}


	D3DXVECTOR3 vDirInterPosition = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vecEyeInterPosition[m_iNowEyeInterPositionIndex + 1].m_vEyeInterPosition - 
		m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vecEyeInterPosition[m_iNowEyeInterPositionIndex].m_vEyeInterPosition;

	D3DXVec3Normalize( &vDirInterPosition, &vDirInterPosition);

	D3DXVECTOR3 vMoveTargetPos = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vecEyeInterPosition[m_iNowEyeInterPositionIndex].m_vEyeInterPosition + 
		vDirInterPosition * m_iNowEyeInterPositionMoveDistance;

	m_kCamera.Move( vMoveTargetPos.x, vMoveTargetPos.y, vMoveTargetPos.z );



	if(AIT_FIXED == m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_eAITType)
	{
		D3DXVECTOR3 vLookAtPos = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vLookAtPos;
		m_kCamera.LookAt( vLookAtPos.x, vLookAtPos.y, vLookAtPos.z );
		/*
		D3DXVECTOR3 vUpVec1 = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[IdxPos[1]].m_vUpVec;


		m_kCamera.UpVec( vUpVec1.x, vUpVec1.y, vUpVec1.z );
		*/
	}
	else if(AIT_TRACE == m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_eAITType)
	{
		D3DXVECTOR3 vLookAtPos = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vLookAtPos;
		int iPlayerIdx = (int)m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_fRotateTime;

		if( g_pX2Game != NULL )
		{
			CX2GUUser *pUser = g_pX2Game->GetUserUnit(iPlayerIdx);
			if(pUser != NULL)
			{
				D3DXVECTOR3 vLookAtPosOffset = vLookAtPos;

				vLookAtPos = pUser->GetPos() + vLookAtPosOffset;
			}
		}
		m_kCamera.LookAt( vLookAtPos.x, vLookAtPos.y, vLookAtPos.z );
	}
	else 
	{
		float t = 0.f;
		if(AIT_LINEAR == m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_eAITType)
		{
			t = s;
		}
		else if(AIT_LOOK == m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_eAITType)
		{
			// Look - 자연스러운 각속도
			//m_fNowKeyFrameElapsedTime += fElapsedTime;

			float fNowKeyFrameRotateTime = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_fRotateTime;

			// milisecond 단위
			fNowKeyFrameRotateTime = fNowKeyFrameRotateTime/10.f;

			t = m_fNowKeyFrameElapsedTime/fNowKeyFrameRotateTime;

			if(t > 1.f)
			{
				t = 1.f;
			}

			//t = t * 2.f;

			//t = t - 1.f;


			t = t * 90;

			t = sinf( D3DXToRadian(t));


			//t = t + 1.f;

			//t = t / 2.f;
		}

		D3DXVECTOR3 vLookAt1 = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vLookAt;
		D3DXVECTOR3 vLookAt2 = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex+1].m_vLookAt;


		D3DXVec3Normalize(&vLookAt1, &vLookAt1);
		D3DXVec3Normalize(&vLookAt2, &vLookAt2);


		// Slerp 1. 쿼터니온 사용

		//D3DXQUATERNION qrtLookAt1(vLookAt1.x, vLookAt1.y, vLookAt1.z, 0.f);
		//D3DXQUATERNION qrtLookAt2(vLookAt2.x, vLookAt2.y, vLookAt2.z, 0.f);

		//D3DXQUATERNION qrtLookAtOut;

		//D3DXQuaternionSlerp(&qrtLookAtOut, &qrtLookAt1, &qrtLookAt2, t);


		//D3DXQuaternionNormalize(&qrtLookAtOut, &qrtLookAtOut);

		//D3DXVECTOR3 vLookAtOut(qrtLookAtOut.x, qrtLookAtOut.y, qrtLookAtOut.z);


		// Slerp 2. Slerp 직접 계산 
		float fAngle = acos(D3DXVec3Dot(&vLookAt1, &vLookAt2));

		D3DXVECTOR3 vLookAtOut(0, 0, 0);

		vLookAtOut = (sinf((1.f - t)*fAngle)/sinf(fAngle))*vLookAt1 + (sinf(t*fAngle)/sinf(fAngle))*vLookAt2;

		D3DXVECTOR3 vLookAtPos = vLookAtOut * 500.f + vMoveTargetPos;
		m_kCamera.LookAt( vLookAtPos.x, vLookAtPos.y, vLookAtPos.z );

	}



	D3DXVECTOR3 vUpVec1 = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_vUpVec;
	D3DXVECTOR3 vUpVec2 = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex+1].m_vUpVec;

	D3DXQUATERNION qrtUpVec1(vUpVec1.x, vUpVec1.y, vUpVec1.z, 0.f);
	D3DXQUATERNION qrtUpVec2(vUpVec2.x, vUpVec2.y, vUpVec2.z, 0.f);

	D3DXQUATERNION qrtUpVecOut;

	D3DXQuaternionSlerp(&qrtUpVecOut, &qrtUpVec1, &qrtUpVec2, s);


	D3DXQuaternionNormalize(&qrtUpVecOut, &qrtUpVecOut);


	D3DXVECTOR3 vUpVecOut(qrtUpVecOut.x, qrtUpVecOut.y, qrtUpVecOut.z);

	m_kCamera.UpVec( vUpVecOut.x, vUpVecOut.y, vUpVecOut.z );



	if(AIT_TRACE == m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[m_iNowKeyFrameIndex].m_eAITType)
	{
		m_kCamera.UpVec( 0.f, 1.f, 0.f );
	}

	return;
}

bool CX2Camera::StartWorldCamera(int iWorldCameraID )
{
	if(m_bEnableWorldCamera == true)
		EndWorldCamera();

	m_bEnableWorldCamera = true;

	if(true == m_bWorldCameraEdit || true == m_bWorldCameraManualPlay )
	{
		g_pMain->SetFreeCamera(false);

		//PreProcessOnCurve(true);
	}

	m_kCamera.SetFixedUpVec(false);

	if(m_bWorldCameraEdit == true)
	{
		ReLoadWorldCamera(); // 테스트 코드 - 항상 루아 파일 새로 읽게
	}


	for(UINT i = 0; i < m_vecWorldCameraData.size(); i++)
	{
		if(iWorldCameraID == m_vecWorldCameraData[i].m_iWorldCameraID)
		{
			m_pNowPlayingWorldCamera = &m_vecWorldCameraData[i];
			m_iNowKeyFrameIndex = 0;
			m_fNowWorldCameraElapsedTime = 0.f;
			m_fNowWorldCameraDuration = m_vecWorldCameraData[i].m_fWorldCameraDuration;

			// 키 프레임이 하나일 경우 뷰 고정
			if((int)m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData.size() == 1)
			{
				D3DXVECTOR3 vMoveTargetPos = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[0].m_vEyePosition;
				D3DXVECTOR3 vLookAtPos = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[0].m_vLookAtPos;
				D3DXVECTOR3 vUpVecOut = m_vecWorldCameraData[0].m_vecKeyFrameCameraData[0].m_vUpVec;

				m_kCamera.Move( vMoveTargetPos.x, vMoveTargetPos.y, vMoveTargetPos.z );
				m_kCamera.LookAt( vLookAtPos.x, vLookAtPos.y, vLookAtPos.z );
				m_kCamera.UpVec( vUpVecOut.x, vUpVecOut.y, vUpVecOut.z );

				m_bHoldViewWorldCamera = true;

				return true;
			}


			if(m_iNowKeyFrameIndex + 1 > (int)m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData.size() - 1)
			{
				EndWorldCamera();
				return true;
			}

			// 시간 기반
			//m_fNowKeyFrameElapsedTime = 0.f;
			//m_fNowKeyFrameTotalTime = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[0].m_fTime;

			// 속도 기반
			m_fNowKeyFrameMoveDistance = 0.f;
			//m_fNowKeyFrameTotalDistance = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[0].m_fCurveLength;	
			m_fNowKeyFrameTotalDistance = m_pNowPlayingWorldCamera->m_vecKeyFrameCameraData[1].m_fCurveLength;


			m_fNowKeyFrameElapsedTime = 0.f;

			return true;
		}
	}
	return false;
}
void CX2Camera::EndWorldCamera()
{
	m_kCamera.SetFixedUpVec(true);

	if(true == m_bWorldCameraEdit || true == m_bWorldCameraManualPlay)
	{
		g_pMain->SetFreeCamera(true);
	}

	m_bWorldCameraManualPlay = false;

	m_bEnableWorldCamera = false;
	m_pNowPlayingWorldCamera = NULL;
	m_iNowKeyFrameIndex = -1;
	m_fNowKeyFrameElapsedTime = -1.f;

	m_fNowWorldCameraElapsedTime	= 0.f;
	m_fNowWorldCameraDuration		= 0.f;

	m_iNowLoopCount = 0;
	m_iNowEyeInterPositionIndex = 0;
	m_iNowEyeInterPositionMoveDistance = 0;

	m_bHoldViewWorldCamera = false;
	return;
}

bool CX2Camera::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case UCECM_EXIT:
		{
			;
		}
		break;
	case UCECM_PREV_KEYFRAME:
		{
			PrevKeyFrame();
			return true;
		}
		break;
	case UCECM_NEXT_KEYFRAME:
		{
			NextKeyFrame();
			return true;
		}
		break;
	case UCECM_ADD_KEYFRAME:
		{
			AddKeyFrame();
			return true;
		}
		break;
	case UCECM_SAVE_KEYFRAME:
		{
			SaveKeyFrame();
			return true;
		}
		break;
	case UCECM_DEL_KEYFRAME:
		{
			DeleteKeyFrame();
			return true;
		}
		break;
	case UCECM_COMBO_ANGLE:
		{
			m_iLastAngleInterpolationIndex = m_pComboAngleInterpolation->GetSelectedItemIndex();
			UpdateUIAngleInterpolation();
			return true;
		}
		break;
	case UCECM_ENTER_MOV_VELO:
		{
			m_iLastMoveVelocity = (int) wcstol( m_pIMEEditMoveVelocity->GetText(), NULL, 10 );
			return true;
		}
		break;
	case UCECM_ENTER_ROT_VELO:
		{
			m_iLastRotateTime = (int) wcstol( m_pIMEEditRotateTime->GetText(), NULL, 10 );
			return true;
		}
		break;
	case UCECM_ENTER_FIXED_COORD_X:
		{
			m_iLastCoordX = (int) wcstol( m_pIMEEditCoordX->GetText(), NULL, 10 );
			return true;
		}
		break;
	case UCECM_ENTER_FIXED_COORD_Y:
		{
			m_iLastCoordY = (int) wcstol( m_pIMEEditCoordY->GetText(), NULL, 10 );
			return true;
		}
		break;
	case UCECM_ENTER_FIXED_COORD_Z:
		{
			m_iLastCoordZ = (int) wcstol( m_pIMEEditCoordZ->GetText(), NULL, 10 );
			return true;
		}
		break;
	}
	return false;
}

#endif KEYFRAME_CAMERA


void CX2Camera::PartsLookTrackingCameraFromPartsEye( CX2GameUnit* pFocusUnit, const D3DXVECTOR3& vLookPos_, const D3DXVECTOR3& vEyePos_, float fTrackingTime_ /*= 0.3f */ )
{
	if ( NULL != pFocusUnit )
	{
		m_CameraState	= CS_EVENT;
		m_kCamera.SetTracking( vEyePos_.x, vEyePos_.y, vEyePos_.z, vLookPos_.x, vLookPos_.y, vLookPos_.z, fTrackingTime_ );
	}
}

void CX2Camera::PartsLookDirectCameraFromPartsEye( CX2GameUnit* pFocusUnit, const D3DXVECTOR3& vLookPos_, const D3DXVECTOR3& vEyePos_ )
{
#ifdef NPC_TRACKINGCAMERA
	if( m_bNpcTrackingCamera == true )
		return;
#endif

	if ( NULL != pFocusUnit )
	{
		m_CameraState	= CS_EVENT;
		m_kCamera.Point( vEyePos_.x, vEyePos_.y, vEyePos_.z, vLookPos_.x, vLookPos_.y, vLookPos_.z );
	}
}

bool CX2Camera::ParsingLookTypeAndPos( KLuaManager& luaManger_, const CX2GameUnit* pFocusUnit_, OUT D3DXVECTOR3& vLookPos_ )
{
	LOOK_TYPE eLookType;
	LUA_GET_VALUE_RETURN_ENUM( luaManger_, "LOOK_TYPE",		eLookType,		LOOK_TYPE, LT_BONE, return false );

	wstring wstrLookParts;
	LUA_GET_VALUE(		luaManger_, "LOOK_PARTS",		wstrLookParts,		L"" );

	D3DXVECTOR3 vLookOffset;
	LUA_GET_VALUE(		luaManger_, "LOOK_OFFSET_X",	vLookOffset.x,	0.0f );
	LUA_GET_VALUE(		luaManger_, "LOOK_OFFSET_Y",	vLookOffset.y,	0.0f );
	LUA_GET_VALUE(		luaManger_, "LOOK_OFFSET_Z",	vLookOffset.z,	0.0f );

	//{{ 2009.01.19 김태완 : 코드정리
	switch ( eLookType )
	{
	case LT_BONE:
		{
			vLookPos_ = pFocusUnit_->GetBonePos( wstrLookParts.c_str() );
		} break;
	case LT_IMPACT_POINT:
		{
			vLookPos_ = pFocusUnit_->GetImpactPos();
		}break;
	case LT_WEAPON:
		{
			// fix!! 일단은 0번 무기의 0번째 메시를 보는 걸로 하드코딩
			vLookPos_ = pFocusUnit_->GetWeaponBonePos( 0, wstrLookParts.c_str(), 0 );
		} break;
	case LT_UNIT_POSITION:
		{
			vLookPos_ = pFocusUnit_->GetPos();
		} break;
	default:
		{
			ASSERT( !"Undefined Camera Looktype" );
			return false;
		} break;
	}
	//}}

	// 캐릭터 좌/우 방향에 영향을 받는 카메라 오프셋 추가
	if( NULL != pFocusUnit_ )
	{
		float fLookOffsetApplyRotateX;
		LUA_GET_VALUE(		luaManger_, "LOOK_APPLY_ROTATE_OFFSET_X",	fLookOffsetApplyRotateX,	0.0f );

		if( false == pFocusUnit_->GetIsRight()  )
		{
			fLookOffsetApplyRotateX *= -1.f;
		}

		vLookOffset.x += fLookOffsetApplyRotateX;
	}

	vLookPos_ += vLookOffset;

	return true;
}

bool CX2Camera::ParsingEyeTypeAndPos( KLuaManager& luaManger_, const CX2GameUnit* pFocusUnit_, OUT D3DXVECTOR3& vEyePos_ )
{
	EYE_TYPE eEyeType;
	LUA_GET_VALUE_RETURN_ENUM( luaManger_, "EYE_TYPE",		eEyeType,		EYE_TYPE, ET_BONE, return false );

	wstring wstrEyeParts;
	LUA_GET_VALUE(		luaManger_, "EYE_PARTS",		wstrEyeParts,		L"" );

	D3DXVECTOR3 vEyeOffset;
	LUA_GET_VALUE(		luaManger_, "EYE_OFFSET_X",	vEyeOffset.x,	0.0f );
	LUA_GET_VALUE(		luaManger_, "EYE_OFFSET_Y",	vEyeOffset.y,	0.0f );
	LUA_GET_VALUE(		luaManger_, "EYE_OFFSET_Z",	vEyeOffset.z,	0.0f );

	//{{ 2009.01.19 김태완 : 코드정리
	switch ( eEyeType )
	{
	case LT_BONE:
		{
			vEyePos_ = pFocusUnit_->GetBonePos( wstrEyeParts.c_str() );
		} break;
	case LT_IMPACT_POINT:
		{
			vEyePos_ = pFocusUnit_->GetImpactPos();
		}break;
	case LT_WEAPON:
		{
			// fix!! 일단은 0번 무기의 0번째 메시를 보는 걸로 하드코딩
			vEyePos_ = pFocusUnit_->GetWeaponBonePos( 0, wstrEyeParts.c_str(), 0 );
		} break;
	case LT_UNIT_POSITION:
		{
			vEyePos_ = pFocusUnit_->GetPos();
		} break;
	default:
		{
			ASSERT( !"Undefined Camera EyeType" );
			return false;
		} break;
	}
	//}}

	vEyePos_ += vEyeOffset;

	return true;
}



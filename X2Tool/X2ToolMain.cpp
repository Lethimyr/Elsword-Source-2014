#include "StdAfx.h"
#include "X2ToolMain.h"
#include "MainFrm.h"
#include "DXUT.h"

#pragma region global
ID3DXLine *g_pLine;
ID3DXLine *g_pLineAxis;
ID3DXLine *g_pLineGrid;
#pragma endregion variable

/** @function 	: CompareVertex
	@brief 		: Vertex의 X 성분 크기 비교
*/
bool CompareVertex( tLineVertex a, tLineVertex b )
{
	 return a.vVertex.x < b.vVertex.x;
}

/** @function 	: ComputeRay
	@brief 		: 반직선 계산
*/
void ComputeRay( D3DXVECTOR3* pvRayOrigin, D3DXVECTOR3* pvRayDirection, const POINTS& mouspos, 
	const D3DXMATRIX* pmatView, const D3DXMATRIX* pmatProj )
{
	RECT rc;
	float w, h;
	GetClientRect( DXUTGetHWND(), &rc );
	w = (float)(rc.right - rc.left);
	h = (float)(rc.bottom - rc.top);

	float left, top;
	left = (float)(mouspos.x-w/2)/w*2.0f;
	top  = (float)(h/2-mouspos.y)/h*2.0f;

	D3DXMATRIX matViewProj, matInvViewProj;
	D3DXMatrixMultiply(&matViewProj, pmatView, pmatProj);
	D3DXMatrixInverse(&matInvViewProj, NULL, &matViewProj);

	D3DXVECTOR3 vFrom, vTo, vRay;
	vFrom = D3DXVECTOR3(left, top, -1.f);
	vTo = D3DXVECTOR3(left, top,  1.f);

	D3DXVec3TransformCoord(&vFrom, &vFrom, &matInvViewProj);
	D3DXVec3TransformCoord(&vTo, &vTo, &matInvViewProj);
	vRay = vTo-vFrom;
	D3DXVec3Normalize(&vRay, &vRay);

	*pvRayDirection = vRay;
	*pvRayOrigin = vFrom;
}

/** @function 	: CX2ToolMain
	@brief 		: 생성자
*/
CX2ToolMain::CX2ToolMain(void)
{	
	m_wstrLineMapFullName = L"";
	m_wstrWorldFullName = L"";
	m_pFont_A = NULL;
	m_pFont_B = NULL;
	m_bCanProcess = false;
	m_bDrag = false;
	m_bLCtrl = false;
	m_bLShift = false;
	m_bLAlt = false;
	m_vDragStartPos = D3DXVECTOR2(0.f, 0.f);
	m_vMousePos = D3DXVECTOR2(0.f, 0.f);
	m_vRectRayLine  = D3DXVECTOR3( 0.f, 0.f, 0.f);

	m_bEditLine	= false;
	m_bEditLineVertex = true;
	m_bEditStartPosition = false;
	m_bEditWorld = false;
	m_bViewLine = true;
	m_bViewWorld = true;
	m_bEditTrigger = false;

	m_bMoveWorldObject = true;
	m_bRotationWorldObject = false;;
	m_bScaleWorldObject = false;;

	m_bFreeMoveLine = false;
	m_bMoveAxisX = false;
	m_bMoveAxisY = false;
	m_bMoveAxisZ = false;
	m_bShowMoveAxis = false;
	m_vPosMoveAxis = D3DXVECTOR3( 0.f, 0.f, 0.f );
	m_vFreeMoveLinePos = D3DXVECTOR3( 0.f, 0.f, 0.f );

	m_bUserCamera = false;

	m_fScaleAxis = 1.f;
	m_iVertPivot = -1;
	m_bSnap = false;	
	m_bCopyLine = false;
	
	m_bFillet = false;
	m_vFilletStartPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vFilletLine[0] = -1;
	m_vFilletLine[1] = -1;
	m_vDirFillet[0] = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vDirFillet[1] = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_bShowLineInfo = true;

	m_bShowLineNumber = true;
	m_bShowLineSetNumber = true;
	m_bShowLineCameraNumber = true;
	m_bShowLineTriggerNumber = true;
	m_bShowInnerWaterInfo = true;
		
	m_bHideUser = false;
	m_bHideNpc = false;
	m_iPickedStartPosition = -1;
	m_iOverStartPosition = -1;
	m_bMoveStartPosition = false;

	m_wstrPickWorldObjectName = L"";
	m_bViewSelectedWorldobject = true;

	m_bSelectedWorldObject = false;	
	m_bPickMoveWorldObjectPivot = false;
	m_bMoveWorld = false;
	m_vMoveWorldObjectPivot = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vMoveWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vRotationWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vScaleWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_iWidth = 1024;
	m_iHeight = 768;
	m_bHeightMap = false;

	m_bAddObjectByLib = false;
	m_wstrSelectedLib = L"";
	m_wstrLibPath = L"";
	m_bViewSelectObject = true;
	m_pRenderTargetTex = g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"ComponentThumbnail", 170, 128 );
	m_pRenderTargetTopView  = g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"TopView", 1024, 768 );
	m_pRenderTargetFrontView  = g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"FrontView", 1024, 768 );	
	m_vPickPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_bRenderTopView = false;

	m_iSectorWidth = 768;
	m_iSectorHeight = 768;
	m_iSectorDepth = 768;
	m_bTileRender = false;
	m_pBox = NULL;
	m_bCanPlay = false;

	m_bCheckNpc = false;
	m_bChangeStartPos = false;

	GetGameOption().SetMusic( false );
	GetGameOption().SetSoundVolume( 5000.f );
		
	GetGameOption().SetTexDetail( CX2GameOption::OL_HIGH, true );
	GetGameOption().SetUnitDetail( CX2GameOption::OL_HIGH, true );
	GetGameOption().SetMapDetail( CX2GameOption::OL_HIGH, true );
	GetGameOption().SetEffectDetail( CX2GameOption::OL_HIGH, true );

	ResetTool( false, false, true, true, false );

	InitX2Data();
}

/** @function 	: ~CX2ToolMain
	@brief 		: 소멸자
*/
CX2ToolMain::~CX2ToolMain(void)
{
	SAFE_CLOSE( m_pXMeshBoundingSphere );
	SAFE_CLOSE( m_pStartLineMesh );
	SAFE_CLOSE( m_pEndLineMesh );
	SAFE_CLOSE( m_pControlXMesh );
	SAFE_CLOSE( m_pControlYMesh );
	SAFE_CLOSE( m_pControlZMesh );
	SAFE_CLOSE( m_pControlAxis );

	SAFE_DELETE( m_pMatrix );
	SAFE_CLOSE( m_pXSkinMesh );
	SAFE_CLOSE( m_pXSkinMeshNpc );

	SAFE_CLOSE( m_pRenderTargetTex );
	SAFE_CLOSE( m_pRenderTargetTopView );
	SAFE_CLOSE( m_pRenderTargetFrontView );
	
	if( m_pBox != NULL )
		m_pBox->Release();

	InitReadyDungeonNpc();

	for( unsigned i=0; i < vecpXSkinAnim.size(); i++ )
	{
		//SAFE_DELETE( (CKTDGXSkinAnim*)vecpXSkinAnim[i] );
		SAFE_DELETE_KTDGOBJECT( (CKTDGXSkinAnim*)vecpXSkinAnim[i] );
	}
	vecpXSkinAnim.clear();
}

/** @function 	: OnFrameMove
	@brief 		: 프레임마다 데이터 갱신
*/
HRESULT CX2ToolMain::OnFrameMove( double fTime, float fElapsedTime )
{
	HRESULT hr = S_OK;
	CX2Main::OnFrameMove(fTime, fElapsedTime);

	// state
#if 0
	{
		if( NULL != g_pInstanceData )
		{
			g_pInstanceData->OnFrameMove( fTime, fElapsedTime );
		}
		g_pKTDXApp->GetDGManager()->OnFrameMove( fTime, fElapsedTime );
		g_pKTDXApp->GetDGManager()->GetCamera()->UpdateCamera( fElapsedTime );
	}
#endif

	if( g_pX2Game == NULL )
	{
		CreateGame();

		g_pData->LoadPicChar1();
		g_pData->LoadPicChar2();
		g_pData->LoadPicChar3();
		g_pData->LoadPicChar4();
		g_pData->LoadPicChar5();
		g_pData->LoadPicChar6();
		g_pData->LoadPicChar7();
		g_pData->LoadPicChar8();
		g_pData->LoadPicChar9();
		g_pData->LoadPicChar10();	

		return hr;
	}
	

	if( g_pX2Game != NULL )
	{
		if( m_bCanPlay == false )
		{
			g_pData->GetPicChar()->Clear();
			g_pData->GetPicCharRed()->Clear();
			g_pData->GetPicCharBlue()->Clear();
			g_pData->GetPicCharGreen()->Clear();
			g_pData->GetPicCharYellow()->Clear();
			g_pData->GetPicCharGameScore()->Clear();
			g_pData->GetPicCharTechPoint()->Clear();
			g_pData->GetScoreBonusPicChar()->Clear();
			g_pData->GetComboPicChar()->Clear();
			g_pData->GetComboPicCharBig()->Clear();
		}

		g_pX2Game->EnableAllNPCAI(m_bCanPlay);

		if( g_pInstanceData != NULL && g_pInstanceData->m_bChangeStage == true )
		{
			g_pInstanceData->m_bChangeStage = false;

			m_wstrWorldFullName = g_pX2Game->GetWorld()->GetWorldName();
			m_wstrSoundName = g_pX2Game->GetWorld()->GetBGMName();

			ParsingLine();

			m_fPerspective = g_pKTDXApp->GetDGManager()->GetPerspectiveValue();

			{
				CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
				CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
				CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
				if( pMain != NULL )
				{
					pMain->m_wndCameraInfo.m_LineCameraDlg.ResetCameraList();
					pMain->m_wndStartPosition.m_LineStartPositionDlg.ResetStartPosition();
					pMain->m_wndDungeonInfo.m_DungeonDlg.m_editLineName.SetWindowText( g_pX2Game->GetWorld()->GetLineMap()->GetLineFullName().c_str() );
					pMain->m_wndDungeonInfo.m_DungeonDlg.m_editWorldName.SetWindowText( g_pX2Game->GetWorld()->GetWorldName().c_str() );
					pMain->m_wndTriggerProperties.InitPropList();

					int iNowStage = pDungeon->GetNowStageIndex();
					int iNowSubStage = pDungeon->GetNowStage()->GetNowSubStageIndex();
					pMain->m_DlgDungeonEditor.ChangeSelItem(iNowStage, iNowSubStage);
					ClearUnDoWorld();
				}
			}
		}

		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();



		if( pMain != NULL && g_pX2Game->GetX2Camera() != NULL )
		{
			D3DXVECTOR3 vEye = g_pX2Game->GetX2Camera()->GetCamera().GetEye();
			D3DXVECTOR3 vLookat = g_pX2Game->GetX2Camera()->GetCamera().GetLookAt();
			char CameraInfo[100];
			wstring wstrCameraInfo = L"";
			sprintf( CameraInfo, "%5.2f %5.2f %5.2f", vEye.x, vEye.y, vEye.z );
			ConvertCharToWCHAR( wstrCameraInfo, CameraInfo );
			pMain->m_wndDungeonInfo.m_DungeonDlg.m_editCameraEye.SetWindowText(wstrCameraInfo.c_str());
			sprintf( CameraInfo, "%5.2f %5.2f %5.2f", vLookat.x, vLookat.y, vLookat.z );
			ConvertCharToWCHAR( wstrCameraInfo, CameraInfo );
			pMain->m_wndDungeonInfo.m_DungeonDlg.m_editCameraLookat.SetWindowText(wstrCameraInfo.c_str());
		}

		if( ::GetKeyState(VK_LCONTROL) & 0x8000 ) // g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_LCONTROL) == TRUE )
			m_bLCtrl = true;
		else
			m_bLCtrl = false;

		if( ::GetKeyState(VK_LSHIFT) & 0x8000 ) // g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_LCONTROL) == TRUE )
			m_bLShift = true;
		else
			m_bLShift = false;

		if( ::GetKeyState(VK_LMENU) & 0x8000 ) // g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_LCONTROL) == TRUE )
			m_bLAlt = true;
		else
			m_bLAlt = false;
		
		

		g_pX2Game->GetWorld()->SetRenderLineMap(m_bViewLine);
		g_pX2Game->GetWorld()->SetShowObjectByTool(m_bViewWorld);

		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_ADD ) == TRUE ||
			g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_EQUALS ) == TRUE )
		{
			g_pMain->GetGameOption().CameraZoomIn( 1 );
		}
		else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_SUBTRACT ) == TRUE ||
			g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_MINUS ) == TRUE )

		{
			g_pMain->GetGameOption().CameraZoomIn( -1 );
		}

		g_pX2Game->SetAllowFrameCount( (int)(85 * 5.3) );
		hr = g_pX2Game->OnFrameMove( fTime, fElapsedTime );

		if( g_pData->GetDungeonRoom() != NULL )
		{
			
			for ( int i = 0; i < g_pData->GetDungeonRoom()->GetSlotNum(); i++ )
			{
				CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData( i );
				if ( pSlotData->m_pUnit == NULL )
				{
					continue;
				}

				CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( pSlotData->m_pUnit->GetUID() );
				if( NULL == pGUUser )
					continue;
				/*
				if( pSlotData->m_PingTime < 60 )
				{
					pGUUser->GetGageManager()->SetPingLevel( 3 );
				}
				else if( pSlotData->m_PingTime < 120 )
				{
					pGUUser->GetGageManager()->SetPingLevel( 2 );
				}
				else if( pSlotData->m_PingTime < 10000 )
				{
					pGUUser->GetGageManager()->SetPingLevel( 1 );
				}
				else
				{
					pGUUser->GetGageManager()->SetPingLevel( 0 );
				}*/
			}
			g_pData->GetDungeonRoom()->OnFrameMove( fTime, fElapsedTime );
		}
	}

	if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL && g_pX2Game->GetWorld()->GetLineMap() != NULL )
	{
		g_pX2Game->GetWorld()->GetLineMap()->UpdateLineVB();
	}

	m_bSelectedWorldObject = false;

	if( g_pX2Game != NULL && m_bEditLine == true )
	{
		if( m_bFreeMoveLine == true )
		{
			CX2World *pWorld = g_pX2Game->GetWorld();
			if( pWorld != NULL )
			{	
				int iPickObjectIndex = -1;
				float fPickObjectDistance = 9999999.f;
				float fHitDistance = 9999999.f;

				vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
				for(UINT i=0; i<vecWorldObjectList.size(); ++i)
				{
					CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
					if( pWorldObject != NULL && pWorldObject->GetObjectType() != CX2WorldObject::OT_PARTICLE && pWorldObject->m_bShowObject == true &&
						g_pKTDXApp->GetDGManager()->IsInFrustum( (CKTDGObject*)pWorldObject ) == true )
					{					
						if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
						{
							CX2WorldObjectMesh *pObject = (CX2WorldObjectMesh*)pWorldObject;
							if( pObject->RayHit(m_vRayLine[0], m_vRayLine[1], &fHitDistance, pObject->GetMatrix().GetMatrix(pObject->GetBillboardType()) ) == true )
							{
								if( fHitDistance > 0.f && fPickObjectDistance >= fHitDistance )
								{
									iPickObjectIndex = i;
									fPickObjectDistance = fHitDistance;
								}
							}
						}
						else
						{
							if( pWorldObject->RayHit(m_vRayLine[0], m_vRayLine[1], &fHitDistance) == true )
							{
								if( fHitDistance > 0.f && fPickObjectDistance >= fHitDistance )
								{
									iPickObjectIndex = i;
									fPickObjectDistance = fHitDistance;
								}
							}
						}
					}
				}

				if( iPickObjectIndex >= 0 )
				{
					D3DXVECTOR3 vDir = m_vRayLine[1] - m_vRayLine[0];
					D3DXVec3Normalize( &vDir, &vDir );
					m_vFreeMoveLinePos = m_vRayLine[0] + vDir * fPickObjectDistance;
				}
			}
		}


		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();			
		vector<CLineStartPosition::STeamStartPos> &vecTeamStartPos = pMain->m_wndStartPosition.m_LineStartPositionDlg.vecTeamStartPos;

		if( vecpXSkinAnim.size() < vecTeamStartPos.size() )
		{			
			for( unsigned i=vecpXSkinAnim.size(); i<vecTeamStartPos.size(); ++i )
			{				
				if( pMain != NULL )
				{					
					if( vecTeamStartPos[i].iTeamColor == CLineStartPosition::TEAM_NONE )
					{
						CKTDGXSkinAnim* pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();

						pXSkinAnim->SetAnimXSkinMesh(  m_pXSkinMeshNpc );
						pXSkinAnim->AddModelXSkinMesh( m_pXSkinMeshNpc );
						pXSkinAnim->SetAlphaObject( true );
						pXSkinAnim->ChangeAnim( L"Normal", false );
						pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
						vecpXSkinAnim.push_back( pXSkinAnim );

						pXSkinAnim->SetCenter( m_pXSkinMeshNpc->GetCenter() );
						pXSkinAnim->SetBoundingRadius( m_pXSkinMeshNpc->GetBoundingRadius() );
					}
					else
					{
						CKTDGXSkinAnim* pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();

						pXSkinAnim->SetAnimXSkinMesh(  m_pXSkinMesh );
						pXSkinAnim->AddModelXSkinMesh( m_pXSkinMesh );
						pXSkinAnim->SetAlphaObject( true );
						pXSkinAnim->ChangeAnim( L"Normal", false );
						pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
						vecpXSkinAnim.push_back( pXSkinAnim );

						pXSkinAnim->SetCenter( m_pXSkinMesh->GetCenter() );
						pXSkinAnim->SetBoundingRadius( m_pXSkinMesh->GetBoundingRadius() );
					}
				}
				
				//CKTDGXSkinAnim* pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
				
			}
		}

		CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
		for( UINT i=0; i<pMain->m_wndStartPosition.m_LineStartPositionDlg.vecTeamStartPos.size(); ++i)
		{
			CLineStartPosition::STeamStartPos& sp = pMain->m_wndStartPosition.m_LineStartPositionDlg.vecTeamStartPos[i];
			const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(sp.iLineIndex);
			if( pLineData != NULL )
			{
				sp.vPos = pLineData->startPos + ( pLineData->dirVector * (pLineData->fLength * sp.fGap) );
			}
		}
		for( unsigned i=0; i<vecpXSkinAnim.size(); i++ )
		{	
			hr = vecpXSkinAnim[i]->OnFrameMove(fTime, fElapsedTime);
		}
		for( UINT i=0; i<m_vecDungeonSkinAnimNpc.size(); ++i)
		{
			hr = m_vecDungeonSkinAnimNpc[i]->OnFrameMove(fTime, fElapsedTime);
		}
	}	
	else if( g_pX2Game != NULL && m_bEditWorld == true )
	{
		CX2World *pWorld = g_pX2Game->GetWorld();
		if( pWorld != NULL )
		{	
			int iPickObjectIndex = -1;
			float fPickObjectDistance = 9999999.f;
			float fHitDistance = 9999999.f;

			vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
			for(UINT i=0; i<vecWorldObjectList.size(); ++i)
			{
				CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
				if( pWorldObject != NULL && pWorldObject->GetObjectType() != CX2WorldObject::OT_PARTICLE && 
					pWorldObject->m_bShowObject == true && pWorldObject->m_bFreezing == false )
				{
					if( (m_bLCtrl == true || (m_bMoveWorld == true && pWorldObject->m_bSelected == false && m_bMoveWorldObject == true )) ||
						(m_bAddObjectByLib == true && m_bLShift == true) )
					{
						if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
						{
							CX2WorldObjectMesh *pObject = (CX2WorldObjectMesh*)pWorldObject;
							if( pObject->RayHit(m_vRayLine[0], m_vRayLine[1], &fHitDistance, pObject->GetMatrix().GetMatrix(pObject->GetBillboardType()) ) == true )
							{
								if( fHitDistance > 0.f && fPickObjectDistance >= fHitDistance )
								{
									iPickObjectIndex = i;
									fPickObjectDistance = fHitDistance;
								}
							}
						}
						else
						{
							if( pWorldObject->RayHit(m_vRayLine[0], m_vRayLine[1], &fHitDistance) == true )
							{
								if( fHitDistance > 0.f && fPickObjectDistance >= fHitDistance )
								{
									iPickObjectIndex = i;
									fPickObjectDistance = fHitDistance;
								}
							}
						}
						
						
					}

					pWorldObject->m_bOver = false;
				}

				if( pWorldObject->m_bSelected == true && m_bMoveWorld == false && m_bMoveAxis == false && 
					(m_bMoveWorldObject == true || m_bRotationWorldObject == true || m_bScaleWorldObject == true) )
				{
					m_bSelectedWorldObject = true;
					m_iVertPivot = i;								
					m_vMoveWorldObjectPivot = pWorldObject->GetPos();

					CKTDGMatrix matPivot = pWorldObject->GetMatrix();
					matPivot.Scale(1.f, 1.f, 1.f);
					m_matPivot =matPivot.GetMatrix(CKTDGMatrix::BT_NONE);
				
				}
			}

			m_wstrPickWorldObjectName = L"";
			if( iPickObjectIndex >= 0 )
			{
				if( m_bMoveWorld == true )
				{
					if( m_bMoveWorldObject == true )
					{
						D3DXVECTOR3 vDir = m_vRayLine[1] - m_vRayLine[0];
						D3DXVec3Normalize( &vDir, &vDir );
						m_vMoveWorldObject = m_vRayLine[0] + vDir * fPickObjectDistance;
					}
					
				}
				else
				{
					if(m_bAddObjectByLib == true && m_bLShift == true)
					{
						D3DXVECTOR3 vDir = m_vRayLine[1] - m_vRayLine[0];
						D3DXVec3Normalize( &vDir, &vDir );
						m_vPickPos = m_vRayLine[0] + vDir * fPickObjectDistance;
					}
					else
					{
						CX2WorldObject *pWorldObject = vecWorldObjectList[iPickObjectIndex].get();
						if( pWorldObject->m_bShowObject == true && pWorldObject->m_bFreezing == false )
						{
							pWorldObject->m_bOver = true;

							switch( pWorldObject->GetObjectType() )
							{
							case CX2WorldObject::OT_STATIC_MESH:
								{
									CX2WorldObjectMesh *pStaticMesh = (CX2WorldObjectMesh*)pWorldObject;
									m_wstrPickWorldObjectName = pStaticMesh->GetMesh()->GetDeviceID();
								}					
								break;
							case CX2WorldObject::OT_SKIN_MESH:
								{
									CX2WorldObjectSkinMesh *pSkinMesh = (CX2WorldObjectSkinMesh*)pWorldObject;
									m_wstrPickWorldObjectName = pSkinMesh->GetXSkinMeshName();
								}
								break;
							case CX2WorldObject::OT_LIGHT_FLOW:
								{
									CX2WorldObjectLightFlow *pLightFlowMesh = (CX2WorldObjectLightFlow*)pWorldObject;
									m_wstrPickWorldObjectName = pLightFlowMesh->GetXMeshName();
								}
								break;
							default:
								m_wstrPickWorldObjectName = L"Unknown";
								break;
							}
						}
					}				
				}				
			}
		}
	}

	if( m_bEditWorld == true && m_bMoveWorld == false && m_bMoveAxis == false && 
		(m_bMoveWorldObject == true || m_bRotationWorldObject == true || m_bScaleWorldObject == true) )
	{
		m_bPickMoveWorldObjectPivot = false;
		m_bPickMoveWorldObjectPivot = PickMoveWorldObject();
		if( m_bPickMoveWorldObjectPivot == true )
		{
			m_bMoveAxis = false;
			m_bMoveAxisX = false;
			m_bMoveAxisY = false;
			m_bMoveAxisZ = false;
		}
		else if( m_bPickMoveWorldObjectPivot == false && m_bLCtrl == false && m_bDrag == false)
			PickMoveAxis();
	}

	if( g_pX2Game != NULL && m_bEditWorld == true )
	{
		if( m_bMoveWorld == true )
		{
			if( m_bMoveWorldObject == true )
			{
				UpdateMoveWorldObject();
			}			
		}		
	}

	if( m_bEditLine == true && m_bLCtrl == false && m_bDrag == false )
		PickMoveAxis();

	return hr;
}

/** @function 	: OnFrameRender
	@brief 		: 프레임마다 렌더링
*/
HRESULT CX2ToolMain::OnFrameRender()
{
	static bool bFirstRender = true;

	HRESULT hr = S_OK;
	if( NULL != g_pX2Game )
	{		
		if( bFirstRender == true )
		{
			CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
			if( pMain != NULL )
			{
				CRect rectClient;
				GetClientRect(pMain->GetSafeHwnd(), &rectClient );
				pMain->OnSize( 0, rectClient.Width(), rectClient.Height() );
			}
			bFirstRender = false;
		}
		

		if( m_bCanPlay == true )
		{
			// Game play
			hr = g_pX2Game->OnFrameRender();
		}
		else		
		{
			if( m_bEditWorld == true )
			{
				CX2World *pWorld = g_pX2Game->GetWorld();

				vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();
				for(UINT i=0; i<vecSkyDome.size(); ++i)
				{
					CKTDGSkyDome *pSkyDome = vecSkyDome[i];
					if( pSkyDome != NULL )
					{
						if( pSkyDome->m_bSelected == true && m_bViewSelectedWorldobject == true && m_bViewSelectObject == true)
						{
							if( pSkyDome->m_bBackupRenderParam == false )
							{
								pSkyDome->m_oriRenderParam = *pSkyDome->GetRenderParam();
								pSkyDome->m_bBackupRenderParam = true;
							}

							if( pSkyDome->m_bSelected == true )
							{
								CKTDGXRenderer::RenderParam *pRenderParam = pSkyDome->GetRenderParam();
								pRenderParam->renderType = CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
								pRenderParam->fOutLineWide = pSkyDome->GetDistanceToCamera() / 3000.0f;
								pRenderParam->outLineColor = D3DXCOLOR( 1.f, 0.f, 0.f, 1.f);
								//pRenderParam->lightPos = g_pX2Game->GetX2Camera()->GetCamera()->GetEye();
								pSkyDome->SetColorFactor( D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f) );
							}
						}
						if( pSkyDome->m_bSelected == false || m_bViewSelectedWorldobject == false )
						{
							if( pSkyDome->m_bBackupRenderParam == true )
							{
								*pSkyDome->GetRenderParam() = pSkyDome->m_oriRenderParam;
								pSkyDome->m_bBackupRenderParam = false;
							}
						}
					}
				}
				
				vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
				for(UINT i=0; i<vecWorldObjectList.size(); ++i)
				{
					CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
					if( pWorldObject != NULL )
					{
						if( (pWorldObject->m_bOver == true || pWorldObject->m_bSelected == true) && 
							m_bViewSelectedWorldobject == true && m_bViewSelectObject == true &&
							pWorldObject->GetObjectType() != CX2WorldObject::OT_PARTICLE )
						{
							if( pWorldObject->m_bBackupRenderParam == false )
							{
								pWorldObject->m_oriRenderParam = *pWorldObject->GetRenderParam();
								pWorldObject->m_bBackupRenderParam = true;
							}

							if( pWorldObject->m_bOver == true || pWorldObject->m_bSelected == true )
							{
								CKTDGXRenderer::RenderParam *pRenderParam = pWorldObject->GetRenderParam();
								pRenderParam->renderType = CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
								pRenderParam->fOutLineWide = pWorldObject->GetDistanceToCamera() / 3000.0f;
								pRenderParam->outLineColor = D3DXCOLOR( 1.f, 0.f, 0.f, 1.f);
								//pRenderParam->lightPos = g_pX2Game->GetX2Camera()->GetCamera()->GetEye();
								pWorldObject->SetColorFactor( D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f) );
							}
						}
						if( (pWorldObject->m_bOver == false && pWorldObject->m_bSelected == false) || 
							m_bViewSelectedWorldobject == false ||
							pWorldObject->GetObjectType() == CX2WorldObject::OT_PARTICLE )
						{
							if( pWorldObject->m_bBackupRenderParam == true )
							{
								*pWorldObject->GetRenderParam() = pWorldObject->m_oriRenderParam;
								pWorldObject->m_bBackupRenderParam = false;
							}
						}
					}
				}
			}
			
			g_pX2Game->GetWorld()->OnFrameRender();

			// world tool & linemap tool
			g_pKTDXApp->GetDGManager()->ObjectChainSort();

			g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();

			g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();

			
		}
		

#if 1		
		if( m_bEditLine == true )
		{
			m_bPickMoveWorldObjectPivot = false;
			RenderStartPos();

			if( m_bEditLineVertex == true && m_bEditStartPosition == false )
			{
				PickLineVertex( m_bDrag );
			}
			else
			{				
				if( m_bEditStartPosition == true )
				{
					if( m_bMoveStartPosition == false )
						PickStartPosition();
				}
				else
				{
					PickLine( m_bDrag );
				}
			}
#if 1
			// 드래그 라인 출력
			if( m_bDrag == true && m_bEditStartPosition == false )
			{
				D3DXCOLOR vColor = D3DXCOLOR(0.f, 1.f, 1.f, 1.f);
				if( m_bLCtrl == true )
					vColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
				RenderRectangle(m_vDragLineView[0], m_vDragLineView[1], vColor);
			}
#endif
			// 라인 버텍스 출력
			{
				int iVertex = m_vecLineVertex.size();
				for(int i = 0; i<iVertex; ++i)
				{
					BOUNDINGBOX_TYPE eBT = BT_NONE;
					if( m_vecLineVertex[i].m_vecLineInfo.size() == 1 )
					{
						if( m_vecLineVertex[i].m_vecLineInfo[0].m_bStartPos == true )
							eBT = BT_START;
						else
							eBT = BT_END;
					}
					else if( m_vecLineVertex[i].m_vecLineInfo.size() >= 2 )
					{
						eBT = BT_MIDDLE;
					}

					RenderBoundingSphere( m_vecLineVertex[i].vVertex, 1.5f, m_vecLineVertex[i].m_bOver, m_vecLineVertex[i].m_bSelected, eBT );
				}					
			}

			// Fillet 출력
			if( m_bFillet == true )
			{
				m_iVertPivot = - 1;
				CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
				CKTDGLineMap::LineData *pBeforeLine = pLineMap->AccessLineData( m_vFilletLine[0] );
				CKTDGLineMap::LineData *pNextLine = pLineMap->AccessLineData( m_vFilletLine[1] );

				if( pBeforeLine != NULL && pNextLine != NULL )
				{
					RenderLine( pBeforeLine->endPos, pNextLine->startPos, D3DXCOLOR(1.f, 1.f, 0.f, 1.f) );
					RenderBoundingSphere( pBeforeLine->endPos, 1.5f, true, false, BT_MIDDLE );
					RenderBoundingSphere( pNextLine->startPos, 1.5f, true, false, BT_MIDDLE );
				}
			}

			// 라인 이동 축 출력
			if( m_iVertPivot < 0 )
			{
				m_bShowMoveAxis = false;
			}
			else
			{
				if( m_bEditStartPosition == false )
				{
					m_bShowMoveAxis = true;
					m_vPosMoveAxis = m_vecLineVertex[m_iVertPivot].vVertex;
					RenderMoveAxis( m_vPosMoveAxis );
				}
				else
				{
					m_bShowMoveAxis = false;
				}
			}
						
			RenderGrid();

			float fInfoTextY = 10;
			if( m_bEditLine == true && m_bShowMoveAxis == true && (m_bMoveAxisX == true || m_bMoveAxisY == true || m_bMoveAxisZ == true) )
			{
				wstring wstrPickAxis = L"";
				if( m_bMoveAxisX == true )
					wstrPickAxis = wstrPickAxis + L"X";
				if( m_bMoveAxisY == true )
					wstrPickAxis = wstrPickAxis + L"Y";
				if( m_bMoveAxisZ == true )
					wstrPickAxis = wstrPickAxis + L"Z";
				wstrPickAxis = wstrPickAxis + L"축이 선택되었습니다.";
				m_pFont_A->Draw2DText( wstrPickAxis.c_str(), 10, (int)fInfoTextY, 
					D3DXCOLOR(1.f, 0.f, 0.f, 1.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f) );
				fInfoTextY += 20;
			}
			if( m_bSnap == true )
			{				
				m_pFont_A->Draw2DText( L"Snap 기능이 활성화 되었습니다.", 10, (int)fInfoTextY, 
					D3DXCOLOR(1.f, 0.f, 0.f, 1.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f) );
			}
		}	
		else if( m_bEditWorld == true )
		{
			m_pFont_A->Draw2DText( m_wstrPickWorldObjectName.c_str(), 10, 10,
				D3DXCOLOR(1.f, 1.f, 0.f, 1.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f) );

			if( m_bSelectedWorldObject == true )
			{
				RenderMoveWorld( m_vMoveWorldObjectPivot );

				m_bShowMoveAxis = true;
				m_vPosMoveAxis = m_vMoveWorldObjectPivot;
				RenderMoveAxis( m_vPosMoveAxis );
			}
			else
			{
				m_bShowMoveAxis = false;
			}

			float fInfoTextY = 10;
			if( m_bShowMoveAxis == true && (m_bMoveAxisX == true || m_bMoveAxisY == true || m_bMoveAxisZ == true) )
			{
				wstring wstrPickAxis = L"";
				if( m_bMoveAxisX == true )
					wstrPickAxis = wstrPickAxis + L"X";
				if( m_bMoveAxisY == true )
					wstrPickAxis = wstrPickAxis + L"Y";
				if( m_bMoveAxisZ == true )
					wstrPickAxis = wstrPickAxis + L"Z";
				wstrPickAxis = wstrPickAxis + L"축이 선택되었습니다.";
				m_pFont_A->Draw2DText( wstrPickAxis.c_str(), 10, (int)fInfoTextY, 
					D3DXCOLOR(1.f, 0.f, 0.f, 1.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f) );
				fInfoTextY += 20;
			}

			if( m_bAddObjectByLib == true )
			{
				float fDist = GetDistance(m_vPickPos, g_pX2Game->GetX2Camera()->GetCamera().GetEye());
				float fRadius = (fDist / 20000.f);
				if( m_bUserCamera == true )
				{
					fRadius *= 0.5f;
				}
				RenderBoundingSphere( m_vPickPos, fRadius, true, true, BT_END );
			}
		}
#endif

// 		if( m_bShowLineInfo == true )
// 		{
// 			
// 		}

#pragma region 라인 정보 출력 함수 호출
		OnFrameRenderLineInfo();
#pragma endregion 

		if( m_bRenderTopView == true && (m_bEditLine == true || m_bEditWorld == true))
		{
			if( m_pRenderTargetTopView != NULL )
				m_pRenderTargetTopView->Draw(0, 0, 400, 300);
			if( m_pRenderTargetFrontView != NULL )
				m_pRenderTargetFrontView->Draw(400, 0, 400, 300);
		}

		if( m_bTileRender == true )
			OnRenderTile();
	}

	return hr;
}

/** @function 	: RenderAxisView
	@brief 		: 중심축 그리기, 거리에 따른 크기 변화 수정 필요
*/
void CX2ToolMain::RenderAxisView()
{
	if( g_pX2Game == NULL || (m_bEditLine == false && m_bEditWorld == false) )
		return;

	CKTDXDeviceRenderTargetTexture *pRenderTarget = NULL;

	for(int i=0; i<2; ++i)
	{
		if( i == 0 )
			pRenderTarget = m_pRenderTargetTopView;
		else
			pRenderTarget = m_pRenderTargetFrontView;
		
		D3DXMATRIX matIden;
		D3DXMATRIX matWorld, matView, matProj;
		bool bOriginalProjcetionMode = g_pKTDXApp->GetDGManager()->GetPerspective();
		float fPerspectiveValue = g_pKTDXApp->GetDGManager()->GetPerspectiveValue();
		g_pKTDXApp->GetDevice()->GetTransform( D3DTS_WORLD, &matWorld );
		g_pKTDXApp->GetDevice()->GetTransform( D3DTS_VIEW, &matView );
		g_pKTDXApp->GetDevice()->GetTransform( D3DTS_PROJECTION, &matProj );

		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), false );	// Ortho


		D3DXMatrixIdentity( &matIden );

		pRenderTarget->BeginRender( true );
		pRenderTarget->Clear(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
		
		D3DXVECTOR3 vCenter = m_vPosMoveAxis;
		D3DXVECTOR3 vEye = vCenter;	
		D3DXVECTOR3 vUp = vCenter - g_pX2Game->GetX2Camera()->GetCamera().GetEye();
		D3DXVec3Normalize(&vUp, &vUp);

		if( i == 0 )
			vEye.y += 10000.f;
		else
		{
			D3DXVECTOR3 vDir = vUp;
			vDir.y = 0.f;
			vEye.y = vCenter.y;
			vEye = vEye - (vDir * 10000.f);
		}

		if( i == 1 )
			vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

		D3DXMATRIX ViewMatrix;
		D3DXMatrixLookAtLH( &ViewMatrix, &vEye, &vCenter, &vUp );
		g_pKTDXApp->SetViewTransform( &ViewMatrix );				

		g_pKTDXApp->SetWorldTransform( &matIden );
		{
			if( m_bEditWorld == true )
			{
				CX2World *pWorld = g_pX2Game->GetWorld();

				vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();
				for(UINT i=0; i<vecSkyDome.size(); ++i)
				{
					CKTDGSkyDome *pSkyDome = vecSkyDome[i];
					if( pSkyDome != NULL )
					{
						if( pSkyDome->m_bSelected == true && m_bViewSelectedWorldobject == true && m_bViewSelectObject == true)
						{
							if( pSkyDome->m_bBackupRenderParam == false )
							{
								pSkyDome->m_oriRenderParam = *pSkyDome->GetRenderParam();
								pSkyDome->m_bBackupRenderParam = true;
							}

							if( pSkyDome->m_bSelected == true )
							{
								CKTDGXRenderer::RenderParam *pRenderParam = pSkyDome->GetRenderParam();
								pRenderParam->renderType = CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
								pRenderParam->fOutLineWide = pSkyDome->GetDistanceToCamera() / 3000.0f;
								pRenderParam->outLineColor = D3DXCOLOR( 1.f, 0.f, 0.f, 1.f);
								//pRenderParam->lightPos = g_pX2Game->GetX2Camera()->GetCamera()->GetEye();
								pSkyDome->SetColorFactor( D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f) );
							}
						}
						if( pSkyDome->m_bSelected == false || m_bViewSelectedWorldobject == false )
						{
							if( pSkyDome->m_bBackupRenderParam == true )
							{
								*pSkyDome->GetRenderParam() = pSkyDome->m_oriRenderParam;
								pSkyDome->m_bBackupRenderParam = false;
							}
						}
					}
				}

				vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
				for(UINT i=0; i<vecWorldObjectList.size(); ++i)
				{
					CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
					if( pWorldObject != NULL )
					{
						if( (pWorldObject->m_bOver == true || pWorldObject->m_bSelected == true) && 
							m_bViewSelectedWorldobject == true && m_bViewSelectObject == true &&
							pWorldObject->GetObjectType() != CX2WorldObject::OT_PARTICLE )
						{
							if( pWorldObject->m_bBackupRenderParam == false )
							{
								pWorldObject->m_oriRenderParam = *pWorldObject->GetRenderParam();
								pWorldObject->m_bBackupRenderParam = true;
							}

							if( pWorldObject->m_bOver == true || pWorldObject->m_bSelected == true )
							{
								CKTDGXRenderer::RenderParam *pRenderParam = pWorldObject->GetRenderParam();
								pRenderParam->renderType = CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
								pRenderParam->fOutLineWide = pWorldObject->GetDistanceToCamera() / 3000.0f;
								pRenderParam->outLineColor = D3DXCOLOR( 1.f, 0.f, 0.f, 1.f);
								//pRenderParam->lightPos = g_pX2Game->GetX2Camera()->GetCamera()->GetEye();
								pWorldObject->SetColorFactor( D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f) );
							}
						}
						if( (pWorldObject->m_bOver == false && pWorldObject->m_bSelected == false) || 
							m_bViewSelectedWorldobject == false ||
							pWorldObject->GetObjectType() == CX2WorldObject::OT_PARTICLE )
						{
							if( pWorldObject->m_bBackupRenderParam == true )
							{
								*pWorldObject->GetRenderParam() = pWorldObject->m_oriRenderParam;
								pWorldObject->m_bBackupRenderParam = false;
							}
						}
					}
				}
			}

			g_pX2Game->GetWorld()->OnFrameRender();

			// world tool & linemap tool
			g_pKTDXApp->GetDGManager()->ObjectChainSort();

			g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();

			g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();					


		}



		if( m_bEditLine == true )
		{
			m_bPickMoveWorldObjectPivot = false;

			// 라인 버텍스 출력
			{
				int iVertex = m_vecLineVertex.size();
				for(int i = 0; i<iVertex; ++i)
				{
					BOUNDINGBOX_TYPE eBT = BT_NONE;
					if( m_vecLineVertex[i].m_vecLineInfo.size() == 1 )
					{
						if( m_vecLineVertex[i].m_vecLineInfo[0].m_bStartPos == true )
							eBT = BT_START;
						else
							eBT = BT_END;
					}
					else if( m_vecLineVertex[i].m_vecLineInfo.size() >= 2 )
					{
						eBT = BT_MIDDLE;
					}

					RenderBoundingSphere( m_vecLineVertex[i].vVertex, 1.5f, m_vecLineVertex[i].m_bOver, m_vecLineVertex[i].m_bSelected, eBT );
				}					
			}

			// Fillet 출력
			if( m_bFillet == true )
			{
				m_iVertPivot = - 1;
				CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
				CKTDGLineMap::LineData *pBeforeLine = pLineMap->AccessLineData( m_vFilletLine[0] );
				CKTDGLineMap::LineData *pNextLine = pLineMap->AccessLineData( m_vFilletLine[1] );

				if( pBeforeLine != NULL && pNextLine != NULL )
				{
					RenderLine( pBeforeLine->endPos, pNextLine->startPos, D3DXCOLOR(1.f, 1.f, 0.f, 1.f) );
					RenderBoundingSphere( pBeforeLine->endPos, 1.5f, true, false, BT_MIDDLE );
					RenderBoundingSphere( pNextLine->startPos, 1.5f, true, false, BT_MIDDLE );
				}
			}
		}

		g_pKTDXApp->GetDGManager()->SetPerspectiveValue(fPerspectiveValue);
		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), bOriginalProjcetionMode );
		g_pKTDXApp->SetProjectionTransform( &matProj );
		g_pKTDXApp->SetWorldTransform( &matWorld );
		g_pKTDXApp->SetViewTransform( &matView );

		pRenderTarget->EndRender();
	}
}

/** @function 	: OnResetDevice
	@brief 		: 장치가 재설정 됐을 때
*/
HRESULT CX2ToolMain::OnResetDevice()
{	
	if( m_pRenderTargetTex == NULL )
		m_pRenderTargetTex = g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"ComponentThumbnail", 128, 128 );
	else
		m_pRenderTargetTex->OnResetDevice();

	if( m_pRenderTargetTopView == NULL )
		m_pRenderTargetTopView = g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"TopView", 1024, 768 );
	else
		m_pRenderTargetTopView->OnResetDevice();

	if( m_pRenderTargetFrontView == NULL )
		m_pRenderTargetFrontView = g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"FrontView", 1024, 768 );
	else
		m_pRenderTargetFrontView->OnResetDevice();
	
	for( UINT i = 0; i < vecpXSkinAnim.size(); ++i )
		vecpXSkinAnim[i]->OnResetDevice();
	
	if( m_pXMeshBoundingSphere != NULL )
		m_pXMeshBoundingSphere->OnResetDevice();

	if(	m_pStartLineMesh != NULL )
		m_pStartLineMesh->OnResetDevice();

	if( m_pEndLineMesh != NULL )
		m_pEndLineMesh->OnResetDevice();

	if( m_pControlXMesh != NULL )
		m_pControlXMesh->OnResetDevice();

	if( m_pControlYMesh != NULL )
		m_pControlYMesh->OnResetDevice();

	if( m_pControlZMesh != NULL )
		m_pControlZMesh->OnResetDevice();

	if( m_pControlAxis != NULL )
		m_pControlAxis->OnResetDevice();

	if( m_pFont_A != NULL )
		m_pFont_A->OnResetDevice();
	
	if( m_pFont_B != NULL )
		m_pFont_B->OnResetDevice();

	if( m_pXSkinMesh != NULL )
		m_pXSkinMesh->OnResetDevice();

	if( m_pXSkinMeshNpc != NULL )
		m_pXSkinMeshNpc->OnResetDevice();

	for(UINT i = 0; i< m_vecDungeonSkinAnimNpc.size(); ++i)
		m_vecDungeonSkinAnimNpc[i]->OnResetDevice();

	for(UINT i = 0; i < m_vecDungeonSkinMeshNpc.size(); ++i)
		m_vecDungeonSkinMeshNpc[i]->OnResetDevice();

	if( g_pLine != NULL )
		g_pLine->OnResetDevice();

	if( g_pLineAxis != NULL )
		g_pLineAxis->OnResetDevice();

	if( g_pLineGrid != NULL )
		g_pLineGrid->OnResetDevice();

	return S_OK;
}

/** @function 	: OnLostDevice
	@brief 		: 장치가 소멸됐을때
*/
HRESULT CX2ToolMain::OnLostDevice()
{		
	if( m_pRenderTargetTex != NULL )
		m_pRenderTargetTex->OnLostDevice();
	if( m_pRenderTargetTopView != NULL )
		m_pRenderTargetTopView->OnLostDevice();
	if( m_pRenderTargetFrontView != NULL )
		m_pRenderTargetFrontView->OnLostDevice();

	for( unsigned i=0; i<vecpXSkinAnim.size(); i++ )
	{	
		vecpXSkinAnim[i]->OnLostDevice();
	}

	if( m_pXMeshBoundingSphere != NULL )
		m_pXMeshBoundingSphere->OnLostDevice();
	if(	m_pStartLineMesh != NULL )
		m_pStartLineMesh->OnLostDevice();
	if( m_pEndLineMesh != NULL )
		m_pEndLineMesh->OnLostDevice();
	if( m_pControlXMesh != NULL )
		m_pControlXMesh->OnLostDevice();
	if( m_pControlYMesh != NULL )
		m_pControlYMesh->OnLostDevice();
	if( m_pControlZMesh != NULL )
		m_pControlZMesh->OnLostDevice();
	if( m_pControlAxis != NULL )
		m_pControlAxis->OnLostDevice();

	if( m_pFont_A != NULL )
		m_pFont_A->OnLostDevice();
	if( m_pFont_B != NULL )
		m_pFont_B->OnLostDevice();

	if( m_pXSkinMesh != NULL )
		m_pXSkinMesh->OnLostDevice();
	if( m_pXSkinMeshNpc != NULL )
		m_pXSkinMeshNpc->OnLostDevice();

	for(UINT i=0; i<m_vecDungeonSkinAnimNpc.size(); ++i)
		m_vecDungeonSkinAnimNpc[i]->OnLostDevice();
	for(UINT i=0; i<m_vecDungeonSkinMeshNpc.size(); ++i)
		m_vecDungeonSkinMeshNpc[i]->OnLostDevice();

	if( g_pLine != NULL )
		g_pLine->OnLostDevice();
	if( g_pLineAxis != NULL )
		g_pLineAxis->OnLostDevice();
	if( g_pLineGrid != NULL )
		g_pLineGrid->OnLostDevice();

	return S_OK;
}

/** @function 	: MsgProc
	@brief 		: 메시지 프로시저
*/
bool CX2ToolMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_bEditLine == true || m_bEditWorld == true )
	{
		switch(uMsg)
		{
		#pragma region WM_LBUTTONDOWN
		case WM_LBUTTONDOWN:
			{
				m_bDrag = true;
				POINTS mousepos = MAKEPOINTS(lParam);				
				if( g_pX2Game != NULL )
				{
					m_vDragStartPos = g_pKTDXApp->GetProj3DPos(m_vRayLine[1]);
					m_vDragLineView[0] = g_pKTDXApp->GetProj3DPos(m_vRectRayLine);
										
					if( m_bEditLine == true )
					{
						if( m_bShowMoveAxis == true && (m_bMoveAxisX == true || m_bMoveAxisY == true || m_bMoveAxisZ == true) )
						{
							AddUndoList();

							m_bDrag = false;
							m_bMoveAxis = true;

							D3DXPLANE p;
							D3DXVECTOR3 vPivot = m_vecLineVertex[m_iVertPivot].vVertex;
							m_vPivot = vPivot;
							m_bFreeMoveLine = false;
							if( m_bMoveAxisX == true && m_bMoveAxisY == true && m_bMoveAxisZ == true )
							{
								m_bFreeMoveLine = true;
							}
							else if( m_bMoveAxisX == true && m_bMoveAxisY == true )
							{
								D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(0.f, 0.f, -1.f));
								D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
							}
							else if( m_bMoveAxisX == true && m_bMoveAxisZ == true )
							{
								D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(0.f, 1.f, 0.f));
								D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
							}
							else if( m_bMoveAxisY == true && m_bMoveAxisZ == true )
							{
								D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(1.f, 0.f, 0.f));
								D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
							}
							else if( m_bMoveAxisX == true )
							{									
								D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(0.f, 0.f, -1.f));
								D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
							}	
							else if( m_bMoveAxisY == true )
							{
								D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(0.f, 0.f, -1.f));
								D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
							}
							else if( m_bMoveAxisZ == true )
							{
								D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(0.f, 1.f, 0.f));
								D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
							}

							if( m_bLShift == true && m_bEditLineVertex == false && m_bEditStartPosition == false )
							{
								m_bCopyLine = true;
								CopyLine();
							}
						}
						else if( m_bEditStartPosition == true )
						{
							if( m_iOverStartPosition >= 0 )
							{
								m_iPickedStartPosition = m_iOverStartPosition;
								m_bMoveStartPosition = true;
								m_iOverStartPosition = -1;

								CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
								if( pMain != NULL )
								{							
									pMain->m_wndStartPosition.m_LineStartPositionDlg.UpdateStartPosition(m_iPickedStartPosition);
								}
							}
							else
							{
								m_iPickedStartPosition = -1;
								m_bMoveStartPosition = false;
							}
						}
					}
					else if( m_bEditWorld == true && m_bSelectedWorldObject == true )
					{
						if( m_bPickMoveWorldObjectPivot == true || 
							( m_bShowMoveAxis == true && ( m_bMoveAxisX == true || m_bMoveAxisY == true || m_bMoveAxisZ == true ) ) )
						{
							if( m_bMoveWorldObject == true && m_bLShift == true )
							{	
								CopyWorldObject();
							}
							else
							{
								CX2World *pWorld = g_pX2Game->GetWorld();
								vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

								tWorldElemGroup tUndoGroup;																
								for(UINT i=0; i<vecWorldObjectList.size(); ++i)
								{
									CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
									if( pWorldObject != NULL && pWorldObject->m_bSelected == true )
									{
										if( m_bMoveWorldObject == true )
										{											
											tWorldElem tUndoElem;
											tUndoElem.m_pWorldObject = pWorldObject;
											tUndoElem.m_eFlag = tWorldElem::UF_FLOAT;
											tUndoElem.m_iEid = PI_EW_WORLD_OBJECT_POSITION_X;											
											tUndoElem.m_fValue = pWorldObject->GetPos().x;
											tUndoGroup.m_vecUndoGroup.push_back(tUndoElem);
											tUndoElem.m_iEid = PI_EW_WORLD_OBJECT_POSITION_Y;
											tUndoElem.m_fValue = pWorldObject->GetPos().y;
											tUndoGroup.m_vecUndoGroup.push_back(tUndoElem);
											tUndoElem.m_iEid = PI_EW_WORLD_OBJECT_POSITION_Z;
											tUndoElem.m_fValue = pWorldObject->GetPos().z;
											tUndoGroup.m_vecUndoGroup.push_back(tUndoElem);
										}
										else if( m_bRotationWorldObject == true )
										{
											D3DXVECTOR3 vRotation = pWorldObject->GetMatrix().GetRotateDegree();
											tWorldElem tUndoElem;
											tUndoElem.m_pWorldObject = pWorldObject;
											tUndoElem.m_eFlag = tWorldElem::UF_FLOAT;
											tUndoElem.m_iEid = PI_EW_WORLD_OBJECT_ROTATION_X;											
											tUndoElem.m_fValue = vRotation.x;
											tUndoGroup.m_vecUndoGroup.push_back(tUndoElem);
											tUndoElem.m_iEid = PI_EW_WORLD_OBJECT_ROTATION_Y;
											tUndoElem.m_fValue = vRotation.y;
											tUndoGroup.m_vecUndoGroup.push_back(tUndoElem);
											tUndoElem.m_iEid = PI_EW_WORLD_OBJECT_ROTATION_Z;
											tUndoElem.m_fValue = vRotation.z;
											tUndoGroup.m_vecUndoGroup.push_back(tUndoElem);
										}
										else if( m_bScaleWorldObject == true )
										{
											D3DXVECTOR3 vScale = pWorldObject->GetMatrix().GetScale();
											tWorldElem tUndoElem;
											tUndoElem.m_pWorldObject = pWorldObject;
											tUndoElem.m_eFlag = tWorldElem::UF_FLOAT;
											tUndoElem.m_iEid = PI_EW_WORLD_OBJECT_SCALE_X;											
											tUndoElem.m_fValue = pWorldObject->GetMatrix().GetXScale();
											tUndoGroup.m_vecUndoGroup.push_back(tUndoElem);
											tUndoElem.m_iEid = PI_EW_WORLD_OBJECT_SCALE_Y;
											tUndoElem.m_fValue = pWorldObject->GetMatrix().GetYScale();
											tUndoGroup.m_vecUndoGroup.push_back(tUndoElem);
											tUndoElem.m_iEid = PI_EW_WORLD_OBJECT_SCALE_Z;
											tUndoElem.m_fValue = pWorldObject->GetMatrix().GetZScale();
											tUndoGroup.m_vecUndoGroup.push_back(tUndoElem);
										}
									}
								}

								if( tUndoGroup.m_vecUndoGroup.size() >= 1 )
									AddUndoWorld(tUndoGroup);
							}
						}

						if( m_bPickMoveWorldObjectPivot == true )
						{
								m_bMoveWorld = true;
						}
						else
						{
							if( m_bShowMoveAxis == true && (m_bMoveAxisX == true || m_bMoveAxisY == true || m_bMoveAxisZ == true) )
							{
								m_bDrag = false;
								m_bMoveAxis = true;

								D3DXPLANE p;
								D3DXVECTOR3 vPivot = m_vMoveWorldObjectPivot;
								m_vPivot = vPivot;
								if( m_bMoveAxisX == true && m_bMoveAxisY == true )
								{
									D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(0.f, 0.f, -1.f));
									D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
								}
								else if( m_bMoveAxisX == true && m_bMoveAxisZ == true )
								{
									D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(0.f, 1.f, 0.f));
									D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
								}
								else if( m_bMoveAxisY == true && m_bMoveAxisZ == true )
								{
									D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(1.f, 0.f, 0.f));
									D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
								}
								else if( m_bMoveAxisX == true )
								{									
									D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(0.f, 0.f, -1.f));
									D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
								}	
								else if( m_bMoveAxisY == true )
								{
									D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(0.f, 0.f, -1.f));
									D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
								}
								else if( m_bMoveAxisZ == true )
								{
									D3DXPlaneFromPointNormal(&p, &vPivot, &D3DXVECTOR3(0.f, 1.f, 0.f));
									D3DXPlaneIntersectLine( &m_vPivot, &p, &m_vRayLine[0], &m_vRayLine[1] );
								}
							}
						}						
					}					
				}
			}
			break;
		#pragma endregion 마우스 왼쪽 버튼 다운

		#pragma region WM_LBUTTONUP
		case WM_LBUTTONUP:
			{
				m_bDrag = false;				
				m_bCopyLine = false;
				m_bMoveStartPosition = false;
				
				#pragma region m_bEditLine
				if ( m_bEditLine == true )
				{
					CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

					if ( m_bMoveAxis == false )
					{
						if ( m_bLShift == true && GetSelectedLineVertCount() <= 0 )
						{
							if ( g_pX2Game != NULL )
							{
								int iLineCount = pLineMap->GetNumLineData();
								for ( int i = 0; i < iLineCount; ++i )
								{
									const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
									if ( pLineData != NULL )
									{
										D3DXVECTOR3 vPoint1, vPoint2;
										bool bFind = CCollision::LineToLine( pLineData->startPos, pLineData->endPos,
																			 m_vRayLine[0], m_vRayLine[1], 30.f, vPoint1, vPoint2 );
										if ( bFind == true )
										{	
											AddUndoList();
											if( m_bEditStartPosition == false )
											{
												AddVertex(vPoint1, i);
											}
											else
											{
												int iKey = AddStartPosition(vPoint1, i);

												CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
												if( m_bCheckNpc == true && pMain->m_DlgDungeonEditor.GetSelectedReadyNpc() != CX2UnitManager::NUI_NONE )
												{
													pMain->m_DlgDungeonEditor.AddNpcData(iKey, i);
												}
											}
											
											break;
										}
									}
								}
							}
						}
						else
						{
							m_iVertPivot = -1;

							if( m_bEditLine == true && m_bEditLineVertex == true && m_bEditStartPosition == false )
							{
								for( UINT i = 0; i < m_vecLineVertex.size(); ++i )
								{
									if( m_vecLineVertex[i].m_bOver == true )
									{
										if( m_bLCtrl == false )															 
											m_vecLineVertex[i].m_bSelected = true;							
										else
											m_vecLineVertex[i].m_bSelected = !m_vecLineVertex[i].m_bSelected;
									}
									else
									{
										if( m_bLCtrl == false )
											m_vecLineVertex[i].m_bSelected = false;							
									}

									if( m_vecLineVertex[i].m_bSelected == true )
										m_iVertPivot = i;
								}
							}
							else if( m_bEditLine == true && m_bEditLineVertex == false )
							{
								for( int i = 0; i < pLineMap->GetNumLineData(); ++i )
								{									
									CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);
									if( pLineData->m_bOver == true )
									{
										if( m_bLCtrl == false )
											pLineData->m_bSelected = true;
										else
											pLineData->m_bSelected = !pLineData->m_bSelected;
									}
									else
									{
										if( m_bLCtrl == false )
											pLineData->m_bSelected = false;
									}

									CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
									if( pMain != NULL )
									{
										if( pLineData->m_bSelected == true )
										{
											pMain->m_DlgLineBaseEditor.SetCheck(i, true);
										}
										else
										{
											pMain->m_DlgLineBaseEditor.SetCheck(i, false);
										}
									}
									
								}

								for( UINT i = 0; i < m_vecLineVertex.size(); ++i )
									m_vecLineVertex[i].m_bSelected = false;

								for( int i = 0; i < pLineMap->GetNumLineData(); ++i )
								{
									vector<int> vecVertIndex;
									const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData( i );
									
									if( pLineData->m_bSelected == true )
									{
										GetVertIndexByLine(vecVertIndex, i);
										for( UINT j = 0; j < vecVertIndex.size(); ++j )
										{
											int iIndex = vecVertIndex[j];
											m_vecLineVertex[iIndex].m_bSelected = true;											
											m_iVertPivot = iIndex;
										}
									}
								}
							}
						}
					}
					else
					{						
						if( m_bSnap == false && m_bMoveAxis == true && (m_bMoveAxisX == true || m_bMoveAxisY == true || m_bMoveAxisZ == true) )
						{
							pLineMap->UpdateLineData();
						}
						else if( m_bSnap == true )
						{	
							ParsingLine();
							ReMakeLineData();
						}
					}

					InitFillet();
					CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
					if( pMain != NULL )
						pMain->m_DlgLineBaseEditor.SetFilletValue(0);
				}
				#pragma endregion 라인 수정
				#pragma region m_bEditWorld
				else if( m_bEditWorld == true )
				{
					if( m_bAddObjectByLib == true && m_bLShift == true )
					{
						if( m_wstrSelectedLib.empty() == false )
						{
							AddWorldObject( m_vPickPos );
						}
					}
					else
					{
						CX2World *pWorld = g_pX2Game->GetWorld();
						vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

						for(UINT i=0; i<vecWorldObjectList.size(); ++i)
						{
							CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();

							if( m_bMoveWorld == false && m_bMoveAxis == false )
							{
								if( pWorldObject != NULL )
								{
									if( m_bLCtrl == true )
									{
										if( pWorldObject->m_bOver == true )
										{
											pWorldObject->m_bSelected = !pWorldObject->m_bSelected;
										}
									}
									else
									{
										pWorldObject->m_bSelected = false;
									}
								}

								CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
								if( pMain != NULL )
									pMain->m_DlgWorldEditor.SetCurSel( i, pWorldObject->m_bSelected );
							}
							else if( pWorldObject->m_bSelected == true )
							{
								CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
								if( pMain != NULL )
								{
									if( m_bMoveWorldObject == true )
									{
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_POSITION_X, (_variant_t)(float)(pWorldObject->GetPos().x));
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_POSITION_Y, (_variant_t)(float)(pWorldObject->GetPos().y));
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_POSITION_Z, (_variant_t)(float)(pWorldObject->GetPos().z));
									}
									else if( m_bRotationWorldObject == true )
									{
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_POSITION_X, (_variant_t)(float)(pWorldObject->GetPos().x));
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_POSITION_Y, (_variant_t)(float)(pWorldObject->GetPos().y));
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_POSITION_Z, (_variant_t)(float)(pWorldObject->GetPos().z));

										D3DXVECTOR3 vRotation = pWorldObject->GetMatrix().GetRotateDegree();
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_ROTATION_X, (_variant_t)(float)vRotation.x);
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_ROTATION_Y, (_variant_t)(float)vRotation.y);
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_ROTATION_Z, (_variant_t)(float)vRotation.z);
									}
									else if( m_bScaleWorldObject == true )
									{
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_POSITION_X, (_variant_t)(float)(pWorldObject->GetPos().x));
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_POSITION_Y, (_variant_t)(float)(pWorldObject->GetPos().y));
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_POSITION_Z, (_variant_t)(float)(pWorldObject->GetPos().z));

										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_SCALE_X, (_variant_t)(float)(pWorldObject->GetMatrix().GetXScale()));
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_SCALE_Y, (_variant_t)(float)(pWorldObject->GetMatrix().GetYScale()));
										pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_SCALE_Z, (_variant_t)(float)(pWorldObject->GetMatrix().GetZScale()));
									}								
								}
								break;
							}
						}
					}					
				}
				#pragma endregion 월드 메쉬 수정

				m_bFreeMoveLine = false;
				m_bMoveAxis = false;
				m_bMoveAxisX = false;
				m_bMoveAxisY = false;
				m_bMoveAxisZ = false;
				m_bMoveWorld = false;
				m_bPickMoveWorldObjectPivot = false;

				if( m_bEditLine == true )
				{
					if( GetSelectedLineVertCount() == 1 && m_iVertPivot >= 0 )
					{
						UpdateVertexProperty();					
					}
					else
					{
						UpdateLineProperty();
					}

					if( IsValidLine() == false )
					{
						MessageBox(NULL, L"라인정보가 이상합니다. 현재 라인들을 정상 처리할 수 없습니다.", L"알림!", MB_OK);
					}
				}				
			}
			break;
		#pragma endregion 마우스 왼쪽 버튼 업

		#pragma region WM_MOUSEMOVE
		case WM_MOUSEMOVE:
			{
				if( g_pX2Game != NULL )
				{
					D3DXVECTOR3 vRayDir;
					mousepos = MAKEPOINTS(lParam);
					D3DXMATRIX matView, matProj;
					matView = g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix();
					g_pKTDXApp->GetProjectionTransform( &matProj );
					ComputeRay( &m_vRayLine[0], &vRayDir, mousepos, &matView, &matProj);

					const float MAGIC_RAY_RANGE = 50000.f;
					if( m_bEditWorld == true )
					{
						m_vRayLine[0] = m_vRayLine[0] - vRayDir * MAGIC_RAY_RANGE;
						m_vRayLine[1] = m_vRayLine[0] + vRayDir * MAGIC_RAY_RANGE * 100.f;
					}
					else
					{
						m_vRayLine[1] = m_vRayLine[0] + vRayDir * MAGIC_RAY_RANGE;
					}

					m_vMousePos = g_pKTDXApp->GetProj3DPos(m_vRayLine[1]);
					
					m_vRectRayLine = m_vRayLine[0] + vRayDir * 10000.f;
					m_vDragLineView[1] = g_pKTDXApp->GetProj3DPos(m_vRectRayLine);

					#pragma region m_bEditLine
					if( m_bEditLine == true )
					{	
						if( m_bMoveAxis == true && (m_bMoveAxisX == true || m_bMoveAxisY == true || m_bMoveAxisZ == true) )
						{
							float fMoveDist = 0.f;
							D3DXVECTOR3 vPlanePos;
							D3DXVECTOR3 vAfterMoveGap;						
							D3DXVECTOR3 vPos;
							D3DXPLANE p;

							for(UINT i=0; i<m_vecLineVertex.size(); ++i)
							{
								if( m_vecLineVertex[i].m_bSelected == true && i == m_iVertPivot )
								{

									vPlanePos = m_vPivot; //m_vecLineVertex[i].vVertex;
									if( m_bSnap == false )
									{
										if( m_bFreeMoveLine )
										{
											vPos = m_vFreeMoveLinePos;
										}
										else if( m_bMoveAxisX == true && m_bMoveAxisY == true )
										{
											D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(0.f, 0.f, -1.f));
											D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
										}
										else if( m_bMoveAxisX == true && m_bMoveAxisZ == true )
										{
											D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(0.f, 1.f, 0.f));
											D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
										}
										else if( m_bMoveAxisY == true && m_bMoveAxisZ == true )
										{
											D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(1.f, 0.f, 0.f));
											D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
										}
										else if( m_bMoveAxisX == true )
										{									
											D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(0.f, 0.f, -1.f));
											D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
										}	
										else if( m_bMoveAxisY == true )
										{
											D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(0.f, 0.f, -1.f));
											D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
										}
										else if( m_bMoveAxisZ == true )
										{
											D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(0.f, 1.f, 0.f));
											D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
										}

										vAfterMoveGap = vPos - vPlanePos;
										fMoveDist = GetDistance( vPos, vPlanePos );
									}
									else
									{
										bool bSnap = false;
										for(UINT iSnap = 0; iSnap<m_vecLineVertex.size(); ++iSnap)
										{
											if( m_vecLineVertex[iSnap].m_bSelected == true || m_vecLineVertex[iSnap].m_vecLineInfo.size() >= 2 )
												continue;

											bSnap = CCollision::LineToSphere(m_vRayLine[0], m_vRayLine[1], 20.f, m_vecLineVertex[iSnap].vVertex, 10.f, NULL);
											if( bSnap == true )
											{											
												vAfterMoveGap = m_vecLineVertex[iSnap].vVertex - m_vecLineVertex[i].vVertex;
												break;
											}
										}

										if( bSnap == false )
										{
											vAfterMoveGap = D3DXVECTOR3(0.f, 0.f, 0.f);
										}
									}

									break;
								}
							}

							for(UINT i=0; i<m_vecLineVertex.size(); ++i)
							{
								if( m_vecLineVertex[i].m_bSelected == true )
								{
									if( m_bSnap == true )
									{		
										m_vecLineVertex[i].vVertex.x += vAfterMoveGap.x;
										m_vecLineVertex[i].vVertex.y += vAfterMoveGap.y;
										m_vecLineVertex[i].vVertex.z += vAfterMoveGap.z;
									}
									else
									{
										if( m_bFreeMoveLine == true )
										{
											m_vecLineVertex[i].vVertex.x += vAfterMoveGap.x;
											m_vecLineVertex[i].vVertex.y += vAfterMoveGap.y;
											m_vecLineVertex[i].vVertex.z += vAfterMoveGap.z;
										}
										else if( m_bMoveAxisX == true && m_bMoveAxisY == true )
										{
											m_vecLineVertex[i].vVertex.x += vAfterMoveGap.x;
											m_vecLineVertex[i].vVertex.y += vAfterMoveGap.y;
										}
										else if( m_bMoveAxisX == true && m_bMoveAxisZ == true )
										{
											m_vecLineVertex[i].vVertex.x += vAfterMoveGap.x;
											m_vecLineVertex[i].vVertex.z += vAfterMoveGap.z;
										}
										else if( m_bMoveAxisY == true && m_bMoveAxisZ == true )
										{
											m_vecLineVertex[i].vVertex.y += vAfterMoveGap.y;
											m_vecLineVertex[i].vVertex.z += vAfterMoveGap.z;
										}
										else if( m_bMoveAxisX == true )
										{
											m_vecLineVertex[i].vVertex.x += vAfterMoveGap.x;
										}
										else if( m_bMoveAxisY == true )									
										{
											m_vecLineVertex[i].vVertex.y += vAfterMoveGap.y;
										}
										else if( m_bMoveAxisZ == true )
										{
											m_vecLineVertex[i].vVertex.z += vAfterMoveGap.z;
										}

										if( i== m_iVertPivot )
										{
											m_vPivot = m_vPivot + vAfterMoveGap;
										}
									}
									
									CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
									for(UINT j=0; j<m_vecLineVertex[i].m_vecLineInfo.size(); ++j)
									{
										CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData( m_vecLineVertex[i].m_vecLineInfo[j].m_iLineIndex );
										if( pLineData != NULL )
										{
											if( m_vecLineVertex[i].m_vecLineInfo[j].m_bStartPos == true )
												pLineData->startPos = m_vecLineVertex[i].vVertex;
											else
												pLineData->endPos = m_vecLineVertex[i].vVertex;

											g_pX2Game->GetWorld()->GetLineMap()->MakeLine(pLineData);
										}
									}
								}							
							}
						}
					}
					#pragma endregion 라인 수정
					#pragma region m_bEditWorld
					else if( m_bEditWorld == true )
					{
						if( m_bMoveAxis == true && (m_bMoveAxisX == true || m_bMoveAxisY == true || m_bMoveAxisZ == true) )
						{
							float fMoveDist = 0.f;
							D3DXVECTOR3 vPlanePos;
							D3DXVECTOR3 vAfterMoveGap;						
							D3DXVECTOR3 vPos;
							D3DXPLANE p;

							CX2World *pWorld = g_pX2Game->GetWorld();
							vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

							for ( UINT i = 0; i < vecWorldObjectList.size(); ++i )
							{
								CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();

								if( pWorldObject != NULL && pWorldObject->m_bSelected == true && i == m_iVertPivot )
								{
									vPlanePos = m_vPivot;

									if( m_bMoveAxisX == true && m_bMoveAxisY == true )
									{
										D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(0.f, 0.f, -1.f));
										D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
									}
									else if( m_bMoveAxisX == true && m_bMoveAxisZ == true )
									{
										D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(0.f, 1.f, 0.f));
										D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
									}
									else if( m_bMoveAxisY == true && m_bMoveAxisZ == true )
									{
										D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(1.f, 0.f, 0.f));
										D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
									}
									else if( m_bMoveAxisX == true )
									{									
										D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(0.f, 0.f, -1.f));
										D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
									}	
									else if( m_bMoveAxisY == true )
									{
										D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(0.f, 0.f, -1.f));
										D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
									}
									else if( m_bMoveAxisZ == true )
									{
										D3DXPlaneFromPointNormal(&p, &vPlanePos, &D3DXVECTOR3(0.f, 1.f, 0.f));
										D3DXPlaneIntersectLine( &vPos, &p, &m_vRayLine[0], &m_vRayLine[1] );
									}

									vAfterMoveGap = vPos - vPlanePos;
									fMoveDist = GetDistance( vPos, vPlanePos );

									break;
								}
							}
							
							if( m_bMoveWorldObject == true )
							{
								MoveWorldObject(vAfterMoveGap);
							}
							else if( m_bRotationWorldObject == true )
							{
								bool bGroup = false;
								if( m_bLShift == true )
									bGroup = true;
								RotationWorldObject(vAfterMoveGap, bGroup);
							}
							else if( m_bScaleWorldObject == true )
							{
								ScaleWorldObject(vAfterMoveGap);
							}
						}	
					}
					#pragma endregion 월드 메시 수정
				}				
			}
			break;
			#pragma endregion 마우스 이동

		case WM_KEYDOWN:
			{
				short KeyState = GetKeyState(VK_LCONTROL);
				KeyState = KeyState >> 15;
				if( KeyState == -1 )
				{
					if( wParam == (int)'Z' || wParam == (int)'z')
					{
						if( m_bEditLine == true )
							UnDoLine();
						else if( m_bEditWorld == true )
							UndoWorld();

						return true;
					}
					else if( wParam == 'a' || wParam == 'A' )
					{
						if( m_bEditLine == true )
						{
							CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

							if( m_bEditLineVertex == true )
							{
								for(UINT i=0; i<m_vecLineVertex.size(); ++i)
								{
									m_vecLineVertex[i].m_bSelected = true;
									m_iVertPivot = i;
								}
							}
							else
							{
								for(int i=0; i<pLineMap->GetNumLineData(); ++i)
								{									
									CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);										
									pLineData->m_bSelected = true;
								}

								for(UINT i=0; i<m_vecLineVertex.size(); ++i)
									m_vecLineVertex[i].m_bSelected = false;

								for(int i=0; i<pLineMap->GetNumLineData(); ++i)
								{
									vector<int> vecVertIndex;
									const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
									if( pLineData->m_bSelected == true )
									{
										GetVertIndexByLine(vecVertIndex, i);
										for(UINT j=0; j<vecVertIndex.size(); ++j)
										{
											int iIndex = vecVertIndex[j];
											m_vecLineVertex[iIndex].m_bSelected = true;											
											m_iVertPivot = iIndex;
										}
									}
								}
							}

							m_bMoveAxis = false;
							m_bMoveAxisX = false;
							m_bMoveAxisY = false;
							m_bMoveAxisZ = false;

							InitFillet();
							CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
							if( pMain != NULL )
								pMain->m_DlgLineBaseEditor.SetFilletValue(0);	

							if( GetSelectedLineVertCount() == 1 && m_iVertPivot >= 0 )
							{
								UpdateVertexProperty();					
							}
							else
							{
								UpdateLineProperty();
							}

							return true;
						}
					}
				}

				switch( wParam )
				{
				case VK_DELETE:
					{
						if( m_bEditLine == true )
						{
							DoDeleteEditLine();
						}
						else if( m_bEditWorld == true )
						{
							CX2World *pWorld = g_pX2Game->GetWorld();
							tAddRmoveWorldElem tUndoElem;
							tUndoElem.m_bAdded = false;

							vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
							for(UINT i=0; i<vecWorldObjectList.size(); ++i)
							{
								CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
								if( pWorldObject != NULL && pWorldObject->m_bSelected == true )
								{
									tUndoElem.m_vecWorldObject.push_back( vecWorldObjectList[i] );
									g_pX2Game->GetWorld()->DeleteWorldObject4Undo(pWorldObject); // RemoveWorldObject(pWorldObject);									
								}
							}

							CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
							pMain->m_DlgWorldEditor.ResetWorldObject();

							AddUndoWorld( tUndoElem );
						}
					}
					break;
				case VK_ADD:
					{
						m_fScaleAxis += 0.5f;
						if( m_fScaleAxis > 2.f )
							m_fScaleAxis = 2.f;
					}
					return true;
				case VK_SUBTRACT:
					{
						m_fScaleAxis -= 0.5f;
						if( m_fScaleAxis < 1.f )
							m_fScaleAxis = 1.f;
					}
					return true;
#if 1
				case VK_F1:
					{
						// front
						D3DXVECTOR3 vLookAt = D3DXVECTOR3(0.f, 0.f, 0.f);
						if( m_iVertPivot >= 0 )
							vLookAt = m_vecLineVertex[m_iVertPivot].vVertex;
						if( m_bEditLine == true )
							SetCameraView(0, vLookAt);
					}
					break;
				case VK_F2:
					{
						// side
						D3DXVECTOR3 vLookAt = D3DXVECTOR3(0.f, 0.f, 0.f);
						if( m_iVertPivot >= 0 )
							vLookAt = m_vecLineVertex[m_iVertPivot].vVertex;
						if( m_bEditLine == true )
							SetCameraView(1, vLookAt);
					}
					break;
				case VK_F3:
					{
						// top
						D3DXVECTOR3 vLookAt = D3DXVECTOR3(0.f, 0.f, 0.f);
						if( m_iVertPivot >= 0 )
							vLookAt = m_vecLineVertex[m_iVertPivot].vVertex;
						if( m_bEditLine == true )
							SetCameraView(2, vLookAt);
					}
					break;
#endif
				case WM_CHAR:
					{
												
					}
					break;
				}
			}
			break;
// 		case WM_CHAR:
// 			{
// 				switch(wParam)
// 				{	
// 				case 'z':
// 				case 'Z':
// 					{
// 						if( m_bLCtrl == true )
// 						{
// 							UnDoLine();
// 							return true;
// 						}
// 					}
// 					break;
// 				default:
// 					break;
// 				}
// 			}
// 			break;
		}
	}

	CX2Main::MsgProc( hWnd, uMsg, wParam, lParam );
	
	if( g_pX2Game != NULL )
		g_pX2Game->MsgProc( hWnd, uMsg, wParam, lParam );

	return true;
}

/** @function 	: InitX2Data
	@brief 		: 엘소드 데이터 초기화, 임시로 Buff 초기화 코드를 넣음 X2StateStartUp에서 한 번 더 수행하는 문제가 있음
*/
void CX2ToolMain::InitX2Data()
{
	 lua_State* L = g_pKTDXApp->GetLuaBinder()->GetLuaState();

	lua_tinker::class_add<CX2EnumManager>(L, "KX2EnumManager");
	lua_tinker::class_def<CX2EnumManager>(L, "AddWorldEnumData_LUA",			&CX2EnumManager::AddWorldEnumData_LUA);
	lua_tinker::class_def<CX2EnumManager>(L, "AddNpcEnumData_LUA",				&CX2EnumManager::AddNpcEnumData_LUA);

	if( !m_enumManager.OpenScriptFile( L"EnumString.lua" ) )
		MessageBox( g_pKTDXApp->GetHWND(), L"EnumString.lua파일을 못읽음!", L"Error!", MB_OK );

	g_pData->ResetServerProtocol();
	g_pData->GetServerProtocol()->ConnectedToGameServerForWorldTool();

	// UDP 초기화
	g_pData->ResetGameUDP( g_pMain->GetGameP2PPort() );

	// UI 파티클 시스템 생성
	g_pData->ResetUIMajorParticle( L"UIMajorParticle.txt" );
	g_pData->ResetUIMinorParticle( L"UIMinorParticle.txt" );

	// UI 메시플레이어
	g_pData->ResetUIMajorXMeshPlayer( L"UIMajorXMeshPlayer.txt" );
	g_pData->ResetUIMinorXMeshPlayer( L"UIMinorXMeshPlayer.txt" );

	// UI 이펙트셋
	g_pData->ResetUIEffectSet( L"UIEffectSet.lua" );

	// Buff Templet, Factor
	CX2BuffTempletManager::GetInstance()->OpenScriptFileForTemplet();
	CX2BuffTempletManager::GetInstance()->OpenScriptFileForFactor();

	if( NULL != g_pMain && NULL != g_pMain->GetKeyPad() )
	{
		g_pMain->GetKeyPad()->InitKeySlotData();
	}

	// 유닛 데이타
	g_pData->ResetUnitManager( L"UnitTemplet.lua", L"NPCTemplet.lua", L"NPCStat.lua" );
	
	// 아이템 데이타
	g_pData->ResetItemManager();
	g_pData->ResetTitleManager();
	g_pData->ResetPetManager();

	// 월드 데이타
	g_pData->ResetWorldManager( L"WorldData.lua" );	

	// 던전 데이타
	g_pData->ResetDungeonManager( L"DungeonData.lua" );
	g_pData->GetDungeonManager()->OpenScriptFile(L"ToolDungeonData.lua");
	
	// 스킬 데이터
	//g_pData->ResetSkillTree( L"NewSkillTemplet.lua", L"SkillData.lua" );
	g_pData->ResetSkillTree( L"NewSkillTempletVer2.lua", L"SkillData.lua" );

	// 슬라이드 샷
	g_pData->ResetSlideShot();

	// UI 관리자
	g_pData->ResetUIManager();
	
	// 공용 장치
	g_pData->LoadCommonDevice();
	
	// 데미지 매니저
	g_pData->ResetDamageManager();

	// 글꼴
	m_pFont_A	= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_13_SEMIBOLD );
	m_pFont_B	= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_NORMAL_MAPPED );

	// 툴용 엘소드
	m_pMatrix = new CKTDGMatrix( g_pKTDXApp->GetDevice() );
	m_pXSkinMesh = NULL;
	m_pXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"tool_pc_elsword.x" );
	
	// 툴용 Npc
	m_pXSkinMeshNpc = NULL;
	m_pXSkinMeshNpc = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"tool_Npc_Zero.x" );

	// 라인 생성
	D3DXCreateLine( g_pKTDXApp->GetDevice(), &g_pLine );
	g_pLine->SetWidth(2.f);	
	
	D3DXCreateLine( g_pKTDXApp->GetDevice(), &g_pLineAxis );
	g_pLineAxis->SetWidth( 3.f );
	g_pLineAxis->SetAntialias( TRUE );
	
	D3DXCreateLine( g_pKTDXApp->GetDevice(), &g_pLineGrid );
	g_pLineGrid->SetWidth( 0.5f );
	g_pLineGrid->SetAntialias( TRUE );
	
	// 기본 3D 리소스 로딩
	m_pXMeshBoundingSphere = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"sphere0.x" );
	m_pStartLineMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"ui_line_start.x" );
	m_pEndLineMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"ui_line_end.x" );
	m_pControlXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"ui_control_x_red.x" );
	m_pControlYMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"ui_control_y_blue.x" );
	m_pControlZMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"ui_control_z_green.x" );
	m_pControlAxis = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Ui_tool_Gizmo.x" );

	// 메인 프레임 얻기
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain != NULL )
	{
		pMain->m_DlgSelectUnit.InitToolunit();
	}	
}

/** @function 	: ResetTool
	@brief 		: 툴 재설정
*/
void CX2ToolMain::ResetTool( bool bEditLine, bool bEditWorld, bool bViewLine, bool bViewWorld, bool bEditTrigger )
{
	m_bEditLine = bEditLine;
	m_bEditWorld = bEditWorld;
	m_bViewLine = bViewLine;
	m_bViewWorld = bViewWorld;
	m_bEditTrigger = bEditTrigger;

	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain != NULL )
	{
		pMain->OnResetMain( bEditLine, bEditWorld, bViewLine, bViewWorld, bEditTrigger );
	}
}

/** @function 	: SetUserCamera
	@brief 		: 유저 카메라 설정
*/
void CX2ToolMain::SetUserCamera(bool bVal)
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();

	m_bUserCamera = bVal;
	if( m_bUserCamera == true )
	{
		m_fPerspective = g_pKTDXApp->GetDGManager()->GetPerspectiveValue();
		g_pKTDXApp->GetDGManager()->SetPerspectiveValue( 10.f );

		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
			g_pKTDXApp->GetDGManager()->GetFar(), true );

		pMain->m_DlgWorldEditor.GetDlgItem(IDC_EDIT_WORLD_PERSPECTIVE)->SetWindowTextW( L"10");
	}
	else
	{
		g_pKTDXApp->GetDGManager()->SetPerspectiveValue( m_fPerspective );

		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
			g_pKTDXApp->GetDGManager()->GetFar(), true );

		char buf[100];
		wstring wstrFrustum = L"";
		sprintf(buf, "%5.2f", m_fPerspective);	
		ConvertCharToWCHAR( wstrFrustum, buf );

		pMain->m_DlgWorldEditor.GetDlgItem(IDC_EDIT_WORLD_PERSPECTIVE)->SetWindowTextW( wstrFrustum.c_str() );
	}	
}

/** @function 	: RefreshNpcData
	@brief 		: Npc 데이터 재활성
*/
void CX2ToolMain::RefreshNpcData(int iKey)
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->m_DlgDungeonEditor.RefreshNpcData(iKey);
}

/** @function 	: SetCameraView
	@brief 		: 카메라뷰 설정
*/
void CX2ToolMain::SetCameraView(int iView, D3DXVECTOR3 vFocus)
{
	CX2Camera *pGameCamera = g_pX2Game->GetX2Camera();
	if( pGameCamera != NULL )
	{
		CKTDGCamera ViewCamera = pGameCamera->GetCamera();
		CFirstPersonCamera *pFpsCamera = g_pX2Game->GetFPSCamera();

		D3DXVECTOR3 vEye = vFocus;
		D3DXVECTOR3 vLookAt = vFocus;
		D3DXVECTOR3 vUpY = D3DXVECTOR3(0.f, 1.f, 0.f);
		D3DXVECTOR3 vUpZ = D3DXVECTOR3(0.f, 0.f, 1.f);

		switch(iView)
		{
		case 0:	// front
			{
				vEye.z -= 3500.f;
				ViewCamera.Point( vEye.x, vEye.y, vEye.z, vLookAt.x, vLookAt.y, vLookAt.z, 0.f, 1.f, 0.f );
				pFpsCamera->SetViewParams(&vEye, &vLookAt, &vUpY);
			}
			break;
		case 1: // side
			{
				vEye.x += 3500.f;
				ViewCamera.Point( vEye.x, vEye.y, vEye.z, vLookAt.x, vLookAt.y, vLookAt.z, 0.f, 1.f, 0.f );
				pFpsCamera->SetViewParams(&vEye, &vLookAt, &vUpY);					
			}
			break;
		case 2: // top
			{
				vEye.y += 3500.f;
				ViewCamera.Point( vEye.x, vEye.y, vEye.z, vLookAt.x, vLookAt.y, vLookAt.z, 0.f, 0.f, 1.f );
				pFpsCamera->SetViewParams(&vEye, &vLookAt, &vUpZ);
			}
			break;
		default:
			break;
		}
	}
}

/** @function 	: UpdateMonsterList
	@brief 		: 몬스터 리스트 갱신
*/
void CX2ToolMain::UpdateMonsterList()
{	

	//map<CX2UnitManager::NPC_UNIT_ID, CX2UnitManager::NPCUnitTemplet*>::const_iterator it;
	//const map<CX2UnitManager::NPC_UNIT_ID, CX2UnitManager::NPCUnitTemplet*>& mapNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTempletMap();

	const CX2UnitManager::UnitTempletMap& mapNPCTemplet = g_pData->GetUnitManager()->GetUnitTempletMap();
	CX2UnitManager::UnitTempletMap::const_iterator it;

	for( it = mapNPCTemplet.begin() ; it != mapNPCTemplet.end(); it++ )
	{
		/*CX2UnitManager::NPCUnitTemplet* pNPCTemplet = it->second;

		if( pNPCTemplet->m_LuaFileName.length() != 0 )
		{
			// 몬스터 리스트
		}*/

	}
}

/** @function 	: LoadConfig
	@brief 		: 높이맵 관련된 정보인 것 같다
*/
bool CX2ToolMain::LoadConfig()
{
	FILE *fp = fopen("X2ToolConfig.cfg", "r");
	if( fp != NULL )
	{
		
		char szLibPath[MAX_PATH];
		int iHeightMap = 0;
		//fwscanf( fp, L"%d%d%d%s",  &m_iWidht, &m_iHeight, &iHeightMap, wLibPath );	
		fscanf( fp, "%d%d%d",  &m_iWidth, &m_iHeight, &iHeightMap );
		if( fscanf( fp, "%s", szLibPath ) != EOF )
		{
			ConvertCharToWCHAR(m_wstrLibPath, szLibPath);
		}
		else
		{
			m_wstrLibPath = L"";
		}

		fclose(fp);		

		if( iHeightMap > 0 )
			m_bHeightMap = true;
		else 
			m_bHeightMap = false;

		if( m_wstrLibPath.empty() == false )
			g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory(m_wstrLibPath);

		return true;
	}

	m_bHeightMap = false;
	return false;
}

/** @function 	: CreateGame
	@brief 		: 게임 생성
*/
void CX2ToolMain::CreateGame( SEnum::DUNGEON_ID eDungeonId, bool bReset )
{
	UidType MAGIC_USER_UID				= 777;
	UidType MAGIC_UNIT_UID_ELSWORD 		= 777;

	// 던전게임 시뮬레이션시 셋팅함
	bool bCanPlay = m_bCanPlay;
	bool bIsNpcLoad = false;
	if( bCanPlay == true )
	{
		bIsNpcLoad = true;
		//g_pMain->SetNowStateID( CX2Main::XS_DUNGEON_GAME );
	}
	
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	CX2Unit::UNIT_CLASS eUnitClass = pMain->m_DlgSelectUnit.m_eUnitClass;
		
	m_tSkillData.Init();
	for(int i=0; i<4; ++i)
	{
		KSkillData skilldata;
		skilldata.m_iSkillID = (short)pMain->m_DlgSelectUnit.m_iSkillId[i];
		skilldata.m_cSkillLevel = 5;		
		m_tSkillData.m_aEquippedSkill[i] = skilldata;
	}
	
	wstring wstrUnitName = pMain->m_DlgSelectUnit.m_wstrUnitName;
	int iUnitLevel = pMain->m_DlgSelectUnit.m_iLevel;
	UINT uHP = pMain->m_DlgSelectUnit.m_uHp;
	int iAtkPhysic = pMain->m_DlgSelectUnit.m_iAtkPhysic;
	int iAtkMagic = pMain->m_DlgSelectUnit.m_iAtkMagic;
	int iDefPhysic = pMain->m_DlgSelectUnit.m_iDefPhysic;
	int iDefMagic = pMain->m_DlgSelectUnit.m_iDefMagic;

	// Notice
#if 0
	{
		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
		if( pMain != NULL )
		{		
			if( pMain->m_DlgStartNotice.IsWindowVisible() == TRUE )
			{
				PWINDOWINFO pWInfo;
				pMain->m_DlgStartNotice.GetWindowInfo(pWInfo);
				
				POINT pt;
				GetCursorPos( &pt );

				if( pt.x > pWInfo->rcWindow.left && pt.x < pWInfo->rcWindow.right &&
					pt.y > pWInfo->rcWindow.top && pt.y < pWInfo->rcWindow.bottom )
				{
					pMain->m_DlgStartNotice.SetWindowPos(NULL, rand()%1024, rand()%768, 100, 100, SWP_NOSIZE);
					pMain->m_DlgStartNotice.SetActiveWindow();
				}
				return;
			}
		}
	}
#endif
	


	//if( bReset == false )
	//{
	// user data setup
	CX2User::UserData* pUserData = new CX2User::UserData;	
	pUserData->userUID			= MAGIC_USER_UID;
	pUserData->userID			= L"WorldToolUserID";
	pUserData->userAuthLevel	= CX2User::XUAL_NORMAL_USER;
	pUserData->IP				= L"127.0.0.1";
	pUserData->port				= 8765;	
	g_pData->ResetMyUser( *pUserData );


	// note!! 전직 추가되면 수정
	KUnitInfo kUnitInfo;
	CX2Unit* pUnit = NULL;
	UidType iUnitUID = MAGIC_UNIT_UID_ELSWORD;
	kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
	kUnitInfo.m_nUnitUID		= iUnitUID;
	kUnitInfo.m_cUnitClass		= (char)eUnitClass;
	kUnitInfo.m_wstrNickName	= wstrUnitName;
	kUnitInfo.m_ucLevel			= iUnitLevel;

	kUnitInfo.m_kStat.m_iBaseHP			= uHP;
	kUnitInfo.m_kGameStat.m_iBaseHP		= uHP;
	kUnitInfo.m_kStat.m_iAtkPhysic		= iAtkPhysic;
	kUnitInfo.m_kStat.m_iAtkMagic		= iAtkMagic;
	kUnitInfo.m_kGameStat.m_iAtkPhysic	= iAtkPhysic;
	kUnitInfo.m_kGameStat.m_iAtkMagic	= iAtkMagic;
	kUnitInfo.m_kStat.m_iDefPhysic		= iDefPhysic;
	kUnitInfo.m_kStat.m_iDefMagic		= iDefMagic;
	kUnitInfo.m_kGameStat.m_iDefPhysic	= iDefPhysic;
	kUnitInfo.m_kGameStat.m_iDefMagic	= iDefMagic;
				
	pUnit = new CX2Unit( kUnitInfo );
	pUnit->SetOwnerUserUID( MAGIC_USER_UID );
	g_pData->GetMyUser()->AddUnit( pUnit );
		
	// 태완 수정
	g_pData->GetMyUser()->SelectUnit( g_pData->GetMyUser()->GetUnitByUID( MAGIC_UNIT_UID_ELSWORD ) );
	//g_pData->GetMyUser()->SelectUnit( g_pData->GetMyUser()->GetUnitByUID( MAGIC_UNIT_UID_ELSWORD + 6 ) );

		
	std::map< CX2SkillTree::SKILL_ID, CX2SkillTree::SkillTreeTemplet > mapSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTempletMap4Tool((int)kUnitInfo.m_cUnitClass);
	std::map<CX2SkillTree::SKILL_ID, CX2SkillTree::SkillTreeTemplet>::iterator mit = mapSkillTreeTemplet.begin();

	vector<KUserSkillData> vecSkillAcquired;
	while(mit != mapSkillTreeTemplet.end() )
	{
		CX2SkillTree::SKILL_ID iSkillId = (CX2SkillTree::SKILL_ID)mit->first;
		CX2SkillTree::SkillTreeTemplet skillTreeTemplet = (CX2SkillTree::SkillTreeTemplet)mit->second;
		KUserSkillData userSkillData;
		userSkillData.m_iSkillID =  (int)iSkillId;// pSkillTemplet->m_iSkillID;
		userSkillData.m_cSkillLevel = skillTreeTemplet.m_iMaxSkillLevel;

		vecSkillAcquired.push_back(userSkillData);



		for(int iSkill=0; iSkill<4; ++iSkill)
		{
			if( m_tSkillData.m_aEquippedSkill[iSkill].m_iSkillID == iSkillId )
				m_tSkillData.m_aEquippedSkill[iSkill].m_cSkillLevel = skillTreeTemplet.m_iMaxSkillLevel;
		}
		++mit;
	}

	pUnit->AccessUnitData().m_UserSkillTree.SetAcquiredSkill( vecSkillAcquired );
	pUnit->AccessUnitData().m_UserSkillTree.SetEquippedSkill( m_tSkillData.m_aEquippedSkill, m_tSkillData.m_aEquippedSkillSlotB );

	CX2GageManager* pGageManager = CX2GageManager::GetInstance();

	if( NULL != pGageManager )
	{
		pGageManager->CreateMyGageUI( pUnit->GetClass(), pUnit->GetUID() );
	}
	//}
	

	CX2DungeonRoom *pRoom = NULL;
	pRoom = g_pData->ResetDungeonRoom();
// 	if( bReset == false )
// 		pRoom = g_pData->ResetDungeonRoom();
// 	else
// 		pRoom = g_pData->GetDungeonRoom();
	pRoom->SetDungeonID(eDungeonId);

	CX2Dungeon::DIFFICULTY_LEVEL eDifficulty = CX2Dungeon::DL_NORMAL;
	//eDifficulty = g_pData->GetDungeonManager()->GetDungeonData(eDungeonId)->m_eDifficulty;
	pRoom->SetDifficulty(eDifficulty);

	//if( bReset == false )
	//{
	std::vector<KRoomSlotInfo> vecKRoomSlotInfo;
		//{
	KRoomSlotInfo kRoomSlotInfo;
	kRoomSlotInfo.m_Index		= 0;
	kRoomSlotInfo.m_SlotState	= CX2Room::SS_LOADING;
	kRoomSlotInfo.m_bHost		= true;
	kRoomSlotInfo.m_bReady		= true;
	kRoomSlotInfo.m_TeamNum		= CX2Room::TN_RED;
	kRoomSlotInfo.m_kRoomUserInfo.m_ucLevel = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level;
	kRoomSlotInfo.m_kRoomUserInfo.m_iOwnerUserUID	= (UidType) MAGIC_USER_UID;
	kRoomSlotInfo.m_kRoomUserInfo.m_nUnitUID		= MAGIC_UNIT_UID_ELSWORD;
	kRoomSlotInfo.m_kRoomUserInfo.m_cUnitClass		= (char)eUnitClass;
	kRoomSlotInfo.m_kRoomUserInfo.m_wstrNickName	= wstrUnitName;

	kRoomSlotInfo.m_kRoomUserInfo.m_UnitSkillData = m_tSkillData;

	vecKRoomSlotInfo.push_back( kRoomSlotInfo );
		//}

	pRoom->Set_KRoomSlotInfoList( vecKRoomSlotInfo );
	//}
// 	else
// 	{
// 		CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData(0);
// 		// 캐릭 변경시 설정
// 	}
		
	CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData(0);
	g_pData->GetGameUDP()->SetMyUID( pSlotData->m_pUnit->GetUID() );
	g_pData->GetGameUDP()->AddPeer( pSlotData->m_pUnit->GetUID(), g_pData->GetGameUDP()->GetMyIPAddress(), g_pData->GetGameUDP()->GetMyExtPort() );

	// 몬스터 생성여부
	if( g_pInstanceData != NULL )
		g_pInstanceData->m_bIsNpcLoad = bIsNpcLoad;

	// game 생성
	if( g_pX2Game != NULL )
		SAFE_DELETE( g_pX2Game );

	g_pX2Game = NULL;
	g_pX2Game = new CX2DungeonGame(); //CWorldToolGame();
	g_pX2Game->GameLoading( (CX2Room*)g_pData->GetDungeonRoom() );
	if( g_pX2Game->GetWorld() != NULL )
	{
		g_pX2Game->GetWorld()->StopBGM();
	}

	CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	pDungeonGame->StageLoading(0);	
	pDungeonGame->StageStart();
	pDungeonGame->SubStageOpen(0);	
	//pDungeonGame->SubStageStart();
		

	
	if( m_bCanPlay == false )
	{
		// tool 에서는 combo manager가 필요없다~
		g_pX2Game->GetMyUnit()->DeleteComboManager();
	}

#if 0
	//if( m_bCanPlay == true )
	{
		CX2GUUser *pUser = g_pX2Game->GetMyUnit();
				
		KSkillData skilldata;
		skilldata.m_iSkillID = (short)CX2SkillTree::SI_A_EIS_MIRAGE_STING;
		skilldata.m_cSkillLevel = 5;

		vector<KUserSkillData> vecSkillAcquired;
		KUserSkillData userSkillData;
		userSkillData.m_iSkillID = (short)CX2SkillTree::SI_A_EIS_MIRAGE_STING;
		userSkillData.m_cSkillLevel = 5;
		vecSkillAcquired.push_back(userSkillData);

		m_tSkillData.Init();
		m_tSkillData.m_aEquippedSkill[0] = skilldata;

		pUser->GetUnit()->GetUnitData()->m_UserSkillTree.SetAcquiredSkill( vecSkillAcquired );
		pUser->GetUnit()->GetUnitData()->m_UserSkillTree.SetEquippedSkill( m_tSkillData.m_aEquippedSkill, m_tSkillData.m_aEquippedSkillSlotB );
	}
#endif


	m_wstrWorldFullName = g_pX2Game->GetWorld()->GetWorldName();
	m_wstrSoundName = g_pX2Game->GetWorld()->GetBGMName();

	//UpdateWorldToolObjectList();

	//ResetUserUnitStateListBox();

	//CreateCharacterSkinMesh( L"ELSWORD" );

	g_pKTDXApp->SkipFrame();

	// 라인데이타 툴용으로 변환
	ParsingLine();

	m_fPerspective = g_pKTDXApp->GetDGManager()->GetPerspectiveValue();
	
	
	if( pMain != NULL )
	{
		pMain->m_wndCameraInfo.m_LineCameraDlg.ResetCameraList();
		pMain->m_wndStartPosition.m_LineStartPositionDlg.ResetStartPosition();
		pMain->m_wndDungeonInfo.m_DungeonDlg.m_editLineName.SetWindowText( g_pX2Game->GetWorld()->GetLineMap()->GetLineFullName().c_str() );
		pMain->m_wndDungeonInfo.m_DungeonDlg.m_editWorldName.SetWindowText( g_pX2Game->GetWorld()->GetWorldName().c_str() );
					
		pMain->m_wndTriggerProperties.InitPropList();
		
		pMain->m_DlgDungeonEditor.ResetDungeon( eDungeonId );

		CX2DungeonGame *pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
		CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
		pMain->m_wndDungeonInfo.m_DungeonDlg.m_editDungeonName.SetWindowText( pDungeon->GetDungeonData()->m_DataFileName.c_str() );
	}
		
	static bool bLoadCfg = false;
	if( bLoadCfg == false || bReset == false )
	{
		DXUTDeviceSettings deviceSettings	= DXUTGetDeviceSettings();

		if( LoadConfig() == false )
		{
			m_iWidth = deviceSettings.pp.BackBufferWidth;
			m_iHeight = deviceSettings.pp.BackBufferHeight;
			m_bHeightMap = false;
		}

		GetGameOption().SetResolution(m_iWidth, m_iHeight);
		SetWindowSize(m_iWidth, m_iHeight);

		bLoadCfg = true;
	}
	
	
	WCHAR wWidth[10];
	WCHAR wHeight[10];
	wsprintfW( wWidth, L"%d",  m_iWidth);
	wsprintfW( wHeight, L"%d", m_iHeight);

	GetGameOption().SetFullScreen(false);
	
	pMain->m_wndDungeonInfo.m_DungeonDlg.GetDlgItem(IDC_EDIT_TOOL_WIDTH)->SetWindowTextW(wWidth);
	pMain->m_wndDungeonInfo.m_DungeonDlg.GetDlgItem(IDC_EDIT_TOOL_HEIGHT)->SetWindowTextW(wHeight);

	CString csSpeed;
	pMain->m_wndDungeonInfo.m_DungeonDlg.GetDlgItem(IDC_EDIT1)->GetWindowText(csSpeed);
	if( csSpeed.IsEmpty() == true )	
	{
		pMain->m_wndDungeonInfo.m_DungeonDlg.GetDlgItem(IDC_EDIT1)->SetWindowTextW(L"5.0");
	}	

	ClearUnDoWorld();
	m_bChangeStartPos = false;
}

void CX2ToolMain::ResetGame(SEnum::DUNGEON_ID eDungeonId)
{
	g_pX2Game->Release();
	SAFE_DELETE(g_pX2Game);

	CreateGame( eDungeonId, true );

	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->m_wndDungeonInfo.m_DungeonDlg.GetDlgItem(IDC_CHECK_USER_CAMERA)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
	SetUserCamera(false);
}




// Frame Interface
DWORD_PTR CX2ToolMain::GetSelectedDungeonItem()
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	return pMain->m_wndClassView.GetSelectedItem();	
}

void CX2ToolMain::NewLine(wstring wstrLineName)
{
	if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL && g_pX2Game->GetWorld()->GetLineMap() != NULL )
	{
		for(int i=vecpXSkinAnim.size()-1; i>=0; --i)
			DeleteObjectStartPosition(i);
		vecpXSkinAnim.clear();

		ClearUnDoList();
		m_bChangeStartPos = false; 

		m_wstrLineMapFullName = wstrLineName;

		CX2World *pWorld = g_pX2Game->GetWorld();
		CKTDGLineMap *pLineMap = pWorld->GetLineMap();
		
		
		pWorld->GetLineMapByTool(wstrLineName);

		g_pX2Game->GetMyUnit()->ClearRecieveSyncData();
		g_pX2Game->GetMyUnit()->InitPosition(true, 0);

		ResetTool( false, false, m_bViewLine, m_bViewWorld, false );

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetWorld() )
		{
			float fLandHeight = g_pX2Game->GetWorld()->GetLineMap()->GetLandHeight();
			g_pX2Game->GetX2Camera()->SetLandHeight( fLandHeight );
		}

		ParsingLine();

		{
			CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
			if( pMain != NULL )
			{
				pMain->m_wndCameraInfo.m_LineCameraDlg.ResetCameraList();
				pMain->m_wndStartPosition.m_LineStartPositionDlg.ResetStartPosition();
				pMain->m_wndDungeonInfo.m_DungeonDlg.m_editLineName.SetWindowText( pLineMap->GetLineFullName().c_str() );
				pMain->m_wndTriggerProperties.InitPropList();
			}
		}
	}	
}

void CX2ToolMain::LoadLineMap( const WCHAR* pwszFileName )
{
	ClearUnDoList();
	m_bChangeStartPos = false;

	//m_wstrLineMapFullName = pwszFileName;

	CompulsorybyConvertFileAnsiToUTF8W( pwszFileName );

	WCHAR wszFilePathOnly[512] = {NULL};
	WCHAR wszFileNameOnly[256] = {NULL};
	DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, pwszFileName );
	//m_wstrLineMapFilePath = wszFilePathOnly;
	//m_wstrLineMapFileName = wszFileNameOnly;	
	m_wstrLineMapFullName = wszFileNameOnly;
	
	g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory(wszFilePathOnly);

	g_pX2Game->GetWorld()->GetLineMapByTool( wszFileNameOnly );
	
	g_pX2Game->GetMyUnit()->ClearRecieveSyncData();
	g_pX2Game->GetMyUnit()->InitPosition(true, 0);

	if( NULL != g_pX2Game && NULL != g_pX2Game->GetWorld() )
	{
		float fLandHeight = g_pX2Game->GetWorld()->GetLineMap()->GetLandHeight();
		g_pX2Game->GetX2Camera()->SetLandHeight( fLandHeight );
	}

	ParsingLine();

	for(int i=vecpXSkinAnim.size()-1; i>=0; --i)
		DeleteObjectStartPosition(i);
	vecpXSkinAnim.clear();

	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain != NULL )
	{			
		pMain->m_wndCameraInfo.m_LineCameraDlg.ResetCameraList();
		pMain->m_wndStartPosition.m_LineStartPositionDlg.ResetStartPosition();
		pMain->m_wndDungeonInfo.m_DungeonDlg.m_editLineName.SetWindowText( g_pX2Game->GetWorld()->GetLineMap()->GetLineFullName().c_str() );
		pMain->m_wndTriggerProperties.InitPropList();
	}
}

tLineVertex* CX2ToolMain::GetSameVertexLine( D3DXVECTOR3 &vPos )
{
	for(UINT i=0; i<m_vecLineVertex.size(); ++i)
	{
		tLineVertex &lineVertex = m_vecLineVertex[i];
		if( IsSamef( vPos.x, lineVertex.vVertex.x ) == true &&
			IsSamef( vPos.y, lineVertex.vVertex.y ) == true &&
			IsSamef( vPos.z, lineVertex.vVertex.z ) == true )
		{
			return &m_vecLineVertex[i];
		}
	}

	return NULL;
}

void CX2ToolMain::RenderBoundingSphere( D3DXVECTOR3& vCenter, float fRadius, bool bOver, bool bSelected, BOUNDINGBOX_TYPE eBT )
{

	//CKTDGStateManager::PushRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	if( NULL == m_pXMeshBoundingSphere )
		return; 
		
	float fScaleRel = fRadius;

	D3DXCOLOR vColor = D3DXCOLOR(1,1,1,1);

	if( m_bEditWorld == true && m_bPickMoveWorldObjectPivot == true )
	{
		vColor = D3DXCOLOR(0,1,0,1);		
	}
	else if( bOver == true && bSelected == false )
	{
		vColor = D3DXCOLOR(1,0,0,1);
	}
	else if( bSelected == true )
	{
		vColor = D3DXCOLOR(1,0,0,1);
	}

	if( eBT != BT_NONE && (bOver == true || bSelected == true) )
		fScaleRel += 1.f;	

	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );		
	CKTDGXRenderer::RenderParam renderParam;
	renderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;

	renderParam.color = vColor;
	matTemp.Scale( fScaleRel, fScaleRel, fScaleRel );
	matTemp.Move( vCenter );
	//pRenderParam->worldMatrix = matTemp.GetMatrix();
	D3DXMATRIX worldMatrix = matTemp.GetMatrix( CKTDGMatrix::BT_ALL );

  	if( eBT == BT_NONE )
  	{ 
		renderParam.bAlphaTestEnable = true;
		renderParam.bZWriteEnable = false;
		renderParam.bZEnable = false;
  		g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pXMeshBoundingSphere );
  	}
  	else if( eBT == BT_START )
  	{
		renderParam.bAlphaTestEnable = true;
		renderParam.bZWriteEnable = false;
		renderParam.bZEnable = false;
 		g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pStartLineMesh );  		
  	}
 	else if( eBT == BT_END || eBT == BT_MIDDLE )
 	{
		renderParam.bAlphaTestEnable = true;
		renderParam.bZWriteEnable = false;
		renderParam.bZEnable = false;
		g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pEndLineMesh );
 	}
	//CKTDGStateManager::PopRenderState( D3DRS_FILLMODE );
}

void CX2ToolMain::RenderRectangle( D3DXVECTOR2 vLeftTop, D3DXVECTOR2 vRightBottom, D3DXCOLOR vColor )
{
	D3DXVECTOR2 vLT, vRB;
	
	if( vLeftTop.x < vRightBottom.x )
	{
		vLT.x = vLeftTop.x;
		vRB.x = vRightBottom.x;
	}
	else
	{
		vLT.x = vRightBottom.x;
		vRB.x = vLeftTop.x;
	}

	if( vLeftTop.y < vRightBottom.y )
	{
		vLT.y = vLeftTop.y;
		vRB.y = vRightBottom.y;
	}
	else
	{
		vLT.y = vRightBottom.y;
		vRB.y = vLeftTop.y;
	}


	D3DXVECTOR2 vList[] = {
		D3DXVECTOR2( vLT.x, vLT.y ),
		D3DXVECTOR2( vRB.x, vLT.y ),
		D3DXVECTOR2( vRB.x, vRB.y ),
		D3DXVECTOR2( vLT.x, vRB.y ),
		D3DXVECTOR2( vLT.x, vLT.y)
	};

	g_pLine->Begin();
	g_pLine->Draw( vList, 5, vColor );
	g_pLine->End();
}

void CX2ToolMain::RenderMoveAxis( D3DXVECTOR3 vPos, bool bUseWorld )
{
	float fScaleRelX = 1.f;
	float fScaleRelY = 1.f;
	float fScaleRelZ = 1.f;
	
	float fTemp = 100.f;
	float fStartTemp = -20.f;

	D3DXCOLOR vColor = D3DXCOLOR(1,1,1,1);		
	CKTDGXRenderer::RenderParam renderParam;
	renderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;
	renderParam.color = vColor;
	renderParam.bAlphaTestEnable = true;
	renderParam.bZWriteEnable = false;
	renderParam.bZEnable = false;

	CKTDGMatrix matTempX( g_pKTDXApp->GetDevice() );
	CKTDGMatrix matTempY( g_pKTDXApp->GetDevice() );
	CKTDGMatrix matTempZ( g_pKTDXApp->GetDevice() );
	
	D3DXVECTOR3 vStartPosX = vPos;
	D3DXVECTOR3 vStartPosY = vPos;
	D3DXVECTOR3 vStartPosZ = vPos;

	if( m_bEditWorld == true )
	{
		vStartPosX.x += 100.f;
		vStartPosY.y += 100.f;
		vStartPosZ.z += 100.f;
	}

	D3DXVECTOR3 vPosX = vStartPosX;
	D3DXVECTOR3 vPosY = vStartPosY;
	D3DXVECTOR3 vPosZ = vStartPosZ;
	vPosX.x += (fTemp * m_fScaleAxis);
	vPosY.y += (fTemp * m_fScaleAxis);
	vPosZ.z += (fTemp * m_fScaleAxis);
	matTempX.Scale( fScaleRelX, 1.f, 1.f );
	matTempX.Move( vPosX );
	matTempY.Scale( 1.f, fScaleRelY, 1.f );
	matTempY.Move( vPosY );
	matTempZ.Scale( 1.f, 1.f, fScaleRelZ );
	matTempZ.Move( vPosZ );
	

	D3DXMATRIX worldMatrixX = matTempX.GetMatrix( CKTDGMatrix::BT_NONE );
	D3DXMATRIX worldMatrixY = matTempY.GetMatrix( CKTDGMatrix::BT_NONE );
	D3DXMATRIX worldMatrixZ = matTempZ.GetMatrix( CKTDGMatrix::BT_NONE );	

	if( m_bEditWorld == true && m_bRotationWorldObject == true )
	{
		// nop
	}
	else
	{
		if( m_bEditWorld == true && m_bScaleWorldObject == true )
		{
			// nop
		}
		else
		{
			g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrixX, *m_pControlXMesh );
			g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrixY, *m_pControlYMesh );
			g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrixZ, *m_pControlZMesh );

			m_pFont_A->OutProjectionText( vPosX, L"x", 0xffffffff, CKTDGFontManager::FS_SHELL, 0x00000000, NULL, DT_CENTER );
			m_pFont_A->OutProjectionText( vPosY, L"y", 0xffffffff, CKTDGFontManager::FS_SHELL, 0x00000000, NULL, DT_CENTER );
			m_pFont_A->OutProjectionText( vPosZ, L"z", 0xffffffff, CKTDGFontManager::FS_SHELL, 0x00000000, NULL, DT_CENTER );
		}
		
	}
	
	D3DXVECTOR3 vRotVos = D3DXVECTOR3(0.f, 0.f, 0.f);

	D3DXVECTOR3 vListX[] = { vPos, D3DXVECTOR3(vPosX.x + fStartTemp, vPosX.y, vPosX.z) };
	D3DXVECTOR3 vListY[] = { vPos, D3DXVECTOR3(vPosY.x, vPosY.y + fStartTemp, vPosY.z) };
	D3DXVECTOR3 vListZ[] = { vPos, D3DXVECTOR3(vPosZ.x, vPosZ.y, vPosZ.z + fStartTemp) };

	D3DXVECTOR3 vListXY[] = { D3DXVECTOR3(vPosX.x + fStartTemp, vPosX.y, vPosX.z), D3DXVECTOR3(vPosY.x, vPosY.y + fStartTemp, vPosY.z) };
	D3DXVECTOR3 vListYZ[] = { D3DXVECTOR3(vPosY.x, vPosY.y + fStartTemp, vPosY.z), D3DXVECTOR3(vPosZ.x, vPosZ.y, vPosZ.z + fStartTemp) };
	D3DXVECTOR3 vListXZ[] = { D3DXVECTOR3(vPosX.x + fStartTemp, vPosX.y, vPosX.z), D3DXVECTOR3(vPosZ.x, vPosZ.y, vPosZ.z + fStartTemp) };

	D3DXVECTOR3 vListRotateX[] = { D3DXVECTOR3(vRotVos.x, vRotVos.y + 100.f, vRotVos.z), D3DXVECTOR3(vRotVos.x, vRotVos.y, vRotVos.z + 100.f),
									D3DXVECTOR3(vRotVos.x, vRotVos.y - 100.f, vRotVos.z),  D3DXVECTOR3(vRotVos.x, vRotVos.y, vRotVos.z - 100.f),
									D3DXVECTOR3(vRotVos.x, vRotVos.y + 100.f, vRotVos.z) };
	D3DXVECTOR3 vListRotateY[] = { D3DXVECTOR3(vRotVos.x + 100.f, vRotVos.y, vRotVos.z), D3DXVECTOR3(vRotVos.x, vRotVos.y, vRotVos.z + 100.f),
									D3DXVECTOR3(vRotVos.x - 100.f, vRotVos.y, vRotVos.z), D3DXVECTOR3(vRotVos.x, vRotVos.y, vRotVos.z - 100.f),
									D3DXVECTOR3(vRotVos.x + 100.f, vRotVos.y, vRotVos.z) };
	D3DXVECTOR3 vListRotateZ[] = { D3DXVECTOR3(vRotVos.x + 100.f, vRotVos.y, vRotVos.z), D3DXVECTOR3(vRotVos.x, vRotVos.y + 100.f, vRotVos.z),
									D3DXVECTOR3(vRotVos.x - 100.f, vRotVos.y, vRotVos.z), D3DXVECTOR3(vRotVos.x, vRotVos.y - 100.f, vRotVos.z),
									D3DXVECTOR3(vRotVos.x + 100.f, vRotVos.y, vRotVos.z) };

	for(int i=0; i<5; ++i)
	{
		D3DXVec3TransformCoord(&vListRotateX[i], &vListRotateX[i], &m_matPivot);
		D3DXVec3TransformCoord(&vListRotateY[i], &vListRotateY[i], &m_matPivot);
		D3DXVec3TransformCoord(&vListRotateZ[i], &vListRotateZ[i], &m_matPivot);
	}
	
	

	D3DXMATRIX worldMatrix = g_pKTDXApp->GetViewProjectionTransform();

	D3DXCOLOR xColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	D3DXCOLOR yColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
	D3DXCOLOR zColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	D3DXCOLOR xyColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	D3DXCOLOR xzColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	D3DXCOLOR yzColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	if( m_bMoveAxisX == true )
		xColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	if( m_bMoveAxisY == true )
		yColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	if( m_bMoveAxisZ == true )
		zColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	if( m_bMoveAxisX == true && m_bMoveAxisY == true )
		xyColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	if( m_bMoveAxisX == true && m_bMoveAxisZ == true )
		xzColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	if( m_bMoveAxisY == true && m_bMoveAxisZ == true )
		yzColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	KD3DPUSH( g_pKTDXApp->GetDGManager()->GetRSICKTDXDeviceLayer(0) )
	{
		g_pLineAxis->Begin();
		
		if( m_bEditWorld == true && m_bRotationWorldObject == true )
		{
			g_pLineAxis->DrawTransform( vListRotateX, 5, &worldMatrix, xColor );
			g_pLineAxis->DrawTransform( vListRotateY, 5, &worldMatrix, yColor );
			g_pLineAxis->DrawTransform( vListRotateZ, 5, &worldMatrix, zColor );
		}
		else
		{
			if( m_bEditWorld == true && m_bScaleWorldObject == true )
			{
				for(int i=0; i<2; ++i)
				{
					vListX[i] = vListX[i] - vPos;
					vListY[i] = vListY[i] - vPos;
					vListZ[i] = vListZ[i] - vPos;
					vListXY[i] = vListXY[i] - vPos;
					vListYZ[i] = vListYZ[i] - vPos;
					vListXZ[i] = vListXZ[i] - vPos;

					D3DXVec3TransformCoord(&vListX[i], &vListX[i], &m_matPivot);
					D3DXVec3TransformCoord(&vListY[i], &vListY[i], &m_matPivot);
					D3DXVec3TransformCoord(&vListZ[i], &vListZ[i], &m_matPivot);
					D3DXVec3TransformCoord(&vListXY[i], &vListXY[i], &m_matPivot);
					D3DXVec3TransformCoord(&vListYZ[i], &vListYZ[i], &m_matPivot);
					D3DXVec3TransformCoord(&vListXZ[i], &vListXZ[i], &m_matPivot);
				}
				
			}

			g_pLineAxis->DrawTransform( vListXY, 2, &worldMatrix, xyColor );
			g_pLineAxis->DrawTransform( vListYZ, 2, &worldMatrix, yzColor );
			g_pLineAxis->DrawTransform( vListXZ, 2, &worldMatrix, xzColor );

			g_pLineAxis->DrawTransform( vListX, 2, &worldMatrix, xColor );
			g_pLineAxis->DrawTransform( vListY, 2, &worldMatrix, yColor );
			g_pLineAxis->DrawTransform( vListZ, 2, &worldMatrix, zColor );
		}		
		
		g_pLineAxis->End();
	}	
	KD3DEND()

	if( m_bEditLine == true )
	{
		float fDist = GetDistance(vPos, g_pX2Game->GetX2Camera()->GetCamera().GetEye());
		float fRadius = (fDist / 20000.f);
		if( m_bUserCamera == true )
		{
			fRadius *= 0.5f;
		}
		RenderBoundingSphere( vPos, fRadius, true, true, BT_NONE );
	}
	
}

void CX2ToolMain::RenderLine( D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXCOLOR vColor )
{		
	D3DXVECTOR3 vList[] = { vStart, vEnd };
	D3DXMATRIX worldMatrix = g_pKTDXApp->GetViewProjectionTransform();

	g_pLineAxis->Begin();
	g_pLineAxis->DrawTransform( vList, 2, &worldMatrix, vColor );
	g_pLineAxis->End();
}

void CX2ToolMain::RenderGrid()
{
#if 0
	float fX = 50.f * g_pKTDXApp->GetResolutionScaleX();
	float fY = 710.f * g_pKTDXApp->GetResolutionScaleY();

	D3DXVECTOR3 vPos = g_pKTDXApp->GetUnProj3DPos( D3DXVECTOR3( fX, fY, 1.f ) );
	
	D3DXCOLOR vColor = D3DXCOLOR(1,1,1,1);		
	CKTDGXRenderer::RenderParam renderParam;
	renderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;
	renderParam.color = vColor;
	renderParam.bAlphaTestEnable = true;
	renderParam.bZWriteEnable = false;
	renderParam.bZEnable = false;

	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );

	matTemp.Scale( 100.f, 100.f, 100.f );
	matTemp.Move( vPos );

	D3DXMATRIX worldMatrix = matTemp.GetMatrix( CKTDGMatrix::BT_NONE );
	g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pControlAxis );
#endif
}

void CX2ToolMain::RenderMoveWorld( D3DXVECTOR3& vCenter )
{
	if( m_bMoveWorldObject == false )
		return;

	float fDist = GetDistance(vCenter, g_pX2Game->GetX2Camera()->GetCamera().GetEye());
	float fRadius = (fDist / 20000.f);
	if( m_bUserCamera == true )
	{
		fRadius *= 0.5f;
	}
	RenderBoundingSphere( vCenter, fRadius, true, true, BT_NONE );

	//RenderBoundingSphere( m_vMoveWorldObject, (fDist / 20000.f), true, true, BT_NONE );	
}

void CX2ToolMain::ParsingLine()
{
	m_iVertPivot = -1;
	m_vecLineVertex.clear();

	if( g_pX2Game == NULL || g_pX2Game->GetWorld() == NULL || g_pX2Game->GetWorld()->GetLineMap() == NULL )
		return;

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	int iLineCount = pLineMap->GetNumLineData();

	for(int i=0; i<iLineCount; ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);
		if( pLineMap != NULL )
		{
			// start vertex 
			tLineVertex *pLineVert = NULL;
			pLineVert = GetSameVertexLine( pLineData->startPos );
			if( pLineVert == NULL )
			{
				tLineVertex lineVert;
				lineVert.vVertex = pLineData->startPos;
				tLineVertexInfo lineVertInfo;
				lineVertInfo.m_bStartPos = true;
				lineVertInfo.m_iLineIndex = pLineData->lineIndex;				
				lineVert.m_vecLineInfo.push_back(lineVertInfo);
				m_vecLineVertex.push_back(lineVert);
			}
			else
			{
				tLineVertexInfo lineVertInfo;
				lineVertInfo.m_bStartPos = true;
				lineVertInfo.m_iLineIndex = pLineData->lineIndex;				
				pLineVert->m_vecLineInfo.push_back(lineVertInfo);
			}
			
			// end vertex
			pLineVert = GetSameVertexLine( pLineData->endPos );
			if( pLineVert == NULL )				
			{
				tLineVertex lineVert;
				lineVert.vVertex = pLineData->endPos;
				tLineVertexInfo lineVertInfo;
				lineVertInfo.m_bStartPos = false;
				lineVertInfo.m_iLineIndex = pLineData->lineIndex;				
				lineVert.m_vecLineInfo.push_back(lineVertInfo);
				m_vecLineVertex.push_back(lineVert);
			}
			else
			{
				tLineVertexInfo lineVertInfo;
				lineVertInfo.m_bStartPos = false;
				lineVertInfo.m_iLineIndex = pLineData->lineIndex;
				pLineVert->m_vecLineInfo.push_back(lineVertInfo);
			}
		}
	}

	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain != NULL )
		pMain->m_DlgLineBaseEditor.UpdateLineInfo();

	if( IsValidLine() == false )
	{
		MessageBox(NULL, L"라인정보가 이상합니다. 현재 라인들을 정상 처리할 수 없습니다.", L"알림!", MB_OK);
	}
}

bool CX2ToolMain::PickLineVertex(bool bDrag)
{
	if( m_bEditLine == true && m_bEditLineVertex == true )
	{			
		//D3DXMatrixLookAtLH( &matView, g_pX2Game->GetFPSCamera()->GetEyePt(), g_pX2Game->GetFPSCamera()->GetEyePt(), &m_Camera.m_UpVec );

		D3DXVECTOR2 vLT, vRB;
		if( m_vDragLineView[0].x < m_vDragLineView[1].x )
		{
			vLT.x = m_vDragLineView[0].x;
			vRB.x = m_vDragLineView[1].x;
		}
		else
		{
			vLT.x = m_vDragLineView[1].x;
			vRB.x = m_vDragLineView[0].x;
		}

		if( m_vDragLineView[0].y < m_vDragLineView[1].y )
		{
			vLT.y = m_vDragLineView[0].y;
			vRB.y = m_vDragLineView[1].y;
		}
		else
		{
			vLT.y = m_vDragLineView[1].y;
			vRB.y = m_vDragLineView[0].y;
		}

		for(UINT i=0; i<m_vecLineVertex.size(); ++i)
		{
			bool bOver = CCollision::LineToSphere(m_vRayLine[0], m_vRayLine[1], 3.f, m_vecLineVertex[i].vVertex, 10.f, NULL);
			if( bOver == true )
				m_vecLineVertex[i].m_bOver = true;
			else
				m_vecLineVertex[i].m_bOver = false;

			if( bOver == false && bDrag == true )
			{				
				D3DXVECTOR2 vPos2D = g_pKTDXApp->GetProj3DPos(m_vecLineVertex[i].vVertex);	

				if( vPos2D.x >= vLT.x && vPos2D.x <= vRB.x && vPos2D.y >= vLT.y && vPos2D.y <= vRB.y )
				{
					m_vecLineVertex[i].m_bOver = true;
				}
				else
				{
					m_vecLineVertex[i].m_bOver = false;
				}
			}
						
			if( m_bLShift == true && m_vecLineVertex[i].m_bSelected == true && m_vecLineVertex[i].m_bOver == true )
			{
				m_iVertPivot = i;
			}
		}	
	}
	else if( m_bEditLine == true && m_bEditLineVertex == false )
	{
		for(UINT i=0; i<m_vecLineVertex.size(); ++i)
		{
			bool bOver = CCollision::LineToSphere(m_vRayLine[0], m_vRayLine[1], 3.f, m_vecLineVertex[i].vVertex, 10.f, NULL);
			
			if( m_bLShift == true && m_vecLineVertex[i].m_bSelected == true && bOver == true )
			{
				m_iVertPivot = i;
				break;
			}
		}	
	}

	return true;
}

bool CX2ToolMain::PickLine(bool bDrag)
{
	if( g_pX2Game != NULL && m_bEditLine == true && m_bEditLineVertex == false )
	{			
		//D3DXMatrixLookAtLH( &matView, g_pX2Game->GetFPSCamera()->GetEyePt(), g_pX2Game->GetFPSCamera()->GetEyePt(), &m_Camera.m_UpVec );

		D3DXVECTOR2 vLT, vRB;
		if( m_vDragLineView[0].x < m_vDragLineView[1].x )
		{
			vLT.x = m_vDragLineView[0].x;
			vRB.x = m_vDragLineView[1].x;
		}
		else
		{
			vLT.x = m_vDragLineView[1].x;
			vRB.x = m_vDragLineView[0].x;
		}

		if( m_vDragLineView[0].y < m_vDragLineView[1].y )
		{
			vLT.y = m_vDragLineView[0].y;
			vRB.y = m_vDragLineView[1].y;
		}
		else
		{
			vLT.y = m_vDragLineView[1].y;
			vRB.y = m_vDragLineView[0].y;
		}

		vector<int> vecVertIndex;
		CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
		for(int i=0; i<pLineMap->GetNumLineData(); ++i)
		{
			CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);
			if( pLineData == NULL )
				continue;

			D3DXVECTOR3 vPoint1, vPoint2;
			bool bOver = CCollision::LineToLine(pLineData->startPos, pLineData->endPos, m_vRayLine[0], m_vRayLine[1], 20.f, vPoint1, vPoint2);
			
			if( bOver == false && bDrag == true )
			{
				D3DXVECTOR3 vPoint1, vPoint2;
				D3DXVECTOR2 vPos2DStart = g_pKTDXApp->GetProj3DPos(pLineData->startPos);
				D3DXVECTOR2 vPos2DEnd = g_pKTDXApp->GetProj3DPos(pLineData->endPos);
				D3DXVECTOR2 vRect[4] = { vLT, D3DXVECTOR2(vRB.x, vLT.y), vRB, D3DXVECTOR2(vLT.x, vRB.y) };

				if( (vPos2DStart.x >= vLT.x && vPos2DStart.x <= vRB.x && vPos2DStart.y >= vLT.y && vPos2DStart.y <= vRB.y) ||
					(vPos2DEnd.x >= vLT.x && vPos2DEnd.x <= vRB.x && vPos2DEnd.y >= vLT.y && vPos2DEnd.y <= vRB.y) )
				{
					bOver = true;
				}
				else
				{
					bool bOverRect = CCollision::LineToLine( vRect[0], vRect[1], vPos2DStart, vPos2DEnd );
					if( bOverRect == false )
						bOverRect = CCollision::LineToLine( vRect[1], vRect[2], vPos2DStart, vPos2DEnd );
					if( bOverRect == false )
						bOverRect = CCollision::LineToLine( vRect[2], vRect[3], vPos2DStart, vPos2DEnd );
					if( bOverRect == false )
						bOverRect = CCollision::LineToLine( vRect[0], vRect[3], vPos2DStart, vPos2DEnd );
					
					if( bOverRect == true )
					{
						bOver = true;
					}
				}				
			}

			pLineData->m_bOver = bOver;
		}

		PickLineVertex();
	}

	return true;
}

bool CX2ToolMain::PickMoveAxis()
{
	if( m_bMoveAxis == false )
	{
		if( (m_bEditLine == true || m_bEditWorld == true) && m_bShowMoveAxis == true )
		{
			float fTemp = 100.f;
			float fStartTemp = -20.f;
			
			D3DXVECTOR3 vPosStartX = m_vPosMoveAxis;
			D3DXVECTOR3 vPosStartY = m_vPosMoveAxis;
			D3DXVECTOR3 vPosStartZ = m_vPosMoveAxis;
			if( m_bEditWorld == true )
			{
				vPosStartX.x += 100.f;
				vPosStartY.y += 100.f;
				vPosStartZ.z += 100.f;
			}

			D3DXVECTOR3 vPosX = vPosStartX;
			D3DXVECTOR3 vPosY = vPosStartY;
			D3DXVECTOR3 vPosZ = vPosStartZ;

			vPosX.x += (fTemp * m_fScaleAxis + fStartTemp);
			vPosY.y += (fTemp * m_fScaleAxis + fStartTemp);
			vPosZ.z += (fTemp * m_fScaleAxis + fStartTemp);
			
			if( m_bEditWorld == true && m_bRotationWorldObject == true )
			{
				
				float fRadius = 10.f;
				D3DXVECTOR3 vCollisionPoint;

				float fDist = GetDistance(m_vPosMoveAxis, g_pX2Game->GetX2Camera()->GetCamera().GetEye());
				float fTemp = fDist/20000.f;
				if( m_bUserCamera == true )
					fTemp *= 0.5f;
				fRadius = 100.f * fTemp;
				
				bool bHit = CCollision::LineToSphere(m_vRayLine[0], m_vRayLine[1], 1.f,
					m_vPosMoveAxis, fRadius, &vCollisionPoint );
				if( bHit == true )
				{
					m_bMoveAxisX = true;
					m_bMoveAxisY = true;
					m_bMoveAxisZ = true;

					return true;
				}


				D3DXVECTOR3 vRotVos = D3DXVECTOR3(0.f, 0.f, 0.f);
				D3DXVECTOR3 vListRotateX[] = { D3DXVECTOR3(vRotVos.x, vRotVos.y + 100.f, vRotVos.z), D3DXVECTOR3(vRotVos.x, vRotVos.y, vRotVos.z + 100.f),
					D3DXVECTOR3(vRotVos.x, vRotVos.y - 100.f, vRotVos.z),  D3DXVECTOR3(vRotVos.x, vRotVos.y, vRotVos.z - 100.f),
					D3DXVECTOR3(vRotVos.x, vRotVos.y + 100.f, vRotVos.z) };
				D3DXVECTOR3 vListRotateY[] = { D3DXVECTOR3(vRotVos.x + 100.f, vRotVos.y, vRotVos.z), D3DXVECTOR3(vRotVos.x, vRotVos.y, vRotVos.z + 100.f),
					D3DXVECTOR3(vRotVos.x - 100.f, vRotVos.y, vRotVos.z), D3DXVECTOR3(vRotVos.x, vRotVos.y, vRotVos.z - 100.f),
					D3DXVECTOR3(vRotVos.x + 100.f, vRotVos.y, vRotVos.z) };
				D3DXVECTOR3 vListRotateZ[] = { D3DXVECTOR3(vRotVos.x + 100.f, vRotVos.y, vRotVos.z), D3DXVECTOR3(vRotVos.x, vRotVos.y + 100.f, vRotVos.z),
					D3DXVECTOR3(vRotVos.x - 100.f, vRotVos.y, vRotVos.z), D3DXVECTOR3(vRotVos.x, vRotVos.y - 100.f, vRotVos.z),
					D3DXVECTOR3(vRotVos.x + 100.f, vRotVos.y, vRotVos.z) };

				for(int i=0; i<5; ++i)
				{
					D3DXVec3TransformCoord(&vListRotateX[i], &vListRotateX[i], &m_matPivot);
					D3DXVec3TransformCoord(&vListRotateY[i], &vListRotateY[i], &m_matPivot);
					D3DXVec3TransformCoord(&vListRotateZ[i], &vListRotateZ[i], &m_matPivot);
				}

				D3DXVECTOR3 vPoint1, vPoint2;
				m_bMoveAxisX = CCollision::LineToLine(vListRotateX[0], vListRotateX[1], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
				if( m_bMoveAxisX == false )
				{
					m_bMoveAxisX = CCollision::LineToLine(vListRotateX[1], vListRotateX[2], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
					if( m_bMoveAxisX == false )
					{
						m_bMoveAxisX = CCollision::LineToLine(vListRotateX[2], vListRotateX[3], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
						if( m_bMoveAxisX == false )
						{
							m_bMoveAxisX = CCollision::LineToLine(vListRotateX[3], vListRotateX[4], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
						}
					}
				}

				m_bMoveAxisY = CCollision::LineToLine(vListRotateY[0], vListRotateY[1], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
				if( m_bMoveAxisY == false )
				{
					m_bMoveAxisY = CCollision::LineToLine(vListRotateY[1], vListRotateY[2], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
					if( m_bMoveAxisY == false )
					{
						m_bMoveAxisY = CCollision::LineToLine(vListRotateY[2], vListRotateY[3], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
						if( m_bMoveAxisY == false )
						{
							m_bMoveAxisY = CCollision::LineToLine(vListRotateY[3], vListRotateY[4], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
						}
					}
				}

				m_bMoveAxisZ = CCollision::LineToLine(vListRotateZ[0], vListRotateZ[1], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
				if( m_bMoveAxisZ == false )
				{
					m_bMoveAxisZ = CCollision::LineToLine(vListRotateZ[1], vListRotateZ[2], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
					if( m_bMoveAxisZ == false )
					{
						m_bMoveAxisZ = CCollision::LineToLine(vListRotateZ[2], vListRotateZ[3], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
						if( m_bMoveAxisZ == false )
						{
							m_bMoveAxisZ = CCollision::LineToLine(vListRotateZ[3], vListRotateZ[4], m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
						}
					}
				}
			}
			else
			{
				D3DXVECTOR3 vStartPos = m_vPosMoveAxis;
				if( m_bEditWorld == true && m_bScaleWorldObject == true )
				{
					vStartPos = vStartPos - m_vPosMoveAxis;
					vPosX = vPosX - m_vPosMoveAxis;
					vPosY = vPosY - m_vPosMoveAxis;
					vPosZ = vPosZ - m_vPosMoveAxis;

					D3DXVec3TransformCoord(&vStartPos, &vStartPos, &m_matPivot);
					D3DXVec3TransformCoord(&vPosX, &vPosX, &m_matPivot);
					D3DXVec3TransformCoord(&vPosY, &vPosY, &m_matPivot);
					D3DXVec3TransformCoord(&vPosZ, &vPosZ, &m_matPivot);
				}

				D3DXVECTOR3 vPoint1, vPoint2;
				m_bMoveAxisX = CCollision::LineToLine(vStartPos, vPosX, m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
				m_bMoveAxisY = CCollision::LineToLine(vStartPos, vPosY, m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
				m_bMoveAxisZ = CCollision::LineToLine(vStartPos, vPosZ, m_vRayLine[0], m_vRayLine[1], 10.f, vPoint1, vPoint2);
				

				if(m_bMoveAxisX == false && m_bMoveAxisY == false && m_bMoveAxisZ == false )
				{
					bool bMoveAxisXY = CCollision::LineToLine(vPosX, vPosY, m_vRayLine[0], m_vRayLine[1], 6.f, vPoint1, vPoint2);
					bool bMoveAxisXZ = CCollision::LineToLine(vPosX, vPosZ, m_vRayLine[0], m_vRayLine[1], 6.f, vPoint1, vPoint2);
					bool bMoveAxisYZ = CCollision::LineToLine(vPosY, vPosZ, m_vRayLine[0], m_vRayLine[1], 6.f, vPoint1, vPoint2);

					if( bMoveAxisXY == true )
					{
						m_bMoveAxisX = true;
						m_bMoveAxisY = true;
						m_bMoveAxisZ = false;
					}
					else if( bMoveAxisXZ == true )
					{
						m_bMoveAxisX = true;
						m_bMoveAxisY = false;
						m_bMoveAxisZ = true;
					}
					else if( bMoveAxisYZ == true )
					{
						m_bMoveAxisX = false;
						m_bMoveAxisY = true;
						m_bMoveAxisZ = true;
					}
				}
			}			
		}
		else 
		{
			m_bMoveAxisX = false;
			m_bMoveAxisY = false;
			m_bMoveAxisZ = false;
		}
	}
		
	return m_bMoveAxisX || m_bMoveAxisY || m_bMoveAxisZ;
}

bool CX2ToolMain::PickMoveWorldObject()
{	
	if( m_bRotationWorldObject == true || m_bScaleWorldObject == true )
		return false;

	D3DXVECTOR3 vCenter =  m_vMoveWorldObjectPivot;
	float fRadius = 10.f;
	D3DXVECTOR3 vCollisionPoint;

	float fDist = GetDistance(vCenter, g_pX2Game->GetX2Camera()->GetCamera().GetEye());
	float fTemp = fDist/20000.f;
	if( m_bUserCamera == true )
		fTemp *= 0.5f;
	fRadius = 100.f * fTemp;
	
	//D3DXMATRIX matTransform = pAnim->GetMatrix().GetMatrix(); 
	//D3DXVec3TransformCoord( &vCenter, &vCenter, &matTransform );

	bool bHit = CCollision::LineToSphere(m_vRayLine[0], m_vRayLine[1], 1.f,
		vCenter, fRadius, &vCollisionPoint );
	if( bHit == true )
		return true;

	return false;
}

bool CX2ToolMain::PickStartPosition()
{	
	float fDist = 99999.f;
	bool bResult = false;
	
	m_iOverStartPosition = -1;
	for(UINT i=0; i<vecpXSkinAnim.size(); ++i)
	{
		CKTDGXSkinAnim *pAnim = vecpXSkinAnim[i];
		if( pAnim != NULL )
		{
			D3DXVECTOR3 vCenter =  pAnim->GetCenter();
			float fRadius = pAnim->GetBoundingRadius();
			D3DXVECTOR3 vCollisionPoint;

			D3DXMATRIX matTransform = pAnim->GetMatrix().GetMatrix(); 
			D3DXVec3TransformCoord( &vCenter, &vCenter, &matTransform );

			bool bHit = CCollision::LineToSphere(m_vRayLine[0], m_vRayLine[1], 1.f,
				vCenter, fRadius, &vCollisionPoint );
			if( bHit == true )
			{
				bResult = true;				
				
				vCollisionPoint -= vCenter;
				float fDistTemp = D3DXVec3Length( &vCollisionPoint );
				if( fDistTemp <= fDist )
				{
					fDist = fDistTemp;
					m_iOverStartPosition = i;
				}
			}
		}
	}
		
	return bResult;
}

void CX2ToolMain::DeleteVertex()	// 버텍스 대상
{
	if( m_bEditStartPosition == true )
		return;

	AddUndoList();

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	for(UINT i=0; i<m_vecLineVertex.size(); ++i)
	{		
		if( m_vecLineVertex[i].m_bSelected == true )
		{
			CKTDGLineMap::LineData *pBeforeLineData = NULL;
			CKTDGLineMap::LineData *pNextLineData = NULL;

			for(UINT j=0; j<m_vecLineVertex[i].m_vecLineInfo.size(); ++j)
			{
				tLineVertexInfo lineVertInfo = m_vecLineVertex[i].m_vecLineInfo[j];
				if( lineVertInfo.m_bStartPos == false )
					pBeforeLineData = pLineMap->AccessLineData( lineVertInfo.m_iLineIndex );
				else
					pNextLineData = pLineMap->AccessLineData( lineVertInfo.m_iLineIndex );
			}
			
			if( m_vecLineVertex[i].m_vecLineInfo.size() == 1 )
			{
				if( pNextLineData != NULL )
				{
					CKTDGLineMap::LineData *pMoreNextLineData = pLineMap->AccessLineData(pNextLineData->nextLine);
					if( pMoreNextLineData != NULL )
					{
						pMoreNextLineData->beforeLine = -1;
						DeleteLineVertByLineIndex( pNextLineData->lineIndex );
					}
				}
				else if( pBeforeLineData != NULL )
				{
					CKTDGLineMap::LineData *pMoreBeforeLineData = pLineMap->AccessLineData(pBeforeLineData->beforeLine);
					if( pMoreBeforeLineData != NULL )
					{
						pMoreBeforeLineData->nextLine = -1;
						DeleteLineVertByLineIndex( pBeforeLineData->lineIndex );
					}
				}
			}
			else
			{
				if( pNextLineData != NULL )
				{
					CKTDGLineMap::LineData *pMoreNextLineData = pLineMap->AccessLineData(pNextLineData->nextLine);
					pBeforeLineData->nextLine = pNextLineData->nextLine;
					pBeforeLineData->endPos = pNextLineData->endPos;
					pLineMap->MakeLine( pBeforeLineData );

					if( pMoreNextLineData != NULL )
					{
						pMoreNextLineData->beforeLine = pBeforeLineData->lineIndex;
					}
					DeleteLineVertByLineIndex( pNextLineData->lineIndex, pBeforeLineData->lineIndex );
				}
			}

			m_vecLineVertex.erase( m_vecLineVertex.begin() + i );
			--i;
		}
	}

	UpdateLineMap();
	pLineMap->UpdateLineData();

	ParsingLine();

	RebuildStartPosition();
}

void CX2ToolMain::DeleteLine()
{
	if( m_bEditStartPosition == true )
		return;

	AddUndoList();
	
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
		if( pLineData == NULL )
			continue;

		if( pLineData->m_bSelected == true )
		{
			int iBeforeIndex = pLineData->beforeLine;
			int iNextIndex = pLineData->nextLine;
			
			pLineMap->DeleteLineData(i);

			CKTDGLineMap::LineData *pBeforeLine = pLineMap->AccessLineData(iBeforeIndex);
			CKTDGLineMap::LineData *pNextLine = pLineMap->AccessLineData(iNextIndex);

			if( pBeforeLine != NULL )
				pBeforeLine->nextLine = -1;
			if( pNextLine != NULL )
				pNextLine->beforeLine = -1;
		}
	}

	UpdateLineMap();
	pLineMap->UpdateLineData();
	
	ParsingLine();

	RebuildStartPosition();
}

void CX2ToolMain::DoDeleteEditLine()
{
	if( m_bEditLineVertex == true )
		DeleteVertex();
	else
	{
		if( m_bEditStartPosition == true && m_iPickedStartPosition >= 0 )
		{
			CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
			if( pMain != NULL )
			{
				pMain->m_wndStartPosition.m_LineStartPositionDlg.DeleteNpcStartPos( m_iPickedStartPosition );
				pMain->m_wndStartPosition.m_LineStartPositionDlg.ResetStartPosition(false);
			}
		}
		else if( m_bEditStartPosition == false )
		{
			DeleteLine();
		}								
	}
}


int CX2ToolMain::AddVertex(D3DXVECTOR3 vAddPos, int lineIndex)	// 라인대상
{
	if( g_pX2Game == NULL || lineIndex <= -1 )
		return -1;	

	D3DXVECTOR3 vOutPos;
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	float fDist = pLineMap->GetNearestByTool(vAddPos, &vOutPos, lineIndex);
	if( fDist >= 99999.f )
		return -1;

	CKTDGLineMap::LineData *pBeforeLineData = pLineMap->AccessLineData(lineIndex);
	CKTDGLineMap::LineData *pAfterLineData = pLineMap->AccessLineData(pBeforeLineData->nextLine);

	CKTDGLineMap::LineData *pNewLine = pLineMap->AddNewLine( pBeforeLineData );
	pBeforeLineData->endPos = vOutPos;
	pNewLine->startPos = vOutPos;	
		
	pBeforeLineData->nextLine = pNewLine->lineIndex;
	pNewLine->beforeLine = pBeforeLineData->lineIndex;
	
	if( pAfterLineData != NULL )
		pAfterLineData->beforeLine = pNewLine->lineIndex;

	pLineMap->MakeLine(pBeforeLineData);
	pLineMap->MakeLine(pNewLine);
		
	pLineMap->UpdateLineData();
	ParsingLine();

	RebuildStartPosition();

	return pNewLine->lineIndex;
}

void CX2ToolMain::SetMakeFirst()
{
	AddUndoList();

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	for(UINT i=0; i<m_vecLineVertex.size(); ++i)
	{
		if( m_vecLineVertex[i].m_bSelected == false )
			continue;
		if( m_vecLineVertex[i].m_vecLineInfo.size() >= 2 )
			continue;
		if( m_vecLineVertex[i].m_vecLineInfo[0].m_bStartPos == true )
			continue;

		int iStartLine = m_vecLineVertex[i].m_vecLineInfo[0].m_iLineIndex;
		
		CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(iStartLine);
		while( pLineData != NULL )
		{
			D3DXVECTOR3 vTemp = pLineData->startPos;
			pLineData->startPos = pLineData->endPos;
			pLineData->endPos = vTemp;

			int iTemp = pLineData->beforeLine;
			pLineData->beforeLine = pLineData->nextLine;
			pLineData->nextLine = iTemp;
		
			g_pX2Game->GetWorld()->GetLineMap()->MakeLine(pLineData);

			pLineData = pLineMap->AccessLineData(pLineData->nextLine);
		}
	}

	pLineMap->UpdateLineData();
	ParsingLine();

	RebuildStartPosition();
}

void CX2ToolMain::SetWeld()
{
	AddUndoList();

	vector<int> m_vecWeldLine;
	vector<int> m_vecWeldIndex;

	for(UINT i=0; i<m_vecLineVertex.size(); ++i)
	{
		if( m_vecLineVertex[i].m_bSelected == true )
			m_vecWeldIndex.push_back(i);
	}

	if( m_vecWeldIndex.size() < 2 || m_vecWeldIndex.size() > 2 )
		return;

	int iRemoveLine = -1;
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	D3DXVECTOR3 vDir = (m_vecLineVertex[m_vecWeldIndex[1]].vVertex - m_vecLineVertex[m_vecWeldIndex[0]].vVertex) * 0.5f;
	
	vector<int> vecBeforeLine;
	vector<int> vecNextLine;

	for(UINT i=0; i<m_vecLineVertex[m_vecWeldIndex[0]].m_vecLineInfo.size(); ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(m_vecLineVertex[m_vecWeldIndex[0]].m_vecLineInfo[i].m_iLineIndex);
		
		if( m_vecLineVertex[m_vecWeldIndex[0]].m_vecLineInfo[i].m_bStartPos == true )
		{
			pLineData->startPos = pLineData->startPos + vDir;
			vecNextLine.push_back( pLineData->lineIndex );
		}
		else
		{
			pLineData->endPos = pLineData->endPos + vDir;
			vecBeforeLine.push_back( pLineData->lineIndex );
		}
		pLineMap->MakeLine(pLineData);

		m_vecWeldLine.push_back(pLineData->lineIndex);
	}
	for(UINT i=0; i<m_vecLineVertex[m_vecWeldIndex[1]].m_vecLineInfo.size(); ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(m_vecLineVertex[m_vecWeldIndex[1]].m_vecLineInfo[i].m_iLineIndex);		
		if( m_vecLineVertex[m_vecWeldIndex[1]].m_vecLineInfo[i].m_bStartPos == true )
		{
			pLineData->startPos = pLineData->startPos - vDir;
			vecNextLine.push_back( pLineData->lineIndex );
		}
		else
		{
			pLineData->endPos = pLineData->endPos - vDir;
			vecBeforeLine.push_back( pLineData->lineIndex );
		}
		pLineMap->MakeLine(pLineData);

		m_vecWeldLine.push_back(pLineData->lineIndex);
	}

	for(UINT i=0; i<m_vecWeldLine.size()-1; ++i)
	{
		for(UINT j=i+1; j<m_vecWeldLine.size(); ++j)
		{
			if( m_vecWeldLine[i] == m_vecWeldLine[j] )
			{
				iRemoveLine = m_vecWeldLine[i];
				break;
			}
		}
		if( iRemoveLine >= 0 )
			break;
	}
	for(UINT i=0; i<vecBeforeLine.size(); ++i)
	{
		if( vecBeforeLine[i] == iRemoveLine )
			vecBeforeLine.erase( vecBeforeLine.begin() + i );
	}
	for(UINT i=0; i<vecNextLine.size(); ++i)
	{
		if( vecNextLine[i] == iRemoveLine )
			vecNextLine.erase( vecNextLine.begin() + i );
	}

	if( vecBeforeLine.size() == 1 && vecNextLine.size() == 1 )
	{
		CKTDGLineMap::LineData *pBeforeLine = pLineMap->AccessLineData( vecBeforeLine[0] );
		CKTDGLineMap::LineData *pNextLine = pLineMap->AccessLineData( vecNextLine[0] );
		if( pBeforeLine != NULL && pNextLine != NULL )
		{
			pBeforeLine->nextLine = pNextLine->lineIndex;
			pNextLine->beforeLine = pBeforeLine->lineIndex;
		}		
	}
	
	if( iRemoveLine >= 0 )
	{
		DeleteLineVertByLineIndex( iRemoveLine );
		UpdateLineMap();
	}	

	pLineMap->UpdateLineData();
	ParsingLine();

	RebuildStartPosition();
}

void CX2ToolMain::CopyLine(int iVal)
{
	if( m_bEditStartPosition == true )
		return;


	AddUndoList();

	struct COPYLINE
	{
		int iOldLineIndex;
		CKTDGLineMap::LineData *pNewLineData;
	};

	bool bHeightCopy = false;
	if( iVal != 0 )
	{
		bHeightCopy = true;
	}

	if( g_pX2Game != NULL && m_bEditLine == true && m_bEditLineVertex == false )
	{
		CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

		for(UINT i=0; i<m_vecLineVertex.size(); ++i)
			m_vecLineVertex[i].m_bSelected = false;

		vector<COPYLINE> vecCopyLine;
		for(int i=0; i<pLineMap->GetNumLineData(); ++i)
		{
			CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);
			if( pLineData != NULL && pLineData->m_bSelected == true )
			{
				pLineData->m_bOver = false;
				CKTDGLineMap::LineData *pNewLine = pLineMap->AddNewLine(pLineData);
				pNewLine->m_bSelected = false;

				if( bHeightCopy == false )
				{
					if( m_bMoveAxis == true )
					{
						if( m_bMoveAxisX == true )
						{
							pNewLine->startPos.x += 1.f;
							pNewLine->endPos.x += 1.f;
						}
						if( m_bMoveAxisY == true )
						{
							pNewLine->startPos.y += 1.f;
							pNewLine->endPos.y += 1.f;
						}
						if( m_bMoveAxisZ == true )
						{
							pNewLine->startPos.z += 1.f;
							pNewLine->endPos.z += 1.f;
						}
					}
				}
				else
				{
					pNewLine->startPos.y += (float)iVal;
					pNewLine->endPos.y += (float)iVal;
				}

				COPYLINE tCopyLine;
				tCopyLine.iOldLineIndex = pLineData->lineIndex;
				tCopyLine.pNewLineData = pNewLine;
				vecCopyLine.push_back(tCopyLine);

				pLineData->m_bSelected = false;
			}
		}

		for(UINT i=0; i<vecCopyLine.size(); ++i)
		{
			CKTDGLineMap::LineData *pLineData = vecCopyLine[i].pNewLineData;
			if( pLineData != NULL )
			{
				pLineData->m_bSelected = true;
				if( pLineData->beforeLine >= 0 )
				{
					int iNewLine = -1;
					for(UINT j=0; j<vecCopyLine.size(); ++j)
					{
						if( vecCopyLine[j].iOldLineIndex == pLineData->beforeLine )
						{
							iNewLine = vecCopyLine[j].pNewLineData->lineIndex;
							break;
						}
					}
					pLineData->beforeLine = iNewLine;
				}
				if( pLineData->nextLine >= 0 )
				{
					int iNewLine = -1;
					for(UINT j=0; j<vecCopyLine.size(); ++j)
					{
						if( vecCopyLine[j].iOldLineIndex == pLineData->nextLine )
						{
							iNewLine = vecCopyLine[j].pNewLineData->lineIndex;
							break;
						}
					}
					pLineData->nextLine = iNewLine;
				}
			}
		}

		pLineMap->UpdateLineData();
		ParsingLine();
		
		for(int i=0; i<pLineMap->GetNumLineData(); ++i)
		{
			vector<int> vecVertIndex;
			const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
			if( pLineData->m_bSelected == true )
			{
				GetVertIndexByLine(vecVertIndex, i);
				for(UINT j=0; j<vecVertIndex.size(); ++j)
				{
					int iIndex = vecVertIndex[j];
					m_vecLineVertex[iIndex].m_bSelected = true;											
					m_iVertPivot = iIndex;
				}
			}
		}

		for(UINT i=0; i<m_vecLineVertex.size(); ++i)
		{
			if( i == m_iVertPivot )
				continue;

			if( m_vecLineVertex[i].m_bSelected == true &&
				IsSamef( m_vecLineVertex[i].vVertex.x, m_vecLineVertex[m_iVertPivot].vVertex.x ) == true &&
				IsSamef( m_vecLineVertex[i].vVertex.y, m_vecLineVertex[m_iVertPivot].vVertex.y ) == true &&
				IsSamef( m_vecLineVertex[i].vVertex.z, m_vecLineVertex[m_iVertPivot].vVertex.z ) == true )
			{
				m_iVertPivot = i;
				break;
			}
		}
	}
}

void CX2ToolMain::DivideLine(int iCount)
{
	if( m_bEditStartPosition == true )
		return;

	if( m_bEditLineVertex == true || iCount <= 1 )
		return;
	
	AddUndoList();

	float fStep = 1.f / iCount;
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
		if( pLineData == NULL )
			continue;

		if( pLineData->m_bSelected == true )
		{
			int iDivideCount = 0;
			D3DXVECTOR3 vStart = pLineData->startPos;
			D3DXVECTOR3 vDir = pLineData->endPos - pLineData->startPos;									
			int iNewLineIndex = pLineData->lineIndex;
			while(iDivideCount < iCount-1)
			{
				++iDivideCount;
				D3DXVECTOR3 vPos = vStart + (vDir * (fStep*iDivideCount));
				iNewLineIndex = AddVertex( vPos, iNewLineIndex );				
			}
			
			break;
		}
	}
}

bool CX2ToolMain::SwapLine( int iOldLine, int iNewLine )
{	
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( pLineMap != NULL )
	{
		CKTDGLineMap::LineData *pOldLine = pLineMap->AccessLineData(iOldLine);
		CKTDGLineMap::LineData *pNewLine = pLineMap->AccessLineData(iNewLine);

		if( pOldLine == NULL || pNewLine == NULL )
		{
			AfxMessageBox(L"교체할 라인이 존재해야 가능합니다.", MB_OK);
			return false;
		}

		AddUndoList();

		CKTDGLineMap::LineData tempLine = *pOldLine;
		*pOldLine = *pNewLine;
		*pNewLine = tempLine;

		// old before/next
		{
			const CKTDGLineMap::LineData *pBeforeLine = pLineMap->GetLineData( pOldLine->beforeLine );
			const CKTDGLineMap::LineData *pNextLine = pLineMap->GetLineData( pOldLine->nextLine );
		}
		// new before/next
		{
			const CKTDGLineMap::LineData *pBeforeLine = pLineMap->GetLineData( pNewLine->beforeLine );
			const CKTDGLineMap::LineData *pNextLine = pLineMap->GetLineData( pNewLine->nextLine );
		}

		CKTDGLineMap::LineData *pLinkLine = pLineMap->AccessLineData(pOldLine->beforeLine);
		if( pLinkLine != NULL )
			pLinkLine->nextLine = iOldLine;
		pLinkLine = pLineMap->AccessLineData(pOldLine->nextLine);
		if( pLinkLine != NULL )
			pLinkLine->beforeLine = iOldLine;
		pLinkLine = pLineMap->AccessLineData(pNewLine->beforeLine);
		if( pLinkLine != NULL )
			pLinkLine->nextLine = iNewLine;
		pLinkLine = pLineMap->AccessLineData(pNewLine->nextLine);
		if( pLinkLine != NULL )
			pLinkLine->beforeLine = iNewLine;

		UpdateLineMap();
		pLineMap->UpdateLineData();
		ParsingLine();
		return true;
	}

	return false;
}

void CX2ToolMain::JoinVertex()
{
	if( GetSelectedLineVertCount() != 2 )
		return;

	int iBeforeIndex = -1;
	int iNextIndex = -1;
	for(UINT i=0; i<m_vecLineVertex.size(); ++i)
	{
		if( m_vecLineVertex[i].m_bSelected == true && m_vecLineVertex[i].m_vecLineInfo.size() == 1 )
		{
			if( m_vecLineVertex[i].m_vecLineInfo[0].m_bStartPos == false )
				iBeforeIndex = m_vecLineVertex[i].m_vecLineInfo[0].m_iLineIndex;
			else
				iNextIndex = m_vecLineVertex[i].m_vecLineInfo[0].m_iLineIndex;
		}

		if( iBeforeIndex != -1 && iNextIndex != -1 )
			break;
	}

	if( iBeforeIndex != -1 && iNextIndex != -1 )
	{
		JoinVertex( iBeforeIndex, iNextIndex );
	}
}

void CX2ToolMain::JoinVertex( int iBeforeIndex, int iNextIndex )
{
	AddUndoList();

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	CKTDGLineMap::LineData *pBeforeLine = pLineMap->AccessLineData( iBeforeIndex );
	CKTDGLineMap::LineData *pNextLine = pLineMap->AccessLineData( iNextIndex );

	if( pBeforeLine != NULL && pNextLine != NULL )
	{
		CKTDGLineMap::LineData *pNewLine = pLineMap->AddNewLine( pBeforeLine );		
		pNewLine->startPos = pBeforeLine->endPos;
		pNewLine->endPos = pNextLine->startPos;
		pNewLine->beforeLine = iBeforeIndex;
		pNewLine->nextLine = iNextIndex;

		pBeforeLine->nextLine = pNewLine->lineIndex;
		pNextLine->beforeLine = pNewLine->lineIndex;

		pLineMap->MakeLine(pNewLine);

		pLineMap->UpdateLineData();
		ParsingLine();
	}
}

void CX2ToolMain::DeleteLineVertByLineIndex( int iIndex, int iReplaceLine )
{
	for(UINT i=0; i<m_vecLineVertex.size(); ++i)
	{
		for(UINT j=0; j<m_vecLineVertex[i].m_vecLineInfo.size(); ++j)
		{
			if( m_vecLineVertex[i].m_vecLineInfo[j].m_iLineIndex == iIndex )
			{
				if( iReplaceLine < 0 )
					m_vecLineVertex[i].m_vecLineInfo.erase( m_vecLineVertex[i].m_vecLineInfo.begin() + j );
				else
					m_vecLineVertex[i].m_vecLineInfo[j].m_iLineIndex = iReplaceLine;
				break;
			}
		}
	}

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	pLineMap->DeleteLineData( iIndex );
}

void CX2ToolMain::SetVertexPos(D3DXVECTOR3 vPos, D3DXVECTOR3 vApply, bool bMulti)
{
	AddUndoList();

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( bMulti == false )
	{
		if( m_iVertPivot >= 0 )
		{			
			m_vecLineVertex[m_iVertPivot].vVertex = vPos;
			for(UINT i=0; i<m_vecLineVertex[m_iVertPivot].m_vecLineInfo.size(); ++i)
			{
				CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData( m_vecLineVertex[m_iVertPivot].m_vecLineInfo[i].m_iLineIndex );
				if( m_vecLineVertex[m_iVertPivot].m_vecLineInfo[i].m_bStartPos == true )
				{
					pLineData->startPos = vPos;
				}
				else
				{
					pLineData->endPos = vPos;
				}

				pLineMap->MakeLine( pLineData );
			}
		}
	}
	else
	{
		for(UINT i=0; i<m_vecLineVertex.size(); ++i)
		{
			if( m_vecLineVertex[i].m_bSelected == true )
			{
				if( vApply.x > 0.f )
					m_vecLineVertex[i].vVertex.x = vPos.x;
				if( vApply.y > 0.f )
					m_vecLineVertex[i].vVertex.y = vPos.y;
				if( vApply.z > 0.f )
					m_vecLineVertex[i].vVertex.z = vPos.z;

				for(UINT j=0; j<m_vecLineVertex[i].m_vecLineInfo.size(); ++j)
				{
					CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData( m_vecLineVertex[i].m_vecLineInfo[j].m_iLineIndex );
					if( pLineData != NULL )
					{
						if( vApply.x > 0.f )
						{
							if( m_vecLineVertex[i].m_vecLineInfo[j].m_bStartPos == true )
								pLineData->startPos.x = vPos.x;
							else
								pLineData->endPos.x = vPos.x;
						}
						if( vApply.y > 0.f )
						{
							if( m_vecLineVertex[i].m_vecLineInfo[j].m_bStartPos == true )
								pLineData->startPos.y = vPos.y;
							else
								pLineData->endPos.y = vPos.y;
						}
						if( vApply.z > 0.f )
						{
							if( m_vecLineVertex[i].m_vecLineInfo[j].m_bStartPos == true )
								pLineData->startPos.z = vPos.z;
							else
								pLineData->endPos.z = vPos.z;
						}							
					}

					pLineMap->MakeLine( pLineData );
				}
			}
		}
	}
}

void CX2ToolMain::ReMakeLineData()
{
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	for(UINT i=0; i<m_vecLineVertex.size(); ++i)
	{
		int iBeforeLine = -1;
		int iNextLine = -1;
		int iBefore = -1;
		int iNext = -1;
		for(UINT j=0; j<m_vecLineVertex[i].m_vecLineInfo.size(); ++j)
		{
			tLineVertexInfo vertInfo = m_vecLineVertex[i].m_vecLineInfo[j];
			if( vertInfo.m_bStartPos == true )
			{
				if( iNext == -1 || iNext == -2)
					iNext = vertInfo.m_iLineIndex;
			}
			else
			{
				if( iBefore == -1 || iBefore == -2)
					iBefore = vertInfo.m_iLineIndex;
			}
		}

		CKTDGLineMap::LineData *pBeforeLine = pLineMap->AccessLineData(iBefore);		
		if( pBeforeLine != NULL )
		{
			if( pBeforeLine->nextLine != iNext )
				pBeforeLine->nextLine = iNext;
		}
		CKTDGLineMap::LineData *pNextLine = pLineMap->AccessLineData(iNext);
		if( pNextLine != NULL )
		{
			if( pNextLine->beforeLine != iBefore )
				pNextLine->beforeLine = iBefore;
		}
	}
}

void CX2ToolMain::UpdateLineMap()
{
	m_vecLineInfo.clear();
	m_vecLineList.clear();
	
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	int nCount = pLineMap->GetNumLineData();

	int iNewLineIndex = 0;
	for(int i=0; i<nCount; ++i)
	{
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
		if( pLineData == NULL )
		{
			m_vecLineInfo.push_back(-1);
		}
		else
		{
			m_vecLineInfo.push_back(iNewLineIndex);
			++iNewLineIndex;
		}
	}

	for(int i=0; i<nCount; ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);
		int iNewIndex = m_vecLineInfo[i];

		if( iNewIndex >= 0 )
		{
			pLineData->lineIndex = iNewIndex;

			int iNewBeforeLine = pLineData->beforeLine;
			int iNewAfterLine = pLineData->nextLine;
			int iTelportLine = pLineData->m_iOriTeleportLineIndex;

			if( iNewBeforeLine < 0 )
				pLineData->beforeLine = iNewBeforeLine;
			else
				pLineData->beforeLine = m_vecLineInfo[iNewBeforeLine];

			if( iNewAfterLine < 0 )
				pLineData->nextLine = iNewAfterLine;
			else
				pLineData->nextLine = m_vecLineInfo[iNewAfterLine];

			if( iTelportLine < 0 )
				pLineData->m_iOriTeleportLineIndex = -1;
			else
				pLineData->m_iOriTeleportLineIndex = m_vecLineInfo[iTelportLine];
			pLineData->m_iTeleportLineIndex = pLineData->m_iOriTeleportLineIndex;

			m_vecLineList.push_back( pLineData );
		}
		else
		{
			SAFE_DELETE( pLineData );
		}
	}

	pLineMap->SwapLineData( m_vecLineList );
	m_vecLineList.clear();	
}

void CX2ToolMain::InitFillet()
{
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	if( m_bFillet == true )
	{
		JoinVertex( m_vFilletLine[0], m_vFilletLine[1] );
		
		m_bFillet = false;
		m_vFilletStartPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_vDirFillet[0] = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_vDirFillet[1] = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_vFilletLine[0] = -1;
		m_vFilletLine[1] = -1;

		return;
	}

	m_bFillet = false;
		
	for(UINT i=0; i<m_vecLineVertex.size(); ++i)
	{
		if( m_vecLineVertex[i].m_bSelected == true && m_vecLineVertex[i].m_vecLineInfo.size() >= 2 )
		{
			CKTDGLineMap::LineData *pBeforeLine = NULL;
			CKTDGLineMap::LineData *pNextLine = NULL;
			if( m_vecLineVertex[i].m_vecLineInfo[0].m_bStartPos == false )
				pBeforeLine = pLineMap->AccessLineData( m_vecLineVertex[i].m_vecLineInfo[0].m_iLineIndex );
			else
				pNextLine = pLineMap->AccessLineData( m_vecLineVertex[i].m_vecLineInfo[0].m_iLineIndex );
			if( m_vecLineVertex[i].m_vecLineInfo[1].m_bStartPos == false )
				pBeforeLine = pLineMap->AccessLineData( m_vecLineVertex[i].m_vecLineInfo[1].m_iLineIndex );
			else
				pNextLine = pLineMap->AccessLineData( m_vecLineVertex[i].m_vecLineInfo[1].m_iLineIndex );

			m_vFilletStartPos = m_vecLineVertex[i].vVertex;
			m_vDirFillet[0] = pBeforeLine->dirVector * -1.f;
			m_vDirFillet[1] = pNextLine->dirVector;
			m_vFilletLine[0] = pBeforeLine->lineIndex;
			m_vFilletLine[1] = pNextLine->lineIndex;

			return;
		}
	}

	m_vFilletStartPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vDirFillet[0] = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vDirFillet[1] = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vFilletLine[0] = -1;
	m_vFilletLine[1] = -1;
}
void CX2ToolMain::SetFillet(int iVal)
{
	if( m_bEditStartPosition == true )
		return;

	if( m_bEditLine == true && m_bEditLineVertex == true && m_vFilletLine[0] >= 0 && m_vFilletLine[1] >= 0 )
	{
		m_bFillet = true;

		CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

		CKTDGLineMap::LineData *pBeforeLine = pLineMap->AccessLineData( m_vFilletLine[0] );
		CKTDGLineMap::LineData *pNextLine = pLineMap->AccessLineData( m_vFilletLine[1] );
			 
		pBeforeLine->endPos = m_vFilletStartPos + (m_vDirFillet[0] * (float)iVal);
		pNextLine->startPos = m_vFilletStartPos + (m_vDirFillet[1] * (float)iVal);
		
		pLineMap->MakeLine( pBeforeLine );
		pLineMap->MakeLine( pNextLine );

		if( iVal == 0 )
			m_bFillet = false;
	}
}

bool CX2ToolMain::IsValidLine()
{
	for(UINT i=0; i<m_vecLineVertex.size(); ++i)
	{
		int iBeforeCount = 0;
		int iNextCount = 0;
		for(UINT j=0; j<m_vecLineVertex[i].m_vecLineInfo.size(); ++j)
		{
			if( m_vecLineVertex[i].m_vecLineInfo[j].m_bStartPos == true )
			{
				++iBeforeCount;
			}
			else
			{
				++iNextCount;
			}

			if( iBeforeCount > 1 || iNextCount > 1 )
				return false;
		}
	}

	return true;
}

void CX2ToolMain::AddUndoList()
{
	// m_vecUnDoLine
	// UpdateLineMap()
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	tLineData lineList;
	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
		CKTDGLineMap::LineData *pNewLineData = NULL;
		if( pLineData != NULL )
		{
			pNewLineData = new CKTDGLineMap::LineData();
			*pNewLineData = *pLineData;
			lineList.m_vecUnDoLineData.push_back( pNewLineData );
		}
		
	}
	m_vecUnDoLine.push_back( lineList );


	if( m_vecUnDoLine.size() > 9 )
	{
		tLineData lineList = m_vecUnDoLine[0];
		for(UINT i=0; i<lineList.m_vecUnDoLineData.size(); ++i)
		{		
			SAFE_DELETE( lineList.m_vecUnDoLineData[i] );
		}
		lineList.m_vecUnDoLineData.clear();

		m_vecUnDoLine.erase( m_vecUnDoLine.begin() );
	}
}

void CX2ToolMain::UnDoLine()
{
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( m_vecUnDoLine.size() < 1 )
		return;

	int iIndex = m_vecUnDoLine.size() - 1;

	tLineData lineList = m_vecUnDoLine[iIndex];
	pLineMap->SwapLineData( lineList.m_vecUnDoLineData );

	for(UINT i=0; i<lineList.m_vecUnDoLineData.size(); ++i)
	{		
		SAFE_DELETE( lineList.m_vecUnDoLineData[i] );
	}
	lineList.m_vecUnDoLineData.clear();

	m_vecUnDoLine.erase( m_vecUnDoLine.begin() + iIndex );

	UpdateLineMap();
	pLineMap->UpdateLineData();
	ParsingLine();

	RebuildStartPosition();
}

void CX2ToolMain::ClearUnDoList()
{
	for(UINT i=0; i<m_vecUnDoLine.size(); ++i)
	{
		tLineData lineList = m_vecUnDoLine[i];
		for(UINT j=0; j<lineList.m_vecUnDoLineData.size(); ++j)
		{		
			SAFE_DELETE( lineList.m_vecUnDoLineData[j] );
		}
		lineList.m_vecUnDoLineData.clear();

		m_vecUnDoLine.erase( m_vecUnDoLine.begin() + i );
		--i;
	}

	m_vecUnDoLine.clear();
}

void CX2ToolMain::RebuildStartPosition()
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain != NULL )
	{
		pMain->m_wndStartPosition.m_LineStartPositionDlg.RebuildStartPosition(-1);
		m_bChangeStartPos = true;
	}	
}

void CX2ToolMain::UpdateVertexProperty()
{	
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain == NULL )
		return;
	
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	pMain->m_wndProperties.SetPropValue(PI_EL_LAND_HEIGHT, (_variant_t)(float)pLineMap->GetLandHeight());

	D3DXVECTOR3 vPos = m_vecLineVertex[m_iVertPivot].vVertex;
	pMain->m_wndProperties.SetPropValue(PI_EL_VERTEX_X, (_variant_t)vPos.x);
	pMain->m_wndProperties.SetPropValue(PI_EL_VERTEX_Y, (_variant_t)vPos.y);
	pMain->m_wndProperties.SetPropValue(PI_EL_VERTEX_Z, (_variant_t)vPos.z);	
}

void CX2ToolMain::UpdateLineProperty()
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain == NULL || g_pX2Game == NULL )
		return;
		
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	pMain->m_wndProperties.SetPropValue(PI_EL_LAND_HEIGHT, (_variant_t)(float)pLineMap->GetLandHeight());

	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
		if( pLineData != NULL && pLineData->m_bSelected == true )
		{
			WCHAR *wcLineType = pMain->m_wndProperties.GetEnumString(ET_LINE_TYPE, pLineData->lineType);
			WCHAR *wcJumpDir = pMain->m_wndProperties.GetEnumString( ET_JUMP_DIR, pLineData->m_eDirection );

			pMain->m_wndProperties.SetPropValue(PI_EL_LINE_TYPE, (_variant_t)wcLineType);
			
			pMain->m_wndProperties.SetPropValue( PI_EL_LINE_JUMP_DIR, (_variant_t)wcJumpDir );
			pMain->m_wndProperties.SetPropValue( PI_EL_KEY_LOCK_TIME, (_variant_t)(float)pLineData->m_fKeyStopTime );
			
			pMain->m_wndProperties.SetPropValue(PI_EL_BEFORE_LINE, (_variant_t)(long)pLineData->beforeLine);
			pMain->m_wndProperties.SetPropValue(PI_EL_NEXT_LINE, (_variant_t)(long)pLineData->nextLine);
			pMain->m_wndProperties.SetPropValue(PI_EL_LINE_SET, (_variant_t)(long)pLineData->lineSet);
			pMain->m_wndProperties.SetPropValue(PI_EL_START_POS_X, (_variant_t)(float)pLineData->startPos.x);
			pMain->m_wndProperties.SetPropValue(PI_EL_START_POS_Y, (_variant_t)(float)pLineData->startPos.y);
			pMain->m_wndProperties.SetPropValue(PI_EL_START_POS_Z, (_variant_t)(float)pLineData->startPos.z);
			pMain->m_wndProperties.SetPropValue(PI_EL_END_POS_X, (_variant_t)(float)pLineData->endPos.x);
			pMain->m_wndProperties.SetPropValue(PI_EL_END_POS_Y, (_variant_t)(float)pLineData->endPos.y);
			pMain->m_wndProperties.SetPropValue(PI_EL_END_POS_Z, (_variant_t)(float)pLineData->endPos.z);

			pMain->m_wndProperties.SetPropValue(PI_EL_TELEPORT_LINE, (_variant_t)(long)pLineData->m_iOriTeleportLineIndex);
			pMain->m_wndProperties.SetPropValue(PI_EL_CAMERA_ID, (_variant_t)(long)pLineData->m_iOriCameraIndex);
			pMain->m_wndProperties.SetPropValue(PI_EL_TRIGGER_ID, (_variant_t)(long)pLineData->m_iTriggerId);
			pMain->m_wndProperties.SetPropValue(PI_EL_TRIGGER_ON_LINE, (_variant_t)(bool)pLineData->m_bFootOnLine);

			pMain->m_wndProperties.SetPropValue(PI_EL_ENABLE, (_variant_t)(bool)pLineData->bOriEnable);
			pMain->m_wndProperties.SetPropValue(PI_EL_CAN_DOWN, (_variant_t)(bool)pLineData->bCanDown);
			pMain->m_wndProperties.SetPropValue(PI_EL_CAN_PASS, (_variant_t)(bool)pLineData->m_bOriCanPass);
			pMain->m_wndProperties.SetPropValue(PI_EL_ENABLE_WALL_JUMP, (_variant_t)(bool)pLineData->m_bEnableWallJump);
			pMain->m_wndProperties.SetPropValue(PI_EL_MONSTER_ALLOWED, (_variant_t)(bool)pLineData->m_bMonsterAllowed);
			pMain->m_wndProperties.SetPropValue(PI_EL_OTHER_ROAD_UP, (_variant_t)(bool)pLineData->m_bOtherRoadUp);

			WCHAR *wcDustType = pMain->m_wndProperties.GetEnumString(ET_DUST_TYPE, pLineData->eDustType);
			pMain->m_wndProperties.SetPropValue(PI_EL_DUST_TYPE, (_variant_t)wcDustType);
			pMain->m_wndProperties.SetPropValue(PI_EL_STOP_FACTOR, (_variant_t)(float)pLineData->fStopFactor);
			pMain->m_wndProperties.SetPropValue(PI_EL_SPEED, (_variant_t)(float)pLineData->m_fSpeed);
			pMain->m_wndProperties.SetPropValue(PI_EL_JUMP_SPEED_X, (_variant_t)(float)pLineData->m_vOriJumpSpeed.x);
			pMain->m_wndProperties.SetPropValue(PI_EL_JUMP_SPEED_Y, (_variant_t)(float)pLineData->m_vOriJumpSpeed.y);
			pMain->m_wndProperties.SetPropValue(PI_EL_CLIFF_HEIGHT, (_variant_t)(float)pLineData->m_fCliffHeight);

			pMain->m_wndProperties.SetPropValue(PI_EL_UNDER_WATER, (_variant_t)(bool)pLineData->m_bUnderWater);
			pMain->m_wndProperties.SetPropValue(PI_EL_UNDERWATER_HEIGHT, (_variant_t)(float)pLineData->m_fWaterHeight);
			pMain->m_wndProperties.SetPropValue(PI_EL_UNDERWATER_BUOYANCY, (_variant_t)(float)pLineData->m_fBuoyancy);
			pMain->m_wndProperties.SetPropValue(PI_EL_UNDERWATER_RESISTANCE, (_variant_t)(float)pLineData->m_fResistance);
			pMain->m_wndProperties.SetPropValue(PI_EL_UNDERWATER_ANIMSPEED, (_variant_t)(float)pLineData->m_fAnimSpeed);
			pMain->m_wndProperties.SetPropValue(PI_EL_UNDERWATER_VARIANCE_OXYEN, (_variant_t)(float)pLineData->m_fVarianceOxyen);
			pMain->m_wndProperties.SetPropValue(PI_EL_UNITCOLOR_R, (_variant_t)(float)pLineData->m_cLineColor.r);
			pMain->m_wndProperties.SetPropValue(PI_EL_UNITCOLOR_G, (_variant_t)(float)pLineData->m_cLineColor.g);
			pMain->m_wndProperties.SetPropValue(PI_EL_UNITCOLOR_B, (_variant_t)(float)pLineData->m_cLineColor.b);
			pMain->m_wndProperties.SetPropValue(PI_EL_FORCE_CHANGE_COLOR, (_variant_t)(bool)pLineData->m_bForceChangeColor);

			return;
		}
	}
}

void CX2ToolMain::SetLineProperty(PROPERTY_ID ePI, int iVal, float fVal, wstring wstrVal, bool bVal)
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain == NULL || g_pX2Game == NULL )
		return;

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	if( ePI == PI_EL_LAND_HEIGHT )
	{
		float fLandHeight = fVal;
		if( fVal <= -1.f )
		{
			fLandHeight = pLineMap->GetLineData(0)->startPos.y;

			for( int i=0; i<pLineMap->GetNumLineData(); ++i )
			{		
				const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);

				if( pLineData->lineType == CKTDGLineMap::LT_BUNGEE ||
					pLineData->lineType == CKTDGLineMap::LT_MONSTER_ROAD ||
					pLineData->lineType == CKTDGLineMap::LT_WALL )
					continue;

				if( pLineData->startPos.y < fLandHeight )
					fLandHeight = pLineData->startPos.y;
				if( pLineData->endPos.y < fLandHeight )
					fLandHeight = pLineData->endPos.y;
			}
		}

		pLineMap->SetLandHeight( fLandHeight );
		g_pX2Game->GetX2Camera()->SetLandHeight( fLandHeight );
		
		UpdateLineProperty();

		return;
	}
		
	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);
		if( pLineData != NULL && pLineData->m_bSelected == true )
		{
			switch(ePI)
			{			
			case PI_EL_LINE_TYPE:
				pLineData->lineType = (CKTDGLineMap::LINE_TYPE)iVal;
				break;
			case PI_EL_LINE_JUMP_DIR:
				pLineData->m_eDirection = (CKTDGLineMap::LINEMAP_DIRECTION_OPTION)iVal;
				break;
			case PI_EL_KEY_LOCK_TIME:
				pLineData->m_fKeyStopTime = fVal;
				break;
			case PI_EL_BEFORE_LINE:
				pLineData->beforeLine = iVal;
				break;
			case PI_EL_NEXT_LINE:
				pLineData->nextLine = iVal;
				break;
			case PI_EL_LINE_SET:
				pLineData->lineSet = iVal;
				break;
			case PI_EL_START_POS_X:
				pLineData->startPos.x = fVal;
				break;
			case PI_EL_START_POS_Y:
				pLineData->startPos.y = fVal;
				break;
			case PI_EL_START_POS_Z:
				pLineData->startPos.z = fVal;
				break;
			case PI_EL_END_POS_X:
				pLineData->endPos.x = fVal;
				break;
			case PI_EL_END_POS_Y:
				pLineData->endPos.y = fVal;
				break;
			case PI_EL_END_POS_Z:
				pLineData->endPos.z = fVal;
				break;
			case  PI_EL_TELEPORT_LINE:
				pLineData->m_iOriTeleportLineIndex = iVal;
				pLineData->m_iTeleportLineIndex = iVal;
				break;
			case PI_EL_CAMERA_ID:
				pLineData->m_iOriCameraIndex = iVal;
				pLineData->m_iCameraIndex = iVal;
				break;
			case PI_EL_TRIGGER_ID:
				pLineData->m_iTriggerId = iVal;
				break;
			case PI_EL_TRIGGER_ON_LINE:
				pLineData->m_bFootOnLine = bVal;
				break;
			case PI_EL_ENABLE:
				pLineData->bOriEnable = bVal;
				pLineData->bEnable = bVal;
				break;
			case PI_EL_CAN_DOWN:
				pLineData->bCanDown = bVal;
				break;
			case PI_EL_CAN_PASS:
				pLineData->m_bCanPass = bVal;
				pLineData->m_bOriCanPass = bVal;
				break;
			case PI_EL_ENABLE_WALL_JUMP:
				pLineData->m_bEnableWallJump = bVal;
				break;
			case PI_EL_MONSTER_ALLOWED:
				pLineData->m_bMonsterAllowed = bVal;
				break;
			case PI_EL_OTHER_ROAD_UP:
				pLineData->m_bOtherRoadUp = bVal;
				break;
			case PI_EL_DUST_TYPE:
				pLineData->eDustType = (CKTDGLineMap::LINE_DUST_TYPE)iVal;
				break;
			case PI_EL_STOP_FACTOR:
				pLineData->m_fSpeed = fVal;
				break;
			case PI_EL_SPEED:
				pLineData->m_fSpeed = fVal;
				break;
			case PI_EL_JUMP_SPEED_X:
				pLineData->m_vOriJumpSpeed.x = fVal;
				pLineData->m_vJumpSpeed.x = fVal;
				break;
			case PI_EL_JUMP_SPEED_Y:
				pLineData->m_vOriJumpSpeed.y = fVal;
				pLineData->m_vJumpSpeed.y = fVal;
				break;
			case PI_EL_CLIFF_HEIGHT:
				pLineData->m_fCliffHeight = fVal;
				break;
			case PI_EL_UNDER_WATER:
				pLineData->m_bUnderWater = bVal;
				break;
			case PI_EL_UNDERWATER_HEIGHT:
				pLineData->m_fWaterHeight = fVal;
				break;
			case PI_EL_UNDERWATER_BUOYANCY:
				pLineData->m_fBuoyancy = fVal;
				break;
			case PI_EL_UNDERWATER_RESISTANCE:
				pLineData->m_fResistance = fVal;
				break;
			case PI_EL_UNDERWATER_ANIMSPEED:
				pLineData->m_fAnimSpeed = fVal;
				break;
			case PI_EL_UNDERWATER_VARIANCE_OXYEN:
				pLineData->m_fVarianceOxyen = fVal;
				break;
			case PI_EL_UNITCOLOR_R:
				CalcUnderWaterUnitColor(0, fVal);
				break;
			case PI_EL_UNITCOLOR_G:
				CalcUnderWaterUnitColor(1, fVal);
				break;
			case PI_EL_UNITCOLOR_B:
				CalcUnderWaterUnitColor(2, fVal);
				break;
			case PI_EL_FORCE_CHANGE_COLOR:
				pLineData->m_bForceChangeColor = bVal;
				break;
			default:
				break;
			}

			pLineMap->MakeLine(pLineData);
		}		
	}	
	
	if(ePI == PI_EL_BEFORE_LINE || ePI == PI_EL_NEXT_LINE || ePI == PI_EL_TELEPORT_LINE || ePI == PI_EL_ENABLE )
	{
		UpdateLineMap();
	}
	pLineMap->UpdateLineData();
	ParsingLine();

	for(UINT i=0; i<m_vecLineVertex.size(); ++i)
		m_vecLineVertex[i].m_bSelected = false;

	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		vector<int> vecVertIndex;
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
		if( pLineData->m_bSelected == true )
		{
			GetVertIndexByLine(vecVertIndex, i);
			for(UINT j=0; j<vecVertIndex.size(); ++j)
			{
				int iIndex = vecVertIndex[j];
				m_vecLineVertex[iIndex].m_bSelected = true;											
				m_iVertPivot = iIndex;
			}
		}
	}

	if( ePI == PI_EL_LINE_TYPE || 
		ePI == PI_EL_START_POS_X || ePI == PI_EL_START_POS_Y || ePI == PI_EL_START_POS_Z || 
		ePI == PI_EL_END_POS_X || ePI == PI_EL_END_POS_Y || ePI == PI_EL_END_POS_Z )
	{
		RebuildStartPosition();
	}
}

void CX2ToolMain::DeleteCameraData(int iItem)
{
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);
		if( pLineData != NULL )
		{
			if( pLineData->m_iOriCameraIndex == iItem )
			{
				pLineData->m_iOriCameraIndex = -1;
				pLineData->m_iCameraIndex = -1;
			}
			else if( pLineData->m_iOriCameraIndex > iItem )
			{
				pLineData->m_iOriCameraIndex -= 1;
				pLineData->m_iCameraIndex -= 1;
			}
		}
	}

	pLineMap->DeleteCameraData(iItem);
}

void CX2ToolMain::CalcUnderWaterUnitColor(int iFlag, float fColor)
{
	float fMaxHeight = 0.f;
	float fRecommendColor = fColor;

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{		
		const CKTDGLineMap::LineData *pLine = pLineMap->GetLineData(i);
		if( pLine != NULL && pLine->m_bSelected == true && pLine->m_bUnderWater == true )
		{			
			float fMinY = min( pLine->startPos.y, pLine->endPos.y );	
			float fLineHeight = pLine->m_fWaterHeight - fMinY;

			if( fLineHeight >= fMaxHeight )			
			{
				fMaxHeight = fLineHeight;
			}
		}		
	}

	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{		
		CKTDGLineMap::LineData *pLine = pLineMap->AccessLineData(i);
		if( pLine != NULL && pLine->m_bSelected == true && pLine->m_bUnderWater == true )
		{
			float fMinY = min( pLine->startPos.y, pLine->endPos.y );
			float fColorRate = (pLine->m_fWaterHeight - fMinY) / fMaxHeight; 
			if( fColorRate < 0.f )
				fColorRate = 0.f;
			switch(iFlag)
			{
			case 0:
				pLine->m_cLineColor.r = (fRecommendColor * fColorRate);
				break;
			case 1:
				pLine->m_cLineColor.g = (fRecommendColor * fColorRate);
				break;
			case 2:
				pLine->m_cLineColor.b = (fRecommendColor * fColorRate);
				break;
			default:
				break;
			}
		}		
	}
}

void CX2ToolMain::SelectStartPos(int iIndex)
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain != NULL )
	{
		pMain->m_wndStartPosition.m_LineStartPositionDlg.SelectStartPos( iIndex );
	}
}

int CX2ToolMain::AddStartPosition(D3DXVECTOR3 vPos, int iIndex)
{
	int iKey = -1;
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain != NULL )
	{
		iKey = pMain->m_wndStartPosition.m_LineStartPositionDlg.AddStartPosition( vPos, iIndex );
		m_bChangeStartPos = true;
	}
	return iKey;
}

void CX2ToolMain::MoveStartPosition(D3DXVECTOR3 vPos)
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain != NULL )
	{
		pMain->m_wndStartPosition.m_LineStartPositionDlg.MoveStartPosition( vPos, m_iPickedStartPosition );
		m_bChangeStartPos = true;
	}
}

void CX2ToolMain::RenderStartPos()
{	
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain == NULL )
		return;

	if( m_bEditLine == true && m_bEditStartPosition == true && m_bMoveStartPosition == true && m_iPickedStartPosition >= 0 )
	{
		D3DXVECTOR3 vPos;
		if( true == GetMousePosOnLine( &vPos, m_vMousePos ) )
		{					
			if( IsSamef(vPos.y, -1.f) == false )
			{
				MoveStartPosition( vPos );								
			}
		}
	}


	vector<CLineStartPosition::STeamStartPos> &vecTeamStartPos = pMain->m_wndStartPosition.m_LineStartPositionDlg.vecTeamStartPos;

	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	CKTDGXRenderer::RenderParam renderParam;
	D3DXMATRIX worldMatrix;	
	
	

	// render start pos
	for( unsigned i=0; i<vecTeamStartPos.size(); ++i )
	{	
		if( (m_bHideUser == true && vecTeamStartPos[i].key == -1) || (m_bHideNpc == true && vecTeamStartPos[i].key != -1) )
			continue;

		D3DXVECTOR3 vPos = vecTeamStartPos[i].vPos;
				
		int iNpcIndex = -1;
		vector<CEditDungeon::READY_NPC> vecReadyNpc = pMain->m_DlgDungeonEditor.GetNpcInfoByStartPos(vecTeamStartPos[i].key);

		//CX2UnitManager::NPC_UNIT_ID iNpcId = pMain->m_DlgDungeonEditor.GetNpcInfoByStartPos(vecTeamStartPos[i].key, iNpcIndex);
	
		if( vecTeamStartPos[i].iTeamColor == CLineStartPosition::TEAM_NONE && vecReadyNpc.size() > 0 )
			//iNpcIndex >= 0 && iNpcIndex < m_vecDungeonSkinAnimNpc.size() &&
			//iNpcId != CX2UnitManager::NUI_NONE )
		{			
			for(UINT j=0; j<vecReadyNpc.size(); ++j)
			{
				iNpcIndex = vecReadyNpc[j].m_iUnitIndex;

				if( iNpcIndex >= 0 && iNpcIndex < (int)m_vecDungeonSkinAnimNpc.size() )
				{
					D3DXVECTOR3 vAddPos = vecReadyNpc[j].m_vAddPos;

					//if( vecTeamStartPos[i].iInitialDir == CLineStartPosition::DIR_LEFT )
					if( vecReadyNpc[j].m_bIsRight == false )
						m_vecDungeonSkinAnimNpc[iNpcIndex]->GetMatrix().RotateDegree( 0, 180, 0 );
					else
						m_vecDungeonSkinAnimNpc[iNpcIndex]->GetMatrix().RotateDegree( 0, 0, 0 );
					m_vecDungeonSkinAnimNpc[iNpcIndex]->GetRenderParam()->color = D3DXCOLOR( 1, 1, 1, 1 );

					m_vecDungeonSkinAnimNpc[iNpcIndex]->GetMatrix().Move( vPos + vAddPos );

					m_vecDungeonSkinAnimNpc[iNpcIndex]->OnFrameRender();

					vecpXSkinAnim[i]->GetMatrix().Move( vPos );
				}
				else
				{
					if( vecTeamStartPos[i].iInitialDir == CLineStartPosition::DIR_LEFT )
						vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 180, 0 );
					else
						vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 0, 0 );

					if( vecTeamStartPos[i].iTeamColor == CLineStartPosition::TEAM_RED )
						vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 1, 0, 0, 1 );
					else if( vecTeamStartPos[i].iTeamColor == CLineStartPosition::TEAM_BLUE )
						vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 0, 0, 1, 1 );
					else if( vecTeamStartPos[i].iTeamColor == CLineStartPosition::TEAM_NONE )
						vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 1, 1, 1, 1 );

					vecpXSkinAnim[i]->GetMatrix().Move( vPos );

					vecpXSkinAnim[i]->OnFrameRender();
				}
			}
		}
		else
		{
			if( vecTeamStartPos[i].iInitialDir == CLineStartPosition::DIR_LEFT )
				vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 180, 0 );
			else
				vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 0, 0 );

			if( vecTeamStartPos[i].iTeamColor == CLineStartPosition::TEAM_RED )
				vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 1, 0, 0, 1 );
			else if( vecTeamStartPos[i].iTeamColor == CLineStartPosition::TEAM_BLUE )
				vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 0, 0, 1, 1 );
			else if( vecTeamStartPos[i].iTeamColor == CLineStartPosition::TEAM_NONE )
				vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 1, 1, 1, 1 );
			
			vecpXSkinAnim[i]->GetMatrix().Move( vPos );
			
			vecpXSkinAnim[i]->OnFrameRender();
		}
	}

	for( UINT i=0; i<vecTeamStartPos.size(); ++i )
	{
		if( (m_bHideUser == true && vecTeamStartPos[i].key == -1) || (m_bHideNpc == true && vecTeamStartPos[i].key != -1) )
			continue;

		if( vecTeamStartPos[i].key >= 0)
		{
			D3DXVECTOR3 vPosTemp = vecTeamStartPos[i].vPos;
			vPosTemp.y += 200.f;
			WCHAR wTemp[10];
			wsprintf(wTemp, L"%d", vecTeamStartPos[i].key);

			D3DXCOLOR vColor = 0xffffffff;
			if( (m_bMoveStartPosition == false && i == m_iOverStartPosition) || i == m_iPickedStartPosition )
			{
				vColor = 0xffff0000;
			}

			m_pFont_B->OutProjectionText( vPosTemp, wTemp, vColor, CKTDGFontManager::FS_SHELL, 0xff000000, NULL, DT_CENTER );
		}	
	}
}
void CX2ToolMain::SelectLine(int iIndex, bool bSelect)
{
	if( m_bEditLine == true && m_bEditLineVertex == false && m_bEditStartPosition == false )
	{
		CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

		for(int i=0; i<pLineMap->GetNumLineData(); ++i)
		{									
			CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);
			if( i == iIndex )
			{
				if( bSelect == true )
					pLineData->m_bSelected = true;
				else
					pLineData->m_bSelected = false;
			}
		}

		for(UINT i=0; i<m_vecLineVertex.size(); ++i)
			m_vecLineVertex[i].m_bSelected = false;

		for(int i=0; i<pLineMap->GetNumLineData(); ++i)
		{
			vector<int> vecVertIndex;
			const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
			if( pLineData->m_bSelected == true )
			{
				GetVertIndexByLine(vecVertIndex, i);
				for(UINT j=0; j<vecVertIndex.size(); ++j)
				{
					int iIndex = vecVertIndex[j];
					m_vecLineVertex[iIndex].m_bSelected = true;											
					m_iVertPivot = iIndex;
				}
			}
		}
	}	
}

float CX2ToolMain::DistanceBetweenPointAndSegment(float& fLerpCoef, D3DXVECTOR3* pvPoint,
	D3DXVECTOR3* pvSegmentStart, D3DXVECTOR3* pvSegmentEnd)
{
	D3DXVECTOR3 vTemp = *pvSegmentStart - *pvPoint;
	D3DXVECTOR3 vSegmentDir = *pvSegmentEnd - *pvSegmentStart;

	D3DXVECTOR3 vTangent;
	D3DXVec3Cross(&vTangent, &vSegmentDir, &vTemp);

	D3DXVECTOR3 vNormal;
	D3DXVec3Cross(&vNormal, &vSegmentDir, &vTangent);
	D3DXVec3Normalize(&vNormal, &vNormal);

	float fDistance = fabs( D3DXVec3Dot(&vNormal, &vTemp) );

	if( D3DXVec3Dot(&vTemp, &vNormal) < 0 )
	{
		vNormal = -vNormal;
	}

	D3DXVECTOR3 vPointOnSegment = *pvPoint + vNormal * fDistance;

	{
		D3DXVECTOR3 vTempA = vPointOnSegment - *pvSegmentStart;
		D3DXVECTOR3 vTempB;
		D3DXVec3Normalize( &vTempB, &vSegmentDir );
		fLerpCoef = D3DXVec3Dot( &vTempB, &vTempA ) / D3DXVec3Length( &vSegmentDir );
	}

	if( 0.f < fLerpCoef && fLerpCoef < 1.f )
	{
		return fDistance;
	}
	else
	{
		D3DXVECTOR3 vTempA = *pvSegmentStart - *pvPoint;
		D3DXVECTOR3 vTempB = *pvSegmentEnd   - *pvPoint;

		float fDistance1 = D3DXVec3Length( &vTempA );
		float fDistance2 = D3DXVec3Length( &vTempB );
		if( fDistance1 < fDistance2 )
		{
			fLerpCoef = 0.f;
			return fDistance1;
		}
		else
		{
			fLerpCoef = 1.f;
			return fDistance2;
		}
	}	
}

bool CX2ToolMain::GetMousePosOnLine( D3DXVECTOR3* pvPosOnLine, const D3DXVECTOR2& mousepos )
{
	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();	
	if( pLineMap == NULL || pLineMap->GetNumLineData() <= 0 )
		return false;

	const D3DXMATRIX& matView = g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix();
	D3DXMATRIX matProj;
	g_pKTDXApp->GetProjectionTransform( &matProj );

	D3DXMATRIX matViewProj, matInvViewProj;
	D3DXMatrixMultiply(&matViewProj, &matView, &matProj);
	D3DXMatrixInverse(&matInvViewProj, NULL, &matViewProj);

	float w = (float)(int)g_pKTDXApp->GetViewport().Width;
	float h = (float)(int)g_pKTDXApp->GetViewport().Height;

	float fMouseX = (float)(mousepos.x-w/2.f)/w *2.f;
	float fMouseY = (float)(h/2.f-mousepos.y)/h *2.f;
	D3DXVECTOR3 vMouse(fMouseX, fMouseY, 0.f);

	CKTDGLineMap::LineData* pLineData = NULL;	

	int iPickedSegmentIndex = 0;
	float fDistance = 0.f;
	float fMinDistance = -1.f;
	float fLerpCoef, fLerpCoefPicked;
	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		pLineData = pLineMap->AccessLineData(i);
		D3DXVECTOR3 vStartPos, vEndPos;
		D3DXVec3TransformCoord( &vStartPos, &pLineData->startPos, &matViewProj );
		D3DXVec3TransformCoord( &vEndPos, &pLineData->endPos, &matViewProj);
		vStartPos.z = 0.f;
		vEndPos.z   = 0.f;

		fDistance = DistanceBetweenPointAndSegment(fLerpCoef, &vMouse, &vStartPos, &vEndPos);

		if( -1.f == fMinDistance || fDistance < fMinDistance )
		{
			iPickedSegmentIndex = i;
			fMinDistance = fDistance;
			fLerpCoefPicked = fLerpCoef;
		}
	}

	float fDistanceBound = 0.1f;
	if( fMinDistance < fDistanceBound )
	{
		const CKTDGLineMap::LineData* pConstLineData = pLineMap->GetLineData(iPickedSegmentIndex);

		D3DXVECTOR3 vStartPos = pConstLineData->startPos;
		D3DXVECTOR3 vEndPos   = pConstLineData->endPos;

		D3DXVECTOR3 vPointOnSegmentPicked = vStartPos + (vEndPos-vStartPos) * fLerpCoefPicked;
		*pvPosOnLine = vPointOnSegmentPicked;
		return true;
	}	
	return false;
}

bool CX2ToolMain::SaveLineMapToFile( const WCHAR* wszFileName, bool bUpdateFileName )
{	
	if( bUpdateFileName == true )
	{
		m_wstrLineMapFullName = wszFileName;
	}

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();	
	if( pLineMap == NULL )
		return false;

	bool bResult = true;

	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain == NULL )
		return false;

	ClearUnDoList();

	pMain->m_wndStartPosition.m_LineStartPositionDlg.SortStartPosition();
	vector<CLineStartPosition::STeamStartPos> &vecTeamStartPos = pMain->m_wndStartPosition.m_LineStartPositionDlg.vecTeamStartPos;	
	

	//LineMapFloatToInt();
	//RecomputeStartPosLineIndex();

	// 파일 확장자가 없으면 붙여주자.
	std::string strFileName;	
	strFileName.reserve(512);
	ConvertWCHARToChar( strFileName, m_wstrLineMapFullName.c_str() );	
	MakeUpperCase( strFileName );

	int iStrLen = strFileName.length();
	if( 'L' != strFileName[iStrLen-3] || 'U' != strFileName[iStrLen-2] || 'A' != strFileName[iStrLen-1] ) 
	{
		ConvertWCHARToChar( strFileName, m_wstrLineMapFullName.c_str() );	
		strFileName.append(".lua");
	}
	else
	{
		ConvertWCHARToChar( strFileName, m_wstrLineMapFullName.c_str() );	
	}

	std::wstring wstrFilePath;
	ConvertCharToWCHAR( wstrFilePath, strFileName.c_str() );

	std::ofstream ofs( strFileName.c_str() );
	if( ofs.is_open() == false )
	{
		ofs.close();
		return false;
	}

	//if( true == bUpdateFileName )
	//	m_wstrLineMapFileName = wstrFilePath;

	ofs << std::endl << std::endl;
	//ofs << "g_pLineMap:SetLandHeight( " << pLineMap->GetLandHeight() << " )" << std::endl;
	float fLandHeight = pLineMap->GetLandHeight();
	ofs << "g_pLineMap:SetLandHeight( " << fLandHeight << " )" << std::endl;
	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << "-- RED, BLUE, TEAM START POS" << std::endl;

	// red, blue startpos을 출력하자
	string strDirection, strTeamColor;
	for( unsigned j=0; j<vecTeamStartPos.size(); j++ )
	{
		CLineStartPosition::STeamStartPos& spos = vecTeamStartPos[j];

		if( spos.iInitialDir == CLineStartPosition::DIR_LEFT )
			strDirection = "false";
		else 
			strDirection = "true";

		if( spos.iTeamColor == CLineStartPosition::TEAM_RED )
			strTeamColor = "true";
		else if( spos.iTeamColor == CLineStartPosition::TEAM_BLUE )
			strTeamColor = "false";

		if( spos.iTeamColor != CLineStartPosition::TEAM_NONE )
		{
			ofs << "g_pLineMap:SetTeamStartPos( " << strTeamColor << ", D3DXVECTOR3( " 
				<< spos.vPos.x << ", " << spos.vPos.y << ", " << spos.vPos.z << " ), " 
				<< strDirection << ", " << spos.iLineIndex << " )" << std::endl;
		}
	}

	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << "-- NPC (or else) START POS" << std::endl;
	for( unsigned j=0; j<vecTeamStartPos.size(); j++ )
	{
		CLineStartPosition::STeamStartPos& spos = vecTeamStartPos[j];

		if( spos.iInitialDir == CLineStartPosition::DIR_LEFT )
			strDirection = "false";
		else 
			strDirection = "true";

		if( spos.iTeamColor == CLineStartPosition::TEAM_NONE )
		{
			ofs << "g_pLineMap:AddStartPos( "
				<< spos.key << ", D3DXVECTOR3( " 
				<< spos.vPos.x << ", " << spos.vPos.y << ", " << spos.vPos.z << " ), " 
				<< strDirection << ", " << spos.iLineIndex << " )" << std::endl;
		}
	}

	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << "-- LINE DATA -- " << std::endl;

	// 라인정보 출력
	CKTDGLineMap::LineData* pLineData = NULL;	
	std::string strFootDustName;
	strFootDustName.reserve(512);
	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		pLineData = pLineMap->AccessLineData(i);

		//if( pLineData->lineSet < 0 )
		//	bThereAreInvalidLineSet = true;

		ofs << "g_pLineMap:AddLine_LUA" << std::endl;
		ofs << "{" << std::endl;
		ofs << "	LINE_NUM		= " << i << ", " << std::endl;
		if( true == pLineData->bOriEnable )
		{
			//ofs << "	ENABLE			= TRUE," << std::endl;
		}
		else
		{
			ofs << "	ENABLE			= FALSE," << std::endl;
		}

		ofs << "	LINE_TYPE 		= ";

		switch( pLineData->lineType )
		{
		default:
		case CKTDGLineMap::LT_NORMAL:
			ofs << "LINE_TYPE[\"LT_NORMAL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_WALL:
			ofs << "LINE_TYPE[\"LT_WALL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_POTAL:
			ofs << "LINE_TYPE[\"LT_POTAL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_HOLE:
			ofs << "LINE_TYPE[\"LT_HOLE\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_JUMP_UP_REL:
			ofs << "LINE_TYPE[\"LT_JUMP_UP_REL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_JUMP_UP_ABS:
			ofs << "LINE_TYPE[\"LT_JUMP_UP_ABS\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_BUNGEE:
			ofs << "LINE_TYPE[\"LT_BUNGEE\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_MONSTER_ROAD:
			ofs << "LINE_TYPE[\"LT_MONSTER_ROAD\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_UNIT_ROAD:
			ofs << "LINE_TYPE[\"LT_UNIT_ROAD\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_OTHER_ROAD:
			ofs << "LINE_TYPE[\"LT_OTHER_ROAD\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_CLIFF:
			ofs << "LINE_TYPE[\"LT_CLIFF\"], " << std::endl;
			break;			
		}

		ofs << "	LINEMAP_DIRECTION_OPTION 		= ";
		switch( pLineData->m_eDirection )
		{
		case CKTDGLineMap::LDO_NONE:
			ofs << "LINEMAP_DIRECTION_OPTION[\"LDO_NONE\"], " << std::endl;
			break;

		case CKTDGLineMap::LDO_LEFT:
			ofs << "LINEMAP_DIRECTION_OPTION[\"LDO_LEFT\"], " << std::endl;
			break;

		case CKTDGLineMap::LDO_RIGHT:
			ofs << "LINEMAP_DIRECTION_OPTION[\"LDO_RIGHT\"], " << std::endl;
			break;
		}

		ofs << "	JUMP_REL_UP_KEYSTOP_TIME	= " << pLineData->m_fKeyStopTime << ", " << std::endl;

		if( -1 != pLineData->potalGroup )
		{
			ofs << "	POTAL_GROUP		= " << pLineData->potalGroup << ", " << std::endl;
		}

		if( 1.f != pLineData->fStopFactor )
		{
			ofs << "	STOP_FACTOR		= " << pLineData->fStopFactor << ", " << std::endl;
		}


		switch( pLineData->eDustType )
		{
		default:
		case CKTDGLineMap::LDT_DEFALT_DUST:
			{
				//ofs << "	DUST_TYPE   = LINE_DUST_TYPE[\"LDT_DEFALT_DUST\"]," << std::endl;
			} break;

		case CKTDGLineMap::LDT_WATER_SPLASH:
			{
				ofs << "    DUST_TYPE   = LINE_DUST_TYPE[\"LDT_WATER_SPLASH\"]," << std::endl;
			} break;
		}

		ofs << "	BEFORE_LINE		= " << pLineData->beforeLine	<< ", " << std::endl;
		ofs << "	NEXT_LINE		= " << pLineData->nextLine		<< ", " << std::endl;
		ofs << "	LINE_SET		= " << pLineData->lineSet		<< ", " << std::endl;

		if( 0.f != pLineData->m_fOriSpeed )
		{
			ofs << "	SPEED			= " << pLineData->m_fOriSpeed << ", " << std::endl;
		}


		if( 0.f != pLineData->m_vOriJumpSpeed.x )
		{
			ofs << "	JUMP_SPEED_X			= " << pLineData->m_vOriJumpSpeed.x << ", " << std::endl;
		}


		if( 0.f != pLineData->m_vOriJumpSpeed.y )
		{
			ofs << "	JUMP_SPEED_Y			= " << pLineData->m_vOriJumpSpeed.y << ", " << std::endl;
		}


		if( false == pLineData->bCanDown )
		{
			ofs << "	CAN_DOWN		= FALSE, " << std::endl;
		}

		if( false == pLineData->m_bOriCanPass )
		{
			ofs << "	CAN_PASS		= FALSE, " << std::endl;
		}

		if( -1 != pLineData->m_iOriTeleportLineIndex )
		{
			ofs << "	TELEPORT_LINE		= " << pLineData->m_iTeleportLineIndex << ", " << std::endl;
		}

		if( true == pLineData->m_bEnableWallJump )
		{
			ofs << "	ENABLE_WALL_JUMP		= TRUE, " << std::endl;
		}

		if( false == pLineData->m_bMonsterAllowed )
		{
			ofs << "	MONSTER_ALLOWED		= FALSE, " << std::endl;
		}

		if( pLineData->m_iOriCameraIndex != -1 )
		{
			ofs << "	CAMERA_INDEX	= " << pLineData->m_iCameraIndex << ", " << std::endl;
		}
		if( pLineData->m_iTriggerId != -1 )
		{
			ofs << "	TRIGGER_ID		= " << pLineData->m_iTriggerId << ", " << std::endl;
			if( pLineData->m_bFootOnLine )
			{
				ofs << "	TRIGGER_ON_LINE	= TRUE," << std::endl;
			}
			else
			{
				ofs << "	TRIGGER_ON_LINE	= FALSE," << std::endl;
			}

		}

		ofs << "	\"START_POS		= D3DXVECTOR3(" << pLineData->startPos.x << ", " << pLineData->startPos.y << ", " << pLineData->startPos.z << ")\", " << std::endl;
		ofs << "	\"END_POS		= D3DXVECTOR3(" << pLineData->endPos.x << ", " << pLineData->endPos.y << ", " << pLineData->endPos.z << ")\", " << std::endl;

#ifdef UNDERWATER_LINEMAP
		if( true == pLineData->m_bUnderWater )
		{
			ofs << "	IS_UNDERWATER			= TRUE," << std::endl;
			ofs << "	UNDERWATER_HEIGHT		= " << pLineData->m_fWaterHeight << ", " << std::endl;
			ofs << "	UNDERWATER_BUOYANCY		= " << pLineData->m_fBuoyancy << ", " << std::endl;
			ofs << "	UNDERWATER_RESISTANCE	= " << pLineData->m_fResistance << ", " << std::endl;
			ofs << "	UNDERWATER_ANIMSPEED	= " << pLineData->m_fAnimSpeed << ", " << std::endl;
			ofs << "	UNDERWATER_VARIANCEOXYEN= " << pLineData->m_fVarianceOxyen << ", " << std::endl;			
		}		
		if( pLineData->m_bForceChangeColor == true )
		{
			ofs << "	FORCE_CHANGE_COLOR		= TRUE," << std::endl;
		}

		if( pLineData->m_bUnderWater == true || pLineData->m_bForceChangeColor == true )
		{
			ofs << "	UNIT_COLOR_R			= " << pLineData->m_cLineColor.r << ", " << std::endl;
			ofs << "	UNIT_COLOR_G			= " << pLineData->m_cLineColor.g << ", " << std::endl;
			ofs << "	UNIT_COLOR_B			= " << pLineData->m_cLineColor.b << ", " << std::endl;
		}		
#endif

		ofs << "}" << std::endl;
		ofs << std::endl;
	}

	// 카메라 정보 출력
	ofs << std::endl;
	ofs << "-- CAMERA DATA " << std::endl;
	ofs << std::endl;

	CKTDGLineMap::CameraData * pCameraData = NULL;	

	for( int i = 0; i < pLineMap->GetNumCameraData(); ++i )
	{
		pCameraData = pLineMap->GetCameraDataListByTool(i);

		ofs << "g_pLineMap:AddCameraData_LUA" << std::endl;
		ofs << "{" << std::endl;


		//{{ 내용출력 시작.		
		if( 0.f == pCameraData->m_vRelativeEye.x &&
			0.f == pCameraData->m_vRelativeEye.y &&
			0.f == pCameraData->m_vRelativeEye.z )
			//if( pCameraData->m_eType == CKTDGLineMap::LCT_ABSOULTE_POSITION )
		{
			if( pCameraData->m_bFocusUnit == false )
			{
				ofs << "	FOCUS_UNIT	= FALSE," << std::endl;
			}
			ofs << "	\"EYE_POS	= D3DXVECTOR3(" << pCameraData->m_vEye.x << ", " << pCameraData->m_vEye.y << ", " << pCameraData->m_vEye.z << ")\", " << std::endl;
			ofs << "	\"LOOKAT_POS	= D3DXVECTOR3(" << pCameraData->m_vLookAt.x << ", " << pCameraData->m_vLookAt.y << ", " << pCameraData->m_vLookAt.z << ")\", " << std::endl;
		}
		else //if( pCameraData->m_eType == CKTDGLineMap::LCT_RELATIE_POSITION )
		{
			ofs << "	RELATIVE_EYE_POS_X	= " << pCameraData->m_vRelativeEye.x << ", " << std::endl;
			ofs << "	RELATIVE_EYE_POS_Y	= " << pCameraData->m_vRelativeEye.y << ", " << std::endl;
			ofs << "	RELATIVE_EYE_POS_Z	= " << pCameraData->m_vRelativeEye.z << ", " << std::endl;

			ofs << "	CAMERA_REPOSITION_SPEED = " << pCameraData->m_fCameraRepositionSpeed << ", " << std::endl;
		}
		//}} 내용출력 끝.
		ofs << "}" << std::endl;
		ofs << std::endl;
	}

	// 트리거 정보 출력
	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
	for(UINT iTrigger=0; iTrigger<pTrigger->m_vecTriggerSeg.size(); ++iTrigger)
	{
		CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pTrigger->m_vecTriggerSeg[iTrigger];

		ofs << "g_pLineMap:AddTrigger_LUA" <<std::endl;
		ofs << "{" << std::endl;

		ofs << "	TRIGGER_ID		= " << iTrigger << "," << std::endl;		
		if( triggerSeg.m_bLoop )		
			ofs << "	TRIGGER_LOOP	= TRUE," << std::endl;
		else
			ofs << "	TRIGGER_LOOP	= FALSE," << std::endl;

		for(UINT iData=0; iData<triggerSeg.m_vecTriggerData.size(); ++iData)
		{
			CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[iData];

			ofs << "	TRIGGER_DATA" << iData << " =" << std::endl;
			ofs << "	{" << std::endl;

			for(UINT iMesh=0; iMesh<triggerData.m_vecTriggerMesh.size(); ++iMesh)
			{
				CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh = triggerData.m_vecTriggerMesh[iMesh];

				ofs << "		TRIGGER_DATA_MESH" << iMesh << " =" << std::endl;
				ofs << "		{" << std::endl;

				switch(triggerMesh.m_iMeshTriggerAction)
				{
				case CKTDGLineMap::TAT_MESH_NONE:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_MESH_ACTION_TYPE[\"TAT_MESH_NONE\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerMesh.m_fTriggerTime << "," << std::endl;						
					}					
					break;
				case CKTDGLineMap::TAT_MESH_CHANGE_ANIM:
					{
						std::string strMeshName, strAnimName;
						ConvertWCHARToChar( strMeshName, triggerMesh.m_wstrMeshName );
						ConvertWCHARToChar( strAnimName, triggerMesh.m_wstrName );	

						ofs << "			TRIGGER_ACTION = TRIGGER_MESH_ACTION_TYPE[\"TAT_MESH_CHANGE_ANIM\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerMesh.m_fTriggerTime << "," << std::endl;
						ofs << "			MESH_NAME = \"" << strMeshName << "\"," << std::endl;
						ofs << "			ANIM_NAME = \"" << strAnimName << "\"," << std::endl;
						ofs << "			PLAY_TYPE = " << triggerMesh.m_iPlayType << "," << std::endl;
					}					
					break;
				case CKTDGLineMap::TAT_MESH_CHANGE_SPEED:
					{
						std::string strMeshName;
						ConvertWCHARToChar( strMeshName, triggerMesh.m_wstrMeshName );

						ofs << "			TRIGGER_ACTION = TRIGGER_MESH_ACTION_TYPE[\"TAT_MESH_CHANGE_SPEED\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerMesh.m_fTriggerTime << "," << std::endl;
						ofs << "			MESH_NAME = \"" << strMeshName << "\"," << std::endl;
						ofs << "			ANIM_SPEED = " << triggerMesh.m_fAnimSpeed << "," << std::endl;
					}					
					break;
				default:					
					break;
				}
				ofs << "		}," << std::endl;
			}

			for(UINT iLine=0; iLine<triggerData.m_vecTriggerLine.size(); ++iLine)
			{
				CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine = triggerData.m_vecTriggerLine[iLine];

				ofs << "		TRIGGER_DATA_LINE" << iLine << " =" << std::endl;
				ofs << "		{" << std::endl;

				switch(triggerLine.m_iLineTriggerAction)
				{
				case CKTDGLineMap::TAT_LINE_NONE:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_NONE\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_ENABLE:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_ENABLE\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						if( triggerLine.m_bEnable )
							ofs << "			LINE_ENABLE	= TRUE," << std::endl;
						else
							ofs << "			LINE_ENABLE	= FALSE," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_SPEED:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_SPEED\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						ofs << "			LINE_SPEED	= " << triggerLine.m_fSpeed << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_JUMPSPEED:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_JUMPSPEED\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						ofs << "			LINE_JUMPSPEED_X	= " << triggerLine.m_vJumpSpeed.x << "," << std::endl;
						ofs << "			LINE_JUMPSPEED_Y	= " << triggerLine.m_vJumpSpeed.y << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_CAMERA:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_CAMERA\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						ofs << "			LINE_CAMERA	= " << triggerLine.m_iCamera << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_CANPASS:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_CANPASS\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						if( triggerLine.m_bEnable )
							ofs << "			LINE_CANPASS	= TRUE," << std::endl;
						else
							ofs << "			LINE_CANPASS	= FALSE," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_TELEPORT:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_TELEPORT\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						ofs << "			LINE_TELEPORT	= " << triggerLine.m_iCamera << "," << std::endl;
					}
					break;
				default:					
					break;
				}
				ofs << "		}," << std::endl;
			}

			for(UINT iEtc=0; iEtc<triggerData.m_vecTriggerEtc.size(); ++iEtc)
			{
				CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc = triggerData.m_vecTriggerEtc[iEtc];

				ofs << "		TRIGGER_DATA_ETC" << iEtc << " =" << std::endl;
				ofs << "		{" << std::endl;

				switch(triggerEtc.m_iEtcTriggerAction)
				{
				case CKTDGLineMap::TAT_ETC_NONE:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_NONE\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_ETC_PLAY_SOUND:
					{
						std::string strActionName;							
						ConvertWCHARToChar( strActionName, triggerEtc.m_wstrActionName );

						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_PLAY_SOUND\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_SOUND = \"" << strActionName << "\"," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_ETC_DURATION_TIME:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_DURATION_TIME\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_ETC_CAMERA:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_CAMERA\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
						ofs << "			CAMERA_INDEX = " << triggerEtc.m_iDummyValue01 << "," << std::endl;
						ofs << "			CAMERA_DURATION = " << triggerEtc.m_fDummyValue01 << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_ETC_CAMERA_SHAKE:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_CAMERA_SHAKE\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
						ofs << "			CAMERA_SHAKE_GAP = " << triggerEtc.m_iDummyValue01 << "," << std::endl;
						ofs << "			CAMERA_SHAKE_DURATION = " << triggerEtc.m_fDummyValue01 << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_ETC_ACTIVE_LAYER:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_ACTIVE_LAYER\"]," << std::endl;
						ofs << "			DELAY_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
						ofs << "			ACTIVE_LAYER_ID = " << triggerEtc.m_iDummyValue01 << "," << std::endl;
						ofs << "			FADE_TIME = " << triggerEtc.m_fDummyValue01 << "," << std::endl;						
					}
					break;
				default:					
					break;
				}
				ofs << "		}," << std::endl;
			}

			ofs << "	}," << std::endl;
		}

		ofs << "}" << std::endl;
	}



	ofs.close();

	ConvertFullPathFileAnsiToUTF8( strFileName, strFileName );

	LoadLineMap( m_wstrLineMapFullName.c_str() );

	m_bChangeStartPos = false;

	return bResult;
}

void CX2ToolMain::ExportLineMap(wstring wstrExportName)
{
	FILE *fp = NULL;
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	wstring wstrMSName = wstrExportName;
	wstrMSName = wstrMSName + L".ms";
	string strMSName = "";
	ConvertWCHARToChar(strMSName, wstrMSName);
	fp = fopen(strMSName.c_str(), "w");
	if( pLineMap != NULL && fp != NULL )
	{
		fprintf(fp, "fn drawLineBetweenTwoPoints pointA pointB clr = \n(\n");
		fprintf(fp, "local sp = SplineShape pos:pointA\n");
		fprintf(fp, "addNewSpline sp\n");
		fprintf(fp, "addKnot sp 1 #corner #line pointA\n");
		fprintf(fp, "addKnot sp 1 #corner #line pointB\n");
		fprintf(fp, "updateShape sp\n");
		fprintf(fp, "sp.wirecolor = clr\n)\n\n");

		for(int i=0; i<pLineMap->GetNumLineData(); ++i)
		{
			CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);
			if( pLineData != NULL )
			{
				fprintf(fp, "drawLineBetweenTwoPoints [%f,%f,%f] [%f,%f,%f] red\n", 
					pLineData->startPos.x, pLineData->startPos.z, pLineData->startPos.y,
					pLineData->endPos.x, pLineData->endPos.z, pLineData->endPos.y );
			}
		}
	}

	fclose(fp);
}

void CX2ToolMain::ReArrangeLine()
{
	m_vecLineInfo.clear();
	m_vecLineList.clear();	

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	int nCount = pLineMap->GetNumLineData();
	int iNewLineIndex = 0;
	
	for(int i=0; i<nCount; ++i)
	{
		m_vecLineInfo.push_back(-1);
		m_vecLineList.push_back(NULL);
	}

	std::sort(m_vecLineVertex.begin(), m_vecLineVertex.end(), CompareVertex);

	for(UINT i=0; i<m_vecLineVertex.size(); ++i)
	{
		tLineVertex tl = m_vecLineVertex[i];
		if( tl.m_vecLineInfo.size() == 1 && tl.m_vecLineInfo[0].m_bStartPos == true )
		{
			int iStartLine = tl.m_vecLineInfo[0].m_iLineIndex;
			int iNextLine = iStartLine;
			while( iNextLine >= 0 )
			{
				const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData( iNextLine );
				if( pLineData != NULL )
				{
					m_vecLineInfo[iNextLine] = iNewLineIndex;
					++iNewLineIndex;

					iNextLine = pLineData->nextLine;
				}
			}
		}
	}
	
	if( iNewLineIndex != nCount )
	{
		AfxMessageBox(L"재정렬 불가!!!.", MB_OK);
		m_vecLineInfo.clear();
		m_vecLineList.clear();
		return;
	}

	for(int i=0; i<nCount; ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(i);
		int iNewIndex = m_vecLineInfo[i];

		if( iNewIndex >= 0 )
		{
			pLineData->lineIndex = iNewIndex;

			int iNewBeforeLine = pLineData->beforeLine;
			int iNewAfterLine = pLineData->nextLine;
			int iTelportLine = pLineData->m_iOriTeleportLineIndex;

			if( iNewBeforeLine < 0 )
				pLineData->beforeLine = iNewBeforeLine;
			else
				pLineData->beforeLine = m_vecLineInfo[iNewBeforeLine];

			if( iNewAfterLine < 0 )
				pLineData->nextLine = iNewAfterLine;
			else
				pLineData->nextLine = m_vecLineInfo[iNewAfterLine];

			if( iTelportLine < 0 )
				pLineData->m_iOriTeleportLineIndex = -1;
			else
				pLineData->m_iOriTeleportLineIndex = m_vecLineInfo[iTelportLine];
			pLineData->m_iTeleportLineIndex = pLineData->m_iOriTeleportLineIndex;

			m_vecLineList[iNewIndex] = pLineData;
		}
	}

	pLineMap->SwapLineData( m_vecLineList );
	m_vecLineList.clear();	

	pLineMap->UpdateLineData();
	ParsingLine();

	RebuildStartPosition();
}

void CX2ToolMain::LoadWorld( const WCHAR* pwszFileName )
{
	CompulsorybyConvertFileAnsiToUTF8W( pwszFileName );

	WCHAR wszFilePathOnly[512] = {NULL};
	WCHAR wszFileNameOnly[256] = {NULL};
	DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, pwszFileName );
	//m_wstrLineMapFilePath = wszFilePathOnly;
	//m_wstrLineMapFileName = wszFileNameOnly;	
	
	g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory(wszFilePathOnly);
	

	CX2World *pWorld = g_pX2Game->GetWorld();

	if ( NULL != pWorld )
		pWorld->RemoveWordLayer(-1);
	
	SAFE_DELETE(pWorld);
	CX2World::WorldData worldData;
	worldData.dataFileName = wszFileNameOnly;
	pWorld = new CX2World( &worldData );
	pWorld->OpenScriptFile( wszFileNameOnly );
	g_pX2Game->SetWorld( pWorld );

	if( NULL != g_pX2Game && 
		NULL != g_pX2Game->GetWorld() && 
		NULL != g_pX2Game->GetWorld()->GetLineMap() &&
		NULL != g_pX2Game->GetX2Camera() )
	{
		float fLandHeight = g_pX2Game->GetWorld()->GetLineMap()->GetLandHeight();
		g_pX2Game->GetX2Camera()->SetLandHeight( fLandHeight );

		m_wstrLineMapFullName	= g_pX2Game->GetWorld()->GetLineMap()->GetLineFullName();
		m_wstrWorldFullName		= g_pX2Game->GetWorld()->GetWorldName();
		m_wstrSoundName			= g_pX2Game->GetWorld()->GetBGMName();

		g_pX2Game->GetMyUnit()->ClearRecieveSyncData();
		g_pX2Game->GetMyUnit()->InitPosition(true, 0);

		if( NULL != g_pX2Game && NULL != g_pX2Game->GetWorld() )
		{
			float fLandHeight = g_pX2Game->GetWorld()->GetLineMap()->GetLandHeight();
			g_pX2Game->GetX2Camera()->SetLandHeight( fLandHeight );
		}

		ParsingLine();

		for(int i=vecpXSkinAnim.size()-1; i>=0; --i)
			DeleteObjectStartPosition(i);
		vecpXSkinAnim.clear();

		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
		if( pMain != NULL )
		{			
			pMain->m_wndCameraInfo.m_LineCameraDlg.ResetCameraList();
			pMain->m_wndStartPosition.m_LineStartPositionDlg.ResetStartPosition();
			pMain->m_wndDungeonInfo.m_DungeonDlg.m_editLineName.SetWindowText( g_pX2Game->GetWorld()->GetLineMap()->GetLineFullName().c_str() );
			pMain->m_wndDungeonInfo.m_DungeonDlg.m_editWorldName.SetWindowText( pwszFileName );
			pMain->m_wndTriggerProperties.InitPropList();

			pMain->m_DlgWorldEditor.ResetWorldEditor();
			pMain->m_DlgDungeonEditor.ResetWorld(m_wstrWorldFullName);
		}

		ClearUnDoWorld();
		m_bChangeStartPos = false;
	}
}

int CX2ToolMain::SelectWorldlayer()
{
	int iCount = 0;
 	if( m_bEditWorld == true )
 	{
 		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
 		CX2World *pWorld = g_pX2Game->GetWorld();
 		vector<CX2WorldLayer*> vecWorldlayer = pWorld->GetWorldLayerList();
 
		for(UINT i=0; i<vecWorldlayer.size(); ++i)
		{							
			UINT uItemState = pMain->m_DlgWorldEditor.m_ctrlListWorldLayer.GetItemState ( i, LVIS_SELECTED );
			CX2WorldLayer *pWorldLayer = vecWorldlayer[i];
			if( pWorldLayer != NULL )
			{
				if( uItemState == LVIS_SELECTED )
				{
					pWorldLayer->m_bSelected = true;
					++iCount;
				}
				else
				{
					pWorldLayer->m_bSelected = false;
				}
			}			
		}
 	}
	return iCount;
}

void CX2ToolMain::SetWorldlayerProperty(PROPERTY_ID ePI, int iVal, float fVal, wstring wstrVal, bool bVal )
{
	if( m_bEditWorld == true )
	{
		CX2World *pWorld = g_pX2Game->GetWorld();
		vector<CX2WorldLayer*> vecWorldlayer = pWorld->GetWorldLayerList();

		for(UINT i=0; i<vecWorldlayer.size(); ++i)
		{									
			CX2WorldLayer *pWorldLayer = vecWorldlayer[i];
			if( pWorldLayer != NULL && pWorldLayer->m_bSelected == true )
			{
				switch( ePI )
				{
				case PI_EW_WORLD_LAYER_TEXTURE:
					pWorldLayer->SetTexW( wstrVal );
					break;
				case PI_EW_WORLD_LAYER_LAYER:
					pWorldLayer->SetLayer(iVal);
					break;
				case PI_EW_WORLD_LAYER_BLENDTYPE:
					pWorldLayer->SetBlendType(iVal);
					break;
				case PI_EW_WORLD_LAYER_POS_X:
					{
						float fx, fy, fw, fh;
						pWorldLayer->GetPos(&fx, &fy, &fw, &fh);
						fx = fVal;
						pWorldLayer->SetPos(fx, fy, fw, fh);
					}
					break;
				case PI_EW_WORLD_LAYER_POS_Y:
					{
						float fx, fy, fw, fh;
						pWorldLayer->GetPos(&fx, &fy, &fw, &fh);
						fy = fVal;
						pWorldLayer->SetPos(fx, fy, fw, fh);
					}
					break;
				case PI_EW_WORLD_LAYER_WIDTH:
					{
						float fx, fy, fw, fh;
						pWorldLayer->GetPos(&fx, &fy, &fw, &fh);
						fw = fVal;
						pWorldLayer->SetPos(fx, fy, fw, fh);
					}
					break;
				case PI_EW_WORLD_LAYER_HEIGHT:
					{
						float fx, fy, fw, fh;
						pWorldLayer->GetPos(&fx, &fy, &fw, &fh);
						fh = fVal;
						pWorldLayer->SetPos(fx, fy, fw, fh);
					}
					break;
				case PI_EW_WORLD_LAYER_COLOR_R:
					{
						D3DXCOLOR vColor = pWorldLayer->GetColor();
						vColor.r = fVal;
						pWorldLayer->SetColor( vColor );
					}
					break;
				case PI_EW_WORLD_LAYER_COLOR_G:
					{
						D3DXCOLOR vColor = pWorldLayer->GetColor();
						vColor.g = fVal;
						pWorldLayer->SetColor( vColor );
					}
					break;
				case PI_EW_WORLD_LAYER_COLOR_B:
					{
						D3DXCOLOR vColor = pWorldLayer->GetColor();
						vColor.b = fVal;
						pWorldLayer->SetColor( vColor );
					}
					break;
				case PI_EW_WORLD_LAYER_COLOR_A:
					{
						D3DXCOLOR vColor = pWorldLayer->GetColor();
						vColor.a = fVal;
						pWorldLayer->SetColor( vColor );
					}
					break;
				case PI_EW_WORLD_LAYER_TEX_U:
					{
						D3DXVECTOR2 vTexUV = pWorldLayer->GetTexUV();
						vTexUV.x = fVal;
						pWorldLayer->SetTexUV(vTexUV.x, vTexUV.y);						
					}
					break;
				case PI_EW_WORLD_LAYER_TEX_V:
					{
						D3DXVECTOR2 vTexUV = pWorldLayer->GetTexUV();
						vTexUV.y = fVal;
						pWorldLayer->SetTexUV(vTexUV.x, vTexUV.y);						
					}
					break;
				case PI_EW_WORLD_LAYER_FEEDBACK_U:
					{
						D3DXVECTOR2 vFeedBackUV = pWorldLayer->GetFeedBackUV();
						vFeedBackUV.x = fVal;
						pWorldLayer->SetFeedBackUV(vFeedBackUV.x, vFeedBackUV.y);
					}
					break;
				case PI_EW_WORLD_LAYER_FEEDBACK_V:
					{
						D3DXVECTOR2 vFeedBackUV = pWorldLayer->GetFeedBackUV();
						vFeedBackUV.y = fVal;
						pWorldLayer->SetFeedBackUV(vFeedBackUV.x, vFeedBackUV.y);
					}
					break;
				case PI_EW_WORLD_LAYER_SHOW:
					pWorldLayer->SetInitShow(bVal);
					break;
				default:
					break;
				}
			}
		}
	}
}

void CX2ToolMain::SetWorldBaseProperty(PROPERTY_ID ePI, float fVal)
{
	CX2World *pWorld = g_pX2Game->GetWorld();

	D3DXCOLOR vUnitColor = pWorld->GetLightColor();
	D3DXCOLOR vWorldColor = pWorld->GetOriginColor();
	D3DXVECTOR3 vLightPos = pWorld->GetLightPos();

	switch( ePI )
	{
	case PI_EW_WORLD_UNIT_COLOR_R:
		pWorld->SetLightColor_LUA( fVal, vUnitColor.g, vUnitColor.b );
		break;
	case PI_EW_WORLD_UNIT_COLOR_G:
		pWorld->SetLightColor_LUA( vUnitColor.r, fVal, vUnitColor.b );
		break;
	case PI_EW_WORLD_UNIT_COLOR_B:
		pWorld->SetLightColor_LUA( vUnitColor.r, vUnitColor.g, fVal );
		break;
	case PI_EW_WORLD_COLOR_R:
		pWorld->SetOriginColor_LUA( fVal, vWorldColor.g, vWorldColor.b );
		break;
	case PI_EW_WORLD_COLOR_G:
		pWorld->SetOriginColor_LUA( vWorldColor.r, fVal, vWorldColor.b );
		break;
	case PI_EW_WORLD_COLOR_B:
		pWorld->SetOriginColor_LUA( vWorldColor.r, vWorldColor.g, fVal );
		break;
	case PI_EW_WORLD_LIGHT_POS_X:
		pWorld->SetLightPos( fVal, vLightPos.y, vLightPos.z );
		break;
	case PI_EW_WORLD_LIGHT_POS_Y:
		pWorld->SetLightPos( vLightPos.x, fVal, vLightPos.z );
		break;
	case PI_EW_WORLD_LIGHT_POS_Z:
		pWorld->SetLightPos( vLightPos.x, vLightPos.y, fVal );
		break;
	}
}

void CX2ToolMain::SetEnableWorldFog(bool bVal)
{
	g_pX2Game->GetWorld()->m_bFog = bVal;
	g_pX2Game->GetWorld()->m_bFogShow = bVal;

	vector<CX2WorldObjectPtr> vecWorldObjectList = g_pX2Game->GetWorld()->GetWorldObjectList4Tool();
	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( pWorldObject != NULL && pWorldObject->IsObjectFogShow() == true )
		{
			switch( pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
			case CX2WorldObject::OT_SKIN_MESH:
				{
					CKTDGXRenderer::RenderParam* pRenderParam = pWorldObject->GetRenderParam();					
					pRenderParam->bFog = bVal;
				}
				break;
			default:
				break;
			}
		}
	}

	vector<CKTDGSkyDome*> vecSkyDome = g_pX2Game->GetWorld()->GetSkyDomeList();
	for(UINT i=0; i<vecSkyDome.size(); ++i)
	{
		CKTDGSkyDome *pSkyDome = vecSkyDome[i];
		if( pSkyDome != NULL && pSkyDome->IsObjectFogShow() == true )
		{
			CKTDGXRenderer::RenderParam* pRenderParam = pSkyDome->GetRenderParam();
			pRenderParam->bFog = bVal;
		}
	}
}

void CX2ToolMain::SetWorldFogProperty(PROPERTY_ID ePI, float fVal)
{
	CX2World *pWorld = g_pX2Game->GetWorld();

	switch(ePI)
	{
	case PI_EW_WORLD_FOG_NEAR_X:
		pWorld->m_fNearX = fVal;
		break;
	case PI_EW_WORLD_FOG_FAR_X:
		pWorld->m_fFarX = fVal;
		break;
	case PI_EW_WORLD_FOG_NEAR_Y:
		pWorld->m_fNearY = fVal;
		break;
	case PI_EW_WORLD_FOG_FAR_Y:
		pWorld->m_fFarY = fVal;
		break;
	case PI_EW_WORLD_FOG_COLOR_R:
		pWorld->m_FogColor.r = fVal;
		break;
	case PI_EW_WORLD_FOG_COLOR_G:
		pWorld->m_FogColor.g = fVal;
		break;
	case PI_EW_WORLD_FOG_COLOR_B:
		pWorld->m_FogColor.b = fVal;
		break;
	case PI_EW_WORLD_FOG_DENSITY:
		pWorld->m_fDensity = fVal;
		break;
	default:
		break;
	}

	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( pWorldObject != NULL && pWorldObject->m_bUseObjectFogProperty == false )
		{
			switch( pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
			case CX2WorldObject::OT_SKIN_MESH:
				{
					CKTDGXRenderer::RenderParam* pRenderParam = pWorldObject->GetRenderParam();
					switch(ePI)
					{
					case PI_EW_WORLD_FOG_NEAR_X:
						pRenderParam->fogNearX = fVal;
						break;
					case PI_EW_WORLD_FOG_FAR_X:
						pRenderParam->fogFarX = fVal;
						break;
					case PI_EW_WORLD_FOG_NEAR_Y:
						pRenderParam->fogNearY = fVal;
						break;
					case PI_EW_WORLD_FOG_FAR_Y:
						pRenderParam->fogFarY = fVal;
						break;
					case PI_EW_WORLD_FOG_COLOR_R:
						pRenderParam->fogColor.r = fVal;
						break;
					case PI_EW_WORLD_FOG_COLOR_G:
						pRenderParam->fogColor.g = fVal;
						break;
					case PI_EW_WORLD_FOG_COLOR_B:
						pRenderParam->fogColor.b = fVal;
						break;
					case PI_EW_WORLD_FOG_DENSITY:
						pRenderParam->fogDensity = fVal;
						break;
					default:
						break;
					}
				}
				break;
			default:
				break;
			}
		}
	}

	vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();
	for(UINT i=0; i<vecSkyDome.size(); ++i)
	{
		CKTDGSkyDome *pSkyDome = vecSkyDome[i];
		if( pSkyDome != NULL && pSkyDome->m_bUseObjectFogProperty == false )
		{
			CKTDGXRenderer::RenderParam* pRenderParam = pSkyDome->GetRenderParam();
			switch(ePI)
			{
			case PI_EW_WORLD_FOG_NEAR_X:
				pRenderParam->fogNearX = fVal;
				break;
			case PI_EW_WORLD_FOG_FAR_X:
				pRenderParam->fogFarX = fVal;
				break;
			case PI_EW_WORLD_FOG_NEAR_Y:
				pRenderParam->fogNearY = fVal;
				break;
			case PI_EW_WORLD_FOG_FAR_Y:
				pRenderParam->fogFarY = fVal;
				break;
			case PI_EW_WORLD_FOG_COLOR_R:
				pRenderParam->fogColor.r = fVal;
				break;
			case PI_EW_WORLD_FOG_COLOR_G:
				pRenderParam->fogColor.g = fVal;
				break;
			case PI_EW_WORLD_FOG_COLOR_B:
				pRenderParam->fogColor.b = fVal;
				break;
			case PI_EW_WORLD_FOG_DENSITY:
				pRenderParam->fogDensity = fVal;
				break;
			default:
				break;
			}
		}
	}
}

int CX2ToolMain::SelectSkydome()
{
	int iCount = 0;
 	if( m_bEditWorld == true )
 	{
 		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
 		CX2World *pWorld = g_pX2Game->GetWorld();
 		vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();
 
 		for(UINT i=0; i<vecSkyDome.size(); ++i)
 		{									
 			UINT uItemState = pMain->m_DlgWorldEditor.m_ctrlSkyDomList.GetItemState ( i, LVIS_SELECTED );
 
 			CKTDGSkyDome *pSkyDome = vecSkyDome[i];
 			if( pSkyDome != NULL )
 			{
 				if( uItemState == LVIS_SELECTED )
 				{
 					pSkyDome->m_bSelected = true;
 					++iCount;
 				}
 				else
 				{
 					pSkyDome->m_bSelected = false;
 				}
 			}			
 		}
 	}

	return iCount;
}

void CX2ToolMain::SetSkyDomeProperty(PROPERTY_ID ePI, int iVal)
{
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();

	for(UINT i=0; i<vecSkyDome.size(); ++i)
	{									
		CKTDGSkyDome *pSkyDome = vecSkyDome[i];
		if( pSkyDome != NULL && pSkyDome->m_bSelected == true )
		{
			switch(ePI)
			{
			case PI_EW_WORLD_SKYDOME_LAYER:
				pSkyDome->SetLayer(iVal);
				break;
			case PI_EW_WORLD_SKYDOME_RENDERTYPE:
				{
					pSkyDome->GetRenderParam()->renderType = (CKTDGXRenderer::RENDER_TYPE)iVal;
					pSkyDome->GetOriRenderParam()->renderType = (CKTDGXRenderer::RENDER_TYPE)iVal;
					pSkyDome->GetRenderParam()->fOutLineWide = 1.5f;
					pSkyDome->GetOriRenderParam()->fOutLineWide = 1.5f;
					pSkyDome->GetRenderParam()->outLineColor = D3DXCOLOR( 1.f, 0.f, 0.f, 1.f);
					pSkyDome->GetOriRenderParam()->outLineColor = D3DXCOLOR( 1.f, 0.f, 0.f, 1.f);
				}
				break;
			case PI_EW_WORLD_SKYDOME_SRCBLEND:
				pSkyDome->GetRenderParam()->srcBlend = (D3DBLEND)iVal;
				pSkyDome->GetOriRenderParam()->srcBlend = (D3DBLEND)iVal;
				break;
			case PI_EW_WORLD_SKYDOME_DESTBLEND:
				pSkyDome->GetRenderParam()->destBlend = (D3DBLEND)iVal;
				pSkyDome->GetOriRenderParam()->destBlend = (D3DBLEND)iVal;
				break;
			default:
				break;
			}
		}
	}
	
}
void CX2ToolMain::SetSkyDomeProperty(PROPERTY_ID ePI, bool bVal)
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();

	for(UINT i=0; i<vecSkyDome.size(); ++i)
	{									
		CKTDGSkyDome *pSkyDome = vecSkyDome[i];
		if( pSkyDome != NULL && pSkyDome->m_bSelected == true )
		{
			switch(ePI)
			{
			case PI_EW_WORLD_SKYDOME_SHOWOBJECT:
				pSkyDome->m_bShowObject = bVal;
				if( pSkyDome->m_bShowObject == true )
				{
					pSkyDome->SetShowObject(true, true);
				}
				else
				{
					pSkyDome->SetShowObject(false, true);
				}
				break;
			case PI_EW_RORLD_SKYDOME_VIEW_TRACE:
				pSkyDome->SetViewTrace( bVal );
				break;
			case PI_EW_WORLD_SKYDOME_ALPHAOBJECT:
				pSkyDome->SetAlphaObject(bVal);
				pSkyDome->GetRenderParam()->bAlphaBlend = bVal;
				pSkyDome->GetOriRenderParam()->bAlphaBlend = bVal;
				break;
			case PI_EW_WORLD_SKYDOME_USE_WORLDCOLOR:
				pSkyDome->SetUseWorldColor(bVal);
				break;
			case PI_EW_WORLD_SKYDOME_FOG:
				pSkyDome->SetFogShow(bVal);
				break;
			case PI_EW_WORLD_SKYDOME_FOG_USEOBJECT:
				{
					pSkyDome->m_bUseObjectFogProperty = bVal;
					if( bVal == false )
					{
						pSkyDome->GetRenderParam()->fogColor = g_pX2Game->GetWorld()->m_FogColor;
						pSkyDome->GetOriRenderParam()->fogColor = g_pX2Game->GetWorld()->m_FogColor;

						pSkyDome->GetRenderParam()->fogNearX = g_pX2Game->GetWorld()->m_fNearX;
						pSkyDome->GetRenderParam()->fogFarX = g_pX2Game->GetWorld()->m_fFarX;
						pSkyDome->GetRenderParam()->fogNearY = g_pX2Game->GetWorld()->m_fNearY;
						pSkyDome->GetRenderParam()->fogFarY = g_pX2Game->GetWorld()->m_fFarY;
						pSkyDome->GetOriRenderParam()->fogNearX = g_pX2Game->GetWorld()->m_fNearX;
						pSkyDome->GetOriRenderParam()->fogFarX = g_pX2Game->GetWorld()->m_fFarX;
						pSkyDome->GetOriRenderParam()->fogNearY = g_pX2Game->GetWorld()->m_fNearY;
						pSkyDome->GetOriRenderParam()->fogFarY = g_pX2Game->GetWorld()->m_fFarY;

						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_COLOR_R, (_variant_t)(float)pSkyDome->GetRenderParam()->fogColor.r);
						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_COLOR_G, (_variant_t)(float)pSkyDome->GetRenderParam()->fogColor.g);
						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_COLOR_B, (_variant_t)(float)pSkyDome->GetRenderParam()->fogColor.b);

						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_NEAR_X, (_variant_t)(float)pSkyDome->GetRenderParam()->fogNearX);
						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_FAR_X, (_variant_t)(float)pSkyDome->GetRenderParam()->fogFarX);
						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_NEAR_Y, (_variant_t)(float)pSkyDome->GetRenderParam()->fogNearY);
						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_FAR_Y, (_variant_t)(float)pSkyDome->GetRenderParam()->fogFarY);
					}
				}
				break;
			default:
				break;
			}
		}
	}	
}
void CX2ToolMain::SetSkyDomeProperty(PROPERTY_ID ePI, float fVal)
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();

	for(UINT i=0; i<vecSkyDome.size(); ++i)
	{									
		CKTDGSkyDome *pSkyDome = vecSkyDome[i];
		if( pSkyDome != NULL && pSkyDome->m_bSelected == true )
		{
			switch(ePI)
			{
			case PI_EW_WORLD_SKYDOME_POSITION_X:
				{
					D3DXVECTOR3 vPos = pSkyDome->GetMatrix().GetPos();
					pSkyDome->GetMatrix().Move( fVal, vPos.y, vPos.z );
				}
				break;
			case PI_EW_WORLD_SKYDOME_POSITION_Y:
				{
					D3DXVECTOR3 vPos = pSkyDome->GetMatrix().GetPos();
					pSkyDome->GetMatrix().Move( vPos.x, fVal, vPos.z );
				}
				break;
			case PI_EW_WORLD_SKYDOME_POSITION_Z:
				{
					D3DXVECTOR3 vPos = pSkyDome->GetMatrix().GetPos();
					pSkyDome->GetMatrix().Move( vPos.x, fVal, vPos.z );
				}
				break;
			case PI_EW_WORLD_SKYDOME_ROTATION_SPEED:
				pSkyDome->SetRotationSpeed(fVal);
				break;
			case PI_EW_WORLD_SKYDOME_TEXUV_SPEED_U:
				{
					D3DXVECTOR2 vTexUV = pSkyDome->GetTex0UVSpeed();
					pSkyDome->SetTex0UVSpeed( fVal, vTexUV.y );
				}
				break;
			case PI_EW_WORLD_SKYDOME_TEXUV_SPEED_V:
				{
					D3DXVECTOR2 vTexUV = pSkyDome->GetTex0UVSpeed();
					pSkyDome->SetTex0UVSpeed( vTexUV.x, fVal );
				}
				break;
			case PI_EW_WORLD_SKYDOME_COLOR_R:
				{
					D3DXCOLOR vColor = pSkyDome->GetSkyDomeColor();
					vColor.r = fVal;
					pSkyDome->SetSkyDomeColor( vColor );
				}
				break;
			case PI_EW_WORLD_SKYDOME_COLOR_G:
				{
					D3DXCOLOR vColor = pSkyDome->GetSkyDomeColor();
					vColor.g = fVal;
					pSkyDome->SetSkyDomeColor( vColor );
				}
				break;
			case PI_EW_WORLD_SKYDOME_COLOR_B:
				{
					D3DXCOLOR vColor = pSkyDome->GetSkyDomeColor();
					vColor.b = fVal;
					pSkyDome->SetSkyDomeColor( vColor );
				}
				break;
			case PI_EW_WORLD_SKYDOME_COLOR_A:
				{
					D3DXCOLOR vColor = pSkyDome->GetSkyDomeColor();
					vColor.a = fVal;
					pSkyDome->SetSkyDomeColor( vColor );
				}
				break;
			case PI_EW_WORLD_SKYDOME_FOG_COLOR_R:
				pSkyDome->m_bUseObjectFogProperty = true;
				pSkyDome->GetRenderParam()->fogColor.r = fVal;
				pSkyDome->GetOriRenderParam()->fogColor.r = fVal;
				pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_USEOBJECT, (_variant_t)(bool)pSkyDome->m_bUseObjectFogProperty);
				break;
			case PI_EW_WORLD_SKYDOME_FOG_COLOR_G:
				pSkyDome->m_bUseObjectFogProperty = true;
				pSkyDome->GetRenderParam()->fogColor.g = fVal;
				pSkyDome->GetOriRenderParam()->fogColor.g = fVal;
				pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_USEOBJECT, (_variant_t)(bool)pSkyDome->m_bUseObjectFogProperty);
				break;
			case PI_EW_WORLD_SKYDOME_FOG_COLOR_B:
				pSkyDome->m_bUseObjectFogProperty = true;
				pSkyDome->GetRenderParam()->fogColor.b = fVal;
				pSkyDome->GetOriRenderParam()->fogColor.b = fVal;
				pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_USEOBJECT, (_variant_t)(bool)pSkyDome->m_bUseObjectFogProperty);
				break;
			case PI_EW_WORLD_SKYDOME_FOG_NEAR_X:
				pSkyDome->m_bUseObjectFogProperty = true;
				pSkyDome->GetRenderParam()->fogNearX = fVal;
				pSkyDome->GetOriRenderParam()->fogNearX = fVal;
				pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_USEOBJECT, (_variant_t)(bool)pSkyDome->m_bUseObjectFogProperty);
				break;
			case PI_EW_WORLD_SKYDOME_FOG_FAR_X:
				pSkyDome->m_bUseObjectFogProperty = true;
				pSkyDome->GetRenderParam()->fogFarX = fVal;
				pSkyDome->GetOriRenderParam()->fogFarX = fVal;
				pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_USEOBJECT, (_variant_t)(bool)pSkyDome->m_bUseObjectFogProperty);
				break;
			case PI_EW_WORLD_SKYDOME_FOG_NEAR_Y:
				pSkyDome->m_bUseObjectFogProperty = true;
				pSkyDome->GetRenderParam()->fogNearY = fVal;
				pSkyDome->GetOriRenderParam()->fogNearY = fVal;
				pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_USEOBJECT, (_variant_t)(bool)pSkyDome->m_bUseObjectFogProperty);
				break;
			case PI_EW_WORLD_SKYDOME_FOG_FAR_Y:
				pSkyDome->m_bUseObjectFogProperty = true;
				pSkyDome->GetRenderParam()->fogFarY = fVal;
				pSkyDome->GetOriRenderParam()->fogFarY = fVal;
				pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_USEOBJECT, (_variant_t)(bool)pSkyDome->m_bUseObjectFogProperty);
				break;
			case PI_EW_WORLD_SKYDOME_FOG_DENSITY:
				pSkyDome->GetRenderParam()->fogDensity = fVal;
				pSkyDome->GetOriRenderParam()->fogDensity = fVal;
				pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_SKYDOME_FOG_USEOBJECT, (_variant_t)(bool)pSkyDome->m_bUseObjectFogProperty);
				break;
			default:
				break;
			}
		}
	}
}

int CX2ToolMain::SelectWorldObject(int iSelectedInx, bool bCheck)
{
	int iCount = 0;
	if( m_bEditWorld == true )
	{
		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
		CX2World *pWorld = g_pX2Game->GetWorld();
		vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

		CX2WorldObject *pWorldObject = vecWorldObjectList[iSelectedInx].get();
		if( pWorldObject != NULL )
		{
			pWorldObject->m_bSelected = bCheck;
		}

// 		for(UINT i=0; i<vecWorldObjectList.size(); ++i)
// 		{
// 			UINT uItemState = pMain->m_DlgWorldEditor.m_ctrlListWorldObject.GetItemState ( i, LVIS_SELECTED );
// 
// 			CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
// 			if( pWorldObject != NULL )
// 			{
// 				if( uItemState == LVIS_SELECTED )
// 				{
// 					pWorldObject->m_bSelected = true;
// 					++iCount;
// 				}
// 				else
// 				{
// 					pWorldObject->m_bSelected = false;
// 				}
// 			}
// 		}
	}
		
	return iCount;
}

void CX2ToolMain::SetWorldObjectProperty(PROPERTY_ID ePI, int iVal, CX2WorldObject *pChangeObject)
{
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( ( pChangeObject == NULL && pWorldObject != NULL && pWorldObject->m_bSelected == true ) ||
			( pChangeObject != NULL && pChangeObject == pWorldObject ) )
		{
			switch(ePI)
			{
			case PI_EW_WORLD_OBJECT_LAYER:
				pWorldObject->SetLayer(iVal);
				break;
			case PI_EW_WORLD_OBJECT_BILLBOARD:
				{					
					if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
					{
						CX2WorldObjectMesh *pObject = (CX2WorldObjectMesh*)pWorldObject;
						pObject->SetBillboardType( iVal );
					}
					else if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
					{
						CX2WorldObjectSkinMesh *pObject = (CX2WorldObjectSkinMesh*)pWorldObject;
						pObject->SetBillboardType( iVal );
					}
				}
				break;
			case PI_EW_WORLD_OBJECT_RENDERTYPE:
				{
					pWorldObject->GetRenderParam()->renderType = (CKTDGXRenderer::RENDER_TYPE)iVal;
					pWorldObject->GetOriRenderParam()->renderType = (CKTDGXRenderer::RENDER_TYPE)iVal;
					pWorldObject->GetRenderParam()->fOutLineWide = 1.5f;
					pWorldObject->GetOriRenderParam()->fOutLineWide = 1.5f;
					pWorldObject->GetRenderParam()->outLineColor = D3DXCOLOR( 1.f, 0.f, 0.f, 1.f);
					pWorldObject->GetOriRenderParam()->outLineColor = D3DXCOLOR( 1.f, 0.f, 0.f, 1.f);
				}
				break;
			case PI_EW_WORLD_OBJECT_SRCBLEND:
				pWorldObject->GetRenderParam()->srcBlend = (D3DBLEND)iVal;
				pWorldObject->GetOriRenderParam()->srcBlend = (D3DBLEND)iVal;
				break;
			case PI_EW_WORLD_OBJECT_DESTBLEND:
				pWorldObject->GetRenderParam()->destBlend = (D3DBLEND)iVal;
				pWorldObject->GetOriRenderParam()->destBlend = (D3DBLEND)iVal;
				break;
			case PI_EW_WORLD_OBJECT_ANIM_TYPE:
				if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
				{
					CX2WorldObjectSkinMesh *pObject = (CX2WorldObjectSkinMesh*)pWorldObject;
					pObject->SetPlayType(iVal);
					pObject->m_iOriPlayType = iVal;

					pObject->PlayAnim_LUA( (CX2WorldObjectSkinMesh::WORLD_OBJECT_SKIN_MESH_PLAY_TYPE)iVal );
				}
				break;
			case PI_EW_WORLD_PARTICLE_PARTICLE_TYPE:
				{
					CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
					pObject->m_bChangeType = true;
					pObject->SetParticleType( (CKTDGParticleSystem::PARTICLE_TYPE)iVal );
				}
				break;
			case PI_EW_WORLD_PARTICLE_TRIGGERCOUNT:
				{
					CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
					pObject->SetTriggerCount( iVal );
				}
				break;
			case PI_EW_WORLD_PARTICLE_DRAWCOUNT:
				{
					CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
					pObject->SetDrawCount( iVal );
				}
				break;

			default:
				break;
			}
		}
	}
	
}
void CX2ToolMain::SetWorldObjectProperty(PROPERTY_ID ePI, bool bVal, CX2WorldObject *pChangeObject)
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( ( pChangeObject == NULL && pWorldObject != NULL && pWorldObject->m_bSelected == true ) ||
			( pChangeObject != NULL && pWorldObject == pChangeObject ) )
		{
			switch(ePI)
			{
			case PI_EW_WORLD_OBJECT_SHOWOBJECT:
				pWorldObject->m_bShowObject = bVal;
				if( pWorldObject->m_bShowObject == true )
					pWorldObject->SetShowObject(true);
				else
					pWorldObject->SetShowObject(false);
				break;
			case PI_EW_WORLD_OBJECT_FREEZING:
				pWorldObject->m_bFreezing = bVal;
				break;
			case PI_EW_WORLD_OBJECT_USE_WORLDCOLOR:
				pWorldObject->SetUseWorldColor(bVal);
				break;
			case PI_EW_WORLD_OBJECT_FORCESHOW:
				pWorldObject->SetForceShow(bVal);
				break;
			case PI_EW_WORLD_OBJECT_CANHIDE:
				{
					if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
					{
						CX2WorldObjectMesh *pObject = (CX2WorldObjectMesh*)pWorldObject;
						pObject->SetCanHide(bVal, bVal);
					}
					else if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
					{
						CX2WorldObjectSkinMesh *pObject = (CX2WorldObjectSkinMesh*)pWorldObject;
						pObject->SetCanHide(bVal, bVal);
					}

					if( bVal == true )
					{
						pWorldObject->SetAlphaObject(true);
						pWorldObject->GetRenderParam()->bAlphaBlend = true;
						pWorldObject->GetOriRenderParam()->bAlphaBlend = true;
						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_ALPHAOBJECT, (_variant_t)(bool)true);
					}
				}
				break;
			case PI_EW_WORLD_OBJECT_NONCULL:
				{
					if( bVal == true )
					{
						pWorldObject->GetRenderParam()->cullMode = D3DCULL_NONE;
						pWorldObject->GetOriRenderParam()->cullMode = D3DCULL_NONE;
					}
					else
					{
						pWorldObject->GetRenderParam()->cullMode = D3DCULL_CCW;
						pWorldObject->GetOriRenderParam()->cullMode = D3DCULL_CCW;
					}
				}
				break;
			case PI_EW_WORLD_OBJECT_ALPHATEST:
				pWorldObject->GetRenderParam()->bAlphaTestEnable = bVal;
				pWorldObject->GetOriRenderParam()->bAlphaTestEnable = bVal;
				break;
			case PI_EW_WORLD_OBJECT_ALPHAOBJECT:
				{
					pWorldObject->SetAlphaObject(true);
					pWorldObject->GetRenderParam()->bAlphaBlend = true;
					pWorldObject->GetOriRenderParam()->bAlphaBlend = true;
				}
				break;
			case PI_EW_WORLD_OBJECT_FOG:
				{
					if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
					{
						CX2WorldObjectMesh *pObject = (CX2WorldObjectMesh*)pWorldObject;
						pObject->SetFogShow(bVal);
					}
					else if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
					{
						CX2WorldObjectSkinMesh *pObject = (CX2WorldObjectSkinMesh*)pWorldObject;
						pObject->SetFogShow(bVal);
					}
				}
				break;
			case PI_EW_WORLD_OBJECT_FOG_USEOBJECT:
				{
					pWorldObject->m_bUseObjectFogProperty = bVal;
					if( bVal == false )
					{
						pWorldObject->GetRenderParam()->fogColor = g_pX2Game->GetWorld()->m_FogColor;
						pWorldObject->GetOriRenderParam()->fogColor = g_pX2Game->GetWorld()->m_FogColor;

						pWorldObject->GetRenderParam()->fogNearX = g_pX2Game->GetWorld()->m_fNearX;
						pWorldObject->GetRenderParam()->fogFarX = g_pX2Game->GetWorld()->m_fFarX;
						pWorldObject->GetRenderParam()->fogNearY = g_pX2Game->GetWorld()->m_fNearY;
						pWorldObject->GetRenderParam()->fogFarY = g_pX2Game->GetWorld()->m_fFarY;
						pWorldObject->GetOriRenderParam()->fogNearX = g_pX2Game->GetWorld()->m_fNearX;
						pWorldObject->GetOriRenderParam()->fogFarX = g_pX2Game->GetWorld()->m_fFarX;
						pWorldObject->GetOriRenderParam()->fogNearY = g_pX2Game->GetWorld()->m_fNearY;
						pWorldObject->GetOriRenderParam()->fogFarY = g_pX2Game->GetWorld()->m_fFarY;


						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_COLOR_R, (_variant_t)(float)pWorldObject->GetRenderParam()->fogColor.r);
						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_COLOR_G, (_variant_t)(float)pWorldObject->GetRenderParam()->fogColor.g);
						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_COLOR_B, (_variant_t)(float)pWorldObject->GetRenderParam()->fogColor.b);

						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_NEAR_X, (_variant_t)(float)pWorldObject->GetRenderParam()->fogNearX);
						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_FAR_X, (_variant_t)(float)pWorldObject->GetRenderParam()->fogFarX);
						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_NEAR_Y, (_variant_t)(float)pWorldObject->GetRenderParam()->fogNearY);
						pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_FAR_Y, (_variant_t)(float)pWorldObject->GetRenderParam()->fogFarY);
					}
				}
				break;
			case PI_EW_WORLD_PARTICLE_CULLCHECK:
				{
					CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
					pObject->SetCullCheck( bVal );
				}
				break;
			default:
				break;
			}
		}
	}	
}
void CX2ToolMain::SetWorldObjectProperty(PROPERTY_ID ePI, float fVal, CX2WorldObject *pChangeObject)
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	
	tWorldElemGroup tUnDoGroup;

	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( ( pChangeObject == NULL && pWorldObject != NULL && pWorldObject->m_bSelected == true ) ||
			( pChangeObject != NULL && pChangeObject == pWorldObject ) )
		{
			tWorldElem tUndoElem;
			tUndoElem.m_iEid = ePI;
			tUndoElem.m_eFlag = tWorldElem::UF_FLOAT;
			tUndoElem.m_pWorldObject = pWorldObject;

			switch(ePI)
			{
			case PI_EW_WORLD_OBJECT_POSITION_X:
				{
					D3DXVECTOR3 vPos = pWorldObject->GetMatrix().GetPos();
					tUndoElem.m_fValue = vPos.x;
					tUnDoGroup.m_vecUndoGroup.push_back(tUndoElem);

					pWorldObject->GetMatrix().Move( fVal, vPos.y, vPos.z );
					if( pWorldObject->GetObjectType() == CX2WorldObject::OT_PARTICLE )
					{
						CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
						D3DXVECTOR3 vParticlePos = D3DXVECTOR3(fVal, vPos.y, vPos.z);
						pObject->SetParticlePos(vParticlePos);
					}
				}
				break;
			case PI_EW_WORLD_OBJECT_POSITION_Y:
				{
					D3DXVECTOR3 vPos = pWorldObject->GetMatrix().GetPos();
					tUndoElem.m_fValue = vPos.y;
					tUnDoGroup.m_vecUndoGroup.push_back(tUndoElem);

					pWorldObject->GetMatrix().Move( vPos.x, fVal, vPos.z );
					if( pWorldObject->GetObjectType() == CX2WorldObject::OT_PARTICLE )
					{
						CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
						D3DXVECTOR3 vParticlePos = D3DXVECTOR3(vPos.x, fVal, vPos.z);
						pObject->SetParticlePos(vParticlePos);
					}
				}
				break;
			case PI_EW_WORLD_OBJECT_POSITION_Z:
				{
					D3DXVECTOR3 vPos = pWorldObject->GetMatrix().GetPos();
					tUndoElem.m_fValue = vPos.z;
					tUnDoGroup.m_vecUndoGroup.push_back(tUndoElem);

					pWorldObject->GetMatrix().Move( vPos.x, vPos.y, fVal );
					if( pWorldObject->GetObjectType() == CX2WorldObject::OT_PARTICLE )
					{
						CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
						D3DXVECTOR3 vParticlePos = D3DXVECTOR3(vPos.x, vPos.y, fVal);
						pObject->SetParticlePos(vParticlePos);
					}
				}
				break;
			case PI_EW_WORLD_OBJECT_ROTATION_X:
				{
					D3DXVECTOR3 vRot = pWorldObject->GetMatrix().GetRotateDegree();
					tUndoElem.m_fValue = vRot.x;
					tUnDoGroup.m_vecUndoGroup.push_back(tUndoElem);

					pWorldObject->GetMatrix().RotateDegree(fVal, vRot.y, vRot.z);
				}
				break;
			case PI_EW_WORLD_OBJECT_ROTATION_Y:
				{
					D3DXVECTOR3 vRot = pWorldObject->GetMatrix().GetRotateDegree();
					tUndoElem.m_fValue = vRot.y;
					tUnDoGroup.m_vecUndoGroup.push_back(tUndoElem);

					pWorldObject->GetMatrix().RotateDegree(vRot.x, fVal, vRot.z);
				}
				break;
			case PI_EW_WORLD_OBJECT_ROTATION_Z:
				{
					D3DXVECTOR3 vRot = pWorldObject->GetMatrix().GetRotateDegree();
					tUndoElem.m_fValue = vRot.z;
					tUnDoGroup.m_vecUndoGroup.push_back(tUndoElem);

					pWorldObject->GetMatrix().RotateDegree(vRot.x, vRot.y, fVal);
				}
				break;
			case PI_EW_WORLD_OBJECT_SCALE_X:
				{
					D3DXVECTOR3 vScale = pWorldObject->GetMatrix().GetScale();
					tUndoElem.m_fValue = vScale.x;
					tUnDoGroup.m_vecUndoGroup.push_back(tUndoElem);

					pWorldObject->GetMatrix().Scale(fVal, vScale.y, vScale.z);
				}
				break;
			case PI_EW_WORLD_OBJECT_SCALE_Y:
				{
					D3DXVECTOR3 vScale = pWorldObject->GetMatrix().GetScale();
					tUndoElem.m_fValue = vScale.y;
					tUnDoGroup.m_vecUndoGroup.push_back(tUndoElem);

					pWorldObject->GetMatrix().Scale(vScale.x, fVal, vScale.z);
				}
				break;
			case PI_EW_WORLD_OBJECT_SCALE_Z:
				{
					D3DXVECTOR3 vScale = pWorldObject->GetMatrix().GetScale();
					tUndoElem.m_fValue = vScale.z;
					tUnDoGroup.m_vecUndoGroup.push_back(tUndoElem);

					pWorldObject->GetMatrix().Scale(vScale.x, vScale.y, fVal);
				}
				break;
			case PI_EW_WORLD_OBJECT_TEXUV_SPEED_U:
				{
					if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
					{
						CX2WorldObjectMesh *pObject = (CX2WorldObjectMesh*)pWorldObject;
						D3DXVECTOR2 vTexUV = pObject->GetTex0UVSpeed();
						pObject->SetTex0UVSpeed(fVal, vTexUV.y);
					}
					else if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
					{
						CX2WorldObjectSkinMesh *pObject = (CX2WorldObjectSkinMesh*)pWorldObject;
						D3DXVECTOR2 vTexUV = pObject->GetTex0UVSpeed();
						pObject->SetTex0UVSpeed(fVal, vTexUV.y);
					}
				}
				break;
			case PI_EW_WORLD_OBJECT_TEXUV_SPEED_V:
				{
					if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
					{
						CX2WorldObjectMesh *pObject = (CX2WorldObjectMesh*)pWorldObject;
						D3DXVECTOR2 vTexUV = pObject->GetTex0UVSpeed();
						pObject->SetTex0UVSpeed(vTexUV.x, fVal);
					}
					else if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
					{
						CX2WorldObjectSkinMesh *pObject = (CX2WorldObjectSkinMesh*)pWorldObject;
						D3DXVECTOR2 vTexUV = pObject->GetTex0UVSpeed();
						pObject->SetTex0UVSpeed(vTexUV.x, fVal);
					}
				}
				break;
			case PI_EW_WORLD_OBJECT_COLOR_R:
				{
					D3DXCOLOR vColor = pWorldObject->GetOriginalColorFactor();
					vColor.r = fVal;
					pWorldObject->SetOriginalColorFactor(vColor);
				}
				break;
			case PI_EW_WORLD_OBJECT_COLOR_G:
				{
					D3DXCOLOR vColor = pWorldObject->GetOriginalColorFactor();
					vColor.g = fVal;
					pWorldObject->SetOriginalColorFactor(vColor);
				}
				break;
			case PI_EW_WORLD_OBJECT_COLOR_B:
				{
					D3DXCOLOR vColor = pWorldObject->GetOriginalColorFactor();
					vColor.b = fVal;
					pWorldObject->SetOriginalColorFactor(vColor);
				}
				break;
			case PI_EW_WORLD_OBJECT_COLOR_A:
				{
					D3DXCOLOR vColor = pWorldObject->GetOriginalColorFactor();
					vColor.a = fVal;
					pWorldObject->SetOriginalColorFactor(vColor);
				}
				break;
			case PI_EW_WORLD_OBJECT_HIDE_FAR_DISTANCE:
				pWorldObject->SetHideDistance(fVal);
				break;
			case PI_EW_WORLD_OBJECT_HIDE_NEAR_DISTANCE:
				pWorldObject->SetHideNearDistance(fVal);
				break;
			case PI_EW_WORLD_OBJECT_FOG_COLOR_R:
				{
					pWorldObject->m_bUseObjectFogProperty = true;
					pWorldObject->GetRenderParam()->fogColor.r = fVal;
					pWorldObject->GetOriRenderParam()->fogColor.r = fVal;
					pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_USEOBJECT, (_variant_t)(bool)pWorldObject->m_bUseObjectFogProperty);
				}
				break;
			case PI_EW_WORLD_OBJECT_FOG_COLOR_G:
				{
					pWorldObject->m_bUseObjectFogProperty = true;
					pWorldObject->GetRenderParam()->fogColor.g = fVal;
					pWorldObject->GetOriRenderParam()->fogColor.g = fVal;
					pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_USEOBJECT, (_variant_t)(bool)pWorldObject->m_bUseObjectFogProperty);
				}
				break;
			case PI_EW_WORLD_OBJECT_FOG_COLOR_B:
				{
					pWorldObject->m_bUseObjectFogProperty = true;
					pWorldObject->GetRenderParam()->fogColor.b = fVal;
					pWorldObject->GetOriRenderParam()->fogColor.b = fVal;
					pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_USEOBJECT, (_variant_t)(bool)pWorldObject->m_bUseObjectFogProperty);
				}
				break;
			case PI_EW_WORLD_OBJECT_FOG_NEAR_X:
				{
					pWorldObject->m_bUseObjectFogProperty = true;
					pWorldObject->GetRenderParam()->fogNearX = fVal;
					pWorldObject->GetOriRenderParam()->fogNearX = fVal;
					pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_USEOBJECT, (_variant_t)(bool)pWorldObject->m_bUseObjectFogProperty);
				}
				break;
			case PI_EW_WORLD_OBJECT_FOG_FAR_X:
				{
					pWorldObject->m_bUseObjectFogProperty = true;
					pWorldObject->GetRenderParam()->fogFarX = fVal;
					pWorldObject->GetOriRenderParam()->fogFarX = fVal;
					pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_USEOBJECT, (_variant_t)(bool)pWorldObject->m_bUseObjectFogProperty);
				}
				break;
			case PI_EW_WORLD_OBJECT_FOG_NEAR_Y:
				{
					pWorldObject->m_bUseObjectFogProperty = true;
					pWorldObject->GetRenderParam()->fogNearY = fVal;
					pWorldObject->GetOriRenderParam()->fogNearY = fVal;
					pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_USEOBJECT, (_variant_t)(bool)pWorldObject->m_bUseObjectFogProperty);
				}
				break;
			case PI_EW_WORLD_OBJECT_FOG_FAR_Y:
				{
					pWorldObject->m_bUseObjectFogProperty = true;
					pWorldObject->GetRenderParam()->fogFarY = fVal;
					pWorldObject->GetOriRenderParam()->fogFarY = fVal;
					pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_USEOBJECT, (_variant_t)(bool)pWorldObject->m_bUseObjectFogProperty);
				}
				break;
			case PI_EW_WORLD_OBJECT_FOG_DENSITY:
				{
					pWorldObject->m_bUseObjectFogProperty = true;
					pWorldObject->GetRenderParam()->fogDensity = fVal;
					pWorldObject->GetOriRenderParam()->fogDensity = fVal;
					pMain->m_wndProperties.SetPropValue(PI_EW_WORLD_OBJECT_FOG_USEOBJECT, (_variant_t)(bool)pWorldObject->m_bUseObjectFogProperty);
				}
				break;
			case PI_EW_WORLD_PARTICLE_DELAYTIME:
				{
					CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
					pObject->SetDelayTime( fVal );
				}
				break;
			case PI_EW_WORLD_PARTICLE_TRIGGERITME:
				{
					CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
					pObject->SetTriggerTime( fVal );
				}
				break;
			case PI_EW_WORLD_PARTICLE_EMITMIN:
				{
					CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
					pObject->SetEmitMinMax( fVal, pObject->m_fEmitMax );
				}
				break;
			case PI_EW_WORLD_PARTICLE_EMITMAX:
				{
					CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
					pObject->SetEmitMinMax( pObject->m_fEmitMin, fVal );
				}
				break;
			case PI_EW_WORLD_PARTICLE_LATENCY:
				{
					CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
					pObject->SetLatency( fVal );
				}
				break;
			default:
				break;
			}
		}
	}

	if( pChangeObject == NULL && tUnDoGroup.m_vecUndoGroup.size() >= 1 )
	{
		AddUndoWorld( tUnDoGroup );
	}
}
void CX2ToolMain::SetWorldObjectProperty(PROPERTY_ID ePI, wstring wstrVal, CX2WorldObject *pChangeObject)
{
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( ( pChangeObject == NULL && pWorldObject != NULL && pWorldObject->m_bSelected == true ) ||
			( pChangeObject != NULL && pChangeObject == pWorldObject ) )
		{
			switch(ePI)
			{
			case PI_EW_WORLD_OBJECT_ANIM_NAME:
				{
					if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
					{
						CX2WorldObjectSkinMesh *pObject = (CX2WorldObjectSkinMesh*)pWorldObject;						
						pObject->m_wstrOriAnimName = wstrVal;
												
						vector<wstring>& wstrPlayList = pObject->GetPlayList();
						wstrPlayList.clear();
						wstrPlayList.push_back( wstrVal );
						pObject->PlayAnim_LUA( (CX2WorldObjectSkinMesh::WORLD_OBJECT_SKIN_MESH_PLAY_TYPE)pObject->m_iOriPlayType );
					}
				}
				break;
			default:
				break;
			}
		}
	}
	
}

void CX2ToolMain::SetMoveWorldObject(bool bVal)
{
	m_bMoveWorld = false;
	m_bPickMoveWorldObjectPivot = false;
	m_bMoveAxis = false;
	m_bMoveAxisX = false;
	m_bMoveAxisY = false;
	m_bMoveAxisZ = false;
	m_bShowMoveAxis = false;

	m_vMoveWorldObjectPivot = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vMoveWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vRotationWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vScaleWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_bMoveWorldObject = bVal;
}
void CX2ToolMain::SetRotationWorldObject(bool bVal)
{
	m_bMoveWorld = false;
	m_bPickMoveWorldObjectPivot = false;
	m_bMoveAxis = false;
	m_bMoveAxisX = false;
	m_bMoveAxisY = false;
	m_bMoveAxisZ = false;
	m_bShowMoveAxis = false;

	m_vMoveWorldObjectPivot = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vMoveWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vRotationWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vScaleWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_bRotationWorldObject = bVal;
}
void CX2ToolMain::SetScaleWorldObject(bool bVal)
{
	m_bMoveWorld = false;
	m_bPickMoveWorldObjectPivot = false;
	m_bMoveAxis = false;
	m_bMoveAxisX = false;
	m_bMoveAxisY = false;
	m_bMoveAxisZ = false;
	m_bShowMoveAxis = false;

	m_vMoveWorldObjectPivot = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vMoveWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vRotationWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vScaleWorldObject = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_bScaleWorldObject = bVal;
}

void CX2ToolMain::MoveWorldObject(D3DXVECTOR3 vAfterGap)
{
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();

		if( pWorldObject != NULL && pWorldObject->m_bSelected == true && pWorldObject->m_bFreezing == false )
		{									
			D3DXVECTOR3 vPos = pWorldObject->GetPos();

			if( m_bMoveAxisX == true && m_bMoveAxisY == true )
			{										
				vPos.x += vAfterGap.x;
				vPos.y += vAfterGap.y;
			}
			else if( m_bMoveAxisX == true && m_bMoveAxisZ == true )
			{
				vPos.x += vAfterGap.x;
				vPos.z += vAfterGap.z;
			}
			else if( m_bMoveAxisY == true && m_bMoveAxisZ == true )
			{
				vPos.y += vAfterGap.y;
				vPos.z += vAfterGap.z;
			}
			else if( m_bMoveAxisX == true )
			{
				vPos.x += vAfterGap.x;
			}
			else if( m_bMoveAxisY == true )									
			{
				vPos.y += vAfterGap.y;
			}
			else if( m_bMoveAxisZ == true )
			{
				vPos.z += vAfterGap.z;
			}

			pWorldObject->SetPos(vPos);
			if( pWorldObject->GetObjectType() == CX2WorldObject::OT_PARTICLE )
			{
				CX2WorldObjectParticle *pParticle = (CX2WorldObjectParticle*)pWorldObject;
				pParticle->SetParticlePos( vPos );
			}

			if( i== m_iVertPivot )
			{
				m_vPivot = m_vPivot + vAfterGap;
			}
		}
	}
}

void CX2ToolMain::RotationWorldObject(D3DXVECTOR3 vAfterGap, bool bGroup)
{
	float fDist = GetDistance(m_vPivot, g_pX2Game->GetX2Camera()->GetCamera().GetEye());
	float fGap = 100.f / fDist;

	m_vRotationWorldObject = vAfterGap;

	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	D3DXVECTOR3 vRotationTemp = vAfterGap;
	vRotationTemp = vRotationTemp * fGap;
	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( pWorldObject != NULL && pWorldObject->m_bSelected == true && pWorldObject->m_bFreezing == false )
		{									
			D3DXVECTOR3 vRotation = pWorldObject->GetMatrix().GetRotateDegree();

			if( m_bMoveAxisX == true && m_bMoveAxisY == true )
			{	
				vRotation.x += vRotationTemp.y;
				vRotation.y -= vRotationTemp.x;
			}
			else if( m_bMoveAxisX == true && m_bMoveAxisZ == true )
			{
				vRotation.x += vRotationTemp.y;
				vRotation.z -= vRotationTemp.x;
			}
			else if( m_bMoveAxisY == true && m_bMoveAxisZ == true )
			{
				vRotation.y -= vRotationTemp.x;
				vRotation.z -= vRotationTemp.x;
			}
			else if( m_bMoveAxisX == true )
			{
				vRotation.x += vRotationTemp.y;
			}
			else if( m_bMoveAxisY == true )									
			{
				vRotation.y -= vRotationTemp.x;
			}
			else if( m_bMoveAxisZ == true )
			{
				vRotation.z -= vRotationTemp.x;
			}

			pWorldObject->GetMatrix().RotateDegree(vRotation.x, vRotation.y, vRotation.z);

			if( bGroup == true )
			{
				D3DXVECTOR3 vPos = pWorldObject->GetMatrix().GetPos();				
				D3DXMATRIX matPivot;
				float fAngleX = vRotationTemp.x * ( D3DX_PI / 180.f );
				float fAngleY = vRotationTemp.y * ( D3DX_PI / 180.f );
				float fAngleZ = vRotationTemp.z * ( D3DX_PI / 180.f );

				D3DXMatrixIdentity(&matPivot);

				if( m_bMoveAxisX == true && m_bMoveAxisY == true )
				{	
					D3DXMatrixRotationX(&matPivot, fAngleY);
					D3DXMatrixRotationY(&matPivot, -fAngleX);
				}
				else if( m_bMoveAxisX == true && m_bMoveAxisZ == true )
				{
					D3DXMatrixRotationX(&matPivot, fAngleY);
					D3DXMatrixRotationZ(&matPivot, -fAngleX);
				}
				else if( m_bMoveAxisY == true && m_bMoveAxisZ == true )
				{
					D3DXMatrixRotationY(&matPivot, -fAngleX);
					D3DXMatrixRotationZ(&matPivot, -fAngleX);
				}
				else if( m_bMoveAxisX == true )
				{
					D3DXMatrixRotationX(&matPivot, fAngleY);
				}
				else if( m_bMoveAxisY == true )									
				{
					D3DXMatrixRotationY(&matPivot, -fAngleX);
				}
				else if( m_bMoveAxisZ == true )
				{
					D3DXMatrixRotationZ(&matPivot, -fAngleX);
				}

				

				D3DXVECTOR3 vFinalPos = vPos - m_vPosMoveAxis;
				D3DXVec3TransformCoord(&vFinalPos, &vFinalPos, &matPivot);
				vFinalPos = vFinalPos + m_vPosMoveAxis;

				pWorldObject->GetMatrix().Move(vFinalPos);
			}

			if( i== m_iVertPivot )
			{
				m_vPivot = m_vPivot + vAfterGap;
			}
		}
	}
}

void CX2ToolMain::ScaleWorldObject(D3DXVECTOR3 vAfterGap)
{
	m_vScaleWorldObject = vAfterGap;

	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( pWorldObject != NULL && pWorldObject->m_bSelected == true && pWorldObject->m_bFreezing == false )
		{									
			D3DXVECTOR3 vScale = pWorldObject->GetMatrix().GetScale();
			D3DXVECTOR3 vScaleTemp = vAfterGap / pWorldObject->GetBoundingRadius();

			if( m_bMoveAxisX == true && m_bMoveAxisY == true )
			{	
				vScale.x += vScaleTemp.x;
				vScale.y += vScaleTemp.y;
			}
			else if( m_bMoveAxisX == true && m_bMoveAxisZ == true )
			{
				vScale.x += vScaleTemp.x;
				vScale.z += vScaleTemp.z;
			}
			else if( m_bMoveAxisY == true && m_bMoveAxisZ == true )
			{
				vScale.y += vScaleTemp.y;
				vScale.z += vScaleTemp.z;
			}
			else if( m_bMoveAxisX == true )
			{
				vScale.x += vScaleTemp.x;
			}
			else if( m_bMoveAxisY == true )									
			{
				vScale.y += vScaleTemp.y;
			}
			else if( m_bMoveAxisZ == true )
			{
				vScale.z += vScaleTemp.z;
			}

			pWorldObject->SetScale(	vScale );
			
			if( i== m_iVertPivot )
			{
				m_vPivot = m_vPivot + vAfterGap;
			}
		}
	}
}

void CX2ToolMain::UpdateMoveWorldObject()
{
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

	D3DXVECTOR3 vDir = m_vMoveWorldObject - m_vMoveWorldObjectPivot;

	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( pWorldObject != NULL && pWorldObject->m_bSelected == true && pWorldObject->m_bFreezing == false )
		{				
			D3DXVECTOR3 vPos = pWorldObject->GetPos() + vDir;				
			pWorldObject->SetPos( vPos );

			if( pWorldObject->GetObjectType() == CX2WorldObject::OT_PARTICLE )
			{
				CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
				pObject->SetParticlePos( vPos );
			}
		}
	}

	m_vMoveWorldObjectPivot = m_vMoveWorldObjectPivot + vDir;
}
void CX2ToolMain::UpdateRotationWorldObject()
{

}
void CX2ToolMain::UpdateScaleWorldObject()
{
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( pWorldObject != NULL && 
			pWorldObject->GetObjectType() != CX2WorldObject::OT_PARTICLE &&
			pWorldObject->m_bSelected == true && pWorldObject->m_bFreezing == false )
		{				
			D3DXVECTOR3 vScale = pWorldObject->GetMatrix().GetScale();
			vScale = vScale + m_vScaleWorldObject;
			pWorldObject->SetScale(vScale);
		}
	}
}

void CX2ToolMain::CopyWorldObject()
{
	CX2World *pWorld = g_pX2Game->GetWorld();

	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	UINT nCount = static_cast<int>( vecWorldObjectList.size() );
	
	for(UINT i=0; i<nCount; ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		CX2WorldObject *pNewWorldObject = NULL;

		if( pWorldObject != NULL && pWorldObject->m_bSelected == true && pWorldObject->m_bFreezing == false )
		{
			if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
			{
				CX2WorldObjectMesh *pSrcObject = (CX2WorldObjectMesh *)pWorldObject;
				CX2WorldObjectMesh *pDestObject = pWorld->CreateObjectMesh();
				pNewWorldObject = (CX2WorldObject*)pDestObject;

				string strMeshName = "";
				ConvertWCHARToChar(strMeshName, pSrcObject->GetXMeshName().c_str());
				pDestObject->SetXMesh_LUA( strMeshName.c_str() );
				pDestObject->SetLayer( pSrcObject->GetLayer() );
				pDestObject->SetLightPos( pWorld->GetLightPos() );
				pDestObject->SetPos( pSrcObject->GetPos() );
				pDestObject->GetMatrix().RotateDegree( pSrcObject->GetMatrix().GetRotateDegree() );
				pDestObject->GetMatrix().Scale( pSrcObject->GetMatrix().GetScale() );
				pDestObject->SetTex0UVSpeed( pSrcObject->GetTex0UVSpeed().x, pSrcObject->GetTex0UVSpeed().y );
				pDestObject->SetUseWorldColor( pSrcObject->GetUseWorldColor() );
				pDestObject->SetOriginalColorFactor( pSrcObject->GetOriginalColorFactor() );
				pDestObject->SetColorFactor( pSrcObject->GetColorFactor() );
				pDestObject->SetForceShow( pSrcObject->GetForceShow() );
				pDestObject->SetCanHide( pSrcObject->GetCanHide(), pSrcObject->GetCanHide() );
				pDestObject->SetNotRenderWhenHidden( pSrcObject->GetNotRenderWhenHidden() );
				pDestObject->SetHideDistance( pSrcObject->GetHideDistance() );
				pDestObject->SetHideNearDistance( pSrcObject->GetHideNearDistance() );
				pDestObject->SetBillboardType( pSrcObject->GetBillboardType() );
				*pDestObject->GetRenderParam() = *pSrcObject->GetOriRenderParam();
				*pDestObject->GetOriRenderParam() = *pSrcObject->GetOriRenderParam();
				
				pDestObject->m_bUseObjectFogProperty = pSrcObject->m_bUseObjectFogProperty;

				pSrcObject->m_bSelected = false;
				pDestObject->m_bSelected = true;
			}
			else if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
			{
				CX2WorldObjectSkinMesh *pSrcObject = (CX2WorldObjectSkinMesh *)pWorldObject;
				CX2WorldObjectSkinMesh *pDestObject = pWorld->CreateObjectSkinMesh();
				pNewWorldObject = (CX2WorldObject*)pDestObject;

				string strMeshName = "";
				ConvertWCHARToChar(strMeshName, pSrcObject->GetXSkinMeshName().c_str());
				pDestObject->SetXSkinMotion_LUA(strMeshName.c_str());
				pDestObject->AddXSkinModel_LUA(strMeshName.c_str());
				pDestObject->SetLayer( pSrcObject->GetLayer() );
				pDestObject->SetLightPos( pWorld->GetLightPos() );
				pDestObject->SetPos( pSrcObject->GetPos() );
				pDestObject->GetMatrix().RotateDegree( pSrcObject->GetMatrix().GetRotateDegree() );
				pDestObject->GetMatrix().Scale( pSrcObject->GetMatrix().GetScale() );
				pDestObject->SetTex0UVSpeed( pSrcObject->GetTex0UVSpeed().x, pSrcObject->GetTex0UVSpeed().y );
				pDestObject->SetUseWorldColor( pSrcObject->GetUseWorldColor() );
				pDestObject->SetOriginalColorFactor( pSrcObject->GetOriginalColorFactor() );
				pDestObject->SetColorFactor( pSrcObject->GetColorFactor() );
				pDestObject->SetForceShow( pSrcObject->GetForceShow() );
				pDestObject->SetCanHide( pSrcObject->GetCanHide(), pSrcObject->GetCanHide() );
				//pDestObject->SetNotRenderWhenHidden( pSrcObject->GetNotRenderWhenHidden() );
				pDestObject->SetHideDistance( pSrcObject->GetHideDistance() );
				pDestObject->SetHideNearDistance( pSrcObject->GetHideNearDistance() );
				pDestObject->SetBillboardType( pSrcObject->GetBillboardType() );
				*pDestObject->GetRenderParam() = *pSrcObject->GetOriRenderParam();
				*pDestObject->GetOriRenderParam() = *pSrcObject->GetOriRenderParam();			
				pDestObject->m_bUseObjectFogProperty = pSrcObject->m_bUseObjectFogProperty;
				string strAnimName = "";
				ConvertWCHARToChar( strAnimName, pSrcObject->m_wstrOriAnimName.c_str() );
				pDestObject->AddAnim_LUA(strAnimName.c_str());
				pDestObject->PlayAnim_LUA( (CX2WorldObjectSkinMesh::WORLD_OBJECT_SKIN_MESH_PLAY_TYPE)pSrcObject->m_iOriPlayType, true );

				pSrcObject->m_bSelected = false;
				pDestObject->m_bSelected = true;
			}
		}
	}

	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->m_DlgWorldEditor.ResetWorldObject(false);

	tAddRmoveWorldElem tUndoElem;
	tUndoElem.m_bAdded = true;

	vector<CX2WorldObjectPtr> vecWorldObjectNewList = pWorld->GetWorldObjectList4Tool();
	for(UINT i=0; i<vecWorldObjectNewList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectNewList[i].get();
		if( pWorldObject != NULL && pWorldObject->m_bSelected == true && pWorldObject->m_bFreezing == false )
		{
			m_bSelectedWorldObject = true;
			m_iVertPivot = i;								
			m_vMoveWorldObjectPivot = pWorldObject->GetPos();

			CKTDGMatrix matPivot = pWorldObject->GetMatrix();
			matPivot.Scale(1.f, 1.f, 1.f);
			m_matPivot = matPivot.GetMatrix(CKTDGMatrix::BT_NONE);

			tUndoElem.m_vecWorldObject.push_back(vecWorldObjectNewList[i]);
		}
	}	

	AddUndoWorld(tUndoElem);
}

void CX2ToolMain::SaveWorldToFile( const WCHAR* pwszFileFullPath, bool bUpdateFileName )
{
	if( bUpdateFileName == true )
	{
		m_wstrWorldFullName = pwszFileFullPath;
	}
	
	if( g_pX2Game == NULL || g_pX2Game->GetWorld() == NULL )
		return;

	if( m_wstrLineMapFullName.empty() == true )
	{
		m_wstrLineMapFullName = g_pX2Game->GetWorld()->GetLineMap()->GetLineFullName();
	}

	ClearUnDoWorld();

	CX2World *pWorld = g_pX2Game->GetWorld();


	std::map< int, const char* > mapLayerString;
	mapLayerString[ XL_SKY_DOME_0 ]								= "X2_LAYER[\"XL_SKY_DOME_0\"]";
	mapLayerString[ XL_SKY_DOME_1 ]								= "X2_LAYER[\"XL_SKY_DOME_1\"]";
	mapLayerString[ XL_SKY_DOME_2 ]								= "X2_LAYER[\"XL_SKY_DOME_2\"]";
	mapLayerString[ XL_SKY_WORLD_BASE_0 ]						= "X2_LAYER[\"XL_SKY_WORLD_BASE_0\"]";
	mapLayerString[ XL_SKY_WORLD_BASE_1 ]						= "X2_LAYER[\"XL_SKY_WORLD_BASE_1\"]";
	mapLayerString[ XL_SKY_WORLD_BASE_2 ]						= "X2_LAYER[\"XL_SKY_WORLD_BASE_2\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_0 ]						= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_0\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_1 ]						= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_1\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_2 ]						= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_2\"]";
	mapLayerString[ XL_UNIT_0	]								= "X2_LAYER[\"XL_UNIT_0\"]";
	mapLayerString[ XL_UNIT_1	]								= "X2_LAYER[\"XL_UNIT_1\"]";
	mapLayerString[ XL_UNIT_2	]								= "X2_LAYER[\"XL_UNIT_2\"]";

	mapLayerString[ XL_EFFECT_0 ]								= "X2_LAYER[\"XL_EFFECT_0\"]";
	mapLayerString[ XL_EFFECT_1 ]								= "X2_LAYER[\"XL_EFFECT_1\"]";
	mapLayerString[ XL_EFFECT_2 ]								= "X2_LAYER[\"XL_EFFECT_2\"]";

	//{{ kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업
#ifdef	ADD_X2_LAYER
	mapLayerString[ XL_SKY_WORLD_OBJECT_OVER_EFFECT_0 ]			= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_OVER_EFFECT_0\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_OVER_EFFECT_1 ]			= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_OVER_EFFECT_1\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_OVER_EFFECT_2 ]			= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_OVER_EFFECT_2\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_OVER_EFFECT_3 ]			= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_OVER_EFFECT_3\"]";
 	mapLayerString[ XL_EFFECT_OVER_ALL_WORLD_OBJECT_0 ]			= "X2_LAYER[\"XL_EFFECT_OVER_ALL_WORLD_OBJECT_0\"]";
 	mapLayerString[ XL_EFFECT_OVER_ALL_WORLD_OBJECT_1 ]			= "X2_LAYER[\"XL_EFFECT_OVER_ALL_WORLD_OBJECT_1\"]";
#endif	ADD_X2_LAYER
	//}} kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업

 	mapLayerString[ XL_LENS_FLARE ]								= "X2_LAYER[\"XL_LENS_FLARE\"]";
 	mapLayerString[ XL_MINIMAP	]								= "X2_LAYER[\"XL_MINIMAP\"]";



	std::map< int, const char* > mapSkinAnimPlayTypeString;
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_ONE ]					= "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_ONE\"]";
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_ONE_WAIT ]			= "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_ONE_WAIT\"]";
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_LOOP ]				= "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_LOOP\"]";
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_PINGPONG ]			= "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_PINGPONG\"]";
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_OCCASIONAL_ONE ]		= "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_OCCASIONAL_ONE\"]";
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_OCCASIONAL_ONE_WAIT ] = "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_OCCASIONAL_ONE_WAIT\"]";

	string strRenderType[] = 
	{
		"RT_REAL_COLOR", // = 0,			/// 자체 색상
		"RT_CARTOON_BLACK_EDGE",		/// 카툰 + 검은 색 외곽선
		"RT_CARTOON_COLOR_EDGE",		/// 카툰 + 다른 색 외곽선
		"RT_CARTOON",					/// 카툰
		"RT_ADD_CARTOON_BLACK_EDGE",	/// 카툰 + 검은 색 외곽선
		"RT_ADD_CARTOON_COLOR_EDGE",	/// 카툰 + 다른 색 외곽선
		"RT_ADD_CARTOON",				/// 카툰
		"RT_BLACK_EDGE",				/// 검은 색 외곽선
		"RT_COLOR_EDGE",				/// 다른 색 외곽선
	};

	string strBlend[] = 
	{
		"D3DBLEND_ZERO", //1
		"D3DBLEND_ONE",
		"D3DBLEND_SRCCOLOR",
		"D3DBLEND_INVSRCCOLOR",
		"D3DBLEND_SRCALPHA",
		"D3DBLEND_INVSRCALPHA",
		"D3DBLEND_DESTALPHA",
		"D3DBLEND_INVDESTALPHA",
		"D3DBLEND_DESTCOLOR",
		"D3DBLEND_INVDESTCOLOR",
		"D3DBLEND_SRCALPHASAT",
		"D3DBLEND_BOTHSRCALPHA",
		"D3DBLEND_BOTHINVSRCALPHA",
	};

	// 파일 확장자가 없으면 붙여주자.
	std::string strFileName;	
	strFileName.reserve(512);
	ConvertWCHARToChar( strFileName, m_wstrWorldFullName.c_str() );	
	MakeUpperCase( strFileName );

	int iStrLen = strFileName.length();
	if( 'L' != strFileName[iStrLen-3] || 'U' != strFileName[iStrLen-2] || 'A' != strFileName[iStrLen-1] ) 
	{
		ConvertWCHARToChar( strFileName, m_wstrWorldFullName.c_str() );	
		strFileName.append(".lua");
	}
	else
	{
		ConvertWCHARToChar( strFileName, m_wstrWorldFullName.c_str() );	
	}

	std::wstring wstrFilePath;
	ConvertCharToWCHAR( wstrFilePath, strFileName.c_str() );

	std::ofstream ofs( strFileName.c_str() );
	if( ofs.is_open() == false )
	{
		ofs.close();
		return;
	}

	WCHAR driveTemp[_MAX_DRIVE];
	WCHAR dirTemp[_MAX_DIR];
	WCHAR fnameTemp[_MAX_FNAME];
	WCHAR extTemp[_MAX_EXT];
	_wsplitpath( pwszFileFullPath, driveTemp, dirTemp, fnameTemp, extTemp);
	string strWorldName;	
	ConvertWCHARToChar( strWorldName, fnameTemp );
	ofs << "-- " << strWorldName << std::endl;
	ofs << std::endl;
	ofs << std::endl;
	ofs << std::endl;

		
	D3DXVECTOR3 vLightPos = pWorld->GetLightPos();
	ofs << "g_pWorld:SetLightPos( " << vLightPos.x << ", " << vLightPos.y << ", " << vLightPos.z << " )" << std::endl;

	D3DXCOLOR& lightColor = pWorld->GetLightColor();
	if( IsSamef(lightColor.r, 1.f) == false && IsSamef(lightColor.g, 1.f) == false &&
		IsSamef(lightColor.b, 1.f) == false && IsSamef(lightColor.a, 1.f) == false )
	{		
		ofs << "g_pWorld:SetLightColor_LUA(  " << (float)lightColor.r << ", " 
			<< (float)lightColor.g << ", " << (float)lightColor.b << " ) " << std::endl;
	}

	D3DXCOLOR& coOriginLightColor = pWorld->GetOriginColor();
	if( IsSamef(coOriginLightColor.r, 1.f) == false && IsSamef(coOriginLightColor.g, 1.f) == false &&
		IsSamef(coOriginLightColor.b, 1.f) == false && IsSamef(coOriginLightColor.a, 1.f) == false )
	{
		ofs << "g_pWorld:SetOriginColor_LUA(  " << (float)coOriginLightColor.r << ", " 
			<< (float)coOriginLightColor.g << ", " << (float)coOriginLightColor.b << " ) " << std::endl;
	}

	WCHAR driveLine[_MAX_DRIVE];
	WCHAR dirLine[_MAX_DIR];
	WCHAR fnameLine[_MAX_FNAME];
	WCHAR extLine[_MAX_EXT];
	_wsplitpath( m_wstrLineMapFullName.c_str(), driveLine, dirLine, fnameLine, extLine);
	string strLineName;
	ConvertWCHARToChar(strLineName, fnameLine);

	ofs << "g_pWorld:SetLineMap( \"" << strLineName << ".lua\" )" << std::endl;
	
	string strSoundName;
	ConvertWCHARToChar(strSoundName, m_wstrSoundName.c_str());
	ofs << "g_pWorld:SetBGM( \"" << strSoundName << "\" )" << std::endl;
	ofs << std::endl;
	ofs << std::endl;
	ofs << std::endl;

	//Add3DEffectBGM_LUA
	//AddEffectBGM_LUA

	wstring wstrWorldCamera = pWorld->GetWorldCamera();
	string strWorldCamera;
	ConvertWCHARToChar(strWorldCamera, wstrWorldCamera.c_str());
	if( strWorldCamera.empty() == false )
	{
		ofs << "g_pWorld:SetWorldCamera( \"" << strWorldCamera << "\" )" << std::endl;
	}
	

	if( pWorld->m_bFog == true && pWorld->m_bFogShow == true )
	{
		ofs << "g_pWorld:SetFogWorld( " << pWorld->m_fNearX << ", " << pWorld->m_fFarX << ", " << pWorld->m_fNearY << ", " << pWorld->m_fFarY 
			<< ", " << pWorld->m_fDensity << ", " 
			<< "D3DXCOLOR( " << pWorld->m_FogColor.r << ", " << pWorld->m_FogColor.g << ", " << pWorld->m_FogColor.b << ", 1.0 ) )" << std::endl;
	}
	
#ifdef MODIFY_FRUSTUM
	float fBase = 0.f;
	float fCamera1 = 0.f;
	float fCamera2 = 0.f;
	float fCamera3 = 0.f;
	
	fBase = pWorld->GetBaseCameraDistance();
	fCamera1 = pWorld->GetCameraStepDistacne(0);
	fCamera2 = pWorld->GetCameraStepDistacne(1);
	fCamera3 = pWorld->GetCameraStepDistacne(2);

	ofs << "g_pWorld:SetProjection( " << g_pKTDXApp->GetDGManager()->GetNear() << ", " << g_pKTDXApp->GetDGManager()->GetFar() << ", " 				
		<< g_pKTDXApp->GetDGManager()->GetPerspectiveValue() << ", "
		<< fBase << ", " << fCamera1 << ", " << fCamera2 << ", " << fCamera3 << " )" << std::endl << std::endl;	
#endif

    // World Layer
	vector<CX2WorldLayer*> vecWorldLayer = pWorld->GetWorldLayerList();
    for(int i=0; i<(int)vecWorldLayer.size(); ++i)
    {
        CX2WorldLayer *pLayer = vecWorldLayer[i];
        if(pLayer != NULL)
        {
            float x, y, w, h;
            D3DXVECTOR2 uv, feedbackUV;
            D3DXCOLOR color;            
            pLayer->GetPos(&x, &y, &w, &h);
            uv = pLayer->GetTexUV();
            feedbackUV = pLayer->GetFeedBackUV();
            color = pLayer->GetColor();

            ofs << "g_pWorldLayer = g_pWorld:CreateWorldLayer()" << std::endl;
            ofs << "g_pWorldLayer:SetLayer(" << mapLayerString[ pLayer->GetLayer() ] << ")" << std::endl;
            ofs << "g_pWorldLayer:SetBlendType(" << pLayer->GetBlendType() << ")" << std::endl;

			string strWorldLayerTexName = "";
			ConvertWCHARToChar( strWorldLayerTexName, pLayer->GetTexName().c_str() );
            ofs << "g_pWorldLayer:SetTex( \"" << strWorldLayerTexName << "\" )" << std::endl;
            
			ofs << "g_pWorldLayer:SetAlphaObject(true)" << std::endl;
            ofs << "g_pWorldLayer:SetPos(" << x << ", " << y << ", " << w << ", " << h << ")" << std::endl;
            ofs << "g_pWorldLayer:SetColor(D3DXCOLOR(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << "))" << std::endl;            
            ofs << "g_pWorldLayer:SetTexUV(" << uv.x << ", " << uv.y << ")" << std::endl << std::endl;
            ofs << "g_pWorldLayer:SetFeedBackUV(" << feedbackUV.x << ", " << feedbackUV.y << ")" << std::endl;
			ofs << "g_pWorldLayer:SetLayerId(" << i << ")" << std::endl;
			if( pLayer->GetShowObject() == true )
				ofs << "g_pWorldLayer:SetInitShow( true )" << std::endl << std::endl;
			else 
				ofs << "g_pWorldLayer:SetInitShow( false )" << std::endl << std::endl;
        }
    }
    // End of World Layer



	// skydome 
	vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();
	ofs << "-- SKY / CLOUD -- " << std::endl;
	for( UINT i=0; i<vecSkyDome.size() ; i++ )
	{
		CKTDGSkyDome* pSkyDome = vecSkyDome[i];
		if( NULL == pSkyDome )
			continue;


		D3DXVECTOR3 vPos = pSkyDome->GetMatrix().GetPos();
		D3DXVECTOR3 vScale = pSkyDome->GetMatrix().GetScale();
		string strMeshName;
		ConvertWCHARToChar( strMeshName, pSkyDome->GetName().c_str() );

		
		ofs << "g_pSkyDome = g_pWorld:CreateSkyDome()" << std::endl;
		if( mapLayerString.end() != mapLayerString.find( pSkyDome->GetLayer() ) )
		{
			ofs << "g_pSkyDome:SetLayer( " << mapLayerString[ pSkyDome->GetLayer() ] << " )" << std::endl;
		}
		else
		{
			ofs << "g_pSkyDome:SetLayer( " << "X2_LAYER[\"XL_SKY_DOME_0\"]" << " )" << std::endl;
		}
			
		
		ofs << "g_pSkyDome:SetDomeMesh( \"" << strMeshName << "\" )" << std::endl;		
		ofs << "g_pSkyDome:SetPos( " << vPos.x << ", " << vPos.y << ", " << vPos.z << " )" << std::endl;
		if( vScale != D3DXVECTOR3(1,1,1) )
			ofs << "g_pSkyDome:SetScale( D3DXVECTOR3( " << vScale.x << ", " << vScale.y << ", " << vScale.z << ") )" << std::endl;
		ofs << "g_pSkyDome:SetRotationSpeed( " << pSkyDome->GetRotationSpeed() << " )" << std::endl;

		D3DXVECTOR2 v2UVSpeed = pSkyDome->GetTex0UVSpeed();
		if( v2UVSpeed.x != 0.f || v2UVSpeed.y != 0.f )
		{
			ofs << "g_pSkyDome:SetTex0UVSpeed( " << v2UVSpeed.x << ", " << v2UVSpeed.y << " )" << std::endl;
		}

		if( true == pSkyDome->GetUseWorldColor() )
			ofs << "g_pSkyDome:SetUseWorldColor( true )" << std::endl;
		else
			ofs << "g_pSkyDome:SetUseWorldColor( false )" << std::endl;

		D3DXCOLOR color = pSkyDome->GetSkyDomeColor();
		ofs << "g_pSkyDome:SetSkyDomeColor( D3DXCOLOR(" << color.r << ", " << color.g << ", " << color.b << ", " << 1 << "))" << std::endl;
		
		if( true == pSkyDome->GetViewTrace() )
            ofs << "g_pSkyDome:SetViewTrace( true )" << std::endl;
		else
			ofs << "g_pSkyDome:SetViewTrace( false )" << std::endl;

		if( true == pSkyDome->IsAlphaObject() )
			ofs << "g_pSkyDome:SetAlphaObject( true )" << std::endl;
		else
			ofs << "g_pSkyDome:SetAlphaObject( false )" << std::endl;

		ofs << "g_pRenderParam = g_pSkyDome:GetRenderParam()" << std::endl;

		CKTDGXRenderer::RenderParam* pRenderParam = pSkyDome->GetRenderParam();
		if( true == pRenderParam->bAlphaBlend )
			ofs << "g_pRenderParam.bAlphaBlend = true" << std::endl;
		else
			ofs << "g_pRenderParam.bAlphaBlend = false" << std::endl;
		ofs << "g_pRenderParam.renderType = RENDER_TYPE[\"" << strRenderType[(int)pRenderParam->renderType] << "\"]" << std::endl;
		ofs << "g_pRenderParam.srcBlend = D3DBLEND[\"" << strBlend[(int)pRenderParam->srcBlend - 1] << "\"]" << std::endl;
		ofs << "g_pRenderParam.destBlend = D3DBLEND[\"" << strBlend[(int)pRenderParam->destBlend - 1] << "\"]" << std::endl;


        if( pRenderParam->bFog == true)
        {
            ofs << "g_pSkyDome:SetFog( " << pRenderParam->fogNearX << ", " << pRenderParam->fogFarX << ", " 
                << pRenderParam->fogNearY << ", " << pRenderParam->fogFarY 
                << ", D3DXCOLOR(" << pRenderParam->fogColor.r << ", " << pRenderParam->fogColor.g << ", " << pRenderParam->fogColor.b << ", " << pRenderParam->fogColor.a << "), " 
                << pRenderParam->fogDensity << " )" << std::endl;
        }


		ofs << std::endl << std::endl;			
	}
	

	// static mesh
	ofs << "-- STATIC MESH -- " << std::endl;
	vector<CX2WorldObjectPtr> vecWorldObjectNewList = pWorld->GetWorldObjectList4Tool();
	for(UINT i=0; i<vecWorldObjectNewList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectNewList[i].get();
		if( pWorldObject == NULL || pWorldObject->GetObjectType() != CX2WorldObject::OT_STATIC_MESH )
			continue;

		CX2WorldObjectMesh* pMesh = (CX2WorldObjectMesh*) pWorldObject;
		D3DXVECTOR3 vPos = pMesh->GetMatrix().GetPos();
		D3DXVECTOR3 vRotationDegree = pMesh->GetMatrix().GetRotateDegree();
		D3DXVECTOR3 vScale = pMesh->GetMatrix().GetScale();
		
		string strMeshName;
		ConvertWCHARToChar( strMeshName, pMesh->GetXMeshName().c_str() );

		ofs << "g_pObjectMesh = g_pWorld:CreateObjectMesh()" << std::endl;
		ofs << "g_pObjectMesh:SetXMesh(    \"" << strMeshName << "\" )" << std::endl;
		ofs << "g_pObjectMesh:SetXMeshLOD( \"" << strMeshName << "\" )" << std::endl;

		if( mapLayerString.end() != mapLayerString.find( pMesh->GetLayer() ) )
		{
			ofs << "g_pObjectMesh:SetLayer( " << mapLayerString[ pMesh->GetLayer() ] << " )" << std::endl;
		}
		else
		{
			ofs << "g_pObjectMesh:SetLayer( " << "X2_LAYER[\"XL_SKY_WORLD_BASE_0\"]" << " )" << std::endl;
		}

		ofs << "g_pObjectMesh:SetPos( D3DXVECTOR3(" << vPos.x << ", " << vPos.y << ", " << vPos.z << ") )" << std::endl;
		ofs << "g_pObjectMesh:SetAngleDegree( D3DXVECTOR3(" << vRotationDegree.x << ", " << vRotationDegree.y << ", " << vRotationDegree.z << ") )" << std::endl;
		ofs << "g_pObjectMesh:SetScale( D3DXVECTOR3( " << vScale.x << ", " << vScale.y << ", " << vScale.z << ") )" << std::endl;
		
		if( pMesh->GetTex0UVSpeed() != D3DXVECTOR2(0, 0) )
		{
			ofs << "g_pObjectMesh:SetTex0UVSpeed( " << pMesh->GetTex0UVSpeed().x << ", " << pMesh->GetTex0UVSpeed().y << " )" << std::endl;
		}
				
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
		//** Static Mesh 컬러펙터 지정
		D3DXCOLOR color = pMesh->GetOriginalColorFactor();
		ofs << "g_pObjectMesh:SetOriginalColorFactor( D3DXCOLOR(" << color.r << ", " << color.g << ", " << color.b << ", " << 1 << "))" << std::endl;            
#endif
		ofs << "g_pObjectMesh:SetColorFactor( D3DXCOLOR(1,1,1,1) )" << std::endl;
		if( true == pMesh->GetCanHide() )
		{
			ofs << "g_pObjectMesh:SetCanHide( true, true )" << std::endl;
		}
		else
		{
			ofs << "g_pObjectMesh:SetCanHide( false, false )" << std::endl;
		}

		if( true == pMesh->GetNotRenderWhenHidden() )
		{
			ofs << "g_pObjectMesh:SetNotRenderWhenHidden( true )" << std::endl;
		}
		else
		{
			ofs << "g_pObjectMesh:SetNotRenderWhenHidden( false )" << std::endl;
		}

		if( true == pMesh->IsAlphaObject() )
		{
			ofs << "g_pObjectMesh:SetAlphaObject( true )" << std::endl;							
		}
		else
		{
			ofs << "g_pObjectMesh:SetAlphaObject( false )" << std::endl;
		}

		ofs << "g_pRenderParam = g_pObjectMesh:GetRenderParam()" << std::endl;
		CKTDGXRenderer::RenderParam* pRenderParam = pMesh->GetRenderParam();
		if( true == pRenderParam->bAlphaBlend )		
		{
			ofs << "g_pRenderParam.bAlphaBlend = true" << std::endl;
		}
		else
		{
			ofs << "g_pRenderParam.bAlphaBlend = false" << std::endl;
		}
#ifdef ADD_ALPHATESTENABLE
		if( true == pRenderParam->bAlphaTestEnable )
		{
			ofs << "g_pRenderParam.bAlphaTestEnable = true" << std::endl;
		}
		else
		{
			ofs << "g_pRenderParam.bAlphaTestEnable = false" << std::endl;
		}
		if( pRenderParam->cullMode == D3DCULL_NONE )
		{
			ofs << "g_pRenderParam.cullMode = D3DCULL[\"D3DCULL_NONE\"]" << std::endl;
		}
#endif
		ofs << "g_pRenderParam.renderType = RENDER_TYPE[\"" << strRenderType[(int)pRenderParam->renderType] << "\"]" << std::endl;
		ofs << "g_pRenderParam.srcBlend = D3DBLEND[\"" << strBlend[(int)pRenderParam->srcBlend - 1] << "\"]" << std::endl;
		ofs << "g_pRenderParam.destBlend = D3DBLEND[\"" << strBlend[(int)pRenderParam->destBlend - 1] << "\"]" << std::endl;
						
		if( false == pMesh->GetUseWorldColor() )
		{
			ofs << "g_pObjectMesh:SetUseWorldColor( false )" << std::endl;
		}

        if( pRenderParam->bFog == true)
        {
            ofs << "g_pObjectMesh:SetFog( " << pRenderParam->fogNearX << ", " << pRenderParam->fogFarX << ", " 
                << pRenderParam->fogNearY << ", " << pRenderParam->fogFarY 
                << ", D3DXCOLOR(" << pRenderParam->fogColor.r << ", " << pRenderParam->fogColor.g << ", " << pRenderParam->fogColor.b << ", " << pRenderParam->fogColor.a << "), " 
                << pRenderParam->fogDensity << " )" << std::endl;
        }

		switch( pMesh->GetBillboardType() )
		{
		default:
		case CKTDGMatrix::BT_NONE:
			break;

		case CKTDGMatrix::BT_ALL:
			ofs << "g_pObjectMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_ALL\"] )" << std::endl;
			break;
		case CKTDGMatrix::BT_X:
			ofs << "g_pObjectMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_X\"] )" << std::endl;
			break;
		case CKTDGMatrix::BT_Y:
			ofs << "g_pObjectMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_Y\"] )" << std::endl;
			break;
		case CKTDGMatrix::BT_Z:
			ofs << "g_pObjectMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_Z\"] )" << std::endl;
			break;
		}

		
		if(pMesh->GetHideDistance() > 0.0f)
		{
			ofs << "g_pObjectMesh:SetHideDistance( " << pMesh->GetHideDistance() << ")" << std::endl;
		}

		float fHideNearDist = pMesh->GetHideNearDistance();
		if( IsSamef(fHideNearDist, 0.f) == false )
			ofs << "g_pObjectMesh:GetHideNearDistance( " << fHideNearDist << ")" << std::endl;
				
#ifdef NEW_VILLAGE
		if(true == pMesh->GetForceShow())
		{
			ofs << "g_pObjectMesh:SetForceShow( true )" << std::endl;
		}
#endif

		ofs << std::endl << std::endl;	
	}

	// skin mesh	
	ofs << "-- SKIN MESH -- " << std::endl;
	for(UINT i=0; i<vecWorldObjectNewList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectNewList[i].get();
		if( pWorldObject == NULL || pWorldObject->GetObjectType() != CX2WorldObject::OT_SKIN_MESH )
			continue;

		CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*)pWorldObject;
		D3DXVECTOR3 vPos = pSkinMesh->GetMatrix().GetPos();
		D3DXVECTOR3 vRotationDegree = pSkinMesh->GetMatrix().GetRotateDegree();
		D3DXVECTOR3 vScale = pSkinMesh->GetMatrix().GetScale();
		string strSkinMeshName;
		string strSkinMotionName;
		ConvertWCHARToChar( strSkinMeshName, pSkinMesh->GetXSkinMeshName().c_str() );
		ConvertWCHARToChar( strSkinMotionName, pSkinMesh->GetXSkinMotionMesh()->GetDeviceID().c_str() );

		ofs << "g_pObjectSkinMesh = g_pWorld:CreateObjectSkinMesh()" << std::endl;
		ofs << "g_pObjectSkinMesh:SetXSkinMotion( \"" << strSkinMotionName << "\" )" << std::endl;
		ofs << "g_pObjectSkinMesh:AddXSkinModel( \"" << strSkinMeshName << "\" )" << std::endl;
		
		wstring wstrAniName = pSkinMesh->m_wstrOriAnimName;
		string strAniName;
		ConvertWCHARToChar( strAniName, wstrAniName.c_str() );
		ofs << "g_pObjectSkinMesh:AddAnim( \"" << strAniName << "\" )" << std::endl;
		ofs << "g_pObjectSkinMesh:PlayAnim( " << mapSkinAnimPlayTypeString[pSkinMesh->m_iOriPlayType] << " )" << std::endl;
		
		// 이거 추가해야하는거임?
#if 0
		switch( pWorldToolObject->GetPlayType() )
		{
		case CX2WorldObjectSkinMesh::WOSMPY_OCCASIONAL_ONE:
		case CX2WorldObjectSkinMesh::WOSMPY_OCCASIONAL_ONE_WAIT:
			{
				bool bUseTwoNumbersForRandom = false;
				if( true == bUseTwoNumbersForRandom )
				{
					ofs << "g_pObjectSkinMesh:SetOccasionalEventTime( RandomFloat_LUA( " << pWorldToolObject->GetOccasionalEventTime().x <<
						", " << pWorldToolObject->GetOccasionalEventTime().y << " )" << std::endl;

					ofs << "g_pObjectSkinMesh:SetOccasionalPlayRate( RandomInt_LUA( " << (int)pWorldToolObject->GetOccasionalPlayRate().x <<
						", " << (int)pWorldToolObject->GetOccasionalPlayRate().y << " )" << std::endl;
				}
				else
				{
					ofs << "g_pObjectSkinMesh:SetOccasionalEventTime( " << pWorldToolObject->GetOccasionalEventTime().x 
						<< " )" << std::endl;

					ofs << "g_pObjectSkinMesh:SetOccasionalPlayRate( " << (int)pWorldToolObject->GetOccasionalPlayRate().x 
						<< " )" << std::endl;
				}
			} break;
		default:
			break;

		}
#endif

		if( mapLayerString.end() != mapLayerString.find( pSkinMesh->GetLayer() ) )
		{
			ofs << "g_pObjectSkinMesh:SetLayer( " << mapLayerString[ pSkinMesh->GetLayer() ] << " )" << std::endl;
		}
		else
		{
			ofs << "g_pObjectSkinMesh:SetLayer( " << "X2_LAYER[\"XL_SKY_WORLD_BASE_0\"]" << " )" << std::endl;
		}

		ofs << "g_pObjectSkinMesh:SetPos( D3DXVECTOR3(" << vPos.x << ", " << vPos.y << ", " << vPos.z << ") )" << std::endl;
		ofs << "g_pObjectSkinMesh:SetAngleDegree( D3DXVECTOR3(" << vRotationDegree.x << ", " << vRotationDegree.y << ", " << vRotationDegree.z << ") )" << std::endl;
		ofs << "g_pObjectSkinMesh:SetScale( D3DXVECTOR3( " << vScale.x << ", " << vScale.y << ", " << vScale.z << ") )" << std::endl;

		if( pSkinMesh->GetTex0UVSpeed() != D3DXVECTOR2(0, 0) )
		{
			ofs << "g_pObjectSkinMesh:SetTex0UVSpeed( " << pSkinMesh->GetTex0UVSpeed().x << ", " << pSkinMesh->GetTex0UVSpeed().y << " )" << std::endl;
		}
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
		D3DXCOLOR color = pSkinMesh->GetOriginalColorFactor();
		ofs << "g_pObjectSkinMesh:SetOriginalColorFactor( D3DXCOLOR(" << color.r << ", " << color.g << ", " << color.b << ", " << 1 << "))" << std::endl;            
#endif
		ofs << "g_pObjectSkinMesh:SetColorFactor( D3DXCOLOR(1,1,1,1) )" << std::endl;

		if( true == pSkinMesh->GetCanHide() )
		{
			ofs << "g_pObjectSkinMesh:SetCanHide( true, true )" << std::endl;
		}
		else
		{
			ofs << "g_pObjectSkinMesh:SetCanHide( false, false )" << std::endl;
		}

		// 현재는 스킨메쉬에는 Not Render 옵션이 존재하지 않는다.

		ofs << "g_pRenderParam = g_pObjectSkinMesh:GetRenderParam()" << std::endl;

		if( true == pSkinMesh->IsAlphaObject() )
		{
			ofs << "g_pObjectSkinMesh:SetAlphaObject( true )" << std::endl;					
		}	
		else
		{
			ofs << "g_pObjectSkinMesh:SetAlphaObject( false )" << std::endl;
		}

		CKTDGXRenderer::RenderParam* pRenderParam = pSkinMesh->GetRenderParam();
		if( true == pRenderParam->bAlphaBlend )		
		{
			ofs << "g_pRenderParam.bAlphaBlend = true" << std::endl;					
		}
		else
		{
			ofs << "g_pRenderParam.bAlphaBlend = false" << std::endl;					
		}


		// 스킨메쉬는 알파테스트를 지원하지 않는다.
		ofs << "g_pRenderParam.bAlphaTestEnable = false" << std::endl;

		if( pRenderParam->cullMode == D3DCULL_NONE )
		{
			ofs << "g_pRenderParam.cullMode = D3DCULL[\"D3DCULL_NONE\"]" << std::endl;
		}

		ofs << "g_pRenderParam.renderType = RENDER_TYPE[\"" << strRenderType[(int)pRenderParam->renderType] << "\"]" << std::endl;
		ofs << "g_pRenderParam.srcBlend = D3DBLEND[\"" << strBlend[(int)pRenderParam->srcBlend - 1] << "\"]" << std::endl;
		ofs << "g_pRenderParam.destBlend = D3DBLEND[\"" << strBlend[(int)pRenderParam->destBlend - 1] << "\"]" << std::endl;

		if( false == pSkinMesh->GetUseWorldColor() )
		{
			ofs << "g_pObjectSkinMesh:SetUseWorldColor( false )" << std::endl;
		}
		switch( pSkinMesh->GetBillboardType() )
		{
		default:
		case CKTDGMatrix::BT_NONE:
			break;

		case CKTDGMatrix::BT_ALL:
			ofs << "g_pObjectSkinMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_ALL\"] )" << std::endl;
			break;
		case CKTDGMatrix::BT_X:
			ofs << "g_pObjectSkinMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_X\"] )" << std::endl;
			break;
		case CKTDGMatrix::BT_Y:
			ofs << "g_pObjectSkinMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_Y\"] )" << std::endl;
			break;
		case CKTDGMatrix::BT_Z:
			ofs << "g_pObjectSkinMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_Z\"] )" << std::endl;
			break;
		}

		
		if(pSkinMesh->GetHideDistance() > 0.0f)
		{
			ofs << "g_pObjectSkinMesh:SetHideDistance( " << pSkinMesh->GetHideDistance() << ")" << std::endl;
		}
		float fHideNearDist = pSkinMesh->GetHideNearDistance();
		if( IsSamef( fHideNearDist ) == false )
			ofs << "g_pObjectSkinMesh:GetHideNearDistance( " << fHideNearDist << ")" << std::endl;

#ifdef NEW_VILLAGE
		if(true == pSkinMesh->GetForceShow())
		{
			ofs << "g_pObjectSkinMesh:SetForceShow( " << pSkinMesh->GetForceShow() << ")" << std::endl;
		}
#endif

		if( pRenderParam->bFog == true)
		{
			ofs << "g_pObjectSkinMesh:SetFog( " << pRenderParam->fogNearX << ", " << pRenderParam->fogFarX << ", " 
				<< pRenderParam->fogNearY << ", " << pRenderParam->fogFarY 
				<< ", D3DXCOLOR(" << pRenderParam->fogColor.r << ", " << pRenderParam->fogColor.g << ", " << pRenderParam->fogColor.b << ", " << pRenderParam->fogColor.a << "), " 
				<< pRenderParam->fogDensity << " )" << std::endl;
		}

		ofs << std::endl << std::endl;
	}

	// particle
	ofs << "-- PARTICLE -- " << std::endl;
	for(UINT i=0; i<vecWorldObjectNewList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectNewList[i].get();
		if( pWorldObject == NULL || pWorldObject->GetObjectType() != CX2WorldObject::OT_PARTICLE )
			continue;

		CX2WorldObjectParticle* pParticle = (CX2WorldObjectParticle*) pWorldObject;

		string strParticleSequenceName;
		ConvertWCHARToChar( strParticleSequenceName, pParticle->GetParticleEventSequence()->GetName() );

		float fDelayTime = pParticle->GetDelayTime();
		D3DXVECTOR3 vPos = pParticle->GetMatrix().GetPos();
		CMinMax<float> emitRate = pParticle->GetParticleEventSequence()->GetEmitRate();
		int iTriggerCount = pParticle->GetParticleEventSequence()->GetTriggerCount();
		float fTriggerTime = pParticle->GetParticleEventSequence()->GetTriggerTime();
		int iDrawCount = pParticle->GetParticleEventSequence()->GetDrawCount();
		bool bCullCheck = pParticle->GetParticleEventSequence()->GetCullCheck();
		CKTDGParticleSystem::PARTICLE_TYPE eParticleType = pParticle->GetParticleEventSequence()->GetParticleType();

		switch(pParticle->m_eParticleType)
		{
		case CX2WorldObjectParticle::PT_GAME_MAJOR:
			if( fDelayTime <= 0.f )
				ofs << "g_pObjectParticle = g_pWorld:CreateObjectParticle( g_pMajorParticle, \"" << strParticleSequenceName << "\" )" << std::endl;
			else
				ofs << "g_pObjectParticle = g_pWorld:CreateObjectParticleDelay( g_pMajorParticle, \"" << strParticleSequenceName << "\", " << fDelayTime << " )" << std::endl;
			break;
		case CX2WorldObjectParticle::PT_GAME_MINOR:
			if( fDelayTime <= 0.f )
				ofs << "g_pObjectParticle = g_pWorld:CreateObjectParticle( g_pMinorParticle, \"" << strParticleSequenceName << "\" )" << std::endl;
			else
				ofs << "g_pObjectParticle = g_pWorld:CreateObjectParticleDelay( g_pMinorParticle, \"" << strParticleSequenceName << "\", " << fDelayTime << " )" << std::endl;
			break;
		case CX2WorldObjectParticle::PT_UI_MAJOR:
			if( fDelayTime <= 0.f )
				ofs << "g_pObjectParticle = g_pWorld:CreateObjectParticle( g_pUIMajorParticle, \"" << strParticleSequenceName << "\" )" << std::endl;
			else
				ofs << "g_pObjectParticle = g_pWorld:CreateObjectParticleDelay( g_pUIMajorParticle, \"" << strParticleSequenceName << "\", " << fDelayTime << " )" << std::endl;
			break;
		case CX2WorldObjectParticle::PT_UI_MINOR:
			if( fDelayTime <= 0.f )
				ofs << "g_pObjectParticle = g_pWorld:CreateObjectParticle( g_pUIMinorParticle, \"" << strParticleSequenceName << "\" )" << std::endl;
			else
				ofs << "g_pObjectParticle = g_pWorld:CreateObjectParticleDelay( g_pUIMinorParticle, \"" << strParticleSequenceName << "\", " << fDelayTime << " )" << std::endl;
			break;
		default:
			ofs << "g_pObjectParticle = g_pWorld:CreateObjectParticle( g_pMinorParticle, \"" << strParticleSequenceName << "\" )" << std::endl;
			break;
		}
		ofs << "g_pObjectParticle:SetParticlePos( D3DXVECTOR3( " << vPos.x << ", " << vPos.y << ", " << vPos.z << " ) )" << std::endl;
		ofs << "g_pObjectParticle:SetEmitMinMax( " << (float)emitRate.m_Min << ", " << (float)emitRate.m_Max << " )" << std::endl;
		ofs << "g_pObjectParticle:SetTriggerCount( " << iTriggerCount << " )" << std::endl;
		ofs << "g_pObjectParticle:SetTriggerTime( " << fTriggerTime << " )" << std::endl;
		ofs << "g_pObjectParticle:SetDrawCount( " << iDrawCount << " )" << std::endl;
		if( true == bCullCheck )
		{
			ofs << "g_pObjectParticle:SetCullCheck( true )" << std::endl;
		}
		else
		{
			ofs << "g_pObjectParticle:SetCullCheck( false )" << std::endl;
		}

		switch( eParticleType )
		{
		case CKTDGParticleSystem::PT_MESH:
			ofs << "g_pObjectParticle:SetParticleType( PARTICLE_TYPE[\"PT_MESH\"] )" << std::endl;
			break;
		case CKTDGParticleSystem::PT_2D_PLANE:
			ofs << "g_pObjectParticle:SetParticleType( PARTICLE_TYPE[\"PT_2D_PLANE\"] )" << std::endl;
			break;
		case CKTDGParticleSystem::PT_3D_PLANE:
			ofs << "g_pObjectParticle:SetParticleType( PARTICLE_TYPE[\"PT_3D_PLANE\"] )" << std::endl;
			break;
		}

		if( mapLayerString.end() != mapLayerString.find( pParticle->GetLayer() ) )
		{
			ofs << "g_pObjectSkinMesh:SetLayer( " << mapLayerString[ pParticle->GetLayer() ] << " )" << std::endl;
		}
		else
		{
			ofs << "g_pObjectSkinMesh:SetLayer( " << "X2_LAYER[\"XL_SKY_WORLD_BASE_0\"]" << " )" << std::endl;
		}


		ofs << std::endl << std::endl;	
	}

	ofs.close();

	ConvertFullPathFileAnsiToUTF8( strFileName, strFileName );
	//ConvertFileAnsiToUTF8( strFileName, strFileName );
}

void CX2ToolMain::ClearUnDoWorld()
{
	for(UINT k=0; k<m_vecUnDoWorld.size(); ++k)
	{
		tUNDOWORLD tUndoWorld = m_vecUnDoWorld[ k ];
		if( tUndoWorld.m_vecUndoARElem.size() >= 1 )
		{
			for(UINT i=0; i<tUndoWorld.m_vecUndoARElem.size(); ++i)
			{
				for(UINT j=0; j<tUndoWorld.m_vecUndoARElem[i].m_vecWorldObject.size(); ++j)
				{
					tUndoWorld.m_vecUndoARElem[i].m_vecWorldObject[j].reset();
				}
			}
		}
		m_vecUnDoWorld.erase( m_vecUnDoWorld.begin() + k );
		--k;
	}
}
void CX2ToolMain::AddUndoWorld(tWorldElemGroup tElem)
{
	tUNDOWORLD undoWorld;
	undoWorld.m_vecUndoElem.push_back(tElem);
	m_vecUnDoWorld.push_back(undoWorld);

	if( m_vecUnDoWorld.size() > 10 )
	{
		tUNDOWORLD tUndoWorld = m_vecUnDoWorld[ 0 ];
		if( tUndoWorld.m_vecUndoARElem.size() >= 1 )
		{
			for(UINT i=0; i<tUndoWorld.m_vecUndoARElem.size(); ++i)
			{
				for(UINT j=0; j<tUndoWorld.m_vecUndoARElem[i].m_vecWorldObject.size(); ++j)
				{
					tUndoWorld.m_vecUndoARElem[i].m_vecWorldObject[j].reset();
				}
			}
		}
		m_vecUnDoWorld.erase( m_vecUnDoWorld.begin() );
	}
}

void CX2ToolMain::AddUndoWorld(tAddRmoveWorldElem tElem)
{
	tUNDOWORLD undoWorld;	
	undoWorld.m_vecUndoARElem.push_back(tElem);
	m_vecUnDoWorld.push_back(undoWorld);

	if( m_vecUnDoWorld.size() > 10 )
	{
		tUNDOWORLD tUndoWorld = m_vecUnDoWorld[ 0 ];
		if( tUndoWorld.m_vecUndoARElem.size() >= 1 )
		{
			for(UINT i=0; i<tUndoWorld.m_vecUndoARElem.size(); ++i)
			{
				for(UINT j=0; j<tUndoWorld.m_vecUndoARElem[i].m_vecWorldObject.size(); ++j)
				{
					tUndoWorld.m_vecUndoARElem[i].m_vecWorldObject[j].reset();
				}
			}
		}
		m_vecUnDoWorld.erase( m_vecUnDoWorld.begin() );
	}
}
void CX2ToolMain::UndoWorld()
{
	if(m_vecUnDoWorld.size() == 0 )
		return;

	tUNDOWORLD undoWorld;
	undoWorld = m_vecUnDoWorld[ m_vecUnDoWorld.size() - 1 ];

	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	
	if( undoWorld.m_vecUndoElem.size() > 0 )
	{
		for( UINT i=0; i<undoWorld.m_vecUndoElem.size(); ++i )
		{
			for( UINT j=0; j<undoWorld.m_vecUndoElem[i].m_vecUndoGroup.size(); ++j )
			{
				tWorldElem undoElem = undoWorld.m_vecUndoElem[i].m_vecUndoGroup[j];

				switch(undoElem.m_eFlag)
				{
				case tWorldElem::UF_INT:
					SetWorldObjectProperty((PROPERTY_ID)undoElem.m_iEid, undoElem.m_iValue, undoElem.m_pWorldObject);
					break;
				case tWorldElem::UF_FLOAT:
					SetWorldObjectProperty((PROPERTY_ID)undoElem.m_iEid, undoElem.m_fValue, undoElem.m_pWorldObject);
					break;
				case tWorldElem::UF_BOOL:
					SetWorldObjectProperty((PROPERTY_ID)undoElem.m_iEid, undoElem.m_bValue, undoElem.m_pWorldObject);
					break;
				case tWorldElem::UF_WSTRING:
					SetWorldObjectProperty((PROPERTY_ID)undoElem.m_iEid, undoElem.m_wstrValue, undoElem.m_pWorldObject);
					break;
				}
			}			
		}
	}
	else if( undoWorld.m_vecUndoARElem.size() > 0 )
	{
		for( UINT i=0; i<undoWorld.m_vecUndoARElem.size(); ++i )
		{
			tAddRmoveWorldElem undoElem = undoWorld.m_vecUndoARElem[i];
			if( undoElem.m_bAdded == true )
			{
				// 추가되었던 것이므로 삭제한다.
				for( UINT j=0; j<undoElem.m_vecWorldObject.size(); ++j)
					pWorld->RemoveWorldObject( undoElem.m_vecWorldObject[j].get() );
			}
			else
			{
				// 삭제되었던 것이므로 새로 추가한다.
				for( UINT j=0; j<undoElem.m_vecWorldObject.size(); ++j)
				{
					pWorld->AddWorldObject4Undo( undoElem.m_vecWorldObject[j] );
				}
			}
		}
	}

	m_vecUnDoWorld.erase( m_vecUnDoWorld.begin() + (m_vecUnDoWorld.size() - 1) );
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->m_DlgWorldEditor.ResetWorldObject();
}

void CX2ToolMain::CreateHeightMap(long x, long y, BYTE h, long iWidht, long iHeight)
{
	int iCubeSize = 100;
	int iCenterX = iWidht / 2;
	int iCenterY = iHeight / 2;
	float fScaledHeight = h * 0.1f;

	float fNewX = static_cast<float>( ( x - iCenterX ) * iCubeSize );
	float fNewY = -1.f * iCubeSize; //(h * iCubeSize );
	float fNewZ = static_cast<float>( ( y - iCenterY ) * iCubeSize );
		
	CX2World *pWorld = g_pX2Game->GetWorld();

	
	CX2WorldObjectMesh *pNewObject = pWorld->CreateObjectMesh();
	string strMeshName = "tool_Box.Y";
	pNewObject->SetXMesh_LUA( strMeshName.c_str() );
	pNewObject->SetLightPos( pWorld->GetLightPos() );
	pNewObject->SetPos( D3DXVECTOR3( fNewX, fNewY, fNewZ ) );	
	pNewObject->GetMatrix().Scale( D3DXVECTOR3(1.f, fScaledHeight, 1.f) );
}

void CX2ToolMain::SetShowWorldObject()
{
	if( m_bEditWorld == false )
		return;

	int iSelectedCount = 0;
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	
	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();

		if( pWorldObject != NULL && pWorldObject->m_bSelected == true )
		{
			pWorldObject->m_bShowObject = !pWorldObject->m_bShowObject;
			++iSelectedCount;
		}
	}

	if( iSelectedCount > 0 )
	{
		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
		pMain->m_DlgWorldEditor.ResetWorldObject(false);
	}
}

void CX2ToolMain::SetFreezingWorldObject()
{
	if( m_bEditWorld == false )
		return;

	int iSelectedCount = 0;
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	
	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();

		if( pWorldObject != NULL && pWorldObject->m_bSelected == true )
		{
			pWorldObject->m_bFreezing = !pWorldObject->m_bFreezing;
			++iSelectedCount;
		}
	}

	if( iSelectedCount > 0 )
	{
		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
		pMain->m_DlgWorldEditor.ResetWorldObject(false);
	}
}

void CX2ToolMain::CreateThumbnail()
{
	if( m_vecThumbnail.size() == 0 )
		return;

	D3DXMATRIX matIden;
	D3DXMATRIX matWorld, matView, matProj;
	bool bOriginalProjcetionMode = g_pKTDXApp->GetDGManager()->GetPerspective();
	float fPerspectiveValue = g_pKTDXApp->GetDGManager()->GetPerspectiveValue();
	g_pKTDXApp->GetDevice()->GetTransform( D3DTS_WORLD, &matWorld );
	g_pKTDXApp->GetDevice()->GetTransform( D3DTS_VIEW, &matView );
	g_pKTDXApp->GetDevice()->GetTransform( D3DTS_PROJECTION, &matProj );
		
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), true );	// Ortho
	//g_pKTDXApp->GetDGManager()->SetPerspectiveValue(10);

	for(UINT i=0; i<m_vecThumbnail.size(); ++i)
	{
		D3DXMatrixIdentity( &matIden );

		m_pRenderTargetTex->BeginRender( true );
		m_pRenderTargetTex->Clear(D3DXCOLOR(0.f, 0.f, 0.f, 0.f));

		wstring wstrMeshName = m_vecThumbnail[i];
		WCHAR wszFilePathOnly[512] = {NULL};
		WCHAR wszFileNameOnly[256] = {NULL};
		DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, wstrMeshName.c_str() );
		
		if( StrStrW(wstrMeshName.c_str(), L".x") != NULL || StrStrW(wstrMeshName.c_str(), L".X") != NULL )		
		{
			// skin mesh
			CKTDXDeviceXSkinMesh *pXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( wszFileNameOnly );
			if( pXSkinMesh != NULL )
			{
				CKTDGXSkinAnim *pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
				
				float fRadius = pXSkinMesh->GetBoundingRadius();				
				float fScale = 1000.f / fRadius;
				D3DXVECTOR3 vCenter = pXSkinMesh->GetCenter();				
				vCenter = vCenter * fScale;
				D3DXVECTOR3 vEye = pXSkinMesh->GetCenter();				
				vEye.z -= 3000;

				D3DXMATRIX ViewMatrix;
				D3DXMatrixLookAtLH( &ViewMatrix, &vEye, &vCenter, &D3DXVECTOR3(0.f,1.f,0.f) );
				g_pKTDXApp->SetViewTransform( &ViewMatrix );				

				g_pKTDXApp->SetWorldTransform( &matIden );
				{
					pXSkinAnim->SetAnimXSkinMesh( pXSkinMesh );
					pXSkinAnim->AddModelXSkinMesh( pXSkinMesh,  NULL, NULL, NULL );
					CKTDGXRenderer::RenderParam *pRenderParam = pXSkinAnim->GetRenderParam();
					pRenderParam->bAlphaBlend = true;
					pRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
					pRenderParam->lightPos = D3DXVECTOR3(10000.f, 10000.f, -10000.f);

					pXSkinAnim->GetMatrix().Scale(D3DXVECTOR3(fScale, fScale, fScale));
					pXSkinAnim->ChangeAnim(0);
					pXSkinAnim->Play(CKTDGXSkinAnim::XAP_ONE_WAIT);
					pXSkinAnim->OnFrameMove(0.f, 0.f);
					pXSkinAnim->OnFrameRender_Draw();					
				}
				
				CKTDGXSkinAnim::DeleteKTDGObject( pXSkinAnim );
			}
			SAFE_CLOSE( pXSkinMesh );
		}
		else
		{
			// static mesh
			CKTDXDeviceXMesh *pXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( wszFileNameOnly );
			if( pXMesh != NULL )
			{
				float fRadius = pXMesh->GetRadius();
				float fScale = 1000.f / fRadius;
				D3DXVECTOR3 vCenter = pXMesh->GetCenter();
				vCenter = vCenter * fScale;
				D3DXVECTOR3 vEye = pXMesh->GetCenter();				
				vEye.z -= 3000;
				
				D3DXMATRIX ViewMatrix;
				D3DXMatrixLookAtLH( &ViewMatrix, &vEye, &vCenter, &D3DXVECTOR3(0.f,1.f,0.f) );
				g_pKTDXApp->SetViewTransform( &ViewMatrix );				

				g_pKTDXApp->SetWorldTransform( &matIden );
				{
					CKTDGXRenderer::RenderParam renderParam;

					renderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;
					renderParam.lightPos = D3DXVECTOR3(10000.f, 10000.f, -10000.f);
					renderParam.bAlphaBlend = true;
					D3DXMATRIX  worldMatrix;
					D3DXMatrixIdentity(&worldMatrix);
										
					D3DXMatrixScaling(&worldMatrix, fScale, fScale, fScale);

					g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *pXMesh, NULL, NULL );
				}
			}

			SAFE_CLOSE( pXMesh );
		}		
		
		g_pKTDXApp->GetDGManager()->SetPerspectiveValue(fPerspectiveValue);
		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), bOriginalProjcetionMode );
		g_pKTDXApp->SetProjectionTransform( &matProj );
		g_pKTDXApp->SetWorldTransform( &matWorld );
		g_pKTDXApp->SetViewTransform( &matView );

		m_pRenderTargetTex->EndRender();

		wstring wstrThumnailImgName = wstrMeshName + L".bmp";
		D3DXSaveTextureToFile(wstrThumnailImgName.c_str(), D3DXIFF_BMP, m_pRenderTargetTex->GetTexture(), NULL );
	}

	m_vecThumbnail.clear();
}

void CX2ToolMain::AddWorldObject( D3DXVECTOR3 vPos )
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain != NULL )
	{
		pMain->m_DlgWorldEditor.AddWorldObject(m_wstrSelectedLib, vPos);
		pMain->m_DlgWorldEditor.m_DlgMeshList.ClearSelected();
		m_bAddObjectByLib = false;
		m_wstrSelectedLib = L"";
	}
}

bool CX2ToolMain::CreateSector()
{
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( pLineMap == NULL )
		return false;

	if(m_bTileRender == false)
		return false;

	bool bResult = true;	

	// 라인정보로 섹터영역을 계산한다.
	int minX, maxX, minY, maxY, minZ, maxZ;
	D3DXVECTOR3 vsp, vep;

	maxX = maxY = maxZ =   -99999;
	minX = minY = minZ =    99999;
	
	CKTDGLineMap::LineData* pLineData = NULL;	

	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		pLineData = pLineMap->AccessLineData(i);

		vsp = pLineData->startPos;
		vep = pLineData->endPos;

		minX = min((int)vsp.x, minX);
		minY = min((int)vsp.y, minY);
		minZ = min((int)vsp.z, minZ);
		maxX = max((int)vsp.x, maxX);
		maxY = max((int)vsp.y, maxY);
		maxZ = max((int)vsp.z, maxZ);

		minX = min((int)vep.x, minX);
		minY = min((int)vep.y, minY);
		minZ = min((int)vep.z, minZ);
		maxX = max((int)vep.x, maxX);
		maxY = max((int)vep.y, maxY);
		maxZ = max((int)vep.z, maxZ);       
	}

	// 주위 여분의 타일을 만들어 둔다.
#if 1 
	//minX -= 500;
	//maxX += 500;
	//minY -= 500;
	maxY += 500;
	//minZ -= 500;
	//maxZ += 500;
#endif

	if(m_pBox != NULL)
		m_pBox->Release();
	m_vecCenter.clear();

	D3DXCreateBox(g_pKTDXApp->GetDevice(), (float)m_iSectorWidth, (float)m_iSectorHeight, (float)m_iSectorDepth, &m_pBox, NULL);

	// Create Sector
	int mapWidth, mapHeight, mapDepth;
	mapWidth = maxX - minX;
	mapHeight = maxY - minY;
	mapDepth = maxZ - minZ;

	int tileXCount = (mapWidth / m_iSectorWidth) + 1; 
	int tileYCount = (mapHeight / m_iSectorHeight) + 1; 
	int tileZCount = (mapDepth / m_iSectorDepth) + 1; 

	if(tileXCount < 0 || tileXCount > 255)
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"X Sector수가 범위를 벗어났습니다.", L"ERROR", MB_OK );
		return false;
	}
	if(tileYCount < 0 || tileYCount > 255)
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"Y Sector수가 범위를 벗어났습니다.", L"ERROR", MB_OK );
		return false;
	}
	if(tileZCount < 0 || tileZCount > 255)
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"Z Sector수가 범위를 벗어났습니다.", L"ERROR", MB_OK );
		return false;
	}

	int sX, sY, sZ, eX, eY, eZ;    
	for(int i=0; i<tileYCount; ++i) // Y
	{
		sY = i * m_iSectorHeight;
		eY = sY + m_iSectorHeight;

		for(int j=0; j<tileZCount; ++j) // Z
		{
			sZ = j * m_iSectorDepth;
			eZ = sZ + m_iSectorDepth;

			for(int k=0; k<tileXCount; ++k) // X
			{
				sX = k * m_iSectorWidth;
				eX = sX + m_iSectorWidth;

				m_vecCenter.push_back(D3DXVECTOR3(minX + sX + (m_iSectorWidth/2.f), minY + sY + (m_iSectorHeight/2.f), minZ + sZ + (m_iSectorDepth/2.f)));                
			}
		}
	}

	return bResult;
}

void CX2ToolMain::OnRenderTile()
{    
	D3DXMATRIX matTrans; 

	g_pKTDXApp->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//for(int i=0; i<2; ++i)
	for(int i=0; i<(int)m_vecCenter.size(); ++i)
	{
		D3DXMatrixTranslation( &matTrans, m_vecCenter[i].x, m_vecCenter[i].y, m_vecCenter[i].z );

		g_pKTDXApp->GetDevice()->SetTransform(D3DTS_WORLD, &matTrans);
		m_pBox->DrawSubset(0);
	}    
	g_pKTDXApp->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

wstring CX2ToolMain::GetSectorInfo()
{
	int minX, maxX, minY, maxY, minZ, maxZ;
	D3DXVECTOR3 vsp, vep;

	maxX = maxY = maxZ = -999999;
	minX = minY = minZ =  999999;

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);

		vsp = pLineData->startPos;
		vep = pLineData->endPos;

		minX = min((int)vsp.x, minX);
		minY = min((int)vsp.y, minY);
		minZ = min((int)vsp.z, minZ);
		maxX = max((int)vsp.x, maxX);
		maxY = max((int)vsp.y, maxY);
		maxZ = max((int)vsp.z, maxZ);

		minX = min((int)vep.x, minX);
		minY = min((int)vep.y, minY);
		minZ = min((int)vep.z, minZ);
		maxX = max((int)vep.x, maxX);
		maxY = max((int)vep.y, maxY);
		maxZ = max((int)vep.z, maxZ);       
	}


	int mapWidth, mapHeight, mapDepth;
	mapWidth = maxX - minX;
	mapHeight = maxY - minY;
	mapDepth = maxZ - minZ;

	int tileXCount = (mapWidth / m_iSectorWidth) + 1; 
	int tileYCount = (mapHeight / m_iSectorHeight) + 1; 
	int tileZCount = (mapDepth / m_iSectorDepth) + 1; 

	WCHAR wszTileInfo[1000];
	wsprintfW(wszTileInfo, L"\r\nTILE_INFO = \r\n{\r\n\tNUM_X = %d,\r\n\tNUM_Y = %d,\r\n\tNUM_Z = %d,\r\n\r\n\tSIZE_X = %d,\r\n\tSIZE_Y = %d,\r\n\tSIZE_Z = %d,\r\n},",
		tileXCount, tileYCount, tileZCount, m_iSectorWidth, m_iSectorHeight, m_iSectorDepth);

	WCHAR wszMapInfo[1000];
	wsprintfW(wszMapInfo, L"\r\n\r\nMAP_INFO = \r\n{\r\n\tMIN_X = %d,\r\n\tMIN_Y = %d,\r\n\tMIN_Z = %d,\r\n\r\n\tMAX_X = %d,\r\n\tMAX_Y = %d,\r\n\tMAX_Z = %d,\r\n},",
		minX, minY, minZ, maxX, maxY, maxZ );

	wstring wstrSectorInfo1 = wszTileInfo;
	wstring wstrSectorInfo2 = wszMapInfo;
	wstring wstrSectorInfo = wstrSectorInfo1 + wstrSectorInfo2;

	return wstrSectorInfo;
}

void CX2ToolMain::InitReadyDungeonNpc()
{
	for(UINT i=0; i<m_vecDungeonSkinMeshNpc.size(); ++i)
		SAFE_CLOSE( m_vecDungeonSkinMeshNpc[i] );
	m_vecDungeonSkinMeshNpc.clear();

	for( UINT i=0; i < m_vecDungeonSkinAnimNpc.size(); i++ )
	{
		//SAFE_DELETE( (CKTDGXSkinAnim*)vecpXSkinAnim[i] );
		SAFE_DELETE_KTDGOBJECT( (CKTDGXSkinAnim*)m_vecDungeonSkinAnimNpc[i] );
	}
	m_vecDungeonSkinAnimNpc.clear();
}
void CX2ToolMain::AddReadyDungeonNpc(CX2UnitManager::NPC_UNIT_ID npcId)
{	
	CX2UnitManager::NPCUnitTemplet* pNpcTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet(npcId);
	if( pNpcTemplet == NULL )
	{
		m_vecDungeonSkinMeshNpc.push_back(NULL);
		m_vecDungeonSkinAnimNpc.push_back(NULL);

		return;
	}

	wstring wstrNpcMeshName = pNpcTemplet->m_wstrMotionName;
	if( wstrNpcMeshName.empty() == true )
	{
		wstrNpcMeshName = L"tool_Npc_Zero.x";
	}

	CKTDXDeviceXSkinMesh* pSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( wstrNpcMeshName.c_str() );
	if( pSkinMesh != NULL )
	{
		CKTDGXSkinAnim* pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();

		pXSkinAnim->SetAnimXSkinMesh(  pSkinMesh );
		pXSkinAnim->AddModelXSkinMesh( pSkinMesh );
		pXSkinAnim->SetAlphaObject( true );
		pXSkinAnim->ChangeAnim( L"Wait", false );
		pXSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );
		pXSkinAnim->SetCenter( pSkinMesh->GetCenter() );
		pXSkinAnim->SetBoundingRadius( pSkinMesh->GetBoundingRadius() );
		
		m_vecDungeonSkinMeshNpc.push_back(pSkinMesh);
		m_vecDungeonSkinAnimNpc.push_back(pXSkinAnim);
	}
	else
	{
		m_vecDungeonSkinMeshNpc.push_back(NULL);
		m_vecDungeonSkinAnimNpc.push_back(NULL);
	}
}
void CX2ToolMain::RemoveReadyDungeonNpc(int iIndex)
{
	SAFE_CLOSE( m_vecDungeonSkinMeshNpc[iIndex] );
	SAFE_DELETE_KTDGOBJECT( (CKTDGXSkinAnim*)m_vecDungeonSkinAnimNpc[iIndex] );

	m_vecDungeonSkinMeshNpc.erase( m_vecDungeonSkinMeshNpc.begin() + iIndex );
	m_vecDungeonSkinAnimNpc.erase( m_vecDungeonSkinAnimNpc.begin() + iIndex );
}

void CX2ToolMain::ChangeUnit()
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain != NULL )
	{
		pMain->m_DlgSelectUnit.ShowWindow(SW_SHOW);
	}	
}

#pragma region OnFrameRenderLineInfo
/** @function 	: OnFrameRenderLineInfo
	@brief 		: 라인 정보 출력
*/
void CX2ToolMain::OnFrameRenderLineInfo()
{
	// 라인정보 출력
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	for ( int i = 0; i < pLineMap->GetNumLineData(); ++i )
	{
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData( i );
		
		if( pLineData != NULL )
		{
			D3DXVECTOR3 vScreen;
			D3DXVECTOR3 vPos = pLineData->startPos; //(pLineData->endPos - pLineData->startPos) * 0.5f;
			vPos.x += 30.f;
			vPos.y += 30.f;
			WCHAR wBuf[100] = { 0, };

			if ( m_bShowLineNumber )
				wsprintf( wBuf, L"%d", pLineData->lineIndex );

			if ( m_bShowLineSetNumber )
			{
				WCHAR wBufTemp[10];
				wsprintf( wBufTemp, L"(%d)", pLineData->lineSet );
				StrCatW(wBuf, wBufTemp);
			}

			if( pLineData->m_iCameraIndex >= 0 && m_bShowLineCameraNumber )
			{
				WCHAR wBufTemp[10];
				wsprintfW( wBufTemp, L"[C%d]", pLineData->m_iCameraIndex);
				StrCatW(wBuf, wBufTemp);
			}
			if( pLineData->m_iTriggerId >= 0 && m_bShowLineTriggerNumber )
			{
				WCHAR wBufTemp[10];
				wsprintfW( wBufTemp, L"[T%d]", pLineData->m_iTriggerId);
				StrCatW(wBuf, wBufTemp);
			}
			if( pLineData->m_bUnderWater == true && m_bShowInnerWaterInfo )
			{
				StrCatW(wBuf, L"[W]");
				//if( m_bShowLineMapModifier == true )
				{
					D3DXVECTOR3 p[2];
					p[0].x = pLineData->startPos.x; 
					p[0].y = pLineData->m_fWaterHeight; 
					p[0].z = pLineData->startPos.z;
					p[1].x = pLineData->endPos.x; 
					p[1].y = pLineData->m_fWaterHeight; 
					p[1].z = pLineData->endPos.z;

					LPD3DXLINE Line;
					D3DXCreateLine( g_pKTDXApp->GetDevice(), &Line);
					Line->SetWidth(3);				
					Line->Begin();
					D3DXMATRIX matTransform, matTransform1, matTransform2;
					D3DXMatrixIdentity(&matTransform);
					g_pKTDXApp->GetWorldTransform(&matTransform);
					g_pKTDXApp->GetViewTransform(&matTransform1);
					g_pKTDXApp->GetProjectionTransform(&matTransform2);

					Line->DrawTransform(p, 2, &(matTransform * matTransform1* matTransform2), D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f));
					Line->End();
					Line->Release();
				}					
			}

			//if( m_bShowLineMapModifier == true )
			{						
				if( pLineData->beforeLine == -1 )
				{
					WCHAR wTemp[10];
					wsprintf(wTemp, L"▲");
					D3DXVECTOR3 vPosTemp = pLineData->startPos;
					//vPosTemp.x -= 3.f;
					vPosTemp.y -= 10.f;
					m_pFont_A->OutProjectionText( vPosTemp, wTemp, 0xff0000ff, CKTDGFontManager::FS_SHELL, 0xffffff00, NULL, DT_CENTER );					
				}
				else if( pLineData->beforeLine == -2 )
				{
					WCHAR wTemp[10];
					wsprintf(wTemp, L"▲");
					D3DXVECTOR3 vPosTemp = pLineData->startPos;
					//vPosTemp.x -= 3.f;
					vPosTemp.y -= 10.f;
					m_pFont_A->OutProjectionText( vPosTemp, wTemp, 0xffff0000, CKTDGFontManager::FS_SHELL, 0xff00ffff, NULL, DT_CENTER );					
				}

				if( pLineData->nextLine == -1)
				{
					WCHAR wTemp[10];
					wsprintf(wTemp, L"▲");
					D3DXVECTOR3 vPosTemp = pLineData->endPos;
					//vPosTemp.x += 3.f;
					vPosTemp.y -= 10.f;
					m_pFont_A->OutProjectionText( vPosTemp, wTemp, 0xff0000ff, CKTDGFontManager::FS_SHELL, 0xffffff00, NULL, DT_CENTER );					
				}
				else if( pLineData->nextLine == -2)
				{
					WCHAR wTemp[10];
					wsprintf(wTemp, L"▲");
					D3DXVECTOR3 vPosTemp = pLineData->endPos;
					//vPosTemp.x += 3.f;
					vPosTemp.y -= 10.f;
					m_pFont_A->OutProjectionText( vPosTemp, wTemp, 0xffff0000, CKTDGFontManager::FS_SHELL, 0xff00ffff, NULL, DT_CENTER );					
				}
				//
				//if( pLineData->nextLine == -1 )
				//{
				//	m_pSphereAnim->GetMatrix().Move( pLineData->endPos );
				//	m_pSphereAnim->GetMatrix().Scale( D3DXVECTOR3(0.1f, 0.1f, 0.1f) );
				//	m_pSphereAnim->GetRenderParam()->color = D3DXCOLOR(0, 0, 1, 1);
				//	m_pSphereAnim->OnFrameRender();
				//}
				//else if( pLineData->nextLine == -2 )
				//{
				//	m_pSphereAnim->GetMatrix().Move( pLineData->endPos );
				//	m_pSphereAnim->GetMatrix().Scale( D3DXVECTOR3(0.1f, 0.1f, 0.1f) );
				//	m_pSphereAnim->GetRenderParam()->color = D3DXCOLOR(1, 0, 0, 1);
				//	m_pSphereAnim->OnFrameRender();
				//}

				if( pLineData->m_bSelected == true )
				{
					m_pFont_A->OutProjectionText( vPos, wBuf, 0xffff0000, CKTDGFontManager::FS_SHELL, 0xff000000, NULL, DT_LEFT );					
				}
				else
				{
					m_pFont_A->OutProjectionText( vPos, wBuf, 0xffffff00, CKTDGFontManager::FS_SHELL, 0xff000000, NULL, DT_LEFT );
				}
			}
		}			
	}
}
#pragma endregion 라인 정보 출력
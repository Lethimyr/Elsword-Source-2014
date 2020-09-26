// EditWorld.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "MainFrm.h"
#include "EditWorld.h"
#include "afxdialogex.h"


// CEditWorld 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditWorld, CDialogEx)

CEditWorld::CEditWorld(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditWorld::IDD, pParent)
{
	m_bResetFrustum = false;
	m_fNear = 1.f;
	m_fFar = 50000.f;
	m_fPerspective = 4.f;

	m_fBaseCamera = 0.f;
	m_fCamera1 = 0.f;
	m_fCamera2 = 0.f;
	m_fCamera3 = 0.f;

	
	m_bCheckList = false;		
}

CEditWorld::~CEditWorld()
{
}

void CEditWorld::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WORLDOBJECT, m_ctrlListWorldObject);
	DDX_Control(pDX, IDC_LIST_EDIT_WORLDLAYER, m_ctrlListWorldLayer);
	DDX_Control(pDX, IDC_EDIT_WORLD_NEAR, m_editNear);
	DDX_Control(pDX, IDC_EDIT_WORLD_FAR, m_editFar);
	DDX_Control(pDX, IDC_EDIT_WORLD_PERSPECTIVE, m_editPerspective);
	DDX_Control(pDX, IDC_EDIT_WORLD_BASECAMERA, m_editBaseCamera);
	DDX_Control(pDX, IDC_EDIT_WORLD_CAMERA1, m_editCamera1);
	DDX_Control(pDX, IDC_EDIT_WORLD_CAMERA2, m_editCamera2);
	DDX_Control(pDX, IDC_EDIT_WORLD_CAMERA3, m_editCamera3);
	DDX_Control(pDX, IDC_LIST_EDIT_SKYDOME, m_ctrlSkyDomList);
}


BEGIN_MESSAGE_MAP(CEditWorld, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_EDIT_WORLD_NEAR, &CEditWorld::OnEnChangeEditWorldNear)
	ON_EN_CHANGE(IDC_EDIT_WORLD_FAR, &CEditWorld::OnEnChangeEditWorldFar)
	ON_EN_CHANGE(IDC_EDIT_WORLD_PERSPECTIVE, &CEditWorld::OnEnChangeEditWorldPerspective)
	ON_EN_CHANGE(IDC_EDIT_WORLD_BASECAMERA, &CEditWorld::OnEnChangeEditWorldBasecamera)
	ON_EN_CHANGE(IDC_EDIT_WORLD_CAMERA1, &CEditWorld::OnEnChangeEditWorldCamera1)
	ON_EN_CHANGE(IDC_EDIT_WORLD_CAMERA2, &CEditWorld::OnEnChangeEditWorldCamera2)
	ON_EN_CHANGE(IDC_EDIT_WORLD_CAMERA3, &CEditWorld::OnEnChangeEditWorldCamera3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_EDIT_WORLDLAYER, &CEditWorld::OnLvnItemchangedListEditWorldlayer)
	ON_BN_CLICKED(IDC_BUTTON_ADD_WORLDLAYER, &CEditWorld::OnBnClickedButtonAddWorldlayer)
	ON_BN_CLICKED(IDC_BUTTON1, &CEditWorld::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEditWorld::OnBnClickedButton2)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_EDIT_WORLDLAYER, &CEditWorld::OnNMSetfocusListEditWorldlayer)
	ON_BN_CLICKED(IDC_BUTTON_ADD_SKYDOME, &CEditWorld::OnBnClickedButtonAddSkydome)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_EDIT_SKYDOME, &CEditWorld::OnLvnItemchangedListEditSkydome)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_EDIT_SKYDOME, &CEditWorld::OnNMSetfocusListEditSkydome)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_WORLDOBJECT, &CEditWorld::OnNMSetfocusListWorldobject)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WORLDOBJECT, &CEditWorld::OnLvnItemchangedListWorldobject)
	ON_BN_CLICKED(IDC_BUTTON_ADD_WORLDOBJECT, &CEditWorld::OnBnClickedButtonAddWorldobject)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PARTICLE, &CEditWorld::OnBnClickedButtonAddParticle)	
	ON_NOTIFY(LVN_ODSTATECHANGED, IDC_LIST_WORLDOBJECT, &CEditWorld::OnLvnOdstatechangedListWorldobject)		
	ON_BN_CLICKED(IDC_BUTTON4, &CEditWorld::OnBnClickedButton4)
END_MESSAGE_MAP()


// CEditWorld 메시지 처리기입니다.
void CEditWorld::OnDropFiles(HWND hWnd, HDROP hDropInfo)
{
	int iEditKind = -1;

	if( m_ctrlListWorldLayer.GetSafeHwnd() == hWnd )
		iEditKind = 0;
	else if( m_ctrlSkyDomList.GetSafeHwnd() == hWnd )
		iEditKind = 1;
	else if( m_ctrlListWorldObject.GetSafeHwnd() == hWnd )
		iEditKind = 2;

	if( iEditKind == -1 )
		return;

	WCHAR wBuf[MAX_PATH];
	UINT uDropCount = ::DragQueryFile( hDropInfo, 0xffffffff, wBuf, MAX_PATH );

	tAddRmoveWorldElem tUndoWorldElem;
	tUndoWorldElem.m_bAdded = true;

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	for( UINT i=0; i<uDropCount; ++i )
	{
		::DragQueryFile( hDropInfo, i, wBuf, MAX_PATH );

		// drop 된 파일 처리		
		if( pMain != NULL )
		{			
			CX2WorldObject *pAddWorldObject = NULL;

			wstring wFile = wBuf;
			MakeUpperCase(wBuf);

			WCHAR wszFilePathOnly[512] = {NULL};
			WCHAR wszFileNameOnly[256] = {NULL};
			DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, wFile.c_str() );

			g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory(wszFilePathOnly);

			switch( iEditKind )
			{
			case 0:
				{
					// world layer
					if( StrStrW(wBuf, L".DDS") != NULL || StrStrW(wBuf, L".TGA") != NULL )
					{
						// world object
						CX2WorldLayer *pLayer = g_pX2Game->GetWorld()->CreateWorldLayer();
						if( pLayer != NULL )
						{
							pLayer->SetTexW(wszFileNameOnly);
						}

						ResetWorldLayer();

						CX2World *pWorld = g_pX2Game->GetWorld();
						vector<CX2WorldLayer*> vecWorldlayer = pWorld->GetWorldLayerList();

						for(UINT i=0; i<vecWorldlayer.size(); ++i)
						{									
							CX2WorldLayer *pWorldLayer = vecWorldlayer[i];
							if( pWorldLayer != NULL )
							{
								if(i == vecWorldlayer.size() - 1)
								{
									pWorldLayer->m_bSelected = true;
									m_ctrlListWorldLayer.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
									m_ctrlListWorldLayer.EnsureVisible(i, TRUE);
									m_ctrlListWorldLayer.SetFocus();
								}
								else
								{
									m_ctrlListWorldLayer.SetItemState(i, 0, LVIS_SELECTED );
								}			
							}
						}
					}
					else
					{
						AfxMessageBox(L"WorldLayer는 (dds, tga) 파일만 지원합니다.");
					}
				}
				break;
			case 1:
				{
					// world skydome
					if( StrStrW(wBuf, L".Y") != NULL )
					{						
						CX2World *pWorld = g_pX2Game->GetWorld();
						CKTDGSkyDome *pSkyDome = pWorld->CreateSkyDome();
						if( pSkyDome != NULL )
						{
							pSkyDome->SetDomeMeshW(wszFileNameOnly);
						}

						ResetWorldSkyDome();

						vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();

						for(UINT i=0; i<vecSkyDome.size(); ++i)
						{									
							CKTDGSkyDome *pSkyDome = vecSkyDome[i];
							if( pSkyDome != NULL )
							{
								if(i == vecSkyDome.size() - 1)
								{
									pSkyDome->m_bSelected = true;
									m_ctrlSkyDomList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
									m_ctrlSkyDomList.EnsureVisible(i, TRUE);
									m_ctrlSkyDomList.SetFocus();
								}
								else
								{
									m_ctrlSkyDomList.SetItemState(i, 0, LVIS_SELECTED );
								}			
							}
						}
					}
					else
					{
						AfxMessageBox(L"SkyDome은 (Static Mesh) 파일만 지원합니다.");
					}
				}
				break;
			case 2:
				{
					// world object
					if( StrStrW(wBuf, L".X") != NULL || StrStrW(wBuf, L".Y") != NULL )
					{
						bool bStaticMesh = true;
						if( StrStrW(wBuf, L".X") != NULL )
							bStaticMesh = false;

						CX2World *pWorld = g_pX2Game->GetWorld();
						if( bStaticMesh == true )
						{
							CX2WorldObjectMesh *pObject = pWorld->CreateObjectMesh();
							string strMeshName = "";
							ConvertWCHARToChar(strMeshName, wszFileNameOnly);
							pObject->SetXMesh_LUA(strMeshName.c_str());
							pObject->SetLayer( (int)XL_SKY_WORLD_BASE_0 );
							pObject->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
							pObject->SetOriginalColorFactor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
							pObject->SetColorFactor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
							pObject->SetCanHide(false, false);
							pObject->SetNotRenderWhenHidden(false);
							pObject->SetAlphaObject(false);
							pObject->GetRenderParam()->bAlphaBlend = false;
							pObject->GetRenderParam()->bAlphaTestEnable = false;
							if( pWorld->m_bFog == true && pWorld->m_bFogShow == true )
							{
								pObject->SetFog( pWorld->m_fNearX, pWorld->m_fFarX, pWorld->m_fNearY, pWorld->m_fFarY, pWorld->m_FogColor, pWorld->m_fDensity );
							}
							
							pAddWorldObject = (CX2WorldObject *)pObject;
						}
						else
						{
							CX2WorldObjectSkinMesh *pObject = pWorld->CreateObjectSkinMesh();
							string strMeshName = "";
							ConvertWCHARToChar(strMeshName, wszFileNameOnly);
							pObject->SetXSkinMotion_LUA(strMeshName.c_str());
							pObject->AddXSkinModel_LUA(strMeshName.c_str());

							vector<wstring> vecAnimList;
							pObject->GetXSkinAnim()->GetAnimNameList(vecAnimList);
							string strAnimName = "";
							ConvertWCHARToChar(strAnimName, vecAnimList[0].c_str());
							pObject->AddAnim_LUA(strAnimName.c_str());
							pObject->PlayAnim_LUA( CX2WorldObjectSkinMesh::WOSMPY_LOOP, true );
							pObject->SetLayer( (int)XL_SKY_WORLD_BASE_0 );
							pObject->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
							pObject->SetOriginalColorFactor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
							pObject->SetColorFactor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
							pObject->SetCanHide(false, false);
							pObject->SetAlphaObject(false);
							pObject->GetRenderParam()->bAlphaBlend = false;
							pObject->GetRenderParam()->bAlphaTestEnable = false;
							if( pWorld->m_bFog == true && pWorld->m_bFogShow == true )
							{
								pObject->SetFog( pWorld->m_fNearX, pWorld->m_fFarX, pWorld->m_fNearY, pWorld->m_fFarY, pWorld->m_FogColor, pWorld->m_fDensity );
							}
							
							pAddWorldObject = (CX2WorldObject *)pObject;
						}

						ResetWorldObject(false);
						if( pAddWorldObject != NULL )
							pAddWorldObject->m_bSelected = true;

						CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
						vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
						for(UINT j=0; j<vecWorldObjectList.size(); ++j)
						{
							CX2WorldObject *pWorldObject = vecWorldObjectList[j].get();
							
							if( pMain != NULL )
								pMain->m_DlgWorldEditor.SetCurSel( j, pWorldObject->m_bSelected );
						}
					}
					else
					{
						AfxMessageBox(L"WorldObject는 (Skin Mesh, Static Mesh) 파일만 지원합니다.");
					}
				}
				break;
			default:
				break;
			}

			// world undo
			{
				CX2World *pWorld = g_pX2Game->GetWorld();
				vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
				for(UINT j=0; j<vecWorldObjectList.size(); ++j)
				{
					if(vecWorldObjectList[j].get() == pAddWorldObject)
					{
						tUndoWorldElem.m_vecWorldObject.push_back(vecWorldObjectList[j]);
						break;
					}
				}
			}			
		}
	}

	if( tUndoWorldElem.m_vecWorldObject.size() >= 1 )
		pMain->AddUndoWorld(tUndoWorldElem);

	::DragFinish( hDropInfo );
}

void CEditWorld::SetCurSel(int iIndex, bool bCheck)
{
	
	if( bCheck == true)
	{
		m_ctrlListWorldObject.SetItemState(iIndex, LVIS_SELECTED, LVIS_SELECTED );
		m_ctrlListWorldObject.EnsureVisible(iIndex, TRUE);
		//m_ctrlListWorldObject.SetFocus();
	}
	else
	{
		m_ctrlListWorldObject.SetItemState(iIndex, 0, LVIS_SELECTED );
	}	

	m_bCheckList = true;
}

wstring CEditWorld::GetWorldLayerString(int iLayer)
{
	wstring wstrObjectLayer = L"";

	switch( (X2_LAYER)iLayer )
	{
	case XL_GAME_BASE:
		wstrObjectLayer = L"GAME_BASE";
		break;
	case XL_SKY_DOME_0:
		wstrObjectLayer = L"SKY_DOME_0";
		break;
	case XL_SKY_DOME_1:
		wstrObjectLayer = L"SKY_DOME_1";
		break;
	case XL_SKY_DOME_2:
		wstrObjectLayer = L"SKY_DOME_2";
		break;
	case XL_SKY_WORLD_BASE_0:
		wstrObjectLayer = L"SKY_WORLD_BASE_0";
		break;
	case XL_SKY_WORLD_BASE_1:
		wstrObjectLayer = L"SKY_WORLD_BASE_1";
		break;
	case XL_SKY_WORLD_BASE_2:
		wstrObjectLayer = L"SKY_WORLD_BASE_2";
		break;
	case XL_SKY_WORLD_OBJECT_0:
		wstrObjectLayer = L"SKY_WORLD_OBJECT_0";
		break;
	case XL_SKY_WORLD_OBJECT_1:
		wstrObjectLayer = L"SKY_WORLD_OBJECT_1";
		break;
	case XL_SKY_WORLD_OBJECT_2:
		wstrObjectLayer = L"SKY_WORLD_OBJECT_2";
		break;
	case XL_UNIT_0:
		wstrObjectLayer = L"UNIT_0";
		break;
	case XL_UNIT_1:
		wstrObjectLayer = L"UNIT_1";
		break;
	case XL_UNIT_2:
		wstrObjectLayer = L"UNIT_2";
		break;
	case XL_EFFECT_0:
		wstrObjectLayer = L"EFFECT_0";
		break;
	case XL_EFFECT_1:
		wstrObjectLayer = L"EFFECT_1";
		break;
	case XL_EFFECT_2:
		wstrObjectLayer = L"EFFECT_2";
		break;
	case XL_SKY_WORLD_OBJECT_OVER_EFFECT_0:
		wstrObjectLayer = L"SKY_WORLD_OBJECT_OVER_EFFECT_0";
		break;
	case XL_SKY_WORLD_OBJECT_OVER_EFFECT_1:
		wstrObjectLayer = L"SKY_WORLD_OBJECT_OVER_EFFECT_1";
		break;
	case XL_SKY_WORLD_OBJECT_OVER_EFFECT_2:
		wstrObjectLayer = L"SKY_WORLD_OBJECT_OVER_EFFECT_2";
		break;
	case XL_SKY_WORLD_OBJECT_OVER_EFFECT_3:
		wstrObjectLayer = L"SKY_WORLD_OBJECT_OVER_EFFECT_3";
		break;
	case XL_EFFECT_OVER_ALL_WORLD_OBJECT_0:
		wstrObjectLayer = L"EFFECT_OVER_ALL_WORLD_OBJECT_0";
		break;
	case XL_EFFECT_OVER_ALL_WORLD_OBJECT_1:
		wstrObjectLayer = L"EFFECT_OVER_ALL_WORLD_OBJECT_1";
		break;
	case XL_LENS_FLARE:
		wstrObjectLayer = L"LENS_FLARE";
		break;
	case XL_MINIMAP:
		wstrObjectLayer = L"MINIMAP";
		break;
	default:
		break;
	}

	return wstrObjectLayer;
}

void CEditWorld::ResetWorldFrustum()
{
	if( g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;
	
	m_bResetFrustum = true;
	// 
	m_fNear = g_pKTDXApp->GetDGManager()->GetNear();
	m_fFar = g_pKTDXApp->GetDGManager()->GetFar();
	m_fPerspective = g_pKTDXApp->GetDGManager()->GetPerspectiveValue();

	m_fBaseCamera = pWorld->GetBaseCameraDistance();
	m_fCamera1 = pWorld->GetCameraStepDistacne(0);
	m_fCamera2 = pWorld->GetCameraStepDistacne(1);
	m_fCamera3 = pWorld->GetCameraStepDistacne(2);

	char buf[100];
	wstring wstrFrustum = L"";

	sprintf(buf, "%5.2f", m_fNear);	
	ConvertCharToWCHAR( wstrFrustum, buf );
	GetDlgItem(IDC_EDIT_WORLD_NEAR)->SetWindowTextW( wstrFrustum.c_str() );

	sprintf(buf, "%5.2f", m_fFar);	
	ConvertCharToWCHAR( wstrFrustum, buf );
	GetDlgItem(IDC_EDIT_WORLD_FAR)->SetWindowTextW(wstrFrustum.c_str());

	sprintf(buf, "%5.2f", m_fPerspective);	
	ConvertCharToWCHAR( wstrFrustum, buf );
	GetDlgItem(IDC_EDIT_WORLD_PERSPECTIVE)->SetWindowTextW(wstrFrustum.c_str());

	sprintf(buf, "%5.2f", m_fBaseCamera);	
	ConvertCharToWCHAR( wstrFrustum, buf );
	GetDlgItem(IDC_EDIT_WORLD_BASECAMERA)->SetWindowTextW(wstrFrustum.c_str());

	sprintf(buf, "%5.2f", m_fCamera1);	
	ConvertCharToWCHAR( wstrFrustum, buf );
	GetDlgItem(IDC_EDIT_WORLD_CAMERA1)->SetWindowTextW(wstrFrustum.c_str());

	sprintf(buf, "%5.2f", m_fCamera2);	
	ConvertCharToWCHAR( wstrFrustum, buf );
	GetDlgItem(IDC_EDIT_WORLD_CAMERA2)->SetWindowTextW(wstrFrustum.c_str());

	sprintf(buf, "%5.2f", m_fCamera3);	
	ConvertCharToWCHAR( wstrFrustum, buf );
	GetDlgItem(IDC_EDIT_WORLD_CAMERA3)->SetWindowTextW(wstrFrustum.c_str());

	m_bResetFrustum = false;
}
void CEditWorld::ResetWorldLayer(bool bReset)
{
	if( g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;
	
	int iScrollPos = m_ctrlListWorldLayer.GetScrollPos(SB_VERT);
		
	m_ctrlListWorldLayer.DeleteColumn(2);
	m_ctrlListWorldLayer.DeleteColumn(1);
	m_ctrlListWorldLayer.DeleteColumn(0);	

	m_ctrlListWorldLayer.DeleteAllItems();

	m_ctrlListWorldLayer.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 30);
	m_ctrlListWorldLayer.InsertColumn(1, _T("Layer Texture Name"), LVCFMT_LEFT, 220 );
	m_ctrlListWorldLayer.InsertColumn(2, _T("Layer"), LVCFMT_LEFT, 210 );

	vector<CX2WorldLayer*> vecWorldLayer = pWorld->GetWorldLayerList();
	for(UINT i=0; i<vecWorldLayer.size(); ++i)
	{
		int iLayer = 0;
		wstring wstrTexName = L"";
		wstring wstrLayerLayer = L"";
		CX2WorldLayer *pWorldLayer = vecWorldLayer[i];
		if( pWorldLayer != NULL )
		{
			wstrTexName = pWorldLayer->GetTexName();
			iLayer = pWorldLayer->GetLayer();
			wstrLayerLayer = GetWorldLayerString(iLayer);
			pWorldLayer->SetLayerId( i );

			if( bReset == true )
				pWorldLayer->m_bSelected = false;
		}

		WCHAR wbuf[10];
		wsprintfW(wbuf, L"%d", i);
		m_ctrlListWorldLayer.InsertItem( i, wbuf );
		m_ctrlListWorldLayer.SetItemText( i, 1, wstrTexName.c_str() );
		m_ctrlListWorldLayer.SetItemText( i, 2, wstrLayerLayer.c_str() );
	}

	m_ctrlListWorldLayer.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

	UpdateData(TRUE);

	for(UINT i=0; i<vecWorldLayer.size(); ++i)
	{
		CX2WorldLayer *pWorldLayer = vecWorldLayer[i];
		if( pWorldLayer != NULL && pWorldLayer->m_bSelected == true )
		{
			m_ctrlListWorldLayer.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
			m_ctrlListWorldLayer.EnsureVisible(i, TRUE);
		}
	}

	int iNum(-1);
	POSITION pos = m_ctrlListWorldLayer.GetFirstSelectedItemPosition();
	iNum = m_ctrlListWorldLayer.GetNextSelectedItem(pos);

	CRect ref;
	m_ctrlListWorldLayer.GetItemRect(0, ref, LVIR_BOUNDS);
	CSize szHeight(0, ref.Height() * iScrollPos);
	m_ctrlListWorldLayer.Scroll(szHeight);
	m_ctrlListWorldLayer.SetScrollPos(SB_VERT, iScrollPos, FALSE);

}
void CEditWorld::ResetWorldSkyDome(bool bReset)
{
	if( g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;

	int iScrollPos = m_ctrlSkyDomList.GetScrollPos(SB_VERT);

	m_ctrlSkyDomList.DeleteColumn(2);
	m_ctrlSkyDomList.DeleteColumn(1);
	m_ctrlSkyDomList.DeleteColumn(0);

	m_ctrlSkyDomList.DeleteAllItems();

	m_ctrlSkyDomList.InsertColumn(0, _T("SkyDome Name"), LVCFMT_CENTER, 300 );
	m_ctrlSkyDomList.InsertColumn(1, _T("Layer"), LVCFMT_CENTER, 150 );
	m_ctrlSkyDomList.InsertColumn(2, _T("Show"), LVCFMT_CENTER, 50 );

	D3DXCOLOR vWorldFogColor = g_pX2Game->GetWorld()->m_FogColor;

	vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();
	for(UINT i=0; i<vecSkyDome.size(); ++i)
	{
		wstring wstrDeviceName = L"";
		wstring wstrObjectLayer = L"";

		CKTDGSkyDome *pSkyDome = vecSkyDome[i];
		if( pSkyDome != NULL )
		{
			int iLayer = pSkyDome->GetLayer();			
			wstrDeviceName = pSkyDome->GetName();
			wstrObjectLayer = GetWorldLayerString(iLayer);

			D3DXCOLOR vColor = pSkyDome->GetRenderParam()->fogColor;
			if( IsSamef(vWorldFogColor.r, vColor.r) == false || IsSamef(vWorldFogColor.g, vColor.g) == false || IsSamef(vWorldFogColor.b, vColor.b) == false )
				pSkyDome->m_bUseObjectFogProperty = true;


			if(bReset == true)
				pSkyDome->m_bSelected = false;
		}
		m_ctrlSkyDomList.InsertItem( i, wstrDeviceName.c_str() );
		m_ctrlSkyDomList.SetItemText( i, 1, wstrObjectLayer.c_str() );
		if(pSkyDome->m_bShowObject == true )
			m_ctrlSkyDomList.SetItemText( i, 2, L"Show" );
		else
			m_ctrlSkyDomList.SetItemText( i, 2, L"" );
	}

	m_ctrlSkyDomList.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

	UpdateData(TRUE);

	for(UINT i=0; i<vecSkyDome.size(); ++i)
	{
		CKTDGSkyDome *pSkyDome = vecSkyDome[i];
		if( pSkyDome != NULL && pSkyDome->m_bSelected == true )
		{
			m_ctrlSkyDomList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
			m_ctrlSkyDomList.EnsureVisible(i, TRUE);
		}
	}

	int iNum(-1);
	POSITION pos = m_ctrlSkyDomList.GetFirstSelectedItemPosition();
	iNum = m_ctrlSkyDomList.GetNextSelectedItem(pos);

	CRect ref;
	m_ctrlSkyDomList.GetItemRect(0, ref, LVIR_BOUNDS);
	CSize szHeight(0, ref.Height() * iScrollPos);
	m_ctrlSkyDomList.Scroll(szHeight);
	m_ctrlSkyDomList.SetScrollPos(SB_VERT, iScrollPos, FALSE);

}
void CEditWorld::ResetWorldObject(bool bReset)
{
	if( g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;
	
	int iScrollPos = m_ctrlListWorldObject.GetScrollPos(SB_VERT);

	m_ctrlListWorldObject.DeleteColumn(4);
	m_ctrlListWorldObject.DeleteColumn(3);
	m_ctrlListWorldObject.DeleteColumn(2);
	m_ctrlListWorldObject.DeleteColumn(1);
	m_ctrlListWorldObject.DeleteColumn(0);

	m_ctrlListWorldObject.DeleteAllItems();

	m_ctrlListWorldObject.InsertColumn(0, _T("Object Name"), LVCFMT_CENTER, 300 );
	m_ctrlListWorldObject.InsertColumn(1, _T("Layer"), LVCFMT_CENTER, 150 );
	m_ctrlListWorldObject.InsertColumn(2, _T("Type"), LVCFMT_CENTER, 50 );
	m_ctrlListWorldObject.InsertColumn(3, _T("Show"), LVCFMT_CENTER, 50 );
	m_ctrlListWorldObject.InsertColumn(4, _T("Mode"), LVCFMT_CENTER, 70 );

	pWorld->SortOnetime();

	D3DXCOLOR vWorldFogColor = g_pX2Game->GetWorld()->m_FogColor;

	//
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		wstring wstrObjectName = L"";
		wstring wstrObjectType = L"";
		int	iObjectlayer = 0;

		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( pWorldObject != NULL )
		{
			wstrObjectName = pWorldObject->GetName();
			wstrObjectType = L"Invalid";
			iObjectlayer = pWorldObject->GetLayer();

			switch( pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh *pStaticMesh = (CX2WorldObjectMesh*)pWorldObject;
					wstrObjectName = pStaticMesh->GetMesh()->GetDeviceID();
					wstrObjectType = L"Static";
				}
				break;
			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh *pSkinMesh = (CX2WorldObjectSkinMesh*)pWorldObject;
					wstrObjectName = pSkinMesh->GetXSkinMeshName();
					wstrObjectType = L"Skin";
				}
				break;
			case CX2WorldObject::OT_PARTICLE:
				{
					CX2WorldObjectParticle *pParticleObject = (CX2WorldObjectParticle *)pWorldObject;
					wstrObjectName = pParticleObject->GetParticleName();
					wstrObjectType = L"particle";
				}
				break;
			case CX2WorldObject::OT_LIGHT_FLOW:
				{
					CX2WorldObjectLightFlow *pLightFlowMesh = (CX2WorldObjectLightFlow*)pWorldObject;
					wstrObjectName = pLightFlowMesh->GetXMeshName();					
					wstrObjectType = L"LightFlow";
				}
				break;
			case CX2WorldObject::OT_REACT_MESH:
				{
					wstrObjectType = L"ReactMesh";
				}
				break;
			default:
				{
					wstrObjectType = L"Unknown";
				}
				break;
			}			
		}
		wstring wstrObjectLayer = GetWorldLayerString(iObjectlayer);

		m_ctrlListWorldObject.InsertItem( i, wstrObjectName.c_str() );
		m_ctrlListWorldObject.SetItemText( i, 1, wstrObjectLayer.c_str() );
		m_ctrlListWorldObject.SetItemText( i, 2, wstrObjectType.c_str() );
		if(pWorldObject->m_bShowObject == true )
			m_ctrlListWorldObject.SetItemText( i, 3, L"Show" );
		else
			m_ctrlListWorldObject.SetItemText( i, 3, L"" );
		if( pWorldObject->m_bFreezing == true )
			m_ctrlListWorldObject.SetItemText( i, 4, L"Freezing" );
		else
			m_ctrlListWorldObject.SetItemText( i, 4, L"" );


		if( pWorldObject->GetObjectType() != CX2WorldObject::OT_PARTICLE && pWorldObject->GetObjectType() != CX2WorldObject::OT_LIGHT_FLOW )
		{
			D3DXCOLOR vColor =   pWorldObject->GetRenderParam()->fogColor;
			if( IsSamef(vWorldFogColor.r, vColor.r) == false || IsSamef(vWorldFogColor.g, vColor.g) == false || IsSamef(vWorldFogColor.b, vColor.b) == false )
				pWorldObject->m_bUseObjectFogProperty = true;
		}
		else
		{
			pWorldObject->m_bUseObjectFogProperty = false;
		}

		if( bReset == true )
			pWorldObject->m_bSelected = false;
	}

	m_ctrlListWorldObject.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );

	UpdateData(TRUE);

	int iNum(-1);
	POSITION pos = m_ctrlListWorldObject.GetFirstSelectedItemPosition();
	iNum = m_ctrlListWorldObject.GetNextSelectedItem(pos);

	CRect ref;
	m_ctrlListWorldObject.GetItemRect(0, ref, LVIR_BOUNDS);
	CSize szHeight(0, ref.Height() * iScrollPos);
	m_ctrlListWorldObject.Scroll(szHeight);
	m_ctrlListWorldObject.SetScrollPos(SB_VERT, iScrollPos, FALSE);

	//m_ctrlListWorldObject.SetExtendedStyle(LVS_EX_CHECKBOXES);
}
void CEditWorld::ResetWorldEditor()
{
	// update list
	if( g_pX2Game == NULL )
		return;

	m_bCheckList = false;

	ResetWorldFrustum();
	ResetWorldLayer();
	ResetWorldSkyDome();
	ResetWorldObject();

	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_wndProperties.UpdateProp(3);
	pMainFrame->m_wndProperties.UpdateWorldBase();
	pMainFrame->m_wndProperties.UpdateWorldFog();
}

BOOL CEditWorld::OnInitDialog()
{
	ResetWorldEditor();
	m_DlgParticleList.Create(IDD_DIALOG_PARTICLE, this);
	m_DlgMeshList.Create(IDD_DIALOG_CATEGORY, this);
	
	return TRUE;
}

void CEditWorld::OnShowWindow(BOOL bShow, UINT nStatus)
{
	static bool bFirstShow = true;
	if( g_pMain == NULL && g_pX2Game == NULL )
		return;

	if( bFirstShow == false )
		return;

	if( bShow == false )
	{
		return;
	}

	bFirstShow = false;

	ResetWorldEditor();
}

BOOL CEditWorld::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		switch( pMsg->wParam )
		{
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;
		case VK_DELETE:
			{
				if( GetFocus() == GetDlgItem(IDC_LIST_EDIT_WORLDLAYER) )
				{
					CX2World *pWorld = g_pX2Game->GetWorld();
					vector<CX2WorldLayer*> &vecWorldlayer = pWorld->GetWorldLayerList();

					for(UINT i=0; i<vecWorldlayer.size(); ++i)
					{
						CX2WorldLayer *pWorldlayer = vecWorldlayer[i];
						if( pWorldlayer != NULL && pWorldlayer->m_bSelected == true )
						{
							g_pX2Game->GetWorld()->RemoveWordLayer(i);
							--i;
						}
					}

					ResetWorldLayer();
				}
				else if( GetFocus() == GetDlgItem(IDC_LIST_EDIT_SKYDOME) )
				{
					CX2World *pWorld = g_pX2Game->GetWorld();
					vector<CKTDGSkyDome*> &vecSkyDome = pWorld->GetSkyDomeList();
					for(UINT i=0; i<vecSkyDome.size(); ++i)
					{
						CKTDGSkyDome *pSkyDome = vecSkyDome[i];
						if( pSkyDome != NULL && pSkyDome->m_bSelected == true )
						{
							g_pX2Game->GetWorld()->RemoveSkyDome(i);
							--i;
						}
					}

					ResetWorldSkyDome();
				}
				else if( GetFocus() == GetDlgItem(IDC_LIST_WORLDOBJECT) )
				{
					CX2World *pWorld = g_pX2Game->GetWorld();
					vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
					for(UINT i=0; i<vecWorldObjectList.size(); ++i)
					{
						CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
						if( pWorldObject != NULL && pWorldObject->m_bSelected == true )
						{
							g_pX2Game->GetWorld()->DeleteWorldObject4Undo(pWorldObject); //RemoveWorldObject(pWorldObject);
						}
					}

					ResetWorldObject();
				}
			}
			break;
		default:
			break;
		}
	}
	else if( pMsg->message == WM_DROPFILES )
	{		
		OnDropFiles(pMsg->hwnd, (HDROP)pMsg->wParam);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CEditWorld::OnEnChangeEditWorldNear()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if( m_bResetFrustum == true || g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;

	CString csTemp;
	m_editNear.GetWindowText(csTemp);	
	//GetDlgItem(IDC_EDIT_EL_DIVIDE)->GetWindowText(csDivide);

	m_fNear = static_cast<float>( _ttof(csTemp) );

	pWorld->SetProjection( m_fNear, m_fFar, m_fPerspective, m_fBaseCamera, m_fCamera1, m_fCamera2, m_fCamera3 );
	g_pKTDXApp->GetDGManager()->SetProjection( m_fNear, m_fFar, g_pKTDXApp->GetDGManager()->GetPerspective() );
}


void CEditWorld::OnEnChangeEditWorldFar()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bResetFrustum == true || g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;

	CString csTemp;
	m_editFar.GetWindowText(csTemp);	
	//GetDlgItem(IDC_EDIT_EL_DIVIDE)->GetWindowText(csDivide);

	m_fFar = static_cast<float>( _ttof(csTemp) );

	pWorld->SetProjection( m_fNear, m_fFar, m_fPerspective, m_fBaseCamera, m_fCamera1, m_fCamera2, m_fCamera3 );
	g_pKTDXApp->GetDGManager()->SetProjection( m_fNear, m_fFar, g_pKTDXApp->GetDGManager()->GetPerspective() );
}


void CEditWorld::OnEnChangeEditWorldPerspective()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bResetFrustum == true || g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;

	CString csTemp;
	m_editPerspective.GetWindowText(csTemp);	
	//GetDlgItem(IDC_EDIT_EL_DIVIDE)->GetWindowText(csDivide);

	m_fPerspective = static_cast<float>( _ttof(csTemp) );

	pWorld->SetProjection( m_fNear, m_fFar, m_fPerspective, m_fBaseCamera, m_fCamera1, m_fCamera2, m_fCamera3 );
	g_pKTDXApp->GetDGManager()->SetProjection( m_fNear, m_fFar, g_pKTDXApp->GetDGManager()->GetPerspective() );
}


void CEditWorld::OnEnChangeEditWorldBasecamera()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bResetFrustum == true || g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;

	CString csTemp;
	m_editBaseCamera.GetWindowText(csTemp);	
	//GetDlgItem(IDC_EDIT_EL_DIVIDE)->GetWindowText(csDivide);

	m_fBaseCamera = static_cast<float>( _ttof(csTemp) );

	pWorld->SetProjection( m_fNear, m_fFar, m_fPerspective, m_fBaseCamera, m_fCamera1, m_fCamera2, m_fCamera3 );
}


void CEditWorld::OnEnChangeEditWorldCamera1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bResetFrustum == true || g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;

	CString csTemp;
	m_editCamera1.GetWindowText(csTemp);	
	//GetDlgItem(IDC_EDIT_EL_DIVIDE)->GetWindowText(csDivide);

	m_fCamera1 = static_cast<float>( _ttof(csTemp) );

	pWorld->SetProjection( m_fNear, m_fFar, m_fPerspective, m_fBaseCamera, m_fCamera1, m_fCamera2, m_fCamera3 );
}


void CEditWorld::OnEnChangeEditWorldCamera2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bResetFrustum == true || g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;

	CString csTemp;
	m_editCamera2.GetWindowText(csTemp);	
	//GetDlgItem(IDC_EDIT_EL_DIVIDE)->GetWindowText(csDivide);

	m_fCamera2 = static_cast<float>( _ttof(csTemp) );

	pWorld->SetProjection( m_fNear, m_fFar, m_fPerspective, m_fBaseCamera, m_fCamera1, m_fCamera2, m_fCamera3 );
}


void CEditWorld::OnEnChangeEditWorldCamera3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bResetFrustum == true || g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;

	CString csTemp;
	m_editCamera3.GetWindowText(csTemp);	
	//GetDlgItem(IDC_EDIT_EL_DIVIDE)->GetWindowText(csDivide);

	m_fCamera3 = static_cast<float>( _ttof(csTemp) );

	pWorld->SetProjection( m_fNear, m_fFar, m_fPerspective, m_fBaseCamera, m_fCamera1, m_fCamera2, m_fCamera3 );
}


void CEditWorld::OnLvnItemchangedListEditWorldlayer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
		
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;	
	if( pMain != NULL )
	{
		CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();		
		
		int iSelected = pMain->SelectWorldlayer();
		if( iSelected > 0 && pNMLV->uNewState > 0)
		{
			pMainFrame->m_wndProperties.UpdateProp(4);
			pMainFrame->m_wndProperties.UpdateWorldlayer();
		}		
	}
}


void CEditWorld::OnBnClickedButtonAddWorldlayer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"Image\0*.tga;*.TGA;*.dds;*.DDS\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		wstring wFile = ofn.lpstrFile;
		
		WCHAR wszFilePathOnly[512] = {NULL};
		WCHAR wszFileNameOnly[256] = {NULL};
		DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, wFile.c_str() );

		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory(wszFilePathOnly);
				
		CX2WorldLayer *pLayer = g_pX2Game->GetWorld()->CreateWorldLayer();
		if( pLayer != NULL )
		{
			pLayer->SetTexW(wszFileNameOnly);
		}
		ResetWorldLayer();

		CX2World *pWorld = g_pX2Game->GetWorld();
		vector<CX2WorldLayer*> vecWorldlayer = pWorld->GetWorldLayerList();

		for(UINT i=0; i<vecWorldlayer.size(); ++i)
		{									
			CX2WorldLayer *pWorldLayer = vecWorldlayer[i];
			if( pWorldLayer != NULL )
			{
				if(i == vecWorldlayer.size() - 1)
				{
					pWorldLayer->m_bSelected = true;
					m_ctrlListWorldLayer.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
					m_ctrlListWorldLayer.EnsureVisible(i, TRUE);
					m_ctrlListWorldLayer.SetFocus();
				}
				else
				{
					m_ctrlListWorldLayer.SetItemState(i, 0, LVIS_SELECTED );
				}			
			}
		}
	}	
}


void CEditWorld::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldLayer*> vecWorldlayer = pWorld->GetWorldLayerList();

	CX2WorldLayer *pSelectedLayer = NULL;
	int iSelectedIndex = -1;
	for(UINT i=0; i<vecWorldlayer.size(); ++i)
	{									
		CX2WorldLayer *pWorldLayer = vecWorldlayer[i];
		if( pWorldLayer != NULL && pWorldLayer->m_bSelected == true )
		{
			if( iSelectedIndex >= 0 )
			{
				AfxMessageBox(L"항목이동은 하나의 항목에 대해서만 가능합니다.", MB_OK);
			}
			iSelectedIndex = i;
			pSelectedLayer = pWorldLayer;
		}
	}
		
	if( iSelectedIndex <= 0 )
		return;
	else if( iSelectedIndex >= (int)vecWorldlayer.size() )
		return;

	g_pX2Game->GetWorld()->SwapWorldLayerId(iSelectedIndex, iSelectedIndex-1);
	
	ResetWorldLayer(false);
}


void CEditWorld::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldLayer*> vecWorldlayer = pWorld->GetWorldLayerList();

	CX2WorldLayer *pSelectedLayer = NULL;
	int iSelectedIndex = -1;
	for(UINT i=0; i<vecWorldlayer.size(); ++i)
	{									
		CX2WorldLayer *pWorldLayer = vecWorldlayer[i];
		if( pWorldLayer != NULL && pWorldLayer->m_bSelected == true )
		{
			if( iSelectedIndex >= 0 )
			{
				AfxMessageBox(L"항목이동은 하나의 항목에 대해서만 가능합니다.", MB_OK);
			}
			iSelectedIndex = i;
			pSelectedLayer = pWorldLayer;
		}
	}

	if( iSelectedIndex < 0 )
		return;
	else if( iSelectedIndex >= (int)vecWorldlayer.size() - 1 )
		return;

	g_pX2Game->GetWorld()->SwapWorldLayerId(iSelectedIndex, iSelectedIndex+1);

	ResetWorldLayer(false);
}


void CEditWorld::OnNMSetfocusListEditWorldlayer(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	for(int i=0; i<m_ctrlSkyDomList.GetItemCount(); ++i)
		m_ctrlSkyDomList.SetItemState(i, 0, LVIS_SELECTED );
	for(int i=0; i<m_ctrlListWorldObject.GetItemCount(); ++i)
		m_ctrlListWorldObject.SetItemState(i, 0, LVIS_SELECTED );

	if( g_pX2Game == NULL )
		return;

	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;

	vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();
	for(UINT i=0; i<vecSkyDome.size(); ++i)
	{
		CKTDGSkyDome *pSkyDome = vecSkyDome[i];
		pSkyDome->m_bSelected = false;
	}
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		pWorldObject->m_bSelected = false;
	}	
}


void CEditWorld::OnBnClickedButtonAddSkydome()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"Static Mesh\0*.y;*.Y\0"; //L"Static Mesh\0*.y\0*.Y\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		wstring wFile = ofn.lpstrFile;

		WCHAR wszFilePathOnly[512] = {NULL};
		WCHAR wszFileNameOnly[256] = {NULL};
		DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, wFile.c_str() );

		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory(wszFilePathOnly);

		CX2World *pWorld = g_pX2Game->GetWorld();
		CKTDGSkyDome *pSkyDome = pWorld->CreateSkyDome();
		if( pSkyDome != NULL )
		{
			pSkyDome->SetDomeMeshW(wszFileNameOnly);
			pSkyDome->SetLayer(XL_SKY_DOME_0);
		}

		ResetWorldSkyDome();

		vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();
		
		for(UINT i=0; i<vecSkyDome.size(); ++i)
		{									
			CKTDGSkyDome *pSkyDome = vecSkyDome[i];
			if( pSkyDome != NULL )
			{
				if(i == vecSkyDome.size() - 1)
				{
					pSkyDome->m_bSelected = true;
					m_ctrlSkyDomList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
					m_ctrlSkyDomList.EnsureVisible(i, TRUE);
					m_ctrlSkyDomList.SetFocus();
				}
				else
				{
					m_ctrlSkyDomList.SetItemState(i, 0, LVIS_SELECTED );
				}			
			}
		}
	}	
}


void CEditWorld::OnLvnItemchangedListEditSkydome(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;	
	if( pMain != NULL )
	{
		CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();		
		
		int iSelected = pMain->SelectSkydome();
		if( iSelected > 0 && pNMLV->uNewState > 0 )
		{
			pMainFrame->m_wndProperties.UpdateProp(5);
			pMainFrame->m_wndProperties.UpdateWorldSkyDome();
		}		
	}
}


void CEditWorld::OnNMSetfocusListEditSkydome(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	*pResult = 0;
	for(int i=0; i<m_ctrlListWorldLayer.GetItemCount(); ++i)
		m_ctrlListWorldLayer.SetItemState(i, 0, LVIS_SELECTED );
	for(int i=0; i<m_ctrlListWorldObject.GetItemCount(); ++i)
		m_ctrlListWorldObject.SetItemState(i, 0, LVIS_SELECTED );

	if( g_pX2Game == NULL )
		return;
	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;

	vector<CX2WorldLayer*> vecWorldLayer = pWorld->GetWorldLayerList();
	for(UINT i=0; i<vecWorldLayer.size(); ++i)
	{
		CX2WorldLayer *pWorldLayer = vecWorldLayer[i];
		pWorldLayer->m_bSelected = false;
	}
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		pWorldObject->m_bSelected = false;
	}
	
}


void CEditWorld::OnNMSetfocusListWorldobject(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	for(int i=0; i<m_ctrlListWorldLayer.GetItemCount(); ++i)
		m_ctrlListWorldLayer.SetItemState(i, 0, LVIS_SELECTED );
	for(int i=0; i<m_ctrlSkyDomList.GetItemCount(); ++i)
		m_ctrlSkyDomList.SetItemState(i, 0, LVIS_SELECTED );

	if( g_pX2Game == NULL )
		return;
	CX2World *pWorld = g_pX2Game->GetWorld();
	if( pWorld == NULL )
		return;

	vector<CX2WorldLayer*> vecWorldLayer = pWorld->GetWorldLayerList();
	for(UINT i=0; i<vecWorldLayer.size(); ++i)
	{
		CX2WorldLayer *pWorldLayer = vecWorldLayer[i];
		pWorldLayer->m_bSelected = false;
	}
	vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();
	for(UINT i=0; i<vecSkyDome.size(); ++i)
	{
		CKTDGSkyDome *pSkyDome = vecSkyDome[i];
		pSkyDome->m_bSelected = false;
	}
}


void CEditWorld::OnLvnItemchangedListWorldobject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;	
	if( pMain != NULL )
	{
		CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();		

		if( (pNMLV->uNewState == 2 || pNMLV->uNewState == 3) && (pNMLV->uOldState == 0 || pNMLV->uOldState == 1) )
		{
			// 선택
			pMain->SelectWorldObject(pNMLV->iItem, true);
			pMainFrame->m_wndProperties.UpdateProp(6);
			pMainFrame->m_wndProperties.UpdateWorldObject();
		}
		else if( pNMLV->uNewState == 0 && pNMLV->uOldState == 2 )
		{
			// 선택 해제
			pMain->SelectWorldObject(pNMLV->iItem, false);
			pMainFrame->m_wndProperties.UpdateProp(6);
			pMainFrame->m_wndProperties.UpdateWorldObject();
		}
	}
}

void CEditWorld::AddWorldObject(wstring wstrName, D3DXVECTOR3 vPos)
{
	WCHAR wszFilePathOnly[512] = {NULL};
	WCHAR wszFileNameOnly[256] = {NULL};
	DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, wstrName.c_str() );

	g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory(wszFilePathOnly);

	bool bStaticMesh = true;
	wstring wstrFileNameUpper = wszFileNameOnly;
	MakeUpperCase(wstrFileNameUpper);
	if( StrStrW(wstrFileNameUpper.c_str(), L".X") != NULL )
		bStaticMesh = false;

	CX2World *pWorld = g_pX2Game->GetWorld();
	CX2WorldObject *pAddWorldObject = NULL;
	if( bStaticMesh == true )
	{
		CX2WorldObjectMesh *pObject = pWorld->CreateObjectMesh();
		pAddWorldObject = (CX2WorldObject*)pObject;
		string strMeshName = "";
		ConvertWCHARToChar(strMeshName, wszFileNameOnly);
		pObject->SetXMesh_LUA(strMeshName.c_str());
		pObject->SetLayer( (int)XL_SKY_WORLD_BASE_0 );
		pObject->SetPos(vPos);
		pObject->SetOriginalColorFactor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		pObject->SetColorFactor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		pObject->SetCanHide(false, false);
		pObject->SetNotRenderWhenHidden(false);
		pObject->SetAlphaObject(false);
		pObject->GetRenderParam()->bAlphaBlend = false;
		pObject->GetRenderParam()->bAlphaTestEnable = false;
		if( pWorld->m_bFog == true && pWorld->m_bFogShow == true )
		{
			pObject->SetFog( pWorld->m_fNearX, pWorld->m_fFarX, pWorld->m_fNearY, pWorld->m_fFarY, pWorld->m_FogColor, pWorld->m_fDensity );
		}
	}
	else
	{
		CX2WorldObjectSkinMesh *pObject = pWorld->CreateObjectSkinMesh();
		pAddWorldObject = (CX2WorldObject*)pObject;
		string strMeshName = "";
		ConvertWCHARToChar(strMeshName, wszFileNameOnly);
		pObject->SetXSkinMotion_LUA(strMeshName.c_str());
		pObject->AddXSkinModel_LUA(strMeshName.c_str());

		vector<wstring> vecAnimList;
		pObject->GetXSkinAnim()->GetAnimNameList(vecAnimList);
		string strAnimName = "";
		ConvertWCHARToChar(strAnimName, vecAnimList[0].c_str());
		pObject->AddAnim_LUA(strAnimName.c_str());
		pObject->PlayAnim_LUA( CX2WorldObjectSkinMesh::WOSMPY_LOOP, true );
		pObject->SetLayer( (int)XL_SKY_WORLD_BASE_0 );
		pObject->SetPos(vPos);
		pObject->SetOriginalColorFactor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		pObject->SetColorFactor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		pObject->SetCanHide(false, false);
		pObject->SetAlphaObject(false);
		pObject->GetRenderParam()->bAlphaBlend = false;
		pObject->GetRenderParam()->bAlphaTestEnable = false;
		if( pWorld->m_bFog == true && pWorld->m_bFogShow == true )
		{
			pObject->SetFog( pWorld->m_fNearX, pWorld->m_fFarX, pWorld->m_fNearY, pWorld->m_fFarY, pWorld->m_FogColor, pWorld->m_fDensity );
		}
	}

	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	if( pToolMain != NULL )
	{
		vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
		for(UINT i=0; i<vecWorldObjectList.size(); ++i)
		{
			if( vecWorldObjectList[i].get() == pAddWorldObject )
			{
				tAddRmoveWorldElem tUndoElem;
				tUndoElem.m_bAdded = true;
				tUndoElem.m_vecWorldObject.push_back(vecWorldObjectList[i]);
				break;
			}
		}			
	}

	ResetWorldObject();

	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( pWorldObject != NULL && pWorldObject == pAddWorldObject )
			pWorldObject->m_bSelected = true;

		if( pMain != NULL )
			pMain->m_DlgWorldEditor.SetCurSel( i, pWorldObject->m_bSelected );
	}
}

void CEditWorld::OnBnClickedButtonAddWorldobject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"Mesh\0*.x;*.X;*.y;*.Y\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		wstring wFile = ofn.lpstrFile;

		AddWorldObject(wFile, D3DXVECTOR3(0.f, 0.f, 0.f));
	}
}

void CEditWorld::OnBnClickedButtonAddParticle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_DlgParticleList.InitDlg();
	m_DlgParticleList.ShowWindow(SW_SHOW);
}

void CEditWorld::AddParticle(int iType, wstring wstrName)
{
	CX2World *pWorld = g_pX2Game->GetWorld();
	CX2WorldObject *pAddWorldObject = NULL;
	CKTDGParticleSystem *pParticleSystem = NULL;

	switch(iType)
	{
	case 0:
		pParticleSystem = g_pX2Game->GetMajorParticle();
		break;
	case 1:
		pParticleSystem = g_pX2Game->GetMinorParticle();
		break;
	case 2:
		pParticleSystem = g_pData->GetUIMajorParticle();
		break;
	case 3:
		pParticleSystem = g_pData->GetUIMinorParticle();
		break;
	default:
		break;
	}

	if( pParticleSystem == NULL )
		return;

	CRect rectClient;
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	::GetClientRect(pMainFrame->GetSafeHwnd(), &rectClient );	
	D3DXVECTOR3 vPos = g_pKTDXApp->GetUnProj3DPos(D3DXVECTOR3(rectClient.Width()/2.f, rectClient.Height()/2.f, 0.9993f));

	string strParticleName;
	ConvertWCHARToChar(strParticleName, wstrName.c_str());
	CX2WorldObjectParticle *pObject = pWorld->CreateObjectParticle(pParticleSystem, strParticleName.c_str());
	pObject->SetLayer( XL_EFFECT_0 );
	pObject->SetParticlePos( vPos );
	ResetWorldObject();

	pObject->m_bSelected = true;

	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();
	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( pWorldObject != NULL && pWorldObject == pAddWorldObject )
			pWorldObject->m_bSelected = true;

		if( pMain != NULL )
			pMain->m_DlgWorldEditor.SetCurSel( i, pWorldObject->m_bSelected );
	}
}

void CEditWorld::OnLvnOdstatechangedListWorldobject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVODSTATECHANGE pStateChanged = reinterpret_cast<LPNMLVODSTATECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CEditWorld::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_DlgMeshList.ShowWindow(SW_SHOW);
	m_DlgMeshList.UpdateLib();
	m_DlgMeshList.UpdateList(L"");
}

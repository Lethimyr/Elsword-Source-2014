#include "StdAfx.h"

#ifdef ELSWORD_NEW_BEGINNING
#include ".\X2BeginningGame.h"

static const int ELSWORD_PROMOTION_ITEM_ID[]	= { 131547, 131548, 131549, 131550, 131551, };
static const int AISHA_PROMOTION_ITEM_ID[]		= { 131367, 131368, 131369, 131370, 131371, };
static const int RENA_PROMOTION_ITEM_ID[]		= { 131345, 131346, 131347, 131348, 131349, };
static const int RAVEN_PROMOTION_ITEM_ID[]		= { 128072, 128073, 128074, 128075, 128076, };
static const int EVE_PROMOTION_ITEM_ID[]		= { 131768, 131769, 131770, 131771, 131772, };



//{{ kimhc // 2010.12.8 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
static const int CHUNG_PROMOTION_ITEM_ID[]		= { 133125, 133126, 133127, 133128, 133129, };	// 캐릭터 생성 창에서 사용될 청의 프로모션 복장 ID
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.8 //  2010-12-23 New Character CHUNG



static const D3DXVECTOR3 CHARACTER_CREATE_POSITION[]	= { D3DXVECTOR3(-14572+250+250+250+250+500, -388-20, 4436+250+250+250+250-400 ), 
															D3DXVECTOR3(-14572+250+250+250+500, -388-20, 4436+250+250+250-400 ),
															D3DXVECTOR3(-14572+250+250+500, -388-20, 4436+250+250-400 ),
															D3DXVECTOR3(-14572+250+500, -388-20, 4436+250-400 ),
															D3DXVECTOR3(-14572+500, -388-20, 4436-400 ), 
															D3DXVECTOR3(-14572+500-250, -388-20, 4436-250-400 ), };

#ifdef ELSWORD_NEW_BEGINNING
static const float LOADING_ANIM_STEP[]	= { 500.f-50.f, 300.f, 93.f, 88.f,
											498.f-50.f, 298.f, 97.f, 92.f,
											497.f-50.f, 297.f, 99.f, 94.f,
											498.f-50.f, 298.f, 97.f, 92.f,
											500.f-50.f, 300.f, 93.f, 88.f,
											502.f-50.f, 302.f, 89.f, 84.f,
											503.f-50.f, 303.f, 87.f, 82.f,
											502.f-50.f, 302.f, 89.f, 84.f, };

static const int LOADING_TEX_UV[]		= { 0, 97, 93, 88	};
#endif ELSWORD_NEW_BEGINNING



namespace		_CONST_BEGINNING_GAME_
{
	const float		g_fFreezeTimeForSelectElsword		= 2.f;
	const float		g_fFreezeTimeForSelectArme			= 2.f;
	const float		g_fFreezeTimeForSelectLire			= 2.f;
	const float		g_fFreezeTimeForSelectRaven		= 2.f;
	const float		g_fFreezeTimeForSelectEve			= 2.f;

	const float		g_fFreezeTimeForUnSelectElsword	= 2.f;
	const float		g_fFreezeTimeForUnSelectArme		= 2.f;
	const float		g_fFreezeTimeForUnSelectLire		= 2.f;
	const float		g_fFreezeTimeForUnSelectRaven		= 2.f;
	const float		g_fFreezeTimeForUnSelectEve		= 2.f;
}

CX2BeginningGame::CX2BeginningGame()
{
	//g_pTFieldGame			= this;

	m_pLoadingFaceTex = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_Loading4.TGA" );

	m_pWorld	= NULL;
	m_pCamera	= NULL;
	
	m_pUiParticle			= NULL;
	//m_pMajorParticle		= NULL;
	//m_pMinorParticle		= NULL;
	//m_pMajorXMeshPlayer		= NULL;
	//m_pMinorXMeshPlayer		= NULL;
	m_pFontForUnitName		= NULL;

	m_pSubSound				= NULL;

	m_bFreeCamera			= false;

	m_bEnableCreateUnit		= false;

	m_pDLGCreateUnitNickName = NULL;

	m_pDLGCreateUnitNickName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Unit_Create_NickName_TEST.lua" );

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCreateUnitNickName );

	m_fRemainTimeOpenNickNameBox = 0.f;
	m_fRemainTimeUnSelectComplete = 0.f;

	InitDummyUnits();

	CX2UnitViewerUI::SetUnitClicked( false );



	CreateElSword();
	CreateArme();
	CreateLire();
	CreateRaven();
	CreateEve();
#ifdef NEW_CHARACTER_CHUNG
	CreateChung();
#endif NEW_CHARACTER_CHUNG
	UnSelect();

	SetEnableCreateUnit(false);

}

CX2BeginningGame::~CX2BeginningGame(void)
{			

	m_pFontForUnitName = NULL;


	SAFE_DELETE( m_pWorld );
	SAFE_DELETE( m_pCamera );
	

	SAFE_CLOSE( m_pSubSound );

	g_pKTDXApp->GetDIManager()->SetEnable( true );

	//SAFE_DELETE( m_pUiParticle );			// delete 하면 안됨
	//SAFE_DELETE( m_pUiXMeshPlayer );		// delete 하면 안됨
	//SAFE_DELETE( m_pMajorParticle );
	//SAFE_DELETE( m_pMinorParticle );
	//SAFE_DELETE( m_pMajorXMeshPlayer );
	//SAFE_DELETE( m_pMinorXMeshPlayer );

	//g_pTFieldGame = NULL;	

	SAFE_DELETE_DIALOG( m_pDLGCreateUnitNickName );
	SAFE_DELETE_DIALOG( m_pDLGMSGOkBoxEnterTutorial );

	SAFE_DELETE( m_pDummyElsword );
	SAFE_DELETE( m_pDummyArme );
	SAFE_DELETE( m_pDummyLire );
	SAFE_DELETE( m_pDummyRaven );

	SAFE_DELETE( m_pDummyEve );
#ifdef NEW_CHARACTER_CHUNG
	SAFE_DELETE( m_pDummyChung );
#endif NEW_CHARACTER_CHUNG

	for ( int i = 0; i < (int)m_CreateUnitViewerUIList.size(); i++ )
	{
		CX2UnitViewerUI* pUnitViewerUI = m_CreateUnitViewerUIList[i];

		CX2UnitViewerUI::DeleteKTDGObject( pUnitViewerUI );
	}
	m_pNowSelectedUnitViewerUI = NULL;



}

bool CX2BeginningGame::LoadWorld(CX2World::WORLD_ID worldID)
{
	m_pCamera		= new CX2Camera();

    /** @NOTE:

        백그라운드 로딩을 사용하려면 월드를 생성할 때, CreateWorld()가 아닌,
        ThreadRequest_CreateWorld()로 생성해야 한다.
        하지만, 던전에 대해서는 urgent data를 처리하는 부분을 구현하지 않았기
        때문에, 던전 진입후에 로딩되는 리소스들이 눈에 보이게 될 것이다.

        urgent data:
            첫 화면에서 보여지는 모든 데이터를 의미한다. 던전의 경우 던전에 처음 진입했을 때
            렌더링할 모든 데이터를 의미한다.
            
        - jintaeks on 2008-10-24, 10:31 */

#ifdef BACKGROUND_LOADING_TEST // 2008-10-18
	m_pWorld = g_pData->GetWorldManager()->CreateWorld( worldID );

    // 광장에 대해 thread 로딩 테스트를 하고 있다. - jintaeks on 2008-10-18, 13:54
	//m_pWorld	= g_pData->GetWorldManager()->ThreadRequest_CreateWorld( worldID );

#else // BACKGROUND_LOADING_TEST // 2008-10-18

    /** EnableWritingPreprocessingData()의 의미:
        백그라운드 로딩을 하기 위해서는 게임 객체의 전처리된 정보가 필요하다.
        현재 - jintaeks on 2008-10-24, 10:34 - 의 경우 그 정보는, bounding sphere의 반지름
        및 메시의 local 중심 좌표이다. 이 값들을 "스크립트파일이름.ppd"로 생성하려면

            EnableWritingPreprocessingData( true );

        로 설정하면 된다.
        이 부분은 게임에 통합하지 말고, 별도의 툴로 작성되어야 할 것이다.

        - jintaeks on 2008-10-24, 10:34 */

	//g_pData->GetWorldManager()->EnableWritingPreprocessingData( true );
	m_pWorld = g_pData->GetWorldManager()->CreateWorld( worldID );
    //g_pData->GetWorldManager()->EnableWritingPreprocessingData( false );

#endif // BACKGROUND_LOADING_TEST // 2008-10-18

	if( m_pWorld != NULL )
	{

		m_pWorld->SetBGM_LUA( "DunGeonGate01.ogg" );

		m_pWorld->PlayBGM();
		m_pCamera->SetLandHeight( m_pWorld->GetLineMap()->GetLandHeight() );
	}


	srand((unsigned int)time(NULL));

	m_pUiParticle = g_pData->GetUIMajorParticle();
	m_pUiXMeshPlayer = g_pData->GetUIMajorXMeshPlayer();

	m_pFontForUnitName = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD );	

	//m_bEnableKey			= true;
	m_bEnableKeyProcess		= true;
	m_bEnableCameraProcess	= true;

	g_pKTDXApp->GetDIManager()->SetEnable( true );

	// oasis907 : 김상윤 [2010.11.11] // 테스트
	//SetFreeCamera(true);

	//g_pData->GetEventScene()->BlackLayOutMove( false, 1.0f, false, true, 3.0f, 1.0f );

	// oasis907 : 김상윤 [2010.11.16] // 
	g_pKTDXApp->SkipFrame();

	return true;
}


void CX2BeginningGame::SetStage(CKTDXStage *pStage)
{

}

bool CX2BeginningGame::GetEnableKey()
{
	return true;
}

HRESULT	CX2BeginningGame::OnFrameMove( double fTime, float fElapsedTime )
{		
	KTDXPROFILE();	

	if( GetEnableKey() == true)
	{
		if( false == m_bFreeCamera )
		{
			KeyProcess();
		}
	}
	else
	{
		InitKey();
	}
	
 	if(m_pWorld != NULL)
 		m_pWorld->OnFrameMove( fTime, fElapsedTime );	


	if( m_bEnableCameraProcess == true )
	{
		if( false == m_bFreeCamera && NULL != m_pWorld && NULL != m_pWorld->GetLineMap())
		{
			m_pCamera->OnFrameMove( fTime, fElapsedTime );
		}
		else
		{
			m_FPSCamera.SetEnablePositionMovement( true );

			m_FPSCamera.FrameMove( fElapsedTime * 300.f );			
			m_pCamera->GetCamera().Move( m_FPSCamera.GetEyePt()->x, m_FPSCamera.GetEyePt()->y, m_FPSCamera.GetEyePt()->z );
			D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * 500.f + *m_FPSCamera.GetEyePt();
			m_pCamera->GetCamera().LookAt( vLookAt.x, vLookAt.y, vLookAt.z );
			m_pCamera->GetCamera().UpdateCamera( fElapsedTime );
		}
	}		


	if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
		true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() )
	{
		D3DXVECTOR3 vLookVec = m_pCamera->GetCamera().GetLookVec();
		D3DXVec3Normalize( &vLookVec, &vLookVec );
		D3DXVECTOR3 vUpVec = m_pCamera->GetCamera().GetUpVec();
		D3DXVec3Normalize( &vUpVec, &vUpVec );

		g_pKTDXApp->GetDSManager()->SetListenerData( m_pCamera->GetCamera().GetEye(), vLookVec, vUpVec );
	}
	



	if(m_bEnableCreateUnit)
	{

		for ( int i = 0; i < (int)m_CreateUnitViewerUIList.size(); i++ )
		{
			CX2UnitViewerUI* pUnitViewerUI = m_CreateUnitViewerUIList[i];

			if ( pUnitViewerUI == NULL )
				continue;

			pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );

			if ( pUnitViewerUI->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAP_ONE_WAIT )
			{
				pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
			}

			if(GetShowMsgBox() == false && m_fRemainTimeOpenNickNameBox <= 0.f && m_fRemainTimeUnSelectComplete <= 0.f)
			{
				// 픽킹
				D3DXVECTOR3 pos = CHARACTER_CREATE_POSITION[i];
				pos.y += 100.0f;

				D3DXVECTOR3 startRay, endRay;
				g_pKTDXApp->Make2DPosToRay( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(),
					g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos(), 10000, startRay, endRay );

				pUnitViewerUI->SetPickUnit(false);
				if( g_pKTDXApp->GetCollision()->LineToSphere( startRay, endRay, 30.0f, pos,	100.f ) == true )
				{
					pUnitViewerUI->SetPickUnit(true);

					BOOL bLbutton = g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState( MOUSE_LBUTTON );
					if(bLbutton == TRUE)
					{
						switch(i)
						{
						case 0:
							SelectElSword();
							break;
						case 1:
							SelectArme();
							break;
						case 2:
							SelectLire();
							break;
						case 3:
							SelectRaven();
							break;
						case 4:
							SelectEve();
							break;
#ifdef NEW_CHARACTER_CHUNG
						case 5:
							SelectChung();
							break;
#endif NEW_CHARACTER_CHUNG
						}
					}
				}
			}
		}

		bool bCheckHaveFocus = false;
		CKTDGUIControl* pControl = m_pDLGCreateUnitNickName->GetControl(L"IMEEditBoxUnitCreateNickName");
		if ( pControl != NULL )
		{
			if ( pControl->GetHaveFocusIn() == true )
			{
				bCheckHaveFocus = true;
			}
		}
/*
		pControl = m_pDLGCreateUnitNickName->GetControl( L"Unit_Create_Attntion_Box" );
		if ( pControl != NULL )
		{
			if ( bCheckHaveFocus == true )
			{
				if ( pControl->GetIsMoving() == false )
					pControl->Move( pControl->GetOffsetPos(), D3DXCOLOR( 1,1,1,1 ), 0.5f, true );
			}
			else
			{
				if ( pControl->GetIsMoving() == false )
					pControl->Move( pControl->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), 0.5f, true );
			}
		}
*/
		if(m_fRemainTimeOpenNickNameBox > 0.f)
		{
			m_fRemainTimeOpenNickNameBox -= fElapsedTime;
			if(m_fRemainTimeOpenNickNameBox <= 0.f)
			{
				m_fRemainTimeOpenNickNameBox = 0.f;
				OpenNickNameBox(-1.f);
			}
		}

		if(m_fRemainTimeUnSelectComplete > 0.f)
		{
			m_fRemainTimeUnSelectComplete -= fElapsedTime;
			if(m_fRemainTimeUnSelectComplete <= 0.f)
			{
				m_fRemainTimeUnSelectComplete = 0.f;
				UnSelect();
			}
		}
	}





	return S_OK;
}

HRESULT	CX2BeginningGame::OnFrameRender()
{	

	KTDXPROFILE();
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
        CKTDGParticleSystem::EnableParticleCulling( true );
#endif  X2OPTIMIZE_CULLING_PARTICLE		
		g_pKTDXApp->GetDGManager()->ObjectChainSort();
		//{{ robobeg : 2008-10-18
		
		g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();


		g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
        CKTDGParticleSystem::EnableParticleCulling( false );
#endif  X2OPTIMIZE_CULLING_PARTICLE	
		g_pKTDXApp->GetDGManager()->FrontUIRender();
		//}} robobeg : 2008-10-18	

	return S_OK;
}

bool CX2BeginningGame::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	if( true == m_bFreeCamera )
	{
		m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);		
	}
	return false;
}

HRESULT	CX2BeginningGame::OnResetDevice()
{

	if( m_pWorld != NULL )
		m_pWorld->OnResetDevice();

	if( m_pUiParticle != NULL )
		m_pUiParticle->OnResetDevice();
	
	//if( m_pMajorParticle != NULL )
	//	m_pMajorParticle->OnResetDevice();
	//if( m_pMajorParticle != NULL )
	//	m_pMajorParticle->OnResetDevice();

	return S_OK;
}

HRESULT	CX2BeginningGame::OnLostDevice()
{

	if( m_pWorld != NULL )
		m_pWorld->OnLostDevice();

	if( m_pUiParticle != NULL )
		m_pUiParticle->OnLostDevice();
	//if( m_pMajorParticle != NULL )
	//	m_pMajorParticle->OnLostDevice();
	//if( m_pMinorParticle != NULL )
	//	m_pMinorParticle->OnLostDevice();

	return S_OK;
}

void CX2BeginningGame::InitKey()
{
	m_InputData.Init();
}


void CX2BeginningGame::KeyProcess()
{
	KTDXPROFILE();

	m_InputData.Init();

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetDoubleKeyPureState(DIK_LEFT) == TRUE )
	{
		m_InputData.pureDoubleLeft = true;
	}
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetDoubleKeyState(DIK_LEFT) == TRUE )
	{
		m_InputData.oneDoubleLeft = true;
	}
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState(DIK_LEFT) == TRUE )
	{
		m_InputData.pureLeft = true;
	}
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_LEFT) == TRUE )
	{
		m_InputData.oneLeft = true;
	}

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetDoubleKeyPureState(DIK_RIGHT) == TRUE )
	{
		m_InputData.pureDoubleRight = true;
	}
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetDoubleKeyState(DIK_RIGHT) == TRUE )
	{
		m_InputData.oneDoubleRight = true;
	}
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState(DIK_RIGHT) == TRUE )
	{
		m_InputData.pureRight = true;
	}
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RIGHT) == TRUE )
	{
		m_InputData.oneRight = true;
	}
}


void CX2BeginningGame::SetFreeCamera( bool bFreeCamera )
{
	m_bFreeCamera = bFreeCamera;

	if( true == m_bFreeCamera )
	{
		D3DXVECTOR3 vEye	= m_pCamera->GetCamera().GetEye();
		D3DXVECTOR3 vLookAt = m_pCamera->GetCamera().GetLookAt();
#ifdef KEYFRAME_CAMERA
		D3DXVECTOR3 vUpVec	= m_pCamera->GetCamera().GetUpVec();
		m_FPSCamera.SetViewParams( &vEye, &vLookAt, &vUpVec );
#else
		m_FPSCamera.SetViewParams( &vEye, &vLookAt );
#endif KEYFRAME_CAMERA
		m_FPSCamera.SetProjParams( D3DX_PI/4, 1.f, g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar() );
		m_FPSCamera.SetRotateButtons( false, false, true, false );
	}
}

bool CX2BeginningGame::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case BGUCM_CREATE_UNIT:
		{
			if( m_bSelectUnit == true )
				Handler_EGS_CREATE_UNIT_REQ();
			else
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_559 ), g_pMain->GetNowState() );
		}
		break;

	case BGUCM_BACK_TO_UNIT_SELECT:
		{
			if( m_bSelectUnit == true )
			{
				UnSelectUnitPlayWorldCamera();
			}
			else
			{
				SetEnableCreateUnit(false);
			}
		}
		break;
	}
	return false;
}

bool CX2BeginningGame::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case EGS_CREATE_UNIT_ACK:
		return Handler_EGS_CREATE_UNIT_ACK( hWnd, uMsg, wParam, lParam );
		break;
	}

	return false;
}

#ifdef KEYFRAME_CAMERA
void CX2BeginningGame::UnSelectUnitPlayWorldCamera()
{
	m_pDLGCreateUnitNickName->SetShowEnable(false, false);

	switch(m_SelectUnitClass)
	{
	case CX2Unit::UC_ELSWORD_SWORDMAN:
		{
			m_fRemainTimeUnSelectComplete = 2.0f;
			PlayWorldCamera(5);
		}
		break;
	case CX2Unit::UC_ARME_VIOLET_MAGE:
		{
			m_fRemainTimeUnSelectComplete = 2.0f;
			PlayWorldCamera(7);
		}
		break;
	case CX2Unit::UC_LIRE_ELVEN_RANGER:
		{
			m_fRemainTimeUnSelectComplete = 2.0f;
			PlayWorldCamera(9);
		}
		break;
	case CX2Unit::UC_RAVEN_FIGHTER:
		{
			m_fRemainTimeUnSelectComplete = 2.0f;
			PlayWorldCamera(11);
		}
		break;
	case CX2Unit::UC_EVE_NASOD:
		{
			m_fRemainTimeUnSelectComplete = 2.0f;
			PlayWorldCamera(13);
		}
		break;
#ifdef NEW_CHARACTER_CHUNG
	case CX2Unit::UC_CHUNG_IRON_CANNON:
		{
			m_fRemainTimeUnSelectComplete = 2.0f;
			PlayWorldCamera(15);
		}
		break;
#endif NEW_CHARACTER_CHUNG
	}
	return;
}





void CX2BeginningGame::PlayWorldCamera( int iWorldCameraID )
{
	GetX2Camera()->StartWorldCamera(iWorldCameraID);
}
#endif KEYFRAME_CAMERA


bool CX2BeginningGame::Handler_EGS_CREATE_UNIT_REQ()
{											

	CKTDGUIIMEEditBox* pIDEditBox = (CKTDGUIIMEEditBox*)m_pDLGCreateUnitNickName->GetControl( L"IMEEditBoxUnitCreateNickName" );
	m_NickName = pIDEditBox->GetText();

	if( CreateErrorCheck() == false )
		return true;

	KEGS_CREATE_UNIT_REQ kPacket;
	kPacket.m_iClass			= m_SelectUnitClass;
	kPacket.m_wstrNickName		= m_NickName;

	g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_UNIT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CREATE_UNIT_ACK );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_555 ), g_pMain->GetNowState() );

	return true;	
}

bool CX2BeginningGame::Handler_EGS_CREATE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_UNIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_CREATE_UNIT_ACK ) == true )
	{
		bool bCheck = false;

		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(kEvent.m_iOK)
		{
		case NetError::ERR_CREATE_UNIT_01:
			{
				SetAlarmAboutName( GET_STRING( STR_ID_556 ) );
				bCheck = true;
			} break;
		case NetError::ERR_CREATE_UNIT_04:
			{

				SetAlarmAboutName( GET_STRING( STR_ID_557 ) );
				bCheck = true;
			} break;
		case NetError::ERR_CREATE_UNIT_06:
			{
				wstringstream wstrstm;
				wstrstm << GET_REPLACED_STRING( ( STR_ID_558, "L", kEvent.m_wstrEnableDate ) );

				SetAlarmAboutName( wstrstm.str().c_str() );
				bCheck = true;
			} break;
		case NetError::ERR_CREATE_UNIT_08:
			{
				SetAlarmAboutName( GET_STRING( STR_ID_362 ) );
				bCheck = true;
			} break;
		default:
			break;
		}//}}


		if ( bCheck == true )
		{
			wstring wstrName( L"Emotion_No" );
			m_pNowSelectedUnitViewerUI->PlayEmotion( CX2Unit::ET_NO, wstrName );

			return true;
		}

		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			//{{ 2009. 7. 6  최육사		사용하지 않는 기능이므로 주석처리하였습니다~
			// 처음 캐릭터를 생성한것인지 검사
			//if(kEvent.m_bIsFirstUnit == true)
			//{
			//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2633 ), g_pMain->GetNowState() );
			//}
			//}}

			//유닛 정보 받아오기, 인벤토리 받아오기, 유닛 셀렉트로 이동하기

			KUnitInfo kUnitInfo = kEvent.m_kUnitInfo;
			CX2Unit* pUnit = new CX2Unit( kUnitInfo );

			pUnit->SetOwnerUserUID( g_pData->GetMyUser()->GetUID() );
			g_pData->GetMyUser()->AddUnit( pUnit );	
			m_UnitUID = pUnit->GetUID();

			SAFE_DELETE_DIALOG( m_pDLGMSGOkBoxEnterTutorial );

			//m_pDLGMSGOkBoxEnterTutorial = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250, 300), L"튜토리얼을 진행하시겠습니까?",
			//	SCUUCM_ENTER_TUTORIAL_OK, this, SCUUCM_ENTER_TUTORIAL_CANCEL );

			if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
			{
				CX2StateAutoChanger::TARGET_DETAIL targetDetail;
				g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), CX2Main::XS_VILLAGE_MAP, targetDetail, m_UnitUID );
			}
			else
			{
				g_pMain->SetIsPlayingTutorial( true );
				if( m_pDLGMSGOkBoxEnterTutorial != NULL )
				{
					g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMSGOkBoxEnterTutorial, NULL, false );
					m_pDLGMSGOkBoxEnterTutorial = NULL;
				}

				// oasis907 : 김상윤 [2010.11.19] // 	
				//g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_552 ) );

				//g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/CX2Main::XS_BEGINNING, NULL, false ); 

				SetEnableCreateUnit(false);
			}	
		}

	}
	return true;
}


bool CX2BeginningGame::CreateErrorCheck()
{
	string multyByteString = "";
	ConvertWCHARToChar( multyByteString, m_NickName.c_str() );

	wstring wstrErrorMessage = L"";

	if ( m_SelectUnitClass == CX2Unit::UC_NONE )
	{
		SetAlarmAboutName(  GET_STRING( STR_ID_559 ) );
		return false;
	}
	else if( true == CX2Main::IsInvalidNickName( m_NickName, wstrErrorMessage ) )
	{
		SetAlarmAboutName( wstrErrorMessage.c_str() );
		wstring wstrName( L"Emotion_No" );
		m_pNowSelectedUnitViewerUI->PlayEmotion( CX2Unit::ET_NO, wstrName );

		return false;
	}

	return true;
}
void CX2BeginningGame::OpenNickNameBox(float fDelayTime /* = -1.f*/ )
{
	if(fDelayTime == -1.0f)
	{
		m_pDLGCreateUnitNickName->GetControl( L"Character_Create_ID" )->SetShow( true );
		m_pDLGCreateUnitNickName->GetControl( L"Character_Create")->SetShow( true );
		m_pDLGCreateUnitNickName->GetControl( L"ButtonCreate" )->SetShow( true );

		m_pDLGCreateUnitNickName->GetControl( L"IMEEditBoxUnitCreateNickName" )->SetEnable( true );
		m_pDLGCreateUnitNickName->GetControl( L"IMEEditBoxUnitCreateNickName" )->RequestFocus();
		m_pDLGCreateUnitNickName->SetShowEnable(true, true);
		m_pDLGCreateUnitNickName->SetFront(true);
	}
	else
	{
		m_pDLGCreateUnitNickName->SetShowEnable(false, false);
		m_fRemainTimeOpenNickNameBox = fDelayTime;
	}
}

void CX2BeginningGame::SetAlarmAboutName( const WCHAR* pContent )
{
	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitNickName->GetControl(L"Character_Create_ID" );
	if ( pStatic != NULL && pStatic->GetString(0) != NULL )
	{
		pStatic->GetString(0)->msg = pContent;
	}
}

void CX2BeginningGame::InitDummyUnits()
{
	m_pDummyElsword		= NULL;
	m_pDummyArme		= NULL;
	m_pDummyLire		= NULL;
	m_pDummyRaven		= NULL;
	m_pDummyEve			= NULL;
#ifdef NEW_CHARACTER_CHUNG
	m_pDummyChung		= NULL;
#endif NEW_CHARACTER_CHUNG

	KUnitInfo kUnitInfo;
	kUnitInfo.Init();
	kUnitInfo.m_wstrNickName = L"DummyElsword";
	kUnitInfo.m_cUnitClass = CX2Unit::UC_ELSWORD_LORD_KNIGHT;
	m_pDummyElsword = new CX2Unit( kUnitInfo );
	//ReadyUnitResources( m_pDummyElsword );

	kUnitInfo.Init();
	kUnitInfo.m_wstrNickName = L"DummyArme";
	kUnitInfo.m_cUnitClass = CX2Unit::UC_ARME_VOID_PRINCESS;
	m_pDummyArme = new CX2Unit( kUnitInfo );
	//ReadyUnitResources( m_pDummyArme );

	kUnitInfo.Init();
	kUnitInfo.m_wstrNickName = L"DummyLire";
	kUnitInfo.m_cUnitClass = CX2Unit::UC_LIRE_GRAND_ARCHER;
	m_pDummyLire = new CX2Unit( kUnitInfo );
	//ReadyUnitResources( m_pDummyLire );

	kUnitInfo.Init();
	kUnitInfo.m_wstrNickName = L"DummyRaven";
	kUnitInfo.m_cUnitClass = CX2Unit::UC_RAVEN_RECKLESS_FIST;
	m_pDummyRaven = new CX2Unit( kUnitInfo );
	//ReadyUnitResources( m_pDummyRaven );



	kUnitInfo.Init();
	kUnitInfo.m_wstrNickName = L"DummyEve";
	kUnitInfo.m_cUnitClass = CX2Unit::UC_EVE_CODE_EMPRESS;
	m_pDummyEve = new CX2Unit( kUnitInfo );
	//ReadyUnitResources( m_pDummyEve );

#ifdef NEW_CHARACTER_CHUNG
	kUnitInfo.Init();
	kUnitInfo.m_wstrNickName = L"DummyChung";
	kUnitInfo.m_cUnitClass = CX2Unit::UC_CHUNG_IRON_CANNON;
	m_pDummyChung = new CX2Unit( kUnitInfo );
	//ReadyUnitResources( m_pDummyChung );
#endif NEW_CHARACTER_CHUNG

	ReadyPromotionUnitResources();

}


void CX2BeginningGame::ReadyPromotionUnitResources()
{
	const CX2Item::ItemTemplet* pTemplet = NULL;

	for( int i=0; i<ARRAY_SIZE(ELSWORD_PROMOTION_ITEM_ID); i++ )
	{
		pTemplet = g_pData->GetItemManager()->GetItemTemplet( ELSWORD_PROMOTION_ITEM_ID[i] );
		if( pTemplet != NULL )
			XSkinMeshReady( pTemplet->GetModelName( 0 ) );
	}

	for( int i=0; i<ARRAY_SIZE(AISHA_PROMOTION_ITEM_ID); i++ )
	{
		pTemplet = g_pData->GetItemManager()->GetItemTemplet( AISHA_PROMOTION_ITEM_ID[i] );
		if( pTemplet != NULL )
			XSkinMeshReady( pTemplet->GetModelName( 0 ) );
	}

	for( int i=0; i<ARRAY_SIZE(RENA_PROMOTION_ITEM_ID); i++ )
	{
		pTemplet = g_pData->GetItemManager()->GetItemTemplet( RENA_PROMOTION_ITEM_ID[i] );
		if( pTemplet != NULL )
			XSkinMeshReady( pTemplet->GetModelName( 0 ) );
	}

	for( int i=0; i<ARRAY_SIZE(RAVEN_PROMOTION_ITEM_ID); i++ )
	{
		pTemplet = g_pData->GetItemManager()->GetItemTemplet( RAVEN_PROMOTION_ITEM_ID[i] );
		if( pTemplet != NULL )
			XSkinMeshReady( pTemplet->GetModelName( 0 ) );
	}


	for( int i=0; i<ARRAY_SIZE(EVE_PROMOTION_ITEM_ID); i++ )
	{
		if( pTemplet != NULL )
			XSkinMeshReady( pTemplet->GetModelName( 0 ) );
	}

	//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG 

	for( int i=0; i<ARRAY_SIZE(CHUNG_PROMOTION_ITEM_ID); i++ )
	{
		pTemplet = g_pData->GetItemManager()->GetItemTemplet( CHUNG_PROMOTION_ITEM_ID[i] );
		if( pTemplet != NULL )
			XSkinMeshReady( pTemplet->GetModelName( 0 ) );
	}

#endif	NEW_CHARACTER_CHUNG
	//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

}


bool CX2BeginningGame::ReadyUnitResources( CX2Unit* pUnit )
{
	if ( pUnit == NULL )
		return false;

	for( int j =0; j < (int)pUnit->GetUnitData().m_NowEqipItemUIDList.size(); j++ )
	{
		CX2Item* pItem = pUnit->GetInventory().GetItem( pUnit->GetUnitData().m_NowEqipItemUIDList[j] );
        XSkinMeshReady( pItem->GetItemTemplet()->GetModelName(0) );
	}


	const CX2Item::ItemTemplet*    pTemplet;
	pTemplet = g_pData->GetItemManager()->GetItemTemplet( pUnit->GetUnitTemplet()->m_BasicHairItemID );
	if( pTemplet != NULL )
		XSkinMeshReady( pTemplet->GetModelName(0) );
	pTemplet = g_pData->GetItemManager()->GetItemTemplet( pUnit->GetUnitTemplet()->m_BasicFaceItemID );
	if( pTemplet != NULL )
		XSkinMeshReady( pTemplet->GetModelName(0) );
	pTemplet = g_pData->GetItemManager()->GetItemTemplet( pUnit->GetUnitTemplet()->m_BasicBodyItemID );
	if( pTemplet != NULL )
		XSkinMeshReady( pTemplet->GetModelName(0) );
	pTemplet = g_pData->GetItemManager()->GetItemTemplet( pUnit->GetUnitTemplet()->m_BasicLegItemID );
	if( pTemplet != NULL )
		XSkinMeshReady( pTemplet->GetModelName(0) );
	pTemplet = g_pData->GetItemManager()->GetItemTemplet( pUnit->GetUnitTemplet()->m_BasicHandItemID );
	if( pTemplet != NULL )
		XSkinMeshReady( pTemplet->GetModelName(0) );
	pTemplet = g_pData->GetItemManager()->GetItemTemplet( pUnit->GetUnitTemplet()->m_BasicFootItemID );
	if( pTemplet != NULL )
		XSkinMeshReady( pTemplet->GetModelName(0) );
	pTemplet = g_pData->GetItemManager()->GetItemTemplet( pUnit->GetUnitTemplet()->m_BasicWeaponItemID );
	if( pTemplet != NULL )
		XSkinMeshReady( pTemplet->GetModelName(0) );

	// 레이븐은 다른 캐릭터와 다르게 왼팔, 오른팔 메시를 나누어서 사용한다
	pTemplet = g_pData->GetItemManager()->GetItemTemplet( pUnit->GetUnitTemplet()->m_RavenLeftArmItemID );
	if( pTemplet != NULL )
		XSkinMeshReady( pTemplet->GetModelName(0) );

	// dmlee 2008.12.12 - 두 번째 무기
	pTemplet = g_pData->GetItemManager()->GetItemTemplet( pUnit->GetUnitTemplet()->m_SecondWeaponItemID );
	if( pTemplet != NULL )
		XSkinMeshReady( pTemplet->GetModelName(0) );


	return true;
}



void CX2BeginningGame::EquipPromotionItem( CX2Unit::UNIT_CLASS eUnitClass )
{
	switch( eUnitClass )
	{
	case CX2Unit::UC_ELSWORD_SWORDMAN:
		{
			for( int i=0; i<ARRAY_SIZE(ELSWORD_PROMOTION_ITEM_ID); i++ )
			{
				m_pNowSelectedUnitViewerUI->AddEquipByTID( ELSWORD_PROMOTION_ITEM_ID[i] );
			}
		} break;

	case CX2Unit::UC_ARME_VIOLET_MAGE:
		{
			for( int i=0; i<ARRAY_SIZE(AISHA_PROMOTION_ITEM_ID); i++ )
			{
				m_pNowSelectedUnitViewerUI->AddEquipByTID( AISHA_PROMOTION_ITEM_ID[i] );
			}
		} break;

	case CX2Unit::UC_LIRE_ELVEN_RANGER:
		{
			for( int i=0; i<ARRAY_SIZE(RENA_PROMOTION_ITEM_ID); i++ )
			{
				m_pNowSelectedUnitViewerUI->AddEquipByTID( RENA_PROMOTION_ITEM_ID[i] );
			}
		} break;

	case CX2Unit::UC_RAVEN_FIGHTER:
		{
			for( int i=0; i<ARRAY_SIZE(RAVEN_PROMOTION_ITEM_ID); i++ )
			{
				m_pNowSelectedUnitViewerUI->AddEquipByTID( RAVEN_PROMOTION_ITEM_ID[i] );
			}
		} break;

	case CX2Unit::UC_EVE_NASOD:
		{
			for( int i=0; i<ARRAY_SIZE(EVE_PROMOTION_ITEM_ID); i++ )
			{
				m_pNowSelectedUnitViewerUI->AddEquipByTID( EVE_PROMOTION_ITEM_ID[i] );
			}
		} break;

		//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UC_CHUNG_IRON_CANNON:
		{
			for( int i=0; i<ARRAY_SIZE(CHUNG_PROMOTION_ITEM_ID); i++ )
			{
				m_pNowSelectedUnitViewerUI->AddEquipByTID( CHUNG_PROMOTION_ITEM_ID[i] );
			}
		} break;
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG
	}

}

void CX2BeginningGame::CreateElSword()
{
	CX2UnitViewerUI* pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	g_pKTDXApp->GetDGManager()->AddObjectChain( pUnitViewerUI );

	pUnitViewerUI->SetUnit( m_pDummyElsword, CX2UnitViewerUI::UVS_CREATEUNIT ); 

	pUnitViewerUI->GetMatrix().Move( CHARACTER_CREATE_POSITION[0] );
	pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(160),D3DXToRadian(0));
	pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3(1.6f, 1.6f, 1.6f) );
	//m_pNowSelectedUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	pUnitViewerUI->PlayAnim( L"LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_ELSWORD_SWORDMAN;
	m_pNowSelectedUnitViewerUI = pUnitViewerUI;
	EquipPromotionItem( m_SelectUnitClass );

	m_CreateUnitViewerUIList.push_back( pUnitViewerUI );

	return;
}


void CX2BeginningGame::CreateArme()
{
	CX2UnitViewerUI* pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	g_pKTDXApp->GetDGManager()->AddObjectChain( pUnitViewerUI );

	pUnitViewerUI->SetUnit( m_pDummyArme, CX2UnitViewerUI::UVS_CREATEUNIT ); 

	pUnitViewerUI->GetMatrix().Move( CHARACTER_CREATE_POSITION[1] );
	pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(160),D3DXToRadian(0));
	pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3(1.6f, 1.6f, 1.6f) );
	//m_pNowSelectedUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	pUnitViewerUI->PlayAnim( L"LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_ARME_VIOLET_MAGE;
	m_pNowSelectedUnitViewerUI = pUnitViewerUI;
	EquipPromotionItem( m_SelectUnitClass );

	m_CreateUnitViewerUIList.push_back( pUnitViewerUI );

	return;

}

void CX2BeginningGame::CreateLire()
{
	CX2UnitViewerUI* pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	g_pKTDXApp->GetDGManager()->AddObjectChain( pUnitViewerUI );

	pUnitViewerUI->SetUnit( m_pDummyLire, CX2UnitViewerUI::UVS_CREATEUNIT ); 

	pUnitViewerUI->GetMatrix().Move( CHARACTER_CREATE_POSITION[2] );
	pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(160),D3DXToRadian(0));
	pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3(1.6f, 1.6f, 1.6f) );
	//m_pNowSelectedUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	pUnitViewerUI->PlayAnim( L"LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_LIRE_ELVEN_RANGER;
	m_pNowSelectedUnitViewerUI = pUnitViewerUI;
	EquipPromotionItem( m_SelectUnitClass );

	m_CreateUnitViewerUIList.push_back( pUnitViewerUI );

	return;

}


void CX2BeginningGame::CreateRaven()
{
	CX2UnitViewerUI* pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	g_pKTDXApp->GetDGManager()->AddObjectChain( pUnitViewerUI );

	pUnitViewerUI->SetUnit( m_pDummyRaven, CX2UnitViewerUI::UVS_CREATEUNIT ); 

	pUnitViewerUI->GetMatrix().Move( CHARACTER_CREATE_POSITION[3] );
	pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(160),D3DXToRadian(0));
	pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3(1.6f, 1.6f, 1.6f) );
	//m_pNowSelectedUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	pUnitViewerUI->PlayAnim( L"LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_RAVEN_FIGHTER;
	m_pNowSelectedUnitViewerUI = pUnitViewerUI;
	EquipPromotionItem( m_SelectUnitClass );

	m_CreateUnitViewerUIList.push_back( pUnitViewerUI );

	return;

}

void CX2BeginningGame::CreateEve()
{
	CX2UnitViewerUI* pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	g_pKTDXApp->GetDGManager()->AddObjectChain( pUnitViewerUI );

	pUnitViewerUI->SetUnit( m_pDummyEve, CX2UnitViewerUI::UVS_CREATEUNIT ); 

	pUnitViewerUI->GetMatrix().Move( CHARACTER_CREATE_POSITION[4] );
	pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(160),D3DXToRadian(0));
	pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3(1.6f, 1.6f, 1.6f) );
	//m_pNowSelectedUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	pUnitViewerUI->PlayAnim( L"LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_EVE_NASOD;
	m_pNowSelectedUnitViewerUI = pUnitViewerUI;
	EquipPromotionItem( m_SelectUnitClass );

	m_CreateUnitViewerUIList.push_back( pUnitViewerUI );

	return;

}
#ifdef NEW_CHARACTER_CHUNG
void CX2BeginningGame::CreateChung()
{
	CX2UnitViewerUI* pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	g_pKTDXApp->GetDGManager()->AddObjectChain( pUnitViewerUI );

	pUnitViewerUI->SetUnit( m_pDummyChung, CX2UnitViewerUI::UVS_CREATEUNIT ); 

	pUnitViewerUI->GetMatrix().Move( CHARACTER_CREATE_POSITION[5] );
	pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(160),D3DXToRadian(0));
	pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3(1.6f, 1.6f, 1.6f) );
	//m_pNowSelectedUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	pUnitViewerUI->PlayAnim( L"LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_CHUNG_IRON_CANNON;
	m_pNowSelectedUnitViewerUI = pUnitViewerUI;
	EquipPromotionItem( m_SelectUnitClass );

	m_CreateUnitViewerUIList.push_back( pUnitViewerUI );

	return;

}
#endif NEW_CHARACTER_CHUNG


void CX2BeginningGame::UnSelect()
{
	m_pNowSelectedUnitViewerUI = NULL;

	m_SelectUnitClass = CX2Unit::UC_NONE;
	m_bSelectUnit = false;
}

void CX2BeginningGame::SelectElSword()
{
	if( m_SelectUnitClass != CX2Unit::UC_NONE )
		return;

	if ( m_bSelectUnit == false )
	{
		//StaticControlAnimate();	

		m_bSelectUnit = true;
	}

	OpenNickNameBox(2.0f);
	PlayWorldCamera(4);
	m_pNowSelectedUnitViewerUI = m_CreateUnitViewerUIList[0];

	m_pNowSelectedUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	m_pNowSelectedUnitViewerUI->PlayAnim( L"Emotion_Hello", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_ELSWORD_SWORDMAN;


	m_pNowSelectedUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );



/*
	CKTDGUIStatic* pStatic = NULL;


	for ( int i = 0; i < CX2Unit::UT_END - 1; i++ )
	{
		WCHAR buff[256] = {0};
		StringCchPrintf( buff, 256, L"StaticUnit_Create_Unit%d", i );
		//wsprintf( buff, L"StaticUnit_Create_Unit%d", i );
		pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( buff );		// note!! 신캐릭터 추가시 스크립트에 추가
		if ( pStatic != NULL )
			pStatic->SetShow( false );
	}

	pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( L"StaticUnit_Create_Unit0" );
	if ( pStatic != NULL )
		pStatic->SetShow( true );


*/


	g_pKTDXApp->SkipFrame();
}

void CX2BeginningGame::SelectArme()
{
	if( m_SelectUnitClass != CX2Unit::UC_NONE )
		return;

	if ( m_bSelectUnit == false )
	{
		//StaticControlAnimate();

		m_bSelectUnit = true;
	}

	OpenNickNameBox(2.0f);
	PlayWorldCamera(6);

#ifdef ELSWORD_NEW_BEGINNING
	m_pNowSelectedUnitViewerUI = m_CreateUnitViewerUIList[1];

	m_pNowSelectedUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	m_pNowSelectedUnitViewerUI->PlayAnim( L"Emotion_Dance", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_ARME_VIOLET_MAGE;

#endif ELSWORD_NEW_BEGINNING

	m_pNowSelectedUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );
	m_pNowSelectedUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );


/*
	CKTDGUIStatic* pStatic = NULL;


	for ( int i = 0; i < CX2Unit::UT_END - 1; i++ )
	{
		WCHAR buff[256] = {0};
		StringCchPrintf( buff, 256, L"StaticUnit_Create_Unit%d", i );
		//wsprintf( buff, L"StaticUnit_Create_Unit%d", i );
		pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( buff );		// note!! 신캐릭터 추가시 스크립트에 추가
		if ( pStatic != NULL )
			pStatic->SetShow( false );
	}

	pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( L"StaticUnit_Create_Unit1" );
	if( pStatic != NULL )
		pStatic->SetShow( true );
*/



	g_pKTDXApp->SkipFrame();
}


void CX2BeginningGame::SelectLire()
{
	if( m_SelectUnitClass != CX2Unit::UC_NONE )
		return;

	if ( m_bSelectUnit == false )
	{	
		//StaticControlAnimate();

		m_bSelectUnit = true;
	}
	OpenNickNameBox(2.0f);
	PlayWorldCamera(8);

#ifdef ELSWORD_NEW_BEGINNING
	m_pNowSelectedUnitViewerUI = m_CreateUnitViewerUIList[2];

	m_pNowSelectedUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	m_pNowSelectedUnitViewerUI->PlayAnim( L"Emotion_Kiss", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_LIRE_ELVEN_RANGER;

#endif ELSWORD_NEW_BEGINNING

	m_pNowSelectedUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );
	m_pNowSelectedUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );

/*
	CKTDGUIStatic* pStatic = NULL;

	for ( int i = 0; i < CX2Unit::UT_END - 1; i++ )
	{
		WCHAR buff[256] = {0};
		StringCchPrintf( buff, 256, L"StaticUnit_Create_Unit%d", i );
		//wsprintf( buff, L"StaticUnit_Create_Unit%d", i );
		pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( buff );			// note!! 신캐릭터 추가시 스크립트에 추가
		if ( pStatic != NULL )
			pStatic->SetShow( false );
	}

	pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( L"StaticUnit_Create_Unit2" );
	if ( pStatic != NULL )
		pStatic->SetShow( true );

*/




	g_pKTDXApp->SkipFrame();
}

void CX2BeginningGame::SelectRaven()
{
	if( m_SelectUnitClass != CX2Unit::UC_NONE )
		return;

	if ( m_bSelectUnit == false )
	{
		//StaticControlAnimate();	

		m_bSelectUnit = true;
	}

	OpenNickNameBox(2.0f);
	PlayWorldCamera(10);

#ifdef ELSWORD_NEW_BEGINNING
	m_pNowSelectedUnitViewerUI = m_CreateUnitViewerUIList[3];

	m_pNowSelectedUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	m_pNowSelectedUnitViewerUI->PlayAnim( L"Emotion_Sad", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_RAVEN_FIGHTER;

#endif ELSWORD_NEW_BEGINNING

	m_pNowSelectedUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );





/*
	CKTDGUIStatic* pStatic = NULL;


	for ( int i = 0; i < CX2Unit::UT_END - 1; i++ )
	{
		WCHAR buff[256] = {0};
		StringCchPrintf( buff, 256, L"StaticUnit_Create_Unit%d", i );
		//wsprintf( buff, L"StaticUnit_Create_Unit%d", i );
		pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( buff );			// note!! 신캐릭터 추가시 스크립트에 추가
		if ( pStatic != NULL )
			pStatic->SetShow( false );
	}

	pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( L"StaticUnit_Create_Unit3" );
	if ( pStatic != NULL )
		pStatic->SetShow( true );

*/

	g_pKTDXApp->SkipFrame();
}


void CX2BeginningGame::SelectEve()
{

	if( m_SelectUnitClass != CX2Unit::UC_NONE )
		return;

	if ( m_bSelectUnit == false )
	{
		//StaticControlAnimate();	

		m_bSelectUnit = true;
	}

	OpenNickNameBox(2.0f);
	PlayWorldCamera(12);

#ifdef ELSWORD_NEW_BEGINNING
	m_pNowSelectedUnitViewerUI = m_CreateUnitViewerUIList[4];

	m_pNowSelectedUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	m_pNowSelectedUnitViewerUI->PlayAnim( L"Emotion_No", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_EVE_NASOD;

#endif ELSWORD_NEW_BEGINNING

	m_pNowSelectedUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );


/*
	CKTDGUIStatic* pStatic = NULL;

	for ( int i = 0; i < CX2Unit::UT_END - 1; i++ )
	{
		WCHAR buff[256] = {0};
		StringCchPrintf( buff, 256, L"StaticUnit_Create_Unit%d", i );
		//wsprintf( buff, L"StaticUnit_Create_Unit%d", i );
		pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( buff );			// note!! 신캐릭터 추가시 스크립트에 추가
		if ( pStatic != NULL )
			pStatic->SetShow( false );
	}

	pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( L"StaticUnit_Create_Unit4" );	
	if ( pStatic != NULL )
		pStatic->SetShow( true );
*/

	g_pKTDXApp->SkipFrame();
}


//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG

/** @function : SelectChung
	@brief : 청을 선택했을 때 동작하는 함수로 닉네임박스, 캐릭터 UnitViewer 등을 보여준다.
*/
void CX2BeginningGame::SelectChung()
{
	if( m_SelectUnitClass != CX2Unit::UC_NONE )
		return;

	if ( m_bSelectUnit == false )
	{
		//StaticControlAnimate();	

		m_bSelectUnit = true;
	}

	OpenNickNameBox(2.0f);
	PlayWorldCamera(14);

#ifdef ELSWORD_NEW_BEGINNING
	m_pNowSelectedUnitViewerUI = m_CreateUnitViewerUIList[5];

	m_pNowSelectedUnitViewerUI->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	m_pNowSelectedUnitViewerUI->PlayAnim( L"LobbyLevelUp", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_SelectUnitClass = CX2Unit::UC_CHUNG_IRON_CANNON;

#endif ELSWORD_NEW_BEGINNING

	m_pNowSelectedUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );


/*
	const int STR_BUF_SIZE = 256;
	for ( int i = 0; i < CX2Unit::UT_END - 1; i++ )
	{
		WCHAR buff[STR_BUF_SIZE] = {0, };
		StringCchPrintf( buff, 256, L"StaticUnit_Create_Unit%d", i );
		
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( buff );			// note!! 신캐릭터 추가시 스크립트에 추가
		if ( pStatic != NULL )
			pStatic->SetShow( false );
	}

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGCreateUnitBack->GetControl( L"StaticUnit_Create_Unit5" );	
	
	if ( pStatic != NULL )
		pStatic->SetShow( true );
*/

	g_pKTDXApp->SkipFrame();
}

#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG




bool CX2BeginningGame::GetShowMsgBox()
{	
	CKTDGUIDialogType pDialog = NULL;

	if(g_pKTDXApp->GetDGManager()->GetDialogManager() != NULL)
	{
		pDialog = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetDialog(L"DLG_OKMsgBox");
		if(pDialog != NULL)
		{
			return pDialog->GetShow();
		}

		pDialog = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetDialog(L"DLG_MsgBox");
		if(pDialog != NULL)
		{
			return pDialog->GetShow();
		}

		pDialog = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetDialog(L"OkAndCancelMsgBox");
		if(pDialog != NULL)
		{
			return pDialog->GetShow();
		}
	}
	return false;
}

void CX2BeginningGame::SetEnableCreateUnit(bool bEnable)
{
	m_bEnableCreateUnit = bEnable;

	for ( int i = 0; i < (int)m_CreateUnitViewerUIList.size(); i++ )
	{
		CX2UnitViewerUI* pUnitViewerUI = m_CreateUnitViewerUIList[i];

		if ( pUnitViewerUI == NULL )
			continue;
		pUnitViewerUI->SetShowObject(bEnable);
	}

	if(false == bEnable)
	{
		m_pDLGCreateUnitNickName->SetShowEnable(false, false);
		UnSelect();
	}
}

void CX2BeginningGame::RenderLoadingFace(int iAnimStep)
{
	g_pKTDXApp->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
	g_pKTDXApp->GetDevice()->BeginScene(); 

	int iStep = iAnimStep * 4;

	m_pLoadingFaceTex->SetDeviceTexture();
	m_pLoadingFaceTex->Draw(LOADING_ANIM_STEP[iStep + 0], 
		LOADING_ANIM_STEP[iStep + 1], 
		LOADING_ANIM_STEP[iStep + 2], 
		LOADING_ANIM_STEP[iStep + 3], 
		LOADING_TEX_UV[0], LOADING_TEX_UV[1], 
		LOADING_TEX_UV[2], LOADING_TEX_UV[3]);

	g_pKTDXApp->GetDevice()->EndScene();
	g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
	return;
}



#endif ELSWORD_NEW_BEGINNING
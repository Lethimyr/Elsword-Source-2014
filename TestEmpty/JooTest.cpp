#include "dxstdafx.h"
#include ".\jootest.h"

CJooViewerUI::CJooViewerUI( const WCHAR* pMotionFileName )
{
	m_pXSkinAnim	= NULL;
	m_pBodyModel	= NULL;

	m_pMotion		= NULL;

	INIT_VECTOR3( m_LightPos, 1000, 1000, 1000 );
	m_BillBoardType	= CKTDGMatrix::BT_NONE;

	if ( pMotionFileName != NULL )
		m_pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( pMotionFileName );

	m_pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
	m_pXSkinAnim->SetAnimXSkinMesh( m_pMotion, NULL );
	//m_pXSkinAnim->AddModelXSkinMesh( m_pMotion );

	m_pBodyModel = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Mesh_Lire_Basic_Body_Sorted.X" );
	m_pXSkinAnim->AddModelXSkinMesh( m_pBodyModel );
}

CJooViewerUI::~CJooViewerUI()
{
	SAFE_CLOSE( m_pMotion );
	SAFE_CLOSE( m_pBodyModel );

	SAFE_DELETE( m_pXSkinAnim );
}

HRESULT CJooViewerUI::OnFrameMove( double fTime, float fElapsedTime )
{
	m_pXSkinAnim->SetBillBoardType( m_BillBoardType );
	m_pXSkinAnim->GetMatrix()->Move( m_pMatrix->GetPos() );
	m_pXSkinAnim->GetMatrix()->Rotate( m_pMatrix->GetRotate() );
	m_pXSkinAnim->GetMatrix()->Scale( m_pMatrix->GetScale() );
	m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );

	static D3DXVECTOR3 tempVec = D3DXVECTOR3( 0, -50, 0 );

	tempVec.y += 0.2f;

	CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinAnim->GetRenderParam();
	pRenderParam->renderType		= CKTDGXRenderer::RT_LIGHT_FLOW_WORLD_POINT;
	//pRenderParam->texOffsetStage0	= D3DXVECTOR2( 0.5f, 0.5f );
	pRenderParam->fLightFlowWide	= 2.0f;
	pRenderParam->fLightFlowImpact	= 1.0f;
	pRenderParam->lightFlowPoint	= tempVec;
	pRenderParam->worldMatrix		= m_pMatrix->GetMatrix( m_BillBoardType );
	pRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
	pRenderParam->lightPos			= m_LightPos;
	pRenderParam->fOutLineWide		= 1.5f;
	pRenderParam->outLineColor		= 0xffffffff;
	pRenderParam->color				= 0xffff00ff;
	pRenderParam->bAlphaBlend		= true;

	return S_OK;
}

HRESULT CJooViewerUI::OnFrameRender()
{
	m_pXSkinAnim->OnFrameRender();

	return S_OK;
}

HRESULT CJooViewerUI::OnResetDevice()
{
	m_pXSkinAnim->OnResetDevice();

	return S_OK;
}

HRESULT CJooViewerUI::OnLostDevice()
{
	m_pXSkinAnim->OnLostDevice();

	return S_OK;
}

void CJooViewerUI::PlayAnim( const WCHAR* pName, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType, bool bTransition )
{
	m_pXSkinAnim->ChangeAnim( pName, bTransition );
	m_pXSkinAnim->Play( playType );
}









CJooTest::CJooTest(void)
{
	/*
	m_pJooViewerUI = NULL;


	m_pJooViewerUI = new CJooViewerUI( L"Motion_Lire_UI_Sorted.X" );

	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-700, 0,0,0 );


	m_pJooViewerUI->GetMatrix()->Move( 0, 0, 0 );
	m_pJooViewerUI->GetMatrix()->Scale(  D3DXVECTOR3(1.0f,1.0f,1.0f) );
	m_pJooViewerUI->GetMatrix()->Rotate(D3DXToRadian(0),D3DXToRadian(70),D3DXToRadian(0));
	m_pJooViewerUI->PlayAnim( L"Win", CKTDGXSkinAnim::XAP_LOOP );*/
	
//	CX2World::WorldData temp;

	//m_pX2World = NULL;
	//m_pX2World = new CX2World( NULL );

	//INIT_VECTOR3( m_LightPos, 1000, 1000, 1000 );

	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0, 0, -700, 0,0,0 );
/*
	m_pTestMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Mesh_Elsword_Basic_Upbody_Sorted.X" );


	m_StartPos = D3DXVECTOR3( -100, 0, 0);
	m_EndPos = D3DXVECTOR3( 100, 0, 0 );

	m_StartPos2 = D3DXVECTOR3( 30, 100, 0 );
	m_EndPos2 = D3DXVECTOR3( 30, -100, 0 );

	//m_BillBoardType	= CKTDGMatrix::BT_NONE;


	m_pLineVB = NULL;

	HRESULT hr;

	if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( 2 * sizeof(LINE_VERTEX), 
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_LINE_VERTEX, 
		D3DPOOL_SYSTEMMEM, &m_pLineVB, NULL) ))
	{
		MessageBox(NULL, L"개털됬다1", L"dd", MB_OK );
	}


	LINE_VERTEX* pVertices;

	if(FAILED(hr = m_pLineVB->Lock( 0, 2 * sizeof(LINE_VERTEX), (void**) &pVertices, D3DLOCK_DISCARD )))
		MessageBox(NULL, L"개털됬다2", L"dd", MB_OK );
	

	pVertices[0].pos		= m_StartPos;
	pVertices[0].color		= 0xffff00ff;
	pVertices[1].pos		= m_EndPos;
	pVertices[1].color		= 0xffff00ff;
	
	if(FAILED(hr = m_pLineVB->Unlock()))
		MessageBox(NULL, L"개털됬다3", L"dd", MB_OK );


	m_pLineVB2 = NULL;



	if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( 2 * sizeof(LINE_VERTEX), 
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_LINE_VERTEX, 
		D3DPOOL_SYSTEMMEM, &m_pLineVB2, NULL) ))
	{
		MessageBox(NULL, L"개털됬다1", L"dd", MB_OK );
	}


	LINE_VERTEX* pVertices2;

	if(FAILED(hr = m_pLineVB2->Lock( 0, 2 * sizeof(LINE_VERTEX), (void**) &pVertices2, D3DLOCK_DISCARD )))
		MessageBox(NULL, L"개털됬다2", L"dd", MB_OK );


	pVertices2[0].pos		= m_StartPos2;
	pVertices2[0].color		= 0xff0000ff;
	pVertices2[1].pos		= m_EndPos2;
	pVertices2[1].color		= 0xff0000ff;

	if(FAILED(hr = m_pLineVB2->Unlock()))
		MessageBox(NULL, L"개털됬다3", L"dd", MB_OK );


	//g_pKTDXApp->GetDGManager()->GetCamera().RotateY( 1.4f );

	m_bCollision = false;

	m_pFont = NULL;

	
	m_pFont = g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewFont( L"돋움체", 20,20, FW_NORMAL );


	m_SelectedOBJ = SO_CAMERA;


	m_MeshPos = D3DXVECTOR3(0,0,0);
	m_MeshRot = D3DXVECTOR3(0,0,0);
	*/

	m_pDeviceXSkinMesh = NULL;
	m_pDeviceXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh(  L"Motion_Elsword_Ui_Sorted2.X" );

}

CJooTest::~CJooTest(void)
{
	SAFE_CLOSE( m_pDeviceXSkinMesh );

/*
	SAFE_RELEASE( m_pLineVB );
	SAFE_RELEASE( m_pLineVB2 );
	SAFE_CLOSE( m_pTestMesh );
	*/
	//SAFE_DELETE( m_pJooViewerUI );
	
	//SAFE_DELETE( m_pX2World );

	//SAFE_DELETE( m_pWOSkinMesh );


}

HRESULT CJooTest::OnFrameMove( double fTime, float fElapsedTime )
{
	/*
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_SPACE ) == TRUE )
	{
		m_SelectedOBJ++;

		if ( m_SelectedOBJ > SO_LINE2 )
			m_SelectedOBJ = SO_CAMERA;
	}

	
	if ( m_SelectedOBJ == SO_CAMERA )
	{

		D3DXVECTOR3 lookPT = g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt();

		static float angle = 0.0f;

		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_RIGHT ) == TRUE )
		{
			angle += fElapsedTime;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_LEFT ) == TRUE )
		{
			angle -= fElapsedTime;
		}
		

		g_pKTDXApp->GetDGManager()->GetCamera().Point( (cosf( angle) * 1000 ) +lookPT.x, lookPT.y, (sinf( angle ) * 1000) +lookPT.z,
			lookPT.x, lookPT.y, lookPT.z);

	}
	else if ( m_SelectedOBJ == SO_MESH )
	{
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_RIGHT ) == TRUE )
		{
			m_MeshPos.x += fElapsedTime * 10;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_LEFT ) == TRUE )
		{
			m_MeshPos.x -= fElapsedTime * 10;
		}

		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_UP ) == TRUE )
		{
			m_MeshPos.y += fElapsedTime * 10;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_DOWN ) == TRUE )
		{
			m_MeshPos.y -= fElapsedTime * 10;
		}

		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_N ) == TRUE )
		{
			m_MeshPos.z += fElapsedTime * 10;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_M ) == TRUE )
		{
			m_MeshPos.z -= fElapsedTime * 10;
		}


		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_A ) == TRUE )
		{
			m_MeshRot.y += fElapsedTime * 10;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_D ) == TRUE )
		{
			m_MeshRot.y -= fElapsedTime * 10;
		}

		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_W ) == TRUE )
		{
			m_MeshRot.x += fElapsedTime * 10;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_S ) == TRUE )
		{
			m_MeshRot.x -= fElapsedTime * 10;
		}
	}
	else if ( m_SelectedOBJ == SO_LINE1 )
	{
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_RIGHT ) == TRUE )
		{
			m_StartPos.x += fElapsedTime * 10;
			m_EndPos.x += fElapsedTime * 10;

		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_LEFT ) == TRUE )
		{
			m_StartPos.x -= fElapsedTime * 10;
			m_EndPos.x -= fElapsedTime * 10;
		}

		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_UP ) == TRUE )
		{
			m_StartPos.y += fElapsedTime * 10;
			m_EndPos.y += fElapsedTime * 10;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_DOWN ) == TRUE )
		{
			m_StartPos.y -= fElapsedTime * 10;
			m_EndPos.y -= fElapsedTime * 10;
		}

		HRESULT hr;

		LINE_VERTEX* pVertices;

		if(FAILED(hr = m_pLineVB->Lock( 0, 2 * sizeof(LINE_VERTEX), (void**) &pVertices, D3DLOCK_DISCARD )))
			MessageBox(NULL, L"개털됬다2", L"dd", MB_OK );


		pVertices[0].pos		= m_StartPos;
		pVertices[0].color		= 0xffff00ff;
		pVertices[1].pos		= m_EndPos;
		pVertices[1].color		= 0xffff00ff;

		if(FAILED(hr = m_pLineVB->Unlock()))
			MessageBox(NULL, L"개털됬다3", L"dd", MB_OK );
	}
	else if ( m_SelectedOBJ == SO_LINE2 )
	{
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_RIGHT ) == TRUE )
		{
			m_StartPos2.x += fElapsedTime * 10;
			m_EndPos2.x += fElapsedTime * 10;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_LEFT ) == TRUE )
		{
			m_StartPos2.x -= fElapsedTime * 10;
			m_EndPos2.x -= fElapsedTime * 10;
		}

		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_UP ) == TRUE )
		{
			m_StartPos2.y += fElapsedTime * 10;
			m_EndPos2.y += fElapsedTime * 10;
		}
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_DOWN ) == TRUE )
		{
			m_StartPos2.y -= fElapsedTime * 10;
			m_EndPos2.y -= fElapsedTime * 10;
		}

		HRESULT hr;

		LINE_VERTEX* pVertices;

		if(FAILED(hr = m_pLineVB2->Lock( 0, 2 * sizeof(LINE_VERTEX), (void**) &pVertices, D3DLOCK_DISCARD )))
			MessageBox(NULL, L"개털됬다2", L"dd", MB_OK );


		pVertices[0].pos		= m_StartPos2;
		pVertices[0].color		= 0xff0000ff;
		pVertices[1].pos		= m_EndPos2;
		pVertices[1].color		= 0xff0000ff;

		if(FAILED(hr = m_pLineVB2->Unlock()))
			MessageBox(NULL, L"개털됬다3", L"dd", MB_OK );
	}
	

	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );

	//m_pJooViewerUI->OnFrameMove( fTime, fElapsedTime );

	//m_pX2World->OnFrameMove(fTime, fElapsedTime);

	//m_pWOSkinMesh->OnFrameMove(fTime, fElapsedTime);

	static bool SphereToSphereCheck = false;

*/

	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );

	
	return S_OK;
}


HRESULT CJooTest::OnFrameRender()
{
	//m_pJooViewerUI->OnFrameRender();
	//m_pX2World->OnFrameRender();

	//m_pWOSkinMesh->OnFrameRender();
/*
	D3DXVECTOR3 rayDir = m_EndPos - m_StartPos;
	D3DXVec3Normalize( &rayDir, &rayDir );

	D3DXMATRIX matTrans;
	D3DXMATRIX matRot;
	D3DXMatrixTranslation( &matTrans, m_MeshPos.x, m_MeshPos.y, m_MeshPos.z );
	D3DXMatrixRotationYawPitchRoll(&matRot, m_MeshRot.y, m_MeshRot.x, m_MeshRot.z );
	m_matMeshWorld = matRot * matTrans;



	//랜더 파라메터 세팅
	CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
	pRenderParam->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
	pRenderParam->worldMatrix		= m_matMeshWorld;
	pRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
	pRenderParam->lightPos			= D3DXVECTOR3(100,100,100);
	pRenderParam->bAlphaBlend		= true;
	pRenderParam->color				= 0xffff00ff;

	//랜더
	g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
	g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pTestMesh );
	g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
/*
	D3DXMatrixTranslation(&tempMat, 0, 0, 0);

	//랜더 파라메터 세팅
	pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
	pRenderParam->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
	pRenderParam->worldMatrix		= tempMat;
	pRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
	pRenderParam->lightPos			= D3DXVECTOR3(100,100,100);
	pRenderParam->bAlphaBlend		= true;
	pRenderParam->color				= 0xffff00ff;

	HRESULT hr;

	D3DXMATRIX tempMat;
	D3DXMatrixIdentity(&tempMat);
	g_pKTDXApp->GetDevice()->SetTransform(D3DTS_WORLD, &tempMat);

	if(FAILED(hr = g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_LINE_VERTEX )))
		MessageBox( NULL, L"SetFVF", L"dd", MB_OK );

	if(FAILED(hr = g_pKTDXApp->GetDevice()->SetStreamSource( 0, m_pLineVB, 0, sizeof(LINE_VERTEX) )))
		MessageBox( NULL, L"SetStreamSource", L"dd", MB_OK );

	if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_LINELIST, 0, 1 )))
		MessageBox( NULL, L"DrawPrimitive", L"dd", MB_OK );


	if(FAILED(hr = g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_LINE_VERTEX )))
		MessageBox( NULL, L"SetFVF", L"dd", MB_OK );

	if(FAILED(hr = g_pKTDXApp->GetDevice()->SetStreamSource( 0, m_pLineVB2, 0, sizeof(LINE_VERTEX) )))
		MessageBox( NULL, L"SetStreamSource", L"dd", MB_OK );

	if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_LINELIST, 0, 1 )))
		MessageBox( NULL, L"DrawPrimitive", L"dd", MB_OK );






	float fDist = 0.0f;

	if ( m_SelectedOBJ  == SO_CAMERA  )
		m_pFont->Draw2DText( L"카메라가 선택된 상태입니다.", 500, 40, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) );
	else if ( m_SelectedOBJ  == SO_MESH )
		m_pFont->Draw2DText( L"메쉬가 선택된 상태입니다.", 500, 40, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) );
	else if ( m_SelectedOBJ  == SO_LINE1 )
		m_pFont->Draw2DText( L"분홍색 선이 선택된 상태입니다.", 500, 40, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) );
	else if ( m_SelectedOBJ  == SO_LINE2 )
		m_pFont->Draw2DText( L"파란색 선이 선택된 상태입니다.", 500, 40, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) );


	if ( g_pKTDXApp->GetCollision()->DXRayCheck(m_pTestMesh->GetMesh(), m_StartPos,  rayDir,m_matMeshWorld, &fDist ) == true )
	{
		m_pFont->Draw2DText( L"분홍색 선과 메쉬가 충돌했습니다.", 500, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	static bool RayTest2 = false;

	rayDir = m_EndPos2 - m_StartPos2;
	D3DXVec3Normalize( &rayDir, &rayDir );

	if ( g_pKTDXApp->GetCollision()->DXRayCheck(m_pTestMesh->GetMesh(), m_StartPos2,  rayDir,m_matMeshWorld, &fDist ) == true )
	{
		m_pFont->Draw2DText( L"파란색 선과 메쉬가 충돌했습니다.", 500, 80, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) );
	}

	D3DXVECTOR3 tempCollisionPoint;
	D3DXVECTOR3 tempCollisionPoint2;

	if ( g_pKTDXApp->GetCollision()->LineToLine(m_StartPos,m_EndPos, m_StartPos2, m_EndPos2, 1, tempCollisionPoint, tempCollisionPoint2) == true )
	{
		m_pFont->Draw2DText( L"선끼리 충돌했습니다.", 500, 100, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) );
	}


*/

	return S_OK;
}

HRESULT CJooTest::OnResetDevice()
{
	//m_pJooViewerUI->OnResetDevice();
	//m_pX2World->OnResetDevice();

	//m_pWOSkinMesh->OnResetDevice();
//	m_pTestMesh->OnResetDevice();
	return S_OK;
}

HRESULT CJooTest::OnLostDevice()
{
	//m_pJooViewerUI->OnLostDevice();
	//m_pX2World->OnLostDevice();

	//m_pWOSkinMesh->OnLostDevice();

//	m_pTestMesh->OnLostDevice();
	return S_OK;
}

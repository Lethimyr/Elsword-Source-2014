#include "dxstdafx.h"
#include ".\testmain.h"

CKTDGParticleSystem*	g_pParticleSystem = NULL;
CKTDGXMeshPlayer*		g_pMeshPlayer = NULL;


CTestMain::CTestMain(void)
{
	//Reset();

	CKTDXDeviceXSkinMesh* pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Motion_Elsword_SwordMan.x" );
	CKTDXDeviceXSkinMesh* pEqip1 = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Mesh_Elsword_Basic_Hair_Sorted.X" );
	CKTDXDeviceXSkinMesh* pEqip2 = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Mesh_Elsword_Basic_Face_Sorted.X" );
	CKTDXDeviceXSkinMesh* pEqip3 = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Mesh_Elsword_Basic_Body_Sorted.X" );
	CKTDXDeviceXSkinMesh* pEqip4 = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Mesh_Elsword_Basic_Leg_Sorted.X" );
	CKTDXDeviceXSkinMesh* pEqip5 = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Mesh_Elsword_Basic_Hand_Sorted.X" );
	CKTDXDeviceXSkinMesh* pEqip6 = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Mesh_Elsword_Basic_Foot_Sorted.X" );
	for( int i = 0; i < 20; i++ )
	{
		m_pAnim[i] = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
		m_pAnim[i]->SetAnimXSkinMesh( pMotion );
		m_pAnim[i]->AddModelXSkinMesh( pEqip1 );
		m_pAnim[i]->AddModelXSkinMesh( pEqip2 );
		m_pAnim[i]->AddModelXSkinMesh( pEqip3 );
		m_pAnim[i]->AddModelXSkinMesh( pEqip4 );
		m_pAnim[i]->AddModelXSkinMesh( pEqip5 );
		m_pAnim[i]->AddModelXSkinMesh( pEqip6 );

		m_pAnim[i]->GetMatrix()->Move( -500 + i*50, 0, 0 );

		m_pAnim[i]->GetRenderParam()->Init();
		m_pAnim[i]->GetRenderParam()->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
		m_pAnim[i]->GetRenderParam()->worldMatrix		= m_pAnim[i]->GetMatrix()->GetMatrix();
		m_pAnim[i]->GetRenderParam()->lightPos			= D3DXVECTOR3(1000,1000,1000);
		m_pAnim[i]->GetRenderParam()->fOutLineWide		= 1.7f;

		m_pAnim[i]->ChangeAnim( L"Wait" );
		m_pAnim[i]->Play( CKTDGXSkinAnim::XAP_LOOP );

		g_pKTDXApp->GetDGManager()->AddObjectChain( m_pAnim[i] );
	}
	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1000, 0,0,0 );
}

CTestMain::~CTestMain(void)
{
	SAFE_DELETE( g_pParticleSystem );
	SAFE_DELETE( g_pMeshPlayer );
}


void CTestMain::Reset()
{
	SAFE_DELETE( g_pParticleSystem );
	SAFE_DELETE( g_pMeshPlayer );

	g_pParticleSystem	= new CKTDGParticleSystem( g_pKTDXApp->GetDevice() );
	g_pMeshPlayer		= new CKTDGXMeshPlayer();

	g_pParticleSystem->OpenScriptFile( L"particle.txt" );
	g_pMeshPlayer->OpenScriptFile( L"meshplayer.txt" );

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"systemLua.txt" );
	if( Info == NULL )
	{
		return;
	}

	//루아 파싱
	KLuaManager luaManager;
	luaManager.DoMemory( Info->pRealData, Info->size );
	//카메라 세팅
	luaManager.BeginTable( "CAMERA" );
	{
		D3DXVECTOR3 eye;
		D3DXVECTOR3 lookat;

		luaManager.GetValue( "EYE_X", eye.x );
		luaManager.GetValue( "EYE_Y", eye.y );
		luaManager.GetValue( "EYE_Z", eye.z );

		luaManager.GetValue( "LOOK_AT_X", lookat.x );
		luaManager.GetValue( "LOOK_AT_Y", lookat.y );
		luaManager.GetValue( "LOOK_AT_Z", lookat.z );

		g_pKTDXApp->GetDGManager()->GetCamera().Point( eye.x,eye.y,eye.z, lookat.x,lookat.y,lookat.z );
	}
	luaManager.EndTable();

	//파티클 생성
	string name;
	D3DXVECTOR3 pos;
	luaManager.BeginTable( "PARTICLE" );
	{
		int index = 1;
		while( luaManager.BeginTable(index) == true )
		{
			luaManager.GetValue( "NAME", name );
			luaManager.GetValue( "POSITION_X", pos.x );
			luaManager.GetValue( "POSITION_Y", pos.y );
			luaManager.GetValue( "POSITION_Z", pos.z );
			luaManager.EndTable();
			index++;

			wstring wname;
			ConvertCharToWCHAR( wname, name.c_str() );
			g_pParticleSystem->CreateSequence( NULL,  wname.c_str(), pos.x,pos.y,pos.z, -1.0f, -1.0f, 0 );
		}
	}
	luaManager.EndTable();

	luaManager.BeginTable( "MESH_PLAYER" );
	{
		int index = 1;
		while( luaManager.BeginTable(index) == true )
		{
			luaManager.GetValue( "NAME", name );
			luaManager.GetValue( "POSITION_X", pos.x );
			luaManager.GetValue( "POSITION_Y", pos.y );
			luaManager.GetValue( "POSITION_Z", pos.z );
			luaManager.EndTable();
			index++;

			wstring wname;
			ConvertCharToWCHAR( wname, name.c_str() );
			g_pMeshPlayer->CreateInstance( NULL,  wname.c_str(), pos.x,pos.y,pos.z, 0,0,0, 0,0,0, 0 );
		}
	}
	luaManager.EndTable();
}

HRESULT CTestMain::OnFrameMove( double fTime, float fElapsedTime )
{
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState(DIK_RIGHT) == TRUE )
	{
		g_pKTDXApp->GetDGManager()->GetCamera().MoveRel( 500.0f * fElapsedTime, 0.0f, 0.0f );
	}
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState(DIK_LEFT) == TRUE )
	{
		g_pKTDXApp->GetDGManager()->GetCamera().MoveRel( -500.0f * fElapsedTime, 0.0f, 0.0f );
	}
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState(DIK_UP) == TRUE )
	{
		g_pKTDXApp->GetDGManager()->GetCamera().MoveRel( 0.0f, 500.0f * fElapsedTime, 0.0f );
	}
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState(DIK_DOWN) == TRUE )
	{
		g_pKTDXApp->GetDGManager()->GetCamera().MoveRel( 0.0f, -500.0f * fElapsedTime, 0.0f );
	}

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F5) == TRUE )
	{
		Reset();
	}

	//g_pParticleSystem->OnFrameMove( fTime, fElapsedTime );
	//g_pMeshPlayer->OnFrameMove( fTime, fElapsedTime );

	for( int i = 0; i < 20; i++ )
	{
		m_pAnim[i]->OnFrameMove( fTime, fElapsedTime );
	}

	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );

	return S_OK;
}

HRESULT CTestMain::OnFrameRender()
{
	/*
	//pMatrix->UpdateWorldMatrix();


	CKTDGXRenderer::RenderParam* pParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
	pParam->renderType	= CKTDGXRenderer::RT_REAL_COLOR;
	pParam->worldMatrix		= pMatrix->GetMatrix();
	pParam->cartoonTexType = CKTDGXRenderer::CTT_NORMAL;
	pParam->lightDir	= D3DXVECTOR3(1000,1000,1000);
	pParam->fOutLineWide = 3;
	pParam->outLineColor = 0xffff0000;
	pParam->color		= 0xffffffff;
	pParam->fLightFlowWide = 300;
	pParam->fLightFlowImpact = fflow;
	pParam->bAlphaBlend	= true;
	pParam->bZEnable	= true;
	pParam->cullMode	= D3DCULL_NONE;
	pParam->srcBlend	= D3DBLEND_SRCALPHA;
	pParam->destBlend	= D3DBLEND_INVSRCALPHA;
	pParam->texOffsetStage0.x = 0;
	pParam->texOffsetStage0.y = 0;
	pParam->texOffsetStage1.x = 0;
	pParam->texOffsetStage1.y = 0;
	pParam->texOffsetStage2.x = 0;
	pParam->texOffsetStage2.y = 0;

	g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
	//g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( pXMesh );
	g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();

	pParam->renderType	= CKTDGXRenderer::RT_LIGHT_FLOW_WORLD_Y;
	pParam->color				= 0xeeeeeeee;
	pParam->fLightFlowWide		= 100;
	pParam->fLightFlowImpact	= fflow;

	g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
	//g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( pXMesh );
	g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();

	
	//pSkinAnim->GetMatrix()->Move( 0,0,0 );

	CKTDGXSkinRenderer::RenderParam* psParam = pSkinAnim->GetRenderParam();
	psParam->worldMatrix			= pSkinAnim->GetMatrix()->GetMatrix();
	psParam->renderType			= CKTDGXSkinRenderer::RT_CARTOON_BLACK_EDGE;
	psParam->cartoonTexType		= CKTDGXSkinRenderer::CTT_NORMAL;
	psParam->lightDir			= D3DXVECTOR3(1000,1000,1000);
	psParam->fOutLineWide		= 0.5;
	psParam->outLineColor		= 0xff0000ff;
	psParam->color				= 0xff333333;
	psParam->fLightFlowWide		= 100;
	psParam->fLightFlowImpact	= fflow;
	psParam->bAlphaBlend		= true;
	psParam->bZEnable			= true;
	psParam->cullMode			= D3DCULL_CCW;
	psParam->srcBlend			= D3DBLEND_SRCALPHA;
	psParam->destBlend			= D3DBLEND_INVSRCALPHA;
	psParam->texOffsetStage0.x = 0;
	psParam->texOffsetStage0.y = 0;
	psParam->texOffsetStage1.x = 0;
	psParam->texOffsetStage1.y = 0;
	psParam->texOffsetStage2.x = 0;
	psParam->texOffsetStage2.y = fflow;

	//pSkinAnim->OnFrameRender();

	psParam->worldMatrix		= pMatrix->GetMatrix();
	psParam->renderType			= CKTDGXSkinRenderer::RT_LIGHT_FLOW_LOCAL_X;
	psParam->cartoonTexType		= CKTDGXSkinRenderer::CTT_NORMAL;
	psParam->lightDir			= D3DXVECTOR3(1000,1000,1000);
	psParam->fOutLineWide		= 0.5;
	psParam->outLineColor		= 0xff0000ff;
	psParam->color				= 0xffffffff;
	psParam->fLightFlowWide		= 3;
	psParam->fLightFlowImpact	= fflow;
	psParam->bAlphaBlend			= true;
	psParam->bZEnable			= true;
	psParam->cullMode			= D3DCULL_CCW;
	psParam->srcBlend			= D3DBLEND_SRCALPHA;
	psParam->destBlend			= D3DBLEND_INVSRCALPHA;

	//pSkinAnim->OnFrameRender();
	*/

	g_pKTDXApp->GetDGManager()->ObjectChainSort();
	g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
	g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
	g_pKTDXApp->GetDGManager()->FrontUIRender();
	

	return S_OK;
}

HRESULT CTestMain::OnResetDevice()
{
	//g_pParticleSystem->OnResetDevice();

	return S_OK;
}

HRESULT CTestMain::OnLostDevice()
{
	//g_pParticleSystem->OnLostDevice();

	return S_OK;
}



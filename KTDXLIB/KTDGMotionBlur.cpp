#include "StdAfx.h"
#include ".\ktdgmotionblur.h"


//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akStates[] = 
{
		KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
		KRenderState( D3DRS_CULLMODE,			D3DCULL_CW ),
		KRenderState( D3DRS_ZENABLE,			FALSE ),
		//KRenderState( D3DRS_TEXTUREFACTOR,	m_BlurLevel ),
		KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
		//KTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE ),
		//KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE ),
};//s_akStates[]
//}} robobeg : 2008-10-13


CKTDGMotionBlur::CKTDGMotionBlur( LPDIRECT3DDEVICE9 pd3dDevice )
{
	//m_pd3dDevice		= pd3dDevice;
	m_bEnable			= false;
	m_BlurLevel			= 0x22ffffff;

	m_pMotionBlurTex	= NULL;

//{{ robobeg : 2008-10-13
	m_RenderStateID = s_akStates;
//}} robobeg : 2008-10-13
}

CKTDGMotionBlur::~CKTDGMotionBlur(void)
{
	SAFE_CLOSE( m_pMotionBlurTex );
}

void CKTDGMotionBlur::BeginRender()
{
	if( m_pMotionBlurTex == NULL )
		return;
    
	m_pMotionBlurTex->BeginRender( true );
}

void CKTDGMotionBlur::EndRender()
{
	if( m_pMotionBlurTex == NULL )
		return;

	m_pMotionBlurTex->EndRender();
	m_pMotionBlurTex->SetDeviceTexture();

    KD3DPUSH( m_RenderStateID )
    	m_pMotionBlurTex->RenderQuadRHW( m_BlurLevel );
    KD3DEND()
}

void CKTDGMotionBlur::OnResetDevice()
{
	if( m_pMotionBlurTex == NULL )
	{
		m_pMotionBlurTex = g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"MotionBlurTex", 1024, 1024 );
	}
}

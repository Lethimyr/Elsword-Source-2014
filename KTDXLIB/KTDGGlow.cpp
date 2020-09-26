#include "StdAfx.h"
#include ".\ktdgglow.h"


//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akStates[] = 
{
		KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
		KRenderState( D3DRS_ALPHATESTENABLE,	FALSE ),
		KRenderState( D3DRS_ZENABLE,			FALSE ),
		KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
		KRenderState( D3DRS_CULLMODE,			D3DCULL_CW ),
		KRenderState( D3DRS_DESTBLEND,			D3DBLEND_DESTALPHA ),
		KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_ADD ),
};//akStates[]
//}} robobeg : 2008-10-13


CKTDGGlow::CKTDGGlow( LPDIRECT3DDEVICE9 pd3dDevice )
{
	//m_pd3dDevice = pd3dDevice;
	m_bEnable = false;

	m_pGlowTex128	= NULL;
	m_pGlowTex32	= NULL;

//{{ robobeg : 2008-10-13
	m_RenderStateID = s_akStates;
//}} robobeg : 2008-10-13

}

CKTDGGlow::~CKTDGGlow(void)
{
	SAFE_CLOSE( m_pGlowTex128 );
	SAFE_CLOSE( m_pGlowTex32 );
}

void CKTDGGlow::OnResetDevice()
{
	if( m_pGlowTex128 == NULL )
	{
		m_pGlowTex128 = g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"GlowTex128", 128, 128 );
	}
	if( m_pGlowTex32 == NULL )
	{
		m_pGlowTex32 = g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"GlowTex64", 32, 32 );
	}
}

void CKTDGGlow::BeginRender()
{
	if( m_bEnable == false || m_pGlowTex128 == NULL || m_pGlowTex32 == NULL )
		return;
}

void CKTDGGlow::EndRender()
{
	if( m_bEnable == false || m_pGlowTex128 == NULL || m_pGlowTex32 == NULL )
		return;

    KD3DPUSH( m_RenderStateID )

	    m_pGlowTex128->CopyBackBufferToSurface();
	    m_pGlowTex128->SetDeviceTexture( 0 );
	    m_pGlowTex128->RenderQuadRHW( 0x22222222 );
	    m_pGlowTex128->CopySurface( m_pGlowTex32->GetSurface() );

	    m_pGlowTex32->CopyBackBufferToSurface();
	    m_pGlowTex32->SetDeviceTexture( 0 );
	    m_pGlowTex32->RenderQuadRHW( 0x33333333 );

    KD3DEND()
}

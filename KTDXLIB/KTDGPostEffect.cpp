#include "StdAfx.h"

#ifdef ADD_POSTEFFECT

#include ".\KTDGPostEffect.h"


//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akStatesPostEffect[] = 
{
		KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
		KRenderState( D3DRS_CULLMODE,			D3DCULL_CCW ),
		KRenderState( D3DRS_ZENABLE,			FALSE ),
		KRenderState( D3DRS_LIGHTING,			FALSE ),
		KRenderState( D3DRS_AMBIENT,			0 ),
		
		//KRenderState( D3DRS_TEXTUREFACTOR,	m_BlurLevel ),
		//KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
		//KTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE ),
		//KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE ),
		//KSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ),
		//KSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR ),
		//KSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR ),
};//s_akStates[]
//}} robobeg : 2008-10-13


CKTDGPostEffect::CKTDGPostEffect()
{
	m_bEnable			= false;
	m_bProcessing		= false;
	m_pPostEffectTex	= NULL;
		
//{{ robobeg : 2008-10-13
	m_RenderStateID = s_akStatesPostEffect;
//}} robobeg : 2008-10-13
	 
	m_pEffect = NULL;
	m_hTechnique[0] = NULL;
	m_hTechnique[1] = NULL;
	m_hTechnique[2] = NULL;
	m_hTechnique[3] = NULL;
	m_hTechnique[4] = NULL;

	m_fTexScale			= 2.f;

#ifdef ADD_POSTEFEFCT_FILTER01
	m_pFilterTex01 = NULL;
#endif
	OnResetDevice();
}

CKTDGPostEffect::~CKTDGPostEffect(void)
{
	SAFE_RELEASE( m_pEffect );
	//SAFE_DELETE( m_pPostEffectTex );
	SAFE_CLOSE( m_pPostEffectTex );
#ifdef ADD_POSTEFEFCT_FILTER01
	SAFE_CLOSE( m_pFilterTex01 );
#endif
}

void CKTDGPostEffect::BeginRender(bool bClear)
{
	if( m_pEffect == NULL || m_pPostEffectTex == NULL )
		return;

	m_pPostEffectTex->BeginRender( bClear );	
	
	m_bProcessing = true;
}

void CKTDGPostEffect::EndRender()
{	
	if( m_pEffect == NULL || m_pPostEffectTex == NULL )
		return;

	m_pPostEffectTex->EndRender();

	m_bProcessing = false;
}

void CKTDGPostEffect::RenderPost()
{
	if( m_pEffect != NULL && m_pPostEffectTex != NULL && m_bEnable == true )
	{
		CKTDXDeviceRenderTargetTexture* m_pRenderTex = NULL;
		
		m_pRenderTex = m_pPostEffectTex;

		if( m_hTechnique[0] != NULL && m_hTechnique[4] != NULL && m_pRenderTex != NULL )
		{
			int iTech = 0;
#ifdef ADD_POSTEFEFCT_FILTER01
			if( m_pFilterTex01 != NULL && g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
				iTech = 4;
#endif //ADD_POSTEFEFCT_FILTER01

			m_pEffect->SetTechnique( m_hTechnique[iTech] );
			m_pEffect->Begin( NULL, 0 );
			m_pEffect->BeginPass(0);

			m_pEffect->SetFloat( m_hTexSizeX, (float)m_pRenderTex->GetWidth() );
			m_pEffect->SetFloat( m_hTexSizeY, (float)m_pRenderTex->GetHeight() );
			m_pEffect->SetTexture( m_hTex, m_pRenderTex->GetTexture() );

#ifdef ADD_POSTEFEFCT_FILTER01
			if( m_pFilterTex01 != NULL )
			{
				m_pEffect->SetFloat( m_hFilterSizeX, (float)m_pFilterTex01->GetWidth() );
				m_pEffect->SetFloat( m_hFilterSizeX, (float)m_pFilterTex01->GetHeight() );
				m_pEffect->SetTexture( m_hFilterTex, m_pFilterTex01->GetDeviceTexture() );
			}
#endif //ADD_POSTEFEFCT_FILTER01

			m_pEffect->CommitChanges();

			KD3DPUSH( m_RenderStateID )
				m_pRenderTex->RenderQuadRHWPost( );
			KD3DEND()

			m_pEffect->End();
		}
	}
}


void CKTDGPostEffect::OnResetDevice()
{	
	D3DVIEWPORT9 Viewport;
	g_pKTDXApp->GetDevice()->GetViewport(&Viewport);
	
	if( m_pPostEffectTex == NULL )
	{		
		m_pPostEffectTex = g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"PostTex", (int)(Viewport.Width*m_fTexScale), (int)(Viewport.Height*m_fTexScale) );
	}
	else
	{
		m_pPostEffectTex->SetWidth( (int)(Viewport.Width*m_fTexScale) );
		m_pPostEffectTex->SetHeight( (int)(Viewport.Height*m_fTexScale) );
		m_pPostEffectTex->OnResetDevice();
	}
	
	if( m_pEffect == NULL )
	{
		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"XPostEffect.fx" );
		if( Info != NULL )
		{
			HRESULT hr = S_OK;
			LPD3DXBUFFER pErr = NULL;
			if(FAILED(hr = D3DXCreateEffect( g_pKTDXApp->GetDevice(), Info->pRealData, Info->size,
				NULL, NULL, 0, /*D3DXFX_NOT_CLONEABLE,*/ NULL, &m_pEffect, &pErr )))
			{
				ErrorLog( KEM_ERROR50 );
				SAFE_RELEASE( m_pEffect );				
			}
			else
			{				
				m_hTechnique[0] = m_pEffect->GetTechniqueByName( "PostEffectTest" );
				m_hTechnique[1] = m_pEffect->GetTechniqueByName( "PostEffectTestDepth" );
				m_hTechnique[2] = m_pEffect->GetTechniqueByName( "PostEffectTestFilter" );
				m_hTechnique[3] = m_pEffect->GetTechniqueByName( "PostEffectTestDOF" );
				m_hTechnique[4] = m_pEffect->GetTechniqueByName( "PostEffectTestFilter01" );
				
				m_hTexSizeX = m_pEffect->GetParameterByName( NULL, "TexSizeX" );
				m_hTexSizeY = m_pEffect->GetParameterByName( NULL, "TexSizeY" );
				m_hFilterSizeX = m_pEffect->GetParameterByName( NULL, "MAP_WIDTH" );
				m_hFilterSizeY = m_pEffect->GetParameterByName( NULL, "MAP_HEIGHT" );

				m_hTex = m_pEffect->GetParameterByName( NULL, "Tex0" );
				m_hDepthTex = m_pEffect->GetParameterByName( NULL, "DepthTex" );
				m_hFilterTex = m_pEffect->GetParameterByName( NULL, "FilterTex" );

				m_hAfWeight = m_pEffect->GetParameterByName( NULL, "weight" );				
				m_hOffsetX = m_pEffect->GetParameterByName( NULL, "offsetX" );	
				m_hOffsetY = m_pEffect->GetParameterByName( NULL, "offsetY" );				
			}
		}	
	}
	else
	{
		m_pEffect->OnResetDevice(); 
	}

#ifdef ADD_POSTEFEFCT_FILTER01
	if( m_pFilterTex01 == NULL )
		m_pFilterTex01 = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Filter01.tga" );
	else
		m_pFilterTex01->OnResetDevice();
#endif
}

void CKTDGPostEffect::OnLostDevice()
{
	if( m_pEffect != NULL )
		m_pEffect->OnLostDevice(); 

	D3DVIEWPORT9 Viewport;
	g_pKTDXApp->GetDevice()->GetViewport(&Viewport);
	
	if( m_pPostEffectTex != NULL )
	{		
		m_pPostEffectTex->SetWidth( (int)(Viewport.Width*m_fTexScale) );
		m_pPostEffectTex->SetHeight( (int)(Viewport.Height*m_fTexScale) );
		m_pPostEffectTex->OnLostDevice();
	}

#ifdef ADD_POSTEFEFCT_FILTER01
	if( m_pFilterTex01 != NULL )
		m_pFilterTex01->OnLostDevice();
#endif
}

#endif //ADD_POSTEFFECT
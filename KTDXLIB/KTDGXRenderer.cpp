#include "StdAfx.h"
#include ".\ktdgxrenderer.h"


#undef  EPHANDLE
#define EPHANDLE(x) (m_ahEffectParameters[ CKTDGXRenderer::EP_##x ])

extern CKTDGEffectStateManager* g_pEffectStateManager;

/** @function : GetRenderTypeTechniqueName
	@brief : RENDER_TYPE을 인자로 전달하면 XRenderer.fx 내에서 쓰고 있는 Technique 명을 얻어 올 수 있는 함수
	@param : RENDER_TYPE
	@return : const char* (Technique 명)
*/
/*static*/
const char*      CKTDGXRenderer::GetRenderTypeTechniqueName( CKTDGXRenderer::RENDER_TYPE eInType_ )
{
    static struct
    {
        RENDER_TYPE m_eType;
        const char* m_pszTechniqueName;
    }
    s_akTechniqueNames[] = 
    {
        { RT_REAL_COLOR,            "REAL_COLOR" },
        { RT_CARTOON_BLACK_EDGE,    "CARTOON_BLACK_EDGE" },
		{ RT_CARTOON_COLOR_EDGE,    "CARTOON_COLOR_EDGE" },
		{ RT_CARTOON,               "CARTOON" },
		{ RT_ADD_CARTOON_BLACK_EDGE,"ADD_CARTOON_BLACK_EDGE" },
		{ RT_ADD_CARTOON_COLOR_EDGE,"ADD_CARTOON_COLOR_EDGE" },
		{ RT_ADD_CARTOON,           "ADD_CARTOON" },
		{ RT_BLACK_EDGE,            "BLACK_EDGE" },
		{ RT_COLOR_EDGE,            "COLOR_EDGE" },
		{ RT_LIGHT_FLOW_WORLD_X,    "LIGHT_FLOW_WORLD_X" },
		{ RT_LIGHT_FLOW_WORLD_Y,    "LIGHT_FLOW_WORLD_Y" },
		{ RT_LIGHT_FLOW_WORLD_Z,    "LIGHT_FLOW_WORLD_Z" },
		{ RT_LIGHT_FLOW_WORLD_POINT,"LIGHT_FLOW_WORLD_POINT" },
		{ RT_LIGHT_FLOW_LOCAL_X,    "LIGHT_FLOW_LOCAL_X" },
		{ RT_LIGHT_FLOW_LOCAL_Y,    "LIGHT_FLOW_LOCAL_Y" },
		{ RT_LIGHT_FLOW_LOCAL_Z,    "LIGHT_FLOW_LOCAL_Z" },
		{ RT_LIGHT_FLOW_LOCAL_POINT,"LIGHT_FLOW_LOCAL_POINT" },				
		{ RT_CARTOON_FADE,			"CARTOON_FADE" },
		{ RT_CARTOON_FADE_NONTEX,	"CARTOON_FADE_NONTEX" },	
    };//s_akTechniqueNames[]

    BOOST_STATIC_ASSERT( ARRAY_SIZE( s_akTechniqueNames ) == RT_END );
    ASSERT( eInType_ >= RENDER_TYPE( 0 ) && eInType_ < RT_END );
    if ( !( eInType_ >= RENDER_TYPE( 0 ) && eInType_ < RT_END ) )
        return "";
    ASSERT( s_akTechniqueNames[ eInType_ ].m_eType == eInType_ );
    return  s_akTechniqueNames[ eInType_ ].m_pszTechniqueName;
}//CKTDGXRenderer::GetRenderTypeTechniqueName()


/** @function : GetEffectParameterName
	@brief : EFFECT_PARAMETER를 인자로 전달하여 XRenderer.fx 내에서 쓰고 있는 변수의 명을 얻어 올 수 있는 함수
	@param : EFFECT_PARAMETER
	@return : const char* ( XRenderer.fx 내의 전역 변수 명)
*/
/*static*/
const char*      CKTDGXRenderer::GetEffectParameterName( CKTDGXRenderer::EFFECT_PARAMETER eInParam_ )
{
    static struct
    {
        EFFECT_PARAMETER    m_eEffectParameter;
        const char* m_pszName;
    }
    s_akEffectParameterName[] = 
    {
        { EP_g_mViewProj,       "g_mViewProj" },
        { EP_g_mWorldViewProj,  "g_mWorldViewProj" },

        { EP_g_Color,           "g_Color" },
        { EP_g_TexOffSet01,     "g_TexOffSet01" },
        { EP_g_LightPos,        "g_LightPos" },
        { EP_g_OutLineColor,    "g_OutLineColor" },
        { EP_g_LightFlowPoint,  "g_LightFlowPoint" },

        { EP_g_fTexOffSet2X,    "g_fTexOffSet2X" },
        { EP_g_fTexOffSet2Y,    "g_fTexOffSet2Y" },
        { EP_g_fOutLineWide,    "g_fOutLineWide" },
        { EP_g_fLightFlowImpact,"g_fLightFlowImpact" },
        { EP_g_fLightFlowWide,  "g_fLightFlowWide" },
		

#ifdef FOG_WORLD

        { EP_g_bFog,            "g_bFog" },
        { EP_g_fFog,            "g_fFog" },
        { EP_g_fEye,            "g_fEye" },
        { EP_g_fColor,          "g_fColor" },
        { EP_g_fDensity,        "g_fDensity" },
#endif // #ifdef FOG_WORLD

        { EP_g_amWorldPalette, "g_amWorldPalette" },
        { EP_g_iShaderIndex, "g_iShaderIndex" },

    };//s_akEffectParameterName[]

    BOOST_STATIC_ASSERT( ARRAY_SIZE( s_akEffectParameterName ) == EP_END );
    ASSERT( eInParam_ >= EFFECT_PARAMETER( 0 ) && eInParam_ < EP_END );
    if ( !( eInParam_ >= EFFECT_PARAMETER( 0 ) && eInParam_ < EP_END ) )
        return "";
    ASSERT( s_akEffectParameterName[ eInParam_ ].m_eEffectParameter == eInParam_ );
    return  s_akEffectParameterName[ eInParam_ ].m_pszName;
}//CKTDGXRenderer::GetEffectParameterName()
//}} robobeg : 2008-10-08

CKTDGXRenderer::CKTDGXRenderer( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pEffect		= NULL;

	for( int i = 0; i < CTT_END; i++ )
	{
		m_pCartoonTexList[i] = NULL;
	}

    ZeroMemory( m_ahTechniques, sizeof( m_ahTechniques ) );
    ZeroMemory( m_ahEffectParameters, sizeof( m_ahEffectParameters ) );

    m_bInRenderGroup = false;

    m_uNumPasses = 0;
    m_uCurPass = 0;
    m_bInBeginPass = false;

    m_iStackTop = -1;

    m_bFog = false;
    m_uMaxSkinningVertices = 0;

#ifndef X2OPTIMIZE_SETSHADERCONSTANT
    m_bCommitChangesPending = false;
#endif//X2OPTIMIZE_SETSHADERCONSTANT
    m_eCurrentRenderType = RENDER_TYPE(0);
    m_iCurrentShaderIndex = 0;
    m_dxcCurrentColor = D3DXCOLOR( 0, 0, 0, 0 );

}

CKTDGXRenderer::~CKTDGXRenderer(void)
{
	SAFE_RELEASE( m_pEffect );

	for( int i = 0; i < CTT_END; i++ )
	{
		SAFE_CLOSE( m_pCartoonTexList[i] );
	}
}

/** @function : SetColor
	@brief : XRenderer.fx 내의 g_Color 변수를 셋팅
	@param : D3DXCOLOR&
	@return : void
*/
void CKTDGXRenderer::SetColor( const D3DXCOLOR& dxcInColor_ )
{
    if ( dxcInColor_ != m_dxcCurrentColor )
    {
        HRESULT hr = m_pEffect->SetVector( EPHANDLE( g_Color ), (const D3DXVECTOR4*) &dxcInColor_ );
        ASSERT( SUCCEEDED( hr ) );
        m_dxcCurrentColor = dxcInColor_;
#ifndef X2OPTIMIZE_SETSHADERCONSTANT
		m_bCommitChangesPending = true;
#endif//X2OPTIMIZE_SETSHADERCONSTANT
    }//if
}//CKTDGXRenderer::SetColor()

/** @function : SetRenderPassFogAndMaxSkinningVertices
	@brief : 포그 사용 여부, Bone의 갯수를 셋팅함 (다른 함수에서 포그 사용 여부, Bone의 갯수에 따라, m_iCurrentShaderIndex가 변경된다.)
	@param : bool bInFog_, unsigned uInSkinningVertices_
	@return : void
*/
void    CKTDGXRenderer::SetRenderPassFogAndMaxSkinningVertices( bool bInFog_, unsigned uInSkinningVertices_ )
{
    m_bFog = bInFog_;
    m_uMaxSkinningVertices = min( uInSkinningVertices_, 4 );
}//CKTDGXRenderer::SetRenderPassFogAndMaxSkinningVertices()


void    CKTDGXRenderer::_CommitChanges()
{
    m_uMaxSkinningVertices = min( m_uMaxSkinningVertices, 4 );

    int iMaxBones = 0;
    if ( m_eCurrentRenderType >= RT_LIGHT_FLOW_BEGIN && m_eCurrentRenderType <= RT_LIGHT_FLOW_END )
        iMaxBones = (int) m_uMaxSkinningVertices;
    else
        iMaxBones = (int) m_uMaxSkinningVertices + ( ( m_bFog ) ? 5 : 0 );

    if ( iMaxBones != m_iCurrentShaderIndex )
    {
        m_iCurrentShaderIndex = iMaxBones;
        m_pEffect->SetInt( EPHANDLE( g_iShaderIndex ), m_iCurrentShaderIndex );
#ifndef X2OPTIMIZE_SETSHADERCONSTANT
		m_bCommitChangesPending = true;
#endif//X2OPTIMIZE_SETSHADERCONSTANT
    }//if

#ifdef X2OPTIMIZE_SETSHADERCONSTANT
	if( m_bInBeginPass )
	{
		m_pEffect->CommitChanges();
		g_pEffectStateManager->PostCommitChanges();
	}
#else//X2OPTIMIZE_SETSHADERCONSTANT
	if ( m_bCommitChangesPending && m_bInBeginPass )
	{
	    m_pEffect->CommitChanges();
	    m_bCommitChangesPending = false;
	}//if
#endif//X2OPTIMIZE_SETSHADERCONSTANT
}//CKTDGXRenderer::_CommitChanges()



HRESULT CKTDGXRenderer::BeginRenderGroup( CKTDGXRenderer::RENDER_TYPE eInRenderType_ )
{
    ASSERT( !m_bInRenderGroup );
    if ( m_bInRenderGroup )
        return E_FAIL;

    if ( !( eInRenderType_ >= CKTDGXRenderer::RENDER_TYPE( 0 ) && eInRenderType_ < RT_END ) )
        return E_FAIL;

    ASSERT( m_ahTechniques[ eInRenderType_ ] != NULL );
    if ( m_ahTechniques[ eInRenderType_ ] == NULL )
        return E_FAIL;

    HRESULT hr;
    if ( eInRenderType_ != m_eCurrentRenderType )
    {
        hr = m_pEffect->SetTechnique( m_ahTechniques[ eInRenderType_ ] ); 
        ASSERT( SUCCEEDED( hr ) );
	    if( FAILED( hr ) )
		    return hr;
        m_eCurrentRenderType = eInRenderType_;
    }//if

    ASSERT( g_pEffectStateManager != NULL );
    hr = g_pEffectStateManager->PrepareEffectState();
    ASSERT( SUCCEEDED( hr ) );
    if ( FAILED( hr ) )
        return hr;

    unsigned    uNumPasses = 0;

    hr = m_pEffect->Begin( &uNumPasses, D3DXFX_DONOTSAVESTATE | D3DXFX_DONOTSAVESAMPLERSTATE | D3DXFX_DONOTSAVESHADERSTATE );
    ASSERT( SUCCEEDED( hr ) );
    if ( FAILED( hr ) )
    {
        g_pEffectStateManager->RestoreEffectState();
        return hr;
    }//if
    ASSERT( uNumPasses > 0 );
    if ( uNumPasses == 0 )
    {
        m_pEffect->End();
        g_pEffectStateManager->RestoreEffectState();
		
        return E_FAIL;
    }//if

    m_bInRenderGroup = true;
    m_uNumPasses = uNumPasses;
    m_uCurPass = 0;
    m_bInBeginPass = false;

    return S_OK;
}//CKTDGXRenderer::BeginRenderGroup()


HRESULT CKTDGXRenderer::NextRenderGroup()
{
    ASSERT( m_bInRenderGroup );
    if ( !m_bInRenderGroup )
        return E_FAIL;

    ASSERT( m_eCurrentRenderType >= CKTDGXRenderer::RENDER_TYPE( 0 ) && m_eCurrentRenderType < RT_END 
        && m_uNumPasses > 0 );

    if ( m_bInBeginPass )
    {
        m_pEffect->EndPass();
        m_bInBeginPass = false;
    }//if

    if ( ++m_uCurPass >= m_uNumPasses )
    {
        m_pEffect->End();
        g_pEffectStateManager->RestoreEffectState();
        m_bInRenderGroup = false;
        m_uNumPasses = 0;
        m_uCurPass = 0;

		return E_FAIL;
    }//if

    return S_OK;
}//CKTDGXRenderer::NextRenderGroup()



HRESULT CKTDGXRenderer::BeginRender(  const CKTDGXRenderer::RenderParam& InRenderParam_, const D3DXMATRIX* pdxInWorldMatrix_  )
{
	KTDXPROFILE();

	CKTDGXRenderer::RENDER_TYPE eRenderType = InRenderParam_.renderType;

    ASSERT( eRenderType >= CKTDGXRenderer::RENDER_TYPE( 0 ) && eRenderType < RT_END );
    if ( !( eRenderType >= CKTDGXRenderer::RENDER_TYPE( 0 ) && eRenderType < RT_END ) )
        return E_FAIL;

    ASSERT( m_ahTechniques[ eRenderType ] != NULL );
    if ( m_ahTechniques[ eRenderType ] == NULL )
        return E_FAIL;

    if ( !m_bInRenderGroup )
    {
        if ( m_eCurrentRenderType != eRenderType )
        {
            HRESULT hr = m_pEffect->SetTechnique( m_ahTechniques[ eRenderType ] );
            ASSERT( SUCCEEDED( hr ) );
            if ( FAILED( hr ) )
	        {
    		    //{{ dmlee 2008.03.30 - note!! 불필요한 로그 같아서 comment out
		        //ErrorLogMsg( KEM_ERROR48, m_Technique.c_str() );
		        //}} dmlee 2008.03.30 - note!! 불필요한 로그 같아서 comment out
	    	    return hr;
    	    }
            m_eCurrentRenderType = eRenderType;
        }//if
    }
    else
    {
#ifndef ARA_CHARACTER_BASE
		ASSERT( m_eCurrentRenderType == eRenderType );
		if ( m_eCurrentRenderType != eRenderType )
			return E_FAIL;
#endif
    }//if.. else..

#ifndef X2OPTIMIZE_SETSHADERCONSTANT
	m_bCommitChangesPending = true;
#endif//X2OPTIMIZE_SETSHADERCONSTANT


#ifdef FOG_WORLD


    if( InRenderParam_.bFogShow == true && InRenderParam_.bFog == true )
    {
        if ( !m_bFog )
            m_bFog = true;

        // Fog
        D3DXMATRIX m;
        D3DXVECTOR4 v;
        float m_xNear					= InRenderParam_.fogNearX; //4000.0f;
        float m_xFar					= InRenderParam_.fogFarX; //20000.0f;
        float m_yNear                   = InRenderParam_.fogNearY; //-5000.0f;
        float m_yFar                    = InRenderParam_.fogFarY; //500.0f;
        float m_fogDensity              = InRenderParam_.fogDensity;
        D3DXCOLOR m_fogColor            = InRenderParam_.fogColor;                        
        D3DXVECTOR3 m_Eye = g_pKTDXApp->GetDGManager()->GetCamera().GetEye();

        m_pEffect->SetBool( EPHANDLE( g_bFog ), true );

        // 선형포그
		if(m_xFar-m_xNear != 0.0f)
		{
			v.x = m_xFar/(m_xFar-m_xNear);
			v.y = -1.0f/(m_xFar-m_xNear);    
		}
		else
		{
			v.x = 0.0f;
			v.y = 0.0f;
		}

		// 높이포그
		if(m_yFar-m_yNear != 0.0f)
		{
			v.z = m_yFar/(m_yFar-m_yNear);
			v.w = -1.0f/(m_yFar-m_yNear);

			// note!! CKTDGXRenderer::BeginRender에서 pdxInWorldMatrix_가 null이면 skin, null 아니면 그냥 mesh, 요고 보고 v.w 부호를 반대로 해버리면 안개 OK
			if(pdxInWorldMatrix_ == NULL)
				v.w *= -1.f;
		}
		else
		{
			v.z = 0.0f;
			v.w = 0.0f;
		}

        m_pEffect->SetVector( EPHANDLE( g_fFog ), &v );   

        // 시점
        //D3DXMatrixInverse( &m, NULL, &InRenderParam_.worldMatrix);                
        //D3DXVec3Transform( &v, &m_Eye, &m );
        v = D3DXVECTOR4(m_Eye.x, m_Eye.y, m_Eye.z, 1.0f);
        m_pEffect->SetVector( EPHANDLE( g_fEye ), &v );
		

        m_pEffect->SetVector( EPHANDLE( g_fColor ), &(D3DXVECTOR4)m_fogColor );   
        m_pEffect->SetFloat( EPHANDLE( g_fDensity ), m_fogDensity );
    }
    else
    {
        m_pEffect->SetBool( EPHANDLE( g_bFog ), false );
    }//if.. else..

#endif

	switch( eRenderType )
	{
		case RT_REAL_COLOR:
		case RT_CARTOON_FADE_NONTEX:
		case RT_CARTOON_FADE:		
			{
                SetColor( InRenderParam_.color );

				D3DXVECTOR4 texOffset01;
				texOffset01.x = InRenderParam_.texOffsetStage0.x;
				texOffset01.y = InRenderParam_.texOffsetStage0.y;
				texOffset01.z = InRenderParam_.texOffsetStage1.x;
				texOffset01.w = InRenderParam_.texOffsetStage1.y;
				float texOffset2X = InRenderParam_.texOffsetStage2.x;
				float texOffset2Y = InRenderParam_.texOffsetStage2.y;
				m_pEffect->SetVector( EPHANDLE( g_TexOffSet01 ),	&texOffset01 );
				m_pEffect->SetFloat( EPHANDLE( g_fTexOffSet2X ),	texOffset2X );
				m_pEffect->SetFloat( EPHANDLE( g_fTexOffSet2Y ),	texOffset2Y );
			}
			break;

		case RT_CARTOON_BLACK_EDGE:
		case RT_ADD_CARTOON_BLACK_EDGE:
			{
				D3DXVECTOR4 lightPos;
				lightPos.x = InRenderParam_.lightPos.x;
				lightPos.y = InRenderParam_.lightPos.y;
				lightPos.z = InRenderParam_.lightPos.z;
				lightPos.w = 1.0f;

				D3DXVec4Normalize(&lightPos,&lightPos);

                SetColor( InRenderParam_.color );
				m_pEffect->SetVector( EPHANDLE( g_LightPos ),		&lightPos );
				m_pEffect->SetFloat( EPHANDLE( g_fOutLineWide ),	InRenderParam_.fOutLineWide );

				m_pCartoonTexList[InRenderParam_.cartoonTexType]->SetDeviceTexture( 1 );

				D3DXVECTOR4 texOffset01;
				texOffset01.x = InRenderParam_.texOffsetStage0.x;
				texOffset01.y = InRenderParam_.texOffsetStage0.y;
				texOffset01.z = InRenderParam_.texOffsetStage1.x;
				texOffset01.w = InRenderParam_.texOffsetStage1.y;
				float texOffset2X = InRenderParam_.texOffsetStage2.x;
				float texOffset2Y = InRenderParam_.texOffsetStage2.y;
				m_pEffect->SetVector( EPHANDLE( g_TexOffSet01 ),	&texOffset01 );
				m_pEffect->SetFloat( EPHANDLE( g_fTexOffSet2X ),	texOffset2X );
				m_pEffect->SetFloat( EPHANDLE( g_fTexOffSet2Y ),	texOffset2Y );
			}
			break;

		case RT_CARTOON_COLOR_EDGE:
		case RT_ADD_CARTOON_COLOR_EDGE:
			{
				D3DXVECTOR4 lightPos;
				lightPos.x = InRenderParam_.lightPos.x;
				lightPos.y = InRenderParam_.lightPos.y;
				lightPos.z = InRenderParam_.lightPos.z;
				lightPos.w = 1.0f;

				D3DXVec4Normalize(&lightPos,&lightPos);

                SetColor( InRenderParam_.color );
				m_pEffect->SetVector( EPHANDLE( g_LightPos ),		&lightPos );
				m_pEffect->SetVector( EPHANDLE( g_OutLineColor ),	&(D3DXVECTOR4)InRenderParam_.outLineColor );
				m_pEffect->SetFloat( EPHANDLE( g_fOutLineWide ),	InRenderParam_.fOutLineWide );

				m_pCartoonTexList[InRenderParam_.cartoonTexType]->SetDeviceTexture( 1 );

				D3DXVECTOR4 texOffset01;
				texOffset01.x = InRenderParam_.texOffsetStage0.x;
				texOffset01.y = InRenderParam_.texOffsetStage0.y;
				texOffset01.z = InRenderParam_.texOffsetStage1.x;
				texOffset01.w = InRenderParam_.texOffsetStage1.y;
				float texOffset2X = InRenderParam_.texOffsetStage2.x;
				float texOffset2Y = InRenderParam_.texOffsetStage2.y;
				m_pEffect->SetVector( EPHANDLE( g_TexOffSet01 ),	&texOffset01 );
				m_pEffect->SetFloat( EPHANDLE( g_fTexOffSet2X ),	texOffset2X );
				m_pEffect->SetFloat( EPHANDLE( g_fTexOffSet2Y ),	texOffset2Y );
			}
			break;
		
		case RT_CARTOON:
		case RT_ADD_CARTOON:		
			{
				D3DXVECTOR4 lightPos;
				lightPos.x = InRenderParam_.lightPos.x;
				lightPos.y = InRenderParam_.lightPos.y;
				lightPos.z = InRenderParam_.lightPos.z;
				lightPos.w = 1.0f;

				D3DXVec4Normalize(&lightPos,&lightPos);

                SetColor( InRenderParam_.color );
				m_pEffect->SetVector( EPHANDLE( g_LightPos ),		&lightPos );

				m_pCartoonTexList[InRenderParam_.cartoonTexType]->SetDeviceTexture( 1 );
				
				D3DXVECTOR4 texOffset01;
				texOffset01.x = InRenderParam_.texOffsetStage0.x;
				texOffset01.y = InRenderParam_.texOffsetStage0.y;
				texOffset01.z = InRenderParam_.texOffsetStage1.x;
				texOffset01.w = InRenderParam_.texOffsetStage1.y;
				float texOffset2X = InRenderParam_.texOffsetStage2.x;
				float texOffset2Y = InRenderParam_.texOffsetStage2.y;
				m_pEffect->SetVector( EPHANDLE( g_TexOffSet01 ),	&texOffset01 );
				m_pEffect->SetFloat( EPHANDLE( g_fTexOffSet2X ),	texOffset2X );
				m_pEffect->SetFloat( EPHANDLE( g_fTexOffSet2Y ),	texOffset2Y );
			}
			break;

		case RT_BLACK_EDGE:
			{
				m_pEffect->SetFloat( EPHANDLE( g_fOutLineWide ),	InRenderParam_.fOutLineWide );
			}
			break;

		case RT_COLOR_EDGE:
			{
				m_pEffect->SetVector( EPHANDLE( g_OutLineColor ),	&(D3DXVECTOR4)InRenderParam_.outLineColor );
				m_pEffect->SetFloat( EPHANDLE( g_fOutLineWide ),	InRenderParam_.fOutLineWide );
			}
			break;

		case RT_LIGHT_FLOW_WORLD_X:

            SetColor( InRenderParam_.color );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowImpact ),	InRenderParam_.fLightFlowImpact );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowWide ),	InRenderParam_.fLightFlowWide );
			break;

		case RT_LIGHT_FLOW_WORLD_Y:

            SetColor( InRenderParam_.color );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowImpact ),	InRenderParam_.fLightFlowImpact );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowWide ),	InRenderParam_.fLightFlowWide );
			break;

		case RT_LIGHT_FLOW_WORLD_Z:

            SetColor( InRenderParam_.color );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowImpact ),	InRenderParam_.fLightFlowImpact );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowWide ),	InRenderParam_.fLightFlowWide );
			break;

		case RT_LIGHT_FLOW_WORLD_POINT:
			{

				D3DXVECTOR4 lightFlowPoint;
				lightFlowPoint.x = InRenderParam_.lightFlowPoint.x;
				lightFlowPoint.y = InRenderParam_.lightFlowPoint.y;
				lightFlowPoint.z = InRenderParam_.lightFlowPoint.z;
				lightFlowPoint.w = 1.0f;

                SetColor( InRenderParam_.color );
				m_pEffect->SetVector( EPHANDLE( g_LightFlowPoint ),	&lightFlowPoint );
				m_pEffect->SetFloat( EPHANDLE( g_fLightFlowWide ),	InRenderParam_.fLightFlowWide );
			}
			break;

		case RT_LIGHT_FLOW_LOCAL_X:

            SetColor( InRenderParam_.color );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowImpact ),	InRenderParam_.fLightFlowImpact );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowWide ),	InRenderParam_.fLightFlowWide );
			break;

		case RT_LIGHT_FLOW_LOCAL_Y:

            SetColor( InRenderParam_.color );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowImpact ),	InRenderParam_.fLightFlowImpact );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowWide ),	InRenderParam_.fLightFlowWide );
			break;

		case RT_LIGHT_FLOW_LOCAL_Z:

            SetColor( InRenderParam_.color );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowImpact ),	InRenderParam_.fLightFlowImpact );
			m_pEffect->SetFloat( EPHANDLE( g_fLightFlowWide ),	InRenderParam_.fLightFlowWide );
			break;

		case RT_LIGHT_FLOW_LOCAL_POINT:
			{
				D3DXVECTOR4 lightFlowPoint;
				lightFlowPoint.x = InRenderParam_.lightFlowPoint.x;
				lightFlowPoint.y = InRenderParam_.lightFlowPoint.y;
				lightFlowPoint.z = InRenderParam_.lightFlowPoint.z;
				lightFlowPoint.w = 1.0f;

                SetColor( InRenderParam_.color );
				m_pEffect->SetVector( EPHANDLE( g_LightFlowPoint ),	&lightFlowPoint );
				m_pEffect->SetFloat( EPHANDLE( g_fLightFlowWide ),	InRenderParam_.fLightFlowWide );
			}
			break;
	}

    // setup matrices
    {
	    D3DXMATRIX mx;
	    mx = g_pKTDXApp->GetViewProjectionTransform();
	    m_pEffect->SetMatrix( EPHANDLE( g_mViewProj ),			&mx );
        if ( pdxInWorldMatrix_ != NULL )
        {
            D3DXMATRIX  mFinal;
	        D3DXMatrixMultiply( &mFinal, pdxInWorldMatrix_, & mx );
	        m_pEffect->SetMatrixArray( EPHANDLE( g_amWorldPalette ), pdxInWorldMatrix_, 1 );
	        m_pEffect->SetMatrix( EPHANDLE( g_mWorldViewProj ),	&mFinal );
        }//if
    }

    if ( !m_bInRenderGroup )
    {
        int iStackTop = CKTDGStateManager::GetStackTop();
        ASSERT( iStackTop >= 0 );

#ifdef ADD_ALPHATESTENABLE
		CKTDGStateManager::PushRenderState( D3DRS_ALPHATESTENABLE,		InRenderParam_.bAlphaTestEnable ? TRUE : FALSE );
		CKTDGStateManager::PushRenderState( D3DRS_ALPHAREF,				0x00000088 );
		CKTDGStateManager::PushRenderState( D3DRS_ALPHAFUNC,			D3DCMP_GREATER );
		
#endif
		CKTDGStateManager::PushRenderState( D3DRS_ALPHABLENDENABLE,	InRenderParam_.bAlphaBlend ? TRUE : FALSE);
	    CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,			InRenderParam_.bZEnable ? TRUE : FALSE );
	    CKTDGStateManager::PushRenderState( D3DRS_ZWRITEENABLE,		InRenderParam_.bZWriteEnable ? TRUE : FALSE );
	    CKTDGStateManager::PushRenderState( D3DRS_CULLMODE,			InRenderParam_.cullMode );
	    CKTDGStateManager::PushRenderState( D3DRS_SRCBLEND,			InRenderParam_.srcBlend );
	    CKTDGStateManager::PushRenderState( D3DRS_DESTBLEND,		InRenderParam_.destBlend );



        ASSERT( g_pEffectStateManager != NULL );
        HRESULT hr = g_pEffectStateManager->PrepareEffectState();
        ASSERT( SUCCEEDED( hr ) );
        if ( FAILED( hr ) )
        {
            CKTDGStateManager::PopStates( iStackTop );
            return hr;
        }//if

        unsigned    uNumPasses = 0;
	    hr = m_pEffect->Begin( &uNumPasses, D3DXFX_DONOTSAVESTATE | D3DXFX_DONOTSAVESAMPLERSTATE | D3DXFX_DONOTSAVESHADERSTATE );
        ASSERT( SUCCEEDED( hr ) );
        if ( SUCCEEDED( hr ) )
        {
            ASSERT( uNumPasses > 0 );
            m_bInBeginPass = false;
            _CommitChanges();
            hr = m_pEffect->BeginPass( 0 );
            ASSERT( SUCCEEDED( hr ) );
            if ( SUCCEEDED( hr ) )
            {
#ifndef X2OPTIMIZE_SETSHADERCONSTANT
				m_bCommitChangesPending = false;
#endif//X2OPTIMIZE_SETSHADERCONSTANT
                m_bInBeginPass = true;
                m_uNumPasses = uNumPasses;
                m_uCurPass = 0;
                m_iStackTop = iStackTop;
                return S_OK;
            }
            m_pEffect->End();
        }//if

        ASSERT( g_pEffectStateManager != NULL );
        hr = g_pEffectStateManager->RestoreEffectState();
        ASSERT( SUCCEEDED( hr ) );

        CKTDGStateManager::PopStates( iStackTop );

        return E_FAIL;
    }
    else
    {
        if ( !m_bInBeginPass )
        {
            _CommitChanges();
            HRESULT hr = m_pEffect->BeginPass( m_uCurPass );
            ASSERT( SUCCEEDED( hr ) );
            if ( FAILED( hr ) )
            {
                m_iStackTop = -1;
                return hr;
            }//if

#ifndef X2OPTIMIZE_SETSHADERCONSTANT
			m_bCommitChangesPending = false;
#endif//X2OPTIMIZE_SETSHADERCONSTANT
            m_bInBeginPass = true;
        }//if
        m_iStackTop = CKTDGStateManager::GetStackTop();
        ASSERT( m_iStackTop >= 0 );

        ASSERT( g_pEffectStateManager != NULL );
#ifdef ADD_ALPHATESTENABLE
		if ( !g_pEffectStateManager->IsRenderStateChanged( D3DRS_ALPHATESTENABLE ) )
			CKTDGStateManager::PushRenderState( D3DRS_ALPHATESTENABLE,		InRenderParam_.bAlphaTestEnable ? TRUE : FALSE );
		if ( !g_pEffectStateManager->IsRenderStateChanged( D3DRS_ALPHAREF ) )
			CKTDGStateManager::PushRenderState( D3DRS_ALPHAREF,				0x00000088 );
		if ( !g_pEffectStateManager->IsRenderStateChanged( D3DRS_ALPHAFUNC ) )
			CKTDGStateManager::PushRenderState( D3DRS_ALPHAFUNC,			D3DCMP_GREATER );
#endif
		
        if ( !g_pEffectStateManager->IsRenderStateChanged( D3DRS_ALPHABLENDENABLE ) )
            CKTDGStateManager::PushRenderState( D3DRS_ALPHABLENDENABLE, InRenderParam_.bAlphaBlend ? TRUE : FALSE );
        if ( !g_pEffectStateManager->IsRenderStateChanged( D3DRS_ZENABLE ) )
	        CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,			InRenderParam_.bZEnable ? TRUE : FALSE );
        if ( !g_pEffectStateManager->IsRenderStateChanged( D3DRS_ZWRITEENABLE ) )
	        CKTDGStateManager::PushRenderState( D3DRS_ZWRITEENABLE,		InRenderParam_.bZWriteEnable ? TRUE : FALSE  );    
        if ( !g_pEffectStateManager->IsRenderStateChanged( D3DRS_CULLMODE ) )
	        CKTDGStateManager::PushRenderState( D3DRS_CULLMODE,			InRenderParam_.cullMode );
        if ( !g_pEffectStateManager->IsRenderStateChanged( D3DRS_SRCBLEND ) )
	        CKTDGStateManager::PushRenderState( D3DRS_SRCBLEND,			InRenderParam_.srcBlend );
        if ( !g_pEffectStateManager->IsRenderStateChanged( D3DRS_DESTBLEND ) )
	        CKTDGStateManager::PushRenderState( D3DRS_DESTBLEND,		InRenderParam_.destBlend );

        return S_OK;
    }//if.. else..
}

HRESULT CKTDGXRenderer::NextRender()
{
    if ( !m_bInRenderGroup )
    {
        if ( m_uNumPasses == 0 )
            return E_FAIL;

        HRESULT hr;

        if ( m_uCurPass < m_uNumPasses )
        {
            hr = m_pEffect->EndPass();
            ASSERT( SUCCEEDED( hr ) );
            m_uCurPass++;
            m_bInBeginPass = false;
            if ( m_uCurPass < m_uNumPasses )
            {
                _CommitChanges();
                hr = m_pEffect->BeginPass( m_uCurPass );
                ASSERT( SUCCEEDED( hr ) );
                if ( SUCCEEDED( hr ) )
                {
#ifndef X2OPTIMIZE_SETSHADERCONSTANT
					m_bCommitChangesPending = false;
#endif//X2OPTIMIZE_SETSHADERCONSTANT
                    m_bInBeginPass = true;
                    return hr;
                }//if
            }//if
        }//if

        hr = m_pEffect->End();
        ASSERT( SUCCEEDED( hr ) );

        ASSERT( g_pEffectStateManager != NULL );
        hr = g_pEffectStateManager->RestoreEffectState();
        ASSERT( SUCCEEDED( hr ) );

        if ( m_iStackTop >= 0 )
            CKTDGStateManager::PopStates( m_iStackTop );

        m_bInBeginPass = false;
        m_uNumPasses = 0;
        m_uCurPass = 0;
        m_iStackTop = -1;
		
	    return E_FAIL;
    }
    else
    {
        if ( m_iStackTop >= 0 )
        {
            CKTDGStateManager::PopStates( m_iStackTop );
            m_iStackTop = -1;
        }//if

        return E_FAIL;
    }//if.. else..
}//CKTDGXRenderer::NextRender()

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
HRESULT CKTDGXRenderer::OnFrameRender( const RenderParam& InRenderParam_, const D3DXMATRIX& mInWorld_, CKTDXDeviceXMesh& pXMesh, 
	CKTDXDeviceXET* pTexChangeXET, CKTDXDeviceXET* pMultiTexXET, 
	const CKTDXDeviceXET::AniData* pAniData, float fAniTime
	, unsigned uInDrawCount
	, std::vector<bool>* pvecDrawSubset )
#else//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
HRESULT CKTDGXRenderer::OnFrameRender( const RenderParam& InRenderParam_, const D3DXMATRIX& mInWorld_, CKTDXDeviceXMesh& pXMesh, 
	CKTDXDeviceXET* pTexChangeXET, CKTDXDeviceXET* pMultiTexXET, 
	const CKTDXDeviceXET::AniData* pAniData, float fAniTime
	, unsigned uInDrawCount )
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
{
	KTDXPROFILE();

	if( uInDrawCount == 0 )
		return E_FAIL;

    m_uMaxSkinningVertices = 0;

    HRESULT hr;

    for( hr = BeginRender( InRenderParam_, &mInWorld_ ); SUCCEEDED( hr ); hr = NextRender() )
    {
        _CommitChanges();
        for( unsigned uCount = 0; uCount < uInDrawCount; uCount++ )
#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
			pXMesh.Render( pTexChangeXET, pMultiTexXET, pAniData, fAniTime, pvecDrawSubset );
#else//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
			pXMesh.Render( pTexChangeXET, pMultiTexXET, pAniData, fAniTime );
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
    }//for

	return S_OK;
}



void CKTDGXRenderer::DrawFrame( CKTDXDeviceXSkinMesh& kInXSkinMesh_, CKTDXDeviceXSkinMesh::MultiAnimFrame& kInFrame_
            , CKTDXDeviceXET* pTexChangeXET, CKTDXDeviceXET* pMultiTexXET
            , const CKTDXDeviceXET::AniData* pAniData, float fAniTime, int iInDetailPercent_, bool abInUseTex_[3]
            , CKTDXDeviceXET* pAnimAniExt, const wchar_t* pwszInNowAnimName_ )
{
    ASSERT( m_bInBeginPass );

	KTDXPROFILE();

	HRESULT hr = S_OK;
	CKTDXDeviceXSkinMesh::MultiAnimMC * pMC = (CKTDXDeviceXSkinMesh::MultiAnimMC *) kInFrame_.pMeshContainer;
    ASSERT( pMC != NULL );
	D3DXMATRIX mx;

    if ( pMC->m_dwMaxSkinningVertices > 0 )
	{

		DWORD dwAttrib, dwPalEntry;

		CKTDXDeviceBaseTexture* orgTex = NULL;
		CKTDXDeviceBaseTexture* pResultTexture0 = NULL;
		CKTDXDeviceBaseTexture* pResultTexture1 = NULL;
		CKTDXDeviceBaseTexture* pResultTexture2 = NULL;

		// for each palette
		for( dwAttrib = 0; dwAttrib < pMC->m_dwNumAttrGroups; ++ dwAttrib )
		{
			KTDXPROFILE_BEGIN( "DrawFrame_SET_MATRIX" );
			// set each transform into the palette
			for( dwPalEntry = 0; dwPalEntry < pMC->m_dwNumPaletteEntries; ++ dwPalEntry )
			{
				DWORD dwMatrixIndex = pMC->GetBoneId( dwAttrib, dwPalEntry );
				if( dwMatrixIndex != UINT_MAX )
				{
					D3DXMatrixMultiply( &kInXSkinMesh_.GetWorkingPalette()[ dwPalEntry ],
						&( pMC->m_amxBoneOffsets[ dwMatrixIndex ] ),
                        &pMC->m_ppBoneFrames[ dwMatrixIndex ]->combineMatrix );
				}//if
			}

			// set the matrix palette into the effect
			hr = m_pEffect->SetMatrixArray( EPHANDLE( g_amWorldPalette ),
											kInXSkinMesh_.GetWorkingPalette(),
											pMC->m_dwNumPaletteEntries );
			KTDXPROFILE_END();


            KD3DBEGIN()

			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here
			KTDXPROFILE_BEGIN( "DrawFrame_SET_TEXTURE" );

            orgTex = (CKTDXDeviceBaseTexture*) pMC->m_apTextures[ pMC->GetAttributeRange( dwAttrib ).AttribId ];
			pResultTexture0 = _SetNowTexture( orgTex, 0, 
										pTexChangeXET, pMultiTexXET, 
										pAniData, fAniTime, abInUseTex_, pAnimAniExt, pwszInNowAnimName_ );
			pResultTexture1 = _SetNowTexture( orgTex, 1, 
										pTexChangeXET, pMultiTexXET, 
										pAniData, fAniTime, abInUseTex_, pAnimAniExt, pwszInNowAnimName_ );
			pResultTexture2 = _SetNowTexture( orgTex, 2, 
										pTexChangeXET, pMultiTexXET, 
										pAniData, fAniTime, abInUseTex_, pAnimAniExt, pwszInNowAnimName_ );

			if( pResultTexture0 != NULL )
				pResultTexture0->SetDeviceTexture( 0 );
			if( pResultTexture1 != NULL )
			{
				CKTDGStateManager::PushTextureStageState( 1, D3DTSS_COLOROP, pMultiTexXET->GetMultiTex( 1 )->colorOP );
				pResultTexture1->SetDeviceTexture( 1 );
			}
			if( pResultTexture2 != NULL )
			{
				CKTDGStateManager::PushTextureStageState( 2, D3DTSS_COLOROP, pMultiTexXET->GetMultiTex( 2 )->colorOP );
				pResultTexture2->SetDeviceTexture( 2 );
			}

            m_uMaxSkinningVertices = max( max( m_uMaxSkinningVertices, pMC->m_dwMaxSkinningVertices ), 1 );

			KTDXPROFILE_END();            
			
#ifdef X2VIEWER                     
			D3DXCOLOR   dxcOldColor;
            if(kInFrame_.m_bSelected)
            {
                dxcOldColor = m_dxcCurrentColor;
                SetColor( D3DXCOLOR( 1, 0, 0, 1 ) );
            }//if
            
#endif
			
            
			KTDXPROFILE_BEGIN( "DrawFrame_DRAW" );

#ifndef X2OPTIMIZE_SETSHADERCONSTANT
			m_bCommitChangesPending = true;
#endif//X2OPTIMIZE_SETSHADERCONSTANT
            _CommitChanges();    

// 			if ( pMC->m_pPMeshFull != NULL )
// 			{
// 				int meshIndex = (int)(pMC->m_cPMeshes * iInDetailPercent_/100.0f);
// 				if ( meshIndex > 0 )
// 					meshIndex--;
// 				hr = pMC->m_ppPMeshes[meshIndex]->DrawSubset( dwAttrib );
// 				//hr = pMC->m_pWorkingPMesh->DrawSubset( dwAttribId );
// 			}
// 			else
				hr = pMC->MeshData.pMesh->DrawSubset( dwAttrib );

			
#ifdef X2VIEWER
            if(kInFrame_.m_bSelected)
            {
                SetColor( dxcOldColor );
            }//if
#endif
			KTDXPROFILE_END();

            KD3DEND()
		}
	}
	else
	{
        m_pEffect->SetMatrixArray( EPHANDLE( g_amWorldPalette ), &kInFrame_.combineMatrix, 1 );

		D3DXMATRIX mx = g_pKTDXApp->GetViewProjectionTransform();
        D3DXMATRIX mFull;
        D3DXMatrixMultiply( &mFull, &kInFrame_.combineMatrix, &mx );
		m_pEffect->SetMatrix( EPHANDLE( g_mWorldViewProj ), &mFull );

#ifdef X2VIEWER          
        D3DXCOLOR   dxcOldColor;
        if(kInFrame_.m_bSelected)
        {
            dxcOldColor = m_dxcCurrentColor;
            SetColor( D3DXCOLOR( 1, 0, 0, 1 ) );
        }//if
#endif

		DWORD dwAttrib;

		// for each palette
        DWORD   dwNumSubsets = ( pMC->m_dwNumAttrGroups == 0 ) ? pMC->NumMaterials : pMC->m_dwNumAttrGroups;
        DWORD   dwAttribId;

		CKTDXDeviceBaseTexture* orgTex = NULL;
		CKTDXDeviceBaseTexture* pResultTexture0 = NULL;
		CKTDXDeviceBaseTexture* pResultTexture1 = NULL;
		CKTDXDeviceBaseTexture* pResultTexture2 = NULL;

		for( dwAttrib = 0; dwAttrib < dwNumSubsets; ++ dwAttrib )
		{
			// the texture here
            CKTDXDeviceBaseTexture* orgTex;
            dwAttribId = ( pMC->m_dwNumAttrGroups == 0 ) ? dwAttrib : pMC->GetAttributeRange( dwAttrib ).AttribId;
    	    orgTex = (CKTDXDeviceBaseTexture*)pMC->m_apTextures[ dwAttribId ];


            KD3DBEGIN()

			pResultTexture0 = _SetNowTexture( orgTex, 0, 
										pTexChangeXET, pMultiTexXET, 
										pAniData, fAniTime, abInUseTex_, pAnimAniExt, pwszInNowAnimName_ );
			pResultTexture1 = _SetNowTexture( orgTex, 1, 
										pTexChangeXET, pMultiTexXET, 
										pAniData, fAniTime, abInUseTex_, pAnimAniExt, pwszInNowAnimName_ );
			pResultTexture2 = _SetNowTexture( orgTex, 2, 
										pTexChangeXET, pMultiTexXET, 
										pAniData, fAniTime, abInUseTex_, pAnimAniExt, pwszInNowAnimName_ );

			if( pResultTexture0 != NULL )
			{
				pResultTexture0->SetDeviceTexture( 0 );
			}
			if( pResultTexture1 != NULL )
			{
				CKTDGStateManager::PushTextureStageState( 1, D3DTSS_COLOROP, pMultiTexXET->GetMultiTex( 1 )->colorOP );
				pResultTexture1->SetDeviceTexture( 1 );
			}
			if( pResultTexture2 != NULL )
			{
				CKTDGStateManager::PushTextureStageState( 2, D3DTSS_COLOROP, pMultiTexXET->GetMultiTex( 2 )->colorOP );
				pResultTexture2->SetDeviceTexture( 2 );
			}

			KTDXPROFILE_BEGIN( "DrawFrame_DRAW2" );

#ifndef X2OPTIMIZE_SETSHADERCONSTANT
			m_bCommitChangesPending = true;
#endif//X2OPTIMIZE_SETSHADERCONSTANT
            _CommitChanges();    


// 			if ( pMC->m_pPMeshFull != NULL )
// 			{
// 				int meshIndex = (int)(pMC->m_cPMeshes * iInDetailPercent_/100.0f);
// 				if ( meshIndex > 0 )
// 					meshIndex--;
// 				hr = pMC->m_ppPMeshes[meshIndex]->DrawSubset( dwAttribId );
// 				//hr = pMC->m_pWorkingPMesh->DrawSubset( dwAttribId );
// 			}
// 			else
//             {
				hr = pMC->MeshData.pMesh->DrawSubset( dwAttribId );
//            }//if

			KTDXPROFILE_END();

            KD3DEND()
		}
		
#ifdef X2VIEWER
        if(kInFrame_.m_bSelected)
        {
            SetColor( dxcOldColor );
        }//if
#endif

	}
}//CKTDGXRenderer::DrawFrame()


CKTDXDeviceBaseTexture* CKTDGXRenderer::_SetNowTexture( CKTDXDeviceBaseTexture* orgTex, int stage,
													CKTDXDeviceXET* pTexChangeXET, CKTDXDeviceXET* pMultiTexXET, 
													const CKTDXDeviceXET::AniData* pAniData, float fAniTime,
                                                    bool abInUseTex_[3], CKTDXDeviceXET* pAnimAniExt, const wchar_t* pwszInNowAnimName_ )
{
	KTDXPROFILE();
	if( orgTex == NULL )
		return NULL;

	//우선순위
	//1. XET 에니메이션 체인지 텍스쳐 데이타
	//2. XET 멀티 텍스쳐 데이타
	//3. XET 체인지 텍스쳐 데이타

	const WCHAR* pOrgTexName = orgTex->GetDeviceID().c_str();
	CKTDXDeviceBaseTexture* pResultTexture = NULL;

	// 애니메이션 체인지 텍스쳐 데이타와 체인지 텍스쳐 데이타 동시에 적용되게끔 수정.. MonkeyB 때문에
	if( stage == 0 )
	{
		if( abInUseTex_ != NULL && abInUseTex_[0] == false )
			return NULL;

		if( pAniData != NULL )		//애니메이션 체인지 텍스쳐 데이타가 있으면 처리
		{
			pResultTexture = pAniData->GetAniTexture( fAniTime, pOrgTexName );
			if( pResultTexture == NULL )
				pResultTexture = orgTex;
		}
		
		if( pTexChangeXET != NULL )		//체인지 텍스쳐 데이타가 있으면 처리
		{
			if ( pResultTexture != NULL )	//애니메이션 체인지 텍스쳐 데이타가 있다면
			{
				CKTDXDeviceBaseTexture* pAniTex = pResultTexture;
				pResultTexture = pTexChangeXET->GetChangeTexture( pResultTexture->GetDeviceID().c_str() );

				if ( pResultTexture == NULL )
					pResultTexture = pAniTex;
			}
			else							//애니메이션 체인지 텍스쳐 데이타가 없다면
			{
				
				pResultTexture = pTexChangeXET->GetChangeTexture( pOrgTexName );

				if( pResultTexture == NULL )
					pResultTexture = orgTex;
			}
		}
		
		if ( pAniData == NULL && pTexChangeXET == NULL )
		{
			// X2Viewer에서 NPC XET 보이도록 하기 위해서 수정 
			if( NULL != pAnimAniExt )
			{
				const CKTDXDeviceXET::AniData* pAniData = pAnimAniExt->GetAniData( pwszInNowAnimName_ );
				if( NULL != pAniData )
				{
					pResultTexture = pAniData->GetAniTexture( fAniTime, pOrgTexName );
					if( NULL == pResultTexture )
						pResultTexture = orgTex;
				}
				else
				{
					pResultTexture = orgTex;
				}
			}
			else
			{
				pResultTexture = orgTex;
			}
		}
	}
	else if( pMultiTexXET != NULL )
	{
		if( stage == 1 && abInUseTex_ != NULL && abInUseTex_[1] == false )
			return NULL;

		if( stage == 2 && abInUseTex_ != NULL && abInUseTex_[2] == false )
			return NULL;

		const CKTDXDeviceXET::MultiTexureData* pMultiTexureData = pMultiTexXET->GetMultiTex( stage );
		if( pMultiTexureData != NULL )
		{
			pResultTexture = pMultiTexureData->pTexture;
		}
	}

	return pResultTexture;

}//CKTDGXRenderer::_SetNowTexture()



HRESULT CKTDGXRenderer::OnResetDevice()
{ 
	HRESULT hr = 0;
	if( m_pEffect == NULL )
	{
		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"XRenderer.fx" );
		if( Info == NULL )
		{
			ErrorLogMsg( KEM_ERROR49, "XRenderer.fx" );
			return E_FAIL;
		}

#ifdef _IN_HOUSE_
		LPD3DXBUFFER pErr = NULL;
		if(FAILED(hr = D3DXCreateEffect( g_pKTDXApp->GetDevice(), Info->pRealData, Info->size,
			NULL, NULL, D3DXFX_NOT_CLONEABLE, NULL, &m_pEffect, &pErr )))

			//if(FAILED(hr = D3DXCreateEffectFromFileA( g_pKTDXApp->GetDevice(), "dat/System/FX/XSkinRenderer.fx",
			//	NULL, NULL, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION | D3DXFX_NOT_CLONEABLE, NULL, &m_pEffect, NULL )))
		{
			ErrorLog( KEM_ERROR50 );
			SAFE_RELEASE( m_pEffect );

			MessageBoxA(NULL, (LPCSTR)pErr->GetBufferPointer(), "Failed CreateEffect XRender", 0);
			return hr;
		}
#else
		if(FAILED(hr = D3DXCreateEffect( g_pKTDXApp->GetDevice(), Info->pRealData, Info->size,
			NULL, NULL, D3DXFX_NOT_CLONEABLE, NULL, &m_pEffect, NULL )))

		//if(FAILED(hr = D3DXCreateEffectFromFileA( g_pKTDXApp->GetDevice(), "dat/System/FX/XSkinRenderer.fx",
		//	NULL, NULL, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION | D3DXFX_NOT_CLONEABLE, NULL, &m_pEffect, NULL )))
		{
			ErrorLog( KEM_ERROR50 );
			SAFE_RELEASE( m_pEffect );
			return hr;
		}
#endif

        ASSERT( g_pEffectStateManager != NULL );
        hr = m_pEffect->SetStateManager( g_pEffectStateManager );
        ASSERT( SUCCEEDED( hr ) );

//{{ robobeg : 2008-10-08
        for( int i = 0; i < RT_END; i++ )
        {
            m_ahTechniques[ i ] = m_pEffect->GetTechniqueByName( CKTDGXRenderer::GetRenderTypeTechniqueName( CKTDGXRenderer::RENDER_TYPE( i ) ) );
            ASSERT( m_ahTechniques[ i ] != NULL );
        }//for

        for( int i = 0; i < EP_END; i++ )
        {
            m_ahEffectParameters[ i ] = m_pEffect->GetParameterByName( NULL
                , GetEffectParameterName( EFFECT_PARAMETER( i ) ) );
            ASSERT( m_ahEffectParameters[ i ] != NULL );
        }//for
//}} robobeg : 2008-10-08

        hr = m_pEffect->SetTechnique( m_ahTechniques[ 0 ] );
        ASSERT( SUCCEEDED( hr ) );
        m_eCurrentRenderType = RENDER_TYPE( 0 );

        hr = m_pEffect->SetInt( EPHANDLE( g_iShaderIndex ), 0 );
        ASSERT( SUCCEEDED( hr ) );
        m_iCurrentShaderIndex = 0;

        m_dxcCurrentColor = D3DXCOLOR( 0, 0, 0, 0 );
        hr = m_pEffect->SetVector( EPHANDLE( g_Color ), (const D3DXVECTOR4*) &m_dxcCurrentColor );
        ASSERT( SUCCEEDED( hr ) );

#ifndef X2OPTIMIZE_SETSHADERCONSTANT
		m_bCommitChangesPending = true;
#endif//X2OPTIMIZE_SETSHADERCONSTANT
	}

	if( m_pCartoonTexList[CTT_NORMAL] == NULL )
	{
		m_pCartoonTexList[CTT_NORMAL] = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"CartoonNormal.bmp" );
	}
	if( m_pCartoonTexList[CTT_RED] == NULL )
	{
		m_pCartoonTexList[CTT_RED] = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"CartoonFire.BMP" );
	}
	if( m_pCartoonTexList[CTT_GREEN] == NULL )
	{
		m_pCartoonTexList[CTT_GREEN] = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"CartoonGreen.bmp" );
	}
	if( m_pCartoonTexList[CTT_VIOLET] == NULL )
	{
		m_pCartoonTexList[CTT_VIOLET] = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"CartoonViolet.bmp" );
	}
	if( m_pCartoonTexList[CTT_NORMAL_COUNTER] == NULL )
	{
		m_pCartoonTexList[CTT_NORMAL_COUNTER] = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"CartoonCounterLight.bmp" );
	}
	if( m_pCartoonTexList[CTT_DAMAGE] == NULL )
	{
		m_pCartoonTexList[CTT_DAMAGE] = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"CartoonDamage.bmp" );
	}
	if( m_pCartoonTexList[CTT_INVINCIBLE] == NULL )
	{
		m_pCartoonTexList[CTT_INVINCIBLE] = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"CartoonInvincible.bmp" );
	}

	if( m_pCartoonTexList[CTT_PETRIFIED] == NULL )
	{
		m_pCartoonTexList[CTT_PETRIFIED] = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"CartoonPetrified.bmp" );		
	}
#ifdef ELSWORD_WAY_OF_SWORD
	if( m_pCartoonTexList[CTT_SKYBLUE] == NULL )
	{
		m_pCartoonTexList[CTT_SKYBLUE] = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"CartoonSkyBlue.bmp" );		
	}
	if( m_pCartoonTexList[CTT_SCARLET] == NULL )
	{
		m_pCartoonTexList[CTT_SCARLET] = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"CartoonScarlet.bmp" );		
	}
#endif ELSWORD_WAY_OF_SWORD


	return m_pEffect->OnResetDevice(); 
}

/*virtual*/
HRESULT	CKTDGXRenderer::OnLostDevice()
{ 
	//if(m_pEffect_Fog != NULL)
	//    m_pEffect_Fog->OnLostDevice();
#ifdef X2OPTIMIZE_SETSHADERCONSTANT
	if ( m_pEffect != NULL )
	{
		HRESULT hr = m_pEffect->OnLostDevice();
		if( SUCCEEDED( hr ) && g_pEffectStateManager )
		{
			g_pEffectStateManager->OnLostDevice();
		}
		return hr; 
	}
#else//X2OPTIMIZE_SETSHADERCONSTANT
	if ( m_pEffect != NULL )
		return m_pEffect->OnLostDevice(); 
#endif//X2OPTIMIZE_SETSHADERCONSTANT

	return S_OK;
}

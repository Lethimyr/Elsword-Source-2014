#pragma once


class CKTDGXRenderer
{
public:

		enum RENDER_TYPE
		{
			RT_REAL_COLOR = 0,			/// 자체 색상
			RT_CARTOON_BLACK_EDGE,		/// 카툰 + 검은 색 외곽선
			RT_CARTOON_COLOR_EDGE,		/// 카툰 + 다른 색 외곽선
			RT_CARTOON,					/// 카툰
			RT_ADD_CARTOON_BLACK_EDGE,	/// 카툰 + 검은 색 외곽선
			RT_ADD_CARTOON_COLOR_EDGE,	/// 카툰 + 다른 색 외곽선
			RT_ADD_CARTOON,				/// 카툰
			RT_BLACK_EDGE,				/// 검은 색 외곽선
			RT_COLOR_EDGE,				/// 다른 색 외곽선

            RT_LIGHT_FLOW_BEGIN,		/// 

			RT_LIGHT_FLOW_WORLD_X = RT_LIGHT_FLOW_BEGIN,
			RT_LIGHT_FLOW_WORLD_Y,
			RT_LIGHT_FLOW_WORLD_Z,
			RT_LIGHT_FLOW_WORLD_POINT,
			RT_LIGHT_FLOW_LOCAL_X,
			RT_LIGHT_FLOW_LOCAL_Y,
			RT_LIGHT_FLOW_LOCAL_Z,
			RT_LIGHT_FLOW_LOCAL_POINT,

            RT_LIGHT_FLOW_END = RT_LIGHT_FLOW_LOCAL_POINT,
			
			RT_CARTOON_FADE,
			RT_CARTOON_FADE_NONTEX,

            RT_END,
		};

		enum CARTOON_TEX_TYPE
		{
			CTT_NORMAL = 0,			/// CartoonNormal.bmp
			CTT_RED,				/// CartoonFire.bmp
			CTT_GREEN,				/// CartoonGreen.bmp
			CTT_VIOLET,				/// CartoonViolet.bmp
			CTT_NORMAL_COUNTER,		/// CartoonCounterLight.bmp
			CTT_DAMAGE,				/// CartoonDamage.bmp
			CTT_INVINCIBLE,			///	CartoonInvincible.bmp
			CTT_PETRIFIED,			/// CartoonPetrified.bmp
			CTT_SKYBLUE,			///	CartoonSkyBlue.bmp
			CTT_SCARLET,			/// CartoonScarlet.bmp
			CTT_END,
		};

        enum EFFECT_PARAMETER
        {
            // matrix
            EP_g_mViewProj,
            EP_g_mWorldViewProj, 

            // vector
            EP_g_Color,
            EP_g_TexOffSet01,
            EP_g_LightPos,
            EP_g_OutLineColor,
            EP_g_LightFlowPoint,

            //float
            EP_g_fTexOffSet2X,
            EP_g_fTexOffSet2Y,
            EP_g_fOutLineWide,
            EP_g_fLightFlowImpact,
            EP_g_fLightFlowWide,

#ifdef FOG_WORLD

            EP_FOG_BEGIN,

            //bool
            EP_g_bFog = EP_FOG_BEGIN,
            //vector
            EP_g_fFog,
            EP_g_fEye,
            EP_g_fColor,
            //float
            EP_g_fDensity,
			EP_FOG_END = EP_g_fDensity,
#endif // #ifdef FOG_WORLD
            
            EP_g_amWorldPalette,
            EP_g_iShaderIndex,
            EP_END,
        };//SKIN_EFFECT_PARAMETER

		struct RenderParam
		{
			RENDER_TYPE			renderType;				/// 유닛 버프에 사용
//{{ robobeg : 2008-10-24
			//D3DXMATRIX			worldMatrix;
//}} robobeg : 2008-10-24

			CARTOON_TEX_TYPE	cartoonTexType;			/// 유닛 버프에 사용
			D3DXVECTOR3			lightPos;
			float				fOutLineWide;			/// 유닛 버프에 사용
			D3DXCOLOR			outLineColor;			/// 유닛 버프에 사용 (a, r, g, b)
			
			D3DXCOLOR			color;					/// 유닛 버프에 사용
			float				fLightFlowWide;
			float				fLightFlowImpact;
			D3DXVECTOR3			lightFlowPoint;			

			D3DXVECTOR2			texOffsetStage0;
			D3DXVECTOR2			texOffsetStage1;
			D3DXVECTOR2			texOffsetStage2;

            bool				bAlphaBlend;			/// 유닛 버프에 사용
			bool				bZWriteEnable;
			bool				bZEnable;
			D3DCULL				cullMode;
			D3DBLEND			srcBlend;
			D3DBLEND			destBlend;

#ifdef FOG_WORLD
            // added by wonpok. fog
            bool                bFog;
            bool                bFogShow;
            D3DXCOLOR           fogColor;
            float               fogNearX;
            float               fogFarX;
            float               fogNearY;
            float               fogFarY;
            float               fogDensity;
#endif
#ifdef ADD_ALPHATESTENABLE
			bool				bAlphaTestEnable;
#endif

			RenderParam()
			{
				Init();
			}
			void Init()
			{
				renderType			= RT_REAL_COLOR;
//{{ robobeg : 2008-10-24
				//D3DXMatrixIdentity( &worldMatrix );
//}} robobeg : 2008-10-24

				cartoonTexType		= CTT_NORMAL;
				lightPos			= D3DXVECTOR3(0,0,0);
				fOutLineWide		= 0.0f;
				outLineColor		= 0xffffffff;

				color				= 0xffffffff;
				fLightFlowWide		= 0.0f;
				fLightFlowImpact	= 0.0f;
				lightFlowPoint		= D3DXVECTOR3(0,0,0);			

				texOffsetStage0		= D3DXVECTOR2(0,0);
				texOffsetStage1		= D3DXVECTOR2(0,0);
				texOffsetStage2		= D3DXVECTOR2(0,0);

				bAlphaBlend			= false;
				bZWriteEnable		= true;
				bZEnable			= true;
				cullMode			= D3DCULL_CCW;
				srcBlend			= D3DBLEND_SRCALPHA;
				destBlend			= D3DBLEND_INVSRCALPHA;

#ifdef FOG_WORLD
                // added by wonpok. fog
                bFog        =   false;
                bFogShow    =   true;
                fogColor    =   D3DXCOLOR(0xd8e3fe);
                fogNearX    =   0.0f;
                fogFarX     =   0.0f;
                fogNearY    =   0.0f;
                fogFarY     =   0.0f;
                fogDensity  =   0.0f;
#endif
#ifdef ADD_ALPHATESTENABLE
				bAlphaTestEnable	= false;
#endif
			}//Init()
		};//struct RenderParam


	public:

        static const char*      GetRenderTypeTechniqueName( RENDER_TYPE eInType_ );
        static const char*      GetEffectParameterName( EFFECT_PARAMETER eInParam_ );

		CKTDGXRenderer( LPDIRECT3DDEVICE9 pd3dDevice );
		virtual ~CKTDGXRenderer(void);

        void    SetColor( const D3DXCOLOR& dxcInColor_ );
        void    SetRenderPassFogAndMaxSkinningVertices( bool bInFog_, unsigned uInSkinningVertices_ );

		virtual HRESULT BeginRender( const CKTDGXRenderer::RenderParam& InRenderParam_, const D3DXMATRIX* pInMatrix_ = NULL );
		virtual HRESULT NextRender();

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
		HRESULT OnFrameRender( const RenderParam& InRenderParam_, const D3DXMATRIX& mInWorld_, CKTDXDeviceXMesh& pXMesh, 
			CKTDXDeviceXET* pTexChangeXET = NULL, CKTDXDeviceXET* pMultiTexXET = NULL, 
			const CKTDXDeviceXET::AniData* pAniData = NULL, float fAniTime = 0, unsigned uInDrawCount = 1, std::vector<bool>* pvecDrawSubset = NULL );
#else//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
		HRESULT OnFrameRender( const RenderParam& InRenderParam_, const D3DXMATRIX& mInWorld_, CKTDXDeviceXMesh& pXMesh, 
			CKTDXDeviceXET* pTexChangeXET = NULL, CKTDXDeviceXET* pMultiTexXET = NULL, 
			const CKTDXDeviceXET::AniData* pAniData = NULL, float fAniTime = 0, unsigned uInDrawCount = 1 );
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

        void    DrawFrame( CKTDXDeviceXSkinMesh& kInXSkinMesh_, CKTDXDeviceXSkinMesh::MultiAnimFrame& kInFrame_
            , CKTDXDeviceXET* pTexChangeXET = NULL, CKTDXDeviceXET* pMultiTexXET = NULL
            , const CKTDXDeviceXET::AniData* pAniData = NULL, float fAniTime = 0, int iInDetailPercent = 0, bool abInUseTex_[3] = NULL
            , CKTDXDeviceXET* pAnimAniExt = NULL, const wchar_t* pwszInNowAnimName_ = NULL );

        HRESULT BeginRenderGroup( CKTDGXRenderer::RENDER_TYPE eInRenderType_ );
        HRESULT NextRenderGroup();

		virtual HRESULT	OnResetDevice();
		virtual HRESULT	OnLostDevice();

//{{ robobeg : 2008-10-28

        // 외부에서 KTDGXRenderer 의 effect 를 직접 access 하는 일이 없도록 한다.

        //D3DXHANDLE   GetHandle( EFFECT_PARAMETER eInParam_ ) const 
        //{ 
        //    return ( eInParam_ >= EFFECT_PARAMETER( 0 ) && eInParam_ < EP_END ) ? m_ahEffectParameters[ eInParam_ ] : NULL; 
        //}//GetHandle()
//}} robobeg : 2008-10-28



    private:

        void                        _CommitChanges();

        CKTDXDeviceBaseTexture*     _SetNowTexture( CKTDXDeviceBaseTexture* orgTex, int stage,
													CKTDXDeviceXET* pTexChangeXET, CKTDXDeviceXET* pMultiTexXET, 
													const CKTDXDeviceXET::AniData* pAniData, float fAniTime,
                                                    bool abInUseTex_[3], CKTDXDeviceXET* pAnimAniExt, const wchar_t* pwszInNowAnimName_ );


    private:

		LPD3DXEFFECT				m_pEffect;

		CKTDXDeviceTexture*			m_pCartoonTexList[CTT_END]; 

        D3DXHANDLE                  m_ahTechniques[RT_END];
        D3DXHANDLE                  m_ahEffectParameters[EP_END];

        bool                        m_bInRenderGroup;
        bool                        m_bInBeginPass;
        UINT                        m_uNumPasses;
        UINT                        m_uCurPass;
        int                         m_iStackTop;

        bool                        m_bFog;	/// 포그 사용 여부 지정
        unsigned                    m_uMaxSkinningVertices;

#ifndef X2OPTIMIZE_SETSHADERCONSTANT
		bool                        m_bCommitChangesPending;
#endif//X2OPTIMIZE_SETSHADERCONSTANT

        CKTDGXRenderer::RENDER_TYPE m_eCurrentRenderType;
        int                         m_iCurrentShaderIndex;
        D3DXCOLOR                   m_dxcCurrentColor;
};//class CKTDGXRenderer


#define RENDER_HINT_NORENDER    RENDER_HINT( RENDER_HINT::NORENDER )
#define RENDER_HINT_DEFAULT     RENDER_HINT( RENDER_HINT::DEFAULT )
#define RENDER_HINT_XMESH( eRenderType, bFog )  RENDER_HINT( RENDER_HINT::XMESH, eRenderType, bFog )
#define RENDER_HINT_XSKINMESH( eRenderType, bFog, uMaxSkinning ) RENDER_HINT( RENDER_HINT::XSKINMESH, eRenderType, bFog, uMaxSkinning )

struct  RENDER_HINT
{
    enum    ERenderHint
    {
       NORENDER = -1,
       DEFAULT = 0,
       XMESH = 1,
       XSKINMESH = 2,
    };//enum    ERenderHint

    ERenderHint                 m_eHint;
    CKTDGXRenderer::RENDER_TYPE m_eRenderType;
    bool                        m_bFog;
    unsigned                    m_uMaxSkinningVertices;

    RENDER_HINT( ERenderHint eInRenderHint_ = DEFAULT , CKTDGXRenderer::RENDER_TYPE eInXRendererType_ = CKTDGXRenderer::RENDER_TYPE( 0 )
        , bool bInFog_ = false, unsigned uInMaxSkinning_ = 0 )
        : m_eHint( eInRenderHint_ ), m_eRenderType( eInXRendererType_ ), m_bFog( bInFog_ ), m_uMaxSkinningVertices( uInMaxSkinning_ ) {}

    bool IsSameCategory( const RENDER_HINT& kIn_ ) const
    {
        if ( m_eHint != kIn_.m_eHint )
            return false;
        else
        {
            if ( m_eHint == XMESH || m_eHint == XSKINMESH )
                return m_eRenderType == kIn_.m_eRenderType;
            else
                return true;
        }//if.. else...
    }//IsSameCategory()


    void MergeRenderHint( const RENDER_HINT& kIn_ )
    {
        switch( m_eHint )
        {
        case NORENDER:
            *this = kIn_;
            break;
        case DEFAULT:
            break;
        default: // XMESH, XSKINMESH
            if ( kIn_.m_eHint == NORENDER )
                break;
            else if ( kIn_.m_eHint == DEFAULT || !IsSameCategory( kIn_ ) )
                *this = RENDER_HINT_DEFAULT;
            else
            {
                if ( kIn_.m_bFog )
                    m_bFog = kIn_.m_bFog;
                if ( m_uMaxSkinningVertices < kIn_.m_uMaxSkinningVertices )
                    m_uMaxSkinningVertices = kIn_.m_uMaxSkinningVertices;
            }//if.. else..   
        }//switch
    }//MergeRenderHint()

    bool operator < ( const RENDER_HINT& kIn_ ) const
    {
        if ( m_eHint < kIn_.m_eHint )
            return true;
        else if ( m_eHint > kIn_.m_eHint )
            return false;
        else
        {
            if ( m_eHint == XMESH )
            {
                if ( m_eRenderType < kIn_.m_eRenderType )
                    return true;
                else if ( m_eRenderType > kIn_.m_eRenderType )
                    return false;
                else
                {
                    if ( m_eRenderType >= CKTDGXRenderer::RT_LIGHT_FLOW_BEGIN && m_eRenderType <= CKTDGXRenderer::RT_LIGHT_FLOW_END )
                        return false;
                    else 
                        return ( !m_bFog && kIn_.m_bFog );
                }//if.. else..
            }
            else if ( m_eHint == XSKINMESH )
            {
                if ( m_eRenderType < kIn_.m_eRenderType )
                    return true;
                else if ( m_eRenderType > kIn_.m_eRenderType )
                    return false;
                else
                {
                    if ( m_eRenderType >= CKTDGXRenderer::RT_LIGHT_FLOW_BEGIN && m_eRenderType <= CKTDGXRenderer::RT_LIGHT_FLOW_END )
                        return false;
                    else if ( !m_bFog && kIn_.m_bFog )
                        return true;
                    else if ( m_bFog && !kIn_.m_bFog )
                        return false;
                    else
                        return m_uMaxSkinningVertices < kIn_.m_uMaxSkinningVertices;
                }//if.. else..
            }
            else
            {
                return false;
            }//if.. else..
        }//if.. else..
    }//operator < ()

    bool operator > ( const RENDER_HINT& kIn_ ) const
    {
        return kIn_.operator < ( *this );
    }//

    bool operator == ( const RENDER_HINT& kIn_ ) const
    {
        return !( *this < kIn_ ) && !( kIn_ < *this );
    }//

    bool operator != ( const RENDER_HINT& kIn_ ) const
    {
        return !( operator == ( kIn_ ) );
    }//

    bool operator <= ( const RENDER_HINT& kIn_ ) const
    {
        return !( kIn_ < *this );
    }//

    bool operator >= ( const RENDER_HINT& kIn_ ) const
    {
        return !( *this < kIn_ );
    }//
};//struct  RENDER_HINT





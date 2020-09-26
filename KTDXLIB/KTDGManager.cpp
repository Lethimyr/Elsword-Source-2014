#include "StdAfx.h"
#include ".\ktdgmanager.h"

//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState akStates[] =
{
	KRenderState( D3DRS_LIGHTING,			FALSE ),
	KRenderState( D3DRS_SPECULARENABLE,	FALSE ),
	KRenderState( D3DRS_ALPHABLENDENABLE,	FALSE ),
	KRenderState( D3DRS_ALPHATESTENABLE,	TRUE ),
	//KRenderState( D3DRS_ALPHAREF,			0x00000001 ),
	//KRenderState( D3DRS_ALPHAFUNC,			D3DCMP_GREATER ),
	KRenderState( D3DRS_ZENABLE,			TRUE ),
	KRenderState( D3DRS_ZWRITEENABLE,		TRUE ),
	KRenderState( D3DRS_CULLMODE,			D3DCULL_CCW ),
	KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
	KRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA ),
	KSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ),
	KSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR ),
	KSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR ),
};//akStates[]


static const CKTDGStateManager::KState akRSICKTDXDeviceTexture[] =
{
	KRenderState( D3DRS_ALPHATESTENABLE,	TRUE ),
	KRenderState( D3DRS_ALPHAFUNC,			D3DCMP_NOTEQUAL ),
	KRenderState( D3DRS_ALPHAREF,			0 ),

	KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
	KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
	KRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA ),

	KRenderState( D3DRS_ZENABLE,			FALSE ),
	KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
	KRenderState( D3DRS_CULLMODE,			D3DCULL_CW ),

	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE ),
};//


static const CKTDGStateManager::KState akRSICKTDGSlashTrace[] =
{
	KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
	KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
	KRenderState( D3DRS_CULLMODE,			D3DCULL_NONE ),
	KRenderState( D3DRS_DESTBLEND,			D3DBLEND_DESTALPHA ),
	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_ADDSIGNED ),
};//





static const CKTDGStateManager::KState akRSICKTDGTexturedSlashTrace[] =
{

	KRenderState( D3DRS_ALPHATESTENABLE, FALSE ),
	KRenderState( D3DRS_ZENABLE, TRUE ),
	KRenderState( D3DRS_ZWRITEENABLE, FALSE ),
	KRenderState( D3DRS_CULLMODE, D3DCULL_NONE ),


	KRenderState( D3DRS_ALPHABLENDENABLE, TRUE ),
	KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
	//KRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA ),
	KRenderState( D3DRS_DESTBLEND,			D3DBLEND_DESTALPHA ),


	KTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 ),
	KTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE ),
	//KTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE ),

	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_SELECTARG1 ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1, 	D3DTA_DIFFUSE ),
	//KTextureStageState( 0, D3DTSS_ALPHAARG2, 	D3DTA_TEXTURE ),


};





static const CKTDGStateManager::KState akRSICKTDGLineMap[] =
{
	KRenderState( D3DRS_LIGHTING,			FALSE ),
	//KRenderState( D3DRS_ZENABLE,			FALSE ),
	//KRenderState( D3DRS_ZWRITEENABLE,	FALSE ),
	//KRenderState( D3DRS_CULLMODE,		D3DCULL_CW ),

	KRenderState( D3DRS_ALPHABLENDENABLE,	FALSE ),
	//KRenderState( D3DRS_SRCBLEND,		D3DBLEND_SRCALPHA ),
	//KRenderState( D3DRS_DESTBLEND,		D3DBLEND_INVSRCALPHA ),

	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_SELECTARG2 ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE ),

	KTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG2 ),
	KTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE ),
	KTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE ),
};

static const CKTDGStateManager::KState akRSICKTDGPlanShadow[] =
{
	KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
	KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
};

static const CKTDGStateManager::KState akRSICKTDGUIDialog[] =
{
	KRenderState( D3DRS_ALPHABLENDENABLE, TRUE ),
	KRenderState( D3DRS_ALPHATESTENABLE, FALSE ),
	KRenderState( D3DRS_ZENABLE, FALSE ),
	KRenderState( D3DRS_ZWRITEENABLE, FALSE ),

	KTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ),
	KTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ),

	KTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ),
};


// Normal
/*static*/
static const CKTDGStateManager::KState akRSICKTDXDeviceLayer0[] =
{
    KRenderState( D3DRS_ALPHATESTENABLE,	FALSE ),

    KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
    KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
    KRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA ),

    KRenderState( D3DRS_ZENABLE,			FALSE ),
    KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
    KRenderState( D3DRS_CULLMODE,			D3DCULL_NONE ),

	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE ),
};//

// 덧셈: 광원/빛 효과 (밝아짐)
/*static*/
static const CKTDGStateManager::KState akRSICKTDXDeviceLayer1[] =
{
    KRenderState( D3DRS_ALPHATESTENABLE,	FALSE ),

    KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
    KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
    KRenderState( D3DRS_DESTBLEND,			D3DBLEND_ONE ),

    KRenderState( D3DRS_ZENABLE,			FALSE ),
    KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
    KRenderState( D3DRS_CULLMODE,			D3DCULL_NONE ),

	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE ),
};//

// 뺄셈: (어두움)
/*static*/
static const CKTDGStateManager::KState akRSICKTDXDeviceLayer2[] =
{
    KRenderState( D3DRS_ALPHATESTENABLE,	FALSE ),

    KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
    KRenderState( D3DRS_BLENDOP,			D3DBLENDOP_REVSUBTRACT ),
    KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
    KRenderState( D3DRS_DESTBLEND,			D3DBLEND_ONE ),

    KRenderState( D3DRS_ZENABLE,			FALSE ),
    KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
    KRenderState( D3DRS_CULLMODE,			D3DCULL_NONE ),

	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE ),
};//

// 곱셈: 밝은곳이 투명해짐 (시야 출력)
/*static*/
static const CKTDGStateManager::KState akRSICKTDXDeviceLayer3[] =
{
    KRenderState( D3DRS_ALPHATESTENABLE,	FALSE ),

    KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
    KRenderState( D3DRS_BLENDOP,            D3DBLENDOP_MIN ),
    KRenderState( D3DRS_SRCBLEND,			D3DBLEND_ZERO ),
    KRenderState( D3DRS_DESTBLEND,			D3DBLEND_SRCCOLOR ),

    KRenderState( D3DRS_ZENABLE,			FALSE ),
    KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
    KRenderState( D3DRS_CULLMODE,			D3DCULL_NONE ),
};//

// 제곱: 밝은곳은 더 밝게 어두운곳은 더 어둡게
/*static*/
static const CKTDGStateManager::KState akRSICKTDXDeviceLayer4[] =
{
    KRenderState( D3DRS_ALPHATESTENABLE,	FALSE ),

    KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
    KRenderState( D3DRS_SRCBLEND,			D3DBLEND_ZERO ),
    KRenderState( D3DRS_DESTBLEND,			D3DBLEND_DESTCOLOR ),

    KRenderState( D3DRS_ZENABLE,			FALSE ),
    KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
    KRenderState( D3DRS_CULLMODE,			D3DCULL_NONE ),
};//

// 반전: 화면색 반전
/*static*/
static const CKTDGStateManager::KState akRSICKTDXDeviceLayer5[] =
{
    KRenderState( D3DRS_ALPHATESTENABLE,	FALSE ),

    KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
    KRenderState( D3DRS_SRCBLEND,			D3DBLEND_INVDESTCOLOR ),
    KRenderState( D3DRS_DESTBLEND,			D3DBLEND_ZERO ),

    KRenderState( D3DRS_ZENABLE,			FALSE ),
    KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
    KRenderState( D3DRS_CULLMODE,			D3DCULL_NONE ),
};//
//}} robobeg : 2008-10-13



CKTDGManager::CKTDGManager( HWND hWnd, LPDIRECT3DDEVICE9 pd3dDevice )
: m_Far( 0.f )
, m_Near( 0.f )
, m_fMaxTimeFadeIn( 0.f )
, m_fMaxTimeFadeOut( 0.f )
, m_uNonAlphaObjectNum( 0 )
, m_kCamera( g_pKTDXApp->GetDevice() )
, m_kFrustum( g_pKTDXApp->GetDevice() )
{
	m_hWnd				= hWnd;

	m_ClearScreen		= 0;
	m_ClearScreenColor	= 0x00ffffff;
	m_ClearColor		= 0x32323232;
	m_Near				= 1.0f;
	m_Far				= 10000.0f;
	m_bPerspective		= true;


#ifdef BUBBLE_BOBBLE_TEST
	m_fOrthoScale		= 1.f;
#endif BUBBLE_BOBBLE_TEST

#ifdef FIELD_CHARACTER_SCREEN_SHOT
	m_bField_Character_Screen_shot	=	false;
	m_bField_NPC_Screen_shot		=	false;
	m_bField_MainCha_Screen_shot	=	false;
	m_bField_BG_Screen_shot			=	false;
#endif

//{{ robobeg : 2008-10-13
//	InitializeCriticalSection( &m_csRenderObjectChainLock );
//}} robobeg : 2008-10-13

//{{ robobeg : 2008-10-13
	//CKTDGStateManager::Init();
	//m_BaseRenderStateID = CKTDGStateManager::RecordStateBegin();
	//{
	//	CKTDGStateManager::CaptureRenderState( D3DRS_LIGHTING,			FALSE );
	//	CKTDGStateManager::CaptureRenderState( D3DRS_SPECULARENABLE,	FALSE );
	//	CKTDGStateManager::CaptureRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );
	//	CKTDGStateManager::CaptureRenderState( D3DRS_ALPHATESTENABLE,	TRUE );
	//	//CKTDGStateManager::CaptureRenderState( D3DRS_ALPHAREF,			0x00000001 );
	//	//CKTDGStateManager::CaptureRenderState( D3DRS_ALPHAFUNC,			D3DCMP_GREATER );
	//	CKTDGStateManager::CaptureRenderState( D3DRS_ZENABLE,			TRUE );
	//	CKTDGStateManager::CaptureRenderState( D3DRS_ZWRITEENABLE,		TRUE );
	//	CKTDGStateManager::CaptureRenderState( D3DRS_CULLMODE,			D3DCULL_CCW );
	//	CKTDGStateManager::CaptureRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA );
	//	CKTDGStateManager::CaptureRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA );


	//	CKTDGStateManager::CaptureSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	//	CKTDGStateManager::CaptureSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	//	CKTDGStateManager::CaptureSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	//}
	//CKTDGStateManager::RecordStateEnd();
//}} robobeg : 2008-10-13

//{{ robobeg : 2008-10-13
	//m_RSICKTDXDeviceTexture		= 0;
	//m_RSICKTDGSlashTrace		= 0;
	//m_RSICKTDGLineMap			= 0;
	//m_RSICKTDGPlanShadow		= 0;
	//m_RSICKTDGUIDialog			= 0;

 //   for(int i=0; i<6; ++i)
 //       m_RSICKTDXDeviceLayer[i]       = 0;
//}} robobeg : 2008-10-13

	BuildRenderStateID();

	//m_pCamera				= new CKTDGCamera( g_pKTDXApp->GetDevice() );
	//m_pFrustum				= new CKTDGFrustum( g_pKTDXApp->GetDevice() );
	m_pDialogManager		= new CKTDGUIDialogManager();
	//m_pGlow					= new CKTDGGlow( g_pKTDXApp->GetDevice() );
	//m_pMotionBlur			= new CKTDGMotionBlur( g_pKTDXApp->GetDevice() );
	m_pXRenderer			= new CKTDGXRenderer( g_pKTDXApp->GetDevice() );
	m_pFontManager			= new CKTDGFontManager( g_pKTDXApp->GetDevice() );
	//m_pGrayScale			= new CKTDGGrayScale();

//{{ robobeg : 2008-10-13
	//m_RenderObjectChain.reserve( 4096 );
	//m_AlphaObjectChain.reserve( 4096 );
	//m_NonAlphaObjectChain.reserve( 4096 );
//}} robobeg : 2008-10-13

	m_InnerUIChain.reserve( 512 );
	m_OverUIChain.reserve( 512 );
#ifdef OVER_UI_ALPHA_RENDER_FIX
	m_OverUIAlphaChain.reserve( 512 );
#endif OVER_UI_ALPHA_RENDER_FIX

	m_bFading						= false;
	m_bFadeOut						= true;
	m_fTimeFade						= 0.0f;
	m_fTimeFadeMax					= 0.0f;
	m_fFadeAlpha					= 0.0f;
	m_pTextureFadeInOut				= NULL;


	m_fCaptureDelay					= 3.0f;

//{{ robobeg : 2008-10-18
    m_uAlphaObjectNum = 0;
    m_nCull = 0;
//}} robobeg : 2008-10-18

#ifdef MODIFY_FRUSTUM
	m_fPerspective = 4.f;
#endif

#ifdef TEST_SCREEN_CAPTURE
	// 09-03-09.hoons.화면캡쳐에 필요함 폴더를 생성해 놓는다.
	WCHAR wszDir[MAX_PATH] = L"";
	::GetCurrentDirectoryW( MAX_PATH, wszDir );

#ifdef	CONVERSION_VS
	wcscat_s( wszDir, _countof(wszDir), L"\\ScreenCapture" );
#else	CONVERSION_VS
	wcscat( wszDir, L"\\ScreenCapture" );
#endif	CONVERSION_VS
	
	if( wcslen(wszDir) > 14 )	// L"\\ScreenCapture" 을 포함한 글자수보다 많아야 폴더를 생성한다.
	{
		::CreateDirectoryW( wszDir, NULL );
	}

#endif TEST_SCREEN_CAPTURE
}

CKTDGManager::~CKTDGManager(void)
{
	//SAFE_DELETE( m_pGrayScale );
	//SAFE_DELETE( m_pCamera );
	//SAFE_DELETE( m_pFrustum );
	SAFE_DELETE( m_pDialogManager );
	//SAFE_DELETE( m_pGlow );
	//SAFE_DELETE( m_pMotionBlur );
	SAFE_DELETE( m_pXRenderer );
	SAFE_DELETE( m_pFontManager );
	SAFE_CLOSE( m_pTextureFadeInOut );


//{{ robobeg : 2008-10-13
	//CKTDGStateManager::Free();
//}} robobeg : 2008-10-13

//{{ robobeg : 2008-10-13
	//EnterCriticalSection( &m_csRenderObjectChainLock );
//}} robobeg : 2008-10-13
	m_RenderObjectChain.clear();
	//m_AlphaObjectChain.clear();
	//m_NonAlphaObjectChain.clear();
#ifdef KTDGMANAGER_USE_DISABLE_LAYER
	m_DisableLayer.clear();
#endif// #ifdef KTDGMANAGER_USE_DISABLE_LAYER
//{{ robobeg : 2008-10-13
	//LeaveCriticalSection( &m_csRenderObjectChainLock );
	//DeleteCriticalSection( &m_csRenderObjectChainLock );
//}} robobeg : 2008-10-13

}

void CKTDGManager::BuildRenderStateID()
{
	m_RSICKTDXDeviceTexture = akRSICKTDXDeviceTexture;
	m_RSICKTDGSlashTrace = akRSICKTDGSlashTrace;
	m_RSICKTDGTexturedSlashTrace = akRSICKTDGTexturedSlashTrace;
	m_RSICKTDGLineMap = akRSICKTDGLineMap;
	m_RSICKTDGPlanShadow = akRSICKTDGPlanShadow;
	m_RSICKTDGUIDialog = akRSICKTDGUIDialog;


    // Normal
    m_RSICKTDXDeviceLayer[0] = akRSICKTDXDeviceLayer0;
    // 덧셈: 광원/빛 효과 (밝아짐)
    m_RSICKTDXDeviceLayer[1] = akRSICKTDXDeviceLayer1;

    // 뺄셈: (어두움)
    m_RSICKTDXDeviceLayer[2] = akRSICKTDXDeviceLayer2;

    // 곱셈: 밝은곳이 투명해짐 (시야 출력)
    m_RSICKTDXDeviceLayer[3] = akRSICKTDXDeviceLayer3;

    // 제곱: 밝은곳은 더 밝게 어두운곳은 더 어둡게
    m_RSICKTDXDeviceLayer[4] = akRSICKTDXDeviceLayer4;

    // 반전: 화면색 반전
    m_RSICKTDXDeviceLayer[5] = akRSICKTDXDeviceLayer5;

}

bool CKTDGManager::BeginRender()
{
	KTDXPROFILE();

	if( m_ClearScreen > 0 )
	{
		m_ClearScreen--;

		const float MAGIC_CLEAR_SCREEN_GAP = 0.2f;
		if( m_ClearScreenTimer.elapsed() > MAGIC_CLEAR_SCREEN_GAP )
		{
			g_pKTDXApp->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_ClearScreenColor, 1.0f, 0);
			m_ClearScreenTimer.restart();
			return false;
		}
	}

	//if( m_pMotionBlur->GetBlurEnable() == true )
	//	m_pMotionBlur->BeginRender();
	//else
		g_pKTDXApp->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_ClearColor, 1.0f, 0);

	//m_pGlow->BeginRender();

	return true;
}

void CKTDGManager::EndRender()
{
	KTDXPROFILE();

	//m_pGlow->EndRender();

	//if( m_pMotionBlur->GetBlurEnable() == true )
	//{
	//	m_pMotionBlur->EndRender();
	//}
}



//{{ robobeg : 2008-10-13
void    CKTDGManager::_RenderObjectRenderHintVector( const CKTDGManager::KObjectRenderHintVector& vecIn_, bool bInLayerExclusive_ )
{

#ifdef KTDGMANAGER_USE_DISABLE_LAYER
	int renderLayer = 0;
    int renderLayer2;
    bool bRender = IsLayerRenderable( renderLayer );
    bool bRender2;
#endif // #ifdef KTDGMANAGER_USE_DISABLE_LAYER

    RENDER_HINT   kRenderHint;
    int               iRenderHintLayer = 0;

    HRESULT hr;
    int iStart = 0;
    int iIndex = 0;
    int iCount = 0;
    int iIndex2;


    KD3DBEGIN()

        CKTDGStateManager::KStateID stateID;
        int     iStackTop = CKTDGStateManager::GetStackTop();

        iStart = 0;
        iIndex = 0;
        iCount = 0;

        BOOST_TEST_FOREACH( const KObjectRenderHint&, kObjectRenderHint, vecIn_ )
        {
            ASSERT( kObjectRenderHint.m_pObject != NULL );
			if( kObjectRenderHint.m_pObject == NULL )
			{
				ErrorLogMsg( KEM_ERROR422, "RenderHint pObject Is NULL" );
				continue;
			}

#ifdef KTDGMANAGER_USE_DISABLE_LAYER
            if( renderLayer != kObjectRenderHint.m_pObject->GetLayer() )
            {
                renderLayer = kObjectRenderHint.m_pObject->GetLayer();
                bRender = IsLayerRenderable( renderLayer );
            }//if

            if( bRender )
            {
#endif // #ifdef KTDGMANAGER_USE_DISABLE_LAYER

                if ( iCount > 0
                    && ( !kRenderHint.IsSameCategory( kObjectRenderHint.m_kRenderHint ) || bInLayerExclusive_ && iRenderHintLayer != kObjectRenderHint.m_pObject->GetLayer() ) )
                {
                    ASSERT( kRenderHint.m_eHint == RENDER_HINT::XMESH
                        || kRenderHint.m_eHint == RENDER_HINT::XSKINMESH );

                    if ( iCount == 1 )
                    {
                        const KObjectRenderHint& prev = vecIn_[ iStart ];

                        if( prev.m_pObject->GetRenderStateID() != stateID )
			            {
                            CKTDGStateManager::PopStates( iStackTop );
                            stateID = prev.m_pObject->GetRenderStateID();
                            CKTDGStateManager::PushStates( stateID );
			            }//if
                        m_pXRenderer->SetRenderPassFogAndMaxSkinningVertices( prev.m_kRenderHint.m_bFog, prev.m_kRenderHint.m_uMaxSkinningVertices );
                        prev.m_pObject->OnFrameRender_Draw(); 
                    }
                    else
                    {
#ifdef KTDGMANAGER_USE_DISABLE_LAYER
                        renderLayer2 = renderLayer;
                        bRender2 = bRender;
#endif // #ifdef KTDGMANAGER_USE_DISABLE_LAYER

                        if ( stateID != CKTDGStateManager::KStateID() )
                        {
                            CKTDGStateManager::PopStates( iStackTop );
                            stateID = CKTDGStateManager::KStateID();
                        }//if

                        for( hr = m_pXRenderer->BeginRenderGroup( kRenderHint.m_eRenderType ); SUCCEEDED( hr );
                            hr = m_pXRenderer->NextRenderGroup() )
                        {
                            for( iIndex2 = iStart; iIndex2 < iIndex; iIndex2++ )
                            {
                                const KObjectRenderHint& prev = vecIn_[ iIndex2 ];
#ifdef KTDGMANAGER_USE_DISABLE_LAYER
                                if( renderLayer2 != prev.m_pObject->GetLayer() )
                                {
                                    renderLayer2 = prev.m_pObject->GetLayer();
                                    bRender2 = IsLayerRenderable( renderLayer2 );
                                }//if
                                if ( bRender2 )
                                {
#endif // #ifdef KTDGMANAGER_USE_DISABLE_LAYER

			                        if( prev.m_pObject->GetRenderStateID() != stateID )
			                        {
                                        CKTDGStateManager::PopStates( iStackTop );
                                        stateID = prev.m_pObject->GetRenderStateID();
                                        CKTDGStateManager::PushStates( stateID );
			                        }//if
                                    m_pXRenderer->SetRenderPassFogAndMaxSkinningVertices( prev.m_kRenderHint.m_bFog, prev.m_kRenderHint.m_uMaxSkinningVertices );
                                    prev.m_pObject->OnFrameRender_Draw();   

#ifdef KTDGMANAGER_USE_DISABLE_LAYER
                                }//if
#endif // #ifdef KTDGMANAGER_USE_DISABLE_LAYER

                            }//for
                        }//for

                        stateID = CKTDGStateManager::KStateID();
                    }//if.. else..
                    iCount = 0;
                }//if
                if ( kObjectRenderHint.m_kRenderHint.m_eHint == RENDER_HINT::XMESH
                    || kObjectRenderHint.m_kRenderHint.m_eHint == RENDER_HINT::XSKINMESH )
                {
                    if ( iCount == 0 )
                    {
                        kRenderHint = kObjectRenderHint.m_kRenderHint;
                        iRenderHintLayer = kObjectRenderHint.m_pObject->GetLayer();
                        iCount = 1;
                        iStart = iIndex;
                    }
                    else
                    {
                        ASSERT( kRenderHint.IsSameCategory( kObjectRenderHint.m_kRenderHint ) );
                        iCount++;
                    }//if.. else..
                }
                else
                {
                    ASSERT( iCount == 0 );
			        if( kObjectRenderHint.m_pObject->GetRenderStateID() != stateID )
			        {
                        CKTDGStateManager::PopStates( iStackTop );
                        stateID = kObjectRenderHint.m_pObject->GetRenderStateID();
                        CKTDGStateManager::PushStates( stateID );
			        }//if
                    kObjectRenderHint.m_pObject->OnFrameRender_Draw();    
                }//if.. else..

#ifdef KTDGMANAGER_USE_DISABLE_LAYER
            }
#endif // #ifdef KTDGMANAGER_USE_DISABLE_LAYER

            iIndex++;
        }//BOOST_TEST_FOREACH()


        if ( iCount > 0 )
        {
			ASSERT( kRenderHint.m_eHint == RENDER_HINT::XMESH
                || kRenderHint.m_eHint == RENDER_HINT::XSKINMESH );

            if ( iCount == 1 )
            {
                const KObjectRenderHint& prev = vecIn_[ iStart ];
			    if( prev.m_pObject->GetRenderStateID() != stateID )
			    {
                    CKTDGStateManager::PopStates( iStackTop );
                    stateID = prev.m_pObject->GetRenderStateID();
                    CKTDGStateManager::PushStates( stateID );
			    }//if
                m_pXRenderer->SetRenderPassFogAndMaxSkinningVertices( prev.m_kRenderHint.m_bFog, prev.m_kRenderHint.m_uMaxSkinningVertices );
                prev.m_pObject->OnFrameRender_Draw(); 
            }
            else
            {

#ifdef KTDGMANAGER_USE_DISABLE_LAYER
                renderLayer2 = renderLayer;
                bRender2 = bRender;
#endif // #ifdef KTDGMANAGER_USE_DISABLE_LAYER


                if ( stateID != CKTDGStateManager::KStateID() )
                {
                    CKTDGStateManager::PopStates( iStackTop );
                    stateID = CKTDGStateManager::KStateID();
                }//if

                for( hr = m_pXRenderer->BeginRenderGroup( kRenderHint.m_eRenderType ); SUCCEEDED( hr );
                    hr = m_pXRenderer->NextRenderGroup() )
                {
                    for( iIndex2 = iStart; iIndex2 < iIndex; iIndex2++ )
                    {
                        const KObjectRenderHint& prev = vecIn_[ iIndex2 ];

#ifdef KTDGMANAGER_USE_DISABLE_LAYER
                        if( renderLayer2 != prev.m_pObject->GetLayer() )
                        {
                            renderLayer2 = prev.m_pObject->GetLayer();
                            bRender2 = IsLayerRenderable( renderLayer2 );
                        }//if
                        if ( bRender2 )
                        {
#endif // #ifdef KTDGMANAGER_USE_DISABLE_LAYER

			                if( prev.m_pObject->GetRenderStateID() != stateID )
			                {
                                CKTDGStateManager::PopStates( iStackTop );
                                stateID = prev.m_pObject->GetRenderStateID();
                                CKTDGStateManager::PushStates( stateID );
			                }//if
                            m_pXRenderer->SetRenderPassFogAndMaxSkinningVertices( prev.m_kRenderHint.m_bFog, prev.m_kRenderHint.m_uMaxSkinningVertices );
                            prev.m_pObject->OnFrameRender_Draw();   
#ifdef KTDGMANAGER_USE_DISABLE_LAYER
                        }//if
#endif // #ifdef KTDGMANAGER_USE_DISABLE_LAYER

                    }//for
                }//for

                stateID = CKTDGStateManager::KStateID();

            }//if.. else..
            iCount = 0;
        }//if

    KD3DEND()

}//CKTDGManager::_RenderObjectRenderHintVector()
//}} robobeg : 2008-10-13



void CKTDGManager::BackUIRender()
{
    _RenderObjectRenderHintVector( m_InnerUIChain );

	//후경 UI그리기
//{{ robobeg : 2008-10-13
    KD3DBEGIN()
	    m_pDialogManager->OnFrameRender( false );
    KD3DEND()
//}} robobeg : 2008-10-13
}


void CKTDGManager::ObjectChainNonAlphaRender()
{
	KTDXPROFILE();

	BackUIRender();

#ifdef FIELD_CHARACTER_SCREEN_SHOT
	if ( m_bField_Character_Screen_shot == TRUE )
	{
		for(int iLayer=10; iLayer<16; ++iLayer) 
		{
			_RenderObjectRenderHintVector( m_NonAlphaGroup[iLayer].m_layerObject, true );
		}
	}
	else if( m_bField_NPC_Screen_shot == TRUE )
	{
		for(int iLayer=10; iLayer<11; ++iLayer) 
		{
			//m_NonAlphaGroup[iLayer].m_layerObject[0].m_pObject->m_hKTDGManagerHandle
			_RenderObjectRenderHintVector( m_NonAlphaGroup[iLayer].m_layerObject, true );
		}
	}
	else if( m_bField_MainCha_Screen_shot == TRUE )
	{
		// 레이어층의 정확한 부분 12,16
		for(int iLayer=11; iLayer<16; ++iLayer) 
		{
			_RenderObjectRenderHintVector( m_NonAlphaGroup[iLayer].m_layerObject, true );
		}
	}
	else if( m_bField_BG_Screen_shot == TRUE )
	{
		// 레이어층의 정확한 부분 5,6
		for(int iLayer=5; iLayer<6; ++iLayer) 
		{
			_RenderObjectRenderHintVector( m_NonAlphaGroup[iLayer].m_layerObject, true );
		}
	}
	else//기존 코드와 호환성을 위해 이런식으로 처리
#endif

	for(int iLayer=0; iLayer<m_ciNumLayerGroups; ++iLayer)
	{
		_RenderObjectRenderHintVector( m_NonAlphaGroup[iLayer].m_layerObject, true );
	}//for
}

void CKTDGManager::ObjectChainAlphaRender()
{
	KTDXPROFILE();

#ifdef FIELD_CHARACTER_SCREEN_SHOT
	if ( m_bField_Character_Screen_shot == TRUE )
	{
		for(int iLayer=10; iLayer<16; ++iLayer) 
		{
			_RenderObjectRenderHintVector( m_AlphaGroup[iLayer].m_layerObject, true );
		}
	}
	else if( m_bField_NPC_Screen_shot == TRUE )
	{
		for(int iLayer=10; iLayer<11; ++iLayer) 
		{
			//m_NonAlphaGroup[iLayer].m_layerObject[0].m_pObject->m_hKTDGManagerHandle
			_RenderObjectRenderHintVector( m_AlphaGroup[iLayer].m_layerObject, true );
		}
	}
	else if( m_bField_MainCha_Screen_shot == TRUE )
	{
		// 레이어층의 정확한 부분 12,16
		for(int iLayer=11; iLayer<16; ++iLayer) 
		{
			_RenderObjectRenderHintVector( m_AlphaGroup[iLayer].m_layerObject, true );
		}
	}
	else if( m_bField_BG_Screen_shot == TRUE )
	{
		// 레이어층의 정확한 부분 5,6
		for(int iLayer=5; iLayer<6; ++iLayer) 
		{
			_RenderObjectRenderHintVector( m_AlphaGroup[iLayer].m_layerObject, true );
		}
	}
	else//기존 코드와 호환성을 위해 이런식으로 처리
#endif FIELD_CHARACTER_SCREEN_SHOT

    for(int iLayer=0; iLayer<m_ciNumLayerGroups; ++iLayer)
    {
        _RenderObjectRenderHintVector( m_AlphaGroup[iLayer].m_layerObject, true );
    }//for
}

void CKTDGManager::FrontUIRender()
{
	//전경 UI그리기
//{{ robobeg : 2008-10-13
    KD3DBEGIN()
	    m_pDialogManager->OnFrameRender( true );
    KD3DEND()
//}} robobeg : 2008-10-13

	// fade in, out effect
	if( m_fFadeAlpha > 0.0f )
	{
		D3DXCOLOR color = 0xffffffff;
		color.a = m_fFadeAlpha;
		if( m_pTextureFadeInOut != NULL )
			m_pTextureFadeInOut->Draw( 0, 0, 1024, 768, color );
	}

    _RenderObjectRenderHintVector( m_OverUIChain );
#ifdef OVER_UI_ALPHA_RENDER_FIX
	_RenderObjectRenderHintVector( m_OverUIAlphaChain );
#endif OVER_UI_ALPHA_RENDER_FIX

//}} robobeg : 2008-10-13
}

#ifdef FILDE_ONLY_CHARACTER
void CKTDGManager::AlphaLayerRender( int iLayer )
{
	if( iLayer >= m_ciNumLayerGroups )
		return;

	_RenderObjectRenderHintVector( m_AlphaGroup[iLayer].m_layerObject, true );
}

void CKTDGManager::NonAlphaLayerRender( int iLayer )
{
	if( iLayer >= m_ciNumLayerGroups )
		return;

	_RenderObjectRenderHintVector( m_NonAlphaGroup[iLayer].m_layerObject, true );
}
#endif FILDE_ONLY_CHARACTER



HRESULT CKTDGManager::OnResetDevice()
{
	////CKTDXThread::CLocker locker( m_csRenderObjectChainLock );

	CKTDGStateManager::RestoreDeviceObjects( 0, D3DDEVTYPE_HAL );
//{{ robobeg : 2008-10-13
    CKTDGStateManager::SetStates( akStates );
//}} robobeg : 2008-10-13

//{{ robobeg : 2008-10-13
	//for( int i = 0; i < (int)m_RenderObjectChain.size(); i++ )
	//{
	//	if ( m_RenderObjectChain[i] != NULL )
	//		m_RenderObjectChain[i]->OnResetDevice();
	//}
    BOOST_TEST_FOREACH( CKTDGObject*, pObject, m_RenderObjectChain )
    {
        if ( pObject != NULL )
            pObject->OnResetDevice();
    }//
//}} robobeg : 2008-10-13
	if ( m_pXRenderer )
		m_pXRenderer->OnResetDevice();

	if ( m_pFontManager != NULL )
		m_pFontManager->OnResetDevice();
	if ( m_pDialogManager != NULL )
		m_pDialogManager->OnResetDevice();
	//m_pGlow->OnResetDevice();
	//m_pMotionBlur->OnResetDevice();

	//m_pGrayScale->OnResetDevice();

	m_pTextureFadeInOut = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"FadeInOut.dds" );

	return S_OK;
}

HRESULT CKTDGManager::OnLostDevice()
{
	////CKTDXThread::CLocker locker( m_csRenderObjectChainLock );

//{{ robobeg : 2008-10-13
	//CKTDGStateManager::EndStateBlock( m_BaseRenderStateID );
//}} robobeg : 2008-10-13
	CKTDGStateManager::InvalidateDeviceObjects();

//{{ robobeg : 2008-10-13
	//for( int i = 0; i < (int)m_RenderObjectChain.size(); i++ )
	//{
	//	if ( m_RenderObjectChain[i] != NULL )
	//		m_RenderObjectChain[i]->OnLostDevice();
	//}
    BOOST_TEST_FOREACH( CKTDGObject*, pObject, m_RenderObjectChain )
    {
        if ( pObject != NULL )
            pObject->OnLostDevice();
    }//
//}} robobeg : 2008-10-13


	if ( m_pDialogManager != NULL )
		m_pDialogManager->OnLostDevice();
	if ( m_pXRenderer != NULL )
		m_pXRenderer->OnLostDevice();
	if ( m_pFontManager != NULL )
		m_pFontManager->OnLostDevice();
	//m_pGrayScale->OnLostDevice();

	return S_OK;
}

HRESULT CKTDGManager::OnFrameMove( double fTime, float fElapsedTime )
{
	m_pFontManager->OnFrameMove( (float)fTime, fElapsedTime );
	//m_pGrayScale->OnFrameMove( fElapsedTime );


	if( m_bFading == true )
	{
		if( m_bFadeOut == true )
		{
			m_fTimeFade += fElapsedTime;

			if( m_fTimeFade > m_fTimeFadeMax )
			{
				m_bFading			= false;
				m_bFadeOut			= false;
				m_fTimeFadeMax		= 0.0f;
				m_fTimeFade			= 0.0f;
				m_fFadeAlpha		= 1.0f;
			}
			else
			{
				m_fFadeAlpha		= m_fTimeFade / m_fTimeFadeMax;
			}
		}
		else
		{
			m_fTimeFade -= fElapsedTime;

			if( m_fTimeFade < 0.0f )
			{
				m_bFading			= false;
				m_bFadeOut			= false;
				m_fTimeFadeMax		= 0.0f;
				m_fTimeFade			= 0.0f;
				m_fFadeAlpha		= 0.0f;
			}
			else
			{
				m_fFadeAlpha		= m_fTimeFade / m_fTimeFadeMax;
			}
		}
	}

	m_fCaptureDelay -= fElapsedTime;
	if( m_fCaptureDelay < 0.0f )
		m_fCaptureDelay = 0.0f;

	return S_OK;
}

HRESULT CKTDGManager::OnFrameRender()
{
	//m_pFontManager->OnFrameRender();

	return S_OK;
}
#ifdef CONVERSION_VS
HRESULT CKTDGManager::AddObjectChain( CKTDGObject* object )
{ 
	KTDXPROFILE();
	if ( object == NULL )
		return E_FAIL;
	ASSERT( CKTDGObject::CKTDGManagerAccess::_GetDGManagerHandle( *object ) == GetRenderObjectChainEnd() );
	if ( CKTDGObject::CKTDGManagerAccess::_GetDGManagerHandle( *object ) != GetRenderObjectChainEnd() )
		return E_FAIL;
	CKTDGObject::CKTDGManagerAccess::_SetDGManagerHandle( *object, m_RenderObjectChain.insert( m_RenderObjectChain.end(), object ) );
	object->NotifyDGManagerChanged( *this );
	return S_OK;
}
HRESULT CKTDGManager::RemoveObjectChain( CKTDGObject* object )
{
	KTDXPROFILE();

	ASSERT( object != NULL );
	if ( object == NULL )
		return E_FAIL;
	if ( CKTDGObject::CKTDGManagerAccess::_GetDGManagerHandle( *object ) == GetRenderObjectChainEnd() )
		return E_FAIL;
	m_RenderObjectChain.erase( CKTDGObject::CKTDGManagerAccess::_GetDGManagerHandle( *object ) );
	CKTDGObject::CKTDGManagerAccess::_SetDGManagerHandle( *object, GetRenderObjectChainEnd() );
	object->NotifyDGManagerChanged( *this );
	return S_OK;
}
#endif CONVERSION_VS

#ifdef BUBBLE_BOBBLE_TEST
	void CKTDGManager::SetProjection( float Near, float Far, bool bPerspective /*= true*/, float fOrthoScale /*= 1.f*/ )
#else
	void CKTDGManager::SetProjection( float Near, float Far, bool bPerspective )
#endif BUBBLE_BOBBLE_TEST
{
	RECT WindowRect;

#ifdef USE_BACKBUFFER_SIZE_WHEN_PROJECTION
	const D3DSURFACE_DESC* pbackBufferSurfaceDesc = DXUTGetBackBufferSurfaceDesc();
	if( pbackBufferSurfaceDesc )
	{
		WindowRect.left = 0;
		WindowRect.right = pbackBufferSurfaceDesc->Width;
		WindowRect.top = 0;
		WindowRect.bottom = pbackBufferSurfaceDesc->Height;
	}
	else
#endif USE_BACKBUFFER_SIZE_WHEN_PROJECTION
	{
		GetClientRect(m_hWnd, &WindowRect);
	}

	m_Near			= Near;
	m_Far			= Far;
	m_bPerspective	= bPerspective;

	float fWidth	= (float)(WindowRect.right - WindowRect.left);
	float fHeight	= (float)(WindowRect.bottom - WindowRect.top);
	if( fWidth <= 0 )
		fWidth = 1024;
	if( fHeight <= 0 )
		fHeight = 768;


	if( bPerspective == true )
	{
		//Create 3D Projection Matrix
#ifdef MODIFY_FRUSTUM
		D3DXMatrixPerspectiveFovLH( &m_matProjection, D3DX_PI / GetPerspectiveValue(),
			fWidth / fHeight,
			m_Near, m_Far );
#else
		D3DXMatrixPerspectiveFovLH( &m_matProjection, D3DX_PI/4,
									fWidth / fHeight,
									m_Near, m_Far );
#endif
	}
	else
	{
#ifdef BUBBLE_BOBBLE_TEST
		m_fOrthoScale = fOrthoScale;
		D3DXMatrixOrthoLH( &m_matProjection, 1024 * fOrthoScale, 768 * fOrthoScale,	m_Near, m_Far );
#else
		D3DXMatrixOrthoLH( &m_matProjection, 1024,
							768,
							m_Near, m_Far );
#endif BUBBLE_BOBBLE_TEST
	}

	g_pKTDXApp->SetProjectionTransform( &m_matProjection );
}

#ifdef KEYFRAME_CAMERA
void CKTDGManager::SetProjectionFov( float Near, float Far, float fFov )
{
	RECT WindowRect;
	GetClientRect(m_hWnd, &WindowRect);

	m_Near			= Near;
	m_Far			= Far;
	m_bPerspective	= true;

	float fWidth	= (float)(WindowRect.right - WindowRect.left);
	float fHeight	= (float)(WindowRect.bottom - WindowRect.top);
	if( fWidth <= 0 )
		fWidth = 1024;
	if( fHeight <= 0 )
		fHeight = 768;

	//Create 3D Projection Matrix
	D3DXMatrixPerspectiveFovLH( &m_matProjection, D3DX_PI/fFov,
		fWidth / fHeight,
		m_Near, m_Far );

	g_pKTDXApp->SetProjectionTransform( &m_matProjection );
}
#endif KEYFRAME_CAMERA

struct CKTDGObjectUISort : public std::binary_function< CKTDGManager::KObjectRenderHint, CKTDGManager::KObjectRenderHint, bool >
{
   bool operator()( const CKTDGManager::KObjectRenderHint& _Left, const CKTDGManager::KObjectRenderHint& _Right ) const
   {
       return ( _Left.m_kRenderHint < _Right.m_kRenderHint
                || !(_Right.m_kRenderHint < _Left.m_kRenderHint)
                && _Left.m_pObject->GetDistanceToCamera() < _Right.m_pObject->GetDistanceToCamera() );
   };
};//struct CKTDGObjectUISort


struct CKTDGObjectNonAlphaSort : public std::binary_function< CKTDGManager::KObjectRenderHint, CKTDGManager::KObjectRenderHint, bool >
{
   bool operator()( const CKTDGManager::KObjectRenderHint& _Left, const CKTDGManager::KObjectRenderHint& _Right ) const
   {
       return _Left.m_pObject->GetLayer() < _Right.m_pObject->GetLayer()
           || _Left.m_pObject->GetLayer() == _Right.m_pObject->GetLayer()
             && ( _Left.m_kRenderHint < _Right.m_kRenderHint
                || !(_Right.m_kRenderHint < _Left.m_kRenderHint)
                && _Left.m_pObject->GetDistanceToCamera() < _Right.m_pObject->GetDistanceToCamera() );
   };
};//struct CKTDGObjectNonAlphaSort


struct CKTDGObjectAlphaSort : public std::binary_function< CKTDGManager::KObjectRenderHint, CKTDGManager::KObjectRenderHint, bool >
{
   bool operator()( const CKTDGManager::KObjectRenderHint& _Left, const CKTDGManager::KObjectRenderHint& _Right ) const
   {
       return   _Left.m_pObject->GetLayer() < _Right.m_pObject->GetLayer()
           || _Left.m_pObject->GetLayer() == _Right.m_pObject->GetLayer()
              && _Left.m_pObject->GetDistanceToCamera() > _Right.m_pObject->GetDistanceToCamera();
   };
};//struct CKTDGObjectAlphaSort


//{{ robobeg : 2008-10-18

void CKTDGManager::ObjectChainSort()
{
	KTDXPROFILE();

	////CKTDXThread::CLocker locker( m_csRenderObjectChainLock );

	m_kFrustum.Construct( GetFar() );

    m_OverUIChain.resize( 0 );
#ifdef OVER_UI_ALPHA_RENDER_FIX
	m_OverUIAlphaChain.resize( 0 );
#endif OVER_UI_ALPHA_RENDER_FIX
    m_InnerUIChain.resize( 0 );

    //m_NonAlphaObjectChain.resize( 0 );

    for(int i=0; i<m_ciNumLayerGroups; ++i)
    {
        m_NonAlphaGroup[i].m_layerObject.resize( 0 );
        m_AlphaGroup[i].m_layerObject.resize( 0 );
    }

    m_nCull = 0;
    m_uAlphaObjectNum = 0;

    KObjectRenderHint   kObjectRenderHint;
    BOOST_TEST_FOREACH( CKTDGObject*, object, m_RenderObjectChain )
    {
        if(object == NULL)
            continue;

        object->SetInstantShow( false );

#ifndef WORLD_TOOL

        if ( IsInFrustum( object ) == false )
        {
            ++m_nCull;
            continue;
        }
#else
		// 09.03.02 김태완 : 월드툴일 때 오브젝트와 카메라까지의 거리를 계산해 주기 위해서.
		// 월드툴이 아닐 때는 IsInFrustum에서 처리되는 코드.
		D3DXVECTOR3 center;
		object->GetTransformCenter( &center );
		object->SetDistanceToCamera( GetDistance( GetCamera().GetEye(), center ) );
#endif

        //{{ seojt
        // frustum안에 있으면 마지막 접근 시간을 갱신한다. - jintaeks on 2008-10-22, 16:39
        //object->SetLastAccessTime( g_NowTime );
        //}} seojt

        kObjectRenderHint.m_pObject = object;
        kObjectRenderHint.m_kRenderHint = object->OnFrameRender_Prepare();
        if ( kObjectRenderHint.m_kRenderHint.m_eHint == RENDER_HINT::NORENDER )
            continue;

        object->SetInstantShow( true );

		//논알파, 알파 분류
        if( object->GetOverUI() == true )
		{
#ifdef OVER_UI_ALPHA_RENDER_FIX
			if( object->IsAlphaObject() == false
				|| object->GetForceLayer() == true )
			{
				m_OverUIChain.push_back( kObjectRenderHint );
			}
			else
			{
				m_OverUIAlphaChain.push_back( kObjectRenderHint );
			}
#else
			m_OverUIChain.push_back( kObjectRenderHint );
#endif OVER_UI_ALPHA_RENDER_FIX
		}
        else if ( object->GetInnerUI() == true )
        {
            m_InnerUIChain.push_back( kObjectRenderHint );
        }
        else
		{
            int iLayer = MapLayerGroup( object->GetLayer() );

            if( iLayer < 0)
                continue;
            ASSERT( iLayer < m_ciNumLayerGroups );
			if( object->IsAlphaObject() == false
				|| object->GetForceLayer() == true )
			{
				m_NonAlphaGroup[iLayer].m_layerObject.push_back(kObjectRenderHint);
				m_uNonAlphaObjectNum++;
			}
			else
			{
				m_AlphaGroup[iLayer].m_layerObject.push_back(kObjectRenderHint);
				m_uAlphaObjectNum++;
			}		
		}
    }//BOOST_TEST_FOREACH()


    std::sort( m_OverUIChain.begin(), m_OverUIChain.end(), CKTDGObjectUISort() );
#ifdef OVER_UI_ALPHA_RENDER_FIX
	std::sort( m_OverUIAlphaChain.begin(), m_OverUIAlphaChain.end(), CKTDGObjectUISort() );
#endif OVER_UI_ALPHA_RENDER_FIX
    std::sort( m_InnerUIChain.begin(), m_InnerUIChain.end(), CKTDGObjectUISort() );

    for( int iLayer = 0; iLayer<m_ciNumLayerGroups; ++iLayer)
    {
        std::sort( m_NonAlphaGroup[iLayer].m_layerObject.begin(), m_NonAlphaGroup[iLayer].m_layerObject.end(), CKTDGObjectNonAlphaSort() );
        std::sort( m_AlphaGroup[iLayer].m_layerObject.begin(), m_AlphaGroup[iLayer].m_layerObject.end(), CKTDGObjectAlphaSort() );
    }//for
    //}
//#endif
}

//}} robobeg : 2008-10-18


void CKTDGManager::StartFadeIn( float fTime )
{
	m_bFading			= true;
	m_bFadeOut			= false;
	m_fTimeFadeMax		= fTime;
	m_fTimeFade			= fTime;
	m_fFadeAlpha		= 1.0f;
}

void CKTDGManager::StartFadeOut( float fTime )
{
	m_bFading			= true;
	m_bFadeOut			= true;
	m_fTimeFadeMax		= fTime;
	m_fTimeFade			= 0.0f;
	m_fFadeAlpha		= 0.0f;
}

//{{ kimhc // 2010.9.8 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트
#ifdef	AVATAR_IMAGE_TEST
bool CKTDGManager::ScreenCaptureForAvatar( OUT wstring& wstrFileName, CKTDXDeviceRenderTargetTexture* pRenderTargetTextureAvatar_ )
{
	if( m_fCaptureDelay > 0.0f )
		return false;

	const int	FILE_WIDTH	= 300;
	const int	FILE_HEIGHT	= 300;

	const float	DEFAULT_RESOLUTION_X = 1024.0f;
	const float DEFAULT_RESOLUTION_Y = 768.0f;

	m_fCaptureDelay = 3.0f;

	RECT rect;
	HWND pDesktop = g_pKTDXApp->GetHWND(); // 내자신의 윈도우의 좌표를 조사한다. 
	GetClientRect( pDesktop, &rect ); // 윈도우 화면 DC와 메모리 DC를 만들고 메모리 비트맵을 선택한다. 

	rect.left	= 0;
	rect.top	= 0;
	rect.right	= static_cast<long>( FILE_WIDTH * rect.right / DEFAULT_RESOLUTION_X );
	rect.bottom = static_cast<long>( FILE_HEIGHT * rect.bottom / DEFAULT_RESOLUTION_Y );

	__time64_t long_time;
	_time64( &long_time );                /* Get time as long integer. */

#ifdef	CONVERSION_VS
	struct tm newtime;
	_localtime64_s( &newtime, &long_time ); /* Convert to local time. */

	std::stringstream strStream;
	strStream << "SC_"
		<< ( 1900 + newtime.tm_year ) << "_"
		<< (newtime.tm_mon+1)	<< "_"
		<< newtime.tm_mday		<< "_"
		<< newtime.tm_hour		<< "_"
		<< newtime.tm_min		<< "_"
		<< newtime.tm_sec		<< "_";
#else	CONVERSION_VS
	struct tm *newtime;
	newtime = _localtime64( &long_time ); /* Convert to local time. */
	std::stringstream strStream;
	if ( newtime != NULL )
    {
	    strStream << "SC_"
		    << ( 1900 + newtime->tm_year ) << "_"
		    << (newtime->tm_mon+1)	<< "_"
		    << newtime->tm_mday		<< "_"
		    << newtime->tm_hour		<< "_"
		    << newtime->tm_min		<< "_"
		    << newtime->tm_sec		<< "_";
    }
    else
    {
	    strStream << "SC_"
    }

#endif	CONVERSION_VS

	srand( (unsigned)time( NULL ) );
	strStream << ( rand() / RandomInt( 1, 9 ) ) + RandomFloat();

	wstring wstrRandom = GET_STRING( RandomInt( STR_ID_0, STR_ID_9000 ) );
	string	strRandom;
	ConvertWCHARToChar( strRandom, wstrRandom );

	strStream << strRandom.c_str();	
	
	string strFinalFileName = KncUtil::GetMD5( strStream.str() );
	strFinalFileName += ".png";	

	WIN32_FIND_DATAA FindFileData;
	if ( INVALID_HANDLE_VALUE != FindFirstFileA( strFinalFileName.c_str(), &FindFileData ) )
	{
		// 중복된 파일 있으면 삭제
		BOOL bDeleteFile = DeleteFileA( strFinalFileName.c_str() );

		// 삭제가 안됬으면
		if ( FALSE == bDeleteFile )
		{
			// 스샷을 보내지 않음
			return false;
		}
	}
	
	HRESULT hResult = D3DXSaveSurfaceToFileA( strFinalFileName.c_str(), D3DXIFF_PNG, pRenderTargetTextureAvatar_->GetSurface(), NULL, &rect );	//D3DXSaveSurfaceToFile( stream.str().c_str(), D3DXIFF_PNG, pBackBuffer, NULL, &rect );

	ConvertCharToWCHAR( wstrFileName, strFinalFileName );

	CxImage cimage;
	bool bSucceeded = cimage.Load( strFinalFileName.c_str(), CXIMAGE_FORMAT_PNG );

	if( bSucceeded == false ) {
		ASSERT( !L"Fail to open ScreenCapture File" );
		return false;
	}
	else
	{
		cimage.Resample( FILE_WIDTH, FILE_HEIGHT );
		cimage.Save( strFinalFileName.c_str(), CXIMAGE_FORMAT_PNG );
	}
	
	return true;
}
#endif	AVATAR_IMAGE_TEST
//}} kimhc // 2010.9.8 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트


bool CKTDGManager::ScreenCaptureOld()
{	
	m_fCaptureDelay = 3.0f;
	//{{ 09-03-04.hoons. 좀도 빠른 캡쳐를 위해
	//DWORD dwTime = ::GetTickCount();

	HBITMAP	hBitmap; 
	RECT	rect; 
	HDC		hMemDC;
	HDC		hScrDC; 
	HBITMAP	hOldBitmap; //DC를 얻는다. 

	HWND	pDesktop = g_pKTDXApp->GetHWND();

	hScrDC = GetDC( pDesktop );

	GetClientRect( pDesktop, &rect ); // 윈도우 화면 DC와 메모리 DC를 만들고 메모리 비트맵을 선택한다. 

	CreateDC( L"DISPLAY", NULL, NULL, NULL ); 
	hMemDC	= CreateCompatibleDC( hScrDC ); 
	hBitmap	= CreateCompatibleBitmap( hScrDC, rect.right - rect.left, rect.bottom - rect.top ); 
	hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap ); // 화면을 메모리 비트맵으로 복사한다. 
	BitBlt( hMemDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hScrDC, rect.left, rect.top, SRCCOPY ); 
	SelectObject( hMemDC, hOldBitmap ); 

	CxImage cimage( CXIMAGE_FORMAT_JPG ); 

	if( cimage.CreateFromHBITMAP( hBitmap ) ) 
	{ 
		cimage.SetJpegQuality( 70 );

		__time64_t long_time;
		_time64( &long_time );                /* Get time as long integer. */

#ifdef	CONVERSION_VS
		struct tm newtime;
		_localtime64_s( &newtime, &long_time ); /* Convert to local time. */

		std::stringstream stream;
		stream << "ScreenCapture\\";
		stream  << "SC_"
			<< ( 1900 + newtime.tm_year ) << "_"
			<< (newtime.tm_mon+1)	<< "_"
			<< newtime.tm_mday		<< "_"
			<< newtime.tm_hour		<< "_"
			<< newtime.tm_min		<< "_"
			<< newtime.tm_sec		<< "_"
			<< ".jpg";
#else	CONVERSION_VS
		struct tm *newtime;
		newtime = _localtime64( &long_time ); /* Convert to local time. */

		std::stringstream stream;
		stream << "ScreenCapture\\";
        if ( newtime != NULL )
        {
		    stream  << "SC_"
			    << ( 1900 + newtime->tm_year ) << "_"
			    << (newtime->tm_mon+1)	<< "_"
			    << newtime->tm_mday		<< "_"
			    << newtime->tm_hour		<< "_"
			    << newtime->tm_min		<< "_"
			    << newtime->tm_sec		<< "_"
			    << ".jpg";
        }
        else
        {
		    stream  << "SC_.jpg";
        }
#endif	CONVERSION_VS

		cimage.Save( stream.str().c_str(), CXIMAGE_FORMAT_JPG ); 
	} 

	DeleteDC( hMemDC ); 
	//DeleteDC( hScrDC ); 
	DeleteObject( hBitmap ); 
	DeleteObject( hOldBitmap );

	DeleteDC( hScrDC );

	return true;
}

bool CKTDGManager::ScreenCapture()
{
	if( m_fCaptureDelay > 0.0f )
		return false;

	m_fCaptureDelay = 3.0f;

#ifndef TEST_SCREEN_CAPTURE
	//DWORD dwTime = ::GetTickCount();

	std::wstringstream stream;

	struct tm *newtime;
	__time64_t long_time;
	_time64( &long_time );                /* Get time as long integer. */
	newtime = _localtime64( &long_time ); /* Convert to local time. */

	IDirect3DSurface9* pBackBuffer = 0;
	g_pKTDXApp->GetDevice()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

    if ( newtime != NULL )
    {
	    stream  << L"SC_"
		    << ( 1900 + newtime->tm_year ) << L"_"
		    << (newtime->tm_mon+1)	<< L"_"
		    << newtime->tm_mday		<< L"_"
		    << newtime->tm_hour		<< L"_"
		    << newtime->tm_min		<< L"_"
		    << newtime->tm_sec		<< L"_"
		    << L".jpg";
    }
    else
    {
	    stream  << L"SC_.jpg";
    }
	D3DXSaveSurfaceToFile( stream.str().c_str(), D3DXIFF_JPG, pBackBuffer, NULL, NULL );
	pBackBuffer->Release();

	//dwTime = GetTickCount() - dwTime;
	//debugWPrintf( L"capture Time : %d", dwTime );
#else
	//{{ 09-03-04.hoons. 좀도 빠른 캡쳐를 위해
	//DWORD dwTime = ::GetTickCount();

	bool bSuccess = false;

	HBITMAP	hBitmap; 
	RECT	rect; 
	HDC		hMemDC;
	HDC		hScrDC; 
	HBITMAP	hOldBitmap; //DC를 얻는다. 

	IDirect3DSurface9* pBackBuffer = 0;
	g_pKTDXApp->GetDevice()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	HRESULT hResultDC = pBackBuffer->GetDC(&hScrDC);

	HWND	pDesktop = g_pKTDXApp->GetHWND();

	if( D3D_OK != hResultDC )
	{
		pBackBuffer->Release();

		hScrDC = GetDC( pDesktop ); 
	}

	GetClientRect( pDesktop, &rect ); // 윈도우 화면 DC와 메모리 DC를 만들고 메모리 비트맵을 선택한다. 

	CreateDC( L"DISPLAY", NULL, NULL, NULL ); 
	hMemDC	= CreateCompatibleDC( hScrDC ); 
	hBitmap	= CreateCompatibleBitmap( hScrDC, rect.right - rect.left, rect.bottom - rect.top ); 
	hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap ); // 화면을 메모리 비트맵으로 복사한다. 
	BitBlt( hMemDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hScrDC, rect.left, rect.top, SRCCOPY ); 
	SelectObject( hMemDC, hOldBitmap ); 

	CxImage cimage( CXIMAGE_FORMAT_JPG ); 

	if( cimage.CreateFromHBITMAP( hBitmap ) ) 
	{ 
		cimage.SetJpegQuality( 70 );

		__time64_t long_time;
		_time64( &long_time );                /* Get time as long integer. */

#ifdef	CONVERSION_VS
		struct tm newtime;
		_localtime64_s( &newtime, &long_time ); /* Convert to local time. */

		std::stringstream stream;
		stream << "ScreenCapture\\";

		stream  << "SC_"
			<< ( 1900 + newtime.tm_year ) << "_"
			<< (newtime.tm_mon+1)	<< "_"
			<< newtime.tm_mday		<< "_"
			<< newtime.tm_hour		<< "_"
			<< newtime.tm_min		<< "_"
			<< newtime.tm_sec		<< "_"
			<< ".jpg";
#else	CONVERSION_VS
		struct tm *newtime;
		newtime = _localtime64( &long_time ); /* Convert to local time. */

		std::stringstream stream;
		stream << "ScreenCapture\\";
        if ( newtime != NULL )
        {
		    stream  << "SC_"
			    << ( 1900 + newtime->tm_year ) << "_"
			    << (newtime->tm_mon+1)	<< "_"
			    << newtime->tm_mday		<< "_"
			    << newtime->tm_hour		<< "_"
			    << newtime->tm_min		<< "_"
			    << newtime->tm_sec		<< "_"
			    << ".jpg";
        }
        else
        {
		    stream  << "SC_.jpg";
        }
#endif	CONVERSION_VS

		cimage.Save( stream.str().c_str(), CXIMAGE_FORMAT_JPG ); 

		bSuccess = true;
	} 

	DeleteDC( hMemDC ); 
	//DeleteDC( hScrDC ); 
	DeleteObject( hBitmap ); 
	DeleteObject( hOldBitmap );

	if( D3D_OK != hResultDC )
	{
		DeleteDC( hScrDC ); 
	}
	else
	{
		pBackBuffer->ReleaseDC(hScrDC);
		pBackBuffer->Release();
	}

	if( bSuccess == false )
	{
		ScreenCaptureOld();
	}
#endif TEST_SCREEN_CAPTURE

	return true;
}
#ifdef FILDE_ONLY_CHARACTER 
struct TEXTURE_DATA
{
	union
	{
		unsigned char arrayData[3];
	};
};
bool CKTDGManager::ScreenCapture_Png()
{

	//DWORD dwTime = ::GetTickCount();

	std::wstringstream stream;

	struct tm newtime;
	__time64_t long_time;
	_time64( &long_time );                /* Get time as long integer. */
	_localtime64_s( &newtime, &long_time ); /* Convert to local time. */

	IDirect3DSurface9* pBackBuffer = 0;
	g_pKTDXApp->GetDevice()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

	stream  << L"ScreenCapture\\SC_"
		<< ( 1900 + newtime.tm_year ) << L"_"
		<< (newtime.tm_mon+1)	<< L"_"
		<< newtime.tm_mday		<< L"_"
		<< newtime.tm_hour		<< L"_"
		<< newtime.tm_min		<< L"_"
		<< newtime.tm_sec		<< L"_"
		<< L".png";
	pBackBuffer->UnlockRect();
	//L_cBuf
	std::stringstream mstream;
	mstream  << "ScreenCapture\\SC_"
		<< ( 1900 + newtime.tm_year ) << "_"
		<< (newtime.tm_mon+1)	<< "_"
		<< newtime.tm_mday		<< "_"
		<< newtime.tm_hour		<< "_"
		<< newtime.tm_min		<< "_"
		<< newtime.tm_sec		<< "_"
		<< ".png";

	D3DXSaveSurfaceToFile( stream.str().c_str(), D3DXIFF_PNG, pBackBuffer, NULL, NULL );

	CxImage cimage;
	bool bSucceeded = cimage.Load( mstream.str().c_str(), CXIMAGE_FORMAT_PNG );//파일열고
	TEXTURE_DATA * L_buf=(TEXTURE_DATA *)cimage.GetBits();//어드레스 기억
	long sizeX=cimage.GetWidth(),sizeY=cimage.GetHeight();//사이즈 할당
	int size=sizeX*sizeY;//실사이즈 구하고
	for(int i=0;i<size;++i)//실사이즈 만큼 돌고
	{
		//뻭버퍼 클리어칼라는 뻬버린다
		//간혹 뻭버퍼랑 같은 칼라면 같이 빠지는데 그건 그때 알아서
		if(L_buf[i].arrayData[0]==50 && L_buf[i].arrayData[1]==50 && L_buf[i].arrayData[2]==50)
		{
			L_buf[i].arrayData[0]=0xff;//흰색으로 채운다
			L_buf[i].arrayData[1]=0xff;
			L_buf[i].arrayData[2]=0xff;
		}

	}
	RGBQUAD bkg = { 0xff, 0xff, 0xff, 0xff };//칼라키를 설정한다
	cimage.AlphaPaletteEnable(true);//알파값 enable설정
	cimage.SetTransIndex(0);
	cimage.SetTransColor(bkg);
	cimage.AlphaStrip();

	if( bSucceeded == false ) {
		ASSERT( !L"Fail to open ScreenCapture File" );
		return false;
	}
	else
	{
		//cimage.Resample( sizeX, sizeY );//사이즈 재조정 한다
		cimage.Save( mstream.str().c_str(), CXIMAGE_FORMAT_PNG );
	}


	pBackBuffer->Release();

	//dwTime = GetTickCount() - dwTime;
	//debugWPrintf( L"capture Time : %d", dwTime );


	return true;

}
#endif FILDE_ONLY_CHARACTER

#ifdef FIELD_CHARACTER_SCREEN_SHOT 
struct TEXTURE_DATA
{
	union
	{
		unsigned char arrayData[4];
	};
};
bool CKTDGManager::ScreenCapture_Png()
{
	m_bField_Character_Screen_shot	=	false;
	m_bField_NPC_Screen_shot		=	false;
	m_bField_MainCha_Screen_shot	=	false;
	m_bField_BG_Screen_shot			=	false;

	std::wstringstream stream;

	struct tm *newtime;
	__time64_t long_time;
	_time64( &long_time );                
	newtime = _localtime64( &long_time ); 

	IDirect3DSurface9* pBackBuffer = 0;
	g_pKTDXApp->GetDevice()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer ); 
	// mono 아니년, 테스트
    if ( newtime != NULL )
    {
	    stream  << L"ScreenCapture\\SC_"
		    << ( 1900 + newtime->tm_year ) << L"_"
		    << (newtime->tm_mon+1)	<< L"_"
		    << newtime->tm_mday		<< L"_"
		    << newtime->tm_hour		<< L"_"
		    << newtime->tm_min		<< L"_"
		    << newtime->tm_sec		<< L"_"
		    << L".png";
    }
    else
    {
	    stream  << L"ScreenCapture\\SC_.png";
    }
	pBackBuffer->UnlockRect();
	//L_cBuf
	std::stringstream mstream;
    if ( newtime != NULL )
    {
	    mstream  << "ScreenCapture\\SC_"
		    << ( 1900 + newtime->tm_year ) << "_"
		    << (newtime->tm_mon+1)	<< "_"
		    << newtime->tm_mday		<< "_"
		    << newtime->tm_hour		<< "_"
		    << newtime->tm_min		<< "_"
		    << newtime->tm_sec		<< "_"
		    << ".png"; 
    }
    else
    {
	    mstream  << "ScreenCapture\\SC_.png";
    }

	D3DXSaveSurfaceToFile( stream.str().c_str(), D3DXIFF_PNG, pBackBuffer, NULL, NULL ); //D3DXIFF_PNG
	// 파일에 표면모습을 저장

	CxImage cimage;
	bool bSucceeded = cimage.Load( mstream.str().c_str(), CXIMAGE_FORMAT_PNG );


	// [120211] 김석근, 전체적으로 알파값을 만들어 추가하고, 마젠타 준 부분만 알파값을 적용해주는 방식.
	cimage.AlphaCreate();
	TEXTURE_DATA * L_buf=(TEXTURE_DATA *)cimage.GetBits();//어드레스 기억
	long sizeX=cimage.GetWidth(),sizeY=cimage.GetHeight();//사이즈 할당
	//int size=sizeX*sizeY;//실사이즈 구하고

	for( int iX = 0; iX < sizeX; ++iX )
	{
		for( int iY = 0; iY < sizeY; ++iY )
		{
			RGBQUAD kRGBQuad = cimage.GetPixelColor( iX, iY );
			if( kRGBQuad.rgbBlue == 0xff &&
				kRGBQuad.rgbGreen == 0x00 &&
				kRGBQuad.rgbRed == 0xff )
			{
				//cimage.SetPixelColor( iX, iY,  )
				cimage.AlphaSet( iX, iY, 0x00 );
			}
		}
	}

	if( bSucceeded == false ) {
		ASSERT( !L"Fail to open ScreenCapture File" );
		return false;
	}
	else
	{
		cimage.Save( mstream.str().c_str(), CXIMAGE_FORMAT_PNG ); 
	}


	pBackBuffer->Release();

	return true;

}

// [120212 김석근], 추가구현예정 부분, TGA로 출력시키려고 하는데, 일단 PNG 작업부터 완료. 그리고, PNG가 더 좋다는 의견이 있었음.
bool CKTDGManager::ScreenCapture_Tga()
{
	m_bField_Character_Screen_shot	=	false;
	m_bField_NPC_Screen_shot		=	false;
	m_bField_MainCha_Screen_shot	=	false;
	m_bField_BG_Screen_shot			=	false;

	std::wstringstream stream;

	struct tm *newtime;
	__time64_t long_time;
	_time64( &long_time );                
	newtime = _localtime64( &long_time ); 

	IDirect3DSurface9* pBackBuffer = 0;
	g_pKTDXApp->GetDevice()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer ); 
	// mono왜의 테스트

	stream  << L"ScreenCapture\\SC_"
		<< ( 1900 + newtime->tm_year ) << L"_"
		<< (newtime->tm_mon+1)	<< L"_"
		<< newtime->tm_mday		<< L"_"
		<< newtime->tm_hour		<< L"_"
		<< newtime->tm_min		<< L"_"
		<< newtime->tm_sec		<< L"_"
		<< L".tga";
	pBackBuffer->UnlockRect();
	//L_cBuf
	std::stringstream mstream;
	mstream  << "ScreenCapture\\SC_"
		<< ( 1900 + newtime->tm_year ) << "_"
		<< (newtime->tm_mon+1)	<< "_"
		<< newtime->tm_mday		<< "_"
		<< newtime->tm_hour		<< "_"
		<< newtime->tm_min		<< "_"
		<< newtime->tm_sec		<< "_"
		<< ".tga"; 

	D3DXSaveSurfaceToFile( stream.str().c_str(), D3DXIFF_TGA, pBackBuffer, NULL, NULL ); 
	// 파일에 표면모습을 저장

	CxImage cimage;
	bool bSucceeded = cimage.Load( mstream.str().c_str(),  7); //CXIMAGE_FORMAT_TGA

	TEXTURE_DATA * L_buf=(TEXTURE_DATA *)cimage.GetBits();//어드레스 기억
	long sizeX=cimage.GetWidth(),sizeY=cimage.GetHeight();//사이즈 할당
	//int size=sizeX*sizeY;//실사이즈 구하고

	for( int iX = 0; iX < sizeX; ++iX )
	{
		for( int iY = 0; iY < sizeY; ++iY )
		{
			RGBQUAD kRGBQuad = cimage.GetPixelColor( iX, iY );
			if( kRGBQuad.rgbBlue == 0xff &&
				kRGBQuad.rgbGreen == 0x00 &&
				kRGBQuad.rgbRed == 0xff )
			{
				//cimage.SetPixelColor( iX, iY,  )
				cimage.AlphaSet( iX, iY, 0x00 );
			}
		}
	}

	RGBQUAD bkg = { 0xff, 0xff, 0xff, 0xff };//칼라키를 설정한다
	cimage.AlphaPaletteEnable(true);//알파값 enable설정
	cimage.SetTransIndex(0);
	cimage.SetTransColor(bkg);
	cimage.AlphaStrip();

	if( bSucceeded == false ) {
		ASSERT( !L"Fail to open ScreenCapture File" );
		return false;
	}
	else
	{
		cimage.Save( mstream.str().c_str(), 7 ); 
	}


	pBackBuffer->Release();

	return true;

}
#endif

void CKTDGManager::ScreenCapture( const WCHAR* pFileName )
{
#ifndef TEST_SCREEN_CAPTURE
	IDirect3DSurface9* pBackBuffer = 0;
	g_pKTDXApp->GetDevice()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

	D3DXSaveSurfaceToFile( pFileName, D3DXIFF_JPG, pBackBuffer, NULL, NULL );
	pBackBuffer->Release();
#else
	//{{ 09-03-04.hoons. 좀도 빠른 캡쳐를 위해
	//DWORD dwTime = ::GetTickCount();

	HBITMAP	hBitmap; 
	RECT	rect; 
	HDC		hMemDC;
	HDC		hScrDC; 
	HBITMAP	hOldBitmap; //DC를 얻는다. 

	IDirect3DSurface9* pBackBuffer = 0;
	g_pKTDXApp->GetDevice()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	HRESULT hResultDC = pBackBuffer->GetDC(&hScrDC);

	HWND	pDesktop = g_pKTDXApp->GetHWND(); // 내자신의 윈도우의 좌표를 조사한다.

	if( D3D_OK != hResultDC )
	{
		pBackBuffer->Release();

		hScrDC = GetDC( pDesktop );
	}

	GetClientRect( pDesktop, &rect ); // 윈도우 화면 DC와 메모리 DC를 만들고 메모리 비트맵을 선택한다. 

	CreateDC( L"DISPLAY", NULL, NULL, NULL ); 
	hMemDC	= CreateCompatibleDC( hScrDC ); 
	hBitmap	= CreateCompatibleBitmap( hScrDC, rect.right - rect.left, rect.bottom - rect.top ); 
	hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap ); // 화면을 메모리 비트맵으로 복사한다. 
	BitBlt( hMemDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hScrDC, rect.left, rect.top, SRCCOPY ); 
	SelectObject( hMemDC, hOldBitmap ); 

	CxImage cimage( CXIMAGE_FORMAT_JPG ); 

	if( cimage.CreateFromHBITMAP( hBitmap ) ) 
	{ 
		cimage.SetJpegQuality( 70 );

		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_ACP, 0, pFileName, -1, charBuf, 255, NULL, NULL );

		cimage.Save( charBuf, CXIMAGE_FORMAT_JPG ); 
	} 

	DeleteDC( hMemDC ); 
	//DeleteDC( hScrDC ); 
	DeleteObject( hBitmap ); 
	DeleteObject( hOldBitmap );

	if( D3D_OK != hResultDC )
	{
		DeleteDC( hScrDC ); 
	}
	else
	{
		pBackBuffer->ReleaseDC(hScrDC);
		pBackBuffer->Release();
	}
#endif TEST_SCREEN_CAPTURE
}


//{{ robobeg : 2008-10-17

bool CKTDGManager::IsInFrustum( CKTDGObject* object )
{
	if ( object == NULL )
		return false;

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
	return object->IsInFrustum( GetCamera().GetEye(), m_kFrustum );
#else//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
	if( object->GetShowObject() == false )
		return false;


    D3DXVECTOR3 center;



	object->GetTransformCenter( &center );
	object->SetDistanceToCamera( GetDistance( GetCamera().GetEye(), center ) );

    if ( object->GetBoundingRadius() <= 0 )
        return true;

	//컬링
#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
    float fScaledBoundingRadius = object->GetScaledBoundingRadius();
#else   X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

    float fScale = 0.f;
	if( object->GetMatrix().GetXScale() > object->GetMatrix().GetYScale() )
	{
		if( object->GetMatrix().GetXScale() > object->GetMatrix().GetZScale() )
		{
			//X가 제일 큼
			fScale = object->GetMatrix().GetXScale();
		}
		else
		{
			//Z가 제일 큼
			fScale = object->GetMatrix().GetZScale();
		}
	}
	else
	{
		if( object->GetMatrix().GetYScale() > object->GetMatrix().GetZScale() )
		{
			//Y가 제일 큼
			fScale = object->GetMatrix().GetYScale();
		}
		else
		{
			//Z가 제일 큼
			fScale = object->GetMatrix().GetZScale();
		}
	}
     float fScaledBoundingRadius = object->GetBoundingRadius() * fScale;
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

	if( m_kFrustum.CheckSphere( center, fScaledBoundingRadius ) == false )
	{
		//...... -_-a I want to use this code but............ T^T
		object->SetIsCulled( true );
		return false;
	}
	else
	{
		object->SetIsCulled( false );
	}
    return true;
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
}

bool CKTDGManager::IsInFrustum( CKTDGObject* object, float& fFrameMoveUpdateTime )
{
	if ( object == NULL )
		return false;

	if( object->GetShowObject() == false )
		return false;


    D3DXVECTOR3 center;


	object->GetTransformCenter( &center );
	object->SetDistanceToCamera( GetDistance( GetCamera().GetEye(), center ) );

    if ( object->GetBoundingRadius() <= 0 )
    {
	    fFrameMoveUpdateTime = 0.0f;
        return true;
    }//if

	//컬링
#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
    float   fScale = object->GetMaxScale();
#else   X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
    float fScale = 0.f;
	if( object->GetMatrix().GetXScale() > object->GetMatrix().GetYScale() )
	{
		if( object->GetMatrix().GetXScale() > object->GetMatrix().GetZScale() )
		{
			//X가 제일 큼
			fScale = object->GetMatrix().GetXScale();
		}
		else
		{
			//Z가 제일 큼
			fScale = object->GetMatrix().GetZScale();
		}
	}
	else
	{
		if( object->GetMatrix().GetYScale() > object->GetMatrix().GetZScale() )
		{
			//Y가 제일 큼
			fScale = object->GetMatrix().GetYScale();
		}
		else
		{
			//Z가 제일 큼
			fScale = object->GetMatrix().GetZScale();
		}
	}
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
    float fScaledBoundingRadius = object->GetBoundingRadius() * fScale;

	float fNeedDistance = 0.0f;
	if( m_kFrustum.CheckSphere( center, fScaledBoundingRadius, fNeedDistance ) == false )
	{
		//...... -_-a I want to use this code but............ T^T
		object->SetIsCulled( true );

		float fFrameMoveUpdateTime = 1/g_pKTDXApp->GetFrameMoveFPS(); //max frame rate!!
		int distanceTimes = (int)(fNeedDistance / object->GetBoundingRadius() * fScale); // 몇배일까?

		//최고 40배까지
		if ( distanceTimes >= 40 )
		{
			fFrameMoveUpdateTime = 999999.0f;
		}
		else if ( distanceTimes > 0 )
		{
			fFrameMoveUpdateTime = fFrameMoveUpdateTime * ( 1.0f + ( distanceTimes * 0.6f ) );
		}
		return false;
	}
	else
	{
		object->SetIsCulled( false );
	}

	fFrameMoveUpdateTime = 0.0f;
	return true;
}

//}} robobeg : 2008-10-17


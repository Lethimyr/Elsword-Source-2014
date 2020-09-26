/** @file   CKTDGStateManager.h
	@desc   interface of CKTDGStateManager
	@author robobeg@kogstudios.com
    @since  2003-1-1

	@date   seojt on 2005-8-26, 11:26:53
            - example comment added
*/

#pragma once

#pragma warning(push)
#pragma warning(disable: 4189 4100 4003 )

#define DEVICE_STATE_STACK_SIZE     512


#define D3DGENERALIZE(interf,pointer,ver) \
    typedef interf##ver interf; \
    typedef struct interf##ver  *LP##pointer, *P##pointer;

#ifdef _XBOX

#define TEXTURE_STAGE_NUM   4
#define MAX_TEXTURE_NUM     TEXTURE_STAGE_NUM

D3DGENERALIZE( IDirect3D, DIRECT3D, 8 )
D3DGENERALIZE( IDirect3DBaseTexture, DIRECT3DBASETEXTURE, 8 )
D3DGENERALIZE( IDirect3DCubeTexture, DIRECT3DCUBETEXTURE, 8 )
D3DGENERALIZE( IDirect3DDevice, DIRECT3DDEVICE, 8 )
D3DGENERALIZE( IDirect3DIndexBuffer, DIRECT3DINDEXBUFFER, 8 )
D3DGENERALIZE( IDirect3DPushBuffer, DIRECT3DPUSHBUFFER, 8 )
D3DGENERALIZE( IDirect3DResource, DIRECT3DRESOURCE, 8 )
D3DGENERALIZE( IDirect3DSurface, DIRECT3DSURFACE, 8 )
D3DGENERALIZE( IDirect3DTexture, DIRECT3DTEXTURE, 8 )
D3DGENERALIZE( IDirect3DVertexBuffer, DIRECT3DVERTEXBUFFER, 8 )
D3DGENERALIZE( IDirect3DVolume, DIRECT3DVOLUME, 8 )
D3DGENERALIZE( IDirect3DVolumeTexture, DIRECT3DVOLUMETEXTURE, 8 )
typedef D3DADAPTER_IDENTIFIER8  D3DADAPTER_IDENTIFIER;
typedef D3DCAPS8                D3DCAPS;
typedef D3DLIGHT8               D3DLIGHT;
typedef D3DMATERIAL8            D3DMATERIAL;
typedef D3DVIEWPORT8            D3DVIEWPORT;

#else

#define TEXTURE_STAGE_NUM   8

#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

#define MAX_TEXTURE_NUM     TEXTURE_STAGE_NUM

#define D3DRS_MAX           (D3DRS_NORMALORDER+1)
#define D3DTSS_MAX          (D3DTSS_RESULTARG+1)

D3DGENERALIZE( IDirect3D, DIRECT3D, 8 )
D3DGENERALIZE( IDirect3DBaseTexture, DIRECT3DBASETEXTURE, 8 )
D3DGENERALIZE( IDirect3DCubeTexture, DIRECT3DCUBETEXTURE, 8 )
D3DGENERALIZE( IDirect3DDevice, DIRECT3DDEVICE, 8 )
D3DGENERALIZE( IDirect3DIndexBuffer, DIRECT3DINDEXBUFFER, 8 )
D3DGENERALIZE( IDirect3DResource, DIRECT3DRESOURCE, 8 )
D3DGENERALIZE( IDirect3DSurface, DIRECT3DSURFACE, 8 )
D3DGENERALIZE( IDirect3DSwapChain, DIRECT3DSWAPCHAIN, 8 )
D3DGENERALIZE( IDirect3DTexture, DIRECT3DTEXTURE, 8 )
D3DGENERALIZE( IDirect3DVertexBuffer, DIRECT3DVERTEXBUFFER, 8 )
D3DGENERALIZE( IDirect3DVolume, DIRECT3DVOLUME, 8 )
D3DGENERALIZE( IDirect3DVolumeTexture, DIRECT3DVOLUMETEXTURE, 8 )
typedef D3DADAPTER_IDENTIFIER8  D3DADAPTER_IDENTIFIER;
typedef D3DCAPS8                D3DCAPS;
typedef D3DCLIPSTATUS8          D3DCLIPSTATUS;
typedef D3DLIGHT8               D3DLIGHT;
typedef D3DMATERIAL8            D3DMATERIAL;
typedef D3DVIEWPORT8            D3DVIEWPORT;

#elif ( DIRECT3D_VERSION >= 0x0900 )

#define SAMPLER_STAGE_NUM   16

#define MAX_TEXTURE_NUM     max(TEXTURE_STAGE_NUM,SAMPLER_STAGE_NUM)

#define D3DRS_MAX           (D3DRS_BLENDOPALPHA+1)
#define D3DTSS_MAX          (D3DTSS_CONSTANT+1)
#define D3DSSS_MAX          (D3DSAMP_DMAPOFFSET+1)

D3DGENERALIZE( IDirect3D, DIRECT3D, 9 )
D3DGENERALIZE( IDirect3DBaseTexture, DIRECT3DBASETEXTURE, 9 )
D3DGENERALIZE( IDirect3DCubeTexture, DIRECT3DCUBETEXTURE, 9 )
D3DGENERALIZE( IDirect3DDevice, DIRECT3DDEVICE, 9 )
D3DGENERALIZE( IDirect3DIndexBuffer, DIRECT3DINDEXBUFFER, 9 )
D3DGENERALIZE( IDirect3DPixelShader, DIRECT3DPIXELSHADER, 9 )
D3DGENERALIZE( IDirect3DQuery, DIRECT3DQUERY, 9 )
D3DGENERALIZE( IDirect3DResource, DIRECT3DRESOURCE, 9 )
D3DGENERALIZE( IDirect3DStateBlock, DIRECT3DSTATEBLOCK, 9 )
D3DGENERALIZE( IDirect3DSurface, DIRECT3DSURFACE, 9 )
D3DGENERALIZE( IDirect3DSwapChain, DIRECT3DSWAPCHAIN, 9 )
D3DGENERALIZE( IDirect3DTexture, DIRECT3DTEXTURE, 9 )
D3DGENERALIZE( IDirect3DVertexBuffer, DIRECT3DVERTEXBUFFER, 9 )
D3DGENERALIZE( IDirect3DVertexDeclaration, DIRECT3DVERTEXDECLARATION, 9 )
D3DGENERALIZE( IDirect3DVertexShader, DIRECT3DVERTEXSHADER, 9 )
D3DGENERALIZE( IDirect3DVolume, DIRECT3DVOLUME, 9 )
D3DGENERALIZE( IDirect3DVolumeTexture, DIRECT3DVOLUMETEXTURE, 9 )
typedef D3DADAPTER_IDENTIFIER9  D3DADAPTER_IDENTIFIER;
typedef D3DCAPS9                D3DCAPS;
typedef D3DCLIPSTATUS9          D3DCLIPSTATUS;
typedef D3DLIGHT9               D3DLIGHT;
typedef D3DMATERIAL9            D3DMATERIAL;
typedef D3DVIEWPORT9            D3DVIEWPORT;

#endif // #ifdef ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

#endif // #ifdef _XBOX

struct  KRenderState
{
    D3DRENDERSTATETYPE m_eType;
    DWORD   m_dwValue;

    KRenderState( D3DRENDERSTATETYPE Type, DWORD Value )
        : m_eType( Type ), m_dwValue( Value ) {}
};//struct  KRenderState

struct  KTextureStageState
{
    DWORD   m_dwStage;
    D3DTEXTURESTAGESTATETYPE m_eType;
    DWORD   m_dwValue;

    KTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value )
        : m_dwStage(Stage), m_eType(Type), m_dwValue(Value) {}
};//struct  KTextureStageState

#ifdef SAMPLER_STAGE_NUM
struct  KSamplerState
{
    DWORD   m_dwStage;
    D3DSAMPLERSTATETYPE m_eType;
    DWORD   m_dwValue;

    KSamplerState( DWORD Stage, D3DSAMPLERSTATETYPE Type, DWORD Value )
        : m_dwStage(Stage), m_eType(Type), m_dwValue( Value ) {}
};//struct  KSamplerState
#endif // #ifdef SAMPLER_STAGE_NUM


class   CKTDGStateManager
{
	public:

    enum    EStateType
    {
        STATE_TYPE_INVALID = -1,
        STATE_TYPE_RENDER = 0,
        STATE_TYPE_TEXTURE_STAGE = 1,
        STATE_TYPE_SAMPLER = 2,
    };//enum    EStackType

    struct  KState
    {
        EStateType  m_eType;
        DWORD       m_dwStage;
        DWORD       m_dwType;
        DWORD       m_dwValue;

        KState()
            : m_eType( STATE_TYPE_INVALID ), m_dwStage( 0 ), m_dwType( 0 ), m_dwValue( 0 ) {}
        KState( const KRenderState& kIn_ )
            : m_eType( STATE_TYPE_RENDER ), m_dwStage( 0), m_dwType( kIn_.m_eType ), m_dwValue( kIn_.m_dwValue ) {}
        KState& operator = ( const KRenderState& kIn_ )
        {
            m_eType = STATE_TYPE_RENDER;
            m_dwStage = 0;
            m_dwType = kIn_.m_eType;
            m_dwValue = kIn_.m_dwValue;
            return *this;
        }//operator = ()
        KState( const KTextureStageState& kIn_ )
            : m_eType( STATE_TYPE_TEXTURE_STAGE ), m_dwStage( kIn_.m_dwStage ), m_dwType( kIn_.m_eType ), m_dwValue( kIn_.m_dwValue ) {}
        KState& operator = ( const KTextureStageState& kIn_ )
        {
            m_eType = STATE_TYPE_TEXTURE_STAGE;
            m_dwStage = kIn_.m_dwStage;
            m_dwType = kIn_.m_eType;
            m_dwValue = kIn_.m_dwValue;
            return *this;
        }//operator = ()
#ifdef SAMPLER_STAGE_NUM
        KState( const KSamplerState& kIn_ )
            : m_eType( STATE_TYPE_SAMPLER ), m_dwStage( kIn_.m_dwStage ), m_dwType( kIn_.m_eType ), m_dwValue( kIn_.m_dwValue ) {}
        KState& operator = ( const KSamplerState& kIn_ )
        {
            m_eType = STATE_TYPE_SAMPLER;
            m_dwStage = kIn_.m_dwStage;
            m_dwType = kIn_.m_eType;
            m_dwValue = kIn_.m_dwValue;
            return *this;
        }//operator = ()
#endif // #ifdef SAMPLER_STAGE_NUM

    };//struct  KState


    struct  KStateID
    {
        const KState*   m_pkStates;
        unsigned        m_uNumStates;

        KStateID() : m_pkStates( NULL ), m_uNumStates( 0 ) {}

        template<size_t n>
        KStateID( const KState (&state)[n] )
            : m_pkStates( &state[0] ), m_uNumStates( n ) {}

        bool operator == ( const KStateID& kIn_ ) const { return m_pkStates == kIn_.m_pkStates && m_uNumStates == kIn_.m_uNumStates; }
        bool operator != ( const KStateID& kIn_ ) const { return m_pkStates != kIn_.m_pkStates || m_uNumStates != kIn_.m_uNumStates; }

        template<size_t n>
        KStateID& operator = ( const KState (&state)[n] )
        {
            m_pkStates = &state[0];
            m_uNumStates = n;
            return *this;
        }//

    };//struct  KStateID


private:

    static UINT                         ms_uiAdapter;
    static D3DDEVTYPE                   ms_d3dDeviceType;


    static BOOL                         ms_bRestored;

    static DWORD                        ms_adwRenderState[D3DRS_MAX];

    static DWORD                       ms_adwTextureStageState[TEXTURE_STAGE_NUM][D3DTSS_MAX];

#ifdef SAMPLER_STAGE_NUM

    static DWORD                       ms_adwSamplerState[SAMPLER_STAGE_NUM][D3DSSS_MAX];

#endif // #ifdef SAMPLER_STAGE_NUM

    static KState                       ms_akStack[DEVICE_STATE_STACK_SIZE];
    static int                         ms_iStackPointer;

    static LPDIRECT3DBASETEXTURE9       ms_apd3dTextures[MAX_TEXTURE_NUM];


    //static D3DVIEWPORT                ms_d3dViewport;
   
    static LPDIRECT3DSURFACE          ms_pd3dBackBuffer, ms_pd3dBackBufferZStencil;
    static D3DSURFACE_DESC            ms_d3dBackBufferDesc, ms_d3dBackBufferZStencilDesc;
    static D3DVIEWPORT                ms_d3dBackBufferViewport;
    static LPDIRECT3DSURFACE          ms_pd3dRenderTarget, ms_pd3dRenderTargetZStencil;
    static D3DVIEWPORT                ms_d3dRenderTargetViewport;

    static D3DDISPLAYMODE               ms_d3dDisplayMode;

    static D3DFORMAT                    ms_d3dColorKeyTextureFormat;
    static D3DFORMAT                    ms_d3dAlphaTextureFormat;



private:

    static HRESULT  _SetPossibleColorKeyTextureFormat();
    static HRESULT  _SetPossibleAlphaTextureFormat();

    static __forceinline KState&   _GetStackTop() 
    { 
        ASSERT( ms_iStackPointer < DEVICE_STATE_STACK_SIZE );
        return ms_akStack[ ms_iStackPointer ];
    }//


public:


    static void            InvalidateDeviceObjects();
    static void            RestoreDeviceObjects(
        UINT uiAdapter, D3DDEVTYPE d3dDeviceType);

    static __forceinline void   UpdateRenderState( D3DRENDERSTATETYPE Type )
    {
        extern LPDIRECT3DDEVICE9        g_pd3dDevice;
        ASSERT( Type >= D3DRENDERSTATETYPE(0) && Type < D3DRS_MAX );

        HRESULT hr = g_pd3dDevice->SetRenderState( Type, ms_adwRenderState[Type] );
        ASSERT( SUCCEEDED( hr ) );
    }//UpdateRenderState()

    static __forceinline HRESULT SetRenderState(D3DRENDERSTATETYPE Type, DWORD Value)
    {
        extern LPDIRECT3DDEVICE9        g_pd3dDevice;
        ASSERT( Type >= D3DRENDERSTATETYPE(0) && Type < D3DRS_MAX );

        if ( ms_adwRenderState[Type] != Value )
        {
            HRESULT hr = g_pd3dDevice->SetRenderState( Type, Value );
            ASSERT( SUCCEEDED( hr ) );
            ms_adwRenderState[Type] = Value;
            return hr;
        }//if

        return S_OK;
    }//SetRenderState()


    static __forceinline  void  PushRenderState( D3DRENDERSTATETYPE Type )
    {
        ASSERT( Type >= D3DRENDERSTATETYPE(0) && Type < D3DRS_MAX );        
        ASSERT( ms_iStackPointer < DEVICE_STATE_STACK_SIZE );
        ms_akStack[ ms_iStackPointer++ ] = KRenderState( Type, ms_adwRenderState[Type] );
    }//PushRenderState()

    static __forceinline  void    PushRenderState(D3DRENDERSTATETYPE Type, DWORD Value)
    {
        PushRenderState( Type );
        SetRenderState( Type, Value );
    }//PushRenderState()

    static __forceinline void     PopRenderState(D3DRENDERSTATETYPE Type)
    {
        ASSERT( ms_iStackPointer > 0 );
        ms_iStackPointer--;
        const KState& kTop = _GetStackTop();
        ASSERT( kTop.m_eType == STATE_TYPE_RENDER && (D3DRENDERSTATETYPE) kTop.m_dwType == Type );
        SetRenderState( (D3DRENDERSTATETYPE) kTop.m_dwType, kTop.m_dwValue );
    }//PopRenderState()

    static __forceinline void    GetRenderState(D3DRENDERSTATETYPE Type, DWORD*  pValue)
    {
        ASSERT( Type >= D3DRENDERSTATETYPE(0) && Type < D3DRS_MAX );
        ASSERT( pValue != NULL );
        *pValue = ms_adwRenderState[Type];
    }//GetRenderState()
    static __forceinline DWORD    GetRenderState(D3DRENDERSTATETYPE Type)
    {
        ASSERT( Type >= D3DRENDERSTATETYPE(0) && Type < D3DRS_MAX );
        return  ms_adwRenderState[Type];
    }//GetRenderState()


    static __forceinline void     UpdateTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type)
    {
        extern LPDIRECT3DDEVICE9        g_pd3dDevice;
        ASSERT( Stage < TEXTURE_STAGE_NUM );
        ASSERT( Type >= D3DTEXTURESTAGESTATETYPE(0) && Type < D3DTSS_MAX );
        HRESULT hr = g_pd3dDevice->SetTextureStageState( Stage, Type, ms_adwTextureStageState[Stage][Type] );
        ASSERT( SUCCEEDED( hr ) );
    }//SetTextureStageState()

    static __forceinline void     SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
    {
        extern LPDIRECT3DDEVICE9        g_pd3dDevice;
        ASSERT( Stage < TEXTURE_STAGE_NUM );
        ASSERT( Type >= D3DTEXTURESTAGESTATETYPE(0) && Type < D3DTSS_MAX );
        if ( ms_adwTextureStageState[Stage][Type] != Value )
        {
            HRESULT hr = g_pd3dDevice->SetTextureStageState( Stage, Type, Value );
            ASSERT( SUCCEEDED( hr ) );
            ms_adwTextureStageState[Stage][Type] = Value;
        }//if
    }//SetTextureStageState()

    static __forceinline void   GetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD*  pValue )
    {
        ASSERT( Stage < TEXTURE_STAGE_NUM );
        ASSERT( Type >= D3DTEXTURESTAGESTATETYPE(0) && Type < D3DTSS_MAX );
        ASSERT( pValue != NULL );
        *pValue = ms_adwTextureStageState[Stage][Type];
    }//GetTextureStageState()

    static __forceinline DWORD    GetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type )
    {
        ASSERT( Stage < TEXTURE_STAGE_NUM );
        ASSERT( Type >= D3DTEXTURESTAGESTATETYPE(0) && Type < D3DTSS_MAX );
        return  ms_adwTextureStageState[Stage][Type];
    }//GetTextureStageState()

    static __forceinline  void    PushTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type )
    {
        ASSERT( Stage < TEXTURE_STAGE_NUM );
        ASSERT( Type >= D3DTEXTURESTAGESTATETYPE(0) && Type < D3DTSS_MAX );

        ASSERT( ms_iStackPointer < DEVICE_STATE_STACK_SIZE );
        ms_akStack[ ms_iStackPointer++ ] = KTextureStageState( Stage, Type, ms_adwTextureStageState[Stage][Type] );
    }//PushTextureStageState()

    static __forceinline  void    PushTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
    {
        PushTextureStageState( Stage, Type );
        SetTextureStageState( Stage, Type, Value );
    }//PushTextureStageState()

    //static __forceinline void     PushTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, bool bInValue_ )
    //{
    //    PushTextureStageState( Stage, Type, (DWORD)( (bInValue_) ? TRUE : FALSE ) );
    //}//

    static __forceinline void     PopTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type)
    {
        ASSERT( Stage < TEXTURE_STAGE_NUM );
        ASSERT( Type >= D3DTEXTURESTAGESTATETYPE(0) && Type < D3DTSS_MAX );

        ASSERT( ms_iStackPointer > 0 );
        ms_iStackPointer--;
        const KState& kTop = _GetStackTop();
        ASSERT( kTop.m_eType == STATE_TYPE_TEXTURE_STAGE && kTop.m_dwStage == Stage && (D3DTEXTURESTAGESTATETYPE) kTop.m_dwType == Type );
        
        SetTextureStageState( kTop.m_dwStage, (D3DTEXTURESTAGESTATETYPE) kTop.m_dwType, kTop.m_dwValue );
    }//PopRenderState()
  
#ifdef SAMPLER_STAGE_NUM

    static __forceinline void     UpdateSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type)
    {
        extern LPDIRECT3DDEVICE9        g_pd3dDevice;
        ASSERT( Stage < SAMPLER_STAGE_NUM );
        ASSERT( Type >= D3DSAMPLERSTATETYPE(0) && Type < D3DSSS_MAX );
        HRESULT hr = g_pd3dDevice->SetSamplerState( Stage, Type, ms_adwSamplerState[Stage][Type] );
        ASSERT( SUCCEEDED( hr ) );
    }//UpdateSamplerState()

    static __forceinline void     SetSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type, DWORD Value)
    {
        extern LPDIRECT3DDEVICE9        g_pd3dDevice;
        ASSERT( Stage < SAMPLER_STAGE_NUM );
        ASSERT( Type >= D3DSAMPLERSTATETYPE(0) && Type < D3DSSS_MAX );
        if ( ms_adwSamplerState[Stage][Type] != Value )
        {
            HRESULT hr = g_pd3dDevice->SetSamplerState( Stage, Type, Value );
            ASSERT( SUCCEEDED( hr ) );
            ms_adwSamplerState[Stage][Type] = Value;
        }//if
    }//SetSamplerState()


    static __forceinline void   GetSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type, DWORD*  pValue )
    {
        ASSERT( Stage < SAMPLER_STAGE_NUM );
        ASSERT( Type >= D3DSAMPLERSTATETYPE(0) && Type < D3DSSS_MAX );
        ASSERT( pValue != NULL );
        *pValue = ms_adwSamplerState[Stage][Type];
    }//GetSamplerState()


    static __forceinline DWORD  GetSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type)
    {
        ASSERT( Stage < SAMPLER_STAGE_NUM );
        ASSERT( Type >= D3DSAMPLERSTATETYPE(0) && Type < D3DSSS_MAX );
        return  ms_adwSamplerState[Stage][Type];
    }//GetSamplerState()


    static __forceinline  void    PushSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type )
    {
        ASSERT( Stage < SAMPLER_STAGE_NUM );
        ASSERT( Type >= D3DSAMPLERSTATETYPE(0) && Type < D3DTSS_MAX );

        ASSERT( ms_iStackPointer < DEVICE_STATE_STACK_SIZE );
        ms_akStack[ ms_iStackPointer++ ] = KSamplerState( Stage, Type, ms_adwSamplerState[Stage][Type] );

    }//PushSamplerState()

    static __forceinline  void    PushSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type, DWORD Value)
    {
        PushSamplerState( Stage, Type );
        SetSamplerState(Stage, Type, Value);
    }//PushSamplerState()

    //static __forceinline void     PushSamplerState( DWORD Stage, D3DSAMPLERSTATETYPE Type, bool bInValue_ )
    //{
    //    PushSamplerState( Stage, Type, (DWORD)( (bInValue_) ? TRUE : FALSE ) );
    //}//

    static __forceinline void     PopSamplerState(DWORD Stage, D3DSAMPLERSTATETYPE Type)
    {
        ASSERT( Stage < SAMPLER_STAGE_NUM );
        ASSERT( Type >= D3DSAMPLERSTATETYPE(0) && Type < D3DTSS_MAX );

        ASSERT( ms_iStackPointer > 0 );
        ms_iStackPointer--;
        const KState& kTop = _GetStackTop();
        ASSERT( kTop.m_eType == STATE_TYPE_SAMPLER && kTop.m_dwStage == Stage && (D3DSAMPLERSTATETYPE) kTop.m_dwType == Type );
        
        SetSamplerState( kTop.m_dwStage, (D3DSAMPLERSTATETYPE) kTop.m_dwType, kTop.m_dwValue );
    }//PopRenderState()

#endif // #ifdef SAMPLER_STAGE_NUM


    static __forceinline void     SetState( const KState& kInState_ )
    {
        switch( kInState_.m_eType )
        {
        case STATE_TYPE_RENDER:
            SetRenderState( (D3DRENDERSTATETYPE) kInState_.m_dwType, kInState_.m_dwValue );
            break;
        case STATE_TYPE_TEXTURE_STAGE:
            SetTextureStageState( kInState_.m_dwStage, (D3DTEXTURESTAGESTATETYPE) kInState_.m_dwType, kInState_.m_dwValue );
            break;
#ifdef SAMPLER_STAGE_NUM
        case STATE_TYPE_SAMPLER:
            SetSamplerState( kInState_.m_dwStage, (D3DSAMPLERSTATETYPE) kInState_.m_dwType, kInState_.m_dwValue );
            break;
#endif // #ifdef SAMPLER_STAGE_NUM
        default:
            ASSERT( !"invalid direct3d device state type" );
        }//switch
    }//SetState()

    static __forceinline void   PushState( const KState& kInState_ )
    {
        switch( kInState_.m_eType )
        {
        case STATE_TYPE_RENDER:
            PushRenderState( D3DRENDERSTATETYPE(kInState_.m_dwType), kInState_.m_dwValue );
            break;
        case STATE_TYPE_TEXTURE_STAGE:
            PushTextureStageState( kInState_.m_dwStage, D3DTEXTURESTAGESTATETYPE( kInState_.m_dwType ), kInState_.m_dwValue );
            break;
#ifdef SAMPLER_STAGE_NUM
        case STATE_TYPE_SAMPLER:
            PushSamplerState( kInState_.m_dwStage, D3DSAMPLERSTATETYPE( kInState_.m_dwType ), kInState_.m_dwValue );
            break;
#endif // #ifdef SAMPLER_STAGE_NUM
        default:
            ASSERT( !"invalid direct3d device state type" );
        }//switch
    }//PushState()



    static __forceinline void   PopState()
    {
        ASSERT( ms_iStackPointer > 0 );
        ms_iStackPointer--;
        SetState( _GetStackTop() );
    }//PopState()


    static __forceinline int    GetStackTop() { return ms_iStackPointer; }

    static __forceinline void   SetStates( const KState akInStateValues[], unsigned uInNumValues_ )
    {
        for( unsigned u = 0; u < uInNumValues_; u++ )
        {
            ASSERT( akInStateValues != NULL );
            SetState( akInStateValues[ u ] );
        }//if
    }//SetStates()

    static __forceinline int    PushStates( const KState akInStateValues[], unsigned uInNumValues_ )
    {
        int iOldStackTop = ms_iStackPointer;
        for( unsigned u = 0; u < uInNumValues_; u++ )
        {
            ASSERT( akInStateValues != NULL );
            PushState( akInStateValues[ u ] );
        }//if
        return iOldStackTop;
    }//PushStates()

    static __forceinline void   SetStates( const KStateID& kIn_ )
    {
        return SetStates( kIn_.m_pkStates, kIn_.m_uNumStates );
    }//PushStates()


    static __forceinline int    PushStates( const KStateID& kIn_ )
    {
        return PushStates( kIn_.m_pkStates, kIn_.m_uNumStates );
    }//PushStates()

    template<size_t n>
    static __forceinline void   SetStates( const KState (&state)[n] )
    {
        SetStates( state, n );
    }//PushStates()

    template<size_t n>
    static __forceinline int    PushStates( const KState (&state)[n] )
    {
        return PushStates( state, n );
    }//PushStates()


    static __forceinline void   PopStates( int iInOldStackTop_ )
    {
        while( ms_iStackPointer > iInOldStackTop_ )
            PopState();
    }//PopStates()


    static __forceinline HRESULT SetTexture( DWORD dwInIndex_, LPDIRECT3DBASETEXTURE9 pd3dTexture )
    {
        extern LPDIRECT3DDEVICE   g_pd3dDevice;
        if ( dwInIndex_ >= ARRAY_SIZE( ms_apd3dTextures ) )
            return g_pd3dDevice->SetTexture( dwInIndex_, pd3dTexture );
        if ( ms_apd3dTextures[ dwInIndex_ ] == pd3dTexture )
            return S_OK;
        HRESULT hr = g_pd3dDevice->SetTexture( dwInIndex_, pd3dTexture );

// 실패한 텍스쳐가 계속 시도되는 것을 방지하기 위해 device에는 NULL 로 세팅한 후 내부적으로는 세팅된 것으로 설정한다.
        if ( FAILED( hr ) )
        {
            g_pd3dDevice->SetTexture( dwInIndex_, NULL );
        }//if
        SAFE_RELEASE( ms_apd3dTextures[ dwInIndex_ ] );
        ms_apd3dTextures[ dwInIndex_ ] = pd3dTexture;
        if ( pd3dTexture != NULL )
            pd3dTexture->AddRef();
        return hr;
    }//SetTexture()


    static __forceinline void SetViewport(const D3DVIEWPORT *pViewport)
    {
        extern LPDIRECT3DDEVICE   g_pd3dDevice;
        HRESULT hr = g_pd3dDevice->SetViewport( pViewport );
        ASSERT( SUCCEEDED( hr ) );
        CopyMemory( &ms_d3dRenderTargetViewport, pViewport, sizeof(D3DVIEWPORT) );
    }//SetViewport()

    static __forceinline void     GetViewport(D3DVIEWPORT *pViewport)
    { 
        CopyMemory( pViewport, &ms_d3dRenderTargetViewport, sizeof(D3DVIEWPORT) );
    }//GetViewport()

    static __forceinline const D3DVIEWPORT* GetViewport() { return  &ms_d3dRenderTargetViewport; }
    static __forceinline const D3DVIEWPORT* GetBackBufferViewport() { return &ms_d3dBackBufferViewport; }

    static __forceinline void SetRenderTarget(LPDIRECT3DSURFACE pRenderTarget, LPDIRECT3DSURFACE pNewZStencil)
    {
        extern LPDIRECT3DDEVICE   g_pd3dDevice;

        //ASSERT( ms_pd3dRenderTarget == ms_pd3dBackBuffer );
        //ASSERT( ms_pd3dRenderTargetZStencil == ms_pd3dBackBufferZStencil );

        D3DSURFACE_DESC    desc;
        HRESULT hr = pRenderTarget->GetDesc( &desc );
        ASSERT( SUCCEEDED( hr ) );

#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

        hr = g_pd3dDevice->SetRenderTarget( pRenderTarget, pNewZStencil );
        ASSERT( SUCCEEDED( hr ) );

#elif ( DIRECT3D_VERSION >= 0x0900 )

        hr = g_pd3dDevice->SetRenderTarget( 0, pRenderTarget );
        ASSERT( SUCCEEDED( hr ) );
        hr = g_pd3dDevice->SetDepthStencilSurface( pNewZStencil );
        ASSERT( SUCCEEDED( hr ) );

#endif// #if.. #elif

        D3DVIEWPORT viewport;
        viewport.X = viewport.Y = 0;
        viewport.Width = desc.Width;
        viewport.Height = desc.Height;
        viewport.MinZ = 0.f;
        viewport.MaxZ = 1.f;
        SetViewport( &viewport );

        ms_pd3dRenderTarget = pRenderTarget;
        ms_pd3dRenderTargetZStencil = pNewZStencil;
    }//SetRenderTarget()

    static __forceinline void SetRenderTarget(LPDIRECT3DSURFACE pRenderTarget, LPDIRECT3DSURFACE pNewZStencil
        , const D3DSURFACE_DESC* pSurfaceDesc )
    {
        extern LPDIRECT3DDEVICE   g_pd3dDevice;

        //ASSERT( ms_pd3dRenderTarget == ms_pd3dBackBuffer );
        //ASSERT( ms_pd3dRenderTargetZStencil == ms_pd3dBackBufferZStencil );

        //CopyMemory( &ms_d3dViewportSaved, &ms_d3dViewport, sizeof(D3DVIEWPORT) );

        HRESULT hr;

#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

        hr = g_pd3dDevice->SetRenderTarget( pRenderTarget, pNewZStencil );
        ASSERT( SUCCEEDED( hr ) );

#elif ( DIRECT3D_VERSION >= 0x0900 )

        hr = g_pd3dDevice->SetRenderTarget( 0, pRenderTarget );
        ASSERT( SUCCEEDED( hr ) );
        hr = g_pd3dDevice->SetDepthStencilSurface( pNewZStencil );
        ASSERT( SUCCEEDED( hr ) );

#endif// #if.. #elif

        D3DVIEWPORT viewport;
        viewport.X = 0; viewport.Y = 0;
        viewport.Width = pSurfaceDesc->Width;
        viewport.Height = pSurfaceDesc->Height;
        viewport.MinZ = 0.f;
        viewport.MaxZ = 1.f;
        SetViewport( &viewport );

        ms_pd3dRenderTarget = pRenderTarget;
        ms_pd3dRenderTargetZStencil = pNewZStencil;
    }//SetRenderTarget()

    static __forceinline void RestoreRenderTarget()
    {
        extern LPDIRECT3DDEVICE   g_pd3dDevice;

        ms_pd3dRenderTarget = ms_pd3dBackBuffer;
        ms_pd3dRenderTargetZStencil = ms_pd3dBackBufferZStencil;

        HRESULT hr;

#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

        hr = g_pd3dDevice->SetRenderTarget( ms_pd3dRenderTarget, ms_pd3dRenderTargetZStencil );
        ASSERT( SUCCEEDED( hr ) );

#elif ( DIRECT3D_VERSION >= 0x0900 )

        hr = g_pd3dDevice->SetRenderTarget( 0, ms_pd3dRenderTarget );
        ASSERT( SUCCEEDED( hr ) );
        hr = g_pd3dDevice->SetDepthStencilSurface( ms_pd3dRenderTargetZStencil );
        ASSERT( SUCCEEDED( hr ) );

#endif// #if.. #elif

        SetViewport( &ms_d3dBackBufferViewport );

    }//RestoreRenderTarget()

    static __forceinline LPDIRECT3DSURFACE   GetRenderTarget()               { return ms_pd3dRenderTarget; }
    static __forceinline LPDIRECT3DSURFACE   GetZStencilSurface()            { return ms_pd3dRenderTargetZStencil; }
    static __forceinline LPDIRECT3DSURFACE   GetBackBuffer()                 { return ms_pd3dBackBuffer; }
    static __forceinline LPDIRECT3DSURFACE   GetBackBufferZStencil()         { return ms_pd3dBackBufferZStencil; }
    static __forceinline const D3DSURFACE_DESC&     GetBackBufferDesc()       { return ms_d3dBackBufferDesc; }
    static __forceinline const D3DSURFACE_DESC&     GetBackBufferZStencilDesc() { return ms_d3dBackBufferZStencilDesc; }

    static __forceinline const D3DDISPLAYMODE       GetDisplayMode()             { return ms_d3dDisplayMode; }

    static HRESULT CheckDeviceFormat( DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat );
    static HRESULT CheckDepthStencilMatch( D3DFORMAT BackBufferFormat, D3DFORMAT DepthFormat );

    static void ClearBackBuffer( D3DCOLOR d3dColor = 0xffffffff, float fZ = 1.0f, DWORD dwStencil = 0x00000000 );


    __forceinline static int  GetAlphaBits( D3DFORMAT d3dFormat )
    {
        switch( d3dFormat )
        {
        case D3DFMT_A1R5G5B5:
            return 1;
        case D3DFMT_A4R4G4B4:
            return 4;
        case D3DFMT_A8R3G3B2:
            return 8;
        case D3DFMT_A8R8G8B8:
            return 8;
        case D3DFMT_A2B10G10R10:
            return 2;
#if ( DIRECT3D_VERSION >= 0x0900 )
        case D3DFMT_A2R10G10B10:
            return 2;
        case D3DFMT_A16B16G16R16:
            return 1;
#endif // #if ( DIRECT3D_VERSION >= 0x0900 )
        }//switch
        return 0;
    }//

    __forceinline static int  GetFormatDepth( D3DFORMAT d3dFormat )
    {
        switch( d3dFormat )
        {
        case D3DFMT_R3G3B2:
            return 8;
        case D3DFMT_R5G6B5:
        case D3DFMT_X1R5G5B5:
        case D3DFMT_A1R5G5B5:
        case D3DFMT_X4R4G4B4:
        case D3DFMT_A4R4G4B4:
        case D3DFMT_A8R3G3B2:
            return 16;
        case D3DFMT_R8G8B8:
            return 24;
        case D3DFMT_X8R8G8B8:
        case D3DFMT_A8R8G8B8:
        case D3DFMT_A2B10G10R10:
            return 32;
#if ( DIRECT3D_VERSION >= 0x0900 )
        case D3DFMT_A2R10G10B10:
            return 32;
        case D3DFMT_A16B16G16R16:
            return 64;
#endif // #if ( DIRECT3D_VERSION >= 0x0900 )
        }//switch
        return 0;
    }//

    __forceinline static int    GetStencilDepth( D3DFORMAT d3dFormat )
    {
        switch( d3dFormat )
        {
        case D3DFMT_D15S1:
            return 1;
        case D3DFMT_D24X4S4:
            return 4;
        case D3DFMT_D24S8:
            return 8;
#if ( DIRECT3D_VERSION >= 0x0900 )
        case D3DFMT_D24FS8:
            return 8;
#endif // #if ( DIRECT3D_VERSION >= 0x0900 )
        }//switch
        return 0;
    }//

    __forceinline static D3DFORMAT GetColorKeyTextureFormat()  { return ms_d3dColorKeyTextureFormat; }
    /// get alpha texture surface format.
    __forceinline static D3DFORMAT GetAlphaTextureFormat()     { return ms_d3dAlphaTextureFormat; }


    __forceinline static HRESULT   SetFVF( DWORD dwFVF )
    {
        extern LPDIRECT3DDEVICE   g_pd3dDevice;

#if ( DIRECT3D_VERSION >= 0x0800 && DIRECT3D_VERSION < 0x0900 )

        return  g_pd3dDevice->SetVertexShader( dwFVF );

#elif ( DIRECT3D_VERSION >= 0x0900 )

        return  g_pd3dDevice->SetFVF( dwFVF );
#endif// #if.. #elif
        return E_FAIL;
    }// SetFVF()

};//class   CKTDGStateManager



class  KD3DStack
{
public:

    KD3DStack() : m_iStackTop( CKTDGStateManager::GetStackTop() ) {}

    template<size_t n>
    KD3DStack( const CKTDGStateManager::KState (&state)[n] )
      : m_iStackTop( CKTDGStateManager::PushStates( state, n ) ) {}

    KD3DStack( const CKTDGStateManager::KStateID& kInID_ )
      : m_iStackTop( CKTDGStateManager::PushStates( kInID_ ) ) {}

    ~KD3DStack() { CKTDGStateManager::PopStates( m_iStackTop ); }

private:

    KD3DStack( const KD3DStack& );
    KD3DStack& operator = ( const KD3DStack& );
    
private:

    int m_iStackTop;
};//class  _KD3DSTACK


#define KD3DBEGIN()                { KD3DStack _kD3DStack;

#define KD3DPUSH( kStateArray )    { KD3DStack _kD3DStack( kStateArray );

#define KD3DEND()      }


#pragma warning(pop)


/** @example    class CKTDGStateManager

  1) OnResetDevice():

    CKTDGStateManager::RestoreDeviceObjects( 0, D3DDEVTYPE_HAL );

    // Setup render state
    CKTDGStateManager::SetRenderState( D3DRS_LIGHTING, TRUE );
    CKTDGStateManager::SetRenderState( D3DRS_DITHERENABLE, TRUE );
    CKTDGStateManager::SetRenderState( D3DRS_ZENABLE, TRUE );
    CKTDGStateManager::SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
    CKTDGStateManager::SetRenderState( D3DRS_AMBIENT, 0x33333333 );
    CKTDGStateManager::SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
    CKTDGStateManager::SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    CKTDGStateManager::SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

  2) OnFrameRender():

    CKTDGStateManager::PushRenderState( D3DRS_LIGHTING, FALSE );
	g_stdMesh.DrawMesh();
	CKTDGStateManager::PopRenderState( D3DRS_LIGHTING );

	2) OnLostDevice():

	CKTDGStateManager::InvalidateDeviceObjects();

	*/


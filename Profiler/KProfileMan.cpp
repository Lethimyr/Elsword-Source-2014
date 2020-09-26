/** @file   KProfileMan.cpp
    @desc   implementation of KProfileMan
*/

#define _TOSTR1(x)    #x
#define _TOSTR(x)     _TOSTR1(x)
#include _TOSTR(PROJECT_PCH)
#include "KProfileMan.h"
#include "KMessageConsole.h"
//#include "KD3DGlobal.h"
#include "jprofile/KProfile.h"
//#include "KJohnD3DUtil.h"
//#include "KGlobalOption.h"
//#include "GameGlobal.h"

//{{ robobeg : 2008-10-21
#include "../KTDXLIB/KTDGStateManager.h"
//}} robobeg : 2008-10-21


#pragma warning(push)
#pragma warning(disable: 4995) // 'identifier': name was marked as #pragma deprecated


//extern LPDIRECT3DDEVICE9            g_pd3dDevice;

ID3DXFont*              g_pFont_Profiler		= NULL;		// Font for drawing text
ID3DXSprite*            g_pTextSprite_Profiler	= NULL;  // Sprite for batching draw text calls


//------------------------------------------------------------------------------
/// @struct     D3DBARVERTEX
///
#define D3DFVF_TLVERTEX     (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

typedef struct _D3DBARVERTEX
{
    union
    {
        struct
        {
            FLOAT   sx; /* Normal */
            FLOAT   sy;
            FLOAT   sz;
        };
        D3DVECTOR   p;
    };

    FLOAT       rhw;        /* Reciprocal of homogeneous w */
    D3DCOLOR    color;      /* Vertex color */

public:
    _D3DBARVERTEX() {}

    _D3DBARVERTEX(const D3DVECTOR& _v, D3DCOLOR _color)
    { 
        sx    = _v.x; 
        sy    = _v.y; 
        sz    = _v.z; 
        rhw   = 1.f;
        color = _color; 
    }//_D3DBARVERTEX()
} D3DBARVERTEX, *LPD3DBARVERTEX;

#define SET_TLVERTEX(v,_x,_y,_color)    v.sx=_x; v.sy=_y; v.sz=0; v.rhw=1;\
                                        v.color = _color;

//------------------------------------------------------------------------------
// static functions
//
static void John_Draw2DBar(IDirect3DDevice9* pd3dDevice,
                           float _fLeft, float _fTop,
                           float _fWidth, float fHeight, D3DCOLOR _dwColor)
{
    float   fRight  = _fLeft + _fWidth;
    float   fBottom = _fTop + fHeight;
    D3DBARVERTEX vertices[4];      ///< pre transform mesh

    // left,bottom
    SET_TLVERTEX( vertices[0], _fLeft, fBottom, _dwColor );
    // left,top
    SET_TLVERTEX( vertices[1], _fLeft, _fTop, _dwColor );
    // right,bottom
    SET_TLVERTEX( vertices[2], fRight, fBottom, _dwColor );
    // right,top
    SET_TLVERTEX( vertices[3], fRight, _fTop, _dwColor );

    // render quad mesh
    pd3dDevice->SetVertexShader( NULL );
    pd3dDevice->SetPixelShader( NULL );
//{{ robobeg : 2008-10-21
    CKTDGStateManager::PushRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    //pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
//    pd3dDevice->SetTexture( 0, NULL );
    CKTDGStateManager::SetTexture( 0, NULL );
    pd3dDevice->SetFVF( D3DFVF_TLVERTEX );
    pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(D3DBARVERTEX) );
    //pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    CKTDGStateManager::PopRenderState( D3DRS_ALPHABLENDENABLE );
//}} robobeg : 2008-10-21
}//John_Draw2DBar()


///-----------------------------------------------------------------------------
//
// constructor.
KProfileMan::KProfileMan()
{
    _Init();
}//KProfileMan::KProfileMan()


// destructor.
KProfileMan::~KProfileMan()
{
    SafeDelete();	
}//KProfileMan::~KProfileMan()


void KProfileMan::_Init()
{
    m_pitorRef  = NULL;
    m_iNumNodes = 0;
    m_iCursor   = 0;
    m_fx        = 16;   // display left
    m_fy        = 200;   // display top
    m_fUpdateInterval   = 1.f;
    m_fUpdateTime       = 0.f;

    m_fThisTime = 0.f;
    m_buf[0]    = NULL;
    m_fBarScaleFactor = 20000.f;

	m_bEnableProfile = false;
	m_bReservedClearRoot = false;
}//KProfileMan::_Init()


void KProfileMan::SafeDelete()
{
	if( NULL != m_pitorRefRoot )
		RHProfileManager::Release_Iterator( m_pitorRefRoot );

	m_pitorRefRoot = NULL;
	m_pitorRef = NULL;
}//KProfileMan::SafeDelete()


void KProfileMan::SetProfileIterator(RHProfileIterator* pitorRef_)
{
	SafeDelete();
	
	m_pitorRefRoot = pitorRef_;
    m_pitorRef = pitorRef_;
}//KProfileMan::SetProfileIterator()


void KProfileMan::Reset()
{
    RHProfileManager::ResetRoot();
}//KProfileMan::Reset()


void KProfileMan::OnPreFrameMove(float fElapsedTime)
{
	if( true == m_bReservedClearRoot )
	{
		m_bReservedClearRoot = false;
		
		m_fUpdateTime = 0.f;
		RHProfileManager::ms_bDoProfile = FALSE;

		RHProfileManager::ClearRoot();
		SetProfileIterator( NULL );
		SetProfileIterator( RHProfileManager::Get_Iterator() );
		return; 
	}


	if( m_bEnableProfile == false )
	{
		RHProfileManager::ms_bDoProfile = FALSE;
		return;
	}

    m_fUpdateTime += fElapsedTime;
    if ( m_fUpdateTime >= m_fUpdateInterval )
    {
        RHProfileManager::ResetRoot();
        RHProfileManager::Increment_Frame_Counter();
        RHProfileManager::ms_bDoProfile = TRUE;
        m_fUpdateTime = 0.f;
    }
    else
    {
        RHProfileManager::ms_bDoProfile = FALSE;
    }//if
}//KProfileMan::OnPreFrameMove()


void KProfileMan::OnCreateDevice( IDirect3DDevice9* pd3dDevice )
{
	D3DXCreateFont( pd3dDevice, 15, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		L"±¼¸²Ã¼", &g_pFont_Profiler );

}

void KProfileMan::OnResetDevice( IDirect3DDevice9* pd3dDevice )
{
	if( g_pFont_Profiler )
		g_pFont_Profiler->OnResetDevice();

	D3DXCreateSprite( pd3dDevice, &g_pTextSprite_Profiler );
}

void KProfileMan::OnLostDevice()
{
	if( g_pFont_Profiler )
		g_pFont_Profiler->OnLostDevice();
	SAFE_RELEASE( g_pTextSprite_Profiler );
}

void KProfileMan::OnDestroyDevice()
{
	SAFE_RELEASE( g_pFont_Profiler );
}

BOOL KProfileMan::OnFrameRender( IDirect3DDevice9* pd3dDevice, float fElapsedTime)
{
    if ( m_pitorRef == NULL || m_bEnableProfile == false )
        return FALSE;

    m_fThisTime = KProfile::AccessTimer().GetActualCurrentTime();

    float   fx;
    float   fy;
    float   fTotalTime  = 0.f;
    float   fHeight     = 16.f;
    float   fWidth;
    const float fxOffset = 600.f;
    float   fCurTime;

    fx = m_fx;
    fy = m_fy;
    m_iNumNodes = 0;
    m_fMaxTime  = 0.f;
    m_pitorRef->First();
    while ( FALSE == m_pitorRef->Is_Done() )
    {
        fCurTime = m_pitorRef->Get_Current_Total_Time();
        KMessageConsole::OutText( fx+1, fy+1, D3DXCOLOR(0,0,0,1), "%s", m_pitorRef->Get_Current_Name() );
		KMessageConsole::OutText( fx, fy, 0xffffffff, "%s", m_pitorRef->Get_Current_Name() );
		
        // draw horizontal 2d bar
        fWidth = fCurTime* m_fBarScaleFactor;
        John_Draw2DBar( pd3dDevice, fx+fxOffset, fy, fWidth, fHeight-1, 0xff0000ff );

		KMessageConsole::OutText( fx+fxOffset+1, fy+1, D3DXCOLOR(0,0,0,1), "call/frame=%i, time=%.4f",
			m_pitorRef->Get_Current_Total_Calls(),
			fCurTime );
		KMessageConsole::OutText( fx+fxOffset, fy, 0xffffff00, "call/frame=%i, time=%.4f",
            m_pitorRef->Get_Current_Total_Calls(),
            fCurTime );

        if (m_iNumNodes == m_iCursor)
		{
			KMessageConsole::OutText( fx-10+1, fy+1, D3DXCOLOR(0,0,0,1), ">" );
			KMessageConsole::OutText( fx-10, fy, 0xffffffff, ">" );
		}

        // set max time
        if ( fCurTime > m_fMaxTime )
            m_fMaxTime = fCurTime;

        fy          += fHeight;
        fTotalTime  += fCurTime;
        m_iNumNodes += 1;
        m_pitorRef->Next();
    }//while

    KMessageConsole::OutText( fx+1, fy+1, D3DXCOLOR(0,0,0,1), "BS: Back, Home: Normalize 2D bar" );
	KMessageConsole::OutText( fx, fy, 0xffffff00, "BS: Back, Home: Normalize 2D bar" );
    fy += fHeight;

    // profiler render time
    m_fThisTime = KProfile::AccessTimer().GetActualCurrentTime() - m_fThisTime;
    fTotalTime += m_fThisTime;

    if ( TRUE == RHProfileManager::ms_bDoProfile )
    {
        sprintf( m_buf, "[total time] = %g / %g = %g ",
            fTotalTime, fElapsedTime, fTotalTime/fElapsedTime );
    }//if
	KMessageConsole::OutText( fx+1, fy+1, D3DXCOLOR(0,0,0,1), m_buf );
    KMessageConsole::OutText( fx, fy, 0xffffff00, m_buf );

    return TRUE;
}//KProfileMan::OnFrameRender()


void KProfileMan::OnKeyboardProc(UINT nChar, bool bKeyDown, bool /*bAltDown*/)
{
    if ( m_pitorRef == NULL ) return;
    if ( false == bKeyDown ) return;

    float fStep = m_fBarScaleFactor / 100.f;
    if ( fStep < 100.f ) fStep = 100.f;

    switch ( nChar )
    {
    case VK_UP:
        m_iCursor = __max(0, m_iCursor-1);
        m_iCursor = __min(m_iNumNodes-1, m_iCursor);
        break;

    case VK_DOWN:
        m_iCursor = __min(m_iNumNodes-1, m_iCursor+1);
        break;

    case VK_BACK:
        m_pitorRef->Enter_Parent();
        break;

    case VK_RETURN:
        m_pitorRef->Enter_Child( m_iCursor );
        break;

    case VK_LEFT:
        m_fBarScaleFactor -= fStep;
        break;

    case VK_RIGHT:
        m_fBarScaleFactor += fStep;
		break;

	//case VK_HOME:
	case VK_INSERT:
		m_fBarScaleFactor = 400.f / m_fMaxTime;
		break;

	case VK_DELETE:
		m_bReservedClearRoot = true;
		break;
		
	case VK_OEM_3:
	//case '~':
	//case '`':
		{
			m_bEnableProfile = !m_bEnableProfile;
		} break;

//    case 'P':
//        g_option.m_bProfileMode ^= 1;
//        g_option.m_bUserKeyProcessing = g_option.m_bProfileMode;
//        break;
    }//switch
}//KProfileMan::OnKeyboardProc()

#pragma warning(pop)

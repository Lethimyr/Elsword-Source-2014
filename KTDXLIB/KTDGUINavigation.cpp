#include "StdAfx.h"
#include ".\ktdguinavigation.h"





CKTDGUINavigation::CKTDGUINavigation(void)
: m_ptLastMouse( POINT() )
, m_rcBG( RECT() )
, m_rcWindow( RECT() )
{
	m_ControlType = UCT_NAVIGATION;

	m_bDraggingWindow = false;

	m_pBGTex		= NULL;
	m_pWindowTex	= NULL;

	m_pBGPoint		= NULL;
	m_pWindowPoint	= NULL;

	m_pSndMouseDown = NULL;


	m_CustomMsgLButtonDown = -1;
	m_CustomMsgMouseDragging = -1;



//{{ robobeg : 2008-10-28
	//KLuaManager kLuaManager;
    KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28
	if( true == g_pKTDXApp->LoadAndDoMemory( &kLuaManager, L"UI_Control_Sound.lua" ) )
	{
		wstring wstrMouseDownSoundName = L"";
		LUA_GET_VALUE( kLuaManager, "Button_Mouse_Up", wstrMouseDownSoundName, L"" );
		m_pSndMouseDown = g_pKTDXApp->GetDeviceManager()->OpenSound( wstrMouseDownSoundName );
	}
}

CKTDGUINavigation::~CKTDGUINavigation(void)
{
	SAFE_DELETE( m_pBGTex );
	SAFE_DELETE( m_pWindowTex );

	SAFE_DELETE( m_pBGPoint );
	SAFE_DELETE( m_pWindowPoint );

	SAFE_CLOSE( m_pSndMouseDown );
}


HRESULT	CKTDGUINavigation::OnFrameMove( double fTime, float fElapsedTime )
{
	if( false == m_bEnable )
		return S_OK;

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	return S_OK;
}

HRESULT CKTDGUINavigation::OnFrameRender()
{
	if( false == m_bShow )
		return S_OK;


	// draw background
	if( NULL != m_pBGTex && NULL != m_pBGPoint )
	{
		m_pBGPoint->pUITextureData = m_pBGTex;

		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
			m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
			*m_pBGPoint );

		RenderVertex( m_Vertex, *m_pBGPoint );
	}


	// draw scrolling window
	if( NULL != m_pWindowTex && NULL != m_pWindowPoint )
	{
		m_pWindowPoint->pUITextureData = m_pWindowTex;

		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
			m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
			*m_pWindowPoint );

		RenderVertex( m_Vertex, *m_pWindowPoint );
	}

	return S_OK;

}


bool CKTDGUINavigation::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( false == m_bEnable )
		return false;

	return false;
}


bool CKTDGUINavigation::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( false == m_bEnable )
		return false;


#ifdef DLL_BUILD
	if( m_bUpdate == false )
		return false;
#endif

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			FromUIPointToRect( &m_rcBG, m_pBGPoint );
			FromUIPointToRect( &m_rcWindow, m_pWindowPoint );
			
			if( TRUE == PtInRect( &m_rcWindow, pt ) )
			{
				if( m_bHasFocus == false )
				{	
					if( m_pDialog != NULL && true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDialog ) )
					{
						m_pDialog->RequestFocus( this );
					}
				}

				m_bDraggingWindow = true;
#ifndef DLL_BUILD
				SetCapture( DXUTGetHWND() );
#endif

				if( m_CustomMsgLButtonDown != -1 )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgLButtonDown, (LPARAM)this );
				else
					g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_EVENT, NEM_LBUTTON_DOWN, (LPARAM)this, true );

				//if ( m_pSndMouseDown != NULL )
				//	m_pSndMouseDown->Play( false, false );
				PlaySound( m_pSndMouseDown );
			}
			else if( TRUE == PtInRect( & m_rcBG, pt ) )
			{
				if( m_bHasFocus == false )
				{	
					if( m_pDialog != NULL && true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDialog ) )
					{
						m_pDialog->RequestFocus( this );
					}
				}

				m_bDraggingWindow = true;
#ifndef DLL_BUILD
				SetCapture( DXUTGetHWND() );
#endif
				//if ( m_pSndMouseDown != NULL )
				//	m_pSndMouseDown->Play( false, false );
				PlaySound( m_pSndMouseDown );
				
				long width = m_rcWindow.right - m_rcWindow.left;
				long height = m_rcWindow.bottom - m_rcWindow.top;
				SetRect( &m_rcWindow, (int) (pt.x - width * 0.5f), (int)(pt.y - height * 0.5f),
									(int)(pt.x + width * 0.5f), (int)(pt.y + height * 0.5f) );
				BoundRectInRect( &m_rcWindow, m_rcBG );
				FromRectToPoint( m_rcWindow, m_pWindowPoint );

				if( m_CustomMsgLButtonDown != -1 )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgLButtonDown, (LPARAM)this );
				else
					g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_EVENT, NEM_LBUTTON_DOWN, (LPARAM)this, true );

				//if ( m_pSndMouseDown != NULL )
				//	m_pSndMouseDown->Play( false, false );
				PlaySound( m_pSndMouseDown );
			}

			m_ptLastMouse = pt;
			return true;

		} break;

	case WM_LBUTTONUP:
		{
			m_bDraggingWindow = false;
			ReleaseCapture();

			return true;
			
		} break;

	case WM_MOUSEMOVE:
		{
			if( true == m_bDraggingWindow )
			{
				FromUIPointToRect( &m_rcWindow, m_pWindowPoint );

				D3DXVECTOR2 vDisplace;
				vDisplace.x = (float) pt.x - m_ptLastMouse.x;
				vDisplace.y = (float) pt.y - m_ptLastMouse.y;

				OffsetRect( &m_rcWindow, (int) vDisplace.x, (int) vDisplace.y );
				BoundRectInRect( &m_rcWindow, m_rcBG );
				FromRectToPoint( m_rcWindow, m_pWindowPoint );

				m_ptLastMouse = pt;

				if( m_CustomMsgMouseDragging != -1 )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseDragging, (LPARAM)this );
				else
					g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_EVENT, NEM_MOUSE_DRAGGING, (LPARAM)this, true );

			}

			return true;
		} break;
	}

	return false;
}


bool CKTDGUINavigation::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_CAPTURECHANGED == uMsg )
	{
		// The application just lost mouse capture. We may not have gotten
		// the WM_MOUSEUP message, so reset m_bDrag here.
		m_bDraggingWindow = false;
	}

	return false;
}

void CKTDGUINavigation::FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData* point )
{
	point->leftTopPoint.x 		= (float)rect.left;
	point->leftTopPoint.y 		= (float)rect.top;
	point->rightTopPoint.x 		= (float)rect.right;
	point->rightTopPoint.y 		= (float)rect.top;
	point->leftBottomPoint.x 	= (float)rect.left;
	point->leftBottomPoint.y 	= (float)rect.bottom;
	point->rightBottomPoint.x 	= (float)rect.right;
	point->rightBottomPoint.y 	= (float)rect.bottom;
}


void CKTDGUINavigation::FromUIPointToRect( RECT* pRect, const CKTDGUIControl::UIPointData* pPointData )
{
	pRect->left = (long) pPointData->leftTopPoint.x;
	pRect->right = (long) pPointData->rightBottomPoint.x;

	pRect->top = (long) pPointData->leftTopPoint.y;
	pRect->bottom = (long) pPointData->rightBottomPoint.y;
}


// let inside RECT reside in outside RECT
void CKTDGUINavigation::BoundRectInRect( RECT* pInsideRect, const RECT& outsideRect )
{
	long iOffsetX = 0;
	long iOffsetY = 0;
	if( pInsideRect->left < outsideRect.left )
	{
		iOffsetX = outsideRect.left - pInsideRect->left;
	}
	else if( pInsideRect->right > outsideRect.right )
	{
		iOffsetX = outsideRect.right - pInsideRect->right;
	}

	if( pInsideRect->top < outsideRect.top )
	{
		iOffsetY = outsideRect.top - pInsideRect->top;
	}
	else if( pInsideRect->bottom > outsideRect.bottom )
	{
		iOffsetY = outsideRect.bottom - pInsideRect->bottom;
	}

	if( iOffsetX != 0 || iOffsetY != 0 )
	{
		OffsetRect( pInsideRect, iOffsetX, iOffsetY );
	}

}

bool CKTDGUINavigation::SetBGTex( const WCHAR* pFileName, const WCHAR* key )
{
	SAFE_DELETE( m_pBGTex );

	m_pBGTex = SetTexureData_( pFileName, key );
	if( m_pBGPoint != NULL )
		m_pBGPoint->pUITextureData = m_pBGTex;

	if( m_pBGTex != NULL )
		return true;
	else
		return false;
}


bool CKTDGUINavigation::SetWindowTex( const WCHAR* pFileName, const WCHAR* key )
{
	SAFE_DELETE( m_pWindowTex );

	m_pWindowTex = SetTexureData_( pFileName, key );
	if( m_pWindowPoint != NULL )
		m_pWindowPoint->pUITextureData = m_pWindowTex;

	if( m_pWindowTex != NULL )
		return true;
	else
		return false;
}

void CKTDGUINavigation::SetBGTex_LUA( const char* pFileName, const char* pKey )
{
	wstring wstrFileName = L"";
	wstring wstrKeyName = L"";

	ConvertUtf8ToWCHAR( wstrFileName, pFileName );
	ConvertUtf8ToWCHAR( wstrKeyName, pKey );

	SetBGTex( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void CKTDGUINavigation::SetWindowTex_LUA( const char* pFileName, const char* pKey )
{
	wstring wstrFileName = L"";
	wstring wstrKeyName = L"";

	ConvertUtf8ToWCHAR( wstrFileName, pFileName );
	ConvertUtf8ToWCHAR( wstrKeyName, pKey );

	SetWindowTex( wstrFileName.c_str(), wstrKeyName.c_str() );
}


void CKTDGUINavigation::SetBGPoint( CKTDGUIControl::UIPointData* pUIPointData )
{
	SAFE_DELETE( m_pBGPoint );
	m_pBGPoint = pUIPointData;
	m_pBGPoint->pUITextureData = m_pBGTex;
	m_pBGPoint->SetAutoPointByTextureSize();
}


void CKTDGUINavigation::SetWindowPoint( CKTDGUIControl::UIPointData* pUIPointData )
{
	SAFE_DELETE( m_pWindowPoint );
	m_pWindowPoint = pUIPointData;
	m_pWindowPoint->pUITextureData = m_pBGTex;
	m_pWindowPoint->SetAutoPointByTextureSize();
}


void CKTDGUINavigation::SetBGPoint_LUA()
{
	CKTDGUIControl::UIPointData* pUIPointData = SetPointData_();
	SetBGPoint( pUIPointData );
}



void CKTDGUINavigation::SetWindowPoint_LUA()
{
	CKTDGUIControl::UIPointData* pUIPointData = SetPointData_();
	SetWindowPoint( pUIPointData );
}


bool CKTDGUINavigation::ContainsPoint( POINT pt )
{
	KTDXPROFILE();
	FromUIPointToRect( &m_rcBG, m_pBGPoint );

	if( TRUE == PtInRect( &m_rcBG, pt ) )
		return true;

	return false;

}

D3DXVECTOR2 CKTDGUINavigation::GetRelativeWindowPos()
{
	D3DXVECTOR2 vRelativePos;
	vRelativePos.x = (m_pWindowPoint->leftTopPoint.x-m_pBGPoint->leftTopPoint.x) / (m_pBGPoint->rightTopPoint.x - m_pBGPoint->leftTopPoint.x);
	vRelativePos.y = (m_pWindowPoint->leftTopPoint.y-m_pBGPoint->leftTopPoint.y) / (m_pBGPoint->leftBottomPoint.y - m_pBGPoint->leftTopPoint.y);

	return vRelativePos;
}

void CKTDGUINavigation::SetRelativeWindowPos( D3DXVECTOR2 vRelativePos )
{
	int iBGWidth = (int) (m_pBGPoint->rightTopPoint.x - m_pBGPoint->leftTopPoint.x);
	int iBGHeight = (int) (m_pBGPoint->leftBottomPoint.y - m_pBGPoint->leftTopPoint.y);

	int iWidth = (int) (m_pWindowPoint->rightTopPoint.x - m_pWindowPoint->leftTopPoint.x);
	int iHeight = (int) (m_pWindowPoint->leftBottomPoint.y - m_pWindowPoint->leftTopPoint.y);

	int left = (int) ( m_pBGPoint->leftTopPoint.x + iBGWidth * vRelativePos.x );
	int top = (int) ( m_pBGPoint->leftTopPoint.y + iBGHeight * vRelativePos.y );

	SetRect( &m_rcWindow, left, top, left + iWidth, top + iHeight );
	FromUIPointToRect( &m_rcBG, m_pBGPoint );
	BoundRectInRect( &m_rcWindow, m_rcBG );
	FromRectToPoint( m_rcWindow, m_pWindowPoint );
}

void CKTDGUINavigation::SetRelativeWindowSize( D3DXVECTOR2 vSize )
{
	int iBGWidth = (int) (m_pBGPoint->rightTopPoint.x - m_pBGPoint->leftTopPoint.x);
	int iBGHeight = (int) (m_pBGPoint->leftBottomPoint.y - m_pBGPoint->leftTopPoint.y);

	int iWidth = (int) (iBGWidth * vSize.x );
	int iHeight = (int) (iBGHeight * vSize.y );

	int left = (int) ( m_pBGPoint->leftTopPoint.x );
	int top = (int) ( m_pBGPoint->leftTopPoint.y );

	SetRect( &m_rcWindow, left, top, left + iWidth, top + iHeight );
	FromUIPointToRect( &m_rcBG, m_pBGPoint );
	BoundRectInRect( &m_rcWindow, m_rcBG );
	FromRectToPoint( m_rcWindow, m_pWindowPoint );
}

#ifdef DLL_BUILD
void CKTDGUINavigation::MoveControl( float fx, float fy )
{
	if( NULL != m_pBGPoint )
		m_pBGPoint->Move(fx, fy);

	if( NULL != m_pWindowPoint )
		m_pWindowPoint->Move(fx, fy);	
}

void CKTDGUINavigation::SetEditGUI( bool bEdit )
{
	m_bUpdate = !bEdit;
}

D3DXVECTOR2 CKTDGUINavigation::GetPos()
{
	if( NULL != m_pBGPoint )
		return m_pBGPoint->leftTopPoint;

	return D3DXVECTOR2(0, 0);
}

vector<D3DXVECTOR2> CKTDGUINavigation::GetPosList()
{
	vector<D3DXVECTOR2> ret;

	if( NULL != m_pBGPoint )
		ret.push_back( m_pBGPoint->leftTopPoint);

	if( NULL != m_pWindowPoint )
		ret.push_back( m_pWindowPoint->leftTopPoint );

	return ret;
}

D3DXVECTOR2 CKTDGUINavigation::GetPos(wstring name)
{
	if( name == L"BG_RIGHT_BOTTOM" && NULL != m_pBGPoint )
		return m_pBGPoint->rightBottomPoint;
	else if( name == L"WINDIOW_RIGHT_BOTTOM" && NULL != m_pWindowPoint )
		return m_pWindowPoint->rightBottomPoint;

	return D3DXVECTOR2(0, 0);
}

#endif

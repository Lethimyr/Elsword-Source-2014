#include "StdAfx.h"
#include ".\ktdguiscrollbar.h"

// Minimum scroll bar thumb size
#define SCROLLBAR_MINTHUMBSIZE 8

// Delay and repeat period when clicking on the scroll bar arrows
#define SCROLLBAR_ARROWCLICK_DELAY  0.33
#define SCROLLBAR_ARROWCLICK_REPEAT 0.05

inline int RectWidth( RECT &rc ) { return ( (rc).right - (rc).left ); }
inline int RectHeight( RECT &rc ) { return ( (rc).bottom - (rc).top ); }


void CKTDGUIScrollBar::FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData& point )
{
	point.leftTopPoint.x = (float)rect.left;
	point.leftTopPoint.y = (float)rect.top;
	point.rightTopPoint.x = (float)rect.right;
	point.rightTopPoint.y = (float)rect.top;
	point.leftBottomPoint.x = (float)rect.left;
	point.leftBottomPoint.y = (float)rect.bottom;
	point.rightBottomPoint.x = (float)rect.right;
	point.rightBottomPoint.y = (float)rect.bottom;
}



CKTDGUIScrollBar::CKTDGUIScrollBar()
: m_LastMouse( POINT() )
, m_rcBoundingBox( RECT() )
, m_rcDownButton( RECT() )
, m_rcUpButton( RECT() )
, m_rcThumb( RECT() )
, m_rcTrack( RECT() )
{
	m_ControlType = UCT_SCROLLBAR;

	m_bShowThumb = true;
	m_bDrag = false;

	m_bIsUpButtonClicked = false;
	m_bIsDownButtonClicked = false;

	SetRect( &m_rcUpButton, 0, 0, 0, 0 );
	SetRect( &m_rcDownButton, 0, 0, 0, 0 );
	SetRect( &m_rcTrack, 0, 0, 0, 0 );
	SetRect( &m_rcThumb, 0, 0, 0, 0 );
	m_nPosition = 0;
	m_nPageSize = 1;
	m_nStart = 0;
	m_nEnd = 1;
	m_Arrow = CLEAR;
	m_dArrowTS = 0.0;

	m_pDownButtonTex = NULL;
	m_pDownButtonDownTex = NULL;
	m_pUpButtonTex = NULL;
	m_pUpButtonDownTex = NULL;
	m_pThumbTex = NULL;
	m_pTrackTex = NULL;
	
	m_bEnableThumb = true;
	m_bEnableTrack = true;


	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;

	m_pSndMouseDown = NULL;

//{{ robobeg : 2008-10-28
	//KLuaManager kLuaManager;
    KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28

	if(  g_pKTDXApp->LoadAndDoMemory( &kLuaManager, L"UI_Control_Sound.lua" ) == false )
	{
		return;
	}

	wstring checkSndFileName;
	LUA_GET_VALUE( kLuaManager, "ScrollBar_Mouse_Down", checkSndFileName, L"" );
	m_pSndMouseDown = g_pKTDXApp->GetDeviceManager()->OpenSound( checkSndFileName );

#ifdef DLL_BUILD
	m_pCheckedEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );
	m_bEditEdge = false;
#endif

}


//--------------------------------------------------------------------------------------
CKTDGUIScrollBar::~CKTDGUIScrollBar()
{
	SAFE_CLOSE( m_pSndMouseDown );

#ifdef DLL_BUILD
	SAFE_CLOSE( m_pCheckedEdgeTexture );
#endif
}


//--------------------------------------------------------------------------------------
void CKTDGUIScrollBar::UpdateRects()
{
	
	SetRect( &m_rcBoundingBox, m_x, m_y, m_x + m_width, m_y + m_height );
	// Make the buttons square

	SetRect( &m_rcUpButton, m_rcBoundingBox.left, m_rcBoundingBox.top,
		m_rcBoundingBox.right, m_rcBoundingBox.top + RectWidth( m_rcBoundingBox ) );
	SetRect( &m_rcDownButton, m_rcBoundingBox.left, m_rcBoundingBox.bottom - RectWidth( m_rcBoundingBox ),
		m_rcBoundingBox.right, m_rcBoundingBox.bottom );
	SetRect( &m_rcTrack, m_rcUpButton.left, m_rcUpButton.bottom,
		m_rcDownButton.right, m_rcDownButton.top );
	
	m_rcThumb.left = m_rcUpButton.left;
	m_rcThumb.right = m_rcUpButton.right;

	UpdateThumbRect();
}


//--------------------------------------------------------------------------------------
// Compute the dimension of the scroll thumb
void CKTDGUIScrollBar::UpdateThumbRect()
{
	if( m_nEnd - m_nStart > m_nPageSize )
	{
		int nThumbHeight = __max( RectHeight( m_rcTrack ) * m_nPageSize / ( m_nEnd - m_nStart ), SCROLLBAR_MINTHUMBSIZE );
		int nMaxPosition = m_nEnd - m_nStart - m_nPageSize;
		m_rcThumb.top = m_rcTrack.top + ( m_nPosition - m_nStart ) * ( RectHeight( m_rcTrack ) - nThumbHeight )
			/ nMaxPosition;
		m_rcThumb.bottom = m_rcThumb.top + nThumbHeight;
		m_bShowThumb = true;

	} 
	else
	{
		// No content to scroll
		m_rcThumb.bottom = m_rcThumb.top;
		m_bShowThumb = false;
	}
}


//--------------------------------------------------------------------------------------
// Scroll() scrolls by nDelta items.  A positive value scrolls down, while a negative
// value scrolls up.
void CKTDGUIScrollBar::Scroll( int nDelta )
{
	// Perform scroll
	m_nPosition += nDelta;

	// Cap position
	Cap();

	// Update thumb position
	UpdateThumbRect();
}


//--------------------------------------------------------------------------------------
void CKTDGUIScrollBar::ShowItem( int nIndex )
{
	// Cap the index

	if( nIndex < 0 )
		nIndex = 0;

	if( nIndex >= m_nEnd )
		nIndex = m_nEnd - 1;

	// Adjust position

	if( m_nPosition > nIndex )
		m_nPosition = nIndex;
	else
		if( m_nPosition + m_nPageSize <= nIndex )
			m_nPosition = nIndex - m_nPageSize + 1;

	UpdateThumbRect();
}


//--------------------------------------------------------------------------------------
bool CKTDGUIScrollBar::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return false;
}


//--------------------------------------------------------------------------------------
bool CKTDGUIScrollBar::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	static int ThumbOffsetY;

	//MessageBox( g_pKTDXApp->GetHWND(), L"스크롤바가 여깄다", L"하하", MB_OK);

	if ( m_bShow == false || m_bEnable == false )
	{
		return false;
	}

	m_LastMouse = pt;
	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			// Check for click on up button

			if( PtInRect( &m_rcUpButton, pt ) )
			{
				//MessageBox(NULL, L"UpButton", L"dd", MB_OK );
#ifndef DLL_BUILD
				SetCapture( DXUTGetHWND() );
#endif
				if( m_nPosition > m_nStart )
					--m_nPosition;
				UpdateThumbRect();
				m_Arrow = CLICKED_UP;
#ifndef DLL_BUILD
				m_dArrowTS = DXUTGetTime();
#endif
				m_bIsUpButtonClicked = true;

				//if ( m_pSndMouseDown != NULL )
				//	m_pSndMouseDown->Play( false, false );
				PlaySound( m_pSndMouseDown );

				return true;
			}

			// Check for click on down button
			if( PtInRect( &m_rcDownButton, pt ) )
			{
				//MessageBox(NULL, L"DownButton", L"dd", MB_OK );
#ifndef DLL_BUILD
				SetCapture( DXUTGetHWND() );
#endif
				if( m_nPosition + m_nPageSize < m_nEnd )
					++m_nPosition;
				UpdateThumbRect();
				m_Arrow = CLICKED_DOWN;
#ifndef DLL_BUILD
				m_dArrowTS = DXUTGetTime();
#endif

				m_bIsDownButtonClicked = true;

				//if ( m_pSndMouseDown != NULL )
				//	m_pSndMouseDown->Play( false, false );
				PlaySound( m_pSndMouseDown );

				return true;
			}


			// Check for click on thumb
			if( true == m_bEnableThumb )
			{
				if( PtInRect( &m_rcThumb, pt ) )
				{
					//MessageBox(NULL, L"Thumb", L"dd", MB_OK );
#ifndef DLL_BUILD
					SetCapture( DXUTGetHWND() );
#endif
					m_bDrag = true;
					ThumbOffsetY = pt.y - m_rcThumb.top;
					return true;
				}
			}

			// Check for click on track
			if( true == m_bEnableTrack )
			{
				if( m_rcThumb.left <= pt.x &&
					m_rcThumb.right > pt.x &&  m_bShowThumb == true )
				{
#ifndef DLL_BUILD
					SetCapture( DXUTGetHWND() );
#endif
					if( m_rcThumb.top > pt.y &&
						m_rcTrack.top <= pt.y )
					{
						//MessageBox(NULL, L"Track1", L"dd", MB_OK );
						Scroll( -( m_nPageSize - 1 ) );
						return true;
					} 
					else if( m_rcThumb.bottom <= pt.y &&
						m_rcTrack.bottom > pt.y )
					{
						//MessageBox(NULL, L"Track2", L"dd", MB_OK );
						Scroll( m_nPageSize - 1 );
						return true;
					}
				}
			}

			break;
		}

	case WM_LBUTTONUP:
		{
			m_bIsUpButtonClicked = false;
			m_bIsDownButtonClicked = false;

			m_bDrag = false;
			ReleaseCapture();
			UpdateThumbRect();
			m_Arrow = CLEAR;
			break;
		}

	case WM_MOUSEMOVE:
		{
			if( m_bDrag == true )
			{
				//MessageBox(NULL, L"Drag", L"dd", MB_OK );

				m_rcThumb.bottom += pt.y - ThumbOffsetY - m_rcThumb.top;
				m_rcThumb.top = pt.y - ThumbOffsetY;
				if( m_rcThumb.top < m_rcTrack.top )
					OffsetRect( &m_rcThumb, 0, m_rcTrack.top - m_rcThumb.top );
				else
					if( m_rcThumb.bottom > m_rcTrack.bottom )
						OffsetRect( &m_rcThumb, 0, m_rcTrack.bottom - m_rcThumb.bottom );

				// Compute first item index based on thumb position

				int nMaxFirstItem = m_nEnd - m_nStart - m_nPageSize;  // Largest possible index for first item
				int nMaxThumb = RectHeight( m_rcTrack ) - RectHeight( m_rcThumb );  // Largest possible thumb position from the top

				// 뻗으면 안돼~
				if( nMaxFirstItem == 0 || nMaxThumb == 0 )
					return true;

				m_nPosition = m_nStart +
					( m_rcThumb.top - m_rcTrack.top +
					nMaxThumb / ( nMaxFirstItem * 2 ) ) * // Shift by half a row to avoid last row covered by only one pixel
					nMaxFirstItem  / nMaxThumb;

				return true;
			}

			break;
		}
	}

	return false;
}


//--------------------------------------------------------------------------------------
bool CKTDGUIScrollBar::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_CAPTURECHANGED == uMsg )
	{
		// The application just lost mouse capture. We may not have gotten
		// the WM_MOUSEUP message, so reset m_bDrag here.
		m_bDrag = false;
	}

	return false;
}

HRESULT	CKTDGUIScrollBar::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bShow == false )
	{
		return S_OK;
	}

#ifdef DLL_BUILD
	if( m_bUpdate == false )
		return S_OK;
#endif

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	RECT tempRECT = m_rcTrack;

	//tempRECT.top -= (m_rcUpButton.bottom - m_rcUpButton.top);
	//tempRECT.bottom += (m_rcDownButton.bottom - m_rcDownButton.top);

	FromRectToPoint( tempRECT, m_NowPoint );

	return S_OK;
}

//--------------------------------------------------------------------------------------
HRESULT CKTDGUIScrollBar::OnFrameRender()
{
	// Check if the arrow button has been held for a while.
	// If so, update the thumb position to simulate repeated
	// scroll.

	if ( m_bShow == false )
	{
		return S_OK;
	}
#ifdef DLL_BUILD
	DrawEditEdge();
#endif
#ifndef DLL_BUILD
	if( m_Arrow != CLEAR )
	{
		double dCurrTime = DXUTGetTime();
		if( PtInRect( &m_rcUpButton, m_LastMouse ) )
		{
			switch( m_Arrow )
			{
			case CLICKED_UP:
				if( SCROLLBAR_ARROWCLICK_DELAY < dCurrTime - m_dArrowTS )
				{
					Scroll( -1 );
					m_Arrow = HELD_UP;
					m_dArrowTS = dCurrTime;
				}
				break;
			case HELD_UP:
				if( SCROLLBAR_ARROWCLICK_REPEAT < dCurrTime - m_dArrowTS )
				{
					Scroll( -1 );
					m_dArrowTS = dCurrTime;
				}
				break;
			}
		} else
			if( PtInRect( &m_rcDownButton, m_LastMouse ) )
			{
				switch( m_Arrow )
				{
				case CLICKED_DOWN:
					if( SCROLLBAR_ARROWCLICK_DELAY < dCurrTime - m_dArrowTS )
					{
						Scroll( 1 );
						m_Arrow = HELD_DOWN;
						m_dArrowTS = dCurrTime;
					}
					break;
				case HELD_DOWN:
					if( SCROLLBAR_ARROWCLICK_REPEAT < dCurrTime - m_dArrowTS )
					{
						Scroll( 1 );
						m_dArrowTS = dCurrTime;
					}
					break;
				}
			}
	}
#endif

	if( true == m_bEnableTrack )
	{
		FromRectToPoint(m_rcTrack, m_TempPoint);
		if ( m_pTrackTex != NULL )
		{
			m_TempPoint.pUITextureData = m_pTrackTex;

			UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
				m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
				m_TempPoint );
			RenderVertex( m_Vertex, m_TempPoint );
		}
	}



	FromRectToPoint(m_rcUpButton, m_TempPoint);
	if ( m_bIsUpButtonClicked == false )
	{
		if ( m_pUpButtonTex != NULL )
		{
			m_TempPoint.pUITextureData = m_pUpButtonTex;

			UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
				m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
				m_TempPoint );
			RenderVertex( m_Vertex, m_TempPoint );
		}

	}
	else
	{
		if ( m_pUpButtonDownTex != NULL )
		{
			m_TempPoint.pUITextureData = m_pUpButtonDownTex;

			UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
				m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
				m_TempPoint );
			RenderVertex( m_Vertex, m_TempPoint );
		}
		else if ( m_pUpButtonTex != NULL )
		{
			m_TempPoint.pUITextureData = m_pUpButtonTex;

			UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
				m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
				m_TempPoint );
			RenderVertex( m_Vertex, m_TempPoint );
		}
	}
	


	FromRectToPoint(m_rcDownButton, m_TempPoint);
	if ( m_bIsDownButtonClicked == false )
	{
		if ( m_pDownButtonTex != NULL )
		{
			m_TempPoint.pUITextureData = m_pDownButtonTex;

			UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
				m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
				m_TempPoint);
			RenderVertex( m_Vertex, m_TempPoint );
		}
	}
	else
	{
		if ( m_pDownButtonDownTex != NULL )
		{
			m_TempPoint.pUITextureData = m_pDownButtonDownTex;

			UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
				m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
				m_TempPoint);
			RenderVertex( m_Vertex, m_TempPoint );
		}
		else if (  m_pDownButtonTex != NULL )
		{
			m_TempPoint.pUITextureData = m_pDownButtonTex;

			UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
				m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
				m_TempPoint);
			RenderVertex( m_Vertex, m_TempPoint );
		}
	}
	


	if( true == m_bEnableThumb )
	{
		if ( m_bShowThumb == true )
		{
			FromRectToPoint(m_rcThumb, m_TempPoint);

			if ( m_pThumbTex != NULL )
			{
				m_TempPoint.pUITextureData = m_pThumbTex;

				UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
					m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
					m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
					m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
					m_TempPoint);
				RenderVertex( m_Vertex, m_TempPoint );
			}		
		}
	}

	return S_OK;

}


//--------------------------------------------------------------------------------------
void CKTDGUIScrollBar::SetTrackRange( int nStart, int nEnd )
{
	m_nStart = nStart; m_nEnd = nEnd;
	Cap();
	UpdateThumbRect();
}

void CKTDGUIScrollBar::SetTrackPos( int nPosition ) 
{
	if ( nPosition <= m_nStart )
		nPosition = m_nStart;

	if( nPosition >= m_nEnd )
		nPosition = m_nEnd;

	m_nPosition = nPosition; 
	Cap(); 
	UpdateThumbRect(); 
}

//--------------------------------------------------------------------------------------
void CKTDGUIScrollBar::Cap()  // Clips position at boundaries. Ensures it stays within legal range.
{
	if( m_nPosition < m_nStart ||
		m_nEnd - m_nStart <= m_nPageSize )
	{
		m_nPosition = m_nStart;
	}
	else
		if( m_nPosition + m_nPageSize > m_nEnd )
			m_nPosition = m_nEnd - m_nPageSize;
}

void CKTDGUIScrollBar::SetUpButtonTex( CKTDGUIControl::UITextureData* pUpButtonTex)
{
	SAFE_DELETE( m_pUpButtonTex );
	m_pUpButtonTex = pUpButtonTex;

}

void CKTDGUIScrollBar::SetDownButtonTex( CKTDGUIControl::UITextureData* pDownButtonTex )
{
	SAFE_DELETE( m_pDownButtonTex );
	m_pDownButtonTex = pDownButtonTex;

}

void CKTDGUIScrollBar::SetUpButtonDownTex( CKTDGUIControl::UITextureData* pUpButtonDownTex)
{
	SAFE_DELETE( m_pUpButtonDownTex );
	m_pUpButtonDownTex = pUpButtonDownTex;

}

void CKTDGUIScrollBar::SetDownButtonDownTex( CKTDGUIControl::UITextureData* pDownButtonDownTex )
{
	SAFE_DELETE( m_pDownButtonDownTex );
	m_pDownButtonDownTex = pDownButtonDownTex;

}

void CKTDGUIScrollBar::SetTrackTex( CKTDGUIControl::UITextureData* pTrackTex )
{
	SAFE_DELETE( m_pTrackTex );
	m_pTrackTex = pTrackTex;

}

void CKTDGUIScrollBar::SetThumbTex( CKTDGUIControl::UITextureData* pThumbTex)
{
	SAFE_DELETE( m_pThumbTex );
	m_pThumbTex = pThumbTex;

}

#ifdef DLL_BUILD
void CKTDGUIScrollBar::MoveControl( float fx, float fy )
{
	SetLocation( m_x + (int)fx, m_y + (int)fy );
}

void CKTDGUIScrollBar::SetEditGUI( bool bEdit )
{
	m_bUpdate = !bEdit;	

	//m_bEditEdge = bEdit;
	//SetColor(D3DXCOLOR(0xffffffff));	// 활성할때 Edge(외곽선) 생기므로 활성화 색깔을 초기화한다.
}


D3DXVECTOR2 CKTDGUIScrollBar::GetPos()
{
	return D3DXVECTOR2((float)m_x, (float)m_y);
}

void CKTDGUIScrollBar::ShowEdge( bool bShow )
{
	SetColor(D3DXCOLOR(0xffffffff));	// 활성할때 Edge(외곽선) 생기므로 활성화 색깔을 초기화한다.
	m_bEditEdge = bShow;	
}

void CKTDGUIScrollBar::DrawEditEdge()
{
	if( false == m_bEditEdge )
		return;	

	if ( m_pCheckedEdgeTexture == NULL )
		return;

	RECT edgeRect;
	edgeRect.left = m_rcUpButton.left;
	edgeRect.top = m_rcUpButton.top;
	edgeRect.right = m_rcDownButton.right;
	edgeRect.bottom = m_rcDownButton.bottom;

	//const CKTDGUIControl::UIPointData & point = *m_pEditEdgePoint;
	D3DXCOLOR tempColor;

	int edgeWidth = 4;
	D3DXCOLOR edgeColor = D3DXCOLOR(0xffff0000);

	tempColor.a = edgeColor.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = edgeColor.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = edgeColor.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = edgeColor.b * m_pDialog->GetColor().b * m_Color.b;


	int _width = (int)(edgeRect.right - edgeRect.left);
	int _height = (int)(edgeRect.bottom - edgeRect.top);

	//if ( m_bDrawEdgeOut == true )
	{
		// 좌 left/top
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top - edgeWidth), 
			edgeWidth , 
			_height + edgeWidth, 
			tempColor );

		// 하left/bottom
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.bottom ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );

		// 우right/top
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.right ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top ), 
			edgeWidth, 
			_height + edgeWidth, 
			tempColor );

		// 상left/top
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top - edgeWidth ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );
	}
}

#endif
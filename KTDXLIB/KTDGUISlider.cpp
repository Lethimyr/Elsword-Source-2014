#include "StdAfx.h"
#include ".\ktdguislider.h"

inline int RectWidth( RECT &rc ) { return ( (rc).right - (rc).left ); }
inline int RectHeight( RECT &rc ) { return ( (rc).bottom - (rc).top ); }

CKTDGUISlider::CKTDGUISlider() 
: m_iButtonWidth( 0 )
, m_iButtonHeight( 0 )
, m_rcButton( RECT() )
, m_rcBoundingBox( RECT() )
{
	m_ControlType = UCT_SLIDER;

	m_nMin = 0;
	m_nMax = 100;
	m_nValue = 50;

	m_nDragOffset = 0;
	m_nDragX = 0;

	m_x = m_y = m_width = m_height = 0;

	m_bPressed = false;

	m_pBGTex = NULL;
	m_pButtonTex = NULL;

	m_CustomMsgValueChanged = -1;

#ifdef DLL_BUILD
	m_pCheckedEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );
	m_bEditEdge = false;
#endif
}


CKTDGUISlider::~CKTDGUISlider(void)
{
	SAFE_DELETE( m_pBGTex );
	SAFE_DELETE( m_pButtonTex );

#ifdef DLL_BUILD
	SAFE_CLOSE( m_pCheckedEdgeTexture );
#endif
}

HRESULT	CKTDGUISlider::OnFrameMove(double fTime, float fElapsedTime )
{
	if ( m_bShow == false )
	{
		return S_OK;
	}

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	FromRectToPoint(m_rcBoundingBox, m_NowPoint);

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CKTDGUISlider::OnFrameRender()
{
	if ( m_bShow == false )
	{
		return false;
	}

#ifdef DLL_BUILD
	DrawEditEdge();
#endif

	int nOffsetX = 0;
	int nOffsetY = 0;

	DXUT_CONTROL_STATE iState = DXUT_STATE_NORMAL;

	if( m_bShow == false )
	{
		iState = DXUT_STATE_HIDDEN;
	}
	else if( m_bEnable == false )
	{
		iState = DXUT_STATE_DISABLED;
	}
	else if( m_bPressed )
	{
		iState = DXUT_STATE_PRESSED;

		nOffsetX = 1;
		nOffsetY = 2;
	}
	else if( m_bMouseOver )
	{
		iState = DXUT_STATE_MOUSEOVER;

		nOffsetX = -1;
		nOffsetY = -2;
	}
	else if( m_bHasFocus )
	{
		iState = DXUT_STATE_FOCUS;
	}


	


	FromRectToPoint(m_rcBoundingBox, m_TempPoint);
	if ( m_pBGTex != NULL )
	{
		m_TempPoint.pUITextureData = m_pBGTex;
	}

	UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
		m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
		m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
		m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
		m_TempPoint );

	RenderVertex( m_Vertex, m_TempPoint );


	FromRectToPoint(m_rcButton, m_TempPoint);
	if ( m_pButtonTex != NULL )
	{
		m_TempPoint.pUITextureData = m_pButtonTex;
	}

	UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
		m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
		m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
		m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
		m_TempPoint );

	RenderVertex( m_Vertex, m_TempPoint );


	/*
	float fBlendRate = ( iState == DXUT_STATE_PRESSED ) ? 0.0f : 0.8f;

	CDXUTElement* pElement = m_Elements.GetAt( 0 );

	// Blend current color
	pElement->TextureColor.Blend( iState, fElapsedTime, fBlendRate ); 
	m_pDialog->DrawSprite( pElement, &m_rcBoundingBox );

	//TODO: remove magic numbers
	pElement = m_Elements.GetAt( 1 );

	// Blend current color
	pElement->TextureColor.Blend( iState, fElapsedTime, fBlendRate );
	m_pDialog->DrawSprite( pElement, &m_rcButton );
	*/

	return S_OK;
}


//--------------------------------------------------------------------------------------
bool CKTDGUISlider::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false || m_bEnable == false )
		return false;

	return false;
}


//--------------------------------------------------------------------------------------
bool CKTDGUISlider::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false || m_bEnable == false )
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
			if( PtInRect( &m_rcButton, pt ) )
			{
				// Pressed while inside the control
				m_bPressed = true;
#ifndef DLL_BUILD
				SetCapture( DXUTGetHWND() );
#endif
				m_nDragX = pt.x;
				//m_nDragY = pt.y;
				m_nDragOffset = m_nButtonX - m_nDragX;

				//m_nDragValue = m_nValue;

				if( !m_bHasFocus )
					m_pDialog->RequestFocus( this );

				return true;
			}

			if( PtInRect( &m_rcBoundingBox, pt ) )
			{
				m_nDragX = pt.x;
				m_nDragOffset = 0;
				//m_bPressed = true;

				if( !m_bHasFocus )
					m_pDialog->RequestFocus( this );

				if( pt.x > m_nButtonX + m_x )
				{
					SetValueInternal( m_nValue + 1, true );
					return true;
				}

				if( pt.x < m_nButtonX + m_x )
				{
					SetValueInternal( m_nValue - 1, true );
					return true;
				}
			}

			break;
		}

	case WM_LBUTTONUP:
		{
			if( m_bPressed )
			{
				m_bPressed = false;
				ReleaseCapture();
				//m_pDialog->SendEvent( EVENT_SLIDER_VALUE_CHANGED, true, this );
				if( m_CustomMsgValueChanged != -1 )
				{
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgValueChanged, (LPARAM)this );
				}
				else
				{
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, SLIDER_VALUE_CHANGED, (LPARAM)this );
				}
				return true;
			}

			break;
		}

	case WM_MOUSEMOVE:
		{
			if( m_bPressed )
			{
				SetValueInternal( ValueFromPos( m_x + pt.x + m_nDragOffset ), true );
				return true;
			}

			break;
		}

	case WM_MOUSEWHEEL:
		{
			int nScrollAmount = int((short)HIWORD(wParam)) / WHEEL_DELTA;
			SetValueInternal( m_nValue - nScrollAmount, true );
			return true;
		}
	};

	return false;
}
/*
//--------------------------------------------------------------------------------------
BOOL CKTDGUISlider::ContainsPoint( POINT pt ) 
{ 
	return ( PtInRect( &m_rcBoundingBox, pt ) || 
		PtInRect( &m_rcButton, pt ) ); 
}
*/

//--------------------------------------------------------------------------------------
void CKTDGUISlider::UpdateRects()
{
	if( 0 == m_width || 0 == m_height )
		return; 

	if( 0 == m_iButtonHeight )
	{
		m_iButtonHeight = m_height;
	}

	if( 0 == m_iButtonWidth )
	{
		m_iButtonWidth = m_height;
	}

	SetRect( &m_rcBoundingBox, m_x, m_y, m_x + m_width, m_y + m_height );
	SetRect( &m_rcButton, m_x, m_y, m_x + m_iButtonWidth, m_y + m_iButtonHeight );


	OffsetRect( &m_rcButton, 0, (m_height - m_iButtonHeight)/2 );							// 버튼의 left_top의 Y 축 위치

	m_nButtonX = (int) ( (m_nValue - m_nMin) * (float)m_width / (m_nMax - m_nMin) );		// 버튼의 중심위치

	OffsetRect( &m_rcButton, m_nButtonX - (int)m_iButtonWidth/2, 0 );						// 버튼의 left_top의 X 축 위치
}

int CKTDGUISlider::ValueFromPos( int x )
{ 
	float fValuePerPixel = (float)(m_nMax - m_nMin) / RectWidth( m_rcBoundingBox );
	return (int) (0.5f + m_nMin + fValuePerPixel * (x - m_rcBoundingBox.left)) ; 
}




//--------------------------------------------------------------------------------------
void CKTDGUISlider::SetRange( int nMin, int nMax ) 
{
	m_nMin = nMin;
	m_nMax = nMax;

	SetValueInternal( m_nValue, false );
}

void CKTDGUISlider::SetBGTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pBGTex);
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pBGTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void CKTDGUISlider::SetButtonTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pButtonTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pButtonTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}



//--------------------------------------------------------------------------------------
void CKTDGUISlider::SetValueInternal( int nValue, bool bFromInput )
{
	// Clamp to range
	nValue = __max( m_nMin, nValue );
	nValue = __min( m_nMax, nValue );

	if( nValue == m_nValue )
		return;

	m_nValue = nValue;
	UpdateRects();

	//m_pDialog->SendEvent( EVENT_SLIDER_VALUE_CHANGED, bFromInput, this );

	if( m_CustomMsgValueChanged != -1 )
	{
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgValueChanged, (LPARAM)this );
	}
	else
	{
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, SLIDER_VALUE_CHANGED, (LPARAM)this );
	}
}




void CKTDGUISlider::FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData& point )
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

#ifdef DLL_BUILD

bool CKTDGUISlider::IsSelectByEditGui( POINT pt )
{
	return ( PtInRect( &m_rcBoundingBox, pt ) || 
		PtInRect( &m_rcButton, pt ) ); 
}

void CKTDGUISlider::MoveControl( float fx, float fy )
{
	m_x += (int)fx;
	m_y += (int)fy;	

	UpdateRects();
}

void CKTDGUISlider::SetEditGUI( bool bEdit )
{
	SetColor( D3DXCOLOR(0xffffffff) );

	m_bUpdate = !bEdit;
	m_bEditEdge = bEdit;

	m_bPressed = false;
}

D3DXVECTOR2 CKTDGUISlider::GetPos()
{
	return D3DXVECTOR2((float)m_x, (float)m_y);
}

vector<D3DXVECTOR2> CKTDGUISlider::GetPosList()
{
	vector<D3DXVECTOR2> ret;

	ret.push_back( GetPos() );

	return ret;
}

void CKTDGUISlider::DrawEditEdge()
{
	if( false == m_bEditEdge )
		return;	

	if ( m_pCheckedEdgeTexture == NULL )
		return;	

	//const CKTDGUIControl::UIPointData & point = *m_pEditEdgePoint;
	D3DXCOLOR tempColor;

	int edgeWidth = 2;
	D3DXCOLOR edgeColor = D3DXCOLOR(0xffff0000);

	tempColor.a = edgeColor.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = edgeColor.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = edgeColor.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = edgeColor.b * m_pDialog->GetColor().b * m_Color.b;

		
	RECT edgeRect;
	UnionRect( &edgeRect, &m_rcButton, &m_rcBoundingBox );

// 	edgeRect.left = m_NowPoint.leftTopPoint.x;
// 	edgeRect.top = m_NowPoint.leftTopPoint.y;
// 	edgeRect.right = m_NowPoint.rightBottomPoint.x;
// 	edgeRect.bottom = m_NowPoint.rightBottomPoint.y;

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


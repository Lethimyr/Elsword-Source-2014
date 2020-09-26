#include "StdAfx.h"
#include ".\ktdguicheckbox.h"



CKTDGUICheckBox::CKTDGUICheckBox()
: m_bPressed( false )
{
	m_ControlType	= CKTDGUIControl::UCT_CHECKBOX;
	m_CheckBoxState	= CBCS_NORMAL;
	m_bChecked		= false;
	m_bEmptyBGWhenChecked	= false;

	m_pNormalTex	= NULL;
	m_pMouseOverTex	= NULL;
	m_pDisableTex	= NULL;
	m_pCheckedTex	= NULL;

	m_pBGPoint		= NULL;
	m_pCheckedPoint	= NULL;
	m_pSndCheck		= NULL;

	m_CustomMsgChecked		= -1;
	m_CustomMsgUnChecked	= -1;

	m_CustomFuncChecked		= "";
	m_CustomFuncUnChecked	= "";


//{{ robobeg : 2008-10-28
	//KLuaManager kLuaManager;
    KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28

	if(  g_pKTDXApp->LoadAndDoMemory( &kLuaManager, L"UI_Control_Sound.lua" ) == false )
	{
		return;
	}

	wstring checkSndFileName;

	LUA_GET_VALUE( kLuaManager, "CheckBox_Check", checkSndFileName, L"" );
	m_pSndCheck = g_pKTDXApp->GetDeviceManager()->OpenSound( checkSndFileName );

#ifdef DLL_BUILD
	m_pCheckedEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );
	m_bEditEdge = false;
#endif
}

CKTDGUICheckBox::~CKTDGUICheckBox(void)
{
	SAFE_DELETE( m_pNormalTex );
	SAFE_DELETE( m_pMouseOverTex );
	SAFE_DELETE( m_pDisableTex );
	SAFE_DELETE( m_pCheckedTex );

	SAFE_DELETE( m_pBGPoint );
	SAFE_DELETE( m_pCheckedPoint );

	SAFE_CLOSE( m_pSndCheck );

#ifdef DLL_BUILD
	SAFE_CLOSE( m_pCheckedEdgeTexture );
#endif
}

HRESULT CKTDGUICheckBox::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	if( m_bShow == false )
		return S_OK;

#ifdef DLL_BUILD
	if( m_bUpdate == false )
		return S_OK;
#endif

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	m_fElapsedTime	= fElapsedTime;
	if( m_EndPoint.fChangeTime != m_NowPoint.fChangeTime )
	{
		ChangePoint( &m_NowPoint, &m_EndPoint );
	}

	switch( m_CheckBoxState )
	{
		case CBCS_NORMAL:
			{
				if( m_bMouseOver == true )
					ChangeState( CBCS_MOUSEOVER );
				else if( m_bEnable == false )
					ChangeState( CBCS_DISABLE );
			}
			break;

		case CBCS_MOUSEOVER:
			{
				if( m_bMouseOver == false )
					ChangeState( CBCS_NORMAL );
				else if( m_bEnable == false )
					ChangeState( CBCS_DISABLE );
			}
			break;

		case CBCS_DISABLE:
			{
				if( m_bEnable == true )
					ChangeState( CBCS_NORMAL );
			}
			break;
	}

	return S_OK;
}

HRESULT CKTDGUICheckBox::OnFrameRender()
{
	KTDXPROFILE();
	HRESULT hr	= S_OK;

	if( m_bShow == false )
		return hr;
#ifdef DLL_BUILD
	DrawEditEdge( m_pCheckedPoint );
#endif
	if( false == m_bEmptyBGWhenChecked || false == m_bChecked )
	{
		//BG Draw
		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
			m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
			m_NowPoint );

		RenderVertex( m_Vertex, m_NowPoint );
	}

	//Checked Mark Draw
	if( m_bChecked == true )
	{
		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
					m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
					m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
					m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
					*m_pCheckedPoint );

		RenderVertex( m_Vertex, *m_pCheckedPoint );
	}

	return hr;
}

//--------------------------------------------------------------------------------------
bool CKTDGUICheckBox::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable == false || m_bShow == false )
		return false;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			if( ContainsPoint( pt ) )
			{
				// Pressed while inside the control
				m_bPressed = true;
#ifndef DLL_BUILD
				SetCapture( DXUTGetHWND() );
#endif

				if( !m_bHasFocus )
					m_pDialog->RequestFocus( this );

				return true;
			}

			break;
		}

	case WM_LBUTTONUP:
		{
			if( m_bPressed )
			{
				m_bPressed = false;
				ReleaseCapture();

				// Button click
				if( ContainsPoint( pt ) )
					SetCheckedInternal( !m_bChecked, true );

				return true;
			}

			break;
		}
	};

	return false;
}

void CKTDGUICheckBox::ChangeState( CHECKBOX_CONTROL_STATE state )
{	
	if( m_CheckBoxState == state )
		return;

	m_CheckBoxState = state;

	m_NowPoint = m_EndPoint = *m_pBGPoint;

	switch( m_CheckBoxState )
	{
		case CBCS_NORMAL:
			{
				m_NowPoint.pUITextureData	= m_pNormalTex;		
			}
			break;

		case CBCS_MOUSEOVER:
			{
				if( m_pMouseOverTex != NULL )
					m_NowPoint.pUITextureData	= m_pMouseOverTex;
				else
					m_NowPoint.pUITextureData	= m_pNormalTex;
			}
			break;

		case CBCS_DISABLE:
			{
				if( m_pDisableTex != NULL )
					m_NowPoint.pUITextureData	= m_pDisableTex;
				else
					m_NowPoint.pUITextureData	= m_pNormalTex;

				if( m_bHasFocus == true )
					m_pDialog->ClearFocus();
			}
			break;
	}
}

void CKTDGUICheckBox::SetNormalTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pNormalTex );

	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pNormalTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pBGPoint != NULL )
		m_pBGPoint->pUITextureData = m_pNormalTex;
}

void CKTDGUICheckBox::SetOverTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pMouseOverTex );

	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pMouseOverTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void CKTDGUICheckBox::SetDisableTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pDisableTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pDisableTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void CKTDGUICheckBox::SetCheckedTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pCheckedTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pCheckedTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pCheckedPoint != NULL )
		m_pCheckedPoint->pUITextureData = m_pCheckedTex;
}

void CKTDGUICheckBox::SetBGPoint_LUA()
{
	SAFE_DELETE( m_pBGPoint );
	m_pBGPoint = SetPointData_();
	m_pBGPoint->pUITextureData = m_pNormalTex;
	m_pBGPoint->SetAutoPointByTextureSize();
	m_NowPoint = *m_pBGPoint;
}

void CKTDGUICheckBox::SetCheckPoint_LUA()
{
	SAFE_DELETE( m_pCheckedPoint );
	m_pCheckedPoint = SetPointData_();
	m_pCheckedPoint->pUITextureData = m_pCheckedTex;
	m_pCheckedPoint->SetAutoPointByTextureSize();
}

void CKTDGUICheckBox::SetCheckedInternal( bool bChecked, bool bFromInput /*= false*/ )
{ 
	m_bChecked = bChecked; 

	if( true == bFromInput )
	{
		if( m_bChecked == true )
		{
			if( m_CustomMsgChecked != -1 )
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgChecked, (LPARAM)this );
			else
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_CHECKBOX_CHECKED, (LPARAM)this );

			if ( m_CustomFuncChecked.empty() == false )
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_FUNC, (WPARAM)m_CustomFuncChecked.c_str(), (LPARAM)this );

		}
		else
		{
			if( m_CustomMsgUnChecked != -1 )
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgUnChecked, (LPARAM)this );
			else
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_CHECKBOX_UNCHECKED, (LPARAM)this );

			if ( m_CustomFuncUnChecked.empty() == false )
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_FUNC, (WPARAM)m_CustomFuncUnChecked.c_str(), (LPARAM)this );
		}

		PlaySound( m_pSndCheck );
	}
}

#ifdef DLL_BUILD
void CKTDGUICheckBox::MoveControl( float fx, float fy )
{
	if( fx == 0.f && fy == 0.f )
		return;
	
	if( NULL != m_pBGPoint )
	{
		m_pBGPoint->Move( fx, fy );
		m_NowPoint = *m_pBGPoint;
		m_EndPoint = *m_pBGPoint;
	}

	if( NULL != m_pCheckedPoint )
	{
		m_pCheckedPoint->Move( fx, fy );
	}	
}

void CKTDGUICheckBox::MoveSubControl( float fx, float fy, wstring subControlName )
{
	if( NULL != m_pBGPoint && subControlName == L"BG" )
	{
		m_pBGPoint->Move( fx, fy );
		m_NowPoint = *m_pBGPoint;
	}

	else if( NULL != m_pCheckedPoint && subControlName == L"CheckPoint" )
	{
		m_pCheckedPoint->Move( fx, fy );
		m_NowPoint = *m_pCheckedPoint;
	}
}

void CKTDGUICheckBox::SetEditGUI( bool bEdit )
{
	m_bUpdate = !bEdit;

	if( true == bEdit )
	{
		m_bChecked = true;		// 편집할때는 CheckPoint상태로 보여지게 한다.
	}

	m_bEditEdge = bEdit;
	SetColor(D3DXCOLOR(0xffffffff));	// 활성할때 Edge(외곽선) 생기므로 활성화 색깔을 초기화한다.
}

void CKTDGUICheckBox::ShowSubView( wstring	name, bool bView )
{
	SetColor(D3DXCOLOR(0xffffffff));		// 하위 컨트롤을 선택했을때 edge(테두리)를 보여주기 위해 색을 해제한다.

	if( true == bView )
	{
		if( name == L"BG" && NULL != m_pBGPoint )
			m_NowPoint = *m_pBGPoint;		
		else if( name == L"CheckPoint" && NULL != m_pCheckedPoint )
			m_NowPoint = *m_pCheckedPoint;
	}
	else
	{
		if( NULL != m_pBGPoint )
			m_NowPoint = *m_pBGPoint;
	}
}

D3DXVECTOR2 CKTDGUICheckBox::GetPos()
{
	if( NULL != m_pBGPoint )
		return m_pBGPoint->leftTopPoint;

	return D3DXVECTOR2(0, 0);
}

vector<D3DXVECTOR2> CKTDGUICheckBox::GetPosList()
{
	vector<D3DXVECTOR2> ret;

	if( NULL != m_pBGPoint )
		ret.push_back(m_pBGPoint->leftTopPoint);

	if( NULL != m_pCheckedPoint )
		ret.push_back(m_pCheckedPoint->leftTopPoint);

	return ret;
}

void CKTDGUICheckBox::DrawEditEdge( CKTDGUIControl::UIPointData* m_pEditEdgePoint )
{
	if( false == m_bEditEdge )
		return;

	if(NULL == m_pEditEdgePoint) 
		return;

	if ( m_pCheckedEdgeTexture == NULL )
		return;


	const CKTDGUIControl::UIPointData & point = *m_pEditEdgePoint;
	D3DXCOLOR tempColor;

	int edgeWidth = 4;
	D3DXCOLOR edgeColor = D3DXCOLOR(0xffff0000);

	tempColor.a = edgeColor.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = edgeColor.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = edgeColor.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = edgeColor.b * m_pDialog->GetColor().b * m_Color.b;


	int _width = (int)(point.rightBottomPoint.x - point.leftTopPoint.x);
	int _height = (int)(point.rightBottomPoint.y - point.leftTopPoint.y);

	//if ( m_bDrawEdgeOut == true )
	{
		// 좌
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + point.leftTopPoint.x - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + point.leftTopPoint.y - edgeWidth), 
			edgeWidth , 
			_height + edgeWidth, 
			tempColor );

		// 하

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + point.leftTopPoint.x - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + point.leftBottomPoint.y ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );

		// 우

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + point.rightTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + point.leftTopPoint.y ), 
			edgeWidth, 
			_height + edgeWidth, 
			tempColor );

		// 상

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + point.leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + point.leftTopPoint.y - edgeWidth ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );
	}
}

#endif

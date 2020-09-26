#include "StdAfx.h"
#include ".\KTDGUIControlList.h"

#ifdef NEW_SKILL_TREE_UI

CKTDGUIContolList::CKTDGUIContolList() :
m_pXScollBarUpButtonTex(NULL),
m_pXScollBarUpButtonDownTex(NULL),
m_pXScollBarDownButtonTex(NULL),
m_pXScollBarDownButtonDownTex(NULL),
m_pXScollBarTrackTex(NULL),
m_pXScollBarThumbTex(NULL),
m_pYScollBarUpButtonTex(NULL),
m_pYScollBarUpButtonDownTex(NULL),
m_pYScollBarDownButtonTex(NULL),
m_pYScollBarDownButtonDownTex(NULL),
m_pYScollBarTrackTex(NULL),
m_pYScollBarThumbTex(NULL),
m_iTopIndexX(0),
m_iTopIndexY(0),
m_iMaxIndexX(0),
m_iMaxIndexY(0),
m_iViewSizeX(1),
m_iViewSizeY(1),
m_iScrollOffsetX(1),
m_iScrollOffsetY(1),
m_CustomMsgScroll(-1),
m_Pos(0, 0),
m_pScrollBarX(NULL),
m_pScrollBarY(NULL)
, m_rcBoundingBox( RECT() )
{
	m_ControlType = UCT_CONTROLLIST;

#ifdef DLL_BUILD
	m_pCheckedEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );
	m_bEditEdge = false;
#endif
}

CKTDGUIContolList::~CKTDGUIContolList(void)
{
// 	for(ControlVectorMap::iterator it = m_mapControls.begin(); it != m_mapControls.end(); ++it)
// 	{
// 		ControlVector vecControl = it->second;
// 		for(ControlVector::iterator vecit = vecControl.begin(); vecit != vecControl.end(); ++ vecit)
// 		{
// 			CKTDGUIControl* pControl = *vecit;
// 			SAFE_DELETE( pControl );
// 			pControl = NULL;
// 		}	
// 	}



	m_mapControls.clear();
	SAFE_DELETE(m_pScrollBarX);
	SAFE_DELETE(m_pScrollBarY);


	SAFE_DELETE( m_pXScollBarUpButtonTex );
	SAFE_DELETE( m_pXScollBarUpButtonDownTex );
	SAFE_DELETE( m_pXScollBarDownButtonTex );
	SAFE_DELETE( m_pXScollBarDownButtonDownTex );
	SAFE_DELETE( m_pXScollBarTrackTex );
	SAFE_DELETE( m_pXScollBarThumbTex );

	SAFE_DELETE( m_pYScollBarUpButtonTex );
	SAFE_DELETE( m_pYScollBarUpButtonDownTex );
	SAFE_DELETE( m_pYScollBarDownButtonTex );
	SAFE_DELETE( m_pYScollBarDownButtonDownTex );
	SAFE_DELETE( m_pYScollBarTrackTex );
	SAFE_DELETE( m_pYScollBarThumbTex );

#ifdef DLL_BUILD
	SAFE_CLOSE( m_pCheckedEdgeTexture );
#endif
}

void CKTDGUIContolList::SetShow(bool bShow, Row row /*= -1*/, Col col /*= -1*/ )
{
	for( ControlVectorMap::iterator Mapit = m_mapControls.begin(); Mapit != m_mapControls.end(); ++Mapit )
	{
		std::pair<Row, Col> key = Mapit->first;
		Row currentRow = key.first;
		Col currentCol = key.second;

		if( ( row == -1 || row == currentRow ) &&
			( col == -1 || col == currentCol ) )
		{
			ControlVector& vecOrgContols = Mapit->second;

			for(ControlVector::iterator it = vecOrgContols.begin(); it != vecOrgContols.end(); ++it )
			{
				ControlInfo& refControlInfo = *it;
				CKTDGUIControl* pControl = refControlInfo.m_pControl;
				pControl->SetShow(bShow);
			}
		}
	}
}

void CKTDGUIContolList::SetEnable(bool bEnable, Row row /*= -1*/, Col col /*= -1*/ )
{
	for( ControlVectorMap::iterator Mapit = m_mapControls.begin(); Mapit != m_mapControls.end(); ++Mapit )
	{
		std::pair<Row, Col> key = Mapit->first;
		Row currentRow = key.first;
		Col currentCol = key.second;

		if( ( row == -1 || row == currentRow ) &&
			( col == -1 || col == currentCol ) )
		{
			ControlVector& vecOrgContols = Mapit->second;

			for(ControlVector::iterator it = vecOrgContols.begin(); it != vecOrgContols.end(); ++it )
			{
				ControlInfo& refControlInfo = *it;
				CKTDGUIControl* pControl = refControlInfo.m_pControl;
				pControl->SetEnable(bEnable);
			}
		}
	}

}

void CKTDGUIContolList::SetViewSize(int X, int Y, bool bUpdateControl /*= true*/ )
{
	m_iViewSizeX = X;
	m_iViewSizeY = Y;
	UpdateScrollBar();
	if(bUpdateControl)
	{
		UpdateRenderControlList();
	}
	SetRect( &m_rcBoundingBox, (int)m_Pos.x, (int)m_Pos.y, (int)(m_Pos.x + m_iScrollOffsetX*m_iViewSizeX), (int)(m_Pos.y + m_iScrollOffsetY*m_iViewSizeY) );
}

void CKTDGUIContolList::SetScrollOffset(int X, int Y)
{
	m_iScrollOffsetX = X;
	m_iScrollOffsetY = Y; 
	SetRect( &m_rcBoundingBox, (int)m_Pos.x, (int)m_Pos.y, (int)(m_Pos.x + m_iScrollOffsetX*m_iViewSizeX), (int)(m_Pos.y + m_iScrollOffsetY*m_iViewSizeY) );
}

void CKTDGUIContolList::SetIndex(int X, int Y, bool bUpdateControl /*= true*/, bool bSendMessage /*= true*/ )
{
	m_iTopIndexX = X;
	m_iTopIndexY = Y;
	UpdateScrollBar();
	if(bUpdateControl)
	{
		UpdateRenderControlList();
	}
	if( true == bSendMessage && m_CustomMsgScroll != -1 )
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgScroll, (LPARAM)this );
	

}

void CKTDGUIContolList::UpdateRenderControlList()
{
	m_vecNowControls.clear();
	for( ControlVectorMap::iterator Mapit = m_mapControls.begin(); Mapit != m_mapControls.end(); ++Mapit )
	{
		bool bInView = false;
		std::pair<Row, Col> key = Mapit->first;
		int row = key.first;
		int col = key.second;
		if( m_iTopIndexX <= col && col < m_iTopIndexX+m_iViewSizeX &&
			m_iTopIndexY <= row && row < m_iTopIndexY+m_iViewSizeY )
		{
			bInView = true;
		}		
		
		ControlVector& vecOrgContols = Mapit->second;

		for(ControlVector::iterator it = vecOrgContols.begin(); it != vecOrgContols.end(); ++it )
		{
			ControlInfo& refControlInfo = *it;
			CKTDGUIControl* pControl = refControlInfo.m_pControl;
			if(bInView)
			{
				if(refControlInfo.m_bUseAbsolutePos)
				{
					pControl->SetOffsetPos( m_Pos + D3DXVECTOR2( (float)(-m_iTopIndexX*m_iScrollOffsetX), (float)(-m_iTopIndexY*m_iScrollOffsetY )) );
				}
				else
				{
					pControl->SetOffsetPos( m_Pos + D3DXVECTOR2( (float)((col - m_iTopIndexX)*m_iScrollOffsetX), (float)((row-m_iTopIndexY)*m_iScrollOffsetY )) );
				}				
				pControl->SetOutOfControlList(false);
				m_vecNowControls.push_back(pControl);
				
			}
			else
			{
				pControl->SetOutOfControlList(true);
			}
		}
	}

}

/*virtual*/ HRESULT	CKTDGUIContolList::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bShow == false || m_bEnable == false )
	{
		return S_OK;
	}

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	int indexX, indexY;
	if(m_pScrollBarX != NULL)
	{
		m_pScrollBarX->OnFrameMove( fTime, fElapsedTime );
		indexX = m_pScrollBarX->GetTrackPos();
	}
	else
	{
		indexX = 0;
	}
	if(m_pScrollBarY != NULL)
	{
		m_pScrollBarY->OnFrameMove( fTime, fElapsedTime );
		indexY = m_pScrollBarY->GetTrackPos();
	}
	else
	{
		indexY = 0;
	}
	if(m_iTopIndexX != indexX || m_iTopIndexY != indexY )
	{
		SetIndex(indexX, indexY);
	}
	

	

	RECT tempRECT;

	tempRECT = m_rcBoundingBox;
	if ( m_pScrollBarY != NULL )
	{		
		tempRECT.left += m_pScrollBarY->GetWidth();
		tempRECT.right += m_pScrollBarY->GetWidth();
	}
	if ( m_pScrollBarX != NULL )
	{		
		tempRECT.top += m_pScrollBarX->GetWidth();
		tempRECT.bottom += m_pScrollBarX->GetWidth();
	}

	FromRectToPoint( tempRECT, m_NowPoint );

	
	

// 	for ( int i = (int)m_vecNowControls.size() - 1; i >= 0 ; i-- )
// 	{
// 		CKTDGUIControl* pControl = m_vecNowControls[i];
// 		pControl->OnFrameMove( fTime, fElapsedTime );
// 	}
	
	return S_OK;    
}

/*virtual*/ HRESULT CKTDGUIContolList::OnFrameRender()
{
// 	for ( int i = 0; i < (int)m_vecNowControls.size(); i++ )
// 	{
// 		CKTDGUIControl* pControl = m_vecNowControls[i];
// 		pControl->OnFrameRender();
// 	}
	if( m_bShow == false )
		return S_OK;


#ifdef DLL_BUILD
	DrawEditEdge( &m_NowPoint );
#endif // DLL_BUILD

	// Render the scroll bar
	if ( m_pScrollBarX != NULL )
	{
		m_pScrollBarX->OnFrameRender();
	}
	if ( m_pScrollBarY != NULL )
	{
		m_pScrollBarY->OnFrameRender();
	}


	return S_OK;    	
}

/*virtual*/	HRESULT	CKTDGUIContolList::OnFrameRenderEnd()
{
// 	for ( int i = 0; i < (int)m_vecNowControls.size(); i++ )
// 	{
// 		CKTDGUIControl* pControl = m_vecNowControls[i];
// 		pControl->OnFrameRenderEnd();
// 	}
	return S_OK;    
}

/*virtual*/	bool CKTDGUIContolList::ContainsPoint( POINT pt )
{
 	if( true == Pick2DRect( pt, m_NowPoint.leftTopPoint, m_NowPoint.rightTopPoint, m_NowPoint.leftBottomPoint ,m_NowPoint.rightBottomPoint ) )
 		return true;

	if(m_pScrollBarX != NULL)
	{
		RECT rect = m_pScrollBarX->GetBoundingBox();
		D3DXVECTOR2 vLeftTop( (float) rect.left, (float) rect.top );
		D3DXVECTOR2 vRightTop( (float) rect.right, (float) rect.top );
		D3DXVECTOR2 vLeftBottom( (float) rect.left, (float) rect.bottom );
		D3DXVECTOR2 vRightBottom( (float) rect.right, (float) rect.bottom );
		if( true == Pick2DRect( pt, vLeftTop, vRightTop, vLeftBottom, vRightBottom ) )
			return true;
	}
	if(m_pScrollBarY != NULL)
	{
		RECT rect = m_pScrollBarY->GetBoundingBox();
		D3DXVECTOR2 vLeftTop( (float) rect.left, (float) rect.top );
		D3DXVECTOR2 vRightTop( (float) rect.right, (float) rect.top );
		D3DXVECTOR2 vLeftBottom( (float) rect.left, (float) rect.bottom );
		D3DXVECTOR2 vRightBottom( (float) rect.right, (float) rect.bottom );
		if( true == Pick2DRect( pt, vLeftTop, vRightTop, vLeftBottom, vRightBottom ) )
			return true;
	}
	

	return false;
}

/*virtual*/ void CKTDGUIContolList::UpdateRects()
{
	// ListViewOffset이랑 Size 이용해서 범위 만들어 주고 그걸 ContainsPoint에서 이용하게 하자..

	SetRect( &m_rcBoundingBox, (int)m_Pos.x, (int)m_Pos.y, (int)(m_Pos.x + m_iScrollOffsetX*m_iViewSizeX), (int)(m_Pos.y + m_iScrollOffsetY*m_iViewSizeY) );

}

/*virtual*/ bool CKTDGUIContolList::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return false;
}

/*virtual*/ bool CKTDGUIContolList::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();

	if ( m_bShow == false || m_bEnable == false )
	{
		return false;
	}

	// First acquire focus
// 	if( WM_LBUTTONDOWN == uMsg )
// 		if( !m_bHasFocus )
// 			m_pDialog->RequestFocus( this );

	// Let the scroll bar handle it first.
	if( m_pScrollBarX != NULL  )
	{
		if ( m_pScrollBarX->HandleMouse( uMsg, pt, wParam, lParam ) == true )
		{
			
			return true;
		}
	}

	if( m_pScrollBarY != NULL  )
	{
		if ( m_pScrollBarY->HandleMouse( uMsg, pt, wParam, lParam ) == true )
		{

			return true;
		}
	}

	// 나머지 우선순위대로 업데이트
	return false;

}

/*virtual*/ bool CKTDGUIContolList::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return false;
}

// ControlVector& CKTDGUIContolList::GetControlVector( int row, int col )
// {
// 	ControlVectorMap::iterator it = m_mapControls.find(std::make_pair(row, col));
// 	if( it != m_mapControls.end())
// 	{
// 		return it->second;
// 	}
// 	return emptyvector;
// }

bool CKTDGUIContolList::GetControlVector( int row, int col, ControlVector& vecControl )
{
	ASSERT( vecControl.empty() );

	ControlVectorMap::iterator it = m_mapControls.find(std::make_pair(row, col));
	if( it != m_mapControls.end())
	{
		const ControlVector& vecOrgContols = it->second;
		copy(vecOrgContols.begin(), vecOrgContols.end(), vecControl.begin());
		return true;
	}

	return false;
}

bool CKTDGUIContolList::AddControlVector( int row, int col, ControlVector& vecControl )
{
	ControlVectorMap::iterator it = m_mapControls.find(std::make_pair(row, col));
	if( it != m_mapControls.end())
	{
		ControlVector& vecOrgContols = it->second;
		
		for(ControlVector::iterator it = vecOrgContols.begin(); it != vecOrgContols.end(); ++it )
		{
			ControlInfo& refControlInfo = *it;
			ControlInfo newControlInfo(refControlInfo);
						
			vecControl.push_back(newControlInfo);
		}
		return true;
	}

	return false;
}

bool CKTDGUIContolList::InsertItem( CKTDGUIControl* pControl, int row, int col, bool bUseAbsolutePosition /*= false*/)
{
	ControlVectorMap::iterator mapit = m_mapControls.find(std::make_pair(row,col));
	if( mapit != m_mapControls.end() )
	{
		ControlVector& vec = mapit->second;
		
		ControlInfo controlinfo;
		controlinfo.m_bUseAbsolutePos = bUseAbsolutePosition;
		controlinfo.m_pControl = pControl;
		vec.push_back(controlinfo);
	}
	else
	{
		ControlVector vec;
		ControlInfo controlinfo;
		controlinfo.m_bUseAbsolutePos = bUseAbsolutePosition;
		controlinfo.m_pControl = pControl;
		vec.push_back(controlinfo);
		m_mapControls.insert( std::make_pair( std::make_pair(row,col), vec ));

	}
	if(col > m_iMaxIndexX)
	{
		m_iMaxIndexX = col;
	}
	if(row > m_iMaxIndexY)
	{
		m_iMaxIndexY = row;
	}
	
	if(pControl->GetDialog() == NULL)
	{
		m_pDialog->AddControl( pControl );
	}
	else if(pControl->GetDialog() != m_pDialog)
	{
		ASSERT( !"Insert Control to ControlList in different dialog!" );
	}
	return false;
}


void	CKTDGUIContolList::SetXScollBarUpButtonTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pXScollBarUpButtonTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pXScollBarUpButtonTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIContolList::SetXScollBarUpButtonDownTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pXScollBarUpButtonDownTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pXScollBarUpButtonDownTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIContolList::SetXScollBarDownButtonTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pXScollBarDownButtonTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pXScollBarDownButtonTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIContolList::SetXScollBarDownButtonDownTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pXScollBarDownButtonDownTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pXScollBarDownButtonDownTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIContolList::SetXScollBarTrackTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pXScollBarTrackTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pXScollBarTrackTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIContolList::SetXScollBarThumbTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pXScollBarThumbTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pXScollBarThumbTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}




void	CKTDGUIContolList::SetYScollBarUpButtonTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pYScollBarUpButtonTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pYScollBarUpButtonTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIContolList::SetYScollBarUpButtonDownTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pYScollBarUpButtonDownTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pYScollBarUpButtonDownTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIContolList::SetYScollBarDownButtonTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pYScollBarDownButtonTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pYScollBarDownButtonTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIContolList::SetYScollBarDownButtonDownTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pYScollBarDownButtonDownTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pYScollBarDownButtonDownTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIContolList::SetYScollBarTrackTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pYScollBarTrackTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pYScollBarTrackTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIContolList::SetYScollBarThumbTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pYScollBarThumbTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pYScollBarThumbTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}




CKTDGUIScrollBar* CKTDGUIContolList::InitXScrollBar_LUA()
{
	SAFE_DELETE(m_pScrollBarX);
	m_pScrollBarX = new CKTDGUIScrollBar();

	m_pScrollBarX->SetDialog( m_pDialog );

	m_pScrollBarX->SetDownButtonTex( m_pXScollBarDownButtonTex );
	m_pScrollBarX->SetUpButtonTex( m_pXScollBarUpButtonTex );
	m_pScrollBarX->SetDownButtonDownTex( m_pXScollBarDownButtonDownTex );
	m_pScrollBarX->SetUpButtonDownTex( m_pXScollBarUpButtonDownTex );
	m_pScrollBarX->SetTrackTex( m_pXScollBarTrackTex );
	m_pScrollBarX->SetThumbTex( m_pXScollBarThumbTex );

	return m_pScrollBarX;
}

CKTDGUIScrollBar* CKTDGUIContolList::InitYScrollBar_LUA()
{
	SAFE_DELETE(m_pScrollBarY);
	m_pScrollBarY = new CKTDGUIScrollBar();

	m_pScrollBarY->SetDialog( m_pDialog );

	m_pScrollBarY->SetDownButtonTex( m_pYScollBarDownButtonTex );
	m_pScrollBarY->SetUpButtonTex( m_pYScollBarUpButtonTex );
	m_pScrollBarY->SetDownButtonDownTex( m_pYScollBarDownButtonDownTex );
	m_pScrollBarY->SetUpButtonDownTex( m_pYScollBarUpButtonDownTex );
	m_pScrollBarY->SetTrackTex( m_pYScollBarTrackTex );
	m_pScrollBarY->SetThumbTex( m_pYScollBarThumbTex );

	return m_pScrollBarY;
}

void CKTDGUIContolList::UpdateScrollBar()
{
	if(m_pScrollBarX != NULL)
	{
		if(m_iMaxIndexX < m_iViewSizeX)
		{
			// oasis907 : 김상윤 [2009.12.2] // ControlList도 ListBox처럼 리스트가 창을 넘지 않아도 스크롤 버튼이 생길 수 있도록
			//m_pScrollBarX->SetShowEnable(false, false);
		}
		else
		{
			m_pScrollBarX->SetShowEnable(true, true);
			m_pScrollBarY->SetTrackPos( m_iTopIndexX );
			m_pScrollBarX->SetTrackRange( 0, m_iMaxIndexX-m_iViewSizeX+2 );
		}
		
	}

	if(m_pScrollBarY != NULL)
	{
		if(m_iMaxIndexY < m_iViewSizeY)
		{
			// oasis907 : 김상윤 [2009.12.2] // ControlList도 ListBox처럼 리스트가 창을 넘지 않아도 스크롤 버튼이 생길 수 있도록
			//m_pScrollBarY->SetShowEnable(false, false);
		}
		else
		{
			m_pScrollBarY->SetShowEnable(true, true);
			m_pScrollBarY->SetTrackPos( m_iTopIndexY );
			m_pScrollBarY->SetTrackRange( 0, m_iMaxIndexY-m_iViewSizeY+2 );
		}
		
	}

}

void CKTDGUIContolList::FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData& point )
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
void CKTDGUIContolList::SetColor( D3DXCOLOR color )
{
	__super::SetColor(color);

	// 내부에 스크롤 바의 색깔 변경
	if( NULL != m_pScrollBarY )
		m_pScrollBarY->SetColor(color);
	if( NULL != m_pScrollBarX )
		m_pScrollBarX->SetColor(color);
}
void CKTDGUIContolList::MoveControl( float fx, float fy )
{
	// ControlList 위치를 이동
	m_Pos.x += fx;
	m_Pos.y += fy;
	UpdateRects();

	// 내부에 스크롤 바 위치를 이동
	if( NULL != m_pScrollBarY )
		m_pScrollBarY->MoveControl( fx, fy );
	if( NULL != m_pScrollBarX )
		m_pScrollBarX->MoveControl( fx, fy );
}

void CKTDGUIContolList::MoveSubControl( float fx, float fy, wstring subControlName )
{
	if( subControlName == L"BackGround" )
	{
		m_Pos.x += fx;
		m_Pos.y += fy;
		UpdateRects();
	}
	else if( subControlName == L"ScrollBar" )
	{
		if( NULL != m_pScrollBarY )
			m_pScrollBarY->MoveControl( fx, fy );
		if( NULL != m_pScrollBarX )
			m_pScrollBarX->MoveControl( fx, fy );
	}
}

void CKTDGUIContolList::SetEditGUI( bool bEdit )
{
	m_bUpdate = !bEdit;		// update 멈춘다.
	m_bEditEdge = bEdit;	// 외각사각형을 그린다.


	if( NULL != m_pScrollBarY )
		m_pScrollBarY->SetEditGUI( bEdit );
	else if( NULL != m_pScrollBarX )
		m_pScrollBarX->SetEditGUI( bEdit );
}

void CKTDGUIContolList::ShowSubView( wstring name, bool bView )
{	
	m_bEditEdge = false;

	if( NULL != m_pScrollBarY )
		m_pScrollBarY->ShowEdge( false );
	else if( NULL != m_pScrollBarX )
		m_pScrollBarX->ShowEdge( false );


	if( name == L"BackGround" )
	{
		__super::SetColor(D3DXCOLOR(0xffff0000));
		m_bEditEdge = true;
	}
	else if( name == L"ScrollBar" )
	{
		if( NULL != m_pScrollBarY )
			m_pScrollBarY->ShowEdge( bView );
		else if( NULL != m_pScrollBarX )
			m_pScrollBarX->ShowEdge( bView );
	}
}

vector<D3DXVECTOR2> CKTDGUIContolList::GetPosList()
{
	vector<D3DXVECTOR2> ret;

	ret.push_back(D3DXVECTOR2(m_Pos));

	if( NULL != m_pScrollBarY )
		ret.push_back( m_pScrollBarY->GetPos() );
	else if( NULL != m_pScrollBarX )
		ret.push_back(m_pScrollBarX->GetPos());

	return ret;
}

D3DXVECTOR2 CKTDGUIContolList::GetPos()
{
	return GetPos(L"BackGround");
}

D3DXVECTOR2 CKTDGUIContolList::GetPos(wstring name)
{
	if( name == L"BackGround" )
		return m_Pos;
	else if( name == L"ScrollBar" )
	{
		if( NULL != m_pScrollBarY )
			return m_pScrollBarY->GetPos();
		else if( NULL != m_pScrollBarX )
			return m_pScrollBarX->GetPos();
	}

	return D3DXVECTOR2();
}

void CKTDGUIContolList::DrawEditEdge( CKTDGUIControl::UIPointData* m_pEditEdgePoint )
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
	D3DXCOLOR edgeColor = D3DXCOLOR(0xffffffff);

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


#endif
#include "dxstdafx.h"
#include ".\texuvout.h"

using std::ios;

CTexUVOut::CTexUVOut(void)
{
	OpenScriptFile( L"Enum.lua" );
	OpenScriptFile( L"Config.lua" );

	m_pFont = g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewFont( L"돋움", 15, 0, FW_BOLD );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->SetFont( 0, m_pFont );
	m_pDialog = NULL;
/*
	wstring datPath = L"dat/";
	datPath += L"rect.tga";
	m_WiredRect.CreateTexture( datPath.c_str() );
*/
	m_pDeviceTextureRect = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );

	m_pIMEEditBox = NULL;

	m_SampleUI.Init( &g_DialogResourceManager ); 

	m_SampleUI.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	m_SampleUI.SetFont( 2, L"Courier New", 16, FW_NORMAL );

	CDXUTIMEEditBox *pIMEEdit;
	if( SUCCEEDED( m_SampleUI.AddIMEEditBox( IDC_EDITBOX1, L"", 20, 390 + 73, 203, 45, false, &pIMEEdit ) ) )
	{
		pIMEEdit->GetElement( 0 )->iFont = 1;
		pIMEEdit->GetElement( 1 )->iFont = 1;
		pIMEEdit->GetElement( 9 )->iFont = 1;
		pIMEEdit->GetElement( 0 )->TextureColor.Init( D3DCOLOR_ARGB( 128, 255, 255, 255 ) );  // Transparent center
		pIMEEdit->SetBorderWidth( 7 );
		pIMEEdit->SetTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		pIMEEdit->SetCaretColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
		pIMEEdit->SetSelectedTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		pIMEEdit->SetSelectedBackColor( D3DCOLOR_ARGB( 255, 40, 72, 72 ) );
	}

	m_pIMEEditBox = pIMEEdit;

	ReInit();
}

CTexUVOut::~CTexUVOut(void)
{
	SAFE_DELETE_DIALOG( m_pDialog );
	g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyFont( m_pFont );

	for ( int i = 0; i < (int)m_TETDataList.size(); i++ )
	{
		TET_DATA* pTetData = m_TETDataList[i];
		SAFE_DELETE( pTetData );
	}
	m_TETDataList.clear();

	SAFE_CLOSE( m_pDeviceTextureRect );
}

void CTexUVOut::ReInit()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pKTDXApp->SetWorldTransform( &matWorld );
	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-700, 0,0,0 );
	g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix() );
	//g_pKTDXApp->GetDevice()->SetTransform( D3DTS_VIEW, (D3DXMATRIX*) &g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix() );
	g_pKTDXApp->GetDGManager()->SetProjection( 0.01f, 12000.f, false );

	//g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDialog, NULL, false );
	SAFE_DELETE_DIALOG( m_pDialog );
	m_pDialog = new CKTDGUIDialog( this, L"DLG_TexUVOut.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDialog );

#if 0
	m_RenderStateID = CKTDGStateManager::RecordStateBegin();
	{
		CKTDGStateManager::CaptureRenderState( D3DRS_ZENABLE,			FALSE );
		CKTDGStateManager::CaptureRenderState( D3DRS_ZWRITEENABLE,		FALSE );
		CKTDGStateManager::CaptureRenderState( D3DRS_CULLMODE,			D3DCULL_CW );
	}
	CKTDGStateManager::RecordStateEnd();
#endif


	m_EditMode = EM_POINT;

	m_bLoadTexture = false;
	m_LoadedTextureName = L"";

	m_bRButtonDragging = false;

	m_NowPointType = LEFT_TOP;

	m_sLinePos.bIsDraw	= false;
	m_bDraggingRectAuto = false;

	m_vPos = D3DXVECTOR2( 0 , 0 );
	m_vTextureScale = D3DXVECTOR2( 1, 1);
	m_vTextureOrgScale = D3DXVECTOR2( 0,0 );
	m_bLButtonDown = false;

	m_LineColor = 0xff000000;

	m_rectAutoRect.bottom = 0;
	m_rectAutoRect.left = 0;
	m_rectAutoRect.right = 0;
	m_rectAutoRect.top = 0;

	for ( int i = 0; i < 4; i++ )
	{
		m_vUVTexCoord[i].x = 0.0f;
		m_vUVTexCoord[i].y = 0.0f;
	}

	for ( int i = 0; i < (int)m_TETDataList.size(); i++ )
	{
		TET_DATA* pTetData = m_TETDataList[i];
		SAFE_DELETE( pTetData );
	}
	m_TETDataList.clear();
}

HRESULT CTexUVOut::OnFrameMove( double fTime, float fElapsedTime )
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );

	if ( m_bLoadTexture == true )
	{
		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_Q ) == TRUE )
		{
			m_LineColor = 0xff000000;
		}

		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_W ) == TRUE )
		{
			m_LineColor = 0xfffffffe;
		}

		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_E ) == TRUE )
		{
			m_LineColor = 0xffff0000;
		}

		if ( m_EditMode == EM_POINT )
		{
			EditPoint();
		

			D3DXMATRIX matWorld;
			D3DXMatrixIdentity(&matWorld);
			g_pKTDXApp->SetWorldTransform( &matWorld );
			g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-700, 0,0,0 );
			g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix() );
			//g_pKTDXApp->GetDevice()->SetTransform( D3DTS_VIEW, (D3DXMATRIX*) &g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix() );
			g_pKTDXApp->GetDGManager()->SetProjection( 0.01f, 12000.f, false );


			m_TexturedRectangle.SetScale( 1.0f );
			m_WiredRect.SetScale(  1.0f );
			m_OutlineLeftRect.SetScale(  1.0f );
			m_OutlineTopRect.SetScale(  1.0f );
			m_OutlineRightRect.SetScale(  1.0f );
			m_OutlineBottomRect.SetScale(  1.0f );
		}
		else if ( m_EditMode == EM_RECTANGLE_AUTO )
		{
			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F5 ) == TRUE )
			{
				m_vTextureScale = D3DXVECTOR2( 1, 1 );
			}

			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_LSHIFT ) == TRUE )
			{
				if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_LEFTARROW ) == TRUE )
				{
					m_rectAutoRect.left -= 1;
					m_rectAutoRect.right -= 1;
				}

				if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_RIGHTARROW ) == TRUE )
				{
					m_rectAutoRect.left += 1;
					m_rectAutoRect.right += 1;
				}

				if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_UPARROW ) == TRUE )
				{
					m_rectAutoRect.top -= 1;
					m_rectAutoRect.bottom -= 1;
				}	

				if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_DOWNARROW ) == TRUE )
				{
					m_rectAutoRect.top += 1;
					m_rectAutoRect.bottom += 1;
				}

				if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_NUMPAD2 ) == TRUE )
				{
					m_rectAutoRect.bottom += 1;
				}

				if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_NUMPAD4 ) == TRUE )
				{
					m_rectAutoRect.right -= 1;
				}

				if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_NUMPAD8 ) == TRUE )
				{
					m_rectAutoRect.bottom -= 1;
				}

				if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( DIK_NUMPAD6 ) == TRUE )
				{
					m_rectAutoRect.right += 1;
				}
			}

			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_LEFTARROW ) == TRUE )
			{
				m_rectAutoRect.left -= 1;
				m_rectAutoRect.right -= 1;
			}

			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_RIGHTARROW ) == TRUE )
			{
				m_rectAutoRect.left += 1;
				m_rectAutoRect.right += 1;
			}

			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_UPARROW ) == TRUE )
			{
				m_rectAutoRect.top -= 1;
				m_rectAutoRect.bottom -= 1;
			}	

			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_DOWNARROW ) == TRUE )
			{
				m_rectAutoRect.top += 1;
				m_rectAutoRect.bottom += 1;
			}

			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_NUMPAD2 ) == TRUE )
			{
				m_rectAutoRect.bottom += 1;
			}

			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_NUMPAD4 ) == TRUE )
			{
				m_rectAutoRect.right -= 1;
			}

			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_NUMPAD8 ) == TRUE )
			{
				m_rectAutoRect.bottom -= 1;
			}

			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_NUMPAD6 ) == TRUE )
			{
				m_rectAutoRect.right += 1;
			}
		}
	}

	//m_WiredRect.SetVertexPos( m_NowPointType, vMouseWorld );
	if ( m_bLoadTexture == true )
	{


		if ( m_EditMode == EM_POINT )
		{

			D3DXVECTOR3 vertex[4];

			for ( int i = 0; i < 4; i++ )
			{
				vertex[i] = m_WiredRect.GetVertexPos(i);
			}

			D3DXVECTOR3 specialVertex;
			specialVertex.z;

			specialVertex = vertex[0];
			specialVertex.x -= 1;
			m_OutlineLeftRect.SetVertexPos( 0, specialVertex, m_LineColor  );
			specialVertex = vertex[0];
			m_OutlineLeftRect.SetVertexPos( 1, specialVertex, m_LineColor  );
			specialVertex = vertex[2];
			specialVertex.x -= 1;
			m_OutlineLeftRect.SetVertexPos( 2, specialVertex, m_LineColor );
			specialVertex = vertex[2];
			m_OutlineLeftRect.SetVertexPos( 3, specialVertex, m_LineColor );


			specialVertex = vertex[0];
			specialVertex.y += 1;
			m_OutlineTopRect.SetVertexPos( 0, specialVertex, m_LineColor  );
			specialVertex = vertex[1];
			specialVertex.y += 1;
			m_OutlineTopRect.SetVertexPos( 1, specialVertex, m_LineColor  );
			specialVertex = vertex[0];
			m_OutlineTopRect.SetVertexPos( 2, specialVertex, m_LineColor );
			specialVertex = vertex[1];
			m_OutlineTopRect.SetVertexPos( 3, specialVertex, m_LineColor );


			specialVertex = vertex[1];
			//specialVertex.x += 1;
			m_OutlineRightRect.SetVertexPos( 0, specialVertex, m_LineColor  );
			specialVertex = vertex[1];
			specialVertex.x += 1;
			m_OutlineRightRect.SetVertexPos( 1, specialVertex, m_LineColor  );
			specialVertex = vertex[3];
			//specialVertex.x += 1;
			m_OutlineRightRect.SetVertexPos( 2, specialVertex, m_LineColor );
			specialVertex = vertex[3];
			specialVertex.x += 1;
			m_OutlineRightRect.SetVertexPos( 3, specialVertex, m_LineColor );


			specialVertex = vertex[2];
			m_OutlineBottomRect.SetVertexPos( 0, specialVertex, m_LineColor  );
			specialVertex = vertex[3];
			m_OutlineBottomRect.SetVertexPos( 1, specialVertex, m_LineColor  );
			specialVertex = vertex[2];
			specialVertex.y -= 1;
			m_OutlineBottomRect.SetVertexPos( 2, specialVertex, m_LineColor );
			specialVertex = vertex[3];
			specialVertex.y -= 1;
			m_OutlineBottomRect.SetVertexPos( 3, specialVertex, m_LineColor );
		}
	}




	





	



	return S_OK;
}

HRESULT CTexUVOut::OnFrameRender()
{
	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT );
	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_POINT );

	if ( m_bLoadTexture == true )
	{
	

		if ( m_EditMode == EM_POINT )
		{
			m_TexturedRectangle.OnFrameRender();

			CKTDGStateManager::PushRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
			CKTDGStateManager::PushRenderState( D3DRS_CULLMODE, D3DCULL_NONE );				

			m_WiredRect.OnFrameRender();			


			CKTDGStateManager::PopRenderState( D3DRS_CULLMODE );
			CKTDGStateManager::PopRenderState( D3DRS_FILLMODE );

			g_pKTDXApp->GetDevice()->SetTexture( 0, NULL );

			m_OutlineLeftRect.OnFrameRender();
			m_OutlineTopRect.OnFrameRender();
			m_OutlineRightRect.OnFrameRender();
			m_OutlineBottomRect.OnFrameRender();



			D3DVIEWPORT vp; 
			g_pKTDXApp->GetDevice()->GetViewport( &vp );
			D3DXMATRIX matWorld, matView, matProj;
			g_pKTDXApp->GetDevice()->GetTransform( D3DTS_WORLD, &matWorld );
			g_pKTDXApp->GetDevice()->GetTransform( D3DTS_VIEW, &matView );
			g_pKTDXApp->GetDevice()->GetTransform( D3DTS_PROJECTION, &matProj );

			float fScale = 1.0f;
			fScale = m_WiredRect.GetScale();

			D3DXMATRIX matRectWorld;
			D3DXMatrixScaling( &matRectWorld, fScale, fScale, 1 );

			D3DXVECTOR3 vertex[4];
			for ( int i = 0; i < 4; i++ )
			{
				vertex[i] = m_WiredRect.GetVertexPos( i );



				//D3DXVec3TransformCoord( &vertex[i], &vertex[i], &matRectWorld );
				D3DXVec3Project( &vertex[i], &vertex[i], &vp, &matProj, &matView, &matRectWorld );
			}


			m_pDeviceTextureRect->Draw( (int)(vertex[0].x / g_pKTDXApp->GetResolutionScaleX()), (int)(vertex[0].y / g_pKTDXApp->GetResolutionScaleY()), (int)((vertex[3].x - vertex[0].x) / g_pKTDXApp->GetResolutionScaleX()), (int)((vertex[3].y - vertex[0].y) / g_pKTDXApp->GetResolutionScaleY()), 0x60ff0000  );
		}
		else if ( m_EditMode == EM_RECTANGLE_AUTO )
		{
			//m_TexturedRectangle.OnFrameRender();
			LPDIRECT3DTEXTURE pTexture = m_TexturedRectangle.GetTexture();
			g_pKTDXApp->GetDevice()->SetTexture( 0, pTexture );
			DrawFace( m_vPos.x, m_vPos.y, m_vTextureOrgScale.x * m_vTextureScale.x, m_vTextureOrgScale.y * m_vTextureScale.y );
			
			

			if( m_sLinePos.bIsDraw == true )
			{
				g_pKTDXApp->GetDevice()->SetTexture( 0, NULL );

				D3DXVECTOR2 pos;
				D3DXVECTOR2 size;

				pos.x = (float)m_rectAutoRect.left * m_vTextureScale.x + m_vPos.x;// + m_vPos.x;
				pos.y = (float)m_rectAutoRect.top * m_vTextureScale.y + m_vPos.y;//+ m_vPos.y;
				size.x = (float)(m_rectAutoRect.right - m_rectAutoRect.left) * m_vTextureScale.x;
				size.y = (float)(m_rectAutoRect.bottom - m_rectAutoRect.top) * m_vTextureScale.y;

				if ( m_bDraggingRectAuto == false )
				{
					m_pDeviceTextureRect->Draw( (int)pos.x,  (int)pos.y, (int)(size.x), (int)(size.y), 0x60ff0000  );
					DrawLine( pos.x , pos.y , size.x , size.y, m_LineColor );
				}
				else
				{
					m_pDeviceTextureRect->Draw( (int)m_sLinePos.vPos.x,  (int)m_sLinePos.vPos.y, (int)(m_sLinePos.vSize.x), (int)(m_sLinePos.vSize.y), 0x60ff0000  );
					DrawLine( m_sLinePos.vPos.x, m_sLinePos.vPos.y, m_sLinePos.vSize.x, m_sLinePos.vSize.y, m_LineColor );
				}
			}
		}
	}

	CKTDGStateManager::PopSamplerState( 1, D3DSAMP_MIPFILTER);
	CKTDGStateManager::PopSamplerState( 1, D3DSAMP_MAGFILTER);
	CKTDGStateManager::PopSamplerState( 1, D3DSAMP_MINFILTER);
	CKTDGStateManager::PopSamplerState( 0, D3DSAMP_MIPFILTER);
	CKTDGStateManager::PopSamplerState( 0, D3DSAMP_MAGFILTER);
	CKTDGStateManager::PopSamplerState( 0, D3DSAMP_MINFILTER);
	
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameRender();
	
	TextRender();

	m_SampleUI.OnRender( DXUTGetElapsedTime() );

	return S_OK;
}

HRESULT CTexUVOut::OnResetDevice()
{
	m_TexturedRectangle.OnResetDevice();
	m_WiredRect.OnResetDevice();

	m_OutlineLeftRect.OnResetDevice();
	m_OutlineTopRect.OnResetDevice();
	m_OutlineRightRect.OnResetDevice();
	m_OutlineBottomRect.OnResetDevice();

	for ( int i = 0; i < 4; i++ )
	{
		m_vUVTexCoord[i].x = 0.0f;
		m_vUVTexCoord[i].y = 0.0f;
	}

	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
}

HRESULT CTexUVOut::OnLostDevice()
{
	m_TexturedRectangle.OnLostDevice();
	m_WiredRect.OnLostDevice();

	m_OutlineLeftRect.OnLostDevice();
	m_OutlineTopRect.OnLostDevice();
	m_OutlineRightRect.OnLostDevice();
	m_OutlineBottomRect.OnLostDevice();

	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
}

bool CTexUVOut::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( m_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	static POINTS	pt;

	switch ( uMsg )
	{
		case CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT:
			{
				switch( wParam )
				{
					case TUOUCE_EDIT_MODE_CHANGED:
						{
							CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)lParam;

							if ( wcscmp( pRadioButton->GetName(), L"RadioButtonEditModePoint" ) == 0 )
							{
								m_EditMode = EM_POINT;

								D3DXMATRIX matWorld;
								D3DXMatrixIdentity(&matWorld);
								g_pKTDXApp->SetWorldTransform( &matWorld );
								g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-700, 0,0,0 );
								g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix() );
								//g_pKTDXApp->GetDevice()->SetTransform( D3DTS_VIEW, (D3DXMATRIX*) &g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix() );
								g_pKTDXApp->GetDGManager()->SetProjection( 0.01f, 12000.f, false );
							}
							else if ( wcscmp( pRadioButton->GetName(), L"RadioButtonEditModeRectAuto" ) == 0 )
							{
								m_EditMode = EM_RECTANGLE_AUTO;
							}
							else if ( wcscmp( pRadioButton->GetName(), L"RadioButtonEditModeRectManual" ) == 0 )
							{
								m_EditMode = EM_RECTANGLE_MANUAL;
							}
						}
						break;

					case TUOUCE_POINT_TYPE_CHANGED:
						{
							CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)lParam;
							
							if ( wcscmp( pRadioButton->GetName(), L"RadioButtonLeftTop" ) == 0 )
							{
								m_NowPointType = LEFT_TOP;
							}
							else if ( wcscmp( pRadioButton->GetName(), L"RadioButtonRightTop" ) == 0 )
							{
								m_NowPointType = RIGHT_TOP;
							}
							else if ( wcscmp( pRadioButton->GetName(), L"RadioButtonLeftBottom" ) == 0 )
							{
								m_NowPointType = LEFT_BOTTOM;
							}
							else if ( wcscmp( pRadioButton->GetName(), L"RadioButtonRightBottom" ) == 0 )
							{
								m_NowPointType = RIGHT_BOTTOM;
							}
						}
						break;

					case TUOUCE_RECT_ADD:
						{
							AddTETData();
						}
						break;

					case TUOUCE_RECT_DELETE:
						{
							DeleteSelecetedTETData();
						}
						break;

					case TUOUCE_RECT_OUT:
						{
							ExtractTETDataList();
						}
						break;
				}
			}
			break;

		case WM_DROPFILES:
			{
				return CatchFile( wParam );
			}
			break;

		case WM_MOUSEWHEEL:
			{
				if ( m_bLoadTexture == true )
				{
					if ( m_EditMode == EM_POINT )
					{
						int zDelta = (int) wParam;
						if( zDelta > 0 )
						{			
							m_TexturedRectangle.Rescale( 2.f );	
							m_WiredRect.Rescale( 2.f );
							m_OutlineLeftRect.Rescale( 2.f );
							m_OutlineTopRect.Rescale( 2.f );
							m_OutlineRightRect.Rescale( 2.f );
							m_OutlineBottomRect.Rescale( 2.f );
							CKTDGCamera* pCamera = &g_pKTDXApp->GetDGManager()->GetCamera();
							pCamera->Point( pCamera->GetEye().x*2.f, pCamera->GetEye().y*2.f, -700, pCamera->GetLookAt().x*2.f, pCamera->GetLookAt().y*2.f,0 );
							g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );
						}
						else
						{
							m_TexturedRectangle.Rescale( 0.5f );
							m_WiredRect.Rescale( 0.5f );
							m_OutlineLeftRect.Rescale( 0.5f );
							m_OutlineTopRect.Rescale( 0.5f );
							m_OutlineRightRect.Rescale( 0.5f );
							m_OutlineBottomRect.Rescale( 0.5f );

							CKTDGCamera* pCamera = &g_pKTDXApp->GetDGManager()->GetCamera();
							pCamera->Point( pCamera->GetEye().x*0.5f, pCamera->GetEye().y*0.5f, -700, pCamera->GetLookAt().x*0.5f, pCamera->GetLookAt().y*0.5f,0 );
							g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );
						}
					}
					else if ( m_EditMode == EM_RECTANGLE_AUTO )
					{
						
						int zDelta = (int) wParam;
						if( zDelta > 0 )
						{			
							m_vTextureScale.x += 0.2f;
							m_vTextureScale.y += 0.2f;
						}
						else
						{
							m_vTextureScale.x -= 0.2f;
							m_vTextureScale.y -= 0.2f;
						}
						
						
					}
				}
				
			} 
			break;

		case WM_MOUSEMOVE:
			{
				POINT mousepos = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
				D3DXVECTOR2 mousePoint;
				mousePoint = g_pKTDXApp->MouseConvertByResolution( (float)mousepos.x, (float)mousepos.y );

				static D3DXVECTOR2 prevMousePoint = mousePoint;
				static POINT PrevMousePoint = mousepos;


				if ( m_bLoadTexture == true )
				{
					if( true == m_bRButtonDragging )
					{
						if ( m_EditMode == EM_POINT )
						{
							D3DXVECTOR2 vOffsetPos = D3DXVECTOR2( (float)(mousePoint.x-prevMousePoint.x), (float)(mousePoint.y-prevMousePoint.y) );
							CKTDGCamera* pCamera = &g_pKTDXApp->GetDGManager()->GetCamera();
							pCamera->Point( pCamera->GetEye().x-vOffsetPos.x, pCamera->GetEye().y+vOffsetPos.y, -700, pCamera->GetLookAt().x-vOffsetPos.x, pCamera->GetLookAt().y+vOffsetPos.y,0 );
							g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &pCamera->GetViewMatrix() );
						}
						else if ( m_EditMode == EM_RECTANGLE_AUTO )
						{
							m_vPos.x += (float)(mousePoint.x - prevMousePoint.x);
							m_vPos.y += (float)(mousePoint.y - prevMousePoint.y);
						}
					}	
				}

				

				prevMousePoint = mousePoint;
				//PrevMousePoint = mousePoint;
					
				if ( m_EditMode == EM_RECTANGLE_AUTO )
				{
					if ( m_bLoadTexture == true && m_bDraggingRectAuto == true  )
					{
						POINTS ptEnd;
						ptEnd = MAKEPOINTS( lParam );



						m_sLinePos.vSize.x = (float)(ptEnd.x - pt.x);
						m_sLinePos.vSize.y = (float)(ptEnd.y - pt.y);

						m_rectAutoRect.right = ptEnd.x;
						m_rectAutoRect.bottom = ptEnd.y;
					}
				}
			} 
			break;

		case WM_RBUTTONDOWN:
			{
				m_bRButtonDragging = true;
			} 
			break;

		case WM_RBUTTONUP:
			{
				m_bRButtonDragging = false;
			} 
			break;

		case WM_LBUTTONDOWN:
			{
				m_bLButtonDown = true;

				POINTS mousepos = MAKEPOINTS(lParam);
				if ( m_EditMode == EM_POINT )
				{
					if ( m_bLoadTexture == true )
					{
						D3DVIEWPORT vp; 
						g_pKTDXApp->GetDevice()->GetViewport( &vp );
						D3DXMATRIX matWorld, matView, matProj;
						g_pKTDXApp->GetDevice()->GetTransform( D3DTS_WORLD, &matWorld );
						g_pKTDXApp->GetDevice()->GetTransform( D3DTS_VIEW, &matView );
						g_pKTDXApp->GetDevice()->GetTransform( D3DTS_PROJECTION, &matProj );

						D3DXVECTOR3 vMouse2 = D3DXVECTOR3( mousepos.x, mousepos.y, 0 );
						D3DXVECTOR3 vMouse;
						D3DXVec3Unproject( &vMouse, &vMouse2, &vp, &matProj, &matView, &matWorld );
						D3DXVECTOR3 vMouseWorld = vMouse;

						vMouse.y = m_TexturedRectangle.GetRectSize() - vMouse.y;
						vMouse.x *= m_TexturedRectangle.GetWidth() / m_TexturedRectangle.GetRectSize();
						vMouse.y *= m_TexturedRectangle.GetHeight() / m_TexturedRectangle.GetRectSize();
						float fTemp2[2] = { (float)(int)vMouse.x, (float)(int)vMouse.y };
						vMouse.x = fTemp2[0];
						vMouse.y = fTemp2[1];
/*
						WCHAR buff[256] = {0};

						swprintf( buff, L"%f %f %f", vMouseWorld.x, vMouseWorld.y, vMouseWorld.z );

						MessageBox( NULL, buff, L"dd", MB_OK );
*/

						m_vUVTexCoord[m_NowPointType].x = vMouse.x;
						m_vUVTexCoord[m_NowPointType].y = vMouse.y;

						m_WiredRect.SetVertexPos( m_NowPointType, vMouseWorld );

					}
				}
				else if ( m_EditMode == EM_RECTANGLE_AUTO )
				{
					if ( m_bLoadTexture == true )
					{
						SetCapture( hWnd );
						pt = MAKEPOINTS( lParam );
						m_bDraggingRectAuto = true;

						m_sLinePos.vPos.x = (float)pt.x;
						m_sLinePos.vPos.y = (float)pt.y;
						m_sLinePos.vSize.x = 0.0f;
						m_sLinePos.vSize.y = 0.0f;

						m_rectAutoRect.left = pt.x;
						m_rectAutoRect.top = pt.y;
						m_rectAutoRect.right = 0;
						m_rectAutoRect.bottom = 0;

						m_sLinePos.bIsDraw = true;
					}
				}
			}
			break;

		case WM_LBUTTONUP:
			{
				if ( m_bLButtonDown == true )
				{
					m_bLButtonDown = false;

					if ( m_EditMode == EM_RECTANGLE_AUTO )
					{
						if ( m_bLoadTexture == true )
						{
							POINTS ptEnd;
							ptEnd = MAKEPOINTS( lParam );
							m_bDraggingRectAuto = false;

							m_sLinePos.vPos.x = (float)pt.x;
							m_sLinePos.vPos.y = (float)pt.y;

							m_sLinePos.vSize.x = (float)(ptEnd.x - pt.x);
							m_sLinePos.vSize.y = (float)(ptEnd.y - pt.y);

							ReleaseCapture();
							TextureFilltering();
						}
					}

					
				}
				
			}
			break;
	}



	return false;
}




void CTexUVOut::TextRender()
{
	
	D3DXVECTOR2 texUV[4];

	if ( m_EditMode == EM_POINT )
	{
		const D3DXVECTOR2* pTexCoords = GetTexCoords();
		for( int i=0; i<4; i++ )
		{
			texUV[i].x = (float)(int)pTexCoords[i].x;
			texUV[i].y = (float)(int)pTexCoords[i].y;
		}
	}
	else if ( m_EditMode == EM_RECTANGLE_AUTO )
	{
		/*
		texUV[0].x = m_sLinePos.vPos.x - m_vPos.x;
		texUV[0].y = m_sLinePos.vPos.y - m_vPos.y;

		texUV[1].x = m_sLinePos.vPos.x + m_sLinePos.vSize.x - m_vPos.x;
		texUV[1].y = m_sLinePos.vPos.y - m_vPos.y;

		texUV[2].x = m_sLinePos.vPos.x - m_vPos.x;
		texUV[2].y = m_sLinePos.vPos.y + m_sLinePos.vSize.y  - m_vPos.y;

		texUV[3].x = m_sLinePos.vPos.x + m_sLinePos.vSize.x - m_vPos.x;
		texUV[3].y = m_sLinePos.vPos.y + m_sLinePos.vSize.y - m_vPos.y;
		*/
		
	
		texUV[0].x = (float)m_rectAutoRect.left;
		texUV[0].y = (float)m_rectAutoRect.top;

		texUV[1].x = (float)m_rectAutoRect.right;
		texUV[1].y = (float)m_rectAutoRect.top;

		texUV[2].x = (float)m_rectAutoRect.left;
		texUV[2].y = (float)m_rectAutoRect.bottom;

		texUV[3].x = (float)m_rectAutoRect.right;
		texUV[3].y = (float)m_rectAutoRect.bottom;
	}


	WCHAR buff[256] = {0};
	swprintf( buff, L" %d, %d", (int)texUV[0].x, (int)texUV[0].y );
	m_pFont->Draw2DText( buff, 80, 225, D3DXCOLOR( 1,1,1,1), D3DXCOLOR( 1,1,1,1) );

	swprintf( buff, L" %d, %d", (int)texUV[1].x, (int)texUV[1].y );
	m_pFont->Draw2DText( buff, 90, 275, D3DXCOLOR( 1,1,1,1), D3DXCOLOR( 1,1,1,1) );

	swprintf( buff, L"%d, %d ", (int)texUV[2].x, (int)texUV[2].y );
	m_pFont->Draw2DText( buff, 110, 325, D3DXCOLOR( 1,1,1,1), D3DXCOLOR( 1,1,1,1) );

	swprintf( buff, L"  %d, %d", (int)texUV[3].x, (int)texUV[3].y );
	m_pFont->Draw2DText( buff, 110, 370, D3DXCOLOR( 1,1,1,1), D3DXCOLOR( 1,1,1,1) );
}




bool CTexUVOut::AddTETData()
{
	if ( m_bLoadTexture == false )
		return false;


	
	wstring keyName = m_pIMEEditBox->GetText();

	if ( keyName.empty() == true )
		return false;

	for ( int i = 0; i < (int)m_TETDataList.size(); i++ )
	{
		TET_DATA* pTetData = m_TETDataList[i];
		
		if ( keyName == pTetData->key )
		{
			MessageBox( g_pKTDXApp->GetHWND(), L"이미 등록된 키 값 입니다.", L"경고", MB_OK );
			return false;
		}
	}

	TET_DATA* pTetData = new TET_DATA();
	
	pTetData->key = keyName;

	const D3DXVECTOR2* pTexCoords = GetTexCoords();
	D3DXVECTOR2 texUV[4];

	if ( m_EditMode == EM_POINT )
	{
		for( int i=0; i<4; i++ )
		{
			texUV[i].x = (float)(int)pTexCoords[i].x;
			texUV[i].y = (float)(int)pTexCoords[i].y;
		}
	}
	else if ( m_EditMode == EM_RECTANGLE_AUTO )
	{
		texUV[0].x = (float)m_rectAutoRect.left;
		texUV[0].y = (float)m_rectAutoRect.top;

		texUV[1].x = (float)m_rectAutoRect.right;
		texUV[1].y = (float)m_rectAutoRect.top;

		texUV[2].x = (float)m_rectAutoRect.left;
		texUV[2].y = (float)m_rectAutoRect.bottom;

		texUV[3].x = (float)m_rectAutoRect.right;
		texUV[3].y = (float)m_rectAutoRect.bottom;

	}

	

	pTetData->pTexUV = new CKTDXDeviceTexture::TEXTURE_UV();
	pTetData->pTexUV->leftTop		= texUV[0];
	pTetData->pTexUV->rightTop		= texUV[1];
	pTetData->pTexUV->leftBottom	= texUV[2];
	pTetData->pTexUV->rightBottom	= texUV[3];

	CKTDGUIListBox* pListBox = (CKTDGUIListBox*)m_pDialog->GetControl( L"ListBoxTETData" );
	pListBox->AddItem( pTetData->key.c_str(), pTetData );

	m_TETDataList.push_back( pTetData );

	return true;
}

bool CTexUVOut::DeleteSelecetedTETData()
{
	CKTDGUIListBox* pListBox = (CKTDGUIListBox*)m_pDialog->GetControl( L"ListBoxTETData" );

	int selectedItemIndex = pListBox->GetSelectedIndex();
	if ( selectedItemIndex == -1 )
		return false;

	CKTDGUIListBox::ListBoxItem* pListBoxItem = pListBox->GetSelectedItem();


	TET_DATA* pTetDataforDelete = (TET_DATA*)pListBoxItem->pData;

	for ( int i = 0; i < (int)m_TETDataList.size(); i++ )
	{
		TET_DATA* pTetData = m_TETDataList[i];

		if ( pTetData == pTetDataforDelete )
		{
			SAFE_DELETE( pTetData );
			m_TETDataList.erase( m_TETDataList.begin() + i );
			pListBox->RemoveItem( selectedItemIndex );
			return true;
		}
	}

	return false;
}

bool CTexUVOut::ExtractTETDataList()
{
	if ( m_bLoadTexture == false )
		return false;

	wstring fileName = m_LoadedTextureName + L".TET";

	OPENFILENAME m_OFN;

	WCHAR lpstrFile[255] = {0,};

	wcscpy(lpstrFile, fileName.c_str());

	memset(&m_OFN, 0, sizeof(OPENFILENAME) );
	m_OFN.lStructSize = sizeof( OPENFILENAME );
	m_OFN.hwndOwner = g_pKTDXApp->GetHWND();
	m_OFN.lpstrFilter = L"TET-file(*.tet)\0*.*\0";
	m_OFN.lpstrFile = lpstrFile;
	m_OFN.nMaxFile = 255;


	WCHAR FullPath[MAX_PATH] = L"";
	if ( GetSaveFileName( &m_OFN ) != 0 )
	{
		
		memset( FullPath, 0, sizeof(FullPath));
		wsprintf( FullPath, lpstrFile );

	}
	else 
	{
		return false;
	}


	
	

	string strFileName;
	ConvertWCHARToChar( strFileName, FullPath );
	const string constStrFileName = strFileName;

	//const string MAGIC_TEMP_FILE_PATH = "C:\\다이얼로그툴_임시파일.txt";

	FILE* pFile = fopen(constStrFileName.c_str(), "w+" );

	if ( pFile == NULL )
	{
		return false;
	}
/*
	for ( int i = 0; i < (int)m_TETDataList.size(); i++ )
	{
		TET_DATA* pTetData = m_TETDataList[i];
		string strKeyName;
		string buffString;
		char buff[256] = {0};


		ConvertWCHARToChar( strKeyName, pTetData->key.c_str() );

		buffString = "m_pTEX:AddRect( \"";
		buffString += strKeyName;
		buffString += "\", ";
		sprintf( buff, "D3DXVECTOR2( %d, %d ), D3DXVECTOR2( %d, %d ),\n", (int)pTetData->pTexUV->leftTop.x, 
																		(int)pTetData->pTexUV->leftTop.y,
																		(int)pTetData->pTexUV->rightTop.x,
																		(int)pTetData->pTexUV->rightTop.y);
		buffString += buff;
		sprintf( buff, "D3DXVECTOR2( %d, %d ), D3DXVECTOR2( %d, %d ) )\n", (int)pTetData->pTexUV->leftBottom.x, 
																		(int)pTetData->pTexUV->leftBottom.y,
																		(int)pTetData->pTexUV->rightBottom.x,
																		(int)pTetData->pTexUV->rightBottom.y);
		buffString += buff;
	
		buffString += "\n";

		fputs(buffString.c_str(),pFile);

	}

	fclose( pFile );
	*/



	std::ofstream ofs;
	ofs.open( constStrFileName.c_str(), ios::out | ios::binary );

	char bom1 = 0xef;
	char bom2 = 0xbb;
	char bom3 = 0xbf;

	ofs.write(&bom1, 1);
	ofs.write(&bom2, 1);
	ofs.write(&bom3, 1);

	for ( int i = 0; i < (int)m_TETDataList.size(); i++ )
	{
		TET_DATA* pTetData = m_TETDataList[i];
		string strKeyName;		

		ConvertWCHARToChar( strKeyName, pTetData->key.c_str() );
		
		ofs << "m_pTEX:AddRect( \"" << strKeyName.c_str() << "\", ";		
		ofs << "D3DXVECTOR2( " << (int)pTetData->pTexUV->leftTop.x << ", " << (int)pTetData->pTexUV->leftTop.y << " ), "; 
		ofs << "D3DXVECTOR2( " << (int)pTetData->pTexUV->rightTop.x << ", " << (int)pTetData->pTexUV->rightTop.y << " ), " << "\n"; 
		ofs << "\t\t\t\t\t         D3DXVECTOR2( " << (int)pTetData->pTexUV->leftBottom.x << ", " << (int)pTetData->pTexUV->leftBottom.y << " ), "; 
		ofs << "D3DXVECTOR2( " << (int)pTetData->pTexUV->rightBottom.x << ", " << (int)pTetData->pTexUV->rightBottom.y << " ) ) " << "\n"; 

		if ( pTetData->pTexUV->rotateValue != 0 )
			ofs << "m_pTEX:RotateRect( \"" << strKeyName.c_str() << "\"," << pTetData->pTexUV->rotateValue << " )" << "\n";

		ofs << "\n";
	}

	ofs.close();
	
	return true;
}

bool CTexUVOut::TETFileLoad( const WCHAR* pFullFileName )
{

	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	wstring fileName;

	_wsplitpath( pFullFileName, drive, dir, fname, ext);

	fileName = fname;
	fileName += ext;

	if ( CreateTexture( pFullFileName ) == true )
	{
		CKTDXDeviceTexture* pDeviceTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( fileName );
		if ( pDeviceTexture == NULL )
		{
			string	strDir;
			char	csTemp[MAX_PATH] = "";

			WideCharToMultiByte( CP_UTF8, 0, drive, -1, csTemp, MAX_PATH, NULL, NULL );
			strDir += csTemp;
			WideCharToMultiByte( CP_UTF8, 0, dir, -1, csTemp, MAX_PATH, NULL, NULL );
			strDir += csTemp;

			g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );
			pDeviceTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( fileName );
			if ( pDeviceTexture == NULL )
			{
				MessageBox( g_pKTDXApp->GetHWND(), L"텍스쳐를 찾을 수 없습니다!", L"에러",  MB_OK );
				ReInit();
				return false;
			}
		}

		map< wstring, CKTDXDeviceTexture::TEXTURE_UV* >::iterator i;
		for ( i = pDeviceTexture->m_mapTexUVRect.begin(); i != pDeviceTexture->m_mapTexUVRect.end(); i++ )
		{
			TET_DATA* pTetData = new TET_DATA();
			pTetData->key = i->first;

			CKTDXDeviceTexture::TEXTURE_UV* pTexUV = i->second;

			int rotateValue = pTexUV->rotateValue;

			if ( rotateValue != 0 )
			{
				string tempString;
				ConvertWCHARToChar( tempString, pTetData->key.c_str() );
				pDeviceTexture->RotateRect_LUA( tempString.c_str(), rotateValue % 360 );
			}

			

			pTetData->pTexUV = new CKTDXDeviceTexture::TEXTURE_UV();
			pTetData->pTexUV->leftTop		= pTexUV->leftTop;
			pTetData->pTexUV->rightTop		= pTexUV->rightTop;
			pTetData->pTexUV->leftBottom	= pTexUV->leftBottom;
			pTetData->pTexUV->rightBottom	= pTexUV->rightBottom;

			pTetData->pTexUV->rotateValue = rotateValue;


			m_TETDataList.push_back( pTetData );

			CKTDGUIListBox* pListBox = (CKTDGUIListBox*)m_pDialog->GetControl( L"ListBoxTETData" );
			pListBox->AddItem( pTetData->key.c_str(), pTetData );
		}

		SAFE_CLOSE( pDeviceTexture );
	}

	return true;
}

void CTexUVOut::EditPoint()
{
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_1 ) == TRUE )
	{
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"RadioButtonLeftTop" );
		pRadioButton->GetCheckedControl()->SetChecked(false);
		pRadioButton->SetChecked( true );
	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_2 ) == TRUE )
	{
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"RadioButtonRightTop" );
		pRadioButton->GetCheckedControl()->SetChecked(false);
		pRadioButton->SetChecked( true );
	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_3 ) == TRUE )
	{
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"RadioButtonLeftBottom" );
		pRadioButton->GetCheckedControl()->SetChecked(false);
		pRadioButton->SetChecked( true );

	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_4 ) == TRUE )
	{
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"RadioButtonRightBottom" );
		pRadioButton->GetCheckedControl()->SetChecked(false);
		pRadioButton->SetChecked( true );

	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_LEFTARROW ) == TRUE )
	{
		D3DXVECTOR2 vPos;
		vPos.x = -1;// * m_WiredRect.GetScale();
		vPos.y = 0;

		m_vUVTexCoord[m_NowPointType].x += -1;
		m_WiredRect.AddVertexPos( m_NowPointType, vPos );
	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_RIGHTARROW ) == TRUE )
	{


		D3DXVECTOR2 vPos;
		vPos.x = 1; //* m_WiredRect.GetScale();
		vPos.y = 0;

		m_vUVTexCoord[m_NowPointType].x += 1;

		m_WiredRect.AddVertexPos( m_NowPointType, vPos );

	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_UPARROW ) == TRUE )
	{
		

		D3DXVECTOR2 vPos;
		vPos.x = 0;
		vPos.y = 1; //* m_WiredRect.GetScale();

		m_vUVTexCoord[m_NowPointType].y += -1;

		m_WiredRect.AddVertexPos( m_NowPointType, vPos );


	}

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_DOWNARROW ) == TRUE )
	{
		

		D3DXVECTOR2 vPos;
		vPos.x = 0;
		vPos.y = -1; //* m_WiredRect.GetScale();

		m_vUVTexCoord[m_NowPointType].y += 1;

		m_WiredRect.AddVertexPos( m_NowPointType, vPos );
	}
}

bool CTexUVOut::CatchFile( WPARAM wParam )
{
	WCHAR PullFileName[MAX_PATH] = L"";
	WCHAR FileName[260] = L"";
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];
	

	DragQueryFile( (HDROP) wParam, 0, PullFileName, MAX_PATH );

	_wsplitpath( PullFileName, drive, dir, fname, ext);
	memset( FileName, 0, sizeof(FileName) );
	wcscat( FileName, fname );
	wcscat( FileName, ext );

	wstring wstrExt = wcsupr( ext );

	if ( wstrExt == L".TGA" || wstrExt == L".DDS" )
	{
		ReInit();
		m_LoadedTextureName = fname;
		return TETFileLoad( PullFileName );
	}
	else
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"확장자가 TGA, DDS인 텍스쳐 파일만 지원합니다", L"경고", MB_OK );
		return false;
	}
}

bool CTexUVOut::CreateTexture( const WCHAR* pFileName )
{
	m_TexturedRectangle.ReleaseTexture();
	m_bLoadTexture = m_TexturedRectangle.CreateTexture( pFileName );

	D3DSURFACE_DESC surface;

#ifdef MODIFY_SURFACE_DESC
	LPDIRECT3DSURFACE9 pSurefaceLevel = NULL;
	if(FAILED( hr = m_TexturedRectangle.GetTexture()->GetSurfaceLevel( 0, &pSurefaceLevel )))
	{
		string strName;
		ConvertWCHARToChar( strName, pFileName );
		ErrorLogMsg( KEM_ERROR79, strName.c_str());

		return false;
	}
	pSurefaceLevel->GetDesc(&surface);
#else
	m_TexturedRectangle.GetTexture()->GetLevelDesc( 0, &surface );
#endif

	m_vTextureOrgScale.x = (float)surface.Width;
	m_vTextureOrgScale.y = (float)surface.Height;

	m_vTextureScale = D3DXVECTOR2( 1, 1);

	return m_bLoadTexture;
}

bool CTexUVOut::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pMain", this );

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
		return false;

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
		return false;

	return true;
}



void CTexUVOut::TextureFilltering()
{
   	if ( m_TexturedRectangle.GetTexture() == NULL )
		return;

	//m_vPos;
	//m_sLinePos;
	LPDIRECT3DSURFACE9 pSurefaceLevel = NULL;

	LPDIRECT3DTEXTURE pTexture = m_TexturedRectangle.GetTexture();

	D3DSURFACE_DESC surface;

#ifdef MODIFY_SURFACE_DESC
	LPDIRECT3DSURFACE9 pSurefaceLevel = NULL;
	if(FAILED( hr = pTexture->GetSurfaceLevel( 0, &pSurefaceLevel )))
	{
		string strName;
		ConvertWCHARToChar( strName, pFileName );
		ErrorLogMsg( KEM_ERROR79, strName.c_str());

		return false;
	}
	pSurefaceLevel->GetDesc(&surface);
#else
	pTexture->GetLevelDesc( 0, &surface );
#endif

	if( pTexture->GetSurfaceLevel( 0, &pSurefaceLevel ) != D3D_OK )
		return;

	if( pSurefaceLevel != NULL )
	{
		HDC hDC;
		if( pSurefaceLevel->GetDC( &hDC ) != D3D_OK )
		{
			pSurefaceLevel->Release();
			return;
		}

		POINT ptStart;
		POINT ptEnd;

		ptStart.x	= (int)( (m_sLinePos.vPos.x - m_vPos.x) / m_vTextureScale.x); //- m_TexturedRectangle.);
		ptStart.y	= (int)( (m_sLinePos.vPos.y - m_vPos.y ) / m_vTextureScale.y); //- m_vPos.y);
		ptEnd.x		= (int)( (m_sLinePos.vSize.x / m_vTextureScale.x) + ptStart.x); //+ ptStart.x);
		ptEnd.y		= (int)( (m_sLinePos.vSize.y / m_vTextureScale.y) + ptStart.y); //+ ptStart.y);

		

		//비교대상 컬러값을 시작점 값으로 임시 사용한다.
		//추후 등록배경색으로 사용하기
		COLORREF bgColor = GetPixel( hDC, (int)(ptStart.x), (int)(ptStart.y) );

		POINT ptSPos;
		POINT ptEPos;
		bool  bStartCheck = false;
		SetPoint( ptSPos, 0, 0 );
		SetPoint( ptEPos, 0, 0 );
		for( int nY = ptStart.y; nY <= ptEnd.y; ++nY )
		{
			for( int nX = ptStart.x; nX <= ptEnd.x; ++nX )
			{
				if( bgColor != GetPixel( hDC, nX , nY ) )
				{
					if( bStartCheck == false )
					{
						SetPoint( ptSPos, nX, nY );
						SetPoint( ptEPos, nX, nY );
						bStartCheck = true;
					}
					else
					{
						if( ptSPos.x > nX )
							ptSPos.x = nX;
						if( ptSPos.y > nY )
							ptSPos.y = nY;

						if( ptEPos.x < nX )
							ptEPos.x = nX;
						if( ptEPos.y < nY )
							ptEPos.y = nY;
					}
				}
			}
		}

		pSurefaceLevel->ReleaseDC( hDC );
		pSurefaceLevel->Release();

		m_rectAutoRect.left		= ptSPos.x;
		m_rectAutoRect.top		= ptSPos.y;
		m_rectAutoRect.right	= ptEPos.x;
		m_rectAutoRect.bottom	= ptEPos.y;
/*
		m_sLinePos.vPos.x = (float)ptSPos.x * m_vTextureScale.x + m_vPos.x;// + m_vPos.x;
		m_sLinePos.vPos.y = (float)ptSPos.y * m_vTextureScale.y + m_vPos.y;//+ m_vPos.y;
		m_sLinePos.vSize.x = (float)(ptEPos.x - ptSPos.x) * m_vTextureScale.x;
		m_sLinePos.vSize.y = (float)(ptEPos.y - ptSPos.y) * m_vTextureScale.y;
		*/
	}
}


void CTexUVOut::SetPoint( POINT& pt, int x, int y )
{
	pt.x = x;
	pt.y = y;
};


void CTexUVOut::DrawFace( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color /* = 0xffffffff  */)
{
	//CKTDGStateManager::BeginStateBlock( m_RenderStateID );
	CKTDGStateManager::SetRenderState( D3DRS_ZENABLE,	FALSE);
	CKTDGStateManager::SetRenderState( D3DRS_ZWRITEENABLE,	FALSE);
	CKTDGStateManager::SetRenderState( D3DRS_CULLMODE,	D3DCULL_NONE);

	DRAW_VERTEX vertex[4];
	ZeroMemory( vertex, sizeof(DRAW_VERTEX) * 4 );

	float X1 = (float)nX - 0.5f;
	float Y1 = (float)nY - 0.5f;
	float X2 = (float)(nX + nWidth);
	float Y2 = (float)(nY + nHeight);

	float UMax = 1.0f;
	float VMax = 1.0f;

	//Set up the 4 corners of a small square
	vertex[0].vPos.x = X1;		vertex[0].vPos.y = Y1;
	vertex[0].vPos.z = 1.0f;	vertex[0].vPos.w = 1.0f;
	vertex[0].u = 0.0f;			vertex[0].v = 0.0f;

	vertex[1].vPos.x = X1;		vertex[1].vPos.y = Y2;
	vertex[1].vPos.z = 1.0f;	vertex[1].vPos.w = 1.0f;
	vertex[1].u = 0.0f;			vertex[1].v = VMax;

	vertex[2].vPos.x = X2;		vertex[2].vPos.y = Y1;
	vertex[2].vPos.z = 1.0f;	vertex[2].vPos.w = 1.0f;
	vertex[2].u = UMax;			vertex[2].v = 0.0f;

	vertex[3].vPos.x = X2;		vertex[3].vPos.y = Y2;
	vertex[3].vPos.z = 1.0f;	vertex[3].vPos.w = 1.0f;
	vertex[3].u = UMax;			vertex[3].v = VMax;

	vertex[0].color = vertex[1].color = 
		vertex[2].color = vertex[3].color = color;

	g_pKTDXApp->GetDevice()->SetFVF( DRAW_VERTEX::FVF );
	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(DRAW_VERTEX) );

	//CKTDGStateManager::EndStateBlock( m_RenderStateID );
}


void CTexUVOut::DrawLine( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color /* = 0xffffffff  */)
{
	//CKTDGStateManager::BeginStateBlock( m_RenderStateID );
	CKTDGStateManager::SetRenderState( D3DRS_ZENABLE,	FALSE);
	CKTDGStateManager::SetRenderState( D3DRS_ZWRITEENABLE,	FALSE);
	CKTDGStateManager::SetRenderState( D3DRS_CULLMODE,	D3DCULL_NONE);

	LINE_VERTEX vertex[8];
	ZeroMemory( vertex, sizeof(LINE_VERTEX) * 8 );

	vertex[0].vPos.x = nX;
	vertex[0].vPos.y = nY;

	vertex[1].vPos.x = nX + nWidth;
	vertex[1].vPos.y = nY;

	vertex[2].vPos.x = nX + nWidth;
	vertex[2].vPos.y = nY;

	vertex[3].vPos.x = nX + nWidth;
	vertex[3].vPos.y = nY + nHeight;

	vertex[4].vPos.x = nX + nWidth;
	vertex[4].vPos.y = nY + nHeight;

	vertex[5].vPos.x = nX;
	vertex[5].vPos.y = nY + nHeight;

	vertex[6].vPos.x = nX;
	vertex[6].vPos.y = nY + nHeight;

	vertex[7].vPos.x = nX;
	vertex[7].vPos.y = nY;

	for( int i = 0; i < 8; ++i )
	{
		vertex[i].color		= color;
		vertex[i].vPos.z	= 1.0f;
		vertex[i].vPos.w	= 1.0f;
	}

	g_pKTDXApp->GetDevice()->SetFVF( LINE_VERTEX::FVF );
	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_LINELIST, 4, vertex, sizeof(LINE_VERTEX) );

	//CKTDGStateManager::EndStateBlock( m_RenderStateID );
}


CTexUVOut::CTexturedRectangle::CTexturedRectangle( )
{
	FVF_CUSTOMVERTEX = ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );

	m_pVertexDecl = NULL;
	m_pVB = NULL;
	m_pIB = NULL;
	m_pTexture = NULL;			
	m_fScale = 1.f;
	m_fRectSize = 512.f;
}

CTexUVOut::CTexturedRectangle::~CTexturedRectangle()
{
	ReleaseBuffer();
	ReleaseTexture();
}



float CTexUVOut::CTexturedRectangle::GetWidth()
{
	D3DSURFACE_DESC surface;

#ifdef MODIFY_SURFACE_DESC
	LPDIRECT3DSURFACE9 pSurefaceLevel = NULL;
	if(FAILED( hr = m_pTexture->GetSurfaceLevel( 0, &pSurefaceLevel )))
	{
		string strName;
		ConvertWCHARToChar( strName, pFileName );
		ErrorLogMsg( KEM_ERROR79, strName.c_str());

		return false;
	}
	pSurefaceLevel->GetDesc(&surface);
#else
	m_pTexture->GetLevelDesc( 0, &surface );
#endif
	return (float)surface.Width;

}

float CTexUVOut::CTexturedRectangle::GetHeight()
{
	D3DSURFACE_DESC surface;

#ifdef MODIFY_SURFACE_DESC
	LPDIRECT3DSURFACE9 pSurefaceLevel = NULL;
	if(FAILED( hr = m_pTexture->GetSurfaceLevel( 0, &pSurefaceLevel )))
	{
		string strName;
		ConvertWCHARToChar( strName, pFileName );
		ErrorLogMsg( KEM_ERROR79, strName.c_str());

		return false;
	}
	pSurefaceLevel->GetDesc(&surface);
#else
	m_pTexture->GetLevelDesc( 0, &surface );
#endif
	return (float)surface.Height;
}


void CTexUVOut::CTexturedRectangle::OnResetDevice()
{
	CreateBuffer();
}

void CTexUVOut::CTexturedRectangle::OnLostDevice()
{
	ReleaseBuffer();
}

void CTexUVOut::CTexturedRectangle::ReleaseBuffer()
{
	SAFE_RELEASE( m_pVertexDecl );
	SAFE_RELEASE( m_pVB );
	SAFE_RELEASE( m_pIB );		
}

HRESULT CTexUVOut::CTexturedRectangle::CreateBuffer()
{
	HRESULT hr;	


		D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
		V_RETURN( D3DXDeclaratorFromFVF(FVF_CUSTOMVERTEX, decl) );
		V_RETURN( g_pKTDXApp->GetDevice()->CreateVertexDeclaration(decl, &m_pVertexDecl) );

		// Create the vertex buffer
		V_RETURN( g_pKTDXApp->GetDevice()->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),
			D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, FVF_CUSTOMVERTEX, 
			D3DPOOL_DEFAULT, &m_pVB, NULL) );

		const float size = m_fRectSize;		
		CUSTOMVERTEX* pVertices=0;
		V_RETURN( m_pVB->Lock(0, 0, (VOID**)&pVertices, 0 ) );
		{
			pVertices[0].p = D3DXVECTOR3( 0, 0, 0);			// left_bottom
			pVertices[1].p = D3DXVECTOR3( size, 0, 0);		// right_bottom
			pVertices[2].p = D3DXVECTOR3( size, size, 0);	// right_top
			pVertices[3].p = D3DXVECTOR3( 0,  size, 0);		// left_top

			pVertices[0].tu = 0.f; pVertices[0].tv = 1.f;
			pVertices[1].tu = 1.f; pVertices[1].tv = 1.f;
			pVertices[2].tu = 1.f; pVertices[2].tv = 0.f;
			pVertices[3].tu = 0.f; pVertices[3].tv = 0.f;

			pVertices[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertices[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertices[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertices[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

			m_pVB->Unlock();
		}

		// create index buffer
		V_RETURN( g_pKTDXApp->GetDevice()->CreateIndexBuffer( 6*sizeof(DWORD), 
			D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB, NULL) );

		WORD* pIndices=0;
		V_RETURN( m_pIB->Lock(0, 0, (void**)&pIndices, 0) );
		{
			*(pIndices++) = 0;	
			*(pIndices++) = 2;	
			*(pIndices++) = 1;
			*(pIndices++) = 0;	
			*(pIndices++) = 3;	
			*(pIndices++) = 2;	
			m_pIB->Unlock();
		}


	

	return hr;
}

HRESULT CTexUVOut::CTexturedRectangle::SetVertexPos( int index, D3DXVECTOR3 vPos, D3DXCOLOR color )
{
	HRESULT hr;
	CUSTOMVERTEX* pVertices=0;

	int vTempPosX = (int)vPos.x;
	int vTempPosY = (int)vPos.y;

	D3DCOLOR d3dColor = color;

	
		V_RETURN( m_pVB->Lock(0, 0, (VOID**)&pVertices, 0 ) );
		{
			switch( index )
			{
			case 0: // left top
				{
					pVertices[3].x = (float)vTempPosX;
					pVertices[3].y = (float)vTempPosY;

					if ( d3dColor == 0xffffffff )
						pVertices[3].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 255);
					else
						pVertices[3].color = color;
					//pVertices[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				} break;

			case 1: // right top
				{
					pVertices[2].x = (float)vTempPosX;
					pVertices[2].y = (float)vTempPosY;

					if ( d3dColor == 0xffffffff )
						pVertices[2].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 255);
					else
						pVertices[2].color = color;

					//pVertices[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				} break;

			case 2: // left bottom
				{
					pVertices[0].x = (float)vTempPosX;
					pVertices[0].y = (float)vTempPosY;

					if ( d3dColor == 0xffffffff )
						pVertices[0].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 255);
					else
						pVertices[0].color = color;

					//pVertices[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				} break;

			case 3: // right bottom
				{
					pVertices[1].x = (float)vTempPosX;
					pVertices[1].y = (float)vTempPosY;

					if ( d3dColor == 0xffffffff )
						pVertices[1].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 255);
					else
						pVertices[1].color = color;

					//pVertices[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				} break;
			}		

			m_pVB->Unlock();
		}


	

	return S_OK;
}



HRESULT	CTexUVOut::CTexturedRectangle::AddVertexPos( int index, D3DXVECTOR2 vPos )
{
	HRESULT hr;
	CUSTOMVERTEX* pVertices=0;

		V_RETURN( m_pVB->Lock(0, 0, (VOID**)&pVertices, 0 ) );
		{
			switch( index )
			{
			case 0: // left top
				{
					pVertices[3].x += vPos.x;
					pVertices[3].y += vPos.y;
					pVertices[3].color = D3DCOLOR_RGBA(0, rand()%256, rand()%256, 0);
					//pVertices[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				} break;

			case 1: // right top
				{
					pVertices[2].x += vPos.x;
					pVertices[2].y += vPos.y;
					pVertices[2].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 0);
					//pVertices[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				} break;

			case 2: // left bottom
				{
					pVertices[0].x += vPos.x;
					pVertices[0].y += vPos.y;
					pVertices[0].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 0);
					//pVertices[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				} break;

			case 3: // right bottom
				{
					pVertices[1].x += vPos.x;
					pVertices[1].y += vPos.y;
					pVertices[1].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 0);
					//pVertices[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				} break;
			}		

			m_pVB->Unlock();
		}


	


	return S_OK;
}

D3DXVECTOR3 CTexUVOut::CTexturedRectangle::GetVertexPos( int index )
{
	D3DXVECTOR3 vertexPos;
	
	

	CUSTOMVERTEX* pVertices=0;


		m_pVB->Lock(0, 0, (VOID**)&pVertices, 0 );


		{
			switch( index )
			{
			case 0: // left top
				{
					/*
					pVertices[3].x = (float)vTempPosX;
					pVertices[3].y = (float)vTempPosY;
					pVertices[3].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 255);
					pVertices[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);
					*/

					vertexPos.x = pVertices[3].x;
					vertexPos.y = pVertices[3].y;
					vertexPos.z = pVertices[3].z;
				} break;

			case 1: // right top
				{
					vertexPos.x = pVertices[2].x;
					vertexPos.y = pVertices[2].y;
					vertexPos.z = pVertices[2].z;
				} break;

			case 2: // left bottom
				{
					vertexPos.x = pVertices[0].x;
					vertexPos.y = pVertices[0].y;
					vertexPos.z = pVertices[0].z;
				} break;

			case 3: // right bottom
				{
					vertexPos.x = pVertices[1].x;
					vertexPos.y = pVertices[1].y;
					vertexPos.z = pVertices[1].z;
				} break;
			}		

			m_pVB->Unlock();
		}
	

	


	return vertexPos;
}

void CTexUVOut::CTexturedRectangle::ReleaseTexture()
{
	/*
	if( NULL != m_pTexture )
		m_pTexture->UnLoad();
	SAFE_DELETE( m_pTexture );
	*/

	SAFE_RELEASE( m_pTexture );
}

bool CTexUVOut::CTexturedRectangle::CreateTexture(LPCWSTR pFilename)
{
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];
	WCHAR wszFileName[_MAX_FNAME] = {0};

	_wsplitpath( pFilename, drive, dir, fname, ext);
	wcscat( wszFileName, fname);
	wcscat( wszFileName, ext);	
/*
	m_pTexture = new CKTDXDeviceTexture( g_pKTDXApp->GetDevice(), wszFileName );
	if( NULL != m_pTexture )
		m_pTexture->Load();
*/
	
	//wstring datPath = L"dat/";
	//datPath += wszFileName;

	if( FAILED( D3DXCreateTextureFromFileEx( g_pKTDXApp->GetDevice(), pFilename,
		D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0,
		NULL, NULL,
		&m_pTexture ) ) )
	{
		if( FAILED( D3DXCreateTextureFromFileEx( g_pKTDXApp->GetDevice(), pFilename,
			D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_X8R8G8B8,
			D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0,
			NULL, NULL,
			&m_pTexture ) ) )
		{
			MessageBox(NULL, L"Could not find texture", L"에러", MB_OK);
			return false;
		}
	}
	
	return true;
}

bool CTexUVOut::CTexturedRectangle::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	return true;
}


HRESULT CTexUVOut::CTexturedRectangle::OnFrameRender()
{

	if( m_pTexture != NULL )
	{
		//m_pTexture->SetDeviceTexture( 0 );
		g_pKTDXApp->GetDevice()->SetTexture( 0, m_pTexture );
	}


		D3DXMatrixScaling( &m_matWorld, m_fScale, m_fScale, 1.f );
		g_pKTDXApp->SetWorldTransform( &m_matWorld );
		g_pKTDXApp->GetDevice()->SetStreamSource( 0, m_pVB, 0, sizeof(CUSTOMVERTEX) );
		g_pKTDXApp->GetDevice()->SetFVF( FVF_CUSTOMVERTEX );
		g_pKTDXApp->GetDevice()->SetIndices( m_pIB );
		g_pKTDXApp->GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 );

	

	
	return S_OK;
}


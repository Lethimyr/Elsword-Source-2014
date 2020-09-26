#pragma once

class CX2DialogToolMain : public CKTDXStage
{
public:
	class CTexturedRectangle
	{
	private:		
		struct CUSTOMVERTEX
		{
			union
			{
				struct
				{
					FLOAT   x;    /* Homogeneous coordinates */
					FLOAT   y;
					FLOAT   z;
				};
				D3DVECTOR   p;
			};
			DWORD		color;
			FLOAT		tu;   /* Texture coordinates */
			FLOAT		tv;
		};

	private:
		DWORD	FVF_CUSTOMVERTEX;	
		LPDIRECT3DVERTEXDECLARATION9	m_pVertexDecl;
		LPDIRECT3DVERTEXBUFFER9			m_pVB;  
		LPDIRECT3DINDEXBUFFER9			m_pIB;

		CKTDXDeviceTexture*				m_pTexture;

		D3DXMATRIX m_matWorld, m_matView;
		float m_fScale;
		float m_fRectSize;

	public:
		wstring& GetTextureName() { return m_pTexture->GetDeviceID(); }
		float GetRectSize() { return m_fRectSize; }
		float GetWidth() { return (float)m_pTexture->GetWidth(); }
		float GetHeight() { return (float)m_pTexture->GetHeight(); }
		float GetScale() { return m_fScale; }
		void Rescale( float multiplier ) 
		{ 
			m_fScale *= multiplier;
			
			//D3DXMatrixScaling( &m_matWorld, (float)m_pTexture->GetWidth()*m_fScale, (float)m_pTexture->GetHeight()*m_fScale, 1.f );
			D3DXMatrixScaling( &m_matWorld, m_fScale, m_fScale, 1.f );
			g_pKTDXApp->SetWorldTransform( &m_matWorld );
		}

	//public:
	//	D3DXVECTOR2& GetMouseClickedPosition() { return m_vMouseClickedPosition; }

	public:
		~CTexturedRectangle()
		{
			ReleaseBuffer();
			ReleaseTexture();
		}

		CTexturedRectangle()
		{
			FVF_CUSTOMVERTEX = ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );

			m_pVertexDecl = NULL;
			m_pVB = NULL;
			m_pIB = NULL;
			m_pTexture = NULL;			
			m_fScale = 1.f;
			m_fRectSize = 512.f;
		}

		void OnResetDevice()
		{
			CreateBuffer();
		}

		void OnLostDevice()
		{
			ReleaseBuffer();
		}

		void ReleaseBuffer()
		{
			SAFE_RELEASE( m_pVertexDecl );
			SAFE_RELEASE( m_pVB );
			SAFE_RELEASE( m_pIB );		
		}

		HRESULT CreateBuffer()
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
				//pVertices[0].p = D3DXVECTOR3(-size, -size, 0);
				//pVertices[1].p = D3DXVECTOR3( size, -size, 0);
				//pVertices[2].p = D3DXVECTOR3( size,  size, 0);
				//pVertices[3].p = D3DXVECTOR3(-size,  size, 0);
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

		HRESULT SetVertexPos( int index, D3DXVECTOR3 vPos )
		{
			HRESULT hr;
			CUSTOMVERTEX* pVertices=0;
			V_RETURN( m_pVB->Lock(0, 0, (VOID**)&pVertices, 0 ) );
			{
				switch( index )
				{
				case 0: // left top
					{
						pVertices[3].x = vPos.x;
						pVertices[3].y = vPos.y;
						pVertices[3].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 255);
					} break;

				case 1: // right top
					{
						pVertices[2].x = vPos.x;
						pVertices[2].y = vPos.y;
						pVertices[2].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 255);
					} break;

				case 2: // left bottom
					{
						pVertices[0].x = vPos.x;
						pVertices[0].y = vPos.y;
						pVertices[0].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 255);
					} break;

				case 3: // right bottom
					{
						pVertices[1].x = vPos.x;
						pVertices[1].y = vPos.y;
						pVertices[1].color = D3DCOLOR_RGBA(rand()%256, rand()%256, rand()%256, 255);
					} break;
				}		

				m_pVB->Unlock();
			}

			return S_OK;
		}

		void ReleaseTexture()
		{
			if( NULL != m_pTexture )
				m_pTexture->UnLoad();
			SAFE_DELETE( m_pTexture );
		}

		void CreateTexture(LPCWSTR pFilename)
		{
			WCHAR drive[_MAX_DRIVE];
			WCHAR dir[_MAX_DIR];
			WCHAR fname[_MAX_FNAME];
			WCHAR ext[_MAX_EXT];
			WCHAR wszFileName[_MAX_FNAME];
			
			_wsplitpath( pFilename, drive, dir, fname, ext);
			wcscat( wszFileName, fname);
			wcscat( wszFileName, ext);	

			m_pTexture = new CKTDXDeviceTexture( g_pKTDXApp->GetDevice(), wszFileName );
			if( NULL != m_pTexture )
				m_pTexture->Load();

			//D3DXMatrixScaling( &m_matWorld, (float)m_pTexture->GetWidth(), (float)m_pTexture->GetHeight(), 1.f );
			//g_pKTDXApp->SetWorldTransform( &m_matWorld );

			//RECT rc = DXUTGetWindowClientRect();
			//D3DVIEWPORT9 vp;
			//vp.X = 0;
			//vp.Y = 0;
			//vp.Width = m_pTexture->GetWidth();//rc.right-rc.left;
			//vp.Height = m_pTexture->GetHeight();//rc.bottom-rc.top;
			//vp.MinZ = 0.01f;
			//vp.MaxZ = 12000.f;

			//g_pKTDXApp->GetDevice()->SetViewport( &vp );
			////g_pKTDXApp->SetViewport( 0, 0, rc.right-rc.left, rc.bottom-rc.top, 0.01f, 12000.f );


			//g_pKTDXApp->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0,0,0,0), 1.0f, 0);
		
		}

		bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
		{
		
			return true;
		}


		HRESULT OnFrameRender()
		{
			//CKTDGStateManager::PushRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
			//CKTDGStateManager::PushRenderState( D3DRS_CULLMODE, D3DCULL_NONE );				
			//CKTDGStateManager::PushRenderState( D3DRS_LIGHTING, FALSE );
			//CKTDGStateManager::PushRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			//CKTDGStateManager::PushRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			//CKTDGStateManager::PushRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			//
			//CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
			//CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			//CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
			//CKTDGStateManager::PushTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
			//CKTDGStateManager::PushTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			//CKTDGStateManager::PushTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

			//g_pKTDXApp->GetDevice()->SetVertexShader( NULL );
			//g_pKTDXApp->GetDevice()->SetPixelShader( NULL );

			//D3DXMatrixIdentity(&matWorld);

			////D3DXMATRIX worldMatrix, viewMatrix, projMatrix, viewProjMatrix;

			////g_pKTDXApp->GetWorldTransform( &worldMatrix );
			////g_pKTDXApp->GetViewTransform( &viewMatrix );
			////g_pKTDXApp->GetProjectionTransform( &projMatrix );

			//g_pKTDXApp->SetWorldTransform( &matWorld );
			//g_pKTDXApp->GetDGManager()->GetCamera()->Point( 0,0,-700, 0,0,0 );
			//
			//g_pKTDXApp->SetViewTransform( (D3DXMATRIX*) &g_pKTDXApp->GetDGManager()->GetCamera()->GetViewMatrix() );
			//g_pKTDXApp->GetDGManager()->SetProjection( 0.01f, 12000.f, false );

			//g_pKTDXApp->SetViewTransform()




			if( NULL != m_pTexture )
				m_pTexture->SetDeviceTexture( 0 );

			//g_pKTDXApp->GetDevice()->SetVertexDeclaration( m_pVertexDecl );
			g_pKTDXApp->GetDevice()->SetStreamSource( 0, m_pVB, 0, sizeof(CUSTOMVERTEX) );
			g_pKTDXApp->GetDevice()->SetFVF( FVF_CUSTOMVERTEX );
			g_pKTDXApp->GetDevice()->SetIndices( m_pIB );
			g_pKTDXApp->GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 );
			

			//g_pKTDXApp->SetWorldTransform( &worldMatrix );

			//CKTDGStateManager::PopTextureStageState( 0, D3DTSS_ALPHAARG2 );
			//CKTDGStateManager::PopTextureStageState( 0, D3DTSS_ALPHAARG1 );
			//CKTDGStateManager::PopTextureStageState( 0, D3DTSS_ALPHAOP );
			//CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
			//CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
			//CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );

			//CKTDGStateManager::PopRenderState( D3DRS_DESTBLEND );
			//CKTDGStateManager::PopRenderState( D3DRS_SRCBLEND );
			//CKTDGStateManager::PopRenderState( D3DRS_ALPHABLENDENABLE );
			//CKTDGStateManager::PopRenderState( D3DRS_LIGHTING );
			//CKTDGStateManager::PopRenderState( D3DRS_CULLMODE );
			//CKTDGStateManager::PopRenderState( D3DRS_FILLMODE );
		

			return S_OK;
		}

	};


public:
	enum MODE
	{
		MODE_NORMAL,
		MODE_TEXTURE_SAMPLING,
	};

	enum DIALOG_ID
	{
		DLG_MAIN = 0,
		DLG_DIALOG0,

		DLG_BUTTON,
		//DLG_CONTROL_STATIC,
	};

	enum DIALOG_CONTROL_ID
	{
		IDC_STATIC_DUMMY,

		//IDC_BUTTON_SHOW_DIALOG,

		IDC_BUTTON_FILE_OPEN,
		IDC_BUTTON_FILE_SAVE,

		IDC_BUTTON_REMOVE_ALL_CONTROL,
		IDC_BUTTON_ADD_CONTROL,		
		IDC_COMBO_CONTROL_TYPE,

		IDC_LIST_CONTROLS,
		IDC_EDIT_CONTROL_NAME,

		//IDC_EDIT_BUTTON_NORMAL_TEX_NAME,
		//IDC_EDIT_BUTTON_NORMAL_TEX_LEFT_TOP,
		//IDC_EDIT_BUTTON_NORMAL_TEX_RIGHT_TOP,
		//IDC_EDIT_BUTTON_NORMAL_TEX_LEFT_BOTTOM,
		//IDC_EDIT_BUTTON_NORMAL_TEX_RIGHT_BOTTOM,


		//IDC_EDIT_NORMAL_POINT_LEFT_TOP,
		//IDC_EDIT_NORMAL_POINT_RIGHT_TOP,
		//IDC_EDIT_NORMAL_POINT_LEFT_BOTTOM,
		//IDC_EDIT_NORMAL_POINT_RIGHT_BOTTOM,
		
		IDC_BUTTON_TEXTURE_FILE_OPEN,
		IDC_BUTTON_TEXTURE_COORD_TO_NOTEPAD,
		IDC_EDIT_TEX_LEFT_TOP,
		IDC_EDIT_TEX_RIGHT_TOP,
		IDC_EDIT_TEX_LEFT_BOTTOM,
		IDC_EDIT_TEX_RIGHT_BOTTOM,

		IDC_EDIT_POINT_LEFT_TOP,
		IDC_EDIT_POINT_RIGHT_TOP,
		IDC_EDIT_POINT_LEFT_BOTTOM,
		IDC_EDIT_POINT_RIGHT_BOTTOM,
		IDC_EDIT_POINT_COLOR,
		IDC_EDIT_POINT_CHANGE_TIME,

		IDC_BUTTON_BUTTON_SET_NORMAL_TEXTURE,
		IDC_BUTTON_BUTTON_SET_OVER_TEXTURE,
		IDC_BUTTON_BUTTON_SET_DOWN_TEXTURE,
	};

public:
	CX2DialogToolMain(void);
	~CX2DialogToolMain(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void OpenScript( const WCHAR* scriptFilename = L"DialogTool.lua" );
	bool SaveScritp2File( WCHAR* wszFileName );
	bool DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath );

	void FinalDialog();
	void InitDialog();

	void UpdateControlListBox();
	
	void SetPickedControl( CKTDGUIControl* pControl ) { m_pControlPicked = pControl; }
	CKTDGUIControl* GetPickedControl() { return m_pControlPicked; }
	CDXUTDialog* GetDialog( DIALOG_ID dlgID ) { return m_vecpDialog[dlgID]; }
	bool& GetShowDialog() { return m_bShowDialog; }
	CKTDGUIDialogType GetDialog() { return m_pDialog; }
	void SetDialog( CKTDGUIDialogType pDialog ) { m_pDialog = pDialog; }	
	void CreateNewDialog() 
	{ 
		if( NULL != m_pDialog ) SAFE_DELETE_DIALOG( m_pDialog ); 
		m_pDialog = new CKTDGUIDialog( this, L"NewDialog" ); 
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDialog );
	}

	CTexturedRectangle& GetTexturedRectangle() { return m_TexturedRectangle; }
	CTexturedRectangle& GetWiredRect() { return m_WiredRect; }
	CDXUTEditBox* GetLastFocusedEditBox() { return m_pLastFocusedEditBox; }
	void SetLastFocusedEditBox( CDXUTEditBox* pEditBox ) { m_pLastFocusedEditBox = pEditBox; }
	D3DXVECTOR2* GetTexCoords() { return &m_vUVTexCoord[0]; }	
	
private:
	KLuaManager*			m_pLuaManager;
	vector<CDXUTDialog*>	m_vecpDialog;
	CDXUTDialog*			m_pTextureSamplingDialog;
	
	CKTDGUIDialogType			m_pDialog;
	bool					m_bShowDialog;
	int						m_iMode;

	CKTDGUIControl*			m_pControlPicked;
	CKTDGUIControl*			m_pControlDragging;
	POINTS					m_OldMousePos;


	CTexturedRectangle		m_WiredRect;
	CTexturedRectangle		m_TexturedRectangle;	
	D3DXVECTOR2				m_vUVTexCoord[4];
	CDXUTEditBox*			m_pLastFocusedEditBox;	
	bool					m_bRButtonDragging;
};










//class CFileRenameTool : public CKTDXStage
//{
//
//public:
//	enum DIALOG_CONTROL_ID
//	{
//		IDC_STATIC_DUMMY,
//		IDC_BUTTON_HELP,
//
//		IDC_EDIT_FILE_EXTENSION,
//		IDC_EDIT_PREFIX_OR_SUFFIX,
//		IDC_BUTTON_ATTACH_OR_DETACH,
//		IDC_BUTTON_PREFIX_OR_SUFFIX,
//	};
//
//	enum
//	{
//		MODE_ATTACH,
//		MODE_DETACH,
//		MODE_REPLACE,
//		STATE_PREFIX,
//		STATE_SUFFIX
//	};
//
//public:
//	CFileRenameTool(void);
//	~CFileRenameTool(void);
//
//	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
//	virtual HRESULT OnFrameRender();
//
//	virtual HRESULT OnResetDevice();
//	virtual HRESULT OnLostDevice();
//
//	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//	int GetMode() { return m_iMode; }
//	int GetState() { return m_iState; }
//	void SetMode(int iMode) { m_iMode = iMode; }
//	void SetState(int iState) { m_iState = iState; }
//
//	wstring& GetFileExtension() { return m_wstrFileExtension; }
//	wstring& GetPrefixOrSuffix() { return m_wstrPrefixOrSuffix; }
//	void SetFileExtension( const WCHAR* wszExtension ) { m_wstrFileExtension = wszExtension; }
//	void SetPrefixOrSuffix( const WCHAR* wszPrefixOrSuffix ) { m_wstrPrefixOrSuffix = wszPrefixOrSuffix; }	
//
//protected:
//	void DetachPrefixFromFileName( WCHAR* wszFilePath, const WCHAR* wszPrefix );
//	void AttachPrefixToFileName( WCHAR* wszFilePath, const WCHAR* wszPrefix );
//	void ReplaceSubString( WCHAR* wszFilePath, const WCHAR* wszOriginal, const WCHAR* wszReplacement );
//
//	void InitDialog();
//	void FinalDialog();
//
//private:
//	wstring m_wstrPrefixOrSuffix;
//	wstring m_wstrFileExtension;
//
//	int m_iMode;
//	int m_iState;
//
//	vector<CDXUTDialog*>	m_vecDialog;
//};

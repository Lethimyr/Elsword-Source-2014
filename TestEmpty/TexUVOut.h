#pragma once

#define IDC_EDITBOX1            5

class CTexUVOut : public CKTDXStage
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

				struct CUSTOMVERTEX2
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
				};


			private:
				DWORD	FVF_CUSTOMVERTEX;	
				LPDIRECT3DVERTEXDECLARATION9	m_pVertexDecl;
				LPDIRECT3DVERTEXBUFFER9			m_pVB;  
				LPDIRECT3DINDEXBUFFER9			m_pIB;

				//CKTDXDeviceTexture*				m_pTexture;
				LPDIRECT3DTEXTURE				m_pTexture;

				D3DXMATRIX m_matWorld, m_matView;
				float m_fScale;
				float m_fRectSize;

			public:
				//wstring& GetTextureName() { return m_pTexture->GetDeviceID(); }
				float GetRectSize() { return m_fRectSize; }
				float GetWidth();
				float GetHeight();
				float GetScale() { return m_fScale; }
				void Rescale( float multiplier ) 
				{ 
					m_fScale *= multiplier;
				}

				void SetScale( float fScale )
				{
					m_fScale = fScale;
				}

			public:

				CTexturedRectangle();
				~CTexturedRectangle();

				void OnResetDevice(  );
				void OnLostDevice();

				void ReleaseBuffer();
				HRESULT CreateBuffer();

				HRESULT SetVertexPos( int index, D3DXVECTOR3 vPos, D3DXCOLOR color = 0xffffffff );
				HRESULT	AddVertexPos( int index, D3DXVECTOR2 vPos );
				
				D3DXVECTOR3		GetVertexPos( int index );


				void				ReleaseTexture();
				bool				CreateTexture(LPCWSTR pFilename);
				LPDIRECT3DTEXTURE	GetTexture(){ return m_pTexture; }
				bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

				HRESULT				OnFrameRender();
		};
/*
		class CTexUVEdit
		{
			public:

				CTexUVPoint();
				~CTexUVPoint();

				virtual void ReInit();

				virtual void OnFrameMove( double fTime, float fElapsedTime );
				virtual void OnFrameRender();

				virtual void OnResetDevice();
				virtual void OnLostDevice();

			protected:

				D3DXVECTOR2				m_vUVTexCoord[4];

		};

		class CTexUVEditPoint : public CTexUVEdit
		{
			public:
				
				CTexUVPoint();
				~CTexUVPoint();

				void ReInit();

				void OnFrameMove( double fTime, float fElapsedTime );
				void OnFrameRender();

				void OnResetDevice();
				void OnLostDevice();

			private:

				CTexturedRectangle		m_TexturedRectangle;
				CTexturedRectangle		m_WiredRect;


		}
*/
	public:

		struct DRAW_VERTEX
		{
			D3DXVECTOR4	vPos;	// x, y, z, rhw
			D3DCOLOR    color;	// diffuse color
			float		u, v;	// tu, tv

			enum _FVF
			{
				FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1,
			};
		};

		struct LINE_VERTEX
		{
			D3DXVECTOR4	vPos;
			D3DCOLOR	color;

			enum _FVF
			{
				FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
			};
		};

		struct LINE_POS
		{
			D3DXVECTOR2	vPos;
			D3DXVECTOR2	vSize;
			bool		bIsDraw;

			LINE_POS()
			{
				vPos = D3DXVECTOR2( 0,0 );
				vSize = D3DXVECTOR2( 0,0 );
				bIsDraw = false;
			};
		};

	public:

		enum EDIT_MODE
		{
			EM_POINT = 0,
			EM_RECTANGLE_AUTO,
			EM_RECTANGLE_MANUAL,
		};

		enum POINT_TYPE
		{
			LEFT_TOP = 0,
			RIGHT_TOP,
			LEFT_BOTTOM,
			RIGHT_BOTTOM,
		};

		enum TEX_UV_OUT_UI_CUSTOM_EVENT
		{
			TUOUCE_EDIT_MODE_CHANGED = 0,
			TUOUCE_POINT_TYPE_CHANGED,
			TUOUCE_RECT_ADD,
			TUOUCE_RECT_OUT,
			TUOUCE_RECT_DELETE,
			TUOUCE_RECT_LIST_BOX_SELECTED,
		};


		struct TET_DATA		//tet데이타 지워줄때 리스트 박스컨트롤에 등록된 아이템까지 찾아서 지워줘야한다 이 tet포인터를 가지고 있어서
		{
			wstring key;
			CKTDXDeviceTexture::TEXTURE_UV* pTexUV;

			TET_DATA()
			{
				pTexUV = NULL;
			}
			~TET_DATA()
			{
				SAFE_DELETE( pTexUV );
			}
		};

	public:

		CTexUVOut(void);
		virtual ~CTexUVOut(void);

		virtual	void	ReInit();

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		CDXUTDialog* GetDialog() { return &m_SampleUI; }

	private:

		virtual	void	TextRender();
				
				bool	AddTETData();
				bool	DeleteSelecetedTETData();
				bool	ExtractTETDataList();
				bool	TETFileLoad( const WCHAR* pFullFileName );

				void	EditPoint();

				bool	CatchFile( WPARAM wParam );
		virtual	bool	CreateTexture( const WCHAR* pFileName );

				bool	OpenScriptFile( const WCHAR* pFileName );
		D3DXVECTOR2*	GetTexCoords() { return &m_vUVTexCoord[0]; }

				void	TextureFilltering();
				void	SetPoint( POINT& pt, int x, int y );

				void	DrawFace( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color  = 0xffffffff  );
				void	DrawLine( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color  = 0xffffffff  );

		




	private:

		EDIT_MODE				m_EditMode;

		bool					m_bLoadTexture;
		wstring					m_LoadedTextureName;			//aaa.tga를 불러들였으면 aaa만 저장됨.

		CTexturedRectangle		m_TexturedRectangle;
		CTexturedRectangle		m_WiredRect;

		CTexturedRectangle		m_OutlineLeftRect;
		CTexturedRectangle		m_OutlineTopRect;
		CTexturedRectangle		m_OutlineRightRect;
		CTexturedRectangle		m_OutlineBottomRect;

		D3DCOLOR				m_LineColor;

		bool					m_bRButtonDragging;

		CKTDGUIDialogType			m_pDialog;

		vector< TET_DATA* >		m_TETDataList;

		D3DXVECTOR2				m_vUVTexCoord[4];
		D3DXVECTOR2				m_vMousePos[4];

		int						m_RenderStateID;
		int						m_RenderStateID2;
		LINE_POS				m_sLinePos;	//라인정보
		bool					m_bDraggingRectAuto;
		D3DXVECTOR2				m_vPos; //텍스쳐 위치
		D3DXVECTOR2				m_vTextureScale;
		D3DXVECTOR2				m_vTextureOrgScale;
		bool					m_bLButtonDown;

		POINT_TYPE						m_NowPointType;
		CKTDGFontManager::CKTDGFont*	m_pFont;
		
		CKTDXDeviceTexture*				m_pDeviceTextureRect;

		RECT							m_rectAutoRect;

		CDXUTDialog						m_SampleUI;
		CDXUTIMEEditBox*				m_pIMEEditBox;
};

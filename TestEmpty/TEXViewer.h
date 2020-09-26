#pragma once

class CTEXViewer : public CKTDXStage
{
	public:

		enum VIEWER_DLG_MESSAGE
		{
			VDM_PREV = 0,
			VDM_NEXT,
		};

		struct TEX_DATA
		{
			wstring texFileName;
			wstring keyName;
			CKTDXDeviceTexture::TEXTURE_UV* pTexUV;

			TEX_DATA()
			{
				pTexUV = NULL;
			}
			~TEX_DATA()
			{
				SAFE_DELETE( pTexUV );
			}
		};

	public:

		CTEXViewer(void);
		virtual ~CTEXViewer(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	private:
				void	ReInit();
				
				void	UIRender();
				
				bool	OpenScriptFile( const WCHAR* pFileName );
				bool	CatchFile( WPARAM wParam );

				bool	AddTexture( const WCHAR* pFileName );

				bool	IsMousePointInPicture( D3DXVECTOR2& pt, int& texDataIndex );
				bool	IsMousePointInEnlargePicture( D3DXVECTOR2&pt );

				bool	IsInMousePoint( D3DXVECTOR2 pt, int x, int y, int width, int height );


			
	private:

		int								m_NowViewPage;

		vector< TEX_DATA* >				m_TexDataList;
		vector< CKTDXDeviceTexture* >	m_TextrueDeviceList;

		CKTDGUIDialogType					m_pDialog;
		CKTDGFontManager::CKTDGFont*	m_pFont;

		bool							m_bClickedPicture;
		int								m_SelectedTexDataIndex;
		
		bool							m_bEnlargeTexture;
		CKTDXDeviceTexture*				m_pEnlargeTexture;
		int								m_EnlargeTexDataIndex;
		int								m_EnlargePercent;
		D3DXVECTOR2						m_EnlargeTexPos;
		bool							m_bEnlargeTexDragging;

};

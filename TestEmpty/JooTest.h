#pragma once


class CJooViewerUI : public CKTDGObject
{
	public:

		CJooViewerUI( const WCHAR* pMotionFileName );
		~CJooViewerUI();

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		void PlayAnim( const WCHAR* pName, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType, bool bTransition = false );

	private:

		CKTDXDeviceXSkinMesh*		m_pMotion;
		CKTDXDeviceXSkinMesh*		m_pBodyModel;
		CKTDGXSkinAnim*				m_pXSkinAnim;

		D3DXVECTOR3					m_LightPos;	
		CKTDGMatrix::BILLBOARD_TYPE	m_BillBoardType;
};

#define D3DFVF_LINE_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

class CJooTest : public CKTDXStage
{
	public:
		struct LINE_VERTEX
		{
			D3DXVECTOR3	pos;
			DWORD		color;
		};

		enum SELECTED_OBJECT
		{	
			SO_CAMERA = 0,
			SO_MESH,
			SO_LINE1,
			SO_LINE2,
		};

	public:

		CJooTest(void);
		~CJooTest(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){ return false; }

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();


	private:

		CJooViewerUI*			m_pJooViewerUI;
		CKTDGXSkinAnim*			m_pXSkinAnim;

		CKTDXDeviceXMesh*		m_pTestMesh;

		LPDIRECT3DVERTEXBUFFER9	m_pLineVB;
		D3DXVECTOR3				m_StartPos;
		D3DXVECTOR3				m_EndPos;
		//CX2World*				m_pX2World;
		//CX2WorldObjectSkinMesh* m_pWOSkinMesh;
		LPDIRECT3DVERTEXBUFFER9	m_pLineVB2;
		D3DXVECTOR3				m_StartPos2;
		D3DXVECTOR3				m_EndPos2;

		bool					m_bCollision;
		CKTDGFontManager::CKTDGFont*				m_pFont;

		int						m_SelectedOBJ;

		D3DXVECTOR3				m_MeshPos;
		D3DXVECTOR3				m_MeshRot;

		D3DXMATRIX				m_matMeshWorld;

		CKTDXDeviceXSkinMesh*	m_pDeviceXSkinMesh;
};

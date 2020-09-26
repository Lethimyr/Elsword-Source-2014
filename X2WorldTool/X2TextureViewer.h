#pragma once

class CX2TextureViewer : public CKTDXStage
{
public:
	enum DIALOG_CONTROL_ID
	{
		IDC_STATIC_DUMMY,
		IDC_EDIT_CENTER_POS, 
		IDC_EDIT_SIZE, 
	};


public:
	CX2TextureViewer(void);
	virtual ~CX2TextureViewer(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


public:
	void FinalDialog();
	void InitDialog();
	void UpdateDialog();
	bool OnDropFile( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath );



	CDXUTDialog& GetMainDialog() { return m_MainDialog; }

	D3DXVECTOR2 GetLeftTopPos() const { return m_vLeftTopPos; }
	void SetLeftTopPos(D3DXVECTOR2 val) { m_vLeftTopPos = val; }

	D3DXVECTOR2 GetSize() const { return m_vSize; }
	void SetSize(D3DXVECTOR2 val) { m_vSize = val; }
	void ParseIntArrayFromEditBox( CDXUTEditBox* pEditBox, int *pNumbers, int nCount );
	void SetTextIntArrayToEditBox( CDXUTEditBox* pEditBox, const int *pNumbers, int nCount );
	
private:
	CFirstPersonCamera	m_FPSCamera;
	CKTDXDeviceTexture* m_pDeviceTexture;


	D3DXVECTOR2 m_vLeftTopPos;
	D3DXVECTOR2 m_vSize;
	CDXUTDialog	m_MainDialog;
};



















//////////////////////////////////////////////////////////////////////////
class CX2NPCTool : public CKTDXStage
{
public:
	enum DIALOG_CONTROL_ID
	{
		IDC_STATIC_DUMMY,
		IDC_EDIT_CENTER_POS, 
		IDC_EDIT_SIZE, 
	};

	struct NPCData
	{
		wstring	NAME;
		wstring	MOTION_FILE_NAME;
		wstring	MOTION_ANI_TEX_XET;
		wstring	MOTION_CHANGE_TEX_XET;

		bool	USE_SLASH_TRACE;
		bool	DRAW_SMALL_MP_BAR;

		bool	STATE_WIN;
		bool	STATE_WAITHABIT;
		bool	STATE_WAITSTART;
		bool	STATE_JUMP_READY;
		bool	STATE_DASH;
		bool	STATE_DASH_END;

		bool	STATE_START_CAMERA;	// start state에서 camera_frame_move함수가 있는지
	};

	struct NPCTemplet
	{
		wstring INIT_SYSTEM;
		wstring INIT_DEVICE;
		wstring INIT_PHYSIC;
		wstring INIT_COMPONENT;
		wstring INIT_AI;

		vector<wstring> READY_TEXTURE;
		vector<wstring> READY_SOUND;
		vector<wstring> READY_XMESH;
		vector<wstring> READY_XSKIN_MESH;

		void Init()
		{
			INIT_SYSTEM			= L"";
			INIT_DEVICE			= L"";
			INIT_PHYSIC			= L"";
			INIT_COMPONENT		= L"";
			INIT_AI				= L"";	

			READY_TEXTURE.clear();
			READY_SOUND.clear();
			READY_XMESH.clear();
			READY_XSKIN_MESH.clear();
		}
	};


public:
	CX2NPCTool(void);
	virtual ~CX2NPCTool(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


public:
	void FinalDialog();
	void InitDialog();
	void UpdateDialog();
	bool OnDropFile( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath );

	CDXUTDialog& GetMainDialog() { return m_MainDialog; }

	void ParseIntArrayFromEditBox( CDXUTEditBox* pEditBox, int *pNumbers, int nCount );
	void SetTextIntArrayToEditBox( CDXUTEditBox* pEditBox, const int *pNumbers, int nCount );
	
	void RegisterLuabind();
	bool OpenScriptFile( const WCHAR* pFileName );
	void SaveNPCScript();
	bool LoadNPCTemplet( const WCHAR* pFileName );

	bool FindTable( wstring& wstrText, WCHAR* wszTableName, int& iStartIndex, int& iEndIndex );


private:
	CDXUTDialog			m_MainDialog;
	NPCTemplet			m_NPCTemplet;
	vector<NPCData>		m_vecNPCData;

};


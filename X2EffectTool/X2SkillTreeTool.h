#pragma once


class CX2SkillTreeTool : public CKTDXStage
{
public:
	enum SKILL_TREE_TOOL_UI_CUSTOM_MSG
	{
		STTUCM_SKILL_ICON_CLICKED,
		STTUCM_SKILL_ICON_DOUBLE_CLICKED,
		STTUCM_SKILL_ICON_RIGHT_CLICKED,
		STTUCM_SKILL_ICON_MOUSE_OVER,
		STTUCM_SKILL_ICON_MOUSE_OUT,

		STTUCM_SKILL_LIST_SELECTED,
		STTUCM_SKILL_LIST_CLOSE,
	};

	enum DIALOG_CONTROL_ID
	{
		IDC_STATIC_DUMMY,
		IDC_EDIT_SKILL_ID,
		IDC_EDIT_TEXTURE_NAME,
		IDC_EDIT_TEXTURE_PIECE_NAME,
		IDC_EDIT_GRID_INDEX,
		IDC_EDIT_SKILL_NAME,
		IDC_LIST_NECESSARY_SKILL_ID,

		IDC_BUTTON_OPEN_TEXTURE,
		IDC_LIST_TEXTURE_PIECE_NAME,

		IDC_BUTTON_HELP,
		IDC_BUTTON_OPEN_SKILL_TEMPLET,
		IDC_BUTTON_SAVE_SKILL_TEMPLET,
		IDC_BUTTON_SAVE_SKILL_EXCEL,
		IDC_COMBO_UNIT_CLASS,
		IDC_EDIT_GRID_SIZE,
		IDC_EDIT_MOVE_ALL_GRID,
		IDC_LIST_AVAILABLE_SKILL,
	};

	enum DIRECTION
	{
		DIR_UP,
		DIR_DOWN, 
		DIR_LEFT,
		DIR_RIGHT,
	};

	struct SKILL_GRID
	{
		CX2SkillTree::SKILL_ID m_eSkillID;
		CKTDGUIStatic::CPictureData* m_pBeforeSkillMarkPicture;

		SKILL_GRID()
		{
			m_eSkillID = CX2SkillTree::SI_NONE;
			m_pBeforeSkillMarkPicture = NULL;
		};
	};


public:
	CX2SkillTreeTool(void);
	virtual ~CX2SkillTreeTool(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool	UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


public:
	void CreateSkillTreeDialog( D3DXVECTOR2 vGridSize );
	bool OpenSkillTemplet( const WCHAR* wszFileName );
	bool SaveSkillTemplet( const WCHAR* wszFilePath );
	bool SaveExcel( const WCHAR* wszFilePath );

	void FinalDialog() {};
	void InitDialog();
	CDXUTDialog& GetMainDialog() { return m_MainDialog; }
	CDXUTDialog& GetPropertyDialog() { return m_PropertyDialog; }
	void UpdatePropertyDialog();
	void UpdateGridUI( int x, int y );
	void UpdateAllGridUI();

	D3DXVECTOR2 GetPickedGridIndex() const { return m_vPickedGridIndex; }
	CX2SkillTree::SkillTemplet* GetPickedSkillTemplet();
	wstring GetNameTextureIcon() const { return m_wstrNameTextureIcon; }

	CX2SkillTree* GetSkillTree() const { return m_pSkillTree; }

	void ResizeGrid( int x, int y );
	void RegisterLuabind();
	bool OpenScriptFile( const WCHAR* pFileName );
	//void UpdateDialog();
	bool OnDropFile( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath );
	void OpenSkillListWindow( bool bOpen );
	
	void UpdateSkillListWindow( bool bCheckBeforeSkill = false );
	

	CX2Unit::UNIT_CLASS GetUnitClass() const { return m_eUnitClass; }
	void SetUnitClass(CX2Unit::UNIT_CLASS val) { m_eUnitClass = val; }
	CX2SkillTreeTool::SKILL_GRID* GetSkillGrid( int iX, int iY );
	CX2SkillTree::SkillTemplet* GetGridSkillTemplet( int iX, int iY );

	void ParseIntArrayFromEditBox( CDXUTEditBox* pEditBox, int *pNumbers, int nCount );
	void SetTextIntArrayToEditBox( CDXUTEditBox* pEditBox, const int *pNumbers, int nCount );

	D3DXVECTOR2 GetGridSize() const { return m_vGridSize; }
	CKTDGUIControl::CPictureData* GetPictureArrow( int iGridX, int iGridY, int iDir );
	void UpdateMainDialog();
	void MoveAllSkillGrid( D3DXVECTOR2 vDisplace );
	void UpdateAvailableSkillList();
	void CloseSkillInfoPopup();
	void OpenSkillInfoPopup( CX2SkillTree::SKILL_ID eSkillID, bool bAtMousePos = false );
	bool GetEnablePopupInfo() const { return m_bEnablePopupInfo; }
	void SetEnablePopupInfo(bool val) { m_bEnablePopupInfo = val; CloseSkillInfoPopup(); }
	
	void ResizeSkillTreeDialog( float fScale );
	void ReplaceString(std::string& text, const std::string& find_token, const std::string& replace_token);
	void ReplaceString(std::wstring& text, const std::wstring& find_token, const std::wstring& replace_token);
	void UpdateAllGridBySkillTemplet();
	void OpenTextureIconPiece( const WCHAR* wszFilePath );
	void UpdateTexturePieceList();
	void UpdatePickedGridBeforeSkillMark();
	bool ValidateSkillTree();


private:
	CKTDGUIDialogType		m_pDLGSkillTree;
	CKTDGUIDialogType		m_pDLGSkillList;
	CKTDGUIDialogType		m_pDLGSkillInfoPopup;


	CDXUTDialog			m_MainDialog;
	CDXUTDialog			m_PropertyDialog;
	
	map< std::pair<int,int>, SKILL_GRID >	m_mapSkillGrid;
	

	CX2SkillTree*		m_pSkillTree;
	
	
	D3DXVECTOR2			m_vGridSize;
	D3DXVECTOR2 		m_vButtonSize;
	D3DXVECTOR2 		m_vButtonGap;

	CX2Unit::UNIT_CLASS		m_eUnitClass;
	D3DXVECTOR2			m_vPickedGridIndex;

	bool				m_bEnablePopupInfo;


	CKTDXDeviceTexture*		m_pTexturePickedGrid;
	
	CKTDXDeviceTexture*		m_pTextureIconPiece;
	wstring					m_wstrNameTextureIcon;


	bool				m_bLeftControlPressed;

	
	
	
};



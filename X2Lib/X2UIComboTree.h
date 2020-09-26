#pragma once




#ifdef COMBO_TREE_UI_TEST

class CX2UIComboTree
{
public:
	enum COMBO_TREE_UI_CUSTOM_MSG
	{
		CTUCM_OPEN_SKILL_TREE		= 35300,
		CTUCM_CLOSE_COMBO_TREE,

		CTUCM_CATEGORY_NORMAL_COMBO,
		CTUCM_CATEGORY_JUMP_COMBO,
		CTUCM_CATEGORY_DASH_COMBO,
		CTUCM_CATEGORY_DASH_JUMP_COMBO,


		CTUCM_SLIDE_OPACITY,
		CTUCM_COMBO_ICON_MOUSE_OVER,
		CTUCM_COMBO_ICON_MOUSE_OUT,

//#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		CTUCM_CATEGORY_SPECIAL_COMBO,
//#endif //SERV_ADD_LUNATIC_PSYKER
	};

public:
	CX2UIComboTree();
	~CX2UIComboTree();

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//{{ uimanager클래스와 연동하기 위해 필요한 함수
	bool GetShow() { return m_bShow; }
	void SetShow( bool bShow );
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetPosition( D3DXVECTOR2 vPos );
	void SetLayer( X2_DIALOG_LAYER layer );
	void OnStateChange();
	//}} uimanager클래스와 연동하기 위해 필요한 함수



	void OpenComboTree( bool bOpen );
	void UpdateComboTree( CX2Unit::UNIT_CLASS eUnitClass, CX2ComboTree::COMBO_CATEGORY eComboCategory = CX2ComboTree::CC_INVALID );


	D3DXVECTOR2 CalcPicturePosition( CX2ComboTree::COMBO_TREE_ICON eIcon, const D3DXVECTOR2& vGridPosition );
	bool GetComboIconTextureName( std::wstring& textureName, std::wstring& pieceName, CX2ComboTree::COMBO_TREE_ICON eIcon );



	void UpdateOpacitySlider();
	void UpdateOpacitySlider( bool bEnable );








private:
#ifdef COMBO_TREE_TOOLTIP
	void						CreateMouseOverButton( const CX2ComboTree::ComboIcon& eIcon, int staticComboIndex_ );
	void						DrawComboTreeDesc( D3DXVECTOR2 vDrawPos_, wstring wstrDesc );
	int							m_iNumMouseOverButton;
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	vector<CKTDGUIButton*>		m_vecMouseOverButton[5];
#else //SERV_ADD_LUNATIC_PSYKER
	vector<CKTDGUIButton*>		m_vecMouseOverButton[4];
#endif //SERV_ADD_LUNATIC_PSYKER
	CKTDGUIDialogType			m_pDLGComboTreeDesc;
#endif COMBO_TREE_TOOLTIP

	CKTDGUIDialogHandle m_hDLGComboTree;
	CX2Unit::UNIT_CLASS m_eUnitClass;
	CX2ComboTree::COMBO_CATEGORY m_eComboCategory;
	bool m_bShow;
	D3DXVECTOR2 m_DLGSize;

	int m_iOpacity;
};

#endif COMBO_TREE_UI_TEST

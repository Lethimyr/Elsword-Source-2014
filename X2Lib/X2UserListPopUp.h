//{{ 김상훈 : [2010.10.13] 캐릭터 우클릭 팝업 메뉴 UI 개선
#ifdef CHARACTER_MOUSE_RBUTTON

#pragma once

using namespace std;

class CX2UserListPopUp : public CX2PopupUIBase
{
public:
	enum USERLIST_POPUP_MSG
	{
		UPM_PREV_PAGE =46000,
		UPM_NEXT_PAGE,
		UPM_QUIT,
		UPM_CLICK1,
		UPM_CLICK2,
		UPM_CLICK3,
		UPM_CLICK4,
	};
	
	struct UserListSlot
	{
		UidType m_UserUid;
		bool m_Enable;
		bool m_bIsPet;  //  1 이면 펫 ,  0 이면 유저
		CKTDGUIButton*					m_pButton;
		
		CKTDGUIDialogType				m_pLevel;

		CKTDGUIControl::CPictureData* m_pLevelOnePic[10];
		CKTDGUIControl::CPictureData* m_pLevelTenPic[10];
	};

	CX2UserListPopUp();
	~CX2UserListPopUp();

	void SetStage( CKTDXStage* pNowState  );
	bool SetPopupMenu();
	void AddList( UidType iUnitUid , bool isPet);
	int GetListSize();

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShowMenu() { return m_bShow; }	
	void OpenPopupMenu( bool bNotOpen );
	void ClosePopupMenu();

	void PrevPage();
	void NextPage();
protected:
	const CX2Unit* GetUnitByUserUid( const UidType uid_ );

protected:    
	CX2State*				m_pNowState;
	CKTDGUIDialogType			m_pDlgMenu;

private: 
	std::vector< std::pair<UidType, bool> > m_UserUidList;

	UserListSlot m_UserListSlot[4];

	int							 m_iNowPage;
	bool							m_bShow;
};

#endif CHARACTER_MOUSE_RBUTTON
//}} 김상훈 : [2010.10.13] 캐릭터 우클릭 팝업 메뉴 UI 개선

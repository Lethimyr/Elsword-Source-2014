#pragma once
/**	@file	: 2012/04/12
	@desc	: 팝업에서 사용하는 중복되는 기능을 하나의 클래스로 모음( 9등분 스킨, 공통되는 함수 등 )
	@author	: JHKang
*/

/** @class	: CX2PopupUIBase
	@desc	: 팝업에 사용되는 리소스 및 기능을 가진다.
	@date	: 2012/04/12
*/

#pragma region CX2PopupUIBase
class CX2PopupUIBase
{
#pragma region publice function
public:
	CX2PopupUIBase(void);
	virtual ~CX2PopupUIBase(void);

	bool GetShowMenu()
	{
		return m_bShow;
	}
	CKTDGUIDialogType GetDialog()
	{
		return m_pDlgPopup;
	}

	virtual void ClosePopupMenu();
	virtual void OpenUserPopupMenu( UidType iUnitUID, bool bPartyMember = false ) { };
	virtual void SetStage( CKTDXStage* pNowState  );

	void ChangeTopImage( IN bool bChange_ );
#pragma endregion

protected:
	bool	m_bShow;

	CKTDGUIDialogType	m_pDlgPopup;		/// 팝업 메뉴를 위한 Dialog( 9분할 )
	
	CKTDGUIControl::CPictureData*	m_pPicLeftTop;		/// 상단 왼쪽( 그라데이션 효과를 위한 2칸의 크기를 가짐 )
	CKTDGUIControl::CPictureData*	m_pPicCenterTop;	/// 상단 중앙
	CKTDGUIControl::CPictureData*	m_pPicRightTop;		/// 상단 오른쪽
	
	CKTDGUIControl::CPictureData*	m_pPicLeftMiddle;	/// 중단 왼쪽
	CKTDGUIControl::CPictureData*	m_pPicCenterMiddle;	/// 중단 중앙
	CKTDGUIControl::CPictureData*	m_pPicRightMiddle;	/// 중단 오른쪽
	
	CKTDGUIControl::CPictureData*	m_pPicLeftBottom;	/// 하단 왼쪽
	CKTDGUIControl::CPictureData*	m_pPicCenterBottom;	/// 하단 중앙
	CKTDGUIControl::CPictureData*	m_pPicRightBottom;	/// 하단 오른쪽

	CX2State*	m_pNowState;

	int		m_nMenuCount;
};
#pragma endregion


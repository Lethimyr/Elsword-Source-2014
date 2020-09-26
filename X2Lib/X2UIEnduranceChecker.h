// 2009.01.29 김태완 : 내구도 표시 UI.
// UICharInfo의 member로 존재. -> 09/03/23 수정 : MiniMapUI 아래로 옮겨왔다.
// SetShow(true)를 받았을 때 Update도 함. 내구도가 20% 아래로 떨어진 장비가 없으면 나타나지 않음.
// 현재 코드에 있는 진입점은 state 이동할 때 ( CX2UIManager::OnStateChange ) 와 장비 변경(수리/갈아입기 등)이 있을 때 ( CX2UICharInfo::ResetNowEquipUI )
// -> 맵 켜질 때 (CX2MiniMapUI::SetShowMiniMap)와 장비변경( CX2UICharInfo::ResetNowEquipUI )으로 교체

#pragma once

class CX2UIEnduranceChecker
{
public:

	CX2UIEnduranceChecker(void);
	virtual ~CX2UIEnduranceChecker(void);

	bool Update();
	
	void SetShow(bool val);
	void SetLayer( X2_DIALOG_LAYER layer );
	
	
private:
	bool					m_bShow;
	
	CKTDGUIDialogType			m_pDLGUIEnduranceChecker;   

};


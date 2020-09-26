#pragma once
#include "afxwin.h"


// CSelectToolUnit 대화 상자입니다.

class CSelectToolUnit : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectToolUnit)

public:
	CSelectToolUnit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSelectToolUnit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SELECT_UNIT };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void InitToolunit();
	void ChangeSkill();
	int GetSkillId(wstring wstrSkillName);
public:
	CX2Unit::UNIT_CLASS m_eUnitClass;
	wstring m_wstrUnitName;
	int m_iLevel;
	int m_iSkillId[4];
	int m_iAtkPhysic;
	int m_iAtkMagic;
	int m_iDefPhysic;
	int m_iDefMagic;
	UINT m_uHp;

	CComboBox m_comboUnitClass;
	afx_msg void OnCbnSelchangeComboUnitclass();
	CComboBox m_comboSkillA;
	CComboBox m_comboSkillS;
	CComboBox m_comboSkillD;
	CComboBox m_comboSkillC;
	CEdit m_editHp;
	CEdit m_editAtkPhysic;
	CEdit m_editAtkMagic;
	CEdit m_editDefPhysic;
	CEdit m_editDefMagic;
	afx_msg void OnCbnSelchangeComboSkilla();
	afx_msg void OnCbnSelchangeComboSkills();
	afx_msg void OnCbnSelchangeComboSkilld();
	afx_msg void OnCbnSelchangeComboSkillc();
	CEdit m_editLevel;
	CEdit m_editUnitName;
	afx_msg void OnEnChangeEditUnitname();
	afx_msg void OnEnChangeEditUnitlevel();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();
};

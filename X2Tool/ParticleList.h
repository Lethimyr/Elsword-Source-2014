#pragma once
#include "afxwin.h"


// ParticleList 대화 상자입니다.

class ParticleList : public CDialogEx
{
	DECLARE_DYNAMIC(ParticleList)

public:
	ParticleList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ParticleList();

	virtual void OnFinalRelease();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PARTICLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
// 	DECLARE_DISPATCH_MAP()
// 	DECLARE_INTERFACE_MAP()

public:
	CKTDGParticleSystem* GetParticleSystem();
	void UpdateParticleList(int iType);
	afx_msg BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	CComboBox m_comboParticleType;
	CListBox m_listParticle;

	int m_iSelCombo;
	wstring	m_wstrParticleName;
	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqPreview;

	afx_msg void OnCbnSelchangeComboParticleType();
	afx_msg void OnLbnSelchangeListParticle();
	afx_msg void OnBnClickedButton1();
};

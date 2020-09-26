#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ParticleList.h"
#include "MeshCategory.h"

// CEditWorld 대화 상자입니다.

class CEditWorld : public CDialogEx
{
	DECLARE_DYNAMIC(CEditWorld)

public:
	CEditWorld(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditWorld();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EDIT_WORLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	bool m_bResetFrustum;
	float m_fNear;
	float m_fFar;
	float m_fPerspective;

	float m_fBaseCamera;
	float m_fCamera1;
	float m_fCamera2;
	float m_fCamera3;

	bool m_bCheckList;

	ParticleList m_DlgParticleList;
	CMeshCategory m_DlgMeshList;

public:
	CListCtrl m_ctrlListWorldObject;

	void SetCurSel(int iIndex, bool bCheck);
	wstring GetWorldLayerString(int iLayer);
	void ResetWorldFrustum();
	void ResetWorldLayer(bool bReset = true);
	void ResetWorldSkyDome(bool bReset = true);
	void ResetWorldObject(bool bReset = true);
	void ResetWorldEditor();
	void OnDropFiles(HWND hWnd, HDROP hDropInfo);
	void AddParticle(int iType, wstring wstrName);
	void AddWorldObject(wstring wstrName, D3DXVECTOR3 vPos);

	CListCtrl m_ctrlListWorldLayer;
	CEdit m_editNear;
	CEdit m_editFar;
	CEdit m_editPerspective;
	CEdit m_editBaseCamera;
	CEdit m_editCamera1;
	CEdit m_editCamera2;
	CEdit m_editCamera3;
	CListCtrl m_ctrlSkyDomList;
	afx_msg void OnEnChangeEditWorldNear();
	afx_msg void OnEnChangeEditWorldFar();
	afx_msg void OnEnChangeEditWorldPerspective();
	afx_msg void OnEnChangeEditWorldBasecamera();
	afx_msg void OnEnChangeEditWorldCamera1();
	afx_msg void OnEnChangeEditWorldCamera2();
	afx_msg void OnEnChangeEditWorldCamera3();
	afx_msg void OnLvnItemchangedListEditWorldlayer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddWorldlayer();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMSetfocusListEditWorldlayer(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnBnClickedButtonAddSkydome();
	afx_msg void OnLvnItemchangedListEditSkydome(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusListEditSkydome(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusListWorldobject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListWorldobject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddWorldobject();
	afx_msg void OnBnClickedButtonAddParticle();	
	afx_msg void OnLvnOdstatechangedListWorldobject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton4();
};

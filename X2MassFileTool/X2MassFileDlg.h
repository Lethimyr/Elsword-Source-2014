// X2MassFileDlg.h : header file
//

#if !defined(AFX_X2MASSFILEDLG_H__34130357_45C0_4842_885A_283A5728849B__INCLUDED_)
#define AFX_X2MASSFILEDLG_H__34130357_45C0_4842_885A_283A5728849B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CX2MassFilerDlg dialog

class CX2MassFileDlg : public CDialog
{
	// Construction
public:
	CX2MassFileDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CX2MassFileDlg();

	enum OPEN_STAGE
	{
		OS_NONE = 1,
		OS_FILE,
		OS_KOM,
		OS_FOLDER,
	};

	// Dialog Data
	//{{AFX_DATA(CX2MassFilerDlg)
	enum { IDD = IDD_X2MASSFILE_DIALOG };
	CListCtrl	c_FileList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CX2MassFileDlg)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CMassFile*		m_pMassFile;
	CString			m_KomFilePath;
	std::string		m_strOutFilename;

	bool			m_bZip;

	void			AddListItem( UINT nIndex, const WCHAR* wstrFileName );
	// 외부에서 들어온 파일경로를 체크하여 kom/일반파일을 구별하여
	// 리스트박스에 추가
	OPEN_STAGE		CheckList( WCHAR* wstrFileName );
	CString			Decompress( CString& csComFile );
	void			FindFloderFile( WCHAR* wstrFloder, std::vector<std::wstring>& vecFloder );
	bool			LoadConfig();

	// Implementation
protected:
	CImageList		m_imglist;
	HACCEL			m_hAccel;

	// Generated message map functions
	//{{AFX_MSG(CX2MassFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnBegindragFilelist(NMHDR* pNMHDR, LRESULT* pResult);	// 리스트파일을 외부로 뺄때..
	afx_msg void OnRclickFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectAll();
	afx_msg void OnInvertSelection();
	afx_msg void OnRemoveFromList();
	afx_msg void OnClearList();
	afx_msg void OnDropFiles(HDROP hDropInfo);							// 외부파일을 리스트로 가져올때
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSave();
	afx_msg void OnBnClickedCheckZip();
	afx_msg void OnBnClickedButSave();
	afx_msg void OnBnClickedButRelease();
	afx_msg void OnRelease();
	afx_msg void OnRemoveKeyDel();
	afx_msg void OnBnClickedButFileopen();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_X2MASSFILEDLG_H__34130357_45C0_4842_885A_283A5728849B__INCLUDED_)

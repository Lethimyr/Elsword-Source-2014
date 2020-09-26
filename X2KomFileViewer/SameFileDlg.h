#pragma once
#include "afxcmn.h"


// CSameFileDlg 대화 상자입니다.

class CSameFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CSameFileDlg)

public:
	CSameFileDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSameFileDlg();
	virtual BOOL OnInitDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };
public:
	void SetSameFileList( std::map< std::wstring, std::vector< std::wstring > >& mapSameList ) { m_mapSameFileList = mapSameList; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_sameList;
	std::map< std::wstring, std::vector< std::wstring > > m_mapSameFileList;
	afx_msg void OnBnClickedOk();
};

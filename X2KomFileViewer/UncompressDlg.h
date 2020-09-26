#pragma once
#include "afxwin.h"


// CUncompressDlg 대화 상자입니다.

class CUncompressDlg : public CDialog
{
	DECLARE_DYNAMIC(CUncompressDlg)

public:
	CUncompressDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUncompressDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UNCOM_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	std::vector< std::wstring > m_vecSelectedFileList;		// 압축해제를 위해 선택된 파일 리스트
	std::map< std::wstring, std::wstring > m_mapKomList;
	UNCOMPRESS_MODE m_eMode;
	CListBox m_komList;
	afx_msg void OnBnClickedFile();		// 선택된 리소스파일만 압축해제
	afx_msg void OnBnClickedKom();		// 선택된 kom 파일만 압축해제
	afx_msg void OnBnClickedAllKom();	// 모든kom 파일 압축해제(kom 파일별 폴더 생성)
	afx_msg void OnBnClickedOneKom();	// 모든kom 파일 압축해제(폴더 하나에 모두 복사)

public:
	UNCOMPRESS_MODE GetCompressMode() { return m_eMode; }
	std::vector< std::wstring >& GetSeletedFileList() { return m_vecSelectedFileList; }
	void SetKomList( std::map< std::wstring, std::wstring >& mapKomList ) { m_mapKomList = mapKomList; }
};

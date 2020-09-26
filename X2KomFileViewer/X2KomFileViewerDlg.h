// X2KomFileViewerDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CX2KomFileViewerDlg 대화 상자
class CX2KomFileViewerDlg : public CDialog
{
// 생성
public:
	CX2KomFileViewerDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_X2KOMFILEVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	bool m_bIsKomParsingOnly;
	void InitListControl();
	void InsertKomList( IN const std::wstring& wstrKomName, IN const std::map< std::wstring, std::vector< MemberFileInfo > >& mapMemberFile );
	void InsertMemberList( IN const std::wstring& wstrKomName, IN const std::vector< MemberFileInfo >& vecMemberFileList );
	void InsertInsertedFileList( IN const std::vector< MemberFileInfo >& vecMemberFileList );
	void InsertDeletedFileList( IN std::vector< MemberFileInfo >& vecMemberFileList );
	bool BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder );

public:	
	CImageList m_imglist;
	CListCtrl m_memberList;
	CListCtrl m_insertedListCtrl;
	CListCtrl m_deletedListCtrl;
	CEdit m_memberCount;
	void SetKomParsingOnly( bool bIsKomParsingOnly ) { m_bIsKomParsingOnly = bIsKomParsingOnly;	}
	afx_msg void OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_komSizeEdit;
	afx_msg void OnBnClickedCreateKom();	// 압축하기
	CListCtrl m_komList;
	afx_msg void OnLvnItemActivateKomList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();		// 파일열기
	afx_msg void OnBnClickedFindFile();		// 파일찾기
	CEdit m_editFileFind;
	afx_msg void OnBnClickedUncompress();	// 압축풀기
	CButton m_checkMT;
	afx_msg void OnBnClickedModifyKom();	// 리소스 갈아끼우기
};

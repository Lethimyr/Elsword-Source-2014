#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CKomListViewDlg dialog

class CKomListViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CKomListViewDlg)

public:
	CKomListViewDlg(CWnd* pParent = NULL);   // standard constructor
	CKomListViewDlg( std::map< std::wstring, std::vector< MemberFileInfo > >& mapNewMemberFile,
		CWnd* pParent = NULL );
	virtual ~CKomListViewDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_KOM_LIST_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_lcKomList;
	LV_COLUMNW		m_COL;
	LV_ITEMW		m_LI;
	const std::map< std::wstring, std::vector< MemberFileInfo > >* m_pmapNewMemberFile;

	//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CEdit m_editFileCount;

public:
	int InitList();
	int SetKomList();
};

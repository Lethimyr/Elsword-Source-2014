#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"

// CX2OldKomFileViewer dialog

class CX2OldKomFileViewer : public CDialog
{
	DECLARE_DYNAMIC(CX2OldKomFileViewer)

public:
	CX2OldKomFileViewer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CX2OldKomFileViewer();

// Dialog Data
	enum { IDD = IDD_DIALOG_OLD_KOM_LIST_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CListCtrl m_lcKomList;
	LV_COLUMNW		m_COL;
	LV_ITEMW		m_LI;

public:
	int InitList();
	void SetOldKomList();
};

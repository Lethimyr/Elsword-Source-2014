#pragma once

#include "DungeonDataDlg.h"


// CDungeonInfo

class CDungeonInfo : public CDockablePane
{
	DECLARE_DYNAMIC(CDungeonInfo)

public:
	CDungeonInfo();
	virtual ~CDungeonInfo();

	//CWnd m_wndChild;
	CDungeonDataDlg m_DungeonDlg;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};



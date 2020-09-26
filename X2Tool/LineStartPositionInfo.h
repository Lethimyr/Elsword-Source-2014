#pragma once

#include "LineStartPosition.h"


// CDungeonInfo

class CLineStartPositionInfo : public CDockablePane
{
	DECLARE_DYNAMIC(CLineStartPositionInfo)

public:
	CLineStartPositionInfo();
	virtual ~CLineStartPositionInfo();

	//CWnd m_wndChild;
	CLineStartPosition m_LineStartPositionDlg;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};



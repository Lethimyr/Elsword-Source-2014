#pragma once

#include "LineCamera.h"


// CDungeonInfo

class CCameraInfo : public CDockablePane
{
	DECLARE_DYNAMIC(CCameraInfo)

public:
	CCameraInfo();
	virtual ~CCameraInfo();

	//CWnd m_wndChild;
	CLineCamera m_LineCameraDlg;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};



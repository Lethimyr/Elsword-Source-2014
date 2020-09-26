#pragma once


// CGraph 대화 상자입니다.
#include "Image.h"
#include "2DPushGraph.h"

class CGraph : public CDialog
{
	DECLARE_DYNAMIC(CGraph)

public:
	CGraph(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGraph();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:	
	virtual BOOL OnInitDialog();
	

	DECLARE_MESSAGE_MAP()
private:
	//CImage m_image;
	C2DPushGraph m_PushGraph;
public:
	afx_msg void OnPaint();	
	afx_msg void OnTimer(UINT nIDEvent);
};
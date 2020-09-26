#pragma once

class CServerElementButton: public CBitmapButton
{
	DECLARE_DYNAMIC(CServerElementButton)

public:
	CServerElementButton();

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
private:
};
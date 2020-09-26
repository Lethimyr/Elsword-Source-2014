#pragma once


// CTriggerEdit 대화 상자입니다.

class CTriggerEdit : public CDialog
{
	DECLARE_DYNAMIC(CTriggerEdit)

public:
	CTriggerEdit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTriggerEdit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TRIGGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};

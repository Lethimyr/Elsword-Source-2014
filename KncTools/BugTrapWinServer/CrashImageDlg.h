
#pragma once
#include "afxwin.h"


// CCrashImageDlg 대화 상자입니다.

class CCrashImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CCrashImageDlg)

public:
	CCrashImageDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCrashImageDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    CStatic m_pic;

    void OnPaint();
    char LoadImage(const char *parm_path);
    void Draw();

private:
    // 이미지 객체의 포인터를 저장할 변수
    LPPICTURE mp_image_object;
    // 읽어들인 이미지의 폭과 높이에 대한 크기 정보
    long m_width_size, m_height_size;

};

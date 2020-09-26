//
// UserProperty.h	:	유저 프로퍼티 클래스
//

#pragma once

class CUserSliderPopup;

//	유저 그리드 슬라이더
class CUserPropertyGridSlider : public CMFCPropertyGridProperty
{
public:

	//	생성자
	CUserPropertyGridSlider( CWnd* pWnd, const CString& strName, int& varValue, DWORD_PTR dwData );
	CUserPropertyGridSlider( CWnd* pWnd, const CString& strName, float& varValue, DWORD_PTR dwData );
	~CUserPropertyGridSlider();

	//	수정 중입니다.
	BOOL OnEdit(LPPOINT lptClick);

	//	수정이 종료 되었습니다.
	BOOL OnEndEdit();

	//	슬라이더 팝업 설정
	void SetPopup( CUserSliderPopup* pPopup );

	//	에디트 창이 활성화 되어 있는지의 여부
	BOOL IsEnableEdit();

	//	최소치와 최대치
	void SetRange( int nMin, int nMax );

	//	부동 소수점
	//	출력할 때 부동 소수점으로 기입합니다.
	//	SetRange에 기입된 것을 0.0~1.0으로 규정하고
	//	SetFloatValue에 기입된 값을 기입하여 최종 값을 결정합니다.
	//	뒤의 인자는 최종 값에 더할 값입니다.
	void SetFloatValue( float fValue, float fAdd = 0.0F );	
	void UpdateSlider();

protected:
	friend class CUserSliderPopup;
	
	//	슬라이더 팝업
	CUserSliderPopup*	m_pPopup;
	CWnd*				m_pGridCtrl;

	//	값들
	int					m_nMin;
	int					m_nMax;
	int					m_nValue;
	float				m_nFloatValue;

	float				m_fAddValue;
	float				m_fValue;
	float				m_fOffsetValue;
	
	//	부동 소수점으로 표현할지의 여부
	BOOL				m_bFloat;
}; //CUserPropertyGridSlider

//	프로퍼티 그리드
class CUserPropertyGrid	: public CMFCPropertyGridCtrl
{
public:
	DECLARE_DYNAMIC( CUserPropertyGrid )

	//	생성자와 해제자
	CUserPropertyGrid()
	{
	}

	~CUserPropertyGrid()
	{
	} 
	
	void SetLeftColumnWidht(int iWidth) { m_nLeftColumnWidth = iWidth; }
protected:

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
private:

}; //class CUserPropertyGrid


class CUserPropertyGridFile : public CMFCPropertyGridFileProperty
{
public:
	CUserPropertyGridFile( const CString& strName, BOOL bOpenFileDialog, const CString& strFileName, LPCTSTR lpszDefExt, DWORD dwFileFlags, LPCTSTR lpszFilter, LPCTSTR lpszDescr, DWORD_PTR dwData)
		: CMFCPropertyGridFileProperty(strName, m_bOpenFileDialog, strFileName, lpszDefExt, dwFileFlags, lpszFilter, lpszDescr, dwData)
	{
	}
	virtual ~CUserPropertyGridFile() {}

	virtual void OnClickButton(CPoint point);
};
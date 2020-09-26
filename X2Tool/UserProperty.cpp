
#include "stdafx.h"
#include "UserProperty.h"
#include "afxcontrolbarutil.h"
#include "afxshellmanager.h"

#define FOCUS_CHECK_EVENT_ID		0x850408
#define FOCUS_CHECK_TIME			100
#define USER_SLIDER_PROPERTY_ID		0x850409

// 슬라이더 팝업
class CUserSliderPopup	:	public CSliderCtrl
{
public:
	DECLARE_DYNAMIC( CUserSliderPopup )

	CUserSliderPopup();
	~CUserSliderPopup();

	//	값을 갱신합니다.
	void UpdateValue();

protected:
	//	메시지 펌프
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

	//	메시지 처리기
	BOOL PreTranslateMessage( MSG* pMsg );

	//	팝업창을 종료 합니다.
	void ClosePopup();

protected:
	friend class CUserPropertyGridSlider;

	//	프로퍼티
	CUserPropertyGridSlider*	m_pProperty;
}; //class CUserSliderPopup

IMPLEMENT_DYNAMIC( CUserSliderPopup, CSliderCtrl )

//{{AFX_MSG_MAP( CUserSliderPopup )
BEGIN_MESSAGE_MAP( CUserSliderPopup, CSliderCtrl )
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CUserSliderPopup::CUserSliderPopup()
:	m_pProperty		( NULL )
{
} //CUserSliderPopup::CUserSliderPopup

CUserSliderPopup::~CUserSliderPopup()
{
} //CUserSliderPopup::~CUserSliderPopup

//	메시지 처리기
BOOL CUserSliderPopup::PreTranslateMessage( MSG* pMsg )
{
	switch( pMsg->message )
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		//	마우스 버튼 클릭
		{
			CRect rect;
			GetClientRect(rect);

			//	마우스의 클릭 위치
			CPoint pt( AFX_GET_X_LPARAM( pMsg->lParam ), AFX_GET_Y_LPARAM( pMsg->lParam ) );

			if( !rect.PtInRect( pt ) )
			{	//	영역을 벗어 났습니다.
				ClosePopup();
				return TRUE;
			} //if
		} // ~마우스 버튼 클릭
		break;
	case WM_KILLFOCUS:
	case WM_SYSKEYDOWN:
	case WM_CONTEXTMENU:
		ClosePopup();
		return TRUE;
	case WM_TIMER:
		{
			KillTimer( FOCUS_CHECK_EVENT_ID );

			//	현재 활성화 된 윈도우
			CWnd* pFocusWnd = GetFocus();

			if( pFocusWnd && m_pProperty )
			{
				if( pFocusWnd != this && !m_pProperty->IsEnableEdit() )
				{	//	부모 윈도우와 현재가 포커스가 맞춰져 있지 않다면 종료 합니다.
					ClosePopup();
				} 
				else
				{	//	포커스가 있다면 특정 시간 마다 검사합니다.
					SetTimer( FOCUS_CHECK_EVENT_ID, FOCUS_CHECK_TIME, NULL );
				} //if..else..			
			} 
			else
			{	//	포커스가 없다면 종료 합니다.
				ClosePopup();
			} //if..else..

			return TRUE;
		}
		break;
	case WM_KEYDOWN:
		if( pMsg->wParam == VK_ESCAPE )
		{	//	강제 취소 처리를 합니다.
			ClosePopup();
			return TRUE;
		} //if
		break;
	}; //switch

	return CSliderCtrl::PreTranslateMessage( pMsg );
} //CUserSliderPopup::PreTranslateMessage

//	팝업창을 종료 합니다.
void CUserSliderPopup::ClosePopup()
{	//	종료 처리
	OnDestroy();

	//	자신을 지웁니다.
	//	이것으로 다른 데서 지우지 않아도 자동적으로 메모리가 해제 되도록 합니다.
	delete this;
} //CUserSliderPopup::ClosePopup

int CUserSliderPopup::OnCreate( LPCREATESTRUCT lpCreateStruct )
{	
	if( CSliderCtrl::OnCreate( lpCreateStruct ) == -1 )
	{	//	생성 실패
		return FALSE;
	} //if

	return TRUE;
} //CUserSliderPopup::OnCreate

void CUserSliderPopup::OnDestroy()
{
	if( m_pProperty )
	{
		ShowWindow( FALSE );
		m_pProperty->SetPopup( NULL );
		m_pProperty = NULL;
	} //if

	CSliderCtrl::OnDestroy();
} //CUserSliderPopup::OnDestroy

//	값을 갱신합니다.
void CUserSliderPopup::UpdateValue()
{
	if( NULL == m_pProperty )	return ;
	
	//	현재 값을 설정합니다.
	m_pProperty->m_nValue = GetPos();

	//	부동 소수점의 경우 특별 처리를 합니다.
	if( m_pProperty->m_bFloat )
	{		
		int nMin = m_pProperty->m_nMin;
		int nMax = m_pProperty->m_nMax;

		//	거리
		int nDist = nMax - nMin;

		//	오프셋값을 설정합니다.
		float fOffset = ( GetPos() - nMin ) / (float)nDist;

		//	최종 값을 설정합니다.
		m_pProperty->m_fValue = fOffset * m_pProperty->m_fOffsetValue;
		m_pProperty->m_fValue += m_pProperty->m_fAddValue;

		//	화면에 출력할 값을 설정합니다.
		m_pProperty->SetValue( (float)m_pProperty->m_fValue );
	} 
	else
	{
		m_pProperty->SetValue( (long)GetPos() );
	} //if

	//	그리드
	CMFCPropertyGridCtrl* pGrid = m_pProperty->m_pWndList;

	if( pGrid )
	{	//	값이 변경되었다고 통지합니다.
		pGrid->OnPropertyChanged( m_pProperty );
	} //if
} //CUserSliderPopup::UpdateValue

CUserPropertyGridSlider::CUserPropertyGridSlider( CWnd* pWnd, const CString& strName, int& varValue, DWORD_PTR dwData )
:	CMFCPropertyGridProperty( strName, (_variant_t)(long)varValue, NULL, dwData )
{
	m_pPopup = NULL;
	m_pGridCtrl = pWnd;

	//	값들
	m_nMin = 0;
	m_nMax = 100;
	m_nValue = varValue;
	m_fValue = 0.0F;
	m_nFloatValue = -1.f;
	m_fAddValue = 0.0F;
	m_fOffsetValue = 0.0F;
	m_bFloat = 0;
} //CUserPropertyGridSlider::CUserPropertyGridSlider
CUserPropertyGridSlider::CUserPropertyGridSlider( CWnd* pWnd, const CString& strName, float& varValue, DWORD_PTR dwData )
	:	CMFCPropertyGridProperty( strName, (_variant_t)(float)varValue, NULL, dwData )
{
	m_pPopup = NULL;
	m_pGridCtrl = pWnd;

	//	값들
	m_nMin = 0;
	m_nMax = 100;
	m_nValue = 0;
	m_fValue = 0.0F;
	m_nFloatValue = varValue;
	m_fAddValue = 0.0F;
	m_fOffsetValue = 0.0F;
	m_bFloat = 0;
		
} //CUserPropertyGridSlider::CUserPropertyGridSlider

CUserPropertyGridSlider::~CUserPropertyGridSlider()
{
	SAFE_DELETE( m_pPopup );
} //CUserPropertyGridSlider::~CUserPropertyGridSlider

//	값을 수정합니다.
BOOL CUserPropertyGridSlider::OnEdit( LPPOINT lptClick )
{
	if( NULL == m_pPopup )
	{	//	슬라이더 컨트롤을 추가합니다.
		m_pPopup = new CUserSliderPopup;
		m_pPopup->Create( TBS_BOTH | TBS_NOTICKS, m_Rect, m_pGridCtrl, USER_SLIDER_PROPERTY_ID );
		m_pPopup->m_pProperty = this;
	} //if

	//	프러퍼티의 속성을 설정합니다.
	m_pPopup->SetRange( m_nMin, m_nMax );
	m_pPopup->SetPos( m_nValue );

	//	왼쪽 컬럼의 넓이를 구합니다.
	int nAddX = m_pWndList->GetLeftColumnWidth();

	//	정확하게 Value창의 밑 부분에 출력 시키기 위해 넓이 값을 구합니다.
	int nParentWidth = m_pWndList->GetListRect().Width();

	//	프로퍼티그리드의 내부 넓이와 프로퍼티의 내부 넓이의 차가 발생할 수 있기에
	//	이것을 구합니다.
	int nPropertyWidth = nParentWidth - m_Rect.Width();

	//	Value창의 크기에 맞도록 조정합니다. 맨뒤의 높이는 적당한 값으로 설정합니다.
	m_pPopup->MoveWindow( m_Rect.left + nAddX - nPropertyWidth, m_Rect.top + m_Rect.Height(), m_Rect.Width() - nAddX + nPropertyWidth, 25 );
	m_pPopup->ShowWindow( TRUE );

	//	프로퍼티를 다시 그립니다.
	Redraw();

	return CMFCPropertyGridProperty::OnEdit( lptClick );
} //CUserPropertyGridSlider::OnEdit

BOOL CUserPropertyGridSlider::OnEndEdit()
{
	if( m_pPopup && ::GetFocus() != m_pPopup->GetSafeHwnd() )
	{
		m_pPopup->KillTimer( FOCUS_CHECK_EVENT_ID );
		m_pPopup->SetTimer( FOCUS_CHECK_EVENT_ID, 1, NULL );
	} //if

	return CMFCPropertyGridProperty::OnEndEdit();
} //CUserPropertyGridSlider::OnEndEdit

//	슬라이더 팝업 설정
void CUserPropertyGridSlider::SetPopup( CUserSliderPopup* pPopup )
{	
	m_pPopup = pPopup;
} //CUserPropertyGridSlider::SetPopup

//	에디트 창이 활성화 되어 있는지의 여부
BOOL CUserPropertyGridSlider::IsEnableEdit()
{
	if( m_pWndInPlace )	return TRUE;
	return FALSE;
} //CUserPropertyGridSlider::IsEnableEdit

//	최소치와 최대치
void CUserPropertyGridSlider::SetRange( int nMin, int nMax )
{
	m_nMin = nMin;
	m_nMax = nMax;
	//m_nValue = nMin;	
	m_fValue = 0.0F;
	m_bFloat = FALSE;
	
} //CUserPropertyGridSlider::SetRange

//	부동 소수점
//	출력할 때 부동 소수점으로 기입합니다.
//	SetRange에 기입된 것을 0.0~1.0으로 규정하고
//	SetFloatValue에 기입된 값을 기입하여 최종 값을 결정합니다.
//	뒤의 인자는 최종 값에 더할 값입니다.
void CUserPropertyGridSlider::SetFloatValue( float fValue, float fAdd )
{
	m_fOffsetValue = fValue;
	m_fAddValue = fAdd;
	m_fValue = fAdd;
	m_bFloat = TRUE;

	m_nValue = (m_nMax - m_nMin) * static_cast<int>( (m_nFloatValue - m_fAddValue) / m_fOffsetValue ) + m_nMin;
} //CUserPropertyGridSlider::SetFloatValue

void CUserPropertyGridSlider::UpdateSlider()
{
	if( m_nFloatValue < 0.f )
	{
		m_nValue = (int)GetValue().intVal;
	}
	else
	{
		float fTemp = (float)GetValue().fltVal;
		m_nValue = (m_nMax - m_nMin) * static_cast<int>( (fTemp - m_fAddValue) / m_fOffsetValue) + m_nMin;
	}
}

IMPLEMENT_DYNAMIC(CUserPropertyGrid, CMFCPropertyGridCtrl)

//{{AFX_MSG_MAP(CMFCPropertyGridCtrl)
BEGIN_MESSAGE_MAP(CUserPropertyGrid, CMFCPropertyGridCtrl)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CUserPropertyGrid::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{	//	리소스 아이디를 얻어 옵니다.
	int nID = pScrollBar->GetDlgCtrlID();

	if( nID == USER_SLIDER_PROPERTY_ID )
	{	//	유저 슬라이더입니다.
		CUserSliderPopup* pUserSlider = (CUserSliderPopup*)pScrollBar;
		pUserSlider->UpdateValue();
	} //if

	CMFCPropertyGridCtrl::OnHScroll( nSBCode, nPos, pScrollBar );
} //CUserPropertyGrid::OnHScroll


void CUserPropertyGridFile::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CString strPath = m_varValue.bstrVal;
	BOOL bUpdate = FALSE;

	
	CFileDialog dlg(m_bOpenFileDialog, m_strDefExt, strPath, m_dwFileOpenFlags, m_strFilter, m_pWndList);
	if (dlg.DoModal() == IDOK)
	{
		bUpdate = TRUE;
		strPath = dlg.GetPathName();
	}

	if (bUpdate)
	{
		CString cstrFileName = strPath;
		wstring wstrFileName = strPath;
		WCHAR wszFilePathOnly[512] = {NULL};
		WCHAR wszFileNameOnly[256] = {NULL};
		DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, wstrFileName.c_str() );

		if (m_pWndInPlace != NULL)
		{
			m_pWndInPlace->SetWindowText(wszFileNameOnly);
		}

		cstrFileName = wszFileNameOnly;
		m_varValue = (LPCTSTR) cstrFileName;
	}

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}
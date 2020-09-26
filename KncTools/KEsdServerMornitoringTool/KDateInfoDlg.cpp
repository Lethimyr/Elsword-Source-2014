// KDateInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "KDateInfoDlg.h"
#include ".\kdateinfodlg.h"


// KDateInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDateInfoDlg, CDialog)
KDateInfoDlg::KDateInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KDateInfoDlg::IDD, pParent)
{
}

KDateInfoDlg::KDateInfoDlg(int iGraphMaxValue, CWnd* pParent /*=NULL*/)
: CDialog(KDateInfoDlg::IDD, pParent)
{
	m_iGraphMaxValue = iGraphMaxValue;
}

KDateInfoDlg::~KDateInfoDlg()
{
}

void KDateInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_dateInfoList);
	DDX_Control(pDX, IDC_TAB1, m_dateInfoTab);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_monthCalCtrl);
	DDX_Control(pDX, IDC_LOAD_STAT, m_dateInfoBtn);
}

BOOL KDateInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RECT rect;
	GetWindowRect( &rect );
	MoveWindow( m_MainWindowPos.left + 3, m_MainWindowPos.top + 396, ( rect.right - rect.left ), ( rect.bottom - rect.top ) );

	// 리스트 박스 초기화
	InitListCtrl();

	// 그래프 초기화
	m_PushGraph.SetPeekRange(0, m_iGraphMaxValue );
	m_PushGraph.CreateFromStatic(IDC_STATICGRAPH2, this);
	m_PushGraph.ModifyStyle(0, WS_THICKFRAME);
	m_PushGraph.SetGridSize( 20 ); // 24칸 -> 1칸/20픽셀
	std::vector< int >& vecSelectedLines = m_PushGraph.GetSelectedLines();
	vecSelectedLines.push_back( TOTAL_CCU_ID );

	// 탭 초기화
	m_dateInfoTab.InsertItem( DIT_DAY, L"Day" );
	m_dateInfoTab.InsertItem( DIT_WEEK, L"Week" );

	// 시간 초기화
	SYSTEMTIME sysTime;
	::GetSystemTime( &sysTime );
	m_wstrSelectedDate = BOOST_FORMAT( ( L"%d-%d-%d" ) % sysTime.wYear % sysTime.wMonth % sysTime.wDay );
	m_today.wYear  = sysTime.wYear;
	m_today.wMonth = sysTime.wMonth;
	m_today.wDay   = sysTime.wDay;
	
	// 컨트롤 위치 저장
	RECT wRect, cRect;	
	this->GetWindowRect( &wRect );
	m_monthCalCtrl.GetWindowRect( &cRect );

	// -ㅅ-.. 이게 무슨 뻘짓이란 말인가..
	m_weekCalPos.left   = (wRect.right - wRect.left)/2 - (cRect.right - cRect.left)/2;
	m_weekCalPos.top    = (wRect.bottom - wRect.top)/2 - (cRect.bottom - cRect.top)/2 + 35;
	m_weekCalPos.right  = m_weekCalPos.left + cRect.right - cRect.left;
	m_weekCalPos.bottom = m_weekCalPos.top + cRect.bottom - cRect.top;

	m_dayCalPos.left = cRect.left - wRect.left;
	m_dayCalPos.top = cRect.top - wRect.top - 22;
	m_dayCalPos.right = m_dayCalPos.left + cRect.right - cRect.left;
	m_dayCalPos.bottom = m_dayCalPos.top + cRect.bottom - cRect.top;

	m_dateInfoBtn.GetWindowRect( &cRect );

	// -ㅅ-.. 이게 무슨 뻘짓이란 말인가..
	m_weekBtnPos.left   = (wRect.right - wRect.left)/2 - (cRect.right - cRect.left)/2;
	m_weekBtnPos.top    = (wRect.bottom - wRect.top)/2 - (cRect.bottom - cRect.top)/2 + 145;
	m_weekBtnPos.right  = m_weekBtnPos.left + cRect.right - cRect.left;
	m_weekBtnPos.bottom = m_weekBtnPos.top + cRect.bottom - cRect.top;

	m_dayBtnPos.left = cRect.left - wRect.left;
	m_dayBtnPos.top = cRect.top - wRect.top - 22;
	m_dayBtnPos.right = m_dayBtnPos.left + cRect.right - cRect.left;
	m_dayBtnPos.bottom = m_dayBtnPos.top + cRect.bottom - cRect.top;

	HWND hSlider = ::GetDlgItem( m_hWnd, IDC_ALPHA_SLIDER );
	::SendMessage(hSlider, TBM_SETPOS, 0, 100l);

	// 기본적으로 반투명처리
	::SetWindowLong( m_hWnd, GWL_EXSTYLE, GetWindowLong( m_hWnd, GWL_EXSTYLE ) | WS_EX_LAYERED );
	::SetLayeredWindowAttributes( m_hWnd, 0, 100, LWA_ALPHA );
	
	return TRUE;
}


BEGIN_MESSAGE_MAP(KDateInfoDlg, CDialog)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR1, OnMcnSelchangeMonthcalendar1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_LOAD_STAT, OnBnClickedOk)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ALPHA_SLIDER, OnNMCustomdrawAlphaSlider)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_ALPHA_SLIDER, OnNMCustomdrawAlphaSlider)
END_MESSAGE_MAP()

void KDateInfoDlg::InitListCtrl()
{
	m_dateInfoList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 38;
		kCol.pszText = L"날짜";

		m_dateInfoList.InsertColumn(0, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 38;
		kCol.pszText = L"최고동접";

		m_dateInfoList.InsertColumn(1, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 100;
		kCol.pszText = L"최고시간대";

		m_dateInfoList.InsertColumn(2, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 100;
		kCol.pszText = L"평균동접";

		m_dateInfoList.InsertColumn(3, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 70;
		kCol.pszText = L"주간평균";

		m_dateInfoList.InsertColumn(4, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 75;
		kCol.pszText = L"월간평균";

		m_dateInfoList.InsertColumn(5, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 70;
		kCol.pszText = L"가입유저";

		m_dateInfoList.InsertColumn(6, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 70;
		kCol.pszText = L"유닉유저";

		m_dateInfoList.InsertColumn(7, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 74;
		kCol.pszText = L"총 가입자수";

		m_dateInfoList.InsertColumn(8, &kCol);
	}

	AutoSizeListCtrl();
}

void KDateInfoDlg::InsertEsdListCtrl( KESDDateStatistics& kInfo )
{
	int iIdx = m_dateInfoList.GetItemCount();

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT;
		kItem.iItem = iIdx;
		kItem.iSubItem = 0;
		kItem.pszText = const_cast<wchar_t*>( kInfo.m_iDate.second.c_str() );

		m_dateInfoList.InsertItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = 1;
		kItem.pszText = const_cast<wchar_t*>( kInfo.m_iTopCCU.second.c_str() );

		m_dateInfoList.SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = 2;
		kItem.pszText = const_cast<wchar_t*>( kInfo.m_iTopCCUTime.second.c_str() );

		m_dateInfoList.SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT;
		kItem.iItem = iIdx;
		kItem.iSubItem = 3;
		kItem.pszText = const_cast<wchar_t*>( kInfo.m_iDayAvgCCU.second.c_str() );

		m_dateInfoList.SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = 4;
		kItem.pszText = const_cast<wchar_t*>( kInfo.m_iWeekAvgCCU.second.c_str() );

		m_dateInfoList.SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = 5;
		kItem.pszText = const_cast<wchar_t*>( kInfo.m_iMonthAvgCCU.second.c_str() );

		m_dateInfoList.SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = 6;
		kItem.pszText = const_cast<wchar_t*>( kInfo.m_iJoinUser.second.c_str() );

		m_dateInfoList.SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = 7;
		kItem.pszText = const_cast<wchar_t*>( kInfo.m_iUnicUser.second.c_str() );

		m_dateInfoList.SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = 8;
		kItem.pszText = const_cast<wchar_t*>( kInfo.m_iTotalUnicUser.second.c_str() );

		m_dateInfoList.SetItem( &kItem );
	}

	AutoSizeListCtrl();
}

void KDateInfoDlg::AutoSizeListCtrl()
{
	m_dateInfoList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_dateInfoList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_dateInfoList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	m_dateInfoList.SetColumnWidth ( 3, LVSCW_AUTOSIZE_USEHEADER );
	m_dateInfoList.SetColumnWidth ( 4, LVSCW_AUTOSIZE_USEHEADER );
	m_dateInfoList.SetColumnWidth ( 5, LVSCW_AUTOSIZE_USEHEADER );
	m_dateInfoList.SetColumnWidth ( 6, LVSCW_AUTOSIZE_USEHEADER );
	m_dateInfoList.SetColumnWidth ( 7, LVSCW_AUTOSIZE_USEHEADER );
	m_dateInfoList.SetColumnWidth ( 8, LVSCW_AUTOSIZE_USEHEADER );
}



void KDateInfoDlg::OnMcnSelchangeMonthcalendar1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);

	m_wstrSelectedDate.clear();
	m_wstrSelectedDate = boost::str( boost::wformat( L"%d-%d-%d" ) % pSelChange->stSelStart.wYear % pSelChange->stSelStart.wMonth % pSelChange->stSelStart.wDay );
	m_today.wYear  = pSelChange->stSelStart.wYear;
	m_today.wMonth = pSelChange->stSelStart.wMonth;
	m_today.wDay   = pSelChange->stSelStart.wDay;
	
	*pResult = 0;
}

void KDateInfoDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	HWND hWnd;

	switch( m_dateInfoTab.GetCurSel() ) 
	{
	case DIT_DAY:
		{
			hWnd = ::GetDlgItem( m_hWnd, IDC_STATICGRAPH2 );
			::ShowWindow( hWnd, SW_SHOW );
			hWnd = ::GetDlgItem( m_hWnd, IDC_STATIC_NUMBER );
			::ShowWindow( hWnd, SW_SHOW );

			m_monthCalCtrl.MoveWindow( &m_dayCalPos );
			m_dateInfoBtn.MoveWindow( &m_dayBtnPos );

			// 통계 정보 리스트 박스에 넣기
			m_dateInfoList.DeleteAllItems();
			if( m_dateStatistics.m_iDate.second.empty() == true )
				break;

			InsertEsdListCtrl( m_dateStatistics );
		}
		break;

	case DIT_WEEK:
		{
			hWnd = ::GetDlgItem( m_hWnd, IDC_STATICGRAPH2 );
			::ShowWindow( hWnd, SW_HIDE );
			hWnd = ::GetDlgItem( m_hWnd, IDC_STATIC_NUMBER );
			::ShowWindow( hWnd, SW_HIDE );
			
			m_monthCalCtrl.MoveWindow( &m_weekCalPos );
			m_dateInfoBtn.MoveWindow( &m_weekBtnPos );

			// 통계 정보 리스트 박스에 넣기
			m_dateInfoList.DeleteAllItems();
			std::vector< KESDDateStatistics >::iterator vit = m_vecWeekStatistics.begin();
			for( ; vit != m_vecWeekStatistics.end(); vit++ )
			{
				InsertEsdListCtrl( *vit );				
			}
		}
		break;

	default:
		break;
	}
	
	*pResult = 0;
}

void KDateInfoDlg::OnBnClickedOk()
{
	switch( m_dateInfoTab.GetCurSel() ) 
	{
	case DIT_DAY:
		{
			// 동일한 날짜를 요구할 경우 예외처리
			if( m_wstrSelectedDate == m_dateStatistics.m_iDate.second )
				break;

			// 선택된 날짜의 그래프 그리기
			DrawLineCCUGraph( m_wstrSelectedDate );

			// 선택된 날짜의 통계 정보 얻기
			theApp.m_kImp.GetDateStatisticsFromDB( m_wstrSelectedDate, 2, m_dateStatistics );

			// 통계 정보 리스트 박스에 넣기
			m_dateInfoList.DeleteAllItems();
			InsertEsdListCtrl( m_dateStatistics );
		}
		break;

	case DIT_WEEK:
		{
			// 동일한 날짜를 요구할 경우 예외처리
			if( m_wstrSelectedDate == m_wstrLastGetDate )
				break;
			
			// 선택된 날짜로 부터 일주일동안의 정보 얻고 리스트 박스에 넣기
			m_vecWeekStatistics.clear();
			m_dateInfoList.DeleteAllItems();
			KESDDateStatistics dateStatistics;
			for( int iDayCount = 1; iDayCount <= 7; iDayCount++ )
			{
				std::wstring wstrDate = BOOST_FORMAT( ( L"%d-%d-%d" ) % m_today.wYear % m_today.wMonth % m_today.wDay );
				theApp.m_kImp.GetDateStatisticsFromDB( wstrDate, 2, dateStatistics );
				m_vecWeekStatistics.push_back( dateStatistics );
				InsertEsdListCtrl( dateStatistics );

				// 전날 날짜 얻기
				BackToYesterday( m_today );
			}

			// 마지막으로 얻은 날짜 기억
			m_wstrLastGetDate = m_wstrSelectedDate;
		}
		break;

	default:
		break;
	}	
}

void KDateInfoDlg::DrawLineCCUGraph( std::wstring& wstrDate )
{
	// 동접자수 로그 데이터 얻기
	int iCCU;
	
#ifdef COMPILE_FOR_MAIN_SERVER
	int iCCUID = TOTAL_CCU_ID;
#else
	int iCCUID = 2;
#endif

	if( theApp.m_kImp.GetDateConcurrentUserFromDB( iCCUID, wstrDate, 2/* DSN은 따로 설정하자! */, m_vecDateCCU, iCCU ) == false )
	{
		MessageBox( L"동접자 수 로그 데이터 얻기 실패!" );
		return;
	}

	// 그래프 초기화
	m_PushGraph.RemoveLine( TOTAL_CCU_ID );

	// 라인 설정
	m_PushGraph.AddLine( TOTAL_CCU_ID, theApp.m_kImp.GetLineColor( theApp.m_kImp.GetTotalCCUTodayLineColor() ) );

	int average = 0, aveCnt = 0;
	std::vector< CCUInfo >::iterator vit = m_vecDateCCU.begin();
	for( ; vit != m_vecDateCCU.end(); vit++ )
	{
		average += vit->m_CCUcount;

		if( aveCnt < 2 )
		{
			aveCnt++;
		}
		else
		{
			average /= 3;
			m_PushGraph.Push( average, TOTAL_CCU_ID );
			average = 0;
			aveCnt = 0;
		}
	}
	
	m_PushGraph.Update();
}

void KDateInfoDlg::BackToYesterday( SYSTEMTIME& sysTime )
{
	boost::gregorian::date nowDay( sysTime.wYear, sysTime.wMonth, sysTime.wDay );

	boost::gregorian::date yesterday = nowDay - boost::gregorian::days( 1 );

	sysTime.wYear  = yesterday.year();
	sysTime.wMonth = yesterday.month();
	sysTime.wDay   = yesterday.day();
}

//void KDateInfoDlg::OnNMCustomdrawAlphaSlider(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//
//	*pResult = 0;
//}

void KDateInfoDlg::OnNMCustomdrawAlphaSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HWND hSlider = ::GetDlgItem( m_hWnd, IDC_ALPHA_SLIDER );

	int iSlideIndex = static_cast<int>(::SendMessage(hSlider, TBM_GETPOS, 0, 0l));

	int iAlphaValue = static_cast<int>( 255 * ( (float)iSlideIndex * (float)0.01f ) );

	if( iAlphaValue < 100 )
		iAlphaValue = 100;

	::SetLayeredWindowAttributes( m_hWnd, 0, iAlphaValue, LWA_ALPHA );
	//////////////////////////////////////////////////////////////////////////

	*pResult = 0;
}

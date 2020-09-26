// Image.cpp: implementation of the CImage class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Image.h"

#define _abs(x) ((x)>0?(x):-(x))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImage::CImage()
{
	m_pstInfo = NULL;
	int nMax = 80;

	for(int i=0 ; i<MAX_TABLE ; ++i)
	{
		m_pMaxTable[i] = nMax;
		nMax += 80;
	}
}

CImage::~CImage()
{

}

//////////////////////////////////////////////////////////////////////
// Note : 초기화
//////////////////////////////////////////////////////////////////////
void CImage::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

//////////////////////////////////////////////////////////////////////
// Note : 메모리 DC에 이미지를 생성한다 - 하루 동접 변화량의 차트 함수.
//////////////////////////////////////////////////////////////////////
void CImage::CreateImage_ForDay( HDC hdc, const wchar_t* pToday, const int* pData, int nMaxData)
{
	int nYMax;				//챠트의 Y축 최대값
	int nX;					//임시 현재 X좌표
	int nY;					//임시 현재 Y좌표
	int nPlus;				//임시 증감치
	int i;					//for루프용
	int nDiff;				//Y축 간격
	int nYCoord;			//임시 현재 Y축 값
	wchar_t strYCoord[20];	//임시

	HDC /*hdc, */hMemDC;
	HBITMAP hOldBitmap;
	HFONT hFont, hOldFont;
	HPEN hPen, hOldPen;

	//hdc = GetDC(m_hWnd);										//화면DC를 얻는다
	m_hBitmap = CreateCompatibleBitmap(hdc, 550, 400);		//비트맵생성
	hMemDC = CreateCompatibleDC(hdc);						//메모리DC를 얻는다
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);	//메모리DC에 비트맵을 올린다

	//챠트 테두리
	Rectangle(hMemDC, 0, 0, 550, 400);

	//챠트 몸체
	SelectObject(hMemDC, GetStockObject(GRAY_BRUSH));
	Rectangle(hMemDC, 70, 70, 520, 330);
	
	nY = 70;
	for(i=0 ; i<7 ; ++i)
	{
		nY += 32;
		MoveToEx(hMemDC, 70, nY, NULL);
		LineTo(hMemDC, 520, nY);
	}

	//폰트설정
	hFont = CreateFont(14, 0, 900, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		VARIABLE_PITCH | FF_SWISS, L"그래픽");

	hOldFont = (HFONT)SelectObject(hMemDC, hFont);

	////////////////////
	//90도 회전텍스트
	////////////////////
	//동접수
	TextOut(hMemDC, 10, 200, L"동접수", 6);

	//챠트의 X축 기준값 (2시간 간격)
	SetTextAlign(hMemDC, TA_RIGHT);
	nX = 65;
	nY = 335;
	nPlus = 37;

	for(i=0 ; i<=24 ; i+=2)
	{
		WCHAR strTime[20];
		wsprintf(strTime, L"%d:00", i);
		TextOut(hMemDC, nX, nY, strTime, static_cast<int>(wcslen(strTime)));
		nX += nPlus;
	}

	SetTextAlign(hMemDC, TA_LEFT);

	////////////////////
	//회전없는 텍스트
	////////////////////
	//분
	SelectObject(hMemDC, hOldFont);
	TextOut(hMemDC, 300, 380, L"min", 3);
	
	//날짜
	TextOut(hMemDC, 250, 10, pToday, static_cast<int>(wcslen(pToday)));

	//챠트의 Y축 기준값 (임시)
	SetTextAlign(hMemDC, TA_RIGHT);

	//테이블로부터 nYMax값을 얻어온다
	if(nMaxData > m_pMaxTable[MAX_TABLE-1])
		nYMax = m_pMaxTable[MAX_TABLE-1];
	else{
		for(i=0 ; i<MAX_TABLE ; ++i)
		{
			if(m_pMaxTable[i] >= nMaxData)
			{
				nYMax = m_pMaxTable[i];
				break;
			}
		}
	}

	//챠트의 Y축 기준값
	nDiff = nYMax / 8;
	nYCoord = nYMax;

	nPlus = 32;		//텍스트 좌표 증가치
	nX = 65;		//텍스트 x좌표 시작픽셀
	nY = 60;		//텍스트 y좌표 시작픽셀

	for(i=0 ; i<9 ; ++i)
	{
		wsprintf(strYCoord, L"%d", nYCoord);
		TextOut(hMemDC, nX, nY, strYCoord, static_cast<int>(wcslen(strYCoord)));

		nY += nPlus;
		nYCoord -= nDiff;
	}
	SetTextAlign(hMemDC, TA_LEFT);

	//DB로부터 얻어온 데이터를 그래프로 나타낸다.
	//원점을 그래프의 시작점으로
	SetMapMode(hMemDC, MM_ANISOTROPIC);
	SetWindowExtEx(hMemDC, 1440, nYMax, NULL);	//좌표범위
	SetViewportExtEx(hMemDC, 450, 260, NULL);	//화면상의 적용영역
	SetViewportOrgEx(hMemDC, 70, 329, NULL);	//원점 설정
	
	hPen = CreatePen(PS_SOLID, 1, 0x88ff55);
	hOldPen = (HPEN)SelectObject(hMemDC, hPen);

	MoveToEx(hMemDC, 0, -pData[0], NULL);
	for(i=0 ; i<1440 ; ++i)
	{
		//Y좌표 증가치가 -방향
		LineTo(hMemDC, i, -pData[i]);
	}

	BitBlt( hdc, 10, 10, 550, 500, hMemDC, -120, -200, SRCCOPY );

	//챠트 생성완료
	//메모리 해제
	SelectObject(hMemDC, hOldPen);
	SelectObject(hMemDC, hOldBitmap);

	DeleteObject(hPen);
	DeleteObject(hFont);
	DeleteDC(hMemDC);										//메모리DC 삭제
	//ReleaseDC(m_hWnd, hdc);									//화면DC 릴리즈
}

//////////////////////////////////////////////////////////////////////
// Note : 메모리 DC에 이미지를 생성한다 - 한달 동접 변화량의 차트 함수.
//////////////////////////////////////////////////////////////////////

void CImage::CreateImage_ForMonth(const wchar_t* pToday, wchar_t* _pTerm[30], const int* pData, int nMaxData, int iDiff)
{
	int nYMax;				//챠트의 Y축 최대값
	int nX;					//임시 현재 X좌표
	int nY;					//임시 현재 Y좌표
	int nPlus;				//임시 증감치
	int i;					//for루프용
	int nDiff;				//Y축 간격
	int nYCoord;			//임시 현재 Y축 값
	wchar_t strYCoord[20];		//임시

	HDC hdc, hMemDC;
	HBITMAP hOldBitmap;
	HFONT hFont, hOldFont;
	HPEN hPen1,hPen2, hOldPen;

	hdc = GetDC(m_hWnd);									//화면DC를 얻는다
	m_hBitmap = CreateCompatibleBitmap(hdc, 550, 405);		//비트맵생성
	hMemDC = CreateCompatibleDC(hdc);						//메모리DC를 얻는다
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);	//메모리DC에 비트맵을 올린다

	//챠트 테두리
	Rectangle(hMemDC, 0, 0, 550, 405);

	//챠트 몸체
	SelectObject(hMemDC, GetStockObject(GRAY_BRUSH));
	Rectangle(hMemDC, 70, 70, 520, 330);

	nY = 70;
	for(i=0 ; i<7 ; ++i) //세로 축을 7칸으로 분할한다.
	{
		nY += 32;
		MoveToEx(hMemDC, 70, nY, NULL);
		LineTo(hMemDC, 520, nY);
	}

	//폰트설정
	hFont = CreateFont(14, 0, 900, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		VARIABLE_PITCH | FF_SWISS, L"그래픽");

	hOldFont = (HFONT)SelectObject(hMemDC, hFont);

	////////////////////
	//90도 회전텍스트
	////////////////////
	//동접수
	TextOut(hMemDC, 10, 200, L"최대 동접", 12);

	//챠트의 X축 기준값
	SetTextAlign(hMemDC, TA_RIGHT);
	nX = 65;
	nY = 335;
	nPlus = 15;

	//for( i = 30 ; i > 0 ; --i)
	for( i = 0 ; i < 30 ; i++ )
	{
		//SYSTEMTIME	st; //시스템 타임으로 우선 현재 로컬 시간을 구한다.
		//FILETIME	ft; //파일 타임으로 바꾸어서 계산 편하게 하기.
		//GetLocalTime(&st);
		//st.wHour = st.wMinute = st.wSecond = st.wMilliseconds = st.wDayOfWeek = 0; //하루단위로 만든다.
		//st.wDay += iDiff;
		//SystemTimeToFileTime(&st,&ft);

		//INT64 i64; //64비트 정수형으로 하나의 숫자로 표현한다.
		//i64 = ((INT64)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
		//i64 -= 864000000000*(i);

		//ft.dwHighDateTime = (DWORD)(i64 >> 32);
		//ft.dwLowDateTime  = (DWORD)(i64 & 0xffffffff);
		//FileTimeToSystemTime( &ft , &st );

		//char strDate[MAX_PATH];
		//sprintf( strDate, "%d%2d%2d", st.wYear, st.wMonth, st.wDay);
		//TextOut(hMemDC, nX, nY, strDate, strlen(strDate));
		//nX += nPlus;

		TextOut( hMemDC, nX, nY, _pTerm[i], 10 );
		nX += nPlus;
	}

	SetTextAlign(hMemDC, TA_LEFT);

	////////////////////
	//회전없는 텍스트
	////////////////////
	SelectObject(hMemDC, hOldFont);

	//날짜
	TextOut(hMemDC, 250, 10, pToday, static_cast<int>(wcslen(pToday)));

	//챠트의 Y축 기준값 (임시)
	SetTextAlign(hMemDC, TA_RIGHT);

	//테이블로부터 nYMax값을 얻어온다
	if(nMaxData > m_pMaxTable[MAX_TABLE-1])
		nYMax = m_pMaxTable[MAX_TABLE-1];
	else
	{
		for(i=0 ; i<MAX_TABLE ; ++i)
		{
			if(m_pMaxTable[i] >= nMaxData)
			{
				nYMax = m_pMaxTable[i];
				break;
			}
		}
	}

	//챠트의 Y축 기준값
	nDiff = nYMax / 8;
	nYCoord = nYMax;

	nPlus = 32;		//텍스트 좌표 증가치
	nX = 65;		//텍스트 x좌표 시작픽셀
	nY = 60;		//텍스트 y좌표 시작픽셀

	for(i=0 ; i<9 ; ++i)
	{
		wsprintf(strYCoord, L"%d", nYCoord);
		TextOut(hMemDC, nX, nY, strYCoord, static_cast<int>(wcslen(strYCoord)));

		nY += nPlus;
		nYCoord -= nDiff;
	}
	SetTextAlign(hMemDC, TA_LEFT);

	//DB로부터 얻어온 데이터를 그래프로 나타낸다.
	//원점을 그래프의 시작점으로
	SetMapMode(hMemDC, MM_TEXT);
	SetViewportOrgEx(hMemDC, 70, 70, NULL);	//원점 설정

	hPen1 = CreatePen(PS_SOLID, 2, 0x88ff55);
	hPen2 = CreatePen(PS_SOLID, 2, RGB(255,0,0));//평균 동접 그릴 펜이다.
	hOldPen = (HPEN)SelectObject(hMemDC, hPen1);

	int iTop = 0;
	int iHeight = 260;

	MoveToEx(hMemDC, 0, iHeight - (int)(pData[0]*iHeight/nYMax) , NULL);
	for( i = 0 ; i < 30 ; i++ )
	{		
		//Y좌표 증가치가 -방향
		LineTo(hMemDC, 15*i, iHeight -(int)(pData[i]*iHeight/nYMax)  );
	}

	hOldPen = (HPEN)SelectObject(hMemDC, hPen2);

	int nSum = 0;
	for ( i=0 ; i < 30 ; i++ )
	{
		nSum += pData[i];
	}
	int nAverage = nSum / 30;
	MoveToEx(hMemDC,0,iHeight -(int)(nAverage*iHeight/nYMax),NULL);
	LineTo(hMemDC , 435,iHeight -(int)(nAverage*iHeight/nYMax));

	//챠트 생성완료
	//메모리 해제
	SelectObject(hMemDC, hOldPen);
	SelectObject(hMemDC, hOldBitmap);

	DeleteObject(hPen1);
	DeleteObject(hPen2);
	DeleteObject(hFont);
	DeleteDC(hMemDC);										//메모리DC 삭제
	ReleaseDC(m_hWnd, hdc);									//화면DC 릴리즈
}

//////////////////////////////////////////////////////////////////////
// Note : 메모리 DC에 이미지를 생성한다 - Mode Play Count의 차트 함수.
//////////////////////////////////////////////////////////////////////
void CImage::CreateImage_ModeCount(const wchar_t * _pToday, wchar_t * _pppTable[128][2], int _iNumberOfData, int _iMaxY, int _total)
{
	const int ZERO		= 0;
	int iImageWidth		= 550;									// 생성될 그림의 폭
	int iImageHeight	= 500;									// 생성될 그림의 높이
	int iChartBegin_X	= 75;									// 그려질 차트의 시작점 X좌표
	int iChartEnd_X		= 525;									// 그려질 차트의 끝점 X좌표
	int iChartBegin_Y	= 50;									// 그려질 차트의 시작점 Y좌표
	int iChartEnd_Y		= 350;									// 그려질 차트의 끝점 Y좌표
	int iChartWidth		= ( iChartEnd_X - iChartBegin_X );		// 그려질 차트의 폭
	int iChartHeight	= ( iChartEnd_Y - iChartBegin_Y );		// 그려질 차트의 높이
	int iChartMaxNum	= ( (_iMaxY/100)*100+100 );				// 차트가 최대 얼마까지 표시할 것인가 나타냄
	int iSpace_X = 0;											// X축 고정된 값 계산에 쓰여질 변수공간
	int	iSpace_Y = 0;											// Y축 고정된 값 계산에 쓰여질 변수공간
	int iTmp_X = 0;												// X축 변하는 값 계산에 쓰여질 변수공간
	int iTmp_Y = 0;												// Y축 변하는 값 계산에 쓰여질 변수공간
	wchar_t strTmp[300];											// 임시 문자열 저장 공간

	HDC hdc, hMemDC;
	HBITMAP hOldBitmap;
	HFONT hFont, hOldFont;
	HPEN hPen, hOldPen;

	hdc = GetDC(m_hWnd);													//화면DC를 얻는다
	m_hBitmap = CreateCompatibleBitmap(hdc, iImageWidth, iImageHeight);		//비트맵생성 : 550 X 500
	hMemDC = CreateCompatibleDC(hdc);										//메모리DC를 얻는다
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);					//메모리DC에 비트맵을 올린다

	// 챠트 테두리 : 550 X 500
	Rectangle(hMemDC, ZERO, ZERO, iImageWidth, iImageHeight);

	// 챠트 몸체	: 450 X 300
	SelectObject(hMemDC, GetStockObject(LTGRAY_BRUSH));
	Rectangle(hMemDC, iChartBegin_X, iChartBegin_Y, iChartEnd_X, iChartEnd_Y);
	Rectangle(hMemDC, (iChartBegin_X+1), (iChartBegin_Y+1), (iChartEnd_X-1), (iChartEnd_Y-1));

	// 차트에 눈금 기준선 긋기 : 9줄 -> 10칸
	iSpace_Y = (iChartHeight/10);
	iTmp_Y	 = iChartBegin_Y;
	for( int i = 0 ; i < 9 ; i++ )
	{
		iTmp_Y += iSpace_Y;

		MoveToEx(hMemDC, iChartBegin_X, iTmp_Y, NULL);
		LineTo(hMemDC, iChartEnd_X, iTmp_Y);
	}

	//폰트설정
	hFont = CreateFont(12, 0, 900, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					   DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, L"그래픽");
	hOldFont = (HFONT)SelectObject(hMemDC, hFont);

	////////////////////
	//90도 회전텍스트
	////////////////////
	// Mode Count 출력
	TextOut(hMemDC, 8, 200, L"Mode Count", 10);

	// 챠트의 X축 기준값 출력
	if( _iNumberOfData != 0 )
	{
		SetTextAlign(hMemDC, TA_RIGHT);
		iTmp_X	 = iChartBegin_X + 5;
		iTmp_Y	 = iChartEnd_Y + 3;
		iSpace_X = (iChartWidth/_iNumberOfData);
		int iVal = 0;
		double dVal = 0.0;
		char strVal[10]="";
		for( int i = 0 ; i < _iNumberOfData ; i++ )
		{
			// Mode Name 출력하는 부분
			iVal = _wtoi( _pppTable[i][1] );
			dVal = (double)iVal;
			wsprintf( strTmp, L"%s (%2.2f)", (_pppTable[i][0]), ((dVal/_total)*100) );
			TextOut(hMemDC, iTmp_X, iTmp_Y, strTmp, static_cast<int>(wcslen(strTmp)));

			iTmp_X += iSpace_X;
		}
	}
	SetTextAlign(hMemDC, TA_LEFT);

	////////////////////
	//회전없는 텍스트
	////////////////////
	SelectObject(hMemDC, hOldFont);

	// 오늘 날짜 출력
	TextOut(hMemDC, 280, 9, _pToday, static_cast<int>(wcslen(_pToday)));

	// Total Mode Play Count 출력
	wsprintf( strTmp, L"Total Mode played Count : %d", _total );
	TextOut(hMemDC, 200, 25, strTmp, 32);


	//챠트의 Y축 기준값
	SetTextAlign(hMemDC, TA_RIGHT);
	iTmp_X	 = iChartBegin_X - 5;
	iTmp_Y	 = iChartBegin_Y - 3;
	iSpace_Y = (iChartHeight/10);
	int iTmpMaxNum = iChartMaxNum ;
	for( int i = 0 ; i < 11 ; i++ )
	{
		wsprintf( strTmp, L"%d", iTmpMaxNum );
		TextOut(hMemDC, iTmp_X, iTmp_Y, strTmp, static_cast<int>(wcslen(strTmp)));

		iTmp_Y += iSpace_Y;					// 어디에 써 줄 것인가
		iTmpMaxNum -= (iChartMaxNum/10);	// 무엇을 써 줄 것인가
	}

	//////////////////////////////////
	//표 다 그렸으면 막대 그래프 출력
	//////////////////////////////////
	hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(0,128,255));
	hOldPen = (HPEN)SelectObject(hMemDC, hPen);

	int iTickness = 9;			// 막대기의 굵기
	int ihalfTickness = (iTickness/2);

	if( _iNumberOfData != 0 )
	{
		iTmp_X = ( iChartBegin_X + (11 - ihalfTickness) );
		iSpace_X = (iChartWidth/_iNumberOfData);
		double dTmp = 0.0;

		for( int i = 0 ; i < _iNumberOfData ; i++ )
		{
			// 막대기의 길이를 구해서
			wsprintf( strTmp, (_pppTable[i][1]) );
			dTmp = _wtoi( strTmp );
			iSpace_Y = (int)( (dTmp/iChartMaxNum)*iChartHeight - 4 );

			if( iSpace_Y < 1 )
			{	continue;	}

			// 선을 그린다.
			for( int j = 0 ; j < iTickness ; j++ )
			{
				iTmp_Y = ( iChartEnd_Y-3 );
				MoveToEx(hMemDC, (iTmp_X+j), iTmp_Y, NULL);
				iTmp_Y -= iSpace_Y;
				LineTo(hMemDC, (iTmp_X+j), iTmp_Y );
			}

			iTmp_X += iSpace_X;
		}
	}

	//챠트 생성완료
	//메모리 해제

	SelectObject(hMemDC, hOldPen);
	SelectObject(hMemDC, hOldBitmap);

	DeleteObject(hPen);
	DeleteObject(hFont);
	DeleteDC(hMemDC);										//메모리DC 삭제
	ReleaseDC(m_hWnd, hdc);									//화면DC 릴리즈
}

//////////////////////////////////////////////////////////////////////
// Note : 메모리 DC에 이미지를 생성한다 - Frame Rate의 이미지 챠트
//////////////////////////////////////////////////////////////////////
void CImage::createImage_FrameRate(const wchar_t* _pToday, wchar_t* _pTerm[30], double _dDataTable[6][30], int _iMaxData, int _iMinData, int _Index)
{
	const int ZERO		= 0;
	int iImageWidth		= 600;									// 생성될 그림의 폭
	int iImageHeight	= 530;									// 생성될 그림의 높이
	int iChartBegin_X	= 45;									// 그려질 차트의 시작점 X좌표
	int iChartEnd_X		= 500;									// 그려질 차트의 끝점 X좌표
	int iChartBegin_Y	= 40;									// 그려질 차트의 시작점 Y좌표
	int iChartEnd_Y		= 440;									// 그려질 차트의 끝점 Y좌표
	int iChartWidth		= ( iChartEnd_X - iChartBegin_X );		// 그려질 차트의 폭
	int iChartHeight	= ( iChartEnd_Y - iChartBegin_Y );		// 그려질 차트의 높이
	int iSpace_X = 0;											// X축 고정된 값 계산에 쓰여질 변수공간
	int	iSpace_Y = 0;											// Y축 고정된 값 계산에 쓰여질 변수공간
	int iTmp_X = 0;												// X축 변하는 값 계산에 쓰여질 변수공간
	int iTmp_Y = 0;												// Y축 변하는 값 계산에 쓰여질 변수공간
	double dTmp = 0.0;											// double값 잠깐잠깐 쓸 일 있을 때 쓰는 변수
	wchar_t strTmp[300];											// 임시 문자열 저장 공간
	int iRGB_Set[6][3] = {{0,255,0},{255,128,0},{0,255,255},{255,0,255},{0,64,128},{128,0,0}};	// 그래프 그릴 때 색깔 뭐 쓸지
	// 앞에서 부터 : 녹색, 주황색, 하늘색,  핑크색, 군청색, 갈색

	HDC hdc, hMemDC;
	HBITMAP hOldBitmap;
	HFONT hFont, hOldFont;
	HPEN hPen, hOldPen;

	hdc = GetDC(m_hWnd);													//화면DC를 얻는다
	m_hBitmap = CreateCompatibleBitmap(hdc, iImageWidth, iImageHeight);		//비트맵생성 : 600 X 530
	hMemDC = CreateCompatibleDC(hdc);										//메모리DC를 얻는다
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);					//메모리DC에 비트맵을 올린다

	// 챠트 테두리 : 550 X 500
	Rectangle(hMemDC, ZERO, ZERO, iImageWidth, iImageHeight);

	// 챠트 몸체	: 450 X 400
	SelectObject(hMemDC, GetStockObject(LTGRAY_BRUSH));
	Rectangle(hMemDC, iChartBegin_X-1, iChartBegin_Y-1, iChartEnd_X+1, iChartEnd_Y+1);
	Rectangle(hMemDC, iChartBegin_X, iChartBegin_Y, iChartEnd_X, iChartEnd_Y);

	// 챠트 기준선 긋기
	int iMaxValue = static_cast<int>(_dDataTable[4][_iMaxData]);
	iMaxValue = ((iMaxValue/10)*10)+10;

	iSpace_Y = (iChartHeight/(iMaxValue/10));
	iTmp_Y	 = iChartBegin_Y;
	int iNum = (iMaxValue/10);
	for( int i = 0 ; i < iNum ; i++ )
	{
		iTmp_Y += iSpace_Y;

		MoveToEx(hMemDC, iChartBegin_X, iTmp_Y, NULL);
		LineTo(hMemDC, iChartEnd_X, iTmp_Y);
	}

	// 설명 박스
	int iIntroBox_Begin_X	= iChartEnd_X+15;
	int iIntroBox_Begin_Y	= iChartBegin_Y;
	int iIntroBox_End_X		= iImageWidth-15;
	int iIntroBox_End_Y		= iChartBegin_Y+200;
	int iIntroBox_Height	= iIntroBox_End_Y - iIntroBox_Begin_Y;
	dTmp = iIntroBox_Height;

	SelectObject(hMemDC, GetStockObject(WHITE_BRUSH));
	Rectangle(hMemDC, iIntroBox_Begin_X, iIntroBox_Begin_Y, iIntroBox_End_X, iIntroBox_End_Y);
	for( int i = 1 ; i < 7 ; i++ )
	{
		hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(iRGB_Set[i-1][0],iRGB_Set[i-1][1],iRGB_Set[i-1][2]));
		hOldPen = (HPEN)SelectObject(hMemDC, hPen);

		Rectangle(hMemDC, (iIntroBox_Begin_X+5), iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*i)-3,
				 (iIntroBox_Begin_X+12), iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*i)+3 );
		Rectangle(hMemDC, (iIntroBox_Begin_X+6), iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*i)-2,
				 (iIntroBox_Begin_X+10), iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*i)+2 );
	}

	//폰트설정
	hFont = CreateFont(12, 0, 900, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, L"그래픽");
	hOldFont = (HFONT)SelectObject(hMemDC, hFont);

	////////////////////
	//90도 회전텍스트
	////////////////////
	// Frame Rate 출력
	TextOut(hMemDC, 8, 250, L"Frame Rate", 10);

	// 챠트의 X축 기준값
	SetTextAlign(hMemDC, TA_RIGHT);
	iTmp_X	 = iChartBegin_X ;
	iTmp_Y	 = iChartEnd_Y + 5;

	dTmp = 0.0;
	TextOut( hMemDC, iTmp_X, iTmp_Y, _pTerm[0], 10 );
	for( int i = 0 ; i < _Index ; i++ )
	{
		dTmp = i;
		iTmp_X = iChartBegin_X + static_cast<int>((dTmp/(_Index-1))*iChartWidth) - 2;

		TextOut( hMemDC, iTmp_X, iTmp_Y, _pTerm[i], 10 );
	}

	///////////////////
	//회전없는 텍스트
	////////////////////
	SelectObject(hMemDC, hOldFont);

	// 오늘 날짜 출력
	TextOut(hMemDC, 280, 9, _pToday, static_cast<int>(wcslen(_pToday)));

	// 챠트의 Y축 기준값
	iMaxValue	= static_cast<int>(_dDataTable[4][_iMaxData]);
	iMaxValue	= ((iMaxValue/10)*10)+10;
	iSpace_Y	= (iChartHeight/(iMaxValue/10));
	iTmp_X		= iChartBegin_X - 5;
	iTmp_Y		= iChartBegin_Y - 5;
	iNum = (iMaxValue/10);
	for( int i = 0 ; i < iNum ; i++ )
	{
		wsprintf( strTmp, L"%d", iMaxValue );
		TextOut(hMemDC, iTmp_X, iTmp_Y, strTmp, 2);

		iTmp_Y += iSpace_Y;
		iMaxValue -= 10;
	}

	// 설명 박스에 글자 적기
	SetTextAlign(hMemDC, TA_LEFT);
	dTmp = iIntroBox_Height;
	int iIntroBox_Begin_Text_X = ( iIntroBox_Begin_X+18 );
	wsprintf( strTmp, L"- 20  " );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*1)-9, strTmp, 6);
	wsprintf( strTmp, L"20 - 30" );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*2)-9, strTmp, 7);
	wsprintf( strTmp, L"30 - 40" );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*3)-9, strTmp, 7);
	wsprintf( strTmp, L"40 - 50" );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*4)-9, strTmp, 7);
	wsprintf( strTmp, L"50 - 60" );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*5)-9, strTmp, 7);
	wsprintf( strTmp, L"60 -  " );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*6)-9, strTmp, 6);


	////////////////////////
	// 그래프 급변점 그리기
	// ( 먼저 그리는 쪽이 그나마 더 깔끔해서 먼저 그림 : 성능은 더 나쁘지만 중요하지 않으므로 )
	// 40-50 & 50-60 그래프의 최고, 최저, 평균을 구하고 적는다.
	int iBigGaps[30] = {0};		// 이전 꺼랑 차이가 5%이상 나면 흰 선 그어줄 목록
	int iBigGap_Index = 0;		// 공간에서 몇 개나 들어있는지 알려줄 인덱스
	double d3_highest	= 0;	// 40-50 그래프에서 최고값
	double d3_lowest	= 100;	// 40-50 그래프에서 최저값
	double d3_average	= 0;	// 40-50 그래프에서 평균값
	double d4_highest	= 0;	// 50-60 그래프에서 최고값
	double d4_lowest	= 100;	// 50-60 그래프에서 최저값
	double d4_average	= 0;	// 50-60 그래프에서 평균값

	// 50-60 프레임 그래프만 검토한다. 이 그래프 변동량을 확인하면 다 알 수 있으므로.
	for( int i = 0 ; i < _Index ; i++ )
	{
		if( i != 0 )
		{
			if( 3 < _abs(_dDataTable[4][i] - _dDataTable[4][i-1]) )
			{
				iBigGaps[iBigGap_Index] = (i-1);
				iBigGap_Index++;
			}
		}
		// 40-50 % 50-60 그래프에서의 최고, 최저, 평균값을 구한다.
		d3_average += _dDataTable[3][i];
		d4_average += _dDataTable[4][i];
		if( d3_highest < _dDataTable[3][i] ){	d3_highest = _dDataTable[3][i];   }
		if( d3_lowest > _dDataTable[3][i] ){	d3_lowest = _dDataTable[3][i];   }
		if( d4_highest < _dDataTable[4][i] ){	d4_highest = _dDataTable[4][i];   }
		if( d4_lowest > _dDataTable[4][i] ){	d4_lowest = _dDataTable[4][i];   }
	}
	// d3_average & d4_average 변수에 들어있는 값을 _Index로 나누면 평균값이 나올겝니다.
	d3_average /= _Index;
	d4_average /= _Index;

    // 위에서 얻은 정보들을 도화지에 적자 - 최고, 최저, 평균
	int iBrief_X = ( iChartEnd_X + 10 );
	int iBrief_Y = ( iIntroBox_End_Y + 10 );
	int iBrief_Y_Gap = 17;
	SetTextColor( hMemDC, RGB(iRGB_Set[4][0],iRGB_Set[4][1],iRGB_Set[4][2]) );	// 군청색
	wsprintf( strTmp, L"최고 %.2f", d4_highest );
	TextOut( hMemDC, iBrief_X, iBrief_Y, strTmp, 10 );
	wsprintf( strTmp, L"최저 %.2f", d4_lowest );
	TextOut( hMemDC, iBrief_X, iBrief_Y+(iBrief_Y_Gap), strTmp, 10 );
	wsprintf( strTmp, L"평균 %.2f", d4_average );
	TextOut( hMemDC, iBrief_X, iBrief_Y+(2*iBrief_Y_Gap), strTmp, 10 );
	SetTextColor( hMemDC, RGB(iRGB_Set[3][0],iRGB_Set[3][1],iRGB_Set[3][2]) );	// 핑크색
	wsprintf( strTmp, L"최고 %.2f", d3_highest );
	TextOut( hMemDC, iBrief_X, iBrief_Y+(3*iBrief_Y_Gap)+5, strTmp, 10 );
	wsprintf( strTmp, L"최저 %.2f", d3_lowest );
	TextOut( hMemDC, iBrief_X, iBrief_Y+(4*iBrief_Y_Gap)+5, strTmp, 10 );
	wsprintf( strTmp, L"평균 %.2f", d3_average );
	TextOut( hMemDC, iBrief_X, iBrief_Y+(5*iBrief_Y_Gap)+5, strTmp, 10 );

	iMaxValue	= static_cast<int>(_dDataTable[4][_iMaxData]);		// 이 새끼가 최대값을 가진 새끼다, 이 새끼가 최대값을 안 가지고 있으면 그건 뭔가 잘못된거다.
	iMaxValue	= ((iMaxValue/10)*10)+10;

	//////////
	// 줄 긋기

	// 하얀줄 긋기 (다음 값이랑 갭이 5% 이상 나는 경우)
	hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(255,255,255));
	hOldPen = (HPEN)SelectObject(hMemDC, hPen);
	for( int i = 0 ; i < iBigGap_Index ; i++ )
	{
		dTmp = iBigGaps[i];
		iTmp_X = iChartBegin_X + static_cast<int>(((dTmp)/(_Index-1))*iChartWidth);
		iTmp_Y = iChartEnd_Y - static_cast<int>((_dDataTable[4][iBigGaps[i]]/iMaxValue)*iChartHeight);
		MoveToEx(hMemDC, iTmp_X, iTmp_Y, NULL);
		LineTo(hMemDC, iTmp_X, iChartEnd_Y);
	}

	// 빨간줄 긋기 ( 4번 그래프에서의 최저값 )
	hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(255,0,0));
	hOldPen = (HPEN)SelectObject(hMemDC, hPen);
	dTmp = _iMinData;
	iTmp_X = iChartBegin_X + static_cast<int>((dTmp/(_Index-1))*iChartWidth);
	iTmp_Y = iChartEnd_Y - static_cast<int>((_dDataTable[4][_iMinData]/iMaxValue)*iChartHeight);
	MoveToEx(hMemDC, iTmp_X, iTmp_Y, NULL);
	LineTo(hMemDC, iTmp_X, iChartEnd_Y);

	// 파란줄 긋기 ( 4번 그래프에서의 최고값 )
	hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(0,0,255));
	hOldPen = (HPEN)SelectObject(hMemDC, hPen);
	dTmp = _iMaxData;
	iTmp_X = iChartBegin_X + static_cast<int>((dTmp/(_Index-1))*iChartWidth);
	iTmp_Y = iChartEnd_Y - static_cast<int>((_dDataTable[4][_iMaxData]/iMaxValue)*iChartHeight);
	MoveToEx(hMemDC, iTmp_X, iTmp_Y, NULL);
	LineTo(hMemDC, iTmp_X, iChartEnd_Y);

	
	///////////////////////
	// 차트에 그래프 그리기 
	iSpace_X	= (iChartWidth/_Index);
	dTmp = 0.0;
	for( int i = 0 ; i < 6 ; i++ )
	{
		hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(iRGB_Set[i][0],iRGB_Set[i][1],iRGB_Set[i][2]));
		hOldPen = (HPEN)SelectObject(hMemDC, hPen);

		iTmp_X = iChartBegin_X;
		iTmp_Y = iChartEnd_Y - static_cast<int>((_dDataTable[i][0]/iMaxValue)*iChartHeight);
		if( iTmp_Y > iChartEnd_Y )
		{	iTmp_Y = iChartEnd_Y;   }

		Rectangle(hMemDC, (iTmp_X-2), (iTmp_Y-2), (iTmp_X+2), (iTmp_Y+2) );

		MoveToEx(hMemDC, iTmp_X, iTmp_Y, NULL);
		for( int j = 1 ; j < _Index ; j++ )
		{
			dTmp = j;
			iTmp_X = iChartBegin_X + static_cast<int>((dTmp/(_Index-1))*iChartWidth);
			iTmp_Y = iChartEnd_Y - static_cast<int>((_dDataTable[i][j]/iMaxValue)*iChartHeight);
			if( iTmp_Y > iChartEnd_Y )
			{	iTmp_Y = iChartEnd_Y-1;   }

			Rectangle(hMemDC, (iTmp_X-2), (iTmp_Y-2), (iTmp_X+2), (iTmp_Y+2) );
			LineTo(hMemDC, iTmp_X, iTmp_Y);
		}		
	}


	//챠트 생성완료
	//메모리 해제

	SelectObject(hMemDC, hOldPen);
	SelectObject(hMemDC, hOldBitmap);

	DeleteObject(hFont);
	DeleteDC(hMemDC);										//메모리DC 삭제
	ReleaseDC(m_hWnd, hdc);									//화면DC 릴리즈
}


//////////////////////////////////////////////////////////////////////
// Note : 메모리 DC의 내용을 BITMAP형식으로 저장한다
//////////////////////////////////////////////////////////////////////
void CImage::SaveBitmap(const wchar_t *strFilename)
{
	DDBToDIB();

	HANDLE hFile;
	DWORD dwWritten;

	//파일을 생성하고 파일 헤더와 정보 구조체, 팔레트, 래스터 데이터를 출력한다
	hFile = CreateFile(strFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, &m_stFileHeader, sizeof(m_stFileHeader), &dwWritten, NULL);
	WriteFile(hFile, m_pstInfo, m_dwSize, &dwWritten, NULL);

	CloseHandle(hFile);

	Delete();
}

//////////////////////////////////////////////////////////////////////
// Note : 비트맵 구조를 DIB형태로 변형한다
//////////////////////////////////////////////////////////////////////
void CImage::DDBToDIB()
{
	BITMAP bit;
	HDC hdc;

	//화면에 대한 DC를 구한다
	hdc = GetDC(m_hWnd);

	//비트맵 핸들로부터 정보구조체를 초기화한다
	GetObject(m_hBitmap, sizeof(BITMAP), &bit);

	ZeroMemory(&m_stInfoHeader, sizeof(BITMAPINFOHEADER));

	m_stInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_stInfoHeader.biWidth = bit.bmWidth;
	m_stInfoHeader.biHeight = bit.bmHeight;
	m_stInfoHeader.biPlanes = 1;
	m_stInfoHeader.biBitCount = bit.bmPlanes*bit.bmBitsPixel;

	if(m_stInfoHeader.biBitCount > 8) 
		m_stInfoHeader.biBitCount = 24;

	m_stInfoHeader.biCompression = BI_RGB;

	//정보구조체 + 팔레트 크기만큼 메모리를 할당하고 이 버퍼에
	//정보구조체를 복사한다
	m_nPalSize = (m_stInfoHeader.biBitCount == 24 ? 0:1 << m_stInfoHeader.biBitCount) * 
		sizeof(RGBQUAD);
	m_pstInfo = (BITMAPINFO*)malloc(m_stInfoHeader.biSize+m_nPalSize);
	m_pstInfo->bmiHeader = m_stInfoHeader;

	//비트맵의 크기를 구한다
	GetDIBits(hdc, m_hBitmap, 0, bit.bmHeight, NULL, m_pstInfo, DIB_RGB_COLORS);
	m_stInfoHeader = m_pstInfo->bmiHeader;

	//비트맵 크기가 구해지지 않았을경우 수작업으로 직접 계산한다
	if(m_stInfoHeader.biSizeImage == 0){
		m_stInfoHeader.biSizeImage = ((((m_stInfoHeader.biWidth*m_stInfoHeader.biBitCount)
			+31) & ~31) >> 3) * m_stInfoHeader.biHeight;
	}

	//래스터 데이터를 읽기위해 메모리를 재할당한다
	m_dwSize = m_stInfoHeader.biSize + m_nPalSize + m_stInfoHeader.biSizeImage;
	m_pstInfo = (BITMAPINFO*)realloc(m_pstInfo, m_dwSize);

	//래스터 데이터를 읽어들인다
	GetDIBits(hdc, m_hBitmap, 0, bit.bmHeight, 
		(PBYTE)m_pstInfo+m_stInfoHeader.biSize+m_nPalSize,
		m_pstInfo, DIB_RGB_COLORS);

	//파일 헤더를 만든다
	m_stFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_nPalSize;
	m_stFileHeader.bfReserved1 = 0;
	m_stFileHeader.bfReserved2 = 0;
	m_stFileHeader.bfSize = m_dwSize + sizeof(BITMAPFILEHEADER);
	m_stFileHeader.bfType = 0x4d42;

	ReleaseDC(m_hWnd, hdc);
}

//////////////////////////////////////////////////////////////////////
// Note : 메모리 해제
//////////////////////////////////////////////////////////////////////
void CImage::Delete()
{
	delete [] m_pstInfo;
	m_pstInfo = NULL;

	DeleteObject(m_hBitmap);
}
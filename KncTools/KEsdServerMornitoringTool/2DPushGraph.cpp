/* ===================================================================

C2DPushGraph Control (2DPushGraph.h and 2DPushGraph.cpp)

Author:  Stuart Konen
Contact: skonen@gmail.com (Job information welcome)

Description: A push graph control similiar to the graph control located
in Microsoft's Task Manager.

====================================================================*/


#include "stdafx.h"
#include "2DPushGraph.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PUSHGRAPH_MAX(num, max, type) \
((num > max) ? (type)(max) : (type)(num))

#define PUSHGRAPH_MIN(num, min, type) \
((num < min) ? (type)(min) : (type)(num))


/* Self-Register */
BOOL C2DPushGraph::m_bRegistered = C2DPushGraph::RegisterClass(); 


/////////////////////////////////////////////////////////////////////////////
// C2DPushGraphm

C2DPushGraph::C2DPushGraph()
{
	m_bDrawTodayLine   = true;
	m_bDrawYesterdayLine = true;
	m_bDrawAverageLine = false;
	m_bDrawGuestUserLine = false;

	m_nMaxCoords       = -1;
	m_nMoveOffset      = 0;
	m_nPeekOffset      = 0;
	m_bStylesModified  = false;
	m_usLineInterval   = 1;
	m_bShowGrid		   = true;
	m_bShowMinMax	   = true;
	m_bTotalCCU		   = true;
	m_bDrawMousePoint  = false;

	m_mousePoint.x	   = 0;
	m_mousePoint.y	   = 0;
	
	//SetLabelForMax("50명");
	//SetLabelForMin("0명");

	SetGridSize(20);
	

	SetBGColor  ( RGB(0, 0, 0));
	SetGridColor( RGB(0, 150, 0)   );
	SetTextColor( RGB(255, 255, 0) );
}


// ===================================================================

C2DPushGraph::~C2DPushGraph()
{
	while (m_aLines.GetSize())
	{
		delete m_aLines[0];
		m_aLines.RemoveAt(0);
	}

	while (m_aYesterdayLines.GetSize())
	{
		delete m_aYesterdayLines[0];
		m_aYesterdayLines.RemoveAt(0);
	}

	while (m_aAverageLines.GetSize())
	{
		delete m_aAverageLines[0];
		m_aAverageLines.RemoveAt(0);
	}
}


// ===================================================================

BOOL C2DPushGraph::RegisterClass()
{
	/* Static function to automatically register this class */

	WNDCLASS wc;
	
	ZeroMemory(&wc, sizeof(WNDCLASS));
                                 
    wc.lpfnWndProc   = ::DefWindowProc;
    wc.hInstance     = (HINSTANCE)::GetModuleHandle(NULL);        
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);                
    wc.lpszClassName = _T("C2DPushGraph");	

    return AfxRegisterClass(&wc);
}


BEGIN_MESSAGE_MAP(C2DPushGraph, CWnd)
	//{{AFX_MSG_MAP(C2DPushGraph)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// ===================================================================

bool C2DPushGraph::CreateFromStatic( UINT nStaticID, CWnd* pParent )
{
	CStatic  wndStatic;		

	if (pParent == NULL || !wndStatic.SubclassDlgItem(nStaticID, pParent))
	{
		return false;
	}
	

	/* Get the static windows rectangle and transform it into
	parent client coordinates, instead of screen coordinates */

	CRect    rectStatic;

	wndStatic.GetWindowRect(&rectStatic);
	pParent->ScreenToClient(&rectStatic);

		
	if ( !CreateEx( wndStatic.GetExStyle(), 
		            NULL, NULL, WS_CHILD | WS_VISIBLE | wndStatic.GetStyle(),
		            rectStatic, pParent, nStaticID, NULL))
	{		
		wndStatic.DestroyWindow();
		return false;
	}
	
	
	wndStatic.DestroyWindow();
	return true;
}


// ==========================================================================
// Functions for Setting and Retrieving attributes
// ==========================================================================


COLORREF C2DPushGraph::GetGridColor() const
{
	return m_crGridColor;
}


// ===================================================================

void C2DPushGraph::SetGridColor( COLORREF crColor )
{
	m_crGridColor = crColor;
}


// ===================================================================

COLORREF C2DPushGraph::GetTextColor() const
{
	return m_crTextColor;
}


// ===================================================================

void  C2DPushGraph::SetTextColor( COLORREF crColor )
{
	m_crTextColor = crColor;
}


// ===================================================================

COLORREF C2DPushGraph::GetBGColor() const
{
	return m_crBGColor;
}


// ===================================================================

void C2DPushGraph::SetBGColor( COLORREF crColor )
{
	m_crBGColor = crColor;
}


// ===================================================================

int C2DPushGraph::GetGridSize() const
{
	return m_nGridSize;
}


// ===================================================================

void C2DPushGraph::SetGridSize( unsigned short usWidthAndHeight )
{
	m_nGridSize = PUSHGRAPH_MIN(usWidthAndHeight, 3, int);
}


// ===================================================================

int C2DPushGraph::GetMaxPeek() const
{
	return m_nMaxPeek;
}


// ===================================================================

void C2DPushGraph::SetMaxPeek( int nMax )
{
	m_nMaxPeek = nMax;
}


// ===================================================================

LPCTSTR C2DPushGraph::GetLabelForMax() const
{
	 return (LPCTSTR)m_strMaxLabel;
}


// ===================================================================

void C2DPushGraph::SetLabelForMax( LPCTSTR lpszLabel )
{
	 m_strMaxLabel = lpszLabel;
}


// ===================================================================

int C2DPushGraph::GetMinPeek() const
{
	return m_nMinPeek;
}


// ===================================================================

void C2DPushGraph::SetMinPeek( int nMin )
{	
	if (nMin < 0)
	{
		m_nPeekOffset = 0 - nMin;		
	}
	else
	{
		m_nPeekOffset = 0;
	}

	m_nMinPeek = nMin;
}


// ===================================================================

LPCTSTR C2DPushGraph::GetLabelForMin() const
{
	 return (LPCTSTR)m_strMinLabel;
}


// ===================================================================

void C2DPushGraph::SetLabelForMin( LPCTSTR lpszLabel )
{
	 m_strMinLabel = lpszLabel;
}


// ===================================================================

void C2DPushGraph::SetPeekRange( int nMin, int nMax )
{
	ASSERT(nMin < nMax);

	SetMinPeek(nMin);
	SetMaxPeek(nMax);

	wchar_t buf[8] = {0};

	// Min Max 라벨
	wsprintf( buf, L"%d명", nMin);
	SetLabelForMin( buf );
	wsprintf( buf, L"%d명", nMax);
	SetLabelForMax( buf );

	// Middle 라벨
	wsprintf( buf, L"%d명", nMax/5 );
	m_strMidLabel[0] = buf;
	wsprintf( buf, L"%d명", nMax/5*2 );
	m_strMidLabel[1] = buf;
	wsprintf( buf, L"%d명", nMax/5*3 );
	m_strMidLabel[2] = buf;
	wsprintf( buf, L"%d명", nMax/5*4 );
	m_strMidLabel[3] = buf;
}


// ===================================================================

unsigned short C2DPushGraph::GetInterval() const
{
	return m_usLineInterval;
}


// ===================================================================

void C2DPushGraph::SetInterval( unsigned short usInterval )
{
	m_usLineInterval = usInterval;
	m_nMaxCoords = -1; // Forces reset
}


// ===================================================================

bool C2DPushGraph::AddLine( UINT uiLineID, COLORREF crColor )
{
	PushGraphLine *pNewLine = new PushGraphLine(uiLineID);

	if (!pNewLine)
	{
		ASSERT(pNewLine && "Out of memory");
		return false;
	}

	pNewLine->crLine = crColor;
	m_aLines.Add(pNewLine);

	return true;
}

bool C2DPushGraph::AddYesterdayLine( UINT uiLineID, COLORREF crColor )
{
	PushGraphLine *pNewLine = new PushGraphLine(uiLineID);

	if (!pNewLine)
	{
		ASSERT(pNewLine && "Out of memory");
		return false;
	}

	pNewLine->crLine = crColor;
	m_aYesterdayLines.Add(pNewLine);

	return true;
}

bool C2DPushGraph::AddAverageLine( UINT uiLineID, COLORREF crColor )
{
	PushGraphLine *pNewLine = new PushGraphLine(uiLineID);

	if (!pNewLine)
	{
		ASSERT(pNewLine && "Out of memory");
		return false;
	}

	pNewLine->crLine = crColor;
	m_aAverageLines.Add(pNewLine);

	return true;
}

bool C2DPushGraph::AddGuestUserLine( UINT uiLineID, COLORREF crColor )
{
	PushGraphLine *pNewLine = new PushGraphLine(uiLineID);

	if (!pNewLine)
	{
		ASSERT(pNewLine && "Out of memory");
		return false;
	}

	pNewLine->crLine = crColor;
	m_aGuestUserLines.Add(pNewLine);

	return true;
}


// ===================================================================

void C2DPushGraph::RemoveLine( UINT uiLineID )
{
	for (int n = static_cast<int>(m_aLines.GetSize())-1; n >= 0; --n)
	{
		if (m_aLines[n]->uiID == uiLineID)
		{
			delete m_aLines[n];
			m_aLines.RemoveAt(n);

			--n; // Because we removed a line
		}
	}	
}

void C2DPushGraph::RemoveYesterdayLine( UINT uiLineID )
{
	for (int n = static_cast<int>(m_aYesterdayLines.GetSize())-1; n >= 0; --n)
	{
		if (m_aYesterdayLines[n]->uiID == uiLineID)
		{
			delete m_aYesterdayLines[n];
			m_aYesterdayLines.RemoveAt(n);

			--n; // Because we removed a line
		}
	}	
}

void C2DPushGraph::RemoveAverageLine( UINT uiLineID )
{
	for (int n = static_cast<int>(m_aAverageLines.GetSize())-1; n >= 0; --n)
	{
		if (m_aAverageLines[n]->uiID == uiLineID)
		{
			delete m_aAverageLines[n];
			m_aAverageLines.RemoveAt(n);

			--n; // Because we removed a line
		}
	}	
}

void C2DPushGraph::RemoveGuestUserLine( UINT uiLineID )
{
	for (int n = static_cast<int>(m_aGuestUserLines.GetSize())-1; n >= 0; --n)
	{
		if (m_aGuestUserLines[n]->uiID == uiLineID)
		{
			delete m_aGuestUserLines[n];
			m_aGuestUserLines.RemoveAt(n);

			--n; // Because we removed a line
		}
	}	
}


// ===================================================================

bool C2DPushGraph::SetLineColor( COLORREF crColor, UINT uiLineID )
{
	PushGraphLine *pLine = internal_LineFromID(uiLineID);
	
	if (pLine == NULL)
	{
		return false; 
	}
	
	pLine->crLine = crColor;
	return true;
}


// ===================================================================

COLORREF C2DPushGraph::GetLineColor( UINT uiLineID )
{
	PushGraphLine *pLine = internal_LineFromID(uiLineID);
	return (pLine) ? pLine->crLine : RGB(0, 0, 0);
}


// ===================================================================

bool C2DPushGraph::Push( int nMagnitude, UINT uiLineID )
{
	PushGraphLine *pLine = internal_LineFromID(uiLineID);
	
	if (pLine == NULL)
	{
		return false; 
	}

	
	/* Now add the magnitude (push point) to the array of push points, but
	first restrict it to the peek bounds */

	if (nMagnitude > m_nMaxPeek) {
		nMagnitude = m_nMaxPeek;
	}
	else if (nMagnitude < m_nMinPeek) {
		nMagnitude = m_nMinPeek;
	}
	
	nMagnitude -= m_nMinPeek;
	nMagnitude += m_nPeekOffset;
	
	pLine->aMagnitudes.Add(nMagnitude);
	return true;
}

bool C2DPushGraph::PushYesterdayLine( int nMagnitude, UINT uiLineID )
{
	PushGraphLine *pLine = internal_YesterdayLineFromID(uiLineID);

	if (pLine == NULL)
	{
		return false; 
	}


	/* Now add the magnitude (push point) to the array of push points, but
	first restrict it to the peek bounds */

	if (nMagnitude > m_nMaxPeek) {
		nMagnitude = m_nMaxPeek;
	}
	else if (nMagnitude < m_nMinPeek) {
		nMagnitude = m_nMinPeek;
	}

	nMagnitude -= m_nMinPeek;
	nMagnitude += m_nPeekOffset;

	pLine->aMagnitudes.Add(nMagnitude);
	return true;
}

bool C2DPushGraph::PushAverageLine( int nMagnitude, UINT uiLineID )
{
	PushGraphLine *pLine = internal_AverageLineFromID(uiLineID);

	if (pLine == NULL)
	{
		return false; 
	}


	/* Now add the magnitude (push point) to the array of push points, but
	first restrict it to the peek bounds */

	if (nMagnitude > m_nMaxPeek) {
		nMagnitude = m_nMaxPeek;
	}
	else if (nMagnitude < m_nMinPeek) {
		nMagnitude = m_nMinPeek;
	}

	nMagnitude -= m_nMinPeek;
	nMagnitude += m_nPeekOffset;

	pLine->aMagnitudes.Add(nMagnitude);
	return true;
}

bool C2DPushGraph::PushGuestUserLine( int nMagnitude, UINT uiLineID )
{
	PushGraphLine *pLine = internal_GuestUserLineFromID(uiLineID);

	if (pLine == NULL)
	{
		return false; 
	}


	/* Now add the magnitude (push point) to the array of push points, but
	first restrict it to the peek bounds */

	if (nMagnitude > m_nMaxPeek) {
		nMagnitude = m_nMaxPeek;
	}
	else if (nMagnitude < m_nMinPeek) {
		nMagnitude = m_nMinPeek;
	}

	nMagnitude -= m_nMinPeek;
	nMagnitude += m_nPeekOffset;

	pLine->aMagnitudes.Add(nMagnitude);
	return true;
}


// ===================================================================

void C2DPushGraph::ShowAsBar( UINT uiLineID, bool bAsBar )
{
	PushGraphLine *pLine = internal_LineFromID(uiLineID);
	
	if (pLine)
	{
		pLine->bShowAsBar = bAsBar;
	}
}


// ===================================================================

void C2DPushGraph::Update()
{
	int nGreatest = 0;  // Largest push point count
	
	
	for (int n=0; n < m_aLines.GetSize(); ++n)
	{
		if (nGreatest < m_aLines[n]->aMagnitudes.GetSize())
		{
			nGreatest = static_cast<int>(m_aLines[n]->aMagnitudes.GetSize());
		}
	}

	if (nGreatest >= m_nMaxCoords)
	{
		m_nMoveOffset = 
			(m_nMoveOffset - (((nGreatest-m_nMaxCoords)+1)*m_usLineInterval))
			% m_nGridSize;	
	}

	RedrawWindow();
}

void C2DPushGraph::YesterdayLineUpdate()
{
	int nGreatest = 0;  // Largest push point count


	for (int n=0; n < m_aYesterdayLines.GetSize(); ++n)
	{
		if (nGreatest < m_aYesterdayLines[n]->aMagnitudes.GetSize())
		{
			nGreatest = static_cast<int>(m_aYesterdayLines[n]->aMagnitudes.GetSize());
		}
	}

	if (nGreatest >= m_nMaxCoords)
	{
		m_nMoveOffset = 
			(m_nMoveOffset - (((nGreatest-m_nMaxCoords)+1)*m_usLineInterval))
			% m_nGridSize;	
	}

	RedrawWindow();
}

void C2DPushGraph::AverageLineUpdate()
{
	int nGreatest = 0;  // Largest push point count


	for (int n=0; n < m_aAverageLines.GetSize(); ++n)
	{
		if (nGreatest < m_aAverageLines[n]->aMagnitudes.GetSize())
		{
			nGreatest = static_cast<int>(m_aAverageLines[n]->aMagnitudes.GetSize());
		}
	}

	if (nGreatest >= m_nMaxCoords)
	{
		m_nMoveOffset = 
			(m_nMoveOffset - (((nGreatest-m_nMaxCoords)+1)*m_usLineInterval))
			% m_nGridSize;	
	}

	RedrawWindow();
}

void C2DPushGraph::GuestUserLineUpdate()
{
	int nGreatest = 0;  // Largest push point count


	for (int n=0; n < m_aGuestUserLines.GetSize(); ++n)
	{
		if (nGreatest < m_aGuestUserLines[n]->aMagnitudes.GetSize())
		{
			nGreatest = static_cast<int>(m_aGuestUserLines[n]->aMagnitudes.GetSize());
		}
	}

	if (nGreatest >= m_nMaxCoords)
	{
		m_nMoveOffset = 
			(m_nMoveOffset - (((nGreatest-m_nMaxCoords)+1)*m_usLineInterval))
			% m_nGridSize;	
	}

	RedrawWindow();
}


// ===================================================================

void C2DPushGraph::OnPaint() 
{	
	CPaintDC paintDC(this);	
	

	/* First we create the back buffer */
	CDC &dc = internal_InitBackBuffer(paintDC);
	

	/* Ensure we don't draw out of our client rectangle */
	
	CRect &rectClient = getClientRect();
	dc.IntersectClipRect(&rectClient);
	

	/* Fill the background */
	dc.FillSolidRect(&rectClient, m_crBGColor);


	dc.SetBkMode(TRANSPARENT);
	


	if (m_bShowMinMax)
	{
		/* Show maximum and minimum labels */

		internal_DrawMinMax(dc, rectClient);
	}

	if (m_bShowGrid)
	{
		/* Show the grid overlay */

		internal_DrawGrid(dc, rectClient);
	}

	// 라인의 시작 기준은 0시 다음 픽셀로 설정
	rectClient.left += 2;

	if( m_bDrawYesterdayLine )
		internal_DrawYesterdayLines(dc, rectClient);

	if( m_bDrawTodayLine )
		internal_DrawLines(dc, rectClient);

	if( m_bDrawAverageLine )
		internal_DrawAverageLines(dc, rectClient);

	if( m_bDrawGuestUserLine )
		internal_DrawGuestUserLines(dc, rectClient);

	// mouse point info
	if( m_bDrawMousePoint )
		internal_DrawMousePointInfo( dc );

	internal_FreeBackBuffer(paintDC);

	if (m_bStylesModified)
	{
		/* Reset styles if resized */

		GetParent()->ModifyStyle(WS_CLIPCHILDREN, 0);
		ModifyStyle(WS_CLIPSIBLINGS, 0);

		m_bStylesModified = false;
	}
}


void C2DPushGraph::OnMouseMove(UINT nFlags, CPoint point)
{
	m_mousePoint.x = point.x;
	m_mousePoint.y = point.y;

	Invalidate();
}

void C2DPushGraph::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bDrawMousePoint = true;
	Invalidate();
}

void C2DPushGraph::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDrawMousePoint = false;
	Invalidate();
}

void C2DPushGraph::internal_DrawMousePointInfo( CDC& dc )
{
	CPen GridPen( PS_SOLID, 1, m_crGridColor );
	CPen *pOldPen = dc.SelectObject(&GridPen);
	CBrush blackBrush( RGB( 0, 0, 0 ) );
	CBrush *pOldBrush = dc.SelectObject(&blackBrush);
	
	dc.Rectangle( m_mousePoint.x-65, m_mousePoint.y-20, m_mousePoint.x, m_mousePoint.y );

	int iPixelValue = m_nMaxPeek / 200;
	int iMaxPeek = m_nMaxPeek;
	
	iMaxPeek -= iPixelValue * m_mousePoint.y;

	CString strPointCount;
	strPointCount.Format( L"%d 명", iMaxPeek );

	dc.TextOut( m_mousePoint.x-63, m_mousePoint.y-18, strPointCount );

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
	GridPen.DeleteObject();	
}

// ===================================================================

CDC& C2DPushGraph::internal_InitBackBuffer(CPaintDC &dc)
{
	/* Create the offscreen DC and associated bitmap */

	m_dcBack.CreateCompatibleDC(&dc);
	m_bmBack.CreateCompatibleBitmap(&dc, getClientRect().Width(),
	         getClientRect().Height());

	m_pOldBitmap = m_dcBack.SelectObject(&m_bmBack);
	return m_dcBack;
}


// ===================================================================

void C2DPushGraph::internal_FreeBackBuffer(CPaintDC &dc)
{
	/* Copy the offscreen buffer to the onscreen CPaintDC.
	Then free the back buffer objects. */

	dc.BitBlt(getClientRect().left, getClientRect().top, 
              getClientRect().Width(),  getClientRect().Height(),
              &m_dcBack, getClientRect().left, getClientRect().top,
			  SRCCOPY);

	dc.SelectObject(m_pOldBitmap);

	m_bmBack.DeleteObject();
	m_dcBack.DeleteDC();
}


// ===================================================================

PushGraphLine* C2DPushGraph::internal_LineFromID( UINT uiLineID )
{
	/* Find the corresponding line to the passed ID */

	for (int n = static_cast<int>(m_aLines.GetSize())-1; n >= 0; --n)
	{
		if (m_aLines[n]->uiID == uiLineID)
		{
			return m_aLines[n];			
		}
	}

	return NULL;
}

PushGraphLine* C2DPushGraph::internal_YesterdayLineFromID( UINT uiLineID )
{
	/* Find the corresponding line to the passed ID */

	for (int n = static_cast<int>(m_aYesterdayLines.GetSize())-1; n >= 0; --n)
	{
		if (m_aYesterdayLines[n]->uiID == uiLineID)
		{
			return m_aYesterdayLines[n];
		}
	}

	return NULL;
}

PushGraphLine* C2DPushGraph::internal_AverageLineFromID( UINT uiLineID )
{
	/* Find the corresponding line to the passed ID */

	for (int n = static_cast<int>(m_aAverageLines.GetSize())-1; n >= 0; --n)
	{
		if (m_aAverageLines[n]->uiID == uiLineID)
		{
			return m_aAverageLines[n];
		}
	}

	return NULL;
}

PushGraphLine* C2DPushGraph::internal_GuestUserLineFromID( UINT uiLineID )
{
	/* Find the corresponding line to the passed ID */

	for (int n = static_cast<int>(m_aGuestUserLines.GetSize())-1; n >= 0; --n)
	{
		if (m_aGuestUserLines[n]->uiID == uiLineID)
		{
			return m_aGuestUserLines[n];
		}
	}

	return NULL;
}


// ===================================================================

void C2DPushGraph::internal_DrawMinMax( CDC &dc, CRect& rect)
{

	CSize MaxSize = dc.GetTextExtent(m_strMaxLabel);
	CSize MinSize = dc.GetTextExtent(m_strMinLabel);

	//int nTextWidth = ((MaxSize.cx > MinSize.cx) ? MaxSize.cx : MinSize.cx) + 6;
	int nTextWidth = LINE::START_GRID;


	/* Draw the labels (max: Top) (min: Bottom) */
	
	int rePos = 7;
	dc.SetTextColor(m_crTextColor);

	dc.TextOut(nTextWidth/2-(MaxSize.cx/2), 2, m_strMaxLabel);
	dc.TextOut(nTextWidth/2-(MaxSize.cx/2), rect.Height()/5 - rePos, m_strMidLabel[3]);
	dc.TextOut(nTextWidth/2-(MaxSize.cx/2), rect.Height()/5*2 - rePos, m_strMidLabel[2]);
	dc.TextOut(nTextWidth/2-(MaxSize.cx/2), rect.Height()/5*3 - rePos, m_strMidLabel[1]);
	dc.TextOut(nTextWidth/2-(MaxSize.cx/2), rect.Height()/5*4 - rePos, m_strMidLabel[0]);
	dc.TextOut(nTextWidth/2-(MinSize.cx/2),	rect.Height()-MinSize.cy-2, m_strMinLabel);


	/* Draw the bordering line */

	CPen penBorder(PS_SOLID, 1, m_crGridColor);
	CPen *pOldPen = dc.SelectObject(&penBorder);

	dc.MoveTo(nTextWidth + 6, 0);
	dc.LineTo(nTextWidth + 6, rect.Height());


	dc.SelectObject(pOldPen);
	penBorder.DeleteObject();


	/* Offset the graph rectangle so it doesn't overlap the labels */

	rect.left = nTextWidth + 6;
	//rect.left = LINE::START_GRID;
}


// ===================================================================

void C2DPushGraph::internal_DrawLines( CDC &dc, CRect& rect)
{
	CPen penLine;
	CPen *pOldPen = NULL;

	int nGreatest = 0;
		
	if (m_nMaxCoords == -1)
	{
		/* Maximum push points not yet calculated */

		m_nMaxCoords = (rect.Width() / m_usLineInterval) + 2
		               + (rect.Width()%m_usLineInterval ? 1 : 0);

		if (m_nMaxCoords <= 0)
		{
			m_nMaxCoords = 1;
		}
	}


	for (int n=0; n < m_aLines.GetSize(); ++n)
	{		
		if (nGreatest < m_aLines[n]->aMagnitudes.GetSize())
		{
			nGreatest = static_cast<int>(m_aLines[n]->aMagnitudes.GetSize());
		}
	}


	if (nGreatest == 0)
	{		
		return; // No lines to draw
	}


	for (n = 0; n < m_aLines.GetSize(); ++n)
	{
		/* If the line has less push points than the line with the greatest
		number of push points, new push points are appended with
		the same magnitude as the previous push point. If no push points
		exist for the line, one is added with the least magnitude possible. */
		
		PushGraphLine *pLine = m_aLines[n];
		
		// 선택된것만 출력
		if( pLine->uiID == TOTAL_CCU_ID && m_bTotalCCU == false )
		{
			continue;
		}
		else
		{
			std::vector< int >::iterator vit = std::find( m_vecSelectedLines.begin(), m_vecSelectedLines.end(), (int)pLine->uiID );
			if( vit == m_vecSelectedLines.end() )
				continue;
		}
			
		if (!pLine->aMagnitudes.GetSize())
		{
			pLine->aMagnitudes.Add(m_nMinPeek);
		}

		while (pLine->aMagnitudes.GetSize() < nGreatest)
		{			
			pLine->aMagnitudes.Add(
				pLine->aMagnitudes[pLine->aMagnitudes.GetSize()-1]);
		}


		while (m_aLines[n]->aMagnitudes.GetSize() >= m_nMaxCoords)
		{
			m_aLines[n]->aMagnitudes.RemoveAt(0);
		}

		if (!m_aLines[n]->aMagnitudes.GetSize())
		{
			/* No push points to draw */
			return;			
		}


		/* Now prepare to draw the line or bar */

		penLine.CreatePen(PS_SOLID, 1, m_aLines[n]->crLine);
		pOldPen = dc.SelectObject(&penLine);


		if (pLine->bShowAsBar)
		{
			dc.MoveTo(rect.left, rect.Height());
		}
		else
		{
			dc.MoveTo(rect.left, nGreatest == 1 ? rect.Height() : 
		    rect.Height()-(pLine->aMagnitudes[0] * 
		    rect.Height()/(m_nMaxPeek-m_nMinPeek)));	
		}
		

		for (int n2 = 0; n2 < pLine->aMagnitudes.GetSize(); ++n2)
		{
			if (pLine->bShowAsBar)
			{

				/* The line is set to be shown as a bar graph, so
				first we get the bars rectangle, then draw the bar */

				CRect rectBar;

				rectBar.left   = rect.left + (n2*(m_usLineInterval)) + 1;
				rectBar.right  = rectBar.left + GetInterval() - 1;
				rectBar.bottom = rect.Height();
				rectBar.top    = rect.Height() -
				                 (pLine->aMagnitudes[n2] * rect.Height() /
				                 (m_nMaxPeek-m_nMinPeek));				

				internal_DrawBar(dc, rectBar, *pLine);
			}

			else
			{
				/* Draw a line */

				dc.LineTo(rect.left + (n2*m_usLineInterval),
				rect.Height() -
				(pLine->aMagnitudes[n2] * rect.Height() /
				(m_nMaxPeek-m_nMinPeek)) );

				/*
				dc.FillSolidRect(rect.left + (n2*m_usLineInterval) - 2,
					rect.Height() -
				(pLine->aMagnitudes[n2] * rect.Height() /
				(m_nMaxPeek-m_nMinPeek)) - 2, 4, 4, m_aLines[n]->crLine); */

			}

		}

		dc.SelectObject(pOldPen);	
		penLine.DeleteObject(); // Free Pen 
	}
}

void C2DPushGraph::internal_DrawYesterdayLines( CDC &dc, CRect &rect )
{
	CPen penLine;
	CPen *pOldPen = NULL;

	int nGreatest = 0;

	if (m_nMaxCoords == -1)
	{
		/* Maximum push points not yet calculated */

		m_nMaxCoords = (rect.Width() / m_usLineInterval) + 2
			+ (rect.Width()%m_usLineInterval ? 1 : 0);

		if (m_nMaxCoords <= 0)
		{
			m_nMaxCoords = 1;
		}
	}


	for (int n=0; n < m_aYesterdayLines.GetSize(); ++n)
	{		
		if (nGreatest < m_aYesterdayLines[n]->aMagnitudes.GetSize())
		{
			nGreatest = static_cast<int>(m_aYesterdayLines[n]->aMagnitudes.GetSize());
		}
	}


	if (nGreatest == 0)
	{		
		return; // No lines to draw
	}


	for (n = 0; n < m_aYesterdayLines.GetSize(); ++n)
	{
		/* If the line has less push points than the line with the greatest
		number of push points, new push points are appended with
		the same magnitude as the previous push point. If no push points
		exist for the line, one is added with the least magnitude possible. */

		PushGraphLine *pLine = m_aYesterdayLines[n];

		// 선택된것만 출력
		if( pLine->uiID == TOTAL_CCU_ID && m_bTotalCCU == false )
		{
			continue;
		}
		else
		{
			std::vector< int >::iterator vit = std::find( m_vecSelectedLines.begin(), m_vecSelectedLines.end(), (int)pLine->uiID );
			if( vit == m_vecSelectedLines.end() )
				continue;
		}

		if (!pLine->aMagnitudes.GetSize())
		{
			pLine->aMagnitudes.Add(m_nMinPeek);
		}

		while (pLine->aMagnitudes.GetSize() < nGreatest)
		{			
			pLine->aMagnitudes.Add(
				pLine->aMagnitudes[pLine->aMagnitudes.GetSize()-1]);
		}


		while (m_aYesterdayLines[n]->aMagnitudes.GetSize() >= m_nMaxCoords)
		{
			m_aYesterdayLines[n]->aMagnitudes.RemoveAt(0);
		}

		if (!m_aYesterdayLines[n]->aMagnitudes.GetSize())
		{
			/* No push points to draw */
			return;			
		}


		/* Now prepare to draw the line or bar */

		penLine.CreatePen(PS_SOLID, 1, m_aYesterdayLines[n]->crLine);
		pOldPen = dc.SelectObject(&penLine);


		if (pLine->bShowAsBar)
		{
			dc.MoveTo(rect.left, rect.Height());
		}
		else
		{
			dc.MoveTo(rect.left, nGreatest == 1 ? rect.Height() : 
		rect.Height()-(pLine->aMagnitudes[0] * 
			rect.Height()/(m_nMaxPeek-m_nMinPeek)));	
		}


		for (int n2 = 0; n2 < pLine->aMagnitudes.GetSize(); ++n2)
		{
			if (pLine->bShowAsBar)
			{

				/* The line is set to be shown as a bar graph, so
				first we get the bars rectangle, then draw the bar */

				CRect rectBar;

				rectBar.left   = rect.left + (n2*(m_usLineInterval)) + 1;
				rectBar.right  = rectBar.left + GetInterval() - 1;
				rectBar.bottom = rect.Height();
				rectBar.top    = rect.Height() -
					(pLine->aMagnitudes[n2] * rect.Height() /
					(m_nMaxPeek-m_nMinPeek));				

				internal_DrawBar(dc, rectBar, *pLine);
			}

			else
			{
				/* Draw a line */

				dc.LineTo(rect.left + (n2*m_usLineInterval),
					rect.Height() -
					(pLine->aMagnitudes[n2] * rect.Height() /
					(m_nMaxPeek-m_nMinPeek)) );

				/*
				dc.FillSolidRect(rect.left + (n2*m_usLineInterval) - 2,
				rect.Height() -
				(pLine->aMagnitudes[n2] * rect.Height() /
				(m_nMaxPeek-m_nMinPeek)) - 2, 4, 4, m_aYesterdayLines[n]->crLine); */

			}

		}

		dc.SelectObject(pOldPen);	
		penLine.DeleteObject(); // Free Pen 
	}
}

void C2DPushGraph::internal_DrawAverageLines( CDC &dc, CRect &rect )
{
	CPen penLine;
	CPen *pOldPen = NULL;

	int nGreatest = 0;

	if (m_nMaxCoords == -1)
	{
		/* Maximum push points not yet calculated */

		m_nMaxCoords = (rect.Width() / m_usLineInterval) + 2
			+ (rect.Width()%m_usLineInterval ? 1 : 0);

		if (m_nMaxCoords <= 0)
		{
			m_nMaxCoords = 1;
		}
	}


	for (int n=0; n < m_aAverageLines.GetSize(); ++n)
	{		
		if (nGreatest < m_aAverageLines[n]->aMagnitudes.GetSize())
		{
			nGreatest = static_cast<int>(m_aAverageLines[n]->aMagnitudes.GetSize());
		}
	}


	if (nGreatest == 0)
	{		
		return; // No lines to draw
	}


	for (n = 0; n < m_aAverageLines.GetSize(); ++n)
	{
		/* If the line has less push points than the line with the greatest
		number of push points, new push points are appended with
		the same magnitude as the previous push point. If no push points
		exist for the line, one is added with the least magnitude possible. */

		PushGraphLine *pLine = m_aAverageLines[n];

		if (!pLine->aMagnitudes.GetSize())
		{
			pLine->aMagnitudes.Add(m_nMinPeek);
		}

		while (pLine->aMagnitudes.GetSize() < nGreatest)
		{			
			pLine->aMagnitudes.Add(
				pLine->aMagnitudes[pLine->aMagnitudes.GetSize()-1]);
		}


		while (m_aAverageLines[n]->aMagnitudes.GetSize() >= m_nMaxCoords)
		{
			m_aAverageLines[n]->aMagnitudes.RemoveAt(0);
		}

		if (!m_aAverageLines[n]->aMagnitudes.GetSize())
		{
			/* No push points to draw */
			return;			
		}


		/* Now prepare to draw the line or bar */

		penLine.CreatePen(PS_SOLID, 1, m_aAverageLines[n]->crLine);
		pOldPen = dc.SelectObject(&penLine);


		if (pLine->bShowAsBar)
		{
			dc.MoveTo(rect.left, rect.Height());
		}
		else
		{
			dc.MoveTo(rect.left, nGreatest == 1 ? rect.Height() : 
		rect.Height()-(pLine->aMagnitudes[0] * 
			rect.Height()/(m_nMaxPeek-m_nMinPeek)));	
		}


		for (int n2 = 0; n2 < pLine->aMagnitudes.GetSize(); ++n2)
		{
			if (pLine->bShowAsBar)
			{

				/* The line is set to be shown as a bar graph, so
				first we get the bars rectangle, then draw the bar */

				CRect rectBar;

				rectBar.left   = rect.left + (n2*(m_usLineInterval)) + 1;
				rectBar.right  = rectBar.left + GetInterval() - 1;
				rectBar.bottom = rect.Height();
				rectBar.top    = rect.Height() -
					(pLine->aMagnitudes[n2] * rect.Height() /
					(m_nMaxPeek-m_nMinPeek));				

				internal_DrawBar(dc, rectBar, *pLine);
			}

			else
			{
				/* Draw a line */

				dc.LineTo(rect.left + (n2*m_usLineInterval),
					rect.Height() -
					(pLine->aMagnitudes[n2] * rect.Height() /
					(m_nMaxPeek-m_nMinPeek)) );

				/*
				dc.FillSolidRect(rect.left + (n2*m_usLineInterval) - 2,
				rect.Height() -
				(pLine->aMagnitudes[n2] * rect.Height() /
				(m_nMaxPeek-m_nMinPeek)) - 2, 4, 4, m_aYesterdayLines[n]->crLine); */

			}

		}

		dc.SelectObject(pOldPen);	
		penLine.DeleteObject(); // Free Pen 
	}
}

void C2DPushGraph::internal_DrawGuestUserLines( CDC &dc, CRect &rect )
{
	CPen penLine;
	CPen *pOldPen = NULL;

	int nGreatest = 0;

	if (m_nMaxCoords == -1)
	{
		/* Maximum push points not yet calculated */

		m_nMaxCoords = (rect.Width() / m_usLineInterval) + 2
			+ (rect.Width()%m_usLineInterval ? 1 : 0);

		if (m_nMaxCoords <= 0)
		{
			m_nMaxCoords = 1;
		}
	}


	for (int n=0; n < m_aGuestUserLines.GetSize(); ++n)
	{		
		if (nGreatest < m_aGuestUserLines[n]->aMagnitudes.GetSize())
		{
			nGreatest = static_cast<int>(m_aGuestUserLines[n]->aMagnitudes.GetSize());
		}
	}


	if (nGreatest == 0)
	{		
		return; // No lines to draw
	}


	for (n = 0; n < m_aGuestUserLines.GetSize(); ++n)
	{
		/* If the line has less push points than the line with the greatest
		number of push points, new push points are appended with
		the same magnitude as the previous push point. If no push points
		exist for the line, one is added with the least magnitude possible. */

		PushGraphLine *pLine = m_aGuestUserLines[n];

		if (!pLine->aMagnitudes.GetSize())
		{
			pLine->aMagnitudes.Add(m_nMinPeek);
		}

		while (pLine->aMagnitudes.GetSize() < nGreatest)
		{			
			pLine->aMagnitudes.Add(
				pLine->aMagnitudes[pLine->aMagnitudes.GetSize()-1]);
		}


		while (m_aGuestUserLines[n]->aMagnitudes.GetSize() >= m_nMaxCoords)
		{
			m_aGuestUserLines[n]->aMagnitudes.RemoveAt(0);
		}

		if (!m_aGuestUserLines[n]->aMagnitudes.GetSize())
		{
			/* No push points to draw */
			return;			
		}


		/* Now prepare to draw the line or bar */

		penLine.CreatePen(PS_SOLID, 1, m_aGuestUserLines[n]->crLine);
		pOldPen = dc.SelectObject(&penLine);


		if (pLine->bShowAsBar)
		{
			dc.MoveTo(rect.left, rect.Height());
		}
		else
		{
			dc.MoveTo(rect.left, nGreatest == 1 ? rect.Height() : 
		rect.Height()-(pLine->aMagnitudes[0] * 
			rect.Height()/(m_nMaxPeek-m_nMinPeek)));	
		}


		for (int n2 = 0; n2 < pLine->aMagnitudes.GetSize(); ++n2)
		{
			if (pLine->bShowAsBar)
			{

				/* The line is set to be shown as a bar graph, so
				first we get the bars rectangle, then draw the bar */

				CRect rectBar;

				rectBar.left   = rect.left + (n2*(m_usLineInterval)) + 1;
				rectBar.right  = rectBar.left + GetInterval() - 1;
				rectBar.bottom = rect.Height();
				rectBar.top    = rect.Height() -
					(pLine->aMagnitudes[n2] * rect.Height() /
					(m_nMaxPeek-m_nMinPeek));				

				internal_DrawBar(dc, rectBar, *pLine);
			}

			else
			{
				/* Draw a line */

				dc.LineTo(rect.left + (n2*m_usLineInterval),
					rect.Height() -
					(pLine->aMagnitudes[n2] * rect.Height() /
					(m_nMaxPeek-m_nMinPeek)) );

				/*
				dc.FillSolidRect(rect.left + (n2*m_usLineInterval) - 2,
				rect.Height() -
				(pLine->aMagnitudes[n2] * rect.Height() /
				(m_nMaxPeek-m_nMinPeek)) - 2, 4, 4, m_aYesterdayLines[n]->crLine); */

			}

		}

		dc.SelectObject(pOldPen);	
		penLine.DeleteObject(); // Free Pen 
	}
}

// ===================================================================

void C2DPushGraph::internal_DrawBar( CDC &dc, CRect& rect, 
                                     PushGraphLine& rLine )
{
	COLORREF &crFill = rLine.crLine;
	GraphColor gcTopLeft, gcBottomRight;
		
	
	/* gcTopLeft is the left and top frame color, gcBottomRight
	is the right and bottom frame color */	


	if (GetRValue(crFill))
	{
		gcTopLeft.bRed     = PUSHGRAPH_MAX(GetRValue(crFill)+40, 255, BYTE);
		gcBottomRight.bRed = PUSHGRAPH_MIN(GetRValue(crFill) - 40, 0, BYTE);
	}

	if (GetBValue(crFill))
	{
		gcTopLeft.bBlue      = PUSHGRAPH_MAX(GetBValue(crFill)+40, 255, BYTE);
		gcBottomRight.bBlue  = PUSHGRAPH_MIN(GetBValue(crFill) - 40, 0, BYTE);
	}

	if (GetGValue(crFill))
	{
		gcTopLeft.bGreen     = PUSHGRAPH_MAX(GetGValue(crFill)+40, 255, BYTE);
		gcBottomRight.bGreen = PUSHGRAPH_MIN(GetGValue(crFill) - 40, 0, BYTE);
	}
			 

	dc.FillSolidRect(&rect, rLine.crLine);
	dc.Draw3dRect(&rect, gcTopLeft, gcBottomRight);
}


// ===================================================================

void C2DPushGraph::internal_DrawGrid( CDC &dc, CRect& rect)
{
	/* Draw the grid overlay. 
	
	We use rect.left as our x offset instead of zero because
	if m_bShowMinMax is true, then rect.left is set to the 
	first pixel after the labels. */

	CPen GridPen( PS_SOLID, 1, m_crGridColor );
	CPen *pOldPen = dc.SelectObject(&GridPen);


	for (int n = rect.Height()-1; n >= 0; n -= m_nGridSize)
	{
		dc.MoveTo(rect.left, n);
		dc.LineTo(rect.right, n);
	}
		
	for (n = rect.left /*+ m_nMoveOffset*/; n < rect.right; n += m_nGridSize) // 그리드가 이동하지 않도록 주석처리
	{
		if (n < rect.left)
		{
			continue;
		}

		dc.MoveTo(n, 0);
		dc.LineTo(n, rect.Height());
	}

	dc.SelectObject(pOldPen);
	GridPen.DeleteObject();
}


// ===================================================================

CRect& C2DPushGraph::getClientRect()
{
	static CRect rectClient;
	return (GetClientRect(&rectClient), rectClient);
}


// ===================================================================

BOOL C2DPushGraph::OnEraseBkgnd(CDC*) 
{
	return FALSE;
}


// ===================================================================

void C2DPushGraph::OnSizing(UINT fwSide, LPRECT pRect) 
{

	if (GetParent()->GetStyle() & ~WS_CLIPCHILDREN)
	{
		/* Eliminate flickering */

		m_bStylesModified = true;
		GetParent()->ModifyStyle(0, WS_CLIPCHILDREN);		
	}

	ModifyStyle(0, WS_CLIPSIBLINGS);


	CWnd::OnSizing(fwSide, pRect);		
	(m_nMaxCoords = -1, Invalidate());	
}


// ===================================================================

void C2DPushGraph::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	(m_nMaxCoords = -1, Invalidate());
}


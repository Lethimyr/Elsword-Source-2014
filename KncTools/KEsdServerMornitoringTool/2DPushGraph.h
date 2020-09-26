/* ===================================================================

C2DPushGraph Control (2DPushGraph.h and 2DPushGraph.cpp)

Author:  Stuart Konen
Contact: skonen@gmail.com (Job information welcome)

Description: A push graph control similiar to the graph control located
in Microsoft's Task Manager.

====================================================================*/


#if !defined(AFX_2DPUSHGRAPH_H_INCLUDED)
#define AFX_2DPUSHGRAPH_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <windows.h>
#include "afxtempl.h"
#include <vector>
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// C2DPushGraph window

#define TOTAL_CCU_ID	0
#define AVERAGE_CCU_ID	0xffffffff

// ===================================================================

struct GraphColor
{	
	GraphColor() { bRed = bGreen = bBlue = 0; }

	operator COLORREF() { return RGB(bRed, bGreen, bBlue); }
	BYTE bRed, bGreen, bBlue;
};


// ===================================================================

struct PushGraphLine
{
	PushGraphLine(UINT uiLineID)
	{
		crLine     = RGB(0, 255, 0);
		uiID       = uiLineID;
		bShowAsBar = false;
	}

	COLORREF crLine;
	UINT     uiID;
	bool     bShowAsBar;

	CArray<int, int> aMagnitudes;	
};


// ===================================================================

class C2DPushGraph : public CWnd
{
	enum LINE
	{
		START_GRID = 62,
	};

public:

	C2DPushGraph();
	virtual ~C2DPushGraph();
	
	bool CreateFromStatic( UINT nStaticID, CWnd* pParent );


	/* Functions that retrieve attributes */
	
	LPCTSTR   GetLabelForMax() const;
	LPCTSTR   GetLabelForMin() const;


	COLORREF  GetBGColor()   const;	
	COLORREF  GetGridColor() const;
	COLORREF  GetLineColor( UINT uiLineID );
	COLORREF  GetTextColor() const;	

	int       GetGridSize()  const;
	int       GetMaxPeek()   const;
	int       GetMinPeek()   const;

	unsigned short GetInterval() const;	


	/* Functions that set attributes */

	void SetBGColor(COLORREF crColor);
	void SetGridColor(COLORREF crColor);
	void SetTextColor(COLORREF crColor);
	void SetGridSize( unsigned short usWidthAndHeight );
	void SetMaxPeek(int nMax);
	void SetMinPeek(int nMin);
	void SetPeekRange(int nMin, int nMax);
	void SetLabelForMax( LPCTSTR lpszLabel );
	void SetLabelForMin( LPCTSTR lpszLabel );
	
	
	void SetInterval( unsigned short usInterval );
	bool SetLineColor( COLORREF crColor, UINT uiLineID );

	
	/* Line control functions */
	
	bool    AddLine( UINT uiLineID, COLORREF crColor );
	bool    AddYesterdayLine( UINT uiLineID, COLORREF crColor );
	bool    AddAverageLine( UINT uiLineID, COLORREF crColor );
	bool	AddGuestUserLine( UINT uiLineID, COLORREF crColor );

	void    RemoveLine( UINT uiLineID );
	void	RemoveYesterdayLine( UINT uiLineID );
	void	RemoveAverageLine( UINT uiLineID );
	void	RemoveGuestUserLine( UINT uiLineID );

	bool    Push( int nMagnitude, UINT uiLineID );
	bool    PushYesterdayLine( int nMagnitude, UINT uiLineID );
	bool    PushAverageLine( int nMagnitude, UINT uiLineID );
	bool	PushGuestUserLine( int nMagnitude, UINT uiLineID );

	void    ShowAsBar( UINT uiLineID, bool bAsBar );
	void    Update();	
	void	YesterdayLineUpdate();
	void	AverageLineUpdate();
	void	GuestUserLineUpdate();


	/* Visibility Functions */

	inline void ShowGrid( bool bShow = true)
	{ m_bShowGrid = bShow; }

	inline void ShowLabels( bool bShow = true)
	{ m_bShowMinMax = bShow; }


	CRect& getClientRect();

	void SetDrawTodayLine( bool val ) { m_bDrawTodayLine = val; RedrawWindow(); }
	void SetDrawYesterdayLine( bool val ) { m_bDrawYesterdayLine = val; RedrawWindow(); }
	void SetDrawAverageLine( bool val ) { m_bDrawAverageLine = val; RedrawWindow(); }
	void SetDrawGuestUserLine( bool val ) { m_bDrawGuestUserLine = val; RedrawWindow(); }

	std::vector< int >& GetSelectedLines() { return m_vecSelectedLines; }
	void SetTotalCCUDraw( bool bTotalCCUDraw ) { m_bTotalCCU = bTotalCCUDraw; }	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C2DPushGraph)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(C2DPushGraph)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	static BOOL m_bRegistered;
	static BOOL RegisterClass();

	DECLARE_MESSAGE_MAP()

private:

	virtual void   internal_DrawGrid( CDC &dc, CRect &rect );
	virtual void   internal_DrawLines( CDC &dc, CRect &rect );
	virtual void   internal_DrawYesterdayLines( CDC &dc, CRect &rect );
	virtual void   internal_DrawAverageLines( CDC &dc, CRect &rect );
	virtual void   internal_DrawGuestUserLines( CDC &dc, CRect &rect );
	virtual void   internal_DrawBar( CDC &dc, CRect &rect, 
	                                 PushGraphLine& rLine );
	virtual void   internal_DrawMinMax( CDC &dc, CRect& rect);

	CDC&           internal_InitBackBuffer( CPaintDC &dc );
	void           internal_FreeBackBuffer( CPaintDC &dc );

	PushGraphLine* internal_LineFromID( UINT uiLineID );
	PushGraphLine* internal_YesterdayLineFromID( UINT uiLineID );
	PushGraphLine* internal_AverageLineFromID( UINT uiLineID );
	PushGraphLine* internal_GuestUserLineFromID( UINT uiLineID );

	void		   internal_DrawMousePointInfo( CDC& dc );
		
	/* Internal data members */

	bool		m_bDrawTodayLine;
	bool		m_bDrawYesterdayLine;
	bool		m_bDrawAverageLine;
	bool		m_bDrawGuestUserLine;

	COLORREF m_crTextColor;
	COLORREF m_crBGColor;
	COLORREF m_crGridColor;

	CString  m_strMaxLabel;
	CString  m_strMidLabel[4];
	CString  m_strMinLabel;

	bool   m_bShowMinMax;
	bool   m_bShowGrid;
	bool   m_bStylesModified;

	int    m_nMoveOffset;
	int    m_nMaxCoords;
	int    m_nMaxPeek;
	int    m_nMinPeek;
	int    m_nGridSize;
	int    m_nPeekOffset;
	

	CBitmap  *m_pOldBitmap; // Original bitmap
	CBitmap  m_bmBack;      // Bitmap for backbuffering	
    CDC      m_dcBack;	

	unsigned short m_usLineInterval;
	CArray<PushGraphLine*, PushGraphLine*> m_aLines;
	CArray<PushGraphLine*, PushGraphLine*> m_aYesterdayLines;
	CArray<PushGraphLine*, PushGraphLine*> m_aAverageLines;
	CArray<PushGraphLine*, PushGraphLine*> m_aGuestUserLines;

	// 선택된 id만 그리도록
	std::vector< int > m_vecSelectedLines;

	// 전체 동접자 그리기
	bool	m_bTotalCCU;

	// 마우스 동접 뷰어
	POINT	m_mousePoint;
	bool	m_bDrawMousePoint;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_2DPUSHGRAPH_H_INCLUDED)

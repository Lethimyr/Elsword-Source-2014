#pragma once

class KBrowserWrapper
{
public:
    // Constructor
    KBrowserWrapper( void );
    // Destructor
    ~KBrowserWrapper( void );

#ifdef SEND_NEXON_WISE_LOG_BY_URL // ±Ë≈¬»Ø
	BOOL Create( RECT& rect, HWND hWndParent, DWORD dwStyle = SW_SHOW );
#else // SEND_NEXON_WISE_LOG_BY_URL
	BOOL Create( RECT& rect, HWND hWndParent );
#endif //  SEND_NEXON_WISE_LOG_BY_URL
    bool OpenBrowser( HWND hDlg );
    void CloseBrowser( void );

    inline BOOL Init()
    {
		return InitPCHWebBrowser();
    }
    inline BOOL UnInit( void )
    {
        return UnInitPCHWebBrowser();
    }
    inline void SetCookie( const std::string& strID, const std::string& strNickName, const std::string& strEtc )
    {
		SetPCHWebBrowserCookie( strID, strNickName, strEtc );
    }
    inline bool AllocateBrowserObj( void )
    {
		m_pPCHBrowser = new KPCHWebBrowser();
		return ( m_pPCHBrowser != NULL );
    }
    inline void DeleteBrowserObj( void )
    {
		SAFE_DELETE( m_pPCHBrowser );
    }
	// strPostString_ <- Post«“ String
	inline BOOL Navigate( const char* strURL, const char* strPostString_ )
    {
		if ( NULL == m_pPCHBrowser ) 
			return FALSE;
		return m_pPCHBrowser->Navigate( strURL, strPostString_ );
    }
    inline bool IsClosed( void ) const
    {
        return m_bClosed;
    }
    inline HWND GetWndHandle( void ) const
    {
        return m_hDlg;
    }

	inline void OnKeyboardEvents( LPMSG pMsg )
	{
		if( NULL != m_pPCHBrowser )
		{
			m_pPCHBrowser->OnKeyboardEvents( pMsg );
		}
	}

	inline bool GetCheckClosed( void ) const { return m_bCheckClosed; }
	inline void SetCheckClosed( bool bCheck ) { m_bCheckClosed = bCheck; }

private:
    RECT            m_rect;
    bool            m_bClosed;
    HWND            m_hDlg;

	bool			m_bCheckClosed;

public:
    KPCHWebBrowser* m_pPCHBrowser;
};

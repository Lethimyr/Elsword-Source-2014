#pragma once

#pragma comment(lib, "../X2Lib/OnlyGlobal/ID/SSOWebLib/SSOWebLib_MD.lib")	
#pragma comment(lib, "../X2Lib/OnlyGlobal/ID/SSOWebLib/SSOWebLib_MDd.lib")	

#include "../X2Lib/OnlyGlobal/ID/SSOWebLib/SSOWebLib.h"

void SWEBCallBack_OnQuit();

class KNMBrowserWrapper
{
public:
    // Constructor
    KNMBrowserWrapper( void );
    // Destructor
    ~KNMBrowserWrapper( void );

	BOOL Create( RECT& rect, HWND hWndParent );
	bool OpenBrowser( HWND hDlg );
	void DestroySSOBrowser( void )
	{
		if( m_pSSOBrowser != NULL )
		{
			m_pSSOBrowser->Destroy();
			SWEBCallBack_OnQuit();
		}
	}
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
		m_pSSOBrowser = new CSSOWebBrowser();
		return ( m_pSSOBrowser != NULL );
    }
	// strPostString_ <- PostÇÒ String
	inline BOOL Navigate( const char* strURL )
    {
		if ( NULL == m_pSSOBrowser ) 
			return FALSE;
		return m_pSSOBrowser->Navigate( strURL );
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
		//if( NULL != m_pSSOBrowser )
		//{
		//	m_pSSOBrowser->OnKeyboardEvents( pMsg );
		//}
	}

	inline bool GetCheckClosed( void ) const { return m_bCheckClosed; }
	inline void SetCheckClosed( bool bCheck ) { m_bCheckClosed = bCheck; }

private:
    RECT            m_rect;
    bool            m_bClosed;
    HWND            m_hDlg;

	bool			m_bCheckClosed;

	CSSOWebBrowser*	m_pSSOBrowser;
};

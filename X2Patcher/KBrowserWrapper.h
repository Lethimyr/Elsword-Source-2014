// file   : KBrowserWrapper.h
// since  : 2003-11-10 ¿ÀÈÄ 12:24:29
// author : pch413@kogstudios.com
// This requires IE 5.0 ( or better ).

#ifndef __BROWSER_WRAPPER_H__
#define __BROWSER_WRAPPER_H__

#define _WIN32_DCOM
#include <windows.h>
#include <tchar.h>
#include <string>

class KBrowserWrapper
{
public:
    // Member constants
    enum
    {
        // Actions
        ACTION_GOBACK,
        ACTION_GOFORWARD,
        ACTION_GOHOME,
        ACTION_SEARCH,
        ACTION_REFRESH,
        ACTION_STOP,

        // Browser type
        BROWSER_TYPE_POPUP = 100,
        BROWSER_TYPE_CHILD = 101,

        // Forces this enumeration to compile to 32 bits in size.
        // This value is not used.
        CONSTANTS_FORCE_DWORD = 0x7fffffff,
    };

    // Constructor
    explicit KBrowserWrapper( HINSTANCE hInstance = NULL );
    // Destructor
    ~KBrowserWrapper( void );

    // Methods
    bool Create( LPTSTR szWebPage = _T("http://www.kogstudios.com/"),
                 int x            = CW_USEDEFAULT,
                 int y            = CW_USEDEFAULT,
                 int w            = CW_USEDEFAULT,
                 int h            = CW_USEDEFAULT,
                 HWND hParentWnd  = HWND_DESKTOP,
                 DWORD type       = BROWSER_TYPE_POPUP );
    void Destroy( void );
    void Repaint( void );
    void Navigate( LPTSTR szWebPage = _T("http://www.kogstudios.com/") );
    void DoAction( DWORD dwAction );

    // Callback functions
    static LRESULT CALLBACK WndProc( HWND hWnd,
                                     UINT uMsg,
                                     WPARAM wParam,
                                     LPARAM lParam );
    static LRESULT CALLBACK BrowserWndProc( HWND hWnd,
                                            UINT uMsg,
                                            WPARAM wParam,
                                            LPARAM lParam );

public:
	HWND		m_hDlg;

private:
    // Initialization functions
    void _Init( void );
    void _Uninit( void );

    // Member variables
    bool        m_bOleInit;
    HWND        m_hWrapperWnd;
    HWND        m_hBrowserWnd;
    HINSTANCE   m_hInstance;
    std::string m_strWebPage;
};

#endif // __BROWSER_WRAPPER_H__
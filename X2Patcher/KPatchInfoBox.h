/// @file   KPatchInfoBox.h
/// @since  2003-09-15 ¿ÀÈÄ 2:06:50
/// @author pch413@kogstudios.com

#ifndef __KPATCHINFOBOX_H__
#define __KPATCHINFOBOX_H__

#include <windows.h>
#include <tchar.h>
#include "KAssert.h"
#include "resource.h"
#include "KBrowserWrapper.h"

/// KPatckInfoBox : Display NetmarbleGrandChase Patch Information.
class KPatchInfoBox
{
private: // Disable construction of this class!
    /// Default constructor - disabled.
    KPatchInfoBox(void)
    { ASSERT(false && _T("Do not use default constructor!")); }
    /// Copy constructor - disabled.
    KPatchInfoBox(const KPatchInfoBox&)
    { ASSERT(false && _T("Do not use copy constructor!")); }
    /// Default destructor.
    ~KPatchInfoBox(void) {}
    /// Assignment operator - disabled.
    const KPatchInfoBox& operator=(const KPatchInfoBox&)
    { ASSERT(false && _T("Do not use assignment operator!")); return *this; }

public:
    /// Open patch info box.
    static INT_PTR Open(HWND hParentWnd = NULL);

private:
    /// Application-defined dialog box callback function.
    static BOOL CALLBACK PatchInfoBoxProc(HWND hDlg,
                                          UINT uMsg,
                                          WPARAM wParam,
                                          LPARAM lParam);

private:
    static KBrowserWrapper m_browser; ///< HTML file browser
};

#endif // __KPATCHINFOBOX_H__
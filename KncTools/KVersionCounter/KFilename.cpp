#include <assert.h>

#include "KFilename.h"

//---------------------------------------------------------------------------
KFilename::KFilename(const char* pcFullPath)
{
    m_acSubDir[0] = '\0';
    // Break down the file name here
    Splitpath(pcFullPath);
}
//---------------------------------------------------------------------------
void KFilename::GetFullPath(char* pcFullPath, unsigned int uiStrLen) const
{
    Makepath(pcFullPath, uiStrLen);
}
//---------------------------------------------------------------------------
// Wrapper/replacement for _splitpath. The char arrays should be allocated
// prior to calling this function.
// This function _does not_ handle multibyte characters.
void KFilename::Splitpath(const char* pcStr)
{
#if _MSC_VER >= 1400
    _splitpath_s(pcStr, m_acDrive, _MAX_DRIVE, m_acDir, _MAX_DIR, 
        m_acFname, _MAX_FNAME, m_acExt, _MAX_EXT);
#else //#if _MSC_VER >= 1400
    _splitpath(pcStr, m_acDrive, m_acDir, m_acFname, m_acExt);
#endif //#if _MSC_VER >= 1400
}

//---------------------------------------------------------------------------
// Wrapper/replacement for _makepath.  pcStr should already be allocated prior
// to calling this function
void KFilename::Makepath(char* pcStr, size_t uiStrLen) const
{
    // Deal with potential subdirectory by appending
    // Copy the subdirectory, if provided
    if (m_acSubDir[0] != '\0')
    {
        assert((strlen(m_acDir) + strlen(m_acSubDir)) < _MAX_DIR);
        char aTemp[_MAX_DIR];
        aTemp[0] = '\0';

        // Start with existing directory
        strncat(aTemp, m_acDir, _MAX_DIR);

        // Do we need to append a trailing directory separator?
        int iLen = (int) strlen(aTemp);

        if (iLen && aTemp[iLen-1] != '\\' && aTemp[iLen-1] != '/')
        {
            aTemp[iLen+1] = '\0';
            aTemp[iLen] = '\\'; // If so, append the slash
        }

        // Append subdirectory
        strncat(aTemp, m_acSubDir, _MAX_DIR);

#if _MSC_VER >= 1400
#else //#if _MSC_VER >= 1400
        _makepath(pcStr, m_acDrive, aTemp, m_acFname, m_acExt);
#endif //#if _MSC_VER >= 1400
    }
    else
    {
#if _MSC_VER >= 1400
#else //#if _MSC_VER >= 1400
        _makepath(pcStr, m_acDrive, m_acDir, m_acFname, m_acExt);
#endif //#if _MSC_VER >= 1400
    }
}
//---------------------------------------------------------------------------
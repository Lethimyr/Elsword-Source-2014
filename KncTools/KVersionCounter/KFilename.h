#pragma once

//#include "NiRTLib.h"
//#include "NiSystem.h"
//#include "NiSystemLibType.h"

#include <Windows.h>

class KFilename
{
public:
    KFilename(const char* pcFullPath);

    void SetDir(const char* pcDir);
    void SetDrive(const char* pcDrive);
    void SetExt(const char* pcExt);
    void SetFilename(const char* pcFname);
    void SetPlatformSubDir(const char* pcSubDir);

    const char* GetDir() const;
    const char* GetDrive() const;
    const char* GetExt() const;
    const char* GetFilename() const;
    const char* GetPlatformSubDir() const;

    void GetFullPath(char* pcFullPath, unsigned int uiStrLen) const;

private:
    void Splitpath(const char* pcStr);
    void Makepath(char* pcStr, size_t uiStrLen) const;

#ifdef WIN32
    char m_acDir[_MAX_DIR]; 
    char m_acDrive[_MAX_DRIVE]; 
    char m_acExt[_MAX_EXT];
    char m_acFname[_MAX_FNAME]; 
    char m_acSubDir[_MAX_DIR]; 
#else
    char m_acDir[256]; 
    char m_acDrive[3]; 
    char m_acExt[256];
    char m_acFname[256]; 
    char m_acSubDir[256]; 
#endif
};

//---------------------------------------------------------------------------
inline void KFilename::SetDir(const char* pcDir)
{
    strncpy(m_acDir, pcDir, _MAX_DIR);
}
//---------------------------------------------------------------------------
inline void KFilename::SetDrive(const char* pcDrive)
{
    strncpy(m_acDrive, pcDrive, _MAX_DRIVE);
}
//---------------------------------------------------------------------------
inline void KFilename::SetExt(const char* pcExt)
{
    strncpy(m_acExt, pcExt, _MAX_EXT);
}
//---------------------------------------------------------------------------
inline void KFilename::SetFilename(const char* pcFname)
{
    strncpy(m_acFname, pcFname, _MAX_FNAME);
}
//---------------------------------------------------------------------------
inline void KFilename::SetPlatformSubDir(const char* pcSubDir)
{
    strncpy(m_acSubDir, pcSubDir, _MAX_DIR);
}
//---------------------------------------------------------------------------
inline const char* KFilename::GetDir() const
{
    return m_acDir;
}
//---------------------------------------------------------------------------
inline const char* KFilename::GetDrive() const
{
    return m_acDrive;
}
//---------------------------------------------------------------------------
inline const char* KFilename::GetExt() const
{
    return m_acExt;
}
//---------------------------------------------------------------------------
inline const char* KFilename::GetFilename() const
{
    return m_acFname;
}
//---------------------------------------------------------------------------
inline const char* KFilename::GetPlatformSubDir() const
{
    return m_acSubDir;
}
//---------------------------------------------------------------------------
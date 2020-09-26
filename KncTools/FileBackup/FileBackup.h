#pragma once

#include <set>
#include <string>
#include "KLogManager.h"

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

class KFileBackup
{
public:
    KFileBackup(void);
    virtual ~KFileBackup(void);

    bool Init();
    bool ParseCmdParam( const int argc, const char** argv );

    bool Backup();

protected:
    inline bool SetSkipExt( IN const char* szExt );
    inline bool MakeFolder( const char* szPath );
    inline bool ShouldSkip( const char* szPath, const char* szFileName );
    inline void PrintHelp();
    inline void ClearSkipExt() { m_setSkipExt.clear(); }

protected:
    std::set<std::string>   m_setSkipExt;
    std::string             m_strMyName;
    std::string             m_strSourceDir;
    std::string             m_strDestDir;
};

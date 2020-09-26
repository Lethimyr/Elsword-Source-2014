#pragma once


#include <vector>
#include "StringEx.h"

#include "IElement.h"
class CParser;
class CZipper;
class CMailSender;
class CMinFTP;

#define ARGC_COUNT 7

class CAutoManager
{
public:
    typedef std::vector< IElement* >  VECELEMENT;
    typedef VECELEMENT::iterator    ITVECELEMENT;

public:
    CAutoManager( void );
    ~CAutoManager(void);
    bool Process( int argc, char* argv[]);

private:

    bool _LoadParam( int argc, char* argv[]);
    bool _ValidChk();
    bool _WaitFileCompete( int nTimeOut );
    bool _Zipping();
    bool _UploadToFTP();
    void _SendMail();
    void _CleanUp();

private:
    CStringEx           m_strINIFile;
    
    CStringEx           m_strZipedFileName;
    VECELEMENT          m_vecFileElement;
    VECELEMENT          m_vecMailElement;

    CElementFTPInfo     m_elFTPElement;
    CElementCrashInfo   m_elServerName;
        
};

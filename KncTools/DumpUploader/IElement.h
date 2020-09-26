#pragma once

#include "StringEx.h"
class IElement
{
public:
    enum ELEMENTTYPE
    {
        ELEMENTTYPE_NONE,
        ELEMENTTYPE_FILEINFO,
        ELEMENTTYPE_MAILINFO,
        ELEMENTTYPE_FTPINFO,
        ELEMENTTYPE_CRASH_SERVERINFO,
        ELEMENTTYPE_MAX,
    };//end ELEMENTTYPE

public:
    IElement(void);
    virtual ~IElement(void);
    ELEMENTTYPE GetType();

    
    bool IsValid();
    virtual void ValidChk() = 0;
protected:
    void _SetValid( bool bValid );
    void _SetType( ELEMENTTYPE eType );

private:
    bool m_bValid;
    ELEMENTTYPE m_eType;
};//class IElement

//------------------------------------------------------------------------------------------------//


class CElementFileInfo : public IElement
{
public:
    CElementFileInfo();
    ~CElementFileInfo();

    virtual void ValidChk();
    void SetFileName( char* strFileName );
    char* GetFileName();

    bool GetUserble(); // 파일을 사용해도 되느냐?
    bool ChkUserble();
private:
    bool m_bUsable;
    CStringEx m_strFileName;
    
};//class CElementFileInfo


//------------------------------------------------------------------------------------------------//

class CElementMailInfo : public IElement
{
public:
    CElementMailInfo();
    ~CElementMailInfo();
    
    char* GetMailAddress();
    char* GetSMTPAddress();

    void SetMailAddress( char* strMailAddress );
    void SetSMTPAddress( char* strSMTPAddress );
    
    virtual void ValidChk();
private:
    CStringEx m_strMailAddress;
    CStringEx m_strSTMPAddress;
};//class CElementMailInfo
//------------------------------------------------------------------------------------------------//


class CElementFTPInfo : public IElement
{
public:
    CElementFTPInfo();
    ~CElementFTPInfo();

    char* GetFTPAddress();
    char* GetFTPPort();
    char* GetUserID();
    char* GetUserPW();

    void SetFTPAddress( char* strFTPAddress );
    void SetFTPPort( char* strFTPPort );
    void SetUserID( char* strUserID );
    void SetUserPW( char* strUserPW );

    virtual void ValidChk();
private:
    CStringEx m_strFTPAddress;
    CStringEx m_strFTPPort;
    CStringEx m_strUserID;
    CStringEx m_strUserPW;

};//class CElementFTPInfo

//------------------------------------------------------------------------------------------------//

class CElementCrashInfo : public IElement
{
public:
    CElementCrashInfo();
    ~CElementCrashInfo();

    char* GetCrashInfo();

    void SetCrashInfo( char* strCrashServer );
    virtual void ValidChk();
private:
    CStringEx m_strCrashServer;
};



//------------------------------------------------------------------------------------------------//
namespace ElementFactory
{
    IElement* GetElement( IElement::ELEMENTTYPE eType );
};
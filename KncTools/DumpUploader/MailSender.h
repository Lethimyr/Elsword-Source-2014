#pragma once

#include "MailMIMEHeader.h"
#include "MailSMTPHeader.h"
#include "IElement.h"

class CMailSender
{
public:
    CMailSender(void);
    ~CMailSender(void);

    
    bool SendMail( CElementMailInfo* pMailInfo, char* strServerName );
    static void _fnEnd(BOOL bSuccess);
private:
    void _Init();
    static LRESULT CALLBACK MsgProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );    
    unsigned long   m_ulSMTPAddr;
};

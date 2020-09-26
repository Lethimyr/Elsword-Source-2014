#include "StdAfx.h"
#include ".\mailsender.h"


#include "StringEx.h"
#include "Logger.h"
extern CLogger* g_Logger;
extern CWinApp theApp;
HWND g_hWnd;

CMailSender::CMailSender(void)
{
}

CMailSender::~CMailSender(void)
{

}
_Mail    m_sMail;
bool CMailSender::SendMail( CElementMailInfo* pMailInfo, char* strServerName )
{
    _Init(); // 구조체 비움
    ZeroMemory( &m_sMail, sizeof(m_sMail) );
    
    CStringEx strSenderMail = "kkurrung@kogstudios.com";
    CStringEx strFakeText = g_Logger->GetLogString();    
    m_ulSMTPAddr =SMTP_TransAddr(pMailInfo->GetSMTPAddress());

    m_sMail.from = strSenderMail.c_str();
    m_sMail.to = pMailInfo->GetMailAddress();

    m_sMail.cc			=NULL;
    m_sMail.bcc		=NULL;
    m_sMail.dataHead1	=MIME_MakeHead1(m_sMail.to, m_sMail.cc);
    m_sMail.dataHead2	=MIME_MakeHead2(strServerName, m_sMail.from, strServerName, 0);
      
    m_sMail.dataBody	=MIME_MakeBody(strFakeText.c_str(), 0);
    m_sMail.dataEnd     = MIME_GetEnd();

    LPSTR lpszClass="First";

    MSG Message;
    WNDCLASS WndClass;
    WndClass.cbClsExtra=0;
    WndClass.cbWndExtra=0;
    WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
    WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
    WndClass.hInstance=theApp.m_hInstance;
    WndClass.lpfnWndProc=(WNDPROC)CMailSender::MsgProc;
    WndClass.lpszClassName=lpszClass;
    WndClass.lpszMenuName=NULL;
    WndClass.style=CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);

    g_hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
        NULL,(HMENU)NULL,theApp.m_hInstance,NULL);
    // CreateWindow함수가 호출될때 메세지 프로시져의 create가 호출된다.
    // 그러니까.. CreateWindow이후에는 메세지 루프를 사용해도 된다는것이군..
    // 여기서 센딩해 보자..

    if (!SMTP_SendMail(g_hWnd, m_ulSMTPAddr, &m_sMail, _fnEnd, NULL)){
        //실패
        MIME_Free(m_sMail.dataHead1);
        MIME_Free(m_sMail.dataHead1);
        MIME_Free(m_sMail.dataBody);
        MIME_Free(m_sMail.dataAttach);
        TRACE("%s",SMTP_GetLastMsg());	//실패의 원인보여주기
        ::SendMessage( g_hWnd, WM_CLOSE,0,0 );
    }

    ::ShowWindow(g_hWnd,1);

    while(GetMessage(&Message,0,0,0)) 
    {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }//end while

    
    return (Message.wParam == 0);
}

void CMailSender::_Init()
{
    
    m_ulSMTPAddr = 0;
}



void CMailSender::_fnEnd(BOOL bSuccess)
{
    MIME_Free(m_sMail.dataHead1);
    MIME_Free(m_sMail.dataHead1);
    MIME_Free(m_sMail.dataBody);
    MIME_Free(m_sMail.dataAttach);
    ::SendMessage( g_hWnd, WM_CLOSE,0,0 );
       
    
}

LRESULT CALLBACK CMailSender::MsgProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
    switch(iMessage) 
    {
    

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
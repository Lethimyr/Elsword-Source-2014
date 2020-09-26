#pragma once



#include <map>
#include <vector>

#include "MailSMTPHeader.h"
#include "MailMIMEHeader.h"

// oasis907 : 김상윤 [2009.12.21] // CX2SMTPMail 기능 확장(이메일 계정 인증 위해), GC의 CSmtp를 사용
// 기존의 CX2SMTPMail 기능 그대로
// 기존의 것과 구별 위해, 새로운 메소드 이름 앞에 SMTP 붙임  ex> SMTPSend();

// CSmtp는 MailMIME.lib, MailSMTP.lib를 사용하지 않고 직접 구현


//{{ oasis907 : 김상윤 [2009.12.21] // 새 CSmtp
#include "base64.h"
#include <iostream>
#include <tchar.h>

#define BUFFER_SIZE 1024 * 1024 //20480	  // SendData and RecvData buffers sizes
#define DELAY_IN_MS 10			// delay between send and recv functions
#define MSG_SIZE_IN_MB 5		// the maximum size of the message with all attachments

const char BOUNDARY_TEXT[] = "__MESSAGE__ID__54yg6f6h6y456345";
//}} oasis907 : 김상윤 [2009.12.21] // 새 CSmtp


extern _Mail g_SendMailData;
extern HWND g_hSendMailWnd;


class CX2SMTPMail
{
//{{ oasis907 : 김상윤 [2009.12.21] // 새 CSmtp
public:
	enum CSmtpError
	{
		CSMTP_NO_ERROR = 0,
		CSMTP_WSA_STARTUP = 100, // WSAGetLastError()
		CSMTP_WSA_VER,
		CSMTP_WSA_SEND,
		CSMTP_WSA_RECV,
		CSMTP_WSA_CONNECT,
		CSMTP_WSA_GETHOSTBY_NAME_ADDR,
		CSMTP_WSA_INVALID_SOCKET,
		CSMTP_WSA_HOSTNAME,
		CSMTP_BAD_IPV4_ADDR,
		CSMTP_UNDEF_MSG_HEADER = 200,
		CSMTP_UNDEF_MAILFROM,
		CSMTP_UNDEF_SUBJECT,
		CSMTP_UNDEF_RECIPENTS,
		CSMTP_UNDEF_LOGIN,
		CSMTP_UNDEF_PASSWORD,
		CSMTP_UNDEF_RECIPENT_MAIL,
		CSMTP_COMMAND_MAIL_FROM = 300,
		CSMTP_COMMAND_EHLO,
		CSMTP_COMMAND_AUTH_LOGIN,
		CSMTP_COMMAND_DATA,
		CSMTP_COMMAND_QUIT,
		CSMTP_COMMAND_RCPT_TO,
		CSMTP_MSG_BODY_ERROR,
		CSMTP_CONNECTION_CLOSED = 400, // by server
		CSMTP_SERVER_NOT_READY, // remote server
		CSMTP_FILE_NOT_EXIST,
		CSMTP_MSG_TOO_BIG,
		CSMTP_BAD_LOGIN_PASS,
		CSMTP_UNDEF_XYZ_RESPOMSE,
		CSMTP_LACK_OF_MEMORY
	};

	enum CSmptXPriority
	{
		XPRIORITY_HIGH = 2,
		XPRIORITY_NORMAL = 3,
		XPRIORITY_LOW = 4
	};
//}} oasis907 : 김상윤 [2009.12.21] // 새 CSmtp


	CX2SMTPMail(void);
	~CX2SMTPMail(void);


	const std::string& GetSMTPAddress() const { return m_strSMTPAddress; }
#ifdef CHANGE_MAILSTRING
	void SetSMTPAddress( const char* val) { m_strSMTPAddress = val; }
#else
	void SetSMTPAddress( char* val) { m_strSMTPAddress = val; }
#endif
	const std::string& GetSenderName() const { return m_strSenderName; }
	void SetSenderName(char* val) { m_strSenderName = val; }
	const std::string& GetSenderEMail() const { return m_strSenderEMail; }
	void SetSenderEMail(char* val) { m_strSenderEMail = val; }
	const std::string& GetReceiverEMail() const { return m_strReceiverEMail; }
	void SetReceiverEMail(char* val) { m_strReceiverEMail = val; }
	const std::string& GetSubject() const { return m_strSubject; }
	void SetSubject(char* val) { m_strSubject = val; }
	const std::string& GetDesc() const { return m_strDesc; }
	void SetDesc(char* val) { m_strDesc = val; }


	void AddFile( std::string strFileName );
	void ClearFile() { m_vecFiles.clear(); }
	bool SendMail( HWND hwnd );
	
	static void _fnEnd(BOOL bSuccess) // 메일 보내는게 성공하면 이 함수가 호출된다
	{
		MIME_Free( g_SendMailData.dataHead1 );
		MIME_Free( g_SendMailData.dataHead2 );
		MIME_Free( g_SendMailData.dataBody );
		MIME_Free( g_SendMailData.dataAttach );

		EndDialog( g_hSendMailWnd, 0 );
	};
	

	// szPath는 "C:\ABC" 
	// szExt는 ".lua"
	void FindFilesByExt( std::vector<std::string>& vecFilePath, const char* szPath, const char* szExt );

private:
	std::string	m_strSMTPAddress;
	std::string	m_strSenderName;
	std::string	m_strSenderEMail;
	std::string	m_strReceiverEMail;
	std::string	m_strSubject;
	std::string	m_strDesc;
	std::vector< std::string > m_vecFiles;



//{{ oasis907 : 김상윤 [2009.12.21] // 새 CSmtp
	//////////////////////////////////////////////////////////////////////////////////////
public:
	bool SMTPAddRecipient(const char *email, const char *name=NULL);
	bool SMTPAddBCCRecipient(const char *email, const char *name=NULL);
	bool SMTPAddCCRecipient(const char *email, const char *name=NULL);    

	void SMTPClearRecipient();


	bool SMTPAddAttachment(const char *path);   
	const unsigned int SMTPGetBCCRecipientCount();    
	const unsigned int SMTPGetCCRecipientCount();
	const unsigned int SMTPGetRecipientCount();    
	const char* const SMTPGetLocalHostIP();
	const char* const SMTPGetLocalHostName();    
	const char* const SMTPGetMessageBody();    
	const char* const SMTPGetReplyTo();
	const char* const SMTPGetMailFrom();
	const char* const SMTPGetSenderName();
	const char* const SMTPGetSubject();    
	const char* const SMTPGetXMailer();
	CSmptXPriority SMTPGetXPriority();
	CSmtpError SMTPGetLastError();
	bool SMTPSend();
	void SMTPSetMessageBody(const char*);
	void SMTPSetSubject(const char*);
	void SMTPSetSenderName(const char*);
	void SMTPSetSenderMail(const char*);
	void SMTPSetReplyTo(const char*);
	void SMTPSetXMailer(const char*);
	void SMTPSetLogin(const char*);
	void SMTPSetPassword(const char*);
	void SMTPSetXPriority(CSmptXPriority);
	void SMTPSetServer(const char* server,const unsigned short port=0);

private:	
	CSmtpError m_oError;
	char* m_pcLocalHostName;
	char* m_pcMailFrom;
	char* m_pcNameFrom;
	char* m_pcSubject;
	char* m_pcMsgBody;
	char* m_pcXMailer;
	char* m_pcReplyTo;
	char* m_pcIPAddr;
	char* m_pcLogin;
	char* m_pcPassword;
	char* m_pcSMTPSrvName;
	unsigned short m_iSMTPSrvPort;
	CSmptXPriority m_iXPriority;
	char *SendBuf;
	char *RecvBuf;



	WSADATA wsaData;
	SOCKET hSocket;

	struct Recipent
	{
		std::string Name;
		std::string Mail;
	};

	std::vector<Recipent> Recipients;
	std::vector<Recipent> CCRecipients;
	std::vector<Recipent> BCCRecipients;
	std::vector<std::string> Attachments;

	bool SMTPReceiveData();
	bool SMTPSendData();
	bool SMTPFormatHeader(char*);
	int SMTPXYZdigits();
	SOCKET SMTPConnectRemoteServer(const char* server, const unsigned short port=NULL);

	friend char* GetErrorText(CSmtpError);
//}} oasis907 : 김상윤 [2009.12.21] // 새 CSmtp


};




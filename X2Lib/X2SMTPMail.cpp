#include "StdAfx.h"
#include ".\x2smtpmail.h"



// note: wchar 안쓰고 char 씀. mail 보내는 library가 char를 씀



// 예제코드
//CX2SMTPMail* g_pSMTPMail = new CX2SMTPMail;
//g_pSMTPMail->SetReceiverEMail( "ID@email.com" );
//g_pSMTPMail->SetSenderName( "동민" );
//g_pSMTPMail->SetSubject( "13시" );
//g_pSMTPMail->SetDesc( "테스트2" );
//g_pSMTPMail->ClearFile();
//g_pSMTPMail->AddFile( "C:\\temp_line_map_lua.lua" );
//std::vector< std::string > vecFiles;
//g_pSMTPMail->FindFilesByExt( vecFiles, "C:\\Nexon\\ElSword\\Data\\GameGuard", ".erl" );
//for( UINT i=0; i<vecFiles.size(); i++ )
//{
//	g_pSMTPMail->AddFile( vecFiles[i].c_str());
//}
//g_pSMTPMail->SendMail( hWnd );




using std::wstring;
using std::string;

_Mail g_SendMailData;  // static function에서 이 변수를 사용한다
HWND g_hSendMailWnd;

//{{ oasis907 : 김상윤 [2009.12.21] // 새 CSmtp
#pragma warning(push)
#pragma warning(disable:4786)
#pragma warning(disable:4288)
#pragma warning(disable:4018)
//}} oasis907 : 김상윤 [2009.12.21] // 새 CSmtp

CX2SMTPMail::CX2SMTPMail(void)
{
	m_strSMTPAddress		= "mail.kog.co.kr";
	m_strSenderName			= "mailer";
	m_strSenderEMail		= "demiahun@kog.co.kr";
	m_strReceiverEMail 		= "demiahun@kog.co.kr";
	m_strSubject			= "subject";
	m_strDesc				= "desc";


	
//{{ oasis907 : 김상윤 [2009.12.21] // 새 CSmtp
	// Initialize variables
	m_oError = CSMTP_NO_ERROR;
	m_iXPriority = XPRIORITY_NORMAL;
	m_iSMTPSrvPort = 0;

	m_pcLocalHostName = NULL;
	m_pcMailFrom = NULL;
	m_pcNameFrom = NULL;
	m_pcSubject = NULL;
	m_pcMsgBody = NULL;
	m_pcXMailer = NULL;
	m_pcReplyTo = NULL;
	m_pcIPAddr = NULL;
	m_pcLogin = NULL;
	m_pcPassword = NULL;
	m_pcSMTPSrvName = NULL;


	hSocket = NULL;

	if((RecvBuf = new char[BUFFER_SIZE]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return;
	}

	if((SendBuf = new char[BUFFER_SIZE]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return;
	}

	// Initialize WinSock
	WORD wVer = MAKEWORD(2,2);    
	if (WSAStartup(wVer,&wsaData) != NO_ERROR)
	{
		m_oError = CSMTP_WSA_STARTUP;
		return;
	}
	if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		m_oError = CSMTP_WSA_VER;
		WSACleanup();
		return;
	}
//}} oasis907 : 김상윤 [2009.12.21] // 새 CSmtp
}



CX2SMTPMail::~CX2SMTPMail(void)
{
//{{ oasis907 : 김상윤 [2009.12.21] / 새 CSmtp

	// Clear vectors
	Recipients.clear();
	CCRecipients.clear();
	BCCRecipients.clear();
	Attachments.clear();

	// Free memory
	if (m_pcLocalHostName)
	{
		delete[] m_pcLocalHostName;
		m_pcLocalHostName = NULL;
	}
	if (m_pcMailFrom)
	{
		delete[] m_pcMailFrom;
		m_pcMailFrom = NULL;
	}
	if (m_pcNameFrom)
	{
		delete[] m_pcNameFrom;
		m_pcNameFrom = NULL;
	}
	if (m_pcSubject)
	{
		delete[] m_pcSubject;
		m_pcSubject = NULL;
	}
	if (m_pcMsgBody)
	{
		delete[] m_pcMsgBody;
		m_pcMsgBody = NULL;
	}
	if (m_pcXMailer)
	{
		delete[] m_pcXMailer;
		m_pcXMailer = NULL;
	}
	if (m_pcReplyTo)
	{
		delete[] m_pcReplyTo;
		m_pcReplyTo = NULL;
	}
	if (m_pcIPAddr)
	{
		delete[] m_pcIPAddr;
		m_pcIPAddr = NULL;
	}
	if (m_pcLogin)
	{
		delete[] m_pcLogin;
		m_pcLogin = NULL;
	}
	if (m_pcPassword)
	{
		delete[] m_pcPassword;
		m_pcPassword = NULL;
	}
	if (m_pcSMTPSrvName)
	{
		delete[] m_pcSMTPSrvName;
		m_pcSMTPSrvName = NULL;
	}
	if(SendBuf)
	{
		delete[] SendBuf;
		SendBuf = NULL;
	}
	if(RecvBuf)
	{
		delete[] RecvBuf;
		RecvBuf = NULL;
	}

	// Cleanup
	WSACleanup();
//}} oasis907 : 김상윤 [2009.12.21] // 새 CSmtp
}


void CX2SMTPMail::AddFile( std::string strFileName )
{
	// make sure the file exist
	HANDLE hFile = ::CreateFileA(
		strFileName.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if( hFile != INVALID_HANDLE_VALUE )
	{
		// add file to report
		m_vecFiles.push_back( strFileName );
		::CloseHandle(hFile);
	}
}


bool CX2SMTPMail::SendMail( HWND hWnd )
{
	if( m_strSMTPAddress.length() == 0 )
		return false;

	if( m_strReceiverEMail.length() == 0 )
		return false;

	
	unsigned long ulSMTPAddr = SMTP_TransAddr( (char*) m_strSMTPAddress.c_str() );

	// 메일데이터를 만들자~~
	g_SendMailData.from			= (char*) m_strSenderEMail.c_str();
	g_SendMailData.to			= (char*) m_strReceiverEMail.c_str();
	g_SendMailData.cc			= NULL;
	g_SendMailData.bcc			= NULL;
	g_SendMailData.dataHead1	= MIME_MakeHead1( g_SendMailData.to, g_SendMailData.cc );
	g_SendMailData.dataHead2	= MIME_MakeHead2( (char*) m_strSenderName.c_str(), g_SendMailData.from, (char*) m_strSubject.c_str(), 1 );
	g_SendMailData.dataBody		= MIME_MakeBody( (char*) m_strDesc.c_str(), 0 );


	if( m_vecFiles.size() > 0 )
	{
		char** ppFiles = NULL;
		int cFiles = 0;
		ppFiles = (char**) malloc( sizeof(char*) * (int)m_vecFiles.size() );
		for( UINT i=0; i<m_vecFiles.size(); i++ )
		{
			ppFiles[i] = (char*) m_vecFiles[i].c_str();
			cFiles++;
		}

		g_SendMailData.dataAttach = MIME_MakeAttach( ppFiles, cFiles );
	}
	else
	{
		g_SendMailData.dataAttach = NULL;
	}
	g_SendMailData.dataEnd		= MIME_GetEnd();


	g_hSendMailWnd = hWnd;
	
	// 데이타 다 만들고 이제 보낸다.
	bool bSuccess = false;
	if( 0 != SMTP_SendMail( hWnd, ulSMTPAddr, &g_SendMailData, _fnEnd, NULL) ) // 비동기함수인듯, 
	{
		// 성공
		bSuccess = true;

		// 나머지처리는 _fnEnd에서,
	}
	else
	{
		// 실패
		bSuccess = false;
		//MessageBox( SMTP_GetLastMsg());	//실패의 원인보여주기

		MIME_Free( g_SendMailData.dataHead1 );
		MIME_Free( g_SendMailData.dataHead2 );
		MIME_Free( g_SendMailData.dataBody );
		MIME_Free( g_SendMailData.dataAttach );

		EndDialog( g_hSendMailWnd, 0 );
	}

	return bSuccess;
}


void CX2SMTPMail::FindFilesByExt( std::vector<std::string>& vecFilePath, const char* szPath, const char* szExt )
{
	if( szPath == NULL )
		return;

	if( szExt == NULL )
		return;

	//_chdir( szPath );

	HANDLE				hSearch;
	WIN32_FIND_DATAA	fd;

	//WCHAR drive[_MAX_DRIVE]		= L"";
	//WCHAR dir[_MAX_DIR]			= L"";	 
	char fname[_MAX_FNAME]		= "";
	char ext[_MAX_EXT]			= "";

	string strExt = szExt;
	MakeUpperCase( strExt );

	string strRootDir = szPath;
	strRootDir += "\\";
	
	string strFindPath = szPath;
	strFindPath += "\\*";

	hSearch = FindFirstFileA( strFindPath.c_str(), &fd );
	if( hSearch == INVALID_HANDLE_VALUE )
		return;

	do
	{
		if( 0 != strcmp(fd.cFileName, ".") && 0 != strcmp(fd.cFileName, "..") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{	
			}
			else
			{
				_splitpath( fd.cFileName, NULL, NULL, fname, ext );
				MakeUpperCase( ext );

				if( 0 == strExt.compare( ext ) )
				{
					string strSrcFileName = strRootDir;
					strSrcFileName += fd.cFileName;
					vecFilePath.push_back( strSrcFileName );
				}			
			}			
		}
	} while( FindNextFileA( hSearch, &fd ) );

	FindClose(hSearch);
}


//{{ oasis907 : 김상윤 [2009.12.21] // 새 CSmtp

bool CX2SMTPMail::SMTPAddAttachment(const char *path)
{
	std::string str(path);
	Attachments.insert(Attachments.end(),str);
	return true;
}

bool CX2SMTPMail::SMTPAddRecipient(const char *email, const char *name)
{
	assert(email);

	if(!email)
	{
		m_oError = CSMTP_UNDEF_RECIPENT_MAIL;
		return false;
	}

	Recipent recipent;
	recipent.Mail.insert(0,email);
	name!=NULL ? recipent.Name.insert(0,name) : recipent.Name.insert(0,"");

	Recipients.insert(Recipients.end(), recipent);

	return true;    
}

bool CX2SMTPMail::SMTPAddCCRecipient(const char *email, const char *name)
{
	assert(email);

	if(!email)
	{
		m_oError = CSMTP_UNDEF_RECIPENT_MAIL;
		return false;
	}

	Recipent recipent;
	recipent.Mail.insert(0,email);
	name!=NULL ? recipent.Name.insert(0,name) : recipent.Name.insert(0,"");

	CCRecipients.insert(CCRecipients.end(), recipent);

	return true;
}

bool CX2SMTPMail::SMTPAddBCCRecipient(const char *email, const char *name)
{
	assert(email);

	if(!email)
	{
		m_oError = CSMTP_UNDEF_RECIPENT_MAIL;
		return false;
	}

	Recipent recipent;
	recipent.Mail.insert(0,email);
	name!=NULL ? recipent.Name.insert(0,name) : recipent.Name.insert(0,"");

	BCCRecipients.insert(BCCRecipients.end(), recipent);

	return true;
}


void CX2SMTPMail::SMTPClearRecipient()
{
	Recipients.clear();
	CCRecipients.clear();
	BCCRecipients.clear();
	Attachments.clear();
	return;
}

bool CX2SMTPMail::SMTPSend()
{
	unsigned int i,rcpt_count,res,FileId;
	char *FileBuf = NULL, *FileName = NULL;
	FILE* hFile = NULL;
	unsigned long int FileSize,TotalSize,MsgPart;

	// ***** CONNECTING TO SMTP SERVER *****

	assert(m_pcSMTPSrvName);

	// connecting to remote host:
	if( (hSocket = SMTPConnectRemoteServer(m_pcSMTPSrvName, m_iSMTPSrvPort)) == INVALID_SOCKET ) 
	{
		m_oError = CSMTP_WSA_INVALID_SOCKET;
		return false;
	}
	Sleep(DELAY_IN_MS);
	if(!SMTPReceiveData())
		return false;

	switch(SMTPXYZdigits())
	{
	case 220:
		break;
	default:
		m_oError = CSMTP_SERVER_NOT_READY;
		return false;
	}

	// EHLO <SP> <domain> <CRLF>
	StringCchPrintfA(SendBuf, BUFFER_SIZE ,"EHLO %s\r\n","domain");
	//sprintf(SendBuf,"EHLO %s\r\n","domain");
	if(!SMTPSendData())
		return false;
	Sleep(DELAY_IN_MS);
	if(!SMTPReceiveData())
		return false;

	switch(SMTPXYZdigits())
	{
	case 250:
		break;
	default:
		m_oError = CSMTP_COMMAND_EHLO;
		return false;
	}

	// AUTH <SP> LOGIN <CRLF>
	strncpy(SendBuf,"AUTH LOGIN\r\n", BUFFER_SIZE - 1 );
	//strcpy(SendBuf,"AUTH LOGIN\r\n");
	if(!SMTPSendData())
		return false;
	Sleep(DELAY_IN_MS);
	if(!SMTPReceiveData())
		return false;

	switch(SMTPXYZdigits())
	{
	case 334:
		break;
	default:
		m_oError = CSMTP_COMMAND_AUTH_LOGIN;
		return false;
	}

	// send login:
	if(!m_pcLogin)
	{
		m_oError = CSMTP_UNDEF_LOGIN;
		return false;
	}
	std::string encoded_login = base64_encode(reinterpret_cast<const unsigned char*>(m_pcLogin),strlen(m_pcLogin));
	StringCchPrintfA(SendBuf, BUFFER_SIZE, "%s\r\n",encoded_login.c_str());
	//sprintf(SendBuf,"%s\r\n",encoded_login.c_str());
	if(!SMTPSendData())
		return false;
	Sleep(DELAY_IN_MS);
	if(!SMTPReceiveData())
		return false;

	switch(SMTPXYZdigits())
	{
	case 334:
		break;
	default:
		m_oError = CSMTP_UNDEF_XYZ_RESPOMSE;
		return false;
	}

	// send password:
	if(!m_pcPassword)
	{
		m_oError = CSMTP_UNDEF_PASSWORD;
		return false;
	}
	std::string encoded_password = base64_encode(reinterpret_cast<const unsigned char*>(m_pcPassword),strlen(m_pcPassword));
	StringCchPrintfA(SendBuf, BUFFER_SIZE ,"%s\r\n",encoded_password.c_str());
	//sprintf(SendBuf,"%s\r\n",encoded_password.c_str());
	if(!SMTPSendData())
		return false;
	Sleep(DELAY_IN_MS);
	if(!SMTPReceiveData())
		return false;

	switch(SMTPXYZdigits())
	{
	case 235:
		break;
	case 535:
		m_oError = CSMTP_BAD_LOGIN_PASS;
		return false;
	default:
		m_oError = CSMTP_UNDEF_XYZ_RESPOMSE;
		return false;
	}

	// ***** SENDING E-MAIL *****

	// MAIL <SP> FROM:<reverse-path> <CRLF>
	if(m_pcMailFrom == NULL)
	{
		m_oError = CSMTP_UNDEF_MAILFROM;
		return false;
	}
	StringCchPrintfA(SendBuf, BUFFER_SIZE-1 , "MAIL FROM:<%s>\r\n",m_pcMailFrom);
	//sprintf(SendBuf,"MAIL FROM:<%s>\r\n",m_pcMailFrom);
	if(!SMTPSendData())
		return false;
	Sleep(DELAY_IN_MS);
	if(!SMTPReceiveData())
		return false;

	switch(SMTPXYZdigits())
	{
	case 250:
		break;
	default:
		m_oError = CSMTP_COMMAND_MAIL_FROM;
		return false;
	}

	// RCPT <SP> TO:<forward-path> <CRLF>
	rcpt_count = Recipients.size();
	for(i=0;i<Recipients.size();i++)
	{
		StringCchPrintfA(SendBuf, BUFFER_SIZE, "RCPT TO:<%s>\r\n",(Recipients.at(i).Mail).c_str());
		//sprintf(SendBuf,"RCPT TO:<%s>\r\n",(Recipients.at(i).Mail).c_str());
		if(!SMTPSendData())
			return false;
		Sleep(DELAY_IN_MS);
		if(!SMTPReceiveData())
			return false;

		switch(SMTPXYZdigits())
		{
		case 250:
			break;
		default:
			m_oError = CSMTP_COMMAND_RCPT_TO;
			rcpt_count--;
		}
	}
	if(!rcpt_count)
		return false;
	for(i=0;i<CCRecipients.size();i++)
	{
		StringCchPrintfA(SendBuf, BUFFER_SIZE , "RCPT TO:<%s>\r\n",(CCRecipients.at(i).Mail).c_str());
		//sprintf(SendBuf,"RCPT TO:<%s>\r\n",(CCRecipients.at(i).Mail).c_str());
		if(!SMTPSendData())
			return false;
		Sleep(DELAY_IN_MS);
		if(!SMTPReceiveData())
			return false;
	}
	for(i=0;i<BCCRecipients.size();i++)
	{
		StringCchPrintfA(SendBuf, BUFFER_SIZE, "RCPT TO:<%s>\r\n",(BCCRecipients.at(i).Mail).c_str());
		//sprintf(SendBuf,"RCPT TO:<%s>\r\n",(BCCRecipients.at(i).Mail).c_str());
		if(!SMTPSendData())
			return false;
		Sleep(DELAY_IN_MS);
		if(!SMTPReceiveData())
			return false;
	}

	// DATA <CRLF>
	strncpy(SendBuf,"DATA\r\n" , BUFFER_SIZE - 1 );
	//strcpy(SendBuf,"DATA\r\n");
	if(!SMTPSendData())
		return false;
	Sleep(DELAY_IN_MS);
	if(!SMTPReceiveData())
		return false;

	switch(SMTPXYZdigits())
	{
	case 354:
		break;
	default:
		m_oError = CSMTP_COMMAND_DATA;
		return false;
	}
	/*
	strcpy(SendBuf,"\r\n.\r\n");
	if(!SendData())
	return false;

	Sleep(DELAY_IN_MS);
	if(!ReceiveData())
	return false;
	*/


	// send header(s)
	if(!SMTPFormatHeader(SendBuf))
	{
		m_oError = CSMTP_UNDEF_MSG_HEADER;
		return false;
	}
	if(!SMTPSendData())
		return false;

	// send text message
	StringCchPrintfA(SendBuf, BUFFER_SIZE, "%s\r\n",m_pcMsgBody); // NOTICE: each line ends with <CRLF>
	//sprintf(SendBuf,"%s\r\n",m_pcMsgBody); // NOTICE: each line ends with <CRLF>
	if(!SMTPSendData())
		return false;

	// next goes attachments (if they are)
	if((FileBuf = new char[55]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return false;
	}
	if((FileName = new char[255]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return false;
	}
	TotalSize = 0;
	for(FileId=0;FileId<Attachments.size();FileId++)
	{
		strncpy(FileName,Attachments[FileId].c_str() , 254 );
		//strcpy(FileName,Attachments[FileId].c_str());

		StringCchPrintfA(SendBuf, BUFFER_SIZE, "--%s\r\n",BOUNDARY_TEXT);
		//sprintf(SendBuf,"--%s\r\n",BOUNDARY_TEXT);

		StringCchCatA(SendBuf, BUFFER_SIZE,"Content-Type: application/x-msdownload; name=\"");
		StringCchCatA(SendBuf,BUFFER_SIZE,&FileName[Attachments[FileId].find_last_of("\\") + 1]);
		StringCchCatA(SendBuf,BUFFER_SIZE,"\"\r\n");
		StringCchCatA(SendBuf,BUFFER_SIZE,"Content-Transfer-Encoding: base64\r\n");
		StringCchCatA(SendBuf,BUFFER_SIZE,"Content-Disposition: attachment; filename=\"");
		StringCchCatA(SendBuf,BUFFER_SIZE,&FileName[Attachments[FileId].find_last_of("\\") + 1]);
		StringCchCatA(SendBuf,BUFFER_SIZE,"\"\r\n");
		StringCchCatA(SendBuf,BUFFER_SIZE,"\r\n");

		/*strcat(SendBuf,"Content-Type: application/x-msdownload; name=\"");
		strcat(SendBuf,&FileName[Attachments[FileId].find_last_of("\\") + 1]);
		strcat(SendBuf,"\"\r\n");
		strcat(SendBuf,"Content-Transfer-Encoding: base64\r\n");
		strcat(SendBuf,"Content-Disposition: attachment; filename=\"");
		strcat(SendBuf,&FileName[Attachments[FileId].find_last_of("\\") + 1]);
		strcat(SendBuf,"\"\r\n");
		strcat(SendBuf,"\r\n");*/

		if(!SMTPSendData())
			return false;

		// opening the file:
		hFile = fopen(FileName,"rb");
		if(hFile == NULL)
		{
			m_oError = CSMTP_FILE_NOT_EXIST;
			break;
		}

		// checking file size:
		FileSize = 0;
		while(!feof(hFile))
			FileSize += fread(FileBuf,sizeof(char),54,hFile);
		TotalSize += FileSize;

		// sending the file:
		if(TotalSize/1024 > MSG_SIZE_IN_MB*1024)
			m_oError = CSMTP_MSG_TOO_BIG;
		else
		{
			fseek (hFile,0,SEEK_SET);

			MsgPart = 0;
			for(i=0;i<FileSize/54+1;i++)
			{
				res = fread(FileBuf,sizeof(char),54,hFile);
				MsgPart ? strcat(SendBuf,base64_encode(reinterpret_cast<const unsigned char*>(FileBuf),res).c_str())
					: strncpy(SendBuf,base64_encode(reinterpret_cast<const unsigned char*>(FileBuf),res).c_str() , BUFFER_SIZE - 1);
					//: strcpy(SendBuf,base64_encode(reinterpret_cast<const unsigned char*>(FileBuf),res).c_str());

				StringCchCatA(SendBuf, BUFFER_SIZE,"\r\n");
				//strcat(SendBuf,"\r\n");
				
				//MsgPart += res + 2;
				//54바이트를 base64 encode시에 72바이트로 늘어남. + "\r\n"
				int iEncodedByte = (int)(ceil( (float)res / 3.f )) * 4;
				MsgPart += iEncodedByte + 2; 
				
				if(MsgPart >= BUFFER_SIZE/2)
				{ // sending part of the message
					MsgPart = 0;
					if(!SMTPSendData())
					{
						delete[] FileBuf;
						delete[] FileName;
						fclose(hFile);
						return false;
					}
				}
			}
			if(MsgPart)
			{
				if(!SMTPSendData())
				{
					delete[] FileBuf;
					delete[] FileName;
					fclose(hFile);
					return false;
				}
			}
		}
		fclose(hFile);
	}
	delete[] FileBuf;
	delete[] FileName;

	// sending last message block (if there is one or more attachments)
	if(Attachments.size())
	{
		StringCchPrintfA(SendBuf, BUFFER_SIZE, "\r\n--%s--\r\n",BOUNDARY_TEXT);
		//sprintf(SendBuf,"\r\n--%s--\r\n",BOUNDARY_TEXT);
		if(!SMTPSendData())
			return false;
	}

	// <CRLF> . <CRLF>
	strncpy(SendBuf,"\r\n.\r\n" , BUFFER_SIZE - 1 );
	//strcpy(SendBuf,"\r\n.\r\n");
	if(!SMTPSendData())
		return false;
	Sleep(DELAY_IN_MS);
	if(!SMTPReceiveData())
		return false;

	switch(SMTPXYZdigits())
	{
	case 250:
		break;
	default:
		m_oError = CSMTP_MSG_BODY_ERROR;
		return false;
	}

	// ***** CLOSING CONNECTION *****

	// QUIT <CRLF>
	strncpy(SendBuf,"QUIT\r\n" , BUFFER_SIZE - 1 );
	//strcpy(SendBuf,"QUIT\r\n");
	if(!SMTPSendData())
		return false;
	Sleep(DELAY_IN_MS);
	if(!SMTPReceiveData())
		return false;

	switch(SMTPXYZdigits())
	{
	case 221:
		break;
	default:
		m_oError = CSMTP_COMMAND_QUIT;
		hSocket = NULL;
		return false;
	}

	closesocket(hSocket);
	hSocket = NULL;
	return true;
}

SOCKET CX2SMTPMail::SMTPConnectRemoteServer(const char *server,const unsigned short port)
{
	short nProtocolPort;
	LPHOSTENT lpHostEnt;
	LPSERVENT lpServEnt;
	SOCKADDR_IN sockAddr;
	ZeroMemory( &sockAddr, sizeof(SOCKADDR_IN) );

	SOCKET hServerSocket = INVALID_SOCKET;
	struct in_addr addr;

	// If the user input is an alpha name for the host, use gethostbyname()
	// If not, get host by addr (assume IPv4)
	if(isalpha(server[0]))
		lpHostEnt = gethostbyname(server);
	else
	{
		addr.s_addr = inet_addr(server);
		if(addr.s_addr == INADDR_NONE) 
		{
			m_oError = CSMTP_BAD_IPV4_ADDR;
			return INVALID_SOCKET;
		} 
		else
			lpHostEnt = gethostbyaddr((char *) &addr, 4, AF_INET);
	}

	if(lpHostEnt != NULL)
	{
		if((hServerSocket = socket(PF_INET, SOCK_STREAM,0)) != INVALID_SOCKET)
		{
			if(port != NULL)
				nProtocolPort = htons(port);
			else
			{
				lpServEnt = getservbyname("mail", 0);
				if (lpServEnt == NULL)
					nProtocolPort = htons(25);
				else 
					nProtocolPort = lpServEnt->s_port;
			}

			sockAddr.sin_family = AF_INET;
			sockAddr.sin_port = nProtocolPort;
			sockAddr.sin_addr = *((LPIN_ADDR)*lpHostEnt->h_addr_list);
			if(connect(hServerSocket,(PSOCKADDR)&sockAddr,sizeof(sockAddr)) == SOCKET_ERROR)
			{
				m_oError = CSMTP_WSA_CONNECT;
				hServerSocket = INVALID_SOCKET;
			}
		}
		else
		{
			m_oError = CSMTP_WSA_INVALID_SOCKET;
			return INVALID_SOCKET;
		}
	}
	else
	{
		m_oError = CSMTP_WSA_GETHOSTBY_NAME_ADDR;
		return INVALID_SOCKET;
	}

	return hServerSocket;
}

int CX2SMTPMail::SMTPXYZdigits()
{
	assert(RecvBuf);
	if(RecvBuf == NULL)
		return 0;
	return (RecvBuf[0]-'0')*100 + (RecvBuf[1]-'0')*10 + RecvBuf[2]-'0';
}

bool CX2SMTPMail::SMTPFormatHeader(char* header)
{
	//int i,s = 0;
	
	TCHAR szDate[500];
	TCHAR sztTime[500];
	char *to = NULL;
	char *cc = NULL;
	char *bcc = NULL;

	// check for at least one recipient
	if(Recipients.size())
	{
		unsigned int uiSize = 0;
		for (unsigned int i = uiSize; i < Recipients.size(); i++)
			uiSize += Recipients[i].Mail.size() + Recipients[i].Name.size() + 3;
		if ( 0 == uiSize ) 
			uiSize = 1;
		if((to = new char[uiSize]) == NULL)
		{
			m_oError = CSMTP_LACK_OF_MEMORY;
			return false;
		}

		to[0] = '\0';
		for (unsigned int i = 0;i < Recipients.size();i++)
		{
			i > 0 ? strcat(to,","):strcpy(to,"");
			strcat(to,Recipients[i].Name.c_str());
			strcat(to,"<");
			strcat(to,Recipients[i].Mail.c_str());
			strcat(to,">");
		}
	}
	else
	{
		m_oError = CSMTP_UNDEF_RECIPENTS;
		return false;
	}

	if(CCRecipients.size())
	{
		unsigned int uiSize = 0;
		for ( unsigned int i = uiSize; i < CCRecipients.size(); i++)
			uiSize += CCRecipients[i].Mail.size() + CCRecipients[i].Name.size() + 3;
		if (uiSize == 0)
			uiSize = 1;
		if((cc = new char[uiSize]) == NULL)
		{
			m_oError = CSMTP_LACK_OF_MEMORY;
			delete[] to;
			return false;
		}

		cc[0] = '\0';
		for (unsigned int i = 0; i < CCRecipients.size(); i++)
		{
			i > 0 ? strcat(cc,","):strcpy(cc,"");
			strcat(cc,CCRecipients[i].Name.c_str());
			strcat(cc,"<");
			strcat(cc,CCRecipients[i].Mail.c_str());
			strcat(cc,">");
		}
	}

	if(BCCRecipients.size())
	{
		unsigned int uiSize = 0;
		for (unsigned int i = uiSize; i < BCCRecipients.size(); i++)
			uiSize += BCCRecipients[i].Mail.size() + BCCRecipients[i].Name.size() + 3;
		if(uiSize == 0)
			uiSize = 1;
		if((bcc = new char[uiSize]) == NULL)
		{
			m_oError = CSMTP_LACK_OF_MEMORY;
			delete[] to;
			delete[] cc;
			return false;
		}

		bcc[0] = '\0';
		for (unsigned int i = 0; i < BCCRecipients.size(); i++)
		{
			i > 0 ? strcat(bcc,","):strcpy(bcc,"");
			strcat(bcc,BCCRecipients[i].Name.c_str());
			strcat(bcc,"<");
			strcat(bcc,BCCRecipients[i].Mail.c_str());
			strcat(bcc,">");
		}
	}

	// Date: <SP> <dd> <SP> <mon> <SP> <yy> <SP> <hh> ":" <mm> ":" <ss> <SP> <zone> <CRLF>
	SYSTEMTIME st={0};
	::GetSystemTime(&st);
	::GetDateFormat(MAKELCID(0x0409,SORT_DEFAULT),0,&st, L"ddd\',\' dd MMM yyyy",szDate,ARRAY_SIZE(szDate));
	::GetTimeFormat(MAKELCID(0x0409,SORT_DEFAULT),TIME_FORCE24HOURFORMAT,&st, L"HH\':\'mm\':\'ss",sztTime,ARRAY_SIZE(sztTime));
	StringCchPrintfA(header, BUFFER_SIZE, "Date: %s %s\r\n", szDate, sztTime); 
	//sprintf(header,"Date: %s %s\r\n", szDate, sztTime); 

	// From: <SP> <sender>  <SP> "<" <sender-email> ">" <CRLF>
	if(m_pcMailFrom == NULL)
	{
		m_oError = CSMTP_UNDEF_MAILFROM;
		delete[] to;
		delete[] cc;
		delete[] bcc;
		return false;
	}
	StringCchCatA(header,BUFFER_SIZE,"From: ");	
	//strcat(header,"From: ");	
	if(m_pcNameFrom)
		StringCchCatA(header,BUFFER_SIZE, m_pcNameFrom);
		//strcat(header, m_pcNameFrom);

	StringCchCatA(header,BUFFER_SIZE," <");
	StringCchCatA(header,BUFFER_SIZE,m_pcMailFrom);
	StringCchCatA(header,BUFFER_SIZE, ">\r\n");
	/*strcat(header," <");
	strcat(header,m_pcMailFrom);
	strcat(header, ">\r\n");*/

	// X-Mailer: <SP> <xmailer-app> <CRLF>
	if (m_pcXMailer != NULL)
	{
		StringCchCatA(header,BUFFER_SIZE,"X-Mailer: ");
		StringCchCatA(header,BUFFER_SIZE, m_pcXMailer);
		StringCchCatA(header,BUFFER_SIZE, "\r\n");
		/*strcat(header,"X-Mailer: ");
		strcat(header, m_pcXMailer);
		strcat(header, "\r\n");*/
	}

	// Reply-To: <SP> <reverse-path> <CRLF>
	if(m_pcReplyTo != NULL)
	{
		StringCchCatA(header,BUFFER_SIZE, "Reply-To: ");
		StringCchCatA(header,BUFFER_SIZE, m_pcReplyTo);
		StringCchCatA(header,BUFFER_SIZE, "\r\n");
		/*strcat(header, "Reply-To: ");
		strcat(header, m_pcReplyTo);
		strcat(header, "\r\n");*/
	}

	// X-Priority: <SP> <number> <CRLF>
	switch(m_iXPriority)
	{
	case XPRIORITY_HIGH:
		StringCchCatA(header,BUFFER_SIZE,"X-Priority: 2 (High)\r\n");
		//strcat(header,"X-Priority: 2 (High)\r\n");
		break;
	case XPRIORITY_NORMAL:
		StringCchCatA(header,BUFFER_SIZE,"X-Priority: 3 (Normal)\r\n");
		//strcat(header,"X-Priority: 3 (Normal)\r\n");
		break;
	case XPRIORITY_LOW:
		StringCchCatA(header,BUFFER_SIZE,"X-Priority: 4 (Low)\r\n");
		//strcat(header,"X-Priority: 4 (Low)\r\n");
		break;
	default:
		StringCchCatA(header,BUFFER_SIZE,"X-Priority: 3 (Normal)\r\n");
		//strcat(header,"X-Priority: 3 (Normal)\r\n");
	}

	// To: <SP> <remote-user-mail> <CRLF>
	StringCchCatA(header,BUFFER_SIZE,"To: ");
	StringCchCatA(header,BUFFER_SIZE, to);
	StringCchCatA(header,BUFFER_SIZE, "\r\n");
	/*strcat(header,"To: ");
	strcat(header, to);
	strcat(header, "\r\n");*/

	// Cc: <SP> <remote-user-mail> <CRLF>
	if(CCRecipients.size())
	{
		StringCchCatA(header,BUFFER_SIZE,"Cc: ");
		StringCchCatA(header,BUFFER_SIZE, cc);
		StringCchCatA(header,BUFFER_SIZE, "\r\n");
		/*strcat(header,"Cc: ");
		strcat(header, cc);
		strcat(header, "\r\n");*/
	}

	if(BCCRecipients.size())
	{
		StringCchCatA(header,BUFFER_SIZE,"Bcc: ");
		StringCchCatA(header,BUFFER_SIZE, bcc);
		StringCchCatA(header,BUFFER_SIZE, "\r\n");
		/*strcat(header,"Bcc: ");
		strcat(header, bcc);
		strcat(header, "\r\n");*/
	}

	// Subject: <SP> <subject-text> <CRLF>
	if(m_pcSubject == NULL) 
	{
		m_oError = CSMTP_UNDEF_SUBJECT;
		StringCchCatA(header,BUFFER_SIZE, "Subject:  ");
		//strcat(header, "Subject:  ");
	}
	else
	{
		StringCchCatA(header,BUFFER_SIZE, "Subject: ");
		StringCchCatA(header,BUFFER_SIZE, m_pcSubject);
		/*strcat(header, "Subject: ");
		strcat(header, m_pcSubject);*/
	}
	StringCchCatA(header,BUFFER_SIZE, "\r\n");
	//strcat(header, "\r\n");

	// MIME-Version: <SP> 1.0 <CRLF>
	StringCchCatA(header,BUFFER_SIZE,"MIME-Version: 1.0\r\n");
	StringCchCatA(header,BUFFER_SIZE,"MIME-Version: 1.0\r\n");

	/*strcat(header,"MIME-Version: 1.0\r\n");
	strcat(header,"MIME-Version: 1.0\r\n");*/
	if(!Attachments.size())
	{ // no attachments
		StringCchCatA(header,BUFFER_SIZE,"Content-type: text/plain; \r\n");
		StringCchCatA(header,BUFFER_SIZE,"Content-Transfer-Encoding: 7bit\r\n");
		StringCchCatA(SendBuf,BUFFER_SIZE,"\r\n");

		/*strcat(header,"Content-type: text/plain; \r\n");
		strcat(header,"Content-Transfer-Encoding: 7bit\r\n");
		strcat(SendBuf,"\r\n");*/
	}
	else
	{ // there is one or more attachments

		StringCchCatA(header,BUFFER_SIZE,"Content-Type: multipart/mixed; boundary=\"");
		StringCchCatA(header,BUFFER_SIZE,BOUNDARY_TEXT);
		StringCchCatA(header,BUFFER_SIZE,"\"\r\n");
		StringCchCatA(header,BUFFER_SIZE,"\r\n");

		/*strcat(header,"Content-Type: multipart/mixed; boundary=\"");
		strcat(header,BOUNDARY_TEXT);
		strcat(header,"\"\r\n");
		strcat(header,"\r\n");*/
		// first goes text message

		StringCchCatA(SendBuf,BUFFER_SIZE,"--");
		StringCchCatA(SendBuf,BUFFER_SIZE,BOUNDARY_TEXT);
		StringCchCatA(SendBuf,BUFFER_SIZE,"\r\n");
		StringCchCatA(SendBuf,BUFFER_SIZE,"Content-type: text/plain; \r\n");
		StringCchCatA(SendBuf,BUFFER_SIZE,"Content-Transfer-Encoding: 7bit\r\n");
		StringCchCatA(SendBuf,BUFFER_SIZE,"\r\n");

		/*strcat(SendBuf,"--");
		strcat(SendBuf,BOUNDARY_TEXT);
		strcat(SendBuf,"\r\n");
		strcat(SendBuf,"Content-type: text/plain; \r\n");
		strcat(SendBuf,"Content-Transfer-Encoding: 7bit\r\n");
		strcat(SendBuf,"\r\n");*/
	}

	// clean up
	delete[] to;
	delete[] cc;
	delete[] bcc;

	// done    
	return true;    
}

bool CX2SMTPMail::SMTPReceiveData()
{
	assert(RecvBuf);

	int res;

	if(RecvBuf == NULL)
		return false;

	if( (res = recv(hSocket,RecvBuf,BUFFER_SIZE,0)) == SOCKET_ERROR )
	{
		m_oError = CSMTP_WSA_RECV;
		return false;
	}
	if(res == 0)
	{
		m_oError = CSMTP_CONNECTION_CLOSED;
		return false;
	}
	RecvBuf[res] = '\0';

	return true;
}

bool CX2SMTPMail::SMTPSendData()
{
	assert(SendBuf);

	int idx = 0,res,nLeft = strlen(SendBuf);
	while(nLeft > 0)
	{
		if( res = send(hSocket,&SendBuf[idx],nLeft,0) == SOCKET_ERROR)
		{
			m_oError = CSMTP_WSA_SEND;
			return false;
		}
		if(!res)
			break;
		nLeft -= res;
		idx += res;
	}
	return true;
}

CX2SMTPMail::CSmtpError CX2SMTPMail::SMTPGetLastError()
{
	return m_oError;
}

/*
const char* const CX2SMTPMail::SMTPGetLocalHostIP()
{
in_addr *iaHost = NULL;
HOSTENT *pHe = NULL;

if (m_pcIPAddr)
{
delete[] m_pcIPAddr;
m_pcIPAddr = NULL;
}

if(gethostname(m_pcHostName,255) != SOCKET_ERROR)
{
pHe = gethostbyname(m_pcHostName);
if (pHe != NULL) 
{
for (int i=0;pHe->h_addr_list[i] != 0;i++)
{
iaHost = (LPIN_ADDR)pHe->h_addr_list[i];
m_pcIPAddr = inet_ntoa(*iaHost);
}
}            
} 
else 
{
m_oError = CSMTP_WSA_GETHOSTBY_NAME_ADDR;
m_pcIPAddr = NULL;
}

return m_pcIPAddr;
}
*/

const char* const CX2SMTPMail::SMTPGetLocalHostName() 
{
	if(m_pcLocalHostName)
		delete[] m_pcLocalHostName;
	if((m_pcLocalHostName = new char[255]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return NULL;
	}
	if(gethostname((char FAR*)m_pcLocalHostName,255) == SOCKET_ERROR)
		m_oError = CSMTP_WSA_HOSTNAME;
	return m_pcLocalHostName;
}

unsigned const int CX2SMTPMail::SMTPGetBCCRecipientCount()
{
	return BCCRecipients.size();
}

unsigned const int CX2SMTPMail::SMTPGetCCRecipientCount() 
{
	return CCRecipients.size();
}

const char* const CX2SMTPMail::SMTPGetMessageBody() 
{
	return m_pcMsgBody;
}

unsigned const int CX2SMTPMail::SMTPGetRecipientCount()
{
	return Recipients.size();
}

const char* const CX2SMTPMail::SMTPGetReplyTo()  
{
	return m_pcReplyTo;
}

const char* const CX2SMTPMail::SMTPGetMailFrom() 
{
	return m_pcMailFrom;
}

const char* const CX2SMTPMail::SMTPGetSenderName() 
{
	return m_pcNameFrom;
}

const char* const CX2SMTPMail::SMTPGetSubject() 
{
	return m_pcSubject;
}

const char* const CX2SMTPMail::SMTPGetXMailer() 
{
	return m_pcXMailer;
}

CX2SMTPMail::CSmptXPriority CX2SMTPMail::SMTPGetXPriority()
{
	return m_iXPriority;
}

void CX2SMTPMail::SMTPSetXPriority(CSmptXPriority priority)
{
	m_iXPriority = priority;
}

void CX2SMTPMail::SMTPSetMessageBody(const char *body)
{
	assert(body);
	int s = strlen(body);
	if (m_pcMsgBody)
	{
		delete[] m_pcMsgBody;
		m_pcMsgBody = NULL;
	}
	if((m_pcMsgBody = new char[s+1]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return;
	}
	strncpy(m_pcMsgBody, body , s);
	m_pcMsgBody[s] = '\0';
	//strcpy(m_pcMsgBody, body);    
}

void CX2SMTPMail::SMTPSetReplyTo(const char *replyto)
{
	assert(replyto);
	int s = strlen(replyto);
	if (m_pcReplyTo)
	{
		delete[] m_pcReplyTo;
		m_pcReplyTo = NULL;
	}
	if((m_pcReplyTo = new char[s+1]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return;
	}
	strncpy(m_pcReplyTo, replyto ,s);
	m_pcReplyTo[s] = '\0';
	//strcpy(m_pcReplyTo, replyto);
}

void CX2SMTPMail::SMTPSetSenderMail(const char *email)
{
	assert(email);
	int s = strlen(email);
	if (m_pcMailFrom)
	{
		delete[] m_pcMailFrom;
		m_pcMailFrom = NULL;
	}
	if((m_pcMailFrom = new char[s+1]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return;
	}
	strncpy(m_pcMailFrom, email ,s);    
	m_pcMailFrom[s] = '\0';
	//strcpy(m_pcMailFrom, email);        
}

void CX2SMTPMail::SMTPSetSenderName(const char *name)
{
	assert(name);
	int s = strlen(name);
	if (m_pcNameFrom)
	{
		delete[] m_pcNameFrom;
		m_pcNameFrom = NULL;
	}
	if((m_pcNameFrom = new char[s+1]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return;
	}
	strncpy(m_pcNameFrom, name , s);
	m_pcNameFrom[s] = '\0';
	//strcpy(m_pcNameFrom, name);
}

void CX2SMTPMail::SMTPSetSubject(const char *subject)
{
	assert(subject);
	int s = strlen(subject);
	if (m_pcSubject)
	{
		delete[] m_pcSubject;
		m_pcSubject = NULL;
	}
	if((m_pcSubject = new char[s+1]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return;
	}
	strncpy(m_pcSubject, subject , s);
	m_pcSubject[s] = '\0';
	//strcpy(m_pcSubject, subject);
}

void CX2SMTPMail::SMTPSetXMailer(const char *xmailer)
{
	assert(xmailer);
	int s = strlen(xmailer);
	if (m_pcXMailer)
	{
		delete[] m_pcXMailer;
		m_pcXMailer = NULL;
	}
	if((m_pcXMailer = new char[s+1]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return;
	}
	strncpy(m_pcXMailer, xmailer , s);
	m_pcXMailer[s] = '\0';
	//strcpy(m_pcXMailer, xmailer);
}

void CX2SMTPMail::SMTPSetLogin(const char *login)
{
	assert(login);
	int s = strlen(login);
	if (m_pcLogin)
	{
		delete[] m_pcLogin;
		m_pcLogin = NULL;
	}
	if((m_pcLogin = new char[s+1]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return;
	}
	strncpy(m_pcLogin, login, s);
	m_pcLogin[s] = '\0';
	//strcpy(m_pcLogin, login);
}

void CX2SMTPMail::SMTPSetPassword(const char *password)
{
	assert(password);
	int s = strlen(password);
	if (m_pcPassword)
	{
		delete[] m_pcPassword;
		m_pcPassword = NULL;
	}
	if((m_pcPassword = new char[s+1]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return;
	}
	strncpy(m_pcPassword, password , s);
	m_pcPassword[s] = '\0';
	//strcpy(m_pcPassword, password);
}

void CX2SMTPMail::SMTPSetServer(const char* SrvName,const unsigned short SrvPort)
{
	assert(SrvName);
	m_iSMTPSrvPort = SrvPort;
	int s = strlen(SrvName);
	if(m_pcSMTPSrvName)
	{
		delete[] m_pcSMTPSrvName;
		m_pcSMTPSrvName = NULL;
	}
	if((m_pcSMTPSrvName = new char[s+1]) == NULL)
	{
		m_oError = CSMTP_LACK_OF_MEMORY;
		return;
	}
	strncpy(m_pcSMTPSrvName, SrvName , s);
	m_pcSMTPSrvName[s] = '\0';
	//strcpy(m_pcSMTPSrvName, SrvName);
}

//////////////////////////////////////////////////////////////////////
// Friends
//////////////////////////////////////////////////////////////////////

char* GetErrorText(CX2SMTPMail::CSmtpError ErrorId)
{
	switch(ErrorId)
	{
	case CX2SMTPMail::CSMTP_NO_ERROR:
		return "";
	case CX2SMTPMail::CSMTP_WSA_STARTUP:
		return "Unable to initialise winsock2.";
	case CX2SMTPMail::CSMTP_WSA_VER:
		return "Wrong version of the winsock2.";
	case CX2SMTPMail::CSMTP_WSA_SEND:
		return "Function send() failed.";
	case CX2SMTPMail::CSMTP_WSA_RECV:
		return "Function recv() failed.";
	case CX2SMTPMail::CSMTP_WSA_CONNECT:
		return "Function connect failed.";
	case CX2SMTPMail::CSMTP_WSA_GETHOSTBY_NAME_ADDR:
		return "Functions gethostbyname() or gethostbyaddr() failed.";
	case CX2SMTPMail::CSMTP_WSA_INVALID_SOCKET:
		return "Invalid winsock2 socket.";
	case CX2SMTPMail::CSMTP_WSA_HOSTNAME:
		return "Function hostname() failed.";
	case CX2SMTPMail::CSMTP_BAD_IPV4_ADDR:
		return "Improper IPv4 address.";
	case CX2SMTPMail::CSMTP_UNDEF_MSG_HEADER:
		return "Undefined message header.";
	case CX2SMTPMail::CSMTP_UNDEF_MAILFROM:
		return "Undefined from is the mail.";
	case CX2SMTPMail::CSMTP_UNDEF_SUBJECT:
		return "Undefined message subject.";
	case CX2SMTPMail::CSMTP_UNDEF_RECIPENTS:
		return "Undefined at least one reciepent.";
	case CX2SMTPMail::CSMTP_UNDEF_RECIPENT_MAIL:
		return "Undefined recipent mail.";
	case CX2SMTPMail::CSMTP_UNDEF_LOGIN:
		return "Undefined user login.";
	case CX2SMTPMail::CSMTP_UNDEF_PASSWORD:
		return "Undefined user password.";
	case CX2SMTPMail::CSMTP_COMMAND_MAIL_FROM:
		return "Server returned error after sending MAIL FROM.";
	case CX2SMTPMail::CSMTP_COMMAND_EHLO:
		return "Server returned error after sending EHLO.";
	case CX2SMTPMail::CSMTP_COMMAND_AUTH_LOGIN:
		return "Server returned error after sending AUTH LOGIN.";
	case CX2SMTPMail::CSMTP_COMMAND_DATA:
		return "Server returned error after sending DATA.";
	case CX2SMTPMail::CSMTP_COMMAND_QUIT:
		return "Server returned error after sending QUIT.";
	case CX2SMTPMail::CSMTP_COMMAND_RCPT_TO:
		return "Server returned error after sending RCPT TO.";
	case CX2SMTPMail::CSMTP_MSG_BODY_ERROR:
		return "Error in message body";
	case CX2SMTPMail::CSMTP_CONNECTION_CLOSED:
		return "Server has closed the connection.";
	case CX2SMTPMail::CSMTP_SERVER_NOT_READY:
		return "Server is not ready.";
	case CX2SMTPMail::CSMTP_FILE_NOT_EXIST:
		return "File not exist.";
	case CX2SMTPMail::CSMTP_MSG_TOO_BIG:
		return "Message is too big.";
	case CX2SMTPMail::CSMTP_BAD_LOGIN_PASS:
		return "Bad login or password.";
	case CX2SMTPMail::CSMTP_UNDEF_XYZ_RESPOMSE:
		return "Undefined xyz SMTP response.";
	case CX2SMTPMail::CSMTP_LACK_OF_MEMORY:
		return "Lack of memory.";
	default:
		return "Undefined error id.";
	}
}


#pragma warning(pop)
//}} oasis907 : 김상윤 [2009.12.21] // 새 CSmtp

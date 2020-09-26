#include "stdafx.h"
#include "HttpSession.h"


KHttpSession::KHttpSession()
	:m_hSession(NULL),m_hConnection(NULL),m_hRequest(NULL),buffer(NULL)
{	
	buffer = new char[HTTP_BUFFERSIZE];
}

KHttpSession::~KHttpSession(void)
{
	if(buffer)
	{
		delete buffer;
		buffer =NULL;
	}

	Disconnect();
	CloseSession();
}


bool KHttpSession::OpenSession(std::string sessionname)
{
	CloseSession();
	m_hSession = InternetOpenA(sessionname.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
	if(m_hSession)
		return true;
	return true;
}

void KHttpSession::CloseSession()
{
	if(m_hSession)
	{
		InternetCloseHandle(m_hSession);
		m_hSession =NULL;
	}
}
bool KHttpSession::Connect( std::string servername, unsigned int port/*=INTERNET_DEFAULT_HTTP_PORT*/ )
{
	Disconnect();
	m_hConnection = InternetConnectA( m_hSession, servername.c_str() , port, NULL,NULL, INTERNET_SERVICE_HTTP, 0  , NULL);
	if(m_hConnection)
		return true;
	return true;
}

void KHttpSession::Disconnect()
{
	if(m_hConnection)
	{
		InternetCloseHandle(m_hConnection);
		m_hConnection = NULL;
	}
}

bool KHttpSession::Open( std::string url,int offset,int length ,int* query )
{	
	m_hRequest = HttpOpenRequestA(m_hConnection,"GET",url.c_str(), "HTTP/1.1",NULL,NULL,				
		INTERNET_FLAG_KEEP_CONNECTION|
		INTERNET_FLAG_PRAGMA_NOCACHE|
		INTERNET_FLAG_NO_AUTH|
		//#if defined(KR_SERVICE_PATCHER) || defined(KR_OPENTEST_PATCHER)
		//{{ oasis907 : ±è»óÀ± [2010.8.31] // 
		INTERNET_FLAG_RELOAD|
		INTERNET_FLAG_NO_CACHE_WRITE|
		//}} oasis907 : ±è»óÀ± [2010.8.31] // 
		//#endif
		INTERNET_FLAG_NO_COOKIES,
		NULL);

	if(offset>0)
	{
		char header[256];
		if(length>0)
		{
			sprintf(header,"Range:bytes=%d-%d\r\n",offset,offset+length -1 );
		}
		else
		{
			sprintf(header,"Range:bytes=%d-\r\n",offset);
		}

		if(!HttpSendRequestA(m_hRequest,header,strlen(header),NULL,0))
		{
			Close();
			return false;
		}
	}
	else
	{		
		if(!HttpSendRequestA(m_hRequest,NULL,0,NULL,0))
		{
			Close();
			return false;
		}
	}		

	
	int querydata;
	DWORD BufLen = sizeof(querydata);

	if(HttpQueryInfoA(m_hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, (LPVOID)&querydata, &BufLen,0))
	{
		if(200 > querydata  || querydata > 206)
		{
			if(query)
				*query=querydata;

			Close();
			return false;
		}		
	}
	
	if(HttpQueryInfoA(m_hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, (LPVOID)&querydata, &BufLen,0))	
	{
		if(query)
			*query=querydata;
		return true;
	}	
	Close();
	return false;
}

void KHttpSession::Close()
{
	if(m_hRequest)
	{
		InternetCloseHandle(m_hRequest);
		m_hRequest = NULL;
	}
}

bool KHttpSession::SplitURL( std::string url,std::string &server,std::string &path )
{
	std::string urlinlower(url);	
	std::transform(urlinlower.begin(),urlinlower.end(),urlinlower.begin(),tolower);
	int httpstart = urlinlower.find_first_of("http://");	

	if(httpstart<0 )
		return false;

	std::string urlwohttp = url.substr(httpstart+7);

	int servernameend = urlwohttp.find_first_of("/");
	if(servernameend<0 )
		return false;

	server = urlwohttp.substr(0,servernameend);
	path = urlwohttp.substr(servernameend);

	return true;
}

bool KHttpSession::Write( std::ostream &stream,FILE_PROGRESS_CALLBACK progress /*= NULL*/ )
{
	int totallen;
	DWORD BufLen = sizeof(totallen);

	if(false == HttpQueryInfoA(m_hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, (LPVOID)&totallen, &BufLen,0))	
	{
		return false;
	}
	return Write(stream,totallen,progress);
}

bool KHttpSession::Write( std::ostream &stream ,int totallen, FILE_PROGRESS_CALLBACK progress)
{	
	if(m_hRequest ==NULL)
		return false;

	DWORD availablesize = 1;
	DWORD readlength;
	int leftlen = totallen;

	if(progress)
	{
#ifdef GATHER_PATCH_INFO
		progress(0.0f, 0.0f);
#else GATHER_PATCH_INFO
		progress(0.0f);
#endif GATHER_PATCH_INFO
	}
	
	while(InternetQueryDataAvailable(m_hRequest,&availablesize,0,0) && leftlen >0)
	{		
		int len = HTTP_BUFFERSIZE;
		if(len > (int)availablesize)
			len = availablesize;

		if(len > leftlen)
			len = leftlen;
		
		if(InternetReadFile(m_hRequest, buffer, len , &readlength))
		{		
			stream.write(buffer,readlength);
			
			leftlen -=readlength;
			
			if(progress)
			{
#ifdef GATHER_PATCH_INFO
				float fTmp = (float)(totallen-leftlen) /(float)totallen;
				if(progress(fTmp, 0.0f) ==false)
					return true;
#else GATHER_PATCH_INFO
				if(progress((float)(totallen-leftlen) /(float)totallen) ==false)
					return true;
#endif GATHER_PATCH_INFO


			}
		}
	}



	if(progress)
	{
#ifdef GATHER_PATCH_INFO
		progress(1.0f, totallen);
#else GATHER_PATCH_INFO
		progress(1.0f);
#endif GATHER_PATCH_INFO
	}

	return true;	
}


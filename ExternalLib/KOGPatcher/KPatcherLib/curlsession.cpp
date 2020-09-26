#include "curlsession.h"
#include <sys/stat.h>

extern void ResetReadOnly(std::string path);

bool KCurlSession::SplitURL( std::string url,std::string &server,std::string &path )
{
	int last = url.find_last_of("/");

	server = url.substr(0,last+1);
	path = url.substr(last+1);

	return true;
}

KCurlSession::KCurlSession()
:m_ctx(NULL)
{
m_ctx = curl_easy_init();
}


KCurlSession::KCurlSession( std::string id,std::string pwd )
:m_ctx(NULL)
{	
	m_ctx = curl_easy_init();

	if(id.empty() ==false && pwd.empty()==false)
	{	
		std::string userpwd(id);
		userpwd.append(":");
		userpwd.append(pwd);	
		curl_easy_setopt( m_ctx , CURLOPT_USERPWD, userpwd.c_str());
	}
}

KCurlSession::~KCurlSession()
{
	curl_easy_cleanup(m_ctx);
}


size_t CurlWrite( void *source , size_t size , size_t nmemb , void *userData )
{	
	SRecieveInfo *recieveinfo = (SRecieveInfo*)userData;	

	if(recieveinfo->progressinfo)
	{
		if(recieveinfo->progressinfo->isstoped)
			return 0;
	}

	size_t recievesize = size * nmemb;
	size_t re = recievesize;
	
	if(recieveinfo->length >= 0)
	{		
		int remainsize = recieveinfo->length - recieveinfo->readlength;
		if(remainsize <=0)
			return 0;		

		if(recievesize > remainsize)
		{	
			recievesize = remainsize;
			re = 0;
		}
	}
	
	recieveinfo->stream.write((const char*)source,recievesize);
	recieveinfo->readlength+= recievesize;

	if(recieveinfo->progressinfo)
		recieveinfo->progressinfo->Read(recievesize);

	if(recieveinfo->progress)		
	{
		recieveinfo->progress(recieveinfo->progressinfo);		
	}	
	return re;
}



bool KCurlSession::WriteData( std::string url,std::ostream &stream,int offset,int length,
								   NETPROGRESS_CALLBACK progress ,SProgressInfo * progressinfo,NETERROR_CALLBACK error)
{
	if(progressinfo)
		if(progressinfo->isstoped)
			return true;

	int start = 0;
	while((start = url.find_first_of(" ")) > 0)
	{
		url.replace(start,1,"%20");
	}
	
	SRecieveInfo recieveinfo(url,stream,offset,length,progress,progressinfo);	

	if(offset>0)
	{
		curl_easy_setopt(m_ctx,CURLOPT_RESUME_FROM,offset);
	}
	curl_easy_setopt(m_ctx,CURLOPT_WRITEDATA, (void*)&recieveinfo);
	curl_easy_setopt(m_ctx,CURLOPT_WRITEFUNCTION,CurlWrite);
	curl_easy_setopt(m_ctx,CURLOPT_URL,url.c_str());	

	CURLcode curlcode = curl_easy_perform(m_ctx);

	if(progressinfo)
		if(progressinfo->isstoped)
			return true;

	long httpcode = 0;
	curl_easy_getinfo (m_ctx, CURLINFO_RESPONSE_CODE, &httpcode);

	if(httpcode<200 || httpcode>299)
	{
		if(progressinfo)
			progressinfo->Rollback(recieveinfo.readlength);

		if(error)		
			error(url,curlcode,httpcode);
		
		return false;
	}

	if(curlcode == CURLE_OK ||
		(curlcode == CURLE_WRITE_ERROR && recieveinfo.length == recieveinfo.readlength) ) // 원래 CURL_WRITEFUNC_PAUSE 를 리턴해주면 멈춰야하지만 버그가 있는듯 하다 
		return true;

	// 아니면 받을 길이 없다
	if(progressinfo)
		progressinfo->Rollback(recieveinfo.readlength);	

	if(error)		
		error(url,curlcode,httpcode);

	return false;
}

bool KCurlSession::DownloadFile( std::string url,std::string filename,
								int off /*= 0*/,int length/*=-1*/,NETPROGRESS_CALLBACK progress/*=NULL*/,SProgressInfo * progressinfo /*=NULL*/,NETERROR_CALLBACK error/*= NULL*/ )
{	

	std::ofstream filestream(filename.c_str(),std::ios_base::binary|std::ios_base::out);	
	if(filestream.is_open() == false)	
	{
		return false;
	}

	if(WriteData(url,filestream,off,length,progress,progressinfo,error)==false)
	{
		filestream.close();
		return false;
	}
	filestream.close();	
	return true;
}

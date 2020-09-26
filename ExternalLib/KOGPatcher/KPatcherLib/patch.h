#pragma once 

#include "stdafx.h"
#include "threadpool.hpp"
#include "curlsession.h"
#include "../Kom2/kom.h"

#define PATCHINFOFILE "patchinfo.xml"
#define MAX_RETRY			5

class KPatchFileInfo
{
public:
	KPatchFileInfo()
		:size(-1),adler32(-1),filetime(-1)
	{}
	KPatchFileInfo(xmlNode * node);	
	KPatchFileInfo(std::string filename);
	KPatchFileInfo(const KPatchFileInfo&r)
		:size(r.size),adler32(r.adler32),filetime(r.filetime),alias(r.alias)
	{}

	~KPatchFileInfo(){}

	KPatchFileInfo & operator = (const KPatchFileInfo & r)
	{
		size = r.size;
		adler32 = r.adler32;
		filetime = r.filetime;
		alias = r.alias;
		return *this;
	}

	bool operator == (const KPatchFileInfo & r)
	{
		return (size == r.size && adler32 == r.adler32 && filetime == r.filetime);
	}

	bool operator != (const KPatchFileInfo & r)
	{
		return (size != r.size || adler32 != r.adler32 || filetime != r.filetime);
	}

	unsigned int GetSize(){return size;}
	unsigned int GetAdler32(){return adler32;}
	unsigned int GetFileTime(){return filetime;}

	bool Verify(std::string filename);

	std::string GetAlias(){return alias;}

	

private:

	unsigned int adler32;
	unsigned int size;
	unsigned int filetime;	
	std::string alias;
};




class KPatchInfo
{
public:
	
	bool Open(KCurlSession * session,std::string url);
	bool Open(KPatchInfo & l,std::string dir);

	KPatchInfo(){Close();}
	KPatchInfo(const KPatchInfo & r)
		:fileinfos(r.fileinfos)
	{}
	~KPatchInfo(){Close();}

	void Close(){fileinfos.clear();}	

	KPatchInfo & operator = (const KPatchInfo & r)
	{
		fileinfos = r.fileinfos;
		return *this;
	}	

	bool operator == (KPatchInfo & r);
	
	void Diff(KPatchInfo &l,KPatchInfo&r);
	unsigned int GetFileSize();

	void Insert(std::string key, std::string filename);
	void Erase(std::string filename);
	
	KPatchFileInfo Find(std::string);
	bool Exist(std::string);
	bool IsEmpty();


	std::map<std::string,KPatchFileInfo> & GetFileInfos(){return fileinfos;}

	std::vector<std::string> & GetPatcherFiles(){return patcherfiles;}
	std::vector<std::string> & GetVerifyFiles(){return verifyfiles;}
private:
	std::map<std::string,KPatchFileInfo> fileinfos;

	std::vector<std::string> patcherfiles;
	std::vector<std::string> verifyfiles;
	boost::mutex mut;
};




class KPatchLib
{
public:


	KPatchLib();
	KPatchLib(int tpsize);
	KPatchLib(int tpsize,std::string id_,std::string pw_);
	~KPatchLib();


	bool GetBooleanFromDat(std::string url,bool &re,NETERROR_CALLBACK error = NULL);	

	bool GetPatchPath(std::string patchpathurl,NETERROR_CALLBACK error = NULL);	
	bool GetServerPatchInfo(){return server_info.Open(&session,patchpath+PATCHINFOFILE);}
	bool GetLocalPatchInfo(){return local_info.Open(server_info,localpath);}

	void Diff();

	bool DownloadFiles(NETSTATRT_CALLBACK start,
        NETPROGRESS_CALLBACK fileprogress=NULL,
        NETERROR_CALLBACK error = NULL,
        COMPLETE_CALLBACK compete = NULL);
	void DownloadFilesAsync(NETSTATRT_CALLBACK start,
        NETPROGRESS_CALLBACK fileprogress=NULL,
        NETERROR_CALLBACK error = NULL,
        COMPLETE_CALLBACK compete = NULL);
	bool IsPatchComplete(){return diff_info.IsEmpty();}

	
	bool CheckPatcherFiles();
	bool DownloadPatcherFiles( NETERROR_CALLBACK error = NULL);
	void ClearPatcherFiles();
	
	bool Run(std::string commandline);

	bool VerifyPatcherName(std::string patchername);
	bool VerifyPatcherPath(std::string path);
	

	void Stop();
	bool IsStoped(){return progressinfo.isstoped;}

	std::string GetLocalPath(){return localpath;}

	bool CreateMutex(std::wstring mutex);
	void ReleaseMutex();
	
private:
	void Init();		

	void DownloadFile(std::pair<std::string,KPatchFileInfo> info,NETPROGRESS_CALLBACK fileprogress,NETERROR_CALLBACK error);
    bool DownloadPatcherFile(std::string filename,NETERROR_CALLBACK error);	
	
	
	KCurlSession session;

	std::string id;
	std::string pw;

	std::string patchpath;
	std::string localpath;

	std::string patcherdir;
	std::string patchername;

	KPatchInfo server_info;
	KPatchInfo local_info;	
	KPatchInfo diff_info;

	bool isstoped;
	int errorcount;

	SProgressInfo progressinfo;
	boost::threadpool::pool tp;	
	HANDLE mutex;	

	std::vector<std::string> patcherfiles;
};
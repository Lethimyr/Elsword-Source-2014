#pragma once 

#include <vector>
#include "kom.h"

#define PATCHINFOFILE "patchinfo.xml"


class KFileInfo
{
public:
	KFileInfo(xmlNode * node);	
	KFileInfo(std::string filename);
	KFileInfo(const KFileInfo&r)
		:size(r.size),adler32(r.adler32),filetime(r.filetime)
	{}

	~KFileInfo(){}

	KFileInfo & operator = (const KFileInfo & r)
	{
		size = r.size;
		adler32 = r.adler32;
		filetime = r.filetime;
		return *this;
	}

	bool operator == (const KFileInfo & r)
	{
		return (size == r.size && adler32 == r.adler32 && filetime == r.filetime);
	}

	bool operator != (const KFileInfo & r)
	{
		return (size != r.size || adler32 != r.adler32 || filetime != r.filetime);
	}

	unsigned int GetSize(){return size;}
	unsigned int GetAdler32(){return adler32;}
	unsigned int GetFileTime(){return filetime;}

	bool Verify(std::string filename);

private:

	unsigned int adler32;
	unsigned int size;
	unsigned int filetime;	
};




class KPatchInfo
{
public:
	
	bool Open(KHttpSession * session,std::string url);
	bool Open( KPatchInfo & l,std::string dir, PROGRESS_CALLBACK checkFileprogress = NULL );

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
	void Add(std::string key, std::string filename);

	std::map<std::string,KFileInfo>& GetFileinfos(){return fileinfos;}

private:
	std::map<std::string,KFileInfo> fileinfos;
};




class KPatcher
{
public:
	KPatcher();
		

	~KPatcher(){}

	bool OpenSession(std::string sessionname){return session.OpenSession(sessionname);};
	void CloseSession(){session.CloseSession();};


	bool Connect(std::string patchpathurl);
	void Disconnect(){session.Disconnect();}
	bool Reconnect();

	bool GetServerPatchInfo(){return server_info.Open(&session,serverurl+PATCHINFOFILE);}
	bool GetLocalPatchInfo( PROGRESS_CALLBACK checkFileprogress = NULL ){return local_info.Open( server_info,localpath, checkFileprogress );}

	void Diff();

	bool ProcessOneFile(PROGRESS_CALLBACK totalprogress = NULL, PROGRESS_CALLBACK fileprogress = NULL);
	bool IsPatchComplete(){return diff_info.GetFileinfos().empty();}

	std::string GetFirstFileName();

	bool CheckPatcherChange();
	bool UpdatePatcher();
	bool RunPatcher(LPSTR commandline);
	bool RunClient(std::string clientname,std::string commandline);
	bool Verify(std::string patchername);

	void Stop(){isstoped=true;};
	bool IsStoped(){return isstoped;}

	unsigned int GetTotalSize() { return m_uiTotalsize; }
#ifdef DLL_COPY_RUN_ELSWORD
	bool DLLCapyStart(std::string localpath);
#endif //DLL_COPY_RUN_ELSWORD
private:

	KHttpSession session;
	std::string patchpath;

	std::string servername;
	std::string serverurl;

	std::string localpath;

	std::string patcherdir;
	std::string patchername;

	KPatchInfo server_info;
	KPatchInfo local_info;

	KPatchInfo diff_info;

	unsigned int m_uiTotalsize;

	bool isstoped;
};

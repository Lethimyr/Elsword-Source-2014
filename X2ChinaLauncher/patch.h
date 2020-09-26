#pragma once 

#include "define.h"

#include <vector>
#include "kom.h"
#include <TlHelp32.h>

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
		return (size == r.size && adler32 == r.adler32);
	}

	bool operator != (const KFileInfo & r)
	{
		return (size != r.size || adler32 != r.adler32);
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
	int GetFileCnt();

	void Add(std::string key, std::string filename);

	std::map<std::string,KFileInfo>& GetFileinfos(){return fileinfos;}

private:
	std::map<std::string,KFileInfo> fileinfos;
};




class KPatcher
{
public:
	enum EPatcherErrorType
	{
		ePatcherError_Type_Connect = 0,
		ePatcherError_Type_FileAccess,
		ePatcherError_Type_InvalidFile,
		ePatcherError_Type_Success
	};

	enum EPatcherKillProcessResultType
	{
		KP_FAILED,
		KP_SUCCESS_CLEAN,
		KP_SUCCESS_KILL
	};

	KPatcher();
		

	~KPatcher(){}

	bool OpenSession(std::string sessionname){return session.OpenSession(sessionname);};
	void CloseSession(){session.CloseSession();};


	bool Connect(std::string patchpathurl);
	void Disconnect(){session.Disconnect();}
	bool Reconnect();

	bool GetServerPatchInfo(){return server_info.Open(&session,serverurl+PATCHINFOFILE);}
	bool GetLocalPatchInfo(){return local_info.Open(server_info,localpath);}

	void Diff();

	EPatcherErrorType ProcessOneFile(TOTAL_PROGRESS_CALLBACK totalprogress = NULL, FILE_PROGRESS_CALLBACK fileprogress = NULL, std::string& strErrorInfo = std::string());
	bool IsPatchComplete(){return diff_info.GetFileinfos().empty();}

	std::string GetFirstFileName();

	bool CheckPatcherChange();
#ifdef GATHER_PATCH_INFO
	bool CheckPatcherChange(OUT unsigned int& uiAdler32,OUT unsigned int& uiSize,OUT unsigned int& uiFiletime);
#endif GATHER_PATCH_INFO
	bool UpdatePatcher();

	bool RunPatcher(LPSTR commandline);
	bool RunClient(std::string clientname,std::string commandline);
	bool Verify(std::string patchername);

	void Stop();
	bool IsStoped(){return isstoped;}
//#ifdef MULTI_CLIENT_OFF
//2011.11.29 lygan_조성욱 // 런처가 클라이언트가 떴는지 체크하는 부분
	DWORD CheckRunClient(LPCWSTR clientname);
	DWORD KillProsessClient(DWORD dwPID, DWORD dwTimeout);
//#endif //MULTI_CLIENT_OFF

	float GetTotalSize(){ return totalsize; };
	int GetTotalCount(){ return totalCount; };

	std::wstring GetLauncherPath() { return m_wstrLauncherPath; }
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

	float totalsize;
	int totalCount;

	bool isstoped;

	std::wstring m_wstrLauncherPath;
};
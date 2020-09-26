#include "stdafx.h"
#include "patch.h"
#include <algorithm>
#include <MMSystem.h>

KFileInfo::KFileInfo( xmlNode * node )
	:adler32(0),size(0),filetime(0)
{
	xmlAttr * attribute =  node->properties;
	while(attribute)
	{		
		if(attribute->children) 
		{
			if(strcmp((char*)attribute->name,"Checksum")==0)
			{
				sscanf((char*)attribute->children->content,"%x",&adler32);
			}			
			else if(strcmp((char*)attribute->name,"Size")==0)
			{
				sscanf((char*)attribute->children->content,"%d",&size);
			}			
			else if(strcmp((char*)attribute->name,"FileTime")==0)
			{
				sscanf((char*)attribute->children->content,"%x",&filetime);
			}
		}		
		attribute=attribute->next;
	}
}

KFileInfo::KFileInfo( std::string filename )
	:adler32(0),size(0),filetime(0)
{
	Komfile kom;
	bool iskom = false;
	if(filename.find_last_of(".kom") == filename.length() - 1)		
		iskom = kom.Open(filename);

	if(iskom)
	{		
		filetime = kom.GetFileTime();
		adler32 = kom.GetAdler32();
		kom.Close();

		WIN32_FILE_ATTRIBUTE_DATA data;
		GetFileAttributesExA(filename.c_str(),GetFileExInfoStandard,&data);
		size = data.nFileSizeLow;
	}
	else
	{
		adler32 = AdlerCheckSum::adler32(filename.c_str(),&size);
	}	
	

	kom.Close();
}

bool KFileInfo::Verify(std::string filename)
{
	KFileInfo info(filename);
	return *this == info;
}



bool KPatchInfo::Open( KHttpSession * session,std::string url )
{
	int headersize;
	
	if(session->Open(url,-1,-1,&headersize)==false)
		return false;

	std::stringstream stream;
	if(session->Write(stream)==false)
	{
		session->Close();
		return false;
	}
	session->Close();

	
	xmlDoc * doc;			
	doc = xmlReadMemory(stream.str().c_str(),headersize,"Komfiles.xml",NULL,0);
	
	xmlNode * patchinfo = xmlDocGetRootElement(doc);
	if(patchinfo->type == XML_ELEMENT_NODE && strcmp((char*)patchinfo->name,"Patchinfo")==0)
	{		
		xmlNode * files = patchinfo->children;		
		while(files)
		{			
			if(files->type == XML_ELEMENT_NODE && strcmp((char*)files->name,"Files")==0)
			{			
				xmlNode * file = files->children;
				while(file)
				{
					if(file->type == XML_ELEMENT_NODE && strcmp((char*)file->name,"File")==0)
					{	
						std::string key;
						xmlAttr * attribute =  file->properties;
						while(attribute)
						{
							if(strcmp((char*)attribute->name,"Name")==0 && attribute->children)
							{
								key =(char*)attribute->children->content;
								break;							
							}
							attribute = attribute->next;
						}						
						std::transform(key.begin(),key.end(),key.begin(),tolower);
						fileinfos.insert(std::map<std::string,KFileInfo>::value_type(key, KFileInfo(file)));
					}
					file = file->next;
				}
				break;
			}	
			files = files->next;
		}

		if(files == NULL)
		{
			xmlFreeDoc(doc);
			return false;
		}
	}
	else
	{
		xmlFreeDoc(doc);
		return false;
	}
	
	xmlFreeDoc(doc);
	return true;
}


bool KPatchInfo::Open( KPatchInfo & l,std::string dir )
{
	std::map<std::string,KFileInfo> &l_fileinfos = l.GetFileinfos();
	std::map<std::string,KFileInfo>::iterator i = l_fileinfos.begin();

	while(i != l_fileinfos.end())
	{
		if(GetFileAttributesA(i->first.c_str()) != INVALID_FILE_ATTRIBUTES) // 존재하면 인포를 만들자 
		{
			std::string filename(dir);
			if(filename.find_last_of('\\') +1 != filename.length())
				filename.append("\\");

			filename.append(i->first);
			fileinfos.insert(std::map<std::string,KFileInfo>::value_type(i->first, KFileInfo(filename)));
		}
		i++;
	}
	return true;
}

void KPatchInfo::Diff( KPatchInfo &l,KPatchInfo&r )
{	
	std::map<std::string,KFileInfo>::iterator iter_left = l.fileinfos.begin();

	while(iter_left != l.fileinfos.end())
	{
		std::map<std::string,KFileInfo>::iterator iter_right = r.fileinfos.find(iter_left->first);
		if(iter_right != r.fileinfos.end())
		{
			if(iter_left->second != iter_right->second)
			{
				fileinfos.insert(std::map<std::string,KFileInfo>::value_type(iter_left->first,iter_left->second));
			}
		}
		else
		{
			fileinfos.insert(std::map<std::string,KFileInfo>::value_type(iter_left->first,iter_left->second));
		}
		iter_left++;
	}
}

unsigned int KPatchInfo::GetFileSize()
{
	unsigned int totalsize = 0;
	std::map<std::string,KFileInfo>::iterator i = fileinfos.begin();
	while(i != fileinfos.end())
	{
		totalsize+=i->second.GetSize();
		i++;
	}
	return totalsize;
}

int KPatchInfo::GetFileCnt()
{
	return (int)fileinfos.size();
}


bool KPatchInfo::operator==( KPatchInfo & r )
{
	if(fileinfos.size()!=r.fileinfos.size())
		return false;

	std::map<std::string,KFileInfo>::iterator iter_left = fileinfos.begin();

	while(iter_left != fileinfos.end())
	{
		std::map<std::string,KFileInfo >::iterator iter_right = r.fileinfos.find(iter_left->first);
		if(iter_right == r.fileinfos.end())
			return false;

		if(iter_left->second != iter_right->second)
			return false;		

		iter_left++;
	}
	return true;
}

void KPatchInfo::Add(std::string key, std::string filename )
{
	std::map<std::string,KFileInfo>::iterator i = fileinfos.find(key);

	if(i!=fileinfos.end())
	{
		fileinfos.erase(i);
	}

	fileinfos.insert(std::map<std::string,KFileInfo>::value_type(key, KFileInfo(filename)));
}

bool KPatcher::Reconnect()
{
	session.Close();
	session.Disconnect();
	return session.Connect(servername);
}

bool KPatcher::Connect( std::string patchpathurl )
{
	if(patchpathurl.find_last_of('/') +1 != patchpath.length())
		patchpathurl.append("/");

	if(KHttpSession::SplitURL(patchpathurl,servername,serverurl)==false)
		return false;

	if(session.Connect(servername)==false)
		return false;


	return true;
}

char tourl(char c)
{
	if(c == '\\')
		return '/';
	return c;
}

std::string GetDir(std::string & path)
{
	std::string dirname;
	dirname = path.substr(0,path.find_last_of("\\"));
	return dirname;
}

bool CreateParentDirectory(std::string &dir)
{	
	DWORD attribute= GetFileAttributesA(dir.c_str());
	if(attribute != INVALID_FILE_ATTRIBUTES && attribute & FILE_ATTRIBUTE_DIRECTORY)	
		return true;
	
	std::string parentdir = GetDir(dir);
	attribute= GetFileAttributesA(parentdir.c_str());
	
	if(attribute != INVALID_FILE_ATTRIBUTES && attribute & FILE_ATTRIBUTE_DIRECTORY)
	{
		if(CreateDirectoryA(dir.c_str(),NULL) == 0)
		{
			return false;
		}
		return true;
	}
	return CreateParentDirectory(parentdir);
}



KPatcher::KPatcher()
:isstoped(false)
{
	char path[2048];
	GetModuleFileNameA(NULL,path,2048);
	std::string pathstr = path;
	std::transform(pathstr.begin(),pathstr.end(),pathstr.begin(),tolower);

	int last = pathstr.find_last_of('\\');
	patchername = pathstr.substr(last+1);
	patcherdir = pathstr.substr(0,last+1);

	LPSTR fileptr;	
	GetFullPathNameA(".",2048,path,&fileptr);
	localpath = path;
	std::transform(localpath.begin(),localpath.end(),localpath.begin(),tolower);
	if(localpath.find_last_of('\\') +1 != localpath.length())
		localpath.append("\\");	

	std::string tempfilename = localpath+patchername+".tmp";
	DeleteFileForce(tempfilename);

	// 레지스트리 등록을 위한 wchar 버전
	wchar_t wstrPath[2048];
	GetModuleFileNameW( NULL, wstrPath, 2048 );
	std::wstring wstrPathStr = wstrPath;
	std::transform( wstrPathStr.begin(), wstrPathStr.end(), wstrPathStr.begin(), tolower );
	last = wstrPathStr.find_last_of( L'\\' );

	LPWSTR wstrFilePtr;
	GetFullPathNameW( L".", 2048, wstrPath, &wstrFilePtr );
	m_wstrLauncherPath = wstrPath;
	std::transform( m_wstrLauncherPath.begin(), m_wstrLauncherPath.end(), m_wstrLauncherPath.begin(), tolower );
	if( m_wstrLauncherPath.find_last_of( L'\\' ) + 1 != m_wstrLauncherPath.length() )
		m_wstrLauncherPath.append( L"\\" );
	
	m_wstrLauncherPath += wstrPathStr.substr( last + 1 );
}

KPatcher::EPatcherErrorType KPatcher::ProcessOneFile(TOTAL_PROGRESS_CALLBACK totalprogress, FILE_PROGRESS_CALLBACK fileprogress, std::string& strErrorInfo)
{
	std::map<std::string,KFileInfo> &fileinfos = diff_info.GetFileinfos();
	std::map<std::string,KFileInfo>::iterator iter = fileinfos.begin();

	std::string url = serverurl + iter->first;
	std::string filename = localpath + iter->first;
	std::transform(url.begin(),url.end(),url.begin(),tourl);

	Komfile url_kom;
	Komfile local_kom;
	bool iskom = false;
	if(filename.find_last_of(".kom") == filename.length() - 1)	
		iskom = local_kom.Open(filename);	

	if( iskom == false ) // 일반파일
	{
		DeleteFileForce(filename);

		if(session.Open(url)==false)
		{
			session.Close();
			return ePatcherError_Type_Connect;
		}
		
		if(CreateParentDirectory(GetDir(filename))==false)
		{
			session.Close();
			strErrorInfo = iter->first;
			return ePatcherError_Type_FileAccess;
		}

		std::ofstream filestream(filename.c_str(),std::ios_base::binary|std::ios_base::out);
		if(filestream.is_open() == false)
		{
			session.Close();			
			strErrorInfo = iter->first;
			return ePatcherError_Type_FileAccess;
		}

		session.Write(filestream,fileprogress);
		filestream.close();		
		session.Close();			
	}
	else 
	{	
		if(url_kom.Open(&session,url)) // 콤이고 양쪽에 있는 경우 
		{
			Komfile dest_kom;
			dest_kom.LeftOuterJoin(url_kom,local_kom);

			url_kom.Close();
			local_kom.Close();

			switch(dest_kom.Save(filename,fileprogress))
			{
			case Komfile::eKomfileError_Type_FileAccess:
				dest_kom.Close();
				DeleteFileForce(filename);	
				strErrorInfo = iter->first;
				return ePatcherError_Type_FileAccess;
			case Komfile::eKomfileError_Type_InvalidFile:
				dest_kom.Close();
				DeleteFileForce(filename);				
				strErrorInfo = iter->first;
				return ePatcherError_Type_InvalidFile;
			}

			dest_kom.Close();
		}
		else
		{
			strErrorInfo = iter->first;
			return ePatcherError_Type_FileAccess;
		}
	}

	if(IsStoped())
	{
		return ePatcherError_Type_Success;
	}

	//서버로 부터 파일을 받았지만, 정상적인 파일이 아닐수도 있다.
	if(iter->second.Verify(filename)==false)
	{
		DeleteFileForce(filename);
		strErrorInfo = iter->first;
		return ePatcherError_Type_InvalidFile;
	}

	fileinfos.erase(iter);
	if(totalprogress)
	{
		totalprogress((totalsize-(float)diff_info.GetFileSize()) / totalsize);
	}
	return ePatcherError_Type_Success;
}

void KPatcher::Stop()
{
	isstoped=true;
}

bool KPatcher::Verify( std::string name )
{
	std::transform(name.begin(),name.end(),name.begin(),tolower);

	return (name == patchername && localpath == patcherdir);
}


bool KPatcher::CheckPatcherChange()
{
	std::map<std::string,KFileInfo>::iterator i = server_info.GetFileinfos().find(patchername);
	if( i== server_info.GetFileinfos().end())
	{
		return false;
	}

	KFileInfo patcher_info(localpath+patchername);
	return patcher_info != i->second;
}

#ifdef GATHER_PATCH_INFO
bool KPatcher::CheckPatcherChange(OUT unsigned int& uiAdler32,OUT unsigned int& uiSize,OUT unsigned int& uiFiletime)
{
	std::map<std::string,KFileInfo>::iterator i = server_info.GetFileinfos().find(patchername);
	if( i== server_info.GetFileinfos().end())
	{
		return false;
	}

	KFileInfo patcher_info(localpath+patchername);

	uiAdler32 = patcher_info.GetAdler32();
	uiSize = patcher_info.GetSize();
	uiFiletime = patcher_info.GetFileTime();

	return patcher_info != i->second;
}
#endif GATHER_PATCH_INFO


bool KPatcher::UpdatePatcher()
{
	std::string patcherfilename =localpath+patchername;
	std::string tempfilename = patcherfilename+".tmp";
	std::string tempfilename2 = tempfilename+".tmp";

	std::map<std::string,KFileInfo>::iterator iter = server_info.GetFileinfos().find(patchername);
	if( iter== server_info.GetFileinfos().end())
	{
		return false;
	}

	std::string url = serverurl + patchername;
	std::transform(url.begin(),url.end(),url.begin(),tourl);

	if(session.Open(url)==false)
		return false;
	
	std::ofstream filestream(tempfilename2.c_str(),std::ios_base::binary|std::ios_base::out);
	if(filestream.is_open() == false)
	{
		session.Close();
		return false;
	}
	session.Write(filestream);
	filestream.close();
	session.Close();

	if(GetFileAttributesA(patcherfilename.c_str()) == INVALID_FILE_ATTRIBUTES)
	{
		if(TRUE ==MoveFileExA(tempfilename2.c_str(),patcherfilename.c_str(),MOVEFILE_REPLACE_EXISTING))
		{			
			return true;
		}
	}

	if(TRUE == MoveFileExA(patcherfilename.c_str(),tempfilename.c_str(),MOVEFILE_REPLACE_EXISTING))
	{
		if(TRUE ==MoveFileExA(tempfilename2.c_str(),patcherfilename.c_str(),MOVEFILE_REPLACE_EXISTING))
		{			
			return true;
		}
		else
		{
			MoveFileExA(tempfilename.c_str(),patcherfilename.c_str(),MOVEFILE_REPLACE_EXISTING);
			return false;
		}
	}
	return true;
}


bool KPatcher::RunPatcher(LPSTR commandline)
{	
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	
	ZeroMemory( &pi, sizeof(pi) );

	std::string command = localpath + patchername + " " + commandline;
	
	if(TRUE == CreateProcessA(NULL,(LPSTR)command.c_str(),NULL,NULL,FALSE,0,NULL,localpath.c_str(),&si,&pi))
		return true;
	return false;
}


bool KPatcher::RunClient( std::string clientname,std::string commandline )
{
	std::map<std::string,KFileInfo>::iterator i = server_info.GetFileinfos().begin();
	std::map<std::string,KFileInfo>::iterator infoEnd = server_info.GetFileinfos().end();
	while(i != infoEnd)
	{
		if( i->first.find(clientname) != std::string::npos )
			break;

		++i;
	}

	if(i == infoEnd)
		return false;	

	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);

	ZeroMemory( &pi, sizeof(pi) );

	std::string clientpath = localpath + i->first;
	std::string workingpath = clientpath.substr(0,clientpath.find_last_of("\\"));
	std::string command = clientpath + " " + commandline;

	if(TRUE == CreateProcessA(NULL,(LPSTR)command.c_str(),NULL,NULL,FALSE,0,NULL,workingpath.c_str(),&si,&pi))
		return true;
	return false;
}

std::string KPatcher::GetFirstFileName()
{
	std::map<std::string,KFileInfo> &fileinfos = diff_info.GetFileinfos();
	if(fileinfos.empty())
		return "";
	return fileinfos.begin()->first;
}

void KPatcher::Diff()
{
	diff_info.Diff(server_info,local_info);

	std::map<std::string,KFileInfo>::iterator i = diff_info.GetFileinfos().find(patchername);
	if( i!= diff_info.GetFileinfos().end())	
		diff_info.GetFileinfos().erase(i);
	
	i = diff_info.GetFileinfos().find("elsword.exe");
	if( i!= diff_info.GetFileinfos().end())	
		diff_info.GetFileinfos().erase(i);

	totalsize= (float)diff_info.GetFileSize();
	totalCount = diff_info.GetFileCnt();
}

//#ifdef MULTI_CLIENT_OFF
DWORD KPatcher::CheckRunClient(LPCWSTR clientname)
{

	CStringW wstrTemp(clientname);
	wstrTemp.MakeUpper();

	CStringW wstrFileName;

	PROCESSENTRY32 Process32;
	HANDLE hnd;
	Process32.dwSize = sizeof(PROCESSENTRY32);

	hnd = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(Process32First(hnd, &Process32) == false)
		return FALSE;

	do 
	{
		wstrFileName = Process32.szExeFile;
		wstrFileName.MakeUpper();
		if(wstrTemp == wstrFileName)
		{
			
			wstrFileName.Empty();
			return Process32.th32ProcessID;
		}
	} while (Process32Next(hnd, &Process32));

	return FALSE;

}

BOOL CALLBACK KillProcessEnum( HWND hwnd, LPARAM lParam )
{
	DWORD dwID ;

	GetWindowThreadProcessId(hwnd, &dwID) ;

	if(dwID == (DWORD)lParam)
	{
		PostMessage(hwnd, WM_CLOSE, 0, 0) ;
	}

	return TRUE ;
}

DWORD KPatcher::KillProsessClient(DWORD dwPID, DWORD dwTimeout)
{
	HANDLE hProcess;
	DWORD dwRet;

	hProcess = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE, dwPID);

	dwRet = GetLastError();

	if(hProcess == NULL)
	{
		return KP_FAILED ;
	}

	// 해당 프로세스에 PostMessage로 WM_CLOSE 을 전송해서 프로세스를 정상 종료하도록 시도한다.
	EnumWindows((WNDENUMPROC)KillProcessEnum, (LPARAM) dwPID);

	// 종료되기를 기다렸다가 타임 아웃이면 강제 종료시킨다.
	if(WaitForSingleObject(hProcess, dwTimeout) != WAIT_OBJECT_0)
		dwRet = (TerminateProcess(hProcess,0) ? KP_SUCCESS_KILL : KP_FAILED);
	else
		dwRet = KP_SUCCESS_CLEAN;

	CloseHandle(hProcess);

	return dwRet;
}
//#endif //MULTI_CLIENT_OFF
#include <windows.h>
#include "define.h"
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


//bool KPatchInfo::Open( KPatchInfo & l,std::string dir )
bool KPatchInfo::Open( KPatchInfo & l,std::string dir, PROGRESS_CALLBACK checkFileprogress  )
{
	std::map<std::string,KFileInfo> &l_fileinfos = l.GetFileinfos();
	std::map<std::string,KFileInfo>::iterator i = l_fileinfos.begin();


	size_t uiCount = 0;

	while(i != l_fileinfos.end())
	{

		if( checkFileprogress )
		{
			++uiCount;
			checkFileprogress( (float)uiCount / (float)l_fileinfos.size() );
		}

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

	if( checkFileprogress )
		checkFileprogress( 0.f );

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
	std::string server;
	std::string url;

	if(KHttpSession::SplitURL(patchpathurl,server,url)==false)
		return false;

	if(session.Connect(server)==false)
		return false;

	int len;
	if(session.Open(url,-1,-1,&len)==false)	
		return false;

	std::stringstream stream;
	std::string patchpath;
	if(session.Write(stream)==false)
	{
		session.Disconnect();
		return false;
	}
	session.Disconnect();
	std::string patchpathstr = stream.str();

	int start = patchpathstr.find_first_of("<");
	int end = patchpathstr.find_last_of(">");

	if( start < 0 || end <0)
		return false;

	start+=1;
	patchpath = patchpathstr.substr(start,end-start);

	if(patchpath.find_last_of('/') +1 != patchpath.length())
		patchpath.append("/");
	
	if(KHttpSession::SplitURL(patchpath,servername,serverurl)==false)
		return false;

	session.Connect(servername);
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

}

bool KPatcher::ProcessOneFile(PROGRESS_CALLBACK totalprogress, PROGRESS_CALLBACK fileprogress)
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
			return false;
		}
		
		if(CreateParentDirectory(GetDir(filename))==false)
		{
			session.Close();
			return false;
		}

		std::ofstream filestream(filename.c_str(),std::ios_base::binary|std::ios_base::out);
		if(filestream.is_open() == false)
		{
			session.Close();			
			return false;
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

			if(dest_kom.Save(filename,fileprogress) == false)
			{
				dest_kom.Close();
				DeleteFileForce(filename);				
				return false;
			}
			dest_kom.Close();
		}
		else
		{
			return false;
		}
	}

	if(IsStoped())
	{
		return true;
	}

	if(iter->second.Verify(filename)==false)
	{
		DeleteFileForce(filename);
		return false;
	}

	fileinfos.erase(iter);
	if(totalprogress)
	{
		totalprogress( (float)( m_uiTotalsize - diff_info.GetFileSize() ) / (float)m_uiTotalsize );
	}
	return true;
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

#ifdef DLL_COPY_RUN_ELSWORD
	DLLCapyStart(localpath);
#endif //DLL_COPY_RUN_ELSWORD
	
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);

	ZeroMemory( &pi, sizeof(pi) );

	std::string clientpath = localpath + clientname;
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

	m_uiTotalsize = diff_info.GetFileSize();
	
}

#ifdef DLL_COPY_RUN_ELSWORD
bool KPatcher::DLLCapyStart(std::string localpath)
{
	if(localpath.find_last_of('\\') +1 != localpath.length())
		localpath.append("\\");	

	std::string supportfilesPathOrigin = localpath + "data\\supportfiles\\";
	std::string dllFileName = supportfilesPathOrigin + "*.dl-";
	std::string resurltPath = localpath + "data\\";
	std::string capyFileName = "";
	std::string supportfilesPathWork = supportfilesPathOrigin;
	std::string resurltPathWork = resurltPath;

	WIN32_FIND_DATAA wfd;
	bool bResult = true;
	

	HANDLE hSearch = FindFirstFileA(dllFileName.c_str(), &wfd);

	if ( hSearch == INVALID_HANDLE_VALUE )
		return false;

	supportfilesPathWork.append(wfd.cFileName);
	capyFileName.append(wfd.cFileName);


	std::string _strextention = ".dl-";

	std::string _strDllBeforePath = capyFileName.substr( capyFileName.size() - _strextention.size()  , _strextention.size());

	if ( _strextention == _strDllBeforePath )
	{
		dllFileName = capyFileName.substr( 0 , capyFileName.size() - _strextention.size() );
		dllFileName += ".dll";
		resurltPathWork += dllFileName;

		CopyFileA(supportfilesPathWork.c_str(), resurltPathWork.c_str(), FALSE);
	}

	

	while(bResult)
	{
				
		bResult = FindNextFileA(hSearch, &wfd);

		capyFileName = "";
		supportfilesPathWork = "";
		supportfilesPathWork = supportfilesPathOrigin;
		resurltPathWork = resurltPath;

		if ( bResult == true)
		{
			supportfilesPathWork.append(wfd.cFileName);
			capyFileName.append(wfd.cFileName);

			std::string _strDllBeforePath = capyFileName.substr( capyFileName.size() - _strextention.size()  , _strextention.size());

			if ( _strextention == _strDllBeforePath )
			{
				dllFileName = capyFileName.substr( 0 , capyFileName.size() - _strextention.size() );
				dllFileName += ".dll";
				resurltPathWork += dllFileName;

				CopyFileA(supportfilesPathWork.c_str(), resurltPathWork.c_str(), FALSE);

			}
		}

	}
	FindClose(hSearch);

	return true;
}
#endif //DLL_COPY_RUN_ELSWORD

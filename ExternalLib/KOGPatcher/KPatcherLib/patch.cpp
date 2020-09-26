
#include "patch.h"


KPatchFileInfo::KPatchFileInfo( xmlNode * node )
	:adler32(0),size(0),filetime(0),alias()
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
			else if(strcmp((char*)attribute->name,"Alias")==0)
			{
				alias = (char*)attribute->children->content;
			}
		}		
		attribute=attribute->next;
	}
}

KPatchFileInfo::KPatchFileInfo( std::string filename )
	:adler32(0),size(0),filetime(0),alias()
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

bool KPatchFileInfo::Verify(std::string filename)
{
	KPatchFileInfo info(filename);
	return *this == info;
}



bool KPatchInfo::Open( KCurlSession * session,std::string url )
{	
	
	std::stringstream stream;
	if(session->WriteData(url,stream)==false)
		return false;

	xmlDoc * doc;			
	doc = xmlReadMemory(stream.str().c_str(),stream.str().length(),"Komfiles.xml",NULL,0);	
	
	xmlNode * patchinfo = xmlDocGetRootElement(doc);
	if(patchinfo->type == XML_ELEMENT_NODE && strcmp((char*)patchinfo->name,"Patchinfo")==0)
	{		
		xmlNode * files = patchinfo->children;		

		if(files == NULL)
		{
			xmlFreeDoc(doc);
			return false;
		}

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
						fileinfos.insert(std::map<std::string,KPatchFileInfo>::value_type(key, KPatchFileInfo(file)));
					}
					file = file->next;
				}				
			}
			else if(files->type == XML_ELEMENT_NODE && strcmp((char*)files->name,"PatcherFiles")==0)
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
						patcherfiles.push_back(key);
					}
					file = file->next;
				}
			}
			else if(files->type == XML_ELEMENT_NODE && strcmp((char*)files->name,"VerifyFiles")==0)
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
						}						
						std::transform(key.begin(),key.end(),key.begin(),tolower);						
						verifyfiles.push_back(key);
					}
					file = file->next;
				}
			}

			files = files->next;
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
	std::map<std::string,KPatchFileInfo> &l_fileinfos = l.GetFileInfos();
	std::map<std::string,KPatchFileInfo>::const_iterator i = l_fileinfos.begin();

	while(i != l_fileinfos.end())
	{
		std::string filename(dir);
		if(filename.find_last_of('\\') +1 != filename.length())
			filename.append("\\");

		filename.append((*i).first);

		if(GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES) // 존재하면 인포를 만들자 
		{			
			fileinfos.insert(std::map<std::string,KPatchFileInfo>::value_type(i->first, KPatchFileInfo(filename)));
		}
		i++;
	}
	return true;
}

void KPatchInfo::Diff( KPatchInfo &l,KPatchInfo&r )
{	
	boost::mutex::scoped_lock lock(mut);

	std::map<std::string,KPatchFileInfo>::iterator iter_left = l.fileinfos.begin();

	while(iter_left != l.fileinfos.end())
	{
		std::map<std::string,KPatchFileInfo>::iterator iter_right = r.fileinfos.find(iter_left->first);
		if(iter_right != r.fileinfos.end())
		{
			if(iter_left->second != iter_right->second)
			{
				fileinfos.insert(std::map<std::string,KPatchFileInfo>::value_type(iter_left->first,iter_left->second));
			}
		}
		else
		{
			fileinfos.insert(std::map<std::string,KPatchFileInfo>::value_type(iter_left->first,iter_left->second));
		}
		iter_left++;
	}
}

unsigned int KPatchInfo::GetFileSize()
{
	boost::mutex::scoped_lock lock(mut);

	unsigned int totalsize = 0;
	std::map<std::string,KPatchFileInfo>::iterator i = fileinfos.begin();
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

	std::map<std::string,KPatchFileInfo>::iterator iter_left = fileinfos.begin();

	while(iter_left != fileinfos.end())
	{
		std::map<std::string,KPatchFileInfo >::iterator iter_right = r.fileinfos.find(iter_left->first);
		if(iter_right == r.fileinfos.end())
			return false;

		if(iter_left->second != iter_right->second)
			return false;		

		iter_left++;
	}
	return true;
}


void KPatchInfo::Erase( std::string filename)
{
	boost::mutex::scoped_lock lock(mut);
	fileinfos.erase(filename);
}



bool KPatchInfo::IsEmpty()
{
	boost::mutex::scoped_lock lock(mut);
	return fileinfos.empty();
}

KPatchFileInfo KPatchInfo::Find( std::string filename)
{
	boost::mutex::scoped_lock lock(mut);
	std::map<std::string,KPatchFileInfo>::iterator i = fileinfos.find(filename);

	if(i != fileinfos.end())
		return i->second;

	return KPatchFileInfo();
}

bool KPatchInfo::Exist( std::string filename)
{
	boost::mutex::scoped_lock lock(mut);
	std::map<std::string,KPatchFileInfo>::iterator i = fileinfos.find(filename);
	return i!=fileinfos.end();
}

bool KPatchLib::GetPatchPath( std::string patchpathurl ,NETERROR_CALLBACK error)
{
	std::stringstream stream;
	if(session.WriteData(patchpathurl,stream,0,-1,NULL,NULL,error)==false)
		return false;


	std::string patchpathstr = stream.str();
	int start = patchpathstr.find_first_of("<");
	int end = patchpathstr.find_last_of(">");

	if( start < 0 || end <0)
		return false;

	start+=1;	
	patchpath = patchpathstr.substr(start,end-start);

	if(patchpath.find_last_of('/') +1 != patchpath.length())
		patchpath.append("/");

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
	std::string dirname (path,0,path.find_last_of("\\"));
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



KPatchLib::KPatchLib()
:isstoped(false),errorcount(0),mutex(NULL),tp(1)
{
	Init();
}

KPatchLib::KPatchLib(int tpsize)
:isstoped(false),errorcount(0),mutex(NULL),tp(tpsize)
{
	Init();
}

KPatchLib::KPatchLib( int tpsize,std::string id_,std::string pw_ )
:isstoped(false),errorcount(0),id(id_),pw(pw_),session(id_,pw_),mutex(NULL),tp(tpsize)
{
	Init();
}

void KPatchLib::Init()
{
	std::locale::global(std::locale("")); 

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
}

bool KPatchLib::VerifyPatcherName( std::string name )
{
	std::transform(name.begin(),name.end(),name.begin(),tolower);

	return name == patchername;
}


bool KPatchLib::VerifyPatcherPath( std::string path )
{
	std::transform(path.begin(),path.end(),path.begin(),tolower);

	return path == localpath;
}



bool KPatchLib::Run( std::string commandline )
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);

	ZeroMemory( &pi, sizeof(pi) );	

	if(TRUE == CreateProcessA(NULL,(LPSTR)commandline.c_str(),NULL,NULL,FALSE,0,NULL,localpath.c_str(),&si,&pi))
		return true;
	return false;
}


void  KPatchLib::DownloadFilesAsync( NETSTATRT_CALLBACK start,NETPROGRESS_CALLBACK progress,NETERROR_CALLBACK error,COMPLETE_CALLBACK complete)
{
	boost::thread t(boost::bind(&KPatchLib::DownloadFiles,this,start,progress,error,complete));	
}


bool KPatchLib::DownloadFiles(NETSTATRT_CALLBACK start,NETPROGRESS_CALLBACK progress,NETERROR_CALLBACK error,COMPLETE_CALLBACK complete)
{	
    if(start)
        start();

	std::vector<std::string> & filenames = server_info.GetVerifyFiles();
	std::vector<std::string>::iterator iter = filenames.begin();
	while(iter!=filenames.end() && IsStoped() ==false)
	{
		if(server_info.Exist(*iter)==true)
		{			
			Komfile kom;			
			bool verify = true;
			if(kom.Open(localpath + *iter))			
				verify = kom.Verify();
			
			kom.Close();

			if(verify==false)
			{
				DeleteFileForce(localpath + *iter);
			}
		}
		iter++;
	}

	if(IsStoped() == false)
		GetLocalPatchInfo();

	if(IsStoped() == false)
		Diff();

	while(IsPatchComplete() == false && IsStoped() ==false)
	{	
		if(errorcount > MAX_RETRY)
		{
			if(complete)
				complete(false);
			Stop();
			return false;
		}

		std::map<std::string,KPatchFileInfo> fileinfos = diff_info.GetFileInfos();
		std::map<std::string,KPatchFileInfo>::iterator i = fileinfos.begin();

		while(i!=fileinfos.end())
		{
			std::pair<std::string,KPatchFileInfo> info =std::pair<std::string,KPatchFileInfo>(i->first,i->second);
			boost::threadpool::schedule(tp,boost::bind(&KPatchLib::DownloadFile,this,info,progress,error));
			i++;
		}		
		tp.wait();
		tp.clear();
	}

	if(complete)
		complete(true);
	return true;
}

void KPatchLib::DownloadFile( std::pair<std::string,KPatchFileInfo> info ,NETPROGRESS_CALLBACK progress,NETERROR_CALLBACK error)
{
	if(IsStoped())	
		return;	
	if(tp.size()==1) // 싱글스레드 일때만
	{
		progressinfo.SetFile(info.first,info.second.GetSize());
	}


	KCurlSession localsession(id,pw);

	std::string url = patchpath;

	if(info.second.GetAlias().length()>0)
		url.append(info.second.GetAlias());
	else
		url.append(info.first);

	std::string filename = localpath + info.first;
	std::transform(url.begin(),url.end(),url.begin(),tourl);


	Komfile url_kom;
	Komfile local_kom;
	bool iskom = false;
	if(filename.find_last_of(".kom") == filename.length() - 1)		
		iskom = local_kom.Open(filename);

	if(info.second.GetFileTime() == 0 || iskom == false ) // 일반파일
	{
		DeleteFileForce(filename);

		if(CreateParentDirectory(GetDir(filename))==false)		
		{
			if (++errorcount>MAX_RETRY)
				Stop();			
			return;	
		}


		if(session.DownloadFile(url,filename,0,-1,progress,&progressinfo,error)==false)
		{
			if (++errorcount>MAX_RETRY)
				Stop();
			return;	
		}	

		if(info.second.Verify(filename)==false)
		{
			DeleteFileForce(filename);
			progressinfo.Rollback(info.second.GetSize());
			if (++errorcount>MAX_RETRY)
				Stop();			
			return;
		}

	}
	else 
	{	
		if(url_kom.Open(&localsession,url)) // 콤이고 양쪽에 있는 경우 
		{
			Komfile dest_kom;
			dest_kom.LeftOuterJoin(url_kom,local_kom);

			url_kom.Close();
			local_kom.Close();

			if(dest_kom.Save(filename,progress,&progressinfo) == false)
			{				
				dest_kom.Close();
				DeleteFileForce(filename);		
				if (++errorcount>MAX_RETRY)
					Stop();
				return;
			}
			dest_kom.Close();

		}
		else
		{			
			if (++errorcount>MAX_RETRY)
				Stop();
			return;
		}
	}

	if(IsStoped())	
		return;	

	diff_info.Erase(info.first);
}

void KPatchLib::Diff()
{
	diff_info.Diff(server_info,local_info);

	if(diff_info.Exist(patchername))
	{
		diff_info.Erase(patchername);
	}

	progressinfo.totallength = diff_info.GetFileSize();
}



bool KPatchLib::CheckPatcherFiles()
{
	std::vector<std::string> & filenames = server_info.GetPatcherFiles();
	std::vector<std::string>::iterator i = filenames.begin();

	while(i!=filenames.end())
	{
		if(server_info.Exist(*i)==true)
        {
            KPatchFileInfo info = server_info.Find(*i);
            KPatchFileInfo info2(localpath+(*i));

            if(info != info2)            
			    patcherfiles.push_back(*i);
        }
		i++;
	}
	
    if(patcherfiles.empty() == true)
        return false;

    return true;
}

bool KPatchLib::DownloadPatcherFiles( NETERROR_CALLBACK error)
{
	std::vector<std::string>::iterator i = patcherfiles.begin();	
	while(i!=patcherfiles.end())
	{
		if(DownloadPatcherFile(*i,error)==false)
			return false;
		i++;
	}
	return true;
}



void KPatchLib::ClearPatcherFiles(  )
{
	std::vector<std::string> &filenames = server_info.GetPatcherFiles();
	std::vector<std::string>::iterator i = filenames.begin();	
	while(i!=filenames.end())
	{
		DeleteFileForce(localpath + *i + ".tmp");			
		i++;
	}
}



bool KPatchLib::DownloadPatcherFile(std::string filename,NETERROR_CALLBACK error)
{
	std::string localfilename =localpath+filename;
	std::string tempfilename = localfilename+".tmp";
	std::string tempfilename2 = tempfilename+".tmp";

	if( false== server_info.Exist(filename))
	{
		return false;
	}

	KPatchFileInfo info =server_info.Find(filename);

	std::string url = patchpath;

	if(info.GetAlias().length()>0)
		url.append(info.GetAlias());
	else
		url.append(filename);

	std::transform(url.begin(),url.end(),url.begin(),tourl);

	if(session.DownloadFile(url,tempfilename2,0,-1,NULL,NULL,error)==false)
	{
		return false;
	}	
	
	if(GetFileAttributesA(localfilename.c_str()) == INVALID_FILE_ATTRIBUTES)
	{
		if(TRUE ==MoveFileExA(tempfilename2.c_str(),localfilename.c_str(),MOVEFILE_REPLACE_EXISTING))
		{			
			return true;
		}
	}

	if(TRUE == MoveFileExA(localfilename.c_str(),tempfilename.c_str(),MOVEFILE_REPLACE_EXISTING))
	{
		if(TRUE ==MoveFileExA(tempfilename2.c_str(),localfilename.c_str(),MOVEFILE_REPLACE_EXISTING))
		{			
			return true;
		}
		else
		{
			MoveFileExA(tempfilename.c_str(),localfilename.c_str(),MOVEFILE_REPLACE_EXISTING);
			return false;
		}
	}
	return true;
}

bool KPatchLib::GetBooleanFromDat( std::string url ,bool &re,NETERROR_CALLBACK error)
{
	std::stringstream stream;
	if(session.WriteData(url,stream,0,-1,NULL,NULL,error) == false)
		return false;

	std::string str = stream.str();

	std::transform(str.begin(),str.end(),str.begin(),tolower);
	
	int start = str.find_first_of("<");
	int end = str.find_last_of(">");

	if(start<0 || end<=start)
		return false;

	std::string str2 = str.substr(start,start-end);

	if(str2=="true")
		re =true;
	else 
		re = false;

	
	return true;
}


bool KPatchLib::CreateMutex( std::wstring str )
{
	mutex = ::CreateMutex( NULL, FALSE, str.c_str() );
	if ( ( NULL == mutex ) || ( ERROR_ALREADY_EXISTS == ::GetLastError() ) )
	{
		return false;
	}
	return true;
}

void KPatchLib::ReleaseMutex()
{
	if ( NULL != mutex )
	{
		::ReleaseMutex( mutex );
		::CloseHandle( mutex );
	}
}

KPatchLib::~KPatchLib()
{	
}

void KPatchLib::Stop()
{
	progressinfo.isstoped = true;
	tp.wait();
}

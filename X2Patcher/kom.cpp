
#include "kom.h"
#include "adler32.h"

#include "patch.h"
extern KPatcher g_patcher;


bool DeleteFileForce( std::string filename )
{
	if(DeleteFileA(filename.c_str()) == 0)
	{
		if(GetLastError() == ERROR_ACCESS_DENIED)
		{
			if(GetFileAttributesA(filename.c_str()) & FILE_ATTRIBUTE_READONLY)
			{
				SetFileAttributesA(filename.c_str(),FILE_ATTRIBUTE_NORMAL);
				if(DeleteFileA(filename.c_str()) != 0)				
					return true;
			}
		}
		else if(GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			return true;
		}
	}
	else
	{
		return true;
	}
	return false;
}

KSubfile::KSubfile( std::string str,std::ifstream & stream,int headersize)
	:filename(str),algorithm(infilate),adler32(0),size(0),compressedsize(0),filetime(0),offset(0),connection(NULL),iscalcadler(false)
{
	stream.read((char*)&size,4);
	stream.read((char*)&compressedsize,4);
	stream.read((char*)&offset,4);
	offset+=headersize;	
	//adler32 = AdlerCheckSum::adler32(stream,offset,compressedsize);
	// 에이들러 만들지 말자
	filetime =0;
}

KSubfile::KSubfile( std::string str,xmlNode * node,int * offset )
	:filename(str),algorithm(infilate),adler32(0),size(0),compressedsize(0),filetime(0),offset(0),connection(NULL),iscalcadler(true)
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
			else if(strcmp((char*)attribute->name,"CompressedSize")==0)
			{
				sscanf((char*)attribute->children->content,"%d",&compressedsize);
			}
			else if(strcmp((char*)attribute->name,"Size")==0)
			{
				sscanf((char*)attribute->children->content,"%d",&size);
			}
			else if(strcmp((char*)attribute->name,"Algorithm")==0)
			{
				sscanf((char*)attribute->children->content,"%d",&algorithm);
			}
			else if(strcmp((char*)attribute->name,"FileTime")==0)
			{
				sscanf((char*)attribute->children->content,"%x",&filetime);
			}
		}		
		attribute=attribute->next;
	}

	this->offset = (*offset);
	(*offset)+=compressedsize;
}

KSubfile::KSubfile( KHttpSession * con,std::string url,xmlNode * node,int * offset )
	:filename(url),algorithm(infilate),adler32(0),size(0),compressedsize(0),filetime(0),offset(0),connection(con),iscalcadler(true)
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
			else if(strcmp((char*)attribute->name,"CompressedSize")==0)
			{
				sscanf((char*)attribute->children->content,"%d",&compressedsize);
			}
			else if(strcmp((char*)attribute->name,"Size")==0)
			{
				sscanf((char*)attribute->children->content,"%d",&size);
			}
			else if(strcmp((char*)attribute->name,"Algorithm")==0)
			{
				sscanf((char*)attribute->children->content,"%d",&algorithm);
			}
			else if(strcmp((char*)attribute->name,"FileTime")==0)
			{
				sscanf((char*)attribute->children->content,"%x",&filetime);
			}
		}		
		attribute=attribute->next;
	}
	this->offset = (*offset);
	(*offset)+=compressedsize;	
}



bool KSubfile::WriteCompressed( std::ostream &stream )
{
	if(connection)
	{

		if(connection->Open(filename,offset,compressedsize)==false)
			return false;
		connection->Write(stream,compressedsize,NULL);
		connection->Close();
		return true;
	}
	else
	{
		std::ifstream filestream(filename.c_str(),std::ios_base::in | std::ios_base::binary);

		filestream.seekg(offset,std::ios_base::beg);

		int totallen = compressedsize;
		if(totallen <0)
			return false;

		unsigned char buffer[2048];
		int len = 2048;
		while (totallen > 0)
		{
			if (totallen < 2048)
			{
				len = totallen;
			}
			else 
			{
				len = 2048;
			}

			filestream.read((char*)buffer,len);		
			stream.write((char*)buffer,len);
			totallen -= len;
		}	

		filestream.close();
	}
	
	return true;
}



Komfile::KOM_TYPE Komfile::CheckKom( std::string filename )
{
	std::ifstream filestream(filename.c_str(),std::ios_base::in | std::ios_base::binary);
	if(filestream.is_open())
	{
		char magicword[52];
		filestream.read(magicword,52);
		// 옛날 버전 콤 처리
		filestream.close();
		if(strcmp(magicword,"KOG GC TEAM MASSFILE V.0.3.")==0)
		{
			return CUR_KOM;
		}
		else if(strcmp(magicword,"KOG GC TEAM MASSFILE V.0.2.")==0 || strcmp(magicword,"KOG GC TEAM MASSFILE V.0.1.")==0)
		{
			return OLD_KOM;
		}		
	}
	return NOT_KOM;
}

bool ReadInFile(std::ifstream &filestream,char*buf,int size)
{
	int pos = filestream.tellg();
	filestream.read(buf,size);
	if((int)filestream.tellg()-pos == size)	
		return true;
	
	filestream.close();
	return false;
}

bool Komfile::Open( std::string filename )
{
	Close();
	if(GetFileAttributesA(filename.c_str()) == INVALID_FILE_ATTRIBUTES)	
		return false;
	
	std::ifstream filestream(filename.c_str(),std::ios_base::in | std::ios_base::binary);
	if(filestream.is_open())
	{
		int pos = filestream.tellg();
		char magicword[52];

		if(ReadInFile(filestream,magicword,52)==false) return false;

		unsigned int size;
		BOOL compressed;

		if(ReadInFile(filestream,(char*)&size,4)==false) return false;
		if(ReadInFile(filestream,(char*)&compressed,4)==false) return false;

		if(strcmp(magicword,"KOG GC TEAM MASSFILE V.0.3.")==0)
		{
			if(ReadInFile(filestream,(char*)&filetime,4)==false) return false;
			if(ReadInFile(filestream,(char*)&adler32,4)==false) return false;
			if(ReadInFile(filestream,(char*)&headersize,4)==false) return false;

			char * header = new char[headersize+1];	

			if(ReadInFile(filestream,header,headersize)==false) 
			{
				delete header;
				return false;
			}
			header[headersize]='\0';

			xmlDoc * doc;			
			doc = xmlReadMemory(header,headersize,"Komfiles.xml",NULL,0);	
			delete header;

			if(!doc) 
			{
				filestream.close();
				return false;
			}

			int offset =  HEADEROFFSET + headersize;
			xmlNode * files = xmlDocGetRootElement(doc);
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
						subfiles.insert(std::map<std::string,KSubfile>::value_type(key, KSubfile(filename,file,&offset)));
					}
					file = file->next;
				}
			}			
			xmlFreeDoc(doc);
		}
		else if(strcmp(magicword,"KOG GC TEAM MASSFILE V.0.2.")==0 || strcmp(magicword,"KOG GC TEAM MASSFILE V.0.1.")==0)
		{	
			
			for (unsigned int i = 0; i < size; i++)
			{
				char key [60];
				if(ReadInFile(filestream,key,60)==false) return false;
				subfiles.insert(std::map<std::string,KSubfile>::value_type(key,KSubfile(filename,filestream,60 + size * 72)));
			}
		}
		else
		{
			filestream.close();	
			return false;
		}		
	}
	filestream.close();	
	return true;
}


bool Komfile::Open(KHttpSession * con, std::string url )
{
	Close();

	if(con->Open(url)==false)
		return false;

	
	char magicword[52];

	if(con->ReadFile((LPVOID)magicword,52)!=52)
	{		
		con->Close();
		return false;
	}
	
	bool re = true;

	unsigned int size;
	unsigned int compress ;

	if(con->ReadFile((LPVOID)&size,4)!=4) {con->Close();return false;}
	if(con->ReadFile((LPVOID)&compress,4)!=4) {con->Close();return false;}
	
	if(strcmp(magicword,"KOG GC TEAM MASSFILE V.0.3.")==0)
	{
		if(con->ReadFile((LPVOID)&filetime,4)!=4) {con->Close();return false;}
		if(con->ReadFile((LPVOID)&adler32,4)!=4) {con->Close();return false;}
		if(con->ReadFile((LPVOID)&headersize,4)!=4) {con->Close();return false;}

		char * header = new char[headersize+1];
		
		if(con->ReadFile((LPVOID)header,headersize)!=headersize)
		{
			delete [] header;
			con->Close();
			return false;
		}
		
		header[headersize]='\0';
		xmlDoc * doc;			
		doc = xmlReadMemory(header,headersize,"Komfiles.xml",NULL,0);			
		delete [] header;

		int offset =  HEADEROFFSET + headersize;
		xmlNode * files = xmlDocGetRootElement(doc);
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
					subfiles.insert(std::map<std::string,KSubfile>::value_type(key, KSubfile(con,url,file,&offset)));
				}
				file = file->next;
			}
		}			
		xmlFreeDoc(doc);
	}
	else
	{		
		re= false;
	}		

	con->Close();
	return re;
}



bool Komfile::Save( std::string filename, PROGRESS_CALLBACK progress)
{
	std::string tmpfilename = filename + ".tmp";
	std::ofstream file(tmpfilename.c_str(),std::ios_base::binary|std::ios_base::out);

	if(!file.is_open())
		return false;

	char magicword [52] = "KOG GC TEAM MASSFILE V.0.3.";	
	unsigned int filetime = GetFileTime();

	std::stringstream stream;
	stream << "<?xml version=\"1.0\"?><Files>";
	std::map<std::string,KSubfile>::iterator i=subfiles.begin();

	float totalsize = 0;
	while(i!=subfiles.end())
	{
		stream << "<File";
		stream << " Name=\"" << i->first << "\"";
		stream << " Size=\"" << i->second.GetSize() << "\"";
		stream << " CompressedSize=\"" << i->second.GetCompressedSize() << "\"";

		char adler32string[16];
		sprintf(adler32string, "%08x", i->second.GetAdler32());

		char filetimestring[16];
		sprintf(filetimestring, "%08x", i->second.GetFileTime());

		stream << " Checksum=\"" << adler32string << "\"";
		stream << " FileTime=\"" << filetimestring << "\"";
		stream << " Algorithm=\"" << i->second.GetAlgorithm() << "\"";
		stream << " />";

		totalsize += i->second.GetSize();
		i++;
	}
	stream << "</Files>";

	unsigned int len = stream.tellp();

	adler32 = AdlerCheckSum::adler32(stream,0,len,true);

	unsigned int size = subfiles.size();
	unsigned int compress = 1;

	file.write(magicword,52);
	file.write((char*)&size,4);
	file.write((char*)&compress,4);
	file.write((char*)&filetime,4);
	file.write((char*)&adler32,4);
	file.write((char*)&len,4);
	
	file << stream.str() ;
	
	
	stream.clear();

	float writesize = 0;
	
	if(progress)
		progress(0.0f);

	i=subfiles.begin();	
	while(i!=subfiles.end())
	{
		i->second.WriteCompressed(file);	
		writesize+=i->second.GetSize();
		i++;	
		if(progress)
		{
			if(progress(writesize / totalsize) == false)
			{
				file.close();
				DeleteFileForce(tmpfilename);
				return true;
			}
		}		
	}
	file.close();

	// 검증 하고

	if(Komfile::Verify(tmpfilename)==false) // 실패하면 
	{
		DeleteFileForce(tmpfilename);
		return false;
	}

	// 성공하면 

	// 이름 바꾸자

	if(GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES)
	{
		if(DeleteFileForce(filename) == false )
			return false;
	}
	
	if(MoveFileA(tmpfilename.c_str(),filename.c_str()))
	{
		if(progress)
			progress(1.0f);
		return true;
	}	
	return false;
}


void Komfile::Close()
{
	subfiles.clear();

	filetime = 0;
	adler32 = 0;
	headersize = 0;		
}


unsigned int Komfile::GetFileTime()
{
	unsigned int filetime= 0;
	std::map<std::string,KSubfile>::iterator i=subfiles.begin();
	while(i!=subfiles.end())
	{
		filetime+=i->second.GetFileTime();
		i++;
	}
	return filetime;
}

bool Komfile::Verify( std::string filename )
{
	Komfile kom;
	if(kom.Open(filename) == false)
		return false;

	return kom.Verify();
}

bool Komfile::Verify( KHttpSession * session , std::string filename )
{
	Komfile kom;
	if(kom.Open(session,filename) == false)
		return false;

	return kom.Verify();
}

bool Komfile::Verify()
{
	std::map<std::string,KSubfile>::iterator i=subfiles.begin();
	while(i!=subfiles.end())
	{
		if(i->second.Verify() == false)
			return false;
		i++;
	}
	return true;
}


bool KSubfile::Verify()
{
	std::stringstream stream;

	if(WriteCompressed(stream)==false)
		return false;

	if(AdlerCheckSum::adler32(stream,0,compressedsize)!=adler32)
		return false;

	return true;
}

unsigned int KSubfile::GetAdler32()
{	
	if(iscalcadler == false)
	{
		std::stringstream stream;
		WriteCompressed(stream);
		adler32 = AdlerCheckSum::adler32(stream,0,compressedsize);
		iscalcadler = true;
	}
	return adler32;
}


bool Komfile::LeftOuterJoin( Komfile &left, Komfile &right )
{
	std::map<std::string,KSubfile>::iterator iter_left = left.subfiles.begin();
	while(iter_left!= left.subfiles.end())
	{
		std::map<std::string,KSubfile>::iterator iter_right = right.subfiles.find(iter_left->first);

		if( iter_right != right.subfiles.end() )// 오른쪽에도 있는 경우만 고른다 
		{
			// 만약에 오른쪽 서브파일과 왼쪽이 똑같은 경우 오른쪽을 쓴다 
			// 예외적으로 오른쪽 파일 타임이 0 인 경우 - 이 경우는 구버전 콤이다 - 파일타임은 체크하지 않는다

			KSubfile subfile(iter_left->second);
			iter_right->second.GetAdler32();
			if(iter_left->second == iter_right->second)
			{
				// 실제 파일은 있던 것을 써도 시간은 갱신을 합니다.
				unsigned int uiTime = iter_left->second.GetFileTime();
				subfile = iter_right->second;
				subfile.SetFileTime( uiTime );
			}
			subfiles.insert(std::map<std::string,KSubfile>::value_type(iter_left->first,subfile));
		}
		else
		{
			// Left Outer Join 인 경우 Right 에 없더라도 저장하자 
			subfiles.insert(std::map<std::string,KSubfile>::value_type(iter_left->first,iter_left->second));
		}
		iter_left++;
	}
	return true;
}

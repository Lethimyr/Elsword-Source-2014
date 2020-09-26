#include "stdafx.h"
#include "kom.h"
#include "adler32.h"
#include "blowfishcbc.h"

bool IsFileExists( std::string filename )
{
	if(_access( filename.c_str(), 0 ) !=-1)
		return true;
	return false;
}

void ResetReadOnly(std::string path)
{		
	if(IsFileExists(path))
	{
		_chmod(path.c_str(),_S_IWRITE);
	}
}

bool DeleteFileForce( std::string filename )
{
	if(DeleteFileA(filename.c_str()) == 0)
	{
		if(GetLastError() == ERROR_ACCESS_DENIED)
		{
			ResetReadOnly(filename);
			if(DeleteFileA(filename.c_str()) != 0)				
				return true;
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
	:filename(str),algorithm(infilate),adler32(0),size(0),compressedsize(0),filetime(0),offset(0),session(NULL),iscalcadler(false)
{
	stream.read((char*)&size,4);
	stream.read((char*)&compressedsize,4);
	stream.read((char*)&offset,4);
	offset+=headersize;		
	filetime =0;
}

KSubfile::KSubfile( std::string str,xmlNode * node,int * offset )
	:filename(str),algorithm(infilate),adler32(0),size(0),compressedsize(0),filetime(0),offset(0),session(NULL),iscalcadler(true)
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

KSubfile::KSubfile( KWriterSession * con,std::string url,xmlNode * node,int * offset )
	:filename(url),algorithm(infilate),adler32(0),size(0),compressedsize(0),filetime(0),offset(0),session(con),iscalcadler(true)
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
	if(session)		
		return session->WriteData(filename,stream,offset,compressedsize,NULL,NULL,NULL);	
		
	std::ifstream filestream(filename.c_str(),std::ios_base::in | std::ios_base::binary);
	filestream.seekg(offset,std::ios_base::beg);

	int totallen = compressedsize;
	if(totallen <0)
		return false;

	char buffer[2048];
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

		filestream.read(buffer,len);		
		stream.write(buffer,len);
		totallen -= len;
	}	
	filestream.close();
	return true;
}

bool KSubfile::WriteCompressed( char * data )
{
	if(session)	
	{
		std::stringstream stream;
		if(session->WriteData(filename,stream,offset,compressedsize,NULL,NULL,NULL)==false)
			return false;

		stream.read(data,compressedsize);				
	}
	else
	{
		std::ifstream filestream(filename.c_str(),std::ios_base::in | std::ios_base::binary);
		filestream.seekg(offset,std::ios_base::beg);
		filestream.read(data,compressedsize);		
	}
	
	return false;
}

bool KSubfile::WriteDecompressed( std::ostream &stream )
{

	char * dest = new char [size];
	if(WriteDecompressed(dest) == false)
	{
		delete [] dest;
		return false;
	}
	stream.write(dest,size);
	delete [] dest;
	return true;
}

bool KSubfile::WriteDecompressed( char * data )
{

	 char * src = new char [compressedsize];				
	WriteCompressed(src);

	if(algorithm == infiateandblowfish)
		Decrypt((unsigned char*)src,compressedsize);

	unsigned long ulsize= size;
	if(Z_OK != uncompress((unsigned char *)data,&ulsize,(unsigned char*)src,compressedsize))
	{
		delete [] src;
		return false;
	}
	delete [] src;
	
	return true;
}



bool KSubfile::Decrypt( unsigned char* data,int len )
{
	if(len >= 8)
	{
		if (len % 8 != 0)
		{
			len -= len % 8;
		}

		CBlowFish blowfish((unsigned char*)KOM_ENCRYPT_KEY,KOM_ENCRYPT_SIZE);
		blowfish.Decrypt((unsigned char*)data,size);	

		return true;
	}
	return false;
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

			char * header = new char[headersize];

			if(ReadInFile(filestream,header,headersize)==false) 
			{
				delete [] header;
				return false;
			}		

			DecryptHeader(header,headersize);

			xmlDoc * doc;			
			doc = xmlReadMemory(header,headersize,"Komfiles.xml",NULL,0);	
			delete [] header;

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
								std::transform(key.begin(),key.end(),key.begin(),tolower);
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


bool Komfile::Open(KWriterSession * session, std::string url )
{
	Close();

	std::stringstream stream;

	if(session->WriteData(url,stream,0,72,NULL,NULL,NULL) == false)
		return false;

	bool re = true;
	unsigned int size;
	unsigned int compress ;
	char magicword[52];

	stream.read(magicword,52);
	stream.read((char *)&size,4);
	stream.read((char *)&compress,4);	
	
	if(strcmp(magicword,"KOG GC TEAM MASSFILE V.0.3.")!=0)
		return false;
	
	stream.read((char *)&filetime,4);
	stream.read((char *)&adler32,4);	
	stream.read((char *)&headersize,4);	

	std::stringstream headerstream;
	
	if(session->WriteData(url,headerstream,72,headersize,NULL,NULL,NULL) == false)
		return false;
	
	xmlDoc * doc;	

	char * header = new char[headersize];	
	headerstream.seekg(0);
	headerstream.read(header,headersize);
	DecryptHeader(header,headersize);
	doc = xmlReadMemory(header,headersize,"Komfiles.xml",NULL,0);
	delete [] header;

	if(!doc)
		return false;

	int offset =  HEADEROFFSET + headersize;
	xmlNode * files = xmlDocGetRootElement(doc);
	if(!files)
	{
		xmlFreeDoc(doc);
		return false;
	}

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
						std::transform(key.begin(),key.end(),key.begin(),tolower);
						break;							
					}
					attribute = attribute->next;
				}						
				subfiles.insert(std::map<std::string,KSubfile>::value_type(key, KSubfile(session,url,file,&offset)));
			}
			file = file->next;
		}
	}			
	xmlFreeDoc(doc);
	
	return true;
}



bool Komfile::Save( std::string filename,NETPROGRESS_CALLBACK progress,SProgressInfo* progressinfo )
{
	std::string tmpfilename = filename + ".tmp";
	std::ofstream file(tmpfilename.c_str(),std::ios_base::binary|std::ios_base::out);

	if(file.is_open() == false)	
	{	
		ResetReadOnly(filename);
		int end = filename.find_last_of("\\");
		if(end>0)
		{
			ResetReadOnly(filename.substr(0,end));			
		}
		file.open(filename.c_str(),std::ios_base::binary|std::ios_base::out);
	}

	if(file.is_open() == false)	
	{
		return false;
	}

	char magicword [52] = "KOG GC TEAM MASSFILE V.0.3.";	
	unsigned int filetime = GetFileTime();

	std::stringstream stream;
	stream << "<?xml version=\"1.0\"?><Files>";
	std::map<std::string,KSubfile>::iterator i=subfiles.begin();

	
	while(i!=subfiles.end())
	{
		char tmp[1024];
		sprintf(tmp,"<File Name=\"%s\" Size=\"%d\" CompressedSize=\"%d\" Checksum=\"%08x\" FileTime=\"%08x\" Algorithm=\"%d\" />",
			i->first.c_str(),i->second.GetSize(),i->second.GetCompressedSize(),i->second.GetAdler32(),i->second.GetFileTime(),i->second.GetAlgorithm());		
		stream <<tmp;
		i++;
	}
	stream << "</Files>";

	
	headersize = stream.tellp();
	if (headersize % 8 != 0)
		headersize += 8 - (headersize % 8);

	char * header = new char[headersize];
	ZeroMemory(header,headersize);

	stream.seekg(0);
	stream.read(header,headersize);
	EncryptHeader(header,headersize);
	adler32 = AdlerCheckSum::adler32((unsigned char*)header,headersize);

	unsigned int size = subfiles.size();
	unsigned int compress = 1;	

	file.write(magicword,52);
	file.write((char*)&size,4);
	file.write((char*)&compress,4);
	file.write((char*)&filetime,4);
	file.write((char*)&adler32,4);
	file.write((char*)&headersize,4);
	file.write(header,headersize);
	
	delete [] header;

	int write = 72+headersize;
	
	if(progressinfo)		
		progressinfo->Read(write);

	if(progressinfo)
	{
		if(progressinfo->isstoped)
		{
			file.close();
			return true;
		}
	}

	if(progress)	
		progress(progressinfo);			

	stream.clear();

	i=subfiles.begin();	
	while(i!=subfiles.end())
	{
		i->second.WriteCompressed(file);
		write+=i->second.GetCompressedSize();

		if(progressinfo)		
		{
			if(progressinfo->isstoped)
			{
				file.close();
				return true;
			}
			progressinfo->Read(i->second.GetCompressedSize());
		}
		
		if(progress)		
			progress(progressinfo);
		i++;
	}
	file.close();

	// 검증 하고

	if(Komfile::Verify(tmpfilename)==false) // 실패하면 
	{
		DeleteFileForce(tmpfilename);
		if(progressinfo)
			progressinfo->Rollback(write);
		return false;
	}

	// 성공하면 

	// 이름 바꾸자

	if(GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES)
	{
		if(DeleteFileForce(filename) == false )
		{
			if(progressinfo)
				progressinfo->Rollback(write);
			return false;
		}
	}

	if(MoveFileA(tmpfilename.c_str(),filename.c_str()))
	{		
		return true;
	}	

	if(progressinfo)
		progressinfo->Rollback(write);
	return false;
}


void Komfile::Close()
{
	subfiles.clear();

	filetime = 0;
	adler32 = 0;
	headersize = 0;		
}


unsigned int Komfile::GetTotalSize()
{
	unsigned int totalsize= 0;
	std::map<std::string,KSubfile>::iterator i=subfiles.begin();
	while(i!=subfiles.end())
	{
		totalsize+=i->second.GetSize();
		i++;
	}
	return totalsize;
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

bool Komfile::Verify( KWriterSession * session , std::string filename )
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
				subfile = iter_right->second;
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

bool Komfile::DecryptHeader( char * header ,int size)
{
	if(header[0]=='<'&&
		header[1]=='?'&&
		header[2]=='x'&&
		header[3]=='m'&&
		header[4]=='l'
		)
	{
		return false;
	}	

	CBlowFish blowfish((unsigned char*)KOM_ENCRYPT_KEY,KOM_ENCRYPT_SIZE);
	blowfish.Decrypt((unsigned char*)header,size);	
	return true;
}

bool Komfile::EncryptHeader( char * header,int size )
{
	if(header[0]=='<'&&
		header[1]=='?'&&
		header[2]=='x'&&
		header[3]=='m'&&
		header[4]=='l'
		)
	{
		CBlowFish blowfish((unsigned char*)KOM_ENCRYPT_KEY,KOM_ENCRYPT_SIZE);
		blowfish.Encrypt((unsigned char*)header,size);	

		return true;
	}	
	
	return false;
}

void Komfile::Add( Komfile &right )
{
	std::map<std::string,KSubfile>::iterator iter_right = right.subfiles.begin();
	while(iter_right!= right.subfiles.end())
	{
		if(subfiles.find(iter_right->first)== subfiles.end())
		{
			subfiles.insert(std::map<std::string,KSubfile>::value_type(iter_right->first,iter_right->second));
		}
		iter_right++;
	}	
}

KSubfile * Komfile::GetSubFile( std::string filename )
{
	transform(filename.begin(),filename.end(),filename.begin(),tolower);
	std::map<std::string,KSubfile>::iterator i = subfiles.find(filename);
	if(i == subfiles.end())
		return NULL;

	return &i->second;
}
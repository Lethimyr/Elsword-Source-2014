#pragma once

#include "stdafx.h"
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "adler32.h"

#define KOM_ENCRYPT_KEY "gpfrpdlxm"
#define KOM_ENCRYPT_SIZE 9

#define HEADEROFFSET 72

extern void ResetReadOnly(std::string path);
extern bool DeleteFileForce( std::string filename );
extern bool IsFileExists( std::string filename );


struct SProgressInfo
{
	SProgressInfo()
		:totallength(0),totalreadlength(0),currentlength(0),currentreadlength(0),isstoped(false)
	{}	

	void Read(int size)
	{
		currentreadlength+=size;
		totalreadlength+=size;
	}

	void Rollback(int size)
	{
		currentreadlength-=size;
		totalreadlength-=size;
	}

	void SetFile(std::string filename,int size)
	{
		currentfilename = filename;
		currentlength = size;
		currentreadlength = 0;
	}

	int totallength;
	int totalreadlength;

	std::string currentfilename;
	int currentlength;
	int currentreadlength;

	bool isstoped;
};

#define NETSTATRT_CALLBACK boost::function0<void>
#define NETPROGRESS_CALLBACK boost::function1<bool,SProgressInfo *>
#define NETERROR_CALLBACK boost::function3<void,std::string,int,int>
#define COMPLETE_CALLBACK boost::function1<void,bool>

class KWriterSession
{
public:
	virtual bool WriteData(std::string url,std::ostream &stream,int offset,int length,NETPROGRESS_CALLBACK progress ,SProgressInfo * progressinfo,NETERROR_CALLBACK error) = 0;
};

class KSubfile
{
public:
	enum EAlgorithm
	{		
		infilate = 0,
		infiateandblowfish = 1
	};
	
	KSubfile(std::string filename,std::ifstream & stream,int headersize); // 구 버전 콤에서 읽는 함수	
	KSubfile(std::string filename,xmlNode * node,int * offset); // 신버전 	
	KSubfile(KWriterSession * con,std::string filename,xmlNode * node,int * offset); // URL 신버전
	

	KSubfile(const KSubfile &src)
		:filename(src.filename),adler32(src.adler32),size(src.size),compressedsize(src.compressedsize),algorithm(src.algorithm),offset(src.offset),
		session(src.session),iscalcadler(src.iscalcadler)
	{
		if(src.filetime != 0)
			filetime = src.filetime;
	}
	~KSubfile(){};

	bool operator == (const KSubfile &r)
	{
		if(r.filetime==0)		
			return (adler32 == r.adler32 && algorithm == r.algorithm &&	compressedsize == r.compressedsize );
		
		return (adler32 == r.adler32 && algorithm == r.algorithm &&	compressedsize == r.compressedsize && filetime == r.filetime);		
	}

	KSubfile & operator= (const KSubfile &r)
	{		
		adler32 = r.adler32;
		algorithm = r.algorithm;
		compressedsize = r.compressedsize;		
		session = r.session;
		filename = r.filename;
		size = r.size;
		offset = r.offset;
		iscalcadler = r.iscalcadler;

		if(r.filetime != 0)
			filetime = r.filetime;
		return *this;
	}

	bool WriteCompressed(std::ostream &stream);
	bool WriteDecompressed(std::ostream &stream);

	bool WriteCompressed( char * data);
	bool WriteDecompressed( char * data);


	bool Verify();


	unsigned int GetFileTime(){return filetime;}
	void SetFileTime(unsigned int ft){filetime = ft;} // 주의해서 잘 써야한다.
	unsigned int GetAdler32();
	unsigned int GetSize(){return size;}
	unsigned int GetCompressedSize(){return compressedsize;}
	int GetAlgorithm(){return algorithm;}

private:

	std::string filename;
	unsigned int filetime;
	unsigned int adler32;
	unsigned int size;
	unsigned int compressedsize;
	int algorithm;

	unsigned int offset;
	KWriterSession * session;
	bool iscalcadler;	

	bool Decrypt(unsigned char* data,int len);
};


class Komfile
{
public:	

	enum KOM_TYPE
	{
		NOT_KOM = -1,
		OLD_KOM = 0,
		CUR_KOM = 1
	};

	static KOM_TYPE CheckKom(std::string filename);
	static bool Verify(std::string filename);
	static bool Verify(KWriterSession * session , std::string filename);
	
	
	Komfile(){Close();};
	Komfile(const Komfile& r)
		:subfiles(r.subfiles),filetime(r.filetime),adler32(r.adler32),headersize(r.headersize)
	{};

	~Komfile(){Close();};

	

	Komfile & operator = (const Komfile & r)
	{
		subfiles = r.subfiles;
		filetime = r.filetime;
		adler32 = r.adler32;
		headersize = r.headersize;

		return *this;
	}

	// 조인 함수들 
	// 왼쪽 우선으로 조인한다
	// 사용시 왼쪽에 URL 을 오른쪽에 파일을 넣기 바람

	bool LeftOuterJoin(Komfile &left, Komfile &right);
	void Add(Komfile &r);	
	
	bool Open(KWriterSession * con,std::string url);
	bool Open(std::string filename);	
	bool Save(std::string filename,NETPROGRESS_CALLBACK progress = NULL,SProgressInfo * progressinfo=NULL);	
	void Close();

	unsigned int GetFileTime();
	unsigned int GetAdler32(){return adler32;}
	unsigned int GetHeaderSize(){return headersize;}
	unsigned int GetTotalSize();

	bool Verify();

	KSubfile * GetSubFile(std::string filename);
private:

	bool DecryptHeader(char * header,int size);
	bool EncryptHeader(char * header,int size);
	
	std::map<std::string,KSubfile> subfiles;
	unsigned int filetime;
	unsigned int adler32;
	unsigned int headersize;
};



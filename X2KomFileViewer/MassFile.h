#pragma once

#define MASSFILE_VERSION		(1)
#define MASSFILE_NAMESIZE		(60)
#define MASSFILE_HEADER_SIZE	(52)

#define MFI_REALDATA			(1)
#define MFI_NODECOMPRESS		(2)

class CMassFile
{
	struct MASSFILE_HEADER
	{
		char strVersionInfo[MASSFILE_HEADER_SIZE];
		UINT iTotalFileNo;
		BOOL bCompressed;

		MASSFILE_HEADER()
		{
			ZeroMemory( strVersionInfo, sizeof(char)*MASSFILE_HEADER_SIZE );
			iTotalFileNo = 0;
			bCompressed  = FALSE;
		}
	};
	struct MASSFILE_MEMBERFILEHEADER
	{
		char strFileName[MASSFILE_NAMESIZE];
		long RealSize;
		long CompressSize;
		long offSet;		//RealData부분에서의 OffSet입니다

		MASSFILE_MEMBERFILEHEADER()
		{
			ZeroMemory( strFileName, sizeof(char)*MASSFILE_NAMESIZE );
			RealSize		= 0;
			CompressSize	= 0;
			offSet			= 0;
		}
	};

public:
	struct MASSFILE_MEMBERFILEINFO
	{
		char strFileName[MASSFILE_NAMESIZE];
		long size;
		long CompressedSize;
		char *pRealData;
		DWORD dwFlag;
	};

public:
	CMassFile();
	virtual ~CMassFile();
	UINT GetFileCount();		//MassFile에 몇개의 File이 포함되어있는지 알아봅니다.

	bool LoadMassFile(const char* strMassFile, const char* strKomFileName);
	bool LoadMassFile(const char* strMassFile);
	bool GetMemberFile(const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut);
	bool GetMemberFile(int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut);

	KGCBufferManager m_BufferManager1;
	KGCBufferManager m_BufferManager2;

	//아래 두 함수는 MassFile을 Load시켜두지 않고 하나만 읽어 들이고자 할때..
	//여러번 사용할시 속도 저하의 원인이 됨
	bool GetMemberFile(const char* strMassFile, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut);
	bool GetMemberFile(const char* strMassFile, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut);

	bool SaveMassFile(char* strFileName, std::vector<char*> *pVecFile, bool bZip = false);
	bool SaveMassFile();
	bool ModifyMassFile( char* strFilePath );	
	bool ModifyMassFile( CMassFile* pMassFile, const char* strFileName, std::vector<char*> *pVecFile );
	bool ModifyMassFile( CMassFile* pMassFile, const char* strFileName, std::vector<char*> *pVecFile, int iSubFileSize );

	//FILE관련...Help
	long GetFileSize(const char* strFileName);
	void GetFileName(char* Buffer,char* strFilePath);

	bool IsCompressed();

private:

	std::vector<MASSFILE_MEMBERFILEHEADER> m_vecMemberFileHeader;
	MASSFILE_HEADER						   m_MassFileHeader;
	char								   m_strMassFileName[MAX_PATH];
	FILE*								   m_File;
	int									   m_iFileVersion;
	std::string							   m_strKomFileName;
};

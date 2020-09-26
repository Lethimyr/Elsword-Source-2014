/* KGCMassFileManager.h: interface for the KGCMassFileManager class.

by KOG GC TEAM 류상욱
===========================================================================================
									GCMassFileManager
===========================================================================================
목적 : 
		- MassFile을 등록하여, Kom파일 내부에 있던 혹은 Data폴더 안에 있는지 여부에 관계없이
		  데이터를 관리할 수 있도록함
		- 차후 압축을 관리 할수 있을것임.


===========================================================================================
										사용예제
===========================================================================================
KGCMassFileManager Manager;
Manager.SetDataDirectory("./Data");
if(!Manager.AddMassFile("./Data/Abta.kom")) return 0;
if(!Manager.AddMassFile("./Data/Texture.kom")) return 0;
if(!Manager.AddMassFile("./Data/Motion.kom")) return 0;

CMassFile::MASSFILE_MEMBERFILEINFO Info;
for(int i = 0; i < Hits; i++)
{	
	if( !Manager.LoadDataFile("ui_game.dds",&Info) )
		return 0;
	
	//Info를 사용하시오
	//Info.pRealData 삭제하지 말것...Manager가 직접 관리함
}

===========================================================================================
									속도 측정 결과 리포트
===========================================================================================
환경
OS - WindowXP
CPU - P4 3.0GHz
Mem - 512MB
HDD - 7200 RPM(Samsung)

3개의 kom파일에서 검색, 총 4457개의 파일이 포함되어 있었음 (10000Hits) - In Debug Mode


Mon_x07.frm : 206 ms
Abta000.dds : 217 ms
ui_game.dds : 253 ms

Average : 10000Hit Time - 225 ms

============================================================================================
						FILE*에서 fseek에 걸리는 시간 측정(Debug Mode)
============================================================================================

100000회 반복시 76ms : 거의 없다고 보면될듯...
*/
#pragma once

#define MASSFILE_VERSION		(1)
#define MASSFILE_NAMESIZE		(60)
#define MFI_REALDATA			(1)

class KGCMassFileManager  
{
	public:
		enum
		{
			FILESEEK_CUR = 0,
			FILESEEK_SET,
			FILESEEK_END,
		};

		class KGCMemoryFile
		{
			public:
				KGCMemoryFile();
				virtual ~KGCMemoryFile();

				void SetFile( char* pData,int size );
				int ReadFile(void* pDest, int ReadBytes);
				char* GetDataPointer() { return m_pCursor; }
				int FileSeek(int iPos, int Pivot);

			private:
				char* m_pData;
				char* m_pCursor;
				int  m_iSize;
				int  m_iCursorPos;
		};

		class KGCBufferManager  
		{
			public:
				KGCBufferManager();
				virtual ~KGCBufferManager();
				char* GetBuffer(int iSize);
				int   GetBufferSize() { return m_iMemorySize; }
				void  Flush();

			private:
				int m_iMemorySize;
				char* m_pData;
		};
	
        class CMassFile
		{
			private:
				struct MASSFILE_HEADER
				{
					char strVersionInfo[50];
					UINT iTotalFileNo;
					bool bCompressed;
				};
				struct MASSFILE_MEMBERFILEHEADER
				{
					char strFileName[MASSFILE_NAMESIZE];
					long RealSize;
					long CompressSize;
					long offSet;		//RealData부분에서의 OffSet입니다
				};

			public:
				struct MASSFILE_MEMBERFILEINFO
				{
					char strFileName[MASSFILE_NAMESIZE];
					long size;
					char *pRealData;
					DWORD dwFlag;

					MASSFILE_MEMBERFILEINFO()
					{
						ZeroMemory( strFileName, sizeof(char) * MASSFILE_NAMESIZE );
						size			= 0;
						pRealData		= NULL;
						dwFlag			= MFI_REALDATA;
					}
				};

			public:
				CMassFile();
				virtual ~CMassFile();
				UINT GetFileCount();		//MassFile에 몇개의 File이 포함되어있는지 알아봅니다.

				bool LoadMassFile( const char* strMassFile);
				bool GetMemberFile(const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut);
				bool GetMemberFile(int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut);

				static KGCBufferManager m_BufferManager1;
				static KGCBufferManager m_BufferManager2;

				//아래 두 함수는 MassFile을 Load시켜두지 않고 하나만 읽어 들이고자 할때..
				//여러번 사용할시 속도 저하의 원인이 됨
				bool GetMemberFile(const char* strMassFile, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut);
				bool GetMemberFile(const char* strMassFile, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut);


				bool SaveMassFile(char* strFileName, std::vector<char*> *pVecFile, bool bZip = false);
				bool ModifyMassFile(CMassFile* pMassFile,char* strFileName, std::vector<char*> *pVecFile, bool bZip = true);

				//FILE관련...Help
				long GetFileSize(char* strFileName);
				void GetFileName(char* Buffer,char* strFilePath);

				bool IsCompressed(){ return m_MassFileHeader.bCompressed; }
				void SetCompressed( bool bCompressed ){ m_MassFileHeader.bCompressed = bCompressed; }

			private:
				std::vector<MASSFILE_MEMBERFILEHEADER> m_vecMemberFileHeader;
				MASSFILE_HEADER						   m_MassFileHeader;
				char								   m_strMassFileName[MAX_PATH];
				FILE*								   m_File;
				int									   m_iFileVersion;
		};

	private:
		struct SMassFileIndex		// file이름으로 MassFile의 포인터와 Index를 역참조 하기위함
		{
			CMassFile* pMassFile;
			unsigned int Index;
		};
		
	public:
		KGCMassFileManager();
		virtual ~KGCMassFileManager();

		void AddDataDirectory(const std::string& strDirectory);
		CMassFile* AddMassFile(const std::string& strKomFileName);

		bool KGCMassFileManager::IsValidFile( std::wstring wstrFileName );
		CMassFile::MASSFILE_MEMBERFILEINFO* LoadDataFile( std::string strFileName, bool bRealData = true );
		CMassFile::MASSFILE_MEMBERFILEINFO* LoadDataFile( std::wstring wstrFileName, bool bRealData = true );
		
		KGCMemoryFile* LoadMemoryFile( std::string strFileName );
		KGCMemoryFile* LoadMemoryFile( std::wstring wstrFileName );

		int  GetTotalFileCount();

		//note : 현재 실행되는 폴더의 하위폴더경로를 셋팅해줌
		bool SetSubDirectory();
		//note : 파일이 있으면 해당파일의 경로명을 리턴
		WCHAR* GetPullPath( WCHAR* wcFileName );
		char*  GetPullPath( char* cFileName );

	private:
		//KOM파일이 아닌 Data폴더에서 실제 파일을 읽어 들이는경우..
		bool LoadRealFile(const std::string& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO *pOut);
		//note : 현제 폴더 경로를 받아 하위폴더경로를 얻어옴
		bool GetSubDirectory( char* szSearchDir);

		std::map< std::string, SMassFileIndex >	m_mapMassFile;
		std::vector< CMassFile* >				m_vecMassFile;
		std::vector< string	>					m_strDataDirectory;

		static KGCBufferManager					m_BufferManager;
		CMassFile::MASSFILE_MEMBERFILEINFO		m_Info;
		KGCMemoryFile							m_MemoryFile;
};


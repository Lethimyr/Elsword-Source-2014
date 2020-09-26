#pragma once


enum FILE_STATE
{
	FS_NONE = 0,
	FS_INSERTED,
	FS_CHANGED,
	FS_DELETED,
};

enum UNCOMPRESS_MODE
{
	UM_NONE = 0,
	UM_ONE_KOM,
	UM_ALL_KOM,
	UM_SEL_KOM,
	UM_SEL_FILE,
};

struct MemberFileInfo
{
	std::wstring m_wstrFileName;
    int			 m_iFileSize;
	int			 m_iFileState;
	std::wstring m_wstrKomString;
	std::wstring m_wstrRealPath;

	MemberFileInfo()
	{
		m_iFileSize  = 0;
		m_iFileState = FS_NONE;
	}

	// find_if
	struct FIND_IF
	{
		std::wstring keyString;

		FIND_IF( std::wstring& key ) { keyString = key;	}
		bool operator()( MemberFileInfo& memberFile )
		{
			return ( keyString == memberFile.m_wstrFileName );
		}
	};
};

struct ModifyData;
class CX2KomFileManager
{
public:
	CX2KomFileManager(void);
	~CX2KomFileManager(void);

	// 중복파일 체크
	bool CheckSameFiles( IN std::wstring& wstrNewPath, OUT std::map< std::wstring, std::vector< std::wstring > >& vecSameFiles );

	// lua 컴파일
	bool CompileLuaScript( IN std::wstring& wstrNewPath, OUT std::map< std::wstring, MemberFileInfo >& mapLuaFiles );
	bool CompileLuaScript( IN std::vector< std::wstring >& vecNewFolderPaths, OUT std::map< std::wstring, MemberFileInfo >& mapLuaFiles );	// hyunchul, 2009-04-01

	bool RemoveCompiledLuaScript( IN std::map< std::wstring, MemberFileInfo >& mapLuaFiles );

	// 스크립트 암호화
	bool PrepareTextFileEncrypt( IN std::wstring& wstrNewPath, OUT std::map< std::wstring, MemberFileInfo >& mapTextFiles );
	bool PrepareTextFileEncrypt( IN std::vector< std::wstring >& vecNewFolderPaths, OUT std::map< std::wstring, MemberFileInfo >& mapTextFiles );	// hyunchul, 2009-04-01
	bool EncryptLuaScript( IN std::map< std::wstring, MemberFileInfo >& mapLuaFiles );		

	// kom 파일 생성
	bool CreateKomFile( IN std::wstring& wstrSavePath, IN std::map< std::wstring, std::vector< std::wstring > >& mapPackingFiles );
	bool CreateKomFile( IN std::wstring& wstrSavePath, IN bool bIsMultiThreadMode = false );
	bool CreateKomFileMultiThread( IN std::wstring& wstrSavePath );

	// kom 파일 압축 해제
	bool UncompressKomFile( IN std::wstring& wstrUncompressPath, IN std::vector< std::wstring >& vecSeletedFileList, IN std::wstring& wstrCurKomName, IN UNCOMPRESS_MODE eMode, IN bool bResultFullPath, OUT std::map< std::wstring, std::vector< std::wstring > >& mapResultFullPath );
	bool Convert_X_File( IN std::wstring& wstrNewPath, IN bool bIsRestore = false );
	bool Convert_X_File( IN std::vector< std::wstring >& vecNewFolderPaths, IN bool bIsRestore = false );	// hyunchul, 2009-04-01

	// kom멤버파일 정렬함수
	bool ParsingKomFileAndMemberFile( IN std::wstring& wstrOldPath, IN std::wstring& wstrNewPath, IN const std::map< std::wstring, MemberFileInfo >& mapLuaFiles, IN bool bIsLuaScriptComplile = false, IN bool bIsFileCrypt = false );
	bool ParsingMemberFile( IN std::wstring& wstrMemberFilePath, IN const std::map< std::wstring, MemberFileInfo >& mapLuaFiles, IN bool bIsLuaScriptComplile = false, IN bool bIsFileCrypt = false );
	bool ParsingKomFile( IN std::wstring& wstrKomFilePath );
	bool ReorganizeMemberFile();
	void FindFolderFile( IN WCHAR* wstrFolder, OUT std::vector< std::wstring >& vecFileList );
	bool MemberFileFind( IN std::wstring& wstrMemberFileName, IN bool bIsOld, OUT std::wstring& wstrKomString, OUT int& iFindIndex );

	// kom파일 재압축
	bool RepackagingKomFile( IN std::wstring& wstrKomSavePath, IN std::map< std::wstring, ModifyData >& mapModifyData, IN bool bLuaCompile = false, IN bool bLuaEncrypt = false );

	bool IsExistFileInNewMember( IN std::wstring& wstrFileName );
	long GetFileSize( const char* strFileName );
	bool IsCompareKom() { return m_bIsCompareKom; }
	std::map< std::wstring, std::vector< MemberFileInfo > >& GetMapOldMemberFile() { return m_mapOldMemberFile;	}
	std::map< std::wstring, std::vector< MemberFileInfo > >& GetMapNewMemberFile() { return m_mapNewMemberFile;	}
	std::vector< MemberFileInfo >& GetInsertedFile() { return m_vecInsertedFile; }
	std::vector< MemberFileInfo >& GetDeletedFile() { return m_vecDeletedFile; }
	int GetKomSize( std::wstring& komName );
	std::map< std::wstring, std::wstring >& GetKomRealPathList() { return m_mapKomRealPath; }

	// Lua Script
	void LoadConfig();
	bool InitKomFileViewerOption_Lua();
	bool AddEncryptionKey_Lua();
	void SetThreadCount_Lua( int iThreadCount ) { m_iThreadCount = iThreadCount; }
	bool SetPathsForCommand_Lua();

	// for multi Thread packing
	bool GetPackingDataForMultiThreading( OUT std::wstring& wstrKomName, OUT std::vector< MemberFileInfo >& vecMemberFiles );
	const std::wstring& GetSavePathForMultiThreading() { return m_wstrSavePathForMT; }

private:
	std::wstring m_wstrOldPath;		// 구 폴더 경로
	std::wstring m_wstrNewPath;		// 신 폴더 경로
	bool		 m_bIsCompareKom;

	int m_iKFS_MIN_SIZE;
	int m_iKFS_MAX_SIZE;
	int	m_iKFS_AVERAGE_SIZE;
	std::vector< int > m_vecEncryptionKey;

	// kom member file 비교 데이터
	std::map< std::wstring, std::vector< MemberFileInfo > > m_mapOldMemberFile;		// 구버전 폴더 경로 지정시 KOM 파일들의 
																					// 구조체 정보를 모아둔 MAP(key:fileName, T: MemberFileInfo 구조체)

	std::map< std::wstring, std::vector< MemberFileInfo > > m_mapNewMemberFile;		// 신버전 폴더 경로 지정시 파일들을 사이즈 규칙에 따라 KOM 파일명을 지어준후 map에 넣어줌
																					// (key: kom 파일명(data*), T: MemberFileInfo 구조체)
	typedef std::map< std::wstring, std::vector< MemberFileInfo > >::iterator MemberFileIterator;

	std::vector< MemberFileInfo > m_vecInsertedFile; // 구버전, 신버전 파일 모두 열었을 때 추가된 파일
	std::vector< MemberFileInfo > m_vecDeletedFile;	 // 삭제된 파일들

	// kom member file 리스트 뷰어 데이터
	std::map< std::wstring, int > m_mapKomSize;

	// for log
	std::wofstream m_ofs;

	// for Lua Script Compile
	std::wstring m_wstrLuacCommand;

    std::wstring m_wstrAnsiToUTF8Command;

	// for X Convert
	std::wstring m_wstrXConvertCommand;

	// for depot path
	std::wstring m_wstrDepotPath;

	// kom file real path
	std::map< std::wstring, std::wstring > m_mapKomRealPath;	// 구버전 폴더 경로 지정시 KOM 파일들의 경로만 모아둔 MAP(key: fiilName, T: 경로)

	// For Multi Thread Packing
	std::map< std::wstring, std::vector< MemberFileInfo > > m_mapNewMemberForMT;
	std::wstring m_wstrSavePathForMT;
	CRITICAL_SECTION m_csNewMemberForMT;
	int	m_iThreadCount;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 수정자: 김현철
// 수정일: 2009.03.25
// 수정내용: 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//{{ hyunchul // 2009-03-25, ??:??
public:
	std::vector< std::wstring > m_vecKomFileList;
	std::vector< std::wstring > m_vecFolderList;				// new Path
	std::vector< std::wstring > m_vecKomNameList;				// 만들어질 kom 파일 리스트
	std::vector< std::wstring > m_vecPathListToCopy;			// 전 버전 폴더에서 복사 해야할 리스트
	std::vector< std::wstring > m_vecPathListToCompress;		// 바뀐 파일들이 있어서 새로 압축 해야할 리스트

	//BOOL GetKomFileList();
	//BOOL GetFolderList();
	std::vector< std::wstring > GetNewFolderList();
	BOOL GetKomFileListFromLuaScript();						// 루아 스크립트로 부터 기존의 kom 파일의 경로들을 받아오기
	BOOL GetFolderToCompressListFromLuaScript( std::vector< std::wstring >& vecNewFolderPaths, std::vector< std::wstring >& vecKomNameList );			// 루아 스크립트로 부터 kom으로 압축할 새로운 폴더들의 경로 받아오기
	BOOL CX2KomFileManager::GetKomFileAndFolderToCompressListFromLuaScript( std::vector< std::wstring >& vecNewFolderPaths, std::vector< std::wstring >& m_vecKomNameList, bool bIsLuaScriptComplile, bool bIsFileCrypt, 
		std::map< std::wstring, MemberFileInfo >& mapLuaFiles );			// 루아 스크립트로 부터 기존의 kom 파일의 경로 및 kom으로 압축할 새로운 폴더들의 경로 받아오기
	void FindFolderFile( IN WCHAR* wstrFolder, OUT std::vector< MemberFileInfo >& vecMemberFileInfoList,
		WCHAR* wstrFolderName, FILE_STATE enumState);		// 해당 폴더 내의 파일들 정보를 담기(구버전만)
	void FindFolderKomFile( IN WCHAR* wstrFolder );			//, OUT std::vector< std::wstring >& vecFileList );	// 해당 폴더 내의 kom 파일 찾기(신버전만)
	void FindFolderFileAndSetFileState( IN WCHAR* wstrFolder, OUT std::vector< MemberFileInfo >& vecMemberFileInfoList, 
		WCHAR* pwstrFolderName, MemberFileIterator mit, BOOL bIsSameFolder, bool bIsLuaScriptComplile, bool bIsFileCrypt, 
		std::map< std::wstring, MemberFileInfo >& mapLuaFiles );							// 해당 폴더 내의 파일들 정보를 담기(구버전, 신버전 둘다)
	// 중복파일 체크
	bool CheckSameFiles( OUT std::map< std::wstring, std::vector< std::wstring > >& vecSameFiles );
	BOOL InitNewFolderPaths_Lua();//( const std::wstring wstrNewPath  );
	const std::wstring&			GetDepotPath() const;
//}} hyunchul // 2009-03-25, ??:??


};

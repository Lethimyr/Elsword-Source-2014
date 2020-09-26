#pragma once

#pragma warning(disable:4786)

class KGCBufferManager;

class KGCMassFileManager  
{
private:
	struct SMassFileIndex		// file이름으로 MassFile의 포인터와 Index를 역참조 하기위함
	{
		CMassFile* pMassFile;
		unsigned int Index;
	};

public:
	KGCMassFileManager();
	virtual ~KGCMassFileManager();

	void		SetDataDirectory(const std::string& strDirectory);
	CMassFile*	AddMassFile(const std::string& strKomFileName);
	bool		LoadDataFile(const std::string& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO *pOut);
	int			GetTotalFileCount();

private:
	//KOM파일이 아닌 Data폴더에서 실제 파일을 읽어 들이는경우..
	bool		LoadRealFile(const std::string& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO *pOut);

	std::map< std::string, SMassFileIndex >	m_mapMassFile;
	std::vector< CMassFile* >				m_vecMassFile;
	std::string								m_strDataDirectory;

	KGCBufferManager						m_BufferManager;
};

// KGCMassFileManager.cpp: implementation of the KGCMassFileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "zlib/zlib.h"
#include "KGCMassFileManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCMassFileManager::KGCBufferManager KGCMassFileManager::m_BufferManager;

KGCMassFileManager::KGCMassFileManager()
{
}

KGCMassFileManager::~KGCMassFileManager()
{
	for(int i = 0; i < (int)m_vecMassFile.size(); i++)
	{
		delete m_vecMassFile[i];
	}
	m_vecMassFile.clear();
	m_mapMassFile.clear();
	m_strDataDirectory.clear();
}

int  KGCMassFileManager::GetTotalFileCount()
{
	return (int)m_mapMassFile.size();
}

void KGCMassFileManager::AddDataDirectory(const std::string& strDirectory)
{
	m_strDataDirectory.push_back( strDirectory );
}


KGCMassFileManager::CMassFile* KGCMassFileManager::AddMassFile(const std::string& strKomFileName)
{
	CMassFile* pMassFile = new CMassFile();
	if( !pMassFile->LoadMassFile(strKomFileName.c_str()) )
	{
		delete pMassFile;
		return NULL;
	}

	m_vecMassFile.push_back(pMassFile);

	CMassFile::MASSFILE_MEMBERFILEINFO Info;
	Info.dwFlag = 0;
	SMassFileIndex sFileIndex;
	//Real Data는 받아오지 않음
	ZeroMemory(&Info,sizeof(Info));

	for(int i = 0; i < (int)pMassFile->GetFileCount(); i++)
	{
		pMassFile->GetMemberFile( i, &Info );
		sFileIndex.Index = i;
		sFileIndex.pMassFile = pMassFile;
		if(m_mapMassFile.find(Info.strFileName) == m_mapMassFile.end())
		{
			m_mapMassFile[Info.strFileName] = sFileIndex;
		}			
	}

	return pMassFile;
}

bool KGCMassFileManager::IsValidFile( std::wstring wstrFileName )
{
	char strFileName[255] = {0,};

	m_Info.dwFlag = false;
	MakeUpperCase( wstrFileName );
	WideCharToMultiByte( CP_ACP, 0, wstrFileName.c_str(), -1, strFileName, 255, NULL, NULL );

#ifndef _DEBUG
	if( m_mapMassFile.find(strFileName) == m_mapMassFile.end() )
	{	
		if( LoadRealFile( strFileName, &m_Info ) == true )
			return true;
		else
			return false;
	}
	else
	{	
		return true;
	}
#else
	if( LoadRealFile( strFileName, &m_Info ) == true )
		return true;
	else if( m_mapMassFile.find(strFileName) != m_mapMassFile.end() )
	{
		return true;
	}
	else
		return false;
#endif
}

KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO* KGCMassFileManager::LoadDataFile( std::string strFileName, bool bRealData )
{
	m_Info.dwFlag = bRealData;
	MakeUpperCase( strFileName );

#ifndef _DEBUG
	if( m_mapMassFile.find(strFileName) == m_mapMassFile.end() )
	{	
		if( LoadRealFile( strFileName, &m_Info ) == true )
			return &m_Info;
		else
		{
            return NULL;
		}
	}
	else
	{	
		SMassFileIndex *sFileIndex = &m_mapMassFile[strFileName];
		sFileIndex->pMassFile->GetMemberFile( sFileIndex->Index, &m_Info );
		return &m_Info;
	}
#else
	if( LoadRealFile( strFileName, &m_Info ) == true )
		return &m_Info;
	else if( m_mapMassFile.find(strFileName) != m_mapMassFile.end() )
	{
		SMassFileIndex *sFileIndex = &m_mapMassFile[strFileName];
		sFileIndex->pMassFile->GetMemberFile( sFileIndex->Index, &m_Info );
		return &m_Info;
	}
	else
	{
		return NULL;
	}
#endif
}

KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO* KGCMassFileManager::LoadDataFile( std::wstring wstrFileName, bool bRealData )
{
	char strFileName[255] = {0,};

	m_Info.dwFlag = bRealData;
	MakeUpperCase( wstrFileName );
	WideCharToMultiByte( CP_ACP, 0, wstrFileName.c_str(), -1, strFileName, 255, NULL, NULL );

#ifndef _DEBUG
	if( m_mapMassFile.find(strFileName) == m_mapMassFile.end() )
	{	
		if( LoadRealFile( strFileName, &m_Info ) == true )
			return &m_Info;
		else
			return NULL;
	}
	else
	{	
		SMassFileIndex *sFileIndex = &m_mapMassFile[strFileName];
		sFileIndex->pMassFile->GetMemberFile( sFileIndex->Index, &m_Info );
		return &m_Info;
	}
#else
	if( LoadRealFile( strFileName, &m_Info ) == true )
		return &m_Info;
	else if( m_mapMassFile.find(strFileName) != m_mapMassFile.end() )
	{
		SMassFileIndex *sFileIndex = &m_mapMassFile[strFileName];
		sFileIndex->pMassFile->GetMemberFile( sFileIndex->Index, &m_Info );
		return &m_Info;
	}
	else
		return NULL;
#endif
}

KGCMassFileManager::KGCMemoryFile* KGCMassFileManager::LoadMemoryFile( std::string strFileName )
{
	if( LoadDataFile( strFileName ) != NULL )
	{
		m_MemoryFile.SetFile( m_Info.pRealData, m_Info.size );
		return &m_MemoryFile;
	}
	else
		return NULL;
}

KGCMassFileManager::KGCMemoryFile* KGCMassFileManager::LoadMemoryFile( std::wstring wstrFileName )
{
	if( LoadDataFile( wstrFileName ) != NULL )
	{
		m_MemoryFile.SetFile( m_Info.pRealData, m_Info.size );
		return &m_MemoryFile;
	}
	else
		return NULL;
}

bool KGCMassFileManager::LoadRealFile(const std::string& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO *pOut)
{
	FILE* file;

	for( int i = 0; i < (int)m_strDataDirectory.size(); i++ )
	{
		std::string strName = m_strDataDirectory[i] + strFileName;

		file = fopen( strName.c_str(), "rb" );
		if(file != NULL)
			break;
	}

	if(file == NULL)
	{
		return false;
	}	

	fseek(file,0,SEEK_END);
	pOut->size = ftell(file);
	fseek(file,0,SEEK_SET);

	if(pOut->dwFlag & MFI_REALDATA)
	{
		pOut->pRealData = (char*)m_BufferManager.GetBuffer(pOut->size);
		fread(pOut->pRealData,1,pOut->size,file);
	}

	strcpy(pOut->strFileName, strFileName.c_str());

	fclose(file);
	return true;
}



////////////////////////////////////////////////////////////////////////////////////
// MassFile
////////////////////////////////////////////////////////////////////////////////////

KGCMassFileManager::KGCBufferManager KGCMassFileManager::CMassFile::m_BufferManager1;
KGCMassFileManager::KGCBufferManager KGCMassFileManager::CMassFile::m_BufferManager2;

KGCMassFileManager::CMassFile::CMassFile()
{
	m_File = NULL;
	memset(&m_MassFileHeader,0,sizeof(MASSFILE_HEADER));	
	m_MassFileHeader.bCompressed = true;
}

KGCMassFileManager::CMassFile::~CMassFile()
{
	if(m_File != NULL)
		fclose(m_File);
}

UINT KGCMassFileManager::CMassFile::GetFileCount()
{
	return m_MassFileHeader.iTotalFileNo;
}

void KGCMassFileManager::CMassFile::GetFileName(char* Buffer,char* strFilePath)
{
	for(int i = (int)strlen(strFilePath)-1; i > 0; i-- )
	{
		if(strFilePath[i] == '\\' || strFilePath[i] == '/')
		{
			strcpy(Buffer,&strFilePath[i+1]);
			return;
		}
	}
	strcpy(Buffer,strFilePath);
	return;
}

bool KGCMassFileManager::CMassFile::LoadMassFile( const char* strMassFile)
{
	if( m_File != NULL )
		fclose(m_File);

	m_File = fopen(strMassFile,"rb");
	if(m_File == NULL)
	{	//Error
		return false;
	}

	strcpy(m_strMassFileName,strMassFile);
	m_vecMemberFileHeader.clear();
	memset(&m_MassFileHeader,0,sizeof(MASSFILE_HEADER));
	fread(&m_MassFileHeader,sizeof(MASSFILE_HEADER),1,m_File);

	m_iFileVersion = (m_MassFileHeader.strVersionInfo[23] -'0') * 10 + m_MassFileHeader.strVersionInfo[25] - '0';

	for(int i = 0; i < (int)m_MassFileHeader.iTotalFileNo; i++)
	{
		MASSFILE_MEMBERFILEHEADER MemberHeader;
		fread(&MemberHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,m_File);

		string temp = MemberHeader.strFileName;
		MakeUpperCase( temp );
		sprintf( MemberHeader.strFileName, temp.c_str() );

		m_vecMemberFileHeader.push_back(MemberHeader);
	}

	return true;
}

bool KGCMassFileManager::CMassFile::GetMemberFile(const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut)
{
	if(m_File == NULL)
		return false;

	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER;
	bool find = false;
	for(int i = 0; i < (int)m_vecMemberFileHeader.size(); i++)
	{
		if(strcmp(m_vecMemberFileHeader[i].strFileName,strRealFile) == 0)
		{
			MEMBERFILEHEADER = m_vecMemberFileHeader[i];
			find = true;
			break;
		}
	}
	if(find == false)
		return false;

	pOut->size				= MEMBERFILEHEADER.RealSize;
	strcpy(pOut->strFileName, MEMBERFILEHEADER.strFileName);

	//GetRealData
	if( pOut->dwFlag & MFI_REALDATA )
	{			
		long posRealDataStart = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * m_MassFileHeader.iTotalFileNo;

		if(m_MassFileHeader.bCompressed)
		{		
			char* pCompressedData = m_BufferManager2.GetBuffer(MEMBERFILEHEADER.CompressSize);
			fseek(m_File, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pCompressedData,sizeof(char),MEMBERFILEHEADER.CompressSize,m_File);

			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);

			unsigned long size = MEMBERFILEHEADER.RealSize;
			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
		}
		else
		{
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
			fseek(m_File, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pOut->pRealData,sizeof(char),MEMBERFILEHEADER.RealSize,m_File);
		}		
	}

	return true;
}

bool KGCMassFileManager::CMassFile::GetMemberFile(int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut)
{
	if(m_File == NULL)
		return false;

	if(iFileIndex < 0 || iFileIndex >= (int)m_vecMemberFileHeader.size())
		return false;	

	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER = m_vecMemberFileHeader[iFileIndex];	

	pOut->size = MEMBERFILEHEADER.RealSize;
	strcpy(pOut->strFileName, MEMBERFILEHEADER.strFileName);
	pOut->pRealData = 0;

	if( pOut->dwFlag & MFI_REALDATA )
	{		
		long posRealDataStart = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * m_MassFileHeader.iTotalFileNo;

		if(m_MassFileHeader.bCompressed)
		{		
			char* pCompressedData = m_BufferManager2.GetBuffer(MEMBERFILEHEADER.CompressSize);
			fseek(m_File, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pCompressedData,sizeof(char),MEMBERFILEHEADER.CompressSize,m_File);

			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);

			unsigned long size = MEMBERFILEHEADER.RealSize;
			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
		}
		else
		{
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
			fseek(m_File, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pOut->pRealData,sizeof(char),MEMBERFILEHEADER.RealSize,m_File);
		}
	}


	return true;
}

bool KGCMassFileManager::CMassFile::GetMemberFile(const char* strMassFile, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut)
{
	FILE* file = fopen(strMassFile,"rb");

	if(file == NULL)
	{	//Error
		return false;
	}

	MASSFILE_HEADER Header;
	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER;
	int iFileIndex = -1;
	fread(&Header,sizeof(MASSFILE_HEADER),1,file);
	for(int i = 0; i < (int)Header.iTotalFileNo; i++)
	{
		fread(&MEMBERFILEHEADER,sizeof(MEMBERFILEHEADER),1,file);
		if(strcmp(MEMBERFILEHEADER.strFileName,strRealFile) == 0)
		{
			iFileIndex = i;
			break;
		}
	}
	if(iFileIndex == -1)
	{
		//파일을 찾을수 없음
		fclose(file);
		return false;
	}

	pOut->size = MEMBERFILEHEADER.RealSize;
	strcpy(pOut->strFileName, MEMBERFILEHEADER.strFileName);
	pOut->pRealData = 0;

	if( pOut->dwFlag & MFI_REALDATA )
	{		
		long posRealDataStart = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * Header.iTotalFileNo;

		if(Header.bCompressed)
		{		
			char* pCompressedData = m_BufferManager2.GetBuffer(MEMBERFILEHEADER.CompressSize);
			fseek(file, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pCompressedData,sizeof(char),MEMBERFILEHEADER.CompressSize,file);

			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);

			unsigned long size = MEMBERFILEHEADER.RealSize;
			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
		}
		else
		{
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
			fseek(file, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pOut->pRealData,sizeof(char),MEMBERFILEHEADER.RealSize,file);
		}
	}

	fclose(file);
	return true;
}

bool KGCMassFileManager::CMassFile::GetMemberFile(const char* strMassFile, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut)
{
	FILE* file = fopen(strMassFile,"rb");

	if(file == NULL)
	{	//Error
		return false;
	}

	MASSFILE_HEADER Header;
	fread(&Header,sizeof(MASSFILE_HEADER),1,file);
	if((int)Header.iTotalFileNo <= iFileIndex)
	{
		fclose(file);
		return false;
	}
	long MemberFileHeaderStart = sizeof(MASSFILE_HEADER);	
	long posRealDataStart = MemberFileHeaderStart + sizeof(MASSFILE_MEMBERFILEHEADER) * Header.iTotalFileNo;

	//GetMemberFileHeader
	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER;
	fseek(file,MemberFileHeaderStart + iFileIndex * sizeof(MASSFILE_MEMBERFILEHEADER),SEEK_SET);
	fread(&MEMBERFILEHEADER,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);

	pOut->size = MEMBERFILEHEADER.RealSize;
	strcpy(pOut->strFileName, MEMBERFILEHEADER.strFileName);
	pOut->pRealData = 0;

	if( pOut->dwFlag & MFI_REALDATA )
	{		
		long posRealDataStart = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * Header.iTotalFileNo;

		if(Header.bCompressed)
		{		
			char* pCompressedData = m_BufferManager2.GetBuffer(MEMBERFILEHEADER.CompressSize);
			fseek(file, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pCompressedData,sizeof(char),MEMBERFILEHEADER.CompressSize,file);

			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);

			unsigned long size = MEMBERFILEHEADER.RealSize;
			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
		}
		else
		{
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
			fseek(file, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pOut->pRealData,sizeof(char),MEMBERFILEHEADER.RealSize,file);
		}
	}

	fclose(file);
	return true;
}

long KGCMassFileManager::CMassFile::GetFileSize(char* strFileName)
{
	FILE* file = fopen(strFileName,"rb");
	if(file == NULL)
		return -1;
	long size = 0;
	fseek(file,0,SEEK_END);
	size = ftell(file);
	fclose(file);

	return size;
}

bool KGCMassFileManager::CMassFile::SaveMassFile(char* strFileName, std::vector<char*> *pVecFile, bool bZip)
{
	FILE* file = fopen(strFileName, "w+b");
	if(file == NULL)
		return false;

	//MASSFILE_HEADER
	MASSFILE_HEADER Header;
	Header.iTotalFileNo = (int)pVecFile->size();
	Header.bCompressed = bZip;
	sprintf( Header.strVersionInfo, "KOG GC TEAM MASSFILE V.%d.%d.",MASSFILE_VERSION/10,MASSFILE_VERSION%10 );
	fwrite(&Header,sizeof(MASSFILE_HEADER),1,file);

	//MASSFILE_MEMBERFILEHEADER

	MASSFILE_MEMBERFILEHEADER MemberFileHeader;
	long TotalSize = 0;
	for(int i = 0; i < (int)pVecFile->size(); i++)
	{
		fwrite(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);
	}
	TotalSize = 0;

	//RealFile
	FILE *pMemberFile = 0;
	char *pFileData = 0;
	long filesize;	

	for(i = 0; i < (int)pVecFile->size(); i++)
	{
		filesize = GetFileSize((*pVecFile)[i]);

		pMemberFile = fopen((*pVecFile)[i],"rb");
		if(pMemberFile == NULL)
		{
			fclose(file);
			MessageBox( NULL, L"MassFile Error", L"Error", 0 );
			//ErrorLogMsg( KEM_ERROR107, strFileName );
			return false;
		}

		unsigned long WriteByte = 0;

		if(bZip)
		{
			pFileData = (char*)m_BufferManager2.GetBuffer(filesize);

			char* pDestData = m_BufferManager1.GetBuffer(compressBound( filesize+8 ));
			WriteByte = m_BufferManager1.GetBufferSize();

			fread(pFileData,sizeof(char),filesize,pMemberFile);

			if(Z_OK	!= compress((BYTE*)pDestData,&WriteByte,(BYTE*)pFileData,filesize))
			{
				fclose(file);
				return false;
			}
			fwrite(pDestData,sizeof(char),WriteByte,file);
		}
		else
		{
			pFileData = (char*)m_BufferManager1.GetBuffer(filesize);
			fread(pFileData,sizeof(char),filesize,pMemberFile);
			fwrite(pFileData,sizeof(char),filesize,file);
			WriteByte = filesize;
		}

		fclose(pMemberFile);


		//==============================
		//현재 파일 커서를 받아온뒤..
		int cursor = ftell(file);

		//Header를 수정하고 다시 커서 제자리로...		
		fseek(file,sizeof(MASSFILE_HEADER)+ sizeof(MASSFILE_MEMBERFILEHEADER) * i,SEEK_SET);

		fread(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);
		fseek(file,sizeof(MASSFILE_HEADER)+ sizeof(MASSFILE_MEMBERFILEHEADER) * i,SEEK_SET);


		memset(MemberFileHeader.strFileName,0,sizeof(char)*MASSFILE_NAMESIZE);
		GetFileName(MemberFileHeader.strFileName,(*pVecFile)[i]);
		if(strlen(MemberFileHeader.strFileName) >= MASSFILE_NAMESIZE)
		{
			fclose(file);
			WCHAR tempStr[255] = {0,};
			MultiByteToWideChar( CP_ACP, 0, MemberFileHeader.strFileName, -1, tempStr, 255);
			MessageBox( NULL, tempStr, L"File Name Too Long!!!", 0 );
			//ErrorLogMsg( KEM_ERROR108, strFileName );
			return false;
		}

		MemberFileHeader.CompressSize = WriteByte;
		MemberFileHeader.offSet = TotalSize;
		TotalSize += WriteByte;
		MemberFileHeader.RealSize = filesize;

		fwrite(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);

		fread(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);

		fseek(file,cursor,SEEK_SET);
		//==============================		
	}

	fclose(file);
	return true;
}

bool KGCMassFileManager::CMassFile::ModifyMassFile(CMassFile* pMassFile,char* strFileName, std::vector<char*> *pVecFile, bool bZip )
{
	FILE* file = fopen(strFileName, "w+b");
	if(file == NULL)
		return false;

	//MASSFILE_HEADER
	MASSFILE_HEADER Header;
	Header.iTotalFileNo = (UINT)pVecFile->size();
	Header.bCompressed = bZip;
	sprintf( Header.strVersionInfo, "KOG GC TEAM MASSFILE V.%d.%d.",MASSFILE_VERSION/10,MASSFILE_VERSION%10 );

	fwrite(&Header,sizeof(MASSFILE_HEADER),1,file);

	MASSFILE_MEMBERFILEINFO OutFile;
	MASSFILE_MEMBERFILEHEADER MemberFileHeader;

	for ( int i = 0; i < (int)pVecFile->size(); i++ )
	{
		fwrite(&MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER),1,file);
	}

	FILE			*pMemberFile = 0;
	char			*pFileData = 0;
	long			FileSize = 0;
	unsigned long	WriteByte = 0;
	long			TotalSize = 0;

	for(int i = 0; i < (int)pVecFile->size(); i++)
	{
		char Filename[MASSFILE_NAMESIZE];
		GetFileName(Filename,(*pVecFile)[i]);
		if( !pMassFile->GetMemberFile(Filename,&OutFile) )
		{
			//실제 파일을 로딩해서 저장
			//파일이 안에 없음
			FileSize = GetFileSize((*pVecFile)[i]);

			pMemberFile = fopen((*pVecFile)[i],"rb");
			if(pMemberFile == NULL)
			{
				fclose(file);
				MessageBox( NULL, L"MassFile Error", L"Error", 0 );
				//ErrorLogMsg( KEM_ERROR109, strFileName );
				return false;
			}

			if(bZip)
			{
				pFileData = (char*)m_BufferManager2.GetBuffer(FileSize);

				char* pDestData = m_BufferManager1.GetBuffer(compressBound( filesize+8 ));
				WriteByte = m_BufferManager1.GetBufferSize();

				fread(pFileData,sizeof(char),FileSize,pMemberFile);

				if(Z_OK	!= compress((BYTE*)pDestData,&WriteByte,(BYTE*)pFileData,FileSize))
				{
					fclose(file);
					return false;
				}
				fwrite(pDestData,sizeof(char),WriteByte,file);
			}
			else
			{
				pFileData = (char*)m_BufferManager1.GetBuffer(FileSize);
				fread(pFileData,sizeof(char),FileSize,pMemberFile);
				fwrite(pFileData,sizeof(char),FileSize,file);
				WriteByte = FileSize;
			}

			fclose(pMemberFile);
		}
		else
		{
			if( bZip )
			{
				if( pMassFile->IsCompressed() )
				{
					pMassFile->SetCompressed( false );

					pMassFile->GetMemberFile( Filename, &OutFile );
					FileSize	= OutFile.size;
					WriteByte	= OutFile.size;

					fwrite(OutFile.pRealData,sizeof(char),WriteByte,file);

					pMassFile->SetCompressed( true );
				}
				else
				{
					FileSize = OutFile.size;

					char* pDestData = m_BufferManager1.GetBuffer(compressBound( filesize+8 ));
					WriteByte = m_BufferManager1.GetBufferSize();

					if(Z_OK	!= compress((BYTE*)pDestData,&WriteByte,(BYTE*)OutFile.pRealData,FileSize))
					{
						fclose(file);
						return false;
					}

					fwrite(pDestData,sizeof(char),WriteByte,file);
				}
			}
			else
			{
				FileSize	= OutFile.size;
				WriteByte	= FileSize;
				fwrite( OutFile.pRealData, sizeof(char), FileSize, file );
			}
		}

		int cursor = ftell(file);

		//Header를 수정하고 다시 커서 제자리로...		
		fseek(file,sizeof(MASSFILE_HEADER)+ sizeof(MASSFILE_MEMBERFILEHEADER) * i,SEEK_SET);

		fread(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);
		fseek(file,sizeof(MASSFILE_HEADER)+ sizeof(MASSFILE_MEMBERFILEHEADER) * i,SEEK_SET);


		memset(MemberFileHeader.strFileName,0,sizeof(char)*MASSFILE_NAMESIZE);
		GetFileName(MemberFileHeader.strFileName,(*pVecFile)[i]);
		if(strlen(MemberFileHeader.strFileName) >= MASSFILE_NAMESIZE)
		{
			fclose(file);
			MessageBoxA(NULL,MemberFileHeader.strFileName,"File Name Too Long!!!",0);
			//ErrorLogMsg( KEM_ERROR110, strFileName );
			return false;
		}

		MemberFileHeader.CompressSize = WriteByte;
		MemberFileHeader.offSet = TotalSize;
		TotalSize += WriteByte;
		MemberFileHeader.RealSize = FileSize;

		fwrite(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);

		fread(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);

		fseek(file,cursor,SEEK_SET);
		//==============================		
	}

	fclose(file);
	return true;
}


////////////////////////////////////////////////////////////////////////////////////
// BufferManager
////////////////////////////////////////////////////////////////////////////////////


KGCMassFileManager::KGCBufferManager::KGCBufferManager()
{
	m_pData = NULL;
	m_iMemorySize = 1024 * 1024;		//1MB

	m_pData = new char[m_iMemorySize];
}

KGCMassFileManager::KGCBufferManager::~KGCBufferManager()
{
	Flush();
}

void KGCMassFileManager::KGCBufferManager::Flush()
{
	if( m_pData != NULL)
	{
		delete []m_pData;
		m_pData = NULL;
	}
	m_iMemorySize = 0;
}

char* KGCMassFileManager::KGCBufferManager::GetBuffer(int iSize)
{
	if( m_iMemorySize < iSize )
	{
		if( m_pData != NULL )
		{		
			delete []m_pData;
			m_pData = NULL;
		}

		while( (m_iMemorySize *= 2) < iSize )
		{
		}

		m_pData = new char[m_iMemorySize];
	}

	ZeroMemory( m_pData, sizeof(char) * m_iMemorySize );

	return m_pData;
}


////////////////////////////////////////////////////////////////////////////////////
// MemoryFile
////////////////////////////////////////////////////////////////////////////////////

KGCMassFileManager::KGCMemoryFile::KGCMemoryFile()
{
	m_iCursorPos	= 0;
	m_pData			= NULL;
	m_pCursor		= NULL;
	m_iSize			= 0;
}

KGCMassFileManager::KGCMemoryFile::~KGCMemoryFile()
{
}

void KGCMassFileManager::KGCMemoryFile::SetFile( char* pData, int size )
{
	m_iCursorPos	= 0;
	m_pData			= pData;
	m_pCursor		= pData;
	m_iSize			= size;
}

int KGCMassFileManager::KGCMemoryFile::ReadFile(void* pDest, int ReadBytes)
{
	if( m_iCursorPos + ReadBytes > m_iSize )
		return 0;

	memcpy(pDest,m_pCursor,ReadBytes);
	m_pCursor		+= ReadBytes;
	m_iCursorPos	+= ReadBytes;

	return ReadBytes;
}

int KGCMassFileManager::KGCMemoryFile::FileSeek(int iPos, int Pivot)
{
	switch(Pivot)
	{	
		case FILESEEK_CUR:
			{
				m_iCursorPos += iPos;
			}
			break;

		case FILESEEK_SET:
			{
				m_iCursorPos = iPos;
			}
			break;

		case FILESEEK_END:
			{
				m_iCursorPos = (m_iSize-1) + iPos;
			}
			break;	
	}
	m_pCursor = m_pData + m_iCursorPos;
	return m_iCursorPos;
}

bool KGCMassFileManager::SetSubDirectory()
{
	char	szSearchDir[MAX_PATH] = "";
	char	szCurrentDir[MAX_PATH] = "";

	GetCurrentDirectoryA( MAX_PATH, szSearchDir );

	strcpy( szCurrentDir, szSearchDir );
	strcat( szCurrentDir, "\\" );

	AddDataDirectory( szCurrentDir );

	return GetSubDirectory( szSearchDir );	
}

bool KGCMassFileManager::GetSubDirectory( char* szSearchDir)
{

	HANDLE				hSearch;
	WIN32_FIND_DATAA	fd;
	char				szSearchPath[256];

	strcpy(szSearchPath, szSearchDir);
	strcat(szSearchPath, "\\*.*");

	hSearch = FindFirstFileA(szSearchPath, &fd);

	if(hSearch == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		if(strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
		{
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char	szNewSearchDir[256];
				string	strDir;

				strcpy(szNewSearchDir, szSearchDir);
				strcat(szNewSearchDir, "\\");
				strcat(szNewSearchDir, fd.cFileName);

				strDir = szNewSearchDir;
				strDir += "\\";
				AddDataDirectory( strDir);

				GetSubDirectory( szNewSearchDir);
			}
		}

	}while(FindNextFileA(hSearch, &fd));

	FindClose(hSearch);

	return true;
}

char* KGCMassFileManager::GetPullPath( char* cFileName )
{
	FILE* file;
	std::string strName;

	for( int i = 0; i < (int)m_strDataDirectory.size(); ++i )
	{
		strName = m_strDataDirectory[i] + cFileName;

		file = fopen( strName.c_str(), "rb" );
		if(file != NULL)
		{
			strcpy( cFileName, strName.c_str());
			break;
		}
	}

	if( file == NULL)
	{
		return NULL;
	}

	fclose( file );
	return cFileName;
}

WCHAR* KGCMassFileManager::GetPullPath( WCHAR* wcFileName )
{
	char strFileName[255] = {0,};
	std::string strName;

	WideCharToMultiByte( CP_ACP, 0, wcFileName, -1, strFileName, 255, NULL, NULL );

	FILE* file;

	for( int i = 0; i < (int)m_strDataDirectory.size(); ++i )
	{
		strName = m_strDataDirectory[i] + strFileName;

		file = fopen( strName.c_str(), "rb" );
		if(file != NULL)
		{
			MultiByteToWideChar( CP_ACP, 0, strName.c_str(), -1, wcFileName, 255);
			break;
		}
	}

	if( file == NULL)
	{
		return NULL;
	}

	fclose( file );
	return wcFileName;
}

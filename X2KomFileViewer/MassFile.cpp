#include "stdafx.h"
#include ".\massfile.h"

//KGCBufferManager CMassFile::m_BufferManager1;
//KGCBufferManager CMassFile::m_BufferManager2;

CMassFile::CMassFile()
{
	m_File = NULL;
	memset(&m_MassFileHeader,0,sizeof(MASSFILE_HEADER));	
	m_MassFileHeader.bCompressed = true;
}

CMassFile::~CMassFile()
{
	if(m_File != NULL)
		fclose(m_File);
}

UINT CMassFile::GetFileCount()
{
	return m_MassFileHeader.iTotalFileNo;
}

bool CMassFile::IsCompressed()
{
	return ( m_MassFileHeader.bCompressed == TRUE );
}
void CMassFile::GetFileName(char* Buffer,char* strFilePath)
{
	for(int i = strlen(strFilePath)-1; i > 0; i-- )
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

bool CMassFile::LoadMassFile(const char* strMassFile)
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


	for( int i = 0; i < (int)(m_MassFileHeader.iTotalFileNo); ++i )
	{
		MASSFILE_MEMBERFILEHEADER MemberHeader;
		fread(&MemberHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,m_File);
		m_vecMemberFileHeader.push_back(MemberHeader);
	}

	return true;
}

bool CMassFile::LoadMassFile(const char* strMassFile, const char* strKomFileName)
{
	m_strKomFileName = strKomFileName;
	return LoadMassFile( strMassFile );
}

bool CMassFile::GetMemberFile(const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut)
{
	if(m_File == NULL)
		return false;

	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER;
	bool find = false;
	for( int i = 0; i < (int)m_vecMemberFileHeader.size(); ++i )
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

	pOut->size = MEMBERFILEHEADER.RealSize;
	strcpy(pOut->strFileName, MEMBERFILEHEADER.strFileName);

	//GetRealData
	if( pOut->dwFlag & MFI_REALDATA )
	{			
		long posRealDataStart = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * m_MassFileHeader.iTotalFileNo;

		if(m_MassFileHeader.bCompressed)
		{		
			char* pCompressedData = m_BufferManager2.GetBuffer(compressBound(MEMBERFILEHEADER.CompressSize));
			fseek(m_File, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pCompressedData,sizeof(char),MEMBERFILEHEADER.CompressSize,m_File);

			if( pOut->dwFlag & MFI_NODECOMPRESS )
			{
				pOut->pRealData = pCompressedData;
				pOut->size = MEMBERFILEHEADER.RealSize;
				pOut->CompressedSize = MEMBERFILEHEADER.CompressSize;
			}
			else
			{
				pOut->pRealData = (char*)m_BufferManager1.GetBuffer(compressBound(MEMBERFILEHEADER.RealSize));

				unsigned long size = MEMBERFILEHEADER.RealSize;
				uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
			}
		}
		else
		{
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(compressBound(MEMBERFILEHEADER.RealSize));
			fseek(m_File, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pOut->pRealData,sizeof(char),MEMBERFILEHEADER.RealSize,m_File);
		}		
	}

	return true;
}

bool CMassFile::GetMemberFile(int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut)
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
			char* pCompressedData = m_BufferManager2.GetBuffer(compressBound(MEMBERFILEHEADER.CompressSize));
			fseek(m_File, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pCompressedData,sizeof(char),MEMBERFILEHEADER.CompressSize,m_File);

			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(compressBound(MEMBERFILEHEADER.RealSize));

			unsigned long size = MEMBERFILEHEADER.RealSize;
			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
		}
		else
		{
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(compressBound(MEMBERFILEHEADER.RealSize));
			fseek(m_File, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pOut->pRealData,sizeof(char),MEMBERFILEHEADER.RealSize,m_File);
		}
	}


	return true;
}

bool CMassFile::GetMemberFile(const char* strMassFile, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut)
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
			char* pCompressedData = m_BufferManager2.GetBuffer(compressBound(MEMBERFILEHEADER.CompressSize));
			fseek(file, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pCompressedData,sizeof(char),MEMBERFILEHEADER.CompressSize,file);

			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(compressBound(MEMBERFILEHEADER.RealSize));

			unsigned long size = MEMBERFILEHEADER.RealSize;
			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
		}
		else
		{
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(compressBound(MEMBERFILEHEADER.RealSize));
			fseek(file, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pOut->pRealData,sizeof(char),MEMBERFILEHEADER.RealSize,file);
		}
	}

	fclose(file);
	return true;
}

bool CMassFile::GetMemberFile(const char* strMassFile, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut)
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
			char* pCompressedData = m_BufferManager2.GetBuffer(compressBound(MEMBERFILEHEADER.CompressSize));
			fseek(m_File, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pCompressedData,sizeof(char),MEMBERFILEHEADER.CompressSize,m_File);

			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(compressBound(MEMBERFILEHEADER.RealSize));

			unsigned long size = MEMBERFILEHEADER.RealSize;
			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
		}
		else
		{
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(compressBound(MEMBERFILEHEADER.RealSize));
			fseek(m_File, posRealDataStart + MEMBERFILEHEADER.offSet,SEEK_SET);
			fread(pOut->pRealData,sizeof(char),MEMBERFILEHEADER.RealSize,m_File);
		}
	}

	fclose(file);
	return true;
}

long CMassFile::GetFileSize(const char* strFileName)
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

bool CMassFile::SaveMassFile(char* strFileName, std::vector<char*> *pVecFile, bool bZip)
{
	FILE* file = fopen(strFileName, "w+b");
	if(file == NULL)
		return false;

	//MASSFILE_HEADER
	MASSFILE_HEADER Header;
	Header.iTotalFileNo = pVecFile->size();
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
			MessageBoxA(NULL,"Unkown Error","Error",0);
			return false;
		}

		unsigned long WriteByte = 0;

		if(bZip)
		{
			pFileData = (char*)m_BufferManager2.GetBuffer(compressBound(filesize));

			char* pDestData = m_BufferManager1.GetBuffer(compressBound(filesize+8));
			WriteByte = m_BufferManager1.GetBufferSize();

			fread(pFileData,sizeof(char),filesize,pMemberFile);

			if(Z_OK	!= compress((BYTE*)pDestData,&WriteByte,(BYTE*)pFileData,filesize))
			{
				fclose(file);
				fclose(pMemberFile);
				return false;
			}
			fwrite(pDestData,sizeof(char),WriteByte,file);
		}
		else
		{
			pFileData = (char*)m_BufferManager1.GetBuffer(compressBound(filesize));
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
			MessageBoxA(NULL,MemberFileHeader.strFileName,"File Name Too Long!!!",0);
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


bool CMassFile::SaveMassFile()
{
	if(m_strMassFileName[0] == 0)
		return false;

	FILE* file = fopen(m_strMassFileName, "w+b");
	if(file == NULL)
		return false;

	m_MassFileHeader.iTotalFileNo = m_vecMemberFileHeader.size();

	fwrite(&m_MassFileHeader,sizeof(MASSFILE_HEADER),1,file);

	//MASSFILE_MEMBERFILEHEADER

	MASSFILE_MEMBERFILEHEADER MemberFileHeader;
	long TotalSize = 0;
	for(int i = 0; i <  (int)m_MassFileHeader.iTotalFileNo; i++)
	{
		fwrite(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);
	}


	//RealFile
	FILE *pMemberFile = 0;
	char *pFileData = 0;
	long filesize;	

	for(i = 0; i < (int)m_MassFileHeader.iTotalFileNo ; i++)
	{
		filesize = GetFileSize(m_vecMemberFileHeader[i].strFileName);
		char* df = m_vecMemberFileHeader[i].strFileName;
		pMemberFile = fopen(m_vecMemberFileHeader[i].strFileName,"rb");
		if(pMemberFile == NULL)
		{
			fclose(file);
			MessageBoxA(NULL,"Unkown Error","Error",0);
			return false;
		}

		unsigned long WriteByte = 0;

		if(m_MassFileHeader.bCompressed)
		{
			pFileData = (char*)m_BufferManager2.GetBuffer(compressBound(filesize));

			char* pDestData = m_BufferManager1.GetBuffer(compressBound(filesize+8));
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
			pFileData = (char*)m_BufferManager1.GetBuffer(compressBound(filesize));
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
		GetFileName(MemberFileHeader.strFileName,m_vecMemberFileHeader[i].strFileName);
		if(strlen(MemberFileHeader.strFileName) >= MASSFILE_NAMESIZE)
		{
			fclose(file);
			MessageBoxA(NULL,MemberFileHeader.strFileName,"File Name Too Long!!!",0);
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

bool CMassFile::ModifyMassFile(CMassFile* pMassFile, const char* strFileName, std::vector<char*> *pVecFile)
{
	FILE* file = fopen(strFileName, "w+b");
	if(file == NULL)
		return false;

	long FileSize = 0;
	unsigned long WriteByte = 0;
	long TotalSize = 0;

	//MASSFILE_HEADER
	MASSFILE_HEADER Header;
	Header.iTotalFileNo = pVecFile->size();
	Header.bCompressed = pMassFile->IsCompressed();
	sprintf( Header.strVersionInfo, "KOG GC TEAM MASSFILE V.%d.%d.",MASSFILE_VERSION/10,MASSFILE_VERSION%10 );

	fwrite(&Header,sizeof(MASSFILE_HEADER),1,file);

	MASSFILE_MEMBERFILEINFO OutFile;
	MASSFILE_MEMBERFILEHEADER MemberFileHeader;

	for ( int i = 0; i < (int)pVecFile->size(); i++ )
	{
		fwrite(&MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER),1,file);
	}


	OutFile.dwFlag = MFI_REALDATA | MFI_NODECOMPRESS;

	for(int i = 0; i < (int)pVecFile->size(); i++)
	{
		char Filename[MASSFILE_NAMESIZE];
		GetFileName(Filename,(*pVecFile)[i]);
		if( !pMassFile->GetMemberFile(Filename,&OutFile) )
		{
			//실제 파일을 로딩해서 저장
			//파일이 안에 없음

			FileSize = GetFileSize((*pVecFile)[i]);
			if( FileSize < 0 )
			{
				std::wstring temp;
				ConvertCharToWCHAR( temp, Filename );
				temp += L" 파일이 존재하지 않습니다. - [참고] Lua Script 파일이라면 Lua컴파일이 실패되지는 않았는지 확인해주시기 바랍니다.";
				MessageBox( NULL, temp.c_str(), L"에러!", MB_OK );
				return false;
			}

			char* pFileData = (char*)m_BufferManager2.GetBuffer(compressBound(FileSize));
			char* pDestData = m_BufferManager1.GetBuffer(compressBound(FileSize+8));
			WriteByte = m_BufferManager1.GetBufferSize();
			FILE* memberFile = fopen((*pVecFile)[i],"rb");
			::ZeroMemory(pFileData,sizeof(char)*FileSize);

			fread(pFileData,sizeof(char),FileSize,memberFile);
			::ZeroMemory(pDestData,sizeof(char)*WriteByte);

			if(Z_OK	!= compress((BYTE*)pDestData,&WriteByte,(BYTE*)pFileData,FileSize))
			{
				fclose(file);
				fclose(memberFile);
				return false;
			}
			fwrite(pDestData,sizeof(char),WriteByte,file);
			fclose(memberFile);

		}
		else
		{
			//여기서는 바로 저장할 수 있다
			fwrite(OutFile.pRealData,sizeof(char),OutFile.CompressedSize,file);
			FileSize = OutFile.size;
			WriteByte = OutFile.CompressedSize;

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

bool CMassFile::ModifyMassFile( CMassFile* pMassFile, const char* strFileName, std::vector<char*> *pVecFile, int iSubFileSize )
{
	//int 
	//while ( true )
	//{
	//	std::ostringstream os;
	//	os << strFileName << "_" << 
	//	strSrcFilename +=

	//	LoadMassFile( strSrcFilename )
	//}

	////

	return ModifyMassFile( pMassFile, strFileName, pVecFile );
}

bool CMassFile::ModifyMassFile( char* strFilePath )
{
	std::vector<char*> vecFile;
	std::vector< MASSFILE_MEMBERFILEHEADER >::iterator vit;
	for( vit = m_vecMemberFileHeader.begin(); vit != m_vecMemberFileHeader.end(); ++vit )
	{
		MASSFILE_MEMBERFILEHEADER& info = *vit;
		vecFile.push_back( info.strFileName );
	}

	std::string strFile = strFilePath;
	strFile += m_strKomFileName;
	char* buf = const_cast<char*>( strFile.c_str() );

	return ModifyMassFile( this, buf, &vecFile );
}
#include "stdafx.h"
#include ".\kgcmassfilemanager.h"

KGCMassFileManager::KGCMassFileManager()
{
}

KGCMassFileManager::~KGCMassFileManager()
{
	for( int i = 0; i < (int)m_vecMassFile.size(); ++i )
	{
		delete m_vecMassFile[i];
	}
	m_vecMassFile.clear();
}

int  KGCMassFileManager::GetTotalFileCount()
{
	int Total = 0;
	for( int i = 0 ; i < (int)m_vecMassFile.size(); ++i )
	{
		Total += m_vecMassFile[i]->GetFileCount();
	}
	return Total;
}

void KGCMassFileManager::SetDataDirectory(const std::string& strDirectory)
{
	m_strDataDirectory = strDirectory;
}


CMassFile* KGCMassFileManager::AddMassFile(const std::string& strKomFileName)
{
	CMassFile* pMassFile = new CMassFile();
	if( !pMassFile->LoadMassFile(strKomFileName.c_str()) )
	{
		delete pMassFile;
		return NULL;
	}

	m_vecMassFile.push_back(pMassFile);

	CMassFile::MASSFILE_MEMBERFILEINFO Info;
	SMassFileIndex sFileIndex;
	//Real Data는 받아오지 않음
	ZeroMemory(&Info,sizeof(Info));

	for(int i = 0; i < (int)pMassFile->GetFileCount(); i++)
	{
		pMassFile->GetMemberFile(i,&Info);
		sFileIndex.Index = i;
		sFileIndex.pMassFile = pMassFile;
		m_mapMassFile[Info.strFileName] = sFileIndex;
	}

	return pMassFile;
}

bool KGCMassFileManager::LoadDataFile(const std::string& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO *pOut)
{
	if( m_mapMassFile.find(strFileName) == m_mapMassFile.end() )
	{	
		return LoadRealFile(strFileName,pOut);
	}
	else
	{	
		SMassFileIndex *sFileIndex = &m_mapMassFile[strFileName];		
		sFileIndex->pMassFile->GetMemberFile(sFileIndex->Index,pOut);
	}

	return true;
}

bool KGCMassFileManager::LoadRealFile(const std::string& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO *pOut)
{
	std::string strName = m_strDataDirectory + "\\" + strFileName;

	FILE* file = fopen(strName.c_str(),"rb");
	if(file == NULL)
	{
		MessageBoxA(NULL,strName.c_str(),"파일을 열 수 없습니다",0);
		return false;
	}	

	fseek(file,0,SEEK_END);
	pOut->size = ftell(file);
	fseek(file,0,SEEK_SET);

	if(pOut->dwFlag & MFI_REALDATA)
	{
		pOut->pRealData = (char*)m_BufferManager.GetBuffer(compressBound(pOut->size));
		fread(pOut->pRealData,1,pOut->size,file);
	}

	strcpy(pOut->strFileName, strFileName.c_str());

	fclose(file);
	return true;
}

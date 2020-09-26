#include "KVC_Imp.h"

#include <time.h>

KVC_Imp::KVC_Imp()
{
}

KVC_Imp::~KVC_Imp()
{
}

void KVC_Imp::Inc_EsdPchPath_VersionByTag( std::string strTag, std::string& strChunk )
{
	int iPosTag = (int) strChunk.find(strTag);
	if (iPosTag != std::string::npos)
	{
		std::string strFileVersion;
		std::string strBufTag_First = strChunk.substr(0, iPosTag + strTag.size());			

		std::istringstream is_Tag(strChunk.substr(iPosTag + strTag.size()));
		is_Tag >> strFileVersion;
		int iLastPos = (int) strFileVersion.find_last_of(">");
		strFileVersion = strFileVersion.substr(0, iLastPos);

		InitEsdVersion(strFileVersion);

		std::string strBufTag_Second = strChunk.substr(iPosTag + strTag.size() + strFileVersion.size());
		strFileVersion = m_strEsdVersion;

		std::ostringstream os_Tag;
		os_Tag << strFileVersion;

		strChunk = strBufTag_First + os_Tag.str() + strBufTag_Second;
	}
}

void KVC_Imp::Inc_EsdCfgGs_VersionByTag( std::string strTag, std::string& strChunk )
{
	int iPosTag = (int) strChunk.find(strTag);
	if (iPosTag != std::string::npos)
	{
		std::string strFileVersion;
		std::string strBufTag_First = strChunk.substr(0, iPosTag + strTag.size());			

		std::istringstream is_Tag(strChunk.substr(iPosTag + strTag.size()));
		is_Tag >> strFileVersion;
		InitEsdVersion(strFileVersion);

		std::string strBufTag_Second = strChunk.substr(iPosTag + strTag.size() + strFileVersion.size() + 1);
		strFileVersion = m_strEsdVersion;

		std::ostringstream os_Tag;
		os_Tag << " " << strFileVersion;

		strChunk = strBufTag_First + os_Tag.str() + strBufTag_Second;
	}
}

void KVC_Imp::Inc_EsdCfg_VersionByTag( std::string strTag, std::string& strChunk )
{
	int iPosTag = (int) strChunk.find(strTag);
	if (iPosTag != std::string::npos)
	{
		std::string strFileVersion;
		std::string strBufTag_First = strChunk.substr(0, iPosTag + strTag.size());			

		std::istringstream is_Tag(strChunk.substr(iPosTag + strTag.size()));
		is_Tag >> strFileVersion;
		int iBFPos = (int) strFileVersion.find("\"");
		int iBLPos = (int) strFileVersion.find_last_of("\"");

		std::string strBF = strFileVersion.substr(0, iBFPos + 1);
		std::string strBL = strFileVersion.substr(iBLPos, strFileVersion.size());
		strFileVersion = strFileVersion.substr(iBFPos + 1, iBLPos - (iBFPos + 1));

		InitEsdVersion(strFileVersion);

		strBufTag_First += strBF;
		std::string strBufTag_Second = strBL + strChunk.substr(
			iPosTag + strTag.size() + strFileVersion.size() + strBF.size() + 2);
		strFileVersion = m_strEsdVersion;

		std::ostringstream os_Tag;
		os_Tag << strFileVersion;

		strChunk = strBufTag_First + os_Tag.str() + strBufTag_Second;
	}
}

void KVC_Imp::InitEsdVersion(std::string strVersion)
{
	if (m_strEsdVersion.empty() == false)
		return;

	int iPosNo = (int) strVersion.rfind("_");
	if (iPosNo != std::string::npos)
	{
		std::string strBufDate = strVersion.substr(0, iPosNo);
		std::string strBufNo = strVersion.substr(iPosNo + 1, strVersion.size());

		time_t t0 = time(0);
		tm* t = localtime(&t0);

		std::ostringstream os_Version;
		os_Version 
			<< t->tm_year + 1900 
			<< std::setw( 2 ) << std::setfill( '0' ) << t->tm_mon + 1 
			<< std::setw( 2 ) << std::setfill( '0' ) << t->tm_mday;

		int iNo = atoi( strBufNo.c_str() );
		if ( os_Version.str() != strBufDate )
			iNo = 0;
		
		os_Version << "_" << iNo;
		
		m_strEsdVersion = os_Version.str();
	}
}

void KVC_Imp::ProcessFile(char* pcFilename, std::string strExt)
{
	// 파일 사이즈 얻음
	FILE* pfFile = fopen(pcFilename, "r");
	if ( pfFile == NULL )
		return;

	fseek(pfFile, 0, SEEK_END);
	int iFileSize = ftell(pfFile);
	fseek(pfFile, 0, SEEK_SET);

	// 버퍼에 파일 로딩
	char* pcBuf = new char[iFileSize + 1];
	memset(pcBuf, 0, iFileSize);
	pcBuf[iFileSize] = 0;

	fread(pcBuf, iFileSize, 1, pfFile);
	fclose(pfFile);

	// Bak 파일 생성
	{
		std::string strBakFilename = pcFilename;
		strBakFilename += ".bak";

		pfFile = fopen(strBakFilename.c_str(), "w");
		fwrite(pcBuf, iFileSize, 1, pfFile);
		fclose(pfFile);
	}

	// Write 용 파일 오픈
	pfFile = fopen(pcFilename, "w");
	std::string strChunk = pcBuf;

	// 각 파일에 대한 처리
	if (strExt == "EsdCfg")
	{
		Inc_EsdCfg_VersionByTag("g_pMain:SetClientVersion_LUA(", strChunk);
	}
	else if (strExt == "EsdCfg_gs")
	{
		Inc_EsdCfgGs_VersionByTag("Main Version =", strChunk);
	}
	else if (strExt == "EsdPchPath")
	{
		Inc_EsdPchPath_VersionByTag("Patch/", strChunk);
	}

	if (strChunk.empty() == false)
		fwrite(strChunk.c_str(), strChunk.size(), 1, pfFile);

	fclose(pfFile);
	delete [] pcBuf;
}


#include "KVC_Imp.h"
#include "KFilename.h"
#include "KStringUtil.h"

void main(int argc, char** argv)
{
	KVC_Imp kVC_Imp;

	if ( argc > 1 )
	{
		kVC_Imp.SetEsdVersion( argv[1] );
	}

	std::fstream fin( "KVCConfig.ini", std::ios::in );

	if ( fin.good() == false )
	{
		return;
	}

	while ( true )
	{
		if ( fin.eof() == true )
			break;

		std::string strTag;
		std::string strData;

		fin >> strTag;

		char acBuf[256] = {0};
		fin.getline(acBuf, 256);
		strData = &acBuf[1];

		// 파일 처리
		KRemoveBlankString(strData);
		kVC_Imp.ProcessFile((char*) strData.c_str(), strTag);
	}

	fin.close();
}  
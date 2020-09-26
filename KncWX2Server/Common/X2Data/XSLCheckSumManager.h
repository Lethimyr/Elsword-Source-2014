#pragma once

#include <Windows.h>

#include <vector>
#include <map>
#include <string>

#include "KNCSingleton.h"



class CXSLCheckSumManager
{
	DeclareSingleton( CXSLCheckSumManager );

	public:

		CXSLCheckSumManager(void);
		~CXSLCheckSumManager(void);

		bool OpenScriptFile( const WCHAR* pFileName );

		bool GetIsValidCheckSum( const std::map< std::wstring, std::wstring >& mapCheckSum );
	
	protected:

		std::wstring GetCheckSum( const WCHAR* pFileName );
		bool AddCheckSum( const WCHAR* pFileName, const WCHAR* pCheckSum );

		std::map< std::wstring, std::wstring >	m_mapCheckSum;			//fileName, CRC

		bool									m_bLoadCheckSumFile;
};

DefSingletonInline( CXSLCheckSumManager );


#include ".\XSLCheckSumManager.h"

#include "../xml/Markup.h"

ImplementSingleton( CXSLCheckSumManager );

CXSLCheckSumManager::CXSLCheckSumManager(void)
{
	m_bLoadCheckSumFile = false;
}

CXSLCheckSumManager::~CXSLCheckSumManager(void)
{
	
}

bool CXSLCheckSumManager::OpenScriptFile( const WCHAR* pFileName )
{
	CMarkup xml;
	if ( xml.Load( pFileName ) == false )
		return false;

	m_bLoadCheckSumFile = true;

	xml.FindElem( L"PatchInfo" );

	if( xml.FindChildElem( L"File" ) )
	{
		xml.IntoElem();

		while( xml.FindChildElem( L"Item" ) )
		{
			xml.IntoElem();

			std::wstring fileFullName = xml.GetAttrib( L"Name" );
			std::wstring fileName;

			int tempStringIndex = fileFullName.rfind( L"\\" );
			if ( tempStringIndex == -1 )
			{
				fileName = fileFullName;
			}
			else
			{
				fileName = fileFullName.substr( tempStringIndex + 1 );
			}

			//m_mapCheckSum.insert( make_pair( fileName, xml.GetAttrib( L"CheckSum" ) ) );
			
			std::wstring checkSum = xml.GetAttrib( L"CheckSum" );

			for( std::wstring::iterator i = fileName.begin(); i != fileName.end(); i++ ) 
				*i = (WCHAR)towupper(*i);

			if ( AddCheckSum( fileName.c_str(), checkSum.c_str() ) == false )
			{
				MessageBox( NULL, fileName.c_str(), L"중복된 체크섬 파일 발견", MB_OK );
			}

			xml.OutOfElem();
		}

		xml.OutOfElem();
	}

	return true;
}

bool CXSLCheckSumManager::GetIsValidCheckSum( const std::map< std::wstring, std::wstring >& mapCheckSum )
{
	if ( m_bLoadCheckSumFile == false )
		return true;

	std::map< std::wstring, std::wstring >::const_iterator mit;
	
	for ( mit = mapCheckSum.begin(); mit != mapCheckSum.end(); mit++ )
	{
		std::wstring fileName = mit->first;

		std::map< std::wstring, std::wstring >::iterator mit2;
		mit2 = m_mapCheckSum.find( fileName );

		if ( mit2 != m_mapCheckSum.end() )
		{
			std::wstring mitCheckSum = mit->second;
			std::wstring mit2CheckSum = mit2->second;

			if ( mitCheckSum != mit2CheckSum )
			{
				return false;
			}
		}
	}

	return true;
}

std::wstring CXSLCheckSumManager::GetCheckSum( const WCHAR* pFileName )
{
	std::map< std::wstring, std::wstring >::iterator mit;

	mit = m_mapCheckSum.find( pFileName );
	if ( mit != m_mapCheckSum.end() )
	{
		return mit->second;
	}

	return L"";
}

bool CXSLCheckSumManager::AddCheckSum( const WCHAR* pFileName, const WCHAR* pCheckSum )
{
	std::map< std::wstring, std::wstring >::iterator mit;

	mit = m_mapCheckSum.find( pFileName );
	if ( mit != m_mapCheckSum.end() )
	{
		return false;
	}
	
	m_mapCheckSum.insert( std::make_pair( pFileName, pCheckSum ) );

	return true;
}


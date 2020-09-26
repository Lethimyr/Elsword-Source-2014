#include "stdafx.h"
#include <vector>
#include <time.h>
#include "SecurityString.h"

KSecurityString::KSecurityString( bool bSrand /*= false*/ )
{
	if( bSrand ) SRand();
}

KSecurityString::KSecurityString( const strType& strObj, bool bSrand /*= false*/ )
{
	if( bSrand ) SRand();
	Init( strObj );
}

KSecurityString::~KSecurityString(void)
{
}

void KSecurityString::SRand()
{
	srand( (unsigned) time( NULL ) );
}

void KSecurityString::Init( const strType& strObj )
{
	m_mapProtectBuf.clear();

	std::map< unsigned int, strType::value_type > mapTemp;
	std::vector< unsigned int > vecIndexList;

	for( unsigned int ui = 0; ui < strObj.length(); ++ui )
	{
		mapTemp.insert( std::make_pair( ui, strObj[ui] ) );
		vecIndexList.push_back( ui );
	}

	while( !vecIndexList.empty() )
	{
		unsigned int index = static_cast<unsigned int>( rand() % vecIndexList.size() );
		std::map< unsigned int, strType::value_type >::iterator mit = mapTemp.find( vecIndexList[index] );
		if( mit == mapTemp.end() )
			break;

		m_mapProtectBuf.insert( std::make_pair( mit->first, mit->second ) );
		vecIndexList.erase( vecIndexList.begin() + index );
	}
}

strType KSecurityString::GetString() const
{
	strType strTemp;

	std::map< unsigned int, strType::value_type >::const_iterator mit;
	for( mit = m_mapProtectBuf.begin(); mit != m_mapProtectBuf.end(); ++mit )
	{
		strTemp += mit->second;
	}

	return strTemp;
}

KClearString KSecurityString::GetClearString() const
{
	KClearString strTemp;

	std::map< unsigned int, strType::value_type >::const_iterator mit;
	for( mit = m_mapProtectBuf.begin(); mit != m_mapProtectBuf.end(); ++mit )
	{
		strTemp += mit->second;
	}

	return strTemp;
}

//void KSecurityString::Init( const strType& strObj )
//{
//	m_mapProtectBuf.clear();
//
//	strType::value_type vtMaxValue = vtMaxValue | 0xFFFFFFFF;
//
//	for( unsigned int ui = 0; ui < strObj.length(); ++ui )
//	{
//		unsigned int uiFirst = ui*2;
//		unsigned int uiSecond = ui*2 + 1;
//		strType::value_type vtDummy = rand() % vtMaxValue;
//
//		m_mapProtectBuf.insert( std::make_pair( uiFirst, strObj[ui] ) );
//		m_mapProtectBuf.insert( std::make_pair( uiSecond, vtDummy ) );
//	}
//}

//void KSecurityString::Init( const KClearString& strObj )
//{
//	m_mapProtectBuf.clear();
//
//	strType::value_type vtMaxValue = vtMaxValue | 0xFFFFFFFF;
//
//	for( unsigned int ui = 0; ui < strObj.length(); ++ui )
//	{
//		unsigned int uiFirst = ui*2;
//		unsigned int uiSecond = ui*2 + 1;
//		strType::value_type vtDummy = rand() % vtMaxValue;
//
//		m_mapProtectBuf.insert( std::make_pair( uiFirst, strObj[ui] ) );
//		m_mapProtectBuf.insert( std::make_pair( uiSecond, vtDummy ) );
//	}
//}

//strType KSecurityString::GetString() const
//{
//	strType strTemp;
//
//	std::map< unsigned int, strType::value_type >::const_iterator mit;
//	for( mit = m_mapProtectBuf.begin(); mit != m_mapProtectBuf.end(); ++mit )
//	{
//		if( ( mit->first % 2 ) == 1 )
//			continue;
//
//		strTemp += mit->second;
//	}
//
//	return strTemp;
//}
//
//KClearString KSecurityString::GetClearString() const
//{
//	KClearString strTemp;
//
//	std::map< unsigned int, strType::value_type >::const_iterator mit;
//	for( mit = m_mapProtectBuf.begin(); mit != m_mapProtectBuf.end(); ++mit )
//	{
//		if( ( mit->first % 2 ) == 1 )
//			continue;
//
//		strTemp += mit->second;
//	}
//
//	return strTemp;
//}





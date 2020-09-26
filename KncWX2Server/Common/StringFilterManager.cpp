#include ".\stringfiltermanager.h"

#ifdef SERV_STRING_FILTER_USING_DB

ImplementSingleton( KStringFilterManager );

KStringFilterManager::KStringFilterManager(void)
{
}

KStringFilterManager::~KStringFilterManager(void)
{
}

ImplToStringW( KStringFilterManager )
{
	return stm_;
}

void KStringFilterManager::Init()
{
	StringFilterVector vecStringFilter;
	SiCXSLStringFilter()->GetStringFilter( vecStringFilter );

	START_LOG( cerr, L"SiCXSLStringFilter()->GetStringFilter( vecStringFilter );" )
		<< BUILD_LOG( vecStringFilter.size() )
		<< END_LOG;

	SetStringFilter( vecStringFilter );

	m_iReleaseTick = 0;

	// 바로 한번 수행해 주자.
	SendToScriptDB( DBE_CHECK_STRING_FILTER_UPDATE_REQ );
	m_tReleaseTickTimer.restart();
}

void KStringFilterManager::Tick()
{
	if( m_tReleaseTickTimer.elapsed() > 60.0f )
	{
		SendToScriptDB( DBE_CHECK_STRING_FILTER_UPDATE_REQ );
		m_tReleaseTickTimer.restart();
	}
}

void KStringFilterManager::SetStringFilter( IN const StringFilterVector& vecStringFilter )
{
	m_BanNickNameList.clear();
	m_BanNickNameWordList.clear();
	m_BanWordList.clear();
	m_BanPostWordList.clear();

	StringFilterVector::const_iterator vitStringFilter;
	for( vitStringFilter = vecStringFilter.begin(); vitStringFilter != vecStringFilter.end(); ++vitStringFilter )
	{
		CXSLStringFilter::FILTER_WORD_TYPE eFilterType = static_cast< CXSLStringFilter::FILTER_WORD_TYPE >( vitStringFilter->m_sStringFilterType );
		std::wstring wstrFilterString = vitStringFilter->m_wstrFilter;
		MakeUpperCase( wstrFilterString );

		switch( eFilterType )
		{
		case CXSLStringFilter::FWT_NICKNAME:
			m_BanNickNameList.push_back( wstrFilterString );
			break;
		case CXSLStringFilter::FWT_NICKNAMEWORD:
			m_BanNickNameWordList.push_back( wstrFilterString );
			break;
		case CXSLStringFilter::FWT_WORD:
			m_BanWordList.push_back( wstrFilterString );
			break;
		case CXSLStringFilter::FWT_POSTWORD:
			m_BanPostWordList.push_back( wstrFilterString );
			break;
		default:
			break;
		}
	}
}

void KStringFilterManager::GetStringFilter( OUT StringFilterVector& vecStringFilter )
{
	std::vector< std::wstring >::iterator vit;
	KStringFilterInfo kStringFilter;
	
	kStringFilter.m_sStringFilterType = CXSLStringFilter::FWT_NICKNAME;
	for( vit = m_BanNickNameList.begin(); vit != m_BanNickNameList.end(); ++vit )
	{
		kStringFilter.m_wstrFilter = *vit;
		vecStringFilter.push_back( kStringFilter );
	}

	kStringFilter.m_sStringFilterType = CXSLStringFilter::FWT_NICKNAMEWORD;
	for( vit = m_BanNickNameWordList.begin(); vit != m_BanNickNameWordList.end(); ++vit )
	{
		kStringFilter.m_wstrFilter = *vit;
		vecStringFilter.push_back( kStringFilter );
	}

	kStringFilter.m_sStringFilterType = CXSLStringFilter::FWT_WORD;
	for( vit = m_BanWordList.begin(); vit != m_BanWordList.end(); ++vit )
	{
		kStringFilter.m_wstrFilter = *vit;
		vecStringFilter.push_back( kStringFilter );
	}

	kStringFilter.m_sStringFilterType = CXSLStringFilter::FWT_POSTWORD;
	for( vit = m_BanPostWordList.begin(); vit != m_BanPostWordList.end(); ++vit )
	{
		kStringFilter.m_wstrFilter = *vit;
		vecStringFilter.push_back( kStringFilter );
	}
}

bool KStringFilterManager::CheckIsValidPostWordString( CXSLStringFilter::FILTER_TYPE filterType, const WCHAR* pString, OUT std::wstring &pOutString )
{
	std::wstring pStringForCheck = pString;
	MakeUpperCase(pStringForCheck);

	if ( filterType == CXSLStringFilter::FT_POST )
	{
		for ( int i = 0; i < (int)m_BanPostWordList.size(); i++ )
		{
			std::wstring banPostWord = m_BanPostWordList[i];
			if ( (int)pStringForCheck.find( banPostWord.c_str() ) != -1 ) //찾았다
			{
				pOutString = banPostWord;
				return false;
			}
		} 
	}

	return true;
}
bool KStringFilterManager::CheckIsValidString( CXSLStringFilter::FILTER_TYPE filterType, std::wstring& wstringForCheck )
{
	// 일단 스크립트에서도 걸리는 놈들은 바로 킥
	if( SiCXSLStringFilter()->CheckIsValidString( filterType, wstringForCheck ) == false )
		return false;

	std::wstring pStringForCheck = wstringForCheck;
	MakeUpperCase( pStringForCheck );

	if ( filterType == CXSLStringFilter::FT_NICKNAME )
	{
		for ( int i = 0; i < (int)m_BanNickNameList.size(); i++ )
		{
			std::wstring banNickName = m_BanNickNameList[i];
			if ( pStringForCheck == banNickName )
			{
				return false;
			}
		}

		//{{ 2009. 2. 17  최육사	닉네임 포함 문자
		for( int i = 0; i < (int)m_BanNickNameWordList.size(); ++i )
		{
			std::wstring banNickNameWord = m_BanNickNameWordList[i];
			if( (int)pStringForCheck.find( banNickNameWord.c_str() ) != -1 ) // 찾았다
			{
				return false;
			}
		}
		//}}

		for ( int i = 0; i < (int)m_BanWordList.size(); i++ )
		{
			std::wstring banWord = m_BanWordList[i];
			if ( (int)pStringForCheck.find( banWord.c_str() ) != -1 ) //찾았다
			{
				return false;
			}
		}
	}
	else if ( filterType == CXSLStringFilter::FT_CHAT )
	{
		for ( int i = 0; i < (int)m_BanWordList.size(); i++ )
		{
			std::wstring banWord = m_BanWordList[i];
			if ( (int)pStringForCheck.find( banWord.c_str() ) != -1 ) //찾았다
			{
				return false;
			}
		}
	}
	else if ( filterType == CXSLStringFilter::FT_POST )
	{
		for ( int i = 0; i < (int)m_BanPostWordList.size(); i++ )
		{
			std::wstring banPostWord = m_BanPostWordList[i];
			if ( (int)pStringForCheck.find( banPostWord.c_str() ) != -1 ) //찾았다
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

std::wstring KStringFilterManager::FilteringNoteString( const WCHAR* pString, WCHAR wcharForReplace )
{
	if ( pString == NULL )
	{
		return std::wstring( L"" );
	}

	std::wstring stringForCheck = pString;
	std::wstring orgStringForCheck = pString;
	MakeUpperCase( stringForCheck );

	for ( int i = 0; i < (int)m_BanWordList.size(); i++ )
	{
		std::wstring banWord = m_BanWordList[i];
		int iLocation = 0;

		if(banWord.compare(L"\n") == 0)
			continue;

		while( (int)stringForCheck.find( banWord.c_str(), iLocation ) != -1 ) //찾았다
		{
			int index = stringForCheck.find( banWord.c_str(), iLocation );
			int banWordSize = (int)banWord.size();
			WCHAR buff[256] = {0};
			for ( int i = 0; i < banWordSize; i++ )
			{
				buff[i] = wcharForReplace;
				buff[i + 1] = L'\0';
			}
			std::wstring stringForReplace = buff;
			orgStringForCheck.replace( index, banWordSize, stringForReplace );
			//stringForCheck = orgStringForCheck;
			iLocation = index + banWordSize;
		}
	}

	return orgStringForCheck;
}

std::wstring KStringFilterManager::FilteringChatString( const WCHAR* pString, WCHAR wcharForReplace )
{
	if ( pString == NULL )
	{
		return std::wstring( L"" );
	}

	//{{ 허상형 : [2010/3/15/] //	필터링 수정
	std::wstring stringOriginal = pString;
	//}} 허상형 : [2010/3/15/] //	
	std::wstring stringForCheck = pString;
	std::wstring orgStringForCheck = pString;
	MakeUpperCase( stringForCheck );

	int nStringLength = lstrlen(pString);
	for(int i=0; i<nStringLength; ++i)
	{
		if( pString[i] == 0x1C ||
			pString[i] == 0x1D ||
			pString[i] == 0x1E ||
			pString[i] == 0x1F )
		{			
			return L"♡♡♡!";
		}
	}

	for ( int i = 0; i < (int)m_BanWordList.size(); i++ )
	{
		std::wstring banWord = m_BanWordList[i];
		int iLocation = 0;

		while( (int)stringForCheck.find( banWord.c_str(), iLocation ) != -1 ) //찾았다
		{
			int index = stringForCheck.find( banWord.c_str(), iLocation );
			int banWordSize = (int)banWord.size();
			WCHAR buff[256] = {0};
			for ( int i = 0; i < banWordSize; i++ )
			{
				buff[i] = wcharForReplace;
				buff[i + 1] = L'\0';
			}
			std::wstring stringForReplace = buff;
			orgStringForCheck.replace( index, banWordSize, stringForReplace );
			//stringForCheck = orgStringForCheck;
			iLocation = index + banWordSize;
		}
	}

	return orgStringForCheck;
}

#endif //SERV_STRING_FILTER_USING_DB
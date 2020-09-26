#include "StdAfx.h" // 엘소드는 PCH(precompiled header)로 고정된 이름 stdafx.h를 사용한다.
#include "kluamanagerproxy.h"

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>


bool    KLuaManagerProxyLuaValue::LuaPush( lua_State* L ) const
{
    if ( L == NULL )
        return false;
    switch( m_eType )
    {
	case LUA_VALUE_NIL:
        lua_pushnil( L );
        break;
	case LUA_VALUE_BOOLEAN:
        lua_pushboolean( L, m_bBoolean );
        break;
	case LUA_VALUE_NUMBER:
        lua_pushnumber( L, m_dNumber );
        break;
	case LUA_VALUE_STRING:
        lua_pushlstring( L, m_strStringUTF8.c_str(), m_strStringUTF8.size() );
        break;
    default:
        ASSERT( 0 );
        lua_pushnil( L );
        break;
    }//switch
    return true;
}


bool    KLuaManagerProxyLuaValue::LuaResetFromTop( lua_State* L )
{
    if ( L == NULL )
        return false;
    switch( lua_type( L, -1 ) )
    {
    case LUA_TNIL:
        Reset();
        return true;
    case LUA_TNUMBER:
        Reset( (double) lua_tonumber( L, -1 ) );
        return true;
    case LUA_TBOOLEAN:
        Reset( ( lua_toboolean( L, -1 ) ) ? true : false );
        return true;
    case LUA_TSTRING:
        Reset( lua_tostring( L, -1 ) );
        return true;
    }
    return false;
}


KLuaManagerProxy::KLuaManagerProxy( lua_State* L )
    : m_pLuaState ( L )
{
	m_dequeLuaTable.push_back( new KLuaManagerProxyLuaTable );
}


  KLuaManagerProxy::KLuaManagerProxy( lua_State* L, KLuaManagerProxy& In_ )
    : m_pLuaState ( L )
{
	KLuaManagerProxyLuaTable* spLuaTable = In_.GetStackFront();
	ASSERT( spLuaTable );
	if ( spLuaTable )
		m_dequeLuaTable.push_back( spLuaTable );
	else
		m_dequeLuaTable.push_back( new KLuaManagerProxyLuaTable );
}//KLuaManagerProxy::KLuaManagerProxy()


KLuaManagerProxy::~KLuaManagerProxy(void)
{
	m_dequeLuaTable.resize( 0 );
}

//////////////////////////////////////////////////////////////////////////
bool KLuaManagerProxy::BeginTable( const char* pszName, int index, bool bLogOutput /*= true */ )
{
    if ( pszName == NULL || pszName[0] == NULL )
        return false;

	char szText[255] = {0,};
	StringCchPrintfA( szText, 255, "%s%d", pszName, index );

	return BeginTable( szText );
}


bool KLuaManagerProxy::BeginTable( const char* pszName, bool bLogOutput /*= true*/ )
{
	KLuaManagerProxyLuaTable* spLuaTable = StackGetTable( pszName );
	if( spLuaTable != NULL )
	{
		StackPush( spLuaTable );
		return true;
	}

	return false;
}

bool KLuaManagerProxy::BeginTable( int iIndex, bool bLogOutput /*= true*/ )
{
	KLuaManagerProxyLuaTable* spLuaTable = StackGetTable( iIndex );
	if( spLuaTable != NULL )
	{
		StackPush( spLuaTable );
		return true;
	}

	return false;
}


bool KLuaManagerProxy::BeginTable( KLuaManagerProxyLuaTable* table )
{
    if ( table != NULL )
    {
        StackPush( table );
        return true;
    }//
    return false;
}//KLuaManagerProxy::BeginTable()

bool KLuaManagerProxy::EndTable( bool bLogOutput /*= true*/ )
{
	if ( m_dequeLuaTable.size() <= 1 ) 
		return false;
	return StackPop();
}


bool KLuaManagerProxy::GetValue( const char* pszName, bool& bValue, bool bLogOutput /*= true*/ )
{
	if( NULL == pszName )
		return false;

	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( pszName );
	return GetBooleanFromLuaValue( bValue, pLuaValue );
}

bool KLuaManagerProxy::GetValue( int iIndex, bool& bValue, bool bLogOutput /*= true*/ )
{
	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( iIndex );
	return GetBooleanFromLuaValue( bValue, pLuaValue );
}

//bool KLuaManagerProxy::GetValue( const char* pszName, char* szValue, int bufferSize, bool bLogOutput /*= true*/ )
//{
//	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( pszName );
//	return	GetStringFromLuaValue( szValue, bufferSize, pLuaValue );
//}
//
//
//bool KLuaManagerProxy::GetValue( const char* pszName, std::string& strValue, bool bLogOutput /*= true*/ )
//{
//	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( pszName );
//	return	GetStringFromLuaValue( strValue, pLuaValue );
//}
//
//
//bool KLuaManagerProxy::GetValue( int iIndex, char* szValue, int bufferSize, bool bLogOutput /*= true*/ )
//{
//	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( iIndex );
//	return	GetStringFromLuaValue( szValue, bufferSize, pLuaValue );
//}
//
//
//
//bool KLuaManagerProxy::GetValue( int iIndex, std::string& strValue, bool bLogOutput /*= true*/ )
//{
//	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( iIndex );
//	return	GetStringFromLuaValue( strValue, pLuaValue );
//}



bool KLuaManagerProxy::GetValue( const char* pszName, wchar_t* wszValue, int bufferSize, bool bLogOutput /*= true*/ )
{
	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( pszName );
	return	GetStringFromLuaValue( wszValue, bufferSize, pLuaValue );
}

bool KLuaManagerProxy::GetValue( const char* pszName, std::wstring& wstrValue, bool bLogOutput /*= true*/ )
{
	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( pszName );
	return	GetStringFromLuaValue( wstrValue, pLuaValue );
}

bool KLuaManagerProxy::GetValue( int iIndex, wchar_t* wszValue, int bufferSize, bool bLogOutput /*= true*/ )
{
	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( iIndex );
	return	GetStringFromLuaValue( wszValue, bufferSize, pLuaValue );
}

bool KLuaManagerProxy::GetValue( int iIndex, std::wstring& wstrValue, bool bLogOutput /*= true*/ )
{
	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( iIndex );
	return	GetStringFromLuaValue( wstrValue, pLuaValue );
}



//////////////////////////////////////////////////////////////////////////
void KLuaManagerProxy::StackPush( KLuaManagerProxyLuaTable* table )
{
	m_dequeLuaTable.push_back( table );
}

bool KLuaManagerProxy::StackPop()
{
	// stack에 최소 1개의 table은 남아있어야 한다.
	assert( m_dequeLuaTable.size() > 1 );
	if( m_dequeLuaTable.size() <= 1 )
	{
		return false;
	}

	m_dequeLuaTable.pop_back();
	return true;
}


KLuaManagerProxyLuaValue* KLuaManagerProxy::StackGetValue( const char* valueName )
{
    if ( valueName == NULL || valueName[0] == NULL )
        return NULL;

    LuaName2ValueMap::iterator it = m_dequeLuaTable.back()->m_map.find( ( valueName ) );

	if( it != m_dequeLuaTable.back()->m_map.end() )
	{
		return &(it->second);
	}
	else
	{
		return NULL;
	}
}


KLuaManagerProxyLuaValue* KLuaManagerProxy::StackGetValue( const std::string& valueName )
{
    if ( valueName.empty() == true )
        return NULL;

    LuaName2ValueMap::iterator it = m_dequeLuaTable.back()->m_map.find( ( valueName ) );

	if( it != m_dequeLuaTable.back()->m_map.end() )
	{
		return &(it->second);
	}
	else
	{
		return NULL;
	}
}



KLuaManagerProxyLuaValue* KLuaManagerProxy::StackGetValue( const int valueIndex )
{
	//assert( valueIndex < 0 || valueIndex >= m_dequeLuaTable.back()->m_vec.size() );
	if( valueIndex <= 0 || valueIndex > (int)m_dequeLuaTable.back()->m_vec.size() )
		return NULL;
	else
		return &(m_dequeLuaTable.back()->m_vec[valueIndex-1]);
}


unsigned    KLuaManagerProxy::GetNumIndexedTables()
{
    int iNumTable = 0;
    while( 1 )
    {
	    KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( iNumTable + 1 );
        if ( NULL == pLuaValue
            || pLuaValue->m_eType != LUA_VALUE_TABLE )
            return iNumTable;
        iNumTable++;
    }//while

    return  (unsigned) iNumTable;
}//KLuaManagerProxy::GetNumIndexedTables()



unsigned    KLuaManagerProxy::GetNumIndexedTables( const char* pszName )
{
    if ( pszName == NULL || pszName[0] == NULL )
        return 0;

	char szText[MAX_PATH] = {0,};

    int iNumTable = 0;
    while( 1 )
    {
	    StringCchPrintfA( szText, MAX_PATH, "%s%d", pszName, iNumTable );
	    KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( szText );
        if ( NULL == pLuaValue
            || pLuaValue->m_eType != LUA_VALUE_TABLE )
            return iNumTable;
        iNumTable++;
    }//while

    return  (unsigned) iNumTable;
}//KLuaManagerProxy::GetNumIndexedTables()


unsigned    KLuaManagerProxy::GetNumIndexedValues( const char* pszName, int iBaseIndex )
{
    if ( pszName == NULL || pszName[0] == NULL || iBaseIndex < 0 )
        return 0;

	char szText[MAX_PATH] = {0,};

    int iNumIndex = iBaseIndex;
    while( 1 )
    {
	    StringCchPrintfA( szText, MAX_PATH, "%s%d", pszName, iNumIndex );
	    KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( szText );
        if ( NULL == pLuaValue )
            return (unsigned) ( iNumIndex - iBaseIndex );
        iNumIndex++;
    }//while

    return  (unsigned) ( iNumIndex - iBaseIndex );
}//KLuaManagerProxy::GetNumIndexedValues()



KLuaManagerProxyLuaTable* KLuaManagerProxy::StackGetTable( const char* tableName )
{
	if ( tableName == NULL || tableName[0] == NULL )
		return NULL;

	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( tableName );
	if( NULL != pLuaValue &&
		LUA_VALUE_TABLE == pLuaValue->m_eType )
	{
		return pLuaValue->m_spTable.get();
	}
	else
	{
		return NULL;
	}
}

KLuaManagerProxyLuaTable* KLuaManagerProxy::StackGetTable( const int tableIndex )
{
	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( tableIndex );
	if( NULL != pLuaValue &&
		LUA_VALUE_TABLE == pLuaValue->m_eType )
	{
		return pLuaValue->m_spTable.get();
	}
	else
	{
		return NULL;
	}

}





bool KLuaManagerProxy::GetBooleanFromLuaValue( bool& val, KLuaManagerProxyLuaValue* pLuaValue )
{
	if( NULL != pLuaValue )
	{
		switch( pLuaValue->m_eType )
		{
		case LUA_VALUE_NIL:
			val = false;
			return true;
		case LUA_VALUE_BOOLEAN:
			val = pLuaValue->m_bBoolean;
			return true;
		case LUA_VALUE_NUMBER:
			val = ( static_cast<int>( pLuaValue->m_dNumber ) != 0 );
			return true;
		}//switch
		//val = true;
		//return true;
	}

	return false;
}



//bool	KLuaManagerProxy::GetStringFromLuaValue( std::string& strValue, KLuaManagerProxyLuaValue* pLuaValue )
//{
//	if ( NULL != pLuaValue )
//	{
//		switch( pLuaValue->m_eType )
//		{
//		case LUA_VALUE_NUMBER:
//			strValue = boost::lexical_cast<std::string>( pLuaValue->m_dNumber );
//			return true;
//		case LUA_VALUE_STRING:
//            {
//                size_t tSize = strValue.size() + 1;
//                wchar_t* wszBuffer = ( wchar_t* )_malloca( tSize * sizeof( wchar_t ) );
//                ASSERT( wszBuffer != NULL );
//                size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strValue.c_str(), strValue.size(), wszBuffer, ( int )tSize );
//                char* szBuffer = ( char* )_malloca( tNewSize * sizeof( char ) );
//                ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
//                strValue = szBuffer;
//
//			    strValue = pLuaValue->m_strStringUTF8;
//            }
//			return true;
//		}//switch
//		return false;
//	}
//
//	return false;
//}
//
//
//bool	KLuaManagerProxy::GetStringFromLuaValue( char* szValue, int bufferSize, KLuaManagerProxyLuaValue* pLuaValue )
//{
//	if ( NULL != pLuaValue )
//	{
//		switch( pLuaValue->m_eType )
//		{
//		case LUA_VALUE_NUMBER:
//			strncpy( szValue, boost::lexical_cast<char*>( pLuaValue->m_dNumber ), bufferSize-1 );
//			szValue[ bufferSize-1 ] = NULL;
//			return true;
//		case LUA_VALUE_STRING:
//			strncpy( szValue, pLuaValue->m_strStringUTF8.c_str(), bufferSize-1 );
//			szValue[ bufferSize-1 ] = NULL;
//			return true;
//		}//switch
//		return false;
//	}
//
//	return false;
//}



bool	KLuaManagerProxy::GetStringFromLuaValue( std::wstring& wstrValue, KLuaManagerProxyLuaValue* pLuaValue )
{
	if ( NULL != pLuaValue )
	{
		switch( pLuaValue->m_eType )
		{
		case LUA_VALUE_NUMBER:
			wstrValue = boost::lexical_cast<std::wstring>( pLuaValue->m_dNumber );
			return true;
		case LUA_VALUE_STRING:
            {
                if ( pLuaValue->m_strStringUTF8.empty() == true )
                {
                    wstrValue.resize(0);
                    return true;
                }
                int iDestSize = ::MultiByteToWideChar( CP_UTF8, 0, pLuaValue->m_strStringUTF8.c_str(), pLuaValue->m_strStringUTF8.size(), NULL, 0 );
                if ( iDestSize <= 0 )
                {
                    wstrValue.resize(0);
                    return true;
                }//if
                wchar_t* wcharBuf = (wchar_t*) _malloca( iDestSize * sizeof( wchar_t ) );
                ASSERT( wcharBuf != NULL );
                int iNewSize = ::MultiByteToWideChar( CP_UTF8, 0, pLuaValue->m_strStringUTF8.c_str(), pLuaValue->m_strStringUTF8.size(), wcharBuf, iDestSize );
                ASSERT( iNewSize == iDestSize );
                wstrValue.assign( wcharBuf, iNewSize );
                _freea( wcharBuf );
            }
			return true;
		}//switch
		return false;
	}

	return false;
}


bool	KLuaManagerProxy::GetStringFromLuaValue( wchar_t* wszValue, int bufferSize, KLuaManagerProxyLuaValue* pLuaValue )
{
    if ( wszValue == NULL || bufferSize <= 0 )
        return false;

	if ( NULL != pLuaValue )
	{
		switch( pLuaValue->m_eType )
		{
		case LUA_VALUE_NUMBER:
            {
                std::wstring wstrValue;
			    wstrValue = boost::lexical_cast<std::wstring>( pLuaValue->m_dNumber );
                if ( bufferSize >= (int) wstrValue.size() + 1 )
                {
                    wcsncpy( wszValue, wstrValue.c_str(), wstrValue.size() );
                    wszValue[ wstrValue.size() ] = 0;
                }
            }
			return true;
		case LUA_VALUE_STRING:
            if ( pLuaValue->m_strStringUTF8.empty() == true )
            {
                wszValue[0] = 0;
                return true;
            }
            int iNewSize = ::MultiByteToWideChar( CP_UTF8, 0, pLuaValue->m_strStringUTF8.c_str(), pLuaValue->m_strStringUTF8.size(), wszValue, bufferSize-1 );
            if ( iNewSize <= 0 )
            {
                wszValue[0] = 0;
                return true;
            }
            wszValue[ iNewSize ] = NULL;
			return true;
		}//switch
		return false;
	}

	return false;
}


bool    KLuaManagerProxy::DoMemoryNotEncrypt( const char* pRealFile, long nSize )
{
    if ( pRealFile == NULL )
        return false;
    if ( nSize >= 3 && pRealFile[0] == (char) 0xEF && pRealFile[1] == (char) 0xBB && pRealFile[2] == (char) 0xBF )
    {
        nSize -= 3;
        pRealFile += 3;
    }
    if ( nSize <= 0 || pRealFile[0] == NULL )
        return false;

    ASSERT( m_dequeLuaTable.size() == 1 );

    const char* pLineStart = pRealFile;
    const char* pLineEnd = NULL;
    const char* pCursor = pRealFile;
    const char* pEnd = pRealFile + nSize;
    std::string strLine;
    bool    bFinish = false;
    bool    bComment = false;

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(" \t\n\v\f\r", "=,{}[]");
    enum    EState
    {
        EState_NONE = 0,
        EState_DONE,
        EState_KEYWORD,
        EState_KEYWORD_REF_LEFT,
        EState_KEYWORD_REF_KEY,
        EState_KEYWORD_ASSIGN,
        EState_KEYWORD_ASSIGN_KEYWORD,
        EState_KEYWORD_ASSIGN_KEYWORD_REF_LEFT,
        EState_KEYWORD_ASSIGN_KEYWORD_REF_KEY,
    };
    std::string strKeyword;
    std::string strKeyword2;
    std::deque<EState>  deqState;
    deqState.push_back( EState_NONE );
    do
    {
        pLineEnd = NULL;
        if ( bComment == false )
        {
            if ( pCursor == pEnd || *pCursor == 0 )
            {
                pLineEnd = pCursor; 
                bFinish = true;
                ++pCursor;
            }
            else if ( *pCursor == '\n' || *pCursor == '\r' )
            {
                pLineEnd = pCursor;
                ++pCursor;
            }
            else if ( *pCursor == '-' && ( pCursor + 1 ) < pEnd && *( pCursor + 1 ) == '-' )
            {
                pLineEnd = pCursor;
                bComment = true;
                pCursor += 2;
            }
            else
            {
                ++pCursor;
            }
        }
        else
        {
            if ( pCursor == pEnd || *pCursor == 0 )
            {
                break;
            }
            else if ( *pCursor == '\n' || *pCursor == '\r' )
            {
                bComment = false;
                ++pCursor;
                pLineStart = pCursor;
            }
            else
            {
                ++pCursor;
            }
        }
        if ( pLineEnd != NULL )
        {
            if ( pLineStart < pLineEnd )
            {
                strLine.assign( pLineStart, pLineEnd );
                tokenizer tokens( strLine, sep );
                for( tokenizer::iterator tok_iter = tokens.begin();
                    tok_iter != tokens.end(); ++tok_iter )
                {
                    const std::string& strToken = *tok_iter;

                    if ( strToken.empty() == false )
                    {
                        KLuaManagerProxyLuaTablePtr spTop = m_dequeLuaTable.back();
                        if ( strToken.size() == 1 )
                        {
                            bool    bCloseTable = false;
                            switch( strToken.front() )
                            {
                            case '=':
                                if ( deqState.back() == EState_KEYWORD )
                                    deqState.back() = EState_KEYWORD_ASSIGN;
                                else
                                {
                                    ASSERT( 0 );
                                    return false;
                                }
                                continue;
                            case '}':
                                if ( deqState.size() <= 1 || m_dequeLuaTable.size() <= 1 )
                                {
                                    ASSERT( 0 );
                                    return false;
                                }
                                bCloseTable = true;
                            case ',':
                                switch( deqState.back() )
                                {
                                case EState_NONE:
                                    break;
                                case EState_DONE:
                                    deqState.back() = EState_NONE;
                                    break;
                                case EState_KEYWORD:
                                    {
                                        KLuaManagerProxyLuaValue luaValue;
                                        if ( LuaKeyword( strKeyword, luaValue ) == true )
                                        {
                                            spTop->m_vec.push_back( luaValue );
                                            deqState.back() = EState_NONE;
                                        }
                                        else
                                        {
                                            ASSERT( 0 );
                                            return false;
                                        }
                                    }
                                    break;
                                case EState_KEYWORD_ASSIGN_KEYWORD:
                                    {
                                        KLuaManagerProxyLuaValue luaValue;
                                        if ( LuaKeyword( strKeyword2, luaValue ) == true )
                                        {
                                            if ( luaValue.m_eType != LUA_VALUE_NIL )
                                                spTop->m_map[ strKeyword ] = luaValue;
                                            else
                                                spTop->m_map.erase( strKeyword );
                                            deqState.back() = EState_NONE;
                                        }
                                        else
                                        {
                                            ASSERT( 0 );
                                            return false;
                                        }
                                    }
                                    break;
                                default:
                                    ASSERT( 0 );
                                    return false;
                                }//switch
                                if ( bCloseTable == true )
                                {
                                    deqState.pop_back();
                                    m_dequeLuaTable.pop_back();
                                }//if
                                continue;
                            case '{':
                                switch( deqState.back() )
                                {
                                case EState_NONE:
                                    {
                                        deqState.back() = EState_DONE;
                                        KLuaManagerProxyLuaValue kTable( new KLuaManagerProxyLuaTable );
                                        spTop->m_vec.push_back( kTable );
                                        deqState.push_back( EState_NONE );
                                        m_dequeLuaTable.push_back( kTable.m_spTable );
                                    }
                                    break;
                                case EState_KEYWORD_ASSIGN:
                                    {
                                        deqState.back() = EState_DONE;
                                        KLuaManagerProxyLuaValue kTable( new KLuaManagerProxyLuaTable );
                                        spTop->m_map[ strKeyword ] = kTable;
                                        deqState.push_back( EState_NONE );
                                        m_dequeLuaTable.push_back( kTable.m_spTable );
                                    }
                                    break;
                                default:
                                    ASSERT( 0 );
                                    return false;
                                }
                                continue;
                            case '[':
                                switch( deqState.back() )
                                {
                                case EState_KEYWORD:
                                    deqState.back() = EState_KEYWORD_REF_LEFT;
                                    break;
                                case EState_KEYWORD_ASSIGN_KEYWORD:
                                    deqState.back() = EState_KEYWORD_ASSIGN_KEYWORD_REF_LEFT;
                                    break;
                                default:
                                    ASSERT( 0 );
                                    return false;
                                }
                                continue;
                            case ']':
                                switch( deqState.back() )
                                {
                                case EState_KEYWORD_REF_KEY:
                                    deqState.back() = EState_DONE;
                                    break;
                                case EState_KEYWORD_ASSIGN_KEYWORD_REF_KEY:
                                    deqState.back() = EState_DONE;
                                    break;
                                default:
                                    ASSERT( 0 );
                                    return false;
                                }
                                continue;
                            }//switch
                        }
                        KLuaManagerProxyLuaValue luaValue;
                        switch( LuaParse( strToken, false, luaValue ) )
                        {
                        case LUA_PARSE_ERROR:
                        default:
                            ASSERT( 0 );
                            return false;
                        case LUA_PARSE_KEYWORD:
                            switch( deqState.back() )
                            {
                            case EState_NONE:
                                strKeyword = strToken;
                                deqState.back() = EState_KEYWORD;
                                break;
                            case EState_DONE:
                                if ( deqState.size() == 1 )
                                {
                                    strKeyword = strToken;
                                    deqState.back() = EState_KEYWORD;
                                }
                                else
                                {
                                    ASSERT( 0 );
                                    return false;
                                }
                                break;
                            case EState_KEYWORD_ASSIGN:
                                strKeyword2 = strToken;
                                deqState.back() = EState_KEYWORD_ASSIGN_KEYWORD;
                                break;
                            default:
                                ASSERT( 0 );
                                return false;
                            }
                            break;
                        case LUA_PARSE_VALUE:
                            switch( luaValue.m_eType )
                            {
                            case LUA_VALUE_NIL:
                                switch( deqState.back() )
                                {
                                case EState_NONE:
                                    spTop->m_vec.push_back( KLuaManagerProxyLuaValue() );
                                    deqState.back() = EState_DONE;
                                    break;
                                case EState_KEYWORD_ASSIGN:
                                    spTop->m_map.erase( strKeyword );
                                    deqState.back() = EState_DONE;
                                    break;
                                default:
                                    ASSERT( 0 );
                                    return false;
                                }//switch
                                break;
                            case LUA_VALUE_BOOLEAN:
                                switch( deqState.back() )
                                {
                                case EState_NONE:
                                    spTop->m_vec.push_back( luaValue );
                                    deqState.back() = EState_DONE;
                                    break;
                                case EState_KEYWORD_ASSIGN:
                                    spTop->m_map[ strKeyword ] = luaValue;
                                    deqState.back() = EState_DONE;
                                    break;
                                default:
                                    ASSERT( 0 );
                                    return false;
                                }//switch
                                break;
                            case LUA_VALUE_NUMBER:
                                {
                                    int iIndex = (int) luaValue.m_dNumber;
                                    bool    bIndex = ( iIndex >= 1 && (double) iIndex == luaValue.m_dNumber );
                                    switch( deqState.back() )
                                    {
                                    case EState_NONE:
                                        {
                                            spTop->m_vec.push_back( luaValue );
                                            deqState.back() = EState_DONE;
                                        }
                                        break;
                                    case EState_KEYWORD_REF_LEFT:
                                        {
                                            if ( bIndex == false )
                                            {
                                                ASSERT( 0 );
                                                return false;
                                            }
                                            if ( LuaTableIndex( strKeyword, iIndex, luaValue ) == true )
                                            {
                                                spTop->m_vec.push_back( luaValue );
                                                deqState.back() = EState_KEYWORD_REF_KEY;
                                            }
                                            else
                                            {
                                                ASSERT( 0 );
                                                return false;
                                            }
                                        }
                                        break;
                                    case EState_KEYWORD_ASSIGN:
                                        {
                                            spTop->m_map[ strKeyword ] = luaValue;
                                            deqState.back() = EState_DONE;
                                        }
                                        break;
                                    case EState_KEYWORD_ASSIGN_KEYWORD_REF_LEFT:
                                        {
                                            if ( bIndex == false )
                                            {
                                                ASSERT( 0 );
                                                return false;
                                            }
                                            if ( LuaTableIndex( strKeyword2, iIndex, luaValue ) == false )
                                            {
                                                ASSERT( 0 );
                                                return false;
                                            }
                                            if ( luaValue.m_eType == LUA_VALUE_NIL )
                                                spTop->m_map.erase( strKeyword );
                                            else
                                                spTop->m_map[ strKeyword ] = luaValue;
                                            deqState.back() = EState_KEYWORD_ASSIGN_KEYWORD_REF_KEY;
                                        }
                                        break;
                                    default:
                                        ASSERT( 0 );
                                        return false;
                                    }//switch
                                }
                                break;
                            case LUA_VALUE_STRING:
                                switch( deqState.back() )
                                {
                                case EState_NONE:
                                    {
                                        spTop->m_vec.push_back( luaValue );
                                        deqState.back() = EState_DONE;
                                    }
                                    break;
                                case EState_KEYWORD_REF_LEFT:
                                    {
                                        KLuaManagerProxyLuaValue    luaRef;
                                        if ( LuaTableKey( strKeyword, luaValue.m_strStringUTF8, luaRef ) == false )
                                        {
                                            ASSERT( 0 );
                                            return false;
                                        }
                                        spTop->m_vec.push_back( luaRef );
                                        deqState.back() = EState_KEYWORD_REF_KEY;
                                    }
                                    break;
                                case EState_KEYWORD_ASSIGN:
                                    {
                                        spTop->m_map[ strKeyword ] = luaValue;
                                        deqState.back() = EState_DONE;
                                    }
                                    break;
                                case EState_KEYWORD_ASSIGN_KEYWORD_REF_LEFT:
                                    {
                                        KLuaManagerProxyLuaValue    luaRef;
                                        if ( LuaTableKey( strKeyword2, luaValue.m_strStringUTF8, luaRef ) == false )
                                        {
                                            ASSERT( 0 );
                                            return false;
                                        }
                                        if ( luaRef.m_eType == LUA_VALUE_NIL )
                                            spTop->m_map.erase( strKeyword );
                                        else
                                            spTop->m_map[ strKeyword ] = luaRef;
                                        deqState.back() = EState_KEYWORD_ASSIGN_KEYWORD_REF_KEY;
                                    }
                                    break;
                                default:
                                    ASSERT( 0 );
                                    return false;
                                }//switch
                                break;
                            default:
                                ASSERT( 0 );
                                return false;
                            }//switch 
                        }//switch
                    }
                }//for
            }
            pLineStart = pCursor;
        }
    }
    while( bFinish == false );

    ASSERT( m_dequeLuaTable.size() == 1 );

    KLuaManagerProxyLuaTablePtr spTop = m_dequeLuaTable.back();
    switch( deqState.back() )
    {
    case EState_NONE:
        break;
    case EState_DONE:
        deqState.back() = EState_NONE;
        break;
    case EState_KEYWORD:
        {
            KLuaManagerProxyLuaValue luaValue;
            if ( LuaKeyword( strKeyword, luaValue ) == true )
            {
                spTop->m_vec.push_back( luaValue );
                deqState.back() = EState_NONE;
            }
            else
            {
                ASSERT( 0 );
                return false;
            }
        }
        break;
    case EState_KEYWORD_ASSIGN_KEYWORD:
        {
            KLuaManagerProxyLuaValue luaValue;
            if ( LuaKeyword( strKeyword2, luaValue ) == true )
            {
                if ( luaValue.m_eType != LUA_VALUE_NIL )
                    spTop->m_map[ strKeyword ] = luaValue;
                else
                    spTop->m_map.erase( strKeyword );
                deqState.back() = EState_NONE;
            }
            else
            {
                ASSERT( 0 );
                return false;
            }
        }
        break;
    default:
        ASSERT( 0 );
        return false;
    }//switch
    if ( deqState.size() != 1 )
    {
        ASSERT( 0 );
        return false;
    }
    ASSERT( m_dequeLuaTable.size() == 1 );
    m_dequeLuaTable.resize( 1 );
    return true;
}


bool    KLuaManagerProxy::LuaCall( const char* pszFunction, const std::vector<KLuaManagerProxyLuaValue>& vecArgs )
{
    if ( m_pLuaState == NULL || pszFunction == NULL || pszFunction[0] == NULL )
        return false;
    lua_pushstring( m_pLuaState, pszFunction );
    lua_getglobal( m_pLuaState, pszFunction );
    if ( lua_isfunction( m_pLuaState,-1) == false )
    {
        lua_pop( m_pLuaState, 1 );
        return false;
    }
    BOOST_FOREACH( const KLuaManagerProxyLuaValue& luaValue, vecArgs )
    {
        luaValue.LuaPush( m_pLuaState );
    }
    lua_call( m_pLuaState, vecArgs.size(), 0 );
    return true;
}


KLuaManagerProxy::ELuaParse   
    KLuaManagerProxy::LuaParse( std::string strToken, bool bExpandKeyword, KLuaManagerProxyLuaValue& luaOut )
{
    luaOut.Reset();
    boost::trim( strToken );
    if ( strToken.empty() == true )
        return LUA_PARSE_ERROR;
    std::locale loc;
    if ( strToken.front() == '\"' )
    {
        if ( strToken.size() < 2 || strToken.back() != '\"' )
            return LUA_PARSE_ERROR;
        luaOut.Reset( strToken.c_str() + 1, strToken.size() - 2 );
        return LUA_PARSE_VALUE;
    }
    else if ( std::isdigit( strToken.front(), loc ) == true )
    {
        double  dNumber = 0.0;
        try
        {
			dNumber = boost::lexical_cast<double>( strToken );
        }
        catch( const boost::bad_lexical_cast & )
        {
            return LUA_PARSE_ERROR;
        }
        luaOut.Reset( dNumber );
        return LUA_PARSE_VALUE;
    }
    else if ( strToken.front() == '+' || strToken.front() == '-' )
    {
        if ( strToken.size() >= 2 && std::isdigit( strToken[1], loc ) == true )
        {
            double  dNumber = 0.0;
            try
            {
			    dNumber = boost::lexical_cast<double>( strToken );
            }
            catch( const boost::bad_lexical_cast & )
            {
                return LUA_PARSE_ERROR;
            }
            luaOut.Reset( dNumber );
            return LUA_PARSE_VALUE;
        }
        else
        {
            return LUA_PARSE_ERROR;
        }
    }
    else if ( strToken == "nil" )
    {
        return LUA_PARSE_VALUE;
    }
    else if ( strToken == "true" )
    {
        luaOut.Reset( true );
        return LUA_PARSE_VALUE;
    }
    else if ( strToken == "false" )
    {
        luaOut.Reset( false );
        return LUA_PARSE_VALUE;
    }
    else
    {
        char    c;
        std::string::const_iterator iter = strToken.begin();
        c = *iter;
        if ( false == ( std::isalpha( c, loc ) == true || c == '_' ) )
            return LUA_PARSE_ERROR;
        for( ++iter; iter != strToken.end(); ++iter )
        {
            char c = *iter;
            if ( std::isalnum( c, loc ) == true || c == '_' )
            {
            }
            else if ( c == '[' )
            {
                if ( strToken.back() == ']' )
                {
                    KLuaManagerProxyLuaValue    luaKey;
                    std::string strKey( iter + 1, strToken.end() - 1 );
                    boost::trim( strKey );
                    if ( LuaParse( strKey, true, luaKey ) == LUA_PARSE_VALUE )
                    {
                        if ( luaKey.m_eType == LUA_VALUE_NUMBER )
                        {
                            int iIndex = (int) luaKey.m_dNumber;
                            if ( luaKey.m_dNumber == (double) iIndex )
                            {
                                if ( LuaTableIndex( std::string( strToken.begin(), iter ), iIndex, luaOut ) == true )
                                    return LUA_PARSE_VALUE;
                            }
                        }
                        else if ( luaKey.m_eType == LUA_VALUE_STRING )
                        {
                            if ( LuaTableKey( std::string( strToken.begin(), iter ), luaKey.m_strStringUTF8, luaOut ) == true )
                                return LUA_PARSE_VALUE;
                        }
                    }
                    return LUA_PARSE_ERROR;
                }
                else
                {
                    return LUA_PARSE_ERROR;
                }
            }
            if ( false == ( std::isalnum( c, loc ) == true || c == '_' ) )
                return LUA_PARSE_ERROR;
        }
        if ( bExpandKeyword == true )
        {
            if ( LuaKeyword( strToken, luaOut ) == true )
                return LUA_PARSE_VALUE;
            else
                return LUA_PARSE_ERROR;;
        }
        return LUA_PARSE_KEYWORD;
    }
    return LUA_PARSE_ERROR;
}


bool    KLuaManagerProxy::LuaKeyword( const std::string& strIn, KLuaManagerProxyLuaValue& luaOut )
{
    luaOut.Reset();
    LuaName2ValueMap::iterator iter = m_dequeLuaTable.front()->m_map.find( strIn );
    if ( iter != m_dequeLuaTable.front()->m_map.end() )
    {
        luaOut = iter->second;
        return true;
    }
    else if ( lua_State* L = m_pLuaState )
    {
        lua_getglobal( L, strIn.c_str() );
        bool bRet = luaOut.LuaResetFromTop( L );
        lua_pop( L, 1 );
        return bRet;
    }
    return false;
}


bool    KLuaManagerProxy::LuaTableKey( const std::string& strIn, const std::string& strKey, KLuaManagerProxyLuaValue& luaOut )
{
    luaOut.Reset();
    LuaName2ValueMap::iterator iter = m_dequeLuaTable.front()->m_map.find( strIn );
    if ( iter != m_dequeLuaTable.front()->m_map.end() )
    {
        if ( iter->second.m_eType != LUA_VALUE_TABLE )
        {
            ASSERT( 0 );
            return false;
        }
        LuaName2ValueMap::iterator iter2 = iter->second.m_spTable->m_map.find( strKey );
        if ( iter2 != iter->second.m_spTable->m_map.end() )
        {
            luaOut = iter2->second;
            return true;
        }
        else
        {
            ASSERT( 0 );
            return true;
        }
    }
    else if ( lua_State* L = m_pLuaState )
    {
        lua_getglobal( L, strIn.c_str() );
        if ( lua_istable(L,-1) )
        {
            lua_getfield( L, -1, strKey.c_str() );
            bool bRet = luaOut.LuaResetFromTop( L );
            lua_pop( L, 2 );
            return bRet;
        }
        else
        {
            lua_pop( L, 1 );
            return false;
        }
    }
    return false;
}


bool    KLuaManagerProxy::LuaTableIndex( const std::string& strIn, int iIndex, KLuaManagerProxyLuaValue& luaOut )
{
    luaOut.Reset();
    LuaName2ValueMap::iterator iter = m_dequeLuaTable.front()->m_map.find( strIn );
    if ( iter != m_dequeLuaTable.front()->m_map.end() )
    {
        if ( iter->second.m_eType != LUA_VALUE_TABLE )
        {
            ASSERT( 0 );
            return false;
        }
        if ( iIndex > 0 && iIndex <= (LONG) iter->second.m_spTable->m_vec.size() )
        {
            luaOut = iter->second.m_spTable->m_vec[ iIndex - 1 ];
            return true;
        }
        else
        {
            ASSERT( 0 );
            return true;
        }
    }
    else if ( lua_State* L = m_pLuaState )
    {
        lua_getglobal( L, strIn.c_str() );
        if ( lua_istable(L,-1) )
        {
            lua_pushnumber( L, iIndex );
            lua_gettable( L, -2 );
            bool bRet = luaOut.LuaResetFromTop( L );
            lua_pop( L, 2 );
            return bRet;
        }
        else
        {
            lua_pop( L, 1 );
            return false;
        }
    }
    return false;
}
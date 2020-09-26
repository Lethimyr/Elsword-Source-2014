#pragma once

#include <hash_map>
#include <boost/intrusive_ptr.hpp>
#include <boost/lexical_cast.hpp>


enum KLuaManagerProxyLuaValueType
{
	LUA_VALUE_NIL,
	LUA_VALUE_BOOLEAN,
	LUA_VALUE_NUMBER,
	LUA_VALUE_STRING,
	LUA_VALUE_TABLE,
};

struct KLuaManagerProxyLuaTable;

namespace boost
{
    void intrusive_ptr_add_ref(KLuaManagerProxyLuaTable * p);
    void intrusive_ptr_release(KLuaManagerProxyLuaTable * p);
}

typedef boost::intrusive_ptr<KLuaManagerProxyLuaTable> KLuaManagerProxyLuaTablePtr;
struct KLuaManagerProxyLuaValue
{
	KLuaManagerProxyLuaValueType m_eType;
	bool m_bBoolean;
	double m_dNumber;
    std::string m_strStringUTF8;
	KLuaManagerProxyLuaTablePtr m_spTable;

	KLuaManagerProxyLuaValue()
		: m_eType( LUA_VALUE_NIL )
		, m_bBoolean( false )
		, m_dNumber( 0.0 )
        , m_strStringUTF8()
        , m_spTable()
	{
	}

    explicit KLuaManagerProxyLuaValue( bool bBoolean )
        : m_eType( LUA_VALUE_BOOLEAN )
        , m_bBoolean( bBoolean )
        , m_dNumber( 0.0 )
        , m_strStringUTF8()
        , m_spTable()
    {
    }

    explicit KLuaManagerProxyLuaValue( double dNumber )
        : m_eType( LUA_VALUE_NUMBER )
        , m_bBoolean( false )
        , m_dNumber( dNumber )
        , m_strStringUTF8()
        , m_spTable()
    {
    }

    explicit KLuaManagerProxyLuaValue( const std::string& strStringUTF8 )
        : m_eType( LUA_VALUE_STRING )
        , m_bBoolean( false )
        , m_dNumber( 0.0 )
        , m_strStringUTF8( strStringUTF8 )
        , m_spTable()
    {
    }

    explicit KLuaManagerProxyLuaValue( const char* pStringUTF8 )
        : m_eType( LUA_VALUE_STRING )
        , m_bBoolean( false )
        , m_dNumber( 0.0 )
        , m_strStringUTF8( ( pStringUTF8 != NULL ) ? pStringUTF8 : "" )
        , m_spTable()
    {
    }

    explicit KLuaManagerProxyLuaValue( const char* pStringUTF8, unsigned uSize )
        : m_eType( LUA_VALUE_STRING )
        , m_bBoolean( false )
        , m_dNumber( 0.0 )
        , m_strStringUTF8()
        , m_spTable()
    {
        m_strStringUTF8.assign( pStringUTF8, pStringUTF8 + uSize );
    }

    explicit KLuaManagerProxyLuaValue( KLuaManagerProxyLuaTable* pTable )
        : m_eType( LUA_VALUE_TABLE )
        , m_bBoolean( false )
        , m_dNumber( 0.0 )
        , m_strStringUTF8()
        , m_spTable( pTable )
    {
    }

    void    Reset()
    {
        m_eType = LUA_VALUE_NIL;
        m_bBoolean = false;
        m_dNumber = 0.0;
        m_strStringUTF8.resize( 0 );
        m_spTable.reset();
    }

    void    Reset( bool bBoolean )
    {
        Reset();
        m_eType = LUA_VALUE_BOOLEAN;
        m_bBoolean = bBoolean;
    }

    void    Reset( double dNumber )
    {
        Reset();
        m_eType = LUA_VALUE_NUMBER;
        m_dNumber = dNumber;
    }

    void    Reset( const std::string& strStringUTF8 )
    {
        Reset();
        m_eType = LUA_VALUE_STRING;
        m_strStringUTF8 = strStringUTF8;
    }

    void    Reset( const char* pStringUTF8 )
    {
        Reset();
        m_eType = LUA_VALUE_STRING;
        if ( pStringUTF8 == NULL )
            m_strStringUTF8.resize( 0 );
        else
            m_strStringUTF8 = pStringUTF8;
    }

    void    Reset( const char* pStringUTF8, unsigned uSize )
    {
        Reset();
        m_eType = LUA_VALUE_STRING;
        m_strStringUTF8.assign( pStringUTF8, uSize );
    }

    void    Reset( KLuaManagerProxyLuaTable* pTable )
    {
        Reset();
        m_eType = LUA_VALUE_TABLE;
        m_spTable = pTable;
    }

    bool    LuaPush( lua_State* L ) const;
    bool    LuaResetFromTop( lua_State* L );
};

typedef stdext::hash_map< std::string, KLuaManagerProxyLuaValue > LuaName2ValueMap;


struct KLuaManagerProxyLuaTable
{
	std::vector< KLuaManagerProxyLuaValue > m_vec;
	LuaName2ValueMap m_map;

    KLuaManagerProxyLuaTable()
        : m_vec()
        , m_map()
        , m_nRefCount(0)
    {
    }

    KLuaManagerProxyLuaTable( const KLuaManagerProxyLuaTable& In_ )
        : m_vec( In_.m_vec )
        , m_map( In_.m_map )
        , m_nRefCount(0)
    {
    }//KLuaManagerProxyLuaTable()

    //bool    LuaPush( lua_State* L );

private:
	friend void ::boost::intrusive_ptr_add_ref(KLuaManagerProxyLuaTable * p);
	friend void ::boost::intrusive_ptr_release(KLuaManagerProxyLuaTable * p);

    ULONG    AddRef()
    {
        return  ++m_nRefCount;
    }//

    ULONG   Release()
    {
        LONG lLong = --m_nRefCount;
	    if( lLong == 0)
	    {
            KLuaManagerProxyLuaTable* pThis = this;
            delete pThis;
	    }
	    return lLong;
    }//

private:

    LONG                m_nRefCount;
};

namespace boost
{
    inline void intrusive_ptr_add_ref(KLuaManagerProxyLuaTable * p)
    {
        if ( p != NULL )
            p->AddRef();
    }

    inline void intrusive_ptr_release(KLuaManagerProxyLuaTable * p)
    {
        if ( p != NULL )
            p->Release();
    }
}


class KLuaManagerProxy
{

public:

	KLuaManagerProxy( lua_State* L );
    KLuaManagerProxy( lua_State* L, KLuaManagerProxy& In_ );

	~KLuaManagerProxy(void);

	bool DoMemoryNotEncrypt( const char* pRealFile, long nSize );
    bool LuaCall( const char* pszFunction, const std::vector<KLuaManagerProxyLuaValue>& vecArgs );
    enum ELuaParse
    {
        LUA_PARSE_ERROR,
        LUA_PARSE_VALUE,
        LUA_PARSE_KEYWORD,
    };
    ELuaParse   LuaParse( std::string strToken, bool bExpandKeyword, KLuaManagerProxyLuaValue& luaOut );
    bool LuaKeyword( const std::string& strIn, KLuaManagerProxyLuaValue& luaOut );
    bool LuaTableKey( const std::string& strIn, const std::string& strKey, KLuaManagerProxyLuaValue& luaOut );
    bool LuaTableIndex( const std::string& strIn, int iIndex, KLuaManagerProxyLuaValue& luaOut );

	//////////////////////////////////////////////////////////////////////////
	bool BeginTable(const char* pszName, int index, bool bLogOutput = true );
	bool BeginTable(const char* pszName, bool bLogOutput = true );
	bool BeginTable(int iIndex, bool bLogOutput = true);
    bool BeginTable( KLuaManagerProxyLuaTable* table );
    unsigned    GetNumIndexedTables( const char* pszName );
    unsigned    GetNumIndexedTables();

	bool EndTable(bool bLogOutput = true);


	//////////////////////////////////////////////////////////////////////////
	// GET BOOL-TYPE VALUE *********************************************

	// GET INTEGER-TYPE VALUE ******************************************
	template< typename T> bool GetValue(const char* pszName, T& sValue, bool bLogOutput = true);
	template< typename T> bool GetValue(int iIndex, T& sValue, bool bLogOutput = true);
	bool GetValue(const char* pszName, bool& bValue, bool bLogOutput = true);
	bool GetValue(int iIndex, bool& bValue, bool bLogOutput = true);
	// GET STRING-TYPE VALUE *******************************************
	//bool GetValue(const char* pszName, char* wszValue, int bufferSize, bool bLogOutput = true );
	//bool GetValue(const char* pszName, std::string& strValue, bool bLogOutput = true);
 //   bool GetValue(int iIndex, char* wszValue, int bufferSize, bool bLogOutput = true );
	//bool GetValue(int iIndex, std::string& strValue, bool bLogOutput = true);
	bool GetValue(const char* pszName, wchar_t* wszValue, int bufferSize, bool bLogOutput = true );
	bool GetValue(const char* pszName, std::wstring& strValue, bool bLogOutput = true);
    bool GetValue(int iIndex, wchar_t* wszValue, int bufferSize, bool bLogOutput = true );
	bool GetValue(int iIndex, std::wstring& strValue, bool bLogOutput = true);

    unsigned    GetNumIndexedValues()  { return m_dequeLuaTable.back()->m_vec.size(); }
    unsigned    GetNumIndexedValues( const char* pszName, int iBaseIndex = 0 );

#pragma message( "아래의 3개의 함수는 사용되는 곳이 있는지 확인하고 만들자" )
	KLuaManagerProxyLuaTable*	GetStackFront() { return m_dequeLuaTable.front().get(); }

	KLuaManagerProxyLuaTable* StackGetTable( const char* tableName );
	KLuaManagerProxyLuaTable* StackGetTable( const int tableIndex );

    unsigned int GetTableDepth() { return (unsigned int) m_dequeLuaTable.size(); }


protected:

	bool StackPop();
	void StackPush( KLuaManagerProxyLuaTable* table );

	KLuaManagerProxyLuaValue* StackGetValue( const std::string& valueName );
	KLuaManagerProxyLuaValue* StackGetValue( const char* valueName );
	KLuaManagerProxyLuaValue* StackGetValue( const int valueIndex );

	bool GetBooleanFromLuaValue( bool& val, KLuaManagerProxyLuaValue* pLuaValue );
	template< typename T> bool GetNumberFromLuaValue( T& val, KLuaManagerProxyLuaValue* pLuaValue );
	//bool GetStringFromLuaValue( std::string& strValue, KLuaManagerProxyLuaValue* pLuaValue );
	//bool GetStringFromLuaValue( char* szValue, int bufferSize, KLuaManagerProxyLuaValue* pLuaValue );
	bool GetStringFromLuaValue( std::wstring& wstrValue, KLuaManagerProxyLuaValue* pLuaValue );
	bool GetStringFromLuaValue( wchar_t* szValue, int bufferSize, KLuaManagerProxyLuaValue* pLuaValue );

protected:
	std::deque< KLuaManagerProxyLuaTablePtr > m_dequeLuaTable;
    lua_State*  m_pLuaState;
};


template< typename T>
bool KLuaManagerProxy::GetValue( const char* pszName, T& sValue, bool bLogOutput /*= true*/ )
{
	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( pszName );
	return GetNumberFromLuaValue( sValue, pLuaValue );
}


template< typename T>
bool KLuaManagerProxy::GetValue( int iIndex, T& sValue, bool bLogOutput /*= true*/ )
{
	KLuaManagerProxyLuaValue* pLuaValue = StackGetValue( iIndex );
	return GetNumberFromLuaValue( sValue, pLuaValue );
}



template< typename T>
bool KLuaManagerProxy::GetNumberFromLuaValue( T& val, KLuaManagerProxyLuaValue* pLuaValue )
{
	if ( NULL != pLuaValue )
	{
		switch( pLuaValue->m_eType )
		{
		case LUA_VALUE_BOOLEAN:
			val = static_cast<T>( pLuaValue->m_bBoolean );
			return true;
		case LUA_VALUE_NUMBER:
			val = static_cast<T>( pLuaValue->m_dNumber );
			return true;
		case LUA_VALUE_STRING:
            try
            {
			    val = boost::lexical_cast<T>( pLuaValue->m_strStringUTF8 );
            }
            catch( const boost::bad_lexical_cast & )
            {
                return false;
            }
			return true;
		}//switch
		return false;
	}//if

	return false;
}


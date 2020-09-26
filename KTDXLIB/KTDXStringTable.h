#pragma once

#   undef _ENUM
#   define _ENUM( id ) id,
enum STR_ID_DEF1
{
#   include "StringID_def1.h"
};

enum STR_ID_DEF2
{
#   include "StringID_def2.h"
};

enum STR_ID_DEF3
{
#   include "StringID_def3.h"
};

enum STR_ID_DEF4
{
#   include "StringID_def4.h"
};

enum STR_ID_DEF5
{
#   include "StringID_def5.h"
};

enum STR_ID_DEF6
{
#   include "StringID_def6.h"
};

enum STR_ID_DEF7
{
#   include "StringID_def7.h"
};

enum STR_ID_DEF8
{
#   include "StringID_def8.h"
};

enum STR_ID_DEF9
{
#   include "StringID_def9.h"
};

enum STR_ID_DEF10
{
#   include "StringID_def10.h"
};

enum STR_ID_DEF11
{
#   include "StringID_def11.h"
};

enum STR_ID_DEF12
{
#   include "StringID_def12.h"
};

enum STR_ID_DEF13
{
#   include "StringID_def13.h"
};

class CKTDXStringTable
{
    DeclareSingleton( CKTDXStringTable );
public:
    CKTDXStringTable();
    virtual ~CKTDXStringTable();

    bool InitializeStringTable( std::wstring& wstrGeneralStringFileName, std::wstring& wstrScriptStringFileName );
    const std::wstring& GetGeneralString( int iID );
	const std::wstring& GetScriptString( int iID );
    std::wstring GetReplacedString( int iID, char* szParam, ... );
    int GetSize();

#ifdef NETERROR_KOREAN_HELP
	bool GetEnableDebugInfo() { return m_bEnableDebugInfo; }
	void SetEnableDebugInfo(bool bEnable) { m_bEnableDebugInfo = bEnable; }
#endif NETERROR_KOREAN_HELP

protected:
    bool LoadStringTable( std::wstring& wstrFileName, std::vector< std::wstring >& vecStringTable );

public:

protected:
    std::vector< std::wstring >     m_vecGeneralStringTable;
	std::vector< std::wstring >     m_vecScriptStringTable;
    std::wstring                    m_wstrEmpty;
#ifdef NETERROR_KOREAN_HELP
	bool							m_bEnableDebugInfo;
#endif NETERROR_KOREAN_HELP
};

#define REPLACE_PART_OF_STRING( src_str, old_part, new_part ) \
do \
{ \
    std::wstring wstrOld = old_part; \
    std::wstring wstrNew = new_part; \
    while( true ) \
    { \
        size_t iPos = src_str.find( wstrOld ); \
        if( iPos == std::wstring::npos ) \
        { \
            break; \
        } \
        src_str.replace( iPos, wstrOld.size(), wstrNew ); \
    } \
} while( false )

#ifdef NETERROR_KOREAN_HELP
#define	GET_STRING( str_id )	( (SiCKTDXStringTable()->GetEnableDebugInfo()) ?	\
								boost::str( boost::wformat( L"[S%1%]%2%" ) % str_id % SiCKTDXStringTable()->GetGeneralString( str_id ) ).c_str()	:	\
								SiCKTDXStringTable()->GetGeneralString( str_id ).c_str() )
#else NETERROR_KOREAN_HELP
#define GET_STRING( str_id ) SiCKTDXStringTable()->GetGeneralString( str_id ).c_str()
#endif NETERROR_KOREAN_HELP

#define GET_REPLACED_STRING( varg ) SiCKTDXStringTable()->GetReplacedString##varg##.c_str()

#define GET_SCRIPT_STRING( str_id ) SiCKTDXStringTable()->GetScriptString( str_id ).c_str()

DefSingletonInline( CKTDXStringTable );
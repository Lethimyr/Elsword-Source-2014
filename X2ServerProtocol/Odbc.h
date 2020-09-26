#ifndef ODBC_H
#define ODBC_H "$Id: ODBC.H,v 1.1 2003/05/23 01:18:40 cvs Exp $"

#include <ostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include "Serializer/SerBuffer.h"
#include <ToString.h>
#include <atltime.h>    // CTime
#include "KncException.h"
#include "KncSmartPtr.h"

SmartPointer( KODBC );

#define DO_QUERY_ODBC(odbc, x) \
	if( !(odbc).Query( boost::str(boost::wformat x).c_str() ) ) \
{ \
	START_LOG( cerr, L#x ) \
	<< BUILD_LOG( (odbc).m_szDiagMsg ) \
	<< BUILD_LOG( (wchar_t*)(odbc).m_sqlState ); \
	goto end_proc; \
}

class KODBC
{
public:

#   define SQLOK(result) (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
#   define SQLNODATA(result) (result == SQL_NO_DATA)
#   define THROW_INVALID_QUERY() { throw std::exception("DB Query Exception"); }
#   define FETCH_DATA( varlist ) \
            { \
                try \
                { \
                    m_kODBC >> varlist;\
                } \
                catch( std::exception& ex) \
                { \
					dbg::cout << LOG_HEADER_STR << KncUtil::toWideString( ex.what() ) << END_LOG; \
                } \
                catch( ... ) \
                { \
					dbg::cout << LOG_HEADER_STR << L"Unknown Exception Occured." << END_LOG; \
                } \
            }

#   define FETCH_DATA_PTR( pODBC, varlist ) \
			{ \
				try \
				{ \
					(*pODBC) >> varlist;\
				} \
				catch( std::exception& ex) \
				{ \
					dbg::cout << LOG_HEADER_STR << KncUtil::toWideString( ex.what() ) << END_LOG; \
				} \
				catch( ... ) \
				{ \
					dbg::cout << LOG_HEADER_STR << L"Unknown Exception Occured." << END_LOG; \
				} \
			}

    DeclToStringW;
    DeclDump;

    KODBC();
    virtual ~KODBC();

    bool DriverConnect( const wchar_t* szDsn );
    bool ReConnect();
    void Close();

    bool Query(const wchar_t* szFormat, ...);
    bool QueryWithBlob( KSerBuffer& kbuff, const wchar_t* szFormat, ...);
    
    bool BeginFetch();
    bool Fetch();
    void EndFetch();
    
    KODBC& operator >>(bool& val);
    KODBC& operator >>(char& val);
	KODBC& operator >>(wchar_t& val);
    KODBC& operator >>(short& val);
    KODBC& operator >>(int& val);
    KODBC& operator >>(UCHAR& val);
    KODBC& operator >>(USHORT& val);
    KODBC& operator >>(DWORD& val);
    KODBC& operator >>(char* val);
    KODBC& operator >>(std::string& val);
    KODBC& operator >>(float& val);
    KODBC& operator >>(KSerBuffer& val);
    KODBC& operator >>(time_t& val);            // 040728
    KODBC& operator >>(wchar_t* val);           // 051229
    KODBC& operator >>(std::wstring& val);      // 051229
    KODBC& operator >>(ULONGLONG& val);         // 061009
    KODBC& operator >>(LONGLONG& val);          // 061009

    CRITICAL_SECTION& GetCS() { return m_cs; }

    static void DumpMaxTime( std::wostream& stm );
    static void ResetMaxTime();

//protected:
public:
    enum {
        BLOB_DATA_PARAM_MARKER,     // 040622. blob 데이터를 전송하기 위해 사용할 아이디
    };
    SQLHENV             m_hEnv;
    SQLHDBC             m_hDbc;
    SQLHSTMT            m_hStmt;

    USHORT              m_usColumNum;
    CRITICAL_SECTION    m_cs;

public:
    // diagnostic message
    SQLWCHAR            m_szDiagMsg[SQL_MAX_MESSAGE_LENGTH];
    SQLSMALLINT         m_sqlMsgLen;
    SQLWCHAR            m_sqlState[6];
    SQLINTEGER          m_sqlNativeErr;

    std::wstring         m_strDSN;
    std::wstring         m_strAbsolutePathDSN;

    static struct KMaxProcessingInfo {
        KncCriticalSection  m_cs;
        std::wstring        m_strQuery;
        DWORD               m_dwTerm;       // 이벤트 하나를 처리완료하기까지 걸린 시간.
        CTime               m_cTime;
    } ms_kMaxProcessingInfo;

};

DeclOstmOperatorA( KODBC );
DeclOstmOperatorW( KODBC );

#endif
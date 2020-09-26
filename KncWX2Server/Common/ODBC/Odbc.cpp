#pragma   warning( disable :4146 )
//{{ florist
#ifdef _STDAFX
#include "stdafx.h"
#endif
//}} florist

#include "ODBC.h"
#include "dbg/dbg.hpp"
#include <Thread/Locker.h>
#include <KncUtil.h>

KODBC::KMaxProcessingInfo KODBC::ms_kMaxProcessingInfo = {
    KncCriticalSection(), L"초기상태", 0, CTime::GetCurrentTime()
};

KODBC::KODBC()
{
    m_hEnv          = SQL_NULL_HENV;
    m_hDbc          = SQL_NULL_HDBC;
    m_hStmt         = SQL_NULL_HSTMT;

    m_usColumNum    = 0;
    memset(m_szDiagMsg, 0, SQL_MAX_MESSAGE_LENGTH);

    InitializeCriticalSection(&m_cs );

	m_bDBConnStr	= false;

} // KODBC()


KODBC::~KODBC()
{
    Close();
    DeleteCriticalSection(&m_cs );
} // ~KODBC()

bool KODBC::DriverConnect( const wchar_t* szConnStr, bool bDBConnStr )
{
	m_bDBConnStr = bDBConnStr;
	
	if( m_bDBConnStr )
	{
		KClearString< std::wstring > kConnStr( szConnStr );
		return DriverConnectString( kConnStr );
	}
	else
	{
		return DriverConnectFileDSN( szConnStr );
	}
}

bool KODBC::DriverConnectFileDSN( const wchar_t* szDsn_ )
{	
	std::wstring strDsn_ = szDsn_;

    if( m_strDSN.size() <= 0 )
    {
        wchar_t buff[MAX_PATH] = {0};
        ::GetCurrentDirectoryW( MAX_PATH, buff );

        m_strDSN = L"FILEDSN=";
        m_strDSN += buff;
        m_strDSN += L"\\";
        m_strDSN += strDsn_;

        m_strAbsolutePathDSN = L"";
        m_strAbsolutePathDSN += buff;
        m_strAbsolutePathDSN += L"\\";
        m_strAbsolutePathDSN += strDsn_;
    }

    SQLWCHAR InCon[255];
    SQLWCHAR OutCon[255];
    SQLSMALLINT cbOutCon;
    SQLRETURN Ret;

    // allocate environment handle.
    Ret = ::SQLAllocHandle( SQL_HANDLE_ENV, 
        SQL_NULL_HANDLE, 
        &m_hEnv );

    if( !SQLOK( Ret ) )
    {
        START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
            << BUILD_LOG( m_strDSN )
            << END_LOG;

        return false;
    }

    // set the ODBC version environment attribute.
    Ret = ::SQLSetEnvAttr( m_hEnv, 
        SQL_ATTR_ODBC_VERSION, 
        ( SQLPOINTER )SQL_OV_ODBC3, 
        SQL_IS_INTEGER );

    if( !SQLOK( Ret ) )
    {
        START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
            << BUILD_LOG( m_strDSN )
            << END_LOG;

        return false;
    }

    //allocate connecton handle.
    Ret = ::SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDbc );

    if( !SQLOK( Ret ) )
    {
        START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
            << BUILD_LOG( m_strDSN )
            << END_LOG;

        return false;
    }

    //dsn파일을 이용해서 연결한다
    wsprintfW( ( wchar_t* )InCon, L"%s", m_strDSN.c_str() );
    Ret = SQLDriverConnectW( m_hDbc, NULL, ( SQLWCHAR* )InCon, sizeof( InCon ), OutCon,
        sizeof( OutCon ), &cbOutCon, SQL_DRIVER_NOPROMPT );

    //에러검사
    if( !SQLOK( Ret ) )
    {
        START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
            << BUILD_LOG( m_strDSN )
            << END_LOG;

        return false;
    }

    //명령핸들 할당
    Ret = SQLAllocHandle( SQL_HANDLE_STMT, m_hDbc, &m_hStmt );

    if( !SQLOK( Ret ) )
    {
        START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
            << BUILD_LOG( m_strDSN )
            << END_LOG;

        return false;
    }


    dbg::cout << L"DriverConnect() success. m_strDSN : " << m_strDSN << dbg::endl;

    return true;
}

//{{ 2009. 6. 15  최육사	conn string
bool KODBC::DriverConnectString( const KClearString< std::wstring >& kDBConnStr )
{
	// Security String
	if( m_SecurityDBConnStr.IsEmpty() )
	{
		m_SecurityDBConnStr.Init( kDBConnStr.GetString() ); // <--- 나중에 KClearString을 전달인자로 넣도록 바꾸자!

		// Password제거된 string저장
		m_strDBConnStr = kDBConnStr.GetString();

		int index = m_strDBConnStr.find( L";DATABASE=" );
		if( index != std::wstring::npos )
		{
			// 앞부분을 지우자!
			m_strDBConnStr.erase( 0, index + 10 );

			index = m_strDBConnStr.find( L";" );
			if( index != std::wstring::npos )
			{
				// 뒷부분을 지우자!
				m_strDBConnStr.erase( index, m_strDBConnStr.size() - index );
			}
		}
	}

	SQLWCHAR InCon[255];
	SQLWCHAR OutCon[255];
	SQLSMALLINT cbOutCon;
	SQLRETURN Ret;

	// allocate environment handle.
	Ret = ::SQLAllocHandle( SQL_HANDLE_ENV, 
		SQL_NULL_HANDLE, 
		&m_hEnv );

	if( !SQLOK( Ret ) )
	{
		START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
			<< BUILD_LOG( m_strDBConnStr )
			<< END_LOG;

		return false;
	}

	// set the ODBC version environment attribute.
	Ret = ::SQLSetEnvAttr( m_hEnv, 
		SQL_ATTR_ODBC_VERSION, 
		( SQLPOINTER )SQL_OV_ODBC3, 
		SQL_IS_INTEGER );

	if( !SQLOK( Ret ) )
	{
		START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
			<< BUILD_LOG( m_strDBConnStr )
			<< END_LOG;

		return false;
	}

	//allocate connecton handle.
	Ret = ::SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDbc );

	if( !SQLOK( Ret ) )
	{
		START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
			<< BUILD_LOG( m_strDBConnStr )
			<< END_LOG;

		return false;
	}

	//dsn파일을 이용해서 연결한다
	wsprintfW( ( wchar_t* )InCon, L"%s", m_SecurityDBConnStr.GetClearString().c_str() );
	Ret = SQLDriverConnectW( m_hDbc, NULL, ( SQLWCHAR* )InCon, sizeof( InCon ), OutCon, sizeof( OutCon ), &cbOutCon, SQL_DRIVER_NOPROMPT );

	//에러검사
	if( !SQLOK( Ret ) )
	{
		START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
			<< BUILD_LOG( m_strDBConnStr )
			<< END_LOG;

		return false;
	}

	//명령핸들 할당
	Ret = SQLAllocHandle( SQL_HANDLE_STMT, m_hDbc, &m_hStmt );

	if( !SQLOK( Ret ) )
	{
		START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
			<< BUILD_LOG( m_strDBConnStr )
			<< END_LOG;

		return false;
	}


	dbg::cout << L"DriverConnect() success. m_strDSN : " << m_strDBConnStr << dbg::endl;

	return true;
}
//}}

bool KODBC::ReConnect()
{
    Close();

    // DB연결을 시도를 해본다.  [7/26/2004]
    //if ( !Connect( m_strDSN.c_str(), m_strLogin.c_str(), m_strPasswd.c_str()) )
    // m_strDSN은 ReConnect가 불릴때, NULL일수 없다. 그래서 의미없이 parameter로 넘겨준다. 
	bool bRet = false;
	if( m_bDBConnStr )
	{
		bRet = DriverConnectString( KClearString< std::wstring >() );
	}
	else
	{
		bRet = DriverConnectFileDSN( m_strDSN.c_str() );
	}

    if( !bRet )
    {
        dbg::cerr << L"재접속 시도 실패" << END_LOG;
        return false;
    }
    else
    {
        START_LOG( cout, L"재접속 성공" );
        return true;
    }
}

void KODBC::Close()
{
    if( SQL_NULL_HSTMT != m_hStmt )
    {
        ::SQLFreeHandle( SQL_HANDLE_STMT, m_hStmt );
        m_hStmt = SQL_NULL_HSTMT;
    } // if
    
    if( SQL_NULL_HDBC != m_hDbc )
    {
        SQLDisconnect( m_hDbc );
        ::SQLFreeHandle( SQL_HANDLE_DBC, m_hDbc );
        m_hDbc = SQL_NULL_HDBC;
    } // if
    
    if( SQL_NULL_HENV != m_hEnv )
    {
        ::SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
        m_hEnv = SQL_NULL_HENV;
    } // if

    m_usColumNum    = 0;
} // Close()

bool KODBC::Query( const std::wstring& wstrQuery )
{
	if( wstrQuery.length() > QUERY_BUFFER_SIZE )
	{
		START_LOG( cerr, L"쿼리 스트링 길이가 너무 길다!" )
			<< BUILD_LOG( wstrQuery.length() )
			<< BUILD_LOG( wstrQuery )
			<< END_LOG;

		// return하지는 않는다. 길이가 긴것이 문제될뿐 쿼리는 일단 호출해야하니까..
	}

	// 마지막 쿼리로 남기기
	m_strLastQuery = wstrQuery;

	DWORD dwElapTime = ::GetTickCount();  // 051001. florist. 가장 오래 걸린 쿼리 시간을 기억한다.

	// 해당 query에 대한 row가 없는 경우 ret는 SQL_NO_DATA가 return 된다.
	SQLRETURN ret = ::SQLExecDirectW( m_hStmt, (SQLWCHAR*)m_strLastQuery.c_str(), SQL_NTS );

	//{{ Modified by ct 2003-04-30 오후 03:53:45
	SQLRETURN     ret2;
	ret2 = SQLGetDiagRecW( SQL_HANDLE_STMT, 
		m_hStmt, 
		1, 
		m_sqlState, 
		&m_sqlNativeErr,
		m_szDiagMsg, 
		sizeof(m_szDiagMsg) / sizeof(SQLWCHAR), 
		&m_sqlMsgLen );
	//}} Modified by ct 2003-04-30 오후 03:53:45

	// godsend odbc연결 실패시 다시 연결하고 쿼리문 다시 실행  [7/22/2004]
	if ( ret == SQL_ERROR || ret == SQL_INVALID_HANDLE )
	{
		wchar_t* err1 = L"08S01";  // Communication link failure
		wchar_t* err2 = L"24000";  // Invalid cursor state

		// godsend 커서 오류와 DB연결 안된 오류 두가지 경우 다시 연결  [7/26/2004]
		if( wcscmp( (const wchar_t*)m_sqlState, err1) == 0 || wcscmp( (const wchar_t*)m_sqlState, err2) == 0 ) 
		{
			dbg::cerr << m_strLastQuery << L"(err:" << (const wchar_t*)m_sqlState << L")" << END_LOG;

			// godsend 연결을 끊는다. 
			Close();

			// DB연결을 시도를 해본다.  [7/26/2004]
			//if ( !Connect( m_strDSN.c_str(), m_strLogin.c_str(), m_strPasswd.c_str()) )
			// godsend Connect -> DriverConnect로 변경 [9/6/2005]
			bool bRet = false;
			if( m_bDBConnStr )
			{
				bRet = DriverConnectString( KClearString< std::wstring >() );
			}
			else
			{
				bRet = DriverConnectFileDSN( m_strDSN.c_str() );
			}

			if( !bRet )
			{
				dbg::cerr << L"재접속 시도 실패" << END_LOG;
				return false;
			}
			else
			{
				ret = ::SQLExecDirectW( m_hStmt, (SQLWCHAR*)m_strLastQuery.c_str(), SQL_NTS );
			}
		}
	}

	dwElapTime = ::GetTickCount() - dwElapTime;

	{   // lock이 걸리는 범위를 조절하는 scope
		KLocker lock( ms_kMaxProcessingInfo.m_cs );

		if( dwElapTime > ms_kMaxProcessingInfo.m_dwTerm )
		{
			ms_kMaxProcessingInfo.m_strQuery    = m_strLastQuery;
			ms_kMaxProcessingInfo.m_cTime       = CTime::GetCurrentTime();
			ms_kMaxProcessingInfo.m_dwTerm      = dwElapTime;

			START_LOG( clog2, L"쿼리 최대 소요시간 갱신." )
				<< BUILD_LOG( ms_kMaxProcessingInfo.m_strQuery )
				<< BUILD_LOG( ms_kMaxProcessingInfo.m_dwTerm );
		}

	}   // lock이 걸리는 범위를 조절하는 scope

	if( SQLOK(ret) == false )
	{
		SQLRETURN ret2;
		ret2 = SQLGetDiagRecW( SQL_HANDLE_STMT, 
			m_hStmt, 
			1, 
			m_sqlState, 
			&m_sqlNativeErr,
			m_szDiagMsg, 
			sizeof(m_szDiagMsg) / sizeof(SQLWCHAR), 
			&m_sqlMsgLen );

		START_LOG( cerr, L"Query 실패!!")
			<< BUILD_LOG( ret )
			<< BUILD_LOG( ret2 )
			<< BUILD_LOG( m_sqlState )
			<< BUILD_LOG( m_sqlNativeErr )
			<< BUILD_LOG( m_szDiagMsg )
			<< BUILD_LOG( m_sqlMsgLen )
			<< END_LOG;
	}

	return SQLOK(ret);
}
//}}

//bool KODBC::Query(const wchar_t* szFormat_, ...)
//{
//	wchar_t     szQuery[QUERY_BUFFER_SIZE] = {0,};
//	va_list     vaList;
//	SQLRETURN   ret;
//
//	va_start( vaList, szFormat_ );
//	vswprintf( szQuery, szFormat_, vaList );
//	va_end( vaList );
//
//	m_strLastQuery = szQuery;
//
//	DWORD dwElapTime = ::GetTickCount();  // 051001. florist. 가장 오래 걸린 쿼리 시간을 기억한다.
//
//	// 해당 query에 대한 row가 없는 경우 ret는 SQL_NO_DATA가 return 된다.
//	ret = ::SQLExecDirectW( m_hStmt, (SQLWCHAR*)szQuery, SQL_NTS );
//
//	//{{ Modified by ct 2003-04-30 오후 03:53:45
//	SQLRETURN     ret2;
//	ret2 = SQLGetDiagRecW( SQL_HANDLE_STMT, 
//		m_hStmt, 
//		1, 
//		m_sqlState, 
//		&m_sqlNativeErr,
//		m_szDiagMsg, 
//		sizeof(m_szDiagMsg) / sizeof(SQLWCHAR), 
//		&m_sqlMsgLen );
//	//}} Modified by ct 2003-04-30 오후 03:53:45
//
//	// godsend odbc연결 실패시 다시 연결하고 쿼리문 다시 실행  [7/22/2004]
//	if ( ret == SQL_ERROR || ret == SQL_INVALID_HANDLE )
//	{
//		wchar_t* err1 = L"08S01";  // Communication link failure
//		wchar_t* err2 = L"24000";  // Invalid cursor state
//
//		// godsend 커서 오류와 DB연결 안된 오류 두가지 경우 다시 연결  [7/26/2004]
//		if( wcscmp( (const wchar_t*)m_sqlState, err1) == 0 || wcscmp( (const wchar_t*)m_sqlState, err2) == 0 ) 
//		{
//			dbg::cerr << szQuery << L"(err:" << (const wchar_t*)m_sqlState << L")" << END_LOG;
//
//			// godsend 연결을 끊는다. 
//			Close();
//
//			// DB연결을 시도를 해본다.  [7/26/2004]
//			//if ( !Connect( m_strDSN.c_str(), m_strLogin.c_str(), m_strPasswd.c_str()) )
//			// godsend Connect -> DriverConnect로 변경 [9/6/2005]
//			bool bRet = false;
//			if( m_bDBConnStr )
//			{
//				bRet = DriverConnectString( KClearString() );
//			}
//			else
//			{
//				bRet = DriverConnectFileDSN( m_strDSN.c_str() );
//			}
//
//			if( !bRet )
//			{
//				dbg::cerr << L"재접속 시도 실패" << END_LOG;
//				return false;
//			}
//			else
//			{
//				ret = ::SQLExecDirectW( m_hStmt, (SQLWCHAR*)szQuery, SQL_NTS );
//			}
//		}
//	}
//
//	dwElapTime = ::GetTickCount() - dwElapTime;
//
//	{   // lock이 걸리는 범위를 조절하는 scope
//		KLocker lock( ms_kMaxProcessingInfo.m_cs );
//
//		if( dwElapTime > ms_kMaxProcessingInfo.m_dwTerm )
//		{
//			ms_kMaxProcessingInfo.m_strQuery    = szQuery;
//			ms_kMaxProcessingInfo.m_cTime       = CTime::GetCurrentTime();
//			ms_kMaxProcessingInfo.m_dwTerm      = dwElapTime;
//
//			START_LOG( clog2, L"쿼리 최대 소요시간 갱신." )
//				<< BUILD_LOG( ms_kMaxProcessingInfo.m_strQuery )
//				<< BUILD_LOG( ms_kMaxProcessingInfo.m_dwTerm );
//		}
//
//	}   // lock이 걸리는 범위를 조절하는 scope
//
//	return SQLOK(ret);
//}

//bool KODBC::QueryWithBlob( KSerBuffer& kbuff_, const wchar_t* szFormat_, ... )
//{
//    wchar_t     szQuery[QUERY_BUFFER_SIZE];
//    va_list     vaList;
//    SQLRETURN   ret;
//
//    va_start( vaList, szFormat_ );
//    vswprintf( szQuery, szFormat_, vaList );
//    va_end( vaList );
//
//    m_strLastQuery = szQuery;
//
//    DWORD dwElapTime = ::GetTickCount();  // 051001. florist. 가장 오래 걸린 쿼리 시간을 기억한다.
//
//    // binding parameter
//    SQLINTEGER cbBlob = SQL_LEN_DATA_AT_EXEC( kbuff_.GetLength() );
//    ::SQLBindParameter( m_hStmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY,
//        kbuff_.GetLength(), 0, (SQLPOINTER)BLOB_DATA_PARAM_MARKER, 0, &cbBlob );
//
//    // 해당 query에 대한 row가 없는 경우 ret는 SQL_NO_DATA가 return 된다.
//    ret = ::SQLExecDirectW( m_hStmt, (SQLWCHAR*)szQuery, SQL_NTS );
//
//    if( ret == SQL_NEED_DATA )
//    {
//        SQLPOINTER pToken;
//        ret = ::SQLParamData( m_hStmt, &pToken );
//        while( ret == SQL_NEED_DATA ) {
//            if( (int)pToken == BLOB_DATA_PARAM_MARKER ) 
//            {
//                ::SQLPutData( m_hStmt, (PTR)kbuff_.GetData(), kbuff_.GetLength() );
//            }
//            ret = ::SQLParamData( m_hStmt, &pToken );
//        }
//    }
//
//    SQLRETURN     ret2;
//    ret2 = SQLGetDiagRecW( SQL_HANDLE_STMT, 
//                        m_hStmt, 
//                        1, 
//                        m_sqlState, 
//                        &m_sqlNativeErr,
//                        m_szDiagMsg, 
//                        sizeof(m_szDiagMsg) / sizeof(SQLWCHAR), 
//                        &m_sqlMsgLen );
//
//    dwElapTime = ::GetTickCount() - dwElapTime;
//
//    {   // lock이 걸리는 범위를 조절하는 scope
//
//        KLocker lock( ms_kMaxProcessingInfo.m_cs );
//
//        if( dwElapTime > ms_kMaxProcessingInfo.m_dwTerm )
//        {
//            ms_kMaxProcessingInfo.m_strQuery    = szQuery;
//            ms_kMaxProcessingInfo.m_cTime       = CTime::GetCurrentTime();
//            ms_kMaxProcessingInfo.m_dwTerm      = dwElapTime;
//
//            START_LOG( clog2, L"쿼리 최대 소요시간 갱신." )
//                << BUILD_LOG( ms_kMaxProcessingInfo.m_strQuery )
//                << BUILD_LOG( ms_kMaxProcessingInfo.m_dwTerm );
//        }
//
//    }   // lock이 걸리는 범위를 조절하는 scope
//
//    return SQLOK(ret);
//}


bool KODBC::BeginFetch()
{
    SQLRETURN   ret;
    m_usColumNum = 0;
    ret = ::SQLFetch( m_hStmt );
    
    if( !SQLOK(ret) )
    {
        EndFetch();

        SQLRETURN     ret2;
        ret2 = SQLGetDiagRecW( SQL_HANDLE_STMT, 
            m_hStmt, 
            1, 
            m_sqlState, 
            &m_sqlNativeErr,
            m_szDiagMsg, 
            sizeof(m_szDiagMsg) / sizeof(SQLWCHAR), 
            &m_sqlMsgLen );

        return false;
    }

    return true;
}


bool KODBC::Fetch()
{
    m_usColumNum = 0;
    SQLRETURN ret = ::SQLFetch( m_hStmt );

    if( ret == SQL_NO_DATA )
        EndFetch();

    return SQLOK(ret);
} // Fetch()


void KODBC::EndFetch()
{
    if( SQL_NULL_HSTMT != m_hStmt )
        ::SQLCloseCursor( m_hStmt );
}


KODBC& KODBC::operator >>(bool& val_)
{
    char        val;
    SQLRETURN   ret;
    SQLINTEGER  LenBin;

    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_STINYINT, 
                        &val, 
                        sizeof(bool), 
                        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    val_ = !(val == 0);
    return *this;
} // operator >>()


KODBC& KODBC::operator >>(char& val_)
{
    SQLRETURN   ret;
    SQLINTEGER  LenBin;

    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_STINYINT, 
                       &val_, 
                        sizeof(char), 
                        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
} // operator >>()

KODBC& KODBC::operator >>(wchar_t& val_)
{
	SQLRETURN   ret;
    SQLINTEGER  LenBin;

	m_usColumNum++;
	ret = ::SQLGetData( m_hStmt, 
		m_usColumNum, 
		SQL_C_WCHAR, 
		&val_, 
		sizeof(wchar_t), 
		&LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

	if( !SQLOK(ret) )
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >>(short& val_)
{
    SQLRETURN   ret;
    SQLINTEGER  LenBin;
    
    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_SHORT, 
                        &val_, 
                        sizeof(short), 
                        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    return *this;
} // operator >>()

KODBC& KODBC::operator >>(int& val_)
{
    SQLRETURN   ret;
    SQLINTEGER  LenBin;
    
    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_SLONG, 
                        &val_, 
                        sizeof(int), 
                        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    return *this;
} // operator >>()

KODBC& KODBC::operator >>(UINT& val_)
{
	SQLRETURN   ret;
	SQLINTEGER  LenBin;

	m_usColumNum++;
	ret = ::SQLGetData( m_hStmt, 
		m_usColumNum, 
		SQL_C_SLONG, 
		&val_, 
		sizeof(UINT), 
		&LenBin);

	if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
		return *this;

	if( !SQLOK(ret) )
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >>(UCHAR& val_)
{
    SQLRETURN   ret;
    SQLINTEGER  LenBin;
    
    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_UTINYINT, 
                        &val_, 
                        sizeof(UCHAR), 
                        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    return *this;
} // operator >>()


KODBC& KODBC::operator >>(USHORT& val_)
{
    SQLRETURN   ret;
    SQLINTEGER  LenBin;
    
    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_SSHORT, 
                        &val_, 
                        sizeof(USHORT), 
                        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    return *this;
} // operator >>()


KODBC& KODBC::operator >>(DWORD& val_)
{
    SQLRETURN   ret;
    SQLINTEGER  LenBin;
    
    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_SLONG, 
                        &val_,
                        sizeof(DWORD), 
                        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
} // operator >>()

KODBC& KODBC::operator >>(ULONGLONG& val_)
{
    SQLRETURN   ret;
    SQLINTEGER  LenBin;

    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
        m_usColumNum, 
        SQL_C_SBIGINT, 
        &val_, 
        sizeof(ULONGLONG), 
        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
}

KODBC& KODBC::operator >>(LONGLONG& val_)
{
    SQLRETURN   ret;
    SQLINTEGER  LenBin;

    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
        m_usColumNum, 
        SQL_C_SBIGINT, 
        &val_, 
        sizeof(LONGLONG), 
        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
}


KODBC& KODBC::operator >>(std::string& val_)
{
    char acBuffer[1024] = {0, };
    
    *this >> acBuffer;
    val_ = acBuffer;
    
    return *this;
} // operator >>()

KODBC& KODBC::operator >>(char* val_)
{
    SQLRETURN   ret;
    int         nLen;
    SQLINTEGER  LenBin;

    m_usColumNum++;
    SQLColAttribute( m_hStmt, 
                     m_usColumNum, 
                     SQL_DESC_OCTET_LENGTH, 
                     0, 0, 0, 
                    &nLen );
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_CHAR, 
                        val_, 
                        1024, 
                        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    return *this;
} // operator >>()


KODBC& KODBC::operator >>(std::wstring& val_)
{
    wchar_t acBuffer[1024] = {0, };

    *this >> acBuffer;
    val_ = acBuffer;

    return *this;
} // operator >>()


KODBC& KODBC::operator >>(wchar_t* val_)
{
    SQLRETURN   ret;
    int         nLen;
    SQLINTEGER  LenBin;

    m_usColumNum++;
    SQLColAttribute( m_hStmt, 
        m_usColumNum, 
        SQL_DESC_OCTET_LENGTH, 
        0, 0, 0, 
        &nLen );
    ret = ::SQLGetData( m_hStmt, 
        m_usColumNum, 
        SQL_C_WCHAR, 
        val_, 
        1024, 
        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
} // operator >>()

KODBC& KODBC::operator >>(float& val_)
{
    SQLRETURN   ret;
    SQLINTEGER  LenBin;

    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_FLOAT, 
                        &val_, 
                        sizeof(float), 
                        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
} // operator >>()

KODBC& KODBC::operator >>(KSerBuffer& val_)
{
    SQLRETURN   ret;
    char        cbuff[1024];
    SQLINTEGER  LenBin;

    m_usColumNum++;
    val_.Clear();

    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_BINARY, 
                        cbuff, 
                        sizeof(cbuff),
                        &LenBin);

    while( ret != SQL_NO_DATA ) {

        if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
            return *this;

        val_.Write( cbuff, LenBin );

        ret = ::SQLGetData( m_hStmt, 
                            m_usColumNum, 
                            SQL_C_BINARY, 
                            cbuff, 
                            sizeof(cbuff),
                            &LenBin);
    }


    if( ret != SQL_NO_DATA )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
}

#include <time.h>

KODBC& KODBC::operator >>(time_t& val_)
{
    SQLRETURN   ret;
    TIMESTAMP_STRUCT sqlTime;
    SQLINTEGER  LenBin;

    m_usColumNum++;
 
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_TYPE_TIMESTAMP, 
                        &sqlTime, 
                        sizeof(sqlTime),
                        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    // TIMESTAMP_STRUCT -> struct tm -> time_t
    struct tm time = {0};
    time.tm_year    = sqlTime.year - 1900;
    time.tm_mon     = sqlTime.month - 1;
    time.tm_mday    = sqlTime.day;
    time.tm_hour    = sqlTime.hour;
    time.tm_min     = sqlTime.minute;
    time.tm_sec     = sqlTime.second;

    val_ = mktime( &time );
    if( val_ == (time_t)-1 )
    {
        THROW_INVALID_QUERY();
    }

    return *this;
}


// 041115
ImplToStringW( KODBC )
{
   //SectionName을 얻어온다.
    wchar_t szSectionName[10] = {0};
   GetPrivateProfileSectionNamesW( szSectionName, 10, m_strAbsolutePathDSN.c_str() );

   //Address Data를 얻어온다.
   wchar_t szAddress[100] = {0};
   GetPrivateProfileStringW( szSectionName, L"Address", 0, szAddress, 100, m_strAbsolutePathDSN.c_str());
   
   //출력한다.
   
   return stm_ << std::endl
               << TOSTRINGW( szAddress )
               << TOSTRINGW( m_strDSN );
}

void KODBC::DumpMaxTime( std::wostream& stm_ )
{
    KLocker lock( ms_kMaxProcessingInfo.m_cs );

    stm_ << L" -- Max Processing query -- " << std::endl
         << L"  query   : " << ms_kMaxProcessingInfo.m_strQuery.c_str() << std::endl
         << L"  elapsed : " << ms_kMaxProcessingInfo.m_dwTerm << std::endl
         << L"  execute : " << (const wchar_t*)ms_kMaxProcessingInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S")) << std::endl;
}

void KODBC::ResetMaxTime()
{
    KLocker lock( ms_kMaxProcessingInfo.m_cs );

	std::string strQuery;

    ms_kMaxProcessingInfo.m_dwTerm = 0;
    ms_kMaxProcessingInfo.m_cTime = CTime::GetCurrentTime();
    strQuery = (CStringA)ms_kMaxProcessingInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S"));
    strQuery.append( " 에 데이터 초기화 되었습니다." );
	ms_kMaxProcessingInfo.m_strQuery = KncUtil::toWideString(strQuery.c_str(), strQuery.length());
}

namespace { // 외부에서 참조당하지 못하도록 anonymous namespace 안에 선언.

    static const wchar_t* g_szInvalidMark[] = {
        L"\'",  // 홑따옴표
        L"\"",  // 쌍따옴표
        L";",   // 세미콜론
        L"--",  // 주석기호(mssql)
        L" ",   // 일반공백
        L"　",  // 넓은공백 - 'ㄱ' 넣고 한자 변환 1번.
        L"\t",  // tab 문자
        L",",   // 쉼표
		//SERV_CHANNELING_AERIA
		//L"~",   // 물결
		L"`",   // 그레이브
		//{{ 2011. 07. 27    김민성    투니랜드 채널링
//#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING //SERV_INT_ONLY
		//L"@",   // 골뱅이								// 주석 처리
//#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		//}}
		L"^",	// 서컴플렉트
		L"&",	// 앰퍼센트
		L"*",	// 곱셈부호
		//L"-",	// 뺄셈부호	//SERV_EMAIL_ID //SERV_INT_ONLY
		L"+",	// 덧셈부호
		L"=",	// 이퀄
		L"|",	// 바
		L"{",	// 중괄호(좌)
		L"}",	// 중괄호(우)
		//L"[",	// 대괄호(좌) //SERV_INT_ONLY
		//L"]",	// 대괄호(우) //SERV_INT_ONLY
		L":",	// 콜론
		L"?",	// 물음표
		L"<",	// 세모괄호(좌)
		L">",	// 세모괄호(우)
//#ifdef SERV_EMAIL_ID //SERV_INT_ONLY
		//L".",	// 닷
//#endif SERV_EMAIL_ID
		L"♪"	// 음표
    };
}

bool KODBC::IsInvalidMarkIn( IN const std::wstring& strData )
{
    for( int i = 0; i < NUM_ELEMENTS( g_szInvalidMark ); i++ )
    {
        if( strData.find( g_szInvalidMark[i] ) != std::string::npos )   // 동일한 문자가 발견된다면
            return true;
    }

    return false;   // 아무것도 발견되지 않았다면 false.
}

namespace { // 외부에서 참조당하지 못하도록 anonymous namespace 안에 선언.

	static const wchar_t* g_szInvalidMarkForLetter[] = {
		L"\'",  // 홑따옴표
	};
}

bool KODBC::IsInvalidMarkInForLetter( IN const std::wstring& strData )
{
	for( int i = 0; i < NUM_ELEMENTS( g_szInvalidMarkForLetter ); i++ )
	{
		if( strData.find( g_szInvalidMarkForLetter[i] ) != std::string::npos )   // 동일한 문자가 발견된다면
			return true;
	}

	return false;   // 아무것도 발견되지 않았다면 false.
}

namespace { // 외부에서 참조당하지 못하도록 anonymous namespace 안에 선언.

	static const wchar_t* g_szInvalidMarkForCheat[] = {
		L";",   // 세미콜론
		L"--",  // 주석기호(mssql)
		L"　",  // 넓은공백 - 'ㄱ' 넣고 한자 변환 1번.
		L"\t",  // tab 문자
		L",",   // 쉼표
		L"~",   // 물결
		L"`",   // 그레이브
		L"@",   // 골뱅이
		L"^",	// 서컴플렉트
		L"&",	// 앰퍼센트
		L"*",	// 곱셈부호
		L"-",	// 뺄셈부호
		L"+",	// 덧셈부호
		L"=",	// 이퀄
		L"|",	// 바
		L"{",	// 중괄호(좌)
		L"}",	// 중괄호(우)
		L"[",	// 대괄호(좌)
		L"]",	// 대괄호(우)
		L":",	// 콜론
		L"?",	// 물음표
		L"<",	// 세모괄호(좌)
		L">",	// 세모괄호(우)
		L".",	// 닷
		L"♪"	// 음표
	};
}

bool KODBC::IsInvalidMarkInForCheat( IN const std::wstring& strData )
{
	{
		for( int i = 0; i < NUM_ELEMENTS( g_szInvalidMarkForCheat ); i++ )
		{
			if( strData.find( g_szInvalidMarkForCheat[i] ) != std::string::npos )   // 동일한 문자가 발견된다면
				return true;
		}

		return false;   // 아무것도 발견되지 않았다면 false.
	}
}

//{{ 2011. 08. 09	최육사	머신ID 블럭기능
//#ifdef SERV_MACHINE_ID_BLOCK

namespace { // 외부에서 참조당하지 못하도록 anonymous namespace 안에 선언.

	static const wchar_t* g_szInvalidMarkForMachineID[] = {
		L"\'",  // 홑따옴표
		L"\"",  // 쌍따옴표
		L";",   // 세미콜론
		L"--",  // 주석기호(mssql)
		L" ",   // 일반공백
		L"　",  // 넓은공백 - 'ㄱ' 넣고 한자 변환 1번.
		L"\t",  // tab 문자
		L",",   // 쉼표
		L"~",   // 물결
		L"`",   // 그레이브		
		L"@",   // 골뱅이
		L"^",	// 서컴플렉트
		L"&",	// 앰퍼센트
		L"*",	// 곱셈부호
		L"-",	// 뺄셈부호
		//L"+",	// 덧셈부호
		//L"=",	// 이퀄
		L"|",	// 바
		L"{",	// 중괄호(좌)
		L"}",	// 중괄호(우)
		L"[",	// 대괄호(좌)
		L"]",	// 대괄호(우)
		L":",	// 콜론
		L"?",	// 물음표
		L"<",	// 세모괄호(좌)
		L">",	// 세모괄호(우)
		L".",	// 닷
		L"♪"	// 음표
	};
}

bool KODBC::IsInvalidMarkInForMachineID( IN const std::wstring& strData )
{
	{
		for( int i = 0; i < NUM_ELEMENTS( g_szInvalidMarkForMachineID ); ++i )
		{
			if( strData.find( g_szInvalidMarkForMachineID[i] ) != std::string::npos )   // 동일한 문자가 발견된다면
				return true;
		}

		return false;   // 아무것도 발견되지 않았다면 false.
	}
}
//#endif SERV_MACHINE_ID_BLOCK
//}}

#include <boost/algorithm/string/replace.hpp>
//#include <boost/algorithm/string/erase.hpp>

// 데이터를 변형시켜서 DB에 가게 되므로 가급적 쓰지 말자.
void KODBC::RemoveInvalidMark( IN OUT std::wstring& strData )
{
    for( int i = 0; i < NUM_ELEMENTS( g_szInvalidMark ); i++ )
    {
        boost::replace_all( strData, g_szInvalidMark[i], L"*" );    // 해당 문자열을 *로 변환.
        //boost::erase_all( strData, g_szInvalidMark[i] );  // 해당 문자열은 그냥 삭제.
    }
}



//{{ 2012. 04. 17	최육사	DB쿼리 성공 체크
//#ifdef SERV_ALL_DB_QUERY_SUCCESS_CHECK
KDBQuerySuccessChecker::KDBQuerySuccessChecker()
{
	Clear();
}

KDBQuerySuccessChecker::~KDBQuerySuccessChecker()
{
}

void KDBQuerySuccessChecker::Clear()
{
	m_vecSuccessQuery.clear();
	m_vecFailedQuery.clear();
}

void KDBQuerySuccessChecker::AddSuccessQuery( IN const std::wstring& wstrQuery )
{
	m_vecSuccessQuery.push_back( wstrQuery );
}

void KDBQuerySuccessChecker::AddFailedQuery( IN const std::wstring& wstrQuery )
{
	m_vecFailedQuery.push_back( wstrQuery );
}

bool KDBQuerySuccessChecker::IsAllQuerySuccess() const
{
	return ( m_vecSuccessQuery.empty() == false  &&  m_vecFailedQuery.empty() == true );
}

void KDBQuerySuccessChecker::PrintLogSuccessQuery() const
{
	std::vector< std::wstring >::const_iterator vit;
	for( vit = m_vecSuccessQuery.begin(); vit != m_vecSuccessQuery.end(); ++vit )
	{
		START_LOG( cout2, L"Success Query : " << *vit );
	}
}

void KDBQuerySuccessChecker::PrintLogFailedQuery() const
{
	std::vector< std::wstring >::const_iterator vit;
	for( vit = m_vecFailedQuery.begin(); vit != m_vecFailedQuery.end(); ++vit )
	{
		START_LOG( cout2, L"Failed Query : " << *vit );
	}	
}
//#endif SERV_ALL_DB_QUERY_SUCCESS_CHECK
//}}



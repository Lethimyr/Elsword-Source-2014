#include "stdafx.h"

#include "KQuery.h"

KImplementGlobalVal( KQuery );

///////////////////////////////////////////////////////////////////////////////
/*
KQuery 클래스:ODBC의 래퍼 클래스. API 수준에서 재사용 가능하도록 작성하였다.
SQL문을 편리하게 실행하고 결과를 쉽게 읽는데 초점을 맞추었으며 SQL 서버와 
액세스에서 완벽하게 테스트되었다.
255자 이하의 정수, 문자열 컬럼에 대해서만 사용 가능하며 그 이상의 길이를 가
지는 필드는 "오른쪽 잘림" 진단 경고가 발생하므로 이 클래스로 읽을 수 없으며
ODBC API 함수를 직접 사용해야 한다.
최대 컬럼 개수는 100개이다. 초과시 사정없이 에러 메시지 박스를 출력하므로 이 
한도를 알아서 넘지 않도록 주의해야 한다. 특히 필드수가 많은 테이블을 select *
로 읽는 것은 삼가하는 것이 좋다.
전진 전용 커서를 사용하므로 이미 읽은 레코드는 쿼리를 다시 실행해야만 읽을 수
있다. 

사용법
1.KQuery 클래스의 객체를 생성한다. 가급적이면 전역으로 선언하는 것이 좋으며
  필요한 수만큼 객체를 만든다. 보통 세 개 정도면 충분하다.
2.적절한 Connect 함수를 호출하여 데이터 소스에 연결한다.
3.Exec 함수로 SQL문을 실행한다. 에러 처리는 Exec 함수 내부에서 처리한다.
4.Fetch 함수로 결과 셋 하나를 가져온다. 여러개의 결과셋이 있는 경우는 while
  루프를 돌면서 차례대로 꺼내올 수 있다.
5.Get* 함수로 컬럼 값을 읽는다.
6.Clear 함수로 명령 핸들을 닫는다.(Select문일 경우만)
7.객체를 파괴한다. 전역 객체인 경우는 일부러 파괴할 필요가 없다.
*/

//-----------------------------------------------------------------------------
// 생성자:각종 초기화를 담당한다.
KQuery::KQuery()
{
	m_iAffectCount = -1; 
	m_hRet = SQL_SUCCESS; 
	char str[256] = { 0 };

	// 환경 핸들을 할당하고 버전 속성을 설정한다.
	m_hRet = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	if (m_hRet != SQL_SUCCESS)
	{
		wsprintfA(str, "m_hRet : %d", (LPCTSTR) m_hRet);
		::MessageBoxA(NULL, str, "초기화, SQLAllocHandle() 실패!", 0); 
		return;
	}

	m_hRet = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER); 
	if (m_hRet != SQL_SUCCESS)
	{
		wsprintfA(str, "m_hRet : %d", (LPCTSTR) m_hRet);
		::MessageBoxA(NULL, str, "초기화, SQLSetEnvAttr() 실패!", 0); 
		return;
	}
}
//-----------------------------------------------------------------------------
// 파괴자:연결 핸들을 해제한다.
KQuery::~KQuery()
{
	if (m_hhStmt) SQLFreeHandle(SQL_HANDLE_STMT, m_hhStmt); 
	if (m_hDbc) SQLDisconnect(m_hDbc); 
	if (m_hDbc) SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc); 
	if (m_hEnv) SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv); 
}
//-----------------------------------------------------------------------------
// 연결 핸들을 할당하고 연결한 후 명령핸들까지 같이 할당한다.
// Type = 1:ConStr은 MDB 파일의 경로를 가진다. 경로 생략시 현재 디렉토리에서 MDB를 찾는다.
// Type = 2:ConStr은 SQL 서버의 연결 정보를 가지는 DSN 파일의 경로를 가진다. 
//        경로는 반드시 완전 경로로 지정해야 한다.
// Type = 3:SQLConnect 함수로 DSN에 직접 연결한다.
// 연결 또는 명령 핸들 할당에 실패하면 FALSE를 리턴한다.
BOOL KQuery::Connect(int Type, char* ConStr, char* UID, char* PWD)
{
	SQLCHAR InCon[255] = { 0 };
	SQLCHAR OutCon[255] = { 0 };
    SQLSMALLINT cbOutCon; 

	int ii = 1; 
	//SQLRETURN Ret; 
	//SQLINTEGER NativeError; 
	SQLCHAR SqlState[6] = { 0 };
	SQLCHAR Msg[255] = { 0 };
	//SQLSMALLINT MsgLen; 
	char str[256] = { 0 };

	// 연결 타입에 따라 MDB 또는 SQL 서버, 또는 DSN에 연결한다.
	m_hRet = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc); 
	if (m_hRet != SQL_SUCCESS)
	{
		wsprintfA(str, "m_hRet : %d", (LPCTSTR) m_hRet);
		::MessageBoxA(NULL, str, "Connect, SQLAllocHandle() 실패!", 0); 
	}

	switch (Type)
	{
	case eConnectType_MDB:
		wsprintfA((char*) InCon, "DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s;", ConStr); 
		m_hRet = SQLDriverConnectA(m_hDbc, NULL, (SQLCHAR*) InCon, sizeof(InCon), OutCon, 
			sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT); 
		break; 
	case eConnectType_SQL:
		wsprintfA((char*) InCon, "FILEDSN=%s", ConStr); 
		m_hRet = SQLDriverConnectA(m_hDbc, NULL, (SQLCHAR*) InCon, sizeof(InCon), OutCon, 
			sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT); 
		break; 
	case eConnectType_SQLDirect:
		wsprintfA((char*) InCon, "%s", ConStr);
		m_hRet = SQLDriverConnectA(m_hDbc, NULL, (SQLCHAR*) InCon, sizeof(InCon), OutCon, 
			sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
		break; 
	}

	// 접속 에러시 진단 정보를 보여준다.
	if ((m_hRet != SQL_SUCCESS) && (m_hRet != SQL_SUCCESS_WITH_INFO))
	{
		wsprintfA(str, "m_hRet : %d", (LPCTSTR) m_hRet); 
		::MessageBoxA(NULL, str, "KQuery::Connect() 실패!", 0); 
		return FALSE; 
	}

	// 명령 핸들을 할당한다.
	m_hRet = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hhStmt); 
	if ((m_hRet != SQL_SUCCESS) && (m_hRet != SQL_SUCCESS_WITH_INFO)) 
	{
		m_hhStmt = 0; 
		return FALSE; 
	}

	// 재접속을 위해 접속 정보를 멤버에 저장해 놓는다.
	this->m_iType = Type; 
	lstrcpyA(this->m_pcConStr, ConStr); 
	if (UID)
		lstrcpyA(this->m_pcUID, UID); 
	if (PWD)
		lstrcpyA(this->m_pcPWD, PWD); 
	return TRUE; 
}
//-----------------------------------------------------------------------------
// SQL문을 실행한다. 실패시 진단 정보를 출력하고 FALSE를 리턴한다.
BOOL KQuery::Exec(LPCTSTR szSQL)
{
#ifdef DEBUG
	KDbgStm.SetLogFilename("ServiceInfoLog.txt"); 
	KDbgStm << "Call KQuery::Exec() - " << szSQL << fileout; 
#endif

	int c; 

	// SQL문을 실행한다. SQL_NO_DATA를 리턴한 경우도 일단 성공으로 취급한다. 
	// 이 경우 Fetch에서 EOF를 리턴하도록 되어 있기 때문에 진단 정보를 출력할 필요는 없다.
	m_hRet = SQLExecDirect(m_hhStmt, (SQLWCHAR*) szSQL, SQL_NTS); 
	if ((m_hRet != SQL_SUCCESS) && (m_hRet != SQL_SUCCESS_WITH_INFO) && (m_hRet != SQL_NO_DATA)) 
	{
		PrintDiag(); 
		return FALSE; 
	}

	// Update, Delete, Insert 명령시 영향받은 레코드 개수를 구해 놓는다.
	SQLRowCount(m_hhStmt, &m_iAffectCount); 

	SQLNumResultCols(m_hhStmt, &m_iCol); 
	if (m_iCol > eMAXCOL) 
	{
		::MessageBoxA(NULL, "최대 컬럼 수를 초과했습니다", "KQuery 에러", MB_OK); 
		return FALSE; 
	}

	// nCol이 0인 경우는 Select문 이외의 다른 명령을 실행한 경우이므로 
	// 바인딩을 할 필요가 없다.
	if (m_iCol == 0) 
	{
		Clear(); 
		return TRUE; 
	}

	// 모든 컬럼을 문자열로 바인딩해 놓는다. Col배열은 zero base, 
	// 컬럼 번호는 one base임에 유의할 것
	for (c = 0; c < m_iCol; c++) 
	{
		SQLBindCol(m_hhStmt, c+1, SQL_C_CHAR, m_acCol[c], 255, &m_alCol[c]); 
		SQLDescribeColA(m_hhStmt, c+1, m_pcColName[c], 30, NULL, NULL, NULL, NULL, NULL); 
	}
	return TRUE; 
}
//-----------------------------------------------------------------------------
// SQL문을 실행하고 결과셋의 첫 칼럼에서 정수값을 읽어 리턴해 준다. 
// 결과를 얻은 후 정리까지 해 준다.
int KQuery::ExecGetInt(LPCTSTR szSQL, int& iVal)
{
	if (Exec(szSQL) == FALSE) 
	{
		return eQueryERROR; 
	}

	// 데이터가 없는 경우 EOF를 리턴한다.
	if (Fetch() == SQL_NO_DATA)
	{
		Clear(); 
		return eQueryEOF; 
	}

	GetInt(1, iVal); 
	Clear(); 
	return eQueryOK; 
}
//-----------------------------------------------------------------------------
// SQL문을 실행하고 결과셋의 첫 칼럼에서 문자열을 읽어 리턴해 준다.
int KQuery::ExecGetStr(LPCTSTR szSQL, char* buf)
{
	// SQL문 실행중 에러가 발생한 경우 문자열 버퍼에 에러를 돌려 보낸다.
	if (Exec(szSQL) == FALSE)
	{
		return eQueryERROR; 
	}

	// 데이터가 없는 경우 EOF를 리턴한다.
	if (Fetch() == SQL_NO_DATA)
	{
		Clear(); 
		return eQueryEOF; 
	}

	GetStr(1, buf); 
	Clear(); 
	return eQueryOK; 
}
//-----------------------------------------------------------------------------
// 결과셋에서 한 행을 가져온다.
SQLRETURN KQuery::Fetch()
{
	m_hRet = SQLFetch(m_hhStmt); 
	return m_hRet; 
}
//-----------------------------------------------------------------------------
// 커서를 닫고 바인딩 정보를 해제한다.
void KQuery::Clear()
{
	SQLCloseCursor(m_hhStmt); 
	SQLFreeStmt(m_hhStmt, SQL_UNBIND); 
}
//-----------------------------------------------------------------------------
// 컬럼 이름으로부터 컬럼 인덱스를 찾는다. 없을 경우 -1을 리턴한다.
int KQuery::FindCol(char* name)
{
	int i; 
	for (i = 0; i < m_iCol; i++)
	{
		if (stricmp(name, (const char*) m_pcColName[i]) == 0)
			return i+1; 
	}
	return -1; 
}
//-----------------------------------------------------------------------------
// nCol의 컬럼값을 정수로 읽어준다. NULL일 경우 KQueryNULL을 리턴한다.
int KQuery::GetInt(int nCol, int& iVal)
{
	if (nCol > this->m_iCol)
		return eQueryNOCOL; 

	if (m_alCol[nCol-1] == SQL_NULL_DATA)
	{
		return eQueryNULL; 
	}
	else
	{
		iVal = atoi(m_acCol[nCol-1]); 
		return eQueryOK; 
	}
}
//-----------------------------------------------------------------------------
// sCol의 컬럼값을 정수로 읽어준다.
int KQuery::GetInt(char* sCol, int& iVal)
{
	int n; 
	n = FindCol(sCol); 
	if (n == -1)
	{
		return eQueryNOCOL; 
	}

	return GetInt(n, iVal); 
}
//-----------------------------------------------------------------------------
// nCol의 컬럼값을 실수로 읽어준다. NULL일 경우 KQueryNULL을 리턴한다.
int KQuery::GetFloat(int nCol, float& fVal)
{
	if (nCol > this->m_iCol)
		return eQueryNOCOL; 

	if (m_alCol[nCol-1] == SQL_NULL_DATA)
	{
		return eQueryNULL; 
	}
	else
	{
		fVal = (float) atof(m_acCol[nCol-1]); 
		return eQueryOK; 
	}
}
//-----------------------------------------------------------------------------
// sCol의 컬럼값을 실수로 읽어준다.
int KQuery::GetFloat(char* sCol, float& fVal)
{
	int n; 
	n = FindCol(sCol); 
	if (n == -1)
	{
		return eQueryNOCOL; 
	}

	return GetFloat(n, fVal); 
}
//-----------------------------------------------------------------------------
// nCol의 컬럼값을 문자열로 읽어준다. NULL일 경우 문자열에 NULL을 채워준다. 
// buf의 길이는 최소한 256이어야 하며 길이 점검은 하지 않는다.
int KQuery::GetStr(int nCol, char* buf)
{
	if (nCol > this->m_iCol)
	{
		return eQueryNOCOL; 
	}

	if (m_alCol[nCol-1] == SQL_NULL_DATA)
	{
		return eQueryNULL; 
	}
	else
	{
		lstrcpyA(buf, m_acCol[nCol-1]); 
		return eQueryOK; 
	}
}
//-----------------------------------------------------------------------------
// sCol의 컬럼값을 문자열로 읽어준다.
int KQuery::GetStr(char* sCol, char* buf)
{
	int n; 
	n = FindCol(sCol); 
	if (n == -1)
	{
		return eQueryNOCOL; 
	}

	return GetStr(n, buf); 
}
//-----------------------------------------------------------------------------
// 문자열형 컬럼 읽기
int KQuery::GetStr(char* sCol, std::string& strBuf)
{
	int nCol; 
	nCol = FindCol(sCol); 
	if (nCol == -1)
	{
		return eQueryNOCOL; 
	}
	else
	{
		if (nCol > this->m_iCol)
		{
			return eQueryNOCOL; 
		}

		if (m_alCol[nCol-1] == SQL_NULL_DATA)
		{
			return eQueryNULL; 
		}
	}

	strBuf = m_acCol[nCol-1]; 
	return eQueryOK; 
}
//-----------------------------------------------------------------------------
// 에러 발생시 진단 정보를 출력해 준다.
void KQuery::PrintDiag()
{
	int ii; 
	SQLRETURN Ret; 
	SQLINTEGER NativeError; 
	SQLCHAR SqlState[6], Msg[255]; 
	SQLSMALLINT MsgLen; 
	char str[256]; 

	ii = 1; 
	while (Ret = SQLGetDiagRecA(SQL_HANDLE_STMT, m_hhStmt, ii, SqlState, &NativeError, 
		Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
	{
		wsprintfA(str, "SQLSTATE:%s, 진단정보:%s", (LPCTSTR) SqlState, (LPCTSTR) Msg); 
		::MessageBoxA(NULL, str, "진단 정보", 0); 
		ii++; 
	}

	// 복구 불가능한 에러가 발생한 경우 프로그램을 종료한다. 극단적 예외 처리가 불필요한
	// 경우는 이 코드를 주석 처리하거나 적당하게 다른 루틴으로 바꿔야 한다.
	if (ii > 1) 
	{
		::MessageBoxA(NULL, "진단 정보가 출력되었습니다. 이 정보가 출력될 경우는 네트웍 끊김, DB 서버 중지 등의\r\n"
			"복구 불가능한 에러가 발생한 경우이며 프로그램 실행을 계속할 수 없습니다.\r\n"
			"에러를 수정하신 후 프로그램을 다시 실행해 주십시오.", "Critical Error", MB_OK | MB_ICONERROR); 

		// 다음 둘 중 하나를 선택할 것
		PostQuitMessage(0); 
		// ExitProcess(0); 
	}
}
//-----------------------------------------------------------------------------
// BLOB 데이터를 buf에 채워준다. 이때 buf는 충분한 크기의 메모리를 미리 할당해 
// 놓아야 한다. NULL일 경우 0을 리턴하고 에러 발생시 -1을 리턴한다. 성공시 읽은 
// 총 바이트 수를 리턴한다. szSQL은 하나의 BLOB 필드를 읽는 Select SQL문이어야 한다.
int KQuery::ReadBlob(LPCTSTR szSQL, void* buf)
{
	SQLCHAR BinaryPtr[eBLOBBATCH]; 
	SQLINTEGER LenBin; 
	char *p; 
	int nGet; 
	int TotalGet = 0; 

	m_hRet = SQLExecDirect(m_hhStmt, (SQLWCHAR*) szSQL, SQL_NTS); 
	if (m_hRet != SQL_SUCCESS) 
	{
		PrintDiag(); 
		return -1; 
	}

	while ((m_hRet = SQLFetch(m_hhStmt)) != SQL_NO_DATA) 
	{
		p = (char*) buf; 
		while ((m_hRet = SQLGetData(m_hhStmt, 1, SQL_C_BINARY, BinaryPtr, sizeof(BinaryPtr), 
			&LenBin)) != SQL_NO_DATA) 
		{
			if (LenBin == SQL_NULL_DATA) 
			{
				Clear(); 
				return 0; 
			}
			if (m_hRet == SQL_SUCCESS)
				nGet = LenBin; 
			else
				nGet = eBLOBBATCH; 
			TotalGet += nGet; 
			memcpy(p, BinaryPtr, nGet); 
			p += nGet; 
		}
	}
	Clear(); 
	return TotalGet; 
}
//-----------------------------------------------------------------------------
// buf의 BLOB 데이터를 저장한다. szSQL은 하나의 BLOB 데이터를 저장하는 Update, Insert
// SQL문이어야 한다.
void KQuery::WriteBlob(LPCTSTR szSQL, void* buf, int size)
{
	SQLINTEGER cbBlob; 
	char tmp[eBLOBBATCH], *p; 
	SQLPOINTER pToken; 
	int nPut; 

	cbBlob = SQL_LEN_DATA_AT_EXEC(size); 
	SQLBindParameter(m_hhStmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY, 
		size, 0, (SQLPOINTER)1, 0, &cbBlob); 
	SQLExecDirect(m_hhStmt, (SQLWCHAR*) szSQL, SQL_NTS); 
	m_hRet = SQLParamData(m_hhStmt, &pToken); 
	while (m_hRet == SQL_NEED_DATA)
	{
		if (m_hRet == SQL_NEED_DATA) 
		{
			if ( reinterpret_cast<int>(pToken) == 1)
			{
				for (p = (char*) buf; p < (char*) buf+size; p += eBLOBBATCH) 
				{
					nPut = min(eBLOBBATCH, (char*) buf+size-p); 
					memcpy(tmp, p, nPut); 
					SQLPutData(m_hhStmt, (PTR)tmp, nPut); 
				}
			}
		}
		m_hRet = SQLParamData(m_hhStmt, &pToken); 
	}
	Clear(); 
}
//-----------------------------------------------------------------------------
void KQuery::StrToTime(char* str, SYSTEMTIME* st)
{
	char Year[5]	 = {0, }; 
	char Month[3]	 = {0, }; 
	char Day[3]		 = {0, }; 
	char Hour[3]	 = {0, };
	char Minute[3]	 = {0, };
	char Second[3]	 = {0, };
	
	// 날짜포맷이 아닌 경우 에러 처리
	if(strlen(str) < 10)
		return; 
	
	// 날짜만을 추출하므로 시간정보는 리셋시켜 주어야 한다.
	// 안 그러면 멍청한 DTP 컨트롤이 날짜를 세트해 주지 않는다.
	memset(st, 0, sizeof(SYSTEMTIME)); 
	strncpy(Year, str, 4); 
	strncpy(Month, str+5, 2); 
	strncpy(Day, str+8, 2);
	strncpy(Hour, str+11, 2);
	strncpy(Minute, str+14, 2);
	strncpy(Second, str+17, 2);

	st->wYear = atoi(Year); 
	st->wMonth = atoi(Month); 
	st->wDay = atoi(Day);
	st->wHour = atoi(Hour);
	st->wMinute = atoi(Minute);
	st->wSecond = atoi(Second);
}
//-----------------------------------------------------------------------------
void KQuery::GetDate(int nCol, SYSTEMTIME* st)
{
	if (nCol > this->m_iCol) 
	{
		memset(st, 0, sizeof(SYSTEMTIME)); 
		return; 
	}

	if (m_alCol[nCol-1] == SQL_NULL_DATA) 
	{
		memset(st, 0, sizeof(SYSTEMTIME)); 
	}
	else
	{
		StrToTime(m_acCol[nCol-1], st); 
	}
}
//-----------------------------------------------------------------------------
void KQuery::GetDate(char* sCol, SYSTEMTIME* st)
{
	int n; 
	n = FindCol(sCol); 
	if (n == -1) 
	{
		memset(st, 0, sizeof(SYSTEMTIME)); 
	}
	else
	{
		GetDate(n, st); 
	}
}
//-----------------------------------------------------------------------------

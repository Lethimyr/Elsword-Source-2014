#pragma once

#include "KSingleton.h"

#include <sql.h>
#include <sqlext.h>
#include <windows.h>

#include <string>
using namespace std;

#pragma comment ( lib, "odbc32.lib" )

class KQuery
{

public:
	// 최대 컬럼수, BLOB 입출력 단위, NULL 필드값
	enum
	{
		eMAXCOL		= 100,
		eBLOBBATCH	= 10000,

		eQueryNULL	= -100,
		eQueryEOF	= -101, 
		eQueryNOCOL	= -102,
		eQueryERROR	= -103,
		eQueryOK	= 1
	};

	enum
	{
		eConnectType_MDB = 1,
		eConnectType_SQL,
		eConnectType_SQLDirect,
		eConnectType_Max
	};

protected:
	SQLHENV		m_hEnv;							// 환경 핸들
	SQLHDBC		m_hDbc;							// 연결 핸들
	SQLRETURN	m_hRet;							// 최후 실행한 SQL문의 결과값
	char		m_acCol[eMAXCOL][255];				// 바인딩될 컬럼 정보

	// 재접속을 위한 것
	int			m_iType;
	char		m_pcConStr[255];
	char		m_pcUID[255];
	char		m_pcPWD[255];

public:
	SQLINTEGER	m_iAffectCount;					// 영향받은 레코드 개수
	SQLHSTMT	m_hhStmt;						// 명령 핸들. 직접 사용할 수도 있으므로 public으로 정의
	SQLSMALLINT m_iCol;							// 컬럼 개수
	SQLCHAR		m_pcColName[eMAXCOL][50];			// 컬럼의 이름들
	SQLINTEGER	m_alCol[eMAXCOL];					// 컬럼의 길이/상태 정보

	KQuery();									// 생성자
	~KQuery();									// 파괴자:연결 핸들을 해제한다.

	void PrintDiag();							// 진단 정보 출력
	BOOL Connect(int Type, char* ConStr, 
		char* UID = NULL, char* PWD = NULL);	// 데이터 소스에 연결한다.
	BOOL Exec(LPCTSTR szSQL);					// SQL문을 실행한다.
	int ExecGetInt(LPCTSTR szSQL, int& iVal);	// SQL문을 실행하고 첫번째 컬럼의 정수 읽음
	int ExecGetStr(LPCTSTR szSQL, char* buf);	// SQL문을 실행하고 첫번째 컬럼의 문자열 읽음
	SQLRETURN Fetch();							// 한 행 가져오기
	void Clear();								// 커서 해제 및 언 바인딩
	int FindCol(char* name);					// 컬럼의 이름으로부터 번호를 찾아준다.

	//////////////////////////////////////////////////////////////////////////
	// 컬럼 읽기
	int GetInt(int nCol, int& iVal);
	int GetInt(char* sCol, int& iVal);
	int GetFloat(int nCol, float& fVal);
	int GetFloat(char* sCol, float& fVal);
	int GetStr(int nCol, char* buf);
	int GetStr(char* sCol, char* buf);
	int GetStr(char* sCol, std::string& strBuf);

	int ReadBlob(LPCTSTR szSQL, void* buf);
	void WriteBlob(LPCTSTR szSQL, void* buf, int size);

	// yyyy-mm-dd 포맷으로 문자열을 날짜 구조체에 넣는다.
	void StrToTime(char* str, SYSTEMTIME* st);
	void GetDate(int nCol, SYSTEMTIME* st);
	void GetDate(char *sCol, SYSTEMTIME* st);

};

KDeclareGlobalVal( KQuery );
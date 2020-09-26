// NMRunParamModuleLib.h: interface for the NMRunParamModuleLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMRUNPARAMLIB_H__FE27D416_83D5_4D19_BD28_95BEB0519D2B__INCLUDED_)
#define AFX_NMRUNPARAMLIB_H__FE27D416_83D5_4D19_BD28_95BEB0519D2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
// Define Lib Name

#if !defined(_LIB) && !defined(_LINKED_NMRUNPARAMLIB)
  #ifndef _NMRUNPARAMLIBNAME
    #define _NMRUNPARAMLIBNAME "NMRunParamLib"
  #endif //#ifndef _NMRUNPARAMLIBNAME

  #ifdef _DLL
    #ifdef _DEBUG	// Debug multithread DLL
      #pragma comment(lib, _NMRUNPARAMLIBNAME "_MDd")
    #else			// multithread DLL
      #pragma comment(lib, _NMRUNPARAMLIBNAME "_MD")
    #endif // #ifdef _DEBUG
  #else
    #ifdef _DEBUG	// Debug multithread
      #pragma comment(lib, _NMRUNPARAMLIBNAME "_MTd")
    #else			// multithread
      #pragma comment(lib, _NMRUNPARAMLIBNAME "_MT")
    #endif // #ifdef _DEBUG
  #endif // #ifdef _DLL

  #define _LINKED_NMRUNPARAMLIB
#endif //#if !defined(_LIB) && !defined(_LINKED_NMRUNPARAMLIB)

//////////////////////////////////////////////////////////////////////////
// Define RunParam Name
/**
@defgroup GROUPERRCODE NMRunParamLib 오류코드
@{
*/
/// @brief - NMRunParamLib 오류코드
enum ERROR_NMRUNPARAM {
	ERROR_NMRUNPARAM_SUCCESS = 0,					///< 성공(오류 없음).
///	@details <b> ERROR_NMRUNPARAM_COMMON,
	ERROR_NMRUNPARAM_COMMON_NOT_IMPLEMENTED = 1001,	///< 구현되어있지 않음.
	ERROR_NMRUNPARAM_COMMON_NOT_SUPPORT,			///< 지원하지 않음.
///	@details <b> ERROR_NMRUNPARAM_LIB
	ERROR_NMRUNPARAM_LIB_NOTINIT = 2001,			///< 정적 라이브러리가 초기화되지 않았음.
	ERROR_NMRUNPARAM_LIB_INVALID_PARAM,				///< 잘못된 파라미터
	ERROR_NMRUNPARAM_LIB_DLL_NOT_EXISTS,			///< DLL 파일이 존재하지 않음
	ERROR_NMRUNPARAM_LIB_DLL_INVALID,				///< 잘못된 DLL 파일
	ERROR_NMRUNPARAM_LIB_DLL_CANT_LOAD,				///< DLL 을 로드할 수 없음.
	ERROR_NMRUNPARAM_LIB_DLL_NOT_LOADED,			///< DLL 이 로드되지 않음.
	ERROR_NMRUNPARAM_LIB_DLL_ALREADY_LOADED,		///< DLL 이 이미 로드됨.
	ERROR_NMRUNPARAM_LIB_DLLPROC_NOT_FOUND,			///< DLL Export Function을 찾을수 없음.
	ERROR_NMRUNPARAM_LIB_NOTEXIST_ENVVAR_COMMONPATH,	///< 공통 모듈 경로 환경 변수를 찾을 수 없음.
	ERROR_NMRUNPARAM_LIB_MISSING_PARAM,				///< 파라미터가 누락됨.
///	@details <b> ERROR_NMRUNPARAM_API
	ERROR_NMRUNPARAM_API_INVALID_PARAM = 3001,		///< 잘못된 파라미터.
	ERROR_NMRUNPARAM_API_NOTENOUGHBUFFER,			///< 버퍼의 크기가 충분하지 않음.
	ERROR_NMRUNPARAM_API_ENVVAR_NOTEXIST,			///< 환경변수가 존재하지 않음.
	ERROR_NMRUNPARAM_API_ENVVAR_SET_FAIL,			///< 환경 별수 설정 실패.
///	@details <b> ERROR_NMRUNPARAM_API_ENVVAR
	ERROR_NMRUNPARAM_API_ENVVAR_NOTEXISTS = 3101,	///< 환경변수가 존재하지 않음.
	ERROR_NMRUNPARAM_API_ENVVAR_FAIL_LOAD_DATA,		///< 환경 변수를 읽는데 실패함.
///	@details <b> ERROR_NMRUNPARAM_API_CMDARG
	ERROR_NMRUNPARAM_API_CMDARG_GET_FAIL = 3201,	///< 실행 인자를 가져오는데 실패함.
	ERROR_NMRUNPARAM_API_CMDARG_INVALIDPARAM,		///< 실행 인자가 잘못되었음.
	ERROR_NMRUNPARAM_API_CMDARG_NOTENOUGHBUFFER,	///< 실행 인자를 가져오기 위한 버퍼의 크기가 부족함.
	ERROR_NMRUNPARAM_API_CMDARG_OUT_OF_MEMORY,		///< 실행 인자를 가져올수 없음(메모리 부족).
	ERROR_NMRUNPARAM_API_CMDARG_NOT_EXISTS_PARAM,	///< 실행 인자가 존재하지 않음.
	ERROR_NMRUNPARAM_API_CMDARG_RUNPROGRAM_NOT_HAVE_PATH,	///< 실행 프로그램의 경로가 존재하지 않음.
	ERROR_NMRUNPARAM_API_CMDARG_CURPATH_CANT_CHANGE,		///< 현재 경로를 변경할 수 없음.
///	@details <b> ERROR_NMRUNPARAM_MODULE
	ERROR_NMRUNPARAM_MODULE_INVALID_PARAM = 5001,	///< 잘못된 파라미터
	ERROR_NMRUNPARAM_MODULE_NOT_ENOUGH_MEM,			///< 메모리 부족.
	ERROR_NMRUNPARAM_MODULE_ALREADY_CREATED,		///< 실행 파라미터 모듈이 이미 생성되었음.
	ERROR_NMRUNPARAM_MODULE_NOTFOUND_DATA,			///< 데이터를 찾을 수 없음.
	ERROR_NMRUNPARAM_MODULE_ENV_INVALID_FORMAT,		///< 잘못된 환경변수 형식
	ERROR_NMRUNPARAM_MODULE_CMDARGKEY_NOTEXISTS,	///< 환경변수 복호화 키값이 존재하지 않음.
	ERROR_NMRUNPARAM_MODULE_GROUPMAP_NOT_FOUND,		///< 그룹 맵을 찾지(생성하지) 못함.
///	@details <b> ERROR_NMRUNPARAM_MODULE_DATAMAP
	ERROR_NMRUNPARAM_MODULE_DATAMAP_INSERT_FAIL = 5101,	///< 데이터 맵에 Insert 실패
///	@details <b> ERROR_NMRUNPARAM_MODULE_PARAMDATA
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_NEW_FAIL,		///< 파라미터 데이트 생성 실패
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_SETDATA_FAIL,	///< 파라미터 데이터 설정 실패
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_INSERT_FAIL,	///< 파라미터 데이터 Insert 실패
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_NOT_EXISTS,	///< 파라미터 데이터가 존재하지 않음.
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_EMPTYDATA,	///< 파라미터 데이터가 비어있음.
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_GETESCAPEDDATA_FAIL,	///< 탈출문자 데이터 가져오기 실패.
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_NOTENOUGHBUFFER,		///< 버퍼의 크기가 충분하지 않음.
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_SETENV_FAIL,			///< 환경변수 세팅에 실패함.
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_RUNPROGRAM_FAIL,		///< 프로그램 실행 실패.
///	@details <b> ERROR_NMRUNPARAM_MODULE_TDES
	ERROR_NMRUNPARAM_MODULE_TDES_INIT_FAIL = 5301,			///< TDES 모듈 초기화 실패
	ERROR_NMRUNPARAM_MODULE_TDES_ENCRYPT_FAIL,				///< TDES 암호화 실패
	ERROR_NMRUNPARAM_MODULE_TDES_DECRYPT_FAIL,				///< TDES 복호화 실패
	ERROR_NMRUNPARAM_MODULE_TDES_NOTENOUGHBUFFER,			///< 버퍼의 크기가 충분하지 않음.
	ERROR_NMRUNPARAM_MODULE_TDES_INVALID_DATALENGTH,		///< 잘못된 데이터 길이.
};
/**
@}
*/

//////////////////////////////////////////////////////////////////////////
// Define RunParam Name
/**
@defgroup GROUPNMPARAMNAME 예약된 파라미터 이름 정의
@{
*/
#define NMPARAMNAME_AUTHCOOKIE		"AuthCookie"	///< 인증쿠키 파라미터 이름 정의
#define NMPARAMNAME_DATACOOKIE		"DataCookie"	///< 데이터쿠키 파라미터 이름 정의
#define NMPARAMNAME_CPCOOKIE		"CpCookie"		///< CP쿠키 파라미터 이름 정의
#define NMPARAMNAME_SERVPORT 		"ServPort"		///< 서버 포트 파라미터 이름 정의
#define NMPARAMNAME_SERVIP			"ServIP"		///< 서버 아이피 파라미터 이름 정의
#define NMPARAMNAME_SPAREPARAM		"SpareParam"	///< 여분 인자 파라미터 이름 정의
/**
@}
*/

//////////////////////////////////////////////////////////////////////////
// Default 

typedef unsigned long	NMRUNPARAM_RETCODE;

//////////////////////////////////////////////////////////////////////////
// Function Definition

/// @brief NMRunParam API 함수
namespace NMRUNPARAMAPIFUNC
{
	/**
	@brif - 수동으로 DLL 파일 경로를 선택한다. (디버깅용)
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] szFileName - 수동으로 설정할 DLL 파일의 경로를 입력한다.
	*/
	NMRUNPARAM_RETCODE	SetDLLFileName(LPCSTR szFileName);

	// convert return Code 2 String 
	/**
	@brif - @ref ERROR_NMRUNPARAM 열거자 코드를 문자열 코드로 변환한다.
	@returns 입력된 열거자 @ref ERROR_NMRUNPARAM 코드 값을 문자열 코드로 변환한 값을 반환한다.
	@param[in] retCode - 문자열 코드로 변환할 열거자 @ref ERROR_NMRUNPARAM 값을 입력한다.
	*/
	LPCSTR				ConvertRetCode2String(NMRUNPARAM_RETCODE retCode);

	/**
	@brif - 실행 결과 코드 값(WinExec 나 ShellExcute 의 리턴값)을 문자열 코드로 변환한다.
	@returns 입력된 실행 결과 코드 값(WinExec 나 ShellExcute 의 리턴값)을 문자열 코드로 변환한 값을 반환한다.
	@param[in] uErr - 문자열 코드로 변환할 실행 결과 코드 값(WinExec 나 ShellExcute 의 리턴값)을 입력한다.
	*/
	LPCSTR				ConvertRunProgError2String(UINT uErr);
};

/// @brief 환견 변수 관련 NMRunParam API 함수
namespace NMRUNPARAMAPIFUNC_ENVVAR
{
	/// @brief - 예약된 환경 변수 코드 열거자.
	enum ENVVARNAME {
		ENVVARNAME_SITENAME = 0,	///< 사이트 코드 환경 변수 코드
		ENVVARNAME_COMMONPATH,		///< 공통 경로 환경 변수 코드
	};

	// Process Environment Function
	/**
	@brif - 주어진 환경 변수 이름 코드를 바탕으로 저장된 환경 변수를 가져온다.
	@returns 주어진 환경 변수 이름 코드를 바탕으로 저장된 환경 변수를 반환한다.
	@param[in] eName - 가져올 환경 변수 값의 환경 변수 이름 코드를 입력한다.
	*/
	LPCSTR				GetEnvName(ENVVARNAME eName);
	/**
	@brif - 주어진 환경 변수 이름을 바탕으로 저장된 환경 변수를 가져온다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] szEnvName - 가져올 환경 변수 값의 환경 변수 이름를 입력한다.
	@param[out] szBuffer - 가져올 환경 변수 값이 저장될 버퍼의 포인터를 입력한다.
	@param[in] dwBufferLen - 가져올 환경 변수 값이 저장될 버퍼의 길이를 입력한다.
	*/
	NMRUNPARAM_RETCODE	GetEnvVar(LPCSTR szEnvName, LPSTR szBuffer, DWORD dwBufferLen);
	/**
	@brif - 주어진 환경 변수 이름 코드를 바탕으로 저장된 환경 변수를 가져온다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] eName - 가져올 환경 변수 값의 환경 변수 이름 코드를 입력한다.
	@param[out] szBuffer - 가져올 환경 변수 값이 저장될 버퍼의 포인터를 입력한다.
	@param[in] dwBufferLen - 가져올 환경 변수 값이 저장될 버퍼의 길이를 입력한다.
	*/
	NMRUNPARAM_RETCODE	GetEnvVar(ENVVARNAME eName, LPSTR szBuffer, DWORD dwBufferLen);
	

	/**
	@brif - 주어진 환경 변수 이름 코드를 바탕으로 저장된 환경 변수의 문자열 길이를 가져온다.
	@returns 입력된 환경 변수 이름 코드를 바탕으로 저장된 환경 변수의 문자열 길이를 반환한다.
	@param[in] eName - 가져올 환경 변수 값의 환경 변수 이름 코드를 입력한다.
	*/
	DWORD				GetEnvVarLength(ENVVARNAME eName);
	/**
	@brif - 주어진 환경 변수 이름을 바탕으로 저장된 환경 변수의 문자열 길이를 가져온다.
	@returns 입력된 환경 변수 이름을 바탕으로 저장된 환경 변수의 문자열 길이를 반환한다.
	@param[in] szEnvName - 가져올 환경 변수 값의 환경 변수 이름을 입력한다.
	*/
	DWORD				GetEnvVarLength(LPCSTR szEnvName);

	/**
	@brif - 주어진 환경 변수 이름 코드에 환경 변수 값을 저장한다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] eName - 저장할 환경 변수 이름 코드 값을 입력한다.
	@param[in] szValue - 저장할 환경 변수 값을 입력한다.
	*/
	NMRUNPARAM_RETCODE	SetEnvVar(ENVVARNAME eName, LPCSTR szValue);
	/**
	@brif - 주어진 환경 변수 이름 에 환경 변수 값을 저장한다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] szEnvName - 저장할 환경 변수 이름을 입력한다.
	@param[in] szValue - 저장할 환경 변수 값을 입력한다.
	*/
	NMRUNPARAM_RETCODE	SetEnvVar(LPCSTR szEnvName, LPCSTR szValue);

	/**
	@brif - 사이트 코드 환경 변수 값을 가져온다. (미구현)
	@returns 사이트 코드 환경 변수 값을 반환한다.
	*/
	LPCSTR				GetSiteName();
	/**
	@brif - 사이트 코드 환경 변수 값을 설정한다. (미구현)
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] szSiteName - 사이트 코드 환경 변수 값에 저장될 사이트 이름을 입력한다.
	*/
	NMRUNPARAM_RETCODE	SetSiteName(LPCSTR szSiteName);
	
	/**
	@brif - 공통 모듈 경로를 가져온다.
	@returns 공통 모듈 경로를 반환한다.
	*/
	LPCSTR				GetCommonPath();
	/**
	@brif - 공통 모듈 경로를 설정한다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] szCommonPath - 설정할 공통 모듈 경로를 입력한다.
	*/
	NMRUNPARAM_RETCODE	SetCommonPath(LPCSTR szCommonPath);
};

/// @brief Command line Argument 관련 NMRunParam API 함수
namespace NMRUNPARAMAPIFUNC_CMDARG
{
	/// @brif Command line Argument 인덱스 열거자.
	enum EDEFARGIDX {
		DEFARGIDX_RUNPARAMKEY = 1, ///< 암복호화 키를 지칭하는 Command line Argument 인덱스
	};
	
	// Command line argument function
	/**
	@brif - Command line Argument 갯수를 가져온다.
	@returns Command line Argument 갯수를 반환한다.
	*/
	DWORD				GetCount();
	
	/**
	@brif - 지시된 인덱스의 Command line Argument 값을 가져온다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] iArgIdx - 가져올 Command line Argument 의 인덱스를 입력한다.
	@param[out] szBuffer - 주어진 인덱스의 Command line Argument 값이 저장될 버퍼를 입력한다.
	@param[in] dwBufferLen - 주어진 인덱스의 Command line Argument 값이 저장될 버퍼의 길이를 입력한다.
	*/
	NMRUNPARAM_RETCODE	GetArg(unsigned short iArgIdx, LPSTR szBuffer, DWORD dwBufferLen);
	/**
	@brif - 지시된 인덱스의 Command line Argument 값의 문자열 길이를 가져온다.
	@returns 지시된 인덱스의 Command line Argument 값의 문자열 길이를 반환한다.
	@param[in] iArgIdx - 가져올 Command line Argument 의 인덱스를 입력한다.
	*/
	DWORD				GetArgLength(unsigned short iArgIdx);
	
	// Command line argument extended function
	/**
	@brif - 현재 실행된 파일의 경로를 가져온다.
	@returns 현재 실행된 파일의 경로를 반환한다.
	*/
	LPCSTR				GetRunFilePath();
	/**
	@brif - 실행 파라미터 복호화 키(Command line Argument 인덱스 1이 지칭하는 값)을 가져온다.
	@returns 실행 파라미터 복호화 키(Command line Argument 인덱스 1이 지칭하는 값)을 반환한다.
	*/
	LPCSTR				GetRunParamKey();
	
	/**
	@brif - 현재 프로세스의 Current Directory 를 실행파일 경로로 변환한다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	*/
	NMRUNPARAM_RETCODE	SetCurPathToRunFilePath();
};

/// @brief 실행 파라미터 암호화 키 관련 NMRunParam API 함수
namespace NMRUNPARAMAPIFUNC_RUNPARAMKEY
{
	/**
	@brif - 실행 파라미터 암호화 키를 새로 생성한다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[out] szBuffer - 실행 파라미터 암호화 키가 저장될 버퍼의 포인터를 입력한다.
	@param[in] dwLength - 실행 파라미터 암호화 키가 저장될 버퍼의 길이를 입력한다.
	*/
	NMRUNPARAM_RETCODE	Create(LPSTR szBuffer, DWORD dwLength);
	/**
	@brif - 새로 생성될 실행 파라미터 암호화 키를 저장되는데 필요한 버퍼의 길이를 가져온다.
	@returns 새로 생성될 실행 파라미터 암호화 키를 저장되는데 필요한 버퍼의 길이를 반환한다.
	*/
	DWORD				GetNeedLength();	
};





//////////////////////////////////////////////////////////////////////////
// Class Definition

/// @brief 실행 파라미터 클래스
class CNMRunParam
{
public:
	/** @brif - CNMRunParam 클래스 생성자 */
	CNMRunParam();
	/** @brif - CNMRunParam 클래스 소멸자 */
	virtual ~CNMRunParam();

	/**
	@brif - 실행 파라미터를 환경변수로 부터 읽어온다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] szDecKey - 실행 파라미터 복호화 키를 입력한다.@br
						해당 값이 NULL일 경우 Command Line Argument 1번째 값을 읽어 처리한다.
	@param[in] bClearEnv - 실행 파라미터 로드후 환경 변수에서 데이터를 제거할지를 입력한다. true를 입력할경우 제거되며, false를 입력할 경우 제거되지 않는다.
	*/
	NMRUNPARAM_RETCODE	Load(LPCSTR szDecKey = NULL, bool bClearEnv = true);
	/**
	@brif - 현재 클래스에 저장된 파라미터를 암호화하여 환경 변수에 저장한다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] szKey - 환경 변수에 저장될때 암호화할 키값을 입력한다.
	*/
	NMRUNPARAM_RETCODE	Save(LPCSTR szKey);
	/**
	@brif - 주어진 프로그램을 현재 클래스의 저장된 파라미터를 전달하여 실행한다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] lpszRunFile - 실행할 프로그램 파일을 입력한다.
	@param[out] pRetExecCode - 주어진 프로그램의 실행 결과값이 저장될 포인터를 입력한다. NULL일 경우 저장되지 않는다.
	@param[in] szEncKey - 현재 클래스의 저장된 파라미터가 암호화될 암호화키를 입력한다. NULL일 경우 자동으로 생성하여 전달한다.
	*/
	NMRUNPARAM_RETCODE	RunProgram(LPCSTR lpszRunFile,UINT* pRetExecCode = NULL, LPCSTR szEncKey = NULL);

	/**
	@brif - 주어진 이름의 실행 파라미터 값을 가져온다.
	@returns 주어진 이름의 실행 파라미터 값을 반환한다.
	@param[in] lpszGroup - 파라미터 그룹 이름. NULL일 경우 인자가 하나인 GetParam() 을 호출하는 것과 동일함.
	@param[in] lpszName - 파라미터 이름
	*/
	LPCSTR				GetParam(LPCSTR lpszGroup, LPCSTR lpszName);
	/**
	@brif - 주어진 이름의 실행 파라미터 값을 가져온다.
	@remark 그룹 이름 인자가 생략된 버전의 메소드들은 내부적으로 존재하는 디폴트 그룹에서 파라미터를 찾음
	@returns 주어진 이름의 실행 파라미터 값을 반환한다.
	@param[in] lpszName - 파라미터 이름
	*/
	LPCSTR				GetParam(LPCSTR lpszName);

	/**
	@brif - 주어진 이름의 실행 파라미터 값을 설정한다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] lpszGroup - 설정할 실행 파라미터 그룹 이름을 입력한다.
	@param[in] lpszName - 설정할 실행 파라미터 이름을 입력한다.
	@param[in] lpszValue - 설정할 실행 파라미터 값을 입력한다.
	*/
	NMRUNPARAM_RETCODE	SetParam(LPCSTR lpszGroup, LPCSTR lpszName, LPCSTR lpszValue);
	/**
	@brif - 주어진 이름의 실행 파라미터 값을 설정한다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] lpszName - 설정할 실행 파라미터 이름을 입력한다.
	@param[in] lpszValue - 설정할 실행 파라미터 값을 입력한다.
	*/
	NMRUNPARAM_RETCODE	SetParam(LPCSTR lpszName, LPCSTR lpszValue);

	/**
	@brif - 주어진 이름의 실행 파라미터 값을 제거한다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] lpszGroup - 제거할 실행 파라미터의 그룹 이름을 입력한다.
	@param[in] lpszName - 제거할 실행 파라미터 이름을 입력한다.
	*/
	NMRUNPARAM_RETCODE	RemoveParam(LPCSTR lpszGroup, LPCSTR lpszName);
	/**
	@brif - 주어진 이름의 실행 파라미터 값을 제거한다.
	@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
	@param[in] lpszName - 제거할 실행 파라미터 이름을 입력한다.
	*/
	NMRUNPARAM_RETCODE	RemoveParam(LPCSTR lpszName);

	/**
	@brif - 주어진 인덱스의 실행 파라미터의 그룹 이름을 가져온다.
	@returns 주어진 인덱스의 실행 파라미터의 그룹 이름을 반환한다.
	@param[in] dwIdx - 인덱스를 입력한다.
	*/
	LPCSTR				GetGroupName(DWORD dwIdx);
	/**
	@brif - 저장된 실행 파라미터 그룹의 갯수를 가져온다.
	@returns 저장된 실행 파라미터 그룹의 갯수를 반환한다.
	*/
	DWORD				GetGroupCount();

	/**
	@brif - 주어진 인덱스의 실행 파라미터 이름을 가져온다.
	@note 현재 정상 동작하지 않음
	@returns 주어진 인덱스의 실행 파라미터 이름을 반환한다.
	@param[in] lpszGroup - 실행 파라미터의 그룹 이름을 입력한다.
	@param[in] dwIdx - 인덱스를 입력한다.
	*/
	LPCSTR				GetParamName(LPCSTR lpszGroup, DWORD dwIdx);
	/**
	@brif - 주어진 인덱스의 실행 파라미터 이름을 가져온다.
	@note 현재 정상 동작하지 않음
	@returns 주어진 인덱스의 실행 파라미터 이름을 반환한다.
	@param[in] dwIdx - 인덱스를 입력한다.
	*/
	LPCSTR				GetParamName(DWORD dwIdx);
	/**
	@brif - 저장된 실행 파라미터 갯수를 가져온다.
	@note 현재 정상 동작하지 않음
	@param[in] lpszGroup - 실행 파라미터의 그룹 이름을 입력한다.
	@returns 저장된 실행 파라미터 갯수를 반환한다.
	*/
	DWORD				GetParamCount(LPCSTR lpszGroup);
	/**
	@brif - 저장된 실행 파라미터 갯수를 가져온다.
	@note 현재 정상 동작하지 않음
	@returns 저장된 실행 파라미터 갯수를 반환한다.
	*/
	DWORD				GetParamCount();

	/**
	@brif - 주어진 이름의 실행 파라미터가 존재하는지 체크한다,
	@returns 주어진 이름의 실행 파라미터가 존재하면 true값을, 존재하지 않으면 false값을 반환한다.
	@param[in] lpszGroup - 체크할 실행 파라미터의 그룹 이름을 입력한다.
	@param[in] lpszName - 체크할 실행 파라미터의 이름을 입력한다.
	*/
	bool				ExistsParam(LPCSTR lpszGroup, LPCSTR lpszName);
	/**
	@brif - 주어진 이름의 실행 파라미터가 존재하는지 체크한다,
	@returns 주어진 이름의 실행 파라미터가 존재하면 true값을, 존재하지 않으면 false값을 반환한다.
	@param[in] lpszName - 체크할 실행 파라미터의 이름을 입력한다.
	*/
	bool				ExistsParam(LPCSTR lpszName);
	
	/**
	@brif - 주어진 그룹에 포함된 모든 파라미터들을 하나의 문자열로 반환한다.
	@returns Name1=Value1|Name2=Value2| ... 형식의 문자열
	@param[in] lpszGroup - 파라미터 그룹 이름
	*/
	LPCSTR				GetParamString(LPCSTR lpszGroup);

};
#endif // !defined(AFX_NMRUNPARAMLIB_H__FE27D416_83D5_4D19_BD28_95BEB0519D2B__INCLUDED_)

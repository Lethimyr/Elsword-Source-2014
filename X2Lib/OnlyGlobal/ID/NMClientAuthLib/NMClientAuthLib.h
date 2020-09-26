// NNClientAuthLib.h: interface for the CSSOWebLib class.
//
/// @file	NNClientAuthLib.h
/// @brief	Client Authentication Library Header
/// @author	Jeong Yeosang<ysjeong76@cj.net>
/// @date 	2012.09.25
//
//////////////////////////////////////////////////////////////////////

#define COUNTRYCODE_INDONESIA	1
#define _DLL_EXPORTFUNC_ extern "C" _declspec(dllexport)


/**
@brif - 인증과 관련된 모든 데이타 변수를 초기화 한다.
@returns 없음
*/
_DLL_EXPORTFUNC_ void NMClientAuthDLL_ClearAuthenticationData();

/**
@brif - 인증과 관련된 정보들을 전달하여 정보를 셋팅한다.
@returns 성공 여부를 리턴한다.
@param[in] userID - 사용자 아이디
@param[in] password - 패스워드
@param[in] gameCode - 실행하는 게임 코드
@param[in] redirectUrl - Redirect 될 URL
@param[in] isPersisted - 로그인 유지 기능 사용 여부
@param[in] isGameToken - 게임 실행을 위한 토큰 발급 여부
@param[in] domain - 법인 도메인
@param[in] xmlURL - 인증 연동 URL
*/
_DLL_EXPORTFUNC_ BOOL NMClientAuthDLL_SetLoginData(	LPCSTR userID,
																						LPCSTR password,
																						LPCSTR gameCode,
																						LPCSTR redirectUrl,
																						BOOL isPersisted,
																						BOOL isGameToken,
																						LPCSTR domain,
																						LPCSTR xmlURL);

/**
@brif - 인증 서버와 통신을 하여 실제 인증절차를 진행하고 성공 시 인증데이타들을 환경변수로 저장한다.
@returns 성공 여부를 리턴한다.
*/
_DLL_EXPORTFUNC_ BOOL NMClientAuthDLL_LoginProcessExecution();

/**
@brif - 인증이완료되면 호출하여 인승성공 여부와 에러코드, 에러메시지, 인증서버 통신쿼리 코드를 받는다.
@returns 성공 여부를 리턴한다. MBCS 형태의 메시지를 리턴받기를 원할 때 사용한다.
@param[in] loginSuccess - 인증 성공 여부
@param[in] errorMessage - 에러 메시지(MBCS)
@param[in] errorCode - 에러 코드(MBCS)
@param[in] queryInfoStatusCode - 함수내 HTTP 통신 쿼리의 리턴값
*/
_DLL_EXPORTFUNC_ BOOL NMClientAuthDLL_IsLoginComplete(	BOOL& loginSuccess,
																							LPCSTR& errorMessage,
																							LPCSTR& errorCode,
																							int& queryInfoStatusCode);

/**
@brif - 인증이완료되면 호출하여 인승성공 여부와 에러코드, 에러메시지, 인증서버 통신쿼리 코드를 받는다.
@returns 성공 여부를 리턴한다. UNICODE 형태의 메시지를 리턴받기를 원할 때 사용한다.
@param[in] loginSuccess - 인증 성공 여부
@param[in] errorMessage - 에러 메시지(UNICODE)
@param[in] errorCode - 에러 코드(UNICODE)
@param[in] queryInfoStatusCode - 함수내 HTTP 통신 쿼리의 리턴값
*/
_DLL_EXPORTFUNC_ BOOL NMClientAuthDLL_IsLoginComplete_UTF16(	BOOL& loginSuccess,
																							LPCWSTR& errorMessage,
																							LPCWSTR& errorCode,
																							int& queryInfoStatusCode);
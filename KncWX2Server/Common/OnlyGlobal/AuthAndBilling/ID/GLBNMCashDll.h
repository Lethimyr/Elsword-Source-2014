// 다음 ifdef 블록은 DLL에서 내보내기하는 작업을 쉽게 해 주는 매크로를 만드는 
// 표준 방식입니다. 이 DLL에 들어 있는 파일은 모두 명령줄에 정의된 _EXPORTS 기호로
// 컴파일되며, 동일한 DLL을 사용하는 다른 프로젝트에서는 이 기호를 정의할 수 없습니다.
// 이렇게 하면 소스 파일에 이 파일이 들어 있는 다른 모든 프로젝트에서는 
// GLBNMCASHDLL_API 함수를 DLL에서 가져오는 것으로 보고, 이 DLL은
// 이 DLL은 해당 매크로로 정의된 기호가 내보내지는 것으로 봅니다.
#ifdef GLBNMCASHDLL_EXPORTS
#define GLBNMCASHDLL_API __declspec(dllexport)
#else
#define GLBNMCASHDLL_API __declspec(dllimport)
#endif


#if !defined(_LIB) && !defined(_GLBNMCASHDLL)
	#ifndef _GLBNMCASHDLLNAME
		#ifdef	_WIN64
			#define _GLBNMCASHDLLNAME "GLBNMCashDll_x64"
		#else
			#define _GLBNMCASHDLLNAME "GLBNMCashDll_x86"
		#endif
	#endif //#ifndef _GLBNMCASHDLLNAME

	#ifdef _DLL
		// multithread DLL
		#pragma comment(lib, _GLBNMCASHDLLNAME "_MD")		
	#else
		// multithread
		#pragma comment(lib, _GLBNMCASHDLLNAME "_MT")		
	#endif // #ifdef _DLL
#endif //#if !defined(_LIB) && !defined(_GLBNMCASHDLL)


#ifdef GLBNMCASHDLL_EXPORTS
#define GLBNMCASHDLL_API __declspec(dllexport)
#else
#define GLBNMCASHDLL_API __declspec(dllimport)
#endif

namespace MarbleWorks
{
	namespace GlobalBillCash
	{
		namespace API
		{
			GLBNMCASHDLL_API int REQ_CIS__Balance( PCHAR  ShopCode,	PCHAR UserNo,	PCHAR RemainAmount, int AmtLen,
													PCHAR  CashType,	int TypeLen,	PCHAR ResCD,		int ResLen );
			GLBNMCASHDLL_API int REQ_CIS__Cancel( PCHAR ShopCode,		PCHAR  OrderNo, 		PCHAR UserNo,
												   PCHAR DeductNo,		PCHAR  CancelAmount,	PCHAR UserIP,
												   PCHAR CntryCD,		PCHAR  Fee,         	PCHAR UseAmount,
												   PCHAR AdminID,		PCHAR  CancelCode,		wchar_t* CancelReason,
												   PCHAR DeductType,	PCHAR ResCD,			int ResLen );
			GLBNMCASHDLL_API int REQ_CIS__Add( PCHAR	ShopCode,	PCHAR OrderNo,	PCHAR UserNo,
												wchar_t* UserID,	PCHAR UserIP,	PCHAR CntryCD,
												PCHAR EvtCode,		PCHAR PrjCode,  PCHAR AuthKey,
												PCHAR AddAmount,	int AmtLen,		PCHAR ResCD, int ResLen );
			GLBNMCASHDLL_API int REQ_CIS__Deduct( PCHAR ShopCode,		PCHAR OrderNo,		PCHAR DeductType,
												   PCHAR UserNo,		wchar_t* UserID,	PCHAR UserIP,
												   PCHAR CntryCD,		PCHAR SaleCode,		PCHAR SaleCond,
												   wchar_t* SaleName,	wchar_t* PKgInfo,	wchar_t* DeductList,
												   PCHAR DeductNoList,	int ListLen,		PCHAR ResCD, int ResLen );
			GLBNMCASHDLL_API int REQ_CIS__Gift( PCHAR ShopCode,		PCHAR OrderNo,			PCHAR DeductType,
												 PCHAR UserNo,			wchar_t* UserID,		PCHAR UserIP,
												 PCHAR CntryCD,			PCHAR SaleCode,			PCHAR SaleCond,
												 wchar_t* SaleName,		PCHAR RcvUserNo,		wchar_t* RcvUserID,
												 wchar_t* ServerName,	wchar_t* CharName,		PCHAR CharLevel,
												 wchar_t* PKgInfo,		wchar_t* DeductList,	PCHAR DeductNoList,
												 int ListLen,			PCHAR ResCD,			int ResLen );
		};
	};
};



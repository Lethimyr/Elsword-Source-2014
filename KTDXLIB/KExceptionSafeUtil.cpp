#include "StdAfx.h"

// KExceptionSafeUtil.cpp 는 모든 구성에 대해 
// C/C++ -> 코드 생성 -> C++ 예외 처리 가능 옵션이 '예, SEH 예외 있음 (/EHa) 로 설정되어야 함
// direct3d dynamic vertex buffer 사용시 XP 머신에서 크래쉬 발생하는 운영체제 오류를 방지하기 위한 것임
// - robobeg, 2014-01-21]

#pragma NOTE( "KExceptionSafeUtil.cpp 은 모든 구성에 대해/EHa 옵션이 설정되어 있어야 합니다. (파일 주석 참고)" )

#ifdef KTDGDEVICEFONT_SIZE_CACHE
bool    KExceptionSafeUtil::ESCopyMemory( void* destination, const void* source, size_t length ) throw()
{
    bool    bExceptionOccurred = false;

    try
    {
        CopyMemory( destination, source, length );
    }
    catch(...)
    {
        bExceptionOccurred = true;
    }

    return  bExceptionOccurred;
}//KExceptionSafeUtil::ESCopyMemory()
#else
#if defined(CLIENT_COUNTRY_TH)
bool    KExceptionSafeUtil::ESCopyMemory( void* destination, const void* source, size_t length ) throw()
{
	bool    bExceptionOccurred = false;

	try
	{
		CopyMemory( destination, source, length );
	}
	catch(...)
	{
		bExceptionOccurred = true;
	}

	return  bExceptionOccurred;
}
#endif // defined(CLIENT_COUNTRY_TH)
#endif
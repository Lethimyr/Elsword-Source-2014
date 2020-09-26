#pragma once

// KExceptionSafeUtil.cpp 는 모든 '구성'에 대해 
// C/C++ -> 코드 생성 -> C++ 예외 처리 가능 옵션이 '예, SEH 예외 있음 (/EHa) 로 설정되어야 함
// direct3d dynamic vertex buffer 사용시 XP 머신에서 크래쉬 발생하는 운영체제 오류를 방지하기 위한 것임
// - robobeg, 2014-01-21

namespace   KExceptionSafeUtil
{
    bool    ESCopyMemory( void* destination, const void* source, size_t length ) throw();
}
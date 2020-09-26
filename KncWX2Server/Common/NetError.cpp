#include "NetError.h"

#include <map>
#include <Windows.h>    // DWORD, GetCurrentThreadID
#include "Thread/Locker.h"

namespace NetError  // 변수 선언은 namespace scope를 열고 해주어야 한다.
{

#undef NET_ERROR
#define NET_ERROR( id, comment ) L#comment,
//     wchar_t* szErrorStr[] = {
 //#include "NetError_def.h"
//     };
	
    // key : thread id. element : last error
    std::map<DWORD, NetErrorID> mapLastError;

    std::map<DWORD, NetErrorID>::iterator mit;     // 빠른 작업을 위해..

    KncCriticalSection  csNetError;
}

//09. 08. 17 김정협 클라이언트에는 외국버전 그대로 적용
wchar_t* NetError::GetErrStrF( NetErrorID errorID )
{
    KLocker lock( csNetError );
#ifdef SERV_NETERROR_EU
	if( errorID >= ERR_SENTINEL || errorID < 0 ) return szErrorStrDE[ ERR_SENTINEL ];
	return szErrorStrDE[ errorID ];
#else //SERV_NETERROR_EU
	if( errorID >= ERR_SENTINEL || errorID < 0 ) return szErrorStrF[ ERR_SENTINEL ];
	return szErrorStrF[ errorID ];
#endif //SERV_NETERROR_EU
}

wchar_t* NetError::GetErrStr( NetErrorID errorID )
{
    KLocker lock( csNetError );

    if( errorID >= ERR_SENTINEL || errorID < 0 ) return szErrorStr[ ERR_SENTINEL ];
    return szErrorStr[ errorID ];
}

void NetError::SetLastError( NetErrorID errorID_ )
{
    KLocker lock( csNetError );

    DWORD dwThreadID = ::GetCurrentThreadId();

    mit = mapLastError.find( dwThreadID );
    if( mit == mapLastError.end() ) // 새로운 스레드의 접근
    {
        mapLastError.insert( std::make_pair( dwThreadID, errorID_ ) );
        return;
    }

    mit->second = errorID_;  // 기존 스레드의 값 변경.
}

int NetError::GetLastError()
{
    KLocker lock( csNetError );

    mit = mapLastError.find( ::GetCurrentThreadId() );

    if( mit == mapLastError.end() ) // 새로운 스레드의 접근
        return 0;

    return mit->second;
}

wchar_t* NetError::GetLastErrMsg()
{
    KLocker lock( csNetError );

#ifdef SERV_NETERROR_EU
	// 2010/11/09	조효진 	에러 메시지 한글로 나오는 거 수정.
    return GetErrStrF( GetLastError() );
#else //SERV_NETERROR_EU
	return GetErrStr( GetLastError() ); 
#endif //SERV_NETERROR_EU
}

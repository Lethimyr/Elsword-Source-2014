#ifndef __KHTTPWRAPPER_H__
#define __KHTTPWRAPPER_H__

#include "Patcher.h"
#include "KAssert.h"
#include <wininet.h>
#include <string>



/// HTTP 연결과 파일 다운로드를 담당하는 클래스.
class KHttpWrapper
{
public:
    /// Default constructor.
    KHttpWrapper( void );
    /// Default destructor.
    ~KHttpWrapper( void );
private:
    /// Copy constructor - disabled.
    KHttpWrapper( const KHttpWrapper& )
    { ASSERT( false && _T("Do not use copy constructor!") ); }
    /// Assignment operator - disabled.
    const KHttpWrapper& operator=( const KHttpWrapper& )
    { ASSERT( false && _T("Do not use assignment operator!") ); return *this; }
public:
    /// HTTP 연결을 연다.
    bool Open( void );
    /// HTTP 연결을 닫는다.
    void Close( void );
    /// HTTP 연결을 리셋한다.
    bool Reset( void );
    /// 주어진 파일을 다운로드한다.
    bool Get( const char* szSrcPath_,
              const char* szFileName_,
              const char* szDestPath_,
			  bool bInvalidRect = true ) const;
private:
    /// 주어진 문자열에서 '\'를 '/'로 바꾼다.
    void BackSlash2Slash( const char* szSrc, char* szDst ) const;
    /// 주어진 문자열에서 '/'를 '\'로 바꾼다.
	void Slash2BackSlash( const char* szSrc, char* szDst ) const;
private:
    HINTERNET   m_hInternet; ///< Internet handle.
};

#endif // __KHTTPWRAPPER_H__
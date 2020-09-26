#include "SimObject.h"
#include "time.h"       // _strtime(), _strdate()
#include <KncUtil.h>    // toWideString

//#undef max
#include <boost/random.hpp>

NiImplementRootRTTI( KSimObject );

KSimObject::KSimObject(void) : m_nUID( 0 )
{
    wchar_t buff1[128] = {0};
    wchar_t buff2[128] = {0};
    wchar_t buff3[128] = {0};

    static unsigned __int64 sSeedNum = 0;

#ifdef _CONVERT_VS_2010
    swprintf_s( buff3, L"SOB_%s_%s_%020d", 
        _wstrdate_s( buff1 ), 
        _wstrtime_s( buff2 ), 
        sSeedNum++);
#else
    swprintf( buff3, L"SOB_%s_%s_%020d", 
        _wstrdate( buff1 ), 
        _wstrtime( buff2 ), 
        sSeedNum++);
#endif _CONVERT_VS_2010

    
    m_strName = buff3 ;
}

KSimObject::~KSimObject(void)
{
}

ImplToStringW( KSimObject )
{
    return START_TOSTRINGW
        << TOSTRINGW( m_strName )
        << TOSTRINGW( GetRefCount() );
}

int KSimObject::GetRefCount() const
{
    try
    {
        return shared_from_this().use_count() - 1; 
    }
    catch(...)
    {
        return -1;
    }
}
#pragma once

#import "msxml6.dll"
#import "MSSOAP30.dll" exclude( "IStream", "ISequentialStream", "_LARGE_INTEGER", "_ULARGE_INTEGER", "tagSTATSTG", "_FILETIME", "IErrorInfo" )

#include "SubThread.h"
#include "KncException.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

class KNexonBilling :public KSubThread
{
    struct KWebServiceAddress
    {
        int m_iSoapAction;
        std::string m_strWSDL;
        std::string m_strMethod;

        KWebServiceAddress()
        {
            m_iSoapAction = 0;
            m_strWSDL = std::string();
            m_strMethod = std::string();
        }

        KWebServiceAddress( const KWebServiceAddress& t )
        {
            *this = t;
        }

        KWebServiceAddress& operator=( const KWebServiceAddress& t )
        {
            m_iSoapAction = t.m_iSoapAction;
            m_strWSDL = t.m_strWSDL;
            m_strMethod = t.m_strMethod;

            return *this;
        }
    };

    _DeclareException;
public:
#   undef _ENUM
#   define _ENUM( id ) id,
    enum
    {
#   include "NexonBillingOrder_def.h"
    };

    KNexonBilling();
    virtual ~KNexonBilling();

    static void AddWebServiceAddress( int iSoapAction, const char* szWSDL, const char* szMethod );
    bool GetWebServiceAddress( int iSoapAction, KWebServiceAddress& kInfo );

protected:
    // derived from KSubThread
    virtual void ProcessEvent( const KEventPtr& spEvent );

    bool GetParameters( const KPurchaseCoupon& kInfo, _variant_t* parameters, int nSize );

    // event handler
    _DECL_ON_FUNC( EGS_APPLY_COUPON_REQ, KPurchaseCoupon );

public:
protected:
    static std::map< int, KWebServiceAddress >  ms_mapWebServiceAddress;
};
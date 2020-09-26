#include "ActorManager.h"
#include "NexonBilling.h"

#include "NetError.h"

#define CLASS_TYPE  KNexonBilling

std::map< int, KNexonBilling::KWebServiceAddress >  KNexonBilling::ms_mapWebServiceAddress;

_ImplementException( KNexonBilling );

KNexonBilling::KNexonBilling()
{
}

KNexonBilling::~KNexonBilling()
{
    End( 15000 );
}

void KNexonBilling::AddWebServiceAddress( int iSoapAction, const char* szWSDL, const char* szMethod )
{
    if( iSoapAction < 0 || iSoapAction >= NBO_NUM )
    {
        START_LOG( cerr, L"이상한 SOAP Action." )
            << BUILD_LOG( iSoapAction )
            << END_LOG;

        return;
    }

    if( szWSDL == NULL )
    {
        START_LOG( cerr, L"WSDL 이상." )
            << BUILD_LOG( iSoapAction )
            << END_LOG;

        return;
    }

    if( szMethod == NULL )
    {
        START_LOG( cerr, L"method 이상." )
            << BUILD_LOG( iSoapAction )
            << END_LOG;

        return;
    }

    KWebServiceAddress kAddress;
    kAddress.m_iSoapAction = iSoapAction;
    kAddress.m_strWSDL = szWSDL;
    kAddress.m_strMethod = szMethod;

    std::map< int, KWebServiceAddress >::iterator mit = ms_mapWebServiceAddress.find( iSoapAction );
    if( mit != ms_mapWebServiceAddress.end() )
    {
        START_LOG( cerr, L"이미 존재하는 soap action." )
            << BUILD_LOG( iSoapAction )
            << END_LOG;
    }

    // 무조건 넣는다.
    ms_mapWebServiceAddress[iSoapAction] = kAddress;
}

bool KNexonBilling::GetWebServiceAddress( int iSoapAction, KWebServiceAddress& kInfo )
{
    if( iSoapAction < 0 || iSoapAction >= NBO_NUM )
    {
        START_LOG( cerr, L"이상한 SOAP Action." )
            << BUILD_LOG( iSoapAction )
            << END_LOG;

        return false;
    }

    std::map< int, KWebServiceAddress >::iterator mit = ms_mapWebServiceAddress.find( iSoapAction );
    if( mit == ms_mapWebServiceAddress.end() )
    {
        START_LOG( cerr, L"존재하지 않는 soap action." )
            << BUILD_LOG( iSoapAction )
            << END_LOG;

        return false;
    }

    kInfo = mit->second;

    return true;
}

void KNexonBilling::ProcessEvent( const KEventPtr& spEvent_ )
{
    DWORD dwElapTime = ::GetTickCount();

    switch( spEvent_->m_usEventID )
    {
    _CASE( EGS_APPLY_COUPON_REQ, KPurchaseCoupon );
    default:
        START_LOG( cerr, L"핸들러가 지정되지 않은 이벤트." )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( spEvent_->GetIDStr() )
            << END_LOG;
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if( dwElapTime > 3000 )
    {
        START_LOG( cwarn, L"이벤트 처리 소요 시간." )
            << BUILD_LOG( spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime );
    }

    return;
}

bool KNexonBilling::GetParameters( const KPurchaseCoupon& kInfo, _variant_t* parameters, int nSize )
{
    if( parameters == NULL )
    {
        START_LOG( cerr, L"포인터 이상." )
            << END_LOG;

        return false;
    }

    if( nSize != 9 )
    {
        START_LOG( cerr, L"파라미터 개수 이상." )
            << BUILD_LOG( nSize )
            << END_LOG;

        return false;
    }

    for( int i = 0; i < nSize; i++ )
    {
        ::VariantInit( &parameters[i] );
    }

    parameters[8] = kInfo.m_uiNexonSN;
    parameters[7] = kInfo.m_wstrUserID.c_str();
    parameters[6] = kInfo.m_wstrUserName.c_str();
    parameters[5] = kInfo.m_wstrCouponSerial.c_str();
    parameters[4] = ( short )kInfo.m_iGameType;
    parameters[3] = kInfo.m_iServerDomain;
    parameters[2] = kInfo.m_wstrUnitNickName.c_str();
    parameters[1] = ( BYTE )kInfo.m_iReason;
    parameters[0] = kInfo.m_wstrCafeNo.c_str();

    return true;
}

_IMPL_ON_FUNC( EGS_APPLY_COUPON_REQ, KPurchaseCoupon )
{
    KEGS_APPLY_COUPON_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_UNKNOWN;

    KWebServiceAddress kAddress;
    JIF( GetWebServiceAddress( NBO_PURCHASE_COUPON, kAddress ) );

    _BEGIN_TRY_BLOCK;

    HRESULT ret;
    _variant_t vtTrue;
    DISPID dispId;
    _variant_t vtParams[9];
    DISPPARAMS dispParams;
    _variant_t resultInfo;
    EXCEPINFO excepInfo;
    LPOLESTR func;

    CoInitialize( NULL );

    CComPtr< MSSOAPLib30::ISoapClient > pSoapClient;
    ret = pSoapClient.CoCreateInstance( __uuidof( MSSOAPLib30::SoapClient30 ) );
    _JIF( ret == S_OK, goto end_proc );
    _JIF( pSoapClient, goto end_proc );

    vtTrue.vt = VT_BOOL; 
    vtTrue.boolVal = VARIANT_TRUE;

    ret = pSoapClient->put_ClientProperty( _com_util::ConvertStringToBSTR( "ServerHTTPRequest" ), vtTrue );
    _JIF( ret == S_OK, goto end_proc );
    ret = pSoapClient->MSSoapInit( _bstr_t( kAddress.m_strWSDL.c_str() ), "", "", "" );
    _JIF( ret == S_OK, goto end_proc );

    wchar_t szMethod[MAX_PATH];
    lstrcpyW( szMethod, KncUtil::toWideString( kAddress.m_strMethod ).c_str() );
    func = T2OLE( szMethod );

    ret = pSoapClient->GetIDsOfNames( IID_NULL, &func, 1, LOCALE_SYSTEM_DEFAULT, &dispId );
    _JIF( ret == S_OK, goto end_proc );

    _JIF( GetParameters( kPacket_, vtParams, 9 ), goto end_proc );

    dispParams.cArgs = 9;
    dispParams.cNamedArgs = 0;
    dispParams.rgdispidNamedArgs = NULL;
    dispParams.rgvarg = vtParams;

    VariantInit( &resultInfo );
    excepInfo.wCode = 0;
    excepInfo.bstrSource = NULL;
    excepInfo.bstrDescription = NULL;
    excepInfo.bstrHelpFile = NULL;

    ret = pSoapClient->Invoke( dispId, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispParams, &resultInfo, &excepInfo, NULL );
    _JIF( ret == S_OK, goto end_proc );

    _JIF( resultInfo.vt == VT_BOOL, goto end_proc );
    START_LOG( clog, L"웹 서비스 응답." )
        << BUILD_LOG( ( ( VARIANT_BOOL )resultInfo == VARIANT_TRUE ) );

    kPacket.m_iOK = ( ( VARIANT_BOOL )resultInfo == VARIANT_TRUE ) ? NetError::NET_OK : NetError::ERR_NX_WEB_SERVICE_00;

end_proc:
    if( pSoapClient )
    {
        pSoapClient.Release();
    }

    CoUninitialize();

    _END_TRY_BLOCK( dbg::endl
        << BUILD_LOG( kPacket_.m_uiNexonSN )
        << BUILD_LOG( kPacket_.m_wstrUserID )
        << BUILD_LOG( kPacket_.m_wstrUserName )
        << BUILD_LOG( kPacket_.m_wstrCouponSerial )
        << BUILD_LOG( kPacket_.m_iGameType )
        << BUILD_LOG( kPacket_.m_iServerDomain )
        << BUILD_LOG( kPacket_.m_wstrUnitNickName )
        << BUILD_LOG( kPacket_.m_iReason )
        << BUILD_LOG( kPacket_.m_wstrCafeNo )
        );

    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { FIRST_SENDER_UID, -1 };
    spEvent->SetData( PI_GS_PROXY, anTrace, EGS_APPLY_COUPON_ACK, kPacket );
    KActorManager::GetKObj()->QueueingTo( FIRST_SENDER_UID, spEvent );
}
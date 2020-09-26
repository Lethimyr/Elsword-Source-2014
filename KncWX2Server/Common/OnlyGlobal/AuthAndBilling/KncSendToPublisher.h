#pragma once

#include "Event.h"



#if defined (SERV_COUNTRY_US)
#include "US/PayletterBillingManager.h"
#elif defined (SERV_COUNTRY_EU)
#include "EU/GameForgeBillingManager.h"
#elif defined (SERV_COUNTRY_TW)
#include "TW/GashBillingTCPManager.h"
#elif defined (SERV_COUNTRY_JP)
#include "JP/PURPLEBillingManager.h"
#elif defined (SERV_COUNTRY_ID)
#include "ID/CJBillingManager.h"
#elif defined (SERV_COUNTRY_BR)
#include "BR/BRPayletterBillingManager.h"
#elif defined (SERV_COUNTRY_IN)
#include "IN/NaunBillingManager.h"
#endif // SERV_COUNTRY_XX


template <class T>
void KncSendToPublisherBilling( DWORD dwPIFrom_, UidType nFrom_, DWORD dwPITo_, UidType nTo_, UidType anTrace_[], u_short usEventID_, const T& data_ )
{
    KEventPtr spEvent( new KEvent );
    spEvent->SetData( dwPITo_, anTrace_, usEventID_, data_ );
    LIF( spEvent->m_kDestPerformer.AddUID( nTo_ ) );

#if defined (SERV_COUNTRY_US)
	SiKPayletterBillingManager()->QueueingEvent( spEvent );
#elif defined (SERV_COUNTRY_EU)
	SiKGameForgeBillingManager()->QueueingEvent( spEvent );
#elif defined (SERV_COUNTRY_TW)
	SiKGASHBillingTCPManager()->QueueingEvent( spEvent );
#elif defined (SERV_COUNTRY_JP)
	SiKPurpleBillingManager()->QueueingEvent( spEvent );
#elif defined (SERV_COUNTRY_ID)
	SiKCJBillingManager()->QueueingEvent( spEvent );
#elif defined (SERV_COUNTRY_BR)
	SiKBRPayletterBillingManager()->QueueingEvent( spEvent );
#elif defined (SERV_COUNTRY_IN)
	SiKNaunBillingManager()->QueueingEvent( spEvent );
#else // SERV_COUNTRY_XX
	START_LOG( cerr, L"해당 국가 Define에는 KncSendToPublisherBilling 호출하면 안됨" )
		<< BUILD_LOG( dwPIFrom_ )
		<< BUILD_LOG( spEvent->m_kDestPerformer.m_dwPerformerID )
		<< BUILD_LOG( spEvent->m_usEventID )
		<< BUILD_LOG( spEvent->GetIDStr() )
		<< END_LOG;
#endif // SERV_COUNTRY_XX
	
}

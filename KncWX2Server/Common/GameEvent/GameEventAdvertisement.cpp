#include ".\gameeventadvertisement.h"

#ifdef SERV_ADVERTISEMENT_EVENT
KGameEventAdvertisement::KGameEventAdvertisement(void) : 
m_iEventID( 0 ),
m_iRewardID( 0 )
{
}

KGameEventAdvertisement::~KGameEventAdvertisement(void)
{
}
#endif SERV_ADVERTISEMENT_EVENT
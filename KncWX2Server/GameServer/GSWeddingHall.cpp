#include "BaseServer.h"
#include "GSWeddingHall.h"
#include "Enum\Enum.h"



//{{ 2013. 04. 01	 ¿Œø¨ Ω√Ω∫≈€ - ±ËπŒº∫
#ifdef SERV_RELATIONSHIP_SYSTEM

KGSWeddingHall::KGSWeddingHall( const KWeddingHallInfo& kInfo )
{
	std::set< UidType > setUserListDumy;
	UpdateRoomInfo( kInfo, setUserListDumy );
}

KGSWeddingHall::~KGSWeddingHall(void)
{
}

void KGSWeddingHall::UpdateRoomInfo( IN const KGSWeddingHall& kInfo )
{
	m_kHallInfo = kInfo.m_kHallInfo;
	m_setJoinUnitList = kInfo.m_setJoinUnitList;
}

void KGSWeddingHall::UpdateRoomInfo( IN const KWeddingHallInfo& kInfo, IN std::set< UidType >& setList )
{
	m_kHallInfo = kInfo;
	m_setJoinUnitList = setList;
}
#endif SERV_RELATIONSHIP_SYSTEM
//}


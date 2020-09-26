#include "BaseServer.h"
#include ".\GSBattleFieldRoom.h"
#include "Enum\Enum.h"
#include "X2Data\XSLRoom.h"
#include "Lottery.h"
#include "NetError.h"


//{{ 2011. 11. 2	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


KGSBattleFieldRoom::KGSBattleFieldRoom( const KBattleFieldRoomInfo& kInfo ) : 
m_iJoinUserCountWithReservedUserCount( 0 )
{
	UpdateRoomInfo( kInfo );
}

KGSBattleFieldRoom::~KGSBattleFieldRoom(void)
{
}

void KGSBattleFieldRoom::UpdateRoomInfo( IN const KBattleFieldRoomInfo& kInfo )
{
	m_kRoomInfo = kInfo;
	m_iJoinUserCountWithReservedUserCount = CalcJoinUserCountWithReservedUserCount();
}

int	KGSBattleFieldRoom::GetPartyMemberCountByPartyUID( IN const UidType iPartyUID )
{
	int iPartyMemberCount = 0;

	std::map< UidType, UidType >::const_iterator mit;
	for( mit = m_kRoomInfo.m_mapUnitUIDPartyUID.begin(); mit != m_kRoomInfo.m_mapUnitUIDPartyUID.end(); ++mit )
	{
		const UidType iJoinUserPartyUID = mit->second;

		// 파티 소속이 아니라면 한명만 추가한다.
		if( iPartyUID != iJoinUserPartyUID )
			continue;

		++iPartyMemberCount;
	}

	return iPartyMemberCount;
}

int KGSBattleFieldRoom::CalcJoinUserCountWithReservedUserCount() const
{
	int iNumMemberSum = 0;

	std::map< UidType, UidType >::const_iterator mit;
	for( mit = m_kRoomInfo.m_mapUnitUIDPartyUID.begin(); mit != m_kRoomInfo.m_mapUnitUIDPartyUID.end(); ++mit )
	{
		const UidType iUnitUID = mit->first;
		const UidType iPartyUID = mit->second;

		// 파티 소속이 아니라면 한명만 추가한다.
		if( iPartyUID == 0 )
		{
			++iNumMemberSum;
			continue;
		}

		std::map< UidType, std::set< UidType > >::const_iterator mitPL;
		mitPL = m_kRoomInfo.m_mapPartyList.find( iPartyUID );
		if( mitPL == m_kRoomInfo.m_mapPartyList.end() )
		{
			// 에러로그가 너무 많이찍혀서 로그 레벨을 낮춘다.
			START_LOG( cwarn, L"파티UID값은 있는데 파티 리스트에는 해당 파티 정보가 없다? 일어나서는 안되는 에러!" )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( iPartyUID )
				<< END_LOG;
			continue;
		}

		const std::set< UidType >& setPartyMemberList = mitPL->second;
		if( setPartyMemberList.find( iUnitUID ) == setPartyMemberList.end() )
		{
			// 에러로그가 너무 많이찍혀서 로그 레벨을 낮춘다.
			START_LOG( cwarn, L"파티 리스트에 파티 정보는 있는데 해당 유저가 파티 소속이 아니다? 일어나서는 안되는 에러!" )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( iPartyUID )
				<< BUILD_LOG( setPartyMemberList.size() )
				<< END_LOG;
			continue;
		}

		// 자기 자신을 제외한 인원수를 덧셈한다.
		iNumMemberSum += ( static_cast<int>(mitPL->second.size()) - 1 );
	}

	return iNumMemberSum;
}


#endif SERV_BATTLE_FIELD_SYSTEM
//}}



#include "GameServer.h"
#include "GSAutoPartyManager.h"


//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


ImplementSingleton( KGSAutoPartyManager );


KGSAutoPartyManager::KGSAutoPartyManager() : 
m_iUpdateTick( 0 )
{
}

KGSAutoPartyManager::~KGSAutoPartyManager()
{
}

ImplToStringW( KGSAutoPartyManager )
{
	stm_ << L"-------------- GSAutoPartyManager --------------" << std::endl
		<< TOSTRINGW( m_mapAutoPartyBonusInfo.size() )
		;

	return stm_;
}

void KGSAutoPartyManager::UpdateAutoPartyBonusInfo( IN const int iDungeonMode, IN const int iDungeonID, IN const bool bActivatedBonus )
{
	KAutoPartyBonusInfoKey kKey( iDungeonMode, iDungeonID );
	std::map< KAutoPartyBonusInfoKey, SAutoPartyBonusInfo >::iterator mit;
	mit = m_mapAutoPartyBonusInfo.find( kKey );
	if( mit == m_mapAutoPartyBonusInfo.end() )
	{
		SAutoPartyBonusInfo sAutoPartyBonusInfo;
		sAutoPartyBonusInfo.m_bIsAutoPartyBonus = bActivatedBonus;
		m_mapAutoPartyBonusInfo.insert( std::make_pair( kKey, sAutoPartyBonusInfo ) );
	}
	else
	{
		mit->second.m_bIsAutoPartyBonus = bActivatedBonus;
	}

	// update tick 갱신
	++m_iUpdateTick;
}

bool KGSAutoPartyManager::IsAutoPartyBonus( IN const int iDungeonMode, IN const int iDungeonID ) const
{
	KAutoPartyBonusInfoKey kKey( iDungeonMode, iDungeonID );
	std::map< KAutoPartyBonusInfoKey, SAutoPartyBonusInfo >::const_iterator mit;
	mit = m_mapAutoPartyBonusInfo.find( kKey );
	if( mit == m_mapAutoPartyBonusInfo.end() )
	{
		// 데이터가 아예 없으면 보너스 없음으로 표시!
		return false;
	}

	return mit->second.m_bIsAutoPartyBonus;
}


#endif SERV_BATTLE_FIELD_SYSTEM
//}}



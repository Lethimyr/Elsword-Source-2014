#pragma once
#include <KNCSingleton.h>
#include <ToString.h>
#include "GSWeddingHall.h"


//{{ 2013. 04. 01	 ¿Œø¨ Ω√Ω∫≈€ - ±ËπŒº∫
#ifdef SERV_RELATIONSHIP_SYSTEM


class KWeddingHallListManager
{
	DeclareSingleton( KWeddingHallListManager );
	DeclDump;
	DeclToStringW;

public:
	KWeddingHallListManager(void);
	~KWeddingHallListManager(void);	

	KGSWeddingHallPtr GetWeddingHallInfo( IN const int iWeddingUid );
	bool	IsExist( IN const int iWeddingUid ) const		{ return ( m_mapWeddingHallList.find( iWeddingUid ) != m_mapWeddingHallList.end() ); }

	void	AddWeddingHallInfo( IN const KGSWeddingHall& kHallInfo );

	void	UpdateWeddingHallInfo( IN const KGSWeddingHall& kHallInfo );
	void	UpdateWeddingHallInfo( IN const KWeddingHallInfo& kInfo, IN std::vector< UidType >& vecUnitList );

	void	DeleteWeddingHallInfo( IN const KGSWeddingHall& kHallInfo );
	void	DeleteWeddingHallInfo( IN const int iWeddingUID );

	void	DelRoomInfoByCnServerUID( IN const UidType iCnServerUID );

private:
	std::map< int, KGSWeddingHallPtr >	m_mapWeddingHallList;		// key : BattleFieldID, value : BattleFieldList
};

DefSingletonInline( KWeddingHallListManager );


#endif SERV_RELATIONSHIP_SYSTEM
//}


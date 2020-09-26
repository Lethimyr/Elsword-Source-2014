#pragma once

// class
#define KGSWeddingHallPtr KGSWeddingHall*
#undef KGSWeddingHallPtr
SmartPointer( KGSWeddingHall );


//{{ 2013. 04. 01	 ¿Œø¨ Ω√Ω∫≈€ - ±ËπŒº∫
#ifdef SERV_RELATIONSHIP_SYSTEM


class KGSWeddingHall
{
public:
	KGSWeddingHall( const KWeddingHallInfo& kInfo );
	virtual ~KGSWeddingHall(void);
		
	UidType GetRoomUID() const									{ return m_kHallInfo.m_iRoomUID; }
	int		GetWeddingHallUID() const							{ return m_kHallInfo.m_iWeddingUID; }
	void	GetWeddingHallInfo( KWeddingHallInfo& kInfo ) const				{ kInfo = m_kHallInfo; }
	bool	IsJoinUser( IN const UidType iUnitUID ) const		{ return ( m_setJoinUnitList.find( iUnitUID ) != m_setJoinUnitList.end() ); }
	void	UpdateRoomInfo( IN const KGSWeddingHall& kInfo );
	void	UpdateRoomInfo( IN const KWeddingHallInfo& kInfo, IN std::set< UidType >& setList );
	void	DelRoomInfoByCnServerUID( IN const UidType iCnServerUID );

protected:
	KWeddingHallInfo		m_kHallInfo;
	std::set< UidType >		m_setJoinUnitList;
};

#endif SERV_RELATIONSHIP_SYSTEM
//}
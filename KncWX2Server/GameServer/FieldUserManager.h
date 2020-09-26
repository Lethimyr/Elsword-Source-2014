#pragma once 

#include "FieldUser.h"
#include "ServerPacket.h"

typedef boost::shared_ptr< std::map< UidType, KFieldUserPtr > > FieldUserListPtr;

class KFieldUserManager
{
	enum FIELD_USER_LIST
	{
		PAGE_PER_USER_COUNT = 8,
	};

public:
	KFieldUserManager();
	virtual ~KFieldUserManager();

	//{{ 2009. 3. 12  최육사	덤프
	void			Dump();
	//}}

	void			Init( int nMaxUser );
    void            SetTileInfo( const KTileInfo& kTileInfo )   { m_kTileInfo = kTileInfo; }
    void            SetMapInfo( const KMapInfo& kMapInfo )      { m_kMapInfo = kMapInfo; }
	KFieldUserPtr	GetUser( UidType nCID );
    bool            GetSectorID( UidType nCID, SectorID& iSectorID );
	bool			IsExistUser( UidType nCID )                 { return ( m_mapUserList.find( nCID ) != m_mapUserList.end() ); }
	int             GetNumMaxUser()                             { return m_nMaxUser; }
    int				GetNumMember()                              { return static_cast< int >( m_mapUserList.size() ); }

    bool			AddUser( KFieldUserPtr spFieldUser,
                             const VECTOR3& vPos,
                             std::vector< UidType >& vecNearbyUnitAddedThis,
                             std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis );
	bool			RemoveUser( UidType nCID,
                                std::vector< UidType >& vecNearbyUnitRemovedThis );
    bool            MoveUser( KFieldUserPtr spFieldUser,
                              SectorID iDestSectorID,
                              KFieldUserInfo& kFieldUserInfoOfThis,
                              std::vector< UidType >& vecNearbyUnitAddedThis,
                              std::vector< UidType >& vecNearbyUnitRemovedThis,
                              std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                              std::vector< UidType >& vecNearbyUnitRemovedFromThis );
    bool            MoveUser( UidType iUnitUID,
                              SectorID iDestSectorID,
                              KFieldUserInfo& kFieldUserInfoOfThis,
                              std::vector< UidType >& vecNearbyUnitAddedThis,
                              std::vector< UidType >& vecNearbyUnitRemovedThis,
                              std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                              std::vector< UidType >& vecNearbyUnitRemovedFromThis );
    bool            MoveUser( KFieldUserPtr spFieldUser,
                              const VECTOR3& vPos,
                              KFieldUserInfo& kFieldUserInfoOfThis,
                              std::vector< UidType >& vecNearbyUnitAddedThis,
                              std::vector< UidType >& vecNearbyUnitRemovedThis,
                              std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                              std::vector< UidType >& vecNearbyUnitRemovedFromThis );
    bool            MoveUser( UidType iUnitUID,
                              const VECTOR3& vPos,
                              KFieldUserInfo& kFieldUserInfoOfThis,
                              std::vector< UidType >& vecNearbyUnitAddedThis,
                              std::vector< UidType >& vecNearbyUnitRemovedThis,
                              std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                              std::vector< UidType >& vecNearbyUnitRemovedFromThis );

	bool			EnterField( const KERM_JOIN_FIELD_REQ& kJoinUser,
                                std::vector< UidType >& vecNearbyUnitAddedThis,
                                std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis );
	bool			LeaveField( UidType nCID,
                                std::vector< UidType >& vecNearbyUnitRemovedThis );

	//{{ 2009. 1. 15  최육사	유저 리스트
	bool			AddUserList( KFieldUserPtr spFieldUser );
	bool			RemoveUserList( UidType nCID );
	//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
	bool			GetFieldUserList( UINT& uiViewPage, UINT& uiTotalPage, std::vector< KCommunityUserInfo >& vecUserList, bool bEdView = false );
#else
	bool			GetFieldUserList( UINT& uiViewPage, UINT& uiTotalPage, std::vector< KCommunityUserInfo >& vecUserList );
#endif SERV_ED_MONITORING_IN_GAME
	//}}
	//}}

	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
	bool			SetUserEquippedTitle( IN UidType nCID, IN int iEquippedTitleID );
#else
	bool			SetUserEquippedTitle( IN UidType nCID, IN short sEquippedTitleID );
#endif SERV_TITLE_DATA_SIZE
	//}}	
	bool			SetUserEquipItem( IN UidType nCID, IN std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
	bool			SetFieldOption( IN UidType nCID, IN const KFieldOption& kFieldOption );
	bool            SetParty( IN UidType nCID, IN UidType iPartyUID, IN const std::set< UidType >& setParty, OUT std::vector< KFieldUserInfo >& vecAdded, OUT std::vector< UidType >& vecRemoved );

	//{{ 2009. 4. 9  최육사		친구리스트
	bool			SetFriend( IN UidType nCID, IN const std::set< UidType >& setFriend, OUT std::vector< KFieldUserInfo >& vecAdded, OUT std::vector< UidType >& vecRemoved );
	//}}
	//{{ 2009. 9. 23  최육사	1차전직캐쉬템
	bool			SetFieldUserInfo( IN const KFieldUserInfo& kInfo );
	//}}
	//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	bool			SetPetInfo( IN UidType nCID, IN const std::vector< KPetInfo >& vecPetInfo );
	bool			SetPetInfo( IN UidType nCID, IN const KPetInfo& kPetInfo );
#endif SERV_PET_SYSTEM
	//}}

    bool			UpdateUserPos( UidType nCID, const VECTOR3& UserPos );

	bool			AddUserToSector( SectorID iSectorID, KFieldUserPtr spUser );
	bool			RemoveUserFromSector( SectorID iSectorID, UidType nCID );

	void			GetSectorUnitList( const std::vector< SectorID >& vecSectorList, std::map< UidType, KFieldUserPtr >& mapUnitList );

    SectorsPtr      GetAdjacentSectors( SectorID iSector );

    bool            GetBroadcastDestination( UidType nCID, std::vector< UidType >& vecDestination );

	//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
	void			RefreshUser_ED( std::map< UidType, int >& mapUnit_ED );
#endif SERV_ED_MONITORING_IN_GAME
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	bool			SetRidingPetInfo( IN const UidType iUnitUID, IN const UidType iRidingPetUID, IN const USHORT usRidingPetID );
#endif	// SERV_RIDING_PET_SYSTM

protected:
	KFieldUserPtr	MakeUser( const KFieldUserInfo& kFieldUserInfo, const std::set< UidType >& setPartyList, const std::set< UidType >& setFriendList, const KFieldOption& kFieldOption );
    void            GetAddedSectorUnitList( SectorID iSectorID, std::map< UidType, KFieldUserPtr >& mapUnitList );
    SectorID        GetDestSectorID( const VECTOR3& vPos );

private:
    int         m_nMaxUser;
    KTileInfo   m_kTileInfo;
	KMapInfo    m_kMapInfo;
    std::map< UidType, KFieldUserPtr >                          m_mapUserList;              // 모든유저를 관리하고
	std::vector< KFieldUserPtr >								m_vecUserList;				// 커뮤니티UI에 리스트로 뿌려주기위해
    std::map< SectorID, FieldUserListPtr >                      m_mapSectorUser;            // 섹터별 유저 관리
    std::map< SectorID, SectorsPtr >                            m_mapAdjacentSector;        // 섹터에 대한 인접 섹터들
    //std::map< SectorID, SectorsPtr >                            m_mapSectorTraverseOrder;   // 인접 섹터들의 검색 순서 -- 안씀
};



#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include <KNCSingleton.h>
#include <ToString.h>

struct KPVPUser
{
	KCommunityUserInfo	m_kUserInfo;
    UidType				m_nGSUID;

	KPVPUser() {}
	KPVPUser( const KCommunityUserInfo& kInfo, UidType nGSUID )
	{
		m_kUserInfo = kInfo;
		m_nGSUID = nGSUID;
	}
};

typedef boost::shared_ptr<KPVPUser> KPVPUserPtr;

class KPVPLobbyManager
{
	DeclareSingleton( KPVPLobbyManager );
	DeclDump;
	DeclToStringW;

	enum
	{
		PAGE_PER_PVP_USER = 8,
	};

public:
	KPVPLobbyManager(void);
	~KPVPLobbyManager(void);

	bool AddPVPUser( IN KCommunityUserInfo& kInfo, IN UidType nGSUID );
	bool RemovePVPUser( IN UidType iUnitUID );
	bool UpdatePVPUserInfo( IN UidType iUnitUID, IN char cState, IN int iRoomListID, IN UidType iRoomUID );
	bool GetPVPUserList( IN UINT& uiViewPage, IN UINT& uiTotalPage, OUT std::vector< KCommunityUserInfo >& vecUserList );
	void RemovePVPUserDisconnectGS( IN UidType nGSUID );

private:
	KPVPUserPtr Get( IN UidType iUnitUID );
	bool IsExist( IN UidType iUnitUID ) { return ( m_mapPVPUserList.find( iUnitUID ) != m_mapPVPUserList.end() ); }
	
private:
	std::vector< KPVPUserPtr >		 m_vecPVPUserList;
	std::map< UidType, KPVPUserPtr > m_mapPVPUserList;
};

DefSingletonInline( KPVPLobbyManager );




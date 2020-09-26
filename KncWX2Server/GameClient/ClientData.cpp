#include "ClientData.h"

KClientData::KClientData()
{
}

KClientData::~KClientData()
{
}

void KClientData::SetUserInfo( UidType iUserUID, std::wstring wstrID, std::wstring wstrName )
{
    m_nUserUID = iUserUID;
    m_wstrID = wstrID;
    m_wstrName = wstrName;
}

void KClientData::SetNumUnitSlot( int i )
{
    m_nUnitSlot = i;
}

void KClientData::SetUnitInfo( const KUnitInfo& kInfo )
{
    m_kUnitInfo = kInfo;
}

void KClientData::SetUnitList( const std::vector< KUnitInfo >& vecUnitInfo )
{
    m_vecUnitInfo = vecUnitInfo;
}

void KClientData::AddUnit( const KUnitInfo& kInfo )
{
    m_vecUnitInfo.push_back( kInfo );
}

void KClientData::DeleteUnit( UidType nCID )
{
    std::vector< KUnitInfo >::iterator vit = m_vecUnitInfo.begin();
    while( vit != m_vecUnitInfo.end() )
    {
        if( vit->m_nUnitUID == nCID )
        {
            vit = m_vecUnitInfo.erase( vit );
        }

        vit++;
    }
}

bool KClientData::GetFirstUnitInfo( KUnitInfo& kUnitInfo )
{
	if( m_vecUnitInfo.empty() == true )
		return false;

	for( int i = 0; i < (int)m_vecUnitInfo.size(); ++i )
	{
		kUnitInfo = m_vecUnitInfo[i];
		return true;
	}

	return false;
}

bool KClientData::GetUnitInfo( UidType nCID, KUnitInfo& kUnitInfo )
{
	if( m_vecUnitInfo.empty() == true )
		return false;

	for( int i = 0; i < (int)m_vecUnitInfo.size(); ++i )
	{
		if( nCID == m_vecUnitInfo[i].m_nUnitUID )
		{
			kUnitInfo = m_vecUnitInfo[i];
			return true;
		}
	}

	return false;
}

void KClientData::GetRoomList( std::vector< KRoomInfo >& vecRoomList )
{
    vecRoomList.clear();
    vecRoomList = m_vecRoomList;
}

void KClientData::SetRoomList( const std::vector< KRoomInfo >& vecRoomList )
{
    m_vecRoomList.clear();
    m_vecRoomList = vecRoomList;
}

void KClientData::GetRoomInfo( KRoomInfo& kInfo )
{
    kInfo = m_kRoomInfo;
}

void KClientData::SetRoomInfo( const KRoomInfo& kInfo )
{
    m_kRoomInfo = kInfo;
}

void KClientData::GetRoomSlotInfo( std::vector< KRoomSlotInfo >& vecRoomSlotInfo )
{
    vecRoomSlotInfo.clear();
    vecRoomSlotInfo = m_vecRoomSlotInfo;
}

void KClientData::SetRoomSlotInfo( const std::vector< KRoomSlotInfo >& vecRoomSlotInfo )
{
    m_vecRoomSlotInfo.clear();
    m_vecRoomSlotInfo = vecRoomSlotInfo;
}

void KClientData::GetSquareList( std::vector< KSquareInfo >& vecSquareList )
{
	vecSquareList.clear();
	vecSquareList = m_vecSquareList;
}

void KClientData::SetSquareList( const std::vector< KSquareInfo >& vecSquareList )
{
	m_vecSquareList.clear();
	m_vecSquareList = vecSquareList;
}

void KClientData::JoinSquareUnit( KSquareUserInfo *pKSquareUserInfo )
{
	if( GetCID() == pKSquareUserInfo->m_iUnitUID )
		return;

	m_vecSquareUnitList.push_back( pKSquareUserInfo->m_iUnitUID );
}

void KClientData::GetSquareUnitList( std::vector< UidType >& vecSquareUnitList )
{	
	vecSquareUnitList.clear();
	vecSquareUnitList = m_vecSquareUnitList;
}

void KClientData::DelSquareUnit( UidType delUnitUID )
{
	std::vector< UidType >::iterator it = m_vecSquareUnitList.begin();

	while( it != m_vecSquareUnitList.end() )
	{
		if( *it == delUnitUID )
		{
			m_vecSquareUnitList.erase( it );
			return;
		}
		else
		{
			it++;
		}
	}
}
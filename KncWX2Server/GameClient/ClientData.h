#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

class KClientData
{
public:
    KClientData();
    ~KClientData();

    void SetUserInfo( UidType iUserUID, std::wstring wstrID, std::wstring wstrName );
    void SetNumUnitSlot( int i );
    void SetUnitInfo( const KUnitInfo& kInfo );
    void SetUnitList( const std::vector< KUnitInfo >& vecUnitInfo );
    void AddUnit( const KUnitInfo& kInfo );
    void DeleteUnit( UidType nCID );

	bool GetFirstUnitInfo( KUnitInfo& kUnitInfo );
	bool GetUnitInfo( UidType nCID, KUnitInfo& kUnitInfo );

    std::wstring GetID()        { return m_wstrID; }
	std::wstring GetNickName()	{ return m_wstrName; }

    void GetRoomList( std::vector< KRoomInfo >& vecRoomList );
    void SetRoomList( const std::vector< KRoomInfo >& vecRoomList );
    void GetRoomInfo( KRoomInfo& kInfo );
    void SetRoomInfo( const KRoomInfo& kInfo );
    void GetRoomSlotInfo( std::vector< KRoomSlotInfo >& vecRoomSlotInfo );
    void SetRoomSlotInfo( const std::vector< KRoomSlotInfo >& vecRoomSlotInfo );

	void GetSquareList( std::vector< KSquareInfo >& vecSquareList );
	void SetSquareList( const std::vector< KSquareInfo >& vecSquareList );
	void JoinSquareUnit( KSquareUserInfo *pKSquareUserInfo );
	void GetSquareUnitList( std::vector< UidType >& vecSquareUnitList );
	void DelSquareUnit( UidType delUnitUID );
	void ClearSquareUnitList(){ m_vecSquareUnitList.clear(); }
	
	void SetQuestTF( bool val ){ m_questTF = val; }
	bool GetQuestTF(){ return m_questTF; }

	int  GetMapID()				{ return m_nMapID; }
	void SetMapID( int nMapID )	{ m_nMapID = nMapID; }
	int	 GetTeamNum()			{ return m_nTeamNum; }
	void SetTeamNum( int nTeamNum ){ m_nTeamNum = nTeamNum; }

	UidType GetCID()			{ return m_kUnitInfo.m_nUnitUID; }

protected:
    UidType                         m_nUserUID;
    std::wstring                    m_wstrID;
    std::wstring                    m_wstrName;
    int                             m_iCash;
    int                             m_nUnitSlot;
	int								m_nMapID;
	int								m_nTeamNum;
    KUnitInfo                       m_kUnitInfo;
    std::vector< KUnitInfo >        m_vecUnitInfo;

    std::vector< KRoomInfo >        m_vecRoomList;
    KRoomInfo                       m_kRoomInfo;
    std::vector< KRoomSlotInfo >    m_vecRoomSlotInfo;

	std::vector< KSquareInfo >		m_vecSquareList;
	std::vector< UidType >			m_vecSquareUnitList;
		
	bool							m_questTF;
};



#pragma once

#include "GSProxy.h"
#include <ObjectPool/BlockAllocator.h>
#include "FSM/support_FSM.h"
#include "ActionDecider.h"
#include <boost/timer.hpp>
#include "ClientData.h"
#include "TRUser.h"
#include <d3dx9math.h>
#include <list>
#include "KRobotPatternManager.h"

#define PACKET_WAIT_TIME 30.0f
#define IMPL_ON_ACTION_FUNC( id ) void CLASS_TYPE##::ON_##id()

SmartPointer( KncRobot );


struct KWaitingEvent
{
    u_short         m_usEvent;
    boost::timer    m_kTimer;
    float           m_fExpirationTime;
};

class KncRobot : public KGSProxy, public BlockAllocator< KncRobot, 3000 >
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareFSM_MT;

public:
#   undef _ENUM
#   define _ENUM( id ) id,
    enum eACTION
    {   
#   include "RobotAction_def.h"
    };

    enum eTIMER
    {
        TM_ACTION_GAP,
		TM_SQUARE_SYNC,
		TM_FIELD_SYNC,
		TM_DISCONNECT_TIME,

        TM_NUM
    };

	enum ROOM_LIST_TYPE
	{
		RLT_NONE = 0,
		RLT_PVP,
		RLT_FREE_PVP,
		RLT_SQUARE,
		RLT_PARTY,

		RLT_END,
	};

	enum SYNC_UNIT_STATE
	{			
		SUS_W		= 0,
		SUS_L		= 1,
		SUS_R		= 2,
		SUS_DL		= 4,
		SUS_DR		= 8,
		SUS_U		= 16,
		SUS_D		= 32,
		SUS_DU		= 64,
		SUS_DD		= 128,
		SUS_DUMY	= 255,
	};

	class KTimerManager
	{
	public:
		KTimerManager()	: m_fAddTime( 0.0f ) {} 
		void restart()						{ m_kTimer.restart(); m_fAddTime = 0.0f; }
		double elapsed() const				{ return ( m_kTimer.elapsed() + m_fAddTime ); }

		void SetAddTime( double fTime )		{ m_fAddTime = fTime; }
		double GetAddTime()					{ return m_fAddTime; }

	private:
		double		 m_fAddTime;
		boost::timer m_kTimer;
	};

    KncRobot();
    virtual ~KncRobot();

    void	SetIndex( int i );
	int		GetIndex();
    void HandleExpiredEvent();
    bool IsWaiting();
    static void AddGSAddress( const KNetAddress& kAddress );
    static const char* GetActionStr( int iAction );

    template < typename T >
    bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false )
    {
        return KSession::SendPacket( PI_GS_USER, 0, NULL, usEventID, data, bLogging, bCompress );
    }

    bool SendID( unsigned short usEventID )
    {
        return KSession::SendID( PI_GS_USER, 0, NULL, usEventID );
    }

	bool SendIDToChServer( unsigned short usEventID )
	{
		return KSession::SendID( PI_CHANNEL_USER, 0, NULL, usEventID );
	}

    static void SetConnectCapacity( float fControlTime, int nCapacity ) { ms_fConnectControlTime = fControlTime; ms_iConnectCapacity = nCapacity; }

    // override
    virtual void OnDestroy();
    virtual void Tick();

protected:
    void AddWaitingEvent( u_short usEventID, float fExpirationTime );
    void RemoveWaitineEvent( u_short usEventID );
    bool GetRecvEvent( KEventPtr& spEvent );
    void ProcessRecvEvent( const KEventPtr& spEvent );

	// channel server
	DECL_ON_FUNC( ECH_GET_CHANNEL_LIST_NOT );

	// connect
    DECL_ON_FUNC( EGS_CONNECT_ACK );
    DECL_ON_FUNC( EGS_NEW_USER_JOIN_ACK );
    DECL_ON_FUNC( EGS_VERIFY_ACCOUNT_ACK );
    DECL_ON_FUNC_NOPARAM( ENX_USER_LOGIN_NOT );

	// server
   _DECL_ON_FUNC( EGS_GET_SERVER_SET_DATA_ACK, KEGS_GET_SERVER_SET_DATA_ACK );
    DECL_ON_FUNC( EGS_STATE_CHANGE_SERVER_SELECT_ACK );
    DECL_ON_FUNC( EGS_SELECT_SERVER_SET_ACK );

	// unit
    DECL_ON_FUNC( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK );   
	DECL_ON_FUNC( EGS_SELECT_UNIT_ACK );
    DECL_ON_FUNC( EGS_CREATE_UNIT_ACK );
    DECL_ON_FUNC( EGS_DELETE_UNIT_ACK );

	// field
	DECL_ON_FUNC( EGS_OPTION_UPDATE_ACK );
	DECL_ON_FUNC( EGS_STATE_CHANGE_FIELD_ACK );
	DECL_ON_FUNC( EGS_FIELD_LOADING_COMPLETE_ACK );

    // quest
   _DECL_ON_FUNC( EGS_ENTER_NPC_HOUSE_ACK, KPacketOK );
   _DECL_ON_FUNC( EGS_TALK_WITH_NPC_ACK, KEGS_TALK_WITH_NPC_ACK );
   _DECL_ON_FUNC( EGS_NEW_QUEST_ACK, KEGS_NEW_QUEST_ACK );
   _DECL_ON_FUNC( EGS_NEW_QUEST_NOT, KEGS_NEW_QUEST_NOT );
   _DECL_ON_FUNC( EGS_GIVE_UP_QUEST_ACK, KEGS_GIVE_UP_QUEST_ACK );

    // shop
   _DECL_ON_FUNC( EGS_ENTER_EQUIP_SHOP_ACK, KPacketOK );
   
    // square
   _DECL_ON_FUNC( EGS_SQUARE_LIST_ACK, KEGS_SQUARE_LIST_ACK );
   _DECL_ON_FUNC( EGS_JOIN_SQUARE_ACK, KEGS_JOIN_SQUARE_ACK );
   _DECL_ON_FUNC( EGS_JOIN_SQUARE_NOT, KEGS_JOIN_SQUARE_NOT );
    //DECL_ON_FUNC( EGS_SQUARE_CHAT_ACK );
	DECL_ON_FUNC( EGS_LEAVE_SQUARE_ACK );
   _DECL_ON_FUNC( EGS_LEAVE_SQUARE_NOT, KEGS_LEAVE_SQUARE_NOT );

	// room
    DECL_ON_FUNC( EGS_ROOM_LIST_ACK );
    DECL_ON_FUNC( EGS_CREATE_ROOM_ACK );
	DECL_ON_FUNC( EGS_CREATE_TC_ROOM_ACK );
    DECL_ON_FUNC( EGS_JOIN_ROOM_ACK );
    DECL_ON_FUNC( EGS_QUICK_JOIN_ACK );
   _DECL_ON_FUNC( EGS_LEAVE_ROOM_ACK, KPacketOK );
   _DECL_ON_FUNC( EGS_CHAT_ACK, KPacketOK );
    DECL_ON_FUNC( EGS_CHANGE_TEAM_ACK );
	DECL_ON_FUNC( EGS_CHANGE_DUNGEON_DIFFICULTY_ACK );

	// party
	DECL_ON_FUNC( EGS_CREATE_PARTY_ACK );
	DECL_ON_FUNC( EGS_PARTY_LIST_ACK );
	DECL_ON_FUNC( EGS_JOIN_PARTY_ACK );
	DECL_ON_FUNC( EGS_JOIN_PARTY_INVITE_NOT );
	DECL_ON_FUNC( EGS_JOIN_PARTY_NOT );
	DECL_ON_FUNC( EGS_LEAVE_PARTY_ACK );

	// etc
   _DECL_ON_FUNC( EGS_SEARCH_UNIT_ACK, KEGS_SEARCH_UNIT_ACK );				// 추가
	DECL_ON_FUNC( EGS_NOTIFY_MSG_NOT );
	DECL_ON_FUNC( EGS_MEGAPHONE_MESSAGE_NOT );

	// guild
	DECL_ON_FUNC( EGS_INVITE_GUILD_NOT );


    void DoAction( int iAction );
#   undef _ENUM
#   define _ENUM( id ) void ON_##id();
#   include "RobotAction_def.h"

	//{{ 2008. 12. 8  최육사	필드
private:
	bool FieldSync();
	int DecisionMoveField();
	bool ConnectTRServer( UidType iUnitUID );

	int										m_iMapID;
	int										m_iDestMapID;
	bool									m_bFieldSync;

	D3DXVECTOR3								m_CharCurPos;

	std::list< KRobotPattern >				m_listFieldPattern;
	//}}

public:
    static KActionDecider                   ms_kActionDecider;
    static const char*                      ms_szAction[];
    static std::string                      m_strNameHead;

protected:
    int                                     m_iIndex;
	int										m_iDifficulty;
    std::vector< KWaitingEvent >            m_vecWaitingEvent;
    boost::timer                            m_kTimer[TM_NUM];
    float                                   m_fActionGap;
    KClientData                             m_kClientData;
    static std::vector< KNetAddress >       ms_vecGSAddress;

	int										m_iRoomType;

    static boost::timer                     ms_kConnectControlTimer;
    static float                            ms_fConnectControlTime;
    static int                              ms_iConnectCapacity;
    static int                              ms_iRemainCapacity;

	//{{ 2008. 12. 26  최육사	TRUser
	std::string								m_strIP;
	KTRUserPtr								m_spTRUser;
	//}}

	//{{ 2009. 3. 10  최육사	마지막위치
	bool									m_bIsRight;
	VECTOR3									m_kCurFieldPos;	
	KTimerManager							m_tStandTimer;
	//}}

};




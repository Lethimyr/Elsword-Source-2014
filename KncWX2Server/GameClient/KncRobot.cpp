#include "KncRobot.h"
#include "RBFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
#include "X2Data/XSLMapData.h"
#include "X2Data/XSLSquareGame.h"
#include "X2Data/XSLDungeon.h"
//#include "KncRobotThreadManager.h"
#include "KncRobotManager.h"
#include "Lib/X2LocationManager.h"

//#ifndef KNC_DONT_USE_IOCP
#include "Socket/IOCP.h"
//#endif

#define CLASS_TYPE KncRobot

KActionDecider              KncRobot::ms_kActionDecider( A_SENTINEL );
std::vector< KNetAddress >  KncRobot::ms_vecGSAddress;
std::string                 KncRobot::m_strNameHead = "RTest";

boost::timer                KncRobot::ms_kConnectControlTimer;
float                       KncRobot::ms_fConnectControlTime = 0.12f;
int                         KncRobot::ms_iConnectCapacity = 2;
int                         KncRobot::ms_iRemainCapacity = 2;

#undef _ENUM
#define _ENUM( id ) #id,
const char*                 KncRobot::ms_szAction[] =
{
#include "RobotAction_def.h"
};

NiImplementRTTI( KncRobot, KGSProxy );

ImplToStringW( KncRobot )
{
    return START_TOSTRING_PARENTW( KGSProxy );
}

KncRobot::KncRobot()
{
	m_iRoomType  = -1;
    m_fActionGap = 5.0f * ( float )( ( float )rand() / ( float )RAND_MAX );

	m_iMapID	 = CXSLMapData::MI_NONE;
	m_iDestMapID = CXSLMapData::MI_NONE;
	m_bFieldSync = false;
	m_tStandTimer.SetAddTime( 5000.f );
}

KncRobot::~KncRobot()
{
}

void KncRobot::SetIndex( int i )
{
    m_iIndex = i;
}

int KncRobot::GetIndex()
{
	return m_iIndex;
}

void KncRobot::HandleExpiredEvent()
{
    std::vector< KWaitingEvent >::iterator vit;
    for( vit = m_vecWaitingEvent.begin(); vit != m_vecWaitingEvent.end(); vit++ )
    {
        if( vit->m_kTimer.elapsed() > vit->m_fExpirationTime )
        {
            OnDestroy();

			START_LOG( cerr, L"나 지워진다. 안돼~~" )
				<< BUILD_LOG( vit->m_usEvent )
				<< BUILD_LOG( KEvent::GetIDStr(vit->m_usEvent) )
				<< END_LOG;

            return;
        }
    }
}

bool KncRobot::IsWaiting()
{
    if( !m_vecWaitingEvent.empty() )
    {
        HandleExpiredEvent();
        return !m_vecWaitingEvent.empty();
    }

    return m_kTimer[TM_ACTION_GAP].elapsed() < m_fActionGap;
}

void KncRobot::AddGSAddress( const KNetAddress& kAddress )
{
    ms_vecGSAddress.push_back( kAddress );
}

const char* KncRobot::GetActionStr( int iAction )
{
    if( iAction < 0 || iAction > A_SENTINEL )
    {
        START_LOG( cerr, L"이런 액션 없다." )
            << BUILD_LOG( iAction )
            << BUILD_LOG( A_SENTINEL )
            << END_LOG;

        return ms_szAction[A_SENTINEL];
    }

    return ms_szAction[iAction];
}

void KncRobot::OnDestroy()
{
	KGSProxy::OnDestroy();

	//신규로 바뀐 FSM은 바로 EXIT로 갈수있기때문에 따로 처리가 필요없음.
    //switch( GetStateID() )
    //{
    //case KGSFSM::S_CHANNEL:
    //    StateTransition( KGSFSM::I_TO_VILLAGE_MAP );
    //    break;
    //case KGSFSM::S_ROOM:
    //    StateTransition( KGSFSM::I_TO_CHANNEL );
    //    StateTransition( KGSFSM::I_TO_VILLAGE_MAP );
    //    break;
    //}

    StateTransition( KRBFSM::I_TO_EXIT );

    m_vecWaitingEvent.empty();

	SiKncRobotManager()->DeleteByRobot( GetIndex() );

	//if( m_nSPIndex != 0 )
	//{
	//	START_LOG( clog, L"SA 삭제. Name : " << m_strName << L", SPI : " << m_nSPIndex );
	//	KncSecurity::GetSADB().Delete( m_nSPIndex );
	//	m_nSPIndex = 0;
	//}
}

void KncRobot::Tick()
{
    KGSProxy::Tick();

    // 수신 이벤트 처리
    KEventPtr spEvent;
    while( GetRecvEvent( spEvent ) )
    {
        if( !spEvent )
        {
            START_LOG( cerr, L"이벤트 포인터가 NULL임." )
                << END_LOG;
        }

        ProcessRecvEvent( spEvent );
    }

	// TRUser tick을 돈다 하트 비트 보낼려고~
	if( m_spTRUser != NULL )
	{
		m_spTRUser->Tick();
	}

	if( m_tStandTimer.elapsed() < 15.f )
		return;

	// 필드 싱크 - 필드 패턴이 모두 소모될때 까지는 DoAction을 하지 않는다.
	if( FieldSync() )
		return;

    // 시간이 적당히 흐를 때까지 기다린다.
    if( IsWaiting() )
    {
        return;
    }

	// 접속 통제 변수 리필.
    if( ( float )( ms_kConnectControlTimer.elapsed() ) >= ms_fConnectControlTime )
    {
        //START_LOG( cerr, L"통제 변수" )
        //    << BUILD_LOG( ms_iConnectCapacity );
        ms_kConnectControlTimer.restart();
        ms_iRemainCapacity = ms_iConnectCapacity;
    }

    // 뭔가 행동을 해야겠지..
    DoAction( ms_kActionDecider.MakeDecision( GetStateID() ) );
}

void KncRobot::AddWaitingEvent( u_short usEventID, float fExpirationTime )
{
    KWaitingEvent kWEvent;
    kWEvent.m_usEvent = usEventID;
    kWEvent.m_kTimer.restart();
    kWEvent.m_fExpirationTime = fExpirationTime;

    m_vecWaitingEvent.push_back( kWEvent );
}

void KncRobot::RemoveWaitineEvent( u_short usEventID )
{
    std::vector< KWaitingEvent >::iterator vit;
    for( vit = m_vecWaitingEvent.begin(); vit != m_vecWaitingEvent.end(); vit++ )
    {
        if( vit->m_usEvent == usEventID )
        {
            m_vecWaitingEvent.erase( vit );
            return;
        }
    }
}

bool KncRobot::GetRecvEvent( KEventPtr& spEvent )
{
    KLocker lock( m_csRecvMsg );

    if( m_queRecvMsg.empty() )
    {
        return false;
    }

    spEvent = m_queRecvMsg.front();
    m_queRecvMsg.pop();

    return true;
}

void KncRobot::ProcessRecvEvent( const KEventPtr& spEvent_ )
{
    if( !spEvent_ )
    {
        START_LOG( cerr, L"처리하려는 이벤트가 NULL임." )
            << END_LOG;

        return;
    }

    RemoveWaitineEvent( spEvent_->m_usEventID );

    switch( spEvent_->m_usEventID )
    {
		CASE( ECH_GET_CHANNEL_LIST_NOT );
        CASE( EGS_CONNECT_ACK );
        CASE( EGS_NEW_USER_JOIN_ACK );
        CASE( EGS_VERIFY_ACCOUNT_ACK );
        CASE_NOPARAM( ENX_USER_LOGIN_NOT );
       _CASE( EGS_GET_SERVER_SET_DATA_ACK, KEGS_GET_SERVER_SET_DATA_ACK );
        CASE( EGS_STATE_CHANGE_SERVER_SELECT_ACK );
        CASE( EGS_SELECT_SERVER_SET_ACK );
        CASE( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK );       
        CASE( EGS_SELECT_UNIT_ACK );
        CASE( EGS_CREATE_UNIT_ACK );
        CASE( EGS_DELETE_UNIT_ACK );
		CASE( EGS_OPTION_UPDATE_ACK );
		CASE( EGS_STATE_CHANGE_FIELD_ACK );
		CASE( EGS_FIELD_LOADING_COMPLETE_ACK );
	   _CASE( EGS_ENTER_NPC_HOUSE_ACK, KPacketOK );
	   _CASE( EGS_TALK_WITH_NPC_ACK, KEGS_TALK_WITH_NPC_ACK );
	   _CASE( EGS_NEW_QUEST_ACK, KEGS_NEW_QUEST_ACK );
	   _CASE( EGS_NEW_QUEST_NOT, KEGS_NEW_QUEST_NOT );
	   _CASE( EGS_GIVE_UP_QUEST_ACK, KEGS_GIVE_UP_QUEST_ACK );       
	   _CASE( EGS_SQUARE_LIST_ACK, KEGS_SQUARE_LIST_ACK );
	   _CASE( EGS_JOIN_SQUARE_ACK, KEGS_JOIN_SQUARE_ACK );
	   _CASE( EGS_JOIN_SQUARE_NOT, KEGS_JOIN_SQUARE_NOT );
	    //CASE( EGS_SQUARE_CHAT_ACK );
	    CASE( EGS_LEAVE_SQUARE_ACK );
	   _CASE( EGS_LEAVE_SQUARE_NOT, KEGS_LEAVE_SQUARE_NOT );
        CASE( EGS_ROOM_LIST_ACK );
		CASE( EGS_CREATE_ROOM_ACK );
		CASE( EGS_CREATE_TC_ROOM_ACK );
        CASE( EGS_JOIN_ROOM_ACK );
        CASE( EGS_QUICK_JOIN_ACK );
       _CASE( EGS_LEAVE_ROOM_ACK, KPacketOK );
       _CASE( EGS_CHAT_ACK, KPacketOK );
		CASE( EGS_CHANGE_TEAM_ACK );
		CASE( EGS_CHANGE_DUNGEON_DIFFICULTY_ACK );

		CASE( EGS_CREATE_PARTY_ACK );
		CASE( EGS_PARTY_LIST_ACK );
		CASE( EGS_JOIN_PARTY_ACK );
		CASE( EGS_JOIN_PARTY_INVITE_NOT );
		CASE( EGS_JOIN_PARTY_NOT );
		CASE( EGS_LEAVE_PARTY_ACK );

	   _CASE( EGS_SEARCH_UNIT_ACK, KEGS_SEARCH_UNIT_ACK );

		CASE( EGS_NOTIFY_MSG_NOT );
		CASE( EGS_MEGAPHONE_MESSAGE_NOT );

		CASE( EGS_INVITE_GUILD_NOT );
    }
}

IMPL_ON_FUNC( ECH_GET_CHANNEL_LIST_NOT )
{
	VERIFY_STATE( ( 1, KRBFSM::S_CHANNEL_SELECT ) );	
		
	//StateTransition( KRBFSM::I_TO_INIT );
}

IMPL_ON_FUNC( EGS_CONNECT_ACK )
{
    //VERIFY_STATE( ( 1, KRBFSM::S_EXIT ) );
	VERIFY_STATE( ( 1, KRBFSM::S_CHANNEL_SELECT ) );

    if( kPacket_.m_iOK != NetError::NET_OK )
    {
        OnDestroy();

		START_LOG( cerr, L"나 지워진다. 안돼~~" )
			//<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

        return;
    }

    StateTransition( KRBFSM::I_TO_INIT );
}

IMPL_ON_FUNC( EGS_NEW_USER_JOIN_ACK )
{
    VERIFY_STATE( ( 1, KRBFSM::S_INIT ) );

}

IMPL_ON_FUNC( EGS_VERIFY_ACCOUNT_ACK )
{
    VERIFY_STATE( ( 1, KRBFSM::S_INIT ) );

    if( kPacket_.m_iOK != NetError::NET_OK )
    {
        OnDestroy();

		START_LOG( cerr, L"나 지워진다. 안돼~~" )
			//<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_nOK ) )
			<< END_LOG;

        return;
    }

    m_kClientData.SetUserInfo( kPacket_.m_kAccountInfo.m_nUserUID, kPacket_.m_kAccountInfo.m_wstrID, kPacket_.m_kAccountInfo.m_wstrName );
    StateTransition( KRBFSM::I_TO_UID_UPDATED );
}

IMPL_ON_FUNC_NOPARAM( ENX_USER_LOGIN_NOT )
{
    VERIFY_STATE( ( 1, KRBFSM::S_UID_UPDATED ) );

    StateTransition( KRBFSM::I_TO_LOGINED );
    SendID( EGS_GET_SERVER_SET_DATA_REQ );
    AddWaitingEvent( EGS_GET_SERVER_SET_DATA_ACK, PACKET_WAIT_TIME );
}

_IMPL_ON_FUNC( EGS_GET_SERVER_SET_DATA_ACK, KEGS_GET_SERVER_SET_DATA_ACK )
{
    VERIFY_STATE( ( 1, KRBFSM::S_LOGINED ) );

    SendID( EGS_STATE_CHANGE_SERVER_SELECT_REQ );
    AddWaitingEvent( EGS_STATE_CHANGE_SERVER_SELECT_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_FUNC( EGS_STATE_CHANGE_SERVER_SELECT_ACK )
{
	VERIFY_STATE( ( 2, KRBFSM::S_LOGINED, KRBFSM::S_FIELD_MAP ) );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
		if( GetStateID() == KRBFSM::S_LOGINED )
		{
			StateTransition( KRBFSM::I_TO_SERVER_SELECT );
		}
		else
		{
			StateTransition( KRBFSM::I_TO_UNIT_SELECT );
		}

		//////////////////////////////////////////////////////////////////////////
		// 필드에서 캐릭터 선택창으로 온거라면 FieldUID를 초기화!
		m_bFieldSync = false;
		//////////////////////////////////////////////////////////////////////////
    }
}

IMPL_ON_FUNC( EGS_SELECT_SERVER_SET_ACK )
{
    VERIFY_STATE( ( 1, KRBFSM::S_SERVER_SELECT ) );

    SendID( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ );
    AddWaitingEvent( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_FUNC( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_SERVER_SELECT ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kClientData.SetNumUnitSlot( kPacket_.m_nUnitSlot );
		m_kClientData.SetUnitList( kPacket_.m_vecUnitInfo );

		// 필드 옵션 업데이트
		KEGS_OPTION_UPDATE_REQ kPacketReq;
		kPacketReq.m_kFieldOption.m_sLimitOption = SiKncRobotManager()->GetLimitOption();
		SendPacket( EGS_OPTION_UPDATE_REQ, kPacketReq );
		AddWaitingEvent( EGS_OPTION_UPDATE_ACK, PACKET_WAIT_TIME );
	}
}

IMPL_ON_FUNC( EGS_OPTION_UPDATE_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_SERVER_SELECT ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 유닛 선택 상태로 이동
		StateTransition( KRBFSM::I_TO_UNIT_SELECT );
	}
	else
	{
		START_LOG( cerr, L"필드 옵션 업데이트 실패!" )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_SELECT_UNIT_ACK )
{
    VERIFY_STATE( ( 1, KRBFSM::S_UNIT_SELECT ) );//KGSFSM::S_UNIT_SELECT ) );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
		//kPacket_.m_kUnitInfo.m_nMapID = SiKRobotPatternManager()->GetRandomMapID();

		// 캐릭터 정보 업데이트
        m_kClientData.SetUnitInfo( kPacket_.m_kUnitInfo );

		//////////////////////////////////////////////////////////////////////////
		// TRServer 접속
		ConnectTRServer( m_kClientData.GetCID() );
		//UpdateToTRServerCharUID();
		//////////////////////////////////////////////////////////////////////////

        // 마을로 이동
        KEGS_STATE_CHANGE_FIELD_REQ kPacket;
		kPacket.m_iMapID = kPacket_.m_kUnitInfo.m_nMapID; // 마지막에 이동했던 마을로..
		m_iMapID		 = kPacket_.m_kUnitInfo.m_nMapID;

        SendPacket( EGS_STATE_CHANGE_FIELD_REQ, kPacket );
        AddWaitingEvent( EGS_STATE_CHANGE_FIELD_ACK, PACKET_WAIT_TIME );
    }
	else
	{
		START_LOG( cerr, L"캐릭터 선택 실패!?" )
			<< BUILD_LOG( m_kClientData.GetCID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_CREATE_UNIT_ACK )
{
    VERIFY_STATE( ( 1, KRBFSM::S_UNIT_SELECT ) );//VERIFY_STATE( ( 1, KGSFSM::S_UNIT_SELECT ) );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        m_kClientData.AddUnit( kPacket_.m_kUnitInfo );
    }
}

IMPL_ON_FUNC( EGS_DELETE_UNIT_ACK )
{
    VERIFY_STATE( ( 1, KRBFSM::S_UNIT_SELECT ) );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        m_kClientData.DeleteUnit( kPacket_.m_iUnitUID );
    }
}

IMPL_ON_FUNC( EGS_STATE_CHANGE_FIELD_ACK )
{
	VERIFY_STATE( ( 2, KRBFSM::S_UNIT_SELECT, KRBFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//마을로 이동
		KEGS_FIELD_LOADING_COMPLETE_REQ kPacket;

		if( !SiCX2LocationManager()->GetRandomPositionInField( m_iMapID, m_CharCurPos ) )
		{
			START_LOG( cerr, L"필드 처음 진입하려는데 StartPos정보가 이상하다!" )
				<< END_LOG;
			return;
		}

		kPacket.m_vPos.x = m_CharCurPos.x;
		kPacket.m_vPos.y = m_CharCurPos.y;
		kPacket.m_vPos.z = m_CharCurPos.z;

		SendPacket( EGS_FIELD_LOADING_COMPLETE_REQ, kPacket );
		AddWaitingEvent( EGS_FIELD_LOADING_COMPLETE_ACK, PACKET_WAIT_TIME );
	}
	else
	{
		START_LOG( cerr, L"필드 이동 요청 실패!" )
			<< BUILD_LOG( m_kClientData.GetCID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_FIELD_LOADING_COMPLETE_ACK )
{
	VERIFY_STATE( ( 2, KRBFSM::S_UNIT_SELECT, KRBFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		StateTransition( KRBFSM::I_TO_FIELD_MAP );

		m_bFieldSync = true;

		//////////////////////////////////////////////////////////////////////////
		// 움직일꺼냐?
		bool bFieldMove = ( ( rand() % 4 ) == 0 ); // 움직일 확률 25%
		if( bFieldMove )
		{
			if( !SiKRobotPatternManager()->GetRandomFieldRobotPattern( m_iMapID, m_listFieldPattern ) )
			{
				START_LOG( cerr, L"필드에서 이동하려는데 패턴얻기 실패했음!" )
					<< BUILD_LOG( m_iMapID )
					<< END_LOG;
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}
	else
	{
		START_LOG( cerr, L"필드 이동 완료 실패!" )
			<< BUILD_LOG( m_kClientData.GetCID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( EGS_ENTER_NPC_HOUSE_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KEGS_TALK_WITH_NPC_REQ kPacket;
		kPacket.m_iNPCID = 0;

		SendPacket( EGS_TALK_WITH_NPC_REQ, kPacket );
		AddWaitingEvent( EGS_TALK_WITH_NPC_ACK, PACKET_WAIT_TIME );
	}
}

_IMPL_ON_FUNC( EGS_TALK_WITH_NPC_ACK, KEGS_TALK_WITH_NPC_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 웬 퀘스트 아이디?
		kPacket_.m_iQuestID;

		if( m_kClientData.GetQuestTF() == false )
		{
			// 퀘스트 수락
			KEGS_NEW_QUEST_REQ kPacket;
			kPacket.m_iQuestID = 10;
			kPacket.m_iTalkNPCID = 10001;

			SendPacket( EGS_NEW_QUEST_REQ, kPacket );
			AddWaitingEvent( EGS_NEW_QUEST_ACK, PACKET_WAIT_TIME );
		}
		else
		{
			// 퀘스트 포기
			KEGS_GIVE_UP_QUEST_REQ kPacket;
			kPacket.m_iQuestID = 10;

			SendPacket( EGS_GIVE_UP_QUEST_REQ, kPacket );
			AddWaitingEvent( EGS_GIVE_UP_QUEST_ACK, PACKET_WAIT_TIME );
		}
	}	
}

_IMPL_ON_FUNC( EGS_NEW_QUEST_ACK, KEGS_NEW_QUEST_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

	m_kClientData.SetQuestTF( true );	
}

_IMPL_ON_FUNC( EGS_NEW_QUEST_NOT, KEGS_NEW_QUEST_NOT )
{
	// 이건 무슨 용도일까?
}

_IMPL_ON_FUNC( EGS_GIVE_UP_QUEST_ACK, KEGS_GIVE_UP_QUEST_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

	m_kClientData.SetQuestTF( false );
}

_IMPL_ON_FUNC( EGS_ENTER_EQUIP_SHOP_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{

	}
}

_IMPL_ON_FUNC( EGS_SQUARE_LIST_ACK, KEGS_SQUARE_LIST_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );
		
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kClientData.SetSquareList( kPacket_.m_vecSquareInfo );
	}
}

_IMPL_ON_FUNC( EGS_JOIN_SQUARE_ACK, KEGS_JOIN_SQUARE_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//광장에 들어가자
		StateTransition( KRBFSM::I_TO_ROOM );

		// 광장에 이미 존재하는 유닛 리스트 저장				
		for( int i = 0; i < (int)kPacket_.m_vecUserInfo.size(); i++ )
		{
			KSquareUserInfo* pKSquareUserInfo = &kPacket_.m_vecUserInfo[i];
			m_kClientData.JoinSquareUnit( pKSquareUserInfo );
		}
	}
}

_IMPL_ON_FUNC( EGS_JOIN_SQUARE_NOT, KEGS_JOIN_SQUARE_NOT )
{
	VERIFY_STATE( ( 1, KRBFSM::S_ROOM ) );

	m_kClientData.JoinSquareUnit( &kPacket_.m_kUserInfo );
}

//IMPL_ON_FUNC( EGS_SQUARE_CHAT_ACK )
//{
//	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
//}

IMPL_ON_FUNC( EGS_LEAVE_SQUARE_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kClientData.ClearSquareUnitList();
		StateTransition( KRBFSM::S_FIELD_MAP );
		//DoAction(  A_STATE_CHANGE_VILLAGE_MAP );
	}
}

_IMPL_ON_FUNC( EGS_LEAVE_SQUARE_NOT, KEGS_LEAVE_SQUARE_NOT )
{
	VERIFY_STATE( ( 1, KRBFSM::S_ROOM ) );

	// 광장을 나간 유닛 UID 제거
	m_kClientData.DelSquareUnit( kPacket_.m_iUnitUID );
}

//_IMPL_ON_FUNC( EGS_ENTER_DUNGEON_CHANNEL_ACK, KPacketOK )
//{
//	VERIFY_STATE( ( 1, KGSFSM::S_LOCAL_MAP ) );
//
//	if( kPacket_.m_iOK == NetError::NET_OK )
//	{
//		m_eChannelType = CT_DUNGEON;
//		StateTransition( KGSFSM::I_TO_CHANNEL );
//	}
//}

IMPL_ON_FUNC( EGS_ROOM_LIST_ACK )
{
    VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

    m_kClientData.SetRoomList( kPacket_.m_vRoomInfo );
}

IMPL_ON_FUNC( EGS_CREATE_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kClientData.SetRoomInfo( kPacket_.m_RoomInfo );
		m_kClientData.SetRoomSlotInfo( kPacket_.m_vecSlot );

		for( int i = 0; i < (int)kPacket_.m_vecSlot.size(); ++i )
		{
			KRoomSlotInfo kSlotInof = kPacket_.m_vecSlot[i];
			if( kSlotInof.m_kRoomUserInfo.m_nUnitUID == m_kClientData.GetCID() )
			{
				m_kClientData.SetTeamNum( kSlotInof.m_TeamNum );

				START_LOG( clog, L"TEAM NUM" )
					<< BUILD_LOG( kSlotInof.m_TeamNum );
				break;
			}
		}

        StateTransition( KRBFSM::I_TO_ROOM );
	}
}

IMPL_ON_FUNC( EGS_CREATE_TC_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//m_kClientData.SetRoomInfo( kPacket_.m_RoomInfo );
		//m_kClientData.SetRoomSlotInfo( kPacket_.m_vecSlot );

		StateTransition( KRBFSM::I_TO_ROOM );

		//로딩완료 패킷을 보내 게임상태로 만든다.
		KEGS_GAME_LOADING_REQ kPacket;
		kPacket.m_iLoadingProgress	= 100;
		SendPacket( EGS_GAME_LOADING_REQ, kPacket );

		AddWaitingEvent( EGS_GAME_LOADING_ACK, PACKET_WAIT_TIME );
		AddWaitingEvent( EGS_GAME_LOADING_NOT, PACKET_WAIT_TIME );
		AddWaitingEvent( EGS_GAME_LOADING_ALL_UNIT_OK_NOT, PACKET_WAIT_TIME );		 
	}
}

IMPL_ON_FUNC( EGS_JOIN_ROOM_ACK )
{
    VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        m_kClientData.SetRoomInfo( kPacket_.m_RoomInfo );
        m_kClientData.SetRoomSlotInfo( kPacket_.m_vecSlot );

		for( int i = 0; i < (int)kPacket_.m_vecSlot.size(); ++i )
		{
			KRoomSlotInfo kSlotInof = kPacket_.m_vecSlot[i];
			if( kSlotInof.m_kRoomUserInfo.m_nUnitUID == m_kClientData.GetCID() )
			{
				m_kClientData.SetTeamNum( kSlotInof.m_TeamNum );

				START_LOG( clog, L"TEAM NUM" )
					<< BUILD_LOG( kSlotInof.m_TeamNum );
				break;
			}
		}

        StateTransition( KRBFSM::I_TO_ROOM );
    }
}

IMPL_ON_FUNC( EGS_QUICK_JOIN_ACK )
{
    VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        m_kClientData.SetRoomInfo( kPacket_.m_RoomInfo );
        m_kClientData.SetRoomSlotInfo( kPacket_.m_vecSlot );

		for( int i = 0; i < (int)kPacket_.m_vecSlot.size(); ++i )
		{
			KRoomSlotInfo kSlotInof = kPacket_.m_vecSlot[i];
			if( kSlotInof.m_kRoomUserInfo.m_nUnitUID == m_kClientData.GetCID() )
			{
				m_kClientData.SetTeamNum( kSlotInof.m_TeamNum );

				START_LOG( clog, L"TEAM NUM" )
					<< BUILD_LOG( kSlotInof.m_TeamNum );
				break;
			}
		}

        StateTransition( KRBFSM::I_TO_ROOM );
    }
}

_IMPL_ON_FUNC( EGS_LEAVE_ROOM_ACK, KPacketOK )
{
    VERIFY_STATE( ( 1, KRBFSM::S_ROOM ) );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        StateTransition( KRBFSM::S_FIELD_MAP );
    }
}

_IMPL_ON_FUNC( EGS_CHAT_ACK, KPacketOK )
{
	VERIFY_STATE( ( 3, KRBFSM::S_FIELD_MAP, KRBFSM::S_ROOM, KRBFSM::S_PARTY ) );
}

IMPL_ON_FUNC( EGS_CHANGE_TEAM_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kClientData.SetTeamNum( !m_kClientData.GetTeamNum() );
	}
}

IMPL_ON_FUNC( EGS_CHANGE_DUNGEON_DIFFICULTY_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KRoomInfo kRoomInfo;
		m_kClientData.GetRoomInfo( kRoomInfo );
		kRoomInfo.m_DifficultyLevel = m_iDifficulty;
		m_kClientData.SetRoomInfo( kRoomInfo );
	}
}

IMPL_ON_FUNC( EGS_CREATE_PARTY_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 파티 상태로 이동
		StateTransition( KRBFSM::I_TO_PARTY );
	}
	else
	{
		START_LOG( cerr, L"파티 생성 실패!" )
			<< BUILD_LOG( m_kClientData.GetNickName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_PARTY_LIST_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		std::vector< KPartyListInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecPartyInfo.begin(); vit != kPacket_.m_vecPartyInfo.end(); ++vit )
		{
			if( !vit->m_kPartyInfo.m_bPublic )
				continue;

			if( ( vit->m_kPartyInfo.m_MaxSlot - vit->m_kPartyInfo.m_JoinSlot ) <= 0 )
				continue;

            KEGS_JOIN_PARTY_REQ kPacket;
			kPacket.m_iPartyUID = vit->m_kPartyInfo.m_iPartyUID;
			SendPacket( EGS_JOIN_PARTY_REQ, kPacket );
			AddWaitingEvent( EGS_JOIN_PARTY_ACK, PACKET_WAIT_TIME );
			return;			
		}
	}
	else
	{
		START_LOG( cerr, L"파티 리스트 요청 실패!" )
			<< BUILD_LOG( m_kClientData.GetNickName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_JOIN_PARTY_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{		
		// 여기는 어떻게 처리할까?

	}
	else
	{
		START_LOG( cerr, L"파티 입장 요청 실패!" )
			<< BUILD_LOG( m_kClientData.GetNickName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_JOIN_PARTY_INVITE_NOT )
{
	VERIFY_STATE( ( 1, KRBFSM::S_PARTY ) );

	// 파티 가입 요청이 왔다~ 어떻게 할까?

	// 테스트를 위해 무조건 ok!

	KEGS_JOIN_PARTY_INVITE_REPLY_NOT kPacket;
	kPacket.m_cReplyReason = KEGS_JOIN_PARTY_INVITE_REPLY_NOT::RR_ACCEPT;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	SendPacket( EGS_JOIN_PARTY_INVITE_REPLY_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_JOIN_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KRBFSM::S_FIELD_MAP, KRBFSM::S_PARTY ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 파티 상태로 이동
		StateTransition( KRBFSM::I_TO_PARTY );
	}
	else
	{
        START_LOG( cerr, L"파티 가입 실패!" )
			<< BUILD_LOG( m_kClientData.GetNickName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_LEAVE_PARTY_ACK )
{
	VERIFY_STATE( ( 1, KRBFSM::S_PARTY ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 필드 상태로 이동
		StateTransition( KRBFSM::I_TO_FIELD_MAP );
	}
}

_IMPL_ON_FUNC( EGS_SEARCH_UNIT_ACK, KEGS_SEARCH_UNIT_ACK )
{
	VERIFY_STATE( ( 2, KRBFSM::S_ROOM, KRBFSM::S_FIELD_MAP ) );
}

IMPL_ON_FUNC( EGS_NOTIFY_MSG_NOT )
{
	START_LOG( clog, L"공지메시지!" )
		<< BUILD_LOGc( kPacket_.m_cNotifyType )
		<< BUILD_LOGc( kPacket_.m_Count )
		<< BUILD_LOG( kPacket_.m_wstrMSG );    
}

IMPL_ON_FUNC( EGS_MEGAPHONE_MESSAGE_NOT )
{
	START_LOG( clog, L"메가폰메시지!" )
		<< BUILD_LOG( kPacket_.m_iSenderChannelID )
		<< BUILD_LOG( kPacket_.m_iSenderLocation )
		<< BUILD_LOG( kPacket_.m_wstrSenderNickName )
		<< BUILD_LOGc( kPacket_.m_cMessageColor )
		<< BUILD_LOG( kPacket_.m_wstrMessage );
}

IMPL_ON_FUNC( EGS_INVITE_GUILD_NOT )
{
	// 길드 초대 받으면 무조건 수락하기!

    KEGS_INVITE_GUILD_REPLY_REQ kPacket;
	kPacket.m_iGuildUID = kPacket_.m_kGuildInfo.m_iGuildUID;
	kPacket.m_wstrSenderNickName = kPacket_.m_wstrSenderNickName;
	kPacket.m_iReason = NetError::NET_OK;
	SendPacket( EGS_INVITE_GUILD_REPLY_REQ, kPacket );
}

//////////////////////////////////////////////////////////////////////////
// 로봇 행동 핸들러
void KncRobot::DoAction( int iAction )
{
    m_kTimer[TM_ACTION_GAP].restart();
    m_fActionGap = 5.0f * ( float )( ( float )rand() / ( float )RAND_MAX );

    switch( iAction )
    {
#   undef _ENUM
#   define _ENUM( id ) case id: ON_##id(); break;
#   include "RobotAction_def.h"
    }
}

IMPL_ON_ACTION_FUNC( A_CONNECT_CHANNEL_SERVER )
{
	// 채널 선택 스테이트
	StateTransition( KRBFSM::I_TO_CHANNEL_SELECT );

	//if( ms_vecGSAddress.empty() )
	//{
	//	START_LOG( cerr, L"접속할 채널 서버 주소가 없음." );
	//	return;
	//}

	//if( ms_iRemainCapacity > 0 )
	//{
	//	ms_iRemainCapacity--;
	//}
	//else
	//{
	//	return;
	//}

	//int iGS = rand() % ms_vecGSAddress.size();
	//m_strIP = KncUtil::toNarrowString( ms_vecGSAddress[iGS].m_wstrIP );
	//u_short usPort = ms_vecGSAddress[iGS].m_usPort;

	//if( m_nSPIndex != 0 )
	//{
	//	START_LOG( cerr, L"보안키가 0이 아닌데 접속 또한다. ;ㅁ;" )
	//		<< BUILD_LOG( m_nSPIndex )
	//		<< END_LOG;

	//	OnDestroy();

	//	return;
	//}

	//if( !Connect( m_strIP.c_str(), usPort ) )
	//{
	//	START_LOG( cerr, L"접속 실패." )
	//		<< BUILD_LOG( m_strIP )
	//		<< BUILD_LOG( usPort )
	//		<< END_LOG;

	//	return;
	//}

	//// 채널 선택 스테이트
	//StateTransition( KRBFSM::I_TO_CHANNEL_SELECT );

	//SendIDToChServer( ECH_GET_CHANNEL_LIST_NOT );
}

IMPL_ON_ACTION_FUNC( A_GET_CHANNEL_LIST )
{
	SendIDToChServer( ECH_GET_CHANNEL_LIST_NOT );
}

IMPL_ON_ACTION_FUNC( A_CONNECT_GAME_SERVER )
{
    if( ms_vecGSAddress.empty() )
    {
        START_LOG( cerr, L"접속할 게임 서버 주소가 없음." );
        return;
    }

    if( ms_iRemainCapacity > 0 )
    {
        ms_iRemainCapacity--;
    }
    else
    {
        return;
    }

    int iGS = rand() % ms_vecGSAddress.size();
    m_strIP = KncUtil::toNarrowString( ms_vecGSAddress[iGS].m_wstrIP );
    u_short usPort = ms_vecGSAddress[iGS].m_usPort;

	if( m_nSPIndex != 0 )
	{
		START_LOG( cerr, L"보안키가 0이 아닌데 접속 또한다. ;ㅁ;" )
			<< BUILD_LOG( m_nSPIndex )
			<< END_LOG;

		OnDestroy();

		return;
	}

    if( !Connect( m_strIP.c_str(), usPort ) )
    {
        START_LOG( cerr, L"접속 실패." )
            << BUILD_LOG( m_strIP )
            << BUILD_LOG( usPort )
            << END_LOG;

        return;
    }

    KEGS_CONNECT_REQ kPacket;
	kPacket.m_wstrVersion = SiKncRobotManager()->GetVersion();//SiKncRobotThreadManager()->GetVersion();

    SendPacket( EGS_CONNECT_REQ, kPacket );
    AddWaitingEvent( EGS_CONNECT_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_MAKE_ACCOUNT )
{
    char szName[MAX_PATH];
    ::wsprintfA( szName, "%s%05d", m_strNameHead.c_str(), m_iIndex );
    std::wstring wstrName = KncUtil::toWideString( std::string( szName ) );

    KEGS_NEW_USER_JOIN_REQ kPacket;
    kPacket.m_wstrID = wstrName;
    kPacket.m_wstrPassword = wstrName;
    kPacket.m_wstrName = wstrName;

    SendPacket( EGS_NEW_USER_JOIN_REQ, kPacket );
    AddWaitingEvent( EGS_NEW_USER_JOIN_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_VERIFY )
{
    char szName[MAX_PATH];
    ::wsprintfA( szName, "%s%05d", m_strNameHead.c_str(), m_iIndex );
    std::wstring wstrName = KncUtil::toWideString( std::string( szName ) );

    KEGS_VERIFY_ACCOUNT_REQ kPacket;
    kPacket.m_bDebugAuth = true;
    kPacket.m_wstrUserID = wstrName.c_str();
    kPacket.m_wstrPasswd = wstrName.c_str();

    SendPacket( EGS_VERIFY_ACCOUNT_REQ, kPacket );
    AddWaitingEvent( EGS_VERIFY_ACCOUNT_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_SELECT_SERVER )
{
    KEGS_SELECT_SERVER_SET_REQ kPacket;
    kPacket.m_ServerSetID = 1;

    SendPacket( EGS_SELECT_SERVER_SET_REQ, kPacket );
    AddWaitingEvent( EGS_SELECT_SERVER_SET_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_SELECT_UNIT )
{
	KUnitInfo kUnitInfo;
	if( m_kClientData.GetFirstUnitInfo( kUnitInfo ) == true )
	{
		KEGS_SELECT_UNIT_REQ kPacket;
		kPacket.m_iUnitUID	= kUnitInfo.m_nUnitUID;

		SendPacket( EGS_SELECT_UNIT_REQ, kPacket );
		AddWaitingEvent( EGS_SELECT_UNIT_ACK, PACKET_WAIT_TIME );
	}
	else
	{
		DoAction( A_CREATE_UNIT );
	}
}

IMPL_ON_ACTION_FUNC( A_CREATE_UNIT )
{
    KEGS_CREATE_UNIT_REQ kPacket;
	kPacket.m_iClass = (rand() % ( CXSLUnit::UT_END - 1) ) + 1;
    kPacket.m_wstrNickName = m_kClientData.GetID();

    SendPacket( EGS_CREATE_UNIT_REQ, kPacket );
    AddWaitingEvent( EGS_CREATE_UNIT_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_DELETE_UNIT )
{
    KUnitInfo kUnitInfo;
    if( m_kClientData.GetFirstUnitInfo( kUnitInfo ) == true )
    {
        KEGS_DELETE_UNIT_REQ kPacket;
        kPacket.m_iUnitUID	= kUnitInfo.m_nUnitUID;

        SendPacket( EGS_DELETE_UNIT_REQ, kPacket );
        AddWaitingEvent( EGS_DELETE_UNIT_ACK, PACKET_WAIT_TIME );
    }
    else
    {
        DoAction( A_CREATE_UNIT );
    }
}

IMPL_ON_ACTION_FUNC( A_STATE_CHANGE_SERVER_SELECT )
{
	SendID( EGS_STATE_CHANGE_SERVER_SELECT_REQ );
	AddWaitingEvent( EGS_STATE_CHANGE_SERVER_SELECT_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_STATE_CHANGE_FIELD )
{
	//////////////////////////////////////////////////////////////////////////
	KEGS_STATE_CHANGE_FIELD_REQ kPacket;
	kPacket.m_iMapID			= SiKRobotPatternManager()->GetRandomMapID();

	if( kPacket.m_iMapID == m_iMapID )
	{
		// 현재 위치한 mapID와 동일한 mapID가 나왔다면 이동하지 말자~!
		return;
	}

	m_iMapID					= kPacket.m_iMapID;

	SendPacket( EGS_STATE_CHANGE_FIELD_REQ, kPacket );
	AddWaitingEvent( EGS_STATE_CHANGE_FIELD_ACK, PACKET_WAIT_TIME );
	//////////////////////////////////////////////////////////////////////////
}

IMPL_ON_ACTION_FUNC( A_FIELD_MOVE )
{
	//////////////////////////////////////////////////////////////////////////
	// 움직이자!
	if( !SiKRobotPatternManager()->GetRandomFieldRobotPattern( m_iMapID, m_listFieldPattern ) )
	{
		START_LOG( cerr, L"필드에서 이동하려는데 패턴얻기 실패했음!" )
			<< BUILD_LOG( m_iMapID )
			<< END_LOG;
	}
	//////////////////////////////////////////////////////////////////////////
}

IMPL_ON_ACTION_FUNC( A_FIELD_STAND )
{
	std::list< KRobotPattern >::const_iterator lit;
	std::list< KRobotPattern > listPattern;

	if( !SiKRobotPatternManager()->GetRandomFieldRobotPattern( m_iMapID, listPattern ) )
	{
		START_LOG( cerr, L"필드에서 이동하려는데 패턴얻기 실패했음!" )
			<< BUILD_LOG( m_iMapID )
			<< END_LOG;
	}

	int iCount = 0;
	int iIndex = rand() % listPattern.size();

	for( lit = listPattern.begin(); lit != listPattern.end(); ++lit )
	{
		if( iCount == iIndex )
		{
			const KRobotPattern& kPattern = *lit;

			m_bIsRight = kPattern.m_bIsRight;
			m_kCurFieldPos = kPattern.m_vPos;
			break;
		}

		++iCount;
	}

	// 가만히 서있기!
	KEGS_FIELD_UNIT_SYNC_DATA_NOT kPacketNot;
	kPacketNot.m_bIsRight	   = m_bIsRight;	
	kPacketNot.m_vPos		   = m_kCurFieldPos;
	SendPacket( EGS_FIELD_UNIT_SYNC_DATA_NOT, kPacketNot );
	
	m_tStandTimer.restart();
}

IMPL_ON_ACTION_FUNC( A_FIELD_CHAT )
{
    KEGS_CHAT_REQ kPacket;
	kPacket.m_cChatPacketType = KEGS_CHAT_REQ::CPT_TOTAL;
	kPacket.m_ToUnitUID = 0;
	kPacket.m_cRoomChatType = CXSLRoom::RCT_ALL;
	
	switch( rand() % 5 )
	{
	case 0: kPacket.m_wstrMsg = L"머라카노~!? 내는 로봇이 아이다! 내는 사람이다! 와 못믿노!?"; break;
	case 1: kPacket.m_wstrMsg = L"먼데? 니 로봇 맞잖아! 왜 거짓말 하는데?"; break;
	case 2: kPacket.m_wstrMsg = L"그래 내 로봇 맞다! 그래서 니는 로봇 아이가?"; break;
	case 3: kPacket.m_wstrMsg = L"니보다는 낫다! 니 어데고!? 내랑 맞짱 까자!"; break;
	default: kPacket.m_wstrMsg = L"내가 왜 니랑 맞짱까는데? 치아라! 헛소리하지말고!!"; break;
	}
	
	SendPacket( EGS_CHAT_REQ, kPacket );
	AddWaitingEvent( EGS_CHAT_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_CREATE_PARTY )
{
	if( GetStateID() == KRBFSM::S_PARTY )
	{
		START_LOG( cerr, L"이미 파티에 가입되어 있는데 생성하려고 하네?" )
			<< BUILD_LOG( m_kClientData.GetNickName() )
			<< END_LOG;
		return;
	}

	// 파티를 생성하자!
	KEGS_CREATE_PARTY_REQ kPacket;
	kPacket.m_wstrPartyName = L"로봇 파티 결성! 모두 집합!";
	kPacket.m_bPublic = true;
	kPacket.m_cGetItemType = CXSLRoom::DGIT_RANDOM;
	kPacket.m_DifficultyLevel = 0;
	kPacket.m_iDungeonID = SEnum::DI_EL_FOREST_WEST_NORMAL;
	kPacket.m_iPartyUID = 0;
	kPacket.m_MaxSlot = 4;
	kPacket.m_JoinSlot = 1;
	
	SendPacket( EGS_CREATE_PARTY_REQ, kPacket );
	AddWaitingEvent( EGS_CREATE_PARTY_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_JOIN_PARTY )
{
	if( GetStateID() == KRBFSM::S_PARTY )
	{
		START_LOG( cerr, L"이미 파티에 가입되어 있는데 가입하려고 하네?" )
			<< BUILD_LOG( m_kClientData.GetNickName() )
			<< END_LOG;
		return;
	}

	// 던전게이트라면 파티리스트 요청!
	switch( m_iMapID )
	{
	case CXSLMapData::MI_VELDER_NORTH:
	case CXSLMapData::MI_VELDER_EAST:
	case CXSLMapData::MI_VELDER_SOUTH:
	case CXSLMapData::MI_ALTERA_ISLAND:
	case CXSLMapData::MI_PEITA:
		{
			// 파티에 들어가기 위해 파티리스트를 요청하자!
			KEGS_PARTY_LIST_REQ kPacket;
			kPacket.m_iID = m_iMapID;
			kPacket.m_cListType = 1;
			kPacket.m_nViewPage = 1;
			SendPacket( EGS_PARTY_LIST_REQ, kPacket );
			AddWaitingEvent( EGS_PARTY_LIST_ACK, PACKET_WAIT_TIME );
		}
		break;
	}
}

IMPL_ON_ACTION_FUNC( A_LEAVE_PARTY )
{
	if( GetStateID() != KRBFSM::S_PARTY )
	{
		START_LOG( cerr, L"파티에 가입되어 있지도 않은데 이탈하려고 하네?" )
			<< BUILD_LOG( m_kClientData.GetNickName() )
			<< END_LOG;
		return;
	}

	// 파티에서 이탈하자!
	KEGS_LEAVE_PARTY_REQ kPacket;
	kPacket.m_iReason = 0;
	SendPacket( EGS_LEAVE_PARTY_REQ, kPacket );
	AddWaitingEvent( EGS_LEAVE_PARTY_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_PARTY_CHAT )
{
	KEGS_CHAT_REQ kPacket;
	kPacket.m_cChatPacketType = KEGS_CHAT_REQ::CPT_PARTY;
	kPacket.m_ToUnitUID = 0;
	kPacket.m_cRoomChatType = CXSLRoom::RCT_ALL;

	switch( rand() % 5 )
	{
	case 0: kPacket.m_wstrMsg = L"이거슨 파티 채팅! 파티원들아~ 잘 들리느뇨!?"; break;
	case 1: kPacket.m_wstrMsg = L"네 잘들립니다~~ 우헤헤"; break;
	case 2: kPacket.m_wstrMsg = L"업드려 뻣쳐~!!!!"; break;
	case 3: kPacket.m_wstrMsg = L"뭔데? 니가 뭔데 업드리라카노!"; break;
	default: kPacket.m_wstrMsg = L"왜냐고? 난 파티장이니까!"; break;
	}

	SendPacket( EGS_CHAT_REQ, kPacket );
	AddWaitingEvent( EGS_CHAT_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_ENTER_NPC_HOUSE )
{
	KEGS_ENTER_NPC_HOUSE_REQ kPacket;
	kPacket.m_iNPCID = 20000; // ㅡ ㅡ ; 왜 0을 넣는거지?

	SendPacket( EGS_ENTER_NPC_HOUSE_REQ, kPacket );
	AddWaitingEvent( EGS_ENTER_NPC_HOUSE_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_ENTER_EQUIP_SHOP )
{
	KEGS_ENTER_NPC_HOUSE_REQ kPacket;
	kPacket.m_iNPCID = 0; // ㅡ ㅡ ; 왜 0을 넣는거지?

	SendPacket( EGS_ENTER_EQUIP_SHOP_REQ, kPacket );
	AddWaitingEvent( EGS_ENTER_EQUIP_SHOP_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_OPEN_PVP_ROOM_LIST )
{
	SendID( EGS_OPEN_PVP_ROOM_LIST_REQ );
	AddWaitingEvent( EGS_OPEN_PVP_ROOM_LIST_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_ROOM_LIST )
{
	KEGS_ROOM_LIST_REQ kPacket;
	kPacket.m_nViewPage = 0;

	SendPacket( EGS_ROOM_LIST_REQ, kPacket );
	AddWaitingEvent( EGS_ROOM_LIST_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_JOIN_SQUARE )
{
	KEGS_JOIN_SQUARE_REQ kPacket;
	kPacket.m_iSquareUID; // 들어갈 광장 SquareUID

	SendPacket( EGS_JOIN_SQUARE_REQ, kPacket );
	AddWaitingEvent( EGS_JOIN_SQUARE_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_SQUARE_UNIT_SYNC )
{
	if( m_kTimer[TM_SQUARE_SYNC].elapsed() < 3.0f )
		return;

	KEGS_SQUARE_UNIT_SYNC_DATA_REQ kEGS_SQUARE_UNIT_SYNC_DATA_REQ;

	kEGS_SQUARE_UNIT_SYNC_DATA_REQ.m_StateID				= 1;
	kEGS_SQUARE_UNIT_SYNC_DATA_REQ.m_StateChangeNum			= 2;
	kEGS_SQUARE_UNIT_SYNC_DATA_REQ.m_vPos.x					= 0;
	kEGS_SQUARE_UNIT_SYNC_DATA_REQ.m_vPos.y					= 0;
	kEGS_SQUARE_UNIT_SYNC_DATA_REQ.m_vPos.z					= 0;
	kEGS_SQUARE_UNIT_SYNC_DATA_REQ.m_LastTouchLineIndex		= 1;
	kEGS_SQUARE_UNIT_SYNC_DATA_REQ.m_bIsRight				= true;

	SendPacket( EGS_SQUARE_UNIT_SYNC_DATA_REQ, kEGS_SQUARE_UNIT_SYNC_DATA_REQ );

	m_kTimer[TM_SQUARE_SYNC].restart();
}

IMPL_ON_ACTION_FUNC( A_CREATE_ROOM )
{
	KEGS_CREATE_ROOM_REQ kPacket;

	switch( m_iRoomType )
	{
	case CXSLRoom::RT_PVP:
		{
			int nPVPGameType = rand() % 3;
			kPacket.m_RoomInfo.m_PVPGameType = nPVPGameType;
			switch( nPVPGameType )
			{
			case CXSLRoom::PGT_TEAM:
				kPacket.m_RoomInfo.m_bCanIntrude	= false;
				kPacket.m_RoomInfo.m_WinKillNum	= 0;
				kPacket.m_RoomInfo.m_fPlayTime		= 200.0f;
				break;
			case CXSLRoom::PGT_TEAM_DEATH:
				kPacket.m_RoomInfo.m_bCanIntrude	= true;
				kPacket.m_RoomInfo.m_WinKillNum	= 4;
				kPacket.m_RoomInfo.m_fPlayTime		= 200.0f;
				break;
			case CXSLRoom::PGT_SURVIVAL:
				kPacket.m_RoomInfo.m_bCanIntrude	= true;
				kPacket.m_RoomInfo.m_WinKillNum	= 3;
				kPacket.m_RoomInfo.m_fPlayTime		= 200.0f;
				break;
			}

			kPacket.m_RoomInfo.m_RoomName		= L"앗싸 고 로봇 대전!";
			kPacket.m_RoomInfo.m_MaxSlot		= 8;
			kPacket.m_RoomInfo.m_bPublic		= true;
			kPacket.m_RoomInfo.m_WorldID		= 2;
			kPacket.m_RoomInfo.m_bTeamBalance	= true;
		}
		break;

	case CXSLRoom::RT_DUNGEON:
		{
			//던전방 생성
			//kPacket.m_RoomInfo.m_RoomName		= L"로보트 고고 던전.!";
			//kPacket.m_RoomInfo.m_bPublic		= true;
			//kPacket.m_RoomInfo.m_wstrPassword	= L"";
			//kPacket.m_RoomInfo.m_MaxSlot		= 4;
			////kPacket.m_RoomInfo.m_DungeonID		= m_kClientData.GetMapID();
			//kPacket.m_RoomInfo.m_iDungeonID		= 30040;
			//kPacket.m_RoomInfo.m_DifficultyLevel = 0;

			//튜토리얼 생성
			kPacket.m_RoomInfo.m_RoomName		= L"tutorial";
			kPacket.m_RoomInfo.m_bPublic		= false;
			kPacket.m_RoomInfo.m_wstrPassword	= L"a*h97lk2";
			kPacket.m_RoomInfo.m_MaxSlot		= 1;
			kPacket.m_RoomInfo.m_iDungeonID		= 30030;
			kPacket.m_RoomInfo.m_DifficultyLevel = 0;
			kPacket.m_RoomInfo.m_fPlayTime		= 19999.0f;
		}
		break;

	case CXSLRoom::RT_SQUARE:
		return;
	}

	SendPacket( EGS_CREATE_ROOM_REQ, kPacket );
	AddWaitingEvent( EGS_CREATE_ROOM_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_CREATE_TC_ROOM )
{
	KEGS_CREATE_TC_ROOM_REQ kPacket;
	kPacket.m_iTCID = 1;

	SendPacket( EGS_CREATE_TC_ROOM_REQ, kPacket );
	AddWaitingEvent( EGS_CREATE_TC_ROOM_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_JOIN_ROOM )
{
	if( CXSLRoom::RT_SQUARE != m_iRoomType )
	{
		std::vector< KRoomInfo > vecRoomList;
		m_kClientData.GetRoomList( vecRoomList );
		if( !vecRoomList.empty() )
		{
			int iIndex = rand() % vecRoomList.size();
			KEGS_JOIN_ROOM_REQ kPacket;
			kPacket.m_iRoomUID = vecRoomList[iIndex].m_RoomUID;

			SendPacket( EGS_JOIN_ROOM_REQ, kPacket );
			AddWaitingEvent( EGS_JOIN_ROOM_ACK, PACKET_WAIT_TIME );
		}
	}
	else
	{		
		std::vector< KSquareInfo > vecSquareList;
		m_kClientData.GetSquareList( vecSquareList );
		if( !vecSquareList.empty())
		{
			int iIndex = rand() % vecSquareList.size();
			KEGS_JOIN_SQUARE_REQ kPacket;

			kPacket.m_iSquareUID = vecSquareList[iIndex].m_iSquareUID;

			START_LOG( clog, "광장 UID : "<<kPacket.m_iSquareUID );

			SendPacket( EGS_JOIN_SQUARE_REQ, kPacket );
			AddWaitingEvent( EGS_JOIN_SQUARE_ACK, PACKET_WAIT_TIME );
		}
	}
}

IMPL_ON_ACTION_FUNC( A_QUICK_JOIN_ROOM )
{
	if( CXSLRoom::RT_SQUARE != m_iRoomType )
	{
		KEGS_QUICK_JOIN_REQ kPacket;
		kPacket.m_iOption = rand() % 3;

		SendPacket( EGS_QUICK_JOIN_REQ, kPacket );
		AddWaitingEvent( EGS_QUICK_JOIN_ACK, PACKET_WAIT_TIME );
	}
}

IMPL_ON_ACTION_FUNC( A_LEAVE_ROOM )
{
	//if( m_kClientData.GetPartyJoin() )
	//{
	//	// 파티에서 나오기
	//	KEGS_LEAVE_PARTY_REQ kPacket;
	//	kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
	//	SendPacket( EGS_LEAVE_PARTY_REQ, kPacket );
	//       AddWaitingEvent( EGS_LEAVE_PARTY_ACK, PACKET_WAIT_TIME );
	//	return;
	//}
	//
	if( CXSLRoom::RT_SQUARE != m_iRoomType )
	{
		// 방에서 나오기
		KEGS_LEAVE_ROOM_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
		SendPacket( EGS_LEAVE_ROOM_REQ , kPacket );
		AddWaitingEvent( EGS_LEAVE_ROOM_ACK, PACKET_WAIT_TIME );
	}
	else
	{
		// 광장에서 나오기
		KEGS_LEAVE_SQUARE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
		SendPacket( EGS_LEAVE_SQUARE_REQ , kPacket );
		AddWaitingEvent( EGS_LEAVE_SQUARE_ACK, PACKET_WAIT_TIME );
	}
}


//IMPL_ON_ACTION_FUNC( A_ENTER_SQUARE_CHANNEL )
//{
//	SendID( EGS_ENTER_SQUARE_CHANNEL_REQ );
//	AddWaitingEvent( EGS_ENTER_SQUARE_CHANNEL_ACK, PACKET_WAIT_TIME );
//}
//
//IMPL_ON_ACTION_FUNC( A_ENTER_DUNGEON_CHANNEL )
//{
//	KEGS_ENTER_DUNGEON_CHANNEL_REQ kPacket;
//
//	//switch( rand() % 3 )
//	//switch( 0 )
//	//{
//	//case 0:
//	//	kPacket.m_nMapID	= 30000;	//엘의나무
//	//	break;
//	//case 1:
//	//	kPacket.m_nMapID	= 30010;	//엘의나무 서쪽
//	//	break;
//	//case 2:
//	//	kPacket.m_nMapID	= 30020;	//엘의나무 북쪽
//	//	break;
//	//}
//
//	kPacket.m_nMapID = 30040;
//
//	m_kClientData.SetMapID( kPacket.m_nMapID );
//
//	SendPacket( EGS_ENTER_DUNGEON_CHANNEL_REQ, kPacket );
//	AddWaitingEvent( EGS_ENTER_DUNGEON_CHANNEL_ACK, PACKET_WAIT_TIME );
//}
//
//IMPL_ON_ACTION_FUNC( A_STATE_CHANGE_LOCAL_MAP )
//{
//    KStateChangeReq kPacket;
//    kPacket.m_nMapID = 20000;
//
//    SendPacket( EGS_STATE_CHANGE_LOCAL_MAP_REQ, kPacket);
//    AddWaitingEvent( EGS_STATE_CHANGE_LOCAL_MAP_ACK, PACKET_WAIT_TIME );
//}
//

IMPL_ON_ACTION_FUNC( A_CHAT )
{
    KEGS_CHAT_REQ kPacket;
    kPacket.m_cChatPacketType = 0;
    kPacket.m_cRoomChatType = 0;
    kPacket.m_ToUnitUID = 0;
    kPacket.m_wstrMsg = L"아무 이유 없어. 난 로봇이야.";

    //SendPacket( EGS_CHAT_REQ, kPacket );
    //return;

	//int chatPer = 0;

	if( GetStateID() == KRBFSM::S_PARTY )
	{
		kPacket.m_cChatPacketType = KEGS_CHAT_REQ::CPT_PARTY;

		SendPacket( EGS_CHAT_REQ, kPacket );
		AddWaitingEvent( EGS_CHAT_ACK, PACKET_WAIT_TIME );
	}
	else
	{
		switch( m_iRoomType )
		{
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_SQUARE:
			{
				kPacket.m_cChatPacketType = KEGS_CHAT_REQ::CPT_TOTAL;

				SendPacket( EGS_CHAT_REQ, kPacket );
				AddWaitingEvent( EGS_CHAT_ACK, PACKET_WAIT_TIME );
			}
			break;
		}
	}

	//if( CT_SQUARE != m_eChannelType  )
	//{
	//	// 방에서 채팅
	//	kPacket_.m_cChatPacketType = KEGS_CHAT_REQ::CPT_TOTAL;

	//	SendPacket( EGS_CHAT_REQ, kPacket );
	//	AddWaitingEvent( EGS_CHAT_ACK, PACKET_WAIT_TIME );
	//}
	//else
	//{
	//	if( m_kClientData.GetPartyJoin() )
	//		chatPer = 2;
	//	else
	//		chatPer = 1;

	//	switch( rand() % chatPer )
	//	{
	//		case 0:
	//			// 광장에서 채팅
	//			SendPacket( EGS_SQUARE_CHAT_REQ , kPacket );
	//			AddWaitingEvent( EGS_CHAT_ACK, PACKET_WAIT_TIME );
	//			break;
	//		case 1:
	//			// 파티 채팅
	//			SendPacket( EGS_PARTY_CHAT_REQ, kPacket );
	//			AddWaitingEvent( EGS_CHAT_ACK, PACKET_WAIT_TIME );
	//			break;
	//		default:
	//			break;
	//	}
	//}
}

IMPL_ON_ACTION_FUNC( A_CHANGE_TEAM )
{
	//대전 채널일때만 패킷을 보낸다.
	if( m_iRoomType != CXSLRoom::RT_PVP )
		return;

	KEGS_CHANGE_TEAM_REQ kPacket;

	// 0 : Red  Team
	// 1 : Blue Team
	if( m_kClientData.GetTeamNum() == 0 )
	{
		kPacket.m_TeamNum = 1;
	}
	else
	{
		kPacket.m_TeamNum = 0;
	}

	SendPacket( EGS_CHANGE_TEAM_REQ, kPacket );
	AddWaitingEvent( EGS_CHANGE_TEAM_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_CHANGE_DIFFICULTY )
{
	//던전일 때만.
	if( m_iRoomType != CXSLRoom::RT_DUNGEON )
		return;

	KEGS_CHANGE_DUNGEON_DIFFICULTY_REQ kPacket;
	KRoomInfo kRoomInfo;
	m_kClientData.GetRoomInfo( kRoomInfo );

	//난이도 => 0 : normal 1 : hard 2 : expert
	for( int i = 0; i <= 2; ++i )
	{
		if( i != kRoomInfo.m_DifficultyLevel )
		{
			kPacket			= i;
			m_iDifficulty	= i;	//성공시 교체를 위해
			break;
		}
	}

	SendPacket( EGS_CHANGE_DUNGEON_DIFFICULTY_REQ, kPacket );
	AddWaitingEvent( EGS_CHANGE_DUNGEON_DIFFICULTY_ACK, PACKET_WAIT_TIME );
}

IMPL_ON_ACTION_FUNC( A_SEARCH_UNIT )
{
	//// 로봇을 찾아라!
	//KEGS_SEARCH_UNIT_REQ kPacket(L"RTest");
	//wchar_t robotCnt[8] = {0};

	//wsprintf(robotCnt, L"%05d", (rand() % SiKncRobotManager()->GetMaxRobotCount())+1);

	//kPacket.append(robotCnt);
	//
	//SendPacket( EGS_SEARCH_UNIT_REQ, kPacket );
}

IMPL_ON_ACTION_FUNC( A_SENTINEL )
{
    return;
}

IMPL_ON_ACTION_FUNC( A_EXIT )
{
    ReserveDestroy();
}

IMPL_ON_ACTION_FUNC( A_CLIENT_QUIT )
{
	KEGS_CLIENT_QUIT_REQ kPacket;
	kPacket.m_iReason = NetError::ERR_CLIENT_QUIT_00;

	SendPacket( EGS_CLIENT_QUIT_REQ, kPacket );
	AddWaitingEvent( EGS_CLIENT_QUIT_REQ, PACKET_WAIT_TIME );
}

bool KncRobot::FieldSync()
{
	if( m_listFieldPattern.empty() )
		return false;

	if( !m_bFieldSync )
		return false;

	if( m_kTimer[TM_FIELD_SYNC].elapsed() < 2.f )
		return true;

	m_kTimer[TM_FIELD_SYNC].restart();

	KRobotPattern kPattern = m_listFieldPattern.front();

	KEGS_FIELD_UNIT_SYNC_DATA_NOT kPacketNot;
	kPacketNot.m_bIsRight	   = kPattern.m_bIsRight;
	kPacketNot.m_vecFieldFrame = kPattern.m_vecFieldFrame;
	kPacketNot.m_vPos		   = kPattern.m_vPos;
    SendPacket( EGS_FIELD_UNIT_SYNC_DATA_NOT, kPacketNot );

	m_listFieldPattern.pop_front();

	// 마지막 위치 저장!
	m_bIsRight = kPacketNot.m_bIsRight;
	m_kCurFieldPos = kPacketNot.m_vPos;
	return true;
}

int KncRobot::DecisionMoveField()
{
	return 0;
}

bool KncRobot::ConnectTRServer( UidType iUnitUID )
{
	if( m_spTRUser == NULL )
	{
		m_spTRUser = KTRUserPtr( new KTRUser );

		m_spTRUser->Init();
	}

	if( !m_spTRUser->IsConnected() )
	{
		if( !m_spTRUser->Connect( m_strIP.c_str(), 9301, iUnitUID ) )
		{
			START_LOG( cerr, L"접속 실패.!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			return false;
		}
		
		if( !m_spTRUser->SendPacket( ETR_REG_UID, iUnitUID ) )
		{
			START_LOG( cerr, L"REG_UID send failed.!" )
				<< END_LOG;
			return false;
		}
	}	

	return true;
}


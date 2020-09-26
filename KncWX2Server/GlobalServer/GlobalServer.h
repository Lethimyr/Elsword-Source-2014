#pragma once
#include "BaseServer.h"
#include "KncSend.h"

class KGlobalServer : public KBaseServer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclDump;
    DeclPfID;

public:
#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-07-02	// 박세훈
	enum EXCHANGE_LIMIT_RESULT
	{
		ELR_SUCCEED,
		ELR_NOT_INITED,
		ELR_EXHAUSTED,
		ELR_CHECK,
		ELR_PASS,
	};
#endif // SERV_ITEM_EXCHANGE_LIMIT

    KGlobalServer(void);
    virtual ~KGlobalServer(void);

    // derived from KBaseServer
    virtual bool Init();
    virtual void RegToLua();
    virtual void ShutDown();
    virtual bool DestroyAllSession();         // 접속중인 세션의 종료.

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void Tick();

	//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
	void	ReserveServerUID( IN const int iServerGroupID, IN const UidType iServerUID );
	void	RemoveServerUID( IN const UidType iServerUID );
	void	GetServerUIDList( IN const int iServerGroupID, OUT std::set<UidType>& setServerUIDList ) const;
#endif SERV_EVENT_JACKPOT
	//}}

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
	void	GetTimeEnchantEventUID( OUT int& iTimeEnchantEventUID ) const{	iTimeEnchantEventUID = m_iTimeEnchantEventUID;	}
	void	GetNumOfBeginTimeEnchantEventInfo( OUT int& iNumOfTimeEnchantEvent ) const{	iNumOfTimeEnchantEvent = m_mapBeginTimeEnchantEventInfo.size();	}
	bool	RetrieveTimeEnchantEventData( OUT KTimeEnchantEventInfo& kInfo, OUT int& iTimeEnchantEventUID );
	bool	CheckTimeEnchantEventUID( IN const int iTimeEnchantEventUID ) const{	return m_iTimeEnchantEventUID == iTimeEnchantEventUID;	}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-29	// 박세훈
	void	GetExchangeLimitUID( OUT int& iExchangeLimitUID ) const{	iExchangeLimitUID = m_iExchangeLimitUID;	}
	void	GetBeginExchangeLimitInfo( OUT std::map< int, int >& mapBeginExchangeLimitInfo ) const;
	int		RetrieveExchangeLimitData( IN const int iDestItemID, OUT int& iLimitUID );
	int		ReInsertExchangeLimitData( IN const int iLimitUID );
	bool	CheckExchangeLimitUID( IN const int iExchangeLimitUID ) const{	return m_iExchangeLimitUID == iExchangeLimitUID;	}
#endif // SERV_ITEM_EXCHANGE_LIMIT

    //////////////////////////////////////////////////////////////////////////

protected:
	virtual void OnServerReadyComplete();
	//{{ 2010. 10. 19	최육사	SMS테스트
#ifdef SERV_SMS_TEST
	void SMSTest_LUA();
#endif SERV_SMS_TEST
	//}}

	//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
	void AddMSGBeforShutDown_LUA( int iHour, int iMin, int iSec );
	void CheckSendMSGBeforeShutDown();
	bool CheckMSGTimeBeforeShutDown();
#endif SERV_SHUTDOWN_SYSTEM
	//}}

	//{{ 공존의 축제 - 지진 효과 - 김민성
#ifdef SERV_EARTH_QUAKE_EFFECT
	void CheckEarthQuakeEffect();
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

	//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
	void	CheckJackPotEvent( void );
#endif SERV_EVENT_JACKPOT
	//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	void	CheckHeroPvpUserList( void );
#endif	// SERV_HERO_PVP_MANAGE_LIST

	//{{ 2013. 05. 29	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
	template < class T > void SendToGSUser( UidType nToGSUID, UidType nToGSUSer, unsigned short usEventID, const T& data );
#endif SERV_BLOCK_LIST
	//}}

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
	void	CheckTimeEnchantEvent( void );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-30	// 박세훈
	void	CheckExchangeLimit( IN const bool bForceNotify = false );
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-04	// 박세훈
	void	CheckFieldBossSystem( IN const CTime tCurrentTime );
#endif // SERV_BATTLE_FIELD_BOSS

	template < class T > void SendToGameServer( unsigned short usEventID, const T& data ) const;
	template < class T > void BroadCastAllGS( unsigned short usEventID, const T& data ) const;


	//{{ 2010. 10. 19	최육사	SMS테스트
#ifdef SERV_SMS_TEST
	template < class T > void SendToSMSDB( unsigned short usEventID, const T& data );
#endif SERV_SMS_TEST
	//}}

//#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	template < class T > void SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data );
//#endif	// SERV_HERO_PVP_MANAGE_LIST

   _DECL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_LOGIN_SERVER_ON_ACK );
	DECL_ON_FUNC_NOPARAM( DBE_UPDATE_SERVER_INFO_ACK );    

	//{{ 2010. 07. 19  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT
   _DECL_ON_FUNC( ESR_ORDER_TO_GLOBAL_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT );
   _DECL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_REALTIME_SCRIPT
	//}}

   //{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
   _DECL_ON_FUNC( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT );
#endif SERV_MORNITORING
   //}}
   //{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	DECL_ON_FUNC( DBE_GET_DEFENCE_CRYSTAL_ACK );
#endif SERV_INSERT_GLOBAL_SERVER
   //}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2012. 04. 25	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	_DECL_ON_FUNC( EGB_INCREASE_THE_PROTECTED_CRYSTAL_COUNT_NOT, int );
	DECL_ON_FUNC_NOPARAM( EGB_RESET_INCREASE_THE_PROTECTED_CRYSTAL_COUNT_NOT );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
	DECL_ON_FUNC( DBE_JACKPOT_EVENT_INIT_ACK );
	DECL_ON_FUNC( DBE_JACKPOT_EVENT_UPDATE_ACK );
#endif SERV_EVENT_JACKPOT
	//}}


#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	DECL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	DECL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK );
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK	
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	DECL_ON_FUNC( DBE_EVENT_UPDATE_ACK );
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER

	//{{ 2013. 05. 14	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
	DECL_ON_FUNC( DBE_CHECK_BLOCK_LIST_ACK );
#endif SERV_BLOCK_LIST
	//}}

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
	DECL_ON_FUNC( DBE_TIME_ENCHANT_EVENT_INFO_ACK );
	DECL_ON_FUNC( DBE_TIME_ENCHANT_EVENT_INFO_NOT );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-29	// 박세훈
	DECL_ON_FUNC( DBE_EXCHANGE_LIMIT_INFO_ACK );
	DECL_ON_FUNC( DBE_EXCHANGE_LIMIT_INFO_NOT );
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_GLOBAL_MISSION_MANAGER
	DECL_ON_FUNC( DBE_GET_GLOBAL_MISSION_INFO_ACK );
#endif SERV_GLOBAL_MISSION_MANAGER

private:
	boost::timer	m_kTimer;

	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
	boost::timer	m_kTimerRandomQuest;
#endif SERV_RANDOM_DAY_QUEST
	//}}
	//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
	// 셧다운제 적용 알림시간 목록
	std::vector< CTimeSpan >		m_vecShutDownMSGTime;
	boost::timer					m_kTimerShutDown;
#endif SERV_SHUTDOWN_SYSTEM
	//}}
	//{{ 공존의 축제 - 지진 효과 - 김민성
#ifdef SERV_EARTH_QUAKE_EFFECT
	boost::timer					m_kTimerEarthQuake;
	int								m_iNextEarthQuake;		// 초단위
#endif SERV_EARTH_QUAKE_EFFECT
	//}}
	//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
	std::map< int, std::set<UidType> > m_mapServerUIDList;	// ServerGroupID, ServerUID
#endif SERV_EVENT_JACKPOT
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	boost::timer					m_tTimeProcessCommunicationONOFF;
	bool							m_bServerRunningProcessCommunicationOnOff;
#endif //SERV_PROCESS_COMMUNICATION_KSMS

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
	int										m_iTimeEnchantEventUID;
	boost::timer							m_kTimeEnchantEventTimer;
	std::map<int, KTimeEnchantEventInfo>	m_mapTimeEnchantEventInfo;		// <m_iEventUID, ...>
	std::map<int, KTimeEnchantEventInfo>	m_mapBeginTimeEnchantEventInfo;	// <m_iEventUID, ...>
	std::set<int>							m_setEndTimeEnchantEventInfo;	// <m_iEventUID>
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-29	// 박세훈
	enum EXCHANGE_LIMIT_INFO
	{
		ELI_CHECK_TERM		= 60,
		ELI_QUANTITY_TERM	= 10,
	};
	typedef	std::list< KExchangeLimitInfo >	EXCHANGE_LIMIT_CONTAINER;	
	boost::timer				m_kExchangeLimitCheckTimer;
	int							m_iExchangeLimitUID;
	EXCHANGE_LIMIT_CONTAINER	m_listExchangeLimitInfo;		// 분류되지 않은 제한 정보
	EXCHANGE_LIMIT_CONTAINER	m_listBeginExchangeLimitInfo;	// 진행 중인 제한 정보
	bool						m_bExchangeLimitInfoChanged;
	boost::timer				m_kExchangeLimitQuantityTimer;
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_GLOBAL_MISSION_MANAGER
	boost::timer	m_kTimerGlobalMission;
#endif SERV_GLOBAL_MISSION_MANAGER
};

DefKObjectInline( KGlobalServer, KBaseServer );

//{{ 2013. 05. 29	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
template < class T >
void KGlobalServer::SendToGSUser( UidType nToGSUID, UidType nToGSUSer, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_USER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToGSUSer ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"타 게임서버로 패킷 보내기 실패.? 왜왜" )
			<< BUILD_LOG( nToGSUID )
			<< BUILD_LOG( nToGSUSer )			
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}
#endif SERV_BLOCK_LIST
//}}

//다른 게임서버로 자료를 보내기 위해
template < class T >
void KGlobalServer::SendToGameServer( unsigned short usEventID, const T& data ) const
{
	// 연결된 게임서버중 한군데만 전송
	UidType iGameServerUID = KBaseServer::GetKObj()->GetFirstActorKey();
	if( iGameServerUID <= 0 )
	{
		START_LOG( cwarn, L"접속된 게임서버가 없습니다." )
			<< BUILD_LOG( iGameServerUID )
			<< END_LOG;
		return;
	}

	UidType anTrace[2] = { iGameServerUID, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, iGameServerUID, anTrace, usEventID, data );
}

template < class T >
void KGlobalServer::BroadCastAllGS( unsigned short usEventID, const T& data ) const
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, usEventID, data );
}

//{{ 2010. 10. 19	최육사	SMS테스트
#ifdef SERV_SMS_TEST
template < class T >
void KGlobalServer::SendToSMSDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( GetPfID(), GetUID(), PI_GLOBAL_SMS_DB, 0, anTrace, usEventID, data );
}
#endif SERV_SMS_TEST
//}}

//#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
template < class T >
void KGlobalServer::SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_CHARACTER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToUnitUID ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"타 게임서버로 패킷 보내기 실패.? 왜왜" )
			<< BUILD_LOG( nToGSUID )
			<< BUILD_LOG( nToUnitUID )
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}
//#endif	// SERV_HERO_PVP_MANAGE_LIST

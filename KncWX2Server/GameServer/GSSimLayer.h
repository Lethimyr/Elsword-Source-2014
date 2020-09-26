#pragma once
#include "SimLayer.h"
#include "X2Data/XSLMain.h"

class KGSSimLayer : public KSimLayer
{
    NiDeclareRTTI;
    DeclDump;
    DeclToStringW;

	//{{ 2011. 04. 04  김민성   LFH
#ifdef SERV_LFH
	#define HEAP_SIZE 1024
#endif SERV_LFH
	//}}

public:

    enum SERVER_GROUP_NUM
    {
        SINGLE_SERVER_GROUP = 1,
        DOUBLE_SERVER_GROUP = 2,
    };

    KGSSimLayer(void);
    virtual ~KGSSimLayer(void);

    // derived from KSimLayer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();
    virtual void RegToLua();

	//{{ 임홍락 [2012.05.16] 서버 단일화 // SERV_USE_NATION_FLAG 필수 포함임
#ifdef SERV_UNITED_SERVER_EU
		std::string GetStrItemLuaName();
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
		std::string GetStrItemTransLuaName();
#endif // SERV_ITEM_LUA_TRANS_DEVIDE
		std::string GetStrFilterLuaName();
		std::string GetStrPvpNpcDataLua();
#endif SERV_UNITED_SERVER_EU
	//}}
			void SendNotMSG( IN char cNotifyType, IN const std::wstring& wstrNotMSG, IN int iCount );
			void SendNotMSG_GRP( const char* szNotMSG, int iCount = 1 );
			//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
			void SendNotMSG_ALL( const char* szNotMSG, int iCount = 1  );
			//}}
			void AddIP( const char* szAddIP );
			bool CheckIP( const char* szIP );

			void AddRepeatEvent( u_short usEventID, int iCnt );

			//{{ 2009. 9. 1  최육사		IOCP종료사유
#ifdef ABNORMAL_REASON
			void AddAbnormalDisconnectReason( DWORD dwReason );
			void SendToDBAbnormalReason( bool bFinal );
#endif ABNORMAL_REASON
			//}}
			//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
			bool GetUDPKickOff() { return m_bUDPKickUserOff; }
			void SetUDPKickOff( bool _bUDPKickOff) { m_bUDPKickUserOff = _bUDPKickOff; }
#endif UDP_CAN_NOT_SEND_USER_KICK
			//}}
			//{{ 2009. 12. 15  최육사	서버관리
	virtual void DumpToLogFile();
			//}}
	//{{ 2011. 04. 04  김민성   LFH
#ifdef SERV_LFH
	void		 Enable_LFH();
	void		 Dump_Blocks_In_All_Heaps();
#endif SERV_LFH
	//}}
	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	void		 TickDump_LUA();
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

#ifdef SERV_USE_XTRAP
	void ReloadXTRAP_MAP();
#ifdef SERV_XTRAP_MAP_LOADING_AUTOMATION
	int GetMapQuantity() { return m_setLoadedMaps.size(); }
	char* GetAllMapPointer() { return &m_vecUsingMap[0]; }
#endif //SERV_XTRAP_MAP_LOADING_AUTOMATION
#endif //SERV_USE_XTRAP

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
	void DeleteCommonFlag_AllGS_LUA( DWORD dwFlag );
	void AddCommonFlag_AllGS_LUA( DWORD dwFlag );
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH

    void SetServerGroupNum( int iGroupNum );
    int GetServerGroupNum();

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
			void SetTimeControlItem_Info(std::map< int, std::vector<KPacketGetItemOnOff> > _maptimeControl_Item) { m_map_TimeControl_Item = _maptimeControl_Item; }
			std::map< int, std::vector<KPacketGetItemOnOff> > GetTimeControlItem_Info() { return m_map_TimeControl_Item; }
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	void SetTimeControlCubeInItemMapping_Info(std::map< int, std::vector<KRandomItemMappingToInfoServer> > _maptimeControl_CubeInItemMapping) { m_map_TimeControl_CubeInItemMapping = _maptimeControl_CubeInItemMapping; }
	std::map< int, std::vector<KRandomItemMappingToInfoServer> > GetTimeControlCubeInItemMapping_Info() { return m_map_TimeControl_CubeInItemMapping; }
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

protected:
    std::vector< std::string >		m_vecIP;

	//클라이언트에서 보내는 방관련 패킷중 중복으로 날아오는 패킷체크를 위해
	//1 arg : event id
	//2 arg : repeat count
	std::map< u_short, int >		m_mapRepeatEvent;

	//{{ 2009. 9. 1  최육사		IOCP종료사유
#ifdef ABNORMAL_REASON
	std::map< DWORD, int >			m_mapAbnormalDisconnectReason;
	boost::timer					m_tAbnormalReasonTimer;
#endif ABNORMAL_REASON
	//}}	
	//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	bool							m_bUDPKickUserOff;
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}

#ifdef SERV_USE_XTRAP
#ifdef SERV_XTRAP_MAP_LOADING_AUTOMATION
	mutable KncCriticalSection      m_csXTrapReload;
	boost::timer					m_tXTrapReloadTimer;
	std::set<std::vector<char> >	m_setLoadedMaps;	// 현재 올라가 있는 맵인지 확인을 위한 정보
	std::vector<char>				m_vecUsingMap;		// 실제 메모리에 올라가는 정보
#endif //SERV_XTRAP_MAP_LOADING_AUTOMATION
#endif //SERV_USE_XTRAP
    int m_iServerGroupNum;
	
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	std::map< int, std::vector<KPacketGetItemOnOff> > m_map_TimeControl_Item;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	std::map< int, std::vector<KRandomItemMappingToInfoServer> > m_map_TimeControl_CubeInItemMapping;
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
};

DefKObjectInline( KGSSimLayer, KSimLayer );

#pragma once

#include <boost/timer.hpp>
#include <ToString.h>
#include "KncSmartPtr.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"


#define KStatisticsPtr KStatistics*
#undef KStatisticsPtr
SmartPointer( KStatistics );

class KStatistics
{
    DeclToStringW;
    DeclDump;

public:
#   undef _ENUM
#   define _ENUM( id ) id,
    enum
    {
#   include "StatisticsID_def.h"
    };

	// 아이템 통계
	enum StatInventory
	{
		SI_ITEM_TRADE = 0,		// 개인거래
		SI_ITEM_PERSONAL_SHOP,	// 개인상점
		SI_ITEM_TRADE_BOARD,	// 거래게시판
		SI_ITEM_BUY,			// 아이템 구매
		SI_ITEM_SELL,			// 아이템 판매
		SI_ITEM_USE,			// 아이템 사용
		SI_ITEM_MAKE,			// 제조
		SI_ITEM_FAILED,			// 제조 실패
		SI_ITEM_DROP,			// 아이템 드롭
		SI_ITEM_QUEST,			// 퀘스트 습득템
		SI_ITEM_EQUIP,			// 아이템 장착
		SI_ITEM_RANDOM,			// 랜덤 큐브 사용
		SI_ITEM_RESOLVE,		// 아이템 분해 시도 횟수
		SI_ITEM_R_RESULT,		// 아이템 분해 결과물
		SI_ITEM_MALL,			// 캐쉬 아이템		
		SI_ITEM_POST,			// 우체국 편지 첨부 아이템
		SI_ITEM_IDENTIFY,		// 속성석 감정
		SI_ITEM_EXCHANGE_S,		// 아이템 교환 [원래것]
		SI_ITEM_EXCHANGE_R,		// 아이템 교환 [바뀐것]
		//{{ 2010. 05. 20  최육사	아이템 버리기 통계
		SI_ITEM_DISCARD,		// 아이템 버리기
		//}}
		//{{ 2010. 9. 14	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
		SI_ITEM_PET_FEED,		// 펫 먹이
//#endif SERV_PET_SYSTEM
		//}}
		//{{ 2011. 04. 26	최육사	대리 상인
//#ifdef SERV_PSHOP_AGENCY
		SI_ITEM_REG_PSHOP,		// 대리상인 Reg Item
		SI_ITEM_BUY_PSHOP_AGENCY, // 대리상인 아이템 구매
		SI_ITEM_PICK_UP_PSHOP,	// 대리상인 PickUp
//#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
//#ifdef SERV_NEXON_SESSION_PROTOCOL
		SI_ITEM_RETRANINING_REWARD,		// 이탈 방지 보상 아이템 지급
//#endif SERV_NEXON_SESSION_PROTOCOL
		//}} 
//#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
		SI_ITEM_EXTRACT_S,		// 아이템 추출 [원래것]
		SI_ITEM_EXTRACT_R,		// 아이템 추출 [바뀐것]
		SI_ITEM_USE_FINALITY_SKILL,		// 궁극기 사용	
//#endif // SERV_FINALITY_SKILL_SYSTEM

		SI_ITEM_MAX,
	};

	// 대전 일일 통계
	enum StatLocalPvp
	{
		SLP_CREATE_COUNT = 0,
		SLP_UNITCLASS_ELSWORD,
		SLP_UNITCLASS_AISHA,
		SLP_UNITCLASS_RENA,
		SLP_UNITCLASS_RAVEN_FIGHTER,		
		SLP_UNITCLASS_EVE_NASOD,
		SLP_UNITCLASS_SWORD_KNIGHT,
		SLP_UNITCLASS_MAGIC_KNIGHT,
		SLP_UNITCLASS_COMBAT_RANGER,
		SLP_UNITCLASS_SNIPING_RANGER,
		SLP_UNITCLASS_HIGH_MAGICIAN,
		SLP_UNITCLASS_DARK_MAGICIAN,
		SLP_UNITCLASS_SOUL_TAKER,
		SLP_UNITCLASS_OVER_TAKER,		
		SLP_UNITCLASS_EXOTIC_GEAR,
		SLP_UNITCLASS_ARCHITECTURE,
		SLP_INTRUDER,
		SLP_GIVE_UP,
		SLP_GET_VP,
		SLP_GET_EXP,
		SLP_TEAM_MATCH_MC,
		SLP_TEAM_MATCH_PT,
		SLP_TEAM_DEATH_MC,
		SLP_TEAM_DEATH_PT,
		SLP_SURVIVAL_MC,
		SLP_SURVIVAL_PT,
//#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		SLP_UNITCLASS_ARA,
//#endif // SERV_ARA_CHANGE_CLASS_SECOND

		SLP_MAX
	};

	// 대전 맵 통계
	enum ESIColumn_PvpMap
	{
		eSIColPvpMap_PlayCount = 0,

		eSIColPvpMap_Max,
	};

	// FPS 통계
	enum ESIColumn_Fps
	{
		eSIColFps_0_20 = 0,
		eSIColFps_21_30,
		eSIColFps_31_40,
		eSIColFps_41_50,
		eSIColFps_51_60,
		eSIColFps_61_70,
		eSIColFps_71_80,
		eSIColFps_81_90,
		eSIColFps_91_unlimited,

		eSIColFps_Max,
	};

	// 근성도 통계
	enum ESIColumn_Spirit
	{
		eSIColSpirit_Play = 0,
		eSIColSpirit_UseSpirit,
		eSIColSpirit_SpiritZero,

		eSIColSpirit_Max,
	};

	// 일일 강화 통계
	enum ESIColumn_Enchant
	{
		eSIColEnchant_Lv_1_Succ = 0,
		eSIColEnchant_Lv_1_Fail,
		eSIColEnchant_Lv_2_Succ,
		eSIColEnchant_Lv_2_Fail,
		eSIColEnchant_Lv_3_Succ,
		eSIColEnchant_Lv_3_Fail,
		eSIColEnchant_Lv_4_Succ,
		eSIColEnchant_Lv_4_Fail,
		eSIColEnchant_Lv_5_Succ,
		eSIColEnchant_Lv_5_Fail,
		eSIColEnchant_Lv_6_Succ,
		eSIColEnchant_Lv_6_Fail,
		eSIColEnchant_Lv_7_Succ,
		eSIColEnchant_Lv_7_Fail,
		eSIColEnchant_Lv_8_Succ,
		eSIColEnchant_Lv_8_Fail,
		eSIColEnchant_Lv_9_Succ,
		eSIColEnchant_Lv_9_Fail,

		eSIColEnchant_Max,
	};

	// 부활석 통계 [DB]
	enum ESIColumn_DB_ResStone
	{
		eSIColResStone_PlayerCount = 0,
		eSIColResStone_UseCount,

		eSIColResStone_Max,
	};

	// 일일 ED 통계 [DB]
	enum ESIColumn_DB_ED
	{
		eSIColDB_ED_PDungeon				= 0,
		eSIColDB_ED_PTraining				= 1,
		eSIColDB_ED_PQuest					= 2,
		eSIColDB_ED_PItemSell				= 3,
		eSIColDB_ED_PItemTrade				= 4,
		eSIColDB_ED_PPersonalShop			= 5,
		eSIColDB_ED_PPostLetter				= 6,
		eSIColDB_ED_MItemBuy				= 7,
		eSIColDB_ED_MItemTrade				= 8,
		eSIColDB_ED_MItemRepair				= 9,
		eSIColDB_ED_MManufacture			= 10,
		eSIColDB_ED_MDunPenalty				= 11,
		eSIColDB_ED_MPersonalShop			= 12,
		eSIColDB_ED_MOpenPShop				= 13,
		eSIColDB_ED_MEnchant				= 14,
		eSIColDB_ED_MSocket					= 15,
		eSIColDB_ED_MPostLetter				= 16,
		eSIColDB_ED_MSendLetterBasic		= 17,
		eSIColDB_ED_MSendLetterItem			= 18,
		eSIColDB_ED_MAttribEnchant			= 19,
		eSIColDB_ED_MSellTaxPShop			= 20,
		eSIColDB_ED_MBuyTaxPShop			= 21,
		//{{ 2011. 05. 11	최육사	대리상인
//#ifdef SERV_PSHOP_AGENCY
		eSIColDB_ED_MPShopAgency			= 22,
		eSIColDB_ED_MBuyTaxPShopAgency		= 23,
		eSIColDB_ED_PPShopPickUp			= 24,
//#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
		eSIColDB_ED_POpenCube				= 25,
//#endif SERV_CUBE_OPEN_ED_CONDITION
		//}

//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-05-08	// 박세훈
		eSIColDB_ED_PBattleField			= 26,		// 배틀 필드에서 획득한 ED
		eSIColDB_ED_MCoboExpressTicketED	= 27,		// 마을 이동서를 통해 소모한 ED
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
//#ifdef SERV_MULTI_RESOLVE // 해외팀 추가 분해 수수료
		eSIColDB_ED_MItemResolve              = 28,
//#endif SERV_MULTI_RESOLVE

//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
		eSIColDB_ED_MItemEvaluate			= 29,		// 아이템 감정으로 소모한 ED
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
        
//#ifdef SERV_GOOD_ELSWORD // 작업날짜 2013-11-08 , ED로 인벤토리 확장, ED 로 은행 확장
        eSIColDB_ED_ExpandInventory         = 30, // 인벤토리
        eSIColDB_ED_ExpandBank              = 31, // 은행
//#endif SERV_GOOD_ELSWORD

//#ifdef SERV_SKILL_PAGE_SYSTEM
		eSIColDB_ED_ExpandSkillPage         = 32, // 스킬 페이지
//#endif // SERV_SKILL_PAGE_SYSTEM

		eSIColDB_ED_Max,
	};

	// 퀘스트 통계 [DB]
	enum ESIColumn_DB_Quest
	{
		eSIColQuest_Accept = 0,
		eSIColQuest_GiveUp,
		eSIColQuest_Complete,
		eSIColQuest_AcceptLevel,
		eSIColQuest_GiveUpLevel,
		eSIColQuest_CompleteLevel,

		eSIColQuest_Max,
	};	

	// 종료 사유 통계
	enum ESIColumn_Disconnect_Reason
	{
		eSIColDR_No_Reason							=  0,	// [0]  이유 없음
		eSIColDR_Unknown							=  1,	// [1]  Unknown
		eSIColDR_HackShield_Auth_Fail				=  2,	// [2]  HackShield 인증 실패 (클라이언트 응답 없음)
		eSIColDR_PCBang_Auth_Delay					=  3,	// [3]  PC방 인증 지연
		eSIColDR_Diff_Main_Ver						=  4,	// [4]  메인버전 다름
		eSIColDR_Limit_User_Num						=  5,	// [5]  수용 인원 초과
		eSIColDR_Nexon_Auth_Fail					=  6,	// [6]  넥슨 인증 실패
		eSIColDR_Same_Server_Connect				=  7,	// [7]  동일 서버 이중 접속
		eSIColDR_Account_Block						=  8,	// [8]  계정 블록
		eSIColDR_IP_Block							=  9,	// [9]  IP 제한
		eSIColDR_NProtect_Auth_First				= 10,	// [10] NProtect 처음 인증
		eSIColDR_PCBang_Auth_Fail					= 11,	// [11] PC방 인증 실패
		eSIColDR_PCBang_Auth_Wrong					= 12,	// [12] PC방 인증 결과 이상
		eSIColDR_Internal_Account					= 13,	// [13] 사내 계정 생성
		eSIColDR_Normal_Disconnect					= 14,	// [14] 정상 종료
		eSIColDR_Client_Crash						= 15,	// [15] 클라이언트 크래쉬
		eSIColDR_Client_Hacking						= 16,	// [16] 클라이언트 해킹
		eSIColDR_HackShield_Wrong_Ack				= 17,	// [17] HackShield 인증 실패 (응답 틀림)
		eSIColDR_PCBang_Auth_Server					= 18,	// [18] PC방 인증 서버의 종료 요청
		eSIColDR_Server_Shutdown					= 19,	// [19] 서버 Shutdown
		eSIColDR_Session_Close						= 20,	// [20] 접속 종료 감지 (Session)
		eSIColDR_WSAECONNRESET						= 21,	// [21] WSAECONNRESET (Socket Object)
		eSIColDR_WSARecvFailed						= 22,	// [22] WSARecvFailed (Socket Object)
		eSIColDR_WSASendFailed_OnSendComp			= 23,	// [23] WSASendFailed (Socket Object) - OnSendCompleted()
		eSIColDR_SendBufferFull						= 24,	// [24] SendBufferFull (Socket Object)
		eSIColDR_WSASendFailed						= 25,	// [25] WSASendFailed (Socket Object) - SendData()
		eSIColDR_GM_Kick_User						= 26,	// [26] 운영자 강제 접속 종료
		eSIColDR_Diff_Server_Connect				= 27,	// [27] 타 서버간 이중접속
		eSIColDR_User_Reg_Error						= 28,	// [28] 유저 등록 중 에러
		eSIColDR_Heart_Beat							= 29,	// [29] 하트 비트
		eSIColDR_IOCP_GetStatus						= 30,	// [30] IOCP_GetStatus Closed by local machine
		eSIColDR_nProtect_Hacking					= 31,	// [31] nProtect 해킹 감지
		eSIColDR_Channel_Change						= 32,	// [32] 채널이동으로 인한 접속종료
		eSIColDR_Channel_Change_Fail				= 33,	// [33] 채널이동 실패로 인한 접속종료
		//{{ 2009. 10. 22  최육사	트래픽 공격 방지
		eSIColDR_Traffic_Attack						= 34,	// [34] 트래픽 유발로 인한 접속종료
		//}}
		//{{ 2010. 06. 22  최육사	계정블럭기능
		eSIColDR_GameServer_Account_Block			= 35,	// [35] 게임서버 계정 블록
		//}}
		//{{ 2011. 08. 09	최육사	머신ID 블럭기능
//#ifdef SERV_MACHINE_ID_BLOCK
		eSIColDR_MachineID_Block					= 36,	// [36] 머신ID 블럭
//#endif SERV_MACHINE_ID_BLOCK
		//}}
		//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
//#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
		eSIColDR_Same_MachineID_Connect				= 37,	// [37] 머신ID 중복 접속
//#endif SERV_MACHINE_ID_DUPLICATE_CHECK
		//}}
		//{{ 2011. 09. 29  김민성	셧다운제 도입
//#ifdef SERV_SHUTDOWN_SYSTEM
		eSIColDR_Policy_Shutdown_Disconnect			= 38,	// [38] 셧다운제로 인한 접속 불가
//#endif SERV_SHUTDOWN_SYSTEM
		//}}
		eSlColDR_Wrong_Account_or_Password			= 39,	// [39] 아이디 또는 비밀번호 틀림 (TW)
		//{{ 허상형 : [2011/2/16/] //	SHA 체크섬
		//#ifdef SERV_USE_CHECKSUM_WITH_SHA
		eSlColDR_Wrong_SHA_Checksum					= 40,	// [40] SHA 체크섬 틀림 (TW)
		//#endif SERV_USE_CHECKSUM_WITH_SHA
		//}} 허상형 : [2011/2/16/] //	SHA 체크섬

		eSIColDR_Account_Invalid_Selling_Kill		= 41,	// [41] 팔기불능 아이템을 판다고 패킷날리는 핵유저 강제 종료 (TW) 
		eSIColDR_Clear_Time_Hack_User				= 42,	// [42] 클리어타임 5초이하 핵 의심 유저로 판별 (TW)

		//{{ 2011.03.20 lygan_조성욱 // HProtect
		//#ifdef SERV_USE_HPROTECT
		eSIColDR_Error_Check_Alive1					= 43,	// [43] A type heartbeat validation error
		eSIColDR_Error_Check_Alive2					= 44,	// [44] B type heartbeat validation error
		eSIColDR_Error_Wait_Check_Time_Out			= 45,	// [45] The client did not respond to packet too long
		eSIColDR_Error_Client_Speed					= 46,	// [46] Client speed abnormally
		eSIColDR_Error_Client_Scan_Result			= 47,	// [47] wParam point to TOOLSSCANRESULT
		eSIColDR_Error_Client_Version				= 48,	// [48] unsigned int wParam type allows for the smallest version of the type unsigned int lParam is the client version number
		eSIColDR_Error_Server_Lost_File				= 49,	// [49] wParam point to file path
		eSIColDR_Error_Client_Protocol				= 50,	// [50] Agreement send data packets when checking failure
		eSIColDR_Error_Client_Sign_Hash				= 51,	// [51] 클라이언트 특징 파일에 에러가있음
		eSIColDR_Error_Wait_Client_Version_Time_Out = 52,	// [52] In 2 minutes, fail to get the client version, unable to initialize the protection system
		//#endif SERV_USE_HPROTECT
		//}}
		//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
		//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		eSIColDR_Check_ServerSN_In_GameServer		= 53,	// [53] 게임서버에서 ServerSN 검사로 인하여 강제 종료
		//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		//}}
		//{{ 2012. 02. 01	최육사	recv buffer full
//#ifdef SERV_CHECK_RECV_BUFFER_FULL
		eSIColDR_RecvBufferFull						= 54,	// [54] 수신된 패킷의 길이가 recv buffer크기를 넘어서는 경우
//#endif SERV_CHECK_RECV_BUFFER_FULL
		//}}		
		//{{ 2012. 02. 14    김민성   넥슨 안심 로그인 - 목표 위치기반 로그인
//#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
		eSIColDR_PeaceOfMindLogin					= 55,	// [55] 넥슨 안심 로그인으로 블럭 처리된 유저
//#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
		//}}
		//{{ 2012. 05. 23	김민성       판매 불가 아이템 판매 시도 해킹 유저로 판단
//#ifdef SERV_IMPOSSIBLE_SELL_ITEM
		eSIColDR_ImpossibleSellItem					= 56,	// [56] 판매 불가 아이템 팔기
//#endif SERV_IMPOSSIBLE_SELL_ITEM
		//}}
//#ifdef SERV_NPROTECT_CS_AUTH_30
		eSIColDR_nProtect_Error                     = 57,    // [57] nProtect 에러
//#endif SERV_NPROTECT_CS_AUTH_30
//{{
//#ifdef SERV_MOVE_DATA_QUE2BUF_FAIL  
		eSIColDR_MovDataQue2Buf_Failed              = 58,    // [58] MovDataQue2Buf 부분에서 비정상적인 버퍼 크기로 인해 게임서버 크래쉬 나는 부분 수정
//#endif //SERV_MOVE_DATA_QUE2BUF_FAIL
//}}
//#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
		eSIColDR_LanBugOutDisconnect				= 59,    // [59] 랜선렉 감시에 걸렸음에도 이를 무시하여 접속 종료 처리한 경우
//#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

//#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
		eSIColDR_FieldWorkingCheck					= 60,    // [60] 필드 작업장 체크 기능
//#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

//#ifdef SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT // 작업날짜: 2013-09-04 // 오현빈
		eSIColDR_InvlideCheckKomScript				= 61,    // [61] CheckKom.xml 스크립트 파싱 오류
//#endif // SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT

//#ifdef SERV_KOM_FILE_CHECK_ADVANCED		// 작업날짜: 2013-11-06 // 임홍락
		eSIColDR_InvaildKomDetect					= 62,   // [62] Kom 변조 감지
//#endif SERV_KOM_FILE_CHECK_ADVANCED

		eSIColDR_Num,
	};

	// 종료 상태 통계
	enum ESIColumn_Disconnect_UserFSM
	{		
		eSIColDU_S_INIT = 0,
		eSIColDU_S_UID_UPDATED,
		eSIColDU_S_LOGINED,
		eSIColDU_S_SERVER_SELECT,
		eSIColDU_S_UNIT_SELECT,
		eSIColDU_S_VILLAGE_MAP,
		eSIColDU_S_LOCAL_MAP,
		eSIColDU_S_WORLD_MAP,
		eSIColDU_S_CHANNEL,
		eSIColDU_S_ROOM,
		eSIColDU_S_CHECK_SECOND_PW,

		eSIColDU_Num,
	};

	//{{ 2011. 02. 28	최육사	사내 통합 통계
#ifdef SERV_CHAR_CREATE_DEL_STAT
	// 캐릭터 통계
	enum ESIColumn_DB_Character
	{
		eSIColDB_Char_Create_Count = 0,
		eSIColDB_Char_Delete_Count,

		eSIColDB_Num,
	};
#endif SERV_CHAR_CREATE_DEL_STAT
	//}}

	// - - - - - - - - - - - - - - - - - - - - - - - - -

    KStatistics();
    virtual ~KStatistics();

    float GetFlushGap()             { return m_fFlushGap; }
    bool  IsWriteToDB()             { return m_bWriteToDB; }
    static const wchar_t* GetIDStr( int iStatisticsID );

	void Init( int iStatisticsID, float fFlushGap, bool bWriteToDB );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	void SetFlushTime( IN const int iHour );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

    void Tick();

    void IncreaseCount( const KStatisticsKey& kKey, int iIndex, int iCount );
    void IncreaseCount( const KStatisticsKey& kKey, const std::vector< __int64 >& vecCount );
    void FlushData();
	void WriteLocalLogData( std::wfstream& fout );	
	void ClearLocalLogData();

	void InitLocalLogKeys();

	static int GetDisconnectUserFSM( int iStateID );
	static bool GetUnitClassPvpStatColumn( char cUnitClass, int& iColumn );
	static const wchar_t* GetDissconnectReasonString( int iReason );

protected:
	unsigned short GetEventID();

protected:
    int m_iStatisticsID;
    KStatisticsData m_kStatisticsData;
    boost::timer m_kUpdateTimer;
    float m_fFlushGap;
    bool m_bWriteToDB;
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	bool m_bTimeFlush;
	CTime m_tFlushTime;
	boost::timer m_kTimeFlushTimer;
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	static const wchar_t* ms_szStatistics[];
};




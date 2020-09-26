#pragma once
#include "Actor.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"

#include <windows.h>    // ::GetTickCount()
#include "CacheData.h"
#include <boost/timer.hpp>
//{{ 2010. 01. 15  최육사	boost allocator
#ifdef SERV_BOOST_ALLOCATOR
	#include <ObjectPool/BoostAllocator.h>
#endif SERV_BOOST_ALLOCATOR
//}}

//----------- 2013.02.27 조효진	해외 기본 구조 작업 (SERV_GLOBAL_BASE)-------------------------------//

//{{ 2012.11.13 조효진 닉네임 관련 체크 검사 시 대소문자 구별 안하도록 수정
//#ifdef SERV_STRING_CHECK_IGNORE_CASE
#include <boost/algorithm/string.hpp>
//#endif SERV_STRING_CHECK_IGNORE_CASE
//}}

#ifdef SERV_NPROTECT_CS_AUTH_30
#include "./nProtect/ggsrv30.h"
#endif SERV_NPROTECT_CS_AUTH_30

#ifdef SERV_NEW_PUNISHMENT
//{{ 박교현 [2010/08/23] 처벌 추가
#include "PunishmentManager.h"
//}}
#endif SERV_NEW_PUNISHMENT

//{{ 2010. 10. 04 조효진	빌링 관련 추가. 대만 코드 참조
#ifdef SERV_GLOBAL_BILLING
#include "BillManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/KncSendToPublisher.h"
#endif // SERV_GLOBAL_BILLING
//}}
//---------------------------------------------------------------------------------------------------//

#include "Inventory.h"
//{{ 2009. 5. 30  최육사	임시 인벤토리
#include "TempInventory.h"
//}}
#include "UserQuestManager.h"
//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	#include "UserAccountQuestManager.h"
#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}
#include "UserSkillTree.h"
#include "CharLog.h"
//#include "GGAuth.h"
//{{ 2009. 8. 18  최육사	hack shield
#include "HackShield/HSAuth.h"
//}}
//{{ 2010. 12. 13. 지헌 : XTRAP 서버 - CXTrapCSM 포함
#ifdef SERV_USE_XTRAP
#include "XTrapCSM.h"
#define CSFILE_NUM				2		// 사용할 MapFile 개수
#define XTRAP_CS4_BUFSIZE_MAP	13000	// MapFile사이즈크기 (고정)
#endif SERV_USE_XTRAP
//}}
#include "UserStatistics.h"
#include "MaxProcessTime.h"
#include "UserGameOption.h"
#include "UserTutorialInfo.h"
#include "UserWishList.h"
#include "UserCashInventory.h"
#include "UserLetterBox.h"
#include "UserTitleManager.h"
//{{ 2009. 7. 7  최육사		랭킹개편
#include "UserRankingManager.h"
//}}
#include "RepeatEventFilter/RepeatEventFilter.h"
//{{ 2009. 6. 10  최육사	타이머 매니저
#include "TimerManager.h"
//}}
//{{ 2009. 9. 13  최육사	유저 로그
#include "UserLogManager.h"
//}}
//{{ 2009. 10. 13  최육사	길드
#ifdef GUILD_TEST
	#include "UserGuildManager.h"
#endif GUILD_TEST
//}}
//{{ 2009. 10. 26  최육사	PC방이벤트
#include "UserEventManager.h"
//}}
//{{ 2010. 01. 08  최육사	추천인리스트
#include "UserRecommendManager.h"
//}}
//{{ 2010. 01. 29  최육사	PC방 상용화
#include "UserPcBangManager.h"
//}}
//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	#include "UserHackingManager.h"
#endif SERV_HACKING_TOOL_LIST
//}}
//{{ 2010. 04. 19  최육사	서버 코드 정리
#include "AbuserLog/KAbuserLogManager.h"
//}}

//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
	#include "AntiAddictionManager.h"
#endif SERV_ANTI_ADDICTION_SYSTEM
//}} 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템

//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	#include "UserAbuserManager.h"
#endif SERV_USER_ABUSER_MANAGER
//}}
//{{ 2010. 07. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	#include "UserPetManager.h"
#endif SERV_PET_SYSTEM
//}}
//{{ 2011. 04. 11	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	#include "UserPersonalShopManager.h"
#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
	#include "UserUnitSelectRewardManager.h"
#endif SERV_SECOND_SECURITY
//}}

//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
#include "ComeBackUserManager.h"
#endif SERV_COME_BACK_USER_REWARD
//}}

//{{ 2011. 06. 21	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	#include "UserPvpManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_BLOCK
	#include "UserSecurityManager.h"
#endif SERV_MACHINE_ID_BLOCK
//}}

//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	#include "UserDungeonManager.h"
#endif SERV_NEW_HENIR_TEST
//}}
//{{ 2011. 12. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "UserPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2012. 02. 29	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "UserSpiritManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "UserServerBuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}}
//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "UserUnitManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2012. 12. 5	최육사		옵저버 대전 난입 모드
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
	#include "UserCheatManager.h"
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
//}}
//{{ 2012. 09. 22	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	#include "Temp.h"
#endif SERV_EVENT_BINGO
//}}
//{{ 2012. 09. 28	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	#include "UserEDManager.h"
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}
//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	#include "UserLocalRankingInfo.h"
#endif SERV_LOCAL_RANKING_SYSTEM
//}}
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-20
	#include "UserRidingPetManager.h"
#endif	// SERV_RIDING_PET_SYSTM
//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	#include "UserRelationshipManager.h"
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef	SERV_AFTER_WORK_STORAGE// 적용날짜: 2013-04-20
	#include "AfterWorkStorage.h"
#endif	// SERV_AFTER_WORK_STORAGE

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-10	// 박세훈
	#include "UserJumpingCharacterManager.h"
#endif // SERV_JUMPING_CHARACTER

#ifdef _DEBUG
#define _VERIFY_POINTER( pointer, ret ) \
	if( !_CrtIsValidHeapPointer( pointer ) ) \
	{ \
		START_LOG( cerr, L#pointer L"포인터 값 이상. (state:" << GetStateIDString() << L", name:" << m_strName << L")" ); \
		return ret; \
	}
#else
#define _VERIFY_POINTER( pointer, ret ) \
	if( pointer == NULL ) \
	{ \
		START_LOG( cerr, L#pointer L"포인터 값 이상. (state:" << GetStateIDString() << L", name:" << m_strName << L")" ); \
		return ret; \
	}
#endif //#ifdef _DEBUG
#define VERIFY_POINTER( pointer )   _VERIFY_POINTER( pointer, (void)0 )

//{{ 2010. 9. 2	최육사	중폭 패킷 필터 개선
#define _VERIFY_STATE_REPEAT_FILTER( loglevel, varg, reqEventid, ackEventid, ackPacket ) \
	if( m_kRepeatEventFilter.CheckReqEvent( reqEventid, ackEventid ) == false ) \
	{ \
		START_LOG( cwarn, L"Repeat Event Filtering!" ) \
			<< BUILD_LOG( GetCharUID() ) \
			<< BUILD_LOG( KEvent::GetIDStr( reqEventid ) ); \
		return; \
	} \
	if( !m_pkCurrentState->CheckState varg ) \
	{ \
		START_LOG( loglevel, L"상태 오류. [return ack] Name : " << m_strName ) \
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
			<< L"    허용 상태 : "L ## #varg << END_LOG; \
		ackPacket kAck; \
		kAck.m_iOK = NetError::ERR_WRONG_STATE_00; \
		SendPacket( ackEventid, kAck ); \
		return; \
	}

#define VERIFY_STATE_REPEAT_FILTER( varg, reqEventid, ackEventid )			_VERIFY_STATE_REPEAT_FILTER( cerr, varg, reqEventid, ackEventid, K##ackEventid )
#define VERIFY_STATE_WARN_REPEAT_FILTER( varg, reqEventid, ackEventid )		_VERIFY_STATE_REPEAT_FILTER( cwarn, varg, reqEventid, ackEventid, K##ackEventid )
//}}

//{{ 2012. 02. 22	박세훈	길드 이름 변경권

/*

목적: EGS_CHANGE_GUILD_NAME_REQ는 반드시 EGS_CHANGE_GUILD_NAME_CHECK를 정상적으로 통과한 뒤에만 작동 하게끔 한다.

1. EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_CHECK_REQ 를 중복 필터를 건다.
2. 중복 검사가 끝난 후,
m_iOK != NetError::NET_OK 이면
REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );를 통해	EGS_CHANGE_GUILD_NAME_REQ를 해제하고,
SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket_ );를 통해			EGS_CHANGE_GUILD_NAME_CHECK_REQ를 해제한다.

m_iOK == NetError::NET_OK 이면
SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket_ );를 통해			EGS_CHANGE_GUILD_NAME_CHECK_REQ만 해제한다.

3. 클라이언트에서 EGS_CHANGE_GUILD_NAME_REQ를 보내왔을 때,
EGS_CHANGE_GUILD_NAME_CHECK_REQ는 걸려 있어선 안되며, 동시에 EGS_CHANGE_GUILD_NAME_REQ는 걸려 있어야만 한다.

- EGS_CHANGE_GUILD_NAME_CHECK 없이 EGS_CHANGE_GUILD_NAME_REQ를 조작하여 보내는 경우를 막았다.
- EGS_CHANGE_GUILD_NAME_CHECK_ACK를 받기 전에 조작된 EGS_CHANGE_GUILD_NAME_REQ를 보내는 경우를 막았다.

프로세스 시작 시에 두개 이상의 조건을 걸고, 처리하기 위함

_JUST_VERIFY_STATE_REPEAT_FILTER : 체크만 한다. reqEventid에 해당하는 정보가 없어도 등록하지 않는다.
_NEGATIVE_VERIFY_STATE_REPEAT_FILTER : !_JUST_VERIFY_STATE_REPEAT_FILTER 와 동일하다. reqEventid에 해당하는 정보가 없을 경우의 처리가 달라질 것을 염두에 두고 만들었다.
*/

#ifdef SERV_GUILD_CHANGE_NAME
#define _JUST_VERIFY_STATE_REPEAT_FILTER( loglevel, varg, reqEventid, ackEventid, ackPacket ) \
	if( m_kRepeatEventFilter.JustVerifyReqEvent( reqEventid ) == false ) \
	{ \
	START_LOG( cwarn, L"Just Verify Event Filtering!" ) \
	<< BUILD_LOG( GetCharUID() ) \
	<< BUILD_LOG( KEvent::GetIDStr( reqEventid ) ); \
	return; \
	} \
	if( !m_pkCurrentState->CheckState varg ) \
	{ \
	START_LOG( loglevel, L"상태 오류. [return ack] Name : " << m_strName ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #varg << END_LOG; \
	ackPacket kAck; \
	kAck.m_iOK = NetError::ERR_WRONG_STATE_00; \
	SendPacket( ackEventid, kAck ); \
	return; \
	}

#define JUST_VERIFY_STATE_REPEAT_FILTER( varg, reqEventid, ackEventid )			_JUST_VERIFY_STATE_REPEAT_FILTER( cerr, varg, reqEventid, ackEventid, K##ackEventid )
#define JUST_VERIFY_STATE_WARN_REPEAT_FILTER( varg, reqEventid, ackEventid )	_JUST_VERIFY_STATE_REPEAT_FILTER( cwarn, varg, reqEventid, ackEventid, K##ackEventid )

#define _NEGATIVE_VERIFY_STATE_REPEAT_FILTER( loglevel, varg, reqEventid, ackEventid, ackPacket ) \
	if( m_kRepeatEventFilter.NegativeCheckReqEvent( reqEventid ) == false ) \
	{ \
	START_LOG( cwarn, L"Negative Event Filtering!" ) \
	<< BUILD_LOG( GetCharUID() ) \
	<< BUILD_LOG( KEvent::GetIDStr( reqEventid ) ); \
	return; \
	} \
	if( !m_pkCurrentState->CheckState varg ) \
	{ \
	START_LOG( loglevel, L"상태 오류. [return ack] Name : " << m_strName ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #varg << END_LOG; \
	ackPacket kAck; \
	kAck.m_iOK = NetError::ERR_WRONG_STATE_00; \
	SendPacket( ackEventid, kAck ); \
	return; \
	}

#define NEGATIVE_VERIFY_STATE_REPEAT_FILTER( varg, reqEventid, ackEventid )			_NEGATIVE_VERIFY_STATE_REPEAT_FILTER( cerr, varg, reqEventid, ackEventid, K##ackEventid )
#define NEGATIVE_VERIFY_STATE_WARN_REPEAT_FILTER( varg, reqEventid, ackEventid )	_NEGATIVE_VERIFY_STATE_REPEAT_FILTER( cwarn, varg, reqEventid, ackEventid, K##ackEventid )

#define REMOVE_VERIFY_STATE_REPEAT_FILTER( reqEventid, ackEventid )								m_kRepeatEventFilter.RemoveReqEvent( reqEventid, ackEventid );

#endif SERV_GUILD_CHANGE_NAME
//}}


SmartPointer(KGSUser);

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BOOST_ALLOCATOR
BOOST_ALOC_SYNC_CLASS_INHERITANCE( KGSUser, public KActor )
#else
class KGSUser : public KActor
#endif SERV_BOOST_ALLOCATOR
//////////////////////////////////////////////////////////////////////////
{
	friend class KUserQuestManager;	// 연관 함수가 많아서 아직은 없애지 못함.
	//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	friend class KUserAccountQuestManager;	// 연관 함수가 많아서 아직은 없애지 못함.
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}
	friend class KPartyManager;		// 점차 코드 리팩토링을 해서 friend를 제거해야함.
	//{{ 2010. 01. 28  최육사	통합파티
#ifdef SERV_CHANNEL_PARTY
	friend class KPartyListManager;	// 통합파티
#endif SERV_CHANNEL_PARTY
	//}}

    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KGSUser(void);
    virtual ~KGSUser(void);

    enum 
	{
		SEARCH_UNIT_REFRESH_TIME_LIMIT = 3,		
		KNM_NEW_FRIEND_REFRESH_TIME_LIMIT = 3,
    };

    enum ENUM_TIMER
    {
        TM_DB_UPDATE = 0,						// DB 업데이트 주기
        TM_WHISPER_TICK,
        TM_ROOM,
		TM_SEARCH_UNIT_REFRESH,		
		TM_KNM_NEW_FRIEND_REFRESH,
        TM_INIT,
        TM_UID_UPDATED,		
		TM_UNIT_DURATION,
		TM_SQUARE_PLAY,
        TM_CHECK_ITEM_EXPIRATION,
		TM_RESOLVE_JACKPOT,
		TM_SKILL_SLOT_EXPIRATION,		
		TM_EVENT_CHECK,							// 시간 이벤트 체크 타이머
		TM_CASH_RES_STONE,						// 실시간 부활석 연속 사용 제한 타이머
		TM_CHECK_TITLE_EXPIRATION,				// 칭호 기간제 검사 타이머
		TM_CHECK_CASH_SKILL_POINT_EXPIRATION,	// cash skill point 타이머
		TM_CHECK_AUTO_PAYMENT,					// 자동결제 체크 타이머
		TM_FIELD_SYNC_GAP,						// 필드 싱크 패킷 간격 타이머
		//{{ 2010. 02. 16  최육사	해킹툴 리스트
		TM_HACKING_TOOL_LIST,					// 해킹툴 리스트
		//}}
#ifdef DEF_TRADE_BOARD
		TM_REFRESH_TRADE_BOARD,					// 거래 게시판 검색 쿨타임
#endif DEF_TRADE_BOARD
		//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
		TM_MONITORING_LETTER,
		TM_MONITORING_TRADE,
#endif SERV_MONITORING_LETTER_AND_TRADE
		//}}
		//{{ 2011. 04. 26	최육사	deserialize 실패 종료
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
		TM_DESERIALIZE_FAIL,
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
		//}}
		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		TM_COME_BACK_USER,
#endif SERV_COME_BACK_USER_REWARD
		//}} 
		//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
		TM_HENIR_REWARD_EVENT,
#endif SERV_NEW_HENIR_TEST
		//}}
		//{{ 2012. 02. 21	김민성	인벤토리 정렬
#ifdef SERV_SORT_CATEGORY_ITEM 
		TM_SORT_INVENTORY,						// 아이템 정렬 기능 쿨타임
#endif SERV_SORT_CATEGORY_ITEM 
		//}}
		//{{ 2012. 02. 27	김민성	인벤토리 정렬	- 무거운 기능을 가볍게 변경
#ifdef SERV_SORT_CATEGORY_ITEM_FIX
		TM_SORT_INVENTORY_USE_TIME,				// 아이템 정렬 기능 패킷 소요시간
#endif SERV_SORT_CATEGORY_ITEM_FIX
		//}}
		//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		TM_CHECK_BUFF_PERIOD,
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
		//{{ 2012. 05. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		TM_AUTO_PARTY_BONUS_CHECK,				// 자동 파티 보너스 체크 타이머
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 06. 13	김민성       선택적 셧다운제
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
		TM_SELECTIVE_SHUTDOWN_SYSTEM_CHECK,
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
		//}}
		//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
		TM_PVP_EVENT_CHECK,
#endif SERV_2012_PVP_SEASON2_EVENT
		//}}
		//{{ 2012. 10. 15	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		TM_CHECK_ED_AND_ITEM_MORNITOING,
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}
		//{{ 2012. 11. 13	최육사		자동파티 대기열 카운트 표시
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		TM_AUTO_PARTY_WAIT_USER_COUNT,
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		//}}
		//{{ 2013. 01. 01	최육사	던전에서 필드로 돌아오지 못하는 오류 수정
#ifdef SERV_SKILL_CHANGE_COOL_TIME_AT_START_COUNT
		TM_SKILL_CHANGE_COOL_TIME_AT_START_COUNT,
#endif SERV_SKILL_CHANGE_COOL_TIME_AT_START_COUNT
		//}}

#ifdef SERV_NPROTECT_CS_AUTH_30
		TM_NPROTECT_CS_AUTH_30,
#endif SERV_NPROTECT_CS_AUTH_30

		//{{ 2013. 03. 18	 만우절 이벤트_레인보우 버프 - 김민성
#ifdef SERV_APRIL_FOOLS_DAY
		TM_APRIL_FOOLS_DAY_RAINBOW_BUFF,
#endif SERV_APRIL_FOOLS_DAY
		//}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
		TM_UDP_KICK_STATE_CHANGE_FIELD_NOT,
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		TM_RELATIONSHIP_REWARD,
#endif SERV_RELATIONSHIP_SYSTEM
		//}
#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		TM_GATE_OF_DARKNESS_SUPPORT_EVENT_TIMER,
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		TM_COBOEVENT_TIMER,
		TM_COBOITEM_LIVE_TIMER,
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		TM_VALENTINE_DUNGEON_NEXT_DAY,
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
#ifdef SERV_GLOBAL_EVENT_TABLE
		TM_GLOBAL_EVENT_TABLE_GET_TIMER,
#endif //SERV_GLOBAL_EVENT_TABLE
        TM_TOTAL_NUM,
    };
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	enum TIME_CONTROL_ITEM_TYPE
	{
        TCIT_SHOP						= 0,			// 일반 상점			
		TCIT_MANUFACTURE				= 1,			// 제조
		TCIT_EXCHANGE_SHOP				= 2,			// 교환
	};
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

	//임시로 특정기능패킷중(현재는 방관련) 중복으로 날아오는 패킷체크를 위해.
	struct KRepeatEvent
	{
		u_short		m_usPreEventID;
		int			m_iPreEventIDCnt;

		void Reset()
		{
			m_usPreEventID		= 0;
			m_iPreEventIDCnt	= 0;
		}
	};

	//{{ 2009. 1. 20  최육사	경험치 제한
	class KUserExpManager
	{
	public:
		void SetExp( int iVal )				{ m_iExp.SetValue( iVal ); }
		int GetExp()						{ return m_iExp; }

		int GetChangeExp()					{ return m_iExp.GetChangeValue(); }
		
		void AddExp( int iExp );

		int GetInitExp()					{ return m_iExp.GetInitValue(); }
		void AdjustInitExp( int iVal )		{ m_iExp.AdjustInitValue( iVal ); }

	private:
		 KCacheData< int > m_iExp;
	};
	//}}

	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	struct KDungeonRequired
	{
		bool							m_bState;
		int								m_Itemid;

		KDungeonRequired()
		{
			Clear();
		}

		void Clear()
		{
			m_bState = false;
			m_Itemid = 0;
		}
	};
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}

	//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
	class KUserInfoByNexon
	{
	public:
		//{{ 2012. 01. 25  김민성	자정에 셧다운제 자동 유저 검사 기능
#ifdef SERV_SHUTDOWN_SYSTEM_AUTO_CHECK
		enum 
		{
			NOT_SHUTDOWN_USER	= -1,
			NONE_AUTH			= 0,		
			SHUTDOWN_USER		= 1,
			KICK_USER		= 2,
		};
#endif SERV_SHUTDOWN_SYSTEM_AUTO_CHECK
		//}}

		//{{ 2012. 06. 13	김민성       선택적 셧다운제
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
		enum AUTH_ERROR
		{
			NONE_AUTH_ERROR					= 0,
			AGE_AUTH_ERROR					= 1,	// 나이정보를 확인 중 오류가 발생하였습니다. 잠시 후 다시 로그인하여 주십시오.
			NAME_AUTH_ERROR					= 2,	// 실명인증을 받지 않은 회원이거나 넥슨닷컴 회원이 아닙니다.
			SHUTDOWN_USER_ERROR				= 3,	// 셧다운제에 의해 게임이용이 차단됩니다.
			SELECTIVE_SHUTDOWN_USER_ERROR	= 4,	// 선택적 셧다운제에 의해 게임이용이 차단됩니다.
			UNKNOWN_ERROR					= 99,	// 게임이용이 차단됩니다.
		};
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
		//}}

		KUserInfoByNexon()
		{
			m_bOutUserRetaining = false;
			//{{ 2012. 06. 13	김민성       선택적 셧다운제
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
			m_iRestrictionUser = NONE_AUTH;
			m_iRestrictionUserMSG = NONE_AUTH_ERROR;
			m_bExistRestrictionTime = false;
			m_bSendRestrictionUserMSG = false;
#else
			//{{ 2011. 09. 29  김민성	셧다운제 도입
	#ifdef SERV_SHUTDOWN_SYSTEM
			m_iShutdownUser = NONE_AUTH;
			m_bShutdownUserMSG = false;
	#endif SERV_SHUTDOWN_SYSTEM
			//}}
			//{{ 2012. 02. 14    김민성   넥슨 안심 로그인 - 목표 위치기반 로그인
	#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
			m_bPeaceOfMindLogin = false;
	#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
			//}}
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
			//}}
		}
		bool GetOutUserRetaining()					{ return m_bOutUserRetaining; }
		void SetOutUserRetaining( bool bRet )		{ m_bOutUserRetaining = bRet; }
		//{{ 2012. 06. 13	김민성       선택적 셧다운제
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
		// 게임 이용 제한으로 통합되므로 셧다운제,안심로그인 삭제
		int GetRestrictionUser()					{ return m_iRestrictionUser; }
		void SetRestrictionUser( int iRet )		{ m_iRestrictionUser = iRet; }
		int GetRestrictionUserMSG()					{ return m_iRestrictionUserMSG; }
		void SetRestrictionUserMSG( int& iMSG )		{ m_iRestrictionUserMSG = iMSG; }
		void GetRestrictionUserTime( std::wstring& wstrTime )		{ wstrTime = m_wstrRestrictionUserTime; }
		void SetRestrictionUserTime( std::wstring& wstrTime )		{ m_wstrRestrictionUserTime = wstrTime; }
		bool IsExistRestrictionTime()						{ return m_bExistRestrictionTime; }
		void SetExistRestrictionTime( bool& bExist )		{ m_bExistRestrictionTime = bExist; }
		bool IsSendRestrictionUserMSG()						{ return m_bSendRestrictionUserMSG; }
		void SetSendRestrictionUserMSG( bool bSend )		{ m_bSendRestrictionUserMSG = bSend; }
#else
		//{{ 2011. 09. 29  김민성	셧다운제 도입
	#ifdef SERV_SHUTDOWN_SYSTEM
		int IsShutdownUser()					{ return m_iShutdownUser; }
		void SetShutdownUser( int iRet )		{ m_iShutdownUser = iRet; }
		bool IsShutdownUserMSG()					{ return m_bShutdownUserMSG; }
		void SetShutdownUserMSG( bool bRet )		{ m_bShutdownUserMSG = bRet; }
	#endif SERV_SHUTDOWN_SYSTEM
		//}}
		//{{ 2012. 02. 14    김민성   넥슨 안심 로그인 - 목표 위치기반 로그인
	#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
		bool GetPeaceOfMindLogin()					{ return m_bPeaceOfMindLogin; }
		void SetPeaceOfMindLogin( bool bPeaceOfMindLogin )		{ m_bPeaceOfMindLogin = bPeaceOfMindLogin; }
	#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
		//}}
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
		//}}

	private:
		bool m_bOutUserRetaining;
		//{{ 2012. 06. 13	김민성       선택적 셧다운제
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
		// 게임 이용 제한으로 통합되므로 셧다운제,안심로그인 삭제
		int m_iRestrictionUser;
		int m_iRestrictionUserMSG;
		std::wstring m_wstrRestrictionUserTime;
		bool m_bExistRestrictionTime;
		bool m_bSendRestrictionUserMSG;
#else
		//{{ 2011. 09. 29  김민성	셧다운제 도입
	#ifdef SERV_SHUTDOWN_SYSTEM
		int m_iShutdownUser;
		bool m_bShutdownUserMSG;
	#endif SERV_SHUTDOWN_SYSTEM
		//}}
		//{{ 2012. 02. 14    김민성   넥슨 안심 로그인 - 목표 위치기반 로그인
	#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
		bool m_bPeaceOfMindLogin;
	#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
		//}}
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
		//}}
		
	};
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 

    // override
    virtual void	Tick();

	//////////////////////////////////////////////////////////////////////////
	// Public Member Function
public:
	//////////////////////////////////////////////////////////////////////////	

//---------------------------------------------------------------------------------------------------//
//----------- 2013.02.27 조효진	해외 기본 구조 작업 (SERV_GLOBAL_BASE)-------------------------------//
//---------------------------------------------------------------------------------------------------//

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	void CheckTradeBlockUnit();
#endif SERV_NEW_UNIT_TRADE_LIMIT

#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	void CheckNewDefenceDungeonEnter100Percent();
#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT

#ifdef SERV_NPROTECT_CS_AUTH_30
	void			CheckCSAuth3();
#endif SERV_NPROTECT_CS_AUTH_30

	//{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
	static bool LastDateSort(const KUnitInfo& refLUnitInfo, const KUnitInfo& refRUnitInfo);
#endif SERV_UNIT_WAIT_DELETE
	//}}

//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//	
	
	// User Info
	//{{ 2011. 08. 09  김민성 KAccontInfo 추가로 삭제됨
#ifdef SERV_NEW_CREATE_CHAR_EVENT
	const std::wstring&	GetUserName() const{ return m_kAccountInfo.m_wstrName; }
//#ifdef	SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-02	// 김민성 // 해외팀 주석 제거
	int			GetAuthLevel() const { return m_kAccountInfo.m_iAuthLevel; }
//#else // SERV_HACKING_USER_CHECK_COUNT
	/*
	char			GetAuthLevel()			{ return static_cast< char >(m_kAccountInfo.m_iAuthLevel); }
	*/
//#endif // SERV_HACKING_USER_CHECK_COUNT
	//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	bool			CheckAuthLevel();
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
	//}}
//#else
//	const std::wstring&	GetUserName()		{ return m_wstrUserName; }
//	char			GetAuthLevel()			{ return m_cAuthLevel; }
#endif SERV_NEW_CREATE_CHAR_EVENT
	//}}
	bool			IsGuestUser() const		{ return ( m_kNexonAccountInfo.m_byteGuestUser == 1 ); }
	bool			IsPcBang()				{ return m_kUserPcBangManager.IsPcBang(); }
#ifdef SERV_PC_BANG_TYPE
	int				GetPcBangType()			{ return m_kUserPcBangManager.GetPcBangType(); }
	KPcBangReward	GetPcBangReward()		{ return m_kUserPcBangManager.GetPcBangReward(); }
#endif SERV_PC_BANG_TYPE

#ifdef SERV_CUSTOM_CONNECT_EVENT
	void			SetCustonEventID( int iValue_ )				{ m_iCustomEventID = iValue_; }
	int				GetCustonEventID() const		{ return m_iCustomEventID; }
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_BLESS_OF_GODDESS_EVENT
	void			SetMaxLevelUnitInAccount( bool bMaxLevelUnitAccount )	{ m_bMaxLevelUnitInAccount = bMaxLevelUnitAccount; }
	bool			IsMaxLevelUnitInAccount() const	{ return m_bMaxLevelUnitInAccount; }
#endif SERV_BLESS_OF_GODDESS_EVENT

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	void			SetGateOfDarknessSupportEventTime( int iGateOfDarknessSupportEventTime ) { m_iGateOfDarknessSupportEventTime = iGateOfDarknessSupportEventTime; }
	int				GetGateOfDarknessSupportEventTime() { return m_iGateOfDarknessSupportEventTime; }
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#if defined(SERV_EVENT_RIDING_WITH_SUB_QUEST) || defined(SERV_RIDING_PET_WITH_SUB_QUEST)
	int				GetiRidingPetSummoned();				// 이벤트용 or 캐시용 펫 여부
	int				GetiRidingSummonedPetID();				// 소환된 라이딩용 펫 ID
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST

#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
	void			SetbSC1( bool mbSC1)				{ m_bSC1 = mbSC1; }
	void			SetbSC2( bool mbSC2)				{ m_bSC2 = mbSC2; }

	bool			IsbSC1()							{ return m_bSC1; }
	bool			IsbSC2()							{ return m_bSC2; }
#endif //SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT

#ifdef SERV_RELATIONSHIP_EVENT_INT
	void			SetCouple( bool bCouple ) { m_bCouple = bCouple; }
	bool			GetCouple() const { return m_bCouple; }
	void			SetRelationTargetUserUid( UidType iRelationTargetUserUid ) { m_iRelationTargetUserUid = iRelationTargetUserUid; }
	UidType			GetRelationTargetUserUid() const { return m_iRelationTargetUserUid; }
	void			SetRelationTargetUserNickname( std::wstring wstrRelationTargetUserNickname ) { m_wstrRelationTargetUserNickname = wstrRelationTargetUserNickname; }
	std::wstring	GetRelationTargetUserNickname() const { return m_wstrRelationTargetUserNickname; }
#endif SERV_RELATIONSHIP_EVENT_INT


	//////////////////////////////////////////////////////////////////////////	
	// Unit Info
	void			SetUnitInfo( const KUnitInfo& kInfo );
	void            GetUnitInfo( KUnitInfo& kInfo );
	void			ResetUnitInfo();
	bool			ResetStat();			// 전직퀘스트 수행후 불려질 함수.
	//{{ 2010. 8. 18	최육사	서버 코드 정리
	void			ClearTempInventory();
	//}}
	
	UidType			GetCharUID()const		{ return m_nUnitUID; }
    const std::wstring&	GetCharName() const	{ return m_wstrNickName; }
	char			GetUnitClass() const	{ return m_cUnitClass; }
	void			SetUnitClass( char cUnitClass ){ m_cUnitClass = cUnitClass; }
	int				GetEXP()				{ return m_kEXP.GetExp(); }
	u_char			GetLevel() const		{ return m_ucLevel; }
	//{{  2011.11.24     김민성    캐릭터 접속 로그
#ifdef SERV_CHAR_CONNECT_LOG
	void			SetLoginLevel( IN u_char ucLoginLevel )			{ m_ucLoginLevel = ucLoginLevel; }
	u_char			GetLoginLevel()									{ return m_ucLoginLevel; }
	void			SetUnitLoginTime( IN CTime& tLogin )			{ m_tCurUnitLoginTime = tLogin; }
	CTime&			GetUnitLoginTime()								{ return m_tCurUnitLoginTime; }
#endif SERV_CHAR_CONNECT_LOG
	//}}

#ifdef SERV_LUNITCONNECTLOG_BUG_FIX
	void			SetUnitLoginTime_INT( IN CTime& tLogin )			{ m_tCurUnitLoginTime_INT = tLogin; }
	CTime&			GetUnitLoginTime_INT()								{ return m_tCurUnitLoginTime_INT; }
#endif //SERV_LUNITCONNECTLOG_BUG_FIX

	int				GetBaseHP()				{ return m_kStat.m_iBaseHP; }
	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	int             GetED() const			{ return m_kEDManager.GetED(); }
	void			SetED( IN const int iED, IN const KUserEDManager::ED_REASON eReason );
	void			IncreaseED( IN const int iIncreaseED, IN const KUserEDManager::ED_REASON eReason );
	void			DecreaseED( IN const int iDecreaseED, IN const KUserEDManager::ED_REASON eReason );
#else
	int             GetED()					{ return m_iED; }
	void			DecreaseED( int iDecreaseED ) { m_iED -= iDecreaseED; }
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}    
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	int             GetRating() const		{ return m_kUserPvpManager.GetRating(); }
	int				GetRPoint()	const		{ return m_kUserPvpManager.GetRPoint(); }
	int				GetAPoint()	const 		{ return m_kUserPvpManager.GetAPoint(); }	
	int				GetWin() const			{ return m_kUserPvpManager.GetWin(); }
	int				GetLose() const			{ return m_kUserPvpManager.GetLose(); }
	CXSLUnit::PVP_RANK GetPvpRank() const	{ return m_kUserPvpManager.GetPvpRank(); }
	//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	CXSLUnit::PVP_RANK GetPvpRankForClient( void ) const;
	void GetPvpRankForClient( IN const int iOfficialMatchCnt, IN OUT char& m_cRank ) const;	// 캐릭터 리스트 읽어올 때 예외 부분 있으니 참고할 것!
	int				GetOfficialMatchCount( void ) const { return m_kUserPvpManager.GetOfficialMatchCount(); }
#endif SERV_2012_PVP_SEASON2
	//}}
#else
	int             GetVSPoint()			{ return m_iVSPoint; }
	int				GetVSPointMax()			{ return m_iVSPointMax; }
#endif SERV_PVP_NEW_SYSTEM
	//}}    
#ifdef SERV_PVP_NEW_SYSTEM	// 잘못된 디파인 해외팀 수정
	void			AddAPoint( IN const int iAPoint ) { m_kUserPvpManager.AddAPoint( iAPoint ); }
#endif SERV_PVP_NEW_SYSTEM

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	void			SetRoomListID( IN const UINT uiRoomListID );
#else	// SERV_HERO_PVP_MANAGE_LIST
	void			SetRoomListID( IN const UINT uiRoomListID )	{ m_uiRoomListID = uiRoomListID; }
#endif	// SERV_HERO_PVP_MANAGE_LIST
	UINT			GetRoomListID() const						{ return m_uiRoomListID; }	
	
	//{{ 2012. 02. 02	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	int				GetMapID() const							{ return m_kLastPos.m_iMapID; }
	void			SetMapID( IN const int iMapID )				{ m_kLastPos.m_iMapID = iMapID; }
	const KLastPositionInfo& GetLastPosInfo() const				{ return m_kLastPos; }
	void			SetLastPosInfo( IN const KLastPositionInfo& kInfo ) { m_kLastPos = kInfo; }
#else
	UINT			GetMapID() const							{ return m_nMapID; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
    UidType         GetRoomUID() const							{ return m_iRoomUID; }
	//{{ 2012. 03. 05	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	UidType         GetBeforeRoomUID() const					{ return m_iBeforeRoomUID; }
	void			SetRoomUID( IN const UidType iUID );
#else
	void			SetRoomUID( IN const UidType iUID )			{ m_iRoomUID = iUID; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	
	UidType			GetTradeUID()								{ return m_iTradeUID; }
	void			SetTradeUID( IN const UidType iUID )		{ m_iTradeUID = iUID; }
	UidType			GetPersonalShopUID()						{ return m_iPersonalShopUID; }
	void			SetPersonalShopUID( IN const UidType iUID ) { m_iPersonalShopUID = iUID; }
	//{{ 2011. 04. 11	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	UidType			GetPShopAgencyUID() const					{ return m_kUserPersonalShopManager.GetPShopAgencyUID(); }
	void			SetPShopAgencyUID( IN const UidType iUID )	{ m_kUserPersonalShopManager.SetPShopAgencyUID( iUID ); }
#endif SERV_PSHOP_AGENCY
	//}}
	UidType			GetFieldUID() const							{ return m_iFieldUID; }
	//{{ 2012. 03. 05	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			SetFieldUID( IN const UidType iUID );
#else
	void			SetFieldUID( IN const UidType iUID )		{ m_iFieldUID = iUID; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	//{{ 2011. 12. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	UidType			GetPartyUID() const									{ return m_kUserPartyManager.GetPartyUID(); }
	void			SetPartyUID( UidType iUID )							{ m_kUserPartyManager.SetPartyUID( iUID ); }
	UidType			GetCandidatePartyUID() const						{ return m_kUserPartyManager.GetCandidatePartyUID(); }
	void			SetCandidatePartyUID( UidType iUID )				{ m_kUserPartyManager.SetCandidatePartyUID( iUID ); }
	UidType			GetQuickJoinPartyUID() const						{ return m_kUserPartyManager.GetQuickJoinPartyUID(); }
	void			SetQuickJoinPartyUID( UidType iUID )				{ m_kUserPartyManager.SetQuickJoinPartyUID( iUID ); }
	//{{ 2010. 02. 09  최육사	빠른파티 기능개선
#ifdef SERV_QUICK_PARTY_UPGRADE	
	void			AddKickedPartyUID( IN UidType iPartyUID )			{ m_kUserPartyManager.AddKickedPartyUID( iPartyUID ); }
	bool			CheckKickedPartyUID( IN UidType iPartyUID )			{ return m_kUserPartyManager.CheckKickedPartyUID( iPartyUID ); }
	void			CheckExpiredKickedPartyUID()						{ m_kUserPartyManager.CheckExpiredKickedPartyUID(); }
#endif SERV_QUICK_PARTY_UPGRADE
	//}}
	void			ClearWaitPartyUser()								{ m_kUserPartyManager.ClearWaitPartyUser(); }
	bool			RegWaitPartyUser( IN const UidType iUnitUID )		{ return m_kUserPartyManager.RegWaitPartyUser( iUnitUID ); }
	bool			IsWaitPartyUser( IN const UidType iUnitUID ) const	{ return m_kUserPartyManager.IsWaitPartyUser( iUnitUID ); }
	bool			IsEmptyWaitPartyUser() const						{ return m_kUserPartyManager.IsEmptyWaitPartyUser(); }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	
	//{{ 2012. 10. 25	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	UidType			GetAutoPartyUID() const								{ return m_kUserDungeonManager.GetAutoPartyUID(); }
	UidType			GetAutoPartyWaitNumber() const						{ return m_kUserDungeonManager.GetAutoPartyWaitNumber(); }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2009. 9. 22  최육사	길드
#ifdef GUILD_TEST
	int				GetGuildUID()			{ return m_kUserGuildManager.GetGuildUID(); }
#endif GUILD_TEST
	//}}
	char            GetUnitType() const			{ return static_cast<char>( CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) ); }
	char			GetUnitState();	//유닛의 현재 위치
	//{{ 2010. 7. 29  최육사	서버 코드 정리
    bool			GetPVPChannelClass( OUT int& iPvpChannelClass ) const;
	//}}
	//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	UidType			GetSummonedPetUID()		{ return m_kUserPetManager.GetSummonedPetUID(); }
#ifdef SERV_PET_WITH_DUNGEON_CLEAR_COUNT
	KPetInfo		GetPetInfo()	{
		KPetInfo kPetInfo;
		m_kUserPetManager.GetPetInfo( GetSummonedPetUID(), kPetInfo );
		return kPetInfo;
	}
#endif //SERV_PET_WITH_DUNGEON_CLEAR_COUNT

#endif SERV_PET_SYSTEM
	//}}
#ifdef SERV_EVENT_PET_INVENTORY
	void			GetSummonePetInfo( OUT std::vector< KPetInfo >& vecPet ) { return m_kUserPetManager.GetSummonedPetInfo(vecPet); }
#endif SERV_EVENT_PET_INVENTORY
	//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	UidType			GetMatchUID()			{ return m_kUserPvpManager.GetMatchUID(); }
	bool			IsRequestMatchMaking() const { return m_kUserPvpManager.IsRequestMatchMaking(); }
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
//#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT // SERV_JAPAN_CHANNELING
	int				GetChannelCode() const	{ return static_cast<int>(m_kNexonAccountInfo.m_uChannelCode); }
//#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT // SERV_JAPAN_CHANNELING
	//}}

#ifdef SERV_STEAM
	bool			IsSteamUser() const { return ((m_kNexonAccountInfo.m_uChannelCode > 1) && (m_kNexonAccountInfo.m_uChannelCode < 6)); }	// 스팀 유저, 전환 스팀 유저 
	bool			IsPureSteamUser() const { return ((m_kNexonAccountInfo.m_uChannelCode == 2) || (m_kNexonAccountInfo.m_uChannelCode == 3)); }
#endif //SERV_STEAM

#ifdef SERV_CHANNELING_AERIA
	bool			IsAeriaUser() const { return ((m_kNexonAccountInfo.m_uChannelCode == 6) || (m_kNexonAccountInfo.m_uChannelCode == 7)); }
#endif //SERV_CHANNELING_AERIA

	//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
	UINT32			GetNexonSN() const	{ return m_kNexonAccountInfo.m_uiNexonSN; }
#endif SERV_RECOMMEND_LIST_EVENT
	//}

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	void			SetNewUnit( bool mbNewUnit)				{ m_bNewUnit = mbNewUnit; }
	void			SetNewUnit2( bool mbNewUnit2)				{ m_bNewUnit2 = mbNewUnit2; }
	void			SetCurrentUnit( bool mbCurrentUnit )	{ m_bCurrentUnit = mbCurrentUnit; }

	bool			IsNewUnit()				{ return m_bNewUnit; }
	bool			IsNewUnit2()				{ return m_bNewUnit2; }
	bool			IsCurrentUnit()				{ return m_bCurrentUnit; }
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
	void			SetConnectExperience ( int miConnectExperience )	{ m_iConnectExperience = miConnectExperience; }
	int				GetConnectExperience()						{ return m_iConnectExperience; }

	void			SetRewardBonusItem ( int miRewardBonusItem )	{ m_iRewardBonusItem = miRewardBonusItem; }
	int				GetRewardBonusItem()						{ return m_iRewardBonusItem; }
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

	//////////////////////////////////////////////////////////////////////////
	// Get Info
	void			GetTutorUnitUIDList( std::vector< UidType >& vecStudentUnitUID );
	void            GetRoomUserInfo( OUT KRoomUserInfo& kRoomUserInfo, IN const int iRoomType = -1 );
	void			GetPartyUserInfo( KPartyUserInfo& kInfo );
	void            GetSquareUserInfo( KSquareUserInfo& kSquareUserInfo );
	//{{ 2008. 11. 4  최육사	광장개편
	void			GetFieldUserInfo( KFieldUserInfo& kFieldUserInfo );
	//}}
	//{{ 2009. 9. 22  최육사	길드
#ifdef GUILD_TEST
	void			GetGuildMemberInfo( KGuildMemberInfo& kInfo );
#endif GUILD_TEST
	//}}
	//{{ 2009. 11. 10  최육사	PC방 채널이동
	void			GetChannelChangeInfo( KChannelChangeInfo& kInfo );
	//}}
	void			GetConnectionUnitInfo( KConnectionUnitInfo& kInfo );	//유닛 상태 & 정보

	//{{ 2010. 01. 15  최육사	길드게시판
#ifdef SERV_GUILD_AD
	void			GetApplyJoinGuildInfo( KApplyJoinGuildInfo& kInfo );
#endif SERV_GUILD_AD
	//}}
	//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	void			GetMatchUserInfo( OUT KMatchUserInfo& kInfo );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			GetAutoPartyUserInfo( OUT KAutoPartyUserInfo& kInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//////////////////////////////////////////////////////////////////////////
	// Check Function
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT// 작업날짜: 2013-07-14	// 박세훈
	bool			CheckCharLevelUp( const bool bHackingCheck = true );									// 레벨업 체크
#else
	bool			CheckCharLevelUp();									// 레벨업 체크
#endif // SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT
	bool			CheckRequireDungeonID( int iRequireDungeonID );
	//{{ 2012. 09. 21   김민성   마을 입장 오류 메시지 구분
#ifdef SERV_ENTER_VILLAGE_ERROR
	bool			CheckEnterTheVillage( IN int iMapID, OUT int& iLimitLevel, OUT int& iRequireDungeonID );		// 마을 입장 조건
#endif SERV_ENTER_VILLAGE_ERROR
	//}}
	bool			CheckEnterTheVillage( IN const int iMapID );		// 마을 입장 조건
	bool			CheckMaxED( __int64 iAddED );						// 최대ED 검사
	//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool			CheckForDungeonGameStart( IN const char cDungeonMode, 
											  IN const int iDungeonID, 
											  IN const char cDifficultyLevel, 
											  IN const bool bCheckForAutoParty,
											  IN const bool bSendErrorPopUp );
	bool CheckItemLevel( IN std::map< int, KInventoryItemInfo >& mapEquippedItem, IN int iDungeonIDAndDif );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	bool			CheckRequiredItemToEnterDungeonRoom( int iDungeonID, CXSLDungeon::DUNGEON_MODE eDungeonMode );
	bool			CheckRequiredItemToEnterDungeonRoom( int iDungeonID, CXSLDungeon::DUNGEON_MODE eDungeonMode, int& iRequireItemID, int& iRequireQuantity );

	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	bool			DeleteRequiredItemToEnterDungeonRoom( int iDungeonID, CXSLDungeon::DUNGEON_MODE eDungeonMode, std::vector< KInventoryItemInfo >& vecInventorySlotInfo, OUT int & iItemID );
#else
	bool			DeleteRequiredItemToEnterDungeonRoom( int iDungeonID, CXSLDungeon::DUNGEON_MODE eDungeonMode, std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}

	//{{ 2010. 8. 6	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	void			CheckAutoFeed();
	bool			AutoFeedInPetInventory( IN bool bOnlyEatSpecial, OUT int& iFeedItemID, OUT std::vector< KInventoryItemInfo >& vecPetInventorySlotInfo );

	//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
	int				CanIUseTheAutoLootingItem( IN const UidType iPetUID );
	void			SetAutoLootingPet( IN const UidType iPetUID );
#endif SERV_PET_AUTO_LOOTING
	//}}

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	int				CanIUseTransformItem( IN const UidType iPetUID );
	void			UseTransformItem( IN int iUsedItemID, IN const UidType iPetUID, OUT int& iBeforePetID, OUT int& iAfterPetID );
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#endif SERV_PET_SYSTEM
	//}}	

	//{{ 허상형 : [2011/3/23/] //	월드 미션
	void			CheckUpdateWorldMission();
	//}} 허상형 : [2011/3/23/] //	월드 미션
	
	bool			IsDenyInviteParty()			{ return m_kUserGameOption.IsDenyParty(); }
	bool			IsExistInventoryItem( int iItemID );
	//{{ 2009. 11. 3  최육사	길드
	bool			IsExistInventoryItemWithBank( int iItemID );
	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	bool			IsEnoughSpaceExist( IN const std::map< int, int >& mapItem );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	bool			IsCompleteQuest( int iQuestID );
	//}}
	//{{ 09.05.20.hoons.
	bool			IsClearDungeon( int iDungeonID, char cDiffLevel );
	//]]
	//{{ 2009. 8. 4  최육사		봉인스킬
	bool			IsSkillUnsealed( int iSkillID ) { return ( m_kSkillTree.IsSkillUnsealed( iSkillID ) ); }
	//}}
	//{{ 2009. 7. 3  최육사		헤니르 시공
	bool			IsHenirDungeonChallengeMode();
	//}}

	//{{ 2009. 6. 10  최육사	퀵슬롯 쿨타임 리셋
	void			QuickSlotCoolTimeReset()	{ m_kInventory.ResetQuickSlotCoolTime(); }
	//}}

	//{{ 2009. 9. 7  최육사		퀘스트 기능 수정
	bool			IsDungeonGetExp( int iDungeonID, char cDifficulty );
	//}}

	//{{ 2010. 05. 12  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	bool			IsSameServerGroupUID( IN UidType iUID );
#endif SERV_INTEGRATION
	//}}

#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
	bool			IsEnoughItemExist( IN const std::map< int, int >& mapRequiredItem, IN const bool bOnlyInventory );
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST

	//{{ 2010. 01. 11  최육사	서버코드정리
	bool			IsEnoughItemExistOnlyInventory( IN const std::map< int, int >& mapRequiredItem );
	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	bool			IsEnchantAbuser()			{ return m_kUserAbuserManager.IsEnchantAbuser(); }
	void			CheckEDAbuser( IN int iGetEDState, IN int iBeforeED );
	void			CheckItemAbuser( IN int iGetItemState, IN const std::map< int, int >& mapGetItemInfo );
#endif SERV_USER_ABUSER_MANAGER
	//}}
	bool			IsChatBlackListUnit( UidType iUnitUID ) { return m_kUserGameOption.IsChatBlackListUnit( iUnitUID ); }
	void			UserStatisticsIncreaseCount( int iTableID, int iSubTableID , int iColumn, int iCount );
	//}}

	//////////////////////////////////////////////////////////////////////////
	// Update Function
	void			SendLeaveParty( int iReason );
	void			SendLeaveField( int iReason );
	void			SendLeavePVPLobby();
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	void			SendLeavePVPMatch( IN const int iReason );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 02. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			SendLeaveAutoParty( IN const int iReason );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	void			SendUpdatePartyUnitInfo( bool bChangeField = false );	
	//{{ 2010. 02. 20  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	void			SendUpdatePartyMemberList( IN UidType iPartyUID, IN const std::set< UidType >& setPartyUIDList );
#else
	void			SendUpdatePartyList();
#endif SERV_CHANNEL_PARTY
	//}}
	void			SendUpdateFriendList();
	//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
	void			SendUpdateGuildMemberInfo();
#endif GUILD_TEST
	//}}
	//{{ 2010. 01. 11  최육사	추천인리스트
	void			SendUpdateRecommendUserInfo( IN const KRecommendUserInfo::RECOMMEND_USER_INFO_TYPE eType );
	//}}

	//{{ 2009. 4. 22  최육사	대전유저리스트
	void			SendUpdatePVPUserInfo( char cState );
	//}}
	//{{ 2009. 6. 5  최육사		장착정보 변경
	void			SendUpdateChangeEquippedItem();
	//}}
	//{{ 2010. 02. 08  최육사	PC방 채널 이동 오류 수정
	void			SendUpdateChangeEquippedItem( IN const std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
	//}}
	//{{ 2010. 02. 10  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST
	void			SendUpdateDropQuestItemByIngQuest();
#endif SERV_DAY_QUEST
	//}}
	//{{ 2010. 8. 19	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	void			SendPetAction( IN CXSLPetManager::PET_ACTION_COMMAND ePetAction );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2013. 05. 29	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
	void			SendCheckBlockUser( IN const KBlockInfo::BLOCK_TYPE eBlockType );
	void			SendCheckBlockUser( IN const std::vector< int >& vecBlockType );
#endif SERV_BLOCK_LIST
	//}}	
	//{{ 2013. 06. 03	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	void			SendEnchantNotice( IN const int iItemID, IN const int iEnchantLevel, IN const int iRandomItemID = 0 ) const;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
#ifdef SERV_RELAY_DB_CONNECTION
	void			SendUpdateUnitInfoToRelayDB( IN const int iUpdateType, IN const bool bDeletedUnit = false, IN const UidType iUnitUID = 0 );
#endif SERV_RELAY_DB_CONNECTION
	//}}

	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	void			HackUserRegRejectedUser( IN const char cReason
#ifdef SERV_PERIOD_ACCOUNT_BLOCK// 작업날짜: 2013-05-27	// 박세훈
		, IN const char cPeriodUAL = 0
		, IN const std::wstring& wstrBlockReason = L""
#endif // SERV_PERIOD_ACCOUNT_BLOCK
		);
	void			HackUserRegTradeBlock( IN const char cReason );
	void			HackUserRegMonitorsUser( IN const char cReason );
	//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	void			HackUserKick( IN const KEGS_SERVER_CHECK_HACK_USER_REQ& kPacket );
#else
	void			HackUserKick( IN const KEGS_SERVER_CHECK_HACK_USER_NOT& kPacket );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	//}}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}
	//{{ 2011. 09. 08	최육사	오토핵 패킷 모니터링
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
	void			HackUserRegPacketMornitor();
#endif SERV_AUTO_HACK_PACKET_MORNITOING
	//}}

#ifdef SERV_FIX_KGSUSER_KICK// 작업날짜: 2013-08-22	// 박세훈
	void            Kick( IN const int iNetError );
	void            Kick( IN const KStatistics::ESIColumn_Disconnect_Reason eDisconnectReason );
#else // SERV_FIX_KGSUSER_KICK
	void            Kick( int iReason );
#endif // SERV_FIX_KGSUSER_KICK
	static void     DumpMaxLoginTime()  { ms_kMaxLoginTime.Dump(); }

	//{{ 2012. 02. 27	김민성	인벤토리 정렬	- 무거운 기능을 가볍게 변경
#ifdef SERV_SORT_CATEGORY_ITEM_FIX
	static void     DumpMaxInventorySortTime()  { ms_kMaxInventorySortTime.Dump(); }
#endif SERV_SORT_CATEGORY_ITEM_FIX
	//}}


	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	bool				IsReserveKickUser()							{ return m_kDungeonRequired.m_bState; }
	void				SetReserveKickUser( bool _state )			{ m_kDungeonRequired.m_bState = _state; }
	int					GetDungeonRequiredItemID()					{ return m_kDungeonRequired.m_Itemid; }
	void				SetDungeonRequiredItemID( int itemid )		{ m_kDungeonRequired.m_Itemid = itemid; }
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}

	//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	void				CheckLetterAndTradeMonitoring();
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}
	//{{ 2011. 04. 26	최육사	deserialize 실패 종료
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
	void				CheckDeserializeFailCount();
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
	//}}
	//{{ 2012. 10. 15	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	void				CheckEDAndItemMornitoring();
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}
	//{{ 2012. 05. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void				CheckAutoPartyBonus();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
	void SetIsUseSecondPW( IN const bool& isUse )					{ m_bUseSecondPW = isUse; }
	void SetSecondPWFailedCount( IN const int& iFailedCount )		{ m_iFailedCount = iFailedCount; }
	int	 GetSecondPWFailedCount()									{ return m_iFailedCount; }
	void SetSecondPW( IN const std::wstring& wstrSecondPW )			{ m_wstrSecondPW = wstrSecondPW; }
	void GetSecondPW( OUT std::wstring& wstrSecondPW )				{ wstrSecondPW = m_wstrSecondPW; }
	void SetLastAuthDate( IN const CTime& tLastAuthDate )			{ m_tLastAuthDate = tLastAuthDate; }
	void SetSuccessSecondSecurity( IN const bool& bSuccess )			{ m_bPassSecondSecurity = bSuccess; }
	void SetIsUseOTP( IN const int& iNexonSecurityType );
	bool GetSuccessSecondSecurity();
	bool GetIsUseSecondPW()											{ return m_bUseSecondPW; }
	bool GetIsUseOTP()												{ return m_bIsUseOTP; }
	int	 GetNexonSecurityType()										{ return m_iNexonSecurityType; }
	bool IsUseSecurity()											{ return m_bIsUseOTP || m_bUseSecondPW; }
	bool IsSameSecondSecurityPW( IN const std::wstring& wstrSecondPW );
	bool CheckIntegritySecondSecurityPW( IN const std::wstring& wstrSecondPW, OUT int& iOK );
	void SendSecurityType();
	void SendFirstLoginSecurityInfo();
	//{{ 2011. 08. 09  김민성 KAccontInfo 추가로 삭제됨
#ifdef SERV_NEW_CREATE_CHAR_EVENT
	void SetLastLoginDate( IN const std::wstring& wstrLoginDate )       { m_kAccountInfo.m_wstrLastLogin = wstrLoginDate; }
	std::wstring& GetLastLoginDate()									{ return m_kAccountInfo.m_wstrLastLogin; }
#else
	void SetLastLoginDate( IN const std::wstring& wstrLoginDate )       { m_wstrLoginDate = wstrLoginDate; }
	std::wstring& GetLastLoginDate()									{ return m_wstrLoginDate; }
#endif SERV_NEW_CREATE_CHAR_EVENT
	//}}
	int  GetSecurityType();
#endif SERV_SECOND_SECURITY
	//}}

	//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
	void	UseResurrectionStone( IN bool bIsUse )				{ m_kUserTitleManager.UseResurrectionStone( bIsUse ); }
	bool	IsUseResurrectionStone()							{ return m_kUserTitleManager.IsUseResurrectionStone(); }
	void	SetStartedNumMember( IN int iStartedNumMember )		{ m_kUserTitleManager.SetStartedNumMember( iStartedNumMember ); }
	int		GetStartedNumMember()								{ return m_kUserTitleManager.GetStartedNumMember(); }
//{{ 2011. 09. 19  김민성	칭호 획득 조건 추가
	std::wstring&	GetUserID()									{ return m_kAccountInfo.m_wstrID; }
//}}
#endif SERV_ADD_TITLE_CONDITION
	//}}
	//{{ 2013. 02. 05  칭호 획득 조건 추가(부활석 사용 횟수, 샌더 마을) - 김민성
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
	void	ClearUseResurrectionStoneCount()							{ m_kUserTitleManager.ClearUseResurrectionStoneCount(); }
	void	AddUseResurrectionStoneCount()								{ m_kUserTitleManager.AddUseResurrectionStoneCount(); }
	int		GetUseResurrectionStoneCount()								{ return m_kUserTitleManager.GetUseResurrectionStoneCount(); }
#endif SERV_ADD_TITLE_CONDITION_SANDER
	//}

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	void CheckComeBackUser();
	void AnnunciateComeBackUser();
	void SendFirstLoginComeBackUser( IN KUnitInfo& kUnit, IN bool bFirst );
	void SendComeBackLogOut();
#endif SERV_COME_BACK_USER_REWARD
	//}}

	//{{ 2011. 08. 09  김민성 (2011.08.11) 특정일 이후 생성한 계정에 대하여 신규케릭터 생성 시 아이템 지급 이벤트
#ifdef SERV_NEW_CREATE_CHAR_EVENT
	void SetAccountInfo( IN const KAccountInfo& AccountInfo )	{ m_kAccountInfo = AccountInfo; }
	void GetAccountInfo( OUT KAccountInfo& AccountInfo )	{ AccountInfo = m_kAccountInfo; }
#endif SERV_NEW_CREATE_CHAR_EVENT
	//}}

	//{{ 2011. 08. 10    김민성    채널링 SOAP
#ifdef SERV_CHANNELING_SOAP
	void GetToonilandID( OUT std::wstring& wstrID )				{ wstrID = m_wsrtToonilandID; }
	void SetToonilandID( IN std::wstring& wstrID )				{ m_wsrtToonilandID = wstrID; }
#endif SERV_CHANNELING_SOAP
	//}}

	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	void CheckHenirRewardCountEvent();
#endif SERV_NEW_HENIR_TEST
	//}}

	//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	int  GetChannelRandomKey()									{ return m_kUserSecurityManager.GetChannelRandomKey(); }
	void SetChannelRandomKey( IN int& iKey )					{ m_kUserSecurityManager.SetChannelRandomKey( iKey ); }
	void SendRefreshChannalRandomKey();
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}

	//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
	bool GetEDMonitoringMode()									{ return m_kUserHackingManager.GetEDMonitoringMode(); }
#endif SERV_ED_MONITORING_IN_GAME
	//}}

	//{{ 2012. 06. 13	김민성       선택적 셧다운제
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
	bool IsRestrictionUser( IN std::map< unsigned short, KPropertyValue >& mapProperty, IN BYTE bytePolicyResultNo );
	bool SetRestrictionUserMSG( IN std::wstring& wstrOption, OUT int& iError, OUT std::wstring& wstrTime, OUT bool& bExistTime );
	void CheckRestrictionUser();
#else
		//{{ 2011. 09. 29  김민성	셧다운제 도입
	#ifdef SERV_SHUTDOWN_SYSTEM
		bool CheckShutdownUser( IN std::map< unsigned short, KPropertyValue >& mapProperty, IN BYTE bytePolicyResultNo );
		bool CheckOutUserRetaining( IN std::map< unsigned short, KPropertyValue >& mapProperty, IN BYTE bytePolicyResultNo, OUT int& iOutUserRetaining  );
		bool CheckShutdownUserMSG( IN std::wstring& wstrOption );
	#endif SERV_SHUTDOWN_SYSTEM
		//}
		//{{ 2012. 02. 14    김민성   넥슨 안심 로그인 - 목표 위치기반 로그인
	#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
		bool CheckPeaceOfMindLoginUser( IN std::map< unsigned short, KPropertyValue >& mapProperty, IN BYTE bytePolicyResultNo, OUT int& iResult );
	#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
		//}}
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
	//}}

	//{{ 2011. 08. 29	최육사	캐릭터 레벨업 이벤트
#ifdef SERV_CHAR_LEVEL_UP_EVENT
	bool GetLevelUpRewardItem( IN u_char ucNowLevel, IN u_char ucLevel, OUT std::vector< int >& vecLevelUpRewardID );
#endif SERV_CHAR_LEVEL_UP_EVENT
	//}}

	//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	void SetBF_Team( IN int& iTeam )							{ m_kNexonAccountInfo.m_iBF_Team = iTeam; }
	int GetBF_Team()											{ return m_kNexonAccountInfo.m_iBF_Team; }
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//}}

	//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	bool GetUnitClassChangeInfo( IN int iItemID, OUT KDBE_BUY_UNIT_CLASS_CHANGE_REQ& kPacketToDB );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	bool GetUnlimitedClassChangeInfo( IN int iNewClassID, OUT KDBE_BUY_UNIT_CLASS_CHANGE_REQ& kPacketToDB );
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

	//{{ 2012. 03. 27	김민성		머신 시간을 검사하자(50분 이상 차이나면 모니터링)
#ifdef SERV_CHECK_MACHINE_LOCALTIME
	bool CheckMachineLocalTime( SYSTEMTIME& st, std::wstring& wstrClientTime );
#endif SERV_CHECK_MACHINE_LOCALTIME
	//}}

	//{{ 2012. 03. 29	김민성		x2.exe 모듈 유무 검사
#ifdef SERV_CHECK_X2_EXE_MODULE
	bool CheckModuleList_X2Module( IN std::vector< std::wstring >& vecModuleList, OUT std::map< std::wstring, bool >& mapExistModule );
#endif SERV_CHECK_X2_EXE_MODULE
	//}}

	//{{ 2012. 05. 23	김민성       판매 불가 아이템 판매 시도 해킹 유저로 판단
#ifdef SERV_IMPOSSIBLE_SELL_ITEM
	void SetRandomTimeKick( int iReason, int iMinTime, int iRange );
	void CheckRandomTimeKick();
#endif SERV_IMPOSSIBLE_SELL_ITEM
	//}}

	//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	void SetPersonalShopBuyItemInfo( IN KERM_BUY_PERSONAL_SHOP_ITEM_REQ& kPacket );
	void GetPersonalShopBuyItemInfo( OUT KERM_BUY_PERSONAL_SHOP_ITEM_REQ& kPacket );
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	//}}
	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
	bool IsGuildMaster( void );
#endif SERV_GUILD_CHANGE_NAME
	//}}
	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	void SetAgencyShopPickUpItemInfo( IN std::vector< UidType >& vecPickUpItemList );
	void GetAgencyShopPickUpItemInfo( OUT std::vector< UidType >& vecPickUpItemList );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	void SetEventMoney( IN int iEventMoney )	{ m_iEventMoney = iEventMoney; }
	int GetEventMoney()							{ return m_iEventMoney; }
#endif // SERV_EVENT_MONEY

	//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
	void CheckPvpEvent();
#endif SERV_2012_PVP_SEASON2_EVENT
	//}}

	//{{ 2011. 10. 14	최육사	아이템 사용 DB 업데이트 수정
#ifdef SERV_USE_ITEM_DB_UPDATE_FIX
	void OnUseItemInInventory( IN const UidType iItemUID
							//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
//#ifdef SERV_PET_AUTO_LOOTING
							 , IN const __int64 iTempCode
//#endif SERV_PET_AUTO_LOOTING
							//}}
							 , IN const bool bNotDeleteItem = false
							 , IN const int iWarpPointMapID = 0
							 );
#endif SERV_USE_ITEM_DB_UPDATE_FIX
	//}}

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	void SendUpdateBuffInfo();
	bool ExistActivateBuff( IN const int iBuffID ) { return m_kUserBuffManager.IsBuffActivated( iBuffID ); }
	bool ActivateBuff( IN const int iBuffID, OUT std::vector< KBuffInfo >& vecActivatedBuff, IN const CTime tUseTime = 0, IN int iPeriod = 0 );	
	bool DeactivateBuff( IN const int iBuffID, OUT std::vector< int >& vecDeleteBuff );
	bool ActivateItemBuff( IN const int iUseItemID, OUT std::vector< KBuffInfo >& vecActivatedBuff, OUT std::vector<int>& vecDeactiveBuff, IN const CTime tUseTime = 0 );
	void CheckBuffPeriod();
	void CheckDeselectUnitBuff( IN const bool bSendAck = true );
	int GetEquippedTitleID() { return m_kUserTitleManager.GetEquippedTitleID(); }
#endif SERV_SERVER_BUFF_SYSTEM
	//}
	//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	const int			GetWinterVacationEventCount( void ) const{	return m_iWinterVacationEventCount;	}
	const std::wstring&	GetWinterVacationEventRegDate( void ) const{	return m_wstrWinterVacationEventRegDate;	}
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}

	//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	void SendUpdateAccountQuestInfo();
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}

#ifdef SERV_INVISIBLE_GM
	void ToggleInvisible() { m_bInvisible = !m_bInvisible; }
	bool IsInvisible() { return m_bInvisible; }
#endif SERV_INVISIBLE_GM

	//{{ 2013. 3. 11	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	void	IncreasedLocalRankingPoint( IN const int iSpirit, IN const int iAP, IN const bool bForce = false );
	void	_IncreasedLocalRankingPoint( IN const int iSpirit, IN const int iAP, IN const bool bForce = false );
	void	LocalRankingInquiryProcess( IN const UINT nViewPage
		, IN const char cMainTabIndex
		, IN const char cSubTabIndex
		, IN const byte byteFilter
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
		, IN const bool bRankingButtonClick
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
		);
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef SERV_NEW_EVENT_TYPES
	void CheckMaxLevel();
	void CheckEnableEventDungeon();
#endif SERV_NEW_EVENT_TYPES	

#ifdef SERV_CODE_EVENT
	void CheckEnableCodeEvent();
#endif SERV_CODE_EVENT

	//{{ 2013. 03. 18	 만우절 이벤트_레인보우 버프 - 김민성
#ifdef SERV_APRIL_FOOLS_DAY
	void CheckRainbowBuff();
#endif SERV_APRIL_FOOLS_DAY
	//}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-05
	void	GetLocalRankingWatchUnitInfo( OUT KLocalRankingUserInfo& kUserInfo, OUT KLocalRankingUnitInfo& kUnitInfo ) const;
#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
	std::wstring	GetAccountRegDate( void ) const{	return m_kAccountInfo.m_wstrRegDate;	}
#endif	// SERV_CRITERION_DATE_EVENT

#ifdef	SERV_NEW_USER_QUEST_HARD_CODING// 적용날짜: 2013-04-11
	std::wstring	GetAccountRegDate( void ) const{	return m_kAccountInfo.m_wstrRegDate;	}
#endif	// SERV_NEW_USER_QUEST_HARD_CODING

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	void CheckRelationshipReward();
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	void CalculatePvpUnitStat( IN int iPVPChannelClass, IN KStat& kModifiedBaseStatBySkill, IN bool bIsDungeonRoom, IN unsigned int uiHP_OnePoint, IN unsigned int uiUnitLevel, IN OUT KRoomUserInfo& kRoomUserInfo );
	void CalculateDungeonUnitStat( IN bool bRevisionDungeon, IN KStat& kModifiedBaseStatBySkill, IN bool bIsDungeonRoom, IN unsigned int uiHP_OnePoint, IN unsigned int uiUnitLevel, IN OUT KRoomUserInfo& kRoomUserInfo, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo );
	void CalculateBattleFieldUnitStat( IN KStat& kModifiedBaseStatBySkill, IN bool bIsDungeonRoom, IN unsigned int uiHP_OnePoint, IN unsigned int uiUnitLevel, IN OUT KRoomUserInfo& kRoomUserInfo, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo );
	void CalculateTrainingCenterUnitStat( IN KStat& kModifiedBaseStatBySkill, IN bool bIsDungeonRoom, IN unsigned int uiHP_OnePoint, IN unsigned int uiUnitLevel, IN OUT KRoomUserInfo& kRoomUserInfo, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo );
	void CalculateSkillUnitStat( IN OUT KRoomUserInfo& kRoomUserInfo, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo );
	void CheckInventoryExpMedal( IN OUT KRoomUserInfo& kRoomUserInfo );
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-06-26	// 박세훈
	int		JumpingCharacterCheck( void ) const;
	int		JumpingCharacterCheck( OUT int& iTargetLevel, OUT CTime& tCurrentDate ) const;
	bool	CheckJumpingCharLevelUp( IN const unsigned char ucLevel );
#endif // SERV_JUMPING_CHARACTER

#ifdef SERV_CRITERION_DATE_EVENT// 작업날짜: 2013-06-26	// 박세훈
	byte GetCriterionDateEventInfo( void ) const;
#endif // SERV_CRITERION_DATE_EVENT

#ifdef HSB_ALWAYS_VALID_IN_THE_CASHSHOP
	bool GetEnterCashShop() { return m_kUserUnitManager.GetEnterCashShop(); }
#endif // HSB_ALWAYS_VALID_IN_THE_CASHSHOP

#ifdef SERV_STEAM
	void SetSteamClientUser( IN bool bVal_ ) { m_bSteamClient = bVal_; }
	bool IsSteamClientUser() const { return m_bSteamClient; }
#endif //SERV_STEAM

#ifdef SERV_CHANNELING_AERIA
	void SetAeriaClientUser( IN bool bVal_ ) { m_bAeriaClient = bVal_; }
	bool IsAeriaClientUser() const { return m_bAeriaClient; }
#endif //SERV_CHANNELING_AERIA
	
	//{{ 2012. 09. 06	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER
	void CheckUpdateGlobalMission();
#endif SERV_GLOBAL_MISSION_MANAGER
	//}} 2012. 09. 06	임홍락	글로벌 미션 매니저

#ifdef SERV_EVENT_CHECK_POWER
	void CheckCheckPowerResult();
	void UpdateCheckPowerScore( int iDungeonID, char cTotalRank, int iPartySize, bool bSuitableLevelDungeon, char cDifficulty, bool bIsMVP, bool bIsClear );
#endif SERV_EVENT_CHECK_POWER
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	void SetChungGiveItem( bool bTemp)
	{
		m_bGiveItem = bTemp;
	}
	bool GetChungGiveItem(void)
	{
		return m_bGiveItem;
	}
	void SetChungGiveItemTime( CTime cTempTime)
	{
		m_cGetItemTime = cTempTime;
	}
	CTime GetChungGiveItemTime(void)
	{
		return m_cGetItemTime;
	}
	void ChungItemGiveTimeTick(void);
#endif SERV_EVENT_CHUNG_GIVE_ITEM

	//////////////////////////////////////////////////////////////////////////
	// protected member function
protected:
    void			WriteUnitInfoToDB( bool bFinal, int iEventID = 0 );
    void			WritePlayTimeToDB();
    void			SendDisconnectEvent( u_short usEventID = -1 );
	//{{ 2009. 4. 3  최육사		치트로그
	void			WriteCheatLogToDB( const wchar_t* szCheatCode, const std::wstring& wstrCheatContents );
	void			WriteCheatLogToDB( const wchar_t* szCheatCode, const CStringW& cwstrCheatContents );
	void			WriteCheatLogToDB( const wchar_t* szCheatCode );
	//}}
	//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	void			WriteCharacterLogToDB( IN const KELOG_USER_CHARACTER_LOG_NOT::LOG_TYPE eLogType );
	void			WriteCharacterLogToDBWithDungeonID( IN const KELOG_USER_CHARACTER_LOG_NOT::LOG_TYPE eLogType, IN const int iDungeonID );
	void			WriteCharacterLogToDBWithProductNo( IN const KELOG_USER_CHARACTER_LOG_NOT::LOG_TYPE eLogType, IN const int iProductNo );
	void			WriteCharacterLogToDBWithParam( IN const KELOG_USER_CHARACTER_LOG_NOT::LOG_TYPE eLogType, IN const int iDungeonID, IN const int iProductNo );
#endif SERV_CHAR_LOG
	//}}

	//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			DungeonGameStart_SingleOpenDungeonRoom( IN const char cDungeonMode, IN const int iDungeonID, IN const char cDifficultyLevel, IN const bool bAutoParty );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{  2011.11.24     김민성    캐릭터 접속 로그
#ifdef SERV_CHAR_CONNECT_LOG
	void			SendUpdateUnitConnectLog();
#endif SERV_CHAR_CONNECT_LOG
	//}}

	//{{ 2009. 6. 12  최육사	데이터 검증	
	bool			CheckValidNickName( const std::wstring& wstrNickName );
	//}}

	// Check Function
	void			CheckTimeEvent();									// 접속 시간 이벤트

	//{{ 2009. 10. 14  최육사	자동결제
#ifdef AP_RESTONE
	void			CheckAutoPayment();									// 자동결제
#endif AP_RESTONE
	//}}
	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	void			CheckHackingToolList();
#endif SERV_HACKING_TOOL_LIST
	//}}
	//{{ 2010. 07. 04  최육사	스킬 포인트 오류 확인
#ifdef SERV_CHECK_SKILL_POINT
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	bool			CheckSkillPointError() const;
#else // SERV_SKILL_PAGE_SYSTEM
	bool			CheckSkillPointError();
#endif // SERV_SKILL_PAGE_SYSTEM


#endif SERV_CHECK_SKILL_POINT
	//}}
#ifdef SERV_SKILL_POINT_CORRECTION
	bool			CheckSkillPointCorrection( OUT int& iCorrectionSP ); // 2013.06.14 lygan_조성욱 // 문제 되는 스킬 포인트 보정 작업 ( 로그인과 렙업시에만 체크 )
#endif //SERV_SKILL_POINT_CORRECTION

	//{{ 2011. 12. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			CheckWaitPartyUser();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}


	//{{ 2011. 06. 17	최육사	패킷 이벤트 보안
#ifdef SERV_KEVENT_FROM
	bool			CheckIsValidPacketEvent( IN const KEventPtr& spEvent, IN const wchar_t* pszPacketStructName, IN const bool bIsDeserializeSuccess, IN const bool bIsValidPacketID );
#endif SERV_KEVENT_FROM
	//}}
	
	bool			CheckingSpirit( IN const int iDungeonID );
	//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool			DecreaseSpirit( IN const int iDungeonID, IN const char cUserCount, IN const short sStageNpcCount, IN const bool bIsTutorial, IN const bool bIsBattleField );
#else
	bool			DecreaseSpirit( IN const int iDungeonID, IN const char cUserCount, IN const short sStageNpcCount, IN const bool bIsTutorial );
	//{{ 2011. 09. 23	최육사	공존의 축제 근성도 소모 이벤트
	//#ifdef SERV_COEXISTENCE_FESTIVAL
	int				CalculateDecreaseSpirit( IN const char cUserCount, IN const short sStageNpcCount, IN const bool bIsTutorial, IN const bool bHalfDecreaseEvent );	// 근성도 소모량 계산 함수
	//#else
	//	int				CalculateDecreaseSpirit( IN const char cUserCount, IN const short sStageNpcCount, IN const bool bIsTutorial );			// 근성도 소모량 계산 함수
	//#endif SERV_COEXISTENCE_FESTIVAL
	//}}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

#ifdef SERV_CHINA_SPIRIT_EVENT
	void			AddChinaSpirit( IN int iLocationIndex, IN int iSpirit )
	{
		m_arrChinaSpirit[iLocationIndex] = min( 2350, m_arrChinaSpirit[iLocationIndex] + iSpirit );
	}
	void			SetChinaSpirit( IN int iLocationIndex, IN int iSpirit )
	{
		m_arrChinaSpirit[iLocationIndex] = iSpirit;
	}
	int				GetChinaSpirit( IN int iLocationIndex )
	{
		if( iLocationIndex > 5 || iLocationIndex < 0 )
		{
			START_LOG( cerr, L"m_iLocationIndex 값이 비정상. 해커!")
				<< BUILD_LOG( GetUserName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( iLocationIndex )
				<< END_LOG;
			return -1;
		}

		return m_arrChinaSpirit[iLocationIndex];
	}
#endif SERV_CHINA_SPIRIT_EVENT

	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	bool			CheckDungeonPartyCondition( IN const int iDungeonID,
												IN const char cDifficultyLevel,
												IN const char cDungeonMode,
												IN const bool bCheckLowLevel,
												IN const bool bDontCheckPartyUID );

	bool			CheckPvpPartyCondition( IN const char cPVPGameType, IN const bool bDontCheckPartyUID );
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2012. 04. 02	김민성		sn 모니터링 기능 분리
#ifdef SERV_SERIAL_NUMBER_MORNITORING
	void			CheckServerSN_Monitoring();
#endif SERV_SERIAL_NUMBER_MORNITORING
	//}}

	//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	void			SetHackingUserSendType( IN char cType );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	//}}
	//{{ 2012. 11. 13	최육사		자동파티 대기열 카운트 표시
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
	void			CheckAutoPartyWaitUserCountRequest();
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
	//}}

	// Check Expire
    void			ExpireItem( bool bForce = false );
	void			ExpireCashSkillPoint( bool bForce = false );
    void			ExpireSkillSlotB( bool bForce = false );
	void			ExpireTitle( bool bForce = false );

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
	bool			CheckLimitedPlayTimes( IN const int iDungeonID );
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

#ifdef SERV_GLOBAL_EVENT_TABLE
	bool			GetGlobalEventTableData( OUT std::map< int, KGlobalEventTableData >&	mapGlobalEventData );
	bool			SendEnableDBGlobalEventTableData( IN std::map< int, KGlobalEventTableData >	mapGlobalEventData );
	void			CheckDisableCodeAndSetGlobalEventdata();
	int				GetGlobalEventData( IN std::vector<int>& veciParamData, IN int iVecIndex );
#endif //SERV_GLOBAL_EVENT_TABLE

	//////////////////////////////////////////////////////////////////////////
	// packet & event send function
public:
	template < typename T > bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false );
	bool SendID( unsigned short usEventID );
    
    template < class T > void SendToAccountDB( unsigned short usEventID, const T& data ) const;
    template < class T > void SendToGameDB( unsigned short usEventID, const T& data );
    template < class T > void SendToGameDB2nd( unsigned short usEventID, const T& data );
    template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	template < class T > void SendToSMSDB( unsigned short usEventID, const T& data );
	//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
    template < class T > void SendToNXWebDB( unsigned short usEventID, const T& data );
#endif SERV_WEB_POINT_EVENT
	//}}
	//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
#ifdef SERV_RELAY_DB_CONNECTION
	template < class T > void SendToRelayDB( unsigned short usEventID, const T& data );
#endif SERV_RELAY_DB_CONNECTION
	//}}
    template < class T > void SendToCnUser( unsigned short usEventID, const T& data );
	//{{ 2009. 4. 24  최육사	대전유저리스트
	template < class T > void SendToCnServer( unsigned short usEventID, const T& data ) const;
	//}}
    template < class T > void SendToLoginAccountDB( unsigned short usEventID, const T& data );	
    template < class T > void _SendToCnRoom( UidType nTo, unsigned short usEventID, const T& data );
    template < class T > void SendToCnRoom( unsigned short usEventID, const T& data );
	//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	template < class T > void _SendToLoginRoom( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToLoginRoom( unsigned short usEventID, const T& data );

	template < class T > void _SendToPShopRoom( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToPShopRoom( unsigned short usEventID, const T& data );
#endif SERV_PSHOP_AGENCY
	//}}
	//{{ 2010. 02. 20  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	template < class T > void _SendToCnParty( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToCnParty( unsigned short usEventID, const T& data );
#endif SERV_CHANNEL_PARTY
	//}}
	template < class T > void _SendToGSField( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToGSField( unsigned short usEventID, const T& data );
	template < class T > void SendToLoginServer( unsigned short usEventID, const T& data );
	template < class T > void _SendToCharacter( UidType nTo, unsigned short usEventID, const T& data );

	//---------------------------------------------------------//
#ifdef SERV_COUNTRY_CN
	// 2011. 03. 02	박교현	플랫폼 접속 구조 개편
	template < class T > void SendToGiantRoleReg( unsigned short usEventID, const T& data );
	template < class T > void SendToGiantBillingServer( unsigned short usEventID, const T& data );
#endif // SERV_COUNTRY_CN

#ifdef SERV_GLOBAL_BILLING
	template < class T > void SendToKOGBillingDB( unsigned short usEventID, const T& data );
	template < class T > void SendToKOGLoginBillingDB( unsigned short usEventID, const T& data );
	template < class T > void SendToPublisherBilling( unsigned short usEventID, const T& data );
	template < class T > void SendToPublisherBillingDB( unsigned short usEventID, const T& data );
	void SendToKOGBillingDB( unsigned short usEventID );
#endif // SERV_GLOBAL_BILLING
	//---------------------------------------------------------//

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	template < class T > void SendToGlobalServer( unsigned short usEventID, const T& data );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가
	//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	template < class T > void _SendToMatch( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToMatch( unsigned short usEventID, const T& data );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 02. 08	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	template < class T > void _SendToAutoParty( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToAutoParty( unsigned short usEventID, const T& data );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2010. 11. 12 조효진	채팅 로그 관련 추가
#ifdef SERV_RECORD_CHAT
	template < class T > void SendToChatLogDB( unsigned short usEventID, const T& data );
	void SendToChatLogDB( unsigned short usEventID );
#endif SERV_RECORD_CHAT
	//}}

#ifdef SERV_ID_NETMARBLE_PCBANG
	template < class T > void SendToIDPcbangDB( unsigned short usEventID, const T& data );
	void SendToIDPcbangDB( unsigned short usEventID );
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_ADD_EVENT_DB
	template < class T > void SendToEventDB( unsigned short usEventID, const T& data );
	void SendToEventDB( unsigned short usEventID );
#endif //SERV_ADD_EVENT_DB
	
    void SendToAccountDB( unsigned short usEventID );
    void SendToGameDB( unsigned short usEventID );
    //void SendToGameDB2nd( unsigned short usEventID );
    void SendToLogDB( unsigned short usEventID );
	void SendToSMSDB( unsigned short usEventID );
	//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
    void SendToNXWebDB( unsigned short usEventID );
#endif SERV_WEB_POINT_EVENT
	//}}
	//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
#ifdef SERV_RELAY_DB_CONNECTION
	void SendToRelayDB( unsigned short usEventID );
#endif SERV_RELAY_DB_CONNECTION
	//}}
    void SendToCnUser( unsigned short usEventID );
	//{{ 2009. 4. 24  최육사	대전유저리스트
	void SendToCnServer( unsigned short usEventID );
	//}}
    void SendToLoginAccountDB( unsigned short usEventID );
	void SendToLoginServer( unsigned short usEventID );
	void SendToCnRoom( unsigned short usEventID );
	//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	void SendToLoginRoom( unsigned short usEventID );
	void SendToPShopRoom( unsigned short usEventID );
#endif SERV_PSHOP_AGENCY
	//}}
	//{{ 2010. 02. 20  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY	
	void SendToCnParty( unsigned short usEventID );
#endif SERV_CHANNEL_PARTY
	//}}
	void SendToGSField( unsigned short usEventID );

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	void SendToGlobalServer( unsigned short usEventID );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가


#ifdef SERV_SKILL_PAGE_SYSTEM
	int GetTheNumberOfSkillPagesAvailable() const { return m_kSkillTree.GetTheNumberOfSkillPagesAvailable(); }
#endif // SERV_SKILL_PAGE_SYSTEM


	//////////////////////////////////////////////////////////////////////////
	// event handler
protected:
#   undef EVENT_HANDLER
#   define EVENT_HANDLER( eventid ) DECL_ON_FUNC( eventid );
#	undef EVENT_HANDLER_NOPARAM
#   define EVENT_HANDLER_NOPARAM( eventid ) DECL_ON_FUNC_NOPARAM( eventid );
#	undef _EVENT_HANDLER
#	define _EVENT_HANDLER( eventid, structtype ) _DECL_ON_FUNC( eventid, structtype );

	// 여기서 부터 GSUser Event Handler의 헤더 파일을 추가합니다.
#	include "GSUserSession.h"
#	include "GSUserSecurity.h"
#	include "GSUserGameCommon.h"
#	include "GSUserField.h"
	//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
#	include "GSUserNewParty.h"
#else
#	include "GSUserParty.h"
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
#	include "GSUserInventory.h"
#	include "GSUserMessenger.h"
#	include "GSUserRoomCommon.h"
#	include "GSUserDungeon.h"
#	include "GSUserPvp.h"
#	include "GSUserTrade.h"
#	include "GSUserPersonalShop.h"
#	include "GSUserCashShop.h"
#	include "GSUserPet.h"
#	include "GSUserGuild.h"
#	include "GSUserLetter.h"
#	include "GSUserAdminCheat.h"
#	include "GSuserMatch.h"
	//{{ 2011. 10. 27	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
#	include "GSUserBattleField.h"
#	include "GSUserAutoParty.h"
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
//#ifdef SERV_COUNTRY_CN
#	include "GSUserGiant.h"
//#endif SERV_COUNTRY_CN
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
#	include "GSUserRelationship.h"
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	//////////////////////////////////////////////////////////////////////////
	// event handling function, util function
protected:	
	bool RoutePacket( const KEvent* pkEvent );	// CnUser, GSUser가 같이 선언하지만 상속은 아님. ^ KEvent가 수정되지 않아야 한다. (SmartPtr로는 불가능)

    // override.
    virtual void ProcessEvent( const KEventPtr& spEvent );
	bool ProcessEventExpand( const KEventPtr& spEvent );
    virtual void OnDestroy();   // LOGINED 상태로 빠져나오고, DB data update를 신청한다 
    virtual bool CheckExceedRefCount( int nCount ); // derived from KSession.	

	//{{ 2009. 5. 29  최육사	함수 통합
	bool OnConnectReq( IN const KEGS_CONNECT_REQ& kReq, OUT KEGS_CONNECT_ACK& kAck );
	bool OnNexonUserAuthenticateAck( IN OUT KELG_NEXON_USER_AUTHENTICATE_ACK& kAck_, OUT KEGS_VERIFY_ACCOUNT_ACK& kAck );
	//{{ 2012. 12. 10  캐릭터 선택 패킷 분할 - 김민성
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
	bool OnAccountSelectUnitAck( IN OUT KDBE_SELECT_UNIT_ACK& kAck_, IN bool bIsChannelChange );
	void OnSelectUnitSuccess( IN const KDBE_SELECT_UNIT_ACK& kAck_, IN bool bIsChannelChange );
	void SendSelectUnitACK( IN const KDBE_SELECT_UNIT_ACK& kPacket );
#else
	bool OnAccountSelectUnitAck( IN OUT KEGS_SELECT_UNIT_ACK& kAck_, IN bool bIsChannelChange );
	void OnSelectUnitSuccess( IN const KEGS_SELECT_UNIT_ACK& kAck_, IN bool bIsChannelChange );
#endif SERV_SELECT_UNIT_PACKET_DIVISION
	//}}
	//}}
	//{{ 2012. 11. 22	최육사		캐릭터 선택 해제 코드 리팩토링
#ifdef SERV_DESELECT_UNIT_CODE_REFACTORING
	void OnDeselectUnit( IN const u_short usEventID, IN const bool bOnDestroy );
#else
	void OnDeselectUnit( IN const bool bFinal, IN const u_short usEventID );
#endif SERV_DESELECT_UNIT_CODE_REFACTORING
	//}}

	//2008.01.29 hoons.. 반복이벤트 체크를 위한 임시함수.
	void CheckRepeatEvent( u_short usEventID );

	// 스킬개편
	void InitTransactionEventFilter();
	bool IsTransactionEnabled( unsigned short usEventID );
	void CheckTransactionReqEvent( unsigned short usEventID );
	void CheckTransactionAckEvent( unsigned short usEventID );

	//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
	void ApplyAntiAddictionRatio(KEGS_NPC_UNIT_DIE_NOT& kPacket);
#endif SERV_ANTI_ADDICTION_SYSTEM
	//}}
	
#ifdef SERV_COUNTRY_TH
	//{{ 허상형 : [2012/10/17] //	AsiaSoft 인증
	void			SetSocketID( IN std::wstring wstrSocketID )	{ m_wstrSocektID = wstrSocketID; }
	std::wstring	GetSocketID()	{ return m_wstrSocektID; }
	//}} 허상형 : [2012/10/17] //	AsiaSoft 인증
#endif //SERV_COUNTRY_TH

#ifdef SERV_GLOBAL_BILLING
	void OnBuyCashItemAck( IN const KEGS_BUY_CASH_ITEM_ACK& kPacket_ );
	int OnGetPurchasedCashItemReq( IN const KEGS_GET_PURCHASED_CASH_ITEM_REQ& kPacket_, IN const KEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ& kPacket2_ );

	bool IsDirectPickUpItem( IN const int iItemID );
	int OnPickUpAck( IN const KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK& kPacket_, IN const KEBILL_PICK_UP_ACK& kPacket2_, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
	std::wstring MakeOrderID();
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	bool IsAbleToUseCashType(int iCashType);	// 국가별로 사용가능한 캐시 종류가 다름
#endif // SERV_SUPPORT_SEVERAL_CASH_TYPES
#endif // SERV_GLOBAL_BILLING
	
	//{{ 2012. 04. 30	박세훈	현자의 주문서 접속 이벤트 ( 우편함 중복 체크 )
#ifdef SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
		bool IsExistRewardInLetter( IN const int iRewardID ) const { return m_kUserLetterBox.IsExistRewardInLetter( iRewardID ); }
#endif SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
	//}}

	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	void AddTempInventoryItem( IN const int iItemID
							 , IN const u_char ucSealData
							 //{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
							 , IN const KItemInfo& kItemInfo
#endif SERV_ENCHANT_ITEM_DROP_EVENT
								//}}
							 );
	void SendQuestRewardLetter( IN const int iQuestID, IN const int iRewardItemID, IN const int iQuantity );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	int GetDungeonIDAndDungeonDifficulty()		{ return static_cast<int>( m_kUserDungeonManager.GetDungeonID() + m_kUserDungeonManager.GetDungeonDifficulty() ); }
#endif SERV_REFORM_QUEST
	//}}
//{{ 2012. 09. 25	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	int SetBingoEventInfo( IN const KDBE_BINGO_EVENT_INFO_READ_ACK& kPacket_ );
	void BingoEventDBWrite( void );
#endif SERV_EVENT_BINGO
	//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
	void SendExpandSkillPageReqToGameDB( const unsigned short usEventID_, const int iTheNumberOfSkillPagesToBeAdded_ );
#endif // SERV_SKILL_PAGE_SYSTEM


	//{{ 2012. 12. 26	박세훈	인벤토리 개편 테스트	- 허상형 ( Merged by 박세훈 )
#ifdef SERV_REFORM_INVENTORY_TEST
	int		GetSlotSize( IN const int iCategory ) const	{	return m_kInventory.GetSlotSize( iCategory );	}
	int		GetExpandedSlotSize( IN const int iCategory ) const	{	return m_kInventory.GetExpandedSlotSize( iCategory );	}
#endif SERV_REFORM_INVENTORY_TEST
	//}}

#ifdef SERV_ADD_WARP_BUTTON
	void			SetWarpVipEndDate( IN __int64 trDate_ ) { m_trWarpVipEndDate = trDate_; }
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_CHANNELING_USER_MANAGER
	void	SetGameServerLoginUser( IN const bool bValue ){	m_bGameServerLoginUser = bValue;	}
	bool	GetGameServerLoginUser( void ){	return m_bGameServerLoginUser;	}
#endif // SERV_CHANNELING_USER_MANAGER

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	void	CheckRidingPetProcess( void );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_PERIOD_PET
	void			CheckExpirePet();
#endif SERV_PERIOD_PET

private:

//---------------------------------------------------------------------------------------------------//
//----------- 2013.02.27 조효진	해외 기본 구조 작업 (SERV_GLOBAL_BASE)-------------------------------//
//---------------------------------------------------------------------------------------------------//
#ifdef SERV_COUNTRY_TH
	std::wstring					m_wstrSocektID;
#endif //SERV_COUNTRY_TH

#ifdef SERV_GLOBAL_BILLING
	KGlobalCashInfo					m_GlobalCashInfo;
#endif //SERV_GLOBAL_BILLING

	//#ifdef SERV_PERSONAL_SHOP_NO_MOVE	// 2012.07.03 lygan_조성욱 // 개인거래에 물품 등록된거 관리
	std::vector< KSellPersonalShopItemInfo >	m_vecPersonalShopSell_ItemInfo;
	//#endif //SERV_PERSONAL_SHOP_NO_MOVE

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	CTime							m_tReleaseTradeBlockTime;
#endif SERV_NEW_UNIT_TRADE_LIMIT

	// 2011.07.25 lygan_조성욱 // 중국용 커뮤니티탭 유저 정보 찾기
#ifdef SERV_USER_WATCH_NEW
	int								m_cCommunity_Tap_type;
#endif //SERV_USER_WATCH_NEW

#ifdef SERV_NPROTECT_CS_AUTH_30
	CCSAuth3						m_CSAuth3;
#endif SERV_NPROTECT_CS_AUTH_30

#ifdef SERV_NEW_PUNISHMENT
	//{{ 박교현 [2010/08/23] 처벌 추가
	KPunishmentManager				m_PunishmentManager;
	//}}
#endif SERV_NEW_PUNISHMENT

#ifdef SERV_ADD_WARP_BUTTON
	__int64							m_trWarpVipEndDate;
#endif SERV_ADD_WARP_BUTTON

#ifdef SERV_CHINA_SPIRIT_EVENT
	int								m_arrChinaSpirit[6];
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_GLOBAL_MISSION_MANAGER
	int								m_iGlobalMissionTickCount;
#endif SERV_GLOBAL_MISSION_MANAGER
//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
	void	SetLanBugOutDisconnectCheck( IN const bool bSet );
	void	CheckLanBugOutDisconnect( void );

	void	InitBlockCount( IN const byte byteBlockCheckCount, IN const std::wstring& wstrBlockCheckResetDate );
	void	IncreaseBlockCount( void );
	bool	GetBlockCountChanged( OUT byte& byteBlockCheckCount, OUT std::wstring& wstrBlockCheckResetDate );
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
	void			IncreaseLocalRankingButtonClickCount( void ){	++m_usLocalRankingClickCount;	}
	unsigned short	GetLocalRankingButtonClickCount( void ) const{	return m_usLocalRankingClickCount;	}
	void			ResetLocalRankingButtonClickCount( void ){	m_usLocalRankingClickCount = 0;	}
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-01	// 박세훈
	void	HackUserAccountBlock( IN const byte byteType, IN const std::wstring& wstrReason, IN const std::wstring& wstrEndDate ) const;
	void	IncreaseCheckCount( IN const byte byteType, IN const std::wstring& wstrReason, IN const bool bAfterInited = false );
	byte	GetAccountBlockType( void ) const;
	void	GetAccountBlockReson( OUT std::wstring& wstrBlockReason ) const{	wstrBlockReason = m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason2;	}
	void	GetAccountBlockEndDate( OUT std::wstring& wstrBlockEndDate ) const{	wstrBlockEndDate = m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockEndDate;	}
#endif // SERV_HACKING_USER_CHECK_COUNT

#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	bool	isCanUseUnsealCashSkillItem( IN int iItemID );
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
	bool	isCanUseUnsealFinalitySkill( IN int iItemID, IN u_char ucLevel );
#endif // SERV_FINALITY_SKILL_SYSTEM

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
	void	GetBossFieldCreateInfo( IN const int iBattleFieldID, OUT KBossFieldCreateInfo& kBossFieldCreateInfo ) const;
	
	void	CreateBossFieldJoinInfo( OUT KBossFieldJoinInfo& kBossFieldJoinInfo ) const;
	void	SetBossFieldJoinInfo( IN const KBossFieldJoinInfo& kBossFieldJoinInfo )	{	m_kBossFieldJoinInfo = kBossFieldJoinInfo;	}
	void	GetBossFieldJoinInfo( OUT KBossFieldJoinInfo& kBossFieldJoinInfo ) const;
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_SKILL_PAGE_SYSTEM
	void	AddLogWhenSkillPagesNumberIsWrong( const WCHAR* wszLog_ );
#endif // SERV_SKILL_PAGE_SYSTEM


#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
public:
	void SetDungeonCount(int TempCount)
	{
		m_DungeonCount = TempCount;
	}
	int GetDungeonCount(void)
	{
		return m_DungeonCount;
	}
	void SetFieldMosterKillCount(int TempKill)
	{
		m_FieldMonsterKillCount = TempKill;
	}
	int GetFieldMonsterKillCOunt(void)
	{
		return m_FieldMonsterKillCount;
	}
	void SetDungeonClearUI(bool DungeonUI)
	{
		m_DungeonClearUI = DungeonUI;
	}
	bool GetDungeonClearUI(void)
	{
		return m_DungeonClearUI;
	}
	void SetFieldCountUI(bool FieldUI)
	{
		m_FieldCountUI = FieldUI;
	}
	bool GetFieldCountUI(void)
	{
		return m_FieldCountUI;
	}
	void SetButtonClickTime(CTime ButtonTime)
	{
		m_tButtonClickTime = ButtonTime;
	}
	CTime GetButtonClickTime(void)
	{
		return m_tButtonClickTime;
	}
	void SetRemaindTime(int TempRemaind)
	{
		m_RemaindTime = TempRemaind;
	}
	int GetRemaindTime(void)
	{
		return m_RemaindTime;
	}
	void SetStartButtonPush(bool PushButton)
	{
		m_ButtonStartUI = PushButton;
	}
	bool GetStartButtonPush(void)
	{
		return m_ButtonStartUI;
	}
	void SetCoboItemGive(bool ItemGive)
	{
		m_bCoboItemGive = ItemGive;
	}
	bool GetCoboItemGive(void)
	{
		return m_bCoboItemGive;
	}
	void RemaindTimeTick(void);
	void NextDayItemGive(void);
	void DungeonClearCountNot( IN const int iDungeonID, IN const std::map< UidType, bool >& mapSuitableLevelInfo);
	void FieldMonsterKillCountNot(int iMonsterID, UidType AttUnit);
	//레벨 10이상만 이벤트에 참여 할 수 있기 때문에 레벨 10되면 자동으로 UI 활성화 되야 한다.
	void CoboEventLimitLevelStart(void);
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
	void SetValentineItemCount(int iTempCount)
	{
		m_iValentineItemCount = iTempCount;
	}
	int GetValentineItemCount(void)
	{
		return m_iValentineItemCount;
	}
	///다음날이 되면 새롭게 갱신이 되야함(접속 중에)
	void NextValentineDungeonItemCount(void);
	///던전 클리어 확인해서 카운트 올려 주자
	void EventDungeonClearCountAdd(IN const int iDungeonID);
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
	bool	IsRecruit()		{ return m_kUserRecommendManager.IsRecruit(); }
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

private:	
    UidType                         m_nUnitUID;	
	u_int							m_uiKNMSerialNum;		// 넥슨코리아 메신져 사용을 위한 시리얼 번호
    char                            m_cUnitClass;
    std::wstring                    m_wstrNickName;
	//{{ 2011. 08. 09  김민성 KAccontInfo 추가로 삭제됨
#ifdef SERV_NEW_CREATE_CHAR_EVENT
#else
	std::wstring                    m_wstrUserName;
#endif SERV_NEW_CREATE_CHAR_EVENT
	//}}

	//{{ 2012. 09. 28	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	KUserEDManager					m_kEDManager;
#else
	KCacheDataMaxLimit< int, __int64 > m_iED;
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

    u_char                          m_ucLevel;
	//{{  2011.11.24     김민성    캐릭터 접속 로그
#ifdef SERV_CHAR_CONNECT_LOG
	u_char                          m_ucLoginLevel;
	CTime							m_tCurUnitLoginTime;
#endif SERV_CHAR_CONNECT_LOG
	//}}

	KUserExpManager					m_kEXP;
	
#ifdef SERV_SKILL_PAGE_SYSTEM
#else // SERV_SKILL_PAGE_SYSTEM
	KCacheData< int >				m_iSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM


	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	KUserPvpManager					m_kUserPvpManager;
#else
	KCacheData< int >               m_iVSPoint;
	KCacheData< int >				m_iVSPointMax;
#endif SERV_PVP_NEW_SYSTEM
	//}}	
	//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	std::set< UidType >				m_setMyUnitUIDList;
#endif SERV_CHAR_LOG
	//}}
	//{{ 2012. 02. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KUserUnitManager				m_kUserUnitManager;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KUserSpiritManager				m_kUserSpiritManager;
#else
	// 근성도
	int								m_iSpiritMax;
	int								m_iSpirit;	
	bool							m_bIsSpiritUpdated;		// 근성도 하루치를 다 썼을경우 true
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
#else
	CXSLUnit::PVP_EMBLEM			m_ePVPEmblem;
#endif SERV_PVP_NEW_SYSTEM
	//}}	

    int                             m_nStraightVictories;
    u_short                         m_usInventorySize;
    u_char                          m_ucQuickSlotSize;

    KStat							m_kStat;
    
    KInventory                      m_kInventory;
	//{{ 2009. 5. 30  최육사	임시 인벤토리
	KTempInventory					m_kTempInventory;
	//}}
	int                             m_nUnitSlot;            // 캐릭터 슬롯 개수

	//{{ 2012. 01. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KLastPositionInfo				m_kLastPos;
#else
	UINT							m_nMapID;				// 현재위치 맵 ID
	u_char							m_ucLastTouchLineIndex; // 현재위치 LineIndex
	u_short							m_usLastPosValue;		// 현재위치 PosValue
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
#else
	int								m_iWin;
	int								m_iLose;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KCacheData< int >               m_iNumResurrectionStone;// 부활석.
	//{{ 2009. 10. 14  최육사	자동결제 부활석
#ifdef AP_RESTONE
	KCacheData< int >               m_iNumAutoPaymentResStone;// 자동결제 부활석.
	CTime							m_tAutoPaymentCheckTime; // 자동결제 빌링패킷 전송예약시각
#endif AP_RESTONE
	//}}

	UINT							m_uiRoomListID;
	UidType                         m_iRoomUID;
	//{{ 2012. 03. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	UidType							m_iBeforeRoomUID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 12. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KUserPartyManager				m_kUserPartyManager;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	UidType							m_iTradeUID;
	UidType							m_iPersonalShopUID;
	//{{ 2008. 11. 4  최육사	광장개편
	UidType							m_iFieldUID;
	//}}

	KTimerManager					m_kTimer[TM_TOTAL_NUM];
	//{{ 2011. 08. 09  김민성 KAccontInfo 추가로 삭제됨
#ifdef SERV_NEW_CREATE_CHAR_EVENT
#else
	char							m_cAuthLevel;
#endif SERV_NEW_CREATE_CHAR_EVENT
	//}}

    CTime                           m_tConnectTime;
#ifdef SERV_LUNITCONNECTLOG_BUG_FIX // 2014.01.28 darkstarbt_조성욱 // 캐릭터 접속로그용으로 따로 하나 만들기 기존 사용 변수는 계정용 ( 국내 사용하는 변수가 있지만 해외랑 국내랑 따로 가기 때문에 개별 발급함 )
	CTime							m_tCurUnitLoginTime_INT;
#endif //SERV_LUNITCONNECTLOG_BUG_FIX

    std::map< int, KDungeonClearInfo >	m_mapDungeonClear;
	std::map< int, KTCClearInfo >		m_mapTCClear;
#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
	std::map< int, KDungeonPlayInfo >	m_mapDungeonPlay;
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

	//{{ 2011. 08. 09  김민성 (2011.08.11) 특정일 이후 생성한 계정에 대하여 신규케릭터 생성 시 아이템 지급 이벤트
#ifdef SERV_NEW_CREATE_CHAR_EVENT
	KAccountInfo					m_kAccountInfo;
#endif SERV_NEW_CREATE_CHAR_EVENT
	//}}

    KNexonAccountInfo               m_kNexonAccountInfo;
	KAccountBlockInfo				m_kAccountBlockInfo;	//080318.hoons.. 접속시 블록유저면 받아 놓는다.
	
    bool                            m_bLoginFail;
    bool                            m_bLogoutFail;

	KUserQuestManager				m_kUserQuestManager;

	//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	KUserAccountQuestManager				m_kUserAccountQuestManager;
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}

	KUserSkillTree					m_kSkillTree;
	
	KUserGameOption					m_kUserGameOption;

    // 로그인서버에 접속 종료를 알릴지 결정한다.
    // 이중접속감지나 또는 채널이동으로 끊기는 경우 로그인서버에 접속 종료를 알릴 필요가 없다.
    bool                            m_bReportDisconnectToLogin;
	//{{ 2011. 08. 09  김민성 KAccontInfo 추가로 삭제됨
#ifdef SERV_NEW_CREATE_CHAR_EVENT
#else
	bool                            m_bInternalUser;
#endif SERV_NEW_CREATE_CHAR_EVENT
	//}}

	// korea nProtect 인증모듈 사용변수.
	//bool							m_bnProtectBegin;
	//KGGAuth						m_kGGAuth;

	//{{ 2009. 8. 18  최육사	hack shield
	KHSAuth							m_kHSAuth;
	//}}
	// 지헌 : XTRAP 서버 - CXTrapCSM 선언
#ifdef SERV_USE_XTRAP
	CXTrapCSM						m_cXTrapCSM;
#endif SERV_USE_XTRAP

	//{{ 2010. 01. 29  최육사	PC방 상용화
	KUserPcBangManager				m_kUserPcBangManager;
	//}}    

#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
	bool							m_bSC1;
	bool							m_bSC2;
#endif //SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT

	// 유저통계
	KUserStatistics					m_kUserStatistics;

    static KMaxProcessTime          ms_kMaxLoginTime;

	KRepeatEvent					m_kRepeatEvent;
	
	double							m_dResolveJackpotTime;	// 분해 대박

	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	KUserHackingManager				m_kUserHackingManager;
#endif SERV_HACKING_TOOL_LIST
	//}}

	//080405. hoons. 개인사제 데이터 관리
	KUserTutorialInfo				m_kUserTutorialInfo;

	// 장바구니
	KUserWishList					m_kUserWishList;

	// 시간 이벤트
	KUserEventManager				m_kUserEventManager;

	// 어뷰저 여부
	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	KUserAbuserManager				m_kUserAbuserManager;
#endif SERV_USER_ABUSER_MANAGER
	//}}
	char							m_cHackingUserType;

	// 캐쉬인벤
	KUserCashInventory				m_kUserCashInventory;

	//{{ 2009. 7. 7  최육사		랭킹개편
	KUserRankingManager				m_kUserRankingManager;
	//}}

	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
//#ifdef SERV_REPEAT_FILTER_REFAC
	KRepeatEventFilterNew			m_kRepeatEventFilter;
//#endif SERV_REPEAT_FILTER_REFAC
	//}}

	KTransactionGuarantor			m_kTransactionGuarantor;

	//{{ 2008. 9. 18  최육사	우편함
	KUserLetterBox					m_kUserLetterBox;
	//}}

	//{{ 2008. 10. 6  최육사	타이틀
	KUserTitleManager				m_kUserTitleManager;
	//}}

	//{{ 2008. 12. 9  최육사	필드 옵션
	KFieldOption					m_kFieldOption;
	//}}

	//{{ 2009. 4. 7  최육사		친구정보
	std::map< std::wstring, UidType > m_mapFriendList;
	//}}

    KMessengerInfo                  m_kMessengerInfo;

	//{{ 2009. 9. 13  최육사	유저 로그
#ifdef USER_LOG
	KUserLogManager					m_kUserLogManager;
#endif USER_LOG
	//}}

	//{{ 2009. 9. 22  최육사	길드
#ifdef GUILD_TEST
	KUserGuildManager				m_kUserGuildManager;
#endif GUILD_TEST
	//}}

	//{{ 2010. 01. 08  최육사	추천인리스트
	KUserRecommendManager			m_kUserRecommendManager;
	//}}

	//{{ 2010. 07. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	KUserPetManager					m_kUserPetManager;
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	KDungeonRequired				m_kDungeonRequired;
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}

	//{{ 2011. 04. 11	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	KUserPersonalShopManager		m_kUserPersonalShopManager;
#endif SERV_PSHOP_AGENCY
	//}}
		
	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	int								m_iWorldMissionTickCount;
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
	bool							m_bUseSecondPW;				// 2차 보안을 사용 중인가?
	std::wstring					m_wstrSecondPW;				// 2차 비밀번호
	int								m_iFailedCount;				// 2차 보안 실패 횟수
	CTime							m_tLastAuthDate;			// 최근 인증 받은 시간
	bool							m_bPassSecondSecurity;		// 2차 보안 인증 성공	
	bool							m_bIsUseOTP;				// OTP 사용 여부
	int								m_iNexonSecurityType;		// 게임 접속시 사용한 넥슨 보안
	KUserUnitSelectRewardManager	m_kUserUnitSelectRewardManager;	// 접속시 지급 아이템
	//{{ 2011. 08. 09  김민성 KAccontInfo 추가로 삭제됨
#ifdef SERV_NEW_CREATE_CHAR_EVENT
#else
	std::wstring					m_wstrLoginDate;			// 로그인 시간
#endif SERV_NEW_CREATE_CHAR_EVENT
	//}}
#endif SERV_SECOND_SECURITY
	//}}

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	KComeBackUserManager			m_kComeBackUserInfo;
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
	KUserInfoByNexon				m_kUserInfoByNexon;
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 
	//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
	KUserSecurityManager			m_kUserSecurityManager;
#endif SERV_MACHINE_ID_BLOCK
	//}}

	//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
	AntiAddictionManager			m_AntiAddictionManager;
	CTime                           m_tUnitSelectTime;
	KGiantAccountType				m_GiantAccountType;
#endif SERV_ANTI_ADDICTION_SYSTEM
	//}} 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
	bool							m_bIdentityConfirmCheck;
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	bool							m_bNickNameCheckOnly;
	UidType							m_iBackUpUnitUID;
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

#ifdef SERV_EPAY_SYSTEM
	std::wstring					m_wstrUserID; // 유저 로그인 계정
	unsigned short					m_usGiantZone; // 자이언트 정보
	unsigned short					m_usGiantGame; // 자이언트 정보
#endif //SERV_EPAY_SYSTEM

#ifdef SERV_GLOBAL_CASH_PACKAGE
	std::vector<int>				m_veciPackageBuyCheck; //2012.01.11 lygan_조성욱 // 캐쉬샵에서 동일 품목의 기간제 아이템 묶음에 대해서 한번만 캐릭터 인벤토리에 들어오게 체크용
#endif //SERV_GLOBAL_CASH_PACKAGE

	//{{ 2011. 08. 10    김민성    채널링 SOAP
#ifdef SERV_CHANNELING_SOAP
	std::wstring					m_wsrtToonilandID;
#endif SERV_CHANNELING_SOAP
	//}}

	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	KUserDungeonManager				m_kUserDungeonManager;
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2012. 02. 27	김민성	인벤토리 정렬	- 무거운 기능을 가볍게 변경
#ifdef SERV_SORT_CATEGORY_ITEM_FIX
	static KMaxProcessTime          ms_kMaxInventorySortTime;
#endif SERV_SORT_CATEGORY_ITEM_FIX
	//}}

	//{{ 2012. 03. 14	김민성		패킷 시퀀스 카운트 체크
#ifdef SERV_CHECK_SEQUENCE_COUNT
	bool						m_bCheckSequence;
#endif SERV_CHECK_SEQUENCE_COUNT
	//}}	
	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	KUserServerBuffManager			m_kUserBuffManager;
#endif SERV_SERVER_BUFF_SYSTEM
	//}

	//{{ 2012. 05. 16	박세훈	첫 접속 시 가이드 라인 띄워주기
#ifdef SERV_EVENT_GUIDELINE_POPUP
		bool							m_bPopupTheGuideLine;
#endif SERV_EVENT_GUIDELINE_POPUP
	//}}

		//{{ 2012. 06. 06	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	std::multimap<int /*iItemID*/, KDailyGiftBoxInfo>	m_mmapDailyGiftBoxList;
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	int								m_iEventMoney;
#endif // SERV_EVENT_MONEY

		//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
	bool							m_bNowPvpEventTime;
	bool							m_bIsPvpEventUser;
#endif SERV_2012_PVP_SEASON2_EVENT
		//}}

		//{{ 2012. 09. 22	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	KGSBingoEvent					m_kGSBingoEvent;
#endif SERV_EVENT_BINGO
		//}}
		//{{ 2012. 12. 5	최육사		옵저버 대전 난입 모드
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
	KUserCheatManager				m_kUserCheatManager;
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
		//}}
		//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	std::wstring					m_wstrWinterVacationEventRegDate;
	int								m_iWinterVacationEventCount;
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}

#ifdef SERV_INVISIBLE_GM
		bool							m_bInvisible;
#endif SERV_INVISIBLE_GM

		//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	KUserLocalRankingInfo			m_kUserLocalRankingInfo;
#endif SERV_LOCAL_RANKING_SYSTEM
		//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	bool							m_bHeroPvpUser;
#endif	// SERV_HERO_PVP_MANAGE_LIST

#ifdef SERV_NEW_EVENT_TYPES
		int								m_iMaxLevelTickCount;
		int								m_iActiveDungeonTickCount;
#endif //SERV_NEW_EVENT_TYPES		
		
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING
	std::vector<KPacketGetItemOnOff>	m_vecTypeBanItem;
	std::set< int >						m_secTypeBanItemOld;
	bool									m_bTimeControlItemCheckDungeonPlay;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CODE_EVENT
		int								m_iCodeEventTickCount;
#endif SERV_CODE_EVENT		

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	bool							m_bNewUnit;
	bool							m_bNewUnit2;
	bool							m_bCurrentUnit;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_CUSTOM_CONNECT_EVENT
		int								m_iCustomEventID;
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
		int								m_iConnectExperience;
		int								m_iRewardBonusItem;
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef SERV_CHANNELING_USER_MANAGER
		bool							m_bGameServerLoginUser;
#endif // SERV_CHANNELING_USER_MANAGER
		
#ifdef SERV_GROW_UP_SOCKET
		int								m_iExchangeCount;
#endif SERV_GROW_UP_SOCKET

#ifdef SERV_STEAM
		bool							m_bSteamClient;
#endif //SERV_STEAM

#ifdef SERV_CHANNELING_AERIA
		bool							m_bAeriaClient;
#endif //SERV_CHANNELING_AERIA

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING
		std::vector<KPacketGetItemOnOff>	m_vecTypeBanItem;
		std::set< int >						m_secTypeBanItemOld;
		bool									m_bTimeControlItemCheckDungeonPlay;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
		
#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
private:
	void _CheckSystemInfoStatistics();
	void _CheckKOGGamePerformanceStatistics();

	bool m_bSendDBSystemInfoStatistics;
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef SERV_COUNTRY_PH
	unsigned short						m_usGarenaCyberCafe;
#endif //SERV_COUNTRY_PH

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-20
	KUserRidingPetManager				m_kUserRidingPetManager;
#endif	// SERV_RIDING_PET_SYSTM

#ifdef	SERV_AFTER_WORK_STORAGE// 적용날짜: 2013-04-20
	KAfterWorkStorage					m_kAfterWorkStorage;
#endif	// SERV_AFTER_WORK_STORAGE

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	KUserRelationshipManager			m_kUserRelationshipManager;
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
	bool								m_bCheckUdpKickStateChangeFieldNot;
	bool								m_bBlockCheckCountChanged;
	byte								m_byteBlockCheckCount;
	std::wstring						m_wstrBlockCheckResetDate;
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
	unsigned short						m_usLocalRankingClickCount;
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-10	// 박세훈
	JumpingCharacter::KUserJumpingCharacterManager	m_kUserJumpingCharacterManager;
#endif // SERV_JUMPING_CHARACTER

#ifdef SERV_BLESS_OF_GODDESS_EVENT
	bool								m_bMaxLevelUnitInAccount;
#endif SERV_BLESS_OF_GODDESS_EVENT

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	int									m_iGateOfDarknessSupportEventTime;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#if defined(SERV_EVENT_RIDING_WITH_SUB_QUEST) || defined(SERV_RIDING_PET_WITH_SUB_QUEST)
	int									m_iRidingPetSummon;
	USHORT								m_usRidingSummonedPetID;
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST

#ifdef SERV_RELATIONSHIP_EVENT_INT
	bool								m_bCouple;
	UidType								m_iRelationTargetUserUid;
	std::wstring						m_wstrRelationTargetUserNickname;
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	CXSLDungeonSubStage::NextStageData	m_kNextStageData;
#endif // SERV_STAGE_CLEAR_IN_SERVER

#ifdef SERV_ELESIS_UPDATE_EVENT
	int									m_iNoteViewCount;
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_NEW_YEAR_EVENT_2014
	u_char								m_ucOldYearMissionRewardedLevel;
	int									m_iNewYearMissionStepID;
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_EVENT_CHECK_POWER
	unsigned char						m_ucCheckPowerCount;
	__int64								m_iCheckPowerTime;
	bool								m_bCheckPowerShowPopUp;
	unsigned char						m_ucCheckPowerScore;
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
	KBossFieldJoinInfo					m_kBossFieldJoinInfo;
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
public:
	bool							   m_bGiveItem;
	CTime							   m_cGetItemTime;
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_4TH_ANNIVERSARY_EVENT
	K4ThAnnivEventInfo					m_4ThAnnivEventInfo;	// 4주년 이벤트 출력용 정보( 첫 플레이 날 등..)
	std::vector<bool>					m_vec4ThAnnivEventRewardInfo;		// 4주년 이벤트 보상 받은 정보
	CTime								m_tLastRewardTime;	// 마지막으로 보상 받은 시간
	bool								m_bGetEventRewardInfo;	// DB에서 이벤트 보상 정보를 받아 왔느냐?
#endif // SERV_4TH_ANNIVERSARY_EVENT

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
public:
	bool								m_ButtonStartUI;
	bool								m_DungeonClearUI;
	bool								m_FieldCountUI;
	int									m_DungeonCount;
	int									m_FieldMonsterKillCount;
	CTime								m_tButtonClickTime;
	int									m_RemaindTime;
	bool								m_bCoboItemGive;
	std::set<int>						m_setMonsterID;
	int									m_iTimeCount; //10초마다 카운트 올려서 6되면 1분 되게 처리 하자
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
	int									m_iValentineItemCount;
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

#ifdef SERV_GLOBAL_EVENT_TABLE
	std::map< int, KGlobalEventTableData >	m_mapGlobalEventData;
	int										m_iDisableCodeEventTickCount;
#endif //SERV_GLOBAL_EVENT_TABLE

#ifdef SERV_STRING_FILTER_USING_DB
	int									m_iAppliedStringFilterReleaseTick;
#endif //SERV_STRING_FILTER_USING_DB
};

template < typename T >
bool KGSUser::SendPacket( unsigned short usEventID, T& data, bool bLogging, bool bCompress )
{
	//{{ 2010. 9. 2	최육사	중폭 패킷 필터 개선
//#ifdef SERV_REPEAT_FILTER_REFAC
	m_kRepeatEventFilter.CheckAckEvent( usEventID );
//#endif SERV_REPEAT_FILTER_REFAC
	//}}

	CheckTransactionAckEvent( usEventID ); // EGS_

	//{{ 2009. 9. 13  최육사	유저로그
#ifdef USER_LOG
	m_kUserLogManager.SaveSendPacket( usEventID );
#endif USER_LOG
	//}}

    return KSession::SendPacket( PI_GS_PROXY, GetUID(), NULL, usEventID, data, bLogging, bCompress );
}

template < class T >
void KGSUser::SendToAccountDB( unsigned short usEventID, const T& data ) const
{
    UidType anTrace[2] = { GetUID(), -1 };
    KncSend( PI_GS_USER, GetUID(), PI_ACCOUNT_DB, 0, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToGameDB( unsigned short usEventID, const T& data )
{
	if( false == IsTransactionEnabled( usEventID ) )
		return;
	CheckTransactionReqEvent( usEventID ); // DBE_

    UidType anTrace[2] = { GetUID(), -1 };
    KncSend( PI_GS_USER, GetUID(), PI_GS_GAME_DB, 0, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToGameDB2nd( unsigned short usEventID, const T& data )
{
    if( false == IsTransactionEnabled( usEventID ) )
        return;
    CheckTransactionReqEvent( usEventID ); // DBE_

    UidType anTrace[2] = { GetUID(), -1 };
    KncSend( PI_GS_USER, GetUID(), PI_GS_GAME_DB_2ND, 0, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToLogDB( unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { GetUID(), -1 };
    KncSend( PI_GS_USER, GetUID(), PI_LOG_DB, 0, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToSMSDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_GS_SMS_DB, 0, anTrace, usEventID, data );
}

//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT

template < class T >
void KGSUser::SendToNXWebDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_GS_NX_WEB_DB, 0, anTrace, usEventID, data );
}

#endif SERV_WEB_POINT_EVENT
//}}

//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
#ifdef SERV_RELAY_DB_CONNECTION
template < class T >
void KGSUser::SendToRelayDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_GS_JP_RELAY_DB, 0, anTrace, usEventID, data );
}
#endif SERV_RELAY_DB_CONNECTION
//}}

template < class T >
void KGSUser::SendToCnUser( unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { GetUID(), -1 };
    KncSend( PI_GS_USER, GetUID(), PI_CN_USER, 0, anTrace, usEventID, data );
}

//{{ 2009. 4. 24  최육사	대전유저리스트
template < class T >
void KGSUser::SendToCnServer( unsigned short usEventID, const T& data ) const
{
	UidType anTrace[2] = { 0, -1 }; // Trace가 0이면 같은ip의 센터로 전송한다
	KncSend( PI_GS_USER, GetUID(), PI_CN_SERVER, 0, anTrace, usEventID, data );
}
//}}

template < class T >
void KGSUser::SendToLoginAccountDB( unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { GetUID(), -1 };
    KncSend( PI_GS_USER, GetUID(), PI_LOGIN_ACCOUNT_DB, 0, anTrace, usEventID, data );
}

template < class T >
void KGSUser::_SendToCnRoom( UidType nTo, unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { GetCharUID(), -1 };
    KncSend( PI_GS_USER, GetCharUID(), PI_CN_ROOM, nTo, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToCnRoom( unsigned short usEventID, const T& data )
{
    _SendToCnRoom( GetRoomUID(), usEventID, data );
}

//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY

template < class T >
void KGSUser::_SendToLoginRoom( UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetCharUID(), -1 };
	KncSend( PI_GS_USER, GetCharUID(), PI_LOGIN_ROOM, nTo, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToLoginRoom( unsigned short usEventID, const T& data )
{
	_SendToLoginRoom( GetRoomUID(), usEventID, data );
}

template < class T >
void KGSUser::_SendToPShopRoom( UidType nTo, unsigned short usEventID, const T& data )
{
	const CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( nTo );
	switch( eRoomType )
	{
	case CXSLRoom::RT_PERSONAL_SHOP:
		_SendToCnRoom( nTo, usEventID, data );
		break;

	case CXSLRoom::RT_PSHOP_AGENCY:
		_SendToLoginRoom( nTo, usEventID, data );
		break;

	default:
		START_LOG( cerr, L"개인상점 방이 아닌데 패킷을 보내려고 하네?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPersonalShopUID() )
			<< BUILD_LOG( eRoomType )
			<< END_LOG;
		break;
	}
}

template < class T >
void KGSUser::SendToPShopRoom( unsigned short usEventID, const T& data )
{
	const CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( GetPersonalShopUID() );
	switch( eRoomType )
	{
	case CXSLRoom::RT_PERSONAL_SHOP:
		_SendToCnRoom( GetPersonalShopUID(), usEventID, data );
		break;

	case CXSLRoom::RT_PSHOP_AGENCY:
		_SendToLoginRoom( GetPersonalShopUID(), usEventID, data );
		break;

	default:
		START_LOG( cerr, L"개인상점 방이 아닌데 패킷을 보내려고 하네?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPersonalShopUID() )
			<< BUILD_LOG( eRoomType )
			<< END_LOG;
		break;
	}
}

#endif SERV_PSHOP_AGENCY
//}}

//{{ 2010. 02. 20  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY

template < class T >
void KGSUser::_SendToCnParty( UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetCharUID(), -1 };
	KncSend( PI_GS_USER, GetCharUID(), PI_CN_PARTY, nTo, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToCnParty( unsigned short usEventID, const T& data )
{
	_SendToCnParty( GetPartyUID(), usEventID, data );
}

#endif SERV_CHANNEL_PARTY
//}}

template < class T >
void KGSUser::_SendToGSField( UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetCharUID(), -1 };
	KncSend( PI_GS_USER, GetCharUID(), PI_GS_FIELD, nTo, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToGSField( unsigned short usEventID, const T& data )
{
	_SendToGSField( GetFieldUID(), usEventID, data );
}

template < class T >
void KGSUser::SendToLoginServer( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_LOGIN_USER, 0, anTrace, usEventID, data );
}

template < class T >
void KGSUser::_SendToCharacter( UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { -1, -1 }; // trace는 비운다, ProxyManager::SendPacket()을 호출하지 않기 위해
	KncSend( PI_GS_CHARACTER, GetCharUID(), PI_GS_CHARACTER, nTo, anTrace, usEventID, data );
}

#ifdef SERV_COUNTRY_CN
template < class T >
void KGSUser::SendToGiantRoleReg( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_LOGIN_GIANT_ROLEREG, 0, anTrace, usEventID, data );
}
#endif // SERV_COUNTRY_CN

#ifdef SERV_GLOBAL_BILLING
template < class T >
void KGSUser::SendToKOGBillingDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_GS_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToKOGLoginBillingDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_LOGIN_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToPublisherBilling( unsigned short usEventID, const T& data )
{
	// 원래 kncSend 로 해결하려고 했으나 PI 값을 오버로딩 하기 위해서는 
	// DBManager처럼 kncSend에서 목적지를 알수 있도록 map으로 목적지 관리가 필요하다. 
	// 현재 그런 구조로 확장하기에는 시간이 많이 걸려 아래처럼 대체 함수를 이용한 Queueing으로 처리함.
	// 단, PublisherBilling의 목적지가 한군데가 아닌 여러개라면 이구조로 커버 불가능함.

	UidType anTrace[2] = { GetUID(), -1 };
	KncSendToPublisherBilling( PI_GS_USER, GetUID(), PI_GS_PUBLISHER_BILLING, 0, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToPublisherBillingDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_GS_PUBLISHER_BILLING_DB, 0, anTrace, usEventID, data );
}
#endif // SERV_GLOBAL_BILLING

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
template < class T >
void KGSUser::SendToGlobalServer( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_GLOBAL_USER, 0, anTrace, usEventID, data );
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
template < class T >
void KGSUser::_SendToMatch( UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetCharUID(), -1 };
	KncSend( PI_GS_USER, GetCharUID(), PI_GLOBAL_MATCH_MAKING, nTo, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToMatch( unsigned short usEventID, const T& data )
{
	_SendToMatch( GetMatchUID(), usEventID, data );
}
#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2012. 02. 08	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
template < class T >
void KGSUser::_SendToAutoParty( UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetCharUID(), -1 };
	KncSend( PI_GS_USER, GetCharUID(), PI_GLOBAL_AUTO_PARTY, nTo, anTrace, usEventID, data );
}

template < class T >
void KGSUser::SendToAutoParty( unsigned short usEventID, const T& data )
{
	_SendToAutoParty( m_kUserDungeonManager.GetAutoPartyUID(), usEventID, data );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2010. 11. 12  조효진	GameForge 채팅 로그 관련 처리
#ifdef SERV_RECORD_CHAT
template < class T >
void KGSUser::SendToChatLogDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_GS_CHAT_LOG_DB, 0, anTrace, usEventID, data );
}
#endif SERV_RECORD_CHAT
//}}

#ifdef SERV_ID_NETMARBLE_PCBANG
template < class T >
void KGSUser::SendToIDPcbangDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_GS_ID_PUBLISHER_PCBANG_DB, 0, anTrace, usEventID, data );
}
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_ADD_EVENT_DB
template < class T >
void KGSUser::SendToEventDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_USER, GetUID(), PI_GS_EVENT_DB, 0, anTrace, usEventID, data );
}
#endif //SERV_ADD_EVENT_DB
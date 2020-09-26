#include "Statistics.h"

#include <Serializer/Serializer.h>
#include <Serializer/SerMap.h>
#include "KncSend.h"

#include "X2Data/XSLItem.h"
#include "X2Data/XSLItemManager.h"
#include "GSFSM.h"

#include <iomanip>

#undef _ENUM
#define _ENUM( id ) L#id,
const wchar_t* KStatistics::ms_szStatistics[] =
{
#include "StatisticsID_def.h"
};

ImplToStringW( KStatistics )
{
    stm_ << L"-> " << GetIDStr( m_iStatisticsID ) << std::endl;

    KStatisticsData::const_iterator mit;
    for( mit = m_kStatisticsData.begin(); mit != m_kStatisticsData.end(); mit++ )
    {
        std::vector< int >::const_iterator vitInt;
        std::vector< std::wstring >::const_iterator vitStr;
		std::vector< __int64 >::const_iterator vitInt64;

        for( vitInt = mit->first.m_vecIntKey.begin(); vitInt != mit->first.m_vecIntKey.end(); vitInt++ )
        {
            stm_ << *vitInt << L" ";
        }
        for( vitStr = mit->first.m_vecStringKey.begin(); vitStr != mit->first.m_vecStringKey.end(); vitStr++ )
        {
            stm_ << *vitStr << L" ";
        }
        for( vitInt64 = mit->second.begin(); vitInt64 != mit->second.end(); vitInt64++ )
        {
            stm_ << *vitInt64 << L" ";
        }
        stm_ << std::endl;
    }

    return stm_;
}

KStatistics::KStatistics()
{
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	m_bTimeFlush = false;
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}
}

KStatistics::~KStatistics()
{
}

const wchar_t* KStatistics::GetIDStr( int iStatistics )
{
    if( iStatistics < 0 || iStatistics > SI_SENTINEL )
    {
        return ms_szStatistics[SI_SENTINEL];
    }

    return ms_szStatistics[iStatistics];
}

void KStatistics::Init( int iStatisticsID, float fFlushGap, bool bWriteToDB )
{
    m_iStatisticsID = iStatisticsID;
    m_kStatisticsData.clear();
    m_kUpdateTimer.restart();
    m_fFlushGap = fFlushGap;
    m_bWriteToDB = bWriteToDB;
}

//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
void KStatistics::SetFlushTime( IN const int iHour )
{
	m_bTimeFlush = true;

	CTime tCurTime = CTime::GetCurrentTime();
	m_tFlushTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), iHour, 0, 0, 0 );
	if( m_tFlushTime < tCurTime )
	{
		m_tFlushTime += CTimeSpan( 1, 0, 0, 0 );
	}

	std::wstring wstrFlushTime = (const wchar_t*)m_tFlushTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	START_LOG( cout, L"특정 시각에 DB통계 Flush 시각 세팅!" )
		<< BUILD_LOG( m_iStatisticsID )
		<< BUILD_LOG( wstrFlushTime )
		<< BUILD_LOG( m_bTimeFlush );
}
#endif SERV_ITEM_STATISTICS_TO_DB
//}}

void KStatistics::Tick()
{
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	if( m_bTimeFlush )
	{
		// 1분마다 Flush시각을 체크합니다.
		if( m_kTimeFlushTimer.elapsed() < 60.0 )
			return;

		m_kTimeFlushTimer.restart();

		// Flush시각을 체크해봅니다.
		CTime tCurTime = CTime::GetCurrentTime();
		if( tCurTime < m_tFlushTime )
			return;

		// 다음날짜로 세팅!
		m_tFlushTime += CTimeSpan( 1, 0, 0, 0 );
        
		// Flush
		FlushData();
		return;
	}
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

    if( m_fFlushGap >= 0.f )
    {
        if( m_kUpdateTimer.elapsed() >= m_fFlushGap )
        {
            FlushData();
        }
    }
}

void KStatistics::IncreaseCount( const KStatisticsKey& kKey, int iIndex, int iCount )
{
    std::vector< __int64 > vecCount;
    for( int i = 0; i < iIndex; i++ )
    {
        vecCount.push_back( 0 );
    }
    vecCount.push_back( iCount );

    IncreaseCount( kKey, vecCount );
}

void KStatistics::IncreaseCount( const KStatisticsKey& kKey, const std::vector< __int64 >& vecCount )
{
    KStatisticsData::iterator mit = m_kStatisticsData.find( kKey );
    if( mit == m_kStatisticsData.end() )
    {
        m_kStatisticsData.insert( std::make_pair( kKey, vecCount ) );
    }
    else
    {
        std::vector< __int64 >::const_iterator vitSrc = vecCount.begin();
        std::vector< __int64 >::iterator vitDest = mit->second.begin();

        while( vitSrc != vecCount.end() )
        {
            if( vitDest == mit->second.end() )
            {
                while( vitSrc != vecCount.end() )
                {
                    mit->second.push_back( *vitSrc );
                    vitSrc++;
                }
            }
            else
            {
                ( *vitDest ) += ( *vitSrc );
                vitSrc++;
                vitDest++;
            }
        }
    }
}

void KStatistics::FlushData()
{
	switch ( m_iStatisticsID )
	{
	case KStatistics::SI_LOC_PVP:
	case KStatistics::SI_LOC_ITEM:
	case KStatistics::SI_LOC_DUNGEON:
	case KStatistics::SI_LOC_SPIRIT:
	case KStatistics::SI_LOC_ENCHANT:		
	case KStatistics::SI_LOC_PVP_VICTORY_PER:		
	case KStatistics::SI_LOC_PVP_ROOM:
	case KStatistics::SI_LOC_DUNGEON_ROOM:
	case KStatistics::SI_LOC_HENIR_RANKING:
	case KStatistics::SI_LOC_TITLE:
		//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	case KStatistics::SI_LOC_PET_SUMMON:
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
	case KStatistics::SI_LOC_SERV_DISCONNECT:
#endif SERV_SERVER_DISCONNECT_LOG
		//}}
		//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	case KStatistics::SI_LOC_BILLING_PACKET:
#endif SERV_BILLING_PACKET_LOG
		//}}
		//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	case KStatistics::SI_LOC_ABUSER_MORNITORING:
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
		//}}
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
	case KStatistics::SI_LOC_UNDEFINED_QUEST_TEMPLET:
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET

		return;
	}

	m_kUpdateTimer.restart();
	if( !m_kStatisticsData.empty() )
	{
		if( m_bWriteToDB )
		{
			KncSend( PC_SERVER, 0, PC_LOG_DB, 0, NULL, GetEventID(), m_kStatisticsData );
		}

		m_kStatisticsData.clear();
	}
}

void KStatistics::ClearLocalLogData()
{
	switch ( m_iStatisticsID )
	{
	case KStatistics::SI_LOC_PVP:
	case KStatistics::SI_LOC_ITEM:
	case KStatistics::SI_LOC_SPIRIT:		
	case KStatistics::SI_LOC_ENCHANT:
		{
			m_kUpdateTimer.restart();
			m_kStatisticsData.clear();
		}
		break;
	}
}

unsigned short KStatistics::GetEventID()
{
	switch( m_iStatisticsID )
	{
	case SI_PVP_MAP:
		return ELOG_STAT_PVP_MAP;
	case SI_FPS:
		return ELOG_STAT_FPS;
	case SI_RES_STONE:
		return ELOG_STAT_RES_STONE;
	case SI_ED:
		return ELOG_STAT_ED;
	case SI_QUEST:
		return ELOG_STAT_QUEST;
	case SI_DISCONNECT_REASON:
		return ELOG_STAT_DISCONNECT_REASON;
	case SI_DISCONNECT_USERFSM:
		return ELOG_STAT_DISCONNECT_USERFSM;
		//{{ 2011. 02. 28	최육사	사내 통합 통계
#ifdef SERV_CHAR_CREATE_DEL_STAT
	case SI_CHARACTER:
		return ELOG_STAT_CHARACTER;
#endif SERV_CHAR_CREATE_DEL_STAT
		//}}
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	case SI_ITEM_DB:
		return ELOG_STAT_ITEM_DB;
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}	

	default:
		START_LOG( cerr, L"통계 ID에 대한 event id가 대응되지 않았음." )
			<< BUILD_LOG( m_iStatisticsID )
			<< END_LOG;
		return EVENT_EVENTID_SENTINEL;
	}
}

void KStatistics::WriteLocalLogData( std::wfstream& fout )
{
	// 최대 컬럼수
	int iMaxColNum = 0;
	int iCurColNum = 0;

	KStatisticsData::const_iterator mit;
	for( mit = m_kStatisticsData.begin(); mit != m_kStatisticsData.end(); ++mit )
	{
		int iColCnt = 0;

		std::vector< __int64 >::const_iterator vitInt;
		for( vitInt = mit->second.begin(); vitInt != mit->second.end(); ++vitInt )
		{
			iColCnt++;
		}

		iMaxColNum = max( iMaxColNum, iColCnt );
	} 

	for( mit = m_kStatisticsData.begin(); mit != m_kStatisticsData.end(); ++mit )
	{
		std::vector< int >::const_iterator vitInt;
		std::vector< std::wstring >::const_iterator vitStr;

		fout << L"\n";

		// Key 출력
		for( vitInt = mit->first.m_vecIntKey.begin(); vitInt != mit->first.m_vecIntKey.end(); ++vitInt )
		{
			fout << std::setw(8) << std::setiosflags( std::ios::left ) << *vitInt << L" ";
		}
		for( vitStr = mit->first.m_vecStringKey.begin(); vitStr != mit->first.m_vecStringKey.end(); ++vitStr )
		{
			fout << std::setw(8) << std::setiosflags( std::ios::left ) << *vitStr << L" ";
		}

		fout << L"\t";

		// Data 출력
		iCurColNum = iMaxColNum;

		std::vector< __int64 >::const_iterator vitInt64;

		for( vitInt64 = mit->second.begin(); vitInt64 != mit->second.end(); ++vitInt64 )
		{
			fout << std::setw(8) << std::setiosflags( std::ios::left ) << *vitInt64 << L"\t";
			iCurColNum--;
		}

		for( int iC = 0; iC < iCurColNum; ++iC )
		{
			fout << std::setw(8) << std::setiosflags( std::ios::left ) << 0 << L"\t";
		}
	}
}

void KStatistics::InitLocalLogKeys()
{
	if ( m_iStatisticsID == KStatistics::SI_LOC_ITEM )
	{
		const std::map< int, CXSLItem::ItemTemplet >& mapItemTemplet = SiCXSLItemManager()->GetItemTempletContainer();
		std::map< int, CXSLItem::ItemTemplet >::const_iterator mit;
		for ( mit = mapItemTemplet.begin(); mit != mapItemTemplet.end(); ++mit )
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( mit->first );
			IncreaseCount( kKey, KStatistics::SI_ITEM_MAX - 1, 0 );
		}
	}	
}

int  KStatistics::GetDisconnectUserFSM( int iStateID )
{
	switch( iStateID ) 
	{
	case KGSFSM::S_INIT:
		return eSIColDU_S_INIT;

	case KGSFSM::S_UID_UPDATED:
		return eSIColDU_S_UID_UPDATED;

	case KGSFSM::S_LOGINED:
		return eSIColDU_S_LOGINED;

	case KGSFSM::S_SERVER_SELECT:
		return eSIColDU_S_SERVER_SELECT;

	case KGSFSM::S_CHECK_SECOND_PW:
		return eSIColDU_S_CHECK_SECOND_PW;

	case KGSFSM::S_FIELD_MAP:
		return eSIColDU_S_VILLAGE_MAP; // 이거 나중에 수정해야됨

	case KGSFSM::S_ROOM:
		return eSIColDU_S_ROOM;
	}

	START_LOG( cerr, L"존재하지 않는 GSFSM값입니다" )
		<< BUILD_LOG( iStateID )
		<< END_LOG;

	return eSIColDU_Num;
}

bool KStatistics::GetUnitClassPvpStatColumn( char cUnitClass, int& iColumn )
{
	switch( cUnitClass )
	{
	case CXSLUnit::UC_ELSWORD_SWORDMAN:		iColumn = KStatistics::SLP_UNITCLASS_ELSWORD;			return true;
	case CXSLUnit::UC_ARME_VIOLET_MAGE:		iColumn = KStatistics::SLP_UNITCLASS_AISHA;				return true;
	case CXSLUnit::UC_LIRE_ELVEN_RANGER:	iColumn = KStatistics::SLP_UNITCLASS_RENA;				return true;
	case CXSLUnit::UC_RAVEN_FIGHTER:		iColumn = KStatistics::SLP_UNITCLASS_RAVEN_FIGHTER;		return true;			
	case CXSLUnit::UC_EVE_NASOD:			iColumn = KStatistics::SLP_UNITCLASS_EVE_NASOD;			return true;
	case CXSLUnit::UC_ELSWORD_KNIGHT:		iColumn = KStatistics::SLP_UNITCLASS_SWORD_KNIGHT;		return true;
	case CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT:	iColumn = KStatistics::SLP_UNITCLASS_MAGIC_KNIGHT;		return true;
	case CXSLUnit::UC_LIRE_COMBAT_RANGER:	iColumn = KStatistics::SLP_UNITCLASS_COMBAT_RANGER;		return true;
	case CXSLUnit::UC_LIRE_SNIPING_RANGER:	iColumn = KStatistics::SLP_UNITCLASS_SNIPING_RANGER;	return true;
	case CXSLUnit::UC_ARME_HIGH_MAGICIAN:	iColumn = KStatistics::SLP_UNITCLASS_HIGH_MAGICIAN;		return true;
	case CXSLUnit::UC_ARME_DARK_MAGICIAN:	iColumn = KStatistics::SLP_UNITCLASS_DARK_MAGICIAN;		return true;
	case CXSLUnit::UC_RAVEN_SOUL_TAKER:		iColumn = KStatistics::SLP_UNITCLASS_SOUL_TAKER;		return true;
	case CXSLUnit::UC_RAVEN_OVER_TAKER:		iColumn = KStatistics::SLP_UNITCLASS_OVER_TAKER;		return true;		
	case CXSLUnit::UC_EVE_EXOTIC_GEAR:		iColumn = KStatistics::SLP_UNITCLASS_EXOTIC_GEAR;		return true;
	case CXSLUnit::UC_EVE_ARCHITECTURE:		iColumn = KStatistics::SLP_UNITCLASS_ARCHITECTURE;		return true;
		//{{ 2009. 10. 24  최육사	엘소드2차전직
	case CXSLUnit::UC_ELSWORD_LORD_KNIGHT:	iColumn = KStatistics::SLP_UNITCLASS_ELSWORD;			return true;
	case CXSLUnit::UC_ELSWORD_RUNE_SLAYER:	iColumn = KStatistics::SLP_UNITCLASS_ELSWORD;			return true;
		//}}
		//{{ 2009. 11. 23  최육사	레나2차전직
	case CXSLUnit::UC_LIRE_WIND_SNEAKER:	iColumn = KStatistics::SLP_UNITCLASS_RENA;				return true;
	case CXSLUnit::UC_LIRE_GRAND_ARCHER:	iColumn = KStatistics::SLP_UNITCLASS_RENA;				return true;
		//}}
		//{{ 2009. 12. 11  최육사	아이샤 2차 전직
	case CXSLUnit::UC_ARME_ELEMENTAL_MASTER:	iColumn = KStatistics::SLP_UNITCLASS_AISHA;			return true;
	case CXSLUnit::UC_ARME_VOID_PRINCESS:	iColumn = KStatistics::SLP_UNITCLASS_AISHA;				return true;
		//}}
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CXSLUnit::UC_ELSWORD_INFINITY_SWORD:	iColumn = KStatistics::SLP_UNITCLASS_ELSWORD;		return true;
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CXSLUnit::UC_ARME_DIMENSION_WITCH:	iColumn = KStatistics::SLP_UNITCLASS_AISHA;				return true;
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CXSLUnit::UC_LIRE_NIGHT_WATCHER:	iColumn = KStatistics::SLP_UNITCLASS_RENA;				return true;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CXSLUnit::UC_RAVEN_VETERAN_COMMANDER:	iColumn = KStatistics::SLP_UNITCLASS_RAVEN_FIGHTER;	return true;
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CXSLUnit::UC_EVE_BATTLE_SERAPH:	iColumn = KStatistics::SLP_UNITCLASS_EVE_NASOD;		return true;
#endif
	}

	return false;
}

const wchar_t* KStatistics::GetDissconnectReasonString( int iReason )
{
	switch( iReason )
	{
	case eSIColDR_No_Reason:							return L"No_Reason [이유 없음]";
	case eSIColDR_Unknown:								return L"Unknown [Unknown]";
	case eSIColDR_HackShield_Auth_Fail:					return L"HackShield_Auth_Fail [HackShield 인증 실패 (클라이언트 응답 없음)]";
	case eSIColDR_PCBang_Auth_Delay:					return L"PCBang_Auth_Delay [PC방 인증 지연]";
	case eSIColDR_Diff_Main_Ver:						return L"Diff_Main_Ver [메인버전 다름]";
	case eSIColDR_Limit_User_Num:						return L"Limit_User_Num [수용 인원 초과]";
	case eSIColDR_Nexon_Auth_Fail:						return L"Nexon_Auth_Fail [넥슨 인증 실패]";
	case eSIColDR_Same_Server_Connect:					return L"Same_Server_Connect [동일 서버 이중 접속]";
	case eSIColDR_Account_Block:						return L"Account_Block [계정 블록]";
	case eSIColDR_IP_Block:								return L"IP_Block [IP 제한]";
	case eSIColDR_NProtect_Auth_First:					return L"NProtect_Auth_First [NProtect 처음 인증]";
	case eSIColDR_PCBang_Auth_Fail:						return L"PCBang_Auth_Fail [PC방 인증 실패]";
	case eSIColDR_PCBang_Auth_Wrong:					return L"PCBang_Auth_Wrong [PC방 인증 결과 이상]";
	case eSIColDR_Internal_Account:						return L"Internal_Account [사내 계정 생성]";
	case eSIColDR_Normal_Disconnect:					return L"Normal_Disconnect [정상 종료]";
	case eSIColDR_Client_Crash:							return L"Client_Crash [클라이언트 크래쉬]";
	case eSIColDR_Client_Hacking:						return L"Client_Hacking [클라이언트 해킹]";
	case eSIColDR_HackShield_Wrong_Ack:					return L"HackShield_Wrong_Ack [HackShield 인증 실패 (응답 틀림)]";
	case eSIColDR_PCBang_Auth_Server:					return L"PCBang_Auth_Server [PC방 인증 서버의 종료 요청]";
	case eSIColDR_Server_Shutdown:						return L"Server_Shutdown [서버 Shutdown]";
	case eSIColDR_Session_Close:						return L"Session_Close [접속 종료 감지 (Session)]";
	case eSIColDR_WSAECONNRESET:						return L"WSAECONNRESET [WSAECONNRESET (Socket Object)]";
	case eSIColDR_WSARecvFailed:						return L"WSARecvFailed [WSARecvFailed (Socket Object)]";
	case eSIColDR_WSASendFailed_OnSendComp:				return L"WSASendFailed_OnSendComp [WSASendFailed (Socket Object) - OnSendCompleted()]";
	case eSIColDR_SendBufferFull:						return L"SendBufferFull [SendBufferFull (Socket Object)]";
	case eSIColDR_WSASendFailed:						return L"WSASendFailed [WSASendFailed (Socket Object) - SendData()]";
	case eSIColDR_GM_Kick_User:							return L"GM_Kick_User [운영자 강제 접속 종료]";
	case eSIColDR_Diff_Server_Connect:					return L"Diff_Server_Connect [타 서버간 이중접속]";
	case eSIColDR_User_Reg_Error:						return L"User_Reg_Error [유저 등록 중 에러]";
	case eSIColDR_Heart_Beat:							return L"Heart_Beat [하트 비트]";
	case eSIColDR_IOCP_GetStatus:						return L"IOCP_GetStatus [IOCP_GetStatus Closed by local machine]";
	case eSIColDR_nProtect_Hacking:						return L"nProtect_Hacking [nProtect 해킹 감지]";
	case eSIColDR_Channel_Change:						return L"Channel_Change [채널이동으로 인한 접속종료]";
	case eSIColDR_Channel_Change_Fail:					return L"Channel_Change_Fail [채널이동 실패로 인한 접속종료]";
		//{{ 2009. 10. 22  최육사	트래픽 공격 방지
	case eSIColDR_Traffic_Attack:						return L"Traffic_Attack [트래픽 유발로 인한 접속종료]";
		//}}
		//{{ 2010. 06. 22  최육사	계정블럭기능
	case eSIColDR_GameServer_Account_Block:				return L"GameServer_Account_Block [게임서버 계정 블록]";
		//}}
		//{{ 2011. 08. 09	최육사	머신ID 블럭기능
		//#ifdef SERV_MACHINE_ID_BLOCK
	case eSIColDR_MachineID_Block:						return L"MachineID_Block [머신ID 블럭]";
		//#endif SERV_MACHINE_ID_BLOCK
		//}}
		//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
		//#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	case eSIColDR_Same_MachineID_Connect:				return L"Same_MachineID_Connect [머신ID 중복 접속]";
		//#endif SERV_MACHINE_ID_DUPLICATE_CHECK
		//}}
		//{{ 2011. 09. 29  김민성	셧다운제 도입
		//#ifdef SERV_SHUTDOWN_SYSTEM
	case eSIColDR_Policy_Shutdown_Disconnect:			return L"Policy_Shutdown_Disconnect [셧다운제로 인한 접속 불가]";
		//#endif SERV_SHUTDOWN_SYSTEM
		//}}
	case eSlColDR_Wrong_Account_or_Password:			return L"Wrong_Account_or_Password [아이디 또는 비밀번호 틀림 (TW)]";
		//{{ 허상형 : [2011/2/16/] //	SHA 체크섬
		//#ifdef SERV_USE_CHECKSUM_WITH_SHA
	case eSlColDR_Wrong_SHA_Checksum:					return L"Wrong_SHA_Checksum [SHA 체크섬 틀림 (TW)]";
		//#endif SERV_USE_CHECKSUM_WITH_SHA
		//}} 허상형 : [2011/2/16/] //	SHA 체크섬

	case eSIColDR_Account_Invalid_Selling_Kill:			return L"Account_Invalid_Selling_Kill [팔기불능 아이템을 판다고 패킷날리는 핵유저 강제 종료 (TW)]";
	case eSIColDR_Clear_Time_Hack_User:					return L"Clear_Time_Hack_User [클리어타임 5초이하 핵 의심 유저로 판별 (TW)]";

		//{{ 2011.03.20 lygan_조성욱 // HProtect
		//#ifdef SERV_USE_HPROTECT
	case eSIColDR_Error_Check_Alive1:					return L"Error_Check_Alive1 [A type heartbeat validation error]";
	case eSIColDR_Error_Check_Alive2:					return L"Error_Check_Alive2 [B type heartbeat validation error]";
	case eSIColDR_Error_Wait_Check_Time_Out:			return L"Error_Wait_Check_Time_Out [The client did not respond to packet too long]";
	case eSIColDR_Error_Client_Speed:					return L"Error_Client_Speed [Client speed abnormally]";
	case eSIColDR_Error_Client_Scan_Result:				return L"Error_Client_Scan_Result [wParam point to TOOLSSCANRESULT]";
	case eSIColDR_Error_Client_Version:					return L"Error_Client_Version [unsigned int wParam type allows for the smallest version of the type unsigned int lParam is the client version number]";
	case eSIColDR_Error_Server_Lost_File:				return L"Error_Server_Lost_File [wParam point to file path]";
	case eSIColDR_Error_Client_Protocol:				return L"Error_Client_Protocol [Agreement send data packets when checking failure]";
	case eSIColDR_Error_Client_Sign_Hash:				return L"Error_Client_Sign_Hash [클라이언트 특징 파일에 에러가있음]";
	case eSIColDR_Error_Wait_Client_Version_Time_Out:	return L"Error_Wait_Client_Version_Time_Out [In 2 minutes, fail to get the client version, unable to initialize the protection system]";
		//#endif SERV_USE_HPROTECT
		//}}
		//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
		//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	case eSIColDR_Check_ServerSN_In_GameServer:			return L"Check_ServerSN_In_GameServer [게임서버에서 ServerSN 검사로 인하여 강제 종료]";
		//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		//}}
		//{{ 2012. 02. 01	최육사	recv buffer full
		//#ifdef SERV_CHECK_RECV_BUFFER_FULL
	case eSIColDR_RecvBufferFull:						return L"RecvBufferFull [수신된 패킷의 길이가 recv buffer크기를 넘어서는 경우]";
		//#endif SERV_CHECK_RECV_BUFFER_FULL
		//}}		
		//{{ 2012. 02. 14    김민성   넥슨 안심 로그인 - 목표 위치기반 로그인
		//#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
	case eSIColDR_PeaceOfMindLogin:						return L"PeaceOfMindLogin [넥슨 안심 로그인으로 블럭 처리된 유저]";
		//#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
		//}}
		//{{ 2012. 05. 23	김민성       판매 불가 아이템 판매 시도 해킹 유저로 판단
		//#ifdef SERV_IMPOSSIBLE_SELL_ITEM
	case eSIColDR_ImpossibleSellItem:					return L"ImpossibleSellItem [판매 불가 아이템 팔기]";
		//#endif SERV_IMPOSSIBLE_SELL_ITEM
		//}}
		//#ifdef SERV_NPROTECT_CS_AUTH_30
	case eSIColDR_nProtect_Error:						return L"nProtect_Error [nProtect 에러]";
		//#endif SERV_NPROTECT_CS_AUTH_30
		//{{
		//#ifdef SERV_MOVE_DATA_QUE2BUF_FAIL  
	case eSIColDR_MovDataQue2Buf_Failed:				return L"MovDataQue2Buf_Failed [MovDataQue2Buf 부분에서 비정상적인 버퍼 크기로 인해 게임서버 크래쉬 나는 부분 수정]";
		//#endif //SERV_MOVE_DATA_QUE2BUF_FAIL
		//}}
		//#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
	case eSIColDR_LanBugOutDisconnect:					return L"LanBugOutDisconnect [랜선렉 감시에 걸렸음에도 이를 무시하여 접속 종료 처리한 경우]";
		//#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

		//#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
	case eSIColDR_FieldWorkingCheck:					return L"FieldWorkingCheck [필드 작업장 체크 기능]";
		//#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

		//#ifdef SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT // 작업날짜: 2013-09-04 // 오현빈
	case eSIColDR_InvlideCheckKomScript:				return L"InvlideCheckKomScript [CheckKom.xml 스크립트 파싱 오류]";
		//#endif // SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT

		//#ifdef SERV_KOM_FILE_CHECK_ADVANCED		// 작업날짜: 2013-11-06 // 임홍락
	case eSIColDR_InvaildKomDetect:						return L"InvaildKomDetect [Kom 변조 감지]";
		//#endif SERV_KOM_FILE_CHECK_ADVANCED

	default:
		return L"정의 되지 않은 종료 사유입니다.";
	}
}



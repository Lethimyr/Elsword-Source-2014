#include "GSUser.h"
#include "HackingProtectionManager.h"
//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
#include "GameSysVal/GameSysVal.h"
#endif SERV_MONITORING_LETTER_AND_TRADE
//}}

//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST


KUserHackingManager::KUserHackingManager(void)
{
	// 초기값 지정
	SetRandomCheckTime();

	//{{ 2011. 04. 26	최육사	deserialize 실패 종료
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
	m_bCheckedDeserializeFail = false;
	m_iTotalDeserializeFailCount = 0;
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
	//}}

	//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	m_setRecvLetterList.clear();
	m_setRecvTradeList.clear();
#endif SERV_MONITORING_LETTER_AND_TRADE
	//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
	m_bEDMonitoringMode = false;
#endif SERV_ED_MONITORING_IN_GAME
	//}}
	//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	m_cLastSendType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;
	m_tHackingUserTimer.restart();
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	//}}
}

KUserHackingManager::~KUserHackingManager(void)
{
}

void KUserHackingManager::SetRandomCheckTime()
{
	int iRandNum = rand() % static_cast<int>(HE_MAX_TIME - HE_MIN_TIME);
	iRandNum += static_cast<int>(HE_MIN_TIME);

	m_fHackingToolCheckTime = static_cast<double>(iRandNum);
}

void KUserHackingManager::GetHackingToolList( OUT int& iRandomKey, OUT std::vector< KHackingToolInfo >& vecHackingToolList )
{
	// 해킹툴 리스트를 얻는다.
	SiKHackingProtectionManager()->GetHackingToolList( iRandomKey, vecHackingToolList );

	// 랜덤 체크 타임을 변경한다.
    SetRandomCheckTime();    
}

//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
bool KUserHackingManager::AddLetterMonitoringList( IN const UidType& iForomUnit )
{
	m_setRecvLetterList.insert( iForomUnit );

	if( m_setRecvLetterList.size() >= (u_int)SiKGameSysVal()->GetLimitsCountOfLetter() )
	{
		return true;
	}
	return false;
}

bool KUserHackingManager::AddTradeMonitoringList( IN const UidType& iForomUnit )
{
	m_setRecvTradeList.insert( iForomUnit );

	if( m_setRecvTradeList.size() >= (u_int)SiKGameSysVal()->GetLimitsCountOfTrade() )
	{
		return true;
	}
	return false;
}
#endif SERV_MONITORING_LETTER_AND_TRADE
//}}

//{{ 2011. 04. 26	최육사	deserialize 실패 종료
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
bool KUserHackingManager::HackingCheckDeserializeFailCount()
{
	// 이미 체크되었다면!
	if( m_bCheckedDeserializeFail )	
        return false;

	// Deserialize 체크해서 이상없으면 데이터 초기화!
	const bool bRet = ( m_iTotalDeserializeFailCount > SiKGameSysVal()->GetDeserializeFailCheckCount() );
	if( bRet )
	{
		START_LOG( cout, L"------------------------- Deserialize Fail Statistics -------------------------" );

		std::map< u_short, int >::const_iterator mit;
		for( mit = m_mapDeserializeFailList.begin(); mit != m_mapDeserializeFailList.end(); ++mit )
		{
			START_LOG( cout, KEvent::GetIDStr( mit->first ) )
				<< BUILD_LOG( mit->second );
		}

		m_bCheckedDeserializeFail = true;
	}
	
	m_iTotalDeserializeFailCount = 0;
	m_mapDeserializeFailList.clear();
	return bRet;
}

void KUserHackingManager::IncreaseDeserializeFailCount( IN const u_short usEventID )
{
	// 특정 패킷 타입에 대해서만 적용!
	switch( usEventID )
	{
	case EGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT:
		break;

	default:
		return;
	}

	// 전체 카운트 1증가!
	++m_iTotalDeserializeFailCount;

	// 통계 내기
	std::map< u_short, int >::iterator mit;
	mit = m_mapDeserializeFailList.find( usEventID );
	if( mit == m_mapDeserializeFailList.end() )
	{
		m_mapDeserializeFailList.insert( std::make_pair( usEventID, 1 ) );
	}
	else
	{
		++mit->second;
	}
}
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
//}}

//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
bool KUserHackingManager::CheckHackingUserRecivedPacket( OUT char& cLastSendType )
{
	if( m_cLastSendType != KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE )
	{
		if( m_tHackingUserTimer.elapsed() < 10 )
		{
			return true;
		}

		// 10초 동안 응답이 없다
		cLastSendType = m_cLastSendType;
		m_cLastSendType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;
		m_tHackingUserTimer.restart();
		return false;
	}
	return true;
}

void KUserHackingManager::SetHackingUserSendType( char cType )
{
	m_cLastSendType = cType;
	m_tHackingUserTimer.restart();
}

bool KUserHackingManager::SetHackingUserReciveType( IN char cType, OUT char& cLastSendType )
{
	cLastSendType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;

	// 문제 없으므로 초기화 하자
	if( m_cLastSendType == cType )
	{
		m_cLastSendType		= KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;
		m_tHackingUserTimer.restart();
		return true;
	}
	else
	{
		// 잘못된 응답이 왔다
		cLastSendType = m_cLastSendType;
		m_cLastSendType		= KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;
		m_tHackingUserTimer.restart();
		return false;
	}
}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//}}


#endif SERV_HACKING_TOOL_LIST
//}}
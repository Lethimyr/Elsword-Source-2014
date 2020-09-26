#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"
#include "GSSimLayer.h"

#include "ExpTable.h"
#include "SpiritTable.h"
#include "StatTable.h"
#include "BaseServer.h"
#include "X2Data/XSLTitleManager.h"
#include "ChannelManager.h"
#include "RoomListManager.h"

//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#ifdef SERV_EVENT_CHECK_POWER
	#include "GameSysval/GameSysVal.h"
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-09	// 박세훈
	#include "X2Data/XSLFieldBossData.h"
#endif // SERV_BATTLE_FIELD_BOSS

#include "X2Data/XSLSquareUnit.h"
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
#include "GameEvent/GameEventScriptManager.h"
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#include "odbc/Odbc.h"
#include "Enum/Enum.h"

#define CLASS_TYPE      KGSUser
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC_NOPARAM( EGS_SEND_STATISTICS_LOCLOG_REQ )
{
	if ( GetAuthLevel() < SEnum::UAL_DEVELOPER )
		return;

	// local statistics flush시 통계정보 삭제
	//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
	KSIManager.QueueingEventID( EGS_SEND_STATISTICS_LOCLOG_REQ );
#else
#pragma ELSE_MESSAGE
	//KSIManager.OnFlushLocalLog( true );
#endif SERV_STATISTICS_THREAD
	//}}

	SendToCnUser( ECN_SEND_STATISTICS_LOCLOG_REQ );

	// 치트 로그
	WriteCheatLogToDB( L"/stloc" );
}

IMPL_ON_FUNC_NOPARAM( EGS_SEND_STATISTICS_DBLOG_REQ )
{
	if ( GetAuthLevel() < SEnum::UAL_DEVELOPER )
		return;

	//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD	
	KSIManager.QueueingEventID( EGS_SEND_STATISTICS_DBLOG_REQ );
#else
#pragma ELSE_MESSAGE
	//KSIManager.FlushData();
#endif SERV_STATISTICS_THREAD
	//}}

	SendToCnUser( ECN_SEND_STATISTICS_DBLOG_REQ );

	// 치트 로그
	WriteCheatLogToDB( L"/stdb" );
}

_IMPL_ON_FUNC( EGS_ADMIN_QUEST_COMPLETE_REQ, KEGS_QUEST_COMPLETE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() == SEnum::UAL_DEVELOPER )
	{
		//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		if( SiCXSLQuestManager()->IsAccountQuest( kPacket_.m_iQuestID ) == true )
		{
			m_kUserAccountQuestManager.Handler_EGS_QUEST_COMPLETE_REQ_Account( kPacket_, GetThisPtr<KGSUser>(), true );
		}
		else
		{
			m_kUserQuestManager.Handler_EGS_QUEST_COMPLETE_REQ( kPacket_, GetThisPtr<KGSUser>(), true );
		}
#else
		m_kUserQuestManager.Handler_EGS_QUEST_COMPLETE_REQ( kPacket_, GetThisPtr<KGSUser>(), true );
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		

		START_LOG( cout, L"운영자도 자동 퀘스트 클리어" )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iQuestID );

		// 치트 로그		
		CStringW cwstrCheatContents;
		cwstrCheatContents.Format( L"%d", kPacket_.m_iQuestID );
		WriteCheatLogToDB( L"/q", cwstrCheatContents );
	}
	else
	{
		START_LOG( cerr, L"운영자도 아닌데 감히 +ㅁ+/" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}
}

IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_SERVER_STATUS_REQ )
{
	KEGS_ADMIN_SERVER_STATUS_ACK kPacket;
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_SERVER_STATUS_ACK, kPacket );

		return;
	}

	SendToLogDB( DBE_ADMIN_SERVER_STATUS_REQ );
}

_IMPL_ON_FUNC( DBE_ADMIN_SERVER_STATUS_ACK, KEGS_ADMIN_SERVER_STATUS_ACK )
{
	// 대전타입별 방갯수
	SiKRoomListManager()->GetNumPvPRoom( kPacket_.m_mapNumPvPRoom );

	// 파티 갯수 & 게임중인 파티수[던전방수]
	SiKChannelManager()->GetTotalPartyInfo( kPacket_.m_iCurPartyCount, kPacket_.m_iPlayGamePartyCount );

	SendPacket( EGS_ADMIN_SERVER_STATUS_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_NOTIFY_MSG_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
	if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_REQ::NT_GM_NOTIFY_ALL_SERVERS  ||
		kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_REQ::NT_GM_NOTIFY_SERVER_GROUP )
	{
		GetKGSSimLayer()->SendNotMSG( kPacket_.m_cNotifyType, kPacket_.m_wstrMSG, kPacket_.m_Count );
	}
	else
	{
		START_LOG( cerr, L"잘못된 GM공지 타입입니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOGc( kPacket_.m_cNotifyType )
			<< END_LOG;
		return;
	}
	//}}

	// 치트 로그	
	WriteCheatLogToDB( L"/n", kPacket_.m_wstrMSG );
}

IMPL_ON_FUNC( EGS_ADMIN_MODIFY_UNIT_LEVEL_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

#ifdef SERV_ADMIN_CHEAT_NICKNAME_CHECK

#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(kPacket_.m_wstrUnitNickName,GetCharName()) == false )
#else
	if( kPacket_.m_wstrUnitNickName.compare(GetCharName()) != 0 )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		START_LOG( cerr, L"치트로 다른 캐릭터의 레벨을 수정할 수 없다!" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_wstrUnitNickName )
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}
#endif SERV_ADMIN_CHEAT_NICKNAME_CHECK

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUnitNickName ) )
	{
		KEGS_ADMIN_MODIFY_UNIT_LEVEL_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_ADMIN_MODIFY_UNIT_LEVEL_ACK, kPacketAck );
		return;
	}

	// EXP값이 0이라면 EXP테이블에서 가져온다.
	if( kPacket_.m_iEXP == 0 )
	{
		kPacket_.m_iEXP = SiKExpTable()->GetRequireTotalExpbyLevel( kPacket_.m_iLevel );
	}
	// EXP값이 0이 아니라면 경험치 퍼센티지다! 계산해서 DB로 보내자!
	else
	{		
		int iTotalExp = SiKExpTable()->GetRequireTotalExpbyLevel( kPacket_.m_iLevel );
		int iNextLevelNeedExp = SiKExpTable()->GetRequireNeedExpbyLevel( kPacket_.m_iLevel + 1 );

		float fIncreaseExp = static_cast<float>(iNextLevelNeedExp) * ( static_cast<float>(kPacket_.m_iEXP) / 100.f );

		kPacket_.m_iEXP = iTotalExp + static_cast<int>(fIncreaseExp);
	}

	SendToGameDB( DBE_ADMIN_MODIFY_UNIT_LEVEL_REQ, kPacket_ );

	// 치트 로그	
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%s %d %d", kPacket_.m_wstrUnitNickName.c_str(), kPacket_.m_iLevel, kPacket_.m_iEXP );
	WriteCheatLogToDB( L"/l", cwstrCheatContents );
}

_IMPL_ON_FUNC( DBE_ADMIN_MODIFY_UNIT_LEVEL_ACK, KEGS_ADMIN_MODIFY_UNIT_LEVEL_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( GetCharUID() == kPacket_.m_kUnitInfo.m_nUnitUID )
		{
			KStatTable::KUnitStatInfo sUnitStat;
			SiKStatTable()->GetUnitStat( GetUnitClass() ,kPacket_.m_kUnitInfo.m_ucLevel, sUnitStat );
			m_ucLevel		= kPacket_.m_kUnitInfo.m_ucLevel;

			// 경험치
			if( kPacket_.m_kUnitInfo.m_iEXP == 0 )
			{
				m_kEXP.SetExp( SiKExpTable()->GetRequireTotalExpbyLevel( kPacket_.m_kUnitInfo.m_ucLevel ) );
			}
			else
			{
				m_kEXP.SetExp( kPacket_.m_kUnitInfo.m_iEXP );
			}			

			//m_iED.SetValue( 0 );
			//m_iVSPoint.SetValue( 0 );
			//m_iVSPointMax.SetValue( 0 );
			//m_iWin = 0; 
			//m_iLose = 0;
			//m_iSeceder = 0;

			m_kStat.m_iBaseHP		= sUnitStat.m_uiHP;
			m_kStat.m_iAtkPhysic	= sUnitStat.m_usAtkPhysic;
			m_kStat.m_iAtkMagic		= sUnitStat.m_usAtkMagic;
			m_kStat.m_iDefPhysic	= sUnitStat.m_usDefPhysic;
			m_kStat.m_iDefMagic		= sUnitStat.m_usDefMagic;
		}
	}

	GetUnitInfo( kPacket_.m_kUnitInfo );

	SendPacket( EGS_ADMIN_MODIFY_UNIT_LEVEL_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_AUTH_LEVEL_REQ )
{
	
	// config_gs_internal.lua 에서  AF_INTERNAL 로 되어 있지 않음
	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_INTERNAL )	// 해외팀 변경
	//if( KSimLayer::GetKObj()->GetBillingFlag() == KSimLayer::BF_NEXON_KOREA_TEST )
	{
		if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
		{
			START_LOG( cerr, L"운영자가 아닌데 감히.." )
				<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			KEGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_12;
			SendPacket( EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK, kPacket );
			return;
		}

		//{{ 2010. 12. 22	최육사	SQL인젝션방지
		if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUserID ) )
		{
			START_LOG( cout, L"개발자님! ID좀 잘 좀 입력하세요!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kPacket_.m_wstrUserID );

			KEGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_ODBC_00;
			SendPacket( EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK, kPacket );
			return;
		}
		//}}

		SendToAccountDB( DBE_ADMIN_CHANGE_AUTH_LEVEL_REQ, kPacket_ );

		// 치트 로그
		CStringW cwstrCheatContents;
		cwstrCheatContents.Format( L"%s %d", kPacket_.m_wstrUserID.c_str(), (int)kPacket_.m_cAuthLevel );
		WriteCheatLogToDB( L"/a", cwstrCheatContents );
	}
	else
	{
		START_LOG( cerr, L"본섭에서 사용 불가!" )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( DBE_ADMIN_CHANGE_AUTH_LEVEL_ACK, KEGS_ADMIN_CHANGE_AUTH_LEVEL_ACK )
{
	SendPacket( EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK, kPacket );

		return;
	}	

	SendToAccountDB( DBE_ADMIN_GET_AUTH_LEVEL_LIST_REQ, kPacket_ );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", (int)kPacket_.m_cAuthLevel );
	WriteCheatLogToDB( L"/al", cwstrCheatContents );
}

_IMPL_ON_FUNC( DBE_ADMIN_GET_AUTH_LEVEL_LIST_ACK, KEGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK )
{
	SendPacket( EGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_ED_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자도 아닌데 감히" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

#ifdef SERV_ADMIN_CHEAT_NICKNAME_CHECK

#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(kPacket_.m_wstrUnitNickName,GetCharName()) == false )
#else
	if( kPacket_.m_wstrUnitNickName.compare(GetCharName()) != 0 )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		START_LOG( cerr, L"치트로 다른 캐릭터의 ED를 수정할 수 없다!" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_wstrUnitNickName )
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}
#endif SERV_ADMIN_CHEAT_NICKNAME_CHECK

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUnitNickName ) )
	{
		KEGS_ADMIN_CHANGE_ED_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_ADMIN_CHANGE_ED_ACK, kPacketAck );
		return;
	}

	SendToGameDB( DBE_ADMIN_CHANGE_ED_REQ, kPacket_ );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%s %d", kPacket_.m_wstrUnitNickName.c_str(), kPacket_.m_nED );
	WriteCheatLogToDB( L"/e", cwstrCheatContents );
}

_IMPL_ON_FUNC( DBE_ADMIN_CHANGE_ED_ACK, KEGS_ADMIN_CHANGE_ED_ACK )
{
	SendPacket( EGS_ADMIN_CHANGE_ED_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_KICK_USER_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자도 아닌데 감히" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

	SendToLoginServer( ELG_ADMIN_KICK_USER_REQ, kPacket_ );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%s", kPacket_.m_wstrUnitNickName.c_str() );
	if( kPacket_.m_bIsUserID )
	{
		WriteCheatLogToDB( L"/kid", cwstrCheatContents );
	}
	else
	{
		WriteCheatLogToDB( L"/k", cwstrCheatContents );
	}	
}

_IMPL_ON_FUNC( ELG_ADMIN_KICK_USER_ACK, KEGS_ADMIN_KICK_USER_ACK )
{
	SendPacket( EGS_ADMIN_KICK_USER_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_SKILL_POINT_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..?ㅁ?" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		SET_ERROR( ERR_SKILL_PAGE_01 );
		START_LOG( cerr, L"활성된 스킬 페이지 번호가 이상합니다 (EGS_ADMIN_CHANGE_SKILL_POINT_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	if( kPacket_.m_iSPoint >= 0 )
	{
#ifdef SERV_SKILL_PAGE_SYSTEM
		// kimhc // 김현철 // 2013-11-22 현재 사용중인 스킬 페이지에만 적용
		std::vector<KUserSkillTree::SkillPageData>& vecSkillPageData
			= m_kSkillTree.AccessSkillPageDataVector();

		for ( UINT i = 0; i < vecSkillPageData.size(); i++ )
		{
			if ( i == kPacket_.m_iActiveSkillPageNumber )
			{
				int iSPointChange = kPacket_.m_iSPoint - vecSkillPageData[i].m_iSPoint.GetInitValue();
				vecSkillPageData[i].m_iSPoint.SetValue( vecSkillPageData[i].m_iSPoint.GetInitValue() );
				vecSkillPageData[i].m_iSPoint += iSPointChange;
				break;
			}
			
		}
#else // SERV_SKILL_PAGE_SYSTEM
		int iSPointChange = kPacket_.m_iSPoint - m_iSPoint.GetInitValue();
		m_iSPoint.SetValue( m_iSPoint.GetInitValue() );
		m_iSPoint += iSPointChange;
#endif // SERV_SKILL_PAGE_SYSTEM


		KEGS_ADMIN_CHANGE_SKILL_POINT_ACK kPacket;
		kPacket.m_iOK		= NetError::NET_OK;
		
#ifdef SERV_SKILL_PAGE_SYSTEM
		kPacket.m_iSPoint					= m_kSkillTree.GetSPoint();
		kPacket.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
#else // SERV_SKILL_PAGE_SYSTEM
		kPacket.m_iSPoint	= m_iSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM
		
		SendPacket( EGS_ADMIN_CHANGE_SKILL_POINT_ACK, kPacket );

		// 치트 로그
		CStringW cwstrCheatContents;
		cwstrCheatContents.Format( L"%d", kPacket_.m_iSPoint );
		WriteCheatLogToDB( L"/sp", cwstrCheatContents );
	}
}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27

#ifdef SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC( EGS_ADMIN_INIT_SKILL_TREE_REQ )
#else // SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_INIT_SKILL_TREE_REQ )
#endif // SERV_SKILL_PAGE_SYSTEM
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..?ㅁ?" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		START_LOG( cerr, L"활성된 스킬 페이지 번호가 이상합니다 (EGS_ADMIN_INIT_SKILL_TREE_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_PAGE_01;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
	int iDefaultSkillID1 = 0;
	int iDefaultSkillID2 = 0;
	int iDefaultSkillID3 = 0;
	int iDefaultSkillID4 = 0;
	int iDefaultSkillID5 = 0;
	int iDefaultSkillID6 = 0;

	if( !SiCXSLSkillTree()->GetUnitClassDefaultSkill( GetUnitClass(), iDefaultSkillID1, iDefaultSkillID2, iDefaultSkillID3, iDefaultSkillID4, iDefaultSkillID5, iDefaultSkillID6 ) )
	{
		START_LOG( cerr, L"유닛타입이 이상함." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetUnitType() )
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}

	int iRetrievedSPoint = 0;
	int iRetrievedCSPoint = 0;
	if( SiCXSLSkillTree()->GetCalcInitSkillPoint( GetLevel(), iRetrievedSPoint ) == false )
	{
		START_LOG( cerr, L"레벨로 환산된 sp 정보가 이상합니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( static_cast<int>(GetLevel()) )
			<< BUILD_LOG( iRetrievedSPoint )
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}

	if( false == m_kSkillTree.IsCashSkillPointExpired() )
	{
		iRetrievedCSPoint = m_kSkillTree.GetMaxCSPoint();
	}
	else
	{
		if( 0 != m_kSkillTree.GetCSPoint() )
		{
			START_LOG( cerr, L"cash skill point기한이 다되었는데 스킬트리에 csp가 남아있다." )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iRetrievedSPoint )
				<< BUILD_LOG( iRetrievedCSPoint )
				<< BUILD_LOG( m_kSkillTree.GetCSPoint() )
				<< END_LOG;

			KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_SKILL_12;
			SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
			return;
		}
	}

	KDBE_INIT_SKILL_TREE_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_iSPoint = iRetrievedSPoint;
	kPacket.m_iCSPoint = iRetrievedCSPoint;
	kPacket.m_iDefaultSkillID1 = iDefaultSkillID1;
	kPacket.m_iDefaultSkillID2 = iDefaultSkillID2;
	kPacket.m_iDefaultSkillID3 = iDefaultSkillID3;
	kPacket.m_iDefaultSkillID4 = iDefaultSkillID4;
	kPacket.m_iDefaultSkillID5 = iDefaultSkillID5;
	kPacket.m_iDefaultSkillID6 = iDefaultSkillID6;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	SendToGameDB( DBE_ADMIN_INIT_SKILL_TREE_REQ, kPacket );

	// 치트 로그
	WriteCheatLogToDB( L"/isk" );
}

_IMPL_ON_FUNC( DBE_ADMIN_INIT_SKILL_TREE_ACK, KEGS_ADMIN_INIT_SKILL_TREE_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{		
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetSPoint( kPacket_.m_iSPoint );
		m_kSkillTree.SetCSPoint( kPacket_.m_iActiveSkillPageNumber - 1, kPacket_.m_iCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
		m_iSPoint.SetValue( kPacket_.m_iSPoint );
		m_kSkillTree.SetCSPoint( kPacket_.m_iCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.ResetThisSkillPage( kPacket_.m_iActiveSkillPageNumber );
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.Reset( true, true, false, false, false );
#endif // SERV_SKILL_PAGE_SYSTEM

		//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
		// 기본 스킬 넣기
		int iDefaultSkillID[6] = {0,};
	
		if( !SiCXSLSkillTree()->GetUnitClassDefaultSkill( GetUnitClass(), iDefaultSkillID[0], iDefaultSkillID[1], iDefaultSkillID[2], iDefaultSkillID[3], iDefaultSkillID[4], iDefaultSkillID[5] ) )
		{
			START_LOG( cerr, L"유닛타입이 이상함." )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetUnitType() )
				<< END_LOG;
		}

		// 스킬 초기화
		for( int i = 0 ; i < 6 ; ++i )
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			// kimhc // 김현철 // 스킬 초기화 치트 사용 시, 사용중인 스킬 페이지만 초기화 하도록 함
			m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( iDefaultSkillID[i], 1, 0 );
#else // SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetSkillLevelAndCSP( iDefaultSkillID[i], 1, 0 );
#endif // SERV_SKILL_PAGE_SYSTEM
		}
	}
	else
	{
		START_LOG( cerr, L"스킬 트리 관련 치트 응답 실패")
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_UNKNOWN;
	}

	SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket_ );
}

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_INIT_SKILL_TREE_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..?ㅁ?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}
	
	//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
#ifdef SERV_FIX_THE_ADMIN_CHEAT

	int iDefaultSkillID = 0;
	int iDefaultSkillID2 = 0;
	if( !SiCXSLSkillTree()->GetUnitTypeDefaultSkill( GetUnitType(), iDefaultSkillID, iDefaultSkillID2 ) )
	{
		START_LOG( cerr, L"유닛타입이 이상함." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetUnitType() )
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}

	KDBE_INIT_SKILL_TREE_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_iSPoint = 1000;
	kPacket.m_iCSPoint = 0;
	kPacket.m_iDefaultSkillID = iDefaultSkillID;
	kPacket.m_iDefaultSkillID2 = iDefaultSkillID2;
	SendToGameDB( DBE_ADMIN_INIT_SKILL_TREE_REQ, kPacket );
#else
	SendToGameDB( DBE_ADMIN_INIT_SKILL_TREE_REQ, GetCharUID() );
#endif SERV_FIX_THE_ADMIN_CHEAT
	//}}

	// 치트 로그
	WriteCheatLogToDB( L"/isk" );
}

_IMPL_ON_FUNC( DBE_ADMIN_INIT_SKILL_TREE_ACK, KEGS_ADMIN_INIT_SKILL_TREE_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kSkillTree.Reset( true, true, false, false, false );

		//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
#ifdef SERV_FIX_THE_ADMIN_CHEAT
		// 기본 스킬 넣기
		int iDefaultSkillID = 0;
		int iDefaultSkillID2 = 0;
		if( !SiCXSLSkillTree()->GetUnitTypeDefaultSkill( GetUnitType(), iDefaultSkillID, iDefaultSkillID2 ) )
		{
			START_LOG( cerr, L"유닛타입이 이상함." )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetUnitType() )
				<< END_LOG;
		}
		m_kSkillTree.SetSkillLevelAndCSP( iDefaultSkillID, 1, 0 );
		m_kSkillTree.SetSkillLevelAndCSP( iDefaultSkillID2, 1, 0 );
#endif SERV_FIX_THE_ADMIN_CHEAT
		//}}
	}
	else
	{
		START_LOG( cerr, L"스킬 트리 관련 치트 응답 실패")
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_UNKNOWN;
	}

	SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket_ );
}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

//{{ 2009. 7. 26  최육사	party fever cheat
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_INCREASE_PARTY_FEVER_POINT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"개발자도 아닌데 감히" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

	if( GetPartyUID() > 0 )
	{
		//{{ 2010. 02. 23  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
		KERM_INCREASE_PARTY_FEVER_NOT kNot;
		kNot.m_iPartyUID = GetPartyUID();

		if( SiKPartyListManager()->E_IncreasePartyFever( kNot ) == false )
#else
		if( SiKPartyManager()->E_IncreasePartyFever( GetPartyUID() ) == false )
#endif SERV_CHANNEL_PARTY
			//}}		
		{
			START_LOG( cerr, L"파티 사기 수치 증가 실패!" )
				<< BUILD_LOG( GetPartyUID() )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cwarn, L"파티uid가 이상합니다!" )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}

	// 치트 로그
	WriteCheatLogToDB( L"/pf" );
}
//}}

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_SPIRIT_REQ )
{
	KEGS_ADMIN_CHANGE_SPIRIT_ACK kPacket;

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자도 아닌데 감히" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;


		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_SPIRIT_ACK, kPacket );
		return;
	}

	// 근성도 값 검사 [범위를 넘어선다면 현재 값을 보내줌]
	if( kPacket_.m_iSpirit < 0  ||  kPacket_.m_iSpirit > SiKSpiritTable()->GetMaxSpirit() )
	{
		//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		kPacket.m_iChangedSpirit = m_kUserSpiritManager.GetSpirit();
#else
		kPacket.m_iChangedSpirit = m_iSpirit;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}		
	}
	else
	{
		//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_kUserSpiritManager.SetSpirit( kPacket_.m_iSpirit );
#else
		m_iSpirit				 = kPacket_.m_iSpirit;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}		
		kPacket.m_iChangedSpirit = kPacket_.m_iSpirit;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_ADMIN_CHANGE_SPIRIT_ACK, kPacket );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iSpirit );
	WriteCheatLogToDB( L"/cs", cwstrCheatContents );
}

IMPL_ON_FUNC( EGS_ADMIN_SET_PC_BANG_REQ )
{
	KEGS_ADMIN_SET_PC_BANG_ACK kPacket;

	// 방 밖에서 게임방 상태를 바꿔야 방에 들어왔을 때 제대로 보인다.
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_09;
		SendPacket( EGS_ADMIN_SET_PC_BANG_ACK, kPacket );
		return;
	}

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자 이상만 가능." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_SET_PC_BANG_ACK, kPacket );
		return;
	}

	//{{ 2010. 01. 29  최육사	PC방 상용화
	m_kUserPcBangManager.SetIsPcBang( kPacket_.m_bOn );
#ifdef SERV_PC_BANG_TYPE
	m_kUserPcBangManager.SetPcBangType( kPacket_.m_iPcBangType );
#endif //SERV_PC_BANG_TYPE
	//}}

	kPacket.m_iOK = NetError::NET_OK;
	//{{ 2010. 01. 29  최육사	PC방 상용화
	kPacket.m_bOn = IsPcBang();
#ifdef SERV_PC_BANG_TYPE
	kPacket.m_iPcBangType = GetPcBangType();
	kPacket.m_kPcBangReward = GetPcBangReward();
#endif //SERV_PC_BANG_TYPE
	//}}
	SendPacket( EGS_ADMIN_SET_PC_BANG_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_VSPOINT_REQ )
{
	KEGS_ADMIN_CHANGE_VSPOINT_ACK kPacket;

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자도 아닌데 감히" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;


		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_VSPOINT_ACK, kPacket );
		return;
	}

	// VSPoint 치트 적용
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 06. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	/* Rating업데이트
		- AddRating의 결과 랭크 업/다운이 발생할 경우,
		강제로 RankPoint값을 지정해준다. ( AddRatingAndRankUpDownProcess 내부에서. )
		그러므로 이후 AddRPoint는 적용되어서는 안된다.
	*/
	if( m_kUserPvpManager.AddRatingAndRankUpDownProcess( kPacket_.m_iRating ) == false )
	{
		m_kUserPvpManager.AddRPoint( kPacket_.m_iRPoint );
	}
#else
	m_kUserPvpManager.AddRating( kPacket_.m_iRating );
	m_kUserPvpManager.AddRPoint( kPacket_.m_iRPoint );
#endif SERV_2012_PVP_SEASON2
	//}}
	m_kUserPvpManager.AddAPoint( kPacket_.m_iAPoint );
#else
	m_iVSPoint		+= kPacket_.m_iVSPoint;
	m_iVSPointMax	+= kPacket_.m_iVSPointMax;
#endif SERV_PVP_NEW_SYSTEM
	//}}	

	kPacket.m_iOK = NetError::NET_OK;
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	kPacket.m_cRank				 = GetPvpRankForClient();
#else
	kPacket.m_iChangedRating	 = GetRating();
#endif SERV_2012_PVP_SEASON2
	//}}
	kPacket.m_iChangedRPoint	 = GetRPoint();
	kPacket.m_iChangedAPoint	 = GetAPoint();
	SendPacket( EGS_ADMIN_CHANGE_VSPOINT_ACK, kPacket );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d %d", kPacket_.m_iRating, kPacket_.m_iRPoint, kPacket_.m_iAPoint );
	WriteCheatLogToDB( L"/vp", cwstrCheatContents );
#else
	kPacket.m_iChangedVSPoint	 = m_iVSPoint;
	kPacket.m_iChangedVSPointMax = m_iVSPointMax;
	SendPacket( EGS_ADMIN_CHANGE_VSPOINT_ACK, kPacket );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d", kPacket_.m_iVSPoint, kPacket_.m_iVSPointMax );
	WriteCheatLogToDB( L"/vp", cwstrCheatContents );
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_UNIT_CLASS_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}	

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", (int)kPacket_.m_cUnitClass );
	WriteCheatLogToDB( L"/uc", cwstrCheatContents );

#ifdef SERV_LIMIT_UNIT
	//국가별로 서버단에서 캐릭터 막는다. 
	const CXSLUnit::UNIT_TYPE eLimitType = static_cast< CXSLUnit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType );	
	const CXSLUnit::UNIT_TYPE eUnitType = CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) );
	if( eLimitType < eUnitType )
	{
		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_01; // 선택한 캐릭터 클래스가 이상함.
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}
#endif SERV_LIMIT_UNIT
 

#ifdef SERV_NO_ARA
	const CXSLUnit::UNIT_TYPE eUnitARAType = CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) );
	if( CXSLUnit::UT_ARA == eUnitARAType )
	{
		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_01; // 선택한 캐릭터 클래스가 이상함.
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}
#endif // SERV_NO_ARA

	// 유닛 클래스 검사
	if( CXSLUnit::IsValidUnitClass( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) ) == false )
	{
		START_LOG( cout, L"존재하지 않는 유닛 클래스로 치트를 사용하였습니다. 영자님 제대로 입력해주세요." )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( kPacket_.m_cUnitClass )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_01; // 선택한 캐릭터 클래스가 이상함.
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}

	KDBE_ADMIN_CHANGE_UNIT_CLASS_REQ kPacketToDB;

	int iDefaultSkill[6] = {0,};
	if( SiCXSLSkillTree()->GetUnitClassDefaultSkill( kPacket_.m_cUnitClass, iDefaultSkill[0], iDefaultSkill[1], iDefaultSkill[2], iDefaultSkill[3], iDefaultSkill[4], iDefaultSkill[5] ) == false )
	{
		START_LOG( cerr, L"전직시 지급하려는 기본 스킬이 이상함" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOGc( kPacket_.m_cUnitClass )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_01; // 선택한 캐릭터 클래스가 이상함.
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}

	if( CXSLUnit::IsInitNormalJob( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) ) == true )
	{
		kPacketToDB.m_iNewDefaultSkill1 = iDefaultSkill[0];
		kPacketToDB.m_iNewDefaultSkill2 = iDefaultSkill[1];
		kPacketToDB.m_iNewDefaultSkill3 = 0;
		kPacketToDB.m_iNewDefaultSkill4 = 0;
		kPacketToDB.m_iNewDefaultSkill5 = 0;
		kPacketToDB.m_iNewDefaultSkill6 = 0;
	}
	else if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) ) == true )
	{
		kPacketToDB.m_iNewDefaultSkill1 = iDefaultSkill[0];
		kPacketToDB.m_iNewDefaultSkill2 = iDefaultSkill[1];
		kPacketToDB.m_iNewDefaultSkill3 = iDefaultSkill[2];
		kPacketToDB.m_iNewDefaultSkill4 = iDefaultSkill[3];
		kPacketToDB.m_iNewDefaultSkill5 = 0;
		kPacketToDB.m_iNewDefaultSkill6 = 0;
	}
	else if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) ) == true )
	{
		kPacketToDB.m_iNewDefaultSkill1 = iDefaultSkill[0];
		kPacketToDB.m_iNewDefaultSkill2 = iDefaultSkill[1];
		kPacketToDB.m_iNewDefaultSkill3 = iDefaultSkill[2];
		kPacketToDB.m_iNewDefaultSkill4 = iDefaultSkill[3];
		kPacketToDB.m_iNewDefaultSkill5 = iDefaultSkill[4];
		kPacketToDB.m_iNewDefaultSkill6 = iDefaultSkill[5];
	}

	kPacketToDB.m_iUnitUID	 = GetCharUID();
	kPacketToDB.m_cUnitClass = kPacket_.m_cUnitClass;
	SendToGameDB( DBE_ADMIN_CHANGE_UNIT_CLASS_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_ADMIN_CHANGE_UNIT_CLASS_ACK )
{
	KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 클래스 저장
		SetUnitClass( kPacket_.m_cUnitClass );
		m_kSkillTree.SetUnitClass( kPacket_.m_cUnitClass );

		// 스킬 초기화
		int iRetrievedSPoint = 0;
		if( SiCXSLSkillTree()->GetCalcInitSkillPoint( GetLevel(), iRetrievedSPoint ) == true )
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetSPointEveryPage( iRetrievedSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
			m_iSPoint.SetValue( iRetrievedSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM
		}
		
		if( m_kSkillTree.IsCashSkillPointExpired() == true )
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetCSPointEveryPage( m_kSkillTree.GetMaxCSPoint() );
#else // SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetCSPoint( m_kSkillTree.GetMaxCSPoint() );
#endif // SERV_SKILL_PAGE_SYSTEM
		}
		else
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetCSPointEveryPage( 0 );
#else // SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetCSPoint( 0 );
#endif // SERV_SKILL_PAGE_SYSTEM
		}

#ifdef SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.ResetEveryPage();
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.Reset( true, true, false, false, false );
#endif // SERV_SKILL_PAGE_SYSTEM

		//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
		// 기본 스킬 넣기
		int iDefaultSkillID[6] = {0,};

		iDefaultSkillID[0] = kPacket_.m_iNewDefaultSkill1;
		iDefaultSkillID[1] = kPacket_.m_iNewDefaultSkill2;
		iDefaultSkillID[2] = kPacket_.m_iNewDefaultSkill3;
		iDefaultSkillID[3] = kPacket_.m_iNewDefaultSkill4;
		iDefaultSkillID[4] = kPacket_.m_iNewDefaultSkill5;
		iDefaultSkillID[5] = kPacket_.m_iNewDefaultSkill6;

		// 스킬 초기화
		for( int i = 0 ; i < 6 ; ++i )
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			// kimhc // 김현철 // 전직 변경 치트 사용 시, 모든 스킬 페이지를 초기화 하도록 함
			m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( iDefaultSkillID[i], 1, 0 );
#else // SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetSkillLevelAndCSP( iDefaultSkillID[i], 1, 0 );
#endif // SERV_SKILL_PAGE_SYSTEM
		}

		kPacket.m_cUnitClass = kPacket_.m_cUnitClass;

		kPacket.m_iDefaultSkill1 = kPacket_.m_iNewDefaultSkill1;
		kPacket.m_iDefaultSkill2 = kPacket_.m_iNewDefaultSkill2;
		kPacket.m_iDefaultSkill3 = kPacket_.m_iNewDefaultSkill3;
		kPacket.m_iDefaultSkill4 = kPacket_.m_iNewDefaultSkill4;
		kPacket.m_iDefaultSkill5 = kPacket_.m_iNewDefaultSkill5;
		kPacket.m_iDefaultSkill6 = kPacket_.m_iNewDefaultSkill6;

		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}

	// m_iOK != NetError::NET_OK
	kPacket.m_cUnitClass = 0;
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iSPoint = m_kSkillTree.GetSPoint();
#else // SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iSPoint = m_iSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM
	
	kPacket.m_iCSPoint = m_kSkillTree.GetCSPoint();
	kPacket.m_iDefaultSkill1 = 0;
	kPacket.m_iDefaultSkill2 = 0;
	kPacket.m_iDefaultSkill3 = 0;
	kPacket.m_iDefaultSkill4 = 0;
	kPacket.m_iDefaultSkill5 = 0;
	kPacket.m_iDefaultSkill6 = 0;
	SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
	return;
}

#ifdef SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC( EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ )
#else // SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ )
#endif // SERV_SKILL_PAGE_SYSTEM
{
	// 레벨에 따라 배울수있는 스킬 다 찍기
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
		return;
	}	

	// 치트 로그
	WriteCheatLogToDB( L"/as" );

	KDBE_ADMIN_AUTO_GET_ALL_SKILL_REQ kPacketToDB;

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		START_LOG( cerr, L"활성된 스킬 페이지 번호가 이상합니다 (EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_PAGE_01;;
		SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	if( !SiCXSLSkillTree()->GetUnitClassDefaultSkill( GetUnitClass(), 
		kPacketToDB.m_iNewDefaultSkill1, 
		kPacketToDB.m_iNewDefaultSkill2, 
		kPacketToDB.m_iNewDefaultSkill3, 
		kPacketToDB.m_iNewDefaultSkill4, 
		kPacketToDB.m_iNewDefaultSkill5, 
		kPacketToDB.m_iNewDefaultSkill6 ) )
	{
		START_LOG( cerr, L"유닛타입이 이상함." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetUnitType() )
			<< END_LOG;

		KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
		return;
	}

	int iRetrievedSPoint = 0;
	int iRetrievedCSPoint = 0;
	if( SiCXSLSkillTree()->GetCalcInitSkillPoint( GetLevel(), iRetrievedSPoint ) == false )
	{
		START_LOG( cerr, L"레벨로 환산된 sp 정보가 이상합니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( static_cast<int>(GetLevel()) )
			<< BUILD_LOG( iRetrievedSPoint )
			<< END_LOG;

		KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
		return;
	}

	if( false == m_kSkillTree.IsCashSkillPointExpired() )
	{
		iRetrievedCSPoint = m_kSkillTree.GetMaxCSPoint();
	}
	else
	{
		if( 0 != m_kSkillTree.GetCSPoint() )
		{
			START_LOG( cerr, L"cash skill point기한이 다되었는데 스킬트리에 csp가 남아있다." )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iRetrievedSPoint )
				<< BUILD_LOG( iRetrievedCSPoint )
				<< BUILD_LOG( m_kSkillTree.GetCSPoint() )
				<< END_LOG;

			KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_SKILL_12;
			SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
			return;
		}
	}

	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_cUnitClass = GetUnitClass();
	kPacketToDB.m_iSPoint = iRetrievedSPoint;
	kPacketToDB.m_iCSPoint = iRetrievedCSPoint;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacketToDB.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	std::map< int, CXSLSkillTree::SkillTreeTemplet > mapSkillTreeList;
	std::map< int, CXSLSkillTree::SkillTreeTemplet >::iterator mit;

	// 배울 수 있는 스킬 목록을 찾자
	mapSkillTreeList.clear();
	SiCXSLSkillTree()->GetUnitClassSkillList( GetUnitClass(), mapSkillTreeList );
	for( mit = mapSkillTreeList.begin() ; mit != mapSkillTreeList.end() ; ++mit )
	{
		// 결혼 스킬은 제외
		if( mit->first == CXSLSkillTree::SI_ETC_WS_COMMON_LOVE )
			continue;

		const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( mit->first );
		if( pSkillTemplet == NULL )
			continue;

		// 레벨을 비교하여 배울 수 있는 tier 를 찾자
		int iTier = mit->second.m_iTier;
		int iGap = 0;
		if( iTier >= 5 )		// 1차 기본 스킬은 제외 시킨다
			++iGap;
		
		if( iTier >= 10 )		// 2차 기본 스킬은 제외 시킨다
			++iGap;

		if( (mit->second.m_iTier - iGap) * 5 > GetLevel() )
			continue;

		// 배울 수 있는 만큼의 스킬 레벨을 올리자
		int iSkillLevel = mit->second.m_iMasterSkillLevel;
		for( ; 0 < iSkillLevel ; --iSkillLevel  )
		{
			if( pSkillTemplet->m_vecRequireCharactorLevel[ iSkillLevel-1 ] <= GetLevel() )
				break;
		}

		if( iSkillLevel > 0 )
			kPacketToDB.m_mapGetSkillList.insert( std::make_pair( mit->first, iSkillLevel ) );

		if( pSkillTemplet->m_bBornSealed == true )
		{
			if( m_kSkillTree.IsSkillUnsealed( mit->first ) == false )
				kPacketToDB.m_vecUnsealedSkillID.push_back( static_cast<short>(mit->first) );
		}
		
	}
	// sp 소모는 없는 것으로 한다.
	
	
	SendToGameDB( DBE_ADMIN_AUTO_GET_ALL_SKILL_REQ, kPacketToDB );
	
}

IMPL_ON_FUNC( DBE_ADMIN_AUTO_GET_ALL_SKILL_ACK)
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
		return;
	}

	// 스킬 초기화	
#ifdef SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.SetSPoint( kPacket_.m_iSPoint );
	m_kSkillTree.SetCSPoint( kPacket_.m_iActiveSkillPageNumber - 1, kPacket_.m_iCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
	m_iSPoint.SetValue( kPacket_.m_iSPoint );
	m_kSkillTree.SetCSPoint( kPacket_.m_iCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.ResetEveryPage();
#else // SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.Reset( true, true, false, false, false );
#endif // SERV_SKILL_PAGE_SYSTEM

	// 기본 스킬 넣기
	int iDefaultSkillID[6] = {0,};

	iDefaultSkillID[0] = kPacket_.m_iNewDefaultSkill1;
	iDefaultSkillID[1] = kPacket_.m_iNewDefaultSkill2;
	iDefaultSkillID[2] = kPacket_.m_iNewDefaultSkill3;
	iDefaultSkillID[3] = kPacket_.m_iNewDefaultSkill4;
	iDefaultSkillID[4] = kPacket_.m_iNewDefaultSkill5;
	iDefaultSkillID[5] = kPacket_.m_iNewDefaultSkill6;

	// 스킬 초기화
	for( int i = 0 ; i < 6 ; ++i )
	{
		std::map<int, int>::iterator mit = kPacket_.m_mapGetSkillList.find( iDefaultSkillID[i] );
		if( mit == kPacket_.m_mapGetSkillList.end() )
		{
			kPacket_.m_mapGetSkillList.insert( std::make_pair( iDefaultSkillID[i], 1 ) );
		}		
	}

	std::map<int, int>::iterator mit = kPacket_.m_mapGetSkillList.begin();
	for( ; mit != kPacket_.m_mapGetSkillList.end() ; ++mit )
	{
#ifdef SERV_SKILL_PAGE_SYSTEM
		// kimhc // 김현철 // 자신의 레벨에 맞는 모든 스킬 배우기 치트 사용 시, 현재 활성화 중인 스킬 페이지에만 적용 되도록 함
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( mit->first, mit->second, 0 );
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetSkillLevelAndCSP( mit->first, mit->second, 0 );
#endif // SERV_SKILL_PAGE_SYSTEM
	}

	BOOST_TEST_FOREACH( short, iSkillID, kPacket_.m_vecUnsealedSkillID )
	{
		m_kSkillTree.SkillUnseal( (int)iSkillID );
	}

	KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
	kPacket.m_iOK						= kPacket_.m_iOK;
	kPacket.m_iSPoint					= kPacket_.m_iSPoint;
	kPacket.m_iCSPoint					= kPacket_.m_iCSPoint;
	kPacket.m_mapSkillList				= kPacket_.m_mapGetSkillList;
	kPacket.m_vecUnsealedSkillID		= kPacket_.m_vecUnsealedSkillID;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_ADMIN_GET_SKILL_REQ )
{
	// 레벨에 따라 배울수있는 스킬 다 찍기
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}	

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		START_LOG( cerr, L"활성된 스킬 페이지 번호가 이상합니다 (EGS_ADMIN_GET_SKILL_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_PAGE_01;;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iSkillID );
	cwstrCheatContents.Format( L"%d", kPacket_.m_iSkillLevel );
	WriteCheatLogToDB( L"/sl", cwstrCheatContents );

	const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetSkillTreeTemplet( GetUnitClass(), kPacket_.m_iSkillID );
	if( pSkillTreeTemplet == NULL )
	{
		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_01;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}

	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( kPacket_.m_iSkillID );
	if( pSkillTemplet == NULL )
	{
		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_01;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}
	
	if( kPacket_.m_iSkillLevel <= 0 )
	{
		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_20;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}

	int iSkillLevel = 0;
	int iSkillCSPoint = 0;

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( m_kSkillTree.IsActiveSkillPageNumberValid() )
	{
		// kimhc // 김현철 // 2013-11-17 // 이 부분은 기존 코드에서도 실패시 별다른 처리가 없기에
		// 그냥 다음 Step 으로 진행 하도록 하였음
		m_kSkillTree.GetSkillLevelAndCSP( iSkillLevel, iSkillCSPoint, kPacket_.m_iSkillID, 
			m_kSkillTree.AccessLearnedSkillTree() );
	}
	else
	{
		AddLogWhenSkillPagesNumberIsWrong( L"EGS_ADMIN_GET_SKILL_REQ" );		
		return;
	}
#else // SERV_SKILL_PAGE_SYSTEM
	if( m_kSkillTree.GetSkillLevelAndCSP( kPacket_.m_iSkillID, iSkillLevel, iSkillCSPoint ) == false )
	{
		iSkillCSPoint = 0;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	bool bUnsealed = false;
	if( pSkillTemplet->m_bBornSealed == true && m_kSkillTree.IsSkillUnsealed( kPacket_.m_iSkillID ) == false )
	{
		bUnsealed = true;
	}

	KDBE_ADMIN_GET_SKILL_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iSkillID = kPacket_.m_iSkillID;
	kPacketToDB.m_iSkillLevel = kPacket_.m_iSkillLevel;
	kPacketToDB.m_iCSPoint = iSkillCSPoint;
	kPacketToDB.m_bUnsealed = true;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacketToDB.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	SendToGameDB( DBE_ADMIN_GET_SKILL_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_ADMIN_GET_SKILL_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}

	if( kPacket_.m_bUnsealed == true )
		m_kSkillTree.SkillUnseal( kPacket_.m_iSkillID );

#ifdef SERV_SKILL_PAGE_SYSTEM
	// kimhc // 김현철 // 한개의 스킬 배우기 치트 사용 시, 현재의 스킬 페이지에 적용 되도록 함
	m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( kPacket_.m_iSkillID, kPacket_.m_iSkillLevel, kPacket_.m_iCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iSkillID, kPacket_.m_iSkillLevel, kPacket_.m_iCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM

	KEGS_ADMIN_GET_SKILL_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iSkillID = kPacket_.m_iSkillID;
	kPacket.m_iSkillLevel = kPacket_.m_iSkillLevel;
	kPacket.m_iCSPoint = kPacket_.m_iCSPoint;
	kPacket.m_bUnsealed = kPacket_.m_bUnsealed;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
}

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
IMPL_ON_FUNC( EGS_ADMIN_CHANGE_UNIT_CLASS_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}	

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", (int)kPacket_.m_cUnitClass );
	WriteCheatLogToDB( L"/uc", cwstrCheatContents );

	// 유닛 클래스 검사
	if( CXSLUnit::IsValidUnitClass( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) ) == false )
	{
		START_LOG( cout, L"존재하지 않는 유닛 클래스로 치트를 사용하였습니다. 영자님 제대로 입력해주세요." )
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOGc( kPacket_.m_cUnitClass )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_01; // 선택한 캐릭터 클래스가 이상함.
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}

	KDBE_CHANGE_UNIT_CLASS_REQ kPacketToDB;
	kPacketToDB.m_usEventID  = EGS_ADMIN_CHANGE_UNIT_CLASS_REQ;
	kPacketToDB.m_iUnitUID	 = GetCharUID();
	kPacketToDB.m_cUnitClass = kPacket_.m_cUnitClass;
	SendToGameDB( DBE_CHANGE_UNIT_CLASS_REQ, kPacketToDB );
}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

//{{ 2008. 10. 24  최육사	타이틀 치트
IMPL_ON_FUNC( EGS_ADMIN_GET_TITLE_CHEAT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_ADMIN_GET_TITLE_CHEAT_ACK kPacketAck;
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_GET_TITLE_CHEAT_ACK, kPacketAck );
		return;
	}

	if( !SiCXSLTitleManager()->IsExistTitleID( kPacket_.m_iTitleID ) )
	{
		kPacketAck.m_iOK = NetError::ERR_TITLE_03;
		SendPacket( EGS_ADMIN_GET_TITLE_CHEAT_ACK, kPacketAck );
		return;
	}

	if( m_kUserTitleManager.IsExistTitle( kPacket_.m_iTitleID ) )
	{
		kPacketAck.m_iOK = NetError::ERR_TITLE_05;
		SendPacket( EGS_ADMIN_GET_TITLE_CHEAT_ACK, kPacketAck );
		return;
	}

	if( kPacket_.m_iPeriod < 0  ||  kPacket_.m_iPeriod > 30 )
	{
		kPacketAck.m_iOK = NetError::ERR_TITLE_04;
		SendPacket( EGS_ADMIN_GET_TITLE_CHEAT_ACK, kPacketAck );
		return;
	}

	kPacketAck.m_iOK = NetError::NET_OK;
	SendPacket( EGS_ADMIN_GET_TITLE_CHEAT_ACK, kPacketAck );

	// DB에 타이틀을 넣자!
	KDBE_INSERT_TITLE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iTitleID = kPacket_.m_iTitleID;
	kPacketToDB.m_sPeriod  = static_cast<short>(kPacket_.m_iPeriod);
	SendToGameDB( DBE_INSERT_TITLE_REQ, kPacketToDB );

	// 중복 보상을 받지 않도록 미리 진행중인 미션 삭제 - TitleID와 MissionID는 동일함
	m_kUserTitleManager.DeleteMissionInstance( kPacketToDB.m_iTitleID );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iPeriod );
	WriteCheatLogToDB( L"/t", cwstrCheatContents );
}
//}}

//{{ 2009. 7. 29  최육사	item set cheat
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_GET_ITEM_SET_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"개발자도 아닌데 감히" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	KDBE_ADMIN_GET_ITEM_SET_NOT kPacket;
	kPacket.m_wstrNickName = GetCharName();
	SendToGameDB( DBE_ADMIN_GET_ITEM_SET_NOT, kPacket );

	// 치트 로그
	WriteCheatLogToDB( L"/is" );
}
//}}

//{{ 2009. 8. 5  최육사		체험ID치트
_IMPL_ON_FUNC( EGS_ADMIN_GUEST_USER_MODE_NOT, bool )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"개발자도 아닌데 감히" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

	// 체험ID 모드 변경
	m_kNexonAccountInfo.m_byteGuestUser = ( kPacket_ ? 1 : 0 );

	START_LOG( cout, L"개발자가 체험ID 모드를 변경하였습니다!" )		
		<< BUILD_LOG( GetName() )		
		<< BUILD_LOG( IsGuestUser() );

	// 치트 로그
	std::wstring wstrGuestFlag;
	if( IsGuestUser() )
	{
		wstrGuestFlag = L"true";
	}
	else
	{
		wstrGuestFlag = L"false";		
	}

	WriteCheatLogToDB( L"/guest", wstrGuestFlag );
}
//}}

#ifdef GUILD_SKILL_TEST

IMPL_ON_FUNC( EGS_ADMIN_GET_GUILD_SKILL_POINT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자도 아닌데 감히" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

	KEGS_ADMIN_GET_GUILD_SKILL_POINT_ACK kPacket;	

	// 치트라고 하더라도 데이터가 꼬일수 있으니 길드원검사 및 길드마스터 검사를 하자!
	// 길드 가입 유저인지 검사
	if( GetGuildUID() <= 0 )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_ADMIN_GET_GUILD_SKILL_POINT_ACK, kPacket );
		return;
	}

	// 해당 치트를 사용할 수 있음
	if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_SKILL_00;
		SendPacket( EGS_ADMIN_GET_GUILD_SKILL_POINT_ACK, kPacket );
		return;
	}

	// 길드 스킬 치트
	START_LOG( cout, L"운영자가 길드 스킬 포인트 치트를 사용하였습니다!" )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( GetGuildUID() )
#else
		<< BUILD_LOG( GetName() )
		<< BUILD_LOG( GetCharName() )
		<< BUILD_LOG( m_kUserGuildManager.GetGuildName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacket_.m_iGuildSPoint );

	KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iGuildSPoint = kPacket_.m_iGuildSPoint;
	SendToLoginServer( ELG_ADMIN_GET_GUILD_SKILL_POINT_REQ, kPacketToLg );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iGuildSPoint );
	WriteCheatLogToDB( L"/gsp", cwstrCheatContents );
}

IMPL_ON_FUNC( ELG_ADMIN_GET_GUILD_SKILL_POINT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_ADMIN_GET_GUILD_SKILL_POINT_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iCurGuildSPoint = kPacket_.m_iGuildSPoint;
	SendPacket( EGS_ADMIN_GET_GUILD_SKILL_POINT_ACK, kPacket );
}

#endif GUILD_SKILL_TEST

#ifdef SERV_PET_SYSTEM

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_PET_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d", kPacket_.m_iCheatType, kPacket_.m_iValue );
	WriteCheatLogToDB( L"/pet", cwstrCheatContents );

	// 치트 처리
	KPetInfo kPetInfo;
	int iPetInventorySlotSize = 0;
	if( m_kUserPetManager.Handler_EGS_ADMIN_CHANGE_PET_INFO_NOT( kPacket_, GetThisPtr<KGSUser>(), kPetInfo, iPetInventorySlotSize ) == false )
	{
		START_LOG( cout, L"펫 개발자 치트 실패!" );
		return;
	}

	switch( kPacket_.m_iCheatType )
	{
	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EVOLUTION_STEP:
		{
			// 펫 진화 성공 했다면 인벤토리 크기 변경하자!
			m_kInventory.ResetPetInventorySize( iPetInventorySlotSize );

			// 펫 진화 결과를 클라이언트에게 알려주자!
			KEGS_PET_EVOLUTION_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::NET_OK;
			kPacketAck.m_kEvolutionResultPetInfo = kPetInfo;
			SendPacket( EGS_PET_EVOLUTION_ACK, kPacketAck );

			START_LOG( cout, L"펫 진화 치트 성공!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kPacket_.m_iValue )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOGc( kPetInfo.m_cEvolutionStep );

			//////////////////////////////////////////////////////////////////////////
			// 필드이거나 방이면 알리자!
			if( GetStateID() == KGSFSM::S_FIELD_MAP )
			{
				if( GetFieldUID() <= 0 )
				{
					KEGS_PET_EVOLUTION_NOT kPacketNot;
					kPacketNot.m_iUnitUID = GetCharUID();
					kPacketNot.m_kPetInfo = kPetInfo;
					SendPacket( EGS_PET_EVOLUTION_NOT, kPacketNot );
				}
				else
				{
					KERM_PET_EVOLUTION_NOT kPacketNot;
					kPacketNot.m_iUnitUID = GetCharUID();
					kPacketNot.m_kEvolutionPetInfo = kPetInfo;
					SendToGSField( ERM_PET_EVOLUTION_NOT, kPacketNot );
				}
			}
			//}}
			// 장착 타이틀을 룸에서 바꿨으면 룸 멤버들에게 알려준다.
			else if( GetStateID() == KGSFSM::S_ROOM )
			{
				if( GetRoomUID() <= 0 )
				{
					START_LOG( cerr, L"룸 UID 이상." )
						<< BUILD_LOG( GetRoomUID() )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					return;
				}

				switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
				{
				case CXSLRoom::RT_PVP:
				case CXSLRoom::RT_DUNGEON:
				case CXSLRoom::RT_TRAININGCENTER:
					//{{ 2012. 11. 08	박세훈	배틀필드 : 필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
				case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
					//}}
					{
						KERM_PET_EVOLUTION_NOT kPacketNot;
						kPacketNot.m_iUnitUID = GetCharUID();
						kPacketNot.m_kEvolutionPetInfo = kPetInfo;
						SendToCnRoom( ERM_PET_EVOLUTION_NOT, kPacketNot );
					}
					break;

				case CXSLRoom::RT_SQUARE:
					// 광장이면 안보낸다!
					break;

				default:
					{
						START_LOG( cerr, L"이상한 방 타입." )
							<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
							<< BUILD_LOG( GetCharUID() )
							<< END_LOG;
					}
					break;
				}
			}
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_SATIETY:
		{
			KEGS_CHANGED_PET_SATIETY_NOT kNot;
			kNot.m_iUnitUID = GetCharUID();
			kNot.m_iPetUID = kPetInfo.m_iPetUID;
			kNot.m_sCurrentSatiety = kPetInfo.m_sSatiety;

			if( GetStateID() == KGSFSM::S_ROOM )
			{
				SendToCnRoom( ERM_CHANGED_PET_SATIETY_NOT, kNot );
			}
			else
			{
				SendPacket( EGS_CHANGED_PET_SATIETY_NOT, kNot );
			}

			// 자동 먹이 기능 체크!
			CheckAutoFeed();

			START_LOG( cout, L"펫 포만도 치트 성공!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kPacket_.m_iValue )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOGc( kPetInfo.m_sSatiety );
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_INTIMACY:
		{
			KEGS_CHANGED_PET_INTIMACY_NOT kNot;
			kNot.m_iUnitUID = GetCharUID();
			kNot.m_iPetUID = kPetInfo.m_iPetUID;
			kNot.m_iCurrentIntimacy = kPetInfo.m_iIntimacy;

			if( GetStateID() == KGSFSM::S_ROOM )
			{
				SendToCnRoom( ERM_CHANGED_PET_INTIMACY_NOT, kNot );
			}
			else
			{
				SendPacket( EGS_CHANGED_PET_INTIMACY_NOT, kNot );
			}

			START_LOG( cout, L"펫 친밀도 치트 성공!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT				
				<< BUILD_LOG( kPacket_.m_iValue )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOGc( kPetInfo.m_sSatiety );
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EXTROVERSION:
		{
			START_LOG( cout, L"펫 외향성 치트 성공!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( kPacket_.m_iValue )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOGc( kPetInfo.m_sExtroversion );
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EMOTION:
		{
			START_LOG( cout, L"펫 감수성 치트 성공!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT	
				<< BUILD_LOG( kPacket_.m_iValue )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOGc( kPetInfo.m_sEmotion );
		}
		break;
	}
}

#endif SERV_PET_SYSTEM

//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT

IMPL_ON_FUNC( EGS_ADMIN_SHOW_ED_CHEAT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%s", kPacket_.m_wstrNickName.c_str() );
	WriteCheatLogToDB( L"/usered", cwstrCheatContents );

	// 로그인 서버로 보내자!
	SendToLoginServer( ELG_ADMIN_SHOW_ED_CHEAT_REQ, kPacket_ );
}

IMPL_ON_FUNC( ELG_ADMIN_SHOW_ED_CHEAT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KELG_ADMIN_SHOW_ED_CHEAT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iSenderUserUID = kPacket_.m_iSenderUserUID;
	kPacket.m_wstrNickName = GetCharName();
	kPacket.m_iED = GetED();
	SendToLoginServer( ELG_ADMIN_SHOW_ED_CHEAT_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_ADMIN_SHOW_ED_CHEAT_ACK, KEGS_ADMIN_SHOW_ED_CHEAT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_ADMIN_SHOW_ED_CHEAT_ACK, kPacket_ );
}

#endif SERV_RESET_PERIOD_EVENT
//}}

//{{ 2011. 04. 16	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
IMPL_ON_FUNC( EGS_ADMIN_PSHOP_AGENCY_CHEAT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_sPeriod );
	WriteCheatLogToDB( L"/shop", cwstrCheatContents );

	KDBE_INSERT_PERIOD_PSHOP_AGENCY_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_sAgencyPeriod = kPacket_.m_sPeriod;
	kPacketToDB.m_usEventID = EGS_ADMIN_PSHOP_AGENCY_CHEAT_REQ;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
    kPacketToDB.m_cShopType = SEnum::AST_PREMIUM; // CXSLSquareUnit::PST_PREMIUM;
#else //SERV_UPGRADE_TRADE_SYSTEM
	kPacketToDB.m_cShopType = CXSLSquareUnit::PST_PREMIUM; // CXSLSquareUnit::PST_PREMIUM;
#endif //SERV_UPGRADE_TRADE_SYSTEM
	SendToGameDB( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ, kPacketToDB );
}
#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 08. 11	최육사	투니 랜드 채널링 
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
IMPL_ON_FUNC( EGS_ADMIN_CHANNELING_CODE_CHEAT_NOT )
{
	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"개발자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	// 치트 적용
	m_kNexonAccountInfo.m_uChannelCode = static_cast<UINT8>(kPacket_.m_iChannelingCode);

	START_LOG( cout, L"개발자가 채널링 치트 코드를 사용하였습니다!" )
		<< BUILD_LOG( GetName() )
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( GetChannelCode() );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iChannelingCode );
	WriteCheatLogToDB( L"/channel", cwstrCheatContents );
}
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
//}}

//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
IMPL_ON_FUNC( EGS_ADMIN_HENIR_REWARD_CHEAT_NOT )
{
	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"개발자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	// 치트 적용
	m_kUserDungeonManager.SetPossibleHenirRewardNormalCount( kPacket_.m_iHenirRewardNormalCnt );

	START_LOG( cout, L"개발자가 채널링 치트 코드를 사용하였습니다!" )
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iHenirRewardNormalCnt );
	WriteCheatLogToDB( L"/henir", cwstrCheatContents );
}
#endif SERV_NEW_HENIR_TEST
//}}

//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
IMPL_ON_FUNC( EGS_ADMIN_ED_MONITORING_CHEAT_NOT )
{
	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"개발자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	// 치트 적용
	m_kUserHackingManager.SetEDMonitoringMode( kPacket_.m_bEDMonitoringMode );

	START_LOG( cout, L"개발자가 ED 감시 치트 코드를 사용하였습니다!" )
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_bEDMonitoringMode );
	WriteCheatLogToDB( L"/EDMonitoring", cwstrCheatContents );

}
#endif SERV_ED_MONITORING_IN_GAME
//}}

#ifdef SERV_CREATE_ELITE_FOR_ADMIN
IMPL_ON_FUNC( EGS_ADMIN_NPC_UNIT_CREATE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"GM이 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_ADMIN_NPC_UNIT_CREATE_REQ, kPacket_ );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_kNPCUnitReq.m_NPCID );
	WriteCheatLogToDB( L"/cm", cwstrCheatContents );
}

_IMPL_ON_FUNC( ERM_ADMIN_NPC_UNIT_CREATE_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_ADMIN_NPC_UNIT_CREATE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_ADMIN_NPC_UNIT_CREATE_NOT, KEGS_ADMIN_NPC_UNIT_CREATE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_ADMIN_NPC_UNIT_CREATE_NOT, kPacket_ );
}
#endif SERV_CREATE_ELITE_FOR_ADMIN

//{{ 2012. 05. 30	최육사	그노시스 기간 치트
#ifdef SERV_CASH_SKILL_POINT_DATE_CHANGE
IMPL_ON_FUNC( EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"개발자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	// 문자열 조합
	std::wstring wstrModifyDate = boost::str( boost::wformat( L"%04d-%02d-%02d %02d:%02d:00" ) % kPacket_.m_iYear % kPacket_.m_iMonth % kPacket_.m_iDay % kPacket_.m_iHour % kPacket_.m_iMinute );

	// 치트 적용
	START_LOG( cout, L"개발자가 캐쉬 스킬 포인트 기간을 변경하는 치트 코드를 사용하였습니다!" )
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( GetCharName() )
		<< BUILD_LOG( wstrModifyDate );

	KDBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_wstrModifyDate = wstrModifyDate;
	SendToGameDB( DBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ, kPacketToDB );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d %d %d %d", kPacket_.m_iYear, kPacket_.m_iMonth, kPacket_.m_iDay, kPacket_.m_iHour, kPacket_.m_iMinute );
	WriteCheatLogToDB( L"/cspdate", cwstrCheatContents );
}

_IMPL_ON_FUNC( DBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK, KEGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		START_LOG( cout, L"캐쉬 스킬 포인트 기간 변경 치트 성공!" )
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_wstrCspEndDate );

		LIF( m_kSkillTree.SetCSPointEndDate( kPacket_.m_wstrCspEndDate ) );
	}
	else
	{
		START_LOG( cerr, L"캐쉬 스킬 포인트 기간 변경 치트 실패!" )
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_wstrCspEndDate )
			<< END_LOG;
	}

	SendPacket( EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK, kPacket_ );
}
#endif SERV_CASH_SKILL_POINT_DATE_CHANGE
//}}

//{{ 2012. 07. 24	박세훈	해당 캐릭터의 모든 스킬을 다 찍는 치트
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL

#ifdef SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC( EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ )
#else // SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ )
#endif // SERV_SKILL_PAGE_SYSTEM
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..?ㅁ?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK, kPacket );
		return;
	}

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		START_LOG( cerr, L"활성된 스킬 페이지 번호가 이상합니다 (EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		KEGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_PAGE_01;;
		SendPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK, kPacket );
		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	// 1. 해당 클래스의 모든 스킬 정보 수집
	const CXSLSkillTree::SkillTreeTempletMap* pSkillTreeTempletMap = SiCXSLSkillTree()->GetSkillTreeTemplet( GetUnitClass() );
	if( pSkillTreeTempletMap == NULL )
	{
		START_LOG( cerr, L"해당 클래스의 스킬 정보를 얻어올 수 없습니다.")
			<< BUILD_LOG( GetUnitClass() )
			<< END_LOG;
		
		KEGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK, kPacket );
		return;
	}

	KDBE_ADMIN_CHEAT_GET_ALL_SKILL_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();

	CXSLSkillTree::SkillTreeTempletMap::const_iterator it;
	for( it=pSkillTreeTempletMap->begin(); it != pSkillTreeTempletMap->end(); ++it )
	{
		// 현재 봉인 되어있는 스킬 정보를 수집한다.
		if( m_kSkillTree.IsSkillUnsealed( it->first ) == false )
		{
			kPacket.m_vecUnSealedSkill.push_back( it->first );
		}
		
		// 현재 해당 스킬에 사용되고 있는 캐쉬 스킬 포인트를 구해오되 없다면, 0으로 설정한다.
		KAdminCheatSkill kAdminCheatSkill;

#ifdef SERV_SKILL_PAGE_SYSTEM
		if ( m_kSkillTree.IsActiveSkillPageNumberValid() )
		{
			// kimhc // 김현철 // 2013-11-17 // 이 부분은 기존 코드에서도 실패시 별다른 처리가 없기에
			// 그냥 다음 Step 으로 진행 하도록 하였음
			m_kSkillTree.GetSkillLevelAndCSP( kAdminCheatSkill.m_iSkillLevel, kAdminCheatSkill.m_iSkillCSPoint, 
				it->first, m_kSkillTree.AccessLearnedSkillTree() );
		}
		else
		{
			AddLogWhenSkillPagesNumberIsWrong( L"EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ" );
			return;
		}
#else // SERV_SKILL_PAGE_SYSTEM
		if( m_kSkillTree.GetSkillLevelAndCSP( it->first, kAdminCheatSkill.m_iSkillLevel, kAdminCheatSkill.m_iSkillCSPoint ) == false )
		{
			kAdminCheatSkill.m_iSkillCSPoint = 0;
		}
#endif // SERV_SKILL_PAGE_SYSTEM
		
		// 스킬 레벨을 최대로 설정한다.
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		kAdminCheatSkill.m_iSkillLevel = it->second.m_iMasterSkillLevel;
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		kAdminCheatSkill.m_iSkillLevel = it->second.m_iMaxSkillLevel;
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

		kPacket.m_mapSkillInfo.insert( std::map<int, KAdminCheatSkill>::value_type( it->first, kAdminCheatSkill ) );
	}
	SendToGameDB( DBE_ADMIN_CHEAT_GET_ALL_SKILL_REQ, kPacket );

	// 치트 로그
	WriteCheatLogToDB( L"/fsk" );
}

IMPL_ON_FUNC( DBE_ADMIN_CHEAT_GET_ALL_SKILL_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"일부 스킬에 대해서 실패하였습니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
	}

	// 스킬 봉인 해제
	if( kPacket_.m_vecUnSealedSkill.empty() == false )
	{
		std::set<int>::iterator it;
		BOOST_TEST_FOREACH( short, iSkillID, kPacket_.m_vecUnSealedSkill )
		{
			m_kSkillTree.SkillUnseal( iSkillID );
		}
	}

	if( kPacket_.m_mapSkillInfo.empty() == false )
	{
		std::map<int, KAdminCheatSkill>::iterator it;
		for( it=kPacket_.m_mapSkillInfo.begin(); it != kPacket_.m_mapSkillInfo.end(); ++it )
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			// kimhc // 김현철 // 모든 스킬 배우기 치트 사용 시, 현재 사용중인 스킬 페이지에 적용 되도록 함
			m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( it->first, it->second.m_iSkillLevel, it->second.m_iSkillCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetSkillLevelAndCSP( it->first, it->second.m_iSkillLevel, it->second.m_iSkillCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM
		}
	}

	//
	const CXSLSkillTree::SkillTreeTempletMap* pSkillTreeTempletMap = SiCXSLSkillTree()->GetSkillTreeTemplet( GetUnitClass() );
	if( pSkillTreeTempletMap == NULL )
	{
		START_LOG( cerr, L"해당 클래스의 스킬 정보를 얻어올 수 없습니다.")
			<< BUILD_LOG( GetUnitClass() )
			<< END_LOG;
	}

	kPacket_.m_vecUnSealedSkill.clear();

	CXSLSkillTree::SkillTreeTempletMap::const_iterator it;
	for( it=pSkillTreeTempletMap->begin(); it != pSkillTreeTempletMap->end(); ++it )
	{
		if( SiCXSLSkillTree()->IsSealSkill( it->first ) == true )
		{
			kPacket_.m_vecUnSealedSkill.push_back( it->first );
		}
	}

	// 클라이언트에게 알리기
	SendPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK, kPacket_ );
}
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
//}}

//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
IMPL_ON_FUNC( EGS_UDP_KICK_GAMEEDIT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}


	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_CN_SERVER, 0, NULL, ECN_UDP_KICK_GAMEEDIT_NOT, kPacket_ );

}

#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

//{{ 배틀필드 위험도 확인 치트 - 김민성
#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
IMPL_ON_FUNC_NOPARAM( EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..?ㅁ?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT kPacket;
		kPacket.m_iDangerousValue = -1;
		SendPacket( EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT, kPacket );
		return;
	}

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				SendToCnRoom( ERM_BATTLE_FIELD_DANGEROUS_VALUE_REQ );

				START_LOG( cout, L"배틀 필드 위험도 확인!." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetRoomUID() )
					<< END_LOG;
			}
			break;

		default:
			START_LOG( cerr, L"배틀 필드가 아닌데 위험도 확인을 하려 합니다." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetRoomUID() )
				<< END_LOG;
		}
	}	
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_DANGEROUS_VALUE_ACK, int )
{
	KEGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT kPacket;
	kPacket.m_iDangerousValue = kPacket_;
	SendPacket( EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT, kPacket );

	START_LOG( cout, L"현재 배틀 필드의 위험도를 확인합니다." )
		<< BUILD_LOG( GetRoomUID() )
		<< BUILD_LOG( kPacket.m_iDangerousValue )
		<< END_LOG;
}
#endif SERV_BATTLE_FIELD_DANGEROUS_CHEAT
//}}

//{{ 2012. 11. 20	박세훈	영웅 대전 참가자 관리용 리스트
#ifdef SERV_HERO_PVP_MANAGE_LIST
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_HERO_PVP_USER_LIST_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_HERO_PVP_USER_LIST_ACK kPacket;
		SendPacket( EGS_ADMIN_HERO_PVP_USER_LIST_ACK, kPacket );
		return;
	}
	
	SendToGlobalServer( EGB_ADMIN_HERO_PVP_USER_LIST_REQ );
	
	// 치트 로그
	WriteCheatLogToDB( L"/obsread");
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_LIST_ACK, KEGS_ADMIN_HERO_PVP_USER_LIST_ACK )
{
	SendPacket( EGS_ADMIN_HERO_PVP_USER_LIST_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_HERO_PVP_USER_ADD_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_HERO_PVP_USER_ADD_ACK kPacket;
		SendPacket( EGS_ADMIN_HERO_PVP_USER_ADD_ACK, kPacket );
		return;
	}

	SendToGlobalServer( EGB_ADMIN_HERO_PVP_USER_ADD_REQ, kPacket_ );

	// 치트 로그
	WriteCheatLogToDB( L"/obsadd");
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_ADD_ACK, KEGS_ADMIN_HERO_PVP_USER_ADD_ACK )
{
	SendPacket( EGS_ADMIN_HERO_PVP_USER_ADD_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_HERO_PVP_USER_REMOVE_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_HERO_PVP_USER_REMOVE_ACK kPacket;
		SendPacket( EGS_ADMIN_HERO_PVP_USER_REMOVE_ACK, kPacket );
		return;
	}

	SendToGlobalServer( EGB_ADMIN_HERO_PVP_USER_REMOVE_REQ, kPacket_ );

	// 치트 로그
	WriteCheatLogToDB( L"/obsdel");
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_REMOVE_ACK, KEGS_ADMIN_HERO_PVP_USER_REMOVE_ACK )
{
	SendPacket( EGS_ADMIN_HERO_PVP_USER_REMOVE_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KPacketOK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket );
		return;
	}

	// 캐릭터 닉네임 길이 검사.
	if( kPacket_.m_wstrNickName.empty() == false )
	{
		if( ( kPacket_.m_wstrNickName.size() < 2 ) || ( 13 < kPacket_.m_wstrNickName.size() ) )
		{
			START_LOG( cerr, L"캐릭터 닉네임 길이가 잘못되었습니다.")
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< END_LOG;

			KPacketOK kPacket;
			kPacket.m_iOK = NetError::ERR_CREATE_UNIT_04;
			SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket );
			return;
		}
		
#ifdef EXTEND_SERVER_GROUP_MASK
		if( kPacket_.m_iServerGroupID <= -1 )
#else EXTEND_SERVER_GROUP_MASK
		if( ( kPacket_.m_iServerGroupID <= SEnum::SGI_INVALID ) || ( SEnum::SGI_END <= kPacket_.m_iServerGroupID ) )
#endif EXTEND_SERVER_GROUP_MASK
		{
			START_LOG( cerr, L"잘못된 서버군 아이디")
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iServerGroupID )
				<< END_LOG;

			KPacketOK kPacket;
			kPacket.m_iOK = NetError::ERR_HERO_PVP_USER_LIST_01;
			SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket );
			return;
		}
	}

	SendToGlobalServer( EGB_ADMIN_NOTIFY_HERO_PVP_USER_REQ, kPacket_ );

	// 치트 로그
	CStringW cwstrCheatContents;

	if( kPacket_.m_wstrNickName.empty() == true )
	{
		cwstrCheatContents.Format( L"%s", kPacket_.m_wstrNickName.c_str() );
		WriteCheatLogToDB( L"/obs", cwstrCheatContents );
	}
	else
	{
		cwstrCheatContents.Format( L"%d %s", kPacket_.m_iServerGroupID, kPacket_.m_wstrNickName.c_str() );
		WriteCheatLogToDB( L"/obsw", cwstrCheatContents );
	}
}

_IMPL_ON_FUNC( EGB_ADMIN_NOTIFY_HERO_PVP_USER_ACK, KPacketOK )
{
	SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EGB_ADMIN_NOTIFY_HERO_PVP_USER_NOT, KEGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ )
{
	SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_CONNECTION_NOT, KEGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT )
{
	SendPacket( EGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_CHECK_NOT, bool )
{
	m_bHeroPvpUser = kPacket_;
}
#endif SERV_HERO_PVP_MANAGE_LIST
//}}

//{{ 2012. 12. 5	최육사		옵저버 대전 난입 모드
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
IMPL_ON_FUNC( EGS_ADMIN_OBSERVER_MODE_REQ )
{
	KEGS_ADMIN_OBSERVER_MODE_ACK kPacket;

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_OBSERVER_MODE_ACK, kPacket );
		return;
	}

	// 옵저버 모드 세팅
	m_kUserCheatManager.SetObserverMode( kPacket_.m_bObserverMode );

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_ADMIN_OBSERVER_MODE_ACK, kPacket );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_bObserverMode );
	WriteCheatLogToDB( L"/observer", cwstrCheatContents );
}
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
//}}

#ifdef SERV_DELETE_GUILD_ADD_CHEAT
IMPL_ON_FUNC( EGS_ADMIN_DELETE_GUILD_ADD_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..?ㅁ?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_DELETE_GUILD_ADD_NOT kPacket;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;

	SendToLoginServer( ELG_ADMIN_DELETE_GUILD_ADD_NOT, kPacket );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iGuildUID );

	WriteCheatLogToDB( L"/dg", cwstrCheatContents );
}
#endif // SERV_DELETE_GUILD_ADD_CHEAT

//{{ 2011.03.04  임규수 헤니르 랭킹 삭제 치트 ( 운영자,개발자 계정 )
#ifdef SERV_DELETE_HENIR_RANKING
IMPL_ON_FUNC( EGS_ADMIN_DELETE_HENIR_RANKING_RANK_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..?ㅁ?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	if( kPacket_.m_iRank < 1 || kPacket_.m_iRank > 100 )
	{
		START_LOG( cerr, L"1위 부터 100위 까지만 삭제 가능" )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_DELETE_HENIR_RANKING_RANK_NOT kPacket;
	kPacket.m_iRankingType = kPacket_.m_iRankingType;
	kPacket.m_iRank = kPacket_.m_iRank;

	SendToLoginServer( ELG_ADMIN_DELETE_HENIR_RANKING_RANK_NOT, kPacket );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d", kPacket_.m_iRankingType, kPacket_.m_iRank );

	WriteCheatLogToDB( L"/dhr", cwstrCheatContents );
}

IMPL_ON_FUNC( EGS_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..?ㅁ?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT kPacket;
	kPacket.m_iRankingType = kPacket_.m_iRankingType;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;

	SendToLoginServer( ELG_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT, kPacket );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d, %s", kPacket_.m_iRankingType, kPacket_.m_wstrNickName );

	WriteCheatLogToDB( L"/dhn", cwstrCheatContents );
}

IMPL_ON_FUNC( EGS_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..?ㅁ?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT kPacket;
	kPacket.m_iRankingType = kPacket_.m_iRankingType;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;

	SendToLoginServer( ELG_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT, kPacket );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d", kPacket_.m_iRankingType, kPacket_.m_iUnitUID );

	WriteCheatLogToDB( L"/dhu", cwstrCheatContents );
}
#endif SERV_DELETE_HENIR_RANKING
//}}

//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
IMPL_ON_FUNC( EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK );

	KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK kPacket;

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..위험도 증가 하려고 했음" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, kPacket );
		return;
	}

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				SendToCnRoom( ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ, kPacket_ );

				START_LOG( cout, L"배틀 필드 위험도 확인!." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetRoomUID() )
					<< END_LOG;
			}
			break;

		default:
			{
				START_LOG( cerr, L"배틀 필드가 아닌데 위험도 증가 하려 합니다." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetRoomUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BATTLEFIELD_07;
				SendPacket( EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, kPacket );
				return;
			}
		}
	}		
}

_IMPL_ON_FUNC( ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK )
{
	SendPacket( EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, kPacket_ );

	START_LOG( cout, L"현재 배틀 필드의 위험도를 증가합니다." )
		<< BUILD_LOG( GetRoomUID() )
		<< BUILD_LOG( kPacket_.m_iDanger )
		<< END_LOG;

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iDanger );
	WriteCheatLogToDB( L"/afd", cwstrCheatContents );
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}
#ifdef SERV_POINT_COUNT_SYSTEM_GAME_EDIT_COMMAND
IMPL_ON_FUNC( EGS_QUEST_POINT_CHEAT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"개발자도 아닌데 감히" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;
		return;
	}

	if (kPacket_.m_iQuestPointCheat < 0)
	{
		return;
	}

	if ( kPacket_.m_iQuestPointCheat > 200 )
		kPacket_.m_iQuestPointCheat = 200;

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iQuestPointCheat );
	WriteCheatLogToDB( L"/questpoint", cwstrCheatContents );

#ifdef SERV_POINT_COUNT_SYSTEM
	m_kUserQuestManager.SetUpdateQuestInstance(GetThisPtr<KGSUser>());
#endif //SERV_POINT_COUNT_SYSTEM
}
#endif //SERV_POINT_COUNT_SYSTEM_GAME_EDIT_COMMAND

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( EGS_ADMIN_CHANGE_COUPLE_DATE_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..위험도 증가 하려고 했음" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_COUPLE_DATE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_COUPLE_DATE_ACK, kPacket );
		return;
	}

	if( m_kUserRelationshipManager.GetRelationshipType() < SEnum::RT_COUPLE )
	{
		KEGS_ADMIN_CHANGE_COUPLE_DATE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_36;
		SendPacket( EGS_ADMIN_CHANGE_COUPLE_DATE_ACK, kPacket );
		return;
	}

	START_LOG( cout, L"커플 맺은 시간을 변경합니다." )
		<< BUILD_LOG( GetRoomUID() )
		<< BUILD_LOG( kPacket_.m_iAddHour )
		<< END_LOG;

	KDBE_ADMIN_CHANGE_COUPLE_DATE_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_iLoverUnitUID = m_kUserRelationshipManager.GetLoverUnitUID();
	kPacket.m_fAddRate = ((float)kPacket_.m_iAddHour) / 24.0f * (-1.f);

	SendToGameDB( DBE_ADMIN_CHANGE_COUPLE_DATE_REQ, kPacket );

	// 치트 로그
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iAddHour );
	WriteCheatLogToDB( L"/couple", cwstrCheatContents );

}

_IMPL_ON_FUNC( DBE_ADMIN_CHANGE_COUPLE_DATE_ACK, KEGS_ADMIN_CHANGE_COUPLE_DATE_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"커플 날짜 변경치트키 사용 오류!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
	}

	SendPacket( EGS_ADMIN_CHANGE_COUPLE_DATE_ACK, kPacket_ );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_EVENT_CHECK_POWER
IMPL_ON_FUNC( EGS_SET_MULTIPLYER )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"개쓰레기 해커 새끼" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	SiKGameSysVal()->m_fMultiplayer = kPacket_.fM;
}
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-09	// 박세훈
IMPL_ON_FUNC( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cwarn, L"허용되지 않은 권한을 사용하려 하였습니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_10;	// 사용할 수 없는 권한입니다.
		SendPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK, kPacket );
		return;
	}

	// 게이트가 열리는 곳인가?
	if( SiCXSLFieldBossData()->DoesFieldAffectTotalDangerousValue( kPacket_.m_iBattleFieldID ) == false )
	{
		KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_01;	// 해당 필드에서는 보스 필드 게이트가 생성되지 않습니다.
		SendPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK, kPacket );
		return;
	}

	SendToGlobalServer( EGB_ADMIN_BOSS_FIELD_GATE_OPEN_REQ, kPacket_ );

	// 치트 로그 남겨야 함.
	// WriteCheatLogToDB 체크
}

IMPL_ON_FUNC( EGB_ADMIN_BOSS_FIELD_GATE_OPEN_ACK )
{
	SendPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cwarn, L"허용되지 않은 권한을 사용하려 하였습니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	SendToGlobalServer( EGB_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT );

	// 치트 로그 남겨야 함.
	// WriteCheatLogToDB 체크
}

IMPL_ON_FUNC( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cwarn, L"허용되지 않은 권한을 사용하려 하였습니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		
		KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK, kPacket );
		return;
	}

	// 게이트가 열리는 곳인가?
	if( SiCXSLFieldBossData()->DoesFieldAffectTotalDangerousValue( kPacket_.m_iBattleFieldID ) == false )
	{
		KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_01;	// 해당 필드에서는 보스 필드 게이트가 생성되지 않습니다.
		SendPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK, kPacket );
		return;
	}

	SendToGlobalServer( EGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ, kPacket_ );

	// 치트 로그 남겨야 함.
	// WriteCheatLogToDB 체크
}

IMPL_ON_FUNC( EGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK )
{
	SendPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cwarn, L"허용되지 않은 권한을 사용하려 하였습니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		
		KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_COUPLE_DATE_ACK, kPacket );
		return;
	}

	// 게이트가 열리는 곳인가?
	if( SiCXSLFieldBossData()->DoesFieldAffectTotalDangerousValue( kPacket_.m_iBattleFieldID ) == false )
	{
		KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_01;	// 해당 필드에서는 보스 필드 게이트가 생성되지 않습니다.
		SendPacket( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK, kPacket );
		return;
	}

	SendToGlobalServer( EGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ, kPacket_ );

	// 치트 로그 남겨야 함.
	// WriteCheatLogToDB 체크
}

IMPL_ON_FUNC( EGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK )
{
	SendPacket( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK, kPacket_ );
}
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
IMPL_ON_FUNC( EGS_EVENT_COBO_ITEM_GIVE_CHEAT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..?ㅁ?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}
	if(kPacket_.m_bNextDay == true)
	{
		KEGS_EVENT_COBO_DUNGEON_FIELD_NOT kPacketCoboNot;
		//현재 시간을 받아온다.
		CTime tChangeEventTime = SiKGameEventScriptManager()->GetCoboEventData()[0];
		CTime tCurTime_ = CTime::GetCurrentTime();
		//현재 시간과 바뀌어야 하는 이벤트 시간을 비교한다.
		std::wstring GameEventTime = tChangeEventTime.Format(L"%Y-%m-%d %H:%M:%S");

		if( tCurTime_ < tChangeEventTime)
		{
			///현재 시간이 바뀌어야 하는 기준 시간보다 작으면 던전 클리어를 열어 주어야 한다.
			///단 주말일 경우에는 필드 카운트도 열어주어야 하니까 주말인지 체크를 해야한다.
			CTime	tWeekEndTimeStart = SiKGameEventScriptManager()->GetCoboEventData()[1];
			CTime   tWeekEndTimeEnd = SiKGameEventScriptManager()->GetCoboEventData()[2];
			GameEventTime = tWeekEndTimeStart.Format(L"%Y-%m-%d %H:%M:%S");
			START_LOG( cerr, L"던전 주말 타임 체크시작" )
				<< BUILD_LOG( GameEventTime )
				<< END_LOG;

			GameEventTime = tWeekEndTimeEnd.Format(L"%Y-%m-%d %H:%M:%S");
			START_LOG( cerr, L"던전 주말 타임 체크끝" )
				<< BUILD_LOG( GameEventTime )
				<< END_LOG;
			///하루가 지났으면 보상 기록 초기화 하고 UI활성화 시켜주면 된다.
			kPacketCoboNot.m_StartButtonUI = true;
			kPacketCoboNot.m_DungeonCountUI = true;
			kPacketCoboNot.m_DungeonCount = 0;
			kPacketCoboNot.m_iRemaindTime = -1;
			if(tCurTime_ > tWeekEndTimeStart && tCurTime_ < tWeekEndTimeEnd) //이러면 주말이다
			{
				//주말이면 필드카운트 UI도 활성화 
				kPacketCoboNot.m_FieldCountUI = true;
				kPacketCoboNot.m_FieldMonsterKillCount = 0;
			}
			SendPacket(EGS_EVENT_COBO_DUNGEON_FIELD_NOT,kPacketCoboNot);
			START_LOG( cerr, L"클릭한적 있을떄 던전 보상 지급받았을때 하루 지났을때" )
				<< BUILD_LOG( kPacketCoboNot.m_StartButtonUI )
				<< BUILD_LOG( kPacketCoboNot.m_DungeonCountUI )
				<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
				<< BUILD_LOG( kPacketCoboNot.m_iRemaindTime )
				<< BUILD_LOG( kPacketCoboNot.m_FieldCountUI )
				<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
				<< END_LOG;
			///DB에 초기화 하러 가자
			//여기서 몬스터 킬수랑 던전 클리어수 보관해야 하니까 저장 하자
			KDBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT kPacketToDB;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_bItemGive = 0;
			kPacketToDB.m_wstrButtonClickTime_One = GetButtonClickTime().Format(L"%Y-%m-%d %H:%M:%S");
			kPacketToDB.m_iDungeonClearCount = 0;
			kPacketToDB.m_iFieldMonsterKillCount = 0;
			SendToGameDB( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT, kPacketToDB );
			SetRemaindTime(kPacketCoboNot.m_iRemaindTime);
			SetStartButtonPush(kPacketCoboNot.m_StartButtonUI);
			SetDungeonClearUI(kPacketCoboNot.m_DungeonCountUI);
			SetFieldCountUI(kPacketCoboNot.m_FieldCountUI);
			SetDungeonCount(kPacketCoboNot.m_DungeonCount);
			SetFieldMosterKillCount(kPacketCoboNot.m_FieldMonsterKillCount);
			SetCoboItemGive(false);

		}			
		else
		{
			///현재 시간이 바뀌어야 하는 기준 시간보다 작으면 던전 클리어를 열어 주어야 한다.
			///단 주말일 경우에는 필드 카운트도 열어주어야 하니까 주말인지 체크를 해야한다.
			CTime	tWeekEndTimeStart = SiKGameEventScriptManager()->GetCoboEventData()[3];
			CTime tWeekEndTimeEnd = SiKGameEventScriptManager()->GetCoboEventData()[4];
			CTime TempTime = SiKGameEventScriptManager()->GetCoboEventData()[0];
			GameEventTime = tWeekEndTimeStart.Format(L"%Y-%m-%d %H:%M:%S");
			START_LOG( cerr, L"필드 주말 타임 체크시작" )
				<< BUILD_LOG( GameEventTime )
				<< END_LOG;
			GameEventTime = tWeekEndTimeEnd.Format(L"%Y-%m-%d %H:%M:%S");
			START_LOG( cerr, L"필드 주말 타임 체크끝" )
				<< BUILD_LOG( GameEventTime )
				<< END_LOG;
			///하루가 지났으면 보상 기록 초기화 하고 UI활성화 시켜주면 된다.
			kPacketCoboNot.m_StartButtonUI = true;
			kPacketCoboNot.m_FieldCountUI = true;
			kPacketCoboNot.m_FieldMonsterKillCount = 0;
			kPacketCoboNot.m_DungeonCount = 0;
			kPacketCoboNot.m_iRemaindTime = -1;
			if(tCurTime_ > tWeekEndTimeStart && tCurTime_ < tWeekEndTimeEnd) //이러면 주말이다
			{
				//주말이면 필드카운트 UI도 활성화 
				kPacketCoboNot.m_DungeonCountUI = true;
			}
			SendPacket(EGS_EVENT_COBO_DUNGEON_FIELD_NOT,kPacketCoboNot);
			START_LOG( cerr, L"클릭한적 있을떄 던전 보상 지급받았을때 하루 지났을때" )
				<< BUILD_LOG( kPacketCoboNot.m_StartButtonUI )
				<< BUILD_LOG( kPacketCoboNot.m_DungeonCountUI )
				<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
				<< BUILD_LOG( kPacketCoboNot.m_iRemaindTime )
				<< BUILD_LOG( kPacketCoboNot.m_FieldCountUI )
				<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
				<< END_LOG;
			///DB에 초기화 하러 가자
			KDBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT kPacketToDB;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_bItemGive = 0;
			kPacketToDB.m_wstrButtonClickTime_One = GetButtonClickTime().Format(L"%Y-%m-%d %H:%M:%S");
			kPacketToDB.m_iDungeonClearCount = 0;
			kPacketToDB.m_iFieldMonsterKillCount = 0;
			SendToGameDB( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT, kPacketToDB );
			SetRemaindTime(kPacketCoboNot.m_iRemaindTime);
			SetStartButtonPush(kPacketCoboNot.m_StartButtonUI);
			SetDungeonClearUI(kPacketCoboNot.m_DungeonCountUI);
			SetFieldCountUI(kPacketCoboNot.m_FieldCountUI);
			SetDungeonCount(kPacketCoboNot.m_DungeonCount);
			SetFieldMosterKillCount(kPacketCoboNot.m_FieldMonsterKillCount);
			SetCoboItemGive(false);
		}
	}
	else
	{
		//보상 기록 초기화 
		KDBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT kPacketToDB;
		kPacketToDB.m_iUnitUID = GetCharUID();
		kPacketToDB.m_bItemGive = kPacket_.m_CoboEventITemGet;
		kPacketToDB.m_wstrButtonClickTime_One = GetButtonClickTime().Format(L"%Y-%m-%d %H:%M:%S");
		kPacketToDB.m_iDungeonClearCount = 0;
		kPacketToDB.m_iFieldMonsterKillCount = 0;
		SendToGameDB( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT, kPacketToDB );
	}
}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
IMPL_ON_FUNC( EGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_CHEAT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"운영자가 아닌데 감히..?ㅁ?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}
	//치트 성공 
	///초기화를 시켜주고 DB에 정보 기록하고 클라에 알려주자
	SetValentineItemCount( kPacket_.m_iValentineItemCount );
	KEGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT kPacketValen;
	kPacketValen.m_iValentineItemCount = kPacket_.m_iValentineItemCount;
	SendPacket(EGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT,kPacketValen);
}
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"

#include "Enum/Enum.h"

#ifdef SERV_COUNTRY_CN

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
void KGSUser::ApplyAntiAddictionRatio(KEGS_NPC_UNIT_DIE_NOT& kPacket)
{
	std::map< UidType, KEXPData >::iterator mit = kPacket.m_EXPList.find( GetCharUID() );
	if( mit != kPacket.m_EXPList.end() )
	{
		mit->second.m_iEXP = static_cast<int>(mit->second.m_iEXP * m_AntiAddictionManager.GetRewardRatio());
		mit->second.m_iPartyEXP = static_cast<int>(mit->second.m_iPartyEXP * m_AntiAddictionManager.GetRewardRatio());
		mit->second.m_iSocketOptEXP = static_cast<int>(mit->second.m_iSocketOptEXP * m_AntiAddictionManager.GetRewardRatio());
	}
}

IMPL_ON_FUNC( DBE_GET_ANTI_ADDICTION_INFO_ACK )
{
	m_tUnitSelectTime = CTime::GetCurrentTime();

	AntiAddictionManager::AntiAddictionInfo info;
	info.m_tLastLogin = m_tUnitSelectTime;

	// 마지막 로그아웃 시간
	if(kPacket_.m_iOK == NetError::NET_OK)
	{
		if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrLastLogoutTime, info.m_tLastLogout ) == false )
		{
			START_LOG( cerr, L"문자열 시간조립 실패.!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kPacket_.m_wstrLastLogoutTime )
				<< END_LOG;

			// 에러났으니 어쩔수 없이 일단 현재 접속 시간으로 업데이트하도록 하자
			info.m_tLastLogout = m_tUnitSelectTime;
		}
	}
	// 기존 접속 데이터가 없다면(신규 계정) 로그아웃 시간 = 최초 접속 시간
	else
	{
		info.m_tLastLogout = m_tUnitSelectTime;

		START_LOG( clog, L"방침미 데이터가 없음. 마지막 로그아웃 시간을 최초 접속 시간으로 대체." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrLastLogoutTime )
			<< END_LOG;
	}
	info.m_iPlayTime = kPacket_.m_iPlayTime;
	info.m_iRestTime = kPacket_.m_iRestTime;

	START_LOG( clog, L"방침미 정보" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( info.m_tLastLogin.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetBuffer() ) 
		<< BUILD_LOG( info.m_tLastLogout.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetBuffer() ) 
		<< BUILD_LOG( info.m_iPlayTime ) 
		<< BUILD_LOG( info.m_iRestTime )
		<< END_LOG;

	m_AntiAddictionManager.Init(GetUID(), info);
	m_AntiAddictionManager.SetEnable(true);
}

IMPL_ON_FUNC( EGS_ANTI_ADDICTION_NOT )
{
	START_LOG( clog, L"방침미 공지 메세지" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_iType )
		<< BUILD_LOG( kPacket_.m_iPlayTime )
		<< END_LOG;

	SendPacket( EGS_ANTI_ADDICTION_NOT, kPacket_ );
}
#endif SERV_ANTI_ADDICTION_SYSTEM
//}}

IMPL_ON_FUNC( EGIANT_ROLEREG_CREATE_UNIT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_CreateUnitReqPacket.m_wstrNickName )
		<< BUILD_LOG( kPacket_.m_CreateUnitReqPacket.m_iClass );

	if(kPacket_.m_iOK == NetError::NET_OK)
	{
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		if(m_bNickNameCheckOnly == false)
		{
			m_bNickNameCheckOnly = true;
			KEGS_CHANGE_NICK_NAME_REQ kReq;
			kReq.m_wstrNickName = kPacket_.m_CreateUnitReqPacket.m_wstrNickName;
			kReq.m_iUnitUID = m_iBackUpUnitUID;
			kReq.m_bCheckOnly = false;
			SendToGameDB( DBE_CHANGE_NICK_NAME_REQ, kReq );

			m_iBackUpUnitUID = 0;
			
		}
		else
		{
			m_bNickNameCheckOnly = true;
			m_iBackUpUnitUID = 0;

			//{{ 2011. 08. 09  김민성 (2011.08.11) 특정일 이후 생성한 계정에 대하여 신규케릭터 생성 시 아이템 지급 이벤트
#ifdef SERV_NEW_CREATE_CHAR_EVENT
			KDBE_GAME_CREATE_UNIT_REQ kReq;

			KAccountInfo AccountTemp;
			GetAccountInfo( AccountTemp );
			kReq.m_wstrRegDate = AccountTemp.m_wstrRegDate;
			kReq.m_wstrNickName = kPacket_.m_CreateUnitReqPacket.m_wstrNickName;
			kReq.m_iClass = kPacket_.m_CreateUnitReqPacket.m_iClass;
			SendToGameDB( DBE_GAME_CREATE_UNIT_REQ, kReq );

#else //SERV_NEW_CREATE_CHAR_EVENT
			SendToGameDB( DBE_GAME_CREATE_UNIT_REQ, kPacket_.m_CreateUnitReqPacket );
#endif //SERV_NEW_CREATE_CHAR_EVENT
			//}}
		}
#else //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
			SendToGameDB( DBE_GAME_CREATE_UNIT_REQ, kPacket_.m_CreateUnitReqPacket );
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		
	}
	else
	{
		// 실패
		LOG_CONDITION( kPacket_.m_iOK == NetError::ERR_CREATE_UNIT_03, cerr, cwarn )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_CreateUnitReqPacket.m_iClass)
			<< BUILD_LOG( kPacket_.m_CreateUnitReqPacket.m_wstrNickName )
			<< END_LOG;

		// 결과 전송
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		if(m_iBackUpUnitUID != 0)
		{
			KEGS_CHANGE_NICK_NAME_ACK kPacketAck;

			kPacketAck.m_iOK = kPacket_.m_iOK;
			kPacketAck.m_iUnitUID = m_iBackUpUnitUID;
			kPacketAck.m_wstrNickName = kPacket_.m_CreateUnitReqPacket.m_wstrNickName;
			kPacketAck.m_bCheckOnly = false;
			
			SendPacket( EGS_CHANGE_NICK_NAME_ACK, kPacketAck );


		}
		else
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		{
			KEGS_CREATE_UNIT_ACK kPacket;
			kPacket.m_iOK = kPacket_.m_iOK;

			SendPacket( EGS_CREATE_UNIT_ACK, kPacket );
		}
		
	}

	return;
}

IMPL_ON_FUNC( EGIANT_ROLEREG_CREATE_GUILD_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_CreateGuildReqPacket.m_iItemUID)
		<< BUILD_LOG( kPacket_.m_CreateGuildReqPacket.m_wstrGuildName )
		<< BUILD_LOG( kPacket_.m_CreateGuildReqPacket.m_wstrGuildMessage );

	if(kPacket_.m_iOK == NetError::NET_OK)
	{
		//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
		// Login Server로 길드 생성하러 가자!
		KELG_CREATE_GUILD_REQ kPacketToLG;
		kPacketToLG.m_iUnitUID = GetCharUID();
		kPacketToLG.m_iItemUID = kPacket_.m_CreateGuildReqPacket.m_iItemUID;
		kPacketToLG.m_wstrGuildName =  kPacket_.m_CreateGuildReqPacket.m_wstrGuildName;
		kPacketToLG.m_wstrGuildMessage = kPacket_.m_CreateGuildReqPacket.m_wstrGuildMessage;
		SendToLoginServer( ELG_CREATE_GUILD_REQ, kPacketToLG );
#else
		// DB로 길드생성 가능한지 검사하러 가자!
		KDBE_CREATE_GUILD_REQ kPacketToDB;
		kPacketToDB.m_iUnitUID = GetCharUID();
		kPacketToDB.m_iItemUID = kPacket_.m_CreateGuildReqPacket.m_iItemUID;
		kPacketToDB.m_wstrGuildName = kPacket_.m_CreateGuildReqPacket.m_wstrGuildName;
		kPacketToDB.m_wstrGuildMessage = kPacket_.m_CreateGuildReqPacket.m_wstrGuildMessage;
		SendToGameDB( DBE_CREATE_GUILD_REQ, kPacketToDB );
#endif SERV_GUILD_CHANGE_NAME
		//}}
	}
	else
	{
		// 실패
		LOG_CONDITION( kPacket_.m_iOK == NetError::ERR_CREATE_UNIT_03, cerr, cwarn )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_CreateGuildReqPacket.m_iItemUID)
			<< BUILD_LOG( kPacket_.m_CreateGuildReqPacket.m_wstrGuildName )
			<< BUILD_LOG( kPacket_.m_CreateGuildReqPacket.m_wstrGuildMessage )
			<< END_LOG;

		KEGS_CREATE_GUILD_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;

		SendPacket( EGS_CREATE_GUILD_ACK, kPacket );
	}

	return;
}

#ifdef SERV_NEW_PUNISHMENT
IMPL_ON_FUNC( DBE_GET_PUNISHMENT_ACK )
{
	// 처벌 기록 적용
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		BOOST_TEST_FOREACH( KPunishmentInfo, info, kPacket_.m_vecPunishment )
		{
			m_PunishmentManager.SetPunishment(info);
		}
	}
}

IMPL_ON_FUNC( EGIANT_INFO_PUNISH_REQ )
{

}
#endif SERV_NEW_PUNISHMENT

#ifdef SERV_INFOSERVER_ADD_WORK
IMPL_ON_FUNC( EGIANT_INFO_REQUESTID_REQ )
{
	KEGIANT_INFO_REQUESTID_ACK kPacket;

	kPacket.m_wstrUserName = kPacket_.m_wstrUserName;
	kPacket.m_wstrGMName = kPacket_.m_wstrGMName;
	kPacket.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
	kPacket.m_wstrAccount = kPacket_.m_wstrAccount;
	kPacket.m_bWrite = true;

	SendToLoginServer(EGIANT_INFO_REQUESTID_ACK, kPacket );


}
#endif //SERV_INFOSERVER_ADD_WORK

#ifdef SERV_CBT2_CASH_DISTRIBUTION_EVENT
IMPL_ON_FUNC( EBILL_GET_TRANS_FOR_CBT2_CASH_DISTRIBUTION_EVENT_ACK )
{
	VERIFY_STATE( ( 4, KGSFSM::S_UID_UPDATED, KGSFSM::S_LOGINED, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK != NetError::NET_OK)
	{
		START_LOG( cerr, L"CBT2 캐시 증정 이벤트 트랜잭션 생성 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iOK ) 
			<< BUILD_LOG( kPacket_.m_wstrTransaction )
			<< END_LOG;

		return;
	}

	KEBILL_CBT2_CASH_DISTRIBUTION_EVENT_REQ kPacket;
	kPacket.m_wstrTransaction = kPacket_.m_wstrTransaction;
	kPacket.m_iUserUID = GetUID();
	kPacket.m_wstrUserID = GetName();
	kPacket.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	kPacket.m_wstrIP = KncUtil::toWideString( GetIPStr() );

	SendToBillingDB( EBILL_CBT2_CASH_DISTRIBUTION_EVENT_REQ, kPacket );
}

IMPL_ON_FUNC( EBILL_CBT2_CASH_DISTRIBUTION_EVENT_ACK )
{
	VERIFY_STATE( ( 4, KGSFSM::S_UID_UPDATED, KGSFSM::S_LOGINED, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK != NetError::NET_OK)
	{
		START_LOG( cerr, L"CBT2 캐시 증정 이벤트 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iOK ) 
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_iReceivedCash )
			<< END_LOG;
		return;
	}

	if(kPacket_.m_iReceivedCash > 0)
	{
		m_bNotifyCBT2CashEvent = true;
		m_iCBT2ReceivedCash = kPacket_.m_iReceivedCash;
	}
}
#endif SERV_CBT2_CASH_DISTRIBUTION_EVENT

#ifdef SERV_CBT2_PVP_COUNT_EVENT
IMPL_ON_FUNC( DBE_GET_PVP_COUNT_EVENT_ACK )
{
	KEGS_CBT2_PVP_COUNT_NOT kPacket;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대전장 영웅 이벤트 정보 가져오기 실패!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_iKillCount )
			<< BUILD_LOG( kPacket_.m_iDieCount )
			<< END_LOG;

		return;
	}

	m_iCBT2KillCount = kPacket_.m_iKillCount;
	m_iCBT2DieCount = kPacket_.m_iDieCount;

	kPacket.m_iKillCount = kPacket_.m_iKillCount;
	kPacket.m_iDieCount = kPacket_.m_iDieCount;

	SendPacket( EGS_CBT2_PVP_COUNT_NOT, kPacket );
}
#endif SERV_CBT2_PVP_COUNT_EVENT

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


#endif //SERV_COUNTRY_CN
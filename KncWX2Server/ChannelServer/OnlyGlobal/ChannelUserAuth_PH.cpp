
#include "ChannelUser.h"

#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_PH
#include <dbg/dbg.hpp>
//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
#include "CHFSM.h"
#else
#include "defaultFSM.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

#include "DBLayer.h"
#include "NetError.h"
#include "ChannelSimLayer.h"
#include "ChannelServer.h"

//{{ 2009. 12. 16  최육사	동접툴
#include "CCUToolManager.h"
//}}

//{{ 2011. 01. 20 김민성	동접 모니터링 툴               디비커리인젝션
#ifdef SERV_CCU_MONITORING_TOOL
#include "odbc/Odbc.h"
#endif SERV_CCU_MONITORING_TOOL
//}}

#include <boost/format.hpp>

//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
#include "MachineBlockManager.h"
#include "HackingProtectionManager.h"
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
#include "ServerSNMaker.h"
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}
//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
#include "WaitQueueManager.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}
//{{ 2012. 07. 18	최육사		재접속 로직
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
#include "DDOSGuardSimulator.h"
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
//}}

#pragma comment(lib, "../Common/OnlyGlobal/AuthAndBilling/PH/libeay32MD.lib")
#include "../Common/OnlyGlobal/AuthAndBilling/PH/Security/base64.h"
#include "../Common/OnlyGlobal/AuthAndBilling/PH/Security/OpensslRSA.h"

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef std::vector<uint8> dataBuffer_t;
typedef std::string string_t;


struct UserTokInfo_t
{
	uint32 Uid;
	char Username[32];
	uint32 Timestamp;
	uint16 GcaType;
	uint8 GoldMemberType;
	uint8 IpBonus;
};


void KChannelUser::RequestPublisherLogin(const KECH_VERIFY_ACCOUNT_REQ& kPacket_)
{
	START_LOG( clog, L"PH 인증 요청." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_strUserToken )
		<< BUILD_LOG( kPacket_.m_bLogin )
		<< END_LOG;

	KECH_VERIFY_ACCOUNT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;


	if ( GetKChannelSimLayer()->GetOpensslRSAInit() == false)
	{
		START_LOG( clog, L"ChannelServer OpensslRSA Init Failed" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_strUserToken )
			<< END_LOG;


		kPacket.m_iOK = NetError::ERR_VERIFY_20;


		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
		ReserveDestroy();
		return;
	}



	
	bool bisSignIn = true;
	
	dataBuffer_t bufToken;
	base64_decode(kPacket_.m_strUserToken, bufToken);


	if(bufToken.size() == 0)
	{
		START_LOG( cerr, L"Garena Invalid token format" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_strUserToken )
			<< END_LOG;

		bisSignIn = false;

		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
		ReserveDestroy();
		return;
		
	}

	

	//OpensslRSA.Init();
	
	dataBuffer_t bufDecrypt;
	//bool bRet = OpensslRSA.Decrypt((char *)&bufToken[0], bufToken.size(), bufDecrypt);
	bool bRet = GetKChannelSimLayer()->GetOpensslRSA().Decrypt((char *)&bufToken[0], bufToken.size(), bufDecrypt);
	if(!bRet)
	{
		START_LOG( cerr, L"Garena Decrypt token failed!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_strUserToken )
			<< END_LOG;
		bisSignIn = false;
	}
	
	

	if (bisSignIn == false)
	{
		START_LOG( clog, L"인증 실패" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_strUserToken )
			<< END_LOG;
	

		kPacket.m_iOK = NetError::ERR_VERIFY_20;

	
		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
		ReserveDestroy();
		return;
	}


	UserTokInfo_t tokInfo = {0};
	memcpy(&tokInfo, &bufDecrypt[0], sizeof(tokInfo));


	time_t tGarenaTimestamp = ntohl(tokInfo.Timestamp);
	//time_t testaaaa = 1375329078;
	CTime tCurrent = CTime::GetCurrentTime();
	CTimeSpan tTimeSpan = tCurrent.GetTime() - tGarenaTimestamp;

	int iCheckMinute = tTimeSpan.GetTotalMinutes();

	if ( iCheckMinute > 5 || iCheckMinute < 0 )
	{

		START_LOG( clog, L"Time Stamp 만료" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG(  iCheckMinute )
			<< END_LOG;


		kPacket.m_iOK = NetError::ERR_VERIFY_20;


		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
		ReserveDestroy();
		return;
	}

	//2013.11.13 lygan_조성욱 // 서비스 도중에 	m_wstrServiceAccountID를 퍼블리셔 SN 값으로 변경하는 이슈 발생해서 아래 코드 주석
	/*int iUsernameSize = strlen(tokInfo.Username);

	int size_needed = MultiByteToWideChar(CP_UTF8, 0, tokInfo.Username, iUsernameSize, NULL, 0);

	std::wstring wstrAccountID(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, tokInfo.Username, iUsernameSize, &wstrAccountID[0], size_needed);*/
	
	WCHAR wchGarenaUID[ 100 ];
	
	_ultow(ntohl(tokInfo.Uid),wchGarenaUID, 10);

	std::wstring wstrAccountID(wchGarenaUID);


	// 정상 로그인 성공
	KDBE_CH_USER_GENERAL_LOGIN_REQ kPacketReq;
	kPacketReq.m_wstrServiceAccountID = wstrAccountID;
	kPacketReq.m_uiPublisherUID = ntohl(tokInfo.Uid);
	kPacketReq.m_wstrIP = KncUtil::toWideString( GetIPStr(), strlen( GetIPStr() ) );
#ifdef SERV_PURCHASE_TOKEN
	kPacketReq.m_wstrPurchaseTok = L"";;
#endif SERV_PURCHASE_TOKEN
	kPacketReq.m_usGarenaCyberCafe = tokInfo.GcaType;
	kPacketReq.m_bServerUseKogOTP = KSimLayer::GetKObj()->GetUseKogOTP();
	
	
	SendToAccountDB( DBE_CH_USER_GENERAL_LOGIN_REQ, kPacketReq );
	
}

void KChannelUser::HandlePublisherLoginAck(const KEPUBLISHER_AUTHENTICATION_ACK& kPacket_, OUT bool& bDoNotDisconnect)
{
	// 국가마다 접속 해제 안하는 NetError 가 다를 수 있음
	if( kPacket_.m_iOK == NetError::ERR_GAMEFORGE_00 ||
		kPacket_.m_iOK == NetError::ERR_GAMEFORGE_01 ||
		kPacket_.m_iOK == NetError::ERR_GAMEFORGE_02 )
	{
		bDoNotDisconnect = true;
	}
	else
	{
		bDoNotDisconnect = false;
	}
	
}

void KChannelUser::RequestKOGOTPLogin(const KECH_VERIFY_ACCOUNT_REQ& kPacket_)
{
	// 이미 X2StateLogin에서 자이언트 인증이 끝났으므로 KOG OTP 인증으로 대체
	KDBE_CH_USER_KOGOTP_LOGIN_REQ kPacketReq;
	kPacketReq.m_wstrServiceAccountID = kPacket_.m_wstrID;
	kPacketReq.m_wstrOTP = kPacket_.m_wstrPassword;
	kPacketReq.m_wstrIP = KncUtil::toWideString(KSession::GetIPStr());
#ifdef SERV_PURCHASE_TOKEN
	kPacketReq.m_wstrPurchaseTok = L"";
#endif SERV_PURCHASE_TOKEN
#if defined( SERV_STEAM ) || defined( SERV_ALL_RENEWAL_SP )
	kPacketReq.m_iChannelingCode = kPacket_.m_iChannelingCode;
#endif //( SERV_STEAM ) || ( SERV_ALL_RENEWAL_SP )

	SendToAccountDB( DBE_CH_USER_KOGOTP_LOGIN_REQ, kPacketReq );
}

void KChannelUser::HandleKOGOTPLoginAck()
{
	// 미국의 경우 여기서 딱히 하는거 없음 
}

void KChannelUser::RequestPublisherSecurityAuth(const KECH_SECURITY_AUTH_REQ& kPacket_)
{

	
}

void KChannelUser::HandlePublisherSecurityAuthAck(const KEPUBLISHER_SECURITY_AUTH_ACK& kPacket_)
{
	// 유럽의 경우 여기서 딱히 하는거 없음 
}

void KChannelUser::HandlePublisherSecurityInfoNot(const KEPUBLISHER_SECURITY_INFO_NOT& kPacket_)
{

}

#endif // SERV_COUNTRY_PH
#endif // SERV_GLOBAL_AUTH

#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "LoginSMSDBThread.h"
#include "LoginServer.h"
#include "NetError.h"
#include "LoginSimLayer.h"

//ImplementDBThread( KLoginSMSDBThread );

#define CLASS_TYPE KLoginSMSDBThread

KLoginSMSDBThread::~KLoginSMSDBThread(void)
{
}

void KLoginSMSDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{    
		CASE( DBE_SEND_PHONE_MSG_NOT );
		//{{ 2010. 11. 2	최육사	서버간 접속 모니터링
#ifdef SERV_MORNITORING
	   _CASE( DBE_SEND_PHONE_MSG_FOR_MORNITORING_REQ, KDBE_SEND_PHONE_MSG_NOT );
#endif SERV_MORNITORING
		//}}

	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
	}
}

void KLoginSMSDBThread::SendToServer( unsigned short usEventID )
{
	SendToServer( usEventID, char() );
}

void KLoginSMSDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

IMPL_ON_FUNC( DBE_SEND_PHONE_MSG_NOT )
{
	BOOST_TEST_FOREACH( const std::wstring&, wstrPhoneNum, kPacket_.m_vecPhoneNum )
	{
// 		DO_QUERY_NO_PROFILE( L"insert into em_tran ( tran_phone, tran_callback, tran_status, tran_date, tran_msg )",
// 			L"values ( N\'%s\', N\'%s\', \'1\', getdate(), N\'%s\' )",
// 			% wstrPhoneNum
// 			% wstrPhoneNum
// 			% kPacket_.m_wstrSMSMessage
// 			);

		//DO_QUERY_NO_PROFILE( L"INSERT INTO SC_TRAN ( TR_SENDDATE, TR_SENDSTAT, TR_RSLTSTAT, TR_PHONE, TR_CALLBACK, TR_MSG )",
		//	L"values ( GetDate(),'0', '00', N\'%s\', N\'%s\', N\'%s\' )",
		//	% wstrPhoneNum
		//	% wstrPhoneNum
		//	% kPacket_.m_wstrSMSMessage
		//	);
        std::wstring wstrProject = L"ES";
        std::wstring wstrNation = KSimLayer::GetKObj()->GetAuthTypeStr( KSimLayer::GetKObj()->GetAuthFlag() );
        DO_QUERY_NO_PROFILE( L"exec SMS.dbo.SMS_Send", L"N\'%s\', N\'%s\',N\'%s\',N\'%s\',N\'%s\',%d, %d", 
            % wstrPhoneNum 
            % wstrPhoneNum
            % kPacket_.m_wstrSMSMessage
            % wstrProject
            % wstrNation
            % 0
            % 0 
            );
	}

	START_LOG( cout2, L"휴대폰 메세지 보내기 성공!" )
		<< BUILD_LOG( kPacket_.m_wstrSMSMessage );
	return;

end_proc:
	START_LOG( cerr, L"휴대폰 메세지 보내기 실패!" )
		<< BUILD_LOG( kPacket_.m_wstrSMSMessage );
}

//{{ 2010. 11. 2	최육사	서버간 접속 모니터링
#ifdef SERV_MORNITORING

_IMPL_ON_FUNC( DBE_SEND_PHONE_MSG_FOR_MORNITORING_REQ, KDBE_SEND_PHONE_MSG_NOT )
{
	BOOST_TEST_FOREACH( const std::wstring&, wstrPhoneNum, kPacket_.m_vecPhoneNum )
	{
        std::wstring wstrProject = L"ES";
        std::wstring wstrNation = KSimLayer::GetKObj()->GetAuthTypeStr( KSimLayer::GetKObj()->GetAuthFlag() );
        DO_QUERY_NO_PROFILE( L"exec SMS.dbo.SMS_Send", L"N\'%s\', N\'%s\',N\'%s\',N\'%s\',N\'%s\',%d, %d", 
            % wstrPhoneNum 
            % wstrPhoneNum
            % kPacket_.m_wstrSMSMessage
            % wstrProject
            % wstrNation
            % 0
            % 0 
            );
	}

	START_LOG( cout2, L"휴대폰 메세지 보내기 성공!" )
		<< BUILD_LOG( kPacket_.m_wstrSMSMessage );
	return;

end_proc:
	START_LOG( cerr, L"휴대폰 메세지 보내기 실패!" )
		<< BUILD_LOG( kPacket_.m_wstrSMSMessage );

	SendToServer( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, kPacket_ );
}

#endif SERV_MORNITORING
//}}


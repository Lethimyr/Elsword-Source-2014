#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "ChannelSMSDBThread.h"
#include "ChannelServer.h"
#include "NetError.h"

//ImplementDBThread( KChannelSMSDBThread );

#define CLASS_TYPE KChannelSMSDBThread

KChannelSMSDBThread::~KChannelSMSDBThread(void)
{
}

void KChannelSMSDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{    
		CASE( DBE_SEND_PHONE_MSG_NOT );


	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
	}
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

		DO_QUERY_NO_PROFILE( L"INSERT INTO SC_TRAN ( TR_SENDDATE, TR_SENDSTAT, TR_RSLTSTAT, TR_PHONE, TR_CALLBACK, TR_MSG )",
			L"values ( GetDate(),'0', '00', N\'%s\', N\'%s\', N\'%s\' )",
			% wstrPhoneNum
			% wstrPhoneNum
			% kPacket_.m_wstrSMSMessage
			);
	}

	START_LOG( cout2, L"휴대폰 메세지 보내기 성공!" )
		<< BUILD_LOG( kPacket_.m_wstrSMSMessage );
	return;

end_proc:
	START_LOG( cerr, L"휴대폰 메세지 보내기 실패!" )
		<< BUILD_LOG( kPacket_.m_wstrSMSMessage );
}




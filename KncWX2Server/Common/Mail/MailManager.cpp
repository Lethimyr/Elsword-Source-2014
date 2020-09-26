#include "MailManager.h"
#include "KncSend.h"
#include "BaseServer.h"



//#ifdef SERV_MAIL_SYSTEM


//////////////////////////////////////////////////////////////////////////
ImplPfID( KMailEventProcess, PI_NULL );

#define CLASS_TYPE  KMailEventProcess

KMailEventProcess::KMailEventProcess()
{
}

KMailEventProcess::~KMailEventProcess()
{
}

void KMailEventProcess::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
		CASE( EMAIL_SEND_EMAIL_NOT );

	default:
		START_LOG( cerr, L"핸들러가 지정되지 않은 이벤트." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"이벤트 처리 소요 시간." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}
}

IMPL_ON_FUNC( EMAIL_SEND_EMAIL_NOT )
{
	CX2SMTPMail	localSMTP;
	//////////////////////////////////////////////////////////////////////////
	// 초기화
	localSMTP.SMTPSetServer("mail.kog.co.kr",25);
#ifdef SERV_INT_ONLY
	localSMTP.SMTPSetLogin("testesint");            // 계정 인증
	localSMTP.SMTPSetPassword("ElsInt@@201216");
#else //SERV_INT_ONLY
	localSMTP.SMTPSetLogin("escrash");            // 계정 인증
	localSMTP.SMTPSetPassword("@Els.123");
#endif //SERV_INT_ONLY	

	localSMTP.SMTPSetSenderName( "엘소드 서버 접속 감시자" );

#ifdef SERV_INT_ONLY
	localSMTP.SMTPSetSenderMail( "testesint@kog.co.kr" );
	localSMTP.SMTPSetReplyTo( "testesint@kog.co.kr" ); // 회신 주소
#else //SERV_INT_ONLY
	localSMTP.SMTPSetSenderMail( "escrash@kog.co.kr" );
	localSMTP.SMTPSetReplyTo( "escrash@kog.co.kr" ); // 회신 주소
#endif //SERV_INT_ONLY	


	//////////////////////////////////////////////////////////////////////////
	// 제목
	{
		localSMTP.SMTPSetSubject( KncUtil::toNarrowString( kPacket_.m_wstrTitle ).c_str() );
	}
	
	//////////////////////////////////////////////////////////////////////////	
	// 받는 사람들
	{
		std::vector< std::wstring >::const_iterator vit;
		for( vit = kPacket_.m_vecRecvMailAddrList.begin(); vit != kPacket_.m_vecRecvMailAddrList.end(); ++vit )
		{
			localSMTP.SMTPAddRecipient( KncUtil::toNarrowString( *vit ).c_str() );
		}
	}	

	localSMTP.SMTPSetXPriority( CX2SMTPMail::XPRIORITY_NORMAL );
	localSMTP.SMTPSetXMailer("The Bat! (v3.02) Professional");

	//////////////////////////////////////////////////////////////////////////
	// 본문 내용
	{
		localSMTP.SMTPSetMessageBody( KncUtil::toNarrowString( kPacket_.m_wstrDesc ).c_str() );
	}	

	//////////////////////////////////////////////////////////////////////////
	// 파일 이름으로 첨부 파일 찾기
	{
		BOOST_TEST_FOREACH( const std::wstring&, wstrFileName, kPacket_.m_vecAttachFileNameList )
		{
			char szFilePath[1024] = "";
			GetCurrentDirectoryA( _MAX_PATH, szFilePath );
			std::string strFileName = szFilePath;
			strFileName += "\\";
			strFileName += KncUtil::toNarrowString( wstrFileName ).c_str();

			localSMTP.SMTPAddAttachment( strFileName.c_str() );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 절대 경로로 첨부 파일 찾기
	{
		BOOST_TEST_FOREACH( const std::wstring&, wstrFileFullPath, kPacket_.m_vecAttachFileFullPathList )
		{
			const std::string strFileName = KncUtil::toNarrowString( wstrFileFullPath );
			localSMTP.SMTPAddAttachment( strFileName.c_str() );
		}
	}

#ifdef SERV_FIX_SCRIPT_PARSING_ERR_REPORT_ONLY_INTERNAL
	if( localSMTP.SMTPSend() )
	{
		//The mail was send successfully
		START_LOG( cout, L"메일 전송 성공!" )
			<< BUILD_LOG( kPacket_.m_wstrTitle )
			<< BUILD_LOG( kPacket_.m_vecRecvMailAddrList.size() )
			<< BUILD_LOG( kPacket_.m_vecAttachFileNameList.size() )
			<< BUILD_LOG( kPacket_.m_vecAttachFileFullPathList.size() );
	}
	else
	{
		//Unable to send the mail
		START_LOG( cout, L"메일 전송 실패!" )
			<< BUILD_LOG( kPacket_.m_wstrTitle )
			<< BUILD_LOG( kPacket_.m_vecRecvMailAddrList.size() )
			<< BUILD_LOG( kPacket_.m_vecAttachFileNameList.size() )
			<< BUILD_LOG( kPacket_.m_vecAttachFileFullPathList.size() );
	}
#endif //SERV_FIX_SCRIPT_PARSING_ERR_REPORT_ONLY_INTERNAL

	// 받는사람 주소, 첨부파일 초기화
	localSMTP.SMTPClearRecipient();
}



//////////////////////////////////////////////////////////////////////////
ImplToStringW( KMailManager )
{
    return stm_;
}

KMailManager::KMailManager()
{
}

KMailManager::~KMailManager()
{
}

void KMailManager::Run()
{
	START_LOG( clog, L"Enter Run()" );

	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 5000 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else    START_LOG( cout, L"*** WaitForSingleObject() - return : " << ret );
	}
}

void KMailManager::Init()
{
	START_LOG( cout, L"Mail Manager Manager 초기화 완료! thread시작!" );

	// thread시작
	Begin();
}

void KMailManager::Shutdown()
{
	// thread정지
	End();
}

void KMailManager::Tick()
{
	m_kMailEventProcess.Tick();
}

void KMailManager::_QueueingEvent( const KEventPtr& spEvent_ )
{
	m_kMailEventProcess.QueueingEvent( spEvent_ );
}

//#endif SERV_MAIL_SYSTEM



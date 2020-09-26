#include "StdAfx.h"
#include "X2MailLog.h"

#ifdef NEW_MAIL_LOG

CX2MailLogManager* CX2MailLogManager::m_pMailLog = NULL;
/*static */CX2MailLogManager* CX2MailLogManager::GetInstance()
{
	if( NULL == m_pMailLog)
		m_pMailLog = new CX2MailLogManager;

	return m_pMailLog;
}

/*static*/ void CX2MailLogManager::ResetMailLogManager()
{
	DestroyInstance();
}

/*static*/ void CX2MailLogManager::DestroyInstance()
{
	SAFE_DELETE( m_pMailLog );
}

CX2MailLogManager::CX2MailLogManager() :
m_pX2SMTPMail( NULL ),
m_bIsSending(false)
{

}

CX2MailLogManager::~CX2MailLogManager()
{
}

/** @function : SendMail
	@brief :메일 보내는 기능 
			CX2Main::SendCheckMail() 떙겨왔음. 
*/
void CX2MailLogManager::SendMail()
{
	if( true == m_bIsSending )
		return;

	if ( NULL == m_pX2SMTPMail )
		m_pX2SMTPMail = new CX2SMTPMail;

	if(m_pX2SMTPMail->SMTPGetLastError() != CX2SMTPMail::CSMTP_NO_ERROR)
	{
		return;
	}

	m_pX2SMTPMail->SMTPSetServer( "smtp.kog.co.kr", 25 );
	m_pX2SMTPMail->SMTPSetLogin("ElswordKorea@smtp.kog.co.kr");            // 계정 인증
	m_pX2SMTPMail->SMTPSetPassword("rhfqoddl@2013");
	m_pX2SMTPMail->SMTPSetSenderName( "KOG_ELSWORD" );
	m_pX2SMTPMail->SMTPSetSenderMail("ElswordKorea@smtp.kog.co.kr");
	m_pX2SMTPMail->SMTPSetReplyTo("ElswordKorea@kog.co.kr"); // 회신 주소

	m_pX2SMTPMail->SMTPAddRecipient( "itexpertkim@kog.co.kr" );
	m_pX2SMTPMail->SMTPAddRecipient( "hboh88@kog.co.kr" );
	m_pX2SMTPMail->SMTPAddRecipient( "charses@kog.co.kr" );
	m_pX2SMTPMail->SMTPSetXPriority(CX2SMTPMail::XPRIORITY_NORMAL);
	m_pX2SMTPMail->SMTPSetXMailer("The Bat! (v3.02) Professional");

	std::map< MAIL_LOG_ID, string >::iterator it = m_mapLogList.begin();
	for( ; it != m_mapLogList.end(); ++it)
	{
#ifdef  X2OPTIMIZE_IGNORE_SERVER_TIME_OUT_LOG
        if ( it->first == MLI_SERVER_TIME_OUT )
        {
            continue;
        }
#endif  X2OPTIMIZE_IGNORE_SERVER_TIME_OUT_LOG
		m_pX2SMTPMail->SMTPSetSubject( GetSubjectString( it->first ).c_str() );

#if defined( _IN_HOUSE_ )
		it->second += "\n사내 서버 ";
#elif defined( _IN_HOUSE_SERVICE_READY_QA_ )
		it->second += "\nQA 서버 ";
#elif defined( _SERVICE_ ) && defined( _OPEN_TEST_ )
		it->second += "\n오픈테스트 서버 ";
#elif defined( _SERVICE_ )
		it->second += "\n라이브 서버 ";
#endif

		m_pX2SMTPMail->SMTPSetMessageBody( it->second.c_str() );
		if( m_pX2SMTPMail->SMTPSend() )
		{
			m_bIsSending = true;
			//The mail was send successfully
		}
		else
		{

			//Unable to send the mail
		}
	}


	m_pX2SMTPMail->SMTPClearRecipient();
}
string CX2MailLogManager::GetSubjectString( MAIL_LOG_ID eMailLogID_ )
{
	string strSubject = "[ES_KR_CLIENT_LOG_MAIL]_";
	switch( eMailLogID_ )
	{
	case MLI_CLOSE_PARTY_BUG :			 strSubject += "MLI_CLOSE_PARTY_BUG"; break;
	case MLI_MASS_FILE_DUPLICATION:		 strSubject += "MLI_MASS_FILE_DUPLICATION"; break;
	case MLI_USE_GAME_EDIT:				 strSubject += "MLI_USE_GAME_EDIT"; break;
	case MLI_SERVER_TIME_OUT:			 strSubject += "MLI_SERVER_TIME_OUT_TEST"; break;
	case MLI_SEARCH_CHECK_KOM_FILTER:	 strSubject += "MLI_SEARCH_CHECK_KOM_FILTER"; break;
	default:
		{
			strSubject += "UNKNOWN";
		} break;
	}

	return strSubject.c_str();
}
void CX2MailLogManager::AddMailLog( MAIL_LOG_ID eMailLogID, const CHAR* pLog )
{
	std::map< MAIL_LOG_ID, string >::iterator it = m_mapLogList.find ( eMailLogID );
	if( it != m_mapLogList.end() )
	{
		it->second += "\n";
		it->second += pLog;
	}
	else
	{
		m_mapLogList.insert( std::make_pair( eMailLogID, pLog ));
	}
}
#endif // NEW_MAIL_LOG



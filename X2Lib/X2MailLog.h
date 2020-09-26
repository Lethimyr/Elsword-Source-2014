#pragma once

/* 오현빈 // 2013-08-09
*/
#ifdef NEW_MAIL_LOG
class CX2MailLogManager
{
public:
	enum MAIL_LOG_ID
	{
		MLI_CLOSE_PARTY_BUG = 0,     // 정상적이지 않은 파티 탈퇴가 이루어졌을 때 메일 발송
		MLI_MASS_FILE_DUPLICATION,	 // 리소스 중복 파일로 인해 게임 종료 됐을 때 메일 발송
		MLI_USE_GAME_EDIT,			 // 본섭에서 치트 사용 시 메일 발송
		MLI_SERVER_TIME_OUT,		 // 패킷 응답 받지 못했을 때 로그
		MLI_SEARCH_CHECK_KOM_FILTER, // 비정상적인 Kom파일 발견됐을 때 메일 발송
	};

public:
	static CX2MailLogManager* GetInstance();
	static void ResetMailLogManager();
	static void DestroyInstance();

	void AddMailLog( MAIL_LOG_ID eMailLogID, const WCHAR* pLog )
	{
		std::string strLog;
		ConvertWCHARToChar( strLog, pLog );
		return AddMailLog( eMailLogID, strLog.c_str() );
	}
	void AddMailLog( MAIL_LOG_ID eMailLogID, const CHAR* pLog );
	void SendMail();

private:
	string GetSubjectString( MAIL_LOG_ID eMailLogID_ );
private:
	CX2MailLogManager();
	~CX2MailLogManager();
	
	static CX2MailLogManager* m_pMailLog;

	CX2SMTPMail* m_pX2SMTPMail;

	// MAIL_LOG_ID, 로그 문자열
	std::map< MAIL_LOG_ID, string > m_mapLogList;

	bool m_bIsSending;  // 메일 전송 여부
};

#endif // NEW_MAIL_LOG


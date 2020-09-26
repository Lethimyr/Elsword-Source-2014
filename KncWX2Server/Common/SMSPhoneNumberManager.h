#pragma once

#include "RefreshSingleton.h"
#include <vector>
#include "CommonPacket.h"


//{{ 2012. 09. 26	최육사		우편 복사 버그 수정
#ifdef SERV_POST_COPY_BUG_FIX


class KSMSPhoneNumberManager
{
	DeclareRefreshSingleton( KSMSPhoneNumberManager );
	DeclareLuaScriptParser;		
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	KSMSPhoneNumberManager(void);
	~KSMSPhoneNumberManager(void);	

	enum PHONE_NUMBER_LIST_TYPE
	{
		PNLT_ELSWORD_DEV_TEAM			= 0,
		//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
//#ifdef SERV_SMS_TOTAL_MANAGER
		PNLT_ELSWORD_QA_TEAM			= 1,
		PNLT_ELSWORD_MOR_TEAM			= 2,
		PNLT_NEXON						= 3,
//#endif SERV_SMS_TOTAL_MANAGER
		//}}

//#ifdef SERV_MORNITORING_ABUSER_NOTIFY_SMS// 작업날짜: 2013-05-22	// 박세훈
		PNLT_ELSWORD_DEV_TEAM_SERVER	= 4,
//#endif // SERV_MORNITORING_ABUSER_NOTIFY_SMS

		PNLT_MAX,
	};

	//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	enum FUNTIONAL_SET
	{
		FS_TEST									= 0,
		FS_MINUS_ED								= 1,
		FS_SEND_A_LETTER_TO_SAME_NICKNAME		= 2,
		FS_INCORRECT_USER_UNIT					= 3,
		FS_CHECK_AUTH_LEVEL						= 4,
		FS_SURVEILLANCE_THE_LETTER_AND_TRADE	= 5,
		FS_PERSONAL_SHOP_ITEM					= 6,
		FS_AUTO_HACK_CHECK_GET_ITEM				= 7,
		FS_DUNGEON_CLEAR_TIME_HACK_USER_CHECK	= 8,
		FS_DB_HACKING_ED_UPDATE_TRAP			= 9,
		FS_CHECK_DROP_CCU						= 10,
		FS_SERVER_STATE							= 11,
		FS_EVENT_JACK_POT						= 12,
		FS_LOCAL_RANKING_SYSTEM					= 13,

//#ifdef SERV_MORNITORING_ABUSER_NOTIFY_SMS// 작업날짜: 2013-05-22	// 박세훈
		FS_MORNITORING_ABUSER_NOTIFY			= 14,
//#endif // SERV_MORNITORING_ABUSER_NOTIFY_SMS
		FS_MAX,
	};
#endif SERV_SMS_TOTAL_MANAGER
	//}}

public:

	//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	void AddFuntionalPhoneNumber_LUA( IN int iType, const char* pPhoneNum );
	void AddTeamPhoneNumber_LUA( IN int iType, const char* pPhoneNum );

	void Link_LUA( IN int iType, IN PHONE_NUMBER_LIST_TYPE ePNLTType );

	void GetPhoneNumberList( IN const FUNTIONAL_SET eType, OUT std::vector< std::wstring >& vecPhoneNumList ) const;
#else
	// for lua
	void AddPhoneNumber_LUA( int iType, const char* pPhoneNum );

	// function
	void GetPhoneNumberList( IN const PHONE_NUMBER_LIST_TYPE eType, OUT std::vector< std::wstring >& vecPhoneNumList ) const;
#endif SERV_SMS_TOTAL_MANAGER
	//}}

private:
	//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	std::vector< std::wstring >				m_vecFuntionalPhoneNumList[FS_MAX];
	std::vector< std::wstring >				m_vecTeamPhoneNumList[PNLT_MAX];
	std::vector< PHONE_NUMBER_LIST_TYPE >	m_vecFuntionalTeamList[FS_MAX];
#else
	std::vector< std::wstring >				m_vecPhoneNumList[PNLT_MAX];
#endif SERV_SMS_TOTAL_MANAGER
	//}}
};

DefRefreshSingletonInline( KSMSPhoneNumberManager );


#endif SERV_POST_COPY_BUG_FIX
//}}
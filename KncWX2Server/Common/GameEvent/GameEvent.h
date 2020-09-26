#pragma once 

#include <string>
#include <vector>
#include <map>
#include "Event.h"

#define VARIFY_ENABLE( return_code ) \
	if( IsEnable() == false ) \
{ \
	return_code; \
} \

class KGameEvent
{
public:
	enum GAME_EVENT_TYPE
	{
		GET_NONE				= 0,

		GET_QUEST				= 1,	// [1]  퀘스트 이벤트
		GET_ITEM_DROP			= 2,	// [2]  던전 아이템 드롭률 이벤트
		GET_REWARD				= 3,	// [3]  경험치&ED&AP&RP 이벤트
		GET_MONSTER				= 4,	// [4]  몬스터 출현 이벤트
		GET_TOURNAMENT			= 5,	// [5]  대전대회용 입장체크 이벤트
		GET_SPIRIT				= 6,	// [6]  근성도 이벤트
        GET_CONNECT_TIME		= 7,	// [7]  접속 시간 이벤트
		GET_SECRET_DUNGEON		= 8,	// [8]  비밀 던전 이벤트
		GET_NOTIFY_MSG			= 9,	// [9]  공지사항 이벤트
		GET_CUMULATIVE_TIME		= 10,	// [10] 누적 시간 이벤트
		GET_PURCHASE_LIMITED	= 11,	// [11] 해외 빌링 구매 제한
		//{{ 2011. 08. 12   김민성      헤니르 개편 
//#ifdef SERV_NEW_HENIR_TEST
		GET_HENIR_REWARD		= 12,	// [12] 헤니르 보상 지급 회수
//#endif SERV_NEW_HENIR_TEST
		//}}
//#ifdef SERV_OPTION_EVENT_SYSTEM
		GET_OPTION				= 13,	// [13] 랜덤 소켓 옵션 버프
//#endif SERV_OPTION_EVENT_SYSTEM

//#ifdef SERV_ADVERTISEMENT_EVENT
		GET_ADVERTISEMENT_EVENT	= 14,	// [14] URL 인게임 광고
//#endif SERV_ADVERTISEMENT_EVENT

//#ifdef SERV_NEW_EVENT_TYPES
		GET_DUNGEON				= 15,	// [15] 던전
		GET_MAX_LEVEL			= 16,	// [16] 레벨
		GET_CONTENTS			= 17,	// [17] 컨텐츠 묶음
//#endif SERV_NEW_EVENT_TYPES

//#ifdef SERV_CODE_EVENT
		GET_CODE				= 18,	// [18] 코드on/off
//#endif SERV_CODE_EVENT
//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
//#ifdef SERV_QUEST_ITEM_DROP_EVENT
		GET_QUEST_ITEM_DROP		= 19,	// [19] 퀘스트 아이템 드롭률 이벤트
//#endif SERV_QUEST_ITEM_DROP_EVENT
//}
//#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// 적용날짜: 2013-04-11
		GET_DEFENSE_DUNGEON		= 20,	// [20] 어둠의 문 오픈 확률 이벤트
//#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

//#ifdef SERV_LEVEL_UP_EVENT// 작업날짜: 2013-06-24	// 박세훈
		GET_LEVEL_UP			= 21,	// [21] 레벨 업 이벤트
//#endif // SERV_LEVEL_UP_EVENT

//#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-09	// 박세훈
		GET_JUMPING_CHARACTER	= 22,	// [22] 점핑 캐릭터 이벤트
//#endif // SERV_JUMPING_CHARACTER

		GET_MAX,
	};

public:
	KGameEvent();
	virtual ~KGameEvent();

	void			SetUID( int iUID )				{ m_iUID = iUID; }
	int				GetUID() const					{ return m_iUID; }

	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
	void			SetScriptID( int iScriptID )	{ m_iScriptID = iScriptID; }
	int				GetScriptID() const				{ return m_iScriptID; }
	//}}

	void			SetType( GAME_EVENT_TYPE eType ){ m_eType = eType; }
	GAME_EVENT_TYPE	GetType() const					{ return m_eType; }

	void			SetEnable( bool bEnable )		{ m_bEnable = bEnable; }
	bool			IsEnable() const				{ return m_bEnable; }

#ifdef SERV_ALLOW_EVENT_ERASE
	void			SetOn( bool bOn )				{ m_bOn = bOn; }
	bool			IsOn() const					{ return m_bOn; }
#endif SERV_ALLOW_EVENT_ERASE

	void			SetEventName( std::wstring wstrEventName ){ m_wstrEventName = wstrEventName; }
	const std::wstring&	GetEventName() const		{ return m_wstrEventName; }

	void			SetBeginDate( CTime& ctBeginDate ){ m_BeginDate = ctBeginDate; }
	void			SetEndDate( CTime& ctEndDate ){ m_EndDate = ctEndDate; }
	const CTime&	GetBeginDate() const			{ return m_BeginDate; }
	const CTime&	GetEndDate() const				{ return m_EndDate; }

protected:
	int					m_iUID;
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
	int					m_iScriptID;
	//}}
	GAME_EVENT_TYPE		m_eType;
	bool				m_bEnable;

	std::wstring		m_wstrEventName;

	CTime				m_BeginDate;
	CTime				m_EndDate;	

#ifdef SERV_ADVERTISEMENT_EVENT
	std::wstring		m_wstrAdvertisementURL;
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_ALLOW_EVENT_ERASE
	bool				m_bOn;
#endif SERV_ALLOW_EVENT_ERASE
};




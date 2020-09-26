#pragma once

#include <ToString.h>
#include <KNCSingleton.h>
#include <Windows.h>
#include "ClientPacket.h"

//##des :
//## 1 : 스승이 없는 유저리스트를 유지
//## 2 : 같은 게임서버에 있는 제자유저에게 패킷을 전달
//위의 기능을 제공하기 위한 클래스..

class KTutorialManager
{
	DeclareSingleton( KTutorialManager );
	DeclToStringW;
	DeclDump;

public:
	enum
	{
		UNIT_LEVEL_STUDENT_MAX = 10,
		UNIT_LEVEL_TEACHER_MIN = 20,
		STUDENT_DISCONNECT_DAY = 7,
		STUDENT_PAGE_NUM = 5,
		STUDENT_NUM = 3,
		TEACHER_NUM = 1,
	};

	enum TUTORIAL_DEL_REASON
	{
		TDR_DISCONNECT_DAY = 0,
		TDR_LEVEL_UP_END,	//졸업을 한경우.
		TDR_DELETE_UNIT,	//유닛을 삭제한 경우
		TDR_REFUSAL_USER,	//유저가 삭제를 요청한 경우

		TDR_TOTAL_NUM,
	};

	//{{ 2008. 9. 8  최육사		제자 졸업 보상
	enum TUTOR_REWARD_ID
	{
        TRI_GRADUATION = 6,
	};
	//}}

public:
	~KTutorialManager(void);

	bool	IsExistbylist( UidType iUnitUID );
	
	bool	AddUnit( KTutorialUnitInfo& kInfo );	//레벨 10이하인 유저가 접속할때 추가..
	bool	DeleteUnit( UidType iUnitUID );

	bool	GetTutorPageInfo( IN KEGS_TUTORIAL_STUDENT_LIST_REQ& kReq, OUT KEGS_TUTORIAL_STUDENT_LIST_ACK& kAck );
	void	SetLevelData( UidType iUnitUID, u_char ucLevel );

	//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	int GetDelReason( TUTORIAL_DEL_REASON eReason );
#else SERV_NETERROR_STR_GET_FROM_CLIENT
	std::wstring GetDelReason( TUTORIAL_DEL_REASON eReason );	
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}

private:
	KTutorialManager(void);

	//std::map<UidType,KTutorialUnitInfo>		m_mapUnitInfo;	//10레벨이하의 유닛리스트.
	std::vector<KTutorialUnitInfo>			m_vecUnitInfo;	//페이지용.

	//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	int m_iDelReason[TDR_TOTAL_NUM];	
#else SERV_NETERROR_STR_GET_FROM_CLIENT
	std::wstring	m_wstrDelReason[TDR_TOTAL_NUM];
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}
};

DefSingletonInline( KTutorialManager );

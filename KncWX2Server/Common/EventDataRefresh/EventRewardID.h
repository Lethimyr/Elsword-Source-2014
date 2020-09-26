#pragma once

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB

//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
enum EVENT_REWARD_ID
{
	ERI_SECOND_SECURITY = 10135,       // 2차 보안 시스템 일일 보상

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	ERI_COME_BACK_USER_TITLE				= 20150,	// 타이틀ID  - 다시 깨어난 자(휴면유저 칭호)
	ERI_COME_BACK_USER_ITEM_CUBE_ONE		= 10143,	// reward
	ERI_COME_BACK_USER_ITEM_CUBE_TWO		= 10144,	// reward
	ERI_COME_BACK_USER_ITEM_CUBE_THREE		= 10145,	// reward
#endif SERV_COME_BACK_USER_REWARD
	//}} 
};
#endif SERV_SECOND_SECURITY
//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

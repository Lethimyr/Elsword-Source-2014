#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif
	// 통신 방식
	// Collect Server <-> Client
	_ENUM( ECL_HEART_BEAT = 0 )
	_ENUM( ECL_ACCEPT_CONNECTION_NOT )    // server -> client connect 요청을 받은 곳에 새로운 보안키를 전달.
	_ENUM( ECL_CONNECTION_LOST_NOT )
	_ENUM( ECL_UDP_PORT_NOT )             // server -> client UDP 패킷에 찍힌 포트번호를 TCP로 알려줌.
	_ENUM( ECL_DISABLE_HB_CHECK_REQ )
	_ENUM( ECL_LOG_NOT )
	_ENUM( ECL_RESERVE_DESTROY )

	//{{ 2009. 12. 16  최육사	동접툴
	_ENUM( ECL_TOOL_GET_CCU_INFO_REQ )
	_ENUM( ECL_TOOL_GET_CCU_INFO_ACK )
	//}}

	//{{ 2011. 01. 13 김민성	동접 모니터링 툴	
	_ENUM( ECL_TOOL_CHECK_LOGIN_REQ )
	_ENUM( ECL_TOOL_CHECK_LOGIN_ACK )

	_ENUM( ECL_TOOL_SERVER_LIST_REQ )
	_ENUM( ECL_TOOL_SERVER_LIST_ACK )
	//}}

	_ENUM( ECL_CHECK_SEQUENCE_COUNT_NOT )
	
	_ENUM( ECL_SYSTEM_EVENT_ID_END )
	//////////////////////////////////////////////////////////////////////////
	
	// 스트링 정보 전송용
	_ENUM( ECL_CRASH_INFO_REQ )		// Cilent -> Server
	_ENUM( ECL_CRASH_INFO_ACK )		// Server -> Cilent

	// 스샷 전송용
	_ENUM( ECL_CRASH_IMAGE_INFO_REQ )		// Cilent -> Server
	_ENUM( ECL_CRASH_IMAGE_INFO_ACK )		// Server -> Cilent

	_ENUM( ECL_DISCONNECT_REQ )		// Cilent -> Server
	_ENUM( ECL_DISCONNECT_ACK )		// Server -> Cilent





	//패킷의 끝
	_ENUM( COLLECT_EVENTID_SENTINEL ) //패킷의 끝


#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif

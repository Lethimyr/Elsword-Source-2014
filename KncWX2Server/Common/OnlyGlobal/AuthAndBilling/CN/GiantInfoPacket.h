#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"
#include "GiantCommonPacket.h"

SmartPointer( KGiantInfoPacket );

class KGiantInfoPacket : public KGiantCommonPacket
{
public:
	enum ENUM_INFO_COMMAND_TYPE { 
		GIP_CT_INFOSERVER				= 2,
		GIP_CT_GMTOOL					= 128,
	};

	enum ENUM_INFO_PARACOMMAND_TYPE { 
		GI_PCT_CCU_REQ					= 3,
		GI_PCT_TOTAL_CCU_ACK			= 4,
		GI_PCT_CHANNEL_CCU_ACK			= 5,
		GI_PCT_REAL_CCU_ACK				= 6,

		GI_PCT_CHAT						= 1,
		GI_PCT_PUNISH					= 4,
		GI_PCT_BROADCAST				= 5,
#ifdef SERV_INFOSERVER_ADD_WORK
		GI_PCT_USER_ON_LINE				= 11,
		GI_PCT_REQUEST_ID				= 13,
#endif //SERV_INFOSERVER_ADD_WORK
	};

public:
	KGiantInfoPacket();
	virtual ~KGiantInfoPacket();

	// 상속에 따른 오버로딩 재정의
	using KGiantCommonPacket::Read;
	using KGiantCommonPacket::Write;

	bool Read( KEGIANT_INFO_GET_CCU_REQ& kPacket );
	bool Write( const KEGIANT_INFO_GET_CCU_REQ& kPacket );
	bool Read( KEGIANT_INFO_GET_TOTAL_CCU_ACK& kPacket );
	bool Write( const KEGIANT_INFO_GET_TOTAL_CCU_ACK& kPacket );
	bool Read( KEGIANT_INFO_GET_CHANNEL_CCU_ACK& kPacket );
	bool Write( const KEGIANT_INFO_GET_CHANNEL_CCU_ACK& kPacket );
	bool Read( KEGIANT_INFO_CHAT_NOT& kPacket );
	bool Write( const KEGIANT_INFO_CHAT_NOT& kPacket );
	bool Read( KEGIANT_INFO_PUNISH_REQ& kPacket );
	bool Write( const KEGIANT_INFO_PUNISH_REQ& kPacket );
#ifdef SERV_INFOSERVER_ADD_WORK
	bool Read( KEGIANT_INFO_USERONLINE_REQ& kPacket );
	bool Write( const KEGIANT_INFO_USERONLINE_REQ& kPacket );
	bool Read( KEGIANT_INFO_REQUESTID_REQ& kPacket );
	bool Write( const KEGIANT_INFO_REQUESTID_REQ& kPacket );
#endif //SERV_INFOSERVER_ADD_WORK
};
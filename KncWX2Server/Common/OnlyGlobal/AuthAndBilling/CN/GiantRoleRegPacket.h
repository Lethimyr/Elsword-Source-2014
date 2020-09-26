#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"
#include "GiantCommonPacket.h"

SmartPointer( KGiantRoleRegPacket );

class KGiantRoleRegPacket : public KGiantCommonPacket
{
public:
	enum ENUM_ROLEREG_COMMAND_TYPE { 
		GRP_RCT_NAME					= 3,
	};

	enum ENUM_ROLEREG_PARACOMMAND_TYPE { 
		GR_PCT_NAME						= 1,
	};

	enum ENUM_ROLEREG_REG_TYPE {
		GR_RT_UNIT						= 1,
		GR_RT_GUILD						= 3,
	};

	enum ENUM_ROLEREG_STATE_TYPE_MASK {
		GR_ST_MASK_EXIST_CHECK			= 1,		// Deprecated. Reserved.
		GR_ST_MASK_INSERT_REQ			= 2,
		GR_ST_MASK_DELETE_REQ			= 4,
		GR_ST_MASK_EXIST				= 8,
		GR_ST_MASK_SUCCESS				= 16,
		GR_ST_MASK_ILLEGAL				= 32,		// Deprecated. Reserved.
	};

public:
	KGiantRoleRegPacket();
	virtual ~KGiantRoleRegPacket();

	// 상속에 따른 오버로딩 재정의
	using KGiantCommonPacket::Read;
	using KGiantCommonPacket::Write;

	virtual bool Read( KEGIANT_ROLEREG& kPacket );
	virtual bool Write( const KEGIANT_ROLEREG& kPacket );
};
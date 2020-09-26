#include "GiantRoleRegPacket.h"

KGiantRoleRegPacket::KGiantRoleRegPacket()
: KGiantCommonPacket()
{
}

KGiantRoleRegPacket::~KGiantRoleRegPacket()
{
}

bool KGiantRoleRegPacket::Read( KEGIANT_ROLEREG& kPacket )
{
	int iCP = 0;
	_JIF( Read2Byte( kPacket.m_usRegType, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usServerID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrName, 32, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiRoleUID, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usState, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantRoleRegPacket::Write( const KEGIANT_ROLEREG& kPacket )
{
	m_byteCommand = GRP_RCT_NAME;
	m_byteParaCommand = GR_PCT_NAME;

	int iCP = 0;
	_JIF( Write2Byte( kPacket.m_usRegType, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usServerID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrName, 32, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiRoleUID, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usState, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}
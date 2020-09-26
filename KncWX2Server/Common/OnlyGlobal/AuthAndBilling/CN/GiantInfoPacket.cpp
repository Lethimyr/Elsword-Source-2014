#include "GiantInfoPacket.h"

#include <boost/foreach.hpp>

KGiantInfoPacket::KGiantInfoPacket()
: KGiantCommonPacket()
{
}

KGiantInfoPacket::~KGiantInfoPacket()
{
}

bool KGiantInfoPacket::Read( KEGIANT_INFO_GET_CCU_REQ& kPacket )
{
	int iCP = 0;
	_JIF( Read8Byte( kPacket.m_TimeStamp, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantInfoPacket::Write( const KEGIANT_INFO_GET_CCU_REQ& kPacket )
{
	m_byteCommand = GIP_CT_INFOSERVER;
	m_byteParaCommand = GI_PCT_CCU_REQ;

	int iCP = 0;
	_JIF( Write8Byte( kPacket.m_TimeStamp, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantInfoPacket::Read( KEGIANT_INFO_GET_TOTAL_CCU_ACK& kPacket )
{
	int iCP = 0;
	_JIF( Read8Byte( kPacket.m_TimeStamp, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usServerID, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usServerType, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( ReadString( kPacket.m_strZoneName, 32, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiCCU, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantInfoPacket::Write( const KEGIANT_INFO_GET_TOTAL_CCU_ACK& kPacket )
{
	m_byteCommand = GIP_CT_INFOSERVER;
	m_byteParaCommand = GI_PCT_TOTAL_CCU_ACK;

	int iCP = 0;
	_JIF( Write8Byte( kPacket.m_TimeStamp, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usServerID, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usServerType, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( WriteString( kPacket.m_strZoneName, 32, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiCCU, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantInfoPacket::Read( KEGIANT_INFO_GET_CHANNEL_CCU_ACK& kPacket )
{
	int iCP = 0;
	_JIF( Read8Byte( kPacket.m_TimeStamp, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usGame, iCP ), return false );
//	_JIF( Read4Byte( kPacket.m_uiCountryNum, iCP ), return false );
//	_JIF( Read4Byte( kPacket.m_uiCountryID, iCP ), return false );
//	_JIF( Read4Byte( kPacket.m_uiCountryCCU, iCP ), return false );
	int size;
	_JIF( Read4Byte( size, iCP ), return false );
	for(int i=0; i<size; ++i)
	{
		UINT uiCountryID;
		UINT uiCountryCCU;
		_JIF( Read4Byte( uiCountryID, iCP ), return false );
		_JIF( Read4Byte( uiCountryCCU, iCP ), return false );
		kPacket.m_mapChannelCCU.insert(std::make_pair(static_cast<UidType>(uiCountryID), uiCountryCCU));
	}
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantInfoPacket::Write( const KEGIANT_INFO_GET_CHANNEL_CCU_ACK& kPacket )
{
	typedef std::map<UidType, UINT> CCUMap;

	m_byteCommand = GIP_CT_INFOSERVER;
	m_byteParaCommand = GI_PCT_CHANNEL_CCU_ACK;

	int iCP = 0;
	_JIF( Write8Byte( kPacket.m_TimeStamp, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usGame, iCP ), return false );
	//_JIF( Write4Byte( kPacket.m_uiCountryNum, iCP ), return false );
	//_JIF( Write4Byte( kPacket.m_uiCountryID, iCP ), return false );
	//_JIF( Write4Byte( kPacket.m_uiCountryCCU, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_mapChannelCCU.size(), iCP ), return false );
	BOOST_FOREACH( const CCUMap::value_type& value, kPacket.m_mapChannelCCU )
	{
		_JIF( Write4Byte( static_cast<unsigned long>(value.first), iCP ), return false );
		_JIF( Write4Byte( value.second, iCP ), return false );
	}
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantInfoPacket::Read( KEGIANT_INFO_CHAT_NOT& kPacket )
{
	int iCP = 0;
	_JIF( Read2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserName, 32, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrGMName, 32, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiCountryID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiSceneID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiType, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrContent, 256, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrServer, 32, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantInfoPacket::Write( const KEGIANT_INFO_CHAT_NOT& kPacket )
{
	m_byteCommand = GIP_CT_GMTOOL;
	m_byteParaCommand = GI_PCT_CHAT;

	int iCP = 0;
	_JIF( Write2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserName, 32, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrGMName, 32, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiCountryID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiSceneID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiType, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrContent, 256, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrServer, 32, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantInfoPacket::Read( KEGIANT_INFO_PUNISH_REQ& kPacket )
{
	int iCP = 0;
	_JIF( Read2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserName, 32, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrCountryName, 32, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrServerName, 32, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrGMName, 32, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrReason, 128, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usOperation, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iDelay, iCP ), return false );
	_JIF( Read2Byte( static_cast<short>(kPacket.m_sLevel), iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiStartTime, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantInfoPacket::Write( const KEGIANT_INFO_PUNISH_REQ& kPacket )
{
	m_byteCommand = GIP_CT_GMTOOL;
	m_byteParaCommand = GI_PCT_PUNISH;

	int iCP = 0;
	_JIF( Write2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserName, 32, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrCountryName, 32, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrServerName, 32, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrGMName, 32, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrReason, 128, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usOperation, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iDelay, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_sLevel, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiStartTime, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}
#ifdef SERV_INFOSERVER_ADD_WORK
bool KGiantInfoPacket::Read( KEGIANT_INFO_USERONLINE_REQ& kPacket )
{
	int iCP = 0;
	_JIF( Read2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserName, 32, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrGMName, 32, iCP ), return false );
	_JIF( ReadByte( kPacket.m_bUserOnline, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantInfoPacket::Write( const KEGIANT_INFO_USERONLINE_REQ& kPacket )
{
	m_byteCommand = GIP_CT_GMTOOL;
	m_byteParaCommand = GI_PCT_USER_ON_LINE;

	int iCP = 0;
	_JIF( Write2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserName, 32, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrGMName, 32, iCP ), return false );
	_JIF( WriteByte( kPacket.m_bUserOnline, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantInfoPacket::Read( KEGIANT_INFO_REQUESTID_REQ& kPacket )
{
	int iCP = 0;
	_JIF( Read2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserName, 32, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrGMName, 32, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrAccount, 32, iCP ), return false );
	_JIF( ReadByte( kPacket.m_bWrite, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantInfoPacket::Write( const KEGIANT_INFO_REQUESTID_REQ& kPacket )
{
	m_byteCommand = GIP_CT_GMTOOL;
	m_byteParaCommand = GI_PCT_REQUEST_ID;

	int iCP = 0;
	_JIF( Write2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserName, 32, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrGMName, 32, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrAccount, 32, iCP ), return false );
	_JIF( WriteByte( kPacket.m_bWrite, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}
#endif //SERV_INFOSERVER_ADD_WORK
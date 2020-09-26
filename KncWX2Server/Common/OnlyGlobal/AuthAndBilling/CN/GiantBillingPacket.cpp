#include "GiantBillingPacket.h"

#ifdef SERV_COUNTRY_CN

KGiantBillingPacket::KGiantBillingPacket()
: KGiantCommonPacket()
{
}

KGiantBillingPacket::~KGiantBillingPacket()
{
}


/* 현재 안 쓰는 함수 주석 처리
bool KGiantBillingPacket::Read( KEGIANT_BILLING_USE_CARD_CMD& kPacket )
{
	int iCP = 0;
	_JIF( Read2Byte( kPacket.m_sSource, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrAccount, 65, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrCardID, 33, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrPasswd, 9, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrIP, 17, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiUID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iAT, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiConTempID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiOperateUID, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantBillingPacket::Write( const KEGIANT_BILLING_USE_CARD_CMD& kPacket )
{
	m_byteCommand = GB_BCT_USE_CARD;
	m_byteParaCommand = GB_PCT_USE_CARD_REQ;

	int iCP = 0;
	_JIF( Write2Byte( kPacket.m_sSource, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrAccount, 65, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrCardID, 33, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrPasswd, 9, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrIP, 17, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiUID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iAT, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiConTempID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiOperateUID, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantBillingPacket::Read( KEGIANT_BILLING_USE_CARD_RET_CMD& kPacket )
{
	int iCP = 0;
	_JIF( Read2Byte( kPacket.m_sSource, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrAccount, 65, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrCardID, 33, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrPasswd, 9, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrIP, 17, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iPoint, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iRet, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiUID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iAT, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiConTempID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiOperateUID, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantBillingPacket::Write( const KEGIANT_BILLING_USE_CARD_RET_CMD& kPacket )
{
	m_byteCommand = GB_BCT_USE_CARD;
	m_byteParaCommand = GB_PCT_USE_CARD_ACK;

	int iCP = 0;
	_JIF( Write2Byte( kPacket.m_sSource, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrAccount, 65, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrCardID, 33, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrPasswd, 9, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrIP, 17, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iPoint, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iRet, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiUID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iAT, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiConTempID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiOperateUID, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}
*/

bool KGiantBillingPacket::Read( KEPUBLISHER_BILLING_BALANCE_REQ& kPacket )
{
	int iCP = 0;
	_JIF( Read4Byte( kPacket.m_uiPublisherUID, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrAccount, 65, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iAT, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantBillingPacket::Write( const KEPUBLISHER_BILLING_BALANCE_REQ& kPacket )
{
	m_byteCommand = GB_BCT_BALANCE;
	m_byteParaCommand = 0;

	int iCP = 0;
	_JIF( Write4Byte( kPacket.m_uiPublisherUID, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrAccount, 65, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iAT, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantBillingPacket::Read( KEGIANT_BILLING_CONSUME_REQ& kPacket )
{
	int iCP = 0;	
	_JIF( Read4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iPoint, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iSource, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrRemark, 201, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrIP, 17, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iAT, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantBillingPacket::Write( const KEGIANT_BILLING_CONSUME_REQ& kPacket )
{
	m_byteCommand = GB_BCT_CONSUME;
	m_byteParaCommand = 0;

	int iCP = 0;
	_JIF( Write4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iPoint, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iSource, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrRemark, 201, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrIP, 17, iCP ), return false );	
	_JIF( Write4Byte( kPacket.m_iAT, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantBillingPacket::Read( KEGIANT_BILLING_PCARD_CMD& kPacket )
{
	int iCP = 0;
	_JIF( Read4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iSource, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrPCardID, 33, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrIP, 17, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iAT, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantBillingPacket::Write( const KEGIANT_BILLING_PCARD_CMD& kPacket )
{
	m_byteCommand = GB_BCT_CARD;
	m_byteParaCommand = GB_PCT_PCARD;

	int iCP = 0;		
	_JIF( Write4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iSource, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrPCardID, 33, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrIP, 17, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iAT, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantBillingPacket::Read( KEGIANT_BILLING_SCARD_CMD& kPacket )
{
	int iCP = 0;
	_JIF( Read4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iSource, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrSCardID, 33, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrIP, 17, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iAT, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantBillingPacket::Write( const KEGIANT_BILLING_SCARD_CMD& kPacket )
{
	m_byteCommand = GB_BCT_CARD;
	m_byteParaCommand = GB_PCT_SCARD;

	int iCP = 0;		
	_JIF( Write4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iSource, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrSCardID, 33, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrIP, 17, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iAT, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantBillingPacket::Read( KEGIANT_BILLING_RETURN& kPacket )
{
	int iCP = 0;
	_JIF( ReadString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iBalance, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iBonus, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iHadFilled, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iRet, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iSubat, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iAT, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iPoint, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantBillingPacket::Write( const KEGIANT_BILLING_RETURN& kPacket )
{
	m_byteCommand = GB_BCT_RETURN;
	m_byteParaCommand = 0;

	int iCP = 0;	
	_JIF( WriteString( kPacket.m_wstrTID, 33, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iBalance, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iBonus, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iHadFilled, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iRet, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iSubat, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iAT, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iPoint, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_uiGiantUID, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

#endif // SERV_COUNTRY_CN


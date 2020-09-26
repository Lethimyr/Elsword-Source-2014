#include "StdAfx.h"

#include <winsock2.h>
#include "CollectPacket.h"

SERIALIZE_DEFINE_PUT( KCrashInfo, obj, ks )
{
	return PUT( m_strFileName )
		&& PUT( m_sType )
		&& PUT( m_strHackingInfo )
		&& PUT( m_strUserID )
		&& PUT( m_strMachineID )
		&& PUT( m_iServerSN )
		&& PUT( m_strRegDate )
		&& PUT( m_strText )
		&& PUT( m_charServerSN[1] )
		&& PUT( m_charServerSN[2] )
		&& PUT( m_charServerSN[3] )
		&& PUT( m_charServerSN[4] )
		&& PUT( m_charServerSN[5] )
		&& PUT( m_charServerSN[6] )
		&& PUT( m_charServerSN[7] )
		&& PUT( m_charServerSN[8] )
		&& PUT( m_charServerSN[9] )
		&& PUT( m_charServerSN[10] )
		&& PUT( m_charServerSN[11] )
		;
};

SERIALIZE_DEFINE_GET( KCrashInfo, obj, ks )
{
	return GET( m_strFileName )
		&& GET( m_sType )
		&& GET( m_strHackingInfo )
		&& GET( m_strUserID )
		&& GET( m_strMachineID )
		&& GET( m_iServerSN )
		&& GET( m_strRegDate )
		&& GET( m_strText )
		&& GET( m_charServerSN[1] )
		&& GET( m_charServerSN[2] )
		&& GET( m_charServerSN[3] )
		&& GET( m_charServerSN[4] )
		&& GET( m_charServerSN[5] )
		&& GET( m_charServerSN[6] )
		&& GET( m_charServerSN[7] )
		&& GET( m_charServerSN[8] )
		&& GET( m_charServerSN[9] )
		&& GET( m_charServerSN[10] )
		&& GET( m_charServerSN[11] )
		;
};

SERIALIZE_DEFINE_PUT( KECL_CRASH_INFO_REQ, obj, ks )
{
	return PUT( m_kCrashInfo )
		&& PUT( m_bExit )
		;
};

SERIALIZE_DEFINE_GET( KECL_CRASH_INFO_REQ, obj, ks )
{
	return GET( m_kCrashInfo )
		&& GET( m_bExit )
		;
};

SERIALIZE_DEFINE_PUT( KECL_CRASH_INFO_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_strFileName )
		&& PUT( m_bExit )
		;
};

SERIALIZE_DEFINE_GET( KECL_CRASH_INFO_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_strFileName )
		&& GET( m_bExit )
		;
};

SERIALIZE_DEFINE_PUT( KECL_CRASH_IMAGE_INFO_REQ, obj, ks )
{
	return PUT( m_strFileName )
		&& PUT( m_sNo )
		&& PUT( m_bLast )
		&& PUT( m_vecImageBuff )
		&& PUT( m_dwBuffSize )
		&& PUT( m_bExit )
		;
};

SERIALIZE_DEFINE_GET( KECL_CRASH_IMAGE_INFO_REQ, obj, ks )
{
	return GET( m_strFileName )
		&& GET( m_sNo )
		&& GET( m_bLast )
		&& GET( m_vecImageBuff )
		&& GET( m_dwBuffSize )
		&& GET( m_bExit )
		;
};

SERIALIZE_DEFINE_PUT( KECL_CRASH_IMAGE_INFO_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_strFileName )
		&& PUT( m_sRecvedNo )
		&& PUT( m_dwRecvedTotalBuffSize )
		&& PUT( m_bExit )
		;
};

SERIALIZE_DEFINE_GET( KECL_CRASH_IMAGE_INFO_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_strFileName )
		&& GET( m_sRecvedNo )
		&& GET( m_dwRecvedTotalBuffSize )
		&& GET( m_bExit )
		;
};
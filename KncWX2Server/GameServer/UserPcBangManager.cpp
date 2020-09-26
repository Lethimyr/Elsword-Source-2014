#include ".\userpcbangmanager.h"

#ifdef SERV_PC_BANG_TYPE
	#include "GameSysVal/GameSysVal.h"
#endif SERV_PC_BANG_TYPE

KUserPcBangManager::KUserPcBangManager(void) :
m_bCanTry( false ),
m_bIsGameBang( false ),
#ifdef SERV_PC_BANG_TYPE
m_iPcBangType( -1 ),
#endif SERV_PC_BANG_TYPE
m_bytePCBangAuthResult( 0 ),
//{{ 2010. 06. 08  ÃÖÀ°»ç	³Ø½¼PC¹æ ÀÎÁõ ¼­¹ö °³Æí
#ifdef SERV_PCBANG_AUTH_NEW
m_byteAuthorizeType( 0 ),
m_byteChargeType( 0 ),
#else
m_byteAddressDesc( 0 ),
m_byteAccountDesc( 0 ),
#endif SERV_PCBANG_AUTH_NEW
//}}
m_bReservePcBangMessage( false ),
m_uiArgument( 0 )
{
}

KUserPcBangManager::~KUserPcBangManager(void)
{
}

//{{ 2010. 06. 08  ÃÖÀ°»ç	³Ø½¼PC¹æ ÀÎÁõ ¼­¹ö °³Æí
#ifdef SERV_PCBANG_AUTH_NEW

void KUserPcBangManager::InitPcBangAuth( BYTE bytePCBangAuthResult, BYTE byteAuthorizeType, BYTE byteChargeType )
{
	m_bytePCBangAuthResult	= bytePCBangAuthResult;
	m_byteAuthorizeType		= byteAuthorizeType;
	m_byteChargeType		= byteChargeType;
}

#else

void KUserPcBangManager::InitPcBangAuth( BYTE bytePCBangAuthResult, BYTE byteAddressDesc, BYTE byteAccountDesc )
{
	m_bytePCBangAuthResult = bytePCBangAuthResult;
	m_byteAddressDesc = byteAddressDesc;
	m_byteAccountDesc = byteAccountDesc;
}

#endif SERV_PCBANG_AUTH_NEW
//}}

void KUserPcBangManager::SetPcBangMessage( u_int uiArgument )
{
	m_uiArgument = uiArgument;

	m_bReservePcBangMessage = true;
}

u_int KUserPcBangManager::GetPcBangMessageArgument()
{
	m_bReservePcBangMessage = false;
	
	return m_uiArgument;
}

#ifdef SERV_PC_BANG_TYPE
KPcBangReward KUserPcBangManager::GetPcBangReward()
{
	return SiKGameSysVal()->GetPcBangReward( GetPcBangType() );	
}
#endif SERV_PC_BANG_TYPE
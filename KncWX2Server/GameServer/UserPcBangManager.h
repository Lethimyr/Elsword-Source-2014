#pragma once
#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"

class KUserPcBangManager
{
public:
	KUserPcBangManager(void);
	~KUserPcBangManager(void);

	void SetCanTry( bool bVal )			{ m_bCanTry = bVal; }

	void SetIsPcBang( bool bVal )		{ m_bIsGameBang = bVal; }
	bool IsPcBang()						{ return m_bIsGameBang; }
#ifdef SERV_PC_BANG_TYPE
	void SetPcBangType( int bVal )		{ m_iPcBangType = bVal; }
	int	GetPcBangType()					{ return m_iPcBangType; }
	KPcBangReward GetPcBangReward();
#endif SERV_PC_BANG_TYPE

	//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
	void InitPcBangAuth( BYTE bytePCBangAuthResult, BYTE byteAuthorizeType, BYTE byteChargeType );
#else
	void InitPcBangAuth( BYTE bytePCBangAuthResult, BYTE byteAddressDesc, BYTE byteAccountDesc );
#endif SERV_PCBANG_AUTH_NEW
	//}}
	
	void SetPcBangMessage( u_int uiArgument );
	u_int GetPcBangMessageArgument();

	bool IsReservedPcBangMessage()		{ return m_bReservePcBangMessage; }

private:
	// PC 방 인증
	bool			m_bCanTry;
	bool			m_bIsGameBang;
#ifdef SERV_PC_BANG_TYPE
	int				m_iPcBangType;
#endif SERV_PC_BANG_TYPE
	BYTE			m_bytePCBangAuthResult;
	//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
	BYTE			m_byteAuthorizeType;
	BYTE			m_byteChargeType;
#else
	BYTE			m_byteAddressDesc;
	BYTE			m_byteAccountDesc;
#endif SERV_PCBANG_AUTH_NEW
	//}}	

	// PC방 인증 서버 메시지
	bool			m_bReservePcBangMessage;
	u_int			m_uiArgument;
};
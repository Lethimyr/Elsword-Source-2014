#pragma once

#include "CommonPacket.h"

#pragma pack( push, 1 )

#define				CRASH_IMAGE_BUFF_MAX		15000

DECL_DATA( KCrashInfo )
{
	std::string							m_strFileName;
	unsigned short						m_sType;
	std::string							m_strHackingInfo;
	std::string							m_strUserID;
	std::string							m_strMachineID;
	std::string							m_strRegDate;
	std::string							m_strText;
	UidType								m_iServerSN;
	unsigned char						m_charServerSN[SERVER_SN];

	KCrashInfo()
	{
		for(int i = 0 ; i < SERVER_SN ; ++i)
		{
			m_charServerSN[i]	= 0;				// ServerSN
		}
	}
};

DECL_PACKET( ECL_CRASH_INFO_REQ )
{
	KCrashInfo					m_kCrashInfo;
	bool						m_bExit;

	KECL_CRASH_INFO_REQ()
	{
		m_bExit = false;
	}
};

DECL_PACKET( ECL_CRASH_INFO_ACK )
{
	int									m_iOK;
	std::string							m_strFileName;
	bool								m_bExit;

	KECL_CRASH_INFO_ACK()
	{
		m_iOK = 0;
		m_bExit = false;
	}
};

DECL_PACKET( ECL_CRASH_IMAGE_INFO_REQ )
{
	std::string							m_strFileName;
	unsigned short						m_sNo;
	bool								m_bLast;
	std::vector<char>					m_vecImageBuff;
	DWORD								m_dwBuffSize;		// 이 패킷의 이미지 버퍼 사이즈
	bool								m_bExit;

	KECL_CRASH_IMAGE_INFO_REQ()
	{
		m_sNo				= 0;
		m_bLast				= false;
		m_dwBuffSize		= CRASH_IMAGE_BUFF_MAX;
		m_bExit				= false;
	}
};

DECL_PACKET( ECL_CRASH_IMAGE_INFO_ACK )
{
	int									m_iOK;
	std::string							m_strFileName;
	unsigned short						m_sRecvedNo;
	DWORD								m_dwRecvedTotalBuffSize;	// 이때 까지 받은 이미지 버퍼 사이즈
	bool								m_bExit;

	KECL_CRASH_IMAGE_INFO_ACK()
	{
		m_iOK					= 0;
		m_sRecvedNo				= 0;
		m_dwRecvedTotalBuffSize		= 0;
		m_bExit				= false;
	}
};

DECL_PACKET( ECL_DISCONNECT_REQ )
{
	enum CLIENT_HACKING_DISCONNECT_REASON
	{
		CHDR_SUCCESS					= 0,	// 정상 종료
		CHDR_SEND_FAIL,							// 전송 실패
		CHDR_END,								// 사유의 끝
	};

	int									m_iReason;
};

DECL_PACKET( ECL_DISCONNECT_ACK )
{
	int									m_iOK;
};



#pragma pack( pop )

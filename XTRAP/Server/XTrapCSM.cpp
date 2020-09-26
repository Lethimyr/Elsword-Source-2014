#include "GSSimLayer.h"
#include "GSUser.h"
#include "XTrapCSM.h"
#include "NetError.h"

#define VERIFY_ENABLE( reCode ) \
	if( GetEnable() == false ) \
	{ \
	reCode; \
	}

#ifdef SERV_USE_XTRAP
CXTrapCSM::CXTrapCSM() : m_bEnable(false), 
						 m_bTimerStart(false)
{

}
CXTrapCSM::~CXTrapCSM()
{

}

bool CXTrapCSM::InitCSM()
{
	VERIFY_ENABLE( return true )

	XTrap_S_SessionInit( 600, GetKGSSimLayer()->GetMapQuantity(), GetKGSSimLayer()->GetAllMapPointer(), m_cSessionBuf );
	return true;
}

// step1 : 인증 패킷 만들어서 클라에 보낸다.
bool CXTrapCSM::CSMStep1( IN KGSUserPtr spUser )
{
	VERIFY_ENABLE( return true )

	static DWORD dwExecute = 0;
	if( ( GetTickCount() - dwExecute ) <= 20000 )
	{
		return true;
	}
		
	KEGS_XTRAP_REQ kPacket;
	kPacket.m_vecData.resize(200);

	unsigned char temp[200];
	INT nRet = XTrap_CS_Step1(m_cSessionBuf, temp);

	for( u_int ui = 0; ui < 200; ++ui )
	{
		kPacket.m_vecData[ui] = temp[ui];
	}

	spUser->SendPacket(EGS_XTRAP_REQ, kPacket);

	if( nRet == XTRAP_API_RETURN_OK )		// 반환값이 정상인 경우
	{
		dwExecute = GetTickCount();

		if( !m_bTimerStart )
		{
			m_bTimerStart = true;
			m_kTimer.restart();
		}
		else
		{
			if(m_kTimer.elapsed() >= ST_LOOP_AUTH_TIME)
			{
				spUser->SetDisconnectReason(KStatistics::eSIColDR_nProtect_Hacking);
				spUser->ReserveDestroy();	
			}
		}

		return true;
	}
	else
	{
		START_LOG( cerr, L"XTrap_CS_Step1 오류 리턴 :" << nRet );

		if( nRet == XTRAP_API_RETURN_DETECTHACK )
		{
			unsigned int DetectCode = 0;
			// 에러 나서 주석
			//memcpy( &DetectCode, (static_cast<unsigned char*>(m_cSessionBuf)) + 8, 4 );

			//START_LOG( cerr, L"스텝1, DetectCode 값 : " << std::hex << DetectCode );
		}

		return false;
	}
}

// step3 : 클라이언트로부터 패킷을 받는다
bool CXTrapCSM::CSMStep3(KEGS_XTRAP_ACK* _packet)
{
	VERIFY_ENABLE( return true )

	m_kTimer.restart();
	unsigned char arrResult[200];

	for( u_int ui = 0; ui < 200; ++ui )
	{
		arrResult[ui] = _packet->m_vecData[ui];
	}

	unsigned int nRet = XTrap_CS_Step3( m_cSessionBuf, arrResult );
	if( nRet == XTRAP_API_RETURN_OK )	// 반환값이 정상인 경우
	{
		return true;
	}
	else
	{
		START_LOG( cerr, L"XTrap step3 오류 리턴 : " << nRet );

		if( nRet == XTRAP_API_RETURN_DETECTHACK )
		{
			unsigned int DetectCode=0;
			memcpy( &DetectCode, ((unsigned char *)m_cSessionBuf+8), 4 );

			START_LOG( cerr, L"스텝3, DetectCode 값 : " << std::hex << DetectCode );
		}

		return false;
	}
}
#endif SERV_USE_XTRAP
#pragma once

#ifdef CONVERSION_VS
//더이상 사용되지 않는 클래스
#else CONVERSION_VS

#include "KTDNPacket.h"
#include "KTDNTransfer.h"
#include "KTDNDP.h"
#include "KTDNUDP.h"

class CKTDNManager : public CKTDXStage
{
	public:
		CKTDNManager(void);
		~CKTDNManager(void);

		CKTDNDP*	GetDP(){ return m_pKTDNDP; }

	private:
		CKTDNDP*	m_pKTDNDP;

};
#endif CONVERSION_VS
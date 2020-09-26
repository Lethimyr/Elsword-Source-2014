#pragma once

#ifdef SERV_USE_XTRAP
#include "Xtrap_S_Interface.h"

class KGSUser;
SmartPointer(KGSUser);

// 개인 유저별 XTrap Map file 체크모듈 운용 클래스
// 1. 서버 -> 클라 : 체크후 정보 요청
// 2. 클라 -> 서버 : 응답 정보 전달
// 3. 서버		   : 정보 저장

class CXTrapCSM{

	enum ENUM_SET_TIME
	{		
		ST_LOOP_AUTH_TIME = 180,
	};

public:
	CXTrapCSM();
	~CXTrapCSM();

	// 사용 설정
	void SetEnable( bool bEnable ) { m_bEnable = bEnable; }
	const bool GetEnable() { return m_bEnable; }

	bool InitCSM();
	bool CSMStep1(IN KGSUserPtr spUser);
	bool CSMStep3(KEGS_XTRAP_ACK* _packet);

	// 버퍼 가져오기
	char* GetBuf(){ return m_cSessionBuf; }

protected:
	bool			m_bEnable;				// XTRAP 활성화 여부
	boost::timer	m_kTimer;
	bool			m_bTimerStart;
	char			m_cSessionBuf[320];		// X-Trap 세션 버퍼
};
#endif SERV_USE_XTRAP
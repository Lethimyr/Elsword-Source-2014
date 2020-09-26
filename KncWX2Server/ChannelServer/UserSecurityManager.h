#pragma once
#include "Event.h"
#include "CommonPacket.h"


//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_BLOCK


class KUserSecurityManager
{
public:
	KUserSecurityManager(void);
	~KUserSecurityManager(void);	

	// 정보
	const std::wstring GetWStringMachineID() const { return m_wstrMachineID; }
	const std::string GetStringMachineID() const;
	void SetMachineID( IN const std::string& strMachineID );	

	// 검사
	bool CheckValidMachineID( IN const std::string& strMachineID ) const;

	//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	int GetChannelRandomKey()						{ return m_iChannelRandomKey; }
	void SetChannelRandomKey( IN int& iKey )		{ m_iChannelRandomKey = iKey; }
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}


private:
	static WORD GetHashFromMachineID( IN const std::string& strMachineID );
	static WORD MakeHashFromMachineID( IN const std::string& strMachineID );

private:
	std::wstring					m_wstrMachineID;

	//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	int								m_iChannelRandomKey;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}

};


#endif SERV_MACHINE_ID_BLOCK
//}}
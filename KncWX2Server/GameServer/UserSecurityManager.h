#pragma once
#include "Event.h"
#include "CommonPacket.h"

//{{ 2012. 04. 02	김민성		sn 모니터링 기능 분리
#if defined(SERV_SERIAL_NUMBER_MORNITORING) || defined(SERV_IMPOSSIBLE_SELL_ITEM)  // 빌드 오류로 해외팀 추가
	#include <boost/timer.hpp>
#endif /* SERV_SERIAL_NUMBER_MORNITORING or SERV_IMPOSSIBLE_SELL_ITEM */
//}}

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
	//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	void SetServerSN( IN __int64& iServerSN )		{ m_iServerSN = iServerSN; }
	void GetServerSN( OUT __int64& iServerSN )		{ iServerSN = m_iServerSN; }
	//{{ 2012. 04. 02	김민성		sn 모니터링 기능 분리
#ifdef SERV_SERIAL_NUMBER_MORNITORING
	enum SERVER_SN_MONITORING
	{
		SSM_NONE		= 0,
		SSM_MONITORING	= 1,
		SSM_BLOCK		= 2,
	};

	void SetServerSN_Flag( IN int& iMonitoringFlag )		{ m_iMonitoringFlag = iMonitoringFlag; }
	bool CheckServerSN_Monitoring();
#endif SERV_SERIAL_NUMBER_MORNITORING
	//}}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}
	
	//{{ 2012. 05. 23	김민성       판매 불가 아이템 판매 시도 해킹 유저로 판단
#ifdef SERV_IMPOSSIBLE_SELL_ITEM
	enum KICK_TYPE
	{
		KT_NONE					= 0,
		KT_AFTER_RANDOM_TIME	= 1,
	};

	void SetKickRandomTime( int iMinTime, int iRange );
	void SetKickState( IN char cState );
	bool CheckIsKickUser();
	int	 GetKickReason()					{ return m_iKickReason; }
	void SetKickReason( int iReason )		{ m_iKickReason = iReason; }
#endif SERV_IMPOSSIBLE_SELL_ITEM
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
	//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	__int64							m_iServerSN;
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}
	//{{ 2012. 04. 02	김민성		sn 모니터링 기능 분리
#ifdef SERV_SERIAL_NUMBER_MORNITORING
	boost::timer					m_TimerSNMonitoring;
	int								m_iMonitoringFlag;
#endif SERV_SERIAL_NUMBER_MORNITORING
	//}}
	//{{ 2012. 05. 23	김민성       판매 불가 아이템 판매 시도 해킹 유저로 판단
#ifdef SERV_IMPOSSIBLE_SELL_ITEM
	int								m_iKickRandomTime;
	char							m_cKickState;
	boost::timer					m_tKickTimer;
	int								m_iKickReason;
#endif SERV_IMPOSSIBLE_SELL_ITEM
	//}}
};


#endif SERV_MACHINE_ID_BLOCK
//}}
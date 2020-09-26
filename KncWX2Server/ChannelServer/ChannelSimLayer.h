#pragma once
#include "SimLayer.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_PH
#pragma comment(lib, "../Common/OnlyGlobal/AuthAndBilling/PH/libeay32MD.lib")
#include "../Common/OnlyGlobal/AuthAndBilling/PH/Security/base64.h"
#include "../Common/OnlyGlobal/AuthAndBilling/PH/Security/OpensslRSA.h"
#endif //SERV_COUNTRY_PH

/** 
@class : KChannelSimLayer
@brief : 게임 로직을 처리하는 SimLayer를 채널서버서 사용하기 위해 상속받은 클래스
@date : 2012/09/28
*/
class KChannelSimLayer : public KSimLayer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclDump;

	//{{ 2010. 07. 02  최육사	IP차단 기능 확장	
#ifdef SERV_IP_CHECK_EXPAND
	enum CHECK_IP_TYPE
	{
		CIT_ALL_IP_ALLOW = 0,
		CIT_CHECK_IP_ALLOW,
		CIT_CHECK_IP_BLOCK,
	};
#endif SERV_IP_CHECK_EXPAND
	//}}

	//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
#ifdef SERV_CHANNEL_SERV_CHECK_MODE
	enum CHECK_IP_MODE
	{
        CIM_SERVICE_MODE = 0,
		CIM_CHECK_MODE,

		CIM_MAX,
	};

	struct KConnectIpCheckInfo
	{
		std::vector< std::string >  m_vecIP;
		CHECK_IP_TYPE				m_eCheckIPType;

		KConnectIpCheckInfo()
		{
			m_eCheckIPType = CIT_ALL_IP_ALLOW;
		}
	};
#endif SERV_CHANNEL_SERV_CHECK_MODE
	//}}

public:
    KChannelSimLayer(void);
    virtual ~KChannelSimLayer(void);

    // derived from KSimLayer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();
    virtual void RegToLua();

			//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
			#ifdef SERV_CHANNEL_SERV_CHECK_MODE
			void SetCheckIPMode_LUA( int iVal );
			#endif SERV_CHANNEL_SERV_CHECK_MODE
			//}}

			//////////////////////////////////////////////////////////////////////////
			// 일반 모드
			void AddIP( const char* szAddIP );
			void SetCheckIPType( int iVal );

			//////////////////////////////////////////////////////////////////////////
			//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
#ifdef SERV_CHANNEL_SERV_CHECK_MODE
			void AddCheckModeIP( const char* szAddIP );
			void SetCheckModeCheckIPType( int iVal );
#endif SERV_CHANNEL_SERV_CHECK_MODE
			//}}

#ifdef SERV_COUNTRY_PH
			COpensslRSA GetOpensslRSA() { return m_OpensslRSAChannel; }
			bool		GetOpensslRSAInit() { return m_bOpensslRSAInit; }
#endif //SERV_COUNTRY_PH

			//////////////////////////////////////////////////////////////////////////
			// IP 체크
			bool CheckIP( IN const char* szIP );

			//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
#ifdef SERV_CHANNEL_SERV_CHECK_MODE
			CHECK_IP_MODE GetCheckIPMode() const { return m_eCheckIPMode; }
protected:
			bool CheckIP( IN const CHECK_IP_MODE eCheckIpMode, IN const char* szIP );
#endif SERV_CHANNEL_SERV_CHECK_MODE
			//}}

protected:
	//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
#ifdef SERV_CHANNEL_SERV_CHECK_MODE
	KConnectIpCheckInfo			m_kConnectIpCheck[CIM_MAX];
	CHECK_IP_MODE				m_eCheckIPMode;
#else
	std::vector< std::string >  m_vecIP;
	CHECK_IP_TYPE				m_eCheckIPType;
#endif SERV_CHANNEL_SERV_CHECK_MODE
	//}}

	//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	bool						m_bCheckServerSN;
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	//}}

#ifdef SERV_COUNTRY_PH
	bool		m_bOpensslRSAInit;
	COpensslRSA m_OpensslRSAChannel;
#endif //SERV_COUNTRY_PH
};

DefKObjectInline( KChannelSimLayer, KSimLayer );



#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_TWHK

//{{ 2011.7.12 지헌 : Gash 포인트 통합
#ifdef SERV_BILLING_ADD_GASH_POINT

class KGASHBillingTCPManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KGASHBillingTCPManager );

public:
	enum GASH_PORT
	{
		GP_AUTHORIZATION,
		GP_ACCOUNTING,

		GP_NUM,
	};

	KGASHBillingTCPManager();
	virtual ~KGASHBillingTCPManager();

	void RegToLua();

	void InitGASHBillingTCPValue( const char* szServiceCode, const char* szServiceRegion );

	void InitGASHBillingTCPAddress(
		const char*     szGconsoleIP,
		unsigned short  usAuthorizationPort,
		unsigned short  usAccountingPort );

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
	virtual void Tick();

	void QueueingSendPacket( int iPortSelect, const boost::shared_ptr< std::string >& spPacket );

	std::string GetServiceCode()            { return m_kGASHAuthInfo.m_strServiceCode; }
	std::string GetServiceRegion()          { return m_kGASHAuthInfo.m_strServiceRegion; }

	// Buy 패킷 보관, 가져오기
	bool InsertBuyPacket( IN UidType iSenderUID, IN KEBILL_BUY_PRODUCT_REQ packet);
	bool GetBuyPacket( IN UidType iSenderUID, OUT KEBILL_BUY_PRODUCT_REQ &packet);

	// 선물 패킷 보관, 가져오기
	bool InsertGiftPacket( IN UidType iSenderUID, IN KEBILL_GIFT_ITEM_REQ packet);
	bool GetGiftPacket( IN UidType iSenderUID, OUT KEBILL_GIFT_ITEM_REQ &packet);

	// 구매 패킷을 이미 보관중인지 체크
	bool IsHaveBuyPacket(IN UidType iSenderUID);
	bool IsHaveGiftPacket(IN UidType iSenderUID);

	// 로그 남기기 위해서 잔액 조회 할 때마다 그 액수를 보관하고, 가져올 수 있도록 했다.
	bool InsertHaveGashPoint( IN UidType iUserUID, IN int iGashPoint );
	bool GetHaveGashPoint( IN UidType iUserUID, OUT int &iGashPoint );

protected:
	void RecvFromAuthorization();
	void SendToAuthorization();
	void RecvFromAccounting();
	void SendToAccounting();

	void SendTo(int iPortEnum);
	void RecvFrom(int iPortEnum);

	bool Connect( int iPortSelect );
	bool IsConnected( int iPortSelect );
	void CheckConnection();

	bool GetSendPacket( int iPortSelect, boost::shared_ptr< std::string >& spPacket );

	void MakeEventFromReceived(int iPortEnum, std::string& strPacket);
	void DumpRecvBuffer( bool bError );

public:

protected:
	struct KGASHAuthInfo
	{
		std::string         m_strServiceCode;
		std::string         m_strServiceRegion;

		std::string         m_strIP;
		unsigned short      m_usGash_Port[GP_NUM];

		KGASHAuthInfo()
		{
			for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
			{
				m_usGash_Port[iPortNum] = 0;
			}
		}

	} m_kGASHAuthInfo;

	DWORD				m_dwLastConnectionCheckTick[GP_NUM];
	static DWORD		ms_dwConnectionCheckGap[GP_NUM];

	SOCKET				m_sock[GP_NUM];

	boost::shared_ptr< KTThread< KGASHBillingTCPManager > >	m_spThreadRecv[GP_NUM];
	boost::shared_ptr< KTThread< KGASHBillingTCPManager > >	m_spThreadSend[GP_NUM];

	std::queue< boost::shared_ptr< std::string > >      m_kSendQueue[GP_NUM];
	mutable KncCriticalSection                          m_csSendQueue[GP_NUM];

	char                                                m_cRecvBuffer[GP_NUM][MAX_PACKET_SIZE_OF_GASH_AUTH];
	int                                                 m_iRecvCP[GP_NUM];

	//{{ 2011.3.16 최초 계정 생성 시 IP 넣기 (HK/TW)
#ifdef SERV_INSERT_ACCOUNT_IP
	std::map< std::string, std::string >                m_mapUserIDUserIP;
#endif
	//}}

	// GASH 빌링의 경우, 소모 포인트만 넘겨주고 받기 때문에, 구입 한 물품 목록은 따로 보관해 둬야 한다.
	mutable KncCriticalSection                          m_csBuyGiftPacket;
	std::map<UidType, KEBILL_BUY_PRODUCT_REQ>			m_mapBuyProductReq;
	std::map<UidType, KEBILL_GIFT_ITEM_REQ>				m_mapGiftItemReq;


	// 이건 로그를 위해서 유저의 GASH Point 를 저장 시켜 놓은 것
	mutable KncCriticalSection                          m_csHaveGashPoint;
	std::map<UidType,  int>								m_mapHaveGashPoint;
};

DefSingletonInline( KGASHBillingTCPManager );

#endif // SERV_BILLING_ADD_GASH_POINT
//}}

#endif // SERV_COUNTRY_TWHK
#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

//#include "NaunBillingPacket.h"

#ifdef SERV_COUNTRY_IN
class KNaunBillingManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KNaunBillingManager );

public:
	/*
	enum NAUN_PORT
	{
		NP_AUTHORIZATION,	// 잔액 조회
		NP_ACCOUNTING,		// 입금(취소)/출금(취소)
		NP_NUM,
	};
	*/
	enum NAUN_BUY_TYPE
	{
		NBT_BUY_ONE_PRODUCT		= 10,	// 단품 구매
		NBT_BUY_ONE_PACKAGE		= 11,	// 패키지 구매
		NBT_BUY_MANY_PRODUCT	= 12,	// 일괄 구매
	};

	KNaunBillingManager();
	virtual ~KNaunBillingManager();

	void RegToLua();
	//void InitNaunBillingValue( const char* szServiceCode, const char* szServiceRegion );
	//void InitNaunBillingAddress( const char* szIPGServerIP, unsigned short usAuthorizationPort, unsigned short usAccountingPort );

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
	virtual void Tick();	// KPerformer::Tick()이 호출되어 KPerformer::ProcessEvent()가 호출되는 것을 막기 위해 의도적으로 아무 기능도 하지 않는 KNaunBillingManager::Tick을 재정의 한 것임.
	//void QueueingSendPacket( int iPortSelect, const boost::shared_ptr< KNaunBillingPacket >& spPacket );

	// 구매 패킷 보관, 가져오기, 보관중인지 확인
	/*
	bool InsertBuyPacket( IN UidType iSenderUID, IN KEBILL_BUY_PRODUCT_REQ packet );
	bool GetBuyPacket( IN UidType iSenderUID, OUT KEBILL_BUY_PRODUCT_REQ& packet );
	bool IsHaveBuyPacket( IN UidType iSenderUID );

	// 선물하기 패킷 보관, 가져오기, 보관중인지 확인
	bool InsertGiftPacket( IN UidType iSenderUID, IN KEBILL_GIFT_ITEM_REQ packet );
	bool GetGiftPacket( IN UidType iSenderUID, OUT KEBILL_GIFT_ITEM_REQ& packet );
	bool IsHaveGiftPacket( IN UidType iSenderUID );
	*/
protected:
	/*
	void RecvFromAuthorization();
	void SendToAuthorization();
	void RecvFromAccounting();
	void SendToAccounting();

	void SendTo(int iPortEnum);
	void RecvFrom(int iPortEnum);

	bool Connect( int iPortSelect );
	bool IsConnected( int iPortSelect );
	void CheckConnection();

	bool GetSendPacket( int iPortSelect, boost::shared_ptr< KNaunBillingPacket >& spPacket );

	void MakeEventFromReceivedPacket( int iPortEnum );

	struct KNaunBillingInfo
	{
		std::string	m_strIP;
		unsigned short m_usNaunPort[NP_NUM];

		KNaunBillingInfo()
		{
			for( int iPortNum = 0; iPortNum < NP_NUM; iPortNum++ )
			{
				m_usNaunPort[iPortNum] = 0;
			}
		}
	} m_kNaunBillingInfo;

	DWORD				m_dwLastConnectionCheckTick[NP_NUM];
	static DWORD		ms_dwConnectionCheckGap[NP_NUM];

	SOCKET				m_sock[NP_NUM];

	boost::shared_ptr< KTThread< KNaunBillingManager > >	m_spThreadRecv[NP_NUM];
	boost::shared_ptr< KTThread< KNaunBillingManager > >	m_spThreadSend[NP_NUM];

	std::queue< boost::shared_ptr< KNaunBillingPacket > >	m_kSendQueue[NP_NUM];
	mutable KncCriticalSection								m_csSendQueue[NP_NUM];

	char                                                m_cRecvBuffer[NP_NUM][MAX_PACKET_SIZE_NUBT];
	int                                                 m_iRecvCP[NP_NUM];

	// 구입 및 선물하기 물품 목록은 따로 보관함.
	mutable KncCriticalSection                          m_csBuyGiftPacket;
	std::map<UidType, KEBILL_BUY_PRODUCT_REQ>			m_mapBuyProductReq;
	std::map<UidType, KEBILL_GIFT_ITEM_REQ>				m_mapGiftItemReq;
	*/
};
DefSingletonInline( KNaunBillingManager );
#endif SERV_COUNTRY_IN
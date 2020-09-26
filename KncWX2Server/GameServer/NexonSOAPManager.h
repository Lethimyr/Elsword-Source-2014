

#pragma once

#include <KNCSingleton.h>
#include <Thread/KCSLockType.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"

//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP

class KNexonSOAPManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KNexonSOAPManager );

public:
	KNexonSOAPManager();
	virtual ~KNexonSOAPManager();

	void RegToLua();

	void Init( IN int nThreadNum
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-11-06	// 박세훈
			 , IN char* szSoapServerAddressForUser
			 , IN char* szSoapServerAddressForCoupon
#endif // SERV_NEXON_COUPON_SYSTEM
			 );

	void AddWebServiceAddress( const char* szWSDL );
	void AddWebMethod( int iSoapAction, const char* szMethod );

	// derived from KThreadManager
	virtual KThread* CreateThread();
	//	virtual void BeginThread();

	// 현재 마땅히 하는 일 없음. KPerformer Tick이 혹시나 호출되어 이벤트 큐 건들이는 것을 막기 위해 재정의함 //
	// 원래 ChannelSimLayer Tick()에서 SiKGameForgeAuthManager()->Tick();하는거 주석 처리함
	virtual void Tick() { };		


protected:
	std::string m_strWsdlPath;

};

DefSingletonInline( KNexonSOAPManager );

#endif SERV_NEXON_AUTH_SOAP
//}}

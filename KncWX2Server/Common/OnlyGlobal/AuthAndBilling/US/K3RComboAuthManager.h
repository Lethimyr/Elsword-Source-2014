#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"

#ifdef SERV_COUNTRY_US

class KK3RComboAuthManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KK3RComboAuthManager );

public:
	KK3RComboAuthManager();
	virtual ~KK3RComboAuthManager();

	void RegToLua();

	void Init( int nThreadNum );

	void AddWebServiceAddress( const char* szWSDL );
	void AddWebMethod( int iSoapAction, const char* szMethod );

	// derived from KThreadManager
	virtual KThread* CreateThread();
	//	virtual void BeginThread();

	// 현재 마땅히 하는 일 없음. KPerformer Tick이 혹시나 호출되어 이벤트 큐 건들이는 것을 막기 위해 재정의함 //
	// 원래 ChannelSimLayer Tick()에서 SiKGameForgeAuthManager()->Tick();하는거 주석 처리함
	virtual void Tick();


protected:
	std::string m_strWsdlPath;

};

DefSingletonInline( KK3RComboAuthManager );

#endif // SERV_COUNTRY_US
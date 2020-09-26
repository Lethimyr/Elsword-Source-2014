#pragma once


#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"

#ifdef SERV_COUNTRY_EU

class KGameForgeAuthManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KGameForgeAuthManager );

public:
	KGameForgeAuthManager();
	virtual ~KGameForgeAuthManager();

	void RegToLua();
    
	void Init( int nThreadNum );
	
	void AddWebServiceAddress( const char* szWSDL );
	void AddWebMethod( int iSoapAction, const char* szMethod );

	// derived from KThreadManager
	virtual KThread* CreateThread();
//	virtual void BeginThread();
	
	// 현재 마땅히 하는 일 없음. KPerformer Tick이 혹시나 호출되어 이벤트 큐 건들이는 것을 막기 위해 재정의함 //
	// 원래 ChannelSimLayer Tick()에서 SiKGameForgeAuthManager()->Tick();하는거 주석 처리함
	virtual void Tick() { };		

	//{{ 2011. 08. 17	조효진	로그인 SOAP 함수 로그 
#ifdef SERV_SOAP_LOGIN_FUNC_LOG
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

	void SendSoapLoginFuncLog(std::wstring wstrUserID, CTime StartFuncTime, DWORD dwDiffTime, int iCase);
#endif SERV_SOAP_LOGIN_FUNC_LOG
	//}}

protected:
	std::string m_strWsdlPath;
	
};

DefSingletonInline( KGameForgeAuthManager );

//{{ 2011. 08. 17	조효진	로그인 SOAP 함수 로그 
#ifdef SERV_SOAP_LOGIN_FUNC_LOG
template < class T >
void KGameForgeAuthManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_CHANNEL_GF_AUTH, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, usEventID, data );
}
#endif SERV_SOAP_LOGIN_FUNC_LOG
//}}

#endif // SERV_COUNTRY_EU


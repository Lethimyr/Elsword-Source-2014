#include <WinSock2.h>
#include "NexonSOAPManager.h"
#include "NexonSOAP.h"
#include "BaseServer.h"

//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP

ImplToStringW( KNexonSOAPManager )
{
	return stm_;
}

ImplPfID( KNexonSOAPManager, PI_GS_NX_SOAP );

ImplementSingleton( KNexonSOAPManager );

KNexonSOAPManager::KNexonSOAPManager()
{
	m_strWsdlPath = "";
}

KNexonSOAPManager::~KNexonSOAPManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>

void KNexonSOAPManager::RegToLua()
{
	lua_tinker::class_add<KNexonSOAPManager>( g_pLua, "KNexonAuthManager" );
	lua_tinker::class_def<KNexonSOAPManager>( g_pLua, "Init",				 &KNexonSOAPManager::Init );
	lua_tinker::class_def<KNexonSOAPManager>( g_pLua, "dump",				 &KNexonSOAPManager::Dump );
	lua_tinker::class_def<KNexonSOAPManager>( g_pLua, "AddWebServiceAddress", &KNexonSOAPManager::AddWebServiceAddress );
	lua_tinker::class_def<KNexonSOAPManager>( g_pLua, "AddWebMethod",	&KNexonSOAPManager::AddWebMethod );

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( g_pLua, #id, KNexonSOAP::##id );
#   include "NexonSOAP_def.h"


	lua_tinker::decl( g_pLua, "NexonSOAP", this );
}

void KNexonSOAPManager::Init( IN int nThreadNum
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-11-06	// 박세훈
							, IN char* szSoapServerAddressForUser
							, IN char* szSoapServerAddressForCoupon
#endif // SERV_NEXON_COUPON_SYSTEM
							)
{
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-11-06	// 박세훈
	KNexonSOAP::SetSoapServerAddressForUser( std::string( szSoapServerAddressForUser ) );
	KNexonSOAP::SetSoapServerAddressForCoupon( std::string( szSoapServerAddressForCoupon ) );
#endif // SERV_NEXON_COUPON_SYSTEM

	// Init 호출 시점은 항상 AddWebServiceAddress 지정후여야 함 //
	// 그 이유는 바로 아래 CreateThread에서 sdlFile과 smlFile이 입력되어야 하기 때문이다.
	KThreadManager::Init( nThreadNum );
}

//    KSimLayer::GetKObj()->Init(); 될때 BeginThread 호출됨


void KNexonSOAPManager::AddWebServiceAddress( const char* szWSDL )
{
	m_strWsdlPath = szWSDL;

	START_LOG( cout, L"Nexon SOAP 주소 등록!" )
		<< BUILD_LOG( m_strWsdlPath );
}

void KNexonSOAPManager::AddWebMethod( int iSoapAction, const char* szMethod )
{
	KNexonSOAP::AddWebMethod( iSoapAction, szMethod );
}


KThread* KNexonSOAPManager::CreateThread()
{
	// Init 호출 시점은 항상 AddWebServiceAddress 지정후여야 함 //
	// 그 이유는 바로 아래 생성자에 sdlFile과 smlFile이 입력되어야 하기 때문이다.


	return new KNexonSOAP(m_strWsdlPath.c_str());
}

#endif SERV_NEXON_AUTH_SOAP
//}}

#pragma once


#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"
#include "KncException.h"


#ifdef SERV_COUNTRY_US

#include "SoapK3R/soapH.h" // or whatever it is called, you must already have it

class KK3RComboAuth :public KSubThread
{
	struct KWebMethod
	{
		int m_iSoapAction;
		std::string m_strMethodPath;

		KWebMethod()
		{
			m_iSoapAction = 0;
			m_strMethodPath = std::string();
			m_strMethodPath = "";
		};

		KWebMethod( const KWebMethod& t )
		{
			*this = t;
		};

		KWebMethod& operator=( const KWebMethod& t )
		{
			m_iSoapAction = t.m_iSoapAction;
			m_strMethodPath = t.m_strMethodPath;

			return *this;
		};
	};

	_DeclareException;


public:
#   undef _ENUM
#   define _ENUM( id ) id,
	enum
	{
#   include "K3RComboAuth_def.h"
	};

	KK3RComboAuth(std::string WsdlFile);
	virtual ~KK3RComboAuth();



public:
	virtual void Run();	// Soap Initialize 를 넣기 위해 재정의함 //

	void SoapInitialize();
	void SoapUninitialize();

	static void AddWebMethod( int iSoapAction, std::string szMethod );
	bool GetWebMethod( int iSoapAction, KWebMethod& kInfo );


	//----------------------//
	// Process Event handle //
	//----------------------//
	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	// event handler
	DECL_ON_FUNC( EPUBLISHER_AUTHENTICATION_REQ );
	DECL_ON_FUNC( EPUBLISHER_SECURITY_AUTH_REQ );

	void testfunction();

protected:
	struct soap*						p_gf_soap;

	std::string							m_strWsdlPath;

	static std::map< int, KWebMethod >  ms_mapWebMethodPath;
};

#endif // SERV_COUNTRY_US
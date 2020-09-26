#pragma once


#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"
#include "KncException.h"


#ifdef SERV_COUNTRY_BR

#include "SoapLevelUp/soapH.h" // or whatever it is called, you must already have it

class KLevelUpAuth :public KSubThread
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
#   include "LevelUpAuth_def.h"
	};

	KLevelUpAuth(std::string WsdlFile);
	virtual ~KLevelUpAuth();



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

	void testfunction();


protected:

	inline void MakeUpperCase(char* str) 
	{
		for( char* i = str; (*i) != 0; i++ ) 
			*i = (char)toupper(*i);
	}

	inline void MakeUpperCase(std::string &str) 
	{
		for( std::string::iterator i = str.begin(); i != str.end(); i++ ) 
			*i = (char)toupper(*i);
	}

	struct soap* p_gf_soap;

	std::string m_strWsdlPath;

	static std::map< int, KWebMethod >  ms_mapWebMethodPath;
};

#endif // SERV_COUNTRY_BR
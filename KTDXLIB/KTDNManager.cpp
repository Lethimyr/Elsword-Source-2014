#include "StdAfx.h"
#include ".\ktdnmanager.h"

#ifdef CONVERSION_VS
#else CONVERSION_VS

CKTDNManager::CKTDNManager(void)
{
	// socket init
	WSADATA     wsaData;

	if( ::WSAStartup(MAKEWORD(2,2), &wsaData) != 0 ) 
	{     
		int errorVal =  ::WSAGetLastError();
		switch( errorVal )
		{
			case WSASYSNOTREADY:
				{
					ErrorLogMsg( KEM_ERROR115, L"WSASYSNOTREADY" );
				}
				break;

			case WSAVERNOTSUPPORTED:
				{
					ErrorLogMsg( KEM_ERROR115, L"WSAVERNOTSUPPORTED" );
				}
				break;

			case WSAEINPROGRESS:
				{
					ErrorLogMsg( KEM_ERROR115, L"WSAEINPROGRESS" );
				}
				break;

			case WSAEPROCLIM:
				{
					ErrorLogMsg( KEM_ERROR115, L"WSAEPROCLIM" );
				}
				break;

			case WSAEFAULT:
				{
					ErrorLogMsg( KEM_ERROR115, L"WSAEFAULT" );
				}
				break;

		}
		::WSACleanup();		
	}

	m_pKTDNDP = new CKTDNDP();
}

CKTDNManager::~CKTDNManager(void)
{
	SAFE_DELETE( m_pKTDNDP );

	::WSACleanup();
}

#endif CONVERSION_VS
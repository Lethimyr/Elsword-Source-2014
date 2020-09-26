#include "StdAfx.h"
#include "zlib.h"
#include ".\ktdndp.h"

#ifdef CONVERSION_VS
#else CONVERSION_VS

GUID g_guidApp = { 0x2ae835d, 0x9179, 0x485f, { 0x83, 0x43, 0x90, 0x1d, 0x32, 0x7c, 0xe7, 0x94 } };
CKTDNDP *CKTDNDP::s_KTDNDP = NULL;

CKTDNDP::CKTDNDP(void)
{
	//netlog << __FUNCTION__ << fileout;

	s_KTDNDP			= this;
	m_pTransfer			= NULL;

	m_bHost				= false;
	m_bConnection		= false;
	m_MaxPlayer			= 0;

	m_pMyUserDesc		= NULL;
	m_pHostUserDesc		= NULL;

	m_pDP				= NULL;

	InitializeCriticalSection( &m_DPNLock );
}

CKTDNDP::~CKTDNDP(void)
{	
	//netlog << __FUNCTION__ << fileout;

	CleanupDirectPlay();	
	s_KTDNDP = NULL;
}

HRESULT CKTDNDP::InitDirectPlay( CKTDNTransfer* pTransfer )
{
	HRESULT hr;

	CleanupDirectPlay();

	s_KTDNDP			= this;
	m_pTransfer			= pTransfer;

	m_bHost				= false;
	m_bConnection		= false;
	m_MaxPlayer			= 0;

	m_pMyUserDesc		= NULL;
	m_pHostUserDesc		= NULL;

	m_pDP				= NULL;

	InitializeCriticalSection( &m_DPNLock );

	// Create IDirectPlay8Peer
	hr = CoCreateInstance( CLSID_DirectPlay8Peer, NULL, 
							CLSCTX_INPROC_SERVER,
							IID_IDirectPlay8Peer, 
							(LPVOID*) &m_pDP );

	// Turn off parameter validation in release builds
#ifdef _DEBUG
	const DWORD dwInitFlags = 0;
#else
	const DWORD dwInitFlags = DPNINITIALIZE_DISABLEPARAMVAL;
#endif // _DEBUG

	// Init IDirectPlay8Peer
	hr = m_pDP->Initialize( NULL, DirectPlayMessageHandler, dwInitFlags );

	return S_OK;
}

void CKTDNDP::CleanupDirectPlay()
{
	//netlog << __FUNCTION__ << fileout;

	if( m_pDP )
		m_pDP->Close(0);
	SAFE_RELEASE(m_pDP);

	s_KTDNDP			= NULL;
	m_pTransfer			= NULL;

	m_bHost				= false;
	m_bConnection		= false;
	m_MaxPlayer			= 0;

	m_pMyUserDesc		= NULL;
	m_pHostUserDesc		= NULL;

	// Cleanup DirectPlay
	
	DeleteCriticalSection( &m_DPNLock );

	map<int,UserDesc*>::iterator iter;
	for( iter = m_UserMap.begin(); iter != m_UserMap.end(); iter++ )
	{
		UserDesc* pUserDesc = iter->second;
		SAFE_DELETE( pUserDesc );
	}
	m_UserMap.clear();

	map<DPNID,UserDesc*>::iterator iter2;
	for( iter2 = m_UserMapByDPNID.begin(); iter2 != m_UserMapByDPNID.end(); iter2++ )
	{
		UserDesc* pUserDesc = iter2->second;
		SAFE_DELETE( pUserDesc );
	}
	m_UserMapByDPNID.clear();	
}

HRESULT CKTDNDP::OpenHost( const WCHAR* pSessionName, const int userID, const int port, const int maxPlayer )
{
	//netlog << __FUNCTION__ << fileout;

	m_bHost			= true;
	m_bConnection	= false;
	m_SessionName	= pSessionName;
	m_MaxPlayer		= maxPlayer;

	HRESULT hr;

	IDirectPlay8Address*   pDP8AddressHost  = NULL;
	// Create the local host address object
	hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
							CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address,
							(LPVOID*) &pDP8AddressHost );

	hr = pDP8AddressHost->SetSP( &CLSID_DP8SP_TCPIP );

	DWORD dwPort = port;
	hr = pDP8AddressHost->AddComponent( DPNA_KEY_PORT, 
										&dwPort, 
										sizeof(dwPort),
										DPNA_DATATYPE_DWORD );

	WCHAR strUserID[50] = {0,};
	_itow( userID, strUserID, 10 );

	DPN_PLAYER_INFO dpPlayerInfo;
	ZeroMemory( &dpPlayerInfo, sizeof(DPN_PLAYER_INFO) );
	dpPlayerInfo.dwSize			= sizeof(DPN_PLAYER_INFO);
	dpPlayerInfo.dwInfoFlags	= DPNINFO_NAME;
	dpPlayerInfo.pwszName		= strUserID;

	if( m_pDP == NULL )
	{
		string strName;
		ConvertWCHARToChar( strName, strUserID );
		ErrorLogMsg( KEM_ERROR63, strName.c_str());

		return E_FAIL;
	}

	hr = m_pDP->SetPeerInfo( &dpPlayerInfo, NULL, NULL, DPNSETPEERINFO_SYNC );

	// Setup the application desc
	DPN_APPLICATION_DESC dnAppDesc;
	ZeroMemory( &dnAppDesc, sizeof(DPN_APPLICATION_DESC) );
	dnAppDesc.dwSize			= sizeof(DPN_APPLICATION_DESC);
	dnAppDesc.guidApplication	= g_guidApp;
	dnAppDesc.pwszSessionName	= (WCHAR*)pSessionName;
	dnAppDesc.dwMaxPlayers		= m_MaxPlayer;
	dnAppDesc.dwFlags			= 0;
	dnAppDesc.dwFlags			|= DPNSESSION_MIGRATE_HOST;
	dnAppDesc.dwFlags			|= DPNSESSION_NODPNSVR;

	hr = m_pDP->Host( &dnAppDesc,               // the application desc
						&pDP8AddressHost,         // array of addresses of the local devices used to connect to the host
						1,                        // number in array
						NULL, NULL,               // DPN_SECURITY_DESC, DPN_SECURITY_CREDENTIALS
						NULL,                     // player context
						DPNHOST_OKTOQUERYFORADDRESSING );

	SAFE_RELEASE( pDP8AddressHost );

	return hr;
}

HRESULT CKTDNDP::ConnectToHost( const WCHAR* pSessionName, const int userID, const WCHAR* pIP, const int port )
{
	//netlog << __FUNCTION__ << fileout;

	m_bHost			= false;
	m_bConnection	= false;
	m_SessionName	= pSessionName;

	HRESULT hr = S_OK;

	IDirectPlay8Address* pDP8DeviceAddress	= CreateDeviceAddress( port );
	IDirectPlay8Address* pDP8HostAddress	= CreateHostAddress( (WCHAR*)pIP, port );

	WCHAR strUserID[50] = {0,};
	_itow( userID, strUserID, 10 );

	DPN_PLAYER_INFO dpPlayerInfo;
	ZeroMemory( &dpPlayerInfo, sizeof(DPN_PLAYER_INFO) );
	dpPlayerInfo.dwSize			= sizeof(DPN_PLAYER_INFO);
	dpPlayerInfo.dwInfoFlags	= DPNINFO_NAME;
	dpPlayerInfo.pwszName		= strUserID;

	DPN_APPLICATION_DESC    dpnAppDesc;
	ZeroMemory( &dpnAppDesc, sizeof(DPN_APPLICATION_DESC) );
	dpnAppDesc.dwSize          = sizeof(DPN_APPLICATION_DESC);
	dpnAppDesc.guidApplication = g_guidApp;

	hr = m_pDP->SetPeerInfo( &dpPlayerInfo, NULL, NULL, DPNSETPEERINFO_SYNC );

	DPNHANDLE hConnectAsyncOp;
	hr = m_pDP->Connect( &dpnAppDesc,       // the application desc
						pDP8HostAddress,      // address of the host of the session
						pDP8DeviceAddress,    // address of the local device the enum responses were received on
						NULL, NULL,                          // DPN_SECURITY_DESC, DPN_SECURITY_CREDENTIALS
						NULL, 0,                             // user data, user data size
						NULL,                                // player context,
						NULL, &hConnectAsyncOp,            // async context, async handle,
						NULL ); // flags

	SAFE_RELEASE( pDP8HostAddress );
	SAFE_RELEASE( pDP8DeviceAddress );

	return hr;
}

HRESULT CKTDNDP::Disconnect( const int userID )
{
	//netlog << __FUNCTION__ << fileout;


	EnterCriticalSection( &m_DPNLock );

	if( m_bHost == false || m_bConnection == false )
	{
		LeaveCriticalSection( &m_DPNLock );

		ErrorLog(KEM_ERROR64);

		return E_FAIL;
	}

	DPNID	dpnID = GetDPNID(userID);
	DWORD   dwDestroyData = 1;
	HRESULT hr = m_pDP->DestroyPeer( dpnID, &dwDestroyData, sizeof(DWORD), 0 );

	LeaveCriticalSection( &m_DPNLock );

	return S_OK;
}

HRESULT CKTDNDP::SendHost( void* pBuffer, int size )
{
	//netlog << __FUNCTION__ << fileout;

	EnterCriticalSection( &m_DPNLock );

	if( m_bConnection == false )
	{
		LeaveCriticalSection( &m_DPNLock );
		return E_FAIL;
	}

	HRESULT hr = Send( m_pMyUserDesc->dpnID, pBuffer, size );
	
	LeaveCriticalSection( &m_DPNLock );

	return hr;
}

HRESULT CKTDNDP::Send( const int userID, void* pBuffer, int size )
{
	//netlog << __FUNCTION__ << fileout;

	EnterCriticalSection( &m_DPNLock );

	if( m_bConnection == false )
	{
		LeaveCriticalSection( &m_DPNLock );
		return E_FAIL;
	}

	DPN_BUFFER_DESC bufferDesc;
	bufferDesc.dwBufferSize = size;
	bufferDesc.pBufferData  = (BYTE*)pBuffer;

	DPNHANDLE	hAsync;
	DPNID		dpnID = GetDPNID(userID);


	unsigned long compressSize = 2000;
	unsigned char compressBuffer[2000] = {0,};
	if(Z_OK	!= compress( (BYTE*)compressBuffer, &compressSize, (BYTE*)pBuffer, size ) )
	{
		LeaveCriticalSection( &m_DPNLock );
		return E_FAIL;
	}
	bufferDesc.dwBufferSize = (int)compressSize;
	bufferDesc.pBufferData  = (BYTE*)compressBuffer;

	HRESULT		hr = m_pDP->SendTo( dpnID, &bufferDesc, 1, 0, NULL, &hAsync, DPNSEND_GUARANTEED );

	LeaveCriticalSection( &m_DPNLock );

	return hr;
}

HRESULT CKTDNDP::BroadCast( void* pBuffer, int size )
{
	EnterCriticalSection( &m_DPNLock );

	if( m_bConnection == false )
	{
		LeaveCriticalSection( &m_DPNLock );
		return E_FAIL;
	}

	DPN_BUFFER_DESC bufferDesc;
	bufferDesc.dwBufferSize = size;
	bufferDesc.pBufferData  = (BYTE*)pBuffer;

	unsigned long compressSize = 2000;
	unsigned char compressBuffer[2000] = {0,};
	if(Z_OK	!= compress( (BYTE*)compressBuffer, &compressSize, (BYTE*)pBuffer, size ) )
	{
		LeaveCriticalSection( &m_DPNLock );
		return E_FAIL;
	}
	bufferDesc.dwBufferSize = (int)compressSize;
	bufferDesc.pBufferData  = (BYTE*)compressBuffer;

	DPNHANDLE hAsync;
	HRESULT	hr = m_pDP->SendTo( DPNID_ALL_PLAYERS_GROUP, &bufferDesc, 1, 0, NULL, &hAsync, DPNSEND_GUARANTEED );

	LeaveCriticalSection( &m_DPNLock );

	return hr;
}

HRESULT CKTDNDP::Receive( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer )
{
	HRESULT hr;

	EnterCriticalSection( &m_DPNLock );
	if( m_pDP != NULL && m_pTransfer != NULL && s_KTDNDP == this )
	{
		switch( dwMessageId )
		{
			case DPN_MSGID_CREATE_PLAYER:
			{			
				//netlog << "DPN_MSGID_CREATE_PLAYER" << fileout;

				PDPNMSG_CREATE_PLAYER pCreatePlayerMsg;
				pCreatePlayerMsg = (PDPNMSG_CREATE_PLAYER)pMsgBuffer;

				// Get the peer info and extract its name
				DWORD dwSize = 0;
				DPN_PLAYER_INFO* pdpPlayerInfo = NULL;
				hr = DPNERR_CONNECTING;

				// GetPeerInfo might return DPNERR_CONNECTING when connecting, 
				// so just keep calling it if it does
				while( hr == DPNERR_CONNECTING ) 
					hr = m_pDP->GetPeerInfo( pCreatePlayerMsg->dpnidPlayer, pdpPlayerInfo, &dwSize, 0 );                                

				if( hr == DPNERR_BUFFERTOOSMALL )
				{
					pdpPlayerInfo = (DPN_PLAYER_INFO*) new BYTE[ dwSize ];

					ZeroMemory( pdpPlayerInfo, dwSize );
					pdpPlayerInfo->dwSize = sizeof(DPN_PLAYER_INFO);

					hr = m_pDP->GetPeerInfo( pCreatePlayerMsg->dpnidPlayer, pdpPlayerInfo, &dwSize, 0 );
					if( SUCCEEDED(hr) )
					{
						
						UserDesc* pUserDesc		= new UserDesc();
						pUserDesc->dpnID		= pCreatePlayerMsg->dpnidPlayer;
						pUserDesc->userID		= _wtoi( pdpPlayerInfo->pwszName );

						if( pdpPlayerInfo->dwPlayerFlags & DPNPLAYER_LOCAL )
						{
							pUserDesc->bLocal	= true;
							m_pMyUserDesc		= pUserDesc;
						}
						else
							pUserDesc->bLocal	= false;

						if( pdpPlayerInfo->dwPlayerFlags & DPNPLAYER_HOST )
						{
							pUserDesc->bHost	= true;
							m_pHostUserDesc		= pUserDesc;
						}
						else
							pUserDesc->bHost	= false;										
						
						m_UserMap.insert( std::make_pair( pUserDesc->userID, pUserDesc ) );
						m_UserMapByDPNID.insert( std::make_pair( pUserDesc->dpnID, pUserDesc ) );					

						if( pUserDesc->bHost == true && pUserDesc->bLocal == true )
						{
							CKTDNPacket::SPT_OPEN_HOST sPT_OPEN_HOST;
							sPT_OPEN_HOST.hostID = pUserDesc->userID;
							m_pTransfer->Receive( GetMyID(), sPT_OPEN_HOST.hostID, sizeof(CKTDNPacket::SPT_OPEN_HOST), &sPT_OPEN_HOST );

							m_bConnection = true;
						}
						else if( pUserDesc->bLocal == false )
						{
							CKTDNPacket::SPT_NEW_CONNECT sPT_NEW_CONNECT;
							sPT_NEW_CONNECT.newUserID = pUserDesc->userID;
							m_pTransfer->Receive( GetMyID(), sPT_NEW_CONNECT.newUserID, sizeof(CKTDNPacket::SPT_NEW_CONNECT), &sPT_NEW_CONNECT );
						}	
					}

					SAFE_DELETE_ARRAY( pdpPlayerInfo );
				}
				break;
			}

			case DPN_MSGID_DESTROY_PLAYER:
			{
				//netlog << "DPN_MSGID_DESTROY_PLAYER" << fileout;

				PDPNMSG_DESTROY_PLAYER pDestroyPlayerMsg;
				pDestroyPlayerMsg = (PDPNMSG_DESTROY_PLAYER)pMsgBuffer;
		
				
				DeleteUserDescByDPNID( pDestroyPlayerMsg->dpnidPlayer );

				CKTDNPacket::SPT_DESTROY_CONNECT sPT_DESTROY_CONNECT;
				sPT_DESTROY_CONNECT.destroyUserID = GetID( pDestroyPlayerMsg->dpnidPlayer );
				m_pTransfer->Receive( GetMyID(), sPT_DESTROY_CONNECT.destroyUserID, sizeof(CKTDNPacket::SPT_DESTROY_CONNECT), &sPT_DESTROY_CONNECT );

				break;
			}

			case DPN_MSGID_CONNECT_COMPLETE:
			{
				//netlog << "DPN_MSGID_CONNECT_COMPLETE" << fileout;
				
				PDPNMSG_CONNECT_COMPLETE pConnectCompleteMsg;
				pConnectCompleteMsg = (PDPNMSG_CONNECT_COMPLETE)pMsgBuffer;

				if( pConnectCompleteMsg->dpnidLocal == m_pMyUserDesc->dpnID )
				{
					CKTDNPacket::SPT_CONNECT_COMPLETE sPT_CONNECT_COMPLETE;
					sPT_CONNECT_COMPLETE.userID = m_pMyUserDesc->userID;
					m_pTransfer->Receive( GetMyID(), sPT_CONNECT_COMPLETE.userID, sizeof(CKTDNPacket::SPT_CONNECT_COMPLETE), &sPT_CONNECT_COMPLETE );

					m_bConnection = true;
				}

				break;
			}

			case DPN_MSGID_HOST_MIGRATE:
			{
				//netlog << "DPN_MSGID_HOST_MIGRATE" << fileout;

				PDPNMSG_HOST_MIGRATE pHostMigrateMsg;
				pHostMigrateMsg = (PDPNMSG_HOST_MIGRATE)pMsgBuffer;

				UserDesc* pNewHost		= GetUserDescByDPNID( pHostMigrateMsg->dpnidNewHost );
				pNewHost->bHost			= true;
				m_pHostUserDesc->bHost	= false;
				m_pHostUserDesc			= pNewHost;

				CKTDNPacket::SPT_HOST_MIGRATE sPT_HOST_MIGRATE;
				sPT_HOST_MIGRATE.newHostID = GetID( pHostMigrateMsg->dpnidNewHost );
				m_pTransfer->Receive( GetMyID(), sPT_HOST_MIGRATE.newHostID, sizeof(CKTDNPacket::SPT_HOST_MIGRATE), &sPT_HOST_MIGRATE );

				break;
			}

			case DPN_MSGID_RECEIVE:
			{
				PDPNMSG_RECEIVE pReceiveMsg;
				pReceiveMsg = (PDPNMSG_RECEIVE)pMsgBuffer;

				unsigned long uncompressSize = 2000;
				unsigned char uncompressBuffer[2000] = {0,};
				uncompress( (BYTE*)uncompressBuffer, &uncompressSize, (BYTE*)pReceiveMsg->pReceiveData, pReceiveMsg->dwReceiveDataSize );

				CKTDNPacket::SPT_BASIC* pSPT_BASIC = (CKTDNPacket::SPT_BASIC*)uncompressBuffer;
				int senderID = GetID(pReceiveMsg->dpnidSender);
				m_pTransfer->Receive( GetMyID(), senderID, (int)uncompressSize, pSPT_BASIC );
				
				break;
			}
		}
	}
	LeaveCriticalSection( &m_DPNLock );

	return S_OK;
}

DPNID CKTDNDP::GetDPNID( const int userID )
{
	if( m_bConnection == false )
		return NULL;

	DPNID dpnID = NULL;

	map<int,UserDesc*>::iterator iter;
	iter = m_UserMap.find( userID );
	if( iter != m_UserMap.end() )
	{
		UserDesc* pUserDesc = iter->second;
		dpnID = pUserDesc->dpnID;
	}

	return dpnID;
}

const int CKTDNDP::GetID( DPNID dpnID )
{
	map<DPNID,UserDesc*>::iterator iter;
	iter = m_UserMapByDPNID.find( dpnID );
	if( iter != m_UserMapByDPNID.end() )
	{
		UserDesc* pUserDesc = iter->second;
		return pUserDesc->userID;
	}

	return -1;
}

DPNID CKTDNDP::GetMyDPNID()
{
	if( m_bConnection == false )
		return NULL;

	DPNID dpnID = NULL;
	dpnID = m_pMyUserDesc->dpnID;

	return dpnID;
}

DPNID CKTDNDP::GetHostDPNID()
{
	if( m_bConnection == false )
		return NULL;

	DPNID dpnID = NULL;
	dpnID = m_pHostUserDesc->dpnID;

	return dpnID;
}

CKTDNDP::UserDesc* CKTDNDP::GetUserDesc( const int userID )
{
	UserDesc* pUserDesc = NULL;

	map<int,UserDesc*>::iterator iter;
	iter = m_UserMap.find( userID );
	if( iter != m_UserMap.end() )
	{
		pUserDesc = iter->second;
	}

	return pUserDesc;
}

CKTDNDP::UserDesc* CKTDNDP::GetUserDescByDPNID( DPNID dpnID )
{
	UserDesc* pUserDesc = NULL;

	map<DPNID,UserDesc*>::iterator iter;
	iter = m_UserMapByDPNID.find( dpnID );
	if( iter != m_UserMapByDPNID.end() )
	{
		pUserDesc = iter->second;
	}

	return pUserDesc;
}

bool CKTDNDP::DeleteUserDesc( const int userID )
{
	map<int,UserDesc*>::iterator iter;
	iter = m_UserMap.find( userID );
	if( iter != m_UserMap.end() )
	{
		UserDesc* pUserDesc = iter->second;

		map<DPNID,UserDesc*>::iterator iter2;
		iter2 = m_UserMapByDPNID.find( pUserDesc->dpnID );
		if( iter2 != m_UserMapByDPNID.end() )
		{
			m_UserMapByDPNID.erase( iter2 );
		}

		SAFE_DELETE( pUserDesc );
		m_UserMap.erase( iter );
		return true;
	}
	return false;
}

bool CKTDNDP::DeleteUserDescByDPNID( DPNID dpnID )
{
	map<DPNID,UserDesc*>::iterator iter;
	iter = m_UserMapByDPNID.find( dpnID );
	if( iter != m_UserMapByDPNID.end() )
	{
		UserDesc* pUserDesc = iter->second;

		map<int,UserDesc*>::iterator iter2;
		iter2 = m_UserMap.find( pUserDesc->userID );
		if( iter2 != m_UserMap.end() )
		{
			m_UserMap.erase( iter2 );
		}

		SAFE_DELETE( pUserDesc );
		m_UserMapByDPNID.erase( iter );
		return true;
	}
	return false;
}





///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

IDirectPlay8Address* CKTDNDP::CreateDeviceAddress( int port )
{
	HRESULT hr = S_OK;

	IDirectPlay8Address* pAddress = NULL;

	// safely create a device address
	hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER,
							IID_IDirectPlay8Address, (LPVOID*)&pAddress );

	// set service provider of device
	hr = pAddress->SetSP( &CLSID_DP8SP_TCPIP );

	/*
	DWORD dwPort = port;
	hr = pAddress->AddComponent( DPNA_KEY_PORT, 
								&dwPort, 
								sizeof(dwPort),
								DPNA_DATATYPE_DWORD );
								*/

	return pAddress;
}

IDirectPlay8Address* CKTDNDP::CreateHostAddress( WCHAR* IP, int port )
{
	HRESULT hr = S_OK;

	IDirectPlay8Address* pAddress = NULL;

	// Create a host address
	hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER,
							IID_IDirectPlay8Address, (LPVOID*)&pAddress );

	// set service provider of host
	hr = pAddress->SetSP( &CLSID_DP8SP_TCPIP );

	// add components to host address
	hr = pAddress->AddComponent( DPNA_KEY_HOSTNAME,						// component name
										IP,								// data buffer
										(DWORD)((wcslen(IP)+1) * sizeof(WCHAR)), // data size
										DPNA_DATATYPE_STRING );			// data type

	DWORD dwPort = port;
	hr = pAddress->AddComponent( DPNA_KEY_PORT,					// component name
										&dwPort,				// data buffer
										sizeof(DWORD),          // data size
										DPNA_DATATYPE_DWORD );  // dwDataType

	return pAddress;
}

#endif CONVERSION_VS
#pragma once

#ifdef CONVERSION_VS
//사용되지 않는 클래스
#else CONVERSION_VS
class CKTDNDP
{
	public:
		struct UserDesc
		{
			int		userID;
			DPNID	dpnID;
			bool	bLocal;
			bool	bHost;

			UserDesc()
			{
				userID	= 0;
				dpnID	= NULL;
				bLocal	= false;
				bHost	= false;
			}
		};

		struct DPHostEnumInfo
		{
			DWORD					dwRef;
			DPN_APPLICATION_DESC*	pAppDesc;
			IDirectPlay8Address*	pHostAddr;
			IDirectPlay8Address*	pDeviceAddr;
			TCHAR					szSession[MAX_PATH];
			DWORD					dwLastPollTime;
			BOOL					bValid;
			DPHostEnumInfo*			pNext;
		};

	public:
		CKTDNDP(void);
		~CKTDNDP(void);

		HRESULT InitDirectPlay( CKTDNTransfer* pTransfer );
		void	CleanupDirectPlay();

		HRESULT OpenHost( const WCHAR* pSessionName, const int userID, const int port, const int maxPlayer = 8 );
		HRESULT ConnectToHost( const WCHAR* pSessionName, const int userID, const WCHAR* pIP, const int port );
		HRESULT Disconnect( const int userID );

		HRESULT SendHost( void* pBuffer, int size );
		HRESULT Send( const int userID, void* pBuffer, int size );
		HRESULT BroadCast( void* pBuffer, int size );
		HRESULT Receive( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer );

		static CKTDNDP* s_KTDNDP;

	private:
		const int	GetMyID()
		{
			if(	m_pMyUserDesc != NULL )
				return m_pMyUserDesc->userID; 
			else
				return -1;
		}
		DPNID		GetDPNID( const int userID );
		const int	GetID( DPNID dpnID );
		DPNID		GetMyDPNID();
		DPNID		GetHostDPNID();

		UserDesc*	GetUserDesc( const int userID );
		UserDesc*	GetUserDescByDPNID( DPNID dpnID );
		UserDesc*	GetMyUserDesc(){ return m_pMyUserDesc; }
		UserDesc*	GetHostUserDesc(){ return m_pHostUserDesc; }

		bool		DeleteUserDesc( const int userID );
		bool		DeleteUserDescByDPNID( DPNID dpnID );

		IDirectPlay8Address* CreateDeviceAddress( int port );
		IDirectPlay8Address* CreateHostAddress( WCHAR* IP, int port );

		static HRESULT WINAPI DirectPlayMessageHandler(PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer)
		{
			return s_KTDNDP->Receive( pvUserContext, dwMessageId, pMsgBuffer );
		}

		bool					m_bHost;
		bool					m_bConnection;
		wstring					m_SessionName;
		int						m_MaxPlayer;

		UserDesc*				m_pMyUserDesc;
		UserDesc*				m_pHostUserDesc;
		map<int,UserDesc*>		m_UserMap;
		map<DPNID,UserDesc*>	m_UserMapByDPNID;
		
		CKTDNTransfer*			m_pTransfer;
		CRITICAL_SECTION		m_DPNLock;
		IDirectPlay8Peer*		m_pDP;
};

#endif CONVERSION_VS
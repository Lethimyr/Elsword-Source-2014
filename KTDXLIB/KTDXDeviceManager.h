#pragma once


struct CKTDXDeviceData
{
	CKTDXDevice*				m_pDevice;
	//bool						m_bLoad;

	CKTDXDeviceData()
	{
		m_pDevice	= NULL;
		//m_bLoad		= false;
	}
	CKTDXDeviceData( const CKTDXDeviceData& rhs_ );
	CKTDXDeviceData& operator = ( const CKTDXDeviceData& rhs_ );

	~CKTDXDeviceData();
};

typedef std::list<CKTDXDeviceData>   CKTDXDeviceDataList;
class CKTDGObject;

//{{ seojt // 2008-10-14, 16:07
/** background loading을 지원하기 위해 KJohnThread을 상속받도록 수정
    - jintaeks on 2008-10-14, 16:07 */
class CKTDXDeviceManager :
    public CKTDXStage,
    public KJohnThread
//}} seojt // 2008-10-14, 16:07
{
	public:

#ifdef KHELP_SYSTEM_TEST // 2008-10-24
        DECLARE_HELP_SYSTEM( CKTDXDeviceManager );
#endif // KHELP_SYSTEM_TEST // 2008-10-24

        enum    EPriority
        {
            PRIORITY_BLOCKING = 0,
            PRIORITY_HIGH = 1,
            PRIORITY_LOW = 2,
            PRIORITY_NUM
        };//enum    EPriority



#ifdef NEW_CHECK_OPENED_DEVICE

		struct OpenDeviceData
		{
			int iRefCount;
			unsigned int iSize;
			bool bChecked;

			OpenDeviceData()
			{
				iRefCount = 0;
				iSize = 0;
				bChecked = false;
			}
		};

		struct OpenDeviceListData
		{
			int nCount;
			unsigned int iSize;

			OpenDeviceListData()
			{
				nCount = 0;
				iSize = 0;
			}
		};

		void SaveOpenedDeviceListToFile();
#endif NEW_CHECK_OPENED_DEVICE


	public:
		CKTDXDeviceManager( IDirect3DDevice9* pd3dDevice );
		virtual ~CKTDXDeviceManager(void);

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		void    ProcessForegroundQueue( float fElapsedTime );

        void    ResumeThread();

        bool    BeginFullSync();
        bool    EndFullSync();

		void SetTextureDetailLevel( CKTDXDeviceTexture::DETAIL_LEVEL detailLevel );
		void AddHighTexName( const WCHAR* highTexName ){ m_HighTexName.push_back( highTexName ); }

		CKTDXDeviceTexture*				OpenTexture( wstring deviceID, D3DFORMAT texFormat = D3DFMT_UNKNOWN, bool bAlwaysHigh = false 
										#ifdef ADD_RESOURCE_ERROR_LOG
													, bool bLog = true
										#endif // ADD_RESOURCE_ERROR_LOG
													);
		CKTDXDeviceXSkinMesh*			OpenXSkinMesh( wstring deviceID, wstring moveBoneName = L"Bip01" );
		CKTDXDeviceXMesh*               OpenXMesh( wstring deviceID );
		CKTDXDeviceRenderTargetTexture*	OpenRenderTargetTexture( wstring deviceID, int width = 128, int height = 128, D3DFORMAT fmt = D3DFMT_UNKNOWN );
		CKTDXDeviceSound*				OpenSound( wstring deviceID, int bufCount = 10, bool bUse3D = true, float fMax3DDistance = -1.f );
#ifdef CHECK_SOUND_LOADING_TIME
		CKTDXDeviceSound*				CheckOpenSound( bool& bCreateSound, wstring deviceID, int bufCount = 10, bool bUse3D = true, float fMax3DDistance = -1.f );
#endif // CHECK_SOUND_LOADING_TIME

		CKTDXDeviceXET*					OpenXET( wstring deviceID, bool bWithoutFile = false );
		


        CKTDXDeviceTexture*             OpenTextureInBackground( wstring deviceID, EPriority ePriority = PRIORITY_HIGH, D3DFORMAT texFormat = D3DFMT_UNKNOWN, bool bAlwaysHigh = false );
        CKTDXDeviceXSkinMesh*			OpenXSkinMeshInBackground( wstring deviceID, EPriority ePriority = PRIORITY_HIGH, const WCHAR* moveBoneName = L"Bip01" );
        CKTDXDeviceXMesh*               OpenXMeshInBackground( wstring deviceID, EPriority ePriority = PRIORITY_HIGH );
		CKTDXDeviceXET*					OpenXETInBackground( wstring deviceID, EPriority ePriority = PRIORITY_HIGH, bool bWithoutFile = false );
		CKTDXDeviceSound*				OpenSoundInBackground( wstring deviceID, 
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
			EPriority ePriority = PRIORITY_HIGH, 
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
			int bufCount = 10, bool bUse3D = true );




		void CloseDeviceDirectory( wstring wstrPath );
		void OpenDeviceDirectory( wstring wstrPath );

		CKTDXDeviceSound* PlaySound( const WCHAR* pFileName, bool bLoop = false, bool b3DSound = true );
		void StopSound( const WCHAR* pFileName );

#ifdef CHECK_VOICE_IN_SLIDESHOT
		bool IsPlaying( const WCHAR* pFileName );
#endif //CHECK_VOICE_IN_SLIDESHOT

		void CloseDevice( wstring deviceID )
		{
            return  CloseDevice( deviceID.c_str() );
        }
		
        void    CloseDevice( const WCHAR* deviceID );

		KGCMassFileManager* GetMassFileManager(){ return &m_MassFileManager; }
		char*				GetPullPath( char* cFileName ){ return m_MassFileManager.GetPullPath( cFileName ); }
		WCHAR*				GetPullPath( WCHAR* wcFileName ){ return m_MassFileManager.GetPullPath( wcFileName ); }

		//CRITICAL_SECTION*	GetDeviceLock(){ return &m_DeviceLock; }

		//{{ 2007. 11. 12  최육사  device name 출력
		void GetDeviceNameAndRefCount( std::vector< std::pair< wstring, std::pair< int, int > > >& vecLoadedDeviceInfo );		
		//}}


		//void CloseCachedTextures();
		//void CloseCachedXMeshes();
		//void CloseCachedXSkinMeshes();

//		UINT  GetPendingResourceCount();

//#ifdef	X2OPTIMIZE_REFACTORING_RESOURCE_BACKGROUND_LOAD
		void			AppendXSkinMesh( CKTDXDeviceDataList& listOut_, const WCHAR* pName_, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
		void			AppendXMesh( CKTDXDeviceDataList& listOut_, const WCHAR* pName_, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
		void			AppendTexture( CKTDXDeviceDataList& listOut_, const WCHAR* pName_, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
		void			AppendXET( CKTDXDeviceDataList& listOut_, const WCHAR* pName_, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
		void			AppendSound( CKTDXDeviceDataList& listOut_, const WCHAR* pName_ );
		void			AppendXSkinMesh( CKTDXDeviceDataList& listOut_, const std::wstring& wstrName_, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
		void			AppendXMesh( CKTDXDeviceDataList& listOut_, const std::wstring& wstrName_, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
		void			AppendTexture( CKTDXDeviceDataList& listOut_, const std::wstring& wstrName_, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
		void			AppendXET( CKTDXDeviceDataList& listOut_, const std::wstring& wstrName_, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
		void			AppendSound( CKTDXDeviceDataList& listOut_, const std::wstring& wstrName_ );
//#endif	X2OPTIMIZE_REFACTORING_RESOURCE_BACKGROUND_LOAD

//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
		void	ReleaseAllMemoryBuffers();
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

	private:

        void    LoadAndCloseDevice( CKTDXDevice* pDevice_
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
				, bool bBackgroundQueueing = false
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
			);

		CKTDXDevice*    CreateAndOpenDevice( CKTDXDevice* pDevice );
#ifdef CHECK_SOUND_LOADING_TIME
		CKTDXDevice*    CheckCreateAndOpenDevice( bool& bCreateSound, CKTDXDevice* pDevice );
#endif // CHECK_SOUND_LOADING_TIME
		//void DestroyDevice( const char* deviceID );
		//void DestroyDevice( CKTDXDevice* pDevice );

        CKTDXDevice*    OpenDevice( wstring deviceID, CKTDXDevice::DEVICETYPE eType );
		CKTDXDevice*    CreateAndPendDevice( CKTDXDevice* pDevice, EPriority ePriority_ = PRIORITY_HIGH );
        CKTDXDevice*    PendDevice( const WCHAR* deviceID, CKTDXDevice::DEVICETYPE eType, EPriority ePriority_ = PRIORITY_HIGH );
        /// KJohnThread에 구현된, RunThread() 멤버함수의 overloading
        /// background resource loading을 담당한다.
        virtual DWORD   RunThread();
        void            _InitializeThreadRelatedObject();
        void            _FinalizeThreadRelatedObject();
        /// thread를 suspend 시킨다.
        void            _CheckFullSyncInDeviceThread();
    public:
//#ifdef NEW_VILLAGE_UNIT_BACKGROUND_LOADING_TEST
		bool			IsValidDevice( CKTDXDevice* pDevice )
		{
			if( NULL == pDevice )
				return false;

			CSLock  locker( m_DeviceLock );

			map<wstring, CKTDXDevice*>::const_iterator iter = m_DeviceMap.find( pDevice->GetDeviceID() );
			if ( iter == m_DeviceMap.end() || iter->second != pDevice )
				return false;			

			return true;
		}
		bool			IsValidDevice( const wstring& deviceID )
		{
			if( true == deviceID.empty() )
				return false;

			CSLock locker( m_DeviceLock );
			map<wstring, CKTDXDevice*>::const_iterator iter = m_DeviceMap.find( deviceID );
			if ( iter == m_DeviceMap.end() || iter->second == NULL )
				return false;

			return true;
		}
//#endif NEW_VILLAGE_UNIT_BACKGROUND_LOADING_TEST




    public:
        void            Debug_RenderHelp( int ix_, int iy_ );
    private:
        enum    EThreadEventType
        {
            TET_PEND_DEVICE,
            TET_KILL_THREAD,
            TET_MAX
        };//

        HANDLE                      m_ahThreadEvents[TET_MAX];

        struct  PendQueue
        {
            MemberCriticalSection   m_csPendingDevice;
            CKTDXDevicePtrList      m_listPendingDevice;
        };//struct  PendQueue

        PendQueue                   m_aPendQueue[PRIORITY_NUM];

        volatile LONG               m_lFullSyncStatus;
        HANDLE                      m_hFullSyncEventForMainThread;
        HANDLE                      m_hFullSyncEventForDeviceThread;

        int                         m_iForegroundLoadCounter;





    private:
		CRITICAL_SECTION					m_DeviceLock;
        typedef std::map<wstring, CKTDXDevice*>  DeviceMap;
		DeviceMap		                    m_DeviceMap;

		KGCMassFileManager					m_MassFileManager;
		CKTDXDeviceTexture::DETAIL_LEVEL	m_TexDetailLevel;
		vector<wstring>						m_HighTexName;





#ifdef NEW_CHECK_OPENED_DEVICE

		typedef std::map< wstring, OpenDeviceData >	OpenDeviceMap;
		OpenDeviceMap					m_mapOpenDeviceOld;
		OpenDeviceMap					m_mapOpenDevice;

		OpenDeviceListData				m_OpenDeviceListDataOld;
		OpenDeviceListData				m_OpenDeviceListData;

#endif NEW_CHECK_OPENED_DEVICE

#ifdef ADD_RESOURCE_ERROR_LOG
		set<wstring>					m_setEmptyDeviceID;
#endif //ADD_RESOURCE_ERROR_LOG

#ifdef  X2OPTIMIZE_COOPERATIVE_BACKGROUND_RESOURCE_LOADING
        DWORD                           m_dwBackgroundThreadLoadSize;
#endif  X2OPTIMIZE_COOPERATIVE_BACKGROUND_RESOURCE_LOADING

};//class CKTDXDeviceManager : public CKTDXStage, public KJohnThread











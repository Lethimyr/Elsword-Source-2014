#pragma once

//#ifdef INTRUDE_THREAD_LOADING
//
//class CX2UnitLoader : public KJohnThread
//{
//	public:
//		struct DeviceData
//		{
//			CKTDXDevice::DEVICETYPE 	m_DeviceType;
//			wstring						m_DeviceID;
//			CKTDXDevice*				m_pDevice;
//			bool						m_bLoad;
//
//			DeviceData()
//			{
//				m_pDevice	= NULL;
//				m_bLoad		= false;
//			}
//			~DeviceData();
//		};
//		struct LoadUserUnitData
//		{
//			UidType					m_UnitUID;
//			vector<DeviceData*>		m_listKTDXDeviceData;
//			bool					m_bCheckThread;
//			bool					m_bComplete;
//
//			LoadUserUnitData()
//			{
//				m_UnitUID	= 0;
//				m_bCheckThread = false;
//				m_bComplete	= false;
//			}
//			~LoadUserUnitData();
//		};
//
//	public:
//		CX2UnitLoader(void);
//		virtual ~CX2UnitLoader(void);
//
//		void				OnFrameMove( float fElapsedTime );
//
//		void				AddLoadData( CX2Unit* pUnit );
//		bool				IsLoadComplete( UidType unitUID );
//		void				DeleteLoadData( UidType unitUID );
//		void				ClearLoadData();
//
//		DWORD				RunThread();
//
//	protected:
//		CKTDXDevice*		LoadDevice( CKTDXDevice::DEVICETYPE m_DeviceType, wstring m_DeviceID );
//		
//		float								m_fElapsedTime;
//		int									m_RenderCount;
//		map<UidType,LoadUserUnitData*>		m_LoadUnitDataMap;
//
//		CRITICAL_SECTION					m_csUnitLoader;
//		bool								m_bLoadingThread;
//};
//#else

class CX2UnitLoader
{


public:

	CX2UnitLoader(void);
	virtual ~CX2UnitLoader(void);

    void    Initialize();
    void    Finalize();

	void				OnFrameMove( float fElapsedTime );

	void				AddUnitLoadDataForGame( CX2Unit* pUnit
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
						, bool bAddPetResource
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
						);
	void				AddUnitLoadDataForSquareOrField( CX2Unit* pUnit, bool bTField );

	bool				IsUnitLoadComplete( UidType unitUID );
	void				DeleteUnitLoadData( UidType unitUID );
	void				ClearUnitLoadData();

	void				AddNPCUnitUIDResource( UINT uid, CX2UnitManager::NPC_UNIT_ID id, const KAttribEnchantNpcInfo* pkAttribInfo );
    void				AddNPCUnitIDResource( CX2UnitManager::NPC_UNIT_ID id );
	bool				IsNPCUnitUIDResourceLoadCompleted( UINT uid );
	bool				IsNPCUnitIDResourceLoadCompleted( CX2UnitManager::NPC_UNIT_ID id  );
	void				RemoveNPCUnitUIDResource( UINT uid );
	void				RemoveNPCUnitIDResource( CX2UnitManager::NPC_UNIT_ID id );
    void                RemoveAllNPCResources();

	static bool			AppendParticleSequence( CKTDXDeviceDataList& listInOut_, CKTDGParticleSystem* pParticleSystem, const WCHAR* pSequenceName, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );

private:

public:

	struct LoadUserUnitData
	{
		UidType					m_UnitUID;
		CKTDXDeviceDataList     m_listKTDXDeviceData;
		CKTDXDeviceDataList     m_listKTDXDeviceDataLoaded;

		LoadUserUnitData()
		{
			m_UnitUID	= 0;
		}
		//~LoadUserUnitData();
	};

	//CKTDXDevice*		LoadDevice( CKTDXDevice::DEVICETYPE m_DeviceType, const char* m_DeviceID );

	float								m_fElapsedTime;
	//int									m_RenderCount;

    typedef std::list<LoadUserUnitData>	LoadUserUnitDataList;
	typedef std::map<UidType,LoadUserUnitData>	LoadUserUnitDataMap;

	LoadUserUnitDataList                m_listLoadUnitData;
	LoadUserUnitDataMap					m_mapLoadUnitDataLoaded;

    struct  NPCResource
    {
		UINT							m_uid;
        CX2UnitManager::NPC_UNIT_ID     m_id;
        CKTDXDeviceDataList             m_listKTDXDeviceData;
        CKTDXDeviceDataList             m_listKTDXDeviceDataLoaded;

        NPCResource()
            : m_uid( UINT(-1) )
			, m_id( CX2UnitManager::NUI_NONE )
            , m_listKTDXDeviceData()
            , m_listKTDXDeviceDataLoaded()
        {
        }//NPCList()
    };//
    typedef std::list<NPCResource>      NPCResourceList;
	typedef std::map<UINT,NPCResource>
										NPCUnitUIDResourceMap;
	typedef std::map<CX2UnitManager::NPC_UNIT_ID,NPCResource>
										NPCUnitIDResourceMap;
    NPCResourceList                     m_listNPCResource;
	NPCUnitUIDResourceMap				m_mapNPCUnitUIDResourceLoaded;
	NPCUnitIDResourceMap				m_mapNPCUnitIDResourceLoaded;

};
//#endif
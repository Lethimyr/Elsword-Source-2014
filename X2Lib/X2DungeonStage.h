#pragma once


class CX2DungeonStage
{
	public:

		//{{ 2010. 03. 24  최육사	비밀던전 헬모드
		enum STAGE_TYPE
		{
			ST_NONE = 0,
			ST_EXTRA_NPC,
			ST_EXTRA,
		};
		//}}

		struct ReadyNPCData
		{
			int						m_UnitID;

            ReadyNPCData()
                : m_UnitID( -1 ) {}
		};

        typedef std::map< int, KAttribEnchantNpcInfo > EnchantedNPCInfoMap;

		struct StageData	
		{
			bool										m_bStartMotion;
			CX2World::WORLD_ID							m_WorldID;
			//{{ 2010. 03. 24  최육사	비밀던전 헬모드
			STAGE_TYPE									m_eStageType;
			//}}
			D3DXCOLOR									m_WorldColor;
			D3DXCOLOR									m_UnitColor;
			vector<CX2UnitManager::NPC_UNIT_ID>			m_ReadyNPCList;
			vector<CX2DungeonSubStage::SubStageData*>	m_SubStageDataList;

			wstring										m_MissionTitle;

//			CX2World*									m_pWorld;
			std::map< int, KAttribEnchantNpcInfo >		m_mapAttribEnchantNpcInfo;

			bool										m_bSecretStage; // 헤니르 시공의 비밀 스테이지인지
#ifdef X2TOOL
			wstring										m_wstrWorldDataName;
#endif

			StageData()
			    : m_bStartMotion( false )
                , m_WorldID( CX2World::WI_NONE )
                , m_eStageType( CX2DungeonStage::ST_NONE )
                , m_WorldColor( -1, -1, -1, 1 )
                , m_UnitColor( -1, -1, -1, 1 )
                , m_ReadyNPCList()
                , m_SubStageDataList()
                , m_MissionTitle()
                , m_bSecretStage( false )
#ifdef X2TOOL
				,m_wstrWorldDataName( L"" )
#endif

				 {}

			~StageData()
			{
				m_ReadyNPCList.clear();

				for( int i = 0; i < (int)m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_SubStageDataList[i];
					SAFE_DELETE( pSubStageData );
				}
				m_SubStageDataList.clear();

//				SAFE_DELETE( m_pWorld );
			}

			bool LoadData( KLuaManager& luaManager, bool bIsNpcLoad = false );

		private:
			void LoadReadyNPC( KLuaManager& luaManager );
			void LoadSubStageData( KLuaManager& luaManager, bool bIsNpcLoad = false );
		};
//#endif // DUNGEON_WORLD_THREAD_LOADING
		

		
	public:
		CX2DungeonStage( StageData* pStageData );
		~CX2DungeonStage(void);

		void                        CreateStageNPCs();
		void                        FlushNPCs();

		void						OnFrameMove( double fTime, float fElapsedTime );

		CX2DungeonSubStage*			CreateSubStage( int subStageIndex );
		void						CloseBeforeSubStage();
		void						DeleteBeforeSubStage();
		
		CX2DungeonSubStage*			GetNowSubStage(){ return m_pNowSubStage; }		
		int							GetNowSubStageIndex(){ return m_NowSubStageIndex; }
		CX2DungeonSubStage*			GetBeforeSubStage(){ return m_pBeforeSubStage; }
		int							GetBeforeSubStageIndex(){ return m_BeforeSubStageIndex; }
		StageData*					GetStageData(){ return m_pStageData; }
		void						EnableCurtainGroup( int curtainGroupIndex, bool bEnable );

		void						StartGoEffect( CX2DungeonSubStage::GO_TYPE goType );
		void						StopGoEffect();
		bool						IsGoEffectOn();

		//CX2World*					FlushWorld();

		void                        DeleteSubStageNPCUnit( int iSubStageIndex );

	
#ifdef X2TOOL
	public:
#else
	protected:
#endif
		//void						LoadReadyNPC();
		void                        LoadReadyNPCResources();
        void                        RemoveReadyNPCResources();

		StageData*					m_pStageData;

		CX2DungeonSubStage*			m_pNowSubStage;
		int							m_NowSubStageIndex;
		CX2DungeonSubStage*			m_pBeforeSubStage;
		int							m_BeforeSubStageIndex;
		
		float						m_fElapsedTime;


//		vector<CX2GUNPC*>			m_ReadyNPCList;

		CKTDGParticleSystem::CParticleEventSequenceHandle
                                    m_hParticleGo;

		std::vector<CX2GUNPC*>      m_vecSubStageGUNPC;
};


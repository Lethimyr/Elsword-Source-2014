#pragma once

class CX2SquareGame
{
	public:
		enum SQUARE_TYPE
		{
			ST_NONE = 0,
			ST_AREA,
			ST_AGE,
			ST_FREE,
			ST_TRADE,
//#ifdef ADDED_RELATIONSHIP_SYSTEM
			ST_WEDDING,
//#endif //ADDED_RELATIONSHIP_SYSTEM
			ST_END,
		};

		struct SquareData
		{
			UidType		m_UID;
			SQUARE_TYPE	m_SquareType;
			wstring		m_SquareName;
			int			m_MaxUser;
			int			m_NowUser;

			wstring		m_RelayIP;
			int			m_Port;

			SquareData()
			{
				m_UID			= 0;
				m_SquareType	= ST_NONE;
				m_MaxUser		= 0;
				m_NowUser		= 0;
				m_Port			= 0;
			}
			void CopySquareData( SquareData* pSquareData )
			{
				if( pSquareData != NULL )
				{
					m_UID			= pSquareData->m_UID;
					m_SquareType	= pSquareData->m_SquareType;
					m_SquareName	= pSquareData->m_SquareName;
					m_MaxUser		= pSquareData->m_MaxUser;
					m_NowUser		= pSquareData->m_NowUser;
					m_RelayIP		= pSquareData->m_RelayIP;
					m_Port			= pSquareData->m_Port;
				}				
			}
			void Set_KSquareInfo( KSquareInfo* pKSquareInfo )
			{
				if( pKSquareInfo != NULL )
				{
					m_UID			= pKSquareInfo->m_iSquareUID;
					m_SquareType	= (SQUARE_TYPE)pKSquareInfo->m_cSquareType;
					m_SquareName	= pKSquareInfo->m_wstrRoomName;
					m_MaxUser		= pKSquareInfo->m_MaxSlot;
					m_NowUser		= pKSquareInfo->m_JoinSlot;
					m_RelayIP		= pKSquareInfo->m_RelayIP;
					m_Port			= pKSquareInfo->m_Port;
				}				
			}
		};

		//struct GarbageAnim
		//{
		//	vector<CKTDGXSkinAnim*>		m_GarbageAnim;

		//	~GarbageAnim()
		//	{
		//		for( int i = 0; i < (int)m_GarbageAnim.size(); i++ )
		//		{
		//			CKTDGXSkinAnim* pCKTDGXSkinAnim = m_GarbageAnim[i];
		//			SAFE_DELETE( pCKTDGXSkinAnim );
		//		}
		//		m_GarbageAnim.clear();
		//	}
		//};

	public:
		CX2SquareGame( SquareData* pSquareData, CX2World::WORLD_ID worldID );
		virtual ~CX2SquareGame(void);

		void						JoinSquareUnit( KSquareUserInfo* pKSquareUserInfo );
		CX2SquareUnit*				AddUnit( CX2Unit* pUnit, bool bMyUnit, bool bInit );
		void						RemoveUnit( UidType unitUID );

		virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT				OnFrameRender();
		virtual bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual HRESULT				OnResetDevice();
		virtual HRESULT				OnLostDevice();

		CX2Camera*					GetCamera(){ return m_pCamera; }
		CX2SquareUnit*				GetMyUnit() const { return m_pMyUnit.get(); }
		int							GetSquareUnitNum(){ return m_UserUnitList.size(); }
		CX2SquareUnit*				GetSquareUnit( int index );
		CX2SquareUnit*				GetSquareUnitByUID( UidType UID );
		CX2SquareUnit*				GetSquareUnitByNickName( const WCHAR* wszNickName );		

		vector<CX2SquareUnit*>		GetUnitList( int& iPage, int iUnitNumPerPage = 10 );
		void						GetUnitPageInfo( int &iMaxPage, int iUnitNumPerPage );
		void						SetShowUserUnit( bool bShow );

		void 						SetEnableKeyProcess(bool val) { m_bEnableKeyProcess = val; }
		void 						SetEnableCameraProcess(bool val) { m_bEnableCameraProcess = val; }

		const CX2SquareGame::SquareData& GetSquareData() const { return m_SquareData; }

		CX2World*					GetWorld(){ return m_pWorld; }
		CKTDGFontManager::CUKFont*	GetFontForUnitName() { return m_pFontForUnitName; }

		void						Handler_EGS_SQUARE_UNIT_SYNC_DATA_NOT( KEGS_SQUARE_UNIT_SYNC_DATA_NOT& kEGS_SQUARE_UNIT_SYNC_DATA_NOT );
		bool						Handler_EGS_JOIN_SQUARE_NOT( KEGS_JOIN_SQUARE_NOT& kEGS_JOIN_SQUARE_NOT );
		void						Handler_EGS_LEAVE_SQUARE_REQ() { m_bLeaveSquareGame = true; }
		bool						Handler_EGS_LEAVE_SQUARE_NOT( KEGS_LEAVE_SQUARE_NOT& kEGS_LEAVE_SQUARE_NOT );
		bool						Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT( KEGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT& kEGS_CHANGE_EQUIPPED_ITEM_NOT );
		bool						Handler_EGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT( KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT& kEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT );

		void						PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
										const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

		int							GetSendSizePerSec(){ return m_SendSizePerSec; }
		void						AddSendSizePerSec( int sendSizePerSec ){ m_SendSizePerSec += sendSizePerSec; }

		CKTDGParticleSystem*		GetMajorParticle()			{ return m_pMajorParticle; }
		CKTDGParticleSystem*		GetMinorParticle()			{ return m_pMinorParticle; }
		CKTDGXMeshPlayer*			GetMajorXMeshPlayer()		{ return m_pMajorXMeshPlayer; }
		CKTDGXMeshPlayer*			GetMinorXMeshPlayer()		{ return m_pMinorXMeshPlayer; }

		bool						GetEnableKey();
		void						SetFreeCamera( bool bFreeCamera );
		bool						GetFreeCamera() const { return m_bFreeCamera; }

		CX2TalkBoxManager*			GetTalkBoxManager() { return m_pTalkBoxManager; }
		CX2TalkBoxManager*			GetTalkBoxManagerPShop() { return m_pTalkBoxMgrForPersonalShop; }

		bool						InExitArea();
		bool						ExitMarket();

		void						DeleteWorld() { SAFE_DELETE(m_pWorld); }
		

	private:
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		void						P2PPacketHandler();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		void						KeyProcess();

		void						PopPersoanlShopTalkBox( UidType iUnitUID, wstring& wstrMsg, D3DXCOLOR color = 0xff000000 );
		
	
		SquareData					m_SquareData;
		
		CX2World*					m_pWorld;
		CX2Camera*					m_pCamera;

		CX2GUUser::InputData		m_InputData;
		CX2SquareUnitPtr            m_pMyUnit;
		
		vector<CX2SquareUnitPtr>    m_UserUnitList;

		CKTDGParticleSystem*		m_pMajorParticle;
		CKTDGParticleSystem*		m_pMinorParticle;
		CKTDGXMeshPlayer*			m_pMajorXMeshPlayer;
		CKTDGXMeshPlayer*			m_pMinorXMeshPlayer;

		float						m_fMarketTime;
		float						m_fReserveTime;
		int							m_MaxCount;
		float						m_fMaxCountCheckTime;
		int							m_fMaxCountRenderTime;
		CKTDGFontManager::CUKFont*	m_pFontForUnitName;
		float						m_fSortTime;

		float						m_fP2PSendGap;

		bool						m_bEnableKeyProcess;
		bool						m_bEnableCameraProcess;

		CX2TalkBoxManager*			m_pTalkBoxMgrForPersonalShop;
		CX2TalkBoxManager*			m_pTalkBoxManager;
		int							m_SendSizePerSec;
		float						m_fSendGap;


		bool						m_bFreeCamera;
		CFirstPersonCamera			m_FPSCamera;

		bool						m_bLeaveSquareGame;
		
		float						m_fTimeCheckShopTalkBox;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hMarketOutArea;
		//CKTDGParticleSystem::CParticle*						m_pOutAreaParticle;

};

#pragma once

#ifdef ADDED_RELATIONSHIP_SYSTEM

class CX2WeddingGame
{
	public:
		enum SQUARE_TYPE
		{
			ST_NONE = 0,
			ST_AREA,
			ST_AGE,
			ST_FREE,
			ST_TRADE,
			ST_END,
		};

		struct UDPData
		{
			wstring		m_RelayIP;
			int			m_Port;
		};

		
		// 임시 // 지워야 할듯..
/*
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
		};*/


	public:
		CX2WeddingGame( UDPData* pSquareData, SEnum::WEDDING_HALL_TYPE eHallType, SEnum::WEDDING_OFFICIANT_TYPE eOfficiantType_ );
		virtual ~CX2WeddingGame(void);

		void						JoinSquareUnit( KSquareUserInfo* pKSquareUserInfo );
		void						JoinSquareUnit( KRoomUserInfo* pKRoomUserInfo );
		CX2SquareUnit*				AddUnit( CX2Unit* pUnit, bool bMyUnit, bool bInit, char cWeddingStatus = 0);
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

		//const CX2WeddingGame::SquareData& GetSquareData() const { return m_SquareData; }

		CX2World*					GetWorld(){ return m_pWorld; }
		CKTDGFontManager::CUKFont*	GetFontForUnitName() { return m_pFontForUnitName; }

		void						Handler_EGS_JOIN_WEDDING_HALL_NOT( KEGS_JOIN_WEDDING_HALL_NOT& kEGS_JOIN_WEDDING_HALL_NOT );
		void						Handler_EGS_LEAVE_WEDDING_HALL_REQ() { m_bLeaveWeddingGame = true; }
		void						Handler_EGS_LEAVE_WEDDING_HALL_NOT( KEGS_LEAVE_WEDDING_HALL_NOT& kEGS_LEAVE_WEDDING_HALL_NOT );

		void						PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
										const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

		void						PopOfficiantTalkBox( int iStrID_, const D3DXVECTOR3& vPos_, const D3DXCOLOR& color_ = 0xff000000, const D3DXCOLOR& outcolor_ = (DWORD)0x00000000, const D3DXCOLOR& backColor_ = (DWORD)0xffffffff );
		
		CKTDGParticleSystem*		GetMajorParticle();
		CKTDGParticleSystem*		GetMinorParticle();
		CKTDGXMeshPlayer*			GetMajorXMeshPlayer();
		CKTDGXMeshPlayer*			GetMinorXMeshPlayer();

		bool						GetEnableKey();
		void						SetFreeCamera( bool bFreeCamera );
		bool						GetFreeCamera() const { return m_bFreeCamera; }

		CX2TalkBoxManager*			GetTalkBoxManager() { return m_pTalkBoxManager; }


		void						DeleteWorld() { SAFE_DELETE(m_pWorld); }

		CFirstPersonCamera*			GetFPSCamera(){return &m_FPSCamera; }

		void						SetOfficiantCameraTime( float fVal ) { m_fOfficiantCameraTime = fVal; }

		void						StartWedding();


		void						GetStartPosByWeddingStatus( IN SEnum::WEDDING_STATUS eWeddingStatus_,  OUT D3DXVECTOR3& vPos_ ) const;

		void						PlayEffectSet_WeddingGame( const WCHAR* pEffectSetName_, const D3DXVECTOR3& vPos_ ,CX2WeddingBehavior::WEDDING_EFFECTSET_POS_TYPE ePosType_ = CX2WeddingBehavior::WEPT_CUSTOM );

		int							GetReturnStateID() const { return m_iReturnStateID; }
		void						SetReturnStateID(int iVal) { m_iReturnStateID = iVal; }

		UINT						GetWeddingHeroineNum() const { return m_iWeddingHeroineNum; }
		void						SetWeddingHeroineNum(UINT val) { m_iWeddingHeroineNum = val; }

		bool						GetIsCanWeddingStart() const;

		bool						IsWeddingHeroineMyUnit() const;
		bool						IsWeddingHeroineByUID( UidType uiUserUID ) const;
	private:
		void						P2PPacketHandler();
		void						KeyProcess();

		void						OnFrameMove_UnitFrameMove( double fTime, float fElapsedTime );
		void						OnFrameMove_SyncPacket();
		void						OnFrameMove_Camera( double fTime, float fElapsedTime );

	
		//SquareData					m_SquareData;
		UDPData						m_udpData;
		
		CX2World*					m_pWorld;
		CX2Camera*					m_pCamera;

		CX2GUUser::InputData		m_InputData;
		CX2SquareUnitPtr            m_pMyUnit;

		float						m_fReserveTime;
		vector<CX2SquareUnitPtr>    m_UserUnitList;

		CKTDGParticleSystem*		m_pMajorParticle;
		CKTDGParticleSystem*		m_pMinorParticle;
		CKTDGXMeshPlayer*			m_pMajorXMeshPlayer;
		CKTDGXMeshPlayer*			m_pMinorXMeshPlayer;

		CKTDGFontManager::CUKFont*	m_pFontForUnitName;

		float						m_fP2PSendGap;

		bool						m_bEnableKeyProcess;
		bool						m_bEnableCameraProcess;

		CX2TalkBoxManager*			m_pTalkBoxManager;


		bool						m_bFreeCamera;
		CFirstPersonCamera			m_FPSCamera;

		bool						m_bLeaveWeddingGame;
		float						m_fOfficiantCameraTime;

		CX2WeddingManager*			m_pWeddingManager;

		std::vector<UidType>		m_vecUserUIDforSyncPacket;	/// 매 프레임 유저리스트에 있는 UID를 가져와서 Broad 캐스트 때 사용 함

		int							m_iReturnStateID;			// 결혼식 완료 이후 되돌아 갈 스테이트 ID
		UINT						m_iWeddingHeroineNum;		// 신랑, 신부 다 들어왔는지 체크하는 변수

		const int					m_iNessesaryHeroineNum;
		
};

#endif //ADDED_RELATIONSHIP_SYSTEM
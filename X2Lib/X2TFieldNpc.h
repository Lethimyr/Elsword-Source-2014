#pragma once


//{{ seojt // 2009-1-14, 21:32
class CX2TFieldNpc;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2TFieldNpc>     CX2TFieldNpcPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2TFieldNpc>     CX2TFieldNpcPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-14, 21:32


class CX2TFieldNpc : public CKTDGObject
{
	public:
		enum NPCTYPE
		{
			NT_NORMAL = 0,
			NT_EQUIP,
			NT_ACCESSORY,
			NT_TRAINNING,
			NT_PVP,
			NT_EVENT,
			NT_ALCHEMIST,
			NT_POSTBOX,
			NT_RANKING,
			NT_PRIVATE_BANK,	// kimhc // 2009-08-06 // 캐릭터별 은행
			NT_NO_RENDER_EVENT,	// kimhc // 2009-12-08 // 크리스마스 이벤트용
			
		};

	private:
		CX2TFieldNpc( bool bBackgroundLoad );
		virtual ~CX2TFieldNpc(void);		

	public: 
		//{{ seojt // 2009-1-14, 21:33
		static CX2TFieldNpcPtr CreateTFieldNPC( bool bBackgroundLoad )
        {
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            CX2TFieldNpcPtr ptrSquareNpc( new CX2TFieldNpc( bBackgroundLoad ) );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			CX2TFieldNpcPtr ptrSquareNpc( new CX2TFieldNpc( bBackgroundLoad ), CKTDGObject::KTDGObjectDeleter() );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            return ptrSquareNpc;
        }//CreateTFieldNPC()
		//}} seojt // 2009-1-14, 21:33

		void InitParticle();
		virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		virtual void				OnFrameRender_Draw();
		virtual bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual HRESULT				OnResetDevice();
		virtual HRESULT				OnLostDevice();
		
		void						CreateFieldNpc(CX2World *pWorld, CX2LocationManager::HouseTemplet *pHouseTemplate);
		CKTDGFontManager::CUKFont*	GetFontForUnitName() { return m_pFontForUnitName; }				

		void						SetShowObject(bool val);
		bool						GetShowObject();
		void						SetInArea(bool val) { m_bInArea = val; }
		bool						GetInArea() { return m_bInArea; }		

		CKTDGParticleSystem*		GetUiParticle();
		D3DXVECTOR3					GetPos() { return m_vPos; }
		D3DXVECTOR3					GetHeadBonePos() 
		{
			if(m_pHouseTemplet != NULL && m_pHouseTemplet->m_ID == CX2LocationManager::HI_BILLBOARD)
			{
				D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"NPC_Pivit" );
				vPos.y += 300.f;
				return vPos;
			}

			if( NULL != m_pXSkinAnim )
				return m_pXSkinAnim->GetCloneFramePosition( L"Bip01_HeadNub" ); 
			
			return D3DXVECTOR3(0, 0, 0 );
		}

		CX2LocationManager::HouseTemplet *GetHouseTemplet() { return m_pHouseTemplet; }
		CX2TFieldNpcShop*			GetNpcShop() { return m_pDlgNpcMessage; }
		void						SetNpcTalk(bool val) { m_bTalk = val; }		
		void						SetNpcBye(bool val) { m_bBye = val; }
		void						ResetNpc();
		bool						GetNpcTalk() { return m_bTalk; }
		bool						GetNpcBye()	{ return m_bBye; }
		bool						GetNpcJoin() { return m_bJoinNpc; }
		void						RenderName();		

		void						SetShowField(bool val);

		bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		CX2TFieldNpc::NPCTYPE GetHouseType();

// #ifdef NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST
// 		virtual bool OnThreadLoad(DWORD dwParam_, DWORD dwParam2_);
// #endif NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST

		bool GetEnable() const { return m_bEnable; }
		void SetEnable(bool val) { m_bEnable = val; }

		void InitQuestMark();
		void GetQuestInfo();
		int GetNpcId() { return m_iNpcIndex; }


		int GetNewQuestRepeat() const { return m_nNewQuestRepeat; }
		int GetNewQuestNormal() const { return m_nNewQuestNormal; }
		int GetEventQuest() const { return m_nEventQuest; }
		int GetDoQuest() const { return m_nDoQuest; }
		int GetCompleteQuestRepeat() const { return m_nCompleteQuestRepeat; }
		int GetCompleteQuestNormal() const { return m_nCompleteQuestNormal; }
		int GetFairLvQuest(){ return m_nFairLvQuest; }

		void ChangeType(CX2TFieldNpcShop::NPC_SHOP_BUTTON_TYPE eType);

		void SetPickNpc(bool val) { m_bPickNpc = val; }
		bool GetPickNpc() { return m_bPickNpc; }		

		void SetCanTalkNpc( bool val, bool bForce = false);
		bool GetCanTalkNpc() { return m_bCanTalkNpc; }
		bool GetIsEventNpc() { return m_bEventNpc; }
		void ResetNpcType();
	private:
		bool Handler_EGS_TALK_WITH_NPC_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID );
		bool Handler_EGS_TALK_WITH_NPC_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef NPC_SOUND
		void NpcPlaySound(wstring &wstrSoundName);
#endif

	private:		
		
		CX2World*					m_pWorld;
		CX2LocationManager::HouseTemplet *m_pHouseTemplet;

		CKTDGFontManager::CUKFont*	m_pFontForUnitName;

		bool						m_bPlay;
		bool						m_bInArea;
		bool						m_bTalk;
		bool						m_bBye;
		bool						m_bJoinNpc;
		bool						m_bOpenQuestReceive;

		int								m_iMotionId;
		D3DXVECTOR3						m_vBonePos;

        // 내부에서 관리하는 객체이므로 smart pointer를 사용하지 않는다.
        // - jintaeks on 2009-01-12, 16:07
		CKTDGXSkinAnim*					m_pXSkinAnim;
		CKTDXDeviceXSkinMesh*			m_pMesh;
		CKTDXDeviceXET*					m_pAniXET;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNpcInArea;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNpcInAreaBg;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNpcEvent;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNpcNewQuest1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNpcNewQuest2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNpcDoQuest;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNpcCompleteQuest1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNpcCompleteQuest2;

		//CKTDGParticleSystem::CParticle*						m_pInAreaParticle;
		//CKTDGParticleSystem::CParticle*						m_pInAreaBgParticle;
		//CKTDGParticleSystem::CParticle*						m_pEventParticle;
		//CKTDGParticleSystem::CParticle*						m_pNewQuestParticle1;
		//CKTDGParticleSystem::CParticle*						m_pNewQuestParticle2;
		//CKTDGParticleSystem::CParticle*						m_pDoQuestParticle;
		//CKTDGParticleSystem::CParticle*						m_pCompleteQuestParticle1;
		//CKTDGParticleSystem::CParticle*						m_pCompleteQuestParticle2;

		float						m_fJoinNpcTime;	
		float						m_fNpcMarkTime;

		CX2TFieldNpcShop			*m_pDlgNpcMessage;
		CX2UIUnit*					m_pUINPC;		

		int							m_iNpcIndex;
		wstring						m_strNpcName;	

		CKTDGPlanShadow*			m_pUnitShadow;		

		bool	m_bOpenedShop;
		wstring m_strWaitTalk;
		wstring m_strMakeTalk;
		wstring m_strShopTalk;
		wstring m_strPvpTalk;
		wstring m_strQuestTalk;
		wstring m_strByeTalk;
		wstring m_strRankingTalk;
		wstring m_strExchangeTalk;

		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
		wstring m_strPrivateBankTalk;
#endif PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef GUILD_BOARD
		//{{ oasis907 : 김상윤 [2010.2.2] // 길드 게시판
		wstring m_strGuildADTalk;
		//}}
#endif GUILD_BOARD
#ifdef DEF_TRADE_BOARD
		//{{ oasis907 : 김상윤 [2010.2.2] // 길드 게시판
		wstring m_strPersonalShopTalk;
		//}}
#endif DEF_TRADE_BOARD

#ifdef SERV_PSHOP_AGENCY
		wstring m_strRegisterTalk;
		wstring m_strReceiveTalk;
#endif
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		wstring m_strSynthesisTalk;
#endif SERV_SYNTHESIS_AVATAR
		//}}		
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		wstring m_strExchangeNewItemTalk;
#endif //SERV_NEW_ITEM_SYSTEM_2013_05

		wstring m_strHouseName;
		

		bool				m_bFirstAnim;
		D3DXVECTOR3			m_vBasePos;
		bool				m_bStaticMesh;
		bool				m_bShowPost;

		CX2LocationManager::HOUSE_TYPE m_eHouseType;



		int m_nNewQuestRepeat;
		int m_nNewQuestNormal;
		int m_nEventQuest;
		int m_nDoQuest;
		int m_nCompleteQuestRepeat;
		int m_nCompleteQuestNormal;
		int m_nFairLvQuest;

		bool m_bPickNpc;

		wstring m_wstrPlaySoundName;

#ifdef TALK_BOX_VILLAGE_NPC
		vector<wstring> m_vecNPCTalkBox;
		float	m_fElapsedTimeAfterDisableNPCTalkBox;
		bool	m_bEnableNPCTalkBox;
		short	m_sTalkNum;
#endif

#ifdef ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_NEAR // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가
		bool	m_bIsPlayNearSound;				// 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능을 사용할 것인가?
		wstring	m_wstrNearSoundFileName;		// 사운드 파일 이름
		float	m_fPlayMaxNearSoundCoolTime;	// 사운드 출력 재사용 시간
		float	m_fPlayNowNearSoundCoolTime;	// 사운드 출력 재사용 시간
		float	m_fPlayNearSoundDistance;		// 사운드 출력 조건 ( 거리 )
		CKTDXDeviceSound* m_pNearPlaySound;		// 사운드 포인터
#endif // ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_CLOSE // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가

	public:

        bool                            m_bBackgroundLoad;
        bool                            m_bLoaded;
        bool                            m_bReservedShowObject;
        bool                            m_bReservedCanTalkNpc;

		int								m_NpcHouseID;
		
		D3DXVECTOR3						m_vPos;
		D3DXVECTOR3						m_vRot;
		D3DXVECTOR3						m_vScale;

		int								m_NPCJoinArea;
		bool							m_bPvpNpc;
		bool							m_bShopNpc;
		bool							m_bMakingNpc;
		bool							m_bTraining;
		bool							m_bFreeTraining;
		//{{ 09.03.24 태완
		bool							m_bEnchant;
		bool							m_bAttribute;
		//}}
#ifdef GUILD_BOARD
		bool							m_bGuild; // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
#endif GUILD_BOARD
		bool							m_bRanking;

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		bool							m_bExchange;
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
		bool							m_bPrivateBank;
#endif PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
		bool							m_bWebEvent;
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD // oasis907 : 김상윤 [2010.3.12] // 
		bool							m_bPersonalShop;
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
		bool							m_bAgencyTraderRegister;
		bool							m_bAgencyTraderReceive;
#endif
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		bool							m_bSynthesisNpc;
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef NPC_EVENT_BUTTON
		bool							m_bEvent1;
		bool							m_bEvent2;
		bool							m_bEvent3;
		bool							m_bEvent4;
#endif NPC_EVENT_BUTTON
//#ifdef NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST
//		string							m_NPCModelName;
//		bool							m_bTryModelLoad;	// 모델 로딩을 시도했었다. 
//#endif NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST

		bool							m_bEnable;
		bool							m_bEventNpc;
		bool							m_bCanTalkNpc;

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		bool							m_bExchangeNewItem;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

		
};

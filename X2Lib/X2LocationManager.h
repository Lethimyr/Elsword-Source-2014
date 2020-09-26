#pragma once
class CX2LocationManager
{
	public:
		enum HOUSE_TYPE
		{
			HT_INVALID = 0,
			HT_NPC_HOUSE,			/// NPC 하우스
			HT_EQUIP_SHOP,			/// 대장
			HT_ACCESSORY_SHOP,		/// 액세서리
			HT_TRAINNING_CENTER,	/// 훈련소
			HT_ALCHEMIST_HOUSE,		/// 연금
			HT_POSTBOX,				/// 우편함
			HT_BILLBOARD,			/// 게시판
			HT_PRIVATE_BANK,		/// 은행
			HT_EVENT,				/// 크리스마스 트리 등의 이벤트 용도 
		};

		enum HOUSE_ID
		{
			HI_INVALID = 0,
			HI_ANNE_HOUSE = 40000,
			HI_HAGERS_HOUSE,
			HI_ADAMS_HOUSE,
			HI_LOW_TRAINNING_ZONE,
			HI_ECHO_ALCHEMIST_HOUSE,
			HI_HOFMANN_HOUSE,
			HI_LUICHEL_HOUSE,
			HI_LENPAD_HOUSE,
			HI_CHACHABUK_HOUSE,		//yungom
			HI_RICHANG_HOUSE,		//acce
			HI_STELLA_HOUSE,		//chonjang
			HI_TOMA_HOUSE,			//shop
			HI_ADEL_HOUSE,
			HI_AMOS_ALCHEMIST_HOUSE,
			HI_AGATHA_EQUIP_SHOP_HOUSE,
			HI_AIDA_ACCESSORY_HOUSE,
			HI_CAMILLA_PVP_HOUSE,
			HI_ARIEL_EVENT_HOUSE,
			HI_HELLEN_SHOP_HOUSE,
			HI_ALLEGRO_PEITA_HOUSE,
			HI_LENTO_PEITA_HOUSE,
			HI_POSTBOX,
			HI_EVENT_NPC1,
			HI_EVENT_NPC2,
			HI_WILLIAM_PORTER1,
			HI_WILLIAM_PORTER2,
			HI_WILLIAM_PORTER3,
			HI_WILLIAM_PORTER4,

			HI_BILLBOARD,		// 게시판(랭킹)
			HI_GLAVE_HOUSE,		// 시공
			HI_PRIVATE_BANK,	// 은행

            HI_GRAIL,	// 벨더 연금술사
			HI_NOEL,	// 벨더 음유시인
			HI_VANESSA,	// 벨더 촌장
			HI_PRAUS,	// 벨더 장신구
			HI_HANNA,	// 벨더 무기점

			HI_CHRISTMAS_TREE,	// 이벤트용 크리스마스 트리
			HI_CAKE_VILLAGE,	// 1주년 기념 케익
		//{{ 김상훈 : 2010.11.19
			HI_EVENT_FAIRY_GUARDIAN, /// 이벤트용 페어리 가디언. 
			HI_SANTA_DARKELF,
		//}} 김상훈 : 2010.11.19

			//{{ JHKang / 강정훈 / 2011/01/19 / 하멜 하우스
		//#ifdef SEASON3_MONSTER_2010_12
			HI_HORATIO,		/// 호레이쇼(대장장이)
			HI_LUCY,		/// 루시(액세서리)
			HI_DAISY,		/// 데이지(비서)
			HI_PENENSIO,	/// 페넨시오(붉은기사단 십부장)
			HI_DENKA,		/// 덴카(연금술사)
		//#endif SEASON3_MONSTER_2010_12
			//}} JHKang / 강정훈 / 2011/01/19 / 하멜 하우스

			//{{ JHKang / 강정훈 / 2011/03/29 / 식목일 만드라실
			HI_MANDRASSIL,	/// 만드라실(식목일)
			//}}

			HI_MU,	//대리상인
			HI_LUTO,

			//{{ JHKang / 강정훈 / 2011.11.08 / 버블파이터 공동 프로모션
			HI_DAO,		/// 다오 NPC
			//}}

			HI_APINK,
			HI_APINK_ARCHANGEL,	// 에이핑크 대천사 NPC

			HI_DARKMOON,			/// JHKang / 강정훈 / 2012.09.24 / 한가위 마을 NPC

			HI_MEGUPOID,			///	일본 이벤트 NPC 메구포이드
			
			HI_ROSEANG,				/// 샌더 액세사리  로즈앙
			HI_EMIRATE,				/// 샌더 촌장 에미리트
			HI_VAPOR,				/// 샌더 연금술사 바포르
			HI_DAPPAR,				/// 샌더 대장상인 다파르

			HI_EVENT_GRAIL,			/// 그레일 이벤트

			HI_EVENT_BENDERS,		/// 이벤트 앙드레 벤더스

			HI_EVENT_CRAYONPOP,		/// 이벤트 크래용팝

			HI_EVENT_MOON_RABBIT,	/// 이벤트 달토끼
			HI_EVENT_SHEATH_NIGHT = 40061, //EVENT_NPC_STANDING_VILLAGE
			HI_EVENT_INFINITY_SWORD = 40062, //EVENT_NPC_STANDING_VILLAGE
			HI_EVENT_LIRE_NIGHT_WATCHER	=   40063, //--ALWAYS_EVENT_LIRE_NIGHT_WATCHER_NPC
			HI_EVENT_ADAMS_UI_SHOP	= 40064, // --ALWAYS_EVENT_ADAMS_UI_SHOP
			HI_EVENT_DIMENSION_WITCH = 40065, //EVENT_NPC_STANDING_VILLAGE
		};

		enum LOCAL_MAP_ID
		{
			LMI_INVALID = 0,
			LMI_RUBEN = 10000,	/// 루벤
			LMI_ELDER,			/// 엘더
			LMI_BESMA,			/// 베스마
			LMI_ALTERA_ISLAND,	/// 알테라
			LMI_PEITA,			/// 페이타
			LMI_VELDER,			/// 벨더
			LMI_HAMEL,			/// 하멜
			LMI_SANDER,			/// 샌더
			LMI_CHINA,			/// 중국이벤트 천조
		};

		enum WORLD_MAP_ID
		{
			WMI_INVALID = 0,
			WMI_ELLIOS = 1,

			WMI_END,
		};


#ifdef QUEST_GUIDE
/*
		struct MapInfo
		{
			UINT									uiMoveMapID; // SEnum::VILLAGE_MAP_ID, CX2BattleFieldManager::BATTLE_FIELD_ID
			int										iStartPos;
			CX2BattleFieldManager::PORTAL_MOVE_TYPE	eMapType;

			MapInfo(UINT uiMoveMapID_, int iStartPos_, CX2BattleFieldManager::PORTAL_MOVE_TYPE eMapType_ = CX2BattleFieldManager::PMT_MOVE_TO_BATTLE_FIELD):
			uiMoveMapID(uiMoveMapID_), iStartPos(iStartPos_), eMapType(eMapType_){};
		};*/

#endif //QUEST_GUIDE
		struct NpcFieldTalk
		{
			bool m_bUserName;
			wstring m_msgNpcTalk;
		};

		struct CommonNpcPos
		{
			CX2World::WORLD_ID	m_eWorldID;
			D3DXVECTOR3 m_vNpcPos;			
			D3DXVECTOR3 m_vNpcRot;
#ifdef LOCAL_NPC_JOIN_AREA // oasis907 : 김상윤 [2010.10.19] // 이 값이 설정되어 있을 경우 m_NPCJoinArea 값보다 우선.
			float		m_fLocalJoinArea;
#endif LOCAL_NPC_JOIN_AREA
		};

		struct HouseTemplet
		{
			//NPC 번호
			HOUSE_ID		m_ID;
			HOUSE_TYPE		m_Type;

			wstring			m_HouseName;

			wstring			m_NPCTextureName;
			wstring			m_NPCTextureKey;
			D3DXVECTOR2		m_NPCTexturePos;

			D3DXVECTOR2		m_QuestInfoPos;
			D3DXVECTOR2		m_EventQuestInfoPos;

			wstring			m_TitleTextureName;
			wstring			m_TitleTextureKey;

			wstring			m_NPCMeshName;
			wstring			m_NPCAniTex;
			vector<wstring>	m_vecNPCJoinTalkSound;
			vector<wstring>	m_vecNPCByeTalkSound;

			D3DXVECTOR3		m_NPCMeshPos;
			D3DXVECTOR3		m_NPCMeshRot;
			D3DXVECTOR3		m_NPCMeshScal;

			int				m_NPCJoinArea;
			bool			m_bPvpNpc;
			bool			m_bMakingNpc;
			bool			m_bTraining;
			bool			m_bFreeTraining;
			bool			m_bEnchant;
			bool			m_bAttribute;
			bool			m_bRepair;
			bool			m_bSell;
#ifdef GUILD_BOARD
			bool			m_bGuild;
#endif GUILD_BOARD
			bool			m_bRanking;

			//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
			bool			m_bExchange;
#endif	ITEM_EXCHANGE_SHOP
			//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

			//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
			bool			m_bPrivateBank;
#endif PRIVATE_BANK
			//}} kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
			bool			m_bWebEvent;
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
			bool			m_bPersonalShop;
#endif DEF_TRADE_BOARD
#ifdef NPC_EVENT_BUTTON
			bool			m_bEvent1;
			bool			m_bEvent2;
			bool			m_bEvent3;
			bool			m_bEvent4;
#endif NPC_EVENT_BUTTON
#ifdef SERV_PSHOP_AGENCY
			bool			m_bAgencyTraderRegister;
			bool			m_bAgencyTraderReceive;
			wstring			m_strRegisterTalk;
			wstring			m_strReceiveTalk;
#endif
			//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
			bool			m_bSynthesis;
#endif SERV_SYNTHESIS_AVATAR
			//}}

			bool			m_bEventNpc;
			float			m_fStartYear;
			float			m_fStartMonth;
			float			m_fStartDay;
			float			m_fStartHour;
			float			m_fWaitMin;
			float			m_fActiveMin;

			vector<NpcFieldTalk>					m_vecNPCTalk;

			vector<CX2UnitManager::NPC_UNIT_ID>		m_NPCList;
			//{{ kimhc // 2009-12-08 // 판매하는 아이템 별로 카테고리가 자동으로 정해지도록 수정
#ifndef	ADD_HOUSE_ID_TO_MANUFACTURE
			#ifdef NEW_VILLAGE_UI
						vector< CX2UIShop::BUY_ITEM_CATEGORY >	m_ShopCategoryTypeList;
			#else
						vector< CX2Shop::BUY_ITEM_CATEGORY >	m_ShopCategoryTypeList;
			#endif
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
			//}} kimhc // 2009-12-08 // 판매하는 아이템 별로 카테고리가 자동으로 정해지도록 수정

			vector< CommonNpcPos >					m_vecCommonPos;

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
			wstring m_strPrivateBank;
#endif PRIVATE_BANK
			//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef GUILD_BOARD
			//{{ oasis907 : 김상윤 [2010.2.2] // 
			wstring m_strGuildADTalk;
			//}}
#endif GUILD_BOARD

#ifdef DEF_TRADE_BOARD
			// oasis907 : 김상윤 [2010.3.31] // 
			wstring m_strPersonalShopTalk;
			//}}
#endif DEF_TRADE_BOARD
			//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
			wstring	m_strSynthesisTalk;
#endif SERV_SYNTHESIS_AVATAR
			//}}
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			wstring m_strExchangeNewItemTalk;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef TALK_BOX_VILLAGE_NPC
			vector<wstring> m_vecNPCTalkBox;
#endif

			//{{ kimhc // 2009-12-04 // 크리스마스 이벤트 나무
#ifdef	CHRISTMAS_TREE
			// Zbutton 입력을 사용할 것인가?
			bool	m_bCanTalkNpc;
#endif	CHRISTMAS_TREE
			//}} kimhc // 2009-12-04 // 크리스마스 이벤트 나무
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			bool m_bExchangeNewItem;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_NEAR // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가
			wstring m_wstrNearSoundFileName;		// 사운드 파일 이름
			float	m_fPlayNearSoundCoolTime;		// 사운드 출력 재사용 시간
			float	m_fPlayNearSoundDistance;		// 사운드 출력 조건 ( 거리 )
#endif // ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_CLOSE // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가
		};


		struct DungeonLoungeUserStartPos
		{
			vector<D3DXVECTOR3> m_vecStartPos;
		};

		struct VillageTemplet
		{
			wstring							m_Name;
			SEnum::VILLAGE_MAP_ID					m_VillageID;
			vector<HOUSE_ID>				m_HouseList;
			wstring							m_FrameMoveFuncName;

			float							m_fCameraDistance;
			float							m_fLookAtHeight;
			float							m_fEyeHeight;

			bool							m_bIsDungeonResultField; // 던전결과방 필드인지
			std::map<CX2World::WORLD_ID, DungeonLoungeUserStartPos > m_mapDungeonLoungeUserStartPos;
		};

		struct LocalMapTemplet
		{
			LOCAL_MAP_ID					m_LocalMapID;
			SEnum::VILLAGE_MAP_ID					m_VillageID;
			SEnum::VILLAGE_MAP_ID					m_DungeonGateID;
			SEnum::VILLAGE_MAP_ID					m_DungeonLoungeID;
			SEnum::VILLAGE_MAP_ID					m_eBattleFieldRestID;
			int								m_RequireUnitLevel;
			vector<SEnum::DUNGEON_ID>	m_DungeonList;
			wstring							m_ScriptFileName;
			vector<int>						m_vecRequireClearDungeonID;
			bool							m_bEnable;
			vector<SEnum::VILLAGE_MAP_ID>	m_vecFieldList;



			LocalMapTemplet( const LOCAL_MAP_ID eLocalMapID_, 
				const SEnum::VILLAGE_MAP_ID eVillageID_, const SEnum::VILLAGE_MAP_ID eDungeonGageID_, 
				const SEnum::VILLAGE_MAP_ID eDungeonLoungeID_, const SEnum::VILLAGE_MAP_ID eBattleFieldRestID_,
				const vector<SEnum::DUNGEON_ID>& vecDungeonIdList_, 
				const vector<SEnum::VILLAGE_MAP_ID>& vecBattleFieldList_, 
				const wstring& wstrScriptFileName_, const bool bEnable_ )
				: m_LocalMapID( eLocalMapID_ ), m_VillageID( eVillageID_ ), m_DungeonGateID( eDungeonGageID_ ),
				m_DungeonLoungeID( eDungeonLoungeID_ ), m_eBattleFieldRestID( eBattleFieldRestID_ ),
				m_RequireUnitLevel( 1 ),
				m_ScriptFileName( wstrScriptFileName_ ), m_bEnable( bEnable_ )
			{
				m_DungeonList = vecDungeonIdList_;
				m_vecFieldList = vecBattleFieldList_;
			}
		};	

		struct WorldMapTemplet
		{
			WORLD_MAP_ID			m_WorldID;
			vector<LOCAL_MAP_ID>	m_LocalMapIDList;

			wstring					m_ScriptFileName;
		};

		struct VillageStartPos
		{
			D3DXVECTOR3			m_StartPos;		/// 캐릭터가 있을 위치
			SEnum::VILLAGE_MAP_ID		m_VillageID;	/// 현재 마을의 ID (VMI_ )		
			UINT				m_uiBattleFieldId;	/// 이동할 필드의 ID
			USHORT				m_usBattleFieldStartLineIndex;	// 이동할 필드에서 시작될 위치 인덱스
			//std::vector<int>	m_vecLinkStartPos;	/// FieldFix: 필드에서 필요 없음(다른 곳으로 이동하는 경우 캐릭터가 있을 위치)

			bool				m_bIsMarket;	/// 거래광장으로 들어가는 입구 위치를 나타내는 지
			bool				m_bIsSummon;	/// 다른 곳에서 부터 이동 했을 때 캐릭터가 있을 위치
			bool				m_bIsWarp;		/// Warp 치트, 마을이동 등을 사용했을때 캐릭터가 있을 위치
			bool				m_bIsRight;		/// 캐릭터 좌/우 바라보는 방향
			
			
		};

		// 던전 플레이 이후 돌와와야 할 곳에 대한 위치 정보
		struct ComeBackInfoFromDungeon 
		{
			UINT	m_uiX2StateBeforeDungeonStart;		/// 던전을 시작했을 때의 X2State 상태 (마을 또는 배틀필드)
			UINT	m_uiVillageId;						/// 마을 인경우 VillageID, 배틀필드인 경우 BattleFieldID
			UINT	m_uiLastTouchedIndex;				/// 돌아가야 하는 곳의 마지막으로 있었던 라인맵 번호
			D3DXVECTOR3	m_vPos;							/// 돌아아가야 하는 곳의 위치

			ComeBackInfoFromDungeon()
				: m_uiX2StateBeforeDungeonStart( 0 ), m_uiVillageId( 0 ), m_uiLastTouchedIndex( 0 ),
				m_vPos( 0.0f, 0.0f, 0.0f )
			{}

			ComeBackInfoFromDungeon( const UINT m_uiX2StateWhenIStartDungeonGame_,
				const UINT m_uiPlaceID_, const UINT m_uiLastTouchedIndex_, const D3DXVECTOR3& vPos_ )
				: m_uiX2StateBeforeDungeonStart( m_uiX2StateWhenIStartDungeonGame_ ), m_uiVillageId( m_uiPlaceID_ ),
				m_uiLastTouchedIndex( m_uiLastTouchedIndex_ ), m_vPos( vPos_ )
			{}
		};

	public:
		CX2LocationManager(void);
		virtual ~CX2LocationManager(void);

		WorldMapTemplet*	GetWorldMapTemplet( WORLD_MAP_ID worldMapID );
		LocalMapTemplet*	GetLocalMapTemplet( LOCAL_MAP_ID localMapID );
		VillageTemplet*		GetVillageMapTemplet( SEnum::VILLAGE_MAP_ID villageMapTemplet );
		HouseTemplet*		GetHouseTemplet( HOUSE_ID houseID );

		SEnum::VILLAGE_MAP_ID GetDungeonLoungeIDByDungeonID( const SEnum::DUNGEON_ID eDungeonId_ );

		SEnum::VILLAGE_MAP_ID GetVillageID( LOCAL_MAP_ID eLocalMapID );	
		SEnum::VILLAGE_MAP_ID GetDungeonGateID( LOCAL_MAP_ID eLocalMapID );
		SEnum::VILLAGE_MAP_ID GetDungeonLoungeID( LOCAL_MAP_ID eLocalMapID );


		CX2LocationManager::LOCAL_MAP_ID GetLocalMapID( SEnum::DUNGEON_ID eDungeonID, WORLD_MAP_ID eWorldID = WMI_INVALID );
		CX2LocationManager::LOCAL_MAP_ID GetLocalMapID( SEnum::VILLAGE_MAP_ID eVillageMapID, WORLD_MAP_ID eWorldID = WMI_INVALID );


		const CX2LocationManager::VillageStartPos* GetVillageStartPos(int startPos) const;
		SEnum::VILLAGE_MAP_ID GetMapId(int startPos) const;
		void GetVecLinkedPos(std::vector<int>& vStart);
		void GetVecMarketPos(std::vector<int>& vStart);
		D3DXVECTOR3 GetStartPosLoc(int startPos) const;
		//int	GetStartPosLink(int startPos) const;		
		int GetLoginPos(int mapId);

		UINT	GetBattleFieldId( const int iStartPosNumber_ ) const;
		
		D3DXVECTOR3 GetNearLoginPos(D3DXVECTOR3 vPos);

		void SetVillage(SEnum::VILLAGE_MAP_ID val, const D3DXVECTOR3& pos, int iLineIndex = 0 );
		void GetVillage(SEnum::VILLAGE_MAP_ID &VillageId, D3DXVECTOR3 &vPos, int* pLineIndex = NULL );		

		//마을에서 캐릭터가 좌/우 바라보는 방향 설정
		void SetVillageIsRight(bool bVal) { m_bIsRight = bVal;}
		bool GetVillageIsRight() {return m_bIsRight; }

		void RestoreVillage();


		SEnum::VILLAGE_MAP_ID GetCurrentVillageID() const 
		{
			return m_eCurrentVillage;
		}

		bool IsVillageField( const SEnum::VILLAGE_MAP_ID eVillageID ) 
		{
			if ( eVillageID >= SEnum::VMI_RUBEN && eVillageID <= SEnum::VMI_SANDER )
				return true;
			else
				return false;
		}

		bool IsDungeonGate( const SEnum::VILLAGE_MAP_ID eVillageID ) 
		{
			if ( eVillageID >= SEnum::VMI_DUNGEON_GATE_RUBEN && eVillageID <= SEnum::VMI_DUNGEON_GATE_HAMEL )
				return true;
			else
				return false;
		}
		
		bool IsDungeonLounge( const SEnum::VILLAGE_MAP_ID eVillageID )
		{
			if ( eVillageID >= SEnum::VMI_DUNGEON_LOUNGE_RUBEN && eVillageID <= SEnum::VMI_DUNGEON_LOUNGE_HAMEL )
				return true;
			else
				return false;
		}

		bool IsBattleFieldRest( const SEnum::VILLAGE_MAP_ID eVillageID )
		{
			if ( eVillageID >= SEnum::VMI_BATTLE_FIELD_ELDER_REST_00 && eVillageID < SEnum::VMI_BATTLE_FIELD_REST_END )
				return true;
			else
				return false;
		}

		bool IsVillage( const SEnum::VILLAGE_MAP_ID eVillageID )
		{
			if ( IsVillageField( eVillageID ) )
				return true;

			if ( IsDungeonLounge( eVillageID ) )
				return true;

			if ( IsBattleFieldRest( eVillageID ) )
				return true;

			return false;
		}

		bool IsBattleField( const SEnum::VILLAGE_MAP_ID eVillageID )
		{
			if ( eVillageID >= SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01 && eVillageID < SEnum::VMI_BATTLE_FIELD_END )
				return true;
			else
				return false;
		}

		int GetWorldID(SEnum::VILLAGE_MAP_ID val);
		void SetLastPos(D3DXVECTOR3 vPos) { m_vLastPos = vPos; }
		D3DXVECTOR3 GetLastPos() { return m_vLastPos; }

		int GetLastPosLineIndex() const { return m_iLastPosLineIndex; }
		void SetLastPosLineIndex(int val) { m_iLastPosLineIndex = val; }


		bool IsValidVillage( int iVillageId );
		bool IsValidWarpField( int iVillageId );
		D3DXVECTOR3 GetWarpPos(int iVillageId );
		bool CX2LocationManager::GetIsRight( int iStartPosID_); //해당 StartPosID와 연결된 캐릭터 좌/우 방향 값 얻기

		void			SetX2StateBeforeDungeonStart( const UINT uiX2StateBeforeDungeonStart_ )
		{
			m_ComeBackInfoFromDungeon.m_uiX2StateBeforeDungeonStart = uiX2StateBeforeDungeonStart_;
		}
		UINT			GetX2StateBeforeDungeonStart() const { return m_ComeBackInfoFromDungeon.m_uiX2StateBeforeDungeonStart; }

		void			SetVillageIdBeforeDungeonStart( const UINT uiPlaceID_ )
		{
			m_ComeBackInfoFromDungeon.m_uiVillageId = uiPlaceID_;
		}
		UINT			GetVillageIdBeforeDungeonStart() const { return m_ComeBackInfoFromDungeon.m_uiVillageId; }

		void			SetLastTouchedIndexInVillageBeforeDungeonStart( const UINT uiLastTouchedIndex_ )
		{
			m_ComeBackInfoFromDungeon.m_uiLastTouchedIndex = uiLastTouchedIndex_;
		}
		UINT			GetLastTouchedIndexInVillageBeforeDungeonStart() const { return m_ComeBackInfoFromDungeon.m_uiLastTouchedIndex; }

		void			SetPositionInVillageBeforeDungeonStart( const D3DXVECTOR3& vPos_ )
		{
			m_ComeBackInfoFromDungeon.m_vPos = vPos_;
		}
		const D3DXVECTOR3& GetPositionInVillageBeforeDungeonStart() const { return m_ComeBackInfoFromDungeon.m_vPos;	}

#ifdef QUEST_GUIDE
/*
		//각 마을 포탈을 통해 갈 수 있는 필드 정보 기록
		const map< SEnum::VILLAGE_MAP_ID, vector<MapInfo> >& GetPortalMoveInfo(){return m_mapPortalMoveInfo;};*/

#endif //QUEST_GUIDE
	protected:

		void	OpenScript();

		void	LocalParsing( KLuaManager& kLuamanager, const vector< LOCAL_MAP_ID >& vecLocalMapID );
		void	VillageParsing( KLuaManager& kLuamanager, const int villageID );
		void	HouseParsing( KLuaManager& kLuamanager, const vector< HOUSE_ID >& vecHouseID );
	
		map< WORLD_MAP_ID,	WorldMapTemplet* >		m_mapWorldMapTemplet;
		map< LOCAL_MAP_ID,	LocalMapTemplet* >		m_mapLocalMapTemplet;
		map< SEnum::VILLAGE_MAP_ID, VillageTemplet* >		m_mapVillageTemplet;
		map< HOUSE_ID,		HouseTemplet* >			m_mapHouseTemplet;

		map< int, VillageStartPos* >				m_mapVillageMap;
		map< int, int>								m_mapWorldMap;

		SEnum::VILLAGE_MAP_ID	m_eCurrentVillage;
		SEnum::VILLAGE_MAP_ID	m_eOldVillage;
		D3DXVECTOR3		m_vOldPos;		
		D3DXVECTOR3		m_vCurrentPos;
		int				m_iLineIndex;
		D3DXVECTOR3     m_vLastPos;					
		int				m_iLastPosLineIndex;		// field를 벗어나기 전의 최종위치의 line index
		bool			m_bIsRight;					// 마을에서 캐릭터가 좌/우 바라보는 방향

		ComeBackInfoFromDungeon						m_ComeBackInfoFromDungeon;


#ifdef QUEST_GUIDE
/*
		//각 마을 포탈을 통해 갈 수 있는 필드 정보 기록
		map< SEnum::VILLAGE_MAP_ID, vector<MapInfo> >		m_mapPortalMoveInfo;*/

#endif //QUEST_GUIDE
};




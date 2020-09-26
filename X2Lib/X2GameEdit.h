#pragma once

class CX2State;
class CX2GameEdit
{
	public:

		enum UI_GAME_EDIT_CUSTOM_MSG
		{
//#ifdef MODIFY_GAME_EDIT_CAMERA
			UGECM_CAMERA_EDIT_CLOSE = 55000,			/// 카메라 치트 UI 닫기
			UGECM_CAMERA_EDIT_FREE_CAMERA_ON,			/// /fc true
			UGECM_CAMERA_EDIT_FREE_CAMERA_OFF,			/// /fc false
			UGECM_CAMERA_EDIT_MENUAL_CAMERA_ON,			/// /mc true
			UGECM_CAMERA_EDIT_MENUAL_CAMERA_OFF,		/// /mc false
			UGECM_CAMERA_EDIT_SLOW_FRAME_01,			/// /sf 0.1 100
			UGECM_CAMERA_EDIT_SLOW_FRAME_02,			/// /sf 0.5 100
			UGECM_CAMERA_EDIT_SLOW_FRAME_03,			/// /sf 1
//#endif //MODIFY_GAME_EDIT_CAMERA
		};

#ifdef CHEAT_CLASS_CHANGE //JHKang
		enum UI_CHANGE_CLASS_CHEAT_MSG
		{
			UCCCM_CLOSE					= 60000,
			UCCCM_DOUBLE_CLICK_CLASS,
		};

#endif //CHEAT_CLASS_CHANGE

#ifdef EFFECT_USE_LOG
		enum EFFECT_LOG_LEVEL 
		{
			ELL_NONE,
			ELL_EFFECT_LOG_1,	// 데미지 이펙트, 이펙트셋 로그 
			ELL_EFFECT_LOG_2,	// 1레벨 로그 + 파티클, 메시 로그(미구현)
		};
#endif // EFFECT_USE_LOG
	public:

		CX2GameEdit();
		virtual ~CX2GameEdit(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		void HotKeyProcess();
		void HotKeyProcess_BossRaid();
#ifdef SUMMON_MONSTER_BY_PRESS_KEY_AT_TRAINING_GAME
		void HotKeyProcess_FreeTraining();
#endif // SUMMON_MONSTER_BY_PRESS_KEY_AT_TRAINING_GAME
#ifdef MODIFY_GAME_EDIT_CAMERA
		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //MODIFY_GAME_EDIT_CAMERA


		void SetShow( bool bShow );
		bool GetShow() { return m_bShow; } 
		void SetEnable( bool bEnable );
		bool GetEnable() { return m_bEnable; }

		void SetNowStage( CX2State* pStage );


		bool ExecCommand( const WCHAR* pCommandString );
#ifdef CHEAT_COMMAND_SHORT_CUT_KEY_TEST
		void ExecuteLastCommand();
#endif CHEAT_COMMAND_SHORT_CUT_KEY_TEST


		void AddString( CX2ChatWindow::CHAT_TYPE chatType, const WCHAR* pString );
		void AddString_LUA( int chatType, const char* pString );

		void AddCommandChange( const char* pOrgCommand, const char* pChangeCommand );
#ifdef MODIFY_EDIT_HELP
		void AddHelperText( const char* pHelperText, CX2User::X2_USER_AUTH_LEVEL userLevel );
#else
		void SetCommandListHelperText( const char* pHelperText, CX2User::X2_USER_AUTH_LEVEL userLevel );
#endif //MODIFY_EDIT_HELP

		void ToggleCommandList();
		//{{ 2007. 11. 12  최육사  현재 로드된 디바이스들을 파일로 출력
		void SaveFileLoadedDevices();
		//}}
#ifdef GUILD_SKILL
		//{{ 2009-11-30  최육사
		void AdminGetGuildSPoint_LUA( int iCount );
		//}}
#endif GUILD_SKILL

#ifdef MONSTER_STATE_CHANGE_CHEAT
		void SetLastCreatedMonster(int val){ m_iLastCreatedMonster = val; }
		int GetLastCreatedMonster(){ return m_iLastCreatedMonster; }
		void MonsterStateChange(const char* pString );
#endif MONSTER_STATE_CHANGE_CHEAT

		//{{ 2010. 8. 10	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM		
		void PetCheat_LUA( int iCheatType, int iValue );
		void PetActionCheat_LUA( const char *motionName );
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2010. 9. 29	최육사	ED 모니터링 치트
#ifdef SERV_ADMIN_SHOW_ED
		void UserEdCheat_LUA( const char* nickName );
#endif SERV_ADMIN_SHOW_ED
		//}}
		//{{ 2011. 04. 16	최육사	대리 상인
//#ifdef SERV_PSHOP_AGENCY
		void ShopCheat_LUA( int iPeriod );
//#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 08. 11	최육사	투니 랜드 채널링 
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		void ChannelingCodeCheat_LUA( int iChannelingCode );
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		//}}
		//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
		void HenirRewardCountCheat_LUA( int iHenirReward );
#endif SERV_NEW_HENIR_TEST
		//}}
#ifdef NEW_HENIR_TEST
		void HenirBuffCheat_LUA( int iHenirBuff );
#endif NEW_HENIR_TEST

#ifdef ELSWORD_WAY_OF_SWORD
		void ElswordWSPCheat_LUA( int iWSP );
#endif ELSWORD_WAY_OF_SWORD

		//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
		void EDMonitoringModeCheat_LUA( bool bMode );
#endif SERV_ED_MONITORING_IN_GAME
		//}}

#ifdef SERV_RANDOM_DAY_QUEST
		void RefreshDailyQuest();
#endif SERV_RANDOM_DAY_QUEST

#ifdef ITEM_VIEW_CHEAT
		void ItemViewCheat( int itemID );
#endif ITEM_VIEW_CHEAT

#ifdef UDP_CAN_NOT_SEND_USER_KICK
		void PvpKickSet_LUA( bool bSetKick );
#endif //UDP_CAN_NOT_SEND_USER_KICK

#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
		void RequireGetDangerousValue_LUA();
#endif //SERV_BATTLE_FIELD_DANGEROUS_CHEAT

#ifdef ADDED_CHEAT_BATTLEFIELD_DANGEROUS_VALUE		
		void IncreaseFieldDangerousValue_LUA (int iAddValue_);
#endif // ADDED_CHEAT_BATTLEFIELD_DANGEROUS_VALUE


#ifdef ADDED_RELATIONSHIP_SYSTEM
		void IncreaseLoverDate_LUA (int iAddValue_);
#endif // ADDED_RELATIONSHIP_SYSTEM


		//{{ kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드
#ifdef	SERV_CHOOSE_FASTEST_HOST
		void AveragePingTime_LUA( DWORD dwAveragePingTime_ );
#endif	SERV_CHOOSE_FASTEST_HOST
		//}} kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드

#ifdef ADD_HERO_MATCH_NOTICE		/// 영웅 대전 치트 인자를 받아오기 위한 함수
#ifdef  X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE
		bool SetValueByHeroMatchWisperNotice( KLuaManagerProxy& kLuaManager_, std::vector<KLuaManagerProxyLuaValue>& vec_, string wstrCommandString, int iNowStringIndex_ );	/// 귓속말 공지
		bool SetValueByHeroMatchDeleteGroupUser( KLuaManagerProxy& kLuaManager_, std::vector<KLuaManagerProxyLuaValue>& vec_, string wstrCommandString, int iNowStringIndex_ );	/// 그룹에서 특정 유저 제거
#else   X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE
		bool SetValueByHeroMatchWisperNotice( string& wstrSlashCommandString_, string wstrCommandString, int iNowStringIndex_ );	/// 귓속말 공지
		bool SetValueByHeroMatchDeleteGroupUser( string& wstrSlashCommandString_, string wstrCommandString, int iNowStringIndex_ );	/// 그룹에서 특정 유저 제거
#endif  X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE
#endif ADD_HERO_MATCH_NOTICE

#ifdef SERV_HERO_PVP_MANAGE_LIST
		CKTDGUIDialogType GetEditChatWindow(){ return m_pDLGGameEdit; }
#endif //SERV_HERO_PVP_MANAGE_LIST

#ifdef MODIFY_GAME_EDIT_CAMERA
		void ToggleGameEditCamera();
#endif //MODIFY_GAME_EDIT_CAMERA

#ifdef SERV_DELETE_GUILD_ADD_CHEAT
		bool DeleteGUildAdd_LUA( int iGuildUID );
#endif

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		void SetCoboEventItemGet_LUA( bool bItemGive,bool bNextDay );
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		void SetValenTineCountCheat_LUA( int iCount );
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		//{{ 2011.03.04  임규수 헤니르 랭킹 삭제 치트 ( 운영자,개발자 계정 )
#ifdef SERV_DELETE_HENIR_RANKING
		bool DeleteHenirRankingRank_LUA( int nRankingType, int nRank );
		bool DeleteHenirRankingNickName_LUA( int nRankingType, const char* pNickName );
		bool DeleteHenirRankingUnitUID_LUA( int nRankingType, UidType iUnitUID );
#endif SERV_DELETE_HENIR_RANKING
		//}}

#ifdef SERV_INVISIBLE_GM
		void ToggleInvisible_LUA();
#endif SERV_INVISIBLE_GM

#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
		void RandomCubeOpen(int itemID, int iKeyID, int icount);
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG

#ifdef RIDING_SYSTEM
		void FullChargeRidingStamina( int iValue_ );
#endif // RIDING_SYSTEM

#ifdef EFFECT_USE_LOG
		void SetEffectLogLevel( EFFECT_LOG_LEVEL eLogLevel ) { m_eShowEffetLogLevel = eLogLevel; }
		EFFECT_LOG_LEVEL GetEffectLogLevel() const { return m_eShowEffetLogLevel; }
#endif // EFFECT_USE_LOG

#ifdef CHEAT_CLASS_CHANGE //JHKang
		void ToggleClassChangeCheat();
#endif //CHEAT_CLASS_CHANGE

#ifdef FIELD_BOSS_RAID
		void OpenRaidGate();
		void CloseRateGate();
		void RequireGetTotalDangerousValue();
		void IncreaseTotalDangerousValue(int iValue_);
#endif //FIELD_BOSS_RAID

#ifdef SERV_EVENT_CHECK_POWER
		void SetMultiplyer( float f );
#endif SERV_EVENT_CHECK_POWER
	protected:

		bool Reset();

		void RegisterLuaBind();
		bool OpenScriptFile( const WCHAR* pFilename );

		void AddCommandHistory( const WCHAR* pCommandMsg );
		bool SetCommandByUpDownArrow( int tempCommandHistoryIndex );

		bool							m_bShow;
		bool							m_bEnable;

		CKTDGUIDialogType					m_pDLGGameEdit;
		CX2ChatWindow*					m_pChatWindow;

		CX2State*						m_pState;

		map< string, string >			m_mapCommandChange; //     /ㅜ -> /n 이런식으로 변경

		vector< wstring >				m_CommandHistory;
		int								m_NowCommandHistoryIndex;

#ifdef MONSTER_STATE_CHANGE_CHEAT
		int							m_iLastCreatedMonster;
#endif MONSTER_STATE_CHANGE_CHEAT


		wstring							m_wstrDevHelp;
		wstring							m_wstrOperatorHelp;

#ifdef MODIFY_GAME_EDIT_CAMERA
		CKTDGUIDialogType				m_pDLGGameEditCamera;
		bool							m_bShowGameEditCamera;
#endif //MODIFY_GAME_EDIT_CAMERA

#ifdef EFFECT_USE_LOG
		EFFECT_LOG_LEVEL				m_eShowEffetLogLevel;
#endif // EFFECT_USE_LOG

#ifdef CHEAT_CLASS_CHANGE //JHKang
		CKTDGUIDialogType				m_pDLGClassChangeCheat;
		bool							m_bShowClassChangeCheat;
#endif //CHEAT_CLASS_CHANGE
};

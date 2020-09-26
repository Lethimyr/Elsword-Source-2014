#pragma once

class CX2LVUpEventMgr
{
	public:

		enum LEVEL_UP_EVENT_ID
		{
			LUEI_NONE = 0,
			LUEI_PVP_PR,
			LUEI_CHANGE_CLASS_GUIDE_FIRST,		// 1차 전직
			LUEI_LEVEL_10,
			LUEI_CHANGE_CLASS_GUIDE_SECOND,		// 2차 전직
		};

		enum LV_UP_EVENT_MGR_CUSTOM_UI_MSG
		{
			LUEMCUM_PVP_PR = 1500,
			LUEMCUM_CHANGE_CLASS_GUIDE_FIRST,
			LUEMCUM_10LV_PR,
			//#ifdef LINK_CASH_SHOP_WHEN_JOB_CHANGE
			LUEMCUM_ELSWORD_SWORD_KNIGHT,
			LUEMCUM_ELSWORD_MAGIC_KNIGHT,
			LUEMCUM_AISHA_HIGH_MAGICIAN,
			LUEMCUM_AISHA_DARK_MAGICIAN,
			LUEMCUM_RENA_SNIPING_RANGER,
			LUEMCUM_RENA_COMBAT_RANGER,
			LUEMCUM_RAVEN_SWORD_TAKER,
			LUEMCUM_RAVEN_OVER_TAKER,
			LUEMCUM_EVE_CODE_EXOTIC,
			LUEMCUM_EVE_CODE_ARCHITECTURE,
			LUEMCUM_CHUNG_FURY_GUARDIAN,
			LUEMCUM_CHUNG_SHOOTING_GUARDIAN,
			//	1차 추가 전직
			LUEMCUM_ELSWORD_SHEATH_KNIGHT,
			LUEMCUM_ARME_BATTLE_MAGICIAN,
			LUEMCUM_LIRE_TRAPPING_RANGER,
			LUEMCUM_RAVEN_WEAPON_TAKER,
			LUEMCUM_EVE_ELECTRA,
			LUEMCUM_CHUNG_SHELLING_GUARDIAN,
			LUEMCUM_ARA_LITTLE_HSIEN,
			LUEMCUM_ARA_LITTLE_DEVIL,
			LUEMCUM_ELESIS_SABER_KNIGHT,
			LUEMCUM_ELESIS_PYRO_KNIGHT,
			//#endif LINK_CASH_SHOP_WHEN_JOB_CHANGE
		};

		struct LVUpEventCondNReact
		{
			int								m_Level;

			vector< CX2Unit::UNIT_TYPE >	m_vecCondUnitType;
			vector< CX2Unit::UNIT_CLASS >	m_vecCondUnitClass;

			vector< wstring >				m_vecReactDialogFile;

#ifdef ADD_PLAY_SOUND //김창한
			vector< wstring >				m_vecSoundFileName;
#endif //ADD_PLAY_SOUND
		};

		struct LVUpEvent 
		{
			LEVEL_UP_EVENT_ID				m_EventID;
			vector<LVUpEventCondNReact*>	m_vecCondNReact;

			int								m_LastCheckLevel;
			bool							m_bCheckShow;
			vector< CKTDGUIDialogType >		m_vecDialog;

			LVUpEvent();
			~LVUpEvent();

			void Reset( int nowLevel );

			void SetNowStage( CKTDXStage* pStage );

			void OnFrameMove( double fTime, float fElapsedTime );
			void DeleteDialog();

			void CheckLevel();
		};

	public:
		CX2LVUpEventMgr(void);
		virtual ~CX2LVUpEventMgr(void);

		void SetNowStage( CKTDXStage* pStage );
		void Reset( int nowLevel );

		void OnFrameMove( double fTime, float fElapsedTime );

		bool OpenScriptFile( const WCHAR* pFileName );
		void AddLevelUpEvent_LUA();

		bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	protected:

		CX2LVUpEventMgr::LVUpEvent* GetLVUpEvent( LEVEL_UP_EVENT_ID eventID );

		CKTDXStage*							 m_pNowStage;
		map< LEVEL_UP_EVENT_ID, LVUpEvent* > m_mapLVUpEvent;
};

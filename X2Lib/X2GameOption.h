#pragma once


#include <algorithm> 

#ifdef X2OPTIMIZE_AUTOSETUP_GAMEOPTION
#include <libxml/parser.h>
#include <libxml/tree.h>
#endif//X2OPTIMIZE_AUTOSETUP_GAMEOPTION

class CX2GameOption
{
	public:
		enum OptionLevel
		{
			OL_HIGH,
			OL_MEDIUM,
			OL_LOW,
		};

		enum BlackListDenyState
		{
			BL_ON,
			BL_OFF,
			BL_ONLY_FRIEND,
		};


		enum FieldLevel
		{
			FL_HIGH,
			FL_MEDIUM,
			FL_LOW,
		};

		enum SKILL_CUTIN_SHOW_STATE //스킬컷인 출력 상태
		{
			SCSS_ONESELF = 0,	//자기자신만 보기
			SCSS_PARTY_MEMBER,	//파티원 보기
			SCSS_HIDE,			//출력하지 않기
		};		

		struct OptionList
		{
			OptionLevel		m_UnitDetail;
			OptionLevel		m_TexDetail;
			OptionLevel		m_MapDetail;
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			OptionLevel		m_eEffect;
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			bool				m_bEffect;
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

			D3DXVECTOR2		m_vResolution;
			
			bool			m_bMusic;
			bool			m_bSound;
			float			m_fMusicVolume;
			float			m_fSoundVolume;

			bool			m_bDynamicCamera;
			bool			m_bPlayGuide;
			int				m_iZoomLevel;	// 0: default, -1:zoom out, +1:zoom in
		
			int				m_iAutoSetLevel;

			OptionLevel		m_MaxUnitDetail;
			OptionLevel		m_MaxTexDetail;
			OptionLevel		m_MaxMapDetail;
			bool			m_bEnable3DSound;



			FieldLevel		m_FieldDetail;
			FieldLevel		m_MaxFieldDetail;
			//bool			m_bFriend;
			bool			m_bParty;
			bool			m_bSD;				// 필드 SD보기 모드
			bool			m_bFullScreen;
#ifdef SERV_KEY_MAPPING_INT
			bool			m_bJoyEnable;
#endif // SERV_KEY_MAPPING_INT


			// 커뮤니티 옵션
			BlackListDenyState				m_eDenyFriendship;			
			BlackListDenyState				m_eDenyInviteGuild;
			BlackListDenyState				m_eDenyParty;
			BlackListDenyState				m_eDenyPersonalTrade;
			std::map< UidType, wstring >	m_mapBlackList;
			
			// 캐릭터 이름에 나올 정보 옵션
			bool			m_bLevel;
			bool			m_bPvpRank;
			bool			m_bGuild;
			bool			m_bNothing;

			// 프라이버스 정보
			bool			m_bRefuseParty;
			bool			m_bRefuseFriend;
			bool			m_bRefusePersonalTrade;
			//{{ kimhc // 2009-10-09 // 길드초대 거부 추가
#ifdef	GUILD_MANAGEMENT
			bool			m_bRefuseGuildInvitation;
#endif	GUILD_MANAGEMENT
			//}} kimhc // 2009-10-09 // 길드초대 거부 추가

#ifdef	ADDED_RELATIONSHIP_SYSTEM	/// 커플신청 거부 추가
			bool			m_bRefuseRequestCouple;
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef	FIX_INVITE_PVP_PLAYER // 김태환		/// 대전 초대 거부 추가
			bool			m_bRefuseInvitePVP;
#endif // FIX_INVITE_PVP_PLAYER
			
			//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
			bool			m_bDisplayObtainingED;		// 채팅창에 ED획득을 표시
			bool			m_bDisplayObtainingEXP;		// 채팅창에 경험치 획득을 표시
#endif	CHAT_WINDOW_IMPROV
			//}} kimhc // 2010.3.12 //	채팅창 개편

#ifdef SERV_PVP_NEW_SYSTEM
			std::vector<int> m_vecPvpMap;
#endif

#ifdef VERTICAL_SYNC_OPTION
			bool			m_bFlashEffect;		/// 화면 번쩍임 효과
			bool			m_bVerticalSync;	/// 수직 동기화
#endif

#ifdef RULE_AGREEMENT
			bool			m_bRuleAgree;
#ifdef NEW_RULE_AGREEMENT
			bool			m_bNewRuleAgree;
#endif NEW_RULE_AGREEMENT
#endif RULE_AGREEMENT
			//{{ 2011.9.16 이준호 반디캠 작업
#ifdef BANDICAM_RECORDING
			int				m_GeneralVideoType;

			bool			m_UserSetting;
			int				m_VideoSizeW;
			int				m_VideoSizeH;
			float			m_VideoFPS;
			int				m_VideoQuality;
			int				m_AudioChannels;
			int				m_AudioSampleRate;
			int				m_VideoFormat;
			
			//콤보박스 인덱스
			//사용자 설정 DLG 생성 시, 옵션값 읽어 올 수 있도록 추가
			int				m_VideoSizeIndex;
			int				m_VideoFPSIndex;
			int				m_VideoFormatIndex;
			int				m_AudioSampleRateIndex;
#endif BANDICAM_RECORDING
			//}}

#ifdef PVP_ZOOM_CAMERA
			bool			m_bAutoCamera;
#endif
			SKILL_CUTIN_SHOW_STATE m_eSkillCutinShowState;

#ifdef SKILL_SLOT_UI_TYPE_B
			bool			m_bIsSkillUITypeA;
#endif //SKILL_SLOT_UI_TYPE_B

#ifdef SHOW_ONLY_MY_DAMAGE
			bool			m_bShowOnlyMyDamage;
#endif //SHOW_ONLY_MY_DAMAGE

#ifdef SERV_LOCAL_RANKING_SYSTEM
			bool			m_bShowRankUpInDungeon;
#endif //SERV_LOCAL_RANKING_SYSTEM

#ifdef PLAYER_ID_IN_GAME_OPTION
			std::wstring	m_wstrSavedLoginID;
#endif // PLAYER_ID_IN_GAME_OPTION

			OptionList()
			{
				m_UnitDetail			= OL_HIGH;
				m_TexDetail				= OL_HIGH;
				m_MapDetail				= OL_HIGH;
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
				m_eEffect			= OL_HIGH;
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
				m_bEffect				= true;
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION


				m_vResolution			= D3DXVECTOR2(1024, 768);
				
				m_bMusic				= true;
				m_bSound				= true;
				m_fMusicVolume			= 0.0f;
				m_fSoundVolume			= 0.0f;
				
				m_bDynamicCamera		= true;
				m_bPlayGuide			= true;

				m_iZoomLevel			= 0;
				m_iAutoSetLevel			= 0;

				m_MaxUnitDetail			= OL_HIGH;
				m_MaxTexDetail			= OL_HIGH;
				m_MaxMapDetail			= OL_HIGH;

				m_eDenyFriendship		= BL_OFF;
				m_eDenyInviteGuild		= BL_OFF;
				m_eDenyParty			= BL_OFF;
				m_eDenyPersonalTrade    = BL_OFF;

				m_bEnable3DSound		= true;

				m_FieldDetail			= FL_HIGH;
				m_MaxFieldDetail		= FL_HIGH;
				
				m_bParty				= false;
				m_bSD					= false;

				m_bLevel				= true;;
				m_bPvpRank				= true;
				m_bGuild				= true;
				m_bNothing				= true;


				m_bRefuseParty			= false;
				m_bRefuseFriend			= false;
				m_bRefusePersonalTrade	= false;

				//{{ kimhc // 2009-10-09 // 길드초대 거부 추가
#ifdef	GUILD_MANAGEMENT
				m_bRefuseGuildInvitation = false;
#endif	GUILD_MANAGEMENT
				//}} kimhc // 2009-10-09 // 길드초대 거부 추가

#ifdef ADDED_RELATIONSHIP_SYSTEM		/// 커플신청 거부 추가
				m_bRefuseRequestCouple	= false;
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef	FIX_INVITE_PVP_PLAYER // 김태환		/// 대전 초대 거부 추가
				m_bRefuseInvitePVP		= false;
#endif // FIX_INVITE_PVP_PLAYER

#ifdef OPTIMIZED_DEFAULT_RESOLUTION
				m_bFullScreen			= false;
#else OPTIMIZED_DEFAULT_RESOLUTION
				m_bFullScreen			= true;
#endif OPTIMIZED_DEFAULT_RESOLUTION

				//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
				m_bDisplayObtainingED	= true;
				m_bDisplayObtainingEXP	= true;
#endif	CHAT_WINDOW_IMPROV
				//}} kimhc // 2010.3.12 //	채팅창 개편

#ifdef SERV_KEY_MAPPING_INT
				m_bJoyEnable = false;
#endif // SERV_KEY_MAPPING_INT
#ifdef SERV_PVP_NEW_SYSTEM
				m_vecPvpMap.clear();
#endif

#ifdef VERTICAL_SYNC_OPTION
				m_bFlashEffect			= false;
				m_bVerticalSync			= false;
#endif
#ifdef RULE_AGREEMENT
				m_bRuleAgree	= false;
#ifdef NEW_RULE_AGREEMENT
				m_bNewRuleAgree = false;
#endif NEW_RULE_AGREEMENT
#endif RULE_AGREEMENT
				//{{ 2011.9.16 이준호 반디캠 작업
#ifdef BANDICAM_RECORDING
				m_GeneralVideoType			= 0;

				m_UserSetting				= false;
				m_VideoSizeW				= 0;//기본타입을 임의로 지정
				m_VideoSizeH				= 0;
				m_VideoSizeIndex			= 7;//1024*768
				m_VideoFPSIndex				= 3;//30fps
				m_VideoFormatIndex			= 0;//mpeg
				m_AudioSampleRateIndex		= 1;//16000k
				m_VideoFPS					= 30.0f;
				m_VideoQuality				= 90;
				m_AudioChannels				= 2;
				m_AudioSampleRate			= 44100;
				m_VideoFormat				=	mmioFOURCC('R','G','B',' ');
#endif BANDICAM_RECORDING
				//}}

#ifdef PVP_ZOOM_CAMERA
				m_bAutoCamera				= false;
#endif
				m_eSkillCutinShowState = SCSS_ONESELF;

#ifdef SKILL_SLOT_UI_TYPE_B
				m_bIsSkillUITypeA			= true;
#endif //SKILL_SLOT_UI_TYPE_B

#ifdef SHOW_ONLY_MY_DAMAGE
				m_bShowOnlyMyDamage			= false;
#endif //SHOW_ONLY_MY_DAMAGE

#ifdef SERV_LOCAL_RANKING_SYSTEM
				m_bShowRankUpInDungeon		= false;
#endif //SERV_LOCAL_RANKING_SYSTEM

#ifdef PLAYER_ID_IN_GAME_OPTION
				m_wstrSavedLoginID			= L"";
#endif // PLAYER_ID_IN_GAME_OPTION
			}
			
		};

	public:
		CX2GameOption(void);
		virtual ~CX2GameOption(void);

		bool OpenScriptFile();
		bool SaveScriptFile();
				
		bool SetAutoOption( int index );
		bool UpAutoOption();
		bool DownAutoOption();
		int GetAutoSetLevel() { return m_OptionList.m_iAutoSetLevel; }

		void SetTexDetail( OptionLevel optionLevel, bool bForce = false );
		void UpTexDetail();
		void DownTexDetail();

		void SetUnitDetail( OptionLevel optionLevel, bool bForce = false );
		void UpUnitDetail();
		void DownUnitDetail();

		void SetMapDetail( OptionLevel optionLevel, bool bForce = false );
		void UpMapDetail();
		void DownMapDetail();

#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		void SetEffectDetail( OptionLevel optionLevel, bool bForce = false );
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		void SetEffectDetail( bool bEffect, bool bForce = false );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		void UpEffectDetail();
		void DownEffectDetail();

		void SetFieldDetail( FieldLevel optionLevel, bool bForce = false );
		void UpFieldDetail();
		void DownFieldDetail();
		FieldLevel GetFieldOptionOld() { return m_eFieldLevelOld; }

		bool GetFieldParty() { return m_OptionList.m_bParty; }
		void SetFieldParty( bool bParty );
		void SetFieldFriend( bool bFriend );

		bool GetFieldSD(){ return m_OptionList.m_bSD; }
		void SetFieldSD(bool bSD);
		void ToggleFieldSD() { SetFieldSD(!m_OptionList.m_bSD); }

		void SetDynamicCamera( bool bCamera ){ m_OptionList.m_bDynamicCamera = bCamera; }
		void SetPlayGuide( bool bGuide, bool bUpdateDB = true );

		void SetEnable3DSound( bool bEnable3DSound );
		void SetMusic( bool bMusic );
		void SetSound( bool bSound );
		void SetMusicVolume( float fMusicVolume );
		float GetMusicVoluem() { return m_OptionList.m_fMusicVolume; }
		void SetSoundVolume( float fSoundVolume );
		float GetSoundVolume() { return m_OptionList.m_fSoundVolume; }
		void SetResolution( DWORD dwWidth, DWORD dwHeight, int iColorBit = 32 );
		const D3DXVECTOR2& GetResolution() { return m_OptionList.m_vResolution; }

		bool GetIsFullScreen() const { return m_OptionList.m_bFullScreen; }




		void SetDenyFriendship(CX2GameOption::BlackListDenyState val) { m_OptionList.m_eDenyFriendship = val; }
		void SetDenyInviteGuild(CX2GameOption::BlackListDenyState val) { m_OptionList.m_eDenyInviteGuild = val; }
		void SetDenyParty(CX2GameOption::BlackListDenyState val) { m_OptionList.m_eDenyParty = val; }
		void SetDenyPersonalTrade(CX2GameOption::BlackListDenyState val) { m_OptionList.m_eDenyPersonalTrade = val; }

		void SetBlackList( std::map< UidType, wstring >& mapBlackList )
		{
			m_OptionList.m_mapBlackList = mapBlackList;
		}
		
		bool AddBlackList( UidType iUnitUID, wstring wstrNickName )
		{
			std::map< UidType, wstring >::iterator it;
			it = m_OptionList.m_mapBlackList.find( iUnitUID );
			if( it == m_OptionList.m_mapBlackList.end() )
			{
				m_OptionList.m_mapBlackList[ iUnitUID ] = wstrNickName;
				return true;
			}
			return false;
		}

		bool RemoveBlackList( UidType iUnitUID, wstring wstrNickName )
		{
			if( m_OptionList.m_mapBlackList.erase( iUnitUID ) > 0 )
			{
				return true;
			}
			return false;
		}

		void CameraZoomIn( int iGap )
		{

			m_OptionList.m_iZoomLevel += iGap;

			if( m_OptionList.m_iZoomLevel < -1 )
				m_OptionList.m_iZoomLevel = -1;

#ifdef CAMERA_ZOOM_IN_RANGE_EXPAND
			if( m_OptionList.m_iZoomLevel > 2 )
				m_OptionList.m_iZoomLevel = 2;
#else
			if( m_OptionList.m_iZoomLevel > 1 )
				m_OptionList.m_iZoomLevel = 1;
#endif //CAMERA_ZOOM_IN_RANGE_EXPAND
		}

		void SetCameraZoomIn( int iLev )
		{
			m_OptionList.m_iZoomLevel = iLev;

			if( m_OptionList.m_iZoomLevel < -1 )
				m_OptionList.m_iZoomLevel = -1;

#ifdef CAMERA_ZOOM_IN_RANGE_EXPAND
			if( m_OptionList.m_iZoomLevel > 2 )
				m_OptionList.m_iZoomLevel = 2;
#else
			if( m_OptionList.m_iZoomLevel > 1 )
				m_OptionList.m_iZoomLevel = 1;
#endif //CAMERA_ZOOM_IN_RANGE_EXPAND
		}
//}}

#ifdef MODIFY_FRUSTUM
		float GetCameraDistance();
#else
		float GetCameraDistance()
		{
			switch( m_OptionList.m_iZoomLevel )
			{
			default:
			case 0:
				{
					return 1200.f;
				} break;

			case 1:
				{
					return 900.f;
				} break;

			case -1:
				{
					return 1500.f;
				} break;

			case -2:
				{
					return 1800.f;
				} break;
			}
		}
#endif
		
		void ApplyAllOption( bool bForce = false );

//{{ robobeg : 2013-09-17
		//OptionList* GetOptionList(){ return &m_OptionList; }
        OptionList& GetOptionList(){ return m_OptionList; }
//}} robobeg : 2013-09-17
		vector<OptionList>& GetOptionListPreset() { return m_vecOptionListPreset; }
		
		void SendFieldOption();
		void SetChangeFieldOption(bool val) { m_bChangeFieldOption = val; }
		bool GetChangeFieldOption() { return m_bChangeFieldOption; }

		// 기타옵션 캐릭터 정보셋팅
		void SetCharInfoLevel( bool bVal )		{ m_OptionList.m_bLevel = bVal; }
		void SetCharInfoPvp( bool bVal )		{ m_OptionList.m_bPvpRank = bVal; }
		void SetCharInfoGuild( bool bVal )		{ m_OptionList.m_bGuild = bVal; }
		void SetCharInfoNothing( bool bVal )	{ m_OptionList.m_bNothing = bVal; }

		// 프라이버스 정보셋팅		
		void SetRefuseParty(CX2GameOption::BlackListDenyState val);
		void SetRefuseFriend(CX2GameOption::BlackListDenyState val);
		void SetRefusePersonalTrade(CX2GameOption::BlackListDenyState val);

		void SetRefuseParty(bool val);
		void SetRefuseFriend(bool val);
		void SetRefusePersonalTrade(bool val);

		//{{ kimhc // 2009-10-09 // 길드초대 거부 추가
#ifdef	GUILD_MANAGEMENT
		void SetRefuseGuildInvitation( CX2GameOption::BlackListDenyState val );
		void SetRefuseGuildInvitation( bool val );
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-10-09 // 길드초대 거부 추가

#ifdef ADDED_RELATIONSHIP_SYSTEM	/// 커플신청 거부 추가
		void SetRefuseRequestCouple( CX2GameOption::BlackListDenyState val );
		void SetRefuseRequestCouple( bool val );
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef FIX_INVITE_PVP_PLAYER // 김태환	/// 대전 초대 거부 추가
		void SetRefuseInvitePVP( CX2GameOption::BlackListDenyState val );
		void SetRefuseInvitePVP( bool val );
#endif // FIX_INVITE_PVP_PLAYER

		void SetFullScreen( bool bFullScreen );

		//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
		void SetDisplayObtainingED( bool bVal ) { m_OptionList.m_bDisplayObtainingED = bVal; }
		bool GetDisplayObtainingED() const { return m_OptionList.m_bDisplayObtainingED; }
		void SetDisplayObtainingEXP( bool bVal ) { m_OptionList.m_bDisplayObtainingEXP = bVal; }
		bool GetDisplayObtainingEXP() const { return m_OptionList.m_bDisplayObtainingEXP; }
#endif	CHAT_WINDOW_IMPROV
		//}} kimhc // 2010.3.12 //	채팅창 개편

#ifdef SERV_KEY_MAPPING_INT
		void SetJoyEnable( bool bEnable );
#endif // SERV_KEY_MAPPING_INT
		//{{ JHKang // 2010.8.27 // 창 모드 전체 화면 구현
#ifdef TOGGLE_WINDOW_FULLSCREEN
		void ToggleWindowFullScreen( bool bFullScreen );
#endif TOGGLE_WINDOW_FULLSCREEN
		//}} JHKang // 2010.8.27 // 창 모드 전체 화면 구현

#ifdef VERTICAL_SYNC_OPTION
		void SetFlashEffect( IN bool bVal_ );
		bool GetFlashEffect() const { return m_OptionList.m_bFlashEffect; }
		void SetVerticalSync( IN bool bVal_ );
		bool GetVerticalSync() const { return m_OptionList.m_bVerticalSync; }
#endif

#ifdef RULE_AGREEMENT
		void SetRuleAgree( bool bVal ) { m_OptionList.m_bRuleAgree = bVal; };
		bool GetRuleAgree() const { return m_OptionList.m_bRuleAgree; };
#ifdef NEW_RULE_AGREEMENT
		void SetNewRuleAgree( bool bVal ) { m_OptionList.m_bNewRuleAgree = bVal; };
		bool GetNewRuleAgree() const { return m_OptionList.m_bNewRuleAgree; };
#endif NEW_RULE_AGREEMENT
#endif RULE_AGREEMENT
		//{{ 2011.9.16 이준호 반디캠 작업
#ifdef BANDICAM_RECORDING
		void SetGeneralVideoType( int iVal ) { m_OptionList.m_GeneralVideoType = iVal; };
		int GetGeneralVideoType() const { return m_OptionList.m_GeneralVideoType; };

		void SetUserSetting(bool bVal){ m_OptionList.m_UserSetting = bVal; };
		bool GetUserSetting() const { return m_OptionList.m_UserSetting; };


		//해상도 옵션(콤보박스)
		void SetVideoSizeW( int iVal ) { m_OptionList.m_VideoSizeW = iVal; };
		void SetVideoSizeH( int iVal ) { m_OptionList.m_VideoSizeH = iVal; };
		void SetVideoSizeIndex( int iVal) { m_OptionList.m_VideoSizeIndex = iVal; };

		int GetVideoSizeW() const { return m_OptionList.m_VideoSizeW; };
		int GetVideoSizeH() const { return m_OptionList.m_VideoSizeH; };
		int GetVideoSizeIndex() const {return m_OptionList.m_VideoSizeIndex; };
		
		//화질
		void SetVideoQuality( int iVal ) { m_OptionList.m_VideoQuality = iVal; };
		int GetVideoQuality() const { return m_OptionList.m_VideoQuality; };

		//프레임(콤보박스)
		void SetVideoFPS( float fVal ) { m_OptionList.m_VideoFPS = fVal; };
		void SetVideoFPSIndex( int iVal ) { m_OptionList.m_VideoFPSIndex = iVal; };

		float GetVideoFPS() const { return m_OptionList.m_VideoFPS; };
		int	GetVideoFPSIndex() const {return m_OptionList.m_VideoFPSIndex; };
		
		//비디오 포맷(콤보박스)
		void SetVideoFormat( int iVal ) { m_OptionList.m_VideoFormat = iVal; };
		void SetVideoFormatIndex(int iVal) { m_OptionList.m_VideoFormatIndex = iVal; };

		int GetVideoFormat() const { return m_OptionList.m_VideoFormat; };
		int GetVideoFormatIndex() const {return m_OptionList.m_VideoFormatIndex; };

		//오디오 채널
		void SetAudioChannels( int iVal ) { m_OptionList.m_AudioChannels = iVal; };
		int GetAudioChannels() const { return m_OptionList.m_AudioChannels; };

		//오디오 음질(콤보박스)
		void SetAudioSampleRate( int iVal ) { m_OptionList.m_AudioSampleRate = iVal; };
		void SetAudioSampleRateIndex( int iVal) { m_OptionList.m_AudioSampleRateIndex = iVal; };

		int GetAudioSampleRate() const { return m_OptionList.m_AudioSampleRate; };
		int GetAudioSampleRateIndex() const {return m_OptionList.m_AudioSampleRateIndex; };		
#endif BANDICAM_RECORDING
		//}}

#ifdef PVP_ZOOM_CAMERA
		void SetAutoCamera(bool bVal) { m_OptionList.m_bAutoCamera = bVal; }
		bool GetAutoCamera() { return m_OptionList.m_bAutoCamera; }
		void ResetAutoCamera();

		void SetSubZoomCameraLv(int iLv) { m_iSubZoomCameraLv = iLv; }
		int GetSubZoomCameraLv() { return m_iSubZoomCameraLv; }
		void SetSubZoomCameraTimer( float fTime ) 
		{
			m_fSubZoomCameraTimer = fTime;
		}
		float GetSubZoomCameraTimer() { return m_fSubZoomCameraTimer; }
		void DoSubZoomCameraTimer( float fElapsedTime )
		{
			if( m_fSubZoomCameraTimer > 0.f )
			{
				m_fSubZoomCameraTimer -= fElapsedTime;
				if( m_fSubZoomCameraTimer < 0.f )
					m_fSubZoomCameraTimer = 0.f;
			}
		}
#endif

#ifdef ADD_MUSIC_SUB_VOLUME
		float GetMusicSubVolume() { return m_fMusicSubVolume; }
		void SetMusicSubVolume(float fVal) 
		{ 
			m_fMusicSubVolume = fVal; 
			SetMusicVolume( GetMusicVoluem() );
		}
		bool GetMusic() { return m_OptionList.m_bMusic; }
#endif

		void SetStateCutinShowState( SKILL_CUTIN_SHOW_STATE eCutinShowState_){m_OptionList.m_eSkillCutinShowState = eCutinShowState_;}
		SKILL_CUTIN_SHOW_STATE GetStateCutinShowState() const {return m_OptionList.m_eSkillCutinShowState;}
#ifdef SKILL_SLOT_UI_TYPE_B
		void SetIsSkillUITypeA( bool bIsSkillUITypeA_, bool bShowBuffIcon_ = true );
		bool GetIsSkillUITypeA() const { return m_OptionList.m_bIsSkillUITypeA; }
#endif //SKILL_SLOT_UI_TYPE_B

#ifdef SHOW_ONLY_MY_DAMAGE
		void SetShowOnlyMyDamage( bool bShowOnlyMyDamage_ ) { m_OptionList.m_bShowOnlyMyDamage = bShowOnlyMyDamage_; }
		bool GetShowOnlyMyDamage() const { return m_OptionList.m_bShowOnlyMyDamage; }
#endif //SHOW_ONLY_MY_DAMAGE

#ifdef SERV_LOCAL_RANKING_SYSTEM
		void SetShowRankUpInDungeon( bool bShow ){ m_OptionList.m_bShowRankUpInDungeon = bShow; }
		bool GetShowRankUpInDungeon() const { return m_OptionList.m_bShowRankUpInDungeon; }
#endif //SERV_LOCAL_RANKING_SYSTEM

	private:
		OptionList			m_OptionList;
		vector<OptionList>	m_vecOptionListPreset;

		FieldLevel	m_eFieldLevelOld;
		bool		m_bPartyOld;
		bool		m_bFriendOld;
		bool		m_bChangeFieldOption;

		//{{ JHKang // 2010.8.27 // 창 모드 전체 화면 구현
#ifdef TOGGLE_WINDOW_FULLSCREEN
		RECT		m_WindowPosition;
#endif TOGGLE_WINDOW_FULLSCREEN
		//}} JHKang // 2010.8.27 // 창 모드 전체 화면 구현

#ifdef PVP_ZOOM_CAMERA
		int	m_iSubZoomCameraLv;
		float m_fSubZoomCameraTimer;

		float m_fDungeonZoom1;
		float m_fDungeonZoom2;
		float m_fDungeonZoom3;
		float m_fPvpZoom1;
		float m_fPvpZoom2;
		float m_fPvpZoom3;
#endif

#ifdef ADD_MUSIC_SUB_VOLUME
		float m_fMusicSubVolume;
#endif

#ifdef X2OPTIMIZE_AUTOSETUP_GAMEOPTION
	private:
		void _DecideProperGraphicOption();
		int _CalcCpuScore();
		void _CalcBestGraphicOption( int iCpuTotalScore, int iGpuTotalScore );
		OptionLevel _CalcHighMiddleLow( const xmlNode* pkElement, int iCpuTotalScore, int iGpuTotalScore );
		bool _CalcOnOff( const xmlNode* pkElement, int iCpuTotalScore, int iGpuTotalScore );
#endif//X2OPTIMIZE_AUTOSETUP_GAMEOPTION
};

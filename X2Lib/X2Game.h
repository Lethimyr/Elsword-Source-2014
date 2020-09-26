#pragma once

#ifdef REDUCED_PRECOMPILED_HEADER_TEST
#include "./X2GUUser.h"
#endif REDUCED_PRECOMPILED_HEADER_TEST

//{{ 2012. 10. 31	박세훈	Merge 랜선랙 방지 작업-릴레이를 기본적으로 사용한다.
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
#include <boost/timer.hpp>
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}




class CX2Transfer;
class CX2Packet;
class CX2GUNPCFunc;

//{{ kimhc // 실시간 엘소드 중 실시간 ED 획득
#ifdef	REAL_TIME_ELSWORD
namespace	_CONST_X2GAME_
{
	const	UINT		g_iMaxED		= 2000000000;	// 유저가 최대 한도로 가질 수 있는 ED양(20억)

// USE_MAXLEVEL_LIMIT_VAL 때문에 해외팀 주석 처리
/*#ifdef _MAXLEVEL_LIMIT_30_
	const	int			g_iMaxLevel		= 30;
#elif defined _MAXLEVEL_LIMIT_40_
	const	int			g_iMaxLevel		= 40;
#else
	const	int			g_iMaxLevel		= 65;
#endif*/
	USE_MAXLEVEL_LIMIT_VAL
}
#endif	REAL_TIME_ELSWORD
//}} kimhc // 실시간 엘소드 중 실시간 ED 획득

class CX2Game : public CKTDXStage
{
	public:
#ifdef KHELP_SYSTEM_TEST
		DECLARE_HELP_SYSTEM( CX2Game );
#endif KHELP_SYSTEM_TEST

		void Debug_RenderHelp( int ix_, int iy_ );



	public:
		enum GAME_STATE
		{
			GS_INIT,
			GS_LOADING,
			GS_PLAY,
			GS_END,
		};

		enum GAME_TYPE
		{
			GT_NONE = 0,
			GT_PVP,
			GT_DUNGEON,
			GT_BATTLE_FIELD,
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			GT_NUMS
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		};

		enum CONNECT_TYPE
		{
			CT_CONNECT_SUCCESS,
			CT_START_GAME,
			CT_INTRUDE,
			CT_CONNECT_TO_INTRUDER,
		};

		enum ALARM_COLOR_TYPE
		{
			ACT_BLACK = 0,
			ACT_RED,
			ACT_BLUE,
			ACT_YELLOW,
			ACT_ORANGE,
			ACT_MAGENTA,
		};

		struct DangerAlarm
		{
			UidType				m_uid;			
			int					m_iStateId;
			float				m_fDelayTime;
			float				m_fShowTime;			
			ALARM_COLOR_TYPE	m_eColor;
			bool				m_bDanger;
			bool				m_bRepeat;
			wstring				m_wstrMessage;
#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
			X2_UK_FONT			m_eFontType;
#endif //SERV_HALLOWEEN_EVENT_2013


			DangerAlarm()
			{
				Init();
			}

			void Init()
			{
				m_uid = -1;
				m_bDanger = false;
				m_wstrMessage = L"";
				m_fDelayTime = 0.f;
				m_bRepeat = false;
				m_eColor = ACT_BLACK;
				m_fShowTime = 0.f;
				m_iStateId = -1;
#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
				m_eFontType = XUF_DODUM_20_BOLD;
#endif //SERV_HALLOWEEN_EVENT_2013
			}
		};

	public:
		struct CreateNPCData
		{
			CX2UnitManager::NPC_UNIT_ID		m_NPCID;
			int								m_Level;
			bool							m_bActive;
			int								m_UID;
			D3DXVECTOR3						m_UnitPos;
			bool							m_bRight;
			float							m_DelayTime;		
			int								m_iKeyCode;

			CX2Room::TEAM_NUM				m_eTeamNum;
			CX2NPCAI::NPC_AI_TYPE			m_eAIType;
			UidType							m_AllyUID;
#ifdef SUMMON_MONSTER_CARD_SYSTEM

	#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-12
			CX2GUNPC::NPC_CREATE_TYPE		m_eCreateMonsterType;	/// 몬스터 생성 타입
	#else // SERV_NEW_DEFENCE_DUNGEON
			bool							m_bIsSummonMonster;		//몬스터 카드로 소환되었는지에 대한 여부
	#endif // SERV_NEW_DEFENCE_DUNGEON

#endif //SUMMON_MONSTER_CARD_SYSTEM
		};

		struct NewGameUser
		{
			UidType		m_UnitUID;
			float		m_fNewUserTime;
			bool		m_bStart;
			int			m_StartPosIndex;

			NewGameUser()
			{
				m_UnitUID			= -1;
				m_fNewUserTime		= 0.0f;
				m_bStart			= false;
				m_StartPosIndex		= 0;
			}
		};

		struct ScoreTime
		{
			KProtectedType<int>		m_Score;
			float					m_fTime;

			ScoreTime()
			{
				m_Score = 0;
				m_fTime = 0.0f;
			}
		};


		// 퀘스트 아이템 수집 정보를 화면에 실시간으로 보여주기 위한 
		struct InfoTextManager
		{
		public:
			vector<CX2TextManager::CX2Text*>		m_vecpText;

			int				m_iMaxLine;
			D3DXVECTOR2		m_vPos;


		public:
			InfoTextManager();
			~InfoTextManager();

			void ClearAllText();
			void RemoveText( CX2TextManager::CX2Text* pText );
			void MakeEmptyLine( bool bScrollUpside, int nCount );
			CX2TextManager::CX2Text* PushText( int fontIndex, const WCHAR* pTextMsg, 
					D3DXCOLOR color, D3DXCOLOR outlineColor, 
					DWORD dtType /*= DT_LEFT*/, float fLineSpace /*= 1.0f*/, float fLifeTime /*= 1.0f*/ );
		};

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD	
		class IntrudeData : public KJohnThread
		{
		public:

			IntrudeData()
			{
				m_pSlotData		= NULL;
				m_UnitUID		= 0;
				m_StartPosIndex = 0;
				m_pGame			= NULL;
			}
			virtual ~IntrudeData()
			{

			}

			virtual bool BeginThread()
			{
				return KJohnThread::BeginThread();
			}//BeginThread()

			virtual void EndThread(DWORD dwTimeout_ = INFINITE)
			{
				KJohnThread::EndThread( dwTimeout_ );
			}//EndThread()

			virtual DWORD RunThread()
			{
				m_pGame->AddUserUnitIntruder( m_pSlotData, m_pSlotData->m_Index );
				m_pGame->IntruderStart( m_UnitUID, m_StartPosIndex );
				m_pGame->GetMinorParticle()->CreateSequence( NULL,  L"INTRUDE_NOT", 0,0,0, 1000,1000, -1, 1 );
				m_pGame->GetMinorParticle()->CreateSequence( NULL,  L"INTRUDE_NOT_ADD", 0,0,0, 1000,1000, -1, 1 );

				m_pGame->InvalidEventThread( GetThreadHandle() );
				
				return 0;
			}//RunThread()

			CX2Room::SlotData*	m_pSlotData;
			UidType				m_UnitUID;
			int					m_StartPosIndex;
			CX2Game*			m_pGame;

		};//class TThread : public KJohnThread
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD		



#ifdef REVERSE_GRAVITY_TEST
		struct ReverseGravityData
		{

			float m_fTimeLeft;
			float m_fGravity;
			D3DXVECTOR3 m_vCenterPos;
			float m_fRange;
			int m_iTeamNumber;


			ReverseGravityData()
			: m_fTimeLeft( 0.f )
			, m_fGravity( 0.f )
			, m_vCenterPos( 0, 0, 0 )
			, m_fRange( 0.f )
			, m_iTeamNumber( 0 )
			{
			}

			bool IsInRange( 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                float   fElapsedTime,
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                const D3DXVECTOR3& vPos, float& fSpeeYDelta, D3DXVECTOR3& vSpeedRotateDelta, int iTeamNumber );

			void OnFrameMove( double fTime, float fElapsedTime )
			{
				if( m_fTimeLeft > 0.f )
				{
					m_fTimeLeft -= fElapsedTime;
					if( m_fTimeLeft < 0.f )
					{
						m_fTimeLeft = 0.f;
					}
				}
			}



		};

#endif REVERSE_GRAVITY_TEST

#ifdef	ELOG_STATISTICS_NEW_DATA
		struct  KFPS_STATISTICS
		{
			float       m_fAverageFps;
			unsigned    m_uNumOfFps;

			KFPS_STATISTICS()
				: m_fAverageFps( 0.f )
				, m_uNumOfFps( 0 )
			{
			}//KFPS_STATISTICS

			void        AddNewFps( float fFPS );
		};//struct  KFPS_STATISTICS

		struct  KGAME_STATISTICS
		{
			KFPS_STATISTICS
				m_kAverageFps;

			KGAME_STATISTICS()
				: m_kAverageFps()
			{
			}

			void        Init()
			{
				m_kAverageFps = KFPS_STATISTICS();
			}

			void        Send_EGS_FRAME_AVERAGE_REQ( int iGameType );
			void        GatherPeriodicStatistics();
		};//KGAME_STATISTICS

#endif	ELOG_STATISTICS_NEW_DATA


#ifdef MODIFY_DUNGEON_STAGING
		struct CinematicUI
		{  //화면 위,아래 검게 만드는 UI
			bool						m_bStart;				
			bool						m_bEnd;
			float						m_fTimer;
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			float						m_fDeleteTime;	// -1 이 아니면 해당 초에 맞게 동작, 그렇다면 원래 소스대로 동작
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hCinematicTop;
			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hCinematicBottom;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticleHandle				m_hCinematicTopParticle;			
			CKTDGParticleSystem::CParticleHandle				m_hCinematicBottomParticle;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticle*						m_pCinematicTopParticle;			
			CKTDGParticleSystem::CParticle*						m_pCinematicBottomParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			HRESULT OnFrameMove ( double fTime, float fElapsedTime );

			CinematicUI();
			~CinematicUI();

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			void Init()
			{
				m_bStart					= false;
				m_bEnd						= false;
				m_fTimer					= -1;
				m_fDeleteTime				= -1;	
			}
			bool GetIsFadeInState () { return m_bStart; }
			bool GetIsFadeOutState () { return m_bEnd; }
			bool GetIsCinematicUIActive () { return ( false == m_bStart  && false  == m_bEnd );  }
			
			void SetCinematicUI( bool bVal, float fTime_ = -1.0f );	// CinematicUI 가 비활성 중이면 fTime 만큼 활성화, fTime 을 사용하지 않으면 옛날 CinematicUI 가 나옴
#else // SERV_BATTLEFIELD_MIDDLE_BOSS
			void SetCinematicUI(bool bVal);


#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
		};
#endif //MODIFY_DUNGEON_STAGING
	public:
		CX2Game();
		virtual ~CX2Game(void);

		virtual void Release() = NULL;		/// X2Game의 소멸자에서 실행되야 한는 것들을 이곳으로 옮겨서 실행함
		//virtual void				P2PSetPeer();
		//virtual void				P2PSetPeer( CX2Unit* pUnit );
		//virtual void				P2PDeletePeer( UidType unitUID );
		//virtual void				P2PConnectTest();
		//virtual bool				P2PConnectTestResult();

		void						SetBuffInfoPacketToNpc( const KEGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT& kEvent_ );
		void						SetBuffInfoPacketToGUUser( const KEGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT& kEvent_ );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        WORD                        Handler_P2P_UnitUserSyncPack( const BYTE* pbyData, BYTE byNumPacks, WORD wMaxSize, UidType uidUnitUID, DWORD dwFrameMoveCount );
		WORD                        Handler_P2P_UnitNpcSyncPack( const BYTE* pData, BYTE byNumNPCs, WORD wMaxSize, UidType uidUnitUID, DWORD dwFrameMoveCount );
		WORD                        Handler_P2P_UnitPetSyncPack( const BYTE* pData, BYTE byNumNPCs, WORD wMaxSize );
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        WORD                        Handler_P2P_UnitNpcMiniSyncPack( const BYTE* pData, BYTE byNumNPCs, WORD wMaxSize, UidType uidUnitUID, DWORD dwFrameMoveCount );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		virtual void				Handler_P2P_UnitNpcSyncPack( CKTDNUDP::RecvData* pRecvData, bool bFirst_ = false );
//		virtual void				Handler_P2P_UnitPetSyncPack( CKTDNUDP::RecvData* pRecvData );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		virtual void				P2PPacketHandler();
		virtual bool				P2PPacketHandler( CKTDNUDP::RecvData* pRecvData );

#ifdef	BATTLE_FIELD_TEST
		void						WriteBroadCastUser();
#endif	BATTLE_FIELD_TEST

		virtual void				GameLoading( CX2Room* pRoom );
		virtual void				SystemLoading();
		virtual void				WorldLoading(){}
		virtual void				UnitLoading();
		virtual void				EtcLoading();

		CX2GUUser*					CreateGUUser( const CX2Room::SlotData* pSlotData_, int iSlotIndex_ );
		virtual	void				AddUserUnit( CX2Room::SlotData* pSlotData, int slotIndex );
		virtual	CX2GUUser*			AddUserUnitIntruder( CX2Room::SlotData* pSlotData, int slotIndex );
		virtual	void				AddUserUnitIntruderByLoader( CX2Unit* pUnit );
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		virtual	void				IntruderStart( CX2GUUser* pX2User, int startPosIndex = -1 );
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		virtual	void				IntruderStart( UidType unitUID, int startPosIndex = -1 );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		virtual void				GameStart();
		virtual void				GameEnd();

		virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT				OnFrameRender();
		virtual bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual HRESULT				OnResetDevice();
		virtual HRESULT				OnLostDevice();

		virtual bool				DetermineLastKill() = NULL;

		virtual void				StopOtherUnit( CX2GameUnit* pNonStopUnit, float fStopTime, D3DXVECTOR3* pPos = NULL, float fDistance = -1, bool bOtherTeam = false );
		virtual void				StopAllUnit( float fStopTime, D3DXVECTOR3* pPos = NULL, float fDistance = -1, CX2GameUnit* pMyUnit = NULL, float fMyStopTime = -1.0f );

		virtual void				Stop2OtherUnit( CX2GameUnit* pNonStopUnit, float fStopTime, D3DXVECTOR3* pPos = NULL, float fDistance = -1, bool bOtherTeam = false );
		virtual void				Stop2AllUnit( float fStopTime, D3DXVECTOR3* pPos = NULL, float fDistance = -1, CX2GameUnit* pMyUnit = NULL, float fMyStopTime = -1.0f );

		virtual void				ChangeFocusUnit(CX2GameUnit* pGameUnit = NULL);	// 곧 지워질 게임 유닛 을 보내줘서 포커스 안잡히도록..
		virtual void				ResetFocusUnit();
		virtual void				SetTimerFocusUnit( CX2GameUnit* pFocusUnit, float fFocusTime = 0.f, float fDistacne = -1.f );
#ifdef INIT_FOCUS_UNIT
		virtual void				InitFocusUnit();
#endif // INIT_FOCUS_UN

		CKTDGLineMap*				GetLineMap()				
		{ 
			if( NULL != GetWorld() ) 
				return GetWorld()->GetLineMap(); 

			return NULL;
		}
#ifdef X2TOOL
		void						SetWorld(CX2World* pWorld)	{ m_pWorld = pWorld; }
#endif
		virtual CX2World*			GetWorld()					{ return m_pWorld; }
		virtual CX2Camera*			GetX2Camera()				{ return m_pCamera; }
		CX2DamageEffect*	        GetDamageEffect()			{ return m_pDamageEffect; }
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        bool				        IsHost()					{ return m_uidHostSlotUID != 0 && m_uidHostSlotUID == m_uidMySlotUID; }
        UidType                     GetHostSlotUID()            { return m_uidHostSlotUID; }
        UidType                     GetMySlotUID()              { return m_uidMySlotUID; }
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		virtual bool				IsHost()					{ return m_bHost; }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
        bool                        IsPacketSendFrame()         { return m_iPacketSendFrameMoveCount == 0; }
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
		
		virtual CX2GUUser*			GetMyUnit()	const			{ return ( null != m_optrMyUnit ? static_cast<CX2GUUser*>( m_optrMyUnit.GetObservable() ) : NULL ); }
		virtual CX2GameUnit*		GetFocusUnit() const		{ return ( null != m_optrFocusUnit ? m_optrFocusUnit.GetObservable() : NULL ); }
		D3DXVECTOR3					GetFocusUnitPos_LUA();
		
		virtual CX2GUUser*			GetUserUnitByUID( UidType unitUID );
		virtual CX2GUNPC*			GetNPCUnitByType( int iNPCID );
		virtual CX2GUNPC*			GetNPCUnitByUID( int npcUID );
		virtual CX2GUNPC*			GetNPCUnitByKeyCode( int iKeyCode );
		virtual CX2GameUnit*		GetUnit( int index );	
		virtual CX2GUUser*			GetUserUnit( int index );	
		virtual CX2GUNPC*			GetNPCUnit( int index );
		
		virtual void				SetNPCUnit(int index, CX2GUNPC* pCX2GUNPC){ m_NPCUnitList[index] = pCX2GUNPC; };
		virtual void				AddUnit( CX2GameUnit* pGameUnit ){ m_UnitList.push_back(pGameUnit); }
		void						KillUnit( bool bNPCKiller, UidType killerUID, UidType maxDamageKillerUID, bool bNPCKilled, UidType killedUID );
		void						SetKillScore( UidType userUnitUID, int killNum, int killedNum, int mdKillNum );
		int							GetKillCount( UidType userUnitUID );
		int							GetKillAssistCount( UidType userUnitUID );


		void						ChangeUserUnitIndex( const UINT uiFirstIndex_, const UINT uiSecondIndex_ );
		virtual bool				DeleteUserUnit( int index );
		virtual bool				DeleteUserUnitByUID( UidType unitUID );
		virtual bool				DeleteUserUnitByData( CX2GameUnit* pGameUnit );
		virtual bool				DeleteNPCUnit( int index );
		virtual bool				DeleteNPCUnitByUID( const UINT uiNpcUid_ );
		virtual bool				DeleteAllNPCUnit();
		
		void                        AddNPCResourcesToDeviceList( CX2UnitManager::NPC_UNIT_ID npcID );
		void                        RemoveAllNPCResourcesFromUnitLoader();

		virtual int					GetUserUnitListSize()	{ return m_UserUnitList.size(); }
		virtual int					GetNPCUnitListSize()	{ return m_NPCUnitList.size(); }
		virtual int					GetUnitNum()			{ return (int)m_UnitList.size(); }
		virtual int					GetUserUnitNum();
		virtual int					GetNPCUnitNum();
		virtual int					GetEmptyNPCIndex();
		int							LiveUserUnitNum();
		int							LiveActiveNPCNum();
		int							LiveNPCNumType_LUA( CX2UnitManager::NPC_UNIT_ID unitID );
		bool						IsKillNPCKeyCode_LUA( int keyCode );
		int							GetKilledKeyCodeNPCCount( int keyCode );
		int							LiveKeyCodeNPCNum( int keyCode );

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-02
		int							LiveKeyCodeNPCNumFreeTeam( int keyCode );
#endif //SERV_NEW_DEFENCE_DUNGEON

		int							GetAliveActiveNPCCount();

#ifdef SERV_INSERT_GLOBAL_SERVER
		int							GetEliteNPCUnitNum(); // oasis907 : 김상윤 [2011.4.19]
		int							GetEliteKeyCodeNPCUnitNum( int keyCode );
#endif SERV_INSERT_GLOBAL_SERVER

#ifdef NEW_CHARACTER_EL
		virtual bool				GetIsExsitancePartyMemberHPLessThanPer( const float fHPRate_, const CX2GUUser* pGUUserWhoCallFunction_ ) const;
#endif // NEW_CHARACTER_EL


		CX2GUNPC*					GetKeyCodeNPC( int keyCode );
		CX2GUNPC*					GetFoucusCameraNPC();
		bool						IsValidUnit( CX2GameUnit* pUnit );

		bool						IsAllUserDead();
		bool 						IsAllUserHPPercentMoreThan( float fHPPercent );
		void 						PartyFeverAllUser( float fAddHyperModeTime );

		void						UpdateNpcNameColor( const int iMyLevel_ );

		bool						RebirthUserUnit( UidType unitUID, int startPosIndex );
#ifdef SERV_CREATE_ELITE_FOR_ADMIN
		CX2GUNPC*
#else SERV_CREATE_ELITE_FOR_ADMIN
		void
#endif SERV_CREATE_ELITE_FOR_ADMIN

#ifdef SERV_INSERT_GLOBAL_SERVER
		CreateNPC( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, int uid, const D3DXVECTOR3& vPos, bool bRight, 
			float fDelayTime, int iKeyCode, 
			CX2Room::TEAM_NUM eTeam = CX2Room::TN_MONSTER, CX2NPCAI::NPC_AI_TYPE eAIType = CX2NPCAI::NAT_NORMAL, UidType iAllyUID = -1, KAttribEnchantNpcInfo* _kAttribEnchantNpcInfo = NULL
	#ifdef	SERV_TRAPPING_RANGER_TEST
			, CX2Room::TEAM_NUM eAllyTeam = CX2Room::TN_NONE
	#endif	SERV_TRAPPING_RANGER_TEST
	#ifdef SUMMON_MONSTER_CARD_SYSTEM
		#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-12
			, CX2GUNPC::NPC_CREATE_TYPE eCreateMonsterType = CX2GUNPC::NCT_NONE		/// 몬스터 생성 타입
		#else // SERV_NEW_DEFENCE_DUNGEON
			, bool bIsMonsterCard = false		//몬스터 카드로 소환되었는지에 대한 여부
		#endif // SERV_NEW_DEFENCE_DUNGEON
	#endif SUMMON_MONSTER_CARD_SYSTEM
			);
#else //SERV_INSERT_GLOBAL_SERVER
		CreateNPC( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, int uid, D3DXVECTOR3 vPos, bool bRight, 
			float fDelayTime, int iKeyCode, 
			CX2Room::TEAM_NUM eTeam = CX2Room::TN_MONSTER, CX2NPCAI::NPC_AI_TYPE eAIType = CX2NPCAI::NAT_NORMAL, UidType iAllyUID = -1
#ifdef	SERV_TRAPPING_RANGER_TEST
			, CX2Room::TEAM_NUM eAllyTeam = CX2Room::TN_NONE
#endif	SERV_TRAPPING_RANGER_TEST
			);
#endif SERV_INSERT_GLOBAL_SERVER
		void SetUserSummonedNPCInfo( CX2NPCAI::NPC_AI_TYPE eAIType, UidType iAllyUID, CX2GUNPC* pNPC, int iHyperMode, CX2UnitManager::NPC_UNIT_ID unitID );

#ifdef CHRISTMAS_EVENT_2013
		void Handler_EGS_NPC_UNIT_CREATE_REQ( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, 
			D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode,
			CX2Room::TEAM_NUM eTeam /*= CX2Room::TN_NONE*/, CX2NPCAI::NPC_AI_TYPE eAIType /*= CX2NPCAI::NAT_NORMAL*/,
			UidType iAllyUID /*= -1*/, bool bElite /* = false */
			, CX2Room::TEAM_NUM eAllyTeam /*= CX2Room::TN_NONE */ 
			, CX2GUNPC::NPC_CREATE_TYPE eCreateMonsterType /*= false */			//몬스터 카드로 소환되었는지에 대한 여부
			);
#endif CHRISTMAS_EVENT_2013

#ifdef SERV_INSERT_GLOBAL_SERVER		
		void						CreateNPCReq( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, D3DXVECTOR3 vPos,
			bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode = -1,
			CX2Room::TEAM_NUM eTeam = CX2Room::TN_MONSTER, CX2NPCAI::NPC_AI_TYPE eAIType = CX2NPCAI::NAT_NORMAL, UidType iAllyUID = -1, bool bElite = false
	#ifdef	SERV_TRAPPING_RANGER_TEST
			, CX2Room::TEAM_NUM eAllyTeam = CX2Room::TN_NONE
	#endif	SERV_TRAPPING_RANGER_TEST
	#ifdef SUMMON_MONSTER_CARD_SYSTEM

		#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-12
			, CX2GUNPC::NPC_CREATE_TYPE eCreateMonsterType = CX2GUNPC::NCT_NONE		/// 몬스터 생성 타입
		#else // SERV_NEW_DEFENCE_DUNGEON
			, bool bIsMonsterCard = false		//몬스터 카드로 소환되었는지에 대한 여부
		#endif // SERV_NEW_DEFENCE_DUNGEON

	#endif SUMMON_MONSTER_CARD_SYSTEM
			);
#else
		void						CreateNPCReq( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, D3DXVECTOR3 vPos,
			bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode = -1,
			CX2Room::TEAM_NUM eTeam = CX2Room::TN_MONSTER, CX2NPCAI::NPC_AI_TYPE eAIType = CX2NPCAI::NAT_NORMAL, UidType iAllyUID = -1
#ifdef	SERV_TRAPPING_RANGER_TEST
			, CX2Room::TEAM_NUM eAllyTeam = CX2Room::TN_NONE
#endif	SERV_TRAPPING_RANGER_TEST
			);
#endif SERV_INSERT_GLOBAL_SERVER

		void						CreateNPCReq_LUA( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop );
#ifdef SERV_INSERT_GLOBAL_SERVER
		void						CreateNPCReqElite( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, bool bElite = false );
#endif SERV_INSERT_GLOBAL_SERVER	
#ifdef SEASON3_MONSTER_2011_7
		void						CreateNPCReqAlly( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, CX2Room::TEAM_NUM eTeam = CX2Room::TN_RED );
#endif SEASON3_MONSTER_2011_7
		
		void						CreateNPCReq_LUA2( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop );
		
		void						PushCreateNPCReq( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, D3DXVECTOR3 vPos,
			bool bRight, float fDelayTime, bool bNoDrop, int iKeycode = 0,
			CX2Room::TEAM_NUM eTeam = CX2Room::TN_MONSTER, CX2NPCAI::NPC_AI_TYPE eAIType = CX2NPCAI::NAT_NORMAL, UidType iAllyUID = -1 );
		void						PushCreateNPCReq_Lua( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode );
		void						FlushCreateNPCReq();

#ifdef CREATE_NPC_REQ_FULL_ARGUMENTS
		void						CreateNPCReq_LUA3( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, int iTeam, int iAIType, UidType iAllyUID );
		void						PushCreateNPCReq_LUA3( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, int iTeam, int iAIType, UidType iAllyUID );
#endif CREATE_NPC_REQ_FULL_ARGUMENTS

		D3DXVECTOR3					GetNPCStartPos( int key )
		{
			if( m_pWorld != NULL && m_pWorld->GetLineMap() != NULL )
			{
				return m_pWorld->GetLineMap()->GetStartPosition( key );
			}
			return D3DXVECTOR3(0,0,0);
		}

		virtual GAME_TYPE			GetGameType()				{ return m_GameType; }
		virtual void				SetGameType( GAME_TYPE gameType ) { m_GameType = gameType; }
		virtual GAME_STATE			GetGameState()				{ return m_GameState; }
		virtual void				SetGameState( GAME_STATE gs ) { m_GameState = gs; }

#ifdef EFFECT_TOOL
		virtual/*FORCEINLINE*/ CKTDGParticleSystem*		GetMajorParticle();			//{ return g_pData->GetGameMajorParticle(); }
		virtual/*FORCEINLINE*/ CKTDGParticleSystem*		GetMinorParticle();			//{ return g_pData->GetGameMinorParticle(); }
		virtual/*FORCEINLINE*/ CKTDGXMeshPlayer*		GetMajorXMeshPlayer();		//{ return g_pData->GetGameMajorXMeshPlayer(); }
		virtual/*FORCEINLINE*/ CKTDGXMeshPlayer*		GetMinorXMeshPlayer();		//{ return g_pData->GetGameMinorXMeshPlayer(); }
#else
		/*FORCEINLINE*/ CKTDGParticleSystem*		GetMajorParticle();			//{ return g_pData->GetGameMajorParticle(); }
		/*FORCEINLINE*/ CKTDGParticleSystem*		GetMinorParticle();			//{ return g_pData->GetGameMinorParticle(); }
		/*FORCEINLINE*/ CKTDGXMeshPlayer*		GetMajorXMeshPlayer();		//{ return g_pData->GetGameMajorXMeshPlayer(); }
		/*FORCEINLINE*/ CKTDGXMeshPlayer*		GetMinorXMeshPlayer();		//{ return g_pData->GetGameMinorXMeshPlayer(); }
#endif // EFFECT_TOOL
		/*FORCEINLINE*/ CX2EffectSet*				GetEffectSet();				//{ return g_pData->GetGameEffectSet(); }


		CKTDXDeviceRenderTargetTexture* GetLastKillShot()		{ return m_pRenderTargetLastkillShot; }

		bool 						GetEnableAllKeyProcess() const { return m_bEnableAllKeyProcess; }
		void 						SetEnableAllKeyProcess(bool val) { m_bEnableAllKeyProcess = val; }
		void						SetEnableCommandKeyProcess( bool bEnable ) { m_bEnableCommandKeyProcess = bEnable; }

		bool						CheckEnableKeyProcess();
				

		virtual int					GetWinTeam()	{ return -1; }
		void						SetLastKillCheck( bool bLastKillCheck ) { m_bLastKillCheck = bLastKillCheck; }
		bool						GetLastKillCheck() { return m_bLastKillCheck; }

		int							GetLoadingPercent() { return m_LoadingPercent; }
		bool						GetBGMOn() { return m_bBGMOn; }		

		void						SetWorldColor_LUA( D3DXCOLOR color )
		{ 
			if( NULL != GetWorld() )
			{
				GetWorld()->SetWorldColor( color ); 
			}
		}		
		
		void BroadCast_XPT_UNIT_NPC_SYNC_PACK();




		CKTDGFontManager::CUKFont*	GetFontForUnitName() { return m_pFontForUnitName; }

		static void					SetRenderNPCName(bool bRenderNPCName) { m_sbRenderNPCName = bRenderNPCName; }
		static bool					GetRenderNPCName(){ return m_sbRenderNPCName; }

		KGAME_STATISTICS&           AccessGameStatistics()          { return    m_kGameStatistics; }

		virtual bool				Handler_EGS_JOIN_ROOM_NOT( KEGS_JOIN_ROOM_NOT& kEGS_JOIN_ROOM_NOT );

		virtual bool				Handler_EGS_LEAVE_ROOM_REQ( int leaveRoomReason = NetError::NOT_LEAVE_ROOM_REASON_00 );
		virtual bool				Handler_EGS_LEAVE_ROOM_ACK( KEGS_LEAVE_ROOM_ACK& kEGS_LEAVE_ROOM_ACK );
		virtual bool				Handler_EGS_LEAVE_ROOM_NOT( KEGS_LEAVE_ROOM_NOT& kEGS_LEAVE_ROOM_NOT );

		virtual bool				Handler_EGS_LEAVE_GAME_REQ();
		virtual bool				Handler_EGS_LEAVE_GAME_ACK( KEGS_LEAVE_GAME_ACK& kEGS_LEAVE_GAME_ACK );
		virtual bool				Handler_EGS_LEAVE_GAME_NOT( KEGS_LEAVE_GAME_NOT& kEGS_LEAVE_GAME_NOT );

		virtual bool				Handler_EGS_STATE_CHANGE_RESULT_REQ();
		virtual bool				Handler_EGS_STATE_CHANGE_RESULT_ACK( KEGS_STATE_CHANGE_RESULT_ACK& kEGS_STATE_CHANGE_RESULT_ACK );
		virtual bool				Handler_EGS_STATE_CHANGE_RESULT_NOT( KEGS_STATE_CHANGE_RESULT_NOT& kEGS_STATE_CHANGE_RESULT_NOT );


		virtual bool				Handler_EGS_PLAY_START_NOT( KEGS_PLAY_START_NOT& kEGS_PLAY_START_NOT );

		virtual bool				Handler_EGS_STATE_CHANGE_GAME_INTRUDE_NOT( KEGS_STATE_CHANGE_GAME_INTRUDE_NOT& kEGS_STATE_CHANGE_GAME_INTRUDE_NOT );
		virtual bool				Handler_EGS_INTRUDE_START_REQ();
		virtual bool				Handler_EGS_INTRUDE_START_ACK( KEGS_INTRUDE_START_ACK& kEGS_INTRUDE_START_ACK );
		virtual bool				Handler_EGS_INTRUDE_START_NOT( KEGS_INTRUDE_START_NOT& kEGS_INTRUDE_START_NOT );

		virtual void				Handler_EGS_CURRENT_TEAM_KILL_SCORE_NOT( KEGS_CURRENT_TEAM_KILL_SCORE_NOT& kEGS_CURRENT_TEAM_KILL_SCORE_NOT );

		virtual bool				Handler_EGS_END_GAME_NOT( KEGS_END_GAME_NOT& kKEGS_END_GAME_NOT );


		virtual bool				Handler_EGS_DROP_ITEM_NOT( KEGS_DROP_ITEM_NOT& kEGS_DROP_ITEM_NOT );

		virtual bool				Handler_EGS_GET_ITEM_REQ( UidType itemUID );
		virtual bool				Handler_EGS_GET_ITEM_ACK( KEGS_GET_ITEM_ACK& kEGS_GET_ITEM_ACK );
		virtual bool				Handler_EGS_GET_ITEM_NOT( KEGS_GET_ITEM_NOT& kEGS_GET_ITEM_NOT );

		virtual bool 				Handler_EGS_NPC_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool 				Handler_EGS_NPC_UNIT_CREATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef FINALITY_SKILL_SYSTEM //JHKang
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		bool						Handler_EGS_USE_FINALITY_SKILL_REQ();
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
		bool						Handler_EGS_USE_FINALITY_SKILL_REQ( UidType itemUID_ );
		virtual bool				Handler_EGS_USE_FINALITY_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_INSERT_GLOBAL_SERVER
		virtual bool 				Handler_EGS_CREATE_ATTRIB_NPC_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_INSERT_GLOBAL_SERVER

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		virtual bool				Handler_EGS_NPC_FIELD_MIDDLE_BOSS_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// 중간 보스 생성 ACK
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
		virtual bool				Handler_EGS_NPC_UNIT_DIE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool				Handler_EGS_NPC_UNIT_DIE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool						NPCUnitDieReq( const KEGS_NPC_UNIT_DIE_REQ& kPacket_ );
//{{ 09.05.12 태완 : 마나이터 드롭 관련으로 작업
#ifdef BONUS_DROP
		bool						NPCUnitDieReq( char cAttUnitType, int iNPCUID, UidType uiAttUnit, UidType uiMDAttUnit, D3DXVECTOR3 diePos, bool bIsAllyNPC, bool bBounsDrop = false );
		bool						NPCUnitDieReq( char cAttUnitType, int iNPCUID, UidType uiAttUnit, UidType uiMDAttUnit, D3DXVECTOR3 diePos, KEGS_NPC_UNIT_DIE_REQ::NPC_DIE_STATE eDieState );
#else
		bool						NPCUnitDieReq( char cAttUnitType, int iNPCUID, UidType uiAttUnit, UidType uiMDAttUnit, D3DXVECTOR3 diePos, bool bIsAllyNPC );
#endif

		virtual bool				Handler_EGS_REMAINING_PLAY_TIME_NOT( KEGS_REMAINING_PLAY_TIME_NOT& kEGS_REMAINING_PLAY_TIME_NOT );

		virtual bool				Handler_EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ();
		virtual bool				Handler_EGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT();

		virtual bool				Handler_EGS_ADD_ON_STAT_ACK( KEGS_ADD_ON_STAT_ACK& kEGS_ADD_ON_STAT_ACK );
		virtual bool				Handler_EGS_ADD_ON_STAT_NOT( KEGS_ADD_ON_STAT_NOT& kEGS_ADD_ON_STAT_NOT );



		virtual void				UserUnitDieNot(	KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT );
		virtual bool				Handler_EGS_USER_UNIT_DIE_COMPLETE_NOT( KEGS_USER_UNIT_DIE_COMPLETE_NOT& kEGS_USER_UNIT_DIE_COMPLETE_NOT );

		//bool						Handler_EGS_ITEM_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		CX2TalkBoxManager*			GetTalkBoxManager() { return m_pTalkBoxMgr; }

		float						GetElapsedTimeAfterLastKeyboardInput() { return m_fElapsedTimeAfterLastKeyboardInput; }
		bool						IsKeyPressed() { return m_InputData.IsKeyPressed(); }
		bool						IsSkipKeyPressed(){ return m_bSkipKeyPressed; }
		void						SetSkipKeyPress(bool bVal){m_bSkipKeyPressed=bVal;}
		CX2GUUser::InputData*		GetInputData() { return &m_InputData; }

		virtual bool				GameChatInfoNot( KEGS_CHAT_NOT& kKEGS_CHAT_NOT ) { return false; }

		virtual bool				Handler_EGS_UPDATE_UNIT_INFO_NOT( KEGS_UPDATE_UNIT_INFO_NOT& kEGS_UPDATE_UNIT_INFO_NOT );
		//virtual bool				Handler_EGS_NOTIFY_MSG_NOT( KEGS_NOTIFY_MSG_NOT& kEGS_NOTIFY_MSG_NOT );

		void						ProcessExpListByNpcUnitDie( const KEGS_NPC_UNIT_DIE_NOT& kEvent );
#ifdef ARA_CHARACTER_BASE
		void						ProcessIKillThisNpc( const KEGS_NPC_UNIT_DIE_NOT& kEvent );
#endif

#ifdef FIX_ARA_AWAKE_ABILITY_BUG
		void						ProcessIKillThisUser( const KEGS_USER_UNIT_DIE_NOT& kEvent );
#endif // FIX_ARA_AWAKE_ABILITY_BUG


		bool						CheckNewUser( UidType unitUID );

		float						GetGameTime(){ return m_fGameTime; }

		void						SetShowF1Help( bool bShow ){ m_bShowF1Help = bShow; }
		bool						GetShowF1Help(){ return m_bShowF1Help; }
		void						RenderF1Help();
		void						InitF1HelpString();

		
		bool						GetEnableNPCTalkBox() { return m_bEnableNPCTalkBox; }

#ifdef CHECK_CLEAR_SCORE
		int							GetGameScore() { return m_GameScore; }
		int							GetGameScoreBack() { return m_GameScoreBack; }
#endif
#ifdef DIALOG_SHOW_TOGGLE
		void						SetGameScore();
#endif
		void						SetGameScore( int gameScore );
		void						AddGameScore( int gameScore );
		void						AddGameScore( int gameScore, float fTime );

		void						SetTechPoint( int techPoint );	/// 사용 안함
		void						AddTechPoint( int techPoint );
		
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM	
		int							GetSubStageTechPoint () const;			//	해당 SubStage 의 테크닉 포인트를 얻어온다.
		void						ResetSubStageTechPoint ();				//	
#endif	// SERV_DUNGEON_FORCED_EXIT_SYSTEM


		bool						GetOpenChatBox() { return m_bOpenChatBox; }
		void						SetOpenChatBox( bool bCheck ) { m_bOpenChatBox = bCheck; }

		void						PauseNPCAI( float fTime );
		void						KillAllNPC_LUA( bool bActiveOnly = true );
		void						KillAllNPC( bool bActiveOnly = true, CX2GameUnit* pAttackerGameUnit_ = NULL, bool bExceptGate = false );
		void						KillNPC( const CX2UnitManager::NPC_UNIT_ID eNPCID, int nCount );
		void						EnableAllNPCAI( bool bEnable );
		void						EnableAllNPCEventProcess( bool bEnable );
		void						SetEnableAllNPCConditionTable( bool bEnable);
		void						SetAllNPCMPFull();


		void						SetFreeCamera( bool bFreeCamera );
		bool						GetFreeCamera() { return m_bFreeCamera; }
#ifdef KEYFRAME_CAMERA
		bool						GetWorldCameraEdit(){ return m_bWorldCameraEdit; }
		void						SetWorldCameraEdit( bool bEnable);
#endif KEYFRAME_CAMERA
		// oasis907 : 김상윤 [2010.12.13] // 본섭 파싱 위해 디파인 싸지 않음
		void						PlayWorldCamera( int iWorldCameraID );

		int							GetTeamVPMaxAdvRed(){ return m_TeamVPMaxAdvRed; }
		int							GetTeamVPMaxAdvBlue(){ return m_TeamVPMaxAdvBlue; }

		int							GetRedTeamKill(){ return m_RedTeamKill; }
		int							GetBlueTeamKill(){ return m_BlueTeamKill; }

		void						SetAllowFrameCount( int allowFrameCount )
		{ 
			if( m_AllowFrameCount >= 0 )
				m_AllowFrameCount += allowFrameCount;
			else
				m_AllowFrameCount = allowFrameCount; 
		}

		bool						GoStage( int iStageIndex, int iSubStageIndex = 0 );

#ifdef SECRET_VELDER
		int		GetNowStageIndex();
#endif

		int							GetGetEXPInGame() const { return m_GetEXPInGame; }

		//{{ kimhc // 2010-12-17 // 가장 가까운 상대를 락온 할 수 있도록 하기 위해 락온 타입 추가
		// Nearst로 찾는 것은 NPC의 Type에 대한 예외처리를 추가해야 하기 때문에 이것을 이용하기로 함
//{{ kimhc // 2010.12.17 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		UidType						GetLockOnNearstTarget( CX2Room::TEAM_NUM eTeamNum_, IN const D3DXVECTOR3& vMyPos_, OUT bool& bUserUnit_ );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.17 //  2010-12-23 New Character CHUNG
#ifdef FIX_PET_ATTACK
		UidType						GetLockOnNearstTargetOnlyBasic( CX2Room::TEAM_NUM eTeamNum_, IN const D3DXVECTOR3& vMyPos_, OUT bool& bUserUnit_ );		
#endif
		//}} kimhc // 2010-12-17 // 가장 가까운 상대를 락온 할 수 있도록 하기 위해 락온 타입 추가
		UidType						GetLockOnTarget( CX2Room::TEAM_NUM teamNum, int seedNum, bool& bUserUnit, int iModulusFactor = 30  );

		//{{ JHKang / 강정훈 / 2011.7.103 / 지정된 범위(min - max) 추가 락온
#ifdef CHUNG_SECOND_CLASS_CHANGE
#ifdef	NEW_CHARACTER_CHUNG
		UidType						GetLockOnNearstTargetInRange( CX2Room::TEAM_NUM eTeamNum_, IN const D3DXVECTOR3& vMyPos_, IN D3DXVECTOR2& vRange_, OUT bool& bUserUnit_ 
	#ifdef BALANCE_DEADLY_CHASER_20130214
																, bool bIsNotTargetPreLockOnTarget = false
	#endif //BALANCE_DEADLY_CHASER_20130214
																);
#endif	NEW_CHARACTER_CHUNG
#endif
		//}}

#ifdef GET_DUNGEON_ID_IN_LUA
		virtual int					GetDungeonID() { return 0; }
#endif GET_DUNGEON_ID_IN_LUA
		virtual int					GetDungeonType() { return 0; }
		virtual int					GetGameLevel() { return 0; }
		CX2SlideShot*				GetSlideShot();

		void						SetShowAllUnitGageBar( bool bShow );
		bool						GetShowAllUnitGageBar() { return m_bShowAllUnitGageBar; }

		void						SetGameEnd(bool val) { m_bGameEnd = val; }
		bool						GetGameEnd() { return m_bGameEnd; }

		CX2DropItemManager*			GetDropItemManager() const { return m_pDropItemManager; }
		bool						AnyUserUnitInRange( D3DXVECTOR3 pos, float fRange );
		bool						IsThereAnyUserUnitOnLine( int iLineIndex );
		bool						IsThereAnyUserUnitUsingSkill( CX2SkillTree::SKILL_ID eSkillID );


		void						CreateStageLoadingTeleportEffectForAllUser( bool bEnterPortal );

		//{{oasis:김상윤////2009-10-7////
		D3DXVECTOR3					GetFixedDistancePosition_LUA( D3DXVECTOR3 pvPos, float fDist, bool bIsRight );
		float						GetDist_LUA( const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2 );
		bool						IsNearNPCUnitUID_LUA( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange = 9999 );
		CX2GUNPC*					GetNearNoneAINPCUnitUID( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange = 9999 );
		CX2GUUser*					GetNearestUserUnit( const D3DXVECTOR3& vPos_ );
		UidType						GetNearestUserUnitUID_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange = 9999 );
		D3DXVECTOR3					GetNearestUserUnitPos_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange = 9999 );
		//}}oasis:김상윤////2009-10-7////
		D3DXVECTOR3					GetNearestNPCUnitPos_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange = 9999 );
#ifdef SEASON3_MONSTER_2011_7
		UidType						GetFarthestUserUnitUID_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange = 9999 );
#endif SEASON3_MONSTER_2011_7
#ifdef VELDER_BOSS_5
		//{{ oasis907 : 김상윤 [2010.7.30] // 
		D3DXVECTOR3					GetFarthestUserUnitPos_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange = 9999 );
		//}}
#endif VELDER_BOSS_5
		int							GetNearNPCUnitUIDInStart_LUA( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange = 9999 );
		CX2GUNPC*					GetNearNPCUnitInStart( const D3DXVECTOR3 vPos_, const CX2UnitManager::NPC_UNIT_ID eUnitID_, const float fMaxRange = 9999 );


        int							GetNearNPCUnitUID_LUA( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange = 9999 );
		CX2GUNPC*					GetNearestNpcInSpecificRangeByNpcId( const D3DXVECTOR3& vPos_, const CX2UnitManager::NPC_UNIT_ID eUnitID_, const float fMaxRange_ = 9999 );

		int							GetNearNPCUnitUIDEx( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, int npcUIDToExcept, float fMaxRange = 9999 );
		int							GetNearNPCUnitUIDEx( D3DXVECTOR3 pos, const vector< CX2UnitManager::NPC_UNIT_ID >& vecUnitID, int npcUIDToExcept, float fMaxRange = 9999 );
		int							GetNearestNPCUnitUID( D3DXVECTOR3 pos, int npcUIDToExcept, float fMaxRange = 9999 );
		CX2GUUser* GetNearestUserUnitInSpecificRange( const D3DXVECTOR3& vPos_, const float fMinRange_, const float fMaxRange_ = 9999 );
		
		CX2GameUnit*				GetFrontNearestEnemyUnit( CX2GameUnit* pMyUnit, float& fDistance );

#ifdef NEAREST_NPC_ON_SAME_DIRECTION
		D3DXVECTOR3					GetNearestNPCUnitPosOnSameDirection_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange, D3DXVECTOR3 vDir );
		CX2GameUnit*				GetNearestNPCUnitOnSameDirection_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange, D3DXVECTOR3 vDir );
#endif NEAREST_NPC_ON_SAME_DIRECTION

		bool						ChangeRebirthStateNPCUnit_LUA( int NPCUID );


		int							GetNPCCountAt( D3DXVECTOR3 vPosition, float fRadius = 30.f, CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );
		D3DXVECTOR3					GetEmptyStartPos();
#ifdef SKILLSTRING_IN_SKILLTEMPLET
		void						SetStateString( const wstring pStr ){ m_StateString = pStr; }
#else //SKILLSTRING_IN_SKILLTEMPLET
		void						SetStateString( const WCHAR* pStr ){ m_StateString = pStr; }
#endif //#ifdef SKILLSTRING_IN_SKILLTEMPLET

		CX2Game::InfoTextManager&	GetInfoTextManager() { return m_InfoTextManager; }
		CX2GUNPCFunc*				GetNPCFunc(){ return m_pNPCFunc; }


		void SetRenderUserName(bool val) { m_bRenderUserName = val; } // tutorial에서 캐릭터 이름 안 그리게

		void						UseItemSpecialAbility( const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser );
		void						ApplyBuffFactorToGUUser( const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser );

#ifdef PET_ITEM_GM
		void						UseItemPetAbility( const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser );
#endif //PET_ITEM_GM

		//{{ kimhc // 실시간 엘소드 중 실시간 장비 및 아바타 교체
#ifdef REAL_TIME_ELSWORD
		bool						RemoveSpecialAbilityInEquip( const CX2Item::ItemTemplet* const pItemTemplet, CX2GUUser* const pCXGUUser );
		bool						SetSpecialAbilityInEquip( const CX2Item::ItemTemplet* const pItemTemplet, CX2GUUser* const pCXGUUser );
#endif REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 실시간 장비 및 아바타 교체

		//{{ kimhc // 게임 로딩이 끝날때 까지 ESC 처리 안되게 하는 작업 // 2009-06-08
		void						SetCanUseEscFlag( bool flag ) { m_bCanUseESC = flag; }
		bool						GetCanUseEscFlag() const { return m_bCanUseESC; }
		//}} kimhc // 게임 로딩이 끝날때 까지 ESC 처리 안되게 하는 작업 // 2009-06-08

		void						UpdateSkillSlotUI(bool bUpdateEquipSkill = false);

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		void						InvalidEventThread( HANDLE hValue );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


		void						SetLastkillShotTime(float val) { m_fLastkillShotTime = val; }

		void						BuffAllNPC( int statType, float fStatVal, float fStatTime );


#ifdef RESET_INVALID_UNIT_POSITION_TEST
		bool CheckMisplacedUnit();
		void ReplaceMisplacedUnit();
#endif RESET_INVALID_UNIT_POSITION_TEST



#ifdef BUBBLE_BOBBLE_TEST
		bool GetFixedCamera() const { return m_bFixedCamera; }
		void SetFixedCamera(bool val) { m_bFixedCamera = val; }
#endif BUBBLE_BOBBLE_TEST


		bool GetEnableMonsterFly() { return m_bEnableMonsterFlyTest; }


#ifdef IN_GAME_MANUAL_CAMERA_POSITION_TEST
		void EnableManualCameraPosition( bool bEnabled );
#endif IN_GAME_MANUAL_CAMERA_POSITION_TEST




#ifdef REVERSE_GRAVITY_TEST

		void ReverseGravityFrameMove( double fTime, float fElapsedTime );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        bool IsInReverseGravityRegion( float fElapsedTime, const D3DXVECTOR3& vPos, float& fSpeedYDelta, D3DXVECTOR3& vSpeedRotateDelta, int iTeamNumber );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool IsInReverseGravityRegion( const D3DXVECTOR3& vPos, float& fSpeedYDelta, D3DXVECTOR3& vSpeedRotateDelta, int iTeamNumber );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void AddReverseGravityRegion( const D3DXVECTOR3& vPos, const float fRange, const float fTime, const float fGravity, int iTeamNumber );

#endif REVERSE_GRAVITY_TEST

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		std::vector<UidType>& GetVecUserUIDforSyncPacket() { return m_vecUserUIDforSyncPacket; }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		void						ResetReBirthStoneNumUI(); // protected에서 옮겨옴

#ifdef DUNGEON_ITEM

#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER		/// 블리언 값 반환으로 변경
		bool
#else	ADD_VELDER_ACADEMY_EVENT_MONSTER
		void
#endif	ADD_VELDER_ACADEMY_EVENT_MONSTER		
		ProcSpecialItem( 
			const CX2Item::ItemTemplet* pItemTemplet, 
			CX2GUUser* pCX2GUUser);
		CX2Item::SPECIAL_ITEM_TYPE GetSpecialItemGroupType( const CX2Item::ItemTemplet* pItemTemplet);
		wstring SummonSpiritEffectName(int iSpirit);
		void ClearSpirit();
		void SummonSpiritEffect(int iEffectIndex);
#ifdef SPECIAL_USE_ITEM
		void SpecialItemEffect(int iEffectIndex);
#endif
		void SummonMonster(CX2GUUser* pCX2GUUser, CX2UnitManager::NPC_UNIT_ID npcId, D3DXVECTOR3 vPos);
		
		void SideEffectGenius(CX2GUUser* pCX2GUUser);
#endif

#ifdef DAMAGE_HISTORY
		void SetSendedLeaveRoom(bool bVal) { m_bSendLeaveRoom = bVal; }
		bool GetSendedLeaveRoom() { return m_bSendLeaveRoom; }
#endif

#ifdef SERV_CREATE_ELITE_FOR_ADMIN
		bool Send_EGS_ADMIN_NPC_UNIT_CREATE_REQ_LUA( int unitID, int level, D3DXVECTOR3 vPos, bool bRight, int iAttrib1, int iAttrib2, int iSA1, int iSA2 );
		virtual bool Handler_EGS_ADMIN_NPC_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_ADMIN_NPC_UNIT_CREATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

#ifdef KEYFRAME_CAMERA
		CFirstPersonCamera* GetFPSCamera(){	return &m_FPSCamera; }
#endif KEYFRAME_CAMERA

#ifdef SERV_WORLD_TRIGGER_RELOCATION
		bool Handler_EGS_WORLD_TRIGGER_RELOCATION_REQ( int iTriggerId );
		bool Handler_EGS_WORLD_TRIGGER_RELOCATION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_WORLD_TRIGGER_RELOCATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

//{{ 허상형 : [2011/3/4/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
		//{{ 허상형 : [2011/3/2/] //	랜덤값 추가
		int GetRandomValue() { return rand(); }
		float GetDungeonProgress();

		int	GetNPCUnitNumByNPCID( int iNPCID );
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 허상형 : [2011/3/4/] //	월드 미션

#ifdef ADDITIONAL_MEMO
		CX2GameUnit *GetNearestUnit( CX2GameUnit* pGameUnit, int iFlag );
#endif

#ifdef SERV_SHARING_BANK_TEST
		wstring GetUserName(int inx);	// 은행 공유로 인한 추가
#endif SERV_SHARING_BANK_TEST

		//{{ kimhc // 2011-07-28 // 옵션수치화
		// 1레벨 보정 던전인가?
		bool IsDamageFreeGame() const { return m_bIsDamageFreeGame; }
		void SetIsDamageFreeGame( const bool bIsDamageFreeGame_ ) { m_bIsDamageFreeGame = bIsDamageFreeGame_; }	
		//}} kimhc // 2011-07-28 // 옵션수치화
#ifdef NEW_HENIR_TEST
		int							GetStartSecretStageEnteringEvent();
		bool						SecretDungeonStageLoadReq(int iVal);

#endif NEW_HENIR_TEST
#ifdef ELSWORD_WAY_OF_SWORD
		void						UpdateElswordSkillSlotEffect();
#endif ELSWORD_WAY_OF_SWORD
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		bool IsFront( CX2GameUnit *pMyUnit, CX2GameUnit *pTargetUnit );
#endif
		void						UpdateEveElectraSkillSlotIcon();
	#ifdef SERV_RENA_NIGHT_WATCHER
		void						UpdateNightWatcherSkillSlotIcon();
	#endif //SERV_RENA_NIGHT_WATCHER
	#ifdef SERV_RAVEN_VETERAN_COMMANDER
		void						UpdateVeteranCommanderSkillSlotIcon();
	#endif //SERV_RAVEN_VETERAN_COMMANDER
	#ifdef FINALITY_SKILL_SYSTEM //김창한
		void						UpdateCodeEmpressSkillSlotIcon();
	#endif //FINALITY_SKILL_SYSTEM
	#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		void						UpdateAddNasodRulerSkillSlotIcon();
	#endif //SERV_9TH_NEW_CHARACTER			
		void ShowDangerAlarm_LUA( int iAlarmId, float fShowTime, ALARM_COLOR_TYPE eColor, bool bDanger, int iStringIndex );
		void ShowDangerAlarm( int iIndex );
		int LoadDangerAlarm( KLuaManager &luaManager, UidType iUid, int iStateId = -1 );
		int GetDangerAlarm( UidType iUid, int iStateId );
		void ClearDangerAlarm()
		{
			for(UINT i=0; i<m_vecDangerAlarm.size(); ++i)
			{
				SAFE_DELETE( m_vecDangerAlarm[ i ] );
				m_vecDangerAlarm.erase( m_vecDangerAlarm.begin() + i);
				--i;
			}
			m_vecDangerAlarm.clear();

			CKTDGParticleSystem::CParticleEventSequence* pSeq = GetMajorParticle()->GetInstanceSequence( m_hDangerAlarm );
			if( pSeq != NULL )
			{
				pSeq->SetShowObject(false);
			}
		}
		bool GetShowAlarm(int iIndex)
		{
			if( iIndex < 0 )
				return false;
			if( m_vecDangerAlarm[iIndex] == m_pShowAlarm )
				return true;
			return false;
		}
		bool GetAlarmRepeat(int iIndex)
		{
			if( iIndex >= 0 )
				return m_vecDangerAlarm[iIndex]->m_bRepeat;

			return false;
		}
		float GetAlarmDelayTime(int iIndex)
		{
			if( iIndex >= 0 )
				return m_vecDangerAlarm[iIndex]->m_fDelayTime;

			return 0.f;
		}

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
void ShowTimerAlarm_LUA( int iAlarmId, float fShowTime, ALARM_COLOR_TYPE eColor, bool bDanger, float fValue_ );
void ShowTimerAlarm( int iIndex, float fValue_ );
#endif //SERV_HALLOWEEN_EVENT_2013

#ifdef ADD_GAME_STAGE_DELETE_DAMAGEEFFECT
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        void AddRemoveDamageEffect( CX2DamageEffect::CEffectHandle hDamage )
		{
			m_vecRemoveDamageEffect.push_back( hDamage );
		}
		void RemoveDamageEffect()
		{
			for(UINT i=0; i<m_vecRemoveDamageEffect.size(); ++i)
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                if ( CX2DamageEffect::CEffect* pDamage = ( GetDamageEffect() != NULL ) 
                    ? GetDamageEffect()->GetInstance( m_vecRemoveDamageEffect[i] ) : NULL )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                if ( CX2DamageEffect::CEffect* pDamage = ( GetDamageEffect() != NULL && GetDamageEffect()->IsLiveInstance( m_vecRemoveDamageEffect[i] ) == true )
                    ? m_vecRemoveDamageEffect[i] : NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				{
					for ( UINT j = 0; j < pDamage->GetDamageEffectDataInLua().m_vecCreateDamageEffect.size(); ++j )
					{
						CX2DamageEffect::DamageEffectData* pData = pDamage->GetDamageEffectDataInLua().m_vecCreateDamageEffect[j];
						if ( NULL != pData && true == pData->bAutoDie )
							GetDamageEffect()->DestroyInstanceHandleSilently(m_vecRemoveDamageEffect[i]); //DestroyInstance( pDamage );
					}
				}
			}
			m_vecRemoveDamageEffect.clear();
		}
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        void AddRemoveDamageEffect( CX2DamageEffect::CEffect *pDamage )
		{
			m_vecRemoveDamageEffect.push_back( pDamage );
		}
		void RemoveDamageEffect()
		{
			for(UINT i=0; i<m_vecRemoveDamageEffect.size(); ++i)
			{
				CX2DamageEffect::CEffect *pDamage = m_vecRemoveDamageEffect[i];
				if( pDamage != NULL && GetDamageEffect() != NULL && GetDamageEffect()->IsLiveInstance( pDamage ) == true )
				{
					for ( UINT j = 0; j < pDamage->GetDamageEffectDataInLua().m_vecCreateDamageEffect.size(); ++j )
					{
						CX2DamageEffect::DamageEffectData* pData = pDamage->GetDamageEffectDataInLua().m_vecCreateDamageEffect[j];

						if ( NULL != pDamage && NULL != pData && true == pData->bAutoDie )
							GetDamageEffect()->DestroyInstanceSilently(pDamage); //DestroyInstance( pDamage );
					}
				}
			}
			m_vecRemoveDamageEffect.clear();
		}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif  ADD_GAME_STAGE_DELETE_DAMAGEEFFECT


#ifdef NEXON_QA_CHEAT_REQ
		void AddHyperModeCount();		//각성 구슬을 하나씩 채워줌
		void AddAraGage();				//아라 기력 게이지를 하나씩 채워줌
		void AddChungCannonBall();		//청 포탄 게이지를 채워줌
		void AddChungExtraCannonBall();	//청 특수 포탄 게이지를 채워줌
#endif //NEXON_QA_CHEAT_REQ


#ifdef SERV_RENA_NIGHT_WATCHER
		const vector<CX2GameUnit*>& GetUnitList() const { return m_UnitList; }							//Unitlist 반환
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		void SetEventMonsterOut( UidType UnitUIDOut ){ m_UnitUIDOut = UnitUIDOut; }
		UidType GetEventMonsterOut() { return m_UnitUIDOut; }
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SUMMON_MONSTER_CARD_SYSTEM
		int  GetSummonMonsterCardNPCLevel( CX2GUNPC* pNPC, int iLevel );							//각 던전에 따른 소환 몬스터의 레벨 반환 함수
		void CreateSummonMonsterCardNPC( const CX2Item::SpecialAbility* pSa, CX2GUUser* pCX2GUUser );		//아이템에서 받아온 값을 이용해 몬스터 소환 동작
#endif SUMMON_MONSTER_CARD_SYSTEM


		void						SendNpcUnitFirstSyncPacketImmediateForce( vector<UidType>& vecNonNpcSyncUserList_, OUT KEGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT& kPacket_ );

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		void						SetPetFirstSyncPacketImmediateForce( vector<UidType>& vecNonNpcSyncUserList_ );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef	BATTLE_FIELD_TEST
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
		CX2GUUser* GetHostGameUnit() const { return ( null != m_optrHostGameUnit ? m_optrHostGameUnit.GetObservable() : NULL ); }
		void SetHostGameUnit( CX2GUUser* pHostGameUnit_ )
		{
			if ( NULL != pHostGameUnit_ )
				m_optrHostGameUnit = pHostGameUnit_; 
		}
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		CX2GameUnit* GetHostGameUnit() const { return ( null != m_optrHostGameUnit ? m_optrHostGameUnit.GetObservable() : NULL ); }
		void SetHostGameUnit( CX2GameUnit* pHostGameUnit_ )
		{
			if ( NULL != pHostGameUnit_ )
				m_optrHostGameUnit = pHostGameUnit_; 
		}
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
		void ResetHostGameUnit() { m_optrHostGameUnit.Reset(); }
//#endif	BATTLE_FIELD_TEST

		virtual void			ResurrectOtherUser() {}
		void					ResurrectOtherUserOK();
		void					ResurrectOtherUserCancel();

		bool CheckAndWarningBusyStateNow( const bool bCheckDead_ = true );		// 필드에서의 바쁜상태 체크 및 경고
		bool					Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_REQ( bool bStop );
		bool					Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_ACK( KEGS_STOP_DUNGEON_CONTINUE_TIME_ACK& kKEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK );
		virtual bool			Handler_EGS_SKIP_CONTINUE_DUNGEON_NOT();

		virtual bool			Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( UidType unitUID, bool bAllowUseCash = false );
		virtual	bool			Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK( KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK& kEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK );
		virtual	bool			Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT( KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT& kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT );

		void					SetShowSmallGageAndNameForAllGameUnit( const bool bShow_ );

#ifdef MODIFY_DUNGEON_STAGING
		CKTDGUIDialogType		GetDLGMyScore(){return m_pDLGMyScore;}
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		void					SetCinematicUI(bool bVal, float fTime_ = -1.0f ) const
		{ 	
			if( NULL != m_pCinematicUI) 
			{
				if ( fTime_ > 0 )
					m_pCinematicUI->SetCinematicUI(bVal, fTime_);
				else
					m_pCinematicUI->SetCinematicUI(bVal);
			}
		}
#else // SERV_BATTLEFIELD_MIDDLE_BOSS
		void					SetCinematicUI(bool bVal){ if( NULL != m_pCinematicUI) m_pCinematicUI->SetCinematicUI(bVal);}
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
		void					CanNotInputAndDisalbeNPCAI(bool bVal);
#endif //MODIFY_DUNGEON_STAGING

		void					InitExpandSkillCoolTime();
#ifdef X2TOOL
	bool DungeonStageLoad( int stageNum );
	int	 GetNpcUID4Tool()
	{
		static int iUid = 0;
		return ++iUid;
	}
#endif

#ifdef UDP_CAN_NOT_SEND_USER_KICK
		void						EraseAbuserUserCheckByUID( UidType iUnitUID_ );
#endif //UDP_CAN_NOT_SEND_USER_KICK


#ifdef GUIDE_QUEST_MONSTER
		void					SetQuestMonster( CX2UnitManager::NPC_UNIT_ID eNPCUnitID_ , bool bShow = true);
#endif //GUIDE_QUEST_MONSTER

#ifdef FIX_OBSERVER_MODE
		/// 옵저버 모드일 때 PVP 유저 UI 표시 순서 인젝스 제어용
		UINT GetPositionIndexRed() const { return m_uiPositionIndexRed; }
		void SetPositionIndexRed( const UINT uiPosIndex_ ) { m_uiPositionIndexRed	= uiPosIndex_; }

		UINT GetPositionIndexBlue() const { return m_uiPositionIndexBlue; }
		void SetPositionIndexBlue( const UINT uiPosIndex_ ) { m_uiPositionIndexBlue	= uiPosIndex_; }

		CX2Game::CinematicUI* CX2Game::GetCinematicUI( );

		FORCEINLINE void InitPositionIndexPVPUserGageForObserver() 
		{
			m_uiPositionIndexRed  = 0;
			m_uiPositionIndexBlue = 0;
		};
#endif FIX_OBSERVER_MODE

#ifdef BALANCE_GRAND_ARCHER_20121213
		bool GetNearUnitUidList( IN CX2Room::TEAM_NUM eMyTeamNum_, IN const D3DXVECTOR3& vMyPos_, OUT std::map<float, UidType>& mapNearUnitUid_, bool bIgnoreDistanceLimit = false);
#endif //BALANCE_GRAND_ARCHER_20121213

		void SetNpcHardLevel( CX2GUNPC* pNpcUnit_, const int iLevel_ );


#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		virtual bool				Send_EGS_DEFENCE_DUNGEON_WAVE_ID_NOT( const int iDungeonWaveID_ ) { return false; };
#endif //SERV_DUNGEON_FORCED_EXIT_SYSTEM

#ifdef SERV_EVENT_VALENTINE_DUNGEON
		void						NotfiyCreateValentimeCupCake();
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef EFFECT_TOOL
		void RefreshDamageEffectScript();
#endif //EFFECT_TOOL

#ifdef ADDED_GET_SUBSTAGE_INDEX_IN_SCRIPT
		int		GetNowSubStageIndex();
#endif // ADDED_GET_SUBSTAGE_NUMBER_IN_SCRIPT

#ifdef ADDED_NPC_REMAINING_WHEN_SUBSTAGE_CLEAR
		void    DeleteRemainingNpcWhenSubStageClear ();
#endif // ADDED_NPC_REMAINING_WHEN_SUBSTAGE_CLEAR

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        void                        UpdateHostGameUnit();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		// 지정된 범위 안의 UnitUID 를 제외한 가장 가까운 NPC 를 가져옴
		// 범위를 -1 로 지정하면 범위 체크를 하지 않고 모든 NPC 를 검색함
		// MyTeam 을 지정하면 해당 Team 과 같으면 검색하지 않음
		// Check Invincible 을 지정하면 무적 상태를 검색하지 않음
		// 이쿠스, 인쿨로드 카드 NPC 적용 함수, kimjh
		CX2GUNPC*	GetNearestNpcInSpecificRangeAndExceptUnitUID( const D3DXVECTOR3& vPos_, const UidType UnitUid_, const float fMaxRange_ /* = 1000 */, const int iMyTeam /* = -1*/ , const bool bCheckInvincible /* = false */ );

#ifdef HAMEL_SECRET_DUNGEON // 김태환
		/// 해로운 효과라고 정의된 공식적인 디버프 리스트 ( 기획 파트에서 제공 )
		const vector<BUFF_TEMPLET_ID>& GetOfficiallyDebuffList() const { return m_vecPossibleOfficiallyDebuffList; }
		void SetOfficiallyDebuffList();

		void DisplayAddHPInformation( IN const float fAddHPValue, IN const D3DXVECTOR3 vPos, IN const D3DXVECTOR3 vDirVector, IN bool bDisplayPercent = true );		/// HP 회복량 표시
		void DisplayAddMPInformation( IN const float fAddMPValue, IN const D3DXVECTOR3 vPos, IN const D3DXVECTOR3 vDirVector );		/// MP 회복량 표시
#endif // HAMEL_SECRET_DUNGEON

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		void DisplayDamageInformation( IN const float fDamageValue_, IN const D3DXVECTOR3 vHeadBonePos_, IN const D3DXVECTOR3 vDirVector );	/// 데미지 표시
#endif //SERV_9TH_NEW_CHARACTER

#ifdef NOT_RENDER_NPC_GAME_EDIT
		bool						GetShowNpcByGameEdit() const { return m_bShowNpcByGameEdit; }
		void						SetShowNpcByGameEdit(bool val)
		{ 
			m_bShowNpcByGameEdit = val; 

			for( UINT i = 0; i < m_NPCUnitList.size(); ++i )
			{
				CX2GUNPC* pGUNPC = m_NPCUnitList[i];
				if( NULL != pGUNPC )
				{
					pGUNPC->SetShowObject( val, true );
				}
			}
		}
#endif // NOT_RENDER_NPC_GAME_EDIT

#ifdef MODFIY_LOG_IN_NPC_SCRIPT_FUNCTION
		void ChatBoxLog( const CHAR* pLog, float fLog );
#endif // MODFIY_LOG_IN_NPC_SCRIPT_FUNCTION

#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
        void FlushSendFrameAverage();
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		void SendFrameAverage();
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

#ifdef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		void SetAlreadySendingFrame(bool bVal_) { m_bIsAlreadySendingFrame = bVal_; }
		bool GetAlreadySendingFrame() const  { return m_bIsAlreadySendingFrame; }
#endif // SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

#ifdef SERV_CODE_EVENT
		bool						IsEnableCodeEvent_LUA( int iScriptID );
#endif SERV_CODE_EVENT

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
		void SetValentineEventTrigger_LUA( D3DXVECTOR3 vPos );
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

protected:

#ifdef	SHOW_UDP_NETWORK_INFO_IN_ROOM
		void						IncreaseUserPacketCountToReceive( const UidType uidGameUnit_ );
#endif	// SHOW_UDP_NETWORK_INFO_IN_ROOM

		void						KeyProcess();
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		void						UpdateMyGameUnit( const CX2Room::SlotData* pSlotData_, CX2GUUser* pUser );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        void                        CheckAmIHost();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		virtual void				UpdateUnitPointer();
		virtual void				AddUserUnit();

		void						CreateDropIem();
		virtual void				Verify();

		//void						ResetReBirthStoneNumUI(); // public 쪽으로 옮김
	
	
		void						UpdateSkillCoolTime( float fElapsedTime );

		void					NoticeContinue() 
		{ 
			m_fResurrectionRemainTime = 10.0f; 
			if( GetMyUnit() != NULL )
				GetMyUnit()->SetResurrectionTimeOut(false); 
		}

		void					ResurrectionFrameMove( double fTime, float fElapsedTime );

		virtual void			ResurrectionKeyProcess();

		float						m_fResurrectionRemainTime;	/// 부활 시간 카운트 (숫자 출력으로도 사용)
		bool						m_bIsSendResurrectionReq;	/// 부활 하겠다는 EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ를 수행 하였다는 플래그로, 부활 하겠다는 키를 누른 후 또 누를 수 없도록 방지
		bool						m_bResurrectionCountStart;	/// 부활 카운트 시작 여부
		float						m_fAutoResurrectionTime;	/// 필드에서 DIE상태로 5분이상 경과시 자동으로 마을로 이동하기 위한 카운트

#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
		void						InsertGUUserToCheckUDPTimer( const UidType uidCheckUser_ )
		{
			boost::timer timerFirst;
			timerFirst.restart();
			m_mapCheckUDPTimer.insert( std::make_pair( uidCheckUser_, timerFirst ) );
		}
		void				AbuserUserCheck( const CX2GUUser* pCheckUser_ );
		bool				CanCheckAbuser( const CX2GUUser* pCheckUser_, std::map< UidType, boost::timer >::iterator mit_ );
#else

#ifdef UDP_CAN_NOT_SEND_USER_KICK
		void						AbuserUserCheck(UidType iUnitUID_);
	#ifdef UDP_DOWNLOAD_BLOCK_CHECK
		void						AbuserUserReport();
	#endif //UDP_DOWNLOAD_BLOCK_CHECK
#endif //UDP_CAN_NOT_SEND_USER_KICK

#endif // SERV_FIX_SYNC_PACKET_USING_RELAY


		void						AddChatLogByGetItem(const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser,
														bool bIsEDItem);
		void						ProcessGetOutlayItem(const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser,
														int DropItemUID, bool bIsItemSuccess );
		void						ProcessGetEquipItem(const CX2Item::ItemTemplet* pItemTemplet);
		void						ProcessGetEDItem(CX2GUUser* pCX2GUUser, bool bIsEDItem,
													KDungeonRewardED DungeonRewardED, UidType uiGetUnitUID);




		CKTDGUIDialogType			m_pDLGResurrect;		/// 다른 유저를 부활 시킬 것인지에 대한 OK, Cancle다이얼로그 
		UidType						m_UnitUIDToResurrect;		/// 부활 시킬 다른 유저의 UID
		CKTDGUIDialogType			m_pDLGContinue;			/// 부활 할 것인지에 대한 다이얼로그
		CKTDXDeviceSound*			m_pSoundContinue;		/// 부활 시 나오는 사운드
		CKTDGUIDialogType			m_pDLGCheckResurrectByUseCash;	/// 부활석 캐시를 사용하겠다고 Z를 눌렀을 때 나오는 부활석 구입 팝업창
		KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ m_ReservedPacket;	/// 이전 부활석 대상을 기억하기 위한 용도, 그러나 UID만 필요한데 이 패킷을 멤버로 사용하는 것은 낭비인듯, 그리고 이전에 다른 유닛을 부활 시켰다면 캐시를 자동으로 다른 유닛 살리는데 사용할 듯 함
		float						m_fTimeWaitStopContinue;	/// 실시간 캐시 부활석 용인것 같은데...캐시부활석 사용 창이 출력되고 있는 경우 9초동안만 출력을 제한 하려는 용도, 그 이후에는 캐시 부활석 사용 창을 없앰
		WCHAR						m_StopContinueAlarmText[MAX_PATH];	/// 캐시 부활석 구매 관련 글
		bool						m_bStopRebirthCount;	/// true가 되면 m_fResurrectionRemaintime을 감소 시키지 않는다.

		float						m_fTimeEnableKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ;	/// 
		bool						m_bEnableSendKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ;	/// 이게 과연 KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ 와 관련이 있는 건지... KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ 와 관련이 있는 건지...

		//{{ kimhc // 게임 로딩이 끝날때 까지 ESC 처리 안되게 하는 작업
		bool						m_bCanUseESC;		/// 이런건 모든 게임에 공유해도 되지 않을까..?
		//}} kimhc // 게임 로딩이 끝날때 까지 ESC 처리 안되게 하는 작업

		GAME_TYPE					m_GameType;			/// 게임 타입
		GAME_STATE					m_GameState;		/// 게임 상태 (INIT, PLAY, LOADING, END)
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        UidType                     m_uidMySlotUID;
        UidType                     m_uidHostSlotUID;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		bool						m_bHost;			/// 다수의 방장이 존재하게될 필드라면 필요없을듯
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		CX2Room*					m_pRoom;			/// 룸이 존재해야하는 경우 포인터말고...참조로..
		float						m_fTime;			/// 필요 없음
		float						m_fElapsedTime;		/// 필요없음
		float						m_fGameTime;		/// m_AverageRenderFrameRate(통계용)를 구하기 위해 사용(PVP에서는 이것을 사용 하나, 던전에서는 SubStage에 있는 것을 사용 함... 필드에서도 사용해야 할 듯)
		bool						m_bGameWait;		/// 서비스가 아닐때 Control 키로 게임 멈출 때사용

		//char						m_NextInGameID;
		CX2GameUnitoPtr				m_optrMyUnit;			/// 공통..으로 써도 될 듯?? 필드시스템에서는 게임시작과 동시에 자신의 GUUser를 생성할 것이라면 참조로 가지고 있거나 해도...
		CX2GameUnitoPtr				m_optrFocusUnit;		/// 특별한 이슈가 없다면 카메라가 향하고 있어야 하는 유닛
		float						m_fFocusTime;		/// 일정 시간동안 특정 유닛에게 카메라 포커스가 향하고 있도록 하려고 하는 것 같은데... UpdateUnitPointer이 지금 제대로 작동 하는건가...?

		vector<CX2GameUnit*>		m_UnitList;			/// 유저, 몬스터 모두의 포인터를 다 가지고 있는 벡터 // 근데.. 굳이 이것을 가지고 있어야 할 이유는 없는듯...
		vector<CX2GUUser*>			m_UserUnitList;		/// 유저들 포인터만 가지고 있는 벡터
		vector<CX2GUNPC*>			m_NPCUnitList;		/// 몬스터들 포인터만 가지고 있는 벡터
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		vector<CX2GUUser*>			m_IntruderList;		/// 난입한 유저 포인터 벡터
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		CX2GUUser::InputData		m_InputData;	/// 공통

		CX2World*					m_pWorld;	/// 공통
		CX2Camera*					m_pCamera;	/// 공통
		CX2DamageEffect*			m_pDamageEffect;	/// 미리 생성?

 		//CKTDGParticleSystem*		m_pMajorParticle;
 		//CKTDGParticleSystem*		m_pMinorParticle;
 		//CKTDGXMeshPlayer*			m_pMajorXMeshPlayer;
 		//CKTDGXMeshPlayer*			m_pMinorXMeshPlayer;
 		//CX2EffectSet				m_EffectSet;



		bool						m_bBGMOn;	/// 사용 안함 (모두 true 임)
		bool						m_bLastKillCheck;	/// 지금 죽인 애를 끝으로 게임이 끝이라면..(던전과 PVP에는 끝이라는게 존재하지만 마을과 필드에는 존재하지 않음..)
		float						m_fLastKillWaitTime;	/// 이 값이 0이 된 이후부터 lastKillCheck를 true로 할지 false로 할 지를 판단 함
		float						m_fLastKillWaitTimeAfterRebirth; /// 부활 한 사람이 부활 직후에는 HP가 0이 되어 버려서 m_bLastKillCheck가 true가 되는데 그 경우를 방지하기 위한 타이머

		bool						m_bEnableCommandKeyProcess;			/// 훈련소, 인탱글등에 의해서 공격키 입력 못하게 할 때 
		bool						m_bEnableAllKeyProcess;				/// 훈련소, Extra 데미지 등에 의해 키입력을 받을 수 없게 하는 용도록 사용

//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
        int                         m_iPacketSendFrameMoveCount;
        int                         m_iNPCRobustPacketSendIndex;
        DWORD                       m_dwNPCRobustPacketSendBaseFrameMoveCount;
//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//		float						m_NPCPacketSendInterval;			/// NPC Packet 전송 간격
//		float						m_NPCPacketSendIntervalNow;			/// NPC Packet 전송 타이머
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

		float						m_LineSyncPacketSendInterval;		/// MOVING_LINE_MAP_TEST 비활성으로 사용하지 않음
		float						m_LineSyncPacketSendIntervalNow;	/// MOVING_LINE_MAP_TEST 비활성으로 사용하지 않음


		CX2UnitLoader				m_UnitLoader;						/// X2Game 공통
		int							m_LoadingPercent;					/// 사용 안함
		vector<CreateNPCData*>		m_CreateNPCDataList;				/// CreateNPC()를 통해 데이터가 push_back 되고 m_bLastKill이 false인 경우에 OnFrameMove에서 생성됨
		KEGS_NPC_UNIT_CREATE_REQ	m_PushCreateNPCReqList;				/// PushCreateNPCReq() 함수를 통해 NPC를 여러명 생성할 경우.. 채워짐, FlushCreateNPCReq()를 통해 SendPacket을 보냄

		CX2DropItemManager*			m_pDropItemManager;					/// Dungeon, PVP, BattleField에서 다 사용 할 듯
		
		
		CKTDGFontManager::CUKFont*	m_pFontForUnitName;					/// X2Game 공통

		bool						m_bStopUnitChain;					/// true인 경우 NPC가 죽었다는 패킷의 즉시 전송, 모든 Unit과 Pet에 대한 FrameMove 처리를 하지 않음 (StageLoding 시 true고, subStage가 Start 할 때 false가 된다. 현재 사용하는 곳은 던전에서만 사용 함)

		float							m_fLastkillShotTime;			/// 던전 스테이지 클리어 시에 스샷찍을때 사용 (지금은 던전에서만 사용하는 것처럼 보이는데...)
		CKTDXDeviceRenderTargetTexture*	m_pRenderTargetLastkillShot;	/// 던전 스테이지 클리어 시에 스샷 찍기

		
		bool						m_bOpenChatBox;	/// 채팅 박스 on/off
		CX2TalkBoxManager*			m_pTalkBoxMgr;	/// X2Game 공통

		float						m_fElapsedTimeAfterLastKeyboardInput;	/// 튜토리얼에서 사용

		vector<NewGameUser>			m_NewGameUserList;		///게임중 새로 난입한 유저리스트

		int							m_RedTeamKill;		/// RedTeamKill 한 횟수, TeamDeath에서만 사용
		int							m_BlueTeamKill;		/// RedTeamKill 한 횟수, TeamDeath에서만 사


		bool						m_bRenderUserName;	/// 유저의 캐릭명을 렌더링 할지 여부
		static bool					m_sbRenderNPCName; /// ctrl-key를 누르고 있으면 npc이름을 화면에 출력(버튼 N으로 켜고 끌수있게 되어 있으나, 멤버는 이곳에, 처리는 StateDungeon에만 되어있음)
		float						m_fSkipTime;		/// 왜 사용하는지 모르겠다... 300이 지나면 왜 스킵하는 것일까? 백그라운드를 위해서??

		bool						m_bShowF1Help;		/// 사용 안함
		vector<wstring>				m_vecF1HelpString;	/// 사용 안함
		CKTDXDeviceTexture*			m_pTextureF1Help;	/// 사용 안함
		CKTDGFontManager::CUKFont*	m_pFontF1Help;		/// 사용 안함

		bool						m_bEnableNPCTalkBox;	/// NPC가 TalkBox를 CommonStateStart에서 수행 할지 여부, subStageOpen 하고 2초 후에 true가 됨
		float						m_fElapsedTimeAfterDisableNPCTalkBox; /// NPC TalkBox가 Disable된 시간

		KProtectedType<int>			m_GameScore;		/// 던전에서만 사용
		KProtectedType<int>			m_GameScoreBack;	/// 필드 들어가면 사라질 예정
		vector<ScoreTime*>			m_ScoreTimeList;	/// 필드 들어가면 사라질 예정

		float											m_fTechPointViewTime;	/// 테크포인트가 보이는 시간 (약 1.5초로 지정되어 그 사이에 또 다른 테크포인트를 추가하면 추가되어 보여지고, 추가하지 않으면 사라지고 score에 더해짐)
		KProtectedType<int>								m_TechPoint;	/// 테크포인트로  m_fTechPointViewTime 내에 TechPoint가 추가 되지 않으면 0으로 초기화 됨
		KProtectedType<int>								m_TechPointFull;	/// 누적 테크 포인트
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템
		KProtectedType<int>								m_iSubStageTechPoint;	/// 서브 스테이지 테크 포인트
#endif	// SERV_DUNGEON_FORCED_EXIT_SYSTEM
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqTech;		/// 테크포인트 UI 핸들


		vector<CX2TextManager::CX2Text*>		m_vecpGuideText;	/// 사용 안함
		CX2TextManager::CX2Text*				m_pTextGuideHelp;	/// 사용 안함
		float						m_fTimePauseNPCAI;				/// 보스네임등이 나올때 AI를 잠시 중지시킬 시간...(근데 이거 사용했다가 시간 다되면 원래 AI가 false인 애들도... AI가 true가 되겠네..)
		
		bool						m_bFreeCamera;					/// X2Game공통 현재는 치트에서만 사용하는 듯...
		CFirstPersonCamera			m_FPSCamera;					/// X2Game공통 bFreeCamera가 false면 쓰지 않는데...

#ifdef KEYFRAME_CAMERA
		bool						m_bWorldCameraEdit;				/// 월드카메라
#endif KEYFRAME_CAMERA

		KProtectedType<int>			m_iNpcDiePacket;

		CKTDGUIDialogType				m_pDLGMyScore;				/// 부활석 과 Score를 나타내는 다이얼로그
		CKTDGUIDialogType				m_pDLGSkillSlot;			/// 스킬 슬롯인데... Gage매니저에 있는게 아니었군...
		
		int							m_TeamScoreAdvRed;				/// 사용 안함
		int							m_TeamScoreAdvBlue;				/// 사용 안함
		int							m_TeamVPMaxAdvRed;				/// 사용 안함
		int							m_TeamVPMaxAdvBlue;				/// 사용 안함

		int							m_AllowFrameCount;				/// EGS_REMAINING_TIME_NOT 패킷에 의해 갱신 되며, 그 시간내에 해당 패킷을 다시 받지 못해서 0이되면 키입력을 할수없게 됨
		int							m_SpeedHackCheck;				/// AllowFrameCout가 0인 경우 EGS_REMAINING_TIME_NOT에서 이 값을 증가 시키고 10을 초과한 경우 해킹 유저로 판단

		map<int, int>				m_mapKilledNPCKeyCode;			/// 죽은 npc의 키코드를 가지고 있음, 서브스테이지 시작할 때 초기화 됨, 던전에서만 필요할 듯

		int							m_GetEXPInGame;					/// 사용 안함(실시간 이후 사용X)

		bool						m_bGameEnd;		/// 게임이 끝났다는 플래그, 끝나지 않는 던전에서는 필요 없을 듯
		
		bool						m_bShowAllUnitGageBar;		/// 슬라이드 샷이 찍힐때 게이지가 보이지 않도록 false가 됨
#ifdef	ELOG_STATISTICS_NEW_DATA
		KGAME_STATISTICS            m_kGameStatistics;
#else	ELOG_STATISTICS_NEW_DATA
		int							m_AverageRenderFrameRate;	/// 평균 프레임 계산. 필드도 필요
#endif	ELOG_STATISTICS_NEW_DATA

		wstring						m_StateString;				/// 유저 현재 스테이트에 스테이트명이 있는 경우 출력 해준다.
		InfoTextManager				m_InfoTextManager;			/// 마나가 부족 하다는 등 화면 가운데에 나오는 메시지
		bool						m_bSkipKeyPressed;			/// 튜토리얼에서 사용
		
		CX2GUNPCFunc*				m_pNPCFunc;					/// 이거 사용하는 NPC들 다시 제작 하고 없애자

		// 게임중 '미션' 관련
		CKTDGUIDialogType				m_pDLGMissionOpening;	/// 사용 안함
		CKTDGUIDialogType				m_pDLGMissionProgress;	/// 사용 안함
		
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CRITICAL_SECTION			m_csGameIntruder;	/// 유저 난입에 대한 크리티컬 섹션
		HANDLE						m_hEvents[X2_INTRUDE_EVENT_MAX_VALUE];	/// 사용 안함
		vector<	IntrudeData* >		m_vecIntrudeData;	/// 사용 안함
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD



#ifdef BUBBLE_BOBBLE_TEST
		bool						m_bFixedCamera;		/// 현재는 던전에서만 사용 되고 있음...
#endif BUBBLE_BOBBLE_TEST


		bool						m_bEnableMonsterFlyTest;	/// 삭제해야할듯, true 외에 변경하는 곳이 없음

		float						m_arraySkillCoolTime[3];	/// 사용 안함
#ifdef NEW_SKILL_TREE
		bool						m_bSkillSlotJustChanged;	/// 내구도가 다 소모되었거나, 장비가 바뀌었거나, Space 키로 Slot을 change 한 후 부터 UpdateSkillCoolTime을 실행하지 않은 경우 true
#endif



		
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		KXPT_UNIT_NPC_SYNC_PACK		m_kXPT_UNIT_NPC_SYNC_PACK;	/// npc싱크패킷을 모아 놓은 구조체
//		std::vector<UidType>		m_vecUserUIDforSyncPacket;	/// 매 프레임 유저리스트에 있는 UID를 가져와서 Broad 캐스트 때 사용 함
//
//#ifdef SERV_PET_SYSTEM
//		KXPT_UNIT_PET_SYNC_PACK		m_kXPT_UNIT_PET_SYNC_PACK;	/// Pet 싱크 패킷을 모아 놓은 구조체
//#endif
//#endif SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		bool m_bShowTeleportEffectOnStageStart;		/// 서브스테이지, 스테이지 등이 있는 던전에서만 필요할 지도... 서브스테이지 클리어 후 다른 라인맵으로 보낼때 포탈효과를 보여줌 (그와 동시에 유저들은 보이지 않다가 substagestart 일 때 다시 보이도록 처리함)


#ifdef FPS_SHOOTING_TEST
		int m_iFPSShootingBulletType;
#endif FPS_SHOOTING_TEST




#ifdef RESET_INVALID_UNIT_POSITION_TEST

		std::set< UidType > m_setMisplacedUser;		/// 사내에서만 사용
		std::set< UidType > m_setMisplacedNPC;		/// 사내에서만 사용

#endif RESET_INVALID_UNIT_POSITION_TEST

#ifdef DUNGEON_ITEM
		std::vector<CX2EffectSet::Handle> m_vechEffectSetSpirit;	/// 던전 대전용 아이템 이펙트 핸들, 먹었을때 생성되고, 서브스테이지 종료시에 삭제..(오리에드, 드라이어드, 라울, 일리피아 등)
#endif


#ifdef REVERSE_GRAVITY_TEST
		std::vector< ReverseGravityData > m_vecReverseGravityData;	/// 란이 사용하는 리버스 그래비티... 포인터가 아니군..
#endif REVERSE_GRAVITY_TEST

#ifdef DUNGEON_ITEM
		D3DXVECTOR3 m_vDropItemPos;		/// 마을 외의 게임에는 모두 필요할듯..

		CX2GameUnitoPtr		m_optrItemDamageEffectGameUnit;	/// 던전, 대전 소환 아이템을 먹은 유저의 포인터
		float		m_fItemDamageEffectTime;	/// 오리에드, 라울의 분노 처럼 데미지를 주는 것의 경우 약 0.85초 이후에 실행되도록 이것으로 DamageEffecttime을 맞춤
		int			m_iItemDamageEffectIndex;	/// 정령 소환 종류 (1은 오리에드, 3을 라울)
		vector<D3DXVECTOR3> m_vecEffectPos;		/// 필요없음. 정령소환에타격당할 애들의 위치를 담는다.. 그러나 멤버로 가지고 있을 이유가 전혀 없음. 그냥 로컬로 가지고있어도 됨
#endif

#ifdef DAMAGE_HISTORY
		bool		m_bSendLeaveRoom;		// 사용하는 곳 없음
#endif
		
		//{{ kimhc // 2011-07-28 // 옵션수치화
		bool		m_bIsDamageFreeGame;	/// 이 값을 얻어오는 것을 고쳐야 함. 1레벨 보정 던전인가?
		//}} kimhc // 2011-07-28 // 옵션수치화

		float m_fShowAlarmTime;
		std::vector<DangerAlarm*> m_vecDangerAlarm;
		DangerAlarm *m_pShowAlarm;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hDangerAlarm;
//#ifdef	BATTLE_FIELD_TEST
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        CX2GUUseroPtr			m_optrHostGameUnit;		// 필드 테스트용
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		CX2GameUnitoPtr			m_optrHostGameUnit;		// 필드 테스트용
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//#endif	BATTLE_FIELD_TEST
		
#ifdef ADD_GAME_STAGE_DELETE_DAMAGEEFFECT
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        std::vector<CX2DamageEffect::CEffectHandle>      m_vecRemoveDamageEffect;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		std::vector<CX2DamageEffect::CEffect *> m_vecRemoveDamageEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif

		//{{ 2012. 10. 31	박세훈	Merge 랜선랙 방지 작업-릴레이를 기본적으로 사용한다.
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
		std::map< UidType, boost::timer >		m_mapCheckUDPTimer;
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
		//}}
		
#ifdef MODIFY_DUNGEON_STAGING		
		CinematicUI	*m_pCinematicUI; //화면 위,아래 검게 만드는 UI
		bool		m_bCanNotInputAndPauseNPCAI;
#endif //MODIFY_DUNGEON_STAGING

		/** @function : InpuData
			@brief : 키입력 체크
		*///오현빈//2012-10-17//코드 길이 조금이라도 줄이기 위해 추가
		inline void InputData( IN GAME_ACTION eGameKey_, OUT bool& pureDouble, OUT bool& oneDouble, OUT bool& pure, OUT bool& one )
		{
			if( GET_DOUBLEKEYPURE_STATE( eGameKey_ ) == TRUE )
			{
				pureDouble = true;
			}
			if( GET_DOUBLEKEY_STATE( eGameKey_ ) == TRUE )
			{
				oneDouble = true;
			}
			if( GET_PUREKEY_STATE( eGameKey_ ) == TRUE )
			{
				pure = true;
			}
			if( GET_KEY_STATE( eGameKey_ ) == TRUE )
			{
				one = true;
			}
		}

#ifdef FIX_OBSERVER_MODE
		UINT m_uiPositionIndexRed;	/// 옵저버 화면에 표시될 PVP 레드 팀 유저 UI 표시 순서 인덱스
		UINT m_uiPositionIndexBlue;	/// 옵저버 화면에 표시될 PVP 블루 팀 유저 UI 표시 순서 인덱스
#endif FIX_OBSERVER_MODE

#ifdef BALANCE_DEADLY_CHASER_20130214
		std::set<UidType>	m_setPreLockOnTargetList;
#endif //BALANCE_DEADLY_CHASER_20130214

#ifdef HAMEL_SECRET_DUNGEON // 김태환
		vector<BUFF_TEMPLET_ID> m_vecPossibleOfficiallyDebuffList;	/// 해로운 효과라고 정의된 공식적인 디버프 리스트 ( 기획 파트에서 제공 )
#endif // HAMEL_SECRET_DUNGEON

#ifdef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		bool				m_bIsAlreadySendingFrame;
#endif // SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		UidType m_UnitUIDOut;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	public:
		void StartTcpPingCheck();

		void Handler_EGS_TCP_PING( KEGS_TCP_PING& kEvent );

	private:

		typedef std::map< UINT, KEGS_TCP_PING > KTcpPingMap;

		KTcpPingMap m_mapTcpPing;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

public:
    CX2FrameUDPPack&    GetFrameUDPPack() { return m_kFrameUDPPack; }

protected:

    CX2FrameUDPPack m_kFrameUDPPack;

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


#ifdef NOT_RENDER_NPC_GAME_EDIT
	bool			m_bShowNpcByGameEdit;		///	kimhc // 20131011 // GameEdit 에서 몬스터들 보이게 또는 안보이게 할 수 있는 치트
#endif // NOT_RENDER_NPC_GAME_EDIT
};

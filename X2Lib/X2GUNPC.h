#pragma once

typedef srutil::delegate0<void> NPCStateCBFunc;

#ifdef  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE
#define NPC_UNIT_DIE_STATE_TIME_LIMIT   10.f
#endif	X2OPTIMIZE_ROBUST_NPC_FINAL_STATE

class CX2GUUser;
class CX2NPCAI;
class CNpcCreatedInfo;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CNpcCreatedInfo> CNpcCreatedInfoPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CNpcCreatedInfo> CNpcCreatedInfoPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

#define MAGIC_SPEED_BOUND 800.0f
#define MAGIC_MAX_UNIT_DISPLACE 25.0f
#define MAGIC_DEFALUT_UNIT_DISPLACE 15.0f
#define MAGIC_DEFAULT_UNIT_WIDTH 100.0f

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
class   CX2FrameUDPPack;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#define NOWSTATE_FLAG( flag )      nowStateData.m_bitsetFlags.test( StateData::FLAG_##flag )
#define NOWSTATE_SET_FLAG( flag, bValue )   nowStateData.m_bitsetFlags.set( StateData::FLAG_##flag, (bValue) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


class CX2GUNPC : public CX2GameUnit
{
//{{ Public Variable Start
	public:
		enum STATE_CHANGE_TYPE
		{
			SCT_NO_CHANGE,
			SCT_MOTION_END,
			SCT_FOOT_ON_LINE_FALSE_DOWN,
			SCT_FOOT_ON_LINE_TRUE,
			SCT_POSITIVE_Y_SPEED,
			SCT_NEGATIVE_Y_SPEED,

			SCT_CONDITION_FUNCTION,
			SCT_CONDITION_TABLE,

			SCT_AI_MESSAGE_BEGIN,
			SCT_AI_WAIT,
			SCT_AI_WALK,
			SCT_AI_DASH,
			SCT_AI_JUMP,
			SCT_AI_DOWN,
			SCT_AI_JUMP_DIR,
			SCT_AI_DOWN_DIR,
			SCT_AI_DIR_CHANGE,

			SCT_AI_FLY_FRONT,
			SCT_AI_FLY_FRONT_UP,
			SCT_AI_FLY_FRONT_DOWN,
			SCT_AI_FLY_BACK,
			SCT_AI_FLY_BACK_UP,
			SCT_AI_FLY_BACK_DOWN,
			SCT_AI_FLY_UP,
			SCT_AI_FLY_DOWN,
//{{oasis:김상윤////2009-10-7////
			SCT_AI_SELF_COMMAND_TRANSIT,			
//}}oasis:김상윤////2009-10-7////
			SCT_AI_MESSAGE_END,
		};


//#ifdef TRANSFORMER_TEST
//		enum KEYBOARD_EVENT_TYPE
//		{
//			KET_NONE,
//
//			KET_PURELEFT,
//			KET_ONELEFT,
//			KET_PUREDOUBLELEFT,
//			KET_ONEDOUBLELEFT,
//
//			KET_PURERIGHT,
//			KET_ONERIGHT,
//			KET_PUREDOUBLERIGHT,
//			KET_ONEDOUBLERIGHT,
//
//			KET_PUREUP,
//			KET_ONEUP,
//			KET_PUREDOUBLEUP,
//			KET_ONEDOUBLEUP,
//
//			KET_PUREDOWN,
//			KET_ONEDOWN,
//			KET_PUREDOUBLEDOWN,
//			KET_ONEDOUBLEDOWN,
//
//			KET_PUREZ,
//			KET_ONEZ,
//			KET_PUREDOUBLEZ,
//			KET_ONEDOUBLEZ,
//
//			KET_PUREX,
//			KET_ONEX,
//			KET_PUREDOUBLEX,
//			KET_ONEDOUBLEX,
//
//			KET_PUREC,
//			KET_ONEC,
//			KET_PUREDOUBLEC,
//			KET_ONEDOUBLEC,
//
//			KET_PUREA,
//			KET_ONEA,
//			KET_PUREDOUBLEA,
//			KET_ONEDOUBLEA,
//
//			KET_PURES,
//			KET_ONES,
//			KET_PUREDOUBLES,
//			KET_ONEDOUBLES,
//
//			KET_PURED,
//			KET_ONED,
//			KET_PUREDOUBLED,
//			KET_ONEDOUBLED,
//
//			KET_PUREF,
//			KET_ONEF,
//			KET_PUREDOUBLEF,
//			KET_ONEDOUBLEF,
//
//
//
//			KET_PURELSHIFT,
//			KET_ONELSHIFT,
//			KET_PUREDOUBLELSHIFT,
//			KET_ONEDOUBLELSHIFT,
//
//			KET_PURELALT,
//			KET_ONELALT,
//			KET_PUREDOUBLELALT,
//			KET_ONEDOUBLELALT,
//
//			KET_PURESPACE,
//			KET_ONESPACE,
//			KET_PUREDOUBLESPACE,
//			KET_ONEDOUBLESPACE,
//
//			KET_PURE_LEFT_CTRL,
//			KET_ONE_LEFT_CTRL,
//			KET_PUREDOUBLE_LEFT_CTRL,
//			KET_ONEDOUBLE_LEFT_CTRL,
//
//
//		};
//
//#endif TRANSFORMER_TEST


		enum MIND_FLAG
		{
			MF_NO = 0,
			MF_TARGET_TAKE,
			MF_TARGET_LOST,
			MF_STAND_UP_ATTACK,
		};

//{{oasis:김상윤////2009-10-7////
		enum GROUP_AI_COMMAND
		{
			GAI_CMD_OFF,
			GAI_CMD_HOLD,
			GAI_CMD_ATTACK_MOVE,
			GAI_CMD_FORCED_MOVE,	/// 보통 Commander가 call 할때 callee의 Group AI로 사용됨
			GAI_CMD_SKILL_A,
			GAI_CMD_SKILL_B,
			GAI_CMD_SKILL_C,
			GAI_CMD_SKILL_D,
			GAI_CMD_SKILL_E,
			GAI_CMD_SKILL_F,

		};

		enum GROUP_AI_TARGET_TYPE
		{
			GAI_TT_USER,
			GAI_TT_NPC,
			GAI_TT_GROUND,
		};		
//}}oasis:김상윤////2009-10-7////

#ifdef RIDING_MONSTER
		enum RIDE_STATE
		{
			RS_NOT_AVAILABLE,
			RS_READY,
			RS_TRY_TO_RIDE,
			RS_ON_RIDING,
		};
		enum RIDE_TYPE
		{
			RT_NONE,
			RT_RIDER,
			RT_RIDING,
		};
#endif RIDING_MONSTER

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-12
		/// 몬스터 생성 타입 ( CreateNPCReq )
		enum NPC_CREATE_TYPE
		{
			NCT_NONE,
			NCT_MONSTER_CARD,
			NCT_DEFENCE_DUNGEON,
#ifdef FINALITY_SKILL_SYSTEM //김창한
			NCT_CHANGESTAGE_REMAIN, //스테이지가 지나가도 소환유지되는 몬스터 타입
#endif //FINALITY_SKILL_SYSTEM
		};
#endif // SERV_NEW_DEFENCE_DUNGEON

		/// 레벨 차이에 따른 네임 색 규정
// 		enum NAME_COLOR_BY_LEVEL_DEFFERENCE
// 		{
// 
// 		};
        
		//{{ 2009. 12. 18  최육사	던전경험치개편
		enum MONSTER_GRADE
		{
			MG_NORMAL_NPC				= 0,
			MG_BOSS_NPC					= 1,
			MG_MIDDLE_BOSS_NPC			= 2,
#ifdef SERV_ADD_MONSTER_GRADE_FOR_DEFINITELY_DROP_HP_BALL_ITEM// 작업날짜: 2013-08-28	// 박세훈
			MG_DEFINITELY_DROP_HP_BALL	= 3,
#endif // SERV_ADD_MONSTER_GRADE_FOR_DEFINITELY_DROP_HP_BALL_ITEM
		};
		//}}

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION

        struct  StateData;

        struct  InitComponentData
        {
            float           m_fDefaultAnimSpeed;
            InitComponentData()
                : m_fDefaultAnimSpeed( 1.0f ) {}
        };

        enum    EStateType
        {
            STATE_TYPE_START_STATE,
            STATE_TYPE_REBIRTH_STATE,
            STATE_TYPE_WAIT_STATE,
            STATE_TYPE_RAGE_STATE,
            STATE_TYPE_SIEGE_STATE,
            STATE_TYPE_DISABLED_STATE,

            STATE_TYPE_REACTION_STATE_BEGIN,

            STATE_TYPE_SMALL_DAMAGE_LAND_FRONT = STATE_TYPE_REACTION_STATE_BEGIN,
            STATE_TYPE_SMALL_DAMAGE_LAND_BACK,
            STATE_TYPE_SMALL_DAMAGE_AIR,
#ifdef AI_FLY
            STATE_TYPE_SMALL_DAMAGE_AIR_FRONT,
            STATE_TYPE_SMALL_DAMAGE_AIR_BACK,
#endif 
            STATE_TYPE_BIG_DAMAGE_LAND_FRONT,
            STATE_TYPE_BIG_DAMAGE_LAND_BACK,
            STATE_TYPE_BIG_DAMAGE_AIR,
#ifdef AI_FLY
            STATE_TYPE_BIG_DAMAGE_AIR_FRONT,
            STATE_TYPE_BIG_DAMAGE_AIR_BACK,
#endif
            STATE_TYPE_DOWN_DAMAGE_LAND_FRONT,
            STATE_TYPE_DOWN_DAMAGE_LAND_BACK,
            STATE_TYPE_DOWN_DAMAGE_AIR,
            STATE_TYPE_DOWN_DAMAGE_AIR_LANDING,
            STATE_TYPE_UP_DAMAGE,
            STATE_TYPE_FLY_DAMAGE_FRONT,
            STATE_TYPE_FLY_DAMAGE_BACK,
#ifdef RIDING_MONSTER
            STATE_TYPE_RIDE_DAMAGE_FRONT,
            STATE_TYPE_RIDE_DAMAGE_BACK,
#endif RIDING_MONSTER
            STATE_TYPE_DAMAGE_FLUSH_LAND_FRONT,
            STATE_TYPE_DAMAGE_FLUSH_LAND_BACK,
            STATE_TYPE_DAMAGE_FLUSH_AIR,
            STATE_TYPE_DAMAGE_REVENGE,
            STATE_TYPE_DEFENCE_STATE,
            STATE_TYPE_SMALL_DAMAGE_LAND_GUARD,
            STATE_TYPE_BIG_DAMAGE_LAND_GUARD,
            STATE_TYPE_DOWN_DAMAGE_LAND_GUARD,
#ifdef GRAPPLING_TEST
            STATE_TYPE_DAMAGE_GRAPPLED_FRONT,
            STATE_TYPE_DAMAGE_GRAPPLED_BACK,
            STATE_TYPE_DAMAGE_THROWN,
#endif GRAPPLING_TEST
            STATE_TYPE_REACTION_STATE_NUMS,
            STATE_TYPE_REVENGE_ATTACK = STATE_TYPE_REACTION_STATE_NUMS,
            STATE_TYPE_DYING_LAND_FRONT,
            STATE_TYPE_DYING_LAND_BACK,
            STATE_TYPE_DYING_SKY,
#ifdef DIE_FLY
            STATE_TYPE_DYING_FLY,
#endif DIE_FLY
            STATE_TYPE_SELF_DESTRUCTION_STATE,
            STATE_TYPE_IMMUNE_SKILL_STATE, 
#ifdef RIDING_MONSTER
            STATE_TYPE_AIR_FALL,
            STATE_TYPE_RIDE_STATE,
#endif RIDING_MONSTER
#ifdef SERV_PVP_NEW_SYSTEM
            STATE_TYPE_WIN_STATE,
            STATE_TYPE_LOSE_STATE,
#endif SERV_PVP_NEW_SYSTEM
#ifdef SUMMON_MONSTER_CARD_SYSTEM
            STATE_TYPE_SUMMON_END_STATE,
#endif SUMMON_MONSTER_CARD_SYSTEM
#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
            STATE_TYPE_LINK_JUMP_STATE,
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST
            STATE_TYPE_NUMS,
        };//

        struct  InitStateData
        {
            std::vector<StateData>                      m_vecStateData;
            std::map<string,char>                       m_mapStateNameID;
            bool                                        m_bSyncTargetInfo;
            std::string                                 m_strCommonCameraMove;
            std::string                                 m_strCommonFrameMove;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            int                                         m_iHintTimerEventID;
            std::vector<float>                          m_vecHintTimerEventTrigger;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            std::string                                m_strAttackedFunc;
            char                                        m_acStateType[STATE_TYPE_NUMS];

            InitStateData()
                : m_vecStateData()
                , m_mapStateNameID()
                , m_bSyncTargetInfo( false )
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                , m_iHintTimerEventID( -1 )
                , m_vecHintTimerEventTrigger()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            {
                ZeroMemory( m_acStateType, sizeof(m_acStateType) );
                m_vecStateData.resize(1);
            }
	
            int                         GetStateID( const std::string& strInState_ ) const
            {
                std::map<string,char>::const_iterator iter = m_mapStateNameID.find( strInState_ );
                if ( iter == m_mapStateNameID.end() )
                    return GUSI_NONE;
                return iter->second;
            }//FindStateIDByName()
        };
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION


		struct  InitData
		{
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
			bool				m_bLuaShareable;
            bool                m_bHeuristicSimulation;
            bool                m_bViewDepthHeuristic;
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
			InitDeviceData      m_device;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
            InitComponentData   m_component;
            InitStateData       m_state;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
			InitData()
				: m_bLuaShareable( false )
                , m_bHeuristicSimulation( false )
                , m_bViewDepthHeuristic( true )
			{
			}
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING

			void                AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const;
		};//struct  InitData

// 		struct HardLevelData
// 		{
// 			float						m_HP;
// 			int							m_EXP;
// 			CX2DamageManager::Damage	m_Attack;
// 			CX2DamageManager::Damage	m_Defence;
// 			float						m_TriggerRate;
// 
// 			HardLevelData()
// 			{
// 				m_HP			= 0.0f;
// 				m_EXP			= 0;
// 				m_TriggerRate	= 0.0f;
// 			}
// 		};




		struct NPCSyncData
		{
			char				nowState;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            KTWO_PARTS          stateChangeParts;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			D3DXVECTOR3			position;
			unsigned char		lastTouchLineIndex;

			bool				bIsRight;
//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			bool				bStateChange;
//#endif   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			bool				bComeBackState;

			char				mindFlag;


#ifdef NEW_RANDOM_TABLE_TEST
			USHORT              m_usRandomTableIndex;
#else NEW_RANDOM_TABLE_TEST
//{{AFX
			char				m_StateRandomSeed;
			char				m_StateRandomSeed2;		
//}}AFX
#endif NEW_RANDOM_TABLE_TEST

			NPCSyncData()
			{
				Init();
			}
			void Init()
			{
				nowState				= 0;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                stateChangeParts.stateChangeNum          = 0;
                stateChangeParts.stateModifyNum          = 0;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				INIT_VECTOR3( position,	0,0,0 );
				lastTouchLineIndex		= 0;

				bIsRight				= true;
//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				bStateChange			= false;
//#endif   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				bComeBackState			= false;

				mindFlag				= MF_NO;
#ifdef NEW_RANDOM_TABLE_TEST
				m_usRandomTableIndex	= 0;
#else NEW_RANDOM_TABLE_TEST
//{{AFX
				m_StateRandomSeed		= 99;		// 이 확률을 이용한 이벤트가 작동하지 않도록 default를 99로 한다. 값 변경할 때 주의!
				m_StateRandomSeed2		= 99;				
//}}AFX
#endif NEW_RANDOM_TABLE_TEST
	
			}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			static void _DecodeNpcSyncPos( D3DXVECTOR3& vPosition, USHORT usPosX, USHORT usPosY, USHORT usPosZ, unsigned char ucLastTouchLineIndex );
            static float _DecodeNpcSyncPosY( USHORT usPosY, unsigned char ucLastTouchLineIndex );

			void SetData( const KDYNAMIC_UNIT_NPC_SYNC& kXPT_UNIT_NPC_SYNC
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                , bool bSkipPosition = false
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                )
			{
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                if ( bSkipPosition == false )
                {
				    _DecodeNpcSyncPos( position, kXPT_UNIT_NPC_SYNC.m_usPosX, kXPT_UNIT_NPC_SYNC.m_usPosY, kXPT_UNIT_NPC_SYNC.m_usPosZ, kXPT_UNIT_NPC_SYNC.m_ucLastTouchLineIndex );
				    lastTouchLineIndex	= kXPT_UNIT_NPC_SYNC.m_ucLastTouchLineIndex;
                }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
				bIsRight			= kXPT_UNIT_NPC_SYNC.ExtractIsRight();
				bComeBackState		= kXPT_UNIT_NPC_SYNC.ExtractIsComeBackState();
				mindFlag			= (char)kXPT_UNIT_NPC_SYNC.ExtractMindFlag();
				m_usRandomTableIndex = kXPT_UNIT_NPC_SYNC.m_usRandomTableIndex;
			}

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			void SetData( const KXPT_UNIT_NPC_SYNC& kXPT_UNIT_NPC_SYNC )
//			{
//				nowState			= kXPT_UNIT_NPC_SYNC.nowState;
//
//				position.x			= kXPT_UNIT_NPC_SYNC.posX;
//				position.y			= kXPT_UNIT_NPC_SYNC.posY;
//				position.z			= kXPT_UNIT_NPC_SYNC.posZ;
//				lastTouchLineIndex	= kXPT_UNIT_NPC_SYNC.lastTouchLineIndex;
//
//				bIsRight			= kXPT_UNIT_NPC_SYNC.ExtractIsRight();
//				bStateChange		= kXPT_UNIT_NPC_SYNC.ExtractStateChange();
//				bComeBackState		= kXPT_UNIT_NPC_SYNC.ExtractIsComeBackState();
//				mindFlag			= (char)kXPT_UNIT_NPC_SYNC.ExtractMindFlag();
//
//#ifdef NEW_RANDOM_TABLE_TEST
//				m_usRandomTableIndex = kXPT_UNIT_NPC_SYNC.m_usRandomTableIndex;
//#else NEW_RANDOM_TABLE_TEST
////{{AFX
//				m_StateRandomSeed	= kXPT_UNIT_NPC_SYNC.m_StateRandomSeed;
//				m_StateRandomSeed2	= kXPT_UNIT_NPC_SYNC.m_StateRandomSeed2;				
////}}AFX
//#endif NEW_RANDOM_TABLE_TEST		
//				
//			}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		};

		struct NPCFrameData
		{
			StateParam		stateParam;
			UnitCondition	unitCondition;
			NPCSyncData		syncData;

			NPCFrameData& operator=( const NPCFrameData& data )
			{
				stateParam		= data.stateParam;
				unitCondition	= data.unitCondition;
				syncData		= data.syncData;

				return *this;
			}
			void Init()
			{
				stateParam.Init();
				unitCondition.Init();
				syncData.Init();
			}
		};

		struct ConditionTableData
		{
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
            bool        m_bNPCNonHostSimulatable;
            bool        m_bRequireTarget;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
			int			m_iIntervalID;
			float		m_fDistanceToTarget;
			float		m_fDistanceOverTarget;
			int			m_iAnimPlayCount;
			int			m_iRate;
			float		m_fSmallThenYSpeedFactor;
			float		m_fStateTimeOverFactor;
			float		m_fAnimTimerFactor;
			float		m_fMyHPLessThanPercentFactor;
			float		m_fMyHPMoreThanPercentFactor;
			float		m_fMyMPLessThanPercentFactor;
			float		m_fMyMPMoreThanPercentFactor;
#ifdef ADD_NPC_CONDITION_TABLE
			float		m_fTargetHPLessThanPercentFactor;
			float		m_fTargetHPMoreThanPercentFactor;
#endif // ADD_NPC_CONDITION_TABLE
#ifdef SEASON3_MONSTER_2010_12
			float		m_fMyMPMoreThanFactor;
#endif SEASON3_MONSTER_2010_12
			bool		m_bAttackSuccessFactor;
			bool		m_bSameLineWithTargetFactor;
			int			m_iFlagTrue;
			int			m_iFlagFalse;
//{{oasis:김상윤////2009-10-7////
			GROUP_AI_COMMAND	m_iGroupAICommandTrue[4]; 
			GROUP_AI_COMMAND	m_iGroupAICommandFalse[4];
//}}oasis:김상윤////2009-10-7////
			bool		m_bEscapeCheck;			
			bool		m_bTargetAboveMe;
			bool		m_bTargetBelowMe;
			int			m_iHaveTarget;	// -1: don't care, 0: false, 1: true
			float		m_fHigherThanPosY;
			int			m_iAliveUserUnitGeq;						
			int			m_iAliveUserUnitLeq;
			int			m_iUserUnitGeq;
			int			m_iUserUnitLeq;
			float 		m_fTimerElapsed[10];
            int         m_iGEKeyCode; // added by wonpok
			bool		m_bDirTarget; // 타켓이 왼쪽/오른쪽에 있는지에 대한 여부를 검사할지 설정
#ifdef ADD_NPC_CONDITION_TABLE
			bool		m_bViewTargetDir; // 타겟 방향을 보고 있는지 검사
#endif // ADD_NPC_CONDITION_TABLE

			bool		m_bRightTarget; // m_bDirTarget 이 TRUE일때 설정한다.


#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
			int			m_iCurrLineDepthHigherThan;
			bool		m_bCurrentLineHasLinkedLine;
			float		m_fTimeElapsedAfterLinkJump;
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST
#ifdef VELDER_BOSS_5
			//{{ oasis907 : 김상윤 [2010.8.1] // 
			bool		m_bFootOnLine;		
			//}} oasis907 : 김상윤 [2010.8.1] // 
#endif VELDER_BOSS_5
#ifdef VELDER_SECRET_DUNGEON
			bool		m_bRageMode;
#endif

#ifdef SUMMON_MONSTER_CARD_SYSTEM
			bool		m_bCheckAnotherTeam;				// 다른 팀인지 검사
#endif SUMMON_MONSTER_CARD_SYSTEM

			ConditionTableData()
			{
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                m_bNPCNonHostSimulatable = false;
                m_bRequireTarget = false;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
				m_iIntervalID					= -1;
				m_fDistanceToTarget				= -1.0f;
				m_fDistanceOverTarget			= -1.0f;
				m_iAnimPlayCount				= -1;
				m_iRate							= -1;
				m_fSmallThenYSpeedFactor		= -1.0f;
				m_fStateTimeOverFactor			= -1.0f;
				m_fAnimTimerFactor			= -1.0f;
				m_fMyHPLessThanPercentFactor	= -1.0f;
				m_fMyHPMoreThanPercentFactor	= -1.0f;
				m_fMyMPLessThanPercentFactor	= -1.0f;
				m_fMyMPMoreThanPercentFactor	= -1.0f;
#ifdef ADD_NPC_CONDITION_TABLE
				m_fTargetHPLessThanPercentFactor = -1.f;
				m_fTargetHPMoreThanPercentFactor = -1.f;
#endif // ADD_NPC_CONDITION_TABLE
#ifdef SEASON3_MONSTER_2010_12
				m_fMyMPMoreThanFactor			= -1.0f;
#endif SEASON3_MONSTER_2010_12
				m_bAttackSuccessFactor			= false;
				m_bSameLineWithTargetFactor		= false;
				m_iFlagTrue						= -1;
				m_iFlagFalse					= -1;
//{{oasis:김상윤////2009-10-7////
				for( int i=0; i<4; i++ )
				{
					m_iGroupAICommandTrue[i] = GAI_CMD_OFF;
					m_iGroupAICommandFalse[i] = GAI_CMD_OFF;
				}
//}}oasis:김상윤////2009-10-7////
				m_bEscapeCheck					= false;
				m_bTargetAboveMe				= false;
				m_bTargetBelowMe				= false;
				m_iHaveTarget					= false;
				m_fHigherThanPosY				= -1.0f;

				m_iAliveUserUnitGeq		= -1;
				m_iAliveUserUnitLeq		= -1;
				m_iUserUnitGeq				= -1;
				m_iUserUnitLeq				= -1;

                m_iGEKeyCode                    = -1;
				m_bDirTarget					= false;
				m_bRightTarget					= false;
#ifdef ADD_NPC_CONDITION_TABLE
				m_bViewTargetDir				= false; // 타겟 방향을 보고 있는지 검사
#endif // ADD_NPC_CONDITION_TABLE

				for( int i=0; i<10; i++ )
				{
					m_fTimerElapsed[i] = -1.f;
				}

#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
				m_iCurrLineDepthHigherThan = -1;			
				m_bCurrentLineHasLinkedLine = false;
				m_fTimeElapsedAfterLinkJump = 0.f;
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST
#ifdef VELDER_BOSS_5
				//{{ oasis907 : 김상윤 [2010.8.1] // 
				m_bFootOnLine					= false;
				//}} oasis907 : 김상윤 [2010.8.1] // 
#endif VELDER_BOSS_5
#ifdef VELDER_SECRET_DUNGEON
				m_bRageMode						= false;
#endif

#ifdef SUMMON_MONSTER_CARD_SYSTEM
				m_bCheckAnotherTeam				= false;		// 다른 팀인지 검사
#endif SUMMON_MONSTER_CARD_SYSTEM

			}

//#ifdef BOOST_SINGLETON_POOL_TEST
//			void* operator new(size_t s);
//			void operator delete(void *p);
//#endif BOOST_SINGLETON_POOL_TEST



		};


//#ifdef TRANSFORMER_TEST
//
//
//		struct KeyCombinationData
//		{
//			KEYBOARD_EVENT_TYPE eType;
//			bool bPressed;
//
//			KeyCombinationData()
//				: eType( KET_NONE )
//				, bPressed( false )
//			{
//			}
//
//		};
//
//		struct KeyEventProcessData
//		{
//			std::vector< KeyCombinationData > m_vecKeyCombinationData;
//			char m_cNextStateID;
//			char m_cIsRight; // -1 : don't change direction, 0 : left, 1 : right
//			D3DXVECTOR3 m_vDisplace;
//			
//			KeyEventProcessData() 
//				: m_cNextStateID( GUSI_NONE )
//				, m_cIsRight( -1 )
//				, m_vDisplace( 0, 0, 0 )
//			{
//			}
//
//		};
//
//#endif TRANSFORMER_TEST
//

		struct NPCEventProcessData
		{
			STATE_CHANGE_TYPE	m_StateChangeType;
            char                m_NextStateID;
			string				m_strCustomCondition;
			ConditionTableData*	m_pConditionTableData;

			NPCEventProcessData()
			{
				m_StateChangeType		= SCT_NO_CHANGE;
                m_NextStateID			= GUSI_NONE;
				m_strCustomCondition    = "";
				m_pConditionTableData	= NULL;
			}

			~NPCEventProcessData()
			{
				SAFE_DELETE( m_pConditionTableData );
			}

//#ifdef BOOST_SINGLETON_POOL_TEST
//			void* operator new(size_t s);
//			void operator delete(void *p);
//#endif BOOST_SINGLETON_POOL_TEST

		};


		struct AnimSpeed
		{
			float m_fStartTime;
			float m_fEndTime;
			float m_fAnimSpeed;
		};

//-------------------------------------------------------------------------------------------------------
#ifdef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

        enum ESoundSetType
        {
            SOUND_SET_DEFUALT,
#ifdef      ARA_CHARACTER_BASE
            SOUND_SET0_NORMAL,
            SOUND_SET0_HYPER,
            SOUND_SET0_BERSERK,
            SOUND_SET1_NORMAL,
            SOUND_SET1_HYPER,
            SOUND_SET1_BERSERK,
            SOUND_SET2_NORMAL,
            SOUND_SET2_HYPER,
            SOUND_SET2_BERSERK,
#endif      ARA_CHARACTER_BASE
            SOUND_SET_NUMS
        };

//-------------------------------------------------------------------------------------------------------

        struct TalkBox
        {
            int         m_iRate;
            int         m_iMessage;
            float       m_fDelayTime;

            TalkBox()
                : m_iRate( 0 )
                , m_iMessage( STR_ID_EMPTY )
                , m_fDelayTime( 0.f )
            {
            }
        };

//-------------------------------------------------------------------------------------------------------
		struct ParticleEffectData
		{
			bool		m_bUse;
			bool		m_bMajor;
			float		m_fTime;
			wstring		m_Name;
			bool		m_bWeapon;
			wstring		m_Pos;
			bool		m_StateEndDelete;
			bool		m_bTrace;
			D3DXVECTOR3 m_vOffset;
			bool		m_bApplyUnitRotation;
			D3DXVECTOR3	m_vRotation;
            D3DXVECTOR3 m_vScale;
			bool		m_bUnitScale;
			
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeq;

			ParticleEffectData()
			{
				m_bUse				= false;
				m_bMajor			= false;
				m_fTime				= 0.0f;
				m_bWeapon			= false;
				m_StateEndDelete	= false;
				m_hSeq				= INVALID_PARTICLE_SEQUENCE_HANDLE;
				m_bTrace			= true;
				m_vOffset			= D3DXVECTOR3(0, 0, 0);
				m_bApplyUnitRotation = false;
				m_vRotation			= D3DXVECTOR3(0, 0, 0);
                m_vScale            = D3DXVECTOR3(1, 1, 1);
				m_bUnitScale		= false;
			}

            CKTDGParticleSystem::CParticleEventSequence*    CreateInstanceSequence( CX2GUNPC* pGUNPC, const D3DXVECTOR3& vPos );
            CKTDGParticleSystem::CParticleEventSequence*    GetInstanceSequence();
            void        DestroyInstanceSequence();
            static void Destroy( ParticleEffectData* pParticleEffectData );

		};

//-------------------------------------------------------------------------------------------------------
		struct MeshPlayerEffectData
		{
			bool		m_bUse;
			bool		m_bMajor;
			float		m_fTime;
			wstring		m_Name;
			bool		m_bWeapon;
			wstring		m_Pos;
			bool		m_StateEndDelete;
			bool		m_Trace;

			CKTDGXMeshPlayer::CXMeshInstanceHandle m_hMeshInst;

			MeshPlayerEffectData()
			{
				m_bUse				= false;
				m_bMajor			= false;
				m_fTime				= 0.0f;
				m_bWeapon			= false;
				m_StateEndDelete	= false;
				m_Trace				= false;
				m_hMeshInst			= INVALID_MESH_INSTANCE_HANDLE;
			}
            CKTDGXMeshPlayer::CXMeshInstance*           CreateMeshInstance( CX2GUNPC* pGUNPC
                , const D3DXVECTOR3& vPos, const D3DXVECTOR3& vRotateDegree );
            CKTDGXMeshPlayer::CXMeshInstance*           GetMeshInstance();
            void        DestroyMeshInstance();                                       
            static void Destroy( MeshPlayerEffectData* pMeshPlayerEffectData );

		};

//-------------------------------------------------------------------------------------------------------
		struct CameraCrashData
		{
			float		m_fTime;
			float		m_fGap;
			float		m_CrashTime;
			bool		m_bOnlyFocus;

			CameraCrashData()
			{
				m_fTime			= 0.0f;
				m_fGap			= 0.0f;
				m_CrashTime		= 0.0f;
				m_bOnlyFocus	= false;
			}

            bool        operator < ( const CameraCrashData& In_ ) const { return m_fTime < In_.m_fTime; }
		};

//-------------------------------------------------------------------------------------------------------

#ifdef  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

        struct  StateInsertBuffFactor
        {
            UINT        m_uiBuffFactorID;
            float       m_fStartAnimationTime;

            StateInsertBuffFactor()
                : m_uiBuffFactorID( 0 )
                , m_fStartAnimationTime( 0.f )
            {
            }

            StateInsertBuffFactor( UINT uiBuffFactorID, float fStartAnimationTime )
                : m_uiBuffFactorID( uiBuffFactorID )
                , m_fStartAnimationTime( fStartAnimationTime )
            {
            }

            bool     operator < ( const StateInsertBuffFactor& rhs_ ) const { return m_fStartAnimationTime < rhs_.m_fStartAnimationTime; }
        };

#endif  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
//-------------------------------------------------------------------------------------------------------


#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION

        struct  StateData
        {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            enum    EFlag
            {
                // 여기서부터는 FrameMove, EventProcess, DamageReact 등 state 중간에 접근할 수 있는 flag 를 기록한다.

                FLAG_REACTION_STATE,
                FLAG_WAIT_STATE,
                FLAG_HINT_NO_PERFRAME_EVENT,
                FLAG_HINT_DAMAGED_EVENT,
                FLAG_ALLOW_DIR_CHANGE,
                FLAG_NOT_USE_LAND_CONNECT,
#ifdef  ADD_CAN_PASS_ALLY_UNIT
                FLAG_CAN_PASS_ALLY_UNIT,
#endif  ADD_CAN_PASS_ALLY_UNIT
                FLAG_APPLY_ANIM_MOVE,
#ifdef  HP_BASED_MONSTER_RAGE
                FLAG_CANNOT_RAGE,
#endif  HP_BASED_MONSTER_RAGE
                FLAG_ATTACK_FOLLOW_HITTER,
#ifdef  SEASON3_MONSTER_2010_12
                FLAG_ATTACK_FOLLOW_HITTER_POS_FRONT,
#endif  SEASON3_MONSTER_2010_12
                FLAG_GUARD_DEFENCE_FRONT,
                FLAG_GUARD_DEFENCE_BACK,
                FLAG_DAMAGE_DOWN,
                FLAG_DELETE_EFFECT_SET_ON_STATE_END,
                FLAG_DELETE_EFFECT_SET_ON_DAMAGE_REACT,
                FLAG_DELETE_EFFECT_SET_ON_DIE,
#ifdef  DELETE_EFFECTSET_ON_CUSTOM_STATE
                FLAG_DELETE_EFFECT_SET_ON_CUSTOM_STATE,
#endif  DELETE_EFFECTSET_ON_CUSTOM_STATE
#ifdef  SERV_ELESIS_SECOND_CLASS_CHANGE
                FLAG_CUSTOM_STATE_DELETE_EFFECT_SET_ON_STATE_END,
#endif  SERV_ELESIS_SECOND_CLASS_CHANGE
                FLAG_HYPER_EFFECT_SET,
                FLAG_SWAP_UNIT_WIDTH_HEIGHT,
                FLAG_DYING_END,
                FLAG_ESCAPE_END,
                FLAG_USE_MIND_FLAG,
                FLAG_FALL_DOWN,
#ifdef  AI_FLY
                FLAG_ENABLE_HEIGHT_FIX,
#endif  AI_FLY
                FLAG_NEVER_DIE_THIS_STATE,
#ifdef MODIFY_DUNGEON_STAGING
                FLAG_FOCUS_CAMERA_AT_CURRENT_STATE,
#endif  MODIFY_DUNGEON_STAGING
#ifdef FIELD_BOSS_RAID
				FLAG_INVALID_LAST_TOUCH_INDEX,
#endif // FIELD_BOSS_RAID

                // 여기서부터는 StateStart, StateEnd 에서만 접근하는 flag 를 기록한다.

                FLAG_IMMEDIATE_PACKET_SEND,
                FLAG_VIEW_TARGET,
                FLAG_SET_DIRECTION,
                FLAG_SET_DIRECTION_VALUE,
                FLAG_SET_MIND_FLAG,
                FLAG_SET_LAND_CONNECT_VALUE,
                FLAG_SET_CAN_PUSH_UNIT_VALUE,
                FLAG_SET_CAN_PASS_UNIT_VALUE,
                FLAG_SET_SUPER_ARMOR_VALUE,
                FLAG_SET_SUPER_ARMOR_NOT_RED_VALUE,
                FLAG_APPLY_MOVEBONE,
                FLAG_SET_DEFENCE,
                FLAG_TRANSITION,
#ifdef  DELETE_EFFECTSET_ON_CUSTOM_STATE
                FLAG_CUSTOM_STATE_DELETE_EFFECT_SET,
#endif  DELETE_EFFECTSET_ON_CUSTOM_STATE
                FLAG_SHOW_STICK_SHAKE,
                FLAG_SET_NEVER_MOVE_VALUE,
                FLAG_SET_SHOW,
                FLAG_SET_SHOW_VALUE,
                FLAG_SET_SHOW_NAME,
                FLAG_SET_SHOW_NAME_VALUE,
#ifdef  SKILL_30_TEST
                FLAG_SET_INVISIBLE_TO_ENEMY,
                FLAG_SET_INVISIBLE_TO_ENEMY_VALUE,
#endif  SKILL_30_TEST
                FLAG_DYING_END_IMMEDIATE,
#ifdef  AI_FLY
                FLAG_SET_FLY_AI,
                FLAG_SET_FLY_AI_VALUE,
#endif  AI_FLY
#ifdef  DAMAGE_ZERO
                FLAG_SET_ZERO_DAMAGE,
#endif  DAMAGE_ZERO
#ifdef  SEASON3_MONSTER_2011_7
                FLAG_SET_ABSORB_MAGIC_VALUE,
#endif  SEASON3_MONSTER_2011_7
#ifdef MODIFY_DUNGEON_STAGING
                FLAG_STOP_AT_CURRENT_STATE,
#endif  MODIFY_DUNGEON_STAGING
                FLAG_FLIP_DIR_END,
                FLAG_SET_COOL_TIME,
#ifdef  SEASON3_MONSTER_2010_12
                FLAG_SET_REFLECT_MAGIC,
#endif  SEASON3_MONSTER_2010_12
                FLAG_SET_G_ACCEL,
                FLAG_SET_DOWN_ACCEL,
                FLAG_SET_SPEED_X,
                FLAG_SET_SPEED_Y,
#ifdef  SEASON3_MONSTER_2010_12
                FLAG_SET_RENDER_PARAM,
#endif  SEASON3_MONSTER_2010_12
                FLAG_SET_ANIM_SPEED,
                FLAG_STOP_LOOP_SOUND,
#ifdef  ARA_CHARACTER_BASE
                FLAG_SOUND_SET0,
                FLAG_SOUND_SET0_USE_HYPER_SOUND,
                FLAG_SOUND_SET0_USE_BERSERK_SOUND,
                FLAG_SOUND_SET1,
                FLAG_SOUND_SET1_USE_HYPER_SOUND,
                FLAG_SOUND_SET1_USE_BERSERK_SOUND,
                FLAG_SOUND_SET2,
                FLAG_SOUND_SET2_USE_HYPER_SOUND,
                FLAG_SOUND_SET2_USE_BERSERK_SOUND,
#endif  ARA_CHARACTER_BASE
				FLAG_MAX
            };
            std::bitset<FLAG_MAX>       m_bitsetFlags;
            std::vector<float>          m_vecfHintAnimEventTimer;
            std::vector<float>          m_vecfHintVisualAnimEventTimer;
            std::vector<float>          m_vecfHintAnimTimer;
            std::vector<float>          m_vecfHintVisualAnimTimer;
            std::vector<float>          m_vecfHintEventTimer;

            float                       m_fSetCoolTimeValue;
            char                        m_cSetMindFlagValue;
            int                         m_iSetFollowHitterValue;
#ifdef  AISHA_SECOND_CLASS_CHANGE
            D3DXVECTOR2                 m_v2IgnoreLineTime;
#endif  AISHA_SECOND_CLASS_CHANGE
            float                       m_fSlashTraceTipWide;
            D3DXVECTOR3                 m_vDefenceTime;
            float                       m_fSetGuardDefenceValue;
            D3DXVECTOR2                 m_v2SetRevengeTimeValue;
            D3DXVECTOR2                 m_v2SetReflectMagicValue;
            std::map< int, D3DXVECTOR3 > m_mapAddSlashTrace;
            std::wstring                m_wstrAnimName;
            CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE m_ePlayType;

#ifdef	CHUNG_FIRST_CLASS_CHANGE
            CX2DamageManager::HITTED_TYPE m_eHittedTypeAtState;	// 스테이트 별 HittedType 지정, 이것도 일단은 청만 사용 하는 걸로 알고 있긴 한데...
#endif  CHUNG_FIRST_CLASS_CHANGE
#ifdef  LINKED_LINEMAP_JUMP_MONSTER_TEST
            float                       m_fSetLinkJumpTimeValue;
            D3DXVECTOR3                 m_vSetLinkJumpAccelerationValue;
#endif  LINKED_LINEMAP_JUMP_MONSTER_TEST
            D3DXVECTOR3                 m_vAddPos;
            D3DXVECTOR3                 m_vAddRotateRadian;
            int                         m_iGrapShakeCount;
            std::vector<TIME_SPEED>     m_vecSpeedFactor;
            float                       m_fSetGAccelValue;
            float                       m_fSetDownAccelValue;
            D3DXVECTOR2                 m_v2SetSpeedValue;
            D3DXVECTOR2                 m_v2SetPassiveSpeedValue;
            float                       m_fDyingSpeed;
            D3DXVECTOR2                 m_v2AfterImageTime;
#ifdef  SECRET_VELDER
            D3DXVECTOR2                 m_v2InvisibleTime;
#endif  SECRET_VELDER
            std::vector<float>          m_vecfEventIntervalTime;
            std::vector<KAttackTimeProjSeqName>
                                        m_vecAttackTime;
		    std::vector<D3DXVECTOR3>	m_vecStopAllList;
		    std::vector<D3DXVECTOR2>	m_vecStopOtherList;
		    std::vector<D3DXVECTOR2>	m_vecStopMyList;
		    std::vector<D3DXVECTOR3>	m_vecStop2AllList;
		    std::vector<D3DXVECTOR2>	m_vecStop2OtherList;
		    std::vector<D3DXVECTOR2>	m_vecStop2MyList;
            std::vector<std::wstring>   m_vecwstrEnableAttackBox;
            std::vector<std::wstring>   m_vecwstrDisableAttackBox;
            std::vector<std::wstring>   m_vecwstrEnableCollisionBox;
            std::vector<std::wstring>   m_vecwstrDisableCollisionBox;
            std::vector<EffectSetToPlay> m_vecEffectSetToPlay;
            float                       m_fEffectSetLifeTime;
#ifdef      ARA_CHARACTER_BASE
            int                         m_iSoundSet0Rate;
            int                         m_iSoundSet1Rate;
            int                         m_iSoundSet2Rate;
#endif      ARA_CHARACTER_BASE
            std::vector<SoundPlayData>  m_avecSoundPlayData[SOUND_SET_NUMS];
            std::vector<SoundPlayData>  m_vecSetSoundPlayLoopValue;

            std::vector<TalkBox>        m_vecTalkBox;
            std::vector<int>            m_veciEnableWeapon;
            std::vector<int>            m_veciDisableWeapon;

#ifdef  SEASON3_MONSTER_2010_12
            CKTDGXRenderer::RENDER_TYPE m_eSetRenderParamValue;
#endif  SEASON3_MONSTER_2010_12
            float                       m_fCommonDamageChangeTime;

    		std::vector<BUFF_TEMPLET_ID> m_vecImmunityAtThisState;	/// 각각의 스테이트에서만 Immune 되어야할 ExtraDamage 지정.. 근데 여기에 놓고선 유저한테만 적용 되도록 해놨군... 수정!!해야함
#ifdef  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
            std::vector<StateInsertBuffFactor>
                                        m_vecStateInsertBuffFactor;
#endif  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

		    std::vector<AnimSpeed>		        m_vecAnimSpeed;
            std::vector<ParticleEffectData>     m_vecParticleSeq;       // m_ParticleEffData
            std::vector<MeshPlayerEffectData>   m_vecMeshPlayer;        // m_MeshPlayerEffData
            std::vector<CameraCrashData>        m_vecCameraCrash;      // m_CameraCrashData

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef  SUPER_ARMOR_TIME
            std::vector<D3DXVECTOR2>    m_vecSuperArmorTime;
#ifdef  SECRET_VELDER
            float                       m_fSetGlobalSuperArmorTimeValue;
#endif  SECRET_VELDER
#endif  SUPER_ARMOR_TIME

            D3DXVECTOR2                 m_v2SetInvincibleTimeValue;
            float                       m_fSetForceInvincibleTimeValue;
            float                       m_fSetShowInvincibleTimeValue;


            float                       m_fStateCoolTime;
#ifdef  SERV_SECRET_HELL
            float                       m_fInitializeStateElapsedTime;
#endif  SERV_SECRET_HELL

            float                       m_fSetAnimWaitTimeValue;
            float                       m_fSetAnimSpeedValue;

			string			            m_strLua_StateStart;
			string			            m_strLua_FrameMove;
			string			            m_strLua_CameraMove;
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			string			            m_strLua_EventProcess;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			string			            m_strLua_StateEnd;

			std::vector< NPCEventProcessData> 
                                        m_vecNPCEventProcessData;
//#ifdef TRANSFORMER_TEST
//			char						m_cKeyEventFailNextStateID;
//			std::vector< KeyEventProcessData > m_vecKeyEventProcessData;
//#endif TRANSFORMER_TEST

            char                        m_cEquivalentReactionState;

            StateData()
                :
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                  m_bitsetFlags(),
                  m_vecfHintAnimEventTimer(),
                  m_vecfHintVisualAnimEventTimer(),
                  m_vecfHintAnimTimer(),
                  m_vecfHintVisualAnimTimer(),
                  m_vecfHintEventTimer(),
                  m_fSetCoolTimeValue( 0.f ),
                  m_cSetMindFlagValue(0),
#ifdef  AISHA_SECOND_CLASS_CHANGE
                  m_v2IgnoreLineTime(0,0),
#endif  AISHA_SECOND_CLASS_CHANGE
                  m_fSlashTraceTipWide( 4.0f ),
                  m_iSetFollowHitterValue( 0 ),
                  m_vDefenceTime(-1.f,-1.f,0.f),
                  m_fSetGuardDefenceValue( 0.f ),
                  m_v2SetRevengeTimeValue( -1.f, -1.f ),
                  m_v2SetReflectMagicValue( 0.f, 0.f ),
                  m_wstrAnimName(),
                  m_ePlayType( CKTDGXSkinAnim::XAP_ONE_WAIT ),
#ifdef	CHUNG_FIRST_CLASS_CHANGE
                  m_eHittedTypeAtState( CX2DamageManager::HTD_NO_SOUND ),
#endif  CHUNG_FIRST_CLASS_CHANGE
#ifdef  LINKED_LINEMAP_JUMP_MONSTER_TEST
                  m_fSetLinkJumpTimeValue( 1.f ),
                  m_vSetLinkJumpAccelerationValue( 0.f, -3000.f, 0.f ),
#endif  LINKED_LINEMAP_JUMP_MONSTER_TEST
                  m_vAddPos( 0, 0, 0 ),
                  m_vAddRotateRadian( 0, 0, 0 ),
                  m_iGrapShakeCount( 0 ),
                  m_vecSpeedFactor(),
                  m_fSetGAccelValue( 0.f ),
                  m_fSetDownAccelValue( 0.f ),
                  m_v2SetSpeedValue( 0, 0 ),
                  m_v2SetPassiveSpeedValue( -1.f, -1.f ),
                  m_fDyingSpeed( 0.5f ),
                  m_v2AfterImageTime( -1.f, -1.f ),
#ifdef  SECRET_VELDER
                  m_v2InvisibleTime( -1.f, -1.f ),
#endif  SECRET_VELDER
                  m_vecfEventIntervalTime(),
                  m_vecAttackTime(),
                  m_vecStopAllList(),
                  m_vecStopOtherList(),
                  m_vecStopMyList(),
                  m_vecStop2AllList(),
                  m_vecStop2OtherList(),
                  m_vecStop2MyList(),
                  m_vecwstrEnableAttackBox(),
                  m_vecwstrDisableAttackBox(),
                  m_vecwstrEnableCollisionBox(),
                  m_vecwstrDisableCollisionBox(),
                  m_vecEffectSetToPlay(),
                  m_fEffectSetLifeTime( -1.f ),
                  m_avecSoundPlayData(),
                  m_vecSetSoundPlayLoopValue(),
#ifdef      ARA_CHARACTER_BASE
                  m_iSoundSet0Rate(0),
                  m_iSoundSet1Rate(0),
                  m_iSoundSet2Rate(0),
#endif      ARA_CHARACTER_BASE
                  m_vecTalkBox(),
                  m_veciEnableWeapon(),
                  m_veciDisableWeapon(),
#ifdef  SEASON3_MONSTER_2010_12
                  m_eSetRenderParamValue( CKTDGXRenderer::RT_CARTOON_BLACK_EDGE ),
#endif  SEASON3_MONSTER_2010_12
                  m_fCommonDamageChangeTime( -1.f ),
                  m_vecImmunityAtThisState(),
#ifdef  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
                  m_vecStateInsertBuffFactor(),
#endif  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
                 m_vecAnimSpeed(),
                 m_vecParticleSeq(),
                 m_vecMeshPlayer(),
                m_vecCameraCrash(),
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef  SUPER_ARMOR_TIME
#ifdef  SECRET_VELDER
                m_fSetGlobalSuperArmorTimeValue( -1.f )
#endif  SECRET_VELDER
#endif  SUPER_ARMOR_TIME
                , m_v2SetInvincibleTimeValue( -1.f, -1.f )
                , m_fSetForceInvincibleTimeValue( -1.f )
                , m_fSetShowInvincibleTimeValue( -1.f )
                , m_fStateCoolTime( 0.f )
#ifdef  SERV_SECRET_HELL
                , m_fInitializeStateElapsedTime( 0.f )
#endif  SERV_SECRET_HELL
                , m_fSetAnimWaitTimeValue( 0.f )
                , m_fSetAnimSpeedValue( 1.f )
//#ifdef  TRANSFORMER_TEST
//                , m_cKeyEventFailNextStateID( -1 )
//#endif  TRANSFORMER_TEST
                , m_cEquivalentReactionState( 0 )
            {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                StateData& nowStateData = *this;

                NOWSTATE_SET_FLAG( HINT_NO_PERFRAME_EVENT, true );

                NOWSTATE_SET_FLAG( ALLOW_DIR_CHANGE, true );
                NOWSTATE_SET_FLAG( SET_LAND_CONNECT_VALUE, true );
                NOWSTATE_SET_FLAG( SET_CAN_PUSH_UNIT_VALUE, true );
#ifdef  ADD_CAN_PASS_ALLY_UNIT
                NOWSTATE_SET_FLAG( CAN_PASS_ALLY_UNIT, true );
#endif  ADD_CAN_PASS_ALLY_UNIT
                NOWSTATE_SET_FLAG( APPLY_ANIM_MOVE, true );
                NOWSTATE_SET_FLAG( APPLY_MOVEBONE, true );
                NOWSTATE_SET_FLAG( GUARD_DEFENCE_FRONT, true );
                NOWSTATE_SET_FLAG( GUARD_DEFENCE_BACK, true );
                NOWSTATE_SET_FLAG( DAMAGE_DOWN, true );
                NOWSTATE_SET_FLAG( TRANSITION, true );
                NOWSTATE_SET_FLAG( USE_MIND_FLAG, true );
                NOWSTATE_SET_FLAG( FALL_DOWN, true );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            }
        };

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        struct  NPCEventProcessRuntime
        {
            int                     m_iLua_CustomConditionRefID;

            NPCEventProcessRuntime()
                : m_iLua_CustomConditionRefID( LUA_REFNIL )
            {
            }
        };
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

        struct  StateRuntime
        {
            float                   m_fStateElapsedTime;
            int                     m_iLua_StateStartRefID;
            int                     m_iLua_FrameMoveRefID;
            int                     m_iLua_CameraMoveRefID;
            int                     m_iLua_StateEndRefID;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			std::vector< NPCEventProcessRuntime > 
                                        m_vecNPCEventProcessRuntime;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            int                     m_iLua_EventProcessRefID;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

            StateRuntime()
                : m_fStateElapsedTime( 0.f )
                , m_iLua_StateStartRefID( LUA_REFNIL )
                , m_iLua_FrameMoveRefID( LUA_REFNIL )
                , m_iLua_CameraMoveRefID( LUA_REFNIL )
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                , m_vecNPCEventProcessRuntime()
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                , m_iLua_EventProcessRefID( LUA_REFNIL )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                , m_iLua_StateEndRefID( LUA_REFNIL )
            {
            }
        };//struct  StateRuntime

#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION

		struct NPCStateData
		{
#ifdef ADD_KPROTECTEDTYPE_VALUE
			KProtectedType<int>	stateID;
#else
			int				stateID;
#endif
			

			wstring			Lua_StateStart;
			wstring			Lua_FrameMove;
			wstring			Lua_CameraMove;
			wstring			Lua_EventProcess;
			wstring			Lua_StateEnd;
            

			float			fStateCoolTime;
			float			fStateElapsedTime;

			std::vector< NPCEventProcessData* > m_vecpNPCEventProcessData;

//#ifdef TRANSFORMER_TEST
//			char								m_cKeyEventFailNextStateID;
//			std::vector< KeyEventProcessData > m_vecKeyEventProcessData;
//#endif TRANSFORMER_TEST



		public:
			void RemoveAllNPCEventProcessData()
			{
				KTDXPROFILE();

				for( int i = 0; i < (int)m_vecpNPCEventProcessData.size(); i++ )
				{
					NPCEventProcessData* pNPCEventProcessData = m_vecpNPCEventProcessData[i];
					SAFE_DELETE( pNPCEventProcessData );
				}

				m_vecpNPCEventProcessData.resize(0);
			}


//#ifdef TRANSFORMER_TEST
//			void RemoveKeyNPCEventProcessData()
//			{
//				KTDXPROFILE();
//				m_vecKeyEventProcessData.resize(0);
//			}
//#endif TRANSFORMER_TEST


			~NPCStateData() 
			{
				RemoveAllNPCEventProcessData();
			}

			NPCStateData()
			{
				Init();
			}

			virtual void Init()
			{
				stateID			= GUSI_NONE;

				Lua_StateStart		= L"";
				Lua_FrameMove		= L"";
				Lua_CameraMove		= L"";
				Lua_EventProcess	= L"";
				Lua_StateEnd		= L"";

				fStateCoolTime		= 0.0f;
				fStateElapsedTime	= 0.0f;

				m_vecpNPCEventProcessData.resize( 0 );
//#ifdef TRANSFORMER_TEST
//				m_cKeyEventFailNextStateID = -1;
//				m_vecKeyEventProcessData.resize( 0 );
//#endif TRANSFORMER_TEST
			}



			// 주의!! 이 copy constructor, assignment operator 삭제하면 안됨
			NPCStateData( const NPCStateData& t ) 
			{ 
				*this = t; 
			}    

			NPCStateData& operator=( const NPCStateData& data )
			{
				stateID				= data.stateID;

				Lua_StateStart		= data.Lua_StateStart;
				Lua_FrameMove		= data.Lua_FrameMove;
				Lua_CameraMove		= data.Lua_CameraMove;
				Lua_EventProcess	= data.Lua_EventProcess;
				Lua_StateEnd		= data.Lua_StateEnd;

				fStateCoolTime		= data.fStateCoolTime;
				fStateElapsedTime	= data.fStateElapsedTime;


				//m_vecpNPCEventProcessData // 주의!! 기존 메모리 해제하고 새로 메모리를 할당하고 복사해줘야 하지만, 코드 다른 부분에서 매 프레임 삭제해주고 새로 로딩하기 때문에 여기서 굳이 복사하도록 하지 않는다


//#ifdef TRANSFORMER_TEST
//				m_cKeyEventFailNextStateID = data.m_cKeyEventFailNextStateID;
//				m_vecKeyEventProcessData = data.m_vecKeyEventProcessData;
//#endif TRANSFORMER_TEST

				return *this;
			}


		};

#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

		struct AttachedParticle
		{
			wstring		m_wstrBoneName;
			bool		m_bIsMajorParticle;
			D3DXVECTOR3 m_vOffsetPos;
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeq;

			AttachedParticle()
                : m_wstrBoneName()
                , m_bIsMajorParticle( true )
                , m_vOffsetPos( 0, 0, 0 )
                , m_hSeq( INVALID_PARTICLE_SEQUENCE_HANDLE )  
            {
            }
            ~AttachedParticle();
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            bool    CreateSequenceHandle( CX2GUNPC* pNPC );
            CKTDGParticleSystem::CParticleEventSequence*    GetInstanceSequence();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		};
                        
		struct Invisible
		{
#ifdef VELDER_BOSS_5
			bool	m_bCloakingUnit;
			bool	m_bCloaking;
			bool	m_bCloaked;
#endif VELDER_BOSS_5
			bool	m_bInvisible;
			float	m_fMinAlpha;		// alpha 값이 min~max 사이의 값으로 변하는데
			float	m_fMaxAlpha;

			float	m_fAlphaLowerBound;	// alpha 값이 lower bound보다 작으면 lower bound로 제한 된다.
			
			float	m_fNowAlpha;
			float	m_fSpeed;
			bool	m_bAdd;
#ifdef SECRET_VELDER
			bool	m_bAlwaysInvisible;
#endif

			Invisible()
			{
#ifdef VELDER_BOSS_5
				m_bCloakingUnit = false;
				m_bCloaking	= false;
				m_bCloaked = false;
#endif VELDER_BOSS_5
				m_bInvisible	= false;
				m_fMinAlpha		= 0.0f;
				m_fMaxAlpha		= 1.0f;
				m_fNowAlpha		= 0.0f;
				m_fSpeed		= 0.0f;
				m_bAdd			= false;

				m_fAlphaLowerBound = 0.f;

#ifdef SECRET_VELDER
				m_bAlwaysInvisible = true;
#endif
			}
		};

//{{oasis:김상윤////2009-10-7////
		struct GroupAIData
		{
			GROUP_AI_COMMAND m_eGroupAICommand;

			CX2GameUnitoPtr	m_optrGroupAiTargetGameUnit;
			//UidType GroupAITargetUid;
			D3DXVECTOR3 m_vGroupAITargetPos;
			bool m_bIsReadySelfCommandTransit;
			bool m_bIsGroundTarget;

			GroupAIData()
			{
				m_bIsReadySelfCommandTransit = false;
				m_eGroupAICommand = GAI_CMD_OFF;
				m_bIsGroundTarget = false;
				m_optrGroupAiTargetGameUnit.Reset();
			}
		};
//}}oasis:김상윤////2009-10-7////


#ifdef MONSTER_REFLECT_DAMAGE_DATA
		//{{ oasis907 : 김상윤 [2010.4.19] // 
		struct ConditionReflectDamage
		{
			float m_fMyHPLessThanPercentFactor;
			bool m_bOnPhysicalDamage;
			bool m_bOnMagicalDamage;

			ConditionReflectDamage()
			{
				m_fMyHPLessThanPercentFactor = -1.0f;
				m_bOnPhysicalDamage = false;
				m_bOnMagicalDamage = false;
			}
		};
		//}}
#endif MONSTER_REFLECT_DAMAGE_DATA

#ifdef FIELD_BOSS_RAID // 타겟 유닛의 본 위치 따라다니기
		struct FollowTargetUnitBone
		{
			UidType m_TargetUnitUID;			// 타겟 유닛 UID
			wstring m_TargetBoneName;			// 따라 갈 본 이름
			bool	m_bUseFollowTargetUnitBone;	// 활성화 여부
			bool	m_bIsUser;					// 타겟이 유저인지 / NPC인지 구분

			FollowTargetUnitBone():
			m_TargetUnitUID(0),
			m_bUseFollowTargetUnitBone(false)
			{}
		};
#endif // FIELD_BOSS_RAID

//{{ Public Variable End

//{{ Private Function Start
	private:

//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
		CX2GUNPC( int unitIndex, int teamNum, int uid, const CX2UnitManager::NPCUnitInfo* pNPCUnitInfo,
			int eAIType, CX2GameUnit* pOwnerGameUnit_ = NULL, float fUnitScale = -1.f );
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//		CX2GUNPC( int unitIndex, int teamNum, const WCHAR* pScriptFileName, int uid, const CX2UnitManager::NPCUnitTemplet* pNPCTemplet, 
//			int eAIType, CX2GameUnit* pOwnerGameUnit_ = NULL, float fUnitScale = -1.f );
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING

		// 필드 NPC 생성용
		CX2GUNPC( const CNpcCreatedInfoPtr NpcCreatedInfoPtr_, const CX2UnitManager::NPCUnitInfo* pNPCUnitInfo );
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
			//, lua_State*	pScriptLuaState
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
		//);

		void CommonConstructor();

		virtual ~CX2GUNPC(void);
//{{ Private Function End

		//void Initialize()
//{{ Public Function Start
	public: 

//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
		static CX2GUNPC* CreateGUNPC( int unitIndex, int teamNum, int uid, const CX2UnitManager::NPCUnitInfo* pNPCUnitInfo,
			int eAIType, CX2GameUnit* pOwnerGameUnit_ = NULL, float fUnitScale = -1.f )
		{
			return new CX2GUNPC( unitIndex, teamNum, uid, pNPCUnitInfo, eAIType, pOwnerGameUnit_, fUnitScale );
		}
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//		static CX2GUNPC* CreateGUNPC( int unitIndex, int teamNum, const WCHAR* pScriptFileName, int uid, const CX2UnitManager::NPCUnitTemplet* pNPCTemplet, 
//			int eAIType, CX2GameUnit* pOwnerGameUnit_ = NULL, float fUnitScale = -1.f )
//		{
//			return new CX2GUNPC( unitIndex, teamNum, pScriptFileName, uid, pNPCTemplet, eAIType, pOwnerGameUnit_, fUnitScale );
//		}
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING

		static CX2GUNPC* CreateBattleFieldNpc( const CNpcCreatedInfoPtr NpcCreatedInfoPtr_
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
            , const CX2UnitManager::NPCUnitInfo* pNPCUnitInfo
//#else   X2OPTIMIZE_NPC_LUASPACE_SHARING
//            , const CX2UnitManager::NPCUnitTemplet* pNPCTemplet
//#endif  X2OPTIMIZE_NPC_LUASPACE_SHARING
		)
		{
			return new CX2GUNPC( NpcCreatedInfoPtr_
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
                , pNPCUnitInfo
//#else   X2OPTIMIZE_NPC_LUASPACE_SHARING
//                , pNPCTemplet, pScriptLuaState
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
			);
		}

//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
		lua_State*					GetFunctionLuaState();
		static void					InitSystem( InitData& OutInit_, KLuaManager& luaManager_ );
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
		static void                 InitComponent( InitData& OutInit_, KLuaManager& luaManager_ );
		static void                 InitMotion( InitData& OutInit_, KLuaManager& luaManager_ );	
		static void					InitInit( InitData& OutInit_, KLuaManager& luaManager_ );
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        static void                 InitState( InitData& OutInit_, KLuaManager& luaManager_
#ifdef  X2OPTIMIZE_VERIFY_NPC_SCRIPT_CONSISTENCY
            , FILE* pLog = NULL, const wchar_t* pwszScriptFile = L"" 
#endif  X2OPTIMIZE_VERIFY_NPC_SCRIPT_CONSISTENCY
            );
        static void                 LoadStateData( StateData& OutStateData_, InitData& OutInit_, KLuaManager& luaManager_
#ifdef  X2OPTIMIZE_VERIFY_NPC_SCRIPT_CONSISTENCY
            , int iStateID, const std::wstring& wstrStateName, bool& bError, FILE* pLog = NULL, const wchar_t* pwszScriptFile = L"" 
#endif  X2OPTIMIZE_VERIFY_NPC_SCRIPT_CONSISTENCY
            );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef NEXON_QA_CHEAT_REQ
		void						InitCheat( const D3DXVECTOR3& vPos, bool bIsRight );
#endif //NEXON_QA_CHEAT_REQ

		virtual void				Init( bool bUseTeamPos = true, int startPosIndex = -1 );
		virtual void				ReInit( bool bUseTeamPos = true, int startPosIndex = -1 );
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        void                        ReInitExtraWork();
//#endif SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		//virtual void				ChangeNPC( int npcID );

		virtual void				InitEffect();
		virtual void				DeleteMinorParticle();

		virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT				OnCameraMove();

//{{ robobeg : 2008-10-28
        virtual RENDER_HINT         OnFrameRender_Prepare();
		virtual void                OnFrameRender_Draw();
//}} robobeg : 2008-10-28

		virtual HRESULT				OnResetDevice();
		virtual HRESULT				OnLostDevice();

		virtual void				SetSmallGage();

		virtual void				Win()
		{
#ifdef SERV_PVP_NEW_SYSTEM
			if( IsPvpBot() == true )
				StateChange( m_WinState, true );
#endif
		}
		virtual void				Lose()
		{
#ifdef SERV_PVP_NEW_SYSTEM
			if( IsPvpBot() == true )
				StateChange( m_LoseState, true );
#endif
		}
		virtual void				Draw()
		{
#ifdef SERV_PVP_NEW_SYSTEM
			if( IsPvpBot() == true )
				StateChange( m_WinState, true );
#endif
		}

#ifdef  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE
        void                        StateChangeToDieForce();
#endif  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE

#ifdef MONSTER_STATE_CHANGE_CHEAT
		void						StateChangeCheat( int state );
#endif MONSTER_STATE_CHANGE_CHEAT
		bool						StateChange_LUA( const char* stateName, bool bDirectChange );		
		bool                        StateChangeForce( int state, bool bDirectChange = false );
		virtual bool                StateChange( int state, bool bDirectChange = false );
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        void                        UnDoStateChange_Reaction();
        bool                        StateChange_Reaction_FIRST( int state, int iAttackerUnitIndex,  const D3DXVECTOR3& vImpact, bool bRelaxReactionCheck = false ); 
        bool                        StateChange_Reaction_Continue( int state, bool bWait = false ); 
        int                         StateChange_CheckOnly( int state, bool bDirectChange = false );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//{{ robobeg : 2008-10-28
		bool                        StateChange( const char* state, bool bDirectChange = false );
		int							GetStateID( const char* state )
        {
            if ( state == NULL )
                return GUSI_NONE;
            return GetStateID( std::string(state) );
        }//
        int                         GetStateID( const std::string& strInState_ )
        {
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
            return  GetInitStateData().GetStateID( strInState_ );
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
            std::map<string,char>::iterator iter = m_StateNameIDMap.find( strInState_ );
            if ( iter == m_StateNameIDMap.end() )
                return GUSI_NONE;
            return iter->second;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        }//FindStateIDByName()
//}} robobeg : 2008-10-28

#ifdef MONSTER_STATE_CHANGE_CHEAT
        int                         GetStateIDByKeyword( const std::string& strKeyword );
		const string&					GetStateNameByStateID( int iStateID );

#endif MONSTER_STATE_CHANGE_CHEAT
		virtual int					GetNowStateID() const;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        const StateData&            GetNowStateData() const
        {
            const std::vector<StateData>& vecStateData = GetInitStateData().m_vecStateData;
            ASSERT( m_NPCFrameData.syncData.nowState >= 0 && m_NPCFrameData.syncData.nowState < (int) vecStateData.size() );
            return vecStateData[ ( m_NPCFrameData.syncData.nowState >= 0 && m_NPCFrameData.syncData.nowState < (int) vecStateData.size() )
                ? m_NPCFrameData.syncData.nowState : 0 ];
        }
        const StateRuntime&         GetNowStateRuntime() const
        {
            ASSERT( m_NPCFrameData.syncData.nowState >= 0 && m_NPCFrameData.syncData.nowState < (int) m_vecStateRuntime.size() );
            return  m_vecStateRuntime[ ( m_NPCFrameData.syncData.nowState >= 0 && m_NPCFrameData.syncData.nowState < (int) m_vecStateRuntime.size() )
                ? m_NPCFrameData.syncData.nowState : 0 ];
        }

        const InitStateData&        GetInitStateData() const    { return m_pInitData->m_state; }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
		virtual void				DamageReact( CX2DamageManager::DamageData* pDamageData );

		void DamageReact_ExtraDamage( CX2DamageManager::DamageData* pDamageData, bool bApplyExtraDamageOfAttacker
#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
            , bool bAttackedByMe
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
            );
		void Process_ExtraDamage( const CX2DamageManager::DamageData* pDamageData, const CX2DamageManager::ExtraDamageData &extraDamageData
#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
            , bool bAttackedByMe 
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
            );
		void DamageReact_Rage( CX2DamageManager::DamageData* pDamageData );

		//{{ oasis907 : 김상윤 [2010.3.22] // 
		void LevelCorrectionExtraDamage( const CX2DamageManager::DamageData* pDamageData, CX2DamageManager::ExtraDamageData* pExtraDamageData);
		//}}

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        bool                        DamageReactStateChange( CX2DamageManager::DamageData* pDamageData, int iAttackerUnitIndex );
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
        int                         DamageReactStateChange_CheckOnly( CX2DamageManager::DamageData* pDamageData );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		bool                        DamageReactStateChange( CX2DamageManager::DamageData* pDamageData );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
		virtual void				AttackResult();
		virtual void				AttackResultStateChange();
		virtual void				RenderName();
		virtual void				RenderCrazyMode();

		void						RenderPvpNpcServerTexture();
		void						InitFullName();
		void						InitNameColor( const int iMyLevel_ );

		void						SetInvincibleTime( float fStart, float fEnd ){ m_NPCFrameData.stateParam.invincibleTime.m_fX = fStart; m_NPCFrameData.stateParam.invincibleTime.m_fY = fEnd; }
		void						SetSuperArmorStateParam( bool bSuperArmor ){ m_NPCFrameData.stateParam.bSuperArmor = bSuperArmor; }

		virtual D3DXVECTOR3			GetHeadBonePos() const
		{
			D3DXVECTOR3 tempPos(0, 0, 0);
			if( true == GetFramePos( &tempPos, m_pFrame_Bip01_Head ) )
			{
				tempPos.y += 70.0f;
			}
			else
			{
				// head bone 이름이 잘못 지정된 몬스터의 경우에 임시로 자기 위치에서 키높이 만큼을 주는 것으로 한다
#ifdef NO_MORE_ERROR_TEST

				WCHAR wszText[256] = { 0, };
				StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"[LOG_IN_HOUSE_ONLY] wrong head bone name : %s_%s"
                    , GetNPCTemplet().m_LuaFileName.c_str(), GetHeadBoneName().c_str() ); 
				InHouseLog( wszText );
#endif NO_MORE_ERROR_TEST

				tempPos = GetPos();
				tempPos.y += GetUnitHeight( false );
			}
			return tempPos; 
		}
#ifdef INDEPENDENT_ANIMATION_TIME
		virtual const CKTDXCollision::CollisionDataListSet&		GetCollisionListSet();
		virtual D3DXVECTOR3										GetGameUnitCloneFramePosition( const WCHAR* pBoneName );
		virtual	bool											GetGameUnitFramePosition( D3DXVECTOR3* pPos, CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame );
		virtual D3DXVECTOR3			GetBonePos( const WCHAR* pBoneName )		{ return GetGameUnitCloneFramePosition( pBoneName ); }
#else
		virtual D3DXVECTOR3			GetBonePos( const WCHAR* pBoneName ) const	{ return ( NULL != m_pXSkinAnim  ? m_pXSkinAnim->GetCloneFramePosition( pBoneName ) : D3DXVECTOR3( 0, 0, 0 ) ); }
	#ifdef MODIFY_GET_BONE_POS
		// 오현빈 // 2012-12-17 // 본이 없는 경우에 대해 체크하기 위해 OUT 인자를 사용하는 함수 추가
		virtual bool		GetBonePos( const WCHAR* pBoneName, OUT D3DXVECTOR3& pos_ ) const	{ return ( NULL != m_pXSkinAnim ? m_pXSkinAnim->GetCloneFramePosition( pBoneName, pos_ ) : false ); }
	#endif //MODIFY_GET_BONE_POS
#endif INDEPENDENT_ANIMATION_TIME
		D3DXVECTOR3					GetBonePos_LUA( const char* pBoneName )
		{ 
			wstring wBoneName;
			ConvertUtf8ToWCHAR( wBoneName, pBoneName );
			return GetBonePos( wBoneName.c_str() ); 
		}

		//{{ kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)
		virtual const D3DXMATRIX*	GetCombineMatrixFromBoneName( const wstring& wstrBoneName ) const;
		//}} kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)	

//{{ robobeg : 2008-10-21
		//virtual void				SetShow( bool bShow );
		virtual void				NotifyShowObjectChanged();
//}} robobeg : 2008-10-21

// X2GUNPC 가 KTDGManager 에 등록, 삭제될 때 장착 부품들도 같이 등록, 삭제하기 위해 사용됨
        virtual void                NotifyDGManagerChanged( CKTDGManager& refInManager_ );
//}} robobeg : 2008-10-17


		virtual void				SetHyperModeCount( int i ){ m_HyperModeCount = i; }
		virtual int					GetHyperModeCount() const { return m_HyperModeCount; }
		virtual int					GetLastStateHyperModeCount() const { return GetHyperModeCount(); }
		virtual void				CreateAndInsertRenderParamByBuffUnitType( const KBuffIdentity& BuffIdentity_, const map<CX2Unit::UNIT_TYPE, StBuffRenderParamPtr>& mapStBuffRenderParamPtr_ );

		virtual float				GetStopTime();

		virtual void				SetStopTime( float fStopTime );
		virtual void				SetStop2Time( float fStopTime );

		virtual const UnitCondition& GetUnitCondition( bool bFuture = false ) const		{ return GetNPCFrameData().unitCondition; }
		bool				        GetUnitCondition_bHit( bool bFuture = false ) const	{ return GetNPCFrameData().unitCondition.bHit; }
		virtual const StateParam&	GetStateparam( bool bFuture = false ) const		{ return GetNPCFrameData().stateParam; }

		virtual const D3DXVECTOR3&	GetPos( bool bFuture = false ) const		{ return GetNPCSyncData().position; }
		virtual float				GetXPos( bool bFuture = false )	const			{ return GetNPCSyncData().position.x; }
		virtual float				GetYPos( bool bFuture = false )	const		{ return GetNPCSyncData().position.y; }
		virtual float				GetZPos( bool bFuture = false )	const		{ return GetNPCSyncData().position.z; }   
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        virtual void                BackupPosition()  
        {
            m_vPositionBackup = m_NPCFrameData.syncData.position; 
            m_iPositionBackup_LineIndex = m_NPCFrameData.syncData.lastTouchLineIndex;
            m_bPositionBackup = true;
            m_bPositionBackup_FootOnLine = m_NPCFrameData.unitCondition.bFootOnLine;
        }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        virtual void                OnFrameMove_PostProcess();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


		virtual const D3DXVECTOR3&	GetRotateDegree() const						{ return m_NPCFrameData.unitCondition.dirDegree; }
		virtual float				GetXRotateDegree() const							{ return m_NPCFrameData.unitCondition.dirDegree.x; }
		virtual float				GetYRotateDegree() const							{ return m_NPCFrameData.unitCondition.dirDegree.y; }
		virtual float				GetZRotateDegree() const							{ return m_NPCFrameData.unitCondition.dirDegree.z; }
		
		virtual float				GetPlaySpeed() const					{ return m_pXSkinAnim->GetPlaySpeed(); }	

		virtual const D3DXVECTOR3&	GetDirVector() const							{ return m_NPCFrameData.unitCondition.dirVector; }
		virtual bool				GetIsRight() const								{ return m_NPCFrameData.syncData.bIsRight; }
#ifdef  X2OPTIMIZE_ROBUST_NPC_ROTATION
		void						SetIsRightNow( bool bIsRight )
        {
            if ( bIsRight != m_NPCFrameData.syncData.bIsRight )
            {
                m_NPCFrameData.syncData.bIsRight = bIsRight;
                if ( m_NPCFrameData.unitCondition.dirDegree.y >= 180.f )
                    m_NPCFrameData.unitCondition.dirDegree.y -= 180.f;
                else
                    m_NPCFrameData.unitCondition.dirDegree.y += 180.0f;
            }//if
        }
#else   X2OPTIMIZE_ROBUST_NPC_ROTATION
		void						SetIsRight( bool bIsRight )					
		{ 
			//CKTDGLineMap*	pLineMap = g_pX2Game->GetWorld()->GetLineMap();
			//D3DXVECTOR3		vPos(0, 0, 0);

			m_NPCFrameData.syncData.bIsRight = bIsRight;

			////정확한 라인 보정위치와 라인 인덱스를 구한다
			//int lineIndex = -1;
			//D3DXVECTOR3	landPos = startPos;
			//landPos = pLineMap->GetLandPosition( startPos, LINE_RADIUS, &lineIndex );
			//pLineMap->IsOnLine( startPos, LINE_RADIUS, &startPos, &lineIndex );

			//CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( lineIndex );
			//if( pLineData != NULL )
			//{
			//	m_NPCFrameData.unitCondition.dirDegree			= pLineData->dirDegree;
			//	if( m_NPCFrameData.syncData.bIsRight == false )
			//		m_NPCFrameData.unitCondition.dirDegree.y		+= 180.0f;
			//}
		}
#endif  X2OPTIMIZE_ROBUST_NPC_ROTATION
        BOOL                        GetDirPos_LUA(D3DXVECTOR3 vPos);        

		//const CX2UnitManager::NPCUnitTemplet* GetNPCTemplet() const { return m_pNPCTemplet; }
        const CX2UnitManager::NPCUnitTemplet& GetNPCTemplet() const { return  m_pNPCUnitInfo->m_templet; }
        const CX2UnitManager::NPCUnitStat&    GetNPCUnitStat() const { return m_pNPCUnitInfo->m_stat; }

		CX2UnitManager::NPC_CLASS_TYPE GetClassType() const { return GetNPCTemplet().m_ClassType; }

		virtual GAME_UNIT_STATE_ID	GetGameUnitState() const					{ return (GAME_UNIT_STATE_ID)m_NPCFrameData.syncData.nowState; }
		virtual bool				GetInvincible()	const						{ return m_NPCFrameData.stateParam.bInvincible; }
		virtual bool				GetIsFallDownState()						
        { 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            const StateData& nowStateData = GetNowStateData();
            return  NOWSTATE_FLAG( FALL_DOWN );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return m_NPCFrameData.stateParam.bFallDown; 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }


		virtual float				GetUnitWidth( bool bFuture ) const			
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            const StateData& nowStateData = GetNowStateData();
            if ( NOWSTATE_FLAG( SWAP_UNIT_WIDTH_HEIGHT ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( true == m_bSwapUnitWidthHeight )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			{
				return GetNPCFrameData().unitCondition.fUnitHeight;
			}
			else
			{
				return GetNPCFrameData().unitCondition.fUnitWidth;
			}
		}
		virtual float				GetUnitHeight( bool bFuture ) const
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            const StateData& nowStateData = GetNowStateData();
            if ( NOWSTATE_FLAG( SWAP_UNIT_WIDTH_HEIGHT ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( true == m_bSwapUnitWidthHeight )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			{
				return GetNPCFrameData().unitCondition.fUnitWidth;
			}
			else
			{
				return GetNPCFrameData().unitCondition.fUnitHeight;
			}
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        float                       GetSlashTraceTipWide() const    { return GetNowStateData().m_fSlashTraceTipWide; }

        bool                        FindAddSlashTraceWithWeaponIndex( int iWeaponIndex, OUT D3DXVECTOR3& vAddSlashTrace ) const;

        const D3DXVECTOR2&          GetAfterImageTime() const       { return GetNowStateData().m_v2AfterImageTime; }



        virtual UINT    GetSizeOfAttackTimeList() const             { return GetNowStateData().m_vecAttackTime.size(); }
        virtual const D3DXVECTOR2* const GetAttackTimeByIndex( const UINT uiIndex_ ) const
        {
            const std::vector<KAttackTimeProjSeqName>& vecAttackTime = GetNowStateData().m_vecAttackTime;
            if ( uiIndex_ < vecAttackTime.size() )
                return  &vecAttackTime[ uiIndex_ ].m_v2AttackTime;
            else
                return  NULL;
        }

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


		D3DXVECTOR3					GetBoneRotateDegree_LUA( char* pBoneName );
		void						SetBoneScale_LUA(char* pBoneName, bool bScaleChild, float sx, float sy, float sz);
		
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		void				        ReceiveFrameData( UidType uidUnitUID, DWORD dwFrameMoveData, const KDYNAMIC_UNIT_NPC_SYNC& pXPT_UNIT_NPC_SYNC );
		bool                        SendPacket( CX2FrameUDPPack& kFrameUDPPack, bool bImmediateOnly );
		bool						SendPacketImmediateForce( CX2FrameUDPPack& kFrameUDPPack );
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        void                        ReceiveMiniSyncData( UidType uidUnitUID, DWORD dwFrameMoveData, const KDYNAMIC_UNIT_NPC_MINISYNC& kMiniSync );
        bool                        SendMiniSyncPacket( CX2FrameUDPPack& kFrameUDPPack );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		virtual void				ReceiveFrameData( KXPT_UNIT_NPC_SYNC* pXPT_UNIT_NPC_SYNC );
//		void						SendPacket( vector<KXPT_UNIT_NPC_SYNC>& syncList );
//		void						SendPacketImmediate( vector<KXPT_UNIT_NPC_SYNC>& syncList );
//		void						SendPacketImmediateForce( vector<KXPT_UNIT_NPC_SYNC>& syncList );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		virtual bool				IsMyUnit() const;
		virtual bool				IsLocalUnit() const;
		virtual bool				IsFocusUnit() const;
		void						SetUID( int uid ){ m_UnitUID = uid; }
		int							GetUID() const { return (int)m_UnitUID; }

		////////////////////////////////////////////////////////////

		const NPCSyncData&			GetNPCSyncData() const						{ return m_NPCFrameData.syncData; }
		const NPCFrameData&			GetNPCFrameData() const						{ return m_NPCFrameData; }
		
		void						CallOtherNPC( CX2UnitManager::NPC_UNIT_ID npcUnitID );
		void						SetRallyToUnit( CX2GameUnit* pRallyGameUnit_ );
		
#ifdef RIDING_MONSTER
		void						CallNearOneNPC( CX2UnitManager::NPC_UNIT_ID npcUnitID, float fMaxRange );
		bool						TryToRideTargetNPC();
		void						RideOff();
		bool						IsMoveableState();

		void						JumpDownFromLinemap();

		void						ResetRideUnit() { m_optrRidingGameUnit.Reset(); }
		void						SetRideUnit(CX2GUNPC* pUnit){ m_optrRidingGameUnit = pUnit; }
		CX2GUNPC*					GetRideUnit();
		void						StopRideToUnit();
#ifdef VELDER_BOSS_5
		void						SetRideToUnit( CX2GameUnit* pGameUnit_ ) ;
#endif VELDER_BOSS_5
#endif RIDING_MONSTER




		CX2NPCAI*					GetNPCAI() { return m_pAI; }

		void						PushAIMessage( STATE_CHANGE_TYPE message, bool bIsRight );
//{{oasis:김상윤////2009-10-7////
		void						PushUnUseAIMessage( STATE_CHANGE_TYPE message);
//}}oasis:김상윤////2009-10-7////

		bool						GetAIEnabled();
		void						SetAIEnable( bool bEnable );
		float GetTimePauseNPCAI() const { return m_fTimePauseNPCAI; }
		void SetTimePauseNPCAI(const float val);

		D3DXVECTOR3					GetAITargetPos(); // warning!! ai target pos은 sync를 안하기 때문에 훈련소처럼 방장 1인플레이에만 써야 된다~
		D3DXVECTOR3					GetNearestUserPos();
		D3DXVECTOR3					GetNearestNPCPos();
        bool                        IsUnitID(CX2UnitManager::NPC_UNIT_ID unitID);

		bool						HasTarget();
#ifdef SECRET_VELDER
		D3DXVECTOR3					GetNearestUserLandPos();
#endif
		
		

		// 입력된 위치값을 지정된 라인을 지나는 Y축에 평행한 평면과 교차하는 지점으로 옮겨서 캐릭터 위치로 설정한다
		virtual void SetPositionOnLine( const D3DXVECTOR3& vPosition, const int iLineIndex );

		// 라인에 딱 붙지 않고 허공에 위치 시킬 수 있는
		void SetPositionForce( const D3DXVECTOR3& vPosition );

		// 라인에 딱 붙어 버린다
		bool						SetPosition( const D3DXVECTOR3& vPos, bool bIsRight );


		void						SetRotateDegree( const D3DXVECTOR3& vDegree );
		
		void						ClearRecieveSyncData();
		bool						AnimEventTimer_LUA( float fTime )
		{			
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            if( NULL != m_pXSkinAnim && m_pXSkinAnim->EventTimerOneshot( fTime ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( NULL != m_pXSkinAnim && m_pXSkinAnim->EventTimer( fTime ) == true && EventCheck( fTime ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				return true;
			else
				return false;
		}
		bool						AnimTimer_LUA( float fTime )
		{			
			if( NULL != m_pXSkinAnim && m_pXSkinAnim->EventTimer( fTime ) == true )
				return true;
			else
				return false;
		}

		float						GetNowAnimTime_LUA() { return (NULL != m_pXSkinAnim ? m_pXSkinAnim->GetNowAnimationTime() : 0.0f); }


		bool						EventTimer( float fTime );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        bool                        StateEventInterval_LUA( int iIntervalID );
        bool                        GetMotionEnd();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool						EventCheck( float fTime );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


		virtual int					GetLastTouchLineIndex( bool bFuture = false ) const
		{
#ifdef FIELD_BOSS_RAID
			const StateData& nowStateData = GetNowStateData();
			if( true == NOWSTATE_FLAG( INVALID_LAST_TOUCH_INDEX ) )
				return -1;
#endif // FIELD_BOSS_RAID
			
			return m_NPCFrameData.syncData.lastTouchLineIndex;
		}

		virtual const D3DXVECTOR3&	GetLandPos( bool bFuture = false ) const { return m_NPCFrameData.unitCondition.landPosition; }
		D3DXVECTOR3					GetLandPosition_LUA(){ return m_NPCFrameData.unitCondition.landPosition; }
		
		void						SetEnableCondtionTableEventProcess(bool val) { m_bEnableCondtionTableEventProcess = val; }
		void						SetEnableEventProcess(bool bEnable) { m_bEnableEventProcess = bEnable; }
		void						SetActiveMonster(bool bIsActive) { m_bIsActiveMonster = bIsActive; }
		bool						IsActiveMonster() { return m_bIsActiveMonster; }
		void						SetAddRotateDegree_LUA( float x, float y, float z ){ m_AddRotateDegree.x = x; m_AddRotateDegree.y = y; m_AddRotateDegree.z = z; }
		CKTDGXSkinAnimPtr			GetXSkinAnimPtr(){ return ( NULL != m_pXSkinAnim ? m_pXSkinAnim : CKTDGXSkinAnimPtr() ); }
		CKTDGXSkinAnim*				GetXSkinAnim_LUA(){ return ( NULL != m_pXSkinAnim ? m_pXSkinAnim.get() : NULL ); }

		void						SetKeyCode( int keyCode ){ m_KeyCode = keyCode; }
		int							GetKeyCode(){ return m_KeyCode; }
		void						SetFocusCamera( bool bFocusCamera ){ m_bFocusCamera = bFocusCamera; }
		bool						GetFocusCamera(){ return m_bFocusCamera; }
//{{oasis:김상윤////2009-10-7////
		void						SetAllFlag_LUA( bool bFlag );
//}}oasis:김상윤////2009-10-7////
		void						SetFlag_LUA( int index, bool bFlag ){ m_FlagList[index] = bFlag; }
		bool						GetFlag_LUA( int index ){ return m_FlagList[index]; }
		void						SetInt_LUA( int index, int bFlag ){ m_IntList[index] = bFlag; }
		int							GetInt_LUA( int index ){ return m_IntList[index]; }
		void						SetVector_LUA( int index, D3DXVECTOR3 vVec ){ m_VecList[index] = vVec; }
		D3DXVECTOR3					GetVector_LUA( int index ){ return m_VecList[index]; }
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
		double						GetNumber_LUA( int index ) { return m_NumberList[ index ]; }
		void						SetNumber_LUA( int index, double dValue_ ) { m_NumberList[ index ] = dValue_; }
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING

		void						SetMajorParticle_LUA( int index, CKTDGParticleSystem::CParticleEventSequenceHandle  hSeq ){ m_hMajorSeqList[index] = hSeq; }
		void						SetMinorParticle_LUA( int index, CKTDGParticleSystem::CParticleEventSequenceHandle  hSeq ){ m_hMinorSeqList[index] = hSeq; }
		void						ClearMinorParticle_LUA( int index )
		{ 
			m_hMinorSeqList[index] = INVALID_PARTICLE_SEQUENCE_HANDLE; 
		}
		void						ClearMajorParticle_LUA( int index )
		{ 
			m_hMajorSeqList[index] = INVALID_PARTICLE_SEQUENCE_HANDLE; 
		}

		//{{ kimhc // 2010.4.19 // 비밀던전 작업
#ifdef SERV_SECRET_HELL
		void								SetEffectSet_LUA( int iIndex, CX2EffectSet::Handle hHandle );
		CX2EffectSet::Handle				GetEffectSet_LUA( int iIndex );
		void								ClearEffectSet_LUA( int iIndex ) { m_hEffectSetHandle[iIndex] = INVALID_EFFECTSET_HANDLE; }
#endif SERV_SECRET_HELL
		//}} kimhc // 2010.4.19 // 비밀던전 작업


		virtual float				GetPowerRate();

//{{ kimhc // 2010.7.7 // 몬스터가 생성한 라인맵
#ifdef	LINE_MAP_CREATED_BY_MONSTER
		void InsertMonsterLineMapIndex_LUA( int iIndex ) { m_vecMonsterLineMapIndex.push_back( iIndex ); }
		UINT GetNumOfMonsterLineMap_LUA() const { return m_vecMonsterLineMapIndex.size(); }
		int	 GetMonsterLineMapIndex_LUA( int iIndex ) { return m_vecMonsterLineMapIndex[iIndex]; }

		void MoveMonsterLineMap( const D3DXVECTOR3& vPosBackup );
#endif	LINE_MAP_CREATED_BY_MONSTER
//}} kimhc // 2010.7.7 // 몬스터가 생성한 라인맵


		bool IsOnSomething()
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            const StateData& nowStateData = GetNowStateData();
            if ( NOWSTATE_FLAG( NOT_USE_LAND_CONNECT ) == true )
                return true;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( m_NPCFrameData.stateParam.bNotUseLandConnect == true )
				return true;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

//#ifdef STEP_ON_MONSTER_TEST
//			return m_NPCFrameData.unitCondition.bFootOnLine || 
//				m_NPCFrameData.unitCondition.bFootOnUnit;
//#else STEP_ON_MONSTER_TEST
			return m_NPCFrameData.unitCondition.bFootOnLine;
//#endif STEP_ON_MONSTER_TEST
		}


		void ResetOnSomething()
		{
			m_NPCFrameData.unitCondition.bFootOnLine = false;
//#ifdef STEP_ON_MONSTER_TEST
//			m_NPCFrameData.unitCondition.bFootOnUnit = false;
//#endif STEP_ON_MONSTER_TEST
		}





#ifdef GRAPPLING_TEST
		virtual void CalcGrapplingPosition( bool bFuture )
		{
			m_NPCFrameData.unitCondition.m_GrapplingState.CalcGrapplingPosition();
		}

		virtual void ResetGrapplingState( bool bOnlyMe = false )
		{
			if( false == bOnlyMe )
			{
				if( true == IsGrappled(false) )
				{
					SetPositionOnLine( GetPos(), GetLastTouchLineIndex() );
				}
			}

			m_NPCFrameData.unitCondition.m_GrapplingState.ResetGrapplingState( bOnlyMe );
		}

		virtual void SetUnitGrapplingMe( bool bFuture, GAME_UNIT_TYPE eGameUnitType, UidType unitUID )
		{
			m_NPCFrameData.unitCondition.m_GrapplingState.SetUnitGrapplingMe( eGameUnitType, unitUID );
		}

		virtual void SetUnitGrappledByMe( bool bFuture, GAME_UNIT_TYPE eGameUnitType, UidType unitUID )
		{
			m_NPCFrameData.unitCondition.m_GrapplingState.SetUnitGrappledByMe( eGameUnitType, unitUID );
		}
		virtual CX2GameUnit* GetUnitGrapplingMe( bool bFuture )
		{
			return m_NPCFrameData.unitCondition.m_GrapplingState.GetUnitGrapplingMe();
		}

		virtual CX2GameUnit* GetUnitGrappledByMe( bool bFuture )
		{
			return m_NPCFrameData.unitCondition.m_GrapplingState.GetUnitGrappledByMe();
		}

		virtual bool IsGrappled( bool bFuture )
		{
			KTDXPROFILE();

			return m_NPCFrameData.unitCondition.m_GrapplingState.m_UnitUIDGrapplingMe > 0; 
		}


#endif GRAPPLING_TEST
		
		virtual void InitAddtionalDashAndJumpBasicStatValue();
		void ScaleMoveSpeed( float fScale );

		CKTDGParticleSystem::CParticleEventSequenceHandle 	GetMajorParticleHandle( int index );

		
		CKTDGParticleSystem::CParticleEventSequence* 	GetMajorParticle_LUA( int index );
		CKTDGParticleSystem::CParticleEventSequence* 	GetMinorParticle_LUA( int index );


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*	GetDamageEffect_LUA( int index );
		void						SetDamageEffect_LUA( int index, CX2DamageEffect::CEffect* val);
		void						ClearDamageEffect_LUA( int index );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*	GetDamageEffect_LUA( int index ) { return m_pDamageEffect[index]; }
		void						SetDamageEffect_LUA( int index, CX2DamageEffect::CEffect* val) { m_pDamageEffect[index] = val; }
		void						ClearDamageEffect_LUA( int index ) { m_pDamageEffect[index] = NULL; }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		void						AttachParticle_LUA( bool bIsMajorParticle, char* particleSeqName, char* pBoneName, D3DXVECTOR3 vOffsetPos );
		        
		float						GetSpeedY() { return m_PhysicParam.nowSpeed.y; }
		void						SetSpeedX( float fSpeed ){ m_PhysicParam.nowSpeed.x = fSpeed; }
		void						SetSpeedY( float fSpeed ){ m_PhysicParam.nowSpeed.y = fSpeed; }
#ifdef SEASON3_MONSTER_2011_7
		float						GetSpeedX() { return m_PhysicParam.nowSpeed.x; }
		void						SetGAccel( float fGAccel ){ m_PhysicParam.fGAccel = fGAccel; }
		void						SetReloadAccel(float fReloadAccel ){ m_PhysicParam.fReloadAccel = fReloadAccel; }
#endif SEASON3_MONSTER_2011_7
		void						SetPassiveSpeed( float x, float y ) { m_PhysicParam.passiveSpeed.x = x; m_PhysicParam.passiveSpeed.y = y; }
		void						SetAddPos( const D3DXVECTOR3& pos );
		void						AddForceDownDelay( float fDelay );

		virtual void				InitPosition( bool bUseTeamPos = true, int startPosIndex = -1 );
		void						SetHardLevel( int hardLevel, bool bResetHP, float fHPScale = 1.f );
		int							GetHardLevel(){ return m_HardLevel; }
		//HardLevelData*				GetHardLevelData(){ return &m_HardLevelData; }

		void						SetNPCStat( float fHP, float fAtkPhysic, float fAtkMagic, float fDefPhysic, float fDefMagic, bool bUpdateMaxHp = true );
		//{{ megagame / 박교현 / 2010.04.30 / NPC 스탯 추가
		void						GetNPCStat( CX2UnitManager::NPCUnitStat& NpcStat );
		void						SetNPCStat( const CX2UnitManager::NPCUnitStat& NpcStat );
		//}}



		bool						GetDamagedThisFrame() 
		{ 
			bool bDamaged = m_bDamagedThisFrameMove;
			m_bDamagedThisFrameMove = false;
			return bDamaged;
		}
#ifdef SEASON3_MONSTER_2010_12
		CX2DamageManager::REACT_TYPE	GetDamageTypeThisFrame()
		{
			return m_eDamageTypeThisFrameMove;
		}
		void InitDamageTypeThisFrame()
		{
			m_eDamageTypeThisFrameMove = CX2DamageManager::RT_NO_DAMAGE;
		}
		
#endif
		virtual float GetNowAnimationTime() const
		{
			return ( m_pXSkinAnim != NULL ? m_pXSkinAnim->GetNowAnimationTime() : -1.0f );
		}
		
		virtual void SetNowAnimationTime( const float fAnimCurrentTime_ )
		{
			if ( NULL != m_pXSkinAnim )
				m_pXSkinAnim->SetAnimationTime( fAnimCurrentTime_ );
		}

		int							GetStartState() const { return m_StartState; }
		int							GetRebirthState() const { return m_RebirthState; }
		int							GetWaitStateID() const { return m_CommonState.m_Wait; }

		int GetFlyDamageFrontStateID() const { return m_FlyDamageFront; }
		
		int							GetDownDamageLandFront() const { return m_CommonState.m_DownDamageLandFront; }
		int							GetDownDamageLandBack() const { return m_CommonState.m_DownDamageLandBack; }

		int							GetDisabledStateID() const { return m_DisabledState; }

#ifdef RIDING_MONSTER
		int							GetUpDamage() const { return m_UpDamage; }
		int							GetAirFallState() const { return m_AirFallState; }
		int							GetRideStateID() const { return m_RideState; }
#endif RIDING_MONSTER
		int							GetImmuneSkillState() const { return m_ImmuneSkillState; }

		virtual bool				GetShowOnMiniMap() const;
		virtual bool				IsGateNpc() const { return (CX2UnitManager::NCT_THING_GATE == GetNPCTemplet().m_ClassType); }

		void						ShowGateGuideFrameMove( double fTime, float fElapsedTime );

		void						SetDieSeq( CKTDGParticleSystem::CParticleEventSequenceHandle hSeq ){ m_hSeqDieEffect = hSeq; }

		
		void						SetLockOnTarget( CX2DamageEffect::CEffect* pCEffect );
		void						SetLockOnNearTarget( CX2DamageEffect::CEffect* pCEffect );
		//2013-08-29패치 : <김창한> uid를 지정해서 유닛을 타겟하도록 하는 함수. 하멜 비던에서 사용됨.
		void						SetLockOnUnitUIDFix( CX2DamageEffect::CEffect* pCEffect, UidType uUID );


		void						ChangeHyperMode_LUA();

		void						SetLockOn( CX2DamageEffect::CEffect* pCEffect, int randomOffset = 0 );
		void						SetLockOnPos( CX2DamageEffect::CEffect* pCEffect, D3DXVECTOR3 vPos );

		void						SetSiegeMode( bool bSiege );
		void						SetSiegeModeForce( bool bSiege );
		bool 						GetSiegeMode() const { return m_bSiegeMode; }
		void						ReleaseSiegeMode( float fRange );

		bool						GetIsFallDownMonster() { return m_bFallDown; }
        bool                        GetIsDamageDown() { return m_bDamageDown; }
		void						SetRageForce( float fTime ) { m_fRageTime = fTime; }
		
#ifndef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void						ClearEventCheck( float fTime )
		{
			map<float,bool>::iterator iter;
			iter = m_EventTimeStamp.find( fTime );
			if( iter != m_EventTimeStamp.end() )
			{
				m_EventTimeStamp.erase( iter );
			}
		}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


		bool						GetFollowHitterLost() { return m_bFollowHitterLost; }
		int							GetFollowHitter() { return m_iFollowHitter; }
		D3DXVECTOR3					GetLineGroupStartPos();
		D3DXVECTOR3					GetLineGroupEndPos();
		float						GetStateTime() { return m_NPCFrameData.unitCondition.fStateTime; }
		float						GetNowHP_LUA(){ return GetNowHp(); }
		void						SetNowHP_LUA( float fHP );


#ifdef SUBBOSS_MONSTER_KILL_EFFECT
		//{{ oasis907 : 김상윤 [2010.4.29] // 
		bool IsPlayBossKillEffect() { return m_bPlayBossKillEffect; }
		void SetPlayBossKillEffect(bool val) { m_bPlayBossKillEffect = val; }
		//}}
#endif SUBBOSS_MONSTER_KILL_EFFECT		

        void SetShowBossGage(bool val) 
        { 
// 			GetGageData()->SetShow(val); 
// 			SetHasBossGage( val );
        }
		bool GetShowBossName() const { return m_bShowBossName; }
		void SetShowBossName(bool val) { m_bShowBossName = val; }

#ifdef MODIFY_DUNGEON_STAGING
		float GetShowBossNameDelayTime() const { return m_fShowBossNameDelayTime; }
		void SetShowBossNameDelayTime(float val) { m_fShowBossNameDelayTime = val; }

		bool GetStopAtStartState() const { return m_bStopAtStartState; }
		void SetStopAtStartState(bool val) { m_bStopAtStartState = val; }
		bool GetStopAtCurrentState() const 
        {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            const StateData& nowStateData = GetNowStateData();
            return NOWSTATE_FLAG( STOP_AT_CURRENT_STATE );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return m_bStopAtCurrentState;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }
		bool GetFocusCameraAtCurrentState() const 
        { 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            const StateData& nowStateData = GetNowStateData();
            return NOWSTATE_FLAG( FOCUS_CAMERA_AT_CURRENT_STATE );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return m_bFocusCameraAtCurrentState; 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }
		bool GetApplyCinematicUI() const { return  m_bIsApplyCinematicUI; }
		void SetApplyCinematicUI(bool bVal) { m_bIsApplyCinematicUI = bVal; }
#endif //MODIFY_DUNGEON_STAGING

		bool GetShowSubBossName() const { return m_bShowSubBossName; }
		void SetShowSubBossName(bool val) { m_bShowSubBossName = val; }

		bool GetHasBossGage() const { return m_bHasBossGage; }
		void SetHasBossGage( const bool bShow_ ) { m_bHasBossGage = bShow_; }

		USHORT GetNumOfGage() const { return m_usNumOfGage; }
		void SetNumOfGage( const USHORT usNumOfGage_ ) { m_usNumOfGage = usNumOfGage_; }

		const std::wstring& GetBossNameTexture() const { return m_wstrBossNameTexture; }
		void SetBossNameTexture(std::wstring val) { m_wstrBossNameTexture = val; }
		//void AddWeaponAttackCollisionBoxForNPC( Weapon* pWeapon );
		//void RemoveAllWeaponAttackCollisionBoxForNPC();
		CKTDXDeviceXSkinMesh* GetMotion() const { return m_pMotion; }

		// manual ai targeting func related
		void SetEnableLuaTargetingFunc(bool val);
		void SetLuaTargetingFunc( const std::string& val);
		void SetTargetUnitType( int val );
#ifdef FIX_LINK_ATTACK
		void SetTargetUnitUID( UidType tartgetUID );
#else
		void SetTargetUnitUID( int tartgetUID );
#endif

		void SetTargetUnit( CX2GameUnit *pUnit );
		int	GetTargetUnitUID();
		CX2GUUser* GetTargetUser();
#ifdef FINALITY_SKILL_SYSTEM
		CX2GUNPC* GetTargetNPC();
#endif //FINALITY_SKILL_SYSTEM

		void SetTimerRestart( int iIndex );
		float GetTimerElapsedTime( int iIndex );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        void SetTimerPause( int iIndex );
        void SetTimerResume( int iIndex );
        bool GetTimerResumed( int iIndex );
        bool TimerEventTrigger( int iIndex, float fTime );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		bool GetSelfResurrection() const { return m_bSelfResurrection; }
		void SetSelfResurrection(bool val) { m_bSelfResurrection = val; }



		bool GetSelfDestruction() const { return m_bSelfDestruction; }
		void SetSelfDestruction(bool val) { m_bSelfDestruction = val; }

		bool IsSelfDestructing()
		{
			if( true == m_bSelfDestruction && 
				m_fSelfDestructionTime != -1.f &&
				m_SelfDestructionTimer.elapsed() < m_fSelfDestructionTime )
			{
				return true;
			}

			return false;
		}

		void StartSelfDestruction( float fTime )
		{
			SetSelfDestruction( true );
			m_fSelfDestructionTime = fTime;
			m_SelfDestructionTimer.restart();
		}

		void StopSelfDestruction()
		{
			SetSelfDestruction( false );
		}

        bool GetAttackFront()
        {
            return GetNPCFrameData().unitCondition.bAttackerFront;            
        }

		virtual int GetRandomInt( int iOffset = 0 ) { return CKTDXRandomNumbers::GetRandomInt( GetNPCSyncData().m_usRandomTableIndex + iOffset ); }
		virtual float GetRandomFloat( int iOffset = 0 ) { return CKTDXRandomNumbers::GetRandomFloat( GetNPCSyncData().m_usRandomTableIndex + iOffset ); }

        virtual void SetCanPassUnit( const bool bFlag_ ) { m_NPCFrameData.stateParam.bCanPassUnit = bFlag_; }

		void	ChangeModelDetail( int detailPercent );

		bool	GetIsFallDowning();
    
        void    SetAttackedUnit();

		void SetRiderNPCUID( int npcUID ) { m_RiderNpcUID = npcUID; }
		int GetRiderNPCUID() { return m_RiderNpcUID; }

		void SetRidingNPCUID( int npcUID ) { m_RidingNPCUID = npcUID; }
		int GetRidingNPCUID() { return m_RidingNPCUID; }

		//void	InitPosByLineData( int iLineIndex );		// obsolete!!

		LeapData& GetLeapData() { return m_LeapData; }


		bool IsImmuneToEnchant( CX2EnchantItem::ENCHANT_TYPE enchantType );
		
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        void AddExtraAbilityID( CX2UnitManager::NPC_ABILITY_ID npcAbilityID );
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		void AddExtraAbilityID( CX2UnitManager::NPC_ABILITY_ID npcAbilityID ) { m_vecExtraAbilityID.push_back( npcAbilityID ); }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
		void AddEnchantType( CX2EnchantItem::ENCHANT_TYPE enchantType ) { m_vecEnchantType.push_back( enchantType ); }
		void ApplyEnchantMonster( const KAttribEnchantNpcInfo& kAttribEnchantNpcInfo_ );
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		static void AppendEnchantMonsterResourcesToDeviceList( CKTDXDeviceDataList& listInOut_, const KAttribEnchantNpcInfo& kAttribEnchantNpcInfo_  );
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

		void ShowSnatchCureEffect();

		void SetUnitFadeStart(CKTDGXRenderer::RENDER_TYPE blend, float r, float g, float b, float a, bool bBlend = false, bool bFade = true);
		void SetUnitFadeMove(float fEndTime, float r, float g, float b, float a);
		void SetUnitFadeScale(float fEndTime, float x, float y, float z);
		void SetUnitColor(float r, float g, float b, float a, bool bBlend = false, D3DBLEND srcBlend = D3DBLEND_SRCALPHA, D3DBLEND destBlend = D3DBLEND_DESTALPHA);
		bool GetNeverMove() const { return m_bNeverMove; }
		
#ifdef VERIFY_STAT_BY_BUFF
		virtual void TransformScale( const PROTECT_VECTOR3& vScale_ );
#else	// VERIFY_STAT_BY_BUFF
		virtual void TransformScale( const D3DXVECTOR3& vScale_ );
#endif // VERIFY_STAT_BY_BUFF

		// note!! 아마게돈 블레이드를 사용하는 몬스터를 위한 예외 코드
		void						DeleteArmagedonBlade();
		void						CreateArmagedonBlade( float fTime );

		// 참고할 몬스터 설정 / 자신을 참고하는 몬스터 설정
		void AddConsultNpc(CX2GUNPC *pConsultNpc);
		void AddConsultedNpc(CX2GUNPC *pConsultedNpc);
		void ClearConsultNpc(CX2GUNPC *pConsultNpc);
		void ClearConsultedNpc(CX2GUNPC *pConsultNpc);
		CX2GUNPC *GetConsultNpc(CX2UnitManager::NPC_UNIT_ID unitID);
//{{oasis:김상윤////2009-10-7////
		CX2GUNPC *GetOnlyConsultNPC_LUA(CX2UnitManager::NPC_UNIT_ID unitID);
		CX2GUNPC *GetOnlyConsultedNPC_LUA(CX2UnitManager::NPC_UNIT_ID unitID);
		CX2GUNPC *GetConsultNearNPC_LUA(D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange = 999999 );
		
//}}oasis:김상윤////2009-10-7////

#ifdef SKIN_LIGHTFLOW
        void    SetLightFlowType(CKTDGXRenderer::RENDER_TYPE fType) { GetXSkinAnimPtr()->SetFlowType(fType); }
        void    SetLightFlowMin(float fValue) { GetXSkinAnimPtr()->SetFlowMin(fValue); }
        void    SetLightFlowMax(float fValue) { GetXSkinAnimPtr()->SetFlowMax(fValue); }
        void    SetLightFlowSpeed(float fValue) { GetXSkinAnimPtr()->SetFlowSpeed(fValue); }
        void    SetLightFlowWide(float fValue) { GetXSkinAnimPtr()->SetWide(fValue); }
        void    SetLightFlowColor(D3DXCOLOR fValue) { GetXSkinAnimPtr()->SetFlowColor(fValue); }
        void    SetLightFlowPoint(float fPoint) { GetXSkinAnimPtr()->SetNowPoint(fPoint); }
#endif

#ifdef BONUS_DROP
		void	SetBonusDrop(bool val){ m_bBonusDrop = val; }
		bool	GetBonusDrop(){ return m_bBonusDrop; }
#endif		

		CX2GameUnit* GetLastAttackGameUnit() { return ( null != m_optrLastGameUnitWhoAttackToMe ? m_optrLastGameUnitWhoAttackToMe.GetObservable() : NULL ); }
		CX2GUNPC* GetLastAttackUnit() { return ( null != m_optrLastGameUnitWhoAttackToMe ? static_cast<CX2GUNPC*>( m_optrLastGameUnitWhoAttackToMe.GetObservable() ) : NULL ); }
		CX2GameUnit* GetHitter();

#ifdef USER_HOLD
		void    SetUserHoldReset();

//{{ kimhc // 2010.7.12 // 여러명의 유저를 HOLD 할 수 있도록 함
	#ifdef ADD_SET_USER_HOLD_OFFSET_ARGUMENT	 /// 유저 위치를 특정 본 위치로 설정( 잡기 )하는 함수에 OffSet 인자 추가
		void	SetUserHold(bool val, const char* pBoneName, bool bHoldOnlyOneUser = true , D3DXVECTOR3 vHoldOffSet = D3DXVECTOR3( 0.f, 0.f, 0.f ) );
	#else  ADD_SET_USER_HOLD_OFFSET_ARGUMENT
		void	SetUserHold(bool val, const char* pBoneName, bool bHoldOnlyOneUser = true );
	#endif ADD_SET_USER_HOLD_OFFSET_ARGUMENT

		bool	GetUnHoldWhenRevenged() const { return m_bUnHoldWhenRevenged; }
		void	SetUnHoldWhenRevenged(bool val) { m_bUnHoldWhenRevenged = val; }
//}} kimhc // 2010.7.12 // 여러명의 유저를 HOLD 할 수 있도록 함
		bool	GetUserHold() { return m_bHold; }
#endif



#ifdef  TEST_GROUP_GRAP
		bool GetUserGrap(){ return m_bUserGrap; }
		bool IsLostGrap();
		void SetUserGrap( bool val, D3DXVECTOR3 vGrapPosition);
		void SetUserGrapReset();
#endif	TEST_GROUP_GRAP

		virtual void SetEnableDash( bool bEnableDash, ENABLE_DASH_STATE eState = EDS_END );		// 대시를 가능하게 하거나 불가능하게 한다
		
		int GetUnitLevel() const;

#ifdef SKILL_30_TEST

		float GetEntangleTrapAttackDuration() const { return m_fEntangleTrapAttackDuration; }
		void SetEntangleTrapAttackDuration(const float val) { m_fEntangleTrapAttackDuration = val; }
#endif SKILL_30_TEST


		int GetRealatedSkillLevel() const { return m_iRelatedSkillLevel; }
		void SetRealatedSkillLevel(const int val) { m_iRelatedSkillLevel = val; }



//{{oasis:김상윤////2009-10-7////
		int GetTargetUnitUID_LUA(); // ok

		void SetGroupAICommand_LUA(CX2GUNPC::GROUP_AI_COMMAND val) { m_GroupAIData.m_eGroupAICommand = val; }
		CX2GUNPC::GROUP_AI_COMMAND GetGroupAICommand_LUA() { return m_GroupAIData.m_eGroupAICommand; }
		
		void SetGroupAIGroundTarget( bool val) { m_GroupAIData.m_bIsGroundTarget = val; }
		bool IsGroupAIGroundTarget() { return m_GroupAIData.m_bIsGroundTarget; }

		void SetGroupAITargetPos(D3DXVECTOR3 pos) { m_GroupAIData.m_vGroupAITargetPos = pos; }
		D3DXVECTOR3 GetGroupAITargetPos() { return m_GroupAIData.m_vGroupAITargetPos; }

		void SetGroupAITargetGameUnit( CX2GameUnit* pGroupAiTargetGameUnit_ ) { m_GroupAIData.m_optrGroupAiTargetGameUnit = pGroupAiTargetGameUnit_; }
		CX2GameUnit* GetGroupAITargetGameUnit() { return ( null != m_GroupAIData.m_optrGroupAiTargetGameUnit ? m_GroupAIData.m_optrGroupAiTargetGameUnit.GetObservable() : NULL ); }

		void SetGroupAISelfCommandTransit_LUA(bool val) { m_GroupAIData.m_bIsReadySelfCommandTransit = val; }
		bool GetGroupAISelfCommandTransit_LUA() { return m_GroupAIData.m_bIsReadySelfCommandTransit; }

		void COMMAND_HOLD_LUA();
		void COMMAND_ATTACK_MOVE_LUA( CX2GameUnit* pGroupAiTargetGameUnit_ );
		void COMMAND_FORCED_MOVE_TARGET_LUA( CX2GameUnit* pGroupAiTargetGameUnit_ );
		void COMMAND_FORCED_MOVE_GROUND_LUA(D3DXVECTOR3 targetPos);
		void COMMAND_SKILL_LUA (CX2GUNPC::GROUP_AI_COMMAND command, CX2GameUnit* pGroupAiTargetGameUnit_ );

		void SetPrevAIMessage_LUA( CX2GUNPC::STATE_CHANGE_TYPE val);

		void SetAItoMainAIData_LUA();
		void SetAItoSubAIData_LUA();
		bool IsRightTarget_LUA(CX2GUNPC *pStartNpc, D3DXVECTOR3 pos);			// 특정위치가 나의 오른쪽인지 왼쪽인지 판별한다.


//}}oasis:김상윤////2009-10-7////

#ifdef MONSTER_REFLECT_DAMAGE_DATA		
		void Process_ReflectDamage(CX2DamageManager::DamageData* pDamageData, CX2GameUnit* pAttackerUnit);

		void SetReflectDamageOnPhysicalDamage( bool bVal );
		void SetReflectDamageOnMagicalDamage( bool bVal );
#endif MONSTER_REFLECT_DAMAGE_DATA

#ifdef RIDING_MONSTER
		void	SetRideState(RIDE_STATE eRideState){ m_eRideState = eRideState; }
		int		GetRideState(){ return m_eRideState; }
		int		GetRideType(){ return m_eRideType; }
#endif RIDING_MONSTER

		//{{ megagame : 박교현 : [2010-04-06]
#ifdef SERV_SECRET_HELL
		void	DamageReact_HitCount( CX2DamageManager::DamageData* pDamageData );

		int		GetHitCount_LUA() const				{ return m_HitCount; }
		void	SetHitCount_LUA(int iHitCount)		{ m_HitCount = iHitCount; }

		int		GetStateID_LUA( const char* state );
		int		GetNextStateID_LUA() const;

			//{{ kimhc // 2010-04-16 // 공격력, 방어력 등 핸들링 할 수 있도록 함
		void	SetAtkPhysic_LUA( float fValue )
		{
			AccessStat().AccessStat().m_fAtkPhysic	= fValue; 
			m_AdditionalPhysicAttack.SetBasicStatValue( fValue );
		}

		void	SetAtkMagic_LUA( float fValue )	
		{ 
			AccessStat().AccessStat().m_fAtkMagic	= fValue; 
			m_AdditionalMagicAttack.SetBasicStatValue( fValue );
		}

		void	SetDefPhysic_LUA( float fValue ) 
		{ 
			AccessStat().AccessStat().m_fDefPhysic	= fValue; 
			m_AdditionalPhysicDefence.SetBasicStatValue( fValue );
		}

		void	SetDefMagic_LUA( float fValue )	
		{ 
			AccessStat().AccessStat().m_fDefMagic	= fValue; 
			m_AdditionalMagicDefence.SetBasicStatValue( fValue );
		}

		float	GetAtkPhysic_LUA() const { return GetPhysicAttackValue(); }
		float	GetAtkMagic_LUA() const { return GetMagicAttackValue(); }
		float	GetDefPhysic_LUA() const { return GetPhysicDefenceValue(); }
		float	GetDefMagic_LUA() const	{ return GetMagicDefenceValue(); }

		void	ReInitAtkAndDef_LUA();

		void	SetDefaultAnimSpeed_LUA( float fValue ) { m_fDefaultAnimSpeed = fValue; }
		float	GetDefaultAnimSpeed_LUA() const { return m_fDefaultAnimSpeed; }
			//}} kimhc // 2010-04-16 // 공격력, 방어력 등 핸들링 할 수 있도록 함

#endif SERV_SECRET_HELL
		//}} megagame : 박교현 : [2010-04-06]

		//{{ kimhc // 2010.6.29 // 스테이트들 중에서 특정 데미지 스테이트를 다른 스테이트로 변경
		void ChangeDamageStateID_LUA( char* lpstrSrcState, char* lpstrDestState );	// small_front, big_back 등의 여러 데미지 스테이트 들 중에서 A라는 stateID를 가진것을 B라는 stateID로 변경
		void ChangeDyingStateID_LUA( char* lpstrSrcState, char* lpstrDestState );	// dying_front, dying_back 등의 여러 dying 스테이드 들 중에서 A라는 stateID를 가진것을 B라는 stateID로 변경
		void ChangeStateID( int* pDamageState, int iSrcStateID, int iDestStateID )
		{
			if ( *pDamageState == iSrcStateID )
				*pDamageState = iDestStateID;
		}
		//}} kimhc // 2010.6.29 // 스테이트들 중에서 특정 데미지 스테이트를 다른 스테이트로 변경

		//{{ kimhc // 2010.6.29 // 피격 모션등은 실행되고, 받는 데미지는 모두 0가 되게하는 기능으로 몬스터 state table 내에서 사용
#ifdef DAMAGE_ZERO
		bool GetZeroDamage() const { return m_bZeroDamage; }
		void SetZeroDamage(bool val) { m_bZeroDamage = val; }
#endif DAMAGE_ZERO
		//}} kimhc // 2010.6.29 // 피격 모션등은 실행되고, 받는 데미지는 모두 0가 되게하는 기능으로 몬스터 state table 내에서 사용

		//{{ kimhc // 2010-07-21 // 현재 스테이트에서 HP가 0이 되어도 죽었다고 판단하지 않게 할 수 있는 기능
#ifdef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool GetNeverDieThisState() const 
        { 
            const StateData& nowStateData = GetNowStateData();
            return NOWSTATE_FLAG( NEVER_DIE_THIS_STATE );
        }
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool GetNeverDieThisState() const { return m_bNeverDieThisState; }
		void SetNeverDieThisState(bool val) { m_bNeverDieThisState = val; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		//}} kimhc // 2010-07-21 // 현재 스테이트에서 HP가 0이 되어도 죽었다고 판단하지 않게 할 수 있는 기능

//#ifdef TRANSFORMER_TEST
//
//		void SetInputData( InputData* pInputData )
//		{
//			if( NULL == pInputData )
//				return; 
//
//			m_InputData = *pInputData;
//		}
//
//#endif TRANSFORMER_TEST

#ifdef DAMAGE_HISTORY
		void SetDamageHistory(bool bVal) { m_bDamageHistory = bVal; }
		bool GetDamageHistory() { return m_bDamageHistory; }
#endif

		float GetDefaultPowerRate() const { return m_fDefaultPowerRate; }
		void SetDefaultPowerRate(const float val) { m_fDefaultPowerRate = val; }

#ifdef VELDER_BOSS_5
		void SetCloaking();
		void SetUnCloaking();
#endif VELDER_BOSS_5

		//{{ kimhc // 2010.8.7 // 무조건 NPC에게 카메라가 가도록 하는 기능
		bool GetFocusCameraForce() const { return m_bFocusCameraForce; }
		void SetFocusCameraForce(bool bVal) { m_bFocusCameraForce = bVal; }
		//}} kimhc // 2010.8.7 // 무조건 NPC에게 카메라가 가도록 하는 기능

#ifdef MONSTER_STATE_LIST_TEST
		void EnumerateStateAndAnimationName( std::vector<std::string>& vecStateName, std::vector< std::wstring>& vecAnimationName );
#endif MONSTER_STATE_LIST_TEST

		//{{ JHKang / 강정훈 / 2010/12/06 / 데미지 글씨 출력 관련
#ifdef NEW_SKILL_2010_11
		void SetDamageOutScreen( IN bool val_ ) { m_bDamageOutScreen = val_; }
#endif NEW_SKILL_2010_11
		//}} JHKang / 강정훈 / 2010/12/06 / 데미지 글씨 출력 관련

		//{{ JHKang / 강정훈 / 2010/12/20 / 피 흡수 이펙트(SetDie 참고)
#ifdef SEASON3_MONSTER_2010_12
		void SetDrainHPSeq( IN CKTDGParticleSystem::CParticleEventSequenceHandle hSeq_ ){ m_hSeqDrainHPEffect = hSeq_; }
		bool GetReflectMagic()
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( m_v2ReflectMagic.y > 0.f && m_v2ReflectMagic.x <= -1.0f )
				return true;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( m_NPCFrameData.stateParam.m_vReflectMagic.y > 0.f && m_NPCFrameData.stateParam.m_vReflectMagic.x <= -1.0f )
				return true;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return false;
		}
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / 강정훈 / 2010/12/20 / 피 흡수 이펙트(SetDie 참고)

#ifdef GUNPC_TOGGLE_APPLY_MOTION_OFFSET
		void SetApplyMotionOffset( bool bApply );
#endif GUNPC_TOGGLE_APPLY_MOTION_OFFSET

		//{{ JHKang / 강정훈 / 2011/02/21 / 소환된 유닛이 NPC(몬스터)에게 준 데미지
#ifdef DUNGEON_RANK_NEW
		UidType		GetOwnerGameUnitUID()	{ return (null == m_optrOwnerGameUnit ? -1 : m_optrOwnerGameUnit->GetUnitUID() ); }
#ifdef  X2OPTIMIZE_UNITTYPE_BUG_FIX
		CX2GUUser* GetOwnerGameUnit();
		void SetOwnerGameUnit( CX2GUUser* pOwnerGameUnit_ );
#else   X2OPTIMIZE_UNITTYPE_BUG_FIX
		CX2GameUnit* GetOwnerGameUnit() { return ( null != m_optrOwnerGameUnit ? m_optrOwnerGameUnit.GetObservable() : NULL ); }
		void SetOwnerGameUnit( CX2GameUnit* pGameUnit_ ) 
		{ 
			if ( NULL != pGameUnit_ )
				m_optrOwnerGameUnit = pGameUnit_; 
		}
#endif  X2OPTIMIZE_UNITTYPE_BUG_FIX
#endif
		//}}

		bool DoScaleHeadBone();

#ifdef FIX_LINK_ATTACK
		CX2GUUser* GetNearestGUUser();// {	return ( null != m_optrNearestGameUnit ? m_optrNearestGameUnit.GetObservable() : NULL ); }
		void SetNearestGUUser( CX2GameUnit* pNearestGUUser )
		{
			if ( NULL != pNearestGUUser && pNearestGUUser->GetGameUnitType() == GUT_USER )
				m_optrNearestGUUser = pNearestGUUser;
		}
#endif

		void	SetCriticalRate( const float fValue_ ) { DISPLAY_ERROR( L"버프로 바꾸자" ); }

#ifdef SERV_INSERT_GLOBAL_SERVER
		bool						GetIsEliteMonster(); // oasis907 : 김상윤 [2011.4.19]
#endif SERV_INSERT_GLOBAL_SERVER

		void SetForceInvincible( float fTime, float fShowTime )
		{
			if( fTime > 0.f )
				m_fForceInvincibleTime = fTime;
			if( fShowTime > 0.f )
				m_fShowInvincibleTime = fShowTime;
		}
		void CreateArmagedonBladeByBot( float fTime );
		void ActiveSkillShow_LUA( char* pBackFaceTexName, float fSizeX, float fSizeY, float fPosX, float fPosY, float fPosZ );
		void ActiveSkillShow( const WCHAR* pBackFaceTexName, D3DXVECTOR2 size, D3DXVECTOR3 pos );

#ifdef SEASON3_MONSTER_2011_7
		bool						GetAbsorbMagicState() { return m_bAbsorbMagicState; }
		void						SetRageCountMax(int _iVal){ m_RageCountMax = _iVal; }
		void						SetEnableAfterImageAlways(bool _bVal){ m_bAfterImageAlways = _bVal; }
		void						ChangeAfterImageColor(D3DXCOLOR _AfterImageColor);
#endif SEASON3_MONSTER_2011_7

#ifdef SERV_PVP_NEW_SYSTEM
		virtual wstring GetUnitName();
#endif

#ifdef	BATTLE_FIELD_TEST
	bool GetCreatedBySyncNot() const { return m_bCreatedBySyncNot; }
	void SetCreatedBySyncNot(bool val) { m_bCreatedBySyncNot = val; }

	bool GetReceiveP2P() const { return m_bReceiveP2P; }
	void SetReceiveP2P( bool val ) { m_bReceiveP2P = val; }
#endif	BATTLE_FIELD_TEST

	void SetIsComeBackState( const bool bIsComeBackState_ ) { m_bIsComeBackState = bIsComeBackState_; }
	bool GetIsComeBackState() const { return m_bIsComeBackState; }

	virtual CX2GageUI* CreateGageUI( const CX2GageData* pOwnerGageData_ );

	float			GetEnchantResist( CX2EnchantItem::ENCHANT_TYPE enchantType ); 

	void			GetBuffInfo( OUT vector<KNpcUnitBuffInfo>& vecNpcUnitBuffInfo_ ) const;
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	virtual bool	DidReceiveFirstSyncPacket() 
	{ 
		//host일 경우에는 항상 최초 패킷을 받은 것처럼 동작
		if( IsLocalUnit() )
			m_bReceiveFirstSyncPacket = true;
		return m_bReceiveFirstSyncPacket; 
	}
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	virtual bool	DidReceiveFirstSyncPacket() const { return IsLocalUnit() || 0 < m_PacketRecvCount; }
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	virtual void	HyperModeBuffEffectStart();
	virtual float	GetHyperModeTime() const { return GetMaxHyperModeTime() * m_HyperModeCount; }

	virtual bool CanApplyBuffToGameUnit() const;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual const std::vector<BUFF_TEMPLET_ID>& GetVecImmunityAtThisState() const   { return GetNowStateData().m_vecImmunityAtThisState; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool IsSuperArmor() const;

	virtual CKTDGXSkinAnimPtr	GetSkinAnimPtr() const { return m_pXSkinAnim; }
	virtual void AnimStop();
	virtual void AnimPlay() {}	
	virtual void SetStateToNowAnimation( const CKTDGXSkinAnim::XSKIN_ANIM_STATE eAnimState_ )
	{
		m_pXSkinAnim->SetState( eAnimState_ );
	}
#ifdef GUIDE_QUEST_MONSTER
	void SetQuestMonster(bool bVal){ m_bIsQuestMonster = bVal; } 
#endif //GUIDE_QUEST_MONSTER

#ifdef SUMMON_MONSTER_CARD_SYSTEM		//몬스터 카드로 소환한 NPC인지 구분
		//몬스터 카드로 소환한 NPC인지 구분
		bool  GetIsMonsterCard() const {  return m_bISMonsterCard; }			
		void  SetIsMonsterCard( bool bISMonsterCard ) { m_bISMonsterCard = bISMonsterCard; }

		int   GetSummmonEndState()					  { return m_SummonEndState; }			//소환 해제 State 반환

		float GetSummonTime()						  { return m_fSummonTime; }				//InitComponent에서 받아온 해당 NPC의 소환 시간
#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		bool  GetIsAllyEventMonster()					  {  return m_bISAllyEventMonster; }			
		void  SetIsAllyEventMonster( bool bISAllyEventMonster ) { m_bISAllyEventMonster = bISAllyEventMonster; }
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_ARME_DIMENSION_WITCH
		bool GetAbsorbMagicSpecial() { return m_bAbsorbMagicSpecial; }
#endif

#ifdef CREATE_ALLY_NPC_BY_MONSTER
		void CreateAllyNpcByMonster_LUA( CX2UnitManager::NPC_UNIT_ID eNpcID);
		void SetCoolTimeSummonCard_LUA();
#endif //CREATE_ALLY_NPC_BY_MONSTER

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		CX2GameUnit* GetNearestUnitPosByOwnerUnit( float fDistance );					/// NPC를 소환한 유저와 가장 가까이 있는 적 유닛을 반환

		void		 AttackResultByType( CX2DamageManager::DamageData &pDamageData );	/// 박격포 공격이 지휘관의 표식에 적용되도록 하기 위하여 추가

		D3DXVECTOR3	 GetNearestActiveNPCPos();											/// 현재 Active 중인 NPC의 위치를 반환
#endif SERV_CHUNG_TACTICAL_TROOPER
		
#ifdef DELETE_NPC_BY_SUB_STAGE_END
		bool GetDeleteNPCBySubStageEnd() { return m_bDeleteNPCBySubStageEnd; }
#endif DELETE_NPC_BY_SUB_STAGE_END	

#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
		/// 머리 크기 설정
		void ScaleHead();
		void RestoreHead();

		/// 각 유닛마다 특정 버프가 실행 또는 해제 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 시전중이라는 플래그 등...)
		virtual void SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
		virtual void UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );

		/// 해당 몬스터를 AllyUnit이 죽였다는 처리를 하도록, DamagedMap에 설정하는 함수
		void		 SetKillNPCByAllyUID();
#endif ADD_VELDER_ACADEMY_EVENT_MONSTER

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용 날짜: 2013-04-12
		/// 몬스터 생성 타입
		CX2GUNPC::NPC_CREATE_TYPE	GetNPCCreateType() const { return m_eNPCCreateType; }
		void						SetNPCCreateType(CX2GUNPC::NPC_CREATE_TYPE val) { m_eNPCCreateType = val; }

		/// 몬스터 이름 출력 여부
		bool GetNoRenderName() const { return m_bNoRenderName; }
		void SetNoRenderName(bool val) { m_bNoRenderName = val; }
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef USE_UNIT_FOG_RENDER_PARAM //JHKang
		void SetFogNPC( float fNearX_, float fFarX_, float fNearY_, float fFarY_, float fDensity_, D3DXCOLOR fogColor_ )
		{
			m_bUnitFog		= true;
			m_fFogNearX		= fNearX_;
			m_fFogNearY		= fNearY_;
			m_fFogFarX		= fFarX_;
			m_fFogFarY		= fFarY_;
			m_fFogDensity	= fDensity_;
			m_cFogColor		= fogColor_;
		}
#endif //USE_UNIT_FOG_RENDER_PARAM

		virtual void		ApplyWorldLightColor( OUT CKTDGXRenderer::RenderParam* pRenderParam_ );

//{{ Public Function End

#ifdef FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER

	virtual float	GetAdditionalAttackDamage( const CX2DamageManager::DamageData* pAttackDamageData );


#endif // FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	int GetBossGroupID() const { return m_uiBossGroupID; }
	void SetBossGroupID(int val) { m_uiBossGroupID = val; }

	bool GetIsMainBoss() const { return m_bIsMainBoss; }
	void SetIsMainBoss(bool val) { m_bIsMainBoss = val; }
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

//#ifdef FIX_NPC_SHOW_OBJECT
//	bool GetShowObjectCurrentState() const { return m_bShowObjectCurrentState; }
//#endif //FIX_NPC_SHOW_OBJECT

#ifdef ADDED_NPC_REMAINING_WHEN_SUBSTAGE_CLEAR
	// Stage, SubStage 가 클리어 되더라도 해당 NPC 를 남길 것인가?
	bool GetIsRemainingNpcWhenStageCleard () const { return m_bIsRemainingNpcWhenStageCleard; }
	void SetIsRemainingNpcWhenStageCleard (bool val) { m_bIsRemainingNpcWhenStageCleard = val; }
#endif // ADDED_NPC_REMAINING_WHEN_SUBSTAGE_CLEAR

//{{ Protected Function Start

#ifdef  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE
    void    MarkNPCUnitDieNotReceived();
#endif  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        void                        SendReserve( int iCount );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        int     GetUserUnitRelativePos( int iUnitIndex )    { return  m_kNonHostReaction.GetLocalRelativePos( iUnitIndex ); }

#ifdef ENTRY_MONSTER_MESSAGE // 김태환
		/// 몬스터 등장시 화면 중앙에 출력하는 메시지 처리
		void SetEntryMonsterMessage( IN const char* pMonsterName_, IN float fVisibleTime_ );
#endif // ENTRY_MONSTER_MESSAGE

#ifdef FIELD_BOSS_RAID
		virtual bool GetIsBosRaidNPC() const { return (MG_BOSS_NPC == m_eMonsterGrade); }
		virtual bool GetIsMiddleBosRaidNPC() const { return (MG_MIDDLE_BOSS_NPC == m_eMonsterGrade); }
		void SetMonsterGrade( MONSTER_GRADE eMonsterGrade_ );

		// 특별히 조건 처리하지않고 보스NPC여부로 검사. // 추후 필요하다면 확장해서 사용해야 함
		virtual bool GetIsImmuneDebuff() const { return GetIsBosRaidNPC(); }
		virtual bool GetIsImmuneAllEDT() const { return GetIsBosRaidNPC(); }

		void SetFollowTargetUnitBone( UidType iTargetUnitUID_, const CHAR* pTargetBoneName_, bool bIsUser_ );
		void ActiveFollowTargetUnitBone( bool bVal ){ m_sFollowTargetUnitData.m_bUseFollowTargetUnitBone = bVal; }

		void CreateEltrionEvenlyLockonMissile();
#endif // FIELD_BOSS_RAID

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
        static void     ResetReactionSimulationCounter()    { KNonHostReaction::ResetReactionSimulationCounter(); }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD

#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef  ADD_CAN_PASS_ALLY_UNIT	
        bool                        CanPassAllyUnit();
#endif  ADD_CAN_PASS_ALLY_UNIT	

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        bool    EstimateFrameAccumPercent( float fPercent );
        static bool    EstimateFrameOneshotPercent( float fPercent );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef SERV_BATTLE_FIELD_BOSS 
		//필드 보스 레이드 로그 작업.
		void AddFieldBossRaidLog();
#endif //SERV_BATTLE_FIELD_BOSS 

#ifdef ADD_RENA_SYSTEM //김창한
		const CX2DamageManager::DamageRelateSkillData& GetRelateSkillData(){ return m_RelateSkillData; }
		void SetRelateSkillData( const CX2DamageManager::DamageRelateSkillData& sData_ ){ m_RelateSkillData = sData_; }
#endif //ADD_RENA_SYSTEM

	protected:
		virtual void				InitDevice();
		virtual void				InitSystem();
		virtual void				InitComponent();

		///////////////////////////////////////////////////////////

		virtual void				InitAI();
		virtual void				InitMotion();
		virtual void				InitEnchantData();
		virtual void				InitState();


		void	CreateAndSetAinmXSkinMesh();

#ifdef GRAPPLING_TEST
		virtual void				GrappledEventProcess();
		virtual void				GrappledPhysicProcess( bool bFuture );
#endif GRAPPLING_TEST


		void NeverMovingPhysicProcess();
		virtual void				PhysicProcess();

		//void PhysicProcess_FastUpwardWind( CKTDGLineMap* pLineMap );
		void PhysicProcess_Bungee( CKTDGLineMap* pLineMap );
	

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        int     ProcessCanPushNPCUnit( CX2GUNPC* pGameUnit_, const D3DXVECTOR2& vMyUnitMinMaxY_, const D3DXVECTOR2& vMyUnitMinMaxYCur_
                                , OUT D3DXVECTOR3& vPos_  );
        int     ProcessCanPushUserUnit( CX2GUUser* pGameUnit_, const D3DXVECTOR2& vMyUnitMinMaxY_
                                , OUT D3DXVECTOR3& vPos_, int iDirection, int iLocalDirection
                                , bool bReservePosReliable );
        bool    ProcessCanPassUnit( const CX2GameUnit* pGameUnit_, const D3DXVECTOR3& vPos_ );
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
	    void	ProcessCanPushUnit( const CX2GameUnit* pGameUnit_, const D3DXVECTOR2& vMyUnitMinMaxY_, OUT D3DXVECTOR3& vPos_ );
        void	ProcessCanPassUnit( const CX2GameUnit* pGameUnit_, OUT D3DXVECTOR3& vPos_ );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION


		void						DoStateEndStart();
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        void						DoFrameMove( bool bNoFrameMove = false );
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		void						DoFrameMove();
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION


#ifdef NO_ALL_KILL_HACKING_TEST
		void CheckHackingOnDyingStart();
#endif NO_ALL_KILL_HACKING_TEST


		virtual	void				DyingStart();

		virtual void				CommonStateStartHost();
		virtual void				CommonStateStart();

		

		virtual void				CommonFrameMoveHost();
		void						CommonFrameMoveHost_ExtraDamage();

		virtual void				CommonFrameMove();
#ifdef SKILL_30_TEST
		virtual void CommonFrameMoveStateAbnormalityFrozen();
#endif SKILL_30_TEST
#ifdef EDT_WATER_HOLD_TEST
		virtual void CommonFrameMoveStateAbnormalityWaterHold();
#endif EDT_WATER_HOLD_TEST
#ifdef EVE_ELECTRA
		virtual void CommonFrameMoveStateAbnormalityFlashBang();
#endif EVE_ELECTRA


		void 						CommonFrameMove_TimedEvent();
		void 						CommonFrameMove_ExtraDamage();
		void 						CommonFrameMove_Rage();
		void 						CommonFrameMove_Invincible();
		void 						CommonFrameMove_Particle();
		void 						CommonFrameMove_DieParticle();
		void 						CommonFrameMove_MindFlagParticle();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        void                        CommonFrameMove_EffectSet( float fNowAnimationTime );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		virtual void				CommonCameraMove();
		virtual void				CommonEventProcess();
#ifdef  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE
        void                        CommonEventProcessNonHost();
#endif  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE
		virtual void				CommonStateEndHost();

		virtual void				CommonStateEnd();
		virtual void				CommonRender_Draw();

//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
//		void						SendReserve();
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK			

#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
		void LinkJumpPhysicProcess( double fTime, float fElapsedTime );
		void LinkJumpStart();
		void LinkJumpEnd();
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST



#ifdef REFACTORING_BY_TOOL_TEAM
		void Send_NPC_UNIT_DIE_REQ();
#endif REFACTORING_BY_TOOL_TEAM





		
		
	
		float						GetCriticalPercent( BYTE byAttackType, float fExtraCritical = 0.f );
		
		float						GetActionSpeedUpValue();
		bool						GetIsAlwaysSuperArmor();
		bool						GetIsHeavyMonster();
		wstring						GetExtraAbilityDesc( bool bFront );	//접두어 인지 접미어인지?

		
		void ResetAnimSpeed();
		void DamageDoubleAttack();
		

#ifndef SERV_9TH_NEW_CHARACTER // 김태환
		/// 유저도 쓰기 위해, CX2GameUnit으로 이동
		bool IsRightTarget(D3DXVECTOR3 vPos);			// 특정위치가 나의 오른쪽인지 왼쪽인지 판별한다.
#endif // SERV_9TH_NEW_CHARACTER
		

		//{{ oasis907 : 김상윤 [2010.4.21] // 
		void DoStateRage();
		//}

		//{{ kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 몬스터 레벨 표시)
#ifdef SERV_SECRET_HELL
		bool GetRenderLv() const { return m_bRenderLv; }
		void SetRenderLv( bool val ) { m_bRenderLv = val; }
#endif SERV_SECRET_HELL
		//}} kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 몬스터 레벨 표시)
		
		//{{ kimhc // 2010.7.12 // 여러명의 유저를 HOLD 할 수 있도록 함
		bool GetHoldOnlyOneUser() const { return m_bHoldOnlyOneUser; }
		void SetHoldOnlyOneUser(bool val) { m_bHoldOnlyOneUser = val; }	
		//}} kimhc // 2010.7.12 // 여러명의 유저를 HOLD 할 수 있도록 함

		//{{ JHKang / 강정훈 / 2010/12/20 / 피 흡수 이펙트(SetDie 참고)
#ifdef SEASON3_MONSTER_2010_12
		void CommonFrameMove_DrainHPParticle();		
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / 강정훈 / 2010/12/20 / 피 흡수 이펙트(SetDie 참고)
		
		//{{ 허상형 : [2011/2/27/] //	월드 미션(항상 Ally 상태인 NPC 처리)
#ifdef SERV_INSERT_GLOBAL_SERVER
		bool IsAllyNPCID( CX2UnitManager::NPC_UNIT_ID nNpcID );
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 허상형 : [2011/2/27/] //	월드 미션(항상 Ally 상태인 NPC 처리)
		
		inline void AddDamagedMapAndPositionToDiePacket( OUT KEGS_NPC_UNIT_DIE_REQ& kPacket_ );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        static void ParseStopTime( StateData& OutStateData_, KLuaManager& luaManager_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void        StopTime_StateStart();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		virtual void DamageDataChangeProcess();		

#ifdef INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        static void ParsingBuffFactorID( StateData& OutStateData_, KLuaManager& luaManager_ );
        void    CommonFrameMove_InsertBuffFactor( float fNowAnimationTime );

#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void        ParsingBuffFactorID( KLuaManager& luaManager_ );
        void CommonFrameMove_InsertBuffFactor();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        
#endif // INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

#ifdef FIELD_BOSS_RAID
		void FollowTargetUnitBone_FrameMove();
#endif // FIELD_BOSS_RAID




#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        void AdjustFrameSkipRate( float fElapsedTime );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		//{{ Protected Function End

//{{ Protected Variable Start
		CX2NPCAI*										m_pAI;

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        std::vector<StateRuntime>                       m_vecStateRuntime;
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		NPCStateData									m_NPCStateData;
		map<char,NPCStateData>							m_StateList;
		//map<wstring,NPCStateData>						m_StateListByName;
        std::map<string,char>                           m_StateNameIDMap;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION


//{{ robobeg : 2008-10-28		
		//wstring											m_StateTableName;
		//wstring											m_StateTableNameFuture;
//}} robobeg : 2008-10-28		
    
//{{oasis:김상윤////2009-10-7////
		GroupAIData										m_GroupAIData;
//}}oasis:김상윤////2009-10-7////

#ifdef MONSTER_STATE_CHANGE_CHEAT
		string											m_strErrorMonsterStateName;

#endif MONSTER_STATE_CHANGE_CHEAT


#ifdef MONSTER_REFLECT_DAMAGE_DATA
//{{ oasis907 : 김상윤 [2010.4.19] // 
		CX2DamageManager::DamageData					m_ReflectDamageData;

		ConditionReflectDamage							m_ConditionReflectDamage;
//}}
#endif MONSTER_REFLECT_DAMAGE_DATA

#ifdef HP_BASED_MONSTER_RAGE
//{{ oasis907 : 김상윤 [2010.4.21] // 
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool											m_bCannotRage;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		int												m_iHPRageCount;										
		int												m_iRageHPPercent;
//}}
#endif HP_BASED_MONSTER_RAGE

		NPCFrameData									m_NPCFrameData;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION

        char                                            m_cMindFlagBackup;
        int                                             m_iLua_CommonCameraMoveRefID;
        int                                             m_iLua_CommonFrameMoveRefID;
        int                                             m_iLua_AttackedFuncRefID;

#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION

		NPCFrameData									m_NPCFrameDataBack;
//{{ kimhc // 2010.8.8 // 무조건 NPC에게 카메라가 가도록 하는 기능
		wstring											m_CommonCameraMove;		// 모든 스테이트에서 공통으로 적용
//}} kimhc // 2010.8.8 // 무조건 NPC에게 카메라가 가도록 하는 기능
        wstring                                         m_CommonFrameMove;
        wstring                                         m_AttackedFunc;

#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        struct NPCFrameSync
        {
            DWORD                   m_dwFrameMoveCount;
            KDYNAMIC_UNIT_NPC_SYNC  m_sync;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
            bool                    m_bMiniSync;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

            NPCFrameSync()
                : m_dwFrameMoveCount(0)
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                , m_bMiniSync( false )
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
            {
            }

            bool    operator < ( const NPCFrameSync& rhs_ ) const   { return m_dwFrameMoveCount < rhs_.m_dwFrameMoveCount; }
            bool    operator < ( DWORD dwrhs_ ) const           { return m_dwFrameMoveCount < dwrhs_; }
            friend bool    operator < ( DWORD dwlhs_, const NPCFrameSync& rhs_ ) { return dwlhs_ < rhs_.m_dwFrameMoveCount; }
    
        };
        UidType                                         m_pidHostUID;
        DWORD                                           m_dwLastProcessedFrameMoveCount;

		struct KLastStateChangeParts
		{
			bool m_bValid;
			KTWO_PARTS m_kLastStateChangeParts;

			void Init()
			{
				m_bValid = false;
				m_kLastStateChangeParts.stateChangeNum = 0;
				m_kLastStateChangeParts.stateModifyNum = 0;
			}

			bool IsValid() { return m_bValid; }
			void SetValue( const KTWO_PARTS& kParts ) { m_bValid = true; m_kLastStateChangeParts = kParts; }
			const KTWO_PARTS& GetValue() { return m_kLastStateChangeParts; }
		};

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		KLastStateChangeParts m_kLastStateChangeParts_Recv;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

		boost::circular_buffer< NPCFrameSync >			m_cbufReceiveDynamicPacket;

		int												m_iSendReserveCount;
		DWORD										    m_dwSendPacketFrame;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		vector<KXPT_UNIT_NPC_SYNC>						m_ReceivePacketBufList;
//		bool											m_bSendReserve;
//		bool											m_bSendReserveStateChange;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		bool											m_bImmediate;

#ifdef  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE
        enum    ENPC_UNIT_DIE_STATE
        {
            eNPCUnitDieState_NONE = 0,
            eNPCUnitDieState_REQ = 1,  
            eNPCUnitDieState_NOT = 2,
        };
        ENPC_UNIT_DIE_STATE                             m_eNPCUnitDieState;
        float                                           m_fNPCUnitDieStateTimer;
        bool                                            m_bClientHoldPosition;

#endif  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION

        class   KNonHostReaction
        {
        public:

            enum    EReactionStateType
            {
                REACTION_STATE_NONE,
                REACTION_STATE_FIRST,
                REACTION_STATE_CONTINUE,
                REACTION_STATE_HOST_FIRST,
                REACTION_STATE_HOST_CONTINUE,
            };

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
            enum    EReactionSubstateType
            {
                REACTION_SUBSTATE_NORMAL,
                REACTION_SUBSTATE_WAIT,
                REACTION_SUBSTATE_CHECKED,
            };
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD

            KNonHostReaction();
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
            ~KNonHostReaction();
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
            void    Init();
            void    DoStateEndStart( bool bLocalUnit, const NPCFrameData& m_NPCFrameData, const PhysicParam& m_PhysicParam );
            void    FixStatesForLocalUnitOrReaction_NONE( bool bLocalUnit );
            void    FixStatesForReaction_NOT_FIRST( bool bLocalUnit );
            void    UpdateNowStateStartSpeed( const NPCFrameData& m_NPCFrameData, const PhysicParam& m_PhysicParam );
            void    DoFrameMove_SimulateReaction_FIRST( NPCFrameData& m_NPCFrameData, PhysicParam& m_PhysicParam,
                        boost::circular_buffer< NPCFrameSync >& m_cbufReceiveDynamicPacket,
                        DWORD& m_dwLastProcessedFrameMoveCount, bool& bUpdatePositionBackup,
                        float& fNowHP, float& fNowMP,
                        bool bSuperArmorFactor, float fAnimSpeedAdjustFactor, float fUnitHeight, const std::vector<StateData>& vecStateData
                        );
            bool    DoFrameMove_SimulateReaction( 
                        NPCFrameData& m_NPCFrameData, PhysicParam& m_PhysicParam,
                        bool& bUpdatePositionBackup, bool& bStateChange, bool& bForceModifyConfig, bool& bNotUpdateDefenceRandomTableIndex,
                        const KDYNAMIC_UNIT_NPC_SYNC& xPT_UNIT_NPC_SYNC, const std::vector<StateData>& vecStateData );
            void    DoFrameMove_StateChangeByHost( bool& bUpdatePositionBackup, const KDYNAMIC_UNIT_NPC_SYNC& xPT_UNIT_NPC_SYNC );

            void    ReserveReceivePos( const D3DXVECTOR3& vReservedReceivePos, int iLastTouchLineIndex, bool bFootOnLine );
            void    ResetReservedReceivePos() { m_bReservedReceivePos = false; }
            bool    IsReceivePosReserved() const    { return m_bReservedReceivePos; }
            const D3DXVECTOR3&  GetReservedReceivePos() const { return m_vReservedReceivePos; }
            int     GetReservedReceiveLineIndex() const { return m_iReservedReceiveLineIndex; }
            bool    GetReservedReceiveFootOnLine() const       { return m_bReservedReceiveFootOnLine; }

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
            void    PrepareAnalyzeReactionState( int iNextReactionState );
            void    ReportOrPrepareAnalyzeReactionState( bool bLocalUnit, bool bRelaxNPCReactionStateCheck, int iNextReactionState );
            void    ReportAnalyzeReactionState( DWORD dwReactionStartFrame );
            void    ReportAnalyzeReactionState( bool bCheckCompatibleReactionState, const KDYNAMIC_UNIT_NPC_SYNC& xPT_UNIT_NPC_SYNC, const std::vector<StateData>& vecStateData );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS

            bool    IsNextStateRection_NONE() const         { return m_eNextStateReaction == REACTION_STATE_NONE; }
            bool    IsNowStateReaction_NONE() const         { return m_eNowStateReaction == REACTION_STATE_NONE; }
            bool    IsNowStateReaction_NONE_OR_WAIT() const { return m_eNowStateReaction == REACTION_STATE_NONE || GetNowStateReactionWait() == true; }
            bool    IsNowStateReaction_FIRST() const        { return m_eNowStateReaction == REACTION_STATE_FIRST; }
            bool    IsNowStateReaction_FIRST_OR_CONTINUE() const { return m_eNowStateReaction == REACTION_STATE_FIRST || m_eNowStateReaction == REACTION_STATE_CONTINUE; }
            bool    GetNowStateReactionWait() const         
            { 
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
                return m_eNowSubstateReaction == REACTION_SUBSTATE_WAIT;
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
                return m_bNowStateReactionWait; 
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
            }
            int     GetNowStateReactionUnitIndex() const    { return m_iNowStateReactionUnitIndex; }
            bool    GetNowStateRelaxReactionCheck() const { return m_bNowStateRelaxReactionCheck; }
          
            void    SetNextStateReaction_NONE();
            void    SetNextStateReaction_FIRST( bool bRelaxReactionCheck,
                        int  iAttackerUnitIndex,
                        const D3DXVECTOR2& vReactionYMinMax );
            void    SetNextStateReaction_CONTINUE( bool bWait );
            void    BuffPositionHistory( bool bLocalUnit, const D3DXVECTOR3& vPosition );
            void    ClearBuffPositionHistory()          { m_cbuffPositionHistory.resize( 0 ); }
            void    ClearBuffNonHostSimState()          { m_cbufNonHostSimState.resize( 0 ); }

            void    ResetSimulationTolerance()          { m_fNonHostSimulationTolerance = 0.f; }
            void    UpdateSimulationTolerance();
            float   GetSimulationTolerance() const      { return m_fNonHostSimulationTolerance; }

            void    AdjustDamageReactionSpeed( NPCFrameData& m_NPCFrameData, PhysicParam& m_PhysicParam, bool bIsRightBackup, float fWalkSpeed, float fLagLatency );

            void    ResetRelativePosAndTimers();
            void    UpdateRelativePosAndTimers( DWORD dwRelativePos );
            void    UpdateRelativePosAndTimer( int iUnitIndex, int iPos );
            DWORD   GetRelativePosFlags() const             { return m_dwRelativePos; }
            int     GetRelativePos( int iUnitIndex ) const  { return  ( m_dwRelativePos >> ( iUnitIndex * 2 ) ) & 0x3; }
            int     GetLocalRelativePos( int iUnitIndex ) const 
            { 
                return ( iUnitIndex >= 0 && iUnitIndex < MAX_GAME_USER_UNIT_NUM ) ? m_acLocalRelativePos[ iUnitIndex ] : 0;
            }
            void    ResetLocalRelativePos();
            void    ResetLocalRelativePosAndTimers();
            void    IncrementLocalAndRelativePosTimers( float fElapsedTime );

            void    PhysicProcess_RetrieveRelativePosInfo( bool bLocalUnit, int iUnitIndex, bool bMyUnit, 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                       int iNumAccumFrames,
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                        OUT int& iLocalDirection, OUT int& iDirection, OUT bool& bReservePosReliable );
            void    PhysicProcess_UpdateRelativePos( bool bLocalUnit, const NPCFrameData& m_NPCFrameData, float fUnitHeight
                    , int iTeam, CX2Room::TEAM_NUM eAllyTeam );

            unsigned    UpdateLastSentMiniInfo( int iTargetUnitIndex );
            bool        CheckLastSentMiniInfo(); 
            USHORT  GetDefenceRandomTableIndex() const { return m_usDefenceRandomTableIndex; }
            void    SetDefenceRandomTableIndex( USHORT usRandomTableIndex ) { m_usDefenceRandomTableIndex = usRandomTableIndex; }
            USHORT  GetShuffledDefenceRandomTableIndex() const { return ( m_usDefenceRandomTableIndex + 5678 ) % CKTDXRandomNumbers::RANDOM_NUMBER_COUNT; }
            int     EvaluateConditionRandomRate( int iTableIndex, int iStateChangeNum, const ConditionTableData& conditionTableData, 
                        const vector<std::pair<D3DXVECTOR3,int> >& m_EventIntervalList );

            int     GetTargetUnitIndex() const { return m_iTargetUnitIndex; }
            void    SetTargetUnitIndex( int iTargetUnitIndex )  { m_iTargetUnitIndex = iTargetUnitIndex; }
            static bool    CheckCompatibleReactionState( char state1, char state2, const std::vector<StateData>& vecStateData );

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
            static void     ResetReactionSimulationCounter()    { ms_uReactionSimulationCounter = 0; }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD

        private:

            static bool    _CheckCompatibleReactionCondition( float fSpeedX1, float fSpeedX2 );
            static bool    _CheckInvincibleState( char StateID, float fTolerance, float fAnimSpeedAdjustFactor, const std::vector<StateData>& vecStateData );

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
            void            _UpdateNowStateReaction( EReactionStateType eState, EReactionSubstateType eSubstate )
            {
                if ( IsNowStateReaction_FIRST_OR_CONTINUE() == true )
                {
                    if ( eState != REACTION_STATE_FIRST && eState != REACTION_STATE_CONTINUE 
                        && ms_uReactionSimulationCounter != 0 )
                        ms_uReactionSimulationCounter--;
                }
                else
                {
                    if ( eState == REACTION_STATE_FIRST || eState == REACTION_STATE_CONTINUE )
                        ms_uReactionSimulationCounter++;
                }
                m_eNowStateReaction = eState;
                m_eNowSubstateReaction = eSubstate;
            }
            static bool             _IsNumReactionSimulationMax();
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD

        private:

            struct  KNonHostSimState
            {
                KTWO_PARTS          m_kStateChangeParts;
                char                m_cState;
                EReactionStateType  m_eReactionStateType;
                int                 m_iReactionStateUnitIndex;
                float               m_fStartSpeedX;
                DWORD               m_dwReactionStartFrame;
                D3DXVECTOR2         m_v2ReactionYMinMax;
                KNonHostSimState()
                    : m_cState(0)
                    , m_eReactionStateType( REACTION_STATE_NONE )
                    , m_iReactionStateUnitIndex( -1 )
                    , m_fStartSpeedX( 0.f )
                    , m_dwReactionStartFrame( 0 )
                    , m_v2ReactionYMinMax(0.f,0.f)
                {
                }
            };

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
            static unsigned                                 ms_uReactionSimulationCounter;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD

            EReactionStateType                              m_eNextStateReaction;
            bool                                            m_bNextStateRelaxReactionCheck;
            bool                                            m_bNextStateReactionWait;
            int                                             m_iNextStateReactionUnitIndex;
            D3DXVECTOR2                                     m_v2NextStateReactionYMinMax;
            EReactionStateType                              m_eNowStateReaction;
            bool                                            m_bNowStateRelaxReactionCheck;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
            EReactionSubstateType                           m_eNowSubstateReaction;
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
            bool                                            m_bNowStateReactionWait;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
            int                                             m_iNowStateReactionUnitIndex;
            boost::circular_buffer<KNonHostSimState>        m_cbufNonHostSimState;
            bool                                            m_bNowStateStartSpeedX;
            float                                           m_fNowStateStartSpeedX;
            DWORD                                           m_dwNowReactionStartFrame;
            float                                           m_fNonHostSimulationTolerance;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
            int                                             m_iAnalyzeReactionState;
            DWORD                                           m_dwAnalyzeReactionFrameIndex;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
            boost::circular_buffer<D3DXVECTOR3>             m_cbuffPositionHistory;
            bool                                            m_bReservedReceivePos;
            D3DXVECTOR3                                     m_vReservedReceivePos;
            int                                             m_iReservedReceiveLineIndex;
            bool                                            m_bReservedReceiveFootOnLine;

            DWORD                                           m_dwRelativePos;
            float                                           m_afRelativePosTimers[MAX_GAME_USER_UNIT_NUM];
            char                                            m_acLocalRelativePos[MAX_GAME_USER_UNIT_NUM];
            float                                           m_afLocalRelativePosTimers[MAX_GAME_USER_UNIT_NUM];

            DWORD                                           m_dwLastSentRelativePos;
            int                                             m_iLastSentTargetUnitIndex;
            unsigned                                        m_uLastSentMiniSyncCount;

            USHORT                                          m_usDefenceRandomTableIndex;
            int                                             m_iTargetUnitIndex;
        };

        KNonHostReaction                                    m_kNonHostReaction;

        bool    _IsReactionState( char StateID );
        bool    _IsReactionWaitState( char StateID );
        bool    _IsDyingState( char StateID );
        
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

		int												m_HardLevel;
		//HardLevelData									m_HardLevelData;
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
        const CX2UnitManager::NPCUnitInfo*              m_pNPCUnitInfo;
        const InitData*                                 m_pInitData;
//#else   X2OPTIMIZE_NPC_LUASPACE_SHARING
//		const CX2UnitManager::NPCUnitTemplet*			m_pNPCTemplet;
//#endif  X2OPTIMIZE_NPC_LUASPACE_SHARING

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		map<float,bool>									m_EventTimeStamp;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		//유닛 컴포넌트
		CKTDXDeviceXSkinMesh*							m_pMotion;		
		CKTDGXSkinAnimPtr								m_pXSkinAnim;		


		CKTDXDeviceXET*									m_pAniXET;
		CKTDXDeviceXET*									m_pChangeTexXET;
		CKTDXDeviceXET*									m_pMultiTexXET;
		D3DXVECTOR2										m_TexStage0Speed;
		D3DXVECTOR2										m_TexStage1Speed;
		D3DXVECTOR2										m_TexStage2Speed;

		D3DXVECTOR3										m_ORGPos;

		//CKTDGXSkinAfterImage*							m_pAfterImage;
	

		STATE_CHANGE_TYPE								m_AIMessage;
		STATE_CHANGE_TYPE								m_UnUseAIMessage;
		bool											m_bAIIsRight;
		bool											m_bAIEnabled;

		float											m_fTimePauseNPCAI;

		float											m_fCoolTime;
		int												m_StartState;
		int												m_RebirthState;

#ifdef SERV_PVP_NEW_SYSTEM
		int												m_WinState;
		int												m_LoseState;
#endif

		int												m_RageState;
		int												m_SiegeState;
		int												m_SmallDamageLandFront;		//# NPC가 지상에 있는 상태에서, 앞에서 Small데미지를 주는 타격 당했을때의 상태 지정
		int												m_SmallDamageLandBack;		//# NPC가 지상에 있는 상태에서, 뒤에서 Small데미지를 주는 타격 당했을때의 상태 지정
		int												m_SmallDamageAir;			//# NPC가 공중에 떠있는 상태에서, Small데미지를 주는 타격 당했을때의 상태 지정
#ifdef AI_FLY
		int												m_SmallDamageAirFront;
		int												m_SmallDamageAirBack;
#endif
		int												m_BigDamageLandFront;
		int												m_BigDamageLandBack;		
		int												m_BigDamageAir;
#ifdef AI_FLY
		int												m_BigDamageAirFront;
		int												m_BigDamageAirBack;
#endif
		
		//int												m_DownDamageLandFront;	
		//int												m_DownDamageLandBack;				
		//int												m_DownDamageAir;

		int												m_UpDamage;
		int												m_FlyDamageFront;			
		
		
		int												m_FlyDamageBack;

#ifdef RIDING_MONSTER
		int												m_RideDamageFront;
		int												m_RideDamageBack;

		int												m_AirFallState;
#endif RIDING_MONSTER
		int												m_RevengeAttack;
		int												m_DamageFlushLandFront;	
		int												m_DamageFlushLandBack;	
		int												m_DamageFlushAir;
		int												m_DamageRevenge;

		int												m_DefenceState;
		int												m_DisabledState;
		
		int												m_DyingLandFront;	
		int												m_DyingLandBack;
		int												m_DyingSky;
#ifdef DIE_FLY
        int                                             m_DyingFly;
#endif
		int												m_SelfDestructionState;

        int                                             m_SmallDamageLandGuard;
        int                                             m_BigDamageLandGuard;
        int                                             m_DownDamageLandGuard;

#ifdef RIDING_MONSTER
		int												m_RideState;
#endif RIDING_MONSTER
		int												m_ImmuneSkillState;

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
		vector<std::pair<D3DXVECTOR3,int> >              m_EventIntervalList;
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		vector<int>										m_vecIndexedState;
		vector<D3DXVECTOR3>								m_EventIntervalList;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool											m_bDyingEnd;
#ifdef BONUS_DROP
		bool											m_bEscapeEnd;
#endif
		bool											m_bUseMindFlag;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		bool											m_bDyingInSky;

		bool											m_bEnableCondtionTableEventProcess;
		bool											m_bEnableEventProcess;
		bool											m_bNeverMove;
		bool											m_bIsActiveMonster;
		D3DXVECTOR3										m_AddRotateDegree;
		D3DXVECTOR3										m_AddPos;
		
		int												m_KeyCode;
		bool											m_bFocusCamera;
		bool											m_FlagList[10];

		int												m_IntList[10];
		D3DXVECTOR3										m_VecList[10];
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
		double											m_NumberList[10];
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hMajorSeqList[10];
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hMinorSeqList[10];
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqDieEffect;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqStickShake;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqStandUpAttackMark;

//{{ kimhc // 2010.4.19 // 비밀던전 작업
#ifdef SERV_SECRET_HELL
		CX2EffectSet::Handle								m_hEffectSetHandle[10];
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.19 // 비밀던전 작업

#ifdef SKILL_CASH_10_TEST
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqDrainParticle;
#endif

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle                           m_ahDamageEffect[10];
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*						m_pDamageEffect[10];
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        struct  TIME_ELAPSED
        {
            float   m_fTime;
            float   m_fTimeBackup;
            bool    m_bResumed;

            TIME_ELAPSED()
                : m_fTime( 0.f )
                , m_fTimeBackup( 0.f )
                , m_bResumed( true )
            {
            }

            void    Restart()
            {
                m_fTime = 0.f;
                m_fTimeBackup = 0.f;
            }

            void    OnFrameMove( float fElapsedTime )
            {
                m_fTimeBackup = m_fTime;
                if ( m_bResumed == true )
                    m_fTime += fElapsedTime;
            }
            void    Pause()             {   m_bResumed = false;}
            void    Resume()            {   m_bResumed = true; }
            bool    GetResumed() const  { return m_bResumed; }
            float   GetTime()        { return m_fTime; }
            bool    EventTimer( float fTime ) const { return m_fTimeBackup < fTime && fTime <= m_fTime; }
            bool    Predict_EventTimer( float fTime, float fElapsedTime ) const { return m_bResumed == true && m_fTime < fTime && fTime <= m_fTime + fElapsedTime; }
            bool    Predict_EventTimer( const std::vector<float>& vecfTime_, float fElapsedTime_ ) const
            {
                if ( m_bResumed == false || vecfTime_.empty() == true )
                    return false;
                std::vector<float>::const_iterator iterGreaterThanBeforeTime = std::upper_bound( vecfTime_.begin(), vecfTime_.end(), m_fTime );
                if ( iterGreaterThanBeforeTime == vecfTime_.end()
                    || m_fTime + fElapsedTime_ < *iterGreaterThanBeforeTime )
                    return false;
                return true;
            }//EventTimer()
        };//
        TIME_ELAPSED                                    m_aTimerList[10];
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		CKTDXTimer										m_TimerList[10];
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE



		CX2GameUnitoPtr									m_optrLastGameUnitWhoAttackToMe;		/// 마지막으로 자신을 공격한 유닛


		float											m_fOutLineWidthScale;

		float											m_fMindFlagHeight;			// head_bone에서 상대 위치
		bool											m_bDamagedThisFrameMove;
#ifdef SEASON3_MONSTER_2010_12
		CX2DamageManager::REACT_TYPE					m_eDamageTypeThisFrameMove;
#endif
		bool											m_bShowOnMiniMap;

		//{{ megagame : 박교현 : [2010-04-06]
#ifdef SERV_SECRET_HELL
		int												m_HitCount;
#endif SERV_SECRET_HELL
		//}} megagame : 박교현 : [2010-04-06]


		int												m_RageCountMax;		
		float											m_fRageTimeMax;
		int												m_RageCount;
		float											m_fRageTime;

		float											m_fElapsedTimeLastGuide;
		int												m_HyperModeCount;

		bool											m_bSiegeMode;					// 현재 시즈 상태인지 아닌지
		bool											m_bAddForceDownGageThisState;
		float											m_fLastDamagePassTime;	/// 최근 타격 받은 후 지난 시간... 근데 이건 도대체 왜 따로 선언 되어 있는가..?

		D3DXCOLOR										m_RageModeColor;
		bool											m_bRageModeColorToRed;
		float											m_fRageModeColorChangeTime;

		bool											m_bUseGuardCollisionBox;		// nasod_guard 처럼 방패에 이름이 guard로 시작하는 특별한 collision box를 사용해서 방어 상태를 처리할지, 이 값이 false이면 앞, 뒤만 구분해서 방어 상태 처리
		bool											m_bFallDown;
        bool                                            m_bDamageDown;
        bool                                            m_bSkyDie;
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		D3DXVECTOR3										m_vStateAddRotate;
		bool 											m_bSwapUnitWidthHeight;			// 캐릭터가 누워서 unit_width와 height를 바꿔야 하는 상태인지
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		int												m_PacketSendCount;
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		bool											m_bReceiveFirstSyncPacket;
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		int												m_PacketRecvCount;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		bool											m_bFollowHitterLost; // 잡고 있던 유닛이 죽거나 사라지면 이 값이 true 가 된다		
		int												m_iFollowHitter;
		CX2GameUnit::GAME_UNIT_TYPE						m_eHitterUnitType;
		UidType											m_HitterUID;
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool											m_bAtackFollowHitter;
		//{{ JHKang / 강정훈 / 2010/12/15 / 몬스터를 캐릭터 얼굴 앞에 붙이기
#ifdef SEASON3_MONSTER_2010_12
		bool											m_bAttackFollowHitterPosFront;
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / 강정훈 / 2010/12/15 / 몬스터를 캐릭터 얼굴 앞에 붙이기
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


		bool											m_bForceBillBoard;
		float											m_fAnimWaitTime;

#ifdef SUBBOSS_MONSTER_KILL_EFFECT
		//{{ oasis907 : 김상윤 [2010.4.29] // 
		bool											m_bPlayBossKillEffect;
		//}}
#endif SUBBOSS_MONSTER_KILL_EFFECT
		bool											m_bShowBossName;
		bool											m_bShowSubBossName;
		bool											m_bHasBossGage;
		USHORT											m_usNumOfGage;
		
		std::wstring									m_wstrBossNameTexture;

		float											m_fPrevHPGage;
		int												m_DefenceRate;

		Invisible										m_Invisible;

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		vector<AnimSpeed>								m_vecAnimSpeed;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE



		bool											m_bSelfResurrection;
		float											m_fSelfResurrectionTime;
		CKTDXTimer										m_SelfResurrectionTimer;
		int												m_iSelfResurrectionCount;
		int												m_iSelfResurrectionHPPercent;


		bool											m_bSelfDestruction;
		float											m_fSelfDestructionTime;
		CKTDXTimer										m_SelfDestructionTimer;
		vector<AttachedParticle*>						m_vecpAttachedParticle;


		float											m_fDefaultAnimSpeed;		/// NPC 스크립트의 InitComponent 정하는 전체적인 기본 AnimSpeed 1.f or else
		float											m_fBornAnimSpeedRate;		// 엘리트 몬스터 혹은 속성, 아이템등의 효과에 의해 애니메이션 속도가 몬스터가 태어날 때 부터 바뀌었는지
		float											m_fAnimSpeedRate;			// 속성 데미지등에 의해 애니메이션 속도가 느려지는 경우, m_fAnimSpeed에 곱해진다.
		
		float											m_fAnimSpeed;				/// 각 스테이트 별로 지정할 수 있는 AnimSpeed (지정되어 있지 않으면 그 스테이트는 m_fDefaultAnimSpeed 사용
		float											m_fTimedEventAnimSpeed;		/// 각 스테이트의 특정 Anim 시간대에 적용되는 스피드값
		bool											m_bColdSlowApplied;
		bool											m_bFrozenSlowApplied;

        bool                                            m_bGuard;

		bool											m_bGgobong;
		bool											m_bOneTimeCheckGgobong;
		float											m_fTimeGgobong;
		
		int												m_RiderNpcUID;	//이 엔피씨를 타는 엔피씨의 UID
		int												m_RidingNPCUID;	//타고있는 엔피씨 UID
		
		LeapData										m_LeapData;

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        vector< const CX2UnitManager::NPCExtraAbility* > m_vecExtraAbilityID;
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		vector< CX2UnitManager::NPC_ABILITY_ID >		m_vecExtraAbilityID;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
		vector< CX2EnchantItem::ENCHANT_TYPE >			m_vecEnchantType;

#ifdef DIE_FLY
        int                                             m_SpeedDyingFly;
        D3DXVECTOR3                                     m_vDieFly;
        bool                                            m_bDieFly;
        float                                           m_flyAngle;
#endif DIE_FLY

		bool											m_bAttributeEmotionFlag;	// 속성 이모션 표시할지 여부 결정(기본 true)

		bool											m_bUnitColor;			// 유닛 블랜드 칼라 지정
#ifdef BONUS_DROP
		bool											m_bBonusDrop;			// 보너스 드롭 아이템이 나올지
#endif
#ifdef ADD_BACKATTACK
		bool											m_bApplyBackAttack;
#endif

		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqAttribEnchant_Fire;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqAttribEnchant_Wind;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqSpecialAbility_Attack;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqSpecialAbility_Attack_Background;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqSpecialAbility_Defend;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqSpecialAbility_Defend_Background;





		// note!! 아마게돈 블레이드를 위한 특별 코드, 나중에 범용으로 바꾸자~
		float											m_fArmagedonBladeTime;
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hArmagedonBlade;		

		// Unit Fade
		bool											m_bUnitFade;
		float											m_fFadeEndTime;
		D3DXCOLOR                                       m_cFadeColor;

		bool											m_bFadeScale;
		float											m_fFadeScaleEndTime;
		D3DXVECTOR3										m_vFadeScale;
		D3DXVECTOR3										m_vFinalScale;

		// 참조몬스터 변수
		vector<CX2GUNPC	*>								m_vecConsultNpc;
		vector<CX2GUNPC *>								m_vecConsultedNpc;

//#ifdef MONSTER_DIE_PARTICLE_TEST
//		bool	m_bCreatedDieParticle;
//#endif MONSTER_DIE_PARTICLE_TEST

#ifdef AI_FLY
		float											m_fFlyHeight;
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool											m_bFixFlyHeight;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#endif	

#ifdef USER_HOLD
		bool											m_bHold;
		wstring											m_wstrHoldBone;

#ifdef ADD_SET_USER_HOLD_OFFSET_ARGUMENT
		D3DXVECTOR3										m_vHoldOffSet;			/// 유저 홀드 위치 오프셋 벡터
#endif ADD_SET_USER_HOLD_OFFSET_ARGUMENT
		
		//{{ kimhc // 2010-07-11 // 여러명의 유저를 HOLD 할 수 있도록 함
		vector<UidType>									m_vecHoldUserUID;
		bool											m_bHoldOnlyOneUser;
		bool											m_bUnHoldWhenRevenged; // 반경당했을 때 Hold를 안할 것인가?
		//}} kimhc // 2010-07-11 // 여러명의 유저를 HOLD 할 수 있도록 함

#endif	USER_HOLD

		bool											m_bShowName;

		std::vector< CX2SkillTree::SKILL_ID >			m_vecImmuneSkill;	// 유저가 특정 필살기를 사용하면 무적상태가 된다.

		float m_fEntangleTrapAttackDuration;		// 인탱글 트랩몬스터(NUI_ENTANGLE_TRAP_RENA)만 사용하는 변수, 인탱글 된 유저를 잡고 있는 시간

		bool m_bAfterImageAlways;
		float m_fAfterImageDepth;
		bool m_bAlwaysAlphaObject;



//#ifdef TRANSFORMER_TEST
//
//		InputData m_InputData;
//
//#endif TRANSFORMER_TEST




		float m_fDefaultPowerRate;

#ifdef MONSTER_NOT_CULL
		bool											m_bNotCull;
#endif MONSTER_NOT_CULL

//{{ kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 몬스터 레벨 표시)
#ifdef SERV_SECRET_HELL
		bool m_bRenderLv;
#endif SERV_SECRET_HELL
//}} kimhc // 2010.5.1 // 비밀던전 작업(비밀던전 몬스터 레벨 표시)

#ifdef FIX_FROZEN01
		bool			m_bDoneBackupSpeed;
		D3DXVECTOR2		m_vNowSpeedBackup;
		D3DXVECTOR2		m_vpassiveSpeedBackup;
#endif

//{{ kimhc // 2010.6.29 // 피격 모션등은 실행되고, 받는 데미지는 모두 0가 되게하는 기능으로 몬스터 state table 내에서 사용
#ifdef DAMAGE_ZERO
		bool			m_bZeroDamage;
#endif DAMAGE_ZERO
//}} kimhc // 2010.6.29 // 피격 모션등은 실행되고, 받는 데미지는 모두 0가 되게하는 기능으로 몬스터 state table 내에서 사용

//{{ kimhc // 2010-07-21 // 현재 스테이트에서 HP가 0이 되어도 죽었다고 판단하지 않게 할 수 있는 기능
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool			m_bNeverDieThisState;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
//}} kimhc // 2010-07-21 // 현재 스테이트에서 HP가 0이 되어도 죽었다고 판단하지 않게 할 수 있는 기능

//{{ kimhc // 2010.7.7 // 몬스터가 생성한 라인맵
#ifdef	LINE_MAP_CREATED_BY_MONSTER
		// 현재 몬스터가 생성한 라인맵이 있다면 그 라인맵의 인덱스들을 가지고 있는 벡터
		vector<int>		m_vecMonsterLineMapIndex;		
#endif	LINE_MAP_CREATED_BY_MONSTER
//}} kimhc // 2010.7.7 // 몬스터가 생성한 라인맵

#ifdef RIDING_MONSTER
		RIDE_STATE										m_eRideState;
		RIDE_TYPE										m_eRideType;
		int												m_RideTargetNPCID;
		int												m_RideTargetNPCUid;
		float											m_fSearchRadius;
		float											m_fRideRadius;
		CX2GameUnitoPtr									m_optrRidingGameUnit;
#endif RIDING_MONSTER

//{{ kimhc // 2010.8.7 // 무조건 NPC에게 카메라가 가도록 하는 기능
		bool											m_bFocusCameraForce;
//}} kimhc // 2010.8.7 // 무조건 NPC에게 카메라가 가도록 하는 기능
		
		//{{ JHKang / 강정훈 / 2010/12/06 / 데미지 글씨 출력 관련
#ifdef NEW_SKILL_2010_11
		bool	m_bDamageOutScreen;			/// 해당 유닛의 데미지 정보 출력을 할 것인가?
#endif NEW_SKILL_2010_11
		//}} JHKang / 강정훈 / 2010/12/06 / 데미지 글씨 출력 관련
		//{{ lucidash 2010.12.14 : 쉐도우 차져 관련 타격잡기.
#ifdef TEST_GROUP_GRAP
		bool											m_bUserGrap;		
		int												m_iGrapShakeCount;
		vector<CX2GameUnitoPtr>							m_vecGameUnitoPtr;
		
		D3DXVECTOR3										m_vGrapPosition;

#endif TEST_GROUP_GRAP
		//}} lucidash 2010.12.14 : 쉐도우 차져 관련 타격잡기.


#ifdef NEW_GRAPPLE_ATTACK

		bool							m_bGrappling;
		vector< UidType >				m_vecGrappleHitterUID;

#endif NEW_GRAPPLE_ATTACK

#ifndef X2OPTIMIZE_NPC_NONHOST_SIMULATION
#ifdef SUPER_ARMOR_TIME
		bool							m_bStateSuperArmor;
#endif
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

		//{{ JHKang / 강정훈 / 2010/12/20 / 피 흡수 이펙트(SetDie 참고)
#ifdef SEASON3_MONSTER_2010_12
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqDrainHPEffect;
#endif
		//}} 

		//{{ JHKang / 강정훈 / 2011/02/21 / 소환된 유닛이 NPC(몬스터)에게 준 데미지
#ifdef DUNGEON_RANK_NEW
		//UidType		m_OwnerUserUnitUID;	/// 유저 소환 몬스터일 경우 소환환 유저의 UID
		CX2GameUnitoPtr	m_optrOwnerGameUnit;	/// 유저 소환 몬스터일 경우 소환환 유저
#endif

#ifdef FIX_LINK_ATTACK
		CX2GameUnitoPtr	m_optrNearestGUUser;	/// 가장 가까이에 있는 유저(링커의 링크 공격 사용)
#endif
#ifdef INVINCIBLE_TO_NPC_KEYCODE
		int			m_iInvincibleToNPCKeyCode;
#endif INVINCIBLE_TO_NPC_KEYCODE

#ifdef SEASON3_MONSTER_2011_7
		bool		m_bAbsorbMagicState;
		bool		m_bAbsorbMagicMonster;
#endif SEASON3_MONSTER_2011_7
#ifdef SERV_ARME_DIMENSION_WITCH
		bool		m_bAbsorbMagicSpecial;
#endif

#ifdef ENTRY_MONSTER_MESSAGE // 김태환
		wstring		m_wstrEnterMonsterName;		/// 몬스터 출현시 출력할 내용
#endif // ENTRY_MONSTER_MESSAGE

//{{ Protected Variable End

		D3DXCOLOR	m_HyperModeColor;
		vector<CX2SubEquip*>		m_vecAttachMesh;

#ifdef SERV_PVP_NEW_SYSTEM
		CKTDXDeviceTexture*			m_pTextureServer;	
#endif

#ifdef VELDER_SECRET_DUNGEON
		D3DXVECTOR2					m_vOrgUnitSize;	//width, height
#endif

		int m_iDangerAlarm;

#ifdef INDEPENDENT_ANIMATION_TIME
		CKTDXCollision::CollisionDataListSet		m_SimpleCollisionListSet;
		CKTDXCollision::CollisionDataList			m_SimpleCollisionDataList;
#endif INDEPENDENT_ANIMATION_TIME

#ifdef SUMMON_MONSTER_CARD_SYSTEM
		bool m_bISMonsterCard;			//몬스터 카드로 소환되었는지에 대한 여부
		float m_fSummonTime;			//InitComponent에서 받아온 해당 NPC의 소환 시간
		int	  m_SummonEndState;			//소환 해제 State
#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef DELETE_NPC_BY_SUB_STAGE_END
		bool m_bDeleteNPCBySubStageEnd;	/// 서브 스테이지 종료시 해당 NPC 삭제 여부
#endif DELETE_NPC_BY_SUB_STAGE_END

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		bool m_bISAllyEventMonster;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

		wstring						m_wstrChampionName;	/// 챔피언 몬스터의 경우 붙는 수식어
		wstring						m_wstrLvAndNameAndPostFix;	 // LV, 접두어, Full Name, 접미사,
		D3DCOLOR					m_colorName;
		D3DCOLOR					m_colorOutLine;

#ifdef	BATTLE_FIELD_TEST
		bool						m_bCreatedBySyncNot;
		bool						m_bReceiveP2P;
#endif	BATTLE_FIELD_TEST

		bool						m_bIsComeBackState;

#ifdef MODIFY_DUNGEON_STAGING
		float						m_fShowBossNameDelayTime;	//보스 이름 연출 출력 지연 시간
		bool						m_bStopAtStartState;		//NPC의 스타트 스테이트 진행중엔 입력 불가 및 NPC AI 정지시키는 기능
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool						m_bStopAtCurrentState;		//NPC의 특정 스테이트에 유저 입력 불가, NPC AI 정지시키는 기능
		bool						m_bFocusCameraAtCurrentState;//NPC의 특정 스테이트에 NPC에게 카메라가 가도록 하는 기능
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool						m_bIsApplyCinematicUI;		 //카메라 포커싱 적용 됐을 때 시네메택 UI도 적용 시킬지 여부.
#endif //MODIFY_DUNGEON_STAGING
		wstring						m_wstrHyperBoostRightParticleName;
		wstring						m_wstrHyperBoostLeftParticleName;

		int							m_iRelatedSkillLevel;

#ifdef GUIDE_QUEST_MONSTER
		bool						m_bIsQuestMonster;			//퀘스트를 수행하기 위해 잡아야하는 몬스터에 대한 가이드 기능
#endif //GUIDE_QUEST_MONSTER

#ifdef ARA_CHARACTER_BASE
		USHORT					m_usSoundSet;		/// 사운드 Set 번호 선택
#endif

//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
		//bool						m_bLuaShared;
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		int					m_uiBossGroupID;			// 해당 몬스터의 Boss Group ID
		bool				m_bIsMainBoss;				// 해당 몬스터가 Main 급 중간 보스 몬스터 인가?
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS


#ifdef IMMUNITY_TIME_STOP		
		bool						m_bImmunityTimeStop;		//타임 스톱 면역 기능 추가
#endif //IMMUNITY_TIME_STOP

//#ifdef FIX_NPC_SHOW_OBJECT
//		bool						m_bShowObjectCurrentState;
//#endif //FIX_NPC_SHOW_OBJECT



#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-11
		NPC_CREATE_TYPE				m_eNPCCreateType;			/// 몬스터 생성 타입
		bool						m_bNoRenderName;			/// 몬스터 이름 출력 여부
		bool						m_bChangeRenderNameColor;	/// 몬스터 이름 색상 변경 여부
		D3DXCOLOR					m_colorRenderName;			/// 이름 색상
		D3DXCOLOR					m_colorRenderNameOutLine;	/// 변경시킬 이름의 아웃 라인 색상
#ifndef X2OPTIMIZE_ROBUST_NPC_ROTATION
		bool						m_bIsSetRightForce;			/// 현재 스테이트의 방향 강제 설정 여부
#endif  X2OPTIMIZE_ROBUST_NPC_ROTATION
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef ADDED_NPC_REMAINING_WHEN_SUBSTAGE_CLEAR
		bool						m_bIsRemainingNpcWhenStageCleard;	// Stage, SubStage 가 클리어 되더라도
																		// 해당 몬스터를 남길 것인가?
#endif // ADDED_NPC_REMAINING_WHEN_SUBSTAGE_CLEAR
#ifdef USE_UNIT_FOG_RENDER_PARAM //JHKang
		bool		m_bUnitFog;
		float		m_fFogNearX;
		float		m_fFogNearY;
		float		m_fFogFarX;
		float		m_fFogFarY;
		float		m_fFogDensity;
		D3DXCOLOR	m_cFogColor;
#endif //USE_UNIT_FOG_RENDER_PARAM

//{{ robobeg : 2013-06-12
// CX2GameUnit 에서 옮겨옴
#ifdef SECRET_VELDER
		//KProtectedType<bool>	m_bGlobalSuperArmor;		/// m_fGlobalTimeSuperArmor 시간동안 무조건 수퍼아머..
		KProtectedType<float>	m_fGlobalTimeSuperArmor;	/// 글로벌 수퍼아머 시간
#endif
//}} robobeg : 2013-06-12
#ifdef FIELD_BOSS_RAID
		FollowTargetUnitBone m_sFollowTargetUnitData;

		MONSTER_GRADE m_eMonsterGrade;
#endif // FIELD_BOSS_RAID

#ifdef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

        ESoundSetType                                   m_eSoundSetType;
        int                                             m_iSpeedFactorProgress;
        int                                             m_iEffectSetToPlayProgress;
#ifdef  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
        int                                             m_iStateInsertBuffFactorProgress;
#endif  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

// CX2GameUnit 에서 옮겨옴
		vector<ParticleEffectData*>						m_ParticleEffData;	/// NPC만 사용 하는 듯
		vector<MeshPlayerEffectData*>					m_MeshPlayerEffData;	/// NPC만 사용하는 것처럼 보이나 실제 사용 하는 곳은 없는 듯
        int                                             m_iCameraCrashProgress;

#ifdef SEASON3_MONSTER_2010_12
		D3DXVECTOR2				                        m_v2ReflectMagic;	// x: 발동시작시간, y:유지시간, z:확율이나, 앞뒤판단등을 위한 extra value(현재사용안함)
#endif

		std::vector<SoundPlayData>						m_vecSoundPlayLoop;		/// NPC 그중에서 벨더 비던의 글리터 등을 만드는 POT 에서만 현재 사용 중
		std::vector<CKTDXDeviceSound*>					m_vecLoopSound;		/// 각 스테이트에 있는 LOOP_SOUND를 NPC가 죽거나, STOP_LOOP_SOUND가 true 인 스테이트를 만났을 때 종료됨

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef ADD_RENA_SYSTEM //김창한
		CX2DamageManager::DamageRelateSkillData			m_RelateSkillData;
#endif //ADD_RENA_SYSTEM

#ifdef ALWAYS_EVENT_MONSTER_ALPHA_BLEND
		bool					m_bForceColor;
#endif ALWAYS_EVENT_MONSTER_ALPHA_BLEND

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
private:
	void _EncodeNpcSyncPos( USHORT& usPosX, USHORT& usPosY, USHORT& usPosZ, const D3DXVECTOR3& position, unsigned char lastTouchLineIndex );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


#ifdef  X2OPTIMIZE_NPC_DOSCALEHEADBONE
	CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_Head_Real;
#endif  X2OPTIMIZE_NPC_DOSCALEHEADBONE

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    int                                             m_iFrameAccumCounter;
    int                                             m_iFrameMoveIntervalCounter;
    float                                           m_fOneShotElapsedTime;

    struct  EVENT_PROCESS_ACCUM
    {
        int     m_iRate;
        int     m_iStateID;

        EVENT_PROCESS_ACCUM()
            : m_iRate( 0 )
            , m_iStateID( 0 )
        {
        }
    };//EVENT_PROCESS_ACCUM

    std::vector<EVENT_PROCESS_ACCUM>                m_vecEventProcessAccum;

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
private:
        enum    EDoubleAttackState
        {
            DOUBLE_ATTACK_NONE = 0,
            DOUBLE_ATTACK_BY_ME = 1,
            DOUBLE_ATTACK_BY_OTHER = 2,
        };
        EDoubleAttackState                              m_eDoubleAttack;
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

};

struct  CX2UnitManager::NPCInitData
{
	CX2GUNPC::InitData  m_init;
};//struct  CX2UnitManager::NPCInitData

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
inline bool    CX2GUNPC::EstimateFrameAccumPercent( float fPercent_ )
{
    if ( fPercent_ <= 0.f )
        return false;
    else if ( fPercent_ >= 100.f )
        return true;
    if ( m_iFrameAccumCounter <= 1 )
    {
        return  ( rand() * ( 100.f/RAND_MAX) ) < fPercent_;
    }
    float   fAccumProb = 0.f;
    float   fProb = fPercent_ * 0.01f;
    for( int i = 0; i < m_iFrameAccumCounter; i++ )
    {
        fAccumProb = fAccumProb * ( 1.f - fProb ) + fProb;
    }

   return  ( rand() * ( 1.f/RAND_MAX) ) < fAccumProb;

}

/*static*/ inline bool    CX2GUNPC::EstimateFrameOneshotPercent( float fPercent_ )
{
    if ( fPercent_ <= 0.f )
        return false;
    else if ( fPercent_ >= 100.f )
        return true;

    return  ( rand() * ( 100.f/RAND_MAX) ) < fPercent_;
}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

typedef KObserverPtr<CX2GUNPC> CX2GUNPCoPtr;


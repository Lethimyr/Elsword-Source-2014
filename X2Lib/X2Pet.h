#ifdef SERV_PET_SYSTEM

#pragma once


class CX2PETAI;

class CX2PET;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2PET>   CX2PETPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2PET>   CX2PETPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
class   CX2FrameUDPPack;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

class CX2PET : public CKTDGObject, public CKTDXDeviceHolder
{	
public:
	enum PET_STATE_ID
	{
		PSI_NONE = 0,
		PSI_LOADING,
		PSI_READY,
		PSI_PLAY,
		PSI_END,
	};

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

	//{{ 2010. 7. 26  최육사	펫 시스템
	enum PET_ACTION_COMMAND
	{
		PAC_NONE = 0,

		PAC_DONOTKNOW,	// 1. 몰라
		PAC_HUNGRY,		// 2. 배고파	- 100%
		PAC_HELLO,		// 3. 인사
		PAC_SIT,		// 4. 앉아!
		PAC_SIT_END,	// 5. 일어나!	- 100%
		PAC_CUTE,		// 6. 애교!  
		PAC_AFRAID,		// 7. 하지마!
		PAC_SLEEP,		// 8. 자자!
		PAC_SLEEP_END,	// 9. 서!		- 100%
		PAC_HAPPY,		// 10. 좋아해!
		PAC_EAT,		// 11. 먹기		- 100%
		PAC_STUFFED,	// 12. 배부름	- 100%
		PAC_FORCE_SLEEP,// 13. 무조건자 - 100%
		PAC_SPECIAL,	// 14. 스페셜!

#ifdef _IN_HOUSE_
		PAC_ATTACK,
		PAC_CHEER,
#endif

		PAC_END,
	};
	//}}

	enum PET_COMMON_EFFECT
	{
		PCE_NONE		= 0,

		PCE_SUMMON,
		PCE_EVOLUTION,
		PCE_CHEER,

		PCE_END,
	};

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	struct  InitDeviceData
	{
		std::vector<std::wstring>    m_vecstrReadyTexture;
		std::vector<std::wstring>    m_vecstrReadyXMesh;
		std::vector<std::wstring>    m_vecstrReadyXSkinMesh;
		std::vector<std::wstring>    m_vecstrReadySound;
		std::vector<std::wstring>	 m_vecstrReadyXET;

		void    AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const;

	};//struct  InitDeviceData


	struct  InitData
	{
		InitDeviceData       m_device;
		void                AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const;
	};//struct  InitData

#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD


	struct SoundPlayData 
	{
		float			m_SoundPlayTime;
		wstring			m_SoundPlayName;
		int				m_SoundPlayRate;
		bool			m_bOnlyIfMyUnit;
		float			m_fMaxSoundDistance;
	};

	struct PhysicParam
	{
		D3DXVECTOR2		nowSpeed;		//현재 스피드X,Y
		D3DXVECTOR2		passiveSpeed;	//이 스피드로 계속 보정해준다
		float			fDownAccel;		//하강 가속도

		float			fReloadAccel;	//전진 속도 감쇄율

		float			fGAccel;		//중력 가속도
		float			fMaxGSpeed;		//최대 하강 속도 Y

		float			fWalkSpeed;		//걷기 속도
		float			fRunSpeed;		//달리기 속도
		float			fJumpSpeed;		//일반 점프 상승속도
		float			fDashJumpSpeed;	//대시 점프 상승속도

		float			fSpeedFactor;	// 걷기, 달리기, 점프, 대시 점프 속도에 곱해진다. fix!! 근데 현재 구현은 이동속도에만 적용되도록 되어있다??

		
		PhysicParam()
		{
			INIT_VECTOR2( nowSpeed, 0.0f, 0.0f );
			INIT_VECTOR2( passiveSpeed, 0.0f, 0.0f );
			fDownAccel		= 0.0f;

			fReloadAccel	= 0.0f;

			fGAccel			= 0.0f;
			fMaxGSpeed		= 0.0f;

			fWalkSpeed		= 0.0f;
			fRunSpeed		= 0.0f;
			fJumpSpeed		= 0.0f;
			fDashJumpSpeed	= 0.0f;

			fSpeedFactor	= 1.f;
		}

		void OnFrameMove( double fTime, float fElapsedTime )
		{
			
		}

		float GetWalkSpeed()
		{
			return fWalkSpeed * fSpeedFactor;
		}

		float GetDashSpeed()
		{
			return fRunSpeed * fSpeedFactor;
		}

		float GetJumpSpeed()
		{
			return fJumpSpeed;		
		}

		float GetDashJumpSpeed()
		{
			return fDashJumpSpeed;		
		}
	};

	struct PetCondition	//상황을 나타내는 파라메터
	{
		char			nextState;
		char			nextSubState;
		float			fStateTime;
		float			fStateTimeBack;
		bool			bTransitionChange;

		float			fUnitWidth;	
		float			fUnitHeight;	

		D3DXVECTOR3		dirDegree;
		D3DXVECTOR3		dirVector;
		
		bool			bFootOnLine;
		D3DXVECTOR3		landPosition;

		bool			bHit;
		bool			bAttackerFront;

		//bool			bFrameBufferPass;
		char			stateChangeNum;


		PetCondition()
		{
			Init();
		}
		void Init()
		{
			nextState					= -1;
			nextSubState				= -1;
			fStateTime					= 0.0f;
			fStateTimeBack				= 0.0f;
			bTransitionChange			= true;

			fUnitWidth					= 0.0f;
			fUnitHeight					= 0.0f;

			INIT_VECTOR3( dirDegree,	0,0,0 );
			INIT_VECTOR3( dirVector,	0,0,0 );

			bFootOnLine					= true;

			landPosition				= D3DXVECTOR3(0.0f,0.0f,0.0f);

			bHit						= false;
			bAttackerFront				= true;

			//bFrameBufferPass			= false;
			stateChangeNum				= 0;


		}
	};

#ifdef EXPAND_SPEED_TIME_SCRIPT_ON_PET
		struct TIME_SPEED
		{
			D3DXVECTOR3 vSpeed;		/// x는 X축, y는 Y축, z는 스피드가 적용될 애니메이션 타임 시점
			int iFlag;				/// 1이면 m_PhysicParam.nowSpeed 의 x, y에 += 연산을\n
									///	2이면 = 연산을 수행하게하는 플래그
			TIME_SPEED() :
			vSpeed( D3DXVECTOR3(0.f, 0.f, 0.f) ),
			iFlag( 0 )
			{
			}
		};
#endif //EXPAND_SPEED_TIME_SCRIPT_ON_PET
	struct PetSyncData
	{
		char				nowState;

		D3DXVECTOR3			position;
		unsigned char		lastTouchLineIndex;

		bool				bIsRight;
		bool				bStateChange;


#ifdef NEW_RANDOM_TABLE_TEST
		USHORT m_usRandomTableIndex;
#else NEW_RANDOM_TABLE_TEST
		//{{AFX
		char				m_StateRandomSeed;
		char				m_StateRandomSeed2;		
		//}}AFX
#endif NEW_RANDOM_TABLE_TEST


		PetSyncData()
		{
			Init();
		}
		void Init()
		{
			nowState				= 0;

			INIT_VECTOR3( position,	0,0,0 );
			lastTouchLineIndex		= 0;

			bIsRight				= true;
			bStateChange			= false;
			
#ifdef NEW_RANDOM_TABLE_TEST
			m_usRandomTableIndex	= 0;
#else NEW_RANDOM_TABLE_TEST
			//{{AFX
			m_StateRandomSeed		= 99;		// 이 확률을 이용한 이벤트가 작동하지 않도록 default를 99로 한다. 값 변경할 때 주의!
			m_StateRandomSeed2		= 99;				
			//}}AFX
#endif NEW_RANDOM_TABLE_TEST
		}		

		void SetData( KXPT_UNIT_PET_SYNC* pXPT_UNIT_PET_SYNC )
		{
			nowState			= pXPT_UNIT_PET_SYNC->nowState;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            _DecodePetSyncPos( position, pXPT_UNIT_PET_SYNC->ucPosX, pXPT_UNIT_PET_SYNC->ucPosY, pXPT_UNIT_PET_SYNC->ucPosZ, pXPT_UNIT_PET_SYNC->lastTouchLineIndex );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			position.x			= pXPT_UNIT_PET_SYNC->posX;
//			position.y			= pXPT_UNIT_PET_SYNC->posY;
//			position.z			= pXPT_UNIT_PET_SYNC->posZ;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			lastTouchLineIndex	= pXPT_UNIT_PET_SYNC->lastTouchLineIndex;

			bIsRight			= pXPT_UNIT_PET_SYNC->bIsRight;
			m_usRandomTableIndex = pXPT_UNIT_PET_SYNC->m_usRandomTableIndex;
		}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        void _DecodePetSyncPos( D3DXVECTOR3& vPosition, USHORT usPosX, USHORT usPosY, USHORT usPosZ, unsigned char ucLastTouchLineIndex );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	};

	struct ConditionTableData
	{
		int			m_iIntervalID;
		float		m_fDistanceToTarget;
		float		m_fDistanceOverTarget;
		int			m_iAnimPlayCount;
		int			m_iRate;
		float		m_fSmallThenYSpeedFactor;
		float		m_fStateTimeOverFactor;
		float		m_fAnimEventTimerFactor;
		float		m_fMyHPLessThanPercentFactor;
		float		m_fMyHPMoreThanPercentFactor;
		float		m_fMyMPLessThanPercentFactor;
		float		m_fMyMPMoreThanPercentFactor;
		bool		m_bAttackSuccessFactor;
		bool		m_bSameLineWithTargetFactor;
		int			m_iFlagTrue;
		int			m_iFlagFalse;
		bool		m_bEscapeCheck;
		bool		m_bTargetAboveMe;
		bool		m_bTargetBelowMe;
		int			m_iHaveTarget;	// -1: don't care, 0: false, 1: true
		float		m_fHigherThenPosY;
		//int			m_iAliveUserUnitMoreThan;								
		//int			m_iAliveUserUnitLessThan;
		//int			m_iUserUnitMoreThan;
		//int			m_iUserUnitLessThan;
		float 		m_fTimerElapsed[10];
		int         m_iGEKeyCode; // added by wonpok
		bool		m_bDirTarget; // 타켓이 왼쪽/오른쪽에 있는지에 대한 여부를 검사할지 설정
		bool		m_bRightTarget; // m_bDirTarget 이 TRUE일때 설정한다.

		ConditionTableData()
		{
			m_iIntervalID					= -1;
			m_fDistanceToTarget				= -1.0f;
			m_fDistanceOverTarget			= -1.0f;
			m_iAnimPlayCount				= -1;
			m_iRate							= -1;
			m_fSmallThenYSpeedFactor		= -1.0f;
			m_fStateTimeOverFactor			= -1.0f;
			m_fAnimEventTimerFactor			= -1.0f;
			m_fMyHPLessThanPercentFactor	= -1.0f;
			m_fMyHPMoreThanPercentFactor	= -1.0f;
			m_fMyMPLessThanPercentFactor	= -1.0f;
			m_fMyMPMoreThanPercentFactor	= -1.0f;
			m_bAttackSuccessFactor			= false;
			m_bSameLineWithTargetFactor		= false;
			m_iFlagTrue						= -1;
			m_iFlagFalse					= -1;
			m_bEscapeCheck					= false;
			m_bTargetAboveMe				= false;
			m_bTargetBelowMe				= false;
			m_iHaveTarget					= false;
			m_fHigherThenPosY				= -1.0f;
			//m_iAliveUserUnitMoreThan		= -1;
			//m_iAliveUserUnitLessThan		= -1;
			//m_iUserUnitMoreThan				= -1;
			//m_iUserUnitLessThan				= -1;

			m_iGEKeyCode                    = -1;
			m_bDirTarget					= false;
			m_bRightTarget					= false;

			for( int i=0; i<10; i++ )
			{
				m_fTimerElapsed[i] = -1.f;
			}
		}
	};

	struct PetEventProcessData
	{
		STATE_CHANGE_TYPE	m_StateChangeType;
		char                m_NextStateID;
		string				m_strCustomCondition;
		ConditionTableData*	m_pConditionTableData;

		PetEventProcessData()
		{
			m_StateChangeType		= SCT_NO_CHANGE;
			m_NextStateID			= PSI_NONE;
			m_strCustomCondition	= "";
			m_pConditionTableData	= NULL;
		}

		~PetEventProcessData()
		{
			SAFE_DELETE( m_pConditionTableData );
		}
	};

	struct PetStateData
	{
		int				stateID;
		
#ifdef GHOST_PET
		std::string		Lua_Start;
#endif
		std::string		Lua_FrameMove;
		
		float			fStateCoolTime;
		float			fStateElapsedTime;		

		std::vector< PetEventProcessData* > m_vecpPetEventProcessData;

	public:
		void RemoveAllPetEventProcessData()
		{
			KTDXPROFILE();

			for( int i = 0; i < (int)m_vecpPetEventProcessData.size(); i++ )
			{
				PetEventProcessData* pPetEventProcessData = m_vecpPetEventProcessData[i];
				SAFE_DELETE( pPetEventProcessData );
			}

			m_vecpPetEventProcessData.resize(0);
		}

		~PetStateData() 
		{
			RemoveAllPetEventProcessData();
		}

		PetStateData()
		{
			Init();
		}

		virtual void Init()
		{
			stateID			= PSI_NONE;

#ifdef GHOST_PET
			Lua_Start			= "";
#endif
			Lua_FrameMove		= "";
			fStateCoolTime = 0.f;
			fStateElapsedTime	= 0.0f;

			m_vecpPetEventProcessData.resize( 0 );
		}



		// 주의!! 이 copy constructor, assignment operator 삭제하면 안됨
		PetStateData( const PetStateData& t ) 
		{ 
			*this = t; 
		}    

		PetStateData& operator=( const PetStateData& data )
		{
			stateID				= data.stateID;

#ifdef GHOST_PET
			Lua_Start			= data.Lua_Start;
#endif
			Lua_FrameMove		= data.Lua_FrameMove;
			fStateCoolTime		= data.fStateCoolTime;
			fStateElapsedTime	= data.fStateElapsedTime;

			return *this;
		}
	};
	
	struct PetInfo
	{
		UidType m_PetUid;
		int		m_PetId;

		wstring m_wstrPetName;		// 이름

		float	m_Intimacy;			// 현재 친밀감
		float	m_Satiety;			// 현재 포만감

		int		m_Extroversion;		// 외향성
		int		m_Emotion;			// 감성

		char	m_Evolution_Step;	// 현재 진화단계

		bool	m_bSummon;			// 현재 소환여부

#ifdef PET_DROP_ITEM_PICKUP
		bool	m_bIsDropItemPickup;// 아이템 줍기 스킬 활성화 여부
#endif //PET_DROP_ITEM_PICKUP
#ifdef SERV_PET_SYSTEM_EX1
		bool	m_bAlwaysMaxSatiety;// 항상 포만도 max 인가?
#endif //SERV_PET_SYSTEM_EX1
#ifdef SERV_EVENT_PET_INVENTORY
		//펫 먹이를 먹었는지 안먹었는지 여기서 판단하는 변수를 만듬
		//이벤트 펫 먹이 추가 
		bool							m_bEventFoodEat;      // 이벤트 먹이의 사용 유무 
		bool							m_bIsEventPetID;      // 이벤트 성 펫인지 확인 한다.
#endif SERV_EVENT_PET_INVENTORY
	public:
		PetInfo()
		{
			m_PetUid = -1;
			m_PetId = -1;
			m_wstrPetName = L"";
			m_Intimacy = 0.f;
			m_Satiety = 0.f;
			m_Extroversion = 0;
			m_Emotion = 0;
			m_Evolution_Step = 0;
			m_bSummon = false;
#ifdef PET_DROP_ITEM_PICKUP
			m_bIsDropItemPickup = false;	// 아이템 줍기 스킬 활성화 여부
#endif //PET_DROP_ITEM_PICKUP
#ifdef SERV_PET_SYSTEM_EX1
			m_bAlwaysMaxSatiety = false;// 항상 포만도 max 인가?
#endif //SERV_PET_SYSTEM_EX1
#ifdef SERV_EVENT_PET_INVENTORY
			//이벤트 펫먹이 변수 초기화 
			m_bEventFoodEat = false;
			m_bIsEventPetID = false;
#endif SERV_EVENT_PET_INVENTORY
		}
	};

private:
	CX2PET();
	virtual ~CX2PET();

public:	
	static CX2PETPtr CreatePet()
	{
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        CX2PETPtr pObject( new CX2PET );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		CX2PETPtr pObject( new CX2PET, CKTDGObject::KTDGObjectDeleter() );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		return pObject;
	}//CreateWorldObjectMesh()

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	static void					InitDevice( InitDeviceData& OutDevice_, KLuaManager& luaManager_ );
	static void                 InitMotion( InitDeviceData& OutDevice_, KLuaManager& luaManager_ );
	static void					InitInit( InitData& OutInit_, KLuaManager& luaManager_ );

	void SetPet( lua_State* pPetLuaState, UidType masterUid );	
	void SetGamePet( lua_State* pPetLuaState, UidType masterUid );
	KLuaManager&				GetLuaManager()	{ return m_LuaManager; }

#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	void SetPet( wstring wstrPetScriptName, UidType masterUid );	
	void SetGamePet( wstring wstrPetScriptName, UidType masterUid );

#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

#ifdef EXPAND_SPEED_TIME_SCRIPT_ON_PET
	std::vector<TIME_SPEED> m_vecSpeedFactor;	/// 유저만 사용중, 스크립트 상에서 특정 시간대에 스피드를 줄때 사용
#endif //EXPAND_SPEED_TIME_SCRIPT_ON_PET

	void SetInfo( PetInfo m_petData);
	PetInfo &GetPetInfo() { return m_petData; }

	virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );

	virtual RENDER_HINT         OnFrameRender_Prepare();
	virtual void                OnFrameRender_Draw();
	void RenderName();

	virtual HRESULT				OnResetDevice();
	virtual HRESULT				OnLostDevice();

	void PushAIMessage( STATE_CHANGE_TYPE message, bool bIsRight );

	const PetCondition& GetCondition() const { return m_petCondition; }
	const PetSyncData& GetSyncData() const { return m_petSyncData; }
	const PhysicParam& GetPhysicParam() const { return m_PhysicParam; }

	UidType GetMasterUid() { return m_MasterUid; }
	void SetMasterUid( UidType masteruid ) { m_MasterUid = masteruid; }
	bool SetPosition( D3DXVECTOR3& vPos, bool bIsRight );
	D3DXVECTOR3 GetPos() { return m_petSyncData.position; }
	bool InitPos();
#ifndef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	void InitGamePet();
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	int GetNowState() { return m_petSyncData.nowState; }

	//virtual bool OnThreadLoad(DWORD dwParam_, DWORD dwParam2_);

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    bool    SendPacketImmediateForce( CX2FrameUDPPack& kFrameUDPPack );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	void    SendPacketImmediateForce( vector<KXPT_UNIT_PET_SYNC>& syncList );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	void ReceiveFrameData( const KXPT_UNIT_PET_SYNC& kXPT_UNIT_PET_SYNC );

	bool StateChange( int iStateId, bool bForce = false, bool bCheck = true );

	CKTDGXSkinAnim* GetXSkinAnim() 
	{ 		
		if( GetLoaded() == true && m_pXSkinAnim != NULL )
		{
			return m_pXSkinAnim; 			
		}
		return NULL;
	}

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
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

	bool						EventTimer( float fTime );
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	bool						EventCheck( float fTime );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	bool AnimEventTimer_LUA( float fTime )
	{			
		if( m_pXSkinAnim == NULL )
			return false;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if( m_pXSkinAnim->EventTimerOneshot( fTime ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( fTime ) == true && EventCheck( fTime ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return true;
		else
			return false;
	}
	float GetNowAnimTime_LUA() 
	{ 
		return m_pXSkinAnim->GetNowAnimationTime(); 
	}
	D3DXVECTOR3	GetLandPosition_LUA()
	{ 
		return m_petCondition.landPosition;
	}
	bool GetIsRight()
	{ 
		return m_petSyncData.bIsRight;
	}
	D3DXVECTOR3			GetDirDegree()								{ return m_petCondition.dirDegree; }
	D3DXVECTOR3			GetDirVector()								{ return m_petCondition.dirVector; }
	D3DXVECTOR3			GetZVector()
	{
		D3DXVECTOR3 vDirVector = GetDirVector();
		if( vDirVector.y != 0.f )
		{
			vDirVector.y = vDirVector.z;
			vDirVector.z = vDirVector.x;
			vDirVector.x = -vDirVector.y;
			vDirVector.y = 0.f;
			D3DXVec3Normalize( &vDirVector, &vDirVector );
		}
		else
		{
			vDirVector.y = vDirVector.z;
			vDirVector.z = vDirVector.x;
			vDirVector.x = -vDirVector.y;
			vDirVector.y = 0.f;
		}
		return vDirVector;
	}

	void SetIsRight( bool bVal )
	{
		m_petSyncData.bIsRight = bVal;
	}
	
	D3DXVECTOR3			GetBonePos( const WCHAR* pBoneName )		
	{
		if( m_pXSkinAnim != NULL )
			return m_pXSkinAnim->GetCloneFramePosition( pBoneName ); 

		return D3DXVECTOR3(0.f, 0.f, 0.f);
	}
	D3DXVECTOR3			GetBonePos_LUA( const char* pBoneName )
	{ 
		wstring wBoneName;
		ConvertUtf8ToWCHAR( wBoneName, pBoneName );
		return GetBonePos( wBoneName.c_str() ); 
	}
	D3DXVECTOR3			GetRotateDegree()							{ return m_petCondition.dirDegree; }

	void AttackResult( float fMp );
	void DamageReact( float fMp );

	bool IsActionState(int iState);

	bool GetinitPet() { return m_bInit; }
	void SetInitPosFlag(bool bVal) { m_bInitPos = bVal; }

	float GetDistTarget();
	bool IsAttacking();	
	bool IsTargetFront();
	void InitOtherTarget(); 

	void SetEnableAi( bool bVal = true );
	bool GetEnableAi();

	bool IsMyPet() { return m_bMyPet; }
	CX2GameUnit *GetMaster() { return ( null != m_optrMasterGameUnit ? m_optrMasterGameUnit.GetObservable() : NULL ); }

	CKTDGParticleSystem*	GetUIMajorParticle();
	CX2EffectSet*			GetUIEffectSet();

	void SetEvolution(bool bVal) { m_bEvolution = bVal; }

	int							GetStateID( const char* state )
	{
		if ( state == NULL )
			return PSI_NONE;
		return GetStateID( std::string( state ) );
	}
	int                         GetStateID( const std::string& strInState_ )
	{
		std::map<string,char>::iterator iter = m_StateNameIDMap.find( strInState_ );
		if ( iter == m_StateNameIDMap.end() )
			return PSI_NONE;
		return iter->second;
	}

	float GetNowIntimacy();
	float GetNowSatiety();
	void SetShowEffect(bool bVal);
	void SetShowGageBar( bool bVal );

	bool GetLoaded() { return m_bLoaded; }
	
	// cheat
	void SetCheat( bool bVal ) { m_bCheat = bVal; }
	void SetStateEffect( CX2EffectSet::Handle hEffect ) { m_hStateEffect = hEffect; }
	float GetNowStateTimer(){ return m_fNowStateTimer; }
	bool GetFootOnLine(){ return m_petCondition.bFootOnLine; }
	bool GetFlyUnit(){ return m_bFlyUnit; }

	bool GetSendReserveStateChange() const { return m_bSendReserveStateChange; }
	void SetSendReserveStateChange( const bool bSendReserveStateChange_ ) { m_bSendReserveStateChange = bSendReserveStateChange_; }

	CX2GameUnit *GetTargetUnit();

#ifdef ADD_PET_NINE_TAIL_FOX
	void	SetLockOnDamageEffect( CX2DamageEffect::CEffect* pEffect );		/// 현재 팻이 목표로 정한 유닛을 인자로 들어온 데미지 이펙트에 록온 시킨다.
	int		GetRandomInt( int iOffset = 0 )	{ return CKTDXRandomNumbers::GetRandomInt( m_petSyncData.m_usRandomTableIndex + iOffset ); }		/// 난수 반환 함수
#endif ADD_PET_NINE_TAIL_FOX

#ifdef ADD_HALLOWEEN_PET_SYSTEM
	int GetHitUnitListSize( CX2DamageManager::DamageData* pDamageData ) { return static_cast<int>( pDamageData->hitUnitList.size() ); }			/// 피격 유닛수 반환
	CX2DamageManager::DamageData* GetDamageData_LUA() { return m_pDamageData; }	/// 데미지 데이터 반환
	void						  SetDamageData_LUA( CX2DamageManager::DamageData* pDamageData ) { m_pDamageData = pDamageData; }				/// 데미지 데이터 설정
	void						  ShowIncreaseValue( D3DXVECTOR3 vecPos, bool IsHP, float fValue );		/// HP or MP 회복 수치 표시
#endif ADD_HALLOWEEN_PET_SYSTEM

	bool GetOnPopup() { return m_bOnPopUp; }
	void SetOnPopup( bool bVal_ ) { m_bOnPopUp = bVal_; }

	void SetAuraSuspendedTargetTime( const float fSuspendedTime_ )	// 오오라 스킬이 적용 되기 까지의 지연시간
	{
		m_TimerAuraSuspended.SetTargetTime( fSuspendedTime_ );
	}

	float GetAuraSuspendedTargetTime() const { return m_TimerAuraSuspended.GetSumOfElapsedTime(); }
	bool CheckAndResetAuraSuspendedSumOfElapsedTime() { return m_TimerAuraSuspended.CheckAndResetElapsedTime(); }
	void ResetAuraSuspendedSumOfElapsedTime() { m_TimerAuraSuspended.ResetSumOfElapsedTime(); }
#ifdef EXPAND_SPEED_TIME_SCRIPT_ON_PET
	bool IsSpeedTime();
#endif //EXPAND_SPEED_TIME_SCRIPT_ON_PET
#ifdef SET_IN_PET_LUA
	void						SetFlag_LUA( int index, bool bFlag ){ m_FlagList[index] = bFlag; }
	bool						GetFlag_LUA( int index ){ return m_FlagList[index]; }
	void						SetInt_LUA( int index, int bFlag ){ m_IntList[index] = bFlag; }
	int							GetInt_LUA( int index ){ return m_IntList[index]; }
	void						SetVector_LUA( int index, D3DXVECTOR3 vVec ){ m_VecList[index] = vVec; }
	D3DXVECTOR3					GetVector_LUA( int index ){ return m_VecList[index]; }
	double						GetNumber_LUA( int index ) { return m_NumberList[ index ]; }
	void						SetNumber_LUA( int index, double dValue_ ) { m_NumberList[ index ] = dValue_; }
#endif SET_IN_PET_LUA

#ifdef ADD_2013_CHRISTMAS_PET // 김태환
	void								SetEffectSet_LUA( int iIndex, CX2EffectSet::Handle hHandle ) { m_hEffectSetHandle[iIndex] = hHandle; }
	CX2EffectSet::Handle				GetEffectSet_LUA( int iIndex ) { return m_hEffectSetHandle[iIndex]; }
	void								ClearEffectSet_LUA( int iIndex ) { m_hEffectSetHandle[iIndex] = INVALID_EFFECTSET_HANDLE; }
#endif // ADD_2013_CHRISTMAS_PET

private:
	void InitDevice();
	void InitSystem();
	void InitMotion();
	void InitPhysic();
	void InitState();

	void DoFrameMove();
	void DoProcessEvent();
	void PhysicProcess();

	void GetMasterInfo( D3DXVECTOR3 &vPos, bool &IsRight );	
	void UpdateSpeedByUser();

	void DoCheer();
	bool GetActionSuccess(int &iStateId, bool bCheckSatietyRand = true);

	void PlayCommonEffect( PET_COMMON_EFFECT eType );
	CKTDXDeviceSound* PlaySound( const WCHAR* pName );
	void DrawPetIcon( int iStateId );

#ifdef PET_AURA_BUFF
	UINT GetBuffLevelByIntimacy();		/// 친밀도에 따른 버프 레벨값 반환
#endif PET_AURA_BUFF

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
public:
	void    UpdateRandomTableIndex();

private:
    void    _EncodePetSyncPos( USHORT& usPosX, USHORT& usPosY, USHORT& usPosZ, const D3DXVECTOR3& position, unsigned char lastTouchLineIndex );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

public:
	int		m_StartState;
	int		m_WaitHabitState;
	int		m_WaitState;
	int		m_EatState;
	int		m_HungryState;
	int		m_StuffedState;
	int		m_SleepStartState;
	int		m_SleepLoopState;
	int		m_SleepEndState;
	int		m_AfraidState;
	int		m_HelloState;
	int		m_Happy1State;
	int		m_Happy2State;
	int		m_Cute1State;
	int		m_Cute2State;
	int		m_SadState;
	int		m_CheerState;
	int		m_SpecialState;
	int		m_AttackState;
	int		m_SitStartState;
	int		m_SitLoopState;
	int		m_SitEndState;
	int		m_NotKnowState;
	int		m_SleepFallState;
	int		m_SitFallState;


private:

	KLuaManager										m_LuaManager;
#ifndef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	wstring											m_ScriptFileName;
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	wstring											m_wstrMotionName;
	wstring											m_wstrChangeTex;
	wstring											m_wstrAniTex;

	CKTDXDeviceXSkinMesh*							m_pMotion;		
	CKTDGXSkinAnim*									m_pXSkinAnim;

	CKTDXDeviceXET*									m_pAniXET;
	CKTDXDeviceXET*									m_pChangeTexXET;

	bool											m_bForceBillBoard;
	CKTDGXRenderer::RenderParam						m_RenderParam;

	PetStateData									m_PetStateData;	
	map<char,PetStateData>							m_StateList;
	std::map<string,char>                           m_StateNameIDMap;

	int		m_PreState;
	
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	map<float,bool>									m_EventTimeStamp;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	STATE_CHANGE_TYPE								m_AIMessage;
	STATE_CHANGE_TYPE								m_UnUseAIMessage;
	bool											m_bAIIsRight;
	bool											m_bAIEnabled;	
	bool											m_bSendReserveStateChange;

	PetCondition	m_petCondition;
	PetSyncData		m_petSyncData;
	PhysicParam		m_PhysicParam;

	PhysicParam		m_ScriptedPhysicParam;
	PhysicParam		m_OrgPhysicParam;

	vector<KXPT_UNIT_PET_SYNC>						m_ReceivePacketBufList;

	vector<D3DXVECTOR3>								m_EventIntervalList;

	CX2PETAI*										m_pAI;

	UidType											m_MasterUid;
	float											m_fElapsedTime;
	bool											m_bFixFlyHeight;
	bool											m_bLandConnect;
	bool											m_bAllowDirChange;

	bool											m_bOldLoaded;
	bool											m_bLoaded;
	float											m_fScale;
	float											m_fLostRange;

	float											m_fExtraJumpSpeed;
	bool											m_bGame;
	bool											m_bInit;
	bool											m_bMyPet;
	bool											m_bInitPos;	
	bool											m_bEvolution;

	CX2GameUnitoPtr									m_optrMasterGameUnit;

	bool											m_bSetData;
	PetInfo											m_petData;
	// 마스터 타격/피격 수치정보
	float		m_fComboIntervalTime;	
	float		m_fTimeAttack;
	float		m_fTimeDefence;
	int			m_AttackComboNum;
	int			m_DefenceComboNum;	

	bool		m_bAttacking;
	bool		m_bAttackTrace;
	float		m_fAttackTraceTime;

	float		m_fNowMp;
	float		m_fNowStateTimer;

	CKTDGUIDialogType		m_pDlgPetGage;

	vector<SoundPlayData>							m_vecSoundPlayData;
#ifdef CLOSE_SOUND_TEST
	CX2SoundCloseManager m_SoundCloseManager;
#endif CLOSE_SOUND_TEST

	float m_fIconTimer;
	CX2EffectSet::Handle m_hEffectCheer;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqIcon;

#ifdef PET_ATTACH_PARTICLE
	struct AttachedPetParticleData
	{
		wstring wstrParticleName;
		wstring wstrBoneName;
		CKTDGParticleSystem::CParticleEventSequenceHandle hSeq;
	};
	vector< AttachedPetParticleData >	m_vecSeqPetParticle;
#endif //PET_ATTACH_PARTICLE

	bool		m_bCheat;

	bool		m_bForceChagneColor;
	bool		m_bUnderWaterHead;
	D3DXCOLOR	m_cLineUnitColor;

#ifdef GHOST_PET
	bool		m_bForceColor;
	D3DXCOLOR	m_cPetColor;
#endif

	CX2EffectSet::Handle m_hStateEffect;

	bool m_bFlyUnit;

#ifdef ADD_HALLOWEEN_PET_SYSTEM
	CX2DamageManager::DamageData*	m_pDamageData;
#endif ADD_HALLOWEEN_PET_SYSTEM

	bool	m_bOnPopUp;

	CKTDXCheckElapsedTime	m_TimerAuraSuspended;	/// 소환된 이후로 경과시간 /// 필드에서 기운효과 적용시 체크

#ifdef EXPAND_SPEED_TIME_SCRIPT_ON_PET
	bool	bSpeedTime;
#endif //EXPAND_SPEED_TIME_SCRIPT_ON_PET
#ifdef SET_IN_PET_LUA
	bool											m_FlagList[10];
	int												m_IntList[10];
	D3DXVECTOR3										m_VecList[10];
	double											m_NumberList[10];
#endif SET_IN_PET_LUA

#ifdef ADD_2013_CHRISTMAS_PET // 김태환
	CX2EffectSet::Handle							m_hEffectSetHandle[10];
#endif // ADD_2013_CHRISTMAS_PET
};

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
struct  CX2PetManager_PetInitTemplate
{
	CX2PET::InitData	m_init;
};
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

#endif
#pragma once

#include "./X2RidingPet.h"

//{{ seojt // 2009-1-13, 17:11
class CX2SquareUnit;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2SquareUnit>    CX2SquareUnitPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2SquareUnit>    CX2SquareUnitPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-13, 17:11

/** @class : CX2SquareUnit
	@brief : 
	@date	JHKang on 2013-03-26,
		- 주석 추가
	@date	JHKang on 2013-03-27
		- 불필요한 Enum( SYNC_UNIT_STATE ), 관련 함수( SetActId_LUA() ) 제거
	@date	JHKang on 2013-03-28
		- Riding -> RidingPet으로 변경
		- Riding 관련 State 추가 및 파싱 처리
	@date	JHKang on 2013-04-02
		- 키로 타기 상태로 변경하는 기능 추가
		- 습관 모션 출력을 위한 RandomSeed 추가
	@date	JHKang on 2013-04-05
		- 탈 것 추가 코드 작성
	@date	JHKang on 2013-04-10
		- 유저를 탈 것 안장에 올리기 위한 코드 작성
		- 누락된 걷기 상태 변수 추가
	@date	JHKang on 2013-04-11
		- 타고 있는 상태에 대한 여부 함수 수정
		- Square lua 파일 정리( 하나로 통일 )
	@date	JHKang on 2013-04-12
		- 유저 앉기 동기화 문제 수정
		- 탈 것 앉기 상태 및 기능 추가
*/
class CX2SquareUnit : public CKTDGObject
{
	public:
		/// 개인 상점 상태
		enum PERSONAL_SHOP_STATE
		{
			PSS_NONE	= 0,
			PSS_WAIT,
			PSS_SHOP,
		};

		/// 개인 상점 종류
#ifndef SERV_UPGRADE_TRADE_SYSTEM // 김태환
		enum PERSONAL_SHOP_TYPE
		{
			PST_NONE = 0,

			PST_NORMAL,
			PST_PREMIUM,

			PST_END,
		};
#endif //SERV_UPGRADE_TRADE_SYSTEM

		/// Unit 상태 동기화 값
		enum SYNC_UNIT_STATE
		{			
			SUS_W		= 0,	/// wait
			SUS_L		= 1,	/// left
			SUS_R		= 2,	/// right
			SUS_DL		= 4,	/// dash_left
			SUS_DR		= 8,	/// dash_right
			SUS_U		= 16,	/// up
			SUS_D		= 32,	/// down
			SUS_DU		= 64,	/// double_up
			SUS_DD		= 128,	/// double_down
			SUS_DUMY	= 255,	/// none
		};

		/// 동기화용 상태 값
		struct SyncUnitState
		{			
			u_char	m_eventID;		/// 스테이트 아이디
			u_char	m_elapsedFrame;	/// 현재 스테이트가 유지된 프레임 수 (카운트 수)		
		};

		/// 동기화 데이터
		struct SyncData
		{
			u_char					m_StateID;
			char					m_StateChangeNum;
			D3DXVECTOR3				m_Pos;
			unsigned char			m_LastTouchLineIndex;

			bool					m_bIsRight;

#ifdef RIDING_SYSTEM
			bool			m_bRidingOn;				/// 탑승?
			D3DXVECTOR3		m_vSaddlePos;				/// 안장 위치
			D3DXVECTOR3		m_vDirDegree;				/// 안장 회전 값
			wstring			m_wstrRidingMotionName;		/// 모션 이름
#endif //RIDING_SYSTEM

			SyncData()
			{
				Init();
			}
			void Init()
			{
				m_StateID				= -1;
				m_StateChangeNum		= -1;
				INIT_VECTOR3( m_Pos,	0,0,0 )
				m_LastTouchLineIndex	= 0;
				m_bIsRight				= true;
#ifdef RIDING_SYSTEM
				m_bRidingOn				= false;
				INIT_VECTOR3( m_vSaddlePos, 0, 0, 0 )
				INIT_VECTOR3( m_vDirDegree, 0, 0, 0 )
				m_wstrRidingMotionName	= L"";
#endif //RIDING_SYSTEM
			}
		};

		/// 상태 데이터
		struct StateData
		{
			string				m_StateName;
			bool				m_bLandConnect;
			string				m_FuncStateStart;
			string				m_FuncFrameMove;
			string				m_FuncEventProcess;
			string				m_FuncStateEnd;

			StateData()
			{
				m_bLandConnect = true;
			}
		};

		/// 공용 상태
		struct CommonState
		{
			int m_Wait;				/// 대기
			int m_Walk;				/// 걷기 
			int m_JumpUp;			/// 점프 업
			int m_JumpDown;			/// 점프 다운
			int m_Dash;				/// 달리기
			int m_DashJumpUp;		/// 달리기 점프 업
			int m_DashJumpDown;		/// 달리기 점프 다운

#ifdef RIDING_SYSTEM
			int m_iRidingOn;			/// 탈 것 타기
			int m_iRidingOff;			/// 내리기
			int	m_iRidingWait;			/// 대기
			int m_iRidingWaitHabit;		/// 습관
			int m_iRidingWalk;			/// 걷기
			int m_iRidingJumpUp;		///	점프 업
			int m_iRidingJumpDown;		/// 점프 다운
			int m_iRidingDash;			/// 달리기
			int m_iRidingDashJumpUp;	/// 달리기 점프 업
			int m_iRidingDashJumpDown;	/// 달리기 점프 다운
#endif //RIDING_SYSTEM

			CommonState()
			: m_Wait( 0 )
			, m_Walk( 0 )
			, m_JumpUp( 0 )
			, m_JumpDown( 0 )
			, m_Dash( 0 )
			, m_DashJumpUp( 0 )
			, m_DashJumpDown( 0 )
#ifdef RIDING_SYSTEM
			, m_iRidingOn( 0 )
			, m_iRidingOff( 0 )
			, m_iRidingWait( 0 )
			, m_iRidingWaitHabit( 0 )
			, m_iRidingWalk( 0 )
			, m_iRidingJumpUp( 0 )
			, m_iRidingJumpDown( 0 )
			, m_iRidingDash( 0 )
			, m_iRidingDashJumpUp( 0 )
			, m_iRidingDashJumpDown( 0 )
#endif //RIDING_SYSTEM
			{
			}
		};

	private:
		//CX2SquareUnit() { ASSERT( !"invalid" ); }
		CX2SquareUnit( bool bMyUnit, CX2Unit* pUnit, const WCHAR* pScriptName, bool bTFieldUnit = false
#ifdef ADDED_RELATIONSHIP_SYSTEM
			, char cWeddingStatus = 0
#endif //ADDED_RELATIONSHIP_SYSTEM
			);
		virtual ~CX2SquareUnit(void);

	public: 
		//{{ seojt // 2009-1-14, 21:31
		static CX2SquareUnitPtr CreateSquareUnit( bool bMyUnit, CX2Unit* pUnit, const WCHAR* pScriptName, bool bTFieldUnit = false 
											#ifdef ADDED_RELATIONSHIP_SYSTEM
												, char cWeddingStatus = 0
											#endif //ADDED_RELATIONSHIP_SYSTEM
			)
		{
			CX2SquareUnitPtr    ptrSquareUnit( new CX2SquareUnit( bMyUnit, pUnit, pScriptName, bTFieldUnit
															#ifdef ADDED_RELATIONSHIP_SYSTEM
																, cWeddingStatus
															#endif //ADDED_RELATIONSHIP_SYSTEM
																)
#ifndef X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                                                , CKTDGObject::KTDGObjectDeleter()
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR                                                
                                                );
            return ptrSquareUnit;
		}//CreateSquareUnit()
		//}} seojt // 2009-1-14, 21:31

//{{ robobeg : 2011-03-18
        static void         AppendToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pUnit, bool bTField = false, CX2UnitViewerUI::UNIT_VIEWER_STATE eState = CX2UnitViewerUI::UVS_NONE );
//}} robobeg : 2011-03-18


//{{ robobeg : 2008-10-18
		virtual void		NotifyShowObjectChanged();
//}} robobeg : 2008-10-18

		bool				GetInit(){ return m_bInit; }
		void				Init();
		void				InitViewer();
		void				InitSystem();
		void				InitState();
	#ifdef RIDING_SYSTEM
		void	InitParticle();
		void	InitWaitMotion();
	#endif //RIDING_SYSTEM


		void SetScriptFileName(const wstring& val) { m_ScriptFileName = val; }




		UidType				GetUID(){ return m_UnitUID; }
		wstring				GetNickName()
		{ 
			if( NULL != m_pUnit )
			{
				return m_pUnit->GetNickName(); 
			}
			else
			{
				return wstring();
			}
		}
		CX2Unit*			GetUnit(){ return m_pUnit; }
		HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		CX2UnitViewerUI*	GetUnitViewer(){ return m_pUnitViewer; }

//{{ robobeg : 2008-10-18
        virtual RENDER_HINT     OnFrameRender_Prepare();
		virtual void            OnFrameRender_Draw();
//}} robobeg : 2008-10-18

		void				RenderRank(int iFinalLeft, int iFinalTop);
#ifdef SERV_INTEGRATION
//{{ oasis907 : 김상윤 [2010.5.17] // 
		void				RenderServer(int iFinalLeft, int iFinalTop);
//}}
#endif SERV_INTEGRATION
		void				RenderName();

// X2UnitVIewerUI 가 KTDGManager 에 등록, 삭제될 때 장착 부품들도 같이 등록, 삭제하기 위해 사용됨
        virtual void    NotifyDGManagerChanged( CKTDGManager& refInManager_ );
//}} robobeg : 2008-10-17
		
		bool		IsAnimationEnd_LUA();
		void		StateChange( char stateID );
		void		StateChange( const char* pStateName );
		void		StateChange_LUA( const char* pStateName );

		u_char		GetStateID( const char* pStateName );		
		u_char		GetNowState() { return m_SyncData.m_StateID; }
		int			GetLastTouchLineIndex() { return m_SyncData.m_LastTouchLineIndex; }
				
		void		SetPos(D3DXVECTOR3 vPos) { m_SyncData.m_Pos = vPos; }
		D3DXVECTOR3	GetPos(){ return m_SyncData.m_Pos; }
        float       GetXPos() { return m_SyncData.m_Pos.x; }
        float       GetYPos() { return m_SyncData.m_Pos.y; }
        float       GetZPos() { return m_SyncData.m_Pos.z; }
		D3DXVECTOR3 GetLandPosition() const { return m_LandPosition; }
#ifdef RIDING_SYSTEM
		D3DXVECTOR3	GetSaddlePos(){ return m_SyncData.m_vSaddlePos; }
		void		SetSaddlePos( IN D3DXVECTOR3 vPos_ ) { m_SyncData.m_vSaddlePos = vPos_; }
		D3DXVECTOR3	GetSaddleDegree(){ return m_SyncData.m_vDirDegree; }
		void		SetSaddleDegree( IN D3DXVECTOR3 vDegree_ ) { m_SyncData.m_vDirDegree = vDegree_; }
		void		SetRidingMotionName( IN const wstring& wstrName_ ) { m_SyncData.m_wstrRidingMotionName = wstrName_; }

		bool GetRidingOn() const { return m_SyncData.m_bRidingOn; }
		void SetRidingOn( IN bool bVal_ ) { m_SyncData.m_bRidingOn = bVal_; }
		void ProcessRidingOn( IN bool bVal_, IN const KRidingPetInfo& _Info = KRidingPetInfo() );
		bool GetCheckWaitHabitElapsedTime() { return m_ElapsedTimeRidingWaitHabit.CheckElapsedTime(); }
		void ResetWaitHabitElapsedTime() { m_ElapsedTimeRidingWaitHabit.ResetSumOfElapsedTime(); }
#endif //RIDING_SYSTEM

		D3DXVECTOR3	GetScale(){ return m_Scale; }
		D3DXVECTOR3	GetDirDegree(){ return m_DirDegree; }
		D3DXVECTOR3	GetDirVector(){ return m_DirVector; }
		bool		GetIsRight(){ return m_SyncData.m_bIsRight; }
		
#ifdef RELATIONSHIP_EFFECT_NOT_DRAW_WHEN_TARGET_POSITION_DATA_IS_MISSING
		// 추가, 마을에서 0, 0, 0 좌표에 커플 파티클 이펙트가
		// 보이는 경우 수정
		void		SetPlanRender( bool bPlanRender );
#else // RELATIONSHIP_EFFECT_NOT_DRAW_WHEN_TARGET_POSITION_DATA_IS_MISSING
		void		SetPlanRender( bool bPlanRender ){ m_bPlanRender = bPlanRender; }
#endif // RELATIONSHIP_EFFECT_NOT_DRAW_WHEN_TARGET_POSITION_DATA_IS_MISSING
		bool        GetPlanRender() { return m_bPlanRender; }

	

		D3DXVECTOR3	GetBonePos( const WCHAR* pBoneName )		
		{ 
			if( NULL != m_pUnitViewer )
			{
				return m_pUnitViewer->GetXSkinAnim()->GetCloneFramePosition( pBoneName );
			}

			return D3DXVECTOR3(0,0,0);
		}


		D3DXVECTOR3	GetZVector()
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




		const CX2GUUser::InputData&		GetInputData() { return (const CX2GUUser::InputData&)m_InputData; }
		void							SetInputData( CX2GUUser::InputData* pInputData );

		bool GetPureLeft_LUA()			{ return m_InputData.pureLeft; }
		bool GetOneLeft_LUA()			{ return m_InputData.oneLeft; }
		bool GetPureDoubleLeft_LUA()	{ return m_InputData.pureDoubleLeft; }
		bool GetOneDoubleLeft_LUA()		{ return m_InputData.oneDoubleLeft; }

		bool GetPureRight_LUA()			{ return m_InputData.pureRight; }
		bool GetOneRight_LUA()			{ return m_InputData.oneRight; }
		bool GetPureDoubleRight_LUA()	{ return m_InputData.pureDoubleRight; }
		bool GetOneDoubleRight_LUA()	{ return m_InputData.oneDoubleRight; }

		bool GetPureUp_LUA()			{ return m_InputData.pureUp; }
		bool GetOneUp_LUA()				{ return m_InputData.oneUp; }
		bool GetPureDoubleUp_LUA()		{ return m_InputData.pureDoubleUp; }
		bool GetOneDoubleUp_LUA()		{ return m_InputData.oneDoubleUp; }

		bool GetPureDown_LUA()			{ return m_InputData.pureDown; }
		bool GetOneDown_LUA()			{ return m_InputData.oneDown; }
		bool GetPureDoubleDown_LUA()	{ return m_InputData.pureDoubleDown; }
		bool GetOneDoubleDown_LUA()		{ return m_InputData.oneDoubleDown; }

		void SetRight_LUA( bool bRight )		{ m_SyncData.m_bIsRight = bRight; }
		bool GetRight_LUA()						{ return m_SyncData.m_bIsRight; }
		bool GetFootOnLine_LUA( bool bRight )	{ return m_bFootOnLine; }
		bool CanDown_LUA();

		void AddPosY_LUA( float fPosY )			{ m_SyncData.m_Pos.y += fPosY; }
		float GetYSpeed_LUA()					{ return m_PhysicParam.nowSpeed.y; }

		void SetXSpeed_LUA( float fSpeedX )	
		{
#ifdef MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
			if( false == m_bForceMoveSpeed )
#endif //MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
				m_PhysicParam.nowSpeed.x = fSpeedX; 
		}
		void SetYSpeed_LUA( float fSpeedY )	{ m_PhysicParam.nowSpeed.y = fSpeedY; }

		void SendPacket( int sendCount = 5 );
		void RecvPacket( KEGS_SQUARE_UNIT_SYNC_DATA_NOT& kEGS_SQUARE_UNIT_SYNC_DATA_NOT );
		void RecvTRPacket( KEGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT& kEGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT );

#ifdef ADDED_RELATIONSHIP_SYSTEM
		// BroadCast 함수의 매개변수 타입 때문에 IN이지만 Const 붙이지 않음.
		void SendPacketP2PForWedding( IN vector<UidType>& vecUnitUIDList_ );
		void GetPacketP2PForWedding( OUT KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL& kPacket_ );
		void RecvPacketP2PForWedding( OUT KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL& kPacket_ );
#endif // ADDED_RELATIONSHIP_SYSTEM

		//void GetPacketP2P( KXPT_SQUARE_UNIT_SYNC& kXPT_SQUARE_UNIT_SYNC );
		//void SendPacketP2P( UidType unitUID = 0, int sendCount = 20 );
		//void RecvPacketP2P( KXPT_SQUARE_UNIT_SYNC& kXPT_SQUARE_UNIT_SYNC );

		//void GetPacketRightP2P( KXPT_SQUARE_UNIT_SYNC_RIGHT& kXPT_SQUARE_UNIT_SYNC_RIGHT );
		//void SendPacketRightP2P();
		//void RecvPacketRightP2P( KXPT_SQUARE_UNIT_SYNC_RIGHT& kXPT_SQUARE_UNIT_SYNC_RIGHT );

		CX2SquareUnit::PERSONAL_SHOP_STATE GetPersonalShopState() const { return m_PersonalShopState; }
		void SetPersonalShopState( CX2SquareUnit::PERSONAL_SHOP_STATE personalShopState ) { m_PersonalShopState = personalShopState; }

		wstring GetPersonalShopName() { return m_PersonalShopName; }
		void SetPersonalShopName( const WCHAR* wszShopName ) { m_PersonalShopName = wszShopName; } 

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
		SEnum::AGENCY_SHOP_TYPE GetShopType() const { return m_eShopType; }
		void SetShopType(SEnum::AGENCY_SHOP_TYPE val) { m_eShopType = val; }
#else // SERV_UPGRADE_TRADE_SYSTEM
		CX2SquareUnit::PERSONAL_SHOP_TYPE GetShopType() const { return m_eShopType; }
		void SetShopType(CX2SquareUnit::PERSONAL_SHOP_TYPE val) { m_eShopType = val; }
#endif // SERV_UPGRADE_TRADE_SYSTEM

		void UpdateEquippedEmblem();
				
		void AddHistoryUnitState();
		D3DXVECTOR3 GetHistorySummary(std::vector<SyncUnitState> &vecSyncData);
#ifndef RIDING_SYSTEM
		void SetActId_LUA( SYNC_UNIT_STATE actId );		/// 2013.03.27 현재 사용하지 않음
#endif //RIDING_SYSTEM

		void SetFieldUnit() { m_bTFieldUnit = true; }
		bool GetFieldUnit_LUA() { return m_bTFieldUnit; }

		void SetFirstGetPacket(bool val) { m_bFirstGetPacket = val; }
		void SetFirstSync(bool val) { m_bFirstSync = val; }
		void SetSyncOUStartPos(D3DXVECTOR3 vPos) { m_SyncOUStartPos = vPos; }

		//bool GetIsOtherLine() { return m_bFindOtherLine; }
		//D3DXVECTOR3 GetOtherLinePos() { return m_vOtherLinePos; }
		void DoJumpUp();
		bool IsMyUnit() const { return m_bMyUnit; }

		bool GetNowJump();
		UidType GetPartyUid() { return m_iPartyUID; }
		void SetPartyUid(UidType iPartyUid) { m_iPartyUID = iPartyUid; }

		const CX2SquareUnit::CommonState& GetStateID() const { return m_StateID; }

		CKTDXDeviceSound* PlaySound_LUA( const char* szSoundFileName, bool b3DSound );		

#ifdef UNIT_EMOTION
		bool PlayEmotion(CX2Unit::EMOTION_TYPE eEmotionType);
		CX2Unit::EMOTION_TYPE GetNowEmotionId() { return m_ePlayedEmotion; }
#endif

#ifdef SERV_ED_MONITORING_IN_GAME
		int GetED() { return m_iED; }
#endif

#ifdef RIDING_SYSTEM
		static const WCHAR* GetSquareUnitScriptFileName( CX2Unit::UNIT_CLASS eUnitClass );
#else //RIDING_SYSTEM
		static const std::wstring GetSquareUnitScriptFileName( CX2Unit::UNIT_CLASS eUnitClass );
#endif //RIDING_SYSTEM

#ifdef HEAD_ATTACHED_CAMERA_TEST

		bool BoneAttachedCamera( const wstring& boneName, const D3DXVECTOR3& vEyeOffset /*= D3DXVECTOR3(0, 0, 0) */ );
		bool BoneAttachedCamera( const CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, const D3DXVECTOR3& vEyeOffset /*= D3DXVECTOR3(0, 0, 0) */ );
		bool GetOnRollerCoaster() const { return m_bOnRollerCoaster; }

#endif HEAD_ATTACHED_CAMERA_TEST

#ifdef SERV_PET_SYSTEM
		const CX2GameUnit::PhysicParam& GetPhysicParam() const { return m_PhysicParam; }
#endif

#ifdef ADDED_RELATIONSHIP_SYSTEM
		SEnum::WEDDING_STATUS GetWeddingStatus() const { return m_eWeddingStatus; }
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
#ifdef ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터
		// 점핑 이벤트 DLG 등장 중, 입력을 받지 않기 위해 proteced 에서 public 으로 변경
		void		SetCanNotInputTime( float fTime_ ){ m_fCanNotInputTime = fTime_; }
#endif // ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터
#endif //MODIFY_LINEMAP_JUMPUP_IN_VILLAGE

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
		bool		IsPlayAvatarEmotionSoundWithouEmotion ( CX2Unit::EMOTION_TYPE eEmotionId );
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨


	protected:
		void		PhysicProcess();

		
		void		StateStart();
		void		StateFrameMove();
		void		StateEventProcess();
		void		StateEnd();

		void		SetSyncEventId(u_char eventId);
		void		SetSyncEventProcess();
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool		EventCheck( float fTime );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


#ifdef MODIFY_LINEMAP_JUMPUP_IN_VILLAGE

#ifndef ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터
		void		SetCanNotInputTime( float fTime_ ){ m_fCanNotInputTime = fTime_; }
#endif // ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터

#endif //MODIFY_LINEMAP_JUMPUP_IN_VILLAGE

        bool                            m_bBackgroundLoad;

		bool							m_bFirstGetPacket;	/// 없어도 될듯 (false인지 아닌지를 체크하는데, false로 사용되는 경우가 없음)
		bool							m_bInit;			/// Init()함수가 수행되었는지 여부 (안되었다면, FrameMove/Render 등이 수행안됨)
		bool							m_bMyUnit;
		
		UidType							m_UnitUID;
		CX2Unit*						m_pUnit;			/// 내 유닛이 아닌 경우에는 소멸자에서 삭제 한다.
		wstring							m_ScriptFileName;		
		KLuaManager						m_LuaManager;

		float							m_fTime;			/// UnitViewer의 OnFrameMove에 넘기는 용도(굳이 가지고 있지 않고 변수로 넘겨도 될듯)
		float							m_fElapsedTime;		/// 사실 이것도 가지고 있을 필요가 없음 (g_pKTDXApp->GetElapsedTime()을 사용하면 됨)
		bool							m_bShow;			/// 현재 쓰이는 곳 없음

		CX2GUUser::InputData			m_BeforeInputData;	/// 현재 쓰이는 곳 없음
		CX2GUUser::InputData			m_InputData;
		CX2GUUser::InputData			m_OrgInputData;		/// 현재 쓰이는 곳은 없음

		CX2UnitViewerUI*				m_pUnitViewer;		/// SD가 아닌 캐릭터의 모델링
        
		CX2GameUnit::PhysicParam		m_OrgPhysicParam;	/// Script에 저장되어 있는 그대로의 값
		CX2GameUnit::PhysicParam		m_PhysicParam;		/// 현재 값

		D3DXVECTOR3						m_Scale;		/// 유저의 스케일 UnitViwer의 Scale을 위해 필요
		D3DXVECTOR3						m_DirDegree;	/// UnitViewer의 Rotate를 위해 필요. (UnitViwer 안쓰면 필요가 없으려나...?)
		D3DXVECTOR3						m_DirVector;	/// 방향벡터

		SyncData						m_SyncBefore;	/// 이제는 사용하지 않아도 됨
		SyncData						m_SyncData;		/// KEGS_SQUARE_UNIT_SYNC_DATA_REQ를 그냥 멤버로 가질수있다면 이게 더 나을지도... 하지만 내부의 VECTOR3가 다르군...
		StateData						m_StateData;	/// 스테이트내의 데이터 (StateStart에서 스크립트로부터 읽어들임)
		//bool							m_bFall;
		bool							m_bFootOnLine;	/// true 면 방향벡터의 Y값을 무시
		D3DXVECTOR3						m_LandPosition;		/// 과연 랜드 포지션이란 정보가 그대로 필요한가...그냥 현재 POS에 Land Y값만 따로 가지고 있어도 될 듯 한데... 아니면 그 반대도 괜찮을 듯
		
		map<char,string>				m_StateNameMap;		/// 상태ID, 상태명
		map<string,char>				m_StateIDMap;		/// 상태명, 상태 ID

		CommonState						m_StateID;			/// Wait, Walk, JumpUp, JumpDown, Dash, DashJumpUp, DashJumpDown에 대한 아이디를 가지고 있는 구조체
		


		int								m_SendPacketCount;	/// 거래 광장에서 사용하는 패킷을 보낸 횟수 (초당 100개 이하)
		float							m_fPacketTime;	/// 거래 광장에서 사용하는 초당 패킷을 100개 이하로만 보내도록 컨트롤 하기 위한 second를 저장하는 변수

		bool							m_bPlanRender;	/// true 면 SD 모드로 실행
        //CKTDXDeviceXMesh*				m_pPlanMesh;
		//CKTDXDeviceXET*					m_pPlanMeshXET;

        /// 내부적으로 관리하는 객체이므로 smart pointer를 사용하지 않는다.
        /// - jintaeks on 2009-01-12, 16:47
		CKTDGXSkinAnim*					m_pXSkinAnim;		/// SD 캐릭용 Skin Anim
		CKTDXDeviceXSkinMesh*			m_pMesh;			/// SD 캐릭용 Skin mesh	

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHeadMarkerMy;		/// 파티가 아닐때의 자신의 캐릭터를 나타내는 빨간줄 화살(?)표시
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHeadMarkerPartyMy;	/// 파티일 때의 자신의 캐릭터를 나타내는 빠란 줄 화살(?) 표시
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHeadMarkerParty;		/// 파티원을 나타내는 파란 화살(?) 표시

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqEmblem;			/// 칭호

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CKTDGParticleSystem::CParticleHandle			m_hHeadMarkerParticleMy;	/// 파티가 아닐때의 자신의 캐릭터를 나타내는 빨간줄 화살(?)표시
		CKTDGParticleSystem::CParticleHandle			m_hHeadMarkerParticlePartyMy;	/// 파티일 때의 자신의 캐릭터를 나타내는 빠란 줄 화살(?) 표시
		CKTDGParticleSystem::CParticleHandle			m_hHeadMarkerParticleParty;	/// 파티원을 나타내는 파란 화살(?) 표시

		CKTDGParticleSystem::CParticleHandle			m_hPart_Emblem_200;			/// 칭호
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CKTDGParticleSystem::CParticle*					m_pHeadMarkerParticleMy;	/// 파티가 아닐때의 자신의 캐릭터를 나타내는 빨간줄 화살(?)표시
		CKTDGParticleSystem::CParticle*					m_pHeadMarkerParticlePartyMy;	/// 파티일 때의 자신의 캐릭터를 나타내는 빠란 줄 화살(?) 표시
		CKTDGParticleSystem::CParticle*					m_pHeadMarkerParticleParty;	/// 파티원을 나타내는 파란 화살(?) 표시

		CKTDGParticleSystem::CParticle*					m_pPart_Emblem_200;			/// 칭호
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE




		PERSONAL_SHOP_STATE				m_PersonalShopState;	/// 공통보다는 광장에서만 필요할 듯

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
		SEnum::AGENCY_SHOP_TYPE			m_eShopType;			/// 공통보다는 광장에서만 필요할 듯
#else // SERV_UPGRADE_TRADE_SYSTEM
		PERSONAL_SHOP_TYPE				m_eShopType;			/// 공통보다는 광장에서만 필요할 듯
#endif // SERV_UPGRADE_TRADE_SYSTEM
		
		wstring							m_PersonalShopName;		/// 공통보다는 광장에서만 필요할 듯

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hShop;		/// 공통보다는 광장에서만 필요할 듯
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPremiumShop;	/// 공통보다는 광장에서만 필요할 듯
//#ifdef TITLE_SYSTEM
        int m_iTitleId;	/// 현재 착용 중인 칭호의 아이디
//#endif

		CKTDGParticleSystem *m_pParticleSystem;		/// 마을에서는 g_pTFieldGame->GetUiParticle(), 광장에서는 g_pSquareGame->GetMajorParticle()
		

		// 각 스테이트와 스테이트 이벤트 맵핑
		u_char		m_iActID;						/// 현재 키보드 입력에 대한 스테이트ID (비트마스크)

		// 자신의 스테이트 히스토리를 관리하기위한 정보		
		std::vector<SyncUnitState>		m_vecSyncUnitState;		/// 키보드 입력을 받아서 State로 변환한 데이터 저장

		// 타유저의 스테이트 히스토리를 관리하기위한 정보
		D3DXVECTOR3						m_SyncOUStartPos_Old;	// Packet을 받기 이전의 m_SyncOUStartPos 정보
		D3DXVECTOR3						m_SyncOUStartPos;		/// Packet으로 받은 데이터를 m_SyncData.m_Pos 에 대입 할 때 까지 가지고 있을 용도로 사용
		std::vector< KFieldFrameInfo >	m_vecFieldFrame;		/// 다른 유저가 수행했던 스테이트 정보 (Packet을 통해 전달 받음)
		u_char							m_iOUFrameCount;		/// 현재 Sync 패킷을 받은 이후 부터 다음 Sync 패킷을 받을 때까지 경과된 FrameCount
		u_char							m_iOUNextFrameCount;	/// m_vecFieldFrame의 m_iNowStateIndex 에 해당 하는 스테이트를 몇 프레임이나 수행 해야 다음 스테이트로 넘어갈 수 있는 지에 대한 정보
		int								m_iNowStateIndex;		/// m_vecFieldFrame의  중 현재 수행 중인 index
		bool							m_SyncIsRight_Old;		/// Packet을 받기 이전의 m_SyncIsRight
		bool							m_SyncIsRight;			/// Packet으로 받은 데이터를 m_SyncData.m_bIsRight 에 대입 할 때까지 가지고 있을 용도로 사용
		bool							m_bFirstSync;			/// 패킷 처리 후 패킷을 받았는지 여부

		bool							m_bTFieldUnit;			/// 마을 유닛인지 광장유닛인지를 알기위한 정보

		CKTDXDeviceTexture*				m_pTextureRank;			/// 대전 랭크 표시 텍스쳐
#ifdef SERV_INTEGRATION
		//{{ oasis907 : 김상윤 [2010.5.17] // 던전 대전 통합 서버 아이콘
		CKTDXDeviceTexture*				m_pTextureServer;		/// 던전 라운지의 경우에만 보여지는 캐릭터 명 옆의 서버군 아이콘
		//}}
#endif SERV_INTEGRATION

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		map<float,bool>							m_EventTimeStamp;	/// 사운드 플레이 데이터의 EventCheck를 위한 TimeStamp (SOUND_PLAY를 사용하는 곳이 없다면 이것도 사용할 이유가 없을듯...)
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		vector<CX2GameUnit::SoundPlayData>		m_vecSoundPlayData;	/// 각 스테이트 Start에서 로드한 SOUND_PLAY 에 대한 데이터 (사용하는 곳이 없음)

		UidType							m_iPartyUID;		/// 파티 Unique 아이디

#ifdef UNIT_EMOTION
		CX2Unit::EMOTION_TYPE			m_ePlayedEmotion;	/// 플레이된 이모션
#endif

		bool m_bMixedEmotion[AVATAR_EMOTION_NUM];		/// 이모션 X 파일 이미 Mix 되었는지 아닌지 판단
		CKTDXDeviceSound*				m_pAvatarEmotionSound;	/// 특정 아바타 착용시 이모션 사용하면 나오는 사운드

#ifdef HEAD_ATTACHED_CAMERA_TEST
		bool m_bOnRollerCoaster; 		
#endif HEAD_ATTACHED_CAMERA_TEST

#ifdef SERV_ED_MONITORING_IN_GAME
		int m_iED;
#endif

#ifdef MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
		bool		m_bForceMoveSpeed;
		float		m_fCanNotInputTime;
#endif //MODIFY_LINEMAP_JUMPUP_IN_VILLAGE

#ifdef HANABI_VILLAGE
		float m_fRedShowTimer;
		float m_fBlueShowTimer;
#endif HANABI_VILLAGE

#ifdef FIELD_CHARACTER_SCREEN_SHOT
		bool		m_bAlphaScreenShot;
#endif

#ifdef RIDING_SYSTEM
		wstring			m_wstrLobbyWaitMotionName;
		CKTDXCheckElapsedTime m_ElapsedTimeRidingWaitHabit;		/// 습관 상태 대기 시간 지정
		//CX2RidingPetPtr	m_ptrRidingPet;		/// 탈 것 스마트 포인트
#endif //RIDING_SYSTEM

#ifdef ADDED_RELATIONSHIP_SYSTEM
		SEnum::WEDDING_STATUS m_eWeddingStatus;
#endif //ADDED_RELATIONSHIP_SYSTEM

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
		bool		m_bIsPlayAvatarEmotionSoundWithoutEmotion;
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨

#ifdef SERV_GROW_UP_SOCKET
		int		m_iTitleLevel;
#endif SERV_GROW_UP_SOCKET
};

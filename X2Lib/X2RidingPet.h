#ifdef RIDING_SYSTEM
#pragma once
/**
	@file	X2RidingPet.h
	@author	JHKang
	@brief	탈 것 정의
	@since  2013-03-26
	
	@date	JHKang on 2013-03-26,
			- 클래스 생성
	@date	JHKang on 2013-03-28,
			- 기본 함수 생성
	@date	JHKang on 2013-04-01,
			- 리소스 로딩 관련 함수 생성
	@date	JHKang on 2013-04-02,
			- 백그라운드 로딩 관련 함수 생성
			- 스크립트 파싱 부분 작성
	@date	JHKang on 2013-04-10
			- 동기화 데이터 작성
			- 컨디션 데이터 삽입
			- Bimap을 사용한 데이터 Index
			- 생성자 함수 변경 -> 탈 것이 계속해서 소멸되는 문제 발생
	@date	JHKang on 2013-04-11
			- 기울기가 있는 라인맵에 대한 예외 처리
			- "F2" Reset 치트 삽입
	@date	JHKang on 2013-04-14
			- 던전 / 필드 내에서 탈 것 사용 가능하도록 수정
	@date	JHKang on 2013-04-16
			- 던전 / 필드 내에서 탈 것 Walk, Wait, Damage, Jump, Dash 관련 상태 작성
	@date	JHKang on 2013-04-17
			- 던전 / 필드 내에서 탈 것 공격 상태 작성
			- Physic 관련 파라미터 파싱 내용 추가
	@date	JHKang on 2013-04-28
			- 특수 이동 기능 추가
	@data	JHKang on 2013-06-02
			- 스콜피온 탈 것 추가

*/
#include <boost/bimap/bimap.hpp>
#include "./X2SoundCloseManager.h"

class CX2RidingPet;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2RidingPet> CX2RidingPetPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2RidingPet> CX2RidingPetPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

#pragma region CX2RidingPet
/** @class : CX2RidingPet
	@brief : 탈 것의 기능을 정의하는 클래스
	@date  : 2013/03/27
*/
class CX2RidingPet : public CKTDGObject, public CKTDXDeviceHolder
{
#pragma region public
public:
	#pragma region RIDING_STATE_ID
	enum RIDING_STATE_ID
	{
		RSI_NONE	= 0,	/// 상태 없음
		RSI_LOADING,		/// 로딩
		RSI_READY,			/// 준비
		RSI_PLAY,			/// 사용 중
		RSI_END,			/// 상태 마지막
	};
	#pragma endregion 탈 것 상태 열거체

	#pragma region RIDING_SPECIAL_MOVE
	enum RIDING_SPECIAL_MOVE
	{
		RSM_NONE			= 0,
		RSM_AIR_DASH,		/// 공중 대쉬
		RSM_DOUBLE_JUMP,	/// 이단 점프
		RSM_BOOST_DASH,		/// 부스트 대쉬
		RSM_PASS_DASH,		/// 통과 대쉬
		RSM_END,
	};
	#pragma endregion 탈 것 특수 이동

#ifdef X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	#pragma region InitDeviceData
	struct InitDeviceData
	{
		std::vector<std::wstring>    m_vecstrReadyTexture;		/// 텍스처 이미지명
		std::vector<std::wstring>    m_vecstrReadyXMesh;		/// 메쉬명
		std::vector<std::wstring>    m_vecstrReadyXSkinMesh;	/// 스킨 메쉬명
		std::vector<std::wstring>    m_vecstrReadySound;		/// 사운드명
		std::vector<std::wstring>	 m_vecstrReadyXET;			/// XET 명

		void AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const;

	};
	#pragma endregion 초기화 구조체

	#pragma region InitData
	struct InitData
	{
		InitDeviceData	m_device;		/// 디바이스 구조체
		void			AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const;
	};
	#pragma endregion
#endif //X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	#pragma region SoundPlayData
	struct SoundPlayData 
	{
		float		m_SoundPlayTime;		/// 재생 시간
		wstring		m_SoundPlayName;		/// 파일 이름
		int			m_SoundPlayRate;		/// 재생 빈도
		bool		m_bOnlyIfMyUnit;		/// 나만 들어
		float		m_fMaxSoundDistance;	/// 최대 거리
		bool		m_b3DSound;				/// 3D 사운드
	};
	#pragma endregion 사운드 구조체

	#pragma region RidingPetSyncData
	struct RidingPetSyncData
	{
		bool			m_bIsRight;		/// 방향
		bool			m_bStateChange;	/// 상태 변경
		bool			m_bFootOnLine;	/// 라인을 밟고 있는가?

		char			m_nowState;			/// 상태
		char			m_StateChangeNum;	/// 상태 변경 번호
		D3DXVECTOR3		m_vPos;				/// 위치

		UCHAR			m_ucLastTouchLineIndex;		/// 최근 접촉한 라인 인덱스 번호

		RidingPetSyncData()
		{
			Init();
		}

		void Init()
		{
			m_bIsRight = true;
			m_bStateChange = false;

			m_nowState			= -1;
			m_StateChangeNum	= 0;
			INIT_VECTOR3( m_vPos, 0, 0, 0 );

			m_ucLastTouchLineIndex	= 0;
		}

		void SetData( IN RidingPetSyncData* pSyncData_ )	// 서버 용으로 바꿔야 함
		{
			m_bIsRight		= pSyncData_->m_bIsRight;
			m_bStateChange	= pSyncData_->m_bStateChange;

			m_vPos			= pSyncData_->m_vPos;

			m_ucLastTouchLineIndex	= pSyncData_->m_ucLastTouchLineIndex;
		}
	};
	#pragma endregion 탈 것 동기화 데이터

	#pragma region RidingPetCondition
	struct RidingPetCondition
	{
		bool		m_bLandConnection;
		float		m_fUnitWidth;
		float		m_fUnitHeight;

		D3DXVECTOR3	m_vDirDegree;
		D3DXVECTOR3	m_vDir;
		D3DXVECTOR3	m_vLandPosition;

		RidingPetCondition()
		{
			Init();
		}

		void Init()
		{
			m_bLandConnection	= true;
			m_fUnitWidth		= 0.f;
			m_fUnitHeight		= 0.f;

			INIT_VECTOR3( m_vDirDegree, 0, 0, 0 );
			INIT_VECTOR3( m_vDir, 0, 0, 0 );
			INIT_VECTOR3( m_vLandPosition, 0, 0, 0 );
		}
	};
	#pragma endregion 구조체

	#pragma region RidingPetInfo
	struct RidingPetInfo
	{
		UidType	m_Uid;			/// 탈 것 UnitID
		int		m_Id;			/// 탈 것 ID

		wstring	m_wstrName;		/// 탈 것 이름
		bool	m_bUse;			/// 사용 여부
		
		RIDING_SPECIAL_MOVE	m_eSpecialMove;		/// 특수 이동

		KProtectedType<float>	m_fStamina;	/// 체력

		RidingPetInfo()
		{
			Init();
		}

		RidingPetInfo( IN const KRidingPetInfo& rhs_ )
			: m_wstrName( L"" )
		{
			m_Uid		= rhs_.m_iRidingPetUID;
			m_Id		= rhs_.m_usRindingPetID;
			m_fStamina	= rhs_.m_fStamina;
			m_bUse		= true;
		}

		void Init()
		{
			m_Uid = -1;
			m_Id = -1;
			m_wstrName = L"";
			m_bUse = false;
			m_fStamina = 0;
			m_eSpecialMove = RSM_NONE;
		}

		bool Verify()
		{
			if( m_fStamina.Verify() == false )
				return false;
			else
				return true;
		}
	};
	#pragma endregion 탈 것 정보 구조체

	#pragma region PhysicParam
	struct PhysicParam
	{
		D3DXVECTOR2	m_vNowSpeed;		/// 현재 스피드X,Y
		D3DXVECTOR2	m_vPassiveSpeed;	/// 이 스피드로 계속 보정해준다

		float			fDownAccel;		/// 하강 가속도
		float			fReloadAccel;	/// 전진 속도 감쇄율
		float			fGAccel;		/// 중력 가속도
		float			fMaxGSpeed;		/// 최대 하강 속도 Y
		float			fWalkSpeed;		/// 걷기 속도
		float			fRunSpeed;		/// 달리기 속도
		float			fJumpSpeed;		/// 일반 점프 상승속도
		float			fDashJumpSpeed;	/// 대시 점프 상승속도
				
		PhysicParam()
		{
			INIT_VECTOR2( m_vNowSpeed, 0.0f, 0.0f );
			INIT_VECTOR2( m_vPassiveSpeed, 0.0f, 0.0f );
			fDownAccel		= 0.0f;
			fReloadAccel	= 0.0f;
			fGAccel			= 0.0f;
			fMaxGSpeed		= 0.0f;
			fWalkSpeed		= 0.0f;
			fRunSpeed		= 0.0f;
			fJumpSpeed		= 0.0f;
			fDashJumpSpeed	= 0.0f;
		}
	};
	#pragma endregion 물리 파라미터 구조체

	#pragma region STATE_CHANGE_TYPE
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

		SCT_AI_MESSAGE_END,

	};
	#pragma endregion 상태 변경 타입

	#pragma region RidingPetStateData
	struct RidingPetStateData
	{
		int			m_StateID;
		
	public:
		~RidingPetStateData() 
		{			
		}

		RidingPetStateData()
		{
			Init();
		}

		virtual void Init()
		{
			m_StateID			= RSI_NONE;
		}

		// 주의!! 이 copy constructor, assignment operator 삭제하면 안됨
		RidingPetStateData( const RidingPetStateData& t ) 
		{ 
			*this = t; 
		}    

		RidingPetStateData& operator=( const RidingPetStateData& data )
		{
			m_StateID			= data.m_StateID;

			return *this;
		}
	};
	#pragma endregion 상태 데이터

	#pragma region typedef
	typedef boost::bimaps::bimap< char, string > StateBiMap;				/// 기존 두 개의 맵을 사용한 것을 bimap으로 바뀜
	typedef boost::bimaps::bimap< char, string >::value_type BiMapValue;	/// biMap에 접근하기 위한 값
	typedef std::map< char, RidingPetStateData > RidingPetStateDataMap;		/// StateDataMap
	typedef map< float, bool > EventTimeMap;								/// 이벤트 타이머 맵
	#pragma endregion 타입 정의

#pragma endregion enum, struct, Inner Class

#pragma region public
public:
	CX2RidingPet( void );
	virtual ~CX2RidingPet( void );

	static CX2RidingPetPtr CreateRidingPetPtr()
	{
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        CX2RidingPetPtr pObject( new CX2RidingPet );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		CX2RidingPetPtr pObject( new CX2RidingPet, CKTDGObject::KTDGObjectDeleter() );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		return pObject;
	}

	virtual HRESULT			OnFrameMove( IN double fTime_, IN float fElapsedTime_ );
	virtual RENDER_HINT		OnFrameRender_Prepare();
	virtual void			OnFrameRender_Draw();
	void					CommonCameraFrameMove();
	void					CommonFrameMove_TimedEvent();

	virtual HRESULT			OnResetDevice();
	virtual HRESULT			OnLostDevice();
	
	#ifdef X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	static void InitDevice( IN InitDeviceData& OutDevice_, IN KLuaManager& luaManager_ );
	static void InitMotion( IN InitDeviceData& OutDevice_, IN KLuaManager& luaManager_ );
	static void InitInit( IN InitData& OutInit_, IN KLuaManager& luaManager_ );

	void SetRidingPet( IN lua_State* pPetLuaState_, IN UidType uUserUid_ );	
	void SetGameRidingPet( IN lua_State* pPetLuaState_, IN UidType uUserUid_ );
	KLuaManager& GetLuaManager() { return m_LuaManager; }
	#endif //X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	void SetInfo( IN const RidingPetInfo& ridingPetInfo_, IN RIDING_SPECIAL_MOVE eSpecialMove_ );
	const RidingPetInfo &GetInfo() const { return m_Info; }

	void VerifyRindingPetInfo() { m_Info.Verify(); }

	UidType GetUserUid() const { return m_UserUid; }
	void SetUserUid( IN UidType userUid ) { m_UserUid = userUid; }
	void GetUserInfo( IN D3DXVECTOR3& vPos, IN bool& bIsRight_ );

	bool GetLoaded() const { return m_bLoaded; }
	const D3DXVECTOR3& GetPos() const { return m_SyncData.m_vPos; }
	void SetPos( IN D3DXVECTOR3& vPos_, IN bool bIsRight_ );
	bool InitPos();
	
	D3DXVECTOR3 GetBonePos( IN const WCHAR* pBoneName_ ) const
	{
		return m_pXSkinAnim->GetCloneFramePosition( pBoneName_ );
	}
	bool GetBonePos( IN const WCHAR* pBoneName_, OUT D3DXVECTOR3& vPos_ ) const
	{
		return m_pXSkinAnim->GetCloneFramePosition( pBoneName_, vPos_);
	}

	const int GetNowState() { return m_SyncData.m_nowState; }

	const CKTDGXSkinAnim* GetXSkinAnim() const
	{
		if ( m_bLoaded && NULL != m_pXSkinAnim )
			return m_pXSkinAnim.get();
		else
			return NULL;
	}

	CKTDGXSkinAnimPtr GetXSkinAnimPtr() const { return m_pXSkinAnim; }

	CKTDGXSkinAnimPtr GetXSkinAnimFuturePtr() const { return m_pXSkinAnimFuture; }

	int GetStateID( IN const char* wcState_ )
	{
		if ( NULL == wcState_ )
			return RSI_NONE;

		return GetStateID( std::string( wcState_ ) );
	}

	int GetStateID( IN const std::string& strInState_ )
	{
		StateBiMap::right_const_iterator rightIter = m_StateBiMap.right.find( strInState_ );

		if ( rightIter == m_StateBiMap.right.end() )
			return RSI_END;

		return rightIter->second;
	}

	float GetUnitWidth() { return m_ConditionData.m_fUnitWidth; }
	float GetUnitHeight() { return m_ConditionData.m_fUnitHeight; }
	const PhysicParam& GetPhysicParam() const { return m_PhysicParam; }

	void StateChangeFuture( IN int iStateID_ );
	void StateChangeFuture( IN const char* stateName_ );

	void StateChange( IN int iStateID_ );
	void StateChange( IN const char* stateName_ );
	void StateChange_LUA( IN const char* stateName_ );

#ifdef FIX_HIT_FLY_RIDING_PET_MOTION // 김태환
	const RidingPetCondition GetConditionData() const { return m_ConditionData; }
#endif //FIX_HIT_FLY_RIDING_PET_MOTION

#pragma endregion function

#pragma region private
private:
	bool IsValidUser();

	void InitDevice();
	void InitSystem();
	void InitMotion();
	void InitPhysic();
	void InitState();

	void ClearState();
	void DoFrameMove();
#pragma endregion function

#pragma region private
private:
	KLuaManager		m_LuaManager;	/// 루아 스크립트 매니저
	
	bool	m_bForceBillBoard;		/// 강제 빌보드 여부
	bool	m_bForceColor;			/// 강제 색상 변환
	bool	m_bForceColorChange;	/// 물속 강제 색상 변환
	bool	m_bGame;				/// 던전 / 필드 게임
	bool	m_bLoaded;				/// 불러짐
	bool	m_bMine;				/// 나의 탈 것
	bool	m_bIgnoreLineSpeed;		/// 라인 스피드 무시
	bool	m_bInit;				/// 초기화 완료
	bool	m_bInitPos;				/// 위치 초기화
	bool	m_bSetData;				/// 데이터 설정
	bool	m_bUnderWaterHead;		/// 물속?
	
	wstring		m_wstrAniTex;		/// Texture 명
	wstring		m_wstrChangeTex;	/// 변경될 Texture 명
	wstring		m_wstrMotionName;	/// 모션 명
	wstring		m_wstrRidingMotion;	/// 유저가 타는 모션
		
	CKTDXDeviceXSkinMesh*	m_pMotion;		/// 스킨 메쉬
	CKTDGXSkinAnimPtr		m_pXSkinAnim;	/// 애니메이션
	CKTDGXSkinAnimPtr		m_pXSkinAnimFuture;	/// 애니메이션

	CKTDXDeviceXET*			m_pAniXET;		/// XET
	CKTDXDeviceXET*			m_pChangeXET;	/// 바뀔 XET

	CKTDGXRenderer::RenderParam		m_RenderParam;	/// 렌더링 파라미터
	D3DXCOLOR		m_cColor;			/// 탈 것 색상
	D3DXCOLOR		m_cLineUnitColor;	/// 물속 관련
	
	RidingPetStateData	m_StateData;		/// 상태 데이터
	RidingPetCondition	m_ConditionData;	/// 컨디션 데이터 : 유저의 정보를 받아야함
	RidingPetInfo		m_Info;				/// 탈 것 정보
	RidingPetSyncData	m_SyncData;			/// 싱크 데이터 : 유저의 정보를 받아야함
	
	PhysicParam		m_PhysicParam;			/// 실제 물리 계산에 사용되는 파라미터

	StateBiMap				m_StateBiMap;		/// 상태ID, 상태명
	RidingPetStateDataMap	m_StateDataMap;		/// 
	
	UidType		m_UserUid;		/// 현재 사용자

	float		m_fElapsedTime;		/// 시간
	float		m_fNowStateTimer;	/// 현재 상태 시간
	float		m_fScale;			/// 크기
	
	vector<SoundPlayData>	m_vecSoundPlayData;		/// 사운드 데이터
#pragma endregion variable

#pragma region public
public:
	int		m_StartState;				/// 던전 시작
	int		m_WaitState;				/// 대기
	int		m_WaitHabitState;			/// 습관
	int		m_WalkState;				/// 걷기
	int		m_JumpUpState;				/// 점프 업
	int		m_JumpDownState;			/// 점프 다운
	int		m_JumpLandingState;			/// 착지
	int		m_DashState;				/// 대시
	int		m_DashEndState;				/// 대시 종료
	int		m_DashJumpUpState;			/// 대시 점프
	int		m_DashJumpLandingState;		/// 대시 점프 착지
	int		m_DamageFrontState;			/// 피격 앞
	int		m_DamageBackState;			/// 피격 뒤
	int		m_DyingState;				/// 죽음
	int		m_AttackZState;				/// 공격 Z
	int		m_JumpAttackZState;			/// 점프 공격 Z
	int		m_AttackXState;				/// 공격 X
	int		m_SpecialAttackState;		/// 특수 공격
	int		m_SitReadyState;			/// 앉기 시작
	int		m_SitWaitState;				/// 앉기
	int		m_StandUpState;				/// 일어나기
	int		m_SpecialMoveState;			/// 특수 이동
#pragma endregion variable

};
#pragma endregion 탈 것 클래스

#ifdef X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
struct RidingPetInitTemplet
{
	CX2RidingPet::InitData	m_init;
};
#endif X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

#endif //RIDING_SYSTEM
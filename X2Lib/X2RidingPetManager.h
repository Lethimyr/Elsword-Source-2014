#ifdef RIDING_SYSTEM
#pragma once
/**
	@file	X2RidingPetManager.h
	@author	JHKang
	@brief	탈 것 관리 클래스
	@since  2013-04-02
	
	@date	JHKang on 2013-04-02,
			- 클래스 생성
	@date	JHKang on 2013-04-02,
			- Riding Templet 관련 함수 생성
	@date	JHKang on 2013-04-03,
			- BackGround 로딩 부분 등 리소스 로딩 부분 생성
	@date	JHKang on 2013-04-10
			- 서버와 통신할 때 사용할 Handler 함수 생성 -> 현재 클라전용으로 작성됨
	@date	JHKang on 2013-04-28
			- 특수 이동 기능 추가
	@date	JHKang on 2013-05-09
			- 매니저 객체를 싱글톤 객체로 변경( 코드 피드백 )
	@date	JHKnag on 2013-06-02
			- 스콜피온 탈 것 추가
*/
#include "X2GUUser.h"
#include "X2GUChung.h"
#include "X2GUArme_VioletMage.h"
#include "X2GULire_ElvenRanger.h"

class CX2RidingPet;

#ifdef X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
struct RidingPetInitTemplet;
#endif //X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

#pragma region CX2RidingPetManager
/** @class : CX2RidingPetManager
	@brief : 탈 것 관리 클래스
	@date  : 2013/04/02
*/
class CX2RidingPetManager
{
#pragma region public 
public:
	#pragma region sRidingPetInfo
	struct sRidingPetInfo : public KRidingPetInfo
	{
		int					  m_iPetListOrder;		// 펫 리스트 정렬 순서 ( 마지막 탑승 시간에 의한 정렬 )
		KProtectedType<float> m_fProtectStamina;

		sRidingPetInfo():KRidingPetInfo(),
			m_fProtectStamina(0.f)
		{}

		sRidingPetInfo(const KRidingPetInfo& KRidingPetInfo_)
		{
			m_iRidingPetUID			= KRidingPetInfo_.m_iRidingPetUID;
			m_usRindingPetID		= KRidingPetInfo_.m_usRindingPetID;
			m_fStamina				= KRidingPetInfo_.m_fStamina;
			m_wstrDestroyDate		= KRidingPetInfo_.m_wstrDestroyDate;

			m_fProtectStamina		= KRidingPetInfo_.m_fStamina;
			m_iPetListOrder		= static_cast<UINT>(KRidingPetInfo_.m_iLastUnSummonDate);
		}

		sRidingPetInfo& operator=( const KRidingPetInfo& KRidingPetInfo_ )
		{
			m_iRidingPetUID			= KRidingPetInfo_.m_iRidingPetUID;
			m_usRindingPetID		= KRidingPetInfo_.m_usRindingPetID;
			m_fStamina				= KRidingPetInfo_.m_fStamina;
			m_wstrDestroyDate		= KRidingPetInfo_.m_wstrDestroyDate;	

			m_fProtectStamina		= KRidingPetInfo_.m_fStamina;
			m_iPetListOrder		= static_cast<UINT>(KRidingPetInfo_.m_iLastUnSummonDate);
			return *this;
		}

		bool Verify()
		{
			if( m_fProtectStamina.Verify() == false )
			{
				g_pKTDXApp->SetFindHacking( true );
				return false;
			}
			else
				return true;
		}
	};
	#pragma endregion KRidingPetInfo 구조체를 상속받음

	#pragma region RIDING_PET_UNIT_ID
	enum RIDING_PET_UNIT_ID
	{
		RPUI_NONE			= 0,	/// 없음
		RPUI_NASOD_MOBI,			/// 나소드 모비
		RPUI_KOUCIKA,				/// 코우시카
		RPUI_ANCIENT_PPORU,			/// 에인션트 뽀루
		RPUI_SCORPION_TYPE_R,		/// 퐁고 스콜피온 TYPE-R
		RPUI_ICE_HAMELING,			/// 아이스 하멜링
		RPUI_DARK_HAMELING,			/// 다크 하멜링 
		RPUI_RED_HAMELING,			/// 레드 하멜링 

		// 기획상 변경된 능력치를 해외팀에 반영하지 않기 위해 추가 한 ID ~
		RPUI_NASOD_MOBI_INT     = 8,    /// 고대의 돌진 전차 모비RT
		RPUI_KOUCIKA_INT        = 9,    /// 고대의 바람의 코우시카
		RPUI_ANCIENT_PPORU_INT  = 10,   /// 고대의 에이션트 라이딩 뽀루
		// ~ 기획상 변경된 능력치를 해외팀에 반영하지 않기 위해 추가 한 ID

		RPUI_OBERON_CUSTOM_650	= 11,	///	이브 커스텀-오베론650
		RPUI_ANCIENT_PPORU_NAVER = 12,	///	네이버 채널링 이벤트 에이션트 뽀루

		RPUI_OBERON_CUSTOM_650_BLACK_SKULL = 13,	/// 이브 커스텀 - 블랙 스컬
		RPUI_OBERON_CUSTOM_650_ANGEL_HEART = 14,	/// 이브 커스텀 - 엔젤 하트

		RPUI_END,					/// 끝
	};
	#pragma endregion 탈 것 아이디 열거체

	#pragma region RIDING_PET_CUSTOM_UI_MSG
	enum RIDING_PET_CUSTOM_UI_MSG
	{
		RPCUM_RIDING_ON,
		RPCUM_BASE_INFO,
		RPCUM_DETAIL_INFO,
	};
	#pragma endregion UI 메시지 열거체
	
	#pragma region FaceImage
	struct FaceImage
	{
		wstring m_wstrImageName;
		wstring m_wstrKeyName;

		FaceImage()
		{
			Init();
		}

		void Init()
		{
			m_wstrImageName = L"";
			m_wstrKeyName = L"";
		}
	};
	#pragma endregion 탈 것 이미지 정보

	#pragma region RidingPetSkillInfo
	struct RidingPetSkillInfo
	{
		wstring m_wstrImageName;	/// 이미지 파일명
		wstring m_wstrKeyName;		/// 이미지 키값
		wstring m_wstrSkillDesc;	/// 스킬 명
		KProtectedType<float>	m_fConsumeMana;		/// 마나 소비량
		KProtectedType<float>	m_fCoolTime;		/// 스킬 쿨타임

		RidingPetSkillInfo()
		{
			m_wstrImageName = L"";
			m_wstrKeyName = L"";
			m_wstrSkillDesc = L"";
			m_fConsumeMana = 0.f;
			m_fCoolTime = 0.f;
		}

		void Verify()
		{
			if( m_fConsumeMana.Verify() == false || m_fCoolTime.Verify() == false )
			{
				g_pKTDXApp->SetFindHacking( true );
			}
		}
	};
	#pragma endregion 스킬 정보 구조체

	#pragma region RidingPetBuffInfo
	struct RidingPetBuffInfo
	{
		wstring m_wstrImageName;	/// 이미지 파일명
		wstring m_wstrKeyName;		/// 이미지 키값
		wstring m_wstrSkillDesc;	/// 스킬 명
		BUFF_FACTOR_ID	m_eBuffFactorID;	/// 적용되어야 할 버프 팩터 ID
		
		RidingPetBuffInfo()
		{
			m_wstrImageName = L"";
			m_wstrKeyName = L"";
			m_wstrSkillDesc = L"";
			m_eBuffFactorID = BFI_NONE;
		}
	};
	#pragma endregion 스킬 정보 구조체

	typedef std::map< UidType, CX2RidingPetPtr > RidingPetMap;
	typedef std::vector<CX2RidingPetPtr> RidingPetVec;
	typedef	RidingPetInitTemplet RPInitTemplet;

	#ifdef X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	#pragma region RidingPetInitLuaTemplet
	struct	RidingPetInitLuaTemplet
	{
		RPInitTemplet*	m_pInitTemplet;
		KLuaManager*	m_pLuaManager;

		RidingPetInitLuaTemplet()
		{
			m_pInitTemplet = NULL;
			m_pLuaManager = NULL;
		}
	};
	#pragma endregion 라이딩 펫 루아 관련 백그라운드 로딩
	#endif //X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	#pragma region RidingPetTemplet
	struct RidingPetTemplet
	{
		RIDING_PET_UNIT_ID	m_Uid;		/// 탈 것 Unit ID
		wstring		m_wstrName;			/// 이름
		wstring		m_wstrScripName;	/// 스크립트 이름
		float		m_fIncreaseRate;	/// 최대 기력 증가 비율
		float		m_fRecoveryRate;	/// 기력 회복 비율
		float		m_fMaxStamina;		/// 탈 것 최대 기력


		float		m_fViewerSizeX;	/// 뷰어에서 볼때 사이즈 X
		float		m_fViewerSizeY;	/// 뷰어에서 볼때 사이즈 Y
		float		m_fViewerSizeZ;	/// 뷰어에서 볼때 사이즈 Z

		wstring		m_wstrDescription;	/// 탈 것 설명
#ifdef X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		RidingPetInitLuaTemplet				m_InitLuaTemplet;	/// 템플릿 백그라운드
#endif //X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		FaceImage							m_FaceImage;		/// 펫 UI상의 얼굴 이미지
		std::vector<RidingPetSkillInfo>		m_vecAttackSkill;	/// 공격 스킬 정보 리스트
		std::vector<RidingPetBuffInfo>		m_vecBuff;			/// 버프 정보 리스트
		std::vector<int>					m_vecSocketOption;	/// 버프 소켓 번호
		CX2RidingPet::RIDING_SPECIAL_MOVE	m_eSpecialMove;		/// 특수 이동

		RidingPetTemplet()
		{
			m_Uid			= RPUI_NONE;
			m_wstrName		= L"";
			m_wstrScripName	= L"";
			m_fIncreaseRate = 1.f;
			m_fRecoveryRate = 0.f;
			m_fMaxStamina	= 0;


			m_fViewerSizeX	= 1.f;
			m_fViewerSizeY	= 1.f;
			m_fViewerSizeZ	= 1.f;

			m_wstrDescription = L"";
// 			#ifdef X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
// 			m_InitLuaTemplet.
// 			#endif //X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
			m_FaceImage.Init();
			m_vecAttackSkill.clear();
			m_vecBuff.clear();
			m_vecSocketOption.clear();
			m_eSpecialMove = CX2RidingPet::RSM_NONE;
		}

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		~RidingPetTemplet();
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	};

	typedef std::map< UINT, RidingPetTemplet* > RidingPetTempletMap;
#pragma endregion 탈 것 정보를 가지는 템플릿

#pragma endregion enum, struct, Inner Class

#pragma region public
public:
	static CX2RidingPetManager* GetInstance();
	static void DestroyInstance();

	HRESULT	OnFrameMove( IN double fTime_, IN float fElapsedTime_ );
	HRESULT OnFrameRender();

	void NotifyShowObjectChanged( IN UidType uUserUid_, IN bool bVal_ );
	void SetShowAll( IN bool bVal_ );
	bool GetShowAll()
	{
		return m_bShow;
	}
	void SetShow( IN UidType uUserUid_, IN bool bVal_ );
	bool GetShow( IN UidType uUserUid_ );
	const bool GetDirectSummon(){ return m_bIsDirectSummon; }
	bool UICustomEventProc( IN HWND hWnd_, IN UINT uMsg_, IN WPARAM wParam_, IN LPARAM lParam_ );
	bool UIServerEventProc( IN HWND hWnd_, IN UINT uMsg_, IN WPARAM wParam_, IN LPARAM lParam_ );

	CX2RidingPetPtr CreateRidingPet( IN UidType uUserUid_, IN const CX2RidingPet::RidingPetInfo& RidingPetInfo_, IN D3DXVECTOR3 vInitPos_ );
	CX2RidingPetPtr CreateGameRidingPet( IN UidType uUserUid_, IN const CX2RidingPet::RidingPetInfo& RidingPetInfo_ );

	void AddRidingPet( IN CX2RidingPetPtr pRidingPet_ );
	RidingPetMap::iterator RemoveRidingPet( IN UidType uUserUid_ );
	void ChangeRidingPet( IN CX2RidingPetPtr pRidingPet_ );
	void RemoveAllRidingPet();
	void RemoveRidingPetList();

	CX2RidingPet* GetMyRidingPet();
	CX2RidingPet* GetRidingPet( IN UidType uUserUid_ );
	CX2RidingPet* GetRidingPetIndex( IN int iIndex_ );
	CX2RidingPet* GetRidingPetByUid( IN UidType iUid_ );
	CX2RidingPetPtr GetRidingPetPtr( IN UidType uUserUid_ );

	UINT GetRidingPetNum()
	{
		return m_mapRidingPet.size();
	}

	void InitPos();
	
	bool OpenScriptFile( IN const WCHAR* pFileName_ );
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	void LoadInitPetData();
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	bool AddRidingPetTemplet_LUA();
	CX2RidingPetManager::RidingPetTemplet* CX2RidingPetManager::GetRidingPetTemplet( IN USHORT usId_ ) { return GetRidingPetTemplet(static_cast<RIDING_PET_UNIT_ID>(usId_));}
	CX2RidingPetManager::RidingPetTemplet* CX2RidingPetManager::GetRidingPetTemplet( IN RIDING_PET_UNIT_ID eId_ );
	
	#pragma region REQ, ACK, NOT
	void Handler_EGS_CREATE_RIDING_PET_REQ( const UidType _uId );
	bool Handler_EGS_CREATE_RIDING_PET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Handler_EGS_SUMMON_RIDING_PET_REQ( UidType petUid );
	bool Handler_EGS_SUMMON_RIDING_PET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_SUMMON_RIDING_PET_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void Handler_EGS_UNSUMMON_RIDING_PET_REQ( bool bIsDirect_ = false, UidType uidDirectPetId_ = -1 );
	bool Handler_EGS_UNSUMMON_RIDING_PET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_UNSUMMON_RIDING_PET_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void Handler_EGS_RELEASE_RIDING_PET_REQ( UidType _uId );
	bool Handler_EGS_RELEASE_RIDING_PET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_RELEASE_RIDING_PET_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	#pragma endregion 

	CX2RidingPetSkillSlot* GetSkillSlot(){ return m_pSkillSlot; }

	UidType GetRidingPetId() const { return m_RidingPetUid; }
	void SetRidingPetList( const std::vector< sRidingPetInfo >& vecRidingPetList_ ) { m_vecRidingPetList = vecRidingPetList_ ; }
	void GetRidingPetList( OUT std::vector< sRidingPetInfo >& vecRidingPetList_ ) const { vecRidingPetList_ = m_vecRidingPetList; }
	bool GetHasRidingPet() const { return !m_vecRidingPetList.empty(); }

	bool GetRidingPetInfoByUID( IN UidType uiRidingPetUID, OUT sRidingPetInfo& RidingPetInfo_ );

	void GetSortedRidingPetList( OUT std::vector< sRidingPetInfo >& vecRidingPetList_ ) { SortRidingPetList(); GetRidingPetList(vecRidingPetList_); }
	UidType GetFirstRidingPetUID();
	void SortRidingPetList();
	void UpdateRidingPetGage();
#ifdef FIX_RIDING_STAMINA_CONSUME
	void UnsummonRidingPet();
	bool IsInvalideRiding() const;
#endif // FIX_RIDING_STAMINA_CONSUME

	void SetEnableStamina_LUA( float _EnableStamina );
	const float GetEnableStamina(){ return m_fEnableStamina; }

	bool GetRidingOn();	            //현재 탑승 가능한지 체크(서버패킷 ACK를 받은 후에 true가 됨)
	bool GetRidingOnState();		//현재 탑승 가능한 상태인지 체크(wait모션일때만 탑승하도록)
	void DeleteRidingPetOnList( UidType deleteUid_ ); //리스트에서 해당 탈 것 삭제
	bool CheckRidingStamina( UidType RidingPetUid_ ); //현재 탑승 가능한 스테미너인지 체크

	// 개발자 치트 함수
	void FullChargeRidingStamina( int iValue_ );

	bool CanPushSummonButton() { return m_ElapsedCanPushSummonButton.CheckElapsedTime(); }
	void RefreshSummonButtonElpasedTime() { m_ElapsedCanPushSummonButton.ResetSumOfElapsedTime(); }

	float GetRecoveryTimePerOne();

#ifdef REALTIME_SCRIPT_PATCH
	void RefreshRidingPetScript();
#endif // REALTIME_SCRIPT_PATCH

#ifdef RIDINGPET_STAMINA_ITEM
	bool SetRidingPetStaminaPercentUP(int iValue_ );
#endif RIDINGPET_STAMINA_ITEM

#pragma endregion function

#pragma region private
private:
	CX2RidingPetManager( void );
	virtual ~CX2RidingPetManager( void );
#pragma endregion function

#pragma region private
private:
	bool			m_bShow;				/// 탈 것을 보기

	static CX2RidingPetManager*	m_pRidingPetManager;
	RidingPetMap		m_mapRidingPet;			/// 탈 것 맵
	RidingPetTempletMap	m_mapRidingPetTemplet;	/// 탈 것 템플릿 맵
	RidingPetVec		m_vecRemoveRidingPet;	/// 제거할 탈 것 목록
	
	CX2RidingPetSkillSlot* m_pSkillSlot;

	UidType		m_RidingPetUid;

	// m_iLastUnSummonDate값은 서버에서는 시간으로 받아오지만,
	// 클라에서 사용 할 때는 Index값으로 변경해서 사용함.
	// 유효 범위 : 0~1 
	std::vector< sRidingPetInfo >		m_vecRidingPetList;

	bool		m_bIsDirectSummon; //소환해제하고 바로 소환하는데 필요.
	UidType		m_DirectSummonUid; //소환해제하고 바로 소환할 탈 것 uid
	CKTDXCheckElapsedTime m_ElapsedTimeDecrease;
	CKTDXCheckElapsedTime m_ElapsedTimeRecovery;
	CKTDXCheckElapsedTime m_ElapsedCanPushSummonButton;		/// 소환 버튼을 1초내에 다시 누르지 못하게 하기 위한 변수

	float		m_fEnableStamina;	//탑승 가능한 스테미너 최소치
#ifdef RIDINGPET_STAMINA_ITEM
	float		m_fUpdateNowStamina;
#endif RIDINGPET_STAMINA_ITEM
#pragma endregion variable

};
#pragma endregion 탈 것 관리 클래스

#pragma region RidingPetInitTemplate
#ifdef X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
struct RidingPetInitTemplate
{
	CX2RidingPet::InitData	m_init;
};
#endif //X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
#pragma endregion 구조체

#endif


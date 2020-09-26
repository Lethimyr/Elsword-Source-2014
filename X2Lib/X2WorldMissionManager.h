#pragma once

#ifdef SERV_INSERT_GLOBAL_SERVER

#define MAX_CRYSTAL_COUNT	5

class CX2UIWorldMission;

class CX2WorldMissionManager
{
public:
	~CX2WorldMissionManager();
	CX2WorldMissionManager();

	void Init();

	bool IsActiveDefenceDungeon() const { return m_bActiveDefenceDungeon; }
	void SetIsActiveDefenceDungeon( bool bActiveDefenceDungeon_ ) { m_bActiveDefenceDungeon = bActiveDefenceDungeon_; }
	//bool IsEXPBonus()		{ return m_bIsEXPBonus; }

//	int GetDungeonTargetCount() { return m_iDungeonTargetCount; }
	int GetNumOfWorldCrystalsProtected() const { return m_iNumOfWorldCrystalsProtected; }
	void SetNumOfWorldCrystalsProtected( int iNumOfWorldCrystalsProtected_ ) { m_iNumOfWorldCrystalsProtected = iNumOfWorldCrystalsProtected_; }

	USHORT GetRemainTimeWorldBuff() const { return m_usRemainTimeWorldBuff; }
	void SetRemainTimeWorldBuff( USHORT usRemainTimeWorldBuff_ ) { m_usRemainTimeWorldBuff = usRemainTimeWorldBuff_; }

	USHORT GetWorldBuffStepNum() const { return m_usWorldBuffStepNum; }
	void SetWorldBuffStepNum( USHORT usWorldBuffStepNum_ ) { m_usWorldBuffStepNum = usWorldBuffStepNum_; }

//	char GetNeedPartyNum() { return m_cNeedPartyNum; }

	void RenderMiniMapTexture( CKTDXDeviceRenderTargetTexture* 	pRenderTargetTexture );
	void RenderCrystal( int iPosX_, int iPosY_, const CX2GUNPC* pCrystal_, CKTDXTimer& tTimer_ );
	
	CX2UIWorldMission *GetUIWorldMission() const { return m_pUIWorldMission; }

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	HRESULT OnFrameMove( double fTime, float fElapsedTime );

	bool Handler_EGS_WORLD_MISSION_UPDATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_UPDATE_WORLD_BUFF_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	CKTDXDeviceTexture* GetTextureDefenceDungeonMiniMap() const { return m_pTextureDefenceDungeonMiniMap; }
	void SetTextureDefenceDungeonMiniMap( IN CKTDXDeviceTexture* pTextureDefenceDungeonMiniMap_ ) { m_pTextureDefenceDungeonMiniMap = pTextureDefenceDungeonMiniMap_; }

	const CKTDXDeviceTexture::TEXTURE_UV* GetUVDefenceDungeonMiniMap() const { return m_pUVDefenceDungeonMiniMap; }
	void SetUVDefenceDungeonMiniMap( const CKTDXDeviceTexture::TEXTURE_UV* pUVDefenceDungeonMiniMap_ ) { m_pUVDefenceDungeonMiniMap = pUVDefenceDungeonMiniMap_; }

	void SetDefenceDungeonMiniMap( const wstring& wstrTextureName, const wstring& wstrKeyName );

	int GetNumOfCrystals() const { return m_iNumOfCrystals; }
	void SetNumOfCrystals( int iNumOfCrystals_ ) { m_iNumOfCrystals = iNumOfCrystals_; }

	// 드래곤의 숨결
	bool GetDragonBreathEnchantStatIncreased( OUT const CX2Stat::EnchantStat*& pEnchantStatInreased_ ) const;
	const int GetDragonBreathAnimSpeedRateIncreased() const;
	const float GetDragonBreathAttackStatRateIncreased() const;

	const float GetEnchantStatFromDrgonBreath( int iEnchantType ) const;
	const bool IsNullWorldBuffDragonBreath() const { return (NULL == m_pWorldBuffDragonBreath ? true : false); }

	// 엘의 축복
	const int GetElBlessCriticalRateIncreased() const;
	const float GetElBlessDefenceStatRateIncreased() const;

	const bool IsNULLWorldBuffElBless() const { return (NULL == m_pWorldBuffElBless ? true : false); }

	int GetRemainTimeToFinish() const { return m_iRemainTimeToFinish; }
	void SetRemainTimeToFinish( const int iRemainTime_ ) { m_iRemainTimeToFinish = iRemainTime_; }

	int GetRemainTimeMissionStart() const { return m_iRemainTimeMissionStart; }
	void SetRemainTimeMissionStart( const int iRemainTimeMissionStart_ ) { m_iRemainTimeMissionStart = iRemainTimeMissionStart_; }

	void GetDefenceDungeonIcnoDesc( OUT wstringstream& wstrStreamDungeonIconDesc_ );
	//const wstring& GetDefenceDungeonIconDesc() const;
	//void LoadScript();

#ifdef NEW_DEFENCE_DUNGEON
	//현재 어둠의 문 난이도
	int GetNowDefenceDungeonLevel() const { return m_iNowDefenceDungeonLevel; }
	void SetNowDefenceDungeonLevel( const int iNowDefenceDungeonLevel_ ) { m_iNowDefenceDungeonLevel = iNowDefenceDungeonLevel_; }
	void ChangeDefenceDungeonLevel();	//지켜낸 수정의 갯수에 따라서 어둠의 문 난이도 값을 설정한다.
#endif NEW_DEFENCE_DUNGEON

#ifdef SERV_2012_PVP_SEASON2_EVENT
	/// 대전 개편 시즌 2 전야 이벤트 - 영광의 시간 활성 여부
	bool GetActiveTimeOfGlory() const { return m_bActiveTimeOfGlory; }
	void SetActiveTimeOfGlory( bool bActiveTimeOfGlory_ ) { m_bActiveTimeOfGlory = bActiveTimeOfGlory_ ; }

	void SetTimeOfGloryDLG();			/// 영광의 시간 활성 다이얼로그 표시 여부
	void ProcessTimeOfGloryDLG();		/// 영광의 시간 다이얼로그 알파값 갱신 함수
	bool Handler_EGS_PVP_EVENT_INFO_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );		/// 영광의 시간 패킷 핸들러
#endif SERV_2012_PVP_SEASON2_EVENT 

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
	bool Handler_EGS_EARTH_QUAKE_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );			/// 공존의 축제 지진 이벤트 패킷
#endif TOGETHER_FESTIVAL_2012_AUTUMN


protected:
	void CreateWorldBuffDragonBreath();
	void DeleteWorldBuffDragonBreath();

	void CreateWorldBuffElBless();
	void DeleteWorldBuffElBless();

private:
	// 월드버프 2단계, 드래곤의 숨결
	struct WORLD_BUFF_DRAGON_BREATH
	{
		CX2Stat::EnchantStat	m_EnchantStatIncreased;
		int						m_iAnimSpeedRateIncreased;
		float					m_fAttackStatRateIncreased;

		WORLD_BUFF_DRAGON_BREATH( const float fEnchantStatIncreased_, const int iAnimSpeedRateIncreased_, const float fAttackStatRateIncreased_ ) 
			: m_EnchantStatIncreased( fEnchantStatIncreased_ ), m_iAnimSpeedRateIncreased( iAnimSpeedRateIncreased_ ), m_fAttackStatRateIncreased( fAttackStatRateIncreased_ )
		{}
	};

	// 월드버프 3단계, 엘의 축복
	struct WORLD_BUFF_EL_BLESS
	{
		int						m_iCriticalRateIncreased;
		float					m_fDefenceStatRateIncreased;

		WORLD_BUFF_EL_BLESS( const int iCriticalRateIncreased_, const float fDefenceStatRateIncreased_ )
			: m_iCriticalRateIncreased( iCriticalRateIncreased_ ), m_fDefenceStatRateIncreased( fDefenceStatRateIncreased_ )
		{}
	};

	bool			m_bActiveDefenceDungeon;			///	현재 던전 개방되어 있는지 여부

	int				m_iNumOfWorldCrystalsProtected;		///	월드 내에서 지켜진 엘의 갯수

	int				m_iRemainTimeToFinish;				///	남은 시간
	USHORT			m_usRemainTimeWorldBuff;			/// 월드 버프 남은 시간
	USHORT			m_usWorldBuffStepNum;				/// 현재 진행 중인 월드 버프 단계
	int				m_iRemainTimeMissionStart;			/// 미션 시작시 까지 남은 시간 (분 단위, 오늘 열릴 예정이 더이상 없는 경우 -1)
		
	CKTDXTimer		m_timerMissionToFinish;				///	미션 진행시간
	CKTDXTimer		m_timerWorldBuff;					///	월드 버프 진행 시간
	CKTDXTimer		m_timerMissionToStart;				/// 미션이 시작될 때 까지의 남은 시간

	//bool			m_bIsEXPBonus;						///	보너스 경험치 여부

	CKTDXTimer		m_timerDamaged[MAX_CRYSTAL_COUNT];	

	CX2UIWorldMission	*m_pUIWorldMission;

	// 아래 두개를 CX2UIWorldMission으로 넣는 방법도 고려해 보자
	CKTDXDeviceTexture*	m_pTextureDefenceDungeonMiniMap;			/// 디펜스 던전 미니맵 텍스쳐 포인터
	const CKTDXDeviceTexture::TEXTURE_UV* m_pUVDefenceDungeonMiniMap;		/// 디펜스 던전 미니맵 텍스쳐 UV 포인터

	const int m_iStartKeyNumberForCrystal;				/// 엘 크리스탈이 가지고 있는 키코드 중 제일 첫번째 KeyNumber
	int m_iNumOfCrystals;								/// 난이도에 따라 달라지는 크리스탈의 갯수

	WORLD_BUFF_DRAGON_BREATH*	m_pWorldBuffDragonBreath;
	WORLD_BUFF_EL_BLESS*		m_pWorldBuffElBless;

#ifdef NEW_DEFENCE_DUNGEON
	int							m_iNowDefenceDungeonLevel;	/// 현재 어둠의 문 난이도
#endif NEW_DEFENCE_DUNGEON

#ifdef SERV_2012_PVP_SEASON2_EVENT
	bool						m_bActiveTimeOfGlory;		///	영광의 시간 활성 여부
	bool						m_bShowTimeOfGloryDLG;		///	영광의 시간 다이얼로그 활성 여부
#endif SERV_2012_PVP_SEASON2_EVENT

	// TEST
// 	int MAGIC_X_LEFT;
// 	int MAGIC_X_RIGHT;
// 	int MAGIC_Z_UP;
// 	int MAGIC_Z_DOWN;
// 
// 	int MAGIC_POS_X;
// 	int MAGIC_POS_Y;
// 
// 	int MAGIC_WIDTH;
// 	int MAGIC_HEIGHT;
// 
// 	int MAGIC_EL_WIDTH;
// 	int MAGIC_EL_HEIGHT;
};

#endif SERV_INSERT_GLOBAL_SERVER

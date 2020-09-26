#pragma once

#ifdef FIELD_BOSS_RAID
class CX2BossRaidManager
{
public:
	void OnFrameMove( double fTime, float fElapsedTime );
	void OnFrameRender();
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
public:
	static CX2BossRaidManager* GetInstance();
	static void ResetBossRaidManager();
	static void DestroyInstance();

	// 레이드 필드 활성화 여부
	bool GetIsOngoingRaid() const { return m_bIsOngoingRaid; }
	void SetIsOngoingRaid(bool val) { m_bIsOngoingRaid = val; }

	// 레이드 필드로 가는 포탈 활성화 여부
	bool IsActiveRaidPortal() const;
	
	// 레이드 필드 MapID 관련
	void InitRaidFieldMapID();
	SEnum::VILLAGE_MAP_ID GetBossRaidCreatorMapID() const { return m_eBossRaidCreatorMapID; }
	SEnum::VILLAGE_MAP_ID GetBossRaidMapID() const { return m_eCurrentRaidMapID; }

	// 난입 막기 설정 여부
	void SetSendIntrudeRestrictionNot(bool val) { m_bSendIntrudeRestrictionNot = val; }

	// 입장 예외처리.
	void JoinFieldProcess( float fFieldClearLeftTime_ = 0.f);

	// 게이트 위치를 비추는 카메라 인덱스
	int GetCameraIndex( IN SEnum::VILLAGE_MAP_ID eFieldMapID_ ) const ;
	void GetLoadingUIFileName( OUT wstring& wstrImgLFileName_, OUT wstring& wstrImgRFileName_) const;

	// 월드가 변경되지 않은 채로, 보스가 생성되려고 할 때의 예외처리.
	void CreateNpcExceptProcess( const CX2UnitManager::NPC_UNIT_ID eNpcID_ ) const;

	CX2UnitManager::NPC_UNIT_ID GetBossNPCIDByMapID( SEnum::VILLAGE_MAP_ID eFieldMapID_ ) const;

	// Load NPC REQ 정상 여부.. 한 필드에서 1회만 보낼 수 있음
	//bool IsValideNpcLoadReq() const;
	//bool GetSendNpcLoadReq() const { return m_bSendNpcLoadReq; }
	//void SetSendNpcLoadReq(bool val) { m_bSendNpcLoadReq = val; }

	// 남은 시간 관련
	void SetRaidFieldClearLimitTime(float val) { m_fRaidFieldClearLimitTime = val; }
	float GetRaidFieldClearLimitTime() const { return m_fRaidFieldClearLimitTime; }

	float GetStandardLeftTime() const { return m_fStandardLeftTime; }
	void SetStandardLeftTime(float val) { m_fStandardLeftTime = val; }

	const CTime& GetReceivePacketTime() const { return m_tReceivePacketTime; }
	void SetReceivePacketTime(CTime& val) { m_tReceivePacketTime = val; }

	/// 치트
	void OpenCheat();
	void CloseCheat();
	// 치트 패킷
	void Handler_EGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ(); // 보스 필드 포탈을 생성
	bool Handler_EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void Send_EGS_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT(); // 보스 필드 포탈을 닫기

	void Handler_EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ(); // 배틀 필드의 TotalDangerousValue를 알아내기
	bool Handler_EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Handler_EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ( int byDangerousValue_ ); // 배틀 필드의 TotalDangerousValue를 설정하기 위한 치트 패킷
	bool Handler_EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ_Cheat() { m_bSendIntrudeRestrictionNot = false; Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ(); };

	// ~End Public Function
private:

	bool Handler_EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	bool Handler_EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_BOSS_FIELD_CLOSE_PROCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_BOSS_FIELD_RETURN_PROCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	// 현재 필드 진입 불가 처리 요청
	void Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ();
	bool Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	// 레이드 필드 입구 이펙트 갱신
	void UpdateLinemapPortalEffect();

	void OnFrameMove_BattleField( double fTime, float fElapsedTime );
	void OnFrameRender_BattleField();

	// 필드 오픈 엠블럼 텍스쳐 이름 얻기
	void GetFieldOpenTextureName( IN SEnum::VILLAGE_MAP_ID eBossRaidCreatorMapID_, OUT wstring& wstrOpenTextureName_ ) const;

	// 난입 불가 요청을 위해 체력 검사
	bool IsBossHPLessThen10() const;

#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
	void RenderRanking() const;

	void UpdateContributionRanking();
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD

	// ~End Private Function
private:
	CX2BossRaidManager();
	~CX2BossRaidManager();
	
	static CX2BossRaidManager* m_pManager;
	//////////////////////////////////////////////////////


	bool						m_bIsOngoingRaid;		// 보스 레이드 진행 여부
	SEnum::VILLAGE_MAP_ID		m_eCurrentRaidMapID;	// 현재 열려있는 레이드 필드 ID

	SEnum::VILLAGE_MAP_ID		m_eBossRaidCreatorMapID;// 위험도 달성을 통해 레이드 필드를 생성한 필드의 ID
	bool						m_bShowFieldClearTime;
	CKTDGFontManager::CUKFont*	m_pFontForTime;		// 타임 아웃등 시간 표시용


	//float						m_fBanLeftTimeFromRaidField;	// 레이드 필드에서 쫓아 낼 남은 시간


	// 보스 레이드 몬스터 체력 10% 이하일 때 난입 막기
	bool						m_bSendIntrudeRestrictionNot;	// 난입 불가 패킷 요청 여부.

#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
	vector<wstring>				m_vecContributionRanking;		// 기여도 랭킹

	float						m_fElaspedTimeCalRank;			// 기여도 계산 시간
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD

	float						m_fRaidFieldClearLimitTime;		// 현재 남은 시간
	CTime						m_tReceivePacketTime;			// 첫 입장 시 서버로부터 남은 시간을 받은 시간
	float						m_fStandardLeftTime;			// 첫 입장 시 서버로부터 받은 남은 시간

	//bool						m_bSendNpcLoadReq;				// NPC Load REQ 전송 여부(1회만 보내기 위해 기록)
#ifdef ADD_RAID_FIELD_LOG
	float						m_fTestLimitTime;
#endif // ADD_RAID_FIELD_LOG
};

#endif // FIELD_BOSS_RAID


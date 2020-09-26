#ifdef SERV_PET_SYSTEM
#pragma once

class CX2NPCUnitViewerUI;

//class CX2StateMenu;
class CX2UIPetInfo
{
public:
	enum PET_INFO_TAB
	{
		PIT_LIST			= 0,
		PIT_INFO,
		PIT_DETAIL,
	};

#ifdef RIDING_SYSTEM
	enum RIDING_PET_INFO_TAB
	{
		RPIT_LIST			= PIT_DETAIL + 1,
		RPIT_INFO,
	};

	enum TAB_SELECT
	{
		PET_TAB_SELECT			= 0,
		RIDING_PET_TAB_SELECT,
	};

	enum KIND_OF_RIDE_BUTTON
	{
		BUTTON_RIDE_ON			= 0,
		BUTTON_RIDE_OFF,
		BUTTON_RIDE_NONE,
	};
#endif //RIDING_SYSTEM

	enum PET_LIST_UI_CUSTOM_MESSAGE
	{
		PLUCM_CLOSE			= 44001,
		PLUCM_VIEW_LIST,
		PLUCM_VIEW_INFO,
		PLUCM_VIEW_DETAIL,
		PLUCM_LIST_SELECT,
		PLUCM_INFO_SUMMON,
		PLUCM_INFO_EVOLUTION,
		PLUCM_INFO_SUMMON_CANCEL,
		PLUCM_INFO_EVOLUTION_OK,
		PLUCM_LIST_PREV,
		PLUCM_LIST_NEXT,
//#ifdef PET_DROP_ITEM_PICKUP
		PLUCM_ITEM_PICKUP_SKILL,
		PLUCM_ITEM_PICKUP_SKILL_OK,
		PLUCM_CASH_SHOP_OPEN_OK,
//#endif PET_DROP_ITEM_PICKUP
//#ifdef RIDING_SYSTEM
		PLUCM_PET_TAP_SELECT,
		PLUCM_RIDING_PET_TAP_SELECT,
		PLUCM_RIDING_PET_INFO,
		PLUCM_RIDING_PET_LIST,
		PLUCM_RIDING_PET_RIDE,
		PLUCM_RIDING_LIST_BUTTON_CLICK,
		PLUCM_RIDING_LIST_BUTTON_SELECT,
		PLUCM_RIDING_PET_RELEASE,
		PLUCM_RIDING_PET_RELEASE_OK,
		PLUCM_RIDING_PET_RELEASE_CANCLE,
		PLUCM_RIDING_LIST_PREV,
		PLUCM_RIDING_LIST_NEXT,
		PLUCM_RIDING_PET_RIDE_OFF,

//#endif //RIDING_SYSTEM
	};
	
	
public:

	CX2UIPetInfo( CKTDXStage* pNowStage );
	virtual ~CX2UIPetInfo(void);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	void SetShow(bool val, UidType eTab = NULL);
	void SetLayer( X2_DIALOG_LAYER layer );
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetPosition( D3DXVECTOR2 vec );

	void ChangeInfo();
	void SetPetInfo( const KPetInfo& kPetInfo );	
#ifdef PET_DROP_ITEM_PICKUP
	void SetPetAutoLooting( UidType uiPetUID, bool bAutoLooting);
#endif //PET_DROP_ITEM_PICKUP

	void NextPage();
	void PrevPage();

	void SetViewSummonedPetUid( );

	void SetViewTab( int eTab ) { m_ePetTab = (PET_INFO_TAB)eTab; }
	CX2UIPetInfo::PET_INFO_TAB GetViewTab() { return m_ePetTab; }

	void UpdateView();
	void ViewListTab(int iPage = 0);
	void ViewInfoTab( KPetInfo *kPetInfo );
	void ViewDetailTab( KPetInfo *kPetInfo );
	UINT GetPetListSize() { return m_vecPetList.size();}
	//펫리스트 UI를 열지 않고 펫 소지 여부를 파악 하기 위해 함수 추가
	void GetPetListReq() {Handler_EGS_GET_PET_LIST_REQ(1);}

#ifdef RIDING_SYSTEM
	void Select_Tab( UidType eTab );
	void Handler_EGS_GET_RIDING_PET_LIST_REQ( UINT iViewPage );
	bool Handler_EGS_GET_RIDING_PET_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void SelectKindRideButton( KIND_OF_RIDE_BUTTON  _select, bool _enable = true );

	const bool GetShowRPList();
	const bool GetShowRPInfo();

	void UpdateRidingButtonGuideDesc( bool IsSetKey_ ); //bool값에 따라 이미 탑승키가 정해져 있는가 아닌가에 따라 툴팁이 바뀜.

	const UidType GetViewRidingPetID(){ return m_ViewRidingPetUid; }
#endif //RIDING_SYSTEM

#ifdef SIMPLE_BUG_FIX
	CKTDGParticleSystem::CParticleEventSequenceHandle GethParticle() {return m_hParticle;} // Pet 성향 파티클 관련함수
	void SethParticle(CKTDGParticleSystem::CParticleEventSequenceHandle mhParticle) { m_hParticle = mhParticle;} // Pet 성향 파티클 관련함수
#endif SIMPLE_BUG_FIX
	
private:
	bool IsInUnitClickReact( D3DXVECTOR2 mousePos );

	void UpdateDLGNPCViewerUI();
#ifdef RIDING_SYSTEM
	void UnitHandling( CX2NPCUnitViewerUI* pViewerUI, CKTDGUIDialogType pDLGType, double fTime, float fElapsedTime );
#else
	void UnitHandling( double fTime, float fElapsedTime );
#endif //RIDING_SYSTEM

#ifdef SERV_PERIOD_PET
	void SetPetRow( int rowIndex, int petId, char petLv = 0, wstring petName = L"", float fSatiety = 0.f, float fIntimacy = 0.f, bool bSummoned = false, wstring wstrDestroyDate = L"" );
#else SERV_PERIOD_PET
	void SetPetRow(int rowIndex, int petId, char petLv = 0, wstring petName = L"", float fSatiety = 0.f, float fIntimacy = 0.f, bool bSummoned = false);
#endif SERV_PERIOD_PET

	void UpdatePetList();
	void UpdatePetInfo( KPetInfo *kPetInfo );
	void UpdatePetDetail( KPetInfo *kPetInfo );

	void Handler_EGS_GET_PET_LIST_REQ( UINT iViewPage );
	bool Handler_EGS_GET_PET_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef RIDING_SYSTEM
	void ViewRidingPetList( int iPage = 0 );
	void Enable_ButtonBG( int rowIndex, bool _enable );
	bool Select_Button( LPARAM lParam );

	static const float				RANKUP_UI_PLAY_TIME; //5.f 실시간피드백UI 노출 시간	//현재 탑승 가능한 state인지 체크(wait일때만 탑승 하도록 제한)
	void SetRidingPetInfo();
	void SetRidingPetRow( const UINT rowIndex, const int ridingpetId, const float stamina = 0 );
	void UpdateStamina_Info( const float _Stamina, const float _MaxStamina ); //정보창에서 스테미나 업데이트
	void UpdateStamina_List(); //리스트에서 스테미나 업데이트
	void UpdatePeriod_List(); //리스트에서 기간제 펫일 경우 기간 업데이트
	void UpdateDLGRidingPetViewerUI();

	void SortRidingPetListByServerData( OUT std::vector<KRidingPetInfo>& vecRidingPetList_ );	//서버에서 받은 펫 리스트 정렬 함수
#endif //RIDING_SYSTEM

private:	
	bool					m_bShow;
	D3DXVECTOR2				m_MovedPosition;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;
	
	CKTDGUIDialogType			m_pDLGUIPetList;
	CKTDGUIDialogType			m_pDLGUIPetInfo;
	CKTDGUIDialogType			m_pDLGUIPetDetail;

	CX2NPCUnitViewerUI*		m_pNPCViewerUI;

	int						m_iMaxPage;
	int						m_iNowPage;

	bool					m_bProcessPetList;

	std::vector< KPetInfo > m_vecPetList;
	
	PET_INFO_TAB			m_ePetTab;
	UidType					m_ViewPetUid;
#ifdef PET_DROP_ITEM_PICKUP
	UidType					m_UseItemUID;
#endif //PET_DROP_ITEM_PICKUP

	float					m_fSummonCoolTime;


	// 유닛뷰어관련
	float					m_fRemainRotY;
	float					m_fMouseSensitivity;
	bool					m_bRotateReturn;
	D3DXVECTOR2				m_UnitClickPos;
	D3DXVECTOR2				m_UnitClickSize;
	bool					m_bUnitClicked;

	bool					m_bDetailPoint;
	D3DXVECTOR2				m_vDetailPoint;
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hParticle;	

#ifdef RIDING_SYSTEM
	CKTDGUIDialogType		m_pDLGUIRidingPetList;
	CKTDGUIDialogType		m_pDLGUIRidingPetInfo;
	CKTDGUIDialogType		m_pDLGUIRidingPetReleasePopup;

	CX2NPCUnitViewerUI*		m_pRidingPetViewerUI;

	int						m_iMaxPageRiding;
	int						m_iNowPageRiding;
	int						m_iSelectButtonIndex;
	UidType					m_ViewRidingPetUid;
	UidType					m_RidingPetUid;

	D3DXVECTOR3				m_vecViewerRidingPetSize;
	D3DXVECTOR3				m_vecViewerPetSize;

	TAB_SELECT				m_eTabSelect;
	RIDING_PET_INFO_TAB		m_eRidingPetTab;

	float					m_fListStaminaUpdateCooltime;
	float					m_fMaxStamina;
	float					m_fMoveViewerPetPositionX; //뷰어에서 펫의 위치 이동( 지금은 에이션트 뽀루만 )

	KIND_OF_RIDE_BUTTON		m_eKindRideButton;
#endif //RIDING_SYSTEM

};

#endif
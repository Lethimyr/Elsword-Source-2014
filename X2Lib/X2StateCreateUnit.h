#pragma once

class CX2StateCreateUnit : public CX2State
{
	public:
		friend class CX2StateAutoChanger;

		enum STATE_CREATE_UNIT_UI_CUSTOM_MSG
		{
			SCUUCM_ELSWORD_BUTTON_OVER = 0,
			SCUUCM_ELSWORD_BUTTON_UP,

			SCUUCM_LIRE_BUTTON_OVER,
			SCUUCM_LIRE_BUTTON_UP,

			SCUUCM_ARME_BUTTON_OVER,
			SCUUCM_ARME_BUTTON_UP,

			SCUUCM_RAVEN_BUTTON_OVER,
			SCUUCM_RAVEN_BUTTON_UP,

			SCUUCM_CREATE_UNIT,
			SCUUCM_BACK_TO_UNIT_SELECT,

			SCUUCM_UNIT_ANIMATION_DOWN,
			SCUUCM_UNIT_ANIMATION_UP,
			SCUUCM_UNIT_ANIMATION_OVER,

			SCUUCM_ENTER_TUTORIAL_OK,
			SCUUCM_ENTER_TUTORIAL_CANCEL,

			SCUUCM_EVE_BUTTON_OVER,
			SCUUCM_EVE_BUTTON_UP,

			SCUUCM_CHUNG_BUTTON_OVER,
			SCUUCM_CHUNG_BUTTON_UP,

			SCUUCM_ARA_BUTTON_OVER, // 사용 안 함
			SCUUCM_ARA_BUTTON_UP,

			SCUUCM_SHUT_DOWN_OK,

			SCUUCM_ELESIS_BUTTON_UP,

// #ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
			// 캐릭터 전직 미리보기 버튼, 첫번째 라인 1차 전직 선택
			SCUUCM_FIRST_LINE_FIRST_CLASS_CHANGE_BUTTON_UP,
			// 캐릭터 전직 미리보기 버튼, 두번째 라인 1차 전직 선택
			SCUUCM_SECOND_LINE_FIRST_CLASS_CHANGE_BUTTON_UP,
			// 캐릭터 전직 미리보기 버튼, 세번째 라인 1차 전직 선택
			SCUUCM_THIRD_LINE_FIRST_CLASS_CHANGE_BUTTON_UP,
			// 캐릭터 전직 미리보기 버튼, 첫번째 라인 2차 전직 선택
			SCUUCM_FIRST_LINE_SECOND_CLASS_CHANGE_BUTTON_UP,
			// 캐릭터 전직 미리보기 버튼, 두번째 라인 2차 전직 선택
			SCUUCM_SECOND_LINE_SECOND_CLASS_CHANGE_BUTTON_UP,
			// 캐릭터 전직 미리보기 버튼, 세번째 라인 2차 전직 선택
			SCUUCM_THIRD_LINE_SECOND_CLASS_CHANGE_BUTTON_UP,
			
			// 캐릭터 서버 선택창 단계로 넘어감
			SCUUCM_CREATE_UNIT_SERVER_SELECT_STEP_BUTTON_UP,
			
			// 가이아, 솔레스 서버 선택
			SCUUCM_SELECT_GAIA_SERVER,
			SCUUCM_SELECT_SOLES_SERVER,
			
			// 
			SCUUCM_PASS_NICKNAME_CHECK_OK,
			SCUUCM_PASS_NICKNAME_CHECK_CANCEL,

			// 닉네임 체크
			SCUUCM_NICKNAME_CHECK,

			SCUUCM_ADD_BUTTON_UP,
// #endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
		};
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		enum UNIT_STATE_INFOMATION_CHART_LEVEL
		{
			USICL_NONE = 0,
			USICL_LOW,
			USICL_MIDDLE,
			USICL_HIGH,

		};
		struct UNIT_STAT_INFOMATION_CHART
		{
		
			UNIT_STATE_INFOMATION_CHART_LEVEL m_eSpeed;
			UNIT_STATE_INFOMATION_CHART_LEVEL m_eAttackDistance;
			UNIT_STATE_INFOMATION_CHART_LEVEL m_eDifficultLevel;

			UNIT_STAT_INFOMATION_CHART () : m_eSpeed ( USICL_NONE ), 
				m_eAttackDistance ( USICL_NONE ), m_eDifficultLevel ( USICL_NONE ) {}
			
			UNIT_STAT_INFOMATION_CHART ( UNIT_STATE_INFOMATION_CHART_LEVEL eSpeed, UNIT_STATE_INFOMATION_CHART_LEVEL eAttackDistance, 
				UNIT_STATE_INFOMATION_CHART_LEVEL eDifficultLevel ) :
						m_eSpeed ( eSpeed ), m_eAttackDistance ( eAttackDistance ), 
						m_eDifficultLevel ( eDifficultLevel ) {}
		};
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

	public:
		CX2StateCreateUnit(void);
		virtual ~CX2StateCreateUnit(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	protected:
		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_CREATE_UNIT_REQ();
		bool Handler_EGS_CREATE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );



		void UnSelect();
		void SelectElSword();
		void SelectLire();
		void SelectArme();
		void SelectRaven();
		void SelectEve();

//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		void SelectChung();
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		void SelectAra();
#endif
#ifdef NEW_CHARACTER_EL
		void SelectElesis();
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
		void SelectAdd();
#endif //SERV_9TH_NEW_CHARACTER


#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		// 상위 클래스 변경 메시지
		void SelectUpperClass ( CX2Unit::UNIT_TYPE eUnitType_, CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eClassLine_  );		
		// UnitUpperClassType 을 받아와 Unit Class Type 을 반환함
		CX2Unit * GetViewerUnitByUnitType ( CX2Unit::UNIT_TYPE eUnitType );		
		// 캐릭터 전직 정보창 DLG 를 보여주거나 숨김
		void ShowCreateUnitClassChangeInfoDlg ( bool bIsShow_ );	
		// 유닛 템플릿의 전직 프로모션 아이템의 정보를 받아와서 장착 시킴
		void EquipPromotionItemByUnitTemplet ( const CX2Unit::UnitTemplet * pUnitTemplet_ );
		// 캐릭터 전직 정보창의 클래스 단위를 변경한다.
		void ChangeCreateUnitTypeChangeInfoDlg ( CX2Unit::UNIT_TYPE eUnitType_ );
		// 오른쪽 깃발의 색을 변경한다.
		void SetRightSideFlagColor ( D3DXCOLOR colorFlag );
		// 캐릭터 변경 시, 밑 스탠드를 교체한다. 
		void ChangeCreateUnitStandMeshInstance ( CX2Unit::UNIT_TYPE eUnitType_ );
		// 캐릭터 전직 정보창의 세부 클래스 단위를 변경한다.
		void ChangeCreateUnitClassChangeInfoDlg ( CX2Unit::UNIT_TYPE eUnitType_, CX2Unit::UNIT_CLASS_TYPE eUpperClassType_, CX2Unit::UNIT_CLASS_LINE eClassLine_ );

		wstring GetUnitClassCreateInfoTextureNameByUnitType ( CX2Unit::UNIT_TYPE eUnitType_ );
		wstring GetUnitClassCreateInfoKeyNameByPictureIndex ( int iIndex_, CX2Unit::UNIT_CLASS_TYPE eUpperClassType_, CX2Unit::UNIT_CLASS_LINE eClassLine_ );

		wstring GetUnitClassChangeInfoKeyNameByPictureIndex ( int iIndex_, CX2Unit::UNIT_CLASS eUnitClass_ );
		UNIT_STAT_INFOMATION_CHART GetUnitStatInfomationChart ( CX2Unit::UNIT_CLASS eUnitClass_ );


		bool Handler_EGS_ENTRY_POINT_CHECK_NICK_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_REQ();
		bool Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh


		bool CreateErrorCheck();

		void InitDummyUnits();
		bool ReadyUnitResources( CX2Unit* pUnit );

		void ReadyPromotionUnitResources();
		void EquipPromotionItem( CX2Unit::UNIT_CLASS eUnitClass );

		
		void StaticControlAnimate();

		void OpenNickNameBox();
		void SetAlarmAboutName( const WCHAR* pContent );
		
		virtual bool LastMsgByESC();
		virtual void LastMsgPopOk();

		//{{ kimhc // 2010.11.30 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		UCHAR GetPageNowUnitButton() const { return m_ucPageNowUnitButton; }
		void SetPageNowUnitButton(UCHAR ucPageNowUnitButton_) { m_ucPageNowUnitButton = ucPageNowUnitButton_; }

		UCHAR GetPageMaxUnitButton() const { return m_ucPageMaxUnitButton; }
		void SetPageMaxUnitButton(UCHAR ucPageMaxUnitButton_) { m_ucPageMaxUnitButton = ucPageMaxUnitButton_; }
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.30 //  2010-12-23 New Character CHUNG

		void InitializeUnitButton();
		void SetUnitButton( int iPageNow_ );

	protected:
		CKTDGUIDialogType			m_pDLGCreateUnitBack;
		CKTDGUIDialogType			m_pDLGCreateUnitFront;

		CX2Unit::UNIT_CLASS		m_SelectUnitClass;
		wstring					m_NickName;
		UidType					m_UnitUID;

		CX2Unit*				m_pDummyElsword;
		CX2Unit*				m_pDummyArme;
		CX2Unit*				m_pDummyLire;
		CX2Unit*				m_pDummyRaven;
		CX2Unit*				m_pDummyEve;

//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		CX2Unit*				m_pDummyChung;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		CX2Unit*				m_pDummyAra;		/// "아라" 더미
#endif

#ifdef NEW_CHARACTER_EL
		CX2Unit*				m_pDummyEL;
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
		CX2Unit*				m_pDummyAdd;
#endif //SERV_9TH_NEW_CHARACTER


		CX2UnitViewerUI*		m_pUnitViewerUI;

		bool					m_bSelectUnit;		//2d캐릭터 클릭한거.. 

		float					m_fMouseSensitivity;

		float					m_fRemainRotY;
		bool					m_bRotateReturn;
		float					m_fRotSpeed;

		CKTDGUIDialogType			m_pDLGMSGOkBoxEnterTutorial;

		//{{ kimhc // 2010.11.16 // 신캐릭터 청
#ifdef	NEW_CHARACTER_CHUNG

		UCHAR					m_ucPageNowUnitButton;		/// 캐릭터 선택 버튼의 현재 페이지
		UCHAR					m_ucPageMaxUnitButton;		/// 캐릭터 선택 버튼의 최대 페이지
		
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.16 // 신캐릭터 청
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		// 선택한 캐릭터의 유닛 타입, 변경 시 전직 관련 창이 등장
		CX2Unit::UNIT_TYPE		m_SelectUnitType;
		
		// 캐릭터 전직 정보창
		CKTDGUIDialogType		m_pDLGCreateUnitChangeClassInfo;	

		// 캐릭터 서버 선택창 DLG
		CKTDGUIDialogType		m_pDLGCreateUnitServerSelect;	

		// 캐릭터 생성 창 최종 확인 DLG
		CKTDGUIDialogType		m_pDLGCreateUnitCheckInfo;	

		// 캐릭터 밑 스탠드
		std::vector<CKTDGXMeshPlayer::CXMeshInstanceHandle>		m_vecStandMeshInst;

		// 캐릭터 밑 스탠드 이펙트
		CX2EffectSet::Handle						m_hStandEffect;

		// 캐릭터 생성 시, 잠시 갖고 있는 목적으로 제작된 변수
		KUnitInfo				m_kUnitInfo;

		// 선택한 서버 값, 캐릭터 생성 시 패킷에 추가됨
		SEnum::SERVER_GROUP_ID	m_eSelectServerGroupID;

		int						m_iGaiaServerCreateAbleUnitMany;
		int						m_iSolesServerCreateAbleUnitMany;
		float					m_fFadeTitleControlValue;

		// 같은 동영상 파일로 중복 재생하는 것을 방지하기 위한 동영상 파일 이름
		wstring					m_wstrIntroMovieName;

		SHORT		m_sSumDelta;	/// 마우스 휠용 값
		SHORT		m_sZoomState;	/// 줌 상태 ( 1 ~ 3 / 3단계 )
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

#ifdef ADD_PLAY_SOUND //김창한
		// 유닛 생성에서 캐릭터 클릭시에 나오는 사운드.
		CKTDXDeviceSound*		m_pCreateUnitSelectSound;
#endif //ADD_PLAY_SOUND

private:
#ifdef ADD_PLAY_SOUND //김창한
	void PlaySoundCreateUnit( CX2Unit::UNIT_TYPE eUnitType );
#endif //ADD_PLAY_SOUND
};

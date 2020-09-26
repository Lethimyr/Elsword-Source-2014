#pragma once
/*
class CX2StateUnitSelect : public CX2State
{
	public:
		friend class CX2StateAutoChanger;

		enum STATE_UNIT_SELECT_UI_CUSTOM_MSG
		{
			
		};

		struct UnitButton
		{
			int				unitIndex;
			CKTDGUIButton*	pButton;

			UnitButton()
			{
				unitIndex	= 0;
				pButton		= NULL;
			}
		};

	public:
		CX2StateUnitSelect(void);
		virtual ~CX2StateUnitSelect(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();
		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		

	protected:
		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		
		void UnitHandling( double fTime, float fElapsedTime );

		bool AnimateCharacterName( CKTDGUIButton* pButton );
		void SetShowCharacterName( bool bShow );

		void ResetCrashReport();

		void SetShowCharacterNameForSelectedUnit();

		

	protected:

		CKTDGUIDialog*			m_pDLGUnitSelectBack;
		CKTDGUIDialog*			m_pDLGUnitSelectFront;
		
		CKTDGUIDialog*			m_pDLGDeleteUnitCheck;
		CKTDGUIDialog*			m_pDLGLoadingState;

		CKTDGUIDialog*			m_pDLGRenameMsgBox;
		CKTDGUIDialog*			m_pDLGTutorialMsgBox;
		CKTDGUIDialog*			m_pDLGGoToFirstDungeon;

		CKTDGUIDialog*			m_pDLGRecommend;
		CKTDGUIDialog*			m_pDLGRecommendReward;
		CKTDGUIDialog*			m_pDLGRecommendSuccess;

		CKTDGUIDialog*			m_pDLGTutorReward;
		CKTDGUIDialog*			m_pDLGGuestUserReward;




		vector<UnitButton*>		m_UnitButtonList;
		vector<CX2UnitViewerUI*> m_UnitViewerUIList;

		CX2Unit*				m_pSelectUnit;
		CKTDGUIButton*			m_pSelectButton;

		bool					m_bUnitClicked;
		float					m_fMouseSensitivity;

		float					m_fRemainRotY;
		bool					m_bRotateReturn;
		float					m_fRotSpeed;

		CKTDGUIButton*			m_pOveredButton;
		CKTDGUIStatic*			m_pOveredName;


		bool					m_bOneTimeInit;
		bool					m_bStateChangingToUnitCreate;

		bool					m_bPlayDownAnim;
		float					m_fDownAnimTime;

		int						m_MaxUnitNum;

		bool					m_bReserveEnterTutorial;
		bool					m_bCheckPostItem;

		bool					m_bCheckRecommend;
		bool					m_bCheckRecommendSuccess;

		int						m_NowPage;
		int						m_MaxPage;
};


*/
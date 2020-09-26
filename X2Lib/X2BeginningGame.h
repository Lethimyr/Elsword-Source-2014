#pragma once

class CX2BeginningGame : public CKTDXDeviceHolder
{
	public:

		enum BEGINNING_GAME_UI_CUSTOM_MSG
		{
			BGUCM_CREATE_UNIT			= 48000,
			BGUCM_BACK_TO_UNIT_SELECT,
		};


		enum CREATE_UNIT_INDEX
		{
			CUI_INVALID					= -1,
			CUI_ELSWORD_SWORDMAN,		// 0
			CUI_ARME_VIOLET_MAGE,		// 1
			CUI_LIRE_ELVEN_RANGER,		// 2
			CUI_RAVEN_FIGHTER,			// 3
			CUI_EVE_NASOD,				// 4
		};

	public:
		CX2BeginningGame();
		virtual ~CX2BeginningGame(void);		

		virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT				OnFrameRender();
		virtual bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual HRESULT				OnResetDevice();
		virtual HRESULT				OnLostDevice();

		bool						LoadWorld(CX2World::WORLD_ID worldID);

		CX2Camera*					GetX2Camera(){ return m_pCamera; }

		//void						SetEndableKey(bool val) { m_bEnableKey = val; }
		void 						SetEnableKeyProcess(bool val) { m_bEnableKeyProcess = val; }
		void 						SetEnableCameraProcess(bool val) { m_bEnableCameraProcess = val; }

		CX2World*					GetWorld(){ return m_pWorld; }
		CKTDGFontManager::CUKFont*	GetFontForUnitName() { return m_pFontForUnitName; }		

		CKTDGParticleSystem*		GetUiParticle()				{ return m_pUiParticle; }
		CKTDGXMeshPlayer*			GetUiXMeshPlayer()			{ return m_pUiXMeshPlayer; }
		//CKTDGParticleSystem*		GetMajorParticle()			{ return m_pMajorParticle; }
		//CKTDGParticleSystem*		GetMinorParticle()			{ return m_pMinorParticle; }
		//CKTDGXMeshPlayer*			GetMajorXMeshPlayer()		{ return m_pMajorXMeshPlayer; }
		//CKTDGXMeshPlayer*			GetMinorXMeshPlayer()		{ return m_pMinorXMeshPlayer; }

		void						SetFreeCamera( bool bFreeCamera );
		
		bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void DeleteWorld() { SAFE_DELETE(m_pWorld); }
		void SetStage(CKTDXStage *pStage);		

		bool GetEnableKey();

		void RenderLoadingFace(int iAnimStep);

		void UnSelectUnitPlayWorldCamera();

		void PlayWorldCamera( int iWorldCameraID );
private:
		void						InitKey();
		void						KeyProcess();
public:

#ifdef KEYFRAME_CAMERA
	CFirstPersonCamera* GetFPSCamera(){	return &m_FPSCamera; }
#endif KEYFRAME_CAMERA

		//{{ oasis907 : 김상윤 [2010.11.19] // 캐릭터 생성
		bool GetEnableCreateUnit(){ return m_bEnableCreateUnit; }
		void SetEnableCreateUnit(bool bEnable);

		void CreateElSword();
		void CreateArme();
		void CreateLire();
		void CreateRaven();
		void CreateEve();
#ifdef NEW_CHARACTER_CHUNG
		void CreateChung();
#endif NEW_CHARACTER_CHUNG
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


		bool CreateErrorCheck();

		void InitDummyUnits();
		bool ReadyUnitResources( CX2Unit* pUnit );

		void ReadyPromotionUnitResources();
		void EquipPromotionItem( CX2Unit::UNIT_CLASS eUnitClass );

#ifdef ELSWORD_NEW_BEGINNING
		void OpenNickNameBox(float fDelayTime = -1.f);
#endif ELSWORD_NEW_BEGINNING
		void SetAlarmAboutName( const WCHAR* pContent );
		bool GetShowMsgBox();

		bool Handler_EGS_CREATE_UNIT_REQ();
		bool Handler_EGS_CREATE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


		bool					m_bEnableCreateUnit;

		CKTDGUIDialogType		m_pDLGCreateUnitNickName;

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


		vector<CX2UnitViewerUI*> m_CreateUnitViewerUIList;

		CX2UnitViewerUI*		m_pNowSelectedUnitViewerUI;


		CX2Unit::UNIT_CLASS		m_SelectUnitClass;
		
		wstring					m_NickName;
		UidType					m_UnitUID;

		float					m_fFreezTimeForSelectUnit;


		CKTDGUIDialogType		m_pDLGMsgBox;
		CKTDGUIDialogType		m_pDLGMSGOkBoxEnterTutorial;

		bool					m_bSelectUnit;		//2d캐릭터 클릭한거.. 

		//}}

private:

		CX2World*					m_pWorld;
		CX2Camera*					m_pCamera;

		CX2GUUser::InputData		m_InputData;

		CKTDGParticleSystem*		m_pUiParticle;
		CKTDGXMeshPlayer*			m_pUiXMeshPlayer;
		//CKTDGParticleSystem*		m_pMajorParticle;
		//CKTDGParticleSystem*		m_pMinorParticle;
		//CKTDGXMeshPlayer*			m_pMajorXMeshPlayer;
		//CKTDGXMeshPlayer*			m_pMinorXMeshPlayer;

		CKTDGFontManager::CUKFont*	m_pFontForUnitName;
		CKTDGFontManager::CKTDGFont*m_pFont;

		bool						m_bEnableKeyProcess;
		bool						m_bEnableCameraProcess;


		bool						m_bFreeCamera;
		CFirstPersonCamera			m_FPSCamera;

		float						m_fRemainTimeOpenNickNameBox;

		float						m_fRemainTimeUnSelectComplete;

		float						m_fCameraDistance;
		float						m_fLookAtHeight;
		float						m_fEyeHeight;

		CKTDXDeviceSound*			m_pSubSound;

		CKTDXDeviceTexture*		m_pLoadingFaceTex;		

};


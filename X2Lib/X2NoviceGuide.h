#pragma once
#ifndef REFORM_NOVICE_GUIDE
#define MAX_GUIDE 6

class CX2NoviceGuide
{
public:
	enum GUIDE_EVENT
	{
		GE_ACCEPT = 0,
		GE_COMPLETE,
		GE_NEXT,
		GE_PREV,
	};

	enum GUIDE_STATE
	{
		GS_START = 0,
		GS_PLAY,
		GS_END
	};

	enum NOVICE_GUIDE_STEP
	{		
		NGS_NONE = 0,
		NGS_STEP1,	// 루벤마을 입장 이벤트(아리엘의 대화요청)
		NGS_STEP2,  // 루벤마을 아리엘 찾아가서 말걸기
		NGS_STEP3,	// 하거스 찾아가거 말걸기
		NGS_STEP4,	// 던전가기
		NGS_STEP5,  // 던전게이트에서 아리엘과 대화		
		NGS_STEP6_1,// 맵 선택
		NGS_STEP6_2,// 엘의나무 서쪽 맵 선택
		NGS_STEP6_3,// 파티만들기
		NGS_STEP6,	// 시작하기/준비하기
		NGS_STEP7,
		NGS_STEP8,

		NGS_END
	};

public:
	CX2NoviceGuide(  );
	virtual ~CX2NoviceGuide(void);		
	
	virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT				OnFrameRender();
	virtual bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
			
	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	

	bool GetIsPlayGuide() { return m_bNoviceGuide; }

	// 파티클/메쉬플레이어 설정
	void SetUIMajorParticle(CKTDGParticleSystem *pMajorParticle) { m_pUiParticle = pMajorParticle; }
	void SetUIXMeshPlayer(CKTDGXMeshPlayer *pXMeshPlayer) { m_pUiXMeshPlayer = pXMeshPlayer; }
	CKTDGParticleSystem*		GetUiParticle()				{ return m_pUiParticle; }
	CKTDGXMeshPlayer*			GetUiXMeshPlayer()			{ return m_pUiXMeshPlayer; }

	CX2NoviceGuide::NOVICE_GUIDE_STEP	GetNowGuideStep() { return m_eGuideStep; }	

	void	SetNotifyGuide(CX2NoviceGuide::GUIDE_EVENT event);

	bool	ProcessGuide(double fTime, float fElapsedTime);
	void	SetHide(bool val);
	void	SetGuideStep(int val);
	int		GetMouseClickIndex() { return m_iGuideMouseClick; }
	void	SetMouseClickIndex(int val);
	
	bool	GetShowNoviceNpcGuide()
	{
		if(m_pDlgNpcMessage == NULL)
			return false;
		return m_pDlgNpcMessage->GetShow();
	}

	void InitPartyGuide(bool bFlag = true);	
	void HideGuideDlg();
	void CreateEpicGuideDlg();
	bool GetShowGuideDlg();

private:	
	void	SetPrevStep();
	void	SetNextStep();

	void CreateNpc();
	void CreateArrow();

	bool DoGuideStep1(double fTime, float fElapsedTime);
	bool DoGuideStep2();	
	bool DoGuideStep3();
	bool DoGuideStep4();
	bool DoGuideStep5(double fTime, float fElapsedTime);
	bool DoGuideStep6();
	bool DoGuideStep6_0(int iMouse);
	bool DoGuideStep6_1() { return DoGuideStep6_0(9); }
	bool DoGuideStep6_2() { return DoGuideStep6_0(10); }
	bool DoGuideStep6_3() { return DoGuideStep6_0(11); }	
	bool DoGuideStep7(double fTime, float fElapsedTime);
	bool DoGuideStep8();

	void DeleteGuideDlg();

private:
	CKTDGParticleSystem*								m_pUiParticle;
	CKTDGXMeshPlayer*									m_pUiXMeshPlayer;
	
	bool												m_bNoviceGuide;		// 가이드 진행 여부
	NOVICE_GUIDE_STEP									m_eGuideStep;		// 현재 진행중인 가이드 단계
	NOVICE_GUIDE_STEP									m_eOldGuideStep;

	float												m_fElapsedTime;

	GUIDE_STATE											m_eGuideState;


	// 가이드에 필요한 변수들...
	float												m_fGuideTimer;
	bool												m_bShowArrow;
	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hGuideLeft;
	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hGuideRight;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGParticleSystem::CParticleHandle				m_hGuideLeftParticle;
	CKTDGParticleSystem::CParticleHandle				m_hGuideRightParticle;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGParticleSystem::CParticle*						m_pGuideLeftParticle;
	CKTDGParticleSystem::CParticle*						m_pGuideRightParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	
	CX2UIUnit*											m_pUINPC;		
	CKTDGUIDialogType										m_pDLGNpcName;
	CX2TFieldNpcShop*									m_pDlgNpcMessage;

	int													m_iGuideMouseClick;
	int													m_iPrevStep;
	
	bool												m_bHideMouseClick;
	bool												m_bPlayEventScene;

	CKTDGUIDialogType									m_pDlgEpicGuide;
	CKTDGUIDialogType									m_pDlgItemGuide;
};


#endif //REFORM_NOVICE_GUIDE
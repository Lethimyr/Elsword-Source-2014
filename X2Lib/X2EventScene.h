
#pragma once

#ifdef EVENT_SCENE
//{{ oasis907 : 김상윤 [2010.6.17] // 
class CX2EventScene
{
public: // 구조체 선언
	enum SCENE_OBJECT_POSITION
	{
		OP_NONE,
		OP_LEFT_POS,
		OP_RIGHT_POS,
		OP_CENTER_POS,
		OP_LEFT_SCREEN_OUT,
		OP_RIGHT_SCREEN_OUT,
		OP_CUSTOM_POS,
		OP_END,
	};

	enum SCENE_OBJECT_TYPE
	{
		OT_NONE,
		OT_TEXTURE,
		OT_XSKINMESH,
	};

	class CSceneObject : public CKTDGObject
	{
	public:
		wstring				m_wstrObjectName;
		SCENE_OBJECT_TYPE	m_eSceneObjectType;
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        CKTDGParticleSystem::CParticleEventSequenceHandle   m_hSceneObjectParticleHandle;
        CKTDGXMeshPlayer::CXMeshInstanceHandle              m_hSceneObjectMeshHandle;
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
		int					m_iSceneObjectHandle; // CParticleEventSequenceHandle, CXMeshInstanceHandle 공용 핸들
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
		CKTDGUIDialogType	m_DlgHandle;

		SCENE_OBJECT_POSITION	m_eLastPosition;
		bool					m_bMirrorVertical;

#ifdef EVENT_SCENE_TOOL
		CKTDGParticleSystem*	m_pParticleSystem;
		CKTDGXMeshPlayer*		m_pXMeshPlayer;
#endif EVENT_SCENE_TOOL
		CKTDGXMeshPlayer::XMeshTemplet* m_pXMeshTemplet; // XMesh의 경우 템플릿 메모리 해제를 위해
#ifdef EVENT_SCENE_TOOL
		CSceneObject()
#else
		CSceneObject()
#endif EVENT_SCENE_TOOL
		{
#ifdef EVENT_SCENE_TOOL			
			m_pParticleSystem = NULL;
			m_pXMeshPlayer = NULL;
#endif EVENT_SCENE_TOOL
			m_pXMeshTemplet = NULL;
			m_wstrObjectName = L"";
			m_eSceneObjectType = CX2EventScene::OT_NONE;
#ifndef X2OPTIMIZE_HANDLE_VALIDITY_CHECK
			m_iSceneObjectHandle = -1;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
			m_DlgHandle = NULL;
			m_eLastPosition = CX2EventScene::OP_NONE;
			m_bMirrorVertical = false;
		}
		void OnFrameRender_Draw();
		RENDER_HINT OnFrameRender_Prepare();
	};


	struct SoundPlayData 
	{
		float			m_SoundPlayTime;
		wstring			m_SoundPlayName;
		int				m_SoundPlayRate;
		bool			m_bOnlyIfMyUnit;
		float			m_fMaxSoundDistance;
	};

	struct EventSceneObject
	{
		wstring				m_wstrObjectName;
		wstring				m_wstrFileName;
		wstring				m_wstrXETName;
		SCENE_OBJECT_TYPE	m_eOjectType;
		D3DXVECTOR3			m_vecPosSet[CX2EventScene::OP_END];
		D3DXVECTOR2			m_Size;

		EventSceneObject()
		{
			m_wstrObjectName = L"";
			m_eOjectType = CX2EventScene::OT_NONE;
			m_wstrFileName = L"";
			m_wstrXETName = L"";

			for(UINT i=0; i< CX2EventScene::OP_END -1 ; i++)
			{
				m_vecPosSet[i] = D3DXVECTOR3(0.f, 0.f, 0.f);
			}
			m_Size = D3DXVECTOR2(0.f, 0.f);
		}
	};

	struct SceneObectPlay
	{
		wstring					m_wstrObjectName;
		SCENE_OBJECT_POSITION	m_eStartPosition;
		SCENE_OBJECT_POSITION	m_eMovePosition; // 이동 연출 위해
		float					m_fMoveStartTime;
		float					m_fMoveDuration;

		float					m_fShakeGap;
		float					m_fShakeDuration;
		bool					m_bMirrorVertical;

		wstring					m_wstrAnimName;
		CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE	m_eAnimPlayType;
		float					m_fPlaySpeed;

		SceneObectPlay()
		{
			m_wstrObjectName = L"";
			m_eStartPosition = CX2EventScene::OP_NONE;
			m_eMovePosition = CX2EventScene::OP_NONE;
			m_fMoveStartTime = 0.f;
			m_fMoveDuration	= 0.f;

			m_wstrAnimName = L"";
			m_eAnimPlayType	= CKTDGXSkinAnim::XAP_LOOP;
			m_fPlaySpeed = 0.0f;

			m_fShakeGap = 0.f;
			m_fShakeDuration = 0.f;
			m_bMirrorVertical = false;
		}
	};

	struct TextPlay
	{
		//{{ 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용
#ifdef	EVENT_SCENE_TEXT_USED_INDEX
		int	m_iTextIndex;
		vector<int> m_PlayerTextIndex;
#else	EVENT_SCENE_TEXT_USED_INDEX
		wstring m_wstrText;
		vector<wstring> m_PlayerText;
#endif	EVENT_SCENE_TEXT_USED_INDEX
		//}} 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용
		float m_fWaitTime;
		int m_iTagName;
		bool m_bTagLeft;
		wstring m_wstrTextColor;
		bool m_bTextSpread;
		float m_fTextSpreadSpeed;
		bool m_bHideTagName;
		bool m_bHideTextBox;
		bool m_bUseTagNameForce;

		TextPlay()
		{
			//{{ 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용
#ifdef	EVENT_SCENE_TEXT_USED_INDEX
			m_iTextIndex = STR_ID_EMPTY;
#else	EVENT_SCENE_TEXT_USED_INDEX
			m_wstrText = L"";
#endif	EVENT_SCENE_TEXT_USED_INDEX
			//}} 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용
			m_fWaitTime = -1;
			m_iTagName = 0;
			m_bTagLeft = true;
			m_wstrTextColor = L"";
			m_bTextSpread = true;
			m_fTextSpreadSpeed = 0.f;
			m_bHideTagName = false;
			m_bHideTextBox = false;
			m_bUseTagNameForce = false;
			
		}
	};

	struct EventSequece
	{
		std::list<SceneObectPlay> m_listSceneObectPlay;
		std::list<TextPlay> m_listTextPlay;
		std::vector<wstring> m_vecRemoveObjectName;
		std::vector<SoundPlayData> m_vecSoundPlayData;
	};

	struct EventScene
	{
		wstring m_iEventSceneID;
		std::list<EventSequece> m_listEventSequence;
		EventScene()
		{
			m_iEventSceneID = L"";
		}
	};


public: // oasis907 : 김상윤 [2010.6.23] //  기본 골격 함수
	CX2EventScene(void);
	~CX2EventScene(void);

	virtual void	OnFrameMove( double fTime, float fElapsedTime );
	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


public: // oasis907 : 김상윤 [2010.6.23] //  외부 인터페이스

	// 스크립트 관련 외부 인터페이스 
	bool	OpenScriptFile( const WCHAR* pFileName );
	bool	AddEventSceneObject_LUA(); // 루아에서 호출되는 함수
	bool	AddEventScene_LUA(); // 루아에서 호출되는 함수

	// 이벤트 씬 플레이 관련 외부 인터페이스
	bool	PlayEventScene(wstring wstrEventSceneID, bool bWaitEventScene = false); // 이벤트 씬을 시작시킴
	
	bool	PlayDelayedEventScene(wstring wstrEventSceneID, float fDelayTime);

	// 기타 외부 인터페이스
	void	SetNowState(CX2State* val) { m_pNowState = val; }
	const bool	GetIsPlayingEventScene(); 

	// 툴에서 사용될 리소스 클리어 함수.
	void	Clear();
	void	EndEventScene(bool bIgnoreWaitEventScene = false);
	// 11000 퀘스트 씬 종료시 호출되는 함수

#ifndef REFORM_NOVICE_GUIDE
	void	NoviceGuideTrigger();
#endif //REFORM_NOVICE_GUIDE
	void	NotifyNewEpicQuest() { m_bNotifyNewEpicQuest = true; }

#ifdef ELSWORD_NEW_BEGINNING
	void	BlackLayOutMove( bool bInning, float fTime, bool bPanorama = true, bool WhiteOut = false,  float WhiteOutTime = 0.5f, float DestAlpha = 0.75f);
#else
	void	BlackLayOutMove( bool bInning, float fTime );
#endif ELSWORD_NEW_BEGINNING

protected: // oasis907 : 김상윤 [2010.6.23] // 내부 함수

	// 스크립트 파싱 내부 함수
	bool	LoadEventSequence( KLuaManager& luaManager, std::list<EventSequece>& listEventSequence );
	bool	LoadSceneObjectPlay( KLuaManager& luaManager, std::list<SceneObectPlay>& listSceneObectPlay );
	bool	LoadTextPlay( KLuaManager& luaManager, std::list<TextPlay>& listTextPlay );

	// 이벤트 씬 플레이 관련 내부 함수, 
	void	PlayText(); // PlayEventScene()이 호출된 이후로 해당하는 씬을 진행하는 함수
						// CX2EventScene의 내부의 MsgProc나 OnFrameMove안에서 키입력에 따라 직접 호출 될 수 있음
	bool	PlayEventSequence();
	bool	PlaySceneObjectParticle(EventSceneObject* pEventSceneObject, SceneObectPlay* pSceneObjectPlay);
	bool	PlaySceneObjectXMesh(EventSceneObject* pEventSceneObject, SceneObectPlay* pSceneObjectPlay);


	wstring GetPlayerObjectName();

	wstring GetPlayerText();
	int		GetPlayerTagName();

	//{{ 중요하지 않은 내부 함수들 (CX2SlideShot의 것이 많고, 코드 정리될 예정입니다.) 
	void			SetIsPlayingEventScene( bool bPlaying ); 

	bool			IsPresentNow(){ return m_bNowPresent; }

	void	BlackOutFrameMove( double fTime, float fElapsedTime );
	void	TextPlayFrameMove( double fTime, float fElapsedTime );



	void	CannotInput( float fTime );
	float	GetNowTime(){ return m_fScenTime; }
	bool	EventTimer( float fTime );
	void	SetTextBoxShow( bool bShow ){ m_TextBoxShow = bShow; }
	void 	SetNameTabShow( bool bShow );


	void	SetTextSpread();
	bool	IsTextSpreading();
	float	GetElapsedTimeAfterTextSpread() { return m_fElapsedTimeAfterTextSpread; }
	void	AddText( bool bTagLeft, int NameID, const WCHAR* wszMsg, wstring wstrColor = L"#C000000", bool bSpread = true, float fSpreadTime = 0.1f );
	void	ClearText();
	int		LineBreak( int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor = L"" );
	//}}
#ifdef GUIDE_FIRST_EPIC_QUEST
	void GuideFirstEpicQuest();
#endif //GUIDE_FIRST_EPIC_QUEST
private: // oasis907 : 김상윤 [2010.6.23] // 내부 변수들
#ifdef EVENT_SCENE_TOOL
	CKTDGParticleSystem*	m_pParticleSystem;
	CKTDGXMeshPlayer*		m_pXMeshPlayer;
#endif EVENT_SCENE_TOOL
	std::map<wstring, EventSceneObject> m_mapEventSceneObject; // EventSceneObject.lua에서 파싱한  EventSceneObject들 등록
	std::map<wstring, EventScene> m_mapEventScene;				  // EventScene.lua에서 파싱한  EventScene들 등록

	std::list<CSceneObject*> m_listPlayingSceneObject;    // 플레이 중인 EventSceneObject의 정보, 렌더, 리소스 삭제등을 관리

	std::list<EventSequece>::iterator m_iterES;			// 내부에서 쓰이는 iterator
	std::list<SceneObectPlay>::iterator m_iterSO;
	std::list<TextPlay>::iterator m_iterTE;


	EventScene* m_pNowPlayingEventScene;
	bool	m_bIsPlayingEventScene;


	bool	m_bIsWaitNextEventScene;
	wstring m_wstrNextEventScene;


	std::deque<wstring> m_wstrWaitEventScene;



	bool	m_bNowPresent;
	bool	m_TextBoxShow;
	int		m_NowScenNum;
	string	m_ScenName;
	float	m_fScenTimeBefore;
	float	m_fScenTime;


	bool	m_bBlackOutStart;
	bool	m_bBlackOutEnd;


	float	m_fBlackOutStartDuration;
	float	m_fBlackOutEndDuration;

	float	m_fBlackOutStartLeftTime;
	float	m_fBlackOutEndLeftTime;

#ifdef ELSWORD_NEW_BEGINNING
	float	m_fBlackOutDestAlpha;
	float	m_fWhiteOutTime;
#endif ELSWORD_NEW_BEGINNING

	float	m_fTextPlayElapsedTime;
	
	bool	m_bNotifyNewEpicQuest;
	bool	m_bWaitDelayEventScene;
	float	m_fEventScenePlayDelayTime;



	CX2State*			m_pNowState;		
	CKTDGUIDialogType	m_pDLGSlideShot;
#ifndef EVENT_SCENE_TOOL
	bool				m_bSetCursor;
#endif EVENT_SCENE_TOOL


#ifdef EVENT_SCENE_TOOL
	CKTDGUIDialogType	m_pDLGSceneID;
#endif EVENT_SCENE_TOOL


	CKTDGUIDialogType	m_pDLGBlackLayOut;

	float				m_fElapsedTimeAfterTextSpread;
};
//}} oasis907 : 김상윤 [2010.6.17] // 
#endif EVENT_SCENE

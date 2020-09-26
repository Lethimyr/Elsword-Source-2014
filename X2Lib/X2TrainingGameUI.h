#pragma once




class CX2TrainingGameUI
{
public:
	enum COMMAND_PICTURE
	{
		CP_LEFT,
		CP_RIGHT,
		CP_UP,
		CP_DOWN,

		CP_Z,
		CP_X,
		CP_C,
		CP_S,
		CP_A,

		CP_BLANK,

		CP_0,
		CP_1,
		CP_2,
		CP_3,
		CP_4,
		CP_5,
		CP_6,
		CP_7,
		CP_8,
		CP_9,
		CP_OPEN_PARENTHESIS,
		CP_CLOSE_PARENTHESIS,
		CP_SLASH,

		CP_MINUS,
		CP_ED,

		CP_OR,
		CP_PRESSED,
		CP_SPECIAL_ATTACK,
		CP_DASH_RIGHT,
		CP_DASH_LEFT,
		CP_SIEGE,

		CP_SPACE,
		CP_REVENGE,

		CP_LEFT_PRESSED,
		CP_RIGHT_PRESSED,
		CP_UP_PRESSED,
		CP_DOWN_PRESSED,

		CP_PLUS,
		CP_PRESSED_SMALL,

		CP_BURNING_RUSH,

		CP_SUCCESSIVE_HITS,
	};


	struct WAITING_SEQUENTIAL_USER_STATE
	{
		vector< int >	vecStateID;
		int				iStateIndex;					// vecStateID 에서 몇번째 state인지
		float			fElapsedTimeAfterLastState;		// 이전 state를 성공하고 시간이 얼마나 지났는지
		float			fStateTimeGap;
		int 			iNowCount;
		int 			iMaxCount;
		int 			iStringIndex;					// GUIStatic에서 몇번째 string인지
		wstring 		wstrObjective;

		WAITING_SEQUENTIAL_USER_STATE()
		{
			iStateIndex = 0;
			fElapsedTimeAfterLastState = 0.f;
			fStateTimeGap = 1.f;
			iNowCount = 0;
			iMaxCount = 0;			
			iStringIndex = -1;
			wstrObjective = L"";
		}
		WAITING_SEQUENTIAL_USER_STATE( const WAITING_SEQUENTIAL_USER_STATE& t ) { *this = t; }    // copy constructor
		WAITING_SEQUENTIAL_USER_STATE& operator=( const WAITING_SEQUENTIAL_USER_STATE& t )        // assign operator
		{
			vecStateID = t.vecStateID;
			iStateIndex = t.iStateIndex;
			fElapsedTimeAfterLastState = t.fElapsedTimeAfterLastState;
			fStateTimeGap = t.fStateTimeGap;
			iNowCount = t.iNowCount;
			iMaxCount = t.iMaxCount;
			iStringIndex = t.iStringIndex;
			wstrObjective = t.wstrObjective;

			return *this;
		}
	};
	

	struct WAITING_USER_STATE
	{
		int 	iStateID;
		int 	iNowCount;
		int 	iMaxCount;
		int 	iObjectiveIndex;		
		wstring wstrObjective;

		WAITING_USER_STATE()
		{
			iStateID = -1;
			iNowCount = 0;
			iMaxCount = 0;			
			iObjectiveIndex = -1;
			wstrObjective = L"";
		}
		WAITING_USER_STATE( const WAITING_USER_STATE& t ) { *this = t; }    // copy constructor
		WAITING_USER_STATE& operator=( const WAITING_USER_STATE& t )        // assign operator
		{
			iStateID = t.iStateID;
			iNowCount = t.iNowCount;
			iMaxCount = t.iMaxCount;
			iObjectiveIndex = t.iObjectiveIndex;
			wstrObjective = t.wstrObjective;

			return *this;
		}
	};


	struct WAITING_SIMPLE_USER_STATE
	{
		int 	iStateID;
		int 	iNowCount;
		int 	iMaxCount;
		int		iIsRight; // -1: dont' care, 0: false, 1: true

		WAITING_SIMPLE_USER_STATE()
		{
			iStateID = -1;
			iNowCount = 0;
			iMaxCount = 0;		
			iIsRight = -1;
		}
	};

	struct GATHER_ITEM
	{
		int 	iItemID;
		int 	iMaxCount;
		int 	iNowCount;
		int 	iObjectiveIndex;
		wstring wstrObjective;

		GATHER_ITEM()
		{
			iItemID = -1;
			iMaxCount = 0;
			iNowCount = 0;
			iObjectiveIndex = -1;
			wstrObjective = L"";
		}
		GATHER_ITEM( const GATHER_ITEM& t ) { *this = t; }    // copy constructor
		GATHER_ITEM& operator=( const GATHER_ITEM& t )        // assign operator
		{
			iItemID = t.iItemID;
			iMaxCount = t.iMaxCount;
			iNowCount = t.iNowCount;
			iObjectiveIndex = t.iObjectiveIndex;
			wstrObjective = t.wstrObjective;

			return *this;
		}
	};


	struct WAITING_SPECIAL_ATTACK
	{
		bool	bShouldHitTarget;
		int 	iNowCount;
		int 	iMaxCount;
		int 	iStringIndex;		// GUIStatic에서 몇번째 string인지
		wstring wstrObjective;

		WAITING_SPECIAL_ATTACK()
		{
			bShouldHitTarget = false;
			iNowCount = 0;
			iMaxCount = 0;
			iStringIndex = -1;
			wstrObjective = L"";
		}
		WAITING_SPECIAL_ATTACK( const WAITING_SPECIAL_ATTACK& t ) { *this = t; }    // copy constructor
		WAITING_SPECIAL_ATTACK& operator=( const WAITING_SPECIAL_ATTACK& t )        // assign operator
		{
			bShouldHitTarget = t.bShouldHitTarget;
			iNowCount = t.iNowCount;
			iMaxCount = t.iMaxCount;
			iStringIndex = t.iStringIndex;
			wstrObjective = t.wstrObjective;

			return *this;
		}
	};


	struct KEY_SEQUENCE
	{
		float	m_fRemainTime;

		KEY_SEQUENCE()
		{
			m_fRemainTime = 0.f;
		}
	};

	

	struct TRAINING_OBJECTIVE
	{
		D3DXVECTOR2 m_vFirstPicturePos;
		D3DXVECTOR2 m_vNowPicturePos;
		D3DXVECTOR2 m_vPictureSize;
		int			m_iPictureIndexOfNowSuccessCount;

		TRAINING_OBJECTIVE()
		{
			m_vPictureSize		= D3DXVECTOR2( 52.f, 50.f );
			m_vFirstPicturePos	= D3DXVECTOR2( 0.f, 0.f );
			m_vNowPicturePos	= D3DXVECTOR2( 0.f, 0.f );
			m_iPictureIndexOfNowSuccessCount = -1;
		}
	};

	

public:
	CX2TrainingGameUI( CX2State* pNowState );
	~CX2TrainingGameUI(void);

	void		OnFrameMove( double fTime, float fElapsedTime );

	void		ScenStart_LUA( char* pScenName, int scenNum );
	void		ScenWait();
	void		ScenEnd();

	void		GoNextScen();
	void		AddText_LUA( int str_id, float fFullTime = 0.0f );

	float		GetNowTime(){ return m_fScenTime; }
	bool		EventTimer( float fTime );



	bool GetSceneStarted() const { return m_bSceneStarted; }

	float GetElapsedTimeAfterTextSpread() const { return m_fElapsedTimeAfterTextSpread; }

	bool IsKeyPressed( int key );  

	void ClearWaitingKeyPress();
	void AddWaitingKeyPress( int key, bool bUseTextureWidth = false );
	void AddWaitingKeyPressScaled( int key, bool bUseTextureSize = false, D3DXVECTOR2 vScale = D3DXVECTOR2(1, 1) );
	

	void AddKeySequence( unsigned char key );
	void ClearKeySequence();

	void SetTextSpread();
	bool IsTextSpreading();


	void SetEnableWaitingUserState(bool val) { m_bEnableWaitingUserState = val; }
	void AddWaitingUserState_LUA( int iStateID, int iMaxCount, int iStringIndex, const char* szTitle );
	void AddWaitingUserState( int iStateID, int iMaxCount, int iStringIndex, const WCHAR* wszTitle );
	void ClearWaitingUserState() { m_vecWaitingUserState.clear(); }
	bool IsWaitingUserState();

	
	void ClearSimpleWaitingUserState() { m_vecWaitingSimpleUserState.clear(); }
	void AddSimpleWaitingUserState( int iStateID, int iMaxCount, int iIsRight );
	bool IsWaitingSimpleUserState();
	float GetElapsedTimeAfterCompletingSimpleUserState() const { return m_fElapsedTimeAfterCompletingSimpleUserState; }


	void ClearGatherItem() { m_vecGatherItem.clear(); }
	void AddGatherItem_LUA( int iItemID, int iMaxCount, int iStringIndex, const char* szTitle = "" );
	void AddGatherItem( int iItemID, int iMaxCount, int iStringIndex, const WCHAR* wszTitle = L"" );
	bool IsWaitingGatherItem();
	void UpdateGatherItem( int iItemID, int iCount );


	void ClearWaitingSpecialAttack() { m_vecWaitingSpecialAttack.clear(); }
	void AddWaitingSpecialAttack_LUA( int iMaxCount, int iStringIndex, const char* szTitle, bool bShouldHitTarget = false );
	void AddWaitingSpecialAttack( int iMaxCount, int iStringIndex, const WCHAR* wszTitle, bool bShouldHitTarget = false );
	bool IsWaitingSpecialAttack();


	int AddWaitingSeqUserState_LUA( float fTimeGap, int iMaxCount, int iStringIndex, const char* szTitle );
	int AddWaitingSeqUserState( float fTimeGap, int iMaxCount, int iStringIndex, const WCHAR* wszTitle );
	void SetWaitingSeqUserState( int iSeqUserStateIndex, int iStateID0, int iStateID1, int iStateID2 );
	void ClearWaitingSeqUserState() { m_vecWaitingSequentialUserState.clear(); }
	bool IsWaitingSeqUserState();


	void ClearTrainingObjective();
	int AddTrainingObjective( D3DXVECTOR2 vPos, D3DXVECTOR2 vPictureSize, int iIndexOfCount );
	bool AddTrainingObjectivePicture( int iObjectiveIndex, int iKeyPicture, bool bNextLine, bool bUseTextureWidth = false  );
	bool AddTrainingObjectivePictureScaled( int iObjectiveIndex, int iKeyPicture, D3DXVECTOR2 vScale = D3DXVECTOR2(1,1), bool bUseTextureWidth = true );

	void SetTrainingObjectiveSuccessCount( int iObjectiveIndex, int nCount );

	void SetShowEnterMark( bool bShow );
	void SetTwinkleEnterMark( float fTime );

	void SetShowSuccessCountEffect(bool val) { m_bShowSuccessCountEffect = val; }




	void OpenSlideWindow( bool bOpen );
	void OpenFreeTrainingMainMenu( bool bOpen );

	CX2UnitManager::NPC_UNIT_ID GetFTNPCID() const { return m_eFTNPCID; }
	void SetFTNPCID(CX2UnitManager::NPC_UNIT_ID val) { m_eFTNPCID = val; }
	int GetFTNPCCount() const { return m_iFTNPCCount; }
	void SetFTNPCCount(int val) { m_iFTNPCCount = val; }
	int GetFTNPCLevel() const { return m_iFTNPCLevel; }
	void SetFTNPCLevel(int val) 
	{ 
		if( val <= 0 ) 
			m_iFTNPCLevel = 1;
		else
			m_iFTNPCLevel = val; 
	}
	bool GetFTEnableNPCAttack() const { return m_bFTEnableNPCAttack; }
	void SetFTEnableNPCAttack(bool val) { m_bFTEnableNPCAttack = val; }

	CX2UnitManager::NPC_UNIT_ID GetNpcIdByIndex(int iVal)
	{
		if( iVal < 0 || iVal > (int)m_vecNpc.size() )
			return CX2UnitManager::NUI_NONE;
		return m_vecNpc[iVal];
	}

protected:		
	void		AddText( const WCHAR* wszMsg, wstring wstrColor = L"#C000000", bool bSpread = true, float fSpreadTime = 0.1f );
	void		ClearText();
	int			LineBreak( int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor = L"" );
	void		SetPictureTex( CKTDGUIStatic::CPictureData* pPicture, COMMAND_PICTURE eKey );
	



private:
	int			m_NowScenNum;
	string		m_ScenName;
	float		m_fScenTimeBefore;
	float		m_fScenTime;
	bool		m_bSceneStarted;	
	
	float		m_fElapsedTimeAfterTextSpread;


	CX2State*			m_pNowState;		
	CKTDGUIDialogType		m_pDLGSlideShot;


	bool				m_bEnableWaitingUserState;
	vector< WAITING_USER_STATE >			m_vecWaitingUserState;
	bool		m_bWasWaitState;			

	vector< WAITING_SIMPLE_USER_STATE >		m_vecWaitingSimpleUserState;
	bool		m_bWasSimpleWaitState;		
	float		m_fElapsedTimeAfterCompletingSimpleUserState;
	
	vector< GATHER_ITEM >					m_vecGatherItem;
	vector< WAITING_SPECIAL_ATTACK >		m_vecWaitingSpecialAttack;
	vector< WAITING_SEQUENTIAL_USER_STATE > m_vecWaitingSequentialUserState;

	vector< KEY_SEQUENCE >					m_vecKeySequence;

	vector< TRAINING_OBJECTIVE >			m_vecTrainingObjective;
	
	int			m_iNowKeySequenceIndex;
	int			m_iNowObjectiveCount;	

	float		m_fRemainTimeEnterMarkTwinkle;

	bool		m_bShowSuccessCountEffect;
	int			m_iLastIndexWaitingKeyPress;


	// 자유훈련
	CKTDGUIDialogType m_pDLGFreeTrainingMenu;

	CX2UnitManager::NPC_UNIT_ID m_eFTNPCID;	// 몬스터 종류
	int				m_iFTNPCCount;
	int				m_iFTNPCLevel;
	bool			m_bFTEnableNPCAttack;

	vector<CX2UnitManager::NPC_UNIT_ID> m_vecNpc;
	
};



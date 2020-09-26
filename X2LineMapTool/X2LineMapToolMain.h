#pragma once



class CX2LineMapToolMain : public CKTDXStage
{
public:
	struct LINE_MAP_VERTEX
	{
		D3DXVECTOR3	pos;
		DWORD		color;
	};

	struct CUSTOMVERTEX
	{
		union
		{
			struct
			{
				FLOAT   x;    
				FLOAT   y;
				FLOAT   z;
			};
			D3DVECTOR  p;
		};
		D3DVECTOR	normal;
		DWORD		color;
	};

	
	enum DIALOG_CONTROL_ID 
	{
		IDC_STATIC_DUMMY,
		IDC_BUTTON_HELP,

        IDC_CHECK_SECTOR,
        IDC_EDIT_WIDTH,
        IDC_EDIT_HEIGHT,
        IDC_EDIT_DEPTH,
        IDC_BUTTON_FILE_TILE_SAVE,
        IDC_BUTTON_TEXT_TILE_SAVE,

		IDC_BUTTON_FILE_OPEN,
		IDC_BUTTON_FILE_SAVE,
		IDC_BUTTON_TO_TEXT,
		IDC_BUTTON_FROM_TEXT,
		IDC_CHECK_NAVIGATE,

		IDC_STATIC_TEAM_COLOR,
		IDC_STATIC_INITIAL_DIR,
		IDC_BUTTON_TEAM_COLOR,
		IDC_BUTTON_INITIAL_DIR,

		IDC_STATIC_LAND_HEIGHT,
		IDC_EDIT_LAND_HEIGHT,

		// start position 정보
		IDC_STATIC_START_POS,
		IDC_LISTBOX_START_POS,
		//IDC_EDIT_START_POS_NUM,		
		IDC_COMBOBOX_START_POS_TEAM_COLOR,
		IDC_COMBOBOX_START_POS_DIRECTION,

		// line segement 정보
		IDC_STATIC_SEGMENT_INFO,
		IDC_LISTBOX_SEGMENT_INFO,
		IDC_EDIT_SEGMENT_INDEX,
		IDC_COMBOBOX_SEGMENT_ENABLE,
		IDC_COMBOBOX_SEGMENT_LINE_TYPE,
		IDC_EDIT_SEGMENT_PORTAL_GROUP,
		IDC_EDIT_SEGMENT_STOP_FACTOR,
		IDC_EDIT_SEGMENT_FOOT_DUST_NAME,
		IDC_COMBOBOX_SEGMENT_BEFORE_LINE,
		IDC_COMBOBOX_SEGMENT_NEXT_LINE,

		IDC_COMBOBOX_LINE_DUST_TYPE,

		IDC_COMBOBOX_CAN_DOWN,
		IDC_COMBOBOX_CAN_PASS,
		IDC_COMBOBOX_MONSTER_ALLOWED,
		IDC_COMBOBOX_ENABLE_WALL_JUMP,
		IDC_EDIT_TELEPORT_LINE_INDEX,
		IDC_EDIT_JUMP_SPEED,




		IDC_BUTTON_PICKED_LINE_INDEX,
		IDC_BUTTON_BUILD_FLOOR_MESH,

		IDC_EDIT_LINE_START_POS,
		IDC_EDIT_LINE_END_POS,
		
		IDC_EDIT_LINE_SET_INDEX,
		IDC_EDIT_LINE_SPEED,

		IDC_COMBOBOX_DUNGEON_DIFFICULTY, 
		IDC_BUTTON_MOUSE_CLICK_TARGET,

		IDC_BUTTON_LOAD_NPC_FROM_DUNGEON_SCRIPT,
		IDC_BUTTON_USE_NPC_MESH,
		IDC_LIST_ANY_INFO,

		IDC_BUTTON_SHOW_USER_UNIT_START_POS,

		IDC_EDIT_ALL_NPC_START_POS_DISPLACE,
		IDC_EDIT_ALL_LINE_POS_DISPLACE,

		IDC_CHECK_SHOW_START_POS_SIMPLE,
		
	};

	enum START_DIR
	{
		DIR_LEFT,
		DIR_RIGHT,
	};

	enum TEAM_COLOR
	{
		TEAM_NONE,
		TEAM_RED,
		TEAM_BLUE,
	};

	enum MOUSE_CLICK_TARGET
	{
		MCT_LINE,
		MCT_NPC,
	};


	struct SNPCStartPos
	{
		CX2DungeonSubStage::NPCData*		m_pNPCData;
		int									m_iStartPosIndex;

		SNPCStartPos( CX2DungeonSubStage::NPCData* pNPCData, int iStartIndex )
		{
			m_pNPCData			= pNPCData;
			m_iStartPosIndex	= iStartIndex;
		}
	};
    

	struct STeamStartPos
	{
		int key;
		int iTeamColor;
		int iInitialDir;
		D3DXVECTOR3 vPos;
		int iLineIndex;

		STeamStartPos()
		{

		}

		STeamStartPos( const int key, const int iTeamColor, const D3DXVECTOR3& vPos, const int iInitialDir, int iLineIndex = 0 )
		{
			this->key				= key;
			this->iTeamColor		= iTeamColor;
			this->vPos				= vPos;
			this->iInitialDir		= iInitialDir;
			this->iLineIndex		= iLineIndex;
		}
	};

	class CTeamStartPosSort
	{
	public: 
		bool operator() ( const STeamStartPos& lhs, const STeamStartPos& rhs ) const
		{ 
			if( lhs.iTeamColor > rhs.iTeamColor )
				return true;
			else if( lhs.iTeamColor == rhs.iTeamColor && lhs.key < rhs.key )
				return true;
			else
				return false;
		}
	};	



public:
	CX2LineMapToolMain(void);
	virtual ~CX2LineMapToolMain(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	void InitUI();
	bool LoadLineMapFromFile( WCHAR* wszFileName );
	bool SaveLineMapToFile( WCHAR* wszFileName, bool bUpdateFileName = true );
    bool SaveSectorFile( WCHAR* wszFileName, bool bUpdateFileName = true );
	void AddNewStartPosition(const POINTS& mousepos); 
	int PickASegment(const POINTS& mousepos);
	float DistanceBetweenPointAndSegment(float& fLerpCoef, D3DXVECTOR3* pvPoint, D3DXVECTOR3* pvSegmentStart, D3DXVECTOR3* pvSegmentEnd);
	bool HighlightPickedSegments();
	HRESULT CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPD3DXMESH* ppMesh, TCHAR* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic, WCHAR* wszText);

	// UI 관련
	void UpdateStartPosListBox(bool bClearAll);
	void UpdateStartPosComboBox();
	void UpdateSegmentInfoListBox( bool bClearAll );
	void UpdateSegmentInfoComboBox( bool bClearAll );

	CKTDGLineMap::LineData* GetLineData( int iIndex ) {	return pLineMap->GetLineData(iIndex); }
	STeamStartPos& GetStartPos(int iIndex) { return (STeamStartPos&)vecTeamStartPos[iIndex]; }
	CDXUTDialog& GetMainUI() { return MainUI; }
	int& GetDir() { return iInitialDir; }
	int& GetTeamColor() { return iTeamColor; }
	const WCHAR* GetFilePath() { return m_wstrLineMapFileName.c_str(); }
	WCHAR* GetTempFilePath() { return wszTempFilePath; }
	void ToggleNavigate() { bNavigate = !bNavigate; }
	int& GetPickedStartPosIndex() { return m_iPickedStartPosIndex; }
	const int GetNumSegments() { if(pLineMap==NULL) return 0; else return pLineMap->GetNumLineData(); }
	void SetLandHeight( float fLandHeight ) { if( pLineMap != NULL ) pLineMap->SetLandHeight(fLandHeight); }
	float GetLandHeight() { if( pLineMap != NULL ) return pLineMap->GetLandHeight(); else return 0; }
	vector<int>& GetPickedSegmentVector() { return m_veciPickedSegmentIndex; }
	void GetLineIndexInRect( float fLeft, float fTop, float fRight, float fBottom );

	void BuildFloorMesh( WCHAR* wszFileName, float floorWidth, float floorHeight );
	void PickLineSet( int iLineSetIndex );
	


	void RenderNPCStartPosIndex( CX2DungeonSubStage::NPCData* pNPCData, int iStageNPCIndex = -1, int iNPCSubIndex = -1 );

	HRESULT RenderNPCPostion( int iDungeonLevel );
	void DeleteAllSkinMeshAndSkinAnim();
	void LoadNPCSkinMesh( CX2UnitManager::NPC_UNIT_ID unitID, const WCHAR* wszSkinMeshFileName );
	CKTDXDeviceXSkinMesh* GetNPCSkinMesh( CX2UnitManager::NPC_UNIT_ID unitID );
	void InitX2Data();
	void RegisterLuabind();
	bool OpenScriptFile( const WCHAR* pFileName );
	void ComputeRay( D3DXVECTOR3* pvRayOrigin, D3DXVECTOR3* pvRayDirection, const POINTS& mouspos,
		const D3DXMATRIX* pmatView, const D3DXMATRIX* pmatProj );
	int& GetShowDungeonLevel() { return m_iShowDungeonLevel; }
	bool NPCRayHit( D3DXVECTOR3 vPos, D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance );
	bool GetMousePosOnLine( D3DXVECTOR3* pvPosOnLine, const POINTS& mousepos );
	bool PickClickedNPC( int iDungeonLevel, POINTS mousepos, bool bClear = true );

	void CollisionCheckNPCStartPos( CX2DungeonSubStage::SubStageData* pSubStageData, D3DXVECTOR3 vRayStart, D3DXVECTOR3 vRayEnd, 
				float fHitDistance, float fShortestHitDistance, CX2DungeonSubStage::NPCData* &pPickedNPCData, int &iPickedNPCStartIndex );

	void UpdateNPCStartPosIndex();
	void UpdateNPCInfoUI();

	float ComputeLandHeight();
	void DisplaceAllUserStartPos( D3DXVECTOR3 vDisplace );
	void DisplaceAllNPCStartPos( D3DXVECTOR3 vDisplace );

	//////////////////////////////////////////////////////////////////////////
	bool LoadNPCPostionFromDungeonScript( const WCHAR* wszFileFullPath );

	void RenderUserUnitStartPos();
	void RenderNPCUnitStartPostion( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficulty );

	void DeleteNPCXSkinAnim( CX2DungeonSubStage::NPCData* pNPCData );
	void DeleteNPCSkinAnims( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficultyLevel );

	CKTDGXSkinAnim* LoadNPCSkinAnim( CX2DungeonSubStage::NPCData* pNPCData, int iSubIndex );
	void LoadNPCSkinAnims( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficultyLevel );
	void CloseDungeonScript( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficultyLevel );

	int GetNextUniqueNPCStartPosKey()
	{
		for( int i=1; i<sizeof(m_abKeyUsed); i++ )
		{
			m_CurrKeyUsed = (m_CurrKeyUsed + 1) % (int)sizeof(m_abKeyUsed);
			if( false == m_abKeyUsed[m_CurrKeyUsed] )
			{
				return m_CurrKeyUsed;
			}
		}

		return -1;
	}
	
	CX2LineMapToolMain::STeamStartPos* GetTeamStartPos( int iNPCStartPosKey );
	bool CheckDungeonScriptError( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficultyLevel );

	void UpdateAnyInfoUI();

    void SetSectorWidth(int val)    { m_iSectorWidth = val; }
    void SetSectorHeight(int val)   { m_iSectorHeight = val; }
    void SetSectorDepth(int val)    { m_iSectorDepth = val; }
    void SetToggleSector()          { m_bTileRender = !m_bTileRender; }
    bool CreateSector();
    void OnRenderTile();

protected:
	void RecomputeStartPosLineIndex();
	void LineMapFloatToInt();

public:
	bool				m_bShowUserUnitStartPos;
	bool				m_bUseNPCMesh;
	MOUSE_CLICK_TARGET	m_MouseClickTarget;				// line을 클릭할지 NPC를 클릭할지
	bool				m_bHideStartPosMesh;

private:
	CKTDGLineMap*		pLineMap;
	CDXUTDialog			MainUI;
	CDXUTDialog			m_DungeonUI;
	bool				m_bEnableUI;

	bool	bNavigate;
	int		iTeamColor;
	int		iInitialDir;

	vector<STeamStartPos>		vecTeamStartPos;

	int			m_iPickedStartPosIndex;
	vector<int> m_veciPickedSegmentIndex;
	

	CKTDXDeviceXSkinMesh*		m_pXSkinMesh;
	vector<CKTDGXSkinAnim*>		vecpXSkinAnim;
	CKTDXDeviceXMesh*			m_pXMeshCube;
	LPDIRECT3DVERTEXBUFFER		m_pVBHighlightedSegment; 
	unsigned					m_nVBHighlightedSegmentSize;
	D3DXCOLOR					m_coHighlight;

	ID3DXMesh*					m_pMesh3DText[12];
	float						m_f3DTextAngle, m_f3DTextAngleStep;

	CFirstPersonCamera			m_FPSCamera;



	bool			m_bMouseDragging;
	D3DXVECTOR2		m_vDragStart;

	map< CX2UnitManager::NPC_UNIT_ID, CKTDXDeviceXSkinMesh* >		m_mapXSkinMesh;

	int								m_iShowDungeonLevel;
	
	CKTDGMatrix*					m_pMatrix;

	WCHAR							wszTempFilePath[MAX_PATH]; // edit 중인 script 파일의 이름
	wstring							m_wstrLineMapFileName;		// currently editing linemap filename
	wstring							m_wstrDungeonFileName[3];

	CX2Dungeon*						m_pNormalDungeon;
	CX2Dungeon*						m_pHardDungeon;
	CX2Dungeon*						m_pExpertDungeon;

	CX2DungeonStage::StageData*		m_pNormalStageData;
	CX2DungeonStage::StageData*		m_pHardStageData;
	CX2DungeonStage::StageData*		m_pExpertStageData;

	map< std::pair<CX2DungeonSubStage::NPCData*, int>, CKTDGXSkinAnim* > m_mapNPCXSkinAnim;	// npc와 그 npc의 몇번째 시작 위치인가를 조합해서 키로
	
	int								m_CurrKeyUsed;
	bool							m_abKeyUsed[1024];

	vector< SNPCStartPos >			m_vecPickedNPCStartPosKey;

    int                             m_iSectorWidth;
    int                             m_iSectorHeight;
    int                             m_iSectorDepth;
    bool                            m_bTileRender;

    ID3DXMesh*                      m_pBox[1];
    std::vector<D3DXVECTOR3>        m_vecCenter;
};

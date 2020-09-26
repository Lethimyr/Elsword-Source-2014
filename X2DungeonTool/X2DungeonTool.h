#pragma once

#include "X2DTDropTable.h"
#include "X2DTGUNPC.h"

#define SET_POINT(point, tx, ty)	 point.x = tx;point.y = ty;
#define ADD_POINT(point, ax, ay)	 point.x += ax;point.y += ay;
#define MAKEPOINTS(l)				 (*((POINTS FAR *)&(l)))
#define BOOL_TO_STRING( _bool, _str ) \
if( _bool ) \
{ \
_str = "TRUE"; \
} \
else \
{ \
_str = "FALSE"; \
} \

#define RED_COLOR		0xffff0000
#define BLACK_COLOR		0xff000000
#define IS_NULL_POINTER( p ) \
if( p == NULL ) \
{ \
return; \
} \

#define INT_TO_WSTRING( int_value ) boost::str( boost::wformat( L"%d" ) % static_cast<int>(int_value) ).c_str()
#define WSTRING_TO_INT( wstr_value, int_value ) \
std::string str; \
ConvertWCHARToChar( str, wstr_value.c_str() ); \
int_value = atoi( str.c_str() ); \

#define VERIFY_RGB_VALUE( wstr ) \
{ \
	std::string strTemp; \
	ConvertWCHARToChar( strTemp, wstr ); \
	if( atoi( strTemp.c_str() ) < -1 || atoi( strTemp.c_str() ) > 255 ) \
	{ \
		MessageBox( NULL, L"RGB값은 0보다 크고 255보다 작아야합니다! -1값은 RGB를 적용하지않도록 처리합니다!", L"", MB_OK ); \
		break; \
	} \
} \

struct GET_GO_TYPE
{	
	CX2DungeonSubStage::GO_TYPE operator()( std::wstring& data )
	{
		if( std::wstring(L"NONE") == data )
			return CX2DungeonSubStage::GO_TYPE::GT_NONE;
		else if( std::wstring(L"RIGHT") == data )
			return CX2DungeonSubStage::GO_TYPE::GT_RIGHT;
		else if( std::wstring(L"LEFT") == data )
			return CX2DungeonSubStage::GO_TYPE::GT_LEFT;
		else if( std::wstring(L"UP") == data )
			return CX2DungeonSubStage::GO_TYPE::GT_UP;
		else if( std::wstring(L"DOWN") == data )
			return CX2DungeonSubStage::GO_TYPE::GT_DOWN;
		else
			return CX2DungeonSubStage::GO_TYPE::GT_NONE;
	}
};

struct GET_CLEAR_CONDITION
{	
	CX2DungeonSubStage::CLEAR_CONDITION operator()( std::wstring& data )
	{
		if( std::wstring(L"KILL_ALL_ACTIVE_NPC") == data )
			return CX2DungeonSubStage::CLEAR_CONDITION::CC_KILL_ALL_ACTIVE_NPC;
		else if( std::wstring(L"KILL_KEYCODE_NPC") == data )
			return CX2DungeonSubStage::CLEAR_CONDITION::CC_KILL_KEYCODE_NPC;
		else if( std::wstring(L"KILL_KEYCODE_NPC_ANY") == data )
			return CX2DungeonSubStage::CLEAR_CONDITION::CC_KILL_KEYCODE_NPC_ANY;
		else if( std::wstring(L"KILL_NPC_TYPE") == data )
			return CX2DungeonSubStage::CLEAR_CONDITION::CC_KILL_NPC_TYPE;
		else
			return CX2DungeonSubStage::CLEAR_CONDITION::CC_KILL_KEYCODE_NPC;
	}

	static std::string GetClearConditionString( CX2DungeonSubStage::CLEAR_CONDITION data )
	{
		switch( data )
		{
		case CX2DungeonSubStage::CLEAR_CONDITION::CC_KILL_ALL_ACTIVE_NPC:
			return std::string( "CC_KILL_ALL_ACTIVE_NPC" );

		case CX2DungeonSubStage::CLEAR_CONDITION::CC_KILL_KEYCODE_NPC:
			return std::string( "CC_KILL_KEYCODE_NPC" );

		case CX2DungeonSubStage::CLEAR_CONDITION::CC_KILL_KEYCODE_NPC_ANY:
			return std::string( "CC_KILL_KEYCODE_NPC_ANY" );

		case CX2DungeonSubStage::CLEAR_CONDITION::CC_KILL_NPC_TYPE:
			return std::string( "CC_KILL_NPC_TYPE" );

		default:
			return std::string( "잘못된 CLEAR_CONDITION값입니다!" );
		}
	}
};

struct GET_CLEAR_TYPE
{	
	CX2DungeonSubStage::CLEAR_TYPE operator()( std::wstring& data )
	{
		if( std::wstring(L"SUB_STAGE") == data )
			return CX2DungeonSubStage::CLEAR_TYPE::CT_SUB_STAGE;
		else if( std::wstring(L"STAGE") == data )
			return CX2DungeonSubStage::CLEAR_TYPE::CT_STAGE;
		else if( std::wstring(L"GAME") == data )
			return CX2DungeonSubStage::CLEAR_TYPE::CT_GAME;
		else
			return CX2DungeonSubStage::CLEAR_TYPE::CT_STAGE;
	}

	static std::string GetClearTypeString( CX2DungeonSubStage::CLEAR_TYPE data )
	{
		switch( data ) 
		{
		case CX2DungeonSubStage::CLEAR_TYPE::CT_SUB_STAGE:
			return std::string( "CT_SUB_STAGE" );

		case CX2DungeonSubStage::CLEAR_TYPE::CT_STAGE:
			return std::string( "CT_STAGE" );

		case CX2DungeonSubStage::CLEAR_TYPE::CT_GAME:
			return std::string( "CT_GAME" );

		default:
			return std::string( "잘못된 CLEAR_TYPE값입니다!" );
		}
	}
};

struct GET_INDEX_NUMBER
{
	UINT operator()( std::wstring& data )
	{
		std::string temp;
		ConvertWCHARToChar( temp, data.c_str() );
		return atoi( temp.c_str() );
	}
};

class KX2DungeonToolCamera : public CFirstPersonCamera
{
public:
	KX2DungeonToolCamera()
	{
		m_fMoveSpeed = 1000.f;
	};
	~KX2DungeonToolCamera() {};


	//const float GetMoveSpeed() { return m_fMoveSpeed; }
	//void IncMoveSpeed() { m_fMoveSpeed += 100.f; }
	//void DecMoveSpeed() { m_fMoveSpeed -= 100.f; if( m_fMoveSpeed < 0.f ) m_fMoveSpeed = 100.f; }	

public:
	float		m_fMoveSpeed;
};

class KX2DungeonTool : public CKTDXStage
{
public:
	struct LINE_MAP_VERTEX
	{
		D3DXVECTOR3	pos;
		DWORD		color;
	};

	enum DIALOG_CONTROL_ID 
	{
		IDC_STATIC,

		// MenuUI
		IDC_GO_TO_MAIN_BUTTON,
		IDC_LOAD_LUA_BUTTON,
		IDC_SAVE_LUA_BUTTON,
		IDC_STAGE_INFO_VISIBLE_CHECK,
		IDC_CURTAIN_VISIBLE_CHECK,
		IDC_NPC_VISIBLE_CHECK,
		IDC_TRIGGER_VISIBLE_CHECK,
		IDC_SUBSTAGE_VISIBLE_CHECK,
		IDC_CLEAR_CONDITION_VISIBLE_CHECK,
		IDC_TOOL_UI_SHOW_CHECK,
		IDC_MAP_UI_SHOW_CHECK,
		IDC_WORLD_SHOW_CHECK,
		IDC_LINE_MAP_SHOW_CHECK,
		IDC_NPC_SHOW_CHECK,
		IDC_CURTAIN_SHOW_CHECK,
		IDC_START_POS_SHOW_CHECK,
		IDC_LINE_MAP_NUMBER_SHOW_CHECK,
		IDC_LINE_NUMBER_EDIT,
		IDC_AUTO_SAVE_CHECK,
		IDC_NPC_COLOR_CHECK,
		IDC_SUMMARY_UI_CHECK,
		IDC_ONLY_SUBS_NPC_CHECK,
		IDC_NEAR_RANGE_CHECK,
		IDC_TARGET_RANGE_CHECK,
		IDC_LOST_RANGE_CHECK,
		IDC_ONLY_SELECT_CHECK,
		IDC_STAGE_LOAD_COUNT_STATIC,
		IDC_NOW_EDITING_DUNGEON_EDIT,
		IDC_NOW_EDITING_STAGE_EDIT,

		// MainUI
		IDC_DUNGEON_LIST,
		IDC_DUNGEON_LOAD_BUTTON,
		IDC_STAGE_LIST,
		IDC_STAGE_LOAD_BUTTON,
		IDC_VIEW_ALL_WORLD_BUTTON,
		IDC_ADD_STAGE_BUTTON,
		IDC_DEL_STAGE_BUTTON,
		IDC_WORLD_ID_COMBO,
		IDC_STATIC_DUNGEON_WORLD_COLOR,
		IDC_DUNGEON_WORLD_COLOR_R,
		IDC_DUNGEON_WORLD_COLOR_G,
		IDC_DUNGEON_WORLD_COLOR_B,
		IDC_STATIC_DUNGEON_UNIT_COLOR,
		IDC_DUNGEON_UNIT_COLOR_R,
		IDC_DUNGEON_UNIT_COLOR_G,
		IDC_DUNGEON_UNIT_COLOR_B,

		// StageInfo
		IDC_STATIC_STAGE_INFO,
		IDC_STATIC_STAGE_INFO_1,
		IDC_STATIC_STAGE_INFO_2,
		IDC_SUB_STAGE_LIST,
		IDC_ADD_SUB_STAGE_BUTTON,
		IDC_DEL_SUB_STAGE_BUTTON,
		IDC_READY_NPC_LIST,
		IDC_NPC_ID_LIST_COMBO,
		IDC_ADD_READY_NPC_BUTTON,
		IDC_DEL_READY_NPC_BUTTON,
		IDC_START_MOTION_CHECK,
		IDC_STATIC_WORLD_COLOR,
		IDC_WORLD_COLOR_R,
		IDC_WORLD_COLOR_G,
		IDC_WORLD_COLOR_B,
		IDC_STATIC_UNIT_COLOR,
		IDC_UNIT_COLOR_R,
		IDC_UNIT_COLOR_G,
		IDC_UNIT_COLOR_B,

		// Curtain
		IDC_STATIC_CURTAIN,
		IDC_STATIC_CURTAIN_1,
		IDC_CURTAIN_LIST,
		IDC_ADD_CURTAIN_BUTTON,
		IDC_DEL_CURTAIN_BUTTON,
		IDC_LINE_INDEX_EDIT,
		IDC_AT_START_CHECK,
		IDC_LOOK_LEFT_CHECK,

		// NPC
		IDC_STATIC_NPC,
		IDC_STATIC_NPC_1,
		IDC_STATIC_NPC_2,
		IDC_STATIC_NPC_3,
		IDC_STATIC_NPC_4,
		IDC_STATIC_NPC_5,
		IDC_NPC_LIST,
		IDC_NPC_LIST_COMBO,
		IDC_ADD_NPC_BUTTON,
		IDC_DEL_NPC_BUTTON,
		IDC_NPC_ID_EDIT,
		IDC_START_POS1_EDIT,
		IDC_START_POS2_EDIT,
		IDC_ADD_POS_Y_EDIT,
		IDC_LEVEL_EDIT,
		IDC_ACTIVE_CHECK,
		IDC_GAGE_BAR_CHECK,
		IDC_RATE_EDIT,
		IDC_STATIC_NPC_KEY_CODE,
		IDC_NPC_KEY_CODE_EDIT,
		IDC_FOCUS_CAMERA_CHECK,
		IDC_HAVE_BOSS_GAGE_CHECK,
		IDC_SUB_NPC_CHECK,
		IDC_SIEGE_MODE_CHECK,
		IDC_IS_RIGHT_CHECK,

		// Trigger
		IDC_STATIC_TRIGGER,
		IDC_STATIC_TRIGGER_1,
		IDC_TRIGGER_LIST,
		IDC_ADD_TRIGGER_BUTTON,
		IDC_DEL_TRIGGER_BUTTON,
		IDC_HOST_CHECK,
		IDC_ONE_TIME_CHECK,
		IDC_INTERVAL_EDIT,
		IDC_CONDITION_FUNC_BUTTON,
		IDC_REACT_FUNG_BUTTON,
		IDC_FUNCTION_VIEWER_EDIT,

		// SubStageInfo
		IDC_STATIC_SUB_STAGE_INFO,
		IDC_STATIC_SUB_STAGE_INFO_1,
		IDC_STATIC_SUB_STAGE_INFO_2,
		IDC_STATIC_SUB_STAGE_INFO_3,
		IDC_STATIC_SUB_STAGE_INFO_4,
		IDC_START_LINE_SET_EDIT,
		IDC_MAIN_LINE_SET_EDIT,
		IDC_END_LINE_SET_EDIT,
		IDC_GO_TYPE_COMBO,

		// Clear Condition
		IDC_STATIC_CLEAR_CONDITION_INFO,
		IDC_CLEAR_CONDITION_LIST,
		IDC_ADD_CLEAR_CONDITION_BUTTON,
		IDC_DEL_CLEAR_CONDITION_BUTTON,
		IDC_CLEAR_CONDITION_COMBO,		

		//////////////////////////////////////////////////////////////////////////		
		IDC_CONDITION_ELEMENT_LIST,
		IDC_CONDITION_ELEMENT_COMBO,
		IDC_ADD_CONDITION_ELEMENT,
		IDC_DEL_CONDITION_ELEMENT,
		//////////////////////////////////////////////////////////////////////////		

		IDC_NEXT_STAGE_LIST,
		IDC_ADD_NEXT_STAGE_BUTTON,
		IDC_DEL_NEXT_STAGE_BUTTON,
		IDC_CLEAR_TYPE_COMBO,
		IDC_STATIC_CLEAR_CONDITION_STAGE,
		IDC_CLEAR_CONDITION_STAGE_COMBO,
		IDC_STATIC_CLEAR_CONDITION_SUBSTAGE,
		IDC_CLEAR_CONDITION_SUBSTAGE_COMBO,
		IDC_STATIC_CLEAR_CONDITION_RATE,
		IDC_CLEAR_CONDITION_RATE_EDIT,

		// MiniMap
		IDC_MAP_STAGE_LIST,
		IDC_MAP_SUB_STAGE_LIST,
		IDC_STATIC_MAP_X,
		IDC_STATIC_MAP_Y,
		IDC_MAP_X_EDIT,
		IDC_MAP_Y_EDIT,		
		IDC_MAP_BOSS_CHECK,
		IDC_IS_NEXT_RANDOM_STAGE_CHECK,
		//IDC_TEXTURE_FILE_NAME_EDIT,
		//IDC_TEXTURE_LOAD_BUTTON,
		IDC_MAP_ALIGN_CHECK,

		// Summary
		IDC_STATIC_SUMMARY,
		IDC_STATIC_COLUMN,
		IDC_SUMMARY_LIST,
		IDC_STATIC_TOTAL_COUNT,
		IDC_NPC_TOTAL_EDIT,
		IDC_EXP_TOTAL_EDIT,
		IDC_ED_TOTAL_EDIT,
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

	struct SUMMARY_INFO
	{
		int				m_iNpcID;
		std::wstring	m_wstrNpcName;
		int				m_iNpcCount;
		int				m_iNpcExp;
		int				m_iNpcED;

		SUMMARY_INFO()
		{
			m_iNpcID	= 0;
			m_iNpcCount	= 0;
			m_iNpcExp	= 0;
			m_iNpcED	= 0;
		}
	};

public:
	KX2DungeonTool(void);
	virtual ~KX2DungeonTool(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

public:
	void InitUI();

	void SetVisibleStageInfo( bool bVal );
	void SetVisibleCurtain( bool bVal );
	void SetVisibleNPC( bool bVal );
	void SetVisibleTrigger( bool bVal );
	void SetVisibleSubStageInfo( bool bVal );
	void SetVisibleClearCondition( bool bVal );
	void SetVisibleCheckBox( bool bVal );

	void RegisterLuaBind();	
	bool LoadDungeonScriptFile();

	void OnMenuUIEventCallBack( UINT nEvent, int nControlID, CDXUTControl* pControl );
	void OnMainUIEventCallback( UINT nEvent, int nControlID, CDXUTControl* pControl );
	void OnToolUIEventCallback( UINT nEvent, int nControlID, CDXUTControl* pControl );
	void OnLeftToolUIEventCallback(  UINT nEvent, int nControlID, CDXUTControl* pControl );
	void OnMapUIEventCallback( UINT nEvent, int nControlID, CDXUTControl* pControl );
	void OnSummaryUIEventCallback( UINT nEvent, int nControlID, CDXUTControl* pControl );

	void RemoveAllItemsToolUI();
	void RemoveCurtainToolUI();
	void RemoveNpcToolUI();
	void RemoveTriggerToolUI();
	void RemoveSubStageInfoToolUI();
	void RemoveClearConditionToolUI();
	void RemoveClearConNextStageToolUI();
	
	bool IsDungeonExist() { return m_pEditingDungeon != NULL; }

	///////////////////////////////////////////////////////////////// Mini Map
	bool PickClickedMap( POINTS mousepos );
	bool MovePickMap( POINTS mousepos );

	/////////////////////////////////////////////////////////// Line HighLight
	bool HighlightPickedSegments();

	////////////////////////////////////////////////////////// NPC 메쉬 띄우기

	void LoadNPCSkinAnims();
	CKTDGXSkinAnim* LoadNPCSkinAnim( CX2DungeonSubStage::NPCData* pNPCData, int iSubIndex );
	CKTDXDeviceXSkinMesh* GetNPCSkinMesh( CX2UnitManager::NPC_UNIT_ID unitID );
	void LoadNPCSkinMesh( CX2UnitManager::NPC_UNIT_ID unitID, const WCHAR* wszSkinMeshFileName );
	void LoadNPCMeshs();	
	void DeleteNPCSkinAnims();
	void DeleteNPCXSkinAnim( CX2DungeonSubStage::NPCData* pNPCData );
	void RenderNPCStartPosIndex( CX2DungeonSubStage::NPCData* pNPCData, int iStageNPCIndex /*= -1*/, int iNPCSubIndex /*= -1*/, bool& bIsSelected );
	void RenderNPCUnitStartPostion();
	KX2DungeonTool::STeamStartPos* GetTeamStartPos( int iNPCStartPosKey );
	HRESULT CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPD3DXMESH* ppMesh, TCHAR* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic, WCHAR* wszText);
	void DeleteAllSkinMeshAndSkinAnim();
	bool PickClickedNPC( POINTS mousepos, bool bClear = true );
	void AddNewStartPosition(const POINTS& mousepos);
	void GetMousePosOnLine( D3DXVECTOR3* pvPosOnLine, const POINTS& mousepos );
	void ComputeRay( D3DXVECTOR3* pvRayOrigin, D3DXVECTOR3* pvRayDirection, const POINTS& mouspos, const D3DXMATRIX* pmatView, const D3DXMATRIX* pmatProj );
	void CollisionCheckNPCStartPos( CX2DungeonSubStage::SubStageData* pSubStageData, D3DXVECTOR3 vRayStart, D3DXVECTOR3 vRayEnd, 
		float fHitDistance, float fShortestHitDistance, CX2DungeonSubStage::NPCData* &pPickedNPCData, int &iPickedNPCStartIndex, CX2DungeonSubStage::SubStageData* &pInNPCSubStage );
	float DistanceBetweenPointAndSegment(float& fLerpCoef, D3DXVECTOR3* pvPoint, D3DXVECTOR3* pvSegmentStart, D3DXVECTOR3* pvSegmentEnd);
	int GetNextUniqueNPCStartPosKey();
	bool NPCRayHit( D3DXVECTOR3 vPos, D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance );

	////////////////////////////////////////////////////// Curtain 메쉬 띄우기
	void RenderCurtain();
	void PickClickedLine( POINTS mousepos, WPARAM wParam );
	int PickASegment( const POINTS& mousepos );

	///////////////////////////////////////////////////// StartPos 메쉬 띄우기
	void RenderStartPos();
	void RenderStartPosIndex( int index, int iLineIndex );

	//////////////////////////////////////////////////////// LineNumber 띄우기
	void RenderLineNumber();

	bool LoadTexture( WCHAR* wszFileName );
	bool LoadDungeon( SEnum::DUNGEON_ID dungeonID );
	bool LoadDungeon( CX2Dungeon* pDungeon );
	bool LoadDungeonFromFile( WCHAR* wszFileName );
	bool LoadStageInfo( int selNum );
	bool LoadStageInfo( CX2DungeonStage::StageData* pStageData );
	bool LoadSubStageInfo( int iStageNum, bool bIsInitSelected = true );
	bool LoadSubStageInfo( CX2DungeonSubStage::SubStageData* pSubStageData, bool bIsInitSelected = true );
	bool LoadCurtainInfo( int iCurtainNum );
	bool LoadCurtainInfo( CX2DungeonSubStage::CurtainData* pCurtainData );
	bool LoadNpcInfo( int iNpcNum );
	bool LoadNpcInfo( CX2DungeonSubStage::NPCData* pNpcData );
	bool LoadTriggerInfo( int iTriggerNum );
	bool LoadTriggerInfo( CX2DungeonSubStage::Trigger* pTrigger );
	bool LoadClearCondition( int iClearConNum );
	bool LoadClearCondition( CX2DungeonSubStage::ClearCondtionData& pClearCon );
	bool LoadClearConNextStage( int iNextStageNum );
	bool LoadClearConNextStage( CX2DungeonSubStage::NextStageData& pNextStageData );
	
	bool LoadConditionElements( CX2DungeonSubStage::CLEAR_CONDITION eClearCon );
	bool LoadSummaryInfo();

	bool SaveDungeonLuaFile( const WCHAR* wszFileName );

	template<typename T>
	void EditingEditBox( KX2DungeonTool::DIALOG_CONTROL_ID id, T& data, T& backUpData );

	template<typename T>
	void EditingEditBox( CDXUTDialog& dialog, KX2DungeonTool::DIALOG_CONTROL_ID id, T& data, T& backUpData );

	template<typename T>
	void EditingEditBoxNonBackup( CDXUTDialog* dialog, KX2DungeonTool::DIALOG_CONTROL_ID id, T& data );

	template<typename T>
	void EditingEditBoxInsert( KX2DungeonTool::DIALOG_CONTROL_ID id, std::vector<T>& data );
	void EditingCheckBox( KX2DungeonTool::DIALOG_CONTROL_ID id, bool& boolData );

	template<typename T>
	void EditingComboBox( KX2DungeonTool::DIALOG_CONTROL_ID id, T& data );

	template<typename T, typename T2>
	void EditingComboBox( CDXUTDialog& dialog, KX2DungeonTool::DIALOG_CONTROL_ID id, T& data, T2& GetValue );
	bool SwapStage( int targetIndex1, int targetIndex2 );	

	//////////////////////////////////////////////////////////////////////////
	bool LoadDungeonToolOption();
	std::wstring& GetAutoSaveFileName() { return m_wsAutoSaveFile; }

    void LoadTriggerFunc(wstring szFileName);

private:

	D3DVIEWPORT9				m_resolution;

	KX2DTDialog					m_menuUI;
	CDXUTDialog					m_mainUI;
	KX2DTDialog					m_toolUI;
	KX2DTDialog					m_LeftToolUI;
	KX2DTDialog					m_mapUI;
	KX2DTDialog					m_summaryUI;

	std::wstring				m_wsSaveFileName;

	KX2EnumManager				m_enumManager;
	KX2DTNpcScaleManager		m_npcScaleManager;
	CX2UnitManager				m_unitManager;
	CX2DungeonManager			m_dungeonManager;
	KX2DTWorldManager			m_worldManager;	
	KX2DungeonToolCamera		m_FPSCamera;
	KDropTable					m_dropTable;
	CX2DTGUNPC					m_gunpcManager;	

	POINT						m_mainUIPoint;
	POINT						m_stageInfoPoint;
	POINT						m_curtainPoint;
	POINT						m_npcViewPoint;
	POINT						m_triggerPoint;
	POINT						m_subStagePoint;
	POINT						m_clearConPoint;
	POINT						m_mapUIPoint;
	POINT						m_summaryPoint;

	CDXUTListBox*				m_pDungeonListBox;
	CDXUTListBox*				m_pStageListBox;

	std::map< std::wstring, SEnum::DUNGEON_ID >	  m_mapDungeonList;
	typedef std::map< std::wstring, SEnum::DUNGEON_ID >::iterator DungeonListIterator;
	std::map< std::wstring, CX2World::WORLD_ID >		  m_mapStageList;
	typedef std::map< std::wstring, CX2World::WORLD_ID >::iterator WorldListIterator;
	std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring > m_mapNpcList;
	typedef std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >::iterator NpcListIterator;
	std::map< CX2World::WORLD_ID, std::wstring >		  m_mapWorldNameList;
	typedef std::map< CX2World::WORLD_ID, std::wstring >::iterator WorldNameListIterator;

	bool						m_bIsConditionFunc;

	////////////////////////////////////////////////////////////////// MiniMap
	std::map< std::pair<int, int>, CX2Dungeon::DungeonMapData > m_mapDungeonMapList;
	typedef std::map< std::pair<int, int>, CX2Dungeon::DungeonMapData >::iterator DungeonMapIterator;
	KX2DTDungeonMapUI*			m_pDungeonMapUI;
	CX2Dungeon::DungeonMapData* m_pEditingMiniMap;
	POINTS						m_beforeMousePos;
	std::wstring				m_wsDungeonMapBG;
	bool						m_bPickMiniMap;

	// 화면에 출력할 월드	
	KX2DTWorld*					m_pWorld; // 할당해제필수!

	/////////////////////////////////////////////////////////// Line HighLight
	std::vector<int>			m_veciPickedSegmentIndex;	
	LPDIRECT3DVERTEXBUFFER		m_pVBHighlightedSegment;
	unsigned					m_nVBHighlightedSegmentSize;
	D3DXCOLOR					m_coHighlight;

	////////////////////////////////////////////////////////// NPC 메쉬 띄우기
	vector<STeamStartPos>		vecTeamStartPos;
	CKTDXDeviceXSkinMesh*		m_pXSkinMesh;	
	vector<CKTDGXSkinAnim*>		vecpXSkinAnim;
	map< std::pair<CX2DungeonSubStage::NPCData*, int>, CKTDGXSkinAnim* > m_mapNPCXSkinAnim;	// npc와 그 npc의 몇번째 시작 위치인가를 조합해서 키로
	map< CX2UnitManager::NPC_UNIT_ID, CKTDXDeviceXSkinMesh* >		m_mapXSkinMesh;
	ID3DXMesh*					m_pMesh3DText[12];
	vector< SNPCStartPos >		m_vecPickedNPCStartPosKey;
	float						m_f3DTextAngle, m_f3DTextAngleStep;
	bool						m_abKeyUsed[1024];
	bool						m_bRenderingNPC;
	bool						m_bMouseDragging;
	bool						m_bNpcColor;
	bool						m_bOnlySubStageNPC;
	int							iTeamColor;
	int							iInitialDir;
	int							m_CurrKeyUsed;
	CKTDGMatrix*				m_pMatrix;

	////////////////////////////////////////////////////// Curtain 메쉬 띄우기
	CKTDXDeviceXMesh*			m_pXCurtainMesh;
	bool						m_bRenderingCurtain;	

	///////////////////////////////////////////////////// StartPos 메쉬 띄우기
	CKTDXDeviceXMesh*			m_pXStartPosMesh;
	bool						m_bRenderingStartPos;
	bool						m_bRenderingLineMapNumber;

	// 실제로 [로드,편집,저장]작업을 진행할 던전 포인터
	CX2Dungeon*							m_pEditingDungeon; // 할당해제필수!
	// 편집을 위한 참조역할의 포인터	
	CX2DungeonStage::StageData*			m_pEditingStage;
	CX2DungeonSubStage::SubStageData*	m_pEditingSubStage;
	CX2DungeonSubStage::CurtainData*	m_pEditingCurtain;
	CX2DungeonSubStage::NPCData*		m_pEditingNpc;
	CX2DungeonSubStage::Trigger*		m_pEditingTrigger;
	CX2DungeonSubStage::ClearCondtionData* m_pEditingClearCon;
	CX2DungeonSubStage::NextStageData*  m_pEditingNextStage;

	// 백업 데이터	
	CX2DungeonSubStage::SubStageData	m_backupSubStage;
	CX2DungeonSubStage::CurtainData		m_backupCurtain;
	CX2DungeonSubStage::NPCData			m_backupNpc;
	CX2DungeonSubStage::Trigger			m_backupTrigger;
	CX2DungeonSubStage::ClearCondtionData m_backupClearCon;
	CX2DungeonSubStage::NextStageData	m_backupNextStage;

	// 임시변수
	int							m_iDelSubStage;
	int							m_iDelReadyNpc;
	int							m_iDelCurtain;
	int							m_iDelNpc;	
	int							m_iDelTrigger;
	int							m_iDelClearCon;
	int							m_iDelNextStage;

	////////////////////////////////////////////////////// 옵션
	// 스테이지 로딩 카운트
	int							m_iStageLoadCount;
	u_int						m_iAutoSaveTime;
	std::wstring				m_wsAutoSavePath;
	std::wstring				m_wsAutoSaveFile;
	std::wstring				m_wsQuickSavePath;
	std::wstring				m_wsQuickSaveFile;
	bool						m_bUIWideMode;

	////////////////////////////////////////////////// 스샷모드
	bool						m_bScreenShotMode;
	bool						m_bUIVisibleBackUp[5];

	///////////////////////////////////////////////// SUMMARY DATA
	std::map< int, SUMMARY_INFO > m_mapSummaryList;
	typedef std::map< int, SUMMARY_INFO >::iterator SummaryIterator;

	//////////////////////////////////////////////// NPC 시야	
	CKTDXDeviceXMesh*			m_pXMeshSphere;		
	CKTDGMatrix*				m_pMatrixSphere;
	bool						m_bRenderingNearRange;
	bool						m_bRenderingTargetRange;
	bool						m_bRenderingLostRange;
	bool						m_bOnlySelect;

    // 트리거 함수 저장용 변수
    wstring m_strFunc;
    bool m_bTriggerFunc;
};
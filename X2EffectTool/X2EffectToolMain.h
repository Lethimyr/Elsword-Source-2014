	

// 툴의 이펙트셋에서 지원하지 않는 일부 동작에 대한 UI 임시 제거.
//#define EXCLUDE_EFFECT_SET_FUNCTION

#ifndef BANANA_EXCEPTION
	// 이펙트툴 실행 전 스크립트 업데이트
	#define PRE_UPDATE_SCRIPT
	#define CHECK_VERSION
	#define SAVE_AND_SVN_UPDATE
#endif // BANANA_EXCEPTION

#define CREATE_EFFECT_SET_BY_DAMAGE_EFFECT

#pragma once
/********************************************************************
	created:	2006/12/04
	created:	4:12:2006   15:08
	filename: 	d:\projectX2\X2Project\X2EffectTool\X2EffectToolMain.h
	file path:	d:\projectX2\X2Project\X2EffectTool
	file base:	X2EffectToolMain
	file ext:	h
	author:		
	
	purpose:	
*********************************************************************/

class CX2EffectToolFileManager;
class CEffectToolGame;
class CWorldToolObject;
class CSkinAnimObject;
class CSkinAnimObject;
class CX2EffectToolParticleEditor;
class CX2EffectToolMeshEditor;

class CEffectToolCamera : public CFirstPersonCamera
{
public:
	CEffectToolCamera():m_fMoveSpeed(1000.f){};
	~CEffectToolCamera() {};
public:
	float		m_fMoveSpeed;
};

class CEffectToolDialog : public CDXUTDialog
{
public:
	CEffectToolDialog():m_bEnable(true){}
	~CEffectToolDialog() {};

	void SetEnable( bool bEnable ) { m_bEnable = bEnable; }
	const bool GetEnable() { return m_bEnable; }

private:
	bool	m_bEnable;
};

class CX2EffectToolMain : public CX2Main
{
public:
	class CCollision
	{
	public:
		static float PointToSegment( float& fLerpCoef, const D3DXVECTOR3* pvPoint, const D3DXVECTOR3* pvSegmentStart, const D3DXVECTOR3* pvSegmentEnd )
		{
			D3DXVECTOR3 vSegmentDir = *pvSegmentEnd - *pvSegmentStart;
			D3DXVECTOR3 vSegmentStartToPoint = *pvPoint - *pvSegmentStart;

			fLerpCoef = D3DXVec3Dot( &vSegmentDir, &vSegmentStartToPoint ) / D3DXVec3Dot( &vSegmentDir, &vSegmentDir );

			if( fLerpCoef < 0.f )
			{
				float fDistance = D3DXVec3Length( &(*pvSegmentStart - *pvPoint) );
				return fDistance;
			}
			else if( fLerpCoef > 1.f )
			{
				float fDistance = D3DXVec3Length( &(*pvSegmentEnd - *pvPoint) );
				return fDistance;
			}
			else
			{
				D3DXVECTOR3 vPointOnSegment = *pvSegmentStart + vSegmentDir * fLerpCoef;
				float fDistance = D3DXVec3Length( &(vPointOnSegment - *pvPoint) );
				return fDistance;
			}
		}


		static bool RayToPlane( float& fDistance, D3DXVECTOR3& vIntersection, 
			const D3DXVECTOR3& vRayOrigin, const D3DXVECTOR3& vRayDirection, 
			const D3DXVECTOR3& vPlaneVertex, const D3DXVECTOR3& vPlaneNormal )
		{
			// ray: P = P0 + t*V
			// plane: P.N + d = 0
			float VdotN = D3DXVec3Dot( &vRayDirection, &vPlaneNormal );
			float P0dotN = D3DXVec3Dot( &vRayOrigin, &vPlaneNormal );
			float d = -D3DXVec3Dot( &vPlaneVertex, &vPlaneNormal );
			if( VdotN == 0.f )
				return false;

			float t = -(P0dotN + d) / VdotN;
			fDistance = t;

			if( t < 0.f ) // no intersection
			{				
				return false;
			}
			else
			{				
				vIntersection = vRayOrigin + vRayDirection * t;
				return true;
			}
		}

		static bool RayToRect( float& fDistance, D3DXVECTOR3& vIntersection, 
			const D3DXVECTOR3& vRayOrigin, const D3DXVECTOR3& vRayDirection, 
			const D3DXVECTOR3& vRectVertex1, const D3DXVECTOR3& vRectVertex2, const D3DXVECTOR3& vRectNormal )
		{
			bool bResult = RayToPlane( fDistance, vIntersection, vRayOrigin, vRayDirection, vRectVertex1, vRectNormal);
			if( bResult == true )
			{
				D3DXVECTOR3 vTemp1 = vIntersection - vRectVertex1;
				D3DXVECTOR3 vTemp2 = vIntersection - vRectVertex2;
				float fIsInside = D3DXVec3Dot( &vTemp1, &vTemp2 );
				if( fIsInside < 0.f )
				{
					return true; 					
				}
			}
			return false;
		}		


		/*
		http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline3d/
		Calculate the line segment PaPb that is the shortest route between
		two lines P1P2 and P3P4. Calculate also the values of mua and mub where
		Pa = P1 + mua (P2 - P1)
		Pb = P3 + mub (P4 - P3)
		Return FALSE if no solution exists.
		*/
		static bool LineToLine( D3DXVECTOR3& pa, D3DXVECTOR3& pb, 
			float& mua, float& mub,
			const D3DXVECTOR3& p1, const D3DXVECTOR3& p2,
			const D3DXVECTOR3& p3, const D3DXVECTOR3& p4,
			const float EPS = 0.000001f )
		{
			D3DXVECTOR3 p13, p43, p21;	
			float d1343,d4321,d1321,d4343,d2121; // dot products
			float numer,denom;

			p43 = p4 - p3;			
			if( fabs(p43.x) < EPS && fabs(p43.y) < EPS && fabs(p43.z) < EPS )
				return(false);			
			p21 = p2 - p1;
			if( fabs(p21.x) < EPS && fabs(p21.y) < EPS && fabs(p21.z) < EPS)
				return(false);
			p13 = p1 - p3;

			d1343 = D3DXVec3Dot(&p13, &p43);
			d4321 = D3DXVec3Dot(&p43, &p21);
			d1321 = D3DXVec3Dot(&p13, &p21);
			d4343 = D3DXVec3Dot(&p43, &p43);
			d2121 = D3DXVec3Dot(&p21, &p21);

			denom = d2121 * d4343 - d4321 * d4321;
			if( fabs(denom) < EPS)
				return(false);
			numer = d1343 * d4321 - d1321 * d4343;

			mua = numer / denom;
			mub = (d1343 + d4321 * mua) / d4343;

			pa = p1 + p21 * mua;
			pb = p3 + p43 * mub;

			return(true);
		}
	};
public:
	enum LINEMAP_FILENAME_TYPE
	{
		LFT_PREFIX,
		LFT_SUFFIX,
	};

	enum TOOL_MODE
	{
		MODE_GAMEPLAY,
		//////////////////////
		MODE_EFFECT_SET,
		MODE_EFFECT_SET_PARTICLE_MESH_EDIT,
		MODE_EFFECT_SET_CAMERA_EDIT,
		MODE_EFFECT_SET_PARTICLE_ADD,
		MODE_EFFECT_SET_MESH_ADD,
	};	

	enum DIALOG_ID
	{
		DLG_OBJECT_PROPERTY,		// 이전 툴
		///////////////////////
		DLG_EFFECT_SET_LIST,
		DLG_EFFECT_SET_MEMBER,			//파티클, 메쉬, 카메라
		DLG_EFFECT_SET_PARTICLE_MESH_EDIT,
		DLG_EFFECT_SET_PARTICLE_MESH_EDIT2,
		DLG_EFFECT_SET_CAMERA_EDIT,

		DLG_PARTICLE_LIST,
		DLG_PARTICLE_LIST_FIND_TEXTURE,	 //텍스쳐 이름으로 파티클 검색

		DLG_MESH_LIST,
		DLG_END,
	};

	enum DIALOG_CONTROL_ID
	{
		IDC_STATIC_DUMMY,
		IDC_BUTTON_HELP,				// 지워


		IDC_BUTTON_RELOAD_OBJECT_IN_WORLD,



		IDC_EDIT_POS_X,				// object 위치
		IDC_EDIT_POS_Y,
		IDC_EDIT_POS_Z,
		IDC_EDIT_SCALE_X,
		IDC_EDIT_SCALE_Y,
		IDC_EDIT_SCALE_Z,
		IDC_EDIT_ROTATION_DEGREE,

		IDC_EDIT_OCCATIONAL_EVENT_TIME,
		IDC_EDIT_OCCATIONAL_EVENT_RATE,

		IDC_EDIT_ROTATION_SPEED,
		IDC_EDIT_TEX0_UV_SPEED,
		IDC_EDIT_SKIN_ANIM_NAME,

#ifdef ADD_ALPHATESTENABLE
		IDC_CHECK_IS_NOT_CULL,
		IDC_CHECK_IS_ALPHATEST_ENABLE,
#endif
		IDC_CHECK_ADDITIVE_BLEND,        
		IDC_CHECK_IS_ALPHA_OBJECT,
		IDC_CHECK_CAN_HIDE,
		IDC_CHECK_NOT_RENDER,
		IDC_CHECK_USE_WORLD_COLOR,

		IDC_COMBO_SKIN_ANIM_PLAY_TYPE,
		IDC_COMBO_LAYER,
		IDC_COMBO_BILLBOARD,
		IDC_COMBO_OBJECT_TYPE,
		
		IDC_EDIT_OBJECT_LIGHT_COLOR,
		IDC_SLIDER_OBJECT_LIGHT_COLOR_RED,
		IDC_SLIDER_OBJECT_LIGHT_COLOR_GREEN,
		IDC_SLIDER_OBJECT_LIGHT_COLOR_BLUE,


        // FOG
        IDC_CHECK_IS_FOG,
        IDC_CHECK_IS_FOGWORLD,
        IDC_EDIT_FOG_NEAR_X,
        IDC_EDIT_FOG_FAR_X,
        IDC_EDIT_FOG_NEAR_Y,
        IDC_EDIT_FOG_FAR_Y,
        IDC_EDIT_FOG_DENSITY,
        IDC_EDIT_FOG_DENSITY_OBJECT,
        IDC_EDIT_ORIGIN_FOG_COLOR,
        IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_RED,
        IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_GREEN,
        IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_BLUE,
        IDC_SLIDER_FOG_NEAR_X,
        IDC_SLIDER_FOG_FAR_X,
        IDC_SLIDER_FOG_NEAR_Y,
        IDC_SLIDER_FOG_FAR_Y,
        IDC_SLIDER_FOG_DENSITY,
        IDC_SLIDER_FOG_DENSITY_OBJECT,

		// 이펙트 툴
		IDC_BUTTON_LOAD_EFFECT_SET,
		IDC_BUTTON_SAVE_EFFECT_SET,
		
		IDC_BUTTON_ADD_DAMAGE_EFFECT,	// "DamageEffect.lua" 생성

		IDC_BUTTON_LOAD_PARTICLE,

		


		IDC_LIST_EFFECTSET_PALETTE,
		IDC_LIST_EFFECTSET_LOADED,
		
		IDC_EDIT_HIDE_DISTANCE,// 09.03.02 김태완 : 일정거리 이상에서 object 숨기는 기능
		
		IDC_CHECK_FORCE_SHOW,	// 09.03.02 김태완 : ForceShow 기능 ( 신마을 )
		

		//DLG_EFFECT_SET_LIST
		IDC_BUTTON_LOAD_EFFECT_SET_WHOLE,	// "Effectset.lua" 파싱
		IDC_BUTTON_SAVE_EFFECT_SET_WHOLE,	// "Effectset.lua" 생성
		IDC_BUTTON_LOAD_DAMAGE_EFFECT,	// "Damage.lua" 파싱

		IDC_BUTTON_FILE_OPEN_WORLD,			// 월드 툴 열기
		IDC_LIST_EFFECT_SET,				// 이펙트셋 목록
		IDC_EDIT_EFFECT_SET_NAME_CHANGE,	// 이펙트셋 이름 변경
		IDC_EDIT_EFFECT_SET_ADD,			// 이펙트셋 추가 스트링.
		IDC_BUTTON_EFFECT_SET_NAME_CHANGE,	// 이펙트셋 이름 변경 버튼
		IDC_BUTTON_EFFECT_SET_DEL,			// 이펙트셋 제거 버튼
		IDC_BUTTON_EFFECT_SET_ADD,			// 이펙트셋 추가
		IDC_BUTTON_EFFECT_SET_MERGE,		// 선택한 이펙트셋 다른 이펙트셋에 통합
		IDC_LIST_SKIN_ANIM_NAME,
		IDC_BUTTON_FILE_OPEN_NPC_SKINANIM,
		IDC_EDIT_EFFECT_SET_SAVE_AS,		// 이펙트셋 다른이름으로 저장
		IDC_BUTTON_EFFECT_SET_SAVE_AS,		// 이펙트셋 다른이름으로 저장

		IDC_EDIT_EFFECT_SET_FIND,			// 이펙트셋 검색.
		IDC_BUTTON_EFFECT_SET_FIND,	

		IDC_EDIT_EFFECT_SET_DESC,			// 주석
		IDC_EDIT_EFFECT_SET_DESC_FIND,		// 주석 검색
		IDC_BUTTON_EFFECT_SET_DESC_FIND,	
		//DLG_EFFECT_SET_LIST

		//DLG_EFFECT_SET_MEMBER
		IDC_COMBO_EFFECT_SET_TYPE,
		IDC_CHECK_EFFECT_SET_AFFECTED_BY_UNIT_SCALE,		
		IDC_EDIT_EFFECT_SET_DELETE_SHAKE_COUNT,

		IDC_LIST_EFFECT_SET_PARTICLE,		/// 파티클 목록
		IDC_LIST_EFFECT_SET_MESH,			/// 메쉬 목록
		IDC_LIST_EFFECT_SET_CAMERA_SHAKE,	/// 카메라 쉐이크 목록

		IDC_BUTTON_EFFECT_SET_PARTICLE_ADD,	/// 파티클 추가
		IDC_BUTTON_EFFECT_SET_PARTICLE_DEL,	/// 파티클 제거
		IDC_BUTTON_EFFECT_SET_MESH_ADD,		/// 메쉬 추가
		IDC_BUTTON_EFFECT_SET_MESH_DEL,		/// 메쉬 제거
		IDC_BUTTON_EFFECT_SET_CAMERA_ADD,	/// 카메라 쉐이크 추가
		IDC_BUTTON_EFFECT_SET_CAMERA_DEL,	/// 카메라 쉐이크 제거

		IDC_BUTTON_EFFECT_SET_PARTICLE,		/// 파티클 세부설정 UI 탭
		IDC_BUTTON_EFFECT_SET_CAMERA,		/// 카메라 세부설정 UI 탭
		IDC_BUTTON_EFFECT_SET_HIDE,			/// UI숨기기 탭
		IDC_BUTTON_EFFECT_SET_LIST_SHOW_TOGGLE, 

		IDC_EDIT_EFFECT_SET_CUSTOM_PARTICLE_ADD,	/// 이펙트셋 멤버 파티클을 수기로 추가
		IDC_BUTTON_EFFECT_SET_CUSTOM_PARTICLE_ADD,	

		IDC_EDIT_EFFECT_SET_CUSTOM_MESH_ADD,	/// 이펙트셋 멤버 파티클을 수기로 추가
		IDC_BUTTON_EFFECT_SET_CUSTOM_MESH_ADD,	

		IDC_BUTTON_CREATE_EFFECT_SET_BY_DAMAGE_EFFECT,
		//DLG_EFFECT_SET_MEMBER

		//DLG_EFFECT_SET_PARTICLE_MESH_EDIT
		IDC_EDIT_EFFECT_SET_START_TIME,
		IDC_EDIT_EFFECT_SET_BONE_NAME,
		IDC_EDIT_EFFECT_SET_RIDING_BONE_NAME,
		IDC_EDIT_EFFECT_SET_POSITION_OFFSET_X,
		IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Y,
		IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Z,
		IDC_CHECK_EFFECT_SET_TRACE,
		IDC_CHECK_EFFECT_SET_TRACE_MATRIX,
		IDC_CHECK_EFFECT_SET_TRACE_MORE,
		IDC_CHECK_EFFECT_SET_REVERSE_Y,
		IDC_CHECK_EFFECT_SET_USE_LAND_POSITION,
		IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION,
		IDC_CHECK_EFFECT_SET_APPLY_INSTANCE_SCALE,
		IDC_CHECK_EFFECT_SET_START_WHEN_TRACE_TARGET_DEAD,
		IDC_CHECK_EFFECT_SET_PASSIVE_EFFECT,
		IDC_CHECK_EFFECT_SET_USE_SUB_ATTACK_LIST_SET,
		IDC_CHECK_EFFECT_SET_BONE_TRACE_TARGET_MESH,
		IDC_EDIT_EFFECT_SET_HYPER_NAME,
		IDC_EDIT_EFFECT_SET_WEAPON_BONE_NAME,
		IDC_EDIT_EFFECT_SET_WEAPON_BONE_INDEX,
		IDC_EDIT_EFFECT_SET_WEAPON_BONE_MODEL_INDEX,
		IDC_EDIT_EFFECT_LIMIT_DISTANCE, //파티클, 메시 플레이어의 생성 거리 제한
		IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION_MIRROR,
		//DLG_EFFECT_SET_PARTICLE_MESH_EDIT

		//DLG_EFFECT_SET_PARTICLE_MESH_EDIT2
		IDC_COMBO_EFFECT_SET_EFFECT_TYPE,
		IDC_EDIT_EFFECT_SET_ROTATE_X,
		IDC_EDIT_EFFECT_SET_ROTATE_Y,
		IDC_EDIT_EFFECT_SET_ROTATE_Z,
		IDC_EDIT_EFFECT_SET_SCALE_X,
		IDC_EDIT_EFFECT_SET_SCALE_Y,
		IDC_EDIT_EFFECT_SET_SCALE_Z,
		IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_X,
		IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Y,
		IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Z,
		IDC_LIST_MOTION_BONE_LIST,
		//DLG_EFFECT_SET_PARTICLE_MESH_EDIT2

		//DLG_EFFECT_SET_CAMERA_EDIT
		IDC_COMBO_EFFECT_SET_CAMERA_SHAKE_TYPE,
		IDC_CHECK_EFFECT_SET_FIXED_LOOCK_VEC,
		IDC_EDIT_EFFECT_SET_CAMERA_START_TIME,
		IDC_EDIT_EFFECT_SET_LIMIT_DISTANCE,
		IDC_EDIT_EFFECT_SET_ONE_DIR_SPEED,
		IDC_EDIT_EFFECT_SET_ONE_DIR_ACCEL,
		IDC_EDIT_EFFECT_SET_TWO_DIR_GAP,
		IDC_EDIT_EFFECT_SET_TWO_DIR_TIME,
		IDC_EDIT_EFFECT_SET_TWO_DIR_TIME_GAP,
		IDC_EDIT_EFFECT_SET_EYE_SHAKE_COUNT,
		IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MIN,
		IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MAX,
		IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MIN,
		IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MAX,
		IDC_EDIT_EFFECT_SET_LOOK_AT_SHAKE_COUNT,
		IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MIN,
		IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MAX,
		IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MIX,
		IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MAX,
		//DLG_EFFECT_SET_CAMERA_EDIT

		//DLG_PARTICLE_LIST
		IDC_COMBO_PARTICLE_FILE,
		IDC_BUTTON_LOAD_BASE_PARTICLE_FILE,
		IDC_BUTTON_LOAD_PARTICLE_FILE,
		IDC_BUTTON_INIT_PARTICLE_FILE,
		IDC_BUTTON_SAVE_PARTICLE_FILE,
		IDC_BUTTON_PARTICLE_BASE_EDITOR,
		IDC_BUTTON_PARTICLE_ETC_EDITOR,
		IDC_LIST_PARTICLE_LIST,
		IDC_BUTTON_DEL_PARTICLE,
		IDC_EDIT_PARTICLE_LIST_COUNT,		
		
		IDC_EDIT_PARTICLE_LIST_FIND,			// 파티클 리스트에서 검색.
		IDC_BUTTON_PARTICLE_LIST_FIND,	

		IDC_BUTTON_PARTICLE_ADD_OTHER_FILE,		// 테스트 파티클 파일-> 여유있는 다른 파티클 파일로 이동
		//DLG_PARTICLE_LIST

		//DLG_PARTICLE_LIST_FIND_TEXTURE
		IDC_LIST_PARTICLE_LIST_FIND_TEXTURE,
		IDC_EDIT_FIND_TEXTURE_NAME,
		IDC_BUTTON_FIND_TEXTURE,
		//DLG_PARTICLE_LIST_FIND_TEXTURE

		//DLG_MESH_LIST
		IDC_BUTTON_LOAD_BASE_MESH_FILE,
		IDC_BUTTON_LOAD_MESH_FILE,
		IDC_BUTTON_SAVE_MESH_FILE,
		IDC_LIST_MESH_LIST,
		IDC_BUTTON_MESH_BASE_EDITOR,
		IDC_BUTTON_DEL_MESH,

		IDC_EDIT_MESH_LIST_FIND,				// 메쉬 리스트에서 검색.
		IDC_BUTTON_MESH_LIST_FIND,	
		//DLG_MESH_LIST
	};

	struct TimedXMeshPlayer
	{
		float			m_fDelayTimeToStart;
		D3DXVECTOR3		m_vRelativePosition;
		bool			m_bRelativeToUnit;
		wstring			m_wstrXMeshPlayerName;
		bool			m_bTraceUnit;
		wstring			m_wstrBoneName;
		CKTDGXMeshPlayer::CXMeshInstance* m_pInstance;

		TimedXMeshPlayer()
		{
			m_fDelayTimeToStart		= 0.f;
			m_vRelativePosition		= D3DXVECTOR3(0,0,0);
			m_wstrXMeshPlayerName	= L"";
			m_bTraceUnit			= false;
			m_wstrBoneName			= L"";
			m_pInstance				= NULL;
			m_bRelativeToUnit		= true;
		}
	};

	struct PreviewXMeshPlayer
	{
		bool			m_bIsCreate;
		wstring			m_wstrXMeshPlayerName;
		CKTDGXMeshPlayer::CXMeshInstance* m_pInstance;

		bool	IsCreate(){ return m_bIsCreate; }
		void	SetIsCreate( bool bIsCreate ){ m_bIsCreate = bIsCreate; }

		void	SetXMeshName( WCHAR* wstrXMeshName ){ m_wstrXMeshPlayerName = wstrXMeshName; }
		void	SetXMeshName( const WCHAR* wstrXMeshName ){ m_wstrXMeshPlayerName = wstrXMeshName; }
		const wstring& GetXMeshName(){ return m_wstrXMeshPlayerName; }

		CKTDGXMeshPlayer::CXMeshInstance* GetXMeshInstPointer(){ return m_pInstance; }
		void	SetXMeshInstPointer( CKTDGXMeshPlayer::CXMeshInstance* pXMeshInst ){ m_pInstance = pXMeshInst; }
		void	SetNULLPointerbyXMeshInst(){ m_pInstance = NULL; }

		PreviewXMeshPlayer()
		{
			m_bIsCreate = false;
			m_pInstance = NULL;
		}

	};

	struct TimedParticle
	{
		float			m_fDelayTimeToStart;
		D3DXVECTOR3		m_vRelativePosition;
		wstring			m_wstrParticleName;
		bool			m_bTraceUnit;
		wstring			m_wstrBoneName;
		CKTDGParticleSystem::CParticleEventSequence* m_pEventSequence; 

		TimedParticle()
		{
			m_fDelayTimeToStart		= 0.f;
			m_vRelativePosition		= D3DXVECTOR3(0,0,0);
			m_wstrParticleName		= L"";
			m_bTraceUnit			= false;
			m_wstrBoneName			= L"";
			m_pEventSequence		= NULL;
		}
	};

	struct	PreviewParticle
	{
		bool			m_bIsCreate;
		wstring			m_wstrParticleName;
		CKTDGParticleSystem::CParticleEventSequence* m_pEventSequence; 

		bool	IsCreate(){ return m_bIsCreate; }
		void	SetIsCreate( bool bIsCreate ){ m_bIsCreate = bIsCreate; }

		void	SetParticleName( WCHAR* wstrParticleName ){ m_wstrParticleName = wstrParticleName; }
		void	SetParticleName( const WCHAR* wstrParticleName ){ m_wstrParticleName = wstrParticleName; }
		const wstring& GetParticleName(){ return m_wstrParticleName; }

		CKTDGParticleSystem::CParticleEventSequence* GetEventSequencePointer(){ return m_pEventSequence; }
		void	SetEventSequencePointer( CKTDGParticleSystem::CParticleEventSequence* pEventSequence ){ m_pEventSequence = pEventSequence; }
		void	SetNULLPointerbyEventSequence(){ m_pEventSequence = NULL; }

		PreviewParticle()
		{
			m_bIsCreate = false;
			m_pEventSequence = NULL;
		}
	};

	struct CameraShake
	{
		float m_fStartTime;
		float m_fAmplitude;
		float m_fDuration;

		CameraShake()
		{
			m_fStartTime = -1.f;
			m_fAmplitude = 0.f;
			m_fDuration = 0.f;
		}
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
	CX2EffectToolMain(void);
	virtual ~CX2EffectToolMain(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();
	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	
	void LoadWorld( const WCHAR* pwszFileName );
	void PostProcLoadWorld();
	void LoadLineMap( const WCHAR* pwszFileName );

	void AddObjectToWorld( const WCHAR* wszFileName, CX2WorldObject::OBJECT_TYPE eObjType = CX2WorldObject::OT_STATIC_MESH );

    void InitFog();

	void ChangeUserUnit( CX2Unit::UNIT_CLASS eUnitClass );
	CX2Unit::UNIT_CLASS& GetCurrUnitClass() { return m_CurrUnitClass; }

    D3DXCOLOR GetFogColor()
    {
        return m_fogColor;
    }

    void SetFogColor( float rColor, float gColor, float bColor )
    {
        m_fogColor = D3DXCOLOR(rColor, gColor, bColor, 1.0f);
    }

	void UpdateWorldToolObjectList();

protected:
	void DestroyPreViewInstance();
	void OnFrameMove_Mesh( double fTime, float fElapsedTime );
	void OnFrameMove_CameraShake();


	void ResetDialogAtModeChange();
	void UpdateDialog();
	void FinalDialog();
	void InitDialog();
	//{{InitDialog 함수 분리
	void InitDialogObjectProperty();	//DLG_OBJECT_PROPERTY

	void InitDialogEffectSet_List();			/// MODE_EFFECT_SET
	void InitDialogEffectSet_Member();			/// 
	void InitDialogEffectSet_ParticleMeshEdit();/// MODE_EFFECT_SET_PARTICLE_MESH_EDIT
	void InitDialogEffectSet_ParticleMeshEdit2();/// MODE_EFFECT_SET_PARTICLE_MESH_EDIT
	void InitDialogEffectSet_CameraEdit();		/// MODE_EFFECT_SET_HIDE_SETTING_UI

	void InitDialogParticle_List();				// DLG_PARTICLE_LIST
	void InitDialogParticle_List_Find_Texture();// DLG_PARTICLE_LIST_FIND_TEXTURE
	void InitDialogMesh_List();					// DLG_MESH_LIST
	//}}InitDialog 함수 분리

	void InitX2Data();


	bool DropFileProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	void CreateGame();
	void DisableGameKey();

	void RenderBoundingSphere( D3DXVECTOR3& vCenter, float fRadius );



public:
    bool DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath );

	CEffectToolCamera& GetCamera() { return m_FPSCamera; }
	vector<CWorldToolObject*>& GetPickedWorldToolObjectList() { return m_vecpPickedWorldToolObject; }
    vector<CWorldToolObject*>& GetWorldToolObjectList() { return m_vecpWorldToolObject; }
	CEffectToolDialog* GetDialog( int index ) 
	{ 
		if( static_cast<int>(m_vecpDialog.size()) <= index )
			return NULL;
		else
			return m_vecpDialog[index]; 
	}

	void ResetXMeshPlayer()
	{
		if( m_pXMeshPlayer != NULL )
		{
			SAFE_DELETE( m_pXMeshPlayer );
		}
		m_pXMeshPlayer = new CKTDGXMeshPlayer;
	}

	CKTDGXMeshPlayer* GetXMeshPlayer() { return m_pXMeshPlayer; }

#ifndef EFFECT_TOOL
	void ResetUserUnitStateListBox();
	void ClearUserUnitStateListBox();
#endif //EFFECT_TOOL
	HRESULT SaveEffectSet( const WCHAR* pwszFileFullPath );	//사용하지 않는 함수.
	HRESULT LoadEffectSet( const WCHAR* pwszFileFullPath );

	void CreateCharacterSkinMesh( const WCHAR* wszCharacterName );
	void CreateCharacterSkinMeshFromFile( const WCHAR* wszFileName );

	void On_IDC_BUTTON_SAVE_XMESH_PLAYER( const WCHAR* pwszFileFullPath );

	void On_IDC_BUTTON_LOAD_EFFECT_SET_WHOLE();
	void On_IDC_BUTTON_SAVE_EFFECT_SET_WHOLE( bool bHotKey_ = false );

	void On_IDC_BUTTON_LOAD_DAMAGE_EFFECT();

	//void On_IDC_BUTTON_SAVE_EFFECT_SET( const WCHAR* pwszFileFullPath );
		
	void On_IDC_BUTTON_ADD_DAMAGE_EFFECT( const WCHAR* pwszFileFullPath );

	void ResetDamageEffect();
	bool IsValidDamageEffectByName( const WCHAR* wcEffectName_ );

	D3DXCOLOR GetFadeWorldColor() const { return m_coFadeWorldColor; }
	void SetFadeWorldColor(D3DXCOLOR val) { m_coFadeWorldColor = val; }

	void ParseIntArrayFromEditBox( CDXUTEditBox* pEditBox, int *pNumbers, int nCount );
	void SetTextIntArrayToEditBox( CDXUTEditBox* pEditBox, const int *pNumbers, int nCount );

	void ReadyEffectDevices();

	void UpdateMonsterList();

	void LoadLineMapInfo();
	void LineMapFloatToInt();
	void RecomputeStartPosLineIndex();
	float ComputeLandHeight();
	void SaveLineMapToText();
	bool SaveLineMapToFile( const WCHAR* wszFileName, bool bUpdateFileName );
	
	bool GetMousePosOnLine( D3DXVECTOR3* pvPosOnLine, const POINTS& mousepos );
	void AddNewStartPosition(const POINTS& mousepos);
	bool PickClickedNPC( int iDungeonLevel, POINTS mousepos, bool bClear = true );
	float DistanceBetweenPointAndSegment(float& fLerpCoef, D3DXVECTOR3* pvPoint, D3DXVECTOR3* pvSegmentStart, D3DXVECTOR3* pvSegmentEnd);

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

	void ResetNpcStartPosKey()
	{
		m_CurrKeyUsed = 0;
		for( int i=0; i<sizeof(m_abKeyUsed); ++i)
		{
			m_abKeyUsed[i] = false;
		}
		for( int i = 0; i < int(vecTeamStartPos.size()); ++i)
		{
			if( vecTeamStartPos[i].iTeamColor == 0 )
			{
				int key = GetNextUniqueNPCStartPosKey();
				if( key == -1 )
					continue;
				vecTeamStartPos.at(i).key = key;
			}
		}
	}

	void DeleteNpcStartPos()
	{
		m_CurrKeyUsed = 0;
		for( int i=0; i<sizeof(m_abKeyUsed); ++i)
		{
			m_abKeyUsed[i] = false;
		}
		for( int i=vecTeamStartPos.size() - 1; i>=0; --i)
		{
			if( vecTeamStartPos[i].iTeamColor == 0 )
			{
				vecTeamStartPos.erase(vecTeamStartPos.begin() + i);
			}
		}

		for( unsigned i=0; i < vecpXSkinAnim.size(); i++ )
		{
			SAFE_DELETE_KTDGOBJECT( (CKTDGXSkinAnim*)vecpXSkinAnim[i] );
		}
		vecpXSkinAnim.clear();
	}
	
	STeamStartPos& GetStartPos(int iIndex) { return (STeamStartPos&)vecTeamStartPos[iIndex]; }
	
	int& GetDir() { return iInitialDir; }
	int& GetTeamColor() { return iTeamColor; }
	int& GetShowDungeonLevel() { return m_iShowDungeonLevel; }
	void GetLineIndexInRect( float fLeft, float fTop, float fRight, float fBottom );
	
	
	//void ToggleNavigate() { bNavigate = !bNavigate; }
	int& GetPickedStartPosIndex() { return m_iPickedStartPosIndex; }	
	//vector<int>& GetPickedSegmentVector() { return m_veciPickedSegmentIndex; }
	void ComputeRay( D3DXVECTOR3* pvRayOrigin, D3DXVECTOR3* pvRayDirection, const POINTS& mouspos, const D3DXMATRIX* pmatView, const D3DXMATRIX* pmatProj );
	void CollisionCheckNPCStartPos( CX2DungeonSubStage::SubStageData* pSubStageData, D3DXVECTOR3 vRayStart, D3DXVECTOR3 vRayEnd, 
		float fHitDistance, float fShortestHitDistance, CX2DungeonSubStage::NPCData* &pPickedNPCData, 
		int &iPickedNPCStartIndex );
	bool NPCRayHit( D3DXVECTOR3 vPos, D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance );
	void DeleteAllSkinMeshAndSkinAnim();
	void OnFrameRenderUnit();
	void RenderUserUnitStartPos();
	void RenderNPCUnitStartPostion( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficulty );
	void RenderNPCStartPosIndex( CX2DungeonSubStage::NPCData* pNPCData, int iStageNPCIndex = -1, int iNPCSubIndex = -1 );
	HRESULT CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPD3DXMESH* ppMesh, TCHAR* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic, WCHAR* wszText);
	STeamStartPos *GetTeamStartPos( int iNPCStartPosKey );


///////////////////////////////////////////////////////////////////
///EFFECT_TOOL
	CX2EffectToolFileManager* GetFileManager(){ return m_pFileManager; }
	void ToggleGameMode();

	

	void On_IDC_BUTTON_EFFECT_SET_DESC_FIND( const WCHAR* strText );
	void On_IDC_BUTTON_EFFECT_SET_NAME_CHANGE( const WCHAR* strText );
	void On_IDC_BUTTON_EFFECT_SET_FIND( const WCHAR* strText );
	void On_IDC_BUTTON_EFFECT_SET_DEL( );
	void On_IDC_BUTTON_EFFECT_SET_ADD( const WCHAR* strText );
	void On_IDC_BUTTON_EFFECT_SET_MERGE( const WCHAR* strText );
	void On_IDC_BUTTON_EFFECT_SET_SAVE_AS( const WCHAR* strText );

	//OnDialogEvent_DLG_EFFECT_SET_MEMBER
	void On_IDC_COMBO_EFFECT_SET_TYPE( CX2EffectSet::EFFECT_SET_TIMER_TYPE eEffectType_ );
	void On_IDC_CHECK_EFFECT_SET_AFFECTED_BY_UNIT_SCALE( bool bVal_ );
	void On_IDC_EDIT_EFFECT_SET_DELETE_SHAKE_COUNT( int iValue_ );
	void On_IDC_BUTTON_EFFECT_SET_CAMERA_ADD();		/// 카메라 쉐이크 추가,제거
	void On_IDC_BUTTON_EFFECT_SET_CAMERA_DEL();
	void On_IDC_BUTTON_EFFECT_SET_PARTICLE_ADD();	/// 파티클 추가,제거
	void On_IDC_BUTTON_EFFECT_SET_PARTICLE_DEL();
	void On_IDC_BUTTON_EFFECT_SET_MESH_ADD();		/// 메쉬 추가,제거
	void On_IDC_BUTTON_CREATE_EFFECT_SET_BY_DAMAGE_EFFECT();
	void On_IDC_BUTTON_EFFECT_SET_MESH_DEL();
	void On_IDC_BUTTON_EFFECT_SET_CUSTOM_PARTICLE_ADD();
	void On_IDC_BUTTON_EFFECT_SET_CUSTOM_MESH_ADD();
	void On_IDC_LIST_EFFECT_SET_PARTICLE_SELECTION( const WCHAR* strName_ );
	void On_IDC_LIST_EFFECT_SET_PARTICLE_DBLCLK( const WCHAR* strName_ );
	void On_IDC_LIST_EFFECT_SET_MESH_SELECTION( const WCHAR* strName_ );
	void On_IDC_LIST_EFFECT_SET_MESH_DBLCLK( const WCHAR* strName_ );

	//OnDialogEvent_DLG_EFFECT_SET_PARTICLE_MESH_EDIT
	void ON_IDC_EDIT_EFFECT_SET_START_TIME( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_BONE_NAME( const WCHAR* strValue_ );
	void ON_IDC_EDIT_EFFECT_SET_RIDING_BONE_NAME( const WCHAR* strValue_ );
	void ON_IDC_EDIT_EFFECT_SET_POSITION_OFFSET_X( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Y( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Z( float fValue_ );
	void ON_IDC_CHECK_EFFECT_SET_TRACE( bool bVal_ );
	void ON_IDC_CHECK_EFFECT_SET_TRACE_MATRIX( bool bVal_ );
	void ON_IDC_CHECK_EFFECT_SET_TRACE_MORE( bool bVal_);
	void ON_IDC_CHECK_EFFECT_SET_REVERSE_Y( bool bVal_ );
	void ON_IDC_CHECK_EFFECT_SET_USE_LAND_POSITION( bool bVal_ );
	void ON_IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION( bool bVal_ );	
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
	void ON_IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION_MIRROR( bool bVal_ );
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	void ON_IDC_CHECK_EFFECT_SET_APPLY_INSTANCE_SCALE( bool bVal_ );
	void ON_IDC_CHECK_EFFECT_SET_START_WHEN_TRACE_TARGET_DEAD( bool bVal_ );
	void ON_IDC_CHECK_EFFECT_SET_PASSIVE_EFFECT( bool bVal_ );
	void ON_IDC_CHECK_EFFECT_SET_USE_SUB_ATTACK_LIST_SET( bool bVal_ );
#ifdef MODIFY_EFFECT_SET_TRACE_BONE // 오현빈
	void ON_IDC_CHECK_EFFECT_SET_BONE_TRACE_TARGET_MESH( bool bVal_ );
#endif //MODIFY_EFFECT_SET_TRACE_BONE 
	void ON_IDC_EDIT_EFFECT_SET_HYPER_NAME( const WCHAR* strValue_ );
	void ON_IDC_EDIT_EFFECT_SET_WEAPON_BONE_NAME( const WCHAR* strValue_ );
	void ON_IDC_EDIT_EFFECT_SET_WEAPON_BONE_INDEX( int iValue_ );
	void ON_IDC_EDIT_EFFECT_SET_WEAPON_BONE_MODEL_INDEX( int iValue_ );
#ifdef MODIFY_CREATE_EFFECT_SET_LIMIT_DISTANCE
	void ON_IDC_EDIT_EFFECT_LIMIT_DISTANCE( int iValue_ );
#endif // MODIFY_CREATE_EFFECT_SET_LIMIT_DISTANCE


	//DLG_EFFECT_SET_PARTICLE_MESH_EDIT2
	void ON_IDC_COMBO_EFFECT_SET_EFFECT_TYPE( CX2EffectSet::EFFECT_TYPE eEffectType_ );
	void ON_IDC_EDIT_EFFECT_SET_ROTATE_X( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_ROTATE_Y( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_ROTATE_Z( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_SCALE_X( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_SCALE_Y( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_SCALE_Z( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_X( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Y( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Z( float fValue_ );

	//DLG_EFFECT_SET_CAMERA_EDIT
	void ON_IDC_COMBO_EFFECT_SET_CAMERA_SHAKE_TYPE( CKTDGCamera::SHAKE_TYPE eShakeType_ );
	void ON_IDC_CHECK_EFFECT_SET_FIXED_LOOCK_VEC( bool bValue_ );
	void ON_IDC_EDIT_EFFECT_SET_CAMERA_START_TIME( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_LIMIT_DISTANCE( int iValue_ );
	void ON_IDC_EDIT_EFFECT_SET_ONE_DIR_SPEED( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_ONE_DIR_ACCEL( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_TWO_DIR_GAP( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_TWO_DIR_TIME( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_TWO_DIR_TIME_GAP( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_EYE_SHAKE_COUNT( int iValue_ );
	void ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MIN( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MAX( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MIN( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MAX( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_LOOK_AT_SHAKE_COUNT( int iValue_ );
	void ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MIN( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MAX( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MIX( float fValue_ );
	void ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MAX( float fValue_ );
	//DLG_EFFECT_SET_CAMERA_EDIT

	//DLG_PARTICLE_LIST
	void ON_IDC_BUTTON_LOAD_BASE_PARTICLE_FILE();
	void ON_IDC_BUTTON_LOAD_PARTICLE_FILE( const WCHAR* strFileName_ );
	void ON_IDC_BUTTON_INIT_PARTICLE_FILE();
	void ON_IDC_BUTTON_SAVE_PARTICLE_FILE();
	void ON_IDC_BUTTON_PARTICLE_BASE_EDITOR();
	void ON_IDC_BUTTON_PARTICLE_ETC_EDITOR();
	void ON_IDC_LIST_PARTICLE_LIST();
	void On_IDC_BUTTON_DEL_PARTICLE();
	void On_IDC_BUTTON_PARTICLE_LIST_FIND( const WCHAR* strText );
	void On_IDC_BUTTON_PARTICLE_ADD_OTHER_FILE();
	//DLG_PARTICLE_LIST


	//DLG_PARTICLE_LIST_FIND_TEXTURE
	void On_IDC_BUTTON_FIND_TEXTURE( const WCHAR* strText );
	//DLG_PARTICLE_LIST_FIND_TEXTURE

	//DLG_MESH_LIST
	void ON_IDC_BUTTON_LOAD_MESH_FILE( const WCHAR* strFileName_ );
	void ON_IDC_BUTTON_SAVE_MESH_FILE();
	void ON_IDC_BUTTON_MESH_BASE_EDITOR();
	void On_IDC_BUTTON_DEL_MESH();
	void On_IDC_BUTTON_MESH_LIST_FIND( const WCHAR* strText );
	//DLG_MESH_LIST
	////////////////////////////////////////////////////////////////////////////////////


	void							GetMajorResourcePath( wstring& wsResourcePath ) const;
	//이펙트셋
	void							UpdateEffectList();
	void							UpdateSelectedEffectSetData( bool bForce = true );
	CX2EffectSet::EffectSetData*	GetSelectedEffectSetData();	//이펙트셋 리스트 중 선택된 항목 

	void							UpdateSelectedEffectData( bool bForce = true );
	CX2EffectSet::EffectData*		GetEffectDataByName( const WCHAR* strName_ );
	CX2EffectSet::EffectData*		GetSelectedEffectData();	//이펙트셋 내부 파티클and 메쉬

	void							UpdateSelectedCameraShake( bool bForce = true );
	CX2EffectSet::CameraShakeData*	GetSelectedCameraShakeData(); //카메라

	void							UpdateEffectSetMember ( const WCHAR* strText );	//이펙트셋 리스트 클릭.

	void							PlayEffectSet();

	// 파티클or메쉬는 같은 이름이 있을 수 있기 때문에 인덱스로 찾기
	void							UpdateEffectData ();		//파티클or메쉬 리스트 클릭
	void							UpdateEffectSetCameraData ( int iSelectedIndex_ );//카메라 리스트 클릭

	CX2EffectSet::EffectSetData*	CreateEffectSetTempletDummy( const WCHAR* strEffectSetName_ );
	
	CX2EffectSet::EffectSetData*	GetEffectSetDataByName( const WCHAR* strText );
	void ChangeToolMode( TOOL_MODE eToolMode_ ) { m_iToolMode = eToolMode_; ResetDialogAtModeChange(); }
	TOOL_MODE						GetEffectToolMode(){ return static_cast<TOOL_MODE>(m_iToolMode); }
		
	void							UnSelectParticleList();
	void							UnSelectMeshList();
	void							SelectParticleList_EffectMember( int iIndex );
	void							SelectMeshList_EffectMember( int iIndex );


	void							ReadyCurrentEffectSet(); //재생시킬 이펙트 준비
	void							SetReadyParticleList( const CX2EffectSet::EffectData* pEffectData_ );
	void							SetReadyMeshList( const CX2EffectSet::EffectData* pEffectData_ );

	wstring							GetParticleFilePath( const WCHAR* wcParticleName_ );
	////////////////////////////////////////////////////////////////////////////////////
	//파티클
	bool							LoadMjoarParticleFile( const WCHAR* strFileFullPath_ );
	void							AddParticleFileComboBox( const WCHAR* strFileName_ );
	void							UpdateParticleListByIndex( UINT uiIndex_ );
	void							UpdateParticleListByFileName( const WCHAR* strFileName_ );
	void							UpdateParticleListCurrentSelect();
	void							AddParticleAtCurrentEffectset( const WCHAR* strName_ );//선택된 이펙트셋에 파티클 추가
	void							SelectParticleListByIndex( int iIndex_ );
	void							SelectParticleListLastIndex();

	CKTDGParticleSystem::CParticleEventSequence* GetParticleByNameAndIndex( const WCHAR* strParticleName_, int iIndex_ = -1 );

	bool							IsEqualTextureName( const WCHAR* pSrcTexture_, const WCHAR* pDestTexture );
	void							ClearParticleFindList(); 
	void							AddParticleFindList( const WCHAR* pParitcleName_ );

	// 스크립트에서 범위형으로 파싱한 후 확정값으로 변경하는 값들에 대해, 파티클 재생 할 때 갱신시켜주는 함수.
	void							SetRandomValue( CKTDGParticleSystem::CParticleEventSequence* pParticle_ );	// 범위형 값 재설정.

	void							PreViewParticle( const WCHAR* strName_ );
	void							PreVieweXMesh( const WCHAR* strName_ );
	wstring							GetMainMeshName_ByDamageEffectName( const WCHAR* strName_ );

	void							ToggleShowEffectSetList() { m_bShowEffectSetList = !m_bShowEffectSetList; ResetDialogAtModeChange(); }


	//파티클 에디터 다이얼로그 갱신
	void							UpdateDialogParticleEditor( CKTDGParticleSystem::CParticleEventSequence* pParticle_ );
	CKTDGParticleSystem::CParticleEventSequence* CreateParticleEventSequence( const WCHAR* strParticleName_, const WCHAR* strParticleFileName_ );	
	////////////////////////////////////////////////////////////////////////////////////
	//메쉬
	void							LoadMjoarMeshFile( const WCHAR* strFileName_ );
	void							AddMeshAtCurrentEffectset( const WCHAR* strName_ );//선택된 이펙트셋에 메쉬 추가.
	CKTDGXMeshPlayer::XMeshTemplet* GetMeshByName( const WCHAR* strMeshName_ );
	void							UpdateDialogMeshEditor( CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ );

	//메쉬 에디터 관련
	CKTDGXMeshPlayer::XMeshTemplet* CreateXMeshTemplet( const WCHAR* strMeshName_ );
	void							SelectMeshListByIndex( int iIndex_ );
	void							SelectMeshListLastIndex();
	void							UpdateMeshList();

	float							GetDialogEvent_DelayTime() const { return m_fDialogEvent_DelayTime;}
	void							SetDialogEvent_DelayTime( float fValue_ ) { m_fDialogEvent_DelayTime = fValue_; }

	const UINT						GetParticleNumCurrentFile() const { return m_uiParticleNumCurrentFile; }
	const UINT						GetParticleNumByFileName( const WCHAR* pFileName_ ) const;

	wstring							GetEffectSetDesc( const WCHAR* pEffectSetName_   );


	void							SetReadyEffectSetResource( bool bVal_ ) { m_bIsReadyEffectSetResource = bVal_ ; }


	void							SaveWhole( bool bHotKey_ = false ); // 전체 저장
	void							CommitSVN();
	void							CreateEffectSetByDamageEffect();
	void							OutputUsedResource();
#pragma region Public Variable
public:
	float						m_fObjectMoveSpeed;
	vector<TimedXMeshPlayer>	m_vecReadyXMeshPlayer;  // 리소스 로딩
	vector<TimedParticle>		m_vecReadyParticle;		// 리소스 로딩

	PreviewParticle				m_sPreviewParticle;
	PreviewXMeshPlayer			m_sPreviewXMeshPlayer;


	int							m_iPickedUserUnitState;
	bool						m_bUseUserUnitState;				// user unit state에 따라서 animation을 플레이 하는 것을 사용할지 말지 'e' 키를 눌렀을때

	CKTDGXMeshPlayer*			m_pXMeshPlayer;
	CKTDGXMeshPlayer*			m_pMinorxMeshPlayer;
	//CKTDGXMeshPlayer*			m_pXMeshPlayerTest;
	CKTDGParticleSystem*		m_pParticleSystem;
	CKTDGParticleSystem*		m_pMinorParticleSystem;
	//CKTDGParticleSystem*		m_pParticleSystemTest; // 저장된 스크립트와 비교를 위해 임시로 사용함.

	vector<CX2EffectSet::CameraShakeData*> m_vecCameraShake;
	vector<CX2EffectSet::CameraShakeData*> m_vecReadyToPlayCameraShake;

    D3DXCOLOR                   m_fogColor;
	
	CEffectToolGame*				m_pGame;
	CEffectToolCamera			m_FPSCamera;

	vector<CEffectToolDialog*>	m_vecpDialog;
	vector<CWorldToolObject*>	m_vecpWorldToolObject;
	vector<CWorldToolObject*>	m_vecpPickedWorldToolObject;
    vector<CX2WorldLayer*>      m_vecWorldLayer;


	// 상태 변수, UI 변수 
	int							m_iPrevToolMode;
	int							m_iToolMode;
	bool						m_bPaused;
	

	wstring						m_wstrWorldFileName;
	wstring						m_wstrLineMapFileName;

	wstring						m_wstrWorldFilePath;
	wstring						m_wstrLineMapFilePath;
	
	CX2Unit::UNIT_CLASS			m_CurrUnitClass;
	bool						m_bRenderLineMap;
	bool						m_bWorldColor;
	float						m_fWorldColorTime;
	float						m_fWorldColorFadeTime;

	float						m_fNowWorldColorTime;

	CKTDXDeviceXMesh*			m_pXMeshBoundingSphere;

	D3DXVECTOR3					m_vFixedCharacterSkinAnimPosition;
	D3DXVECTOR3					m_vFixedCharacterSkinAnimRotation;
	bool						m_bFixPostionCharacterSkinAnim;
	bool						m_bShowCharacterSkinAnim;
	bool						m_bShowUserUnit;
	CSkinAnimObject*			m_pCharacterSkinAnim;
	
	D3DXCOLOR					m_coFadeWorldColor;

	LINEMAP_FILENAME_TYPE		m_eLineMapFileNameType;

	wstring						m_wstrTempWorldScript;
	wstring						m_wstrTempLineMapScript_Prefix;
	wstring						m_wstrTempLineMapScript_Suffix;


	CKTDGFontManager::CUKFont*	m_pFont_A;	

	CKTDXTimer					m_TimerRenderPickedObject;
	bool						m_bRenderBoundingSphere;
	bool						m_bRenderFlicker;


	bool						m_bCenterGreenDot;
    bool                        m_bFog;

	bool						m_bShowLineMapModifier;
	WCHAR						wszTempFilePath[MAX_PATH];
	vector<STeamStartPos>		vecTeamStartPos;

	bool							m_bMouseDragging;
	D3DXVECTOR2						m_vDragStart;
	int								m_CurrKeyUsed;
	bool							m_abKeyUsed[1024];
	vector< SNPCStartPos >			m_vecPickedNPCStartPosKey;
	bool				m_bUseNPCMesh;	
	bool				m_bHideStartPosMesh;
	bool				m_bShowUserUnitStartPos;
	int					m_iPickedStartPosIndex;
	
	int		iTeamColor;
	int		iInitialDir;
	int								m_iShowDungeonLevel;

	CKTDGMatrix*					m_pMatrix;
	CKTDXDeviceXSkinMesh*		m_pXSkinMesh;
	vector<CKTDGXSkinAnim*>		vecpXSkinAnim;

	ID3DXMesh*					m_pMesh3DText[12];
	float						m_f3DTextAngle, m_f3DTextAngleStep;	
	int							m_iStartPosListIndex;

	bool						m_bShowUserStartPos;
	bool						m_bShowNpcStartPos;	
	bool						m_bShowWorld;
#ifdef CHECK_VERSION
	wstring							m_wstrVersion;
#endif //CHECK_VERSION
#pragma endregion Public Variable

private:
	////////////////////////////////////////////////////////////
	//이펙트셋
	CX2EffectToolFileManager*		m_pFileManager;		
	CX2EffectSet::EffectSetData*	m_pSelectedEffectSetData;	//현재 선택된 이펙트셋
	CX2EffectSet::EffectData*		m_pSelectedEffectData;		//현재 선택된 이펙트data (Particle or Mesh)
	CX2EffectSet::CameraShakeData*	m_pSelectedCameraData;		//현재 선택된 카메라쉐이크 data

	int								m_iSelectedCameraIndex;
	int								m_iSelectedEffectSetDataIndex; 
	int								m_iSelectedParticleMeshIndex; // 파티클or메쉬 인덱스

	bool							m_bShowEffectSetList;

	////////////////////////////////////////////////////////////
	//파티클
	//컨테이너 벡터 :  중복되면 안되지만, 인덱스로 접근도 가능해야 되기 때문에 벡터로 하고, 중복은 push 전 체크
	vector<wstring>					m_vecParticleFileName;		/// 로드한 파티클 파일 이름 (확장자 포함)
	wstring							m_wstrCurrentParticleFileName;
	wstring							m_wstrLastBaseFileName;
	UINT							m_uiParticleFileNum;
	UINT							m_uiParticleNumCurrentFile;

	CX2EffectToolParticleEditor*	m_pParticleEditor;
	CX2EffectToolMeshEditor*		m_pMeshEditor;

	float							m_fDialogEvent_DelayTime;

	//
	map<wstring, wstring>			m_mapParticleFilePath;	// <파티클 파일명, 파일 경로>


	bool							m_bIsReadyEffectSetResource;
	bool							m_bPreScriptLoad;

};

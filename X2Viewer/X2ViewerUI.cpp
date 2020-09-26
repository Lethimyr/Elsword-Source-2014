#include "dxstdafx.h"
#include ".\x2viewerui.h"

#define SiSelf()				ms_pSelfInstance
#define SiMain()				ms_pSelfInstance->m_pMain
#define SiGetObject( s )		ms_pSelfInstance->m_pMain->GetObject( CX2ViewerObject::##s )
#define SiGetBaseDlg( s )		ms_pSelfInstance->m_BaseUI.GetControl( s )
#define SiGetSkinMeshDlg( s )	ms_pSelfInstance->m_SkinMeshUI.GetControl( s )
#define SiGetBoneDlg( s )		ms_pSelfInstance->m_BoneUI.GetControl( s )
#define SiGetEquipDlg( s )		ms_pSelfInstance->m_EquipUI.GetControl( s )
#define SiGetWeaponDlg( s )		ms_pSelfInstance->m_WeaponUI.GetControl( s )
#define SiGetAccessoryDlg( s )	ms_pSelfInstance->m_AccessoryUI.GetControl( s )
#define SiGetAnimtaionDlg( s )	ms_pSelfInstance->m_AnimationUI.GetControl( s )
#define SiGetMeshDlg( s )		ms_pSelfInstance->m_Mesh.GetControl( s )
#define SiGetRPDlg( s )			ms_pSelfInstance->m_RenderParamUI.GetControl( s )

#ifndef UPDATE_X2VIEWER_2013 //JHKang
#define SiGetParticleDlg( s )	ms_pSelfInstance->m_ParticleBasic.GetControl( s )
#define SiGetParticleEditorDlg( s )	ms_pSelfInstance->m_ParticleEditor.GetControl( s )
#endif //UPDATE_X2VIEWER_2013

#define	GET_LUA_POS( s, x, y, w, h ) \
{ \
	LUA_GET_VALUE( luaManager, #s "_X", x, 0 ); \
	LUA_GET_VALUE( luaManager, #s "_Y", y, 0 ); \
	LUA_GET_VALUE( luaManager, #s "_W", w, 0 ); \
	LUA_GET_VALUE( luaManager, #s "_H", h, 0 ); \
} \

#define	GET_PLUA_POS( s, x, y, w, h ) \
{ \
	PLUA_GET_VALUE( luaManager, #s "_X", x, 0 ); \
	PLUA_GET_VALUE( luaManager, #s "_Y", y, 0 ); \
	PLUA_GET_VALUE( luaManager, #s "_W", w, 0 ); \
	PLUA_GET_VALUE( luaManager, #s "_H", h, 0 ); \
} \

CX2ViewerUI*	CX2ViewerUI::ms_pSelfInstance	= NULL;

template <class T>
bool from_string( T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&) )
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

void DropAnimationFile( CX2ViewerUI* pViewerUI, const WCHAR* pFileName, const WCHAR* pDir )
{
	pViewerUI->DropFile( pFileName, pDir );
}

#pragma region CX2ViewerUI
CX2ViewerUI::CX2ViewerUI( CX2ViewerMain* pMain )
	: m_pMain( pMain )
	, m_fElapsedTime( 0.0f )
	, m_bIsInit( false )
	, m_MeshSel( MS_NONE )
{
	CX2ViewerObject::SetObjectStyle( CX2ViewerObject::OS_UI );

	ms_pSelfInstance = this;
	m_vecDialog.clear();
	m_strMotionFileList.clear();
	m_bModifyBone = false;
	m_bSkinMeshUI = false;
	m_bWeaponUI = false;
	m_bAccessoryUI = false;
	
	m_fAnimTimeInc = 0.033f;

	InitCharacerStringMap();

	InitBaseUI();			// 기본 UI 초기화
	InitSkinMeshUI();		// SkinMesh UI
	InitBoneUI();			// Bone UI 초기화
	InitEquipUI();			// Equip UI 초기화
	InitWeaponUI();			// Weapon UI 초기화
	InitAccessoryUI();		// Accessory UI 초기화
	InitAnimationUI();		// Animation UI 초기화
	InitRenderParamUI();	// RenderParam UI 초기화

	#pragma region Mesh Option
	m_Mesh.Init( &g_DialogResourceManager );
	m_Mesh.SetCallback( OnGUIMeshEvent );

	m_Mesh.AddStatic( UI_STATIC_MESH_SCALE, L"Scale", 0, 0, 0, 0 );
	m_Mesh.AddStatic( UI_STATIC_MESH_SCALE_X, L"X", 0, 0, 0, 0 );
	m_Mesh.AddStatic( UI_STATIC_MESH_SCALE_Y, L"Y", 0, 0, 0, 0 );
	m_Mesh.AddStatic( UI_STATIC_MESH_SCALE_Z, L"Z", 0, 0, 0, 0 );
	m_Mesh.AddEditBox( UI_EDIT_MESH_SCALE_X, L"", 0, 0, 0, 0 );
	m_Mesh.AddEditBox( UI_EDIT_MESH_SCALE_Y, L"", 0, 0, 0, 0 );
	m_Mesh.AddEditBox( UI_EDIT_MESH_SCALE_Z, L"", 0, 0, 0, 0 );

	m_Mesh.AddStatic( UI_STATIC_MESH_LIGHT_POS, L"LightPos", 0, 0, 0, 0 );
	m_Mesh.AddStatic( UI_STATIC_MESH_LIGHT_POS_X, L"X", 0, 0, 0, 0 );
	m_Mesh.AddStatic( UI_STATIC_MESH_LIGHT_POS_Y, L"Y", 0, 0, 0, 0 );
	m_Mesh.AddStatic( UI_STATIC_MESH_LIGHT_POS_Z, L"Z", 0, 0, 0, 0 );
	m_Mesh.AddEditBox( UI_EDIT_MESH_LIGHT_POS_X, L"", 0, 0, 0, 0 );
	m_Mesh.AddEditBox( UI_EDIT_MESH_LIGHT_POS_Y, L"", 0, 0, 0, 0 );
	m_Mesh.AddEditBox( UI_EDIT_MESH_LIGHT_POS_Z, L"", 0, 0, 0, 0 );
	m_Mesh.AddCheckBox( UI_CHECK_MESH_LIGHT_ONOFF, L"Light On/Off", 0, 0, 0, 0, true );

	m_Mesh.AddButton( UI_BUT_MESH_RENDER_PARAM, L"Render\nParam", 0, 0, 0, 0 );

	m_vecDialog.push_back( &m_Mesh );
	#pragma endregion Mesh 옵션, Y Fiile format

	#pragma region Rendering Parameter Option
	#pragma endregion 렌더링 파라미터

	#pragma region Particle Option
	#ifndef UPDATE_X2VIEWER_2013 //JHKang
	m_ParticleBasic.Init( &g_DialogResourceManager );
	m_ParticleBasic.SetCallback( OnGUIParticleEvent );

	m_ParticleBasic.AddListBox( UI_LIST_PARTICLE_LIST, 0, 0, 0, 0 );

	m_ParticleBasic.AddButton( UI_BUT_PARTICLE_DELETE, L"Del", 0, 0, 0, 0 );

	m_ParticleBasic.AddStatic( UI_STATIC_PARTICLE_TIME, L"Time", 0, 0, 0,0 );
	m_ParticleBasic.AddEditBox( UI_EDIT_PARTICLE_TIME, L"0.0", 0, 0, 0, 0 );

	m_ParticleBasic.AddButton( UI_BUT_PARTICLE_BONESET, L"SetBone", 0, 0, 0, 0 );
	m_ParticleBasic.AddButton( UI_BUT_PARTICLE_BONECLEAR, L"ClearBone", 0, 0, 0, 0 );
	m_ParticleBasic.AddCheckBox( UI_CHECK_PARTICLE_TRACE, L"Trace", 0,0,0,0, true );

	m_ParticleBasic.AddStatic( UI_STATIC_PARTICLE_OFFSET, L"Pos", 0, 0, 0,0 );
	m_ParticleBasic.AddEditBox( UI_EDIT_PARTICLE_OFFSET_X, L"0", 0, 0, 0, 0 );
	m_ParticleBasic.AddEditBox( UI_EDIT_PARTICLE_OFFSET_Y, L"0", 0, 0, 0, 0 );
	m_ParticleBasic.AddEditBox( UI_EDIT_PARTICLE_OFFSET_Z, L"0", 0, 0, 0, 0 );
	m_ParticleBasic.AddCheckBox( UI_CHECK_PARTICLE_LANDPOS, L"LandPos", 0, 0, 0, 0, false );

	m_ParticleBasic.AddStatic( UI_STATIC_PARTICLE_ROT, L"Rot", 0, 0, 0,0 );
	m_ParticleBasic.AddEditBox( UI_EDIT_PARTICLE_ROT_X, L"0", 0, 0, 0, 0 );
	m_ParticleBasic.AddEditBox( UI_EDIT_PARTICLE_ROT_Y, L"0", 0, 0, 0, 0 );
	m_ParticleBasic.AddEditBox( UI_EDIT_PARTICLE_ROT_Z, L"0", 0, 0, 0, 0 );
	m_ParticleBasic.AddCheckBox( UI_CHECK_PARTICLE_APPUNITROT, L"Apply Unit Rot", 0, 0, 0, 0, true );
	m_ParticleBasic.AddButton( UI_BUT_PARTICLE_SAVESEQUENCE, L"Save\nSequence", 0, 0, 0, 0 );

	m_ParticleBasic.AddButton( UI_BUT_PARTICLE_PARTICLEEDITOR, L"ParticleEditor", 0, 0, 0, 0 );

	m_vecDialog.push_back( &m_ParticleBasic );

	// Particle Editor
	m_ParticleEditor.Init( &g_DialogResourceManager );
	m_ParticleEditor.SetCallback( OnGUIParticleEditorEvent );
	m_ParticleEditor.AddListBox( UI_LIST_PARTICLE_EDITOR_MYPARTICLE, 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_MYPARTICLE_LOAD, L"Load", 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_MYPARTICLE_DELETE, L"Delete", 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_MYPARTICLE_SAVE, L"Save", 0, 0, 0, 0 );

	m_ParticleEditor.AddListBox( UI_LIST_PARTICLE_EDITOR_PARTICLETEMPLET, 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_PARTICLETEMPLET_COPY, L"Copy", 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_PARTICLETEMPLET_RELOAD, L"Reload", 0, 0, 0, 0 );

	m_ParticleEditor.AddStatic( UI_STATIC_PARTICLE_EDITOR_EMITTERATTRIBUTE, L"EmitterProperties", 0, 0, 0,0 );
	m_ParticleEditor.AddListBox( UI_LIST_PARTICLE_EDITOR_EMITTERATTRIBUTE, 0, 0, 0, 0 );
	m_ParticleEditor.AddStatic( UI_STATIC_PARTICLE_EDITOR_EMITTERATTRIBUTE_VALUE, L"Value", 0, 0, 0,0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_SINGLE, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_X, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Y, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Z, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddComboBox( UI_COMBO_PARTICLE_EDITOR_EMITTERATTRIBUTE, 0, 0, 0, 0 );
	//m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_EMITTERATTRIBUTE_DEFAULT, L"DefaultValue", 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_EMITTERATTRIBUTE_APPLY, L"Apply", 0, 0, 0, 0 );

	m_ParticleEditor.AddListBox( UI_LIST_PARTICLE_EDITOR_EVENT, 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_EVENT_DELETE, L"Delete", 0, 0, 0, 0 );

	m_ParticleEditor.AddStatic( UI_STATIC_PARTICLE_EDITOR_EVENT_TYPE, L"EventType", 0, 0, 0,0 );
	m_ParticleEditor.AddListBox( UI_LIST_PARTICLE_EDITOR_EVENT_TYPE, 0, 0, 0, 0 );
	m_ParticleEditor.AddStatic( UI_STATIC_PARTICLE_EDITOR_EVENT_TIME, L"Time", 0, 0, 0,0 );
	m_ParticleEditor.AddCheckBox( UI_CHECK_PARTICLE_EDITOR_EVENT_FADE, L"Fade", 0, 0, 0, 0 );
	m_ParticleEditor.AddStatic( UI_STATIC_PARTICLE_EDITOR_EVENT_FROM, L"From", 0, 0, 0,0 );
	m_ParticleEditor.AddStatic( UI_STATIC_PARTICLE_EDITOR_EVENT_TO, L"To", 0, 0, 0,0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME1, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME2, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddStatic( UI_STATIC_PARTICLE_EDITOR_EVENT_VALUE, L"Value", 0, 0, 0,0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EVENT_SINGLE, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EVENT_X, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EVENT_Y, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EVENT_Z, L"0.0", 0, 0, 0, 0 );

	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EVENT_R, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EVENT_G, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EVENT_B, L"0.0", 0, 0, 0, 0 );
	m_ParticleEditor.AddEditBox( UI_EDIT_PARTICLE_EDITOR_EVENT_A, L"0.0", 0, 0, 0, 0 );

	m_ParticleEditor.AddComboBox( UI_COMBO_PARTICLE_EDITOR_EVENT, 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_EVENT_APPLY, L"Apply", 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_EVENT_NEW, L"New", 0, 0, 0, 0 );


	m_ParticleEditor.AddListBox( UI_LIST_PARTICLE_EDITOR_MODEL_LIST, 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_MODEL_ADD, L"Add", 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_MODEL_DELETE, L"Delete", 0, 0, 0, 0 );

	m_ParticleEditor.AddStatic( UI_STATIC_PARTICLE_EDITOR_PLAY_TYPE, L"Play Type", 0, 0, 0,0 );
	m_ParticleEditor.AddComboBox( UI_COMBO_PARTICLE_EDITOR_PLAY_TYPE, 0, 0, 0, 0 );
	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_PLAY, L"Play▶", 0, 0, 0, 0 );

	m_ParticleEditor.AddStatic( UI_STATIC_PARTICLE_EDITOR_SPEED, L"Play Type", 0, 0, 0,0 );
	m_ParticleEditor.AddSlider( UI_SLIDE_PARTICLE_EDITOR_SPEED, 0, 0, 0, 0, 0, 100, 100 );
	m_ParticleEditor.AddStatic( UI_STATIC_PARTICLE_EDITOR_TIME, L"0.000 / 0.000", 0, 0, 0,0 );

	m_ParticleEditor.AddButton( UI_BUT_PARTICLE_EDITOR_EXIT, L"Exit Particle Editor", 0, 0, 0, 0 );

	m_vecDialog.push_back( &m_ParticleEditor );
	#endif //UPDATE_X2VIEWER_2013
	#pragma endregion 파티클 생성
	// m_vecDialog에 Push하는 다이얼로그를 더 추가할 경우, OnGUIParticleEditorEvent의 case UI_BUT_PARTICLE_EDITOR_EXIT 항목
	// 내부에 있는 m_vecDialog.push_back 관련 부분에도 함께 추가해 주세요. 파티클 에디터로 갔다가 돌아올 때 UI 표시상태를
	// 원상복구 시키기 위해서 필요합니다.

	SetSkinMeshOnOff( false );
	SetBoneOnOff( false );
	SetEquipUIOnOff( false );
	SetWeaponOnOff( false );
	SetAccessoryOnOff( false );
	SetAnimationOnOff( false );
	SetRenderParamOnOff( false );
	//SetMeshOnOff( false );
#ifndef UPDATE_X2VIEWER_2013 //JHKang
	SetEffectButOnOff( false );
	SetParticleButOnOff( true );
	SetParticleEditButOnOff( false );
#endif //UPDATE_X2VIEWER_2013

	SiMain()->SetSelectedAnimIndex( -1 );
}
#pragma endregion 생성자

#pragma region ~CX2ViewerUI
CX2ViewerUI::~CX2ViewerUI(void)
{
	SiSelf()->m_vecFrameNameList.clear();
	SiSelf()->m_vecModifyFrameNameListBase.clear();
	m_vecDialog.clear();
}
#pragma endregion 소멸자

#pragma region OnFrameMove
HRESULT CX2ViewerUI::OnFrameMove( double fTime, float fElapsedTime )
{
	m_fElapsedTime = fElapsedTime;

	/// 포커스 처리
	CX2ViewerCamera* pCamera = (CX2ViewerCamera*)SiGetObject( OS_CAMERA );

	if ( pCamera )
	{
		if ( SiGetBaseDlg( UI_EDIT_LIGHT_POS_X )->m_bHasFocus == true || SiGetBaseDlg( UI_EDIT_LIGHT_POS_Y )->m_bHasFocus == true
			 || SiGetBaseDlg( UI_EDIT_LIGHT_POS_Z )->m_bHasFocus == true || SiGetBaseDlg( UI_LIST_MODEL )->m_bHasFocus == true
			 || SiGetSkinMeshDlg( UI_EDIT_SCALE_X )->m_bHasFocus == true || SiGetSkinMeshDlg( UI_EDIT_SCALE_Y )->m_bHasFocus == true
			 || SiGetSkinMeshDlg( UI_EDIT_SCALE_Z )->m_bHasFocus == true || SiGetSkinMeshDlg( UI_EDIT_POSITION_X )->m_bHasFocus == true
			 || SiGetSkinMeshDlg( UI_EDIT_POSITION_Y )->m_bHasFocus == true || SiGetSkinMeshDlg( UI_EDIT_POSITION_Z )->m_bHasFocus == true
			 || SiGetSkinMeshDlg( UI_LIST_BONE )->m_bHasFocus == true || SiGetSkinMeshDlg( UI_LIST_MODIFY_BONE )->m_bHasFocus == true
			 || SiGetSkinMeshDlg( UI_COMBO_DEFAULT_BONE )->m_bHasFocus == true || SiGetBoneDlg( UI_EDIT_BONE_SCALE_X )->m_bHasFocus == true
			 || SiGetBoneDlg( UI_EDIT_BONE_SCALE_Y )->m_bHasFocus == true || SiGetBoneDlg( UI_EDIT_BONE_SCALE_Z )->m_bHasFocus == true
			 || SiGetBoneDlg( UI_EDIT_BONE_POS_Y )->m_bHasFocus == true || SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_X )->m_bHasFocus == true 
			 || SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_Y )->m_bHasFocus == true || SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_Z )->m_bHasFocus == true
			 || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_X )->m_bHasFocus == true || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_Y )->m_bHasFocus == true
			 || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_Z )->m_bHasFocus == true || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_X )->m_bHasFocus == true
			 || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_Y )->m_bHasFocus == true || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_Z )->m_bHasFocus == true
			 || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_X )->m_bHasFocus == true || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_Y )->m_bHasFocus == true
			 || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_Z )->m_bHasFocus == true || SiGetEquipDlg( UI_LIST_ATTACH_BONES )->m_bHasFocus == true
			 || SiGetAnimtaionDlg( UI_SLIDE_ANIM_SPEED )->m_bHasFocus == true || SiGetAnimtaionDlg( UI_SLIDE_ANIM_FRAME )->m_bHasFocus == true
			 || SiGetAnimtaionDlg( UI_LIST_ANIMATION )->m_bHasFocus == true || SiGetRPDlg( UI_COMBO_RENDERTYPE )->m_bHasFocus == true
			 || SiGetRPDlg( UI_COMBO_CARTOON_TEX_TYPE )->m_bHasFocus == true || SiGetRPDlg( UI_COMBO_CULLMODE )->m_bHasFocus == true
			 || SiGetRPDlg( UI_COMBO_SRCBLEND )->m_bHasFocus == true || SiGetRPDlg( UI_COMBO_DESTBLEND )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_OUTLINE_COLOR_A )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_OUTLINE_COLOR_R )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_OUTLINE_COLOR_G )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_OUTLINE_COLOR_B )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_COLOR_A )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_COLOR_R )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_COLOR_G )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_COLOR_B )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_LIGHTFLOW_WIDE )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_LIGHTFLOW_IMPACT_MIN )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_LIGHTFLOW_IMPACT_MAX )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_LIGHTFLOW_IMPACT_ANIMTIME )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_LIGHTFLOW_POINT_X )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_LIGHTFLOW_POINT_Y )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_LIGHTFLOW_POINT_Z )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE0_MIN_X )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE0_MIN_Y )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE0_MAX_X )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE0_MAX_Y )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE0_ANIMTIME )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE1_MIN_X )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE1_MIN_Y )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE1_MAX_X )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE1_MAX_Y )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE1_ANIMTIME )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE2_MIN_X )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE2_MIN_Y )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE2_MAX_X )->m_bHasFocus == true
			 || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE2_MAX_Y )->m_bHasFocus == true || SiGetRPDlg( UI_EDIT_TEXOFFSET_STAGE2_ANIMTIME )->m_bHasFocus == true
		   )
		{
			if ( pCamera->GetUsingWheel() == true )
				pCamera->SetUsingWheel( false );

			CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );

			if ( pSkinMesh )
			{
				if ( SiGetBaseDlg( UI_EDIT_LIGHT_POS_X )->m_bHasFocus == true || SiGetBaseDlg( UI_EDIT_LIGHT_POS_Y )->m_bHasFocus == true
					|| SiGetBaseDlg( UI_EDIT_LIGHT_POS_Z )->m_bHasFocus == true )
				{
					pSkinMesh->SetLightPosX( ChangeEditBoxValueToWheel( UI_EDIT_LIGHT_POS_X, &m_BaseUI, 50.f ) );
					pSkinMesh->SetLightPosY( ChangeEditBoxValueToWheel( UI_EDIT_LIGHT_POS_Y, &m_BaseUI, 50.f ) );
					pSkinMesh->SetLightPosZ( ChangeEditBoxValueToWheel( UI_EDIT_LIGHT_POS_Z, &m_BaseUI, 50.f ) );
				}

				if ( SiGetSkinMeshDlg( UI_EDIT_SCALE_X )->m_bHasFocus == true || SiGetSkinMeshDlg( UI_EDIT_SCALE_Y )->m_bHasFocus == true
					|| SiGetSkinMeshDlg( UI_EDIT_SCALE_Z )->m_bHasFocus == true )
				{
					pSkinMesh->SetScaleX( ChangeEditBoxValueToWheel( UI_EDIT_SCALE_X, &m_SkinMeshUI, 0.1f ) );
					pSkinMesh->SetScaleY( ChangeEditBoxValueToWheel( UI_EDIT_SCALE_Y, &m_SkinMeshUI, 0.1f ) );
					pSkinMesh->SetScaleZ( ChangeEditBoxValueToWheel( UI_EDIT_SCALE_Z, &m_SkinMeshUI, 0.1f ) );
				}

				if ( SiGetSkinMeshDlg( UI_EDIT_POSITION_X )->m_bHasFocus == true || SiGetSkinMeshDlg( UI_EDIT_POSITION_Y )->m_bHasFocus == true
					|| SiGetSkinMeshDlg( UI_EDIT_POSITION_Z )->m_bHasFocus == true )
				{
					pSkinMesh->SetPositionX( ChangeEditBoxValueToWheel( UI_EDIT_POSITION_X, &m_SkinMeshUI, 50.f ) );
					pSkinMesh->SetPositionY( ChangeEditBoxValueToWheel( UI_EDIT_POSITION_Y, &m_SkinMeshUI, 50.f ) );
					pSkinMesh->SetPositionZ( ChangeEditBoxValueToWheel( UI_EDIT_POSITION_Z, &m_SkinMeshUI, 50.f ) );
				}

				if ( m_vecModifyFrameNameListBase.size() > 0 )
				{
					wstring strBoneName = reinterpret_cast<CDXUTEditBox*>( SiGetSkinMeshDlg( UI_EDIT_MODIFY_BONE ) )->GetText();
					CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = pSkinMesh->GetXSkinAnim()->GetCloneFrame( strBoneName.c_str() );
										
					if ( pFrame )
					{
						D3DXVECTOR3 vScale( 1.f, 1.f, 1.f );
						float fBoneY = 0.f;

						if ( SiGetBoneDlg( UI_EDIT_BONE_SCALE_X )->m_bHasFocus == true || SiGetBoneDlg( UI_EDIT_BONE_SCALE_Y )->m_bHasFocus == true
							|| SiGetBoneDlg( UI_EDIT_BONE_SCALE_Z )->m_bHasFocus == true || SiGetBoneDlg( UI_EDIT_BONE_POS_Y )->m_bHasFocus == true )
						{
							vScale.x = ChangeEditBoxValueToWheel( UI_EDIT_BONE_SCALE_X, &m_BoneUI, 0.1f );
							vScale.y = ChangeEditBoxValueToWheel( UI_EDIT_BONE_SCALE_Y, &m_BoneUI, 0.1f );
							vScale.z = ChangeEditBoxValueToWheel( UI_EDIT_BONE_SCALE_Z, &m_BoneUI, 0.1f );
							fBoneY = ChangeEditBoxValueToWheel( UI_EDIT_BONE_POS_Y, &m_BoneUI, 1.0f );

							SetFrameScale( &vScale, true, pFrame );
							SetFrameMoveBoneY( pFrame, fBoneY );

							LPCSTR			szName;
							std::wstring	wstrName;
							WCHAR			wszName[128] = L"";
							
							BOOST_FOREACH( CKTDXDeviceXSkinMesh::MultiAnimFrame* pMAF, SiSelf()->m_vecModifyFrameNameListBase )
							{
								szName = pMAF->Name;
								MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);
								wstrName = wszName;

								if ( wstrName.compare( strBoneName.c_str() ) == 0 )
								{
									pMAF->m_vScaleRel = vScale;
									pMAF->m_fMoveBoneY = fBoneY;
								}
							}
						}
					}
				}

				CDXUTEditBox* pEditBox = NULL;
				CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_WEAPON ) );
				int nIndex = pListBox->GetSelectedIndex( -1 );

				if ( SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_X )->m_bHasFocus == true || SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_Y )->m_bHasFocus == true
					|| SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_Z )->m_bHasFocus == true )
				{
					if ( pListBox->GetSize() > 0 && nIndex > -1 )
					{
						SWeaponInfo* pWeaponInfo = pSkinMesh->GetWeaponInfo( pListBox->GetItem( nIndex )->strText );

						if ( pWeaponInfo )
						{
							pWeaponInfo->m_vRotate.x = ChangeEditBoxValueToWheel( UI_EDIT_WEAPON_BONE_ROT_X, &m_WeaponUI, 0.1f );
							pWeaponInfo->m_vRotate.y = ChangeEditBoxValueToWheel( UI_EDIT_WEAPON_BONE_ROT_Y, &m_WeaponUI, 0.1f );
							pWeaponInfo->m_vRotate.z = ChangeEditBoxValueToWheel( UI_EDIT_WEAPON_BONE_ROT_Z, &m_WeaponUI, 0.1f );
						}
					}
				}

				pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_ACCESSORY ) );
				nIndex = pListBox->GetSelectedIndex( -1 );

				if ( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_X )->m_bHasFocus == true || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_Y )->m_bHasFocus == true
					 || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_Z )->m_bHasFocus == true || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_X )->m_bHasFocus == true
					 || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_Y )->m_bHasFocus == true || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_Z )->m_bHasFocus == true
					 || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_X )->m_bHasFocus == true || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_Y )->m_bHasFocus == true
					 || SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_Z )->m_bHasFocus == true
				   )
				{
					if ( pListBox->GetSize() > 0 && nIndex > -1 )
					{
						SAccessoryInfo* pAccessoryInfo = pSkinMesh->GetAccessoryInfo( pListBox->GetItem( nIndex )->strText );

						if ( pAccessoryInfo )
						{
							pAccessoryInfo->m_vMove.x = ChangeEditBoxValueToWheel( UI_EDIT_ACCESSORY_MOVE_X, &m_AccessoryUI, 5.0f );
							pAccessoryInfo->m_vMove.y = ChangeEditBoxValueToWheel( UI_EDIT_ACCESSORY_MOVE_Y, &m_AccessoryUI, 5.0f );
							pAccessoryInfo->m_vMove.z = ChangeEditBoxValueToWheel( UI_EDIT_ACCESSORY_MOVE_Z, &m_AccessoryUI, 5.0f );

							pAccessoryInfo->m_vRotate.x = ChangeEditBoxValueToWheel( UI_EDIT_ACCESSORY_ROT_X, &m_AccessoryUI, 1.0f );
							pAccessoryInfo->m_vRotate.y = ChangeEditBoxValueToWheel( UI_EDIT_ACCESSORY_ROT_Y, &m_AccessoryUI, 1.0f );
							pAccessoryInfo->m_vRotate.z = ChangeEditBoxValueToWheel( UI_EDIT_ACCESSORY_ROT_Z, &m_AccessoryUI, 1.0f );

							pAccessoryInfo->m_vScale.x = ChangeEditBoxValueToWheel( UI_EDIT_ACCESSORY_SCALE_X, &m_AccessoryUI, 0.1f );
							pAccessoryInfo->m_vScale.y = ChangeEditBoxValueToWheel( UI_EDIT_ACCESSORY_SCALE_Y, &m_AccessoryUI, 0.1f );
							pAccessoryInfo->m_vScale.z = ChangeEditBoxValueToWheel( UI_EDIT_ACCESSORY_SCALE_Z, &m_AccessoryUI, 0.1f );
						}
					}
				}
			}
		}
		else
		{
			if ( pCamera->GetUsingWheel() == false )
				pCamera->SetUsingWheel( true );
		}

		if ( SiGetBaseDlg( UI_EDIT_CAMERA_EYE_POS_X )->m_bHasFocus == false && SiGetBaseDlg( UI_EDIT_CAMERA_EYE_POS_Y )->m_bHasFocus == false
			&& SiGetBaseDlg( UI_EDIT_CAMERA_EYE_POS_Z )->m_bHasFocus == false && SiGetBaseDlg( UI_EDIT_CAMERA_LOOK_POS_X )->m_bHasFocus == false
			&& SiGetBaseDlg( UI_EDIT_CAMERA_LOOK_POS_Y )->m_bHasFocus == false && SiGetBaseDlg( UI_EDIT_CAMERA_LOOK_POS_Z )->m_bHasFocus == false )
		{
			WCHAR strNum[128] = L"";
			D3DXVECTOR3 vTemp = pCamera->GetEye();
			CDXUTEditBox* pEditBox = NULL;
			swprintf( strNum, L"%.2f", vTemp.x );
			pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_EYE_POS_X );
			pEditBox->SetText( strNum );
			swprintf( strNum, L"%.2f", vTemp.y );
			pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_EYE_POS_Y );
			pEditBox->SetText( strNum );
			swprintf( strNum, L"%.2f", vTemp.z );
			pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_EYE_POS_Z );
			pEditBox->SetText( strNum );

			vTemp = pCamera->GetLookVec();
			swprintf( strNum, L"%.2f", vTemp.x );
			pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_LOOK_POS_X );
			pEditBox->SetText( strNum );
			swprintf( strNum, L"%.2f", vTemp.y );
			pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_LOOK_POS_Y );
			pEditBox->SetText( strNum );
			swprintf( strNum, L"%.2f", vTemp.z );
			pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_LOOK_POS_Z );
			pEditBox->SetText( strNum );
		}
	}
	
	DrawAnimFrame();
#ifndef UPDATE_X2VIEWER_2013 //JHKang
	DrawParticleTime();
#endif //UPDATE_X2VIEWER_2013
	
	return S_OK;
}
#pragma endregion 매 프레임 수행

#pragma region OnFrameRender
HRESULT CX2ViewerUI::OnFrameRender()
{
	for( int i = 0; i < (int)m_vecDialog.size(); ++i )
	{
		m_vecDialog[i]->OnRender( m_fElapsedTime );
	}

	return S_OK;
}
#pragma endregion 매 프레임 렌더링

#pragma region OnResetDevice
HRESULT CX2ViewerUI::OnResetDevice()
{
	m_BaseUI.GetListBox( UI_LIST_MODEL )->RemoveAllItems();

	for( int i = 0; i < (int)m_vecDialog.size(); ++i )
	{
		m_vecDialog[i]->SetLocation( 0, 0 );
	}

	Init();

	return S_OK;
}
#pragma endregion 장치 재설정

#pragma region OnLostDevice
HRESULT CX2ViewerUI::OnLostDevice()
{
	return S_OK;
}
#pragma endregion 장치 소실 시

#pragma region MsgProc
bool CX2ViewerUI::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( uMsg == WM_DROPFILES )
	{
		WCHAR PullFileName[MAX_PATH] = L"";
		
        int nFiles;

        nFiles = DragQueryFile((HDROP) wParam, 0xFFFFFFFF, PullFileName, MAX_PATH);

        for(int index = 0; index < nFiles; ++index)
        {
            WCHAR FileName[256] = L"";
            WCHAR PullDir[MAX_PATH] = L"";
            WCHAR drive[_MAX_DRIVE] = L"";
            WCHAR dir[_MAX_DIR] = L"";
            WCHAR fname[_MAX_FNAME] = L"";
            WCHAR ext[_MAX_EXT] = L"";

            DragQueryFile((HDROP) wParam, index, PullFileName, MAX_PATH);
            _wsplitpath( PullFileName, drive, dir, fname, ext);

            wcscat( FileName, fname);
            wcscat( FileName, ext);

            wcscat( PullDir, drive );
            wcscat( PullDir, dir );

            DropFile( FileName, PullDir );
        }
		
	}

	for( int i = 0; i < (int)m_vecDialog.size(); ++i )
	{
		if( m_vecDialog[i]->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}

	return false;
}
#pragma endregion 메시지 처리

#pragma region OnGUIBaseEvent
void CALLBACK CX2ViewerUI::OnGUIBaseEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case UI_BUT_OPEN:
		{
			if( SiSelf()->m_FileOS.FileOpen( L"X-file(*.x)\0*.x\0" ) == CX2ViewerFileOS::FS_XFILE )
			{
				WCHAR FileName[256] = L"";
				WCHAR PullDir[MAX_PATH] = L"";
				WCHAR drive[_MAX_DRIVE] = L"";
				WCHAR dir[_MAX_DIR] = L"";
				WCHAR fname[_MAX_FNAME] = L"";
				WCHAR ext[_MAX_EXT] = L"";

				_wsplitpath( SiSelf()->m_FileOS.GetPullFileName().c_str(), drive, dir, fname, ext);

				wcscat( FileName, fname);
				wcscat( FileName, ext);

				wcscat( PullDir, drive );
				wcscat( PullDir, dir );

				SiSelf()->DropFile( FileName, PullDir );
			}
		} break;

	case UI_BUT_RESET:
		{
			CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
			pSkinMesh->Reset();

			CX2ViewerMesh*	pMesh = (CX2ViewerMesh*)SiGetObject( OS_MESH );
			pMesh->Reset();

			SiSelf()->m_Param.Reset();

			SiSelf()->Init();
			SiSelf()->SetSkinMeshOnOff( false );
			SiSelf()->SetBoneOnOff( false );
			SiSelf()->SetEquipUIOnOff( false );
			SiSelf()->SetWeaponOnOff( false );
			SiSelf()->SetAccessoryOnOff( false );
			SiSelf()->SetAnimationOnOff( false );
			SiSelf()->SetMeshOnOff( false );
			SiSelf()->SetEffectButOnOff( false );
			SiSelf()->SetRenderParamOnOff( false );

			SiSelf()->m_vecFrameNameList.clear();
			SiSelf()->m_vecModifyFrameNameListBase.clear();

			SiMain()->OnResetDevice();
		} break;	

	case UI_BUT_UI_INIT:
		{
			SiSelf()->Init();
		} break;

	case UI_BUT_UI_HIDE:
		{
			g_bShowUI = !g_bShowUI;
			SiSelf()->SetShow( g_bShowUI );
		} break;

	case UI_CHECK_GRID:
		{
			bool bIsChecked = ((CDXUTCheckBox*)SiGetBaseDlg( UI_CHECK_GRID ))->GetChecked();
			CX2ViewerGrid*	pGrid = (CX2ViewerGrid*)SiGetObject( OS_GRID );
			pGrid->SetOnOff( bIsChecked );
		} break;

	case UI_CHECK_WIREFRAME:
		{
			bool bIsChecked = ((CDXUTCheckBox*)SiGetBaseDlg( UI_CHECK_WIREFRAME ))->GetChecked();
			CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
			pSkinMesh->SetWireFrameMode( bIsChecked );
		} break;

	case UI_EDIT_LIGHT_POS_X:
	case UI_EDIT_LIGHT_POS_Y:
	case UI_EDIT_LIGHT_POS_Z:
		{
			if( nEvent == EVENT_EDITBOX_STRING || nEvent == EVENT_EDITBOX_CHANGE )
			{
				CDXUTEditBox*		pEditBox = NULL;
				CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
				pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_LIGHT_POS_X );
				pSkinMesh->SetLightPosX( (float)_wtof( pEditBox->GetText() ) );
				pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_LIGHT_POS_Y );
				pSkinMesh->SetLightPosY( (float)_wtof( pEditBox->GetText() ) );
				pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_LIGHT_POS_Z );
				pSkinMesh->SetLightPosZ( (float)_wtof( pEditBox->GetText() ) );
			}
		} break;

	case UI_CHECK_LIGHT_ONOFF:
		{
			CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );

			bool bIsChecked = ((CDXUTCheckBox*)SiGetBaseDlg( UI_CHECK_LIGHT_ONOFF ))->GetChecked();
			
			if ( pSkinMesh )
				pSkinMesh->SetLightOnOff( bIsChecked );
		} break;

	case UI_RADIO_CAMERA_NORMAL:
		{
			CX2ViewerCamera*	pCamera = (CX2ViewerCamera*)SiGetObject( OS_CAMERA );
			
			if ( pCamera )
				pCamera->SetCameraMode( CX2ViewerCamera::CM_NORMAL );
		} break;

	case UI_RADIO_CAMERA_NAVIGATION:
		{
			CX2ViewerCamera* pCamera = (CX2ViewerCamera*)SiGetObject( OS_CAMERA );
			
			if ( pCamera )
				pCamera->SetCameraMode( CX2ViewerCamera::CM_NAVIGATION );
		} break;

	case UI_BUT_CAMERA_RESET:
		{
			CX2ViewerCamera* pCamera = reinterpret_cast<CX2ViewerCamera*>( SiGetObject( OS_CAMERA ) );
			
			if ( pCamera )
				pCamera->CameraReset();
		} break;

	case UI_EDIT_CAMERA_EYE_POS_X:
	case UI_EDIT_CAMERA_EYE_POS_Y:
	case UI_EDIT_CAMERA_EYE_POS_Z:
		{
			if( nEvent == EVENT_EDITBOX_STRING || nEvent == EVENT_EDITBOX_CHANGE )
			{
				CDXUTEditBox*		pEditBox = NULL;
				CX2ViewerCamera*	pCamera = reinterpret_cast<CX2ViewerCamera*>( SiGetObject( OS_CAMERA ) );
				if ( pCamera )
				{
					pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_CAMERA_EYE_POS_X );
					pCamera->SetEyeX( (float)_wtof( pEditBox->GetText() ) );
					pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_CAMERA_EYE_POS_Y );
					pCamera->SetEyeY( (float)_wtof( pEditBox->GetText() ) );
					pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_CAMERA_EYE_POS_Z );
					pCamera->SetEyeZ( (float)_wtof( pEditBox->GetText() ) );
				}
			}
		} break;

	case UI_EDIT_CAMERA_LOOK_POS_X:
	case UI_EDIT_CAMERA_LOOK_POS_Y:
	case UI_EDIT_CAMERA_LOOK_POS_Z:
		{
			if( nEvent == EVENT_EDITBOX_STRING || nEvent == EVENT_EDITBOX_CHANGE )
			{
				CDXUTEditBox*		pEditBox = NULL;
				CX2ViewerCamera*	pCamera = reinterpret_cast<CX2ViewerCamera*>( SiGetObject( OS_CAMERA ) );
				if ( pCamera )
				{
					pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_CAMERA_LOOK_POS_X );
					pCamera->SetLookVecX( (float)_wtof( pEditBox->GetText() ) );
					pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_CAMERA_LOOK_POS_Y );
					pCamera->SetLookVecY( (float)_wtof( pEditBox->GetText() ) );
					pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_CAMERA_LOOK_POS_Z );
					pCamera->SetLookVecZ( (float)_wtof( pEditBox->GetText() ) );
				}
			}
		} break;

	case UI_LIST_MODEL:
		{
			if ( nEvent == EVENT_LISTBOX_ITEM_DBLCLK )
			{
				UINT iIndex = reinterpret_cast<CDXUTListBox*>( pControl )->GetSelectedIndex();
				wstring strSelectItem = reinterpret_cast<CDXUTListBox*>( pControl )->GetSelectedItem( iIndex )->strText;

				SiSelf()->DropFile( strSelectItem.c_str(), L"" );
			}

			if ( nEvent == EVENT_LISTBOX_SELECTION )
			{
				UINT iIndex = reinterpret_cast<CDXUTListBox*>( pControl )->GetSelectedIndex();
				wstring strSelectItem = reinterpret_cast<CDXUTListBox*>( pControl )->GetSelectedItem( iIndex )->strText;
				CDXUTEditBox* pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBaseDlg( UI_EDIT_MODEL ) );

				if ( pEditBox )
					pEditBox->SetText( strSelectItem.c_str() );
			}
		} break;

	case UI_EDIT_BG_A:
	case UI_EDIT_BG_R:
	case UI_EDIT_BG_G:
	case UI_EDIT_BG_B:
		{
			if( nEvent == EVENT_EDITBOX_STRING )
			{
				CDXUTEditBox*	pEditBox = NULL;
				pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_BG_A );
				g_pKTDXApp->GetDGManager()->SetClearColorA( _wtoi( pEditBox->GetText() ) );
				pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_BG_R );
				g_pKTDXApp->GetDGManager()->SetClearColorR( _wtoi( pEditBox->GetText() ) );
				pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_BG_G );
				g_pKTDXApp->GetDGManager()->SetClearColorG( _wtoi( pEditBox->GetText() ) );
				pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_BG_B );
				g_pKTDXApp->GetDGManager()->SetClearColorB( _wtoi( pEditBox->GetText() ) );
			}
		}
		return;
/*
	case UI_BUT_WORLD_MESH:
		{
			CX2ViewerWorldMesh* pWorld = (CX2ViewerWorldMesh*)SiGetObject( OS_WORLD_MESH );
			if( SiSelf()->m_FileOS.FileOpen( L"X-file(*.x)\0*.x\0" ) == CX2ViewerFileOS::FS_XFILE )
			{
				pWorld->SetMesh( SiSelf()->m_FileOS.GetPullFileName().c_str() );
			}
		}
		return;


	case UI_BUT_WORLD_MESH_RESET:
		{
			CX2ViewerWorldMesh* pWorld = (CX2ViewerWorldMesh*)SiGetObject( OS_WORLD_MESH );
			pWorld->Reset();
		}
		return;

	case UI_EDIT_WORLD_MESH_X:
	case UI_EDIT_WORLD_MESH_Y:
	case UI_EDIT_WORLD_MESH_Z:
		{
			if( nEvent == EVENT_EDITBOX_STRING )
			{
				CX2ViewerWorldMesh* pWorld = (CX2ViewerWorldMesh*)SiGetObject( OS_WORLD_MESH );

				D3DXVECTOR3 vec3;
				CDXUTEditBox* pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_WORLD_MESH_X );
				vec3.x = (float)_wtof( pEditBox->GetText() );
				pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_WORLD_MESH_Y );
				vec3.y = (float)_wtof( pEditBox->GetText() );
				pEditBox = (CDXUTEditBox*)SiGetBaseDlg( UI_EDIT_WORLD_MESH_Z );
				vec3.z = (float)_wtof( pEditBox->GetText() );
				pWorld->GetMatrix()->Move( vec3 );
			}
		}
		return;

	case UI_BUT_EFFECT_SET:
		{
			SiSelf()->m_Param.SetEffect();

			switch( SiSelf()->m_MeshSel )
			{
			case MS_MESH:
				{
					CX2ViewerMesh*	pMesh = (CX2ViewerMesh*)SiGetObject( OS_MESH );
					SiSelf()->m_Param.GetRenderParam( pMesh->GetRenderParam(), pMesh->GetImpactData(), *(pMesh->GetTexStageData()) );
				}
				break;

			case MS_SKIN_MESH:
				{
					CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
					SiSelf()->m_Param.GetRenderParam( pSkinMesh->GetRenderParam(), pSkinMesh->GetImpactData(), *(pSkinMesh->GetTexStageData()) );
				}
				break;
			}

			SiSelf()->m_Param.SetParamDlg( &(SiSelf()->m_RenderParam) );
		}
		break;
		*/
	}
}
#pragma endregion Base UI Event

#pragma region OnGUISkinMeshEvent
void CALLBACK CX2ViewerUI::OnGUISkinMeshEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case UI_EDIT_SCALE_X:
	case UI_EDIT_SCALE_Y:
	case UI_EDIT_SCALE_Z:
		#pragma region EditBox Scale
		{
			if( nEvent == EVENT_EDITBOX_STRING || nEvent == EVENT_EDITBOX_CHANGE )
			{
				CDXUTEditBox*		pEditBox = NULL;
				CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
				pEditBox = (CDXUTEditBox*)SiGetSkinMeshDlg( UI_EDIT_SCALE_X );
				pSkinMesh->SetScaleX( static_cast<float>( _wtof( pEditBox->GetText() ) ) );
				pEditBox = (CDXUTEditBox*)SiGetSkinMeshDlg( UI_EDIT_SCALE_Y );
				pSkinMesh->SetScaleY( static_cast<float>( _wtof( pEditBox->GetText() ) ) );
				pEditBox = (CDXUTEditBox*)SiGetSkinMeshDlg( UI_EDIT_SCALE_Z );
				pSkinMesh->SetScaleZ( static_cast<float>( _wtof( pEditBox->GetText() ) ) );
			}
		} break;
		#pragma endregion case

	case UI_EDIT_POSITION_X:
	case UI_EDIT_POSITION_Y:
	case UI_EDIT_POSITION_Z:
		#pragma region EditBox Position
		{
			if( nEvent == EVENT_EDITBOX_STRING || nEvent == EVENT_EDITBOX_CHANGE )
			{
				CDXUTEditBox*		pEditBox = NULL;
				CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
				pEditBox = (CDXUTEditBox*)SiGetSkinMeshDlg( UI_EDIT_POSITION_X );
				pSkinMesh->SetPositionX( static_cast<float>(_wtof( pEditBox->GetText() ) ) );
				pEditBox = (CDXUTEditBox*)SiGetSkinMeshDlg( UI_EDIT_POSITION_Y );
				pSkinMesh->SetPositionY( static_cast<float>( _wtof( pEditBox->GetText() ) ) );
				pEditBox = (CDXUTEditBox*)SiGetSkinMeshDlg( UI_EDIT_POSITION_Z );
				pSkinMesh->SetPositionZ( static_cast<float>( _wtof( pEditBox->GetText() ) ) );
			}
		} break;
		#pragma endregion case

	case UI_LIST_BONE:
		#pragma region List Bone
		{
			if( nEvent == EVENT_LISTBOX_SELECTION )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetSkinMeshDlg( UI_LIST_BONE ) );
				CDXUTEditBox* pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetSkinMeshDlg( UI_EDIT_BONE ) );

				if ( pListBox == NULL )
					return;

				if ( pEditBox == NULL )
					return;

				int nIndex = pListBox->GetSelectedIndex( -1 );

				if ( 0 >= static_cast<int>(SiSelf()->m_vecFrameNameList.size()) )
					return;

				for(int i=0; i < static_cast<int>(SiSelf()->m_vecFrameNameList.size()); ++i)
				{
					CKTDXDeviceXSkinMesh::MultiAnimFrame *pMA = SiSelf()->GetFrameList(i);
					pMA->m_bSelected = false;
				}

				CKTDXDeviceXSkinMesh::MultiAnimFrame *pMA = SiSelf()->GetFrameList(nIndex); 
				pMA->m_bSelected = true;

				LPCSTR			szName;
				std::wstring	wstrName;
				WCHAR			wszName[128] = L"";

				szName = pMA->Name;
				MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);
				wstrName = wszName;

				pEditBox->SetText( wstrName.c_str() );

				CX2ViewerSkinMesh*	pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				pSkinMesh->SetAttachPoint( wszName );
			}

			if( nEvent == EVENT_LISTBOX_ITEM_DBLCLK )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetSkinMeshDlg( UI_LIST_BONE ) );

				if ( pListBox == NULL )
					return;

				int nIndex = pListBox->GetSelectedIndex();

				if ( 0 >= static_cast<int>(SiSelf()->m_vecFrameNameList.size()) )
					return;

				CKTDXDeviceXSkinMesh::MultiAnimFrame *pMA = SiSelf()->GetFrameList( nIndex ); 

				LPCSTR			szName;
				std::wstring	wstrName;
				WCHAR			wszName[128] = L"";

				szName = pMA->Name;
				MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);
				wstrName = wszName;

				BOOST_FOREACH( CKTDXDeviceXSkinMesh::MultiAnimFrame* pMAF, SiSelf()->m_vecModifyFrameNameListBase )
				{
					szName = pMAF->Name;
					MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);

					if ( wstrName.compare( wszName ) == 0 )
						return;
				}

				if ( wstrName.compare(L"Scene_Root") != 0 && wstrName.compare(L"") != 0 && wstrName.compare(L"<no_name>") != 0 )
				{
					SiSelf()->m_vecModifyFrameNameListBase.push_back( pMA );
					CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetSkinMeshDlg( UI_LIST_MODIFY_BONE ) );
					pListBox->AddItem( wstrName.c_str(), NULL );
				}
			}
		} break;
#pragma endregion case

	case UI_CHECK_BONE_MODIFY:
		#pragma region CheckBox Bone Modify
		{
			CDXUTCheckBox* pCheckbox = reinterpret_cast<CDXUTCheckBox*>( SiGetSkinMeshDlg( UI_CHECK_BONE_MODIFY ) );

			if ( pCheckbox )
			{
				SiSelf()->m_bModifyBone = pCheckbox->GetChecked();

				SiSelf()->SetBoneOnOff( SiSelf()->m_bModifyBone );

				if ( SiSelf()->m_bModifyBone == false )
				{
					CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetSkinMeshDlg( UI_LIST_MODIFY_BONE ) );
					CDXUTEditBox* pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetSkinMeshDlg( UI_EDIT_MODIFY_BONE ) );
					CX2ViewerSkinMesh*	pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
					SiSelf()->m_vecModifyFrameNameListBase.clear();
					pSkinMesh->SetUnitReForm( CI_NONE );
					pListBox->RemoveAllItems();
					pEditBox->ClearText();
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_X ) );
					pEditBox->ClearText();
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_Y ) );
					pEditBox->ClearText();
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_Z ) );
					pEditBox->ClearText();
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_POS_Y ) );
					pEditBox->ClearText();
				}

			}
		} break;
		#pragma endregion case

	case UI_LIST_MODIFY_BONE:
		#pragma region List Modify Bone
		{
			if( nEvent == EVENT_LISTBOX_SELECTION )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetSkinMeshDlg( UI_LIST_MODIFY_BONE ) );
				CDXUTEditBox* pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetSkinMeshDlg( UI_EDIT_MODIFY_BONE ) );

				if ( pListBox == NULL )
					return;

				if ( pEditBox == NULL )
					return;

				int nIndex = pListBox->GetSelectedIndex( -1 );

				if ( 0 >= static_cast<int>( SiSelf()->m_vecModifyFrameNameListBase.size() ) )
					return;

				for ( int i=0; i < static_cast<int>( SiSelf()->m_vecModifyFrameNameListBase.size() ); ++i )
				{
					CKTDXDeviceXSkinMesh::MultiAnimFrame *pMA = SiSelf()->GetFrameList(i);
					pMA->m_bSelected = false;
				}

				CKTDXDeviceXSkinMesh::MultiAnimFrame *pMA = SiSelf()->m_vecModifyFrameNameListBase[nIndex];
				pMA->m_bSelected = true;
				
				LPCSTR			szName;
				std::wstring	wstrName;
				WCHAR			wszName[128] = L"";

				szName = pMA->Name;
				MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);
				wstrName = wszName;

				pEditBox->SetText( wstrName.c_str() );

				CX2ViewerSkinMesh*	pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				pSkinMesh->SetAttachPoint( wszName );

				swprintf( wszName, L"%.2f", pMA->m_vScaleRel.x );
				reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_X ) )->SetText( wszName );
				swprintf( wszName, L"%.2f", pMA->m_vScaleRel.y );
				reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_Y ) )->SetText( wszName );
				swprintf( wszName, L"%.2f", pMA->m_vScaleRel.z );
				reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_Z ) )->SetText( wszName );
				swprintf( wszName, L"%.2f", pMA->m_fMoveBoneY );
				reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_POS_Y ) )->SetText( wszName );
			}

			if( nEvent == EVENT_LISTBOX_ITEM_DBLCLK )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetSkinMeshDlg( UI_LIST_MODIFY_BONE ) );
				CDXUTEditBox* pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetSkinMeshDlg( UI_EDIT_MODIFY_BONE ) );

				if ( pListBox == NULL )
					return;

				if ( pEditBox == NULL )
					return;

				int nIndex = pListBox->GetSelectedIndex( -1 );

				LPCSTR			szName;
				std::wstring	wstrName;
				WCHAR			wszName[128] = L"";

				BOOST_FOREACH( CKTDXDeviceXSkinMesh::MultiAnimFrame* pMAF, SiSelf()->m_vecModifyFrameNameListBase )
				{
					szName = pMAF->Name;
					MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);
					wstrName = wszName;

					if ( wstrName.compare( pListBox->GetItem( nIndex )->strText ) == 0 )
					{
						VecMultiAnimFrame::iterator iter;

						iter = std::find( SiSelf()->m_vecModifyFrameNameListBase.begin(), SiSelf()->m_vecModifyFrameNameListBase.end(), pMAF );

						if ( iter != SiSelf()->m_vecModifyFrameNameListBase.end() )
							SiSelf()->m_vecModifyFrameNameListBase.erase( iter-- );

						pListBox->RemoveItem( nIndex );
						pEditBox->ClearText();
					}
				}
			}
		} break;
		#pragma endregion case

	case UI_COMBO_DEFAULT_BONE:
		#pragma region Combo Default Bone
		{
			if( nEvent == EVENT_COMBOBOX_SELECTION_CHANGED )
			{
				CDXUTComboBox* pComboBox = reinterpret_cast<CDXUTComboBox*>( SiGetSkinMeshDlg( UI_COMBO_DEFAULT_BONE ) );
				
				if ( pComboBox && SiSelf()->m_bModifyBone )
				{
					CX2ViewerSkinMesh*	pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
					UINT iIndex = pComboBox->GetSelectedIndex();
					CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetSkinMeshDlg( UI_LIST_MODIFY_BONE ) );
					CDXUTEditBox* pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetSkinMeshDlg( UI_EDIT_MODIFY_BONE ) );
					
					SiSelf()->m_vecModifyFrameNameListBase.clear();
					
					pListBox->RemoveAllItems();
					pEditBox->ClearText();
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_X ) );
					pEditBox->ClearText();
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_Y ) );
					pEditBox->ClearText();
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_Z ) );
					pEditBox->ClearText();
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_POS_Y ) );
					pEditBox->ClearText();

					pSkinMesh->SetUnitReForm( CI_NONE );
					pSkinMesh->SetUnitReForm( static_cast<CHARACTER_ID>( iIndex ) );
					pSkinMesh->GetModifyBoneList( static_cast<CHARACTER_ID>( iIndex ), SiSelf()->m_vecModifyFrameNameListBase );

					LPCSTR			szName;
					std::wstring	wstrName;
					WCHAR			wszName[128] = L"";

					BOOST_FOREACH( CKTDXDeviceXSkinMesh::MultiAnimFrame* pMAF, SiSelf()->m_vecModifyFrameNameListBase )
					{
						szName = pMAF->Name;
						MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);
						wstrName = wszName;

						CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetSkinMeshDlg( UI_LIST_MODIFY_BONE ) );
						pListBox->AddItem( wstrName.c_str(), NULL );
					}
				}
			}
		} break;
		#pragma endregion case

	case UI_BUT_OPEN_BONE:
		#pragma region Bone Open
		{
			WCHAR wstrDir[256] = L"";
			wcscpy( wstrDir, SiSelf()->m_wstrRootDir );
			WCHAR wstrAddDir[256] = L"\\dat\\System\\Bone";
			wcscat( wstrDir, wstrAddDir );
			SetCurrentDirectory( wstrDir );

			if( SiSelf()->m_FileOS.FileOpen( L"Lua-file(*.lua)\0*.lua\0" ) == CX2ViewerFileOS::FS_LUA )
			{
				WCHAR FileName[256] = L"";
				WCHAR PullDir[MAX_PATH] = L"";
				WCHAR drive[_MAX_DRIVE] = L"";
				WCHAR dir[_MAX_DIR] = L"";
				WCHAR fname[_MAX_FNAME] = L"";
				WCHAR ext[_MAX_EXT] = L"";

				_wsplitpath( SiSelf()->m_FileOS.GetPullFileName().c_str(), drive, dir, fname, ext);

				wcscat( FileName, fname);
				wcscat( FileName, ext);

				wcscat( PullDir, drive );
				wcscat( PullDir, dir );

				CX2ViewerSkinMesh*	pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				
				pSkinMesh->OpenScriptFile( FileName, g_pKTDXApp->GetLuaBinder() );
			}
		} break;
		#pragma endregion case

	case UI_BUT_SAVE_BONE:
		#pragma region Scene Save
		{
			SiSelf()->SaveBoneFile();
		} break;
		#pragma endregion case

	case UI_CHECK_MOTION_ONOFF:
		#pragma region Check Motion OnOff
		{
			CX2ViewerSkinMesh*	pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
			CDXUTCheckBox* pCheckbox = reinterpret_cast<CDXUTCheckBox*>( SiGetSkinMeshDlg( UI_CHECK_MOTION_ONOFF ) );

			pSkinMesh->SetMotionOnOff( pCheckbox->GetChecked() );
		} break;
		#pragma endregion case

	case UI_CHECK_BOUNDING:
		#pragma region Check Bounding
		{
			CX2ViewerSkinMesh*	pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
			CDXUTCheckBox* pCheckbox = reinterpret_cast<CDXUTCheckBox*>( SiGetSkinMeshDlg( UI_CHECK_BOUNDING ) );

			pSkinMesh->SetBounding( pCheckbox->GetChecked() );
		} break;
		#pragma endregion case

	case UI_CHECK_ATTACK_BOX:
		#pragma region Check Attack Box
		{
			CX2ViewerSkinMesh*	pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
			CDXUTCheckBox* pCheckbox = reinterpret_cast<CDXUTCheckBox*>( SiGetSkinMeshDlg( UI_CHECK_ATTACK_BOX ) );

			pSkinMesh->SetShowAttackBox( pCheckbox->GetChecked() );
		} break;
		#pragma endregion case
		    
	case UI_BUT_RENDER_PARAM:
		{
			SiSelf()->SetRenderParamOnOff( !(SiSelf()->m_RenderParamUI.GetVisible()) );
		} break;
	/*
	case UI_BUT_ANIM_NAME_CHANGE:
		{
			if( SiMain()->GetSelectedAnimIndex() == -1 )
				return;

			//MessageBox( g_pKTDXApp->GetHWND(), L"훈형을 갈궈요..ㅎ", L"미구현", MB_OK );

			int animIndex = SiMain()->GetSelectedAnimIndex();

			CX2ViewerSkinMesh* pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );

			LPD3DXANIMATIONSET pAnimSet = pSkinMesh->GetXSkinAnim()->GetAnimSet( animIndex );
			LPSTR pAnimSetName = (LPSTR)pAnimSet->GetName();
			
			CDXUTEditBox*		pEditBox = NULL;
			pEditBox = (CDXUTEditBox*)SiGetSkinMeshDlg( UI_EDIT_ANIM_NAME );

			string changeAnimName;
			ConvertWCHARToChar( changeAnimName, pEditBox->GetText() );
			sprintf( pAnimSetName, changeAnimName.c_str() );

			
			HRESULT			hr = S_OK;
			LPD3DXMESH		pMeshMerged = NULL;
			LPD3DXBUFFER	pbufAdjacencyMerged = NULL;
			LPD3DXBUFFER	pbufMaterialsMerged = NULL;

			BOOL bSaveHierarchy = TRUE;
			DWORD xFormat = D3DXF_FILEFORMAT_COMPRESSED;

			//OPENFILENAME m_OFN;

			WCHAR animFileName[255] = {0,};

			wcscpy(animFileName, SiMain()->GetAnimFileName().c_str());

			WCHAR animDirName[255] = {0};
			wcscpy(animDirName, SiMain()->GetAnimDirName().c_str());


			WCHAR FullPath[255] = {0};
			WCHAR* pFullPath = NULL;
			
			pFullPath = wcscat( animDirName, animFileName );

			wcscpy( FullPath, pFullPath );

			CKTDXDeviceXSkinMesh* pMotion = NULL;
			pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( SiMain()->GetAnimFileName() );

			if ( pMotion == NULL )
			{
				MessageBox(g_pKTDXApp->GetHWND(), L"모션 XSkinMesh 로드 에러", L"에러", MB_OK );
				return;
			}


			LPD3DXANIMATIONCONTROLLER pAC = pMotion->GetCloneAC();

			if( strcmp( pMotion->GetFrameRoot()->Name, "<no_name>" ) == 0 )
			{
				hr = D3DXSaveMeshHierarchyToFile(FullPath,
					xFormat,
					(LPD3DXFRAME) pMotion->GetFrameRoot()->pFrameFirstChild,
					pAC,
					NULL);

				SAFE_RELEASE( pAC );

				if (FAILED(hr))
				{
					MessageBox( NULL, L"에러", L"세이브 확인", MB_OK);
					return;
				}
			}
			else
			{
				hr = D3DXSaveMeshHierarchyToFile(FullPath,
					xFormat,
					(LPD3DXFRAME) pMotion->GetFrameRoot(),
					pAC,
					NULL);

				SAFE_RELEASE( pAC );

				if (FAILED(hr))
				{
					MessageBox( NULL, L"에러", L"세이브 확인", MB_OK);
					return;
				}
			}

			DropAnimationFile( SiSelf(), animFileName, animDirName );

			pSkinMesh->ChangeAnim( animIndex );
		} break;

*/
	}
}
#pragma endregion SkinMesh UI Event

#pragma region OnGUIBoneEvent
void CALLBACK CX2ViewerUI::OnGUIBoneEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case UI_EDIT_BONE_SCALE_X:
	case UI_EDIT_BONE_SCALE_Y:
	case UI_EDIT_BONE_SCALE_Z:
		#pragma region EditBox Scale
		{
			if( nEvent == EVENT_EDITBOX_STRING || nEvent == EVENT_EDITBOX_CHANGE )
			{
				CDXUTEditBox*		pEditBox = NULL;
				CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
				wstring strBoneName = reinterpret_cast<CDXUTEditBox*>( SiGetSkinMeshDlg( UI_EDIT_MODIFY_BONE ) )->GetText();
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = pSkinMesh->GetXSkinAnim()->GetCloneFrame( strBoneName.c_str() );
				D3DXVECTOR3 vScale( 1.f, 1.f, 1.f );

				pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_X ) );
				vScale.x = static_cast<float>( _wtof( pEditBox->GetText() ) );
				pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_Y ) );
				vScale.y = static_cast<float>( _wtof( pEditBox->GetText() ) );
				pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_SCALE_Z ) );
				vScale.z = static_cast<float>( _wtof( pEditBox->GetText() ) );

				SetFrameScale( &vScale, true, pFrame );
			}
		} break;
		#pragma endregion case

	case UI_EDIT_BONE_POS_Y:
		#pragma region EditBox Position
		{
			if( nEvent == EVENT_EDITBOX_STRING || nEvent == EVENT_EDITBOX_CHANGE )
			{
				CDXUTEditBox*		pEditBox = NULL;
				CX2ViewerSkinMesh*	pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetBoneDlg( UI_EDIT_BONE_POS_Y ) );
				wstring strBoneName = reinterpret_cast<CDXUTEditBox*>( SiGetSkinMeshDlg( UI_EDIT_MODIFY_BONE ) )->GetText();
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = pSkinMesh->GetXSkinAnim()->GetCloneFrame( strBoneName.c_str() );
				float fBoneMoveY = static_cast<float>( _wtof( pEditBox->GetText() ) );

				SetFrameMoveBoneY( pFrame, fBoneMoveY );
			}
		} break;
		#pragma endregion case
	}
}
#pragma endregion Bone UI Event

#pragma region OnGUIEquipEvent
void CALLBACK CX2ViewerUI::OnGUIEquipEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case UI_LIST_EQUIP:
		#pragma region Equip List
		{
			if( nEvent == EVENT_LISTBOX_ITEM_DBLCLK )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_EQUIP ) );
				int nIndex = pListBox->GetSelectedIndex( -1 );

				CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				if( pSkinMesh->DelModelXSkinMesh( pListBox->GetItem( nIndex )->strText ) == true )
				{
					pListBox->RemoveItem( nIndex );
				}
				else
				{
					WARNINGMSG( L"Object 삭제실패..?" );
				}
			}

		} break;
		#pragma endregion case

	case UI_BUT_CLEAR_EQUIP_LIST:
		#pragma region Clear Equip List
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_EQUIP ) );
				int iSize = pListBox->GetSize();

				for ( int i = 0; i < iSize; ++i )
				{
					CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
					if( pSkinMesh->DelModelXSkinMesh( pListBox->GetItem( 0 )->strText ) == true )
					{
						pListBox->RemoveItem( 0 );
					}
					else
					{
						WARNINGMSG( L"Object 삭제실패..?" );
					}
				}				
			} break;
		#pragma endregion case

	case UI_BUT_OPEN_EQUIP:
		#pragma region Open Equips
			{
				SiSelf()->OpenEquip();
			} break;
		#pragma endregion case

	case UI_LIST_WEAPON:
		#pragma region Weapon List
		{
			if( nEvent == EVENT_LISTBOX_ITEM_DBLCLK )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_WEAPON ) );
				int nIndex = pListBox->GetSelectedIndex( -1 );

				CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				if( pSkinMesh->DelModelXSkinAnim( pListBox->GetItem( nIndex )->strText ) == true &&
					pSkinMesh->DelModelXSkinMesh( pListBox->GetItem( nIndex )->strText ) == true )
				{
					pListBox->RemoveItem( nIndex );
					SiSelf()->SetWeaponOnOff( false );
				}
				else
				{
					WARNINGMSG( L"Object 삭제실패..?" );
				}
			}

			if ( nEvent == EVENT_LISTBOX_SELECTION )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_WEAPON ) );
				CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );

				if ( pListBox == NULL )
					return;

				if ( pSkinMesh == NULL )
					return;

				if ( pListBox->GetSize() > 0 )
				{
					int nIndex = pListBox->GetSelectedIndex( -1 );
					if ( nIndex == - 1 )
						return;

					SWeaponInfo* pWeaponInfo = pSkinMesh->GetWeaponInfo( pListBox->GetItem( nIndex )->strText );

					if ( pWeaponInfo )
					{
						WCHAR buf[256] = {0};

						reinterpret_cast<CDXUTEditBox*>( SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_NAME ) )->SetText( pWeaponInfo->m_BoneName.c_str() );
						swprintf( buf, L"%.2f", pWeaponInfo->m_vRotate.x );
						reinterpret_cast<CDXUTEditBox*>( SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_X ) )->SetText( buf );
						swprintf( buf, L"%.2f", pWeaponInfo->m_vRotate.y );
						reinterpret_cast<CDXUTEditBox*>( SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_Y ) )->SetText( buf );
						swprintf( buf, L"%.2f", pWeaponInfo->m_vRotate.z );
						reinterpret_cast<CDXUTEditBox*>( SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_Z ) )->SetText( buf );
						SiSelf()->SetWeaponOnOff( true );
					}
				}
			}
		} break;
		#pragma endregion case

	case UI_BUT_CLEAR_WEAPON_LIST:
		#pragma region Clear Weapon List
		{
			CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_WEAPON ) );
			int iSize = pListBox->GetSize();

			for ( int i = 0; i < iSize; ++i )
			{
				CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				if( pSkinMesh->DelModelXSkinAnim( pListBox->GetItem( 0 )->strText ) == true &&
					pSkinMesh->DelModelXSkinMesh( pListBox->GetItem( 0 )->strText ) == true )
				{
					pListBox->RemoveItem( 0 );
					SiSelf()->m_bWeaponUI = false;
				}
				else
				{
					WARNINGMSG( L"Object 삭제실패..?" );
				}
			}

			SiSelf()->SetWeaponOnOff( false );
		} break;
		#pragma endregion case

	case UI_BUT_OPEN_WEAPON:
		#pragma region Open Weapon
		{
			SiSelf()->OpenWeapon();
		} break;
		#pragma endregion case

	case UI_LIST_ACCESSORY:
		#pragma region Accessory List
		{
			if( nEvent == EVENT_LISTBOX_ITEM_DBLCLK )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_ACCESSORY ) );
				int nIndex = pListBox->GetSelectedIndex( -1 );

				CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				if( pSkinMesh->DelModelXMesh( pListBox->GetItem( nIndex )->strText ) == true )
				{
					pListBox->RemoveItem( nIndex );
					SiSelf()->SetAccessoryOnOff( false );
				}
				else
				{
					WARNINGMSG( L"Object 삭제실패..?" );
				}
			}

			if ( nEvent == EVENT_LISTBOX_SELECTION )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_ACCESSORY ) );
				CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );

				if ( pListBox == NULL )
					return;

				if ( pSkinMesh == NULL )
					return;

				if ( pListBox->GetSize() > 0 )
				{
					int nIndex = pListBox->GetSelectedIndex( -1 );

					if ( nIndex == - 1 )
						return;

					SAccessoryInfo* pAccessoryInfo = pSkinMesh->GetAccessoryInfo( pListBox->GetItem( nIndex )->strText );

					if ( pAccessoryInfo )
					{
						WCHAR buf[256] = {0};

						reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_BONE_NAME ) )->SetText( pAccessoryInfo->m_BoneName.c_str() );
						
						swprintf( buf, L"%.2f", pAccessoryInfo->m_vMove.x );
						reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_X ) )->SetText( buf );
						swprintf( buf, L"%.2f", pAccessoryInfo->m_vMove.y );
						reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_Y ) )->SetText( buf );
						swprintf( buf, L"%.2f", pAccessoryInfo->m_vMove.z );
						reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_Z ) )->SetText( buf );

						swprintf( buf, L"%.2f", pAccessoryInfo->m_vRotate.x );
						reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_X ) )->SetText( buf );
						swprintf( buf, L"%.2f", pAccessoryInfo->m_vRotate.y );
						reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_Y ) )->SetText( buf );
						swprintf( buf, L"%.2f", pAccessoryInfo->m_vRotate.z );
						reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_Z ) )->SetText( buf );

						swprintf( buf, L"%.2f", pAccessoryInfo->m_vScale.x );
						reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_X ) )->SetText( buf );
						swprintf( buf, L"%.2f", pAccessoryInfo->m_vScale.y );
						reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_Y ) )->SetText( buf );
						swprintf( buf, L"%.2f", pAccessoryInfo->m_vScale.z );
						reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_Z ) )->SetText( buf );

						SiSelf()->SetAccessoryOnOff( true );
					}
				}
			}
		} break;
		#pragma endregion case

	case UI_BUT_CLEAR_ACCESSORY_LIST:
		#pragma region Clear Accessory List
		{
			CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_ACCESSORY ) );
			int iSize = pListBox->GetSize();

			for ( int i = 0; i < iSize; ++i )
			{
				CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				if ( pSkinMesh->DelModelXMesh( pListBox->GetItem( 0 )->strText ) == true )
				{
					pListBox->RemoveItem( 0 );
					SiSelf()->SetAccessoryOnOff( false );
				}
				else
				{
					WARNINGMSG( L"Object 삭제실패..?" );
				}
			}

			//SiSelf()->SetWeaponOnOff( false );
		} break;
		#pragma endregion case

	case UI_BUT_OPEN_ACCESSORY:
		#pragma region Open Accessory
		{
			SiSelf()->OpenAccessory();
		} break;
		#pragma endregion case

	case UI_LIST_ATTACH_BONES:
		#pragma region Attach Bones List
		{
			if ( nEvent == EVENT_LISTBOX_SELECTION )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_ATTACH_BONES ) );
				
				if ( pListBox == NULL )
					return;
								
				if ( pListBox->GetSize() > 0 )
				{
					int nIndex = pListBox->GetSelectedIndex( -1 );
					wstring strText = pListBox->GetSelectedItem( nIndex )->strText;
					WCHAR *pMem;

					if ( OpenClipboard( NULL ) )
					{
						HGLOBAL hMem;

						hMem = GlobalAlloc( GHND | GMEM_DDESHARE, MAX_PATH );

						if ( hMem )
						{
							pMem = (WCHAR *)GlobalLock( hMem );
							wcscpy( pMem, strText.c_str() );
							GlobalUnlock( hMem );
							EmptyClipboard();
							SetClipboardData( CF_UNICODETEXT, hMem );
						}
					}
				}
			}
		} break;
		#pragma endregion case

	case UI_BUT_SCENE_OPEN:
		#pragma region Scene Open
		{
			WCHAR wstrDir[256] = L"";
			wcscpy( wstrDir, SiSelf()->m_wstrRootDir );
			WCHAR wstrAddDir[256] = L"\\dat\\System\\Scene";
			wcscat( wstrDir, wstrAddDir );
			SetCurrentDirectory( wstrDir );

			if( SiSelf()->m_FileOS.FileOpen( L"Lua-file(*.lua)\0*.lua\0" ) == CX2ViewerFileOS::FS_LUA )
			{
				WCHAR FileName[256] = L"";
				WCHAR PullDir[MAX_PATH] = L"";
				WCHAR drive[_MAX_DRIVE] = L"";
				WCHAR dir[_MAX_DIR] = L"";
				WCHAR fname[_MAX_FNAME] = L"";
				WCHAR ext[_MAX_EXT] = L"";

				_wsplitpath( SiSelf()->m_FileOS.GetPullFileName().c_str(), drive, dir, fname, ext);

				wcscat( FileName, fname);
				wcscat( FileName, ext);

				wcscat( PullDir, drive );
				wcscat( PullDir, dir );
				
				SiSelf()->OpenScriptFile( FileName, g_pKTDXApp->GetLuaBinder() );
			}
		} break;
		#pragma endregion case

	case UI_BUT_SCENE_SAVE:
		#pragma region Scene Save
		{
			SiSelf()->SaveScene();
		} break;
		#pragma endregion case
	}
}
#pragma endregion Equip UI Event

#pragma region OnGUIWeaponEvent
void CALLBACK CX2ViewerUI::OnGUIWeaponEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case UI_EDIT_WEAPON_BONE_NAME:
		#pragma region Bone Name
		{
			if ( nEvent == EVENT_EDITBOX_STRING )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_WEAPON ) );
				int nIndex = pListBox->GetSelectedIndex( -1 );
				CDXUTEditBox* pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_NAME ) );
				CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				if ( pListBox->GetSize() > 0 && nIndex > -1 )
				{
					SWeaponInfo* pWeaponInfo = pSkinMesh->GetWeaponInfo( pListBox->GetItem( nIndex )->strText );

					CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
					pFrame = pSkinMesh->GetXSkinAnim()->GetCloneFrame( pEditBox->GetText() );

					if ( pFrame )
					{
						pWeaponInfo->m_BoneName = pEditBox->GetText();
						pWeaponInfo->m_pMatrix = &(pFrame->combineMatrix);
					}
					else
					{
						WARNINGMSG( L"본 이름이 정확하지 않아요!" );
					}
				}
			}
		} break;
		#pragma endregion case

	case UI_EDIT_WEAPON_BONE_ROT_X:
	case UI_EDIT_WEAPON_BONE_ROT_Y:
	case UI_EDIT_WEAPON_BONE_ROT_Z:
		#pragma region Rotate
		{
			if ( nEvent == EVENT_EDITBOX_STRING || nEvent == EVENT_EDITBOX_CHANGE )
			{
				CDXUTEditBox* pEditBox = NULL;
				CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_WEAPON ) );
				int nIndex = pListBox->GetSelectedIndex( -1 );

				if ( pListBox->GetSize() > 0 )
				{
					SWeaponInfo* pWeaponInfo = pSkinMesh->GetWeaponInfo( pListBox->GetItem( nIndex )->strText );

					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_X ) );
					pWeaponInfo->m_vRotate.x = static_cast<float>( _wtof( pEditBox->GetText() ) );
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_X ) );
					pWeaponInfo->m_vRotate.y = static_cast<float>( _wtof( pEditBox->GetText() ) );
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetWeaponDlg( UI_EDIT_WEAPON_BONE_ROT_X ) );
					pWeaponInfo->m_vRotate.z = static_cast<float>( _wtof( pEditBox->GetText() ) );
				}
			}
		} break;
		#pragma endregion case
	}
}
#pragma endregion Weapon UI Event

#pragma region OnGUIAccessoryEvent
void CALLBACK CX2ViewerUI::OnGUIAccessoryEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case UI_EDIT_ACCESSORY_BONE_NAME:
		#pragma region Bone Name
		{
			if ( nEvent == EVENT_EDITBOX_STRING )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_ACCESSORY ) );
				int nIndex = pListBox->GetSelectedIndex( -1 );
				CDXUTEditBox* pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_BONE_NAME ) );
				CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				if ( pListBox->GetSize() > 0 && nIndex > -1 )
				{
					SAccessoryInfo* pAccessoryInfo = pSkinMesh->GetAccessoryInfo( pListBox->GetItem( nIndex )->strText );

					CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
					pFrame = pSkinMesh->GetXSkinAnim()->GetCloneFrame( pEditBox->GetText() );

					if ( pFrame )
					{
						pAccessoryInfo->m_BoneName = pEditBox->GetText();
						pAccessoryInfo->m_pMatrix = &(pFrame->combineMatrix);
					}
					else
					{
						WARNINGMSG( L"본 이름이 정확하지 않아요!" );
					}
				}
			}
		} break;
		#pragma endregion case

	case UI_EDIT_ACCESSORY_MOVE_X:
	case UI_EDIT_ACCESSORY_MOVE_Y:
	case UI_EDIT_ACCESSORY_MOVE_Z:
	case UI_EDIT_ACCESSORY_ROT_X:
	case UI_EDIT_ACCESSORY_ROT_Y:
	case UI_EDIT_ACCESSORY_ROT_Z:
	case UI_EDIT_ACCESSORY_SCALE_X:
	case UI_EDIT_ACCESSORY_SCALE_Y:
	case UI_EDIT_ACCESSORY_SCALE_Z:
		#pragma region Move, Rotate, Scale
		{
			if ( nEvent == EVENT_EDITBOX_STRING || nEvent == EVENT_EDITBOX_CHANGE )
			{
				CDXUTEditBox* pEditBox = NULL;
				CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_ACCESSORY ) );
				int nIndex = pListBox->GetSelectedIndex( -1 );

				if ( pListBox->GetSize() > 0 )
				{
					SAccessoryInfo* pAccessoryInfo = pSkinMesh->GetAccessoryInfo( pListBox->GetItem( nIndex )->strText );

					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_X ) );
					pAccessoryInfo->m_vMove.x = static_cast<float>( _wtof( pEditBox->GetText() ) );
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_Y ) );
					pAccessoryInfo->m_vMove.y = static_cast<float>( _wtof( pEditBox->GetText() ) );
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_MOVE_Z ) );
					pAccessoryInfo->m_vMove.z = static_cast<float>( _wtof( pEditBox->GetText() ) );

					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_X ) );
					pAccessoryInfo->m_vRotate.x = static_cast<float>( _wtof( pEditBox->GetText() ) );
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_Y ) );
					pAccessoryInfo->m_vRotate.y = static_cast<float>( _wtof( pEditBox->GetText() ) );
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_ROT_Z ) );
					pAccessoryInfo->m_vRotate.z = static_cast<float>( _wtof( pEditBox->GetText() ) );

					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_X ) );
					pAccessoryInfo->m_vScale.x = static_cast<float>( _wtof( pEditBox->GetText() ) );
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_Y ) );
					pAccessoryInfo->m_vScale.y = static_cast<float>( _wtof( pEditBox->GetText() ) );
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAccessoryDlg( UI_EDIT_ACCESSORY_SCALE_Z ) );
					pAccessoryInfo->m_vScale.z = static_cast<float>( _wtof( pEditBox->GetText() ) );
				}
			}
		} break;
		#pragma endregion case
	}
}
#pragma endregion Accessory UI Event

#pragma region OnGUIAnimationEvent
void CALLBACK CX2ViewerUI::OnGUIAnimationEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case UI_BUT_ANIM_PLAY_PAUSE:
		#pragma region Play / Pause
		{
			CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
			CDXUTButton* pButton = reinterpret_cast<CDXUTButton*>( SiGetAnimtaionDlg( UI_BUT_ANIM_PLAY_PAUSE ) );
			if( pSkinMesh->SetAnimPlayPause() == true )
				pButton->SetText( L"||" );
			else
				pButton->SetText( L"▶" );
		} break;
		#pragma endregion case

	case UI_BUT_ANIM_STOP:
		#pragma region Stop
		{
			CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
			CDXUTButton* pButton = reinterpret_cast<CDXUTButton*>( SiGetAnimtaionDlg( UI_BUT_ANIM_PLAY_PAUSE ) );

			if ( pSkinMesh )
			{
				pSkinMesh->SetAnimStop();
				pButton->SetText( L"▶" );
			}
		} break;
		#pragma endregion case

	case UI_BUT_ANIM_PREV_FRAME:
		#pragma region Prev Frame
		{
			CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );

			if( NULL != pSkinMesh )
			{
				float fNowTime, fMaxTime;
				pSkinMesh->GetAnimTime( fNowTime, fMaxTime );
				fNowTime = fNowTime - SiSelf()->GetAnimTimeInc();
				if( fNowTime < 0.f )
					fNowTime = 0.f;
				else if( fNowTime > fMaxTime )
					fNowTime = fMaxTime;

				pSkinMesh->SetAnimTime( fNowTime );
			}
		} break;
		#pragma endregion case

	case UI_BUT_ANIM_NEXT_FRAME:
		#pragma region Next Frame
		{
			CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
			if( NULL != pSkinMesh )
			{
				float fNowTime, fMaxTime;
				pSkinMesh->GetAnimTime( fNowTime, fMaxTime );
				fNowTime = fNowTime + SiSelf()->GetAnimTimeInc();
				if( fNowTime < 0.f )
					fNowTime = 0.f;
				else if( fNowTime > fMaxTime )
					fNowTime = fMaxTime;

				pSkinMesh->SetAnimTime( fNowTime );
			}
		} break;
		#pragma endregion case

	case UI_EDIT_ANIM_FRAME_TIME_INC:
		#pragma region One Frame Value
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					CDXUTEditBox*	pEditBox = NULL;
					pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAnimtaionDlg( UI_EDIT_ANIM_FRAME_TIME_INC ) );

					float fValue = static_cast<float>( _wtof( pEditBox->GetText() ) );
					if( fValue <= 0.f )
					{
						fValue= SiSelf()->GetAnimTimeInc();
					}

					pEditBox->SetTextFloatArray( &fValue, 1 );
					SiSelf()->SetAnimTimeInc( fValue );

				} break;
			}
		} break;
		#pragma endregion case

	case UI_SLIDE_ANIM_SPEED:
		#pragma region Anim Speed
		{
			CX2ViewerSkinMesh*	pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
			CDXUTSlider* pSlider = reinterpret_cast<CDXUTSlider*>( SiGetAnimtaionDlg( UI_SLIDE_ANIM_SPEED ) );
			int nValue = pSlider->GetValue();

			pSkinMesh->SetPlaySpeed( nValue * 0.001f );

			//Static 문구 출력
			CDXUTStatic* pStatic = reinterpret_cast<CDXUTStatic*>( SiGetAnimtaionDlg( UI_STATIC_ANIM_SPEED ) );
			WCHAR wszSpeed[128] = L"";
			swprintf( wszSpeed, L"speed : %d", nValue );
			pStatic->SetText( wszSpeed );
		} break;
		#pragma endregion case

	case UI_COMBO_PLAY_TYPE:
		#pragma region Play Type
		{
			if( nEvent == EVENT_COMBOBOX_SELECTION_CHANGED )
			{
				CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
				CDXUTComboBox* pComboBox = reinterpret_cast<CDXUTComboBox*>( SiGetAnimtaionDlg( UI_COMBO_PLAY_TYPE ) );
				DXUTComboBoxItem* pItem = pComboBox->GetSelectedItem();
				pSkinMesh->SetPlayType( pItem->strText );
			}
		} break;
		#pragma endregion case

	case UI_LIST_ANIMATION:
		#pragma region Animation List
		{
			if( nEvent == EVENT_LISTBOX_SELECTION )
			{
				CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetAnimtaionDlg( UI_LIST_ANIMATION ) );
				int nIndex = pListBox->GetSelectedIndex( -1 );

				CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );

				if ( NULL != pSkinMesh )
				{
					pSkinMesh->ChangeAnim( pListBox->GetItem( nIndex )->strText );

					CDXUTEditBox* pEditBox = reinterpret_cast<CDXUTEditBox*>( SiGetAnimtaionDlg( UI_EDIT_ANIM_NAME ) );
					pEditBox->SetText(  pListBox->GetItem( nIndex )->strText );

					SiMain()->SetSelectedAnimIndex( nIndex );

					CDXUTButton* pButton = reinterpret_cast<CDXUTButton*>( SiGetAnimtaionDlg( UI_BUT_ANIM_PLAY_PAUSE ) );
					pButton->SetText( L"||" );
					
				}
			}
		} break;
		#pragma endregion case

	case UI_SLIDE_ANIM_FRAME:
		#pragma region Anim Frame Slider
		{
			CX2ViewerSkinMesh*	pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
			CDXUTSlider* pSlider = reinterpret_cast<CDXUTSlider*>( SiGetAnimtaionDlg( UI_SLIDE_ANIM_FRAME ) );
			int nValue = pSlider->GetValue();

			pSkinMesh->SetAnimTime( static_cast<float>( nValue * /*0.0333333f*/ 0.001f ) );
		} break;
		#pragma endregion case
	}
}
#pragma endregion Animation UI Event

#pragma region OnGUIMeshEvent
void CALLBACK CX2ViewerUI::OnGUIMeshEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case UI_EDIT_MESH_SCALE_X:
	case UI_EDIT_MESH_SCALE_Y:
	case UI_EDIT_MESH_SCALE_Z:
		{
			if( nEvent == EVENT_EDITBOX_STRING )
			{
				CDXUTEditBox*	pEditBox = NULL;
				CX2ViewerMesh*	pMesh = (CX2ViewerMesh*)SiGetObject( OS_MESH );
				pEditBox = (CDXUTEditBox*)SiGetMeshDlg( UI_EDIT_MESH_SCALE_X );
				pMesh->SetScaleX( (float)_wtof( pEditBox->GetText() ) );
				pEditBox = (CDXUTEditBox*)SiGetMeshDlg( UI_EDIT_MESH_SCALE_Y );
				pMesh->SetScaleY( (float)_wtof( pEditBox->GetText() ) );
				pEditBox = (CDXUTEditBox*)SiGetMeshDlg( UI_EDIT_MESH_SCALE_Z );
				pMesh->SetScaleZ( (float)_wtof( pEditBox->GetText() ) );
			}
		}
		return;

	case UI_EDIT_MESH_LIGHT_POS_X:
	case UI_EDIT_MESH_LIGHT_POS_Y:
	case UI_EDIT_MESH_LIGHT_POS_Z:
		{
			CDXUTEditBox*	pEditBox = NULL;
			CX2ViewerMesh*	pMesh = (CX2ViewerMesh*)SiGetObject( OS_MESH );
			pEditBox = (CDXUTEditBox*)SiGetMeshDlg( UI_EDIT_MESH_LIGHT_POS_X );
			pMesh->SetLightPosX( (float)_wtof( pEditBox->GetText() ) );
			pEditBox = (CDXUTEditBox*)SiGetMeshDlg( UI_EDIT_MESH_LIGHT_POS_Y );
			pMesh->SetLightPosY( (float)_wtof( pEditBox->GetText() ) );
			pEditBox = (CDXUTEditBox*)SiGetMeshDlg( UI_EDIT_MESH_LIGHT_POS_Z );
			pMesh->SetLightPosZ( (float)_wtof( pEditBox->GetText() ) );
		}
		return;

	case UI_CHECK_MESH_LIGHT_ONOFF:
		{
			CX2ViewerMesh*	pMesh = (CX2ViewerMesh*)SiGetObject( OS_MESH );

			bool bIsChecked = ((CDXUTCheckBox*)SiGetMeshDlg( UI_CHECK_MESH_LIGHT_ONOFF ))->GetChecked();
			pMesh->SetLightOnOff( bIsChecked );
		}
		return;

	case UI_BUT_MESH_RENDER_PARAM:
		{
			SiSelf()->SetRenderParamOnOff( !(SiSelf()->m_RenderParamUI.GetVisible()) );
		}
		return;
	}
}
#pragma endregion Mesh UI Event

#pragma region OnGUIRPEvent
void CALLBACK CX2ViewerUI::OnGUIRPEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case UI_BUT_PARAM_OK:
		{
			SiSelf()->m_Param.GetParamDlg( &(SiSelf()->m_RenderParamUI) );

			switch( SiSelf()->m_MeshSel )
			{
			case MS_MESH:
				{
					CX2ViewerMesh*	pMesh = (CX2ViewerMesh*)SiGetObject( OS_MESH );
					SiSelf()->m_Param.GetRenderParam( pMesh->GetRenderParam(), pMesh->GetImpactData(), *(pMesh->GetTexStageData()) );
				}
				break;

			case MS_SKIN_MESH:
				{
					CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
					SiSelf()->m_Param.GetRenderParam( pSkinMesh->GetRenderParam(), pSkinMesh->GetImpactData(), *(pSkinMesh->GetTexStageData()) );
				}
				break;
			}

			SiSelf()->m_Param.SetParamDlg( &(SiSelf()->m_RenderParamUI) );

			//SiSelf()->SetRenderParamOnOff();
		}
		return;

	case UI_BUT_PARAM_CANCEL:
		{
			SiSelf()->m_Param.SetParamDlg( &(SiSelf()->m_RenderParamUI) );
			SiSelf()->SetRenderParamOnOff( false );
		}
		return;
	}
}
#pragma endregion Rendering Parameter UI Event

#pragma region OnGUIParticleEvent
#ifndef UPDATE_X2VIEWER_2013 //JHKang
void CALLBACK CX2ViewerUI::OnGUIParticleEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CDXUTListBox* pListBox = (CDXUTListBox*)SiGetParticleDlg(UI_LIST_PARTICLE_LIST);

	wstring Name;
	
	if( pListBox->GetSelectedItem() != NULL )
		Name = pListBox->GetSelectedItem()->strText;
	int SelectedIndex = pListBox->GetSelectedIndex();
	CX2ViewerParticle* pParticleObj = (CX2ViewerParticle*)(SiGetObject( OS_PARTICLE ));

	switch( nControlID )
	{
	case UI_LIST_PARTICLE_LIST:
		{
			if( nEvent == EVENT_LISTBOX_SELECTION )
			{
				if( pParticleObj->GetParticleEffectDataByName(Name) != NULL )
				{
					CX2ViewerParticle::ParticleEffectData* pData = pParticleObj->GetParticleEffectDataByName(Name);
					CDXUTEditBox* pEditBox;
					CDXUTCheckBox* pCheckBox;

					WCHAR buf[256] = {0};
					// Particle Basic

					pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_TIME );
					swprintf(buf, L"%.4f", pData->m_fTime );  
					pEditBox->SetText(buf);

					pCheckBox = (CDXUTCheckBox*)SiGetParticleDlg( UI_CHECK_PARTICLE_TRACE );
					pCheckBox->SetChecked( pData->m_bTrace );

					pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_OFFSET_X );
					swprintf(buf, L"%.4f", pData->m_vOffset.x );  
					pEditBox->SetText(buf);

					pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_OFFSET_Y );
					swprintf(buf, L"%.4f", pData->m_vOffset.y );  
					pEditBox->SetText(buf);

					pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_OFFSET_Z );
					swprintf(buf, L"%.4f", pData->m_vOffset.z );  
					pEditBox->SetText(buf);

					pCheckBox = (CDXUTCheckBox*)SiGetParticleDlg( UI_CHECK_PARTICLE_LANDPOS );
					pCheckBox->SetChecked( pData->m_bLandPosition );

					pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_ROT_X );
					swprintf(buf, L"%.4f", pData->m_vRotation.x );  
					pEditBox->SetText(buf);

					pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_ROT_Y );
					swprintf(buf, L"%.4f", pData->m_vRotation.y );  
					pEditBox->SetText(buf);

					pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_ROT_Z );
					swprintf(buf, L"%.4f", pData->m_vRotation.z );  
					pEditBox->SetText(buf);

					pCheckBox = (CDXUTCheckBox*)SiGetParticleDlg( UI_CHECK_PARTICLE_APPUNITROT ); 
					pCheckBox->SetChecked( pData->m_bApplyUnitRotation );

				}
			}

		} break;

	case UI_BUT_PARTICLE_DELETE:
		{
			if( true == pParticleObj->DeleteParticleEffectData( Name ) )
			{
				pListBox->RemoveItem( SelectedIndex );
				pListBox->SelectItem(-1);
			}
			else
			{
				WARNINGMSG( L"내부 오류!");
			}
			
		} break;

	case UI_EDIT_PARTICLE_TIME:
		{
			CDXUTEditBox* pEditBox = (CDXUTEditBox*) pControl;
			if( pParticleObj->GetParticleEffectDataByName(Name) != NULL )
				pParticleObj->GetParticleEffectDataByName(Name)->m_fTime = (float)_wtof( pEditBox->GetText() );
		} break;

	case UI_BUT_PARTICLE_BONESET:
		{
			CDXUTListBox* pListBox = (CDXUTListBox*)SiGetSkinMeshDlg( UI_LIST_BONE );
			wstring BoneName;
			if( pListBox->GetSelectedItem() != NULL )
			{
				BoneName = pListBox->GetSelectedItem()->strText;
			}

			if( pParticleObj->GetParticleEffectDataByName(Name) != NULL )
				pParticleObj->GetParticleEffectDataByName(Name)->m_Pos = BoneName;
		} break;
	case UI_BUT_PARTICLE_BONECLEAR:
		{
			if( pParticleObj->GetParticleEffectDataByName(Name) != NULL )
				pParticleObj->GetParticleEffectDataByName(Name)->m_Pos = L"";
		} break;
	case UI_CHECK_PARTICLE_TRACE:
		{
			if( pParticleObj->GetParticleEffectDataByName(Name) != NULL )
				pParticleObj->GetParticleEffectDataByName(Name)->m_bTrace = ((CDXUTCheckBox*)pControl)->GetChecked();

		} break;

	case UI_EDIT_PARTICLE_OFFSET_X:
	case UI_EDIT_PARTICLE_OFFSET_Y:
	case UI_EDIT_PARTICLE_OFFSET_Z:
		{
			if( pParticleObj->GetParticleEffectDataByName(Name) != NULL )
			{
				D3DXVECTOR3 vOff(0,0,0);
				CDXUTEditBox*	pEditBox = NULL;
				pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_OFFSET_X );
				vOff.x = (float)_wtof( pEditBox->GetText() );
				pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_OFFSET_Y );
				vOff.y = (float)_wtof( pEditBox->GetText() );
				pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_OFFSET_Z );
				vOff.z = (float)_wtof( pEditBox->GetText() );
				pParticleObj->GetParticleEffectDataByName(Name)->m_vOffset = vOff;

			}
			

		} break;
	case UI_CHECK_PARTICLE_LANDPOS:
		{
			if( pParticleObj->GetParticleEffectDataByName(Name) != NULL )
				pParticleObj->GetParticleEffectDataByName(Name)->m_bLandPosition = ((CDXUTCheckBox*)pControl)->GetChecked();

		} break;

	case UI_EDIT_PARTICLE_ROT_X:
	case UI_EDIT_PARTICLE_ROT_Y:
	case UI_EDIT_PARTICLE_ROT_Z:
		{
			D3DXVECTOR3 vRot(0,0,0);
			CDXUTEditBox*	pEditBox = NULL;
			pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_ROT_X );
			vRot.x = (float)_wtof( pEditBox->GetText() );
			pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_ROT_Y );
			vRot.y = (float)_wtof( pEditBox->GetText() );
			pEditBox = (CDXUTEditBox*)SiGetParticleDlg( UI_EDIT_PARTICLE_ROT_Z );
			vRot.z = (float)_wtof( pEditBox->GetText() );
			if( pParticleObj->GetParticleEffectDataByName(Name) != NULL )
				pParticleObj->GetParticleEffectDataByName(Name)->m_vRotation = vRot;

		} break;
	case UI_CHECK_PARTICLE_APPUNITROT:
		{
			if( pParticleObj->GetParticleEffectDataByName(Name) != NULL )
				pParticleObj->GetParticleEffectDataByName(Name)->m_bApplyUnitRotation = ((CDXUTCheckBox*)pControl)->GetChecked();
		} break;
	case UI_BUT_PARTICLE_SAVESEQUENCE:
		{
			OPENFILENAME ofn;        // common dialog box structure
			WCHAR wszFileName[512];  // path까지 포함한 파일 이름

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize		= sizeof(ofn);
			ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile		= (LPWSTR)wszFileName;
			ofn.lpstrFile[0]	= '\0';
			ofn.nMaxFile		= sizeof(wszFileName);
			ofn.lpstrFilter		= L"lua script\0*.lua\0";
			ofn.nFilterIndex	= 1;
			ofn.lpstrFileTitle	= NULL;
			ofn.nMaxFileTitle	= 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if( TRUE == GetSaveFileName( &ofn ) )
			{
				pParticleObj->SaveParticleEffectData( ofn.lpstrFile );				
			}

		} break;
	case UI_BUT_PARTICLE_PARTICLEEDITOR:
		{
			// 모델 안보이게 가려주고..
			CX2ViewerSkinMesh*	pSkinMesh	= (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
			pParticleObj->SetMode( CX2ViewerParticle::PM_EDITOR );

			if(pSkinMesh != NULL)
			{
				if( pSkinMesh->GetXSkinAnim() != NULL )
					pSkinMesh->GetXSkinAnim()->SetShowObject(false);

				if( NULL != pSkinMesh->GetWeaponXSkinAnim() )
					pSkinMesh->GetWeaponXSkinAnim()->SetShowObject(false);
			}
						
			// Toggle 했을 때 기존의 On/Off를 유지하기 위해서 아예 벡터에서 빼는 식으로 제작.
			// 이후 UI_BUT_PARTICLE_EDITOR_EXIT 입력이 들어왔을 때 벡터를 다시 원상복구 시킨다.
			SiSelf()->m_vecDialog.clear();
			//SiSelf()->m_vecDialog.push_back( &(SiSelf()->m_ParticleEditor) );

			//SiSelf()->SetParticleEditButOnOff( true );

			//////////////////////////////////////////////////////////////////////////
			// 에디터 초기화
			//SiSelf()->InitParticleEditor();
		} break;
	}

	return;
}

// 파티클 수정 모드
void CALLBACK CX2ViewerUI::OnGUIParticleEditorEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2ViewerParticle* pParticleObj = (CX2ViewerParticle*)(SiGetObject( OS_PARTICLE ));
	CX2ViewerParticleEditor& refParticleEditor = pParticleObj->GetParticleEditor();
	CDXUTListBox* pTempletListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_PARTICLETEMPLET);
	CDXUTListBox* pCustomListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_MYPARTICLE);
	CDXUTListBox* pEmitterPropertiesListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_EMITTERATTRIBUTE);
	
	CDXUTListBox* pParticleEventListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_EVENT);
	CDXUTListBox* pEventTempletBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_EVENT_TYPE);
	
	CDXUTListBox* pModelViewListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_MODEL_LIST);

	switch(nControlID)
	{
	case UI_LIST_PARTICLE_EDITOR_MYPARTICLE:		// 수정중인 파티클 목록 선택시
		{
			if( nEvent == EVENT_LISTBOX_SELECTION )
			{
				if( pCustomListBox->GetSelectedItem() == NULL )
					break;

				pTempletListBox->m_nSelected = -1;
				pEventTempletBox->m_nSelected = -1;
				pParticleEventListBox->m_nSelected = -1;
				pModelViewListBox->m_nSelected = -1;
				wstring name(pCustomListBox->GetSelectedItem()->strText);

				if( pParticleObj != NULL)
				{
					pParticleObj->SetPreviewParticle( name, false );
				}

				SiSelf()->RefreshEventList();

			}

		} break;
	case UI_BUT_PARTICLE_EDITOR_MYPARTICLE_LOAD:
		{
			OPENFILENAME ofn;        // common dialog box structure
			WCHAR wszFileName[512];  // path까지 포함한 파일 이름

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize		= sizeof(ofn);
			ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile		= (LPWSTR)wszFileName;
			ofn.lpstrFile[0]	= '\0';
			ofn.nMaxFile		= sizeof(wszFileName);
			ofn.lpstrFilter		= L"text script\0*.txt\0";
			ofn.nFilterIndex	= 1;
			ofn.lpstrFileTitle	= NULL;
			ofn.nMaxFileTitle	= 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if( TRUE == GetOpenFileName( &ofn ) )
			{				
				// file path와 name을 분리
				WCHAR drive[_MAX_DRIVE];
				WCHAR dir[_MAX_DIR];
				WCHAR fname[_MAX_FNAME];
				WCHAR ext[_MAX_EXT];
				_wsplitpath( ofn.lpstrFile, drive, dir, fname, ext);

				wstring strFileName(fname);
				strFileName += ext;				

				pParticleObj->GetCustomParticleSystem()->OpenScriptFile( strFileName.c_str() );
			}
			// list update
			{
				pCustomListBox->RemoveAllItems();
				const map<wstring, CKTDGParticleSystem::CParticleEventSequence*>& templetSeq = pParticleObj->GetCustomParticleSystem()->GetTempletSequences();
				map<wstring, CKTDGParticleSystem::CParticleEventSequence*>::const_iterator it;
				for( it=templetSeq.begin(); it != templetSeq.end(); it++ )
				{
					wstring& wstrName = (wstring)it->first;
					pCustomListBox->AddItem( wstrName.c_str(), NULL );
				}
			}

		} break;
	case UI_BUT_PARTICLE_EDITOR_MYPARTICLE_DELETE:
		{
			if( pCustomListBox->GetSelectedItem() == NULL )
				break;

			wstring name = pCustomListBox->GetSelectedItem()->strText;
			pParticleObj->GetCustomParticleSystem()->DeleteTempletSequence( name );
			pCustomListBox->RemoveItem( pCustomListBox->GetSelectedIndex());

			if( pParticleObj != NULL)
			{
				pParticleObj->SetPreviewParticle( L"", true );
			}

		} break;
	case UI_BUT_PARTICLE_EDITOR_MYPARTICLE_SAVE:	// Script Export는 사실 물밑에서 자주 되고 있지만, 그걸 강제로 해주는 버튼. 임시 파일과 실제 저장하는 파일을 분리해야겠다..
		{
			OPENFILENAME ofn;        // common dialog box structure
			WCHAR wszFileName[512];  // path까지 포함한 파일 이름

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize		= sizeof(ofn);
			ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile		= (LPWSTR)wszFileName;
			ofn.lpstrFile[0]	= '\0';
			ofn.nMaxFile		= sizeof(wszFileName);
			ofn.lpstrFilter		= L"text script\0*.txt\0";
			ofn.nFilterIndex	= 1;
			ofn.lpstrFileTitle	= NULL;
			ofn.nMaxFileTitle	= 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if( TRUE == GetSaveFileName( &ofn ) )
			{
				refParticleEditor.ExportParticleScript( pParticleObj->GetCustomParticleSystem(), ofn.lpstrFile );
			}
			refParticleEditor.ExportParticleScript( pParticleObj->GetCustomParticleSystem(), L"ParticleTemp.txt" );
						
		} break;
	case UI_LIST_PARTICLE_EDITOR_PARTICLETEMPLET:	// 파티클 템플릿 리스트를 누른 경우
		{
			if( nEvent == EVENT_LISTBOX_SELECTION )
			{
				pEmitterPropertiesListBox->m_nSelected = -1;
				pCustomListBox->m_nSelected = -1;
				pEventTempletBox->m_nSelected = -1;
				pParticleEventListBox->m_nSelected = -1;
				pModelViewListBox->m_nSelected = -1;

				pParticleEventListBox->RemoveAllItems();

				if( pTempletListBox->GetSelectedItem() == NULL )
					break;
				wstring name(pTempletListBox->GetSelectedItem()->strText);

				if( pParticleObj != NULL)
				{
					pParticleObj->SetPreviewParticle( name, true );
				}			
			}
			

		} break;
	case UI_BUT_PARTICLE_EDITOR_PARTICLETEMPLET_COPY:		// 템플릿을 수정하고자 하는 파티클 리스트로 카피한다
		{
			if( pTempletListBox->GetSelectedIndex() == -1 ) 
				return;
			pParticleObj->CopyTempletParticleToCustom( pTempletListBox->GetSelectedItem()->strText );

			// list update
			pCustomListBox->RemoveAllItems();
			const map<wstring, CKTDGParticleSystem::CParticleEventSequence*>& templetSeq = pParticleObj->GetCustomParticleSystem()->GetTempletSequences();
			map<wstring, CKTDGParticleSystem::CParticleEventSequence*>::const_iterator it;
			for( it=templetSeq.begin(); it != templetSeq.end(); it++ )
			{
				wstring& wstrName = (wstring)it->first;
				pCustomListBox->AddItem( wstrName.c_str(), NULL );
			}



		} break;
	case UI_BUT_PARTICLE_EDITOR_PARTICLETEMPLET_RELOAD:		// 템플릿을 다시 로드
		{
			if(pParticleObj != NULL)
			{
				pParticleObj->ReloadParticleFile();


				CKTDGParticleSystem* pParticleSystem = pParticleObj->GetParticleSystem();
				 if( pParticleSystem != NULL )
				 {
					 const map<wstring, CKTDGParticleSystem::CParticleEventSequence*>& templetSeq = pParticleSystem->GetTempletSequences();

					 CDXUTListBox* pListBox		= (CDXUTListBox*)SiGetParticleEditorDlg( UI_LIST_PARTICLE_EDITOR_PARTICLETEMPLET );

					 pListBox->RemoveAllItems();
					 map<wstring, CKTDGParticleSystem::CParticleEventSequence*>::const_iterator it;
					 for( it=templetSeq.begin(); it != templetSeq.end(); it++ )
					 {
						 wstring& wstrName = (wstring)it->first;
						 pListBox->AddItem( wstrName.c_str(), NULL );
					 }

				 }
			}

		} break;

	case UI_LIST_PARTICLE_EDITOR_EMITTERATTRIBUTE:		// 수정하고자 하는 파티클의 Emitter 정보를 클릭시
		{

			if( nEvent == EVENT_LISTBOX_SELECTION )
			{
				if( pCustomListBox->GetSelectedItem() == NULL )
					return;

				CX2ViewerParticleEditor::PropertyData* pData = (CX2ViewerParticleEditor::PropertyData*)pEmitterPropertiesListBox->GetSelectedItem()->pData;
				if( pData != NULL )
				{
					// 주의 : const_cast 사용! 
					CKTDGParticleSystem::CParticleEventSequence* pSeq = 
						const_cast<CKTDGParticleSystem::CParticleEventSequence*>(
						pParticleObj->GetCustomParticleSystem()->GetTempletSequencesByName( pCustomListBox->GetSelectedItem()->strText ) );

					CDXUTStatic* pStatic = (CDXUTStatic*)SiGetParticleEditorDlg( UI_STATIC_PARTICLE_EDITOR_EMITTERATTRIBUTE_VALUE );
					CDXUTEditBox* pSingleEdit = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_SINGLE );
					CDXUTEditBox* pEditX = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_X );
					CDXUTEditBox* pEditY = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Y );
					CDXUTEditBox* pEditZ = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Z );
					CDXUTComboBox* pCombo = (CDXUTComboBox*)SiGetParticleEditorDlg( UI_COMBO_PARTICLE_EDITOR_EMITTERATTRIBUTE );

					if( pData->m_bAllowMinMax )
					{
						pStatic->SetText( L"Value(Random Allowed)" );
					}
					else
					{
						pStatic->SetText( L"Value(NO RANDOM)" );
					}

					pSingleEdit->SetVisible( false );
					pEditX->SetVisible( false );
					pEditY->SetVisible( false );
					pEditZ->SetVisible( false );
					pCombo->SetVisible( false );

					switch( pData->m_valuetype )
					{
					case CX2ViewerParticleEditor::VT_XYZ:
						{
							pEditX->SetVisible( true );
							pEditY->SetVisible( true );
							pEditZ->SetVisible( true );

							CMinMax<D3DXVECTOR3> val = refParticleEditor.GetXYZValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type );
							pEditX->SetText( refParticleEditor.GetMinMaxString( val.m_Min.x, val.m_Max.x ).c_str() );
							pEditY->SetText( refParticleEditor.GetMinMaxString( val.m_Min.y, val.m_Max.y ).c_str() );
							pEditZ->SetText( refParticleEditor.GetMinMaxString( val.m_Min.z, val.m_Max.z ).c_str() );

						} break;
					case CX2ViewerParticleEditor::VT_XY:
						{
							pEditX->SetVisible( true );
							pEditY->SetVisible( true );

							CMinMax<D3DXVECTOR2> val = refParticleEditor.GetXYValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type );
							pEditX->SetText( refParticleEditor.GetMinMaxString( val.m_Min.x, val.m_Max.x ).c_str() );
							pEditY->SetText( refParticleEditor.GetMinMaxString( val.m_Min.y, val.m_Max.y ).c_str() );
						} break;
					case CX2ViewerParticleEditor::VT_STRING:
						{
							pSingleEdit->SetVisible( true );
							wstring val = refParticleEditor.GetStringValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type );
							pSingleEdit->SetText( val.c_str() );

						} break;
					case CX2ViewerParticleEditor::VT_FLOAT:
						{
							pSingleEdit->SetVisible( true );
							CMinMax<float> val = refParticleEditor.GetFloatValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type );
							pSingleEdit->SetText( refParticleEditor.GetMinMaxString( val ).c_str() );

						} break;
					case CX2ViewerParticleEditor::VT_INT:
						{
							wstringstream wstrm;
							pSingleEdit->SetVisible( true );
							int val = refParticleEditor.GetIntValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type );
							wstrm << val;
							pSingleEdit->SetText( wstrm.str().c_str() );
						} break;
					case CX2ViewerParticleEditor::VT_BOOL:
						{
							pCombo->SetVisible( true );
							pCombo->RemoveAllItems();
							pCombo->AddItem( L"True", NULL );
							pCombo->AddItem( L"False", NULL );

							if( refParticleEditor.GetBoolValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type ) )
							{
								pCombo->SetSelectedByText( L"True" );
							}
							else
							{
								pCombo->SetSelectedByText( L"False" );
							}

						} break;

					case CX2ViewerParticleEditor::VT_COMBO_CUSTOM:
						{
							pCombo->SetVisible( true );
							refParticleEditor.GetCustomComboBoxValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type, pCombo );
						} break;

					}
				}
			}
		} break;

// 	case UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_SINGLE:
// 		{
// 
// 		} break;
// 	case UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_X:
// 		{
// 
// 		} break;
// 	case UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Y:
// 		{
// 
// 		} break;
// 	case UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Z:
// 		{
// 
// 		} break;
// 	case UI_COMBO_PARTICLE_EDITOR_EMITTERATTRIBUTE:
// 		{
// 
// 		} break;
// 	case UI_BUT_PARTICLE_EDITOR_EMITTERATTRIBUTE_DEFAULT:
// 		{
// 
// 		} break;
	case UI_BUT_PARTICLE_EDITOR_EMITTERATTRIBUTE_APPLY:		// Emitter 속성변경 적용
		{
			///
			if( pCustomListBox->GetSelectedItem() == NULL )
				break;
			if( pEmitterPropertiesListBox->GetSelectedItem() == NULL )
				break;
			CX2ViewerParticleEditor::PropertyData* pData = (CX2ViewerParticleEditor::PropertyData*)pEmitterPropertiesListBox->GetSelectedItem()->pData;
			if( pData != NULL )
			{
				// 주의 : const_cast 사용! 여기서는 실제로 값도 변화시키고 있으므로 주의.
				CKTDGParticleSystem::CParticleEventSequence* pSeq = 
					const_cast<CKTDGParticleSystem::CParticleEventSequence*>(
					pParticleObj->GetCustomParticleSystem()->GetTempletSequencesByName( pCustomListBox->GetSelectedItem()->strText ) );

				CDXUTStatic* pStatic = (CDXUTStatic*)SiGetParticleEditorDlg( UI_STATIC_PARTICLE_EDITOR_EMITTERATTRIBUTE_VALUE );
				CDXUTEditBox* pSingleEdit = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_SINGLE );
				CDXUTEditBox* pEditX = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_X );
				CDXUTEditBox* pEditY = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Y );
				CDXUTEditBox* pEditZ = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Z );
				CDXUTComboBox* pCombo = (CDXUTComboBox*)SiGetParticleEditorDlg( UI_COMBO_PARTICLE_EDITOR_EMITTERATTRIBUTE );

				switch( pData->m_valuetype )
				{
				case CX2ViewerParticleEditor::VT_XYZ:
					{
						CMinMax<D3DXVECTOR3> val;
						val = refParticleEditor.ParseCMinMaxD3DVECTOR3( pEditX->GetText(), pEditY->GetText(), pEditZ->GetText() );

						refParticleEditor.SetXYZValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type, val );
					} break;
				case CX2ViewerParticleEditor::VT_XY:
					{
						CMinMax<D3DXVECTOR2> val;
						val = refParticleEditor.ParseCMinMaxD3DVECTOR2( pEditX->GetText(), pEditY->GetText() );

						refParticleEditor.SetXYValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type, val );
					} break;
				case CX2ViewerParticleEditor::VT_STRING:
					{
						refParticleEditor.SetStringValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type, pSingleEdit->GetText() );

					} break;
				case CX2ViewerParticleEditor::VT_FLOAT:
					{
						CMinMax<float> val = refParticleEditor.ParseCMinMaxFloat( pSingleEdit->GetText() );
						refParticleEditor.SetFloatValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type, val );
					} break;
				case CX2ViewerParticleEditor::VT_INT:
					{
						int val = _wtoi( pSingleEdit->GetText() );
						refParticleEditor.SetIntValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type, val );
						
					} break;
				case CX2ViewerParticleEditor::VT_BOOL:
					{
						if( pCombo->GetSelectedItem() == NULL )
							break;

						if( 0 == wcscmp( pCombo->GetSelectedItem()->strText, L"True" ) )
						{
							refParticleEditor.SetBoolValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type, true );
						}
						else
						{
							refParticleEditor.SetBoolValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type, false );
						}
					} break;

				case CX2ViewerParticleEditor::VT_COMBO_CUSTOM:
					{
						refParticleEditor.SetCustomComboBoxValue( pSeq, (CX2ViewerParticleEditor::EMITTER_PROPERTIES)pData->m_Type, pCombo->GetSelectedItem() );
					} break;

				}

			}

			pParticleObj->ReplayPreviewParticle();

		} break;

	case UI_LIST_PARTICLE_EDITOR_EVENT:				// 파티클에 이미 만들어져 있는 이벤트들 리스트
		{

			if( nEvent == EVENT_LISTBOX_SELECTION )
			{
				pEventTempletBox->m_nSelected = -1;

				if( pParticleEventListBox->GetSelectedItem() == NULL )
					return;

				// 아래쪽 선택칸 변경. 기존값을 읽어와 보여준다.
				CDXUTButton* pButApply = (CDXUTButton*)SiGetParticleEditorDlg( UI_BUT_PARTICLE_EDITOR_EVENT_APPLY );
				//CDXUTButton* pButNew = (CDXUTButton*)SiGetParticleEditorDlg( UI_BUT_PARTICLE_EDITOR_EVENT_NEW );

				CDXUTStatic* pStatic = (CDXUTStatic*)SiGetParticleEditorDlg( UI_STATIC_PARTICLE_EDITOR_EVENT_VALUE );
				CDXUTEditBox* pSingleEdit = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_SINGLE );
				CDXUTEditBox* pEditX = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_X );
				CDXUTEditBox* pEditY = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Y );
				CDXUTEditBox* pEditZ = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Z );
				CDXUTComboBox* pCombo = (CDXUTComboBox*)SiGetParticleEditorDlg( UI_COMBO_PARTICLE_EDITOR_EVENT );
				CDXUTEditBox* pEditR = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_R );
				CDXUTEditBox* pEditG = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_G );
				CDXUTEditBox* pEditB = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_B );
				CDXUTEditBox* pEditA = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_A );
				CDXUTCheckBox* pCheckFade = (CDXUTCheckBox*)SiGetParticleEditorDlg( UI_CHECK_PARTICLE_EDITOR_EVENT_FADE );

				CDXUTEditBox* pEditTime1 = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME1 );
				CDXUTEditBox* pEditTime2 = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME2 );

				CKTDGParticleSystem::CParticleEvent* pEvent = (CKTDGParticleSystem::CParticleEvent*) pParticleEventListBox->GetSelectedItem()->pData;
				if( pEvent == NULL )
					return;
				pButApply->SetVisible( true );

				// 시간 세팅해주고
				WCHAR buf[64];
				swprintf( buf, L"%.4f", pEvent->GetActualTime().m_Min );
				pEditTime1->SetText( buf );
				swprintf( buf, L"%.4f", pEvent->GetActualTime().m_Max );
				pEditTime2->SetText( buf );
				pCheckFade->SetChecked( pEvent->IsFade() );
				if( pEvent->IsFade() )
				{
					pEditTime2->SetVisible( true );
				}
				else
				{
					pEditTime2->SetVisible( false );
				}

				CX2ViewerParticleEditor::PropertyData* pPropertyData = refParticleEditor.GetEventProperties( pEvent->GetEventType() );
				if( pPropertyData == NULL )
					return;

				if( pPropertyData->m_bAllowMinMax )
				{
					pStatic->SetText( L"Value(Random Allowed)" );
				}
				else
				{
					pStatic->SetText( L"Value(NO RANDOM)" );
				}

				pSingleEdit->SetVisible( false );
				pEditX->SetVisible( false );
				pEditY->SetVisible( false );
				pEditZ->SetVisible( false );
				pCombo->SetVisible( false );
				pEditR->SetVisible( false );
				pEditG->SetVisible( false );
				pEditB->SetVisible( false );
				pEditA->SetVisible( false );

				switch( pPropertyData->m_valuetype )
				{
				case CX2ViewerParticleEditor::VT_XYZ:
					{
						pEditX->SetVisible( true );
						pEditY->SetVisible( true );
						pEditZ->SetVisible( true );

						CMinMax<D3DXVECTOR3> val = refParticleEditor.GetXYZValue( pEvent, pEvent->GetEventType() );
						pEditX->SetText( refParticleEditor.GetMinMaxString( val.m_Min.x, val.m_Max.x ).c_str() );
						pEditY->SetText( refParticleEditor.GetMinMaxString( val.m_Min.y, val.m_Max.y ).c_str() );
						pEditZ->SetText( refParticleEditor.GetMinMaxString( val.m_Min.z, val.m_Max.z ).c_str() );

					} break;
				case CX2ViewerParticleEditor::VT_XY:
					{
						pEditX->SetVisible( true );
						pEditY->SetVisible( true );

						CMinMax<D3DXVECTOR2> val = refParticleEditor.GetXYValue( pEvent, pEvent->GetEventType() );
						pEditX->SetText( refParticleEditor.GetMinMaxString( val.m_Min.x, val.m_Max.x ).c_str() );
						pEditY->SetText( refParticleEditor.GetMinMaxString( val.m_Min.y, val.m_Max.y ).c_str() );
					} break;
				case CX2ViewerParticleEditor::VT_STRING:
					{
						pSingleEdit->SetVisible( true );
						wstring val = refParticleEditor.GetStringValue( pEvent, pEvent->GetEventType() );
						pSingleEdit->SetText( val.c_str() );

					} break;
				case CX2ViewerParticleEditor::VT_FLOAT:
					{
						pSingleEdit->SetVisible( true );
						CMinMax<float> val = refParticleEditor.GetFloatValue( pEvent, pEvent->GetEventType() );
						pSingleEdit->SetText( refParticleEditor.GetMinMaxString( val ).c_str() );

					} break;
				case CX2ViewerParticleEditor::VT_RGBA:
					{
						pEditR->SetVisible( true );
						pEditG->SetVisible( true );
						pEditB->SetVisible( true );
						pEditA->SetVisible( true );
						CMinMax<D3DXCOLOR> val = refParticleEditor.GetRGBAValue( pEvent, pEvent->GetEventType() );
						pEditR->SetText( refParticleEditor.GetMinMaxString( val.m_Min.r, val.m_Max.r ).c_str() );
						pEditG->SetText( refParticleEditor.GetMinMaxString( val.m_Min.g, val.m_Max.g ).c_str() );
						pEditB->SetText( refParticleEditor.GetMinMaxString( val.m_Min.b, val.m_Max.b ).c_str() );
						pEditA->SetText( refParticleEditor.GetMinMaxString( val.m_Min.a, val.m_Max.a ).c_str() );

					} break;
				}

			}
			

		} break;
	case UI_BUT_PARTICLE_EDITOR_EVENT_DELETE:
		{
			if( pCustomListBox->GetSelectedItem() == NULL )
				return;
			if( pParticleEventListBox->GetSelectedItem() == NULL )
				return;

			CKTDGParticleSystem::CParticleEvent* pEvent = NULL;
			CX2ViewerParticleEditor::PropertyData* pPropData = NULL;
			CKTDGParticleSystem::CParticleEventSequence* pSeq = 
				const_cast<CKTDGParticleSystem::CParticleEventSequence*>(
				pParticleObj->GetCustomParticleSystem()->GetTempletSequencesByName( pCustomListBox->GetSelectedItem()->strText ) );
			if( pSeq == NULL )
				return;

			pEvent = (CKTDGParticleSystem::CParticleEvent*)pParticleEventListBox->GetSelectedItem()->pData;

			/*vector<CKTDGParticleSystem::CParticleEvent*>* pVecEventList = pSeq->GetEventList();
			for( vector<CKTDGParticleSystem::CParticleEvent*>::iterator it = pVecEventList->begin(); it < pVecEventList->end(); ++it )
			{				
				if( *it == pEvent )
				{
					SAFE_DELETE( pEvent );
					pVecEventList->erase( it );
					pParticleEventListBox->RemoveItem( pParticleEventListBox->GetSelectedIndex() );
				}
			}*/

			CKTDGParticleSystem::CParticleEventList* pVecEventList = pSeq->GetEventList();
			

		} break;
	case UI_LIST_PARTICLE_EDITOR_EVENT_TYPE:		// 오른쪽에 있는 파티클 이벤트 템플릿 목록
		{

			if( nEvent == EVENT_LISTBOX_SELECTION )
			{
				pParticleEventListBox->m_nSelected = -1;
				if( pEventTempletBox->GetSelectedItem() == NULL )
					return;
				// 아래쪽 선택칸만 변경. Apply 버튼을 New로 바꾼다. 값은 Clear 해준다.
				// 아래쪽 선택칸만 변경. New 버튼을 Apply로 바꾼다. 기존값을 읽어와 보여준다.
				CDXUTButton* pButApply = (CDXUTButton*)SiGetParticleEditorDlg( UI_BUT_PARTICLE_EDITOR_EVENT_APPLY );
				//CDXUTButton* pButNew = (CDXUTButton*)SiGetParticleEditorDlg( UI_BUT_PARTICLE_EDITOR_EVENT_NEW );


				CDXUTStatic* pStatic = (CDXUTStatic*)SiGetParticleEditorDlg( UI_STATIC_PARTICLE_EDITOR_EVENT_VALUE );
				CDXUTEditBox* pSingleEdit = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_SINGLE );
				CDXUTEditBox* pEditX = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_X );
				CDXUTEditBox* pEditY = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Y );
				CDXUTEditBox* pEditZ = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Z );
				CDXUTComboBox* pCombo = (CDXUTComboBox*)SiGetParticleEditorDlg( UI_COMBO_PARTICLE_EDITOR_EVENT );
				CDXUTEditBox* pEditR = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_R );
				CDXUTEditBox* pEditG = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_G );
				CDXUTEditBox* pEditB = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_B );
				CDXUTEditBox* pEditA = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_A );
				CDXUTCheckBox* pCheckFade = (CDXUTCheckBox*)SiGetParticleEditorDlg( UI_CHECK_PARTICLE_EDITOR_EVENT_FADE );

				CDXUTEditBox* pEditTime1 = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME1 );
				CDXUTEditBox* pEditTime2 = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME2 );

				pButApply->SetVisible( false );

				// 시간 세팅해주고			
				pEditTime1->SetText( L"0" );
				pEditTime2->SetText( L"0" );
				pEditTime2->SetVisible( false );
				pCheckFade->SetChecked( false );

				CX2ViewerParticleEditor::PropertyData* pPropertyData = (CX2ViewerParticleEditor::PropertyData*)pEventTempletBox->GetSelectedItem()->pData;
				if( pPropertyData == NULL )
					return;

				if( pPropertyData->m_bAllowMinMax )
				{
					pStatic->SetText( L"Value(Random Allowed)" );
				}
				else
				{
					pStatic->SetText( L"Value(NO RANDOM)" );
				}

				pSingleEdit->SetVisible( false );
				pEditX->SetVisible( false );
				pEditY->SetVisible( false );
				pEditZ->SetVisible( false );
				pCombo->SetVisible( false );
				pEditR->SetVisible( false );
				pEditG->SetVisible( false );
				pEditB->SetVisible( false );
				pEditA->SetVisible( false );

				switch( pPropertyData->m_valuetype )
				{
				case CX2ViewerParticleEditor::VT_XYZ:
					{
						pEditX->SetVisible( true );
						pEditY->SetVisible( true );
						pEditZ->SetVisible( true );

						pEditX->SetText( L"0" );
						pEditY->SetText( L"0" );
						pEditZ->SetText( L"0" );

					} break;
				case CX2ViewerParticleEditor::VT_XY:
					{
						pEditX->SetVisible( true );
						pEditY->SetVisible( true );

						pEditX->SetText( L"0" );
						pEditY->SetText( L"0" );
					} break;
				case CX2ViewerParticleEditor::VT_STRING:
					{
						pSingleEdit->SetVisible( true );
						pSingleEdit->SetText( L"" );

					} break;
				case CX2ViewerParticleEditor::VT_FLOAT:
					{
						pSingleEdit->SetVisible( true );
						pSingleEdit->SetText( L"0" );

					} break;
				case CX2ViewerParticleEditor::VT_RGBA:
					{
						pEditR->SetVisible( true );
						pEditG->SetVisible( true );
						pEditB->SetVisible( true );
						pEditA->SetVisible( true );

						pEditR->SetText( L"0" );
						pEditG->SetText( L"0" );
						pEditB->SetText( L"0" );
						pEditA->SetText( L"0" );

					} break;
				}


			}
			
		} break;

	case UI_CHECK_PARTICLE_EDITOR_EVENT_FADE:	// 이벤트 시간설정 관련..
		{
			//CDXUTEditBox* pEditTime1 = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME1 );
			CDXUTEditBox* pEditTime2 = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME2 );

			CDXUTCheckBox* pCheck = (CDXUTCheckBox*)pControl;
			if( pCheck->GetChecked() )
			{
				pEditTime2->SetVisible( true );
			}
			else
			{
				pEditTime2->SetVisible( false );
			}

		} break;
// 	case UI_EDIT_PARTICLE_EDITOR_EVENT_TIME1:
// 		{
// 
// 		} break;
	case UI_EDIT_PARTICLE_EDITOR_EVENT_TIME2:		// 시간이 Lifetime을 넘으면 거기까지만으로
		{
			if( pCustomListBox->GetSelectedItem() == NULL )
				return;

			CDXUTEditBox* pEditBox = (CDXUTEditBox*)pControl;
			
			wstring name = pCustomListBox->GetSelectedItem()->strText;
			CKTDGParticleSystem::CParticleEventSequence* pTempletSeq = 
				const_cast<CKTDGParticleSystem::CParticleEventSequence*>(pParticleObj->GetCustomParticleSystem()->GetTempletSequencesByName( name ));

			if( pTempletSeq->GetLifeTime().m_Max < (float)_wtof( pEditBox->GetText() ) )
			{
				wstringstream wstrm;
				wstrm << pTempletSeq->GetLifeTime().m_Max;
				pEditBox->SetText( wstrm.str().c_str() );
			}

		} break;

// 	case UI_EDIT_PARTICLE_EDITOR_EVENT_SINGLE:
// 		{
// 
// 		} break;
// 	case UI_EDIT_PARTICLE_EDITOR_EVENT_X:
// 		{
// 
// 		} break;
// 	case UI_EDIT_PARTICLE_EDITOR_EVENT_Y:
// 		{
// 
// 		} break;
// 	case UI_EDIT_PARTICLE_EDITOR_EVENT_Z:
// 		{
// 
// 		} break;
// 	case UI_COMBO_PARTICLE_EDITOR_EVENT:
// 		{
// 
// 		} break;
// 	case UI_EDIT_PARTICLE_EDITOR_EVENT_R:
// 		{
// 		}break;
// 	case UI_EDIT_PARTICLE_EDITOR_EVENT_G:
// 		{
// 		}break;
// 	case UI_EDIT_PARTICLE_EDITOR_EVENT_B:
// 		{
// 		} break;
// 	case UI_EDIT_PARTICLE_EDITOR_EVENT_A:
// 		{
// 
// 		} break;
	case UI_BUT_PARTICLE_EDITOR_EVENT_APPLY:
		{
			//////////////////////////////////////////////////////////////////////////
			if( pCustomListBox->GetSelectedItem() == NULL )
				return;

			CKTDGParticleSystem::CParticleEvent* pEvent = NULL;
			CX2ViewerParticleEditor::PropertyData* pPropData = NULL;
			CKTDGParticleSystem::CParticleEventSequence* pSeq = 
				const_cast<CKTDGParticleSystem::CParticleEventSequence*>(
				pParticleObj->GetCustomParticleSystem()->GetTempletSequencesByName( pCustomListBox->GetSelectedItem()->strText ) );
			if( pSeq == NULL )
				return;

			if( pParticleEventListBox->GetSelectedItem() == NULL )
				return;
			// 기존 이벤트 수정
			pEvent = (CKTDGParticleSystem::CParticleEvent*)pParticleEventListBox->GetSelectedItem()->pData;
			pPropData = refParticleEditor.GetEventProperties( pEvent->GetEventType() );
			
			if( pPropData == NULL || pEvent == NULL )
				return;
			
			// 값 설정 시작
			CDXUTCheckBox* pCheckFade = (CDXUTCheckBox*)SiGetParticleEditorDlg( UI_CHECK_PARTICLE_EDITOR_EVENT_FADE );
			CDXUTEditBox* pEditTime1 = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME1 );
			CDXUTEditBox* pEditTime2 = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME2 );
			pEvent->SetFade( pCheckFade->GetChecked() );
			if( pCheckFade->GetChecked() )
			{
				CMinMax<float> val;
				val.m_Min = (float)_wtof( pEditTime1->GetText() );
				val.m_Max = (float)_wtof( pEditTime2->GetText() );
				pEvent->SetActualTime( val );
			}
			else
			{
				CMinMax<float> val;
				val.m_Min = (float)_wtof( pEditTime1->GetText() );
				val.m_Max = val.m_Min;
				pEvent->SetActualTime( val );
			}

			switch( pPropData->m_valuetype )
			{
			case CX2ViewerParticleEditor::VT_RGBA:
				{
					CDXUTEditBox* pEditR = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_R );
					CDXUTEditBox* pEditG = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_G );
					CDXUTEditBox* pEditB = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_B );
					CDXUTEditBox* pEditA = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_A );

					CMinMax<D3DXCOLOR> val;
					CMinMax<float> valr = refParticleEditor.ParseCMinMaxFloat( pEditR->GetText() );
					CMinMax<float> valg = refParticleEditor.ParseCMinMaxFloat( pEditG->GetText() );
					CMinMax<float> valb = refParticleEditor.ParseCMinMaxFloat( pEditB->GetText() );
					CMinMax<float> vala = refParticleEditor.ParseCMinMaxFloat( pEditA->GetText() );

					val.m_Min = D3DXCOLOR(valr.m_Min, valg.m_Min, valb.m_Min, vala.m_Min);
					val.m_Max = D3DXCOLOR(valr.m_Max, valg.m_Max, valb.m_Max, vala.m_Max);

					refParticleEditor.SetRGBAValue( pEvent, pEvent->GetEventType(), val );
 
				} break;
			case CX2ViewerParticleEditor::VT_XYZ:
				{
					CDXUTEditBox* pEditX = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_X );
					CDXUTEditBox* pEditY = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Y );
					CDXUTEditBox* pEditZ = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Z );

					CMinMax<D3DXVECTOR3> val;
					val = refParticleEditor.ParseCMinMaxD3DVECTOR3( pEditX->GetText(), pEditY->GetText(), pEditZ->GetText() );

					refParticleEditor.SetXYZValue( pEvent, pEvent->GetEventType(), val );
				} break;
			case CX2ViewerParticleEditor::VT_XY:
				{
					CDXUTEditBox* pEditX = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_X );
					CDXUTEditBox* pEditY = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Y );
					CMinMax<D3DXVECTOR2> val;
					val = refParticleEditor.ParseCMinMaxD3DVECTOR2( pEditX->GetText(), pEditY->GetText() );

					refParticleEditor.SetXYValue( pEvent, pEvent->GetEventType(), val );
				} break;
			case CX2ViewerParticleEditor::VT_STRING:
				{
					CDXUTEditBox* pSingleEdit = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_SINGLE );
					refParticleEditor.SetStringValue( pEvent, pEvent->GetEventType(), pSingleEdit->GetText() );

				} break;
			case CX2ViewerParticleEditor::VT_FLOAT:
				{
					CDXUTEditBox* pSingleEdit = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_SINGLE );
					CMinMax<float> val = refParticleEditor.ParseCMinMaxFloat( pSingleEdit->GetText() );
					refParticleEditor.SetFloatValue( pEvent, pEvent->GetEventType(), val );
				} break;			
			}

			SiSelf()->RefreshEventList();

			pParticleObj->ReplayPreviewParticle();
			
		} break;
	case UI_BUT_PARTICLE_EDITOR_EVENT_NEW:
		{
			//////////////////////////////////////////////////////////////////////////
			if( pCustomListBox->GetSelectedItem() == NULL )
				return;

			CKTDGParticleSystem::CParticleEvent* pEvent = NULL;
			CX2ViewerParticleEditor::PropertyData* pPropData = NULL;
			CKTDGParticleSystem::CParticleEventSequence* pSeq = 
				const_cast<CKTDGParticleSystem::CParticleEventSequence*>(
				pParticleObj->GetCustomParticleSystem()->GetTempletSequencesByName( pCustomListBox->GetSelectedItem()->strText ) );
			if( pSeq == NULL )
				return;
		
			// 여기도 Const_cast...
			// 새로 만들기
			if( pEventTempletBox->GetSelectedItem() != NULL )
			{
				pPropData = (CX2ViewerParticleEditor::PropertyData*)pEventTempletBox->GetSelectedItem()->pData;
				pEvent = refParticleEditor.EventFactory( (CKTDGParticleSystem::EVENT_TYPE)pPropData->m_Type );
			}
			else if( pParticleEventListBox->GetSelectedItem() != NULL )
			{				
				CKTDGParticleSystem::CParticleEvent* pOrgEvent = (CKTDGParticleSystem::CParticleEvent*)pParticleEventListBox->GetSelectedItem()->pData;
				pPropData = refParticleEditor.GetEventProperties( pOrgEvent->GetEventType() );
				pEvent = refParticleEditor.EventFactory( pOrgEvent->GetEventType() );
			}
			else
			{
				return;
			}
				

			//vector<CKTDGParticleSystem::CParticleEvent*>* pVecEventList = pSeq->GetEventList();
			CKTDGParticleSystem::CParticleEventList* pVecEventList = pSeq->GetEventList();
			pVecEventList->push_back(pEvent);

			if( pPropData == NULL || pEvent == NULL )
				return;

			// 값 설정 시작
			CDXUTCheckBox* pCheckFade = (CDXUTCheckBox*)SiGetParticleEditorDlg( UI_CHECK_PARTICLE_EDITOR_EVENT_FADE );
			CDXUTEditBox* pEditTime1 = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME1 );
			CDXUTEditBox* pEditTime2 = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME2 );
			pEvent->SetFade( pCheckFade->GetChecked() );
			if( pCheckFade->GetChecked() )
			{
				CMinMax<float> val;
				val.m_Min = (float)_wtof( pEditTime1->GetText() );
				val.m_Max = (float)_wtof( pEditTime2->GetText() );
				pEvent->SetActualTime( val );
			}
			else
			{
				CMinMax<float> val;
				val.m_Min = (float)_wtof( pEditTime1->GetText() );
				val.m_Max = val.m_Min;
				pEvent->SetActualTime( val );
			}

			switch( pPropData->m_valuetype )
			{
			case CX2ViewerParticleEditor::VT_RGBA:
				{
					CDXUTEditBox* pEditR = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_R );
					CDXUTEditBox* pEditG = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_G );
					CDXUTEditBox* pEditB = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_B );
					CDXUTEditBox* pEditA = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_A );

					CMinMax<D3DXCOLOR> val;
					CMinMax<float> valr = refParticleEditor.ParseCMinMaxFloat( pEditR->GetText() );
					CMinMax<float> valg = refParticleEditor.ParseCMinMaxFloat( pEditG->GetText() );
					CMinMax<float> valb = refParticleEditor.ParseCMinMaxFloat( pEditB->GetText() );
					CMinMax<float> vala = refParticleEditor.ParseCMinMaxFloat( pEditA->GetText() );

					val.m_Min = D3DXCOLOR(valr.m_Min, valg.m_Min, valb.m_Min, vala.m_Min);
					val.m_Max = D3DXCOLOR(valr.m_Max, valg.m_Max, valb.m_Max, vala.m_Max);

					refParticleEditor.SetRGBAValue( pEvent, pEvent->GetEventType(), val );

				} break;
			case CX2ViewerParticleEditor::VT_XYZ:
				{
					CDXUTEditBox* pEditX = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_X );
					CDXUTEditBox* pEditY = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Y );
					CDXUTEditBox* pEditZ = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Z );

					CMinMax<D3DXVECTOR3> val;
					val = refParticleEditor.ParseCMinMaxD3DVECTOR3( pEditX->GetText(), pEditY->GetText(), pEditZ->GetText() );

					refParticleEditor.SetXYZValue( pEvent, pEvent->GetEventType(), val );
				} break;
			case CX2ViewerParticleEditor::VT_XY:
				{
					CDXUTEditBox* pEditX = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_X );
					CDXUTEditBox* pEditY = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Y );
					CMinMax<D3DXVECTOR2> val;
					val = refParticleEditor.ParseCMinMaxD3DVECTOR2( pEditX->GetText(), pEditY->GetText() );

					refParticleEditor.SetXYValue( pEvent, pEvent->GetEventType(), val );
				} break;
			case CX2ViewerParticleEditor::VT_STRING:
				{
					CDXUTEditBox* pSingleEdit = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_SINGLE );
					refParticleEditor.SetStringValue( pEvent, pEvent->GetEventType(), pSingleEdit->GetText() );

				} break;
			case CX2ViewerParticleEditor::VT_FLOAT:
				{
					CDXUTEditBox* pSingleEdit = (CDXUTEditBox*)SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_SINGLE );
					CMinMax<float> val = refParticleEditor.ParseCMinMaxFloat( pSingleEdit->GetText() );
					refParticleEditor.SetFloatValue( pEvent, pEvent->GetEventType(), val );
				} break;			
			}

			SiSelf()->RefreshEventList();

			pParticleObj->ReplayPreviewParticle();

		} break;
	case UI_LIST_PARTICLE_EDITOR_MODEL_LIST:
		{

			if( nEvent == EVENT_LISTBOX_SELECTION )
			{
				pTempletListBox->m_nSelected = -1;
				pCustomListBox->m_nSelected = -1;
				pEventTempletBox->m_nSelected = -1;
				pParticleEventListBox->m_nSelected = -1;
				pEmitterPropertiesListBox->m_nSelected = -1;

				pParticleEventListBox->RemoveAllItems();

				if( pModelViewListBox->GetSelectedItem() == NULL )
					break;
				CX2ViewerParticle::ParticleEffectData* pData = (CX2ViewerParticle::ParticleEffectData*)pModelViewListBox->GetSelectedItem()->pData;
				if( pData == NULL )
					return;

				if( pParticleObj != NULL)
				{
					pParticleObj->SetPreviewParticle( pData->m_SequenceName, pData->m_bIsTemplet );
				}	
			}
			
			
		} break;
	case UI_BUT_PARTICLE_EDITOR_MODEL_ADD:
		{
			CX2ViewerParticle::ParticleEffectData* pData = new CX2ViewerParticle::ParticleEffectData();
			
			if( pCustomListBox->GetSelectedItem() != NULL )
			{
				// 새로 만든 거 :$
				pData->m_SequenceName = pCustomListBox->GetSelectedItem()->strText;
				pData->m_bIsTemplet = false;
			}
			else if( pTempletListBox->GetSelectedItem() != NULL )
			{
				// 원래 있는 거
				pData->m_SequenceName = pTempletListBox->GetSelectedItem()->strText;
				pData->m_bIsTemplet = true;
			}
			///
			WCHAR buf[256];
			int i = 0;
			wstring newname;
			do 
			{
				wsprintf(buf, L"%s_%02d", pData->m_SequenceName.c_str(), i);
				i++;
				pData->m_Name = buf;
			} while(pParticleObj->GetParticleEffectDataByName( pData->m_Name ));

			pParticleObj->AddParticleEffectData( pData );
			pModelViewListBox->AddItem( pData->m_Name.c_str(), (void*)pData );			

		} break;
	case UI_BUT_PARTICLE_EDITOR_MODEL_DELETE:
		{
			if( pModelViewListBox->GetSelectedItem() == NULL )
				return;
			CX2ViewerParticle::ParticleEffectData* pData = (CX2ViewerParticle::ParticleEffectData*)pModelViewListBox->GetSelectedItem()->pData;
			if( pData == NULL )
				return;

			pParticleObj->DeleteParticleEffectData( pData->m_Name );
			pModelViewListBox->RemoveItem( pModelViewListBox->GetSelectedIndex() );
		} break;

	case UI_COMBO_PARTICLE_EDITOR_PLAY_TYPE:
		{
			CDXUTComboBox* pCombo = (CDXUTComboBox*) pControl;
			if( nEvent == EVENT_COMBOBOX_SELECTION_CHANGED )
			{
				//**
				WCHAR* wszPlayType = pCombo->GetSelectedItem()->strText;
				if( wcscmp( wszPlayType, L"ONE" ) == 0 )
				{
					pParticleObj->SetPreviewPlayMode( false );
				}
				else if( wcscmp( wszPlayType, L"LOOP" ) == 0 )
				{
					pParticleObj->SetPreviewPlayMode( true );
				}
			}
            
		} break;
	case UI_BUT_PARTICLE_EDITOR_PLAY:
		{
			pParticleObj->ReplayPreviewParticle();
		} break;
	
	case UI_SLIDE_PARTICLE_EDITOR_SPEED:
		{			
			CDXUTSlider* pSlider = (CDXUTSlider*) pControl;
			int nValue = pSlider->GetValue();

			pParticleObj->SetPreviewPlaySpeed( nValue/100.0f );
		} break;

	case UI_BUT_PARTICLE_EDITOR_EXIT:
		{
			CX2ViewerSkinMesh*	pSkinMesh	= (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );

			if(pSkinMesh != NULL)
			{
				if( pSkinMesh->GetXSkinAnim() != NULL )
					pSkinMesh->GetXSkinAnim()->SetShowObject(true);
				if( NULL != pSkinMesh->GetWeaponXSkinAnim() )
					pSkinMesh->GetWeaponXSkinAnim()->SetShowObject(true);
			}

			pParticleObj->SetMode( CX2ViewerParticle::PM_NORMAL );

			// UI_BUT_PARTICLE_PARTICLEEDITOR에서 뺐던 벡터들을 원상복구 시킨다.
			// 다이얼로그 셋을 추가하려면 여기에도 추가해야 함.
			//SiSelf()->SetParticleEditButOnOff( false );
			SiSelf()->m_vecDialog.clear();
			SiSelf()->m_vecDialog.push_back( &(SiSelf()->m_BaseUI) );
			SiSelf()->m_vecDialog.push_back( &(SiSelf()->m_SkinMeshUI) );
			SiSelf()->m_vecDialog.push_back( &(SiSelf()->m_Mesh) );
			SiSelf()->m_vecDialog.push_back( &(SiSelf()->m_RenderParamUI) );
			//SiSelf()->m_vecDialog.push_back( &(SiSelf()->m_ParticleBasic) );

			// 바깥쪽 리스트 업데이트
			CDXUTListBox* pListBox_Outside = (CDXUTListBox*)SiGetParticleDlg(UI_LIST_PARTICLE_LIST);
			pListBox_Outside->RemoveAllItems();
			vector<CX2ViewerParticle::ParticleEffectData*> vecParticleEffect = pParticleObj->GetParticleEffectData();
			for( vector<CX2ViewerParticle::ParticleEffectData*>::iterator it=vecParticleEffect.begin(); it != vecParticleEffect.end(); it++ )
			{
				CX2ViewerParticle::ParticleEffectData* pData = *it;
				if( pData == NULL )
					continue;
				pListBox_Outside->AddItem( pData->m_Name.c_str(), (void*)pData );
			}

		} break;
	default:
		break;
	}

}
#endif //UPDATE_X2VIEWER_2013
#pragma endregion 모델뷰 모드에서 표시되는 파티클 관련 Event

#pragma region Init
void CX2ViewerUI::Init()
{
	//UI의 기본크기 기준은 1024/768 로 한다.
	float fScaleX = 0.0f;
	float fScaleY = 0.0f;

	RECT rt;
	SetRect( &rt, 0, 0, 0, 0 );

	if( m_bIsInit == false )
	{
		fScaleX = 1.0f;
		fScaleY = 1.0f;

		m_bIsInit = true;

		GetClientRect( g_pKTDXApp->GetHWND(), &rt );
	}
	else
	{
		GetClientRect( g_pKTDXApp->GetHWND(), &rt );

		fScaleX = rt.right / WIDTH;
		fScaleY = rt.bottom / HEIGHT;
	}

	KLuaManager	luaManager(g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true);
	
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"ViewerUI.lua" );
	if( Info == NULL )
	{
		ERRORMSG( L"ViewerUI.lua 파일열기 실패" );
		return;
	}

	if( luaManager.DoMemory( Info->pRealData, Info->size ) == false )
	{
		ERRORMSG( L"ViewerUI.lua 파싱 실패" );
		return;
	}

	GetCurrentDirectory( MAX_PATH, m_wstrRootDir );

	if( rt.right == 0 && rt.bottom == 0 )
	{
		rt.right	= 1280;
		rt.bottom	= 960;
	}
	m_SkinMeshUI.SetLocation( rt.right - 150, 0 );
	m_BoneUI.SetLocation( rt.right - 300, 535 );
	m_EquipUI.SetLocation( rt.right - 300, 0 );
	m_WeaponUI.SetLocation( rt.right - 450, 175 );
	m_AccessoryUI.SetLocation( rt.right - 500, 310 );
	m_AnimationUI.SetLocation( rt.left + 370 , rt.bottom - 250 );
	m_RenderParamUI.SetLocation( rt.left + 130, 0 );

	m_Mesh.SetLocation( rt.right - 150, 0 );

	ResetBaseUI( &luaManager );			// BaseUI 재설정
	ResetSkinMeshUI( &luaManager );		// SkinMeshUI 재설정	
	ResetBoneUI( &luaManager );			// BoneUI 재설정
	ResetEquipUI( &luaManager );		// EquipUI 재설정
	ResetWeaponUI( &luaManager );		// WeaponUI 재설정
	ResetAccessoryUI( &luaManager );	// AccessoryUI 재설정
	ResetAnimationUI( &luaManager );	// AnimationUI 재설정
	ResetRenderParamUI( &luaManager );	// RenderParamUI 재설정

	/*
	//STATIC_WORLD_MESH
	GET_LUA_POS( STATIC_WORLD_MESH, nX, nY, nW, nH );
	pControl = m_BaseOption.GetControl( UI_STATIC_WORLD_MESH );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//BUT_WORLD_MESH
	GET_LUA_POS( BUT_WORLD_MESH, nX, nY, nW, nH );
	pControl = m_BaseOption.GetControl( UI_BUT_WORLD_MESH );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	
	//BUT_WORLD_MESH_RESET
	GET_LUA_POS( BUT_WORLD_MESH_RESET, nX, nY, nW, nH );
	pControl = m_BaseOption.GetControl( UI_BUT_WORLD_MESH_RESET );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_WORLD_MESH_X
	GET_LUA_POS( STATIC_WORLD_MESH_X, nX, nY, nW, nH );
	pControl = m_BaseOption.GetControl( UI_STATIC_WORLD_MESH_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_WORLD_MESH_Y
	GET_LUA_POS( STATIC_WORLD_MESH_Y, nX, nY, nW, nH );
	pControl = m_BaseOption.GetControl( UI_STATIC_WORLD_MESH_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_WORLD_MESH_Z
	GET_LUA_POS( STATIC_WORLD_MESH_Z, nX, nY, nW, nH );
	pControl = m_BaseOption.GetControl( UI_STATIC_WORLD_MESH_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//EDIT_WORLD_MESH_X
	GET_LUA_POS( EDIT_WORLD_MESH_X, nX, nY, nW, nH );
	pControl = m_BaseOption.GetControl( UI_EDIT_WORLD_MESH_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );

	//EDIT_WORLD_MESH_Y
	GET_LUA_POS( EDIT_WORLD_MESH_Y, nX, nY, nW, nH );
	pControl = m_BaseOption.GetControl( UI_EDIT_WORLD_MESH_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );

	//EDIT_WORLD_MESH_Z
	GET_LUA_POS( EDIT_WORLD_MESH_Z, nX, nY, nW, nH );
	pControl = m_BaseOption.GetControl( UI_EDIT_WORLD_MESH_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );

	//BUT_EFFECT_SET
	GET_LUA_POS( BUT_EFFECT_SET, nX, nY, nW, nH );
	pControl = m_BaseOption.GetControl( UI_BUT_EFFECT_SET );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// World Mesh Init
	CX2ViewerWorldMesh* pWorld = (CX2ViewerWorldMesh*)SiGetObject( OS_WORLD_MESH );
	swprintf( strNum, L"%.2f", pWorld->GetMatrix()->GetXPos() );
	m_BaseOption.GetEditBox( UI_EDIT_WORLD_MESH_X )->SetText( strNum );
	swprintf( strNum, L"%.2f", pWorld->GetMatrix()->GetYPos() );
	m_BaseOption.GetEditBox( UI_EDIT_WORLD_MESH_Y )->SetText( strNum );
	swprintf( strNum, L"%.2f", pWorld->GetMatrix()->GetZPos() );
	m_BaseOption.GetEditBox( UI_EDIT_WORLD_MESH_Z )->SetText( strNum );

	//BUT_RENDER_PARAM
	GET_LUA_POS( BUT_RENDER_PARAM, nX, nY, nW, nH );
	pControl = m_Unit.GetControl( UI_BUT_RENDER_PARAM );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//COMBO_PLAY_TYPE
	GET_LUA_POS( COMBO_PLAY_TYPE, nX, nY, nW, nH );
	pControl = m_Unit.GetControl( UI_COMBO_PLAY_TYPE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	CDXUTComboBox* pComboBox = (CDXUTComboBox*)pControl;
	pComboBox->RemoveAllItems();
	pComboBox->AddItem( L"ONE", NULL );
	pComboBox->AddItem( L"LOOP", NULL );
	pComboBox->SetDropHeight( 50 );
	switch( ((CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH ))->GetPlayType() )
	{
		case CKTDGXSkinAnim::XAP_ONE_WAIT:
		pComboBox->SetSelectedByText( L"ONE" );
		break;

		case CKTDGXSkinAnim::XAP_LOOP:
		pComboBox->SetSelectedByText( L"LOOP" );
		break;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Mesh Seting
	//STATIC_MESH_SCALE
	GET_LUA_POS( STATIC_MESH_SCALE, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_STATIC_MESH_SCALE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_MESH_SCALE_X
	GET_LUA_POS( STATIC_MESH_SCALE_X, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_STATIC_MESH_SCALE_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_MESH_SCALE_X
	GET_LUA_POS( EDIT_MESH_SCALE_X, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_EDIT_MESH_SCALE_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_MESH_SCALE_Y
	GET_LUA_POS( STATIC_MESH_SCALE_Y, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_STATIC_MESH_SCALE_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_MESH_SCALE_Y
	GET_LUA_POS( EDIT_SCALE_Y, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_EDIT_MESH_SCALE_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_MESH_SCALE_Z
	GET_LUA_POS( STATIC_MESH_SCALE_Z, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_STATIC_MESH_SCALE_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_MESH_SCALE_Z
	GET_LUA_POS( EDIT_MESH_SCALE_Z, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_EDIT_MESH_SCALE_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	vTemp = ((CX2ViewerMesh*)SiGetObject( OS_MESH ))->GetScale();
	pEditBox = NULL;
	swprintf( strNum, L"%.2f", vTemp.x );
	pEditBox = m_Mesh.GetEditBox( UI_EDIT_MESH_SCALE_X );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.y );
	pEditBox = m_Mesh.GetEditBox( UI_EDIT_MESH_SCALE_Y );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.z );
	pEditBox = m_Mesh.GetEditBox( UI_EDIT_MESH_SCALE_Z );
	pEditBox->SetText( strNum );

	//STATIC_MESH_LIGHT_POS
	GET_LUA_POS( STATIC_MESH_LIGHT_POS, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_STATIC_MESH_LIGHT_POS );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_MESH_LIGHT_POS_X
	GET_LUA_POS( STATIC_MESH_LIGHT_POS_X, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_STATIC_MESH_LIGHT_POS_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_MESH_SCALE_X
	GET_LUA_POS( EDIT_MESH_LIGHT_POS_X, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_EDIT_MESH_LIGHT_POS_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_MESH_LIGHT_POS_Y
	GET_LUA_POS( STATIC_MESH_LIGHT_POS_Y, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_STATIC_MESH_LIGHT_POS_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_MESH_LIGHT_POS_Y
	GET_LUA_POS( EDIT_MESH_LIGHT_POS_Y, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_EDIT_MESH_LIGHT_POS_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_MESH_LIGHT_POS_Z
	GET_LUA_POS( STATIC_MESH_LIGHT_POS_Z, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_STATIC_MESH_LIGHT_POS_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_MESH_LIGHT_POS_Z
	GET_LUA_POS( EDIT_MESH_LIGHT_POS_Z, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_EDIT_MESH_LIGHT_POS_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//CHECK_MESH_LIGHT_ONOFF
	GET_LUA_POS( CHECK_MESH_LIGHT_ONOFF, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_CHECK_MESH_LIGHT_ONOFF );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	vTemp = ((CX2ViewerMesh*)SiGetObject( OS_MESH ))->GetLightPos();
	pEditBox = NULL;
	swprintf( strNum, L"%.2f", vTemp.x );
	pEditBox = m_Mesh.GetEditBox( UI_EDIT_MESH_LIGHT_POS_X );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.y );
	pEditBox = m_Mesh.GetEditBox( UI_EDIT_MESH_LIGHT_POS_Y );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.z );
	pEditBox = m_Mesh.GetEditBox( UI_EDIT_MESH_LIGHT_POS_Z );
	pEditBox->SetText( strNum );

	//BUT_MESH_RENDER_PARAM
	GET_LUA_POS( BUT_MESH_RENDER_PARAM, nX, nY, nW, nH );
	pControl = m_Mesh.GetControl( UI_BUT_MESH_RENDER_PARAM );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// Mesh Seting End.!
	//////////////////////////////////////////////////////////////////////////
	*/
	//////////////////////////////////////////////////////////////////////////
	// RENDER PARAM SETING
	
	//////////////////////////////////////////////////////////////////////////
	// Particle : 모델뷰 화면에서 나타나는 파티클 관련 UI
#ifndef UPDATE_X2VIEWER_2013 //JHKang
	GET_LUA_POS( LIST_PARTICLE_LIST, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_LIST_PARTICLE_LIST );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	
	GET_LUA_POS( BUT_PARTICLE_DELETE, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_BUT_PARTICLE_DELETE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( STATIC_PARTICLE_TIME, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_STATIC_PARTICLE_TIME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	
	GET_LUA_POS( EDIT_PARTICLE_TIME, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_EDIT_PARTICLE_TIME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_BONESET, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_BUT_PARTICLE_BONESET );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_BONECLEAR, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_BUT_PARTICLE_BONECLEAR );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( CHECK_PARTICLE_TRACE, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_CHECK_PARTICLE_TRACE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( STATIC_PARTICLE_OFFSET, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_STATIC_PARTICLE_OFFSET );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_OFFSET_X, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_EDIT_PARTICLE_OFFSET_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_OFFSET_Y, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_EDIT_PARTICLE_OFFSET_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_OFFSET_Z, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_EDIT_PARTICLE_OFFSET_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( CHECK_PARTICLE_LANDPOS, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_CHECK_PARTICLE_LANDPOS );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( STATIC_PARTICLE_ROT, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_STATIC_PARTICLE_ROT );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );


	GET_LUA_POS( EDIT_PARTICLE_ROT_X, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_EDIT_PARTICLE_ROT_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );


	GET_LUA_POS( EDIT_PARTICLE_ROT_Y, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_EDIT_PARTICLE_ROT_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );


	GET_LUA_POS( EDIT_PARTICLE_ROT_Z, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_EDIT_PARTICLE_ROT_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( CHECK_PARTICLE_APPUNITROT, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_CHECK_PARTICLE_APPUNITROT );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_SAVESEQUENCE, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_BUT_PARTICLE_SAVESEQUENCE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );	

	GET_LUA_POS( BUT_PARTICLE_PARTICLEEDITOR, nX, nY, nW, nH );
	pControl = m_ParticleBasic.GetControl( UI_BUT_PARTICLE_PARTICLEEDITOR );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//////////////////////////////////////////////////////////////////////////
	// Particle Editor UI
	GET_LUA_POS( LIST_PARTICLE_EDITOR_MYPARTICLE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_LIST_PARTICLE_EDITOR_MYPARTICLE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	
	GET_LUA_POS( BUT_PARTICLE_EDITOR_MYPARTICLE_LOAD, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_MYPARTICLE_LOAD );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_EDITOR_MYPARTICLE_DELETE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_MYPARTICLE_DELETE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_EDITOR_MYPARTICLE_SAVE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_MYPARTICLE_SAVE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( LIST_PARTICLE_EDITOR_PARTICLETEMPLET, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_LIST_PARTICLE_EDITOR_PARTICLETEMPLET );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
		
	GET_LUA_POS( BUT_PARTICLE_EDITOR_PARTICLETEMPLET_COPY, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_PARTICLETEMPLET_COPY );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_EDITOR_PARTICLETEMPLET_RELOAD, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_PARTICLETEMPLET_RELOAD );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );	

	GET_LUA_POS( STATIC_PARTICLE_EDITOR_EMITTERATTRIBUTE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_STATIC_PARTICLE_EDITOR_EMITTERATTRIBUTE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( LIST_PARTICLE_EDITOR_EMITTERATTRIBUTE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_LIST_PARTICLE_EDITOR_EMITTERATTRIBUTE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	
	GET_LUA_POS( STATIC_PARTICLE_EDITOR_EMITTERATTRIBUTE_VALUE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_STATIC_PARTICLE_EDITOR_EMITTERATTRIBUTE_VALUE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	
	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_SINGLE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_SINGLE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	
	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_X, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Y, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Z, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( COMBO_PARTICLE_EDITOR_EMITTERATTRIBUTE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_COMBO_PARTICLE_EDITOR_EMITTERATTRIBUTE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

// 	GET_LUA_POS( BUT_PARTICLE_EDITOR_EMITTERATTRIBUTE_DEFAULT, nX, nY, nW, nH );
// 	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_EMITTERATTRIBUTE_DEFAULT );
// 	pControl->SetLocation( nX, nY );
// 	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_EDITOR_EMITTERATTRIBUTE_APPLY, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_EMITTERATTRIBUTE_APPLY );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );


	GET_LUA_POS( LIST_PARTICLE_EDITOR_EVENT, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_LIST_PARTICLE_EDITOR_EVENT );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_EDITOR_EVENT_DELETE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_EVENT_DELETE );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( STATIC_PARTICLE_EDITOR_EVENT_TYPE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_STATIC_PARTICLE_EDITOR_EVENT_TYPE );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( LIST_PARTICLE_EDITOR_EVENT_TYPE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_LIST_PARTICLE_EDITOR_EVENT_TYPE );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( STATIC_PARTICLE_EDITOR_EVENT_TIME, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_STATIC_PARTICLE_EDITOR_EVENT_TIME );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( CHECK_PARTICLE_EDITOR_EVENT_FADE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_CHECK_PARTICLE_EDITOR_EVENT_FADE );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( STATIC_PARTICLE_EDITOR_EVENT_FROM, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_STATIC_PARTICLE_EDITOR_EVENT_FROM );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( STATIC_PARTICLE_EDITOR_EVENT_TO, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_STATIC_PARTICLE_EDITOR_EVENT_TO );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EVENT_TIME1, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME1 );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EVENT_TIME2, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EVENT_TIME2 );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( STATIC_PARTICLE_EDITOR_EVENT_VALUE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_STATIC_PARTICLE_EDITOR_EVENT_VALUE );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EVENT_SINGLE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EVENT_SINGLE );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EVENT_X, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EVENT_X );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EVENT_Y, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EVENT_Y );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EVENT_Z, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EVENT_Z );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( COMBO_PARTICLE_EDITOR_EVENT, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_COMBO_PARTICLE_EDITOR_EVENT );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EVENT_R, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EVENT_R );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EVENT_G, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EVENT_G );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EVENT_B, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EVENT_B );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( EDIT_PARTICLE_EDITOR_EVENT_A, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_EDIT_PARTICLE_EDITOR_EVENT_A );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_EDITOR_EVENT_APPLY, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_EVENT_APPLY );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_EDITOR_EVENT_NEW, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_EVENT_NEW );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( LIST_PARTICLE_EDITOR_MODEL_LIST, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_LIST_PARTICLE_EDITOR_MODEL_LIST );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_EDITOR_MODEL_ADD, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_MODEL_ADD );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_EDITOR_MODEL_DELETE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_MODEL_DELETE );
	pControl->SetLocation( rt.right - 1024 + nX, nY );
	pControl->SetSize( nW, nH );
	
	GET_LUA_POS( STATIC_PARTICLE_EDITOR_PLAY_TYPE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_STATIC_PARTICLE_EDITOR_PLAY_TYPE );
	pControl->SetLocation( rt.right - 1024 + nX, rt.bottom -768 + nY );
	pControl->SetSize( nW, nH );
	GET_LUA_POS( COMBO_PARTICLE_EDITOR_PLAY_TYPE, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_COMBO_PARTICLE_EDITOR_PLAY_TYPE );
	pControl->SetLocation( rt.right - 1024 + nX, rt.bottom -768 + nY );
	pControl->SetSize( nW, nH );
	GET_LUA_POS( BUT_PARTICLE_EDITOR_PLAY, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_PLAY );
	pControl->SetLocation( rt.right - 1024 + nX, rt.bottom -768 + nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( STATIC_PARTICLE_EDITOR_SPEED, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_STATIC_PARTICLE_EDITOR_SPEED );
	pControl->SetLocation( rt.right - 1024 + nX, rt.bottom -768 + nY );
	pControl->SetSize( nW, nH );
	
	GET_LUA_POS( SLIDE_PARTICLE_EDITOR_SPEED, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_SLIDE_PARTICLE_EDITOR_SPEED );
	pControl->SetLocation( rt.right - 1024 + nX, rt.bottom -768 + nY );
	pControl->SetSize( nW, nH );
	
	GET_LUA_POS( STATIC_PARTICLE_EDITOR_TIME, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_STATIC_PARTICLE_EDITOR_TIME );
	pControl->SetLocation( rt.right - 1024 + nX, rt.bottom -768 + nY );
	pControl->SetSize( nW, nH );

	GET_LUA_POS( BUT_PARTICLE_EDITOR_EXIT, nX, nY, nW, nH );
	pControl = m_ParticleEditor.GetControl( UI_BUT_PARTICLE_EDITOR_EXIT );
	pControl->SetLocation( rt.right - 1024 + nX, rt.bottom -768 + nY );
	pControl->SetSize( nW, nH );
#endif //UPDATE_X2VIEWER_2013

	//////////////////////////////////////////////////////////////////////////
	
	m_Param.SetParamDlg( &m_RenderParamUI );

	SiMain()->SetSelectedAnimIndex( -1 );
}
#pragma endregion UI 초기화 ViewerUI.lua에 설정된 좌표 값 얻어오기

#pragma region DropFile
void CX2ViewerUI::DropFile( std::wstring fileName, std::wstring dir )
{
	CX2ViewerSkinMesh*	pSkinMesh	= (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
	CX2ViewerMesh*		pMesh		= (CX2ViewerMesh*)SiGetObject( OS_MESH );

	if( pSkinMesh == NULL && pMesh == NULL )
	{
		WARNINGMSG( L"Skin Mesh/Mesh 가져오기 실패(CX2ViewerUI::DropFile():538) 안돼.." );
		return;
	}

	// 해당 파일이 있는 폴더를 Data Dir에 추가
	std::string strDir;
	ConvertWCHARToChar( strDir, dir.c_str() );

	if ( strDir.size() > 1 )
		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir, true );

	switch( pSkinMesh->InsertSkinMesh( fileName, dir ) )
	{
	case CX2ViewerSkinMesh::SOT_NONE:
		//WARNINGMSG( L"SKIN MESH 로딩 실패.(CX2ViewerUI::DropFile():545)" )
		return;

	case CX2ViewerSkinMesh::SOT_SKINMESH:
		{
			m_AnimationUI.GetListBox( UI_LIST_ANIMATION )->RemoveAllItems();
			m_SkinMeshUI.GetListBox( UI_LIST_BONE )->RemoveAllItems();
			m_EquipUI.GetListBox( UI_LIST_EQUIP )->RemoveAllItems();

			std::vector<std::wstring> vecAnimNameList;
            
            pSkinMesh->SetAttachPoint(L"");
            SiSelf()->m_vecFrameNameList.clear();
			pSkinMesh->GetAnimNameList( vecAnimNameList );
            pSkinMesh->GetFrameNameList( m_vecFrameNameList );
            
            sort(m_vecFrameNameList.begin(), m_vecFrameNameList.end());

			int nAnimMax = (int)vecAnimNameList.size();
            int nFrameMax = (int)m_vecFrameNameList.size();

			//////////////////////////////////////////////////////////////////////////
			// UI 셋팅부분

			// 애니메이션 이름 리스트
			for( int i = 0; i < nAnimMax; ++i )
			{
				m_AnimationUI.GetListBox( UI_LIST_ANIMATION )->AddItem( vecAnimNameList[i].c_str(), (LPVOID)(size_t)0 );
			}

            // 뼈대 이름 리스트
            for( int i = 0; i < nFrameMax; ++i )
            {
                LPCSTR			szName;
                std::wstring	wstrName;
                WCHAR			wszName[128] = L"";

                szName = m_vecFrameNameList[i]->Name;

                MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);

                wstrName = wszName;
                m_SkinMeshUI.GetListBox( UI_LIST_BONE )->AddItem( wstrName.c_str(), (LPVOID)(size_t)0 );
            }

			SetSkinMeshOnOff( true );
			SetEquipUIOnOff( true );
			SetAnimationOnOff( true );

			//애니메이션 속도 초기화
			InitAnimSpeed();

			// 애니메이션 개수
			WCHAR wstrNum[255] = L"";
			swprintf( wstrNum, L"( %d )", nAnimMax );
			m_AnimationUI.GetStatic( UI_STATIC_ANIM_NUM )->SetText( wstrNum );

			CX2ViewerSkinMesh*	pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
			m_Param.GetRenderParam( pSkinMesh->GetRenderParam(), pSkinMesh->GetImpactData(), *(pSkinMesh->GetTexStageData()) );

			//Init();
			//SetUnitOnOff( true );

			//pMesh->Reset();
			SetMeshOnOff( false );
			m_Param.Reset();
			m_MeshSel = MS_SKIN_MESH;
			SetRenderParamOnOff( false );

			SiMain()->SetAnimFileName( fileName.c_str() );
			SiMain()->SetAnimDirName( dir.c_str() );
		}
		break;

	case CX2ViewerSkinMesh::SOT_MESH:
		{
			if ( pSkinMesh )
			{
				m_EquipUI.GetListBox( UI_LIST_EQUIP )->AddItem( fileName.c_str(), (LPVOID)(size_t)0 );
				pSkinMesh->SetMotionOnOff( false );
				m_SkinMeshUI.GetCheckBox( UI_CHECK_MOTION_ONOFF )->SetChecked( false );
			}
		}
		break;

	case CX2ViewerSkinMesh::SOT_NOT_ADDMESH:
		{
			pMesh->SetMesh( fileName.c_str(), dir.c_str() );
			SetMeshOnOff( true );

			SetSkinMeshOnOff( false );
			SetEquipUIOnOff( false );
			SetAnimationOnOff( false );
			pSkinMesh->Reset();
			m_Param.Reset();
			m_MeshSel = MS_MESH;
			SetRenderParamOnOff( false );

			CX2ViewerMesh*	pMesh = (CX2ViewerMesh*)SiGetObject( OS_MESH );
			m_Param.GetRenderParam( pMesh->GetRenderParam(), pMesh->GetImpactData(), *(pMesh->GetTexStageData()) );
		}
		break;
	}

	SetEffectButOnOff( true );
}
#pragma endregion 관련 리소스 파일을 드롭했을 때 처리

#pragma region DrawAnimFrame
void CX2ViewerUI::DrawAnimFrame()
{
	CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );

	float fNow, fMax;
	fNow = fMax = 0.0f;
	pSkinMesh->GetAnimTime( fNow, fMax );

	CDXUTSlider* pSlider = reinterpret_cast<CDXUTSlider*>( SiGetAnimtaionDlg( UI_SLIDE_ANIM_FRAME ) );
	pSlider->SetRange( 0, max( 1, static_cast<int>( fMax / /*0.0333333f*/ 0.001f ) ) );
	pSlider->SetValue( max( 1, static_cast<int>( fNow / /*0.0333333f*/ 0.001f ) ) );
	//Static 문구 출력
	CDXUTStatic* pStatic = reinterpret_cast<CDXUTStatic*>( SiGetAnimtaionDlg( UI_STATIC_ANIM_FRAME ) );
	WCHAR wszSpeed[128] = L"";
	swprintf( wszSpeed, L"Frame : %d / %d\n %.3f / %.3f", (int)(fNow/0.033333f), (int)(fMax/0.033333f), fNow, fMax );
	pStatic->SetText( wszSpeed );
}
#pragma endregion 애니메이션 프레임 갱신

#pragma region DrawParticleTime
#ifndef UPDATE_X2VIEWER_2013 //JHKang
// 파티클 에디터에서 해당 파티클의 작동 시간
void CX2ViewerUI::DrawParticleTime()
{
	CX2ViewerParticle* pParticleObj = (CX2ViewerParticle*)(SiGetObject( OS_PARTICLE ));
	CDXUTStatic* pStatic = (CDXUTStatic*)SiGetParticleEditorDlg( UI_STATIC_PARTICLE_EDITOR_TIME );
	
	if( pParticleObj != NULL )
	{
		WCHAR wszSpeed[64];
		swprintf( wszSpeed, L"%.3f", pParticleObj->GetPreviewPlayTime() );
		pStatic->SetText( wszSpeed );
	}
}
#endif //UPDATE_X2VIEWER_2013
#pragma endregion 파티클 시간 갱신

#pragma region InitAnimSpeed
void CX2ViewerUI::InitAnimSpeed()
{
	CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
	CDXUTSlider* pSlider = reinterpret_cast<CDXUTSlider*>( SiGetAnimtaionDlg( UI_SLIDE_ANIM_FRAME ) );
	float fNow, fMax;
	fNow = fMax = 0.0f;
	pSkinMesh->GetAnimTime( fNow, fMax );
	pSlider->SetRange( 0, max( 1, static_cast<int>( fMax / /*0.0333333f*/ 0.001f ) ) );
	pSlider->SetValue( static_cast<int>( fNow / /*0.0333333f*/ 0.001f ) );

	pSlider = reinterpret_cast<CDXUTSlider*>( SiGetAnimtaionDlg( UI_SLIDE_ANIM_SPEED ) );
	pSlider->SetValue( 100 );

	//Static 문구 출력
	CDXUTStatic* pStatic = reinterpret_cast<CDXUTStatic*>( SiGetAnimtaionDlg( UI_STATIC_ANIM_SPEED ) );
	WCHAR wszSpeed[128] = L"";
	swprintf( wszSpeed, L"speed : %d", 100 );
	pStatic->SetText( wszSpeed );
}
#pragma endregion 애니메이션 속도 초기화

#pragma region InitParticleEditor RefreshEvent
// 파티클 에디터 초기화 관련 코드
#ifndef UPDATE_X2VIEWER_2013 //JHKang
void CX2ViewerUI::InitParticleEditor()
{
	CX2ViewerParticle* pParticleObj = (CX2ViewerParticle*)(SiGetObject( OS_PARTICLE ));
	CDXUTListBox* pCustomListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_MYPARTICLE);
	CDXUTListBox* pTempletListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_PARTICLETEMPLET);
	CDXUTListBox* pModelParticleListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_MODEL_LIST);
	CDXUTListBox* pEventListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_EVENT);
	
	CDXUTListBox* pEmitterPropertiesListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_EMITTERATTRIBUTE);
	CDXUTListBox* pEventTypeListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_EVENT_TYPE);

	// list update
	{
		pCustomListBox->RemoveAllItems();
		const map<wstring, CKTDGParticleSystem::CParticleEventSequence*>& templetSeq = pParticleObj->GetCustomParticleSystem()->GetTempletSequences();
		map<wstring, CKTDGParticleSystem::CParticleEventSequence*>::const_iterator it;
		for( it=templetSeq.begin(); it != templetSeq.end(); it++ )
		{
			wstring& wstrName = (wstring)it->first;
			pCustomListBox->AddItem( wstrName.c_str(), NULL );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	{
		pTempletListBox->RemoveAllItems();
		const map<wstring, CKTDGParticleSystem::CParticleEventSequence*>& templetSeq = pParticleObj->GetParticleSystem()->GetTempletSequences();
		map<wstring, CKTDGParticleSystem::CParticleEventSequence*>::const_iterator it;
		for( it=templetSeq.begin(); it != templetSeq.end(); it++ )
		{
			wstring& wstrName = (wstring)it->first;
			pTempletListBox->AddItem( wstrName.c_str(), NULL );
		}

	}
	
	//////////////////////////////////////////////////////////////////////////
	{
		pModelParticleListBox->RemoveAllItems();
		
		vector<CX2ViewerParticle::ParticleEffectData*> vecParticleEffect = pParticleObj->GetParticleEffectData();
		for( vector<CX2ViewerParticle::ParticleEffectData*>::iterator it=vecParticleEffect.begin(); it != vecParticleEffect.end(); it++ )
		{
			CX2ViewerParticle::ParticleEffectData* pData = *it;
			if( pData == NULL )
				continue;
			pModelParticleListBox->AddItem( pData->m_Name.c_str(), NULL );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	pEventListBox->RemoveAllItems();
	//////////////////////////////////////////////////////////////////////////
	pEmitterPropertiesListBox->RemoveAllItems();
	
	for( int i = 0; i < CX2ViewerParticleEditor::EP_END; ++i )
	{
		CX2ViewerParticleEditor::PropertyData* refData = pParticleObj->GetParticleEditor().GetEmitterProperties( (CX2ViewerParticleEditor::EMITTER_PROPERTIES)i );
		pEmitterPropertiesListBox->AddItem( refData->m_name.c_str(), (void*)refData );
	}
	//////////////////////////////////////////////////////////////////////////
	pEventTypeListBox->RemoveAllItems();
	for( int i = 0; i < CKTDGParticleSystem::ET_END; ++i )
	{
		CX2ViewerParticleEditor::PropertyData* refData = pParticleObj->GetParticleEditor().GetEventProperties( (CKTDGParticleSystem::EVENT_TYPE)i );
		if( refData->m_name == L"" ) continue;
		pEventTypeListBox->AddItem( refData->m_name.c_str(), (void*)refData );
	}
	
	CDXUTComboBox* pComboBox = (CDXUTComboBox*)m_ParticleEditor.GetControl( UI_COMBO_PARTICLE_EDITOR_PLAY_TYPE );
	pComboBox->RemoveAllItems();
	pComboBox->AddItem( L"ONE", NULL );
	pComboBox->AddItem( L"LOOP", NULL );
	if( pParticleObj->GetPreviewPlayMode() )
	{
		pComboBox->SetSelectedByText( L"LOOP" );
	}
	else
	{
		pComboBox->SetSelectedByText( L"ONE" );
	}

	CDXUTSlider* pSlider = (CDXUTSlider*)SiGetParticleEditorDlg( UI_SLIDE_PARTICLE_EDITOR_SPEED );
	pSlider->SetValue( 100 );
	pParticleObj->SetPreviewPlaySpeed( 1 );

	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_SINGLE )->SetVisible( false );
	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_X )->SetVisible( false );
	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Y )->SetVisible( false );
	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Z )->SetVisible( false );
	SiGetParticleEditorDlg( UI_COMBO_PARTICLE_EDITOR_EMITTERATTRIBUTE )->SetVisible( false );

	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_SINGLE )->SetVisible( false );
	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_X )->SetVisible( false );
	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Y )->SetVisible( false );
	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_Z )->SetVisible( false );
	SiGetParticleEditorDlg( UI_COMBO_PARTICLE_EDITOR_EVENT )->SetVisible( false );
	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_R )->SetVisible( false );
	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_G )->SetVisible( false );
	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_B )->SetVisible( false );
	SiGetParticleEditorDlg( UI_EDIT_PARTICLE_EDITOR_EVENT_A )->SetVisible( false );
}

// 이벤트 리스트 갱신
void CX2ViewerUI::RefreshEventList()
{

	CX2ViewerParticle* pParticleObj = (CX2ViewerParticle*)(SiGetObject( OS_PARTICLE ));
	CX2ViewerParticleEditor& refParticleEditor = pParticleObj->GetParticleEditor();
	CDXUTListBox* pCustomListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_MYPARTICLE);
	
	CDXUTListBox* pParticleEventListBox = (CDXUTListBox*) SiGetParticleEditorDlg(UI_LIST_PARTICLE_EDITOR_EVENT);


	pParticleEventListBox->RemoveAllItems();

	if( pCustomListBox->GetSelectedItem() == NULL )
		return;

	wstring name(pCustomListBox->GetSelectedItem()->strText);
	// 취급주의 : const_cast
	CKTDGParticleSystem::CParticleEventSequence* pTempletSeq = 
		const_cast<CKTDGParticleSystem::CParticleEventSequence*>(pParticleObj->GetCustomParticleSystem()->GetTempletSequencesByName( name ));

	if( pTempletSeq == NULL )
		return;

	//vector<CKTDGParticleSystem::CParticleEvent*>* pvecEvent = pTempletSeq->GetEventList();
	CKTDGParticleSystem::CParticleEventList* pvecEvent = pTempletSeq->GetEventList();

	//for( vector<CKTDGParticleSystem::CParticleEvent*>::iterator it = pvecEvent->begin(); it < pvecEvent->end(); ++it )
	for( list< CKTDGParticleSystem::CParticleEvent* >::iterator it = pvecEvent->begin(); it != pvecEvent->end(); ++it )
	{
		CKTDGParticleSystem::CParticleEvent* pEvent = *it;
		if( pEvent == NULL )
			continue;

		wstringstream wstrm;
		CX2ViewerParticleEditor::PropertyData* pPropertyData = refParticleEditor.GetEventProperties( pEvent->GetEventType() );
		if( pPropertyData == NULL )
			continue;
		wstrm << pPropertyData->m_name;
		wstrm << L" (" << refParticleEditor.GetEventTimeString( pEvent, pTempletSeq->GetLifeTime().m_Max ) << L")";
		pParticleEventListBox->AddItem( wstrm.str().c_str(), (void*)pEvent );

	}
}
#endif //UPDATE_X2VIEWER_2013
#pragma endregion 파티클 초기화 및 이벤트 갱신

#pragma region GetMotionFileList
void CX2ViewerUI::GetMotionFileList( wstring strParamPath_ )
{
	wstring strPath, strFilePath;

	strPath = strParamPath_;
	strPath += L"*.*";

	WIN32_FIND_DATA fileData;
	HANDLE searchHandle = FindFirstFile( strPath.c_str(), &fileData );

	if ( INVALID_HANDLE_VALUE != searchHandle )
	{
		do
		{
			if ( FILE_ATTRIBUTE_DIRECTORY & fileData.dwFileAttributes )
			{
				if ( fileData.cFileName[0] != '.' )
				{
					strFilePath = strParamPath_;
					strFilePath += fileData.cFileName;
					strFilePath += L"\\";
					GetMotionFileList( strFilePath );
				}
			}
			else
			{
				m_strMotionFileList.push_back( fileData.cFileName );
			}
		} while( FindNextFile( searchHandle, &fileData ) );

		FindClose( searchHandle );
	}
}
#pragma endregion 

#pragma region SetShow
void CX2ViewerUI::SetShow( bool bValue_ /* = false */ )
{
	m_BaseUI.SetVisible( bValue_ );

	if ( bValue_ == true )
	{
		m_SkinMeshUI.SetVisible( m_bSkinMeshUI );
		m_EquipUI.SetVisible( m_bSkinMeshUI );
		m_AnimationUI.SetVisible( m_bSkinMeshUI );
		m_RenderParamUI.SetVisible( m_bSkinMeshUI );
	}
	else
	{
		m_SkinMeshUI.SetVisible( bValue_ );
		m_EquipUI.SetVisible( bValue_ );
		m_AnimationUI.SetVisible( bValue_ );
		m_RenderParamUI.SetVisible( bValue_ );
	}

	if ( bValue_ == true )
	{
		m_BoneUI.SetVisible( m_bModifyBone );
		m_WeaponUI.SetVisible( m_bWeaponUI );
		m_AccessoryUI.SetVisible( m_bAccessoryUI );
	}
	else
	{
		m_BoneUI.SetVisible( bValue_ );
		m_WeaponUI.SetVisible( bValue_ );
		m_AccessoryUI.SetVisible( bValue_ );
	}
}
#pragma endregion 모든 UI 숨기기

#pragma region ChangeEditBoxValueToWheel
float CX2ViewerUI::ChangeEditBoxValueToWheel( UI_ID eValue_, CDXUTDialog* pDialog, float fUpValue_/* = 1.0f*/ )
{
	CDXUTEditBox* pEditBox = reinterpret_cast<CDXUTEditBox*>( pDialog->GetControl( eValue_ ) );

	int iDelta = pEditBox->GetWheelDeltaValue();
	float fTemp = (float)_wtof( pEditBox->GetText() );

	if ( pEditBox->m_bHasFocus == false )
		return fTemp;

	if ( iDelta != 0 )
	{
		WCHAR strNum[128] = L"";
		swprintf( strNum, L"%.2f", fTemp + static_cast<float>( iDelta ) * fUpValue_ );
		pEditBox->SetText( strNum );
		pEditBox->SetWheelDeltaValue( 0 );
	}					
	
	return fTemp;
}
#pragma endregion EditBox 내부 숫자 증감 값 지정

#pragma region OpenEquip
void CX2ViewerUI::OpenEquip()
{
	OPENFILENAME	OFN;
	WCHAR			lpstrFile[50000] = L"";
	WCHAR			wcDirectory[MAX_PATH] = L"";
	WCHAR			wcFileName[MAX_PATH] = L"";
	WCHAR			wcAddItem[MAX_PATH] = L"";
	WCHAR			*p;
	int				i = 1;

	memset( &OFN, 0, sizeof(OPENFILENAME) );
	OFN.lStructSize		= sizeof(OPENFILENAME);
	OFN.hwndOwner		= g_pKTDXApp->GetHWND();
	OFN.lpstrFilter		= L"X-file(*.x)\0*.x\0";
	OFN.lpstrFile		= lpstrFile;
	OFN.nMaxFile		= 10000;
	OFN.Flags			= OFN_EXPLORER | OFN_ALLOWMULTISELECT;

	if( GetOpenFileName(&OFN) != 0 ) 
	{
		p = lpstrFile;
		wcscpy( wcDirectory, p );
		wcscpy( wcFileName, wcDirectory );

		if( *(p + wcslen( wcFileName ) + 1) == 0 )
		{
			WCHAR drive[10] = L"";
			WCHAR dir[256]   = L"";
			WCHAR fname[256] = L"";
			WCHAR ext[10]    = L"";

			WCHAR fileName[256] = L"";
			WCHAR PullDir[256] = L"";
			_wsplitpath( wcFileName, drive, dir, fname, ext);

			wcscat( fileName, fname);
			wcscat( fileName, ext);
			wcscat( PullDir, drive );
			wcscat( PullDir, dir );

			SiSelf()->DropFile( fileName, PullDir );
		}
		else
		{
			for (;;) 
			{
				p = p + wcslen( wcFileName ) + 1;
				if( *p == 0 )
				{
					break;
				}

				wcscpy( wcFileName, p );
				wcscpy( wcAddItem, wcDirectory );
				wcscat( wcAddItem, L"\\" );
				wcscat( wcAddItem, wcFileName );

				SiSelf()->DropFile( wcFileName, wcDirectory );
			}
		}
	}
	else 
	{
		if( CommDlgExtendedError() == FNERR_BUFFERTOOSMALL )
		{
			ERRORMSG( L"버퍼 크기가 너무 작습니다" );
		}
	}
}
#pragma endregion 장비 열기

#pragma region OpenWeapon
void CX2ViewerUI::OpenWeapon()
{
	CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );

	if ( pSkinMesh != NULL )
	{
		CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_WEAPON ) );

		D3DXVECTOR3 vAddRotWeapon( 0.f, 0.f, 0.f );
		OPENFILENAME	OFN;
		WCHAR			lpstrFile[50000] = L"";
		WCHAR			wcDirectory[MAX_PATH] = L"";
		WCHAR			wcFileName[MAX_PATH] = L"";
		WCHAR			wcAddItem[MAX_PATH] = L"";
		WCHAR			*p;
		int				i = 1;

		memset( &OFN, 0, sizeof(OPENFILENAME) );
		OFN.lStructSize		= sizeof(OPENFILENAME);
		OFN.hwndOwner		= g_pKTDXApp->GetHWND();
		OFN.lpstrFilter		= L"X-file(*.x)\0*.x\0";
		OFN.lpstrFile		= lpstrFile;
		OFN.nMaxFile		= 10000;
		OFN.Flags			= OFN_EXPLORER | OFN_ALLOWMULTISELECT;

		if( GetOpenFileName(&OFN) != 0 ) 
		{
			p = lpstrFile;
			wcscpy( wcDirectory, p );
			wcscpy( wcFileName, wcDirectory );

			if( *(p + wcslen( wcFileName ) + 1) == 0 )
			{
				pSkinMesh->AddWeapon( lpstrFile, L"Bip01", vAddRotWeapon );

				WCHAR drive[10] = L"";
				WCHAR dir[256]   = L"";
				WCHAR fname[256] = L"";
				WCHAR ext[10]    = L"";

				WCHAR fileName[256] = L"";
				WCHAR PullDir[256] = L"";
				_wsplitpath( wcFileName, drive, dir, fname, ext);

				wcscat( fileName, fname);
				wcscat( fileName, ext);
				wcscat( PullDir, drive );
				wcscat( PullDir, dir );

				pListBox->AddItem( fileName, NULL );
			}
			else
			{
				for (;;) 
				{
					p = p + wcslen( wcFileName ) + 1;
					if( *p == 0 )
					{
						break;
					}

					wcscpy( wcFileName, p );
					wcscpy( wcAddItem, wcDirectory );
					wcscat( wcAddItem, L"\\" );
					wcscat( wcAddItem, wcFileName );

					pSkinMesh->AddWeapon( wcAddItem, L"Bip01", vAddRotWeapon );
					pListBox->AddItem( wcFileName, NULL );
				}
			}

			SiSelf()->SetWeaponOnOff( true );
			SiSelf()->m_bWeaponUI = true;
		}
		else 
		{
			if( CommDlgExtendedError() == FNERR_BUFFERTOOSMALL )
			{
				ERRORMSG( L"버퍼 크기가 너무 작습니다" );
			}
		}
	}
}
#pragma endregion 무기 열기

#pragma region OpenAccessory
void CX2ViewerUI::OpenAccessory()
{
	CX2ViewerSkinMesh* pSkinMesh = (CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH );
	D3DXVECTOR3 vAddTransAccessory( 0.f, 0.f, 0.f );

	if ( pSkinMesh != NULL )
	{
		CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_ACCESSORY ) );

		D3DXVECTOR3 vAddRotWeapon( 0.f, 0.f, 0.f );
		OPENFILENAME	OFN;
		WCHAR			lpstrFile[50000] = L"";
		WCHAR			wcDirectory[MAX_PATH] = L"";
		WCHAR			wcFileName[MAX_PATH] = L"";
		WCHAR			wcAddItem[MAX_PATH] = L"";
		WCHAR			*p;
		int				i = 1;

		memset( &OFN, 0, sizeof(OPENFILENAME) );
		OFN.lStructSize		= sizeof(OPENFILENAME);
		OFN.hwndOwner		= g_pKTDXApp->GetHWND();
		OFN.lpstrFilter		= L"Y-file(*.y)\0*.y\0X-file(*.x)\0*.x";
		OFN.lpstrFile		= lpstrFile;
		OFN.nMaxFile		= 10000;
		OFN.Flags			= OFN_EXPLORER | OFN_ALLOWMULTISELECT;

		if( GetOpenFileName(&OFN) != 0 ) 
		{
			p = lpstrFile;
			wcscpy( wcDirectory, p );
			wcscpy( wcFileName, wcDirectory );

			if( *(p + wcslen( wcFileName ) + 1) == 0 )
			{
				pSkinMesh->AddAccessory( lpstrFile, L"Bip01", vAddTransAccessory );

				WCHAR drive[10] = L"";
				WCHAR dir[256]   = L"";
				WCHAR fname[256] = L"";
				WCHAR ext[10]    = L"";

				WCHAR fileName[256] = L"";
				WCHAR PullDir[256] = L"";
				_wsplitpath( wcFileName, drive, dir, fname, ext);

				wcscat( fileName, fname);
				wcscat( fileName, ext);
				wcscat( PullDir, drive );
				wcscat( PullDir, dir );

				pListBox->AddItem( fileName, NULL );
			}
			else
			{
				for (;;) 
				{
					p = p + wcslen( wcFileName ) + 1;
					if( *p == 0 )
					{
						break;
					}

					wcscpy( wcFileName, p );
					wcscpy( wcAddItem, wcDirectory );
					wcscat( wcAddItem, L"\\" );
					wcscat( wcAddItem, wcFileName );

					pSkinMesh->AddAccessory( wcAddItem, L"Bip01", vAddTransAccessory );
					pListBox->AddItem( wcFileName, NULL );
				}
			}

			SiSelf()->SetAccessoryOnOff( true );
			SiSelf()->m_bAccessoryUI = true;
		}
		else 
		{
			if( CommDlgExtendedError() == FNERR_BUFFERTOOSMALL )
			{
				ERRORMSG( L"버퍼 크기가 너무 작습니다" );
			}
		}
	}
}
#pragma endregion 악세서리 열기

#pragma region OpenScriptFile
bool CX2ViewerUI::OpenScriptFile( IN const WCHAR* pFileName_, KLuabinder* pKLuabinder )
{
	if ( pKLuabinder == NULL )
		pKLuabinder = g_pKTDXApp->GetLuaBinder();

	lua_tinker::decl( pKLuabinder->GetLuaState(), "g_pViewerUI", this );

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName_ );
	if ( Info == NULL )
	{
		ERRORMSG( L"lua 파일열기 실패" );
		return false;
	}

	if( pKLuabinder->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	{
		ERRORMSG( L"lua 파싱 실패" );
		return false;
	}

	return true;
}
#pragma endregion g_pViewerUI 함수를 사용하는 스크립트 파일 로딩

#pragma region AddEquip
bool CX2ViewerUI::AddEquip()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	int index = 0;
	wstring strFileName;
	
	while( true == luaManager.BeginTable( L"EQUIP", index ) )
	{
		LUA_GET_VALUE( luaManager, 1, strFileName, L"" );
		SiSelf()->DropFile( strFileName.c_str(), L"" );

		++index;
		luaManager.EndTable();
	}

	return true;
}
#pragma endregion 스크립트를 통한 장비 목록 열기

#pragma region AddWeapon
bool CX2ViewerUI::AddWeapon()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	int index = 0;
	wstring strFileName, strBoneName;
	D3DXVECTOR3 vAddRotWeapon( 0.f, 0.f, 0.f );
	CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
	CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_WEAPON ) );

	if ( pSkinMesh == NULL )
		return false;

	if ( pListBox == NULL )
		return false;

	while( true == luaManager.BeginTable( L"WEAPON", index ) )
	{
		LUA_GET_VALUE( luaManager, 1, strFileName, L"" );
		LUA_GET_VALUE( luaManager, 2, strBoneName, L"" );

		pSkinMesh->AddWeapon( strFileName.c_str(), strBoneName.c_str(), vAddRotWeapon );
		pListBox->AddItem( strFileName.c_str(), NULL );

		++index;
		luaManager.EndTable();
	}

	return true;
}
#pragma endregion 스크립트를 통한 무기 목록 열기

#pragma region AddAccessory
bool CX2ViewerUI::AddAccessory()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	int index = 0;
	wstring strFileName, strBoneName;
	D3DXVECTOR3 vAddTransAccessory( 0.f, 0.f, 0.f );
	CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
	CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_ACCESSORY ) );

	if ( pSkinMesh == NULL )
		return false;

	if ( pListBox == NULL )
		return false;

	while( true == luaManager.BeginTable( L"ACCESSORY", index ) )
	{
		LUA_GET_VALUE( luaManager, 1, strFileName, L"" );
		LUA_GET_VALUE( luaManager, 2, strBoneName, L"" );

		pSkinMesh->AddAccessory( strFileName.c_str(), strBoneName.c_str(), vAddTransAccessory );
		pListBox->AddItem( strFileName.c_str(), NULL );

		++index;
		luaManager.EndTable();
	}

	return true;
}
#pragma endregion 스크립트를 통한 악세서리 목록 열기

#pragma region SaveScene
void CX2ViewerUI::SaveScene()
{
	OPENFILENAME	OFN;
	WCHAR			lpstrFile[50000] = L"";
	WCHAR			wcDirectory[MAX_PATH] = L"";
	WCHAR			wcFileName[MAX_PATH] = L"";
	WCHAR			wcAddItem[MAX_PATH] = L"";
	WCHAR			*p;
	int				i = 1;

	memset( &OFN, 0, sizeof(OPENFILENAME) );
	OFN.lStructSize		= sizeof(OPENFILENAME);
	OFN.hwndOwner		= g_pKTDXApp->GetHWND();
	OFN.lpstrFilter		= L"Lua-file(*.lua)\0*.lua\0";
	OFN.lpstrFile		= lpstrFile;
	OFN.nMaxFile		= 10000;
	OFN.Flags			= OFN_EXPLORER;

	if( GetSaveFileName( &OFN ) != 0 ) 
	{
		WCHAR drive[_MAX_DRIVE];
		WCHAR dir[_MAX_DIR];
		WCHAR fname[_MAX_FNAME];
		WCHAR ext[_MAX_EXT];

		p = lpstrFile;
		wcscpy( wcDirectory, p );
		wcscpy( wcFileName, wcDirectory );

		_wsplitpath( wcFileName, drive, dir, fname, ext);

		string strFileName;
		ConvertWCHARToChar( strFileName, fname );
		strFileName += ".lua";
		
		std::wfstream fstrm;
		fstrm.open( strFileName.c_str(), std::fstream::out | std::fstream::trunc );
		
		fstrm << L"-- lua header. UTF-8" << std::endl << std::endl;
		
		CX2ViewerSkinMesh* pSkinMesh = reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ) );
		CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_EQUIP ) );
		int iSize = pListBox->GetSize();

		fstrm << L"g_pViewerUI:AddEquip" << std::endl << L"{" << std::endl;
		for ( int i = 0; i < iSize; ++i )
		{
			fstrm << L"\t" << L"EQUIP" << i << " = " << "{ " << "\"" << pListBox->GetItem( i )->strText << "\"" << ", ";
			fstrm << "}," << std::endl;
		}
		fstrm << "}" << std::endl;

		pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_WEAPON ) );
		iSize = pListBox->GetSize();

		fstrm << L"g_pViewerUI:AddWeapon" << std::endl << L"{" << std::endl;
		for ( int i = 0; i < iSize; ++i )
		{
			SWeaponInfo* pWeaponInfo = pSkinMesh->GetWeaponInfo( pListBox->GetItem( i )->strText );
			fstrm << L"\t" << L"WEAPON" << i << " = " << "{ " << "\"" << pListBox->GetItem( i )->strText << "\"" << ", ";
			fstrm << "\"" << pWeaponInfo->m_BoneName << "\"" << ", ";
			fstrm << "}," << std::endl;
		}
		fstrm << "}" << std::endl;

		pListBox = reinterpret_cast<CDXUTListBox*>( SiGetEquipDlg( UI_LIST_ACCESSORY ) );
		iSize = pListBox->GetSize();

		fstrm << L"g_pViewerUI:AddAccessory" << std::endl << L"{" << std::endl;
		for ( int i = 0; i < iSize; ++i )
		{
			SAccessoryInfo* pWeaponInfo = pSkinMesh->GetAccessoryInfo( pListBox->GetItem( i )->strText );
			fstrm << L"\t" << L"ACCESSORY" << i << " = " << "{ " << "\"" << pListBox->GetItem( i )->strText << "\"" << ", ";
			fstrm << "\"" << pWeaponInfo->m_BoneName << "\"" << ", ";
			fstrm << "}," << std::endl;
		}
		fstrm << "}" << std::endl;

		fstrm.close();
		ConvertFileAnsiToUTF8( strFileName, strFileName );
	}
}
#pragma endregion 신 저장

#pragma region SaveBoneFile
void CX2ViewerUI::SaveBoneFile()
{
	OPENFILENAME	OFN;
	WCHAR			lpstrFile[50000] = L"";
	WCHAR			wcDirectory[MAX_PATH] = L"";
	WCHAR			wcFileName[MAX_PATH] = L"";
	WCHAR			wcAddItem[MAX_PATH] = L"";
	WCHAR			*p;
	int				i = 1;

	memset( &OFN, 0, sizeof(OPENFILENAME) );
	OFN.lStructSize		= sizeof(OPENFILENAME);
	OFN.hwndOwner		= g_pKTDXApp->GetHWND();
	OFN.lpstrFilter		= L"Lua-file(*.lua)\0*.lua\0";
	OFN.lpstrFile		= lpstrFile;
	OFN.nMaxFile		= 10000;
	OFN.Flags			= OFN_EXPLORER;

	if( GetSaveFileName( &OFN ) != 0 ) 
	{
		WCHAR drive[_MAX_DRIVE];
		WCHAR dir[_MAX_DIR];
		WCHAR fname[_MAX_FNAME];
		WCHAR ext[_MAX_EXT];

		p = lpstrFile;
		wcscpy( wcDirectory, p );
		wcscpy( wcFileName, wcDirectory );

		_wsplitpath( wcFileName, drive, dir, fname, ext);

		string strFileName;
		ConvertWCHARToChar( strFileName, fname );
		strFileName += ".lua";

		CDXUTComboBox* pCombotBox = reinterpret_cast<CDXUTComboBox*>( SiGetSkinMeshDlg( UI_COMBO_DEFAULT_BONE ) );
		CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( SiGetSkinMeshDlg( UI_LIST_MODIFY_BONE ) );
								
		if ( pCombotBox == NULL )
		{
			WARNINGMSG( L"UI_COMBO_DEFAULT_BONE를 찾지 못햇습니다." );
			return;
		}

		MapCharacterID::iterator mapIter;
		CHARACTER_ID eCharID = static_cast<CHARACTER_ID>( pCombotBox->GetSelectedIndex() );
		mapIter = SiSelf()->m_mapCharacterIDStr.find( eCharID );
				
		if( mapIter == SiSelf()->m_mapCharacterIDStr.end() )
		{
			WARNINGMSG( L"Character ID String 읽기오류" );
			return;
		}

		if ( pListBox == NULL )
		{
			WARNINGMSG( L"UI_LIST_MODIFY_BONE를 찾지 못햇습니다." );
			return;
		}

		int iSize = pListBox->GetSize();

		std::wfstream fstrm;
		fstrm.open( strFileName.c_str(), std::fstream::out | std::fstream::trunc );

		fstrm << L"-- lua header. UTF-8" << std::endl << std::endl;

		fstrm << L"g_pViewerSkinMesh:AddBoneInfo" << std::endl << L"{" << std::endl;
		fstrm << L"\t" << L"UNIT_ID = CHARACTER_ID[\"" << mapIter->second << L"\"]," << std::endl;
		
		LPCSTR			szName;
		std::wstring	wstrName;
		WCHAR			wszName[128] = L"";
		int				index = 1;

		BOOST_FOREACH( CKTDXDeviceXSkinMesh::MultiAnimFrame* pMAF, SiSelf()->m_vecModifyFrameNameListBase )
		{
			szName = pMAF->Name;
			MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);
			wstrName = wszName;

			if ( pMAF != NULL )
			{
				if ( IsSamef( pMAF->m_vScaleRel.x, 1.f ) == false || IsSamef( pMAF->m_vScaleRel.y, 1.f ) == false || 
					IsSamef( pMAF->m_vScaleRel.z, 1.f ) == false )
				{
					fstrm << L"\t" << L"BONE_MODIFY" << index << " = " << "{ " << "\"" << wstrName.c_str() << "\"" << ", ";
					fstrm << L"MODIFY_BONE_STYLE[\"MBS_SCALE\"], " << pMAF->m_vScaleRel.x << L", " 
					<< pMAF->m_vScaleRel.y << L", " << pMAF->m_vScaleRel.z << L", }," << std::endl;

					++index;
				}
				
				if ( IsSamef( pMAF->m_fMoveBoneY ) == false )
				{
					fstrm << L"\t" << L"BONE_MODIFY" << index << " = " << "{ " << "\"" << wstrName.c_str() << "\"" << ", ";
					fstrm << L"MODIFY_BONE_STYLE[\"MBS_MOVE\"], " << L"0.0" << L", " << pMAF->m_fMoveBoneY << L", "
						<< L"0.0, }," << std::endl;

					++index;
				}
			}
			else
			{
				WARNINGMSG( L"뼈대를 찾지 못햇습니다." );
				return;
			}
		}
		fstrm << "}" << std::endl;

		fstrm.close();
		ConvertFileAnsiToUTF8( strFileName, strFileName );
	}
}
#pragma endregion 본 변형 정보 저장

#pragma region InitBaseUI
void CX2ViewerUI::InitBaseUI()
{
	m_BaseUI.Init( &g_DialogResourceManager );
	m_BaseUI.SetCallback( OnGUIBaseEvent );
	m_BaseUI.SetCaptionText( L"기본 UI [F3]" );
	m_BaseUI.EnableCaption( true );
	m_BaseUI.SetSize( 130, 880 );
	m_BaseUI.EnableKeyboardInput( true );
	m_BaseUI.SetBackgroundColors( D3DCOLOR_ARGB( 55, 98, 138, 206 ), D3DCOLOR_ARGB( 55, 54, 105, 192 ), 
		D3DCOLOR_ARGB( 55, 54, 105, 192 ), D3DCOLOR_ARGB( 55, 10,  73, 179 ) );

	m_BaseUI.AddStatic( UI_STATIC_FILE, L"X/Y파일", 0, 0, 0, 0 );
	m_BaseUI.AddButton( UI_BUT_OPEN, L"열기", 0, 0, 0, 0 );
	m_BaseUI.AddButton( UI_BUT_RESET, L"리셋", 0, 0, 0, 0 );

	m_BaseUI.AddStatic( UI_STATIC_UI, L"UI", 0, 0, 0, 0 );
	m_BaseUI.AddButton( UI_BUT_UI_INIT, L"초기화(F1)", 0, 0, 0, 0 );
	m_BaseUI.AddButton( UI_BUT_UI_HIDE, L"숨기기(F2)", 0, 0, 0, 0 );

	m_BaseUI.AddStatic( UI_STATIC_VIEW, L"화면", 0, 0, 0, 0 );
	m_BaseUI.AddCheckBox( UI_CHECK_GRID, L"격자/축", 0, 0, 0, 0, true );
	m_BaseUI.AddCheckBox( UI_CHECK_WIREFRAME, L"와이어 프레임", 0, 0, 0, 0 );

	m_BaseUI.AddStatic( UI_STATIC_LIGHT_POS, L"광원", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_LIGHT_POS_X, L"X", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_LIGHT_POS_Y, L"Y", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_LIGHT_POS_Z, L"Z", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_LIGHT_POS_X, L"", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_LIGHT_POS_Y, L"", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_LIGHT_POS_Z, L"", 0, 0, 0, 0 );
	m_BaseUI.AddCheckBox( UI_CHECK_LIGHT_ONOFF, L"켜기/끄기", 0, 0, 0, 0, true );

	m_BaseUI.AddStatic( UI_STATIC_CAMERAMODE, L"카메라", 0, 0, 0, 0 );
	m_BaseUI.AddRadioButton( UI_RADIO_CAMERA_NORMAL, 1, L"기본", 0, 0, 0, 0, true );
	m_BaseUI.AddRadioButton( UI_RADIO_CAMERA_NAVIGATION, 1, L"자유", 0, 0, 0, 0 );
	m_BaseUI.AddButton( UI_BUT_CAMERA_RESET, L"원위치", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_CAMERA_EYE_POS_X, L"EyeX", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_CAMERA_EYE_POS_Y, L"EyeY", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_CAMERA_EYE_POS_Z, L"EyeZ", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_CAMERA_EYE_POS_X, L"", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_CAMERA_EYE_POS_Y, L"", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_CAMERA_EYE_POS_Z, L"", 0, 0, 0, 0 );

	m_BaseUI.AddStatic( UI_STATIC_CAMERA_LOOK_POS_X, L"LookX", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_CAMERA_LOOK_POS_Y, L"LookY", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_CAMERA_LOOK_POS_Z, L"LookZ", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_CAMERA_LOOK_POS_X, L"", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_CAMERA_LOOK_POS_Y, L"", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_CAMERA_LOOK_POS_Z, L"", 0, 0, 0, 0 );

	m_BaseUI.AddStatic( UI_STATIC_MODEL, L"모션 리스트", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_MODEL, L"", 0, 0, 0, 0 );
	m_BaseUI.AddListBox( UI_LIST_MODEL, 0, 0, 0, 0 );
	
	m_BaseUI.AddStatic( UI_STATIC_BGSET, L"B.G Color", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_BG_A, L"A", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_BG_R, L"R", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_BG_G, L"G", 0, 0, 0, 0 );
	m_BaseUI.AddStatic( UI_STATIC_BG_B, L"B", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_BG_A, L"A", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_BG_R, L"R", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_BG_G, L"G", 0, 0, 0, 0 );
	m_BaseUI.AddEditBox( UI_EDIT_BG_B, L"B", 0, 0, 0, 0 );

	m_vecDialog.push_back( &m_BaseUI );
}
#pragma endregion BaseUI 초기화

#pragma region InitSkinMeshUI
void CX2ViewerUI::InitSkinMeshUI()
{
	m_SkinMeshUI.Init( &g_DialogResourceManager );
	m_SkinMeshUI.SetCallback( OnGUISkinMeshEvent );
	m_SkinMeshUI.SetCaptionText( L"스킨메쉬 UI [F4]" );
	m_SkinMeshUI.EnableCaption( true );
	m_SkinMeshUI.SetSize( 150, 825 );
	m_SkinMeshUI.EnableKeyboardInput( true );
	m_SkinMeshUI.SetBackgroundColors( D3DCOLOR_ARGB( 55, 98, 138, 206 ), D3DCOLOR_ARGB( 55, 54, 105, 192 ), 
		D3DCOLOR_ARGB( 55, 54, 105, 192 ), D3DCOLOR_ARGB( 55, 10,  73, 179 ) );

	m_SkinMeshUI.AddStatic( UI_STATIC_SCALE, L"크기", 0, 0, 0, 0 );
	m_SkinMeshUI.AddStatic( UI_STATIC_SCALE_X, L"X", 0, 0, 0, 0 );
	m_SkinMeshUI.AddStatic( UI_STATIC_SCALE_Y, L"Y", 0, 0, 0, 0 );
	m_SkinMeshUI.AddStatic( UI_STATIC_SCALE_Z, L"Z", 0, 0, 0, 0 );
	m_SkinMeshUI.AddEditBox( UI_EDIT_SCALE_X, L"", 0, 0, 0, 0 );
	m_SkinMeshUI.AddEditBox( UI_EDIT_SCALE_Y, L"", 0, 0, 0, 0 );
	m_SkinMeshUI.AddEditBox( UI_EDIT_SCALE_Z, L"", 0, 0, 0, 0 );

	m_SkinMeshUI.AddStatic( UI_STATIC_POSITION, L"위치", 0, 0, 0, 0 );
	m_SkinMeshUI.AddStatic( UI_STATIC_POSITION_X, L"X", 0, 0, 0, 0 );
	m_SkinMeshUI.AddStatic( UI_STATIC_POSITION_Y, L"Y", 0, 0, 0, 0 );
	m_SkinMeshUI.AddStatic( UI_STATIC_POSITION_Z, L"Z", 0, 0, 0, 0 );
	m_SkinMeshUI.AddEditBox( UI_EDIT_POSITION_X, L"", 0, 0, 0, 0 );
	m_SkinMeshUI.AddEditBox( UI_EDIT_POSITION_Y, L"", 0, 0, 0, 0 );
	m_SkinMeshUI.AddEditBox( UI_EDIT_POSITION_Z, L"", 0, 0, 0, 0 );

	m_SkinMeshUI.AddStatic( UI_STATIC_BONE, L"뼈대 리스트", 0, 0, 0, 0 );
	m_SkinMeshUI.AddEditBox( UI_EDIT_BONE, L"", 0, 0, 0, 0 );
	m_SkinMeshUI.AddListBox( UI_LIST_BONE, 0, 0, 0, 0 );
	m_SkinMeshUI.AddCheckBox( UI_CHECK_BONE_MODIFY, L"뼈대 변환 적용", 0, 0, 0, 0, false );

	m_SkinMeshUI.AddStatic( UI_STATIC_MODIFY_BONE, L"변환될 뼈대 리스트", 0, 0, 0, 0 );
	m_SkinMeshUI.AddEditBox( UI_EDIT_MODIFY_BONE, L"", 0, 0, 0, 0 );
	m_SkinMeshUI.AddListBox( UI_LIST_MODIFY_BONE, 0, 0, 0, 0 );

	m_SkinMeshUI.AddStatic( UI_STATIC_DEFAULT_BONE, L"기본 설정된 변환 목록", 0, 0, 0, 0 );
	m_SkinMeshUI.AddComboBox( UI_COMBO_DEFAULT_BONE, 0, 0, 0, 0 );

	m_SkinMeshUI.AddButton( UI_BUT_OPEN_BONE, L"열기", 0, 0, 0, 0 );
	m_SkinMeshUI.AddButton( UI_BUT_SAVE_BONE, L"저장", 0, 0, 0, 0 );

	m_SkinMeshUI.AddCheckBox( UI_CHECK_MOTION_ONOFF, L"모션", 0, 0, 0, 0, true );
	m_SkinMeshUI.AddCheckBox( UI_CHECK_BOUNDING, L"바운딩 박스", 0, 0, 0, 0, false );
	m_SkinMeshUI.AddCheckBox( UI_CHECK_ATTACK_BOX, L"어택 박스", 0, 0, 0, 0, false );
	
	m_SkinMeshUI.AddButton( UI_BUT_RENDER_PARAM, L"렌더링 파라미터", 0, 0, 0, 0 );

	m_vecDialog.push_back( &m_SkinMeshUI );
}
#pragma endregion SkinMeshUI 초기화

#pragma region InitBoneUI
void CX2ViewerUI::InitBoneUI()
{
	m_BoneUI.Init( &g_DialogResourceManager );
	m_BoneUI.SetCallback( OnGUIBoneEvent );
	m_BoneUI.SetCaptionText( L"뼈대 UI" );
	m_BoneUI.EnableCaption( true );
	m_BoneUI.SetSize( 150, 140 );
	m_BoneUI.EnableKeyboardInput( true );
	m_BoneUI.SetBackgroundColors( D3DCOLOR_ARGB( 55, 98, 138, 206 ), D3DCOLOR_ARGB( 55, 54, 105, 192 ), 
		D3DCOLOR_ARGB( 55, 54, 105, 192 ), D3DCOLOR_ARGB( 55, 10,  73, 179 ) );

	m_BoneUI.AddStatic( UI_STATIC_BONE_SCALE, L"크기", 0, 0, 0, 0 );
	m_BoneUI.AddStatic( UI_STATIC_BONE_SCALE_X, L"X", 0, 0, 0, 0 );
	m_BoneUI.AddStatic( UI_STATIC_BONE_SCALE_Y, L"Y", 0, 0, 0, 0 );
	m_BoneUI.AddStatic( UI_STATIC_BONE_SCALE_Z, L"Z", 0, 0, 0, 0 );
	m_BoneUI.AddEditBox( UI_EDIT_BONE_SCALE_X, L"", 0, 0, 0, 0 );
	m_BoneUI.AddEditBox( UI_EDIT_BONE_SCALE_Y, L"", 0, 0, 0, 0 );
	m_BoneUI.AddEditBox( UI_EDIT_BONE_SCALE_Z, L"", 0, 0, 0, 0 );

	m_BoneUI.AddStatic( UI_STATIC_BONE_POS_Y, L"위치 Y", 0, 0, 0, 0 );
	m_BoneUI.AddEditBox( UI_EDIT_BONE_POS_Y, L"", 0, 0, 0, 0 );

	m_vecDialog.push_back( &m_BoneUI );
}
#pragma endregion BoneUI 초기화

#pragma region InitEquipUI
void CX2ViewerUI::InitEquipUI()
{
	m_EquipUI.Init( &g_DialogResourceManager );
	m_EquipUI.SetCallback( OnGUIEquipEvent );
	m_EquipUI.SetCaptionText( L"장비 UI [F5]" );
	m_EquipUI.EnableCaption( true );
	m_EquipUI.SetSize( 150, 640 );
	m_EquipUI.EnableKeyboardInput( true );
	m_EquipUI.SetBackgroundColors( D3DCOLOR_ARGB( 55, 98, 138, 206 ), D3DCOLOR_ARGB( 55, 54, 105, 192 ), 
		D3DCOLOR_ARGB( 55, 54, 105, 192 ), D3DCOLOR_ARGB( 55, 10,  73, 179 ) );

	m_EquipUI.AddStatic( UI_STATIC_EQUIP, L"장비 리스트", 0, 0, 0, 0 );
	m_EquipUI.AddListBox( UI_LIST_EQUIP, 0, 0, 0, 0 );
	m_EquipUI.AddButton( UI_BUT_CLEAR_EQUIP_LIST, L"초기화", 0, 0, 0, 0 );
	m_EquipUI.AddButton( UI_BUT_OPEN_EQUIP, L"열기", 0, 0, 0, 0 );

	m_EquipUI.AddStatic( UI_STATIC_WEAPON, L"무기 리스트", 0, 0, 0, 0 );
	m_EquipUI.AddListBox( UI_LIST_WEAPON, 0, 0, 0, 0 );
	m_EquipUI.AddButton( UI_BUT_CLEAR_WEAPON_LIST, L"초기화", 0, 0, 0, 0 );
	m_EquipUI.AddButton( UI_BUT_OPEN_WEAPON, L"열기", 0, 0, 0, 0 );

	m_EquipUI.AddStatic( UI_STATIC_ACCESSORY, L"악세서리 리스트", 0, 0, 0, 0 );
	m_EquipUI.AddListBox( UI_LIST_ACCESSORY, 0, 0, 0, 0 );
	m_EquipUI.AddButton( UI_BUT_CLEAR_ACCESSORY_LIST, L"초기화", 0, 0, 0, 0 );
	m_EquipUI.AddButton( UI_BUT_OPEN_ACCESSORY, L"열기", 0, 0, 0, 0 );

	m_EquipUI.AddListBox( UI_LIST_ATTACH_BONES, 0, 0, 0, 0 );

	m_EquipUI.AddStatic( UI_STATIC_SCENE, L"장면", 0, 0, 0, 0 );
	m_EquipUI.AddButton( UI_BUT_SCENE_OPEN, L"열기", 0, 0, 0, 0 );
	m_EquipUI.AddButton( UI_BUT_SCENE_SAVE, L"저장", 0, 0, 0, 0 );

	m_vecDialog.push_back( &m_EquipUI );
}
#pragma endregion EquipUI 초기화

#pragma region InitWeaponUI
void CX2ViewerUI::InitWeaponUI()
{
	m_WeaponUI.Init( &g_DialogResourceManager );
	m_WeaponUI.SetCallback( OnGUIWeaponEvent );
	m_WeaponUI.SetCaptionText( L"무기 UI" );
	m_WeaponUI.EnableCaption( true );
	m_WeaponUI.SetSize( 150, 135 );
	m_WeaponUI.EnableKeyboardInput( true );
	m_WeaponUI.SetBackgroundColors( D3DCOLOR_ARGB( 55, 98, 138, 206 ), D3DCOLOR_ARGB( 55, 54, 105, 192 ), 
		D3DCOLOR_ARGB( 55, 54, 105, 192 ), D3DCOLOR_ARGB( 55, 10,  73, 179 ) );

	m_WeaponUI.AddStatic( UI_STATIC_WEAPON_BONE_NAME, L"뼈대 위치", 0, 0, 0, 0 );
	m_WeaponUI.AddEditBox( UI_EDIT_WEAPON_BONE_NAME, L"", 0, 0, 0, 0 );

	m_WeaponUI.AddStatic( UI_STATIC_WEAPON_BONE_ROT_X, L"회전 X", 0, 0, 0, 0 );
	m_WeaponUI.AddStatic( UI_STATIC_WEAPON_BONE_ROT_Y, L"회전 Y", 0, 0, 0, 0 );
	m_WeaponUI.AddStatic( UI_STATIC_WEAPON_BONE_ROT_Z, L"회전 Z", 0, 0, 0, 0 );
	m_WeaponUI.AddEditBox( UI_EDIT_WEAPON_BONE_ROT_X, L"", 0, 0, 0, 0 );
	m_WeaponUI.AddEditBox( UI_EDIT_WEAPON_BONE_ROT_Y, L"", 0, 0, 0, 0 );
	m_WeaponUI.AddEditBox( UI_EDIT_WEAPON_BONE_ROT_Z, L"", 0, 0, 0, 0 );

	m_vecDialog.push_back( &m_WeaponUI );
}
#pragma endregion WeaponUI 초기화

#pragma region InitAccessoryUI
void CX2ViewerUI::InitAccessoryUI()
{
	m_AccessoryUI.Init( &g_DialogResourceManager );
	m_AccessoryUI.SetCallback( OnGUIAccessoryEvent );
	m_AccessoryUI.SetCaptionText( L"악세서리 UI" );
	m_AccessoryUI.EnableCaption( true );
	m_AccessoryUI.SetSize( 200, 195 );
	m_AccessoryUI.EnableKeyboardInput( true );
	m_AccessoryUI.SetBackgroundColors( D3DCOLOR_ARGB( 55, 98, 138, 206 ), D3DCOLOR_ARGB( 55, 54, 105, 192 ), 
		D3DCOLOR_ARGB( 55, 54, 105, 192 ), D3DCOLOR_ARGB( 55, 10,  73, 179 ) );

	m_AccessoryUI.AddStatic( UI_STATIC_ACCESSORY_BONE_NAME, L"뼈대 위치", 0, 0, 0, 0 );
	m_AccessoryUI.AddEditBox( UI_EDIT_ACCESSORY_BONE_NAME, L"", 0, 0, 0, 0 );

	m_AccessoryUI.AddStatic( UI_STATIC_ACCESSORY_X, L"X", 0, 0, 0, 0 );
	m_AccessoryUI.AddStatic( UI_STATIC_ACCESSORY_Y, L"Y", 0, 0, 0, 0 );
	m_AccessoryUI.AddStatic( UI_STATIC_ACCESSORY_Z, L"Z", 0, 0, 0, 0 );

	m_AccessoryUI.AddStatic( UI_STATIC_ACCESSORY_MOVE, L"이동", 0, 0, 0, 0 );
	m_AccessoryUI.AddStatic( UI_STATIC_ACCESSORY_ROT, L"회전", 0, 0, 0, 0 );
	m_AccessoryUI.AddStatic( UI_STATIC_ACCESSORY_SCALE, L"크기", 0, 0, 0, 0 );

	m_AccessoryUI.AddEditBox( UI_EDIT_ACCESSORY_MOVE_X, L"", 0, 0, 0, 0 );
	m_AccessoryUI.AddEditBox( UI_EDIT_ACCESSORY_MOVE_Y, L"", 0, 0, 0, 0 );
	m_AccessoryUI.AddEditBox( UI_EDIT_ACCESSORY_MOVE_Z, L"", 0, 0, 0, 0 );

	m_AccessoryUI.AddEditBox( UI_EDIT_ACCESSORY_ROT_X, L"", 0, 0, 0, 0 );
	m_AccessoryUI.AddEditBox( UI_EDIT_ACCESSORY_ROT_Y, L"", 0, 0, 0, 0 );
	m_AccessoryUI.AddEditBox( UI_EDIT_ACCESSORY_ROT_Z, L"", 0, 0, 0, 0 );

	m_AccessoryUI.AddEditBox( UI_EDIT_ACCESSORY_SCALE_X, L"", 0, 0, 0, 0 );
	m_AccessoryUI.AddEditBox( UI_EDIT_ACCESSORY_SCALE_Y, L"", 0, 0, 0, 0 );
	m_AccessoryUI.AddEditBox( UI_EDIT_ACCESSORY_SCALE_Z, L"", 0, 0, 0, 0 );

	m_vecDialog.push_back( &m_AccessoryUI );
}
#pragma endregion 악세서리 UI 초기화

#pragma region InitAnimationUI
void CX2ViewerUI::InitAnimationUI()
{
	m_AnimationUI.Init( &g_DialogResourceManager );
	m_AnimationUI.SetCallback( OnGUIAnimationEvent );
	m_AnimationUI.SetCaptionText( L"애니메이션 UI [F6]" );
	m_AnimationUI.EnableCaption( true );
	m_AnimationUI.SetSize( 370, 200 );
	m_AnimationUI.EnableKeyboardInput( true );
	m_AnimationUI.SetBackgroundColors( D3DCOLOR_ARGB( 55, 98, 138, 206 ), D3DCOLOR_ARGB( 55, 54, 105, 192 ), 
		D3DCOLOR_ARGB( 55, 54, 105, 192 ), D3DCOLOR_ARGB( 55, 10,  73, 179 ) );

	m_AnimationUI.AddSlider( UI_SLIDE_ANIM_FRAME, 0, 0, 0, 0, 1, 100, 100 );
	m_AnimationUI.AddButton( UI_BUT_ANIM_PLAY_PAUSE, L"||", 0, 0, 0, 0 );
	m_AnimationUI.AddButton( UI_BUT_ANIM_STOP, L"■", 0, 0, 0, 0 );
	m_AnimationUI.AddButton( UI_BUT_ANIM_PREV_FRAME, L"《", 0, 0, 0, 0 );
	m_AnimationUI.AddButton( UI_BUT_ANIM_NEXT_FRAME, L"》", 0, 0, 0, 0 );
	m_AnimationUI.AddStatic( UI_STATIC_ANIM_FRAME, L"Frame : 1/31", 0, 0, 0, 0 );
	m_AnimationUI.AddEditBox( UI_EDIT_ANIM_FRAME_TIME_INC, L"0.033", 0, 0, 0, 0, false );
	m_AnimationUI.AddStatic( UI_STATIC_ANIM_SPEED, L"speed : 100", 0, 0, 0, 0 );
	m_AnimationUI.AddSlider( UI_SLIDE_ANIM_SPEED, 0, 0, 0, 0, 1, 100, 100 );
	m_AnimationUI.AddComboBox( UI_COMBO_PLAY_TYPE, 0, 0, 0, 0 );

	m_AnimationUI.AddStatic( UI_STATIC_ANIMATION, L"Animation List", 0, 0, 0, 0 );
	m_AnimationUI.AddListBox( UI_LIST_ANIMATION, 0, 0, 0, 0 );
	m_AnimationUI.AddStatic( UI_STATIC_ANIM_NUM, L"( 0 )", 0, 0, 0, 0 );
	m_AnimationUI.AddEditBox( UI_EDIT_ANIM_NAME, L"Not Selected", 0, 0, 0, 0 );
	m_AnimationUI.AddButton( UI_BUT_ANIM_NAME_CHANGE, L"AnimName Change", 0, 0, 0, 0 );

	m_vecDialog.push_back( &m_AnimationUI );
}
#pragma endregion 애니메이션 UI 초기화

#pragma region InitRenderParamUI
void CX2ViewerUI::InitRenderParamUI()
{
	m_RenderParamUI.Init( &g_DialogResourceManager );
	m_RenderParamUI.SetCallback( OnGUIRPEvent );
	m_RenderParamUI.SetCaptionText( L"렌더링 파라미터 UI [F7]" );
	m_RenderParamUI.SetSize( 230, 800 );
	m_RenderParamUI.EnableCaption( true );
	m_RenderParamUI.EnableKeyboardInput( true );
	m_RenderParamUI.SetBackgroundColors( D3DCOLOR_ARGB( 55, 98, 138, 206 ), D3DCOLOR_ARGB( 55, 54, 105, 192 ), 
		D3DCOLOR_ARGB( 55, 54, 105, 192 ), D3DCOLOR_ARGB( 55, 10,  73, 179 ) );

	m_RenderParamUI.AddStatic( UI_STATIC_RENDERTYPE, L"TYPE", 0, 0, 0, 0 );
	m_RenderParamUI.AddComboBox( UI_COMBO_RENDERTYPE, 0, 0, 0, 0 );

	m_RenderParamUI.AddStatic( UI_STATIC_CULLMODE, L"CULL MODE", 0, 0, 0, 0 );
	m_RenderParamUI.AddComboBox( UI_COMBO_CULLMODE, 0, 0, 0, 0 );

	m_RenderParamUI.AddStatic( UI_STATIC_CARTOON_TEX_TYPE, L"CARTOON TYPE", 0, 0, 0, 0 );
	m_RenderParamUI.AddComboBox( UI_COMBO_CARTOON_TEX_TYPE, 0, 0, 0, 0 );

	m_RenderParamUI.AddStatic( UI_STATIC_SRCBLEND, L"SRC BLEND", 0, 0, 0, 0 );
	m_RenderParamUI.AddComboBox( UI_COMBO_SRCBLEND, 0, 0, 0, 0 );

	m_RenderParamUI.AddStatic( UI_STATIC_DESTBLEND, L"DEST BLEND", 0, 0, 0, 0 );
	m_RenderParamUI.AddComboBox( UI_COMBO_DESTBLEND, 0, 0, 0, 0 );

	m_RenderParamUI.AddCheckBox( UI_CHECK_ALPHA_BLEND, L"ALPHA BLEND", 0, 0, 0, 0, false );
	m_RenderParamUI.AddCheckBox( UI_CHECK_ZENABLE, L"ZENABLE", 0, 0, 0, 0, false );
	m_RenderParamUI.AddCheckBox( UI_CHECK_ZWRITABLE, L"ZWRITABLE", 0, 0, 0, 0, false );	

	m_RenderParamUI.AddStatic( UI_STATIC_COLOR, L"CARTOON COLOR", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_COLOR_A, L"A", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_COLOR_R, L"R", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_COLOR_G, L"G", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_COLOR_B, L"B", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_COLOR_A, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_COLOR_R, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_COLOR_G, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_COLOR_B, L"", 0, 0, 0, 0 );
	
	m_RenderParamUI.AddStatic( UI_STATIC_OUTLINE_COLOR, L"OUTLINE COLOR", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_OUTLINE_COLOR_A, L"A", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_OUTLINE_COLOR_R, L"R", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_OUTLINE_COLOR_G, L"G", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_OUTLINE_COLOR_B, L"B", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_OUTLINE_COLOR_A, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_OUTLINE_COLOR_R, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_OUTLINE_COLOR_G, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_OUTLINE_COLOR_B, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_OUTLINE_WIDE, L"", 0, 0, 0, 0 );

	m_RenderParamUI.AddStatic( UI_STATIC_LIGHTFLOW_WIDE, L"LIGHT FLOW WIDE(폭)", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_LIGHTFLOW_WIDE, L"", 0, 0, 0, 0 );

	m_RenderParamUI.AddStatic( UI_STATIC_LIGHTFLOW_IMPACT, L"LIGHT FLOW IMPACT", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_LIGHTFLOW_IMPACT_MIN, L"Min", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_LIGHTFLOW_IMPACT_MAX, L"Max", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_LIGHTFLOW_IMPACT_ANIMTIME, L"AnimTime", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_LIGHTFLOW_IMPACT_MIN, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_LIGHTFLOW_IMPACT_MAX, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_LIGHTFLOW_IMPACT_ANIMTIME, L"", 0, 0, 0, 0 );

	m_RenderParamUI.AddStatic( UI_STATIC_LIGHTFLOW_POINT,   L"LIGHT FLOW POINT", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_LIGHTFLOW_POINT_X, L"X", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_LIGHTFLOW_POINT_Y, L"Y", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_LIGHTFLOW_POINT_Z, L"Z", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_LIGHTFLOW_POINT_X, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_LIGHTFLOW_POINT_Y, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_LIGHTFLOW_POINT_Z, L"", 0, 0, 0, 0 );

	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE0,   L"TEXTURE OFFSET STAGE 0", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE0_X, L"X", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE0_Y, L"Y", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE0_MIN, L"Min", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE0_MAX, L"Max", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE0_ANIMTIME, L"Anim Time", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE0_MIN_X, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE0_MIN_Y, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE0_MAX_X, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE0_MAX_Y, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE0_ANIMTIME, L"", 0, 0, 0, 0 );

	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE1,   L"TEXTURE OFFSET STAGE 1", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE1_X, L"X", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE1_Y, L"Y", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE1_MIN, L"Min", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE1_MAX, L"Max", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE1_ANIMTIME, L"Anim Time", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE1_MIN_X, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE1_MIN_Y, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE1_MAX_X, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE1_MAX_Y, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE1_ANIMTIME, L"", 0, 0, 0, 0 );

	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE2,   L"TEXTURE OFFSET STAGE 2", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE2_X, L"X", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE2_Y, L"Y", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE2_MIN, L"Min", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE2_MAX, L"Max", 0, 0, 0, 0 );
	m_RenderParamUI.AddStatic( UI_STATIC_TEXOFFSET_STAGE2_ANIMTIME, L"Anim Time", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE2_MIN_X, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE2_MIN_Y, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE2_MAX_X, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE2_MAX_Y, L"", 0, 0, 0, 0 );
	m_RenderParamUI.AddEditBox( UI_EDIT_TEXOFFSET_STAGE2_ANIMTIME, L"", 0, 0, 0, 0 );

	m_RenderParamUI.AddButton( UI_BUT_PARAM_OK, L"적 용", 0, 0, 0, 0 );
	m_RenderParamUI.AddButton( UI_BUT_PARAM_CANCEL, L"창닫기", 0, 0, 0, 0 );

	m_vecDialog.push_back( &m_RenderParamUI );
}
#pragma endregion 렌더링 

#pragma region ResetBaseUI
void CX2ViewerUI::ResetBaseUI( KLuaManager* luaManager )
{
	CDXUTControl* pControl;
	D3DXVECTOR3 vTemp;
	WCHAR strNum[128] = L"";
	int nX, nY, nW, nH;
	nX = nY = nW = nH = 0;

	// STATIC_FILE
	GET_PLUA_POS( STATIC_FILE, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_FILE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_OPEN
	GET_PLUA_POS( BUT_OPEN, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_BUT_OPEN );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_RESET
	GET_PLUA_POS( BUT_RESET, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_BUT_RESET );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_UI
	GET_PLUA_POS( STATIC_UI, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_UI );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_UI_INIT
	GET_PLUA_POS( BUT_UI_INIT, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_BUT_UI_INIT );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_UI_HIDE
	GET_PLUA_POS( BUT_UI_HIDE, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_BUT_UI_HIDE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_VIEW
	GET_PLUA_POS( STATIC_VIEW, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_VIEW );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// CHECK_GRID
	GET_PLUA_POS( CHECK_GRID, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_CHECK_GRID );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// CHECK_WIREFRAME
	GET_PLUA_POS( CHECK_WIREFRAME, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_CHECK_WIREFRAME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_LIGHT_POS
	GET_PLUA_POS( STATIC_LIGHT_POS, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_LIGHT_POS );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_LIGHT_POS_X
	GET_PLUA_POS( STATIC_LIGHT_POS_X, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_LIGHT_POS_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_LIGHT_POS_X
	GET_PLUA_POS( EDIT_LIGHT_POS_X, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_LIGHT_POS_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_LIGHT_POS_Y
	GET_PLUA_POS( STATIC_LIGHT_POS_Y, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_LIGHT_POS_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_LIGHT_POS_Y
	GET_PLUA_POS( EDIT_LIGHT_POS_Y, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_LIGHT_POS_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_LIGHT_POS_Z
	GET_PLUA_POS( STATIC_LIGHT_POS_Z, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_LIGHT_POS_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_LIGHT_POS_Z
	GET_PLUA_POS( EDIT_LIGHT_POS_Z, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_LIGHT_POS_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// CHECK_LIGHT_ONOFF
	GET_PLUA_POS( CHECK_LIGHT_ONOFF, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_CHECK_LIGHT_ONOFF );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	vTemp = ((CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH ))->GetLightPos();
	CDXUTEditBox* pEditBox = NULL;
	swprintf( strNum, L"%.2f", vTemp.x );
	pEditBox = m_BaseUI.GetEditBox( UI_EDIT_LIGHT_POS_X );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.y );
	pEditBox = m_BaseUI.GetEditBox( UI_EDIT_LIGHT_POS_Y );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.z );
	pEditBox = m_BaseUI.GetEditBox( UI_EDIT_LIGHT_POS_Z );
	pEditBox->SetText( strNum );

	// STATIC_CAMERAMODE
	GET_PLUA_POS( STATIC_CAMERA_MODE, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_CAMERAMODE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// RADIO_CAMERA_NORMAL
	GET_PLUA_POS( RADIO_CAMERA_NORMAL, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_RADIO_CAMERA_NORMAL );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// RADIO_CAMERA_NAVIGATION
	GET_PLUA_POS( RADIO_CAMERA_NAVIGATION, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_RADIO_CAMERA_NAVIGATION );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_CAMERA_RESET
	GET_PLUA_POS( BUT_CAMERA_RESET, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_BUT_CAMERA_RESET );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_CAMERA_EYE_POS_X
	GET_PLUA_POS( STATIC_CAMERA_EYE_POS_X, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_CAMERA_EYE_POS_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_CAMERA_EYE_POS_X
	GET_PLUA_POS( EDIT_CAMERA_EYE_POS_X, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_CAMERA_EYE_POS_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_CAMERA_EYE_POS_Y
	GET_PLUA_POS( STATIC_CAMERA_EYE_POS_Y, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_CAMERA_EYE_POS_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_CAMERA_EYE_POS_Y
	GET_PLUA_POS( EDIT_CAMERA_EYE_POS_Y, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_CAMERA_EYE_POS_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_CAMERA_EYE_POS_Z
	GET_PLUA_POS( STATIC_CAMERA_EYE_POS_Z, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_CAMERA_EYE_POS_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_CAMERA_EYE_POS_Z
	GET_PLUA_POS( EDIT_CAMERA_EYE_POS_Z, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_CAMERA_EYE_POS_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_CAMERA_LOOK_POS_X
	GET_PLUA_POS( STATIC_CAMERA_LOOK_POS_X, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_CAMERA_LOOK_POS_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_CAMERA_LOOK_POS_X
	GET_PLUA_POS( EDIT_CAMERA_LOOK_POS_X, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_CAMERA_LOOK_POS_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_CAMERA_LOOK_POS_Y
	GET_PLUA_POS( STATIC_CAMERA_LOOK_POS_Y, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_CAMERA_LOOK_POS_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_CAMERA_LOOK_POS_Y
	GET_PLUA_POS( EDIT_CAMERA_LOOK_POS_Y, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_CAMERA_LOOK_POS_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_CAMERA_LOOK_POS_Z
	GET_PLUA_POS( STATIC_CAMERA_LOOK_POS_Z, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_CAMERA_LOOK_POS_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_CAMERA_LOOK_POS_Z
	GET_PLUA_POS( EDIT_CAMERA_LOOK_POS_Z, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_CAMERA_LOOK_POS_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	vTemp = reinterpret_cast<CX2ViewerCamera*>( SiGetObject( OS_CAMERA ) )->GetEye();
	pEditBox = NULL;
	swprintf( strNum, L"%.2f", vTemp.x );
	pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_EYE_POS_X );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.y );
	pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_EYE_POS_Y );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.z );
	pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_EYE_POS_Z );
	pEditBox->SetText( strNum );

	vTemp = reinterpret_cast<CX2ViewerCamera*>( SiGetObject( OS_CAMERA ) )->GetLookVec();
	pEditBox = NULL;
	swprintf( strNum, L"%.2f", vTemp.x );
	pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_LOOK_POS_X );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.y );
	pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_LOOK_POS_Y );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.z );
	pEditBox = m_BaseUI.GetEditBox( UI_EDIT_CAMERA_LOOK_POS_Z );
	pEditBox->SetText( strNum );

	// STATIC_MODEL
	GET_PLUA_POS( STATIC_MODEL, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_MODEL );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_MODEL
	GET_PLUA_POS( EDIT_MODEL, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_MODEL );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// LIST_MODEL
	GET_PLUA_POS( LIST_MODEL, nX, nY, nW, nH );
	CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( m_BaseUI.GetControl( UI_LIST_MODEL ) );
	pListBox->SetLocation( nX, nY );
	pListBox->SetSize( nW, nH );

	pListBox->RemoveAllItems();

	BOOST_FOREACH( wstring& pStr, m_strMotionFileList )
	{
		pListBox->AddItem( pStr.c_str(), (LPVOID)(size_t)0 );
	}

	//STATIC_BGSET
	GET_PLUA_POS( STATIC_BG_COLOR, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_BGSET );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//EDIT_BG_A
	GET_PLUA_POS( EDIT_BG_A, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_BG_A );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_BG_A
	GET_PLUA_POS( STATIC_BG_A, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_BG_A );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//EDIT_BG_R
	GET_PLUA_POS( EDIT_BG_R, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_BG_R );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_BG_R
	GET_PLUA_POS( STATIC_BG_R, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_BG_R );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//EDIT_BG_G
	GET_PLUA_POS( EDIT_BG_G, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_BG_G );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_BG_G
	GET_PLUA_POS( STATIC_BG_G, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_BG_G );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//EDIT_BG_B
	GET_PLUA_POS( EDIT_BG_B, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_EDIT_BG_B );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_BG_B
	GET_PLUA_POS( STATIC_BG_B, nX, nY, nW, nH );
	pControl = m_BaseUI.GetControl( UI_STATIC_BG_B );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//초기 텍스트 셋팅
	m_BaseUI.GetEditBox( UI_EDIT_BG_A )->SetText( L"50" );
	m_BaseUI.GetEditBox( UI_EDIT_BG_R )->SetText( L"50" );
	m_BaseUI.GetEditBox( UI_EDIT_BG_G )->SetText( L"50" );
	m_BaseUI.GetEditBox( UI_EDIT_BG_B )->SetText( L"50" );
	g_pKTDXApp->GetDGManager()->SetClearColorA( _wtoi( m_BaseUI.GetEditBox( UI_EDIT_BG_A )->GetText() ) );
	g_pKTDXApp->GetDGManager()->SetClearColorR( _wtoi( m_BaseUI.GetEditBox( UI_EDIT_BG_R )->GetText() ) );
	g_pKTDXApp->GetDGManager()->SetClearColorG( _wtoi( m_BaseUI.GetEditBox( UI_EDIT_BG_G )->GetText() ) );
	g_pKTDXApp->GetDGManager()->SetClearColorB( _wtoi( m_BaseUI.GetEditBox( UI_EDIT_BG_B )->GetText() ) );
}
#pragma endregion BaseUI 재설정

#pragma region ResetSkinMeshUI
void CX2ViewerUI::ResetSkinMeshUI( KLuaManager* luaManager )
{
	CDXUTControl* pControl;
	D3DXVECTOR3 vTemp;
	WCHAR strNum[128] = L"";
	int nX, nY, nW, nH;
	nX = nY = nW = nH = 0;

	// STATIC_SCALE
	GET_PLUA_POS( STATIC_SCALE, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_STATIC_SCALE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_SCALE_X
	GET_PLUA_POS( STATIC_SCALE_X, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_STATIC_SCALE_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_SCALE_Y
	GET_PLUA_POS( STATIC_SCALE_Y, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_STATIC_SCALE_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_SCALE_Z
	GET_PLUA_POS( STATIC_SCALE_Z, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_STATIC_SCALE_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_SCALE_X
	GET_PLUA_POS( EDIT_SCALE_X, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_EDIT_SCALE_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_SCALE_Y
	GET_PLUA_POS( EDIT_SCALE_Y, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_EDIT_SCALE_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_SCALE_Z
	GET_PLUA_POS( EDIT_SCALE_Z, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_EDIT_SCALE_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	vTemp = ((CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH ))->GetScale();
	swprintf( strNum, L"%.2f", vTemp.x );
	CDXUTEditBox* pEditBox = NULL;
	pEditBox = m_SkinMeshUI.GetEditBox( UI_EDIT_SCALE_X );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.y );
	pEditBox = m_SkinMeshUI.GetEditBox( UI_EDIT_SCALE_Y );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.z );
	pEditBox = m_SkinMeshUI.GetEditBox( UI_EDIT_SCALE_Z );
	pEditBox->SetText( strNum );

	// STATIC_POSITION
	GET_PLUA_POS( STATIC_POSITION, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_STATIC_POSITION );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_POSITION_X
	GET_PLUA_POS( STATIC_POSITION_X, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_STATIC_POSITION_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_POSITION_Y
	GET_PLUA_POS( STATIC_POSITION_Y, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_STATIC_POSITION_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_POSITION_Z
	GET_PLUA_POS( STATIC_POSITION_Z, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_STATIC_POSITION_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_POSITION_X
	GET_PLUA_POS( EDIT_POSITION_X, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_EDIT_POSITION_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_POSITION_Y
	GET_PLUA_POS( EDIT_POSITION_Y, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_EDIT_POSITION_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_POSITION_Z
	GET_PLUA_POS( EDIT_POSITION_Z, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_EDIT_POSITION_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	vTemp = ((CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH ))->GetPosition();    
	swprintf( strNum, L"%.2f", vTemp.x );
	pEditBox = NULL;
	pEditBox = m_SkinMeshUI.GetEditBox( UI_EDIT_POSITION_X );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.y );
	pEditBox = m_SkinMeshUI.GetEditBox( UI_EDIT_POSITION_Y );
	pEditBox->SetText( strNum );
	swprintf( strNum, L"%.2f", vTemp.z );
	pEditBox = m_SkinMeshUI.GetEditBox( UI_EDIT_POSITION_Z );
	pEditBox->SetText( strNum );

	// STATIC_BONE
	GET_PLUA_POS( STATIC_BONE, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_STATIC_BONE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// EDIT_BONE
	GET_PLUA_POS( EDIT_BONE, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_EDIT_BONE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	pEditBox = m_SkinMeshUI.GetEditBox( UI_EDIT_BONE );
	pEditBox->SetText( L"뼈대 표시" );

	// LIST_BONE
	GET_PLUA_POS( LIST_BONE, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_LIST_BONE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// CHECK_BONE_MODIFY
	GET_PLUA_POS( CHECK_BONE_MODIFY, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_CHECK_BONE_MODIFY );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	reinterpret_cast<CDXUTCheckBox*>( pControl )->SetChecked( false );

	// STATIC_MODIFY_BONE
	GET_PLUA_POS( STATIC_MODIFY_BONE, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_STATIC_MODIFY_BONE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// EDITMODIFY__BONE
	GET_PLUA_POS( EDIT_MODIFY_BONE, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_EDIT_MODIFY_BONE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	pEditBox = m_SkinMeshUI.GetEditBox( UI_EDIT_MODIFY_BONE );
	pEditBox->SetText( L"변환된 뼈대 표시" );

	// LISTMODIFY__BONE
	GET_PLUA_POS( LIST_MODIFY_BONE, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_LIST_MODIFY_BONE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_DEFAULT_BONE
	GET_PLUA_POS( STATIC_DEFAULT_BONE, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_STATIC_DEFAULT_BONE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// COMBO_DEFAULT_BONE
	GET_PLUA_POS( COMBO_DEFAULT_BONE, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_COMBO_DEFAULT_BONE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	CDXUTComboBox* pComboBox = reinterpret_cast<CDXUTComboBox*>( pControl );
	pComboBox->RemoveAllItems();
	#pragma region 아이템 추가
	pComboBox->AddItem( L"엘소드", NULL );
	pComboBox->AddItem( L"소드 나이트", NULL );
	pComboBox->AddItem( L"매직 나이트", NULL );
	pComboBox->AddItem( L"시스 나이트", NULL );
	pComboBox->AddItem( L"로드 나이트", NULL );
	pComboBox->AddItem( L"룬 슬레이어", NULL );
	pComboBox->AddItem( L"인피니티 소드", NULL );

	pComboBox->AddItem( L"아이샤", NULL );
	pComboBox->AddItem( L"하이 매지션", NULL );
	pComboBox->AddItem( L"다크 매지션", NULL );
	pComboBox->AddItem( L"배틀 매지션", NULL );
	pComboBox->AddItem( L"엘리멘탈 마스터", NULL );
	pComboBox->AddItem( L"보이드 프린세스", NULL );
	pComboBox->AddItem( L"디멘션 위치", NULL );

	pComboBox->AddItem( L"레나", NULL );
	pComboBox->AddItem( L"컴뱃 레인저", NULL );
	pComboBox->AddItem( L"스나이핑 레인저", NULL );
	pComboBox->AddItem( L"트래핑 레인저", NULL );
	pComboBox->AddItem( L"윈드 스니커", NULL );
	pComboBox->AddItem( L"그랜드 아처", NULL );
	pComboBox->AddItem( L"나이트 와처", NULL );

	pComboBox->AddItem( L"레이븐", NULL );
	pComboBox->AddItem( L"소드 테이커", NULL );
	pComboBox->AddItem( L"오버 테이커", NULL );
	pComboBox->AddItem( L"웨폰 테이커", NULL );
	pComboBox->AddItem( L"블레이드 마스터", NULL );
	pComboBox->AddItem( L"레크리스 피스트", NULL );
	pComboBox->AddItem( L"베터랑 커맨더", NULL );

	pComboBox->AddItem( L"이브", NULL );
	pComboBox->AddItem( L"엑조틱 기어", NULL );
	pComboBox->AddItem( L"아키텍처", NULL );
	pComboBox->AddItem( L"일렉트라", NULL );
	pComboBox->AddItem( L"코드 네메시스", NULL );
	pComboBox->AddItem( L"코드 엠프레스", NULL );
	pComboBox->AddItem( L"배틀 세라프", NULL );

	pComboBox->AddItem( L"청", NULL );
	pComboBox->AddItem( L"퓨리 가디언", NULL );
	pComboBox->AddItem( L"슈팅 가디언", NULL );
	pComboBox->AddItem( L"쉘링 가디언", NULL );
	pComboBox->AddItem( L"아이언 팔라딘", NULL );
	pComboBox->AddItem( L"데들리 체이서", NULL );
	pComboBox->AddItem( L"택티컬 트루퍼", NULL );

	pComboBox->AddItem( L"아라", NULL );
	pComboBox->AddItem( L"소선", NULL );
	pComboBox->AddItem( L"소마", NULL );
	pComboBox->AddItem( L"제천", NULL );
	pComboBox->AddItem( L"명왕", NULL );

	pComboBox->AddItem( L"엘리시스", NULL );
	pComboBox->AddItem( L"세이버 나이트", NULL );
	pComboBox->AddItem( L"파이로 나이트", NULL );
	pComboBox->SetDropHeight( 50 );
	#pragma endregion 캐릭터 전직명

	// BUT_BONE_OPEN
	GET_PLUA_POS( BUT_BONE_OPEN, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_BUT_OPEN_BONE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_BONE_SAVE
	GET_PLUA_POS( BUT_BONE_SAVE, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_BUT_SAVE_BONE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
		
	// CHECK_MOTION_ONOFF
	GET_PLUA_POS( CHECK_MOTION_ONOFF, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_CHECK_MOTION_ONOFF );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	reinterpret_cast<CDXUTCheckBox*>( pControl )->SetChecked( true );
	
	//CHECK_BOUNDING
	GET_PLUA_POS( CHECK_BOUNDING, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_CHECK_BOUNDING );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	reinterpret_cast<CDXUTCheckBox*>( pControl )->SetChecked( ((CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH ))->GetBounding() );

	//CHECK_ATTACK_BOX
	GET_PLUA_POS( CHECK_ATTACK_BOX, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_CHECK_ATTACK_BOX );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	reinterpret_cast<CDXUTCheckBox*>( pControl )->SetChecked( ((CX2ViewerSkinMesh*)SiGetObject( OS_SKIN_MESH ))->GetShowAttackBox() );

	// BUTTON : CLEAR LIST
	GET_PLUA_POS( BUT_RENDER_PARAM, nX, nY, nW, nH );
	pControl = m_SkinMeshUI.GetControl( UI_BUT_RENDER_PARAM );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
}
#pragma endregion SkinMeshUI 재설정

#pragma region ResetBoneUI
void CX2ViewerUI::ResetBoneUI( KLuaManager* luaManager )
{
	CDXUTControl* pControl;
	D3DXVECTOR3 vTemp;
	WCHAR strNum[128] = L"";
	int nX, nY, nW, nH;
	nX = nY = nW = nH = 0;

	// STATIC_BONE_SCALE
	GET_PLUA_POS( STATIC_BONE_SCALE, nX, nY, nW, nH );
	pControl = m_BoneUI.GetControl( UI_STATIC_BONE_SCALE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_SCALE_X
	GET_PLUA_POS( STATIC_BONE_SCALE_X, nX, nY, nW, nH );
	pControl = m_BoneUI.GetControl( UI_STATIC_BONE_SCALE_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_SCALE_Y
	GET_PLUA_POS( STATIC_BONE_SCALE_Y, nX, nY, nW, nH );
	pControl = m_BoneUI.GetControl( UI_STATIC_BONE_SCALE_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_SCALE_Z
	GET_PLUA_POS( STATIC_BONE_SCALE_Z, nX, nY, nW, nH );
	pControl = m_BoneUI.GetControl( UI_STATIC_BONE_SCALE_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_SCALE_X
	GET_PLUA_POS( EDIT_BONE_SCALE_X, nX, nY, nW, nH );
	pControl = m_BoneUI.GetControl( UI_EDIT_BONE_SCALE_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_SCALE_Y
	GET_PLUA_POS( EDIT_BONE_SCALE_Y, nX, nY, nW, nH );
	pControl = m_BoneUI.GetControl( UI_EDIT_BONE_SCALE_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_SCALE_Z
	GET_PLUA_POS( EDIT_BONE_SCALE_Z, nX, nY, nW, nH );
	pControl = m_BoneUI.GetControl( UI_EDIT_BONE_SCALE_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_BONE_POS_Y
	GET_PLUA_POS( STATIC_BONE_POSITION, nX, nY, nW, nH );
	pControl = m_BoneUI.GetControl( UI_STATIC_BONE_POS_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_BONE_POS_Y
	GET_PLUA_POS( EDIT_BONE_POSITION_Y, nX, nY, nW, nH );
	pControl = m_BoneUI.GetControl( UI_EDIT_BONE_POS_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
}
#pragma endregion BoneUI 재설정

#pragma region ResetEquipUI
void CX2ViewerUI::ResetEquipUI( KLuaManager* luaManager )
{
	CDXUTControl* pControl;
	D3DXVECTOR3 vTemp;
	WCHAR strNum[128] = L"";
	int nX, nY, nW, nH;
	nX = nY = nW = nH = 0;

	// STATIC : EQUIP
	GET_PLUA_POS( STATIC_EQUIP, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_STATIC_EQUIP );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// LIST : EQUIP
	GET_PLUA_POS( LIST_EQUIP, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_LIST_EQUIP );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUTTON : CLEAR LIST
	GET_PLUA_POS( BUT_CLEAR_EQUIP_LIST, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_BUT_CLEAR_EQUIP_LIST );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUTTON : OPEN
	GET_PLUA_POS( BUT_OPEN_EQUIP, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_BUT_OPEN_EQUIP );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC : WEAPON
	GET_PLUA_POS( STATIC_WEAPON, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_STATIC_WEAPON );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// LIST : WEAPON
	GET_PLUA_POS( LIST_WEAPON, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_LIST_WEAPON );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUTTON : CLEAR LIST
	GET_PLUA_POS( BUT_CLEAR_WEAPON_LIST, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_BUT_CLEAR_WEAPON_LIST );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUTTON : OPEN
	GET_PLUA_POS( BUT_OPEN_WEAPON, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_BUT_OPEN_WEAPON );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC : ACCESSORY
	GET_PLUA_POS( STATIC_ACCESSORY, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_STATIC_ACCESSORY );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// LIST : ACCESSORY
	GET_PLUA_POS( LIST_ACCESSORY, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_LIST_ACCESSORY );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUTTON : CLEAR ACCESSORY LIST
	GET_PLUA_POS( BUT_CLEAR_ACCESSORY_LIST, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_BUT_CLEAR_ACCESSORY_LIST );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUTTON : ACCESSORY OPEN
	GET_PLUA_POS( BUT_OPEN_ACCESSORY, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_BUT_OPEN_ACCESSORY );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// LIST : ATTACH_BONES
	GET_PLUA_POS( LIST_ATTACH_BONES, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_LIST_ATTACH_BONES );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	CDXUTListBox* pListBox = reinterpret_cast<CDXUTListBox*>( pControl );
	pListBox->RemoveAllItems();
	pListBox->AddItem( L"Bip01", NULL );
	pListBox->AddItem( L"Dummy1_Rhand", NULL );
	pListBox->AddItem( L"Dummy2_Lhand", NULL );
	pListBox->AddItem( L"Dummy3_Weapon", NULL );
	pListBox->AddItem( L"Bip01_Head", NULL );
	pListBox->AddItem( L"Bip01_HeadNub", NULL );
	pListBox->AddItem( L"Bip01_R_Upperarm", NULL );
	pListBox->AddItem( L"Bip01_L_Upperarm", NULL );
	pListBox->AddItem( L"Bip01_R_Forearm", NULL );
	pListBox->AddItem( L"Bip01_L_Forearm", NULL );
	pListBox->AddItem( L"Bip01_R_Hand", NULL );
	pListBox->AddItem( L"Bip01_L_Hand", NULL );
	pListBox->AddItem( L"Bip01_Spine", NULL );
	pListBox->AddItem( L"Bip01_Spine1", NULL );
	pListBox->AddItem( L"Bip01_Pelvis", NULL );
	pListBox->AddItem( L"Bip01_R_Thigh", NULL );
	pListBox->AddItem( L"Bip01_L_Thigh", NULL );
	pListBox->AddItem( L"Bip01_R_Foot", NULL );
	pListBox->AddItem( L"Bip01_L_Foot", NULL );

	// STATIC_SCENE
	GET_PLUA_POS( STATIC_SCENE, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_STATIC_SCENE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_SCENE_OPEN
	GET_PLUA_POS( BUT_SCENE_OPEN, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_BUT_SCENE_OPEN );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_SCENE_SAVE
	GET_PLUA_POS( BUT_SCENE_SAVE, nX, nY, nW, nH );
	pControl = m_EquipUI.GetControl( UI_BUT_SCENE_SAVE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
}
#pragma endregion EquipUI 재설정

#pragma region ResetWeaponUI
void CX2ViewerUI::ResetWeaponUI( KLuaManager* luaManager )
{
	CDXUTControl* pControl;
	D3DXVECTOR3 vTemp;
	WCHAR strNum[128] = L"";
	int nX, nY, nW, nH;
	nX = nY = nW = nH = 0;

	// STATIC_WEAPON_BONE_NAME
	GET_PLUA_POS( STATIC_WEAPON_BONE_NAME, nX, nY, nW, nH );
	pControl = m_WeaponUI.GetControl( UI_STATIC_WEAPON_BONE_NAME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_WEAPON_BONE_NAME
	GET_PLUA_POS( EDIT_WEAPON_BONE_NAME, nX, nY, nW, nH );
	pControl = m_WeaponUI.GetControl( UI_EDIT_WEAPON_BONE_NAME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
		
	// STATIC_WEAPON_BONE_ROT_X
	GET_PLUA_POS( STATIC_WEAPON_BONE_ROT_X, nX, nY, nW, nH );
	pControl = m_WeaponUI.GetControl( UI_STATIC_WEAPON_BONE_ROT_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_WEAPON_BONE_ROT_Y
	GET_PLUA_POS( STATIC_WEAPON_BONE_ROT_Y, nX, nY, nW, nH );
	pControl = m_WeaponUI.GetControl( UI_STATIC_WEAPON_BONE_ROT_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_WEAPON_BONE_ROT_Z
	GET_PLUA_POS( STATIC_WEAPON_BONE_ROT_Z, nX, nY, nW, nH );
	pControl = m_WeaponUI.GetControl( UI_STATIC_WEAPON_BONE_ROT_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_ROT_X
	GET_PLUA_POS( EDIT_WEAPON_BONE_ROT_X, nX, nY, nW, nH );
	pControl = m_WeaponUI.GetControl( UI_EDIT_WEAPON_BONE_ROT_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_ROT_Y
	GET_PLUA_POS( EDIT_WEAPON_BONE_ROT_Y, nX, nY, nW, nH );
	pControl = m_WeaponUI.GetControl( UI_EDIT_WEAPON_BONE_ROT_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_ROT_Z
	GET_PLUA_POS( EDIT_WEAPON_BONE_ROT_Z, nX, nY, nW, nH );
	pControl = m_WeaponUI.GetControl( UI_EDIT_WEAPON_BONE_ROT_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
}
#pragma endregion WeaponUI 재설정

#pragma region ResetAccessoryUI
void CX2ViewerUI::ResetAccessoryUI( KLuaManager* luaManager )
{
	CDXUTControl* pControl;
	D3DXVECTOR3 vTemp;
	WCHAR strNum[128] = L"";
	int nX, nY, nW, nH;
	nX = nY = nW = nH = 0;

	// STATIC_ACCESSORY_BONE_NAME
	GET_PLUA_POS( STATIC_ACCESSORY_BONE_NAME, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_STATIC_ACCESSORY_BONE_NAME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_ACCESSORY_BONE_NAME
	GET_PLUA_POS( EDIT_ACCESSORY_BONE_NAME, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_EDIT_ACCESSORY_BONE_NAME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_ACCESSORY_X
	GET_PLUA_POS( STATIC_ACCESSORY_X, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_STATIC_ACCESSORY_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_ACCESSORY_Y
	GET_PLUA_POS( STATIC_ACCESSORY_Y, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_STATIC_ACCESSORY_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_ACCESSORY_Z
	GET_PLUA_POS( STATIC_ACCESSORY_Z, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_STATIC_ACCESSORY_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_ACCESSORY_MOVE
	GET_PLUA_POS( STATIC_ACCESSORY_MOVE, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_STATIC_ACCESSORY_MOVE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_ACCESSORY_ROT
	GET_PLUA_POS( STATIC_ACCESSORY_ROT, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_STATIC_ACCESSORY_ROT );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_ACCESSORY_SCALE
	GET_PLUA_POS( STATIC_ACCESSORY_SCALE, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_STATIC_ACCESSORY_SCALE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// EDIT_ACCESSORY_MOVE_X
	GET_PLUA_POS( EDIT_ACCESSORY_MOVE_X, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_EDIT_ACCESSORY_MOVE_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_ACCESSORY_MOVE_Y
	GET_PLUA_POS( EDIT_ACCESSORY_MOVE_Y, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_EDIT_ACCESSORY_MOVE_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_ACCESSORY_MOVE_Z
	GET_PLUA_POS( EDIT_ACCESSORY_MOVE_Z, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_EDIT_ACCESSORY_MOVE_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// EDIT_ACCESSORY_ROT_X
	GET_PLUA_POS( EDIT_ACCESSORY_ROT_X, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_EDIT_ACCESSORY_ROT_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_ACCESSORY_ROT_Y
	GET_PLUA_POS( EDIT_ACCESSORY_ROT_Y, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_EDIT_ACCESSORY_ROT_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_ACCESSORY_ROT_Z
	GET_PLUA_POS( EDIT_ACCESSORY_ROT_Z, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_EDIT_ACCESSORY_ROT_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// EDIT_ACCESSORY_SCALE_X
	GET_PLUA_POS( EDIT_ACCESSORY_SCALE_X, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_EDIT_ACCESSORY_SCALE_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_ACCESSORY_SCALE_Y
	GET_PLUA_POS( EDIT_ACCESSORY_SCALE_Y, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_EDIT_ACCESSORY_SCALE_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_ACCESSORY_SCALE_Z
	GET_PLUA_POS( EDIT_ACCESSORY_SCALE_Z, nX, nY, nW, nH );
	pControl = m_AccessoryUI.GetControl( UI_EDIT_ACCESSORY_SCALE_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
}
#pragma endregion Accessory UI 재설정

#pragma region ResetAnimationUI
void CX2ViewerUI::ResetAnimationUI( KLuaManager* luaManager )
{
	CDXUTControl* pControl;
	D3DXVECTOR3 vTemp;
	WCHAR strNum[128] = L"";
	int nX, nY, nW, nH;
	nX = nY = nW = nH = 0;

	// SLIDE_ANIM_FRAME
	GET_PLUA_POS( SLIDE_ANIM_FRAME, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_SLIDE_ANIM_FRAME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// BUT_PLAY_PAUSE
	GET_PLUA_POS( BUT_ANIM_PLAY_PAUSE, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_BUT_ANIM_PLAY_PAUSE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_STOP
	GET_PLUA_POS( BUT_ANIM_STOP, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_BUT_ANIM_STOP );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_STOP
	GET_PLUA_POS( BUT_ANIM_PREV_FRAME, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_BUT_ANIM_PREV_FRAME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_STOP
	GET_PLUA_POS( BUT_ANIM_NEXT_FRAME, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_BUT_ANIM_NEXT_FRAME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_ANIM_FRAME
	GET_PLUA_POS( STATIC_ANIM_FRAME, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_STATIC_ANIM_FRAME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// EDIT_ANIM_FRAME_TIME_INC
	GET_PLUA_POS( EDIT_ANIM_FRAME_TIME_INC, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_EDIT_ANIM_FRAME_TIME_INC );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	reinterpret_cast<CDXUTEditBox*>( pControl )->SetTextFloatArray( &m_fAnimTimeInc, 1 );

	// STATIC_ANIM_SPEED
	GET_PLUA_POS( STATIC_ANIM_SPEED, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_STATIC_ANIM_SPEED );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// SLIDE_ANIM_SPEED
	GET_PLUA_POS( SLIDE_ANIM_SPEED, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_SLIDE_ANIM_SPEED );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// COMBO_PLAY_TYPE
	GET_PLUA_POS( COMBO_PLAY_TYPE, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_COMBO_PLAY_TYPE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	CDXUTComboBox* pComboBox = (CDXUTComboBox*)pControl;
	pComboBox->RemoveAllItems();
	pComboBox->AddItem( L"ONE", NULL );
	pComboBox->AddItem( L"LOOP", NULL );
	pComboBox->SetDropHeight( 20 );
	switch( reinterpret_cast<CX2ViewerSkinMesh*>( SiGetObject( OS_SKIN_MESH ))->GetPlayType() )
	{
	case CKTDGXSkinAnim::XAP_ONE_WAIT:
		pComboBox->SetSelectedByText( L"ONE" );
		break;

	case CKTDGXSkinAnim::XAP_LOOP:
		pComboBox->SetSelectedByText( L"LOOP" );
		break;
	}

	// STATIC_ANIMATION
	GET_PLUA_POS( STATIC_ANIMATION, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_STATIC_ANIMATION );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// LIST_ANIMATION
	GET_PLUA_POS( LIST_ANIMATION, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_LIST_ANIMATION );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// STATIC_ANIM_NUM
	GET_PLUA_POS( STATIC_ANIM_NUM, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_STATIC_ANIM_NUM );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	swprintf( strNum, L"( %d )", m_AnimationUI.GetListBox( UI_LIST_ANIMATION )->GetSize() );
	reinterpret_cast<CDXUTStatic*>( pControl )->SetText( strNum );
	// EDIT_ANIM_NAME
	GET_PLUA_POS( EDIT_ANIM_NAME, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_EDIT_ANIM_NAME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// BUT_ANIM_NAME_CHANGE
	GET_PLUA_POS( BUT_ANIM_NAME_CHANGE, nX, nY, nW, nH );
	pControl = m_AnimationUI.GetControl( UI_BUT_ANIM_NAME_CHANGE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
}
#pragma endregion Animation UI 재설정

#pragma region ResetRenderParamUI
void CX2ViewerUI::ResetRenderParamUI( KLuaManager* luaManager )
{
	CDXUTControl* pControl;
	D3DXVECTOR3 vTemp;
	WCHAR strNum[128] = L"";
	int nX, nY, nW, nH;
	nX = nY = nW = nH = 0;

	// STATIC_RENDERTYPE
	GET_PLUA_POS( STATIC_RENDERTYPE, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_RENDERTYPE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// COMBO_RENDERTYPE
	GET_PLUA_POS( COMBO_RENDERTYPE, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_COMBO_RENDERTYPE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_CULLMODE
	GET_PLUA_POS( STATIC_CULLMODE, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_CULLMODE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//COMBO_CULLMODE
	GET_PLUA_POS( COMBO_CULLMODE, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_COMBO_CULLMODE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	
	// STATIC_CARTOON_TEX_TYPE
	GET_PLUA_POS( STATIC_CARTOON_TEX_TYPE, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_CARTOON_TEX_TYPE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// COMBO_CARTOON_TEX_TYPE
	GET_PLUA_POS( COMBO_CARTOON_TEX_TYPE, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_COMBO_CARTOON_TEX_TYPE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_SRCBLEND
	GET_PLUA_POS( STATIC_SRCBLEND, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_SRCBLEND );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// COMBO_SRCBLEND
	GET_PLUA_POS( COMBO_SRCBLEND, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_COMBO_SRCBLEND );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// STATIC_DESTBLEND
	GET_PLUA_POS( STATIC_DESTBLEND, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_DESTBLEND );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// COMBO_DESTBLEND
	GET_PLUA_POS( COMBO_DESTBLEND, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_COMBO_DESTBLEND );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// CHECK_ALPHA_BLEND
	GET_PLUA_POS( CHECK_ALPHA_BLEND, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_CHECK_ALPHA_BLEND );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// COMBO_ZENABLE
	GET_PLUA_POS( CHECK_ZENABLE, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_CHECK_ZENABLE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// COMBO_ZWRITABLE
	GET_PLUA_POS( CHECK_ZWRITABLE, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_CHECK_ZWRITABLE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//EDIT_OUTLINE_WIDE
	GET_PLUA_POS( EDIT_OUTLINE_WIDE, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_OUTLINE_WIDE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );

	//STATIC_OUTLINE_COLOR
	GET_PLUA_POS( STATIC_OUTLINE_COLOR, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_OUTLINE_COLOR );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_OUTLINE_COLOR_A
	GET_PLUA_POS( STATIC_OUTLINE_COLOR_A, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_OUTLINE_COLOR_A );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_OUTLINE_COLOR_R
	GET_PLUA_POS( STATIC_OUTLINE_COLOR_R, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_OUTLINE_COLOR_R );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_OUTLINE_COLOR_G
	GET_PLUA_POS( STATIC_OUTLINE_COLOR_G, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_OUTLINE_COLOR_G );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_OUTLINE_COLOR_B
	GET_PLUA_POS( STATIC_OUTLINE_COLOR_B, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_OUTLINE_COLOR_B );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//EDIT_OUTLINE_COLOR_A
	GET_PLUA_POS( EDIT_OUTLINE_COLOR_A, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_OUTLINE_COLOR_A );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );
	//EDIT_OUTLINE_COLOR_R
	GET_PLUA_POS( EDIT_OUTLINE_COLOR_R, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_OUTLINE_COLOR_R );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );
	//EDIT_OUTLINE_COLOR_G
	GET_PLUA_POS( EDIT_OUTLINE_COLOR_G, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_OUTLINE_COLOR_G );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );
	//EDIT_OUTLINE_COLOR_B
	GET_PLUA_POS( EDIT_OUTLINE_COLOR_B, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_OUTLINE_COLOR_B );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );

	//STATIC_COLOR
	GET_PLUA_POS( STATIC_COLOR, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_COLOR );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_COLOR_A
	GET_PLUA_POS( STATIC_COLOR_A, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_COLOR_A );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_COLOR_R
	GET_PLUA_POS( STATIC_COLOR_R, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_COLOR_R );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_COLOR_G
	GET_PLUA_POS( STATIC_COLOR_G, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_COLOR_G );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_COLOR_B
	GET_PLUA_POS( STATIC_COLOR_B, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_COLOR_B );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//EDIT_COLOR_A
	GET_PLUA_POS( EDIT_COLOR_A, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_COLOR_A );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );
	//EDIT_COLOR_R
	GET_PLUA_POS( EDIT_COLOR_R, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_COLOR_R );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );
	//EDIT_COLOR_G
	GET_PLUA_POS( EDIT_COLOR_G, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_COLOR_G );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );
	//EDIT_COLOR_B
	GET_PLUA_POS( EDIT_COLOR_B, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_COLOR_B );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );

	//STATIC_LIGHTFLOW_WIDE
	GET_PLUA_POS( STATIC_LIGHTFLOW_WIDE, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_LIGHTFLOW_WIDE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_LIGHTFLOW_WIDE
	GET_PLUA_POS( EDIT_LIGHTFLOW_WIDE, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_LIGHTFLOW_WIDE );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );

	//STATIC_LIGHTFLOW_IMPACT
	GET_PLUA_POS( STATIC_LIGHTFLOW_IMPACT, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_LIGHTFLOW_IMPACT );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_LIGHTFLOW_IMPACT_MIN
	GET_PLUA_POS( STATIC_LIGHTFLOW_IMPACT_MIN, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_LIGHTFLOW_IMPACT_MIN );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_LIGHTFLOW_IMPACT_MAX
	GET_PLUA_POS( STATIC_LIGHTFLOW_IMPACT_MAX, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_LIGHTFLOW_IMPACT_MAX );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_LIGHTFLOW_IMPACT_ANIMTIME
	GET_PLUA_POS( STATIC_LIGHTFLOW_IMPACT_ANIMTIME, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_LIGHTFLOW_IMPACT_ANIMTIME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_LIGHTFLOW_IMPACT_MIN
	GET_PLUA_POS( EDIT_LIGHTFLOW_IMPACT_MIN, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_LIGHTFLOW_IMPACT_MIN );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );
	//EDIT_LIGHTFLOW_IMPACT_MAX
	GET_PLUA_POS( EDIT_LIGHTFLOW_IMPACT_MAX, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_LIGHTFLOW_IMPACT_MAX );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );
	//EDIT_LIGHTFLOW_IMPACT_ANIMTIME
	GET_PLUA_POS( EDIT_LIGHTFLOW_IMPACT_ANIMTIME, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_LIGHTFLOW_IMPACT_ANIMTIME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );

	//STATIC_LIGHTFLOW_POINT
	GET_PLUA_POS( STATIC_LIGHTFLOW_POINT, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_LIGHTFLOW_POINT );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_LIGHTFLOW_POINT_X
	GET_PLUA_POS( STATIC_LIGHTFLOW_POINT_X, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_LIGHTFLOW_POINT_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_LIGHTFLOW_POINT_Y
	GET_PLUA_POS( STATIC_LIGHTFLOW_POINT_Y, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_LIGHTFLOW_POINT_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_LIGHTFLOW_POINT_Z
	GET_PLUA_POS( STATIC_LIGHTFLOW_POINT_Z, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_LIGHTFLOW_POINT_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_LIGHTFLOW_POINT_X
	GET_PLUA_POS( EDIT_LIGHTFLOW_POINT_X, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_LIGHTFLOW_POINT_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );
	//EDIT_LIGHTFLOW_POINT_Y
	GET_PLUA_POS( EDIT_LIGHTFLOW_POINT_Y, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_LIGHTFLOW_POINT_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );
	//EDIT_LIGHTFLOW_POINT_Z
	GET_PLUA_POS( EDIT_LIGHTFLOW_POINT_Z, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_LIGHTFLOW_POINT_Z );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	((CDXUTEditBox*)pControl)->SetText( L"" );

	//STATIC_TEXOFFSET_STAGE0
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE0, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE0 );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE0_X
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE0_X, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE0_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE0_Y
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE0_Y, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE0_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE0_MIN
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE0_MIN, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE0_MIN );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE0_MAX
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE0_MAX, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE0_MAX );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE0_ANIMTIME
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE0_ANIMTIME, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE0_ANIMTIME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE0_MIN_X
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE0_MIN_X, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE0_MIN_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE0_MIN_Y
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE0_MIN_Y, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE0_MIN_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE0_MAX_X
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE0_MAX_X, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE0_MAX_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE0_MAX_Y
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE0_MAX_Y, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE0_MAX_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE0_ANIMTIME
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE0_ANIMTIME, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE0_ANIMTIME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_TEXOFFSET_STAGE1
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE1, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE1 );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE1_X
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE1_X, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE1_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE1_Y
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE1_Y, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE1_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE1_MIN
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE1_MIN, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE1_MIN );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE1_MAX
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE1_MAX, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE1_MAX );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE1_ANIMTIME
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE1_ANIMTIME, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE1_ANIMTIME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE1_MIN_X
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE1_MIN_X, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE1_MIN_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE1_MIN_Y
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE1_MIN_Y, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE1_MIN_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE1_MAX_X
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE1_MAX_X, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE1_MAX_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE1_MAX_Y
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE1_MAX_Y, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE1_MAX_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE1_ANIMTIME
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE1_ANIMTIME, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE1_ANIMTIME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	//STATIC_TEXOFFSET_STAGE2
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE2, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE2 );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE2_X
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE2_X, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE2_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE2_Y
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE2_Y, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE2_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE2_MIN
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE2_MIN, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE2_MIN );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE2_MAX
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE2_MAX, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE2_MAX );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//STATIC_TEXOFFSET_STAGE2_ANIMTIME
	GET_PLUA_POS( STATIC_TEXOFFSET_STAGE2_ANIMTIME, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_STATIC_TEXOFFSET_STAGE2_ANIMTIME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE2_MIN_X
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE2_MIN_X, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE2_MIN_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE2_MIN_Y
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE2_MIN_Y, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE2_MIN_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE2_MAX_X
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE2_MAX_X, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE2_MAX_X );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//EDIT_TEXOFFSET_STAGE2_MAX_Y
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE2_MAX_Y, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE2_MAX_Y );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	// EDIT_TEXOFFSET_STAGE2_ANIMTIME
	GET_PLUA_POS( EDIT_TEXOFFSET_STAGE2_ANIMTIME, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_EDIT_TEXOFFSET_STAGE2_ANIMTIME );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );

	// BUT_PARAM_OK
	GET_PLUA_POS( BUT_PARAM_OK, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_BUT_PARAM_OK );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
	//BUT_PARAM_CANCEL
	GET_PLUA_POS( BUT_PARAM_CANCEL, nX, nY, nW, nH );
	pControl = m_RenderParamUI.GetControl( UI_BUT_PARAM_CANCEL );
	pControl->SetLocation( nX, nY );
	pControl->SetSize( nW, nH );
}
#pragma endregion ResetRenderParamUI UI 재설정

#pragma region 
void CX2ViewerUI::InitCharacerStringMap()
{
	m_mapCharacterIDStr.clear();
	
	m_mapCharacterIDStr.insert( std::make_pair( CI_NONE,				L"CI_NONE" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_ELSWORD,				L"CI_ELSWORD" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_SWORD_KNIGHT,		L"CI_SWORD_KNIGHT" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_MAGIC_KNIGHT,		L"CI_MAGIC_KNIGHT" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_SHEATH_KNIGHT,		L"CI_SHEATH_KNIGHT" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_LORD_KNIGHT,			L"CI_LORD_KNIGHT" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_RUNE_SLAYER,			L"CI_RUNE_SLAYER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_INFINITE_SWORD,		L"CI_INFINITE_SWORD" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_AISHA,				L"CI_AISHA" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_DARK_MAGICIAN,		L"CI_DARK_MAGICIAN" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_BATTLE_MAGICIAN,		L"CI_BATTLE_MAGICIAN" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_ELEMENTAL_MASTER,	L"CI_ELEMENTAL_MASTER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_VOID_PRINCESS,		L"CI_VOID_PRINCESS" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_DIMENSION_WITCH,		L"CI_DIMENSION_WITCH" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_LENA,				L"CI_LENA" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_COMBAT_RANGER,		L"CI_COMBAT_RANGER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_SNIPING_RANGER,		L"CI_SNIPING_RANGER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_TRAPPING_RANGER,		L"CI_TRAPPING_RANGER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_WIND_SNIKER,			L"CI_WIND_SNIKER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_GRAND_ARCHER,		L"CI_GRAND_ARCHER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_NIGHT_WATCHER,		L"CI_NIGHT_WATCHER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_RAVEN,				L"CI_RAVEN" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_SWORD_TAKER,			L"CI_SWORD_TAKER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_WEAPON_TAKER,		L"CI_WEAPON_TAKER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_BLADE_MASTER,		L"CI_BLADE_MASTER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_RECKRESS_FIST,		L"CI_RECKRESS_FIST" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_VETERAN_COMMANDER,	L"CI_VETERAN_COMMANDER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_EVE,					L"CI_EVE" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_EXOTIC_GEAR,			L"CI_EXOTIC_GEAR" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_ARCHTECTURE,			L"CI_ARCHTECTURE" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_ELECTRA,				L"CI_ELECTRA" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_CODE_NEMESIS,		L"CI_CODE_NEMESIS" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_CODE_EMPRESS,		L"CI_CODE_EMPRESS" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_BATTLE_SERAPH,		L"CI_BATTLE_SERAPH" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_CHUNG,				L"CI_CHUNG" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_FURY_GUARDIAN,		L"CI_FURY_GUARDIAN" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_SHOOTING_GUARDIAN,	L"CI_SHOOTING_GUARDIAN" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_SHELLING_GUARDING,	L"CI_SHELLING_GUARDING" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_IRON_PALADIN,		L"CI_IRON_PALADIN" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_DEADLY_CHASER,		L"CI_DEADLY_CHASER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_TATICAL_TROOPER,		L"CI_TATICAL_TROOPER" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_ARA,					L"CI_ARA" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_LITTLE_HSIEN,		L"CI_LITTLE_HSIEN" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_LITTLE_DEVIL,		L"CI_LITTLE_DEVIL" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_SAKRA_DEVANAM,		L"CI_SAKRA_DEVANAM" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_YAMA_RAJA,			L"CI_YAMA_RAJA" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_ELISIS,				L"CI_ELISIS" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_SAVER_KNIGHT,		L"CI_SAVER_KNIGHT" ) );
	m_mapCharacterIDStr.insert( std::make_pair( CI_PYRO_KINIGHT,		L"CI_PYRO_KINIGHT" ) );
}
#pragma endregion 
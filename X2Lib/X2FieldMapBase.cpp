#include "StdAfx.h"
#include ".\x2fieldmapbase.h"



CX2FieldMapBase::CX2FieldMapBase(void) :
m_pTextureWholeMap( NULL )
	, m_fFov( 0.785f )
	, m_fAspect( 1.f )
	, m_pMyChar( NULL )
{

	for( int i=0; i<ARRAY_SIZE(m_pTextureIcon); i++ )
	{
		m_pTextureIcon[i] = NULL;
	}

	m_pTextureIcon[MI_MYSELF]				= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Char_My.dds" );
	m_pTextureIcon[MI_PARTY] 				= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Char_Party.dds" );
	m_pTextureIcon[MI_PARTY_ARROW]			= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Arrow.dds" );
	m_pTextureIcon[MI_MARKET_PLACE]			= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Market.dds" );
	m_pTextureIcon[MI_PVP_ARENA]			= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Arena.dds" );
	m_pTextureIcon[MI_BANK] 				= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Bank.dds" );
	m_pTextureIcon[MI_POSTBOX] 				= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Post.dds" );
	m_pTextureIcon[MI_GATE] 				= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Gate.dds" );
	m_pTextureIcon[MI_NPC_COMMON]			= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Char_NPC.dds" );
	m_pTextureIcon[MI_NPC_WEAPON_SHOP]		= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Shop_Weapon.dds" );
	m_pTextureIcon[MI_NPC_ACCESSARY_SHOP]	= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Shop_Acc.dds" );
	m_pTextureIcon[MI_NPC_ALCHEMIST]		= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Shop_Alche.dds" );

	m_pTextureIcon[MI_NPC_COMPLETE_QUEST]			= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Quset_Complete_A.dds" );
	m_pTextureIcon[MI_NPC_COMPLETE_REPEAT_QUEST]	= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Quest_Complete_B.dds" );
	m_pTextureIcon[MI_NPC_INCOMPLETE_QUEST]			= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Quest_Not_Complete.dds" );
	m_pTextureIcon[MI_NPC_AVAIL_QUEST]				= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Quest_A.dds" );
	m_pTextureIcon[MI_NPC_AVAIL_REPEAT_QUEST]		= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Quest_B.dds" );
	m_pTextureIcon[MI_NPC_AVAIL_EVENT_QUEST]		= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Event_Notice.dds" );

	m_pTextureIcon[MI_BOARD]				= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Board.dds" );

#pragma region 

	for ( int i = 0; i < ARRAY_SIZE( m_pPartyMember ); ++i )
		m_pPartyMember[i] = NULL;

	m_ViewPort.X 		= 0;
	m_ViewPort.Y 		= 0;
	m_ViewPort.Width	= (DWORD) 512;
	m_ViewPort.Height	= (DWORD) 512;
	m_ViewPort.MinZ 	= 0.f;
	m_ViewPort.MaxZ 	= 1.f;
}

CX2FieldMapBase::~CX2FieldMapBase(void)
{
	SAFE_CLOSE( m_pTextureWholeMap );

	for( int i=0; i<ARRAY_SIZE(m_pTextureIcon); i++ )
	{
		SAFE_CLOSE( m_pTextureIcon[i] );
	}

	m_pMyChar = NULL;

	for ( int i = 0; i < ARRAY_SIZE( m_pPartyMember ); ++i )
		m_pPartyMember[i] = NULL;
}


// minimap window texture 내부의 좌표값을 입력으로 받아 내부에 있는지 판단
bool CX2FieldMapBase::IsInWindowTexture( const D3DXVECTOR2& vProjectedPos )
{
	if( vProjectedPos.x < 0.f )
		return false;

	if( vProjectedPos.y < 0.f )
		return false;

	if( vProjectedPos.x > m_vWindowTextureSize.x )
		return false;

	if( vProjectedPos.y > m_vWindowTextureSize.y )
		return false;

	return true;
}


// 3D 공간상의 좌표를 2D 미니맵 스크린샷 상의 좌표로
D3DXVECTOR2 CX2FieldMapBase::ProjectToScreenShot( const D3DXVECTOR3& vPos )
{
	D3DXVECTOR3 vProjected;
	D3DXVec3Project( &vProjected, &vPos, &m_ViewPort, &m_matProj, &m_matView, &m_matWorld );

	return D3DXVECTOR2( vProjected.x, vProjected.y );
}


// 2D 스크린샷 상의 좌표를 2D Minimap window texture 안의 좌표로
D3DXVECTOR2 CX2FieldMapBase::CalcPosInWindowTexture( const D3DXVECTOR2& vPos ) 
{
	return vPos - m_vWindowTexturePos;
}


// 2D Minimap window texture 안의 좌표를 실제 렌더링될 screen 상의 좌표로
D3DXVECTOR2 CX2FieldMapBase::CalcPosOnScreen( const D3DXVECTOR2& vPos ) 
{
	D3DXVECTOR2 vScreenPos = vPos;
	vScreenPos.x *= m_vMiniMapWindowSize.x / m_vWindowTextureSize.x;
	vScreenPos.y *= m_vMiniMapWindowSize.y / m_vWindowTextureSize.y;

	return m_vMiniMapWindowPos + vScreenPos;
}



/** @function 	: ResetCameraMatrix
	@brief 		: 월드맵 관련 카메라( Eye Position, LookAt Position, Fov( Field Of View ), Aspect Ratio ) 값 설정
*/
void CX2FieldMapBase::ResetCameraMatrix( int iMapID )
{
	switch( iMapID )
	{
#pragma region 루벤 월드맵 카메라
	case SEnum::VMI_RUBEN:
		{
			m_vEyePos	= D3DXVECTOR3( 1274.792f, 1125.527f, -5136.493f );
			m_vLookAt	= D3DXVECTOR3( 2414.343f, 1132.362f, -1496.845f );
			m_fFov		= D3DXToRadian( 39.677f );
			m_fAspect	= 1.8f;
		} break;

	case SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01:
		{
			m_vEyePos	= D3DXVECTOR3( 2085.998f, 11330.764f, -13711.854f );
			m_vLookAt	= D3DXVECTOR3( 4098.498f,  9544.014f, -10838.354f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
#pragma endregion (마을, 노아스의 호수)

#pragma region 엘더 월드맵 카메라
	case SEnum::VMI_ELDER:		// 마을
		{
			m_vEyePos	= D3DXVECTOR3( -146.066f, 3925.611f, -13455.207f );
			m_vLookAt	= D3DXVECTOR3(  -82.319f, 2255.196f, -1118.566f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;

		} break;
	
	case SEnum::VMI_BATTLE_FIELD_ELDER_FIELD_01:	// 월리의 기념교
		{
			m_vEyePos	= D3DXVECTOR3(  -65.146f, 5140.780f, -19388.723f );
			m_vLookAt	= D3DXVECTOR3( 2740.993f, 4960.204f,  -5464.505f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;

	case SEnum::VMI_BATTLE_FIELD_ELDER_REST_00:		// 보리스의 평야
		{
			m_vEyePos	= D3DXVECTOR3( 19260.854f, 4172.215f, -13523.379f );
			m_vLookAt	= D3DXVECTOR3( 18249.990f, 4503.619f,  -4707.083f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;

	case SEnum::VMI_BATTLE_FIELD_ELDER_FIELD_02:	// 쌍둥이 감시탑
		{
			m_vEyePos	= D3DXVECTOR3( 34954.887f, 5309.212f, -15253.939f );
			m_vLookAt	= D3DXVECTOR3( 32551.742f, 4222.235f, -4728.554f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
#pragma endregion ( 마을, 월리 기념교, 보리스 평야_휴식처, 쌍둥이 감시탑 )

#pragma region 베스마 월드맵 카메라
	case SEnum::VMI_BESMA:
		{
			m_vEyePos	= D3DXVECTOR3( 3291.935f, 6791.385f, -12993.682f );
			m_vLookAt	= D3DXVECTOR3( 4709.893f, 5789.633f,  1064.364f );
			//2013.11.7 김창한 마을 동선 수정 작업.
			/*m_vEyePos	= D3DXVECTOR3( 1472.159f, 7698.483f, -16419.963f );
			m_vLookAt	= D3DXVECTOR3( 2091.424f, 5407.994f,  -2236.412f );*/
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_01:
		{
			m_vEyePos	= D3DXVECTOR3( -21362.055f, 6009.691f, -1911.630f );
			m_vLookAt	= D3DXVECTOR3( -14352.490f, 5029.604f,  8524.340f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_BESMA_REST_00:
		{
			m_vEyePos	= D3DXVECTOR3(  524.692f, 3399.535f,  456.215f );
			m_vLookAt	= D3DXVECTOR3( 1072.023f, 1729.144f, 8055.742f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_02:
		{
			m_vEyePos	= D3DXVECTOR3(     930.0f, 8671.783f, -16126.716f );
			m_vLookAt	= D3DXVECTOR3( 11514.109f, 3049.915f,  -3851.114f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_03:
		{
			m_vEyePos	= D3DXVECTOR3( 24414.623f, 8586.150f, -2551.433f );
			m_vLookAt	= D3DXVECTOR3( 16225.633f, 2983.920f, 10721.196f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
#pragma endregion ( 마을, 베스아가라 폭포, 아슬아슬 하늘길_휴식처, 레이키키 호수, 토리투게라 캐년 )
	
#pragma region 알테라 월드맵 카메라
	case SEnum::VMI_ALTERA:
		{
			m_vEyePos	= D3DXVECTOR3( 29419.719f, 3525.825f, -10963.555f );
			m_vLookAt	= D3DXVECTOR3( 29543.623f, 1385.362f,  -1053.433f );
			//2013.11.7 김창한 마을 동선 수정 작업.
			/*m_vEyePos	= D3DXVECTOR3( -23.610f, 4109.580f, -10304.420f );
			m_vLookAt	= D3DXVECTOR3(  21.532f, 1805.671f,     84.281f );*/
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_01:
		{
			m_vEyePos	= D3DXVECTOR3( -11420.804f, 19666.855f, -24558.283f );
			m_vLookAt	= D3DXVECTOR3(  -4914.962f, 16722.271f, -12323.271f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00:
		{
			m_vEyePos	= D3DXVECTOR3( -2938.691f, 6841.651f, -26348.705f );
			m_vLookAt	= D3DXVECTOR3( -1689.506f, 5268.320f, -14463.540f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_02:
		{
			m_vEyePos	= D3DXVECTOR3( 582.554f, 19652.393f, -12058.375f );
			m_vLookAt	= D3DXVECTOR3( 664.451f, 19433.611f,  -8930.830f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_03:
		{
			m_vEyePos	= D3DXVECTOR3( -28969.400f, 5220.237f, -10913.870f );
			m_vLookAt	= D3DXVECTOR3( -16129.707f,	3457.279f,  -7110.588f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
#pragma endregion ( 마을, 정화의 무덤, 천공의 기지, 퐁고족 은신처_휴식처, 나소드 폐기장 )

#pragma region 페이타 월드맵 카메라
	case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_01:
		{
			m_vEyePos	= D3DXVECTOR3( -26529.629f, 9009.373f, -13523.900f );
			m_vLookAt	= D3DXVECTOR3( -26423.480f,	4287.000f,  -1143.804f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_PEITA:
		{
			m_vEyePos	= D3DXVECTOR3( -8340.546f, 4571.142f, -11710.418f );
			m_vLookAt	= D3DXVECTOR3( -8215.504f, 3130.585f,  -6848.827f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_02:
		{
			m_vEyePos	= D3DXVECTOR3( 211.167f, 9611.736f, -15916.546f );
			m_vLookAt	= D3DXVECTOR3( -17.606f, 7535.575f, -2023.345f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_03:
		{
			m_vEyePos	= D3DXVECTOR3( -492.566f, 4737.257f, -13368.516f );
			m_vLookAt	= D3DXVECTOR3( -166.027f, 3792.669f, -2061.968f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
#pragma endregion ( 순례자의 관문, 마을, 순례자의 터, 망자의 언덕 )

#pragma region 벨더 월드맵 카메라
	case SEnum::VMI_VELDER:
		{
			m_vEyePos	= D3DXVECTOR3( 1341.619f, 3071.656f, -9848.519f );
			m_vLookAt	= D3DXVECTOR3( 1606.615f, 2452.488f,   271.273f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_01:
		{
			m_vEyePos	= D3DXVECTOR3( 22830.664f, 6885.775f, -11951.539f );
			m_vLookAt	= D3DXVECTOR3( 24424.658f, 3939.283f,   -887.091f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_REST_00:
		{
			m_vEyePos	= D3DXVECTOR3( -8588.021f, 4468.711f, -8081.594f );
			m_vLookAt	= D3DXVECTOR3( -8407.028f, 3464.584f,    91.001f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_02:
		{
			m_vEyePos	= D3DXVECTOR3( -31940.904f, 8545.185f, 17040.867f );
			m_vLookAt	= D3DXVECTOR3( -18499.314f, 1547.670f, 16923.559f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_03:
		{
			m_vEyePos	= D3DXVECTOR3( 12401.698f, 8919.659f, -10264.087f );
			m_vLookAt	= D3DXVECTOR3(  4419.365f, 2650.891f, -2295.221f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_04:
		{
			m_vEyePos	= D3DXVECTOR3( 32031.506f, 6333.563f, 24426.586f );
			m_vLookAt	= D3DXVECTOR3( 21086.574f,  520.146f, 43200.227f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_SHIP_STAGE:
		{
			m_vEyePos	= D3DXVECTOR3( 2995.209f, 2934.804f, -5370.426f );
			m_vLookAt	= D3DXVECTOR3( 1217.546f, 1637.938f, -969.744f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
#pragma endregion ( 마을, 번영의 길, 시계탑 광장_휴식처, 공존의 길, 평화의 길, 루렌시아 항구, 벨더->하멜 배 )

#pragma region 하멜 월드맵 카메라
	case SEnum::VMI_BATTLE_FIELD_HAMEL_SHIP_STAGE:
		{
			m_vEyePos	= D3DXVECTOR3( -3010.585f, 2945.908f, -5707.207f );
			m_vLookAt	= D3DXVECTOR3( -1897.977f, 1860.489f, -1246.024f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_HAMEL:
		{
			m_vEyePos	= D3DXVECTOR3( -94.806f, 4394.715f, -13069.134f );
			m_vLookAt	= D3DXVECTOR3( -94.803f, 3346.282f,  -1085.112f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_01:
		{
			m_vEyePos	= D3DXVECTOR3( 23763.000f, 7564.000f, -12396.000f );
			m_vLookAt	= D3DXVECTOR3( 23986.000f, 7284.000f,  -9206.000f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_02:
		{
			m_vEyePos	= D3DXVECTOR3( 38944.965f, 6832.954f, -13090.224f );
			m_vLookAt	= D3DXVECTOR3( 39195.453f, 6632.424f,  -7353.335f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00:
		{
			m_vEyePos	= D3DXVECTOR3( 53096.000f, 6560.000f, -8176.000f );
			m_vLookAt	= D3DXVECTOR3( 53096.000f, 6165.000f, -3670.000f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_03:
		{
			m_vEyePos	= D3DXVECTOR3( 69040.664f, 7634.834f, -16863.391f );
			m_vLookAt	= D3DXVECTOR3( 68569.945f, 6926.875f, -7880.147f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_04:
		{
			m_vEyePos	= D3DXVECTOR3( 68789.000f, 8063.000f, -16933.000f );
			m_vLookAt	= D3DXVECTOR3( 69005.000f, 6655.000f,  -4583.000f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
#pragma endregion ( 하멜->벨더 배, 마을, 엘라임의 물결, 휩쓸린 도시, 치유의 샘터_휴식처, 원형 수로, 노아호의 무덤 )

#pragma region 하멜 월드맵 카메라
#ifdef VILLAGE_SANDER
	case SEnum::VMI_SANDER:
		{
			m_vEyePos	= D3DXVECTOR3( 0.f, 5404.068f, -26794.80f );
			m_vLookAt	= D3DXVECTOR3( 0.f, 4012.577f, -2290.02f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
		case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_01:
		{
			m_vEyePos	= D3DXVECTOR3( -1909.341f,	3987.617f,	-15597.238f );
			m_vLookAt	= D3DXVECTOR3( -1909.341f,	2481.070f,	   175.192f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
		case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_02:
		{
			m_vEyePos	= D3DXVECTOR3( -58.73f,	9687.86f,	-15616.15f );
			m_vLookAt	= D3DXVECTOR3( -58.73f,	4539.44f,	-232.99f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
		case SEnum::VMI_BATTLE_FIELD_SANDER_REST_00: //윈드스톤 유적지
		{
			m_vEyePos	= D3DXVECTOR3( -864.318f,	24192.576f,	-67126.297f );
			m_vLookAt	= D3DXVECTOR3( -587.312f,	23635.068f,	-53583.059f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
#ifdef SANDER_DUNGEON_5_6
		case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_03: // 샌틸러스 호의 무덤
		{
			m_vEyePos	= D3DXVECTOR3( 2330.28f,	5955.543f,	-12782.739f );
			m_vLookAt	= D3DXVECTOR3( 2330.28f,	3469.856f,	103.025f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
		case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_04: // 베히모스 크레이터
		{
			m_vEyePos	= D3DXVECTOR3( -8960.431f,	4163.422f,	-10446.534f );
			m_vLookAt	= D3DXVECTOR3( -441.546f,	2070.409f,	-379.415f );
			m_fFov		= D3DXToRadian( 35.978f );
			m_fAspect	= 2.f;
		} break;
#endif //SANDER_DUNGEON_5_6
#endif //VILLAGE_SANDER
#pragma endregion ( 마을, 바람의 사원 )
	}

	CalcCameraMatrix();
}


void CX2FieldMapBase::CalcCameraMatrix()
{
	//m_vScreenShotSize = D3DXVECTOR2( 512, 512 );
	m_vScreenShotSize = D3DXVECTOR2( 512, 512 );

	D3DXVECTOR3 vUpVec(0, 1, 0 );
	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixIdentity( &m_matView );
	D3DXMatrixIdentity( &m_matProj );

	// FOV 값을 Horizontal FOV로 받았을 경우 Vertical FOV로 변환하는 공식
	// float fFov = float( 2.0 * atan( tan( m_fFov / 2.f ) / m_fAspect ) );

	D3DXMatrixLookAtLH( &m_matView, &m_vEyePos, &m_vLookAt, &vUpVec );
	D3DXMatrixPerspectiveFovLH( &m_matProj, m_fFov, m_fAspect, 0.f, 100000.f );
	
	m_ViewPort.X 		= 0;
	m_ViewPort.Y 		= 0;
	m_ViewPort.Width	= (DWORD) m_vScreenShotSize.x;
	m_ViewPort.Height	= (DWORD) m_vScreenShotSize.y;
	m_ViewPort.MinZ 	= 0.f;
	m_ViewPort.MaxZ 	= 1.f;
}

/// 월드맵 상 미니맵 관련
void CX2FieldMapBase::ResetMapPosAndSize( int iMapID )
{
	switch( iMapID )
	{
	//{{ 09.02.27 김태완 : 미니맵 크기 변경
	case SEnum::VMI_RUBEN:
	case SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01:
	
	case SEnum::VMI_ELDER:
	case SEnum::VMI_BATTLE_FIELD_ELDER_FIELD_01:
	case SEnum::VMI_BATTLE_FIELD_ELDER_REST_00:
	case SEnum::VMI_BATTLE_FIELD_ELDER_FIELD_02:
	
	case SEnum::VMI_BESMA:
	case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_01:
	case SEnum::VMI_BATTLE_FIELD_BESMA_REST_00:
	case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_02:
	case SEnum::VMI_BATTLE_FIELD_BESMA_FIELD_03:
	
	case SEnum::VMI_ALTERA:
	case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_01:
	case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_02:
	case SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00:
	case SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_03:

	case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_01:
	case SEnum::VMI_PEITA:
	case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_02:
	case SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_03:

	case SEnum::VMI_VELDER:
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_01:
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_02:
	case SEnum::VMI_BATTLE_FIELD_VELDER_REST_00:
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_03:
	case SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_04:
	case SEnum::VMI_BATTLE_FIELD_VELDER_SHIP_STAGE:

	case SEnum::VMI_BATTLE_FIELD_HAMEL_SHIP_STAGE:
	case SEnum::VMI_HAMEL:
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_01:
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_02:
	case SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00:
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_03:
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_04:
#ifdef VILLAGE_SANDER
	case SEnum::VMI_SANDER:
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_01:
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_02:
	case SEnum::VMI_BATTLE_FIELD_SANDER_REST_00: /// 윈드스톤 유적지
#ifdef SANDER_DUNGEON_5_6
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_03: // 샌틸러스 호의 무덤
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_04: // 베히모스 크레이터
#endif //SANDER_DUNGEON_5_6
#endif
		{
			m_vMiniMapWindowPos		= D3DXVECTOR2( 0, 0 );
			m_vMiniMapWindowSize	= D3DXVECTOR2( 1536, 768 );	/// 화면상의 크기

			m_vWindowTexturePos		= D3DXVECTOR2( 0.f, 0.f );
			m_vWindowTextureSize	= D3DXVECTOR2( 512, 512 );	/// 텍스쳐 사이즈
			
		} break;
	}
}
#include "StdAfx.h"
#include ".\x2npcunitviewerui.h"



CX2NPCUnitViewerUI::CX2NPCUnitViewerUI(void)
: m_pXSkinMesh( NULL )
, m_vLightPosition( 300.f, 300.f, -500.f )
{
	SetAlphaObject( false );

#ifdef SERV_PET_SYSTEM
	m_RenderParam.Init();
	m_fScale = 1.f;
	m_bForceBillBoard = false;

	m_pChangeTexXET = NULL;
#endif

}

CX2NPCUnitViewerUI::~CX2NPCUnitViewerUI(void)
{
	m_pXSkinAnim.reset();
	SAFE_CLOSE( m_pXSkinMesh );

#ifdef SERV_PET_SYSTEM
	SAFE_CLOSE( m_pChangeTexXET );
#endif
}


bool CX2NPCUnitViewerUI::ResetNPC( CX2UnitManager::NPC_UNIT_ID eNPCID )
{
	const CX2UnitManager::NPCUnitTemplet* pNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( eNPCID );
	if( NULL == pNPCTemplet )
		return false;


	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	if( false == g_pKTDXApp->LoadLuaTinker( pNPCTemplet->m_LuaFileName.c_str() ) )
	{
		ASSERT( !"NO" );
		return false;
	}
	
	if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, pNPCTemplet->m_LuaFileName.c_str() ) )
	{
		ASSERT( !"NO" );
		return false;
	}


	std::wstring motionFileName = L"";
	if( true == luaManager.BeginTable( "INIT_MOTION" ) )
	{
		LUA_GET_VALUE( luaManager, "MOTION_FILE_NAME", motionFileName, L"" );
		luaManager.EndTable(); // INIT_MOTION
	}

		
	if( true == motionFileName.empty() )
		return false;


	CKTDXDeviceXSkinMesh* pNewXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( motionFileName.c_str() );
	if( NULL == pNewXSkinMesh )
		return false;

	m_pXSkinAnim.reset();
	SAFE_CLOSE( m_pXSkinMesh );
	m_pXSkinMesh = pNewXSkinMesh;

	m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnimPtr();
	m_pXSkinAnim->SetAnimXSkinMesh( m_pXSkinMesh );
	m_pXSkinAnim->AddModelXSkinMesh( m_pXSkinMesh, m_pXSkinMesh->GetXET() );

	ResetRenderParam();

	return true;
}

#ifdef SERV_PET_SYSTEM
bool CX2NPCUnitViewerUI::ResetPet( int petId, char iLv )
{
	CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)petId );
	if( pTemplet == NULL )
		return false;

#ifdef PET_PREVIEW
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	//벡터 범위 초과했을 경우에 대한 예외처리 추가(크래쉬 방지)
	if( static_cast<char>(pTemplet->m_Evolution_Step_InitLuaTemplet.size()) <= iLv &&
		false == pTemplet->m_Evolution_Step_InitLuaTemplet.empty())
		iLv = 0;

#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	//벡터 범위 초과했을 경우에 대한 예외처리 추가(크래쉬 방지)
	if( static_cast<char>(pTemplet->m_Evolution_Step_ScriptName.size()) <= iLv &&
		false == pTemplet->m_Evolution_Step_ScriptName.empty())
		iLv = 0;

#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

#endif //PET_PREVIEW

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	KLuaManager* pLuaManager = pTemplet->m_Evolution_Step_InitLuaTemplet[iLv].m_pLuaManager;
	if ( pLuaManager == NULL )
	{
		ASSERT( !"NO" );
		return false;
	}
	KLuaManager	luaManager( pLuaManager->GetLuaState(), 0, false );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	if( false == g_pKTDXApp->LoadLuaTinker( pTemplet->m_Evolution_Step_ScriptName[iLv].c_str() ) )
	{
		ASSERT( !"NO" );
		return false;
	}
	if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, pTemplet->m_Evolution_Step_ScriptName[iLv].c_str() ) )
	{
		ASSERT( !"NO" );
		return false;
	}
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	m_RenderParam.Init();
	m_bForceBillBoard = false;

	if( true == luaManager.BeginTable( "INIT_SYSTEM" ) )
	{
		LUA_GET_VALUE( luaManager, "UNIT_SCALE", m_fScale, 1.f );		

		LUA_GET_VALUE_ENUM( luaManager, "RENDER_PARAM", m_RenderParam.renderType, CKTDGXRenderer::RENDER_TYPE, CKTDGXRenderer::RT_CARTOON_BLACK_EDGE );
#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
		m_RenderParam.fOutLineWide	= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
		m_RenderParam.fOutLineWide	= 1.7f;
#endif //UNIT_SCALE_COMBINE_ONE

		LUA_GET_VALUE( luaManager, "ALPHA_BLEND", m_RenderParam.bAlphaBlend, false );
		if( m_RenderParam.bAlphaBlend == true )
		{
			SetAlphaObject( true );
		}
		bool bCullMode;
		LUA_GET_VALUE( luaManager, "CULL_MODE", bCullMode, true );
		if( bCullMode == false )
		{
			m_RenderParam.cullMode = D3DCULL_NONE;
		}
		LUA_GET_VALUE( luaManager, "Z_ENABLE", m_RenderParam.bZEnable, true );
		m_RenderParam.bZWriteEnable = m_RenderParam.bZEnable;			// fix!! 일단 요렇게

		LUA_GET_VALUE( luaManager, "FORCE_BILLBOARD", m_bForceBillBoard, false );

		luaManager.EndTable(); // INIT_MOTION
	}
	
	std::wstring motionFileName = L"";
	std::wstring wstrChangeTex = L"";
	if( true == luaManager.BeginTable( "INIT_MOTION" ) )
	{
		LUA_GET_VALUE( luaManager, "MOTION_FILE_NAME", motionFileName, L"" );
		LUA_GET_VALUE( luaManager, "MOTION_CHANGE_TEX_XET", wstrChangeTex, L"" );		
		luaManager.EndTable(); // INIT_MOTION
	}

	SAFE_CLOSE( m_pChangeTexXET );
	CKTDXDeviceXET* m_pChangeTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( wstrChangeTex.c_str() );

	if( true == motionFileName.empty() )
		return false;


	CKTDXDeviceXSkinMesh* pNewXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( motionFileName.c_str() );
	if( NULL == pNewXSkinMesh )
		return false;

	m_pXSkinAnim.reset();
	SAFE_CLOSE( m_pXSkinMesh );
	m_pXSkinMesh = pNewXSkinMesh;

	m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnimPtr();
	m_pXSkinAnim->SetAnimXSkinMesh( m_pXSkinMesh );
	m_pXSkinAnim->AddModelXSkinMesh( m_pXSkinMesh, m_pXSkinMesh->GetXET(), NULL, m_pChangeTexXET );

	ResetRenderParam();

	return true;
}
#endif







/*virtual*/ HRESULT CX2NPCUnitViewerUI::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( m_pXSkinAnim != NULL )
	{
		if( true == IsManagedByDGManager() )
		{
			m_pXSkinAnim->GetMatrix().Move( GetMatrix().GetPos() );
		}

#ifdef SERV_PET_SYSTEM
		if( m_bForceBillBoard == true )
		{
			m_pXSkinAnim->SetBillBoardType( CKTDGMatrix::BT_ALL );
		}
		else
		{
			m_pXSkinAnim->GetMatrix().Rotate( GetMatrix().GetRotate() );
		}
#else
		m_pXSkinAnim->GetMatrix().Rotate( GetMatrix().GetRotate() );
#endif
		m_pXSkinAnim->GetMatrix().Scale( GetMatrix().GetScale() );

		m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );
	}


	return S_OK;
}




void CX2NPCUnitViewerUI::ResetRenderParam()
{
	CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinAnim->GetRenderParam();
	pRenderParam->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
	pRenderParam->outLineColor		= 0xffffffff;
	pRenderParam->lightPos			= m_vLightPosition;
#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
	pRenderParam->fOutLineWide		= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
	pRenderParam->fOutLineWide		= 1.5f;
#endif //UNIT_SCALE_COMBINE_ONE
	pRenderParam->bAlphaBlend		= false;

#ifdef SERV_PET_SYSTEM
	pRenderParam->renderType	= m_RenderParam.renderType;
	pRenderParam->fOutLineWide	= m_RenderParam.fOutLineWide;	
	pRenderParam->bAlphaBlend	= m_RenderParam.bAlphaBlend;
	pRenderParam->cullMode		= m_RenderParam.cullMode;
	pRenderParam->bZEnable		= m_RenderParam.bZEnable;
#endif
}



/*virtual*/ RENDER_HINT CX2NPCUnitViewerUI::OnFrameRender_Prepare()
{
	KTDXPROFILE();

	if ( m_pXSkinAnim == NULL )
		return RENDER_HINT_NORENDER;

	m_pXSkinAnim->SetInstantShow( false );
	if ( !m_pXSkinAnim->GetShowObject() )
		return RENDER_HINT_NORENDER;

	RENDER_HINT renderHintResult = m_pXSkinAnim->OnFrameRender_Prepare();

	if( RENDER_HINT::NORENDER != renderHintResult.m_eHint )
		m_pXSkinAnim->SetInstantShow( true );

	return renderHintResult;
}//CX2UnitViewerUI::OnFrameRender_Prepare()



/*virtual*/ void CX2NPCUnitViewerUI::OnFrameRender_Draw()
{
	KTDXPROFILE();

	if( m_pXSkinAnim == NULL )
		return; 

	if( false == m_pXSkinAnim->GetShowObject() )
		return;

	if( false == m_pXSkinAnim->IsInstantShow() )
		return; 


	m_pXSkinAnim->OnFrameRender_Draw();
}


HRESULT CX2NPCUnitViewerUI::OnResetDevice()
{
	if( m_pXSkinAnim != NULL )
	{
		m_pXSkinAnim->OnResetDevice();
	}

	return S_OK;
}

HRESULT CX2NPCUnitViewerUI::OnLostDevice()
{
	if( m_pXSkinAnim != NULL )
	{
		m_pXSkinAnim->OnLostDevice();
	}

	return S_OK;
}



void CX2NPCUnitViewerUI::PlayAnim( const WCHAR* pName, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType /*= CKTDGXSkinAnim::XAP_LOOP*/, bool bTransition /*= false */ )
{
	if ( m_pXSkinAnim == NULL )
		return;

	if( NULL != pName )
	{
		m_pXSkinAnim->ChangeAnim( pName, bTransition );

	}
	else
	{
		m_pXSkinAnim->ChangeAnim( 0, bTransition );
	}
	m_pXSkinAnim->Play( playType );
}


void CX2NPCUnitViewerUI::SetPosition( const D3DXVECTOR3& vPosition )
{
	GetMatrix().Move( vPosition );
	if( m_pXSkinAnim != NULL )
	{
		m_pXSkinAnim->GetMatrix().Move( GetMatrix().GetPos() );
	}
}


void CX2NPCUnitViewerUI::SetScale( const D3DXVECTOR3& vScale )
{	
	GetMatrix().Scale( vScale );
	if( m_pXSkinAnim != NULL )
	{
		m_pXSkinAnim->GetMatrix().Scale( GetMatrix().GetScale() );
	}
}

#ifdef RIDING_SYSTEM
bool CX2NPCUnitViewerUI::ResetRidingPet( int RidingPetId )
{
	CX2RidingPetManager::RidingPetTemplet *pTemplet = CX2RidingPetManager::GetInstance()->GetRidingPetTemplet( static_cast<CX2RidingPetManager::RIDING_PET_UNIT_ID>(RidingPetId) );
	if( pTemplet == NULL )
		return false;


//#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	KLuaManager* pLuaManager = pTemplet->m_InitLuaTemplet.m_pLuaManager;
	if ( pLuaManager == NULL )
	{
		ASSERT( !"NO" );
		return false;
	}
	KLuaManager	luaManager( pLuaManager->GetLuaState(), 0, false );
//#endif	//X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	m_RenderParam.Init();
	m_bForceBillBoard = false;

	if( true == luaManager.BeginTable( "INIT_SYSTEM" ) )
	{
		LUA_GET_VALUE( luaManager, "UNIT_SCALE", m_fScale, 1.f );		

		LUA_GET_VALUE_ENUM( luaManager, "RENDER_PARAM", m_RenderParam.renderType, CKTDGXRenderer::RENDER_TYPE, CKTDGXRenderer::RT_CARTOON_BLACK_EDGE );
#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
		m_RenderParam.fOutLineWide	= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
		m_RenderParam.fOutLineWide	= 1.7f;
#endif //UNIT_SCALE_COMBINE_ONE

		LUA_GET_VALUE( luaManager, "ALPHA_BLEND", m_RenderParam.bAlphaBlend, false );
		if( m_RenderParam.bAlphaBlend == true )
		{
			SetAlphaObject( true );
		}
		bool bCullMode;
		LUA_GET_VALUE( luaManager, "CULL_MODE", bCullMode, true );
		if( bCullMode == false )
		{
			m_RenderParam.cullMode = D3DCULL_NONE;
		}
		LUA_GET_VALUE( luaManager, "Z_ENABLE", m_RenderParam.bZEnable, true );
		m_RenderParam.bZWriteEnable = m_RenderParam.bZEnable;			// fix!! 일단 요렇게

		LUA_GET_VALUE( luaManager, "FORCE_BILLBOARD", m_bForceBillBoard, false );

		luaManager.EndTable(); // INIT_MOTION
	}

	std::wstring motionFileName = L"";
	std::wstring wstrChangeTex = L"";
	if( true == luaManager.BeginTable( "INIT_MOTION" ) )
	{
		LUA_GET_VALUE( luaManager, "MOTION_FILE_NAME", motionFileName, L"" );
		LUA_GET_VALUE( luaManager, "MOTION_CHANGE_TEX_XET", wstrChangeTex, L"" );		
		luaManager.EndTable(); // INIT_MOTION
	}

	SAFE_CLOSE( m_pChangeTexXET );
	CKTDXDeviceXET* m_pChangeTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( wstrChangeTex.c_str() );

	if( true == motionFileName.empty() )
		return false;


	CKTDXDeviceXSkinMesh* pNewXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( motionFileName.c_str() );
	if( NULL == pNewXSkinMesh )
		return false;

	m_pXSkinAnim.reset();
	SAFE_CLOSE( m_pXSkinMesh );
	m_pXSkinMesh = pNewXSkinMesh;

	m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnimPtr();
	m_pXSkinAnim->SetAnimXSkinMesh( m_pXSkinMesh );
	m_pXSkinAnim->AddModelXSkinMesh( m_pXSkinMesh, m_pXSkinMesh->GetXET(), NULL, m_pChangeTexXET );

	ResetRenderParam();

	return true;
}
#endif //RIDING_SYSTEM
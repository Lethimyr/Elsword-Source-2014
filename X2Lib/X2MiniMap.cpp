#include "StdAfx.h"
#include ".\x2minimap.h"

CX2MiniMap::CX2MiniMap( CX2MiniMapUI* pMiniMapUI )
{
	CKTDGObject::SetLayer( XL_MINIMAP ); 
	CKTDGObject::SetAlphaObject( true );

	m_pMiniMapUI = pMiniMapUI;

	m_pFont = NULL;
	m_pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_11_NORMAL );

	m_vMiniMapWindowPos		= D3DXVECTOR2( 0, 0 );
	//m_vMiniMapWindowSize	= D3DXVECTOR2( 248, 105 );
	m_vMiniMapWindowSize	= D3DXVECTOR2( 248, 105 );
	
	m_fEyeDistance			= 2000.f;

	m_fRenderTargetTextureScale		= 1.0f;
	const int iRenderTargetWidth	= (int)(m_vMiniMapWindowSize.x*m_fRenderTargetTextureScale);
	const int iRenderTargetHeight	= (int)(m_vMiniMapWindowSize.y*m_fRenderTargetTextureScale);

	m_pRenderTargetTexture	= g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"MiniMapTex", iRenderTargetWidth, iRenderTargetHeight, D3DFMT_A8R8G8B8 );
	m_pTextureMiniMap		= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"HQ_MiniMap.tga" );

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

//	m_pTextureMonsterCount	= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_Monster_Num.tga" );

	// 09.03.11 김태완 : 미니맵 최소화 기능 제거
	//m_bMinimized	= false;
	m_pStage = g_pMain->GetNowState();
}

CX2MiniMap::~CX2MiniMap(void)
{
	SAFE_CLOSE( m_pRenderTargetTexture );
	SAFE_CLOSE( m_pTextureMiniMap );

	for( int i=0; i<ARRAY_SIZE(m_pTextureIcon); i++ )
	{
		SAFE_CLOSE( m_pTextureIcon[i] );
	}

	g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, false );	
}


//{{ robobeg : 2008-10-28
//HRESULT CX2MiniMap::OnFrameRender()
/*virtual*/
void    CX2MiniMap::OnFrameRender_Draw()
//}} robobeg : 2008-10-28
{

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager() != NULL && g_pKTDXApp->GetDGManager()->GetDialogManager() != NULL )
	{
		if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() )
		{
			return;
		}
	}
#endif
	if( false == m_pMiniMapUI->GetShow() )
		return;

	switch( g_pMain->GetNowStateID() )
	{
	//case CX2Main::XS_PVP_GAME:
	//case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			RenderX2Game();
		} break;

	case CX2Main::XS_SQUARE_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			RenderSquare();
		} break;

	case CX2Main::XS_VILLAGE_MAP:
		{
			RenderVillage();
		}
	}

//	return S_OK;
}

void CX2MiniMap::NomalDirectCamera( CX2GameUnit* pFocusUnit, float distance, float height, float angleDegree, float eyeDistance, float lookatDistance )
{
	KTDXPROFILE();

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;
	const float fLandHeight = g_pX2Game->GetX2Camera()->GetLandHeight();

	lookAt = pFocusUnit->GetPos();
	lookAt.y += 100.0f;
	
	if( pFocusUnit->GetIsRight() == true )
	{
		lookAt	+=	(pFocusUnit->GetDirVector() * lookatDistance);
		eye.x	=	pFocusUnit->GetXPos() + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
		eye.y	=	pFocusUnit->GetYPos() + height; 
		eye.z	=	pFocusUnit->GetZPos() + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + angleDegree) ) * -distance;
		eye		+=	(pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		lookAt	+=	(pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x	=	pFocusUnit->GetXPos() + sinf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
		eye.y	=	pFocusUnit->GetYPos() + height; 
		eye.z	=	pFocusUnit->GetZPos() + cosf( D3DXToRadian(pFocusUnit->GetYRotateDegree() + 180.0f - angleDegree) ) * -distance;
		eye		+=	(pFocusUnit->GetDirVector() * -eyeDistance);
	}
	if( eye.y < fLandHeight + 400.0f )
		eye.y = fLandHeight + 400.0f;
	if( lookAt.y < fLandHeight + 400.0f )
		lookAt.y = fLandHeight + 400.0f;	

	const D3DXVECTOR3 vUpVec = D3DXVECTOR3( 0, 1, 0 );
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &eye, &lookAt, &vUpVec );	
	g_pKTDXApp->SetViewTransform( &matView );
}

void CX2MiniMap::RenderMiniMapTexture()
{
	KTDXPROFILE();

	if( NULL == m_pRenderTargetTexture )
		return;

	if( NULL == g_pX2Game )
		return;

	if( NULL == g_pX2Game->GetMyUnit() )
		return;
	
	KTDXPROFILE_BEGIN( "CLEAR" );
	m_pRenderTargetTexture->BeginRender( true );
	m_pRenderTargetTexture->Clear( D3DCOLOR_RGBA( 0, 0, 0, 0 ) );
	KTDXPROFILE_END();
	
	// 카메라 위치 설정
	D3DXMATRIX matViewOld;	
	g_pKTDXApp->GetViewTransform( &matViewOld );
	NomalDirectCamera( g_pX2Game->GetMyUnit(), m_fEyeDistance );

	
	KTDXPROFILE_BEGIN( "DRAW_MAP" );
	// 맵 그리기
	if( NULL != g_pX2Game &&
		NULL != g_pX2Game->GetWorld() && 
		NULL != g_pX2Game->GetWorld()->GetLineMap() )
	{	
		g_pX2Game->GetWorld()->GetLineMap()->OnFrameRender();
	}
	KTDXPROFILE_END();

	RenderGameUnitTexture();

	KTDXPROFILE_BEGIN( "END_RENDER" );
	g_pKTDXApp->SetViewTransform( &matViewOld );
	m_pRenderTargetTexture->EndRender();    
	KTDXPROFILE_END();
}

HRESULT CX2MiniMap::RenderX2Game()
{
	KTDXPROFILE();

	if( NULL == m_pRenderTargetTexture )
		return E_FAIL;

	if( false == m_pRenderTargetTexture->IsOk() )
		return E_FAIL;
		
	if( NULL == m_pTextureMiniMap )
		return E_FAIL;

	//{{ 허상형 : [2011/3/14/] //	월드미션
#ifdef SERV_INSERT_GLOBAL_SERVER
	if( m_pMiniMapUI != NULL && m_pMiniMapUI->IsOpenDefenceMinimap() == true )
	{
		g_pData->GetWorldMissionManager()->RenderMiniMapTexture( m_pRenderTargetTexture );
	}
	else
	{
		RenderMiniMapTexture();
	}
#else SERV_INSERT_GLOBAL_SERVER
	//}} 허상형 : [2011/3/14/] //	월드미션
	RenderMiniMapTexture();
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 허상형 : [2011/3/14/] //	월드미션

	m_pRenderTargetTexture->Draw( (int)m_vMiniMapWindowPos.x + 824, (int)m_vMiniMapWindowPos.y + 558, (int)m_vMiniMapWindowSize.x, (int)m_vMiniMapWindowSize.y );

	// 전체 프레임을 통째로 그리고 
	m_pTextureMiniMap->Draw( m_vMiniMapWindowPos.x + 824, m_vMiniMapWindowPos.y + 558, m_vMiniMapWindowSize.x, m_vMiniMapWindowSize.y, 1, 1, 255, 129 );

	if( NULL != g_pX2Game && g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
		if( NULL != pDungeonGame )
		{
			CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();
			// 몬스터수
			if( NULL != pDungeon && NULL != pDungeon->GetNowStage() )
			{

				const map< std::pair<int,int>, CX2Dungeon::DungeonMapData >& mapDungeonMapData = pDungeon->GetMapDungeonMapData();
				map< std::pair<int,int>, CX2Dungeon::DungeonMapData >::const_iterator it;
				it = mapDungeonMapData.find( std::make_pair( pDungeon->GetNowStageIndex(), pDungeon->GetNowStage()->GetNowSubStageIndex() ) );
				if( it != mapDungeonMapData.end() )
				{
					const CX2Dungeon::DungeonMapData& dungeonMapData = it->second;
					if( false == dungeonMapData.m_bIsBossStage )
						m_pMiniMapUI->SetMonsterNum(true, pDungeonGame->GetAliveActiveNPCCount());
					else
						m_pMiniMapUI->SetMonsterNum(false);
				}
				else
					m_pMiniMapUI->SetMonsterNum(false);
			}
		}
	}
	if( g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD )
		m_pMiniMapUI->SetMonsterNum( false );

	return S_OK;
}

void CX2MiniMap::NomalDirectCameraSquare( CX2SquareUnit* pFocusUnit, float distance, float height, float angleDegree, float eyeDistance, float lookatDistance )
{
	KTDXPROFILE();

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;
	float fLandHeight = 0.f;
	
	if(g_pTFieldGame != NULL)
		fLandHeight = g_pTFieldGame->GetCamera()->GetLandHeight();
	else if(g_pSquareGame != NULL)
		fLandHeight = g_pSquareGame->GetCamera()->GetLandHeight();

	D3DXVECTOR3 vUnitPos = pFocusUnit->GetPos();

	lookAt = pFocusUnit->GetPos();
	lookAt.y += 100.0f;

	if( pFocusUnit->GetIsRight() == true )
	{
		lookAt	+=	(pFocusUnit->GetDirVector() * lookatDistance);
		eye.x	=	vUnitPos.x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye.y	=	vUnitPos.y + height; 
		eye.z	=	vUnitPos.z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye		+=	(pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		lookAt	+=	(pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x	=	vUnitPos.x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye.y	=	vUnitPos.y + height; 
		eye.z	=	vUnitPos.z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye		+=	(pFocusUnit->GetDirVector() * -eyeDistance);
	}
	if( eye.y < fLandHeight + 400.0f )
		eye.y = fLandHeight + 400.0f;
	if( lookAt.y < fLandHeight + 400.0f )
		lookAt.y = fLandHeight + 400.0f;	

	const D3DXVECTOR3 vUpVec = D3DXVECTOR3( 0, 1, 0 );
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &eye, &lookAt, &vUpVec );	
	g_pKTDXApp->SetViewTransform( &matView );
}

void CX2MiniMap::RenderMiniMapTextureSquare()
{
	KTDXPROFILE();

	if( NULL == m_pRenderTargetTexture )
		return;

	if( NULL == g_pSquareGame && NULL == g_pTFieldGame )
		return;

	if( g_pSquareGame != NULL && NULL == g_pSquareGame->GetMyUnit() && 
		g_pTFieldGame != NULL && NULL == g_pTFieldGame->GetMyUnit() )
		return;

	KTDXPROFILE_BEGIN( "CLEAR" );
	m_pRenderTargetTexture->BeginRender( true );
	m_pRenderTargetTexture->Clear( D3DCOLOR_RGBA( 0, 0, 0, 0 ) );
	KTDXPROFILE_END();

	// 카메라 위치 설정
	D3DXMATRIX matViewOld;	
	g_pKTDXApp->GetViewTransform( &matViewOld );

	CX2SquareUnit *pUnit = NULL;
	if(g_pTFieldGame != NULL && g_pTFieldGame->GetMyUnit() != NULL)
		pUnit = g_pTFieldGame->GetMyUnit();
	else if(g_pSquareGame != NULL && g_pSquareGame->GetMyUnit() != NULL)
		pUnit = g_pSquareGame->GetMyUnit();

	if( pUnit != NULL )
		NomalDirectCameraSquare( pUnit, m_fEyeDistance );	


	KTDXPROFILE_BEGIN( "DRAW_MAP" );
	// 맵 그리기
	
	if( NULL != g_pSquareGame &&
		NULL != g_pSquareGame->GetWorld() && 
		NULL != g_pSquareGame->GetWorld()->GetLineMap() )
	{	
		g_pSquareGame->GetWorld()->GetLineMap()->OnFrameRender();
	}
	if( g_pTFieldGame != NULL &&
		g_pTFieldGame->GetWorld() &&
		g_pTFieldGame->GetWorld()->GetLineMap() )
	{
		g_pTFieldGame->GetWorld()->GetLineMap()->OnFrameRender();
	}

	KTDXPROFILE_END();

	D3DVIEWPORT vp;
	D3DXMATRIX matWorld, matView, matProj;

	KTDXPROFILE_BEGIN( "SET_MATRIX" );
	vp.X = 0;
	vp.Y = 0;
	vp.Width  = (DWORD)( (float)m_pRenderTargetTexture->GetWidth()  / g_pKTDXApp->GetResolutionScaleX() );
	vp.Height = (DWORD)( (float)m_pRenderTargetTexture->GetHeight() / g_pKTDXApp->GetResolutionScaleY() );

	g_pKTDXApp->GetProjectionTransform( &matProj );
	g_pKTDXApp->GetViewTransform( &matView );
	D3DXMatrixIdentity( &matWorld );

	KTDXPROFILE_END();


	const float MAGIC_MY_UNIT_SIZE		= 22.f 		/ g_pKTDXApp->GetResolutionScaleX() 	* 1500.f / m_fEyeDistance * m_fRenderTargetTextureScale;
	const float MAGIC_ALLY_UNIT_SIZE	= 12.f		/ g_pKTDXApp->GetResolutionScaleX() 	* 1500.f / m_fEyeDistance * m_fRenderTargetTextureScale;
	const float MAGIC_ENEMY_UNIT_SIZE	= 12.f		/ g_pKTDXApp->GetResolutionScaleX() 	* 1500.f / m_fEyeDistance * m_fRenderTargetTextureScale;
	const float MAGIC_GATE_WIDTH		= 9.f		/ g_pKTDXApp->GetResolutionScaleX() 	* 1500.f / m_fEyeDistance * m_fRenderTargetTextureScale;
	const float MAGIC_GATE_HEIGHT		= 18.f		/ g_pKTDXApp->GetResolutionScaleY() 	* 1500.f / m_fEyeDistance * m_fRenderTargetTextureScale;


	KTDXPROFILE_BEGIN( "DRAW_USER_UNIT" );
	// 유닛 위치 그리기
	CX2SquareUnit* pMyUnit = NULL;
	int maxUnit = 0;

	if(g_pTFieldGame != NULL)
	{
		pMyUnit = g_pTFieldGame->GetMyUnit();
		maxUnit = (int)g_pTFieldGame->GetSquareUnitNum();
	}
	else if(g_pSquareGame != NULL)
	{
		pMyUnit = g_pSquareGame->GetMyUnit();
		maxUnit = (int)g_pSquareGame->GetSquareUnitNum();
	}

	

	for( int i = 0; i < maxUnit; i++ )
	{
		CX2SquareUnit* pCX2SquareUnit = NULL;
		
		if(g_pTFieldGame != NULL)
		{
			pCX2SquareUnit = g_pTFieldGame->GetSquareUnit(i);
		}
		else if(g_pSquareGame != NULL)
		{
			pCX2SquareUnit = g_pSquareGame->GetSquareUnit(i);
		}

		
		if( NULL != pCX2SquareUnit )
		{
			D3DXVECTOR3 vPos = pCX2SquareUnit->GetPos();
			D3DXVec3Project( &vPos, &vPos, &vp, &matProj, &matView, &matWorld );

			if( pCX2SquareUnit == pMyUnit )
			{
				m_pTextureMiniMap->Draw( vPos.x-MAGIC_MY_UNIT_SIZE*0.5f, vPos.y-MAGIC_MY_UNIT_SIZE, 
							MAGIC_MY_UNIT_SIZE, MAGIC_MY_UNIT_SIZE, 1, 131, 24, 22 );
			}
			else
			{
				m_pTextureMiniMap->Draw( vPos.x-MAGIC_ALLY_UNIT_SIZE*0.5f, vPos.y-MAGIC_ALLY_UNIT_SIZE, 
							MAGIC_ALLY_UNIT_SIZE, MAGIC_ALLY_UNIT_SIZE, 28, 131, 12, 12 );
			}
		}
	}
	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "END_RENDER" );
	g_pKTDXApp->SetViewTransform( &matViewOld );
	m_pRenderTargetTexture->EndRender();    
	KTDXPROFILE_END();
}

HRESULT CX2MiniMap::RenderSquare()
{
	KTDXPROFILE();

	if( NULL == m_pRenderTargetTexture )
		return E_FAIL;

	if( false == m_pRenderTargetTexture->IsOk() )
		return E_FAIL;

	if( NULL == m_pTextureMiniMap )
		return E_FAIL;

	const int MAGIC_MINIMIZED_BAR_HEIGHT = 20;


	RenderMiniMapTextureSquare();

	m_pRenderTargetTexture->Draw( (int)m_vMiniMapWindowPos.x + 824, (int)m_vMiniMapWindowPos.y + 558, (int)m_vMiniMapWindowSize.x, (int)m_vMiniMapWindowSize.y );
	
	// 전체 프레임을 통째로 그리고 
	m_pTextureMiniMap->Draw( m_vMiniMapWindowPos.x + 824, m_vMiniMapWindowPos.y + 558, m_vMiniMapWindowSize.x, m_vMiniMapWindowSize.y, 1, 1, 255, 129 );

	return S_OK;
}

void CX2MiniMap::NomalDirectCameraVillage( CX2SquareUnit* pFocusUnit, float distance, float height, float angleDegree, float eyeDistance, float lookatDistance )
{
	KTDXPROFILE();

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;
	float fLandHeight = 0.f;

	if(g_pTFieldGame != NULL)
		fLandHeight = g_pTFieldGame->GetCamera()->GetLandHeight();
	else if(g_pSquareGame != NULL)
		fLandHeight = g_pSquareGame->GetCamera()->GetLandHeight();

	D3DXVECTOR3 vUnitPos = pFocusUnit->GetPos();

	lookAt = pFocusUnit->GetPos();
	lookAt.y += 100.0f;

	if( pFocusUnit->GetIsRight() == true )
	{
		lookAt	+=	(pFocusUnit->GetDirVector() * lookatDistance);
		eye.x	=	vUnitPos.x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye.y	=	vUnitPos.y + height; 
		eye.z	=	vUnitPos.z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + angleDegree) ) * -distance;
		eye		+=	(pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		lookAt	+=	(pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x	=	vUnitPos.x + sinf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye.y	=	vUnitPos.y + height; 
		eye.z	=	vUnitPos.z + cosf( D3DXToRadian(pFocusUnit->GetDirDegree().y + 180.0f - angleDegree) ) * -distance;
		eye		+=	(pFocusUnit->GetDirVector() * -eyeDistance);
	}
	if( eye.y < fLandHeight + 400.0f )
		eye.y = fLandHeight + 400.0f;
	if( lookAt.y < fLandHeight + 400.0f )
		lookAt.y = fLandHeight + 400.0f;	

	const D3DXVECTOR3 vUpVec = D3DXVECTOR3( 0, 1, 0 );
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &eye, &lookAt, &vUpVec );	
	g_pKTDXApp->SetViewTransform( &matView );
}

void CX2MiniMap::RenderMiniMapTextureVillage()
{
	KTDXPROFILE();

	if( NULL == m_pRenderTargetTexture )
		return;

	if( NULL == g_pTFieldGame )
		return;

	if( g_pTFieldGame != NULL && NULL == g_pTFieldGame->GetMyUnit() )
		return;

	KTDXPROFILE_BEGIN( "CLEAR" );
	m_pRenderTargetTexture->BeginRender( true );
	m_pRenderTargetTexture->Clear( D3DCOLOR_RGBA( 0, 0, 0, 0 ) );
	KTDXPROFILE_END();

	// 카메라 위치 설정
	D3DXMATRIX matViewOld;	
	g_pKTDXApp->GetViewTransform( &matViewOld );

	CX2SquareUnit *pUnit = NULL;
	if(g_pTFieldGame != NULL && g_pTFieldGame->GetMyUnit() != NULL)
		pUnit = g_pTFieldGame->GetMyUnit();
	
	NomalDirectCameraVillage( pUnit, m_fEyeDistance );	


	KTDXPROFILE_BEGIN( "DRAW_MAP" );
	// 맵 그리기

	if( g_pTFieldGame != NULL &&
		g_pTFieldGame->GetWorld() &&
		g_pTFieldGame->GetWorld()->GetLineMap() )
	{
		g_pTFieldGame->GetWorld()->GetLineMap()->OnFrameRender();
	}

	KTDXPROFILE_END();

	D3DVIEWPORT vp;
	D3DXMATRIX matWorld, matView, matProj;

	KTDXPROFILE_BEGIN( "SET_MATRIX" );
	vp.X = 0;
	vp.Y = 0;
	vp.Width  = (DWORD)( (float)m_pRenderTargetTexture->GetWidth()  / g_pKTDXApp->GetResolutionScaleX() );
	vp.Height = (DWORD)( (float)m_pRenderTargetTexture->GetHeight() / g_pKTDXApp->GetResolutionScaleY() );

	g_pKTDXApp->GetProjectionTransform( &matProj );
	g_pKTDXApp->GetViewTransform( &matView );
	D3DXMatrixIdentity( &matWorld );

	KTDXPROFILE_END();
	
	const int MAGIC_SMALL_ICON_SIZE_X = static_cast<int>( 22 * m_fRenderTargetTextureScale );	
	const int MAGIC_SMALL_ICON_SIZE_Y = static_cast<int>( 22 * m_fRenderTargetTextureScale );
	const int MAGIC_ICON_SIZE_X = static_cast<int>( 36 * m_fRenderTargetTextureScale );
	const int MAGIC_ICON_SIZE_Y = static_cast<int>( 36 * m_fRenderTargetTextureScale );
	const int MAGIC_BIG_ICON_OFFSET_Y = 8;

	/*const int MAGIC_SMALL_ICON_SIZE_X = static_cast<int>( 22.f / g_pKTDXApp->GetResolutionScaleX() * 1500.f / m_fEyeDistance * m_fRenderTargetTextureScale );	
	const int MAGIC_SMALL_ICON_SIZE_Y = static_cast<int>( 22.f / g_pKTDXApp->GetResolutionScaleX() * 1500.f / m_fEyeDistance * m_fRenderTargetTextureScale );
	const int MAGIC_ICON_SIZE_X = static_cast<int>( 36.f / g_pKTDXApp->GetResolutionScaleX() * 1500.f / m_fEyeDistance * m_fRenderTargetTextureScale );
	const int MAGIC_ICON_SIZE_Y = static_cast<int>( 36.f / g_pKTDXApp->GetResolutionScaleX() * 1500.f / m_fEyeDistance * m_fRenderTargetTextureScale );
	const int MAGIC_BIG_ICON_OFFSET_Y = 8;*/
	
	KTDXPROFILE_BEGIN( "DRAW_GATE" );
	for( int i = 0; i < g_pTFieldGame->GetGatePortalCount(); ++i )
	{
		const D3DXVECTOR3 vPos = g_pTFieldGame->GetGatePortalPos( i );
		D3DXVECTOR3 vProjectedPos;

		D3DXVec3Project( &vProjectedPos, &vPos, &vp, &matProj, &matView, &matWorld );

		if ( m_pTextureIcon[MI_GATE] != NULL )
			m_pTextureIcon[MI_GATE]->Draw( static_cast<int>(vProjectedPos.x) - MAGIC_SMALL_ICON_SIZE_X, 
										   static_cast<int>(vProjectedPos.y) - MAGIC_SMALL_ICON_SIZE_Y,
										   MAGIC_SMALL_ICON_SIZE_X, MAGIC_SMALL_ICON_SIZE_Y );
	}
	KTDXPROFILE_END();


	KTDXPROFILE_BEGIN( "DRAW_MARKET" );
	for( int i = 0; i < g_pTFieldGame->GetMarketEnteranceCount(); ++i )
	{
		const D3DXVECTOR3 vPos = g_pTFieldGame->GetMarketEnterancePos( i );
		D3DXVECTOR3 vProjectedPos;

		D3DXVec3Project( &vProjectedPos, &vPos, &vp, &matProj, &matView, &matWorld );

		if ( m_pTextureIcon[MI_MARKET_PLACE] != NULL )
			m_pTextureIcon[MI_MARKET_PLACE]->Draw( static_cast<int>(vProjectedPos.x) - MAGIC_ICON_SIZE_X,
												   static_cast<int>(vProjectedPos.y) - MAGIC_ICON_SIZE_Y + MAGIC_BIG_ICON_OFFSET_Y,
												   MAGIC_ICON_SIZE_X, MAGIC_ICON_SIZE_Y );
	}
	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "DRAW_NPC" );
	const float MAGIC_NPC_UNIT_SIZE	= 36.f / g_pKTDXApp->GetResolutionScaleX() * 1500.f / m_fEyeDistance * m_fRenderTargetTextureScale;

	for( int i=0; i<g_pTFieldGame->GetFieldNPCCount(); i++ )
	{
		CX2TFieldNpc* pFieldNPC = g_pTFieldGame->GetFieldNPC( i );
		if( NULL == pFieldNPC )
			continue;

		const D3DXVECTOR3 vPos = pFieldNPC->GetPos();
		D3DXVECTOR3 vProjectedPos;

		D3DXVec3Project( &vProjectedPos, &vPos, &vp, &matProj, &matView, &matWorld );

		switch( pFieldNPC->GetHouseType() )
		{
		case CX2TFieldNpc::NT_EQUIP:
			{
				m_pTextureMiniMap->Draw( vProjectedPos.x - MAGIC_NPC_UNIT_SIZE * 0.5f, vProjectedPos.y - MAGIC_NPC_UNIT_SIZE, 
					MAGIC_NPC_UNIT_SIZE, MAGIC_NPC_UNIT_SIZE, 28, 131, 12, 12 );
			} break;
		case CX2TFieldNpc::NT_ACCESSORY:
			{
				m_pTextureMiniMap->Draw( vProjectedPos.x - MAGIC_NPC_UNIT_SIZE * 0.5f, vProjectedPos.y - MAGIC_NPC_UNIT_SIZE, 
					MAGIC_NPC_UNIT_SIZE, MAGIC_NPC_UNIT_SIZE, 28, 131, 12, 12 );
			} break;
		case CX2TFieldNpc::NT_PVP:
			{
				m_pTextureMiniMap->Draw( vProjectedPos.x - MAGIC_NPC_UNIT_SIZE * 0.5f, vProjectedPos.y - MAGIC_NPC_UNIT_SIZE, 
					MAGIC_NPC_UNIT_SIZE, MAGIC_NPC_UNIT_SIZE, 28, 131, 12, 12 );
			} break;

		case CX2TFieldNpc::NT_ALCHEMIST:
			{
				m_pTextureMiniMap->Draw( vProjectedPos.x - MAGIC_NPC_UNIT_SIZE * 0.5f, vProjectedPos.y - MAGIC_NPC_UNIT_SIZE, 
					MAGIC_NPC_UNIT_SIZE, MAGIC_NPC_UNIT_SIZE, 28, 131, 12, 12 );
			} break;

		case CX2TFieldNpc::NT_POSTBOX:
			{
				m_pTextureMiniMap->Draw( vProjectedPos.x - MAGIC_NPC_UNIT_SIZE * 0.5f, vProjectedPos.y - MAGIC_NPC_UNIT_SIZE, 
					MAGIC_NPC_UNIT_SIZE, MAGIC_NPC_UNIT_SIZE, 28, 131, 12, 12 );
			} break;

		case CX2TFieldNpc::NT_RANKING:
			{
				m_pTextureMiniMap->Draw( vProjectedPos.x-MAGIC_NPC_UNIT_SIZE*0.5f, vProjectedPos.y-MAGIC_NPC_UNIT_SIZE, 
					MAGIC_NPC_UNIT_SIZE, MAGIC_NPC_UNIT_SIZE, 28, 131, 12, 12 );
			} break;

		case CX2TFieldNpc::NT_PRIVATE_BANK:
			{
				m_pTextureMiniMap->Draw( vProjectedPos.x - MAGIC_NPC_UNIT_SIZE * 0.5f, vProjectedPos.y - MAGIC_NPC_UNIT_SIZE, 
					MAGIC_NPC_UNIT_SIZE, MAGIC_NPC_UNIT_SIZE, 28, 131, 12, 12 );
			} break;

			//{{ kimhc // 2009-12-08 // 미니맵 Render 안함
		case CX2TFieldNpc::NT_NO_RENDER_EVENT:	
			break;
			//}} kimhc // 2009-12-08 // 미니맵 Render 안함

		case CX2TFieldNpc::NT_EVENT:
		case CX2TFieldNpc::NT_TRAINNING:
		case CX2TFieldNpc::NT_NORMAL:
		default:
			{
				m_pTextureMiniMap->Draw( vProjectedPos.x - MAGIC_NPC_UNIT_SIZE * 0.5f, vProjectedPos.y - MAGIC_NPC_UNIT_SIZE, 
					MAGIC_NPC_UNIT_SIZE, MAGIC_NPC_UNIT_SIZE, 28, 131, 12, 12 );
			} break;
		}

	}
	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "DRAW_USER_UNIT" );
	// 유닛 위치 그리기
	if ( NULL != g_pTFieldGame->GetMyUnit() )
	{
		D3DXVECTOR3 vPos = g_pTFieldGame->GetMyUnit()->GetPos();

		if ( g_pTFieldGame->GetWorld() &&
			g_pTFieldGame->GetWorld()->GetLineMap() )
		{
			if ( true == g_pTFieldGame->GetMyUnit()->GetIsRight() )
				g_pTFieldGame->GetWorld()->GetLineMap()->CheckNearLines( vPos, true );
			else
				g_pTFieldGame->GetWorld()->GetLineMap()->CheckNearLines( vPos, false );
		}

		D3DXVec3Project( &vPos, &vPos, &vp, &matProj, &matView, &matWorld );

		if ( m_pTextureIcon[MI_MYSELF] != NULL )
			m_pTextureIcon[MI_MYSELF]->Draw( static_cast<int>( ceil(vPos.x) ) - MAGIC_SMALL_ICON_SIZE_X,
											 static_cast<int>(vPos.y) - MAGIC_SMALL_ICON_SIZE_Y,
											 MAGIC_SMALL_ICON_SIZE_X, MAGIC_SMALL_ICON_SIZE_Y );
	}	
	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "DRAW_PARTY" );
	if( true == g_pData->GetPartyManager()->DoIHaveParty() )
	{
		for( int i=0; i < g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount(); i++ )
		{
			const CX2PartyManager::PartyMemberData* pPartyMemberData = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( i );
			ASSERT( pPartyMemberData != NULL );

			if(  g_pData->GetMyUser()->GetSelectUnit() != NULL
				&& pPartyMemberData->m_iUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				continue;

			if( CX2Unit::CUS_FIELD_MAP != pPartyMemberData->m_eState ||
				pPartyMemberData->m_iStateCode != (int) g_pData->GetLocationManager()->GetCurrentVillageID() )
			{
				continue;
			}

			CX2SquareUnit* pPartySquareUnit = g_pTFieldGame->GetSquareUnitByUID( pPartyMemberData->m_iUnitUID );
			D3DXVECTOR3 vPos = pPartyMemberData->m_vPosition;
			if( NULL != pPartySquareUnit )
			{
				vPos = pPartySquareUnit->GetPos();
			}
			D3DXVECTOR3 vProjection;

			D3DXVec3Project( &vProjection, &vPos, &vp, &matProj, &matView, &matWorld );

			D3DXVECTOR3 vMyPos = g_pTFieldGame->GetMyUnit()->GetPos();
			D3DXVECTOR3 vMyProjection;
			D3DXVec3Project( &vMyProjection, &vMyPos, &vp, &matProj, &matView, &matWorld );

			vMyProjection.z = 0.f;
			vProjection.z = 0.f;

			if ( true == IsInWindowTexture( vProjection ) )
			{
				if ( m_pTextureIcon[MI_PARTY] != NULL )
				{
					m_pTextureIcon[MI_PARTY]->Draw( static_cast<int>( ceil( vProjection.x ) ) - MAGIC_SMALL_ICON_SIZE_X,
													static_cast<int>(vProjection.y) - MAGIC_SMALL_ICON_SIZE_Y,
													MAGIC_SMALL_ICON_SIZE_X, MAGIC_SMALL_ICON_SIZE_Y );
				}
			}
			else
			{
				D3DXVECTOR3 vDist = vProjection - vMyProjection;
				vDist.z = 0.f;
				D3DXVec3Normalize( &vDist, &vDist );
				D3DXVECTOR3 vDir = GetDirVecToDegree( vDist );
				D3DXVECTOR3 vArrowPos( 0.f, 0.f, 0.f );
				D3DXVECTOR3 vCenterPos = m_vMiniMapWindowSize * m_fRenderTargetTextureScale / 2.f;
				vCenterPos.x -= 20.f;

				if ( 0 > vProjection.x && 0 > vDist.x )
				{
					vArrowPos = vMyProjection + vDist * 77.f;
					vDir.z *= -1.f;
				}
				else if ( 0 < vProjection.x && vMyProjection.x < vProjection.x )
				{
					float fDist = vProjection.x - vMyProjection.x;
					
					if ( fDist >= 77.f )
						fDist = 77.f;

					vArrowPos = vCenterPos + vDist * fDist;
					vDir.z += 180.f;
				}
				else if ( 0 < vProjection.x && vMyProjection.x > vProjection.x )
				{
					float fDist = vMyProjection.x - vProjection.x;

					if ( fDist >= 77.f )
						fDist = 77.f;

					vArrowPos = vCenterPos + vDist * fDist;
					vDir.z *= -1.f;
				}

				if ( 17.f > vProjection.y )
					vArrowPos.y = 17.f;

				if ( vCenterPos.y + 40.f < vProjection.y )
					vArrowPos.y = vCenterPos.y + 40.f;

				if ( m_pTextureIcon[MI_PARTY_ARROW] != NULL )
					m_pTextureIcon[MI_PARTY_ARROW]->Draw( static_cast<int>(vArrowPos.x) - MAGIC_SMALL_ICON_SIZE_X, 
														  static_cast<int>(vArrowPos.y) - MAGIC_SMALL_ICON_SIZE_Y,
														  MAGIC_SMALL_ICON_SIZE_X, MAGIC_SMALL_ICON_SIZE_Y,
														  D3DCOLOR_RGBA(255, 255, 255, 255), vDir.z );		
			}
		}
	}
	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "END_RENDER" );
	g_pKTDXApp->SetViewTransform( &matViewOld );
	m_pRenderTargetTexture->EndRender();    
	KTDXPROFILE_END();
}

HRESULT CX2MiniMap::RenderVillage()
{
	KTDXPROFILE();

	if( NULL == m_pRenderTargetTexture )
		return E_FAIL;

	if( false == m_pRenderTargetTexture->IsOk() )
		return E_FAIL;

	if( NULL == m_pTextureMiniMap )
		return E_FAIL;

	const int MAGIC_MINIMIZED_BAR_HEIGHT = 20;


	RenderMiniMapTextureVillage();

	m_pRenderTargetTexture->Draw( (int)m_vMiniMapWindowPos.x + 824, (int)m_vMiniMapWindowPos.y + 558, (int)m_vMiniMapWindowSize.x, (int)m_vMiniMapWindowSize.y );

	// 전체 프레임을 통째로 그리고 
	m_pTextureMiniMap->Draw( m_vMiniMapWindowPos.x + 824, m_vMiniMapWindowPos.y + 558, m_vMiniMapWindowSize.x, m_vMiniMapWindowSize.y, 1, 1, 255, 129 );

	if ( NULL != m_pMiniMapUI )
		m_pMiniMapUI->SetMonsterNum( false );

	return S_OK;
}

void CX2MiniMap::NotifyShowObjectChanged()   
{
	if( NULL != m_pMiniMapUI )
	{
		m_pMiniMapUI->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, GetShowObject() );
	}
}

bool CX2MiniMap::IsInWindowTexture( const D3DXVECTOR3& vProjectedPos )
{
	if( vProjectedPos.x * m_fRenderTargetTextureScale < 0.f )
		return false;

	if( vProjectedPos.y * m_fRenderTargetTextureScale < 0.f )
		return false;

	if( vProjectedPos.x * m_fRenderTargetTextureScale > m_vMiniMapWindowSize.x * m_fRenderTargetTextureScale - 60.f )
		return false;

	if( vProjectedPos.y * m_fRenderTargetTextureScale > m_vMiniMapWindowSize.y * m_fRenderTargetTextureScale )
		return false;

	return true;
}

void CX2MiniMap::RenderGameUnitTexture()
{
	KTDXPROFILE_BEGIN( "SET_MATRIX" );

	const float& fResolutionScaleX = g_pKTDXApp->GetResolutionScaleX();
	const float& fResolutionScaleY = g_pKTDXApp->GetResolutionScaleY();

	D3DVIEWPORT vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width  = static_cast<DWORD>( m_pRenderTargetTexture->GetWidth()  / fResolutionScaleX );
	vp.Height = static_cast<DWORD>( m_pRenderTargetTexture->GetHeight() / fResolutionScaleY );

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );

	const D3DXMATRIX& matView = g_pKTDXApp->GetViewTransform();
	const D3DXMATRIX& matProj = g_pKTDXApp->GetProjectionTransform();

	KTDXPROFILE_END();

	const float fDivisorX = g_pKTDXApp->GetResolutionScaleX() * m_fEyeDistance;
	const float fDivisorY = g_pKTDXApp->GetResolutionScaleY() * m_fEyeDistance;
	const float fMultiplier = 1500.0f * m_fRenderTargetTextureScale;

	const float MAGIC_ALLY_UNIT_SIZE	= 12.f * fMultiplier / fDivisorX;
	const float MAGIC_ENEMY_UNIT_SIZE	= 12.f * fMultiplier / fDivisorX;

	KTDXPROFILE_BEGIN( "DRAW_GAME_UNIT" );

	const int iMyTeam = g_pX2Game->GetMyUnit()->GetTeam();	/// 내 팀 번호
	const vector<CX2GameUnit*>& vecGameUnitList = g_pX2Game->GetUnitList();
	BOOST_FOREACH( const CX2GameUnit* pGameUnit, vecGameUnitList )
	{
		if ( NULL != pGameUnit && pGameUnit->GetShowOnMiniMap() )
		{
			D3DXVECTOR3 vPos = pGameUnit->GetPos();
			D3DXVec3Project( &vPos, &vPos, &vp, &matProj, &matView, &matWorld );

			if ( pGameUnit->IsMyUnit() )
			{
				const float MAGIC_MY_UNIT_SIZE		= 22.f * fMultiplier / fDivisorX;
				m_pTextureMiniMap->Draw( vPos.x-MAGIC_MY_UNIT_SIZE*0.5f, vPos.y-MAGIC_MY_UNIT_SIZE, 
					MAGIC_MY_UNIT_SIZE, MAGIC_MY_UNIT_SIZE, 1, 131, 24, 22 );
			}
			else if ( pGameUnit->GetTeam() == iMyTeam )	/// 같은 팀이면
			{
				m_pTextureMiniMap->Draw( vPos.x-MAGIC_ALLY_UNIT_SIZE*0.5f, vPos.y-MAGIC_ALLY_UNIT_SIZE, 
					MAGIC_ALLY_UNIT_SIZE, MAGIC_ALLY_UNIT_SIZE, 28, 131, 12, 12 );
			}
			else	/// 자신도 아니고 같은 팀도 아니면 적!
			{
				if ( pGameUnit->IsGateNpc() )	/// Gate NPC
				{
					const float MAGIC_GATE_WIDTH		= 9.f * fMultiplier / fDivisorX;
					const float MAGIC_GATE_HEIGHT		= 18.f * fMultiplier / fDivisorY;

					m_pTextureMiniMap->Draw( vPos.x-MAGIC_GATE_WIDTH*0.5f, vPos.y-MAGIC_GATE_HEIGHT, 
						MAGIC_GATE_WIDTH, MAGIC_GATE_HEIGHT, 56, 131, 9, 18 );
				}
				else	/// 일반 NPC
				{
					m_pTextureMiniMap->Draw( vPos.x-MAGIC_ENEMY_UNIT_SIZE*0.5f, vPos.y-MAGIC_ENEMY_UNIT_SIZE, 
						MAGIC_ENEMY_UNIT_SIZE, MAGIC_ENEMY_UNIT_SIZE, 42, 131, 12, 12 );
				}
			}
		}	
	}
	KTDXPROFILE_END();
}

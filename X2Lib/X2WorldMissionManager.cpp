#include "StdAfx.h"
#include ".\x2WorldMissionManager.h"

#ifdef SERV_INSERT_GLOBAL_SERVER

CX2WorldMissionManager::CX2WorldMissionManager()
: m_bActiveDefenceDungeon( false )
, m_iNumOfWorldCrystalsProtected( 0 )
, m_iRemainTimeToFinish( 0 )
, m_usRemainTimeWorldBuff( 0 )
, m_usWorldBuffStepNum( 0 )
//, m_bIsEXPBonus( false )
, m_pUIWorldMission( NULL )
, m_pTextureDefenceDungeonMiniMap( NULL )
, m_pUVDefenceDungeonMiniMap( NULL )
, m_iStartKeyNumberForCrystal( 5 )
, m_iNumOfCrystals( 0 )
, m_pWorldBuffDragonBreath( NULL )
, m_pWorldBuffElBless( NULL )
, m_iRemainTimeMissionStart( -1 )
#ifdef NEW_DEFENCE_DUNGEON
, m_iNowDefenceDungeonLevel( -1 )	//현재 어둠의 문 난이도
#endif NEW_DEFENCE_DUNGEON
#ifdef SERV_2012_PVP_SEASON2_EVENT
, m_bActiveTimeOfGlory( false )		///	영광의 시간 활성 여부
, m_bShowTimeOfGloryDLG( false )	///	영광의 시간 다이얼로그 활성 여부
#endif SERV_2012_PVP_SEASON2_EVENT
// , MAGIC_X_LEFT( 0 )
// , MAGIC_X_RIGHT( 0 )
// , MAGIC_Z_UP( 0 )
// , MAGIC_Z_DOWN( 0 )
// , MAGIC_POS_X( 0 )
// , MAGIC_POS_Y( 0 )
// , MAGIC_WIDTH( 0 )
// , MAGIC_HEIGHT( 0 )
// , MAGIC_EL_WIDTH( 0 )
// , MAGIC_EL_HEIGHT( 0 )
{
	Init();

	m_pUIWorldMission = new CX2UIWorldMission;
}		


CX2WorldMissionManager::~CX2WorldMissionManager()
{
	SAFE_DELETE( m_pUIWorldMission );
}

void CX2WorldMissionManager::Init()
{
	SetIsActiveDefenceDungeon( false );
	SetNumOfWorldCrystalsProtected( 0 );
	SetRemainTimeToFinish( 0 );
//	m_bIsEXPBonus = false;

	for( int i = 0; i < MAX_CRYSTAL_COUNT; i++ )
	{
		m_timerDamaged[i].restart();
	}
}

void CX2WorldMissionManager::RenderMiniMapTexture( CKTDXDeviceRenderTargetTexture* 	pRenderTargetTexture )
{
	if( pRenderTargetTexture == NULL )
		return;

	if( NULL == pRenderTargetTexture )
		return;

	KTDXPROFILE_BEGIN( "CLEAR" );
	pRenderTargetTexture->BeginRender( true );
	pRenderTargetTexture->Clear( D3DCOLOR_RGBA( 0, 0, 0, 0 ) );
	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "END_RENDER" );
	pRenderTargetTexture->EndRender();    
	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "MAP_DRAW" );
	
	const int MAGIC_X_LEFT = -5869;
	const int MAGIC_X_RIGHT = 5869;
	const int MAGIC_Z_UP = 1669;
	const int MAGIC_Z_DOWN = -5187;

	const int MAGIC_POS_X = 785;
	const int MAGIC_POS_Y = 1;

	const int MAGIC_WIDTH = 180;
	const int MAGIC_HEIGHT = 65;

#ifdef NEW_DEFENCE_DUNGEON
	//지형 변경으로 인한 미니맵 아이콘 이동 경로 수정
	const float	fIncreaseDegree			   = 1.34f;		//시작 위치 각도 연동
	const float fMiniMapIconDistance	   =  70.f;		//미니맵 안에서 던전맵 중심점과 유저 아이콘 과의 거리
	const float MiniMapCenterPosition_x    = 877.f;		//미니맵 안에서 던전맵 중심점 x 좌표
	const float MiniMapCenterPosition_y	   =  13.f;		//미니맵 안에서 던전맵 중심점 z 좌표
	const float DungeonMapCenterPosition_x =   0.f;		//던전맵 중심점 x 좌표
	const float DungeonMapCenterPosition_z = 800.f;		//던전맵 중심점 z 좌표
#endif NEW_DEFENCE_DUNGEON

	if ( NULL != GetTextureDefenceDungeonMiniMap() && NULL != GetUVDefenceDungeonMiniMap() )
	{
		const D3DXVECTOR2& vMiniMapWindowPos = g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->GetMiniMapWindowPos();
		const D3DXVECTOR2& vWindowTextureSize = g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->GetMiniMapWindowSize();

		GetTextureDefenceDungeonMiniMap()->Draw( static_cast<int>( vMiniMapWindowPos.x ), static_cast<int>( vMiniMapWindowPos.y ), 
			static_cast<int>( vWindowTextureSize.x ), static_cast<int>( vWindowTextureSize.y ), GetUVDefenceDungeonMiniMap() );

		KTDXPROFILE_BEGIN( "DRAW_CRYSTAL" );

		int iEndKeyCode = m_iStartKeyNumberForCrystal + GetNumOfCrystals();

		for( int iKeyCode = m_iStartKeyNumberForCrystal; iKeyCode < iEndKeyCode; ++iKeyCode )
		{
			const CX2GUNPC* pCrystal = g_pX2Game->GetNPCUnitByKeyCode( iKeyCode );

			if ( NULL != pCrystal ) 
			{
				const D3DXVECTOR3& vPos = pCrystal->GetPos();

#ifdef NEW_DEFENCE_DUNGEON
				//던전맵 중심점과 엘의 수정 과의 각도
				float fDegree = static_cast<float>( ( atan2( DungeonMapCenterPosition_x - vPos.x, DungeonMapCenterPosition_z - vPos.z ) ) + fIncreaseDegree );

				RenderCrystal( 
								static_cast<int>( MiniMapCenterPosition_x + ( fMiniMapIconDistance * cos( fDegree ) ) ) - 5,
								static_cast<int>( MiniMapCenterPosition_y + ( fMiniMapIconDistance * sin( fDegree ) ) ) + 1, 
								pCrystal, 
								m_timerDamaged[iKeyCode-m_iStartKeyNumberForCrystal] 
								);
#else  NEW_DEFENCE_DUNGEON
				int iPosX = static_cast<int>( MAGIC_POS_X + ( (vPos.x + MAGIC_X_RIGHT) * MAGIC_WIDTH ) / (MAGIC_X_RIGHT - MAGIC_X_LEFT) );
				int iPosY = static_cast<int>( MAGIC_POS_Y + ( (vPos.z + MAGIC_Z_DOWN) * MAGIC_HEIGHT ) / (MAGIC_Z_DOWN - MAGIC_Z_UP) );

				RenderCrystal( iPosX, iPosY, pCrystal, m_timerDamaged[iKeyCode-m_iStartKeyNumberForCrystal] );
#endif NEW_DEFENCE_DUNGEON
			}
		}
		KTDXPROFILE_END();
	}
	
	KTDXPROFILE_BEGIN( "DRAW_USER_UNIT" );

	CKTDXDeviceTexture* pTextureMyUnit  = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Char_My.dds" );
	CKTDXDeviceTexture* pTexturePartyUnit  = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Char_Party.dds" );

	int MAGIC_PLAYER_WIDTH = 350;

	// 유닛 위치 그리기
	CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();
	for( int i = 0; i < (int)g_pX2Game->GetUserUnitListSize(); i++ )
	{
		CX2GUUser* pCX2GUUser = g_pX2Game->GetUserUnit(i);
		if( NULL != pCX2GUUser &&
			pCX2GUUser->GetNowHp() > 0.f &&
			pCX2GUUser->GetGameUnitState() != CX2GameUnit::GUSI_DIE )
		{
			D3DXVECTOR3 vPos = pCX2GUUser->GetPos();

#ifdef NEW_DEFENCE_DUNGEON
			//던전맵 중심점과 유저와의 각도
			float fDegree = static_cast<float>( ( atan2( DungeonMapCenterPosition_x - vPos.x, DungeonMapCenterPosition_z - vPos.z ) ) + fIncreaseDegree );

			//던전맵 중심점과 유저가 이루는 각도를 그대로 미니맵 아이콘에도 적용시켜, 유저의 움직임을 연동 시킨다.
			if( pCX2GUUser == pMyUnit )
			{
				pTextureMyUnit->Draw( 
											static_cast<int>( MiniMapCenterPosition_x + ( fMiniMapIconDistance * cos( fDegree ) ) ), 
											static_cast<int>( MiniMapCenterPosition_y + ( fMiniMapIconDistance * sin( fDegree ) ) ), 
											30, 
											30 
									);
			}
			else if( pCX2GUUser->GetTeam() == pMyUnit->GetTeam() )
			{
				pTexturePartyUnit->Draw( 
											static_cast<int>( MiniMapCenterPosition_x + ( fMiniMapIconDistance * cos( fDegree ) ) ), 
											static_cast<int>( MiniMapCenterPosition_y + ( fMiniMapIconDistance * sin( fDegree ) ) ), 
											30, 
											30 
										);
			}
#else  NEW_DEFENCE_DUNGEON
			if( pCX2GUUser == pMyUnit )
			{
				pTextureMyUnit->Draw( static_cast<int>( MAGIC_POS_X + ((vPos.x + MAGIC_PLAYER_WIDTH + MAGIC_X_RIGHT) * MAGIC_WIDTH) / (MAGIC_X_RIGHT - MAGIC_X_LEFT) ), static_cast<int>( MAGIC_POS_Y + ((vPos.z + MAGIC_Z_DOWN) * MAGIC_HEIGHT) / (MAGIC_Z_DOWN - MAGIC_Z_UP) ), 30, 30 );
			}
			else if( pCX2GUUser->GetTeam() == pMyUnit->GetTeam() )
			{
				pTexturePartyUnit->Draw( static_cast<int>( MAGIC_POS_X + ((vPos.x + MAGIC_PLAYER_WIDTH + MAGIC_X_RIGHT) * MAGIC_WIDTH) / (MAGIC_X_RIGHT - MAGIC_X_LEFT) ), static_cast<int>( MAGIC_POS_Y + ((vPos.z + MAGIC_Z_DOWN) * MAGIC_HEIGHT) / (MAGIC_Z_DOWN - MAGIC_Z_UP) ) , 30, 30 );
			}
#endif NEW_DEFENCE_DUNGEON
		}
	}
	KTDXPROFILE_END();

	KTDXPROFILE_END();
}

void CX2WorldMissionManager::RenderCrystal( int iPosX_, int iPosY_, const CX2GUNPC* pCrystal_, CKTDXTimer& tTimer_ )
{
	CKTDXDeviceTexture* pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_UI_Common_Texture46.tga" );
	if ( NULL == pTexture )
	{
		ASSERT( NULL != pTexture );
		return;
	}

	int iWidth			= 24;
	int iHeight			= 42;
	int MAGIC_EL_WIDTH	= 7;
	int MAGIC_EL_HEIGHT = -10;

	// 작은 수정의 경우 1/2
	if ( CX2UnitManager::NUI_DEFENCE_CRYSTAL_SMALL == pCrystal_->GetNPCTemplet().m_nNPCUnitID )
	{
		iWidth /= 2;
		iHeight /= 2;
		MAGIC_EL_WIDTH = 15;
		MAGIC_EL_HEIGHT = 11;
	}
	
	if( tTimer_.elapsed() > 1.8f )
	{
		CX2GUNPC* pCrystal = const_cast<CX2GUNPC*>( pCrystal_ );
		if( pCrystal->GetDamagedThisFrame() == true )
		{
			tTimer_.restart();
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Event_Dungeon_Hit_Alert.ogg", false, false );
		}
	}
	else
	{
		const CKTDXDeviceTexture::TEXTURE_UV* pTextureUVForDamaged = pTexture->GetTexUV( L"ELL_OUT");

		//	깜빡이는 연출을한다. 어떻게?
		int iTime = static_cast<int>( tTimer_.elapsed() * 10.0 );

		if( iTime % 4 < 2 )
		{
			int iSubtractPosValue = -3;
			int iAddWidthHeightValue = 5;			
            
			// 작은 수정의 경우 1/2
			if ( CX2UnitManager::NUI_DEFENCE_CRYSTAL_SMALL == pCrystal_->GetNPCTemplet().m_nNPCUnitID )
			{
				iSubtractPosValue = -2;
				iAddWidthHeightValue = 2;				
			}

			ASSERT( NULL != pTextureUVForDamaged );
			if ( NULL != pTextureUVForDamaged )
				pTexture->Draw( iPosX_ + MAGIC_EL_WIDTH + iSubtractPosValue, iPosY_ + MAGIC_EL_HEIGHT + iSubtractPosValue, iWidth + iAddWidthHeightValue, iHeight + iAddWidthHeightValue, pTextureUVForDamaged );
		}
	}

	//	체력에 따라 다른 색 텍스쳐를 그려주자
	const float fMaxHP = pCrystal_->GetMaxHp();
	const float fNowHP = pCrystal_->GetNowHp();
	const float fPercentHP = fNowHP / fMaxHP * 100;
	
	const CKTDXDeviceTexture::TEXTURE_UV* pTextureUV = NULL;
	if( fPercentHP > 70.0f )
	{
		pTextureUV = pTexture->GetTexUV( L"ELL_GREEN");
	}
	else if ( fPercentHP > 30.0f )
	{
		pTextureUV = pTexture->GetTexUV( L"ELL_YELLOW");
	}
	else if ( fPercentHP > 1.0f )
	{
		pTextureUV = pTexture->GetTexUV( L"ELL_RED");
	}
	else 
	{
		return;
	}

	if ( NULL == pTextureUV )
	{
		ASSERT( NULL != pTextureUV );
		return;
	}

	pTexture->Draw( iPosX_ + MAGIC_EL_WIDTH, iPosY_ + MAGIC_EL_HEIGHT, iWidth, iHeight, pTextureUV );
	
}

bool CX2WorldMissionManager::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_pUIWorldMission != NULL )
	{
		if( true == m_pUIWorldMission->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
		{
			return true;
		}
	}

	return false;
}

bool CX2WorldMissionManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();
	switch( wParam )
	{
	case EGS_WORLD_MISSION_UPDATE_NOT:
		{
			return Handler_EGS_WORLD_MISSION_UPDATE_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_UPDATE_WORLD_BUFF_NOT:
		{
			return Handler_EGS_UPDATE_WORLD_BUFF_NOT( hWnd, uMsg, wParam, lParam );
		} break;

#ifdef SERV_2012_PVP_SEASON2_EVENT
	case EGS_PVP_EVENT_INFO_NOT:
		{
			return Handler_EGS_PVP_EVENT_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_2012_PVP_SEASON2_EVENT
#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
	case EGS_EARTH_QUAKE_NOT:
		{
			return Handler_EGS_EARTH_QUAKE_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif TOGETHER_FESTIVAL_2012_AUTUMN
	default:
		break;
	}
	return false;
}

HRESULT CX2WorldMissionManager::OnFrameMove( double fTime, float fElapsedTime )
{
	if( true == IsActiveDefenceDungeon() )
	{
		//int iProgress = 0;
		int iTime =  GetRemainTimeToFinish() - static_cast<int>(m_timerMissionToFinish.elapsed() );

		if( iTime < 0 )
		{
			iTime = 0;
		}


		// 마을일경우 UI 개방 ( 시간 , 보너스 경험치 )
		if( 0 != iTime &&
			(g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP || 
			g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD) )
		{
			if(g_pData != NULL && g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == true)
			{
				// 캐시샵에서는 열리지 않음
			}
			else
			{
				m_pUIWorldMission->UpdateTimeDlg( iTime );
#ifndef SERV_NEW_DEFENCE_DUNGEON
				m_pUIWorldMission->UpdateNumOfCrystalsProtected( GetNumOfWorldCrystalsProtected() );
#endif // SERV_NEW_DEFENCE_DUNGEON
			}
		}	
		else
		{
			if( m_pUIWorldMission->GetTimeDlg() != NULL )
			{
				m_pUIWorldMission->SetShowTimeDlg( false );
			}
		}
	}
	else
	{
		if( m_pUIWorldMission->GetTimeDlg() != NULL )
		{
			m_pUIWorldMission->SetShowTimeDlg( false );
		}
	}

	return S_OK;
}

bool	CX2WorldMissionManager::Handler_EGS_WORLD_MISSION_UPDATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_WORLD_MISSION_UPDATE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( false == IsActiveDefenceDungeon() && true == kEvent.m_bActive )
	{
		 //////////////////////////////////////////////////////////////////////////
		//	월드 미션 시작

		// 마을에 있을때 이 패킷을 받으면 '마족침공' 이라는 이미지를 보여줌
		// 반대로 이미 미션이 진행 중일 때 이 패킷을 받으면 (캐릭터 선택 시점, 마을 전), 이미지를 보여주지 않음
		if( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP ||
			g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME ||
			g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD )
		{
			SetIsActiveDefenceDungeon( true );
			// 캐시샵에서는 열리지 않음
			if( NULL == g_pData || NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() )
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
				pX2State->OpenStartMission();
#ifdef FIX_WORLDMISSION_UI_BUG
				m_pUIWorldMission->SetShowTimeDlg( true );
			}
#else
			}
			m_pUIWorldMission->SetShowTimeDlg( true );
#endif //FIX_WORLDMISSION_UI_BUG
		}

#ifdef NEW_DEFENCE_DUNGEON
		//어둠의 문 개방시 난이도 보통
		g_pData->GetWorldMissionManager()->SetNowDefenceDungeonLevel( CX2Dungeon::DL_NORMAL );
		
#ifndef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-15	
	/// 레벨 변경 없음
	#ifdef FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
		ChangeDefenceDungeonLevel();
	#endif FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
#endif // SERV_NEW_DEFENCE_DUNGEON

#endif NEW_DEFENCE_DUNGEON

	}

	//////////////////////////////////////////////////////////////////////////
	// 월드 미션 종료
#ifdef SERV_NEW_DEFENCE_DUNGEON
	if( true == IsActiveDefenceDungeon() )
	{
		if ( false == kEvent.m_bActive )
#else // SERV_NEW_DEFENCE_DUNGEON
	if( true == IsActiveDefenceDungeon() && false == kEvent.m_bActive )
	{
#endif // SERV_NEW_DEFENCE_DUNGEON
		{
			//	마을일경우 UI 종료
			if( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP ||
				g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME ||
				g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD )
			{
				SetIsActiveDefenceDungeon( false );
				// 캐시샵에서는 열리지 않음
				if( NULL == g_pData || NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() )
				{
					CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
		#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-04-16
					/// 어둠의 문 성공 조건 변경
					pX2State->OpenResultMission( kEvent.m_iWorldCrystalCount >= 0 );
		#else // SERV_NEW_DEFENCE_DUNGEON
					pX2State->OpenResultMission( GetNumOfWorldCrystalsProtected() >= 3 );
		#endif // SERV_NEW_DEFENCE_DUNGEON
				}
			}

			m_pUIWorldMission->SetShowTimeDlg( false );

#ifdef NEW_DEFENCE_DUNGEON
			//어둠의 문 난이도 초기화
			g_pData->GetWorldMissionManager()->SetNowDefenceDungeonLevel( -1 );
#endif NEW_DEFENCE_DUNGEON
		}
#ifdef SERV_NEW_DEFENCE_DUNGEON
		else		/// 월드 미션 UI 갱신
		{
			if ( NULL != m_pUIWorldMission )
				m_pUIWorldMission->UpdateNumOfCrystalsProtected( kEvent.m_iWorldCrystalCount, kEvent.m_iWorldMaxCrystalCount );
		}
#endif // SERV_NEW_DEFENCE_DUNGEON
	}

// 	if( true == kEvent.m_bIsEXPBonus && m_pUIWorldMission->GetEXPBonusDlg() == NULL )
// 	{
// 		if(g_pData != NULL && g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == true)
//		{
			// 캐시샵에서는 열리지 않음
//		}
// 		else
// 		{
// 			m_pUIWorldMission->SetShowEXPBonusDlg( true );
// 		}
// 	}
// 	else if ( false == kEvent.m_bIsEXPBonus && m_pUIWorldMission->GetEXPBonusDlg() != NULL )
// 	{
// 		m_pUIWorldMission->SetShowEXPBonusDlg( false );
// 	}
	SetRemainTimeMissionStart( kEvent.m_iMissionStartRemainTime );
	m_timerMissionToStart.restart();

	SetRemainTimeToFinish( kEvent.m_iRemainTime );
	m_timerMissionToFinish.restart();

	SetNumOfWorldCrystalsProtected( kEvent.m_iWorldCrystalCount );
	
#ifdef NEW_DEFENCE_DUNGEON	
	//지켜낸 수정의 갯수에 따라 난이도 값을 저장한다.
	ChangeDefenceDungeonLevel();

#ifndef FIX_NEW_DEFENCE_DUNGEON_PARTY_UI		/// 해당 패킷을 받을 때 마다 파티 UI가 갱신되지 않도록 수정
	g_pMain->GetPartyUI()->UpdatePartyDLG();
#endif  FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
	
#endif NEW_DEFENCE_DUNGEON

//	m_bIsEXPBonus = kEvent.m_bIsEXPBonus;

#ifdef SERV_2012_PVP_SEASON2_EVENT
	SetTimeOfGloryDLG();		/// 영광의 시간 다이얼로그 활성 함수 -  월드 미션 업데이트 NOT 올때마다 검사 한다.
#endif SERV_2012_PVP_SEASON2_EVENT

	return true;
}

/** @function : Handler_EGS_UPDATE_WORLD_BUFF_NOT
	@brief : 디펜스던전 컨텐츠의 월드버프 패킷에 대한 핸들러
	@return : bool
*/
bool CX2WorldMissionManager::Handler_EGS_UPDATE_WORLD_BUFF_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_WORLD_BUFF_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	
	// 월드 버프가 한개라도 시작 되면
	if ( 0 < kEvent.m_usSuccessWorldBuff )
	{
		SetWorldBuffStepNum( kEvent.m_usSuccessWorldBuff );

		// 디펜스 던전 종료 때 까지 남은 시간
		int iRemainTimeByClosingDefenceDungeon = GetRemainTimeToFinish() - static_cast<int>( m_timerMissionToFinish.elapsed() );
		if ( 0 > iRemainTimeByClosingDefenceDungeon )
			iRemainTimeByClosingDefenceDungeon = 0;

		SetRemainTimeWorldBuff( kEvent.m_usWorldBuffDurationTime + iRemainTimeByClosingDefenceDungeon );
		m_timerWorldBuff.restart();

		CreateWorldBuffDragonBreath();
		CreateWorldBuffElBless();
	}
	
	return true;
}
void CX2WorldMissionManager::SetDefenceDungeonMiniMap( const wstring& wstrTextureName, const wstring& wstrKeyName )
{
	SetTextureDefenceDungeonMiniMap( g_pKTDXApp->GetDeviceManager()->OpenTexture( wstrTextureName ) );

	if ( NULL != GetTextureDefenceDungeonMiniMap() )
	{
		const CKTDXDeviceTexture::TEXTURE_UV* pTexUV = GetTextureDefenceDungeonMiniMap()->GetTexUV( wstrKeyName );
		SetUVDefenceDungeonMiniMap( pTexUV );
	}
}

void CX2WorldMissionManager::CreateWorldBuffDragonBreath()
{
	// 드래곤의 숨결이 이미 생성 되어 있으면
	if ( NULL != m_pWorldBuffDragonBreath )
		return;

	// 월드 버프가 2단계 까지 진행 되지 않았으면
	if ( 2 > GetWorldBuffStepNum() )
		return;	

	m_pWorldBuffDragonBreath = new WORLD_BUFF_DRAGON_BREATH( 100.0f, 3, 1.05f );	
}

void CX2WorldMissionManager::DeleteWorldBuffDragonBreath()
{
	SAFE_DELETE( m_pWorldBuffDragonBreath );
}

bool CX2WorldMissionManager::GetDragonBreathEnchantStatIncreased( OUT const CX2Stat::EnchantStat*& pEnchantStatInreased_ ) const
{
	if ( NULL == m_pWorldBuffDragonBreath )
	{
		pEnchantStatInreased_ = NULL;
		return false;
	}
	else
	{
		pEnchantStatInreased_ = &(m_pWorldBuffDragonBreath->m_EnchantStatIncreased);
		return true;
	}
}

const int CX2WorldMissionManager::GetDragonBreathAnimSpeedRateIncreased() const
{
	if ( NULL == m_pWorldBuffDragonBreath )
		return 0;
	else
		return m_pWorldBuffDragonBreath->m_iAnimSpeedRateIncreased;
}

const float CX2WorldMissionManager::GetDragonBreathAttackStatRateIncreased() const
{
	if ( NULL == m_pWorldBuffDragonBreath )
		return 0.0f;
	else
		return m_pWorldBuffDragonBreath->m_fAttackStatRateIncreased;
}

/** @function : GetEnchantStatFromDrgonBreath
	@brief : 인첸트 타입별 수치 값 리턴
	@param : CX2EnchantItem::ENCHANT_TYPE eType (수치를 알고싶은 인첸트 타입)
	@return : const float (해당 인첸트 타입의 수치 값)
*/
const float CX2WorldMissionManager::GetEnchantStatFromDrgonBreath( int iEnchantType ) const
{
	// 이 함수를 콜 하는 상위 함수에서 m_pWorldBuffDragonBreath에 대하여
	// NULL Check를 해야 함
	switch ( CX2EnchantItem::ENCHANT_TYPE( iEnchantType ) )
	{
	case CX2EnchantItem::ET_BLAZE:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefBlaze;
		break;

	case CX2EnchantItem::ET_WATER:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefWater;
		break;

	case CX2EnchantItem::ET_NATURE:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefNature;
		break;

	case CX2EnchantItem::ET_WIND:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefWind;
		break;

	case CX2EnchantItem::ET_LIGHT:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefLight;
		break;

	case CX2EnchantItem::ET_DARK:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefDark;
		break;

	default:
		return 0.0f;
		break;
	} // switch
	
}

const int CX2WorldMissionManager::GetElBlessCriticalRateIncreased() const
{
	if ( NULL == m_pWorldBuffElBless )
		return 0;
	else
		return m_pWorldBuffElBless->m_iCriticalRateIncreased;
}

const float CX2WorldMissionManager::GetElBlessDefenceStatRateIncreased() const
{
	if ( NULL == m_pWorldBuffElBless )
		return 0.0f;
	else
		return m_pWorldBuffElBless->m_fDefenceStatRateIncreased;
}

void CX2WorldMissionManager::CreateWorldBuffElBless()
{
	// 엘의 축복이 이미 생성 되어 있으면
	if ( NULL != m_pWorldBuffElBless )
		return;

	// 월드 버프가 3단계 까지 진행 되지 않았으면
	if ( 3 > GetWorldBuffStepNum() )
		return;	

	m_pWorldBuffElBless = new WORLD_BUFF_EL_BLESS( 2, 1.05f );	
}

void CX2WorldMissionManager::DeleteWorldBuffElBless()
{
	SAFE_DELETE( m_pWorldBuffElBless );
}

void CX2WorldMissionManager::GetDefenceDungeonIcnoDesc( OUT wstringstream& wstrStreamDungeonIconDesc_ )
{
	if ( false == IsActiveDefenceDungeon() )
	{
		// 오늘은 더이상 디펜스 던전이 열리지 않으면
		if ( -1 == GetRemainTimeMissionStart() )
		{
			wstrStreamDungeonIconDesc_ << GET_STRING( STR_ID_12117 ) << '\n';
		}
		// 디펜스 던전이 언제 열리는지 알려 줌
		else
		{
			wstrStreamDungeonIconDesc_ << GET_STRING( STR_ID_12116 );

			int iMinute = GetRemainTimeMissionStart() + 1;

			// 60분 미만이면 분으로 표시
			if ( 60 > iMinute )
			{
				wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_12125, "i", iMinute ) ) << '\n';
			}
			// 60분 이상 이면 시간으로 표시
			else
			{
				int iHour = iMinute / 60;	// 몫
				iMinute %= 60;	// 나머지

				wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_12126, "ii", iHour, iMinute ) ) << '\n';
			}		
		}
	}
	else
	{
		wstrStreamDungeonIconDesc_ << L"";
	}
}

// void CX2WorldMissionManager::LoadScript()
// {
// 	const wstring wstrFileName( L"WordlMissionTest.lua" );
// 	string	strFileName;
// 
// 
// 	ConvertWCHARToChar( strFileName, wstrFileName );
// 	ConvertFileAnsiToUTF8( strFileName, strFileName );
// 
// 	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
// 
// 	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
// 	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( strFileName );
// 
// 	if( Info != NULL )
// 	{
// 		if( luaManager.DoMemory( Info->pRealData, Info->size ) == true )
// 		{
// 			if ( luaManager.BeginTable( "MINI_MAP" ) == true )
// 			{
// 				LUA_GET_VALUE(	luaManager, "MAGIC_X_LEFT", MAGIC_X_LEFT,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_X_RIGHT", MAGIC_X_RIGHT,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_Z_UP", MAGIC_Z_UP,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_Z_DOWN", MAGIC_Z_DOWN,	0 );
// 
// 				LUA_GET_VALUE(	luaManager, "MAGIC_POS_X", MAGIC_POS_X,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_POS_Y", MAGIC_POS_Y,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_WIDTH", MAGIC_WIDTH,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_HEIGHT", MAGIC_HEIGHT,	0 );
// 
// 				LUA_GET_VALUE(	luaManager, "MAGIC_EL_WIDTH", MAGIC_EL_WIDTH,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_EL_HEIGHT", MAGIC_EL_HEIGHT,	0 );
// 
// 				luaManager.EndTable();
// 			}
// 		}
// 	}
// }



#endif SERV_INSERT_GLOBAL_SERVER

#ifdef NEW_DEFENCE_DUNGEON
//지켜낸 수정의 갯수에 따라서 어둠의 문 난이도 값을 설정한다.
void CX2WorldMissionManager::ChangeDefenceDungeonLevel()
{
#ifdef FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
	if( NULL == g_pData || NULL == g_pData->GetWorldMissionManager() )
		return;

	const int iBeforeDefenceCrystal = GetNowDefenceDungeonLevel();		/// 변경 전 어둠의 문 레벨
#endif FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
	const int iNumOfCrystal =  g_pData->GetWorldMissionManager()->GetNumOfWorldCrystalsProtected();

	if( iNumOfCrystal >= 0 && iNumOfCrystal < 7 )
		SetNowDefenceDungeonLevel( CX2Dungeon::DL_NORMAL );
	else if( iNumOfCrystal >= 7 && iNumOfCrystal < 13 )
		SetNowDefenceDungeonLevel( CX2Dungeon::DL_HARD );
	else
		SetNowDefenceDungeonLevel( CX2Dungeon::DL_EXPERT );

#ifdef FIX_NEW_DEFENCE_DUNGEON_PARTY_UI		/// 어둠의 문 난이도가 변경되었을 때만 파티 UI 갱신 처리
	if( iBeforeDefenceCrystal != GetNowDefenceDungeonLevel() )
	{
		if( NULL == g_pData->GetPartyManager() || NULL == g_pData->GetPartyManager()->GetMyPartyData() || NULL == g_pData->GetDungeonManager() )
			return;

		/// 어둠의 문이 선택중일 때만 갱신
		if( true == g_pData->GetDungeonManager()->IsDefenceDungeon( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) )
		{
			if( NULL != g_pMain && NULL != g_pMain->GetPartyUI() )
				g_pMain->GetPartyUI()->UpdatePartyDLG();
		}
	}
#endif  FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
}
#endif NEW_DEFENCE_DUNGEON

#ifdef SERV_2012_PVP_SEASON2_EVENT
void CX2WorldMissionManager::SetTimeOfGloryDLG()
{
	if( false == m_bShowTimeOfGloryDLG && true == m_bActiveTimeOfGlory )		/// 영광의 시간 활성 상태에서, 다이얼로그 표시되지 않았을 때
	{
		if( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP || g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )	/// 마을이나 던전일 때
		{
			if( NULL == g_pData || NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() )			/// 캐시샵이 아닐 때
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );

				if( NULL != pX2State )
					pX2State->OpenTimeOfGlory();	/// 영광의 시간 다이얼로그 활성

				if( NULL != g_pData && NULL != g_pData->GetPremiumBuffManager() )		/// 영광의 시간 버프 삽입( 로컬 )
					g_pData->GetPremiumBuffManager()->SetLocalTimeOfGloryBuff();

				m_bShowTimeOfGloryDLG = true;		/// 다이얼로그 표시 처리 설정
			}
		}
	}
	else if( true == m_bShowTimeOfGloryDLG && false == m_bActiveTimeOfGlory )	/// 영광의 시간 비활성 상태에서, 다이얼로그 표시된 상태일 때
	{
		m_bShowTimeOfGloryDLG = false;				/// 다이얼로그 표시 처리 해제
	}
}

bool CX2WorldMissionManager::Handler_EGS_PVP_EVENT_INFO_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PVP_EVENT_INFO_NOT kNot; 
	DeSerialize( pBuff, &kNot );

	m_bActiveTimeOfGlory = kNot.m_bEventInfo;		/// 영광의 시간 설정

	if( false == m_bActiveTimeOfGlory )				/// 만약 비활성 될 때, 영광의 시간 버프 삭제
	{
		if( NULL != g_pData->GetPremiumBuffManager() )
		{
			g_pData->GetPremiumBuffManager()->ErasePremiumBuff( CX2PremiumBuffTemplet::BI_TIME_OF_GLORY );
		}
	}

	return S_OK;
}
#endif SERV_2012_PVP_SEASON2_EVENT

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
bool CX2WorldMissionManager::Handler_EGS_EARTH_QUAKE_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )		/// 공존의 축제 지진 이벤트 패킷
{
	if( NULL != g_pData && NULL != g_pData->GetCashShop() && false == g_pData->GetCashShop()->GetOpen() &&
		NULL != g_pTFieldGame && false == g_pTFieldGame->GetJoinNpc() )					/// 캐시샵 및 대화중 예외 처리
	{
		if( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetCamera() )
		{
			if( NULL != g_pKTDXApp && NULL != g_pKTDXApp->GetDeviceManager() )			/// 지지진 사운드 재생
				g_pKTDXApp->GetDeviceManager()->PlaySound( L"Event_Earthquake.ogg", false, false );

			g_pTFieldGame->GetCamera()->GetCamera().UpDownCrashCameraSmooth( 35.f, 10.f );	/// 카메라 크래시 설정

			if( NULL != g_pMain )
			{
				CX2StateField* pStateField	= NULL;

				pStateField	= static_cast< CX2StateField* >( g_pMain->GetNowState() );

				if( NULL != pStateField )
					pStateField->SetTogetherFestivalEarthQuakeNoticeTime( 5.f );		/// 공존의 축제 지진 이벤트 발생시 생성되는 문구 생성
			}
		}
	}

	return S_OK;
}
#endif TOGETHER_FESTIVAL_2012_AUTUMN
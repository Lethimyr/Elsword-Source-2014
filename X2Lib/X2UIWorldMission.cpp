//{{ 허상형 : [2011/3/22/] //	월드 미션 UI

#include "StdAfx.h"
#include "X2UIWorldMission.h"

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-26

namespace _CONST_UI_WORLD_MISSION_
{
	const float MAX_DEFENCE_VALUE							= 100.f;	/// 어둠의 문 UI 최대 진행 수치
	const float DEFENCE_DUNGEON_WORLD_MISSION_UI_GAGE_WIDTH	= 74.f;		/// 월드 미션 UI의 게이지 길이
}

#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_INSERT_GLOBAL_SERVER
CX2UIWorldMission::CX2UIWorldMission() :
// m_pDlgReward( NULL )
m_pDlgTime( NULL )
//,m_pDlgEXPBonus( NULL )
//,m_bReservedReward( false )
//,m_iRewardItemID( 0 )
//,m_iRewardQuantity( 0 )
, m_iNumOfCrystalsProtected( 0 )
, m_bEnableTimeDlg( false )
, m_bForceUpdate(false)
#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-27
, m_bIsFlickerDanger(false)
#endif // SERV_NEW_DEFENCE_DUNGEON

{
}

CX2UIWorldMission::~CX2UIWorldMission()
{
	//SAFE_DELETE_DIALOG(m_pDlgReward);
	SAFE_DELETE_DIALOG(m_pDlgTime);
}

// bool CX2UIWorldMission::SetShowRewardDlg( bool bEnable )
// {
// 	if(bEnable == true)
// 	{
// 		if( m_iRewardItemID < 0 )
// 			return false;
// 
// 		if( m_iRewardQuantity < 0 )
// 			return false;
// 
// 		m_pDlgReward = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Defence_Dungeon_Reward.lua" );
// 		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgReward );
// 
// 		CKTDGUIStatic* pStaticReward = (CKTDGUIStatic*) m_pDlgReward->GetControl( L"g_pStatic_item_Slot_Image" );
// 
// 		CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_iRewardItemID );
// 
// 		if(pStaticReward != NULL && pItemTemplet != NULL)
// 		{
// 			pStaticReward->GetPicture(0)->SetTex( pItemTemplet->m_ShopImage.c_str() );
// 		}
// 
// 		// 스트링(아이템 과 수량에 따라 분류)
// 		CKTDGUIStatic* pStaticTitle = (CKTDGUIStatic*)( m_pDlgReward->GetControl( L"Static_Item_Name" ) );
// 
// 		std::wstring wstrContent;
// 		int iStrID = 0;
// 
// 		if( m_iRewardItemID == REWARD_SUCCESS_ITEM_ID )
// 		{
// 			iStrID = STR_ID_11380;
// 		}
// 		else if( m_iRewardItemID == REWARD_FAIL_ITEM_ID )
// 		{
// 			switch( m_iRewardQuantity )
// 			{
// 			case 5:
// 				iStrID = STR_ID_11381;
// 				break;
// 			case 3:
// 				iStrID = STR_ID_11382;
// 				break;
// 			case 2:
// 				iStrID = STR_ID_11383;
// 				break;
// 			}
// 		}
// 
// 		if ( pStaticTitle != NULL )
// 		{
// 			pStaticTitle->SetString( 0, GET_STRING( iStrID ) );
// 		}
// 
// 		return true;
// 	}
// 	else
// 	{
// 		if(m_pDlgReward != NULL)
// 		{
// 			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgReward, NULL, false );
// 			m_pDlgReward = NULL;
// 		}
// 
// 		return true;
// 	}
// 
// 	return false;
// }

void CX2UIWorldMission::SetShowTimeDlg( bool bEnable )
{

	// 어문 활성화 상태가 아닌데 true로 변경시도하면 처리하지 않기
	if( true == bEnable )
	{
		if( NULL != g_pData &&
			NULL != g_pData->GetWorldMissionManager() )
		{
			if( false == g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon() )
				return;
		}
	}

	if( false == bEnable )
	{
		if( NULL != m_pDlgTime )
		{
			m_pDlgTime->SetShow(false);
			SetEnableTimeDlg(false);
			return;
		}
	}

	if ( GetEnableTimeDlg() != bEnable )
	{
		if ( true == bEnable )
		{
			if(m_pDlgTime == NULL)
			{
				m_pDlgTime = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_World_Mission_Time.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgTime );
				m_pDlgTime->SetLayer(XDL_NORMAL);
			}

			m_pDlgTime->SetShowEnable( bEnable, bEnable );		
			SetEnableTimeDlg( bEnable );
		}
	}
// 	if( true == bEnable &&  )
// 	{
// 		if(m_pDlgTime == NULL)
// 		{
// 			m_pDlgTime = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_World_Mission_Time.lua" );
// 			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgTime );
// 			m_pDlgTime->SetLayer(XDL_NORMAL);
// 		}
// 
// 		m_pDlgTime->SetShowEnable( bEnable, bEnable );		
// 		SetEnableTimeDlg( bEnable );
// 
// 		return true;
// 	}
// 	else if ( false == bEnable && GetEnableTimeDlg() != bEnable )
// 	{
// 		m_pDlgTime->SetShowEnable( bEnable, bEnable );
// 		SetEnableTimeDlg( bEnable );
// 		if(m_pDlgTime != NULL)
// 		{
// 			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgTime, NULL, false );
// 			m_pDlgTime = NULL;
// 
// 			// UI가 가지고 있을 보호된 엘의 갯수 정보 초기화
// 			SetNumOfCrystalsProtected( 0 );
// 		}

// 		return true;
// 	}
// 
// 	return false;
}

// bool CX2UIWorldMission::SetShowEXPBonusDlg( bool bEnable )
// {
// 	if(bEnable == true)
// 	{
// 		if(m_pDlgEXPBonus == NULL)
// 		{
// 			m_pDlgEXPBonus = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_EXP_Bonus.lua" );
// 			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgEXPBonus );
// 
// 			m_pDlgEXPBonus->SetShowEnable( true, true );
// 		}
// 
// 		return true;
// 	}
// 	else
// 	{
// 		if(m_pDlgEXPBonus != NULL)
// 		{
// 			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgEXPBonus, NULL, false );
// 			m_pDlgEXPBonus = NULL;
// 		}
// 
// 		return true;
// 	}
// 
// 	return false;
// }

bool CX2UIWorldMission::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case WMUM_REWARD_CLOSE:
		{
			/*SetShowRewardDlg( false );*/
		}
		return true;
	default:
		break;
	}
	
	return false;
}

// void CX2UIWorldMission::SetReservedRewardItem( bool bEnable, int iItemID, int iQuantity )
// {
// 	if( iItemID < 0 )
// 		return;
// 
// 	if( iQuantity < 0 )
// 		return;
// 
// 	m_bReservedReward = bEnable;
// 	m_iRewardItemID = iItemID;
// 	m_iRewardQuantity = iQuantity;
// }

void CX2UIWorldMission::UpdateTimeDlg( int iTime )
{
	if( iTime < 0 )
	{
		iTime = 0;
	}

	if( m_pDlgTime == NULL )
	{
		SetShowTimeDlg( true );
	}

	int iMin = iTime / 60;
	int iSec = iTime % 60;

	const wstring MAGIC_TEXTURE_KEY[] = {	L"BG_NUM_0",
											L"BG_NUM_1",
											L"BG_NUM_2",
											L"BG_NUM_3",
											L"BG_NUM_4",
											L"BG_NUM_5",
											L"BG_NUM_6",
											L"BG_NUM_7",
											L"BG_NUM_8",
											L"BG_NUM_9" };

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-26
	CKTDGUIStatic* pStaticTime = (CKTDGUIStatic*) m_pDlgTime->GetControl( L"TIME" );
#else // SERV_NEW_DEFENCE_DUNGEON
	CKTDGUIStatic* pStaticTime = (CKTDGUIStatic*) m_pDlgTime->GetControl( L"time" );
#endif // SERV_NEW_DEFENCE_DUNGEON

	//	크래시 방지
	const UINT uiSizeOfTextureKey = ARRAY_SIZE( MAGIC_TEXTURE_KEY );

	int iMinTen			= iMin / 10 % 10;
	if ( uiSizeOfTextureKey <= iMinTen )
		iMinTen = 0;
	int iSecTen			= iSec / 10 % 10;
	if ( uiSizeOfTextureKey <= iSecTen )
		iSecTen = 0;

	if( pStaticTime != NULL )
	{
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-03-26

		const int MAGIC_MIN_TEN			= 0;
		const int MAGIC_MIN_ONE			= 1;
		const int MAGIC_SEC_TEN			= 3;
		const int MAGIC_SEC_ONE			= 4;

#else // SERV_NEW_DEFENCE_DUNGEON

		const int MAGIC_MIN_TEN			= 21;
		const int MAGIC_MIN_ONE			= 22;
		const int MAGIC_SEC_TEN			= 24;
		const int MAGIC_SEC_ONE			= 25;

#endif // SERV_NEW_DEFENCE_DUNGEON

		//	시간표시
		pStaticTime->GetPicture( MAGIC_MIN_TEN )->SetTex( L"DLG_UI_Common_Texture65_NEW.tga", MAGIC_TEXTURE_KEY[iMinTen].c_str() );
		pStaticTime->GetPicture( MAGIC_MIN_ONE )->SetTex( L"DLG_UI_Common_Texture65_NEW.tga", MAGIC_TEXTURE_KEY[iMin%10].c_str() );

// 		const int MAGIC_SHIELD	= 14;	//	방패아이콘
// 		const float MAGIC_POS_X	= 574.0f;
// 		const float MAGIC_POS_Y	= 46.0f;
// 		const int MAGIC_SIZE_X	= 127;

//		if( iProgress > 100 ) iProgress = 100;

		pStaticTime->GetPicture( MAGIC_SEC_TEN )->SetTex( L"DLG_UI_Common_Texture65_NEW.tga", MAGIC_TEXTURE_KEY[iSecTen].c_str() );
		pStaticTime->GetPicture( MAGIC_SEC_ONE )->SetTex( L"DLG_UI_Common_Texture65_NEW.tga", MAGIC_TEXTURE_KEY[iSec%10].c_str() );

		//	던전 클리어 진행도 표시
// 		pStaticTime->GetPicture( MAGIC_SHIELD )->SetPos( D3DXVECTOR2( MAGIC_POS_X + (MAGIC_SIZE_X * iProgress / 100), MAGIC_POS_Y ) );
// 
// 		wstringstream wstrstm;
// 		if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
// 		{
// 			CKTDGUIStatic* pStaticTime = (CKTDGUIStatic*) m_pDlgTime->GetControl( L"Static_Dummy" );
// 			wstrstm << L"Mission:" << g_pData->GetWorldMissionManager()->GetDungeonClearCount() << L"/" << g_pData->GetWorldMissionManager()->GetDungeonTargetCount() << L" (" << iProgress << L"%)";
// 
// 			pStaticTime->SetString(0, wstrstm.str().c_str());
// 			pStaticTime->SetShow( true );
// 		}
		
		//}} 허상형 : [2011/3/23/] //	임시
	}
}

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-15
void CX2UIWorldMission::UpdateNumOfCrystalsProtected( int iNumOfCrystalsProtected_ /*= 0*/, int iNumOfMaxCrystalsProtected_/*= 1*/ )
#else // SERV_NEW_DEFENCE_DUNGEON
void CX2UIWorldMission::UpdateNumOfCrystalsProtected( int iNumOfCrystalsProtected_ )
#endif // SERV_NEW_DEFENCE_DUNGEON
{
	// 이전의 UpdateTimeDlg에서 NULL 인 경우 다시 생성을 했으므로, 디버그 모드에서만 체크함
	ASSERT( NULL != m_pDlgTime );

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-26
	/// 어둠의 문 개편으로 인하여, 월드 미션 UI 표시 구조 싹 바뀜

	CKTDGUIStatic* pStaticTime = static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"WORLD_MISSION_NEW" ) );
	if ( NULL == pStaticTime )
	{
		ASSERT( ! L"Static Time Is NULL" );
		return;
	}

	CKTDGUIStatic* pStaticGageAngel		= static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"GAGE_ANGEL" ) );
	CKTDGUIStatic* pStaticGageDemon		= static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"GAGE_DEMON" ) );
	CKTDGUIStatic* pStaticGageParticle	= static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"GAGE_PARTICLE" ) );
	CKTDGUIStatic* pStaticDanger		= static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"DANGER" ) );

	if ( NULL == pStaticGageAngel || NULL == pStaticGageDemon || NULL == pStaticGageParticle || NULL == pStaticDanger )
	{
		ASSERT( ! L"World Mission  Gage UI Is NULL" );
		return;
	}

	CKTDGUIControl::CPictureData* pGageAngel		= pStaticGageAngel->GetPicture(0);
	CKTDGUIControl::CPictureData* pGageDemon		= pStaticGageDemon->GetPicture(0);
	CKTDGUIControl::CPictureData* pGageParticle		= pStaticGageParticle->GetPicture(0);
	CKTDGUIControl::CPictureData* pDanger = pStaticDanger->GetPicture(0);

	if ( NULL == pGageAngel || NULL == pGageDemon || NULL == pGageParticle || NULL == pDanger )
	{
		ASSERT( ! L"World Mission  Gage UI Pictire Is NULL" );
		return;
	}

	/// 서버에서 어둠의문 진행 최대 수치를 0으로 보내줬다면, 일단 1로 설정해 주자 ( 0이면 길이 연산 결과가 0이 되어서, 게이지가 출력되지 않는다. )
	if ( 0 == iNumOfMaxCrystalsProtected_ )
		iNumOfMaxCrystalsProtected_ = 1;

	/// 아군 게이지 설정

	/// 어둠의 문 현재 진행 % 에 따른  아군 게이지 길이 설정
	const float fGageWidth = _CONST_UI_WORLD_MISSION_::DEFENCE_DUNGEON_WORLD_MISSION_UI_GAGE_WIDTH;

	float fGageAngelWidth = fGageWidth + ( fGageWidth* ( static_cast<float>( iNumOfCrystalsProtected_ ) /  static_cast<float>( iNumOfMaxCrystalsProtected_ ) ) );

	if ( 0.f > fGageAngelWidth )
		fGageAngelWidth = 0.f;
	else if ( fGageWidth * 2 < fGageAngelWidth )
		fGageAngelWidth = fGageWidth * 2;

	pGageAngel->SetSize( D3DXVECTOR2( fGageAngelWidth, 13.f ) );


	/// 마족 게이지 설정
	D3DXVECTOR2 vGageAngelPos	=  pGageAngel->GetPos();
	float		iGageDemonWidth = ( fGageWidth * 2.f ) - fGageAngelWidth;		/// 마족 게이지 길이 연산
	pGageDemon->SetPos( D3DXVECTOR2( vGageAngelPos.x + fGageAngelWidth, vGageAngelPos.y ) );
	pGageDemon->SetSize( D3DXVECTOR2( iGageDemonWidth, 13.f ) );

	/// 가운데 대립중인 화살표 아미지 위치 설정
	D3DXVECTOR2 vGageParticlePos = pGageDemon->GetPos();
	pGageParticle->SetPos( D3DXVECTOR2( vGageParticlePos.x - 18.f, 02.f ) );

	/// 마족 측으로 넘어갔다면, Danger 마크 표시
	if ( 93.f > fGageAngelWidth && iNumOfCrystalsProtected_ != iNumOfMaxCrystalsProtected_ )
	{
		if ( false == GetIsFlickerDanger() )	/// Danger 마크가 생성되어 있지 않을 때
		{
			pStaticDanger->SetShow( true );		/// 컨트롤 표시
			SetIsFlickerDanger( true );			/// 생성 처리

			pDanger->SetFlicker(0.5f, 1.f, 0.f);		/// Danger 마크 깜빡임 설정
		}
	}
	/// 아군 측으로 넘어갔다면, Danger 마크 해제
	else
	{
		pStaticDanger->SetShow( false );		/// 컨트롤 해제
		SetIsFlickerDanger( false );			/// 해제 처리

		pDanger->SetFlicker(99999.f, 0.f, 0.f);		/// Danger 마크 깜빡임 설정
	}

#else // SERV_NEW_DEFENCE_DUNGEON

	// 지켜진 크리스탈의 갯수가 변경 됬으면
	if ( m_iNumOfCrystalsProtected != iNumOfCrystalsProtected_ ||
		true == m_bForceUpdate )
	{
		// UIStatic을 얻어옴
		CKTDGUIStatic* pStaticTime = static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"time" ) );
		if ( NULL == pStaticTime )
		{
			ASSERT( ! L"Static Time Is NULL" );
			return;
		}

		// 모든 엘의 수정을 다 꺼줌
		// 색깔이 있는 크리스탈은 MAGIC_CRYSTAL_PICTURE_START_INDEX + 1에서 부터 시작 한다.
		const int MAGIC_CRYSTAL_PICTURE_START_INDEX = 5;
		const int MAGIC_CRYSTAL_PICTURE_END_INDEX = 20;

		for ( int i = MAGIC_CRYSTAL_PICTURE_START_INDEX + 1; i <= MAGIC_CRYSTAL_PICTURE_END_INDEX; ++i )
		{
			SetShowCrystalPictureData( pStaticTime, i, false );
		}

		// 지켜진 것이 있다면
		if ( 0 != iNumOfCrystalsProtected_ )
		{
			// 색깔이 있는 크리스탈은 MAGIC_CRYSTAL_PICTURE_START_INDEX + 1에서 부터 시작 한다.
			const int MAGIC_CRYSTAL_PICTURE_START_INDEX = 5;
			const int MAGIC_CRYSTAL_PICTURE_END_INDEX = MAGIC_CRYSTAL_PICTURE_START_INDEX + iNumOfCrystalsProtected_;

			for ( int i = MAGIC_CRYSTAL_PICTURE_START_INDEX + 1; i <= MAGIC_CRYSTAL_PICTURE_END_INDEX; ++i )
			{
				SetShowCrystalPictureData( pStaticTime, i, true );
			}
		}
		m_iNumOfCrystalsProtected = iNumOfCrystalsProtected_ ;
		//SetNumOfCrystalsProtected( iNumOfCrystalsProtected_ );
		/*
		// UI 상의 크리스탈은 한개당 3등분으로 표시되고, 전체 5개가 있음

		// color로 다 채워질 크리스탈의 갯수
		const int iNumOfCrystalsWithFullColor = iNumOfCrystalsProtected_ / 3;	

		// 부분 칼라로 채워 질 크리스탈이 얼마나 채워질 것인지
		const int iNumOfCrystalsWithPartialColor = iNumOfCrystalsProtected_ % 3;

		// UIStatic을 얻어옴
		CKTDGUIStatic* pStaticTime = static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"time" ) );
		if ( NULL == pStaticTime )
		{
			ASSERT( ! L"Static Time Is NULL" );
			return;
		}

		// iNumOfCrystalsWithFullColor에 값이 있는 경우, FullColor의 그림을 SetShow, true로 지정 해주고
		for ( int i = 1; i <= iNumOfCrystalsWithFullColor; ++i )
		{
			const int iIndexOfFullColor = MAGIC_CRYSTAL_PICTURE_START_INDEX + i * 3;

			SetShowCrystalPictureData( pStaticTime, iIndexOfFullColor, true );
			// 2/3에 해당하는 그림은 꺼준다.
			SetShowCrystalPictureData( pStaticTime, iIndexOfFullColor - 2, false );
			// 1/3에 해당하는 그림은 꺼준다.
			SetShowCrystalPictureData( pStaticTime, iIndexOfFullColor - 1, false );
		}

		// iNumOfCrystalsWithFullColor에도 값이 있으면, 1/3, 2/3에 해당하는 그림을 true 시켜준다.
		if ( 0 != iNumOfCrystalsWithPartialColor )
		{
			const int iIndexOfPartialColor = MAGIC_CRYSTAL_PICTURE_START_INDEX + iNumOfCrystalsWithFullColor * 3 + iNumOfCrystalsWithPartialColor;
			SetShowCrystalPictureData( pStaticTime, iIndexOfPartialColor, true );
		}
		*/

	}

#endif // SERV_NEW_DEFENCE_DUNGEON
}

void CX2UIWorldMission::SetShowCrystalPictureData( CKTDGUIStatic* pStaticTime_, int iIndexOfColor_, bool bShow_ )
{
	CKTDGUIControl::CPictureData* pPictureData = 
		pStaticTime_->GetPicture( iIndexOfColor_ );

	if ( NULL == pPictureData )
	{
		ASSERT( ! L"PictureData Is NULL" );
		return;
	}

	pPictureData->SetShow( bShow_ );
}
#endif	SERV_INSERT_GLOBAL_SERVER
//}} 허상형 : [2011/3/22/] //	월드 미션 UI


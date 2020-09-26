#include "stdafx.h"
#include ".\X2GageUI.h"

//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akStates[] = 
{
	KRenderState( D3DRS_ALPHATESTENABLE,	TRUE ),
	KRenderState( D3DRS_ALPHAFUNC,			D3DCMP_NOTEQUAL ),
	KRenderState( D3DRS_ALPHAREF,			0 ),

	KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
	KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
	KRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA ),

	KRenderState( D3DRS_ZENABLE,			FALSE ),
	KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
	KRenderState( D3DRS_CULLMODE,			D3DCULL_CW ),

	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE ),
};//s_akStates[]
//}} robobeg : 2008-10-13

/*virtual*/ CX2GageUI::~CX2GageUI()
{
	//m_optrGameUnit.Reset();
}

const float CX2BossGageUI::MAGIC_LEFT_HP_BAR_LENGTH = 11.f;
const float CX2BossGageUI::MAGIC_HP_BAR_LENGTH = 546.f + 11.f;
const WCHAR CX2BossGageUI::WCHAR_HP_BAR_POST_FIX[KIND_OF_HP_GAGE_BAR_COLOR][MAX_NUM_OF_POST_FIX_COLOR] = { L"_V", L"_PB", L"_B", L"_G", L"_YG", L"_O", L"" };

CX2BossGageUI::CX2BossGageUI( CX2GameUnit* pGameUnit_ ) : CX2GageUI(),
	m_optrGameUnit( pGameUnit_ ), m_pDLGBossGage( NULL ), m_usNumOfGage( 0 )
{
	if ( null != m_optrGameUnit )
	{
		SetNumOfGage( static_cast<CX2GUNPC*>( m_optrGameUnit.GetObservable() )->GetNumOfGage() );
	}
}

/*virtual*/ CX2BossGageUI::~CX2BossGageUI()
{
	m_optrGameUnit.Reset();
	SAFE_DELETE_DIALOG( m_pDLGBossGage );
}


/*virtual*/ void CX2BossGageUI::InitUI()
{
	SAFE_DELETE_DIALOG( m_pDLGBossGage );
	m_pDLGBossGage = new CKTDGUIDialog( NULL, L"DLG_BossState_NEW.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBossGage );
#ifdef SKILL_SLOT_UI_TYPE_B
	if( NULL != g_pMain )
	{
		D3DXVECTOR2 vPos(207,100);
		m_bSkillSlotUITypeA = g_pMain->GetGameOption().GetIsSkillUITypeA();

		if( false == m_bSkillSlotUITypeA )
		{
			vPos.y += 44.f;
		}
		m_pDLGBossGage->SetPos(vPos);		
	}
#endif //SKILL_SLOT_UI_TYPE_B

	SetShow( false );
}

#ifdef SKILL_SLOT_UI_TYPE_B
void CX2BossGageUI::ResetBossUIPos()
{
	if( NULL != g_pMain )
	{
		D3DXVECTOR2 vPos(207,100);
		if( false == g_pMain->GetGameOption().GetIsSkillUITypeA() )
			vPos.y += 44.f;

		m_pDLGBossGage->SetPos(vPos);		
	}
}
#endif //SKILL_SLOT_UI_TYPE_B

void CX2BossGageUI::SetBossGageTexture( const WCHAR* wszFaceTexName_, const WCHAR* wszFaceTexPieceName_ )
{
	if ( NULL != m_pDLGBossGage )
	{
		CKTDGUIStatic* pStatic_BossFace = static_cast<CKTDGUIStatic*>( m_pDLGBossGage->GetControl( L"Boss_State_Face" ) );
		string strFaceTexName, strFaceTexPieceName;
		ConvertWCHARToChar( strFaceTexName, wszFaceTexName_ );
		ConvertWCHARToChar( strFaceTexPieceName, wszFaceTexPieceName_ );
		if ( pStatic_BossFace != NULL && pStatic_BossFace->GetPicture( 0 ) != NULL )
			pStatic_BossFace->GetPicture(0)->SetTex_LUA( strFaceTexName.c_str(), strFaceTexPieceName.c_str() );
	}
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BossGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BossGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( GetShow() && null != m_optrGameUnit)
	{
	#ifdef SKILL_SLOT_UI_TYPE_B
		if( NULL != g_pMain )
		{
			if( m_bSkillSlotUITypeA != g_pMain->GetGameOption().GetIsSkillUITypeA() )
			{
				m_bSkillSlotUITypeA = g_pMain->GetGameOption().GetIsSkillUITypeA();
				ResetBossUIPos();
			}
		}
	#endif //SKILL_SLOT_UI_TYPE_B
		//*
		CKTDGUIStatic* pStatic_BossState = static_cast<CKTDGUIStatic*>( m_pDLGBossGage->GetControl( L"Boss_State" ) );

		if ( NULL != pStatic_BossState )
		{

			//{{ JHKang / 강정훈 / 2011/01/26 / HP 계산 // SERV_BOSS_GAUGE_HP_LINES
						
			//{{ 색상 카운터
			const float fMaxHp = m_optrGameUnit->GetMaxHp();
			const float fNowHp = m_optrGameUnit->GetNowHp();

			float fPartialHP = fMaxHp / GetNumOfGage();
			int iLineNumber = static_cast<int>( fNowHp / fPartialHP);
			short sColor = (KIND_OF_HP_GAGE_BAR_COLOR - 2) - ( iLineNumber - 1 ) % (KIND_OF_HP_GAGE_BAR_COLOR - 1);

			float fHPPercent = 1.f;
			float fHPBarLength = 1.f;

			if ( iLineNumber >= 1 )
			{
				fHPPercent = fNowHp / fPartialHP - static_cast<float>(iLineNumber);
				fHPBarLength = MAGIC_HP_BAR_LENGTH * fHPPercent;
			}
			if ( iLineNumber == 0 )
			{
				sColor = KIND_OF_HP_GAGE_BAR_COLOR - 1;
				fHPBarLength = MAGIC_HP_BAR_LENGTH * fNowHp / fPartialHP;
			}
			if ( GetNumOfGage() == iLineNumber )	// 뱅가드 및 소환되는 몬스터 고려
			{
				--iLineNumber;
				sColor = (KIND_OF_HP_GAGE_BAR_COLOR - 2) - ( iLineNumber - 1 ) % (KIND_OF_HP_GAGE_BAR_COLOR - 1);

				if ( 0 > sColor )
					sColor = 0;
				else if ( KIND_OF_HP_GAGE_BAR_COLOR <= sColor )
					sColor = KIND_OF_HP_GAGE_BAR_COLOR - 1;

				fHPBarLength = MAGIC_HP_BAR_LENGTH;
			}
			//}} 색상 카운터

			//{{ 변화 되는 HP Bar
			wstring tempString( L"BOSS_HP1" );
			tempString += WCHAR_HP_BAR_POST_FIX[sColor];
			
			bool bExistNull = false;
			CKTDGUIControl::CPictureData* pHPBar_Left = pStatic_BossState->GetPicture(5);
			if ( NULL != pHPBar_Left )
			{
				pHPBar_Left->SetTex( L"DLG_BossState.TGA", tempString.c_str() );
				pHPBar_Left->SetShow( true );
			}
			else
				bExistNull = true;

			tempString.clear();
			tempString = L"BOSS_HP2";
			tempString += WCHAR_HP_BAR_POST_FIX[sColor];

			CKTDGUIControl::CPictureData* pHPBar_Middle = pStatic_BossState->GetPicture(6);
			if ( NULL != pHPBar_Middle )
			{
				pHPBar_Middle->SetTex( L"DLG_BossState.TGA", tempString.c_str() );
				pHPBar_Middle->SetShow( true );
			}
			else
				bExistNull = true;
			if ( !bExistNull )
			{
				if( fHPBarLength < MAGIC_LEFT_HP_BAR_LENGTH )
				{
					pHPBar_Left->SetSizeX( fHPBarLength );
					pHPBar_Middle->SetSizeX( 0.f );
				}
				else
				{
					pHPBar_Left->SetSizeX( MAGIC_LEFT_HP_BAR_LENGTH );
					pHPBar_Middle->SetSizeX( fHPBarLength - MAGIC_LEFT_HP_BAR_LENGTH );
				}
			}	

			if ( iLineNumber >= 1 && fMaxHp != fNowHp )
			{
				short sBottomBarColor = sColor + 1;
				
				if ( 0 > sBottomBarColor )
					sBottomBarColor = 0;
				else if ( KIND_OF_HP_GAGE_BAR_COLOR <= sBottomBarColor )
					sBottomBarColor = KIND_OF_HP_GAGE_BAR_COLOR - 1;

				//{{ 다음 색으로 표시되는 HP Bar
				tempString = L"BOSS_HP1";
				tempString += WCHAR_HP_BAR_POST_FIX[sBottomBarColor];

				CKTDGUIControl::CPictureData* pHPBar_Left2 = pStatic_BossState->GetPicture(3);
				if ( NULL != pHPBar_Left2 )
				{
					pHPBar_Left2->SetTex( L"DLG_BossState.TGA", tempString.c_str() );
					pHPBar_Left2->SetSizeX( MAGIC_LEFT_HP_BAR_LENGTH );
					pHPBar_Left2->SetShow( true );
				}

				tempString.clear();
				tempString = L"BOSS_HP2";
				tempString += WCHAR_HP_BAR_POST_FIX[sBottomBarColor];

				CKTDGUIControl::CPictureData* pHPBar_Middle2 = pStatic_BossState->GetPicture(4);
				if ( NULL != pHPBar_Middle2 )
				{
					pHPBar_Middle2->SetTex( L"DLG_BossState.TGA", tempString.c_str() );
					pHPBar_Middle2->SetSizeX( MAGIC_HP_BAR_LENGTH - MAGIC_LEFT_HP_BAR_LENGTH );
					pHPBar_Middle2->SetShow( true );
					tempString.clear();
					//}} 다음 색으로 표시되는 HP Bar				
				}				
			}
			else
			{
				CKTDGUIControl::CPictureData* pHPBar_Left2 = pStatic_BossState->GetPicture(3);
				if ( NULL != pHPBar_Left2 )
					pHPBar_Left2->SetShow( false );

				CKTDGUIControl::CPictureData* pHPBar_Middle2 = pStatic_BossState->GetPicture(4);
				if ( NULL != pHPBar_Middle2 )
					pHPBar_Middle2->SetShow( false );
			}

			u_short usBarNumber = iLineNumber + 1;

			if ( usBarNumber > GetNumOfGage() )
				usBarNumber = GetNumOfGage();

			// Bar 라인 수 숫자 표시
			if ( fNowHp <= 0 )
			{
				usBarNumber = 0;
			}

			CKTDGUIControl::CPictureData* pLineX			= pStatic_BossState->GetPicture(10);				
			if ( NULL != pLineX )
			{
				CKTDGUIControl::CPictureData* pLineNumber100	= pStatic_BossState->GetPicture(9);				
				if ( NULL != pLineNumber100 )
				{
					if ( usBarNumber >= 100 )
					{
						int iNumHundred = usBarNumber / 100;
						usBarNumber = usBarNumber % 100;
						SetNumberIcon( pLineNumber100, iNumHundred );
						pLineNumber100->SetShow( true );
					}
					else
					{
						pLineNumber100->SetShow( false );
						pLineX->pos.x = pLineNumber100->pos.x;
					}
				}
				CKTDGUIControl::CPictureData* pLineNumber10		= pStatic_BossState->GetPicture(8);				
				if ( NULL != pLineNumber10 )
				{
					if ( usBarNumber >= 10 )
					{
						int iNumTen = usBarNumber / 10;
						SetNumberIcon( pLineNumber10, iNumTen );
						pLineNumber10->SetShow( true );
					}
					else
					{
						pLineNumber10->SetShow( false );
						pLineX->pos.x =  pLineNumber10->pos.x;
					}
				}
				CKTDGUIControl::CPictureData* pLineNumber1		= pStatic_BossState->GetPicture(7);				
				if ( NULL != pLineNumber1 )
				{
					if ( usBarNumber >= 0 )
					{
						int iNumOne = usBarNumber % 10;
						SetNumberIcon( pLineNumber1, iNumOne );
						pLineNumber1->SetShow( true );
					}
				}

				pLineX->SetShow( true );
			}
			//}} JHKang / 강정훈 / 2011/01/26 / HP 계산 // SERV_BOSS_GAUGE_HP_LINES
			
		}
		//*/
		
	}
	
}

void CX2BossGageUI::SetNumberIcon( CKTDGUIControl::CPictureData* pPictureData_, const int iNum_ )
{
	switch ( iNum_ )
	{
	case 0:
		pPictureData_->SetTex( L"PC_0.dds" );
		break;

	case 1:
		pPictureData_->SetTex( L"PC_1.dds" );
		break;

	case 2:
		pPictureData_->SetTex( L"PC_2.dds" );
		break;

	case 3:
		pPictureData_->SetTex( L"PC_3.dds" );
		break;

	case 4:
		pPictureData_->SetTex( L"PC_4.dds" );
		break;

	case 5:
		pPictureData_->SetTex( L"PC_5.dds" );
		break;

	case 6:
		pPictureData_->SetTex( L"PC_6.dds" );
		break;

	case 7:
		pPictureData_->SetTex( L"PC_7.dds" );
		break;

	case 8:
		pPictureData_->SetTex( L"PC_8.dds" );
		break;

	case 9:
		pPictureData_->SetTex( L"PC_9.dds" );
		break;

	default:
		ASSERT( false );
		break;
	}
}

bool CX2BossGageUI::IsOwnerGameUnit( const UidType uidBoss_ )
{
	if ( null == m_optrGameUnit || uidBoss_ != m_optrGameUnit->GetUnitUID() )
		return false;
	else
		return true;
}


CX2SmallGageUI::CX2SmallGageUI( CX2GameUnit* pGameUnit_ )
	: CX2GageUI(), m_optrGameUnit( pGameUnit_ ), m_RenderStateID( s_akStates ),
	m_pHPSmallBar( NULL ), m_pMPSmallBar( NULL ), m_fMPGageAlpha( 1.0f )
{
}

/*virtual*/ CX2SmallGageUI::~CX2SmallGageUI()
{
	m_optrGameUnit.Reset();
	SAFE_CLOSE( m_pHPSmallBar );
	SAFE_CLOSE( m_pMPSmallBar );
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2SmallGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2SmallGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if( true == GetShow() )
	{
		if ( GetAlpha() >= 255.0f )
			SetAlpha( 255.0f );
		else
        {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            SetAlpha( GetAlpha() + fElapsedTime_ * 400 );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			SetAlpha( GetAlpha() + g_pKTDXApp->GetElapsedTime() * 400 );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }
	}
	else
	{
		if ( GetAlpha() <= 0.0f )
			SetAlpha( 0.0f );
		else
        {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            SetAlpha( GetAlpha() - fElapsedTime_ * 400 );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			SetAlpha( GetAlpha() - g_pKTDXApp->GetElapsedTime() * 400 );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }
	}
}

/*virtual*/ void CX2SmallGageUI::OnFrameRender()
{
	// 포커스 유닛이 있는 없는지는 위에서 계산
	if ( GetShow() )
		DrawSmallGage();
}



void CX2SmallGageUI::DrawFace( float fX_, float fY_, float fWidth_, float fHeight_, D3DCOLOR color_ /* = 0xffffffff */ )
{
	fWidth_		*= g_pKTDXApp->GetResolutionScaleX();
	fHeight_	*= g_pKTDXApp->GetResolutionScaleY();
	
	float X1 = fX_ - 0.5f;
	float Y1 = fY_ - 0.5f;
	float X2 = fX_ + fWidth_;
	float Y2 = fY_ + fHeight_;

	float UMax = 1.0f;
	float VMax = 1.0f;

	DRAWFACE_RHW_VERTEX vertex[4];
	ZeroMemory( vertex, sizeof(DRAWFACE_RHW_VERTEX) * 4 );

	//Set up the 4 corners of a small square
	vertex[0].x = X1;     vertex[0].y = Y1;
	vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;
	vertex[0].u = 0.0f;   vertex[0].v = 0.0f;

	vertex[1].x = X1;     vertex[1].y = Y2;
	vertex[1].u = 0.0f;   vertex[1].v = VMax;
	vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;

	vertex[2].x = X2;     vertex[2].y = Y1;
	vertex[2].u = UMax;   vertex[2].v = 0.0f;
	vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;

	vertex[3].x = X2;     vertex[3].y = Y2;
	vertex[3].u = UMax;   vertex[3].v = VMax;
	vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;

	vertex[0].color = vertex[1].color = 
	vertex[2].color = vertex[3].color = color_;

	KD3DPUSH( m_RenderStateID )

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_DRAWFACE_RHW_VERTEX == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1
		, D3DPT_TRIANGLESTRIP, 2, vertex );
//#else
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_DRAWFACE_RHW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(DRAWFACE_RHW_VERTEX) );
//#endif

	KD3DEND()
}

void CX2SmallGageUI::DrawSmallGage()
{
	if ( NULL != m_pHPSmallBar || NULL != m_pMPSmallBar )
	{
		D3DXVECTOR3 vGameUnitPos( m_optrGameUnit->GetPos() );
		vGameUnitPos.y -= 7.0f;

		D3DVIEWPORT vp;
		D3DXMATRIX matWorld, matView, matProj;

		vp.X = 0;
		vp.Y = 0;
		vp.Width  = (DWORD)( g_pKTDXApp->GetViewport().Width );
		vp.Height = (DWORD)( g_pKTDXApp->GetViewport().Height );

		g_pKTDXApp->GetProjectionTransform( &matProj );
		g_pKTDXApp->GetViewTransform( &matView );
		D3DXMatrixIdentity( &matWorld );

		D3DXVECTOR3 vOut( 0.0f, 0.0f, 0.0f );
		D3DXVec3Project( &vOut, &vGameUnitPos, &vp, &matProj, &matView, &matWorld );

		vOut.x -= ( 25 * g_pKTDXApp->GetResolutionScaleX() );
		vOut.y += ( 28 * g_pKTDXApp->GetResolutionScaleY() );

		DrawHpGage( vOut );

		if ( NULL != m_pMPSmallBar )
		{
			switch ( g_pX2Game->GetGameType() )
			{
			case CX2Game::GT_DUNGEON:
			case CX2Game::GT_BATTLE_FIELD:
			default:
				{
					if ( CX2GameUnit::GUT_NPC == m_optrGameUnit->GetGameUnitType() )
					{
						vOut.y += ( 5 * g_pKTDXApp->GetResolutionScaleY() );
						DrawMpGage( vOut );
					}

				} break;

			case CX2Game::GT_PVP:
				{
					if( NULL != g_pX2Game && 
						NULL != g_pX2Game->GetMyUnit() )
					{
						if( ( NULL != g_pX2Room->GetMySlot() && g_pX2Room->GetMySlot()->m_bObserver == true ) ||
							true == g_pX2Game->GetMyUnit()->GetCashItemAbility().m_bShowOppnentMP
							&& g_pX2Game->GetMyUnit()->GetTeam() != m_optrGameUnit->GetTeam() )
						{
							vOut.y += ( 5 * g_pKTDXApp->GetResolutionScaleY() );
							DrawMpGage( vOut );
						}
					}
					//}} dmlee 2008.04.11 캐시템 상대팀 MP 보기
				} break;
			}
		}	
	} // if ( GetShow() )
}

void CX2SmallGageUI::DrawHpGage( const D3DXVECTOR3& vOut_ )
{
	CKTDGStateManager::SetTexture( 0, NULL );

	DrawFace( vOut_.x, vOut_.y, 50.0f, 7.0f, D3DCOLOR_ARGB( static_cast<int>( GetAlpha() ), 0, 0, 0 ) );		// 검은색 게이지

	const float fNowHp = m_optrGameUnit->GetNowHp();
	const float fMaxHp = m_optrGameUnit->GetMaxHp();

	if( fNowHp > 0.0f )
	{
		if( m_pHPSmallBar != NULL )
			m_pHPSmallBar->SetDeviceTexture();

		const float fNowHpPercent = (48.0f * fNowHp) / fMaxHp;
		DrawFace( vOut_.x+1.0f, vOut_.y+1.0f, fNowHpPercent, 5.0f, 
			D3DCOLOR_ARGB( static_cast<int>( GetAlpha() ), 255, 255, 255 ) );
	}
}

void CX2SmallGageUI::DrawMpGage( const D3DXVECTOR3& vOut_ )
{
	const float fNowMp = m_optrGameUnit->GetNowMp();
	const float fMaxMp = m_optrGameUnit->GetMaxMp();

	CKTDGStateManager::SetTexture( 0, NULL );
	DrawFace( vOut_.x, vOut_.y, 50.0f, 7.0f, D3DCOLOR_ARGB( static_cast<int>( GetAlpha() ), 0, 0, 0 ) );

	if ( fNowMp > 0.0f )
	{
		if( m_pMPSmallBar != NULL )
			m_pMPSmallBar->SetDeviceTexture();

		float fNowMpPercent = (48.0f * fNowMp) / fMaxMp;

		if ( fNowMp < fMaxMp )
			m_fMPGageAlpha = 1.0f;
		else
		{
			m_fMPGageAlpha -= 0.03f;

			if ( 0.0f > m_fMPGageAlpha )
				m_fMPGageAlpha = 1.0f;					
		}
		DrawFace( vOut_.x + 1.0f, vOut_.y + 1.0f, fNowMpPercent, 5.0f, 
			D3DCOLOR_ARGB( static_cast<int>( GetAlpha() * m_fMPGageAlpha), 255, 255, 255 ) );
	}
}

void CX2SmallGageUI::SetHpTexture(  const WCHAR* pTexName_ )
{
	SAFE_CLOSE( m_pHPSmallBar );
	m_pHPSmallBar = g_pKTDXApp->GetDeviceManager()->OpenTexture( pTexName_ );
}

void CX2SmallGageUI::SetMpTexture(  const WCHAR* pTexName_ )
{
	SAFE_CLOSE( m_pMPSmallBar );
	m_pMPSmallBar = g_pKTDXApp->GetDeviceManager()->OpenTexture( pTexName_ );
}

CX2MySmallGageUI::CX2MySmallGageUI( CX2GameUnit* pGameUnit_ ) 
	: CX2GageUI(), m_optrGameUnit( pGameUnit_ ), m_pDlgAirGage( NULL )
{
}

CX2MySmallGageUI::~CX2MySmallGageUI()
{
	DestroyAirGage();
}

/*virtual*/ void CX2MySmallGageUI::InitUI()
{
	SetShow( false );
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2MySmallGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2MySmallGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( null != m_optrGameUnit )
	{
		// 게이지 UI 가 없다면
		if ( NULL == m_pDlgAirGage )
		{
			// UI가 생성되어야 하면 생성
			if ( true == m_optrGameUnit->GetUnderWater() && m_optrGameUnit->GetNowHp() > 0.0f )
				CreateAirGage();
		}
		else	// 게이지 UI가 있다면
		{
			// 물안에 있고, 살아 있다면
			if ( true == m_optrGameUnit->GetUnderWater() && m_optrGameUnit->GetNowHp() > 0.0f )
				UpdateAirGage();
			else	// 그렇지 않으면 UI 삭제
				DestroyAirGage();
		}		
	}
}

/*virtual*/ bool CX2MySmallGageUI::GetShow() const
{
	return ( NULL == m_pDlgAirGage ? false : m_pDlgAirGage->GetShow() );
}

/*virtual*/ void CX2MySmallGageUI::SetShow( const bool bShow_ )
{
	CX2GageUI::SetShow( bShow_ );
	if ( NULL != m_pDlgAirGage )
		m_pDlgAirGage->SetShow(	bShow_ );
}

void CX2MySmallGageUI::CreateAirGage()
{
	m_pDlgAirGage = new CKTDGUIDialog( NULL, L"DLG_UI_AIR_GAGE.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgAirGage );
	SetShow( true );
}

void CX2MySmallGageUI::DestroyAirGage()
{
	SAFE_DELETE_DIALOG( m_pDlgAirGage );
}

void CX2MySmallGageUI::UpdateAirGage()
{
	// 업데이트
	const D3DXVECTOR3& vUserPos = m_optrGameUnit->GetPos();
	D3DXVECTOR2 vAirGagePos = g_pKTDXApp->GetProj3DPos( D3DXVECTOR3( vUserPos.x, vUserPos.y + 400.0f, vUserPos.z ) );

	vAirGagePos.x /= g_pKTDXApp->GetResolutionScaleX();
	vAirGagePos.x -= 25.f;

	vAirGagePos.y /= g_pKTDXApp->GetResolutionScaleY();

	m_pDlgAirGage->SetPos(vAirGagePos);
	SetShow( true );

	CX2GUUser* pUser = static_cast<CX2GUUser*>( m_optrGameUnit.GetObservable() );
	int iGage = static_cast<int>( ceil( pUser->GetOxygenGage() * 0.06f ) );

	if ( iGage > 12 )
		iGage = 12;
	if ( iGage < 0 )
		iGage = 0;

	CKTDGUIStatic* pStaticAir = (CKTDGUIStatic*)m_pDlgAirGage->GetControl( L"g_pStatic_AIR_GAGE_BAR" );
	if( pStaticAir != NULL )
	{
		WCHAR keyName[50];
		ZeroMemory( keyName, sizeof(WCHAR) * ARRAY_SIZE( keyName ) );

		wsprintf(keyName, L"AIR_%02d", iGage);
		pStaticAir->GetPicture(0)->SetTex( L"DLG_AIR_GAGE.tga", keyName );
	}
}

/*virtual*/ CX2StatusGageUI::~CX2StatusGageUI()
{
	m_pGageData = NULL;
#ifdef BUFF_ICON_UI
	SAFE_DELETE_DIALOG( m_pDlgBuffIcon );
#endif //BUFF_ICON_UI
}

void CX2StatusGageUI::UpdateGageForUV( IN CKTDGUIStatic* pStaticBar_, IN int iPicNum, IN const WCHAR* pUVName_, IN float fPercent_, IN bool isWidth_ )
{
	CKTDGUIControl::CPictureData* pPicture = pStaticBar_->GetPicture( iPicNum );
	if ( NULL != pPicture && NULL != pPicture->pTexture && NULL != pPicture->pTexture->pTexture )
	{
		const CKTDXDeviceTexture::TEXTURE_UV* pTexUV = pPicture->pTexture->pTexture->GetTexUV( pUVName_ );
		float fRate = 1.0f;

		if ( NULL != pTexUV )
		{
			if ( true == isWidth_ )
			{
				const float fGageUVWidth = (pTexUV->rightTop.x - pTexUV->leftTop.x)  * fPercent_;

				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]		= pTexUV->leftTop;
				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]		= pTexUV->rightTop;;
				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]	= pTexUV->leftBottom;;
				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]	= pTexUV->rightBottom;;

				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP].x	= pTexUV->leftTop.x + fGageUVWidth;
				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM].x	= pTexUV->leftBottom.x + fGageUVWidth;
				pPicture->pTexture->SetTextureUV();

				fRate = pPicture->GetOriginalSize().x * fPercent_;
				pPicture->SetSizeX( fRate );
			}
			else
			{
				const float fGageUVWidth = (pTexUV->leftBottom.y - pTexUV->leftTop.y)  * fPercent_;

				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]		= pTexUV->leftTop;
				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]		= pTexUV->rightTop;
				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]	= pTexUV->leftBottom;
				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]	= pTexUV->rightBottom;

				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP].y		= pTexUV->leftBottom.y - fGageUVWidth;
				pPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP].y	= pTexUV->rightBottom.y - fGageUVWidth;
				pPicture->pTexture->SetTextureUV();

				fRate = pPicture->GetOriginalSize().y * fPercent_;
				pPicture->SetSizeY( fRate, true );
			}
		}
	}
}

#ifdef BUFF_ICON_UI
/** @function : PushBuff
	@brief : GameUnit에서 새로운 버프가 등록되었을 때 UI출력에 사용 되는 버프 리스트 갱신
	@param : 해당 버프 ID,텍스쳐명(BuffIconInfo_), 버프,디버프 구분(bIsDeBuff)
*/
void CX2StatusGageUI::PushBuff(const BuffIcon& BuffIconInfo_, bool bIsDeBuff_)
{
	BUFF_TEMPLET_ID eBuffID = BuffIconInfo_.eBuffID;
	auto FindFunc = [eBuffID](const BuffIcon& BuffIconInfo ) {
		return BuffIconInfo.eBuffID == eBuffID;
	};

	vector<BuffIcon>* pVecBuffList = NULL;
	(false == bIsDeBuff_) ? pVecBuffList = &m_vecBuffList : pVecBuffList = &m_vecDebuffList;
	
	//기존에 등록되지 않은 버프 일 때만 추가
	if( std::_Find_if( pVecBuffList->begin(),pVecBuffList->end(), FindFunc) == pVecBuffList->end() )
	{
		pVecBuffList->push_back(BuffIconInfo_);
		UpdateBuffIcon();
	}
	else //기존에 등록되어 있었다면 플리커 정보만 초기화
	{	
		InitBuffIconFlicker( BuffIconInfo_.eBuffID );
	}
}
/** @function : EraseBuff
	@brief : GameUnit에서 버프가 제거될 때 UI출력에 사용 되는 버프 리스트 갱신
	@param : 버프가 갱신된 유닛UID(uiUnitUID), 해당 버프 ID(eBuffID_), 버프,디버프 구분(bIsDeBuff)
*/
void CX2StatusGageUI::EraseBuff(BUFF_TEMPLET_ID eBuffID_, bool bIsDeBuff_)
{
	auto FindFunc = [eBuffID_](const BuffIcon& BuffIconInfo_ ) {
		return BuffIconInfo_.eBuffID == eBuffID_;
	};

	vector<BuffIcon>* pVecBuffList = NULL;
	(false == bIsDeBuff_) ? pVecBuffList = &m_vecBuffList : pVecBuffList = &m_vecDebuffList;

	vector<BuffIcon>::iterator it = std::_Find_if( pVecBuffList->begin(),pVecBuffList->end(), FindFunc) ;
	if( it != pVecBuffList->end() )
	{
		if( NULL != m_pDlgBuffIcon )
		{
			/// 버프 이미지 제거
			WCHAR szStaticName[20];
			StringCchPrintfW( szStaticName, 20, L"Buff_%d", it->eBuffID );

			CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDlgBuffIcon->GetControl(szStaticName)) ;

			if( NULL != pStatic )
			{
				pStatic->SetShow(false);
			}

			/// 툴팁용 투명 버튼 제거
			WCHAR szButtonName[20];
			StringCchPrintfW( szButtonName, 20, L"ButtonBuff_%d", it->eBuffID );

			CKTDGUIButton* pButtonBuffIcon = static_cast<CKTDGUIButton*>( m_pDlgBuffIcon->GetControl( szButtonName ) ) ;

			if( NULL != pButtonBuffIcon )
			{
				m_pDlgBuffIcon->DeleteControl( szButtonName );
			}
		}

		pVecBuffList->erase(it);

		UpdateBuffIcon();
	}
}

/** @function : ClearBuffList
	@brief : UI출력에 사용되는 버프 리스트 초기화
	@param : 버프가 갱신된 유닛UID(uiUnitUID)
*/
void CX2StatusGageUI::ClearBuffList()
{
	m_vecBuffList.clear(); 
	m_vecDebuffList.clear();

	SAFE_DELETE_DIALOG(m_pDlgBuffIcon);
}

/** @function : SetBuffIconStatic
	@brief : 버프아이콘 UI STATIC추가
	@param : 해당 버프 ID,텍스쳐명(BuffIconInfo_), 출력 위치를 계산하기 위한 index(iIndex), 버프,디버프 구분(bIsDeBuff)
*/
void CX2StatusGageUI::SetBuffIconStatic(const BuffIcon& BuffIconInfo_, int iIndex_, bool bIsDebuff_, const D3DXVECTOR2& vSize_ )
{
	if( NULL != m_pDlgBuffIcon )
	{
		/// 툴팁용 투명 버튼 생성
		WCHAR szSButtonName[20];
		StringCchPrintfW( szSButtonName, 20, L"ButtonBuff_%d", BuffIconInfo_.eBuffID);
		CKTDGUIButton* pButtonBuffIcon = NULL;

		if( false == m_pDlgBuffIcon->CheckControl( szSButtonName ) )		/// 생성된 적 없는 버튼이면, 새로 생성
		{
			pButtonBuffIcon = m_pDlgBuffIcon->CreateButton();		/// 버튼 생성

			pButtonBuffIcon->SetName( szSButtonName );
			pButtonBuffIcon->SetSize( vSize_ );

			pButtonBuffIcon->SetNormalTex( L"DLG_Common_New_Texture03.tga", L"INVISIBLE" );
			pButtonBuffIcon->SetOverTex( L"DLG_Common_New_Texture03.tga", L"INVISIBLE" );
			pButtonBuffIcon->SetDownTex( L"DLG_Common_New_Texture03.tga", L"INVISIBLE" );

			CKTDGUIControl::UIPointData* pNormalPointData = new CKTDGUIControl::UIPointData();
			pNormalPointData->addSize.y = 24.f;
			pNormalPointData->addSize.x = 24.f;
			pNormalPointData->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			CKTDGUIControl::UIPointData* pOverPointData = new CKTDGUIControl::UIPointData();
			pOverPointData->addSize.y = 24.f;
			pOverPointData->addSize.x = 24.f;
			pOverPointData->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			CKTDGUIControl::UIPointData* pDownPointData = new CKTDGUIControl::UIPointData();
			pDownPointData->addSize.y = 24.f;
			pDownPointData->addSize.x = 24.f;
			pDownPointData->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pButtonBuffIcon->SetNormalPoint( pNormalPointData );
			pButtonBuffIcon->SetOverPoint( pOverPointData );
			pButtonBuffIcon->SetDownPoint( pDownPointData );
			pButtonBuffIcon->SetShowEnable( true, true );

#ifdef ERASE_BUFF_CHEAT
			pButtonBuffIcon->SetCustomMsgMouseDblClk( CX2GageManager::GUCM_ERASE_BUFF_CHEAT );
			pButtonBuffIcon->AddDummyInt( static_cast<int>(BuffIconInfo_.eBuffID) );
#endif // ERASE_BUFF_CHEAT

			m_pDlgBuffIcon->AddControl( pButtonBuffIcon );
		}
		else
		{
			pButtonBuffIcon = static_cast<CKTDGUIButton*>(m_pDlgBuffIcon->GetControl(szSButtonName)) ;
		}

		if( NULL != pButtonBuffIcon )
		{
			float fOffsetY = 0.f;
			if( true == bIsDebuff_ )
			{
				fOffsetY = vSize_.x +1.f;
#ifdef DISPLAY_BUFF_DURATION_TIME
				fOffsetY += 4.f;
#endif // DISPLAY_BUFF_DURATION_TIME
			}

			D3DXVECTOR2 vPos( ( -vSize_.y - 1.f ) * iIndex_, fOffsetY );

			pButtonBuffIcon->SetOffsetPos( vPos );
			pButtonBuffIcon->SetGuideDescOffsetPos( D3DXVECTOR2( vPos.x, vPos.y + 140.f  ) );
		}

		/// 아이콘 이미지 생성
		WCHAR szStaticName[20];
		StringCchPrintfW( szStaticName, 20, L"Buff_%d", BuffIconInfo_.eBuffID);

		CKTDGUIStatic* pStaticBuffIcon = NULL;

		if( false == m_pDlgBuffIcon->CheckControl(szStaticName) )
		{
			pStaticBuffIcon = new CKTDGUIStatic();
			if( NULL != pStaticBuffIcon )
			{
				pStaticBuffIcon->SetName( szStaticName );
				m_pDlgBuffIcon->AddControl( pStaticBuffIcon );
				CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
				pStaticBuffIcon->AddPicture( pPicture );
				pPicture->SetTex( BuffIconInfo_.szTextureFileName.c_str(), BuffIconInfo_.szTextureKeyName.c_str());
				pPicture->SetSize( vSize_ );
			}
			
#ifdef DISPLAY_BUFF_DURATION_TIME
			// 남은 시간 툴팁 추가
			{
				CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
				pStaticBuffIcon->AddString( pString );
				pString->fontIndex = XUF_DODUM_11_NORMAL;
				pString->color = D3DXCOLOR( 1,1,1,1 );
				pString->outlineColor = D3DXCOLOR( 0,0,0,1 );
				pString->msg = L"";
				pString->fontStyle = CKTDGFontManager::FS_SHELL;
				pString->sortFlag = DT_RIGHT;
				pString->pos = D3DXVECTOR2( 12.f, 12.f+4.f);
			}
#endif // DISPLAY_BUFF_DURATION_TIME

		}
		else
		{
			pStaticBuffIcon = static_cast<CKTDGUIStatic*>(m_pDlgBuffIcon->GetControl(szStaticName)) ;

#ifdef DISPLAY_BUFF_DURATION_TIME
			// 남은 시간 툴팁 추가
			{
				if( NULL != pStaticBuffIcon->GetString(0) )
				{
					pStaticBuffIcon->GetString(0)->msg = L"";
				}
			}
#endif // DISPLAY_BUFF_DURATION_TIME
		}

		if( NULL != pStaticBuffIcon )
		{
			CKTDGUIControl::CPictureData* pPicture = pStaticBuffIcon->GetPicture(0);
			if( NULL != pPicture )
			{
				pPicture->InitAction();
				if( true == BuffIconInfo_.bNew )
				{
					pPicture->SetFadeIn(1.2f);
				}
				else
				{
					if ( NULL != pPicture->pPoint )
						pPicture->pPoint->color.a = 1.f;

					if( true == pPicture->m_bFlicker && true == pPicture->m_bFlickerLoop )
						NotifyDurationTime5sec( BuffIconInfo_.eBuffID );					
					else
						InitBuffIconFlicker( BuffIconInfo_.eBuffID );

				}
			}

			float fOffsetY = 0.f;
			if( true == bIsDebuff_ )
			{
				fOffsetY = vSize_.x +1.f;
#ifdef DISPLAY_BUFF_DURATION_TIME
				fOffsetY += 4.f;
#endif // DISPLAY_BUFF_DURATION_TIME
			}

			pStaticBuffIcon->SetShow(CX2GageManager::GetInstance()->GetShowMyGageUI());
			pStaticBuffIcon->SetOffsetPos( D3DXVECTOR2( (-vSize_.y-1.f) * (iIndex_++), fOffsetY ) );
		}
	}
}
/** @function : NotifyDurationTime5sec
	@brief : 지속 시간이 5초 남은 버프 알림
	@param : 해당 버프 ID
*/
void CX2StatusGageUI::NotifyDurationTime5sec(BUFF_TEMPLET_ID eBuffID_)
{
	auto FindFunc = [eBuffID_](const BuffIcon& BuffIconInfo_ ) {
		return BuffIconInfo_.eBuffID == eBuffID_;
	};

	bool bCompleteNotify = false;
	vector<BuffIcon>::iterator it;

	//버프 목록에 있나 확인
	it = std::_Find_if( m_vecBuffList.begin(),m_vecBuffList.end(), FindFunc);
	if( it != m_vecBuffList.end() )
	{		
		bCompleteNotify = true;
	}

	if( false == bCompleteNotify )
	{
		it = std::_Find_if( m_vecDebuffList.begin(), m_vecDebuffList.end(), FindFunc);
		if( it != m_vecDebuffList.end() )
		{
			bCompleteNotify = true;
		}
	}

	if( true == bCompleteNotify )
	{
		if( NULL != m_pDlgBuffIcon )
		{
			WCHAR szStaticName[20];
			StringCchPrintfW( szStaticName, 20, L"Buff_%d", eBuffID_);
			CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDlgBuffIcon->GetControl(szStaticName)) ;
			if( NULL != pStatic && NULL != pStatic->GetPicture(0))
			{
				pStatic->GetPicture(0)->SetFlicker(0.4f, 1.f, 0.f);
			}
		}
		//UpdateBuffIcon();
	}
}

void CX2StatusGageUI::InitBuffIconFlicker( BUFF_TEMPLET_ID eBuffID_ )
{
	if( NULL == m_pDlgBuffIcon )
		return;

	WCHAR szStaticName[20];
	StringCchPrintfW( szStaticName, 20, L"Buff_%d", eBuffID_ );

	if( true == m_pDlgBuffIcon->CheckControl( szStaticName ) )
	{
		CKTDGUIStatic* pStaticBuffIcon = static_cast<CKTDGUIStatic*>(m_pDlgBuffIcon->GetControl(szStaticName)) ;
		if( NULL != pStaticBuffIcon && NULL != pStaticBuffIcon->GetPicture(0) ) 
		{
			pStaticBuffIcon->GetPicture(0)->SetFlicker(0.1f,1.f,1.f,false);
			pStaticBuffIcon->GetPicture(0)->SetShow(true);
		}
	}			
}
#endif //BUFF_ICON_UI

CX2MyGageUI::CX2MyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ )
	: CX2StatusGageUI( pGageData_, eOwnerUnitClass_ ), m_pDLGMyUnit( NULL )
	, m_bOnPopUp( false ), m_bIsShow( true )
{}


/*virtual*/ CX2MyGageUI::~CX2MyGageUI()
{
	SAFE_DELETE_DIALOG( m_pDLGMyUnit );
}

/*virtual*/ void CX2MyGageUI::InitUI()
{
	InitStatusUI();	
	InitWakeOrb();
}


/*virtual*/ void CX2MyGageUI::InitWakeOrb()
{
	CKTDGUIStatic* pStaticWakeOrb = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeOrb") );
	if ( NULL != pStaticWakeOrb )
	{
#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_EVE ) );			// 이브 구슬 삭제
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_ARA ) );			// 아라 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_RAVEN ) );		// 레이븐 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_CHUNG ) );		// 청 구슬 삭제
	}

	// 각성 구슬 슬롯
	CKTDGUIStatic* pStaticWakeSlot = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeSlot") );
	if ( NULL != pStaticWakeSlot )
	{
#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_EVE ) );			// 이브 구슬 삭제
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_ARA ) );			// 아라 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_RAVEN ) );		// 레이븐 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_CHUNG ) );		// 청 구슬 삭제
	}
}

/*virtual*/ void CX2MyGageUI::SetShow( const bool bShow_ )
{
	CX2GageUI::SetShow( bShow_ );

#ifdef FIX_OBSERVER_MODE	/// 옵저버리면 숨겨버리자.
	if ( NULL != g_pX2Room && NULL != g_pX2Room->GetMySlot() &&
		true == g_pX2Room->GetMySlot()->m_bObserver )
	{
		if ( NULL != m_pDLGMyUnit )
			m_pDLGMyUnit->SetShow( false );

#ifdef BUFF_ICON_UI
		if( NULL != m_pDlgBuffIcon )
			m_pDlgBuffIcon->SetShow( false );
#endif // BUFF_ICON_UI
	}
	else
#endif FIX_OBSERVER_MODE
	{
		if ( NULL != m_pDLGMyUnit )
			m_pDLGMyUnit->SetShow( bShow_ );

#ifdef BUFF_ICON_UI
		if( NULL != m_pDlgBuffIcon )
			m_pDlgBuffIcon->SetShow( bShow_ );
#endif // BUFF_ICON_UI
	}
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2MyGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2MyGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( NULL == m_pDLGMyUnit )
		return;

	UpdateHpGage();
	UpdateMpAndHyperAndDetonationGage();

	UpdateHyperModeOrb();

	UpdateHyperModeRemainTime();
	UpdatePartyLeaderUI();



	UpdateInfoString();

	if ( m_pDLGMyUnit != NULL && m_pDLGMyUnit->GetShow() == true )
	{
		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_BATTLE_FIELD:
		case CX2Main::XS_VILLAGE_MAP:
			{
				if ( g_pKTDXApp->GetDIManager()->GetMouse()->GetPureButtonState( MOUSE_RBUTTON ) == TRUE )
				{
					D3DXVECTOR2 vResoultion( g_pKTDXApp->GetResolutionScaleX(), g_pKTDXApp->GetResolutionScaleY() );
					D3DXVECTOR2 leftTop = m_pDLGMyUnit->GetPos();
					leftTop.x *= vResoultion.x;
					leftTop.y *= vResoultion.y;
					D3DXVECTOR2 RigthBottom( leftTop.x + m_pDLGMyUnit->GetSize().x * vResoultion.x, leftTop.y + m_pDLGMyUnit->GetSize().y * vResoultion.y );
					D3DXVECTOR2 mousePoint( (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(), (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos() );

					if ( mousePoint.x >= leftTop.x && mousePoint.x <= RigthBottom.x && 
						mousePoint.y >= leftTop.y && mousePoint.y <= RigthBottom.y )
					{
						m_bOnPopUp = true;
						g_pData->GetPetManager()->ClosePetPopupMenu();
						const CX2Unit::UnitData& kMyUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
						
						{
							CX2PartyManager* pPartyManager = g_pData->GetPartyManager();
							bool bIsParty = false;

							if ( NULL != pPartyManager && true == pPartyManager->DoIHaveParty() )
								bIsParty = true;

							g_pData->GetUserMenu()->OpenUserPopupMenu( kMyUnitData.m_UnitUID, bIsParty );
						}
					}
					else
					{
						if ( true == m_bOnPopUp && false == g_pData->GetUserMenu()->GetShowMenu() )
						{
							g_pData->GetUserMenu()->ClosePopupMenu();
						}

						m_bOnPopUp = false;
					}
				}
			}
			break;
		}
	}


	if ( NULL != g_pData->GetPlayGuide() )
	{
		if( m_pGageData->GetNowMp() >= 100.f )
		{	//체력 100 이상이라면 스킬사용에 관한 가이드 출력

			if( NULL != g_pMain && NULL != g_pMain->GetKeyPad() )
			{
				g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_SPECIAL_ATTACK, true, 
					g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL1 ), g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL2 ), 
					g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL3 ), g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL4 ));
			}
		}
		else
		{
			g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_SPECIAL_ATTACK, false);
		}
	}
	//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
// #ifdef	NEW_CHARACTER_CHUNG
// 	// 청의 캐논볼 UI를 표시해 줘야 하면
// 	if ( 0 < m_pCannonBallUIPtr.use_count() && NULL != m_pCannonBallUIPtr->GetDLGMyUnit() )
// 	{
// 		if ( true == m_pCannonBallUIPtr->GetBerserkModeChanged() )
// 		{
// 			CKTDGUIStatic* pStaticCharPictures = static_cast<CKTDGUIStatic*>( m_pCannonBallUIPtr->GetDLGMyUnit()->GetControl( L"StaticPVPGameMyChar" ) );
// 
// 			if( NULL != pStaticCharPictures->GetPicture( 0 ) )
// 			{
// 				wstring wstrFileName;
// 				wstring wstrPieceName;
// 
// 				CX2Unit::UNIT_CLASS eClassType = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass;
// 				if ( false == m_pCannonBallUIPtr->IsBerserkMode() )
// 					CX2Data::GetCharacterImageName( wstrFileName, wstrPieceName, eClassType, CX2Data::CIT_MyGage );
// 				// 광폭화 모드이면
// 				else
// 				{
// 					GetBerserkModeMyStateImage( wstrFileName, wstrPieceName, eClassType );
// 				}
// 
// 				pStaticCharPictures->GetPicture( 0 )->SetTex( wstrFileName.c_str(), wstrPieceName.c_str() );
// 				pStaticCharPictures->GetPicture( 0 )->SetSizeAsTextureSize();
// 
// 				m_pCannonBallUIPtr->SetBerserkModeChanged( false );
// 			}
// 		}
// 		m_pCannonBallUIPtr->CannonBallUIFrameMove();
// 	}
// #endif	NEW_CHARACTER_CHUNG
	//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG

#ifdef BUFF_ICON_UI
	SetBuffIconGuideDesc();		/// 버프 아이콘 툴팁 설정
#endif // BUFF_ICON_UI
}

/*virtual*/ void CX2MyGageUI::CrashMyStateGageUI( const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, D3DXCOLOR changeColor )
{
	if ( NULL != m_pDLGMyUnit && m_pDLGMyUnit->GetIsMoving() == false )
	{
		D3DXVECTOR2 vPosToMove = RandomNumber( _min, _max );
		m_pDLGMyUnit->Move( vPosToMove, changeColor, 0.05f, true );
		m_pDLGMyUnit->Move( D3DXVECTOR2(0,0) , changeColor, 0.05f, false );
		vPosToMove = RandomNumber( _min, _max );
		m_pDLGMyUnit->Move( vPosToMove, changeColor, 0.05f, false);
		m_pDLGMyUnit->Move( D3DXVECTOR2(0,0), D3DXCOLOR( 1, 1, 1, 1), 0.05f, false);
		vPosToMove = RandomNumber( _min, _max );
		m_pDLGMyUnit->Move( vPosToMove, changeColor, 0.05f, false);
		m_pDLGMyUnit->Move( D3DXVECTOR2(0,0), D3DXCOLOR( 1, 1, 1, 1), 0.05f, false);
	}
}


void CX2MyGageUI::UpdateHyperModeOrb()
{
	CKTDGUIStatic* pStaticHyperModeOrb = static_cast<CKTDGUIStatic*> ( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeOrb") );

	if ( NULL != pStaticHyperModeOrb )
	{
		const UINT COUNT_OF_HYPER_MODE = m_pGageData->GetHyperModeCount();

		const UINT uiPictureSizeOfHyperModeOrb = pStaticHyperModeOrb->GetPictureNum();
		for ( UINT uiIndex = 0; uiIndex < uiPictureSizeOfHyperModeOrb; ++uiIndex )
		{
			CKTDGUIControl::CPictureData* pPictureHyperModeOrb = pStaticHyperModeOrb->GetPicture(uiIndex);
			if ( NULL == pPictureHyperModeOrb )
				continue;
			
			if ( COUNT_OF_HYPER_MODE > uiIndex )
			{
				// 이전에 없던 각성 구슬이 생긴 것이면 이펙트를 보여줌
				if ( false == pPictureHyperModeOrb->GetShow() )
				{
					pPictureHyperModeOrb->SetShow( true );

					CKTDGParticleSystem::CParticleEventSequence* pSeqHyperBallTick = g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"HyperBallTick", 
						pStaticHyperModeOrb->GetPicture(uiIndex)->GetPos().x + pStaticHyperModeOrb->GetPicture(uiIndex)->GetSize().x * 0.5f, 
						pStaticHyperModeOrb->GetPicture(uiIndex)->GetPos().y + pStaticHyperModeOrb->GetPicture(uiIndex)->GetSize().y * 0.5f, 
						0.0f, 1000,1000, -1, 10 );
					if( pSeqHyperBallTick != NULL )
					{
						pSeqHyperBallTick->SetOverUI( true );
						pSeqHyperBallTick->SetBlackHolePosition( pSeqHyperBallTick->GetPosition() );
						pSeqHyperBallTick->UseLookPoint( true );
						pSeqHyperBallTick->SetLookPoint( pSeqHyperBallTick->GetPosition() );
					}
					CKTDGParticleSystem::CParticleEventSequence* pSeqHyperBallRing = g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"HyperBallRing", 
						pStaticHyperModeOrb->GetPicture(uiIndex)->GetPos().x + pStaticHyperModeOrb->GetPicture(uiIndex)->GetSize().x * 0.5f,
						pStaticHyperModeOrb->GetPicture(uiIndex)->GetPos().y + pStaticHyperModeOrb->GetPicture(uiIndex)->GetSize().y * 0.5f,
						0.0f, 1000,1000, -1, 1 );
					if( pSeqHyperBallRing != NULL )
						pSeqHyperBallRing->SetOverUI( true );
				}
			}
			else
			{
				pPictureHyperModeOrb->SetShow( false );
			}
		}			
	}
}

void CX2MyGageUI::UpdateHpGage()
{
	CKTDGUIStatic* pStaticBar_HP = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateBar_HP") );
	ASSERT( NULL != pStaticBar_HP );
	if ( NULL != pStaticBar_HP && NULL != m_pGageData )
	{
		const float fMaxHp = m_pGageData->GetMaxHp();
		const float fHpPercent = m_pGageData->GetNowHp() / fMaxHp;

		UpdateGageForUV( pStaticBar_HP, 0, L"HP", fHpPercent, true );
	}
}

/*virtual*/ void CX2MyGageUI::UpdateMpAndHyperAndDetonationGage()
{
	CKTDGUIStatic* pStaticStateBar = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateBar") );
	if ( NULL != pStaticStateBar )
	{
		UpdateMpGageAndString( pStaticStateBar );
		UpdateHyperGage( pStaticStateBar );
		UpdateDetonationGage( pStaticStateBar );
	}
}

void CX2MyGageUI::UpdateMpGageAndString( CKTDGUIStatic* pStaticStateBar_ )
{
	if ( NULL != m_pGageData )
	{
		const float fNowMp = m_pGageData->GetNowMp();
		const float fMaxMp = m_pGageData->GetMaxMp();
		const float fMpPercent =  fNowMp / fMaxMp;
		UpdateGageForUV( pStaticStateBar_, PGUB_MY_MP, L"MP", fMpPercent, true );
		CKTDGUIStatic* pStaticMp = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"g_pStatic_MP") );
		if ( pStaticMp != NULL )
		{
			// mp 배율증가 옵션 추가로 인해 max값출력시 소수부분 올려서 출력하는 부분에서 문제시 되는 부분 수정
			// ex:) max가 10.3, now가 10.3이 되었을 경우 출력시 max는 11, now는 10이 되어 출력되는 현상 발생
			const UINT uiStrSize = 32;
			WCHAR buf[uiStrSize];
			ZeroMemory( buf, sizeof(WCHAR) * uiStrSize );
			
			StringCchPrintf( buf, uiStrSize, L"%ld/%ld", 
				static_cast<UINT>( floor( fNowMp + 0.5f ) ), static_cast<UINT>( floor( fMaxMp + 0.5f ) ) );
			pStaticMp->GetString(0)->msg = buf;
		}
		UpdateMpGuidePoint();
	}
}
/** @function : UpdateMpGuidePoint
	@brief : MP량에 따라 MP 가이드 포인트 위치 변경
*/
void CX2MyGageUI::UpdateMpGuidePoint()
{
	//최대 MP량 변경이 있을 때만 MP 가이드 포인트 위치 변경
	if( true == m_pGageData->GetUpdateMpPoint() ) 
	{
		const float fMaxGageWidth = 224.f;
		const float fMaxMp = m_pGageData->GetMaxMp();
		const float f100MpPointPos = fMaxGageWidth / fMaxMp * 100; 
		const float f200MpPointPos = fMaxGageWidth / fMaxMp * 200; 

		D3DXVECTOR2 vec100MpPointPos((f100MpPointPos - (fMaxGageWidth/3.f)),0.f);
		D3DXVECTOR2 vec200MpPointPos((f200MpPointPos - (fMaxGageWidth/3.f*2.f)),0.f);

		CKTDGUIStatic* pStaticMpPoint = static_cast<CKTDGUIStatic*>(m_pDLGMyUnit->GetControl(L"100mpAndPoint"));
		if( NULL != pStaticMpPoint)
			pStaticMpPoint->SetOffsetPos(vec100MpPointPos);

		pStaticMpPoint = static_cast<CKTDGUIStatic*>(m_pDLGMyUnit->GetControl(L"200mp"));
		if( NULL != pStaticMpPoint)
			pStaticMpPoint->SetOffsetPos(vec200MpPointPos);

		m_pGageData->SetUpdateMpPoint(false);
	}
}
/*virtual*/ void CX2MyGageUI::UpdateHyperGage( CKTDGUIStatic* pStaticStateBar_ )
{
	if ( NULL != m_pGageData )
	{
		const float fNowHyper = m_pGageData->GetNowSoul();
		const float fMaxHyper = m_pGageData->GetMaxSoul();
		const float fHyperPercent = fNowHyper / fMaxHyper;

		UpdateGageForUV( pStaticStateBar_, PGUB_MY_SOUL, L"RAGE", fHyperPercent, false );
	}
}

/*virtual*/ void CX2MyGageUI::UpdateDetonationGage( CKTDGUIStatic* pStaticStateBar_ )
{
	if ( NULL != m_pGageData )
	{
	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		/// 게임이 아닌 곳에서는 기폭 게이지 채우지 말자.
		const float fNowDetonation = ( NULL != g_pX2Game ) ? m_pGageData->GetNowChargeMpForDetonation() : 0.f;
	#else //SERV_ADD_LUNATIC_PSYKER
		const float fNowDetonation = m_pGageData->GetNowChargeMpForDetonation();
	#endif //SERV_ADD_LUNATIC_PSYKER

		const float fDetonationPercent = fNowDetonation / THREE_CHARGE;
		UpdateGageForUV( pStaticStateBar_, PGUB_MY_ACTIVE_MP, L"MP_ACTIVE", fDetonationPercent, true );
	}
}

void CX2MyGageUI::UpdateHyperGageString( const float fNowHyper_, const float fMaxHyper_ )
{
	CKTDGUIStatic* pStaticHyper = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"g_pStatic_Soul") );
	if ( pStaticHyper != NULL )
	{
		// mp 배율증가 옵션 추가로 인해 max값출력시 소수부분 올려서 출력하는 부분에서 문제시 되는 부분 수정
		// ex:) max가 10.3, now가 10.3이 되었을 경우 출력시 max는 11, now는 10이 되어 출력되는 현상 발생
		const UINT uiNowHyper = static_cast<UINT>( floor( fNowHyper_ + 0.5f ) );
		const UINT uiMaxHyper = static_cast<UINT>( floor( fMaxHyper_ + 0.5f ) );
		const UINT uiStrSize = 32;
		WCHAR buf[uiStrSize];
		ZeroMemory( buf, sizeof(WCHAR) * uiStrSize );
		StringCchPrintf( buf, uiStrSize, L"%ld/%ld", uiNowHyper, uiMaxHyper );
		pStaticHyper->GetString(0)->msg = buf;
	}
}

void CX2MyGageUI::UpdateHyperModeRemainTime()
{
	CKTDGUIStatic* pStatic_HyperMode_Time = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"Static_HyperMode_Time" ) );
	
	if ( NULL != pStatic_HyperMode_Time && NULL != m_pGageData )
	{
		const float fRemainTime = m_pGageData->GetHyperModeRemainTime();
		UINT uiHyperModeTime = static_cast<UINT>( fRemainTime );
		if( fRemainTime > 0.f)
			++uiHyperModeTime;
			
		if ( uiHyperModeTime > 0 )
		{
			const UINT uiStrSize = 32;
			WCHAR wstrHyperModeRemainTime[uiStrSize];
			ZeroMemory( wstrHyperModeRemainTime, sizeof(WCHAR) * uiStrSize );
			
			if ( uiHyperModeTime < 8 )
			{				
				StringCchPrintf( wstrHyperModeRemainTime, uiStrSize, L"#CFF0000%ld%s", uiHyperModeTime, GET_STRING( STR_ID_180 ) );
			}
			else
			{
				StringCchPrintf( wstrHyperModeRemainTime, uiStrSize, L"%ld%s", uiHyperModeTime, GET_STRING( STR_ID_180 ) );
			}

			pStatic_HyperMode_Time->SetString( 0, wstrHyperModeRemainTime );
		}
		else
		{
			pStatic_HyperMode_Time->SetString( 0, L"" );
		}
	}
}

void CX2MyGageUI::UpdatePartyLeaderUI()
{
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGMyUnit->GetControl(L"g_pStaticPartyLeader"));
	if( NULL != pStatic )
	{
		if( NULL != g_pData && NULL != g_pData->GetPartyManager() )
		{
			bool bShow = g_pData->GetPartyManager()->AmIPartyLeader(); 	
			if( NULL != g_pX2Game && CX2Game::GT_PVP == g_pX2Game->GetGameType() )
				bShow = false;

			pStatic->SetShow(bShow);
		}
	}
}

void CX2MyGageUI::SetCharacterImage( const CX2Unit::UNIT_CLASS eGameUnitClass_ )
{
	SetOwnerUnitClass( eGameUnitClass_ );

	CKTDGUIStatic*	pStaticCharPictures = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"StaticPVPGameMyChar" ) );
	CKTDGUIControl::CPictureData* pPictureCharacterImage = ( NULL != pStaticCharPictures ? pStaticCharPictures->GetPicture( 0 ) : NULL );

	if( NULL != pPictureCharacterImage )
	{
		wstring fileName;
		wstring pieceName;
		if( true == CX2Data::GetCharacterImageName( fileName, pieceName, GetOwnerUnitClass(), CX2Data::CIT_MyGage ) )
		{
			pStaticCharPictures->GetPicture( 0 )->SetTex( fileName.c_str(), pieceName.c_str() );
			pStaticCharPictures->GetPicture( 0 )->SetSizeAsTextureSize();
		}
		else
		{
			ASSERT( !"CharacterImage is not existed" );
		}
	}
}

void CX2MyGageUI::UpdateInfoString()
{
	CKTDGUIStatic* pStaticText = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"NAME") );
	ASSERT( NULL != pStaticText );
	if ( NULL != pStaticText && NULL != g_pData )
	{
		const CX2Unit::UnitData& kMyUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
		{
			const UINT uiStrSize = 8;
			WCHAR buf[uiStrSize];
			ZeroMemory( buf, sizeof(WCHAR) * uiStrSize );
			StringCchPrintf( buf, uiStrSize, L"%d", kMyUnitData.m_Level );

#ifdef HARDCODING_STRING_BR
			pStaticText->GetString(0)->msg = GET_STRING(STR_ID_489);
#else // HARDCODING_STRING_BR
			pStaticText->GetString(0)->msg = L"Lv";
#endif // HARDCODING_STRING_BR
			pStaticText->GetString(1)->msg = buf;
			pStaticText->GetString(2)->msg = kMyUnitData.m_NickName;
		}
	}
}

void CX2MyGageUI::InitStatusUI()
{
	SAFE_DELETE_DIALOG( m_pDLGMyUnit );
	m_pDLGMyUnit = new CKTDGUIDialog( NULL, L"DLG_PVP_Game_My_State_NEW.lua" );
	m_pDLGMyUnit->SetShow( false );

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMyUnit );

	SetCharacterImage( m_eOwnerGameUnitClass );


	CKTDGUIStatic* pStaticBar = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"StaticPVPGameStateBar" ) );
	CKTDGUIControl::CPictureData* pPictureMyMp = ( NULL != pStaticBar ? pStaticBar->GetPicture( PGUB_MY_MP ) : NULL );
	if ( NULL != pPictureMyMp )
		pPictureMyMp->SetSize( D3DXVECTOR2(0, pPictureMyMp->GetSize().y) );

	CKTDGUIControl::CPictureData* pPictureMyDetonationGage = ( NULL != pStaticBar ? pStaticBar->GetPicture( PGUB_MY_ACTIVE_MP ) : NULL );
	if ( NULL != pPictureMyDetonationGage )
		pPictureMyDetonationGage->SetSize( D3DXVECTOR2(0, pPictureMyDetonationGage->GetSize().y) );





}

#ifdef BUFF_ICON_UI
/** @function : UpdateBuffIcon
	@brief : 버프 아이콘 UI 를 갱신하는 함수
			 위치 지정을 위해 CX2MyGageUI, CX2PartyMemberGageUI에서 각각 재정의
*/
void CX2MyGageUI::UpdateBuffIcon()
{	
#ifdef FIX_VISIBLE_BUFF_ICON_UI		/// 굳이 버프 아이콘을 만들 필요가 없는 State면 만들지 말자.
	if( NULL != g_pMain )
	{
		switch ( g_pMain->GetNowStateID() ) 
		{
		case CX2Main::XS_PVP_LOBBY:
#ifdef ADDED_RELATIONSHIP_SYSTEM
		case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
			{
				return;
			} break;
		default:
			break;
		}
	}
#endif FIX_VISIBLE_BUFF_ICON_UI

	if( NULL == m_pDlgBuffIcon )
	{
		m_pDlgBuffIcon = new CKTDGUIDialog( NULL, L"DLG_BuffManager.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgBuffIcon );		
		m_pDlgBuffIcon->SetShow(true);		
		//m_pDlgBuffIcon->SetPos(D3DXVECTOR2(720.f, 2.f));

#ifdef ERASE_BUFF_CHEAT
		m_pDlgBuffIcon->SetStage( g_pMain->GetNowState() );
#endif // ERASE_BUFF_CHEAT
	}	
	
	if( NULL != m_pDlgBuffIcon )
	{	//버프 리스트
		int iIndex = 0;
		bool bIsDebuff = false;
		vector<BuffIcon>::iterator it = m_vecBuffList.begin();

		for( ; it != m_vecBuffList.end(); ++it)
		{
			SetBuffIconStatic( (*it), iIndex++, bIsDebuff, D3DXVECTOR2(24,24));
			it->bNew = false;
		}

		//디버프 리스트
		iIndex = 0;
		bIsDebuff = true;
		for( it = m_vecDebuffList.begin(); it != m_vecDebuffList.end(); ++it)
		{
			SetBuffIconStatic( (*it), iIndex++, bIsDebuff, D3DXVECTOR2(24,24));
			it->bNew = false;
		}
	}
}

/** @function : SetBuffIconGuideDesc
	@brief : 버프 아이콘 툴팁을 설정하는 함수
*/
void CX2MyGageUI::SetBuffIconGuideDesc()
{
	BOOST_FOREACH( BuffIcon BuffIconData ,m_vecBuffList )	/// 버프 툴팁
	{
		if( -1 != BuffIconData.iBuffName )
		{
			WCHAR szStaticName[20];
			StringCchPrintfW( szStaticName, 20, L"ButtonBuff_%d", BuffIconData.eBuffID);

			CKTDGUIButton* pButtonBuffIcon = static_cast<CKTDGUIButton*>( m_pDlgBuffIcon->GetControl( szStaticName ) );

			if( NULL !=  pButtonBuffIcon )
			{
				std::wstring slotItemDesc = GET_STRING( BuffIconData.iBuffName );	/// 버프 이름
				slotItemDesc += L"\n\n";
#ifdef SERV_PC_BANG_TYPE
				// PC방 타입에 따라 버프 디스크립션을 변경해 줍니다.
				if ( NULL != g_pData->GetPremiumBuffManager() && BuffIconData.eBuffID == BTI_BUFF_PREMIUM_PC_ROOM )
					slotItemDesc += g_pData->GetPremiumBuffManager()->GetPcBangBuffString();
				else
#endif SERV_PC_BANG_TYPE
				slotItemDesc += GET_STRING( BuffIconData.iBuffDesc );				/// 버프 내용

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
				if ( true == g_pData->GetMyUser()->GetSelectUnit()->IsTradeBlockUnit() && BuffIconData.eBuffID == BTI_BUFF_TRADE_BLOCK )
					UpdateTradeBlockDesc( slotItemDesc );
#endif // SERV_NEW_UNIT_TRADE_LIMIT

#ifdef _IN_HOUSE_
				slotItemDesc += L"\n\nKey : ";
				slotItemDesc += BuffIconData.szTextureKeyName;
#endif //_IN_HOUSE_

#ifdef ERASE_BUFF_CHEAT
				slotItemDesc += L"\n더블 클릭 : 버프 제거(치트)";
#endif // ERASE_BUFF_CHEAT

				pButtonBuffIcon->SetGuideDesc( slotItemDesc.c_str() );			
			}
		}
	}

	BOOST_FOREACH( BuffIcon BuffIconData ,m_vecDebuffList )		/// 디버프 툴팁
	{
		if( -1 != BuffIconData.iBuffName )
		{
			WCHAR szStaticName[20];
			StringCchPrintfW( szStaticName, 20, L"ButtonBuff_%d", BuffIconData.eBuffID);

			CKTDGUIButton* pButtonBuffIcon = static_cast<CKTDGUIButton*>( m_pDlgBuffIcon->GetControl( szStaticName ) );

			if( NULL !=  pButtonBuffIcon )
			{
				std::wstring slotItemDesc = GET_STRING( BuffIconData.iBuffName );	/// 버프 이름
				slotItemDesc += L"\n\n";
				slotItemDesc += GET_STRING( BuffIconData.iBuffDesc );				/// 버프 내용

#ifdef _IN_HOUSE_
				slotItemDesc += L"\n\nKey : ";
				slotItemDesc += BuffIconData.szTextureKeyName;
#endif //_IN_HOUSE_

				pButtonBuffIcon->SetGuideDesc( slotItemDesc.c_str() );
			}
		}
	}
}
#endif //BUFF_ICON_UI
#ifdef DISPLAY_BUFF_DURATION_TIME
/*virtual*/ void CX2MyGageUI::SetDurationTime( BUFF_TEMPLET_ID eBuffID_, int iTime_)
{
	if( NULL != m_pDlgBuffIcon )
	{
		WCHAR szStaticName[20];
		StringCchPrintfW( szStaticName, 20, L"Buff_%d", eBuffID_);
		if( true == m_pDlgBuffIcon->CheckControl(szStaticName) )
		{
			CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDlgBuffIcon->GetControl(szStaticName)) ;
			if( NULL != pStatic && NULL != pStatic->GetString(0))
			{
				if( (iTime_ > 0) && (iTime_ < 999) ) 
				{
					pStatic->GetString(0)->msg = GET_REPLACED_STRING((STR_ID_2632, "i", iTime_));
				}
				else
					pStatic->GetString(0)->msg = L"";
			}
		}
	}
}
#endif // DISPLAY_BUFF_DURATION_TIME

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
void CX2MyGageUI::UpdateTradeBlockDesc( std::wstring& wstrBuffDesc )
{
	wstring wstrTradeBlockReleaseTime = g_pData->GetMyUser()->GetSelectUnit()->GetUnitCreateDate();
	CTime tCurrentTime, tTradeBlockReleaseTime;
	tCurrentTime = g_pData->GetServerCurrentTime();
	KncUtil::ConvertStringToCTime( wstrTradeBlockReleaseTime, tTradeBlockReleaseTime );

	tTradeBlockReleaseTime += CTimeSpan( g_pData->GetMyUser()->GetSelectUnit()->GetNewUnitTradeBlockDay(), 0, 0, 0 );

	if( tTradeBlockReleaseTime < tCurrentTime &&  g_pData->GetMyUser()->GetSelectUnit()->GetNewUnitTradeBlockUnitClass() != 0 
		&& g_pData->GetMyUser()->GetSelectUnit()->GetClass() < g_pData->GetMyUser()->GetSelectUnit()->GetNewUnitTradeBlockUnitClass() )
	{
		// 전직 후에 거래가능합니다.
		wstrBuffDesc += GET_STRING( STR_ID_24523 );
		return;
	}

	CTimeSpan tTimeGap = tTradeBlockReleaseTime - tCurrentTime;

	wstrBuffDesc += L" ";

	if( tTimeGap.GetDays() > 0 )
	{
		wstrBuffDesc += GET_REPLACED_STRING( ( STR_ID_22727, "i", tTimeGap.GetDays() ) );

		if( tTimeGap.GetHours() > 0)
		{
			wstrBuffDesc += GET_REPLACED_STRING( ( STR_ID_22728, "i", tTimeGap.GetHours() ) );
		}
	}
	else if( tTimeGap.GetDays() == 0 )
	{
		if( tTimeGap.GetHours() > 0 )
		{
			wstrBuffDesc += GET_REPLACED_STRING( ( STR_ID_22728, "i", tTimeGap.GetHours() ) );
		}
		else if( tTimeGap.GetHours() == 0 )
		{
			wstrBuffDesc += GET_STRING( STR_ID_22729 );
		}
	}

	return;
}
#endif // SERV_NEW_UNIT_TRADE_LIMIT

CX2ChungMyGageUI::CX2ChungMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ )
	: CX2MyGageUI( pGageData_, eOwnerUnitClass_ ), m_iNumOfVerticalOfCannonBall( 0 ), m_bUseExtraCannonBallGage( false )
{

}

void CX2ChungMyGageUI::InitUI()
{
	InitStatusUI();

	m_pDLGMyUnit->OpenScriptFile( L"DLG_Chung_Cannonball_NEW.lua" );

	InitNumOfVerticalOfCannonBall( m_eOwnerGameUnitClass );
	InitWakeOrb();
	InitCannonBallUI();
}

void CX2ChungMyGageUI::InitWakeOrb()
{
	CKTDGUIStatic* pStaticWakeOrb = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeOrb") );
	if ( NULL != pStaticWakeOrb )
	{
#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_EVE ) );			// 이브 구슬 삭제
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_ARA ) );			// 아라 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_RAVEN ) );		// 레이븐 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB3 ) );				// 일반 구슬 삭제
	}

	// 각성 구슬 슬롯
	CKTDGUIStatic* pStaticWakeSlot = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeSlot") );
	if ( NULL != pStaticWakeSlot )
	{
#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_EVE ) );			// 이브 구슬 삭제
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_ARA ) );			// 아라 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_RAVEN ) );		// 레이븐 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB3 ) );			// 일반 구슬 삭제
	}
}

void CX2ChungMyGageUI::InitCannonBallUI()
{	
	for ( int iStaticIndex = 0; iStaticIndex < 2; ++iStaticIndex )
	{
		wstringstream wstrStream;
		wstrStream << L"Static_Chung_Cannonball_" << iStaticIndex;

		CKTDGUIStatic* pStaticVerticalCannonBall = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( wstrStream.str().c_str() ) );

		ASSERT( NULL != pStaticVerticalCannonBall );
		if ( NULL != pStaticVerticalCannonBall )
			pStaticVerticalCannonBall->SetShow( false );
	}

	for ( int iStaticIndex = 0; iStaticIndex < GetNumOfVerticalOfCannonBall(); ++iStaticIndex )
	{
		wstringstream wstrStream;
		wstrStream << L"Static_Chung_Cannonball_" << iStaticIndex;

		CKTDGUIStatic* pStaticVerticalCannonBall = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( wstrStream.str().c_str() ) );

		ASSERT( NULL != pStaticVerticalCannonBall );
		if ( NULL != pStaticVerticalCannonBall )
			pStaticVerticalCannonBall->SetShow( true );
	}

	CKTDGUIStatic* pStaticVerticalCannonBallEx = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"Static_Chung_Cannonball_Ex" ) );

	ASSERT( NULL != pStaticVerticalCannonBallEx );
	if ( NULL != pStaticVerticalCannonBallEx )
		pStaticVerticalCannonBallEx->SetShow( GetUseExtraCannonBallGage() );

#ifdef FIX_CHUNG_GAGE_UI_UPDATE_BUG2
	CX2ChungGageData* pChungGageData = static_cast<CX2ChungGageData*>( m_pGageData );
	pChungGageData->SetCannonBallChanged( true );
#endif //FIX_CHUNG_GAGE_UI_UPDATE_BUG2
}

void CX2ChungMyGageUI::InitNumOfVerticalOfCannonBall( const CX2Unit::UNIT_CLASS eOwnerUnitClass_ )
{
	switch ( eOwnerUnitClass_ )
	{
	case CX2Unit::UC_CHUNG_IRON_CANNON:
	case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
	case CX2Unit::UC_CHUNG_IRON_PALADIN:
	case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
		{
			SetNumOfVerticalOfCannonBall( 1 );
			SetUseExtraCannonBallGage( false );
		} break;
	
	case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:
		{
			SetNumOfVerticalOfCannonBall( 1 );
			SetUseExtraCannonBallGage( true );
		} break;

	case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
	case CX2Unit::UC_CHUNG_DEADLY_CHASER:
		{
			SetNumOfVerticalOfCannonBall( 2 );
			SetUseExtraCannonBallGage( false );
		} break;

	default:
		ASSERT( !L"UnitClass is Undefined In this Function" );
		break;	
	}
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2ChungMyGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2ChungMyGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    CX2MyGageUI::OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	CX2MyGageUI::OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	UpdateCharacterImage();
	UpdateCannonBallCountUI();
}

void CX2ChungMyGageUI::UpdateCannonBallCountUI()
{
	CX2ChungGageData* pChungGageData = static_cast<CX2ChungGageData*>( m_pGageData );
	
	if ( pChungGageData->GetCannonBallChanged() )
	{
		pChungGageData->SetCannonBallChanged( false );

		for ( int iStaticIndex = 0; iStaticIndex < GetNumOfVerticalOfCannonBall(); ++iStaticIndex )
		{
			wstringstream wstrStream;
			wstrStream << L"Static_Chung_Cannonball_" << iStaticIndex;
		 
			CKTDGUIStatic* pStaticVerticalCannonBall = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( wstrStream.str().c_str() ) );
		
			ASSERT( NULL != pStaticVerticalCannonBall );
			if ( NULL != pStaticVerticalCannonBall )
			{
				int iNumOfPicture = pStaticVerticalCannonBall->GetPictureNum();

				// 라인당 최소 캐논볼의 갯수 (0번째 라인은 0개, 1번째 라인은 6개 등)
				// Picture 수는 BG Picture 포함해서 7개 이기 때문 -1 해줌
				for ( int iPictureIndex = 0; iPictureIndex < iNumOfPicture ; ++iPictureIndex )
				{
					bool bShowPictureDataCannonBall = false;

					// 광폭화 모드면 무조건 Full 차지로 보여야 함
					if ( pChungGageData->GetBerserkMode() )
						bShowPictureDataCannonBall = true;
					// 캐논볼의 갯수가 라인당 최소 캐논볼 갯수 + 그 라인의 캐논볼 갯수(픽쳐의 인덱스) 보다 많거나 같으면
					else if ( pChungGageData->GetNowCannonBallCount() >= iStaticIndex * 6 + iPictureIndex  )
						bShowPictureDataCannonBall = true;

					if( 0 == iPictureIndex )
						bShowPictureDataCannonBall = true;

					CKTDGUIControl::CPictureData* pPictureDataCannonBall = pStaticVerticalCannonBall->GetPicture( iPictureIndex );
					ASSERT( NULL != pPictureDataCannonBall );
					pPictureDataCannonBall->SetShow( bShowPictureDataCannonBall );
				}
			}
		}

		if ( GetUseExtraCannonBallGage() )
		{
			CKTDGUIStatic* pStaticVerticalCannonBall
				= static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"Static_Chung_Cannonball_Ex" ) );

			ASSERT( NULL != pStaticVerticalCannonBall );
			if ( NULL != pStaticVerticalCannonBall )
			{
				int iNumOfPicture = pStaticVerticalCannonBall->GetPictureNum();

				// 라인당 최소 캐논볼의 갯수 (0번째 라인은 0개 )
				for ( int iPictureIndex = 0; iPictureIndex < iNumOfPicture; ++iPictureIndex )
				{
					bool bShowPictureDataCannonBall = false;

					//그 라인의 캐논볼 갯수(픽쳐의 인덱스) 보다 많거나 같으면
					if ( pChungGageData->GetNowCannonBallCountEx() >= iPictureIndex  )
						bShowPictureDataCannonBall = true;

					CKTDGUIControl::CPictureData* pPictureDataCannonBall = pStaticVerticalCannonBall->GetPicture( iPictureIndex );
					ASSERT( NULL != pPictureDataCannonBall );
					pPictureDataCannonBall->SetShow( bShowPictureDataCannonBall );
				}
			}
		}
	}	
}

void CX2ChungMyGageUI::UpdateCharacterImage()
{
	CX2ChungGageData* pChungGageData = static_cast<CX2ChungGageData*>( m_pGageData );
	CKTDGUIStatic* pStaticCharPictures = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"StaticPVPGameMyChar" ) );

	if( NULL != pStaticCharPictures->GetPicture( 0 ) )
	{
		wstring wstrFileName;
		wstring wstrPieceName;
		
		if( true == pChungGageData->GetBerserkMode() &&
			NULL != g_pX2Game )
		{
			GetBerserkModeMyStateImage( wstrFileName, wstrPieceName, GetOwnerUnitClass() );
		}
		else
		{
			CX2Data::GetCharacterImageName( wstrFileName, wstrPieceName, GetOwnerUnitClass(), CX2Data::CIT_MyGage );
		}

		ASSERT( !wstrFileName.empty() && !wstrPieceName.empty() );

		pStaticCharPictures->GetPicture( 0 )->SetTex( wstrFileName.c_str(), wstrPieceName.c_str() );
		pStaticCharPictures->GetPicture( 0 )->SetSizeAsTextureSize();

		pChungGageData->SetBerserkModeChanged( false );
	}
}

void CX2ChungMyGageUI::GetBerserkModeMyStateImage( OUT wstring& wstrFileName, OUT wstring& wstrPieceName, IN const CX2Unit::UNIT_CLASS eClassType )
{
	switch ( eClassType )
	{
	case CX2Unit::UC_CHUNG_IRON_CANNON:
		wstrFileName	= L"DLG_UI_Common_Texture61_NEW.tga";
		wstrPieceName	= L"CHUNG_STATE_RAGE";
		break;

		//{{ kimhc // 2011.1.26 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
		wstrFileName	= L"DLG_UI_Common_Texture61_NEW.tga";
		wstrPieceName	= L"F_GUARDIAN_RAGE";
		break;

	case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
		wstrFileName	= L"DLG_UI_Common_Texture61_NEW.tga";
		wstrPieceName	= L"S_GUARDIAN_RAGE";
		break;
#endif	CHUNG_FIRST_CLASS_CHANGE
		//}} kimhc // 2011.1.26 // 청 1차 전직

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
		wstrFileName	= L"DLG_UI_Common_Texture61_NEW.tga";
		wstrPieceName	= L"SH_GUARDIAN_RAGE";
		break;
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef CHUNG_SECOND_CLASS_CHANGE
	case CX2Unit::UC_CHUNG_IRON_PALADIN:
		wstrFileName	= L"DLG_UI_Common_Texture61_NEW.tga";
		wstrPieceName	= L"I_PALADIN_RAGE";
		break;

	case CX2Unit::UC_CHUNG_DEADLY_CHASER:
		wstrFileName	= L"DLG_UI_Common_Texture61_NEW.tga";
		wstrPieceName	= L"D_CHASER_RAGE";
		break;
#endif CHUNG_SECOND_CLASS_CHANGE
	//택티컬트루퍼
	#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:
		{
			wstrFileName	= L"DLG_UI_Common_Texture66_NEW.tga";
			wstrPieceName	= L"T_TROOPER_RAGE";
		} break;
	#endif //SERV_CHUNG_TACTICAL_TROOPER
	default:
		break;
	} // switch
}


CX2ElswordMyGageUI::~CX2ElswordMyGageUI()
{
	m_pStaticElswordVigor	= NULL;
	m_pStaticElswordDest	= NULL;

	StopUIEffectElswordWS();
}

void CX2ElswordMyGageUI::InitUI()
{
#if !defined(WORLD_TOOL)
	InitStatusUI();

	m_pDLGMyUnit->OpenScriptFile( L"DLG_Elsword_WayOfSword.lua" );

	InitUIElswordWSP();
	InitWakeOrb();
#endif //WORLD_TOOL
}

/*virtual*/ void CX2ElswordMyGageUI::SetShow( const bool bShow_ )
{
	CX2MyGageUI::SetShow( bShow_ );

	CKTDGParticleSystem* pGameMinorParticle = g_pData->GetGameMinorParticle();

	for ( int i = 0; i < ARRAY_SIZE( m_hSeqVigorEffect ); ++i )
	{
		if( m_hSeqVigorEffect[i] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = pGameMinorParticle->GetInstanceSequence( m_hSeqVigorEffect[i] );
			if( NULL != pSeq )
				pSeq->SetShowObject( bShow_ );
		}

		if( m_hSeqDestEffect[i] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = pGameMinorParticle->GetInstanceSequence( m_hSeqDestEffect[i] );
			if( NULL != pSeq )
				pSeq->SetShowObject( bShow_ );
		}
	}
#ifdef BUFF_ICON_UI
	if( NULL != m_pDlgBuffIcon )
	{
		m_pDlgBuffIcon->SetShow(bShow_);
	}
#endif //BUFF_ICON_UI
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2ElswordMyGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2ElswordMyGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    CX2MyGageUI::OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	CX2MyGageUI::OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	const bool bDialogHide = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog();

	// 다이얼로그를 숨겨야 하는데 이펙트가 보이고 있다면
	if ( bDialogHide == GetShowWspParticle() )
	{
		SetShowWspParticle( !bDialogHide );
	}

	UpdateUIElswordWSP();
	UIFrameMoveElswordWSP();
}

void CX2ElswordMyGageUI::InitUIElswordWSP()
{
	m_pStaticElswordVigor = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"g_pStatic_EL_BLUE" ) );
	m_pStaticElswordDest = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"g_pStatic_EL_RED" ) );
}

void CX2ElswordMyGageUI::StopUIEffectElswordWS()
{
	if( NULL == g_pX2Game ||
		NULL == g_pX2Game->GetMinorParticle() )
		return;

	if ( m_hSeqVigorEffect[0] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqVigorEffect[0] );
	if ( m_hSeqVigorEffect[1] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqVigorEffect[1] );
	if ( m_hSeqVigorEffect[2] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqVigorEffect[2] );
	if ( m_hSeqDestEffect[0] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqDestEffect[0] );
	if ( m_hSeqDestEffect[1] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqDestEffect[1] );
	if ( m_hSeqDestEffect[2] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqDestEffect[2] );

	return;
}

void CX2ElswordMyGageUI::PlayUIEffectDestruction()
{	
	if( NULL == g_pX2Game ||
		NULL == g_pX2Game->GetMinorParticle() )
		return;

	StopUIEffectElswordWS();

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		return;
#endif

	D3DXVECTOR3 vecEffectPos( 0.f, 0.f, 0.f );

	vecEffectPos.x = m_pStaticElswordDest->GetPicture(4)->GetPos().x +  m_pStaticElswordDest->GetPicture(4)->GetSize().x / 2.0f;
	vecEffectPos.y = m_pStaticElswordDest->GetPicture(4)->GetPos().y +  m_pStaticElswordDest->GetPicture(4)->GetSize().y / 2.0f;

	m_hSeqDestEffect[0] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_demolition_UI_01", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle1 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqDestEffect[0] );
	if( pParticle1 != NULL )
	{
		pParticle1->SetOverUI( true );
		pParticle1->SetResolutionConvert( true );
	}

	m_hSeqDestEffect[1] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_demolition_UI_02", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle2 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqDestEffect[1] );
	if( pParticle2 != NULL )
	{
		pParticle2->SetOverUI( true );
		pParticle2->SetResolutionConvert( true );
	}

	m_hSeqDestEffect[2] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_demolition_UI_03", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle3 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqDestEffect[2] );
	if( pParticle3 != NULL )
	{
		pParticle3->SetOverUI( true );
		pParticle3->SetResolutionConvert( true );
	}
	
	return;
}

void CX2ElswordMyGageUI::PlayUIEffectVigor()
{
	if( NULL == g_pX2Game ||
		NULL == g_pX2Game->GetMinorParticle() )
		return;

	StopUIEffectElswordWS();

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		return;
#endif

	D3DXVECTOR3 vecEffectPos( 0.f, 0.f, 0.f );

	vecEffectPos.x = m_pStaticElswordVigor->GetPicture(4)->GetPos().x +  m_pStaticElswordVigor->GetPicture(4)->GetSize().x / 2.0f;
	vecEffectPos.y = m_pStaticElswordVigor->GetPicture(4)->GetPos().y +  m_pStaticElswordVigor->GetPicture(4)->GetSize().y / 2.0f;

	m_hSeqVigorEffect[0] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_vitality_UI_01", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle1 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqVigorEffect[0] );
	if( pParticle1 != NULL )
	{
		pParticle1->SetOverUI( true );
		pParticle1->SetResolutionConvert( true );
	}

	m_hSeqVigorEffect[1] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_vitality_UI_02", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle2 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqVigorEffect[1] );
	if( pParticle2 != NULL )
	{
		pParticle2->SetOverUI( true );
		pParticle2->SetResolutionConvert( true );
	}

	m_hSeqVigorEffect[2] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_vitality_UI_03", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle3 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqVigorEffect[2] );
	if( pParticle3 != NULL )
	{
		pParticle3->SetOverUI( true );
		pParticle3->SetResolutionConvert( true );
	}
	
	return;
}

void CX2ElswordMyGageUI::UIFrameMoveElswordWSP()
{
	if ( NULL == g_pData && NULL == g_pData->GetDamageManager() )
		return;

	//CX2GUUser *pMyUser = static_cast< CX2GUUser* >( m_pGameUnit );
	CX2ElswordGageData* pElswordGageData = static_cast<CX2ElswordGageData*>( m_pGageData );

	if ( NULL == pElswordGageData )
		return;

	CKTDGUIControl::CPictureData* pGagePicture = NULL;
	CKTDGUIControl::CPictureData* pGageEffectPicture = NULL;
	float fGageMax = 1.f;
	wstring wstrTexPieceName = L"";
	wstring wstrTexEffectPieceName = L"";
	switch( pElswordGageData->GetWayOfSwordState() )
	{
	case -2: // CX2GUElsword_SwordMan::WSS_DESTRUCTION
		{
			if ( NULL != m_pStaticElswordDest &&
				 NULL != m_pStaticElswordDest->GetPicture( 1 ) &&
				 NULL != m_pStaticElswordDest->GetPicture( 2 ) )
			{
				pGagePicture			= m_pStaticElswordDest->GetPicture( 1 );
				pGageEffectPicture		= m_pStaticElswordDest->GetPicture( 2 );
				fGageMax				= g_pData->GetDamageManager()->GetDestStateThreshold();
				wstrTexPieceName		= L"EL_RED";
				wstrTexEffectPieceName	= L"EL_EFFECT";
			}
		}
		break;
	case -1: // CX2GUElsword_SwordMan::WSS_TOWARD_DESTRUCTION
		{
			if ( NULL != m_pStaticElswordDest &&
				 NULL != m_pStaticElswordDest->GetPicture( 1 ) )
			{
				pGagePicture			= m_pStaticElswordDest->GetPicture( 1 );
				pGageEffectPicture		= NULL;
				fGageMax				= g_pData->GetDamageManager()->GetDestStateThreshold();
				wstrTexPieceName		= L"EL_RED";
				wstrTexEffectPieceName	= L"";
			}
		}
		break;
	case 0: // CX2GUElsword_SwordMan::WSS_CENTER
		{
			pGagePicture			= NULL;
			pGageEffectPicture		= NULL;
			fGageMax				= 1.f;
			wstrTexPieceName		= L"";
			wstrTexEffectPieceName	= L"";
		}
		break;
	case 1: // CX2GUElsword_SwordMan::WSS_TOWARD_VIGOR
		{
			if ( NULL != m_pStaticElswordVigor &&
				 NULL != m_pStaticElswordVigor->GetPicture( 1 ) )
			{
				pGagePicture			= m_pStaticElswordVigor->GetPicture( 1 );
				pGageEffectPicture		= NULL;
				fGageMax				= g_pData->GetDamageManager()->GetVigorStateThreshold();
				wstrTexPieceName		= L"EL_BLUE";
				wstrTexEffectPieceName	= L"";
			}

		}
		break;
	case 2: // CX2GUElsword_SwordMan::WSS_VIGOR
		{
			if ( NULL != m_pStaticElswordVigor &&
				 NULL != m_pStaticElswordVigor->GetPicture( 1 ) &&
				 NULL != m_pStaticElswordVigor->GetPicture( 2 ) )
			{
				pGagePicture			= m_pStaticElswordVigor->GetPicture( 1 );
				pGageEffectPicture		= m_pStaticElswordVigor->GetPicture( 2 );
				fGageMax				= g_pData->GetDamageManager()->GetVigorStateThreshold();
				wstrTexPieceName		= L"EL_BLUE";
				wstrTexEffectPieceName	= L"EL_EFFECT";
			}
		}
		break;
	}

	if( NULL != pGagePicture && NULL != pGagePicture->pTexture && NULL != pGagePicture->pTexture->pTexture )
	{
		const CKTDXDeviceTexture::TEXTURE_UV* pTexUV			= pGagePicture->pTexture->pTexture->GetTexUV( wstrTexPieceName );

		if( pTexUV != NULL )
		{
			float fGageUVWidth = (pTexUV->rightTop.x - pTexUV->leftTop.x)  * ( pElswordGageData->GetWayOfSwordPoint() / fGageMax );


			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]			= pTexUV->leftTop;
			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]			= pTexUV->rightTop;
			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]		= pTexUV->leftBottom;
			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]		= pTexUV->rightBottom;

			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP].x		= pTexUV->leftTop.x + fGageUVWidth;
			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM].x		= pTexUV->leftBottom.x + fGageUVWidth;
			pGagePicture->pTexture->SetTextureUV();
		}

		float fGageWidth = pGagePicture->GetOriginalSize().x * ( pElswordGageData->GetWayOfSwordPoint() / fGageMax );
		pGagePicture->SetSizeX(fGageWidth);
	}

	if( NULL != pGageEffectPicture && NULL != pGageEffectPicture->pTexture && NULL != pGageEffectPicture->pTexture->pTexture )
	{
		const CKTDXDeviceTexture::TEXTURE_UV* pTexUV			= pGageEffectPicture->pTexture->pTexture->GetTexUV( wstrTexEffectPieceName );

		if( pTexUV != NULL )
		{
			float fGageUVWidth = (pTexUV->rightTop.x - pTexUV->leftTop.x)  * ( pElswordGageData->GetWayOfSwordPoint() / fGageMax );


			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]			= pTexUV->leftTop;
			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]		= pTexUV->rightTop;
			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]		= pTexUV->leftBottom;
			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]		= pTexUV->rightBottom;

			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP].x		= pTexUV->leftTop.x + fGageUVWidth;
			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM].x	= pTexUV->leftBottom.x + fGageUVWidth;
			pGageEffectPicture->pTexture->SetTextureUV();
		}

		float fGageWidth = pGageEffectPicture->GetOriginalSize().x * ( pElswordGageData->GetWayOfSwordPoint() / fGageMax );
		pGageEffectPicture->SetSizeX(fGageWidth);
	}


	return;
}

void CX2ElswordMyGageUI::UpdateUIElswordWSP()
{
	CX2ElswordGageData* pElswordGageData = static_cast<CX2ElswordGageData*>( m_pGageData );

	// 변경 된 것이 있는 경우에
	if ( pElswordGageData->GetChangedWayOfSwordState() )
	{
		StopUIEffectElswordWS();

		switch( pElswordGageData->GetWayOfSwordState() )
		{
		case -2: // CX2GUElsword_SwordMan::WSS_DESTRUCTION
			{
				m_pStaticElswordDest->SetShow( true );
				m_pStaticElswordDest->GetPicture( 0 )->SetShow( true );
				m_pStaticElswordDest->GetPicture( 1 )->SetShow( true );
				m_pStaticElswordDest->GetPicture( 2 )->SetShow( true );
				m_pStaticElswordDest->GetPicture( 2 )->SetFlicker( 0.4f, 1.0f, 0.1f );
				m_pStaticElswordDest->GetPicture( 3 )->SetShow( false );
				m_pStaticElswordDest->GetPicture( 4 )->SetShow( true );

				m_pStaticElswordVigor->SetShow( false );
				pElswordGageData->SetChangedWayOfSwordState( false );
				PlayUIEffectDestruction();
			}
			break;
		case -1: // CX2GUElsword_SwordMan::WSS_TOWARD_DESTRUCTION
			{
				m_pStaticElswordDest->SetShow( true );
				m_pStaticElswordDest->GetPicture( 0 )->SetShow( true );
				m_pStaticElswordDest->GetPicture( 1 )->SetShow( true );
				m_pStaticElswordDest->GetPicture( 2 )->SetShow( false );
				m_pStaticElswordDest->GetPicture( 3 )->SetShow( true );
				m_pStaticElswordDest->GetPicture( 4 )->SetShow( false );

				m_pStaticElswordVigor->SetShow( false );
				pElswordGageData->SetChangedWayOfSwordState( false );
			}
			break;
		case 0: // CX2GUElsword_SwordMan::WSS_CENTER
			{

				const float fWayOfSwordPoint = pElswordGageData->GetWayOfSwordPoint();

				if ( fWayOfSwordPoint >= g_pData->GetDamageManager()->GetVigorStateThreshold() )
					pElswordGageData->SetWayOfSwordState( 2 ); // WSS_VIGOR
				else if( fWayOfSwordPoint <= g_pData->GetDamageManager()->GetDestStateThreshold() )
					pElswordGageData->SetWayOfSwordState( -2 ); // WSS_DESTRUCTION;
				else if( fWayOfSwordPoint > 0 )
					pElswordGageData->SetWayOfSwordState( 1 ); // WSS_TOWARD_VIGOR;
				else if( fWayOfSwordPoint < 0 )
					pElswordGageData->SetWayOfSwordState( -1 ); // WSS_TOWARD_DESTRUCTION;
				else
				{
					m_pStaticElswordVigor->SetShow( true );
					m_pStaticElswordVigor->GetPicture( 0 )->SetShow( true );
					m_pStaticElswordVigor->GetPicture( 1 )->SetShow( false );
					m_pStaticElswordVigor->GetPicture( 2 )->SetShow( false );
					m_pStaticElswordVigor->GetPicture( 3 )->SetShow( false );
					m_pStaticElswordVigor->GetPicture( 4 )->SetShow( false );
					m_pStaticElswordVigor->GetPicture( 5 )->SetShow( true );

					m_pStaticElswordDest->SetShow( false );
					pElswordGageData->SetChangedWayOfSwordState( false );
				}
			}
			break;
		case 1: // CX2GUElsword_SwordMan::WSS_TOWARD_VIGOR
			{
				m_pStaticElswordVigor->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 0 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 1 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 2 )->SetShow( false );
				m_pStaticElswordVigor->GetPicture( 3 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 4 )->SetShow( false );
				m_pStaticElswordVigor->GetPicture( 5 )->SetShow( false );

				m_pStaticElswordDest->SetShow( false );
				pElswordGageData->SetChangedWayOfSwordState( false );
			}
			break;
		case 2: // CX2GUElsword_SwordMan::WSS_VIGOR
			{
				m_pStaticElswordVigor->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 0 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 1 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 2 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 2 )->SetFlicker( 0.4f, 1.0f, 0.1f );
				m_pStaticElswordVigor->GetPicture( 3 )->SetShow( false );
				m_pStaticElswordVigor->GetPicture( 4 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 5 )->SetShow( false );

				m_pStaticElswordDest->SetShow( false );
				pElswordGageData->SetChangedWayOfSwordState( false );
				PlayUIEffectVigor();
			}
			break;
		}

		if (g_pX2Game != NULL && CX2Main::XS_VILLAGE_MAP != g_pMain->GetNowStateID() )
			g_pX2Game->UpdateElswordSkillSlotEffect();
	}
}

void CX2ElswordMyGageUI::SetShowWspParticle( const bool bShowWspParticle_ )
{
	m_bShowWspParticle = bShowWspParticle_;

	CKTDGParticleSystem* pGameMinorParticle = g_pData->GetGameMinorParticle();

	for ( int i = 0; i < ARRAY_SIZE(m_hSeqVigorEffect); ++i)
	{
		if( m_hSeqVigorEffect[i] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = pGameMinorParticle->GetInstanceSequence( m_hSeqVigorEffect[i] );
			if( NULL != pSeq )
				pSeq->SetShowObject( bShowWspParticle_ );
		}

		if( m_hSeqDestEffect[i] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = pGameMinorParticle->GetInstanceSequence( m_hSeqDestEffect[i] );
			if( NULL != pSeq )
				pSeq->SetShowObject( bShowWspParticle_ );
		}
	}
}

CX2RavenMyGageUI::CX2RavenMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ )
	: CX2MyGageUI( pGageData_, eOwnerUnitClass_ ), m_hSeqHyperBall( INVALID_PARTICLE_SEQUENCE_HANDLE )
{}


/*virtual*/ CX2RavenMyGageUI::~CX2RavenMyGageUI()
{
	if( m_hSeqHyperBall != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle(m_hSeqHyperBall);
}

/*virtual*/ void CX2RavenMyGageUI::InitWakeOrb()
{
	CKTDGUIStatic* pStaticWakeOrb = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeOrb") );
	if ( NULL != pStaticWakeOrb )
	{
#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_EVE ) );			// 이브 구슬 삭제
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_ARA ) );			// 아라 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_CHUNG ) );		// 청 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB3 ) );				// 일반 구슬 삭제
	}

	// 각성 구슬 슬롯
	CKTDGUIStatic* pStaticWakeSlot = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeSlot") );
	if ( NULL != pStaticWakeSlot )
	{
#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_EVE ) );			// 이브 구슬 삭제
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_ARA ) );			// 아라 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_CHUNG ) );		// 청 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB3 ) );			// 일반 구슬 삭제
	}
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2RavenMyGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2RavenMyGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    CX2MyGageUI::OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	CX2MyGageUI::OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	UpdateHyperOrb();
}

void CX2RavenMyGageUI::UpdateHyperOrb()
{
	if ( m_pGageData->GetHyperModeCount() == 3 )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

		CKTDGParticleSystem* pUIMajorParticleSystem = g_pData->GetUIMajorParticle();

		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqHyperBall)
		{
			D3DXVECTOR3 vHyperBallPos = D3DXVECTOR3( 100.f * g_pKTDXApp->GetResolutionScaleX(), 120.f *g_pKTDXApp->GetResolutionScaleY(), 0.f );
			m_hSeqHyperBall = pUIMajorParticleSystem->CreateSequenceHandle( (CKTDGObject*) this,  L"raven_powrUp_UI_01", vHyperBallPos );
			if( pUIMajorParticleSystem->GetInstanceSequence(m_hSeqHyperBall ) != NULL )
			{
				pSeq = pUIMajorParticleSystem->GetInstanceSequence(m_hSeqHyperBall );
				pSeq->SetOverUI( true );
			}					
		}
		else
		{
			pSeq = pUIMajorParticleSystem->GetInstanceSequence(m_hSeqHyperBall);
			if( pSeq != NULL )
			{
				D3DXVECTOR3 vHyperBallPos = D3DXVECTOR3(100.f * g_pKTDXApp->GetResolutionScaleX(), 120.f *g_pKTDXApp->GetResolutionScaleY(), 0.f);
				pSeq->SetPosition( vHyperBallPos );
			}				
		}

#ifdef DIALOG_SHOW_TOGGLE
		if( pSeq != NULL )
		{
			if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
				pSeq->SetShowObject(false);
			else
				pSeq->SetShowObject(true);
		}
#endif
	}
	else
	{
		if( m_hSeqHyperBall != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem* pUIMajorParticleSystem = g_pData->GetUIMajorParticle();
			pUIMajorParticleSystem->DestroyInstanceHandle(m_hSeqHyperBall);
		}	
	}
}

/*virtual*/ CX2PartyMemberGageUI::~CX2PartyMemberGageUI()
{
#ifdef BUFF_ICON_UI
	ClearBuffList();
#endif //BUFF_ICON_UI
	SAFE_DELETE_DIALOG( m_pDLGOtherUnit );
}

void CX2PartyMemberGageUI::InitUI()
{
	SAFE_DELETE_DIALOG( m_pDLGOtherUnit );
	m_pDLGOtherUnit = new CKTDGUIDialog( NULL, L"DLG_PVP_Game_Other_State_NEW.lua" );
	
	CKTDGUIStatic* pStaticOtherPlayerMp 
		= ( NULL != m_pDLGOtherUnit ? 
		static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl( L"OtherPlayerMP" ) ) : NULL );

	if ( NULL != pStaticOtherPlayerMp )
	{
		CKTDGUIStatic::CPictureData* pPictureDataMp = pStaticOtherPlayerMp->GetPicture(0);
		if ( NULL != pPictureDataMp )
			pPictureDataMp->SetShow( true );
	}

	SetLevelString( m_uiLevel );
	SetNickNameString( m_wstrNickName );
	SetPosition( m_uiPositionIndex );
	SetCharacterImage( m_eOwnerGameUnitClass );

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템

	SAFE_DELETE ( m_pButtonVotePlayer );
	SAFE_DELETE ( m_pButtonVoteComplete );

	m_pButtonVotePlayer = static_cast<CKTDGUIButton*> ( m_pDLGOtherUnit->GetControl ( L"BanPlayer" ) );		// BanPlayer 버튼에 대한 정보를 파싱함

	//눌려진 버튼의 소유주를 확인하기 위해 dummy int 추가
	if( false == m_pButtonVotePlayer->SetDummyInt(0, m_uiPositionIndex ) )
		m_pButtonVotePlayer->AddDummyInt( m_uiPositionIndex );

	m_pButtonVoteComplete = static_cast<CKTDGUIButton*> ( m_pDLGOtherUnit->GetControl ( L"VoteComplete" ) );// VoteComplete 버튼에 대한 정보를 파싱함

	SetShowRelationVoteButtons( false, false );
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGOtherUnit );
}

void CX2PartyMemberGageUI::SetLevelString( const UINT uiLevel_ )
{
	m_uiLevel = uiLevel_;
	if ( NULL != m_pDLGOtherUnit )
	{
		 CKTDGUIStatic* pStaticLevel = 
			 static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl(L"StaticOtherPlayerLV") );

		 if ( NULL != pStaticLevel )
		 {
			 if( -1 == uiLevel_ )
			 {	//레벨이 -1로 설정되어있으면 안보여주기( 대전NPC)
				 pStaticLevel->SetShow(false);
				 return;
			 }

			 pStaticLevel->SetShow(true);
			 WCHAR wstrLv[10];
			 ZeroMemory( wstrLv, sizeof(WCHAR) * 10 );
#ifdef HARDCODING_STRING_BR
			 pStaticLevel->SetString( 0, L"Niv" );
#else // HARDCODING_STRING_BR
			 pStaticLevel->SetString( 0, L"Lv" );
#endif // HARDCODING_STRING_BR
			 StringCchPrintf( wstrLv, ARRAY_SIZE(wstrLv), L"%d", uiLevel_ );
			 pStaticLevel->SetString( 1, wstrLv );
		 }
	}
}

void CX2PartyMemberGageUI::SetNickNameString( const wstring& wstrNickName_ )
{
	m_wstrNickName = wstrNickName_;
	if ( NULL != m_pDLGOtherUnit )
	{
		CKTDGUIStatic* pStaticNickName = 
			static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl(L"StaticOtherPlayerID") );

		if ( NULL != pStaticNickName )
			pStaticNickName->SetString( 0, wstrNickName_.c_str() );			
	}
}

void CX2PartyMemberGageUI::SetPosition( const UINT uiPositionIndex_ )
{
	SetPositionIndex( uiPositionIndex_ );
	if ( NULL != m_pDLGOtherUnit )
	{
		D3DXVECTOR2 vPos( 6.f, 121.f + ( ( uiPositionIndex_ )  * 44 ) );

	#ifdef SKILL_SLOT_UI_TYPE_B
		if( NULL != g_pMain )
		{
			if( false == g_pMain->GetGameOption().GetIsSkillUITypeA() )
				vPos.y += 20.f;
		}
	#endif //SKILL_SLOT_UI_TYPE_B
		m_pDLGOtherUnit->SetPos( vPos );
	}

	if( NULL != m_pDlgBuffIcon )
	{
		m_bIsSkillUITypeA = g_pMain->GetGameOption().GetIsSkillUITypeA();
		D3DXVECTOR2 vPos(150.f, 125.f+ (m_uiPositionIndex * 44));
		if( false == g_pMain->GetGameOption().GetIsSkillUITypeA() )
			vPos.y += 20.f;
		m_pDlgBuffIcon->SetPos( vPos );	
	}

}

/*virtual*/ void CX2PartyMemberGageUI::SetCharacterImage( const CX2Unit::UNIT_CLASS eGameUnitClass_ )
{
	SetOwnerUnitClass( eGameUnitClass_ );
	if ( NULL != m_pDLGOtherUnit )
	{
		CKTDGUIStatic*	pStaticCharPictures = static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl( L"StaticPVPStateFace" ) );
		CKTDGUIControl::CPictureData* pPictureCharacterImage = ( NULL != pStaticCharPictures ? pStaticCharPictures->GetPicture( 0 ) : NULL );

		if( NULL != pPictureCharacterImage )
		{
			wstring fileName;
			wstring pieceName;

			if( true == CX2Data::GetCharacterImageName( fileName, pieceName, eGameUnitClass_, CX2Data::CIT_Party ) )
			{
				pStaticCharPictures->GetPicture( 0 )->SetTex( fileName.c_str(), pieceName.c_str() );
			}
			else
			{
				ASSERT( !"CharacterImage is not existed" );
			}
		}
	}	
}

/*virtual*/ void CX2PartyMemberGageUI::SetShow( const bool bShow_ )
{
	CX2GageUI::SetShow( bShow_ );

	if ( NULL != m_pDLGOtherUnit )
		m_pDLGOtherUnit->SetShow( bShow_ );

#ifdef BUFF_ICON_UI
	if( NULL != m_pDlgBuffIcon )
		m_pDlgBuffIcon->SetShow( bShow_ );
#endif // BUFF_ICON_UI
}


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2PartyMemberGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2PartyMemberGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	ASSERT( NULL != m_pGageData );
	if ( NULL != m_pGageData )
	{
		CKTDGUIStatic* pStaticOtherPlayerHp 
			= ( NULL != m_pDLGOtherUnit ? 
			static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl( L"OtherPlayerHP" ) ) : NULL );

		if ( NULL != pStaticOtherPlayerHp )
		{
			const float fMaxHp = m_pGageData->GetMaxHp();
			const float fHpPercent = m_pGageData->GetNowHp() / fMaxHp;

			UpdateGageForUV( pStaticOtherPlayerHp, 0, L"PARTY_HP", fHpPercent, true );
		}

		CKTDGUIStatic* pStaticOtherPlayerMp 
			= ( NULL != m_pDLGOtherUnit ? 
			static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl( L"OtherPlayerMP" ) ) : NULL );

		if ( NULL != pStaticOtherPlayerMp )
		{
			const float fMaxMp = m_pGageData->GetMaxMp();
			const float fMpPercent = m_pGageData->GetNowMp() / fMaxMp;

			UpdateGageForUV( pStaticOtherPlayerMp, 0, L"PARTY_MP", fMpPercent, true );
		}
		if ( m_pDLGOtherUnit != NULL && m_pDLGOtherUnit->GetShow() == true )
		{
			switch ( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_BATTLE_FIELD:
			case CX2Main::XS_VILLAGE_MAP:
				{
					if ( g_pKTDXApp->GetDIManager()->GetMouse()->GetPureButtonState( MOUSE_RBUTTON ) == TRUE )
					{
						D3DXVECTOR2 vResoultion( g_pKTDXApp->GetResolutionScaleX(), g_pKTDXApp->GetResolutionScaleY() );
						D3DXVECTOR2 leftTop = m_pDLGOtherUnit->GetPos();
						leftTop.x *= vResoultion.x;
						leftTop.y *= vResoultion.y;
						D3DXVECTOR2 RigthBottom( leftTop.x + m_pDLGOtherUnit->GetSize().x * vResoultion.x, leftTop.y + m_pDLGOtherUnit->GetSize().y * vResoultion.y );
						D3DXVECTOR2 mousePoint( (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(), (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos() );
						
						if ( mousePoint.x >= leftTop.x && mousePoint.x <= RigthBottom.x && 
							mousePoint.y >= leftTop.y && mousePoint.y <= RigthBottom.y )
						{
							m_bOnPopUp = true;
							g_pData->GetPetManager()->ClosePetPopupMenu();

							CX2PartyManager::PartyMemberData* pPartyUnitData = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( this->m_wstrNickName );
							if ( NULL != pPartyUnitData )
							{
								CX2PartyManager* pPartyManager = g_pData->GetPartyManager();
								bool bIsParty = false;

								if ( NULL != pPartyManager && true == pPartyManager->DoIHaveParty() )
									bIsParty = true;

								g_pData->GetUserMenu()->OpenUserPopupMenu( pPartyUnitData->m_iUnitUID, bIsParty );
							}
						}
						else
						{
							if ( true == m_bOnPopUp )
								g_pData->GetUserMenu()->ClosePopupMenu();

							m_bOnPopUp = false;
						}
					}
				}
				break;
			}
		}
		UpdateResurrectShortKey();
		UpdatePartyLeaderUI();

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		CX2GageManager * pGageManager = CX2GageManager::GetInstance();
		if ( NULL != pGageManager )
		{	// 불량 유저에 대한 UI 를 업데이트 한다.c
			UpdateFaultyPlayerUI ( pGageManager->GetMyUserIsFaultyPlayer()  );				
		}
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

	}
}
/** @function : UpdateResurrectString
	@brief : 파티원 부활 단축키 안내 갱신
*/
void CX2PartyMemberGageUI::UpdateResurrectShortKey()
{
	CKTDGUIStatic* pStaticResurrect = static_cast<CKTDGUIStatic*>(m_pDLGOtherUnit->GetControl( L"StaticResurrectOtherPlayer" ));
	if ( pStaticResurrect != NULL && pStaticResurrect->GetString(0) != NULL )
	{
		pStaticResurrect->SetShow(true);

		if(g_pData->GetDungeonRoom() != NULL )
		{
			switch(g_pData->GetDungeonRoom()->GetDungeonID())
			{
	#ifdef CHILDRENS_DAY_EVENT_DUNGEON
			case SEnum::DI_EVENT_KIDDAY_RUBEN:
			case SEnum::DI_EVENT_KIDDAY_ELDER:
			case SEnum::DI_EVENT_KIDDAY_BESMA:
			case SEnum::DI_EVENT_KIDDAY_ALTERA:
				{
					pStaticResurrect->SetShow(false);
					return;
				} break;
	#endif CHILDRENS_DAY_EVENT_DUNGEON
			case SEnum::DI_ELDER_HENIR_SPACE:
				{					
					if( (CX2Dungeon::DUNGEON_MODE) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode == CX2Dungeon::DM_HENIR_CHALLENGE )
					{
						pStaticResurrect->SetShow(false);
						return;
					}
					else
						pStaticResurrect->SetShow(true);
				} break;
			default:
				break;
			}
		}

		pStaticResurrect->GetString(0)->msg = L"";

		if ( NULL != g_pX2Game &&  NULL != g_pX2Game->GetMyUnit() &&
			 NULL != g_pX2Game->GetMyUnit()->GetUnit() &&
			CX2Game::GT_PVP != g_pX2Game->GetGameType() &&
			m_pGageData->GetNowHp() <= 0 )
		{
			if ( g_pX2Game->GetMyUnit()->GetUnit()->GetResurrectionStoneNum() <= 0 
#ifdef AUTO_PAYMENT	// 빌드 오류로 해외팀 추가
				&& g_pX2Game->GetMyUnit()->GetUnit()->GetAutoResStoneNum() <= 0
#endif	//	AUTO_PAYMENT
				 )
			{
				switch(m_uiPositionIndex)
				{
				case 0:	pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_181 );	break;
				case 1:	pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_182 );	break;
				case 2:	pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_183 );	break;
				}
			}
			else
			{
				switch(m_uiPositionIndex)
				{
				case 0:	pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_184 );	break;
				case 1:	pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_185 );	break;
				case 2:	pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_186 );	break;
				}
			}
		}
	}	
}

void CX2PartyMemberGageUI::UpdatePartyLeaderUI()
{
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGOtherUnit->GetControl(L"g_pStaticPartyLeader"));
	if( NULL != pStatic )
	{
		if( NULL != g_pData && NULL != g_pData->GetPartyManager() )
		{
			bool bShow = (m_uidGameUnit == g_pData->GetPartyManager()->GetMyPartyData()->m_iPartyLeaderUID);
			if( NULL != g_pX2Game && CX2Game::GT_PVP == g_pX2Game->GetGameType() )
				bShow = false;

			pStatic->SetShow(bShow);
		}
	}
}

#ifdef BUFF_ICON_UI
/** @function : UpdateBuffIcon
	@brief : 버프 아이콘 UI 를 갱신하는 함수
			 위치 지정을 위해 CX2MyGageUI, CX2PartyMemberGageUI에서 각각 재정의
*/
void CX2PartyMemberGageUI::UpdateBuffIcon()
{
	if( NULL == m_pDlgBuffIcon )
	{
		m_pDlgBuffIcon = new CKTDGUIDialog( NULL, L"DLG_BuffManager.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgBuffIcon );		
		m_pDlgBuffIcon->SetShow(true);		

		D3DXVECTOR2 vPos(150.f, 125.f+ (m_uiPositionIndex * 44));
#ifdef SKILL_SLOT_UI_TYPE_B		
		if( NULL != g_pMain )
		{
			m_bIsSkillUITypeA = g_pMain->GetGameOption().GetIsSkillUITypeA();
			if( false == m_bIsSkillUITypeA )
				vPos.y += 20.f;
		}
#endif //SKILL_SLOT_UI_TYPE_B		
		m_pDlgBuffIcon->SetPos( vPos );
	}
#ifdef SKILL_SLOT_UI_TYPE_B
	if( m_bIsSkillUITypeA != g_pMain->GetGameOption().GetIsSkillUITypeA() )
	{
		m_bIsSkillUITypeA = g_pMain->GetGameOption().GetIsSkillUITypeA();
		D3DXVECTOR2 vPos(150.f, 125.f+ (m_uiPositionIndex * 44));
		if( false == g_pMain->GetGameOption().GetIsSkillUITypeA() )
			vPos.y += 20.f;
		m_pDlgBuffIcon->SetPos( vPos );
	}
#endif //SKILL_SLOT_UI_TYPE_B

	if( NULL != m_pDlgBuffIcon )
	{	//버프 리스트
		int iIndex = 0;
		bool bIsDebuff = false;
		vector<BuffIcon>::iterator it = m_vecBuffList.begin();
		for( ; it != m_vecBuffList.end(); ++it)
		{			
			it->bNew = false; //파티원의 버프아이콘은 출력 효과 없음
			SetBuffIconStatic( (*it), iIndex--, bIsDebuff, D3DXVECTOR2(16,16));
		}

		//디버프 리스트
		iIndex = 0;
		bIsDebuff = true;
		for( it = m_vecDebuffList.begin(); it != m_vecDebuffList.end(); ++it)
		{
			it->bNew = false; //파티원의 버프아이콘은 출력 효과 없음
			SetBuffIconStatic( (*it), iIndex--, bIsDebuff, D3DXVECTOR2(16,16));
		}
	}
}
#endif //BUFF_ICON_UI


#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템

void CX2PartyMemberGageUI::SetShowRelationVoteButtons ( const bool bVote_, const bool bComplete_ )
{
	CX2GageManager * pGageManager = CX2GageManager::GetInstance();

	if ( NULL != m_pButtonVotePlayer )
	{
		m_pButtonVotePlayer->SetShowEnable( bVote_, bVote_ );				// 강퇴 하기 버튼을 보여주고 활성화 시키거나 말거나
	}

	if ( NULL != m_pButtonVoteComplete )
	{
		m_pButtonVoteComplete->SetShowEnable( bComplete_, false );	// 투표 완료 버튼을 보여주고 활성화 시키거나 말거나
		// 원래는 m_pButtonVoteComplete->SetShowEnable( bComplete_, bComplete_ ); 였는데 입력 막아달라는 요청으로 변경
	}
}


void CX2PartyMemberGageUI::UpdateFaultyPlayerUI( const bool b_faultyValue_ )			// 해당 유저의 버튼 타입을 받아서 상황에 맞게 보여준다.
{
	
	if ( true == b_faultyValue_ ) // UI 를 가진 소유자가 불량 유저라면
	{
		SetShowRelationVoteButtons( false, false ); // 무조건 UI 를 숨긴다.
	}

	else						// 불량 유저가 아니라면 각 타입에 맞게 뿌려준다.
	{
		int iVoteButtonType = GetVoteButtonType ();
		switch ( iVoteButtonType )
		{
		case BVBT_NONE :
			SetShowRelationVoteButtons( false, false );
			break;

		case BVBT_NEED_VOTE :		
			SetShowRelationVoteButtons( true, false );	// 투표 가능 버튼으로 변경한다.
			break;

		case BVBT_COMPLETE_VOTE :
			SetShowRelationVoteButtons( false, true );	 // UI 를 가진 소유자가 불량 유저라면
			break;
		}
	}
}

#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템


/*virtual*/ CX2PVPPlayerGageUI::~CX2PVPPlayerGageUI()
{
	SAFE_DELETE_DIALOG( m_pDLGOtherUnit );
}

void CX2PVPPlayerGageUI::InitUI()
{
	SAFE_DELETE_DIALOG( m_pDLGOtherUnit );
	m_pDLGOtherUnit = new CKTDGUIDialog( NULL, L"DLG_PVP_Game_Other_State_NEW.lua" );

	CKTDGUIStatic* pStaticOtherPlayerMp 
		= ( NULL != m_pDLGOtherUnit ? 
		static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl( L"OtherPlayerMP" ) ) : NULL );

	if ( NULL != pStaticOtherPlayerMp )
	{
		CKTDGUIStatic::CPictureData* pPictureDataMp = pStaticOtherPlayerMp->GetPicture(0);
		if ( NULL != pPictureDataMp )
			pPictureDataMp->SetShow( true );
	}

	SetLevelString( m_uiLevel );
	SetNickNameString( m_wstrNickName );
	SetPosition( m_uiPositionIndex );

	/*

	if( m_pGameUnit->IsPvpBot() == true )
	{
		CX2GUNPC *pNpc = (CX2GUNPC*)m_pGameUnit;
		CKTDGUIControl::CPictureData* pPicture = pTempStatic->GetPicture(0);

		wstring fileName;
		wstring pieceName;
		if( CX2Data::GetPvpNpcImageName( fileName, pieceName, pNpc->GetNPCTemplet().m_nNPCUnitID ) == true )
		{
			pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
		}	
	}
*/
	SetCharacterImage( m_eOwnerGameUnitClass );
	SetRankImage();

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGOtherUnit );
}

void CX2PVPPlayerGageUI::SetRankImage()
{
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGOtherUnit->GetControl(L"g_pStaticPartyRank"));
	if( NULL != pStatic && NULL != pStatic->GetPicture(0) &&
		NULL != g_pMain && NULL != g_pMain->GetPVPEmblem() )
	{
		CX2PVPEmblem::PVPEmblemData* pEmbleData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( static_cast<CX2PVPEmblem::PVP_RANK>( m_cRank ) );
		if( NULL != pEmbleData )
		{
			pStatic->GetPicture(0)->SetTex( pEmbleData->m_TextureName.c_str(),	pEmbleData->m_TextureKey.c_str());
		}
		else
		{
			pStatic->GetPicture(0)->SetTex( L"DLG_Common_New_Texture03.tga", L"invisible" );
		}
	}
}
void CX2PVPPlayerGageUI::SetLevelString( const UINT uiLevel_ )
{
	m_uiLevel = uiLevel_;
	if ( NULL != m_pDLGOtherUnit )
	{
		 CKTDGUIStatic* pStaticLevel = 
			 static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl(L"StaticOtherPlayerLV") );

		 if ( NULL != pStaticLevel )
		 {
			 if( -1 == uiLevel_ )
			 {	//레벨이 -1로 설정되어있으면 안보여주기( 대전NPC)
				 pStaticLevel->SetShow(false);
				 return;
			 }
			 pStaticLevel->SetShow(true);

			 WCHAR wstrLv[10];
			 ZeroMemory( wstrLv, sizeof(WCHAR) * 10 );
#ifdef HARDCODING_STRING_BR
			 pStaticLevel->SetString( 0, L"Niv" );
#else // HARDCODING_STRING_BR
			 pStaticLevel->SetString( 0, L"Lv" );
#endif // HARDCODING_STRING_BR
			 StringCchPrintf( wstrLv, ARRAY_SIZE(wstrLv), L"%d", uiLevel_ );
			 pStaticLevel->SetString( 1, wstrLv );
		 }
	}
}

void CX2PVPPlayerGageUI::SetNickNameString( const wstring& wstrNickName_ )
{
	m_wstrNickName = wstrNickName_;
	if ( NULL != m_pDLGOtherUnit )
	{
		CKTDGUIStatic* pStaticNickName = 
			static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl(L"StaticOtherPlayerID") );

		if ( NULL != pStaticNickName )
			pStaticNickName->SetString( 0, wstrNickName_.c_str() );			
	}
}

void CX2PVPPlayerGageUI::SetPosition( const UINT uiPositionIndex_ )
{
	SetPositionIndex( uiPositionIndex_ );

	if ( NULL != m_pDLGOtherUnit )
	{
		D3DXVECTOR2 vPos( 0, 0 );

		if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
		{
			UINT uiTeam = g_pX2Game->GetMyUnit()->GetTeam();

			if ( m_uiMyTeam == uiTeam )
			{
				vPos.x = 6.f;
				vPos.y = 121.f + ( uiPositionIndex_ * 44 );
	#ifdef SKILL_SLOT_UI_TYPE_B
				if( NULL != g_pMain )
				{
					if( false == g_pMain->GetGameOption().GetIsSkillUITypeA() )
						vPos.y += 20.f;
				}
	#endif //SKILL_SLOT_UI_TYPE_B
			}
			else
			{
				vPos.x = 868.f;
				vPos.y = 121.f + ( uiPositionIndex_ * 44 ) - 44;
			}
		}
#ifdef FIX_OBSERVER_MODE
		/// 옵저버일 땐 게이지 UI 위치 설정 새로 해주자.
		else if( g_pX2Room != NULL && g_pX2Room->GetMySlot() != NULL && g_pX2Room->GetMySlot()->m_bObserver == true )
		{
			if ( m_uiMyTeam == CX2Room::TN_RED )
			{
				const UINT uiPositionIndexRed = g_pX2Game->GetPositionIndexRed();

				vPos.x = 6.f;
				vPos.y = 121.f + ( uiPositionIndexRed * 44 ) - 44;

				g_pX2Game->SetPositionIndexRed( uiPositionIndexRed + 1 );
			}
			else
			{
				const UINT uiPositionIndexBlue = g_pX2Game->GetPositionIndexBlue();

				vPos.x = 868.f;
				vPos.y = 121.f + ( uiPositionIndexBlue * 44 ) - 44;

				g_pX2Game->SetPositionIndexBlue( uiPositionIndexBlue + 1 );
			}
		}
#endif FIX_OBSERVER_MODE

		m_pDLGOtherUnit->SetPos( vPos );
	}

	if( NULL != m_pDlgBuffIcon )
	{
		D3DXVECTOR2 vPos(150.f, 125.f+ (m_uiPositionIndex * 44));
		m_bIsSkillUITypeA = g_pMain->GetGameOption().GetIsSkillUITypeA();
		if( false == g_pMain->GetGameOption().GetIsSkillUITypeA() )
			vPos.y += 20.f;
		m_pDlgBuffIcon->SetPos( vPos );
	}
}

/*virtual*/ void CX2PVPPlayerGageUI::SetCharacterImage( const CX2Unit::UNIT_CLASS eGameUnitClass_ )
{
	SetOwnerUnitClass( eGameUnitClass_ );
	if ( NULL != m_pDLGOtherUnit )
	{
		CKTDGUIStatic*	pStaticCharPictures = static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl( L"StaticPVPStateFace" ) );
		CKTDGUIControl::CPictureData* pPictureCharacterImage = ( NULL != pStaticCharPictures ? pStaticCharPictures->GetPicture( 0 ) : NULL );

		if( NULL != pPictureCharacterImage )
		{
			wstring fileName = L"";
			wstring pieceName = L"";
			
			if( CX2Unit::UC_NONE == eGameUnitClass_ )
			{
				CX2Data::GetPvpNpcImageName( fileName, pieceName, static_cast<CX2UnitManager::NPC_UNIT_ID>(m_iNpcId) );
			}
			else
			{
				CX2Data::GetCharacterImageName( fileName, pieceName, eGameUnitClass_, CX2Data::CIT_Party ) ;
			}

			if( !fileName.empty() && !pieceName.empty() )
			{
				pStaticCharPictures->GetPicture( 0 )->SetTex( fileName.c_str(), pieceName.c_str() );
			}
			else
			{
				ASSERT( !"CharacterImage is not existed" );
			}
		}

		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGOtherUnit->GetControl(L"g_pStaticPartyLeader"));
		if( NULL != pStatic )
		{
			pStatic->SetShow(false);
		}
	}	
}

/*virtual*/ void CX2PVPPlayerGageUI::SetShow( const bool bShow_ )
{
	CX2GageUI::SetShow( bShow_ );

	if ( NULL != m_pDLGOtherUnit )
		m_pDLGOtherUnit->SetShow( bShow_ );

#ifdef BUFF_ICON_UI
	if( NULL != m_pDlgBuffIcon )
		m_pDlgBuffIcon->SetShow( bShow_ );
#endif // BUFF_ICON_UI
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2PVPPlayerGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2PVPPlayerGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	ASSERT( NULL != m_pGageData );
	if ( NULL != m_pGageData )
	{
		CKTDGUIStatic* pStaticOtherPlayerHp 
			= ( NULL != m_pDLGOtherUnit ? 
			static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl( L"OtherPlayerHP" ) ) : NULL );

		if ( NULL != pStaticOtherPlayerHp )
		{
			const float fMaxHp = m_pGageData->GetMaxHp();
			const float fHpPercent = m_pGageData->GetNowHp() / fMaxHp;

			UpdateGageForUV( pStaticOtherPlayerHp, 0, L"PARTY_HP", fHpPercent, true );
		}

		CKTDGUIStatic* pStaticOtherPlayerMp 
			= ( NULL != m_pDLGOtherUnit ? 
			static_cast<CKTDGUIStatic*>( m_pDLGOtherUnit->GetControl( L"OtherPlayerMP" ) ) : NULL );

		if ( NULL != pStaticOtherPlayerMp )
		{
			const float fMaxMp = m_pGageData->GetMaxMp();
			const float fMpPercent = m_pGageData->GetNowMp() / fMaxMp;

			UpdateGageForUV( pStaticOtherPlayerMp, 0, L"PARTY_MP", fMpPercent, true );
		}
		UpdatePvpRank();
	}
}

void CX2PVPPlayerGageUI::UpdatePvpRank()
{
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGOtherUnit->GetControl(L"g_pStaticPartyRank"));
	if( NULL != pStatic && NULL != pStatic->GetPicture(0))
	{
		if( NULL != g_pX2Game && CX2Game::GT_PVP == g_pX2Game->GetGameType() )
		{
			pStatic->SetShow(true);
		}
		else
		{
			pStatic->SetShow(false);
		}
	}
}

#ifdef BUFF_ICON_UI
/** @function : UpdateBuffIcon
	@brief : 버프 아이콘 UI 를 갱신하는 함수
			 위치 지정을 위해 CX2MyGageUI, CX2PVPPlayerGageUI에서 각각 재정의
*/
void CX2PVPPlayerGageUI::UpdateBuffIcon()
{
	if( NULL == m_pDlgBuffIcon )
	{
		m_pDlgBuffIcon = new CKTDGUIDialog( NULL, L"DLG_BuffManager.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgBuffIcon );		
		m_pDlgBuffIcon->SetShow(true);		

		D3DXVECTOR2 vPos(150.f, 125.f+ (m_uiPositionIndex * 44));
#ifdef SKILL_SLOT_UI_TYPE_B
		{
			m_bIsSkillUITypeA = g_pMain->GetGameOption().GetIsSkillUITypeA();
			if( false == m_bIsSkillUITypeA )
				vPos.y += 20.f;
		}
#endif //SKILL_SLOT_UI_TYPE_B
		m_pDlgBuffIcon->SetPos( vPos );
	}
#ifdef SKILL_SLOT_UI_TYPE_B
	if( m_bIsSkillUITypeA != g_pMain->GetGameOption().GetIsSkillUITypeA())
	{
		m_bIsSkillUITypeA = g_pMain->GetGameOption().GetIsSkillUITypeA();
		D3DXVECTOR2 vPos(150.f, 125.f+ (m_uiPositionIndex * 44));
		if( false == g_pMain->GetGameOption().GetIsSkillUITypeA() )
			vPos.y += 20.f;
		m_pDlgBuffIcon->SetPos( vPos );
	}
#endif //SKILL_SLOT_UI_TYPE_B

	if( NULL != m_pDlgBuffIcon )
	{	//버프 리스트
		int iIndex = 0;
		bool bIsDebuff = false;
		vector<BuffIcon>::iterator it = m_vecBuffList.begin();
		for( ; it != m_vecBuffList.end(); ++it)
		{			
			it->bNew = false; //파티원의 버프아이콘은 출력 효과 없음
			SetBuffIconStatic( (*it), iIndex--, bIsDebuff, D3DXVECTOR2(16,16));
		}

		//디버프 리스트
		iIndex = 0;
		bIsDebuff = true;
		for( it = m_vecDebuffList.begin(); it != m_vecDebuffList.end(); ++it)
		{
			it->bNew = false; //파티원의 버프아이콘은 출력 효과 없음
			SetBuffIconStatic( (*it), iIndex--, bIsDebuff, D3DXVECTOR2(16,16));
		}
	}
}
#endif //BUFF_ICON_UI

#pragma region CX2AraMyGageUI

#pragma region CX2AraMyGageUI
/** @function 	: CX2AraMyGageUI
	@brief 		: 생성자
*/
CX2AraMyGageUI::CX2AraMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ )
	: CX2MyGageUI( pGageData_, eOwnerUnitClass_ )
{
}
#pragma endregion 생성자

#pragma region InitUI
/** @function 	: InitUI
	@brief 		: UI 초기화
*/
/*virtual*/ void CX2AraMyGageUI::InitUI()
{
	InitStatusUI();

	m_pDLGMyUnit->OpenScriptFile( L"DLG_Ara_ForcePower.lua" );

	InitWakeOrb();
	InitForcePowerUI();
}
#pragma endregion UI 초기화

#pragma region InitWakeOrb
/** @function 	: InitWakeOrb
	@brief 		: 다른 캐릭터의 3각성 구슬 이미지 제거
*/
/*virtual*/ void CX2AraMyGageUI::InitWakeOrb()
{
	CKTDGUIStatic* pStaticWakeOrb = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeOrb") );
	if ( NULL != pStaticWakeOrb )
	{
#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_EVE ) );		// 이브 구슬 삭제
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_RAVEN ) );	// 레이븐 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_CHUNG ) );	// 청 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB3 ) );			// 일반 구슬 삭제
	}

	// 각성 구슬 슬롯
	CKTDGUIStatic* pStaticWakeSlot = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeSlot") );
	if ( NULL != pStaticWakeSlot )
	{
#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_EVE ) );		// 이브 구슬 삭제
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_RAVEN ) );	// 레이븐 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_CHUNG ) );	// 청 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB3 ) );		// 일반 구슬 삭제

	}
}
#pragma endregion 다른 캐릭터의 3각성 구슬 이미지 제거

#pragma region InitForcePowerUI
/** @function 	: InitForcePowerUI
	@brief 		: 기력 UI 초기화
*/
void CX2AraMyGageUI::InitForcePowerUI()
{
	CKTDGUIStatic* pStaticForcePower = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"Ara_ForcePower" ) );

	ASSERT( NULL != pStaticForcePower );
	if ( NULL != pStaticForcePower )
		pStaticForcePower->SetShow( true );
}
#pragma endregion 기력 UI 초기화

#pragma region OnFrameMove
/** @function 	: OnFrameMove
	@brief 		: 온프레임무브
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2AraMyGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2AraMyGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    CX2MyGageUI::OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    CX2MyGageUI::OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	UpdateCharacterImage();
	UpdateForcePowerUI();
}
#pragma endregion 온프레임무브

#pragma region UpdateCharacterImage
/** @function 	: UpdateCharacterImage
	@brief 		: 캐릭터 이미지 갱신
*/
void CX2AraMyGageUI::UpdateCharacterImage()
{
	CX2AraGageData* pAraGageData = static_cast<CX2AraGageData*>( m_pGageData );
	CKTDGUIStatic* pStaticCharPictures = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"StaticPVPGameMyChar" ) );

	if( NULL != pStaticCharPictures->GetPicture( 0 ) )
	{
		wstring wstrFileName;
		wstring wstrPieceName;

		if( true == pAraGageData->GetBerserkMode() &&
			NULL != g_pX2Game )
		{
			GetBerserkModeMyStateImage( wstrFileName, wstrPieceName, GetOwnerUnitClass() );
		}
		else
		{
			CX2Data::GetCharacterImageName( wstrFileName, wstrPieceName, GetOwnerUnitClass(), CX2Data::CIT_MyGage );
		}

		ASSERT( !wstrFileName.empty() && !wstrPieceName.empty() );

		pStaticCharPictures->GetPicture( 0 )->SetTex( wstrFileName.c_str(), wstrPieceName.c_str() );
		pStaticCharPictures->GetPicture( 0 )->SetSizeAsTextureSize();

		pAraGageData->SetBerserkModeChanged( false );
	}
}
#pragma endregion 캐릭터 이미지 갱신

#pragma region UpdateForcePowerCountUI
/** @function 	: UpdateForcePowerUI
	@brief 		: 기력 UI 갱신
*/
void CX2AraMyGageUI::UpdateForcePowerUI()
{
	CX2AraGageData* pAraGageData = static_cast<CX2AraGageData*>( m_pGageData );

	if ( pAraGageData->GetForcePowerChanged() )
	{
		pAraGageData->SetForcePowerChanged( false );

		CKTDGUIStatic* pStaticForcePower = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"Ara_ForcePower" ) );
		
		ASSERT( NULL != pStaticForcePower );
		if ( NULL != pStaticForcePower )
		{
			int iNumOfPicture = pStaticForcePower->GetPictureNum();
			D3DXVECTOR2 vPos( 0.f, 0.f );

			for ( int iPictureIndex = 0; iPictureIndex < iNumOfPicture ; ++iPictureIndex )
			{
				bool bShowPictureDataForcePower = false;

				if ( pAraGageData->GetMaxForcePower() - 2 >= iPictureIndex  )
					bShowPictureDataForcePower = true;

				CKTDGUIControl::CPictureData* pPictureDataForcePower = pStaticForcePower->GetPicture( iPictureIndex );
				ASSERT( NULL != pPictureDataForcePower );
				pPictureDataForcePower->SetShow( bShowPictureDataForcePower );
				
				if ( true == bShowPictureDataForcePower )
					vPos = pPictureDataForcePower->GetPos();
			}

			CKTDGUIControl::CPictureData* pPictureDataForcePower = pStaticForcePower->GetPicture( iNumOfPicture - 1 );
			ASSERT( NULL != pPictureDataForcePower );
			vPos.x += 20.f;
			pPictureDataForcePower->SetPos( vPos );
			pPictureDataForcePower->SetShow( true );
		}

		CKTDGUIStatic* pStaticForcePowerBall = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"Ara_ForcePower_Ball" ) );

		ASSERT( NULL != pStaticForcePowerBall );
		if ( NULL != pStaticForcePowerBall )
		{
			int iNumOfPicture = pStaticForcePowerBall->GetPictureNum();
						
			for ( int iPictureIndex = 0; iPictureIndex < iNumOfPicture ; ++iPictureIndex )
			{
				bool bShowPictureDataForcePower = false;

				if ( pAraGageData->GetNowForcePower() > iPictureIndex  )
					bShowPictureDataForcePower = true;
				
				CKTDGUIControl::CPictureData* pPictureDataForcePower = pStaticForcePowerBall->GetPicture( iPictureIndex );
				ASSERT( NULL != pPictureDataForcePower );
				pPictureDataForcePower->SetShow( bShowPictureDataForcePower );
			}

			pStaticForcePowerBall->SetShow( true );
		}
	}
}
#pragma endregion 기력 UI 갱신

#pragma region GetBerserkModeMyStateImage
/** @function 	: GetBerserkModeMyStateImage
	@brief 		: 광폭화 모드 이미지 얻기
*/
void CX2AraMyGageUI::GetBerserkModeMyStateImage( OUT wstring& wstrFileName, OUT wstring& wstrPieceName
												 , IN const CX2Unit::UNIT_CLASS eClassType  )
{
	switch ( eClassType )
	{
	case CX2Unit::UC_ARA_MARTIAL_ARTIST:
		wstrFileName	= L"DLG_UI_Common_Texture68_NEW.tga";
		wstrPieceName	= L"ARA_FACE_RAGE";
		break;
#ifdef ARA_CHANGE_CLASS_FIRST
	case CX2Unit::UC_ARA_LITTLE_HSIEN:
		wstrFileName	= L"DLG_UI_Common_Texture68_NEW.tga";
		wstrPieceName	= L"ARA_LH_FACE_RAGE";
		break;
	case CX2Unit::UC_ARA_SAKRA_DEVANAM:
		wstrFileName	= L"DLG_UI_Common_Texture70_NEW.tga";
		wstrPieceName	= L"ARA_SD_FACE_RAGE";
		break;
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CX2Unit::UC_ARA_LITTLE_DEVIL:
		wstrFileName	= L"DLG_UI_Common_Texture77_NEW.tga";
		wstrPieceName	= L"L_DEVIL_STATE_RAGE";
		break;
	case CX2Unit::UC_ARA_YAMA_RAJA:
		wstrFileName	= L"DLG_UI_Common_Texture77_NEW.tga";
		wstrPieceName	= L"Y_RAJA_STATE_RAGE";
		break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	default:
		break;
	} // switch
}
#pragma endregion 광폭화 모드 이미지 얻기

#pragma endregion 클래스

#ifdef NEW_CHARACTER_EL
CX2ElesisMyGageUI::~CX2ElesisMyGageUI()
{
	m_pStaticElswordVigor	= NULL;
	m_pStaticElswordDest	= NULL;

	StopUIEffectElswordWS();
}

void CX2ElesisMyGageUI::InitUI()
{
#if !defined(WORLD_TOOL)
	InitStatusUI();

	m_pDLGMyUnit->OpenScriptFile( L"DLG_Elesis_WayOfSword.lua" ); 

	InitUIElswordWSP();
	InitWakeOrb();
#endif //WORLD_TOOL
}

/*virtual*/ void CX2ElesisMyGageUI::SetShow( const bool bShow_ )
{
	CX2MyGageUI::SetShow( bShow_ );

	CKTDGParticleSystem* pGameMinorParticle = g_pData->GetGameMinorParticle();

	for ( int i = 0; i < ARRAY_SIZE( m_hSeqVigorEffect ); ++i )
	{
		if( m_hSeqVigorEffect[i] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = pGameMinorParticle->GetInstanceSequence( m_hSeqVigorEffect[i] );
			if( NULL != pSeq )
				pSeq->SetShowObject( bShow_ );
		}

		if( m_hSeqDestEffect[i] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = pGameMinorParticle->GetInstanceSequence( m_hSeqDestEffect[i] );
			if( NULL != pSeq )
				pSeq->SetShowObject( bShow_ );
		}
	}
#ifdef BUFF_ICON_UI
	if( NULL != m_pDlgBuffIcon )
	{
		m_pDlgBuffIcon->SetShow(bShow_);
	}
#endif //BUFF_ICON_UI
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2ElesisMyGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2ElesisMyGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    CX2MyGageUI::OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	CX2MyGageUI::OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	const bool bDialogHide = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog();

	// 다이얼로그를 숨겨야 하는데 이펙트가 보이고 있다면
	if ( bDialogHide == GetShowWspParticle() )
	{
		SetShowWspParticle( !bDialogHide );
	}

	UpdateUIElswordWSP();
	UIFrameMoveElswordWSP();
}

void CX2ElesisMyGageUI::InitUIElswordWSP()
{
	m_pStaticElswordVigor = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"g_pStatic_EL_BLUE" ) );
	m_pStaticElswordDest = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"g_pStatic_EL_RED" ) );
}

void CX2ElesisMyGageUI::StopUIEffectElswordWS()
{
	if( NULL == g_pX2Game ||
		NULL == g_pX2Game->GetMinorParticle() )
		return;

	if ( m_hSeqVigorEffect[0] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqVigorEffect[0] );
	if ( m_hSeqVigorEffect[1] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqVigorEffect[1] );
	if ( m_hSeqVigorEffect[2] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqVigorEffect[2] );
	if ( m_hSeqDestEffect[0] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqDestEffect[0] );
	if ( m_hSeqDestEffect[1] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqDestEffect[1] );
	if ( m_hSeqDestEffect[2] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqDestEffect[2] );

	return;
}

void CX2ElesisMyGageUI::PlayUIEffectDestruction()
{	
	if( NULL == g_pX2Game ||
		NULL == g_pX2Game->GetMinorParticle() )
		return;

	StopUIEffectElswordWS();

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		return;
#endif

	D3DXVECTOR3 vecEffectPos( 0.f, 0.f, 0.f );

	vecEffectPos.x = m_pStaticElswordDest->GetPicture(WOSDP_FULL_BEAD)->GetPos().x +  m_pStaticElswordDest->GetPicture(WOSDP_FULL_BEAD)->GetSize().x / 2.0f;
	vecEffectPos.y = m_pStaticElswordDest->GetPicture(WOSDP_FULL_BEAD)->GetPos().y +  m_pStaticElswordDest->GetPicture(WOSDP_FULL_BEAD)->GetSize().y / 2.0f;

	m_hSeqDestEffect[0] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_demolition_UI_01", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle1 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqDestEffect[0] );
	if( pParticle1 != NULL )
	{
		pParticle1->SetOverUI( true );
		pParticle1->SetResolutionConvert( true );
	}

	m_hSeqDestEffect[1] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_demolition_UI_02", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle2 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqDestEffect[1] );
	if( pParticle2 != NULL )
	{
		pParticle2->SetOverUI( true );
		pParticle2->SetResolutionConvert( true );
	}

	m_hSeqDestEffect[2] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_demolition_UI_03", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle3 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqDestEffect[2] );
	if( pParticle3 != NULL )
	{
		pParticle3->SetOverUI( true );
		pParticle3->SetResolutionConvert( true );
	}

	return;
}

void CX2ElesisMyGageUI::PlayUIEffectVigor()
{
	if( NULL == g_pX2Game ||
		NULL == g_pX2Game->GetMinorParticle() )
		return;

	StopUIEffectElswordWS();

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		return;
#endif

	D3DXVECTOR3 vecEffectPos( 0.f, 0.f, 0.f );

	vecEffectPos.x = m_pStaticElswordVigor->GetPicture(4)->GetPos().x +  m_pStaticElswordVigor->GetPicture(4)->GetSize().x / 2.0f;
	vecEffectPos.y = m_pStaticElswordVigor->GetPicture(4)->GetPos().y +  m_pStaticElswordVigor->GetPicture(4)->GetSize().y / 2.0f;

	m_hSeqVigorEffect[0] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_vitality_UI_01", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle1 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqVigorEffect[0] );
	if( pParticle1 != NULL )
	{
		pParticle1->SetOverUI( true );
		pParticle1->SetResolutionConvert( true );
	}

	m_hSeqVigorEffect[1] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_vitality_UI_02", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle2 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqVigorEffect[1] );
	if( pParticle2 != NULL )
	{
		pParticle2->SetOverUI( true );
		pParticle2->SetResolutionConvert( true );
	}

	m_hSeqVigorEffect[2] = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( NULL, L"elswrod_vitality_UI_03", vecEffectPos.x, vecEffectPos.y, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pParticle3 = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqVigorEffect[2] );
	if( pParticle3 != NULL )
	{
		pParticle3->SetOverUI( true );
		pParticle3->SetResolutionConvert( true );
	}

	return;
}

void CX2ElesisMyGageUI::UIFrameMoveElswordWSP()
{
	//CX2GUUser *pMyUser = static_cast< CX2GUUser* >( m_pGameUnit );
	CX2ElswordGageData* pElswordGageData = static_cast<CX2ElswordGageData*>( m_pGageData );
	CKTDGUIControl::CPictureData* pGagePicture = NULL;
	CKTDGUIControl::CPictureData* pGageEffectPicture = NULL;
	float fGageMax = 1.f;
	wstring wstrTexPieceName = L"";
	wstring wstrTexEffectPieceName = L"";
	switch( pElswordGageData->GetWayOfSwordState() )
	{
	case -2: // CX2GUElsword_SwordMan::WSS_DESTRUCTION
		{
			pGagePicture			= m_pStaticElswordDest->GetPicture( WOSDP_POINT_BAR );
			pGageEffectPicture		= m_pStaticElswordDest->GetPicture( WOSDP_GAGE_EFFECT );
			fGageMax				= g_pData->GetDamageManager()->GetDestStateThreshold();
			wstrTexPieceName		= L"EL_RED";
			wstrTexEffectPieceName	= L"EL_EFFECT";
		}
		break;
	case -1: // CX2GUElsword_SwordMan::WSS_TOWARD_DESTRUCTION
		{
			pGagePicture			= m_pStaticElswordDest->GetPicture( WOSDP_POINT_BAR );
			pGageEffectPicture		= NULL;
			fGageMax				= g_pData->GetDamageManager()->GetDestStateThreshold();
			wstrTexPieceName		= L"EL_RED";
			wstrTexEffectPieceName	= L"";
		}
		break;
	case 0: // CX2GUElsword_SwordMan::WSS_CENTER
		{
			pGagePicture			= NULL;
			pGageEffectPicture		= NULL;
			fGageMax				= 1.f;
			wstrTexPieceName		= L"";
			wstrTexEffectPieceName	= L"";
		}
		break;
	case 1: // CX2GUElsword_SwordMan::WSS_TOWARD_VIGOR
		{
			pGagePicture			= m_pStaticElswordVigor->GetPicture( 1 );
			pGageEffectPicture		= NULL;
			fGageMax				= g_pData->GetDamageManager()->GetVigorStateThreshold();
			wstrTexPieceName		= L"EL_BLUE";
			wstrTexEffectPieceName	= L"";

		}
		break;
	case 2: // CX2GUElsword_SwordMan::WSS_VIGOR
		{
			pGagePicture			= m_pStaticElswordVigor->GetPicture( 1 );
			pGageEffectPicture		= m_pStaticElswordVigor->GetPicture( 2 );
			fGageMax				= g_pData->GetDamageManager()->GetVigorStateThreshold();
			wstrTexPieceName		= L"EL_BLUE";
			wstrTexEffectPieceName	= L"EL_EFFECT";
		}
		break;
	}

	if( pGagePicture != NULL )
	{
		const CKTDXDeviceTexture::TEXTURE_UV* pTexUV			= pGagePicture->pTexture->pTexture->GetTexUV( wstrTexPieceName );

		if( pTexUV != NULL )
		{
			float fGageUVWidth = (pTexUV->rightTop.x - pTexUV->leftTop.x)  * ( pElswordGageData->GetWayOfSwordPoint() / fGageMax );


			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]			= pTexUV->leftTop;
			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]			= pTexUV->rightTop;
			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]		= pTexUV->leftBottom;
			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]		= pTexUV->rightBottom;

			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP].x		= pTexUV->leftTop.x + fGageUVWidth;
			pGagePicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM].x		= pTexUV->leftBottom.x + fGageUVWidth;
			pGagePicture->pTexture->SetTextureUV();
		}

		float fGageWidth = pGagePicture->GetOriginalSize().x * ( pElswordGageData->GetWayOfSwordPoint() / fGageMax );
		pGagePicture->SetSizeX(fGageWidth);
	}

	if( pGageEffectPicture != NULL )
	{
		const CKTDXDeviceTexture::TEXTURE_UV* pTexUV			= pGageEffectPicture->pTexture->pTexture->GetTexUV( wstrTexEffectPieceName );

		if( pTexUV != NULL )
		{
			float fGageUVWidth = (pTexUV->rightTop.x - pTexUV->leftTop.x)  * ( pElswordGageData->GetWayOfSwordPoint() / fGageMax );


			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]			= pTexUV->leftTop;
			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]		= pTexUV->rightTop;
			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]		= pTexUV->leftBottom;
			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]		= pTexUV->rightBottom;

			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP].x		= pTexUV->leftTop.x + fGageUVWidth;
			pGageEffectPicture->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM].x	= pTexUV->leftBottom.x + fGageUVWidth;
			pGageEffectPicture->pTexture->SetTextureUV();
		}

		float fGageWidth = pGageEffectPicture->GetOriginalSize().x * ( pElswordGageData->GetWayOfSwordPoint() / fGageMax );
		pGageEffectPicture->SetSizeX(fGageWidth);
	}


	return;
}

void CX2ElesisMyGageUI::UpdateUIElswordWSP()
{
	CX2ElswordGageData* pElswordGageData = static_cast<CX2ElswordGageData*>( m_pGageData );

	// 변경 된 것이 있는 경우에
	if ( pElswordGageData->GetChangedWayOfSwordState() )
	{
		StopUIEffectElswordWS();

		switch( pElswordGageData->GetWayOfSwordState() )
		{
		case -2: // CX2GUUser::WSS_DESTRUCTION
			{
				m_pStaticElswordDest->SetShow( true );
				m_pStaticElswordDest->GetPicture( WOSDP_BG )->SetShow( true );
				m_pStaticElswordDest->GetPicture( WOSDP_POINT_BAR )->SetShow( true );
				m_pStaticElswordDest->GetPicture( WOSDP_DETONATION_CHARGE)->SetShow( true );
				m_pStaticElswordDest->GetPicture( WOSDP_GAGE_EFFECT )->SetShow( true );
				m_pStaticElswordDest->GetPicture( WOSDP_GAGE_EFFECT )->SetFlicker( 0.4f, 1.0f, 0.1f );
				m_pStaticElswordDest->GetPicture( WOSDP_EMPTY_BEAD )->SetShow( false );
				m_pStaticElswordDest->GetPicture( WOSDP_FULL_BEAD )->SetShow( true );

				m_pStaticElswordVigor->SetShow( false );
				pElswordGageData->SetChangedWayOfSwordState( false );
				PlayUIEffectDestruction();
			}
			break;
		case -1: // CX2GUUser::WSS_TOWARD_DESTRUCTION
			{
				m_pStaticElswordDest->SetShow( true );
				m_pStaticElswordDest->GetPicture( WOSDP_BG )->SetShow( true );
				m_pStaticElswordDest->GetPicture( WOSDP_POINT_BAR )->SetShow( true );
				m_pStaticElswordDest->GetPicture( WOSDP_DETONATION_CHARGE)->SetShow( false );
				m_pStaticElswordDest->GetPicture( WOSDP_GAGE_EFFECT )->SetShow( false );
				m_pStaticElswordDest->GetPicture( WOSDP_EMPTY_BEAD )->SetShow( true );
				m_pStaticElswordDest->GetPicture( WOSDP_FULL_BEAD )->SetShow( false );

				m_pStaticElswordVigor->SetShow( false );
				pElswordGageData->SetChangedWayOfSwordState( false );
			}
			break;
		case 0: // CX2GUUser::WSS_CENTER
			{

				const float fWayOfSwordPoint = pElswordGageData->GetWayOfSwordPoint();

				if ( fWayOfSwordPoint >= g_pData->GetDamageManager()->GetVigorStateThreshold() )
					pElswordGageData->SetWayOfSwordState( 2 ); // WSS_VIGOR
				else if( fWayOfSwordPoint <= g_pData->GetDamageManager()->GetDestStateThreshold() )
					pElswordGageData->SetWayOfSwordState( -2 ); // WSS_DESTRUCTION;
				else if( fWayOfSwordPoint > 0 )
					pElswordGageData->SetWayOfSwordState( 1 ); // WSS_TOWARD_VIGOR;
				else if( fWayOfSwordPoint < 0 )
					pElswordGageData->SetWayOfSwordState( -1 ); // WSS_TOWARD_DESTRUCTION;
				else
				{
					m_pStaticElswordVigor->SetShow( true );
					m_pStaticElswordVigor->GetPicture( 0 )->SetShow( true );
					m_pStaticElswordVigor->GetPicture( 1 )->SetShow( false );
					m_pStaticElswordVigor->GetPicture( 2 )->SetShow( false );
					m_pStaticElswordVigor->GetPicture( 3 )->SetShow( false );
					m_pStaticElswordVigor->GetPicture( 4 )->SetShow( false );
					m_pStaticElswordVigor->GetPicture( 5 )->SetShow( true );

					m_pStaticElswordDest->SetShow( false );
					pElswordGageData->SetChangedWayOfSwordState( false );
				}
			}
			break;
		case 1: // CX2GUUser::WSS_TOWARD_VIGOR
			{
				m_pStaticElswordVigor->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 0 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 1 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 2 )->SetShow( false );
				m_pStaticElswordVigor->GetPicture( 3 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 4 )->SetShow( false );
				m_pStaticElswordVigor->GetPicture( 5 )->SetShow( false );

				m_pStaticElswordDest->SetShow( false );
				pElswordGageData->SetChangedWayOfSwordState( false );
			}
			break;
		case 2: // CX2GUUser::WSS_VIGOR
			{
				m_pStaticElswordVigor->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 0 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 1 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 2 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 2 )->SetFlicker( 0.4f, 1.0f, 0.1f );
				m_pStaticElswordVigor->GetPicture( 3 )->SetShow( false );
				m_pStaticElswordVigor->GetPicture( 4 )->SetShow( true );
				m_pStaticElswordVigor->GetPicture( 5 )->SetShow( false );

				m_pStaticElswordDest->SetShow( false );
				pElswordGageData->SetChangedWayOfSwordState( false );
				PlayUIEffectVigor();
			}
			break;
		}

		if (g_pX2Game != NULL && CX2Main::XS_VILLAGE_MAP != g_pMain->GetNowStateID() )
			g_pX2Game->UpdateElswordSkillSlotEffect();
	}
}

void CX2ElesisMyGageUI::SetShowWspParticle( const bool bShowWspParticle_ )
{
	m_bShowWspParticle = bShowWspParticle_;

	CKTDGParticleSystem* pGameMinorParticle = g_pData->GetGameMinorParticle();

	for ( int i = 0; i < ARRAY_SIZE(m_hSeqVigorEffect); ++i)
	{
		if( m_hSeqVigorEffect[i] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = pGameMinorParticle->GetInstanceSequence( m_hSeqVigorEffect[i] );
			if( NULL != pSeq )
				pSeq->SetShowObject( bShowWspParticle_ );
		}

		if( m_hSeqDestEffect[i] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = pGameMinorParticle->GetInstanceSequence( m_hSeqDestEffect[i] );
			if( NULL != pSeq )
				pSeq->SetShowObject( bShowWspParticle_ );
		}
	}
}
/** @function : UpdateMpAndHyperAndDetonationGage()
	@brief : 엘리시스 섬멸 상태의 기폭 처리를 위해 재정의
*/
/*virtual*/ void CX2ElesisMyGageUI::UpdateMpAndHyperAndDetonationGage()
{
	CKTDGUIStatic* pStaticStateBar = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateBar") );
	if ( NULL != pStaticStateBar )
	{
		UpdateMpGageAndString( pStaticStateBar );
		UpdateHyperGage( pStaticStateBar );

#ifdef ELSWORD_WAY_OF_SWORD
		CX2ElesisGageData* pElesisGageData = static_cast<CX2ElesisGageData*>(m_pGageData);
		if( NULL != pElesisGageData )
		{
			// 섬멸 상태일 경우 기폭 UI를 섬멸 게이지에 보여주기
			if( CX2GUUser::WSS_DESTRUCTION == pElesisGageData->GetWayOfSwordState() )
			{
				if( NULL != m_pStaticElswordDest )
				{
					UpdateDetonationGage( m_pStaticElswordDest );
					UpdateGageForUV( pStaticStateBar, PGUB_MY_ACTIVE_MP, L"MP_ACTIVE", 0.f, true );
				}
			}
			// 섬멸 상태가 아니라면 기폭 게이지 MP게이지에 보여주기
			else
			{
				UpdateDetonationGage( pStaticStateBar );
			}
		}
#else
		UpdateDetonationGage( pStaticStateBar );
#endif // ELSWORD_WAY_OF_SWORD
	}
}
/** @function : UpdateDetonationGage()
	@brief : 엘리시스 섬멸 상태의 기폭 처리를 위해 재정의
*/
/*virtual*/ void CX2ElesisMyGageUI::UpdateDetonationGage( CKTDGUIStatic* pStaticStateBar_ )	/// 기폭 게이지
{
	CX2ElesisGageData* pElesisGageData = static_cast<CX2ElesisGageData*>(m_pGageData);
	if( NULL != pElesisGageData )
	{
#ifdef ELSWORD_WAY_OF_SWORD
		if( CX2GUUser::WSS_DESTRUCTION == pElesisGageData->GetWayOfSwordState() )
		{
			const float fNowDetonation = pElesisGageData->GetNowChargeMpForDetonation();
			const float fDetonationPercent = fNowDetonation / _CONST_ELESIS_DETONATION_::MAX_CHARGE_DETONATION_VALUE;
			UpdateGageForUV( pStaticStateBar_, WOSDP_DETONATION_CHARGE, L"EL_RED_2", fDetonationPercent, true );
		}
		else
#endif // ELSWORD_WAY_OF_SWORD
		{
			const float fNowDetonation = pElesisGageData->GetNowChargeMpForDetonation();
			const float fDetonationPercent = fNowDetonation / THREE_CHARGE;
			UpdateGageForUV( pStaticStateBar_, PGUB_MY_ACTIVE_MP, L"MP_ACTIVE", fDetonationPercent, true );
		}
	}
}
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
CX2AddMyGageUI::CX2AddMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ )
	: CX2MyGageUI( pGageData_, eOwnerUnitClass_ )
{

}

void CX2AddMyGageUI::InitUI()
{
	InitStatusUI();

	m_pDLGMyUnit->OpenScriptFile( L"DLG_Add_DP_And_Mutation.lua" );

	InitWakeOrb();
	InitDPGageUI();
	InitMutationCountUI();
	InitFormationModeConsumeDPGuide();
}

void CX2AddMyGageUI::InitWakeOrb()
{
	CKTDGUIStatic* pStaticWakeOrb = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeOrb") );
	if ( NULL != pStaticWakeOrb )
	{
#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_EVE ) );			// 이브 구슬 삭제
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_ARA ) );			// 아라 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_RAVEN ) );		// 레이븐 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_CHUNG ) );		// 청 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB3 ) );				// 세번째 일반 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB2 ) );				// 두번째 일반 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB1 ) );				// 첫번째 일반 구슬 삭제
	}

	// 각성 구슬 슬롯
	CKTDGUIStatic* pStaticWakeSlot = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeSlot") );
	if ( NULL != pStaticWakeSlot )
	{
#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_EVE ) );			// 이브 구슬 삭제
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_ARA ) );			// 아라 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_RAVEN ) );		// 레이븐 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_CHUNG ) );		// 청 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB3 ) );			// 세번째 일반 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB2 ) );			// 두번째 일반 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB1 ) );			// 첫번째 일반 구슬 삭제
	}
}

void CX2AddMyGageUI::InitDPGageUI()
{
	CKTDGUIStatic* pStaticDPGage = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"g_pStatic_DP_Gage" ) );

	if ( NULL != pStaticDPGage )
		pStaticDPGage->SetShow( true );
}

void CX2AddMyGageUI::InitMutationCountUI()
{	
	CKTDGUIStatic* pStaticVerticalMutationCount = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"g_pStatic_Mutation_Gage" ) );

	ASSERT( NULL != pStaticVerticalMutationCount );
	if ( NULL != pStaticVerticalMutationCount )
		pStaticVerticalMutationCount->SetShow( true );
}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2AddMyGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2AddMyGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		CX2MyGageUI::OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		CX2MyGageUI::OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	UpdateDPGage();
	//UpdateCharacterImage();
	UpdateMutationCountUI();
}

void CX2AddMyGageUI::InitFormationModeConsumeDPGuide()
{
	CX2AddGageData* pAddrGageData	= static_cast<CX2AddGageData*>( m_pGageData );
	CKTDGUIStatic*	pStaticGuide	= static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"FormationConsumeDPGuide") );

	if ( NULL != pAddrGageData && NULL != pStaticGuide )
	{
		const float fMaxGageWidth	= 193;
		const float fUseDP			= 2000.f;		/// 구성 모드 전환에 필요한 DP 량 ( 연동이 힘들다... 강제로 넣어놓자... )
		const float fMaxDP			= pAddrGageData->GetMaxDPValue();

		D3DXVECTOR2 vec100MpPointPos( fMaxGageWidth * ( fUseDP / fMaxDP ),0.f);

		pStaticGuide->SetOffsetPos(vec100MpPointPos);
	}
}

void CX2AddMyGageUI::UpdateMutationCountUI()
{
	CX2AddGageData* pAddrGageData = static_cast<CX2AddGageData*>( m_pGageData );

	if ( NULL != pAddrGageData )
	{
		/// DP 게이지
		if ( true == pAddrGageData->GetChangedMutationCount() )
		{
			pAddrGageData->SetChangedMutationCount( false );

			CKTDGUIStatic* pStaticVerticalMutationCount = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"g_pStatic_Mutation_Gage" ) );

			ASSERT( NULL != pStaticVerticalMutationCount );
			if ( NULL != pStaticVerticalMutationCount )
			{
				int iNumOfPicture = pStaticVerticalMutationCount->GetPictureNum();

				// 라인당 최소 변이 수치의 갯수 (0번째 라인은 0개, 1번째 라인은 6개 등)
				// Picture 수는 BG Picture 포함해서 7개 이기 때문 -1 해줌
				for ( int iPictureIndex = 0; iPictureIndex < iNumOfPicture ; ++iPictureIndex )
				{
					bool bShowPictureDataMutationCount = false;

					// 변이 수치의 갯수가 라인당 최소 변이 수치 갯수 + 그 라인의 변이 수치 갯수(픽쳐의 인덱스) 보다 많거나 같으면
					if ( pAddrGageData->GetMutationCount() >= iPictureIndex  )
						bShowPictureDataMutationCount = true;

					if( 0 == iPictureIndex )
						bShowPictureDataMutationCount = true;

					CKTDGUIControl::CPictureData* pPictureDataCannonBall = pStaticVerticalMutationCount->GetPicture( iPictureIndex );
					ASSERT( NULL != pPictureDataCannonBall );
					pPictureDataCannonBall->SetShow( bShowPictureDataMutationCount );
				}
			}
		}

		/// 구성 모드 전환 여부 쿨타임
		CKTDGUIStatic*	pStaticCoolTime	= static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"FormationModeCoolTime") );

		ASSERT( NULL != pStaticCoolTime );
		if ( NULL != pStaticCoolTime )
		{
			int iShowPictureIndex = 5;	/// 보여질 픽처 인덱스

			/// 쿨타임이 돌고 있다면
			if ( 0.f < pAddrGageData->GetChangeFormationCoolTime() )
			{
				/// 남은 쿨타임을 픽처 인덱스로 변환
				iShowPictureIndex = static_cast<int>( pAddrGageData->GetChangeFormationCoolTime() );
			}

			/// 텍스처 활성 여부 설정
			for( int i = 0; i < 6; ++i )
			{
				CKTDGUIControl::CPictureData* pPictureCoolTime = pStaticCoolTime->GetPicture( i );

				if ( NULL != pPictureCoolTime )
				{
					bool bShow = false;

					/// 보여줄 인덱스만 표시
					if ( i == iShowPictureIndex )
						bShow = true;

					pPictureCoolTime->SetShow( bShow );
				}
			}
		}
	}	
}

/*virtual*/ void CX2AddMyGageUI::UpdateHyperGage( CKTDGUIStatic* pStaticStateBar_ )
{
	if ( NULL != m_pGageData )
		UpdateGageForUV( pStaticStateBar_, PGUB_MY_SOUL, L"RAGE", 0.f, false );
}

void CX2AddMyGageUI::UpdateMpAndHyperAndDetonationGage()
{
	CKTDGUIStatic* pStaticStateBar = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateBar") );
	if ( NULL != pStaticStateBar )
	{
		UpdateHyperGage( pStaticStateBar );
		UpdateMpGageAndString( pStaticStateBar );
		UpdateDetonationGage( pStaticStateBar );
	}
}

void CX2AddMyGageUI::UpdateDPGage()
{
	if ( NULL != m_pDLGMyUnit && NULL != m_pGageData )
	{
		CKTDGUIStatic*	pStaticStateBar	= static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"g_pStatic_DP_Gage") );
		CX2AddGageData* pAddrGageData	= static_cast<CX2AddGageData*>( m_pGageData );

		if ( NULL != pStaticStateBar && NULL != pAddrGageData )
		{
			/// 게이지 설정
			const float fNowDP		= pAddrGageData->GetDPValue();
			const float fMaxDP		= pAddrGageData->GetMaxDPValue();
			const float fNPPercent	= fNowDP / fMaxDP;

			UpdateGageForUV( pStaticStateBar, 1, L"BG_DPSYSTEM_GAUGE", fNPPercent, true );

			/// 구성 모드 활성을 표시하는 DP 게이지 반짝임
			CKTDGUIControl::CPictureData* pFlickerBar = pStaticStateBar->GetPicture( 2 );

			if ( NULL != pFlickerBar )
			{
				/// 구성 모드일 때만 표시
				if ( NULL != g_pX2Game && true == pAddrGageData->GetIsFormationMode() )
				{
					/// 비활성 중이면, 활성 시키면서 반짝임 설정
					if ( false == pFlickerBar->GetShow() )
					{
						pStaticStateBar->GetPicture( 2 )->SetFlicker( 0.4f, 1.f, 0.1f );
						pFlickerBar->SetShow( true );
					}

					UpdateGageForUV( pStaticStateBar, 2, L"BG_ADDGAUGE_GLOW", fNPPercent, true );
				}
				else
				{
					pStaticStateBar->GetPicture( 2 )->SetShow( false );
				}
			}


			/// 스트링 설정
			CKTDGUIStatic* pStaticString = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"g_pStatic_DP") );

			if ( NULL != pStaticString )
			{
				const float fNowDp			= pAddrGageData->GetDPValue();
				const float fMaxDp			= pAddrGageData->GetMaxDPValue();

				const UINT uiStrSize = 32;
				WCHAR buf[uiStrSize];
				ZeroMemory( buf, sizeof(WCHAR) * uiStrSize );

				StringCchPrintf( buf, uiStrSize, L"%ld/%ld", 
					static_cast<UINT>( floor( fNowDp + 0.5f ) / DISPLAY_DP_RATE ), static_cast<UINT>( floor( fMaxDp + 0.5f ) / DISPLAY_DP_RATE ) );
					pStaticString->GetString(0)->msg = buf;
			}
		}
	}
}

void CX2AddMyGageUI::UpdateHyperModeRemainTime()
{
	return;
}
#endif //SERV_9TH_NEW_CHARACTER


#ifdef ADD_RENA_SYSTEM //김창한
/** @function 	: CX2RenaMyGageUI
	@brief 		: 생성자
*/
CX2RenaMyGageUI::CX2RenaMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ )
	: CX2MyGageUI( pGageData_, eOwnerUnitClass_ )
{
}

/** @function 	: InitUI
	@brief 		: UI 초기화
*/
/*virtual*/ void CX2RenaMyGageUI::InitUI()
{
	InitStatusUI();

	m_pDLGMyUnit->OpenScriptFile( L"DLG_Rena_NaturalForce.lua" );

	InitWakeOrb();
	InitNaturalForceUI();
}

/** @function 	: InitForcePowerUI
	@brief 		: 기력 UI 초기화
*/
void CX2RenaMyGageUI::InitNaturalForceUI()
{
	CKTDGUIStatic* pStaticNaturalForce = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"g_pStatic_NF_Gage_Back" ) );

	ASSERT( NULL != pStaticNaturalForce );
	if ( NULL != pStaticNaturalForce )
		pStaticNaturalForce->SetShow( true );
}

/** @function 	: OnFrameMove
	@brief 		: 온프레임무브
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2RenaMyGageUI::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2RenaMyGageUI::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    CX2MyGageUI::OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    CX2MyGageUI::OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	UpdateNaturalForceUI();
}

/** @function 	: UpdateNaturalForceUI
	@brief 		: 자연의 기운 UI 갱신
*/
void CX2RenaMyGageUI::UpdateNaturalForceUI()
{
	CX2RenaGageData* pRenaGageData = static_cast<CX2RenaGageData*>( m_pGageData );

	if ( pRenaGageData->GetNaturalForceChanged() )
	{
		pRenaGageData->SetNaturalForceChanged( false );

		CKTDGUIStatic* pStaticNaturalForce = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( L"g_pStatic_NF_Gage" ) );

		ASSERT( NULL != pStaticNaturalForce );
		if ( NULL != pStaticNaturalForce )
		{
			int iNumOfPicture = pStaticNaturalForce->GetPictureNum();
			D3DXVECTOR2 vPos( 0.f, 0.f );

			for ( int iPictureIndex = 0; iPictureIndex < iNumOfPicture ; ++iPictureIndex )
			{
				bool bShowPictureDataForcePower = false;

				if ( pRenaGageData->GetNowNaturalForce() > iPictureIndex  )
					bShowPictureDataForcePower = true;

				CKTDGUIControl::CPictureData* pPictureDataForcePower = pStaticNaturalForce->GetPicture( iPictureIndex );
				ASSERT( NULL != pPictureDataForcePower );
				pPictureDataForcePower->SetShow( bShowPictureDataForcePower );
			}

			pStaticNaturalForce->SetShow( true );
		}
	}
}
#endif //ADD_RENA_SYSTEM


#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어

CX2EveMyGageUI::CX2EveMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ )
	: CX2MyGageUI( pGageData_, eOwnerUnitClass_ )
{}


/*virtual*/ void CX2EveMyGageUI::InitWakeOrb()
{
	CKTDGUIStatic* pStaticWakeOrb = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeOrb") );
	if ( NULL != pStaticWakeOrb )
	{
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_ARA ) );			// 아라 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_RAVEN ) );		// 레이븐 구슬 삭제		
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_CHUNG ) );		// 청 구슬 삭제
		pStaticWakeOrb->RemovePictureByIndex( static_cast<int>( PGUWO_ORB3 ) );				// 일반 구슬 삭제
	}

	// 각성 구슬 슬롯
	CKTDGUIStatic* pStaticWakeSlot = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl(L"StaticPVPGameStateWakeSlot") );
	if ( NULL != pStaticWakeSlot )
	{
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_ARA ) );			// 아라 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_RAVEN ) );		// 레이븐 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB_CHUNG ) );		// 청 구슬 삭제
		pStaticWakeSlot->RemovePictureByIndex( static_cast<int>( PGUWO_ORB3 ) );			// 일반 구슬 삭제
	}
}

#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어

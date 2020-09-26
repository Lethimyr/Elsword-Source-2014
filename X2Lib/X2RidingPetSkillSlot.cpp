#include "StdAfx.h"

#ifdef RIDING_SYSTEM

#include "X2RidingPetSkillSlot.h"

/*
	@date	JHKang on 2013-05-01
			- 마나에 따른 스킬 슬롯 상태 갱신
*/

CX2RidingPetSkillSlot::CX2RidingPetSkillSlot() : m_pDlgSkillSlot( NULL )
{
	m_fCoolTime[0] = m_fCoolTime[1] = m_fCoolTime[2] = 0.f;

	m_pDlgSkillSlot = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_RIDING_SKILL_SLOT.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgSkillSlot );

	m_pDlgSkillSlot->SetShowEnable( false, false );

}

CX2RidingPetSkillSlot::~CX2RidingPetSkillSlot()
{
	SAFE_DELETE_DIALOG( m_pDlgSkillSlot );
}

void CX2RidingPetSkillSlot::SetSkillUIType( bool bIsTypeA )
{
	if( NULL == m_pDlgSkillSlot )
		return;

	CKTDGUIStatic* pStatic_Stamina = static_cast<CKTDGUIStatic*>( m_pDlgSkillSlot->GetControl( L"STAMINA" ) );
	if( NULL == pStatic_Stamina )
		return;

	D3DXVECTOR2 vSlotPos(0,0);
	D3DXVECTOR2 vOffsetPos(0,0);
	
	
	if( false == bIsTypeA )
	{
		vSlotPos.x = -297.f;
#ifdef INT_WIDE_BAR
		vSlotPos.y = -595.f;
#else //INT_WIDE_BAR
		vSlotPos.y = -625.f;
#endif // INT_WIDE_BAR
		vOffsetPos.y = 66.f;

		pStatic_Stamina->SetOffsetPos( vOffsetPos );
	}

	pStatic_Stamina->SetOffsetPos( vOffsetPos );
	m_pDlgSkillSlot->SetPos( vSlotPos );
}

void CX2RidingPetSkillSlot::UpdateSkillSlotString()
{
	if( NULL == m_pDlgSkillSlot )
		return;

	CX2KeyPad* pKeyPad = g_pMain->GetKeyPad();
	if( NULL == pKeyPad )
		return;

	CKTDGUIStatic* pStaticHotKey = static_cast<CKTDGUIStatic*>( m_pDlgSkillSlot->GetControl( L"SkillHotKey" ) );
	if( NULL == pStaticHotKey )
		return;

	if( NULL != pStaticHotKey->GetString(0) )
		pStaticHotKey->GetString(0)->msg = pKeyPad->GetKeyString( GAMEACTION_ATTACK_FAST );
	if( NULL != pStaticHotKey->GetString(1) )
		pStaticHotKey->GetString(1)->msg = pKeyPad->GetKeyString( GAMEACTION_ATTACK_STRONG );
	if( NULL != pStaticHotKey->GetString(2) )
		pStaticHotKey->GetString(2)->msg = pKeyPad->GetKeyString( GAMEACTION_SKILL4 );
	if( NULL != pStaticHotKey->GetString(3) )
		pStaticHotKey->GetString(3)->msg = pKeyPad->GetKeyString( GAMEACTION_RIDING );
}

void CX2RidingPetSkillSlot::UpdateStamina( IN const float fStamina_, IN const float fMaxStamina_ )
{
	if( NULL != m_pDlgSkillSlot )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDlgSkillSlot->GetControl( L"STAMINA" ) );
		if( NULL == pStatic )
			return;

		CKTDGUIControl::CPictureData* pStamina = pStatic->GetPicture(1);
		if( NULL == pStamina )
			return;

#ifdef RIDINGPET_SKILLUI_NULLCHECK
		if( NULL != pStamina->pTexture && NULL != pStamina->pTexture->pTexture )
#endif //RIDINGPET_SKILLUI_NULLCHECK
		{	
			float fStamina = fStamina_;
			float fMaxStamina = fMaxStamina_;
			float fStaminaPercent = fStamina / fMaxStamina;
		
			const CKTDXDeviceTexture::TEXTURE_UV* pTexUV = pStamina->pTexture->pTexture->GetTexUV( L"RIDING_STAMINA_BAR" );
			float fRate = 1.0f;

			if ( NULL != pTexUV )
			{
				const float fGageUVWidth = ( pTexUV->rightTop.x - pTexUV->leftTop.x ) * fStaminaPercent;

				pStamina->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]		= pTexUV->leftTop;
				pStamina->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]		= pTexUV->rightTop;
				pStamina->pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]	= pTexUV->leftBottom;
				pStamina->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]	= pTexUV->rightBottom;

				pStamina->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP].x	= pTexUV->leftTop.x + fGageUVWidth;
				pStamina->pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM].x	= pTexUV->leftBottom.x + fGageUVWidth;
				pStamina->pTexture->SetTextureUV();

				fRate = pStamina->GetOriginalSize().x * fStaminaPercent;
				pStamina->SetSizeX( fRate );
			}		

			WCHAR Stamina[30] = {0,};
			if( NULL != pStatic->GetString(0) )
			{
				StringCchPrintf( Stamina, 30, L"%d/%d", static_cast<int>(fStamina), static_cast<int>(fMaxStamina) );
				pStatic->SetString( 0, Stamina);
			}

			if( fStamina_ <= 10.f && false == pStamina->GetIsNowFlicker() ) //스테미나 10이하일 때 깜빡이는 기능
				pStamina->SetFlicker( 0.4f, 1.0f, 0.f );
			else if( fStamina_ > 10.f && true == pStamina->GetIsNowFlicker() )
				pStamina->SetFlicker( 0.4f, 1.0f, 1.f, false );
		}
	}
}

#ifdef ADJUST_RIDINGPET_SKILLSLOT
void CX2RidingPetSkillSlot::SetSkillSlotTexture()
{
	if( NULL != m_pDlgSkillSlot )
	{
		UidType RidingPetID = -1;
		if( NULL != CX2RidingPetManager::GetInstance() && NULL != CX2RidingPetManager::GetInstance()->GetMyRidingPet() )
			RidingPetID = CX2RidingPetManager::GetInstance()->GetMyRidingPet()->GetInfo().m_Id;
		if( -1 == RidingPetID )
			return;

		CX2RidingPetManager::RidingPetTemplet* pTemplet = NULL;
		pTemplet = CX2RidingPetManager::GetInstance()->GetRidingPetTemplet( static_cast<CX2RidingPetManager::RIDING_PET_UNIT_ID>(RidingPetID) );
		if( NULL == pTemplet )
			return;

		CKTDGUISlot* pSlot = NULL;
		WCHAR SlotName[10] = {0,};
		int index = 0;

		std::vector<CX2RidingPetManager::RidingPetSkillInfo>::iterator itr;
		for( itr = pTemplet->m_vecAttackSkill.begin(); itr != pTemplet->m_vecAttackSkill.end(); ++itr )
		{
			StringCchPrintf( SlotName, 10, L"SlotB%d", index );
			pSlot = static_cast<CKTDGUISlot*>( m_pDlgSkillSlot->GetControl( SlotName ) );
			if( NULL != pSlot )
			{
				pSlot->SetItemTex( itr->m_wstrImageName.c_str(), itr->m_wstrKeyName.c_str() );
				pSlot->SetGuideDesc( itr->m_wstrSkillDesc.c_str() );
			}

			++index;
		}

		StringCchPrintf( SlotName, 10, L"SlotB%d", index );
		pSlot = static_cast<CKTDGUISlot*>( m_pDlgSkillSlot->GetControl( SlotName ) );
		if( NULL != pSlot )
		{
			pSlot->SetItemTex( L"DLG_SKILL_RIDING.tga", L"RIDING_GET_OFF" );
			UpdateRideOffSlotGuide();
		}
	}
}
void CX2RidingPetSkillSlot::UpdateSkillSlotTexture()
{
	if( NULL != m_pDlgSkillSlot )
	{
		CKTDGUISlot* pSlot = NULL;
		WCHAR SlotName[10] = {0,};
		int index = 0;

		for( index; index < 4; ++index )
		{
			StringCchPrintf( SlotName, 10, L"SlotB%d", index );
			pSlot = static_cast<CKTDGUISlot*>( m_pDlgSkillSlot->GetControl( SlotName ) );
			if( NULL != pSlot )
			{
				D3DXVECTOR2 vOffsetPos = D3DXVECTOR2 ( 0, 0 );
				if ( g_pMain->GetGameOption().GetIsSkillUITypeA() == false )
				{
					wstring guideDesc = pSlot->GetGuideDesc();
					CKTDGFontManager::CUKFont* pItemDescFont = 
						g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( 2 );

					float itemDescWidth = (float)( pItemDescFont->GetWidth( guideDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + 12;

					vOffsetPos.x = - ( itemDescWidth / 2.f ) - 297;
					vOffsetPos.y = pSlot->GetSize().y;

					pSlot->SetGuideDescLT( true );
				}
				else
				{
					pSlot->SetGuideDescLT( false );
				}
				pSlot->SetGuideDescOffsetPos ( vOffsetPos );
			}
		}
	}
}
#else //ADJUST_RIDINGPET_SKILLSLOT
void CX2RidingPetSkillSlot::UpdateSkillSlotTexture()
{
	if( NULL != m_pDlgSkillSlot )
	{
		UidType RidingPetID = -1;
		if( NULL != CX2RidingPetManager::GetInstance() && NULL != CX2RidingPetManager::GetInstance()->GetMyRidingPet() )
			RidingPetID = CX2RidingPetManager::GetInstance()->GetMyRidingPet()->GetInfo().m_Id;
		if( -1 == RidingPetID )
			return;

		CX2RidingPetManager::RidingPetTemplet* pTemplet = NULL;
		pTemplet = CX2RidingPetManager::GetInstance()->GetRidingPetTemplet( static_cast<CX2RidingPetManager::RIDING_PET_UNIT_ID>(RidingPetID) );
		if( NULL == pTemplet )
			return;

		CKTDGUISlot* pSlot = NULL;
		WCHAR SlotName[10] = {0,};
		int index = 0;

		std::vector<CX2RidingPetManager::RidingPetSkillInfo>::iterator itr;
		for( itr = pTemplet->m_vecAttackSkill.begin(); itr != pTemplet->m_vecAttackSkill.end(); ++itr )
		{
			StringCchPrintf( SlotName, 10, L"SlotB%d", index );
			pSlot = static_cast<CKTDGUISlot*>( m_pDlgSkillSlot->GetControl( SlotName ) );
			if( NULL != pSlot )
			{
				pSlot->SetItemTex( itr->m_wstrImageName.c_str(), itr->m_wstrKeyName.c_str() );
				
				{
					D3DXVECTOR2 vOffsetPos = D3DXVECTOR2 ( 0, 0 );
					if ( g_pMain->GetGameOption().GetIsSkillUITypeA() == false )
					{
						wstring guideDesc = pSlot->GetGuideDesc();
						CKTDGFontManager::CUKFont* pItemDescFont = 
							g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( 2 );
						
						float itemDescWidth = (float)( pItemDescFont->GetWidth( guideDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + 12;

						vOffsetPos.x = - ( itemDescWidth / 2.f ) - 297;
						vOffsetPos.y = pSlot->GetSize().y;

						pSlot->SetGuideDescLT( true );
					}
					else
					{
						pSlot->SetGuideDescLT( false );
					}
					pSlot->SetGuideDescOffsetPos ( vOffsetPos );
				}
				pSlot->SetGuideDesc( itr->m_wstrSkillDesc.c_str() );
			}

			++index;
		}

		StringCchPrintf( SlotName, 10, L"SlotB%d", index );
		pSlot = static_cast<CKTDGUISlot*>( m_pDlgSkillSlot->GetControl( SlotName ) );
		if( NULL != pSlot )
		{
			pSlot->SetItemTex( L"DLG_SKILL_RIDING.tga", L"RIDING_GET_OFF" );
			UpdateRideOffSlotGuide();
		}
	}
}
#endif //ADJUST_RIDINGPET_SKILLSLOT

void CX2RidingPetSkillSlot::UpdateRideOffSlotGuide()
{
	if( NULL != m_pDlgSkillSlot && NULL != g_pMain && NULL != g_pMain->GetKeyPad() )
	{
		CKTDGUISlot* pSlot = static_cast<CKTDGUISlot*>( m_pDlgSkillSlot->GetControl(L"SlotB3") );
		if( NULL != pSlot )
		{
			bool IsSetKey = (0 == StrCmp(g_pMain->GetKeyPad()->GetKeyString( GA_RIDING ).c_str(), L"" ))? false : true;

			if( true == IsSetKey )
			{
				pSlot->SetGuideDesc( GET_STRING(STR_ID_24530) );
			}
			else
			{
				pSlot->SetGuideDesc( GET_STRING(STR_ID_24531) );
			}
		}
	}
}

void CX2RidingPetSkillSlot::SetBlindSkillSlot( UINT _slotNum, bool _val )
{
	if( _slotNum > 3)
		return;

	if( NULL != m_pDlgSkillSlot )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDlgSkillSlot->GetControl( L"Skill_CoolTime" ) );
		if( NULL == pStatic )
			return;

		if( NULL != pStatic->GetPicture(_slotNum) )
			pStatic->GetPicture(_slotNum)->SetShow(_val);
	}
}

#pragma region SetSkillSlotCoolTime
void CX2RidingPetSkillSlot::SetSkillSlotCoolTime( IN UINT islotNum_, IN float fCoolTime_ )
{
	m_fCoolTime[islotNum_] = fCoolTime_;
	
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();
	if( NULL != pGageManager )
		pGageManager->SetMyRidingSkillCoolTimeList( islotNum_, fCoolTime_ );

	SetBlindSkillSlot( islotNum_, (fCoolTime_ > 0.f) );
}
#pragma endregion 

#pragma region UpdateSkillSlotCoolTime
void CX2RidingPetSkillSlot::OnFrameMove( double fTime, float fElapsedTime )
{
	for ( int i = 0; i < 3; ++i )
	{
		if ( m_fCoolTime[i] > 0.f )
		{
			m_fCoolTime[i] -= fElapsedTime;
		
			if ( m_fCoolTime[i] < 0.f )
			{
				m_fCoolTime[i] = 0.f;
				SetBlindSkillSlot( i, false );
			}

			CX2GageManager* pGageManager = CX2GageManager::GetInstance();
			
			if ( NULL != pGageManager )
				pGageManager->SetMyRidingSkillCoolTimeList( i, m_fCoolTime[i] );

			CKTDGUIStatic* pStaticCoolTime = static_cast<CKTDGUIStatic*>( m_pDlgSkillSlot->GetControl( L"CoolTime" ) );
			if( NULL == pStaticCoolTime )
				return;

			if( NULL != pStaticCoolTime->GetString(i) )
			{
				if ( m_fCoolTime[i] > 1.f )
					pStaticCoolTime->GetString(i)->msg = 
						GET_REPLACED_STRING( ( STR_ID_2632, "L", g_pMain->GetEDString( static_cast<int>( m_fCoolTime[i] ) ) ) );
				else
					pStaticCoolTime->GetString(i)->msg = L"";
			}
		}
	}

	if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() && NULL != CX2RidingPetManager::GetInstance() )
	{
		UidType UserUid = g_pX2Game->GetMyUnit()->GetUnitUID();
		CX2RidingPetManager* pRidingPetManager =  CX2RidingPetManager::GetInstance();

		CX2GUUser* pUser = static_cast<CX2GUUser*>( g_pX2Game->GetUserUnitByUID( UserUid ) );

		if ( NULL != pUser && NULL != pUser->GetRidingPet() && NULL != pRidingPetManager )
		{
			CX2RidingPetManager::RidingPetTemplet* pRidingPetTemplet = pRidingPetManager->GetRidingPetTemplet( 
				static_cast<CX2RidingPetManager::RIDING_PET_UNIT_ID>( pUser->GetRidingPet()->GetInfo().m_Id ) );

			if ( NULL != pRidingPetTemplet )
			{
				for ( int i = 0; i < 3; ++i )
				{
					if ( pUser->GetNowMp() < pRidingPetTemplet->m_vecAttackSkill.at(i).m_fConsumeMana )
					{	
						SetBlindSkillSlot( i, true );
					}
					else
					{
						if ( m_fCoolTime[i] <= 0.f )
							SetBlindSkillSlot( i, false );
					}
				}
			}
		}
	}
}
#pragma endregion 

void CX2RidingPetSkillSlot::SetShowRidingSkillSlot( bool _Val )
{
	if( NULL != m_pDlgSkillSlot )
	{
		if( true == _Val )
		{
			UpdateSkillSlotString();
			UpdateSkillSlotTexture();
		}

		m_pDlgSkillSlot->SetShowEnable( _Val, _Val );
	}

}
#endif //RIDING_SYSTEM
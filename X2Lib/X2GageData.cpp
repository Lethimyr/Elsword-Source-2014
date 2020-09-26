#include "stdafx.h"
#include ".\X2GageData.h"

void CX2GageData::Gage::Increase( const float fIncrement, const float fMinimum /*= 0.f */ )
{
	THEMIDA_ENCODE_START

		fNow += fIncrement;

	if( fNow > fMax )
		fNow = fMax;

	if( fNow < fMinimum )
		fNow = fMinimum;

	THEMIDA_ENCODE_END
}
/*virtual*/ void CX2GageData::DetonationFrameMove( double fTime, float fElapsedTime )
{
	// 기폭 관련
	if( GetCanChargeMpForDetonation() == true )
	{
		SetMaxChargeMpForDetonation( GetNowMp() );
		UpNowChargeMpForDetonation( GetChangeRateChargeMpForDetonation() * fElapsedTime );

		const float fNowChargeMp = GetNowChargeMpForDetonation();

		if( fNowChargeMp > THREE_CHARGE )
			SetNowChargeMpForDetonation( THREE_CHARGE );
		else if( fNowChargeMp > GetMaxChargeMpForDetonation() )
			SetNowChargeMpForDetonation( GetMaxChargeMpForDetonation() );
		else if( fNowChargeMp < 0.0f )
			SetNowChargeMpForDetonation( 0.0f );

		SetCanChargeMpForDetonation( false );
	}
	else
	{
		SetNowChargeMpForDetonation( 0.0f );
		SetCanChargeMpForDetonation( false );
	}
}
/*virtual*/ void CX2GageData::OnFrameMove( double fTime, float fElapsedTime )
{
	DetonationFrameMove(fTime, fElapsedTime);

	if ( GetNowHp() > 0 )
	{
		if( GetEnableHPRelativeChangeRate() )
			UpNowHp( GetChangeRateHp() * GetMaxHp() * fElapsedTime );
		else
			UpNowHp( GetChangeRateHp() * fElapsedTime );
	}

	//}} oasis : 김상윤 // 2009-10-6 //

	if( GetChangeRateTimeByItem() > 0.f )
	{
		SetChangeRateTimeByItem( GetChangeRateTimeByItem() - fElapsedTime );
		if( GetChangeRateTimeByItem() <= 0.f )
		{
			SetChangeRateTimeByItem( 0.0f );
			SetChangeRateByItem( 0.0f );
		}
	}

	const float fMpToBeUp = ( GetChangeRateMp() + GetChangeRateByItem() ) * fElapsedTime;
	UpNowMp( fMpToBeUp );

	if( GetNowHp() <= 0)
		m_pMpGage->ClearFactor();

	for ( UINT i = 0; i < m_pMpGage->vecFactor.size(); ++i )
	{
		GageFactor &gageFactor = m_pMpGage->vecFactor[i];
		if( gageFactor.fTime > 0.f )
		{
			m_pMpGage->fNow += gageFactor.fFactor * fElapsedTime;
			gageFactor.fTime -= fElapsedTime;

			if( gageFactor.fTime <= 0.f )
			{
				m_pMpGage->vecFactor.erase( m_pMpGage->vecFactor.begin() + i );
				--i;
			}
		}
	}

	if ( g_pKTDXApp->GetIsNowVeryfy() == true
		&& g_pInstanceData != NULL 
		&& g_pInstanceData->GetVerifyGageManagerTimer() <= 0.f )
		Veryfy();
}

bool CX2GageData::FlushMp( const float fFlushMp_ )
{
	if( NULL == m_pMpGage || fFlushMp_ > m_pMpGage->fNow )
		return false;
	else
	{
		UpNowMp( -fFlushMp_ );
		return true;
	}
}

/*virtual*/ bool CX2GageData::DamageFlushMp()
{
	// 기폭 : 모인 만큼 터지게 변경
	if( GetNowChargeMpForDetonation() >= ONE_CHARGE )	
	{
#ifdef FIX_FORCE_DOWN_AND_DETONATION_BUG
		SetActivateDetonation( true, GetNowChargeMpForDetonation() );
#else
		UpNowMp( GetNowChargeMpForDetonation() * -1 );
		SetNowChargeMpForDetonation( 0.0f );
#endif // FIX_FORCE_DOWN_AND_DETONATION_BUG

		return true;
	}
	else
		return false;
}

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
bool    CX2GageData::DamageFlushMp_CheckOnly()
{
	// 기폭 : 모인 만큼 터지게 변경
	if( GetNowChargeMpForDetonation() >= ONE_CHARGE )	
	{
		return true;
	}
	else
		return false;
}
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS

bool CX2GageData::FlushForceDown()
{
	int iNowForceDown = static_cast<int>( GetNowForceDown() );
	int iMaxForceDown = static_cast<int>( GetMaxForceDown() );

	WCHAR buf[100] = { 0, };

	wsprintf( buf, L"NowForceDown: %d, MaxForceDown: %d\n", iNowForceDown, iMaxForceDown );

	DXUTTRACE( buf );

	if( GetNowForceDown() < GetMaxForceDown() )
		return false;
	else
	{
#ifdef FIX_FORCE_DOWN_AND_DETONATION_BUG
		SetActivateForceDown( true );
#else
		SetNowForceDown( 0.0f );
#endif // FIX_FORCE_DOWN_AND_DETONATION_BUG

		return true;
	}
}
#ifdef FIX_FORCE_DOWN_AND_DETONATION_BUG
bool CX2GageData::UpdateDamageFlushMP()
{
	if( true == GetActivateDetonation() )
	{
		// MP 소모
		UpNowMp( m_fNowChargeMpForDetonation * -1 );
		SetNowChargeMpForDetonation( 0.f );

		// 기폭 활성화 여부 초기화
		SetActivateDetonation(false, 0.f);

		return true;
	}

	return false;
}
bool CX2GageData::UpdateFlushForceDown()
{
	// 다운수치 활성화 상태라면 다운수치 초기화
	if( true == GetActivateForceDown() )
	{
		// 다운수치 초기화
		SetNowForceDown( 0.0f );

		// 활성화 여부 초기화
		SetActivateForceDown( false );

		return true;
	}

	return false;
}
#endif // FIX_FORCE_DOWN_AND_DETONATION_BUG

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
bool CX2GageData::FlushForceDown_CheckOnly()
{
	int iNowForceDown = static_cast<int>( GetNowForceDown() );
	int iMaxForceDown = static_cast<int>( GetMaxForceDown() );

	if( GetNowForceDown() < GetMaxForceDown() )
		return false;
	else
	{
		return true;
	}
}
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS

bool CX2GageData::FlushSoul( const float fFlushSoul_ )
{
	if( GetNowSoul() >= fFlushSoul_ )
	{
		UpNowSoul( -fFlushSoul_ );
		return true;
	}
	else
		return false;
}

/*virtual*/ void CX2GageData::UpdateDataFromGameUnit( const CX2GageData* pGageData_ )
{
#ifdef GAGEDATA_NULLCHECK
	if( NULL != pGageData_ )
#endif //GAGEDATA_NULLCHECK
	{
		SetNowHp( pGageData_->GetNowHp() );
		SetMaxHp( pGageData_->GetMaxHp() );

		SetNowMp( pGageData_->GetNowMp() );
		SetMaxMp( pGageData_->GetMaxMp() );

		SetNowSoul( pGageData_->GetNowSoul() );
		SetMaxSoul( pGageData_->GetMaxSoul() );

		SetHyperModeCount( pGageData_->GetHyperModeCount() );
		SetHyperModeRemainTime( pGageData_->GetHyperModeRemainTime() );
	
		SetNowChargeMpForDetonation( pGageData_->GetNowChargeMpForDetonation() );
	}
}

/*virtual*/ void CX2GageData::InitWhenGameIsOver()
{
	SetHyperModeRemainTime( 0.0f );
}

/*virtual*/ bool CX2GageData::GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus_ ) const
{
	if ( Veryfy() )
	{
		kGamePlayStatus_.m_cCurHyperCount	= max( static_cast<char>( 0 ), static_cast<char>( GetHyperModeCount() ) );
		kGamePlayStatus_.m_iCurHP			= max( static_cast<int>( 0 ), static_cast<int>( GetNowHp() ) );
		kGamePlayStatus_.m_iCurHyperGage	= max( static_cast<int>( 0 ), static_cast<int>( GetNowSoul() ) );
		kGamePlayStatus_.m_iCurMP			= max( static_cast<int>( 0 ), static_cast<int>( GetNowMp() ) );
		kGamePlayStatus_.m_iMaxHP			= max( static_cast<int>( 0 ), static_cast<int>( GetMaxHp() ) );
		kGamePlayStatus_.m_iMaxMP			= max( static_cast<int>( 0 ), static_cast<int>( GetMaxMp() ) );
		kGamePlayStatus_.m_mapPetMP;		// 생각 좀 해보자...
		kGamePlayStatus_.m_mapQuickSlotCoolTime;	// 이것도 생각 좀 해보자...
		kGamePlayStatus_.m_mapSkillCoolTime;	// 이것도 생각좀 해보자.

		return true;
	}
	else
		return false;
}

/*virtual*/ void CX2GageData::SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus_ )
{
	SetHyperModeCount( static_cast<const BYTE>( kGamePlayStatus_.m_cCurHyperCount ) );
	SetMaxHp( static_cast<const float>( kGamePlayStatus_.m_iMaxHP ) );
	SetMaxMp( static_cast<const float>( kGamePlayStatus_.m_iMaxMP ) );
	SetNowHp( static_cast<const float>( kGamePlayStatus_.m_iCurHP ) );
	SetNowMp( static_cast<const float>( kGamePlayStatus_.m_iCurMP ) );
	SetMaxSoul( 100.0f );
	SetNowSoul( static_cast<const float>( kGamePlayStatus_.m_iCurHyperGage ) );
	
}

bool CX2GageData::Veryfy() const
{
#ifdef FIX_GAGEMANAGER01
	bool bIsVerify = true;
	bool bFindHacking = false;
	string strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_Verify;

	if( m_pHpGage->fNow.Verify()						== false ||
		m_pHpGage->fMax.Verify()						== false )
	{
		bIsVerify = false;
		bFindHacking = true;
		strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_Hp_Verify;
	}
	else if( m_pMpGage->fNow.Verify()					== false ||
		m_pMpGage->fMax.Verify()					== false )
	{
		bIsVerify = false;
		bFindHacking = true;
		strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_Mp_Verify;
	}
	else if( m_ForceDownGage.fNow.Verify()			== false ||
		m_ForceDownGage.fMax.Verify()				== false )
	{
		bIsVerify = false;


		strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_DownGage_Verify;
	}
#ifdef ATTACK_DELAY_GAGE
	else if( m_AttackDelayGage.fNow.Verify()		== false ||
		m_AttackDelayGage.fMax.Verify()		== false )
	{
		bIsVerify = false;
		strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_AttackDelay_Verify;
	}
#endif
	else if( m_pHpGage->fChangeRate.Verify()			== false )
	{
		bIsVerify = false;
		strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_Hp_ChangeRate_Verify;
	}		
	else if( m_pMpGage->fChangeRate.Verify()			== false )
	{
		bIsVerify = false;			
		strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_Mp_ChangeRate_Verify;
	}
	else if( m_ChargeMpGageForDetonation.fNow.Verify()			== false ||
		m_ChargeMpGageForDetonation.fMax.Verify()			== false )
	{
		bIsVerify = false;
		strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_MpChargeGage_Verify;
	}
	else if( m_ChargeMpGageForDetonation.fChangeRate.Verify()	== false )
	{
		bIsVerify = false;
		strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_MpChangeGage_ChangeRate_Verify;
	}
	else if( m_SoulGage.fNow.Verify()				== false ||
		m_SoulGage.fMax.Verify()				== false )
	{
		bIsVerify = false;
		bFindHacking = true;
		strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_SoulGage_Verify;
	}
	else if( m_SoulGage.fChangeRate.Verify()		== false)
	{
		bIsVerify = false;
		strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_SoulGage_ChangeRate_Verify;
	}

	if( bIsVerify == false )
	{			
		if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER &&
			g_pKTDXApp->GetFindHacking() == false )
		{
			g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
		}			


#ifdef ADD_COLLECT_CLIENT_INFO
		g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_03, strAntiHackingDesc.c_str(), bFindHacking, true );
		g_pInstanceData->SetRemainedTimeByForceQuitGame( REMAINED_TIME_BY_FORCE_QUIT_GAME );
#else
		if ( g_pInstanceData != NULL &&
			g_pInstanceData->GetVerifyGageManagerTimer() <= 0.f )
		{


			g_pMain->SendHackMail_DamageHistory(strAntiHackingDesc.c_str());

			g_pInstanceData->SetRemainedTimeByForceQuitGame( REMAINED_TIME_BY_FORCE_QUIT_GAME );

			if( bFindHacking == true )
			{
				g_pKTDXApp->SetFindHacking( true );
			}
		}			
#endif
	}
#endif


#ifdef DUNGEON_ITEM
	if( m_fChangeRateByItem.Verify() == false )
	{
		if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
			g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
		
	}
	if( m_fChangeRateTimeByItem.Verify() == false )
	{
		if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
			g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
		
	}
	if( m_fHyperModeRemainTime.Verify() == false )
	{
		if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
			g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
		
	}
#endif

	return bIsVerify;
}

void CX2GageData::UseSpecialAbilityInVillage( const CX2Item::ItemTemplet* pItemTemplet_ )
{
	const CX2Unit* pX2Unit = g_pData->GetMyUser()->GetSelectUnit();
	const CX2Unit::UnitData* pUnitData = &pX2Unit->GetUnitData();
	const CX2Unit::UNIT_CLASS eUnitClass = pX2Unit->GetClass();
	
	//길드 스킬 중 연금술의 달인(포션, 음식, 던전 회복 아이템의 효과가 X%증가)
	float	fIncreaseRateFromMasterOfAlchemistSkill							= 1.0f;
	bool	bDoIHaveMasterOfAlchemistSkill			= false;
	int		iSkillLvMasterOfAlchemist				= 0;

	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pUnitData->m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_GP_COMMON_MASTER_OF_ALCHEMIST, pUnitData->m_byMemberShipGrade );

		if ( pSkillTemplet != NULL )
		{
			bDoIHaveMasterOfAlchemistSkill  = true;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const CX2UserSkillTree& userSkillTree = pUnitData->m_UserSkillTree;
	
			const int iSkillLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID, true ) );	/// 스킬 레벨
	
			fIncreaseRateFromMasterOfAlchemistSkill = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL, iSkillLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			fIncreaseRateFromMasterOfAlchemistSkill = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}

	}

    unsigned uNumSA = pItemTemplet_->GetNumSpecialAbility();
    for( unsigned u = 0; u != uNumSA; ++u )
	{
        const CX2Item::SpecialAbility& specialAbility = pItemTemplet_->GetSpecialAbility(u);

		switch ( specialAbility.GetType() )
		{
		case CX2Item::SAT_HP_UP:
			{
				float fIncreateRateHpUp = fIncreaseRateFromMasterOfAlchemistSkill;
				float fValue = static_cast< float >( specialAbility.m_Value1 );

				// 퓨리가디언 신진대사 촉진
				switch ( eUnitClass )
				{
				case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
				case CX2Unit::UC_CHUNG_IRON_PALADIN:
					{
						const CX2UserSkillTree& userSkillTree = pUnitData->m_UserSkillTree;

		#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
						int iSkillLv = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST, true );
		#else // UPGRADE_SKILL_SYSTEM_2013
						int iSkillLv = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST );
		#endif // UPGRADE_SKILL_SYSTEM_2013
						
						if ( 0 < iSkillLv )
						{
		#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
							const CX2SkillTree::SkillTemplet* pSkillTemplet = 
								g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST );
							float fIncreaseRateMetabolism = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL, iSkillLv );
		#else // UPGRADE_SKILL_SYSTEM_2013
							const CX2SkillTree::SkillTemplet* pSkillTemplet = 
								g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST, iSkillLv );
							float fIncreaseRateMetabolism = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL );
		#endif // UPGRADE_SKILL_SYSTEM_2013
							fIncreaseRateMetabolism = CalculateIncreasingRate( fIncreaseRateMetabolism );

							fIncreateRateHpUp += fIncreaseRateMetabolism;
						} // if
					} break;

				default:
					break;
				}
				

				// 길드 스킬 중 연금술의 달인(포션, 음식, 던전 회복 아이템의 효과가 X%증가)
				fValue *= fIncreateRateHpUp;

				UpNowHp( fValue );
				
// 				std::wstringstream strStream;
// 				strStream << L"HP+ " << static_cast< int >( fValue );
// 				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
// 				pos.y += 50.0f + (i * 30.0f);
// 				if( NULL != g_pData->GetPicCharGreen() )
// 					g_pData->GetPicCharGreen()->DrawText( strStream.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER 
			} break;

		case CX2Item::SAT_MP_UP:
			{
				float fIncreaseRateMpUp = fIncreaseRateFromMasterOfAlchemistSkill;
				float fValue = static_cast< float >( specialAbility.m_Value1 );
				// 길드 스킬 중 연금술의 달인(포션, 음식, 던전 회복 아이템의 효과가 X%증가)
				fValue *= fIncreaseRateMpUp;

				UpNowMp( fValue );
				
// 				std::wstringstream strStream;
// 				strStream << L"MP+ " << static_cast< int >( fValue );
// 				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
// 				pos.y += 50.0f + (i * 30.0f);
// 				if( NULL != g_pData->GetPicCharBlue() )
// 					g_pData->GetPicCharBlue()->DrawText( strStream.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );

			} break;

		case CX2Item::SAT_SOUL_GAGE_UP:
			{
				UpNowSoul( static_cast<float>( specialAbility.m_Value1 ) );

// 				std::wstringstream strStream;
// 				strStream << L"SOUL+ " << pSa->m_Value1;
// 				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
// 				pos.y += 50.0f + (i * 30.0f);
// 				if( NULL != g_pData->GetPicCharRed() )
// 					g_pData->GetPicCharRed()->DrawText( strStream.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
			} break;

		case CX2Item::SAT_HP_PERCENT_UP:
			{
				float fIncreateRateHpPercentUp = fIncreaseRateFromMasterOfAlchemistSkill;
				float fValue = specialAbility.m_Value1 / 100.0f;

				// 퓨리가디언 신진대사 촉진
				if ( CX2Unit::UC_CHUNG_FURY_GUARDIAN == eUnitClass )
				{
					const CX2UserSkillTree& userSkillTree = pUnitData->m_UserSkillTree;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
					int iSkillLv = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST, true );
#else // UPGRADE_SKILL_SYSTEM_2013
					int iSkillLv = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST );
#endif // UPGRADE_SKILL_SYSTEM_2013
					
					if ( 0 < iSkillLv )
					{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
						const CX2SkillTree::SkillTemplet* pSkillTemplet = 
							g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST );

						float fIncreaseRateMetabolism = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL, iSkillLv );
#else // UPGRADE_SKILL_SYSTEM_2013
						const CX2SkillTree::SkillTemplet* pSkillTemplet = 
							g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST, iSkillLv );

						float fIncreaseRateMetabolism = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL );
#endif // UPGRADE_SKILL_SYSTEM_2013
						fIncreaseRateMetabolism = CalculateIncreasingRate( fIncreaseRateMetabolism );

						fIncreateRateHpPercentUp += fIncreaseRateMetabolism;
					} // if
				}
				// 길드 스킬 중 연금술의 달인(포션, 음식, 던전 회복 아이템의 효과가 X%증가)
				if ( true == bDoIHaveMasterOfAlchemistSkill &&
                    EGG_ITEM_ID == pItemTemplet_->GetItemID()
                    )
					fValue += ( iSkillLvMasterOfAlchemist / 100.0f );

				fValue *= fIncreateRateHpPercentUp;

				UpNowHp( GetMaxHp() * fValue );

				std::wstringstream strStream;
				strStream << L"HP+ " << static_cast< int >( fValue * 100 ) << L" PERCENT";
// 				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
// 				pos.y += 50.0f + (i * 30.0f);
// 				if( NULL != g_pData->GetPicCharGreen() )
// 					g_pData->GetPicCharGreen()->DrawText( strStream.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );

			} break;

		case CX2Item::SAT_MP_PERCENT_UP:
			{
				float fValue = specialAbility.m_Value1 / 100.0f;
				if ( bDoIHaveMasterOfAlchemistSkill == true )
					fValue *= fIncreaseRateFromMasterOfAlchemistSkill;

				UpNowMp( GetMaxMp() * fValue );

// 				std::wstringstream strStream;
// 				strStream << L"MP+ " << static_cast< int >( pSa->m_Value1 * fIncreaseRateFromMasterOfAlchemistSkill ) << L" PERCENT";
// 				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
// 				pos.y += 50.0f + (i * 30.0f);
// 				if( NULL != g_pData->GetPicCharGreen() )
// 					g_pData->GetPicCharBlue()->DrawText( strStream.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
			} break;

		default:
			break;
		}		
	}
	
}

/*virtual*/ CX2GageData* CX2GageData::GetCloneGageData()
{
	return new CX2GageData( *this );
}

/*virtual*/ void CX2GageData::CopyGageData( IN CX2GageData* pGageData_ )
{
	ASSERT( NULL != pGageData_ );
	*this = *pGageData_;
}

/*virtual*/ void CX2ChungGageData::UpdateDataFromGameUnit( const CX2GageData* pGageData_ )
{
#ifdef GAGEDATA_NULLCHECK
	if( NULL == pGageData_ )
		return;

#endif // GAGEDATA_NULLCHECK

	CX2GageData::UpdateDataFromGameUnit( pGageData_ );

	// 캐논볼 등!!
	const CX2ChungGageData* pChungGageData = static_cast<const CX2ChungGageData*>( pGageData_ );

#ifdef GAGEDATA_NULLCHECK
	if( NULL != pChungGageData )
#endif //GAGEDATA_NULLCHECK
	{
		SetNowCannonBallCount( pChungGageData->GetNowCannonBallCount() );
		SetMaxCannonBallCount( pChungGageData->GetMaxCannonBallCount() );
	
		SetNowCannonBallCountEx( pChungGageData->GetNowCannonBallCountEx() );
		SetMaxCannonBallCountEx( pChungGageData->GetMaxCannonBallCountEx() );

		SetBerserkMode( pChungGageData->GetBerserkMode() );
		SetBerserkModeChanged( pChungGageData->GetBerserkModeChanged() );
		SetCannonBallChanged( pChungGageData->GetCannonBallChanged() );
	}
}

/*virtual*/ void CX2ChungGageData::InitWhenGameIsOver()
{
	CX2GageData::InitWhenGameIsOver();
	SetBerserkMode( false );
	SetBerserkModeChanged( true );
}

/*virtual*/ bool CX2ChungGageData::GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const
{
	/// EXTRA_CANNON_BALL_COUNT_BITMASK_VALUE(7)을 곱해준 이유는 bitmask 처럼 생각하면 된다.
	kGamePlayStatus.m_iCharAbilCount = static_cast<char>( GetNowCannonBallCountEx() * EXTRA_CANNON_BALL_COUNT_BITMASK_VALUE + GetNowCannonBallCount() ); // 청

	return CX2GageData::GetMyPlayStatusToPacket( kGamePlayStatus );
}

/*virtual*/ void CX2ChungGageData::SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus )
{
	CX2GageData::SetMyPlayStatusFromPacket( kGamePlayStatus );

	/// EXTRA_CANNON_BALL_COUNT_BITMASK_VALUE(7) 부분 bitmask 연산 처럼 생각하면 된다.
	const int iCannonBallCount = kGamePlayStatus.GetCannonBallCount() % EXTRA_CANNON_BALL_COUNT_BITMASK_VALUE;
	const int iExtraCannonBallCount = (kGamePlayStatus.GetCannonBallCount() - iCannonBallCount) / EXTRA_CANNON_BALL_COUNT_BITMASK_VALUE;
	
	// 여기서 MaxCount를 m_cCannonBallCount로 셋팅한 이유는 MaxCount 값이 아직은 0이어서 NowCannonBallCount가 제대로 셋팅 되지 않기 때문
	SetMaxCannonBallCount( iCannonBallCount );
	SetNowCannonBallCount( iCannonBallCount );

	SetNowCannonBallCountEx( iExtraCannonBallCount );
	SetMaxCannonBallCountEx( iExtraCannonBallCount );

	SetCannonBallChanged( true );
}

/*virtual*/ CX2GageData* CX2ChungGageData::GetCloneGageData()
{
	return new CX2ChungGageData( *this );
}

void CX2ChungGageData::CopyGageData( IN CX2GageData* pGageData_ )
{
	ASSERT( NULL != pGageData_ );
	*this = *( static_cast<CX2ChungGageData*>( pGageData_ ) );
	SetCannonBallChanged( true );
}




/*virtual*/ void CX2ElswordGageData::UpdateDataFromGameUnit( const CX2GageData* pGageData_ )
{
#ifdef GAGEDATA_NULLCHECK
	if ( NULL == pGageData_ )
		return;
#endif //GAGEDATA_NULLCHECK

	CX2GageData::UpdateDataFromGameUnit( pGageData_ );

	// 검의 길
	const CX2ElswordGageData* pElswordGageData = static_cast<const CX2ElswordGageData*>( pGageData_ );

#ifdef GAGEDATA_NULLCHECK
	if( NULL != pElswordGageData )
	{
#endif //GAGEDATA_NULLCHECK
		SetWayOfSwordState( pElswordGageData->GetWayOfSwordState() );
		SetWayOfSwordPoint( pElswordGageData->GetWayOfSwordPoint() );
#ifdef GAGEDATA_NULLCHECK
	}
#endif //GAGEDATA_NULLCHECK
}

/*virtual*/ void CX2ElswordGageData::InitWhenGameIsOver()
{
	CX2GageData::InitWhenGameIsOver();
	SetWayOfSwordState( 0 );
	SetChangedWayOfSwordState( true );
}

bool CX2ElswordGageData::GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const
{
	kGamePlayStatus.m_iCharAbilCount = static_cast<int>( GetWayOfSwordPoint() );	// 엘소드
	return CX2GageData::GetMyPlayStatusToPacket( kGamePlayStatus );
}

/*virtual*/ void CX2ElswordGageData::SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus )
{
	CX2GageData::SetMyPlayStatusFromPacket( kGamePlayStatus );
	SetWayOfSwordPoint( static_cast<float>( kGamePlayStatus.GetWSP() ) );
}

CX2GageData* CX2ElswordGageData::GetCloneGageData()
{
	return new CX2ElswordGageData( *this );
}

void CX2ElswordGageData::CopyGageData( IN CX2GageData* pGageData_ )
{
	ASSERT( NULL != pGageData_ );
	*this = *( static_cast<CX2ElswordGageData*>( pGageData_ ) );
	SetChangedWayOfSwordState( true );
}

#pragma region CX2AraGageData

#pragma region GetBerserkMode
/** @function 	: GetBerserkMode
	@brief 		: 광폭화 모드 여부 얻기
*/
bool CX2AraGageData::GetBerserkMode() const
{
	return m_bBerserkMode;
}
#pragma endregion 광폭화 모드 확인

#pragma region SetBerserkMode
/** @function 	: SetBerserkMode
	@brief 		: 광폭화 모드 설정
*/
void CX2AraGageData::SetBerserkMode( const bool bBerserkMode_ )
{
	m_bBerserkMode = bBerserkMode_;
}
#pragma endregion 광폭화 모드 설정

#pragma region GetBerserkModeChanged
/** @function 	: GetBerserkModeChanged
	@brief 		: 광폭화 모드로 변경되었는지 확인
*/
bool CX2AraGageData::GetBerserkModeChanged() const
{
	return m_bBerserkModeChanged;
}
#pragma endregion 광폭화 모드로 변경되었는지 확인

#pragma region SetBerserkModeChanged
/** @function 	: SetBerserkModeChanged
	@brief 		: 광폭화 모드로 변경되었는지 설정
*/
void CX2AraGageData::SetBerserkModeChanged( bool bBerserkModeChanged_ )
{
	m_bBerserkModeChanged = bBerserkModeChanged_;
}
#pragma endregion 광폭화 모드로 변경되었는지 설정

#pragma region GetForcePowerChanged
/** @function 	: GetForcePowerChanged
	@brief 		: 기력 변경 확인
*/
bool CX2AraGageData::GetForcePowerChanged() const
{
	return m_bForcePowerChanged;
}
#pragma endregion 기력 변경 확인

#pragma region SetForcePowerChanged
/** @function 	: SetForcePowerChanged
	@brief 		: 기력 변경 설정
*/
void CX2AraGageData::SetForcePowerChanged( const bool bForcePowerChanged_ )
{
	m_bForcePowerChanged = bForcePowerChanged_;
}
#pragma endregion 기력 변경 설정

#pragma region GetNowForcePower
/** @function 	: GetNowForcePower
	@brief 		: 현재 기력 확인
*/
int CX2AraGageData::GetNowForcePower() const
{
	return m_iNowForcePowerCount;
}
#pragma endregion 현재 기력 확인

#pragma region SetNowForcePower
/** @function 	: SetNowForcePower
	@brief 		: 현재 기력 설정
*/
void CX2AraGageData::SetNowForcePower( const int iNowForcePowerCount_ )
{
	if ( GetMaxForcePower() < iNowForcePowerCount_ )
		m_iNowForcePowerCount = GetMaxForcePower();
	else if ( 0 > iNowForcePowerCount_ )
		m_iNowForcePowerCount = 0;
	else
		m_iNowForcePowerCount = iNowForcePowerCount_;
}
#pragma endregion 현재 기력 설정

#pragma region GetMaxForcePower
/** @function 	: GetMaxForcePower
	@brief 		: 최대 기력 확인
*/
int CX2AraGageData::GetMaxForcePower() const
{
	return m_iMaxForcePowerCount;
}
#pragma endregion 최대 기력 확인

#pragma region SetMaxForcePower
/** @function 	: SetMaxForcePower
	@brief 		: 최대 기력 설정
*/
void CX2AraGageData::SetMaxForcePower( const int iMaxForcePowerCount_ )
{
	m_iMaxForcePowerCount = iMaxForcePowerCount_;
}
#pragma endregion 최대 기력 설정

#pragma region IsFullForcePower
/** @function 	: IsFullForcePower
	@brief 		: 기력이 최대치인지 확인
*/
bool CX2AraGageData::IsFullForcePower() const
{
	return GetNowForcePower() >= GetMaxForcePower();
}
#pragma endregion 기력이 최대치인지 확인

#pragma region IsEmptyForcePower
/** @function 	: IsEmptyForcePower
	@brief 		: 기력이 없는지 확인
*/
bool CX2AraGageData::IsEmptyForcePower() const
{
	return GetNowForcePower() <= 0;
}
#pragma endregion 기력이 없는지 확인

#pragma region UpdateDataFromGameUnit
/** @function 	: UpdateDataFromGameUnit
	@brief 		: GageDate 갱신
*/
/*virtual*/ void CX2AraGageData::UpdateDataFromGameUnit( const CX2GageData* pGageData_ )
{
	CX2GageData::UpdateDataFromGameUnit( pGageData_ );
	const CX2AraGageData* pAraGageData = static_cast<const CX2AraGageData*>( pGageData_ );

#ifdef GAGEDATA_NULLCHECK
	if( NULL != pAraGageData )
	{
#endif //GAGEDATA_NULLCHECK
		SetNowForcePower( pAraGageData->GetNowForcePower() );
		SetMaxForcePower( pAraGageData->GetMaxForcePower() );
	
		SetBerserkMode( pAraGageData->GetBerserkMode() );
		SetBerserkModeChanged( pAraGageData->GetBerserkModeChanged() );
		SetForcePowerChanged( pAraGageData->GetForcePowerChanged() );
#ifdef GAGEDATA_NULLCHECK
	}
#endif //GAGEDATA_NULLCHECK
}
#pragma endregion GageDate 갱신

#pragma region InitWhenGameIsOver
/** @function 	: InitWhenGameIsOver
	@brief 		: 게임이 끝났을 때 초기화
*/
/*virtual*/ void CX2AraGageData::InitWhenGameIsOver()
{
	CX2GageData::InitWhenGameIsOver();
	SetBerserkMode( false );
	SetBerserkModeChanged( true );
}
#pragma endregion 게임이 끝났을 때 초기화

#pragma region GetMyPlayStatusToPacket
/** @function 	: GetMyPlayStatusToPacket
	@brief 		: 패킷에서 나의 플레이 상태 얻기
*/
/*virtual*/ bool CX2AraGageData::GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const
{
	kGamePlayStatus.m_iCharAbilCount = static_cast<char>( GetNowForcePower() );

	return CX2GageData::GetMyPlayStatusToPacket( kGamePlayStatus );
}
#pragma endregion 패킷에서 나의 플레이 상태 얻기

#pragma region SetMyPlayStatusFromPacket
/** @function 	: SetMyPlayStatusFromPacket
	@brief 		: 패킷에 나의 플레이 상태 설정
*/
/*virtual*/ void CX2AraGageData::SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus )
{
	CX2GageData::SetMyPlayStatusFromPacket( kGamePlayStatus );

	const int iForcePower = kGamePlayStatus.GetForcePower();
	const int iLevel = g_pData->GetMyUser()->GetSelectUnit()->GetPrevLevel();
	
	SetMaxForcePower( min( iLevel / 10, 6 ) + 4 );
	SetNowForcePower( iForcePower );
	SetForcePowerChanged( true );
}
#pragma endregion 패킷에 나의 플레이 상태 설정

#pragma region GetCloneGageData
/** @function 	: GetCloneGageData
	@brief 		: Gage 복사 데이터 얻기
*/
/*virtual*/ CX2GageData* CX2AraGageData::GetCloneGageData()
{
	return new CX2AraGageData( *this );
}
#pragma endregion Gage 복사 데이터 얻기

#pragma region CopyGageData
/** @function 	: CopyGageData
	@brief 		: GageDate 복사
*/
void CX2AraGageData::CopyGageData( IN CX2GageData* pGageData_ )
{
	ASSERT( NULL != pGageData_ );
	*this = *( static_cast<CX2AraGageData*>( pGageData_ ) );
	SetForcePowerChanged( true );
}
#pragma endregion GageDate 복사

#pragma endregion 클래스


#ifdef NEW_CHARACTER_EL
/*virtual*/ void CX2ElesisGageData::DetonationFrameMove( double fTime, float fElapsedTime )
{
	// 기폭 관련
	if( GetCanChargeMpForDetonation() == true )
	{
#ifdef ELSWORD_WAY_OF_SWORD
		// 섬멸 상태에서는 섬멸 게이지 이용한 기폭 처리
		if( WSS_DESTRUCTION == GetWayOfSwordState() )		
		{
			// 기폭 최대 수치 설정
			SetMaxChargeMpForDetonation( _CONST_ELESIS_DETONATION_::MAX_CHARGE_DETONATION_VALUE );

			// 기폭 충전 설정
			UpNowChargeMpForDetonation( _CONST_ELESIS_DETONATION_::CHARGE_RATE_FOR_DETONATION * fElapsedTime );

			// 기폭 최대 수치로 제한
			const float fNowChargeMp = GetNowChargeMpForDetonation();
			if( fNowChargeMp > GetMaxChargeMpForDetonation() )
				SetNowChargeMpForDetonation( GetMaxChargeMpForDetonation() );

			SetCanChargeMpForDetonation( false );
		}
		else
#endif // ELSWORD_WAY_OF_SWORD
		{
			SetMaxChargeMpForDetonation( GetNowMp() );
			UpNowChargeMpForDetonation( GetChangeRateChargeMpForDetonation() * fElapsedTime );

			const float fNowChargeMp = GetNowChargeMpForDetonation();

			if( fNowChargeMp > THREE_CHARGE )
				SetNowChargeMpForDetonation( THREE_CHARGE );
			else if( fNowChargeMp > GetMaxChargeMpForDetonation() )
				SetNowChargeMpForDetonation( GetMaxChargeMpForDetonation() );
			else if( fNowChargeMp < 0.0f )
				SetNowChargeMpForDetonation( 0.0f );

			SetCanChargeMpForDetonation( false );
		}
	}
	else
	{
		SetNowChargeMpForDetonation( 0.0f );
		SetCanChargeMpForDetonation( false );
	}
}

/*virtual*/ void CX2ElesisGageData::UpdateDataFromGameUnit( const CX2GageData* pGageData_ )
{
#ifdef GAGEDATA_NULLCHECK
	if( NULL == pGageData_ )
		return;
#endif //GAGEDATA_NULLCHECK

	CX2GageData::UpdateDataFromGameUnit( pGageData_ );

	// 검의 길
	const CX2ElesisGageData* pElesisGageData = static_cast<const CX2ElesisGageData*>( pGageData_ );

#ifdef GAGEDATA_NULLCHECK
	if( NULL != pElesisGageData )
#endif //GAGEDATA_NULLCHECK
	{
		SetWayOfSwordState( pElesisGageData->GetWayOfSwordState() );
		SetWayOfSwordPoint( pElesisGageData->GetWayOfSwordPoint() );
	}
}

/*virtual*/ void CX2ElesisGageData::InitWhenGameIsOver()
{
	CX2GageData::InitWhenGameIsOver();
	SetWayOfSwordState( 0 );
	SetChangedWayOfSwordState( true );
}

bool CX2ElesisGageData::GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const
{
	kGamePlayStatus.m_iCharAbilCount = static_cast<int>( GetWayOfSwordPoint() );	// 엘소드
	return CX2GageData::GetMyPlayStatusToPacket( kGamePlayStatus );
}

/*virtual*/ void CX2ElesisGageData::SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus )
{
	CX2GageData::SetMyPlayStatusFromPacket( kGamePlayStatus );
	SetWayOfSwordPoint( static_cast<float>( kGamePlayStatus.GetWSP() ) );
}

CX2GageData* CX2ElesisGageData::GetCloneGageData()
{
	return new CX2ElesisGageData( *this );
}

void CX2ElesisGageData::CopyGageData( IN CX2GageData* pGageData_ )
{
	ASSERT( NULL != pGageData_ );
	*this = *( static_cast<CX2ElesisGageData*>( pGageData_ ) );
	SetChangedWayOfSwordState( true );
}
/*virtual*/ bool CX2ElesisGageData::DamageFlushMp()
{
#ifdef ELSWORD_WAY_OF_SWORD // 검의 길
	if( WSS_DESTRUCTION == GetWayOfSwordState() )
	{		
		if( GetNowChargeMpForDetonation() >= _CONST_ELESIS_DETONATION_::MAX_CHARGE_DETONATION_VALUE )	
		{
			const float fConsumeWspForDetonation = g_pData->GetDamageManager()->GetDestRageWSPGain_Elesis();
			float fWayOfSwordPoint = GetWayOfSwordPoint();
			// 기본 수치가 음수값이기 때문에 소모시키려면 + 연산 필요
			fWayOfSwordPoint += fConsumeWspForDetonation;

			// 소모로 인해 질풍 수치로 변하지 않도록 최대값 제한
			SetWayOfSwordPoint( min( 0.f, fWayOfSwordPoint ) );
			return true;
		}
		else
			return false;
	}
	else
#endif //ELSWORD_WAY_OF_SWORD
	{
		return CX2GageData::DamageFlushMp();
	}

	return false;
}

#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
CX2GageData* CX2AddGageData::GetCloneGageData()
{
	return new CX2AddGageData( *this );
}

void CX2AddGageData::CopyGageData( IN CX2GageData* pGageData_ )
{
	ASSERT( NULL != pGageData_ );
	*this = *( static_cast<CX2AddGageData*>( pGageData_ ) );

	SetChangedMutationCount( true );	/// 변이 게이지 갱신
}

/*virtual*/ void CX2AddGageData::UpdateDataFromGameUnit( const CX2GageData* pGageData_ )
{
	if( NULL == pGageData_ )
		return;

	CX2GageData::UpdateDataFromGameUnit( pGageData_ );

	const CX2AddGageData* pAddrGageData = static_cast<const CX2AddGageData*>( pGageData_ );

	if( NULL != pAddrGageData )
	{
		SetIsFormationMode( pAddrGageData->GetIsFormationMode() );
		SetChangedMutationCount( pAddrGageData->GetChangedMutationCount() );
		SetMutationCount( pAddrGageData->GetMutationCount() );
		SetDPValue( pAddrGageData->GetDPValue() );
		SetChangeFormationCoolTime( pAddrGageData->GetChangeFormationCoolTime() );
		SetMaxDPValue( MAX_DP_GAGE_VALUE );
	}
}

/*virtual*/ bool CX2AddGageData::GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const
{
	if ( Veryfy() )
	{
		CX2GageData::GetMyPlayStatusToPacket( kGamePlayStatus );

		int iSyncData = ( true == GetIsFormationMode() ) ? 1 : 0;				/// 구성 모드 적용 여부 설정

		iSyncData = iSyncData << 4;												/// 구성 모드 전환 쿨타임을 넣기 위해, 4비트만 앞으로 옮기자

		iSyncData |= static_cast<int>( GetChangeFormationCoolTime() ) & 0x000F;	/// 구성 모드 전환 쿨타임 설정

		iSyncData = iSyncData << 4;												/// 변이 수치를 넣기 위해, 4비트만 앞으로 옮기자

		iSyncData |= GetMutationCount() & 0x000F;								/// 변이 수치 설정

		iSyncData =  iSyncData << 16;											/// DP 수치를 넣기 위해, 16비트만 앞으로 옮자

		iSyncData |= static_cast<int>( GetDPValue() ) & 0xFFFF;					/// DP 수치 설정

		kGamePlayStatus.m_iCharAbilCount = iSyncData;

	return true;
	}
	else
		return false;
}

/*virtual*/ void CX2AddGageData::SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus )
{
	CX2GageData::SetMyPlayStatusFromPacket( kGamePlayStatus );

	int iSyncData = kGamePlayStatus.GetNPAndMutationAndFormationMode();

	SetIsFormationMode( ( ( ( iSyncData >> 24 ) & 0x000F ) == 1 ) ? true : false  );		/// 구성 모드 적용 여부 대입

	SetChangeFormationCoolTime( static_cast<float>( ( iSyncData  >> 20 ) & 0x000F ) );	/// 구성 모드 전환 쿨타임 대입

	SetMutationCount( ( iSyncData  >> 16 ) & 0x000F  );									/// 변이 수치 대입

	SetDPValue( static_cast<float>( iSyncData & 0xFFFF ) );								/// DP 수치 대입

	SetMaxDPValue( MAX_DP_GAGE_VALUE );													/// DP 최대 수치 대입

	SetChangedMutationCount( true );													/// 갱신
}

/*virtual*/ void CX2AddGageData::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2GageData::OnFrameMove(fTime, fElapsedTime);

	/// 각성 쿨타임 연산
	if ( 0.f < m_fChangeFormationCoolTime ) 
	{
		m_fChangeFormationCoolTime -= fElapsedTime;

		if ( 0.f >= m_fChangeFormationCoolTime )
			m_fChangeFormationCoolTime = 0.f;
	}
}
#endif //SERV_9TH_NEW_CHARACTER



#ifdef ADD_RENA_SYSTEM //김창한
/** @function 	: GetNFBuffMode
	@brief 		: NF 버프 모드 설정
*/
bool CX2RenaGageData::GetNFBuffMode() const
{
	return m_bNFBuffMode;
}

/** @function 	: SetNFBuffMode
	@brief 		: NF 버프 모드 설정
*/
void CX2RenaGageData::SetNFBuffMode( const bool bNFBuffMode_ )
{
	m_bNFBuffMode = bNFBuffMode_;
}

/** @function 	: GetNaturalForceChanged
	@brief 		: NF 게이지 변화 설정
*/
bool CX2RenaGageData::GetNaturalForceChanged() const
{
	return m_bNaturalForceChanged;
}

/** @function 	: SetNaturalForceChanged
	@brief 		: NF 게이지 변화 설정
*/
void CX2RenaGageData::SetNaturalForceChanged( const bool bNaturalForceChanged_ )
{
	m_bNaturalForceChanged = bNaturalForceChanged_;
}

/** @function 	: GetNowNaturalForce
	@brief 		: 현재 NF 게이지
*/
int CX2RenaGageData::GetNowNaturalForce() const
{
	return m_iNowNaturalForceCount;
}

/** @function 	: SetNowNaturalForce
	@brief 		: 현재 NF 게이지
*/
void CX2RenaGageData::SetNowNaturalForce( const int iNowNaturalForceCount_ )
{
	if( GetMaxNaturalForce() < iNowNaturalForceCount_ )
		m_iNowNaturalForceCount = GetMaxNaturalForce();
	else if( 0 > iNowNaturalForceCount_ )
		m_iNowNaturalForceCount = 0;
	else
		m_iNowNaturalForceCount = iNowNaturalForceCount_;

}

/** @function 	: GetMaxNaturalForce
	@brief 		: 최대 NF 게이지
*/
int CX2RenaGageData::GetMaxNaturalForce() const
{
	return m_iMaxNaturalForceCount;
}

/** @function 	: GetMaxNaturalForce
	@brief 		: 최대 NF 게이지
*/
void CX2RenaGageData::SetMaxNaturalForce( const int iMaxNaturalForceCount_ )
{
	m_iMaxNaturalForceCount = iMaxNaturalForceCount_;
}

/** @function 	: IsFullNaturalForce
	@brief 		: NF 게이지가 최대치인지 체크
*/
bool CX2RenaGageData::IsFullNaturalForce() const
{
	return GetNowNaturalForce() >= GetMaxNaturalForce();
}

/** @function 	: IsEmptyNaturalForce
	@brief 		: NF 게이지가 비었는지 체크
*/
bool CX2RenaGageData::IsEmptyNaturalForce() const
{
	return GetNowNaturalForce() <= 0;
}

/** @function 	: UpdateDataFromGameUnit
	@brief 		: GageDate 갱신
*/
/*virtual*/ void CX2RenaGageData::UpdateDataFromGameUnit( const CX2GageData* pGageData_ )
{
	CX2GageData::UpdateDataFromGameUnit( pGageData_ );
	const CX2RenaGageData* pRenaGageData = static_cast<const CX2RenaGageData*>( pGageData_ );

#ifdef GAGEDATA_NULLCHECK
	if( NULL != pRenaGageData )
#endif //GAGEDATA_NULLCHECK
	{
		SetNowNaturalForce( pRenaGageData->GetNowNaturalForce() );
		SetMaxNaturalForce( MAX_NATURAL_FORCE_VALUE );

		SetNFBuffMode( pRenaGageData->GetNFBuffMode() );
		SetNaturalForceChanged( pRenaGageData->GetNaturalForceChanged() );
	}
}

/** @function 	: InitWhenGameIsOver
	@brief 		: 게임이 끝났을 때 초기화
*/
/*virtual*/ void CX2RenaGageData::InitWhenGameIsOver()
{
	CX2GageData::InitWhenGameIsOver();
	SetNFBuffMode( false );
}

/** @function 	: GetMyPlayStatusToPacket
	@brief 		: 패킷에서 나의 플레이 상태 얻기
*/
/*virtual*/ bool CX2RenaGageData::GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus ) const
{
	kGamePlayStatus.m_iCharAbilCount = static_cast<char>( GetNowNaturalForce() );

	return CX2GageData::GetMyPlayStatusToPacket( kGamePlayStatus );
}

/** @function 	: SetMyPlayStatusFromPacket
	@brief 		: 패킷에 나의 플레이 상태 설정
*/
/*virtual*/ void CX2RenaGageData::SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus )
{
	CX2GageData::SetMyPlayStatusFromPacket( kGamePlayStatus );

	const int iNaturalForce = kGamePlayStatus.GetNaturalForce();
	
	SetMaxNaturalForce( MAX_NATURAL_FORCE_VALUE );
	SetNowNaturalForce( iNaturalForce );
	SetNaturalForceChanged(true);
}

/** @function 	: GetCloneGageData
	@brief 		: Gage 복사 데이터 얻기
*/
/*virtual*/ CX2GageData* CX2RenaGageData::GetCloneGageData()
{
	return new CX2RenaGageData( *this );
}

/** @function 	: CopyGageData
	@brief 		: GageDate 복사
*/
void CX2RenaGageData::CopyGageData( IN CX2GageData* pGageData_ )
{
	ASSERT( NULL != pGageData_ );
	*this = *( static_cast<CX2RenaGageData*>( pGageData_ ) );
	SetNaturalForceChanged( true );
}


#endif //ADD_RENA_SYSTEM
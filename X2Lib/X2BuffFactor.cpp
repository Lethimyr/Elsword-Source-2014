/**@file : X2BuffFactor.cpp
   @breif : 버프를 발생 시키는 구성요소를 모아놓은 파일
*/
#pragma once

#include "StdAfx.h"
#include ".\X2BuffFactor.h"

#define CASE_BEHAVIOR_FACTOR( func_, type_ ) \
case type_: \
	{ \
	if ( false == ##func_( luaManager_ , STRINGIZEA2(type_), type_ ) ) \
	return DISPLAY_ERROR( STRINGIZE2(type_) ); \
	} break; \

#define CASE_FINALIZER_FACTOR( func_, type_ ) \
case type_: \
	{ \
	if ( false == ##func_( luaManager_ , STRINGIZEA2(type_), type_ ) ) \
	return DISPLAY_ERROR( STRINGIZE2(type_) ); \
	} break; \

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang

#define CASE_BEHAVIOR_FACTOR_SKILL( func_, type_, isSkill_ ) \
case type_: \
	{ \
	if ( false == ##func_( luaManager_ , STRINGIZEA2(type_), type_, isSkill_ ) ) \
	return DISPLAY_ERROR( STRINGIZE2(type_) ); \
	} break; \

#define CASE_FINALIZER_FACTOR_SKILL( func_, type_, isSkill_ ) \
case type_: \
	{ \
	if ( false == ##func_( luaManager_ , STRINGIZEA2(type_), type_, isSkill_ ) ) \
	return DISPLAY_ERROR( STRINGIZE2(type_) ); \
	} break; \

#endif //UPGRADE_SKILL_SYSTEM_2013

/** @function : GetBehaviorFactor
	@brief : 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	@param : 요소 타입(eType_), 정보를 담아갈 이중 포인터(ppFactor_)
	@return : 성공시 true, 실패시 false
*/
bool CX2BuffFactor::GetBehaviorFactor( const BUFF_BEHAVIOR_TYPE eType_, OUT const KBuffBehaviorFactor** ppFactor_ ) const
{
	BOOST_FOREACH( const KBuffBehaviorFactor& factor, m_pDataBuffFactor->m_vecBuffBehaviorFactors )
	{
		if ( eType_ == static_cast<BUFF_BEHAVIOR_TYPE>( factor.m_uiType ) )
		{
			*ppFactor_ = &factor;
			return true;
		}
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	return false;
#else // UPGRADE_SKILL_SYSTEM_2013
	return DISPLAY_ERROR( L"Behavior Factor doesn't exist" );
#endif // UPGRADE_SKILL_SYSTEM_2013
}

/** @function : GetFinalizerFactor
	@brief : 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	@param : 요소 타입(eType_), 정보를 담아갈 이중 포인터(ppFactor_)
	@return : 성공시 true, 실패시 false
*/
bool CX2BuffFactor::GetFinalizerFactor( const BUFF_FINALIZER_TYPE eType_, OUT const KBuffFinalizerFactor** ppFactor_ ) const
{
	BOOST_FOREACH( const KBuffFinalizerFactor& factor, m_pDataBuffFactor->m_vecBuffFinalizerFactors )
	{
		if ( eType_ == factor.m_uiType )
		{
			*ppFactor_ = &factor;
			return true;
		}
	}

	return DISPLAY_ERROR( L"Finalizer Factor doesn't exist" );
}

/** @function : ParsingCombinationBehavior
	@brief : 어떤 행동요소들의 조합으로 이루어졌는지 알아낸 후 해당 조합들을 파싱함
	@param : 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingCombinationBehavior( KLuaManager& luaManager_, bool bIsSkill/* = true*/ )
{
	vector<UINT> vecBehaviorType;

#ifdef EXCEPTION_BUFF_FACTOR_VER2
	bool bExceptionCase = true;
#endif // EXCEPTION_BUFF_FACTOR_VER2

	/// Behavior가 어떤 행동요소들의 조함으로 이루어 졌는지 얻어옴
	if ( CX2BuffTempletManager::GetCombination( luaManager_, vecBehaviorType ) )
	{
		vector<UINT>::iterator vItr = vecBehaviorType.begin();
		while ( vecBehaviorType.end() != vItr )		/// 조합에 포함된 행동요소들을 파싱
		{
#ifdef EXCEPTION_BUFF_FACTOR_VER2
			if( static_cast<BUFF_DISPLAYER_TYPE>( *vItr ) != BBT_REFLECT_MAGIC )
				bExceptionCase = false;
#endif // EXCEPTION_BUFF_FACTOR_VER2

			switch ( static_cast<BUFF_BEHAVIOR_TYPE>( *vItr ) )
			{
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeNowHpPerSecondBehavior, BBT_CHANGE_NOW_HP_PER_SECOND, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeNowMpPerSecondBehavior, BBT_CHANGE_NOW_MP_PER_SECOND, bIsSkill )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_REVERSE_LEFT_RIGHT )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_DASH_IMPOSSIBLE )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_ATTACK_IMPOSSIBLE )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_DOWN_IMPOSSIBLE )

				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_MAX_HP, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_PHYSIC_ATTACK, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_MAGIC_ATTACK, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_PHYSIC_DEFENCE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_MAGIC_DEFENCE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_ANIM_SPEED, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_WALK_RUN_SPEED, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_JUMP_POWER, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_ACCURACY, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_AVOIDANCE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_CRITICAL_RATE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_CRITICAL_ATTACK, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_ANTI_CRITICAL_RATE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_CRITICAL_DEFENCE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_ADDITIONAL_ATTACK, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_ADDITIONAL_DEFENCE, bIsSkill )

				CASE_BEHAVIOR_FACTOR( ParsingCreateBuffFactorBehavior, BBT_CREATE_BUFF_FACTOR )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingAbsorbEffectAttackBehavior, BBT_ABSORB_EFFECT_ATTACK, bIsSkill )

				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHARGE_MP_HIT_MELEE_ATTACK, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHARGE_MP_HIT_RANGE_ATTACK, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHARGE_MP_HITTED, bIsSkill )

				CASE_BEHAVIOR_FACTOR( ParsingGameUnitChangeHyperModeBehavior, BBT_CHANGE_HYPER_MODE )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_SUPER_ARMOR )

				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_MELEE_ATTACK, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_RANGE_ATTACK, bIsSkill )

				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_FINAL_DEFENCE_MULTIPLIER, bIsSkill )

				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_FORCE_DOWN_MULTIPLIER_MELEE_ATTACK, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_FORCE_DOWN_MULTIPLIER_RANGE_ATTACK, bIsSkill )

				CASE_BEHAVIOR_FACTOR_SKILL( ParsingResetSkillCoolTimeByBuffBehavior, BBT_RESET_SKILL_COOL_TIME, bIsSkill )

				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_CHANGE_UNIT_SCALE )

				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_BLAZE_DEFENCE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_WATER_DEFENCE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_NATURE_DEFENCE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_WIND_DEFENCE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_LIGHT_DEFENCE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_DARK_DEFENCE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_EVERY_ATTRIBUTE_DEFENCE, bIsSkill )

				CASE_BEHAVIOR_FACTOR( ParsingCustomBehavior, BBT_CUSTOM_FUNCTION )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_DRAIN_HP_NORMAL_ATTACK, bIsSkill )
				CASE_BEHAVIOR_FACTOR( ParsingEffectSetWithDamageBehavior, BBT_EFFECT_SET_WITH_DAMAGE )

				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_STUN )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_FINISH_OTHER_BUFF_DEBUFF )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_FROZEN )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_CAN_PASS_UNIT )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_ATTACK_ALL_TEAM )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_NEVER_MOVE )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_MAX_MP, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeAttackByTypeBehavior, BBT_CHANGE_ATTACK_BY_TYPE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeEnchantAttackRateBehavior, BBT_CHANGE_ENCHANT_ATTACK_RATE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingDoubleAttackByBuffBehavior, BBT_DOUBLE_ATTACK, bIsSkill )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_CHANGE_ATTACK_SPHERE_SCALE )
				CASE_BEHAVIOR_FACTOR( ParsingAddSkillLevelByBuffBehavior, BBT_ADD_SKILL_LEVEL )

				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_SKILL_ATTACK_IMPOSSIBLE )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_ZX_ATTACK_IMPOSSIBLE )

	#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
				CASE_BEHAVIOR_FACTOR( ParsingReflectMagicBehavior, BBT_REFLECT_MAGIC )
	#endif // SERV_ARA_CHANGE_CLASS_SECOND
	#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangConsumeMpRateBehavior, BBT_CHANGE_CONSUME_MP_RATE, bIsSkill )
	#endif //FIX_SKILL_BALANCE_AISHA_LENA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				// Z 공격 불가, X 공격 불가 버프 타입 추가
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_Z_ATTACK_IMPOSSIBLE )
				CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_X_ATTACK_IMPOSSIBLE )
				
				// 각 속성 별 인챈트 확률 증가
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeEnchantAttackRateBehavior, BBT_CHANGE_ATTRIBUTE_BLAZE_ATTACK_RATE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeEnchantAttackRateBehavior, BBT_CHANGE_ATTRIBUTE_WATER_ATTACK_RATE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeEnchantAttackRateBehavior, BBT_CHANGE_ATTRIBUTE_NATURE_ATTACK_RATE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeEnchantAttackRateBehavior, BBT_CHANGE_ATTRIBUTE_WIND_ATTACK_RATE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeEnchantAttackRateBehavior, BBT_CHANGE_ATTRIBUTE_LIGHT_ATTACK_RATE, bIsSkill )
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeEnchantAttackRateBehavior, BBT_CHANGE_ATTRIBUTE_DARK_ATTACK_RATE, bIsSkill )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
				/// 각성 충전 속도
				CASE_BEHAVIOR_FACTOR_SKILL( ParsingChangeStatBehavior, BBT_CHANGE_HYPER_CHARGE_SPEED, bIsSkill )
#endif //SERV_ADD_LUNATIC_PSYKER

			case BBT_EMPTY:
				break;
#ifdef EXCEPTION_BUFF_FACTOR_VER2
#ifndef SERV_ARA_CHANGE_CLASS_SECOND
			case BBT_REFLECT_MAGIC:
				++vItr;
				continue;
#endif SERV_ARA_CHANGE_CLASS_SECOND
#endif EXCEPTION_BUFF_FACTOR_VER2
			default:
				return DISPLAY_ERROR( L"Unknown Behavior Factor Type" );	/// false 리턴
				break;
			}

			++vItr;
		}

		return true;
	}
	else
		return true;
}
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingCombinationBehavior( KLuaManager& luaManager_ )
{
	vector<UINT> vecBehaviorType;

	/// Behavior가 어떤 행동요소들의 조함으로 이루어 졌는지 얻어옴
	if ( CX2BuffTempletManager::GetCombination( luaManager_, vecBehaviorType ) )
	{
		vector<UINT>::iterator vItr = vecBehaviorType.begin();
		while ( vecBehaviorType.end() != vItr )		/// 조합에 포함된 행동요소들을 파싱
		{
			switch ( static_cast<BUFF_BEHAVIOR_TYPE>( *vItr ) )
			{
				CASE_BEHAVIOR_FACTOR( ParsingChangeNowHpPerSecondBehavior, BBT_CHANGE_NOW_HP_PER_SECOND )
					CASE_BEHAVIOR_FACTOR( ParsingChangeNowMpPerSecondBehavior, BBT_CHANGE_NOW_MP_PER_SECOND )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_REVERSE_LEFT_RIGHT )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_DASH_IMPOSSIBLE )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_ATTACK_IMPOSSIBLE )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_DOWN_IMPOSSIBLE )

					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_MAX_HP )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_PHYSIC_ATTACK )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_MAGIC_ATTACK )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_PHYSIC_DEFENCE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_MAGIC_DEFENCE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_ANIM_SPEED )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_WALK_RUN_SPEED )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_JUMP_POWER )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_ACCURACY )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_AVOIDANCE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_CRITICAL_RATE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_CRITICAL_ATTACK )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_ANTI_CRITICAL_RATE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_CRITICAL_DEFENCE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_ADDITIONAL_ATTACK )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_ADDITIONAL_DEFENCE )

					CASE_BEHAVIOR_FACTOR( ParsingCreateBuffFactorBehavior, BBT_CREATE_BUFF_FACTOR )
					CASE_BEHAVIOR_FACTOR( ParsingAbsorbEffectAttackBehavior, BBT_ABSORB_EFFECT_ATTACK )

					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHARGE_MP_HIT_MELEE_ATTACK )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHARGE_MP_HIT_RANGE_ATTACK )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHARGE_MP_HITTED )

					CASE_BEHAVIOR_FACTOR( ParsingGameUnitChangeHyperModeBehavior, BBT_CHANGE_HYPER_MODE )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_SUPER_ARMOR )

					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_MELEE_ATTACK )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_RANGE_ATTACK )

					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_FINAL_DEFENCE_MULTIPLIER )

					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_FORCE_DOWN_MULTIPLIER_MELEE_ATTACK )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_FORCE_DOWN_MULTIPLIER_RANGE_ATTACK )

#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환
					/// 쿨타임 설정 값을 파싱하기 위해, 함수 추가
					CASE_BEHAVIOR_FACTOR( ParsingResetSkillCoolTimeByBuffBehavior, BBT_RESET_SKILL_COOL_TIME )
#else // UPGRADE_SKILL_SYSTEM_2013
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_RESET_SKILL_COOL_TIME )
#endif // UPGRADE_SKILL_SYSTEM_2013

					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_CHANGE_UNIT_SCALE )

					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_BLAZE_DEFENCE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_WATER_DEFENCE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_NATURE_DEFENCE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_WIND_DEFENCE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_LIGHT_DEFENCE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_ATTRIBUTE_DARK_DEFENCE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_EVERY_ATTRIBUTE_DEFENCE )

					CASE_BEHAVIOR_FACTOR( ParsingCustomBehavior, BBT_CUSTOM_FUNCTION )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_DRAIN_HP_NORMAL_ATTACK )
					CASE_BEHAVIOR_FACTOR( ParsingEffectSetWithDamageBehavior, BBT_EFFECT_SET_WITH_DAMAGE )

					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_STUN )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_FINISH_OTHER_BUFF_DEBUFF )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_FROZEN )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_CAN_PASS_UNIT )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_ATTACK_ALL_TEAM )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_NEVER_MOVE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeStatBehavior, BBT_CHANGE_MAX_MP )
					CASE_BEHAVIOR_FACTOR( ParsingChangeAttackByTypeBehavior, BBT_CHANGE_ATTACK_BY_TYPE )
					CASE_BEHAVIOR_FACTOR( ParsingChangeEnchantAttackRateBehavior, BBT_CHANGE_ENCHANT_ATTACK_RATE )
					CASE_BEHAVIOR_FACTOR( ParsingDoubleAttackByBuffBehavior, BBT_DOUBLE_ATTACK )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_CHANGE_ATTACK_SPHERE_SCALE )
					CASE_BEHAVIOR_FACTOR( ParsingAddSkillLevelByBuffBehavior, BBT_ADD_SKILL_LEVEL )

					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_SKILL_ATTACK_IMPOSSIBLE )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_ZX_ATTACK_IMPOSSIBLE )

	#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
					CASE_BEHAVIOR_FACTOR( ParsingReflectMagicBehavior, BBT_REFLECT_MAGIC )
	#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					// Z 공격 불가, X 공격 불가 버프 타입 추가
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_Z_ATTACK_IMPOSSIBLE )
					CASE_BEHAVIOR_FACTOR( ParsingEmptyBehavior, BBT_X_ATTACK_IMPOSSIBLE )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			case BBT_EMPTY:
				break;

			default:
				return DISPLAY_ERROR( L"Unknown Behavior Factor Type" );	/// false 리턴
				break;
			}

			++vItr;
		}

		return true;
	}
	else
		return true;
}
#endif //UPGRADE_SKILL_SYSTEM_2013

/** @function : ParsingCombinationFinalizer
	@brief : 어떤 종료요소들의 조합으로 이루어졌는지 알아낸 후 해당 조합들을 파싱함
	@param : 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingCombinationFinalizer( KLuaManager& luaManager_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingCombinationFinalizer( KLuaManager& luaManager_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	vector<UINT> vecFinalizerType;

	/// Finalizer가 어떤 종료요소들의 조함으로 이루어 졌는지 얻어옴
	if ( CX2BuffTempletManager::GetCombination( luaManager_, vecFinalizerType ) )
	{
		vector<UINT>::iterator vItr = vecFinalizerType.begin();
		while ( vecFinalizerType.end() != vItr )	/// 조합에 포함된 종료요소들을 파싱
		{
			switch ( static_cast<BUFF_FINALIZER_TYPE>( *vItr ) )
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				CASE_FINALIZER_FACTOR_SKILL( ParsingTimeFinalizer, BFT_TIME, bIsSkill );
				CASE_FINALIZER_FACTOR_SKILL( ParsingHitCountFinalizer, BFT_HIT_COUNT, bIsSkill );
				CASE_FINALIZER_FACTOR_SKILL( ParsingHittedCountFinalizer, BFT_HITTED_COUNT, bIsSkill );
				CASE_FINALIZER_FACTOR_SKILL( ParsingDamageValueFinalizer, BFT_DAMAGE_VALUE, bIsSkill );
				CASE_FINALIZER_FACTOR_SKILL( ParsingPassHpFinalizer, BFT_PASS_HP, bIsSkill );
				CASE_FINALIZER_FACTOR_SKILL( ParsingPassMpFinalizer, BFT_PASS_MP, bIsSkill );
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_STATE_CHANGE );
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_IMMEDIATE );
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_DUNGEON_STAGE_CHANGE );
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_LIVE_CREATOR );
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_FINISH_HYPER );
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_RIDING_ON_OR_NOT )
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_X2STATE_CHANGE )
#else //UPGRADE_SKILL_SYSTEM_2013
				CASE_FINALIZER_FACTOR( ParsingTimeFinalizer, BFT_TIME );
				CASE_FINALIZER_FACTOR( ParsingHitCountFinalizer, BFT_HIT_COUNT );
				CASE_FINALIZER_FACTOR( ParsingHittedCountFinalizer, BFT_HITTED_COUNT );
				CASE_FINALIZER_FACTOR( ParsingStateChangeFinalizer, BFT_STATE_CHANGE );
				CASE_FINALIZER_FACTOR( ParsingDamageValueFinalizer, BFT_DAMAGE_VALUE );
				CASE_FINALIZER_FACTOR( ParsingPassHpFinalizer, BFT_PASS_HP );
				CASE_FINALIZER_FACTOR( ParsingPassMpFinalizer, BFT_PASS_MP );
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_IMMEDIATE );
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_DUNGEON_STAGE_CHANGE );
#ifdef ADD_LIVE_CREATOR_FINALIZER
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_LIVE_CREATOR );
#endif ADD_LIVE_CREATOR_FINALIZER
#ifdef ADD_BUFF_FINALIZER_FINISH_HYPER
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_FINISH_HYPER );
#endif
#ifdef RIDING_SYSTEM
				CASE_FINALIZER_FACTOR( ParsingEmptyFinalizer, BFT_RIDING_ON_OR_NOT )
#endif	// RIDING_SYSTEM
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef EXCEPTION_BUFF_FACTOR
			case BFT_RIDING_ON_OR_NOT:
				++vItr;
				continue;
#endif // EXCEPTION_BUFF_FACTOR		

			default:
				return DISPLAY_ERROR( L"Unknown Finalizer Factor Type" );	/// false 리턴
				break;
			}

			++vItr;
		}

		return true;
	}
	else
		return false;
}

/** @function : UpdateValueByRelationType
	@brief : 연동수치를 사용해야하는 경우 이 함수를 사용하여 연동수치를 구함
	@param : 연동타입(eRelationType_), 연동수치를 저장할 곳(& fValue_), 게임유닛정보(pGameUnit_)
*/
void CX2BuffFactor::UpdateValueByRelationType( const BUFF_RELATION_TYPE eRelationType_, OUT float& fValue_, CX2GameUnit* pGameUnit_ )
{
    const CX2Stat::Stat& kStat = pGameUnit_->GetStat().GetStat();
	{
		switch ( eRelationType_ )
		{
		case BRT_MAX_HP:
			{
				fValue_ = kStat.m_fBaseHP;
			} break;

		case BRT_PHYSIC_ATTACK:
			{
				fValue_ = kStat.m_fAtkPhysic;
			} break;

		case BRT_MAGIC_ATTACK:
			{
				fValue_ = kStat.m_fAtkMagic;
			} break;

		case BRT_MIX_ATTACK:
			{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				// 잘못된 구문 수정
				fValue_ = ( kStat.m_fAtkPhysic + kStat.m_fAtkMagic ) * 0.5f;
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				fValue_ = ( kStat.m_fAtkPhysic + kStat.m_fAtkPhysic ) * 0.5f;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			} break;

		case BRT_PHYSIC_DEFENCE:
			{
				fValue_ = kStat.m_fDefPhysic;
			} break;

		case BRT_MAGIC_DEFENCE:
			{
				fValue_ = kStat.m_fDefMagic;
			} break;

		case BRT_MIX_DEFENCE:
			{
				fValue_ = ( kStat.m_fDefPhysic + kStat.m_fDefMagic ) * 0.5f;
			} break;

		default:
			break;
		}
	}
}

/** @function : ParsingBuffFactor
	@brief : 버프 발생 요소를 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingBuffFactor( KLuaManager& luaManager_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingBuffFactor( KLuaManager& luaManager_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_TEMPLET_ID", m_pDataBuffFactor->m_BuffIdentity.m_eBuffTempletID, 
		BUFF_TEMPLET_ID, BTI_NONE, return DISPLAY_ERROR( L"BUFF_TEMPLET_ID" ); );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	ParsingTableForWhile( luaManager_, "RATE", m_pDataBuffFactor->m_vecRate, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
	LUA_GET_VALUE_RETURN( luaManager_, "RATE", m_pDataBuffFactor->m_fRate, 0.0f, return DISPLAY_ERROR( L"RATE" ); );
#endif //UPGRADE_SKILL_SYSTEM_2013

	LUA_GET_VALUE_RETURN( luaManager_, "IGNORE_REGIST_FOR_RATE", m_pDataBuffFactor->m_bIgnoreRegistForRate, 
		false, return DISPLAY_ERROR( L"IGNORE_REGIST_FOR_RATE" ); );
	
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( !ParsingBehavior( luaManager_, bIsSkill ) )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( !ParsingBehavior( luaManager_ ) )
#endif //UPGRADE_SKILL_SYSTEM_2013
		return DISPLAY_ERROR( L"BEHAVIOR FACTOR" );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( !ParsingFinalizer( luaManager_, bIsSkill ) )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( !ParsingFinalizer( luaManager_ ) )
#endif //UPGRADE_SKILL_SYSTEM_2013
		return DISPLAY_ERROR( L"FINALIZER FACTOR" );

	return true;
}

/** @function : BehaviorParsing
	@brief : 버프 발생 요소 중 행동 부분을 루아로 부터 읽어 들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingBehavior( KLuaManager& luaManager_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingBehavior( KLuaManager& luaManager_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( "BEHAVIOR" ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			return ParsingCombinationBehavior( luaManager_, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
			return ParsingCombinationBehavior( luaManager_ );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

	return false;
}

/** @function : ParsingFinalizer
	@brief : 버프 발생 요소 중 종료 부분을 루아로 부터 읽어 들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingFinalizer( KLuaManager& luaManager_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingFinalizer( KLuaManager& luaManager_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( "FINALIZER" ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			return ParsingCombinationFinalizer( luaManager_, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
			return ParsingCombinationFinalizer( luaManager_ );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

	return false;
}

/** @function : ApplyExternalFactorToFinalizer
	@brief : 특정 FinalizerFactor에 외부요소를 적용하는 함수(적용 방법은 각 종료 마다 다름)
	@param : 외부변경요소를 적용할 종료타입(eType_), 외부적용요소(fValue_)
*/
void CX2BuffFactor::ApplyExternalFactorToFinalizer( const BUFF_FINALIZER_TYPE eType_, const float fValue_ )
{
	if ( NULL != m_pDataBuffFactor )
	{
		BOOST_FOREACH( KBuffFinalizerFactor& kFactor, m_pDataBuffFactor->m_vecBuffFinalizerFactors )
		{
			switch ( kFactor.m_uiType )
			{
			case BFT_TIME:
				ApplyExternalFactorToTimeFinalizer( kFactor, fValue_ );
				break;

			default:
				break;
			}
		}
	}
}

/** @function : SetGameUnitBuffFactor
	@brief : 게임유닛을 참조해야하는 버프요소 셋팅
	@param : 버프요소를 발생 시키는 유닛(pGameUnit_)
*/
#ifdef ADD_LIVE_CREATOR_FINALIZER
void CX2BuffFactor::SetGameUnitBuffFactor( CX2GameUnit* pGameUnit_, UidType iCreatorUID /*= -1*/, bool bIsUserUID /*= true*/ )
#else  ADD_LIVE_CREATOR_FINALIZER
void CX2BuffFactor::SetGameUnitBuffFactor( CX2GameUnit* pGameUnit_ )
#endif ADD_LIVE_CREATOR_FINALIZER
{
	if ( NULL != pGameUnit_ )
	{
#ifdef ADD_LIVE_CREATOR_FINALIZER		/// 버프 시전자의 UID가 인자로 들어왔다면, 저장
		if( -1 < iCreatorUID )
		{
			SetUidGameUnit( iCreatorUID );
			SetIsUserUID( bIsUserUID );
		}
		else
#endif ADD_LIVE_CREATOR_FINALIZER
			SetUidGameUnit( pGameUnit_->GetUnitUID() );

		BOOST_FOREACH( KBuffBehaviorFactor& factor, m_pDataBuffFactor->m_vecBuffBehaviorFactors )
		{
			SetGameUnitBehavior( factor, pGameUnit_ );
		}

		BOOST_FOREACH( KBuffFinalizerFactor& factor, m_pDataBuffFactor->m_vecBuffFinalizerFactors )
		{
			SetGameUnitFinalizer( factor, pGameUnit_ );
		}
	}
}

/** @function : SetGameUnitBehavior
	@brief : 각 행동요소 마다 GameUnit으로 부터 셋팅 되야할 정보를 셋팅
	@param : 행동요소(factor_), 시전자 게임유닛(pGameUnit_)
*/
void CX2BuffFactor::SetGameUnitBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ )
{
	switch ( static_cast<BUFF_BEHAVIOR_TYPE>( factor_.m_uiType ) )
	{
	case BBT_CHANGE_NOW_HP_PER_SECOND:	/// 초당 HP 감소의 경우
		SetGameUnitChangeNowHpPerSecondBehavior( factor_, pGameUnit_ );
		break;

	case BBT_CHANGE_MAX_HP:
	case BBT_CHANGE_PHYSIC_ATTACK:
	case BBT_CHANGE_MAGIC_ATTACK:
	case BBT_CHANGE_PHYSIC_DEFENCE:
	case BBT_CHANGE_MAGIC_DEFENCE:
	case BBT_CHANGE_ANIM_SPEED:
	case BBT_CHANGE_WALK_RUN_SPEED:
	case BBT_CHANGE_JUMP_POWER:
	case BBT_CHANGE_ACCURACY:
	case BBT_CHANGE_AVOIDANCE:
	case BBT_CHANGE_CRITICAL_RATE:
	case BBT_CHANGE_CRITICAL_ATTACK:
	case BBT_CHANGE_ANTI_CRITICAL_RATE:
	case BBT_CHANGE_CRITICAL_DEFENCE:
	case BBT_CHANGE_ADDITIONAL_ATTACK:
	case BBT_CHANGE_ADDITIONAL_DEFENCE:
	case BBT_CHANGE_ATTRIBUTE_BLAZE_DEFENCE:
	case BBT_CHANGE_ATTRIBUTE_WATER_DEFENCE:
	case BBT_CHANGE_ATTRIBUTE_NATURE_DEFENCE:
	case BBT_CHANGE_ATTRIBUTE_WIND_DEFENCE:
	case BBT_CHANGE_ATTRIBUTE_LIGHT_DEFENCE:
	case BBT_CHANGE_ATTRIBUTE_DARK_DEFENCE:
	case BBT_CHANGE_EVERY_ATTRIBUTE_DEFENCE:
	case BBT_DRAIN_HP_NORMAL_ATTACK:
	case BBT_CHANGE_MAX_MP:
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case BBT_CHANGE_HYPER_CHARGE_SPEED:
#endif //SERV_ADD_LUNATIC_PSYKER
		SetGameUnitChangeStatBehavior( factor_, pGameUnit_ );
		break;

	case BBT_CHANGE_HYPER_MODE:
		SetGameUnitChangeHyperModeBehavior( factor_, pGameUnit_ );
		break;

	default:
		break;
	}	
}

/** @function : SetGameUnitFinalizer
	@brief : 각 종료요소 마다 GameUnit으로 부터 셋팅 되야할 정보를 셋팅
	@param : 종료요소(factor_), 시전자 게임유닛(pGameUnit_)
*/
void CX2BuffFactor::SetGameUnitFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ )
{
	switch ( static_cast<BUFF_FINALIZER_TYPE>( factor_.m_uiType ) )
	{
	case BFT_TIME:
		{
			SetGameUnitTimeFinalizer( factor_, pGameUnit_ );
		} break;
	case BFT_HIT_COUNT:
		{
			SetGameUnitHitCountFinalizer( factor_, pGameUnit_ );
		} break;

	case BFT_HITTED_COUNT:
		{
			SetGameUnitHittedCountFinalizer( factor_, pGameUnit_ );
		} break;

	case BFT_DAMAGE_VALUE:
		{
			SetGameUnitDamageValueFinalizer( factor_, pGameUnit_ );
		} break;

	default:
		break;
	}	
}

/** @function : ParsingChangeNowHpPerSecondBehavior
	@brief : 행동 부분 중 초당 HP 를 변경 시키는 요소에 대하여 읽어들임
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)	
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingChangeNowHpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingChangeNowHpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableNameUTF8_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		/// 0. 수치에 의한 변경인지 %에 의한 변경인지를 판단
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, 
			BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		switch ( eChangeType )
		{
		case BCT_FIX_VALUE:
			{
				/// 1. 수치를 읽어들임
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "FIX_VALUE", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fFixValue = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "FIX_VALUE", fFixValue, 0.0f, return DISPLAY_ERROR( L"FIX_VALUE" ) );
				BuffBehaviorFactor.m_vecValues.push_back( fFixValue );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
			break;

		case BCT_RELATION_VALUE:
			{
				/// 1. 공격력에 곱하여질 배율을 읽어들임(수치, % 모두 배율이 필요함)
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "MULTIPLIER", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fMultiplier = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "MULTIPLIER", fMultiplier, 0.0f, return DISPLAY_ERROR( L"MULTIPLIER" ) );
				BuffBehaviorFactor.m_vecValues.push_back( fMultiplier );
#endif //UPGRADE_SKILL_SYSTEM_2013

				/// 2. 물리, 마법, 혼합 평균 중 어떤 방식을 적용 할 것인지
				float fRelationType = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "BUFF_RELATION_TYPE", fRelationType, 0.0f, return DISPLAY_ERROR( L"BUFF_RELATION_TYPE" ) );
				BuffBehaviorFactor.m_vecValues.push_back( fRelationType );

				/// 3. 위의 타입과 관련한 공격력 (공간만 확보)
				BuffBehaviorFactor.m_vecValues.push_back( 0.0f );
			} break;

		case BCT_PERCENT:
			{
				/// 0. % 를 읽어들임
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "PERCENT", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fPercent = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "PERCENT", fPercent, 0.0f, return DISPLAY_ERROR( L"PERCENT" ) );
				BuffBehaviorFactor.m_vecValues.push_back( fPercent );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		default:
			break;
		}
							
		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

/** @function : SetGameUnitChangeNowHpPerSecondBehavior
	@brief : GameUnit으로 부터 초당 HP가 변경되는 행동에서 필요한 정보들을 얻어옴
	@param : 행동요소(factor_), 시전자 게임유닛(pGameUnit_)
*/
void CX2BuffFactor::SetGameUnitChangeNowHpPerSecondBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ )
{
	BUFF_CHANGE_TYPE eChangeType 
		= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, factor_[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_CHANGE_TYPE] );
	
	if ( BCT_RELATION_VALUE == eChangeType )	/// 수치에 의한 변경인 경우에만 실행
	{
		/// 적용할 버프의 데미지 타입을 얻어옴
		/// Percent 데미지의 경우 공격력을 얻어올 필요 없으나, Vector에 담기는 순서가 변경될 여지가 있어서
		/// 모두 얻어오도록 함
		
		BUFF_RELATION_TYPE eRelationType 
			= STATIC_CAST_FLOAT_TO_ENUM( BUFF_RELATION_TYPE, factor_[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_RELATION_TYPE] );

		/// 3. 위의 타입과 관련한 공격력
		const CX2Stat::Stat& kStat = pGameUnit_->GetStat().GetStat();	/// 시전자의 스탯
		if ( CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_ATTACK_VALUE < factor_.m_vecValues.size() )
		{
			switch ( eRelationType )
			{
			case BRT_PHYSIC_ATTACK:		/// 물리공격력으로 초당데미지 적용
				{
					factor_.m_vecValues[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_ATTACK_VALUE] = kStat.m_fAtkPhysic;
				} break;

			case BRT_MAGIC_ATTACK:		/// 마법공격력으로 초당데미지 적용
				{
					factor_.m_vecValues[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_ATTACK_VALUE] = kStat.m_fAtkMagic;
				} break;

			case BRT_MIX_ATTACK:		/// 물리+마법 공격력으로 초당 데미지 적용(1/2 함)
				{
					factor_.m_vecValues[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_ATTACK_VALUE] = (kStat.m_fAtkPhysic + kStat.m_fAtkMagic) * 0.5f;
				} break;

			default:
				DISPLAY_ERROR( L"BUFF_RELATION_TYPE" );
				break;
			}
		}
		else
		{
			DISPLAY_ERROR( L"BUFF_RELATION_TYPE" );
		}
	}
}

/** @function : TimeFinalizerParsing
	@brief : 종료 부분 중 시간에 의해서 종료되는 요소를 읽어들임
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingTimeFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingTimeFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffFinalizerFactor BuffFinalizerFactor( eFinalizerType_ );

		/// 0. 종료시간 타입 지정
		BUFF_DURATION_TIME_TYPE eDurationTimeType = BDTT_NORMAL_TIME;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_DURATION_TIME_TYPE", eDurationTimeType,
			BUFF_DURATION_TIME_TYPE, BDTT_NORMAL_TIME, return DISPLAY_ERROR( L"BUFF_DURATION_TIME_TYPE" ) );
		BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eDurationTimeType ) );

		switch ( eDurationTimeType )
		{
		case BDTT_NORMAL_TIME:
			{
				/// 1. 종료시간 (NORMAL인 경우 기입한 시간)
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "NORMAL_TIME", BuffFinalizerFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fTime = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "NORMAL_TIME", fTime, 0.0f, return DISPLAY_ERROR( L"NORMAL_TIME" ) );					
				BuffFinalizerFactor.m_vecValues.push_back( fTime );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		case BDTT_RANDOM_TIME:	/// 종료시간 타입이 랜덤인 경우
			{
				// MIN, MAX 값을 정함
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableMinMaxForWhile( luaManager_, "RANDOM_TIME", BuffFinalizerFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fMinTime = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "MIN_TIME", fMinTime, 0.0f, return DISPLAY_ERROR( L"MIN_TIME" ) );
				float fMaxTime = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "MAX_TIME", fMaxTime, 0.0f, return DISPLAY_ERROR( L"MAX_TIME" ) );

				if ( !(fMinTime < fMaxTime) )
					return DISPLAY_ERROR( L"MIN, MAX TIME invaild" );

				/// 1. MinTime 지정
				BuffFinalizerFactor.m_vecValues.push_back( fMinTime );
				/// 2. MaxTime 지정
				BuffFinalizerFactor.m_vecValues.push_back( fMaxTime );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		case BDTT_RELATION_HYPER_MODE_COUNT:	/// 각성구슬개수를 참조한 시간
			{
				/// 강제 각성 등의 버프 경우 시간입력
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "FORCE_TIME", BuffFinalizerFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fTime = 0.0f;
				LUA_GET_VALUE( luaManager_, "FORCE_TIME", fTime, 0.0f );
				if ( 0.0f < fTime )
					BuffFinalizerFactor.m_vecValues.push_back( fTime );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		default:
			return false;
			break;
		}
		
		m_pDataBuffFactor->m_vecBuffFinalizerFactors.push_back( BuffFinalizerFactor );		
		return true;
	}
	else
		return false;
}

/** @function : SetGameUnitTimeFinalizer
	@brief : 타격 횟수에 의한 종료요소에서 필요로 하는 시전자의 정보를 셋팅
	@param : 종료요소(factor_), 시전자(pGameUnit)
*/
void CX2BuffFactor::SetGameUnitTimeFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ )
{
	BUFF_DURATION_TIME_TYPE eDurationTimeType 
		= STATIC_CAST_FLOAT_TO_ENUM( BUFF_DURATION_TIME_TYPE, factor_[CX2BuffTimeFinalizerTemplet::FO_DURATION_BUFF_TIME_TYPE] );

	switch ( eDurationTimeType )
	{
	case BDTT_RELATION_HYPER_MODE_COUNT:
		{
			if ( factor_.m_vecValues.size() > CX2BuffTimeFinalizerTemplet::FO_FORCE_TIME )
			{
				float& fHyperModeTime = factor_.m_vecValues.back();
				pGameUnit_->SetForceHyperModeTime( fHyperModeTime );
				fHyperModeTime = pGameUnit_->GetHyperModeTime();
			}
			else
			{
				const float fHyperModeTime = pGameUnit_->GetHyperModeTime();
				factor_.m_vecValues.push_back( fHyperModeTime );
			}
		} break;

	default:
		break;
	}
}

/** @function : ApplyExternalFactorToTimeFinalizer
	@brief : 시간에 의해 종료조건에 외부요소를 적용하는 함수
	@param : 변경될 종료조건(factor_), 적용할 외부요소(fValue_)
*/
void CX2BuffFactor::ApplyExternalFactorToTimeFinalizer( KBuffFinalizerFactor& factor_, const float fValue_ )
{
	if ( static_cast<UINT>( CX2BuffTimeFinalizerTemplet::FO_NORMAL_TIME ) < factor_.m_vecValues.size() )
	{
		float& fNormalTime = factor_.m_vecValues[CX2BuffTimeFinalizerTemplet::FO_NORMAL_TIME];
		fNormalTime += fValue_;
	}
}

/** @function : ParsingHitCountFinalizer
	@brief : 종료 부분 중 타격카운트 의해서 종료되는 요소를 읽어들임
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingHitCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingHitCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffFinalizerFactor BuffFinalizerFactor( eFinalizerType_ );

		/// 0. 버프 또는 디버프 개수를 배율로 사용할 것인지 여부
		BUFF_USE_COUNT_TYPE eUseBuffCountType = BUCT_NOT_USE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_USE_COUNT_TYPE", eUseBuffCountType, 
			BUFF_USE_COUNT_TYPE, BUCT_NOT_USE, return DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE" ) );
		BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eUseBuffCountType ) );
		
		/// 1. 지정한 타격 횟수
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		ParsingTableForWhile( luaManager_, "COUNT", BuffFinalizerFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
		float fCount = 0.0f;
		LUA_GET_VALUE_RETURN( luaManager_, "COUNT", fCount, 0.0f, return DISPLAY_ERROR( L"COUNT" ) );
		BuffFinalizerFactor.m_vecValues.push_back( fCount );
#endif //UPGRADE_SKILL_SYSTEM_2013

		/// 2. 지정한 타격 횟수에 배율로 해야 할 것에 대한 공간 확보
		BuffFinalizerFactor.m_vecValues.push_back( 0.0f );

		m_pDataBuffFactor->m_vecBuffFinalizerFactors.push_back( BuffFinalizerFactor );
		return true;
	}
	else
		return false;
}

/** @function : SetGameUnitHitCountFinalizer
	@brief : 타격 횟수에 의한 종료요소에서 필요로 하는 시전자의 정보를 셋팅
	@param : 종료요소(factor_), 시전자(pGameUnit)
*/
void CX2BuffFactor::SetGameUnitHitCountFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ )
{
	/// 2. 버프 또는 디버프 개수 와 지정한 피격횟수를 고려한 최종 결과 값
	BUFF_USE_COUNT_TYPE eUseBuffCountType = 
		STATIC_CAST_FLOAT_TO_ENUM( BUFF_USE_COUNT_TYPE, factor_[CX2BuffHitCountFinalizerTemplet::FO_USE_BUFF_COUNT_TYPE] );
	
	if ( CX2BuffHitCountFinalizerTemplet::FO_RESULT_COUNT < factor_.m_vecValues.size() )
	{
		switch ( eUseBuffCountType )
		{
		case BUCT_NOT_USE:
			{
				/// 버프/디버프 개수를 사용하지 않는 경우 지정된 카운트를 FO_RESULT_COUNT로 복사해준다.
				/// FO_RESULT_COUNT를 사용하는 이유는 서버로 부터 FACTOR를 받았을 때
				/// 버프/디버프 사용 타입에 관계 없이 그냥 사용할 수 있도록 해주기 위해서이다.
				factor_.m_vecValues[CX2BuffHitCountFinalizerTemplet::FO_RESULT_COUNT] =
					factor_[CX2BuffHitCountFinalizerTemplet::FO_COUNT];
			} break;

		case BUCT_MY_BUFF_COUNT:
			{
				factor_.m_vecValues[CX2BuffHitCountFinalizerTemplet::FO_RESULT_COUNT] =
					factor_[CX2BuffHitCountFinalizerTemplet::FO_COUNT] * pGameUnit_->GetAppliedBuffCount( false );
			} break;

		case BUCT_MY_DEBUFF_COUNT:
			{
				factor_.m_vecValues[CX2BuffHitCountFinalizerTemplet::FO_RESULT_COUNT] = 
					factor_[CX2BuffHitCountFinalizerTemplet::FO_COUNT] * pGameUnit_->GetAppliedBuffCount( true );
			} break;

		default:
			break;
		}
	}
}

/** @function : ParsingHittedCountFinalizer
	@brief : 종료 부분 중 피격카운트 의해서 종료되는 요소를 읽어들임
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingHittedCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingHittedCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffFinalizerFactor BuffFinalizerFactor( eFinalizerType_ );

		/// 0. 버프 또는 디버프 개수를 배율로 사용할 것인지 여부
		BUFF_USE_COUNT_TYPE eUseBuffCountType = BUCT_NOT_USE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_USE_COUNT_TYPE", eUseBuffCountType, 
			BUFF_USE_COUNT_TYPE, BUCT_NOT_USE, return DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE" ) );
		BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eUseBuffCountType ) );

		/// 1. 지정한 피격 횟수
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		ParsingTableForWhile( luaManager_, "COUNT", BuffFinalizerFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
		float fCount = 0.0f;
		LUA_GET_VALUE_RETURN( luaManager_, "COUNT", fCount, 0.0f, return DISPLAY_ERROR( L"COUNT" ) );
		BuffFinalizerFactor.m_vecValues.push_back( fCount );
#endif //UPGRADE_SKILL_SYSTEM_2013

		/// 2. 버프 또는 디버프 개수 와 지정한 피격횟수를 고려한 최종 결과 값 공간 확보
		BuffFinalizerFactor.m_vecValues.push_back( 0.0f );

		m_pDataBuffFactor->m_vecBuffFinalizerFactors.push_back( BuffFinalizerFactor );
		return true;
	}
	else
		return false;
}

/** @function : SetGameUnitHittedCountFinalizer
	@brief : 피격 횟수에 의한 종료요소에서 필요로 하는 시전자의 정보를 셋팅
	@param : 종료요소(factor_), 시전자(pGameUnit_)
*/
void CX2BuffFactor::SetGameUnitHittedCountFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ )
{
	/// 2. 버프 또는 디버프 개수 와 지정한 피격횟수를 고려한 최종 결과 값
	BUFF_USE_COUNT_TYPE eUseBuffCountType 
		= STATIC_CAST_FLOAT_TO_ENUM( BUFF_USE_COUNT_TYPE, factor_[CX2BuffHittedCountFinalizerTemplet::FO_USE_BUFF_COUNT_TYPE] );

	if ( CX2BuffHittedCountFinalizerTemplet::FO_RESULT_COUNT < factor_.m_vecValues.size() )
	{
		switch ( eUseBuffCountType )
		{
		case BUCT_NOT_USE:
			{
				/// 버프/디버프 개수를 사용하지 않는 경우 지정된 카운트를 FO_RESULT_COUNT로 복사해준다.
				/// FO_RESULT_COUNT를 사용하는 이유는 서버로 부터 FACTOR를 받았을 때
				/// 버프/디버프 사용 타입에 관계 없이 그냥 사용할 수 있도록 해주기 위해서이다.
				factor_.m_vecValues[CX2BuffHittedCountFinalizerTemplet::FO_RESULT_COUNT] =
					factor_[CX2BuffHittedCountFinalizerTemplet::FO_COUNT];
			} break;

		case BUCT_MY_BUFF_COUNT:
			{
				factor_.m_vecValues[CX2BuffHittedCountFinalizerTemplet::FO_RESULT_COUNT] =
					factor_[CX2BuffHittedCountFinalizerTemplet::FO_COUNT] * pGameUnit_->GetAppliedBuffCount( false );
			} break;

		case BUCT_MY_DEBUFF_COUNT:
			{
				factor_.m_vecValues[CX2BuffHittedCountFinalizerTemplet::FO_RESULT_COUNT] =
					factor_[CX2BuffHittedCountFinalizerTemplet::FO_COUNT] * pGameUnit_->GetAppliedBuffCount( true );
			} break;

		default:
			break;
		}
	}
}

#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
/** @function : ParsingStateChangeFinalizer
	@brief : 스테이트가 변경되면 종료되는 요소를 읽어들임
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
bool CX2BuffFactor::ParsingStateChangeFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ )
{
	if ( luaManager_.BeginTable( pwszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

			KBuffFinalizerFactor BuffFinalizerFactor( BFT_STATE_CHANGE );
		m_pDataBuffFactor->m_vecBuffFinalizerFactors.push_back( BuffFinalizerFactor );
		return true;
	}
	else
		return false;
}
#endif //UPGRADE_SKILL_SYSTEM_2013

/** @function : ParsingDamageValueFinalizer
	@brief : 일정 데미지를 받으면 종료되는 요소를 읽어들임
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingDamageValueFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingDamageValueFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffFinalizerFactor BuffFinalizerFactor( eFinalizerType_ );

		/// 치환, 연동, 고정, % 중 선택
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, BUFF_CHANGE_TYPE,
			BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		switch ( eChangeType )
		{
		case BCT_FIX_VALUE:	/// 고정 수치 입력값
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "FIX_VALUE", BuffFinalizerFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fFixValue = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "FIX_VALUE", fFixValue, 0.0f, return DISPLAY_ERROR( L"FIX_VALUE" ) );
				if ( 0.0f > fFixValue )
				{
					return DISPLAY_ERROR( L"FIX_VALUE MUST BE POSITIVE" );
				}
				else
					BuffFinalizerFactor.m_vecValues.push_back( fFixValue );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		case BCT_RELATION_VALUE:	/// 연동인 경우 소스타입(물공, 최대HP 등), 배율을 통해 계산
			{
				float fRelationValue = 0.0f;	/// 연동수치는 SetGameUnit 함수를 통해 얻어옴
				BuffFinalizerFactor.m_vecValues.push_back( fRelationValue );

				BUFF_RELATION_TYPE eRelationType = BRT_END;
				LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_RELATION_TYPE", eRelationType, BUFF_RELATION_TYPE, 
					BRT_END, return DISPLAY_ERROR( L"BUFF_RELATION_TYPE" ) );
				BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eRelationType ) );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "MULTIPLIER", BuffFinalizerFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fMultiplier = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "MULTIPLIER", fMultiplier, 0.0f, return DISPLAY_ERROR( L"MULTIPLIER" ) );

				if ( 0.0f > fMultiplier )
				{
					return DISPLAY_ERROR( L"MULTIPLIER MUST BE POSITIVE" );
				}
				else
					BuffFinalizerFactor.m_vecValues.push_back( fMultiplier );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		default:
			return DISPLAY_ERROR( L"can not select this type" );
			break;
		}	

		m_pDataBuffFactor->m_vecBuffFinalizerFactors.push_back( BuffFinalizerFactor );
		return true;
	}
	else
		return false;
}

/** @function : SetGameUnitDamageValueFinalizer
	@brief : 데미지량을 정할 때 필요한 요소를 pGameUnit_으로 부터 얻어오기 위한 함수
	@param : 버프요소(factor_), 시전자유닛(pGameUnit_)
*/
void CX2BuffFactor::SetGameUnitDamageValueFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ )
{
	const BUFF_CHANGE_TYPE eChangeType 
		= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, factor_[CX2BuffDamageValueFinalizerTemplet::FO_CHANGE_TYPE] );

	if ( BCT_RELATION_VALUE == eChangeType )	/// 연동수치 인 경우
	{
		const BUFF_RELATION_TYPE eRelationType 
			= STATIC_CAST_FLOAT_TO_ENUM( BUFF_RELATION_TYPE, factor_[CX2BuffDamageValueFinalizerTemplet::FO_RELATION_TYPE] );

		const UINT uiIndex = CX2BuffDamageValueFinalizerTemplet::FO_VALUE;
		if ( factor_.m_vecValues.size() > uiIndex )
			UpdateValueByRelationType( eRelationType, factor_.m_vecValues[uiIndex], pGameUnit_ );
	}
}

/** @function : ParsingPassHpFinalizer
	@brief : 기준 HP를 상향 또는 하향 돌파 하면 종료되는 요소를 읽어들임
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingPassHpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingPassHpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffFinalizerFactor BuffFinalizerFactor( eFinalizerType_ );

		/// 0. 돌파 타입(상향, 하향)
		bool bPassUp = false;	/// 상향 돌파 시 종료인가?
		LUA_GET_VALUE_RETURN( luaManager_, "PASS_UP", bPassUp, false, return DISPLAY_ERROR( L"PASS_UP" ) );
		BuffFinalizerFactor.m_vecValues.push_back( (bPassUp ? 1.0f : 0.0f) );

		/// 1. 변화 타입(% 인가 값인가?)
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, 
			BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		/// 2. 기준 입력 ( 1의 타입에 따라 % 또는 값으로 사용)
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		ParsingTableForWhile( luaManager_, "CRITERION", BuffFinalizerFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
		float fCriterion = 0.0f;
		LUA_GET_VALUE_RETURN( luaManager_, "CRITERION", fCriterion, 0.0f, return DISPLAY_ERROR( L"CRITERION" ) );
		BuffFinalizerFactor.m_vecValues.push_back( fCriterion );
#endif //UPGRADE_SKILL_SYSTEM_2013

		m_pDataBuffFactor->m_vecBuffFinalizerFactors.push_back( BuffFinalizerFactor );

		return true;
	}
	else
		return false;
}

/** @function : ParsingPassHpFinalizer
	@brief : 기준 HP를 상향 또는 하향 돌파 하면 종료되는 요소를 읽어들임
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingPassMpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingPassMpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffFinalizerFactor BuffFinalizerFactor( eFinalizerType_ );

		/// 0. 돌파 타입(상향, 하향)
		bool bPassUp = false;	/// 상향 돌파 시 종료인가?
		LUA_GET_VALUE_RETURN( luaManager_, "PASS_UP", bPassUp, false, return DISPLAY_ERROR( L"PASS_UP" ) );
		BuffFinalizerFactor.m_vecValues.push_back( (bPassUp ? 1.0f : 0.0f) );

		/// 1. 변화 타입(% 인가 값인가?)
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, 
			BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		/// 2. 기준 입력 ( 1의 타입에 따라 % 또는 값으로 사용)
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		ParsingTableForWhile( luaManager_, "CRITERION", BuffFinalizerFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
		float fCriterion = 0.0f;
		LUA_GET_VALUE_RETURN( luaManager_, "CRITERION", fCriterion, 0.0f, return DISPLAY_ERROR( L"CRITERION" ) );
		BuffFinalizerFactor.m_vecValues.push_back( fCriterion );
#endif //UPGRADE_SKILL_SYSTEM_2013

		m_pDataBuffFactor->m_vecBuffFinalizerFactors.push_back( BuffFinalizerFactor );

		return true;
	}
	else
		return false;
}

/** @function : ParsingPassMpFinalizer
	@brief : 내용이 없는 종료요소를 루아스크립트로 부터 파싱
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
bool CX2BuffFactor::ParsingEmptyFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffFinalizerFactor BuffFinalizerFactor( eFinalizerType_ );

		m_pDataBuffFactor->m_vecBuffFinalizerFactors.push_back( BuffFinalizerFactor );

		return true;
	}
	else
		return false;
}

/** @function : ParsingChangeNowMpPerSecondBehavior
	@brief : 초당 MP가 감소하는 행동요소를 루아스크립트로 부터 읽어들임
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingChangeNowMpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingChangeNowMpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableNameUTF8_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		/// 0. 수치에 의한 변경인지 %에 의한 변경인지를 판단
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, 
			BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		switch ( eChangeType )
		{
		default:
		case BCT_FIX_VALUE:
			{
				/// 1. 수치를 읽어들임
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "FIX_VALUE", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fValue = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "FIX_VALUE", fValue, 0.0f, return DISPLAY_ERROR( L"FIX_VALUE" ) );
				BuffBehaviorFactor.m_vecValues.push_back( fValue );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		case BCT_PERCENT:
			{
				/// 1. 배율을 읽어들임
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "PERCENT", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fPercent = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "PERCENT", fPercent, 0.0f, return DISPLAY_ERROR( L"PERCENT" ) );
				BuffBehaviorFactor.m_vecValues.push_back( fPercent );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;
		}
		
		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;;
}

/** @function : ParsingChangeStatBehavior
	@brief : 스탯을 변경시키는 행동요소를 루아스크립트로 부터 읽어들임
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingChangeStatBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingChangeStatBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		/// 치환, 연동, 고정, % 중 선택
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, BUFF_CHANGE_TYPE,
			BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		switch ( eChangeType )
		{
		case BCT_SWAP_VALUE:	/// 치환인 경우 수치만
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "SWAP_VALUE", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fSwapValue = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "SWAP_VALUE", fSwapValue, 0.0f, return DISPLAY_ERROR( L"SWAP_VALUE" ) );
				BuffBehaviorFactor.m_vecValues.push_back( fSwapValue );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		case BCT_FIX_VALUE:	/// 고정 수치 입력값
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "FIX_VALUE", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fValue = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "FIX_VALUE", fValue, 0.0f, return DISPLAY_ERROR( L"FIX_VALUE" ) );
				BuffBehaviorFactor.m_vecValues.push_back( fValue );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		case BCT_RELATION_VALUE:	/// 연동인 경우 소스타입(물공, 최대HP 등), 배율을 통해 계산
			{
				float fRelationValue = 0.0f;	/// 연동수치는 SetGameUnit 함수를 통해 얻어옴
				BuffBehaviorFactor.m_vecValues.push_back( fRelationValue );

				BUFF_RELATION_TYPE eRelationType = BRT_END;
				LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_RELATION_TYPE", eRelationType, BUFF_RELATION_TYPE, 
					BRT_END, return DISPLAY_ERROR( L"BUFF_RELATION_TYPE" ) );
				BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eRelationType ) );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "MULTIPLIER", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fMultiplier = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "MULTIPLIER", fMultiplier, 0.0f, return DISPLAY_ERROR( L"MULTIPLIER" ) );
				BuffBehaviorFactor.m_vecValues.push_back( fMultiplier );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		case BCT_PERCENT:	/// % 입력값
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "PERCENT", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fPercent = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "PERCENT", fPercent, 0.0f, return DISPLAY_ERROR( L"PERCENT" ) );
				if ( 0.0f > fPercent )	/// 양수만 가능
				{
					return DISPLAY_ERROR( L"Percent value must be positive" );
				}
				BuffBehaviorFactor.m_vecValues.push_back( fPercent );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		default:
			break;
		}	

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

/** @function : SetGameUnitChangeStatBehavior
	@brief : 버프를 시전한 게임유닛의 정보를 참고하여 행동요소 셋팅
	@param : 버프요소(factor_), 게임유닛(pGameUnit_)
*/
void CX2BuffFactor::SetGameUnitChangeStatBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ )
{
	const BUFF_CHANGE_TYPE eChangeType 
		= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, factor_[CX2BuffChangeStatBehaviorTemplet::FO_CHANGE_TYPE] );

	if ( BCT_RELATION_VALUE == eChangeType )	/// 연동수치 인 경우
	{
		const BUFF_RELATION_TYPE eRelationType 
			= STATIC_CAST_FLOAT_TO_ENUM( BUFF_RELATION_TYPE, factor_[CX2BuffChangeStatBehaviorTemplet::FO_RELATION_TYPE] );

		const UINT uiIndex = CX2BuffChangeStatBehaviorTemplet::FO_VALUE;
		if ( factor_.m_vecValues.size() > uiIndex )
			UpdateValueByRelationType( eRelationType, factor_.m_vecValues[uiIndex], pGameUnit_ );
	}
}

/** @function : ParsingCreateBuffFactorBehavior
	@brief : 초당 MP가 감소하는 행동요소를 루아스크립트로 부터 읽어들임
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
bool CX2BuffFactor::ParsingCreateBuffFactorBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		float fBuffFactorId = 0.0f;
		LUA_GET_VALUE_RETURN( luaManager_, "BUFF_FACTOR_ID", fBuffFactorId, 0.0f, return DISPLAY_ERROR( L"BUFF_FACTOR_ID" ) );
		BuffBehaviorFactor.m_vecValues.push_back( fBuffFactorId );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		float fSkillId = 0.f;
		LUA_GET_VALUE_RETURN( luaManager_, "SKILL_ID", fSkillId, 0.0f, return DISPLAY_ERROR( L"SKILL_ID" ) );
		BuffBehaviorFactor.m_vecValues.push_back( fSkillId );
#endif //UPGRADE_SKILL_SYSTEM_2013

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

/** @function : ParsingAbsorbEffectAttackBehavior
	@brief : 이펙트 공격을 흡수하여 MP를 증가시키는 행동요소를 루아스크립트로 부터 읽어들임
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingAbsorbEffectAttackBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingAbsorbEffectAttackBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		ParsingTableForWhile( luaManager_, "ABSORB_VALUE", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
		float fAbsorbValue = 0.0f;
		LUA_GET_VALUE_RETURN( luaManager_, "ABSORB_VALUE", fAbsorbValue, 0.0f,return DISPLAY_ERROR( L"ABSORB_VALUE" ) );
		BuffBehaviorFactor.m_vecValues.push_back( fAbsorbValue );
#endif //UPGRADE_SKILL_SYSTEM_2013

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

/** @function : ParsingChargeMpHitBehavior
	@brief : 타격시 MP 회수율을 증가시키는 행동요소를 루아스크립트로 부터 읽어들임
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingChargeMpHitBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingChargeMpHitBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		ParsingTableForWhile( luaManager_, "MULTIPLIER", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
		float fMultiplier = 1.0f;
		LUA_GET_VALUE_RETURN( luaManager_, "MULTIPLIER", fMultiplier, 1.0f, return DISPLAY_ERROR( L"MULTIPLIER" ) );
		BuffBehaviorFactor.m_vecValues.push_back( fMultiplier );
#endif //UPGRADE_SKILL_SYSTEM_2013

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return true;
}

/** @function : ParsingChargeMpHittedBehavior
	@brief : 피격시 MP 회수율을 증가시키는 행동요소를 루아스크립트로 부터 읽어들임
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingChargeMpHittedBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingChargeMpHittedBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		ParsingTableForWhile( luaManager_, "MULTIPLIER", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
		float fMultiplier = 1.0f;
		LUA_GET_VALUE_RETURN( luaManager_, "MULTIPLIER", fMultiplier, 1.0f, return DISPLAY_ERROR( L"MULTIPLIER" ) );
		BuffBehaviorFactor.m_vecValues.push_back( fMultiplier );
#endif //UPGRADE_SKILL_SYSTEM_2013

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

/** @function : ParsingGameUnitChangeHyperModeBehavior
	@brief : 각성 행동요소를 루아스크립트로 부터 읽어들임(종료요소는 따로 있음)
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
bool CX2BuffFactor::ParsingGameUnitChangeHyperModeBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		float fHyperTime = 0.0f;
		LUA_GET_VALUE( luaManager_, "HYPER_TIME", fHyperTime, 0.0f );
		
		/// 각성 시간이 지정되어 있으면
		if ( 0.0f < fHyperTime )
			BuffBehaviorFactor.m_vecValues.push_back( fHyperTime );

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

/** @function : SetGameUnitChangeHyperModeBehavior
	@brief : 게임유닛 정보를 참조하여 각성을 시키는 행동요소를 셋팅
	@param : 행동요소(factor_), 게임유닛(pGameUnit_)
*/
void CX2BuffFactor::SetGameUnitChangeHyperModeBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ )
{
	/// Parsing 할 때 지정된 시간이 없다면
	if ( factor_.m_vecValues.empty() )
	{
		const float fHyperModeTime = pGameUnit_->GetHyperModeTime();
		factor_.m_vecValues.push_back( fHyperModeTime );
	}

	const float fHyperModeCount = static_cast<float>( pGameUnit_->GetLastStateHyperModeCount() );
	factor_.m_vecValues.push_back( fHyperModeCount );	
}

/** @function : ParsingEmptyBehavior
	@brief : 내용이 없는 행동요소를 루아스크립트로 부터 파싱
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
bool CX2BuffFactor::ParsingEmptyBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );
		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

/** @function : ParsingCustomBehavior
	@brief : 내용이 없는 행동요소를 루아스크립트로 부터 파싱
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
bool CX2BuffFactor::ParsingCustomBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		/// 아래의 값들은 사용할 수도, 안 할수도 있음
		int iIndex = 1;
		float fValue = 0.0f;
		while ( luaManager_.GetValue( iIndex, fValue ) )
		{
			BuffBehaviorFactor.m_vecValues.push_back( fValue );
			++iIndex;
		}

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

/** @function : ParsingEffectSetWithDamageBehavior
	@brief : 같은 버프 템플릿에서도 이펙트셋을 사용하거나 사용하지 않고 싶은 경우를 Factor에서 셋팅할 수 있도록 함
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
bool CX2BuffFactor::ParsingEffectSetWithDamageBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		bool bNotUse = false;	/// 이펙트셋을 사용 안할 것인가?
		LUA_GET_VALUE( luaManager_, "NOT_USE", bNotUse, false );
		BuffBehaviorFactor.m_vecValues.push_back( (bNotUse ? 1.0f : 0.0f) );

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );

        luaManager_.EndTable();

		return true;
	}
	else
		return false;
}

/** @function : ParsingChangeAttackByTypeBehavior
	@brief : 공격시 AttackType에 따른 데미지 변경 효과를 Factor에서 셋팅할 수 있도록 함
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingChangeAttackByTypeBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingChangeAttackByTypeBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		/// 치환, 연동, 고정, % 중 선택
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, BUFF_CHANGE_TYPE,
			BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		switch ( eChangeType )
		{
		case BCT_PERCENT:	/// % 입력값
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "PERCENT", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fPercent = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "PERCENT", fPercent, 0.0f, return DISPLAY_ERROR( L"PERCENT" ) );
				if ( 0.0f > fPercent )	/// 양수만 가능
				{
					return DISPLAY_ERROR( L"Percent value must be positive" );
				}
				BuffBehaviorFactor.m_vecValues.push_back( fPercent );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		default:
			{
				DISPLAY_ERROR( L"BCT_PERCENT Only!!" );
			}
			break;
		}

		/// 공격 타입 AT_NORMAL, AT_SPECIAL 중 선택
		CX2DamageManager::ATTACK_TYPE eAttackType = CX2DamageManager::AT_NORMAL;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "ATTACK_TYPE", eAttackType, CX2DamageManager::ATTACK_TYPE,
			CX2DamageManager::AT_NORMAL, return DISPLAY_ERROR( L"ATTACK_TYPE" ) );
		BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eAttackType ) );

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

/** @function : ParsingChangeAttackByTypeBehavior
	@brief : 소켓 속성 공격 발동 확률을 변경 효과를 Factor에서 셋팅할 수 있도록 함
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingChangeEnchantAttackRateBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingChangeEnchantAttackRateBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		/// 치환, 연동, 고정, % 중 선택
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, BUFF_CHANGE_TYPE,
			BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		switch ( eChangeType )
		{
		case BCT_PERCENT:	/// % 입력값
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "PERCENT", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fPercent = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "PERCENT", fPercent, 0.0f, return DISPLAY_ERROR( L"PERCENT" ) );
				if ( 0.0f > fPercent )	/// 양수만 가능
				{
					return DISPLAY_ERROR( L"Percent value must be positive" );
				}
				BuffBehaviorFactor.m_vecValues.push_back( fPercent );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// BCT_FIX_VALUE 값을 파싱 할 수 있도록 추가 
		case BCT_FIX_VALUE :
			{
				ParsingTableForWhile( luaManager_, "FIX_VALUE", BuffBehaviorFactor.m_vecValues, bIsSkill );
			} break;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

		default:
			{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				DISPLAY_ERROR( L"BCT_PERCENT OR BCT_FIX_VALUE Only!!" );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				DISPLAY_ERROR( L"BCT_PERCENT Only!!" );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			}
			break;
		}	

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

/** @function : ParsingDoubleAttackByBuffBehavior
	@brief : 더블 어택 효과를 Factor에서 셋팅할 수 있도록 함
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingDoubleAttackByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingDoubleAttackByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		/// 치환, 연동, 고정, % 중 선택
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, BUFF_CHANGE_TYPE,
			BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		switch ( eChangeType )
		{
		case BCT_PERCENT:	/// % 입력값
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "PERCENT", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fPercent = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "PERCENT", fPercent, 0.0f, return DISPLAY_ERROR( L"PERCENT" ) );
				if ( 0.0f > fPercent )	/// 양수만 가능
				{
					return DISPLAY_ERROR( L"Percent value must be positive" );
				}
				BuffBehaviorFactor.m_vecValues.push_back( fPercent );
#endif //UPGRADE_SKILL_SYSTEM_2013
			} break;

		default:
			{
				DISPLAY_ERROR( L"BCT_PERCENT Only!!" );
			}
			break;
		}	

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

/** @function : ParsingAddSkillLevelByBuffBehavior
	@brief : add 시킬 스킬 레벨을 를 Factor에서 셋팅할 수 있도록 함
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
bool CX2BuffFactor::ParsingAddSkillLevelByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		int	iAddSkillLevel = 0;
		LUA_GET_VALUE_RETURN( luaManager_, "ADD_SKILL_LEVEL", iAddSkillLevel, 0, return DISPLAY_ERROR( L"ADD_SKILL_LEVEL" ) );
		BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( iAddSkillLevel ) );

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환

bool CX2BuffFactor::ParsingReflectMagicBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

			KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		/// 치환, 연동, 고정, % 중 선택
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, BUFF_CHANGE_TYPE,
			BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		switch ( eChangeType )
		{
		case BCT_PERCENT:	/// % 입력값
			{
				ParsingTableForWhile( luaManager_, "PERCENT", BuffBehaviorFactor.m_vecValues, bIsSkill );
			} break;

		default:
			{
				DISPLAY_ERROR( L"BCT_PERCENT Only!!" );
			}
			break;
		}

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경

/** @function : ParsingResetSkillCoolTimeByBuffBehavior
	@brief : 초기화 시킬 스킬 쿨타임 값을 Factor에서 셋팅할 수 있도록 함
	@param : 읽어들일 루아스크립트의 매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
bool CX2BuffFactor::ParsingResetSkillCoolTimeByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2BuffFactor::ParsingResetSkillCoolTimeByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		/// 치환, 연동, 고정, % 중 선택
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, BUFF_CHANGE_TYPE,
			BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		switch ( eChangeType )
		{
		case BCT_FIX_VALUE:
			{
				/// 1. 수치를 읽어들임
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "FIX_VALUE", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fFixValue = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "FIX_VALUE", fFixValue, 0.0f, return DISPLAY_ERROR( L"FIX_VALUE" ) );
				BuffBehaviorFactor.m_vecValues.push_back( fFixValue );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
			break;

		case BCT_RELATION_VALUE:
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				ParsingTableForWhile( luaManager_, "MULTIPLIER", BuffBehaviorFactor.m_vecValues, bIsSkill );
#else //UPGRADE_SKILL_SYSTEM_2013
				float fMultiplier = 0.0f;
				LUA_GET_VALUE_RETURN( luaManager_, "MULTIPLIER", fMultiplier, 0.0f, return DISPLAY_ERROR( L"MULTIPLIER" ) );
				BuffBehaviorFactor.m_vecValues.push_back( fMultiplier );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
			break;

#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가

		case BCT_PERCENT:
			{
				ParsingTableForWhile( luaManager_, "PERCENT", BuffBehaviorFactor.m_vecValues, bIsSkill );
			}
			break;



#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가

		default:
			{
#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME				// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
				return DISPLAY_ERROR( L"BCT_FIX_VALUE or BCT_RELATION_VALUE or BCT_PERCENT" );
#else  // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME
				return DISPLAY_ERROR( L"BCT_FIX_VALUE or BCT_RELATION_VALUE" );
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가			
			}
			break;
		}

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}

#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
bool CX2BuffFactor::ParsingChangConsumeMpRateBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill/* = true*/ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType_ );

		/// 치환, 연동, 고정, % 중 선택
		BUFF_CHANGE_TYPE eChangeType = BCT_SWAP_VALUE;
		LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", eChangeType, BUFF_CHANGE_TYPE,
			BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
		BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

		float fSkillId = 0.f;
		LUA_GET_VALUE_RETURN( luaManager_, "SKILL_ID", fSkillId, 0.0f, return DISPLAY_ERROR( L"SKILL_ID" ) );
		BuffBehaviorFactor.m_vecValues.push_back( fSkillId );

		switch ( eChangeType )
		{
		case BCT_PERCENT:	/// % 입력값
			{
				ParsingTableForWhile( luaManager_, "PERCENT", BuffBehaviorFactor.m_vecValues, bIsSkill );
			} break;

		default:
			{
				DISPLAY_ERROR( L"BCT_PERCENT Only!!" );
			}
			break;
		}

		m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( BuffBehaviorFactor );
		return true;
	}
	else
		return false;
}
#endif //FIX_SKILL_BALANCE_AISHA_LENA

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
void CX2BuffFactor::ParsingTableForWhile( IN KLuaManager& luaManager_, IN const char* pszTableName_,
	OUT vector<float>& vecValues_, IN bool bUpdateCount_/* = true*/ )
{
	if ( true == luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		int iIndex = 1;
		int iValueSize = 1;
		float fValue = 0.f;

		if ( bUpdateCount_ )
		{
			vecValues_.push_back( static_cast<float>( iIndex ) );
			iValueSize = vecValues_.size() - 1;
		}

		while( luaManager_.GetValue( iIndex, fValue ) == true )
		{
			vecValues_.push_back( fValue );
			++iIndex;
		}

		if ( bUpdateCount_ && iIndex > 1 )
			vecValues_.at( iValueSize ) = static_cast<float>( iIndex - 1 );
	}
	else
		DISPLAY_ERROR( pszTableName_ )
}

void CX2BuffFactor::ParsingTableMinMaxForWhile( IN KLuaManager& luaManager_, IN const char* pszTableName_, OUT vector<float>& vecValues_, IN bool bUpdateCount_/* = true*/ )
{
	if ( true == luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		int iIndex = 1;
		int iValueSize = 1;
		float fValue = 0.f;

		if ( bUpdateCount_ )
		{
			vecValues_.push_back( static_cast<float>( iIndex ) );
			iValueSize = vecValues_.size() - 1;
		}

		int iTableIndex = 1;	// 테이블 인덱스
		while ( true == luaManager_.BeginTable( iTableIndex ) )
		{
			while( luaManager_.GetValue( iIndex, fValue ) == true )
			{
				vecValues_.push_back( fValue );
				++iIndex;
			}

			luaManager_.EndTable();
			++iTableIndex;	// 테이블의 인덱스 증가
		}

		if ( bUpdateCount_ && iIndex > 1 )
			vecValues_.at( iValueSize ) = static_cast<float>( iIndex - 1 );
	}
	else
		DISPLAY_ERROR( pszTableName_ )
}

void BUFF_FACTOR::GetBehaviorFactorToLevel( IN const vector<KBuffBehaviorFactor>& vecInBehaviorFactorList ,
											  OUT vector<KBuffBehaviorFactor>& vecOutBehaviorFactorList, IN const UINT uiLevel_ )
{
	BOOST_FOREACH( const KBuffBehaviorFactor& factor, vecInBehaviorFactorList )
	{
		const BUFF_BEHAVIOR_TYPE eBehaviorType =static_cast<BUFF_BEHAVIOR_TYPE>( factor.m_uiType );
		KBuffBehaviorFactor BuffBehaviorFactor( eBehaviorType );
		
		switch ( eBehaviorType )
		{
		case BBT_CHANGE_NOW_HP_PER_SECOND:	/// 초당 HP 감소의 경우
			{
				const BUFF_CHANGE_TYPE eChangeType 
					= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, factor[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_CHANGE_TYPE] );

				BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

				if ( BCT_RELATION_VALUE == eChangeType )	/// 연동수치 인 경우
				{
					UINT uiCount = static_cast<UINT>( factor[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_MULTIPLIER_OR_VALUE] );
					
					if ( uiCount > 1 )
					{
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_RELATION_TYPE + uiLevel_] );
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_RELATION_TYPE + uiCount] );
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_ATTACK_VALUE + uiCount] );
					}
					else
					{
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_RELATION_TYPE + 1] );
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_RELATION_TYPE + 2] );
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_ATTACK_VALUE + 2] );
					}
				}
				else
				{
					UINT uiCount = static_cast<UINT>( factor[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_MULTIPLIER_OR_VALUE] );

					if ( uiCount > 1 )
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_MULTIPLIER_OR_VALUE + uiLevel_] );
					else
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeNowHpPerSecondBehaviorTemplet::FO_MULTIPLIER_OR_VALUE + 1] );
				}

				vecOutBehaviorFactorList.push_back( BuffBehaviorFactor );
			}
			break;

		case BBT_CHANGE_MAX_HP:
		case BBT_CHANGE_PHYSIC_ATTACK:
		case BBT_CHANGE_MAGIC_ATTACK:
		case BBT_CHANGE_PHYSIC_DEFENCE:
		case BBT_CHANGE_MAGIC_DEFENCE:
		case BBT_CHANGE_ANIM_SPEED:
		case BBT_CHANGE_WALK_RUN_SPEED:
		case BBT_CHANGE_JUMP_POWER:
		case BBT_CHANGE_ACCURACY:
		case BBT_CHANGE_AVOIDANCE:
		case BBT_CHANGE_CRITICAL_RATE:
		case BBT_CHANGE_CRITICAL_ATTACK:
		case BBT_CHANGE_ANTI_CRITICAL_RATE:
		case BBT_CHANGE_CRITICAL_DEFENCE:
		case BBT_CHANGE_ADDITIONAL_ATTACK:
		case BBT_CHANGE_ADDITIONAL_DEFENCE:
		case BBT_CHANGE_ATTRIBUTE_BLAZE_DEFENCE:
		case BBT_CHANGE_ATTRIBUTE_WATER_DEFENCE:
		case BBT_CHANGE_ATTRIBUTE_NATURE_DEFENCE:
		case BBT_CHANGE_ATTRIBUTE_WIND_DEFENCE:
		case BBT_CHANGE_ATTRIBUTE_LIGHT_DEFENCE:
		case BBT_CHANGE_ATTRIBUTE_DARK_DEFENCE:
		case BBT_CHANGE_EVERY_ATTRIBUTE_DEFENCE:
		case BBT_DRAIN_HP_NORMAL_ATTACK:
		case BBT_CHANGE_MAX_MP:
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		case BBT_CHANGE_HYPER_CHARGE_SPEED:
#endif //SERV_ADD_LUNATIC_PSYKER
			{
				const BUFF_CHANGE_TYPE eChangeType 
					= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, factor[CX2BuffChangeStatBehaviorTemplet::FO_CHANGE_TYPE] );

				BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

				if ( BCT_RELATION_VALUE == eChangeType )	/// 연동수치 인 경우
				{
					BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeStatBehaviorTemplet::FO_VALUE] );
					
					if ( factor[CX2BuffChangeStatBehaviorTemplet::FO_RELATION_TYPE] > 1 )
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeStatBehaviorTemplet::FO_RELATION_TYPE + uiLevel_] );
					else
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeStatBehaviorTemplet::FO_RELATION_TYPE + 1] );
				}
				else
				{
					if ( factor[CX2BuffChangeStatBehaviorTemplet::FO_VALUE] > 1 )
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeStatBehaviorTemplet::FO_VALUE + uiLevel_] );
					else
						BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeStatBehaviorTemplet::FO_VALUE + 1] );
				}

				vecOutBehaviorFactorList.push_back( BuffBehaviorFactor );
			}
			break;

		case BBT_CHANGE_HYPER_MODE:
			vecOutBehaviorFactorList.push_back( factor );
			break;

		case BBT_ABSORB_EFFECT_ATTACK:
			{
				if ( 30.f <= factor.m_vecValues.size() )
					BuffBehaviorFactor.m_vecValues.push_back( factor[uiLevel_] );
				else
					BuffBehaviorFactor.m_vecValues.push_back( factor[0] );
				vecOutBehaviorFactorList.push_back( BuffBehaviorFactor );
			} break;

		case BBT_CUSTOM_FUNCTION:
			{
				if ( 30.f <= factor.m_vecValues.size() )
				{
					BuffBehaviorFactor.m_vecValues.push_back( factor[0] );
					BuffBehaviorFactor.m_vecValues.push_back( factor[uiLevel_] );
				}
				else
				{
					BuffBehaviorFactor.m_vecValues.push_back( factor[0] );
					BuffBehaviorFactor.m_vecValues.push_back( factor[0] );
				}
				vecOutBehaviorFactorList.push_back( BuffBehaviorFactor );
			} break;
#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
		case BBT_CHANGE_CONSUME_MP_RATE:
			{
				const BUFF_CHANGE_TYPE eChangeType 
					= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, factor[CX2BuffChangeConsumeMpRate::FO_CHANGE_TYPE] );

				BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );
				BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeConsumeMpRate::FO_SKILL_ID] );

				if ( BCT_PERCENT == eChangeType )
				{
					BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeConsumeMpRate::FO_VALUE + uiLevel_] );
				}
				else
					DISPLAY_ERROR( L"BCT_PERCENT Only!!" );

				vecOutBehaviorFactorList.push_back( BuffBehaviorFactor );
			} break;
#endif //FIX_SKILL_BALANCE_AISHA_LENA

		default:
			{
				const BUFF_CHANGE_TYPE eChangeType 
					= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, factor[CX2BuffChangeStatBehaviorTemplet::FO_CHANGE_TYPE] );

				float fValue = 0.f;
				BuffBehaviorFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );
				fValue = factor[CX2BuffChangeStatBehaviorTemplet::FO_VALUE];

				if ( 30.f <= fValue )
					BuffBehaviorFactor.m_vecValues.push_back( factor[CX2BuffChangeStatBehaviorTemplet::FO_VALUE + uiLevel_] );
				else
					BuffBehaviorFactor.m_vecValues.push_back( factor[0] );

				vecOutBehaviorFactorList.push_back( BuffBehaviorFactor );
			}
			break;
		}
	}
}

void BUFF_FACTOR::GetFinalizerFactorToLevel( IN const vector<KBuffFinalizerFactor>& vecInFinalizerFactorList ,
											OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const UINT uiLevel_ )
{
	BOOST_FOREACH( const KBuffFinalizerFactor& factor, vecInFinalizerFactorList )
	{
		const BUFF_FINALIZER_TYPE eFinalizerType =static_cast<BUFF_FINALIZER_TYPE>( factor.m_uiType );
		
		switch ( eFinalizerType )
		{
		case BFT_TIME:
			{
				PushFinalizerTimeToLevel( vecOutFinalizerFactorList, factor, uiLevel_ );
			} break;
		case BFT_HIT_COUNT:
			{
				PushFinalizerHitToLevel( vecOutFinalizerFactorList, factor, uiLevel_ );
			} break;

		case BFT_HITTED_COUNT:
			{
				PushFinalizerHittedToLevel( vecOutFinalizerFactorList, factor, uiLevel_ );
			} break;
					
		case BFT_DAMAGE_VALUE:
			{
				PushFinalizerDamageValueToLevel( vecOutFinalizerFactorList, factor, uiLevel_ );
			} break;

		case BFT_PASS_HP:
			{
				PushFinalizerHpToLevel( vecOutFinalizerFactorList, factor, uiLevel_ );
			} break;

		case BFT_PASS_MP:
			{
				PushFinalizerMpToLevel( vecOutFinalizerFactorList, factor, uiLevel_ );
			} break;

		default:
			{
				KBuffFinalizerFactor BuffFinalizerFactor( eFinalizerType );
				vecOutFinalizerFactorList.push_back( BuffFinalizerFactor );
			}
			break;
		}	
	}
}

void BUFF_FACTOR::PushFinalizerTimeToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, const KBuffFinalizerFactor& factor_, IN const UINT uiLevel_ )
{
	KBuffFinalizerFactor BuffFinalizerFactor( BFT_TIME );

	BUFF_DURATION_TIME_TYPE eDurationTimeType 
		= STATIC_CAST_FLOAT_TO_ENUM( BUFF_DURATION_TIME_TYPE, factor_[CX2BuffTimeFinalizerTemplet::FO_DURATION_BUFF_TIME_TYPE] );

	BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eDurationTimeType ) );

	switch ( eDurationTimeType )
	{
	case BDTT_NORMAL_TIME:
		{
			UINT uiCount = static_cast<UINT>( factor_[CX2BuffTimeFinalizerTemplet::FO_NORMAL_TIME] );

			if ( uiCount > 1 )
				BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffTimeFinalizerTemplet::FO_NORMAL_TIME + uiLevel_] );
			else
				BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffTimeFinalizerTemplet::FO_NORMAL_TIME + 1] );
		} break;

	case BDTT_RELATION_HYPER_MODE_COUNT:
		{
			UINT uiCount = static_cast<UINT>( factor_[CX2BuffTimeFinalizerTemplet::FO_NORMAL_TIME] );

			if ( uiCount > 1 )
			{
				if ( factor_[CX2BuffTimeFinalizerTemplet::FO_NORMAL_TIME + uiLevel_] > 0.f )
					BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffTimeFinalizerTemplet::FO_NORMAL_TIME + uiLevel_] );
			}
			else
			{
				if ( factor_[CX2BuffTimeFinalizerTemplet::FO_NORMAL_TIME + uiLevel_] > 0.f )
					BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffTimeFinalizerTemplet::FO_NORMAL_TIME + 1] );
			}
		} break;

	case BDTT_RANDOM_TIME:
		{
			UINT uiCount = static_cast<UINT>( factor_[CX2BuffTimeFinalizerTemplet::FO_MIN_TIME] );

			if ( uiCount > 1 )
			{
				BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffTimeFinalizerTemplet::FO_MIN_TIME + uiLevel_] );
				BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffTimeFinalizerTemplet::FO_MAX_TIME + uiLevel_] );
			}
			else
			{
				BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffTimeFinalizerTemplet::FO_MIN_TIME + 1] );
				BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffTimeFinalizerTemplet::FO_MAX_TIME + 1] );
			}
		} break;

	default:
		return;
		break;
	}

	vecOutFinalizerFactorList.push_back( BuffFinalizerFactor );
}

void BUFF_FACTOR::PushFinalizerHitToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, const KBuffFinalizerFactor& factor_, IN const UINT uiLevel_ )
{
	KBuffFinalizerFactor BuffFinalizerFactor( BFT_HIT_COUNT );

	BUFF_USE_COUNT_TYPE eUseCountType 
		= STATIC_CAST_FLOAT_TO_ENUM( BUFF_USE_COUNT_TYPE, factor_[CX2BuffHitCountFinalizerTemplet::FO_USE_BUFF_COUNT_TYPE] );

	BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eUseCountType ) );

	switch ( eUseCountType )
	{
	case BUCT_NOT_USE:
	case BUCT_MY_BUFF_COUNT:
	case BUCT_MY_DEBUFF_COUNT:
		{
			UINT uiCount = static_cast<UINT>( factor_[CX2BuffHitCountFinalizerTemplet::FO_COUNT] );

			if ( uiCount > 1 )
				BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffHitCountFinalizerTemplet::FO_COUNT + uiLevel_] );
			else
				BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffHitCountFinalizerTemplet::FO_COUNT + 1] );
		} break;	

	default:
		return;
		break;
	}

	/// 2. 지정한 타격 횟수에 배율로 해야 할 것에 대한 공간 확보
	BuffFinalizerFactor.m_vecValues.push_back( 0.0f );

	vecOutFinalizerFactorList.push_back( BuffFinalizerFactor );
}

void BUFF_FACTOR::PushFinalizerHittedToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, const KBuffFinalizerFactor& factor_, IN const UINT uiLevel_ )
{
	KBuffFinalizerFactor BuffFinalizerFactor( BFT_HITTED_COUNT );

	BUFF_USE_COUNT_TYPE eUseCountType 
		= STATIC_CAST_FLOAT_TO_ENUM( BUFF_USE_COUNT_TYPE, factor_[CX2BuffHittedCountFinalizerTemplet::FO_USE_BUFF_COUNT_TYPE] );

	BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eUseCountType ) );

	switch ( eUseCountType )
	{
	case BUCT_NOT_USE:
	case BUCT_MY_BUFF_COUNT:
	case BUCT_MY_DEBUFF_COUNT:
		{
			UINT uiCount = static_cast<UINT>( factor_[CX2BuffHittedCountFinalizerTemplet::FO_COUNT] );

			if ( uiCount > 1 )
				BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffHittedCountFinalizerTemplet::FO_COUNT + uiLevel_] );
			else
				BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffHittedCountFinalizerTemplet::FO_COUNT + 1] );
		} break;	

	default:
		return;
		break;
	}

	/// 2. 지정한 타격 횟수에 배율로 해야 할 것에 대한 공간 확보
	BuffFinalizerFactor.m_vecValues.push_back( 0.0f );

	vecOutFinalizerFactorList.push_back( BuffFinalizerFactor );
}

void BUFF_FACTOR::PushFinalizerDamageValueToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, const KBuffFinalizerFactor& factor_, IN const UINT uiLevel_ )
{
	KBuffFinalizerFactor BuffFinalizerFactor( BFT_DAMAGE_VALUE );

	BUFF_CHANGE_TYPE eChangeType 
		= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, factor_[CX2BuffDamageValueFinalizerTemplet::FO_CHANGE_TYPE] );

	BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );
	BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffDamageValueFinalizerTemplet::FO_VALUE] );
	BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffDamageValueFinalizerTemplet::FO_RELATION_TYPE] );

	UINT uiCount = static_cast<UINT>( factor_[CX2BuffDamageValueFinalizerTemplet::FO_RELATION_TYPE + 1] );

	if ( BCT_RELATION_VALUE == eChangeType )	/// 연동수치 인 경우
	{
		if ( uiCount > 1 )
			BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffDamageValueFinalizerTemplet::FO_RELATION_TYPE + 1 + uiLevel_] );
		else
			BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffDamageValueFinalizerTemplet::FO_RELATION_TYPE + 2] );
	}
	else
	{
		if ( uiCount > 1 )
			BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffDamageValueFinalizerTemplet::FO_VALUE + 1 + uiLevel_] );
		else
			BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffDamageValueFinalizerTemplet::FO_VALUE + 2] );
	}

	vecOutFinalizerFactorList.push_back( BuffFinalizerFactor );
}

void BUFF_FACTOR::PushFinalizerHpToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, const KBuffFinalizerFactor& factor_, IN const UINT uiLevel_ )
{
	KBuffFinalizerFactor BuffFinalizerFactor( BFT_PASS_HP );

	BUFF_CHANGE_TYPE eChangeType 
		= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, factor_[CX2BuffPassHpFinalizerTemplet::FO_CHANGE_TYPE] );

	BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffPassHpFinalizerTemplet::FO_PASS_UP] );
	BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

	UINT uiCount = static_cast<UINT>( factor_[CX2BuffPassHpFinalizerTemplet::FO_CRITERION] );

	if ( uiCount > 1 )
		BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffPassHpFinalizerTemplet::FO_CRITERION + uiLevel_] );
	else
		BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffPassHpFinalizerTemplet::FO_CRITERION + 1] );

	vecOutFinalizerFactorList.push_back( BuffFinalizerFactor );
}

void BUFF_FACTOR::PushFinalizerMpToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, const KBuffFinalizerFactor& factor_, IN const UINT uiLevel_ )
{
	KBuffFinalizerFactor BuffFinalizerFactor( BFT_PASS_MP );

	BUFF_CHANGE_TYPE eChangeType 
		= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, factor_[CX2BuffPassMpFinalizerTemplet::FO_CHANGE_TYPE] );

	BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffPassMpFinalizerTemplet::FO_PASS_UP] );
	BuffFinalizerFactor.m_vecValues.push_back( static_cast<float>( eChangeType ) );

	UINT uiCount = static_cast<UINT>( factor_[CX2BuffPassMpFinalizerTemplet::FO_CRITERION] );

	if ( uiCount > 1 )
		BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffPassMpFinalizerTemplet::FO_CRITERION + uiLevel_] );
	else
		BuffFinalizerFactor.m_vecValues.push_back( factor_[CX2BuffPassMpFinalizerTemplet::FO_CRITERION + 1] );

	vecOutFinalizerFactorList.push_back( BuffFinalizerFactor );
}
#endif //UPGRADE_SKILL_SYSTEM_2013
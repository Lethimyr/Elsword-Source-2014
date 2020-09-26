/**@file : X2BuffBehaviorTemplet.cpp
   @breif : 버프의 행동을 정의하는 Templet 파일
*/

#pragma once

#include "StdAfx.h"
#include ".\X2BuffBehaviorTemplet.h"

#define CASE_BEHAVIOR_TEMPLET( class_, type_ ) \
case type_: \
	{ \
		CX2BuffBehaviorTempletPtr ptrBehaviorTemplet = class_::CreateBuffBehaviorTempletPtr(); \
		if ( NULL != ptrBehaviorTemplet && ptrBehaviorTemplet->ParsingBehaviorTemplateMethod( luaManager_, STRINGIZEA2(type_) ) ) \
		{ \
			ptrBehaviorTemplet->SetType( type_ ); \
			vecBuffBehaviorTempletPtr_.push_back( ptrBehaviorTemplet ); \
		} \
		else \
			return DISPLAY_ERROR( STRINGIZE2(type_) ); \
	} break; \

/** @function : ParsingCombinationBehavior
	@brief : 버프의 행동이 어떤종류로 구성되어 있는지 알아내고, 구성된 타입별로 파싱
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_), 파싱성공한 BehaviorTempletPtr을 담을 vector(vecBuffBehaviorTempletPtr_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*static*/ bool CX2BuffBehaviorTemplet::ParsingCombinationBehavior( KLuaManager& luaManager_, OUT vector<CX2BuffBehaviorTempletPtr>& vecBuffBehaviorTempletPtr_ )
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
			{
				bExceptionCase = false;
			}
#endif // EXCEPTION_BUFF_FACTOR_VER2

			switch ( static_cast<BUFF_BEHAVIOR_TYPE>( *vItr ) )
			{
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeNowHpPerSecondBehaviorTemplet, BBT_CHANGE_NOW_HP_PER_SECOND )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeNowMpPerSecondBehaviorTemplet, BBT_CHANGE_NOW_MP_PER_SECOND )
			CASE_BEHAVIOR_TEMPLET( CX2BuffReverseLeftRightBehaviorTemplet, BBT_REVERSE_LEFT_RIGHT )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_DASH_IMPOSSIBLE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_ATTACK_IMPOSSIBLE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_MAX_HP )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_PHYSIC_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_MAGIC_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_PHYSIC_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_MAGIC_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ANIM_SPEED )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_WALK_RUN_SPEED )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_JUMP_POWER )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ACCURACY )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_AVOIDANCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_CRITICAL_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_CRITICAL_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ANTI_CRITICAL_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_CRITICAL_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ADDITIONAL_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ADDITIONAL_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffCreateBuffFactortBehaviorTemplet, BBT_CREATE_BUFF_FACTOR )
			CASE_BEHAVIOR_TEMPLET( CX2BuffAbsorbEffectAttackBehaviorTemplet, BBT_ABSORB_EFFECT_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHARGE_MP_HIT_MELEE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHARGE_MP_HIT_RANGE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHARGE_MP_HITTED )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeHyperModeBehaviorTemplet, BBT_CHANGE_HYPER_MODE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_SUPER_ARMOR )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_MELEE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_RANGE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_FINAL_DEFENCE_MULTIPLIER )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_FORCE_DOWN_MULTIPLIER_MELEE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_FORCE_DOWN_MULTIPLIER_RANGE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffResetSkillCoolTimeBehaviorTemplet, BBT_RESET_SKILL_COOL_TIME )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeUnitScaleBehaviorTemplet, BBT_CHANGE_UNIT_SCALE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_BLAZE_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_WATER_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_NATURE_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_WIND_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_LIGHT_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_DARK_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_EVERY_ATTRIBUTE_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffCustomFunctionBehaviorTemplet, BBT_CUSTOM_FUNCTION )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_DRAIN_HP_NORMAL_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffEffectSetWithDamageBehaviorTemplet, BBT_EFFECT_SET_WITH_DAMAGE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffStunBehaviorTemplet, BBT_STUN )
			CASE_BEHAVIOR_TEMPLET( CX2BuffFinishOtherBuffDebuffBehaviorTemplet, BBT_FINISH_OTHER_BUFF_DEBUFF )
			CASE_BEHAVIOR_TEMPLET( CX2BuffFreezeBehaviorTemplet, BBT_FROZEN )
			CASE_BEHAVIOR_TEMPLET( CX2BuffCanPassUnitBehaviorTemplet, BBT_CAN_PASS_UNIT )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_ATTACK_ALL_TEAM )
			CASE_BEHAVIOR_TEMPLET( CX2BuffNeverMoveBehaviorTemplet, BBT_NEVER_MOVE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_MAX_MP )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeAttackByTypeBehaviorTemplet, BBT_CHANGE_ATTACK_BY_TYPE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ENCHANT_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffDoubleAttackBehaviorTemplet, BBT_DOUBLE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeAttackSphereScaleBehaviorTemplet, BBT_CHANGE_ATTACK_SPHERE_SCALE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_DOWN_IMPOSSIBLE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffAddSkillLevelBehaviorTemplet, BBT_ADD_SKILL_LEVEL )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_SKILL_ATTACK_IMPOSSIBLE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_ZX_ATTACK_IMPOSSIBLE )
	#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
			CASE_BEHAVIOR_TEMPLET( CX2BuffReflectMagicBehaviorTemplet, BBT_REFLECT_MAGIC )
	#endif // SERV_ARA_CHANGE_CLASS_SECOND
	#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeConsumeMpRate, BBT_CHANGE_CONSUME_MP_RATE )
	#endif //FIX_SKILL_BALANCE_AISHA_LENA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// Z 키 사용 X 키 사용 불가, 버프 타입 추가
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_Z_ATTACK_IMPOSSIBLE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_X_ATTACK_IMPOSSIBLE )
			
			// 각 속성 별 인챈트 확률 증가
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_BLAZE_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_WATER_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_NATURE_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_WIND_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_LIGHT_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_DARK_ATTACK_RATE )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_HYPER_CHARGE_SPEED )
#endif //SERV_ADD_LUNATIC_PSYKER

			case BBT_EMPTY:	/// 행동을 지정하지 않겠다고 명시적으로 해야함
				return true;
#ifdef EXCEPTION_BUFF_FACTOR_VER2
#ifndef SERV_ARA_CHANGE_CLASS_SECOND
			case BBT_REFLECT_MAGIC:
				++vItr;
				continue;
#endif SERV_ARA_CHANGE_CLASS_SECOND
#endif //EXCEPTION_BUFF_FACTOR_VER2
				break;

			default:
				return DISPLAY_ERROR( L"Unknown Behavior Templet Type" );	/// false 리턴
				break;
			}

			++vItr;
		}
	}

	if ( vecBuffBehaviorTempletPtr_.empty() 
#ifdef EXCEPTION_BUFF_FACTOR_VER2
		&& !bExceptionCase
#endif //EXCEPTION_BUFF_FACTOR_VER2
		)
	{
		return DISPLAY_ERROR( L"BEHAVIOR Templet is empty" );
	}
	else
		return true;
}

/** @function : ParsingBehaviorTemplateMethod
	@brief : 각 파생클래스에서 구현된 ParsingBehavior를 똑같은 전후 처리 사이에서 호출하기 위한 템플릿메소드
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_), 파싱성공한 BehaviorTempletPtr을 담을 vector(vecBuffBehaviorTempletPtr_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
bool CX2BuffBehaviorTemplet::ParsingBehaviorTemplateMethod( KLuaManager& luaManager_, const char* pszTableName_ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		return ParsingBehavior( luaManager_ );
	}
	else
		return DISPLAY_ERROR( pszTableName_ );	
}

/** @function : SetFactorFromPacketTemplateMothod
	@brief : 패킷으로 부터 받은 버프 정보로 각 행동의 SetFactorFromPacket을 실행 시키는 템플릿 메소드
	@param : 서버로 부터 받은 버프정보(kBuffFactor_), 적용할 게임유닛(pGameUnit_)
	@return : 성공(true), 실패(false)
*/
bool CX2BuffBehaviorTemplet::SetFactorFromPacketTemplateMothod( const KBuffFactor& kBuffFactor_, CX2GameUnit* pGameUnit_ )
{
	BOOST_FOREACH( const KBuffBehaviorFactor& kFactor_, kBuffFactor_.m_vecBuffBehaviorFactors )
	{
		if ( GetType() == kFactor_.m_uiType )
		{
			SetFactorFromPacket( kBuffFactor_, kFactor_, pGameUnit_ );
			return true;
		}	
	}

	return DISPLAY_ERROR( L"SetFactorFromPacket Error" );
}

/** @function : ParsingBehavior
	@brief :초당 HP가 증감되는 버프 행동을 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffChangeNowHpPerSecondBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	float fTimeInterval = 0.0f;	/// 몇초간격으로 HP가 변경되는가
	LUA_GET_VALUE_RETURN( luaManager_, "TIME_INTERVAL", fTimeInterval, 0.0f, return DISPLAY_ERROR( L"TIME_INTERVAL" ) );
	m_CheckElapsedTimeForInterval.SetTargetTime( fTimeInterval );

	LUA_GET_VALUE_RETURN( luaManager_, "DRAW_TEXT", m_bDrawText, false, return DISPLAY_ERROR( L"DRAW_TEXT" ) );
	return true;
}

/** @function : OnFrameMove
	@brief : interval 타이머 Update, HP 변경
	@param : HP가 변경되야 하는 게임유닛(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeNowHpPerSecondBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeNowHpPerSecondBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_CheckElapsedTimeForInterval.OnFrameMove( fElapsedTime_ );	/// Interval 타이머 진행
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_CheckElapsedTimeForInterval.OnFrameMove();	/// Interval 타이머 진행
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_delegateChangeHpByTypeFunc( pGameUnit_ );		/// HP 변경
}

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeNowHpPerSecondBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		const BUFF_CHANGE_TYPE eChangeType 
			= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] );

		if ( eChangeType == m_eChangeType )
		{
			/// 변경타입(CHANGE_TYPE)에 따라 수행 되야할 함수를 선택
			SelectDelegate();
			/// 저항을 무시하는 것도 아니고 영향을 받을 저항이 있는 경우 저항값을 얻어옴

// 			const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 				0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );

			const float fRegist = pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE;

			// 현재 효과의 적용값
			const float fNewValueOrPercent = m_delegateGetValueOrPercentPerSecondFunc( pBehaviorFactor, fRegist );

			if ( DidStart() )	/// 중첩 또는 교체라면
			{
				/// 중첩이 가능하다면 현재 얼마나 중첩되어 있는지를 얻어냄 (첫 효과 or 중첩이 안되면 0 이다)
				const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
				if ( uiAccumulationCountBefore > 0 )	/// 중첩 이라면
				{
#ifdef ADJUST_BUFF_ACCMULATION_CALCULATE
					if( eChangeType == BCT_PERCENT )
					{
						//바뀐 공식은 첫 효과가 1중첩이 된 것이라고 생각하고 작성됨.
						//따라서 uiAccumulationCountBefore에 1을 더해줌.
						m_fResultValue = 1 + ( ( fNewValueOrPercent - 1.0f ) * ( uiAccumulationCountBefore + 1 ) );
						if( m_fResultValue < 0.0f )
						{
							//m_fREsultValue값이 0보다 작지 않도록 값들을 계산해서 적어야 한다.(기획파트에서 정함)
							DISPLAY_ERROR( L"m_fResultValue error" );
							m_fResultValue = max( m_fResultValue, 0.0f );
						}
					}
					else
#endif //ADJUST_BUFF_ACCMULATION_CALCULATE
					{		
						// 중첩되었을 때 적용할 배율
						const float fAccumulationMultiflier = pBuffTemplet_->GetAccumulationMultiflier();
						// 이전 효과의 적용값
						const float fOldValueOrPercent = m_fResultValue / ( 1 + fAccumulationMultiflier * (uiAccumulationCountBefore - 1) );
						/// 이전 효과와 현재 효과 중 절대값 수치가 더 높을 것을 적용
						m_fResultValue = ( abs(fNewValueOrPercent) > abs(fOldValueOrPercent) ? fNewValueOrPercent : fOldValueOrPercent );
						/// 중첩 적용
						m_fResultValue *= ( 1 + fAccumulationMultiflier * uiAccumulationCountBefore );	/// 현재의 중첩횠수 - 1을 곱해줘야 하는 Before는 현재-1과 같으므로 그냥 곱한다.
					}
				}
				else	/// 교체 라면
					m_fResultValue = ( abs(fNewValueOrPercent) > abs(m_fResultValue) ? fNewValueOrPercent : m_fResultValue );
			}
			else	/// 중첩, 교체가 아니라면(첫시작 또는 중복)
			{
				m_fResultValue = fNewValueOrPercent;
			}
			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeNowHpPerSecondBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeNowHpPerSecondBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 초당 Hp를 증감 시키는 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeNowHpPerSecondBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fResultValue = kFactor_[0];
	SelectDelegate();
}

/** @function : SelectDelegate
	@brief : 초당 변경되야하는 타입에 따라 실행해야하는 함수를 선택함
	@param : 행동요소의 포인터(pBehaviorFactor_)
*/
void CX2BuffChangeNowHpPerSecondBehaviorTemplet::SelectDelegate()
{
	switch ( m_eChangeType )
	{
	case BCT_FIX_VALUE:			/// 값에 의한 변경이면 해당 델리게이트 셋팅
		{
			m_delegateChangeHpByTypeFunc = DelegateChangeHpByType::from_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::ChangeHpByValue>( this );
			m_delegateGetValueOrPercentPerSecondFunc = DelegateGetValueOrPercentPerSecond::from_const_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetFixValuePerSecond>( this );
		} break;
	case BCT_RELATION_VALUE:	/// 공격력에 의한 변경이면 해당 델리게이트 셋팅
		{
			m_delegateChangeHpByTypeFunc = DelegateChangeHpByType::from_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::ChangeHpByValue>( this );
			m_delegateGetValueOrPercentPerSecondFunc = DelegateGetValueOrPercentPerSecond::from_const_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetValuePerSecond>( this );
		} break;

	case BCT_PERCENT:	/// Percent에 의한 변경이면 해당 델리게이트 셋팅
		{
			m_delegateChangeHpByTypeFunc = DelegateChangeHpByType::from_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::ChangeHpByPercent>( this );
			m_delegateGetValueOrPercentPerSecondFunc = DelegateGetValueOrPercentPerSecond::from_const_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetPercentPerSecondByPercent>( this );
		} break;

	default:
		DISPLAY_ERROR( L"ChangeType Invalid" );
		break;
	}
}

/** @function : ChangeHpByValue
	@brief : 값에 의한 HP 변경
	@param : HP가 변경될 유닛(pGameUnit_)
*/
void CX2BuffChangeNowHpPerSecondBehaviorTemplet::ChangeHpByValue( CX2GameUnit* pGameUnit_ )
{
	if ( m_CheckElapsedTimeForInterval.CheckAndResetElapsedTime() )
	{
#ifdef FIX_RECOVERY_HP
		if( pGameUnit_->GetNowHp() <= 0 )
			return;
#endif // FIX_RECOVERY_HP

		pGameUnit_->UpNowHp( m_fResultValue );
		if ( m_bDrawText )
			DrawText( pGameUnit_, m_fResultValue );
	}
}

/** @function : ChangeHpByPercent
	@brief : %에 의한 HP 변경
	@param : HP가 변경될 유닛(pGameUnit_)
*/
void CX2BuffChangeNowHpPerSecondBehaviorTemplet::ChangeHpByPercent( CX2GameUnit* pGameUnit_ )
{
	if ( m_CheckElapsedTimeForInterval.CheckAndResetElapsedTime() )
	{
#ifdef FIX_RECOVERY_HP
		if( pGameUnit_->GetNowHp() <= 0 )
			return;
#endif // FIX_RECOVERY_HP

		const float fDeltaValue = pGameUnit_->GetMaxHp() * m_fResultValue;
		pGameUnit_->UpNowHp( fDeltaValue );
		if ( m_bDrawText )
			DrawText( pGameUnit_, fDeltaValue );
	}
}

/** @function : GetValuePerSecond
	@brief : 값에 의해 적용될 초당 변경량 얻어옴
	@param : 저항률(fRegist_)
	@return : 변경량(float)
*/
float CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetFixValuePerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const
{	
	/// 수치 * 저항
	return (*pBehaviorFactor_)[FO_MULTIPLIER_OR_VALUE] * ( 1.0f - fRegist_ );
}

/** @function : GetValuePerSecond
	@brief : 값에 의해 적용될 초당 변경량 얻어옴
	@param : 저항률(fRegist_)
	@return : 변경량(float)
*/
float CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetValuePerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const
{	
	/// 수치 * 배율 * 저항
	return (*pBehaviorFactor_)[FO_ATTACK_VALUE] * (*pBehaviorFactor_)[FO_MULTIPLIER_OR_VALUE] * ( 1.0f - fRegist_ );	/// 공격력 * 배율
}

/** @function : GetPercentPerSecondByPercent
	@brief : 값에 의해 적용될 초당 변경 % 얻어옴
	@param : 저항률(fRegist_)
	@return : 변경률(float)
*/
float CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetPercentPerSecondByPercent( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const
{
	/// 배율 * 저항
	return (*pBehaviorFactor_)[FO_PERCENT] * ( 1.0f - fRegist_ );
}

/** @function : DrawText
	@brief : HP의 변화량을 출력하는 함수
	@param : 게임유닛(pGameUnit_), 변화량(fDeltaValue_)
*/
void CX2BuffChangeNowHpPerSecondBehaviorTemplet::DrawText( CX2GameUnit* pGameUnit_, const float fDeltaValue_ )
{
	const UINT uiSize = 20;
	WCHAR wstrText[uiSize] = {0, };
	D3DXVECTOR3 numPos = pGameUnit_->GetHeadBonePos();
	numPos.y += -50.0f;

	if ( fDeltaValue_ > 0.0f )
	{
		StringCchPrintf( wstrText, uiSize, L"HP +%d", static_cast<int>( fDeltaValue_ ) );
		if( NULL != g_pData->GetPicCharRed() )
			g_pData->GetPicCharGreen()->DrawText( wstrText, numPos, pGameUnit_->GetDirVector(), CKTDGPicChar::AT_CENTER );
	}
	else
	{
		StringCchPrintf( wstrText, uiSize, L"HP %d", static_cast<int>( fDeltaValue_ ) );
		if( NULL != g_pData->GetPicCharRed() )
			g_pData->GetPicCharRed()->DrawText( wstrText, numPos, pGameUnit_->GetDirVector(), CKTDGPicChar::AT_CENTER );
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief : 초당 MP 증감이 이루어지는 버프 행동을 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffChangeNowMpPerSecondBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	float fTimeInterval = 0.0f;	/// 몇초간격으로 Mp가 변경되는가
	LUA_GET_VALUE_RETURN( luaManager_, "TIME_INTERVAL", fTimeInterval, 0.0f, return DISPLAY_ERROR( L"TIME_INTERVAL" ) );
	m_CheckElapsedTimeForInterval.SetTargetTime( fTimeInterval );

	LUA_GET_VALUE_RETURN( luaManager_, "DRAW_TEXT", m_bDrawText, false, return DISPLAY_ERROR( L"DRAW_TEXT" ) );	
	return true;
}

/** @function : OnFrameMove
	@brief : interval 타이머 Update, Mp 변경
	@param : Mp가 변경되야 하는 게임유닛(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeNowMpPerSecondBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeNowMpPerSecondBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_CheckElapsedTimeForInterval.OnFrameMove( fElapsedTime_ );	/// Interval 타이머 진행
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_CheckElapsedTimeForInterval.OnFrameMove();	/// Interval 타이머 진행
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_delegateChangeMpByTypeFunc( pGameUnit_ );		/// Mp 변경
}

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeNowMpPerSecondBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		const BUFF_CHANGE_TYPE eChangeType 
			= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] );

		if ( eChangeType == m_eChangeType )
		{
			/// 변경타입(CHANGE_TYPE)에 따라 수행 되야할 함수를 선택
			SelectDelegate();
			/// 저항을 무시하는 것도 아니고 영향을 받을 저항이 있는 경우 저항값을 얻어옴

// 			const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 				0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );

			const float fRegist = pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE;

			// 현재 효과의 적용값
			const float fNewValueOrPercent = GetValueOrPercentPerSecond( pBehaviorFactor, fRegist );

			if ( DidStart() )	/// 중첩 또는 교체라면
			{
				/// 중첩이 가능하다면 현재 얼마나 중첩되어 있는지를 얻어냄 (첫 효과 or 중첩이 안되면 0 이다)
				const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
				if ( uiAccumulationCountBefore > 0 )	/// 중첩 이라면
				{
#ifdef ADJUST_BUFF_ACCMULATION_CALCULATE
					if( eChangeType == BCT_PERCENT )
					{
						//바뀐 공식은 첫 효과가 1중첩이 된 것이라고 생각하고 작성됨.
						//따라서 uiAccumulationCountBefore에 1을 더해줌.
						m_fResultValue = 1 + ( ( fNewValueOrPercent - 1.0f ) * ( uiAccumulationCountBefore + 1 ) );

						if( m_fResultValue < 0.0f )
						{
							//m_fREsultValue값이 0보다 작지 않도록 값들을 계산해서 적어야 한다.(기획파트에서 정함)
							DISPLAY_ERROR( L"m_fResultValue error" );
							m_fResultValue = max( m_fResultValue, 0.0f );
						}
					}
					else
#endif //ADJUST_BUFF_ACCMULATION_CALCULATE
					{
						// 중첩되었을 때 적용할 배율
						const float fAccumulationMultiflier = pBuffTemplet_->GetAccumulationMultiflier();
						// 이전 효과의 적용값
						const float fOldValueOrPercent = m_fResultValue / ( 1 + fAccumulationMultiflier * (uiAccumulationCountBefore - 1) );
						/// 이전 효과와 현재 효과 중 절대값 수치가 더 높을 것을 적용
						m_fResultValue = ( abs(fNewValueOrPercent) > abs(fOldValueOrPercent) ? fNewValueOrPercent : fOldValueOrPercent );
						/// 중첩 적용
						m_fResultValue *= ( 1 + fAccumulationMultiflier * uiAccumulationCountBefore );	/// 현재의 중첩횠수 - 1을 곱해줘야 하는 Before는 현재-1과 같으므로 그냥 곱한다.
					}
				}
				else	/// 교체 라면
					m_fResultValue = ( abs(fNewValueOrPercent) > abs(m_fResultValue) ? fNewValueOrPercent : m_fResultValue );
			}
			else	/// 중첩, 교체가 아니라면(첫시작 또는 중복)
			{
				m_fResultValue = fNewValueOrPercent;
			}
			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeNowMpPerSecondBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeNowMpPerSecondBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeNowMpPerSecondBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffChangeNowMpPerSecondBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 초당 MP를 증감 시키는 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeNowMpPerSecondBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fResultValue = kFactor_[0];
	SelectDelegate();
}

/** @function : SelectDelegate
	@brief : 초당 변경되야하는 타입에 따라 실행해야하는 함수를 선택함
	@param : 행동요소의 포인터(pBehaviorFactor_)
*/
void CX2BuffChangeNowMpPerSecondBehaviorTemplet::SelectDelegate()
{
	switch ( m_eChangeType )
	{
	case BCT_FIX_VALUE:	/// 공격력에 의한 변경이면 해당 델리게이트 셋팅
		{
			m_delegateChangeMpByTypeFunc = DelegateChangeMpByType::from_method<CX2BuffChangeNowMpPerSecondBehaviorTemplet, &CX2BuffChangeNowMpPerSecondBehaviorTemplet::ChangeMpByValue>( this );
		} break;

	case BCT_PERCENT:	/// Percent에 의한 변경이면 해당 델리게이트 셋팅
		{
			m_delegateChangeMpByTypeFunc = DelegateChangeMpByType::from_method<CX2BuffChangeNowMpPerSecondBehaviorTemplet, &CX2BuffChangeNowMpPerSecondBehaviorTemplet::ChangeMpByPercent>( this );
		} break;

	default:
		DISPLAY_ERROR( L"ChangeType Invalid" );
		break;
	}
}

/** @function : ChangeMpByValue
	@brief : 값에 의한 Mp 변경
	@param : Mp가 변경될 유닛(pGameUnit_)
*/
void CX2BuffChangeNowMpPerSecondBehaviorTemplet::ChangeMpByValue( CX2GameUnit* pGameUnit_ )
{
	if ( m_CheckElapsedTimeForInterval.CheckAndResetElapsedTime() )
	{
		pGameUnit_->UpNowMp( m_fResultValue );
		if ( m_bDrawText )
			DrawText( pGameUnit_, m_fResultValue );
	}
}

/** @function : ChangeMpByPercent
	@brief : %에 의한 Mp 변경
	@param : Mp가 변경될 유닛(pGameUnit_)
*/
void CX2BuffChangeNowMpPerSecondBehaviorTemplet::ChangeMpByPercent( CX2GameUnit* pGameUnit_ )
{
	if ( m_CheckElapsedTimeForInterval.CheckAndResetElapsedTime() )
	{
		const float fDeltaValue = pGameUnit_->GetMaxMp() * m_fResultValue;
		pGameUnit_->UpNowMp( fDeltaValue );
		if ( m_bDrawText )
			DrawText( pGameUnit_, fDeltaValue );
	}
}

/** @function : GetValueOrPercentPerSecond
	@brief : 값에 의해 적용될 초당 변경 수치 or %를 얻어옴
	@param : 저항률(fRegist_)
	@return : 변경률(float)
*/
float CX2BuffChangeNowMpPerSecondBehaviorTemplet::GetValueOrPercentPerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const
{
	/// 배율 * 저항
	return (*pBehaviorFactor_)[FO_VALUE_OR_PERCENT] * ( 1.0f - fRegist_ );
}

/** @function : DrawText
	@brief : MP의 변화량을 출력하는 함수
	@param : 게임유닛(pGameUnit_), 변화량(fDeltaValue_)
*/
void CX2BuffChangeNowMpPerSecondBehaviorTemplet::DrawText( CX2GameUnit* pGameUnit_, const float fDeltaValue_ )
{
	const UINT uiSize = 20;
	WCHAR wstrText[uiSize] = {0, };
	D3DXVECTOR3 numPos = pGameUnit_->GetHeadBonePos();
	numPos.y += -50.0f;

	if ( fDeltaValue_ > 0.0f )
	{
		StringCchPrintf( wstrText, uiSize, L"MP +%d", static_cast<int>( fDeltaValue_ ) );
		if( NULL != g_pData->GetPicCharBlue() )
			g_pData->GetPicCharBlue()->DrawText( wstrText, numPos, pGameUnit_->GetDirVector(), CKTDGPicChar::AT_CENTER );
	}
	else
	{
		StringCchPrintf( wstrText, uiSize, L"MP %d", static_cast<int>( fDeltaValue_ ) );
		if( NULL != g_pData->GetPicCharRed() )
			g_pData->GetPicCharRed()->DrawText( wstrText, numPos, pGameUnit_->GetDirVector(), CKTDGPicChar::AT_CENTER );
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief : 버프 행동을 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffReverseLeftRightBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	float fTimeInterval = 0.0f;	/// 몇초 간격으로 좌우가 반전 되는가?
	LUA_GET_VALUE_RETURN( luaManager_, "TIME_INTERVAL", fTimeInterval, 0.0f, return DISPLAY_ERROR( L"TIME_INTERVAL" ) );

	m_CheckElapsedTimeForInterval.SetTargetTime( fTimeInterval );
	return true;
}

/** @function : OnFrameMove
	@brief : interval 타이머 Update 및 좌우반전 갱신
	@param : 좌우반전이 갱신되야 하는 게임유닛(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffReverseLeftRightBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffReverseLeftRightBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_CheckElapsedTimeForInterval.OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_CheckElapsedTimeForInterval.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_CheckElapsedTimeForInterval.CheckAndResetElapsedTime() )
		pGameUnit_->ToggleReverseLeftRightByBuff( pBuffTemplet_->GetBuffIdentity() );
}

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffReverseLeftRightBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		/// 중첩이 가능하다면 현재 얼마나 중첩되어 있는지를 얻어냄 (첫 효과 or 중첩이 안되면 0 이다)
		if ( !DidStart() )	/// 중첩 적용 안함
		{
			// 교체 적용 안함(교체도 효과가 달라지는게 아니기 때문에 적용할 필요 없음)
			pGameUnit_->CreateAndInsertReverseLeftRightByBuff( pBuffTemplet_->GetBuffIdentity() ); /// 좌우반전 인스턴스 생성 및 적용
		}	
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffReverseLeftRightBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffReverseLeftRightBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffReverseLeftRightBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffReverseLeftRightBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 좌우반전을 적용시키는 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffReverseLeftRightBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	pGameUnit_->CreateAndInsertReverseLeftRightByBuff( kBuffFactor_.m_BuffIdentity );
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffReverseLeftRightBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseReverseLeftRightByBuff( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffIdentityBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		/// 중첩이 가능하다면 현재 얼마나 중첩되어 있는지를 얻어냄 (첫 효과 or 중첩이 안되면 0 이다)
		if ( !DidStart() )	/// 중첩 적용 안함
		{
			// 교체 적용 안함(교체도 효과가 달라지는게 아니기 때문에 적용할 필요 없음)
			pGameUnit_->CreateAndInsertBuffIdentity( GetType(), pBuffTemplet_->GetBuffIdentity() ); /// 공격불가 인스턴스 생성 및 적용
		}
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffAttackImpossibleBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffIdentityBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffIdentityBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffIdentityBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 공격(콤보, 스킬)을 못하게 하는 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffIdentityBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	pGameUnit_->CreateAndInsertBuffIdentity( GetType(), kBuffFactor_.m_BuffIdentity );
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffIdentityBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseBuffIdentity( GetType(), pBuffTemplet_->GetBuffIdentity() );		
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief :스탯(이동속도, 크리티컬확률등등 포함)을 변경 시키는 버프 행동을 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffChangeStatBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_RELATION_TYPE", m_eRelationType, 
		BUFF_RELATION_TYPE, BRT_END, return DISPLAY_ERROR( L"BUFF_RELATION_TYPE" ) );

	return true;
}

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeStatBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
			if ( BCT_RELATION_VALUE != m_eChangeType || STATIC_CAST_FLOAT_TO_ENUM( BUFF_RELATION_TYPE, (*pBehaviorFactor)[FO_RELATION_TYPE] ) == m_eRelationType )
			{
				/// 저항을 무시하는 것도 아니고 영향을 받을 저항이 있는 경우 저항값을 얻어옴

// 				const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 					0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );

				const float fRegist = pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE;

				const float fNewValue = GetValue( pBehaviorFactor, pGameUnit_, fRegist );
				if ( DidStart() )	/// 중첩 또는 교체라면
				{
					/// 중첩이 가능하다면 현재 얼마나 중첩되어 있는지를 얻어냄 (첫 효과 or 중첩이 안되면 0 이다)
					const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
					if ( uiAccumulationCountBefore > 0 )	/// 중첩 이라면
					{
						// 중첩되었을 때 적용할 배율
						const float fAccumulationMultiflier = pBuffTemplet_->GetAccumulationMultiflier();

						if ( IsSamef( fAccumulationMultiflier ) )
							return DISPLAY_ERROR( L"AccumulationMultiflier must not be 0" );

						switch ( m_eChangeType )
						{
						case BCT_FIX_VALUE:
						case BCT_RELATION_VALUE:
							{
								// 이전 효과의 적용값	/// 첫 중첩이면(uiAccumulationCountBefore <= 1) 기존의 적용값을 old 값으로 그대로 사용
								const float fOldValue =
									(uiAccumulationCountBefore <= 1) ? m_fResultValue : ( m_fResultValue / ( 1 + fAccumulationMultiflier * (uiAccumulationCountBefore - 1) ) );

								/// 이전 효과와 현재 효과 중 절대값 수치가 더 높을 것을 적용 (마이너스의 경우에도 효과가 큰것)
								m_fResultValue = ( abs(fNewValue) > abs(fOldValue) ? fNewValue : fOldValue );
								/// 중첩 적용
								/// 현재의 중첩횟수 - 1 한 값을 을 곱해줘야 하는 uiAccumulationCountBefore가 이미 현재중첩횟수-1과 같으므로 그냥 곱한다.
								m_fResultValue += m_fResultValue * fAccumulationMultiflier * uiAccumulationCountBefore;	
							} break;

						case BCT_PERCENT:
							{
#ifdef ADJUST_BUFF_ACCMULATION_CALCULATE
								//바뀐 공식은 첫 효과가 1중첩이 된 것이라고 생각하고 작성됨.
								//따라서 uiAccumulationCountBefore에 1을 더해줌.
								m_fResultValue = 1 + ( ( fNewValue - 1.0f ) * ( uiAccumulationCountBefore + 1 ) );

								if( m_fResultValue < 0.0f )
								{
									//m_fREsultValue값이 0보다 작지 않도록 값들을 계산해서 적어야 한다.(기획파트에서 정함)
									DISPLAY_ERROR( L"m_fResultValue error" );
									m_fResultValue = max( m_fResultValue, 0.0f );
								}
#else //ADJUST_BUFF_ACCMULATION_CALCULATE
								// 이전 효과의 적용값	/// 첫 중첩이면(uiAccumulationCountBefore <= 1) 기존의 적용값을 old 값으로 그대로 사용
								const float fOldValue =
									(uiAccumulationCountBefore <= 1) ? m_fResultValue : ( m_fResultValue / pow( fAccumulationMultiflier, static_cast<float>(uiAccumulationCountBefore - 1) ) );

								/// 이전 효과와 현재 효과 중 절대값 수치가 더 높을 것을 적용 (1.0 보다 작은 경우 0.8 배 보다 0.7 배가 더 많은 효과를 주는 것이므로 0.7을 선택)
								m_fResultValue = ( abs(fNewValue - 1.0f) > abs(fOldValue - 1.0f) ? fNewValue: fOldValue );
								/// 중첩 적용
								/// 현재의 중첩횟수 - 1 한 값을 을 곱해줘야 하는 uiAccumulationCountBefore가 이미 현재중첩횟수-1과 같으므로 그냥 곱한다.
								m_fResultValue *= pow( fAccumulationMultiflier, static_cast<float>( uiAccumulationCountBefore ) );
								//m_fResultValue += 1.0f;
#endif //ADJUST_BUFF_ACCMULATION_CALCULATE
							} break;

						default:	/// 치환인 경우 행동요소는 중복 수행 안함
							break;
						}
					}
					else	/// 교체 라면
						m_fResultValue = ( abs(fNewValue) > abs(m_fResultValue) ? fNewValue : m_fResultValue );

					pGameUnit_->UpdateAdditionalStat( GetType(), m_eChangeType, pBuffTemplet_->GetBuffIdentity(), m_fResultValue );
				}
				else	/// 중첩, 교체가 아니라면(첫시작 또는 중복)
				{
					m_fResultValue = fNewValue;
					pGameUnit_->InsertAdditionalStat( GetType(), m_eChangeType, pBuffTemplet_->GetBuffIdentity(), m_fResultValue );
				}
				return true;
			}
			else
				return DISPLAY_ERROR( L"MISS MATCH BUFF_RELATION_TYPE" );
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeStatBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeStatBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeStatBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffChangeStatBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 스탯을 변경하는 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeStatBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fResultValue = kFactor_[0];
	pGameUnit_->InsertAdditionalStat( GetType(), m_eChangeType, kBuffFactor_.m_BuffIdentity, m_fResultValue );
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeStatBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseAdditionalStat( GetType(), m_eChangeType, pBuffTemplet_->GetBuffIdentity() );		
}

/** @function : GetValue
	@brief : ChangeType 에 따라 각기 다른 공식으로 변경값을 얻어내는 함수
	@param : 행동요소(pBehaviorFactor_), 버프가 적용된 게임유닛(pGameUnit_), 저항(fRegist_)
	@return : 변경값(float)
*/
float CX2BuffChangeStatBehaviorTemplet::GetValue( const KBuffBehaviorFactor* pBehaviorFactor_, CX2GameUnit* pGameUnit_, const float fRegist_ ) const
{
	const float fValue = (*pBehaviorFactor_)[FO_VALUE];

	switch ( m_eChangeType )
	{
	case BCT_SWAP_VALUE:
		{
			return fValue;
		} break;

	case BCT_FIX_VALUE:
		{
			if ( MustUseOptionData() )
			{
				const UINT uiLevel = pGameUnit_->GetUnitLevel();
				return fValue * ( 1.5f * (uiLevel - 1) + 9.5f ) * (1.0f - fRegist_);	/// 옵션 수치화 스크립트에 적용된 연산
			}
			else
			{
				return fValue * (1.0f - fRegist_);
			}
		} break;

	case BCT_RELATION_VALUE:
		{
			return fValue * (*pBehaviorFactor_)[FO_MULTIPLIER] * (1.0f - fRegist_);
		} break;

	case BCT_PERCENT:
		{
			/// 디버프라고 생각되는 값의 경우 (fValue <1.0f && fValue >0.0f)
			/// fRegist_ 적용			
			if ( fValue >= 1.0f )
				return fValue;
			else if ( fValue > 0.0f )
				return 1.0f - ( (1.0f - fValue) * (1.0f - fRegist_) );
			else
				return fValue;
		} break;

	default:
		return 0.0f;
		break;
	}
}

/** @function : MustUseOptionData
	@brief : 옵션 수치화를 사용해야 하는 행동인지를 얻어옴
	@return : 옵션 수치를 사용해야 하면 true, 그렇지 않으면 false
*/
bool CX2BuffChangeStatBehaviorTemplet::MustUseOptionData() const
{
	switch ( GetType() )
	{
	case BBT_CHANGE_ANIM_SPEED:
	case BBT_CHANGE_WALK_RUN_SPEED:
	case BBT_CHANGE_JUMP_POWER:
	case BBT_CHANGE_ACCURACY:
	case BBT_CHANGE_AVOIDANCE:
	case BBT_CHANGE_CRITICAL_RATE:
	case BBT_CHANGE_ADDITIONAL_ATTACK:
	case BBT_CHANGE_ADDITIONAL_DEFENCE:
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case BBT_CHANGE_HYPER_CHARGE_SPEED:
#endif //SERV_ADD_LUNATIC_PSYKER
		return true;
		break;
		
	default:
		return false;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffCreateBuffFactortBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		m_uiBuffFactorId = static_cast<UINT>( (*pBehaviorFactor)[FO_BUFF_FACTOR_ID] );
		if ( 0 < m_uiBuffFactorId )
		{
			if ( DidStart() )	/// 기존에 작동 중인 것이었으면 기존의 효과 삭제
				pGameUnit_->EraseTemporaryBuffFactor( pBuffTemplet_->GetBuffIdentity() );

		#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			m_uiSkillId = static_cast<UINT>( (*pBehaviorFactor)[FO_SKILL_ID] );
			if ( 0 > m_uiSkillId )
				return DISPLAY_ERROR( L"SKILL_ID doesn't exist" );
		#endif //UPGRADE_SKILL_SYSTEM_2013

			CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( m_uiBuffFactorId );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( pGameUnit_->GetGameUnitType() == CX2GameUnit::GUT_USER )
			{
				CX2GUUser* pUser = reinterpret_cast<CX2GUUser*>( pGameUnit_ );
				
				if ( NULL == pUser->GetUnit() )
					return false;

				const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SKILL_ID( m_uiSkillId ) ) );	/// 스킬 레벨
				pGameUnit_->CreateAndInsertTemporaryBuffFactor( *ptrBuffFactor, pBuffTemplet_->GetBuffIdentity(), iSkillTempletLevel );

				return true;
			}
#else //UPGRADE_SKILL_SYSTEM_2013
			pGameUnit_->CreateAndInsertTemporaryBuffFactor( *ptrBuffFactor, pBuffTemplet_->GetBuffIdentity() );
#endif //UPGRADE_SKILL_SYSTEM_2013
			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_FACTOR_ID doesn't exist" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffCreateBuffFactortBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffCreateBuffFactortBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffCreateBuffFactortBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffCreateBuffFactortBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_uiBuffFactorId ) );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 다른 유닛에게 지정된 버프 또는 디버프를 적용할 수 있는 행동 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffCreateBuffFactortBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_uiBuffFactorId = static_cast<UINT>( kFactor_[0] );
	CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( m_uiBuffFactorId );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( pGameUnit_->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser* pUser = reinterpret_cast<CX2GUUser*>( pGameUnit_ );
				
		if ( NULL == pUser->GetUnit() )
			return;

		const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SKILL_ID( m_uiSkillId ) ) );	/// 스킬 레벨

		pGameUnit_->CreateAndInsertTemporaryBuffFactor( *ptrBuffFactor, kBuffFactor_.m_BuffIdentity, iSkillTempletLevel );
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	pGameUnit_->CreateAndInsertTemporaryBuffFactor( *ptrBuffFactor, kBuffFactor_.m_BuffIdentity );
#endif //UPGRADE_SKILL_SYSTEM_2013
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffCreateBuffFactortBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseTemporaryBuffFactor( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffAbsorbEffectAttackBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( DidStart() )	/// 중첩 또는 교체
		{
			const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
			
			if ( uiAccumulationCountBefore > 1 )	/// 중첩 이라면
				m_fAbsorbValue += (*pBehaviorFactor)[FO_ABSORB_VALUE];
			else
				m_fAbsorbValue = (*pBehaviorFactor)[FO_ABSORB_VALUE];

			pGameUnit_->UpdateAbsorbEffectAttack( pBuffTemplet_->GetBuffIdentity(), m_fAbsorbValue );
		}
		else	/// 첫효과 또는 중복
		{
			m_fAbsorbValue = (*pBehaviorFactor)[FO_ABSORB_VALUE];
			pGameUnit_->CreateAndInsertAbsorbEffectAttack( pBuffTemplet_->GetBuffIdentity(), m_fAbsorbValue );
		}
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffAbsorbEffectAttackBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffAbsorbEffectAttackBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffAbsorbEffectAttackBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffAbsorbEffectAttackBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
	
	BehaviorFactor.m_vecValues.push_back( m_fAbsorbValue );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 이펙트를 흡수하여 MP로 전환하는 행동 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffAbsorbEffectAttackBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fAbsorbValue = kFactor_[0];
	pGameUnit_->CreateAndInsertAbsorbEffectAttack( kBuffFactor_.m_BuffIdentity, m_fAbsorbValue );
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffAbsorbEffectAttackBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseAbsorbEffectAttack( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

// /** @function : SetFactor
// 	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
// 	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
// */
// /*virtual*/ bool CX2BuffChangeMpHitBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
// {
// 	const KBuffBehaviorFactor* pBehaviorFactor = NULL;
// 
// 	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
// 	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
// 	{
// 		/// 저항을 무시하는 것도 아니고 영향을 받을 저항이 있는 경우 저항값을 얻어옴
// 		const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 			0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );
// 		// 현재 효과의 적용값
// 		const float fNewMultiplier = (*pBehaviorFactor)[FO_MULTIPLIER] * (1.0f - fRegist);
// 
// 		if ( DidStart() )	/// 중첩 또는 교체라면
// 		{
// 			/// 중첩이 가능하다면 현재 얼마나 중첩되어 있는지를 얻어냄 (첫 효과 or 중첩이 안되면 0 이다)
// 			const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
// 			if ( uiAccumulationCountBefore > 0 )	/// 중첩 이라면
// 			{
// 				// 중첩되었을 때 적용할 배율
// 				const float fAccumulationMultiflier = pBuffTemplet_->GetAccumulationMultiflier();
// 				// 이전 효과의 적용값
// 				const float fOldMultiplier = ( m_fMultiplier - 1.0f ) / ( 1 + fAccumulationMultiflier * (uiAccumulationCountBefore - 1) );
// 
// 				/// 이전 효과와 현재 효과 중 절대값 수치가 더 높을 것을 적용
// 				m_fMultiplier 
// 					= ( abs(fNewMultiplier - 1.0f) > abs(fOldMultiplier) ? fNewMultiplier - 1.0f : fOldMultiplier );
// 				/// 중첩 적용
// 				m_fMultiplier *= ( 1 + fAccumulationMultiflier * uiAccumulationCountBefore );	/// 현재의 중첩횠수 - 1을 곱해줘야 하는 Before는 현재-1과 같으므로 그냥 곱한다.
// 				m_fMultiplier += 1.0f;
// 			}
// 			else	/// 교체 라면
// 				m_fMultiplier = ( abs(fNewMultiplier) > abs(m_fMultiplier) ? fNewMultiplier : m_fMultiplier );
// 			
// 			pGameUnit_->UpdateAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity(), m_fMultiplier );
// 		}
// 		else	/// 첫효과 또는 중복
// 		{
// 			m_fMultiplier = fNewMultiplier;
// 			pGameUnit_->InsertAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity(), m_fMultiplier );
// 		}
// 		return true;
// 	}
// 	else
// 		return DISPLAY_ERROR( L"CX2BuffChangeMpHitBehaviorTemplet GetBehaviorFactor didn't work" );
// }
// 
// /** @function : GetClonePtr
// 	@return : CX2BuffBehaviorTempletPtr
// */
// /*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeMpHitBehaviorTemplet::GetClonePtr() const
// {
// 	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeMpHitBehaviorTemplet( *this ) );
// }
// 
// /** @function : GetFactor
// 	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
// 	@param : 버프요소 인스턴스(factor_)
// */
// /*virtual*/ void CX2BuffChangeMpHitBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
// {
// 	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
// 	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
// 
// 	BehaviorFactor.m_vecValues.push_back( m_fMultiplier );
// }
// 
// /** @function : SetFactorFromPacket
// 	@brief : 패킷으로 부터 받은 버프 정보로 타격시 MP 회수량을 증가하는 행동 템플릿을 셋팅
// 	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
// */
// /*virtual*/ void CX2BuffChangeMpHitBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
// {
// 	m_fMultiplier = kFactor_[0];
// 	pGameUnit_->InsertAdditionalStat( GetType(), BCT_SWAP_VALUE, kBuffFactor_.m_BuffIdentity, m_fMultiplier );
// }
// 
// /** @function : DoFinish
// 	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
// 	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
// */
// /*virtual*/ void CX2BuffChangeMpHitBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
// {
// 	pGameUnit_->EraseAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity() );
// }
// 
// //////////////////////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// /** @function : SetFactor
// 	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
// 	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
// */
// /*virtual*/ bool CX2BuffChangeMpHittedBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
// {
// 	const KBuffBehaviorFactor* pBehaviorFactor = NULL;
// 
// 	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
// 	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
// 	{
// 		/// 저항을 무시하는 것도 아니고 영향을 받을 저항이 있는 경우 저항값을 얻어옴
// 		const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 			0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );
// 		// 현재 효과의 적용값
// 		const float fNewMultiplier = (*pBehaviorFactor)[FO_MULTIPLIER] * (1.0f - fRegist);
// 
// 		if ( DidStart() )	/// 중첩 또는 교체라면
// 		{
// 			/// 중첩이 가능하다면 현재 얼마나 중첩되어 있는지를 얻어냄 (첫 효과 or 중첩이 안되면 0 이다)
// 			const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
// 			if ( uiAccumulationCountBefore > 0 )	/// 중첩 이라면
// 			{
// 				// 중첩되었을 때 적용할 배율
// 				const float fAccumulationMultiflier = pBuffTemplet_->GetAccumulationMultiflier();
// 				// 이전 효과의 적용값
// 				const float fOldMultiplier = ( m_fMultiplier - 1.0f ) / ( 1 + fAccumulationMultiflier * (uiAccumulationCountBefore - 1) );
// 
// 				/// 이전 효과와 현재 효과 중 절대값 수치가 더 높을 것을 적용
// 				m_fMultiplier 
// 					= ( abs(fNewMultiplier - 1.0f) > abs(fOldMultiplier) ? fNewMultiplier - 1.0f : fOldMultiplier );
// 				/// 중첩 적용
// 				m_fMultiplier *= ( 1 + fAccumulationMultiflier * uiAccumulationCountBefore );	/// 현재의 중첩횠수 - 1을 곱해줘야 하는 Before는 현재-1과 같으므로 그냥 곱한다.
// 				m_fMultiplier += 1.0f;
// 			}
// 			else	/// 교체 라면
// 				m_fMultiplier = ( abs(fNewMultiplier) > abs(m_fMultiplier) ? fNewMultiplier : m_fMultiplier );
// 
// 			pGameUnit_->UpdateAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity(), m_fMultiplier );
// 		}
// 		else	/// 첫효과 또는 중복
// 		{
// 			m_fMultiplier = fNewMultiplier;
// 			pGameUnit_->InsertAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity(), m_fMultiplier );
// 		}
// 		return true;
// 	}
// 	else
// 		return DISPLAY_ERROR( L"CX2BuffChangeMpHittedBehaviorTemplet GetBehaviorFactor didn't work" );
// }
// 
// /** @function : GetClonePtr
// 	@return : CX2BuffBehaviorTempletPtr
// */
// /*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeMpHittedBehaviorTemplet::GetClonePtr() const
// {
// 	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeMpHittedBehaviorTemplet( *this ) );
// }
// 
// /** @function : GetFactor
// 	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
// 	@param : 버프요소 인스턴스(factor_)
// */
// /*virtual*/ void CX2BuffChangeMpHittedBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
// {
// 	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
// 	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
// 
// 	BehaviorFactor.m_vecValues.push_back( m_fMultiplier );
// }
// 
// /** @function : SetFactorFromPacket
// 	@brief : 패킷으로 부터 받은 버프 정보로 피격시 회수되는 MP량을 증감시키는 행동 템플릿을 셋팅
// 	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
// */
// /*virtual*/ void CX2BuffChangeMpHittedBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
// {
// 	m_fMultiplier = kFactor_[0];
// 	pGameUnit_->InsertAdditionalStat( GetType(), BCT_SWAP_VALUE, kBuffFactor_.m_BuffIdentity, m_fMultiplier );
// }
// 
// /** @function : DoFinish
// 	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
// 	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
// */
// /*virtual*/ void CX2BuffChangeMpHittedBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
// {
// 	pGameUnit_->EraseAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity() );		
// }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : OnFrameMove
	@brief : 보여주기용 각성 시간을 갱신
	@param : 각성중인 유닛(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeHyperModeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeHyperModeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( 0.0f < m_fRemainTime )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_fRemainTime -= fElapsedTime_;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_fRemainTime -= g_pKTDXApp->GetElapsedTime();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_fRemainTime = max( m_fRemainTime, 0.0f );
		pGameUnit_->SetRemainHyperModeTime( m_fRemainTime );
// robobeg : frame move 호출 빈도에 따라 이펙트가 달라질 수도 있다. --;
		pGameUnit_->HyperModeBuffEffectOnFrameMove();
	}
}

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeHyperModeBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( 0 == m_uiHyperModeCount )	/// 재각성 등이 아닌 경우에만 갱신(3각성 중 1카운트로 재각성 했을 때  3각성이 1각성으로 변경되지 않도록!)
			m_uiHyperModeCount = max( m_uiHyperModeCount, static_cast<UINT>( (*pBehaviorFactor)[FO_HYPER_MODE_COUNT] ) );

		if ( static_cast<UINT>(CX2GageUI::PGUWO_ORB3) < m_uiHyperModeCount )
			pGameUnit_->SetFullHyperMode( true );

		m_fRemainTime = m_fRemainTime + (*pBehaviorFactor)[FO_HYPER_MODE_TIME];
		pGameUnit_->SetRemainHyperModeTime( m_fRemainTime );
		pGameUnit_->HyperModeBuffEffectStart();
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeHyperModeBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeHyperModeBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeHyperModeBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffChangeHyperModeBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_uiHyperModeCount ) );
	BehaviorFactor.m_vecValues.push_back( m_fRemainTime );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 각성 행동 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeHyperModeBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_uiHyperModeCount = static_cast<UINT>( kFactor_[0] );
	if ( static_cast<UINT>(CX2GageUI::PGUWO_ORB3) < m_uiHyperModeCount )
		pGameUnit_->SetFullHyperMode( true );

	m_fRemainTime = kFactor_[1];
	pGameUnit_->SetRemainHyperModeTime( m_fRemainTime );
	pGameUnit_->HyperModeBuffEffectStart();
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeHyperModeBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	CX2GageData* pGageData = CX2GageManager::GetInstance()->GetMyGageData();
	if ( NULL != pGageData )
		pGageData->SetHyperModeRemainTime( 0.0f );

	pGameUnit_->SetRemainHyperModeTime( 0.0f );
	pGameUnit_->HyperModeBuffEffectEnd();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : OnFrameMove
	@brief : 스킬 쿨타임 초기화
	@param : 버프에 걸리 게임유닛(pGameUnit_), 버프의 템플리정보(pBuffTemplet_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffResetSkillCoolTimeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffResetSkillCoolTimeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( DidStart() )
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환

#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
		pGameUnit_->ResetLeftSkillCoolTimeBySkillType( static_cast<CX2SkillTree::SKILL_TYPE>( m_eSkillTypeToResetCoolTime ), m_fCoolTime, false, m_eChangeType, m_fCoolTimeRate, m_bIsRelativeAllSkill );
#else // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME
		pGameUnit_->ResetLeftSkillCoolTimeBySkillType( static_cast<CX2SkillTree::SKILL_TYPE>( m_eSkillTypeToResetCoolTime ), m_fCoolTime, false, m_eChangeType );
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
		
#else // UPGRADE_SKILL_SYSTEM_2013
		pGameUnit_->ResetLeftSkillCoolTimeBySkillType( static_cast<CX2SkillTree::SKILL_TYPE>( m_eSkillTypeToResetCoolTime ) );
#endif // UPGRADE_SKILL_SYSTEM_2013
}

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffResetSkillCoolTimeBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
			switch ( m_eChangeType )
			{
			case BCT_FIX_VALUE:
			case BCT_RELATION_VALUE:
				{
					m_fCoolTime = (*pBehaviorFactor)[FO_VALUE];
					return true;

				} break;
#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
			case BCT_PERCENT :
				{
					m_fCoolTimeRate = (*pBehaviorFactor)[FO_VALUE];

					if ( m_fCoolTimeRate < 0 || m_fCoolTimeRate > 1 ) 
						return DISPLAY_ERROR( L"MISS MATCH BCT_PERCENT" );

					return true;			
				} break;
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
			
			default:
				{
#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME				// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
					return DISPLAY_ERROR( L"BCT_FIX_VALUE or BCT_RELATION_VALUE or BCT_PERCENT" );
#else  // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME
					return DISPLAY_ERROR( L"BCT_FIX_VALUE or BCT_RELATION_VALUE" );
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
				} break;
			}
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
#endif // UPGRADE_SKILL_SYSTEM_2013
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffResetSkillCoolTimeBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffResetSkillCoolTimeBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffResetSkillCoolTimeBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffResetSkillCoolTimeBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
}

/** @function : ParsingBehavior
	@brief : 버프템플릿 스크립트로 부터 초기화 할 스킬 타입을 읽어들임
	@param : 루아매니터(luaManager_)
	@return : 성공시 true, 실패시 false
*/
/*virtual*/ bool CX2BuffResetSkillCoolTimeBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
#endif // UPGRADE_SKILL_SYSTEM_2013

	LUA_GET_VALUE_RETURN( luaManager_, "SKILL_TYPE", m_eSkillTypeToResetCoolTime, 0, return DISPLAY_ERROR( L"SKILL_TYPE") );

#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
	LUA_GET_VALUE ( luaManager_, "RELATIVE_ALL_SKILL", m_bIsRelativeAllSkill, 0 );	
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeUnitScaleBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( !DidStart() )	/// 처음 적용
		{
			pGameUnit_->CreateAndInsertChangeUnitScaleByBuff( pBuffTemplet_->GetBuffIdentity(), m_vScale );
		}
#ifdef POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE
		else				/// 중첩 또는 교체
		{
			pGameUnit_->UpdateChangeUnitScaleByBuff( pBuffTemplet_->GetBuffIdentity(), m_vScale,
													pBuffTemplet_->GetAccumulationLimit());
		}
#endif POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeUnitScaleBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeUnitScaleBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeUnitScaleBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffChangeUnitScaleBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 유닛의 스케일을 변경하는 행동 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeUnitScaleBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	if ( !DidStart() )
		pGameUnit_->CreateAndInsertChangeUnitScaleByBuff( kBuffFactor_.m_BuffIdentity, m_vScale );
}

/** @function : ParsingBehavior
	@brief : 유닛의 스케일을 변경하는 행동 템플릿을 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffChangeUnitScaleBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "SCALE_X", m_vScale.x, 0.0f, return DISPLAY_ERROR( L"SCALE_X" ) );
	LUA_GET_VALUE_RETURN( luaManager_, "SCALE_Y", m_vScale.y, 0.0f, return DISPLAY_ERROR( L"SCALE_Y" ) );
	LUA_GET_VALUE_RETURN( luaManager_, "SCALE_Z", m_vScale.z, 0.0f, return DISPLAY_ERROR( L"SCALE_Z" ) );

	/// 스케일이 원래대로 돌아갈 때 정해진 시간간격과 방법을 사용할 것인가
	LUA_GET_VALUE_RETURN( luaManager_, "USE_TIME_TO_RETURN", m_bUseTimeToReturn, false, return DISPLAY_ERROR( L"USE_TIME_TO_RETURN" ) );

	return true;
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeUnitScaleBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
		pGameUnit_->EraseChangeUnitScaleByBuff( pBuffTemplet_->GetBuffIdentity(), m_bUseTimeToReturn );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffCustomFunctionBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( !DidStart() )
		{
			m_kBuffBehaviorFactor = *pBehaviorFactor;
			pGameUnit_->CreateAndInsertCustomFunctionByBuff( pBuffTemplet_->GetBuffIdentity(), *pBehaviorFactor );
		}

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffCustomFunctionBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffCustomFunctionBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffCustomFunctionBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffCustomFunctionBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( m_kBuffBehaviorFactor );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 CustomFunction을 유닛에 적용
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffCustomFunctionBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	if ( !DidStart() )
		pGameUnit_->CreateAndInsertCustomFunctionByBuff( kBuffFactor_.m_BuffIdentity, kFactor_ );
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffCustomFunctionBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
		pGameUnit_->EraseCustomFunctionByBuff( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffEffectSetWithDamageBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( NULL != g_pX2Game && !DidStart() )
		{
			m_bNotUse = ( 0.0f < (*pBehaviorFactor)[0] ? true : false );
			m_fPowerRate = pGameUnit_->GetPowerRate();	/// 사용 당시의 PowerRate 저장
			if ( !m_bNotUse )	/// FACTOR에서 사용하지 않겠다고 지정한것이 아니면
				PlayEffectSet( pGameUnit_ );
		}
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffEffectSetWithDamageBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/CX2BuffBehaviorTempletPtr CX2BuffEffectSetWithDamageBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffEffectSetWithDamageBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/void CX2BuffEffectSetWithDamageBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( ( true == m_bNotUse ? 1.0f : 0.0f ) );
	BehaviorFactor.m_vecValues.push_back( m_fPowerRate );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 데미지를 이펙트셋을 실행
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/void CX2BuffEffectSetWithDamageBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_bNotUse = ( 0.0f < kFactor_[0] ? true : false );
	m_fPowerRate = kFactor_[1];
	if ( !m_bNotUse )	/// FACTOR에서 사용하지 않겠다고 지정한것이 아니면
		PlayEffectSet( pGameUnit_ );
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/void CX2BuffEffectSetWithDamageBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game && DidStart() )
	{
		BOOST_FOREACH( CX2EffectSet::Handle& hHandle, m_vecHandleEffectSet )
		{
			g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
		}
	}
}

/** @function : ParsingBehavior
	@brief : 유닛의 스케일을 변경하는 행동 템플릿을 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/bool CX2BuffEffectSetWithDamageBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	int iIndex = 1;

	wstring wstrEffectSetName;
	while( luaManager_.GetValue( iIndex, wstrEffectSetName ) )
	{
		m_vecWstrEffectSetName.push_back( wstrEffectSetName );
		m_vecHandleEffectSet.push_back( INVALID_EFFECTSET_HANDLE );
		wstrEffectSetName.resize(0);
		++iIndex;
	}

	if ( 1 != iIndex )	
		return true;
	else
		return false;
}

/** @function : PlayEffectSet
	@brief : 벡터에 있는 이펙트 셋들을 실행
	@param : 해당 버프에 걸렸던 게임유닛(pGameUnit_)
*/
void CX2BuffEffectSetWithDamageBehaviorTemplet::PlayEffectSet( CX2GameUnit* pGameUnit_ )
{
	if ( NULL != g_pX2Game && !DidStart() )
	{
		UINT uiSize = m_vecWstrEffectSetName.size();
		for ( UINT uiIndex = 0; uiIndex < uiSize; ++uiIndex )
		{
			m_vecHandleEffectSet[uiIndex] 
			= g_pX2Game->GetEffectSet()->PlayEffectSet( m_vecWstrEffectSetName[uiIndex], pGameUnit_,
				NULL, ( 0.0f < pGameUnit_->GetRemainHyperModeTime() ), m_fPowerRate, -1.0f);//, pGameUnit_->GetScaleByUnit() );
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffStunBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		/// 중첩이 가능하다면 현재 얼마나 중첩되어 있는지를 얻어냄 (첫 효과 or 중첩이 안되면 0 이다)
		if ( !DidStart() )	/// 중첩 적용 안함
		{
			// 교체 적용 안함(교체도 효과가 달라지는게 아니기 때문에 적용할 필요 없음)
			pGameUnit_->CreateAndInsertStunByBuff( pBuffTemplet_->GetBuffIdentity() ); /// 좌우반전 인스턴스 생성 및 적용

#ifdef NOT_CANCEL_BBT_STUN_BY_HIT		/// 적용 즉시 그로기 상태로 변경 ( CommonFrameMove에서 처음 적용시엔 무조건 그로기 상태 변경하도록 설정 )
			if( CX2GameUnit::GUT_USER ==  pGameUnit_->GetGameUnitType() )
			{
				CX2GUUser *pUser = static_cast<CX2GUUser*>( pGameUnit_ );
				if( NULL != pUser )
					pUser->SetChangeGroggyState( true );
			}
#endif NOT_CANCEL_BBT_STUN_BY_HIT
		}	
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffStunBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/CX2BuffBehaviorTempletPtr CX2BuffStunBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffStunBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/void CX2BuffStunBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 데미지를 이펙트셋을 실행
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/void CX2BuffStunBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	pGameUnit_->CreateAndInsertStunByBuff( kBuffFactor_.m_BuffIdentity );

#ifdef NOT_CANCEL_BBT_STUN_BY_HIT		/// 적용 즉시 그로기 상태로 변경 ( CommonFrameMove에서 처음 적용시엔 무조건 그로기 상태 변경하도록 설정 )
	if( CX2GameUnit::GUT_USER ==  pGameUnit_->GetGameUnitType() )
	{
		CX2GUUser *pUser = static_cast<CX2GUUser*>( pGameUnit_ );
		if( NULL != pUser )
			pUser->SetChangeGroggyState( true );
	}
#endif NOT_CANCEL_BBT_STUN_BY_HIT
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/void CX2BuffStunBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseStunByBuff( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffFinishOtherBuffDebuffBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		EraseBuffTempletFromGameUnit( pGameUnit_, BuffFactor_.GetBuffTempletID() );
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffFinishOtherBuffDebuffBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffFinishOtherBuffDebuffBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffFinishOtherBuffDebuffBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffFinishOtherBuffDebuffBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 특정 버프들을 종료 시킴
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
///*virtual*/ void CX2BuffFinishOtherBuffDebuffBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
// {
// 	EraseBuffTempletFromGameUnit( pGameUnit_ );
// }

/** @function : ParsingBehavior
	@brief : 종료 시킬 버프의 템플리ID를 루아스크립트로 부터 읽어들인다
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffFinishOtherBuffDebuffBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	int iIndex = 1;

	UINT uiBuffTempletID = 0;
	while( luaManager_.GetValue( iIndex, uiBuffTempletID ) )
	{
		m_vecBuffTempletIDToBeFinished.push_back( static_cast<BUFF_TEMPLET_ID>( uiBuffTempletID ) );
		++iIndex;
	}

	if ( 1 != iIndex )	
		return true;
	else
		return false;
}

/** @function : EraseBuffTempletFromGameUnit
	@brief : 특정 버프가 있는 경우 종료시킨다
	@param : 버프에 걸린 게임 유닛(pGameUnit_), 삭제에서 제외할 BuffTempletID로 자기자신을 삭제하는 경우를 방지하기 위해 자신의 BuffTempletID를 넣어준다.
*/
void CX2BuffFinishOtherBuffDebuffBehaviorTemplet::EraseBuffTempletFromGameUnit( CX2GameUnit* pGameUnit_, const BUFF_TEMPLET_ID eExceptBuffTempletID_ )
{
	BOOST_FOREACH( const BUFF_TEMPLET_ID eBuffTempletID_, m_vecBuffTempletIDToBeFinished )
	{
		if ( eExceptBuffTempletID_ != eBuffTempletID_ )
			pGameUnit_->EraseBuffTempletFromGameUnit( eBuffTempletID_ );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffFreezeBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		/// 중첩이 가능하다면 현재 얼마나 중첩되어 있는지를 얻어냄 (첫 효과 or 중첩이 안되면 0 이다)
		if ( !DidStart() )	/// 중첩 적용 안함
		{
			// 교체 적용 안함(교체도 효과가 달라지는게 아니기 때문에 적용할 필요 없음)
			pGameUnit_->CreateAndInsertFreezeByBuff( pBuffTemplet_->GetBuffIdentity() ); /// 좌우반전 인스턴스 생성 및 적용
			m_fAnimCurrentTime = pGameUnit_->GetNowAnimationTime();
			pGameUnit_->AnimStop();
		}	
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffStunBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/CX2BuffBehaviorTempletPtr CX2BuffFreezeBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffFreezeBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/void CX2BuffFreezeBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( m_fAnimCurrentTime );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 데미지를 이펙트셋을 실행
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/void CX2BuffFreezeBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fAnimCurrentTime = kFactor_[0];
	pGameUnit_->SetNowAnimationTime( m_fAnimCurrentTime );
	pGameUnit_->AnimStop();
	//pGameUnit_->CreateAndInsertFreezeByBuff( kBuffFactor_.m_BuffIdentity );
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/void CX2BuffFreezeBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseFreezeByBuff( pBuffTemplet_->GetBuffIdentity() );

	pGameUnit_->SetStateToNowAnimation( CKTDGXSkinAnim::XAS_PLAYING );

	pGameUnit_->AnimPlay();

	if( true == m_bIsDown && CX2GameUnit::GUT_USER == pGameUnit_->GetGameUnitType())		/// 버프 종료시 다운시킬지 설정 ( NPC인 경우, 다운 시키지 않는다. )
	{
#ifdef RIDING_SYSTEM
		/// 탈것 탑승중이면, 탈것 데미지 스테이트로 전환
		if ( true == pGameUnit_->GetRidingOn() )
			pGameUnit_->StateChange( CX2GUUser::USI_RIDING_DAMAGE_FRONT, true );
		else
#endif // RIDING_SYSTEM
			pGameUnit_->StateChange( CX2GUUser::USI_DAMAGE_DOWN_FRONT, true );
	}
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffFreezeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffFreezeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( DidStart() )
		pGameUnit_->AnimStop();
}

/** @function : ParsingBehavior
	@brief : 동작이 멈추는 행동을 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffFreezeBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "IS_DOWN", m_bIsDown, false, return DISPLAY_ERROR( L"IS_DOWN" ) );

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffCanPassUnitBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffCanPassUnitBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffCanPassUnitBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffCanPassUnitBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffCanPassUnitBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : OnFrameMove
	@brief : 매 프레임 SetCanPassUnit 지정
	@param : 버프에 걸리 게임유닛(pGameUnit_), 버프의 템플리정보(pBuffTemplet_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffCanPassUnitBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffCanPassUnitBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( DidStart() )
		pGameUnit_->SetCanPassUnit( m_bCanPassUnit );
}

/** @function : ParsingBehavior
	@brief : 종료 시킬 버프의 템플리ID를 루아스크립트로 부터 읽어들인다
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffCanPassUnitBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "CAN_PASS_UNIT", m_bCanPassUnit, 
		false, return DISPLAY_ERROR( L"CAN_PASS_UNIT" ) );
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef NEVER_MOVE_GRAVITY_APPLY
/** @function : ParsingBehavior
	@brief : 중력이 적용되는가를 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffNeverMoveBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "IS_GRAVITY", m_bIsGravity, false, return DISPLAY_ERROR( L"IS_GRAVITY" ) );

	return true;
}
#endif //NEVER_MOVE_GRAVITY_APPLY


/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffNeverMoveBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( !DidStart() )
		{
			m_vPosition = pGameUnit_->GetPos(true);
#ifdef NEVER_MOVE_GRAVITY_APPLY
			pGameUnit_->CreateAndInsertNeverMoveByBuff( pBuffTemplet_->GetBuffIdentity(), m_vPosition, m_bIsGravity );
#else
			pGameUnit_->CreateAndInsertNeverMoveByBuff( pBuffTemplet_->GetBuffIdentity(), m_vPosition );
#endif //NEVER_MOVE_GRAVITY_APPLY
		}
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffNeverMoveBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffNeverMoveBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffNeverMoveBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffNeverMoveBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( m_vPosition.x );
	BehaviorFactor.m_vecValues.push_back( m_vPosition.y );
	BehaviorFactor.m_vecValues.push_back( m_vPosition.z );
#ifdef NEVER_MOVE_GRAVITY_APPLY
	BehaviorFactor.m_vecValues.push_back( ( true == m_bIsGravity ? 1.0f : 0.0f ) );
#endif //NEVER_MOVE_GRAVITY_APPLY
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 행동 템플릿 실행
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffNeverMoveBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_vPosition.x = kFactor_[0];
	m_vPosition.y = kFactor_[1];
	m_vPosition.z = kFactor_[2];
#ifdef NEVER_MOVE_GRAVITY_APPLY
	m_bIsGravity  = ( 0.0f < kFactor_[3] ? true : false );
#endif //NEVER_MOVE_GRAVITY_APPLY

#ifdef NEVER_MOVE_GRAVITY_APPLY
	pGameUnit_->CreateAndInsertNeverMoveByBuff( kBuffFactor_.m_BuffIdentity, m_vPosition, m_bIsGravity );
#else
	pGameUnit_->CreateAndInsertNeverMoveByBuff( kBuffFactor_.m_BuffIdentity, m_vPosition );
#endif //NEVER_MOVE_GRAVITY_APPLY
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffNeverMoveBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseNeverMoveByBuff( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief : 타입에 따른 공격력 수치를 변경 시키는 버프 행동을 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffChangeAttackByTypeBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	return true;
}

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeAttackByTypeBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
			if ( BCT_PERCENT == m_eChangeType )	/// 일단 무조건 배율
			{
				/*if ( DidStart() )
				{*/
					m_fResultValue = (*pBehaviorFactor)[FO_VALUE];
					m_eAttackType = STATIC_CAST_FLOAT_TO_ENUM( CX2DamageManager::ATTACK_TYPE, (*pBehaviorFactor)[FO_ATTACK_TYPE] );

					pGameUnit_->CreateAndInsertChangeAttackByType( pBuffTemplet_->GetBuffIdentity(), m_eAttackType, m_fResultValue );

					return true;
				//}
			}
			else
				return DISPLAY_ERROR( L"ONLY BCT_PERCENT" );
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeStatBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeAttackByTypeBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeAttackByTypeBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffChangeAttackByTypeBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eChangeType ) );
	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eAttackType ) );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 스탯을 변경하는 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeAttackByTypeBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	if ( kFactor_.m_vecValues.size() >= static_cast<UINT>( FO_END ) )
	{
		m_eChangeType	= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, kFactor_.m_vecValues[FO_CHANGE_TYPE] );
		m_fResultValue	= kFactor_.m_vecValues[FO_VALUE];
		m_eAttackType	= STATIC_CAST_FLOAT_TO_ENUM( CX2DamageManager::ATTACK_TYPE, kFactor_.m_vecValues[FO_ATTACK_TYPE] );

		pGameUnit_->CreateAndInsertChangeAttackByType( kBuffFactor_.m_BuffIdentity, m_eAttackType, m_fResultValue );
	}
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeAttackByTypeBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseChangeAttackByType( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief : 소켓 속성 공격 발동 확률을 변경 시키는 버프 행동을 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffChangeEncahntAttackRateBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 버프 타입에 대해 지정 가능하도록 추가
	LUA_GET_VALUE_ENUM ( luaManager_, "ENCHANT_ATTRIBUTE_TYPE", m_eEnchantAttributeType, 
		BUFF_ENCHANT_ATTRIBUTE_TYPE, BEAT_ALL );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	return true;
}

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeEncahntAttackRateBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;


	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// BBT_CHANGE_ATTRIBUTE_BLAZE_ATTACK_RATE 등 특정 속성만 강화 시키는 BBT 는
			// m_eEnchantAttributeType 값만 수정해서 넣는다.
			// 후에 확률 계산 때 해당 속성에 대해서만 체크하게 됨
			switch ( GetType() )
			{
			case BBT_CHANGE_ATTRIBUTE_BLAZE_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_BLAZE;
				break;
			case BBT_CHANGE_ATTRIBUTE_WATER_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_FROZEN;
				break;
			case BBT_CHANGE_ATTRIBUTE_NATURE_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_POSION;
				break;
			case BBT_CHANGE_ATTRIBUTE_LIGHT_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_SHOCK;
				break;
			case BBT_CHANGE_ATTRIBUTE_DARK_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_SNATCH;
				break;
			case BBT_CHANGE_ATTRIBUTE_WIND_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_PIERCING;			
				break;
			}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편


			if ( BCT_PERCENT == m_eChangeType ) /// 일단 무조건 배율
			{
				/*if ( DidStart() )
				{*/
					m_fRate = (*pBehaviorFactor)[FO_VALUE];
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					// 공격 타입 및 변경 조건에 대해 추가
					pGameUnit_->CreateAndInsertChangeEnchantAttackRate( pBuffTemplet_->GetBuffIdentity(), m_fRate, m_eEnchantAttributeType, m_eChangeType );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편					
					pGameUnit_->CreateAndInsertChangeEnchantAttackRate( pBuffTemplet_->GetBuffIdentity(), m_fRate );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편					

					return true;
				//}
			}
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// FIX_VALUE 까지 확장, 인챈트 속성 발동 확률이 0 이 넘어야지 적용된다.
			else if ( BCT_FIX_VALUE == m_eChangeType )		
			{
				m_fRate = (*pBehaviorFactor)[FO_VALUE];
				// 공격 타입 및 변경 조건에 대해 추가
				pGameUnit_->CreateAndInsertChangeEnchantAttackRate( pBuffTemplet_->GetBuffIdentity(), m_fRate, m_eEnchantAttributeType, m_eChangeType );
				return true;
			}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			else
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				return DISPLAY_ERROR( L"ONLY BCT_PERCENT OR BCT_FIX_VALUE" );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				return DISPLAY_ERROR( L"ONLY BCT_PERCENT" );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeStatBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeEncahntAttackRateBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeEncahntAttackRateBehaviorTemplet( *this ) );

}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffChangeEncahntAttackRateBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );

	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eChangeType ) );
	BehaviorFactor.m_vecValues.push_back( m_fRate );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 스탯을 변경하는 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeEncahntAttackRateBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	if ( kFactor_.m_vecValues.size() >= static_cast<UINT>( FO_END ) )
	{
		m_eChangeType	= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, kFactor_.m_vecValues[FO_CHANGE_TYPE] );
		m_fRate = kFactor_.m_vecValues[FO_VALUE];

		pGameUnit_->CreateAndInsertChangeEnchantAttackRate( kBuffFactor_.m_BuffIdentity, m_fRate );
	}
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeEncahntAttackRateBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseChangeEnchantAttackRate( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief : 소켓 속성 공격 발동 확률을 변경 시키는 버프 행동을 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffDoubleAttackBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	return true;
}

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffDoubleAttackBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
			if ( BCT_PERCENT == m_eChangeType ) /// 일단 무조건 배율
			{
				/*if ( DidStart() )
				{*/
					m_fRate = (*pBehaviorFactor)[FO_VALUE];

					pGameUnit_->CreateAndInsertDoubleAttackByBuff( pBuffTemplet_->GetBuffIdentity(), m_fRate );

					return true;
				//}
			}
			else
				return DISPLAY_ERROR( L"ONLY BCT_PERCENT" );
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeStatBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffDoubleAttackBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffDoubleAttackBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffDoubleAttackBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );

	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eChangeType ) );
	BehaviorFactor.m_vecValues.push_back( m_fRate );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 스탯을 변경하는 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffDoubleAttackBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	if ( kFactor_.m_vecValues.size() >= static_cast<UINT>( FO_END ) )
	{
		m_eChangeType	= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, kFactor_.m_vecValues[FO_CHANGE_TYPE] );
		m_fRate = kFactor_.m_vecValues[FO_VALUE];
		
		pGameUnit_->CreateAndInsertDoubleAttackByBuff( kBuffFactor_.m_BuffIdentity, m_fRate );
	}
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffDoubleAttackBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseDoubleAttackByBuff( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeAttackSphereScaleBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( !DidStart() )
		{
			BOOST_FOREACH( const AttackSphereNameAndScale& attackSphereNameAndScale, m_vecAttackSphereNameAndScale )
			{
				float fScale = pGameUnit_->GetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName );
				fScale += attackSphereNameAndScale.m_fScale;

				if ( fScale < 0.0f )	/// fScale이 마이너스면 정상이 아니라고 판단하여
					fScale = 1.0f;		/// 1.0 (기본값)으로 셋팅

				pGameUnit_->SetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName, fScale );
			}
		}

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeUnitScaleBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : ParsingBehavior
	@brief : 어택박스 크기를 변경 시키는 버프 행동을 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffChangeAttackSphereScaleBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	int iIndex = 1;
	while ( luaManager_.BeginTable( iIndex ) )
	{
		AttackSphereNameAndScale attackSphereNameAndScale;

		LUA_GET_VALUE( luaManager_, "ATTACK_SPHERE_NAME", attackSphereNameAndScale.m_wstrAttackSphereName, L"" );
		LUA_GET_VALUE( luaManager_, "SCALE", attackSphereNameAndScale.m_fScale, 1.0f );

		m_vecAttackSphereNameAndScale.push_back( attackSphereNameAndScale );
		++iIndex;

        luaManager_.EndTable();
	}

	return true;
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeAttackSphereScaleBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeAttackSphereScaleBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffChangeAttackSphereScaleBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 스탯을 변경하는 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeAttackSphereScaleBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// 오작동 할 것으로 예상 됨... // 김현철 // 2013-07-18
	if ( !DidStart() )
	{
		BOOST_FOREACH( const AttackSphereNameAndScale& attackSphereNameAndScale, m_vecAttackSphereNameAndScale )
		{
			float fScale = pGameUnit_->GetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName );
			fScale += attackSphereNameAndScale.m_fScale;

			if ( fScale < 0.0f )	/// fScale이 마이너스면 정상이 아니라고 판단하여
				fScale = 1.0f;		/// 1.0 (기본값)으로 셋팅

			pGameUnit_->SetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName, fScale );
		}
	}
	
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeAttackSphereScaleBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	BOOST_FOREACH( const AttackSphereNameAndScale& attackSphereNameAndScale, m_vecAttackSphereNameAndScale )
	{
		float fScale = pGameUnit_->GetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName );
		fScale -= attackSphereNameAndScale.m_fScale;

		if ( fScale < 0.0f )	/// fScale이 마이너스면 정상이 아니라고 판단하여
			fScale = 1.0f;		/// 1.0 (기본값)으로 셋팅

		pGameUnit_->SetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName, fScale );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffAddSkillLevelBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( !DidStart() )	/// 일단 중첩등 허용 안함
		{
			m_iAddSkillLevel = static_cast<int>( (*pBehaviorFactor)[FO_SKILL_LEVEL] );
			pGameUnit_->CreateAndInsertAddSkillLevelByBuff( pBuffTemplet_->GetBuffIdentity(), m_iAddSkillLevel );
			pGameUnit_->UpdatePassiveAndActiveSkillState();
		}

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffAddSkillLevelBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffAddSkillLevelBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffAddSkillLevelBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffAddSkillLevelBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_iAddSkillLevel ) );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 스탯을 변경하는 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffAddSkillLevelBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_iAddSkillLevel = static_cast<int>( kFactor_.m_vecValues[FO_SKILL_LEVEL] );
	pGameUnit_->CreateAndInsertAddSkillLevelByBuff( kBuffFactor_.m_BuffIdentity, m_iAddSkillLevel );
	pGameUnit_->UpdatePassiveAndActiveSkillState();
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffAddSkillLevelBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseAddSkillLevelByBuff( pBuffTemplet_->GetBuffIdentity() );
	pGameUnit_->UpdatePassiveAndActiveSkillState();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환

/** @function : ParsingBehavior
	@brief : 타입에 따른 공격력 수치를 변경 시키는 버프 행동을 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffReflectMagicBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	return true;
}

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffReflectMagicBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
			if ( BCT_PERCENT == m_eChangeType )	/// 무조건 배율
			{
				if ( DidStart() )	/// 기존에 작동 중인 것이었으면 기존의 효과 삭제
					pGameUnit_->EraseReflectMagicByBuff( pBuffTemplet_->GetBuffIdentity() );

				/// 효과 설정
				m_fResultValue = (*pBehaviorFactor)[FO_VALUE];

				pGameUnit_->CreateAndInsertReflectMagicByBuff( pBuffTemplet_->GetBuffIdentity(), m_fResultValue );

				return true;
			}
			else
				return DISPLAY_ERROR( L"ONLY BCT_PERCENT" );
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeStatBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffReflectMagicBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffReflectMagicBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffReflectMagicBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eChangeType ) );
	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 스탯을 변경하는 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffReflectMagicBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_eChangeType	= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, kFactor_.m_vecValues[FO_CHANGE_TYPE] );
	m_fResultValue = kFactor_.m_vecValues[FO_VALUE];

	pGameUnit_->CreateAndInsertReflectMagicByBuff( kBuffFactor_.m_BuffIdentity, m_fResultValue );
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffReflectMagicBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseReflectMagicByBuff( pBuffTemplet_->GetBuffIdentity() );
}

#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
/** @function : ParsingBehavior
	@brief : 타입에 따른 공격력 수치를 변경 시키는 버프 행동을 정의한 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffChangeConsumeMpRate::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	return true;
}

/** @function : SetFactor
	@brief : BuffFactor에 담겨진 값으로 Templet 셋팅
	@param : 버프요소(BuffFactor_), 버프를 가지고 있을 게임유닛(pGameUnit_), 기존에 가지고 있던 또는 새로운 버프템플릿 포인터(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeConsumeMpRate::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// 버프가 가지고 있는 여러 요소 중 현재 클래스에 해당 하는 정보를 얻어옴
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		float fMPConsume = 0.f;
		m_uiSkillId = static_cast<UINT>( (*pBehaviorFactor)[FO_SKILL_ID] );
		
		if ( 0 > m_uiSkillId )
			return DISPLAY_ERROR( L"SKILL_ID doesn't exist" );

		if ( pGameUnit_->GetGameUnitType() == CX2GameUnit::GUT_USER )
		{
			CX2GUUser* pUser = reinterpret_cast<CX2GUUser*>( pGameUnit_ );

			if ( NULL == pUser->GetUnit() )
				return false;

			const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SKILL_ID( m_uiSkillId ) ) );
						
			if ( DidStart() )
			{
				m_fResultValue = m_fResultValue + m_fInitMp * (*pBehaviorFactor)[FO_VALUE];
			}
			else
			{	/// 중첩, 교체가 아니라면(첫시작 또는 중복)
				m_fInitMp = pUser->GetActualMPConsume( CX2SkillTree::SKILL_ID( m_uiSkillId ), iSkillTempletLevel );
				m_fResultValue = m_fInitMp + m_fInitMp * (*pBehaviorFactor)[FO_VALUE];
			}
		}

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffCreateBuffFactortBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeConsumeMpRate::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeConsumeMpRate( *this ) );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffChangeConsumeMpRate::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
	
	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eChangeType ) );
	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_uiSkillId ) );
	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 다른 유닛에게 지정된 버프 또는 디버프를 적용할 수 있는 행동 템플릿을 셋팅
	@param : 서버로 부터 받은 행동요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeConsumeMpRate::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_eChangeType	= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, kFactor_.m_vecValues[FO_CHANGE_TYPE] );
	m_uiSkillId		= static_cast<UINT>( kFactor_[FO_SKILL_ID] );
	m_fResultValue	= kFactor_.m_vecValues[FO_VALUE];
}

/** @function : DoFinish
	@brief : 버프가 종료됬을 때 정리해야하는 행동들을 처리
	@param : @param : 해당 버프에 걸렸던 게임유닛(pGameUnit_), 버프템플릿(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeConsumeMpRate::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseTemporaryBuffFactor( pBuffTemplet_->GetBuffIdentity() );
}
#endif //FIX_SKILL_BALANCE_AISHA_LENA
/**@file : X2BuffBehaviorTemplet.cpp
   @breif : 버프의 행동을 정의하는 Templet 파일
*/

#pragma once

#include "StdAfx.h"
#include ".\X2BuffDisplayerTemplet.h"

#define CASE_DISPLAYER_TEMPLET( class_, type_ ) \
case type_: \
	{ \
	CX2BuffDisplayerTempletPtr ptrDisplayerTemplet = class_::CreateBuffDisplayerTempletPtr(); \
	if ( NULL != ptrDisplayerTemplet && ptrDisplayerTemplet->ParsingDisplayerTemplateMethod( luaManager_, STRINGIZEA2(type_) ) ) \
		{ \
		ptrDisplayerTemplet->SetType( type_ ); \
		vecBuffDisplayerTempletPtr_.push_back( ptrDisplayerTemplet ); \
		} \
		else \
		return DISPLAY_ERROR( STRINGIZE2(type_) ); \
	} break; \

/** @function : ParsingCombinationDisplayer
	@brief : 버프의 표현이 어떤 종류로 구성되어 있는지 알아내고 구성된 타입별로 파싱
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_), 파싱성공한 BehaviorTempletPtr을 담을 vector(vecBuffBehaviorTempletPtr_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*static*/ bool CX2BuffDisplayerTemplet::ParsingCombinationDisplayer( KLuaManager& luaManager_, OUT vector<CX2BuffDisplayerTempletPtr>& vecBuffDisplayerTempletPtr_ )
{

	vector<UINT> vecDisplayerType;

#ifdef EXCEPTION_BUFF_FACTOR
	bool bExceptionCase = true;
#endif // EXCEPTION_BUFF_FACTOR
#ifdef EXCEPTION_BUFF_FACTOR_VER2
	bool bExceptionCase = true;
#endif // EXCEPTION_BUFF_FACTOR_VER2
	
	/// Displayer가 어떤 표현요소들의 조함으로 이루어 졌는지 얻어옴
	if ( CX2BuffTempletManager::GetCombination( luaManager_, vecDisplayerType ) )
	{
		vector<UINT>::iterator vItr = vecDisplayerType.begin();
		while ( vecDisplayerType.end() != vItr )		/// 조합에 포함된 행동요소들을 파싱
		{

#ifdef EXCEPTION_BUFF_FACTOR
			if(static_cast<BUFF_DISPLAYER_TYPE>( *vItr ) != BDT_EFFECT_SET_CREATE_GAP && 
				static_cast<BUFF_DISPLAYER_TYPE>( *vItr ) != BDT_EFFECT_SET_NORMAL_ATTACK)
				bExceptionCase = false;
#endif // EXCEPTION_BUFF_FACTOR
#ifdef EXCEPTION_BUFF_FACTOR_VER2
			if( static_cast<BUFF_DISPLAYER_TYPE>( *vItr ) != BDT_EFFECT_SET_ONLY_MY_CLASS )
				bExceptionCase = false;
#endif // EXCEPTION_BUFF_FACTOR_VER2
			
			switch ( static_cast<BUFF_DISPLAYER_TYPE>( *vItr ) )
			{
			CASE_DISPLAYER_TEMPLET( CX2BuffRenderParamDisplayerTemplet, BDT_RENDER_PARAM )
			CASE_DISPLAYER_TEMPLET( CX2BuffRenderParamByUnitTypeDisplayerTemplet, BDT_RENDER_PARAM_BY_UNIT_TYPE )
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetDisplayerTemplet, BDT_EFFECT_SET )
			CASE_DISPLAYER_TEMPLET( CX2BuffUnitSlashTraceDisplayerTemplet, BDT_UNIT_SLASH_TRACE )
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetImpactPointDisplayerTemplet, BDT_EFFECT_SET_MELEE_IMPACT_POINT )
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetImpactPointDisplayerTemplet, BDT_EFFECT_SET_HITTED )
			CASE_DISPLAYER_TEMPLET( CX2BuffWeaponParticleDisplayerTemplet, BDT_WEAPON_PARTICLE )
			CASE_DISPLAYER_TEMPLET( CX2BuffAfterImageDisplayerTemplet, BDT_AFTER_IMAGE )
			CASE_DISPLAYER_TEMPLET( CX2BuffSoundDisplayTemplet, BDT_SOUND )
#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE 
			CASE_DISPLAYER_TEMPLET( CX2BuffWeaponAfterImageDisplayerTemplet, BDT_WEAPON_AFTER_IMAGE )
#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

//#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-10	// 해외팀 주석 처리
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetCreateGapDisplayerTemplet, BDT_EFFECT_SET_CREATE_GAP )
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetImpactPointDisplayerTemplet, BDT_EFFECT_SET_NORMAL_ATTACK )
//#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef HAMEL_SECRET_DUNGEON // 김태환
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetOnlyMyClassDisplayerTemplet, BDT_EFFECT_SET_ONLY_MY_CLASS )
#endif // HAMEL_SECRET_DUNGEON

#ifdef EXCEPTION_BUFF_FACTOR
			case BDT_EFFECT_SET_CREATE_GAP:
			case BDT_EFFECT_SET_NORMAL_ATTACK:
				++vItr;
				continue;
#endif // EXCEPTION_BUFF_FACTOR
			default:
				return DISPLAY_ERROR( L"Unknown Displayer Templet Type" );	/// false 리턴
				break;
			}

			++vItr;
		}
	}

	if ( vecBuffDisplayerTempletPtr_.empty() 
#ifdef EXCEPTION_BUFF_FACTOR
			&& !bExceptionCase 
#endif // EXCEPTION_BUFF_FACTOR
#ifdef EXCEPTION_BUFF_FACTOR_VER2
			&& !bExceptionCase
#endif EXCEPTION_BUFF_FACTOR_VER2
			)
	{
		return DISPLAY_ERROR( L"DISPLAYER Templet is empty" );
	}
	else
		return true;
}

/** @function : ParsingDisplayerTemplateMethod
	@brief : 각 파생클래스에서 구현된 ParsingDisplayer를 똑같은 전후 처리 사이에서 호출하기 위한 템플릿메소드
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_), 파싱성공한 DisplayerTempletPtr을 담을 vector(vecBuffDisplayerTempletPtr_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
bool CX2BuffDisplayerTemplet::ParsingDisplayerTemplateMethod( KLuaManager& luaManager_, const char* pszTableNameUTF8_ )
{
	if ( luaManager_.BeginTable( pszTableNameUTF8_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		return ParsingDisplayer( luaManager_ );
	}
	else
		return DISPLAY_ERROR( pszTableNameUTF8_ );	
}

/** @function : ParsingDisplayer
	@brief : 게임유닛에 적용할 RenderParam 변경 정보를 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffRenderParamDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager_, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE_ENUM( luaManager_, "RENDER_TYPE", m_stBuffRenderParam.m_eRenderType, CKTDGXRenderer::RENDER_TYPE, CKTDGXRenderer::RT_REAL_COLOR );
	LUA_GET_VALUE_ENUM( luaManager_, "CARTOON_TEX_TYPE", m_stBuffRenderParam.m_eCartoonTexType, CKTDGXRenderer::CARTOON_TEX_TYPE, CKTDGXRenderer::CTT_NORMAL );
	LUA_GET_VALUE( luaManager_, "OUT_LINE_WIDE", m_stBuffRenderParam.m_fOutLineWide, 0.0f );

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR_OUT_LINE", m_stBuffRenderParam.m_d3dxColorOutLine, D3DXCOLOR(1,1,1,1) );
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR", m_stBuffRenderParam.m_d3dxColor, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	m_stBuffRenderParam.m_d3dxColorOutLine 
		= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR_OUT_LINE" );	
	m_stBuffRenderParam.m_d3dxColor	
		= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR" );	
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE( luaManager_, "ALPHA_BLEND", m_stBuffRenderParam.m_bAlphaBlend, false );
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	LUA_GET_VALUE( luaManager_, "FLICKER_COLOR_GAP", m_stBuffRenderParam.m_fFlickerColorGap, 0.f );
#endif //SERV_ADD_LUNATIC_PSYKER

	return true;
}

/** @function : StartDisplayer
	@brief : RenderParam 변경을 시작 하는 함수
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffRenderParamDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )	/// 중첩 된 것이라면 실행하지 않는다.
		pGameUnit_->CreateAndInsertRenderParamByBuff( pBuffTemplet_->GetBuffIdentity(), m_stBuffRenderParam );
}

/** @function : GetClonePtr
	@brief : 템플릿의 클론 스마트 포인터를 반환
	@return : 클론 스마트포인터(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffRenderParamDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffRenderParamDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : 버프에의해 적용된 RenderParam 변경을 해제하는 합수
	@param : 표현 종료를 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffRenderParamDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseRenderParamByBuff( pBuffTemplet_->GetBuffIdentity() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingDisplayer
	@brief : 타입별로 다른 RenderParam을 게임유닛에 적용할 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffRenderParamByUnitTypeDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	int iTableIndex = 1;

	/// 각 유닛 타입별로 RenderParam 셋팅을 읽어들임
	while ( luaManager_.BeginTable( iTableIndex ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		TableBind( &luaManager_, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

		StBuffRenderParamPtr ptrStBuffRenderParam( new StBuffRenderParam );
		if ( NULL != ptrStBuffRenderParam )
		{
			CX2Unit::UNIT_TYPE eUnitType = CX2Unit::UT_NONE;
			LUA_GET_VALUE_ENUM( luaManager_, "UNIT_TYPE", eUnitType, CX2Unit::UNIT_TYPE, CX2Unit::UT_NONE );

			LUA_GET_VALUE_ENUM( luaManager_, "RENDER_TYPE", ptrStBuffRenderParam->m_eRenderType, CKTDGXRenderer::RENDER_TYPE, CKTDGXRenderer::RT_REAL_COLOR );
			LUA_GET_VALUE_ENUM( luaManager_, "CARTOON_TEX_TYPE", ptrStBuffRenderParam->m_eCartoonTexType, CKTDGXRenderer::CARTOON_TEX_TYPE, CKTDGXRenderer::CTT_NORMAL );
			LUA_GET_VALUE( luaManager_, "OUT_LINE_WIDE", ptrStBuffRenderParam->m_fOutLineWide, 0.0f );

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
            LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR_OUT_LINE", ptrStBuffRenderParam->m_d3dxColorOutLine, D3DXCOLOR(1,1,1,1) );
            LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR", ptrStBuffRenderParam->m_d3dxColor, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
			ptrStBuffRenderParam->m_d3dxColorOutLine 
				= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR_OUT_LINE" );	
			ptrStBuffRenderParam->m_d3dxColor	
				= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR" );	
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

			LUA_GET_VALUE( luaManager_, "ALPHA_BLEND", ptrStBuffRenderParam->m_bAlphaBlend, false );

			pair<map<CX2Unit::UNIT_TYPE, StBuffRenderParamPtr>::iterator, bool> pairReturn 
				= m_mapStBuffRenderParamPtr.insert( make_pair( eUnitType, ptrStBuffRenderParam ) );

			/// 같은 키값을 가지고 있는 것이 이미 존재하면 실패처리
			if ( false == pairReturn.second )
				return DISPLAY_ERROR( L"UNIT TYPE already existed" );
		}	
		++iTableIndex;
	}		

	/// 공통 RenderParam 값이 없으면 안됨!
	if ( m_mapStBuffRenderParamPtr.end() == m_mapStBuffRenderParamPtr.find( CX2Unit::UT_NONE ) )
		return DISPLAY_ERROR( L"UT_NONE Type's RenderParam Must exist" );
	
	return true;
}

/** @function : StartDisplayer
	@brief : 유닛의 타입별 RenderParam을 적용하는 표현요소 시작
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffRenderParamByUnitTypeDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )	/// 중첩 된 것이라면 실행하지 않는다.
		pGameUnit_->CreateAndInsertRenderParamByBuffUnitType( pBuffTemplet_->GetBuffIdentity(), m_mapStBuffRenderParamPtr );
}

/** @function : GetClonePtr
	@brief : 템플릿의 클론 스마트 포인터를 반환
	@return : 클론 스마트포인터(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffRenderParamByUnitTypeDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffRenderParamByUnitTypeDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : 해당 버프에 의한 RenderParam 적용 해제
	@param : 표현 종료를 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffRenderParamByUnitTypeDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseRenderParamByBuff( pBuffTemplet_->GetBuffIdentity() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingDisplayer
	@brief : 게임유닛에 적용할 이펙트셋 명을 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffEffectSetDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
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
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 교체 되는 타입의 버프일 경우 해당 Flag 가 True 일 때, 이펙트 셋을 다시 뿌려준다.
	LUA_GET_VALUE( luaManager_, "REPLAY_EFFECT_SET_ACCUMULATION_LIMIT1", m_bIsReplayEffectSetAccumulationType1, false );
	++iIndex;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	
	if ( 1 != iIndex )	
		return true;
	else
		return false;
}

/** @function : StartDisplayer
	@brief : 버프의 표현요소로 이펙트셋을 Play시키는 함수
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game )
	{
		const UINT uiAccumulationLimit = pBuffTemplet_->GetAccumulationLimit();
		if ( uiAccumulationLimit > 1 && m_vecWstrEffectSetName.size() > 1 )	/// 중첩 가능한 것이고 이펙트가 중첩 별로 있다면
		{
			/// 기존의 이펙트 삭제 (레퍼런스로 실행해야함)
			BOOST_FOREACH( CX2EffectSet::Handle& hHandle, m_vecHandleEffectSet )
			{
				if ( INVALID_EFFECTSET_HANDLE != hHandle )
					g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
			}

			const UINT uiIndexPlayEffectSet = pBuffTemplet_->GetAccumulationCountNow() - 1;
			if ( uiIndexPlayEffectSet < m_vecWstrEffectSetName.size() )
			{
				m_vecHandleEffectSet[uiIndexPlayEffectSet] =
					g_pX2Game->GetEffectSet()->PlayEffectSet( m_vecWstrEffectSetName[uiIndexPlayEffectSet], pGameUnit_,
					NULL, ( 0 < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() );
			}

		}
		else
		{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// 교체 되는 타입의 버프일 경우 해당 Flag 가 True 일 때, 이펙트 셋을 다시 뿌려준다.
			if ( ( !DidStart() || m_bIsReplayEffectSetAccumulationType1 == true ) && !m_vecWstrEffectSetName.empty() )
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			if ( !DidStart() && !m_vecWstrEffectSetName.empty() )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				if ( true == m_bIsReplayEffectSetAccumulationType1 )
				{
					if ( INVALID_EFFECTSET_HANDLE != m_vecHandleEffectSet[0] )
						g_pX2Game->GetEffectSet()->StopEffectSet( m_vecHandleEffectSet[0] );
				}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				m_vecHandleEffectSet[0] =
					g_pX2Game->GetEffectSet()->PlayEffectSet( m_vecWstrEffectSetName[0], pGameUnit_,
					NULL, ( 0 < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() );
			}
		}
	}
}

/** @function : GetClonePtr
	@brief : 템플릿의 클론 스마트 포인터를 반환
	@return : 클론 스마트포인터(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffEffectSetDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : 버프에 의해 출력 중인 EffectSet을 종료하는 함수
	@param : 표현 종료를 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game )
	{
		BOOST_FOREACH( CX2EffectSet::Handle& hHandle, m_vecHandleEffectSet )
		{
			if ( INVALID_EFFECTSET_HANDLE != hHandle )
				g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : 버프의 표현요소로 유닛에 붙은 트레이스를 시작시키는 함수
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffUnitSlashTraceDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )	/// 중첩 된 것이라면 실행하지 않는다.
	{
		//pGameUnit_->AddUnitSlashData( m_eSlashTraceCondition );
		pGameUnit_->SetUnitSlashTraceConditionToDo( static_cast<CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION>( m_eSlashTraceCondition ) );
	}
}

/** @function : GetClonePtr
	@brief : 템플릿의 클론 스마트 포인터를 반환
	@return : 클론 스마트포인터(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffUnitSlashTraceDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffUnitSlashTraceDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : 버프에 의해 출력 중인 Trace를 종료하는 함수
	@param : 표현 종료를 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffUnitSlashTraceDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
	{
		pGameUnit_->SetUnitSlashTraceConditionToDo( CX2UnitSlashTraceManager::STC_NONE );
	}
}

/** @function : ParsingDisplayer
	@brief : 출력할 유닛컨디션 종류 파싱
	@param : 루아매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
/*virtual*/ bool CX2BuffUnitSlashTraceDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_ENUM( luaManager_, "SLASH_TRACE_CONDITION", m_eSlashTraceCondition, CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION, CX2UnitSlashTraceManager::STC_NONE );
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : 버프의 표현요소로 타격점에 출력될 이펙트셋을 셋팅
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetImpactPointDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )
		StartDisplayerByType( pGameUnit_, pBuffTemplet_ );
}

/** @function : GetClonePtr
	@brief : 템플릿의 클론 스마트 포인터를 반환
	@return : 클론 스마트포인터(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffEffectSetImpactPointDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetImpactPointDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : 버프에 의해 셋팅된 타격점 이펙트셋을 해제
	@param : 표현 종료를 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetImpactPointDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
		DoFinishByType( pGameUnit_, pBuffTemplet_ );
}

/** @function : ParsingDisplayer
	@brief : 출력할 타격점 이펙트셋명 파싱
	@param : 루아매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
/*virtual*/ bool CX2BuffEffectSetImpactPointDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	if ( luaManager_.GetValue( 1, m_wstrEffectSetName ) )
	{
		luaManager_.GetValue( 2, m_wstrSoundName );		/// 사운드는 없을 수도 있음

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09
		luaManager_.GetValue( 3, m_fCreateGap );		///  생성 간격 또한 없을 수도 있음
#endif // SERV_NEW_DEFENCE_DUNGEON

		return true;
	}
	else
		return false;
}

/** @function : StartDisplayerByType
	@brief : 타입에 의해 이펙트셋을 선택 적용
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
void CX2BuffEffectSetImpactPointDisplayerTemplet::StartDisplayerByType( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const WCHAR* pszSoundName	= ( m_wstrSoundName.empty() ? L"" : m_wstrSoundName.c_str() );

	switch ( GetType() )
	{
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09		생성 간격 추가
		case BDT_EFFECT_SET_MELEE_IMPACT_POINT:		/// 근접 공격 타격시 이펙트 셋 생성
			pGameUnit_->CreateAndInsertEffectSetImpactPointByBuff( pBuffTemplet_->GetBuffIdentity(), m_wstrEffectSetName.c_str(), pszSoundName, m_fCreateGap );
			break;

		case BDT_EFFECT_SET_HITTED:					/// 피격시 이펙트 셋 생성
			pGameUnit_->CreateAndInsertEffectSetHittedByBuff( pBuffTemplet_->GetBuffIdentity(), m_wstrEffectSetName.c_str(), pszSoundName, m_fCreateGap );
			break;

		case BDT_EFFECT_SET_NORMAL_ATTACK:			/// 일반 공격 타격시 이펙트 셋 생성
			pGameUnit_->CreateAndInsertEffectSetNormalAttackByBuff( pBuffTemplet_->GetBuffIdentity(), m_wstrEffectSetName.c_str(), pszSoundName, m_fCreateGap );
			break;
#else // SERV_NEW_DEFENCE_DUNGEON
		case BDT_EFFECT_SET_MELEE_IMPACT_POINT:
			pGameUnit_->CreateAndInsertEffectSetImpactPointByBuff( pBuffTemplet_->GetBuffIdentity(), m_wstrEffectSetName.c_str(), pszSoundName );
			break;

		case BDT_EFFECT_SET_HITTED:
			pGameUnit_->CreateAndInsertEffectSetHittedByBuff( pBuffTemplet_->GetBuffIdentity(), m_wstrEffectSetName.c_str(), pszSoundName );
			break;
#endif // SERV_NEW_DEFENCE_DUNGEON

	default:
		break;
	}
}

/** @function : DoFinishByType
	@brief : 타입에 의해 이펙트셋을 선택 해제
	@param : 표현 종료를 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
void CX2BuffEffectSetImpactPointDisplayerTemplet::DoFinishByType( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	switch ( GetType() )
	{
	case BDT_EFFECT_SET_MELEE_IMPACT_POINT:	/// 근접 공격 타격시 이펙트 생성 버프 삭제
		pGameUnit_->EraseEffectSetImpactPointByBuff( pBuffTemplet_->GetBuffIdentity() );
		break;

	case BDT_EFFECT_SET_HITTED:				/// 피격시시 이펙트 생성 버프 삭제
		pGameUnit_->EraseEffectSetHittedByBuff( pBuffTemplet_->GetBuffIdentity() );
		break;

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-14
	case BDT_EFFECT_SET_NORMAL_ATTACK:		/// 일반 공격 타격시 이펙트 생성 버프 삭제
		pGameUnit_->EraseEffectSetNormalAttackByBuff( pBuffTemplet_->GetBuffIdentity() );
		break;
#endif // SERV_NEW_DEFENCE_DUNGEON

	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : 버프의 표현요소로 Weapon 파티클 생성
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffWeaponParticleDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game && !DidStart() )
		StartDisplayerByBuffTempletID( pGameUnit_, pBuffTemplet_ );
}

/** @function : GetClonePtr
	@brief : 템플릿의 클론 스마트 포인터를 반환
	@return : 클론 스마트포인터(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffWeaponParticleDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffWeaponParticleDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : 버프에 의해 출력 중인 Weapon 파티클을 종료
	@param : 표현 종료를 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffWeaponParticleDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game && DidStart() )
	{
		CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();
		BOOST_FOREACH( CKTDGParticleSystem::CParticleEventSequenceHandle& hHandle, m_vecHandleParticle )
		{
			pMajorParticleSystem->DestroyInstanceHandle( hHandle );
		}
	}
}

/** @function : OnFrameMove
	@brief : Displayer의 OnFrameMove
	@param : 버프에 걸린 게임유닛(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffWeaponParticleDisplayerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffWeaponParticleDisplayerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    switch( m_eFrameMoveType )
    {
    case FMT_SWORD_FIRE:
        OnFrameMoveSwordFire( pGameUnit_ );
        break;
    case FMT_ENCHANT:
        OnFrameMoveSwordEnchant( pGameUnit_ );
        break;
    default:
        CX2BuffDisplayerTemplet::OnFrameMove( pGameUnit_, fElapsedTime_ );
    }
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_delegateOnFrameMove( pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
}

/** @function : StartDisplayerByBuffTempletID
	@brief : 버프의 표현요소로 버프템플릿아이디별로 Weapon에 붙을 파티클을 셋팅
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
void CX2BuffWeaponParticleDisplayerTemplet::StartDisplayerByBuffTempletID( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	switch ( pBuffTemplet_->GetBuffTempletID() )
	{
	case BTI_SI_SA_EMK_SWORD_FIRE:
		{
			CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();
			const UINT uiSize = m_vecWstrParticleName.size();
			for ( UINT uiIndex = 0; uiIndex < uiSize; ++uiIndex )
			{
				m_vecHandleParticle[uiIndex]
					= pMajorParticleSystem->CreateSequenceHandle( pGameUnit_,  m_vecWstrParticleName[uiIndex].c_str(), 0,0,0 );
			}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_eFrameMoveType = FMT_SWORD_FIRE;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_delegateOnFrameMove = DelegateOnFrameMoveByType::from_method<CX2BuffWeaponParticleDisplayerTemplet, &CX2BuffWeaponParticleDisplayerTemplet::OnFrameMoveSwordFire>( this );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		} break;

	case BTI_BUFF_SWORD_ENCHANT_ICE:
	case BTI_BUFF_SWORD_ENCHANT_FIRE:
	case BTI_BUFF_SWORD_ENCHANT_POISON:
		{
			CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();
			const UINT uiSize = m_vecWstrParticleName.size();
			for ( UINT uiIndex = 0; uiIndex < uiSize; ++uiIndex )
			{
				m_vecHandleParticle[uiIndex]
				= pMajorParticleSystem->CreateSequenceHandle( pGameUnit_,  m_vecWstrParticleName[uiIndex].c_str(), 0,0,0 );
			}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_eFrameMoveType = FMT_ENCHANT;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_delegateOnFrameMove = DelegateOnFrameMoveByType::from_method<CX2BuffWeaponParticleDisplayerTemplet, &CX2BuffWeaponParticleDisplayerTemplet::OnFrameMoveSwordEnchant>( this );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		} break;
	default:
		{
			// 오현빈 // CX2BuffWeaponParticleDisplayerTemplet 은 무조건 예외처리를 해주어야 합니다.
			DISPLAY_ERROR( L"Empty WeaponDisplayer OnFrameMove" );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_eFrameMoveType = FMT_DEFAULT;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_delegateOnFrameMove = DelegateOnFrameMoveByType::from_method<CX2BuffDisplayerTemplet, &CX2BuffDisplayerTemplet::OnFrameMove>( this );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}
		break;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    OnFrameMove( pGameUnit_, 0.f );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_delegateOnFrameMove( pGameUnit_ );	/// 위치등 셋팅으로 한번 미리 실행
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
}

/** @function : ParsingDisplayer
	@brief : 게임유닛에 적용할 Weapon 파티클 네임을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffWeaponParticleDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	int iIndex = 1;

	wstring wstrParticleName;
	while( luaManager_.GetValue( iIndex, wstrParticleName ) )
	{
		m_vecWstrParticleName.push_back( wstrParticleName );
		m_vecHandleParticle.push_back( INVALID_PARTICLE_SEQUENCE_HANDLE );
		wstrParticleName.resize(0);
		++iIndex;
	}

	if ( 1 != iIndex )	
		return true;
	else
		return false;
}

/** @function : OnFrameMoveSwordFire
	@brief : 버프가 소드파이어인 경우 실행될 OnFrameMove
	@param : 버프에 걸린 게임유닛(pGameUnit_)
*/
void CX2BuffWeaponParticleDisplayerTemplet::OnFrameMoveSwordFire( CX2GameUnit* pGameUnit_ )
{
	const UINT NUM_OF_SWORD_FIRE_PARTICLE = 3;

	ASSERT( m_vecHandleParticle.size() == NUM_OF_SWORD_FIRE_PARTICLE );
	if ( m_vecHandleParticle.size() == NUM_OF_SWORD_FIRE_PARTICLE )	/// 소드파이어는 파티클이 3개
	{
		D3DXVECTOR3 vParticlePos[NUM_OF_SWORD_FIRE_PARTICLE] = {
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_START0", 0 ),
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_END0", 0 ),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f )
		};

		vParticlePos[2] = ( vParticlePos[0] - vParticlePos[1] ) * 0.5f + vParticlePos[1];

		CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();
		for ( UINT uiIndex = 0; uiIndex < NUM_OF_SWORD_FIRE_PARTICLE; ++uiIndex )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq 
				= pMajorParticleSystem->GetInstanceSequence( m_vecHandleParticle[uiIndex] );
			if ( NULL != pSeq )
				pSeq->SetPosition( vParticlePos[uiIndex] );
		}
	}
}
/** @function : OnFrameMoveSwordEnchant
	@brief : 버프가 소드인첸트인 경우 실행될 OnFrameMove
	@param : 버프에 걸린 게임유닛(pGameUnit_)
*/
void CX2BuffWeaponParticleDisplayerTemplet::OnFrameMoveSwordEnchant( CX2GameUnit* pGameUnit_ )
{
	const UINT NUM_OF_SWORD_ENCHANT_PARTICLE = 8;

	ASSERT( m_vecHandleParticle.size() == NUM_OF_SWORD_ENCHANT_PARTICLE );
	if ( m_vecHandleParticle.size() == NUM_OF_SWORD_ENCHANT_PARTICLE )	/// 소드인첸트는 파티클이 7개
	{
		D3DXVECTOR3 vParticlePos[NUM_OF_SWORD_ENCHANT_PARTICLE] = {
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_START0", 0 ),
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_START0", 0 ),
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_END0", 0 ),
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_END0", 0 ),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f )
		};

		vParticlePos[4] = vParticlePos[5] = vParticlePos[6] = vParticlePos[7] = ( vParticlePos[0] - vParticlePos[2] ) * 0.5f + vParticlePos[2];

		CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();
		for ( UINT uiIndex = 0; uiIndex < NUM_OF_SWORD_ENCHANT_PARTICLE; ++uiIndex )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq 
				= pMajorParticleSystem->GetInstanceSequence( m_vecHandleParticle[uiIndex] );
			if ( NULL != pSeq )
				pSeq->SetPosition( vParticlePos[uiIndex] );
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : 버프의 표현요소로 AfterImage 실행
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffAfterImageDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )
		pGameUnit_->CreateAndInsertAfterImageByBuff( pBuffTemplet_->GetBuffIdentity(), m_d3dxColor );
}

/** @function : GetClonePtr
	@brief : 템플릿의 클론 스마트 포인터를 반환
	@return : 클론 스마트포인터(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffAfterImageDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffAfterImageDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : 버프에의해 적용된 AfterImage 종료
	@param : 표현 종료를 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffAfterImageDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
		pGameUnit_->EraseAfterImageByBuff( pBuffTemplet_->GetBuffIdentity() );
}

/** @function : ParsingDisplayer
	@brief : 게임유닛에 적용할 AfterImage 색깔 파싱
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffAfterImageDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager_, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR", m_d3dxColor, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	m_d3dxColor	= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR" );	
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	return true;
}

#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : 버프의 표현요소로 무기의 AfterImage 실행
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffWeaponAfterImageDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )
		pGameUnit_->CreateAndInsertWeaponAfterImageByBuff( pBuffTemplet_->GetBuffIdentity(), m_d3dxColor );
}

/** @function : GetClonePtr
	@brief : 템플릿의 클론 스마트 포인터를 반환
	@return : 클론 스마트포인터(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffWeaponAfterImageDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffWeaponAfterImageDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : 버프에의해 적용된 무기의 AfterImage 종료
	@param : 표현 종료를 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffWeaponAfterImageDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
		pGameUnit_->EraseWeaponAfterImageByBuff( pBuffTemplet_->GetBuffIdentity() );
}

/** @function : ParsingDisplayer
	@brief : 게임유닛에 적용할 무기의 AfterImage 색깔 파싱
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffWeaponAfterImageDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager_, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR", m_d3dxColor, D3DXCOLOR(1,1,1,1) );	
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	m_d3dxColor	= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR" );	
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	return true;
}
#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : 버프의 표현요소로 사운드 출력
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffSoundDisplayTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->PlaySound( m_wstrSoundName.c_str() );
}

/** @function : GetClonePtr
	@brief : 템플릿의 클론 스마트 포인터를 반환
	@return : 클론 스마트포인터(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffSoundDisplayTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffSoundDisplayTemplet( *this ) );
}

/** @function : ParsingDisplayer
	@brief : 버프에 걸렸을 때 출력할 사운드명 파싱
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffSoundDisplayTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	if ( luaManager_.GetValue( 1, m_wstrSoundName ) )
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-12
/** @function : ParsingDisplayer
	@brief : 게임유닛에 적용할 이펙트셋 명과 생성 간격을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffEffectSetCreateGapDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	int iIndex = 1;
	
	wstring wstrEffectSetName;
	while( luaManager_.GetValue( iIndex, wstrEffectSetName ) )
	{
		int iCreateGap;

		if ( true == luaManager_.GetValue( iIndex + 1, iCreateGap ) )
		{
			ASSERT( 0 != iCreateGap );
			m_vecPairEffectSetNameAndGap.push_back( std::make_pair( wstrEffectSetName, static_cast<float>( iCreateGap ) ) );
		}
		else	/// 생성 쿨타임이 누락되었다!
		{
			ASSERT( !L"BDT_EFFECT_SET_CREATE_GAP : Create EffectSet Gap is Nothing !" );
			return false;
		}

		iIndex += 2;
	}
	
	if ( 1 != iIndex )	
		return true;
	else
		return false;
}

/** @function : StartDisplayer
	@brief : 버프의 표현요소로 이펙트셋을 Play시키는 함수
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetCreateGapDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game )
	{
		typedef std::pair< wstring, float > PairEffectSetNameAndGap;

		BOOST_FOREACH( const PairEffectSetNameAndGap& PairValue, m_vecPairEffectSetNameAndGap )
		{
			m_vecHandleEffectSet.push_back( g_pX2Game->GetEffectSet()->PlayEffectSet( PairValue.first, pGameUnit_,
				NULL, ( 0 < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() ) );

			CKTDXCheckElapsedTime CreateEffectSetGap = 0.f;
			CreateEffectSetGap.SetTargetTime( PairValue.second );
			m_vecCheckElapsedTimeForEffectSet.push_back( std::make_pair( PairValue.first, CreateEffectSetGap ) );
		}
	}
}

/** @function : GetClonePtr
	@brief : 템플릿의 클론 스마트 포인터를 반환
	@return : 클론 스마트포인터(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffEffectSetCreateGapDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetCreateGapDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : 버프에 의해 출력 중인 EffectSet을 종료하는 함수
	@param : 표현 종료를 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetCreateGapDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game )
	{
		BOOST_FOREACH( CX2EffectSet::Handle& hHandle, m_vecHandleEffectSet )
		{
			if ( INVALID_EFFECTSET_HANDLE != hHandle )
				g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
		}
	}
}

/** @function : OnFrameMove
	@brief : Displayer의 OnFrameMove
	@param : 버프에 걸린 게임유닛(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffEffectSetCreateGapDisplayerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffEffectSetCreateGapDisplayerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( NULL != g_pX2Game && CX2Game::GS_PLAY == g_pX2Game->GetGameState() )
	{
		typedef std::pair< wstring, CKTDXCheckElapsedTime > PairElapsedTimeForEffectSet;

		BOOST_FOREACH( PairElapsedTimeForEffectSet& PairValue, m_vecCheckElapsedTimeForEffectSet )
		{
			CKTDXCheckElapsedTime& CreateEffectSetGap = PairValue.second;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            CreateEffectSetGap.OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			CreateEffectSetGap.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

			if ( true == CreateEffectSetGap.CheckAndResetElapsedTime() )
			{
				m_vecHandleEffectSet.push_back( g_pX2Game->GetEffectSet()->PlayEffectSet( PairValue.first, pGameUnit_,
					NULL, ( 0 < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() ) );
			}
		}
	}
}

#endif // SERV_NEW_DEFENCE_DUNGEON

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HAMEL_SECRET_DUNGEON // 김태환

/** @function : ParsingDisplayer
	@brief : 게임유닛에 적용할 이펙트셋 명을 템플릿을 루아로 부터 읽어들이는 함수
	@param : 읽어들이려는 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*virtual*/ bool CX2BuffEffectSetOnlyMyClassDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	const int iMaxUnitNum = static_cast<int>( CX2Unit::UT_END ) - 1;		/// 최대 유닛 수

	/// 유닛수 만큼 순회
	for ( int i = 0; i < iMaxUnitNum; ++i )
	{
		wstring wstrEffectSetName = L"";	/// 이펙트셋 이름

		/// 유닛 타입 인덱스에 따라 이펙트셋 이름 파싱
		switch ( i )
		{
			case 0: LUA_GET_VALUE( luaManager_, "ELSWORD", wstrEffectSetName, L"" );break;
			case 1: LUA_GET_VALUE( luaManager_, "AISHA",	wstrEffectSetName, L"" );break;
			case 2: LUA_GET_VALUE( luaManager_, "RENA",	wstrEffectSetName, L"" );break;
			case 3: LUA_GET_VALUE( luaManager_, "RAVEN",	wstrEffectSetName, L"" );break;
			case 4: LUA_GET_VALUE( luaManager_, "EVE",		wstrEffectSetName, L"" );break;
			case 5: LUA_GET_VALUE( luaManager_, "CHUNG",	wstrEffectSetName, L"" );break;
			case 6: LUA_GET_VALUE( luaManager_, "ARA",		wstrEffectSetName, L"" );break;
			case 7: LUA_GET_VALUE( luaManager_, "ELESIS",	wstrEffectSetName, L"" );break;
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
			case 8: LUA_GET_VALUE( luaManager_, "NEW_CHARACTER",	wstrEffectSetName, L"" );break;
#endif //SERV_9TH_NEW_CHARACTER
			default: DISPLAY_ERROR( L"DISPLAYER EFFECTSET ONLY MY CLASS - UNKNOWN CLASS" ) break;
		}

		/// 저장
		m_vecEffectSetName.push_back( wstrEffectSetName );
	}

	/// 저장한 인자수가 최대 유닛 수가 아니라면, 오류로 설정
	if ( static_cast<int>( m_vecEffectSetName.size() ) != iMaxUnitNum )
	{
		DISPLAY_ERROR( L"DISPLAYER EFFECTSET ONLY MY CLASS - PARSING ERROR" );

		return false;
	}

	return true;
}

/** @function : StartDisplayer
	@brief : 버프의 표현요소로 이펙트셋을 Play시키는 함수
	@param : 표현을 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetOnlyMyClassDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game && 
        NULL != pGameUnit_ &&
		 CX2GameUnit::GUT_USER == pGameUnit_->GetGameUnitType() && 
		 NULL != static_cast<CX2GUUser*>(pGameUnit_)->GetUnit() )
	{
		if ( !DidStart() )	/// 처음 걸렸을 때
		{
			/// 기존의 이펙트 삭제 (레퍼런스로 실행해야함)
			if ( INVALID_EFFECTSET_HANDLE != m_hEffectSetHandle )
				g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffectSetHandle );

			/// 버프에 적용된 유닛의 타입
			int iUnitTypeIndex = static_cast<int>( static_cast<CX2GUUser*>(pGameUnit_)->GetUnit()->GetType() ) - 1;

			/// 컨테이너 내 유요한 인덱스일 때, 이펙트셋 적용
			if ( iUnitTypeIndex < static_cast<int>( m_vecEffectSetName.size() ) )
			{
				m_hEffectSetHandle =
					g_pX2Game->GetEffectSet()->PlayEffectSet( m_vecEffectSetName[iUnitTypeIndex], pGameUnit_,
					NULL, ( 0 < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() );

				/// 이펙트셋 생성 실패 했어요!!!
				if ( INVALID_EFFECTSET_HANDLE == m_hEffectSetHandle )
					DISPLAY_ERROR( L"DISPLAYER EFFECTSET ONLY MY CLASS - UNKNOWN EFFECTSET NAME" );
			}

		}
	}
}

/** @function : GetClonePtr
	@brief : 템플릿의 클론 스마트 포인터를 반환
	@return : 클론 스마트포인터(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffEffectSetOnlyMyClassDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetOnlyMyClassDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : 버프에 의해 출력 중인 EffectSet을 종료하는 함수
	@param : 표현 종료를 적용할 유닛(pGameUnit_), 버프 정보(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetOnlyMyClassDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game )
	{
		if ( INVALID_EFFECTSET_HANDLE != m_hEffectSetHandle )
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffectSetHandle );
	}
}

#endif // HAMEL_SECRET_DUNGEON
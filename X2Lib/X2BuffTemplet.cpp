/** @file : X2BuffTemplet.cpp
    @breif : 버프의 행동과 종료등을 가지고 있는 클래스 정의 파일
*/
#pragma once

#include "StdAfx.h"
#include ".\X2BuffTemplet.h"

/** @function : ParsingScript
	@brief : 버프 템플릿 스크립트을 읽어들임
	@param : 현재 스크립트의 루아매니저(luaManager_)
	@return : 파싱 성공 true, 실패 false 리턴
*/
bool CX2BuffTemplet::ParsingScript( KLuaManager& luaManager_ )
{
	/// 버프 아이디
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_TEMPLET_ID", m_BuffIdentity.m_eBuffTempletID, 
		BUFF_TEMPLET_ID, BTI_NONE, return DISPLAY_ERROR( L"BUFF_TEMPLET_ID" ) );

	/// 월드 여부
	LUA_GET_VALUE_RETURN( luaManager_, "IS_WORLD_BUFF", m_bWorldBuff, false, return DISPLAY_ERROR( L"IS_WORLD_BUFF" ) );

	/// 버프, 디버프 여부
	LUA_GET_VALUE_RETURN( luaManager_, "IS_DEBUF", m_bDeBuff, false, return DISPLAY_ERROR( L"IS_DEBUF" ) );

	/// 중복, 교체, 중첩 여부
	LUA_GET_VALUE_RETURN( luaManager_, "ACCUMULATION_LIMIT", m_uiAccumulationLimit, 0, return DISPLAY_ERROR( L"ACCUMULATION_LIMIT" ) );

	if ( 1 < m_uiAccumulationLimit )	/// 중첩 횟수가 정해져 있으면 (0 과 1은 중첩이 아님)
	{
		/// 중첩 배율을 읽어들임
		float fAccumulationMultiplier = 0.0f;
		LUA_GET_VALUE_RETURN( luaManager_, "ACCUMULATION_MULTIPLIER", fAccumulationMultiplier, 0, return DISPLAY_ERROR( L"ACCUMULATION_MULTIPLIER" ) );

		m_mapUnCommonMember.insert( make_pair( UCM_ACCUMULATION_MULTIPLIER, fAccumulationMultiplier ) );

		/// 현재 중첩 카운트
		m_mapUnCommonMember.insert( make_pair( UCM_ACCUMULATION_COUNT_NOW, 0.0f ) );
	}

	/// 저항 타입
	float fRegistType = 0.0f;
	LUA_GET_VALUE_RETURN( luaManager_, "REGIST_TYPE", fRegistType, 0.0f, return DISPLAY_ERROR( L"REGIST_TYPE" ) );

	/// 저항 타입이 ET_NONE(0) 이 아니면
	if ( 0.0f < fRegistType )
		m_mapUnCommonMember.insert( make_pair( UCM_REGIST_TYPE, fRegistType ) );


	/// 행동 요소 파싱
	if ( luaManager_.BeginTable( "BEHAVIOR" ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		if ( !CX2BuffBehaviorTemplet::ParsingCombinationBehavior( luaManager_, m_vecBehaviorPtr ) )
			return false;		
	}
	else
		return DISPLAY_ERROR( L"BEHAVIOR Templet doesn't exist" );


	/// 종료 요소 파싱
	if ( luaManager_.BeginTable( "FINALIZER" ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

			if ( !CX2BuffFinalizerTemplet::ParsingCombinationFinalizer( luaManager_, m_vecFinalizerPtr ) )
				return false;		
	}
	else
		return DISPLAY_ERROR( L"FINALIZER Templet doesn't exist" );

	/// 종료 요소 파싱
	if ( luaManager_.BeginTable( "DISPLAYER" ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

			if ( !CX2BuffDisplayerTemplet::ParsingCombinationDisplayer( luaManager_, m_vecDisplayerPtr ) )
				return false;		
	}
	// Displayer는 없을 수도 있으므로 else 판단을 하지 않는다.

#ifdef BUFF_ICON_UI
	LUA_GET_VALUE( luaManager_, "BUFF_ICON_TEXTURE_NAME", m_wstIconFileName, L"" );
	m_bUseBuffIcon = ( true == m_wstIconFileName.empty() ) ? false : true ;
	if( true == m_bUseBuffIcon )
	{
		LUA_GET_VALUE( luaManager_, "BUFF_ICON_TEXTURE_KEY",  m_wstIconKeyName,  L"" );
		ASSERT( m_wstIconKeyName != L"" );

		LUA_GET_VALUE( luaManager_, "BUFF_NAME",			  m_iBuffName,		 STR_ID_2674 );
		LUA_GET_VALUE( luaManager_, "BUFF_DESC",			  m_iBuffDesc,		 STR_ID_2606 );
	}
#endif //BUFF_ICON_UI
	return true;
}

/** @function : SetFactor
	@brief : 버프 요소를 받아서 버프 템플릿에 셋팅하고, 버프를 시작 시키는 함수
	@param : 버프 요소 (buffFactor_), 버프가 적용되야할 유닛(pGameUnit_)
	@return : 성공시 true, 실패시 false
*/
bool CX2BuffTemplet::SetFactor( const CX2BuffFactor& buffFactor_, CX2GameUnit* pGameUnit_ )
{
	/// 행동조건 셋팅
	BOOST_FOREACH( CX2BuffBehaviorTempletPtr ptrBehavior, m_vecBehaviorPtr )
	{
		if ( NULL != ptrBehavior && ptrBehavior->SetFactor( buffFactor_, pGameUnit_, this ) )
			ptrBehavior->SetStart( true );	/// 정상 실행
		else
			return false;
	}

	/// 종료조건 셋팅
	if ( false == SetFactorToOnlyFinalizer( buffFactor_, pGameUnit_ ) )
		return false;

	if ( GetAccumulationLimit() > 1 )	/// 중첩 되는 것이라면 위의 항목들을 실행 후 중첩 카운트 올려줌
		IncreaseAccumulationCountNow();

	/// 표현조건 셋팅
	BOOST_FOREACH( CX2BuffDisplayerTempletPtr ptrDisplayer, m_vecDisplayerPtr )
	{
		if ( NULL != ptrDisplayer )
		{
			ptrDisplayer->StartDisplayer( pGameUnit_, this );
			ptrDisplayer->SetStart( true );
		}
	}

#ifdef BUFF_ICON_UI		
	if( true == GetUseBuffIcon() )
	{
		CX2GageUI::BuffIcon BuffIconData( buffFactor_.GetBuffTempletID(), GetIconFileName(), 
			GetIconKeyName(), GetBuffName(), GetBuffDesc() );
		CX2GageManager::GetInstance()->PushBuff( pGameUnit_->GetUnitUID(), BuffIconData, IsDeBuff() );
	}
#endif //BUFF_ICON_UI

	return true;
}

/** @function : SetFactorFromPacket
	@brief : 서버로 부터 받은 버프요소로 버프템플릿을 실행 시킴
	@param : 서버로부터 받은 버프요소(kBuffFactor_), 버프가 적용될 게임유닛(pGameUnit_)
	@return : 성공(true), 실패(false)
*/
bool CX2BuffTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, CX2GameUnit* pGameUnit_ )
{
	SetUniqueNum( kBuffFactor_.m_BuffIdentity.m_uiUniqueNum );

	if ( GetAccumulationLimit() > 1 )	/// 중첩 가능한 것이면
	{
		const float fAccumulationCountNow = static_cast<float>( kBuffFactor_.m_ucAccumulationCountNow );
		
		pair<MAP_UN_COMMON_MEMBER::iterator, bool> pairReturn
			= m_mapUnCommonMember.insert( make_pair( UCM_ACCUMULATION_COUNT_NOW, fAccumulationCountNow ) ); 

		/// 기존에 키값이 존재 했다면
		if ( !pairReturn.second )
			pairReturn.first->second = fAccumulationCountNow;
	}
	
	/// 행동조건 셋팅
	BOOST_FOREACH( CX2BuffBehaviorTempletPtr ptrBehavior, m_vecBehaviorPtr )
	{
		if ( NULL != ptrBehavior && ptrBehavior->SetFactorFromPacketTemplateMothod( kBuffFactor_, pGameUnit_ ) )
			ptrBehavior->SetStart( true );	/// 정상 실행
		else
			return false;
	}

	/// 종료조건 셋팅
	BOOST_FOREACH( CX2BuffFinalizerTempletPtr ptrFinalizer, m_vecFinalizerPtr )
	{
		if ( NULL != ptrFinalizer && !ptrFinalizer->SetFactorFromPacketTemplateMothod( kBuffFactor_, pGameUnit_ ) )
			return false;
	}

	/// 표현조건 셋팅
	BOOST_FOREACH( CX2BuffDisplayerTempletPtr ptrDisplayer, m_vecDisplayerPtr )
	{
		if ( NULL != ptrDisplayer )
		{
			ptrDisplayer->StartDisplayer( pGameUnit_, this );
			ptrDisplayer->SetStart( true );
		}
	}

	return true;
}

/** @function : GetAccumulationCountNow
	@brief : 현재 몇 중첩인지 얻어옴
	@return : 현재 중첩 횟수 리턴(UINT, 중첩이 안되는 경우 0을 리턴)
*/
UINT CX2BuffTemplet::GetAccumulationCountNow() const
{
	if ( !m_mapUnCommonMember.empty() )
	{
 		MAP_UN_COMMON_MEMBER::const_iterator mItr = m_mapUnCommonMember.find( UCM_ACCUMULATION_COUNT_NOW );
 		if ( m_mapUnCommonMember.end() != mItr )
 			return static_cast<UINT>( mItr->second );
	}

	return 0;
}

/** @function : IncreaseAccumulationCountNow
	@brief : 중첩이 되는 경우 중첩 횟수 1증가
*/
void CX2BuffTemplet::IncreaseAccumulationCountNow()
{
	if ( !m_mapUnCommonMember.empty() )
	{
		MAP_UN_COMMON_MEMBER::iterator mItr = m_mapUnCommonMember.find( UCM_ACCUMULATION_COUNT_NOW );
		if ( m_mapUnCommonMember.end() != mItr )
			mItr->second += 1;
	}
}

/** @function : GetRegistType
	@brief : 적용될 저항 타입을 얻어옴
	@return : 저항타입(UINT)으로 주로 CX2EnchantItem::ENCHANT_TYPE으로 변환하여 사용
*/
UINT CX2BuffTemplet::GetRegistType() const
{
	if ( !m_mapUnCommonMember.empty() )
	{
		MAP_UN_COMMON_MEMBER::const_iterator mItr = m_mapUnCommonMember.find( UCM_REGIST_TYPE );
		if ( m_mapUnCommonMember.end() != mItr )
			return static_cast<UINT>( mItr->second );
	}

	return 0;
}

/** @function : GetAccumulationMultiflier
	@brief : 중첩 가능한 경우 중첩 배율을 얻어옴
	@return : 중첩 배율(float)
*/
float CX2BuffTemplet::GetAccumulationMultiflier() const
{
	if ( !m_mapUnCommonMember.empty() )
	{
		MAP_UN_COMMON_MEMBER::const_iterator mItr = m_mapUnCommonMember.find( UCM_ACCUMULATION_MULTIPLIER );
		if ( m_mapUnCommonMember.end() != mItr )
			return mItr->second;
	}

	DISPLAY_ERROR( L"UCM_ACCUMULATION_COUNT_NOW doesn't exist" );

	return 0.0f;
}

/** @function : OnFrameMoveBehavior
	@brief : 버프템플릿의 행동 요소들의 OnFrameMove 실행
	@param : 버프가 적용된 유닛(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffTemplet::OnFrameMoveBehavior( CX2GameUnit* pGameUnit_, float fElapsedTime )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffTemplet::OnFrameMoveBehavior( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	BOOST_FOREACH( CX2BuffBehaviorTempletPtr ptrBehavior, m_vecBehaviorPtr )
	{
		if ( NULL != ptrBehavior )
        {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            ptrBehavior->OnFrameMove( pGameUnit_, this, fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			ptrBehavior->OnFrameMove( pGameUnit_, this );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }
	}
}

/** @function : OnFrameMoveFinalizerAndCheckFinish
	@brief : 버프템플릿의 종료 요소들의 OnFrameMove 실행 및 종료여부를 판단
	@param : 버프가 적용된 유닛(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffTemplet::OnFrameMoveFinalizerAndCheckFinish( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffTemplet::OnFrameMoveFinalizerAndCheckFinish( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	BOOST_FOREACH( CX2BuffFinalizerTempletPtr ptrFinalizer, m_vecFinalizerPtr )
	{
		if ( NULL != ptrFinalizer )
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            ptrFinalizer->OnFrameMove( pGameUnit_, fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			ptrFinalizer->OnFrameMove( pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

			if ( ptrFinalizer->DidFinish( pGameUnit_ ) )
				SetDidFinish( true );
		}
	}
}


/** @function : OnFrameMoveDisplayer
	@brief : 버프템플릿의 표현 요소들의 OnFrameMove 실행
	@param : 버프가 적용된 유닛(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffTemplet::OnFrameMoveDisplayer( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffTemplet::OnFrameMoveDisplayer( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( !m_vecDisplayerPtr.empty() )
	{
		BOOST_FOREACH( CX2BuffDisplayerTempletPtr ptrDisplayer, m_vecDisplayerPtr )
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            ptrDisplayer->OnFrameMove( pGameUnit_, fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			ptrDisplayer->OnFrameMove( pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}
	}
}

/** @function : DoFinishBehavior
	@brief : 현재 실행되고 있는 버프가 종료될 때 템플릿의 행동요소가 종료될 때 수행되야하는 함수들을 실행
	@param : 버프가 적용중인 게임유닛(pGameUnit_)
*/
void CX2BuffTemplet::DoFinishBehavior( CX2GameUnit* pGameUnit_ )
{
	BOOST_FOREACH( CX2BuffBehaviorTempletPtr ptrBehavior, m_vecBehaviorPtr )
	{
		ptrBehavior->DoFinish( pGameUnit_, this );
	}
}

/** @function : DoFinishBehavior
	@brief : 현재 실행되고 있는 템플릿의 행동요소가 종료될 때 수행되야하는 함수들을 실행
	@param : 버프가 적용중인 게임유닛(pGameUnit_)
*/
void CX2BuffTemplet::DoFinishDisplayer( CX2GameUnit* pGameUnit_ )
{
	BOOST_FOREACH( CX2BuffDisplayerTempletPtr ptrDisplayer, m_vecDisplayerPtr )
	{
		ptrDisplayer->DoFinish( pGameUnit_, this );
	}
}

/** @function : GetFactor
	@brief : 현재 실행되고 있는 버프의 수행요소 정보를 얻어오는 함수(서버를 통해 패킷전송하기 위해)
	@param : 정보를 담을 저장소(kBuffFactor_), 버프를 가지고 있는 게임유닛(pGameUnit_)
*/
void CX2BuffTemplet::GetFactor( OUT KBuffFactor& kBuffFactor_, const CX2GameUnit* pGameUnit_ ) const
{
	if ( !GetDidFinish() )
	{
		BOOST_FOREACH( CX2BuffBehaviorTempletPtr ptr, m_vecBehaviorPtr )
		{
			ptr->GetFactor( kBuffFactor_.m_vecBuffBehaviorFactors );
		}

		BOOST_FOREACH( CX2BuffFinalizerTempletPtr ptr, m_vecFinalizerPtr )
		{
			ptr->GetFactor( kBuffFactor_.m_vecBuffFinalizerFactors, pGameUnit_ );
		}

		/// 표현조건은 난입자에게 전송할 필요가 없으므로 수행하지 않는다.(m_vecDisplayerPtr)

		kBuffFactor_.m_BuffIdentity = m_BuffIdentity;

		if ( GetAccumulationLimit() > 1 )
		{
			kBuffFactor_.m_fAccumulationMultiplier = GetAccumulationMultiflier();
			kBuffFactor_.m_ucAccumulationCountNow = GetAccumulationCountNow();
		}
	}
}

/** @function : SetFactorToOnlyFinalizer
	@brief : 종료조건에만 팩터 적용(중첩카운트가 최대인 이후에도 종료조건은 계속 중첩에 의해서 갱신되도록 하기 위해 사용)
	@param : 버프 요소 (buffFactor_), 버프가 적용되야할 유닛(pGameUnit_)
	@return : 성공시 true, 실패시 false
*/
bool CX2BuffTemplet::SetFactorToOnlyFinalizer( const CX2BuffFactor& buffFactor_, CX2GameUnit* pGameUnit_ )
{
	/// 종료조건 셋팅
	BOOST_FOREACH( CX2BuffFinalizerTempletPtr ptrFinalizer, m_vecFinalizerPtr )
	{
		if ( !ptrFinalizer->SetFactor( buffFactor_, pGameUnit_, this ) )
			return false;
	}
	return true;
}

/** @function : GetFinalizerTempletPtrList
	@brief : 종료조건들을 얻어오는 함수
	@param : 얻어온 종료조건을 담을 함수
*/
void CX2BuffTemplet::GetFinalizerTempletPtrList( OUT vector<CX2BuffFinalizerTempletPtr>& vecFinalizerPtr_ )
{
	vecFinalizerPtr_.reserve( m_vecFinalizerPtr.size() );
	vecFinalizerPtr_ = m_vecFinalizerPtr;
}

/** @function : SetFinalizerTempletPtrList
	@brief : 종료조건들을 셋팅 하는 함수
	@param : 세팅할 종료조건
*/
void CX2BuffTemplet::ChangeFinalizerTempletPtrList( const vector<CX2BuffFinalizerTempletPtr>& vecFinalizerPtr_ )
{
	m_vecFinalizerPtr.resize( vecFinalizerPtr_.size() );
	m_vecFinalizerPtr = vecFinalizerPtr_;
}

/** @function : ReserveToFinish
	@brief : 다음 체크시 종료되도록 예약 해놓는 함수
*/
void CX2BuffTemplet::ReserveToFinish()
{
	/// 종료조건 셋팅
	BOOST_FOREACH( CX2BuffFinalizerTempletPtr ptrFinalizer, m_vecFinalizerPtr )
	{
		if ( NULL != ptrFinalizer )
			ptrFinalizer->ReserveToFinish();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetBasicStatValue
	@brief :스탯중 기본값을 셋팅하는 함수(스크립트값 또는 월드 변수등을 적용한 값 등)
	@param : 셋팅할 값(fValue_)
*/
void CX2AdditionalStatByBuff::SetBasicStatValue( const float fValue_ )
{ 
	if ( !IsSamef( GetBasicStatValue(), fValue_ ) )
	{
		m_fBasicStatValue = fValue_;
		SetMustChangeResultValue( true ); 
	}
}

/** @function : SetOptionValue
	@brief :스탯중 소켓 옵션에 의한 값을 셋팅하는 함수
	@param : 셋팅할 값(fValue_)
*/
void CX2AdditionalStatByBuff::SetOptionValue( const float fValue_ )
{
	if ( !IsSamef( GetOptionValue(), fValue_ ) )
	{
		m_fOptionValue = fValue_;
		SetMustChangeResultValue( true ); 
	}
}

/** @function : SetPassiveSum
	@brief : 스탯을 올리는 패시브 중 0.0을 기준으로 감소시에는 마이너스(-), 증가시에는 (+) 값인 요소를 셋팅하는 함수
	@param : 셋팅할 값(fValue_)
*/
void CX2AdditionalStatByBuff::SetPassiveSum( const float fValue_ )
{
	if ( !IsSamef( GetPassiveSum(), fValue_ ) )
	{
		m_fPassiveSum = fValue_;
		SetMustChangeResultValue( true ); 
	}
}

/** @function : SetPassiveMultiple
	@brief : 스탯을 올리는 패시브 중 1.0을 기준으로 감소시에는 0.0~1.0, 증가시에는 1.0 ~ 값인 요소를 셋팅
	@param : 셋팅할 값(fValue_)
*/
void CX2AdditionalStatByBuff::SetPassiveMultiple( const float fValue_ )
{
	if ( !IsSamef( GetPassiveMultiple(), fValue_ ) )
	{
		m_fPassiveMultiple = fValue_;
		SetMustChangeResultValue( true ); 
	}
}

/** @function : UpdateResultValueByChangeType
	@brief : 변경 타입별(치환, 고정수치, 연동수치, %) 결과값 갱신
*/
void CX2AdditionalStatByBuff::UpdateResultValueByChangeType()
{
	if ( m_bitsetMustChange.any() )	/// 변경된 것이 있다면 갱신
	{
		if ( m_bitsetMustChange.test( BCT_SWAP_VALUE ) )
		{
			/// 무조건 치환요소중 제일 나중에 걸린것 적용
			if ( m_vecStatFactorArray[BCT_SWAP_VALUE].empty() )
				m_fArrayResultValueByChangeType[BCT_SWAP_VALUE] = 0.0f;
			else
			{
				CX2StatFactorByBuffPtr& ptrStatFactor = m_vecStatFactorArray[BCT_SWAP_VALUE].back();
				m_fArrayResultValueByChangeType[BCT_SWAP_VALUE] = ptrStatFactor->GetValue();
			}
		}			

		if ( m_bitsetMustChange.test( BCT_FIX_VALUE ) )
		{
			float fResultValueByChangeType = 0.0f;

			auto FuncPlus = [&fResultValueByChangeType]( CX2StatFactorByBuffPtr ptrStatFactor_ ) {
				fResultValueByChangeType += ptrStatFactor_->GetValue();
			};

			if ( !m_vecStatFactorArray[BCT_FIX_VALUE].empty() )
				for_each( m_vecStatFactorArray[BCT_FIX_VALUE].begin(), m_vecStatFactorArray[BCT_FIX_VALUE].end(), FuncPlus );

			m_fArrayResultValueByChangeType[BCT_FIX_VALUE] = fResultValueByChangeType;
		}

		if ( m_bitsetMustChange.test( BCT_RELATION_VALUE ) )
		{
			float fResultValueByChangeType = 0.0f;

			auto FuncPlus = [&fResultValueByChangeType]( CX2StatFactorByBuffPtr ptrStatFactor_ ) {
				fResultValueByChangeType += ptrStatFactor_->GetValue();
			};

			if ( !m_vecStatFactorArray[BCT_RELATION_VALUE].empty() )
				for_each( m_vecStatFactorArray[BCT_RELATION_VALUE].begin(), m_vecStatFactorArray[BCT_RELATION_VALUE].end(), FuncPlus );

			m_fArrayResultValueByChangeType[BCT_RELATION_VALUE] = fResultValueByChangeType;
		}

		if ( m_bitsetMustChange.test( BCT_PERCENT ) )
		{
			float fResultValueByChangeType = 1.0f;

			auto FuncMultiplies = [&fResultValueByChangeType]( CX2StatFactorByBuffPtr ptrStatFactor_ ) {
				fResultValueByChangeType *= ptrStatFactor_->GetValue();
			};

			if ( !m_vecStatFactorArray[BCT_PERCENT].empty() )
				for_each( m_vecStatFactorArray[BCT_PERCENT].begin(), m_vecStatFactorArray[BCT_PERCENT].end(), FuncMultiplies );

			m_fArrayResultValueByChangeType[BCT_PERCENT] = fResultValueByChangeType;
		}

		m_bitsetMustChange.reset();	/// 변경을 적용 했으므로 초기화
	}		
}

/** @function : UpdateResultValue
	@brief : 옵션수치화와 관계없는 또는 기본값이 존재하는 스탯의 결과값을 업데이트 하는 함수
*/
void CX2AdditionalStatByBuff::UpdateResultValue()
{
	if ( !m_vecStatFactorArray[BCT_SWAP_VALUE].empty() )	/// 치환요소가 있다면
	{
		/// 무조건 치환요소중 제일 나중에 걸린것 적용
		SetResultValue( m_fArrayResultValueByChangeType[BCT_SWAP_VALUE] );
	}
	else	/// 치환요소가 없다면
	{
		float fResultValue = GetBasicStatValue();
		fResultValue += GetOptionValue();
		fResultValue += m_fArrayResultValueByChangeType[BCT_FIX_VALUE];
		fResultValue += m_fArrayResultValueByChangeType[BCT_RELATION_VALUE];

		/// 패시브스킬의 합배율과 곱배율 및 버프의 percent 배율을 모두 곱함
		const float fMultiplier 
			= ( 1 + GetPassiveSum() ) * GetPassiveMultiple() * m_fArrayResultValueByChangeType[BCT_PERCENT];
		if ( fMultiplier > 0.0f )	/// 효과가 감소되는 경우에는 0보다 크고 1.0보다 작음, 증가되는 경우 1.0보다 큼
			fResultValue += GetBasicStatValue() * (fMultiplier - 1.0f);
#ifdef _IN_HOUSE_
		else
		{
			// 오현빈 // 2013-08-16
			// 0으로 만들었을 때 발생할 수 있는 오류 사항에 대한 
			// 예외처리를 위해 기획적으로 사용 할 수 없도록 제한했습니다.
			// ex ) 인스펙터의 경우 특정 구간에 무조건 데미지를 1로 주기 위해, 
			//		방어력을 높게 잡아서. 90% 감소를 시켜도 1로 피격 되는데,
			//		100% 무시가 발생하면 한 번에 많은 데미지를 줄 수 있기 때문에 제한.
			DISPLAY_ERROR(L"BattleFieldBuffFactor.lua 설정 오류.\nPERCENT 값을 0으로 설정 할 수 없습니다.");
		}
#endif // _IN_HOUSE_

		SetResultValue( fResultValue );
	}
	SetMustChangeResultValue( false );
}

/** @function : UpdateResultValue
	@brief : 옵션수치화와 관계없는 또는 기본값이 존재하는 스탯이 변경되었는지를 검사하고, 결과값을 업데이트 하는 함수
*/
void CX2AdditionalStatByBuff::CheckAndUpdateResultValue()
{ 
	if ( GetMustChangeResultValue() )
	{
		/// 각 타입별 리스트 갱신
		UpdateResultValueByChangeType();

		/// 전체값 갱신
		UpdateResultValue();
	}
}

/** @function : CreateAndInsertStatFactor
	@brief : 버프에 의해 증가되는 요소를 만들고 삽입함
	@param : 고정, 연동 수치등 변동타입(eChangeType_), 어떤 버프에 의한 변화인지 구분(BuffIdentity_), 스탯값(fStatValue_)
*/
void CX2AdditionalStatByBuff::CreateAndInsertStatFactor( const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_, const float fStatValue_ )
{
	UINT uiIndex = eChangeType_;
	if ( uiIndex < ARRAY_SIZE( m_vecStatFactorArray ) )
	{
		CX2StatFactorByBuffPtr ptrStatFActor 
			= CX2StatFactorByBuffPtr( new CX2StatFactorByBuff( BuffIdentity_, fStatValue_ ) );

		m_vecStatFactorArray[eChangeType_].push_back( ptrStatFActor );
		m_bitsetMustChange.set( eChangeType_ );
		SetMustChangeResultValue( true );
	}				
}

/** @function : UpdateAddtionalStatFactor
	@brief : 버프의 Identity가 일치하는 것을 찾아서 스탯 갱신
	@param : 고정, 연동 수치등 변동타입(eChangeType_), 어떤 버프에 의한 변화인지 구분(BuffIdentity_), 스탯변경값(fStatValue_)
	@return: 성공(true), 실패(false) 로 실패하면 외부에서 Create 실행
*/
bool CX2AdditionalStatByBuff::UpdateAddtionalStatFactor( const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_, const float fValue_ )
{
	if ( !m_vecStatFactorArray[eChangeType_].empty() )
	{
		vector<CX2StatFactorByBuffPtr>::iterator vItr
			= FindSameBuffIdentity<vector<CX2StatFactorByBuffPtr>::iterator, CX2StatFactorByBuffPtr>( m_vecStatFactorArray[eChangeType_], BuffIdentity_ );

		if ( m_vecStatFactorArray[eChangeType_].end() != vItr )
		{
			(*vItr)->SetValue( fValue_ );
			m_bitsetMustChange.set( eChangeType_ );
			SetMustChangeResultValue( true );
			return true;
		}
	}

	return false;
}

/** @function : EraseStatFactor
	@brief :버프의 Identity가 일치하는 것을 찾아서 효과 삭제
	@param : 고정, 연동 수치등 변동타입(eChangeType_), 어떤 버프에 의한 변화인지 구분(BuffIdentity_)
*/
void CX2AdditionalStatByBuff::EraseStatFactor( const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_ )
{
	UINT uiIndex = eChangeType_;
	if ( uiIndex < ARRAY_SIZE( m_vecStatFactorArray ) )
	{
		vector<CX2StatFactorByBuffPtr>::iterator vItr
			= FindSameBuffIdentity<vector<CX2StatFactorByBuffPtr>::iterator, CX2StatFactorByBuffPtr>( m_vecStatFactorArray[uiIndex], BuffIdentity_ );

		if ( m_vecStatFactorArray[uiIndex].end() != vItr )
		{
			m_vecStatFactorArray[uiIndex].erase( vItr );
			m_bitsetMustChange.set( eChangeType_ );
			SetMustChangeResultValue( true );
		}
	}				
}
#ifdef VERIFY_STAT_BY_BUFF
bool CX2AdditionalStatByBuff::Verify()
{
	if ( m_fBasicStatValue.Verify() &&
		m_fPassiveSum.Verify() &&
		m_fPassiveMultiple.Verify() &&
		m_fOptionValue.Verify() &&
		m_fResultValue.Verify() &&
		VerifyStatFactor() &&
		VerifyArrayResult() )
		return true;
	else
		return false;
}

bool CX2AdditionalStatByBuff::VerifyStatFactor()
{
	for ( int i = 0; i < BCT_END; i++ )
	{
		BOOST_FOREACH( CX2StatFactorByBuffPtr& ptrStatFactor, m_vecStatFactorArray[i] )
		{
			if ( NULL != ptrStatFactor && false == ptrStatFactor->Verify() )
				return false;
		}
	}

	return true;
}

bool CX2AdditionalStatByBuff::VerifyArrayResult()
{
	for ( int i = 0; i < BCT_END; i++ )
	{
		if ( false == m_fArrayResultValueByChangeType[i].Verify() )
			return false;
	}

	return true;
}

#endif // VERIFY_STAT_BY_BUFF

/** @function : PlayEffectSetImpactPointByBuff
	@brief : 타격 위치에 지정된 이펙트셋 출력
	@param : 이펙트셋을 실행시키는 게임유닛(pGameUnit_), 타격위치(vImpactPos_)
*/
void CX2EffectSetImpactPointByBuff::PlayEffectSetImpactPointByBuff( CX2GameUnit* pGameUnit_, const D3DXVECTOR3& vImpactPos_ )
{
	if ( NULL != g_pX2Game )
	{
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜 : 2013-04-09
		if ( m_fNowCoolTime >= m_fCreateGap )		/// 생성 간격이 지났을 때, 생성 처리
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( m_wstrEffectSetName, pGameUnit_, 
				NULL, ( 0.0f < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate(), -1.f, D3DXVECTOR3( 1.0f, 1.0f, 1.0f ), 
				true, vImpactPos_ );
			PlaySound( pGameUnit_ );

			m_fNowCoolTime = 0.f;		/// 생성 간격 초기화
		}
#else // SERV_NEW_DEFENCE_DUNGEON
		g_pX2Game->GetEffectSet()->PlayEffectSet( m_wstrEffectSetName, pGameUnit_, 
			NULL, ( 0.0f < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate(), -1.f, D3DXVECTOR3( 1.0f, 1.0f, 1.0f ), 
			true, vImpactPos_ );
		PlaySound( pGameUnit_ );
#endif // SERV_NEW_DEFENCE_DUNGEON
	}
}

/** @function : PlayEffectSetHittedByBuff
	@brief : 피격시 지정된 이펙트셋 출력
	@param : 이펙트셋을 실행시키는 게임유닛(pGameUnit_)
*/
void CX2EffectSetImpactPointByBuff::PlayEffectSetHittedByBuff( CX2GameUnit* pGameUnit_ )
{
	if ( NULL != g_pX2Game )
	{
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜 : 2013-04-09
		if ( m_fNowCoolTime >= m_fCreateGap )		/// 생성 간격이 지났을 때, 생성 처리
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( m_wstrEffectSetName, pGameUnit_,
			NULL, ( 0.0f < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() );
			PlaySound( pGameUnit_ );

			m_fNowCoolTime = 0.f;	/// 생성 간격 초기화
		}
#else // SERV_NEW_DEFENCE_DUNGEON
		g_pX2Game->GetEffectSet()->PlayEffectSet( m_wstrEffectSetName, pGameUnit_,
			NULL, ( 0.0f < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() );
		PlaySound( pGameUnit_ );
#endif // SERV_NEW_DEFENCE_DUNGEON
	}
}

/** @function : PlaySound
	@brief : 사운드 출력
	@param : 사운드를 실행시키는 게임유닛(pGameUnit_)
*/
void CX2EffectSetImpactPointByBuff::PlaySound( CX2GameUnit* pGameUnit_ )
{
	if ( !m_wstrSoundName.empty() )
		pGameUnit_->PlaySound( m_wstrSoundName.c_str() );
}

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜 : 2013-04-09

/** @function : PlayEffectSetNormalAttackByBuff
	@brief : 일반 공격 타격시 이펙트셋 생성
	@param : 이펙트셋을 실행시키는 게임유닛(pGameUnit_), 타격위치(vImpactPos_)
*/
void CX2EffectSetImpactPointByBuff::PlayEffectSetNormalAttackByBuff( CX2GameUnit* pGameUnit_, const CX2DamageManager::ATTACK_TYPE eAttackType )
{
	if ( NULL != g_pX2Game )
	{
		if ( m_fNowCoolTime >= m_fCreateGap && CX2DamageManager::AT_NORMAL == eAttackType )		/// 생성 간격이 지났고, 일반 공격일때 생성 처리
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( m_wstrEffectSetName, pGameUnit_, 
				NULL, ( 0.0f < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate(), -1.f, D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
			PlaySound( pGameUnit_ );

			m_fNowCoolTime = 0.f;		/// 생성 간격 초기화
		}
	}
}

/** @function : ProcessCoolTime
	@brief : 쿨타임 갱신 함수
	@param : 프레임당 시간
*/
void CX2EffectSetImpactPointByBuff::ProcessCoolTime( float fElapsedTime )
{
	if ( m_fNowCoolTime < m_fCreateGap )
		m_fNowCoolTime += fElapsedTime;
}
#endif // SERV_NEW_DEFENCE_DUNGEON

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : CX2CustomFunctionByBuffPhantomSword
	@brief : CX2CustomFunctionByBuffPhantomSword 의 생성자
*/
CX2CustomFunctionByBuffPhantomSword::CX2CustomFunctionByBuffPhantomSword( const KBuffIdentity& BuffIdentity_, CX2GameUnit* pGameUnit_ ) 
	: CX2CustomFunctionByBuff( BuffIdentity_ )
	, MAGIC_PHANTOM_SWORD_DELAY( 0.2f )
	, MAGIC_PHANTOM_DAMAGE_MULTIPLIER( 1.2f )
	, MAGIC_PHANTOM_STOP_TIME_DEF( 0.033f )
	, MAGIC_MAX_COUNT_CATCH_UP( 2 )
	, m_fThirdBladeTime( 0.0f )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    , m_hDamageEffectThirdBlade( INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	, m_pDamageEffectThirdBlade( NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	, m_hThirdBladeCenterMesh( INVALID_MESH_INSTANCE_HANDLE )
	//, m_hThirdBladeSheathMesh( INVALID_MESH_INSTANCE_HANDLE )
	, m_strPhantomBladeDamageDataNameUTF8()
	, m_iPhantomBladeAttackTimeIndex( -1 )
	, m_vPhantomBladeBackSpeed( 0.0f, 0.0f )
	, m_PhantomBladeReActType( CX2DamageManager::RT_NO_DAMAGE )
{
	InitializeDamageEffectThridBlade( pGameUnit_ );
	InitializeMeshThirdBaldeCenter( pGameUnit_ );	
}



/** @function : ~CX2CustomFunctionByBuffPhantomSword
	@brief : CX2CustomFunctionByBuffPhantomSword의 소멸자
*/
/*virtual*/ CX2CustomFunctionByBuffPhantomSword::~CX2CustomFunctionByBuffPhantomSword()
{
	if ( NULL != g_pX2Game )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hThirdBladeCenterMesh );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( m_hDamageEffectThirdBlade != INVALID_DAMAGE_EFFECT_HANDLE )
            g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hDamageEffectThirdBlade );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		g_pX2Game->GetDamageEffect()->DestroyInstance( m_pDamageEffectThirdBlade );
        m_pDamageEffectThirdBlade = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}

	m_ThirdBladeMatrixQueue.swap( std::queue<ThirdBladeMatrixPtr>() );
	m_ThirdBladeDamageDataList.swap( std::list<ThirdBladeDamageDataPtr>() );
}

/** @function : FunctionInCommonStateStart
	@brief : CommonStateStart 에서 수행될 함수
	@param : 게임유닛(pGameUnit_), 루아매니저(luaManager_)
*/
/*virtual*/ void CX2CustomFunctionByBuffPhantomSword::FunctionInCommonStateStart( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_UTF8(	luaManager_, "PHANTOM_BLADE_DAMAGE_DATA_NAME",		m_strPhantomBladeDamageDataNameUTF8,	"" );
	LUA_GET_VALUE(	luaManager_, "PHANTOM_BLADE_ATTACK_TIME_INDEX",		m_iPhantomBladeAttackTimeIndex,		-1 );

	LUA_GET_VALUE(	luaManager_, "PHANTOM_BLADE_BACK_SPEED_X",			m_vPhantomBladeBackSpeed.x,			0.f );
	LUA_GET_VALUE(	luaManager_, "PHANTOM_BLADE_BACK_SPEED_Y",			m_vPhantomBladeBackSpeed.y,			0.f );

	LUA_GET_VALUE_ENUM( luaManager_, "PHANTOM_BLADE_REACT_TYPE",		m_PhantomBladeReActType,			CX2DamageManager::REACT_TYPE,		CX2DamageManager::RT_NO_DAMAGE );

	
	if ( pGameUnit_->GetSizeOfAttackTimeList() > 0 && -1 != m_iPhantomBladeAttackTimeIndex )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pDamageEffectThirdBlade = g_pX2Game->GetDamageEffect()->GetInstance( m_hDamageEffectThirdBlade ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( CX2DamageEffect::CEffect* pDamageEffectThirdBlade = m_pDamageEffectThirdBlade )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = pDamageEffectThirdBlade->GetMainEffect();
			if( NULL != pMeshInst )			
			{
				CreateAndInsertThirdBladeData( pGameUnit_, luaManager_ );
				if ( m_ThirdBladeDamageDataList.size() == 1 )	/// kimhc // 이건 왜 이렇게 했을까..?
                {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    MoveDataDataToDamageEffect( pGameUnit_, pDamageEffectThirdBlade );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
					MoveDataDataToDamageEffect( pGameUnit_ );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                }
			}	// if( NULL != pMeshInst )			
		}	// if ( NULL != m_pDamageEffectThirdBlade )
	}
}

/** @function : FunctionInCommonFrameMove
	@brief : CommonFrameMove 에서 수행될 함수
	@param : 게임유닛(pGameUnit_), 루아매니저(luaManager_)
*/
/*virtual*/ void CX2CustomFunctionByBuffPhantomSword::FunctionInCommonFrameMove( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CX2DamageEffect::CEffect* m_pDamageEffectThirdBlade = g_pX2Game->GetDamageEffect()->GetInstance( m_hDamageEffectThirdBlade ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if ( NULL != m_pDamageEffectThirdBlade )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = m_pDamageEffectThirdBlade->GetMainEffect();
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInstCenter = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hThirdBladeCenterMesh );

		if ( NULL != pMeshInst && NULL != pMeshInstCenter )
		{
			m_fThirdBladeTime = pMeshInst->GetGlobalTime();
			ThirdBladeMatrixPtr	ptrThirdBladeMatrix( new ThirdBladeMatrix );
			if ( NULL != ptrThirdBladeMatrix )
			{
				const D3DXMATRIX* pMatrixBone =	pGameUnit_->GetCombineMatrixFromBoneName( L"Dummy1_Rhand" );
				ptrThirdBladeMatrix->PastCombineMatrix = *pMatrixBone;
				ptrThirdBladeMatrix->fPastTime = static_cast<float>( g_pKTDXApp->GetTime() ); 
				m_ThirdBladeMatrixQueue.push( ptrThirdBladeMatrix );

				const float fDelayedTime = static_cast<float>( g_pKTDXApp->GetTime() ) - MAGIC_PHANTOM_SWORD_DELAY;
				ThirdBladeMatrixPtr ptrFirstThirdBladeMatrix = m_ThirdBladeMatrixQueue.front();
				
				for ( int i = 0; i < MAGIC_MAX_COUNT_CATCH_UP; ++i )
				{
					if ( ptrFirstThirdBladeMatrix->fPastTime > fDelayedTime )
						break;
					
					m_ThirdBladeMatrixQueue.pop();
					if ( !m_ThirdBladeMatrixQueue.empty() )
						ptrFirstThirdBladeMatrix = m_ThirdBladeMatrixQueue.front();
				}
				pMeshInst->SetUseDXMatrix( ptrFirstThirdBladeMatrix->PastCombineMatrix );
				pMeshInstCenter->SetUseDXMatrix( ptrFirstThirdBladeMatrix->PastCombineMatrix );
				
				const D3DXVECTOR3& vPos = pGameUnit_->GetPos();
				pMeshInstCenter->SetPos( vPos );
				pMeshInst->SetPos( vPos );
			}
		}
		else
			m_fThirdBladeTime = 0;

		if ( !m_ThirdBladeDamageDataList.empty() )
		{
			ThirdBladeDamageDataPtr ptrThirdBladeDamageData = m_ThirdBladeDamageDataList.front();

			if ( ptrThirdBladeDamageData->m_vAttackTime.y < m_fThirdBladeTime  )		
			{
				m_ThirdBladeDamageDataList.pop_front();
				if ( !m_ThirdBladeDamageDataList.empty() )
				{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    if ( CX2DamageEffect::CEffect* pDamageEffectThirdBlade = g_pX2Game->GetDamageEffect()->GetInstance( m_hDamageEffectThirdBlade ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	                if ( CX2DamageEffect::CEffect* pDamageEffectThirdBlade = m_pDamageEffectThirdBlade )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                        MoveDataDataToDamageEffect( pGameUnit_, pDamageEffectThirdBlade );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
					    MoveDataDataToDamageEffect( pGameUnit_ );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    }
				}
			}
		}
	}
}

/** @function : FunctionInCommonStateEnd
	@brief : CommonStateEnd 에서 수행될 함수
	@param : 게임유닛(pGameUnit_), 루아매니저(luaManager_)
*/
/*virtual*/ void CX2CustomFunctionByBuffPhantomSword::FunctionInCommonStateEnd( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ )
{
	if ( pGameUnit_->GetSizeOfAttackTimeList() > 0 )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* m_pDamageEffectThirdBlade = g_pX2Game->GetDamageEffect()->GetInstance( m_hDamageEffectThirdBlade ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	    if ( NULL != m_pDamageEffectThirdBlade )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = m_pDamageEffectThirdBlade->GetMainEffect();
			if ( NULL != pMeshInst && !m_ThirdBladeDamageDataList.empty() )
			{
				ThirdBladeDamageDataPtr ptrThirdBladeDamageDataBack	= m_ThirdBladeDamageDataList.back();
				const float fInterruptedAttackTime = m_fThirdBladeTime + MAGIC_PHANTOM_SWORD_DELAY;

				if ( fInterruptedAttackTime < ptrThirdBladeDamageDataBack->m_vAttackTime.y )
					ptrThirdBladeDamageDataBack->m_vAttackTime.y = fInterruptedAttackTime;

				if ( m_ThirdBladeDamageDataList.size() == 1 )
				{
					ThirdBladeDamageDataPtr ptrThirdBladeDamageDataFront 
						= m_ThirdBladeDamageDataList.front();
					m_pDamageEffectThirdBlade->SetAttackTime( ptrThirdBladeDamageDataFront->m_vAttackTime );
				}
			}
		}
	}
}

/** @function : InitializeDamageEffectThridBlade
	@brief : 팬텀소드의 데미지 이펙트 생성
	@param : 팬텀소드를 사용하는 게임유닛(pGameUnit_)
*/
void CX2CustomFunctionByBuffPhantomSword::InitializeDamageEffectThridBlade( CX2GameUnit* pGameUnit_ )
{
	wstring wstrEffectName = L"THIRD_BLADE";

#ifdef ADD_MEMO_1ST_CLASS //김창한
	CX2GUUser* pUser = static_cast<CX2GUUser*>( pGameUnit_ );
	if( NULL != pUser && pUser->GetEqippedSkillMemo(CX2SkillTree::SMI_ELSWORD_MEMO26) == true )
		wstrEffectName += L"_MEMO";
#endif //ADD_MEMO_1ST_CLASS

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect*
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pDamageEffectThirdBlade
		= g_pX2Game->GetDamageEffect()->CreateInstance( pGameUnit_, wstrEffectName.c_str(), 
		pGameUnit_->GetPowerRate(), pGameUnit_->GetPos(), pGameUnit_->GetRotateDegree(), 
		pGameUnit_->GetRotateDegree(),
		pGameUnit_->GetLandPos().y );	
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hDamageEffectThirdBlade = ( m_pDamageEffectThirdBlade != NULL ) 
        ? m_pDamageEffectThirdBlade->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if ( NULL != m_pDamageEffectThirdBlade )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = m_pDamageEffectThirdBlade->GetMainEffect();
		if ( NULL != pMeshInst )
		{
			pMeshInst->SetBoundingRadius( 0.0f );

			const D3DXMATRIX* pMatrixBone =	pGameUnit_->GetCombineMatrixFromBoneName( L"Dummy1_Rhand" );
			if ( NULL != pMatrixBone )
			{
				pMeshInst->SetUseDXMatrix( *pMatrixBone );
				pMeshInst->OnFrameMove( g_pKTDXApp->GetTime(), 0.01f );
			}
		}
	}
}

/** @function : InitializeMeshThirdBaldeCenter
	@brief : 팬텀소드의 메시 생성
	@param : 팬텀소드를 사용하는 게임유닛(pGameUnit_)
*/
void CX2CustomFunctionByBuffPhantomSword::InitializeMeshThirdBaldeCenter( CX2GameUnit* pGameUnit_ )
{
	CKTDGXMeshPlayer::CXMeshInstance* pMeshInstCenter = NULL;

#ifdef ADD_MEMO_1ST_CLASS //김창한
	CX2GUUser* pUser = static_cast<CX2GUUser*>( pGameUnit_ );
	if( NULL != pUser && pUser->GetEqippedSkillMemo(CX2SkillTree::SMI_ELSWORD_MEMO26) == true )
	{
		pMeshInstCenter = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( pGameUnit_,  L"ThirdBladeCenter_MEMO",
			pGameUnit_->GetPos(), pGameUnit_->GetRotateDegree(), pGameUnit_->GetRotateDegree() );
	}
	else
#endif //ADD_MEMO_1ST_CLASS
	pMeshInstCenter = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( pGameUnit_,  L"ThirdBladeCenter",
		pGameUnit_->GetPos(), pGameUnit_->GetRotateDegree(), pGameUnit_->GetRotateDegree() );

	if ( NULL != pMeshInstCenter )
	{
		m_hThirdBladeCenterMesh = pMeshInstCenter->GetHandle();
		pMeshInstCenter->SetBoundingRadius( 0.0f );

		const D3DXMATRIX* pMatrixBone =	pGameUnit_->GetCombineMatrixFromBoneName( L"Dummy1_Rhand" );
		if ( NULL != pMatrixBone )
		{
			pMeshInstCenter->SetUseDXMatrix( *pMatrixBone );
			pMeshInstCenter->OnFrameMove( g_pKTDXApp->GetTime(), 0.01f );
		}
	}
}

/** @function : CreateAndInsertThirdBladeData
	@brief : 팬텀소드의 데미지 데이타 생성 및 queue에 push하는 함수
	@param : 팬텀소드를 사용하는 게임유닛(pGameUnit_), 루아매니저(luaManager_)
*/
void CX2CustomFunctionByBuffPhantomSword::CreateAndInsertThirdBladeData( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ )
{
	const D3DXVECTOR2* const pAttackTime 
		= pGameUnit_->GetAttackTimeByIndex( m_iPhantomBladeAttackTimeIndex );
	if ( NULL != pAttackTime )
	{
		ThirdBladeDamageDataPtr ptrThirdBladeDamageData( new ThirdBladeDamageData );
		if ( NULL != ptrThirdBladeDamageData )
		{
			ptrThirdBladeDamageData->m_DamageData.SimpleInit();
			ptrThirdBladeDamageData->m_DamageData.attackerType			= CX2DamageManager::AT_UNIT;
			ptrThirdBladeDamageData->m_DamageData.optrAttackerGameUnit	= pGameUnit_;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            ptrThirdBladeDamageData->m_DamageData.hAttackerEffect		= INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			ptrThirdBladeDamageData->m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			if ( m_strPhantomBladeDamageDataNameUTF8.empty() )
				g_pData->GetDamageManager()->SetDamageDataFromLUA( &(ptrThirdBladeDamageData->m_DamageData), luaManager_, "DAMAGE_DATA" );
			else
				g_pData->GetDamageManager()->SetDamageDataFromLUA( &(ptrThirdBladeDamageData->m_DamageData), luaManager_, m_strPhantomBladeDamageDataNameUTF8.c_str() );
			
			const D3DXVECTOR2 vDelayedAttackTime = *pAttackTime / pGameUnit_->GetPlaySpeed();

			ptrThirdBladeDamageData->m_vAttackTime.x				= m_fThirdBladeTime + vDelayedAttackTime.x + MAGIC_PHANTOM_SWORD_DELAY; 
			ptrThirdBladeDamageData->m_vAttackTime.y				= m_fThirdBladeTime + vDelayedAttackTime.y + MAGIC_PHANTOM_SWORD_DELAY; 
			ptrThirdBladeDamageData->m_DamageData.damage.fPhysic	*= MAGIC_PHANTOM_DAMAGE_MULTIPLIER;
			ptrThirdBladeDamageData->m_DamageData.damage.fMagic		*= MAGIC_PHANTOM_DAMAGE_MULTIPLIER;
			ptrThirdBladeDamageData->m_DamageData.fForceDownValue	= 0.f;
			ptrThirdBladeDamageData->m_DamageData.fHitAddMP			= 0.f;
			ptrThirdBladeDamageData->m_DamageData.fStopTimeDef		+= MAGIC_PHANTOM_STOP_TIME_DEF;
			ptrThirdBladeDamageData->m_DamageData.backSpeed			= m_vPhantomBladeBackSpeed;
			ptrThirdBladeDamageData->m_DamageData.bArrangedFly		= true;
			ptrThirdBladeDamageData->m_DamageData.attackType		= CX2DamageManager::AT_SPECIAL;
			ptrThirdBladeDamageData->m_DamageData.optrAttackerGameUnit	= pGameUnit_;
#ifdef ADD_MEMO_1ST_CLASS //김창한
			CX2GUUser* pUser = static_cast<CX2GUUser*>( pGameUnit_ );
			if( NULL != pUser )
			{
				if( pUser->GetEqippedSkillMemo(CX2SkillTree::SMI_ELSWORD_MEMO26) == true )
				{
					ptrThirdBladeDamageData->m_DamageData.m_bIgnoreDefence = true;
					ptrThirdBladeDamageData->m_DamageData.m_fApplyCriticalDamage = 0.1f;
				}
			}
#endif //ADD_MEMO_1ST_CLASS

			if ( CX2DamageManager::RT_NO_DAMAGE != m_PhantomBladeReActType )
				ptrThirdBladeDamageData->m_DamageData.reActType = m_PhantomBladeReActType;

			m_ThirdBladeDamageDataList.push_back( ptrThirdBladeDamageData );
		}
	}
}

/** @function : MoveDataDataToDamageEffect
	@brief : 유저의 스테이트에서 읽어들인 데미지 데이터를 데미지이펙트에 전달
	@param : 팬텀소드를 사용중인 게임유닛(pGameUnit_)
*/
void CX2CustomFunctionByBuffPhantomSword::MoveDataDataToDamageEffect( CX2GameUnit* pGameUnit_
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    , CX2DamageEffect::CEffect*   pDamageEffectThirdBlade
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    )
{
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect*   pDamageEffectThirdBlade = m_pDamageEffectThirdBlade;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	/// m_ThirdBladeDamageDataList의 empty 검사, m_pDamageEffectThirdBlade NULL 체크는 caller에서 수행함
	ThirdBladeDamageDataPtr ptrThirdBladeDamageData = m_ThirdBladeDamageDataList.front();
	pDamageEffectThirdBlade->SetAttackTime( ptrThirdBladeDamageData->m_vAttackTime );

	CX2DamageManager::DamageData& kDamageData = pDamageEffectThirdBlade->GetDamageData();
	kDamageData = ptrThirdBladeDamageData->m_DamageData;
	kDamageData.attackerType = CX2DamageManager::AT_EFFECT;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    kDamageData.hAttackerEffect = pDamageEffectThirdBlade->GetHandle();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	kDamageData.pAttackerEffect = pDamageEffectThirdBlade;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	pDamageEffectThirdBlade->SetFollowLineRight( pGameUnit_->GetIsRight() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

// CX2CustomFunctionByBuffMagicalMakeUp::CX2CustomFunctionByBuffMagicalMakeUp( const KBuffIdentity& BuffIdentity_, CX2GameUnit* pGameUnit_ )
// 	: CX2CustomFunctionByBuff( BuffIdentity_ ), MAGICAL_MAKE_UP_WEAPON_ID( 40930 )
// {
// 	CX2Unit* pUnit = pGameUnit_->GetUnit();
// 	if ( NULL != pUnit )
// 	{
// 		m_ptrEquipTransform = CX2ItemManager::CreateDummyEquip( MAGICAL_MAKE_UP_WEAPON_ID, pUnit, pGameUnit_->GetSkinAnimPtr() );
// 		m_ptrEquipTransform->SetManagerObject( this );
// 		m_ptrEquipTransform->SetShowObject(false);
// 	}
// }
// 
// CX2CustomFunctionByBuffMagicalMakeUp::~CX2CustomFunctionByBuffMagicalMakeUp()
// {
// 	/// 각 유닛마다 특정 버프가 해제될 때 셋팅해줘야 하는 값(ex: 매지컬메이크업이 시전중이라는 플래그 등...)
// //	pGameUnit_->UpSetSpecificValueByBuffTempletID( m_DataBuffIdentity.m_eBuffTempletID );
// }
// 
// void CX2CustomFunctionByBuffMagicalMakeUp::FunctionInCommonFrameMove( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ )
// {
// // 	if ( NULL != m_ptrEquipTransform )
// // 	{
// // 		m_ptrEquipTransform->SetRenderParam( m_fTime, m_fElapsedTime * m_AdvanceTimeCount, m_pXSkinAnim->GetRenderParam() );
// // 		m_ptrEquipTransform->OnFrameMove( m_fTime, m_fElapsedTime * m_AdvanceTimeCount );
// // 	}	
// }

/*virtual*/ void CX2CustomFunctionByBuffBigBangStream::FunctionInCommonStateStart( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ )
{
	m_vecAnimationTime.resize( 0 );
	if ( luaManager_.BeginTable( "BIG_BANG_STREAM" ) )
	{
		int iIndex = 1;
		float fValue = 0.0f;
		while ( luaManager_.GetValue( iIndex, fValue ) )
		{
			m_vecAnimationTime.push_back( fValue );
			++iIndex;
		}

		/// 큰것에 부터 작은 순으로 정렬
		std::sort( m_vecAnimationTime.begin(), m_vecAnimationTime.end(), std::greater<float>() );
		m_vStateStartPos = pGameUnit_->GetPos();	/// 스테이트가 시작됬을 때의 유닛 위치를 저장

		luaManager_.EndTable();
	}
}

/*virtual*/ void CX2CustomFunctionByBuffBigBangStream::DoExtraAttack( CX2GameUnit* pGameUnit_ )
{
	const D3DXVECTOR3& vBonePos = pGameUnit_->GetBonePos( L"Bip01" );
	CX2DamageEffect::CEffect* pDamageEffect = NULL;
#ifdef ADD_MEMO_1ST_CLASS //김창한
	CX2GUUser* pUser = static_cast<CX2GUUser*>( pGameUnit_ );
	if( NULL != pUser && pUser->GetEqippedSkillMemo( CX2SkillTree::SMI_CHUNG_MEMO26 ) == true 
		&& pUser->GetRandomFloat() <= 0.2f )
		pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( pGameUnit_, L"CHUNG_BIGBANGSTREAM_MEMO", pGameUnit_->GetPowerRate(), vBonePos, pGameUnit_->GetRotateDegree(), pGameUnit_->GetRotateDegree(),  pGameUnit_->GetLandPos().y );
	else
#endif //ADD_MEMO_1ST_CLASS

	pDamageEffect 
		= g_pX2Game->GetDamageEffect()->CreateInstance( 
		pGameUnit_, L"HAND_GRENADE_0", pGameUnit_->GetPowerRate(), vBonePos,
		pGameUnit_->GetRotateDegree(), pGameUnit_->GetRotateDegree(),  pGameUnit_->GetLandPos().y );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( pDamageEffect == NULL )
        return;
    g_pX2Game->AddRemoveDamageEffect( pDamageEffect->GetHandle() );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    g_pX2Game->AddRemoveDamageEffect( pDamageEffect );	
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	
	
	CKTDGXMeshPlayer::CXMeshInstance *pMesh = pDamageEffect->GetMainEffect();
	if ( NULL != pMesh )
	{
#ifdef FIX_BIGBANG_STREAM_BUFF
		D3DXVECTOR3 vDir = pGameUnit_->GetDirVector() * 150.f;

		float fDirX = ( (vDir.x + abs(vDir.z)) * 0.3f ) * 10.f + 100.f;
		const float fDirY = (vDir.y * 10.f) + 100.f;
#else
		const D3DXVECTOR3& vPos = pGameUnit_->GetPos();
		D3DXVECTOR3 vDir = vPos - m_vStateStartPos;
		float fDirX = ( (vDir.x + vDir.z) * 0.5f ) * 10.f + 100.f;
		const float fDirY = (vDir.y * 10.f) + 100.f;

		if ( pGameUnit_->GetIsRight() == false )
			fDirX *= -1.f;
#endif // FIX_BIGBANG_STREAM_BUFF

		pMesh->SetVelocity( D3DXVECTOR3( fDirX, fDirY, 0.f) );
	}
}


/*virtual*/ void CX2CustomFunctionByBuffMarkOfCommand::FunctionInCommonFrameMove( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_checkElapsedTime.OnFrameMove( pGameUnit_->GetElapsedTime() );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_checkElapsedTime.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
}

/*virtual*/ void CX2CustomFunctionByBuffBigBangStream::FunctionInCommonFrameMove( CX2GameUnit* pGameUnit_, KLuaManager& luaManager_ )
{
	if ( !m_vecAnimationTime.empty() && pGameUnit_->GetSkinAnimPtr() )
	{
		const float fNowAnimationTime = pGameUnit_->GetSkinAnimPtr()->GetNowAnimationTime();

		while ( !m_vecAnimationTime.empty() && m_vecAnimationTime.back() <= fNowAnimationTime )
		{
			DoExtraAttack( pGameUnit_ );
			m_vecAnimationTime.pop_back();
		}		
	}
}

/*virtual*/ void CX2CustomFunctionByBuffMarkOfCommand::FunctionInDamageReact( CX2GameUnit* pGameUnit_, IN CX2DamageManager::DamageData& damageData_ )
{
	DoMarkOfCommand( pGameUnit_, damageData_ );
}

void CX2CustomFunctionByBuffMarkOfCommand::DoMarkOfCommand( CX2GameUnit* pGameUnit_, IN CX2DamageManager::DamageData& damageData_ )
{
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
if( damageData_.m_fHpPercentUp <= 0.f &&
	damageData_.reActType != CX2DamageManager::RT_CREATE_DAMAGE_EFFECT &&
	damageData_.reActType != CX2DamageManager::RT_NO_DAMAGE &&
	damageData_.reActResult != CX2DamageManager::RT_DUMMY_DAMAGE &&
	damageData_.reActResult != CX2DamageManager::RT_SENSOR &&
	damageData_.hitType != CX2DamageManager::HT_GROUND_HIT &&
	damageData_.m_eDamageTrigger != CX2DamageManager::DTT_MARK_OF_COMMANDER &&
	damageData_.m_bNoDamage == false )
{
	if ( null != damageData_.optrAttackerGameUnit &&
		 m_checkElapsedTime.CheckAndResetElapsedTime() )
	{
		CX2GameUnit* pAttackerGameUnit = damageData_.optrAttackerGameUnit.GetObservable();

		D3DXVECTOR3 vDirectVectorZeroY = pAttackerGameUnit->GetDirVector();
		vDirectVectorZeroY.y	= 0.f;
		const float fConstUnitHeight = 1080.f;
		const float fShellPowerRate = damageData_.damage.GetAllDamage() * damageData_.m_fDungeonRate;

		for ( UINT uiIndex = 0; uiIndex < m_uiDamageEffectCount; ++uiIndex )
		{
			float fRandReservTime = 0.5f;
			D3DXVECTOR3 vRotateDegree = pAttackerGameUnit->GetRotateDegree();

			if ( 0 < uiIndex )
			{
				fRandReservTime += pAttackerGameUnit->GetRandomFloat( uiIndex ) * 0.3f * uiIndex;
				fRandReservTime += pAttackerGameUnit->GetRandomFloat( pGameUnit_->GetUnitIndex() ) * 0.3f;
				vRotateDegree.z += ( pAttackerGameUnit->GetRandomFloat( pGameUnit_->GetUnitIndex() + uiIndex) * 10.f - 5.f );
			}

			D3DXVECTOR3 vShellStartPos = pGameUnit_->GetPos();
			vShellStartPos.y += fConstUnitHeight;			

			if ( pAttackerGameUnit->GetIsRight() )
				vShellStartPos += ( m_fBasicPosX[uiIndex] * vDirectVectorZeroY );
			else
				vShellStartPos -= ( m_fBasicPosX[uiIndex] * vDirectVectorZeroY );

			/// 메모를 배웠으면
			const WCHAR* pwszDamageEffectName
				= ( ( 0.0f < m_fMemoRate && pAttackerGameUnit->GetRandomFloat( uiIndex + 3 ) <= m_fMemoRate ) ?
				L"CHUNG_MARK_OF_COMMANDER_SHELL_MEMO" : L"CHUNG_MARK_OF_COMMANDER_SHELL" );
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
			g_pX2Game->GetDamageEffect()->CreateInstance( pAttackerGameUnit, pwszDamageEffectName, 
			fShellPowerRate * m_fAddPowerRate, vShellStartPos, vRotateDegree, vRotateDegree,  
			pAttackerGameUnit->GetLandPos().y, true, fRandReservTime );						

#else // UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
			g_pX2Game->GetDamageEffect()->CreateInstance( pAttackerGameUnit, pwszDamageEffectName, 
				fShellPowerRate, vShellStartPos, vRotateDegree, vRotateDegree,  
				pAttackerGameUnit->GetLandPos().y, true, fRandReservTime );				
#endif // UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈								
		}
		
	}
}
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN

}

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
// 행동 불능 디버프 인가? 특정 BBT 를 가지고 있으면 참을 반환
// Ex. 블레이징 하트, 불꽃 개화
bool CX2BuffTemplet::IsNonControlDebuff ()
{
	BOOST_FOREACH( CX2BuffBehaviorTempletPtr ptrBehavior, m_vecBehaviorPtr )
	{
		switch ( ptrBehavior->GetBuffBehaviorType() )
		{
		case BBT_DASH_IMPOSSIBLE :
		case BBT_ATTACK_IMPOSSIBLE :
		case BBT_REVERSE_LEFT_RIGHT :
		case BBT_CHANGE_UNIT_SCALE :
		case BBT_STUN :
		case BBT_FROZEN :
		case BBT_NEVER_MOVE :
		case BBT_SKILL_ATTACK_IMPOSSIBLE :
		case BBT_ZX_ATTACK_IMPOSSIBLE :
		case BBT_Z_ATTACK_IMPOSSIBLE :
		case BBT_X_ATTACK_IMPOSSIBLE :
			return true;
			break;

		default :
			break;
		}
	}
	return false;
}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
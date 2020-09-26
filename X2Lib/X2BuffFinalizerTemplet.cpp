/** @file : X2BuffFinalizerTemplet.cpp
    @breif : 버프의 종료 조건을 정의하는 Templet 파일
*/

#pragma once

#include "StdAfx.h"
#include ".\X2BuffFinalizerTemplet.h"

#define CASE_FINALIZER_TEMPLET( class_, type_ ) \
case type_: \
	{ \
	CX2BuffFinalizerTempletPtr ptrFinalizerTemplet = class_::CreateBuffFinalizerTempletPtr(); \
	if ( NULL != ptrFinalizerTemplet && ptrFinalizerTemplet->ParsingFinalizerTemplateMethod( luaManager_, STRINGIZEA2(type_) ) ) \
		{ \
		ptrFinalizerTemplet->SetType( type_ ); \
		vecBuffFinalizerTempletPtr_.push_back( ptrFinalizerTemplet ); \
		} \
		else \
		return DISPLAY_ERROR( STRINGIZE2(type_) ); \
	} break; \

/** @function : ParsingCombinationFinalizer
	@brief : 버프의 종료조건이 어떤종류로 구성되어 있는지 알아내고, 구성된 타입별로 파싱
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_), 파싱성공한 FinalizerTempletPtr을 담을 vector(vecBuffFinalizerTempletPtr_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
/*static*/ bool CX2BuffFinalizerTemplet::ParsingCombinationFinalizer( KLuaManager& luaManager_, OUT vector<CX2BuffFinalizerTempletPtr>& vecBuffFinalizerTempletPtr_ )
{
	vector<UINT> vecFinalizerType;

#ifdef EXCEPTION_BUFF_FACTOR
	bool bExceptionCase = true;
#endif // EXCEPTION_BUFF_FACTOR
	
	/// Finalizer가 어떤 종료요소들의 조함으로 이루어 졌는지 얻어옴
	if ( CX2BuffTempletManager::GetCombination( luaManager_, vecFinalizerType ) )
	{
		vector<UINT>::iterator vItr = vecFinalizerType.begin();
		while ( vecFinalizerType.end() != vItr )	/// 조합에 포함된 종료요소들을 파싱
		{
#ifdef EXCEPTION_BUFF_FACTOR
			if(static_cast<BUFF_FINALIZER_TYPE>( *vItr ) != BFT_RIDING_ON_OR_NOT)
				bExceptionCase = false;
#endif // EXCEPTION_BUFF_FACTOR

			switch ( static_cast<BUFF_FINALIZER_TYPE>( *vItr ) )
			{
			CASE_FINALIZER_TEMPLET( CX2BuffTimeFinalizerTemplet, BFT_TIME )
			CASE_FINALIZER_TEMPLET( CX2BuffHitCountFinalizerTemplet, BFT_HIT_COUNT )
			CASE_FINALIZER_TEMPLET( CX2BuffHittedCountFinalizerTemplet, BFT_HITTED_COUNT )
			CASE_FINALIZER_TEMPLET( CX2BuffStateChangeFinalizerTemplet, BFT_STATE_CHANGE )
			CASE_FINALIZER_TEMPLET( CX2BuffDamageValueFinalizerTemplet, BFT_DAMAGE_VALUE )
			CASE_FINALIZER_TEMPLET( CX2BuffPassHpFinalizerTemplet, BFT_PASS_HP )
			CASE_FINALIZER_TEMPLET( CX2BuffPassMpFinalizerTemplet, BFT_PASS_MP )
			CASE_FINALIZER_TEMPLET( CX2BuffImmediateFinalizerTemplet, BFT_IMMEDIATE )
			CASE_FINALIZER_TEMPLET( CX2BuffDungeonStageChangeFinalizerTemplet, BFT_DUNGEON_STAGE_CHANGE )
#ifdef ADD_LIVE_CREATOR_FINALIZER
			CASE_FINALIZER_TEMPLET( CX2BuffLiveCreatorFinalizerTemplet, BFT_LIVE_CREATOR )
#endif // ADD_LIVE_CREATOR_FINALIZER
#ifdef ADD_BUFF_FINALIZER_FINISH_HYPER
			CASE_FINALIZER_TEMPLET( CX2BuffFinishHyperFinalizerTemplet, BFT_FINISH_HYPER )
#endif // ADD_BUFF_FINALIZER_FINISH_HYPER
#ifdef RIDING_SYSTEM
			CASE_FINALIZER_TEMPLET( CX2BuffFinishRidingPetOnOrNotFinalizerTemplet, BFT_RIDING_ON_OR_NOT )
#endif	// RIDING_SYSTEM
			CASE_FINALIZER_TEMPLET( CX2BuffX2StateChangeFinalizerTemplet, BFT_X2STATE_CHANGE )

#ifdef EXCEPTION_BUFF_FACTOR
			case BFT_RIDING_ON_OR_NOT:
				++vItr;
				continue;
#endif // EXCEPTION_BUFF_FACTOR

			default:
				return DISPLAY_ERROR( L"Unknown Finalizer Type" );	/// false 리턴
				break;
			}

			++vItr;
		}
	}

	if ( vecBuffFinalizerTempletPtr_.empty()
#ifdef EXCEPTION_BUFF_FACTOR
		&& !bExceptionCase 
#endif // EXCEPTION_BUFF_FACTOR	
		)
	{
		return DISPLAY_ERROR( L"FINALIZER Templet is empty" );
	}
	else
		return true;
}

/** @function : ParsingFinalizerTemplateMethod
	@brief : 각 파생클래스에서 구현된 ParsingFinalizer를 똑같은 전후 처리 사이에서 호출하기 위한 템플릿메소드
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_), 파싱성공한 FinalizerTempletPtr을 담을 vector(vecBuffFinalizerTempletPtr_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
bool CX2BuffFinalizerTemplet::ParsingFinalizerTemplateMethod( KLuaManager& luaManager_, const char* pszTableName_ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		return ParsingFinalizer( luaManager_ );
	}
	else
		return DISPLAY_ERROR( pszTableName_ );	
}

/** @function : SetFactorFromPacketTemplateMothod
	@brief : 패킷으로 부터 받은 버프 정보로 각 종료조건의 SetFactorFromPacket을 실행 시키는 템플릿 메소드
	@param : 서버로 부터 받은 버프정보(kBuffFactor_), 적용할 게임유닛(pGameUnit_)
	@return : 성공(true), 실패(false)
*/
bool CX2BuffFinalizerTemplet::SetFactorFromPacketTemplateMothod( const KBuffFactor& kBuffFactor_, CX2GameUnit* pGameUnit_ )
{
	BOOST_FOREACH( const KBuffFinalizerFactor& kFactor_, kBuffFactor_.m_vecBuffFinalizerFactors )
	{
		if ( GetType() == kFactor_.m_uiType )
		{
			SetFactorFromPacket( kFactor_, pGameUnit_ );
			return true;
		}		
	}

	return DISPLAY_ERROR( L"SetFactorFromPacket Error" );
}

/** @function : ParsingFinalizer
	@brief : 시간에 의해 종료되는 템플릿 파싱
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_)
	@return : 성공시 true
*/
/*virtual*/ bool CX2BuffTimeFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_DURATION_TIME_TYPE", m_eDurationTimeType,
		BUFF_DURATION_TIME_TYPE, BDTT_NONE, return DISPLAY_ERROR( L"BUFF_DURATION_TIME_TYPE" ) );

	return true;
}

/** @function : OnFrameMove;
	@brief : 매 프레임 마다 DurationTime을 감소 시키고 0 미만이 되면 종료됨
	@param : 버프에 걸린 유닛의 포인터(pGameUnit_)
	@return : 
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffTimeFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffTimeFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_fDurationTime -= fElapsedTime_;	
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_fDurationTime -= g_pKTDXApp->GetElapsedTime();	
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef BUFF_ICON_UI //버프 남은 지속 시간 얻기	
	if( false == m_bIsAlreadyNotifyDurationTime // 남은시간에 대한 알림을 보내지 않았을 때
		&& 5.f >= m_fDurationTime )
	{
		CX2GageManager::GetInstance()->NotifyDurationTime5sec( pGameUnit_->GetUnitUID(), m_eBuffTempleteID );
		m_bIsAlreadyNotifyDurationTime = true;
	}
#ifdef DISPLAY_BUFF_DURATION_TIME
	int iDurationSec = (false == IsSamef(m_fDurationTime,0.f)) ? static_cast<int>(m_fDurationTime) + 1 : 0;
	if( iDurationSec != m_iDurationSec )
	{
		m_iDurationSec = iDurationSec;
		CX2GageManager::GetInstance()->SetDurationTime( pGameUnit_->GetUnitUID(), m_eBuffTempleteID, m_iDurationSec );
	}
#endif // DISPLAY_BUFF_DURATION_TIME

#endif //BUFF_ICON_UI
}

/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffTimeFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{

	const KBuffFinalizerFactor* pFactor = NULL;
	
	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		BUFF_DURATION_TIME_TYPE eDurationTimeType 
			= STATIC_CAST_FLOAT_TO_ENUM( BUFF_DURATION_TIME_TYPE, (*pFactor)[FO_DURATION_BUFF_TIME_TYPE] );
		
		/// 저항을 무시하는 것도 아니고 영향을 받을 저항이 있는 경우 저항값을 얻어옴

// 		const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 			0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );

		const float fRegist = pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE;

		if ( m_eDurationTimeType == eDurationTimeType )
		{
			switch ( m_eDurationTimeType )
			{
			case BDTT_NORMAL_TIME:
				{
					const float fNormalTime = (*pFactor)[FO_NORMAL_TIME] * (1.0f - fRegist);
					// 지속시간과 비교해서 시간이 긴것으로 교체
					m_fDurationTime = ( m_fDurationTime > fNormalTime ? m_fDurationTime : fNormalTime );
				} break;

			case BDTT_RELATION_HYPER_MODE_COUNT:
				{
					const float fNormalTime = (*pFactor)[FO_NORMAL_TIME] * (1.0f - fRegist);
					// 지속시간과 비교해서 시간이 긴것으로 교체
					m_fDurationTime += fNormalTime;
				} break;

			case BDTT_RANDOM_TIME:
				{
					// 랜덤 시간은 fMinTime ~ fMaxTime 사이
					const float fMinTime = (*pFactor)[FO_MIN_TIME];
					const float fMaxTime = (*pFactor)[FO_MAX_TIME];
					const UINT uiTimeGap = static_cast<UINT>( fMaxTime - fMinTime );

					/// 다른 유닛이기 때문에 랜덤 오프셋을 0으로 통일 해도 문제 없음
					const float fNewTime = ( fMinTime + ( pGameUnit_->GetRandomInt( 0 ) % (uiTimeGap > 0 ? uiTimeGap : 1) ) ) * (1.0f - fRegist);

					/// 새로운 시간이 기존의 시간보다 클때 적용
					m_fDurationTime = ( m_fDurationTime > fNewTime ? m_fDurationTime : fNewTime );
				} break;

			default:
				return DISPLAY_ERROR( L"BUFF_DURATION_TIME_TYPE Invalid" );
				break;
			}

			//레이븐 용병의 처신술 패시브(SI_P_RWT_BEHAVIOR_OF_MERCENARY) 효과를 적용시키기 위해 추가
			m_fDurationTime *= pGameUnit_->GetReduceDebuffDurationRate( pBuffTemplet_->GetBuffTempletID() );
#ifdef BUFF_ICON_UI
			m_eBuffTempleteID = BuffFactor_.GetBuffTempletID();
			m_bIsAlreadyNotifyDurationTime = false;
#endif //BUFF_ICON_UI

			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_DURATION_TIME_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffTimeFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffTimeFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffTimeFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffTimeFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( !DidReserveToFinish() && 0 < m_fDurationTime )
		return false;
	else
		return true;
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffTimeFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	/// 남은 시간
	FinalizerFactor.m_vecValues.push_back( m_fDurationTime );

	/// 전송시의 FrameMoveCount;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    DWORD dwCurFrameMoveCount = ( g_pX2Game != NULL ) ? g_pX2Game->GetFrameUDPPack().GetFrameMoveCount() : 0;
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( dwCurFrameMoveCount ) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	FinalizerFactor.m_vecValues.push_back( static_cast<float>( pGameUnit_->GetFrameMoveCountNow() ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 시간에 의해 종료되는 템플릿을 셋팅
	@param : 서버로 부터 받은 종료요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffTimeFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// 전송 받은 남은 시간을 대입
	m_fDurationTime = kFactor_[0];

	const DWORD dwFrameMoveCount = static_cast<DWORD>( kFactor_[1] );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    DWORD dwCurFrameMoveCount = ( g_pX2Game != NULL ) ? g_pX2Game->GetFrameUDPPack().GetFrameMoveCount() : 0;
	if ( dwCurFrameMoveCount > dwFrameMoveCount )
	{
		const float fElapsedTime = static_cast<float>( ( dwCurFrameMoveCount - dwFrameMoveCount ) / g_pKTDXApp->GetFrameMoveFPS() );
		m_fDurationTime -= fElapsedTime;
	}
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	if ( pGameUnit_->GetFrameMoveCountNow() > dwFrameMoveCount )
//	{
//		const float fElapsedTime = static_cast<float>( ( pGameUnit_->GetFrameMoveCountNow() - dwFrameMoveCount ) / g_pKTDXApp->GetFrameMoveFPS() );
//		m_fDurationTime -= fElapsedTime;
//	}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingFinalizer
	@brief : 타격 카운트에 의해 종료되는 템플릿 파싱
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_)
	@return : 성공시 true
*/
/*virtual*/ bool CX2BuffHitCountFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_USE_COUNT_TYPE", m_eUseCountType,
		BUFF_USE_COUNT_TYPE, BUCT_NOT_USE, return DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE" ) );
	return true;
}

/** @function : OnFrameMove
	@brief : 타격카운트가 유효범위를 벗어나면 m_iCountToBeFinished를 재설정 한다.
	@param : 버프에 걸린 유닛(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffHitCountFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffHitCountFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	/// 이번 프레임에 타격한 수만큼을 카운트에서 제한다.
	if ( m_bStart && MAX_COUNT <= m_iCountToBeFinished )
	{
		const UCHAR ucCountNow = pGameUnit_->GetHitCount();
		/// 현재의 카운트가 시작했을 때의 카운트보다 작다면
		/// 변수의 유효범위(UCHAR:0~255)를 한바퀴 돈것이기 때문에 재설정함
		if ( ucCountNow < m_ucCountWhenStart )
			m_iCountToBeFinished -= MAX_COUNT;	
	}
}

/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffHitCountFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		/// 타입을 알아내고
		const BUFF_USE_COUNT_TYPE eUseCountType = STATIC_CAST_FLOAT_TO_ENUM( BUFF_USE_COUNT_TYPE, (*pFactor)[FO_USE_BUFF_COUNT_TYPE] );

		if ( eUseCountType == m_eUseCountType )
		{
			const UCHAR ucCountNow = pGameUnit_->GetHitCount();
			
			if ( false == m_bStart )
			{
				m_ucCountWhenStart = ucCountNow;
				m_bStart = true;
			}

			const UINT uiCountNew = GetNewCount( pFactor, pGameUnit_ );
			/// 기존의 것보다 새로운 것(uiCountNew)의 유지 카운트가 길면 새로운 것으로 대체
			const int iCountOld = m_iCountToBeFinished - static_cast<int>( ucCountNow );

			m_iCountToBeFinished 
				= ( iCountOld > static_cast<int>( uiCountNew ) ? m_iCountToBeFinished : static_cast<int>( ucCountNow + uiCountNew ) );
			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffHitCountFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffHitCountFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffHitCountFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffHitCountFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( m_bStart && ( m_iCountToBeFinished <= static_cast<int>( pGameUnit_->GetHitCount() ) || DidReserveToFinish() ) )
		return true;	/// 종료
	else
		return false;	/// 계속
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffHitCountFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_iCountToBeFinished ) );	/// 버프가 끝나기 위한 카운트
	FinalizerFactor.m_vecValues.push_back( m_ucCountWhenStart );	/// 버프가 시작했을 때의 카운트
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 타격 카운트에 의해 종료되는 템플릿을 셋팅
	@param : 서버로 부터 받은 종료요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffHitCountFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_iCountToBeFinished = static_cast<int>( kFactor_[0] );
	m_ucCountWhenStart = static_cast<UCHAR>( kFactor_[1] );

	m_bStart = true;
	const UCHAR ucCountNow = pGameUnit_->GetHitCount();
	/// 현재의 카운트가 시작했을 때의 카운트보다 작다면
	/// 변수의 유효범위(UCHAR:0~255)를 한바퀴 돈것이기 때문에 재설정함
	if ( ucCountNow < m_ucCountWhenStart )
		m_iCountToBeFinished -= MAX_COUNT;	
}

/** @function : GetNewCount
	@brief : 적용할 카운트 값을 얻어냄
	@param : 버프종료요소(pFactor_), 버프에 걸린 게임유닛(pGameUnit_)
	@return : 카운트 값
*/
UINT CX2BuffHitCountFinalizerTemplet::GetNewCount( const KBuffFinalizerFactor* pFactor_, CX2GameUnit* pGameUnit_ ) const
{
	if ( 0.0f < (*pFactor_)[FO_RESULT_COUNT] )		/// 결정된 카운트가 있다면 바로 적용(이미 진행중인 것을 서버로 부터 받는 경우 또는 시전자의 버프카운트 적용 시)
		return static_cast<UINT>( (*pFactor_)[FO_RESULT_COUNT] );
	else	/// 없다면
	{
		switch ( m_eUseCountType )
		{
		case BUCT_ENEMY_BUFF_COUNT:
			{
				return static_cast<UINT>( (*pFactor_)[FO_COUNT] * pGameUnit_->GetAppliedBuffCount( false ) );
			} break;

		case BUCT_ENEMY_DEBUFF_COUNT:
			{
				return static_cast<UINT>( (*pFactor_)[FO_COUNT] * pGameUnit_->GetAppliedBuffCount( true ) );
			} break;

		default:
			{
				DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE" );	/// 다른 타입에 이곳을 실행한다면 ERROR
				return 0;
			} break;
		}
	}		
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingFinalizer
	@brief : 피격 카운트에 의해 종료되는 템플릿 파싱
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
/*virtual*/ bool CX2BuffHittedCountFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_USE_COUNT_TYPE", m_eUseCountType,
		BUFF_USE_COUNT_TYPE, BUCT_NOT_USE, return DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE" ) );
	return true;
}

/** @function : OnFrameMove
	@brief : 피격카운트가 유효범위를 벗어나면 m_iCountToBeFinished를 재설정 한다.
	@param : 버프에 걸린 유닛(pGameUnit_)
*/

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffHittedCountFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffHittedCountFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	/// 이번 프레임에 타격한 수만큼을 카운트에서 제한다.
	if ( m_bStart && MAX_COUNT <= m_iCountToBeFinished )
	{
		const UCHAR ucCountNow = pGameUnit_->GetHittedCount();
		/// 현재의 카운트가 시작했을 때의 카운트보다 작다면
		/// 변수의 유효범위(UCHAR:0~255)를 한바퀴 돈것이기 때문에 재설정함
		if ( ucCountNow < m_ucCountWhenStart )
			m_iCountToBeFinished -= MAX_COUNT;	
	}
}

/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffHittedCountFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		/// 타입을 알아내고
		const BUFF_USE_COUNT_TYPE eUseCountType = STATIC_CAST_FLOAT_TO_ENUM( BUFF_USE_COUNT_TYPE, (*pFactor)[FO_USE_BUFF_COUNT_TYPE] );

		if ( eUseCountType == m_eUseCountType )
		{
			const UCHAR ucCountNow = pGameUnit_->GetHittedCount();

			if ( false == m_bStart )
			{
				m_ucCountWhenStart = ucCountNow;
				m_bStart = true;
			}

			const UINT uiCountNew = GetNewCount( pFactor, pGameUnit_ );
			/// 기존의 것보다 새로운 것(uiCountNew)의 유지 카운트가 길면 새로운 것으로 대체
			const int iCountOld = m_iCountToBeFinished - static_cast<int>( ucCountNow );

			m_iCountToBeFinished 
				= ( iCountOld > static_cast<int>( uiCountNew ) ? m_iCountToBeFinished : static_cast<int>( ucCountNow + uiCountNew ) );
			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffHitCountFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffHittedCountFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffHittedCountFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffHittedCountFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( m_bStart && ( m_iCountToBeFinished <= static_cast<int>( pGameUnit_->GetHittedCount() ) || DidReserveToFinish() ) )
		return true;	/// 종료
	else
		return false;	/// 계속
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffHittedCountFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_iCountToBeFinished ) );	/// 버프가 끝나기 위한 카운트
	FinalizerFactor.m_vecValues.push_back( m_ucCountWhenStart );	/// 버프가 시작했을 때의 카운트
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 피격 카운트에의해 종료되는 템플릿을 셋팅
	@param : 서버로 부터 받은 종료요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffHittedCountFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_iCountToBeFinished = static_cast<int>( kFactor_[0] );
	m_ucCountWhenStart = static_cast<UCHAR>( kFactor_[1] );

	//if ( pGameUnit_->DidReceiveFirstSyncPacket() )
	{
		m_bStart = true;
		const UCHAR ucCountNow = pGameUnit_->GetHittedCount();
		/// 현재의 카운트가 시작했을 때의 카운트보다 작다면
		/// 변수의 유효범위(UCHAR:0~255)를 한바퀴 돈것이기 때문에 재설정함
		if ( ucCountNow < m_ucCountWhenStart )
			m_iCountToBeFinished -= MAX_COUNT;	
	}
// 	else
// 	{
// 		DISPLAY_ERROR( L"GameUnit didn't receive first sync packet" );
// 	}
}

/** @function : GetNewCount
	@brief : 적용할 카운트 값을 얻어냄
	@param : 버프종료요소(pFactor_), 버프에 걸린 게임유닛(pGameUnit_)
	@return : 카운트 값
*/
UINT CX2BuffHittedCountFinalizerTemplet::GetNewCount( const KBuffFinalizerFactor* pFactor_, CX2GameUnit* pGameUnit_ ) const
{
	if ( 0.0f < (*pFactor_)[FO_RESULT_COUNT] )		/// 결정된 카운트가 있다면 바로 적용(이미 진행중인 것을 서버로 부터 받는 경우 또는 시전자의 버프카운트 적용 시)
		return static_cast<UINT>( (*pFactor_)[FO_RESULT_COUNT] );
	else	/// 없다면
	{
		switch ( m_eUseCountType )
		{
		case BUCT_ENEMY_BUFF_COUNT:
			{
				return static_cast<UINT>( (*pFactor_)[FO_COUNT] * pGameUnit_->GetAppliedBuffCount( false ) );
			} break;

		case BUCT_ENEMY_DEBUFF_COUNT:
			{
				return static_cast<UINT>( (*pFactor_)[FO_COUNT] * pGameUnit_->GetAppliedBuffCount( true ) );
			} break;

		default:
			{
				DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE" );	/// 다른 타입에 이곳을 실행한다면 ERROR
				return 0;
			} break;
		}
	}		
}
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffStateChangeFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		if ( 0 == m_uiStateID )
			m_uiStateID = pGameUnit_->GetNowStateID();

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffHitCountFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffStateChangeFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffStateChangeFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffStateChangeFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return ( DidReserveToFinish() || m_uiStateID != static_cast<UINT>( pGameUnit_->GetNowStateID() ) );		/// 다르면 종료
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffStateChangeFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_uiStateID ) );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 유닛의 스테이트가 변경되면 종료되는 템플릿을 셋팅
	@param : 서버로 부터 받은 종료요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffStateChangeFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_uiStateID = static_cast<UINT>( kFactor_[0] );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingFinalizer
	@brief : 일정 데미지를 받으면 종료되는 조건 파싱
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
/*virtual*/ bool CX2BuffDamageValueFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_RELATION_TYPE", m_eRelationType, 
		BUFF_RELATION_TYPE, BRT_END, return DISPLAY_ERROR( L"BUFF_RELATION_TYPE" ) );

	return true;
}

/** @function : OnFrameMove
	@brief : 변경된 HP가 이전 프레임의 HP보다 작으면 버프가 유지될 수 있는 데미지를 감소 시킴
	@param : 버프에 걸린 유닛(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffDamageValueFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffDamageValueFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( m_bStart )
	{
		ModifyDamageValue( pGameUnit_ );

		const float fHpNowFrame = pGameUnit_->GetNowHp();

#ifndef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 이중으로 Damage Value 감소 시키는 문제 수정
		/// kimhc // 이전 프레임의 Hp 가 더 크면 공격받은 것이라고 간주(한프레임에 타격을 받고 회복한다면?)
		if ( fHpNowFrame < m_fNowHpOldFrame )
			m_fDamageValue -= (m_fNowHpOldFrame - fHpNowFrame);
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

		m_fNowHpOldFrame = fHpNowFrame;	/// 프레임의 HP가 이전프레임의 HP가 됨

	}	
}

/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffDamageValueFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		const BUFF_CHANGE_TYPE eChangeType 
			= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pFactor)[FO_CHANGE_TYPE] );
		
		if ( eChangeType == m_eChangeType )
		{
			if ( BCT_RELATION_VALUE == m_eChangeType )
			{
				const BUFF_RELATION_TYPE eRelationType 
					= STATIC_CAST_FLOAT_TO_ENUM( BUFF_RELATION_TYPE, (*pFactor)[FO_RELATION_TYPE] );

				if ( eRelationType != m_eRelationType )
					return DISPLAY_ERROR( L"BUFF_RELATION_TYPE MISS MATCH" );
			}

			const float fValue = GetValue( pFactor, pGameUnit_ );

			if ( false == m_bStart )
			{
				m_fDamageValue = fValue;
				m_fNowHpOldFrame = pGameUnit_->GetNowHp();
				m_bStart = true;
			}
			else
				m_fDamageValue = ( fValue > m_fDamageValue ? fValue : m_fDamageValue );

			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffDamageValueFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffDamageValueFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffDamageValueFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffDamageValueFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return ( DidReserveToFinish() || !(m_fDamageValue > 0) );	/// 0보다 크면 끝나지 않은것
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffDamageValueFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();
	
	FinalizerFactor.m_vecValues.push_back( m_fNowHpOldFrame );	/// 보내는 시점의 OldFrame Hp (수행할 때는 이 클래스의 OnFrameMove를 수행하고 막 m_fDamageValue가 조정되었을 때이다.
	FinalizerFactor.m_vecValues.push_back( m_fDamageValue );	/// 남은 DamageValue
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 일정 데미지릅 다으면 종료되는 템플릿을 셋팅
	@param : 서버로 부터 받은 종료요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffDamageValueFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fNowHpOldFrame = kFactor_[0];
	m_fDamageValue = kFactor_[1];		

	m_bStart = true;
	ModifyDamageValue( pGameUnit_ );
}

/** @function : GetValue
	@brief : 타입에 따른 값을 얻어옴
	@param : 종료요소(pFinalizerFactor_), 게임유닛(pGameUnit_)
	@return : 타입에 따른 결과 값
*/
float CX2BuffDamageValueFinalizerTemplet::GetValue( const KBuffFinalizerFactor* pFinalizerFactor_, CX2GameUnit* pGameUnit_ ) const
{
	const float fValue = (*pFinalizerFactor_)[FO_VALUE];

	switch ( m_eChangeType )
	{
	case BCT_FIX_VALUE:
		{
			return fValue;
		} break;

	case BCT_RELATION_VALUE:
		{
			return fValue * (*pFinalizerFactor_)[FO_MULTIPLIER];
		} break;

	default:
		return 0.0f;
		break;
	}
}

/** @function : ModifyDamageValue
	@brief : 이전 프레임과 현재 HP를 비교하여 종료될 데미지양을 보정함
	@param : 버프가 적용된 게임유닛
*/
void CX2BuffDamageValueFinalizerTemplet::ModifyDamageValue( const CX2GameUnit* pGameUnit_ )
{
	const float fNowHp = pGameUnit_->GetNowHp();
	if ( fNowHp > m_fNowHpOldFrame )
		m_fDamageValue += fNowHp - m_fNowHpOldFrame;	/// 현재의 Hp가 패킷을 보냈을때의 Hp보다 크면 차 만큼 더해주고
	else
		m_fDamageValue -= m_fNowHpOldFrame - fNowHp;	/// 현재의 Hp가 패킷을 보냈을때의 Hp보다 작으면 차 만큼 빼준다
	/// 패킷 전송될때까지 Hp를 채운것이라면 어떻게 하나...
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingFinalizer
	@brief : 기준 HP를 상향 또는 하향 돌파 하면 종료되는 클래스를 파싱
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
/*virtual*/ bool CX2BuffPassHpFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "PASS_UP", m_bPassUp, false, return DISPLAY_ERROR( L"PASS_UP" ) );
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eBuffChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
	return true;
}

/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffPassHpFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		const bool bPassUp = ( (*pFactor)[FO_PASS_UP] > 0.0f ? true : false );	/// 상향 돌파 인가 하향돌파인가?
		if ( bPassUp == m_bPassUp )
		{
			const BUFF_CHANGE_TYPE eBuffChangeType = STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pFactor)[FO_CHANGE_TYPE] );		/// % 인가 값 인가?
			if ( eBuffChangeType == m_eBuffChangeType )
			{
				SelectDelegate();

				const float fCriterion = (*pFactor)[FO_CRITERION];	/// 기준 수치
				m_fCriterion = ( m_fCriterion > fCriterion ? m_fCriterion : fCriterion );
				return true;
			}
			else
				return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE MISS MATCH" );
		}
		else
			return DISPLAY_ERROR( L"PASS_UP MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffPassHpFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffPassHpFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffPassHpFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffPassHpFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return DidReserveToFinish() || m_delegateCheckHpByPassTypeFunc( pGameUnit_ );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffPassHpFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( m_fCriterion );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 일정 HP를 상향 또는 하향 돌파하면 종료되는 템플릿을 셋팅
	@param : 서버로 부터 받은 종료요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffPassHpFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fCriterion = kFactor_[0];
	SelectDelegate();
}

/** @function : SelectDelegate
	@brief : 변경타입에 따라 다르게 수행할 함수 선택
*/
void CX2BuffPassHpFinalizerTemplet::SelectDelegate()
{
	if ( m_bPassUp )
		m_delegateCheckHpByPassTypeFunc = DelegateCheckHpByPassType::from_const_method<CX2BuffPassHpFinalizerTemplet, &CX2BuffPassHpFinalizerTemplet::CheckHpByUp>( this );
	else
		m_delegateCheckHpByPassTypeFunc = DelegateCheckHpByPassType::from_const_method<CX2BuffPassHpFinalizerTemplet, &CX2BuffPassHpFinalizerTemplet::CheckHpByDown>( this );		

	switch ( m_eBuffChangeType )
	{
	case BCT_FIX_VALUE:	
		m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassHpFinalizerTemplet, &CX2BuffPassHpFinalizerTemplet::GetCriterionByValue>( this );
		break;

	case BCT_PERCENT:
		m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassHpFinalizerTemplet, &CX2BuffPassHpFinalizerTemplet::GetCriterionByPercent>( this );
		break;

	default:
		{
			DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" );
			m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassHpFinalizerTemplet, &CX2BuffPassHpFinalizerTemplet::GetCriterionByValue>( this );
		} break;
	}
}

/** @function : CheckHpByUp
	@brief : 상향돌파 여부를 체크
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : 상향돌파시 true 아니면 false
*/
bool CX2BuffPassHpFinalizerTemplet::CheckHpByUp( CX2GameUnit* pGameUnit_ ) const
{
	/// 현재 HP가 상향 돌파
	return pGameUnit_->GetNowHp() > m_delegateGetCriterionByChangeTypeFunc( pGameUnit_ );
}

/** @function : CheckHpByDown
	@brief : 하향돌파 여부를 체크
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : 하향돌파시 true 아니면 false
*/
bool CX2BuffPassHpFinalizerTemplet::CheckHpByDown( CX2GameUnit* pGameUnit_ ) const
{
	/// 현재 HP가 하향 돌파
	return pGameUnit_->GetNowHp() < m_delegateGetCriterionByChangeTypeFunc( pGameUnit_ );
}

/** @function : GetCriterionByValue
	@brief : 기준이 수치로 정해져 있어 수치를 리턴
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : 기준 수치
*/
float CX2BuffPassHpFinalizerTemplet::GetCriterionByValue( CX2GameUnit* pGameUnit_ ) const
{
	return m_fCriterion;	/// 수치
}

/** @function : GetCriterionByPercent
	@brief : 기준이 %로 정해져 있어 MaxHp 대비 Percent를 기준 수치로 리턴
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : 기준 수치(%)
*/
float CX2BuffPassHpFinalizerTemplet::GetCriterionByPercent( CX2GameUnit* pGameUnit_ ) const
{
	return m_fCriterion * pGameUnit_->GetMaxHp();	/// 최대HP 대비 수치
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingFinalizer
	@brief : 기준 Mp를 상향 또는 하향 돌파 하면 종료되는 클래스를 파싱
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_)
	@return : 성공시 true, 실패시 false
*/
/*virtual*/ bool CX2BuffPassMpFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "PASS_UP", m_bPassUp, false, return DISPLAY_ERROR( L"PASS_UP" ) );
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eBuffChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
	return true;
}

/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffPassMpFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		const bool bPassUp = ( (*pFactor)[FO_PASS_UP] > 0.0f ? true : false );	/// 상향 돌파 인가 하향돌파인가?
		if ( bPassUp == m_bPassUp )
		{
			const BUFF_CHANGE_TYPE eBuffChangeType = STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pFactor)[FO_CHANGE_TYPE] );		/// % 인가 값 인가?
			if ( eBuffChangeType == m_eBuffChangeType )
			{
				SelectDelegate();
				const float fCriterion = (*pFactor)[FO_CRITERION];	/// 기준 수치
				m_fCriterion = ( m_fCriterion > fCriterion ? m_fCriterion : fCriterion );
				return true;
			}
			else
				return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE MISS MATCH" );
		}
		else
			return DISPLAY_ERROR( L"PASS_UP MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffPassMpFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffPassMpFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffPassMpFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffPassMpFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return DidReserveToFinish() || m_delegateCheckMpByPassTypeFunc( pGameUnit_ );
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffPassMpFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( m_fCriterion );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 일정 MP를 상향 또는 하향 돌파하면 종료되는 템플릿을 셋팅
	@param : 서버로 부터 받은 종료요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffPassMpFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fCriterion = kFactor_[0];
	SelectDelegate();
}

/** @function : SelectDelegate
	@brief : 변경타입에 따라 다르게 수행할 함수 선택
*/
void CX2BuffPassMpFinalizerTemplet::SelectDelegate()
{
	if ( m_bPassUp )
		m_delegateCheckMpByPassTypeFunc = DelegateCheckMpByPassType::from_const_method<CX2BuffPassMpFinalizerTemplet, &CX2BuffPassMpFinalizerTemplet::CheckMpByUp>( this );
	else
		m_delegateCheckMpByPassTypeFunc = DelegateCheckMpByPassType::from_const_method<CX2BuffPassMpFinalizerTemplet, &CX2BuffPassMpFinalizerTemplet::CheckMpByDown>( this );		

	switch ( m_eBuffChangeType )
	{
	case BCT_FIX_VALUE:	
		m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassMpFinalizerTemplet, &CX2BuffPassMpFinalizerTemplet::GetCriterionByValue>( this );
		break;

	case BCT_PERCENT:
		m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassMpFinalizerTemplet, &CX2BuffPassMpFinalizerTemplet::GetCriterionByPercent>( this );
		break;

	default:
		{
			DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" );
			m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassMpFinalizerTemplet, &CX2BuffPassMpFinalizerTemplet::GetCriterionByValue>( this );
		} break;
	}
}

/** @function : CheckMpByUp
	@brief : 상향돌파 여부를 체크
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : 상향돌파시 true 아니면 false
*/
bool CX2BuffPassMpFinalizerTemplet::CheckMpByUp( CX2GameUnit* pGameUnit_ ) const
{
	/// 현재 Mp가 상향 돌파
	return pGameUnit_->GetNowMp() > m_delegateGetCriterionByChangeTypeFunc( pGameUnit_ );
}

/** @function : CheckMpByDown
	@brief : 하향돌파 여부를 체크
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : 하향돌파시 true 아니면 false
*/
bool CX2BuffPassMpFinalizerTemplet::CheckMpByDown( CX2GameUnit* pGameUnit_ ) const
{
	/// 현재 Mp가 하향 돌파
	return pGameUnit_->GetNowMp() < m_delegateGetCriterionByChangeTypeFunc( pGameUnit_ );
}

/** @function : GetCriterionByValue
	@brief : 기준이 수치로 정해져 있어 수치를 리턴
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : 기준 수치
*/
float CX2BuffPassMpFinalizerTemplet::GetCriterionByValue( CX2GameUnit* pGameUnit_ ) const
{
	return m_fCriterion;	/// 수치
}

/** @function : GetCriterionByPercent
	@brief : 기준이 %로 정해져 있어 MaxMp 대비 Percent를 기준 수치로 리턴
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : 기준 수치(%)
*/
float CX2BuffPassMpFinalizerTemplet::GetCriterionByPercent( CX2GameUnit* pGameUnit_ ) const
{
	return m_fCriterion * pGameUnit_->GetMaxMp();	/// 최대Mp 대비 수치
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffImmediateFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffImmediateFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffImmediateFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffImmediateFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffImmediateFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return true;
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffImmediateFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffDungeonStageChangeFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		if ( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
		{
			CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
			m_uiIndex = pDungeonGame->GetNowStageIndex();
			return true;
		}
		else
			return false;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffDungeonStageChangeFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffDungeonStageChangeFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffDungeonStageChangeFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffDungeonStageChangeFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
		if ( DidReserveToFinish() || pDungeonGame->GetNowStageIndex() != m_uiIndex )	/// 버프를 시작 되었을 때하고 스테이지가 다르면
			return true;	/// 종료
		else
			return false;
	}
	else
		return true;
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffDungeonStageChangeFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_uiIndex ) );
}


/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 종료 요소를 셋팅 하여 작동 시킴
	@param : 서버로 부터 받은 종료요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffDungeonStageChangeFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// 버프가 시작되었을 때의 스테이지
	m_uiIndex = static_cast<UINT>( kFactor_[0] );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ADD_LIVE_CREATOR_FINALIZER
/** @function : OnFrameMove;
	@brief : 매 프레임 마다 버프 시전 유닛이 살아있거나 존재하는지 확인
	@param : 버프에 걸린 유닛의 포인터(pGameUnit_)
	@return : 
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffLiveCreatorFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffLiveCreatorFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if( null == m_optrCreatorUnit || 0.f >= m_optrCreatorUnit->GetNowHp() )		/// 시전자가 죽거나 없는지 검사
		m_bIsLiveCreator = false;
}

/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffLiveCreatorFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		m_uidCreatorUID = BuffFactor_.GetUidGameUnit();

		if( true == BuffFactor_.GetIsUserUID() )	/// 유저일 경우
		{
			m_bIsUserUnit = true;
			CX2GUUser* pCreatroUnit = static_cast<CX2GUUser*>( g_pX2Game->GetUserUnitByUID( m_uidCreatorUID ) );

			if( NULL != pCreatroUnit && 0.f < pCreatroUnit->GetNowHp() )
			{
				m_optrCreatorUnit = CX2GameUnitoPtr( pCreatroUnit );	/// 버프 생성자 유닛 객체 저장
				m_bIsLiveCreator  = true;		/// 버프 시전자 존재 혹은 살아있음

				return true;
			}
		}
		else	/// NPC일 경우
		{
			CX2GUNPC* pCreatroUnit = static_cast<CX2GUNPC*>( g_pX2Game->GetNPCUnitByUID( static_cast<int>( m_uidCreatorUID ) ) );

			if( NULL != pCreatroUnit && 0.f < pCreatroUnit->GetNowHp() )
			{
				m_optrCreatorUnit = CX2GameUnitoPtr( pCreatroUnit );	/// 버프 생성자 유닛 객체 저장
				m_bIsLiveCreator  = true;		/// 버프 시전자 존재 혹은 살아있음

				return true;
			}
		}

		m_bIsLiveCreator  = false;		/// 버프 시전자 사망 혹은 없음
	}
	else
		return DISPLAY_ERROR( L"CX2BuffPassMpFinalizerTemplet GetFinalizerFactor didn't work" );

	return true;
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffLiveCreatorFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffLiveCreatorFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffLiveCreatorFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( NULL != g_pX2Game )
	{
		if( true == m_bIsLiveCreator )		/// 버프 시전자가 살아있으면, 버프 계속 진행
			return false;
	}
	
	return true;							/// 버프 시전자가 사망하거나 존재하지 않으면, 종료
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffLiveCreatorFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	/// int64를 16비트씩 float 4개에 쪼개서 넣음...
	float fUnitUIDBackDown	=  static_cast<float>( m_uidCreatorUID & 0xFFFF );
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( fUnitUIDBackDown ) );	/// 유닛 UID 하위 아래 비트

	float fUnitUIDBackUp	=  static_cast<float>( ( m_uidCreatorUID >> 16 ) & 0xFFFF );
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( fUnitUIDBackUp ) );		/// 유닛 UID 하위 위 비트

	float fUnitUIDFrontDown =  static_cast<float>( ( m_uidCreatorUID >> 32 ) & 0xFFFF );
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( fUnitUIDFrontDown ) );	/// 유닛 UID 상위 아래 비트

	float fUnitUIDFrontUp	=  static_cast<float>( ( m_uidCreatorUID >> 48 ) & 0xFFFF );
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( fUnitUIDFrontUp ) );		/// 유닛 UID 상위 위 비트

	/// 유저인지 NPC인지 여부
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_bIsLiveCreator ) );
}


/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 종료 요소를 셋팅 하여 작동 시킴
	@param : 서버로 부터 받은 종료요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffLiveCreatorFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// 16비트씩 float 4개로 쪼개진 UID를 int64를 병합

	UidType uidCreatorUID1 = static_cast<UidType>( kFactor_[FO_UNIT_UID_BACK_DOWN] );	/// 유닛 UID 하위 아래 비트
	UidType uidCreatorUID2 = static_cast<UidType>( kFactor_[FO_UNIT_UID_BACK_UP] );		/// 유닛 UID 하위 위 비트
	UidType uidCreatorUID3 = static_cast<UidType>( kFactor_[FO_UNIT_UID_FRONT_DOWN] );	/// 유닛 UID 상위 아래 비트
	UidType uidCreatorUID4 = static_cast<UidType>( kFactor_[FO_UNIT_UID_FRONT_UP] );	/// 유닛 UID 상위 위 비트

	/// 각 비트 위치 조절
	uidCreatorUID2 = uidCreatorUID2 << 16;
	uidCreatorUID3 = uidCreatorUID3 << 32;
	uidCreatorUID4 = uidCreatorUID4 << 48;

	m_uidCreatorUID = uidCreatorUID1 | uidCreatorUID2 | uidCreatorUID3 | uidCreatorUID4;	/// 비트 합체
	
	if( false == IsSamef( kFactor_[FO_IS_USER_UNIT], 0.f ) )	/// 유저일 경우
	{
		m_bIsUserUnit = true;
		CX2GUUser* pCreatroUnit = static_cast<CX2GUUser*>( g_pX2Game->GetUserUnitByUID( m_uidCreatorUID ) );

		if( NULL != pCreatroUnit && 0.f < pCreatroUnit->GetNowHp() )
		{
			m_optrCreatorUnit = CX2GameUnitoPtr( pCreatroUnit );	/// 버프 생성자 유닛 객체 저장
			m_bIsLiveCreator  = true;		/// 버프 시전자 존재 혹은 살아있음

			return;
		}
	}
	else		/// NPC일 경우
	{
		CX2GUNPC* pCreatroUnit = static_cast<CX2GUNPC*>( g_pX2Game->GetNPCUnitByUID( static_cast<int>( m_uidCreatorUID ) ) );

		if( NULL != pCreatroUnit && 0.f < pCreatroUnit->GetNowHp() )
		{
			m_optrCreatorUnit = CX2GameUnitoPtr( pCreatroUnit );	/// 버프 생성자 유닛 객체 저장
			m_bIsLiveCreator  = true;		/// 버프 시전자 존재 혹은 살아있음

			return;
		}
	}

	m_bIsLiveCreator  = false;		/// 버프 시전자 사망 혹은 없음
}
#endif ADD_LIVE_CREATOR_FINALIZER

#ifdef ADD_BUFF_FINALIZER_FINISH_HYPER
/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffFinishHyperFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		if ( NULL != pGameUnit_ )
		{
			CX2GUUser* pCreatroUnit = static_cast<CX2GUUser*>( pGameUnit_ );

			if( NULL != pCreatroUnit && 0.f < pCreatroUnit->GetHyperModeTime() )
			{
				m_bHyper = true;
				m_eBuffTempleteID = BuffFactor_.GetBuffTempletID();
				m_bIsAlreadyNotifyDurationTime = false;

				return true;
			}
			else
			{
				m_bHyper = false;

				return false;
			}
		}

		m_bHyper = false;
		return false;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffFinishHyperFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffFinishHyperFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffFinishHyperFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffFinishHyperFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( DidReserveToFinish() || false == m_bHyper )
		return true;
	else
		return false;
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffFinishHyperFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( ( true == m_bHyper ? 1.0f : 0.0f ) );
}

/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 종료 요소를 셋팅 하여 작동 시킴
	@param : 서버로 부터 받은 종료요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffFinishHyperFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// 버프가 시작되었을 때의 스테이지
	m_bHyper = ( kFactor_[0] == 1.f ? true : false );
}

/** @function : OnFrameMove;
	@brief : 매 프레임 확인
	@return : 
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffFinishHyperFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffFinishHyperFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( NULL != pGameUnit_ )
	{
		CX2GUUser* pCreatroUnit = static_cast<CX2GUUser*>( pGameUnit_ );

		if( NULL != pCreatroUnit && 0.f >= pCreatroUnit->GetRemainHyperModeTime() )
			m_bHyper = false;
		else
			m_bHyper = true;

		m_fDurationTime = pCreatroUnit->GetRemainHyperModeTime();

		if( false == m_bIsAlreadyNotifyDurationTime
			&& 5.f >= m_fDurationTime )
		{
			CX2GageManager::GetInstance()->NotifyDurationTime5sec( pGameUnit_->GetUnitUID(), m_eBuffTempleteID );
			m_bIsAlreadyNotifyDurationTime = true;
		}
		else if ( true == m_bIsAlreadyNotifyDurationTime && 5.f < m_fDurationTime )
		{
			CX2GageManager::GetInstance()->InitBuffIconFlicker( pGameUnit_->GetUnitUID(), m_eBuffTempleteID );
			m_bIsAlreadyNotifyDurationTime = false;
		}
	}
}
#endif //ADD_BUFF_FINALIZER_FINISH_HYPER

#ifdef RIDING_SYSTEM
/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffFinishRidingPetOnOrNotFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffFinishRidingPetOnOffFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffFinishRidingPetOnOrNotFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffFinishRidingPetOnOrNotFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffFinishRidingPetOnOrNotFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( NULL != pGameUnit_ && 
		 ( pGameUnit_->GetRidingOn() != m_bFinishRidingOn ) )	/// 종료 조건과 반대면
		 return false;	/// 종료하지 않고
	else	/// pGameUnit이 없거나, 종료조건과 일치하면
		return true;	/// 버프 종료
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffFinishRidingPetOnOrNotFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
}

/** @function : ParsingFinalizerTemplateMethod
	@brief : 각 파생클래스에서 구현된 ParsingFinalizer를 똑같은 전후 처리 사이에서 호출하기 위한 템플릿메소드
	@param : 읽어들이고 있는 루아스크립트의 루아매니저(luaManager_), 파싱성공한 FinalizerTempletPtr을 담을 vector(vecBuffFinalizerTempletPtr_)
	@return : 파싱 성공시 true, 실패시 false 리턴
*/
bool CX2BuffFinishRidingPetOnOrNotFinalizerTemplet::ParsingFinalizerTemplateMethod( KLuaManager& luaManager_, const char* pszTableName_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "FINISH_WHEN_RIDING_ON", m_bFinishRidingOn, false, 
		return DISPLAY_ERROR( L"FINISH_WHEN_RIDING_ON" ) );

	return true;
}
#endif //RIDING_SYSTEM


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : 종료 요소를 템플릿에 셋팅하여 작동 시킴
	@param : 종료 요소(factor_)
*/
/*virtual*/ bool CX2BuffX2StateChangeFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		if( 0 == m_uiIndex )
			m_uiIndex = static_cast<UINT>(g_pMain->GetNowStateID());
		
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffGameStateChangeFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : 자신을 복사한 객체의 스마트포인터를 리턴하는 함수
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffX2StateChangeFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffX2StateChangeFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : 해당 종료조건을 가진 버프가 끝났는지를 검사하는 함수
	@param : 버프에 걸린 게임유닛(pGameUnit_)
	@return : true 면 종료, false 면 진행
*/
/*virtual*/ bool CX2BuffX2StateChangeFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return ( DidReserveToFinish() || m_uiIndex != static_cast<UINT>( g_pMain->GetNowStateID() ) );		/// 다르면 종료
}

/** @function : GetFactor
	@brief : 템플릿 인자들 중 필요 한 것들을 factor로 변환
	@param : 버프요소 인스턴스(factor_)
*/
/*virtual*/ void CX2BuffX2StateChangeFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_uiIndex ) );
}


/** @function : SetFactorFromPacket
	@brief : 패킷으로 부터 받은 버프 정보로 종료 요소를 셋팅 하여 작동 시킴
	@param : 서버로 부터 받은 종료요소(kFactor_), 적용할 게임유닛(pGameUnit_)
*/
/*virtual*/ void CX2BuffX2StateChangeFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// 버프가 시작되었을 때의 스테이트
	m_uiIndex = static_cast<UINT>( kFactor_[0] );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
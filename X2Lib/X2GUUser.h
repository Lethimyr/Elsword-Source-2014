#pragma once

typedef srutil::delegate0<void> GUStateCBFunc;

typedef srutil::delegate1<void, CKTDGLineMap*> DelegatePhysicProcessPortalByGameType;

//{{ kimhc // 2010.11.3 // 레피 - 승화된 분노
#ifdef	NEW_SKILL_2010_11

// 각성할 수 있는지 (재각성 할 수 있는지 여부 추가)
#define ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( hyperModeState ) \
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && \
	m_InputData.oneLCtr == true && m_FrameDataFuture.syncData.m_HyperModeCount > 0 && \
	( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) ) \
	{ StateChange( hyperModeState ); }

#else

// 각성할 수 있는지
#define ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( hyperModeState ) \
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && \
	m_InputData.oneLCtr == true && m_FrameDataFuture.syncData.m_HyperModeCount > 0 && GetRemainHyperModeTime() <= 0.0f ) \
	{ StateChange( hyperModeState ); }

#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.3 // 레피 - 승화된 분노


// kimhc // 2010-11-30 // 콤보 중 방향키를 사용하여 캔슬 가능 여부를 판단 (반대 방향으로만 캔슬 가능)
#define CAN_WALK_CANCEL \
	( ( true == m_InputData.pureRight &&  false == m_FrameDataFuture.syncData.bIsRight ) || \
		( true == m_InputData.pureLeft && true == m_FrameDataFuture.syncData.bIsRight ) )

// kimhc // 2010-11-27 // 콤보 중 대시를 사용하여 캔슬 가능 여부를 판단
#define CAN_DASH_CANCEL \
	( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )

//{{오현빈 // 2012-04-30 // 스킬슬롯 체인지 없이 확장 스킬 사용 할 수 있도록 수정
// 특정 애니메이션 시간이 지난 후에 방향키, ZXASDCQWER를 눌렀을 때 (QWER추가)
#define ELSE_IF_ARROW_ZXASDC_PRESSED_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( m_InputData.pureUp == true || m_InputData.pureDown == true || m_InputData.pureRight == true || m_InputData.pureLeft == true \
	|| m_InputData.pureZ == true || m_InputData.pureX == true \
	|| m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true) )



// 특정 애니메이션 시간이 지난 후에 방향키, ASDCQWER를 눌렀을 때 (QWER추가)
#define ELSE_IF_ARROW_ASDC_PRESSED_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( m_InputData.pureUp == true || m_InputData.pureDown == true || m_InputData.pureRight == true || m_InputData.pureLeft == true \
	|| m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true) )





// 특정 애니메이션 시간이 지난 후에 ASDCQWER를 눌렀을 때 (QWER추가)
#define ELSE_IF_ASDC_PRESSED_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true) )



// 특정 애니메이션 시간이 지난 후에 ZX를 눌렀을 때
#define ELSE_IF_ZX_PRESSED_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	(m_InputData.pureZ == true || m_InputData.pureX == true) )

//}}오현빈 // 2012-04-30 // 스킬슬롯 체인지 없이 확장 스킬 사용 할 수 있도록 수정

#ifdef ELSWORD_SHEATH_KNIGHT
#define BWALK_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true) || \
	( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false) ) ) \
	{ StateChange( USI_WAIT ); }

#define WALK_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureRight || true == m_InputData.pureLeft ) ) \
	{ StateChange( USI_WAIT ); }

#define WALK_CANCEL_AFTER_CANNOT_ATTACKTIME( time, cannotattacktime ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureRight || true == m_InputData.pureLeft ) ) \
	{ StateChange( USI_WAIT ); m_fCanNotAttackTime = cannotattacktime; }

#define DASH_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft ) ) \
	{ StateChangeDashIfPossible(); }
	
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환

	/// 스킬 캔슬 메크로에 각성 캔슬도 추가
	#define SKILL_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	SpecialAttackEventProcess() == true ) \
	{ } \
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && \
	m_InputData.oneLCtr == true && m_FrameDataFuture.syncData.m_HyperModeCount > 0 && \
	( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) && \
	(time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) ) \
	{ StateChange( GetHypetMpdeStateID() ); }

	/// if로 시작하는 스킬 캔슬 메크로가 필요하여 추가 ( 위의 스킬 캔슬 메크로와 완전 동일 )
	#define IF_SKILL_CANCEL_AFTER( time ) \
	if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	SpecialAttackEventProcess() == true ) \
	{ } \
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && \
	m_InputData.oneLCtr == true && m_FrameDataFuture.syncData.m_HyperModeCount > 0 && \
	( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) && \
	(time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) ) \
	{ StateChange( GetHypetMpdeStateID() ); }

#else // SKILL_CANCEL_BY_HYPER_MODE

	#define SKILL_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	SpecialAttackEventProcess() == true ) \
	{ }
#endif //SKILL_CANCEL_BY_HYPER_MODE

// 특정 애니메이션 시간이 지난 후에 ZX가 눌려져 있을 때
#define ZX_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) )\
	{ if ( m_InputData.pureZ == true ) StateChange( GetComboZStateID() ); \
      else if ( m_InputData.pureX == true ) StateChange( GetComboXStateID() ); }

// 특정 애니메이션 시간이 지난 후에 ZX가 눌렀을 때( OneX, OneZ )
#define ZX_CANCEL_AFTER_EX( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) )\
	{ if ( m_InputData.oneZ == true ) StateChange( GetComboZStateID() ); \
	  else if ( m_InputData.oneX == true ) StateChange( GetComboXStateID() ); }
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_RAVEN_VETERAN_COMMANDER		/// 공중에서 사용할 수 있는 스킬에 대해서만 캔슬할 수 있다.

#define SKILL_CANCEL_AFTER_FLY( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) == true ) \

#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef RIDING_SYSTEM
#define RIDING_BWALK_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && \
	m_ptrRidingPet->GetXSkinAnimFuturePtr()->GetNowAnimationTime() > (time) && \
	( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true) || \
	( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false) ) ) \
	{ StateChange( USI_RIDING_WAIT ); }

#define RIDING_WALK_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && \
	m_ptrRidingPet->GetXSkinAnimFuturePtr()->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureRight || true == m_InputData.pureLeft ) ) \
	{ StateChange( USI_RIDING_WAIT ); }

#define RIDING_WALK_CANCEL_AFTER_CANNOT_ATTACKTIME( time, cannotattacktime ) \
	else if( (time) >= 0.f NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && \
	m_ptrRidingPet->GetXSkinAnimFuturePtr()->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureRight || true == m_InputData.pureLeft ) ) \
	{ StateChange( USI_RIDING_WAIT ); m_fCanNotAttackTime = cannotattacktime; }

#define RIDING_DASH_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && \
	m_ptrRidingPet->GetXSkinAnimFuturePtr()->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft ) ) \
	{ StateChangeDashIfPossible(); }

#define RIDING_SKILL_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && \
	m_ptrRidingPet->GetXSkinAnimFuturePtr()->GetNowAnimationTime() > (time) && \
	RidingPetSpecialAttackEventProcess() == true ) \
	{ }
#endif //RIDING_SYSTEM

////////// 이 밑으로의 스킵 Define은 옛날 것이므로 되도록 위의 것을 사용해주세요!! //////////////



//{{오현빈 // 2012-04-30 // 스킬슬롯 체인지 없이 확장 스킬 사용 할 수 있도록 수정
// 스킵 포인트에 필살기 키가 눌려졌는지 (QWER추가)
#define ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C \
	else if( m_InputData.pureUp == true || m_InputData.pureDown == true || m_InputData.pureRight == true || m_InputData.pureLeft == true \
	|| m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true )



// 스킵 포인트에 ZX 필살기 (QWER추가)
#define ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC \
	else if( m_InputData.pureUp == true || m_InputData.pureDown == true || m_InputData.pureRight == true || m_InputData.pureLeft == true \
	|| m_InputData.pureZ == true || m_InputData.pureX == true \
	|| m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true )



// 스킵 포인트에 ZX 필살기, 각성, 즉, 아무키나 다 (QWER추가)
#define ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS \
	else if( m_InputData.pureUp == true || m_InputData.pureDown == true || m_InputData.pureRight == true || m_InputData.pureLeft == true \
	|| m_InputData.pureZ == true || m_InputData.pureX == true \
	|| m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true \
	|| m_InputData.pureLCtr == true )
//}}오현빈 // 2012-04-30 // 스킬슬롯 체인지 없이 확장 스킬 사용 할 수 있도록 수정


// 방향이 바뀌면 선입력된 콤보를 취소시킨다
#define CANCEL_COMBO_ON_DIRECTION_CHANGE( eventFlagCount ) \
	if( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true) || \
		( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false) ) \
	{ for( int i=0; i< (int)(eventFlagCount); i++ ) m_FrameDataFuture.stateParam.bEventFlagList[i] = false; } 



// 0~지정된 시간사이에 특정 조건을 만족하면 flag = true로 만들고, 지정된 시간이 되었을 때 flag == true이면 지정된 상태로 바뀐다
#define ELSE_IF_STATE_CHANGE_ON_( iEventFlagIndex, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	else if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > (float)(fChangeTiming) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }

#ifdef ELSWORD_SHEATH_KNIGHT
// 0~지정된 시간사이에 특정 조건을 만족하면 flag = true로 만들고, 지정된 시간이 되었을 때 flag == true이면 지정된 상태로 바뀐다
#define IF_STATE_CHANGE_ON_EX_( iEventFlagIndex, fInputTimingStart, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > (float)(fChangeTiming) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) && m_pXSkinAnimFuture->GetNowAnimationTime() >= (float)(fInputTimingStart) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#define ELSE_IF_STATE_CHANGE_ON_EX_( iEventFlagIndex, fInputTimingStart, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	else if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > (float)(fChangeTiming) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) && m_pXSkinAnimFuture->GetNowAnimationTime() >= (float)(fInputTimingStart) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#endif ELSWORD_SHEATH_KNIGHT

#ifdef NEW_CHARACTER_EL
#define IF_STATE_CHANGE_ON_EX_STRUCT( iEventFlagIndex, iEventTImeIndex, bCondition, iStateID ) \
	if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > (float)(m_fEventTime[iEventTImeIndex].stateChange) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(m_fEventTime[iEventTImeIndex].keyInputEnd) && m_pXSkinAnimFuture->GetNowAnimationTime() >= (float)(m_fEventTime[iEventTImeIndex].keyInputStart) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#define ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( iEventFlagIndex, iEventTImeIndex, bCondition, iStateID ) \
	else if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > (float)(m_fEventTime[iEventTImeIndex].stateChange) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(m_fEventTime[iEventTImeIndex].keyInputEnd) && m_pXSkinAnimFuture->GetNowAnimationTime() >= (float)(m_fEventTime[iEventTImeIndex].keyInputStart) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
// 0~지정된 시간사이에 특정 조건을 만족하면 flag = true로 만든다
#define ELSE_IF_SET_EVENT_FLAG( iEventFlagIndex, fInputTimingStart, fInputTiming, bCondition ) \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) && m_pXSkinAnimFuture->GetNowAnimationTime() >= (float)(fInputTimingStart) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#endif // SERV_ARA_CHANGE_CLASS_SECOND

// 공중에 떠 있는 시간이 일정시간 이상되면 wait 상태로 바꾼다
//#ifdef LINEMAP_FAST_WIND_TEST
//	#define IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( time, targetState ) \
//		if( false == m_bWaitInTheAir ) { m_bWaitInTheAir = true; m_fTimeInTheAir = (time); } \
//		else if( 0.f == m_fTimeInTheAir ) StateChange( (targetState) ); 
//#endif LINEMAP_FAST_WIND_TEST

#define MAGIC_OXYGEN_GAGE	200.f


#ifdef X2OPTIMIZE_NPC_NONHOST_SIMULATION
class CX2GUNPC;
#endif // X2OPTIMIZE_NPC_NONHOST_SIMULATION



#ifdef SUPERPOSITION_HYPER_MODE_BUG
#define IF_HYPER_MODE_STATE( stateID_ ) \
	if( USI_HYPER_MODE == stateID_ || USI_RIDING_HYPER_MODE == stateID_ )
#endif // SUPERPOSITION_HYPER_MODE_BUG


class CX2GUUser : public CX2GameUnit
{
public:
	//{{ Public Variable Start
#ifdef KHELP_SYSTEM_TEST
	DECLARE_HELP_SYSTEM( CX2GUUser );
#endif KHELP_SYSTEM_TEST

	void Debug_RenderHelp( int ix_, int iy_ );

	enum USER_STATE_ID
	{
		USI_BASE = GUSI_END + 1,

		USI_DIE_FRONT,						/// 앞으로 쓰러져서 죽는 스테이트
		USI_DIE_BACK,						/// 뒤로 쓰러져서 죽는 스테이트
		
		USI_START,							/// 던전, 대전 시작할 때의 스테이트
		USI_START_INTRUDE,					/// 대전 등에서 중간에 난입 했을 때의 스테이트
		
		USI_WIN,							/// 승리 했을 때의 스테이트
		USI_LOSE,							/// 패배 했을 때의 스테이트

		USI_WAIT,							/// 가만히 서 있는 스테이트
		USI_WALK,							/// 걷는 스테이트
		USI_JUMP_READY,						/// 점프 전 스테이트
		USI_JUMP_UP,						/// 점프 중 스테이트
		USI_JUMP_DOWN,						/// 점프 한 후, 착지 전 하강하는 스테이트
		USI_JUMP_LANDING,					/// 착지 한 스테이트
		USI_DASH,							/// 대시하는 스테이트
		USI_DASH_END,						/// 대시하다가 정지하는 스테이트
		USI_DASH_JUMP,						/// 대시 점프 하는 스테이트
		USI_DASH_JUMP_LANDING,				/// 대시 점프 후 착지 하는 스테이트
				
		USI_HYPER_MODE,						/// 각성 하는 스테이트

		USI_DAMAGE_GROGGY,					/// HP가 많이 소모되어 힘들어하는 스테이트
		USI_DAMAGE_SMALL_FRONT,				/// 전방에서 SMALL_TYPE의 공격을 맞았을 때의 스테이트
		USI_DAMAGE_SMALL_BACK,				/// 후방에서 SMALL_TYPE의 공격을 맞았을 때의 스테이트
		USI_DAMAGE_BIG_FRONT,				/// 전방에서 BIG_TYPE의 공격을 맞았을 때의 스테이트
		USI_DAMAGE_BIG_BACK,				/// 후방에서 BIG_TYPE의 공격을 맞았을 때의 스테이트
		USI_DAMAGE_DOWN_FRONT,				/// 전방에서 공격을 받아 DOWN 되었을 때의 스테이트
		USI_DAMAGE_DOWN_BACK,				/// 후방에서 공격을 받아 DOWN 되었을 때의 스테이트
		USI_DAMAGE_STANDUP_FRONT,			/// 전방에서 공격을 받아서 DOWN된 후에 일어나는 스테이트
		USI_DAMAGE_STANDUP_BACK,			/// 후방에서 공격을 받아서 DOWN된 후에 일어나는 스테이트
		USI_DAMAGE_AIR_SMALL,				/// 공중에 떠 있는 상태에서 SMALL_TYPE의 공격을 받았을 때의 스테이트
		USI_DAMAGE_AIR_DOWN,				/// 공중에 떠 있는 상태에서 DOWN_TYPE의 공격을 받았을 때의 스테이트
		USI_DAMAGE_AIR_DOWN_INVINCIBLE,		/// 데미지를 일정 이상 받아서 DOWN 되는 스테이트 (DOWN 공격에 의해서가 아님)
		USI_DAMAGE_AIR_DOWN_LANDING,		/// 공중에서 DOWN되어 쓰러지는 스테이트
		USI_DAMAGE_AIR_FALL,				/// UP_TYPE의 공격에 의해 수직으로 올라갔다가 떨어지는 스테이트
		USI_DAMAGE_AIR_UP,					/// UP_TYPE의 공격등에 의하여 수직으로 올라가는 스테이트
		USI_DAMAGE_AIR_FLY_FRONT,			/// 전방에서 FLY_TYPE 공격을 받았을 때의 스테이트
		USI_DAMAGE_AIR_FLY_BACK,			/// 전방에서 FLY_TYPE 공격을 받았을 때의 스테이트
		USI_DAMAGE_REVENGE,					/// 공격했다가 엘소드의 반격을 받았을 때 변경되는 스테이트
		USI_DAMAGE_GRAPPLED_FRONT,			/// 사내에만 적용 중
		USI_RIDING_HYPER_MODE,				/// 탈 것 탑승 중 각성

		USI_PEPPER_RUN_READY,				/// 청양고추 아이템 사용 스테이트
		USI_PEPPER_RUN,						/// 
		USI_PEPPER_RUN_END,					/// 
		USI_PEPPER_RUN_JUMP_UP,				/// 
		USI_PEPPER_RUN_JUMP_DOWN,			/// 

		USI_SPECIAL_ATTACK_1,				/// A슬롯의 첫번째 스킬슬롯(현재는A키)에 해당하는 스테이트
		USI_SPECIAL_ATTACK_HYPER_1,			/// A슬롯의 첫번째 스킬슬롯(현재는A키)에 해당하는 각성 스테이트
		USI_SPECIAL_ATTACK_2,				/// A슬롯의 두번째 스킬슬롯(현재는S키)에 해당하는 스테이트
		USI_SPECIAL_ATTACK_HYPER_2,			/// A슬롯의 두번째 스킬슬롯(현재는S키)에 해당하는 각성 스테이트
		USI_SPECIAL_ATTACK_3,				/// A슬롯의 세번째 스킬슬롯(현재는D키)에 해당하는 스테이트
		USI_SPECIAL_ATTACK_HYPER_3,			/// A슬롯의 세번째 스킬슬롯(현재는D키)에 해당하는 각성 스테이트
		USI_SPECIAL_ATTACK_4,				/// A슬롯의 네번째 스킬슬롯(현재는C키)에 해당하는 스테이트
		USI_SPECIAL_ATTACK_HYPER_4,			/// A슬롯의 네번째 스킬슬롯(현재는C키)에 해당하는 각성 스테이트

		USI_SPECIAL_ATTACK_SLOTB_1,			/// B슬롯의 첫번째 스킬슬롯(현재는A키)에 해당하는 스테이트
		USI_SPECIAL_ATTACK_SLOTB_HYPER_1,	/// B슬롯의 첫번째 스킬슬롯(현재는A키)에 해당하는 각성 스테이트
		USI_SPECIAL_ATTACK_SLOTB_2,			/// B슬롯의 두번째 스킬슬롯(현재는S키)에 해당하는 스테이트
		USI_SPECIAL_ATTACK_SLOTB_HYPER_2,	/// B슬롯의 두번째 스킬슬롯(현재는S키)에 해당하는 각성 스테이트
		USI_SPECIAL_ATTACK_SLOTB_3,			/// B슬롯의 세번째 스킬슬롯(현재는D키)에 해당하는 스테이트
		USI_SPECIAL_ATTACK_SLOTB_HYPER_3,	/// B슬롯의 세번째 스킬슬롯(현재는D키)에 해당하는 각성 스테이트
		USI_SPECIAL_ATTACK_SLOTB_4,			/// B슬롯의 네번째 스킬슬롯(현재는C키)에 해당하는 스테이트
		USI_SPECIAL_ATTACK_SLOTB_HYPER_4,	/// B슬롯의 네번째 스킬슬롯(현재는C키)에 해당하는 각성 스테이트

		//#ifdef SPECIAL_USE_ITEM
		USI_THROW_ITEM,						/// 던전사용 아이템, 던지기
		USI_INSTALL_ITEM,					/// 던전사용 아이템, 설치
		//#endif SPECIAL_USE_ITEM

#ifdef RIDING_SYSTEM			// 적용날짜: 2013-04-02
		USI_RIDING_START,				/// 타고 있는 상태에서 던전 시작
		USI_RIDING_ON,					/// 타기
		USI_RIDING_OFF,					/// 내리기
		USI_RIDING_WAIT_HABIT,			/// 습관
		USI_RIDING_WAIT,				/// 타고 있음
		USI_RIDING_WALK,				/// 걷기
		USI_RIDING_JUMP_UP,				/// 점프 업
		USI_RIDING_JUMP_DOWN,			/// 점프 다운
		USI_RIDING_JUMP_LANDING,		/// 점프 착지
		USI_RIDING_DASH,				/// 대시
		USI_RIDING_DASH_END,			/// 대시 멈춤
		USI_RIDING_DASH_JUMP,			/// 대시 점프
		USI_RIDING_DASH_JUMP_LANDING,	/// 대쉬 점프 착지
		USI_RIDING_DAMAGE_FRONT,		/// 피격 앞
		USI_RIDING_DAMAGE_BACK,			/// 피격 뒤
		USI_RIDING_DIE,					/// 타고 죽는 상태

		USI_RIDING_ATTACK_Z,			/// Z 공격
		USI_RIDING_JUMP_ATTACK_Z,		/// 점프 Z 공격
		USI_RIDING_ATTACK_X,			/// X 공격
		USI_RIDING_SPECIAL_ATTACK,		/// 특수 공격
		USI_RIDING_SPECIAL_MOVE,		/// 특수 이동

//#ifdef MODIFY_RIDING_PET_AWAKE
		//스테이트 255개 초과 관련 문제로 임시 제거
		//USI_RIDING_HYPER_MODE,			/// 각성
//#endif // MODIFY_RIDING_PET_AWAKE

#endif //RIDING_SYSTEM

		USI_END,
	};

#ifdef NEW_SKILL_TREE
		enum SPECIAL_ATTACK_KEY_PRESSED
		{
			SAKP_NONE,
			SAKP_A,
			SAKP_S,
			SAKP_D,
			SAKP_C,
			SAKP_Q,
			SAKP_W,
			SAKP_E,
			SAKP_R,
		};
#endif NEW_SKILL_TREE

		// 모션, 상태변화 없이 바뀌는 것 sync 
		enum USER_ACTION_ID
		{
			UAI_NONE = 0,
			UAI_ENDURANCE,
#ifdef SKILL_30_TEST
			UAI_MANA_SHIELD,
			UAI_AHM_MEDITATION_START,
			UAI_AHM_MEDITATION_STOP,
#endif SKILL_30_TEST
			UAI_SIEGE_FIRE_ANGLE_UP,
			UAI_SIEGE_FIRE_ANGLE_DOWN,
			UAI_MEGA_ELECTRONBALL,
			UAI_MEGA_ELECTRONBALL_CHARGE_FAIL,

			UAI_ELEMENTAL_FRIENDSHIP_UP,

			//{{ kimhc // 2010.11.1 // 신스킬
#ifdef	NEW_SKILL_2010_11
			UAI_REDUCE_PAIN,				/// 로드나이트 - 고통 억제
			UAI_ENDURANCE_AND_REDUCE_PAIN,	/// 로드나이트 - 극기 와 고통억제가 같이 발동 되는 경우 (임시)
			UAI_CHARGING_BOOSTER,	// 엠프 - 충전 추진기
#endif	NEW_SKILL_2010_11
			//}} kimhc // 2010.11.1 // 신스킬
#ifdef EVE_ELECTRA
			UAI_THOUSANDS_OF_STARS,
			UAI_ELECTRA_PLANE_ANGLE_UP,
			UAI_ELECTRA_PLANE_ANGLE_DOWN,
#endif EVE_ELECTRA

#ifdef SERV_RENA_NIGHT_WATCHER
			UAI_START_OF_DELAYED_FIRING,	/// 지연의 신호탄 전체 폭발
#endif SERV_RENA_NIGHT_WATCHER
#ifdef SERV_EVE_BATTLE_SERAPH
			UAI_TASER_PILUM,
			UAI_TASER_PILUM_CHARGE_FAIL,
			UAI_CHANGE_SPECTRO_WITHOUT_MOTION,
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			UAI_AHM_SHINING_BODY,			/// 샤이닝 바디
			UAI_AVP_SHADOW_BODY,			/// 섀도우 바디
			UAI_ADW_ENERGETIC_BODY,			/// 에너제틱 바디

			UAI_KUGELBLITZ,					/// 쿠글블릿츠
			UAI_KUGELBLITZ_CHARGE_FAIL,		/// 쿠글블릿츠 충전 실패
#endif //UPGRADE_SKILL_SYSTEM_2013

			UAI_ENDURANCE_POWER,			/// 극기 - 강(强)

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
			UAI_REFLECTION,					/// 반탄공
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef FINALITY_SKILL_SYSTEM //김창한
			UAI_EEP_LINK_OVERCHARGE_ILLUSION,	// 엠프레스 궁극기 - 링크 오버차지 썬더볼트
#endif //FINALITY_SKILL_SYSTEM
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
			UAI_RELEASE_FORMATION_MODE,		/// 애드 구성 모드 해제
			UAI_ACTIVE_STASIS_FIELD,		/// 애드 스테이시스 필드 발동
#endif //SERV_9TH_NEW_CHARACTER

		};

		// kimhc // 2010.11.3 모든 유저가 공통으로 쓰는 메이저 파티클
		enum COMMON_MAJOR_PARTICLE_INSTANCE_ID
		{
			COMMON_MAJOR_PII_REVENGE_IMPACT_SLASH_CW,		/// 반격기에 당하기 직전에 생기는 파티클(시계방향)
			COMMON_MAJOR_PII_REVENGE_IMPACT_SLASH_CCW,		/// 반격기에 당하기 직전에 생기는 파티클(반시계방향)
			COMMON_MAJOR_PII_REVENGE_IMPACT,				/// 	
			COMMON_MAJOR_PII_REVENGE_IMPACT_TICK,			/// 
			COMMON_MAJOR_PII_MAGIC_FAIL,					/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT,					/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE,			/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH,			/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_RED,				/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_RING_RED,		/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE_RED,		/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH_RED,		/// 

			COMMON_MAJOR_PII_END,
		};

		// kimhc // 2010.11.3 모든 유저가 공통으로 쓰는 마이너 파티클
		enum COMMON_MINOR_PARTICLE_INSTANCE_ID
		{
			COMMON_MINOR_PII_STEP_SMOKE,
			COMMON_MINOR_PII_STEP_WATER,
			COMMON_MINOR_PII_HYPER_MODE_CHANGE_LINE,
			COMMON_MINOR_PII_HYPER_MODE_CHANGE_CENTER,
			COMMON_MINOR_PII_GROUND_SHOCK_WAVE,
			COMMON_MINOR_PII_WHITE_SHOCK_WAVE,
			COMMON_MINOR_PII_IMPACT_TICK,
			COMMON_MINOR_PII_STR_TOKANG,
			COMMON_MINOR_PII_AIR_DOWN_TICK,
			COMMON_MINOR_PII_DOWN_SMOKE,
	
			COMMON_MINOR_PII_END,
		};

#ifdef ADD_STATE_EXTRA_ATTACK
		enum STATE_EXTRA_ATTACK
		{
			SEA_NONE					= 0,
			SEA_BIGBANG_STREAM_BOMB,
			SEA_END,
		};
#endif
#ifdef ELSWORD_WAY_OF_SWORD
		enum WAY_OF_SWORD_STATE
		{
			WSS_DESTRUCTION				= -2,
			WSS_TOWARD_DESTRUCTION		= -1,
			WSS_CENTER					= 0,
			WSS_TOWARD_VIGOR			= 1,
			WSS_VIGOR					= 2,
		};
#endif ELSWORD_WAY_OF_SWORD
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
		enum UNIT_CLASS_LINE //유닛 클래스 몇번째 라인인지.
		{
			UCL_FIRST					= 0, // 첫번째 라인 EX) 엘소드인 경우 소드 나이트 - 로드 나이트 라인
			UCL_SECOND,						 // 두번째 라인 EX) 엘소드인 경우 매직 나이트 - 룬 슬레이어 라인
			UCL_THIRD,						 // 세번째 라인 EX) 엘소드인 경우 시스 나이트 - 인피니티 소드 라인
		};
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
		struct  InitData
		{
			InitDeviceData          m_device;
			void                AppendToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pX2Unit_ ) const;
		};//struct  InitData

		//{{ kimhc // 2010.11.2 // 쿨타임, 남은시간 등을 필요로하는 스킬등에 사용하기 위한 클래스
#ifdef	NEW_SKILL_2010_11
				
		/** @class : CSkillDataBase
			@brief : 유저 및 각 캐릭터 클래스들 에서 사용되는 스킬 데이터를 통합하여 관리 하기 위한 클래스
					 2010/11/09일 현재는 기본적인 쿨타임의 기능만 가지고 있지만, 이펙트, 데이타 등도 관리할 수 있도록
					 확장 or 변경할 예정
			@date  : 2010/11/09
		*/
        class   CSkillDataBase;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        typedef boost::intrusive_ptr<CSkillDataBase> CSkillDataBasePtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		typedef boost::shared_ptr<CSkillDataBase> CSkillDataBasePtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		class CSkillDataBase
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		{
			// kimhc // 2010-11-10 // 생각해보면 쿨타임이 필요 없을듯.. 신캐 이후 정리 요망!
		private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			CX2SkillTree::SKILL_ID m_eSkill_ID;		/// 유저의 스킬 ID
			float		m_fCoolTime;				/// 쿨타임이 있는 스킬의 경우 다시 사용하기 까지의 제한시간
			float		m_fTimerBasedOnCoolTime;	/// 쿨타임에서 부터 0초 까지로 변화되는, 현재 진행중인 타이머
			float		m_fEffectiveTime;			/// 스킬 지속시간

		protected:
			CSkillDataBase() : m_eSkill_ID( CX2SkillTree::SI_NONE ), m_fCoolTime( 0.f ), 
				m_fTimerBasedOnCoolTime( 0.f ), m_fEffectiveTime( 0.f ) 
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            {}
			CSkillDataBase( CX2SkillTree::SKILL_ID eSkill_ID_, float fCoolTime_ ) : m_eSkill_ID( eSkill_ID_ ), 
				m_fCoolTime( fCoolTime_ ), m_fTimerBasedOnCoolTime( 0.f ), m_fEffectiveTime( 0.f )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            {}

		public:
			
			static CSkillDataBasePtr CSkillCreateSkillDataBasePtr() { return CSkillDataBasePtr( new CSkillDataBase() ); }
			static CSkillDataBasePtr CSkillCreateSkillDataBasePtr(  CX2SkillTree::SKILL_ID eSkill_ID_, float fCoolTime_ ) { return CSkillDataBasePtr( new CSkillDataBase( eSkill_ID_, fCoolTime_ ) ); }

			CX2SkillTree::SKILL_ID GetSkill_ID() const { return m_eSkill_ID; }
			void SetSkill_ID( CX2SkillTree::SKILL_ID eSkill_ID_ ) { m_eSkill_ID = eSkill_ID_; }

			float GetCoolTime() const { return m_fCoolTime; }
			void SetCoolTime( float fCoolTime_ ) {	m_fCoolTime = fCoolTime_; }

			float GetTimerBasedOnCoolTime() const { return m_fTimerBasedOnCoolTime; };
			void SetTimerBasedOnCoolTime( float fTimerBasedOnCoolTime_ ) { m_fTimerBasedOnCoolTime = fTimerBasedOnCoolTime_; }

			float GetEffectiveTime() const { return m_fEffectiveTime; }
			void SetEffectiveTime(float fEffectiveTime_ ) { m_fEffectiveTime = fEffectiveTime_; }

			/** @function : IsEndCoolTime() const
			@brief : 쿨타임이 끝났는지를 확인하는 함수. (m_fTimerBaseOnCoolTime이 0.f이 됬는지를 확인)
			@param : void
			@return : 쿨타임이 끝났으면 true, 아직 쿨타임 중이면 false
			*/
			bool IsEndCoolTime() const { return ( 0.f >= m_fTimerBasedOnCoolTime ? true : false ); }
			bool IsEndEffectiveTime() const { return ( 0.f >= m_fEffectiveTime ? true : false ); }

			void OnFrameMove( float fElapsedTime_ )
			{
				if ( false == IsEndCoolTime() )
				{
					SetTimerBasedOnCoolTime( GetTimerBasedOnCoolTime() - fElapsedTime_ );
				}

				if ( false == IsEndEffectiveTime() )
				{
					SetEffectiveTime( GetEffectiveTime() - fElapsedTime_ );
				}
			}

			void ResetTimerBasedOnCoolTime() { SetTimerBasedOnCoolTime( GetCoolTime() ); }

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            void    AddRef()    {   ++m_uRefCount; }
            void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		};

#endif	NEW_SKILL_2010_11
		//}} kimhc // 2010.11.2 // 쿨타임, 남은시간 등을 필요로하는 스킬등에 사용하기 위한 클래스

//{{ kimhc // 2010.4.7 // 비밀던전 작업(셋트아이템효과)
#ifdef SERV_SECRET_HELL
		struct Bone_Shield	// 셋트아이템 효과 중 본쉴드를 위한 구조체
		{
			float	m_fIntervalTime;				/// 쿨타임
			float	m_fReducePercent;				/// 피해 감소양
			float	m_fTimeLeft;					/// 남은 시간
			BYTE	m_byCountLeft;					/// 남은 횟수
			CX2EffectSet::Handle m_hEffectSet;		/// 이펙트 셋
			wstring m_wstrEffectName;				/// 타격 당했을 때 나오는 이펙트 셋 명, 위의 이펙트 셋과 다른 이펙트

			Bone_Shield() 
			{
				Init();
			}

			~Bone_Shield()
			{
				DestroyParticle();
			}

			void Init()
			{
				m_fIntervalTime		= 0.0f;
				m_fReducePercent	= 0.0f;
				m_fTimeLeft			= 0.0f;
				m_byCountLeft		= 0;
				m_hEffectSet		= INVALID_EFFECTSET_HANDLE;
				m_wstrEffectName.clear();
			}

			void DestroyParticle();
		};
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.7 // 비밀던전 작업(셋트아이템효과)

		//{{ kimhc // 2009-11-18 // 길드스킬 역전의 기회에서 쓰일 구조체
		// 횟수와 시간 고려함
#ifdef	GUILD_SKILL_PART_2
		struct GuildSkillInfiniteStrength
		{
			float		m_byCountLeft;				// 남은 횟수
			float		m_fRecoveryHpOnce;			// 한번에 회복될 양
			double		m_fElapsedTime;				// 이전 이펙트 후 경과된 시간
			const float	m_fTimeGap;					// 전의 효과와의 시간 차
			const float m_fRecoveryPerOneDamage;	// 데미지당 회복될 % 
			CKTDGParticleSystem::CParticleEventSequenceHandle m_hSeqEffect;
			
			GuildSkillInfiniteStrength() : m_fTimeGap( 0.8f ), m_fRecoveryPerOneDamage( 0.4f )
			{
				Init();
			}

			void Init()
			{
				m_byCountLeft		= 0;
				m_fRecoveryHpOnce	= 0.0f;
				m_fElapsedTime		= 0.0f;
				m_hSeqEffect		= INVALID_PARTICLE_SEQUENCE_HANDLE;
			}

			bool IsElaspedTimeOverGap( float fElapsedTime )
			{
				m_fElapsedTime += fElapsedTime;

				if ( m_fElapsedTime > m_fTimeGap )
				{
					m_byCountLeft--;
					m_fElapsedTime = 0.0f;
					return true;
				}

				return false;
			}
		};

		struct GuildSkillData 
		{
			bool							m_bLearnChanceToReverse;
			GuildSkillInfiniteStrength		m_InfiniteStrength;

			GuildSkillData()
			{
				Init();
			}

			void Init()
			{
				m_bLearnChanceToReverse = false;
				m_InfiniteStrength.Init();
			}
		};

#endif	GUILD_SKILL_PART_2
		//}} kimhc // 2009-11-18 // 길드스킬 역전의 기회에서 쓰일 구조체
		
		struct SkillCutIn
		{
			std::wstring fileName;
			D3DXVECTOR2 vSize;
			D3DXVECTOR3 vPosition;

			SkillCutIn( const std::wstring& fileName_, const D3DXVECTOR2& vSize_, const D3DXVECTOR3& vPosition_ )
			: fileName( fileName_ )
			, vSize( vSize_ )
			, vPosition( vPosition_ )
			{
			}


			SkillCutIn( const std::wstring& fileName_)
				: fileName( fileName_ )
				, vSize( D3DXVECTOR2(512.f,512.f) )
				, vPosition( D3DXVECTOR2(256.f,512.f) )
			{
			}
		};

		struct SkillCutInSet
		{
			SkillCutIn cutIn[3];
		};


		struct SyncData
		{
			DWORD					dwFrameMoveCount;
			KProtectedType<UCHAR>	nowState;
			KProtectedType<char>	nowSubState;
			KProtectedType<UCHAR>	nowAction;

			bool					bDirectChange;
			char					stateChangeNum;

			D3DXVECTOR3				position;
			unsigned char 			lastTouchLineIndex;

			bool					bIsRight;
			bool					bFrameStop;

			KProtectedType<float>	fNowHP;
			KProtectedType<float>	fNowMP;

			KProtectedType<int>		m_HyperModeCount;

//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			KProtectedType<int>		m_CannonBallCount;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG

#ifdef NEW_RANDOM_TABLE_TEST
			USHORT m_usRandomTableIndex;
#else NEW_RANDOM_TABLE_TEST
//{{AFX
			char					m_RandSeed;
			char					m_RandSeed2;
//}}AFX
#endif NEW_RANDOM_TABLE_TEST

			UCHAR				ucHitCount;			/// 타격횟수
			UCHAR				ucHittedCount;		/// 피격횟수
			UCHAR				ucNumOfDeBuff;		/// 디버프개수
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
            DWORD                   m_dwRelativePos;
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC

			SyncData()
			{
				Init();
			}
			void Init()
			{
				dwFrameMoveCount		= 0;
				nowState				= 0;
				nowSubState				= -1;
				nowAction				= UAI_NONE;
				bDirectChange			= false;
				stateChangeNum			= 0;

				INIT_VECTOR3( position,	0,0,0 );
				lastTouchLineIndex		= 0;

				bIsRight				= true;

				fNowHP					= 0.0f;
				fNowMP					= 0.0f;

				bFrameStop				= false;

//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
				m_CannonBallCount		= 0;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG
				m_HyperModeCount		= 0;
				
#ifdef NEW_RANDOM_TABLE_TEST
				m_usRandomTableIndex	= (USHORT) ( rand() % CKTDXRandomNumbers::RANDOM_NUMBER_COUNT );
#else NEW_RANDOM_TABLE_TEST
//{{AFX
				m_RandSeed				= (char) (rand() % 100);
				m_RandSeed2				= (char) (rand() % 100);
//}}AFX
#endif NEW_RANDOM_TABLE_TEST		

				ucHitCount				= 0;			/// 타격횟수
				ucHittedCount			= 0;		/// 피격횟수
				ucNumOfDeBuff			= 0;		/// 디버프개수
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
                m_dwRelativePos         = 0;
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
			}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

			void SetData( const KDYNAMIC_UNIT_USER_SYNC& SyncPacket, DWORD dwRealFrameMoveCount, float fMaxHP, float fMaxMP );

//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
            void _DecodeUserSyncPos( D3DXVECTOR3& vPosition, USHORT usPosX, USHORT usPosZ, unsigned char ucLastTouchLineIndex );
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//			void _DecodeUserSyncPos( D3DXVECTOR3& vPosition, USHORT usPosX, USHORT usPosY, USHORT usPosZ, unsigned char ucLastTouchLineIndex );
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef OPTIMIZED_P2P
//            void SetData( KXPT_UNIT_USER_SYNC& SyncPacket, USHORT usHP, USHORT usMP );
//#else
//            void SetData( KXPT_UNIT_USER_SYNC* pSyncPacket );
//#endif
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

			void Verify();
#ifdef ELSWORD_WAY_OF_SWORD
			int		IsActionActive( UINT uiActionIndex_ );
#endif ELSWORD_WAY_OF_SWORD

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK        
            bool    operator < ( const SyncData& rhs_ ) const   { return dwFrameMoveCount < rhs_.dwFrameMoveCount; }
            bool    operator < ( DWORD dwrhs_ ) const           { return dwFrameMoveCount < dwrhs_; }
            friend bool    operator < ( DWORD dwlhs_, const SyncData& rhs_ ) { return dwlhs_ < rhs_.dwFrameMoveCount; }
//#endif     SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK      
                 };          
            
            
        
		struct FrameData
		{
			StateParam		stateParam;
			UnitCondition	unitCondition;
			SyncData		syncData;

			FrameData& operator=( const FrameData& data )
			{
				stateParam		= data.stateParam;
				unitCondition	= data.unitCondition;
				syncData		= data.syncData;

				return *this;
			}
			void Init()
			{
				stateParam.Init();
				unitCondition.Init();
				syncData.Init();
			}
		};

		
		struct UserUnitStateData
		{
//{{ robobeg : 2008-10-28
			UCHAR				stateID;
			//wstring				stateTable;
//}} robobeg : 2008-10-28
			char				m_SPLevel;
#ifdef NEW_SKILL_TREE
			float				m_fPowerRate;
#endif NEW_SKILL_TREE
			CX2SkillTree::SKILL_ID	m_eSkillID;			// 필살기 상태인 경우에 현재 사용한 필살기가 어떤 것인지

			bool    m_bHyperState;	// 각성중인 상태인지

			GUStateCBFunc		StateInit;
			GUStateCBFunc		StateStartFuture;
			GUStateCBFunc		StateStart;
			GUStateCBFunc		OnFrameMoveFuture;
			GUStateCBFunc		OnFrameMove;
			GUStateCBFunc		OnCameraMove;
			GUStateCBFunc		OnEventProcess;
//{{ robobeg : 2008-10-28
			//GUStateCBFunc		OnCustomRender;
//}} robobeg : 2008-10-28
			GUStateCBFunc		StateEndFuture;
			GUStateCBFunc		StateEnd;

			CX2ComboAndSkillChainInfoPtr	m_ptrComboAndSkillChainInfo;

			UserUnitStateData()
			{
				Init();
			}

			virtual void Init()
			{
				stateID				= GUSI_NONE;
//{{ robobeg : 2008-10-28
				//stateTable			= L"";
//}} robobeg : 2008-10-28
				m_SPLevel			= 0;					// 필살기의 SP level은 1부터 시작이고, 그외 통상콤보의 SP level은 모두 0이다
#ifdef NEW_SKILL_TREE
				m_fPowerRate		= 1.f;
#endif NEW_SKILL_TREE
				m_eSkillID			= CX2SkillTree::SI_NONE;

				StateInit			= GUStateCBFunc();
				StateStartFuture	= GUStateCBFunc();
				StateStart			= GUStateCBFunc();
				OnFrameMove			= GUStateCBFunc();
				OnFrameMoveFuture	= GUStateCBFunc();
				OnCameraMove		= GUStateCBFunc();
				OnEventProcess		= GUStateCBFunc();
//{{ robobeg : 2008-10-28
				//OnCustomRender		= GUStateCBFunc();
//}} robobeg : 2008-10-28
				StateEndFuture		= GUStateCBFunc();
				StateEnd			= GUStateCBFunc();

				m_bHyperState = false;
				m_ptrComboAndSkillChainInfo.reset();
			}


			UserUnitStateData( const UserUnitStateData& t ) { *this = t; }    // copy constructor
			UserUnitStateData& operator=( const UserUnitStateData& rhs )
			{
				stateID				= rhs.stateID;
//{{ robobeg : 2008-10-28
				//stateTable			= data.stateTable;
//}} robobeg : 2008-10-28
				m_SPLevel			= rhs.m_SPLevel;
#ifdef NEW_SKILL_TREE
				m_fPowerRate		= rhs.m_fPowerRate;
#endif NEW_SKILL_TREE
				m_eSkillID			= rhs.m_eSkillID;

				StateInit			= rhs.StateInit;
				StateStartFuture	= rhs.StateStartFuture;
				StateStart			= rhs.StateStart;
				OnFrameMove			= rhs.OnFrameMove;
				OnFrameMoveFuture	= rhs.OnFrameMoveFuture;
				OnCameraMove		= rhs.OnCameraMove;
				OnEventProcess		= rhs.OnEventProcess;
//{{ robobeg : 2008-10-28
				//OnCustomRender		= data.OnCustomRender;
//}} robobeg : 2008-10-28
				StateEndFuture		= rhs.StateEndFuture;
				StateEnd			= rhs.StateEnd;

				m_bHyperState		= rhs.m_bHyperState;
				m_ptrComboAndSkillChainInfo = rhs.m_ptrComboAndSkillChainInfo;

				return *this;
			}
		};
		typedef map<UCHAR, UserUnitStateData> UserUnitStateDataMap;


//#ifndef NOT_USE_DICE_ROLL
//		struct DiceRoll
//		{
//			DiceRoll( CX2GUUser* pGUUser );
//			~DiceRoll();
//			void OnFrameMove( double fTime, float fElapsedTime );
//
//			void PushDiceRoll( bool bWinningDice )
//			{
//				m_vecDiceRoll.push_back( bWinningDice );
//			}
//
//			D3DXVECTOR3 GetParticlePos();
//			D3DXVECTOR3	GetZVector( D3DXVECTOR3& vDirVector );
//			
//		public:
//			CX2GUUser*		m_pGUUser;
//			vector<bool>	m_vecDiceRoll;	// true이면 6이 나오는 주사위 굴림, false 이면 6을 제외한 나머지 숫자가 나오는 주사위 굴림
//
//			int		m_iNowRollingDiceCount;
//			int		m_iNowDiceSeqIndex;
//
//			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqRollingDice[6];			// 구르는 주사위 파티클
//			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqDice[6];					// 멈춘 주사위 파티클
//
//			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqDiceBalloon;
//			CKTDGParticleSystem::CParticle*					m_pParticle;
//		};
//#endif //NOT_USE_DICE_ROLL

		//{{ dmlee 2008.04.11 - 캐시아이템 관련
		struct CashItemAbility
		{
			bool		m_bShowOppnentMP;
			float		m_fUpMPAtAttackOrDamage;

			CashItemAbility()
			{
				m_bShowOppnentMP		= false;
				m_fUpMPAtAttackOrDamage = 0.f;
			};
		};
		//}} dmlee 2008.04.11 - 캐시아이템 관련
		



		struct CommonRandomState
		{
			std::string	    m_StateName;
			int				m_iPercent;
		};


#ifdef SERV_PET_SYSTEM
		struct PetCheer
		{
			float m_fTime;

			float m_fChangeHp;			

			float m_fSpeed;
			float m_fMoveSpeed;
			float m_fCriticalRate;
			float m_fChargeMp;
			float m_fChargeHp;

			PetCheer()
			{
				Init();
			}

			void Init()
			{
				m_fTime = 0.f;
				m_fChangeHp = 0.f;
				m_fSpeed = 0.f;
				m_fMoveSpeed = 0.f;
				m_fCriticalRate = 0.f;
				m_fChargeMp = 0.f;
				m_fChargeHp = 0.f;
			}
		}; 
#endif

#ifdef ELSWORD_SHEATH_KNIGHT
		struct EventProcessTime
		{
			float keyInputStart;
			float keyInputEnd;
			float stateChange;
			EventProcessTime()
			{
				keyInputStart	= 0.f;
				keyInputEnd		= 9999.f;
				stateChange		= 9999.f;
			}
		};
#endif ELSWORD_SHEATH_KNIGHT

#ifdef ADD_STATE_EXTRA_ATTACK
		struct StateExtraAttack
		{
			STATE_EXTRA_ATTACK	eId;
			float				m_fStartTime;
			StateExtraAttack()
			{
				eId = SEA_NONE;
				m_fStartTime = 9999.f;
			}
		};
#endif

		struct SummonNpcSocket
		{
			CX2UnitManager::NPC_UNIT_ID m_iNpcId;
			float						m_fSummonRate;
			float						m_fSummonNpcCoolTime;
		};

#ifdef SUMMON_MONSTER_CARD_SYSTEM		// mauntain : 김태환 [2012.06.26] 몬스터 카드 소환 기능 - 해당 유저의 몬스터 소환 정보
		struct SummonMonsterCardData
		{
			int	  m_iSummonMonsterUID;		//소환한 몬스터 UID
			float m_fSummonMonsterTime;		//소환한 몬스터의 생성 유지 시간
#ifdef EVENT_MONSTER_CARD_SUMMON_ENEMY
			std::vector<int> m_vecSummonMonsterUID;  //4마리 소환시 몬스터 UID를 담을 벡터
#endif //EVENT_MONSTER_CARD_SUMMON_ENEMY
			SummonMonsterCardData()
			{
				m_iSummonMonsterUID  = -1;
				m_fSummonMonsterTime = 0.f;
			}

			void init()
			{
				m_iSummonMonsterUID  = -1;
				m_fSummonMonsterTime = 0.f;
			}

			int GetSummonMonsterUID() const						
			{ 
				return m_iSummonMonsterUID;
			}
			void SetSummonMonsterUID( int iSummonMonsterUID ) 
			{ 
				m_iSummonMonsterUID = iSummonMonsterUID;
			}
#ifdef EVENT_MONSTER_CARD_SUMMON_ENEMY
			void SetSummonMonsterUIDInVec( int iSummonMonsterUID ) 
			{ 
				m_vecSummonMonsterUID.push_back( iSummonMonsterUID );
			}
			std::vector<int> GetSummonMonsterUIDVec()							
			{ 
				return m_vecSummonMonsterUID;
			}
			void ClearSummonMonsterUIDInVec() 
			{ 
				m_vecSummonMonsterUID.clear();
			}
#endif //EVENT_MONSTER_CARD_SUMMON_ENEMY
			float GetSummonMonsterTime() const						
			{ 
				return m_fSummonMonsterTime;
			}
			void SetSummonMonsterTime( float fSummonMonsterTime ) 
			{ 
				m_fSummonMonsterTime = fSummonMonsterTime;
			}
		};
#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		struct GameCameraOffset
		{
			bool	bEnable; 
			float	fCameraDistance;
			float	fHeight;
			float	fAngleDegree;
			float	fEyeDistance;
			float	fLookatDistance;
			float	fTrackingTime;
			float	fTrackingAtTime;
			GameCameraOffset()
			{
				bEnable	= false;
				fCameraDistance		= 0.f;
				fHeight				= 0.f;
				fAngleDegree		= 0.f;
				fEyeDistance		= 0.f;
				fLookatDistance		= 0.f;
				fTrackingTime		= 0.f;
				fTrackingAtTime		= 0.f;
			}
		};
#endif SERV_CHUNG_TACTICAL_TROOPER

		//{{ Public Variable End

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환

	/// 현빈이가 제작한 스킬 키 입력 제어 기능 머지
	/** @struct : CurrentSkillKeyManager
	@brief : 같은 스킬키를 여러번 입력하여 발동하는 스킬에 대한 입력 키 관리 구조체
	
	사용 시 필수적으로 적용해야 하는 사항
	1. StartFuture에서 InitCurrentSkillKeyManager() 호출
	2. EventProcess에서 CheckSkillSlotSwap() 호출
	3. 키입력 여부 체크는 IsPressCurrentSkillKey()로 체크
*/
	struct CurrentSkillKeyManager
	{
		int iSkillSlotIndex;
		bool m_bIsSkillSLotSwap;

		CurrentSkillKeyManager():
		iSkillSlotIndex(-1),
			m_bIsSkillSLotSwap(false)
		{}

		void CheckSkillSlotSwap();
		void InitCurrentSkillKeyManager( int iSkillSlotIndex_ );
		bool IsPressedCurrentSkillKey( const CX2GUUser::InputData& InputData_ );
		bool IsUnpressedCurrentSkillKey( const CX2GUUser::InputData& InputData_ );
		void Init();
	};

#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef ADD_RENA_SYSTEM //김창한
	struct AllDamageRelateSkillData	
	{
		Byte m_byteSkillIndex;	//스킬 id가 같을 수도 있으므로 구별을 위한 index값
		CX2DamageManager::DamageRelateSkillData m_NowData;	// 현재 동작중인 관련 data 값
		std::vector<CX2DamageManager::DamageRelateSkillData> m_vecSaveData;	// 공격이 명중한 후 damagedata에 저장되어 있는 값과 비교를 위한 저장값.

		AllDamageRelateSkillData():m_byteSkillIndex(0){}

		void Init()
		{
			m_byteSkillIndex = 0;
			m_NowData.Init();
			m_vecSaveData.clear();
		}
	};

	//현재 m_NowData를 가져오는 함수
	const CX2DamageManager::DamageRelateSkillData& GetNowDamageRelateSkillData(){ return m_AllDamageRelateSkillData.m_NowData; }

	//현재 접근 가능한 m_NowData를 가져오는 함수
	CX2DamageManager::DamageRelateSkillData& GetAccessNowDamageRelateSkillData(){ return m_AllDamageRelateSkillData.m_NowData; }

	//현재 접근 가능한 m_AllDamageRelateSkillData를 가져오는 함수
	AllDamageRelateSkillData& GetAccessAllDamageRelateSkillData(){ return m_AllDamageRelateSkillData; }

	//현재 관련 데이터를 설정하는 함수. 설정하며 m_vecSaveData 저장도 같이 함.
	void SetNowDamageRelateSkillData( Byte byteRelateData_ );

	//인자로 들어온 값이 m_vecSaveData에 저장된 값 중에 일치하는 것이 있는지 체크하는 함수 
	virtual bool CheckDamageRelateSkillData( const CX2DamageManager::DamageRelateSkillData sData_ ){ return false; }

	//m_vecSaveData 값 중에 인자로 들어온 값과 일치하는 것을 삭제하는 함수
	virtual void DeleteDamageRelateSkillData( const CX2DamageManager::DamageRelateSkillData sData_ ){};

	//DamageReact를 실행하기 전에 해당 DamageData를 수정할 수 있는 함수
	virtual void AdjustDamageDataBeforeDamageReact( CX2DamageManager::DamageData* pDamageData ){};
#endif //ADD_RENA_SYSTEM

#ifdef NEXON_QA_CHEAT_REQ
	bool	m_bIsInvincibleAndNoMpConsume;
#endif //NEXON_QA_CHEAT_REQ


	protected:
		//{{ Protected Function Start
		CX2GUUser( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			int frameBufferNum, CX2Unit* pUnit );
		virtual ~CX2GUUser(void);
		//{{ Protected End Start

	public:
		//{{ Public Function Start
		static void                 InitMotion( InitData& OutInit_, KLuaManager& luaManager_ );
		static void					InitInit( InitData& OutInit_, KLuaManager& luaManager_);

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void					InitComponent( InitData& OutInit_, KLuaManager& luaManager_);
		static void					AppendMotionToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pX2Unit_ );
		static void					AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pX2Unit_ );
		static void					AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pX2Unit_ );
		static void					AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pX2Unit_ );

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 무기 강화 이펙트 선로딩 함수
		static void					AppendUpgradeWeaponParticleToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit::UNIT_TYPE eUnitType_, int iUpgradeLevel_ );
#endif ADD_UPGRADE_WEAPON_PARTICLE

#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		virtual void				Init( bool bUseTeamPos = true, int startPosIndex = -1 );
		virtual void				ReInit( bool bUseTeamPos = true, int startPosIndex = -1 );

		void						InitByGameType();

		virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT				OnCameraMove();

//{{ robobeg : 2008-10-28
        virtual RENDER_HINT			OnFrameRender_Prepare();
		virtual void                OnFrameRender_Draw();
//}} robobeg : 2008-10-28


		virtual void				PreRenderForce();

		virtual HRESULT				OnResetDevice();
		virtual HRESULT				OnLostDevice();

		//virtual void				StateChange( int state, bool bDirectChange = false );
		//virtual void				StateChange( int state, int subState, bool bDirectChange = false );
		virtual bool				StateChange( int state, bool bDirectChange = false );
		virtual bool				StateChange( int state, int subState, bool bDirectChange = false );
        int							GetStartStateID(){ return USI_START; }
		int							GetStartIntrudeStateID(){ return USI_START_INTRUDE; }
		int							GetWalkStateID(){ return USI_WALK; }
		int							GetDashStateID(){ return USI_DASH; }
		int							GetDashEndStateID(){ return USI_DASH_END; }
		int							GetChargeMpStateID() { return m_ChargeMpState; }
		int 						GetDamageGroggyStateID() const { return USI_DAMAGE_GROGGY; }
		int 						GetDamageAirDownStateID() const { return USI_DAMAGE_AIR_DOWN; }

		virtual int					GetComboZStateID() const { return USI_WAIT; }
		virtual int					GetComboXStateID() const { return USI_WAIT; }

#ifdef RIDING_SYSTEM
		// 오현빈 // 코드 정리를 위해 스테이트 얻는 함수 추가
		int			 GetWaitStateID() { if (true == GetRidingOn()) return USI_RIDING_WAIT; return m_CommonState.m_Wait; }
		USER_STATE_ID GetJumpDownStateID() { if( true == GetRidingOn()) return USI_RIDING_JUMP_DOWN; return USI_JUMP_DOWN; }
		USER_STATE_ID GetWinStateID() { if (true == GetRidingOn()) return USI_RIDING_WAIT_HABIT; return USI_WIN; }
		USER_STATE_ID GetLoseStateID() { if (true == GetRidingOn()) return USI_RIDING_WAIT; return USI_LOSE; }
#else
		int			  GetWaitStateID(){ return m_CommonState.m_Wait; }
		USER_STATE_ID GetJumpDownStateID() { return USI_JUMP_DOWN; }
		USER_STATE_ID GetWinStateID() { return USI_WIN; }
		USER_STATE_ID GetLoseStateID() { return USI_LOSE; }
#endif //RIDING_SYSTEM

//{{ kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
// #ifdef SERV_SECRET_HELL
// 		CKTDGXSkinAfterImage* GetAfterImage() const { return m_pAfterImage; }
// #endif SERV_SECRET_HELL
//}} kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)

#ifdef _CLASS_CUTIN_
		wstring						GetCutInFileName(bool IsRight);
#endif _CLASS_CUTIN_

		virtual float				GetPowerRate(); // moved by wonpok. 20091215.

#ifdef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	#ifdef ADD_MEMO_1ST_CLASS
		void                        LoadEffectSetFromScript( IN KLuaManager& luaManager, IN const bool IsEqippedMemo_ = false );
	#else //ADD_MEMO_1ST_CLASS
		void                        LoadEffectSetFromScript( IN KLuaManager& luaManager );
	#endif //ADD_MEMO_1ST_CLASS
        void                        CommonFrameMove_EffectSet( float fNowAnimationTime );

		//{{ kimhc // 2010.11.3 // 신스킬 작업
#ifdef	NEW_SKILL_2010_11
		// 각 스테이트에서 Immune되어야할 ExtraDamage를 스크립트에서 읽어옴
		void LoadImmunityAtThisStateFromScript( IN KLuaManager& luaManager_ );
#endif	NEW_SKILL_2010_11
		//}} kimhc // 2010.11.3 // 신스킬 작업


        const D3DXVECTOR2&          GetNowAfterImageTime() const { return m_v2NowAfterImageTime; }
        bool                        FindNowSlashTraceTimeWithWeaponIndex( int iWeaponIndex, OUT D3DXVECTOR3& vSlashTraceTime );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


//#ifdef PVP_BOSS_COMBAT_TEST
//		int							GetFrozenState() const { return m_FrozenState; }
//#endif PVP_BOSS_COMBAT_TEST


		virtual void				DamageReact( CX2DamageManager::DamageData* pDamageData );

		bool						IsSocketSuperArmor() const { return m_bSocketSuperArmor; }
		float						GetTimeSocketSuperArmor() const { return m_fSocketSuperArmor; }
		
		void DamageReact_ExtraDamage( CX2DamageManager::DamageData* pDamageData, bool bApplyExtraDamageOfAttacker );
		void Process_ExtraDamage( const CX2DamageManager::DamageData* pDamageData, const CX2DamageManager::ExtraDamageData &extraDamageData );
//{{ oasis907 : 김상윤 [2010.3.22] // 
		void LevelCorrectionExtraDamage( const CX2DamageManager::DamageData* pDamageData, CX2DamageManager::ExtraDamageData* pExtraDamageData);
//}}

		//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
		void SetShowTooniLandEventEffectSet( bool bShow_ );
#endif	SERV_TOONILAND_CHANNELING_EVENT
		//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

#ifdef ARA_CHANGE_CLASS_FIRST
		virtual void				DamageReactStateChange( CX2DamageManager::DamageData* pDamageData, int iRevengeAttack = -1, int iAvoidance = -1 );
#else
		virtual void				DamageReactStateChange( CX2DamageManager::DamageData* pDamageData, int iRevengeAttack = -1 );
#endif //ARA_CHANGE_CLASS_FIRST
		virtual void				DetonationStateChange( CX2DamageManager::DamageData* pDamageData );

		int							GetDamageCount(){ return m_nDamageCount; }
		void						SetDamageCount( int nCount ) { m_nDamageCount = nCount; }
		virtual void				AttackResult();
		virtual void				AttackResultStateChange( int damageRevenge );
		
		//{{ kimhc // 2010-11-22 // 각 캐릭터의 AttackResult를 통합함
		virtual void				AttackResultPlayEffect();// 각 캐릭터의 AttackResult의 기능을 이쪽으로 옮김

#ifdef UPGRADE_RAVEN
		virtual void				AttackResultByType( CX2DamageManager::DamageData &pDamageData );
#endif

		void						PlayRevengeReactTypeParticle( const D3DXVECTOR3& vImpactPoint_ );
		void						PlaySmallDamageReactTypeParticle( const D3DXVECTOR3& vImpactPoint_ );
		void						PlayEtcReactTypeParticle( const D3DXVECTOR3& vImpactPoint_ );
		//}} kimhc // 2010-11-22 // 각 캐릭터의 AttackResult를 통합함

		//{{ kimhc // 2011-05-25 // SetOption, TitleOption에만 적용 되는 option 모든 itemOption에 적용 되도록 수정
		float						CalculateHpUpPercentBySocketOption( const vector<D3DXVECTOR2>& vecHpUpPercentBySocketOption );
		float						CalculateMpUpBySocketOption( const vector<D3DXVECTOR2>& vecMpUpBySocketOption );

#ifdef ADJUST_SECRET_ITEM_OPTION //김창한
		float						CalculateMpUpAttackedBySocketOption( vector<D3DXVECTOR4>& vecMpUpAttackedBySocketOption );
#endif //ADJUST_SECRET_ITEM_OPTION

		//}} kimhc // 2011-05-25 // SetOption, TitleOption에만 적용 되는 option 모든 itemOption에 적용 되도록 수정

		virtual void				ProcessAttackResult();
#ifdef SKILL_CASH_10_TEST
		void						ProcessDrainParticle( D3DXVECTOR3 vPos, bool bCreate = true );
#endif
#ifdef ATTACK_DELAY_GAGE
		//{{AFX
		virtual void				AddAttackDelay( float fDelay );
		//}}AFX
#endif ATTACK_DELAY_GAGE

		virtual void				AddForceDownDelay( float fDelay );
		virtual void				Win();
		virtual void				Lose();
		virtual void				Draw(); // 비겼다
#ifdef DUNGEON_NEW_RESULT
		virtual void				Wait();
#endif
		

		virtual D3DXVECTOR3			GetBonePos( const WCHAR* pBoneName ) const;
		virtual D3DXVECTOR3			GetHeadBonePos() const;	//optimization	

#ifdef MODIFY_GET_BONE_POS
		// 오현빈 // 2012-12-17 // 본이 없는 경우에 대해 체크하기 위해 OUT 인자를 사용하는 함수 추가
		virtual bool				GetBonePos( const WCHAR* pBoneName, OUT D3DXVECTOR3& pos_ ) const;
#endif //MODIFY_GET_BONE_POS

		//{{ kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)
		virtual const D3DXMATRIX*	GetCombineMatrixFromBoneName( const wstring& wstrBoneName ) const;
		//}} kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)

//{{ robobeg : 2008-10-21
		//virtual void				SetShow( bool bShow );
        virtual void                NotifyShowObjectChanged();
//}} robobeg : 2008-10-21

// X2GUUser 가 KTDGManager 에 등록, 삭제될 때 장착 부품들도 같이 등록, 삭제하기 위해 사용됨
        virtual void                NotifyDGManagerChanged( CKTDGManager& refInManager_ );
//}} robobeg : 2008-10-17

		virtual void				SetHyperModeCount( int i )			
		{ 
			if ( NULL != GetGageData() )
				GetGageData()->SetHyperModeCount( i );
			m_FrameDataFuture.syncData.m_HyperModeCount = i; 
		}
		
		virtual int					GetHyperModeCount()	const			{ return m_FrameDataFuture.syncData.m_HyperModeCount; }
		virtual float				GetHyperModeTime() const;

		virtual int					GetLastStateHyperModeCount() const { return m_iLastStateHyperModeCount; }
		
		virtual void				CreateAndInsertRenderParamByBuffUnitType( const KBuffIdentity& BuffIdentity_, const map<CX2Unit::UNIT_TYPE, StBuffRenderParamPtr>& mapStBuffRenderParamPtr_ );

		virtual float				GetStopTime();

		virtual void				SetStopTime( float fStopTime );
		virtual void				SetStop2Time( float fStopTime );

		virtual const UnitCondition& GetUnitCondition( bool bFuture = false ) const	{ return GetFrameData(bFuture).unitCondition; }
		virtual const StateParam&	GetStateparam( bool bFuture = false ) const	{ return GetFrameData(bFuture).stateParam; }

#ifdef USER_HOLD
		void						SetHold(bool val);
		bool						GetHold()								{ return m_bHold; }
		virtual void				SetPos( D3DXVECTOR3 vPos );
#endif

#ifdef TEST_GROUP_GRAP
		void						SetGrap( bool val );			/// 현재 잡혀 있는 상태인가?
		bool						GetGrap() { return m_bGrap; }	/// 현재 잡혀 있는 상태인가?
		void						SetGrapShakeCount( int val ){ m_iGrapShakeCount = val; }	/// 키보드를 좌우로 움직여야 하는 횟수 (이 횟수 만큼 흔들면 잡기에서 풀림)
		int							GetGrapShakeCount(){ return m_iGrapShakeCount; }			/// 키보드를 좌우로 움직여야 하는 횟수 (이 횟수 만큼 흔들면 잡기에서 풀림)

		int							GetGrapStateID(){ return m_iGrapStateID; }			/// 붙잡혔을 때 강제로 변경될 스테이트 (잡힌 동안에는 이 스테이트를 실행 함)
		void						SetGrapStateID(int val) { m_iGrapStateID = val; }	/// 붙잡혔을 때 강제로 변경될 스테이트 (잡힌 동안에는 이 스테이트를 실행 함)
		
		void						SetGrappedPosition( D3DXVECTOR3 vPos );		/// 붙잡한 상태에서의 유저 위치	//optimization
		D3DXVECTOR3					GetGrappedPosition(){ return m_vGrap; }		/// 붙잡한 상태에서의 유저 위치	//optimization
		void						SetBeforeGrapPosition( D3DXVECTOR3 vPos );	/// 붙잡히기 이전의 유저 위치		//optimization
		void						ResetToBeforeGrapPos();		/// 붙잡히기 이전의 유저 위치로 되돌림
#endif TEST_GROUP_GRAP


#ifdef NEW_GRAPPLE_ATTACK
		void						SetGrappled( bool val );
		bool						GetGrappled(){ return m_bGrappled; }
		
		void						SetGrappledStatdID( int val ){ m_iGrappledStateID = val; }
		int							GetGrappledStateID() { return m_iGrappledStateID; }
#endif NEW_GRAPPLE_ATTACK


		// 입력된 위치값을 지정된 라인을 지나는 Y축에 평행한 평면과 교차하는 지점으로 옮겨서 캐릭터 위치로 설정한다
		virtual void SetPositionOnLine( const D3DXVECTOR3& vPosition, const int iLineIndex );


		virtual const D3DXVECTOR3&	GetPos( bool bFuture = false )	const	{ return GetSyncData(bFuture).position; }	//optimization
		virtual float				GetXPos( bool bFuture = false )	const		{ return GetSyncData(bFuture).position.x; }
		virtual float				GetYPos( bool bFuture = false )	const		{ return GetSyncData(bFuture).position.y; }
		virtual float				GetZPos( bool bFuture = false )	const		{ return GetSyncData(bFuture).position.z; }
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        virtual void                BackupPosition()  
        {
            const SyncData& kSyncData = GetSyncData(true);
            const UnitCondition& kUnitCondition = GetUnitCondition(true);
            m_vPositionBackup = kSyncData.position;
            m_iPositionBackup_LineIndex = kSyncData.lastTouchLineIndex;
            m_bPositionBackup = true;
            m_bPositionBackup_FootOnLine = kUnitCondition.bFootOnLine;
        }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        virtual void    OnFrameMove_PostProcess()   { BackupPosition(); }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		virtual const D3DXVECTOR3&	GetRotateDegree() const					{ return m_FrameDataNow.unitCondition.dirDegree; }
		virtual float				GetXRotateDegree() const					{ return m_FrameDataNow.unitCondition.dirDegree.x; }
		virtual float				GetYRotateDegree() const					{ return m_FrameDataNow.unitCondition.dirDegree.y; }
		virtual float				GetZRotateDegree() const				{ return m_FrameDataNow.unitCondition.dirDegree.z; }

		virtual float				GetPlaySpeed() const					{ return m_pXSkinAnim->GetPlaySpeed(); }

		virtual const D3DXVECTOR3&	GetDirVector() const							{ return m_FrameDataNow.unitCondition.dirVector; }
		virtual bool				GetIsRight() const							{ return m_FrameDataNow.syncData.bIsRight; }
		virtual bool				GetIsRightBefore() const					{ return m_FrameDataNowBefore.syncData.bIsRight; }

		virtual GAME_UNIT_STATE_ID	GetGameUnitState() const					{ return (GAME_UNIT_STATE_ID)( (UCHAR) m_FrameDataNow.syncData.nowState); }
		virtual bool				GetInvincible()	const					{ return m_FrameDataNow.stateParam.bInvincible; }
		virtual bool				GetIsFallDownState()					
        { 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return m_bNowFallDown;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return m_FrameDataNow.stateParam.bFallDown; 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        float                       GetNowSlashTraceTipWide() const         { return m_fNowSlashTraceTipWide; }
        bool                        FindNowAddSlashTraceWithWeaponIndex( int iWeaponIndex, OUT D3DXVECTOR3& vSlaceTraceTime ) const;
        virtual UINT                GetSizeOfAttackTimeList() const             { return m_vecAttackTime.size(); }
        virtual const D3DXVECTOR2* const GetAttackTimeByIndex( const UINT uiIndex_ ) const
        {
            if ( uiIndex_ < m_vecAttackTime.size() )
                return  &m_vecAttackTime[ uiIndex_ ].m_v2AttackTime;
            else
                return  NULL;
        }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef FIX_MEDITATION_RUN
		virtual GAME_UNIT_STATE_ID	GetGameUnitStateFuture() const			{ return (GAME_UNIT_STATE_ID)( (UCHAR) m_FrameDataFuture.syncData.nowState); }
#endif FIX_MEDITATION_RUN

		virtual float				GetUnitWidth( bool bFuture ) const		{ return GetFrameData( bFuture ).unitCondition.fUnitWidth;	}
		virtual float				GetUnitHeight( bool bFuture ) const		{ return GetFrameData( bFuture ).unitCondition.fUnitHeight; }

		bool						GetAttackSuccess( bool bFuture )		{ return GetFrameData( bFuture ).unitCondition.bHit; }


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		void				        ReceiveFrameData( DWORD dwFrameMoveCount, const KDYNAMIC_UNIT_USER_SYNC& kUserSync );
        void                        _PostProcess_ReceiveFrameData();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		virtual void				ReceiveFrameData( KXPT_UNIT_USER_SYNC_PACK* pKXPT_UNIT_USER_SYNC_PACK );
//#ifdef UNIT_SYNC_PACKET_DUNGEON_FOR_TEST
//		//virtual void				ReceiveFrameData( KXPT_UNIT_USER_SYNC_PACK_FOR_DUNGEON* pKXPT_UNIT_USER_SYNC_PACK );
//#endif UNIT_SYNC_PACKET_DUNGEON_FOR_TEST
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		void						SendFrameData();

		virtual bool				IsMyUnit() const;
		virtual bool				IsLocalUnit() const;
		virtual bool				IsFocusUnit() const;

		//////////////////////////////////////////////////////////
		UidType						GetOwnerUnitUID()	{ return m_OwnerUserUID; }
		UidType						GetUnitUID() const	{ return m_UnitUID; }
		//virtual CX2Unit*			GetUnit() const		{ return m_pUnit; }
        CX2Unit*			        GetUnit() const		{ return m_pUnit; }
		const SyncData&				GetSyncData( bool bFuture = false ) const;
		const FrameData&			GetFrameData( bool bFuture = false ) const;
        const FrameData&			GetFrameDataDead() const { 	return m_FrameDataDead; }

		const InputData&			GetInputData() { return (const InputData&)m_InputData; }
		void						SetInputData( InputData* pInputData );
#ifdef GUUSER_UNIT_POINTER_CRASH_FIX
		void						SetUnit( CX2Unit* pUnit){ m_pUnit = pUnit; }
#endif GUUSER_UNIT_POINTER_CRASH_FIX
	
		void						ClearRecieveSyncData();

		CX2ComboManager*			GetComboManager() { return m_pComboManager; }
		virtual void				InitPosition( bool bUseTeamPos = true, int startPosIndex = -1 );
		virtual void				InitPosition( CX2GUUser* pCX2GUUser );
		virtual void				InitPosByDeadPos( CX2GUUser* pCX2GUUser );
		void						InitPosByLineData( int iLineIndex );
		void						InitPosByBattleFieldPosition();

//{{ kimhc // 2010.8.10 // 특정 몬스터의 위치 값을 부활 위치로 사용
		bool						InitPosByMonsterPos();
//}} kimhc // 2010.8.10 // 특정 몬스터의 위치 값을 부활 위치로 사용

		const FrameData&			GetNowFrameData() const { return m_FrameDataNow; }
		FrameData&			        AccessNowFrameData() { return m_FrameDataNow; }
//{{ robobeg : 2013-11-04
        // m_FrameDataFuture은 My unit 의 경우에만 의미있음. 다른 유져 캐릭에 대해서는 GetFrameData( true ) 을 사용하시오.
		const FrameData&			GetFutureFrameData() const { return m_FrameDataFuture; }
        FrameData&			        AccessFutureFrameData() { return m_FrameDataFuture; }
//}} robobeg : 2013-11-04
		void						SetRebirthTime( float fRebirthTime ){ m_fRebirthTime = fRebirthTime; }
		float						GetRebirthTime(){ return m_fRebirthTime; }

		virtual int					GetNowStateID() const { return m_NowStateData.stateID; }
		int							GetFutureStateID() const { return m_FutureStateData.stateID; }
		CX2SkillTree::SKILL_ID		GetNowStateSkillID() const { return m_NowStateData.m_eSkillID; }
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		CX2SkillTree::SKILL_ID		GetFutureStateSkillID() const { return m_FutureStateData.m_eSkillID; }
#endif //SERV_ADD_LUNATIC_PSYKER

		bool IsHyperState() { return m_NowStateData.m_bHyperState; }

		virtual void				RenderName();

		virtual void				InitEffect();

		virtual void				InitializeGUUserMajorParticleArray();		// 메이저 파티클 배열 초기화	// kimhc // 2010.11.5
		virtual void				CreateMajorParticleArray();			// 메이저 파티클 배열을 생성함	// kimhc // 2010.11.5
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void					AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CKTDGParticleSystem::CParticleEventSequence* SetCommonMajorParticleByEnum( COMMON_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// 공통적으로 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클을 셋팅함 // kimhc // 2010.11.5 

		ParticleEventSequenceHandle	GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) const	// 공통적으로 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴// kimhc // 2010.11.5 
		{
			ASSERT( COMMON_MAJOR_PII_END > eVal_ && COMMON_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahCommonMajorParticleInstance[eVal_];
		}
		
		void						SetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ )	// 공통적으로 쓰는 메이저 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함 // kimhc // 2010.11.5 
		{
			ASSERT( COMMON_MAJOR_PII_END > eVal_ && COMMON_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			m_ahCommonMajorParticleInstance[eVal_] = hHandle_;
		}
		void						DeleteGUUserMajorParticle();
		

		virtual void				InitializeGUUserMinorParticleArray();	// 마이너 파티클 배열 초기화	// kimhc // 2010.11.6 
		virtual void				CreateMinorParticleArray();		// 마이너 파티클 배열을 생성함	// kimhc // 2010.11.5 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void					AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CKTDGParticleSystem::CParticleEventSequence* SetCommonMinorParticleByEnum( COMMON_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrPatricleName_, int iDrawCount_ = -1 ); // 공통적으로 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클을 셋팅함 // kimhc // 2010.11.5 
		ParticleEventSequenceHandle	GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PARTICLE_INSTANCE_ID eVal_ ) const // 공통적으로 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		{
			ASSERT( COMMON_MINOR_PII_END > eVal_ && COMMON_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahCommonMinorParticleInstance[eVal_];
		}
		void						SetHandleCommonMinorParticleByEnum( COMMON_MINOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ ) // 공통적으로 쓰는 마이너 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함 // kimhc // 2010.11.5 
		{
			ASSERT( COMMON_MINOR_PII_END > eVal_ && COMMON_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			m_ahCommonMinorParticleInstance[eVal_] = hHandle_;
		}
		void						DeleteGUUserMinorParticle();

		virtual void				DeleteMinorParticle();

		// 공통으로 사용하는 weapon enchant particle 관련 함수
		void CreateEnchantedWeaponParticleAtHand( const WCHAR* pName1, const WCHAR* pName2 = NULL, const WCHAR* pName3 = NULL, const WCHAR* pName4 = NULL );
		void ProcessEnchantedWeaponEffectAtHand( CX2Unit::UNIT_TYPE unitType );
		void InitEnchantedWeaponEffectAtHand();
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void	AppendEnchantedWeaponEffectAtHandToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		void SetShowEnchantedWeaponEffectAtHand( bool bShow );


				
		// unit class 별로 별도로 사용하는 weapon enchant particle 관련 함수, CX2GameUnit::Weapon 클래스에 포함되어 있는 파티클이다
		virtual void				InitEnchantWeaponEffectUnitSpecific() = NULL;



		//////////////////////////////////////////////////////////////////////////
		bool						GetHyperModeUsed() { return m_bHyperModeUsed; }		// 각성을 사용했는지 안했는지
		const UserUnitStateDataMap&		GetStateList() const { return m_StateList; }
		void						ToggleSkillSlotAB();

		bool						GetResurrectionTimeOut() { return m_bResurrectionTimeOut; }
		void						SetResurrectionTimeOut( bool bCheck ) { m_bResurrectionTimeOut = bCheck; }
		
//{{ kimhc // 2010.12.17 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		// 락온 타입을 추가함
		void						SetLockOn( CX2DamageEffect::CEffect* pCEffect, int randomOffset = 0, CX2DamageEffect::LOCK_ON_TYPE eLockOnType_ = CX2DamageEffect::LOT_NONE, int iModulusFactor = 30 );
#else	NEW_CHARACTER_CHUNG
		void						SetLockOn( CX2DamageEffect::CEffect* pCEffect, int randomOffset = 0 );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.17 //  2010-12-23 New Character CHUNG

		void						SetDevAddDamage(float val = 0.f) { m_fDevAddDamage = val; }
		void						ForceChangeHyperMode( int iAddBall_ );
		void						ForceChangeHyperModeWithoutMotion( float fAddTime_ );


		void						SetNowSpecialAttack(int val) { m_iNowSpecialAttack = val; }
		int							GetNowSpecialAttack() const { return m_iNowSpecialAttack; }
		bool						GetHoldSpecialAttack() const { return m_bHoldSpecialAttack; }
		bool						GetHoldingSpecialAttack() const { return m_bHoldingSpecialAttack; }
//#ifndef NOT_USE_DICE_ROLL
//		DiceRoll*					GetDiceRoll() { return m_pDiceRoll; }
//#endif //NOT_USE_DICE_ROLL

		const CX2GUUser::CashItemAbility& GetCashItemAbility() const { return m_CashItemAbility; }
        CX2GUUser::CashItemAbility& AccessCashItemAbility() { return m_CashItemAbility; }


		int 				GetRandomIntFuture( int iOffset = 0 ) { return CKTDXRandomNumbers::GetRandomInt( m_FrameDataFuture.syncData.m_usRandomTableIndex + iOffset ); }
		float				GetRandomFloatFuture( int iOffset = 0 ) { return CKTDXRandomNumbers::GetRandomFloat( m_FrameDataFuture.syncData.m_usRandomTableIndex + iOffset ); }


		virtual int 				GetRandomInt( int iOffset = 0 ) { return CKTDXRandomNumbers::GetRandomInt( m_FrameDataNow.syncData.m_usRandomTableIndex + iOffset ); }
		virtual float				GetRandomFloat( int iOffset = 0 ) { return CKTDXRandomNumbers::GetRandomFloat( m_FrameDataNow.syncData.m_usRandomTableIndex + iOffset ); }


		virtual const D3DXVECTOR3&	GetLandPosition() const { return m_FrameDataNow.unitCondition.landPosition; }


		virtual int					GetLastTouchLineIndex( bool bFuture = false ) const
		{
			if( true == bFuture )
				return m_FrameDataFuture.syncData.lastTouchLineIndex;
			else 
				return m_FrameDataNow.syncData.lastTouchLineIndex;
		}

		virtual const D3DXVECTOR3&		GetLandPos( bool bFuture = false ) const
		{
			if( true == bFuture )
				return m_FrameDataFuture.unitCondition.landPosition; 
			else
				return m_FrameDataNow.unitCondition.landPosition; 
		}



		const wstring GetRandomWinSpeech() //optimization
		{
			if( m_vecDungeonWinSpeech.size() == 0 )
			{
				return L"VICTORY";
			}

			int iRand = rand() % (int) m_vecDungeonWinSpeech.size(); 
			return m_vecDungeonWinSpeech[ iRand ];
		} 


		//virtual float				GetEvadePercent();
		//virtual float				GetAntiEvadePercent();

		//{{ kimhc // 2011-07-19 // 옵션데이타 수치화 작업
		// 옵션 수치화에 의한 추가 데미지
#ifdef ELSWORD_SHEATH_KNIGHT
		/*virtual*/ float			GetAdditionalAttackDamage( const CX2DamageManager::DamageData* pAttackDamageData );
#else
		/*virtual*/ float			GetAdditionalAttackDamage( const CX2DamageManager::DamageData* pAttackDamageData ) const;
#endif ELSWORD_SHEATH_KNIGHT
		//}} kimhc // 2011-07-19 // 옵션데이타 수치화 작업

		int							GetSkillLevelUpNum( CX2SkillTree::SKILL_ID skillID ) const;

#ifdef VERIFY_STAT_BY_BUFF
		virtual	void				TransformScale( const PROTECT_VECTOR3& vScale_ );
#else	// VERIFY_STAT_BY_BUFF
		virtual	void				TransformScale( const D3DXVECTOR3& vScale_ );
#endif // VERIFY_STAT_BY_BUFF
		
		virtual float				GetCriticalPercent( BYTE byDamageType, float fExtraCritical = 0.f );
		
		void						ChangeModelDetail( int detailPercent );

		bool						GetIsRidingMachine() { return m_bRidingMachine; }


		bool						IsImmuneToEnchant( CX2EnchantItem::ENCHANT_TYPE enchantType );
		void						AddEnchantResist( const CX2SocketItem::SocketData& kSocketData );
#ifdef PET_AURA_SKILL
		void						AddEnchantAttack( const CX2SocketItem::SocketData& kSocketData);
		bool						GetApplyPetAura() { return m_bApplyAura; }
		void						SetApplyPetAura( bool bVal ) { m_bApplyAura = bVal; }
#endif
		//void AddEnchantResist( CX2EnchantItem::ENCHANT_TYPE eEnchantType, float fResistValue );


// 김상훈 2010.11.1
#ifdef NEW_SKILL_2010_11
	#ifndef FIX_LIMITED_MANA_MANAGEMENT
		void						SetLimitManaManagement( bool bBool ){ m_bLimitManaManagement = bBool; }
	#endif //FIX_LIMITED_MANA_MANAGEMENT
#endif 
// 김상훈 2010.11.1

		virtual void				ShowSnatchCureEffect();

		bool						IsSummonSpecialAttack( CX2SkillTree::SKILL_ID iSkiiID );
		bool						CheckSummonSpecialAttack( CX2SkillTree::SKILL_ID iSkillID_ );

		float						GetDamageUpPercentBySMA( int npcID ); // SpecificMonsterAttack
		float						GetEvadeUpPerBySMA( int npcID );

		const CKTDGXSkinAnim* GetXSkinAnim() const { return m_pXSkinAnim.get(); }
		//{{ kimhc // 실시간 엘소드 중 실시간 레벨업 이펙트 효과
		CKTDGXSkinAnimPtr GetXSkinAnimPtr() const { return m_pXSkinAnim; }
		//{{ kimhc // 실시간 엘소드 중 실시간 레벨업 이펙트 효과
		const CKTDXDeviceXSkinMesh* GetMotion() const { return m_pMotion; }


		int GetDamageDownLandFront() const { return USI_DAMAGE_DOWN_FRONT; }
		int GetDamageDownLandBack() const { return USI_DAMAGE_DOWN_BACK; }
		int GetDamageAirDownLanding() const { return USI_DAMAGE_AIR_DOWN_LANDING; }



		void SetPepperRunTime(float val) { m_fPepperRunTime = val; }
		void StartPepperRun( float fTime );



		void GenericGroundComboEndEventProcess();
		void GenericSpecialActiveSkillEventProcess();
		void GenericActiveSkillEventProcess();



		bool IsOnSomethingFuture()
		{
//#ifdef STEP_ON_MONSTER_TEST
//			return m_FrameDataFuture.unitCondition.bFootOnLine || 
//					m_FrameDataFuture.unitCondition.bFootOnUnit;
//#else STEP_ON_MONSTER_TEST
			return m_FrameDataFuture.unitCondition.bFootOnLine;
//#endif STEP_ON_MONSTER_TEST
		}

#ifdef CLIFF_CLIMBING_TEST
		
		bool IsOnCliffFuture();
		void ClimbWaitEventProcess();
		void ClimbFrontFrameMoveFuture();
		void ClimbFrontEventProcess();
		void ClimbUpEventProcess();
		void ClimbDownEventProcess();
		void ClimbUpFrontFrameMoveFuture();
		void ClimbUpFrontEventProcess();
		void ClimbDownFrontFrameMoveFuture();
		void ClimbDownFrontEventProcess();


#endif CLIFF_CLIMBING_TEST


		void ResetOnSomethingFuture()
		{
			m_FrameDataFuture.unitCondition.bFootOnLine = false;
//#ifdef STEP_ON_MONSTER_TEST
//			m_FrameDataFuture.unitCondition.bFootOnUnit = false;
//#endif STEP_ON_MONSTER_TEST
		}

	//{{ kimhc // 2010.11.3 // 신스킬 작업
#ifdef	NEW_SKILL_2010_11
		// 레피 - 승화된 분노
		bool GetCanChangeHyperModeInHyperModeState() const { return m_bCanChangeHyperModeInHyperModeState; };
		void SetCanChangeHyperModeInHyperModeState( bool bVal_ ) { m_bCanChangeHyperModeInHyperModeState = bVal_; }
#endif	NEW_SKILL_2010_11
	//}} kimhc // 2010.11.3 // 신스킬 작업


		//////////////////////////////////////////////////////////////////////////
		
		virtual void PEPPER_RUN_READY_EventProcess();
		virtual void PEPPER_RUN_FrameMoveFuture();
		virtual void PEPPER_RUN_EventProcess();
		virtual void PEPPER_RUN_END_FrameMove();
		virtual void PEPPER_RUN_END_EventProcess();

		virtual void PEPPER_RUN_JUMP_UP_StateStartFuture();
		virtual void PEPPER_RUN_JUMP_UP_FrameMoveFuture();
		virtual void PEPPER_RUN_JUMP_UP_EventProcess();

		virtual void PEPPER_RUN_JUMP_DOWN_StateStartFuture();
		virtual void PEPPER_RUN_JUMP_DOWN_FrameMoveFuture();
		virtual void PEPPER_RUN_JUMP_DOWN_EventProcess();


		virtual void DAMAGE_GROGGY_EventProcess();

#ifdef SPECIAL_USE_ITEM
		void THROW_ITEM_StateStart();
		void THROW_ITEM_FrameMoveFuture();
		void THROW_ITEM_FrameMove();
		void THROW_ITEM_CameraMove();
		void THROW_ITEM_EventProcess();
		void THROW_ITEM_StateEnd();
#endif SPECIAL_USE_ITEM

		void THROW_WOODEN_PIECE_FrameMoveFuture();
		virtual void THROW_WOODEN_PIECE_FrameMove() {}
		void THROW_WOODEN_PIECE_CameraMove();
		void THROW_WOODEN_PIECE_EventProcess();

		

		void MACHINE_GUN_RIDE_StartFuture();
		void MACHINE_GUN_RIDE_Start();
		void MACHINE_GUN_RIDE_FrameMoveFuture();
		void MACHINE_GUN_RIDE_FrameMove();
		void MACHINE_GUN_RIDE_CameraMove();
		void MACHINE_GUN_RIDE_EventProcess();

		void MACHINE_GUN_LEAVE_StartFuture();
		void MACHINE_GUN_LEAVE_Start();
		void MACHINE_GUN_LEAVE_FrameMoveFuture();
		void MACHINE_GUN_LEAVE_FrameMove();
		void MACHINE_GUN_LEAVE_CameraMove();
		void MACHINE_GUN_LEAVE_EventProcess();

		void MACHINE_GUN_WAIT_StartFuture();
		void MACHINE_GUN_WAIT_Start();
		void MACHINE_GUN_WAIT_FrameMoveFuture();
		void MACHINE_GUN_WAIT_EventProcess();
		
		void MACHINE_GUN_WALK_FrameMoveFuture();
		void MACHINE_GUN_WALK_EventProcess();
		
		void MACHINE_GUN_ATTACK_FrameMoveFuture();
		void MACHINE_GUN_ATTACK_FrameMove();
		void MACHINE_GUN_ATTACK_EventProcess();

		void MACHINE_GUN_JUMP_UP_FrameMoveFuture();
		void MACHINE_GUN_JUMP_UP_EventProcess();

		void MACHINE_GUN_JUMP_DOWN_FrameMoveFuture();
		void MACHINE_GUN_JUMP_DOWN_EventProcess();

		void SUMMON_MAGIC_SPEAR_FrameMove();
		void SUMMON_MAGIC_SPEAR_EventProcess();

		virtual void COMMON_BUFF_FrameMove() = NULL;
		void COMMON_BUFF_EventProcess();

#ifdef ADDED_RELATIONSHIP_SYSTEM
		virtual void COMMON_RELATIONSHIP_SKILL_FrameMove();
		virtual void COMMON_RELATIONSHIP_SKILL_EventProcess();
#endif // ADDED_RELATIONSHIP_SYSTEM

		// Start 스테이트
		void StartEventProcess();
		void StartEnd();
		// StartIntrude 스테이트
		void StartIntrudeStart();
		void StartIntrudeFrameMove();
		void StartIntrudeEventProcess();
		// Die 스테이트
		void DieFrameMove( float fTimeCreateStepDust_, float fTimeSacrificeOfHero_ );	// Front, Back 공통으로 사용
		// WaitStart 상태에서 조건에 따라 애니메이션을 실행 시킴
		void PlayAnimationWaitStart( CKTDGXSkinAnim* pXSkinAnim_, const FrameData& refFrameData_ );	
		bool PlaySitReadyWaitStart( CKTDGXSkinAnim* pXSkinAnim_ );
		void WalkFrameMoveFuture();

		// JumpUp, JumpDown 상태에서 이동과 관련하여 공통으로 처리되는 부분
		void JumpFrameMoveFuture();
		void JumpLandingStart();

		// Dash Jump Landing
		void DashJumpLandingStartFuture();

		// Dash
		virtual void DashStartFuture();
		void DashStart();
		void DashFrameMoveFuture();

		// StandUp( Back, Front )
		void DamageStandUpEventProcess();
		void DamageStandUpEnd();

		// DamageAirSmall
		void DamageAirSmallStartFuture();
		void DamageAirSmallEventProcess();

		// DamageAirDown
		void DamageAirDownEventProcess();

		// DamageAirDown_INVINCIBLE
		void DamageAirDownInvincibleEventProcess();

		// DamageAirDownLanding
		void DamageAirDownLandingStart();
		void DamageAirDownLandingFrameMove();

		// DamageAirFall
		void DamageAirFallEventProcess();
		// DamageAirUp
		void DamageAirUpEventProcess();
		// DamageAirFly
		void DamageAirFlyEventProcess();
		// DamageRevenge
		void DamageRevengeStart();
		void DamageRevengeCameraMove();
		void DamageRevengeEventProcess();
		// Wall Jump	
#ifdef WALL_JUMP_TEST
		void WallLandingEventProcess();
		void WallLandingEndFuture();
#endif WALL_JUMP_TEST

		// 각성
		void HyperModeStart();
		virtual void HyperModeFrameMove() = NULL;
		void HyperModeEventProcess();
#ifdef HYPER_MODE_FIX
		void HyperModeStartFuture();
		void HyperModeEnd();
#endif HYPER_MODE_FIX

#ifdef MODIFY_RIDING_PET_AWAKE
		// 각성
		void			RidingHyperModeStart();
		void			RidingHyperModeEventProcess();
#ifdef HYPER_MODE_FIX
		void			RidingHyperModeStartFuture();
		void			RidingHyperModeEnd();
#endif HYPER_MODE_FIX

#endif // MODIFY_RIDING_PET_AWAKE

#ifdef RIDING_SYSTEM
		void RidingStartStart();			// 던전 시작
		void RidingStartStartFuture();
		void RidingStartEventProcess();

		void RidingOnStart();			// 탑승
		void RidingOnStartFuture();
		void RidingOnEventProcess();
		void RidingOnEnd();
				
		void RidingOffEventProcess();		// 내림
		void RidingOffEnd();

		void RidingWaitHabitStart();		// 습관
		void RidingWaitHabitStartFuture();
		void RidingWaitHabitEventProcess();

		void RidingWaitStart();				// 대기
		void RidingWaitStartFuture();
		virtual void RidingWaitEventProcess();

		void RidingWalkStart();				// 걷기
		void RidingWalkStartFuture();
		virtual void RidingWalkEventProcess();

		void RidingJumpUpStart();			// 점프
		void RidingJumpUpStartFuture();
		void RidingJumpUpEventProcess();

		void RidingJumpDownStart();			// 점프 내려옴
		void RidingJumpDownStartFuture();
		void RidingJumpDownEventProcess();

		void RidingJumpLandingStart();		// 착지
		void RidingJumpLandingStartFuture();
		virtual void RidingJumpLandingEventProcess();

		void RidingDashStart();				// 대시
		virtual void RidingDashEventProcess();

		void RidingDashEndStart();			// 대시 끝
		void RidingDashEndStartFuture();
		virtual void RidingDashEndEventProcess();
		void RidingDashEndFrameMove();

		void RidingDashJumpStart();			// 대시 점프
		void RidingDashJumpStartFuture();
		void RidingDashJumpEventProcess();
		void RidingDashJumpFrameMoveFuture();
		void RidingDashJumpEndFuture();

		void RidingDashJumpLandingStart();	// 대시 점프 착지
		void RidingDashJumpLandingEventProcess();

		void RidingDamageFrontStart();		// 피격 앞
		void RidingDamageFrontStartFuture();
		void RidingDamageFrontEventProcess();

		void RidingDamageBackStart();		// 피격 뒤
		void RidingDamageBackStartFuture();
		void RidingDamageBackEventProcess();
				
		void RidingAttackZStart();
		void RidingAttackZStartFuture();
		void RidingAttackZEventProcess();

		void RidingJumpAttackZStart();
		void RidingJumpAttackZStartFuture();
		void RidingJumpAttackZFrameMoveFuture();
		void RidingJumpAttackZEventProcess();
		
		void RidingAttackXStart();
		void RidingAttackXStartFuture();
		void RidingAttackXEventProcess();

		void RidingSpecialAttackStart();
		void RidingSpecialAttackStartFuture();
		void RidingSpecialAttackFrameMove();
		void RidingSpecialAttackEventProcess();
				
		void RidingSpecialMoveStartFuture();		// AirialDash, DoubleJump
		void RidingSpecialMoveStart();				// AirialDash, BoostDash
		void RidingSpecialMoveEventProcess();		// AirialDash, BoostDash, DoubleJump
		void RidingSpecialMoveFrameMoveFuture();	// AirialDash, BoostDash
		void RidingSpecialMoveFrameMove();			// AirialDash, BoostDash
		void RidingSpecialMoveEndFutrue();			// AirialDash
		void RidingSpecialMoveEnd();				// BoostDash

		int	GetRidingStartStateID() const { return USI_RIDING_START; }
		int	GetRidingWaitStateID() const { return USI_RIDING_WAIT; }
		int	GetRidingWaitHabitStateID() const { return USI_RIDING_WAIT_HABIT; }

		const D3DXVECTOR3&	GetSaddlePos() const { return m_vSaddlePos; }
		void		SetSaddlePos( IN D3DXVECTOR3 vPos_ ) { m_vSaddlePos = vPos_; }
		const D3DXVECTOR3&	GetSaddleDegree() const { return m_vDirDegree; }
		void		SetSaddleDegree( IN D3DXVECTOR3 vDegree_ ) { m_vDirDegree = vDegree_; }
		const wstring& GetRidingMotionName() const { return m_wstrRidingMotionName; }
		void		SetRidingMotionName( IN const wstring& wstrName_ ) { m_wstrRidingMotionName = wstrName_; }

		virtual bool GetRidingOn() const { return m_bRidingOn; }
		void SetRidingOn( IN bool bVal_ ) { m_bRidingOn = bVal_; }
		void ProcessRidingOn( IN bool bVal_, IN const KRidingPetInfo& _Info  = KRidingPetInfo() );

		const std::vector<TIME_SPEED>& GetSpeedFatorVector() const { return m_vecSpeedFactor; }
        std::vector<TIME_SPEED>& AccessSpeedFatorVector() { return m_vecSpeedFactor; }

		void SetDeleteEffectSetOnStateEnd( IN bool bVal_ ) { m_bDeleteEffectSetOnStateEnd = bVal_; }
		void SetDeleteEffectSetOnDamageReact( IN bool bVal_ ) { m_bDeleteEffectSetOnDamageReact = bVal_; }
		void SetDeleteEffectSetOnDie( IN bool bVal_ ) { m_bDeleteEffectSetOnDie = bVal_; }

		bool RidingPetSpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND );

		const vector<SoundPlayData>& GetSoundPlayData() const { return m_vecSoundPlayData; }
        vector<SoundPlayData>& AccessSoundPlayData() { return m_vecSoundPlayData; }

		void SetStateSuperArmor( IN bool bVal_ ) { m_bStateSuperArmor = bVal_; }
		
		void SetSkillCancelAfter( IN float fVal_ ) { m_fSkillCancelAfter = fVal_; }
		void SetBWalkCancelAfter( IN float fVal_ ) { m_fBWalkCancelAfter = fVal_; }
		void SetWalkCancelAfter( IN float fVal_ ) { m_fWalkCancelAfter = fVal_; }
		void SetDashCancelAfter( IN float fVal_ ) { m_fDashCancelAfter = fVal_; }
		virtual bool CanRidingState();
#ifdef CHECK_CAMERA_INFO
		void SetIsLineMapCamera( IN bool bVal_ ) { m_bIsLineMapCamera = bVal_;}
#endif // CHECK_CAMERA_INFO
		float   GetCameraAngleDegree() const { return m_fAngleDegree; }
		const GameCameraOffset& GetGameCameraOffset() const { return m_GameCameraOffset; }
#endif //RIDING_SYSTEM
		
		// 각성 할 때 보여줘야 하는 이펙트
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		void ShowMinorParticleHyperModeChange( const wstring& wstrBoneName = L"Dummy1_Rhand" );
		void ShowMinorParticleHyperModeShock( const wstring& wstrBoneName = L"Dummy1_Rhand" );
#else // SERV_9TH_NEW_CHARACTER
		void ShowMinorParticleHyperModeChange();
		void ShowMinorParticleHyperModeShock();
#endif // SERV_9TH_NEW_CHARACTER
		void ShowMinorParticleHyperModeTrace();
#ifdef HYPER_MODE_EFFECT_ADD_BY_ITEM
		void ShowSpecialEffectHyperMode();
#endif // HYPER_MODE_EFFECT_ADD_BY_ITEM

//#ifdef PVP_BOSS_COMBAT_TEST
//
//		void Frozen_StateStart();
//		void Frozen_StateEnd();
//		void Frozen_EventProcess();
//		
//#endif PVP_BOSS_COMBAT_TEST

#ifdef UNIT_EMOTION
		bool PlayEmotion(CX2Unit::EMOTION_TYPE eEmotionId, wstring &wstrEmotionType);
		CX2Unit::EMOTION_TYPE GetNowEmotionId() { return m_ePlayedEmotion; }
#endif

#ifdef EQUIP_STRIP_TEST
		bool IsEquipStripped( CX2Unit::EQIP_POSITION eEquipPosition );
		void SetEquipStrippedState( CX2Unit::EQIP_POSITION eEquipPosition, bool bStripped );
		CX2EqipPtr GetBasicEquipAtStrippedPosition( CX2Unit::EQIP_POSITION eEquipPosition );
		void SetBasicEquipAtStrippedPosition( CX2Unit::EQIP_POSITION eEquipPosition, CX2EqipPtr pEquip );
#endif EQUIP_STRIP_TEST

		//{{ kimhc // 실시간 엘소드 중 실시간 칭호, 장비 교체 관련 부분 추가
#ifdef REAL_TIME_ELSWORD
		void			UpdateSocketDataAndEnchantData();
		virtual void	UpdatePassiveAndActiveSkillState();
		void			UpdateEquippedEmblem();
		
//{{ kimhc // 2010.12.10 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	
	#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
		#ifdef FIX_ARA_ONE_PIECE_FOR_BERSERK
		virtual void UpdateNotRenderingOtherEquipList();
		#else //FIX_ARA_ONE_PIECE_FOR_BERSERK
		void UpdateNotRenderingOtherEquipList();
		#endif //FIX_ARA_ONE_PIECE_FOR_BERSERK
	#endif	NOT_RENDERING_OTHER_EQUIP_POS
		void UpdateEquipCollisionData( IN vector<CX2EqipPtr>& m_vecEquipList );
		CX2EqipPtr	GetEquipPtrFromEquipViewListByEquipPosition( const CX2Unit::EQIP_POSITION eEquipPosition );			
		virtual void SetShowViewList( bool bShow_ );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.10 //  2010-12-23 New Character CHUNG
		
		virtual void		UpdateWeaponEnchantEffectAndExtraDamageType();
		virtual void		UpdateEquipCollisionData();
		virtual bool		UpdateEquipViewListInfo( CX2Item* pItemToAddInViewList, CX2Item* const pItemToRemoveInViewList );
		void		AddEquipToViewList( CX2Item* const pItem );
		void		RemoveEquipFromViewList( CX2EqipPtr pEquipPtrToRemove, int index	);	//optimization
		//void		RemoveEquipFromViewListByEquipPostion( CX2Unit::EQIP_POSITION equipPosition );
		void		DeleteEnchantedWeaponEffectAtHand();

		void UpdateSlashTraceTextureName();
#ifdef FIX_SLASH_TRACE
		void UpdateSlashTrace()
		{
			if( m_LuaManager.BeginTable( "INIT_COMPONENT" ) == true )
			{
				for( UINT i=0; i<m_vecpWeapon.size(); ++i )
				{
					Weapon* pWeapon = m_vecpWeapon[i];
					pWeapon->InitSlashTrace();
					pWeapon->LoadDataForUser( m_LuaManager );
				}

				m_LuaManager.EndTable(); // INIT_COMPONENT
			}	
		}
#endif
#endif REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 실시간 칭호, 장비 교체 관련 부분 추가




		float GetStatAtkRateOfSummoned() const { return m_SkillRelatedData.m_fStatAtkRateOfSummoned; }
		const CX2SkillTree::SkillTemplet* GetEquippedActiveSkillTemplet( CX2SkillTree::SKILL_ID eSkillID ) const;

#ifdef SERV_SKILL_NOTE
		bool GetEqippedSkillMemo(CX2SkillTree::SKILL_MEMO_ID eMemoId) const;
#endif


		void ResetMaxHP();
		void ResetMaxMP();
		void ResetMPChangeRate( float fMPChangeRate );

		
		void CreateTeleportEffect( const D3DXVECTOR3& vPos_, const D3DXVECTOR3& vRotDegree_ );	//optimization

#ifdef NO_DETONATION
		bool GetNoDetonation() { return m_bNoDetonation; }
		void SetNoDetonation(bool val) { m_bNoDetonation = val; }
#endif

		virtual void SetEnableDash( bool bEnableDash, ENABLE_DASH_STATE eState = EDS_END );
		void StateChangeDashIfPossible();

//{{ robobeg : 2012-11-16
		static CX2DamageManager::EXTRA_DAMAGE_TYPE	CalcWeaponEnchantExtraDamageType( CX2Unit* pX2Unit_ );
//}} robobeg : 2012-11-16
		void CalcWeaponEnchantExtraDamageType();

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 현제 무기의 강화 레벨 반환
		static int CalcWeaponEnchantLevel( CX2Unit* pX2Unit_ );
#endif ADD_UPGRADE_WEAPON_PARTICLE



#ifdef HEAD_ATTACHED_CAMERA_TEST
		bool BoneAttachedCamera( const wstring& boneName, const D3DXVECTOR3& vEyeOffset = D3DXVECTOR3(0, 0, 0) );
		bool BoneAttachedCamera( const CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, const D3DXVECTOR3& vEyeOffset = D3DXVECTOR3(0, 0, 0) );
#endif HEAD_ATTACHED_CAMERA_TEST

		//{{ kimhc // 2009-11-20 // 길드 스킬 3, 4라인
#ifdef	GUILD_SKILL_PART_2
		const GuildSkillData& GetGuildSkillData() { return m_GuildSkillData; }	//optimization

		virtual void GuildSkillProcess( const CX2SkillTree::SKILL_ID eSkillID );
		void ProcessSacrificeOfHero();		// 영웅의 희생 길드스킬
#endif	GUILD_SKILL_PART_2
		//}} kimhc // 2009-11-20 // 길드 스킬 3, 4라인

		bool DoScaleHeadBone();

#ifdef DUNGEON_ITEM		
		void SetSuperArmor(bool bVal, float fVal)
		{
			m_bSuperArmorByItem = bVal;
			m_fSuperArmorByItem = fVal;
		}

		void ScaleHead();
		void RestoreHead();
#endif

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-04-01
		void ScaleHand( D3DXVECTOR3 vChangeScale );
		void RestoreHand();
#endif // SERV_NEW_DEFENCE_DUNGEON

		//{{ kimhc // 2010.2.10 //	나소드 무기 강화 핸들러
#ifdef	EVE_SECOND_CLASS_CHANGE
		void				SetHitNasodWeapon( bool bValue ) { m_bHitNasodWeapon = bValue; }
		bool				DidHitNasodWeapon() const { return m_bHitNasodWeapon; }
#endif	EVE_SECOND_CLASS_CHANGE
		//}} kimhc // 2010.2.10 //	나소드 무기 강화 핸들러

//#ifdef TRANSFORMER_TEST
//		void TransformIntoUser();
//		void TransformIntoMonster( CX2UnitManager::NPC_UNIT_ID eNPCID );
//
//		CX2GUNPC* GetTransformer() { return m_pTransformer; }
//		void SetTransformer( CX2GUNPC* val) { m_pTransformer = val; }
//
//
//#endif TRANSFORMER_TEST
		
		//{{ 임규수 임규수 던전 스타트 표시 나오기 전 , 던전 보스 죽인 후에 퀵슬롯 사용 못하도록 수정
#ifdef FIX_QUICK_SLOT_USE_DUNGEON_PLAY
		bool GetStarted(){ return m_bStarted; }
#endif 
		//}}

		int GetUnitLevel() const
		{
			if(m_pUnit != NULL)
				return GetUnit()->GetUnitData().m_Level; 
			return 0;
		}

		const CX2Unit::UNIT_CLASS GetUnitClass()
		{
			if( m_pUnit != NULL )
			{
				return GetUnit()->GetUnitData().m_UnitClass;
			}

			return CX2Unit::UC_NONE;
		}

		const CX2Unit::UNIT_TYPE GetUnitType()
		{
			if( m_pUnit != NULL )
				return GetUnit()->GetType();

			return CX2Unit::UT_NONE;
		}

#ifdef SERV_PET_SYSTEM
		int GetDieCount() { return m_iDieCount; }
		const PetCheer& GetPetCheer() const { return m_petCheer; }
        PetCheer& AccessPetCheer() { return m_petCheer; }
#endif

		//{{ JHKang / 강정훈 / 2010/12/16 / 현재 피격 상태인가?
#ifdef SEASON3_MONSTER_2010_12
		bool GetNowHit() { return m_bNowHit; }
		void SetNowHit( IN bool bHit_ ) { m_bNowHit = bHit_; }
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / 강정훈 / 2010/12/16 / 현재 피격 상태인가?

		//{{ JHKang / 강정훈 / 2011/01/17 / 최종 데미지 값을 전달할 멤버 함수
#ifdef SEASON3_MONSTER_2010_12
		float GetRealDamage() { return m_fRealDamage; }
		void SetAnimSpeed_LUA( IN float fAnimSpeedRate_ )
		{
			m_fAnimSpeedRateByNpc = fAnimSpeedRate_;
		}
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / 강정훈 / 2011/01/17 / 최종 데미지 값을 전달할 멤버 함수

		float GetOxygenGage() const { return m_fOxygenGage; }
		//{{ JHKang / 강정훈 / 2011/02/14 / 던전 랭크 개선 관련
#ifdef DUNGEON_RANK_NEW
		/// HP 회복량( 소비 아이템(포션, 음식) / 상자, 일리피아, 등 기타 회복류 제외 )
		void	AddRecoveryHP( IN int iHP_ ) { m_iTotalRecoveryHP += iHP_; }
		int		GetTotalAttackedDamage( void ) { return m_iTotalAttackedDamage; }
		void	SetTotalAttackedDamage( IN int iAttackedDamage_ ) { m_iTotalAttackedDamage = iAttackedDamage_; }
		int		GetTotalRecoveryHP( void ) { return m_iTotalRecoveryHP; }
		int		GetUsingSkillCount( void );
		void	CountUsedSkill( int iSlotNumber_ );
		int		GetTotalGivenDamamge( void ) { return m_iTotalGivenDamamge; }
		void	AddGivenDamamge( IN int iGivenDamage_ ) { m_iTotalGivenDamamge += iGivenDamage_; }
#endif DUNGEON_RANK_NEW
		//}} JHKang / 강정훈 / 2011/02/14 / 던전 랭크 개선 관련

		void	SetCurrentDamage( IN float fFinalDamage_ ) { m_fCurrentFinalDamage = fFinalDamage_; }
		float	GetCurrentDamage() { return m_fCurrentFinalDamage; }

#ifdef DUNGEON_DASH_LOG
		int		GetUsingDashCount( void ){ return m_iDashCount; }
#endif DUNGEON_DASH_LOG

#ifdef SERV_PVP_NEW_SYSTEM
		virtual wstring GetUnitName() 
		{
			return GetUnit()->GetNickName();
		}
#endif

#ifdef NEW_HENIR_TEST
		void StartHenirBuffFirst();
#ifndef ADD_HENIR_BUFF		/// 버프로 변경되면서 쓰이지 않게 된 구문
		void StartHenirBuffFire();
		void StartHenirBuffWater();
		void StartHenirBuffNature();
		void StartHenirBuffWind();
		void StartHenirBuffLight();
#endif ADD_HENIR_BUFF
		void StartHenirBuffDark();
		void EndHenirBuff();

		float GetHpUpRateBySkill();
		float GetMpUpRateBySkill();
#endif NEW_HENIR_TEST

#ifdef SPECIAL_USE_ITEM
		void StartSpecialItemBuffWind();
		void EndSpecialItemBuff();
		void StopSpecialItemBuffWind();
		void SetThrowItemValue( int iValue_ ){ m_iThrowItemValue = iValue_; }
#endif

#ifdef FACE_OFF_MONSTER_HEAD_TEST
		void SetHideNoRenderable( bool bVal ){ m_bHideNoRenderable = bVal; }
#endif FACE_OFF_MONSTER_HEAD_TEST

#ifdef ELSWORD_WAY_OF_SWORD
		virtual int			GetWayOfSwordState() const;
		virtual float		GetWayOfSwordPoint() const;
		virtual void		SetWayOfSwordPoint( float fWayOfSwordPoint_ );
		bool				IsActionActive( USER_ACTION_ID eUserActionID_ );
#endif ELSWORD_WAY_OF_SWORD
#ifdef ELSWORD_SHEATH_KNIGHT
//		void				AddComboToBrutalSlayer();
// 		float				GetBrutalSlayerDamageRel();
// 		float				GetBrutalSlayerManaBurnRate();
// 		float				GetBrutalSlayerManaBurnABS();
#endif ELSWORD_SHEATH_KNIGHT


#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		bool IsTransformed() { return m_bIsTransformed; }
#endif

#ifdef	SERV_TRAPPING_RANGER_TEST
		void				ProcessVPP();
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef EVE_ELECTRA
		bool				GetWhiteOut(){ return m_bWhiteOut; }
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	float GetCannonBallDamageUp() { return m_fCannonBallDamageUp; }
	float GetForceDownRelDamageRateBase() { return m_fForceDownRelDamageRateBase; }
#endif

#ifdef BUFF_TEMPLET_SYSTEM
	void UpdateBuffInfo( const KStat& kStat_, const vector<KBuffInfo>& vecSetBuffInfo_, const vector<int>& vecEraseBuffInfo_ );
	//버프 이펙트 제어 함수
	void UpdateBuffEffect( const vector<KBuffInfo>& vecSetBuffInfo_, const vector<int>& vecEraseBuffInfo_ );
	
	//10초당 전체 체력의 % 회복 소켓
	void SetHPChangeRatePerSecond( float fElapsedTime );
	//엠피 지속 회복 소켓
	void SetMPChangeRateValue( float fElapsedTime );
#endif BUFF_TEMPLET_SYSTEM

#ifdef PVP_ZOOM_CAMERA
	void SetSubZoomCameraTimer();
#endif

#ifdef SERV_ELSWORD_INFINITY_SWORD
	bool IsDashState()
	{
		switch( GetNowStateID() )
		{
		case USI_DASH:
		case USI_DASH_END:
		case USI_DASH_JUMP:
		case USI_DASH_JUMP_LANDING:
			return true;
		default:
			break;
		}

		return false;
	}	
#endif

#ifdef SUMMON_MONSTER_CARD_SYSTEM
	const SummonMonsterCardData& GetSummonMonsterCardData() const { return m_SummonMonsterCardData; }			///소환 몬스터용 카드 정보 반환
    SummonMonsterCardData& AccessSummonMonsterCardData() { return m_SummonMonsterCardData; }			///소환 몬스터용 카드 정보 반환
#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef FINALITY_SKILL_SYSTEM
	void ResetLinkOverChargeIllusion();
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_RENA_NIGHT_WATCHER
	void UpdateStartOfDelayedFiring( bool bInsertUnit = true );			/// 공격자가 발사한 지연의 신호탄 정보 갱신
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	float GetEDTBleedingTime();
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	int GetHyperModeCountUsed(){	return m_iHyperModeCountUsed;	}
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
	bool GetUseChangeWeapon() { return m_bUseChangeWeapon; }										/// 아이샤의 매지컬 메이크 업이나 청의 3각성을 통한 무기 변경 여부 반환
	void SetUseChangeWeapon( bool bUseChangeWeapon_ ) { m_bUseChangeWeapon = bUseChangeWeapon_; }	/// 아이샤의 매지컬 메이크 업이나 청의 3각성을 통한 무기 변경 여부 설정
#endif FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	int GetAllyEventMonsterUID() { return m_iAllyEventMonsterUID; }
	void SetAllyEventMonsterUID( int iAllyEventMonsterUID ) { m_iAllyEventMonsterUID = iAllyEventMonsterUID; }
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

		virtual void		InitEquippedActiveSkillState( bool bOnlySkillLevel = false );

		void				ChangeEquippedSkillState( const int iSkillSlotId_, const int iSkillId_ );
		void				ChangeEquippedSkillState( CX2Unit::UnitData& kUnitData_, const CX2SkillTree::SKILL_ID eSkillId_, const int iSlotIndex_, const bool bSlotB_ );

		// 바쁜 상태에서는 필드에서 캐시샵 입장, 장비교체, 던전/대전 도구 사용이 불가( true를 리턴하면 사용 불가)
		bool IsBusyStateNow() { return !( m_ElapsedTimeNotBusyState.CheckElapsedTime() ); }
		float GetRemainElapsedTimeNotBusyState() const {	return m_ElapsedTimeNotBusyState.GetTargetTime() - m_ElapsedTimeNotBusyState.GetSumOfElapsedTime();	}

		void				UpdateBuffInfo( const KStat& kStat_, const vector<KBuffInfo>& vecBuffInfo_ );

		virtual void ChargeMpAndEtcInTrainingGame();

		virtual void				InitAquiredActiveSkillState( const int iSkillID_ ) {}
		virtual void				InitPassiveSkillState();
		void						SendBuffInfoNot( const UidType uidGameUnitToSendNot_ );
		virtual bool				DidReceiveFirstSyncPacket() const { return m_bFirstDataReceive; }		/// 해당 게임유닛의 첫 싱크패킷을 받았는지 여부를 알아보는 함수


#ifdef UPGRADE_SKILL_SYSTEM_2013
#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
		virtual void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType_, const float fCoolTime = 0.f, bool bForceSet = true, BUFF_CHANGE_TYPE eChangeType = BCT_RELATION_VALUE, const float fCoolTimeRate = 1.f, const bool bIsRelativeAllSkill = false );
#else // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME
		virtual void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType_, const float fCoolTime = 0.f, bool bForceSet = true, BUFF_CHANGE_TYPE eChangeType = BCT_RELATION_VALUE );
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
#else // UPGRADE_SKILL_SYSTEM_2013
		virtual void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType_ );
#endif // UPGRADE_SKILL_SYSTEM_2013

		virtual CKTDGXSkinAnimPtr	GetSkinAnimPtr() const { return m_pXSkinAnim; }
		void DoGuildSkillChanceToReverse();

		/// 각 유닛마다 특정 버프가 실행 또는 해제 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 시전중이라는 플래그 등...)
		virtual void SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
		virtual void UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
		virtual void				AnimStop();
		virtual void				AnimPlay();
		
		virtual void SetStateToNowAnimation( const CKTDGXSkinAnim::XSKIN_ANIM_STATE eAnimState_ )
		{
			m_pXSkinAnim->SetState( eAnimState_ );
		}
		
		virtual float GetNowAnimationTime() const
		{
			return ( m_pXSkinAnim != NULL ? m_pXSkinAnim->GetNowAnimationTime() : -1.0f );
		}

		virtual void SetNowAnimationTime( const float fAnimCurrentTime_ )
		{
			if ( NULL != m_pXSkinAnim )
				m_pXSkinAnim->SetAnimationTime( fAnimCurrentTime_ );
		}

		inline	void UpdateUserAllStatus()
		{
			UpdateSocketDataAndEnchantData();
			UpdatePassiveAndActiveSkillState();
		}
		void SetEnterCashShop(bool bVal);

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		bool GetHasHyperModeItem() const { return m_bHasHyperModeItem; }
#endif //SERV_9TH_NEW_CHARACTER
		void SetHasHyperModeItem(bool bVal){m_bHasHyperModeItem = bVal;}

#ifdef MODIFY_SET_DAMAGE_DATA
		virtual	void				SetDamageExceptionProcess( CX2DamageManager::DamageData* pDamageData_, float& fPowerRate);
#endif //MODIFY_SET_DAMAGE_DATA

#ifdef NOT_CANCEL_BBT_STUN_BY_HIT		/// 버프를 통한 스턴 그로기 스테이트 전환 여부
		void SetChangeGroggyState( bool bChangeGroggyState_ ) { m_bChangeGroggyState = bChangeGroggyState_; }
#endif NOT_CANCEL_BBT_STUN_BY_HIT

		float GetNowTimeAnimFuture() const
		{
			if ( NULL != m_pXSkinAnimFuture )
				return m_pXSkinAnimFuture->GetNowAnimationTime();
			else
				return 0.0f;
		}

		bool IsAnimFutureEnd() const
		{
			if ( NULL != m_pXSkinAnimFuture )
				return m_pXSkinAnimFuture->IsAnimationEnd();
			else
				return false;
		}
		

		// 오현빈 // X2Game에서 UI에 출력 하기 위해 사용 할 수 있도록 public으로 이동.
		virtual float				GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;
		virtual float				GetActualCoolTime( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN int iSkillLevel ) const;

#ifdef NEXON_QA_CHEAT_REQ
		void SetInvincibleAndNoMpConsume_Cheat( bool bValue_ ) { m_bIsInvincibleAndNoMpConsume = bValue_; }
		bool GetInvincibleAndNoMpConsume_Cheat() const { return m_bIsInvincibleAndNoMpConsume; }
#endif //NEXON_QA_CHEAT_REQ


#ifdef EFFECT_TOOL
		void SetXSkinAnim_EffectTool( CKTDGXSkinAnim* pAnim_ ) { m_pXSkinAnim_EffectTool = pAnim_; }
#endif //EFFECT_TOOL

#ifdef FIX_SUMMONED_MONSTER_POSITION
		virtual void	CallSummonedNPC( CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );
#endif //FIX_SUMMONED_MONSTER_POSITION


#ifdef CHECK_CAMERA_INFO		
		bool GetIsLineMapCamera() const { return m_bIsLineMapCamera; }
#endif //CHECK_CAMERA_INFO

#ifdef  ADDED_RELATIONSHIP_SYSTEM
		UidType GetRelationshipUnitUID() const { return m_RelationshipTargetUID; }
		void SetRelationshipUnitUID(UidType val) { m_RelationshipTargetUID = val; }
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef NEW_CHARACTER_EL
		// (명중률 관리)
		float GetAddAccuaracyPercent() const { return min(m_fAddAccuaracyPercent, 1.f); }
		void SetAddAccuaracyPercent(float val) { m_fAddAccuaracyPercent = val; }

		// (마법 공격 크리티컬 발동 확률 )
		float GetAddMagicAttackCriticalRate() const { return min(m_fAddMagicAttackCriticalRate, 1.f); }
		void SetAddMagicAttackCriticalRate(float val) { m_fAddMagicAttackCriticalRate = val; }

		// (마법 공격 크리티컬 데미지 배율)
		float GetAddMagicAttackCriticalDamageRate() const { return min(m_fAddMagicAttackCriticalDamageRate, 1.f); }
		void SetAddMagicAttackCriticalDamageRate(float val) { m_fAddMagicAttackCriticalDamageRate = val; }
#endif // NEW_CHARACTER_EL
		const vector< int >& GetSetItemOptions(){ return m_vecSetItemOptions; }
		int GetMyUnitSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_ );
#ifdef FIX_FORCE_DOWN_AND_DETONATION_BUG
		void ForceDownAndDetonationProcess();
#endif //FIX_FORCE_DOWN_AND_DETONATION_BUG

#ifdef MODIFY_AFTER_IMAGE
		void UpdateAfterImageEquip() { if( NULL != m_pAfterImage ) m_pAfterImage->UpdateEquipView( m_pXSkinAnim.get() ); }
#endif // MODIFY_AFTER_IMAGE

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		/// 스킬 슬롯 스테이트 아이디에 해당하는 스테이트 아이디 반환
		virtual void GetStateIDBySecretSkillTriggerStateID( IN OUT int& iStateID_ ) const {};
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef HAMEL_SECRET_DUNGEON // 김태환
		void ApplyBuffByUseHyperMode();								/// 각성시 적용할 버프를 설정하는 함수
		void EraseDebuffAndAddMP( IN const float fMPValue_ );		///  (define ADJUST_SECRET_ITEM_OPTION) 디버프 삭제, 삭제 갯수에 관계없이 mp 회복으로 수정 <- 걸려있는 디버프를 지우고, 지운 개수만큼 MP 회복

#ifdef ADJUST_SECRET_ITEM_OPTION //김창한
		const float GetSocketOptionHyperCoolTime(){ return m_fSocketOptionHyperCoolTime; }
#endif //ADJUST_SECRET_ITEM_OPTION

#endif // HAMEL_SECRET_DUNGEON

		virtual void		ApplyWorldLightColor( OUT CKTDGXRenderer::RenderParam* pRenderParam_ );

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
		virtual const int GetHypetMpdeStateID() { return static_cast<int>( USI_HYPER_MODE ); }
#endif //SKILL_CANCEL_BY_HYPER_MODE

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		virtual void		UpNowDPValueByDamage( IN const float fDamage_ ) {}				/// 애드일 때, 피격시 DP 변동 처리
		virtual const float	GetDPValue( IN const float fDPValue_ ) { return 0.f; }			/// 애드일 때, DP 설정
		virtual void		SetDPValue( IN const float fDPValue_ ) {}						/// 애드일 때, DP 설정
		virtual const bool	GetIsFormationMode() const { return false; }					/// 구성 모드 적용 여부 반환 함수
		virtual void		SetIsFormationMode( IN const bool bIsFormationMode_ ) {}		/// 구성 모드 적용 여부 설정 함수

		virtual void		SetStasisfieldHitValue( IN const float fHitValue_ ) {}			/// 스테이시스 필드 타격치 설정 함수
		virtual void		SetStasisfieldDamageValue( IN const float fDamageValue_ ) {}	/// 스테이시스 필드 피격치 설정 함수

		virtual const bool	GetUpDPByAttackThisFrame() { return false; }					/// 타격시 한 프레임에 DP 획득된 여부 Flag 반환
		virtual void		SetUpDPByAttackThisFrame( IN const bool bval_ ) {}				/// 타격시 한 프레임에 DP 획득된 여부 Flag 설정
		virtual const bool	GetUpDPByDamageThisFrame() { return false; }					/// 타격시 한 프레임에 DP 획득된 여부 Flag 반환
		virtual void		SetUpDPByDamageThisFrame( IN const bool bval_ ) {}				/// 타격시 한 프레임에 DP 획득된 여부 Flag 설정

		/// 특정 상태일 때, 애니메이션을 바꾸는 함수
		virtual void		SetCustomAnimName( IN const UserUnitStateData& UserStateData, IN OUT wstring& wstrAnimName ) {}

		/// 스킬 아이디에 해당하는 SA값을 받아오는 함수
		const float			GetSpecialAbilityValue( IN const CX2SkillTree::SKILL_ABILITY_TYPE eSAValue, IN const CX2SkillTree::SKILL_ID eSkillID ) const;

		/// MP 없을 때 나가는 만두 설정 ( 원래 각 캐릭터 별로 선언 되어 있었으나, 다 같은 함수를 쓰고 있어서 X2GUUser로 이동 )
		void				CreateNotEnoughMPEffect( IN const D3DXVECTOR3 vPos, IN const float fDegreeX, IN const float fDegreeY, IN const float fDegreeZ );

		/// 공격에 필요한 MP가 충분하면 true, 충분하지 않으면 false와 함께 만두 이펙트 발사
		bool				IsEnoughMPForAttack( IN const float fConsumeMP, IN const D3DXVECTOR3 vPos, IN const float fOffsetPos = 0.f, 
			IN const D3DXVECTOR3 vRotDegree = D3DXVECTOR3( 0.f, 0.f, 0.f ) );

		const bool			CheckElapsedTimeRidingWaitHabit() { return m_ElapsedTimeRidingWaitHabit.CheckElapsedTime(); }
		const int			GetAirDashCount(){ return m_iAirDashCount; }

#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		virtual void		SetMutationCount( IN const int iMutationCount_ ) {}						/// 애드일 때, 변이 포인트 설정
		virtual void		SetReverseReactorTransitionDamage( IN const float fFinalDamage_ ) {}	/// 최종 데미지 값 반환 및 설정 함수
		const int			GetSkillLevelValue( IN const CX2SkillTree::SKILL_ID eSkillID_ );		/// 스킬 아이디에 해당하는 스킬 레벨을 받아오는 함수
		void				SetEffectSoundParticle( IN const D3DXVECTOR3& vPos_, IN const wstring& wstrEffectName_ );		/// 효과음 이펙트 출력 함수
#endif //SERV_ADD_LUNATIC_PSYKER

		//{{ Public Function End

#ifdef  SUPER_ARMOR_TIME
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        const std::vector<D3DXVECTOR2>&   GetVecNowSuperArmorTime() const { return m_vecNowSuperArmorTime; }
        std::vector<D3DXVECTOR2>&   AccessVecNowSuperArmorTime() { return m_vecNowSuperArmorTime; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#endif  SUPER_ARMOR_TIME

#ifdef CHEAT_SELF_DAMAGE // 김태환
		void SetSelfDamage( IN int iValue_, IN int iFrontDamage_ );					/// 자신에게 데미지를 주는 기능
#endif //CHEAT_SELF_DAMAGE

#ifdef FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE // 김태환
		bool ProcessSkillAtDie( IN const float fFinalDamage_, IN CX2DamageManager::DamageData* pDamageData_ = NULL );	/// 사망 직전에 발동되는 스킬 검사
#endif //FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE

#ifdef FIX_SKILL_SLOT_CHANGE_BUG
		bool IsValideSlotChange() const { return ( m_fSlotChangeLeftCoolTime > 0.f ) ? false : true; } 
		void SetSlotChangeCoolTime(float val) { m_fSlotChangeLeftCoolTime = val; }
#endif // FIX_SKILL_SLOT_CHANGE_BUG

#ifdef FIX_SHOOTING_MEGABALL_AT_JUMP_LINE_MAP
		virtual void InitInpuDataProcess();	
#endif // FIX_SHOOTING_MEGABALL_AT_JUMP_LINE_MAP

#ifdef SERV_9TH_NEW_CHARACTER
		void InitPhaseShift();
#endif //SERV_9TH_NEW_CHARACTER

#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
		virtual		void		AddManeuverGauge(float fVal) {}				// 이브 기동 게이지 증가 가상 함수
		virtual		bool		IsWaitManeuverCore () { return false; }	// 이브 기동 코어가 공격 중인 상태인가?	
		virtual		void		SetAttackManeuverCore ( CX2DamageManager::DamageData & pDamageData ) {}		//이브 기동 코어를 공격 상태로 변환한다.
		virtual		bool		IsSkillSummonedMonster ( CX2UnitManager::NPC_UNIT_ID eUnitID ) { return false; }	// 스킬로 소환된 몬스터인가? 각 캐릭터 별로 설정
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어


	protected:
		//{{ Protected Function Start
		virtual void				Verify();
		virtual void				InitDevice();
		virtual void				InitSystem();
		virtual void				InitStat();
		virtual void				InitComponent();

		virtual void ParseCommonRandomState() = NULL;
		void ParseCommonRandomState( const char* tableName );


		
		///////////////////////////////////////////////////////////////
		virtual void				InitMotion();
		virtual	void				InitSocketData();

		bool						CanApplySocketDataByGameType( const CX2SocketItem::SocketData& kSocketData_ );
		//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
		void						SumSocketOption( const vector<int>& vecSocketOption, const float fAtkMagic, const int iSocketLevel_ );
		//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

		virtual void				InitSetItemOptions();
		virtual void				InitEnchantData();
		virtual void				InitEqip();
		virtual void				InitState();


		void InitStateCommonRandom( OUT std::string &tableNameStartUTF8, OUT std::string &tableNameWinUTF8, OUT std::string &tableNameLoseUTF8 );
		
		void						ProcessAcceleraterBuff( CX2SkillTree::SKILL_ID eSkillID, bool bHyperMode );	/// 엑셀레이터류 버프 시전
		virtual	float				GetAdditionalAccelBuffTime( const CX2SkillTree::SKILL_ID eSkillID_ ) const { return 0.0f; }	/// 엑셀레이터류 버프 추가시간
		void						ApplyAcceleratorBuffToGameUnit( const CX2SkillTree::SkillTemplet* pSkillTemplet_, const float fAddtionalAccelBuffTime_, const D3DXVECTOR3& vMyPos_ );
		virtual void				ApplyAuraAcceleratorBuffToGameUnit( const CX2SkillTree::SkillTemplet* pSkillTemplet_, const float fAddtionalAccelBuffTime_, const D3DXVECTOR3& vMyPos_ );
		virtual bool				CanApplyBuffToGameUnit() const;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        virtual const std::vector<BUFF_TEMPLET_ID>& GetVecImmunityAtThisState() const   { return m_vecImmunityAtThisState; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


		virtual void				SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ ) = NULL;
		void						SetEquippedSkillFunc( const CX2Unit::UnitData& kUnitData_, const bool bSlotB_ );
		virtual void				SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ ) = NULL;

		virtual void				SetEquippedSkillLevelCommon( const CX2Unit::UnitData& kUnitData_, bool bSlotB_ );
		const CX2GUUser::UserUnitStateData&			GetPressedSkillState( const int iPressedIndex_ );

		void						CalcEquippedSkillLevelPlusAndMPCostBySlotID( CX2UserSkillTree& cUserSkillTree_, const int iSlotIndex_, const bool bSlotB_ );
		void						CalcEquippedSkillLevelPlusAndMPCost( CX2Unit::UnitData& kUnitData );

		void						DoStateEndStartFuture();
		void						DoStateEndStart();
		void						DoFrameMoveFuture();
		void						DoFrameMove();

		virtual	void				DyingStart();
		virtual void				CommonStateStartFuture();
		virtual void				CommonStateStart();
		virtual void				CommonFrameMoveFuture();		

		void CommonFrameMoveFuture_ExtraDamage();
		void CommonFrameMoveFuture_TimedEvent();

		virtual void				CommonFrameMove();


//#ifdef HEAD_INVERSE_KINEMATICS_TEST
//		void HeadIKFrameMove( double fTime, float fElapsedTime );
//#endif HEAD_INVERSE_KINEMATICS_TEST


		void CommonFrameMove_Invincible();
		void CommonFrameMove_Particle();
		void CommonFrameMove_TimedEvent();

#ifdef CHUNG_SECOND_CLASS_CHANGE
		//void CommonFrameMove_ChainLockOn();
		//void SetLockOnCount( WCHAR* pwstrName_, float fPowerRate_, D3DXVECTOR3 vPos_, int iCount_ = 0 );
#endif

		//void PlayLuaSubCamera( int iCameraTableIndex );
		virtual void				CommonCameraMove();
		virtual void				CommonEventProcess();
		virtual void				CommonStateEndFuture();
		virtual void				CommonStateEnd();
		virtual void				DetonationEventProcess();

//{{ robobeg : 2008-10-28
		//virtual void				CommonRender();
        virtual RENDER_HINT			CommonRender_Prepare();	//optimization
//}} robobeg : 2008-10-28
		virtual void				CreateStepDust();	// kimhc // 2010-11-19 // 라인맵의 eDustType에 따라 보여줄 파티클을 플레이함(각 캐릭터에 있는 중복 코드를 옮겨옴)

#ifdef GRAPPLING_TEST
		virtual void				GrappledEventProcess();
		virtual void				GrappledPhysicProcess( bool bFuture );
#endif GRAPPLING_TEST

		virtual void				PhysicProcess();


		//void PhysicProcess_FastUpwardWind( CKTDGLineMap* pLineMap );
		void PhysicProcess_Jumping( CKTDGLineMap* pLineMap );
		void PhysicProcess_Bungee( CKTDGLineMap* pLineMap );
		void PhysicProcess_Portal( CKTDGLineMap* pLineMap );
		void PhysicProcess_PortalInBattleField( CKTDGLineMap* pLineMap );

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
        bool    ProcessCanPushUnit( CX2GameUnit* pGameUnit_, const D3DXVECTOR2& vMyUnitMinMaxY_, OUT D3DXVECTOR3& vMyPos_, int iDirection );
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//        bool    ProcessCanPushUserUnit( CX2GUUser* pGameUnit_, const D3DXVECTOR2& vMyUnitMinMaxY_, const D3DXVECTOR2& vMyUnitMinMaxYCur_, OUT D3DXVECTOR3& vMyPos_ );
//		bool    ProcessCanPushNPCUnit( CX2GUNPC* pGameUnit_, const D3DXVECTOR2& vMyUnitMinMaxY_, OUT D3DXVECTOR3& vMyPos_, int iDirection );
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
		bool    CheckPassUnit( const CX2GameUnit* pGameUnit_, OUT D3DXVECTOR3& vPos_ );
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		FORCEINLINE void	ProcessCanPushUnit( const CX2GameUnit* pGameUnit_, const D3DXVECTOR2& vMyUnitMinMaxY_, OUT D3DXVECTOR3& vPos_ );
		FORCEINLINE void	ProcessCanPassUnit( const CX2GameUnit* pGameUnit_, OUT D3DXVECTOR3& vPos_ );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef LINKED_LINEMAP_JUMP_USER_TEST
		void LinkJumpEventProcess();
		void LinkJumpPhysicProcess( double fTime, float fElapsedTime );
		bool CanLinkJumpNow( float fElapsedTimeLastLinkJump = 0.f );
		void LinkJumpStart();
		void LinkJumpEnd();
#endif LINKED_LINEMAP_JUMP_USER_TEST


		bool CommonSpecialAttackEventProcess( IN const CX2UserSkillTree& cUserSkillTree_, OUT const CX2UserSkillTree::SkillSlotData* & pSkillSlotDataPressed_, OUT int& iSkillSlotIndexPressed_ );

		virtual bool				SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND ) = NULL;

		virtual void				NoStateChangeActionFrameMove() = NULL;		
		virtual bool				SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet ) = NULL;

#ifdef ACTUAL_CB_CONSUME
		virtual	USHORT				GetActualCBConsume( CX2SkillTree::SKILL_ID eSkill_ID_, int iSkillLevel_ );
#endif //ACTUAL_CB_CONSUME

#ifdef ADD_MEMO_1ST_CLASS //김창한
		bool CheckSkillUseCondition( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition, CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseConditionSkillTemplet );
#else //ADD_MEMO_1ST_CLASS
		bool CheckSkillUseCondition( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition, const CX2SkillTree::SkillTemplet* pSkillTemplet );
#endif //ADD_MEMO_1ST_CLASS

		bool CheckSkillUsePVPOfficial( CX2SkillTree::SKILL_ID eSkill_ID_ );

#ifdef FIX_NO_STATE_SKILL_BUG
		virtual bool IsValideSkillRidingOn( CX2SkillTree::SKILL_ID eSkill_ID_ ){ return true;}
#endif // FIX_NO_STATE_SKILL_BUG

		virtual	bool				IsSuperArmor() const;

		bool						EventTimer( float fTime, bool bFuture );
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool						EventCheck( float fTime, bool bFuture );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		
//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
        void						PushFrameData( bool bSendForce );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool                        PopFrameData( bool bPopOnce, OUT bool& bFrameBufferPass );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool                        PopFrameData( bool bPopOnce );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//		void						PushFrameData();
//		void						PopFrameData();
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

	
		//virtual float				GetPowerRate(); // // moved by wonpok. 20091215. to public.

		//optimization
		void						ActiveSkillShow( const WCHAR* pBoneName, const SkillCutInSet *s_SkillCutInSet, int iCutInSet, int iCutIn, bool bHyper, bool bOnlyLight = false);
		void						ActiveSkillShow( const WCHAR* pBoneName, const WCHAR* pBackFaceTexName, D3DXVECTOR2 size, D3DXVECTOR3 pos, bool bHyper, bool bOnlyLight = false );
		void						ActiveSkillShow( D3DXVECTOR3 bonePos, const WCHAR* pBackFaceTexName, D3DXVECTOR2 size, D3DXVECTOR3 pos, bool bHyper, bool bOnlyLight = false );

		bool						CheckDashAngleChangeTime();
		float						GetDashCameraAngleDegree();

		virtual float				GetBasicReducedDamagePercent();


		virtual float				GetReducedDamagePercentByMonsterAttack( int monsterID );
		
		virtual void				HyperModeBuffEffectStart();
		virtual void				HyperModeBuffEffectEnd();
		
		virtual float 				GetDamageUpByAMADS();


		//인챈트 특수 데미지는 따로 계산함 여기서 하지 않음!!
		virtual void				GetDamageReduceByResistExtraDamage( CX2DamageManager::EXTRA_DAMAGE_TYPE damageType, float& fFinalDamage );
	

		



		virtual bool				IsImmuneBySocketOption( CX2DamageManager::EXTRA_DAMAGE_TYPE exDamageType );

		void ResetAnimSpeed( float fAnimSpeed, bool bSpeedUpByItem, bool bFuture, float fExtraSpeedRate = 1.f );

#ifdef UNIT_EMOTION
		void						EmotionFrameMove();
#endif

		void DamageDoubleAttack();



		virtual void CommonFrameMoveStateAbnormalityFrozen();

#ifdef EDT_WATER_HOLD_TEST
		virtual void CommonFrameMoveStateAbnormalityWaterHold();
#endif EDT_WATER_HOLD_TEST

#ifdef EVE_ELECTRA
		virtual void CommonFrameMoveStateAbnormalityFlashBang();
#endif EVE_ELECTRA

#ifdef GRAPPLING_TEST
		virtual void CalcGrapplingPosition( bool bFuture )
		{
			if( true == bFuture )
				m_FrameDataFuture.unitCondition.m_GrapplingState.CalcGrapplingPosition();
			else
				m_FrameDataNow.unitCondition.m_GrapplingState.CalcGrapplingPosition();
		}

		virtual void ResetGrapplingState( bool bOnlyMe = false )
		{
			if( false == bOnlyMe )
			{
				if( true == IsGrappled( false ) )
				{
					SetPositionOnLine( GetPos(), GetLastTouchLineIndex() );
				}
			}

			m_FrameDataFuture.unitCondition.m_GrapplingState.ResetGrapplingState( bOnlyMe );
			m_FrameDataNow.unitCondition.m_GrapplingState.ResetGrapplingState( bOnlyMe );
		}

		virtual void SetUnitGrapplingMe( bool bFuture, GAME_UNIT_TYPE eGameUnitType, UidType unitUID )
		{
			if( true == bFuture )
				m_FrameDataFuture.unitCondition.m_GrapplingState.SetUnitGrapplingMe( eGameUnitType, unitUID );
			else
				m_FrameDataNow.unitCondition.m_GrapplingState.SetUnitGrapplingMe( eGameUnitType, unitUID );
		}

		virtual void SetUnitGrappledByMe( bool bFuture, GAME_UNIT_TYPE eGameUnitType, UidType unitUID )
		{
			if( true == bFuture )
				m_FrameDataFuture.unitCondition.m_GrapplingState.SetUnitGrappledByMe( eGameUnitType, unitUID );
			else
				m_FrameDataNow.unitCondition.m_GrapplingState.SetUnitGrappledByMe( eGameUnitType, unitUID );
		}


		virtual CX2GameUnit* GetUnitGrapplingMe( bool bFuture )
		{
			KTDXPROFILE();

			if( true == bFuture )
				return m_FrameDataFuture.unitCondition.m_GrapplingState.GetUnitGrapplingMe();
			else
				return m_FrameDataNow.unitCondition.m_GrapplingState.GetUnitGrapplingMe();
		}

		virtual CX2GameUnit* GetUnitGrappledByMe( bool bFuture )
		{
			KTDXPROFILE();

			if( true == bFuture )
				return m_FrameDataFuture.unitCondition.m_GrapplingState.GetUnitGrappledByMe();
			else
				return m_FrameDataNow.unitCondition.m_GrapplingState.GetUnitGrappledByMe();
		}

		virtual bool IsGrappled( bool bFuture )
		{
			KTDXPROFILE();

			if( true == bFuture )
				return m_FrameDataFuture.unitCondition.m_GrapplingState.m_UnitUIDGrapplingMe > 0; 
			else
				return m_FrameDataNow.unitCondition.m_GrapplingState.m_UnitUIDGrapplingMe > 0; 
		}


#endif GRAPPLING_TEST



#ifdef RENA_SIEGE_MODE_ANGLE_TEST
		void LookAtIKFrameMove( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, const D3DXVECTOR3& vRotationAxis, const float fRotationAngle );
#endif RENA_SIEGE_MODE_ANGLE_TEST

		float DoAimingTarget(float fDist, const D3DXVECTOR3& vCoefficient, bool &bIsTarget, D3DXVECTOR3 &vTargetPos, D3DXVECTOR3 &vLandPos, bool bGap = true );
		void RotateBone( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, const D3DXVECTOR3& vRotationAxis, const float fRotationAngle );

#ifdef ELSWORD_SECOND_CLASS_CHANGE
		void SetUnitReForm();
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
		void ResetPassiveCriticalRate() { m_fPassiveCriticalRate = 0.f; }
		void SetPassiveCriticalRate(float fVal) { m_fPassiveCriticalRate = fVal; }
		float GetPassiveCriticalRate() { return m_fPassiveCriticalRate; }
#endif

//{{ kimhc // 2010.4.1 // 비밀던전 작업(셋트아이템효과)
#ifdef SERV_SECRET_HELL
		//{{ kimhc // 2010.6.14 //	드래고닉셋트 오류 수정위해 추가
		float GetOriginalMPChangeRate();
		void SetOriginalMPChangeRate(float val) { m_fOriginalMPChangeRate = val; }
		//}} kimhc // 2010.6.14 //	드래고닉셋트 오류 수정위해 추가

		float	GetNowMPChange();			// 현재 자신의 MP 증가량을 얻어옴 // 단! 놀방인 경우 증가량을 나눈 값을 전달해줌 (놀방인경우 원래 MP증가량에 * 7.0을 해주는 처리가 있음)
		bool	InitSocketEffectEx( const CX2SocketItem::SocketData& kSocketData );		// SOCKET_ITEM_EFFECT_EX 값이 있으면 처리
		CX2SocketItem::SOCKET_ITEM_EFFECT_EX ProcessSocketEffectEx( const CX2SocketItem::SOCKET_ITEM_EFFECT_EX eEx_, CX2GameUnit* pGameUnit_ );		// 해당 셋트효과가 발생될지 안될지를 결정함, 리턴값은 발생여부를 판단함, 발생이 안되면 NONE를 리턴

		float	GetManaRecoveryVal() const { return m_fManaRecoveryVal; }
		float	GetManaRecoveryTime() const { return m_fManaRecoveryDurationTime; }
		void	CheckNChangeManaRecoveryValueNTime( float fElpasedTime );		// 자연마나회복량을 늘려주는 시간을 체크한 후에 time이 0이되면 valu를 0.0f로 변경 시켜주고 자연 마나회복량 역시 원래되로 돌려줌

		void	SetManaRecoveryVal( float fVal ) { m_fManaRecoveryVal = fVal; }
		void	SetManaRecoveryTime( float fTime ) { m_fManaRecoveryDurationTime = fTime; }

		void	SetIntervalTimeForCureDebuff( float fVal ) { m_fIntervalTimeForCureDebuff = fVal; }
		float	GetIntervalTimeForCureDebuff() const { return m_fIntervalTimeForCureDebuff; }
		void	CheckNChangeIntervalTimeForCureDebuff( float fElpasedTime );

		
		void	CreateBoneShield( const CX2SocketItem::SocketData& kSocketData );
		void	DeleteBoneShield() { SAFE_DELETE( m_pBoneShieldData ); }
		void	CheckAndDeleteBoneShield( float fElpasedTime );

		// kimhc // 2010-11-01 GameUnit으로 옮김
		/*static	bool GreaterExtraDamageTime( CX2DamageManager::ExtraDamageData* pFirst, CX2DamageManager::ExtraDamageData *pSecond );*/

#ifdef ADJUST_SECRET_ITEM_OPTION //김창한
		const float GetSocketOptionCoolTime(){ return m_fSocketOptionCoolTime; }
		void SetSocketOptionCoolTime( const float fVal_ ){ m_fSocketOptionCoolTime = fVal_; }
		void CheckSocketOptionCoolTime( const float fElpasedTime_ );

		void SetSocketOptionHyperCoolTime( const float fVal_ ){ m_fSocketOptionHyperCoolTime = fVal_; }
		void CheckSocketOptionHyperCoolTime( const float fElpasedTime_ );

		void CheckSocketMpUpAttackedCoolTime( const float fElpasedTime_ );

		const float GetSocketReducedDamagePercentInCase( const CX2SocketItem::SocketData& kSocketData_ );
#endif //ADJUST_SECRET_ITEM_OPTION

#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.1 // 비밀던전 작업(셋트아이템효과)

		void				RenderRank(int iFinalLeft, int iFinalTop);
#ifdef SERV_INTEGRATION
		//{{ oasis907 : 김상윤 [2010.5.17] // 
		void				RenderServer(int iFinalLeft, int iFinalTop);
		//}}
#endif SERV_INTEGRATION

		//{{ kimhc // 2010.6.16 
		// 1. GameUnit::Weapon::SetEnchantParticleShow() 과 GUUser::SetShowEnchantedWeaponEffectAtHand()을
		// 같이 쓰는 경우가 많으므로 한데 묶음
		// 2. NotifyShowObjectChanged()에서는 서로 따로 호출하도록 되어있는것을 한곳에서만 호출하도록 수정
		// 3. virtual로 만들어서 GameUnit에서는 GameUnit::Weapon::SetEnchantParticleShow()를 호출하고
		// GUUser에서는 SetShowEnchantedWeaponEffectAtHand()를 호출하도록 변경
#ifdef	ENCHANT_BUG_TEST
		virtual void SetShowEnchantWeaponParticle( bool bShow );
#endif	ENCHANT_BUG_TEST
		//}} kimhc // 2010.6.16 

#ifdef SERV_PET_SYSTEM
		void CommonStartStateEnd();	
		void WinStateStart();
		void LoseStateStart();
#endif

		//{{ kimhc // 2010.7.12 // 여러명의 유저를 HOLD 할 수 있도록 함
		// HOLD 상태일때 변경될 스테이트 지정 및 얻어오기( 기본은 m_DamageAirFall )임
		int GetHoldStateID() const { return m_iHoldStateID; }
		void SetHoldStateID( int val ) { m_iHoldStateID = val; }
		void SetHoldStateByReactType( CX2DamageManager::REACT_TYPE eReactType = CX2DamageManager::RT_DOWN );
		//}} kimhc // 2010.7.12 // 여러명의 유저를 HOLD 할 수 있도록 함

		void PlayCommonBuffMinorParticle();	// CommonBuffFrame에서 쓰이는 파티클 출력
		bool GetSkillSlotIndexUsed( OUT int& iSkillSlotIndex_, OUT bool& bSlotB_ );	// 사용된 슬롯의 인덱스 및 B슬롯 사용여부를 리턴

//{{ kimhc // 2010.12.6 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
#ifdef ADD_MEMO_1ST_CLASS
		bool SetDamageData( const char* pszTableNameUTF8_, const bool IsEqippedMemo_ = false );
#else //ADD_MEMO_1ST_CLASS
		void SetDamageData( const char* pszTableNameUTF8_ );
#endif //ADD_MEMO_1ST_CLASS

		void CreateWeapon( 
            IN const CX2Item::ItemTemplet* pItemTemplet_, 
            CX2EqipPtr pEquipWeaponPtr_ );

	#ifdef	REAL_TIME_ELSWORD
		bool GetAddItemAndRemoveItemToViewList( OUT CX2EqipPtr& pEquipPtrToRemove_, IN OUT CX2Item** ppItemToAddInViewList_, IN CX2Item* const pItemToRemoveInViewList_, OUT int& iIndexInViewListForRemove_ );
	#endif	REAL_TIME_ELSWORD

#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.6 //  2010-12-23 New Character CHUNG

		void AddDefenceEnchantData( CX2EnchantItem::ENCHANT_TYPE eEnchantType, int iEnchantValue );
		void AddExtraDamageImmuneData( CX2DamageManager::EXTRA_DAMAGE_TYPE eExtraDamageType, float fImmuneValue );
//{{ kimhc // 2011.1.14 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
		void AddEnchantStatFromPassiveSkillToDefenceEnchantData();
		void AddExtraDamageImmuneRateFromPassiveSkillToImmuneData();
		const float GetEnchantStatFromPassiveSkillByEnchantType( int iEnchantType );	// 인첸트 타입별 수치 값 리턴

		float GetAddAnimSpeedFactor() const { return m_fAddAnimSpeedFactor; }	// 청 1차 전직, 슈팅 가디언의 난전의 기술 스킬에 의한 애니메이션 속도 증가
		void SetAddAnimSpeedFactor(float fAddAnimSpeedFactor_) { m_fAddAnimSpeedFactor = fAddAnimSpeedFactor_; }
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.14 // 청 1차 전직

		//{{ kimhc // 2011.4.24 // 월드버프 2단계, 드래곤의 숨결
#ifdef	SERV_INSERT_GLOBAL_SERVER
		void AddEnchantStatFromDragonBreathToDefenceEnchantData();	
#endif	SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011.4.24 // 월드버프 2단계, 드래곤의 숨결

#ifdef SUPER_SOCKET_IN_TITLE
		void SetSuperArmorSocket();
#endif
		

		//{{ mauntain : 김태환 [2012.07.26] 클로킹 메모 쓸쓸한 기다림 버그 수정 ( 작업자 : 엘소드 대만 이지헌 )
#ifdef EVE_MEMO5_BUG_FIX
		void SetAdditionalMPChangeRateByPassive(float val) { m_fAdditionalMPChangeRateByPassive = val; }
#endif EVE_MEMO5_BUG_FIX

		bool GetShowCutInAndChangeWorldColor();
		void ChangeWorldColorByHyperMode();
		bool GetShowActiveSkillShow() const;
		
		void ResetNotBusyTimerOnSpecificState();	// 바쁜 상태 타이머를 리셋
		virtual bool ShouldResetNotBusyTimer();	// 바쁜 상태 타이머가 리셋 되야 하는 상황 인지 판단
		
		virtual void SetCanPassUnit( const bool bFlag_ )
		{
			m_FrameDataNow.stateParam.bCanPassUnit = bFlag_;
			m_FrameDataFuture.stateParam.bCanPassUnit = bFlag_;
		}

#ifdef REALTIME_SCRIPT_PATCH
		void SetScriptFileName();
#endif //REALTIME_SCRIPT_PATCH

#ifdef ARA_CHANGE_CLASS_FIRST
		/// 오현빈 // 2013-01-10 // 회피 가능 여부 ( 현재는 소선의 이화접목으로만 회피가 가능합니다. )
		void SetCanAvoidance(bool bCanAvoidance_ ) { m_bCanAvoidance = bCanAvoidance_; }
		bool GetCanAvoidance() const { return m_bCanAvoidance; }
#endif //ARA_CHANGE_CLASS_FIRST

		void StopTime_StateStart();

#ifdef SERV_SKILL_USE_SUBQUEST
		void Send_SKILL_USE_REQ( CX2SkillTree::SKILL_ID eSkillID );
#endif SERV_SKILL_USE_SUBQUEST

		virtual void DamageDataChangeProcess();

#ifdef INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

#ifdef ADD_MEMO_1ST_CLASS
		void ParsingBuffFactorID( KLuaManager& luaManager_, const bool IsEqippedMemo_ = false );
#else //ADD_MEMO_1ST_CLASS
		void ParsingBuffFactorID( KLuaManager& luaManager_ );
#endif // ADD_MEMO_1ST_CLASS

			void CommonFrameMove_InsertBuffFactor();
#endif // INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
		
		virtual void ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ ){}
		//}} Protected Function End

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
		virtual void ChangeDamageData () {}
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef BALANCE_PATCH_20131107
		//각 유닛마다 Enchant가 적용될 때 셋팅해야 하는 함수 실행. NPC에는 필요없다 생각하여 GUUser에 작성.
		virtual void				SetSpecificValueByEnchant(){}
#endif //BALANCE_PATCH_20131107

#ifdef MODIFY_NO_STATE_SKILL_DURING_DAMAGE_REACT
		bool IsAvaliableActionDuringDamageReact( const UCHAR ucNowAction_ ) const; // 피격 중 사용가능한 Action
		void GetActionSyncData( OUT map< int, UCHAR >& mapActionSyncData_, IN const vector<SyncData>& ReceiveSyncDataList_ ) const;
		void SetActionSyncData( OUT vector<SyncData>& ReceiveSyncDataList_, IN const map< int, UCHAR >& mapActionSyncData_ ) const;
#endif // MODIFY_NO_STATE_SKILL_DURING_DAMAGE_REACT



#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        void    ClearAnimEventTimerOneshotNow();
        void    ClearAnimEventTimerOneshotFuture();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
		bool	IsPlayAvatarEmotionSoundWithouEmotion ( CX2Unit::EMOTION_TYPE eEmotionId );
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
	protected:
		//{{ Protected Variable Start
		UserUnitStateData								m_FutureStateData;	/// Future의 StateData
		UserUnitStateData								m_NowStateData;	/// Now의 StateData
		UserUnitStateDataMap							m_StateList;	///Key: StateID, Value: UserUnitStateData
		
		int												m_MachineGunWaitState;	/// 이거 안쓰면 그냥 삭제 하는게... 엘소드만 있고 다른건 없군...
		int												m_ChargeMpState;	/// MP를 충전하는 스테이트... 레나와 아이샤, 청은 실제 스테이트가.. 나머지는 그냥 Wait이 들어있음

		CKTDXTimer	m_TimerPepperRun;		/// 청양고추..? 최근에 다시 사용...optimization
		float		m_fPepperRunTime;		/// 청양고추 지속 시간



#ifdef SERV_APRIL_FOOLS_DAY		//만우절 이벤트 

		bool m_bIsFoolsDay; //해당 날짜에만 스킬 컷신 바꾸기 위해 날짜 체크 변수 추가

#endif //APRIL_FOOLS_DAY


//#ifdef PVP_BOSS_COMBAT_TEST
//		
//		int			m_FrozenState;
//		
//#endif PVP_BOSS_COMBAT_TEST



//{{ robobeg : 2008-10-28		
		//wstring											m_StateTableName;
		//wstring											m_StateTableNameFuture;
//}} robobeg : 2008-10-28		

		//optimization
		//FrameData										m_FrameDataFutureBefore;	/// 사용안함
		FrameData										m_FrameDataFuture;	
		FrameData										m_FrameDataNowBefore;	/// syncData의 IsRight만 사용
		FrameData										m_FrameDataNow;
		FrameData										m_FrameDataDead;		/// 단지,landPostion, dirDegree, lastTouchLineIndex만 필요한 거라면 그것만 가지고 가자.. 캐릭터가 죽어서 사라지기 바로 직전의 프레임 데이타
		int	m_DeadStageIndex;		/// 사용안함, 캐릭터가 죽어서 사라지기 바로 직전의 스테이지 번호
		int m_DeadSubStageIndex;	/// 사용안함, 캐릭터가 죽어서 사라지기 바로 직전의 스테이지 번호	

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef  SUPER_ARMOR_TIME
        std::vector<D3DXVECTOR2>                        m_vecNowSuperArmorTime;
#endif  SUPER_ARMOR_TIME
#ifdef SKILL_BALANCE_PATCH
		bool	m_bFutureCanPassUnit;		// 다른 유닛이 패스 할 수 있느냐 인것 같은데.. 유저의 CommonStateStartFuture에 루아 파싱 부분을 넣으면 이 부분을 삭제해도 될 듯..
#endif
        bool    m_bFutureApplyAnimMove;
        bool    m_bNowFallDown;
		bool											m_bDisableGravity;				/// 도대체 이게 왜... 여기 있는지... User만 사용 함...코드내에서 중력 영향 무시하도록 할 때 사용합니다
		bool											m_bDisableGravityInScript;		/// 캐릭터, 몬스터 스크립트에서 중력 영향 무시하도록 할 때 사용합니다 라고 하지만 이것도 User만 사용 하는군...
		D3DXVECTOR2										m_vDisableGravityInScriptTime;	/// 중력 무시 시간 (X는 시작시간, Y는 종료시간)
#ifdef  AISHA_SECOND_CLASS_CHANGE
		D3DXVECTOR2										m_vFutureIgnoreLineTime;				/// 이 애니메이션 시간 동안은 떨어지는 중에도 라인 체크를 무시한다. 즉, 선위에 밟고 서지 않는다. (헬드롭하고, 아틸러리가 사용하는 듯) NPC만 사용 중이네..
#endif  AISHA_SECOND_CLASS_CHANGE
        float                                           m_fNowSlashTraceTipWide;
        std::map< int, D3DXVECTOR3 >                    m_mapNowAddSlashTrace;
        float                                           m_fCommonDamageChangeTime;
//{{ kimhc // 2011.1.21 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
		CX2DamageManager::HITTED_TYPE					m_eHittedTypeAtState;	// 스테이트 별 HittedType 지정, 이것도 일단은 청만 사용 하는 걸로 알고 있긴 한데...
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.21 // 청 1차 전직

        D3DXVECTOR2                                     m_v2NowAfterImageTime;
        std::vector<KAttackTimeProjSeqName>             m_vecAttackTime;
		vector<D3DXVECTOR3>								m_StopAllList;
		vector<D3DXVECTOR2>								m_StopOtherList;
		vector<D3DXVECTOR2>								m_StopMyList;
		vector<D3DXVECTOR3>								m_Stop2AllList;
		vector<D3DXVECTOR2>								m_Stop2OtherList;
		vector<D3DXVECTOR2>								m_Stop2MyList;
        std::vector<SoundPlayData>                      m_vecSoundPlayData;
		std::vector<TIME_SPEED> m_vecSpeedFactor;	/// 유저만 사용중, 스크립트 상에서 특정 시간대에 스피드를 줄때 사용

		std::vector< EffectSetToPlay >                  m_vecEffectSetToPlay;	/// 플레이 되야 하는 이펙트 셋

		bool m_bDeleteEffectSetOnStateEnd;	/// m_vecEffectSetToPlay에 있는 것 들을 m_vecEffectSetToDeleteOnStateEnd에 넣어야 하는지 여부
		bool m_bDeleteEffectSetOnDamageReact;	/// m_vecEffectSetToPlay에 있는 것 들을 m_vecEffectSetToDeleteOnDamageReact에 넣어야 하는지 여부
		bool m_bDeleteEffectSetOnDie;	/// m_vecEffectSetToPlay에 있는 것 들을 m_vecEffectSetToDeleteOnDie에 넣어야 하는지 여부
#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
		bool m_bDeleteEffectSetOnCustomState;	/// m_vecEffectSetToPlay에 있는 것 들을 m_vecEffectSetToPlay에 넣어야 하는지 여부
#endif DELETE_EFFECTSET_ON_CUSTOM_STATE
		bool m_bHyperEffectSet;	/// 이펙트 셋에 HYPER_NAME을 지정하고, 이것을 설정 하면 HYPER_NAME으로 지정된 이펙트를 실행
		float m_fEffectSetLifeTime;	/// 이펙트 셋의 플레이 시간...
#ifdef ADDITIONAL_MEMO
		int	m_iMemoId;
#endif
#ifdef CUSTOM_DELETE_EFFECT_ON_DAMAGE_REACT
		bool m_bIsCustomStateDeleteEffectOnDamageReact;
#endif // CUSTOM_DELETE_EFFECT_ON_DAMAGE_REACT

		// kimhc // 2010-12-08	
		std::vector<BUFF_TEMPLET_ID>	                m_vecImmunityAtThisState;	/// 각각의 스테이트에서만 Immune 되어야할 ExtraDamage 지정.. 근데 여기에 놓고선 유저한테만 적용 되도록 해놨군... 수정!!해야함

#ifdef  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
		map< UINT, float >                              m_mapStateInsertBuffFactor;	// 특정 스테이트의 지정 시간이 되면 자기 자신에게 거는 버프팩터 리스트
#endif  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		UidType											m_OwnerUserUID;		/// UserUID 계정 UID
		int												m_FrameBufferNum;	/// 일반적인 상황에서 패킷을 몇 프레임씩 묶어서 보내는가
		CX2Unit*										m_pUnit;	/// X2Unit 클래스
		
		bool											m_bFirstDataReceive;	/// P2P 패킷을 한번이라도 받았는지 안받았는지.. 안받았으면 OnFrameRender_Prepare를 수행하지 않음
		bool											m_bPopAgain;	/// POP을 한번 수행 한 후에도 m_ReceiveSyncDataList.size() > m_FrameBufferNum(7) + m_AddFrameBuffer(1) 이면 Pop을 한번 더 수행 한다. 

		InputData										m_BeforeInputData;	/// 이것도 pureRight과 pureLeft만 가지고 있으면 됨
		InputData										m_InputData;		/// 개인적인 생각으로는 이것도 있을 필요가 없을 것 같은데...
		InputData										m_OrgInputData;		/// 기폭을 위해서 사용하는 듯 한데, pureX와 pureZ만 가지고 잇으면 됨

		//optimization
		vector<SyncData>								m_SendSyncDataList;		/// 전송할 SyncData를 저장
		vector<SyncData>								m_ReceiveSyncDataList;	/// 받은 SyncData를 저장
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        vector<SyncData>                                m_LastSendSyncDataList;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		map<float,bool>									m_EventTimeStampFuture;	/// CommonStateStartFuture에서 clear, Key: 애니메이션 타임, Value: false 면 이미 수행한 애니메이션
		map<float,bool>									m_EventTimeStampNow;	/// CommonStateStart 및 청의 특정 AnimationEnd 시에 clear, Key: 애니메이션 타임, Value: false 면 이미 수행한 애니메이션
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		//유닛 컴포넌트
		CKTDXDeviceXSkinMesh*							m_pMotionFuture;	/// 자기 캐릭터한테만 필요
		CKTDXDeviceXSkinMesh*							m_pMotion;		
		CKTDGXSkinAnim*									m_pXSkinAnimFuture;	/// 이것도 자기 캐릭터한테만 필요

		//{{ dmlee 2009.3.12 백그라운드 로딩 관련 크래시 때문에 shared_ptr로 수정
		CKTDGXSkinAnimPtr								m_pXSkinAnim;		
		//}} dmlee 2009.3.12 백그라운드 로딩 관련 크래시 때문에 shared_ptr로 수정

		CKTDXDeviceXET*									m_pAniXET;	/// 사용 안함

		CKTDXDeviceXSkinMesh*							m_pOrgMotionFuture;	/// 사용 안함
		CKTDXDeviceXSkinMesh*							m_pOrgMotion;		/// 배매 사용..
		CKTDGXSkinAnim*									m_pOrgXSkinAnimFuture;	/// 사용 안함
		CKTDGXSkinAnimPtr								m_pOrgXSkinAnim;	/// 사용 안함




        /// 내부적으로 관리하는 객체이므로 smart pointer를 사용하지 않는다.
        /// - jintaeks on 2009-01-12, 17:39
#ifdef FIX_MEMLEAK01
		CKTDXDeviceXSkinMesh*							m_pAfterImageMotion;
#endif
		//CKTDGXSkinAfterImage*							m_pAfterImage;	/// 이거.. 안쓰는 쪽으로 생각해 보자...
		

		CKTDGSlashTrace*								m_pSlashTrace;	/// 사용 안함
		CKTDGSlashTrace*								m_pSlashTraceTip;	/// 사용 안함


		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHeadMarker;	/// 머리위의 표시
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqEmblem;		/// 칭호
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticleHandle				m_hHeadMarkerParticle;
		CKTDGParticleSystem::CParticleHandle				m_hPart_Emblem_200;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CKTDGParticleSystem::CParticle*						m_pHeadMarkerParticle;
		CKTDGParticleSystem::CParticle*						m_pPart_Emblem_200;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHoldEffCenter;	/// 사용안함.. 예전에 마나 Hold로 사용 하던때의 것이군...구현이 이상한데?? Create는 Major에서 하고선, Get은 왜 Minor에서 하나?
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHoldEffCircle;	/// 구현이 이상한데?? Create는 Major에서 하고선, Get은 왜 Minor에서 하나?

		/// 이것들도 근데 인챈트 안한 유저들은??? 필요 없잖아...
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeaponCommon1;	/// 인챈트 파티클
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeaponCommon2;	/// 인챈트 파티클
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeaponCommon3;	/// 인챈트 파티클

		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchantedWeaponCommon4;	/// 트리플 속성 무기일 경우에 손에 붙는 파티클

		

#ifdef SKILL_CASH_10_TEST
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqDrainParticle;	/// 블러디 액셀에 맞았을 때 생기는 이펙트..
#endif
		//{{ seojt // 2009-1-13, 16:09
		vector<CX2EqipPtr>								m_ViewEqipList;		/// 눈에 보이는 장비
		//}} seojt // 2009-1-13, 16:09

#ifdef EQUIP_STRIP_TEST
		CX2ItemManager::EquipPositionMap				m_mapBasicEquip;		/// 기본 장비
		std::map< CX2Unit::EQIP_POSITION, bool >		m_mapEquipStripped;		/// 장비가 탈의된 상태인지.

		float											m_fTimeLeftToClearOutSuccessiveHit;
		int												m_iSuccessiveHitCount;
#endif EQUIP_STRIP_TEST

		int												m_AddFrameBuffer;	/// 1 값으로 고정되어 있는데.. 왜 필요한 걸까??
		float											m_fFrameWaitTime;
		int												m_FrameWaitNum;
		float											m_fFrameSkipTime;
		int												m_FrameSkipNum;

		float											m_fRebirthTime;		/// 부활 시간

		int												m_nDamageCount;		/// 피격획수

		bool											m_StopAdvanceTimeFuture;
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		int												m_AdvanceTimeCount;		/// 사용안함 1이외의 값을 셋팅하는 곳이 없음
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_Spine;
		

		bool						m_bHyperModeUsed;	/// 튜토리얼에서 각성을 사용했는지 체크하기 위한 용도
		float						m_fElaspedTimeLastGuideMessage;


		bool											m_bConnect;
		float											m_fConnectTimeOut;
		bool											m_bConnectSend;
		bool											m_bSwapSkillSlot;
#ifdef ATTACK_DELAY_GAGE
//{{AFX
		bool											m_bAddAttackDelayThisState;
		float											m_fLastAttackPassTime;
//}}AFX
#endif ATTACK_DELAY_GAGE

		bool											m_bAddForceDownGageThisState;
		float											m_fLastDamagePassTime;		/// 최근 타격 받은 후 지난 시간... 근데 이건 도대체 왜 따로 선언 되어 있는가..?

		bool											m_bResurrectionTimeOut;


		float											m_fDevAddDamage;


		int												m_iNowSpecialAttack;		/// 1, 2, 3 special attack, 0: nothing, 훈련소에서 필살기 훈련때 사용		
		bool											m_bHoldSpecialAttack;		/// 홀드했다가 필살기를 사용한건지~
		bool											m_bHoldingSpecialAttack;	/// 필살기를 hold하고 있는지

//#ifndef NOT_USE_DICE_ROLL
//		DiceRoll*										m_pDiceRoll;	/// 던전에서만 필요
//#endif //NOT_USE_DICE_ROLL

		//optimization
		CashItemAbility									m_CashItemAbility;


		// 라인맵 점프 발판 관련
		D3DXVECTOR3										m_vAddJumpSpeed;
		bool											m_bSteppedJumpLinemap;


		float											m_fAnimSpeedRate;			/// 속성 데미지등에 의해 애니메이션 속도가 느려지는 경우, m_fAnimSpeed에 곱해진다.
		float											m_fAnimSpeed;				/// 스크립트에 지정된 각 상태별 애니메이션 속도, default = 1.f
#ifdef ADD_ANIM_SPEED_FUTURE
		float											m_fAnimSpeedFuture;			/// 스크립트에 지정된 각 상태별 애니메이션 속도, default = 1.f, used in future state
#endif // ADD_ANIM_SPEED_FUTURE
		bool											m_bColdSlowApplied;
		bool											m_bFrozenSlowApplied;




		//optimization
		std::vector< wstring >							m_vecDungeonWinSpeech;

		float											m_fTimeToDashCameraChange;
		float											m_fAngleDegree;
		float											m_fTimeToReturnCameraAngle; //대시 끝나고 카메라 앵글 되돌리기 시작할 카운트 


		vector< int >									m_vecSetItemOptions;
        vector< int >                                 m_vecTitleOptions;

#ifdef PORTAL_LINEMAP_TEST
		bool											m_bEnterPortal;		
		bool											m_bLeavePortal;		
		CKTDXTimer										m_timerInPortal;
#endif PORTAL_LINEMAP_TEST
		
		float											m_fOriginalMPChangeRate; // 아이샤, 레나처럼 charge상태가 따로 있는 경우에 원래대로 복구하기 위해

		//{{ mauntain : 김태환 [2012.07.26] 클로킹 메모 쓸쓸한 기다림 버그 수정 ( 작업자 : 엘소드 대만 이지헌 )
#ifdef EVE_MEMO5_BUG_FIX
		float											m_fAdditionalMPChangeRateByPassive;	// 페시브에 의해 추가된 mp 회복량
#endif EVE_MEMO5_BUG_FIX
		//}}

		bool											m_bRidingMachine;
		bool											m_bLeaping;

		map< int, float >									m_mapNpcIdNDamageUpPercent; //특정 몬스터 때렸을 때 데미지 더 증가.
		map< int, float >									m_mapNpcIDNDEvadeUpPercent;	//특정 몬스터가 하는 공격에 회피율 증가
		map< CX2DamageManager::EXTRA_DAMAGE_TYPE, float >	m_mapExtraDamageNImmunePer;


#ifdef SHOOTING_TEST
		CKTDXTimer										m_TimerGunFireCooling;
		CKTDXTimer										m_TimerGrenadeCooling;
#endif SHOOTING_TEST

		bool											m_bReserveForceDownForShock;


		CKTDXTimer										m_TimerRandomSeedSync;
		static const double								RANDOM_SEED_RESET_TIME;

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		//optimization
//		KXPT_UNIT_USER_SYNC_PACK                        m_kXPT_UNIT_USER_SYNC_PACK;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		
#ifdef UNIT_EMOTION
		CX2Unit::EMOTION_TYPE							m_ePlayedEmotion;		
		bool											m_bReserveSitReadyEmotion;
#endif

		bool											m_bSocketSuperArmor;
		float											m_fSocketSuperArmor;
		bool											m_bStateSuperArmor;
#ifdef DUNGEON_ITEM
		//optimization
		bool											m_bSuperArmorByItem;
		float											m_fSuperArmorByItem;		
#endif

		//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
		CX2EffectSet::Handle m_hEffectTooniLandEvent;
#endif	SERV_TOONILAND_CHANNELING_EVENT
		//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

#ifdef BUFF_TEMPLET_SYSTEM
		//엘의 수정 버프 이펙트
		CX2EffectSet::Handle m_hEffectCrystalOfElBuff;
#endif BUFF_TEMPLET_SYSTEM

//#ifdef PVP_BOSS_COMBAT_TEST
//		CX2EffectSet::Handle m_hEffectSetBossMark;		
//		int			m_iFriendlyHittedCountAtFrozenState;
//#endif PVP_BOSS_COMBAT_TEST

#ifdef USER_HOLD
		bool		m_bHold;
		D3DXVECTOR3 m_vHold;
#endif

#ifdef SKILL_CASH_10_TEST
		CX2GameUnitoPtr	m_optrLastAttackUnit;
#endif

#ifdef NO_DETONATION
		bool				m_bNoDetonation;		// 기폭 가능여부 
		bool				m_bNowStateDamageReact; // 현재 스테이트가 피격으로 인해 변경된 것인지... 
#endif

		CX2ComboGuide*		m_pComboGuide;



#ifdef RENA_SIEGE_MODE_ANGLE_TEST
		bool m_bEnableLookAtIK_Spine;
		float m_fLookAtIKAngle_Spine;
#endif RENA_SIEGE_MODE_ANGLE_TEST
		bool m_bRotateBone;
		float m_fRotateBoneDegree;

		
		//optimization
		std::vector< CommonRandomState > m_vecRandomStartState;
		std::vector< CommonRandomState > m_vecRandomWinState;
		std::vector< CommonRandomState > m_vecRandomLoseState;


		SPECIAL_ATTACK_KEY_PRESSED m_eSpecialAttackKeyPressed;
		bool m_bSpecialAttackEventProcessedAtThisFrame; // 이번 프레임에 필살기 키 이벤트 처리 했는지 안했는지


		int m_iSkillCutInSetIndex;
		int m_iSkillCutInSetSubIndex;

		//{{ kimhc // 2009-11-20 // 길드 스킬 3, 4라인
#ifdef	GUILD_SKILL_PART_2
		//optimization
		GuildSkillData			m_GuildSkillData;			//  길드 스킬과 관련된 정보를 담는 구조체
		bool					m_bHaveSacrificeOfHero;		// 영웅의 희생을 가지고 있는가?
#endif	GUILD_SKILL_PART_2
		//}} kimhc // 2009-11-20 // 길드 스킬 3, 4라인

#ifdef RAVEN_SECOND_CLASS_CHANGE
		float m_fPassiveCriticalRate;		// 추가 크리티컬 확율수치(패시프 버프)
		//optimization
		std::vector<TIME_PUSHPASS> m_vecCanPushUnit;
		std::vector<TIME_PUSHPASS> m_vecCanPassUnit;
//#ifdef  X2OPTIMIZE_USER_PASS_PUSH_UNIT_TIME_BUG_FIX
		std::vector<TIME_PUSHPASS> m_vecCanPushUnitNow;
		std::vector<TIME_PUSHPASS> m_vecCanPassUnitNow;
//#endif  X2OPTIMIZE_USER_PASS_PUSH_UNIT_TIME_BUG_FIX
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		float					m_fShadowOfDisasterRate;	/// 재앙의 그림자로 증가되는 크리티컬 비율
		CurrentSkillKeyManager	m_sCurrentSkillKeyManager;	/// 키 추가 입력 스킬 관리 구조체
		bool					m_bIsLimitCrusherCharge;	/// 레이븐 레크리스 피스트 - 리미트 크러셔 차지 상태 여부
#endif //UPGRADE_SKILL_SYSTEM_2013

//#ifdef TRANSFORMER_TEST
//		CX2GUNPC* m_pTransformer;
//#endif TRANSFORMER_TEST
		
		//{{ 임규수 임규수 던전 스타트 표시 나오기 전 , 던전 보스 죽인 후에 퀵슬롯 사용 못하도록 수정
#ifdef FIX_QUICK_SLOT_USE_DUNGEON_PLAY
		bool	  m_bStarted;
#endif 
		//}}

		//{{ kimhc // 2010.2.10  // 이브 2차전직 
#ifdef	EVE_SECOND_CLASS_CHANGE
		bool					m_bHitNasodWeapon;	//	이브 나소드 웨폰에 맞았는가?
#endif	EVE_SECOND_CLASS_CHANGE
		//}} kimhc // 2010.2.10 //	이브 2차전직

//{{ kimhc // 2010.4.2 // 비밀던전 작업(셋트아이템효과)
#ifdef SERV_SECRET_HELL
		// 추가된 소켓아이템 효과를 키값으로 옵션id들을 넣는다.

		//optimization
		typedef map< CX2SocketItem::SOCKET_ITEM_EFFECT_EX, vector<int> > SOCKET_EFFECT_EX_MAP;
		SOCKET_EFFECT_EX_MAP m_mapSocketItemEffectEx; // value는 소켓옵션id

		float				m_fManaRecoveryDurationTime;	// 셋트아이템 의해 마나 자연회복양에 + 되는 효과의 지속 시간
		float				m_fManaRecoveryVal;				// 마나 자연회복 양에 + 되는 값

		float				m_fIntervalTimeForCureDebuff;	// 셋트 효과중 CureDebuff에 대하여 이시간에 0이 되지 않으면 다시 발생하지 않음
		Bone_Shield*		m_pBoneShieldData;				// 셋트 효과중 뼈방패

#ifdef ADJUST_SECRET_ITEM_OPTION //김창한
		float				m_fSocketOptionCoolTime;		// 소켓 5셋 옵션 쿨타임
		float				m_fSocketOptionHyperCoolTime;	// 각성시 적용되는 옵션 쿨타임
#endif //ADJUST_SECRET_ITEM_OPTION

#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.2 // 비밀던전 작업(셋트아이템효과)
		CKTDXDeviceTexture*				m_pTextureRank;		/// 대전 랭크 표시 텍스쳐
#ifdef SERV_INTEGRATION
		//{{ oasis907 : 김상윤 [2010.5.17] // 던전 대전 통합 서버 아이콘
		CKTDXDeviceTexture*				m_pTextureServer;	
		//}}
#endif SERV_INTEGRATION

#ifdef SERV_PET_SYSTEM
		int			m_iDieCount;
		PetCheer	m_petCheer;
#endif

//{{ kimhc // 2010.7.12 // 여러명의 유저를 HOLD 할 수 있도록 함
		// HOLD 상태일때 변경될 스테이트( 기본은 m_DamageAirFall )임
		int					m_iHoldStateID;	
//}} kimhc // 2010.7.12 // 여러명의 유저를 HOLD 할 수 있도록 함
		
#ifdef PET_AURA_SKILL
		bool		m_bApplyAura;	// 펫 기운스킬 적용여부
#endif

//{{ 김상훈 2010.10.29
#ifdef NEW_SKILL_2010_11
	#ifndef FIX_LIMITED_MANA_MANAGEMENT
		bool		m_bLimitManaManagement;//확률적으로 패시브 스킬이 발동했는지.
	#endif //FIX_LIMITED_MANA_MANAGEMENT
#endif 
//}} 김상훈 2010.10.29
		
		bool	m_bDownForce;	/// 점프 등의 상태에서 키보드에서 아래쪽 방향 화살표를 눌렀는지에 대한 여부를 저장 (각 캐릭터에게 있는 것을 옮겨옴)

		//{{ kimhc // 2010.12.9 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		vector<CX2EqipPtr>	m_vecCloneEquipList;		// 다른 장비들 보다 우선으로 보여야 하는 장비(클론)의 리스트		
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.9 //  2010-12-23 New Character CHUNG


#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		bool	m_bIsTransformed;
#endif

#ifdef ELSWORD_SHEATH_KNIGHT
		EventProcessTime		m_fEventTime[10];
		float					m_fAttackCancelAfterEx; // OneZ, OneX
		float					m_fAttackCancelAfter;	// PureZ, PuerX
		float					m_fSkillCancelAfter;
		float					m_fBWalkCancelAfter;
		float					m_fWalkCancelAfter;
		float					m_fDashCancelAfter;
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		float					m_fSkillCancelAfterFly;		/// 공중용 스킬 캔슬
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef HYPER_MODE_FIX
		int						m_iLastStateHyperModeCount;
#endif HYPER_MODE_FIX
#ifdef SERV_CHUNG_TACTICAL_TROOPER
		int						m_iHyperModeCountUsed;
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef SPECIAL_USE_ITEM
		CX2EffectSet::Handle m_hSpecialItemBuff_Wind;
		bool			m_bThrowSuccess;
		int				m_iThrowItemValue;
		float			m_fThrowStartTime;
		D3DXVECTOR3		m_vThrowStartPosOffset;
		D3DXVECTOR3		m_vThrowStartVelocity;
#endif

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	float		m_fCannonBallDamageUp;
	float		m_fForceDownRelDamageRateBase;
#endif

	float m_fSummonNpcCoolTime;
	vector<SummonNpcSocket> m_vecSummonNpcSocket;

#ifdef SERV_ELSWORD_INFINITY_SWORD
	float	m_bLearnLightningStep;
#endif

#ifdef SERV_ARME_DIMENSION_WITCH
	int		m_iAdvancedTeleportationLv;
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	GameCameraOffset m_GameCameraOffset;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffectHandle   m_hCEffectTacticalField;	/// 택티컬 필드 데미지 이펙트
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect* m_pCEffectTacticalField;	/// 택티컬 필드 데미지 이펙트
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif SERV_CHUNG_TACTICAL_TROOPER


#ifdef SERV_TRAPPING_RANGER_TEST
	typedef	srutil::delegate0<void>	DelegateVitalPointPiercing;
	DelegateVitalPointPiercing		m_delegateVitalPointPiercing;
#endif SERV_TRAPPING_RANGER_TEST

	CX2ComboAndSkillChainSystem	m_ComboAndSkillChainSystem;

#ifdef NEW_CHARACTER_EL
	float m_fAddAccuaracyPercent;	/// 집중력 향상 패시브, 명중률 퍼센트 증가 값 ( 기본값 : 0 / 최대값 1)
	float m_fAddMagicAttackCriticalRate;  /// 집중력 향상 패시브, 마법 공격 크리티컬 발동 확률 증가 값( 기본값 : 0 / 최대값 1 )
	float m_fAddMagicAttackCriticalDamageRate;  /// 집중력 향상 패시브, 마법 공격 크리티컬 데미지 증가 증가 배율 값( 기본값 : 0 / 최대값 1 )
#endif //NEW_CHARACTER_EL

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	bool		m_bSetCustomRenderParam;			/// 정의한 m_RenderParam의 값을 그대로 적용할 수 있게하기 위한 변수.
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

private:

	//{{ kimhc // 2010.11.3 // 스킬추가 기능
#ifdef	NEW_SKILL_2010_11
		// 혹시 다른 캐릭터에게 재각성이 생길 경우를 생각해서 추가함
		bool		m_bCanChangeHyperModeInHyperModeState;	/// 각성상태에서 각성을 사용할 수 있는가? (레피 - 승화된 분노)
#endif	NEW_SKILL_2010_11
	//}} kimhc // 2010.11.3 // 스킬추가 기능

		//{{ JHKang / 강정훈 / 2010/12/16 / 현재 피격 상태인가?
#ifdef SEASON3_MONSTER_2010_12
		bool	m_bNowHit;				/// 피해를 받았는지?
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / 강정훈 / 2010/12/16 / 현재 피격 상태인가?

		//{{ JHKang / 강정훈 / 2011/01/17 / 최종 데미지 값을 저장할 멤버 변수
#ifdef SEASON3_MONSTER_2010_12
		float	m_fRealDamage;			/// 현재 User가 받은 데미지 수치
		float   m_fAnimSpeedRateByNpc;	/// User Animation 속도
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / 강정훈 / 2011/01/17 / 최종 데미지 값을 저장할 멤버 변수

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahCommonMajorParticleInstance[COMMON_MAJOR_PII_END];
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahCommonMinorParticleInstance[COMMON_MINOR_PII_END];

		float	m_fOxygenGage;

		//{{ JHKang / 강정훈 / 2011/02/14 / 던전 랭크 개선 관련
#ifdef DUNGEON_RANK_NEW
		KProtectedType<int> m_iTotalAttackedDamage;		/// 던전 종료까지 받은 총 데미지 누적량
		KProtectedType<int> m_iTotalRecoveryHP;			/// HP 회복량( 소비 아이템(포션, 음식) / 상자, 일리피아, 등 기타 회복류 제외 )
		KProtectedType<int> m_iUsingSkillCount;			/// 사용 스킬 카운트(8개의 스킬 중 2회 이상 사용시 카운트)
		KProtectedType<int> m_iTotalGivenDamamge;		/// 유저가 몬스터에게 준 피해량
		int m_iSkillSlotACount[4], m_iSkillSlotBCount[4];
#endif DUNGEON_RANK_NEW
		//}} JHKang / 강정훈 / 2011/02/14 / 던전 랭크 개선 관련
#ifdef DUNGEON_DASH_LOG
		int m_iDashCount;			/// 유저가 던전에서 대쉬 사용 한 횟수 카운트
#endif DUNGEON_DASH_LOG

		KProtectedType<float>	m_fCurrentFinalDamage;		/// 현재 유저가 상대에게 준 최종적인 데미지


#ifdef TEST_GROUP_GRAP
		bool						m_bGrap;				/// 현재 잡혀 있는 상태인가?		
		int							m_iGrapShakeCount;		/// 키보드를 좌우로 움직여야 하는 횟수 (이 횟수 만큼 흔들면 잡기에서 풀림)
		int							m_iGrapStateID;			/// 붙잡혔을 때 강제로 변경될 스테이트 (잡힌 동안에는 이 스테이트를 실행 함)
		D3DXVECTOR3					m_vGrap;				/// 붙잡한 상태에서의 유저 위치
		D3DXVECTOR3					m_vBeforeGrapPos;		/// 붙잡히기 이전의 유저 위치
		
		bool						m_bGrapEscapeKeyLeftCheck;	/// 이전에 Left 방향키를 눌렀는지에 대한 정보를 담고 있음
		bool						m_bGrapEscapeKeyRightCheck;	/// 이전에 Right 방향키를 눌렀는지에 대한 정보를 담고 있음

#endif TEST_GROUP_GRAP


#ifdef NEW_GRAPPLE_ATTACK
		
		int							m_iGrappledStateID;
		int							m_iShaking;
		bool						m_bGrappled;			

#endif NEW_GRAPPLE_ATTACK
#ifdef WORLD_TRIGGER
		float						m_fTriggerTime;
#endif

#ifdef CHUNG_FIRST_CLASS_CHANGE
		float						m_fAddAnimSpeedFactor;			/// 청 1차 전직, 슈팅 가디언의 난전의 기술 스킬에 의한 애니메이션 속도 증가
#endif

		//}} Protected Variable End
		bool m_bMixedEmotion[AVATAR_EMOTION_NUM];
		CKTDXDeviceSound*				m_pAvatarEmotionSound;

#ifdef NEW_HENIR_TEST
		CX2EffectSet::Handle m_hHenirBuff_Nature;
		CX2EffectSet::Handle m_hHenirBuff_Wind;
#endif NEW_HENIR_TEST

#ifdef FACE_OFF_MONSTER_HEAD_TEST
		bool			m_bHideNoRenderable;
#endif FACE_OFF_MONSTER_HEAD_TEST

#ifdef SERV_ADD_TITLE_CONDITION
		unsigned char m_ucDieReason;
#endif

#ifdef UPGRADE_RAVEN
		bool m_bParryingState;
#endif

#ifdef ARA_CHANGE_CLASS_FIRST
		bool	m_bQuickenGuardBlocking;		/// 1-2차 번득이는 기지 패시브 막기
		bool	m_bQuickenGuardTurning;			/// 1-2차 번득이는 기지 패시브 뒤돌아맞기
#endif

#ifdef HYPER_MODE_FIX
		bool m_bReserveHyperModeZero;
#endif HYPER_MODE_FIX

#ifdef EVE_ELECTRA
		bool m_bWhiteOut;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqWhiteOut;
#endif EVE_ELECTRA
		
#ifdef BUFF_TEMPLET_SYSTEM
		//10초당 전체 체력의 % 회복 소켓 - 회복 카운트 시간
		float m_fSocketHPIncreasePerSecondTime;
#endif BUFF_TEMPLET_SYSTEM

#ifdef SERV_ELSWORD_INFINITY_SWORD
		UCHAR			m_OldStateId;	// 이전 스테이트를 저장함 (언젠간 써먹을곳이 있지 않을까?)
#endif


		CKTDXCheckElapsedTime m_ElapsedTimeNotBusyState;	// 현재 바쁜 상태인지 아닌지 체크 용도

		DelegatePhysicProcessPortalByGameType		m_delegatePhysicProcessPortalByGameType;


#ifdef SUMMON_MONSTER_CARD_SYSTEM
		SummonMonsterCardData m_SummonMonsterCardData;		/// 소환 몬스터 카드 정보 저장 객체
#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
		bool			m_bUseChangeWeapon;					/// 아이샤의 매지컬 메이크 업이나 청의 3각성을 통한 무기 변경 여부
#endif FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
		
		CKTDGParticleSystem::CParticleEventSequenceHandle m_hEnterCashShop;

		bool m_bHasHyperModeItem;

		bool m_bEnterCashShop;								/// 캐시삽 입장 상태

#ifdef ARA_CHANGE_CLASS_FIRST
		/// 오현빈 // 2013-01-10 // 회피 가능 여부 ( 현재는 소선의 이화접목으로만 회피가 가능합니다. )
		bool m_bCanAvoidance;
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef NOT_CANCEL_BBT_STUN_BY_HIT
		bool m_bChangeGroggyState;		/// 버프를 통한 스턴 그로기 스테이트 전환 여부
#endif NOT_CANCEL_BBT_STUN_BY_HIT

#ifdef EFFECT_TOOL
		CKTDGXSkinAnim*				m_pXSkinAnim_EffectTool;
#endif //EFFECT_TOOL
#ifdef RIDING_SYSTEM
		bool			m_bRidingOn;	/// 탑승?
		D3DXVECTOR3		m_vSaddlePos;	/// 안장 위치
		D3DXVECTOR3		m_vDirDegree;	/// 안장 회전 값

		wstring		m_wstrRidingMotionName;					/// 라이딩 모션
		CKTDXCheckElapsedTime m_ElapsedTimeRidingWaitHabit;		/// 습관 상태 대기 시간 지정
		UINT		m_iRidingBoost;		/// 탈 것의 Boost 가속을 위한 멤버 변수
		float		m_fAirDashTime;		/// 공중 달리기
		UINT		m_iAirDashCount;	/// 공중 달리기 횟수 제한
		UINT		m_iDoubleJumpCount;	/// 이단 점프 횟수
#endif //RIDING_SYSTEM

#ifdef CHECK_CAMERA_INFO
		bool			m_bIsLineMapCamera;
#endif //CHECK_CAMERA_INFO


#ifdef  ADDED_RELATIONSHIP_SYSTEM
		UidType		m_RelationshipTargetUID;
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef	CHUNG_FIRST_CLASS_CHANGE
		// 오현빈 // X2Unit에 있는 변수 그대로 긁어옴.
		// X2Unit에서 처리 하면 자기 자신 만 수행하기 때문에, 
		// 다른 유저에게도 정상 동작 할 수 있도록 Guuser로 옮김
		// X2Unit에 처리하고 있는 것을 유지하는 이유는 UI 때문!!
		CX2Stat::EnchantStat		m_EnchantStatFromPassiveSkill;	// 패시브 스킬 등에 의해 증가되는 EnchantStat
#endif	CHUNG_FIRST_CLASS_CHANGE

#ifdef INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
		// 현재 스테이트 스킬 레벨 
		// 지정한 시간에 자기 자신에게 적용하는 버프를 위해 사용
		UINT m_uiNowStateSkillLevel;
#endif // INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

	// 오현빈 // 2013-07-02
	// 스킬컷인 관리를 스크립트에서 할 수 있도록 추가
	float m_fShowSkillCutInTime;
	bool m_bChangeWorldColor;

#ifdef CHEAT_SELF_DAMAGE // 김태환
	int m_iFrontDamage;		/// 자신에게 데미지를 주는 치트중 피격 방향 설정
#endif //CHEAT_SELF_DAMAGE

#ifdef ADD_MEMO_1ST_CLASS //김창한
	bool m_bApplyMemo;		/// 케릭터 스크립트에 정의된 메모 ID가 현재 장착되어 있는지 체크.
#endif //ADD_MEMO_1ST_CLASS

#ifdef ADD_RENA_SYSTEM //김창한
	AllDamageRelateSkillData	m_AllDamageRelateSkillData;
#endif //ADD_RENA_SYSTEM

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	float						m_fGetFinalDamageValue;
#endif //SERV_ADD_LUNATIC_PSYKER

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
	bool	m_bManeuverParryingState;
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어

private:
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
    void _EncodeUserSyncPos( USHORT& usPosX, USHORT& usPosZ, const D3DXVECTOR3& position, unsigned char lastTouchLineIndex );
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//	void _EncodeUserSyncPos( USHORT& usPosX, USHORT& usPosY, USHORT& usPosZ, const D3DXVECTOR3& position, unsigned char lastTouchLineIndex );
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC

private:
	DWORD m_adwInitFrameMoveCount[2];

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
private:
	DWORD m_dwLastRecvFrameMoveCount;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef MODIFY_AFTER_IMAGE // 오현빈 // 2013-07-19	
	CKTDXDeviceXSkinMesh* m_pHairXSkinMesh; // 모자 착용 중 애프터 이미지에 현재 착용중인 Hair를 추가 하기 위한 변수
#endif //MODIFY_AFTER_IMAGE

#ifdef CHECK_SOUND_LOADING_TIME
	DWORD dwStartTime;
	DWORD dwTickCount;
#endif // CHECK_SOUND_LOADING_TIME

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	bool m_bRightAtThisState;			/// 해당 스테이트에서만 정면을 보고, 스테이트가 끝나면 원래 방향으로 돌아봄
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef FIX_BATTLE_FIELD_DYNAMIC_CAMERA
	float fPreWorldNear;
#endif // FIX_BATTLE_FIELD_DYNAMIC_CAMERA

//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
    void    ResetLocalAndRelativePosAndTimers();
    void    IncrementLocalRelativePosTimers( float fElapsedTime );
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC_BUG_FIX
    int     GetRelativePos( int iUnitIndex ) const          { return  ( GetSyncData( true ).m_dwRelativePos >> ( iUnitIndex * 2 ) ) & 0x3; }
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC_BUG_FIX
//    DWORD   GetRelativePosFlags() const                     { return m_FrameDataNow.syncData.m_dwRelativePos; }
//    int     GetRelativePos( int iUnitIndex ) const          { return  ( m_FrameDataNow.syncData.m_dwRelativePos >> ( iUnitIndex * 2 ) ) & 0x3; }
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC_BUG_FIX
    int     GetLocalRelativePos( int iUnitIndex ) const 
    { 
        return ( iUnitIndex >= 0 && iUnitIndex < MAX_GAME_USER_UNIT_NUM ) ? m_acLocalRelativePos[ iUnitIndex ] : 0;
    }
    void    PhysicProcess_RetrieveRelativePosInfo( CX2GUUser* pGameUnit, int iUnitIndex, OUT int& iDirection );
    void    PhysicProcess_UpdateRelativePos();
    char    m_acLocalRelativePos[MAX_GAME_USER_UNIT_NUM];
    float   m_afLocalRelativePosTimers[MAX_GAME_USER_UNIT_NUM];
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC

#ifdef FIX_SKILL_SLOT_CHANGE_BUG
	float	m_fSlotChangeLeftCoolTime;	// 슬롯 변경 쿨타임 남은 시간
#endif // FIX_SKILL_SLOT_CHANGE_BUG


#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		bool											m_bDoubleAttack;		/// 더블어택 성공여부
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

#ifdef ADD_PLAY_SOUND //김창한
		void PlaySoundRevive();	//부활했을때 사운드 출력.
#endif //ADD_PLAY_SOUND


#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
	bool	m_bIsPlayAvatarEmotionSoundWithoutEmotion;
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	int	  m_iAllyEventMonsterUID;		//소환한 몬스터 UID
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
};

namespace _CONST_GUUSER_
{
	const int aNormalSkillStateID[EQUIPPED_SKILL_SLOT_COUNT]		= { CX2GUUser::USI_SPECIAL_ATTACK_1, CX2GUUser::USI_SPECIAL_ATTACK_2, CX2GUUser::USI_SPECIAL_ATTACK_3, CX2GUUser::USI_SPECIAL_ATTACK_4, };
	const int aHyperSkillStateID[EQUIPPED_SKILL_SLOT_COUNT]			= { CX2GUUser::USI_SPECIAL_ATTACK_HYPER_1, CX2GUUser::USI_SPECIAL_ATTACK_HYPER_2, CX2GUUser::USI_SPECIAL_ATTACK_HYPER_3, CX2GUUser::USI_SPECIAL_ATTACK_HYPER_4, };
	const int aNormalSkillStateIDSlotB[EQUIPPED_SKILL_SLOT_COUNT]	= { CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_1, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_2, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_3, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_4, };
	const int aHyperSkillStateIDSlotB[EQUIPPED_SKILL_SLOT_COUNT]	= { CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_1, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_2, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_3, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_4, };
}

struct  CX2UnitManager_UnitTypeTemplet
{
	std::wstring         m_wstrLuaScriptFile;
	CX2GUUser::InitData m_init;

	CX2UnitManager_UnitTypeTemplet()
		: m_wstrLuaScriptFile()
		, m_init()
	{
	}//CX2UnitManager_UnitTypeTemplet()
};//struct  CX2UnitManager_UnitTypeTemplet


typedef KObserverPtr<CX2GUUser> CX2GUUseroPtr;

IMPLEMENT_INTRUSIVE_PTR( CX2GUUser::CSkillDataBase );
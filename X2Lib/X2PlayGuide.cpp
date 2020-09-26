#include "StdAfx.h"
#include "X2PlayGuide.h"


CX2PlayGuide::CX2PlayGuide(void):
m_pDLGPlayGuide( NULL ),
m_fOnTime(0.f),
m_bHide(false),
m_uiGuideStep(-1),
m_uiCurrentGuideStep(-1),
m_bStart(false),
m_bCompleteTutorial(false),
m_bActiveNoviceGuide(false),
m_bShowDungeonMenu(false),
m_hIndicator(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_hIndicatorMark(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_pDLGParticle(NULL),
m_bShowGuideParticle(false),
m_hInputGuide(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_eOldPlayGuide(PGT_NONE),
m_hParticleGo( INVALID_PARTICLE_SEQUENCE_HANDLE )
#ifdef GUIDE_FIRST_EPIC_QUEST
, m_pDLGUIFirstEpicQuestGuide(NULL)
, m_bShowFirstEpicQuestGuide(false)
, m_fEpicGuideDelayTime( 0.f )
, m_fEpicGuideAlphaTime( 0.f )
#endif //GUIDE_FIRST_EPIC_QUEST
{
	m_pDLGPlayGuide		= new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_PlayGuide_New.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPlayGuide );
	m_pDLGPlayGuide->SetColor(D3DXCOLOR(1,1,1,0));
	m_pDLGPlayGuide->SetShowEnable( false, false );

	m_PlayGuide[PGT_LEVEL_UP].SetPlayGuide(			 PGT_LEVEL_UP,				GPT_ALL,			STR_ID_17260, 99, false);
	m_PlayGuide[PGT_OBTAIN_SKILL_POINT].SetPlayGuide(PGT_OBTAIN_SKILL_POINT,	GPT_ALL,			STR_ID_17261, 99, false);
	m_PlayGuide[PGT_DUNGEON_PLAY].SetPlayGuide(PGT_DUNGEON_PLAY,				GPT_VILLAGE_FIELD,	STR_ID_18721, 99 );
	m_PlayGuide[PGT_QUEST_COMPLETE].SetPlayGuide(	 PGT_QUEST_COMPLETE,		GPT_ALL,			STR_ID_18683, 40 );
	m_PlayGuide[PGT_NOVICE_GUIDE_01].SetPlayGuide(	 PGT_NOVICE_GUIDE_01,		GPT_ALL,			STR_ID_18286, 10 );
	m_PlayGuide[PGT_NOVICE_GUIDE_02].SetPlayGuide(	 PGT_NOVICE_GUIDE_02,		GPT_DUNGEON_FIELD,	STR_ID_18287, 10 );
	m_PlayGuide[PGT_HYPER_MODE].SetPlayGuide(		 PGT_HYPER_MODE,			GPT_DUNGEON_FIELD,	STR_ID_17262, 10 );
	m_PlayGuide[PGT_SPECIAL_ATTACK].SetPlayGuide(	 PGT_SPECIAL_ATTACK,		GPT_DUNGEON_FIELD,	STR_ID_17263, 10 );
	m_PlayGuide[PGT_GO_NEXT_STAGE].SetPlayGuide(	 PGT_GO_NEXT_STAGE,			GPT_DUNGEON,		STR_ID_17264, 10 );
	m_PlayGuide[PGT_OBTAIN_EQUIP_ITEM].SetPlayGuide( PGT_OBTAIN_EQUIP_ITEM,		GPT_ALL,			STR_ID_17265, 10 );
	m_PlayGuide[PGT_OBTAIN_TITLE].SetPlayGuide(		 PGT_OBTAIN_TITLE,			GPT_ALL,			STR_ID_17266, 10 );
	m_PlayGuide[PGT_HAS_SKILL_POINT].SetPlayGuide(	 PGT_HAS_SKILL_POINT,		GPT_VILLAGE_FIELD,	STR_ID_17267, 10 );
	m_PlayGuide[PGT_COMPLETE_EPIC_QUEST].SetPlayGuide(PGT_COMPLETE_EPIC_QUEST,	GPT_VILLAGE_FIELD,	STR_ID_17268, 99 );
}


CX2PlayGuide::~CX2PlayGuide(void)
{
	SAFE_DELETE_DIALOG( m_pDLGPlayGuide );
#ifdef GUIDE_FIRST_EPIC_QUEST
	SAFE_DELETE_DIALOG( m_pDLGUIFirstEpicQuestGuide );
#endif //GUIDE_FIRST_EPIC_QUEST

	DeleteQuestGuideParticle();
	DeleteInputGuideParticle();

	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hParticleGo )
	{
		if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hParticleGo);
		}
	}
}


void CX2PlayGuide::OnFrameMove( double fTime, float fElapsedTime )
{
	OnFrameMove_Tutorial();

#ifdef GUIDE_FIRST_EPIC_QUEST
	OnFrameMove_FirstEpicGuide(fElapsedTime);
#endif //GUIDE_FIRST_EPIC_QUEST

	if( NULL == m_pDLGPlayGuide )
	{
		return;
	}

	if( false == IsShowGuide() )
	{
		m_pDLGPlayGuide->SetColor(D3DXCOLOR(1,1,1,0));
		return;
	}

	PLAY_GUIDE_TYPE eCurrentPlayGuide = GetCurrentPlayGuide();
	
	if( true == m_GuideOrderManager.GetGuideListEmpty() )
	{
		m_pDLGPlayGuide->SetShowEnable(false,false);
		return;
	}
	else
	{
		m_pDLGPlayGuide->SetShowEnable(true,true);
	}

	if( m_eOldPlayGuide != eCurrentPlayGuide && eCurrentPlayGuide != PGT_NONE)
	{ 
		m_eOldPlayGuide = eCurrentPlayGuide;

		//설정된 가이드에 맞게 출력 정보 설정 
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGPlayGuide->GetControl(L"g_pStatic_HELP");
		if( NULL != pStatic && NULL != pStatic->GetString(0) && NULL != pStatic->GetPicture(0) )
		{
			CKTDGUIControl::UIStringData* pString = pStatic->GetString(0);
			CKTDGUIControl::CPictureData* pPicture = pStatic->GetPicture(0);

			//가이드 스트링 길이를 기준으로 배경 크기 수정.
			CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pString->fontIndex );
			if( NULL != pFont )
			{
				if( false == m_PlayGuide[eCurrentPlayGuide].m_GuideString.empty() )
				{
					float fWidth = static_cast<float>(pFont->GetWidth(m_PlayGuide[eCurrentPlayGuide].m_GuideString.c_str())) + 35.f;

					pPicture->SetSizeX( fWidth );
					D3DXVECTOR2 vPos = pPicture->GetPos();
					vPos.x = 508 - (fWidth/2);
					pPicture->SetPos( vPos );
					//가이드 스트링 수정
					pString->msg = m_PlayGuide[eCurrentPlayGuide].m_GuideString;
				}
				else
				{
					SetPlayGuide(eCurrentPlayGuide, false);
					m_pDLGPlayGuide->SetColor(D3DXCOLOR(1,1,1,0));
				}
			}
		}
		else
		{
			m_pDLGPlayGuide->SetColor(D3DXCOLOR(1,1,1,0));
			return;
		}
	}

	OnFrameMove_SetAlpha(fElapsedTime);
}

/** @function : OnFrameMove_SetAlpha
	@brief : 플레이가이드의 알파값 설정
			OnFrameMove에서 호출됨.
*/
void CX2PlayGuide::OnFrameMove_SetAlpha( float fElapsedTime )
{
	if( NULL == m_pDLGPlayGuide )
		return;

	//정상적으로 설정 끝낸 이후에도 스트링이 비어있다면 안보여주도록 변경
	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGPlayGuide->GetControl(L"g_pStatic_HELP");
	if( NULL != pStatic && NULL != pStatic->GetString(0) )
	{
		if( true == pStatic->GetString(0)->msg.empty() )
		{
			m_pDLGPlayGuide->SetColor(D3DXCOLOR(1,1,1,0));
			m_GuideOrderManager.SetInvalideGuide();
			return;
		}
	}

	m_fOnTime  += fElapsedTime ;
	float fAlpha = 0.0f;

	if( m_fOnTime > 0.f )	//페이드 인
		fAlpha = m_fOnTime * 1.66f;

	if( m_fOnTime > 0.6f )	//인 유지
		fAlpha = 1.f;

	if( m_fOnTime > 1.8f )	//페이드 아웃
		fAlpha = 1.f - (m_fOnTime-1.8f)*1.66f;

	if( m_fOnTime > 2.4f ) //아웃유지
	{
		fAlpha = 0.f;
		m_fOnTime = 0.f;
		m_GuideOrderManager.SetEndCurrentGuide();
		// 반복하지 않는 1회성 가이드라면 한번 출력 후 비활성화 상태로 설정
		if( false == m_PlayGuide[m_eOldPlayGuide].m_bRepeat )
		{
			SetPlayGuide(m_eOldPlayGuide, false);
		}
	}

	m_pDLGPlayGuide->SetColor(D3DXCOLOR(1,1,1,fAlpha));		
}

/** @function : GetCurrentPlayGuide
	@brief	: 현재 출력해야 할 가이드 내용 얻기
	@return : 현재 출력해야 할 가이드 타입 (PLAY_GUIDE_TYPE)
*/

CX2PlayGuide::PLAY_GUIDE_TYPE CX2PlayGuide::GetCurrentPlayGuide()
{
	PLAY_GUIDE_TYPE eNextGuide = m_GuideOrderManager.GetNextGuide();		
	if( PGT_NONE != eNextGuide &&
		false == GetShowGuideByPlaceType(m_PlayGuide[eNextGuide].m_ePlaceType) )
	{
		m_GuideOrderManager.SetEndCurrentGuide();
		m_GuideOrderManager.EraseGuide(eNextGuide);
		m_PlayGuide[eNextGuide].m_bShow = false;
		eNextGuide = PGT_NONE;
	}
	return eNextGuide;;
}
/** @function : GetShowGuideByPlaceType
	@brief	: 현재 위치에서 출력 될 수 있는 가이드 여부 확인
	@param	: 가이드 타입(ePalceType_)
	@return	: 출력 가능 true, 출력 불가 false
*/
bool CX2PlayGuide::GetShowGuideByPlaceType( GUIDE_PLACE_TYPE ePlaceType_)
{
	CX2Main::X2_STATE eStateID = g_pMain->GetNowStateID();

	switch( ePlaceType_ )
	{
	case GPT_ALL:
		{
			return true;
		} break;
	case GPT_DUNGEON:
		{
			if( CX2Main::XS_DUNGEON_GAME == eStateID )
			{
				return true;
			}
		} break;
	case GPT_DUNGEON_FIELD:
		{
			if( CX2Main::XS_DUNGEON_GAME == eStateID ||
				CX2Main::XS_BATTLE_FIELD == eStateID )
			{
				return true;
			}
		} break;
	case GPT_VILLAGE_FIELD:
		{
			if( CX2Main::XS_VILLAGE_MAP == eStateID ||
				CX2Main::XS_BATTLE_FIELD == eStateID )
			{
				return true;
			}
		} break;
	default:
		break;
	}

	return false;
}

/**	@function : SetShowGuide
	@brief	: SetPlayGuide()에서 사용되며, 가이드의 출력 여부 설정
	@param	: 가이드 타입(eGuideType), 출력 여부(bShow)	?
*/
void CX2PlayGuide::SetShowGuide( CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType, bool bShow )
{
	//가이드 종류에 상관 없는 동작을 하는 경우 PGT_NONE을 인자로 받게 설정.
	if( eGuideType == PGT_NONE)
	{
		PLAY_GUIDE_TYPE eCurrentPlayGuide = GetCurrentPlayGuide();
		if( PGT_NONE != eCurrentPlayGuide )
		{
			m_PlayGuide[eCurrentPlayGuide].m_bShow = bShow;
		}
		return;
	}

 	//기존 설정과 같은 경우 아래 동작은 하지 않음.
 	if( m_PlayGuide[eGuideType].m_bShow == bShow )
 		return;

	//추가조건 확인.
	if( true == bShow )
	{
		m_PlayGuide[eGuideType].m_bShow = false;

		//레벨 조건. 가이드 출력 제한 레벨 보다 캐릭터 레벨이 높다면 가이드 출력하지 않음.
		if( g_pData != NULL && g_pData->GetSelectUnitLevel() > m_PlayGuide[eGuideType].m_iLimitLV )
		{
			return;
		}

		if( false == GetShowGuideByPlaceType(m_PlayGuide[eGuideType].m_ePlaceType) )
			return;
	}

	//모든 조건에 걸리지 않았으면 true로 설정
	m_PlayGuide[eGuideType].m_bShow   = bShow;
	if( true == bShow )
	{
		m_GuideOrderManager.InsertGuide(eGuideType);
	}
	else
	{
		m_GuideOrderManager.EraseGuide(eGuideType);
	}
}

/** @function : SetGuideString
	@brief	: 변수를 이용한 가이드 문자열 등록
	@param	: 가이드 타입(eGuideType), 문자열에 등록할 변수(iVal)
	@bool	: 문자열 등록 성공 true, 실패 false
*/
bool CX2PlayGuide::SetGuideString(PLAY_GUIDE_TYPE eGuideType, int iVal)
{
	switch(eGuideType)
	{
	case PGT_LEVEL_UP:
	case PGT_OBTAIN_SKILL_POINT:
		{
			m_PlayGuide[eGuideType].m_GuideString = GET_REPLACED_STRING( ( m_PlayGuide[eGuideType].m_iStrID, "i", iVal ) );
		}break;
	default:
		{
			return false;
		}break;
	}
	return true;

}
/** @function : SetGuideString
	@brief	: 변수를 이용한 가이드 문자열 등록
	@param	: 가이드 타입(eGuideType), 문자열에 등록할 변수(str)
	@bool	: 문자열 등록 성공 true, 실패 false
*/
bool CX2PlayGuide::SetGuideString(PLAY_GUIDE_TYPE eGuideType, wstring str)
{
	switch(eGuideType)
	{
	case PGT_QUEST_COMPLETE:
	case PGT_HYPER_MODE:
	case PGT_OBTAIN_EQUIP_ITEM:
	case PGT_OBTAIN_TITLE:
	case PGT_HAS_SKILL_POINT:
	case PGT_COMPLETE_EPIC_QUEST:
		{
			m_PlayGuide[eGuideType].m_GuideString = GET_REPLACED_STRING( ( m_PlayGuide[eGuideType].m_iStrID, "L", str ) );
		}break;
	default:
		{
			return false;
		}break;
	}

	return true;
}

/** @function : SetGuideString
	@brief	: 변수를 이용한 가이드 문자열 등록
	@param	: 가이드 타입(eGuideType), 문자열에 등록할 변수(str1~4)
	@bool	: 문자열 등록 성공 true, 실패 false
*/
bool CX2PlayGuide::SetGuideString(PLAY_GUIDE_TYPE eGuideType, wstring str1, wstring str2, wstring str3, wstring str4)
{
	switch(eGuideType)
	{
	case PGT_SPECIAL_ATTACK:
		{
			m_PlayGuide[eGuideType].m_GuideString = GET_REPLACED_STRING( ( m_PlayGuide[eGuideType].m_iStrID, "LLLL", str1, str2, str3, str4) );
		}break;
	default:
		{
			return false;
		}break;
	}
	return true;
}
/** @function : SetPlayGuide
	@brief	: 출력 할 가이드 설정
	@param	: 가이드 타입(eGuideType), 출력 여부(bShow), 가이드내용에 포함시킬 변수(iVal)	
*/
void CX2PlayGuide::SetPlayGuide( CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType, bool bShow, int iVal )
{
	SetShowGuide(eGuideType, bShow ) ;

	if( -1 == iVal )
		return;

	switch(eGuideType)
	{
	case PGT_LEVEL_UP:
	case PGT_OBTAIN_SKILL_POINT:
		{
			SetGuideString(eGuideType, iVal);
		}break;
	default:
		break;
	}
}

/** @function : SetPlayGuide
	@brief	: 출력 할 가이드 설정
	@param	: 가이드 타입(eGuideType), 출력 여부(bShow), 가이드내용에 포함시킬 변수(str1)	
*/
void CX2PlayGuide::SetPlayGuide( CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType, bool bShow, wstring str1, wstring str2, wstring str3, wstring str4)
{	
	SetShowGuide(eGuideType, bShow ) ;

	switch(eGuideType)
	{
	case PGT_QUEST_COMPLETE:
	case PGT_HYPER_MODE:
	case PGT_OBTAIN_EQUIP_ITEM:
	case PGT_OBTAIN_TITLE:
	case PGT_HAS_SKILL_POINT:
	case PGT_COMPLETE_EPIC_QUEST:
		{
			SetGuideString(eGuideType, str1);
		}break;
	case PGT_SPECIAL_ATTACK:
		{
			SetGuideString(eGuideType, str1, str2, str3, str4);
		}break;
	default:
		break;
	}
}

void CX2PlayGuide::SetVillageID(int eVillageID)
{
	m_PlayGuide[PGT_COMPLETE_EPIC_QUEST].m_eVillageID = eVillageID;
	switch( eVillageID )
	{
	case SEnum::VMI_ELDER:
		{
			SetGuideString(PGT_COMPLETE_EPIC_QUEST, GET_STRING(STR_ID_1114));			
		}break;
	case SEnum::VMI_BESMA:
		{
			SetGuideString(PGT_COMPLETE_EPIC_QUEST, GET_STRING(STR_ID_1116));	
		}break;
	case SEnum::VMI_ALTERA:
		{
			SetGuideString(PGT_COMPLETE_EPIC_QUEST, GET_STRING(STR_ID_1120));	
		}break;
	case SEnum::VMI_VELDER:
		{
			SetGuideString(PGT_COMPLETE_EPIC_QUEST, GET_STRING(STR_ID_4622));
		}break;
	case SEnum::VMI_HAMEL:
		{
			SetGuideString(PGT_COMPLETE_EPIC_QUEST, GET_STRING(STR_ID_10232));
		}break;
	default:
		{
			m_PlayGuide[PGT_COMPLETE_EPIC_QUEST].m_bShow = false;
		}break;
	}
}

void CX2PlayGuide::ResetNoviceGuide()
{
	if( true == m_bCompleteTutorial )
	{
#ifdef FIX_NOVICE_GUIDE_INITIALIZE
		m_GuideOrderManager.ClearGuideList();
#endif //FIX_NOVICE_GUIDE_INITIALIZE
		return;
	}

#ifdef CHECK_INHERIT_STATE_MENU
	if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
	{
		CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>(g_pMain->GetNowState());
		if( NULL != pStateMenu )
			pStateMenu->DeleteMouseClickUI();
	}

	m_uiCurrentGuideStep = NGS_STEP_NONE;
	m_uiGuideStep		 = NGS_STEP_NONE;
	m_bStart			 = false;
	m_bActiveNoviceGuide = false;
	m_bShowDungeonMenu   = false;
	//SetPlayGuide(CX2PlayGuide::PGT_NOVICE_GUIDE_01, false );
	SetPlayGuide(CX2PlayGuide::PGT_NOVICE_GUIDE_02, false );


	if( NULL != g_pData && NULL != g_pData->GetQuestManager() )
	{
		if( true == g_pData->GetQuestManager()->GetUnitCompleteQuest( TQI_CHASE_THIEF ) )
		{
			m_bShowDungeonMenu = true;
		}
		else
		{
			//첫번 째 에픽퀘스트가 없는 경우는 캐릭 생성 후 첫 마을 입장이라고 판단
			StartGoEffect();
		}

 		CX2QuestManager::QuestInst* pQuestInst = NULL;
		//도적단 추적		
		pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( TQI_CHASE_THIEF );
		if( NULL != pQuestInst )
		{
			m_bActiveNoviceGuide = true;
			if( true == pQuestInst->IsComplete() )
			{
				m_uiCurrentGuideStep = NGS_STEP_NONE;
				m_uiGuideStep		 = NGS_STEP_0;
				m_bStart			 = true;
				SetMouseClickIndex( m_uiGuideStep );
				SetPlayGuide(CX2PlayGuide::PGT_NOVICE_GUIDE_02, false );
				m_bShowDungeonMenu = true;
			}

			if( true == pQuestInst->GetSubQuest(11005)->IsComplete() ) //루벤숲 입장
			{
				SetPlayGuide(CX2PlayGuide::PGT_NOVICE_GUIDE_01, false );
				if( false ==  pQuestInst->GetSubQuest(11006)->IsComplete() )
				{
					SetPlayGuide(CX2PlayGuide::PGT_NOVICE_GUIDE_02, true );
#ifdef GUIDE_FIRST_EPIC_QUEST
					SetFirstEpicQuestGuide(EQGS_KILL_TOBU);
#endif //GUIDE_FIRST_EPIC_QUEST
				}
			}
			else
			{
				StartGoEffect();
				SetPlayGuide(CX2PlayGuide::PGT_NOVICE_GUIDE_01, true );
			}
			return;
		}

		//엘을 지켜라
		pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( TQI_SAVE_EL );
		if( NULL != pQuestInst )
		{
			m_bActiveNoviceGuide = true;

			if( NULL != pQuestInst->GetSubQuest(TQI_SAVE_EL) &&
				false == pQuestInst->GetSubQuest(TQI_SAVE_EL)->IsComplete() )
			{				
				SetPlayGuide(CX2PlayGuide::PGT_DUNGEON_PLAY, true); 
				m_uiCurrentGuideStep = NGS_STEP_NONE;
				m_uiGuideStep		 = NGS_STEP_5;
				m_bStart			 = true;
				SetMouseClickIndex( m_uiGuideStep );
			}
			else
			{
				SetPlayGuide(CX2PlayGuide::PGT_DUNGEON_PLAY, false); 
			}
			return;
		}	

		pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( TQI_VISIT_RUIN_OF_ELF );
		if( NULL != pQuestInst)
		{
			m_bActiveNoviceGuide = true;

			if( NULL != pQuestInst->GetSubQuest(TQI_VISIT_RUIN_OF_ELF) &&
				false == pQuestInst->GetSubQuest(TQI_VISIT_RUIN_OF_ELF)->IsComplete() )
			{
				SetMouseClickIndex( CX2PlayGuide::NGS_STEP_5 );		
			}
			return;
		}
	}
}

void CX2PlayGuide::SetMouseClickIndex(int val)
{ 
	if( false == GetActiveNoviceGuide() )
		return;

	if( m_uiCurrentGuideStep == val )
	{
		m_bStart = false;
		return;
	}

	m_uiCurrentGuideStep = val;

#ifdef CHECK_INHERIT_STATE_MENU
	if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
	{
		CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>(g_pMain->GetNowState());

		D3DXVECTOR2 vPos = D3DXVECTOR2(0.f, 0.f);
		CX2State::MouseClickUI::MOUSE_CLICK_STYLE arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_BOTTOM;


		if( pStateMenu != NULL )
		{
			switch(val)
			{
			case NGS_STEP_0: // 퀘스트 메뉴
				arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP;
				vPos = D3DXVECTOR2(783, 233);
				break;
			case NGS_STEP_1: // 에픽퀘스트 탭
				arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP;
				vPos = D3DXVECTOR2(135, 150);
				break;	
			case NGS_STEP_2: // 에픽퀘스트 완료목록
				vPos = D3DXVECTOR2(300, 135);
				break;
			case NGS_STEP_3: // 퀘스트 완료
				vPos = D3DXVECTOR2(230, 515);
				break;
			case NGS_STEP_4: // 퀘스트 보상 완료
				vPos = D3DXVECTOR2(220, 460);
				break;
			case NGS_STEP_5: // 던전도구
				vPos = D3DXVECTOR2(872, 125 );
				break;
			case NGS_STEP_6: // 엘의 나무
				arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP;
				vPos = D3DXVECTOR2(525, 435 );
				break;
			case NGS_STEP_7: // 던전신청
				arrowStyle = CX2State::MouseClickUI::MCS_FROM_LEFT_BOTTOM;
				vPos = D3DXVECTOR2(960, 474 );
				break;
			case NGS_STEP_8: // 숲속의 폐허
				arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP;
				vPos = D3DXVECTOR2(390, 620 );
				break;
			case NGS_STEP_END:
			default:
				pStateMenu->DeleteMouseClickUI();
				return;
			}

			pStateMenu->CreateMouseClickUI( (CKTDXStage*) pStateMenu, vPos, 0.5f, 1000, arrowStyle );
			return;
		}
	}
}
#ifdef GUIDE_FIRST_EPIC_QUEST
void CX2PlayGuide::OnFrameMove_FirstEpicGuide(float fElapsedTime)
{
	if( true == m_bShowFirstEpicQuestGuide )
	{
		if( NULL != m_pDLGUIFirstEpicQuestGuide )
		{
			switch( g_pMain->GetNowStateID())
			{
			case CX2Main::XS_VILLAGE_MAP:
			case CX2Main::XS_BATTLE_FIELD:
				{
					if( m_fEpicGuideDelayTime > 0.f )
						m_fEpicGuideDelayTime -= fElapsedTime;
					else
					{
						m_pDLGUIFirstEpicQuestGuide->SetShow(true);
						
						m_fEpicGuideAlphaTime += fElapsedTime;
						

						float fAlpha = 0.f;

						bool bShow = true;

						//던전 또는 대전 매칭 중일 때 보이지 않도록
						if(NULL != g_pData && NULL != g_pData->GetPartyManager() &&
							(true == g_pData->GetPartyManager()->GetProcessDungeonMatch() ||
							true == g_pData->GetPartyManager()->GetProcessPvpMatch() ))
						{
							bShow = false;							
						}

						if( NULL != g_pData && NULL != g_pData->GetCashShop() &&
							true == g_pData->GetCashShop()->GetOpen())
						{
							bShow = false;
						}

						if( true == bShow )
						{
							if( m_fEpicGuideAlphaTime < 3.f ) 
								fAlpha = 1.f;
							else if ( m_fEpicGuideAlphaTime < 4.f )
								fAlpha = 1.f - (m_fEpicGuideAlphaTime - 3.f);  // (m_fEpicGuideAlphaTime -3.f) : 0~1
							else if ( m_fEpicGuideAlphaTime < 14.f )
								fAlpha = 0.f;
							else if ( m_fEpicGuideAlphaTime < 15.f )
								fAlpha = (m_fEpicGuideAlphaTime - 14.f);  // (m_fEpicGuideAlphaTime -14.f) : 0~1
							else if ( m_fEpicGuideAlphaTime < 16.f )
							{
								m_fEpicGuideAlphaTime = 0.f;
							}
						}

						m_pDLGUIFirstEpicQuestGuide->SetColor(D3DXCOLOR(1,1,1,fAlpha));
					}
				} break;
			default:
				{
					m_pDLGUIFirstEpicQuestGuide->SetShow(false);
					m_bShowFirstEpicQuestGuide = false;
				} break;
			}
		}
	}
}
#endif //GUIDE_FIRST_EPIC_QUEST

void CX2PlayGuide::OnFrameMove_Tutorial()
{
	if( false == g_pMain->GetIsPlayingTutorial() )
		return;

	//튜토리얼 던전에서의 퀘스트 가이드 파티클
	if( NULL == g_pX2Game )
		return;

	CreateQuestGuideParticle();
	CreateInputGuide();

	if( NULL != g_pX2Game->GetMyUnit() )
	{
		float fMyPosX = g_pX2Game->GetMyUnit()->GetLandPosition().x;

		if( fMyPosX >= -132 && fMyPosX <= 3390 )
		{
			SetShowGuideParticle(true);
		}
		else
		{
			SetShowGuideParticle(false);
		}
		SetPostionInputGuide(g_pX2Game->GetMyUnit()->GetHeadBonePos());
	}


}

void CX2PlayGuide::CreateQuestGuideParticle()
{
	if( NULL == m_pDLGParticle )
	{//파티클의 레이어를 관리 하기 위해 다이얼로그 생성
		m_pDLGParticle = new CKTDGUIDialog( g_pMain->GetNowState(), NULL);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGParticle );
		m_pDLGParticle->SetLayer(XDL_NORMAL_1);
		m_pDLGParticle->SetFront(true);
		m_pDLGParticle->SetShow(true);
	}

	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hIndicator && INVALID_PARTICLE_SEQUENCE_HANDLE == m_hIndicatorMark )
	{
		if( NULL != g_pData && NULL != g_pData->GetUIManager() )
		{
			//방향 가리키는 화살표 파티클
			m_hIndicator = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"NPCTargetArrowEpic", 70, 450, 0);	
			//퀘스트 마크 파티클
			m_hIndicatorMark = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"NPCTargetQuestion", 70, 450, 0);	

			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hIndicator )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence(m_hIndicator);
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(967.f,330.f,0.0f) );
					pSeq->SetShowObject(false);

#ifdef PARTICLE_RENDER_BY_DIALOG
					if( NULL != m_pDLGParticle )
					{
						m_pDLGParticle->AddParticle(static_cast<CKTDGObject*>(pSeq));
					}
#endif //PARTICLE_RENDER_BY_DIALOG
				}
			}

			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hIndicatorMark )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence(m_hIndicatorMark);
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(967.f,330.f,0.0f) );
					pSeq->SetShowObject(false);

#ifdef PARTICLE_RENDER_BY_DIALOG
					if( NULL != m_pDLGParticle )
					{
						m_pDLGParticle->AddParticle(static_cast<CKTDGObject*>(pSeq));
					}
#endif //PARTICLE_RENDER_BY_DIALOG
				}
			}
		}
	}
}

void CX2PlayGuide::DeleteQuestGuideParticle()
{

	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hIndicator )
	{
		if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hIndicator);
		}
	}

	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hIndicatorMark )
	{
		if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hIndicatorMark);
		}
	}

	SAFE_DELETE_DIALOG(m_pDLGParticle);
}
void CX2PlayGuide::DeleteInputGuideParticle()
{
	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hInputGuide )
	{
		if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hInputGuide);
		}
	}
}

void CX2PlayGuide::SetShowGuideParticle(bool bVal)
{
	if( m_bShowGuideParticle == bVal )
		return;

	m_bShowGuideParticle = bVal;
	if( NULL != g_pData && NULL != g_pData->GetUIMinorParticle() )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence(m_hIndicator);
		if( NULL != pSeq)
		{
			pSeq->SetShowObject(bVal);
		}
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence(m_hIndicatorMark);
		if( NULL != pSeq)
		{
			pSeq->SetShowObject(bVal);
		}
	}
}
void CX2PlayGuide::CreateInputGuide()
{
	if( NULL == g_pX2Game->GetMyUnit() )
		return;

	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hInputGuide )
		return;

	if( NULL != g_pData && NULL != g_pData->GetUIMinorParticle() )
	{
		m_hInputGuide = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"InputGuide",0,0,0);

		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hInputGuide )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence(m_hInputGuide);
			if( NULL != pSeq )
			{				
				pSeq->CreateNewParticle( g_pX2Game->GetMyUnit()->GetHeadBonePos() + D3DXVECTOR3(0.f, 50.f, 0.f));				
				pSeq->SetOverUI(false);
				pSeq->SetLayer(12);
				pSeq->SetShowObject(true);
				
			}
		}
	}

}

void CX2PlayGuide::SetPostionInputGuide(const D3DXVECTOR3& vPos)
{
	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hInputGuide )
	{
		CreateInputGuide();
		return;
	}

	D3DXVECTOR2 finalPos = g_pKTDXApp->GetProj3DPos( vPos );
	float fOffsetY	=350.f * g_pKTDXApp->GetResolutionScaleY();

	CKTDGParticleSystem::CParticleEventSequence* pInputGuide = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hInputGuide );
	pInputGuide->SetPosition(D3DXVECTOR3(finalPos.x, finalPos.y - fOffsetY, 0.f));
}

void CX2PlayGuide::ClearPlayGuide()
{
	m_GuideOrderManager.ClearGuideList();
	if( NULL != m_pDLGPlayGuide )
	{
		m_pDLGPlayGuide->SetShowEnable(false,false);
	}
}

bool CX2PlayGuide::IsShowGuide()
{
	//이벤트씬등으로 전체 UI가 안보이도록 하는 설정
	if( true == m_bHide )
	{
		return false;
	}

	//F1 도움말 토글로 Hide 설정
	if( false == g_pMain->GetGameOption().GetOptionList().m_bPlayGuide )
	{
		return false;
	}

	//PVP, DUNGEON RESULT 체크
	switch(g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_PVP_RESULT:
	case CX2Main::XS_DUNGEON_RESULT:
		{
			return false;
		} break;
	default:
		break;
	}


	if( NULL != g_pData && NULL != g_pData->GetCashShop() )
	{
		if( true == g_pData->GetCashShop()->GetOpen() )
		{
			return false ;
		}
	}

	return true;
}

//================================================================================================================================
CX2PlayGuide::PlayGuideData::PlayGuideData():
m_eVillageID(SEnum::VMI_INVALID),
m_eGuideType(PGT_NONE),
m_ePlaceType(GPT_NONE),
m_iStrID(0),
m_iLimitLV(0),	
m_bRepeat(true),
m_bShow(false),
m_GuideString(L"")
{
}

void CX2PlayGuide::PlayGuideData::SetPlayGuide(CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType, CX2PlayGuide::GUIDE_PLACE_TYPE ePlaceType,
	int iStrID, int iLimitLV, bool bRepeat)
{
	m_eGuideType	= eGuideType;
	m_ePlaceType	= ePlaceType;
	m_iStrID		= iStrID;
	m_iLimitLV		= iLimitLV;
	m_bRepeat		= bRepeat;
	m_GuideString	= GET_STRING(m_iStrID);
}

CX2PlayGuide::PLAY_GUIDE_TYPE CX2PlayGuide::GuideOrderManager::GetNextGuide()
{
	//현재 출력중인 가이드가 종료되지 않았으면 다음 가이드 보여주지 않기.
	if( true == m_setGuideList.empty() )
		return PGT_NONE;

	if( false == m_bEndCurrentGuide && m_eCurrentGuide != PGT_NONE )
		return PGT_NONE;

	m_bEndCurrentGuide = false;



	m_iShowCount++;
	if( m_iShowCount < 2 )
	{
		return m_eCurrentGuide;
	}
	else
	{
		m_iShowCount = 0;
	}

	std::set<PLAY_GUIDE_TYPE>::iterator it = m_setGuideList.find(m_eCurrentGuide);
	if( it != m_setGuideList.end() )
	{
		if( ++it == m_setGuideList.end() ) 
		{
			it = m_setGuideList.begin();
		}

		return m_eCurrentGuide = *it;
	}
	else
	{
		it = m_setGuideList.begin();
		return m_eCurrentGuide = *it;
	}

	return PGT_NONE;
}

void CX2PlayGuide::GuideOrderManager::InsertGuide(const PLAY_GUIDE_TYPE ePlayGuide_)
{
	m_setGuideList.insert(ePlayGuide_);
	if( PGT_NONE == m_eCurrentGuide ||
		m_setGuideList.find(m_eCurrentGuide) == m_setGuideList.end())
	{
		m_eCurrentGuide = ePlayGuide_;
		m_bEndCurrentGuide = true;
	}
}

/** @funciton : EraseGuide
	@brief	: 가이드 목록에 있는 내용 제거.
			  현재 존재하는 가이드라면, 이전 가이드를 현재가이드로 지정하고 제거
	@param	: 제거 할 가이드 (ePlayGuide_)
*/
void CX2PlayGuide::GuideOrderManager::EraseGuide(const PLAY_GUIDE_TYPE ePlayGuide_)
{
	if( PGT_NONE == ePlayGuide_ )
		return;

	std::set<PLAY_GUIDE_TYPE>::iterator it = m_setGuideList.find(ePlayGuide_);
	if( it != m_setGuideList.end() )
	{
		//지우려고 하는 가이드가 현재 출력중인 가이드라면
		if( ePlayGuide_ == m_eCurrentGuide )
		{
			if( it == m_setGuideList.begin() )
			{
				m_setGuideList.erase(ePlayGuide_);
			}
			else
			{
				m_eCurrentGuide = *(--it);
				m_setGuideList.erase(ePlayGuide_);
			}
		}
		else
		{
			m_setGuideList.erase(ePlayGuide_);
		}
	}
}


/** @funciton : GetGuideListEmpty
	@brief	: 출력 할 가이드 리스트 소지 여부 확인
	@param	: 출력 할 가이드 있으면 true, 없으면 false
*/
bool CX2PlayGuide::GuideOrderManager::GetGuideListEmpty() const
{
	return m_setGuideList.empty();
}

void CX2PlayGuide::StartGoEffect()
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hParticleGo &&
		CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() )
	{
		if( NULL != g_pData->GetUIMinorParticle() )
		{
			m_hParticleGo = g_pData->GetUIMinorParticle()->CreateSequenceHandle( NULL,  L"SubStageGoRight_UI", 850.0f, 300.0f, 0.0f, 1, 1 );
		}
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Go.ogg", true, false );
	}
}

void CX2PlayGuide::StopGoEffect()
{
	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hParticleGo ) 
	{
		g_pKTDXApp->GetDeviceManager()->StopSound( L"Go.ogg" );
		if( NULL != g_pData &&
			NULL != g_pData->GetUIMinorParticle() )
		{
			g_pData->GetUIMinorParticle()->DestroyInstanceHandle( m_hParticleGo );
		}
	}
}

#ifdef GUIDE_FIRST_EPIC_QUEST
/** @funtion : SetFirstEpicQuestGuide
	@brief : 첫 에픽 퀘스트에 대한 추가 가이드
	@param : iIndex 
	bKillNPC_ : true = 토부 잡는 퀘스트, false = 노아스 호수 입장 퀘스트
*/
void CX2PlayGuide::SetFirstEpicQuestGuide( EPIC_QUEST_GUIDE_STEP GuideStep_ )
{
	if( NULL != g_pData && g_pData->GetSelectUnitLevel() >= 10 )
		return;

	if( NULL == m_pDLGUIFirstEpicQuestGuide )
	{
		m_pDLGUIFirstEpicQuestGuide = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Quest_Guide.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIFirstEpicQuestGuide );
		m_pDLGUIFirstEpicQuestGuide->SetShow(false);
	}

	if ( NULL == m_pDLGUIFirstEpicQuestGuide )
		return;

	m_pDLGUIFirstEpicQuestGuide->SetShow(false);

	CKTDGUIStatic* pStaticVisitField = 
		static_cast<CKTDGUIStatic*>(m_pDLGUIFirstEpicQuestGuide->GetControl(L"Quest_Visit_Noas_Field"));
	if( NULL == pStaticVisitField )
		return;

	CKTDGUIStatic* pStaticKillTobu = 
		static_cast<CKTDGUIStatic*>(m_pDLGUIFirstEpicQuestGuide->GetControl(L"Quest_Kill_NPC"));
	if( NULL == pStaticKillTobu )
		return;

	CKTDGUIStatic* pStaticEnterElTree = 
		static_cast<CKTDGUIStatic*>(m_pDLGUIFirstEpicQuestGuide->GetControl(L"Quest_Enter_ElTree"));
	if( NULL == pStaticEnterElTree )
		return;

	CKTDGUIStatic* pStaticEnterDungeon = 
		static_cast<CKTDGUIStatic*>(m_pDLGUIFirstEpicQuestGuide->GetControl(L"Quest_Enter_Dungeon"));
	if( NULL == pStaticEnterDungeon )
		return;

	CKTDGUIStatic* pStaticKillThiefPporu = 
		static_cast<CKTDGUIStatic*>(m_pDLGUIFirstEpicQuestGuide->GetControl(L"Quest_Kill_Thief_Pporu"));
	if( NULL == pStaticKillThiefPporu )
		return;

	CKTDGUIStatic* pStaticKillBorr = 
		static_cast<CKTDGUIStatic*>(m_pDLGUIFirstEpicQuestGuide->GetControl(L"Quest_Kill_Borr"));
	if( NULL == pStaticKillBorr)
		return;

	CKTDGUIStatic* pStaticQMarkGuide = 
		static_cast<CKTDGUIStatic*>(m_pDLGUIFirstEpicQuestGuide->GetControl(L"Quest_QMark_Guide"));
	if( NULL == pStaticQMarkGuide)
		return;

	pStaticVisitField->SetShow(false);
	pStaticKillTobu->SetShow(false);
	pStaticEnterElTree->SetShow(false);
	pStaticEnterDungeon->SetShow(false);
	pStaticKillThiefPporu->SetShow(false);
	pStaticKillBorr->SetShow(false);
	pStaticQMarkGuide->SetShow(false);

	m_fEpicGuideDelayTime = 4.5f;
	switch(GuideStep_)
	{
	case EQGS_VISIT_NOAS: //노아스 호스 입장 
		{
			pStaticVisitField->SetShow(true);
		} break;
	case EQGS_KILL_TOBU: //토부 잡는 퀘스트
		{
			pStaticKillTobu->SetShow(true);
		} break;
	case EQGS_ENTER_EL_TREE: //엘의 나무 입장 퀘스트
		{
			pStaticEnterElTree->SetShow(true);
		} break;
	case EQGS_ENTER_DUNGEON: //던전 입장 가이드(공용)
		{
			pStaticEnterDungeon->SetShow(true);
		} break;
	case EQGS_KILL_THIEF_PPORU: //도적 뽀루 킬 가이드
		{
			pStaticKillThiefPporu->SetShow(true);
		} break;
	case EQGS_KILL_BORR: //보르 킬 가이드
		{
			pStaticKillBorr->SetShow(true);
		} break;
	case EQGS_QMARK_GUIDE://Q마크를 따라 이동하여 퀘스트를 수행하세요
		{
			pStaticQMarkGuide->SetShow(true);
		} break;
	}

	m_bShowFirstEpicQuestGuide = true;
}
#endif //GUIDE_FIRST_EPIC_QUEST

void CX2PlayGuide::SetShowPlayGuide(bool bVal_)
{
	if( NULL != m_pDLGPlayGuide )
		m_pDLGPlayGuide->SetShow(bVal_);

#ifdef GUIDE_FIRST_EPIC_QUEST
	if( NULL != m_pDLGUIFirstEpicQuestGuide )
		m_pDLGUIFirstEpicQuestGuide->SetShow(false);
#endif //GUIDE_FIRST_EPIC_QUEST
}

#ifdef GUIDE_FIRST_EPIC_QUEST
void CX2PlayGuide::SetShowFirstEpicQuestGuide(bool bShow_)
{
	if( NULL == m_pDLGUIFirstEpicQuestGuide )
		return;

	m_pDLGUIFirstEpicQuestGuide->SetShow(bShow_);
	m_bShowFirstEpicQuestGuide = bShow_;
}
#endif //GUIDE_FIRST_EPIC_QUEST
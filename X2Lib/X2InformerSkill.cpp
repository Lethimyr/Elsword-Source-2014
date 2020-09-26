#include "StdAfx.h"
#include ".\x2informerskill.h"


// dmlee, 2008.11.24, "New" 표시로 대체됨, 사용안되게 comment out


CX2InformerSkill::CX2InformerSkill(void)
{
	m_InformerType				= CX2Informer::XIT_SKILL;

#ifdef SERV_PET_SYSTEM
#ifdef INT_WIDE_BAR
	m_vTalkBoxPos				= D3DXVECTOR2( 850, 702 );
#else // INT_WIDE_BAR
	m_vTalkBoxPos				= D3DXVECTOR2( 770, 702 );
#endif // INT_WIDE_BAR
#else
	m_vTalkBoxPos				= D3DXVECTOR2( 389, 711 );
#endif

	m_bCheckFirstConnect = false;

	m_fTimeCheckIsNowInfoChange	= 0.0f;
	m_NowIsPossibleLearnSkillNum = 0;
}

CX2InformerSkill::~CX2InformerSkill(void)
{
}

HRESULT CX2InformerSkill::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( g_pMain->GetNowStateID() != CX2Main::XS_DUNGEON_GAME )
	{
		m_fTimeCheckIsNowInfoChange += fElapsedTime;
		return CX2Informer::OnFrameMove( fTime, fElapsedTime );
	}

	return S_OK;
}

void CX2InformerSkill::Reset()
{
	CX2Informer::Reset();
	
	m_NowIsPossibleLearnSkillNum = 0;


	m_bCheckFirstConnect = false;
}

void CX2InformerSkill::SetNowInfo()
{
	//if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	//{
	//	m_NowIsPossibleLearnSkillNum = g_pData->GetSkillTree()->GetPossibleLearnSkillNum();
	//}
}

bool CX2InformerSkill::IsNowInfoChange()
{
	//if ( m_fTimeCheckIsNowInfoChange > 1.0f )
	//{
	//	m_fTimeCheckIsNowInfoChange = 0.0f;

	//	if ( m_bCheckFirstConnect == false )
	//	{
	//		m_bCheckFirstConnect = true;

	//		if ( m_NowIsPossibleLearnSkillNum >= 1 )
	//			return true;
	//	}

	//	if ( g_pData->GetSkillTree()->GetPossibleLearnSkillNum() > m_NowIsPossibleLearnSkillNum )
	//	{
	//		return true;
	//	}
	//}

	return false;
}

void CX2InformerSkill::SetShow( bool bShow )
{
	//CX2Informer::SetShow( bShow );
	//if ( bShow == true )
	//{
	//	CX2TalkBoxManagerImp::TalkBox talkBox;


	//	talkBox.m_vPos				= m_vTalkBoxPos;
	//	talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;
	//	talkBox.m_fRemainTime		= 99999.0f;

	//	talkBox.m_wstrTalkContent	= L"새로운 스킬을 익힐 수 있습니다!!";
	//	talkBox.m_bTraceUnit		= false;
	//	talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_RIGHT;
	//	talkBox.m_coTextColor		= D3DXCOLOR(0,0,0,1);

	//	g_pKTDXApp->GetDeviceManager()->PlaySound( L"MessageAlarm.ogg", false, false );

	//	m_pTalkBoxManager->Push( talkBox );
	//}
	//else
	//{
	//	m_pTalkBoxManager->Clear();
	//}
}

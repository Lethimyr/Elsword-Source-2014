#include "StdAfx.h"
#include ".\x2informercashshop.h"

CX2InformerCashShop::CX2InformerCashShop(void)
{
	m_InformerType				= CX2Informer::XIT_CASH_SHOP;
	
#ifdef SERV_PET_SYSTEM
	m_vTalkBoxPos				= D3DXVECTOR2( 1000, 702 );
#else
	m_vTalkBoxPos				= D3DXVECTOR2( 609, 711 );
#endif
	m_bCheckFirstConnect = false;
	m_fTimeCheckIsNowInfoChange	= 0.0f;
	m_NowArrivedPresentNum = 0;

#ifdef CASHSHOP_CLASS_CHANGE_NOTIFICATION
	m_bCanClassChange = false;
	m_LastCheckLevel = 0;
#endif CASHSHOP_CLASS_CHANGE_NOTIFICATION
}

CX2InformerCashShop::~CX2InformerCashShop(void)
{

}

HRESULT CX2InformerCashShop::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( g_pMain->GetNowStateID() != CX2Main::XS_DUNGEON_GAME )
	{
		m_fTimeCheckIsNowInfoChange += fElapsedTime;
		return CX2Informer::OnFrameMove( fTime, fElapsedTime );
	}

	return S_OK;
}

void CX2InformerCashShop::Reset()
{
	CX2Informer::Reset();
	m_bCheckFirstConnect = false;

#ifdef CASHSHOP_CLASS_CHANGE_NOTIFICATION
	m_LastCheckLevel = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level;
	m_bCanClassChange = false;
#endif CASHSHOP_CLASS_CHANGE_NOTIFICATION
}

void CX2InformerCashShop::SetNowInfo()
{
}

bool CX2InformerCashShop::IsNowInfoChange()
{
	if ( m_fTimeCheckIsNowInfoChange > 1.0f )
	{
		m_fTimeCheckIsNowInfoChange = 0.0f;

		if ( m_bCheckFirstConnect == false )
		{
			m_bCheckFirstConnect = true;

			if ( g_pData->GetArrivedPresentNum() >= 1 )
			{
				m_NowArrivedPresentNum = g_pData->GetArrivedPresentNum();
				g_pData->SetArrivedPresentNum( 0 );
				return true;
			}
		}

#ifdef CASHSHOP_CLASS_CHANGE_NOTIFICATION
		if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

			if( 15 > m_LastCheckLevel && 15 <= pUnit->GetUnitData().m_Level ||
				35 > m_LastCheckLevel && 35 <= pUnit->GetUnitData().m_Level )
			{
				m_bCanClassChange = true;
			}
			else
			{
				m_bCanClassChange = false;
			}

			m_LastCheckLevel = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level;

			return m_bCanClassChange;
		}
#endif CASHSHOP_CLASS_CHANGE_NOTIFICATION

	}

	return false;
}

void CX2InformerCashShop::SetShow( bool bShow )
{
	CX2Informer::SetShow( bShow );
	if ( bShow == true )
	{
		CX2TalkBoxManagerImp::TalkBox talkBox;

		//캐릭터 상태에 따라 메뉴 버튼의 위치가 달라 지기 때문에 위치 수정
		talkBox.m_vPos				= m_vTalkBoxPos+m_vTalkBoxOffsetPos;
		talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;
		talkBox.m_fRemainTime		= 99999.0f;

		
		wstringstream wstrstm;
        wstrstm << GET_REPLACED_STRING( ( STR_ID_228, "i", m_NowArrivedPresentNum) );
		talkBox.m_wstrTalkContent	= wstrstm.str().c_str();

#ifdef CASHSHOP_CLASS_CHANGE_NOTIFICATION
		if ( m_bCanClassChange == true )
		{
			talkBox.m_OwnerUnitUID		= XUUII_CLASS_CHANGE;
			talkBox.m_wstrTalkContent	= GET_STRING( STR_ID_15579 );
		}
		else
		{
			talkBox.m_OwnerUnitUID		= XUUII_PRESENT;
		}
#endif CASHSHOP_CLASS_CHANGE_NOTIFICATION

		talkBox.m_bTraceUnit		= false;
		talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_RIGHT;
		talkBox.m_coTextColor		= D3DXCOLOR(0,0,0,1);

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"MessageAlarm.ogg", false, false );

		m_pTalkBoxManager->Push( talkBox );
	}
	else
	{
		m_pTalkBoxManager->Clear();
	}
}

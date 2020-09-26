
#include "StdAfx.h"
#include "X2UIGlobalMission.h"

#ifdef SERV_GLOBAL_MISSION_MANAGER
CX2UIGlobalMission::CX2UIGlobalMission() :
m_pDlgGlobalMissionDlg( NULL )
, m_iNumOfNowClearCount( 0 )
, m_iNumOfClearCount( 0 )
{
}

CX2UIGlobalMission::~CX2UIGlobalMission()
{
	SAFE_DELETE_DIALOG(m_pDlgGlobalMissionDlg);
}

void CX2UIGlobalMission::SetShowGlobalMissionDlg( bool bShow )
{
	if ( true == bShow )
	{
		if(m_pDlgGlobalMissionDlg == NULL)
		{
			m_pDlgGlobalMissionDlg = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Global_Mission.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgGlobalMissionDlg );
			m_pDlgGlobalMissionDlg->SetLayer(XDL_NORMAL);
		}

		m_pDlgGlobalMissionDlg->SetShowEnable( bShow, bShow );		
	}
	else
	{
		if(m_pDlgGlobalMissionDlg != NULL)
		{
			m_pDlgGlobalMissionDlg->SetShowEnable( bShow, bShow );
		}
	}
}

bool CX2UIGlobalMission::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	default:
		break;
	}
	
	return false;
}

void CX2UIGlobalMission::UpdateNumOfNowClearCount( int iNumOfNowClearCount )
{
	// 이전의 UpdateTimeDlg에서 NULL 인 경우 다시 생성을 했으므로, 디버그 모드에서만 체크함
	ASSERT( m_pDlgGlobalMissionDlg != NULL );

	if ( GetNumOfNowClearCount() != iNumOfNowClearCount )
	{
		// 여기서 클리어 카운트 갱신
		SetNumOfNowClearCount( iNumOfNowClearCount );
	}

	float fPercent = static_cast<float>(static_cast<float>(m_iNumOfNowClearCount) / static_cast<float>(m_iNumOfClearCount));
	CKTDGUIStatic* pStaticGagebar = static_cast<CKTDGUIStatic*>( m_pDlgGlobalMissionDlg->GetControl( L"time" ) );
	if ( NULL == pStaticGagebar )
	{
		ASSERT( ! L"Static GageBar Is NULL" );
		return;
	}

	CKTDGUIControl::CPictureData* pPictureGagebar = pStaticGagebar->GetPicture( 6 );
	CKTDGUIControl::CPictureData* pPictureGage = pStaticGagebar->GetPicture( 7 );
	D3DXVECTOR2 vGagebarSize = pPictureGagebar->GetSize();

	if( fPercent > 1.0f )
	{
		fPercent = 1.0f;
	}

	pPictureGage->SetSizeX(fPercent * vGagebarSize.x);

	CKTDGUIStatic* pStaticPiratePercent = static_cast<CKTDGUIStatic*>(m_pDlgGlobalMissionDlg->GetControl(L"PiratePercent"));
	int iPercent = static_cast<int>(fPercent * 100.0f);

	if( pStaticPiratePercent != NULL )
	{
#ifdef SERV_SECOND_CLASS_WORLD_WAR_EVENT
		int iRemainCount = ( m_iNumOfNowClearCount > m_iNumOfClearCount ) ? 0 : ( m_iNumOfClearCount - m_iNumOfNowClearCount );	
		wstring wstrRemainCount = L"";
		WCHAR wszRemainCount[10] = {0, };
		_itow(iRemainCount, wszRemainCount, 10);
		wstrRemainCount = wszRemainCount;

		pStaticPiratePercent->GetString(0)->msg = wstrRemainCount;
#else
		wstring wstrPercent = L"";
		WCHAR wszPercent[10] = {0, };
		_itow(iPercent, wszPercent, 10);
		wstrPercent = wszPercent;
		wstrPercent += L"%";
	
		if( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
		{
			WCHAR wszNowClearCount[7];
			WCHAR wszClearCount[7];
			_itow(m_iNumOfNowClearCount, wszNowClearCount, 10);
			_itow(m_iNumOfClearCount, wszClearCount, 10);
			wstrPercent += L" ";
			wstrPercent += wszNowClearCount;
			wstrPercent += L"/";
			wstrPercent += wszClearCount;
		}

		pStaticPiratePercent->GetString(0)->msg = wstrPercent;
#endif SERV_SECOND_CLASS_WORLD_WAR_EVENT
	}
}

void CX2UIGlobalMission::UpdateNumOfClearCount( int iNumOfClearCount )
{
	// 이전의 UpdateTimeDlg에서 NULL 인 경우 다시 생성을 했으므로, 디버그 모드에서만 체크함
	ASSERT( m_pDlgGlobalMissionDlg != NULL );

	if ( GetNumOfClearCount() != iNumOfClearCount )
	{
		// 여기서 클리어 카운트 갱신
		SetNumOfClearCount( iNumOfClearCount );
	}
}
#endif SERV_GLOBAL_MISSION_MANAGER



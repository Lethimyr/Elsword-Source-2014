#include "StdAfx.h"
#include ".\x2informer.h"

CX2Informer::CX2Informer(void)
{
	m_pTalkBoxManager = CX2TalkBoxManager::CreateTalkBoxManger();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pTalkBoxManager );
	m_pTalkBoxManager->SetOverUI( true );

	m_vTalkBoxPos				= D3DXVECTOR2(0,0);
	m_bCheckInfo				= false;

	m_bMouseDown				= false;

	m_pSoundAlarm				= g_pKTDXApp->GetDeviceManager()->OpenSound( L"MessageAlarm.ogg" );

#ifdef EVENT_SCENE
	m_bSetShowAfterEventSceneEnd		= false;
#endif EVENT_SCENE
	m_vTalkBoxOffsetPos			= D3DXVECTOR2(0,0);

	SetNeedCheckInfo( true );

	m_InformerType = XIT_SKILL;
}

CX2Informer::~CX2Informer(void)
{
	CX2TalkBoxManager::DeleteKTDGObject( m_pTalkBoxManager );
	SAFE_CLOSE( m_pSoundAlarm );
}

bool CX2Informer::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( m_pTalkBoxManager->MsgProc( hWnd, uMsg, wParam, lParam) == true )
		return true;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		{
			UidType unitUID = 0;

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			if ( m_pTalkBoxManager->CheckMousePointInTalkbox( mousePos, unitUID ) == true && m_pTalkBoxManager->GetTalkBoxNum() != 0 )
			{
				m_bMouseDown = true;
				return true;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			UidType unitUID = 0;
			bool bCheck = false;

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			if ( m_pTalkBoxManager->CheckMousePointInTalkbox( mousePos, unitUID ) == true && m_bMouseDown == true )
			{
				InvalidInfo();
				bCheck = true;
			}

			m_bMouseDown = false;

			if ( bCheck == true )
				return true;
		}
		break;
	}
	
	return false;
}

HRESULT CX2Informer::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( GetNeedCheckInfo() )
	{
		CheckInfo( false );
		SetNeedCheckInfo( false );	/// 체크 했으니 다시 체크 할 필요 없다고 셋팅
	}

#ifdef EVENT_SCENE
	if(g_pData->GetEventScene()->GetIsPlayingEventScene())
	{
		if(m_pTalkBoxManager->GetShowTalk())
		{
			m_pTalkBoxManager->SetShowTalk( false );
			m_bSetShowAfterEventSceneEnd = true;
		}
	}
	else if(m_bSetShowAfterEventSceneEnd)
	{
		m_pTalkBoxManager->SetShowTalk( true );
		m_bSetShowAfterEventSceneEnd = false;
	}
#endif EVENT_SCENE

	return m_pTalkBoxManager->OnFrameMove( fTime, fElapsedTime );
}

void CX2Informer::Reset()
{
	m_bCheckInfo = false;
}

void CX2Informer::CheckInfo( bool bStateChange )
{
	if ( bStateChange == true ) 
	{
		if ( m_bCheckInfo == true ) //이전에 정보가 활성화되 있었다.
		{
			//혹시 뭐가 바꼈을 지도 모르니까 체크
			if ( IsNowInfoChange() == true )
			{
				//켜둔채로 그대로 둔다.
				SetNowInfo();
				SetShow( true );
			}
			else
			{
				InvalidInfo();
			}
		}
		else
		{
			if ( IsNowInfoChange() == true )
			{
				m_bCheckInfo = true;
				SetNowInfo();
				SetShow( true );
			}
			else
			{
				InvalidInfo();
			}
		}
	}
	else
	{
		if ( IsNowInfoChange() == true )
		{
			//켜둔채로 그대로 둔다.
			SetShow( true );
			m_bCheckInfo = true;
			SetNowInfo();
		}
	}
}

void CX2Informer::InvalidInfo()
{
	SetShow( false );
}

void CX2Informer::SetShow( bool bShow )
{
	
	
}


#include "StdAfx.h"
#include ".\x2talkboxmanager.h"


CX2TalkBoxManager::CX2TalkBoxManager( CX2TalkBoxManager::TALK_BOX_MANAGER_TYPE talkBoxManagerType )
{
	m_TalkBoxManagerType = talkBoxManagerType;
	SetAlphaObject( false );
	m_pTalkBoxManagerImp = NULL;	
	
	switch(m_TalkBoxManagerType)
	{
	case CX2TalkBoxManager::TBMT_DEFAULT:
		m_pTalkBoxManagerImp = new CX2TalkBoxManagerImp();
		break;
	case CX2TalkBoxManager::TBMT_PERSONAL_SHOP:
		m_pTalkBoxManagerImp = new CX2TalkBoxManagerImpPShop();
		break;
	case CX2TalkBoxManager::TBMT_PARTY_INFO:
		m_pTalkBoxManagerImp = new CX2TalkBoxManagerImpPartyLeader();
		break;
	default:
		m_pTalkBoxManagerImp = new CX2TalkBoxManagerImp();
		break;
	}

	m_bShowTalk = true;
}

CX2TalkBoxManager::~CX2TalkBoxManager(void)
{
	SAFE_DELETE( m_pTalkBoxManagerImp );
}

bool CX2TalkBoxManager::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( m_pTalkBoxManagerImp != NULL )
	{
		if ( m_pTalkBoxManagerImp->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}

	return false;
}

HRESULT CX2TalkBoxManager::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if ( m_pTalkBoxManagerImp != NULL )
		m_pTalkBoxManagerImp->OnFrameMove( fTime, fElapsedTime );

	return S_OK;
}

//{{ robobeg : 2008-10-28
//HRESULT CX2TalkBoxManager::OnFrameRender()
/*virtual*/
void CX2TalkBoxManager::OnFrameRender_Draw()
//}} robobeg : 2008-10-28
{
	KTDXPROFILE();

	if(m_bShowTalk == false)
		return;

	if ( m_pTalkBoxManagerImp != NULL )
		m_pTalkBoxManagerImp->OnFrameRender();

//	return S_OK;
}





void CX2TalkBoxManager::Push( CX2TalkBoxManagerImp::TalkBox& talkBox )
{
	KTDXPROFILE();

	if ( m_pTalkBoxManagerImp != NULL )
		m_pTalkBoxManagerImp->Push( talkBox );
}

bool CX2TalkBoxManager::Delete( UidType unitUID, bool bTraceUnit, CX2GameUnit::GAME_UNIT_TYPE gameUnitType )
{
	if ( m_pTalkBoxManagerImp != NULL )
		return m_pTalkBoxManagerImp->Delete( unitUID, bTraceUnit, gameUnitType );

	return false;
}

void CX2TalkBoxManager::SetPos( UidType unitUID, D3DXVECTOR2 pos, int boxType, bool bTraceUnit, CX2GameUnit::GAME_UNIT_TYPE gameUnitType )
{
	if ( m_pTalkBoxManagerImp != NULL )
		m_pTalkBoxManagerImp->SetPos( unitUID, pos, boxType, bTraceUnit, gameUnitType );
}

void CX2TalkBoxManager::SetPos( int talkBoxID, D3DXVECTOR2 pos, int boxType, bool bTraceUnit /* = false */ , CX2GameUnit::GAME_UNIT_TYPE gameUnitType /* = CX2GameUnit::GUT_USER */ )
{
	if ( m_pTalkBoxManagerImp != NULL )
		m_pTalkBoxManagerImp->SetPos( talkBoxID, pos, boxType, bTraceUnit, gameUnitType );
}

int CX2TalkBoxManager::GetMaxStrLen()
{
	if ( m_pTalkBoxManagerImp != NULL )
	{
		return m_pTalkBoxManagerImp->GetMaxStrLen();
	}

	return 0;
}

void CX2TalkBoxManager::Clear()
{
	if ( m_pTalkBoxManagerImp != NULL )
		m_pTalkBoxManagerImp->Clear( );
}

void CX2TalkBoxManager::SetOffset(D3DXVECTOR2 vPos)
{
	if(m_pTalkBoxManagerImp != NULL)
		m_pTalkBoxManagerImp->SetOffset(vPos);
}

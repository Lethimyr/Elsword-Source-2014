#include "StdAfx.h"
#include ".\x2pagemanager.h"

CX2PageManager::CX2PageManager( const WCHAR* pFileName )
{
	m_bReverseDLGLayer = false;

	m_NowPage = 1;
	m_MaxPage = 1;

	m_MaxSlotNumPerPage = 0;

	m_pDialog = NULL;

	if ( pFileName != NULL )
	{
		m_pDialog = new CKTDGUIDialog( g_pMain->GetNowState(), pFileName );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDialog );
	}

	SetOpen( false );
}

CX2PageManager::~CX2PageManager(void)
{
	SAFE_DELETE_DIALOG( m_pDialog );

	for ( int i = 0; i < (int)m_vecPageSlot.size(); i++ )
	{
		CX2PageManager::CX2PageSlot* pCX2PageSlot = m_vecPageSlot[i];
		SAFE_DELETE( pCX2PageSlot );
	}
	m_vecPageSlot.clear();
}

void CX2PageManager::Init( CX2PageManager::CX2PageSlot* pCX2PageSlot, const WCHAR* pSlotDLGName, int maxSlotNumPerPage )
{
	for ( int i = 0; i < (int)m_vecPageSlot.size(); i++ )
	{
		CX2PageSlot* pCX2PageSlot = m_vecPageSlot[i];
		SAFE_DELETE( pCX2PageSlot );
	}
	m_vecPageSlot.clear();

	m_MaxSlotNumPerPage = maxSlotNumPerPage;
	for ( int i = 0; i < (int)m_MaxSlotNumPerPage; i++ )
	{
		CX2PageManager::CX2PageSlot* pTempCX2PageSlot = CreateSlot( pCX2PageSlot, i, pSlotDLGName );
		m_vecPageSlot.push_back( pTempCX2PageSlot );
	}

	SAFE_DELETE( pCX2PageSlot );
}

void CX2PageManager::SetOpen( bool bOpen )
{
	m_bOpen = bOpen;

	if ( m_pDialog != NULL )
		m_pDialog->SetShowEnable( bOpen, bOpen );

	if ( m_bOpen == true )
	{
		InitbyOpen();

		ResetNowPage();
	}
	else
	{
		for ( int i = 0; i < (int)m_vecPageSlot.size(); i++ )
		{
			CX2PageManager::CX2PageSlot* pPageSlot = m_vecPageSlot[i];
			if ( pPageSlot != NULL )
				pPageSlot->SetShow( false );
		}
	}
}

bool CX2PageManager::NextPage()
{
	if ( m_NowPage >= m_MaxPage )
		return false;

	m_NowPage++;

	ResetNowPage();

	return true;
}

bool CX2PageManager::PrevPage()
{
	if ( m_NowPage <= 1 )
		return false;

	m_NowPage--;

	ResetNowPage();

	return true;
}

#ifdef COUPON_SYSTEM
bool CX2PageManager::LastPage()
{
	m_NowPage = m_MaxPage;

	ResetNowPage();

	return true;
}
bool CX2PageManager::FirstPage()
{
	m_NowPage = 1;

	ResetNowPage();

	return true;
}
#endif // COUPON_SYSTEM

CX2PageManager::CX2PageSlot* CX2PageManager::CreateSlot( CX2PageSlot* pPageSlot, int slotIndex, const WCHAR* pSlotDLGName )
{
	return pPageSlot->Clone( slotIndex, pSlotDLGName );
}



void CX2PageManager::ResetNowPage()
{
	for ( int i = 0; i < (int)m_vecPageSlot.size(); i++ )
	{
		CX2PageManager::CX2PageSlot* pPageSlot = m_vecPageSlot[i];
		if ( pPageSlot == NULL )
			continue;

		pPageSlot->SetShow( false );
	}

	int j = 0;
	for ( int i = (m_NowPage-1) * m_MaxSlotNumPerPage; i < (m_NowPage)*m_MaxSlotNumPerPage && j < (int)m_vecPageSlot.size(); i++, j++ )
	{
		CX2PageManager::CX2PageSlot* pPageSlot = m_vecPageSlot[j];
		if ( pPageSlot == NULL )
			continue;

		pPageSlot->SetShow( true );
		if ( m_bReverseDLGLayer == true )
		{

			CX2PageManager::CX2PageSlot* pPageSlotPrev = NULL;
			if ( j - 1 >= 0 )
				pPageSlotPrev = m_vecPageSlot[j - 1];

			if ( pPageSlotPrev != NULL && pPageSlot->GetDialog() != NULL && pPageSlotPrev->GetDialog() != NULL )
				g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( pPageSlot->GetDialog(), pPageSlotPrev->GetDialog(), false ); 
		}
	}
}

CX2PageManager::CX2PageSlot* CX2PageManager::GetPageSlot( CKTDGUIDialogType pDialog )
{
	CX2PageManager::CX2PageSlot* pCX2PageSlot = NULL;

	if ( pDialog == NULL )
		return pCX2PageSlot;

	for ( int i = 0; i < (int)m_vecPageSlot.size(); i++ )
	{
		pCX2PageSlot = m_vecPageSlot[i];
		if ( pCX2PageSlot->GetDialog() == pDialog )
		{
			return pCX2PageSlot;
		}
	}
	
	return pCX2PageSlot;
}

CX2PageManager::CX2PageSlot::CX2PageSlot()
{
	m_Index = 0;
	m_pDialog = NULL;
	SetShow( false );
}

CX2PageManager::CX2PageSlot::CX2PageSlot( int index, const WCHAR* pFileName )
{
	m_Index = index;

	m_pDialog = new CKTDGUIDialog( g_pMain->GetNowState(), pFileName );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDialog );
	m_pDialog->SetPos( D3DXVECTOR2( m_pDialog->GetDummyPos(index).x, m_pDialog->GetDummyPos(index).y ) );

	SetShow( false );
}
CX2PageManager::CX2PageSlot::~CX2PageSlot()
{
	if ( m_pDialog != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDialog, NULL, false );

	m_pDialog = NULL;
}

void CX2PageManager::CX2PageSlot::SetShow( bool bShow )
{
	if ( m_pDialog != NULL )
		m_pDialog->SetShowEnable( bShow, bShow );
}

void CX2PageManager::CX2PageSlot::SetPos( D3DXVECTOR2 pos )
{
	if ( m_pDialog != NULL )
		m_pDialog->SetPos( pos );
}
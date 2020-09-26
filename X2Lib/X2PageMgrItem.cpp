#include "StdAfx.h"
#include ".\x2pagemgritem.h"

CX2PageMgrItem::CX2PageMgrItem( const WCHAR* pFileName ) :
CX2PageManager( pFileName )
{
}

CX2PageMgrItem::~CX2PageMgrItem(void)
{
}

void CX2PageMgrItem::Reset( vector<int>& vecItemID )
{
	m_NowPage = 1;
	m_vecItemID = vecItemID;
	int checkSize = (int)m_vecItemID.size() - 1;
	if ( checkSize <= 0 )
		checkSize = 0;
	m_MaxPage = (checkSize / m_MaxSlotNumPerPage) + 1;

	for ( int i = 0; i < (int)m_vecPageSlot.size(); i++ )
	{
		CX2PageMgrItem::CX2PageSlotItem* pPageSlot = (CX2PageMgrItem::CX2PageSlotItem*)m_vecPageSlot[i];
		if ( pPageSlot != NULL )
			pPageSlot->SetShow( false );
	}

	if ( m_bOpen == true )
	{
		ResetNowPage();
	}
}

void CX2PageMgrItem::ResetNowPage()
{
	for ( int i = 0; i < (int)m_vecPageSlot.size(); i++ )
	{
		CX2PageMgrItem::CX2PageSlotItem* pPageSlot = (CX2PageMgrItem::CX2PageSlotItem*)m_vecPageSlot[i];
		if ( pPageSlot == NULL )
			continue;

		pPageSlot->SetShow( false );
	}

	int j = 0;
	for ( int i = (m_NowPage-1) * m_MaxSlotNumPerPage; i < (m_NowPage)*m_MaxSlotNumPerPage && j < (int)m_vecPageSlot.size() && i < (int)m_vecItemID.size(); i++, j++ )
	{
		int itemID = m_vecItemID[i];
		CX2PageMgrItem::CX2PageSlotItem* pPageSlot = (CX2PageMgrItem::CX2PageSlotItem*)m_vecPageSlot[j];
		pPageSlot->SetShow( true );
		pPageSlot->SetItemID( itemID );
		pPageSlot->UpdateUI();

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

CX2PageMgrItem::CX2PageSlotItem::CX2PageSlotItem( int index, const WCHAR* pFileName )
: CX2PageMgrItem::CX2PageSlot( index, pFileName )
{
	m_ItemID = 0;
}

CX2PageMgrItem::CX2PageSlotItem::~CX2PageSlotItem()
{

}
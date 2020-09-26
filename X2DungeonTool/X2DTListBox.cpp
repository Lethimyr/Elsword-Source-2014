#include "dxstdafx.h"
#include ".\x2dtlistbox.h"

KX2DTListBox::KX2DTListBox( CDXUTDialog *pDialog ) : CDXUTListBox( pDialog )
{
}

KX2DTListBox::~KX2DTListBox(void)
{
}

void KX2DTListBox::RemoveAllItems( bool bIsRemoveSelectd )
{
	for( int i = 0; i < m_Items.GetSize(); ++i )
	{
		DXUTListBoxItem *pItem = m_Items.GetAt( i );
		delete pItem;
	}

	m_Items.RemoveAll();
	m_ScrollBar.SetTrackRange( 0, 1 );

	// 모든 리스트 아이템 삭제 후 선택된 인덱스를 -1과 0으로 선택 초기화 (크래쉬의 원인)
	if( bIsRemoveSelectd )
	{
		m_nSelected = -1;
		m_nSelStart = 0;
	}
}

void KX2DTListBox::LastItemSelect()
{
    m_nSelected = m_Items.GetSize() - 1;
	m_nSelStart = m_nSelected;
}
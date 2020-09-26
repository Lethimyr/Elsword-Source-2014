#include "StdAfx.h"
#include ".\x2itemtoolx2item.h"

CX2Item::CX2Item( ItemData* pItemData )
{
	m_pItemData		= pItemData;
	//m_pItemTemplet	= g_pData->GetItemManager()->GetItemTemplet( m_pItemData->m_ItemID );

	m_bEqip			= false;
}

CX2Item::~CX2Item(void)
{
	SAFE_DELETE( m_pItemData );
}
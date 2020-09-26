#include "StdAfx.h"
#ifdef FIELD_NAVIGATOR
#include "X2FieldNavigator.h"

CX2FieldNavigator::CX2FieldNavigator(void)
{
}


CX2FieldNavigator::~CX2FieldNavigator(void)
{
}

void CX2FieldNavigator::InsertFieldMapLinkInfo(FieldMapLinkInfo MapLinkInfo)
{
	m_vecFieldMapLinkInfo.push_back(MapLinkInfo);
}

void CX2FieldNavigator::InsertFieldMapLinkInfo(UINT uiMapID)
{	
	if( false == m_vecFieldMapLinkInfo.empty())
	{
		m_mapFieldMapLinkInfo.insert(std::make_pair(uiMapID, m_vecFieldMapLinkInfo));
		m_vecFieldMapLinkInfo.clear();
	}
}
#endif //FIELD_NAVIGATOR
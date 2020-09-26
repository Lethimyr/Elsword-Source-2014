#include <StdAfx.h>
#include ".\ktdxcollision.h"

CKTDXCollision::CKTDXCollision(void)
{
}

CKTDXCollision::~CKTDXCollision(void)
{
}





void CKTDXCollision::CollisionData::SetPoint1_LUA( const char* pFrameName, D3DXVECTOR3 offset )
{
	ConvertUtf8ToWCHAR( m_FrameName, pFrameName );
	m_Offset = offset;
}

void CKTDXCollision::CollisionData::SetPoint2_LUA( const char* pFrameName, D3DXVECTOR3 offset )
{
	ConvertUtf8ToWCHAR( m_FrameNameLineEnd, pFrameName );
	m_OffsetLineEnd = offset;
}




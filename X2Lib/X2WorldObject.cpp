#include "stdafx.h"
#include ".\x2worldobject.h"

CX2WorldObject::CX2WorldObject( bool bBackgroundLoad_ /*= false*/ ) :
m_bBackgroundLoad( bBackgroundLoad_ ),
m_ObjectType( OT_INVALID ),
m_bLOD( false ),
m_wstrObjectName( L"" ),
m_LightPos( 0, 0, 0 ),
m_fElapsedTime( 0.f ),
m_coColor( D3DXCOLOR( 1, 1, 1, 1 ) ),
m_bUseWorldColor( true ),
m_fHideDistance( 0.f ),
m_fHideNearDistance( 0.f ),
m_bForceShow( false )
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
, m_coOriginalColor( 1, 1, 1, 1 )
#endif COLORFACTOR_FOR_EVERY_WORLDMESH
{
#ifdef X2TOOL
	m_bSelected = false;
	m_bOver = false;
	m_bBackupRenderParam = false;
		
	m_bUseObjectFogProperty = false;

	m_bShowObject = true;
	m_bFreezing = false;
#endif
}

CX2WorldObject::~CX2WorldObject(void)
{
}

HRESULT CX2WorldObject::OnFrameMove( double fTime, float fElapsedTime )
{
	m_fElapsedTime = fElapsedTime;
	return S_OK;
}

void CX2WorldObject::SetName( const char* pName )
{
	ConvertUtf8ToWCHAR( m_wstrObjectName, pName );
}

//{{ robobeg : 2008-10-17
//void CX2WorldObject::SetPos( D3DXVECTOR3 pos )
//{
//	m_Pos = pos;
//	GetMatrix().Move( m_Pos );
//}
//
//void CX2WorldObject::SetAngleDegree( D3DXVECTOR3 angleDegree )
//{
//	m_AngleDegree = angleDegree;
//	GetMatrix().Rotate( D3DXToRadian(m_AngleDegree.x), D3DXToRadian(m_AngleDegree.y), D3DXToRadian(m_AngleDegree.z) );
//}
//
//void CX2WorldObject::SetScale( D3DXVECTOR3 vScale )
//{
//	m_vScale = vScale;
//	GetMatrix().Scale( vScale );
//}

void CX2WorldObject::SetPos( D3DXVECTOR3 pos )
{
	GetMatrix().Move( pos );
}

void CX2WorldObject::SetAngleDegree( D3DXVECTOR3 angleDegree )
{
	GetMatrix().Rotate( D3DXToRadian(angleDegree.x), D3DXToRadian(angleDegree.y), D3DXToRadian(angleDegree.z) );
}

void CX2WorldObject::SetScale( D3DXVECTOR3 vScale )
{
	GetMatrix().Scale( vScale );
}

//}} robobeg : 2008-10-17







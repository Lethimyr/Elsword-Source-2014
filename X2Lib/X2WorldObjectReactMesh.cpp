#include "stdafx.h"
#include ".\x2worldobjectreactmesh.h"

CX2WorldObjectReactMesh::CX2WorldObjectReactMesh( ReactMeshObjectData* pReactMeshObjectData )
							:CX2WorldObjectSkinMesh( pReactMeshObjectData )
{
	m_pReactMeshObjectData	= pReactMeshObjectData;
	m_bNowReact				= false;
}

CX2WorldObjectReactMesh::~CX2WorldObjectReactMesh(void)
{
}

HRESULT CX2WorldObjectReactMesh::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2WorldObjectSkinMesh::OnFrameMove( fTime, fElapsedTime );
	if( m_bNowReact == true && m_pAnim->IsAnimationEnd() == true )
	{
		m_pAnim->ChangeAnim( m_pReactMeshObjectData->animName.c_str() );
		m_pAnim->Play( CKTDGXSkinAnim::XAP_LOOP );
		m_bNowReact = false;
	}
	return S_OK;
}

void CX2WorldObjectReactMesh::React( D3DXVECTOR3 pos )
{
	if( GetDistance( pos, GetMatrix().GetPos() ) < m_pReactMeshObjectData->fReactDistance )
	{
		m_bNowReact = true;
		m_pAnim->ChangeAnim( m_pReactMeshObjectData->reactAnim.c_str() );
		m_pAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
	}	
}

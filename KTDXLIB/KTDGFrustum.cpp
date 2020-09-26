// KTDGFrustum.cpp: implementation of the KTDGFrustum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTDGFrustum.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKTDGFrustum::CKTDGFrustum( LPDIRECT3DDEVICE9 pd3dDevice )
{
    
}

CKTDGFrustum::~CKTDGFrustum()
{

}

bool CKTDGFrustum::Construct( float ZDistance )
{
	D3DXMATRIX	frustumMatrix, viewMatrix, projectionMatrix;
	float		ZMin, Q;   

	// Calculate FOV data    
	g_pKTDXApp->GetProjectionTransform( &projectionMatrix );

	// 원근 투영일 경우
	if ( g_pKTDXApp->GetDGManager()->GetPerspective() == true )
	{
		if ( ZDistance != 0.0f )
		{
			// Calculate new projection matrix based on distance provided
			ZMin = -projectionMatrix._43 / projectionMatrix._33;
			Q = ZDistance / (ZDistance - ZMin);
			projectionMatrix._33 = Q;
			projectionMatrix._43 = Q * -ZMin;
		}
	}
	// 직교 투영일 경우
	else
	{		
		if ( ZDistance != 0.0f )
		{
			// Calculate new projection matrix based on distance provided
			ZMin = -projectionMatrix._43 / projectionMatrix._33;
			Q = 1 / (ZDistance - ZMin);
			projectionMatrix._33 = Q;
			projectionMatrix._43 = Q * -ZMin;
		}
	}
	
	g_pKTDXApp->GetViewTransform( &viewMatrix );

	D3DXMatrixMultiply( &frustumMatrix, &viewMatrix, &projectionMatrix );

	// Calculate the planes
	m_Planes[0].a = frustumMatrix._14 + frustumMatrix._13; // Near
	m_Planes[0].b = frustumMatrix._24 + frustumMatrix._23;
	m_Planes[0].c = frustumMatrix._34 + frustumMatrix._33;
	m_Planes[0].d = frustumMatrix._44 + frustumMatrix._43;
	D3DXPlaneNormalize(&m_Planes[0], &m_Planes[0]);

	m_Planes[1].a = frustumMatrix._14 - frustumMatrix._13; // Far
	m_Planes[1].b = frustumMatrix._24 - frustumMatrix._23;
	m_Planes[1].c = frustumMatrix._34 - frustumMatrix._33;
	m_Planes[1].d = frustumMatrix._44 - frustumMatrix._43;
	D3DXPlaneNormalize(&m_Planes[1], &m_Planes[1]);

	m_Planes[2].a = frustumMatrix._14 + frustumMatrix._11; // Left
	m_Planes[2].b = frustumMatrix._24 + frustumMatrix._21;
	m_Planes[2].c = frustumMatrix._34 + frustumMatrix._31;
	m_Planes[2].d = frustumMatrix._44 + frustumMatrix._41;
	D3DXPlaneNormalize(&m_Planes[2], &m_Planes[2]);

	m_Planes[3].a = frustumMatrix._14 - frustumMatrix._11; // Right
	m_Planes[3].b = frustumMatrix._24 - frustumMatrix._21;
	m_Planes[3].c = frustumMatrix._34 - frustumMatrix._31;
	m_Planes[3].d = frustumMatrix._44 - frustumMatrix._41;
	D3DXPlaneNormalize(&m_Planes[3], &m_Planes[3]);

	m_Planes[4].a = frustumMatrix._14 - frustumMatrix._12; // Top
	m_Planes[4].b = frustumMatrix._24 - frustumMatrix._22;
	m_Planes[4].c = frustumMatrix._34 - frustumMatrix._32;
	m_Planes[4].d = frustumMatrix._44 - frustumMatrix._42;
	D3DXPlaneNormalize(&m_Planes[4], &m_Planes[4]);

	m_Planes[5].a = frustumMatrix._14 + frustumMatrix._12; // Bottom
	m_Planes[5].b = frustumMatrix._24 + frustumMatrix._22;
	m_Planes[5].c = frustumMatrix._34 + frustumMatrix._32;
	m_Planes[5].d = frustumMatrix._44 + frustumMatrix._42;
	D3DXPlaneNormalize(&m_Planes[5], &m_Planes[5]);

	return true;
}



bool CKTDGFrustum::CheckCube(float XCenter, float YCenter, float ZCenter, float Size) const
{
	// Make sure at least one point is completely in frustum
	for( int i = 0; i < 6; i++ )
	{
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-Size, YCenter-Size, ZCenter-Size)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+Size, YCenter-Size, ZCenter-Size)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-Size, YCenter+Size, ZCenter-Size)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+Size, YCenter+Size, ZCenter-Size)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-Size, YCenter-Size, ZCenter+Size)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+Size, YCenter-Size, ZCenter+Size)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-Size, YCenter+Size, ZCenter+Size)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+Size, YCenter+Size, ZCenter+Size)) >= 0.0f)
			continue;

		return false;
	}
	return true;
}

bool CKTDGFrustum::CheckRectangle(float XCenter, float YCenter, float ZCenter, float XSize, float YSize, float ZSize) const
{
	// Make sure at least one point is in frustum
	for( int i = 0; i < 6; i++ ) 
	{
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-XSize, YCenter-YSize, ZCenter-ZSize)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+XSize, YCenter-YSize, ZCenter-ZSize)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-XSize, YCenter+YSize, ZCenter-ZSize)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+XSize, YCenter+YSize, ZCenter-ZSize)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-XSize, YCenter-YSize, ZCenter+ZSize)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+XSize, YCenter-YSize, ZCenter+ZSize)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-XSize, YCenter+YSize, ZCenter+ZSize)) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+XSize, YCenter+YSize, ZCenter+ZSize)) >= 0.0f)
			continue;

		return false;
	}
	return true;
}

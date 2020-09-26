// KTDGMatrix.cpp: implementation of the KTDGMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTDGMatrix.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKTDGMatrix::CKTDGMatrix( LPDIRECT3DDEVICE9 pd3dDevice ) :

m_bChange( true ),
m_LastBillboardType( BT_NONE ),
m_ViewMatChangeCount( 0 )
{
    //{{ robobeg : 2008-10-17

    m_MatrixSet.m_Pos = D3DXVECTOR3( 0.f, 0.f, 0.f);
    m_MatrixSet.m_Rotate = D3DXVECTOR3( 0.f, 0.f, 0.f);
    m_MatrixSet.m_Scale = D3DXVECTOR3( 1.f, 1.f, 1.f );

    //}} robobeg : 2008-10-17

	//ZeroMemory( &m_StackBuffer,			sizeof(MatrixSet) );

	D3DXMatrixIdentity(&m_MatrixSet.m_TranslationMatrix);
	D3DXMatrixIdentity(&m_MatrixSet.m_RotationMatrix);
	D3DXMatrixIdentity(&m_MatrixSet.m_ScalingMatrix);

	//Move( 0.0f, 0.0f, 0.0f );
	//Rotate( 0.0f, 0.0f, 0.0f );
	//Scale( 1.0f, 1.0f, 1.0f );

	//m_bChange			= true;
	//m_LastBillboardType = BT_NONE;
	//m_ViewMatChangeCount = 0;
}

CKTDGMatrix::~CKTDGMatrix()
{
}

void CKTDGMatrix::UpdateWorldMatrix( BILLBOARD_TYPE billboardType )
{
	D3DXMATRIX mat = GetMatrix( billboardType );
	g_pKTDXApp->SetWorldTransform( &mat );
}

void CKTDGMatrix::Move(float XPos, float YPos, float ZPos)
{
	m_bChange = true;

	m_MatrixSet.m_Pos.x = XPos;
	m_MatrixSet.m_Pos.y = YPos;
	m_MatrixSet.m_Pos.z = ZPos;	
}

void CKTDGMatrix::MoveRel(float XAdd, float YAdd, float ZAdd)
{
	m_bChange = true;

	m_MatrixSet.m_Pos.x += XAdd;
	m_MatrixSet.m_Pos.y += YAdd;
	m_MatrixSet.m_Pos.z += ZAdd;
}

void CKTDGMatrix::Rotate(float XRot, float YRot, float ZRot)
{
	m_bChange = true;

	m_MatrixSet.m_Rotate.x = XRot;
	m_MatrixSet.m_Rotate.y = YRot;
	m_MatrixSet.m_Rotate.z = ZRot;
}

void CKTDGMatrix::RotateRel(float XAdd, float YAdd, float ZAdd)
{
	m_bChange = true;

	m_MatrixSet.m_Rotate.x += XAdd;
	m_MatrixSet.m_Rotate.y += YAdd;
	m_MatrixSet.m_Rotate.z += ZAdd;
}

void CKTDGMatrix::Scale(float XScale, float YScale, float ZScale)
{
	m_bChange = true;

	m_MatrixSet.m_Scale.x = XScale;
	m_MatrixSet.m_Scale.y = YScale;
	m_MatrixSet.m_Scale.z = ZScale;
}

void CKTDGMatrix::ScaleRel(float XAdd, float YAdd, float ZAdd)
{
	m_bChange = true;

	m_MatrixSet.m_Scale.x += XAdd;
	m_MatrixSet.m_Scale.y += YAdd;
	m_MatrixSet.m_Scale.z += ZAdd;
}

//{{ robobeg : 2008-10-17
//void CKTDGMatrix::PushMatrix()
//{
//	m_bChange = true;
//
//	m_StackBuffer.m_FinalMatrix			= m_FinalMatrix;
//	m_StackBuffer.m_TranslationMatrix	= m_TranslationMatrix;
//	m_StackBuffer.m_RotationMatrix		= m_RotationMatrix;
//	m_StackBuffer.m_ScalingMatrix		= m_ScalingMatrix;
//	
//	m_StackBuffer.m_Pos					= m_Pos;
//	m_StackBuffer.m_Rotate				= m_Rotate;
//	m_StackBuffer.m_Scale				= m_Scale;
//
//	m_StackMatrix.push( m_StackBuffer );
//}
//
//int CKTDGMatrix::PopMatrix()
//{
//	m_bChange = true;
//
//	if( m_StackMatrix.empty() != true )
//	{
//		MatrixSet& matrixSet = m_StackMatrix.top();
//
//		m_FinalMatrix		= matrixSet.m_FinalMatrix;
//		m_TranslationMatrix	= matrixSet.m_TranslationMatrix;
//		m_RotationMatrix	= matrixSet.m_RotationMatrix;
//		m_ScalingMatrix		= matrixSet.m_ScalingMatrix;
//		
//		m_Pos				= matrixSet.m_Pos;
//		m_Rotate			= matrixSet.m_Rotate;
//		m_Scale				= matrixSet.m_Scale;
//		
//		m_StackMatrix.pop();
//	}
//	return (int)m_StackMatrix.size();
//}
//}} robobeg : 2008-10-17

const D3DXMATRIX& CKTDGMatrix::GetMatrix( BILLBOARD_TYPE billboardType /*= BT_NONE*/, D3DXMATRIX* pRotationMatrix /*= NULL*/ )
{
	if( m_bChange == true 
		|| m_LastBillboardType != billboardType
		|| ( (billboardType == BT_ALL || billboardType == BT_X || billboardType == BT_Y || billboardType == BT_Z) 
		&& g_pKTDXApp->GetViewMatChangeCount() != m_ViewMatChangeCount) )
	{
		D3DXMatrixTranslation(&m_MatrixSet.m_TranslationMatrix, m_MatrixSet.m_Pos.x, m_MatrixSet.m_Pos.y, m_MatrixSet.m_Pos.z);

		if( NULL == pRotationMatrix )
		{
			D3DXMatrixRotationYawPitchRoll(&m_MatrixSet.m_RotationMatrix, m_MatrixSet.m_Rotate.y, m_MatrixSet.m_Rotate.x, m_MatrixSet.m_Rotate.z);
		}
		else
		{
			m_MatrixSet.m_RotationMatrix = *pRotationMatrix;
		}
		
		D3DXMatrixScaling(&m_MatrixSet.m_ScalingMatrix, m_MatrixSet.m_Scale.x, m_MatrixSet.m_Scale.y, m_MatrixSet.m_Scale.z);

		switch( billboardType )
		{
			case BT_NONE:
				m_MatrixSet.m_FinalMatrix = m_MatrixSet.m_ScalingMatrix * m_MatrixSet.m_RotationMatrix * m_MatrixSet.m_TranslationMatrix;
				break;

			case BT_ALL:
				{
					D3DXMATRIX matView;
					D3DXMATRIX matbill;

					g_pKTDXApp->GetViewTransform( &matView );
					D3DXMatrixIdentity(&matbill);

					matbill._11 = matView._11;
					matbill._12 = matView._21;
					matbill._13 = matView._31;
					matbill._21 = matView._12;
					matbill._22 = matView._22;
					matbill._23 = matView._32;
					matbill._31 = matView._13;
					matbill._32 = matView._23;
					matbill._33 = matView._33;

					m_MatrixSet.m_FinalMatrix = m_MatrixSet.m_ScalingMatrix * m_MatrixSet.m_RotationMatrix * matbill * m_MatrixSet.m_TranslationMatrix;
				}
				break;

			case BT_X:
				{
					D3DXMATRIX matView, matTranspose;
					D3DXMATRIX matbill, matFinal;

					g_pKTDXApp->GetViewTransform( &matView );
					D3DXMatrixIdentity(&matbill);

					matbill._22 = matView._22;
					matbill._23 = matView._23;
					matbill._32 = matView._32;
					matbill._33 = matView._33;

					D3DXMatrixInverse( &matbill, NULL, &matbill );

					m_MatrixSet.m_FinalMatrix = m_MatrixSet.m_ScalingMatrix * m_MatrixSet.m_RotationMatrix * matbill * m_MatrixSet.m_TranslationMatrix;
				}
				break;

			case BT_Y:
				{
					D3DXMATRIX matView, matTranspose;
					D3DXMATRIX matbill, matFinal;

					g_pKTDXApp->GetViewTransform( &matView );
					D3DXMatrixIdentity(&matbill);

					matbill._11 = matView._11;
					matbill._13 = matView._13;
					matbill._31 = matView._31;
					matbill._33 = matView._33;

					D3DXMatrixInverse( &matbill, NULL, &matbill );

					m_MatrixSet.m_FinalMatrix = m_MatrixSet.m_ScalingMatrix * m_MatrixSet.m_RotationMatrix * matbill * m_MatrixSet.m_TranslationMatrix;
				}
				break;

			case BT_Z:
				{
					D3DXMATRIX matView, matTranspose;
					D3DXMATRIX matbill, matFinal;

					g_pKTDXApp->GetViewTransform( &matView );
					D3DXMatrixIdentity(&matbill);

					matbill._11 = matView._11;
					matbill._12 = matView._12;
					matbill._21 = matView._21;
					matbill._22 = matView._22;

					D3DXMatrixInverse( &matbill, NULL, &matbill );

					m_MatrixSet.m_FinalMatrix = m_MatrixSet.m_ScalingMatrix * m_MatrixSet.m_RotationMatrix * matbill * m_MatrixSet.m_TranslationMatrix;
				}
				break;
		}
	}

	m_LastBillboardType = billboardType;
	m_bChange			= false;

	return m_MatrixSet.m_FinalMatrix;
}


#ifdef BOOST_SINGLETON_POOL_TEST
	typedef boost::singleton_pool<boost::pool_allocator_tag, sizeof(CKTDGMatrix)> CKTDGMatrixPool;

	void* CKTDGMatrix::operator new(size_t s)
	{
		return CKTDGMatrixPool::malloc();
	}

	void CKTDGMatrix::operator delete(void *p)
	{
		CKTDGMatrixPool::free(p);
	}
#endif BOOST_SINGLETON_POOL_TEST






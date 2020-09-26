// KTDGMatrix.h: interface for the KTDGMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTDGMATRIX_H__A20CB309_8899_4A8B_A53E_FA7CC4ABC1DA__INCLUDED_)
#define AFX_KTDGMATRIX_H__A20CB309_8899_4A8B_A53E_FA7CC4ABC1DA__INCLUDED_

#pragma once

//{{ robobeg : 2008-10-17
struct CKTDGMatrixSet
{
	D3DXMATRIX	m_FinalMatrix;
	D3DXMATRIX	m_TranslationMatrix;
	D3DXMATRIX	m_RotationMatrix;
	D3DXMATRIX	m_ScalingMatrix;

	D3DXVECTOR3	m_Pos;
	D3DXVECTOR3	m_Rotate;
	D3DXVECTOR3	m_Scale;
};
//}} robobeg : 2008-10-17

class CKTDGMatrix  
{
	public:
		enum BILLBOARD_TYPE
		{
			BT_NONE,
			BT_ALL,
			BT_X,
			BT_Y,
			BT_Z,
		};

	private:


	public:
		CKTDGMatrix( LPDIRECT3DDEVICE9 pd3dDevice );
		virtual ~CKTDGMatrix();


#ifdef BOOST_SINGLETON_POOL_TEST
		void* operator new(size_t s);
		void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST


		void UpdateWorldMatrix( BILLBOARD_TYPE billboardType = BT_NONE );

		void Move(float XPos, float YPos, float ZPos);
		void Move( const D3DXVECTOR3& pos ){ Move( pos.x, pos.y, pos.z ); }
		void MoveRel(float XAdd, float YAdd, float ZAdd);
		void MoveRel( const D3DXVECTOR3& addPos ){ MoveRel( addPos.x, addPos.y, addPos.z ); }

		void Rotate(float XRot, float YRot, float ZRot);
		void Rotate( const D3DXVECTOR3& rot ){ Rotate( rot.x, rot.y, rot.z ); }
		void RotateRel(float XAdd, float YAdd, float ZAdd);
		void RotateRel( const D3DXVECTOR3& addRot ){ RotateRel( addRot.x, addRot.y, addRot.z ); }
		void RotateDegree(float XRot, float YRot, float ZRot){ Rotate( D3DXToRadian(XRot), D3DXToRadian(YRot), D3DXToRadian(ZRot) ); }
		void RotateDegree(const D3DXVECTOR3& rot){ RotateDegree( rot.x, rot.y, rot.z ); }

		void Scale(float XScale, float YScale, float ZScale);
		void Scale( const D3DXVECTOR3& scale ){ Scale( scale.x, scale.y, scale.z ); }
#ifdef VERIFY_STAT_BY_BUFF
		void Scale( const PROTECT_VECTOR3& scale ){ Scale( scale.x, scale.y, scale.z ); }
#endif // VERIFY_STAT_BY_BUFF

		void ScaleRel(float XAdd, float YAdd, float ZAdd);
		void ScaleRel( const D3DXVECTOR3& addScale ){ ScaleRel( addScale.x, addScale.y, addScale.z ); }

//{{ robobeg : 2008-10-17
		//void PushMatrix();
		//int	 PopMatrix();
#ifdef X2OPTIMIZE_CULLING_PARTICLE
		CKTDGMatrixSet& GetMatrixSet() { return m_MatrixSet; }
#else//X2OPTIMIZE_CULLING_PARTICLE
        const CKTDGMatrixSet& GetMatrixSet() const { return m_MatrixSet; }
#endif//X2OPTIMIZE_CULLING_PARTICLE
        void    SetMatrixSet( const CKTDGMatrixSet& kIn_ ) { m_MatrixSet = kIn_; m_bChange = true; }
//}} robobeg : 2008-10-17

		const D3DXMATRIX&	GetMatrix( BILLBOARD_TYPE billboardType = BT_NONE, D3DXMATRIX* pRotationMatrix = NULL );
		const D3DXMATRIX&	GetTransMatrix()	{ return m_MatrixSet.m_TranslationMatrix; }
		const D3DXMATRIX&	GetRotMatrix()		{ return m_MatrixSet.m_RotationMatrix; }
		const D3DXMATRIX&	GetScaleMatrix()	{ return m_MatrixSet.m_ScalingMatrix; }

		const D3DXVECTOR3& GetPos()				{ return m_MatrixSet.m_Pos; }
		const D3DXVECTOR3& GetRotate()			{ return m_MatrixSet.m_Rotate; }
		D3DXVECTOR3	GetRotateDegree()	
		{
			D3DXVECTOR3 out;
			out.x = D3DXToDegree(m_MatrixSet.m_Rotate.x);
			out.y = D3DXToDegree(m_MatrixSet.m_Rotate.y);
			out.z = D3DXToDegree(m_MatrixSet.m_Rotate.z);
			return out; 
		}
		const D3DXVECTOR3& GetScale()			{ return m_MatrixSet.m_Scale; }		

		float GetXPos()				{ return m_MatrixSet.m_Pos.x; }
		float GetYPos()				{ return m_MatrixSet.m_Pos.y; }
		float GetZPos()				{ return m_MatrixSet.m_Pos.z; }
		float GetXRotate()			{ return m_MatrixSet.m_Rotate.x; }
		float GetYRotate()			{ return m_MatrixSet.m_Rotate.y; }
		float GetZRotate()			{ return m_MatrixSet.m_Rotate.z; }
		float GetXRotateDegree()	{ return D3DXToDegree(m_MatrixSet.m_Rotate.x); }
		float GetYRotateDegree()	{ return D3DXToDegree(m_MatrixSet.m_Rotate.y); }
		float GetZRotateDegree()	{ return D3DXToDegree(m_MatrixSet.m_Rotate.z); }
		float GetXScale()			{ return m_MatrixSet.m_Scale.x; }
		float GetYScale()			{ return m_MatrixSet.m_Scale.y; }
		float GetZScale()			{ return m_MatrixSet.m_Scale.z; }


	private:
		
		bool				m_bChange;
		BILLBOARD_TYPE		m_LastBillboardType;
		int					m_ViewMatChangeCount;

//{{ robobeg : 2008-10-17
		//D3DXMATRIX			m_FinalMatrix;
		//D3DXMATRIX			m_TranslationMatrix;
		//D3DXMATRIX			m_RotationMatrix;
		//D3DXMATRIX			m_ScalingMatrix;

		//D3DXVECTOR3			m_Pos;
		//D3DXVECTOR3			m_Rotate;
		//D3DXVECTOR3			m_Scale;

		//MatrixSet			m_StackBuffer;
		//stack<MatrixSet>	m_StackMatrix;

        CKTDGMatrixSet          m_MatrixSet;
//}} robobeg : 2008-10-17
};

#endif // !defined(AFX_KTDGMATRIX_H__A20CB309_8899_4A8B_A53E_FA7CC4ABC1DA__INCLUDED_)

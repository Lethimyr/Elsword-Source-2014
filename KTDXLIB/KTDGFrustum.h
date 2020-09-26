#pragma once

class CKTDGFrustum  
{
	public:
		CKTDGFrustum( LPDIRECT3DDEVICE9 pd3dDevice );
		virtual ~CKTDGFrustum();
		
		bool Construct( float ZDistance = 0.0f );
		__forceinline bool CheckPoint( float XPos, float YPos, float ZPos ) const
        {
	        // Make sure point is in frustum
			D3DXVECTOR3 vCenter(XPos, YPos, ZPos);
	        for( int i = 0; i < 6; i++ ) 
	        {
		        if(D3DXPlaneDotCoord( &m_Planes[i], &vCenter) < 0.0f)
			        return false;
	        }

	        return true;
        }
		__forceinline bool CheckPoint( const D3DXVECTOR3& vCenter ) const
        {
	        // Make sure point is in frustum
	        for( int i = 0; i < 6; i++ ) 
	        {
		        if(D3DXPlaneDotCoord( &m_Planes[i], &vCenter) < 0.0f)
			        return false;
	        }

	        return true;
        }
		__forceinline bool CheckCube( float XCenter, float YCenter, float ZCenter, float Size ) const;
		__forceinline bool CheckRectangle( float XCenter, float YCenter, float ZCenter, float XSize, float YSize, float ZSize ) const;
		__forceinline bool CheckSphere( float XCenter, float YCenter, float ZCenter, float Radius ) const
		{
			D3DXVECTOR3 vCenter(XCenter, YCenter, ZCenter);

			// Make sure radius is in frustum
			for( int i = 0; i < 6; i++ ) 
			{
				if( D3DXPlaneDotCoord( &m_Planes[i], &vCenter ) < -Radius)
					return false;
			}
            return true;
        }
        __forceinline bool CheckSphere( const D3DXVECTOR3& vCenter, float Radius ) const
        {
			// Make sure radius is in frustum
			for( int i = 0; i < 6; i++ ) 
			{
				if( D3DXPlaneDotCoord( &m_Planes[i], &vCenter ) < -Radius)
					return false;
			}
            return true;
		}

		__forceinline bool CheckSphere( float XCenter, float YCenter, float ZCenter, float Radius, float& fNeedDistance ) const
		{
			D3DXVECTOR3 vCenter(XCenter, YCenter, ZCenter);

			// Make sure radius is in frustum
			for( int i = 0; i < 6; i++ ) 
			{
				float fDistance = D3DXPlaneDotCoord(&m_Planes[i], &vCenter );
				if( fDistance < -Radius )
				{
					fNeedDistance = abs( fDistance ) - Radius;
					return false;
				}
			}

			fNeedDistance = 0;
			return true;
		}
		__forceinline bool CheckSphere( const D3DXVECTOR3& vCenter, float Radius, float& fNeedDistance ) const
		{
			// Make sure radius is in frustum
			for( int i = 0; i < 6; i++ ) 
			{
				float fDistance = D3DXPlaneDotCoord(&m_Planes[i], &vCenter );
				if( fDistance < -Radius )
				{
					fNeedDistance = abs( fDistance ) - Radius;
					return false;
				}
			}

			fNeedDistance = 0;
			return true;
		}
#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET_SPEEDUP

        enum    EInclusionType
        {
            TYPE_EXCLUDED,
            TYPE_INCLUDED,
            TYPE_INTERSECTED,
        };

		__forceinline EInclusionType    CheckSphereInclusionType( float XCenter, float YCenter, float ZCenter, float Radius ) const
		{
			D3DXVECTOR3 vCenter(XCenter, YCenter, ZCenter);

			// Make sure radius is in frustum
            float   fDist;
            EInclusionType  eType = TYPE_INCLUDED;
			for( int i = 0; i < 6; i++ ) 
			{
                fDist = D3DXPlaneDotCoord( &m_Planes[i], &vCenter );
				if( fDist <= -Radius)
					return TYPE_EXCLUDED;
                else if ( fDist < Radius )
                    eType = TYPE_INTERSECTED;
			}
            return eType;
        }
        __forceinline EInclusionType    CheckSphereInclusionType( const D3DXVECTOR3& vCenter, float Radius ) const
        {
			// Make sure radius is in frustum
            float   fDist;
            EInclusionType  eType = TYPE_INCLUDED;
			for( int i = 0; i < 6; i++ ) 
			{
				fDist = D3DXPlaneDotCoord( &m_Planes[i], &vCenter );
                if ( fDist <= -Radius)
					return TYPE_EXCLUDED;
                else if ( fDist < Radius )
                    eType = TYPE_INTERSECTED;
			}
            return eType;
		}
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET_SPEEDUP
	private:
	    D3DXPLANE			m_Planes[6];

};

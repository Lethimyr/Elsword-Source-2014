#pragma once



//{{ seojt // 2009-1-13, 16:59
class CX2WorldObjectMesh;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2WorldObjectMesh>   CX2WorldObjectMeshPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2WorldObjectMesh>   CX2WorldObjectMeshPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-13, 16:59

class CX2WorldObjectMesh : public CX2WorldObject
{
	private:
		CX2WorldObjectMesh( bool bBackgroundLoad_ );
		virtual ~CX2WorldObjectMesh(void);

	public: 
		static CX2WorldObjectMeshPtr CreateWorldObjectMesh( bool bBackgroundLoad_ )
        {
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            CX2WorldObjectMeshPtr pObject( new CX2WorldObjectMesh( bBackgroundLoad_ ) );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			CX2WorldObjectMeshPtr pObject( new CX2WorldObjectMesh( bBackgroundLoad_ ), CKTDGObject::KTDGObjectDeleter() );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            return pObject;
        }//CreateWorldObjectMesh()

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

//{{ robobeg : 2008-10-15
        //virtual bool	GetShowObject() const;
        virtual RENDER_HINT   OnFrameRender_Prepare();
		//virtual HRESULT OnFrameRender();
        virtual void              OnFrameRender_Draw();
//}} robobeg : 2008-10-15


		virtual void	SetLOD( bool bLOD );
		virtual bool	RayHit( D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance=NULL );
		virtual bool	RayHit( const D3DXVECTOR3& rayStart, const D3DXVECTOR3& rayEnd, float* pHitDistance, const D3DXMATRIX& transform );

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-20
        /// resource loading thread의 내부에서 객체가 사용하는 리소스를 로딩하는 부분을
        /// 이곳에 구현한다. - jintaeks on 2008-10-20, 17:44
        //virtual bool    OnThreadLoad(DWORD dwParam_, DWORD);
        bool            IsValidMesh() const { return m_bXMeshLoaded && m_pXMesh; }
        bool            IsValidLODMesh() const { return m_bXMeshLODLoaded && m_pXMeshLOD; }
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20

		wstring& GetXMeshName() { return m_XMeshName; }
		wstring& GetXMeshLODName() { return m_XMeshLODName; }		

		const CKTDGMatrix::BILLBOARD_TYPE GetBillboardType() { return m_BillboardType; }
		
		//LUA
		void SetXMesh_LUA( const char* pFileName );
		void SetXMeshLOD_LUA( const char* pFileName );
		void SetMultiTexXET_LUA( const char* pFileName );
		void SetChangeTexXET_LUA( const char* pFileName );

		CKTDXDeviceXMesh* GetMesh();

		CKTDGXRenderer::RenderParam* GetRenderParam(){ return &m_RenderParam; }


		bool GetCanHide() { return m_bCanHide; }
		void SetCanHide( bool bCanHide, bool bAnyCamera = false ){ m_bCanHide = bCanHide; m_bAnyCamera = bAnyCamera; }

		bool GetNotRenderWhenHidden() const { return m_bNotRenderWhenHidden; }
		void SetNotRenderWhenHidden( bool val) { m_bNotRenderWhenHidden = val; }


		void SetBillboardType( int billboardType ){ m_BillboardType = (CKTDGMatrix::BILLBOARD_TYPE)billboardType; }
		void SetTex0UVSpeed( float x, float y ){ m_Tex0UVSpeed.x = x; m_Tex0UVSpeed.y = y; }
		void SetTex1UVSpeed( float x, float y ){ m_Tex1UVSpeed.x = x; m_Tex1UVSpeed.y = y; }
		void SetTex2UVSpeed( float x, float y ){ m_Tex2UVSpeed.x = x; m_Tex2UVSpeed.y = y; }

		D3DXVECTOR2 GetTex0UVSpeed() { return m_Tex0UVSpeed; }
		D3DXVECTOR2 GetTex1UVSpeed() { return m_Tex1UVSpeed; }
		D3DXVECTOR2 GetTex2UVSpeed() { return m_Tex2UVSpeed; }

#ifdef FOG_WORLD
        void SetFog(float xNear, float xFar, float yNear, float yFar, D3DXCOLOR color =  D3DXCOLOR(0xd3, 0xe3, 0xf3, 0xff), float density = 0.0f);
        void SetFogShow(bool flag)  { m_RenderParam.bFogShow = flag; }
        bool GetFogShow()  { return m_RenderParam.bFogShow; }
#endif




	protected:
		wstring							m_XMeshName;
		wstring							m_XMeshLODName;
		CKTDXDeviceXMesh*				m_pXMesh;
		CKTDXDeviceXMesh*				m_pXMeshLOD;
//#ifdef BACKGROUND_LOADING_TEST // 2008-10-20
        bool                            m_bXMeshLoaded;
        bool                            m_bXMeshLODLoaded;
//#endif // BACKGROUND_LOADING_TEST // 2008-10-20
		CKTDXDeviceXET*					m_pMultiTexXET;
		CKTDXDeviceXET*					m_pChangeTexXET;
		CKTDGXRenderer::RenderParam		m_RenderParam;
		
		float							m_fNowAlpha;

		bool							m_bCanHide;
		bool							m_bNotRenderWhenHidden;
		bool							m_bAnyCamera;
		CKTDGMatrix::BILLBOARD_TYPE		m_BillboardType;

        D3DXVECTOR2						m_Tex0UVSpeed;
		D3DXVECTOR2						m_Tex1UVSpeed;
		D3DXVECTOR2						m_Tex2UVSpeed;

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
	public:
		__forceinline virtual bool IsInFrustum( const D3DXVECTOR3& kEye, const CKTDGFrustum& kFrustum )
		{
			if( GetShowObject() == false )
				return false;

			CKTDXDeviceXMesh* pXMesh = NULL;

			if( m_bLOD == true && m_bXMeshLODLoaded )
				pXMesh = m_pXMeshLOD;
			else if ( m_bXMeshLoaded )
				pXMesh = m_pXMesh;

			if( pXMesh == NULL )
				return false;

			D3DXVECTOR3 center;
			

			GetTransformCenter( &center );
			SetDistanceToCamera( GetDistance( kEye, center ) );

			if ( GetBoundingRadius() <= 0 )
			{
				m_vecDrawSubset.resize( 0 );
				return true;
			}
            float fScale = GetMaxScale();
			//컬링
			//mesh bounding로 culling test
#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET_SPEEDUP
            switch( kFrustum.CheckSphereInclusionType( center, GetBoundingRadius() * fScale ) )
            {
            case CKTDGFrustum::TYPE_INCLUDED:
				m_vecDrawSubset.resize( 0 );
				return true;
                break;
            case CKTDGFrustum::TYPE_EXCLUDED:
                return false;
            }
#else   X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET_SPEEDUP
			if( kFrustum.CheckSphere( center, GetBoundingRadius() * fScale ) == false )
				return false;
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET_SPEEDUP

			//mesh subset bounding로 culling test
			{
				int iCulledNum = 0;
				const std::vector< CKTDXDeviceXMesh::KSubsetCullingInfo >& vecSubsetBoundingSphere = pXMesh->GetSubsetCullingInfoVec();
				m_vecDrawSubset.resize( vecSubsetBoundingSphere.size() );
				for( int i = 0; i < (int) vecSubsetBoundingSphere.size(); i++ )
				{
					D3DXVec3TransformCoord( &center, &vecSubsetBoundingSphere[i].m_vCenter, &GetMatrix().GetMatrix() );
					//컬링
					if( kFrustum.CheckSphere( center, vecSubsetBoundingSphere[i].m_fRadius * fScale ) == false )
					{
						m_vecDrawSubset[i] = false;
						iCulledNum++;
					}
					else
						m_vecDrawSubset[i] = true;
				}
				if( iCulledNum == vecSubsetBoundingSphere.size() )
					return false;
			}
			return true;
		}

	private:
		std::vector<bool> m_vecDrawSubset;//frustum 체크를 통해 그릴지 여부를 저장한 subset 정보
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

#ifdef X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
	private:
		CKTDXDeviceXMesh* m_pkBoundingSphere;
#endif//X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST

};

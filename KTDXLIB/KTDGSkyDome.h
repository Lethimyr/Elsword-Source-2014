#pragma once

class CKTDGSkyDome : public CKTDGObject
{
	private:
		CKTDGSkyDome();
		virtual ~CKTDGSkyDome(void);

	public:
		static CKTDGSkyDome* CreateSkyDome() { return new CKTDGSkyDome; }
		//static void DeleteKTDGObject( CKTDGSkyDome* pObject ) 
		//{ 	
		//	if( NULL != pObject )
		//	{
		//		CKTDGObject::DeleteKTDGObject( pObject ); 
		//	} 
		//}



		void SetColorFactor( D3DXCOLOR color )
		{ 
			if( m_bUseWorldColor == true )
			{
				m_vColorFactor = color;
				D3DXCOLOR vColor = m_vColorFactor;
				vColor.r *= m_vSkyDomeColorFactor.r;
				vColor.g *= m_vSkyDomeColorFactor.g;
				vColor.b *= m_vSkyDomeColorFactor.b;
				vColor.a *= m_vSkyDomeColorFactor.a;
				m_RenderParam.color = vColor;
			}
			else
			{
				m_RenderParam.color = m_vSkyDomeColorFactor; 
			}
		}
		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

//{{ robobeg : 2008-10-24
		//virtual HRESULT OnFrameRender();
        virtual RENDER_HINT   OnFrameRender_Prepare();
        virtual void              OnFrameRender_Draw();
//}} robobeg : 2008-10-24

#ifdef X2TOOL
		wstring GetName() { return m_pDomeMesh->GetDeviceID(); }
		void SetDomeMeshW(wstring wstrMeshName);
#endif
		const CKTDXDeviceXMesh* GetXMesh() const { return m_pDomeMesh; };
		const float GetRotationSpeed() { return m_fRotationSpeed; }
		const bool GetViewTrace() { return m_bViewTrace; }
		const bool GetViewTraceXZ() { return m_bViewTraceXZ; }
		const bool GetViewRotation() { return m_bViewRotation; }
		
		//LUA
		CKTDGXRenderer::RenderParam* GetRenderParam(){ return &m_RenderParam; }
		void SetTex0UVSpeed( float x, float y ){ m_Tex0UVSpeed.x = x; m_Tex0UVSpeed.y = y; }
		void SetTex1UVSpeed( float x, float y ){ m_Tex1UVSpeed.x = x; m_Tex1UVSpeed.y = y; }
		void SetTex2UVSpeed( float x, float y ){ m_Tex2UVSpeed.x = x; m_Tex2UVSpeed.y = y; }
		void SetRotationSpeed( float fSpeed ){ m_fRotationSpeed = fSpeed; }


		D3DXVECTOR2 GetTex0UVSpeed() { return m_Tex0UVSpeed; }
		D3DXVECTOR2 GetTex1UVSpeed() { return m_Tex1UVSpeed; }
		D3DXVECTOR2 GetTex2UVSpeed() { return m_Tex2UVSpeed; }

		void SetDomeMesh_LUA( const char* pMeshName );
		void SetMultiTexXET_LUA( const char* pFileName );
		void SetChangeTexXET_LUA( const char* pFileName );
		void SetViewTrace( bool bTrace ){ m_bViewTrace = bTrace; }				// XYZ 세축으로 view trace
		void SetViewTraceXZ( bool bTraceXZ ) { m_bViewTraceXZ = bTraceXZ; }		// XZ 축으로만 view trace
		void SetViewRotation( bool bEnable ) { m_bViewRotation = bEnable; }
		
		void SetPos( float x, float y, float z ){ GetMatrix().Move( x, y, z ); }
        
		bool GetUseWorldColor() { return m_bUseWorldColor; }
		void SetUseWorldColor(bool bVal) { m_bUseWorldColor = bVal; }
		D3DXCOLOR GetSkyDomeColor() { return m_vSkyDomeColorFactor; };
		void SetSkyDomeColor(D3DXCOLOR vColor) { m_vSkyDomeColorFactor = vColor; }

#ifdef FOG_WORLD
        bool GetFog() { return m_RenderParam.bFog; }
        void SetFog(float xNear, float xFar, float yNear, float yFar, D3DXCOLOR color =  D3DXCOLOR(0xd3, 0xe3, 0xf3, 0xff), float density = 0.0f);
        bool GetFogShow() { return m_RenderParam.bFogShow; }
        void SetFogShow(bool flag) { m_RenderParam.bFogShow = flag; }
#endif

#ifdef X2TOOL
		bool IsEnableObjectFog()
		{
			return m_RenderParam.bFog && m_RenderParam.bFogShow;
		}
		bool IsObjectFogShow()
		{
			return m_RenderParam.bFogShow;
		}
		CKTDGXRenderer::RenderParam *GetOriRenderParam() { return &m_oriRenderParam; }
#endif

#ifdef X2TOOL
	public:		
		bool							m_bUseObjectFogProperty;		
		bool							m_bSelected;

		bool							m_bBackupRenderParam;
		CKTDGXRenderer::RenderParam		m_oriRenderParam;

		bool							m_bShowObject;
#endif

	private:
		CKTDGXRenderer::RenderParam		m_RenderParam;

		D3DXVECTOR2						m_Tex0UVSpeed;
		D3DXVECTOR2						m_Tex1UVSpeed;
		D3DXVECTOR2						m_Tex2UVSpeed;

		float							m_fRotationSpeed;
		CKTDXDeviceXMesh*				m_pDomeMesh;
		CKTDXDeviceXET*					m_pMultiTexXET;
		CKTDXDeviceXET*					m_pChangeTexXET;
		bool							m_bViewTrace;
		bool							m_bViewTraceXZ;

		bool							m_bViewRotation;

		bool							m_bUseWorldColor;
		D3DXCOLOR						m_vColorFactor;
		D3DXCOLOR						m_vSkyDomeColorFactor;

};

#pragma once

#define D3DFVF_UNIT_SHADOW_VERTEX (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE)

class CKTDGPlanShadow : public CKTDGObject
{
	public:
		struct UNIT_SHADOW_VERTEX
		{
			float x, y, z;
			DWORD color;
			float u, v;
		};

	private:
		//CKTDGPlanShadow() { ASSERT( !"invalid" ); }
		CKTDGPlanShadow( float fSize );
		virtual ~CKTDGPlanShadow(void);

	public:
		static CKTDGPlanShadow* CreatePlanShadow( const float fSize ) { return new CKTDGPlanShadow( fSize ); }
		//static void DeleteKTDGObject( CKTDGPlanShadow* pObject ) 
		//{ 	
		//	if( NULL != pObject )
		//	{
		//		CKTDGObject::DeleteKTDGObject( pObject ); 
		//	} 
		//}


//{{ robobeg : 2008-10-28
		//virtual HRESULT OnFrameRender();
//#ifdef  DYNAMIC_VERTEX_BUFFER_OPT
		virtual RENDER_HINT OnFrameRender_Prepare();
//#endif
        virtual void    OnFrameRender_Draw();
//}} robobeg : 2008-10-28

		void SetShadowTexture( wstring tex );
		void UpdatePosition( D3DXVECTOR3 pos, D3DXVECTOR3 landPos, D3DXVECTOR3 rotate );

	private:
		
		//LPDIRECT3DDEVICE9	m_pd3dDevice;

		CKTDXDeviceTexture*	m_pShadowTexture;
		D3DXVECTOR3			m_Position;
		D3DXVECTOR3			m_LandPosition;
		D3DXVECTOR3			m_Rotate;
		float				m_fSize;
		float				m_fSizeFactor;
		float				m_fAlpha;

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//		LPDIRECT3DVERTEXBUFFER9 m_pVB;
//		bool                m_bVBToBeUpdated;
//#endif
};

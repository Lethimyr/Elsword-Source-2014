#pragma once

class CKTDGPostEffect
{
	public:
		CKTDGPostEffect( );
		~CKTDGPostEffect(void);

		void BeginRender( bool bClear = true );
		void EndRender();
		void RenderPost();

		virtual void OnResetDevice();
		virtual void OnLostDevice();
		
		bool GetEnable() { return m_bEnable; }
		void SetEnable(bool bVal) { m_bEnable = bVal; }
		bool IsProcessing() { return m_bProcessing; }
		
		void SetTexScale(float fVal) { m_fTexScale = fVal; }
		float GetTexScale() { return m_fTexScale; }
		
	private:
		//LPDIRECT3DDEVICE9				m_pd3dDevice;
		bool							m_bEnable;
		bool							m_bProcessing;
		
		LPD3DXEFFECT					m_pEffect;
		D3DXHANDLE						m_hTechnique[5];
		D3DXHANDLE						m_hTex;
		D3DXHANDLE						m_hDepthTex;
		D3DXHANDLE						m_hFilterTex;

		D3DXHANDLE						m_hTexSizeX;
		D3DXHANDLE						m_hTexSizeY;
		D3DXHANDLE						m_hFilterSizeX;
		D3DXHANDLE						m_hFilterSizeY;
		
		D3DXHANDLE						m_hAfWeight;
		D3DXHANDLE						m_hOffsetX;
		D3DXHANDLE						m_hOffsetY;
		

//{{ robobeg : 2008-10-13
	    CKTDGStateManager::KStateID		m_RenderStateID;
//}} robobeg : 2008-10-13		
		
		CKTDXDeviceRenderTargetTexture* m_pPostEffectTex;

		float							m_fTexScale;	

#ifdef ADD_POSTEFEFCT_FILTER01
		CKTDXDeviceTexture				*m_pFilterTex01;
#endif
};

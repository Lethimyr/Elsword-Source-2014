#pragma once

class CKTDGGlow
{
	public:
		CKTDGGlow( LPDIRECT3DDEVICE9 pd3dDevice );
		~CKTDGGlow(void);

		void BeginRender();
		void EndRender();
		virtual void OnResetDevice();

		void SetGlow( bool bGlow ){ m_bEnable = bGlow; }

	private:
		//LPDIRECT3DDEVICE9				m_pd3dDevice;
		bool							m_bEnable;
//{{ robobeg : 2008-10-13
	    CKTDGStateManager::KStateID m_RenderStateID;
//}} robobeg : 2008-10-13

		CKTDXDeviceRenderTargetTexture* m_pGlowTex128;
		CKTDXDeviceRenderTargetTexture* m_pGlowTex32;
};

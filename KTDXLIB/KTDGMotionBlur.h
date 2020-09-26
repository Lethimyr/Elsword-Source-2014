#pragma once

class CKTDGMotionBlur
{
	public:
		CKTDGMotionBlur( LPDIRECT3DDEVICE9 pd3dDevice );
		~CKTDGMotionBlur(void);

		void BeginRender();
		void EndRender();
		virtual void OnResetDevice();

		void SetBlur( bool bBlur ){ m_bEnable = bBlur; }
		bool GetBlurEnable(){ return m_bEnable; }
		void SetBlurLevel( DWORD level = 0xffffffff ){ m_BlurLevel = level; }

	private:
		//LPDIRECT3DDEVICE9				m_pd3dDevice;
		bool							m_bEnable;
		DWORD							m_BlurLevel;
//{{ robobeg : 2008-10-13
	    CKTDGStateManager::KStateID m_RenderStateID;
//}} robobeg : 2008-10-13

		CKTDXDeviceRenderTargetTexture* m_pMotionBlurTex;
};

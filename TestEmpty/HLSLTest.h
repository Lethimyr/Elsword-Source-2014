#pragma once

class CHLSLTest	: public CKTDXStage
{
	public:
		CHLSLTest(void);
		~CHLSLTest(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

	private:
		LPDIRECT3DDEVICE9		m_pd3dDevice;
		LPD3DXEFFECT			m_pEffect;

		CKTDXDeviceTexture*		m_pTex;
		CKTDXDeviceXMesh*		m_pXMesh;
		CKTDGMatrix*			m_pMatrix;
};

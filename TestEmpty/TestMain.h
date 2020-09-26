#pragma once

class CTestMain : public CKTDXStage
{
	public:
		CTestMain(void);
		~CTestMain(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		void Reset();

		CKTDGXSkinAnim* m_pAnim[20];
};

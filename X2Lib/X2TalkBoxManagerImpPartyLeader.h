#pragma once

class CX2TalkBoxManagerImpPartyLeader : public CX2TalkBoxManagerImp
{
	public:
		CX2TalkBoxManagerImpPartyLeader(void);
		virtual ~CX2TalkBoxManagerImpPartyLeader(void);

#ifdef HENIR_TEST
		virtual HRESULT OnFrameRender();
#endif HENIR_TEST


	protected:

		void DrawBalloon( int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, 
						float fWidthScale, float fHeightScale, float fTipLerpCoeff, D3DXCOLOR backTexColor );
		void DrawPartyInfoIcon( int posX, int posY );
		

		CKTDXDeviceTexture* m_pTextureLvLimit;
		CKTDXDeviceTexture* m_pTexturePartyInfoIcon;
		CKTDXDeviceTexture* m_pTexturePartyInfoIconOver;
		CKTDXDeviceTexture* m_pTexturePartyFeverGage;

};

#pragma once

class CX2TalkBoxManagerImpPShop : public CX2TalkBoxManagerImp
{
	public:

		CX2TalkBoxManagerImpPShop(void);
		virtual ~CX2TalkBoxManagerImpPShop(void);

	protected:

		void DrawBalloon( int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, 
						float fWidthScale, float fHeightScale, float fTipLerpCoeff, D3DXCOLOR backTexColor );
		void DrawPersonalShopIcon( int posX, int posY );

		CKTDXDeviceTexture* m_pTexturePShopIcon;
};

#include "StdAfx.h"
#include ".\x2talkboxmanagerimppshop.h"

CX2TalkBoxManagerImpPShop::CX2TalkBoxManagerImpPShop(void)
{
	m_pTexturePShopIcon	= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"HQ_Personal_Shop_Icon.tga" );
}

CX2TalkBoxManagerImpPShop::~CX2TalkBoxManagerImpPShop(void)
{
	SAFE_CLOSE( m_pTexturePShopIcon );
}



// 말풍선의 뾰족한 끝을 중심(fX,fY)으로 말풍선 전체를 그린다
void CX2TalkBoxManagerImpPShop::DrawBalloon( int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, float fWidthScale, 
											float fHeightScale, float fTipLerpCoeff, D3DXCOLOR backTexColor )
{
	KTDXPROFILE();

	m_pTalkBoxTexture->SetDeviceTexture();

	const float fCornerWidthScaled  = m_fTalkBoxCornerWidth * fWidthScale;
	const float fCornerHeightScaled = m_fTalkBoxCornerHeight * fHeightScale;

	const float fCenterWidthScaled  = fCenterWidth * fWidthScale;
	const float fCenterHeightScaled = fCenterHeight * fHeightScale;

	float _fX = 0.0f, _fY = 0.0f;
	switch( iType )
	{
	case TBT_FROM_DOWN_LEFT: 
		{
			_fX = fX - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
			_fY = fY - fCornerHeightScaled; 
		} 
		break;

	case TBT_FROM_DOWN_RIGHT:
		{
			_fX = fX - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
			_fY = fY - fCornerHeightScaled; 
		} 
		break;

	case TBT_FROM_UP_RIGHT: 
		{
			_fX = fX - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
			_fY = fY + 3.f * fCornerHeightScaled + fCenterHeightScaled;
		} 
		break;

	case TBT_FROM_UP_LEFT:
		{
			_fX = fX - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
			_fY = fY + 3.f * fCornerHeightScaled + fCenterHeightScaled;
		}
		break;
		/*
		case TBT_FROM_LEFT: 
		{
		_fX = fX + fCornerWidthScaled;
		_fY = fY + fCenterHeightScaled * (1.f-fTipLerpCoeff) + fCornerHeightScaled;
		} break;

		case TBT_FROM_RIGHT: 
		{
		_fX = fX - 3.f * fCornerWidthScaled - fCenterWidthScaled;
		_fY = fY + fCenterHeightScaled * (1.f-fTipLerpCoeff) + fCornerHeightScaled;
		} break;
		*/
	}

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	DrawBalloonBodyAndTip( iType, _fX, _fY, fX, fY, fCenterWidth, fCenterHeight, fWidthScale, fHeightScale, backTexColor );
//#else
//	DrawBalloonBody( _fX, _fY, fCenterWidth, fCenterHeight, fWidthScale, fHeightScale, backTexColor );
//	DrawBalloonTip( iType, fX, fY, fCenterWidth, fCenterHeight, fWidthScale, fHeightScale, backTexColor );
//#endif

//{{ robobeg : 2008-10-24
	//g_pKTDXApp->SetNULLTexture( 0 );
    CKTDGStateManager::SetTexture( 0, NULL );
//}} robobeg : 2008-10-24

	_fY = fY-fCornerHeightScaled-fCenterHeightScaled;

	DrawPersonalShopIcon( (int)( _fX / g_pKTDXApp->GetResolutionScaleX() ) - 25, (int)( _fY / g_pKTDXApp->GetResolutionScaleY() ) -76+34  );
}




void CX2TalkBoxManagerImpPShop::DrawPersonalShopIcon( int _fX, int _fY )
{
	if ( m_pTexturePShopIcon != NULL )
	{
		m_pTexturePShopIcon->Draw( _fX, _fY, 128, 128 );
	}
}


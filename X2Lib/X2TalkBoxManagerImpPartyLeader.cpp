#include "StdAfx.h"
#include ".\x2talkboxmanagerimpPartyLeader.h"

CX2TalkBoxManagerImpPartyLeader::CX2TalkBoxManagerImpPartyLeader(void)
{
	m_pTextureLvLimit = g_pKTDXApp->GetDeviceManager()->OpenTexture(L"Level_Limit.dds");
	m_pTexturePartyInfoIcon	= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_UI_Party_Info_Box.dds" );
	m_pTexturePartyInfoIconOver	= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_UI_Party_Info_Box_Over.dds" );
	m_pTexturePartyFeverGage	= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_UI_Common_Texture11.tga" );
	
	m_Size.x = 107;		// ?
	m_Size.y = 41;		// ?

	m_FontHeight	= 12;
#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	m_pFont			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_11_NORMAL_MAPPED );
#else
	m_pFont			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont( XF_DODUM_11_NORMAL );
#endif
	m_pUKFont		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_11_NORMAL );	

	m_bOverTalkBox = false;
}


CX2TalkBoxManagerImpPartyLeader::~CX2TalkBoxManagerImpPartyLeader(void)
{
	SAFE_CLOSE( m_pTextureLvLimit );
	SAFE_CLOSE( m_pTexturePartyInfoIcon );
	SAFE_CLOSE( m_pTexturePartyInfoIconOver );
	SAFE_CLOSE( m_pTexturePartyFeverGage );
}




#ifdef HENIR_TEST

	HRESULT CX2TalkBoxManagerImpPartyLeader::OnFrameRender()
	{
		KTDXPROFILE();

		//float boxWidth = 186;
		float boxBottomHeight = 39;
		float boxUpperHeight = 13;
		//float boxCenterHeight = 53;

		KD3DBEGIN()

		CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

		CKTDGStateManager::PushSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRROR );
		CKTDGStateManager::PushSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRROR );

		CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

		CKTDGStateManager::PushSamplerState( 1, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRROR );
		CKTDGStateManager::PushSamplerState( 1, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRROR );


		int incSize = m_FontHeight;

		CKTDGStateManager::SetTexture( 0, NULL );

		for ( int i = 0; i < (int)m_TalkBoxList.size(); i++ )
		{
			TalkBox& talkBox = m_TalkBoxList[i];

			if(talkBox.m_bShow == false)
				continue;

			m_bTalkBoxOverImg = talkBox.m_bOverTalkBox;
			m_bExtraVariable = talkBox.m_bExtraVariable;

			DrawBalloon( talkBox.m_TalkBoxType, 
				talkBox.m_vPos.x * g_pKTDXApp->GetResolutionScaleX(), 
				talkBox.m_vPos.y * g_pKTDXApp->GetResolutionScaleY(), 
				(talkBox.m_vTalkBoxSize.x - m_fTalkBoxCornerWidth * 2.f) * g_pKTDXApp->GetResolutionScaleX(),
				(talkBox.m_vTalkBoxSize.y - m_fTalkBoxCornerHeight * 2.f) * g_pKTDXApp->GetResolutionScaleY(), 
				talkBox.m_vTalkBoxSizeScale.x, talkBox.m_vTalkBoxSizeScale.y, talkBox.m_fTalkBoxTipLerpCoef, talkBox.m_BackTexColor );

			CKTDGFontManager::FontArticle* pFontArticle = m_pFont->Get2DArticle( talkBox.m_iFontArticleID );
			if ( pFontArticle != NULL )
			{			
				m_pFont->Draw2DText( talkBox.m_wstrTalkContent.c_str(), (int)pFontArticle->pos.x, (int)pFontArticle->pos.y, pFontArticle->color, pFontArticle->outLineColor,
					DT_LEFT, CKTDGFontManager::FS_NONE, pFontArticle->iSpreadCount );
			}
		}

		CKTDGStateManager::SetTexture( 0, NULL );
		KD3DEND()

		return S_OK;
	}


#endif HENIR_TEST


// 말풍선의 뾰족한 끝을 중심(fX,fY)으로 말풍선 전체를 그린다
void CX2TalkBoxManagerImpPartyLeader::DrawBalloon( int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, float fWidthScale, 
											float fHeightScale, float fTipLerpCoeff, D3DXCOLOR backTexColor )
{
	KTDXPROFILE();


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
	}

	//DrawBalloonBody( _fX, _fY, fCenterWidth, fCenterHeight, fWidthScale, fHeightScale, backTexColor );
	//DrawBalloonTip( iType, fX, fY, fCenterWidth, fCenterHeight, fWidthScale, fHeightScale, backTexColor );

//{{ robobeg : 2008-10-24
	//g_pKTDXApp->SetNULLTexture( 0 );
    CKTDGStateManager::SetTexture( 0, NULL );
//}} robobeg : 2008-10-24

	_fY = fY-fCornerHeightScaled-fCenterHeightScaled;

	DrawPartyInfoIcon( (int)( _fX / g_pKTDXApp->GetResolutionScaleX() - 10), (int)( _fY / g_pKTDXApp->GetResolutionScaleY() - 20)  );
}




void CX2TalkBoxManagerImpPartyLeader::DrawPartyInfoIcon( int _fX, int _fY )
{
	if(m_bOverTalkBox == false)
	{
		if ( m_pTexturePartyInfoIcon != NULL )
		{			
			m_pTexturePartyInfoIcon->Draw( _fX, _fY, 128, 128 );
		}
	}
	else
	{
		if(m_bTalkBoxOverImg == false)
		{
			if ( m_pTexturePartyInfoIcon != NULL )
			{		
				m_pTexturePartyInfoIcon->Draw( _fX, _fY, 128, 128 );
			}
		}
		else
		{
			if ( m_pTexturePartyInfoIconOver != NULL )
			{					
				m_pTexturePartyInfoIconOver->Draw( _fX-1, _fY, 128, 128 );
			}
		}		
	}
	// oasis907 : 김상윤 [2010.3.10] //  레벨 제한 아이콘 출력 안되게
	/*
	if( m_bExtraVariable == true)
		m_pTextureLvLimit->Draw( _fX + 14, _fY - 6, 32, 32 );
	*/
}




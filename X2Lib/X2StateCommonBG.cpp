#include "StdAfx.h"

#include ".\x2statecommonbg.h"

CX2StateCommonBG::CX2StateCommonBG(void)
{
	m_pDLGCommonBG = NULL;

	m_bShowCommonBG = true;

	m_fCommonBGEffectTime		= 0.0f;
	m_fCommonBGEffectCoolTime	= RandomNumber( 0.1f, 1.1f );

	SetCommonBG();
}

CX2StateCommonBG::~CX2StateCommonBG(void)
{
	if( NULL != m_pDLGCommonBG )
	{
		m_pDLGCommonBG->SetShow( false );
		m_pDLGCommonBG->SetEnable( false );
	}
}

HRESULT CX2StateCommonBG::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	CX2StateOption::OnFrameMove( fTime, fElapsedTime );
	BGEffectFrameMove( fTime, fElapsedTime );

	SetShowCommonBG();

	return S_OK;
}

void CX2StateCommonBG::SetOpenCommonBG( bool bCheck )
{
	m_bShowCommonBG = bCheck;
	if ( m_pDLGCommonBG != NULL )
	{
		m_pDLGCommonBG->SetShow( bCheck );
		m_pDLGCommonBG->SetEnable( bCheck );
	}
}

bool CX2StateCommonBG::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateOption::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	//switch( wParam )
	//{
	//	case EGS_ITEM_EXPIRATION_NOT:
	//		{
	//			if ( g_pData->GetCharacterRoom() != NULL )
	//				return g_pData->GetCharacterRoom()->Handler_EGS_ITEM_EXPIRATION_NOT( hWnd, uMsg, wParam, lParam );

	//			return false;
	//		}
	//		break;
	//}

	return false;
}

bool CX2StateCommonBG::SetCommonBG()
{
	m_pDLGCommonBG = g_pMain->GetMemoryHolder()->GetCommonDLG( this );

	return true;
}

void CX2StateCommonBG::BGEffectFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if ( m_bShowCommonBG == true &&
		NULL != m_pDLGCommonBG )
	{

		m_fCommonBGEffectTime += fElapsedTime;

		if ( m_fCommonBGEffectTime >= m_fCommonBGEffectCoolTime )
		{
			int pictureIndex = RandomNumber( 0, 8 );

			WCHAR buff[256] = {0};
			StringCchPrintf( buff, 256, L"BGEffect%d", pictureIndex );
			//wsprintf( buff, L"BGEffect%d", pictureIndex );

			CKTDGUIStatic* pStaticBGEffect = (CKTDGUIStatic*)m_pDLGCommonBG->GetControl( buff );

			CX2Unit::UNIT_CLASS unitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass();

			D3DXCOLOR bgEffectColor = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.5f );
			/*
			switch ( unitClass )
			{
			case CX2Unit::UC_ELSWORD_SWORDMAN:
			{
			bgEffectColor = D3DXCOLOR( 1.0f, 0.2f, 0.2f, 1.0f );
			}
			break;

			case CX2Unit::UC_ARME_VIOLET_MAGE:
			{
			bgEffectColor = D3DXCOLOR( 0.8f, 0.6f, 1.0f, 1.0f );
			}
			break;

			case CX2Unit::UC_LIRE_ELVEN_RANGER:
			{
			bgEffectColor = D3DXCOLOR( 0.4f, 1.0f, 0.4f, 1.0f );
			}
			break;

			default:
			{
			bgEffectColor = D3DXCOLOR( 1,1,1,1.0f );
			}
			break;
			}
			*/


			if ( pStaticBGEffect != NULL )
			{
				if ( pStaticBGEffect->GetIsMoving() == false )
				{
					pStaticBGEffect->SetOffsetPos( D3DXVECTOR2( 0, pStaticBGEffect->GetOffsetPos().y ) );
				}

				pStaticBGEffect->SetColor( bgEffectColor );
				pStaticBGEffect->Move( D3DXVECTOR2( 1224, pStaticBGEffect->GetOffsetPos().y ), bgEffectColor, 0.9f, true );
			}

			m_fCommonBGEffectTime		= 0.0f;
			m_fCommonBGEffectCoolTime	= RandomNumber( 0.1f, 1.1f );
		}
	}
}

void CX2StateCommonBG::SetShowCommonBG()
{
	if ( m_pDLGCommonBG != NULL )
	{
		m_pDLGCommonBG->SetShow( true );
		m_pDLGCommonBG->SetEnable( true );
	}
}
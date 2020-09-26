#include "StdAfx.h"
#include ".\ktdguistatic.h"

CKTDGUIStatic::CKTDGUIStatic()
{
	m_CustomMsgMouseDblClk	= -1;
	m_CustomMsgMouseUp		= -1;

	m_ControlType	= CKTDGUIControl::UCT_STATIC;
	SetEnable( false );

	m_pEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );
}

CKTDGUIStatic::~CKTDGUIStatic(void)
{
	for( int i = 0; i < (int)m_PictureDataList.size(); i++ )
	{		
		CKTDGUIControl::CPictureData* pPictureData = m_PictureDataList[i];
		SAFE_DELETE( pPictureData );
	}
	m_PictureDataList.clear();
	for( int i = 0; i < (int)m_StringDataList.size(); i++ )
	{
		CKTDGUIControl::UIStringData* stringData = m_StringDataList[i];
		SAFE_DELETE( stringData );
	}
	m_StringDataList.clear();

#if defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #ifdef MOVIE_TEST	 	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경
	BOOST_FOREACH( CMovieData* pMovieData, m_MovieDataList )
	{
		SAFE_DELETE( pMovieData );
	}
	m_MovieDataList.clear();

#endif // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #endif //  MOVIE_TEST	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경


	SAFE_CLOSE( m_pEdgeTexture );
}

HRESULT CKTDGUIStatic::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	if( m_bShow == false )
		return S_OK;

	
	
	KTDXPROFILE_BEGIN("control_framemove");
	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );
	KTDXPROFILE_END();



	KTDXPROFILE_BEGIN("string_framemove");
	for( UINT i=0; i<m_StringDataList.size(); i++ )
	{
		CKTDGUIControl::UIStringData* pStringData = m_StringDataList[i];
		if( NULL != pStringData )
		{
			if( true == pStringData->bSpread )
			{
				pStringData->OnFrameMove( fTime, fElapsedTime );
			}
		}
	}
	KTDXPROFILE_END();


	KTDXPROFILE_BEGIN("picture_framemove");
	for( int i = 0; i < (int)m_PictureDataList.size(); i++ )
	{		
		CKTDGUIControl::CPictureData* pPictureData = m_PictureDataList[i];
		if( NULL != pPictureData )
		{
			if( true == pPictureData->m_bTwinkle ||
				true == pPictureData->m_bFlicker ||
#ifdef BUFF_ICON_UI
				true == pPictureData->m_bFadeIn ||
				true == pPictureData->m_bFadeOut ||
#endif //BUFF_ICON_UI
				0.f != pPictureData->m_VibrateData.m_fMaxTime )
			{
				pPictureData->OnFrameMove( fTime, fElapsedTime );
			}
		}
	}
	KTDXPROFILE_END();

#if defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #ifdef MOVIE_TEST	 	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경

	KTDXPROFILE_BEGIN("movie_framemove");
	BOOST_FOREACH( CMovieData* pMovieData, m_MovieDataList )
	{
		if ( NULL != pMovieData )
			pMovieData->OnFrameMove();
	}
	KTDXPROFILE_END();
#endif // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #endif //  MOVIE_TEST	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경


	m_fElapsedTime = fElapsedTime;

	return S_OK;
}

HRESULT CKTDGUIStatic::OnFrameRender()
{
	KTDXPROFILE();
	if( m_bShow == false )
		return S_OK;

	for( int i = 0; i < (int)m_PictureDataList.size(); i++ )
	{
		KTDXPROFILE_BEGIN("Picture");
		CKTDGUIControl::CPictureData* pPictureData = m_PictureDataList[i];

		if ( pPictureData->GetShow() == true 
			&& pPictureData->pPoint != NULL 
			&& pPictureData->pTexture != NULL 
			&& pPictureData->pTexture->pTexture != NULL )
		{
			if ( pPictureData->m_EdgeWidth != 0 )
				DrawEdge( pPictureData->m_bDrawEdgeOut, pPictureData->m_EdgeWidth, pPictureData->m_EdgeColor, pPictureData->pPoint );

			UpdateVertex( m_Vertex[VP_LEFT_TOP], m_Vertex[VP_RIGHT_TOP],
											m_Vertex[VP_LEFT_BOTTOM], m_Vertex[VP_RIGHT_BOTTOM],
											*pPictureData->pPoint );
			

            KD3DBEGIN()
			    CKTDGStateManager::PushRenderState( D3DRS_SRCBLEND,			pPictureData->pPoint->m_eSrcBlend );
			    CKTDGStateManager::PushRenderState( D3DRS_DESTBLEND,		pPictureData->pPoint->m_eDestBlend );
			    RenderVertex( m_Vertex, *pPictureData->pPoint, pPictureData->pPoint->m_iDrawCount );
            KD3DEND()

			if ( pPictureData->m_EdgeWidth != 0 )
				DrawEdge( pPictureData->m_bDrawEdgeOut, pPictureData->m_EdgeWidth, pPictureData->m_EdgeColor, pPictureData->pPoint );
		}
		KTDXPROFILE_END();
	}

	for( int i = 0; i < (int)m_StringDataList.size(); i++ )
	{
		KTDXPROFILE_BEGIN("Text");
		CKTDGUIControl::UIStringData* pStringData = m_StringDataList[i];
		if( pStringData->msg.size() <= 0 )
			continue;

		if ( pStringData->bUseUkFont == true )
		{
			CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStringData->fontIndex );
			D3DXVECTOR2 pos;
			D3DXCOLOR	tempColor;
			D3DXCOLOR	tempOutLineColor;

			tempColor.a = pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
			tempColor.r = pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
			tempColor.g = pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
			tempColor.b = pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

			tempOutLineColor.a = pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
			tempOutLineColor.r = pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
			tempOutLineColor.g = pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
			tempOutLineColor.b = pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;

			pos = pStringData->pos + m_pDialog->GetPos() + m_OffsetPos;		
			//pFont->Draw2DText( pStringData->msg.c_str(), (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, pStringData->sortFlag, pStringData->fontStyle );

			if( true == pStringData->bSpread )
			{
				wstring spreadMsg = pStringData->msg.substr( 0, pStringData->iSpreadCount );
				pFont->OutTextMultiline( (int)pos.x, (int)pos.y, spreadMsg.c_str(), tempColor, pStringData->fontStyle, tempOutLineColor, 1.0f, NULL, pStringData->sortFlag );
			}
			else
			{
				pFont->OutTextMultiline( (int)pos.x, (int)pos.y, pStringData->msg.c_str(), tempColor, pStringData->fontStyle, tempOutLineColor, 1.0f, NULL, pStringData->sortFlag );
			}
		}
		else
		{
#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
			CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetMappedUKFont( pStringData->fontIndex );
#else
			CKTDGFontManager::CKTDGFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont( pStringData->fontIndex );
#endif
			D3DXVECTOR2 pos;
			D3DXCOLOR	tempColor;
			D3DXCOLOR	tempOutLineColor;

			tempColor.a = pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
			tempColor.r = pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
			tempColor.g = pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
			tempColor.b = pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

			tempOutLineColor.a = pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
			tempOutLineColor.r = pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
			tempOutLineColor.g = pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
			tempOutLineColor.b = pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;

			pos = pStringData->pos + m_pDialog->GetPos() + m_OffsetPos;		
			//pFont->Draw2DText( pStringData->msg.c_str(), (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, pStringData->sortFlag, pStringData->fontStyle );

			if( true == pStringData->bSpread )
			{
				wstring spreadMsg = pStringData->msg.substr( 0, pStringData->iSpreadCount );
				//pFont->OutTextMultiline( (int)pos.x, (int)pos.y, spreadMsg.c_str(), tempColor, pStringData->fontStyle, tempOutLineColor, 1.0f, NULL, pStringData->sortFlag );
				pFont->Draw2DText( spreadMsg.c_str(), (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, pStringData->sortFlag, pStringData->fontStyle, (int)spreadMsg.size() );
			}
			else
			{
				//pFont->OutTextMultiline( (int)pos.x, (int)pos.y, pStringData->msg.c_str(), tempColor, pStringData->fontStyle, tempOutLineColor, 1.0f, NULL, pStringData->sortFlag );
				pFont->Draw2DText( pStringData->msg.c_str(), (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, pStringData->sortFlag, pStringData->fontStyle, (int)pStringData->msg.size() );
			}
		}


		
		KTDXPROFILE_END();
	}

#if defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #ifdef MOVIE_TEST	 	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경
	BOOST_FOREACH( CMovieData* pMovieData, m_MovieDataList )
	{
		if ( NULL != pMovieData &&
			 pMovieData->GetShow() )
		{
			D3DXVECTOR2 vPos( (pMovieData->GetOriginPos().x + m_pDialog->GetPos().x + m_OffsetPos.x) * g_pKTDXApp->GetResolutionScaleX(),
				(pMovieData->GetOriginPos().y + m_pDialog->GetPos().y + m_OffsetPos.y) * g_pKTDXApp->GetResolutionScaleY() );
			pMovieData->SetPos( vPos );

			pMovieData->OnFrameRender();
		}
	}
#endif // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #endif //  MOVIE_TEST	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경


	return S_OK;
}


bool CKTDGUIStatic::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();

	if( m_bEnable == false || m_bShow == false )
		return false;

	switch( uMsg )
	{
		case WM_LBUTTONDOWN:		
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONUP:
			{
				if( ContainsPoint( pt ) == true )
				{
					// 스태틱일 경우, 특별한 메세지 처리를 하지 않는 경우
					// 메세지를 통과 시킴

					//return false;

					if ( uMsg == WM_LBUTTONDOWN )
					{
						if ( m_pDialog != NULL )
							m_pDialog->SetMouseDownForDrag( true );
					}
					else if( uMsg == WM_LBUTTONUP )
					{
						if( m_CustomMsgMouseUp != -1 )
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseUp, (LPARAM)this );

						if ( m_pDialog != NULL )
							m_pDialog->SetMouseDownForDrag( false );
					}	
					else if ( uMsg != WM_MOUSEMOVE )
					{
						if ( m_pDialog != NULL )
							m_pDialog->SetMouseDownForDrag( false );
					}
										

					return true;
				}
			}
			break;

		case WM_LBUTTONDBLCLK: // static 더블클릭					
			{
				// Button click
				if( ContainsPoint( pt ) == true )
				{
					if( m_CustomMsgMouseDblClk != -1 )
					{
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseDblClk, (LPARAM)this );
						return true;
					}
					//PlaySound( m_pSndMouseUp );				
				}

			}
			break;
	};

	return false;
}

bool CKTDGUIStatic::ContainsPoint( POINT pt )
{
	for( int i = 0; i < (int)m_PictureDataList.size(); i++ )
	{
		CKTDGUIControl::CPictureData* pPictureData = m_PictureDataList[i];
		
		if ( pPictureData != NULL && pPictureData->pPoint != NULL && 
			Pick2DRect( pt, 
			pPictureData->pPoint->leftTopPoint, pPictureData->pPoint->rightTopPoint, 
			pPictureData->pPoint->leftBottomPoint, pPictureData->pPoint->rightBottomPoint ) == true )
		{
			return true;
		}
		/*
		if ( pPictureData != NULL && 
		Pick2DRect( pt, 
			D3DXVECTOR2( pPictureData->GetPos().x, pPictureData->GetPos().y ), 
			D3DXVECTOR2( pPictureData->GetPos().x + pPictureData->GetSize().x, pPictureData->GetPos().y ), 
			D3DXVECTOR2( pPictureData->GetPos().x, pPictureData->GetPos().y + pPictureData->GetSize().y ), 
			D3DXVECTOR2( pPictureData->GetPos().x + pPictureData->GetSize().x, pPictureData->GetPos().y + pPictureData->GetSize().y ) ) == true )
		{
			return true;
		}
		*/
	}

	return false;
}

void CKTDGUIStatic::AddPictureAt( int iIdx, CKTDGUIControl::CPictureData* pPicture )
{
	vector<CKTDGUIControl::CPictureData*>::iterator iV = m_PictureDataList.begin();

	std::advance( iV, iIdx );
	m_PictureDataList.insert( iV, pPicture );
}

void CKTDGUIStatic::AddStringAt( int iIdx, CKTDGUIControl::UIStringData* pString )
{
	vector<CKTDGUIControl::UIStringData*>::iterator iV = m_StringDataList.begin();

	std::advance( iV, iIdx );
	m_StringDataList.insert( iV, pString );
}

bool CKTDGUIStatic::CanHaveFocus()
{
	if( m_bShow == true )
		return true;
	else
		return false;
}

void CKTDGUIStatic::AddString_LUA()
{
	CKTDGUIControl::UIStringData* pString = SetStringData_();
	AddString( pString );
}

CKTDGUIControl::UIStringData* CKTDGUIStatic::GetString( int index )
{
	if( index < 0 || index >= (int)m_StringDataList.size() )
	{
		return NULL;
	}
	else
		return m_StringDataList[index];
}


CKTDGUIControl::CPictureData* CKTDGUIStatic::GetPictureIndex( int index )
{
    if( index <= 0)
	{
        return NULL;
	}

    CKTDGUIControl::CPictureData* pNodePicture;

    for(int i=0; i<(int)m_PictureDataList.size(); ++i)
    {
        pNodePicture = m_PictureDataList[i];
        if(pNodePicture != NULL && pNodePicture->GetIndex() == index)
        {
            return pNodePicture;
        }
    }

    return NULL;
}

CKTDGUIControl::CPictureData* CKTDGUIStatic::GetPicture( int index )
{
	if( index < 0 || index >= (int)m_PictureDataList.size() )
	{
		return NULL;
	}
	else
		return m_PictureDataList[index];
}

void CKTDGUIStatic::SetString( int index, const WCHAR* wszMsg )
{
	if ( GetString(index) != NULL )
	{
		GetString(index)->msg = wszMsg;
	}
}

//{{ kimhc // 2010.12.18 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
void CKTDGUIStatic::RemovePictureByIndex( int iIndex_ )
{	// 이 함수 사용 할 때는 PVP_GAME_UI_WAKE_ORB 의 Index 숫자 높은 것에서 부터 낮은 것으로 지워야 합니다.
	// Ex. Eve 다음 Ara 다음 Raven ....
	if ( 0 > iIndex_ || m_PictureDataList.size() <= static_cast<UINT>( iIndex_ ) )
	{
		return;
	} // if
	
	CKTDGUIControl::CPictureData* pPictureData = m_PictureDataList[iIndex_];
	m_PictureDataList.erase( m_PictureDataList.begin() + iIndex_ );
	SAFE_DELETE( pPictureData );
}
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.18 //  2010-12-23 New Character CHUNG

void CKTDGUIStatic::RemoveAllPicture()
{
	for( int i = 0; i < (int)m_PictureDataList.size(); i++ )
	{		
		CKTDGUIControl::CPictureData* pPictureData = m_PictureDataList[i];
		SAFE_DELETE( pPictureData );
	}
	m_PictureDataList.clear();
}

void CKTDGUIStatic::RemoveAllString()
{
	for( int i = 0; i < (int)m_StringDataList.size(); i++ )
	{
		CKTDGUIControl::UIStringData* stringData = m_StringDataList[i];
		SAFE_DELETE( stringData );
	}
	m_StringDataList.clear();
}

void CKTDGUIStatic::SetMaximize( bool val )
{
	CKTDGUIControl::SetMaximize( val );

	int iSign = 1;
	if ( val == false )
		iSign = -1;

	CKTDGUIControl::CPictureData** acPicData = &m_PictureDataList[0];
	for ( unsigned int iS = 0; iS < m_PictureDataList.size(); ++iS )
	{
		CKTDGUIControl::CPictureData* pcPicData = m_PictureDataList[iS];

		D3DXVECTOR2 kPos = pcPicData->GetPos();
		D3DXVECTOR2 kSize = pcPicData->GetSize();

		if ( pcPicData->bMaximizePos )
			pcPicData->SetPos( D3DXVECTOR2( kPos.x - m_kMaximizeOffset.x * iSign, kPos.y - m_kMaximizeOffset.y * iSign ) );

		if ( pcPicData->bMaximizeSize )
			pcPicData->SetSize( D3DXVECTOR2( kSize.x + m_kMaximizeOffset.x * iSign, kSize.y + m_kMaximizeOffset.y * iSign ) );
	}
}


void CKTDGUIStatic::DrawEdge( bool bDrawOut, int edgeWidth, D3DXCOLOR edgeColor, CKTDGUIControl::UIPointData* pPoint )
{
	if ( edgeWidth == 0 || m_pEdgeTexture == NULL )
		return;

	if ( pPoint == NULL )
		return;

	int _width = (int)(pPoint->rightTopPoint.x - pPoint->leftTopPoint.x);
	int _height = (int)(pPoint->rightBottomPoint.y - pPoint->leftTopPoint.y);

	D3DXCOLOR tempColor;

	tempColor.a = edgeColor.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = edgeColor.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = edgeColor.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = edgeColor.b * m_pDialog->GetColor().b * m_Color.b;

	if ( bDrawOut == true )
	{
		// 좌
		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y - edgeWidth), 
			edgeWidth , 
			_height + edgeWidth, 
			tempColor );

		// 하

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftBottomPoint.y ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );

		// 우

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->rightTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y ), 
			edgeWidth, 
			_height + edgeWidth, 
			tempColor );

		// 상

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y - edgeWidth ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );
	}
	else
	{
		// 좌
		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y ), 
			edgeWidth , 
			_height, 
			tempColor );

		// 하

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftBottomPoint.y - edgeWidth ), 
			_width, 
			edgeWidth, 
			tempColor );

		// 우

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->rightTopPoint.x - edgeWidth ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y ), 
			edgeWidth, 
			_height, 
			tempColor );

		// 상

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y ), 
			_width , 
			edgeWidth, 
			tempColor );

	}

}


void CKTDGUIStatic::SetRoundBoxPosSize( D3DXVECTOR2 vPos, D3DXVECTOR2 vRectSize, D3DXVECTOR2 vRoundSize, CKTDGUIStatic::CPictureData** apPictureData )
{
	CPictureData* pLeftTop		= apPictureData[0];
	CPictureData* pMiddleTop	= apPictureData[1];
	CPictureData* pRightTop		= apPictureData[2];

	CPictureData* pLeftMiddle	= apPictureData[3];
	CPictureData* pMiddleMiddle	= apPictureData[4];
	CPictureData* pRightMiddle	= apPictureData[5];

	CPictureData* pLeftBottom	= apPictureData[6];
	CPictureData* pMiddleBottom	= apPictureData[7];
	CPictureData* pRightBottom	= apPictureData[8];


	if( NULL != pLeftTop )
	{
		pLeftTop->SetPos( vPos - vRoundSize );
		pLeftTop->SetSize( vRoundSize );
	}

	if( NULL != pMiddleTop )
	{
		pMiddleTop->SetPos( vPos + D3DXVECTOR2( vRoundSize.x, 0 ) );
		pMiddleTop->SetSize( D3DXVECTOR2(vRectSize.x, vRoundSize.y ) );
	}

	if( NULL != pRightTop )
	{
		pRightTop->SetPos( vPos + D3DXVECTOR2( vRoundSize.x, 0) + D3DXVECTOR2( vRoundSize.x, 0 ) );
		pRightTop->SetSize( vRoundSize );
	}



	if( NULL != pLeftMiddle )
	{
		pLeftMiddle->SetPos( vPos - D3DXVECTOR2( vRoundSize.x, 0) );
		pLeftMiddle->SetSize( D3DXVECTOR2( vRoundSize.x, vRectSize.y ) );
	}	

	if( NULL != pMiddleMiddle )
	{
		pMiddleMiddle->SetPos( vPos );
		pMiddleMiddle->SetSize( vRectSize );
	}	

	if( NULL != pRightMiddle )
	{
		pRightMiddle->SetPos( vPos + D3DXVECTOR2( vRectSize.x, 0) );
		pRightMiddle->SetSize( D3DXVECTOR2( vRoundSize.x, vRectSize.y ) );
	}	



	if( NULL != pLeftBottom )
	{
		pLeftBottom->SetPos( vPos - D3DXVECTOR2( vRoundSize.x, 0) + D3DXVECTOR2( 0, vRectSize.y ) );
		pLeftBottom->SetSize( vRoundSize );
	}	

	if( NULL != pMiddleBottom )
	{
		pMiddleBottom->SetPos( vPos + D3DXVECTOR2( 0, vRectSize.y ) );
		pMiddleBottom->SetSize( D3DXVECTOR2(vRectSize.x, vRoundSize.y ) );
	}	

	if( NULL != pRightBottom )
	{
		pRightBottom->SetPos( vPos + vRectSize );
		pRightBottom->SetSize( vRoundSize );
	}	
	
}

/*virtual*/ void CKTDGUIStatic::MoveControl( float fx, float fy )
{
	if( fx == 0.f && fy == 0.f )
		return;



	for( UINT i=0; i<m_PictureDataList.size(); i++ )
	{
		CKTDGUIControl::CPictureData* pPicture = m_PictureDataList[i];
		if( NULL != pPicture &&
			NULL != pPicture->pPoint )
		{
			pPicture->pPoint->leftTopPoint = pPicture->pos;
			pPicture->pPoint->SetAutoPointByTextureSize();

			pPicture->pPoint->Move( fx, fy );
			pPicture->pos = pPicture->pPoint->leftTopPoint;
#ifdef DLL_BUILD
			if( pPicture->pPoint->fRotDegree != 0 )
				pPicture->pPoint->Rotate(pPicture->pPoint->fRotDegree);
#endif
		}
	}
#ifdef DLL_BUILD
	for( UINT i=0; i<m_StringDataList.size(); i++ )
	{
		CKTDGUIControl::UIStringData* pStringData = m_StringDataList[i];
		if( NULL != pStringData )
		{
			pStringData->pos += D3DXVECTOR2( fx, fy );
		}
	}
#endif
}

/*virtual*/ void CKTDGUIStatic::ScaleControl( float fx, float fy )
{
	if( fx == 1.f && fy == 1.f )
		return;

	for( UINT i=0; i<m_PictureDataList.size(); i++ )
	{
		CKTDGUIControl::CPictureData* pPicture = m_PictureDataList[i];
		if( NULL != pPicture &&
			NULL != pPicture->pPoint )
		{
			pPicture->pPoint->Scale( fx, fy );
			pPicture->pos = pPicture->pPoint->leftTopPoint;

			// fix!! size는 나중에
		}
	}
}

#ifdef DLL_BUILD
void CKTDGUIStatic::MoveSubControl( float fx, float fy, wstring subControlName )
{
	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(subControlName);
	if( NULL != pPictureData )
	{
		if( NULL != pPictureData->pPoint )
		{
			pPictureData->pPoint->leftTopPoint = pPictureData->pos;
			pPictureData->pPoint->SetAutoPointByTextureSize();

			pPictureData->pPoint->Move( fx, fy );
			pPictureData->pos = pPictureData->pPoint->leftTopPoint;

			if( pPictureData->pPoint->fRotDegree != 0 )
				pPictureData->pPoint->Rotate(pPictureData->pPoint->fRotDegree);
			return;
		}
	}	

	int index = 1;
	for each( CKTDGUIControl::UIStringData * pStringData in m_StringDataList )
	{
		if( NULL != pStringData )
		{
			std::wstringstream wstrstm;
			wstrstm << index << L"_" << pStringData->msg;
			if( wstrstm.str() == subControlName )
			{
				pStringData->pos += D3DXVECTOR2( fx, fy );
				return;
			}
		}

		index++;
	}
}

void CKTDGUIStatic::ScaleSubControl( float fx, float fy, wstring subControlName )
{
	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(subControlName);
	if( NULL != pPictureData )
	{
		if( NULL != pPictureData->pPoint )
		{
			pPictureData->pPoint->leftTopPoint = pPictureData->pos;
			pPictureData->pPoint->Scale( fx, fy );
			pPictureData->SetSizeAsTextureSize();
			pPictureData->pos = pPictureData->pPoint->leftTopPoint;

			if( pPictureData->pPoint->fRotDegree != 0 )
				pPictureData->pPoint->Rotate(pPictureData->pPoint->fRotDegree);

			return;
		}			
	}	
}

void CKTDGUIStatic::ShowSubView( wstring name, bool bView )
{
	SetColor(D3DXCOLOR(0xffffffff));

	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(name);
	if( NULL != pPictureData )
	{
		if( bView )
			pPictureData->SetEdge( true, 4, D3DXCOLOR(1,0,0,1) );
		else
			pPictureData->SetEdge( true, 0, D3DXCOLOR(1,1,1,1) );

		return;
	}			

}

D3DXVECTOR2 CKTDGUIStatic::GetPos()
{
	if( m_PictureDataList.size() > 0 )
	{
		CKTDGUIControl::CPictureData* pPicture = m_PictureDataList[0];
		if( pPicture != NULL )
			return pPicture->pos;
	}

	if( m_StringDataList.size() > 0 )
	{
		CKTDGUIControl::UIStringData* pStringData = m_StringDataList[0];
		if( NULL != pStringData )
		{
			return pStringData->pos;
		}
	}
	return D3DXVECTOR2(0.f, 0.f);
}

vector<D3DXVECTOR2> CKTDGUIStatic::GetPosList()
{
	vector<D3DXVECTOR2> ret;
	for( UINT i=0; i<m_PictureDataList.size(); i++ )
	{
		CKTDGUIControl::CPictureData* pPicture = m_PictureDataList[i];
		if( NULL != pPicture )		
		{
			ret.push_back(pPicture->pos);
		}
	}

	return ret;
}

vector<D3DXVECTOR2> CKTDGUIStatic::GetPosList( wstring name )
{
	vector<D3DXVECTOR2> ret;

	if( name == L"Picture" )
	{
		return GetPosList();
	}
	else if( name == L"String" )
	{
		for each( CKTDGUIControl::UIStringData * pStringData in m_StringDataList )
		{			
			if( NULL != pStringData )
			{
				ret.push_back(pStringData->pos);
			}
		}
	}

	return ret;
}


D3DXVECTOR2 CKTDGUIStatic::GetPos(wstring name)
{	
	int index = 1;
	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(name);
	if( NULL != pPictureData )
	{
		return pPictureData->pos;
	}

	index = 1;
	for each( CKTDGUIControl::UIStringData * pStringData in m_StringDataList )
	{
		if( NULL != pStringData)
		{
			std::wstringstream wstrstm;
			wstrstm << index << L"_" << pStringData->msg;
			if( wstrstm.str() == name )
				return pStringData->pos;
		}

		index++;
	}

	return D3DXVECTOR2(0, 0);
}

D3DXVECTOR2 CKTDGUIStatic::GetSize( wstring name )
{
	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(name);
	if( NULL != pPictureData )
	{
		return pPictureData->size;
	}

	return D3DXVECTOR2(0.f, 0.f);
}

D3DXVECTOR2 CKTDGUIStatic::GetAddSize( wstring name )
{
	int index = 1;
	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(name);
	if( NULL != pPictureData )
	{
		if( NULL != pPictureData->pPoint )
		{
			return pPictureData->pPoint->addSize;
		}
	}	

	return D3DXVECTOR2(0.f, 0.f);
}


vector<wstring> CKTDGUIStatic::GetNameList( wstring name )
{
	vector<wstring> ret;
	if( name == L"Picture" )
	{
		int index = 1;
		for each( CKTDGUIControl::CPictureData * pPicture in m_PictureDataList )
		{
			if( NULL != pPicture)
			{
				std::wstringstream wstrstm;
				/*
				if( NULL != pPicture->pTexture )
				{					
					wstrstm << index << L"_" <<  pPicture->pTexture->keyName;
					ret.push_back( wstrstm.str() );
				}
				else
				*/
				{
					wstrstm << index << L"_picture";
					ret.push_back( wstrstm.str() );
				}

				index++;
			}
		}
	}
	else if( name == L"String" )
	{
		int index = 1;
		for each( CKTDGUIControl::UIStringData * pStringData in m_StringDataList )
		{
			if( NULL != pStringData )
			{
				std::wstringstream wstrstm;
				wstrstm << index << L"_" <<  pStringData->msg;
				
				ret.push_back(wstrstm.str());
			}
			index++;
		}
	}

	

	return ret;
}

vector<D3DXVECTOR2> CKTDGUIStatic::GetAddScaleList()
{
	vector<D3DXVECTOR2> ret;
	for( UINT i=0; i<m_PictureDataList.size(); i++ )
	{
		CKTDGUIControl::CPictureData* pPicture = m_PictureDataList[i];
		if( NULL != pPicture )		
		{
			ret.push_back(pPicture->pPoint->addSize);
		}
	}

	return ret;
}

float CKTDGUIStatic::GetRotation( wstring name )
{
	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(name);
	if( NULL != pPictureData )
	{
		if( NULL != pPictureData->pPoint )
		{
			return pPictureData->pPoint->fRotDegree;
			
		}
	}
	return  0.f;
}

void CKTDGUIStatic::Retate( wstring name, float fDegree )
{
	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(name);
	if( NULL != pPictureData )
	{
		if( NULL != pPictureData->pPoint )
		{
			//pPictureData->fRotDegree = fDegree;
			pPictureData->pPoint->leftTopPoint = pPictureData->pos;
			pPictureData->pPoint->SetAutoPointByTextureSize();

			pPictureData->pPoint->fRotDegree = fDegree;
			pPictureData->pPoint->Rotate( fDegree );
		}
	}
}

vector<float> CKTDGUIStatic::GetRotationList()
{
	vector<float> ret;
	for each( CKTDGUIControl::CPictureData * pPicture in m_PictureDataList )
	{
		if( NULL != pPicture &&
			NULL != pPicture->pPoint )
		{
			ret.push_back(pPicture->pPoint->fRotDegree);						
		}
	}

	return ret;
}

bool CKTDGUIStatic::IsSelectByEditGui( POINT pt )
{
	if( true == ContainsPoint(pt) )
		return true;

	/*
	for each( CKTDGUIControl::UIStringData * pStringData in m_StringDataList )
	{
		if( NULL != pStringData )
		{
			Pick2DRect( pt, 
				pStringData->pos, pPictureData->pPoint->rightTopPoint, 
				pPictureData->pPoint->leftBottomPoint, pPictureData->pPoint->rightBottomPoint ) == true )
			{
				return true;
		}
	}
	*/
	return false;

}

vector<wstring> CKTDGUIStatic::GetTextureKeyList( wstring name )
{
	vector<wstring> ret;
	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(name);
	if( NULL != pPictureData )
	{
		CKTDGUIControl::UITextureData * pTextureData = pPictureData->pTexture;
		if( NULL != pTextureData && NULL != pTextureData->pTexture )
		{
			//GetMapTexUVRect
			const CKTDXDeviceTexture::KeyTexUVMap & uvMap = pTextureData->pTexture->GetMapTexUVRect();

			CKTDXDeviceTexture::KeyTexUVMap::const_iterator itor = uvMap.cbegin();
			for( ; itor != uvMap.end() ; itor++)
			{
				ret.push_back(itor->first);
			} 

			return ret;
		}		
	}

	return vector<wstring>();
}

void CKTDGUIStatic::SetTexture( wstring name, wstring fileName )
{
	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(name);
	if( NULL != pPictureData )
	{
		pPictureData->SetTex( fileName.c_str() );
		pPictureData->SetPoint();
	}
}

void CKTDGUIStatic::SetTextureKey( wstring name, wstring key )
{
	int index = 1;
	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(name);
	if( NULL != pPictureData )
	{
		CKTDGUIControl::UITextureData * pTextureData = pPictureData->pTexture;
		if( NULL != pTextureData && NULL != pTextureData->pTexture )
		{
			pTextureData->keyName = key;
			MakeUpperCase(key);

			const CKTDXDeviceTexture::TEXTURE_UV* pTexUV = pTextureData->pTexture->GetTexUV( key );
			if( pTexUV != NULL )
			{
				pTextureData->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]		= pTexUV->leftTop;
				pTextureData->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]	= pTexUV->rightTop;
				pTextureData->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]	= pTexUV->leftBottom;
				pTextureData->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]	= pTexUV->rightBottom;

				pTextureData->texSize = pTexUV->rectSize;

				pTextureData->SetTextureUV();
			}

			if( NULL !=  pPictureData->pPoint )
			{
				pPictureData->pPoint->addSize = D3DXVECTOR2( 0, 0 );
				pPictureData->pPoint->SetAutoPointByTextureSize();
				pPictureData->size.x = pPictureData->pPoint->rightTopPoint.x - pPictureData->pPoint->leftTopPoint.x;
				pPictureData->size.y = pPictureData->pPoint->rightBottomPoint.y - pPictureData->pPoint->leftTopPoint.y;
			}
		}
	}
}

wstring CKTDGUIStatic::GetAttributeData( wstring name )
{
	CKTDGUIControl::CPictureData * pPictureData = _GetPictureData(name);
	if( NULL != pPictureData )
	{
		std::wstringstream wstrstm1;
		if( pPictureData->pTexture != NULL )
			wstrstm1 << pPictureData->pTexture->texName 
			<< L";" << pPictureData->pTexture->keyName
			<< L";" << pPictureData->pos.x
			<< L";" << pPictureData->pos.y
			<< L";" << pPictureData->pPoint->addSize.x
			<< L";" << pPictureData->pPoint->addSize.y;
		else
			wstrstm1 << L";"
			<< L";" << pPictureData->pos.x
			<< L";" << pPictureData->pos.y
			<< L";" << pPictureData->pPoint->addSize.x
			<< L";" << pPictureData->pPoint->addSize.y;

		return wstrstm1.str();
	}	

	int index = 1;
	for each( CKTDGUIControl::UIStringData * pStringData in m_StringDataList )
	{
		if( NULL != pStringData )
		{
			std::wstringstream wstrstm;
			wstrstm << index << L"_" << pStringData->msg;
			if( wstrstm.str() == name )
			{
				std::wstringstream wstrstm1;
				
				wstrstm1 << pStringData->stringTableID 
					<< L";" << pStringData->msg
					<< L";" << pStringData->pos.x
					<< L";" << pStringData->pos.y
					<< L";" << pStringData->fontIndex
					<< L";" << pStringData->fontStyle
					<< L";" << pStringData->sortFlag				
					<< L";" << pStringData->color.r
					<< L";" << pStringData->color.g
					<< L";" << pStringData->color.b
					<< L";" << pStringData->color.a

					<< L";" << pStringData->outlineColor.r
					<< L";" << pStringData->outlineColor.g
					<< L";" << pStringData->outlineColor.b
					<< L";" << pStringData->outlineColor.a;
				
				return wstrstm1.str();
			}
		}

		index++;
	}

	return L"";
}

void CKTDGUIStatic::SetStringTableID( wstring name, int id )
{
	CKTDGUIControl::UIStringData * pStringData = _GetStringData(name);
	if( pStringData )
	{
		pStringData->stringTableID = id;
	}
}

void CKTDGUIStatic::SetString( wstring name, wstring str )
{
	CKTDGUIControl::UIStringData * pStringData = _GetStringData(name);
	if( pStringData )
	{
		pStringData->msg = str;
	}
}

void CKTDGUIStatic::SetFontIndex( wstring name, int index )
{
	CKTDGUIControl::UIStringData * pStringData = _GetStringData(name);
	if( pStringData )
	{
		pStringData->fontIndex = index;
	}
}

void CKTDGUIStatic::SetFontStyle( wstring name, int style )
{
	CKTDGUIControl::UIStringData * pStringData = _GetStringData(name);
	if( pStringData )
	{
		pStringData->fontStyle = (CKTDGFontManager::FONT_STYLE)style;
	}
}

void CKTDGUIStatic::SetFontSort( wstring name, int flag )
{
	CKTDGUIControl::UIStringData * pStringData = _GetStringData(name);
	if( pStringData )
	{
		pStringData->sortFlag = flag;
	}
}

void CKTDGUIStatic::SetFontColor( wstring name, D3DXCOLOR color )
{
	CKTDGUIControl::UIStringData * pStringData = _GetStringData(name);
	if( pStringData )
	{
		pStringData->color = color;
	}
}

void CKTDGUIStatic::SetFontOutlineColor( wstring name, D3DXCOLOR color )
{
	CKTDGUIControl::UIStringData * pStringData = _GetStringData(name);
	if( pStringData )
	{
		pStringData->outlineColor = color;
	}
}

CKTDGUIControl::CPictureData * CKTDGUIStatic::_GetPictureData( wstring name )
{
	int index = 1;
	for each( CKTDGUIControl::CPictureData * pPicture in m_PictureDataList )
	{
		if( NULL != pPicture &&
			NULL != pPicture->pPoint )
		{
			std::wstringstream wstrstm;
			wstrstm << index << L"_picture";
			if( wstrstm.str() == name )
				return pPicture;
		}

		index++;
	}

	return NULL;
}

CKTDGUIControl::UIStringData * CKTDGUIStatic::_GetStringData( wstring name )
{
	int index = 1;
	for each( CKTDGUIControl::UIStringData * pStringData in m_StringDataList )
	{
		if( NULL != pStringData )
		{
			std::wstringstream wstrstm;
			wstrstm << index << L"_" << pStringData->msg;
			if( wstrstm.str() == name )
			{
				return pStringData;
			}
		}

		index++;
	}

	return NULL;
}

#endif

#if defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #ifdef MOVIE_TEST	 	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경
CKTDGUIControl::CMovieData* CKTDGUIStatic::GetMovie( const int index_ )
{
	if( index_ < 0 || index_ >= static_cast<int>( m_MovieDataList.size() ) )
	{
		return NULL;
	}
	else
		return m_MovieDataList[index_];
}
#endif // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #endif //  MOVIE_TEST	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경


